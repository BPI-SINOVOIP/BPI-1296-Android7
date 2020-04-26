
# Copyright (C) 2014, 2015 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libfdt
PKG_VERSION:=1.4.1
PKG_RELEASE:=1
PKG_LICENSE:=GPL

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=604e61
PKG_SOURCE_URL:=git://git.kernel.org/pub/scm/utils/dtc/dtc.git
PKG_SOURCE_PROTO:=git

include $(INCLUDE_DIR)/package.mk

define Package/libfdt
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=A Library to manipulate Flat Device Trees
  URL:=git://git.kernel.org/pub/scm/utils/dtc/dtc.git
endef

define Package/libfdt/description
  libfdt is a library to manipulate Flat Device Trees
endef

PKG_INSTALL:=1

define Package/libfdt/install
	$(INSTALL_DIR) $(1)/usr/local/lib
	$(CP) $(PKG_BUILD_DIR)/libfdt/libfdt-1.4.1.so $(1)/usr/local/lib/
	( cd $(1)/usr/local/lib ; $(LN) libfdt-$(PKG_VERSION).so libfdt.so.1 ; $(LN) libfdt-$(PKG_VERSION).so libfdt.so )
endef

$(eval $(call BuildPackage,libfdt))
