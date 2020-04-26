# 
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ottwifi
PKG_VERSION:=4.3
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=xxx
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_BRANCH:=master
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/ottwifi-$(PKG_VERSION)
PKG_BUILD_PARALLEL:=1

PKG_DRIVERS = \
	ottrtl8188eu ottrtl8188fu ottrtl8189es ottrtl8189fs ottrtl8192cu ottrtl8192eu ottrtl8723bs ottrtl8723ds ottrtl8723bu ottrtl8723du \
	ottrtl8811au ottrtl8812au ottrtl8812ae ottrtl8812bu ottrtl8814au ottrtl8821au ottrtl8821cu ottrtl8821as ottrtl8821cs ottrtl8821ce \
	ottrtl8822bu ottrtl8822bs ottrtl8822be ottrtl8192ee ottrtl8192es ottrtl8822cs

PKG_CONFIG_DEPENDS:= $(patsubst %,CONFIG_PACKAGE_kmod-%,$(PKG_DRIVERS))

include $(INCLUDE_DIR)/package.mk

define KernelPackage/ottwifi/Default
  VERSION:=$(LINUX_VERSION)-$(BOARD)-$(PKG_RELEASE)
  SUBMENU:=Wireless Drivers
  TITLE:=Realtek Wifi for Android
  KCONFIG:= \
		CONFIG_CFG80211=y \
		CONFIG_CFG80211_DEFAULT_PS=y \
		CONFIG_NL80211_TESTMODE=n \
		CONFIG_CFG80211_DEVELOPER_WARNINGS=n \
		CONFIG_CFG80211_REG_DEBUG=n \
		CONFIG_CFG80211_DEBUGFS=n \
		CONFIG_CFG80211_INTERNAL_REGDB=n \
		CONFIG_CFG80211_WEXT=n
endef

define KernelPackage/ottwifi/Default/description
  Realtek Wifi Drivers for Android
endef

define KernelPackage/ottrtl8188eu
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8188E USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8188eu/8188eu.ko
endef

define KernelPackage/ottrtl8188fu
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8188F USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8188fu/8188fu.ko
endef

define KernelPackage/ottrtl8189es
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8189E SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8189es/8189es.ko
endef

define KernelPackage/ottrtl8189fs
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8189F SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8189fs/8189fs.ko
endef

define KernelPackage/ottrtl8192cu
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8192C USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8192cu/8192cu.ko
endef

define KernelPackage/ottrtl8192eu
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8192E USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8192eu/8192eu.ko
endef

define KernelPackage/ottrtl8723bs
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8723B SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8723bs/8723bs.ko
endef

define KernelPackage/ottrtl8723ds
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8723D SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8723ds/8723ds.ko
endef

define KernelPackage/ottrtl8723bu
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8723B USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8723bu/8723bu.ko
endef

define KernelPackage/ottrtl8723du
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8723D USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8723du/8723du.ko
endef

define KernelPackage/ottrtl8811au
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8811A USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8811au/8811au.ko
endef

define KernelPackage/ottrtl8812au
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8812A USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8812au/8812au.ko
endef

define KernelPackage/ottrtl8812bu
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8812B USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8812bu/8812bu.ko
endef

define KernelPackage/ottrtl8814au
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8814A USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8814au/8814au.ko
endef

define KernelPackage/ottrtl8821au
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8821A USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8821au/8821au.ko
endef

define KernelPackage/ottrtl8821cu
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8821C USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8821cu/8821cu.ko
endef

define KernelPackage/ottrtl8822bu
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8822B USB)
  FILES:=$(PKG_BUILD_DIR)/rtl8822bu/8822bu.ko
endef

define KernelPackage/ottrtl8821as
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8821A SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8821as/8821as.ko
endef

define KernelPackage/ottrtl8821cs
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8821C SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8821cs/8821cs.ko
endef

define KernelPackage/ottrtl8821ce
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8821C PCIe)
  FILES:=$(PKG_BUILD_DIR)/rtl8821ce/8821ce.ko
endef

define KernelPackage/ottrtl8812ae
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8812A PCIe)
  FILES:=$(PKG_BUILD_DIR)/rtl8812ae/8812ae.ko
endef

define KernelPackage/ottrtl8822be
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8822B PCIe)
  FILES:=$(PKG_BUILD_DIR)/rtl8822be/8822be.ko
endef

define KernelPackage/ottrtl8822bs
  $(call KernelPackage/ottwifi/Default)
  DEPENDS+= +kmod-ottbtusb
  TITLE+= (RTL8822B SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8822bs/8822bs.ko
endef

define KernelPackage/ottrtl8192ee
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8192E PCIe)
  FILES:=$(PKG_BUILD_DIR)/rtl8192ee/8192ee.ko
endef

define KernelPackage/ottrtl8192es
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8192E SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8192es/8192es.ko
endef

define KernelPackage/ottrtl8822cs
  $(call KernelPackage/ottwifi/Default)
  TITLE+= (RTL8822C SDIO)
  FILES:=$(PKG_BUILD_DIR)/rtl8822cs/8822cs.ko
endef

MAKE_OPTS:= CROSS="$(TARGET_CROSS)" \
	LINUX_KERNEL_PATH="${LINUX_DIR}" \
	CONFIG_PLATFORM_RTK119X=$(CONFIG_TARGET_rtd1195) \
	CONFIG_PLATFORM_RTK129X=$(CONFIG_TARGET_rtd1295) \
	CONFIG_PLATFORM_RTK139X=$(CONFIG_TARGET_rtd1395) \
	USER_EXTRA_CFLAGS="${USER_EXTRA_CFLAGS}"

define Build/Compile
	+( \
	for wifi in $(shell echo $(config-m) | tr A-Z a-z | sed 's/ott//g'); do \
		(cd $(PKG_BUILD_DIR)/$$$${wifi} || continue; \
		sed -i -e '/TOP/d' Makefile; \
		$(CP) $(KERNEL_BUILD_DIR)/Module.symvers $(PKG_BUILD_DIR)/$$$${wifi}; \
		$(MAKE) $(PKG_JOBS) $(MAKE_OPTS)); \
	done \
	)
endef

config_package=$(if $(CONFIG_PACKAGE_kmod-$(1)),m)

config-$(call config_package,ottrtl8188eu) += OTTRTL8188EU
config-$(call config_package,ottrtl8188fu) += OTTRTL8188FU
config-$(call config_package,ottrtl8189es) += OTTRTL8189ES
config-$(call config_package,ottrtl8189fs) += OTTRTL8189FS
config-$(call config_package,ottrtl8192cu) += OTTRTL8192CU
config-$(call config_package,ottrtl8192eu) += OTTRTL8192EU
config-$(call config_package,ottrtl8723bs) += OTTRTL8723BS
config-$(call config_package,ottrtl8723ds) += OTTRTL8723DS
config-$(call config_package,ottrtl8723bu) += OTTRTL8723BU
config-$(call config_package,ottrtl8723du) += OTTRTL8723DU
config-$(call config_package,ottrtl8811au) += OTTRTL8811AU
config-$(call config_package,ottrtl8812au) += OTTRTL8812AU
config-$(call config_package,ottrtl8812bu) += OTTRTL8812BU
config-$(call config_package,ottrtl8812ae) += OTTRTL8812AE
config-$(call config_package,ottrtl8814au) += OTTRTL8814AU
config-$(call config_package,ottrtl8821au) += OTTRTL8821AU
config-$(call config_package,ottrtl8821cu) += OTTRTL8821CU
config-$(call config_package,ottrtl8821as) += OTTRTL8821AS
config-$(call config_package,ottrtl8821cs) += OTTRTL8821CS
config-$(call config_package,ottrtl8821ce) += OTTRTL8821CE
config-$(call config_package,ottrtl8822bu) += OTTRTL8822BU
config-$(call config_package,ottrtl8822bs) += OTTRTL8822BS
config-$(call config_package,ottrtl8822be) += OTTRTL8822BE
config-$(call config_package,ottrtl8192ee) += OTTRTL8192EE
config-$(call config_package,ottrtl8192es) += OTTRTL8192ES
config-$(call config_package,ottrtl8822cs) += OTTRTL8822CS

$(eval $(call KernelPackage,ottrtl8188eu))
$(eval $(call KernelPackage,ottrtl8188fu))
$(eval $(call KernelPackage,ottrtl8189es))
$(eval $(call KernelPackage,ottrtl8189fs))
$(eval $(call KernelPackage,ottrtl8192cu))
$(eval $(call KernelPackage,ottrtl8192eu))
$(eval $(call KernelPackage,ottrtl8723bs))
$(eval $(call KernelPackage,ottrtl8723ds))
$(eval $(call KernelPackage,ottrtl8723bu))
$(eval $(call KernelPackage,ottrtl8723du))
$(eval $(call KernelPackage,ottrtl8811au))
$(eval $(call KernelPackage,ottrtl8812au))
$(eval $(call KernelPackage,ottrtl8812bu))
$(eval $(call KernelPackage,ottrtl8814au))
$(eval $(call KernelPackage,ottrtl8821au))
$(eval $(call KernelPackage,ottrtl8821cu))
$(eval $(call KernelPackage,ottrtl8821as))
$(eval $(call KernelPackage,ottrtl8821cs))
$(eval $(call KernelPackage,ottrtl8821ce))
$(eval $(call KernelPackage,ottrtl8822bu))
$(eval $(call KernelPackage,ottrtl8822be))
$(eval $(call KernelPackage,ottrtl8822bs))
$(eval $(call KernelPackage,ottrtl8812ae))
$(eval $(call KernelPackage,ottrtl8192ee))
$(eval $(call KernelPackage,ottrtl8192es))
$(eval $(call KernelPackage,ottrtl8822cs))
