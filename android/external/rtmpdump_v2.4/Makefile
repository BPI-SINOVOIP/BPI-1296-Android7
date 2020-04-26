##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 11:11:28 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
#-include ../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
rm=/bin/rm -f
#CC= $(MYCC)
#CXX=$(MYCXX)
#STRIP=$(MYSTRIP)
CC=/opt/android-ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc
CXX=/opt/android-ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-g++
STRIP=/opt/android-ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip
DEFINES= -DDVR -DUSE_BIONIC -D_RTK_ANDROID_ARM_ -mcpu=cortex-a7  -DLINUX_NPTL --sysroot=/opt/android-ndk//platforms/android-18/arch-arm -I/opt/android-ndk//platforms/android-18/arch-arm/usr/include -D__u_char_defined -D__daddr_t_defined
DEFS=
PROGNAME= rtmpdump

CRYPTO=OPENSSL
CRYPTO_DEF=$(DEF_$(CRYPTO))
VERSION=v2.4
DEF=-DRTMPDUMP_VERSION=\"$(VERSION)\" $(CRYPTO_DEF) $(XDEF)

LIBRTMP=librtmp/librtmp.a

CXXFLAGS := -I$(INC_DIR) -Wall
CFLAGS= $(DEFINES) $(COMPILE_OPT) $(DEF) 
LIBS= -L../../out/target/product/phoenix/system/lib

LIBS+= -Wl,--start-group

LIBS += librtmp/librtmp.a
#  CFLAGS += -static

LIBS+= -lssl -lcrypto -lz

SRCS += rtmpdump.cpp
OBJS += rtmpdump.o

DEFINES+= $(INCLUDES) $(DEFS) -DSYS_UNIX=1 $(MYDEFS)
LIBS+= -Wl,--end-group



.c.o:
	$(rm) $@
	$(CC) $(CFLAGS) $(DEBUGFLAG) $(WARNING) -O3 -c $< -o $@ 
	
all: $(LIBRTMP) $(PROGNAME) 

$(PROGNAME) : $(LIBRTMP) $(OBJS)
	$(CC) $(CXXFLAGS) $(CFLAGS) -o $(PROGNAME).debug $(OBJS) $(LIBS) -Wl,-Map,mapfile
#	$(CC) $(CXXFLAGS) $(CFLAGS) -o $(PROGNAME).debug $(OBJS) $(LDFLAGS) $(LIBS) -Wl,-Map,mapfile
	$(STRIP) -o $(PROGNAME) $(PROGNAME).debug

FORCE:

$(LIBRTMP): FORCE
	@cd librtmp; $(MAKE) all


clean:
	$(rm) $(OBJS) $(PROGNAME) $(PROGNAME).debug core *~

link: RM all

RM: 
	$(rm) $(PROGNAME)
