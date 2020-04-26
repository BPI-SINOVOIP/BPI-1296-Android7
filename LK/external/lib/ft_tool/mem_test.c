#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rand.h>
#include <arch.h>
#include <arch/ops.h>
#include <arch/arch_ops.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include <platform/io.h>
#include <platform/rbus/mis_reg.h>
#include <platform/rbus/iso_reg.h>
#include <debug.h>
#include "mem_test.h"

#define MEM_DEBUG 1
#define MT_ENABLE 0

#define udelay(_usec)      spin(_usec)

#if MEM_DEBUG
#define DBG(format, ...) printf(format , ## __VA_ARGS__)
#else
#define DBG(format, ...)
#endif

/* Set the data bus width to 32 bits. */
typedef volatile unsigned int datum;

/* Set the data bus width to 64 bits. */
typedef volatile unsigned long long datum64;

extern void platform_init_secondary_mmu_mappings(void);

void _sync(void)
{
	// Rbus/Dbus sync
	dmb();
	Rreg32(0x9801a020) = 0x0;
	dmb();
}

void dcsys_memory_trash_init(datum *sAddr, datum *eAddr, datum region)
{
	// Memory Trash
	Wreg32(DC_MT_SADDR(region), ((datum64)sAddr >> 3) | (0x1 << 30));
	Wreg32(DC_MT_EADDR(region), ((datum64)eAddr >> 3));
	Wreg32(DC_MT_TABLE(region), 0x00ffffff);
	Wreg32(DC_MT_MODE(region), 0x00aa8005);
}

int dc_scramble_enable(datum *sAddr, datum *eAddr, datum region, datum mode)
{
	datum pattern = *sAddr;

	// Scramble Range (4KB aligned)
	Rreg32(DC_mem_scramble_saddr(region)) = ((datum64)sAddr >> 2);
	Rreg32(DC_mem_scramble_eaddr(region)) = ((datum64)eAddr >> 2);
	Rreg32(DC_mem_scramble_ctrl) |= (mode << (16 + region));
	Rreg32(DC_mem_scramble_ctrl) |= (0x1 << region);

	_sync();
	
	DBG("Enable DC scramble region %d (%p ~ %p): ", region, sAddr, eAddr);
	if (pattern != *sAddr) {
		DBG("OK\n");
		return 0;
	}
	else {
		DBG("Fail\n");
		return 1;
	}
}

int scpu_wrapper_mode_test(datum *sAddr, datum *eAddr, datum region, datum offset, datum step, datum loop)
{
	uint32_t i = 0;
	t_md_descriptor *md_dscpt = (t_md_descriptor *)MD_DES_ADDR;
	
	/* Disable MD go bit */
	Wreg32_M(REG_CRT, 0xFBFFFFFF, 0x0);
	Wreg32_M(REG_CRT, 0xFBFFFFFF, 0x4000000);

	md_dscpt->mode = 0x45;
	md_dscpt->Daddr = (datum64)sAddr;
	md_dscpt->Saddr = 0x5a;
	md_dscpt->Length = (datum64)eAddr - (datum64)sAddr;

	/* Set MD ring buffer */
	Wreg32(MD_SMQBase, (uintptr_t)md_dscpt);
	Wreg32(MD_SMQLimit, (uintptr_t)md_dscpt + sizeof(t_md_descriptor) * 2);
	Wreg32(MD_SMQRdptr, (uintptr_t)md_dscpt);
	Wreg32(MD_SMQWrptr, (uintptr_t)md_dscpt + sizeof(t_md_descriptor));

	/* Enable MD go bit */
	Wreg32(MD_SMQ_CNTL, 0x7);

	while (Rreg32(MD_SMQ_CNTL) != 0xE) {
		/* Check MD Error */
		if (Rreg32(MD_SMQ_INT_STATUS) & 0xFFFFFFF7) {
			DBG("[MD ERROR] MD_SMQ_INT_STATUS = 0x%x\n", Rreg32(MD_SMQ_INT_STATUS));
			DBG("[MD ERROR] MD_SMQ_CNTL = 0x%x\n", Rreg32(MD_SMQ_CNTL));
			return 1;
		}
	}

	/* Test SCPU wrapper command (Not to access 64-byte aligned address) */
	DBG("Test CPU wrapper mode at %p ~ %p: ", sAddr, eAddr);
	for (i = 0; i < loop; ++i) {
		if (Rreg32(((datum64)sAddr + step*i + offset)) != 0x5a5a5a5a) {
			DBG("Fail\n");
			return 1;
		}

		// Dump memory trash
#if MT_ENABLE
		dcsys_memory_trash_init(sAddr, eAddr, region);
		DBG("\nRead 0x%x, DC_MT_ADDCMD_LO_0 = 0x%x\n", (datum)((datum64)sAddr + step*i + offset), Rreg32(DC_MT_ADDCMD_LO_0(region)));
#endif
	}
	DBG("OK\n");

	return 0;
}

uint32_t cp_sha256(uint8_t *src_addr, uint32_t length, uint8_t *dst_addr)
{
	//uint32_t count = 0;
	t_mcp_descriptor *mcp_dscpt = (t_mcp_descriptor *) 0x80007000;

	Wreg32(K_MCP_DES_COUNT, 0x80000000);

	mcp_dscpt->mode = 0xb;

	mcp_dscpt->key[0] = SHA256_H0;
	mcp_dscpt->key[1] = SHA256_H1;
	mcp_dscpt->key[2] = SHA256_H2;
	mcp_dscpt->key[3] = SHA256_H3;
	mcp_dscpt->key[4] = SHA256_H4;
	mcp_dscpt->key[5] = SHA256_H5;
	mcp_dscpt->ini_key[0] = SHA256_H6;
	mcp_dscpt->ini_key[1] = SHA256_H7;

	mcp_dscpt->src_addr = (uintptr_t)src_addr;
	mcp_dscpt->dst_addr = (uintptr_t)dst_addr;
	mcp_dscpt->length = length;

	Wreg32(K_MCP_EN, 0xfe);		// disable interrupt
	Wreg32(K_MCP_CTRL, 0x2);	// disable go bit

	// set ring buffer register
	Wreg32(K_MCP_BASE, (uintptr_t)mcp_dscpt);
	Wreg32(K_MCP_LIMIT, (uintptr_t)mcp_dscpt + sizeof(t_mcp_descriptor) * 2);
	Wreg32(K_MCP_RDPTR, (uintptr_t)mcp_dscpt);
	Wreg32(K_MCP_WRPTR, (uintptr_t)mcp_dscpt + sizeof(t_mcp_descriptor));	// writer pointer cannot be equal to limit

	Wreg32(K_MCP_CTRL, 0xB);	 // go MCP

	while ((Rreg32(K_MCP_STATUS) & 0x6) == 0);

	// clear MCP register
	Wreg32(K_MCP_CTRL, 0x2);
	Wreg32(K_MCP_STATUS, 0x6);

	return 1;
}

uint32_t md_test(void)
{
	uint32_t i = 0;
	t_md_descriptor *md_dscpt = (t_md_descriptor *)MD_DES_ADDR;
	uint32_t *Hash = (uint32_t *) CP_HASH_ADDR;


	/* Disable MD go bit */
	//Wreg32(MD_SMQ_CNTL, 0x6);
	//Wreg32(MD_SMQ_INT_STATUS, 0xFFFFFFFE);
	Wreg32_M(REG_CRT, 0xFBFFFFFF, 0x0);
	Wreg32_M(REG_CRT, 0xFBFFFFFF, 0x4000000);

	/* MD constant write mode */
	for (i = 0; i < PAT_NUM; ++i) {
		(md_dscpt + i)->mode = 0x45;
		(md_dscpt + i)->Daddr = MD_START_ADDR + i*MD_LENGTH;
		(md_dscpt + i)->Saddr = i;
		(md_dscpt + i)->Length = MD_LENGTH & 0xFFFFF;
	}
	(md_dscpt + PAT_NUM)->mode = 0x45;
	(md_dscpt + PAT_NUM)->Daddr = MD_DST_ADDR;
	(md_dscpt + PAT_NUM)->Saddr = 0x0;
	(md_dscpt + PAT_NUM)->Length = 0x0;
	(md_dscpt + PAT_NUM + 1)->mode = 0x5;
	(md_dscpt + PAT_NUM + 1)->Daddr = MD_DST_ADDR;
	(md_dscpt + PAT_NUM + 1)->Saddr = MD_START_ADDR;
	(md_dscpt + PAT_NUM + 1)->Length = 0x0;
	(md_dscpt + PAT_NUM + 2)->mode = 0x45;
	(md_dscpt + PAT_NUM + 2)->Daddr = MD_START_ADDR;
	(md_dscpt + PAT_NUM + 2)->Saddr = 0x0;
	(md_dscpt + PAT_NUM + 2)->Length = 0x0;

	/* Use CP to verify the MD command */
	cp_sha256((uint8_t *)MD_DES_ADDR, sizeof(t_md_descriptor)*(PAT_NUM + 3), (uint8_t *)Hash);

	for (i = 0; i < 8; ++i) {
		if (Hash[i] != Gcmd[i]) {
			/* Debug */
			DBG("[MD CMD ERROR] Hash =  ");
			for (i = 0; i < 8; i++) DBG("0x%x ", Hash[i]);
			DBG("\n");

			return 0;
		}
		Hash[i] = 0;
	}

	/* Set MD ring buffer */
	Wreg32(MD_SMQBase, (uintptr_t)md_dscpt);
	Wreg32(MD_SMQLimit, (uintptr_t)md_dscpt + sizeof(t_md_descriptor)*(PAT_NUM + 4));
	Wreg32(MD_SMQRdptr, (uintptr_t)md_dscpt);
	Wreg32(MD_SMQWrptr, (uintptr_t)md_dscpt + sizeof(t_md_descriptor)*(PAT_NUM + 3));

	/* Enable MD go bit */
	Wreg32(MD_SMQ_CNTL, 0x7);

	while (Rreg32(MD_SMQ_CNTL) != 0xE) {
		/* Check MD Error */
		if (Rreg32(MD_SMQ_INT_STATUS) & 0xFFFFFFF7) {
			DBG("[MD ERROR] MD_SMQ_INT_STATUS = 0x%x\n", Rreg32(MD_SMQ_INT_STATUS));
			DBG("[MD ERROR] MD_SMQ_CNTL = 0x%x\n", Rreg32(MD_SMQ_CNTL));
			return 0;
		}
	}

	/* Use CP to verify the DDR content */
	cp_sha256((uint8_t *)MD_DST_ADDR, CP_LENGTH, (uint8_t *)Hash);

	for (i = 0; i < 8; ++i) {
		if (Hash[i] != Gdata[i]) {
			/* Debug */
			DBG("[CP ERROR] Hash =  ");
			for (i = 0; i < 8; i++) DBG("0x%x ", Hash[i]);
			DBG("\n");

			return 0;
		}
		Hash[i] = 0;
	}

	return 1;
}

/* DRAM Data Bus Test */
int mem_test_data_bus(datum *pAddr, uintptr_t *pFailAddr)
{
	datum pattern;

	*pFailAddr = (uintptr_t) NULL;

	/* Perform a walking 1's test at the given address. */
	for (pattern = 1; pattern != 0; pattern <<= 1)
	{
		/* Write the test pattern. */
		*pAddr = pattern;

		/* Read back the test pattern. */
		if (*pAddr != pattern)
		{
			*pFailAddr = (uintptr_t) pAddr;
			DBG("Data bit probably exists open-short fault.\n");
			return 0;
		}
	}

	return 1;
}

/* DRAM Address Bus Test */
int mem_test_addr_bus(datum baseAddr, unsigned int numBytes, uintptr_t *pFailAddr)
{
	unsigned int addrMask = numBytes - 1;
	unsigned int offset, testOffset;
	datum pattern = 0xAAAAAAAA, antipattern = ~pattern;

	*pFailAddr = (uintptr_t) NULL;

	/* Write the default pattern at each of the power-of-two offsets. */
	for (offset = sizeof(datum); (offset & addrMask) != 0; offset <<= 1)
		*(datum *)((uintptr_t)(baseAddr | offset)) = pattern;

	/* Check for address bits stuck at high. */
	*(datum *)((uintptr_t)baseAddr) = antipattern;

	for (offset = sizeof(datum); offset & addrMask; offset <<= 1)
	{
		if (*(datum *)((uintptr_t)(baseAddr | offset)) != (uintptr_t)pattern)
		{
			*pFailAddr = (uintptr_t)(baseAddr | offset);
			DBG("Address bit occurs stuck-at-high fault.\n");
			return 0;
		}
	}

	*(datum *)((uintptr_t)baseAddr) = pattern;

	/* Check for address bits suck at low or short to ecah other. */
	for (testOffset = sizeof(datum); testOffset & addrMask; testOffset <<= 1)
	{
		*(datum *)((uintptr_t)(baseAddr | testOffset)) = antipattern;

		if (*(datum *)((uintptr_t)baseAddr) != (uintptr_t)pattern)
		{
			*pFailAddr = (uintptr_t)(baseAddr | testOffset);
			DBG("Address bit occurs stuck-at-low fault.\n");
			return 0;
		}

		for (offset = sizeof(datum); offset & addrMask; offset <<= 1)
		{
			if ((*(datum *)((uintptr_t)(baseAddr | offset)) != (uintptr_t)pattern) && (offset != testOffset))
			{
				*pFailAddr = (uintptr_t)(baseAddr | offset);
				DBG("Address bits are probably shorted to each other.\n");
				return 0;
			}
		}

		*(datum *)((uintptr_t)(baseAddr | testOffset)) = (uintptr_t)pattern;
	}

	return 1;
}

static void diag_pass(void) {
#if PLATFORM_RTD1295
	rtd_setbits(MIS_GP0DIR,  BIT20);
	rtd_clearbits(MIS_GP0DATO, BIT20);
	printf("1295 GPIO20 low\n");
#endif
#if PLATFORM_RTD1395
	rtd_clearbits(ISO_GPDATO, BIT30);
	rtd_setbits(ISO_GPDIR, BIT30);
	printf("1395 GPIO30 low\n");
#endif
}

/* Memory Test for 1GB DDR */
void mem_test_entry1(void)
{
	/* 1GB DDR Settings */
	datum baseAddr = 0x20000000;
	datum numBytes = 0x20000000;

	uintptr_t FailAddr;

	/* disable MMU, DCACHE, INTERRUPTS */
	arch_disable_ints();
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();

	/* DDR Data Bus Test */
	DBG("DRAM data bus test for DC0: ");
	if (!mem_test_data_bus((datum *)TST_ADDR, &FailAddr)) {
		DBG("Fail\n");
		DBG("fail address = 0x%lx\n", FailAddr);
		return;
	}
	else DBG("OK\n");

	/* DDR Address Bus Test */
	DBG("DRAM address bus test for DC0: ");
	if (!mem_test_addr_bus(baseAddr, numBytes, &FailAddr)) {
		DBG("Fail\n");
		DBG("fail address = 0x%lx\n", FailAddr);
		return;
	}
	else DBG("OK\n");

	/* DDR MD Test */
	DBG("MD test for DC0: ");
	if (!md_test()) {
		DBG("Fail\n");
		return;
	}
	else DBG("OK\n");

	/* enable MMU, DCACHE, INTERRUPTS */
	platform_init_secondary_mmu_mappings();
	arch_enable_ints();

	diag_pass();
}

/* Memory Test for 2GB DDR */
void mem_test_entry2(void)
{
	/* 2GB DDR Settings */
	datum baseAddr = 0x40000000;
	datum numBytes = 0x40000000;

	uintptr_t FailAddr;

	/* disable MMU, DCACHE, INTERRUPTS */
	arch_disable_ints();
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();

	/* DDR Data Bus Test */
	DBG("DRAM data bus test for DC0: ");
	if (!mem_test_data_bus( (datum *)TST_ADDR, &FailAddr)) {
		DBG("Fail\n");
		DBG("fail address = 0x%lx\n", FailAddr);
		return;
	}
	else DBG("OK\n");

	/* DDR Address Bus Test */
	DBG("DRAM address bus test for DC0: ");
	if (!mem_test_addr_bus(baseAddr, numBytes, &FailAddr)) {
		DBG("Fail\n");
		DBG("fail address = 0x%lx\n", FailAddr);
		return;
	}
	else DBG("OK\n");

	/* DDR MD Test */
	DBG("MD test for DC0: ");
	if (!md_test()) {
		DBG("Fail\n");
		return;
	}
	else DBG("OK\n");

	/* enable MMU, DCACHE, INTERRUPTS */
	platform_init_secondary_mmu_mappings();
	arch_enable_ints();

	diag_pass();
}

/* Memory Test for 1296 */
void mem_test_entry3(void)
{
	datum *dc_sys_misc = (datum *)0x98008004;

	/* 2GB DDR Settings */
	datum baseAddr = 0x20000000;
	datum numBytes = 0x20000000;

	uintptr_t pFailAddr;

	/* disable MMU, DCACHE, INTERRUPTS */
	arch_disable_ints();
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();

	/* DDR Data Bus Test for DC0 */
	DBG("DRAM data bus test for DC0: ");
	if (!mem_test_data_bus( (datum *)TST_ADDR, &pFailAddr)) {
		DBG("Fail\n");
		DBG("\nDC0 fail address = %lx\n", pFailAddr);
		return;
	}
	else DBG("OK\n");

	/* DDR MD Test for DC0 */
	DBG("MD test for DC0: ");
	if (!md_test()) {
		DBG("Fail\n");
		return;
	}
	else DBG("OK\n");

	/* 1296 FT LB Work around */
	*dc_sys_misc = 0x01c00fcd;		// Enable two dc_sys and set the granule size to 256MB

	/* DDR Data Bus Test for DC1 */
	DBG("DRAM data bus test for DC1: ");
	if (!mem_test_data_bus( (datum *)0x10000000, &pFailAddr)) {
		DBG("Fail\n");
		DBG("DC1 fail address = %lx\n", pFailAddr);
		return;
	}
	else DBG("OK\n");

	/* DDR MD Test for DC1 */
	DBG("MD test for DC1: ");
	if (!md_test()) {
		DBG("Fail\n");
		return;
	}
	else DBG("OK\n");

	/* DDR Address Bus Test */
	DBG("DRAM address bus test for DC0 and DC1: ");
	if (!mem_test_addr_bus(baseAddr, numBytes, &pFailAddr)) {
		DBG("Fail\n");
		DBG("fail address = %lx\n", pFailAddr);
		return;
	}
	else DBG("OK\n");

	/* 1296 FT LB Work around */
	*dc_sys_misc = 0x01c00f00;		// Enable one dc_sys only

	/* enable MMU, DCACHE, INTERRUPTS */
	platform_init_secondary_mmu_mappings();
	arch_enable_ints();

	diag_pass();
}

/* SCPU Wrapper Mode Test with DDR scramble */
void scpu_test_entry1(void)
{
	int err = 0;

	/* disable MMU, DCACHE, INTERRUPTS */
	arch_disable_ints();
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();

	/* Enable DC Scramble Function */
	err |= dc_scramble_enable((datum*)(TST_ADDR + 0x0000), (datum*)(TST_ADDR + 0x2000), 1, FAST_MODE);
	err |= dc_scramble_enable((datum*)(TST_ADDR + 0x2000), (datum*)(TST_ADDR + 0x4000), 2, AES);
	err |= dc_scramble_enable((datum*)(TST_ADDR + 0x4000), (datum*)(TST_ADDR + 0x8000), 4, AES);
	err |= dc_scramble_enable((datum*)(TST_ADDR + 0x8000), (datum*)(TST_ADDR + 0xa000), 8, AES);
	
	/* Test SCPU wrapper mode */
	err |= scpu_wrapper_mode_test((datum*)(TST_ADDR + 0x0000), (datum*)(TST_ADDR + 0x2000), 0, 0x30, 0x300, 10);
	err |= scpu_wrapper_mode_test((datum*)(TST_ADDR + 0x2000), (datum*)(TST_ADDR + 0x4000), 1, 0x30, 0x300, 10);
	err |= scpu_wrapper_mode_test((datum*)(TST_ADDR + 0x4000), (datum*)(TST_ADDR + 0x8000), 2, 0x30, 0x300, 10);
	err |= scpu_wrapper_mode_test((datum*)(TST_ADDR + 0x8000), (datum*)(TST_ADDR + 0xa000), 3, 0x30, 0x300, 10);

	if (!err) {
		rtd_clearbits(ISO_GPDATO_1, BIT3);
		rtd_setbits(ISO_GPDIR_1, BIT3);
		printf("1395 GPIO35 low\n");
	}

	/* enable MMU, DCACHE, INTERRUPTS */
	platform_init_secondary_mmu_mappings();
	arch_enable_ints();

	/* Disable DDR Scramble */
	Wreg32(DC_mem_scramble_ctrl, 0x00000000);
	_sync();
}
