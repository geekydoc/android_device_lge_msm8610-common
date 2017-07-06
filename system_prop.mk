# IO Scheduler
PRODUCT_PROPERTY_OVERRIDES += \
    sys.io.scheduler=bfq

# System properties
PRODUCT_PROPERTY_OVERRIDES += \
    persist.hwc.mdpcomp.enable=true \
    persist.timed.enable=true \
    ro.opengles.version=196608 \
    ro.qualcomm.perf.cores_online=2 \
    dalvik.vm.dex2oat-flags=--no-watch-dog \
	dalvik.vm.dex2oat-filter=interpret-only \
    debug.qualcomm.sns.hal=0 \
    debug.qualcomm.sns.daemon=e \
    debug.qualcomm.sns.libsensor1=e

# Storage
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.sdcardfs=true

#Camera properties
PRODUCT_PROPERTY_OVERRIDES += \
    camera2.portability.force_api=1 \
    media.stagefright.legacyencoder=true \
    media.stagefright.less-secure=true

# Wifi properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.data.large_tcp_window_size=true \
    wifi.interface=wlan0 \
    wifi.supplicant_scan_interval=15 \
	wlan.driver.config=/system/etc/firmware/wlan/prima/WCNSS_qcom_cfg.ini
    
# QCOM Display
PRODUCT_PROPERTY_OVERRIDES += \
    persist.hwc.mdpcomp.enable=true \
    debug.sf.fb_always_on=1 \
    debug.composition.type=gpu \
    debug.gralloc.map_fb_memory=0 \
    debug.hwc.dynThreshold=1.5 \
    dalvik.vm.heapsize=36m \
    dev.pm.dyn_samplingrate=1
    
# QCOM HARDWARE
PRODUCT_PROPERTY_OVERRIDES += \
    com.qc.hardware=true

# Telephony properties
PRODUCT_PROPERTY_OVERRIDES += \
    ro.config.vc_call_vol_steps=10 \
    persist.radio.proc_nw_scan=0 \
    ro.use_data_netmgrd=true \
    persist.data.netmgrd.qos.enable=true \
    ro.telephony.call_ring.multiple=0 \
    rild.libpath=/vendor/lib/libril-qc-qmi-1.so \
    rild.libargs=-d[SPACE]/dev/smd0 \
    persist.data.qmi.adb_logmask=0

# GPS
PRODUCT_PROPERTY_OVERRIDES += \
    persist.gps.qc_nlp_in_use=0 \
    ro.gps.agps_provider=1

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
	qcom.bluetooth.soc=smd \
    ro.qualcomm.bt.hci_transport=smd \
    ro.bluetooth.request.master=true 

# Audio properties
PRODUCT_PROPERTY_OVERRIDES += \
    mm.enable.smoothstreaming=true \
    ro.qc.sdk.audio.fluencetype=endfire \
    persist.audio.fluence.voicecall=true \
    audio.offload.buffer.size.kb=32 \
    av.offload.enable=true \
    av.streaming.offload.enable=false \
    audio.offload.multiple.enabled=false \
    audio.offload.gapless.enabled=true \
    tunnel.audio.encode=true \
    media.aac_51_output_enabled=true \
    audio.offload.pcm.16bit.enable=true \
    audio.offload.pcm.24bit.enable=true \
    use.voice.path.for.pcm.voip=true \
    persist.audio.calfile0=/etc/acdbdata/Bluetooth_cal.acdb \
    persist.audio.calfile1=/etc/acdbdata/General_cal.acdb \
    persist.audio.calfile2=/etc/acdbdata/Global_cal.acdb \
    persist.audio.calfile3=/etc/acdbdata/Handset_cal.acdb \
    persist.audio.calfile4=/etc/acdbdata/Hdmi_cal.acdb \
    persist.audio.calfile5=/etc/acdbdata/Headset_cal.acdb \
    persist.audio.calfile6=/etc/acdbdata/Speaker_cal.acdb
