LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := crossguidtest
LOCAL_CFLAGS := -Wall
LOCAL_SRC_FILES := ../../guid.cpp ../../test.cpp jnitest.cpp
LOCAL_CPP_FLAGS := -std=c++11
LOCAL_CPPFLAGS := -DGUID_ANDROID -Wno-c++11-extensions

include $(BUILD_SHARED_LIBRARY)
