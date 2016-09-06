# Root AOSP source makefile
# su is built here, and 
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := su
LOCAL_MODULE_TAGS := optional
LOCAL_WHOLE_STATIC_LIBRARIES := libcutils libutils libbinder liblog
LOCAL_SRC_FILES := su.c daemon.c utils.c pts.c
LOCAL_SRC_FILES += binder/appops-wrapper.cpp binder/pm-wrapper.c
LOCAL_CFLAGS += -Werror
LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)

LOCAL_INIT_RC := superuser.rc

include $(BUILD_EXECUTABLE)

SYMLINKS := $(addprefix $(TARGET_OUT)/bin/,su)
$(SYMLINKS):
	@echo "Symlink: $@ -> /system/xbin/su"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf ../xbin/su $@

# We need this so that the installed files could be picked up based on the
# local module name
ALL_MODULES.$(LOCAL_MODULE).INSTALLED := \
    $(ALL_MODULES.$(LOCAL_MODULE).INSTALLED) $(SYMLINKS)

