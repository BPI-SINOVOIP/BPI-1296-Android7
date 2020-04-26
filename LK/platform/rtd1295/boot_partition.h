#ifndef __BOOT_PARTITION_H__
#define __BOOT_PARTITION_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/rbus/crt_sys_reg.h>
#include <platform/rbus/sb2_reg.h>
#include <platform/io.h>
#include <platform/sb2.h>
#include <platform/setting.h>
#include <lib/heap.h>
#include <lib/bio.h>
#include <lib/console.h>
#include <arch/ops.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include <debug.h>
#include <lib/mincrypt/sha256.h>

#include "rtkemmc.h"

/* Enable boot power-on write protect */
#define EXT_CSD_BOOT_WP_PWR_WP_EN	(1 << 0)

/* Bit 1 (Power-on) or Bit 3 (Permanent) selects the partition to protect */
#define EXT_CSD_BOOT_WP_PART_SELECT	(1 << 7)

/* permanent write protection select */
#define EXT_CSD_BOOT_PERM_EN         (1<<2)

/* Power-on write protect only one boot partition */
#define EXT_CSD_BOOT_WP_PWR_SEL_PART1	(0 << 1)
#define EXT_CSD_BOOT_WP_PWR_SEL_PART2	(1 << 1)

//BOOT_WP_STATUS 
#define BOOT1_POWERON_PROTECTION    (1<<0)
#define BOOT1_PERMANENT_PROTECTION  (1<<1)
#define BOOT2_POWERON_PROTECTION    (1<<2)
#define BOOT2_PERMANENT_PROTECTION  (1<<3)

//BOOT_WP
#define B_SEC_WP_SEL   		  (1<<7)
#define B_PERM_WP_SEL_BOOT2   (1<<3)
#define B_PERM_WP_EN   		  (1<<2)
#define B_POW_WP_SEL_BOOT2    (1<<1)
#define B_PWR_WP_EN   		  (1<<0)


typedef enum {
	READ	     = 1,	
	WRITE        = 2,
	POWERON      = 3,
	PERMANENT  	 = 4,
	FAIL         = 5
}t_OperationType;
 



#endif 
