#pragma once

#define HERCULES_REV_A00   0x0000
#define HERCULES_REV_A01   0x0001
#define HERCULES_REV_B00   0x0002


#include <platform/rbus/sb2_reg.h>
#include <platform/rbus/iso_reg.h>

static inline int get_chip_rev(void)
{
    return (*(unsigned int *)SB2_CHIP_INFO) >> 16;
}

#define RTK1355_CPU_ID          0x00000000
#define RTK1395_CPU_ID          0x00000001

#define RTK1355_CPU_MASK        0x00000800

static inline int get_chip_id(void) {
    int cpu_id = RTK1395_CPU_ID;

    if (((*(unsigned int *)ISO_CHIP_INFO1) & RTK1355_CPU_MASK) == RTK1355_CPU_ID) {
            cpu_id = RTK1355_CPU_ID;
    }

    return cpu_id;
}

