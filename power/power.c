/*
 * Copyright (C) 2016 The CyanogenMod Project
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
#define LOG_TAG "PowerHAL"

#include <hardware/hardware.h>
#include <hardware/power.h>

#include <stdbool.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <utils/Log.h>

#include "power.h"

#define GOV_PATH "/sys/devices/system/cpu/cpufreq/pegasusq/"

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static int boostpulse_fd = -1;

static int current_power_profile = -1;
static int requested_power_profile = -1;

static int sysfs_write_str(char *path, char *s)
{
    char buf[80];
    int len;
    int ret = 0;
    int fd;

    fd = open(path, O_WRONLY);
    if (fd < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error opening %s: %s\n", path, buf);
        return -1 ;
    }

    len = write(fd, s, strlen(s));
    if (len < 0) {
        strerror_r(errno, buf, sizeof(buf));
        ALOGE("Error writing to %s: %s\n", path, buf);
        ret = -1;
    }

    close(fd);

    return ret;
}

static int sysfs_write_int(char *path, int value)
{
    char buf[80];
    snprintf(buf, 80, "%d", value);
    return sysfs_write_str(path, buf);
}

static bool check_governor(void)
{
    struct stat s;
    int err = stat(GOV_PATH, &s);
    if (err != 0) return false;
    if (S_ISDIR(s.st_mode)) return true;
    return false;
}

static int is_profile_valid(int profile)
{
    return profile >= 0 && profile < PROFILE_MAX;
}

static void power_init(__attribute__((unused)) struct power_module *module)
{
    ALOGI("%s", __func__);
}		

static void power_set_interactive(__attribute__((unused)) struct power_module *module, int on)
{
    ALOGI("Set_Interactive is not supported on the current PowerHAL");
}

static void set_power_profile(int profile)
{
    if (!is_profile_valid(profile)) {
        ALOGE("%s: unknown profile: %d", __func__, profile);
        return;
    }

    // break out early if governor is not pegasusq
    if (!check_governor()) return;

    if (profile == current_power_profile)
        return;

    ALOGD("%s: setting profile %d", __func__, profile);

    sysfs_write_int(GOV_PATH "cpu_down_rate",
                    profiles[profile].cpu_down_rate);
    sysfs_write_int(GOV_PATH "cpu_up_rate",
                    profiles[profile].cpu_up_rate);
    sysfs_write_int(GOV_PATH "down_differential",
                    profiles[profile].down_differential);
    sysfs_write_int(GOV_PATH "freq_for_responsiveness",
                    profiles[profile].freq_for_responsiveness);
    sysfs_write_int(GOV_PATH "freq_step",
                    profiles[profile].freq_step);
    sysfs_write_int(GOV_PATH "hotplug_freq_1_1",
                    profiles[profile].hotplug_freq_1_1);
    sysfs_write_int(GOV_PATH "hotplug_freq_2_0",
                    profiles[profile].hotplug_freq_2_0);
    sysfs_write_int(GOV_PATH "hotplug_freq_2_1",
                    profiles[profile].hotplug_freq_2_1);
    sysfs_write_int(GOV_PATH "hotplug_freq_3_0",
                    profiles[profile].hotplug_freq_3_0);
    sysfs_write_int(GOV_PATH "hotplug_freq_3_1",
                    profiles[profile].hotplug_freq_3_1);
    sysfs_write_int(GOV_PATH "hotplug_freq_4_0",
                    profiles[profile].hotplug_freq_4_0);
    sysfs_write_int(GOV_PATH "hotplug_rq_1_1",
                    profiles[profile].hotplug_rq_1_1);
    sysfs_write_int(GOV_PATH "hotplug_rq_2_0",
                    profiles[profile].hotplug_rq_2_0);
    sysfs_write_int(GOV_PATH "hotplug_rq_2_1",
                    profiles[profile].hotplug_rq_2_1);
    sysfs_write_int(GOV_PATH "hotplug_rq_3_0",
                    profiles[profile].hotplug_rq_3_0);
    sysfs_write_int(GOV_PATH "hotplug_rq_3_1",
                    profiles[profile].hotplug_rq_3_1);
    sysfs_write_int(GOV_PATH "hotplug_rq_4_0",
                    profiles[profile].hotplug_rq_4_0);
    sysfs_write_int(GOV_PATH "ignore_nice_load",
                    profiles[profile].ignore_nice_load);
    sysfs_write_int(GOV_PATH "io_is_busy",
                    profiles[profile].io_is_busy);
    sysfs_write_int(GOV_PATH "sampling_down_factor",
                    profiles[profile].sampling_down_factor);
    sysfs_write_int(GOV_PATH "sampling_rate",
                    profiles[profile].sampling_rate);
    sysfs_write_int(GOV_PATH "sampling_rate_min",
                    profiles[profile].sampling_rate_min);
    sysfs_write_int(GOV_PATH "up_threshold",
                    profiles[profile].up_threshold);
    sysfs_write_int(GOV_PATH "up_threshold_at_min_freq",
                    profiles[profile].up_threshold_at_min_freq);
    current_power_profile = profile;  
}

static void power_hint(__attribute__((unused)) struct power_module *module,
                       power_hint_t hint, void *data)
{
    char buf[80];
    int len;

    switch (hint) {
    case POWER_HINT_LAUNCH:
    case POWER_HINT_CPU_BOOST:
        break;
    case POWER_HINT_SET_PROFILE:
        pthread_mutex_lock(&lock);
        set_power_profile(*(int32_t *)data);
        pthread_mutex_unlock(&lock);
        break;
    case POWER_HINT_LOW_POWER:
        /* This hint is handled by the framework */
        break;
    default:
        break;
    }
}

static struct hw_module_methods_t power_module_methods = {
    .open = NULL,
};

void set_feature(struct power_module *module, feature_t feature, int state)
{
#ifdef TAP_TO_WAKE_NODE
    if (feature == POWER_FEATURE_DOUBLE_TAP_TO_WAKE) {
        sysfs_write_str(TAP_TO_WAKE_NODE, state ? "1":"0");
        return;
    }
#endif
    return;
}

static int get_feature(__attribute__((unused)) struct power_module *module,
                       feature_t feature)
{
    if (feature == POWER_FEATURE_SUPPORTED_PROFILES) {
        return PROFILE_MAX;
    }
    return -1;
}

struct power_module HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = POWER_MODULE_API_VERSION_0_2,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = POWER_HARDWARE_MODULE_ID,
        .name = "msm8610 custom Power HAL",
        .author = "CyanogenMod/LineageOS/geekydoc",
        .methods = &power_module_methods,
    },

    .init = power_init,
    .setInteractive = power_set_interactive,
    .powerHint = power_hint,
	.setFeature = set_feature,
    .getFeature = get_feature
};
