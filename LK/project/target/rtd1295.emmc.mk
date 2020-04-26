# main project for rtd1295 (LK was loaded from SPI)
ARCH := arm64
ARM_CPU := cortex-a53
TARGET := rtd1295

CHIP_ID ?= rtd1295
CUSTOMER_ID ?= demo
CHIP_TYPE ?= 0002
PRJ ?= 1295_force_emmc_nS_nE

NAS_ENABLE ?= 0
LK_FW_LOAD ?= 1
HW_MCP ?= 1
JTAG_DEBUG ?= 0

# FLASH TYPE : EMMC_FLASH / SPI_FLASH / NAND_FLASH
FLASH_TYPE ?= EMMC_FLASH
ANDROID_REBOOT_CMD := 1

# Customized
LK_WAS_LOADED_FROM_SPI := 1
FORCE_BOOT_FROM_USB := 1
SUPPORT_BOOT_RU_CMD := 1

#PWM setting
PWM_0_PIN_0 := 0
PWM_0_PIN_1 := 0
PWM_1_PIN_0 := 0
PWM_1_PIN_1 := 0
PWM_2_PIN_0 := 0
PWM_2_PIN_1 := 0
PWM_3_PIN_0 := 0
PWM_3_PIN_1 := 0

ifeq ($(NAS_ENABLE),1)
KERNEL_LOADADDR := 0x03000000
FDT_LOADADDR := 0x02100000
ROOTFS_LOADADDR := 0x02200000
AUDIO_LOADADDR := 0x01b00000
  ifeq ($(FLASH_TYPE), NAND_FLASH)
  FACTORY_BLOCKS := 4
  endif # FLASH_TYPE
else #!NAS_ENABLE
KERNEL_LOADADDR := 0x03000000
FDT_LOADADDR := 0x02100000
ROOTFS_LOADADDR := 0x02200000
AUDIO_LOADADDR := 0x0F900000
endif

HEAP_START := 0x12000000
HEAP_SIZE := 0x08000000
FACTORY_START := 0x220000
FACTORY_SIZE := 0x400000

WITH_KERNEL_VM=0
WITH_CPP_SUPPORT=true
WITH_LINKER_GC := 0

MODULES += \
	app/rtkboot \
	app/fatload \
	external/lib/factory \
	external/lib/mincrypt \
	external/lib/rtkfdt \
	external/lib/tar \
	external/lib/xyzmodem \
	external/lib/mp_tool \
	external/lib/i2c \
	external/lib/pmic \
	external/lib/ping \
	external/lib/tftp_client \
	external/lib/fastboot \
	lib/aes \
	lib/aes/test \
	lib/cksum \
	lib/debugcommands \
	lib/libm \
	lib/sysparam \
	lib/version

GLOBAL_DEFINES += \
	COUNTER_FREQUENCY=27000000 \
	SYSPARAM_ALLOW_WRITE=1 \
	BSP_REALTEK=1 \
	SLAVE_CPU_SPIN_TABLE \
	RTK_BCB_IN_FACTORY \
	HDMITX_HPD_IGPIO_NUM=6 \
	HDMI_ONE_STEP \
	DEFAULT_TV_SYSTEM=25 \
	ENABLE_PANIC_SHELL=1 \
	CUSTOM_DEFAULT_STACK_SIZE=8192 \
	JTAG_DEBUG=$(JTAG_DEBUG)

ifeq ($(JTAG_DEBUG),0)
GLOBAL_DEFINES += INSTALL_BUTTON_GPIO_NUM=8
endif
