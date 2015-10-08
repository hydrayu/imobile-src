LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := iMobileReadSerialNumber
LOCAL_SRC_FILES := com_imobile_iq8readserialnumber_iMobileReadSerialNumber.c
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)