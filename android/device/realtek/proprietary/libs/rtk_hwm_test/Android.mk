#ifeq ($(USE_RT_MEDIA_PLAYER),true)

LOCAL_PATH:= $(call my-dir)

#include $(CLEAR_VARS)

#LOCAL_MODULE    := libRTKHWM
#LOCAL_SRC_FILES := libRTKHWM.a

#include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)

#LOCAL_STATIC_LIBRARIES:= \
		libRTKHWM						

LOCAL_SRC_FILES:= \
		main.c

LOCAL_C_INCLUDES:= \
		bionic							\
		external/stlport/stlport		\
		external/openssl/include		\
		device/realtek/proprietary/libs/RT_IPC \
		device/realtek/proprietary/libs	\
		device/realtek/proprietary/libs/RtkHWM	\


LOCAL_SHARED_LIBRARIES:= \
		libRTKHWM						\
		libc							\
		libstdc++						\
		libcrypto						\
        libutils                        \
        liblog							


#LOCAL_LDLIBS := -L$(LOCAL_PATH)/ -lRTKHWM

LOCAL_MODULE:= rtk_hwm_test

LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)
#include $(BUILD_STATIC_LIBRARY)

#endif
