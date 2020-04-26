DTV_ROOT	:= ../../..
INC_DIR		:= $(DTV_ROOT)/include

#============================================================
# include $(DTV_ROOT)/include/configGlobal.$(MK_EXT)
include $(DTV_ROOT)/pvr_make.config

#============================================================
#CC			:= mips-uclibc-gcc
#AR			:= mips-uclibc-ar rcs
CFLAGS		:= -g -Wall -DCF_LOGGER_COLOR
INCLUDES	:= -I $(INC_DIR) -I $(DTV_ROOT)/../../../Include/Filters/NavigationFilter/SiDvb/include

#============================================================
SRCS		:= $(wildcard *.c)
OBJS		:= $(patsubst %.c, %.o, $(SRCS))


all:
	$(SHOW_COMMAND)echo -=--=--=- [$(SRCS)] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(SRCS) -c $(CFLAGS) $(INCLUDES)
	
clean:
	rm  *.o *~ -rf
