BOARDNAME := RTD1295 media NAS with eMMC
FEATURES += squashfs
DEFAULT_PACKAGES += kmod-mali

define Target/Description
	Build media NAS firmware image for Realtek RTD1295 SoC devices.
	Currently produces eMMC image for Giraffe board.
endef
