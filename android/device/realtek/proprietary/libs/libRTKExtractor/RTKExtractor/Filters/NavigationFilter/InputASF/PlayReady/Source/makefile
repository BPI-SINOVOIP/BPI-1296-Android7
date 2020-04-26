#**@@@+++@@@@*********************************************************
#*
#* Microsoft PlayReady
#* Copyright (c) Microsoft Corporation. All rights reserved.
#*
#**@@@---@@@@********************************************************/

#include ../DrmSetEnvironment.mk

SUBDIRS = certs crypto constants core initiators blackbox devicedevcert devcertparser secure_clock metering synclist \
          oem drmmanager cmdlnpars wmdrmcert domain domainstore xmr noncestore ndt ndtmgr ndtasf ndr msdb 

all: 	
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

clean: 	
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done


