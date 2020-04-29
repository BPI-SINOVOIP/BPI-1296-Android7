#
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=osspuuid
PKG_VERSION:=1.6.2
PKG_RELEASE:=1

PKG_SOURCE:=uuid-$(PKG_VERSION).tar.gz
PKG_MD5SUM:=5db0d43a9022a6ebbbc25337ae28942f
PKG_SOURCE_URL:=ftp://ftp.ossp.org/pkg/lib/uuid/ \
		http://pkgs.fedoraproject.org/repo/pkgs/uuid/uuid-1.6.2.tar.gz/$(PKG_MD5SUM)/

PKG_BUILD_DIR:=$(BUILD_DIR)/uuid-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/osspuuid
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=OSSP uuid
  DEPENDS=
  URL:=http://www.ossp.org/pkg/lib/uuid/
endef

define Package/osspuuid/description
    OSSP uuid
    http://www.ossp.org/pkg/lib/uuid/
endef


define Package/osspuuid/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/.libs/uuid $(1)/usr/bin/uuid
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/.libs/libuuid.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,osspuuid))

