#
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=uuid
PKG_VERSION:=1.6.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=ftp://ftp.ossp.org/pkg/lib/uuid/

include $(INCLUDE_DIR)/package.mk

define Package/uuid
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=OSSP uuid
  DEPENDS=
  URL:=http://www.ossp.org/pkg/lib/uuid/
endef

define Package/uuid/description
    OSSP uuid
    http://www.ossp.org/pkg/lib/uuid/
endef


define Package/uuid/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/.libs/uuid $(1)/usr/bin/uuid
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/.libs/libuuid.so* $(1)/usr/lib
endef

$(eval $(call BuildPackage,uuid))

