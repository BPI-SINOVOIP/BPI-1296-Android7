#ifndef __CPU_H__
#define __CPU_H__

#include "rbus/iso_reg.h"

#define CPU_RELEASE_ADDR		ISO_ISO_MODE_REG_4

void rtk_mmu_on(u64 ttbr_addr, u64 tcr_flags, u64 mair);
void rtk_cache_coherence_setting(void);

void bootup_slave_cpu(void);

#endif
