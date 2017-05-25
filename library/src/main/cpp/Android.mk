LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto
LOCAL_SRC_FILES := $(LOCAL_PATH)/thirdparty/lib/libcrypto.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libssl
LOCAL_SRC_FILES := $(LOCAL_PATH)/thirdparty/lib/libssl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libnghttp2
LOCAL_SRC_FILES := $(LOCAL_PATH)/thirdparty/lib/libnghttp2.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcurl
LOCAL_SRC_FILES := $(LOCAL_PATH)/thirdparty/lib/libcurl.a
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    		:= curl-client
LOCAL_SRC_FILES 		:= \
    native.cpp \
    log.h \
    native.h

LOCAL_C_INCLUDES        := $(LOCAL_PATH)/thirdparty/include

TARGET_PLATFORM         := android-14



LOCAL_LDLIBS += -latomic -lz -llog
LOCAL_STATIC_LIBRARIES  := libcurl libnghttp2  libssl libcrypto

include $(BUILD_SHARED_LIBRARY)

