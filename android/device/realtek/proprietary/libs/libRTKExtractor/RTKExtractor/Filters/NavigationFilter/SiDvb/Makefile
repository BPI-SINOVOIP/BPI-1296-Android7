DTV_ROOT	:= .
include pvr_make.config

INCLUDES	:= -I. -Iinclude -I../../../Include/Filters/NavigationFilter
CFLAGS		:= -g -Wall

ifeq ($(USE_NEW_CF), Y)
CF_DIR		:= $(DTV_ROOT)/../../../Application/Projects/SantaMaria/Component/CFramework
else
CF_DIR		:= $(DTV_ROOT)/librtd/CFramework
endif


all:
	make -C $(CF_DIR)
	make -C librtd/si4
#	$(CC) $(SRCS) -c $(INCLUDES) $(CFLAGS)


clean:
	make -C $(CF_DIR) clean
	make -C librtd/si4 clean
#	rm *.o *~ -f
