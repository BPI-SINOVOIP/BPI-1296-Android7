LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

WITH_SMP ?= 1

LK_HEAP_IMPLEMENTATION ?= dlmalloc

MODULE_SRCS += \
	$(LOCAL_DIR)/debug.c \
	$(LOCAL_DIR)/platform.c \
	$(LOCAL_DIR)/secondary_boot.S \
	$(LOCAL_DIR)/uart.c \
	$(LOCAL_DIR)/mmu.c \
	$(LOCAL_DIR)/ops.S \
	$(LOCAL_DIR)/cpu.c \
	$(LOCAL_DIR)/rtkemmc.c \
	$(LOCAL_DIR)/rtkspi.c \
	$(LOCAL_DIR)/rtknand.c \
	$(LOCAL_DIR)/ddr.c \
	$(LOCAL_DIR)/bdinfo.c \
	$(LOCAL_DIR)/gpio.c \
	$(LOCAL_DIR)/hdmitx/logo_disp_read_edid.c \
	$(LOCAL_DIR)/hdmitx/hdmitx.c \
	$(LOCAL_DIR)/rpmb.c \
	$(LOCAL_DIR)/pwm.c \
	$(LOCAL_DIR)/boot_partition.c \
	$(LOCAL_DIR)/rtkirda.c \
	$(LOCAL_DIR)/rtkusb.c \
	$(LOCAL_DIR)/rtkusb-phy.c

MODULE_EXTRA_OBJS += \
          $(LOCAL_DIR)/libefuse.a
	
MEMBASE := 0x00000000
MEMSIZE ?= 0x40000000   # 1GB
KERNEL_LOAD_OFFSET := 0x30000 # 192K

ifeq ($(PLATFORM_RTD1355),1)
MODULE_DEPS += \
	lib/cbuf \
	lib/fdt \
	dev/interrupt/arm_gic \
	dev/timer/arm_generic \
	dev/md \
	platform/rtd1395/acpu_image \
	external/lib/factory
else
MODULE_DEPS += \
	lib/cbuf \
	lib/fdt \
	dev/interrupt/arm_gic \
	dev/timer/arm_generic \
	dev/md \
	dev/net/rtl8168 \
	platform/rtd1395/acpu_image \
	external/lib/factory \
	external/lib/rtk_storage_layout
endif

GLOBAL_DEFINES += \
	MEMBASE=$(MEMBASE) \
	MEMSIZE=$(MEMSIZE) \
	HEAP_START=$(HEAP_START) \
	HEAP_SIZE=$(HEAP_SIZE) \
	HAVE_MREMAP=0 \
	PLATFORM_SUPPORTS_PANIC_SHELL=1 \
	FLASH_TYPE=$(FLASH_TYPE) \
	LK_OFFSET=$(KERNEL_LOAD_OFFSET)

# GLOBAL_DEFINES += MMU_WITH_TRAMPOLINE=1 \

LINKER_SCRIPT += \
	$(BUILDDIR)/system-onesegment.ld

#PWM defines
GLOBAL_DEFINES += \
	PWM_0_PIN_0=$(PWM_0_PIN_0) \
	PWM_0_PIN_1=$(PWM_0_PIN_1) \
	PWM_1_PIN_0=$(PWM_1_PIN_0) \
	PWM_1_PIN_1=$(PWM_1_PIN_1) \
	PWM_2_PIN_0=$(PWM_2_PIN_0) \
	PWM_2_PIN_1=$(PWM_2_PIN_1) \
	PWM_3_PIN_0=$(PWM_3_PIN_0) \
	PWM_3_PIN_1=$(PWM_3_PIN_1)
	
# CRYPTO configuration
ifeq ($(HW_MCP),1)
MODULE_DEPS += dev/mcp
GLOBAL_DEFINES += HW_MCP=1
endif #HW_MCP

ifeq ($(VMX),1)
GLOBAL_DEFINES += VMX
endif #VMX
ifeq ($(USB_GPIO_SETTING),1)
# 1395 USB GPIO setting
GLOBAL_DEFINES += \
	USB_PORT0_GPIO_TYPE=\"$(USB_PORT0_GPIO_TYPE)\" \
	USB_PORT0_GPIO_NUM=$(USB_PORT0_GPIO_NUM) \
	USB_PORT0_TYPE_C_SWITCH_GPIO_TYPE=\"$(USB_PORT0_TYPE_C_SWITCH_GPIO_TYPE)\" \
	USB_PORT0_TYPE_C_SWITCH_GPIO_NUM=$(USB_PORT0_TYPE_C_SWITCH_GPIO_NUM) \
	USB_PORT1_GPIO_TYPE=\"$(USB_PORT1_GPIO_TYPE)\" \
	USB_PORT1_GPIO_NUM=$(USB_PORT1_GPIO_NUM) \
	USB_PORT2_GPIO_TYPE=\"$(USB_PORT2_GPIO_TYPE)\" \
	USB_PORT2_GPIO_NUM=$(USB_PORT2_GPIO_NUM)
endif

ifeq ($(PLATFORM_RTD1355),1)
GLOBAL_DEFINES += PLATFORM_RTD1355
GLOBAL_DEFINES += DEVICE_NO_ETHERNET
endif

include make/module.mk
