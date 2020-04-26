#
# Copyright (C) 2015-2016 Realtek
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rtkbist
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

define Package/rtkbist
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Realtek BIST tool
  DEPENDS:=@TARGET_rtd1295 +memtester +tinymembench +testusb +iozone
endef

define Package/rtkbist/description
 Realtek BIST tool
endef

define Build/Prepare
        mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile/Default

endef


define Package/rtkbist/install
	$(INSTALL_DIR) $(1)/usr/sbin/
	$(INSTALL_BIN) ./src/rtkbist.sh $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,rtkbist))
