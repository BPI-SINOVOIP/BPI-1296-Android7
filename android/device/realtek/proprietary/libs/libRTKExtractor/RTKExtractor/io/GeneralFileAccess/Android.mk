LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/../../chip.mk

LOCAL_SRC_FILES:= \
file_db.c file_util.c gfile_common.c array_funcs.c list_funcs.c simple_funcs.c
# fastcopy_wrapper.cpp

LOCAL_C_INCLUDES:= \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/../../Include \
	$(LOCAL_PATH)/../../Include/IPC \
	$(LOCAL_PATH)/../../OSAL/include \
	$(LOCAL_PATH)/../../OSAL/include/OSAL \
	$(LOCAL_PATH)/../../Include/IPC/include/ \
	$(LOCAL_PATH)/../../common/IPC/include \

LOCAL_CFLAGS += $(CHIP_MODEL_DEFS)
LOCAL_CFLAGS += -Wno-error=format-security -Wno-error=non-virtual-dtor -Wno-error=address -Wno-error=sequence-point -Wno-error=return-type

LOCAL_MODULE:= libRTKGeneralFileAccess

LOCAL_SHARED_LIBRARIES := liblog

include $(BUILD_STATIC_LIBRARY)


include $(call all-makefiles-under,$(LOCAL_PATH))

