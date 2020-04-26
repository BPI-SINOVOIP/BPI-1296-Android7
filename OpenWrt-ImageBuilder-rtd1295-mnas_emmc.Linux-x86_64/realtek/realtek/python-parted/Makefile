#
# Copyright (C) 2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=python-parted
#PKG_VERSION:=2016
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/python-parted
  SECTION:=language-python
  CATEGORY:=Languages
  SUBMENU:=Python
  TITLE:=Python Parted
  URL:=https://github.com/rhinstaller/pyparted
  DEPENDS:=+python +parted 
endef
define Package/python-parted/description
 Python-parted is a set of native Python bindings for libparted
endef
define Build/Prepare
endef

define Build/Compile
endef

define Build/InstallDev

endef

define Package/python-parted/install
	$(SCRIPT_DIR)/ipkg-extract $(PKG_NAME) "$(1)" "$(TOPDIR)/packages"
endef

$(eval $(call BuildPackage,python-parted))

