/*
 * Copyright (C) 2012 Sony Mobile Communications AB.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define LOG_TAG "DASH - ak9911"

#include <stdlib.h>
#include <string.h>
#include <linux/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <linux/input.h>
#include <pthread.h>
#include <errno.h>
#include "sensors_log.h"
#include "sensors_list.h"
#include "sensors_fifo.h"
#include "sensors_select.h"
#include "sensor_util.h"
#include "sensors_id.h"
#include "sensors_config.h"
#include "sensors_sysfs.h"

#define EVENT_CODE_MAGV_X ABS_X
#define EVENT_CODE_MAGV_Y ABS_Y
#define EVENT_CODE_MAGV_Z ABS_Z


static const char *ak9911_sysfs_path = "/sys/class/sensors/akm09911-mag";

enum {
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	NUM_AXIS
};

static int ak9911_init(struct sensor_api_t *s);
static int ak9911_activate(struct sensor_api_t *s, int enable);
static int ak9911_set_delay(struct sensor_api_t *s, int64_t ns);
static void ak9911_close(struct sensor_api_t *s);
static void *ak9911_read(void *arg);

struct sensor_desc {
	struct sensor_t sensor;
	struct sensor_api_t api;
	struct sensors_sysfs_t sysfs;
	struct sensors_select_t select_worker;
	float scale;
	int data[NUM_AXIS];
	int map[NUM_AXIS];
	int sign[NUM_AXIS];
	char *map_prefix;
	int64_t delay;
	pthread_mutex_t lock;
	char *input_name;
};

static struct sensor_desc ak9911_compass = {
	.sensor = {
		name: AKM_CHIP_NAME" Magnetic Field",
		vendor: "Asahi Kasei Corp.",
		version: sizeof(sensors_event_t),
		handle: SENSOR_MAGNETIC_FIELD_HANDLE,
		type: SENSOR_TYPE_MAGNETIC_FIELD,
		maxRange: 4912,
		resolution: 0.06,
		power: 0.24,
	},
	.api = {
		init: ak9911_init,
		activate: ak9911_activate,
		set_delay: ak9911_set_delay,
		close: ak9911_close,
	},
	.map_prefix = "ak9911magnetic",
	.map = {2, 1, 0},
	.sign = {-1, 1, 1},
	.scale = 0.56,
	.input_name = "compass",

};

struct config_record {
	int max;
	int min;
	int size;
	int *store;
};

static int ak9911_read_axis(char *prefix, char *key, struct config_record *cr)
{
	int rc = -1;
	int tmp[NUM_AXIS];

	if (!sensors_config_get_key(prefix, key, TYPE_ARRAY_INT, tmp, cr->size)) {
		int i;
		for (i = 0; i < cr->size; i++) {
			if (tmp[i] < cr->min || tmp[i] > cr->max)
				break;
		}
		if (i == cr->size) {
			memcpy(cr->store, tmp, sizeof(tmp));
			rc = 0;
		} else {
			ALOGE("%s: bad config (%s_%s) [%2d %2d %2d]", __func__,
					prefix, key, tmp[AXIS_X], tmp[AXIS_Y],
					tmp[AXIS_Z]);
		}
	} else {
		ALOGE("%s: failed to read %s_%s", __func__, prefix, key);
	}
	return rc;
}

static void ak9911_read_sensor_map(struct sensor_desc *d)
{
	struct config_record rec;

	if (!sensors_have_config_file()) {
		ALOGI("%s: No config file found. Using default config.",
		     __func__);
		return;
	}
	rec.size = NUM_AXIS;
	rec.min = AXIS_X;
	rec.max = AXIS_Z;
	rec.store = d->map;
	ak9911_read_axis(d->map_prefix, "axis_map", &rec);

	rec.min = -1;
	rec.max = 1;
	rec.store = d->sign;
	ak9911_read_axis(d->map_prefix, "axis_sign", &rec);

	ALOGD("%s: %s: axis map [%2d %2d %2d]", __func__, d->sensor.name,
	     d->map[AXIS_X], d->map[AXIS_Y], d->map[AXIS_Z]);
	ALOGD("%s: %s: axis sign [%2d %2d %2d]", __func__, d->sensor.name,
	     d->sign[AXIS_X], d->sign[AXIS_Y], d->sign[AXIS_Z]);
}

static int ak9911_set_delay(struct sensor_api_t *s, int64_t ns)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);

	if (ns < 10000000) {
		ns = 10000000;
	}
	
	/* Update Kernel Driver */
	d->sysfs.write_int(&d->sysfs, "poll_delay", ns/1000000);

	d->delay = ns;
	d->select_worker.set_delay(&d->select_worker, ns);

	return 0;
}

static void ak9911_compass_register(struct sensor_desc *d)
{
	pthread_mutex_init(&d->lock, NULL);
	sensors_list_register(&d->sensor, &d->api);
}

static int ak9911_init(struct sensor_api_t *s_api)
{
	int fd;
	struct sensor_desc *d = container_of(s_api, struct sensor_desc, api);

	/* check for availablity */
	fd = open_input_dev_by_name(d->input_name, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		ALOGE("%s: unable to find %s input device!\n", __func__,
			d->input_name);
		return -1;
	}
	close(fd);

	ak9911_read_sensor_map(&d->sensor);
	sensors_sysfs_init(&d->sysfs, ak9911_sysfs_path, SYSFS_TYPE_ABS_PATH);
	sensors_select_init(&d->select_worker, ak9911_read, d, -1);

	return 0;
}

static void ak9911_close(struct sensor_api_t *s)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	d->select_worker.destroy(&d->select_worker);
}

static int ak9911_activate(struct sensor_api_t *s, int enable)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	int ret = 0;
	int fd = d->select_worker.get_fd(&d->select_worker);
	
	/* Enable kernel driver */
	d->sysfs.write_int(&d->sysfs, "enable", enable);

	if (enable && (fd < 0)) {
		fd = open_input_dev_by_name(d->input_name,
						O_RDONLY | O_NONBLOCK);
		if (fd < 0) {
			ALOGE("%s: Failed to open input device %s", __func__,
				d->input_name);
			ret = -1;
			goto exit;
		}

		d->select_worker.set_fd(&d->select_worker, fd);
		d->select_worker.resume(&d->select_worker);
	} else if (!enable && (fd > 0)) {
		d->select_worker.suspend(&d->select_worker);
		d->select_worker.set_fd(&d->select_worker, -1);
	}

exit:
	return ret;
}

static void *ak9911_read(void *arg)
{
	struct input_event evbuf[10];
	struct input_event *event;
	struct sensor_desc *d = arg;
	int fd = d->select_worker.get_fd(&d->select_worker);
	sensors_event_t sdata;
	int n;
	int i;

	memset(&sdata, 0, sizeof(sdata));

	pthread_mutex_lock(&d->lock);
	n = read(fd, evbuf, sizeof(evbuf)) / sizeof(evbuf[0]);
	for (i = 0; i < n; i++) {
		event = evbuf + i;
		if (event->type == EV_SYN) {
			sdata.version = d->sensor.version;
			sdata.sensor = d->sensor.handle;
			sdata.type = d->sensor.type;
			sdata.timestamp = get_current_nano_time();
			sdata.magnetic.x = d->data[d->map[0]] * d->sign[0] * d->scale;
			sdata.magnetic.y = d->data[d->map[1]] * d->sign[1] * d->scale;
			sdata.magnetic.z = d->data[d->map[2]] * d->sign[2] * d->scale;
			sensors_fifo_put(&sdata);
			continue;
		}
		if (event->type != EV_ABS)
			continue;
		switch (event->code) {
			case EVENT_CODE_MAGV_X:
				d->data[0] = event->value;
				break;
			case EVENT_CODE_MAGV_Y:
				d->data[1] = event->value;
				break;
			case EVENT_CODE_MAGV_Z:
				d->data[2] = event->value;
				break;
		}
	}
	pthread_mutex_unlock(&d->lock);

	return NULL;
}

list_constructor(ak9911_init_driver);
void ak9911_init_driver()
{
	ak9911_compass_register(&ak9911_compass);
}

