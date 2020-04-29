#
# Copyright (C) 2006-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=iozone
PKG_VERSION:=3.434
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)$(subst .,_,$(PKG_VERSION)).tar
PKG_SOURCE_URL:=http://www.iozone.org/src/current/
PKG_MD5SUM:=3e8f4213581407225065b91774e970ed
PKG_MAINTAINER:=
PKG_LICENSE:= IOzone license (NO DERIVED WORKS ALLOWED)
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)$(subst .,_,$(PKG_VERSION))

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS+=

define Package/iozone
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=disc
  TITLE:=File System Benchmark
  URL:=http://www.iozone.org/
endef

define Package/iozone/description
 File System Benchmark
endef

define Build/Compile
	$(MAKE) -C "$(PKG_BUILD_DIR)/src/current" \
		CC="$(TARGET_CC)" \
		GCC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		linux-arm
endef

define Package/iozone/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/current/$(PKG_NAME) $(1)/sbin
endef

$(eval $(call BuildPackage,iozone))
