DTV_ROOT	:= ../../..
INC_DIR		:= $(DTV_ROOT)/include

#============================================================
include $(DTV_ROOT)/pvr_make.config
include $(DTV_ROOT)/librtd/si4/make.config

#============================================================
SRCS		:= $(wildcard *.c)
OBJS		:= $(patsubst %.c, %.o, $(SRCS))

AGENT_MOD	:= siagent2_dvr

all:
	$(SHOW_COMMAND)echo -=--=--=- [$(SRCS)] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(SRCS) -c $(CFLAGS) $(INCLUDES)
	make -C $(AGENT_MOD) -f Makefile.$(MK_EXT)

clean:
	rm  *.o *~ -rf
	make -C $(AGENT_MOD) -f Makefile.$(MK_EXT) clean
