/*
 * Copyright (c) 2012-2015 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <arch.h>
#include <err.h>
#include <debug.h>
#include <trace.h>
#include <dev/interrupt/arm_gic.h>
#include <dev/timer/arm_generic.h>
#include <dev/uart.h>
#include <lk/init.h>
#include <kernel/vm.h>
#include <kernel/spinlock.h>
#include <platform.h>
#include <platform/gic.h>
#include <platform/interrupts.h>
#include <platform/rtd1295.h>
#include <platform/cpu.h>
#include <platform/io.h>
#include <platform/emmc.h>
#include <platform/spi.h>
#include <platform/rbus/iso_reg.h>
#include <platform/setting.h>
#include <platform/pwm.h>
#include <libfdt.h>
#include <factorylib.h>
#include <lib/console.h>
#include <lib/sysparam.h>
#include "platform_p.h"
#include "rtknand.h"
#include "power.h"
#if WITH_LIB_MINIP
#include <lib/minip.h>
#endif
#include <stdlib.h>
#ifdef MAX_CPUFREQ
void cpu_set_max_capacity(void);
#else
static inline void cpu_set_max_capacity(void) {}
#endif


void platform_early_init(void)
{
	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
	uart_init_early();
	/* initialize the interrupt controller */
	if (current_el == 3) // If LK starts from EL2, gic init is done by BL31 instead
		arm_gic_init();

	arm_generic_timer_init(ARM_GENERIC_TIMER_PHYSICAL_INT, 0);
}

void platform_halt(platform_halt_action suggested_action,
			platform_halt_reason reason)
{
#if ENABLE_PANIC_SHELL
	if (reason == HALT_REASON_SW_PANIC) {
		dprintf(ALWAYS, "CRASH: starting debug shell... (reason = %d)\n", reason);
		arch_disable_ints();
		panic_shell_start();
	}
#endif // ENABLE_PANIC_SHELL

	if (suggested_action == HALT_ACTION_SHUTDOWN)
		printf("HALT Not supported, switch to REBOOT instead...\n");

	printf("REBOOTING...");
	spin(50000);	// delay for 50 ms

	// trigger watchdog reset
	rtd_outl(ISO_TCWCR_reg,0xa5);
	rtd_outl(ISO_TCWTR_reg,0x1);
	rtd_outl(ISO_TCWOV_reg,1);
	rtd_outl(ISO_TCWCR_reg,0);

	while(1) {};
}

#define ADDR_BUF_LEN    32
void sysparam_default(void)
{
	char buff[ADDR_BUF_LEN] = "";

	/* Setup Default Parameter */
	snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)KERNEL_LOADADDR);
	sysparam_add("kernel_loadaddr", buff, strlen(buff));

	snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)FDT_LOADADDR);
	sysparam_add("fdt_loadaddr", buff, strlen(buff));

	snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)ROOTFS_LOADADDR);
	sysparam_add("rootfs_loadaddr", buff, strlen(buff));

	snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)AUDIO_LOADADDR);
	sysparam_add("audio_loadaddr", buff, strlen(buff));

	srand(current_time_hires());
	snprintf(buff, ADDR_BUF_LEN, "00:10:20:30:%02x:%02x", (unsigned int)rand() % 256, (unsigned int)rand() % 256);
	sysparam_add("ethaddr", buff, strlen(buff));

	snprintf(buff, ADDR_BUF_LEN, "%s", "192.168.100.254");
	sysparam_add("gatewayip", buff, strlen(buff));

	snprintf(buff, ADDR_BUF_LEN, "%s", "192.168.100.1");
	sysparam_add("ipaddr", buff, strlen(buff));

	snprintf(buff, ADDR_BUF_LEN, "%s", "255.255.255.0");
	sysparam_add("netmask", buff, strlen(buff));
}

#ifdef FT_TOOL
extern void ft_init_gpio(void);
#endif //FT_TOOL

void disable_soc_full_reset(void)
{
	rtd_outl(ISO_TCWOV_RSTB_PAD, 0);
	printf("Disable Watchdog OE\n");
}

void platform_init(void)
{
	BOOT_FLASH_T boot_flash_type = FLASH_TYPE;

	cpu_set_max_capacity();
	uart_init();
	#ifdef FT_TOOL
	ft_init_gpio();
	#endif
	rtd129x_pwm_init();

	if (boot_flash_type == BOOT_EMMC) {
		if (rtk_emmc_init())
			panic("*** EMMC init FAIL ***");
	} else if (boot_flash_type == BOOT_NAND) {
		if (nf_initial())
			panic("*** NAND init FAIL ***");		   
		printf("%s, NAND  supported TEST \n", __func__);
	} else if (boot_flash_type == BOOT_NOR_SERIAL) {
		if (rtk_spi_init())
			panic("*** SPI init FAIL ***");
	} else {
		panic("No FLASH_TYPE Defined!!\n");
	}
	/* Factory init */
	factory_init();
	/* setup sysparam from factory if any */
	if (sysparam_scan_factory() != NO_ERROR)
		sysparam_default();

	bootup_slave_cpu();
}
