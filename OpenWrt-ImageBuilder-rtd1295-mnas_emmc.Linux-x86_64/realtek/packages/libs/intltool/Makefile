#
# Copyright (C) 2009-2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=intltool
PKG_LIBVER:=0.51
PKG_VERSION:=$(PKG_LIBVER).0
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=@GNOME/intltool/$(PKG_LIBVER)
PKG_SOURCE_URL=https://launchpad.net/intltool/trunk/0.51.0/+download/
PKG_MD5SUM:=
PKG_MAINTAINER:=Ted Hess <thess@kitschensync.net>

PKG_HOST_ONLY:=1

include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/package.mk

define Package/intltool
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=set of tools to centralize translation using GNU gettext
  URL:=http://www.freedesktop.org/wiki/Software/intltool
  BUILDONLY:=1
endef

define Package/intltool/description
  intltool is a set of tools to centralize translation of many different
  file formats using GNU gettext-compatible PO files.
endef

define Require/perl-xml-parser
  perl -e 'use XML::Parser;'
endef

$(eval $(call Require,perl-xml-parser, \
	Please install Perl XML::Parser \
))

define Build/HostInstall/PatchShebang
	find \
	    $(STAGING_DIR_HOST)/bin \
	    -name "intltool*" \
	    -type f -executable -exec grep -Iq "$(STAGING_DIR_HOST)" {} \; -and -print \
	    | xargs --no-run-if-empty sed -i -r 's,^#!.?$(subst +,\+,$(STAGING_DIR_HOST))(/usr)?/bin/([^ ]+) *.*$$$$,#!/usr/bin/env \2,g'
endef
Hooks/HostInstall/Post += Build/HostInstall/PatchShebang

$(eval $(call BuildPackage,intltool))
$(eval $(call HostBuild))
