#**@@@+++@@@@*********************************************************
#*
#* Microsoft Portable Build
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#include ../../../../../../../../include/MakeConfig
_DRM_SOURCE_PATH := ../../
include ../../../DrmSetEnvironment.mk

rm=/bin/rm -f
CC= $(MYCC)
DEFS=  
PROGNAME= player

INCLUDES += \
           -I$(_DRM_SOURCE_PATH)tools/common

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)

CFLAGS += -static

LIBS = -Wl,--start-group
LIBS += -L$(_DRM_LIB_PATH) $(TARGETLIBS)

#LIBS+= -L. -lefence_344
LIBS += -Wl,--end-group

_DRM_COMMON_LIBS = \
    -ldrmcore               \
    -lcmdlnpars   \
    -ldrmcerts             \
    -ldrmndcerts        \
    -ldrmxmrparser    \
    -ldrmxmrbuilder  \
    -ldrminitiators   \
    -ldrmnoncestore   \
    -loem_ansi    \
    -loem_common    \
    -loem_trace_ansi    \
    -ldrmdomain    \
    -ldrmdomainstore    \
    -ldrmbignum    \
    -ldrmpkcrypto    \
    -ldrmsha256    \
    -ldrmcrypto    \
    
_DRM_COMMON_LIBS += -lpkdrmchkdbgforpc \
    -ldrmconstants    \
    -ldrmxmrcommon

_DRM_OEM_MAIN_LIB = -loemmain_ansi

TARGETLIBS =                                  \
    $(_DRM_COMMON_LIBS)                       \
    $(_DRM_OEM_MAIN_LIB)                      \
    -ldrmblackbox      \
    -ldrmmanager        \
    -ldrmdevcert \
    -ldrmmetering      \
    -ldrmsynclist      \
    -ldrmdevicedevcert     \
    -ldrmsecureclock         \
    -lwmdrmcert         \
    -loemrsa                  \
    -ldrmtoolscommon \
    -loemaes                \

SRCS = player.c 

OBJS = player.o 

.c.o:                                                                     
	$(SHOW_COMMAND)$(rm) -f $@                                        
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-  
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.c         

all: $(PROGNAME)

$(PROGNAME) : $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGNAME) $(LDFLAGS) $(OBJS) $(LIBS)


clean:
	$(rm) $(OBJS) $(PROGNAME) core* *~


