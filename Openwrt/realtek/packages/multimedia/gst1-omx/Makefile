#
# Copyright (C) 2006-2016 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=gst1-omx
PKG_VERSION:=1.2.0
PKG_RELEASE=$(PKG_SOURCE_VERSION)

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=
PKG_SOURCE_SUBDIR:=gst-omx-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=gst-omx-$(PKG_VERSION).tar.gz
PKG_BUILD_DIR:=$(BUILD_DIR)/gst-omx-$(PKG_VERSION)

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

PKG_BUILD_DEPENDS:= libgstreamer1 gstreamer1-plugins-base

include $(INCLUDE_DIR)/package.mk

define Package/gst1-omx
  CATEGORY:=Multimedia
  SECTION:=multimedia
  TITLE:=GStreamer OpenMAX plugin
  DEPENDS:= +libgstreamer1 +gstreamer1-plugins-base \
	    +libgst1audio +libgst1pbutils +libgst1video
endef

define Package/gst1-omx/description
 GStreamer OpenMAX IL wrapper plugin
endef

GST_VERSION:=1.0

define Build/Prepare
endef

define Package/gst1-omx/install
endef

$(eval $(call BuildPackage,gst1-omx))
