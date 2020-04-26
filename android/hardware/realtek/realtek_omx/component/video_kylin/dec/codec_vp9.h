#ifndef CODEC_VP9_H
#define CODEC_VP9_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "codec.h"
#include "ion/ion.h"

// create codec instance
    CODEC_PROTOTYPE *RTKHwDecOmx_decoder_create_vp9(PRIVATE_STRUCT pRtkInfo);

#ifdef __cplusplus
}
#endif
#endif                       // CODEC_VP9_H