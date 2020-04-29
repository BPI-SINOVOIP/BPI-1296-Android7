BOARDNAME := RTD1295 generic media NAS with eMMC
FEATURES += squashfs

define Target/Description
	Build NAS firmware image with transcode for Realtek RTD1295 SoC devices.
	Currently produces eMMC image for Giraffe board.
endef
