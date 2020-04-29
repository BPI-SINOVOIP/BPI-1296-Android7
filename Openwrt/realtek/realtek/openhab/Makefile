
# Copyright (C) 2008-2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=openhab
PKG_VERSION:=2.1.0
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=wget
PKG_SOURCE_URL=https://bintray.com/openhab/mvn/download_file?file_path=org%2Fopenhab%2Fdistro%2Fopenhab%2F2.1.0%2Fopenhab-2.1.0.tar.gz
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

TAR_CMD:=$(HOST_TAR) -C $(PKG_BUILD_DIR) $(TAR_OPTIONS)

STRIP:=/bin/true
RSTRIP:= \
    NM="$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS)nm" \
    STRIP="$(STRIP)" \
    STRIP_KMOD="$(STRIP)" \
    $(SCRIPT_DIR)/rstrip.sh

define Package/openhab
  SUBMENU:=Java
  SECTION:=lang
  CATEGORY:=Languages
  URL:=https://www.openhab.org/
  TITLE:=Java SE standard Based on OpenJDK
  DEPENDS:=@TARGET_rtd1295 +libc +zlib +libstdcpp +alsa-lib +libfreetype +zulu
  USERID:=openhab=200:openhab=200
endef

define Package/openhab/description
  A vendor and technology agnostic open source automation software for your home
endef

define Build/Compile
endef

define Package/openhab/install
	$(INSTALL_DIR)	$(1)/opt/openhab2
	$(CP)		$(PKG_BUILD_DIR)/addons $(1)/opt/openhab2
	$(CP)		$(PKG_BUILD_DIR)/conf $(1)/opt/openhab2
	$(CP)		$(PKG_BUILD_DIR)/runtime $(1)/opt/openhab2
	$(CP)		$(PKG_BUILD_DIR)/*.* $(1)/opt/openhab2
	$(CP)		$(PKG_BUILD_DIR)/userdata $(1)/opt/openhab2
endef

$(eval $(call BuildPackage,openhab))
