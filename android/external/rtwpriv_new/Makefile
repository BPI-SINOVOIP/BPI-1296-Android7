########################################################
#  MakeFile for Realtek Wireless LAN Private TOOL.     #
#                                                      #
########################################################

#CC      ?= gcc
#ARM32
#CC=arm-linux-androideabi-gcc
#CC= /root/Documents/android-ndk-r10e/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86/bin/arm-linux-androideabi-gcc-4.8 --sysroot=/root/Documents/android-ndk-r10e/platforms/android-21/arch-arm

#ARM64
#CC=aarch64-linux-android-gcc
#x86
#CC=i686-linux-android-gcc
#x86_64
#CC=x86_64-linux-android-gcc
#mips
#CC=mipsel-linux-android-gcc
#mips64
CC=mips64el-linux-android-gcc

CCFLAGS += -Wall
#CCFLAGS += -DDEBUG
BUILDFLAGS := -static

target	:= rtwpriv
object  := $(target).o

GENOBJ = $(CC) $(CCFLAGS) -c


all: $(target)

$(target): $(object)
	$(CC) $(CCFLAGS) $(BUILDFLAGS) $(LIB) $(object) -o $@

%.o: %.c
	$(GENOBJ) $<

.PHONY: clean
clean:
	rm -f *.o *~ $(target)
