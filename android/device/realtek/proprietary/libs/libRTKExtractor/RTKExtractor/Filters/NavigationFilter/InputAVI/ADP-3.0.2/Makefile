
all: libraries

libraries:
	cd libDrmCommon/src && $(MAKE) -e
	cd libDrmDecrypt/src && $(MAKE) -e 
	cd libAES/src && $(MAKE) -e

.PHONEY: clean clean_libraries clean_codec clean_applications clean_install

clean: clean_libraries

clean_libraries:
	cd libDrmCommon/src && $(MAKE) clean
	cd libDrmDecrypt/src && $(MAKE) clean
	cd libAES/src && $(MAKE) clean

