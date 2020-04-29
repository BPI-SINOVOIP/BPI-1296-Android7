
# Copyright (C) 2008-2017 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=zulu
ifeq ($(CONFIG_ZULU_32BIT_JVM), y)
PKG_VERSION:=1.8.0_152-8.25.0.76
PKG_BUILD_DIR=$(BUILD_DIR)//ezdk-1.8.0_152-8.25.0.76-eval-linux_aarch32hf
else
PKG_VERSION:=1.8.0_152-8.25.0.79
PKG_BUILD_DIR=$(BUILD_DIR)/ezdk-1.8.0_152-8.25.0.79-eval-linux_aarch64
endif
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://cdn.azul.com/zulu-embedded/bin/
ifeq ($(CONFIG_ZULU_32BIT_JVM), y)
PKG_SOURCE:=ezdk-$(PKG_VERSION)-eval-linux_aarch32hf.tar.gz
PKG_MD5SUM:=69e43f93b95a20702213fb186ce73d2a
else
PKG_SOURCE:=ezdk-$(PKG_VERSION)-eval-linux_aarch64.tar.gz
PKG_MD5SUM:=643cea65c8158d8f3b004a41beaa287c
endif


include $(INCLUDE_DIR)/package.mk

STRIP:=/bin/true
RSTRIP:= \
    NM="$(TOOLCHAIN_DIR)/bin/$(TARGET_CROSS)nm" \
    STRIP="$(STRIP)" \
    STRIP_KMOD="$(STRIP)" \
    $(SCRIPT_DIR)/rstrip.sh

define Package/zulu
  SUBMENU:=Java
  SECTION:=lang
  CATEGORY:=Languages
  URL:=https://www.azul.com/
  TITLE:=Java SE standard Based on OpenJDK
  KCONFIG:= \
	CONFIG_ARMV8_DEPRECATED=y \
	CONFIG_COMPAT_NETLINK_MESSAGES=y \
	CONFIG_CP15_BARRIER_EMULATION=y \
	CONFIG_SETEND_EMULATION=y \
	CONFIG_SWP_EMULATION=y
  DEPENDS:=@TARGET_rtd1295 +libc +zlib +libstdcpp +alsa-lib +libfreetype +libX11 +libXfixes +libXext +libthread_db +libXext +libXi +libXrender +libXtst
endef

define Package/zulu/description
  Zulu® Embedded is a certified build of OpenJDK that is fully compliant with the Java SE standard
endef

define Package/zulu/config
	source "$(SOURCE)/Config.in"
endef

define Build/Compile
endef

define Package/zulu/install
	$(INSTALL_DIR)	$(1)/usr/lib/jvm/zulu-8
	$(INSTALL_DIR)  $(1)/lib
	$(CP) $(PKG_BUILD_DIR)/bin $(1)/usr/lib/jvm/zulu-8
	$(CP) $(PKG_BUILD_DIR)/demo $(1)/usr/lib/jvm/zulu-8
	$(CP) $(PKG_BUILD_DIR)/include $(1)/usr/lib/jvm/zulu-8
	$(CP) $(PKG_BUILD_DIR)/jre $(1)/usr/lib/jvm/zulu-8
	$(CP) $(PKG_BUILD_DIR)/lib $(1)/usr/lib/jvm/zulu-8
	$(CP) $(PKG_BUILD_DIR)/sample $(1)/usr/lib/jvm/zulu-8
	$(CP) $(TOOLCHAIN_DIR)/lib/libthread_db* $(1)/usr/lib
ifeq ($(CONFIG_ZULU_32BIT_JVM), y)
	$(CP) ./files/arm-linux-gnueabihf/* $(1)/usr/lib/jvm/zulu-8/lib/aarch32
	$(CP) ./files/arm-linux-gnueabihf/ld-2.19.so $(1)/lib/ld-linux-armhf.so.3
endif
endef

$(eval $(call BuildPackage,zulu))
