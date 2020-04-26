#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-dom-xpath
#PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/python-dom-xpath
  SECTION:=language-python
  CATEGORY:=Languages
  SUBMENU:=Python
  TITLE:=Python XPath for DOM trees
  DEPENDS:=+python
endef
define Package/python-dom-xpath/description
   py-dom-xpath is a pure Python implementation of XPath 1.0. It supports almost all XPath 1.0, with the main exception being the namespace axis. It operates on DOM 2.0 nodes, and works well with xml.dom.minidom.
endef
define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev

endef

define Package/python-dom-xpath/install
	$(SCRIPT_DIR)/ipkg-extract $(PKG_NAME) "$(1)" "$(TOPDIR)/packages"
endef

$(eval $(call BuildPackage,python-dom-xpath))

