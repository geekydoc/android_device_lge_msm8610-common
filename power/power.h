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
    int cpu_down_rate;
    int cpu_up_rate;
	int down_differential; 
	int freq_for_responsiveness;
	int freq_step;
	int hotplug_freq_1_1;
	int hotplug_freq_2_0;    
	int hotplug_freq_2_1;
	int hotplug_freq_3_0;
	int hotplug_freq_3_1;
	int hotplug_freq_4_0;
	int hotplug_rq_1_1;
	int hotplug_rq_2_0;
	int hotplug_rq_2_1;
	int hotplug_rq_3_0;
	int hotplug_rq_3_1;
	int hotplug_rq_4_0;
	int ignore_nice_load;
	int io_is_busy;
	int sampling_down_factor;
    int sampling_rate;
	int sampling_rate_min; 
    int up_threshold;
    int up_threshold_at_min_freq;               
} power_profile;

static power_profile profiles[PROFILE_MAX] = {
    [PROFILE_POWER_SAVE] = {
    	.cpu_down_rate = 5,
    	.cpu_up_rate = 15,
		.down_differential = 1,
		.freq_for_responsiveness = 787200,
		.freq_step = 50,
		.hotplug_freq_1_1 = 1094400,
		.hotplug_freq_2_0 = 787200,
		.hotplug_freq_2_1 = 1094400,
		.hotplug_freq_3_0 = 787200,
		.hotplug_freq_3_1 = 1094400,
		.hotplug_freq_4_0 = 787200,
		.hotplug_rq_1_1 = 350,
		.hotplug_rq_2_0 = 200,
		.hotplug_rq_2_1 = 350,
		.hotplug_rq_3_0 = 200,
		.hotplug_rq_3_1 = 350,
		.hotplug_rq_4_0 = 200,
		.ignore_nice_load = 1,
		.io_is_busy = 0,
		.sampling_down_factor = 2,
	    .sampling_rate = 80000,
		.sampling_rate_min = 60000,
		.up_threshold = 98,
		.up_threshold_at_min_freq = 90,
    },
    [PROFILE_BALANCED] = {
    	.cpu_down_rate = 5,
    	.cpu_up_rate = 15,
		.down_differential = 2,
		.freq_for_responsiveness = 384000,
		.freq_step = 20,
		.hotplug_freq_1_1 = 787200,
		.hotplug_freq_2_0 = 384000,
		.hotplug_freq_2_1 = 1094400,
		.hotplug_freq_3_0 = 600000,
		.hotplug_freq_3_1 = 1094400,
		.hotplug_freq_4_0 = 600000,
		.hotplug_rq_1_1 = 350,
		.hotplug_rq_2_0 = 150,
		.hotplug_rq_2_1 = 350,
		.hotplug_rq_3_0 = 200,
		.hotplug_rq_3_1 = 300,
		.hotplug_rq_4_0 = 200,
		.ignore_nice_load = 1,
		.io_is_busy = 1,
		.sampling_down_factor = 2,
	    .sampling_rate = 70000,
		.sampling_rate_min = 30000,
		.up_threshold = 95,
		.up_threshold_at_min_freq = 85,
    },
    [PROFILE_HIGH_PERFORMANCE] = {
    	.cpu_down_rate = 30,
    	.cpu_up_rate = 10,
		.down_differential = 15,
		.freq_for_responsiveness = 384000,
		.freq_step = 50,
		.hotplug_freq_1_1 = 384000,
		.hotplug_freq_2_0 = 300000,
		.hotplug_freq_2_1 = 384000,
		.hotplug_freq_3_0 = 300000,
		.hotplug_freq_3_1 = 384000,
		.hotplug_freq_4_0 = 300000,
		.hotplug_rq_1_1 = 100,
		.hotplug_rq_2_0 = 10,
		.hotplug_rq_2_1 = 100,
		.hotplug_rq_3_0 = 10,
		.hotplug_rq_3_1 = 100,
		.hotplug_rq_4_0 = 10,
		.ignore_nice_load = 0,
		.io_is_busy = 1,
		.sampling_down_factor = 2,
	    .sampling_rate = 70000,
		.sampling_rate_min = 30000,
		.up_threshold = 80,
		.up_threshold_at_min_freq = 65,
    },
};
