# Exynos, omap4 and msm8960 have their own native camera
# implementations, see
# hardware/samsung_slsi/exynos5/libcamera2
# hardware/ti/omap4xxx
# hardware/qcom/camera/QCamera/HAL2/core
ifeq ($(findstring $(TARGET_BOARD_PLATFORM),omap4 exynos5 msm8960),)

LOCAL_PATH:= $(call my-dir)

################################################################################
include $(CLEAR_VARS)

LOCAL_SRC_FILES := camera.$(TARGET_BOARD_PLATFORM).so
LOCAL_MODULE_TAGS:= optional
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
LOCAL_MODULE_SUFFIX := .so
LOCAL_MODULE := camera.$(TARGET_BOARD_PLATFORM)

include $(BUILD_PREBUILT)

endif
