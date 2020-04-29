#
# Copyright (C) 2008-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=yafc
PKG_VERSION:=1.3.7
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=http://www.yafc-ftp.com/downloads/
PKG_MD5SUM:=4b6e0d46ab7ab78956bbb106ae904820

PKG_FIXUP:=autoreconf
PKG_REMOVE_FILES:=autogen.sh aclocal.m4

include $(INCLUDE_DIR)/package.mk

define Package/yafc
  SUBMENU:=File Transfer
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libncurses +libreadline +libopenssl +libssh +USE_GLIBC:libbsd
  TITLE:=Yafc is yet another ftp client.
  URL:=http://www.yafc-ftp.com/
endef

# uses GNU configure
CONFIGURE_ARGS += \
	--with-socks=no \
	--with-socks5=no \
	--with-krb4=no \
	--with-krb5=no \
	--with-gssapi=no

CONFIGURE_VARS += \
	bash_cv_func_sigsetjmp=missing

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)
endef

define Package/yafc/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_BUILD_DIR)/yafc $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,yafc))
