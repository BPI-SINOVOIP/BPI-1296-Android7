#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/bananapi-2GB
  NAME:=Bananapi 2GB board
endef

define Profile/bananapi-2GB/Description
	Bananapi W2 board V1.0 2GB
endef

$(eval $(call Profile,bananapi-2GB))

define Profile/ccbox-4GB
  NAME:=CCbox 4GB board
endef

define Profile/ccbox-4GB/Description
	CCbox board V1.0 4GB
endef

$(eval $(call Profile,ccbox-4GB))
