LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := test
LOCAL_SRC_FILES := guid.cpp test.cpp
LOCAL_CPP_FLAGS := -std=c++11
LOCAL_LDLIBS := -luuid

include $(BUILD_EXECUTABLE)
