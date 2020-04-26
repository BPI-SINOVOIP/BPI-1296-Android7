# 
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=mali
ifeq ($(BOARD), rtd1295)
PKG_VERSION:=$(if $(CONFIG_TARGET_ANDROID_N),15.0,13.0)
else
PKG_VERSION:=5.0
endif
PKG_RELEASE:=4

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=xxx
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
ifeq ($(BOARD), rtd1295)
PKG_SOURCE_BRANCH:=midgard_$(PKG_VERSION)
PKG_SOURCE_VERSION:=$(if $(CONFIG_TARGET_ANDROID_N),674aeea,9a0a2e9)
else
PKG_SOURCE_BRANCH:=mali_5.0
PKG_SOURCE_VERSION:=3c77626
endif

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/mali-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_CROSS_NAME:=$(patsubst %-,%,$(TARGET_CROSS))

define KernelPackage/mali
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  SUBMENU:=Other modules
  DEPENDS:=@(TARGET_rtd1295||TARGET_rtd1195)
  TITLE:=ARM's Mali GPU driver
ifeq ($(BOARD), rtd1295)
  FILES:= \
	$(PKG_BUILD_DIR)/mali/drivers/base/ump/src/ump.ko \
	$(PKG_BUILD_DIR)/mali/drivers/base/kds/kds.ko \
	$(PKG_BUILD_DIR)/mali/drivers/base/ump/src/imports/ion/ump_kernel_import_ion.ko \
	$(PKG_BUILD_DIR)/mali/drivers/gpu/arm/midgard/mali_kbase.ko
else
  FILES:= \
	$(PKG_BUILD_DIR)/driver/src/devicedrv/mali/mali.ko \
	$(PKG_BUILD_DIR)/driver/src/devicedrv/ump/ump.ko
endif
endef

define KernelPackage/mali/Default/description
  ARM's Mali GPU driver for Midgard and Utgard
endef

ifeq ($(BOARD), rtd1295)
define Build/Compile
	cd $(PKG_BUILD_DIR)/mali; \
	$(MAKE) TARGET_KDIR=$(LINUX_DIR) \
		ARCH="$(LINUX_KARCH)" \
		TARGET="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		MALI_CROSS_COMPILE="$(TARGET_CROSS_NAME)"
endef
else
define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/driver/src/devicedrv/ump \
		KDIR=$(LINUX_DIR) \
		ARCH="$(LINUX_KARCH)" \
		TARGET="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		CONFIG=rtk BUILD=release
	$(MAKE) -C $(PKG_BUILD_DIR)/driver/src/devicedrv/mali \
		KDIR=$(LINUX_DIR) \
		ARCH="$(LINUX_KARCH)" \
		TARGET="$(LINUX_KARCH)" \
		CROSS_COMPILE="$(TARGET_CROSS)" \
		MALI_PLATFORM=rtk USING_MMU=1 USING_UMP=0 USING_PMM=0 BUILD=release \
		USING_PROFILING=0
endef
endif

define KernelPackage/mali/install
	$(INSTALL_DIR) $(KERNEL_BUILD_DIR)/mali
	$(CP) $(FILES) $(KERNEL_BUILD_DIR)/mali
	$(TARGET_CROSS)strip $(KERNEL_BUILD_DIR)/mali/*.ko --strip-unneeded
endef

$(eval $(call KernelPackage,mali))
