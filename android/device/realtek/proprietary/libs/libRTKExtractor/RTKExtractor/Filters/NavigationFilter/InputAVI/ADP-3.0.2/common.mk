#Determine if the system is a powerpc system
#Add other atuo-detect for CPU as required
#Set to X86 is not a PPC, however may be performing cross compilation on X86 so allow manual setting of
#CPU after detection

include ../../../../../../../include/MakeConfig
LIB_DIR = ../../../../../../../lib/


ifeq ($(TARGET_BOARD), YES)
	CPU = MIPS
else
	CPU = X86
endif

#ifeq "$(findstring ppc,$(shell uname -m))" "ppc"
#   CPU = POWERPC
#else
#   CPU = X86
#endif

#echo $(CPU)
    	
# force CPU here for example when performing cross compilation or other CPUs not
#explicitly supported yet

#CPU = X86
#CPU = ARM
#CPU = POWERPC
#CPU = MIPS


# Platform architecture
ifeq "$(strip $(CPU))" "X86"
ARCH =
#### Processor specific optimization - asm routines etc. Comment out for generic implementations
X86ACCEL = 1
ifdef X86ACCEL
CPU_OPTIMIZE = -DARCH_X86 -DX86
else
CPU_OPTIMIZE = -DARCH_GENERIC
endif
## create seperate make variable to enable x86 acceleration in makefiles
else
ifeq "$(strip $(CPU))" "ARM"
ARCH = arm-elf-
#### Processor specific optimization - asm routines etc
# Arm currently uses generic code
CPU_OPTIMIZE = -DARCH_GENERIC
else
ifeq "$(strip $(CPU))" "POWERPC"
ARCH = 
#### Processor specific optimization - asm routines etc
# Arm currently uses generic code
CPU_OPTIMIZE = -DARCH_GENERIC -DPOWERPC
else
ifeq "$(strip $(CPU))" "MIPS"
ARCH = mipsel-linux-
endif
endif
endif
endif


#Tools
CC = $(MYCC)
CXX = $(MYCXX)
LDXX = $(CXX)
LD = $(CC)
AR = $(MYAR)

AS = nasm


PLATFORM = -DLINUX
DEBUG = -g

#Additional debugging output for codec libraries
#DEBUG += -D_DEBUG

#OPTIMIZE = -O2

#Select Large File Support (>2GB)
# comment out LARGE_FILE_SUPPORT definition to use 32 bit file calls
LARGE_FILE_SUPPORT=1
ifdef LARGE_FILE_SUPPORT
LFS = -DLARGE_FILE_SUPPORT -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
else
LFS= -DSMALL_FILE_SUPPORT
endif

## Compilation flags
CFLAGS += $(DEFINES) $(DEBUG) $(PLATFORM) $(OPTIMIZE) $(CPU_OPTIMIZE) $(INCLUDE_DIRS) $(LFS)
CXXFLAGS = $(CFLAGS)
LDFLAGS = $(OPTIMIZE)
ARFLAGS +=

