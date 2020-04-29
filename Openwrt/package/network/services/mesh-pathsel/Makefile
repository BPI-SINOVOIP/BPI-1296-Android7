#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=mesh-pathsel
#PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/mesh-pathsel
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Realtek 802.11s Mesh Path Selection Daemon
endef
define Package/mesh-pathsel/description
	Realtek 802.11s Mesh Path Selection Daemon
endef
define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev

endef

define Package/mesh-pathsel/install
	$(SCRIPT_DIR)/ipkg-extract $(PKG_NAME) "$(1)" "$(TOPDIR)/packages"
endef

$(eval $(call BuildPackage,mesh-pathsel))

