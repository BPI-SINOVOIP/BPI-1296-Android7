#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=opencapwap
PKG_VERSION:=1.0
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/opencapwap/Default
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Routing and Redirection
  TITLE:=Realtek opencapwap daemon
endef

define Package/opencapwap/description
	Realtek opencapwap Daemon
endef

define Package/opencapwap
$(call Package/opencapwap/Default)
DEPENDS:=+libapmib
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/opencapwap/* $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default)
endef

#EXTRA_CFLAGS:= -I$(STAGING_DIR)/usr/include -DCONFIG_RTL_8197F -DRTK_129X_PLATFORM -DNOT_DELETE_LOG -I.
EXTRA_CFLAGS:= -I$(STAGING_DIR)/usr/include -DCONFIG_RTL_8197F -DNEW_CHECKINTERNET -DRTK_129X_PLATFORM -I.

MAKE_OPTS:= CC="$(TARGET_CC)" \
	APMIB_SHARED=1 \
	EXTRA_CFLAGS="$(EXTRA_CFLAGS)" \
	CONFIG_OPENWRT_SDK=$(CONFIG_OPENWRT_SDK) \
	DIR_OPENWRT_STAGING=$(STAGING_DIR_ROOT) \
	CONFIG_RTK_SMART_ROAMING=$(CONFIG_RTK_SMART_ROAMING) \
	CONFIG_RTL_DOT11K_SUPPORT=$(CONFIG_RTL_DOT11K_SUPPORT) \
	CONFIG_RTL_MESH_SUPPORT=$(CONFIG_RTL_MESH_SUPPORT) \
	CONFIG_RTL_MULTI_CLONE_SUPPORT=$(CONFIG_RTL_MULTI_CLONE_SUPPORT) \
	CONFIG_USE_PCIE_SLOT_0=$(CONFIG_USE_PCIE_SLOT_0) \
	CONFIG_USE_PCIE_SLOT_1=$(CONFIG_USE_PCIE_SLOT_1) \
	CONFIG_RTL_COMAPI_CFGFILE=$(CONFIG_RTL_COMAPI_CFGFILE) \
	CONFIG_SLOT_0_8192EE=$(CONFIG_SLOT_0_8192EE) \
	CONFIG_SLOT_1_8192EE=$(CONFIG_SLOT_1_8192EE) \
	CONFIG_RTL_8812_SUPPORT=$(CONFIG_RTL_8812_SUPPORT) \
	CONFIG_WLAN_HAL_8822BE=$(CONFIG_WLAN_HAL_8822BE) \
	CONFIG_ENABLE_EFUSE=$(CONFIG_ENABLE_EFUSE)

define Build/Compile
	(cd $(PKG_BUILD_DIR);$(MAKE) $(MAKE_OPTS))
endef

define Package/opencapwap/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/AC $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/WTP $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/AACWTP $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DATA) ./files/opencapwap/config.ac $(1)/etc/config
	$(INSTALL_DATA) ./files/opencapwap/config.wtp $(1)/etc/config
	$(INSTALL_DATA) ./files/opencapwap/config.ac $(1)/etc/config
	$(INSTALL_DATA) ./files/opencapwap/settings.wtp.txt $(1)/etc/config
	$(INSTALL_DATA) ./files/opencapwap/settings.ac.txt $(1)/etc/config
	
endef

$(eval $(call BuildPackage,opencapwap))
