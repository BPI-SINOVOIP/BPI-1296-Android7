# main project for rtd1355
ARCH := arm64
ARM_CPU := cortex-a53
TARGET := rtd1395_ft
PLATFORM_RTD1355 := 1

CHIP_ID ?= rtd1395
SUB_CHIP_ID ?= rtd1355
CUSTOMER_ID ?= demo
CHIP_TYPE ?= 0000
PRJ ?= 1355_force_emmc

NAS_ENABLE ?= 0
LK_FW_LOAD ?= 1

# FLASH TYPE : EMMC_FLASH / SPI_FLASH / NAND_FLASH
FLASH_TYPE ?= EMMC_FLASH
ANDROID_REBOOT_CMD := 1

#PWM setting
PWM_0_PIN_0 := 0
PWM_0_PIN_1 := 0
PWM_1_PIN_0 := 0
PWM_1_PIN_1 := 0
PWM_2_PIN_0 := 0
PWM_2_PIN_1 := 0
PWM_3_PIN_0 := 0
PWM_3_PIN_1 := 0

# 1395 USB GPIO Setting
USB_GPIO_SETTING := 1
# define 1395 USB GPIO control
# Port 0, DRD, TYPE_C
USB_PORT0_GPIO_TYPE := ISOGPIO
USB_PORT0_GPIO_NUM := 48
USB_PORT0_TYPE_C_SWITCH_GPIO_TYPE := ISOGPIO
USB_PORT0_TYPE_C_SWITCH_GPIO_NUM := 11
# Port 1, xhci u2 host
USB_PORT1_GPIO_TYPE := NONE
USB_PORT1_GPIO_NUM := 0
# Port 2, xhci u2 host
USB_PORT2_GPIO_TYPE := ISOGPIO
USB_PORT2_GPIO_NUM := 49

# pmic command
USE_PMIC_G2237 := 1

ifeq ($(NAS_ENABLE),1)
KERNEL_LOADADDR := 0x03000000
FDT_LOADADDR := 0x02100000
ROOTFS_LOADADDR := 0x02200000
AUDIO_LOADADDR := 0x01b00000
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
	external/lib/ft_tool \
	external/lib/fastboot \
	lib/aes \
	lib/aes/test \
	lib/cksum \
	lib/debugcommands \
	lib/libm \
	lib/sysparam \
	lib/ping \
	lib/tftp_client \
	lib/version

GLOBAL_DEFINES += \
	COUNTER_FREQUENCY=27000000 \
	SYSPARAM_ALLOW_WRITE=1 \
	BSP_REALTEK=1 \
	SLAVE_CPU_SPIN_TABLE \
	RTK_BCB_IN_FACTORY \
	HDMITX_HPD_IGPIO_NUM=7 \
	HDMITX_MODE=1 \
	INSTALL_BUTTON_GPIO_NUM=52 \
	DEFAULT_TV_SYSTEM=25 \
	ENABLE_PANIC_SHELL=1 \
	FT_TOOL=1 \
	CUSTOM_DEFAULT_STACK_SIZE=8192
