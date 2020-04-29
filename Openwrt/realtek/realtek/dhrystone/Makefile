# 
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dhrystone
PKG_VERSION:=2.1
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)


include $(INCLUDE_DIR)/package.mk

define Package/dhrystone
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Easy-to-use integer benchmark
  URL:=http://www.netlib.org/benchmark/dhry-c
  DEPENDS:=
  SUBMENU:=CPU tools
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)
	for i in ./patches/*.patch; do \
	patch -d $$(PKG_BUILD_DIR) -p1 < $$$$i; \
	done
endef

CONFIGURE_VARS+= \
    CC="$(TOOLCHAIN_DIR)/bin/$(TARGET_CC)"

define Package/dhrystone/install
	$(INSTALL_DIR) $(1)/bin
	$(CP) $(PKG_BUILD_DIR)/dhrystone $(1)/bin
endef

$(eval $(call BuildPackage,dhrystone))
