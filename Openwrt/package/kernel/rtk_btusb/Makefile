# 
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=rtk_btusb
PKG_VERSION:=3.1
PKG_RELEASE:=1

PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_SOURCE_SUBDIR)
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/rtk_btusb/Default/description
Realtek bluetooth USB driver for Linux
endef

define KernelPackage/rtk_btusb
  SUBMENU:=Other modules
  DEPENDS:= +@USB_SUPPORT +kmod-bluetooth +@BT
  TITLE:= Realtek bluetooth USB driver for Linux
  FILES:=$(PKG_BUILD_DIR)/rtk_btusb.ko
  AUTOLOAD:=$(call AutoProbe,rtk_btusb,1)
  MENU:=1
endef

define KernelPackage/rtk_btusb/config
  if PACKAGE_kmod-rtk_btusb
	config RTL8822BU_BT_FW
		bool "Realtek RTL8822BU BT Firmware"
		default n
	config RTL8821CU_BT_FW
		bool "Realtek RTL8821CU BT Firmware"
		default n
	config RTL8723DU_BT_FW
		bool "Realtek RTL8723DU BT Firmware"
		default n
  endif
endef

define Build/Prepare
	rm -rf $(PKG_BUILD_DIR)
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./files/drv_src/* $(PKG_BUILD_DIR)/
	$(call Build/Prepare/Default)
endef

MAKE_OPTS:= CROSS_COMPILE="$(TARGET_CROSS)" \
	LINUX_KERNEL_PATH="${LINUX_DIR}" \
	ARCH="${LINUX_KARCH}"

define Build/Compile
	$(CP) $(LINUX_DIR)/Module.symvers $(PKG_BUILD_DIR)/; \
	cd $(PKG_BUILD_DIR); \
	$(MAKE) $(MAKE_OPTS);
endef

define KernelPackage/rtk_btusb/install
	$(INSTALL_DIR) $(1)/lib/firmware
ifneq ($(CONFIG_RTL8822BU_BT_FW),)
	$(INSTALL_DATA) ./files/firmware/8822bu/* $(1)/lib/firmware
endif
ifneq ($(CONFIG_RTL8821CU_BT_FW),)
	$(INSTALL_DATA) ./files/firmware/8821cu/* $(1)/lib/firmware
endif
ifneq ($(CONFIG_RTL8723DU_BT_FW),)
	$(INSTALL_DATA) ./files/firmware/8723du/* $(1)/lib/firmware
endif
endef

$(eval $(call KernelPackage,rtk_btusb))
