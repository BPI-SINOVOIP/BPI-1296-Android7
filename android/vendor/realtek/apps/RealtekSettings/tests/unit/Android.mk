LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

# We only want this apk build for tests.
LOCAL_MODULE_TAGS := tests
LOCAL_CERTIFICATE := platform

LOCAL_JAVA_LIBRARIES := android.test.runner

LOCAL_STATIC_JAVA_LIBRARIES := \
    android-support-test \
    mockito-target \
    espresso-core

# Include all test java files.
LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_OVERRIDES_PACKAGES := SettingsUnitTests
LOCAL_PACKAGE_NAME := RealtekSettingsUnitTests

LOCAL_INSTRUMENTATION_FOR := RealtekSettings

include $(BUILD_PACKAGE)
