LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := PoJNIT
LOCAL_SRC_FILES := com_ooieueioo_pojnireadwriteproc_PoJNIT.c
LOCAL_LDLIBS += -llog
include $(BUILD_SHARED_LIBRARY)