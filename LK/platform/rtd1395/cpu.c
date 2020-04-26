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
#include <lk/init.h>
#include <platform.h>
#include <platform/rtd1395.h>
#include <platform/cpu.h>
#include <platform/io.h>
#include <platform/sb2.h>
#include <arch/ops.h>
#include <arch/arm64/mmu.h>

#define LOCAL_TRACE 0

void bootup_slave_cpu(void)
{
	printf("Bring UP slave CPUs\n");

	 // core 123 jump to FSBL directly
	rtd_outl(0x9801D538, 0x00000FF0);
	asm volatile ("ISB SY" : : : "memory");
	sync();

	rtd_outl(0x9801D510, 0x00000000);
	asm volatile ("ISB SY" : : : "memory");
	sync();

	rtd_outl(0x9801D100, 0x8F1F3BFF);
	asm volatile ("ISB SY" : : : "memory");
	sync();

	//rtd_outl(0x9801D100, 0x0F1F3BFF);
	//asm volatile ("ISB SY" : : : "memory");
	//sync();
	
	return;
}

#ifdef SLAVE_CPU_SPIN_TABLE
#define RELEASE_ADDR_VALID(x)	((x != 0x0) && (x != LK_OFFSET))

static void slave_cpu_prepare_boot(void)
{
	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;

	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();

	arch_disable_cache(ICACHE);
	arch_invalidate_icache_all();

	if (current_el == 3)
		arm64_el3_to_el2();
}

void slave_cpu_spin(uint level)
{
	unsigned int release_addr = 0;
	void (*kernel_secondary_entry)(void);

	slave_cpu_prepare_boot();

	while(1) {
		asm volatile("wfe" : : : "memory");
		release_addr = rtd_inl(CPU_RELEASE_ADDR);
		if (RELEASE_ADDR_VALID(release_addr)) {
			kernel_secondary_entry = (void(*)(void))(u64)release_addr;
			kernel_secondary_entry();
		}
	}
}

LK_INIT_HOOK_FLAGS(slave_cpu_spin, slave_cpu_spin,
		LK_INIT_LEVEL_HEAP - 1, LK_INIT_FLAG_SECONDARY_CPUS);
#endif // SLAVE_CPU_SPIN_TABLE
