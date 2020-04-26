//------------------------------------------------------------------------------
// File: vpuhelper.h
//
// Copyright (c) 2006, Realtek.  All rights reserved.
//------------------------------------------------------------------------------
#ifndef _VPUHELPER_H_
#define _VPUHELPER_H_

#include "vpuapifunc.h"
#include "vpuapi.h"

#if defined(__GENERIC_LINUX__) && defined(VPU_GET_CC)
#define	USER_DATA_INFO_OFFSET			(8*17)

typedef struct VpuReportConfig_t VpuReportConfig_t;

struct VpuReportConfig_t {
    PhysicalAddress userDataBufAddr;
    int userDataEnable;
    int userDataReportMode; // (0 : interrupt mode, 1 interrupt disable mode)
    int userDataBufSize;

};
#endif //defined(__GENERIC_LINUX__) && defined(VPU_GET_CC)

#if defined (__cplusplus) && defined(__LINARO_SDK__)
extern "C" {
#endif

    int FillBsResetBufHelper(Uint32 core_idx,
         unsigned char *buf,
         PhysicalAddress paBsBufAddr,
         int bsBufsize,
         int endian);

    int FillBsRingBufHelper(Uint32 core_idx,
        EncHandle handle,
        unsigned char *buf,
        PhysicalAddress paBsBufStart,
        PhysicalAddress paBsBufEnd,
        int defaultsize,
        int endian);

    int ReadBsResetBufHelper(Uint32 core_idx,
                         osal_file_t streamFp,
                         PhysicalAddress bitstream,
                         int size,
                         int endian);
#if defined (__cplusplus) && defined(__LINARO_SDK__)
}
#endif
#endif
