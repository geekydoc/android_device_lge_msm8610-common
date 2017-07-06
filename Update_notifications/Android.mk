LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional 
LOCAL_SRC_FILES := $(call all-java-files-under, java)

LOCAL_PACKAGE_NAME := Update_Notifications
LOCAL_CERTIFICATE := platform
LOCAL_STATIC_JAVA_LIBRARIES := \
    android-support-v7-appcompat \
    android-support-v4 

LOCAL_RESOURCE_DIR := \
    $(LOCAL_PATH)/res \
    frameworks/support/v7/appcompat/res


LOCAL_AAPT_FLAGS := --auto-add-overlay \
	--extra-packages android.support.v4:android.support.v7.appcompat

include frameworks/base/packages/SettingsLib/common.mk

include $(BUILD_PACKAGE)

include $(call all-makefiles-under,$(LOCAL_PATH))
