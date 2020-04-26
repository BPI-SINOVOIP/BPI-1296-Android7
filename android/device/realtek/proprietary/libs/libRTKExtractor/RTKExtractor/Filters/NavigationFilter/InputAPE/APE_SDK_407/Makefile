##----------------------------------------------------------------------------
# Created with genmake.pl v1.1 on Tue Sep  7 10:40:25 2004
# genmake.pl home: http://muquit.com/muquit/software/
# Copryright: GNU GPL (http://www.gnu.org/copyleft/gpl.html)
##----------------------------------------------------------------------------
#-include ../../../../branch_src_sharedMemory_integration/Tools/MConfig/.config
include ../../../../../include/MakeConfig

SUBDIRS = Shared MACLib #Console 

all:
	set -e; for i in $(SUBDIRS); do $(MAKE) -C $$i; done
clean:
	set -e; for i in $(SUBDIRS); do $(MAKE) clean -C $$i; done
f:
	@echo $(subst |, , $(APCFG_FLAGS))

