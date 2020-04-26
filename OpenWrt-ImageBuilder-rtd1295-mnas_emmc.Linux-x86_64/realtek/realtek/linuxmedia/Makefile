#
# Copyright (C) 2009-2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=linuxmedia
PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_SOURCE_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/linuxmedia
  CATEGORY:=Multimedia
  SECTION:=multimedia
  TITLE:=Realtek media libraries
  DEPENDS:=+libstdcpp $(LIBRPC_DEPENDS)
endef

define Package/linuxmedia/description
 Realtek media libraries
endef

define Build/Prepare
endef

define Package/linuxmedia/install
endef

$(eval $(call BuildPackage,linuxmedia))
