
ifneq ($(filter w5 w3ds w55ds,$(TARGET_DEVICE)),)

include $(call all-subdir-makefiles)

endif
