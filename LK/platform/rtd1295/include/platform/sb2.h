#pragma once

#define KYLIN_REV_A00   0x0000
#define KYLIN_REV_A01   0x0001
#define KYLIN_REV_B00   0x0002


#include <platform/rbus/sb2_reg.h>
#include <platform/rbus/iso_reg.h>

static inline int get_chip_rev(void)
{
    return (*(unsigned int *)SB2_CHIP_INFO) >> 16;
}

#define EFUSE_CHIP_INFO         0x980171d8

#define RTK1295_CPU_ID          0x00000000
#define RTK1294_CPU_ID          0x00000001
#define RTK1296_CPU_ID          0x00000002
#define RTK1198_CPU_ID          0x00000003

#define RTK129x_CPU_MASK        0x00000003
#define RTK1296_CPU_MASK        0x00000800
#define RTK1198_CPU_MASK        0x000003FF
#define RTK1198_CPU_VAL         0x201

static inline int get_chip_id(void) {
    int cpu_id = RTK1295_CPU_ID;

    if (((*(unsigned int *)EFUSE_CHIP_INFO) & RTK129x_CPU_MASK) == RTK1294_CPU_ID) {
            cpu_id = RTK1294_CPU_ID;
    } else if ((*(unsigned int *)ISO_CHIP_INFO1) & RTK1296_CPU_MASK) {
            cpu_id = RTK1296_CPU_ID;
    } else if (((*(unsigned int *)ISO_CHIP_INFO1) & RTK1198_CPU_MASK) == RTK1198_CPU_VAL) {
			cpu_id = RTK1198_CPU_ID;
	}	

    return cpu_id;
}

