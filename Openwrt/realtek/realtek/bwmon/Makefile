#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=bwmon
#PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/bwmon
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=CPU tools
  TITLE:=Bandwidth monitor for Realtek SoC
  KCONFIG:= \
	CONFIG_UIO=y \
	CONFIG_UIO_ASSIGN_MINOR=y \
	CONFIG_UIO_RTK_RBUS=y \
	CONFIG_UIO_RTK_REFCLK=y \
	CONFIG_UIO_RTK_SE=y \
	CONFIG_UIO_RTK_MD=y
  DEPENDS:= +jansson +libstdcpp
endef
define Package/bwmon/description
 Realtek SoC bandwidth monitor
endef
define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev

endef

define Package/bwmon/install
	$(SCRIPT_DIR)/ipkg-extract $(PKG_NAME) "$(1)" "$(TOPDIR)/packages"
endef

$(eval $(call BuildPackage,bwmon))

