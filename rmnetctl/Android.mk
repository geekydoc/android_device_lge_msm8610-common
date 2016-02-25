
ifneq ($(filter w5 w3ds w55ds l70pn l70pds,$(TARGET_DEVICE)),)

include $(call all-subdir-makefiles)

endif
