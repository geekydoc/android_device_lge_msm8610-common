#
# Copyright (C) 2013 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

PRODUCT_PROPERTY_OVERRIDES += \
    dalvik.vm.heapstartsize=5m \
    dalvik.vm.heapgrowthlimit=64m \
    dalvik.vm.heapsize=72m \
    dalvik.vm.heaptargetutilization=0.75 \
    dalvik.vm.heapminfree=512k \
    dalvik.vm.heapmaxfree=2m

$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)

# Specific overlay
DEVICE_PACKAGE_OVERLAYS += device/lge/msm8610-common/overlay

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/vendor/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/vendor/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/vendor/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/vendor/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/vendor/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/vendor/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/vendor/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/vendor/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/vendor/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.gyroscope.xml:system/vendor/etc/permissions/android.hardware.sensor.gyroscope.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepcounter.xml:system/vendor/etc/permissions/android.hardware.sensor.stepcounter.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepdetector.xml:system/vendor/etc/permissions/android.hardware.sensor.stepdetector.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/vendor/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/vendor/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/vendor/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/vendor/etc/permissions/android.software.sip.voip.xml \
    frameworks/native/data/etc/android.hardware.sensor.accelerometer.xml:system/vendor/etc/permissions/android.hardware.sensor.accelerometer.xml \
    frameworks/native/data/etc/android.hardware.sensor.compass.xml:system/vendor/etc/permissions/android.hardware.sensor.compass.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/vendor/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.telephony.cdma.xml:system/vendor/etc/permissions/android.hardware.telephony.cdma.xml \
    frameworks/native/data/etc/android.hardware.audio.low_latency.xml:system/vendor/etc/permissions/android.hardware.audio.low_latency.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/vendor/etc/permissions/android.hardware.bluetooth_le.xml \
    device/lge/msm8610-common/prebuilt/etc/permissions/com.qualcomm.location.xml:system/vendor/etc/permissions/com.qualcomm.location.xml

# Configs
PRODUCT_COPY_FILES += \
    device/lge/msm8610-common/prebuilt/etc/wifi/p2p_supplicant_overlay.conf:system/etc/wifi/p2p_supplicant_overlay.conf \
    device/lge/msm8610-common/prebuilt/etc/wifi/wpa_supplicant_overlay.conf:system/etc/wifi/wpa_supplicant_overlay.conf \
    device/lge/msm8610-common/prebuilt/etc/wifi/WCNSS_cfg.dat:system/etc/firmware/wlan/prima/WCNSS_cfg.dat \
    device/lge/msm8610-common/prebuilt/etc/wifi/WCNSS_qcom_cfg.ini:system/etc/firmware/wlan/prima/WCNSS_qcom_cfg.ini \
    device/lge/msm8610-common/prebuilt/etc/wifi/WCNSS_qcom_wlan_nv.bin:system/etc/firmware/wlan/prima/WCNSS_qcom_wlan_nv.bin \
    device/lge/msm8610-common/prebuilt/etc/wifi/WCNSS_qcom_wlan_nv.bin:system/etc/firmware/wlan/prima/WCNSS_qcom_wlan_nv_boot.bin \
    device/lge/msm8610-common/prebuilt/etc/hostapd/hostapd.accept:system/etc/hostapd/hostapd.accept \
    device/lge/msm8610-common/prebuilt/etc/hostapd/hostapd.deny:system/etc/hostapd/hostapd.deny \
    device/lge/msm8610-common/prebuilt/etc/hostapd/hostapd_default.conf:system/etc/hostapd/hostapd_default.conf \
    device/lge/msm8610-common/prebuilt/etc/audio_policy.conf:vendor/etc/audio_policy.conf \
    device/lge/msm8610-common/prebuilt/etc/audio_effects.conf:vendor/etc/audio_effects.conf \
    device/lge/msm8610-common/prebuilt/etc/mixer_paths.xml:vendor/etc/mixer_paths.xml \
    device/lge/msm8610-common/prebuilt/etc/media_codecs.xml:vendor/etc/media_codecs.xml \
    device/lge/msm8610-common/prebuilt/etc/media_profiles.xml:vendor/etc/media_profiles.xml \
    device/lge/msm8610-common/prebuilt/usr/idc/touch_dev.idc:system/usr/idc/touch_dev.idc \
    device/lge/msm8610-common/prebuilt/usr/keylayout/gpio-keys.kl:system/usr/keylayout/gpio-keys.kl \
    device/lge/msm8610-common/prebuilt/etc/init.crda.sh:system/etc/init.crda.sh \
    device/lge/msm8610-common/gps/etc/sap.conf:system/etc/sap.conf \
    device/lge/msm8610-common/gps/etc/flp.conf:system/etc/flp.conf \
    device/lge/msm8610-common/gps/etc/gps.conf:system/etc/gps.conf \
    device/lge/msm8610-common/prebuilt/etc/msap.conf:system/etc/msap.conf \
    device/lge/msm8610-common/prebuilt/etc/clatd.conf:system/etc/clatd.conf \
    device/lge/msm8610-common/prebuilt/etc/sec_config:system/etc/sec_config \
    device/lge/msm8610-common/gps/etc/izat.conf:system/etc/izat.conf \
    device/lge/msm8610-common/gps/etc/quipc.conf:system/etc/quipc.conf \
	device/lge/msm8610-common/prebuilt/etc/init.qcom.fm.sh:system/etc/init.qcom.fm.sh \
    device/lge/msm8610-common/prebuilt/etc/libnfc-brcm.conf:vendor/etc/libnfc-brcm.conf \
    device/lge/msm8610-common/prebuilt/etc/libnfc-nxp.conf:vendor/etc/libnfc-nxp.conf

# Ramdisk
PRODUCT_COPY_FILES += \
    device/lge/msm8610-common/rootdir/init.msm8610.rc:root/init.msm8610.rc \
    device/lge/msm8610-common/rootdir/init.msm8610.usb.rc:root/init.msm8610.usb.rc \
    device/lge/msm8610-common/rootdir/ueventd.msm8610.rc:root/ueventd.msm8610.rc \
	device/lge/msm8610-common/rootdir/init.qcom.ril.sh:system/etc/init.qcom.ril.sh \
    device/lge/msm8610-common/prebuilt/etc/init.zetaw.bt.sh:system/etc/init.zetaw.bt.sh 

# TWRP
PRODUCT_COPY_FILES += \
    device/lge/msm8610-common/rootdir/twrp.fstab:recovery/root/etc/twrp.fstab

# Audio
PRODUCT_PACKAGES += \
    audio.primary.msm8610 \
    audio.a2dp.default \
    audio.usb.default \
    audio.r_submix.default \
    libaudio-resampler \
    libaudioparameter \
    libqcomvisualizer \
    libqcompostprocbundle \
    libqcomvoiceprocessing \
    tinycap \
    tinymix \
    tinypcminfo \
    tinyplay \
    libalsa-intf \
    libaudio-resampler \
    libaudioutils \
    aplay \
    amix \
    arec \
    alsaucm_test \
    android.hardware.audio@2.0-impl \
    android.hardware.audio.effect@2.0-impl \
    android.hardware.audio@2.0-service \
    android.hardware.audio.effect@2.0-service

# Misc
PRODUCT_PACKAGES += \
    curl \
    libbson \
    libcurl \
    tcpdump \
    libxml2

# CMActions
PRODUCT_PACKAGES += \
    CMActions
 
# Misc
PRODUCT_PACKAGES += \
    com.android.future.usb.accessory

# Bluetooth
PRODUCT_PACKAGES += \
    hwaddrs \
    libbt-vendor \
    android.hardware.bluetooth@1.0-impl \
    android.hardware.bluetooth@1.0-service

# Crda
PRODUCT_PACKAGES += \
    crda \
    linville.key.pub.pem \
    regdbdump \
    regulatory.bin

# Qcom SoftAP
PRODUCT_PACKAGES += \
    libQWiFiSoftApCfg

# Live Wallpapers
PRODUCT_PACKAGES += \
    LiveWallpapers \
    LiveWallpapersPicker \
    MagicSmokeWallpapers \
    HoloSpiralWallpaper \
    VisualizationWallpapers \
    librs_jni

# Wifi
PRODUCT_PACKAGES += \
    p2p_supplicant_overlay.conf \
    wpa_supplicant_overlay.conf \
    libwcnss_qmi \
    hostapd \
    wpa_supplicant \
    wpa_supplicant.conf \
    wificond \
    hostapd.accept \
    hostapd.deny \
    hostapd_default.conf \
    libnetcmdiface \
    android.hardware.wifi@1.0-service

# Telephony-ext
PRODUCT_PACKAGES += telephony-ext
PRODUCT_BOOT_JARS += telephony-ext

# Charger
PRODUCT_PACKAGES += charger charger_res_images

# Ebtables
PRODUCT_PACKAGES += \
    ebtables \
    ethertypes \
    libebtc

# FM radio
PRODUCT_PACKAGES += \
	qcom.fmradio \
    libqcomfm_jni \
    FM2 \
    FMRecord \
    libfmjni

# GPS
PRODUCT_PACKAGES += \
    gps.msm8610 \
    android.hardware.gnss@1.0-impl \
    android.hardware.gnss@1.0-service

# HAL
PRODUCT_PACKAGES += \
    copybit.msm8610\
    gralloc.msm8610 \
    hwcomposer.msm8610 \
    keystore.msm8610 \
    lights.msm8610 \
    memtrack.msm8610 \
    power.msm8610

# Power
PRODUCT_PACKAGES += \
    android.hardware.power@1.0-impl

# Utilities
PRODUCT_PACKAGES += \
    charge_only_mode \
    mkfs.f2fs \
    fsck.f2fs \
    fibmap.f2fs \
    wcnss_service \
    librmnetctl

# EGL config
PRODUCT_COPY_FILES += \
    device/lge/msm8610-common/prebuilt/egl.cfg:system/lib/egl/egl.cfg


# QCOM Display
PRODUCT_PACKAGES += \
    libgenlock \
    libmemalloc \
    liboverlay \
    libqdutils \
    libtilerenderer \
    android.hardware.graphics.allocator@2.0-impl \
    android.hardware.graphics.mapper@2.0-impl \
    android.hardware.graphics.composer@2.1-impl \
    android.hardware.memtrack@1.0-impl \
    android.hardware.memtrack@1.0-service

# Omx
PRODUCT_PACKAGES += \
    libdivxdrmdecrypt \
    libmm-omxcore \
    libOmxCore \
    libstagefrighthw \
    libOmxVdec \
    libOmxVenc \
    libOmxAacEnc \
    libOmxAmrEnc \
    libOmxEvrcEnc \
    libOmxQcelp13Enc \



# Filesystem management tools
PRODUCT_PACKAGES += \
    make_ext4fs \
    e2fsck \
    resize2fs \
    setup_fs


# NFC
PRODUCT_PACKAGES += \
    android.hardware.nfc@1.0-impl

# Sensors
PRODUCT_PACKAGES += \
	sensors.msm8610 \
    android.hardware.sensors@1.0-impl \
    android.hardware.sensors@1.0-service


# BoringSSL
PRODUCT_PACKAGES += \
    libboringssl-compat

# Jelly
PRODUCT_PACKAGES += \
    Jelly

# Camera
PRODUCT_PACKAGES += \
	Snap \
    android.hardware.camera.provider@2.4-impl-legacy \
    camera.device@1.0-impl-legacy

# DRM
PRODUCT_PACKAGES += \
    android.hardware.drm@1.0-impl \
    android.hardware.drm@1.0-service

# USB
PRODUCT_PACKAGES += \
    android.hardware.usb@1.0-service

# Vibrator
PRODUCT_PACKAGES += \
    android.hardware.vibrator@1.0-impl

# Update Notify Service
PRODUCT_PACKAGES += \
	Update_Notifications

# System properties
-include $(LOCAL_PATH)/system_prop.mk

# Media codecs
PRODUCT_COPY_FILES += \
    frameworks/av/media/libstagefright/data/media_codecs_google_audio.xml:system/etc/media_codecs_google_audio.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_telephony.xml:system/etc/media_codecs_google_telephony.xml \
    frameworks/av/media/libstagefright/data/media_codecs_google_video_le.xml:system/etc/media_codecs_google_video_le.xml

PRODUCT_BUILD_PROP_OVERRIDES += BUILD_UTC_DATE=0

$(call inherit-product, vendor/lge/msm8610-common/msm8610-vendor.mk)
