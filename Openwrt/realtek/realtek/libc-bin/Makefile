#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

PKG_NAME:=libc-bin
PKG_VERSION:=$(call qstrip,$(CONFIG_GLIBC_VERSION))
PKG_REVISION:=$(call qstrip,$(CONFIG_GLIBC_REVISION))
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/libc-bin
  SECTION:=util
  CATEGORY:=Utilities
  URL:=http://www.eglibc.org
  TITLE:=Binaries for GNU C libraries
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/libc-bin/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(TOOLCHAIN_DIR)/bin/getent $(1)/usr/bin
	$(INSTALL_BIN) $(TOOLCHAIN_DIR)/bin/getconf $(1)/usr/bin
endef

$(eval $(call BuildPackage,libc-bin))
