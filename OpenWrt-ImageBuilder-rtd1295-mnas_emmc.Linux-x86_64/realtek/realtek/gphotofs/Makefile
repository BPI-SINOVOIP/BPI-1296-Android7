#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/nls.mk

PKG_NAME:=gphotofs
PKG_VERSION:=0.5
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=@SF/gphoto
#PKG_MD5SUM:=2635075f702b40eb2e95a80658bd4773

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/gphotofs
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=GPhotoFS is a filesystem client based on libgphoto2 
  URL:=http://www.gphoto.org/
  DEPENDS:=+libgphoto2 +libgphoto2-drivers +libpthread +glib2  +kmod-fuse +libfuse $(INTL_DEPENDS)  $(ICONV_DEPENDS)
endef

define Package/gphotofs/description
 	GPhotoFS is a filesystem client based on libgphoto2 that exposes
       supported cameras as filesystems; while some cameras implement the USB
       Mass Storage class and already appear as filesystems (making this
       program redundant), many use the Picture Transfer Protocol (PTP) or
       some other custom protocol. But as long as the camera is supported by
       libgphoto2, it can be mounted as a filesystem using this program.

       As libgphoto2 is a userspace library for interacting with cameras, it
       is natural that if one to build a filesystem ontop of it, one should
       use FUSE, and that is what I have done.
endef

LIBGPHOTO2_LIBS=$(LDFLAGS) -lgphoto2 -lgphoto2_port

CONFIGURE_ARGS += \
	--with-libiconv-prefix="$(ICONV_PREFIX)" \
	--with-libintl-prefix="$(INTL_PREFIX)" \
	--prefix=/usr/local/ \
	--with-glib-prefix="$(STAGING_DIR)/usr" \

CONFIGURE_ARGS += \
	LIBGPHOTO2_CFLAGS="$(CFLAGS) -I$(STAGING_DIR)/usr/include/gphoto2 $(CPPFLAGS)" \
	LIBGPHOTO2_LIBS="$(LDFLAGS) -lgphoto2 -lgphoto2_port"

TARGET_CFLAGS += $(FPIC) $(if $(ICONV_FULL),-liconv) $(if $(INTL_FULL),-lintl)

TARGET_CPPFLAGS += \
	-I$(STAGING_DIR)/usr/include \
	-I$(ICONV_PREFIX)/include \
	-I$(INTL_PREFIX)/include \
	-L$(STAGING_DIR)/usr/lib/libintl-full \
	-L$(PKG_INSTALL_DIR)/usr/lib \
	-L$(STAGING_DIR)/usr/lib/ \

define Package/gphotofs/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(CP) $(PKG_INSTALL_DIR)/usr/bin/$(PKG_NAME) $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/etc/hotplug.d/usb
	$(INSTALL_BIN) ./files/10-gphoto-mount $(1)/etc/hotplug.d/usb
	$(INSTALL_BIN) ./files/11-usbfs-umount $(1)/etc/hotplug.d/usb
endef

$(eval $(call BuildPackage,gphotofs))
