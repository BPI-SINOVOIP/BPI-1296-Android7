#
# Copyright (C) 2007-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=dot11k_daemon
PKG_VERSION:=1
PKG_SOURCE_SUBDIR:=$(PKG_NAME)
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define Package/dot11k_daemon/Default
  SECTION:=net
  CATEGORY:=Network
endef

define Package/dot11k_daemon/description
	Realtek 802.11k Daemon
endef

define Package/dot11k_daemon
$(call Package/dot11k_daemon/Default)
  TITLE:=Realtek 802.11k daemon
endef

define Package/11k_test_daemon
$(call Package/dot11k_daemon/Default)
  TITLE:=Realtek 802.11k test daemon
endef

define Package/11v_test_daemon
$(call Package/dot11k_daemon/Default)
  TITLE:=Realtek 802.11v test daemon
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/src/* $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default)
endef

MAKE_OPTS:= CROSS_COMPILE="$(TARGET_CROSS)" \
	CONFIG_RTL_11V_SUPPORT=$(CONFIG_RTL_11V_SUPPORT)


define Build/Compile
	(cd $(PKG_BUILD_DIR);$(MAKE) $(MAKE_OPTS))
endef

define Package/dot11k_daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dot11k_deamon $(1)/usr/bin/
endef

define Package/11k_test_daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_11k $(1)/usr/bin/
endef

define Package/11v_test_daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/test_11v $(1)/usr/bin/
endef

$(eval $(call BuildPackage,dot11k_daemon))
$(eval $(call BuildPackage,11k_test_daemon))
$(eval $(call BuildPackage,11v_test_daemon))
