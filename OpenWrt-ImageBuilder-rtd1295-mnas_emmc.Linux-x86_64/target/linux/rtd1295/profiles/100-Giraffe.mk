#
# Copyright (C) 2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

define Profile/giraffe-1GB
  NAME:=Giraffe 1GB board
endef

define Profile/giraffe-1GB/Description
	Giraffe board V0.1 1GB
endef

$(eval $(call Profile,giraffe-1GB))

define Profile/giraffe-2GB
  NAME:=Giraffe 2GB board
endef

define Profile/giraffe-2GB/Description
	Giraffe board V0.1 2GB
endef

$(eval $(call Profile,giraffe-2GB))

define Profile/giraffe-2GB-tee    
  NAME:=Giraffe 2GB-tee board 
endef   

define Profile/giraffe-2GB-tee/Description    
    Giraffe board V0.1 2GB-tee    
endef   

$(eval $(call Profile,giraffe-2GB-tee))

define Profile/giraffe-1.5GB
  NAME:=Giraffe 1.5GB board
endef

define Profile/giraffe-1.5GB/Description
	Giraffe 1.5GB board
endef

$(eval $(call Profile,giraffe-1.5GB))
