//--=========================================================================--
//  This file is a part of VPU Reference API project
//-----------------------------------------------------------------------------
//
//       This confidential and proprietary software may be used only
//     as authorized by a licensing agreement from Chips&Media Inc.
//     In the event of publication, the following notice is applicable:
//
//            (C) COPYRIGHT 2006 - 2011  CHIPS&MEDIA INC.
//                      ALL RIGHTS RESERVED
//
//       The entire notice above must be reproduced on all authorized
//       copies.
//
//--=========================================================================--
#ifndef __COMMON_FUNCTION_H__
#define __COMMON_FUNCTION_H__

#include "vpuapi.h"
#include "product.h"

#define VPU_HANDLE_TO_DECINFO(_handle)          (&(((CodecInst*)_handle)->CodecInfo.decInfo))
#define VPU_HANDLE_TO_ENCINFO(_handle)          (&(((CodecInst*)_handle)->CodecInfo.encInfo))

#define CODA7Q_MVCOL_BUF_SIZE(_w, _h)        ((((VPU_ALIGN32(_w)*VPU_ALIGN32(_h)*3)>>1)+4)/5)
#define CODA7Q_FBC_LUMA_TABLE_SIZE(_w, _h)   (((_h+15)/16)*((_w+255)/256)*128)
#define CODA7Q_FBC_CHROMA_TABLE_SIZE(_w, _h) (((_h+15)/16)*((_w/2+255)/256)*128)

#define WAVE4_SUBSAMPLED_ONE_SIZE(_w, _h)   ((((_w/4)+15)&~15)*(((_h/4)+7)&~7))

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    extern Int32 Wave4VpuGetProductId(
        Uint32      coreIdx
    );

    extern RetCode Wave4VpuGetVersion(
        Uint32  coreIdx,
        Uint32* versionInfo,
        Uint32* revision
    );

    extern RetCode Wave4VpuInit(
        Uint32      coreIdx,
        void*       firmware,
        Uint32      size
    );

    extern RetCode Wave4VpuReInit(
        Uint32      coreIdx,
        void*       firmware,
        Uint32      size
    );

    extern RetCode Wave4VpuDeinit(
        Uint32      coreIdx
    );

    extern Uint32 Wave4VpuIsInit(
        Uint32      coreIdx
    );

    extern Int32 Wave4VpuIsBusy(
        Uint32      coreIdx
    );

    extern Int32 Wave4VpuWaitInterrupt(
        Uint32      coreIdx,
        Int32       timeout
    );

    extern RetCode Wave4VpuClearInterrupt(
        Uint32      coreIdx,
        Uint32      flags
    );

    extern RetCode Wave4VpuReset(
        Uint32      coreIdx,
        SWResetMode resetMode
    );

    extern RetCode Wave4VpuSleepWake(
        Uint32      coreIdx,
        int         iSleepWake,
        const Uint16* code,
        Uint32 size
    );

    extern RetCode Wave4VpuBuildUpDecParam(
        CodecInst*      instance,
        DecOpenParam*   param
    );

    extern RetCode Wave4VpuDecFiniSeq(
        CodecInst*  instance
    );

    extern RetCode Wave4VpuDecRegisterFramebuffer(
        CodecInst*      instance,
        FrameBuffer*    fb,
        TiledMapType    mapType,
        Uint32          count
    );

    extern RetCode Wave4VpuDecSetBitstreamFlag(
        CodecInst*  instance,
        BOOL        running,
        BOOL        eos
    );


    extern RetCode Wave4VpuDecFlush(
        CodecInst*          instance,
        FramebufferIndex*   framebufferIndexes,
        Uint32              size
    );

    extern RetCode Wave4VpuDecCancelPrescan(
        CodecInst*          instance
    );

    extern RetCode Wave4DecWriteProtect(
        CodecInst* instance
    );

    extern RetCode Wave4EncWriteProtect(
        CodecInst* instance
    );

    extern void Wave4BitIssueCommand(
        CodecInst* instance,
        Uint32 cmd
    );

    extern RetCode Wave4VpuEncFiniSeq(
        CodecInst*  instance
    );

    extern RetCode Wave4VpuBuildUpEncParam(
        CodecInst*      pCodec,
        EncOpenParam*   param
    );

#ifdef ENABLE_TEE_DRM_FLOW
extern RetCode Wave4VpuInitProtect(
    Uint32      coreIdx,
    void*       firmware,
    Uint32      size
);
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMON_FUNCTION_H__ */

