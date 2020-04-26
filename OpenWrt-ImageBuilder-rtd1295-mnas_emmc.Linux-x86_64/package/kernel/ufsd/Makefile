# 
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ufsd
PKG_RELEASE:=2

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ufsd
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  SUBMENU:=Filesystems
  TITLE:=Paragon's File System
  DEPENDS:=+libc
  FILES:= \
	$(PKG_BUILD_DIR)/ufsd.ko \
	$(PKG_BUILD_DIR)/jnl.ko
  AUTOLOAD:=$(call AutoProbe,ufsd)
endef

define KernelPackage/ufsd/Default/description
  Parago's driver for NTFS/exFAT/HFS+
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
	$(SCRIPT_DIR)/ipkg-extract "$(1)" "$(PKG_BUILD_DIR)" "$(TOPDIR)/packages" ./lib/
	find "$(PKG_BUILD_DIR)/lib" -name "*.ko" -print | xargs mv -t "$(PKG_BUILD_DIR)/"
	rm -rf "$(PKG_BUILD_DIR)/lib"
endef

define KernelPackage/ufsd/install
	$(SCRIPT_DIR)/ipkg-extract kmod-$(PKG_NAME) "$(PKG_BUILD_DIR)" "$(TOPDIR)/packages" ./usr/
	mv "$(PKG_BUILD_DIR)/usr" "$(1)/"
endef

$(eval $(call KernelPackage,ufsd))
