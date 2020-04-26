#**@@@+++@@@@*********************************************************
#*
#* Microsoft Portable Build
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#include ../../../include/MakeConfig
_DRM_SOURCE_PATH := ../
include ../../DrmSetEnvironment.mk

rm=/bin/rm -f
CC= $(MYCC)
LINK= -=LINK=-
DEFS=  
AR= $(MYAR) cq
RANLIB= $(MYRANLIB)
LIBNAME= $(_DRM_LIB_PATH)libdrmcore.a

SUBDIRS= pkdrmchkdbgforpc

DEFINES+= $(_C_COMPILER_SWITCHES) $(_C_DEFINE_SWITCHES) $(_C_INCLUDE_SWITCHES) -DSYS_UNIX=1
CFLAGS= $(DEFINES) $(COMPILE_OPT)


SRCS =     drmaes.c                   \
    drmbase64.c                \
    drmbytemanip.c             \
    drmcbc.c                   \
    drmchain.c                 \
    drmcipher.c                \
    drmcleanstore.c            \
    drmcrt.c                   \
    drmdatastore.c             \
    drmdebug.c                 \
    drmdevicestore.c           \
    drmdevstore.c              \
    drmembeddedstore.c         \
    drmembedding.c             \
    drmexpreval.c              \
    drmexprfunc.c              \
    drmexprvariable.c          \
    drmhash.c                  \
    drmhdrbuilder.c            \
    drmhdrparser.c             \
    drmhds.c                   \
    drmhmac.c                  \
    drmkeygen.c                \
    drmlicacqreq.c             \
    drmlicacqresp.c            \
    drmliceval.c               \
    drmlicparser.c             \
    drmlicstore.c              \
    drmmd5.c                   \
    drmorderedlist.c           \
    drmpkcrypto.c              \
    drmprofile.c               \
    drmptrlist.c               \
    drmrevocation.c            \
    drmsecstore.c              \
    drmsha1.c                  \
    drmstkalloc.c              \
    drmutilities.c             \
    drmutilitieslite.c         \
    drmutilitiescertcache.c    \
    drmviewrights.c            \
    drmxmlbuilder.c            \
    drmxmlbuilderlite.c        \
    drmxmlparser.c             \
    drmxmlparserlite.c         \
    drmxmlutilities.c          \
    drmoutputlevels.c          \
    drmint64.c                 \
    drmplayreadyobj.c          \
    drmrsa.c                   \
    drmrevocationstore.c       \
    drmmetercertstore.c        \
    drmcontract.c	       \
    drmxmlbuildera.c           \
    drmutf.c                   \
    drmxmlsig.c                \
    drmsoapxmlutility.c        \
    drmlicacqv3.c              \
    drmstrsafe.c               \
    drmbcrlparser.c     

OBJS = drmaes.o                \
    drmbase64.o                \
    drmbytemanip.o             \
    drmcbc.o                   \
    drmchain.o                 \
    drmcipher.o                \
    drmcleanstore.o            \
    drmcrt.o                   \
    drmdatastore.o             \
    drmdebug.o                 \
    drmdevicestore.o           \
    drmdevstore.o              \
    drmembeddedstore.o         \
    drmembedding.o             \
    drmexpreval.o              \
    drmexprfunc.o              \
    drmexprvariable.o          \
    drmhash.o                  \
    drmhdrbuilder.o            \
    drmhdrparser.o             \
    drmhds.o                   \
    drmhmac.o                  \
    drmkeygen.o                \
    drmlicacqreq.o             \
    drmlicacqresp.o            \
    drmliceval.o               \
    drmlicparser.o             \
    drmlicstore.o              \
    drmmd5.o                   \
    drmorderedlist.o           \
    drmpkcrypto.o              \
    drmprofile.o               \
    drmptrlist.o               \
    drmrevocation.o            \
    drmsecstore.o              \
    drmsha1.o                  \
    drmstkalloc.o              \
    drmutilities.o             \
    drmutilitieslite.o         \
    drmutilitiescertcache.o    \
    drmviewrights.o            \
    drmxmlbuilder.o            \
    drmxmlbuilderlite.o        \
    drmxmlparser.o             \
    drmxmlparserlite.o         \
    drmxmlutilities.o          \
    drmoutputlevels.o          \
    drmint64.o                 \
    drmplayreadyobj.o          \
    drmrsa.o                   \
    drmrevocationstore.o       \
    drmmetercertstore.o        \
    drmcontract.o	       \
    drmxmlbuildera.o           \
    drmutf.o                   \
    drmxmlsig.o                \
    drmsoapxmlutility.o        \
    drmlicacqv3.o              \
    drmstrsafe.o               \
    drmbcrlparser.o 


.c.o:
	$(SHOW_COMMAND)$(rm) -f $@
	$(SHOW_COMMAND)echo -=--=--=- [$*.c] -=--=--=--=--=--=--=--=--=-
	$(SHOW_COMMAND)$(CC) $(CFLAGS) $(DEBUGFLAG) -c -Wall $*.c

all: $(LIBNAME)
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

$(LIBNAME) : $(OBJS)
	$(SHOW_COMMAND)$(rm) $@
	$(SHOW_COMMAND)$(AR) $@ $(OBJS)
	$(SHOW_COMMAND)$(RANLIB) $@

clean:
	$(SHOW_COMMAND)$(rm) $(OBJS) $(LIBNAME) core *~
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

