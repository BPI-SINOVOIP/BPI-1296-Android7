all:
	@echo
	@echo 'Building wpa_supplicant library'
	@echo
	make -f Makefile_wpa_supplicant
	rm -f *.o
	@echo
	@echo 'Building hostapd library'
	@echo
	make -f Makefile_hostapd
	@echo

clean:
	make clean -f Makefile_wpa_supplicant
	make clean -f Makefile_hostapd
