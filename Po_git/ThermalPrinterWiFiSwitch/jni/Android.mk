LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := imobileJNI
LOCAL_SRC_FILES := com_imobile_thermalprinterwifiswitch_imobileJNI.c
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)