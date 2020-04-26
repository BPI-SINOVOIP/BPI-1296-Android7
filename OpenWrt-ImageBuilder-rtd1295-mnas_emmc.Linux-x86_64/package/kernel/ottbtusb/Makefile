# 
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ottbtusb
PKG_VERSION:=1.0
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=xxx
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_BRANCH:=master
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ottbtusb/Default/description
Realtek bluetooth USB driver for Android
endef

define KernelPackage/ottbtusb
  SUBMENU:=Other modules
  DEPENDS:= +@USB_SUPPORT
  TITLE:= Realtek bluetooth USB driver for Android
  FILES:=$(PKG_BUILD_DIR)/rtk_btusb/rtk_btusb.ko
endef


MAKE_OPTS:= CROSS_COMPILE="$(TARGET_CROSS)" \
	LINUX_KERNEL_PATH="${LINUX_DIR}" \
	ARCH="${LINUX_KARCH}"

define Build/Compile
		$(CP) $(KERNEL_BUILD_DIR)/Module.symvers $(PKG_BUILD_DIR)/rtk_btusb; \
		cd $(PKG_BUILD_DIR)/rtk_btusb; \
		sed -i -e '/TOP/d' Makefile; \
		$(MAKE) $(MAKE_OPTS);
endef

$(eval $(call KernelPackage,ottbtusb))
