//--=========================================================================--
//  This file is a part of VPU Reference API project
//-----------------------------------------------------------------------------
//
//       This confidential and proprietary software may be used only
//     as authorized by a licensing agreement from Chips&Media Inc.
//     In the event of publication, the following notice is applicable:
//
//            (C) COPYRIGHT CHIPS&MEDIA INC.
//                      ALL RIGHTS RESERVED
//
//       The entire notice above must be reproduced on all authorized
//       copies.
//
//--=========================================================================--
#ifndef _SKIP_H_
#define _SKIP_H_

typedef enum {
    NUT_TRAIL_N         = 0,
    NUT_TRAIL_R         = 1,
    NUT_TSA_N           = 2,
    NUT_TSA_R           = 3,
    NUT_STSA_N          = 4,
    NUT_STSA_R          = 5,
    NUT_RADL_N          = 6,
    NUT_RADL_R          = 7,
    NUT_RASL_N          = 8,
    NUT_RASL_R          = 9,
    NUT_RSV_N10         = 10,
    NUT_RSV_R11         = 11,
    NUT_RSV_N12         = 12,
    NUT_RSV_R13         = 13,
    NUT_RSV_N14         = 14,
    NUT_RSV_R15         = 15,
    NUT_BLA_W_LP        = 16,
    NUT_BLA_W_RADL      = 17,
    NUT_BLA_N_LP        = 18,
    NUT_IDR_W_RADL      = 19,
    NUT_IDR_N_LP        = 20,
    NUT_CRA             = 21,
    NUT_RSV_IRAP22      = 22,
    NUT_RSV_IRAP23      = 23,
    NUT_VPS             = 32,
    NUT_SPS             = 33,
    NUT_PPS             = 34,
    NUT_AUD             = 35,
    NUT_EOS             = 36,
    NUT_EOB             = 37,
    NUT_FD              = 38,
    NUT_PREFIX_SEI      = 39,
    NUT_SUFFIX_SEI      = 40,
    NUT_MAX             = 63
} NalType;

#define H265_MAX_TEMPORAL_ID        6

#define SKIP_CMD_DECODE             0
#define SKIP_CMD_NON_IRAP           1       //<<! Skip except IRAP pictures, H.265
#define SKIP_CMD_NON_REF            2       //<<! Skip except non-reference pictures
#define SKIP_CMD_TARGET_SUBLAYER    3       //<<! Temporal scalability
#define SKIP_CMD_NONE               4
typedef struct _SkipCmd {
    uint32_t    cmd;
    uint32_t    tid;                        //<<! valid when cmd is SKIP_CMD_TARGET_SUBLAYER
} SkipCmd;

typedef struct DecodedPicInfo {
    int32_t  displayNo;
    int32_t  decodedNo;             //<<! Logical decoded sequence number
    int32_t  picType;
    NalType  nalType;
    int32_t  temporalId;
    int32_t  width;
    int32_t  height;
    BOOL     skip;                  //<<! TRUE - skip
} DecodedPicInfo;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    DecodedPicInfo* LoadDecodedInfoFile(
        const char* path,
        uint32_t*   nframes
    );

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _SKIP_H_ */

