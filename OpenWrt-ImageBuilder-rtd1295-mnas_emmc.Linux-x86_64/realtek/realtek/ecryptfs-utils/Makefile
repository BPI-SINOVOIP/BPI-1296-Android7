# 
# Copyright (C) 2006-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ecryptfs-utils
PKG_VERSION:=106
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).orig.tar.gz
PKG_SOURCE_URL:=https://launchpad.net/ecryptfs/trunk/$(PKG_VERSION)/+download/
PKG_MD5SUM:=bff8052636f6be642f15c6be45a14ea3

PKG_BUILD_DEPENDS:=intltool/host

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/ecryptfs-utils
	SECTION:=utils
	CATEGORY:=Utilities
	SUBMENU:=Filesystem
	TITLE:=eCryptfs utilities
	URL:=http://ecryptfs-utils.sourceforge.net/
	DEPENDS:=+keyutils +openssl +libecryptfs +libc-bin +gettext-tools
endef

define Package/ecryptfs-utils/description
	eCryptfs utilities
endef

define Package/libecryptfs
	SECTION:=libs
	SUBMENU:=Filesystem
	CATEGORY:=Libraries
	TITLE:=eCryptfs Library
	DEPENDS:= +libgcrypt +libkeyutils +libopenssl
endef

define Package/libecryptfs/description
	eCryptfs library
endef

TARGET_LDFLAGS += \
        -Wl,-rpath-link,$(STAGING_DIR)/usr/lib \
        -lgcrypt


define Build/Configure
	$(call Build/Configure/Default, \
		--enable-openssl \
		--disable-pywrap \
		--disable-pam \
		--disable-gui \
		--disable-nss \
		--disable-pkcs11-helper \
		--disable-tspi \
	)
endef

define Package/libecryptfs/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/* $(1)/usr/lib/
endef

define Package/ecryptfs-utils/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/sbin/* $(1)/sbin
#	$(INSTALL_DIR) $(1)/etc
#	$(CP) ./files/netconfig $(1)/etc/
#	$(INSTALL_DIR) $(1)/etc/init.d
#	$(INSTALL_BIN) ./files/ecryptfs-utils.init $(1)/etc/init.d/ecryptfs-utils
endef

$(eval $(call BuildPackage,ecryptfs-utils))
$(eval $(call BuildPackage,libecryptfs))
