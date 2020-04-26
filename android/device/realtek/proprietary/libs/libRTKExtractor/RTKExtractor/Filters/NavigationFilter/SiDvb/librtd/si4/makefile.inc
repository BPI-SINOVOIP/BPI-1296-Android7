DTV_ROOT	:= ../..
INC_DIR		:= $(DTV_ROOT)/include

#============================================================
include $(DTV_ROOT)/pvr_make.config
include make.config

#============================================================
T_SRCS		=
T_SRCS		+= agent/*.c
T_SRCS		+= api/*.c
T_SRCS		+= channel/*.c
T_SRCS		+= collector/*.c
T_SRCS		+= database/*.c
T_SRCS		+= util/*.c
T_SRCS		+= tpInterface/$(TARGET_SYSTEM)/*.c

ifeq ($(ENABLE_SYSTEM_SOFTWARE_UPDATE), YES)
#T_SRCS      += agent/ssu/*.c
endif

OBJS		:= $(patsubst %.c, %.o, $(T_SRCS))

LIB_NAME	:= si_$(MK_EXT).lib

#============================================================

all:
	rm -f $(DTV_ROOT)/../../../../lib/libSiDvb.a
	make -C channel		-f Makefile.$(MK_EXT)
	make -C database	-f Makefile.$(MK_EXT)
	make -C collector	-f Makefile.$(MK_EXT)
	make -C agent		-f Makefile.$(MK_EXT)
	make -C api			-f Makefile.$(MK_EXT)
	make -C util		-f Makefile.$(MK_EXT)
	make -C tpInterface	-f Makefile.$(MK_EXT)
#	make -C ssu			-f Makefile.$(MK_EXT)
	$(AR) $(LIB_NAME) $(OBJS)
	$(RANLIB) $(LIB_NAME)
	cp $(LIB_NAME) $(DTV_ROOT)/../../../../lib/libSiDvb.a

clean:
	make -C channel		-f Makefile.$(MK_EXT) clean
	make -C database	-f Makefile.$(MK_EXT) clean
	make -C collector	-f Makefile.$(MK_EXT) clean
	make -C agent		-f Makefile.$(MK_EXT) clean
	make -C api			-f Makefile.$(MK_EXT) clean
	make -C util		-f Makefile.$(MK_EXT) clean
	make -C tpInterface -f Makefile.$(MK_EXT) clean
	rm -f $(DTV_ROOT)/../../../../lib/libSiDvb.a
	rm -f $(LIB_NAME) *~

