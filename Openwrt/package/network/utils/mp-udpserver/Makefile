#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mp-udpserver
PKG_VERSION:=2015-07-08
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/mp-udpserver
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Realtek WiFi AP Router MP UDP Server
endef

define Package/mp-udpserver/description
	Realtek WiFi AP Router MP UDP Server
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	echo $(PKG_BUILD_DIR)
	echo $(TARGET_CROSS)
	$(CP) ./files/mp-daemon/* $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default)
endef

define Build/Configure
	echo $(PKG_BUILD_DIR)
endef

MAKE_OPTS:= CROSS_COMPILE="$(TARGET_CROSS)"

define Build/Compile
	(echo $(PKG_BUILD_DIR); cd $(PKG_BUILD_DIR);$(MAKE) $(MAKE_OPTS))
endef

define Package/mp-udpserver/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/UDPserver $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,mp-udpserver))
