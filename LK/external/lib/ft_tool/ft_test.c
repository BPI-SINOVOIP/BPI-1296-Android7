#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arch.h>
#include <arch/ops.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include <platform/io.h>
#include <platform/sb2.h>
#include <platform/rbus/mis_reg.h>
#include <platform/rbus/iso_reg.h>
#include <platform/rbus/crt_sys_reg.h>
#include <platform/rbus/emmc_reg.h>

#if defined(WITH_LIB_CONSOLE)
#include <lib/console.h>

#define MAX_NAME_LEN 128
#endif

#define FT_DEBUG 1

#if FT_DEBUG
#define DBG(format, ...) printf(format , ## __VA_ARGS__)
#else
#define DBG(format, ...)
#endif

extern void scpu_test_entry1(void);
extern void mem_test_entry1(void);
extern void mem_test_entry2(void);
extern void mem_test_entry3(void);
extern void GPHY_test_entry(void);
extern void PCIE1_test_entry(void);
extern void PCIE2_test_entry(void);
extern void platform_init_secondary_mmu_mappings(void);
extern void pattern_21(void);

void ft_init_gpio(void)
{
#if PLATFORM_RTD1295
	//GPIO20
	//0x9801_B100, bit 20 = 1
	//0x9801_B110, bit 20 = 1
	rtd_setbits(MIS_GP0DIR, BIT20);
	rtd_setbits(MIS_GP0DATO, BIT20);
	DBG("1295 GPIO20 high\n");

	//GPIO82
	//EMMC_MUXPAD1 0x9801_2604, bit[19..18] = 00, Mux to gpio[82]
	//GP2DIR 0x9801_B108, bit 18 = 1
	//GP2DATO 0x9801_B118, bit 18 = 0
	rtd_clearbits(EMMC_MUXPAD1, BIT18 | BIT19);
	rtd_setbits(MIS_GP2DIR, BIT18);
	rtd_clearbits(MIS_GP2DATO, BIT18);
	DBG("1295 GPIO82 low\n");
#endif

#if PLATFORM_RTD1395
	// Booting Check
	rtd_clearbits(ISO_GPDATO_1, BIT2);
	rtd_setbits(ISO_GPDIR_1, BIT2);
	printf("1395 GPIO34 low\n");

	// DDR Pattern Check
	rtd_setbits(ISO_GPDATO, BIT30);
	rtd_setbits(ISO_GPDIR, BIT30);
	printf("1395 GPIO30 high\n");

	// SCPU Pattern Check
	rtd_setbits(ISO_GPDATO_1, BIT3);
	rtd_setbits(ISO_GPDIR_1, BIT3);
	printf("1395 GPIO35 high\n");

#endif
	#if 0
	//GPIO16
	//GP0DIR 0x9801_B100, bit 16 = 1
	//GP0DATO 0x9801_B110, bit 16 = 0
	rtd_setbits(MIS_GP0DIR, BIT16);
	rtd_clearbits(MIS_GP0DATO, BIT16);
	DBG("1295 GPIO16 low\n");

	//GPIO19
	//GP0DIR 0x9801_B100, bit 19 = 1
	//GP0DATO 0x9801_B110, bit 19 = 0
	rtd_setbits(MIS_GP0DIR, BIT19);
	rtd_clearbits(MIS_GP0DATO, BIT19);
	DBG("1295 GPIO19 low\n");
	#endif
}

void DDR_test_pattern_0(void)
{

#if 1
	//mem_test_entry();
#if PLATFORM_RTD1295
	if (get_chip_id() != RTK1296_CPU_ID)
		mem_test_entry2();
	else
		mem_test_entry3();
#endif

#if PLATFORM_RTD1395
	if (get_chip_id() == RTK1355_CPU_ID)
		mem_test_entry1();
	else
		mem_test_entry2();
#endif

#else
  rtd_maskl(CRT_MUXPAD1, ~(BIT19|BIT18),0);
  rtd_setbits(GP0DIR,BIT20);
  rtd_setbits(GP0DATO,BIT20);
  printf("GPIO20 high\n");
#endif
}


#ifdef CONFIG_FT_RESCUE
void call_go_rescue_linux(void)
{
	setenv("bootcmd", "bootr");
	run_command_list("go r", -1, 0);
}
#endif //CONFIG_FT_RESCUE

void ft_test_0(void)
{
#if PLATFORM_RTD1295
	/* disable MMU, DCACHE, INTERRUPTS */
	arch_disable_ints();
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();

	/* call real test function */
	//dcache_disable();

	printf("\nGo DDR test pattern\n");
	DDR_test_pattern_0();

	printf("\nDo GPHY ADC bias calibration\n");
	GPHY_test_entry();

#if 0
	printf("\nTest PCIE Gen1 Ethernet loopback\n");
	PCIE1_test_entry();

	printf("\nTest PCIE Gen2 Ethernet loopback\n");
	PCIE2_test_entry();
#endif

#endif

#if PLATFORM_RTD1395
	
	/* disable MMU, DCACHE, INTERRUPTS */
	/*arch_disable_ints();
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();*/

	printf("\n**********Go DDR test pattern**********\n");
	DDR_test_pattern_0();

	printf("\n**********Go CPU test pattern**********\n");
	scpu_test_entry1();

	printf("\nExit Final Test...\n");
#endif

	/* enable MMU, DCACHE, INTERRUPTS */
	platform_init_secondary_mmu_mappings();
	pattern_21();
	arch_enable_ints();

#ifdef CONFIG_FT_RESCUE
	//call_go_rescue_linux();
#endif //CONFIG_FT_RESCUE
}

#if defined(WITH_LIB_CONSOLE)
static int ft_cmd(int argc, const cmd_args *argv)
{
	ft_init_gpio();
	ft_test_0();

	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("ft_tool", "FT tool", &ft_cmd)
STATIC_COMMAND_END(ft_tool);
#endif
