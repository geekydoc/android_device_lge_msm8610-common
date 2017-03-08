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

#define LOG_TAG "DASH - proximity"

#include <stdlib.h>
#include <string.h>
#include "sensors_log.h"
#include <fcntl.h>
#include <linux/input.h>
#include <errno.h>
#include "sensors_list.h"
#include "sensors_fifo.h"
#include "sensors_select.h"
#include "sensor_util.h"
#include "sensors_id.h"
#include "sensors_sysfs.h"

#define PROXIMITY_DEV_NAME "proximity"
#define THRESH_DEFAULT   5
#define PATH_SIZE      100
#define UN_INIT         -1

static int apds9130_init(struct sensor_api_t *s);
static int apds9130_activate(struct sensor_api_t *s, int enable);
static int apds9130_set_delay(struct sensor_api_t *s, int64_t ns);
static void apds9130_close(struct sensor_api_t *s);
static void *apds9130_read(void *arg);

struct sensor_desc {
	struct sensors_select_t select_worker;
	struct sensors_sysfs_t sysfs;
	struct sensor_t sensor;
	struct sensor_api_t api;

	float distance;
	int64_t delay;

	int th_det;
	int th_not_det;
	char th_path[PATH_SIZE];
};

static struct sensor_desc apds970x = {
	.sensor = {
		.name = PROXIMITY_SENSOR_NAME,
		.vendor = "Avago Technologies",
		.version = sizeof(sensors_event_t),
		.handle = SENSOR_PROXIMITY_HANDLE,
		.type = SENSOR_TYPE_PROXIMITY,
		.maxRange = 1.0,
		.resolution = 1.0,
		.power = 2
	},
	.api = {
		.init = apds9130_init,
		.activate = apds9130_activate,
		.set_delay = apds9130_set_delay,
		.close = apds9130_close
	},
	.th_not_det = UN_INIT,
};


static int apds9130_init(struct sensor_api_t *s)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	int fd;

	/* check for availability */
	fd = open_input_dev_by_name(PROXIMITY_DEV_NAME, O_RDONLY | O_NONBLOCK);
	if (fd < 0) {
		ALOGE("%s: unable to find %s input device!\n", __func__,
			PROXIMITY_DEV_NAME);
		return fd;
	}
	close(fd);

	sensors_sysfs_init(&d->sysfs, PROXIMITY_DEV_NAME, SYSFS_TYPE_INPUT_DEV);
	sensors_select_init(&d->select_worker, apds9130_read, s, -1);
	return 0;
}

static int apds9130_activate(struct sensor_api_t *s, int enable)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	int fd = d->select_worker.get_fd(&d->select_worker);
	int ret;

	if (enable && (fd < 0)) {
		fd = open_input_dev_by_name(PROXIMITY_DEV_NAME,
			O_RDONLY | O_NONBLOCK);
		if (fd < 0) {
			ALOGE("%s: failed to open input dev %s\n", __func__,
				PROXIMITY_DEV_NAME);
			return fd;
		}
	
		/* Notify kernel-space */
		ret = d->sysfs.write_int(&d->sysfs, "enable", 1);
		if (ret < 0) {
			ALOGE("Enabling Proximity sensor failed: %s\n", strerror(-ret));
			return ret;
		}

		d->select_worker.set_fd(&d->select_worker, fd);
		d->select_worker.resume(&d->select_worker);
	} else if (!enable && (fd > 0)) {

		/* Notify kernel-space */
		ret = d->sysfs.write_int(&d->sysfs, "enable", 0);
		if (ret < 0) {
			ALOGE("Disabling Proximity sensor failed: %s\n", strerror(-ret));
			return ret;
		}
		d->select_worker.set_fd(&d->select_worker, -1);
		d->select_worker.suspend(&d->select_worker);
	}

	return 0;
}

static int apds9130_set_delay(struct sensor_api_t *s, int64_t ns)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);

	d->delay = ns;
	d->select_worker.set_delay(&d->select_worker, ns);

	return 0;
}

static void apds9130_close(struct sensor_api_t *s)
{
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);

	d->select_worker.destroy(&d->select_worker);
}

static void apds9130_store_dist(struct sensor_desc *d, int value)
{
	d->distance = value ? 1:0;
}

static void *apds9130_read(void *arg)
{
	struct sensor_api_t *s = arg;
	struct sensor_desc *d = container_of(s, struct sensor_desc, api);
	struct input_event event;
	sensors_event_t data;
	int ret;
	int fd = d->select_worker.get_fd(&d->select_worker);

	while ((ret = read(fd, &event, sizeof(event))) > 0) {
		switch (event.type) {
		case EV_MSC:
			if (event.code == MSC_RAW)
				apds9130_store_dist(d, event.value);
			break;
		case EV_ABS:
			if (event.code == ABS_DISTANCE)
				apds9130_store_dist(d, event.value);
			break;
		case EV_SYN:
			memset(&data, 0, sizeof(data));

			data.distance = d->distance;

			data.version = apds970x.sensor.version;
			data.sensor = apds970x.sensor.handle;
			data.type = apds970x.sensor.type;
			data.timestamp = get_current_nano_time();

			sensors_fifo_put(&data);
			break;
		default:
			break;
		}
	}

	return NULL;
}

list_constructor(apds9130_init_driver);
void apds9130_init_driver()
{
	(void)sensors_list_register(&apds970x.sensor, &apds970x.api);
}
