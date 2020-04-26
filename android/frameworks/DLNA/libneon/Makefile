
SUBDIRS = src

all: 
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
clean: 
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done

