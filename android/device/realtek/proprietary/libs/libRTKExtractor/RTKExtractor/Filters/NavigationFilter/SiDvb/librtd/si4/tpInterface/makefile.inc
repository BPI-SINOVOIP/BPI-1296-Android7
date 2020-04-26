DTV_ROOT        := ../../..

#============================================================
include $(DTV_ROOT)/pvr_make.config
include $(DTV_ROOT)/librtd/si4/make.config

#============================================================

all:
	make -C $(TARGET_SYSTEM) -f Makefile.$(MK_EXT)

clean:
	make -C $(TARGET_SYSTEM) -f Makefile.$(MK_EXT) clean
