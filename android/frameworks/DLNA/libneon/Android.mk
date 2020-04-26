LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#LOCAL_SRC_FILES := $(call all-c-files-under, src)
LOCAL_SRC_FILES := src/ne_request.c src/ne_session.c 	    \
					src/ne_string.c 		    \
					src/ne_uri.c src/ne_dates.c src/ne_alloc.c  \
					src/ne_utils.c    \
					src/ne_socket.c \
					src/ne_redirect.c src/ne_compress.c src/ne_stats.c \
					src/ne_auth.c src/ne_md5.c

LOCAL_CFLAGS := -DHAVE_CONFIG_H  -D_LARGEFILE64_SOURCE -DNE_LFS

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libneon

LOCAL_C_INCLUDES := .
	
LOCAL_SYSTEM_SHARED_LIBRARIES := libc

include $(BUILD_STATIC_LIBRARY)
