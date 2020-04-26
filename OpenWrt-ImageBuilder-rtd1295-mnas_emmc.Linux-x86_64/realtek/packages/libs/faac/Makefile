#
# Copyright (C) 2008-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=faac
PKG_VERSION:=1.28
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/faac
PKG_MD5SUM:=c5dde68840cefe46532089c9392d1df0
PKG_MAINTAINER:=

PKG_CONFIG_DEPENDS := \
	CONFIG_SOFT_FLOAT \
	CONFIG_FAAC_ALWAYS_USE_FLOATING_POINT

PKG_FIXUP:=libtool
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += $(FPIC)

# Use fixed point math, if we're using soft floats, and the user didn't
# specifically ask to use floats anyways.
ifeq ($(CONFIG_SOFT_FLOAT)-$(CONFIG_FAAC_ALWAYS_USE_FLOATING_POINT),y-)
TARGET_CFLAGS+= -DFIXED_POINT
FAAC_CONFIG_OPTS= --without-fp -nfp
endif

define Package/faac/Default
  URL:=http://www.audiocoding.com/faac.html
  TITLE:=Freeware Advanced Audio Coder
endef

define Package/faac/Default/description
	FAAC is an Advanced Audio Coder (MPEG2-AAC, MPEG4-AAC).
endef

define Package/faac
$(call Package/faac/Default)
  SECTION:=sound
  CATEGORY:=Sound
  TITLE+=player
  DEPENDS:=+libfaac
endef

define Package/faac/description
$(call Package/faac/Default/description)
  This package contains a binary to encode to AAC or MP4 files.
endef

define Package/libfaac
$(call Package/faac/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE+=library
  DEPENDS:=@BUILD_PATENTED
  MENU:=1
endef

define Package/libfaac/config
	source "$(SOURCE)/Config.in"
endef

define Package/libfaac/description
$(call Package/faac/Default/description)
  This package contains the library.
endef

define Build/Configure
	(cd $(PKG_BUILD_DIR); rm -rf config.{cache,status}; \
		autoreconf -vif \
	);
	$(call Build/Configure/Default, \
		--with-mp4v2 \
		$(FAAC_CONFIG_OPTS) \
		--without-xmms \
		, \
		FAAC_CPPFLAGS="-fno-builtin-cos -fno-builtin-sin -fno-builtin-log" \
	)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/*.h $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libfaac.{a,so*} $(1)/usr/lib/
endef

define Package/faac/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/faac $(1)/usr/bin/
endef

define Package/libfaac/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libfaac.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,faac))
$(eval $(call BuildPackage,libfaac))
