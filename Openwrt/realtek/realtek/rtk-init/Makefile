#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rtk-init
#PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/rtk-init
  SECTION:=base
  CATEGORY:=Base system
  TITLE:=init process for Realtek SoC
  DEPENDS:=@TARGET_rtd1295 +libc
  DEFAULT:=y
endef
define Package/rtk-init/description
 Realtek SoC init process for OpenWRT platform
endef
define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev

endef

define Package/rtk-init/install
	$(SCRIPT_DIR)/ipkg-extract $(PKG_NAME) "$(1)" "$(TOPDIR)/packages"
endef

$(eval $(call BuildPackage,rtk-init))

