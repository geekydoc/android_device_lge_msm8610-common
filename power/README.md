

CUSTOM POWER/CPU MANAGEMENT INTRODUCTION
----------------------------------------


This project attemps to use a custom Power and CPU management combo to get even better battery life on L Fino, while taking advantage of the full power of the quad cortex-A7 cores that the platform comes with. From Marshmallow onwards, battery life while the device is in a sleep state has increased drastically, due to the introduction of the Doze Mode. On the other hand, battery consumption on heavy device usage is big. After some intense testing it turns out that, with some proper configuration/tweaking, battery consumption on heavy device usage can be reduced too. 

**Kernel Space Cpu Hotplug Drivers**

Getting the cpu hotplugging code in kernel space, offers smaller execution times as well as greater configuration options. The kernel for this project integrates the intelliplug hotplug driver by faux123, which plugs/unplugs cores based on execution load. A special interactive governor tuning has also been done, in order to utilize mostly lower but also less power hungry cpu frequencies (see below). This change eliminates the need for the closed-source mpdecision daemon by qualcomm, which was dropped in from a different device (and most probably tuned for it by qualcomm developers). The above change is accompanied by a kernel-space thermal driver, intellithermal (again by faux123), which substitutes the closed-source thermal-engine by qualcomm.

**The PowerHAL**

By eliminating mpdecision,  Qualcomm's PowerHAL which is designed to work with it, becomes unusable. This project uses a customized version of Nick D2's PowerHAL for msm8226 devices, further customized to integrate with the kernel-space hotplug driver.
The PowerHAL supports the following power profiles: 

 - POWERSAVE
 - BALANCED
 - PERFORMANCE

The device boots into BALANCED power profile by default. While on this power profile, the tuned interactive governor will try to use the most power efficient cpu frequencies when possible. When the screen is on and the device is idle, it will settle most times to 300Mhz, with only one core running. The high_speed frequency is set to 787Mhz: when more power is needed, the governor will ramp up to 787Mhz first and, if that is not enough, ramp up to higher frequencies. This tuning can produce lag in some situations. To avoid this, input boost is enabled through the kernel cpu-boost driver. Touching the screen will boost the frequency to 1.2Ghz for 1000ms to provide a lag-free experience.
On POWERSAVE power profile (set when the powersaver is on), the PowerHAL will limit the maximum cores to 2, limit the maximum cpu frequency to 787Mhz, change the hotplug profile to "eco" and disable input boost. POWERSAVE should be used to extend battery life when the device is low on power in the expense of lower system responsiveness, so be prepared for some lag.
On PERFORMANCE power profile, the device will give everything it can, without caring to preserve power supplies.

On all power profiles, some special tuning to the cpu governor is done when the screen is off. Also, in this scenario, the device is limited to using only the first cpu core. This is enough for doing tasks such as listening to music or FM radio. This design is made in an effort to save power when the screen is off but the device does not enter doze mode. 

This PowerHAL is also customised to support special features such as native Tap to Wake.

FAQS:
-----

**Can I save some more battery with this?**

You can set the input boost manually to 787Mhz, which is a more power efficient frequency, with acceptable device responsiveness. If you do not care about lag in some situations, you can disable this completely. You can change the input boost values through an app like Kernel Adiutor, under the cpu settings.

**Is it possible to tune this more?**

This setup is very tunable, although the presence of the PowerHAL limits some options. If you are interested in changing some values, you will have to install an app that can change kernel parameters, like Kernel Adiutor. You can change any values you want, but while the PowerHAL is in use, it will change back governor values and hotplug parameters to the ones hard-coded in it when the power profile is changed or the screen is powered on/off. To avoid this you can disable the PowerHAL completely by renaming the library at /system/lib/hw/power.msm8610.so to power.msm8610.so.old and rebooting. **Note** that after this, the device will boot without any hotplug or thermal driver enabled, you will have to manualy enable and configure them. If you are into coding, you can also easily change some hard coded values in the PowerHAL library and rebuild it if you think that you have found a good configuration. In this case, please share your changes with us!

This is considered a work in progress, so expect configuration to be changed as more experience is gathered. If you experience issues or have proposals, please share them with us.
