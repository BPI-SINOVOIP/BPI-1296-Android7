all:	UDPserver


#CROSS_COMPILE=mips-wrs-linux-gnu-mips_74k_softfp-glibc_small-
CC = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip

#CC	= mips-uclibc-gcc
#STRIP	= mips-uclibc-strip

INCS = -I$(TOP_USERS_DIR)/include
CFLAGS	= $(INCS)
IFLAGS	=
LDFLAGS =

ifeq ($(SLINK),1)
CFLAGS  += -ffunction-sections -fdata-sections
LDFLAGS += --static -Wl,--gc-sections
endif


DEBUG	= -Wall -Os

UDPserver: Makefile UDPserver.o
	$(CC) -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $(LDFLAGS) UDPserver.o
	$(STRIP) $@
romfs:
	$(ROMFSINST) /bin/UDPserver

clean:
	rm -f UDPserver *.o 

UDPserver.o: UDPserver.c
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
