SUBDIRS_SYSTEM = IPC/src IPC/generate/src/system
SUBDIRS = IPC/script

all:
	echo "ycyang do all"
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done

system-all:
	echo "ycyang do system-all"
	set -e; for i in $(SUBDIRS_SYSTEM); do $(MAKE) -C $$i; done

clean:
	set -e; for i in $(SUBDIRS_SYSTEM); do $(MAKE) clean -C $$i; done;
