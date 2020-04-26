LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:= librtmp/log.c librtmp/rtmp.c  librtmp/amf.c librtmp/hashswf.c librtmp/parseurl.c

LOCAL_SHARED_LIBRARIES := libssl libcrypto libz

CRYPTO=OPENSSL
CRYPTO_DEF=$(DEF_$(CRYPTO))
VERSION=v2.4
DEF=-DRTMPDUMP_VERSION=\"$(VERSION)\" $(CRYPTO_DEF) $(XDEF)

LOCAL_C_INCLUDES := externel/boringssl/include externel/boringssl/include/src/include

LOCAL_CFLAGS := -Werror -Wall

LOCAL_CFLAGS += -DSYS_UNIX=1 $(DEF) -DDVR -DUSE_BIONIC -D_RTK_ANDROID_ARM_ -D__u_char_defined -D__daddr_t_defined
LOCAL_CFLAGS += -DDVR -DUSE_BIONIC -D_RTK_ANDROID_ARM_ -mcpu=cortex-a7  -DLINUX_NPTL --sysroot=/opt/android-ndk//platforms/android-18/arch-arm -I/opt/android-ndk//platforms/android-18/arch-arm/usr/include -D__u_char_defined -D__daddr_t_defined
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie

LOCAL_MODULE:= librtmp
LOCAL_MODULE_TAGS := optional

include $(BUILD_STATIC_LIBRARY)
###############################################################################
include $(CLEAR_VARS)

#LOCAL_SRC_FILES:= rtmpdump.cpp
LOCAL_SRC_FILES:= rtmpdump.c

LOCAL_SHARED_LIBRARIES := libz libc
LOCAL_STATIC_LIBRARIES := librtmp libssl_static libcrypto_static 
#LOCAL_FORCE_STATIC_EXECUTABLE := true

CRYPTO=OPENSSL
CRYPTO_DEF=$(DEF_$(CRYPTO))
VERSION=v2.4
DEF=-DRTMPDUMP_VERSION=\"$(VERSION)\" $(CRYPTO_DEF) $(XDEF)

LOCAL_C_INCLUDES := 

LOCAL_CFLAGS := -Werror -Wall

LOCAL_CFLAGS += -DSYS_UNIX=1 $(DEF) -DDVR -DUSE_BIONIC -D_RTK_ANDROID_ARM_ -D__u_char_defined -D__daddr_t_defined
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie

LOCAL_MODULE:= rtmpdump
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
