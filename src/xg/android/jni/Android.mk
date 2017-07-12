LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := crossguidtest
LOCAL_CFLAGS := -Wall
LOCAL_SRC_FILES := ../../Guid.cpp ../../test/Test.cpp jnitest.cpp
LOCAL_CPP_FLAGS := -std=c++14
LOCAL_CPPFLAGS := -DGUID_ANDROID -Wno-c++11-extensions -Wno-missing-braces
LOCAL_LDLIBS := -latomic

include $(BUILD_SHARED_LIBRARY)
