#**@@@+++@@@@*********************************************************
#*
#* Microsoft Portable Build
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

SUBDIRS = inmemory dvs

all: 	
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean: 	
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

