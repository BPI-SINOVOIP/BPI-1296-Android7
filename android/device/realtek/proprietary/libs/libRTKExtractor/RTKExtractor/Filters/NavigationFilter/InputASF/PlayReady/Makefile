#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#include ../DrmSetEnvironment.mk

SUBDIRS = Source

all: 	
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean: 	
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done


