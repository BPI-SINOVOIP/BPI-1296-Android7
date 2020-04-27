LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := ion.c
LOCAL_MODULE := libion
LOCAL_MODULE_TAGS := optional
LOCAL_SHARED_LIBRARIES := liblog libutils
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/kernel-headers
LOCAL_EXPORT_C_INCLUDE_DIRS := $(LOCAL_PATH)/include $(LOCAL_PATH)/kernel-headers
#LOCAL_CFLAGS := -Werror
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := ion.c ion_test.c
LOCAL_MODULE := iontest
LOCAL_MODULE_TAGS := optional tests
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/kernel-headers
LOCAL_SHARED_LIBRARIES := liblog libutils
LOCAL_CFLAGS := -Werror
include $(BUILD_EXECUTABLE)
include $(call first-makefiles-under,$(LOCAL_PATH))

#include $(CLEAR_VARS)
#LOCAL_SRC_FILES := ion.c ion_test_2.c
#LOCAL_MODULE := iontest2
#LOCAL_MODULE_TAGS := optional tests
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/kernel-headers
#LOCAL_SHARED_LIBRARIES := liblog libutils
#include $(BUILD_EXECUTABLE)
#include $(call first-makefiles-under,$(LOCAL_PATH))

#include $(CLEAR_VARS)
#LOCAL_SRC_FILES := ion.c ion_test_3.c
#LOCAL_MODULE := iontest3
#LOCAL_MODULE_TAGS := optional tests
#LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/kernel-headers
#LOCAL_SHARED_LIBRARIES := liblog libutils
#include $(BUILD_EXECUTABLE)
#include $(call first-makefiles-under,$(LOCAL_PATH))

include $(call first-makefiles-under,$(LOCAL_PATH))
