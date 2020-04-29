#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
#include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=libapmib
PKG_VERSION:=2017-07-20
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/libapmib
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Realtek AP MIB operation library
endef

define Package/libapmib/description
	Routines to handle MIB operation
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/apmib/* $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default)
endef

define Build/Configure
	echo $(PKG_BUILD_DIR)
endef

#MAKE_FLAGS := -DRTK_129X_PLATFORM -D_RTL8187B_ -D_RTL865X_
#MAKE_FLAGS := -DPATHSEL_DAEMON_DEBUG
EXTRA_CFLAGS := -DCONFIG_RTL_AP -DCONFIG_RTL_8197F

ifeq ($(CONFIG_RTL_8812_SUPPORT),y)
	EXTRA_CFLAGS += -DCONFIG_RTL_8812_SUPPORT
endif

ifeq ($(CONFIG_WLAN_HAL_8822BE),y)
	EXTRA_CFLAGS += -DCONFIG_WLAN_HAL_8822BE
endif

ifeq ($(CONFIG_RTL_8814_SUPPORT),y)
	EXTRA_CFLAGS += -DCONFIG_RTL_8814_SUPPORT
endif

MAKE_OPTS:= CC="$(TARGET_CC)" EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
		APMIB_SHARED=1 \
		APMIB_LIB=libapmib.so


define Build/Compile
	(echo $(PKG_BUILD_DIR); cd $(PKG_BUILD_DIR);$(MAKE) $(MAKE_OPTS))
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_BUILD_DIR)/apmib.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/list.h $(1)/usr/include/
	$(CP) $(PKG_BUILD_DIR)/mibdef.h $(1)/usr/include/
endef

define Package/libapmib/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libapmib.so $(1)/usr/lib/ 
endef

$(eval $(call BuildPackage,libapmib))
