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
#include <bits.h>
#include <err.h>
#include <debug.h>
#include <trace.h>
#include <lk/init.h>
#include <platform.h>
#include <platform/gic.h>
#include <platform/interrupts.h>
#include <platform/rtd1395.h>
#include <platform/cpu.h>
#include <libfdt.h>
#include <arch/arm64/mmu.h>
#include <arch/ops.h>
#include <kernel/vm.h>
#include "platform_p.h"

/* RTD 1295 mapping
 * Use 4K table granule, 2MB block mapping
 * - Map total 1G DDR 0x00000000 ~ 0x40000000
 * - Map RBUS 0x98000000 ~ 0x9820000
 * - Map CPU interface 0xFF000000 ~ 0x100000000
 */

#define LOCAL_TRACE	0

#define TABLE_GRANULE			0x1000	// use 4K table size
#define TABLE_COUNT			8	// Default allocated page tables
#define PAGE_ENTRIES			(TABLE_GRANULE / sizeof(pte_t))
#define MAX_L1_TABLES			2
#define MAX_L2_TABLES			4

#define RTK_BLOCK_NORMAL_MEMORY_DESCRIPTOR ( \
	MMU_PTE_ATTR_AF | \
	MMU_PTE_ATTR_SH_INNER_SHAREABLE | \
	MMU_PTE_ATTR_NORMAL_MEMORY | \
	MMU_PTE_ATTR_AP_P_RW_U_RW | \
	MMU_PTE_L012_DESCRIPTOR_BLOCK)

#define RTK_BLOCK_NORMAL_NC_MEMORY_DESCRIPTOR ( \
	MMU_PTE_ATTR_AF | \
	MMU_PTE_ATTR_SH_INNER_SHAREABLE | \
	MMU_PTE_ATTR_NORMAL_NC_MEMORY | \
	MMU_PTE_ATTR_AP_P_RW_U_RW | \
	MMU_PTE_L012_DESCRIPTOR_BLOCK)

#define RTK_BLOCK_DEVICE_MEMORY_DESCRIPTOR ( \
	MMU_PTE_ATTR_UXN | \
	MMU_PTE_ATTR_PXN | \
	MMU_PTE_ATTR_AF | \
	MMU_PTE_ATTR_DEVICE | \
	MMU_PTE_ATTR_AP_P_RW_U_NA | \
	MMU_PTE_L012_DESCRIPTOR_BLOCK)

#define VA_BITS			40
#define TCR_IPS_BIT_EL1 	(2UL << 32)
#define TCR_IPS_BIT_EL2 	(2UL << 16)
#define TCR_IPS_BIT_EL3 	(2UL << 16)

#define RTK_TCR_FLAGS ( \
	(MMU_TCR_TG0(MMU_TG0(MMU_USER_PAGE_SIZE_SHIFT))) | \
	MMU_TCR_SH0(MMU_SH_INNER_SHAREABLE) | \
	MMU_TCR_ORGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | \
	MMU_TCR_IRGN0(MMU_RGN_WRITE_BACK_ALLOCATE) | \
	MMU_TCR_T0SZ(64 - VA_BITS))

#define PAGE_TABLE_SIZE	(TABLE_COUNT * TABLE_GRANULE / sizeof(pte_t))
/* Page Table, default allocat 8 4K pages to cover 0x0 ~ 0xFFFFFFFF */
pte_t page_table[PAGE_TABLE_SIZE]
	__ALIGNED(0x10000)
	__SECTION(".bss.prebss.translation_table");

pte_t *lvl0_tbl = page_table;
pte_t *lvl1_tbl0 = page_table + (1 * PAGE_ENTRIES);
pte_t *lvl1_tbl1 = page_table + (2 * PAGE_ENTRIES);
pte_t *lvl2_tbl0 = page_table + (3 * PAGE_ENTRIES);
pte_t *lvl2_tbl1 = page_table + (4 * PAGE_ENTRIES);
pte_t *lvl2_tbl2 = page_table + (5 * PAGE_ENTRIES);
pte_t *lvl2_tbl3 = page_table + (6 * PAGE_ENTRIES);

pte_t *lvl1_page_ref[MAX_L1_TABLES] = {
	[0] = page_table + (1 * PAGE_ENTRIES),
	[1] = page_table + (2 * PAGE_ENTRIES),
};

pte_t *lvl2_page_ref[MAX_L1_TABLES][MAX_L2_TABLES] = {
	[0][0] = page_table + (3 * PAGE_ENTRIES),
	[0][1] = page_table + (4 * PAGE_ENTRIES),
	[0][2] = page_table + (5 * PAGE_ENTRIES),
	[0][3] = page_table + (6 * PAGE_ENTRIES),
};

struct rtk_mmu_mapping {
	paddr_t phys;
	vaddr_t virt;
	size_t  size;
	u64 flags;
	const char *name;
};

#define MAPPING_TABLE_COUNT		8
/* initial memory mappings. Please make sure that PHYS/VIRT ADDR is within 2MB alignment */
struct rtk_mmu_mapping rtk_mmu_initial_mappings[MAPPING_TABLE_COUNT] = {
    /* all of memory */
    { // First, default map all memory as Normal memory
        .phys = MEMORY_BASE_PHYS,
        .virt = MEMORY_BASE_VIRT,
        .size = MEMORY_SIZE,
        .flags = RTK_BLOCK_NORMAL_MEMORY_DESCRIPTOR,
        .name = "memory"
    },

    {
        .phys = 0x20000000,
        .virt = 0x20000000,
        .size = 0x20000000,
        .flags = RTK_BLOCK_NORMAL_NC_MEMORY_DESCRIPTOR,
        .name = "non-cache memory"
    },

    { // Map RBUS region
        .phys = PERIPHERAL_BASE_PHYS,
        .virt = PERIPHERAL_BASE_VIRT,
        .size = PERIPHERAL_BASE_SIZE,
        .flags = RTK_BLOCK_DEVICE_MEMORY_DESCRIPTOR,
        .name = "peripherals"
    },

	{ // Map NOR flash region
        .phys = NOR_FLASH_BASE_PHYS,
        .virt = NOR_FLASH_BASE_VIRT,
        .size = NOR_FLASH_BASE_SIZE,
        .flags = RTK_BLOCK_DEVICE_MEMORY_DESCRIPTOR,
        .name = "nor flash"
    },

    {
        .phys = CPUPRIV_BASE_PHYS,
        .virt = CPUPRIV_BASE_VIRT,
        .size = CPUPRIV_SIZE,
        .flags = RTK_BLOCK_DEVICE_MEMORY_DESCRIPTOR,
        .name = "cpu"
    },

#ifdef VMX
    {
        .phys = VMX_FW_TABLE_BASE_PHYS,
        .virt = VMX_FW_TABLE_BASE_VIRT,
        .size = VMX_FW_TABLE_SIZE,
        .flags = RTK_BLOCK_NORMAL_MEMORY_DESCRIPTOR,
        .name = "fw table"
    },
#endif

    /* null entry to terminate the list */
    { 0 }
};

/*
 * Check whether an entry cover mapping range
 */
static inline int is_in_mapping_range(
		vaddr_t entry_start, vaddr_t entry_end,
		vaddr_t mapping_start, vaddr_t mapping_end)
{
	/* First, check the case that entry is greater than mapping */
	if (mapping_start > entry_start && mapping_end < entry_end)
		return 1;
	if (entry_start >= mapping_start && entry_start < mapping_end)
		return 1;
	if (entry_end <= mapping_end && entry_end > mapping_start)
		return 1;

	return 0;
}

static void setup_level_2_table(pte_t *target_table, vaddr_t map_vaddr, u64 map_size)
{
	int n_entry = TABLE_GRANULE / sizeof(pte_t); // Number of entries in this table
	u64 entry_map_size = map_size / n_entry; // size that each entry maps
	vaddr_t entry_vaddr; // Current entry mapping vaddr
	pte_t *entry = target_table;
	int i, j;
	u64 virt_offset;

	entry_vaddr = map_vaddr;

	for (i = 0 ; i < n_entry ; i++) {

		for (j = 0 ; j < MAPPING_TABLE_COUNT ; j++) {
			struct rtk_mmu_mapping *mapping_table = &rtk_mmu_initial_mappings[j];
			bool need_mapping = is_in_mapping_range(entry_vaddr, entry_vaddr + entry_map_size,
						mapping_table->virt, mapping_table->virt + mapping_table->size);

			if (mapping_table->size == 0)
				break;

			if (need_mapping) {
					virt_offset = entry_vaddr - mapping_table->virt;
					entry[i] = mapping_table->phys + virt_offset;
					entry[i] |= mapping_table->flags;
			}
		}
		entry_vaddr += entry_map_size;
	}
}

/*
 * Setup target level1 pagetable, and also setup level2
 * pagetable it useses.
 *
 * target_table : Table passed in to be setup
 * tbl_start : Start VADDR this table maps
 * tbl_size  : VADDR size this table map
 * table_refs : L1 table number for level 1/2 refs
 */
static void setup_level_1_table(pte_t *target_table, unsigned table_refs, vaddr_t map_vaddr, u64 map_size)
{
	int n_entry = TABLE_GRANULE / sizeof(pte_t); // Number of entries in this table
	u64 entry_map_size = map_size / n_entry; // size that each entry maps
	vaddr_t entry_vaddr; // Current entry mapping vaddr
	pte_t *entry = target_table;
	int i, j;

	entry_vaddr = map_vaddr;
	//dprintf(INFO, "Setting lvl1 %u (0x%08lx), map_vaddr:0x%08lx, map_size:0x%08lx\n",
	//	table_refs, target_table, map_vaddr, map_size);

	for (i = 0 ; i < n_entry ; i++) {

		for (j = 0 ; j < MAPPING_TABLE_COUNT ; j++) {
			struct rtk_mmu_mapping *mapping_table = &rtk_mmu_initial_mappings[j];
			bool need_mapping = is_in_mapping_range(entry_vaddr, entry_vaddr + entry_map_size,
						mapping_table->virt, mapping_table->virt + mapping_table->size);

			if (mapping_table->size == 0)
				break;

			if (need_mapping) {
				entry[i] = (pte_t)lvl2_page_ref[table_refs][i] | MMU_PTE_L012_DESCRIPTOR_TABLE;
				setup_level_2_table(lvl2_page_ref[table_refs][i], entry_vaddr, entry_map_size);
			}
		}
		entry_vaddr += entry_map_size;
	}
}

void platform_init_mmu_mappings(void)
//void test_platform_init_mmu_mappings(void)
{
	int i;
	u64 entry_map_size = 0x8000000000; // 512G for each entry at lvl0
	vaddr_t entry_vaddr = 0;
	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;

	arch_clean_invalidate_cache_range(MEMORY_BASE_PHYS, MEMORY_SIZE);
	invalidate_tlb_all();

	if (current_el == 3)
		rtk_cache_coherence_setting();

	/* Default zero out entire PT, make invalid addr trigger Exception */
	memset(page_table, 0, TABLE_COUNT * TABLE_GRANULE);

	/* Start at Level 0 and calling into level 2~3 */
	for (i = 0 ; i < MAX_L1_TABLES ; i++ ) {
		lvl0_tbl[i] = (pte_t)lvl1_page_ref[i] | MMU_PTE_L012_DESCRIPTOR_TABLE;
		setup_level_1_table(lvl1_page_ref[i], i, entry_vaddr, entry_map_size);
		entry_vaddr += entry_map_size;
	}

	if (current_el == 1) {
		rtk_mmu_on((u64)page_table, RTK_TCR_FLAGS | TCR_IPS_BIT_EL1, MMU_MAIR_VAL);
	} else if (current_el == 2) {
		rtk_mmu_on((u64)page_table, RTK_TCR_FLAGS | TCR_IPS_BIT_EL2, MMU_MAIR_VAL);
	} else {
		rtk_mmu_on((u64)page_table, RTK_TCR_FLAGS | TCR_IPS_BIT_EL3, MMU_MAIR_VAL);
	}

}

void platform_init_secondary_mmu_mappings(void)
{
	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;

	arch_invalidate_cache_range(MEMORY_BASE_PHYS, MEMORY_SIZE);
	invalidate_tlb_all();

	if (current_el == 3)
		rtk_cache_coherence_setting();

	if (current_el == 1) {
		rtk_mmu_on((u64)page_table, RTK_TCR_FLAGS | TCR_IPS_BIT_EL1, MMU_MAIR_VAL);
	} else if (current_el == 2) {
		rtk_mmu_on((u64)page_table, RTK_TCR_FLAGS | TCR_IPS_BIT_EL2, MMU_MAIR_VAL);
	} else {
		rtk_mmu_on((u64)page_table, RTK_TCR_FLAGS | TCR_IPS_BIT_EL3, MMU_MAIR_VAL);
	}
}

int mmu_enabled(void)
{
	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
	unsigned int sctlr = 0;

	if (current_el == 1) {
		sctlr = ARM64_READ_SYSREG(sctlr_el1);
	} else if (current_el == 2) {
		sctlr = ARM64_READ_SYSREG(sctlr_el2);
	} else {
		sctlr = ARM64_READ_SYSREG(sctlr_el3);
	}

	return BIT(sctlr, 0);
}
