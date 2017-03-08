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

#define LOG_TAG "DASH - K3DH"

#include <string.h>
#include "sensors_log.h"
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#include "sensors_list.h"
#include "sensors_fifo.h"
#include "sensors_select.h"
#include "sensor_util.h"
#include "sensors_id.h"
#include "sensors_config.h"
#include "sensors_sysfs.h"

#define K3DH_INPUT_NAME "accelerometer"

#define VALID_HANDLE(h) ((h) > CLIENT_ANDROID && (h) < MAX_CLIENTS)


static int k3dh_input_init(struct sensor_api_t *s);
static int k3dh_input_activate(struct sensor_api_t *s, int enable);
static int k3dh_input_set_delay(struct sensor_api_t *s, int64_t ns);
static void k3dh_input_close(struct sensor_api_t *s);
static void *k3dh_input_read(void *arg);

struct sensor_desc {
	struct sensors_select_t select_worker;
	struct sensors_sysfs_t sysfs;
	struct sensor_t sensor;
	struct sensor_api_t api;

	int input_fd;
	float current_data[3];
	int64_t delay;

	/* config options */
	int axis_x;
	int axis_y;
	int axis_z;

	int neg_x;
	int neg_y;
	int neg_z;
};

static struct sensor_desc k3dh_input = {
	.sensor = {
		name: "k3dh accelerometer",
		vendor: "ST microdevices",
		version: sizeof(sensors_event_t),
		handle: SENSOR_ACCELEROMETER_HANDLE,
		type: SENSOR_TYPE_ACCELEROMETER,
		maxRange: 19.613, 
		resolution: 0.005,
		power: 0.23,
		minDelay: 500000
	},
	.api = {
		init: k3dh_input_init,
		activate: k3dh_input_activate,
		set_delay: k3dh_input_set_delay,
		close: k3dh_input_close
	},
	.input_fd = -1,
	.axis_x = 0,
	.axis_y = 1,
	.axis_z = 2,
	.neg_x = 0,
	.neg_y = 0,
	.neg_z = 0
};

static inline float ev2grav(int evv)
{
	return (float)evv / 100;
}

static void k3dh_input_read_config(struct sensor_desc *d)
{
	int conf_axis_x, conf_axis_y, conf_axis_z;
	int conf_neg_x, conf_neg_y, conf_neg_z;
	struct {
		char *key;
		int min;
		int max;
		int *store;
	} conf_values[] = {
		{ "axis_x", 0, 2, &conf_axis_x },
		{ "axis_y", 0, 2, &conf_axis_y },
		{ "axis_z", 0, 2, &conf_axis_z },

		{ "neg_x", 0, 1, &conf_neg_x },
		{ "neg_y", 0, 1, &conf_neg_y },
		{ "neg_z", 0, 1, &conf_neg_z },
	};
	unsigned int i;

	if (!sensors_have_config_file()) {
		ALOGI("%s: No config file found: using default config.",
		     __func__);
		return;
	}

	for (i = 0; i < (sizeof(conf_values)/sizeof(conf_values[0])); i++) {
		int value;
		if (sensors_config_get_key("k3dhinput", conf_values[i].key,
					   TYPE_INT,
					   (void*)&value,
					   sizeof(value)) < 0) {
			ALOGE("%s: failed to read %s", __func__, conf_values[i].key);
			return;
		}

		if ((value < conf_values[i].min) || (value > conf_values[i].max)) {
			ALOGE("%s: %s value out of bounds: %d\n", __func__,
			     conf_values[i].key, value);
			return;
		}

		*conf_values[i].store = value;
	}

	d->axis_x = conf_axis_x;
	d->axis_y = conf_axis_y;
	d->axis_z = conf_axis_z;

	d->neg_x = conf_neg_x;
	d->neg_y = conf_neg_y;
	d->neg_z = conf_neg_z;
}

static int k3dh_input_init(struct sensor_api_t *s)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	int fd;
	k3dh_input_read_config(d);

	fd = open_input_dev_by_name(K3DH_INPUT_NAME, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		ALOGE("%s: failed to open input dev %s, error: %s\n",
			__func__, K3DH_INPUT_NAME, strerror(errno));
		return -1;
	}
	close(fd);

	sensors_sysfs_init(&d->sysfs, K3DH_INPUT_NAME, SYSFS_TYPE_INPUT_DEV);
	sensors_select_init(&d->select_worker, k3dh_input_read, s, -1);

	return 0;
}

static int k3dh_input_activate(struct sensor_api_t *s, int enable)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	int fd = d->select_worker.get_fd(&d->select_worker);
	int ret;

	if (enable && (fd < 0)) {
		fd = open_input_dev_by_name(K3DH_INPUT_NAME,
			O_RDONLY | O_NONBLOCK);
		if (fd < 0) {
			ALOGE("%s: failed to open input dev %s, error: %s\n",
				__func__, K3DH_INPUT_NAME, strerror(errno));
			return -1;
		}
		
		/* Notify kernel-space */
		ret = d->sysfs.write_int(&d->sysfs, "enable", 1);
		if (ret < 0) {
			ALOGE("Enabling K3DH polling failed: %s\n", strerror(-ret));
			return ret;
		}
		d->select_worker.set_fd(&d->select_worker, fd);
		d->select_worker.resume(&d->select_worker);
	} else if (!enable && (fd > 0)) {
		
		/* Notify kernel-space */
		ret = d->sysfs.write_int(&d->sysfs, "enable", 0);
		if (ret < 0) {
			ALOGE("Disabling K3DH polling failed: %s\n", strerror(-ret));
			return ret;
		}
		d->select_worker.set_fd(&d->select_worker, -1);
		d->select_worker.suspend(&d->select_worker);
	}
	return 0;
}

static int k3dh_input_set_delay(struct sensor_api_t *s, int64_t ns)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	int ret;

	if (ns < d->sensor.minDelay) {
		ns = d->sensor.minDelay;
	}

	d->delay = ns;
	d->select_worker.set_delay(&d->select_worker, ns);

	/* Update SysFS. On K3DH, the kernel driver will calculate the nearest supported polling delay and configure the device accordingly. Value should be in millis */ 
	unsigned int ms = ns/(1000*1000);
	ret = d->sysfs.write_int(&d->sysfs, "poll_delay", ms);
	if (ret < 0) {
		ALOGE("updating K3DH polling delay failed: %s\n", strerror(-ret));
		return ret;
	}

	return ret;
}

static void k3dh_input_close(struct sensor_api_t *s)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);

	d->select_worker.destroy(&d->select_worker);
}

static void *k3dh_input_read(void *arg)
{
	struct sensor_api_t *s = arg;
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	struct input_event event;
	int fd = d->select_worker.get_fd(&d->select_worker);
	sensors_event_t data;

	while (read(fd, &event, sizeof(event)) > 0) {
		switch (event.type) {
		case EV_ABS:
			switch (event.code) {
			case ABS_X:
				d->current_data[0] = ev2grav(event.value);
				break;

			case ABS_Y:
				d->current_data[1] = ev2grav(event.value);
				break;

			case ABS_Z:
				d->current_data[2] = ev2grav(event.value);
				break;

			default:
				ALOGE("%s: unknown event code 0x%X\n",
					__func__, event.code);
				goto exit;
			}
			break;

		case EV_SYN:
			data.acceleration.x = (d->neg_x ? -d->current_data[d->axis_x] :
						d->current_data[d->axis_x]);
			data.acceleration.y = (d->neg_y ? -d->current_data[d->axis_y] :
						d->current_data[d->axis_y]);
			data.acceleration.z = (d->neg_z ? -d->current_data[d->axis_z] :
						d->current_data[d->axis_z]);
			data.acceleration.status = SENSOR_STATUS_ACCURACY_HIGH;

			data.sensor = k3dh_input.sensor.handle;
			data.type = k3dh_input.sensor.type;
			data.version = k3dh_input.sensor.version;
			data.timestamp = get_current_nano_time();

			sensors_fifo_put(&data);
			goto exit;

		default:
			ALOGE("%s: unknown event type 0x%X\n",
				__func__, event.type);
			goto exit;
		}
	}

exit:
	return NULL;
}

list_constructor(k3dh_input_init_driver);
void k3dh_input_init_driver()
{
	(void)sensors_list_register(&k3dh_input.sensor, &k3dh_input.api);
}
