#
# Copyright (C) 2007-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rtkshaper
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Jo-Philipp Wich <jow@openwrt.org>

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/rtkshaper
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+kmod-ifb +ip +kmod-sched +tc
  TITLE:=rtkshaper
  PKGARCH:=all
endef

define Package/rtkshaper/description
 A script to do traffing shaping with the HTB algorithm.
endef

define Build/Prepare
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/rtkshaper/install
	$(INSTALL_DIR) $(1)/usr/sbin/ $(1)/etc/init.d $(1)/etc/config $(1)/lib $(1)/etc/hotplug.d/iface
	$(INSTALL_BIN) ./files/rtkshaper.sh $(1)/usr/sbin/
	$(INSTALL_BIN) ./files/rtkshaper_func.sh $(1)/lib/
	$(INSTALL_BIN) ./files/rtkshaper.init $(1)/etc/init.d/rtkshaper
	$(INSTALL_BIN) ./files/rtkshaper.iface $(1)/etc/hotplug.d/iface/10-rtkshaper
	$(INSTALL_DATA) ./files/rtkshaper.config $(1)/etc/config/rtkshaper
endef

define Package/rtkshaper/conffiles
/etc/config/rtkshaper
endef

$(eval $(call BuildPackage,rtkshaper))
