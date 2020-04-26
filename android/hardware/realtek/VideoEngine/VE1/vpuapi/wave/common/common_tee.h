#ifndef __COMMON_TEE_FUNCTION_H__
#define __COMMON_TEE_FUNCTION_H__

#include "vpuapi.h"
#include "product.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    RetCode Wave4VpuTEEInit(
        Uint32      coreIdx,
        void*       firmware,
        Uint32      size
    );

    RetCode Wave4VpuTEEReInit(
        Uint32      coreIdx,
        void*       firmware,
        Uint32      size
    );

    RetCode Wave4VpuTEEDecRegisterFramebuffer(
        CodecInst*      instance,
        FrameBuffer*    fb,
        TiledMapType    mapType,
        Uint32          count
    );

    RetCode Wave4VpuTEEPrepareDecodingPicture(
        CodecInst* instance,
        Uint32 cmd
    );
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMON_TEE_FUNCTION_H__ */

