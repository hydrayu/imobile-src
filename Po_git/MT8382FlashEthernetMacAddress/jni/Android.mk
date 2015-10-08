LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := PoTestJNI
LOCAL_SRC_FILES := com_ooieueioo_potestjni_PoJNITtest.c
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)