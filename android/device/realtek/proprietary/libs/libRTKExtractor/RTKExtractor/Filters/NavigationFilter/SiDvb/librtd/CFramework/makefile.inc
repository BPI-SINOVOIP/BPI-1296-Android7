DTV_ROOT		:= ../..
INC_DIR			:= $(DTV_ROOT)/include

#============================================================
# include $(DTV_ROOT)/include/configGlobal.$(MK_EXT)
include $(DTV_ROOT)/pvr_make.config

#============================================================
#CC				:= mips-uclibc-gcc
#AR				:= mips-uclibc-ar rcs
CFLAGS			:= -g -Wall
INCLUDES		:= -I $(INC_DIR) -I $(DTV_ROOT)/../../../Include/Filters/NavigationFilter/SiDvb/include

#============================================================
SRCS			:= $(wildcard *.c)
T_SRCS			:= $(wildcard *.c)			\
					$(wildcard memory/*.c)	\
					$(wildcard message/*.c)	\
					$(wildcard logger/*.c)	\
					$(wildcard timer/*.c)

OBJS			:= $(patsubst %.c, %.o, $(T_SRCS))

LIB_NAME		:= cf_$(MK_EXT).lib

all:
	$(SHOW_COMMAND)echo -=--=--=- [$(SRCS)] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(SRCS) -c $(CFLAGS) $(INCLUDES)
	make -C memory		-f Makefile.$(MK_EXT)
	make -C timer		-f Makefile.$(MK_EXT)
	make -C message		-f Makefile.$(MK_EXT)
	make -C logger		-f Makefile.$(MK_EXT)
	$(AR) $(LIB_NAME) $(OBJS)
	cp $(LIB_NAME) $(DTV_ROOT)/../../../../lib/libCF.a
	
clean:
	make -C memory 		-f Makefile.$(MK_EXT) clean
	make -C timer		-f Makefile.$(MK_EXT) clean
	make -C message		-f Makefile.$(MK_EXT) clean
	make -C logger		-f Makefile.$(MK_EXT) clean
	rm $(LIB_NAME) *.o *~ -f
