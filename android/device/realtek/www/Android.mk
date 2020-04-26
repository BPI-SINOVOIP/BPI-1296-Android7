LOCAL_PATH := $(call my-dir)
# =============================================================
ifeq ($(ENABLE_LIGHTTPD), true)
# Compile CGI
include $(call all-subdir-makefiles)
endif
