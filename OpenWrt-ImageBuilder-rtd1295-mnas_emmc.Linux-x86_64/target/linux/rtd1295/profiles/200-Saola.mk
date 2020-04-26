#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/saola-1GB
  NAME:=Saola 1GB board
endef

define Profile/saola-1GB/Description
	Saola board V1.0 1GB
endef

$(eval $(call Profile,saola-1GB))

define Profile/saola-2GB
  NAME:=Saola 2GB board
endef

define Profile/saola-2GB/Description
	Saola board V0.1 2GB
endef

$(eval $(call Profile,saola-2GB))

define Profile/saola-2GB-tee
  NAME:=Saola 2GB-tee board
endef

define Profile/saola-2GB-tee/Description
	Saola board V0.1 2GB-tee
endef

$(eval $(call Profile,saola-2GB-tee))

define Profile/saola-3GB
  NAME:=Saola 3GB board
endef

define Profile/saola-3GB/Description
	Saola board V1.0 3GB
endef

$(eval $(call Profile,saola-3GB))

define Profile/saola-4GB-tee
  NAME:=Saola 4GB-tee board
endef

define Profile/saola-4GB-tee/Description
	Saola board V0.1 4GB-tee
endef

$(eval $(call Profile,saola-4GB-tee))

