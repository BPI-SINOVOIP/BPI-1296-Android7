LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

RTK_TOP := hardware/realtek/realtek_omx/rtk_libs/OSAL

include $(RTK_TOP)/../system/include/MakeConfig

LOCAL_SRC_FILES := \
	OSAL.cpp \
	CThread.cpp \
	PLI.cpp \
	Threads.cpp \
	IO.cpp \
	remote_malloc_svc.c \
	remote_malloc_xdr.c \
	debug_level_clnt.c \
	debug_level_xdr.c

#LOCAL_SRC_FILES += remote_malloc_svr.c
LOCAL_SRC_FILES += remote_malloc_ion.c ionapi.c

ifneq ($(TARGET_ARCH), mips)
    DEFINES := $(filter-out -mips32r2,$(DEFINES))
endif
LOCAL_CFLAGS := $(DEFINES) -DSYS_UNIX=1

ifeq ($(HOST), mipsel-linux)
LOCAL_CFLAGS += -D_GNU_SOURCE -D_MIPSEL_LINUX
endif

ifneq ($(TARGET_BOARD), NO)
LOCAL_CFLAGS += -DTARGET_BOARD
endif

ifeq ($(DFB_MULTI_APP), YES)
LOCAL_CFLAGS += -fPIC
endif

ifeq ($(ENABLE_DFB_VKBD), YES)
#for compile dfb_vkd
LOCAL_CFLAGS += -fPIC
endif

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libRTK_OSAL

LOCAL_C_INCLUDES := \
	. \
	$(RTK_TOP)/include \
	$(RTK_TOP)/include/ion \
	$(RTK_TOP)/include/OSAL \
	$(RTK_TOP)/../common/IPC/include \
	bionic

LOCAL_SYSTEM_SHARED_LIBRARIES := libc libstdc++

include $(BUILD_STATIC_LIBRARY)

################################################################################

include $(CLEAR_VARS)
LOCAL_MODULE := libhook
LOCAL_SRC_FILES := hook.cpp
LOCAL_SHARED_LIBRARIES := libutils libdl liblog
LOCAL_MODULE_TAGS := optional
include $(BUILD_SHARED_LIBRARY)

