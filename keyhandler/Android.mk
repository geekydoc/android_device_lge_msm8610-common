LOCAL_PATH:= $(call my-dir)

ifneq ($(filter w5 w3ds w55ds d290n d295,$(TARGET_DEVICE)),)

include $(CLEAR_VARS)

LOCAL_MODULE := com.cyanogenmod.keyhandler
LOCAL_SRC_FILES := $(call all-java-files-under,src)
LOCAL_MODULE_TAGS := optional

include $(BUILD_JAVA_LIBRARY)

endif
