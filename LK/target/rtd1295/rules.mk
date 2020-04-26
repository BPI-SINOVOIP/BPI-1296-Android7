LOCAL_DIR := $(GET_LOCAL_DIR)

GLOBAL_INCLUDES += \
    $(LOCAL_DIR)/include

PLATFORM := rtd1295

#include make/module.mk

tools/flash_writer/dvrboot.exe.bin: $(BUILDDIR)/lk.bin  .FORCE
	@echo building flash_writer: $@
	make -C tools/flash_writer \
		BOOTCODE_IS_LK=1 BOOTCODE_LK_PATH=`pwd`/$(BUILDDIR)/lk.bin \
		FLASH_TYPE=$(FLASH_TYPE) PRJ=$(PRJ) \
		CHIP_ID=$(CHIP_ID) CUSTOMER_ID=$(CUSTOMER_ID) CHIP_TYPE=$(CHIP_TYPE) \
		MARKET_ID=$(MARKET_ID) VMX=$(VMX) VMX_3RD_PARTY=$(VMX_3RD_PARTY) VMX_BL=$(VMX_BL) DISABLE_UART_RX=$(DISABLE_UART_RX)
	echo "FLASH_TYPE=$(FLASH_TYPE)" > $(BUILDDIR)/dvrboot_config
	echo "PRJ=$(PRJ)" >> $(BUILDDIR)/dvrboot_config
	echo "CHIP_ID=$(CHIP_ID)" >> $(BUILDDIR)/dvrboot_config
	echo "CUSTOMER_ID=$(CUSTOMER_ID)" >> $(BUILDDIR)/dvrboot_config
	echo "CHIP_TYPE=$(CHIP_TYPE)" >> $(BUILDDIR)/dvrboot_config
	echo "MARKET_ID=$(MARKET_ID)" >> $(BUILDDIR)/dvrboot_config
	echo "VMX=$(VMX)" >> $(BUILDDIR)/dvrboot_config
	echo "VMX_3RD_PARTY=$(VMX_3RD_PARTY)" >> $(BUILDDIR)/dvrboot_config
	echo "VMX_BL=$(VMX_BL)" >> $(BUILDDIR)/dvrboot_config
	echo "DISABLE_UART_RX=$(DISABLE_UART_RX)" >> $(BUILDDIR)/dvrboot_config

$(BUILDDIR)/dvrboot.exe.bin: tools/flash_writer/dvrboot.exe.bin
	@echo copy $< to $@
	$(NOECHO) cp $< $@

efuse_apps: efuse_verify

efuse_verify: efuse_programmer efuse_programmer.vmx
	@echo building efuse_verify
	make -C tools/efuse_verify VMX=$(VMX) VMX_3RD_PARTY=$(VMX_3RD_PARTY) VMX_BL=$(VMX_BL)

efuse_programmer: $(BUILDDIR)/dvrboot.exe.bin
	@echo building efuse_programmer
	make -C tools/efuse_programmer

efuse_programmer.vmx: $(BUILDDIR)/dvrboot.exe.bin
	@echo building efuse_programmer.vmx
	make -C tools/efuse_programmer.vmx

.FORCE:

all:: efuse_apps

