#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=xmlstarlet
#PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/xmlstarlet
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=An xml editor
  URL:=http://xmlstar.sourceforge.net/
  MAINTAINER:=Mikhail Grushinskiy
  DEPENDS:=+libxslt +libxml2 +libexslt
endef
define Package/xmlstarlet/description
  XMLStarlet is open source free software under MIT license which allows free use and distribution for both commercial and non-commercial projects.
endef
define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev

endef

define Package/xmlstarlet/install
	$(SCRIPT_DIR)/ipkg-extract $(PKG_NAME) "$(1)" "$(TOPDIR)/packages"
endef

$(eval $(call BuildPackage,xmlstarlet))

