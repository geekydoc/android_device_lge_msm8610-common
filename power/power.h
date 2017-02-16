/*
 * Copyright (C) 2015 The CyanogenMod Project
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

enum {
    PROFILE_POWER_SAVE = 0,
    PROFILE_BALANCED,
    PROFILE_HIGH_PERFORMANCE,
	PROFILE_MAX
};

typedef struct governor_settings {
    int is_interactive;
    int boost;
    int boostpulse_duration;
    int go_hispeed_load;
    int go_hispeed_load_off;
    int hispeed_freq;
    int hispeed_freq_off;
    int timer_rate;
    int timer_rate_off;
    char *above_hispeed_delay;
    int io_is_busy;
    int min_sample_time;
    int sampling_down_factor;
    char *target_loads;
    char *target_loads_off;
} power_profile;

static power_profile profiles[PROFILE_MAX] = {
    [PROFILE_POWER_SAVE] = {
		.boost = 0,
        .boostpulse_duration = 200000,
        .go_hispeed_load = 80,
        .go_hispeed_load_off = 110,
        .hispeed_freq = 787200,
        .hispeed_freq_off = 787200,
        .timer_rate = 20000,
        .timer_rate_off = 20000,
        .above_hispeed_delay = "80000",
        .io_is_busy = 1,
        .min_sample_time = 60000,
        .sampling_down_factor = 100000,
        .target_loads = "90",
        .target_loads_off = "95",
    },
    [PROFILE_BALANCED] = {
        .boost = 0,
        .boostpulse_duration = 800000,
        .go_hispeed_load = 95,
        .go_hispeed_load_off = 60,
        .hispeed_freq = 787200,
        .hispeed_freq_off = 787200,
        .timer_rate = 35000,
        .timer_rate_off = 20000,
        .above_hispeed_delay = "10000 787200:40000",
        .io_is_busy = 0,
        .min_sample_time = 40000,
        .sampling_down_factor = 100000,
        .target_loads = "65 300000:90 787000:75",
        .target_loads_off = "80",
    },
    [PROFILE_HIGH_PERFORMANCE] = {
        .boost = 0,
        .boostpulse_duration = 0, /* prevent unnecessary write */
        .go_hispeed_load = 50,
        .go_hispeed_load_off = 110,
        .hispeed_freq = 1190400,
        .hispeed_freq_off = 1190400,
        .timer_rate = 20000,
        .timer_rate_off = 20000,
        .above_hispeed_delay = "20000",
        .io_is_busy = 1,
        .min_sample_time = 60000,
        .sampling_down_factor = 100000,
        .target_loads = "80",
        .target_loads_off = "80",
    },
};
