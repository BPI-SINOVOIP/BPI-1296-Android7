#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <debug.h>
#include <target/debugconfig.h>
#include <arch/arch_ops.h>
#include <lib/console.h>
#include <platform/io.h>
#include <platform/cpu.h>
#include <arch/ops.h>
#include <arch/arm64/mmu.h>
#include <platform.h>
#include <platform/timer.h>
#include <platform/gic.h>
#include <platform/interrupts.h>

extern void platform_init_secondary_mmu_mappings(void );
//static void pattern_1(void) __attribute__((optimize("-O0")));
static void pattern_2(void) __attribute__((optimize("-O0")));

static inline void wake_slave_cpu_with_addr(unsigned int release_addr)
{
	printf("release slave cpu at 0x%x\n", release_addr);
	rtd_outl(CPU_RELEASE_ADDR, release_addr);
	__asm__ volatile("sev");
}

unsigned long delay_time;
unsigned long cpu_ddr;
unsigned long cpu_wfi;
unsigned long cpu_l2;
int stability_ddr = 3;
int sleep_flag = 0;
int num_cpu_pass = 0;
#define udelay(_usec)      spin(_usec)
#define LOOPCOUNT	1000000000


static inline void __writel(unsigned int value, volatile unsigned int *addr){
	*addr = value;
}
static inline unsigned int __readl(volatile unsigned int *addr){
	return *addr;
}
#define U64_PTR(_v)   ((void *)(unsigned long)(_v))
#define cr_writel(_v, _a)  __writel(_v, U64_PTR(_a))
#define cr_readl(_a)       __readl(U64_PTR(_a))

#define ALL_CPU_MASK    0xf     // 4-cores, 0x1111
volatile int cpu_mask = 0;
lk_time_t last_run_start_time[4] = {0};
lk_time_t last_run_before_wfi[4] = {0};
unsigned int sync_round[4] = {0};

static enum handler_return timer_int(void *arg)
{
	int cpu = arch_curr_cpu_num();

	platform_stop_timer();
	printf("cpu%d timer-interrupt, stop timer\n", cpu);

	return INT_NO_RESCHEDULE;
}

static void setup_timer(void)
{
	int cpu = arch_curr_cpu_num();
	int i;

	printf("cpu%d enter %s\n", cpu, __func__);

	if (cpu == 0) {
		// disable all the other interrupt first
		for (i = 0 ; i < MAX_INT ; i++) {
			mask_interrupt(i);
		}
	} else {
		/* slave-cpu need to setup gic first if interrupt mode desired */
	}

	register_int_handler(ARM_GENERIC_TIMER_PHYSICAL_INT, &timer_int, NULL);
	unmask_interrupt(ARM_GENERIC_TIMER_PHYSICAL_INT);
}

/* delay use msec */
static void all_cpu_sync(lk_time_t delay, unsigned long addr, size_t length, int tlb_flush)
{
        int cpu = arch_curr_cpu_num();
        int cur_mask = 1 << cpu;
		lk_time_t sync_rescue_time = current_time() + 3000;

        if (cpu_mask & cur_mask) {
                printf("CPU%d : CPU-SYNC mask error:0x%x\n", cpu, cpu_mask);
                while (1) {};
        }

        printf("CPU%d reach %s round:%u sleep:%u start:%u\n",
		cpu, __func__, sync_round[cpu], last_run_before_wfi[cpu], last_run_start_time[cpu]);

	sync_round[cpu]++;

	/* Flush target cache range if set */
	if (length != 0) {
		printf("CPU%d flush/invalidate cache:0x%lx/0x%lx\n", cpu, addr, length);
		arch_clean_invalidate_cache_range(addr, length);
	}

        atomic_or(&cpu_mask, 1 << cpu);
        /* Now wait until all cores reach here and enter wfi */
        while (cpu_mask != ALL_CPU_MASK) 
		{
			if (current_time() >= sync_rescue_time)
				__asm__ volatile("sev");
		}
		
	platform_stop_timer();

	last_run_before_wfi[cpu] = current_time();
        // Core 0 is responsible of reset cpu-mask
	if (cpu == 0) {
                cpu_mask = 0;
	}

	if (tlb_flush)
		invalidate_tlb_all();

	// Each core need to setup its own timer
	platform_set_oneshot_timer(NULL, NULL, delay);

	asm volatile( "wfi" : : : "memory");

        last_run_start_time[cpu] = current_time();
}

static void pattern_1(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash = 3 * (unsigned long)LOOPCOUNT;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_1);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	printf("CPU%d run into pattern1\n", cpu);
	arch_disable_ints();

	while (1) {
		if (i == threash) {
			printf("CPU%d still running\n", cpu);
			i = 0;
		}
		i++;
	};

	panic("Why here?");
}

static void pattern_2(void)
{
	int cpu = arch_curr_cpu_num();
	//int a = 42, b = 2;
	unsigned long i=0;
	unsigned long threash = LOOPCOUNT;
	int life_universe_and_everything = 11;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_2);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	printf("CPU%d run into pattern2\n", cpu);
	arch_disable_ints();

/*
	while (1) {
		life_universe_and_everything = 42 * 2;
	}
*/
	while (1) {
		asm volatile(
			"mov	x1, #42\n\t"
			"mov	x2, #2\n\t"
			"mul	%0, x1, x2\n\t"
			: "=&r" (life_universe_and_everything)
			:
			: "x1", "x2"
		);
		if (i == threash) {
			printf("CPU%d still running\n", cpu);
			i = 0;
		}
		i++;
	}

	//dummy
	printf("%d\n", life_universe_and_everything);

	panic("Why here?");
}

// simply use the HEAP region since it won't go back
// to kernel anymore...
#define REGION_BASE			0x16000000
#define REGION_L2_BASE		0x20000000
#define REGION_SIZE_16K		0x00004000
#define REGION_SIZE			0x01000000
#define REGION_SRAM_BASE	0x80000000
#define REGION_SIZE_SRAM	0x00014000
static void pattern_3(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash = 3 * (unsigned long)LOOPCOUNT;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_3);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu == 0 || cpu == 1) {
		printf("CPU%d run into pattern3 - while-loop\n", cpu);

		while (1) {
			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		};
	} else if (cpu == 2 || cpu == 3) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern3 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		panic("invalid cpu number %d", cpu);
	}

	panic("Why here?");
}

static void pattern_4(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash;
	int target_cpu = 0;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_4);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu == target_cpu) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern4 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		while(1) {
			printf("CPU%d run-into wfe\n", cpu);
			asm volatile(
					"wfe\n\t"
					"wfe\n\t" : : : "memory");
			printf("CPU%d out wfe\n", cpu);
			panic("shoudn't leave\n");
		}
	}

	panic("Why here?");
}

static void pattern_5(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_5);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern5 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_6(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_6);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE_16K);
		unsigned long *iter = base;

		printf("CPU%d run into pattern6 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE_16K);
		//arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_16K);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE_16K) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				//arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_16K);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_7(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned int save_register;
	unsigned long i=0;
	unsigned long threash;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_7);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	
	/*Set CPU to SWC mode*/
	save_register = cr_readl(0x9800000c);
	cr_writel( save_register | ((0x1) << 19) , 0x9800000c);
	
	/*Set CPU to SWC mode*/
	save_register = cr_readl(0x98000054);
	cr_writel( save_register | ((0x1) << 3) , 0x98000054);

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_SRAM_BASE + (cpu * REGION_SIZE_SRAM/4);
		unsigned long *iter = base;

		printf("CPU%d run into pattern7 - memory-access-loop in SRAM whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE_SRAM/4);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_SRAM/4);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE_SRAM/4) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_SRAM/4);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_8(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash = 3 * (unsigned long)LOOPCOUNT;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_8);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu == 0 || cpu == 1) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern8 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		};
	} else if (cpu == 2 || cpu == 3) {
		
		printf("CPU%d run into pattern8 - max poower without ldr instruction\n", cpu);
		
	    asm volatile("msr     daifset, #1" : : : "memory");
		asm volatile("mrs     x0, mdscr_el1" : : : "memory");
		asm volatile("orr     x0, x0, #(0x1 << 14)" : : : "memory");
		asm volatile("msr     mdscr_el1, x0" : : : "memory");
		asm volatile("mov     x0, #0" : : : "memory");
		asm volatile("msr     oslar_el1, x0" : : : "memory");
		asm volatile("isb" : : : "memory");

		asm volatile(
		"mov     x0, #0x21E7 \n\t"
		"msr     dbgbcr0_el1, x0 \n\t"
		"msr     dbgbcr1_el1, x0 \n\t"
		"msr     dbgbcr2_el1, x0 \n\t"
		"msr     dbgbcr3_el1, x0 \n\t"
		"msr     dbgbcr4_el1, x0 \n\t"
		"msr     dbgbcr5_el1, x0 \n\t"
		:
		:
		: "memory"
		);
		
		asm volatile(
		"mov     x0, #0x3FE7 \n\t"
		"msr     dbgwcr0_el1, x0 \n\t"
		"msr     dbgwcr1_el1, x0 \n\t"
		"msr     dbgwcr2_el1, x0 \n\t"
		"msr     dbgwcr3_el1, x0 \n\t"
		:
		:
		: "memory"
		);				
		
		
		asm volatile(
		"ldr     x0, =0xFFFFFFFF55555554 \n\t"
		"ldr     x1, =0x0000000055555554 \n\t"
		"ldr     x2, =0xFFFFFFFDAAAAAAA8 \n\t"
		"ldr     x3, =0x00000000AAAAAAA8 \n\t"
		"ldr     x4, =0xFFFFFFFF55555554 \n\t"
		"ldr     x5, =0x0000000055555554 \n\t"
		"ldr     x6, =0xFFFFFFFDAAAAAAA8 \n\t"
		"ldr     x7, =0x00000000AAAAAAA8 \n\t"
		:
		:
		: "memory"
		);

		asm volatile(
		"movi v1.16b, #0x66 \n\t"
		"movi v2.16b, #0xAA \n\t"
		"movi v3.16b, #0x66 \n\t"
		"movi v4.16b, #0xAA \n\t"
		"movi v5.16b, #0x66 \n\t"
		"movi v6.16b, #0xAA \n\t"
		"movi v7.16b, #0x66 \n\t"
		"movi v8.16b, #0xAA \n\t"
		"movi v9.16b, #0x66 \n\t"
		:
		:
		: "memory"
		);	

		asm volatile(
		"ldr     x9,  =0xFFFFFFFD55555554 \n\t"
		"ldr     x10, =0x0000000155555554 \n\t"
		:
		:
		: "memory"
		);
		
		asm volatile("power_loop:");
		
		asm volatile(
		"fmla      v1.4s, v2.4s, v3.4s \n\t"
		//"ldr       w8, [x0, x1, lsl #2] \n\t"
		//"ldr       w8, [x2, x3, lsl #2] \n\t"
		"add       x0, x1, x9 \n\t"
		"sub       x2, x3, x10 \n\t"
		//"ldr       w8, [x4, x5, lsl #2] \n\t"
		//"ldr       w8, [x6, x7, lsl #2] \n\t"
		"add       x4, x9, x5 \n\t"
		:
		:
		: "memory"
		);
		
		asm volatile(
		"fmla      v4.4s, v5.4s, v6.4s \n\t"
		//"ldr       w8, [x0, x3, lsl #2] \n\t"
		//"ldr       w8, [x2, x1, lsl #2] \n\t"
		"sub       x0, x3, x10 \n\t"
		"add       x2, x1, x9 \n\t"
		//"ldr       w8, [x4, x7, lsl #2] \n\t"
		//"ldr       w8, [x6, x7, lsl #2] \n\t"
		"sub       x4, x7, x10 \n\t"
		:
		:
		: "memory"
		);
		
		asm volatile(
		"fmla      v7.4s, v8.4s, v9.4s \n\t"
		//"ldr       w8, [x0, x1, lsl #2] \n\t"
		//"ldr       w8, [x2, x3, lsl #2] \n\t"
		"add       x0, x1, x9 \n\t"
		"sub       x2, x3, x10 \n\t"
		//"ldr       w8, [x4, x5, lsl #2] \n\t"
		//"ldr       w8, [x6, x7, lsl #2] \n\t"
		"add       x4, x9, x5 \n\t"
		:
		:
		: "memory"
		);
		
		asm volatile(
		"fmla      v4.4s, v5.4s, v6.4s \n\t"
		//"ldr       w8, [x0, x3, lsl #2] \n\t"
		//"ldr       w8, [x2, x1, lsl #2] \n\t"
		"sub       x0, x3, x10 \n\t"
		"add       x2, x1, x9 \n\t"
		//"ldr       w8, [x4, x7, lsl #2] \n\t"
		//"ldr       w8, [x6, x7, lsl #2] \n\t"
		"sub       x4, x7, x10 \n\t"
		:
		:
		: "memory"
		);
		
		asm volatile(
		//"subs    x11, x11, #1 \n\t"
		//"add     x11, x11, #1 \n\t"
		"b     power_loop \n\t"
		:
		:
		: "memory"
		);
		
	} else {
		panic("invalid cpu number %d", cpu);
	}

	panic("Why here?");
}

#define SYNC_TIME	500	// perform all-cpu-sync every 500 ms
static void pattern_9(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_9);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	setup_timer();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;
		lk_time_t sync_time = current_time() + SYNC_TIME;

		printf("CPU%d run into pattern9 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (current_time() >= sync_time) {
				all_cpu_sync(100, (unsigned long)base, REGION_SIZE, 1);
				sync_time = current_time() + SYNC_TIME;
			}
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_10(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned int save_register;
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_10);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	
	/*Set CPU to SWC mode*/
	save_register = cr_readl(0x9800000c);
	cr_writel( save_register | ((0x1) << 19) , 0x9800000c);
	
	/*Set CPU to SWC mode*/
	save_register = cr_readl(0x98000054);
	cr_writel( save_register | ((0x1) << 3) , 0x98000054);

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_SRAM_BASE + (cpu * REGION_SIZE_SRAM/4);
		unsigned long *iter = base;

		printf("CPU%d run into pattern10 - memory-access-loop and clean tlb in SRAM whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE_SRAM/4);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_SRAM/4);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE_SRAM/4) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_SRAM/4);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
			
			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_11(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_11);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	setup_timer();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;
		lk_time_t sync_time = current_time() + SYNC_TIME;

		printf("CPU%d run into pattern11 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (current_time() >= sync_time) {
				all_cpu_sync(100, (unsigned long)base, REGION_SIZE, 0);
				sync_time = current_time() + SYNC_TIME;
			}
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_12(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long * time = &delay_time;
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_12);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern12 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		printf("CPU%d delay time is %d us\n", cpu, (int)*time);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 10000;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}
			
			udelay((int)*time);
			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_13(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned int save_register;
	unsigned long * cpu_ddr_num = &cpu_ddr;
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_13);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4 && (cpu != (int)*cpu_ddr_num)) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern13 - memory-access-loop and clean tlb in DDR whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else if(cpu == (int)*cpu_ddr_num){
		/*Set CPU to SWC mode*/
		save_register = cr_readl(0x9800000c);
		cr_writel( save_register | ((0x1) << 19) , 0x9800000c);
		
		/*Set CPU to SWC mode*/
		save_register = cr_readl(0x98000054);
		cr_writel( save_register | ((0x1) << 3) , 0x98000054);
		
		void *base = (void*)REGION_SRAM_BASE + (cpu * REGION_SIZE_SRAM/4);
		unsigned long *iter = base;

		printf("CPU%d run into pattern13 - memory-access-loop and clean tlb in SRAM whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE_SRAM/4);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_SRAM/4);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE_SRAM/4) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_SRAM/4);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
			
			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
		
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_14(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_14);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 3) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern14 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else if (cpu == 3) {
		printf("CPU%d run into pattern14 - while 1 loop\n", cpu);
		while (1) {
			threash = 3 * (unsigned long)LOOPCOUNT;
			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_15(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0, j=0;
	unsigned long * cpu_wfi_num = &cpu_wfi;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_15);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	
	if (cpu == (int)*cpu_wfi_num)
	{
		platform_stop_timer();
		printf("cpu%d timer-interrupt, stop timer\n", cpu);
		printf("CPU%d run into pattern15 - into wfi\n", cpu);
		asm volatile( "wfi" : : : "memory");
		return;
	}

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern15 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_16(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0, j=0;
	unsigned long * cpu_l2_num = &cpu_l2;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_16);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4 && (cpu != (int)*cpu_l2_num)) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern16 - memory-access-loop whith base 0x%lx\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else if (cpu == (int)*cpu_l2_num) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern16 - memory-access-loop whith base 0x%lx with no clean cache\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE_16K);
		//arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_16K);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE_16K) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				//arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_16K);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_17(void)
{
	int cpu = arch_curr_cpu_num();
	unsigned long i=0;
	unsigned long threash;
	unsigned long dummy_value;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_17);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE_16K);
		unsigned long *iter = base;

		printf("CPU%d run into pattern17 - memory-access-loop whith base 0x%lx with no clean cache\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE_16K);
		//arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_16K);
		threash = LOOPCOUNT / 2;

		while (1) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE_16K) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);				
				dummy_value = cr_readl(0x98017418);
				dummy_value = dummy_value + cr_readl(iter);
				iter = base;
				//arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE_16K);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_18(void)
{
	int cpu = arch_curr_cpu_num();
	int max_range = 50, k=0, a=0, num_prime=0;
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_18);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern_18 - memory-access-loop whith base 0x%lx and find prime number\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (1) {
			
			num_prime = 1;
			for (a=3; a < max_range; a++)
			{
				for (k=2; k < a; k++)
				{
					if(k*k > a)
					{
						num_prime++;
						break;
					}
					else
					{
						if(a%k == 0)
							break;							
					}
				}
			}
			
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_19(void)
{
	int cpu = arch_curr_cpu_num();
	long max_range = 20000000000, k=0, a=0, num_prime=0;
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_19);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	setup_timer();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern_19 - memory-access-loop whith base 0x%lx and find prime number\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;
		
		udelay(cpu*1000*1000*4);

		while (1) {
			
			if (cpu != stability_ddr)
			{
				num_prime = 1;
				printf("CPU%d finding prime number\n", cpu);
				for (a=3; a < max_range; a++)
				{
					for (k=2; k < a; k++)
					{
						if(k*k > a)
						{
							num_prime++;
							break;
						}
						else
						{
							if(a%k == 0)
								break;							
						}
					}
				}
				stability_ddr = cpu;
				printf("CPU%d finish find prime number\n", cpu);
				platform_stop_timer();
				platform_set_oneshot_timer(NULL, NULL, 100);
				asm volatile( "wfi" : : : "memory");
			}
			else{
				if (*iter == 0xaaaaaaaaaaaaaaaa) {
					*iter = 0x5555555555555555;
				} else if (*iter == 0x5555555555555555) {
					*iter = 0xaaaaaaaaaaaaaaaa;
				} else {
					printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
				}

				iter++;
				if (iter == base + REGION_SIZE) {
					//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
					//	cpu, (unsigned long)iter, (unsigned long)base);
					iter = base;
					arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
				}

				if (i == threash) {
					printf("CPU%d finish access ddr\n", cpu);
					i = 0;
					//stability_ddr = (cpu + 1) % 4;
					//platform_stop_timer();
					//platform_set_oneshot_timer(NULL, NULL, 100);
					//asm volatile( "wfi" : : : "memory");
				}
				i++;

				if (j == threash_2) {
					invalidate_tlb_all();
					j=0;
				}
				j++;
			}
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

static void pattern_20(void)
{
	int cpu = arch_curr_cpu_num();
	long max_range = 20000000000, k=0, a=0, num_prime=0;
	unsigned long i=0;
	unsigned long threash;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_20);
	} else {
		platform_init_secondary_mmu_mappings();
	}

	arch_disable_ints();
	setup_timer();

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern_20 - memory-access-loop whith base 0x%lx and find prime number\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		
		udelay(cpu*1000*1000*4);

		while (1) {
			
			if (cpu == 0)
			{
				sleep_flag = 0;
				num_prime = 1;
				printf("CPU%d finding prime number\n", cpu);
				for (a=3; a < max_range; a++)
				{
					for (k=2; k < a; k++)
					{
						if(k*k > a)
						{
							num_prime++;
							break;
						}
						else
						{
							if(a%k == 0)
								break;							
						}
					}
				}
				sleep_flag = 1;
				all_cpu_sync(100, (unsigned long)base, REGION_SIZE, 0);				
			}
			else{
				if (*iter == 0xaaaaaaaaaaaaaaaa) {
					*iter = 0x5555555555555555;
				} else if (*iter == 0x5555555555555555) {
					*iter = 0xaaaaaaaaaaaaaaaa;
				} else {
					printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
				}

				iter++;
				if (iter == base + REGION_SIZE) {
					//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
					//	cpu, (unsigned long)iter, (unsigned long)base);
					iter = base;
					arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
				}

				if (i == threash) {
					printf("CPU%d finish access ddr\n", cpu);
					i = 0;
				}
				i++;
				
				if (sleep_flag)
					all_cpu_sync(100, (unsigned long)base, REGION_SIZE, 0);
			}
		}
	} else {
		panic("Error CPU%d", cpu);
	}

	panic("Why here?");
}

void pattern_21(void)
{
	int cpu = arch_curr_cpu_num();
	int cur_mask = 1 << cpu;
	int *total_pass = &num_cpu_pass;
	int fail_flag = 0;
	unsigned long i=0, j=0;
	unsigned long threash;
	unsigned long threash_2;

	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)&pattern_21);
	} else {
		//platform_init_secondary_mmu_mappings();
	}
	
	if (cpu_mask & cur_mask) {
        printf("CPU%d : CPU-SYNC mask error:0x%x\n", cpu, cpu_mask);
        while (1) {};
	}
	
	/*
		This pattern is for ft_test.
		Just check every cpu is normal.
		Before running this pattern, the dcache should be opened.
	 */

	arch_disable_ints();
	
	udelay(cpu*10000*5);

	if (cpu >= 0 && cpu < 4) {
		void *base = (void*)REGION_BASE + (cpu * REGION_SIZE);
		unsigned long *iter = base;

		printf("CPU%d run into pattern_21 - memory-access-loop whith base 0x%lx\n\n", cpu, (unsigned long)base);
		memset(base, 0xaa, REGION_SIZE);
		arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
		threash = LOOPCOUNT / 2;
		threash_2 = LOOPCOUNT / 100;

		while (i < 0x10000) {
			if (*iter == 0xaaaaaaaaaaaaaaaa) {
				*iter = 0x5555555555555555;
			} else if (*iter == 0x5555555555555555) {
				*iter = 0xaaaaaaaaaaaaaaaa;
			} else {
				fail_flag++;
				printf("CPU%d pattern error 0x%lx at addr 0x%lx\n", cpu, *iter, (unsigned long)iter);
			}

			iter++;
			if (iter == base + REGION_SIZE) {
				//printf("CPU%d touch bottom 0x%lx, roll back to 0x%lx\n",
				//	cpu, (unsigned long)iter, (unsigned long)base);
				iter = base;
				arch_clean_invalidate_cache_range((unsigned long)base, REGION_SIZE);
			}

			if (i == threash) {
				printf("CPU%d still running\n", cpu);
				i = 0;
			}
			i++;

			if (j == threash_2) {
				invalidate_tlb_all();
				j=0;
			}
			j++;
		}
	} else {
		panic("Error CPU%d", cpu);
	}
	
	printf("CPU%d run finished\n", cpu);
	
	if (cpu == 0) {
		wake_slave_cpu_with_addr((unsigned)(uint64_t)LK_OFFSET);
	}
	
	if(!fail_flag)
		(*total_pass)++;
	
	atomic_or(&cpu_mask, 1 << cpu);
    /* Now wait until all cores reach here and enter wfi */
    while (cpu_mask != ALL_CPU_MASK){}
	
	if (cpu == 0) {
		cpu_mask = 0;
	}
	
	if((*total_pass) == 4 && cpu == 0)
	{
		printf("All cpu passed!\n");
	}
	
}

static void print_usage(void)
{
	printf("usage:\n");
	printf("\tcputest <PATTERN> : let each core run into certain test-pattern\n");
	printf("\t\t 1  : let 4-core run in while-1 loop\n");
	printf("\t\t 2  : let 4-core run in while-1 loop doing multiply\n");
	printf("\t\t 3  : let 2-core run in while-1 loop, other 2 doing memory access\n");
	printf("\t\t 4  : let 1-core doing memory access, other 3 core enter wfe\n");
	printf("\t\t 5  : let 4-core doing memory access with clean tlb\n");
	printf("\t\t 6  : let 4-core doing memory access and no flush\n");
	printf("\t\t 7  : let 4-core doing memory access in SRAM\n");
	printf("\t\t 8  : let 2-core run in max power without ldr instruction, other 2 doing memory access\n");
	printf("\t\t 9  : let 4-core doing memory access with cache/tlb flush and delay(wfi)\n");
	printf("\t\t 10 : let 4-core doing memory access in SRAM and clean tlb\n");
	printf("\t\t 11 : let 4-core doing memory access with cache flush and delay(wfi)\n");
	printf("\t\t 12 : let 4-core doing slow down memory access with clean tlb\n");
	printf("\t\t 13 : let 3-core doing memory access with clean tlb and choose 1 core access sram\n");
	printf("\t\t 14 : let 3-core doing memory access with clean tlb and core 3 while 1 loop\n");
	printf("\t\t 15 : let 3-core doing memory access with clean tlb and choose 1 core wfi\n");
	printf("\t\t 16 : let 3-core doing memory access with clean tlb and choose 1 core access l2\n");
	printf("\t\t 17 : let 4-core doing memory access and no flush with reading dummy register\n");
	printf("\t\t 18 : let 4-core doing memory access with clean tlb and find prime number\n");
	printf("\t\t 19 : let 4-core simulate the stability test\n");
	printf("\t\t 20 : let 3-core memory access with cache flush and core-0 find prime number\n");
	printf("\t\t 21 : let 4-core doing memory access with clean tlb for checking cpu alive\n");
}

int cputest_cmd(int argc, const cmd_args *argv)
{
	if (argc <= 1) {
		print_usage();
		return -1;
	}

	if (strcmp(argv[1].str, "1") == 0) {
		pattern_1();
	} else if (strcmp(argv[1].str, "2") == 0){
		pattern_2();
	} else if (strcmp(argv[1].str, "3") == 0){
		pattern_3();
	} else if (strcmp(argv[1].str, "4") == 0){
		pattern_4();
	} else if (strcmp(argv[1].str, "5") == 0){
		pattern_5();
	} else if (strcmp(argv[1].str, "6") == 0){
		pattern_6();
	} else if (strcmp(argv[1].str, "7") == 0){
		pattern_7();
	} else if (strcmp(argv[1].str, "8") == 0){
		pattern_8();
	} else if (strcmp(argv[1].str, "9") == 0){
		pattern_9();
	} else if (strcmp(argv[1].str, "10") == 0){
		pattern_10();
	} else if (strcmp(argv[1].str, "11") == 0){
		pattern_11();
	} else if (strcmp(argv[1].str, "12") == 0){
		delay_time = argv[2].u;
		pattern_12();
	} else if (strcmp(argv[1].str, "13") == 0){
		cpu_ddr = argv[2].u;
		pattern_13();
	} else if (strcmp(argv[1].str, "14") == 0){
		pattern_14();
	} else if (strcmp(argv[1].str, "15") == 0){
		cpu_wfi = argv[2].u;
		pattern_15();
	} else if (strcmp(argv[1].str, "16") == 0){
		cpu_l2 = argv[2].u;
		pattern_16();
	} else if (strcmp(argv[1].str, "17") == 0){
		pattern_17();
	} else if (strcmp(argv[1].str, "18") == 0){
		pattern_18();
	} else if (strcmp(argv[1].str, "19") == 0){
		pattern_19();
	} else if (strcmp(argv[1].str, "20") == 0){
		pattern_20();
	} else if (strcmp(argv[1].str, "21") == 0){
		pattern_21();
	} else {
		printf("invalid pattern:%s!!\n", argv[1].str);
		print_usage();
		return -1;
	}

	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("cputest", "block device GPT command", (console_cmd)&cputest_cmd)
STATIC_COMMAND_END(cputest_cmd);