#
#
#$Id: Makefile 3761 2006-05-18 01:06:16Z kfry $
#
#Copyright (c) 2003-2006 DivX, Inc. All rights reserved.
#
#This software is the confidential and proprietary information of DivX,
#Inc. and may be used only in accordance with the terms of your license from
#DivX, Inc.

include ./common.mk

LIB			:= $(LIB_DIR)/libMenuController.a
SOURCES 	:= divxMenuControllerAPI Memoryalloc MenuChunk 

ifdef WCHARUTILITY
SOURCES += wcharutility
endif

OBJS 		:= $(foreach src,$(SOURCES),$(src).o)
INCLUDE_DIRS	+= -I../ADP-4.0.0/common


all: $(LIB)

$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $(LIB) $(OBJS)
	
$(OBJS): %.o: %.c

clean:
	rm -f $(OBJS) $(LIB)
