#
# Copyright (C) 2007-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=rrdtool
PKG_VERSION:=1.5.5
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://oss.oetiker.ch/rrdtool/pub/
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_MD5SUM:=d8b3dcb3d193c2d6ad0a282bde69ee11

PKG_BUILD_DEPENDS:=python

PKG_CONFIG_DEPENDS:= \
	CONFIG_PACKAGE_python-rrd \
	CONFIG_PACKAGE_rrdcgi \

PKG_FIXUP:=autoreconf gettext-version

PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

$(call include_mk, python-package.mk)

define Package/rrdtool/Default
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Round Robin Database (RRD)
  URL:=http://oss.oetiker.ch/rrdtool/
endef

define Package/librrd
$(call Package/rrdtool/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE+= library (v1.5.x)
  DEPENDS+= +host-pkgconf +libglib2 +libxml2 +cairo +pango +liberation-fonts-ttf +dejavu-fonts-ttf
endef

define Package/librrd-lite
$(call Package/rrdtool/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE+= library (v1.5.x, no graph)
  DEPENDS+= +libxml2
endef

define Package/rrdcgi
$(call Package/rrdtool/Default)
  TITLE+= CGI graphing tool (v1.5.x)
  DEPENDS+= +librrd +cgilib
endef

define Package/rrdtool
$(call Package/rrdtool/Default)
  TITLE+= management tools (v1.5.x)
  DEPENDS+= +librrd
endef

define Package/rrdtool-lite
$(call Package/rrdtool/Default)
  TITLE+= tools (v1.5.x, no graph)
  DEPENDS+= librrd-lite
endef


define Package/python-rrd
$(call Package/rrdtool/Default)
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE+= python bindings (v1.5.x)
  DEPENDS+= +python-mini +librrd
endef

CONFIGURE_ARGS += \
	--enable-shared=yes \
	--enable-static=yes \
	--disable-rpath \
	--with-gnu-ld \
	--disable-mmap \
	--disable-perl \
	--disable-tcl \
	--disable-ruby \
	--disable-examples \
	--disable-libdbi \
	--disable-librados \
	--disable-libwrap \
	--disable-lua

ifneq ($(SDK)$(CONFIG_PACKAGE_rrdtool-lite),)
  CONFIGURE_ARGS += \
	--disable-rrd_graph \
	--with-rrd-default-font="/usr/share/rrdtool/fonts/DejaVuSansMono.ttf" 
endif


ifneq ($(SDK)$(CONFIG_PACKAGE_rrdcgi),)
  CONFIGURE_ARGS += --enable-rrdcgi
else
  CONFIGURE_ARGS += --disable-rrdcgi
endif

ifneq ($(SDK)$(CONFIG_PACKAGE_python-rrd),)
  CONFIGURE_ARGS += \
	--enable-python
  CONFIGURE_VARS += \
	PYTHON="$(PYTHON)"
else
  CONFIGURE_ARGS += --disable-python
endif

define Build/Compile
	$(if $(Build/Compile/PyMod),,@echo Python packaging code not found.; false)
	$(call Build/Compile/Default)
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib/rrdtool-1.5/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/rrd.h $(1)/usr/lib/rrdtool-1.5/include/
	$(INSTALL_DIR) $(1)/usr/lib/rrdtool-1.5/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/librrd{,_th}.{a,so*} $(1)/usr/lib/rrdtool-1.5/lib/
	$(LN) librrd.so.4 $(1)/usr/lib/rrdtool-1.5/librrd.so.0
endef

define Package/librrd/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/librrd.so.* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/librrd_th.so.* $(1)/usr/lib/
	$(LN) librrd.so.4 $(1)/usr/lib/librrd.so.0
endef

define Package/librrd-lite/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/librrd.so.* $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/librrd_th.so.* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/share/rrdtool/fonts
	$(CP) ./data/DejaVuSansMono.ttf $(1)/usr/share/rrdtool/fonts/
endef

define Package/rrdcgi/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rrdcgi $(1)/usr/bin/
endef

define Package/rrdtool-lite/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rrdtool $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rrdupdate $(1)/usr/bin/
endef

define Package/rrdtool/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rrdtool $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rrdupdate $(1)/usr/bin/
endef

define PyPackage/python-rrd/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) $(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* $(1)$(PYTHON_PKG_DIR)/
endef

$(eval $(call BuildPackage,librrd))
$(eval $(call BuildPackage,rrdcgi))
$(eval $(call BuildPackage,rrdtool))

$(eval $(call BuildPackage,librrd-lite))
$(eval $(call BuildPackage,rrdtool-lite))

$(eval $(call PyPackage,python-rrd))
$(eval $(call BuildPackage,python-rrd))
