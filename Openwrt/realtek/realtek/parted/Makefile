#
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=parted
PKG_VERSION:=3.2
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=@GNU/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/parted
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=disc
  TITLE:=parted Partition editor
  DEPENDS=+libreadline +libncurses +libblkid +libuuid
  URL:=http://www.gnu.org/software/parted/index.shtml
endef

define Package/parted/description
    parted Partition editor
    http://www.gnu.org/software/parted/index.shtml
endef

define Build/Configure
	$(call Build/Configure/Default, --with-readline --disable-device-mapper)
endef

define Package/parted/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/libparted/.libs/libparted.so* $(1)/usr/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/parted/.libs/parted $(1)/usr/sbin/parted
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/partprobe/.libs/partprobe $(1)/usr/sbin/partprobe
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/parted
	$(CP) $(PKG_BUILD_DIR)/libparted/.libs/libparted.so* $(STAGING_DIR)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/include/parted/*.h $(STAGING_DIR)/usr/include/parted
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libparted.pc $(STAGING_DIR)/usr/lib/pkgconfig
endef

$(eval $(call BuildPackage,parted))

