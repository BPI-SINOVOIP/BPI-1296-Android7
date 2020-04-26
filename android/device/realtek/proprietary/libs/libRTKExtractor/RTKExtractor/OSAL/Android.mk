LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../chip.mk

LOCAL_SRC_FILES := \
	OSAL.cpp \
	CThread.cpp \
	PLI.cpp \
	Threads.cpp \
	IO.cpp \

#	remote_malloc_svc.c remote_malloc_xdr.c debug_level_clnt.c debug_level_xdr.c

#LOCAL_SRC_FILES += libc/arch-arm/syscalls/msgctl.S \
#			libc/arch-arm/syscalls/msgget.S \
#			libc/arch-arm/syscalls/msgrcv.S \
#			libc/arch-arm/syscalls/msgsnd.S \
#			libc/arch-arm/syscalls/semctl.S \
#			libc/arch-arm/syscalls/semget.S \
#			libc/arch-arm/syscalls/semop.S \
#			libc/arch-arm/syscalls/shmat.S \
#			libc/arch-arm/syscalls/shmctl.S \
#			libc/arch-arm/syscalls/shmdt.S \
#			libc/arch-arm/syscalls/shmget.S

#LOCAL_SRC_FILES += remote_malloc_svr.c
#LOCAL_SRC_FILES += remote_malloc_ion.c ionapi.c

LOCAL_CFLAGS := $(DEFINES) -DSYS_UNIX=1

LOCAL_CFLAGS += -DANDROID
LOCAL_CFLAGS += -D__ARM_EABI__

#ifeq ($(HOST), mipsel-linux)
LOCAL_CFLAGS += -D_GNU_SOURCE -D_MIPSEL_LINUX
#endif

#ifneq ($(TARGET_BOARD), NO)
#LOCAL_CFLAGS += -DTARGET_BOARD
#endif

ifeq ($(DFB_MULTI_APP), YES)
LOCAL_CFLAGS += -fPIC
endif

#ifeq ($(ENABLE_DFB_VKBD), YES)
#for compile dfb_vkd
#LOCAL_CFLAGS += -fPIC
#endif

LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libRTKOSAL

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../Include \
	$(LOCAL_PATH)/../Include/IPC \
	$(LOCAL_PATH)/../OSAL/include \
	$(LOCAL_PATH)/../OSAL/include/OSAL \
	$(LOCAL_PATH)/../Include/IPC/include/ \
	$(LOCAL_PATH)/../common/IPC/include \
	bionic

LOCAL_SYSTEM_SHARED_LIBRARIES := libc libstdc++

LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type

LOCAL_SHARED_LIBRARIES := liblog

include $(BUILD_STATIC_LIBRARY)
