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

#ifndef VPUAPI_UTIL_H_INCLUDED
#define VPUAPI_UTIL_H_INCLUDED

#include "vpuapi.h"

// COD_STD
enum {
    HEVC_DEC = 0,
    AVC_DEC  = 0,
    VC1_DEC  = 1,
    HEVC_ENC  = 1,
    MP2_DEC  = 2,
    MP4_DEC  = 3,
    DV3_DEC  = 3,
    RV_DEC   = 4,
    AVS_DEC  = 5,
    VPX_DEC  = 7,
    MAX_DEC  = 7,
    AVC_ENC  = 8,
    MP4_ENC  = 11,
    MAX_CODECS,
};

// new COD_STD since Coda7Q, WAVE412
enum {
    C7_HEVC_DEC                 = 0x00,
    C7_HEVC_ENC                 = 0x01,

    C7_AVC_DEC                  = 0x10,
    C7_VC1_DEC                  = 0x11,
    C7_MP2_DEC                  = 0x12,
    C7_MP4_DEC                  = 0x13,
    C7_DV3_DEC                  = 0x13,
    C7_RVX_DEC                  = 0x14,
    C7_AVS_DEC                  = 0x15,
    C7_VP9_DEC                  = 0x16,
    C7_VP8_DEC                  = 0x17,

    C7_AVC_ENC                  = 0x18,
    C7_MP4_ENC                  = 0x19,

    STD_UNKNOWN                 = 0xFF
};

// AUX_COD_STD
enum {
    MP4_AUX_MPEG4 = 0,
    MP4_AUX_DIVX3 = 1
};

enum {
    VPX_AUX_THO = 0,
    VPX_AUX_VP6 = 1,
    VPX_AUX_VP8 = 2,
    VPX_AUX_NUM
};

enum {
    AVC_AUX_AVC = 0,
    AVC_AUX_MVC = 1
};

// BIT_RUN command
enum {
    DEC_SEQ_INIT = 1,
    ENC_SEQ_INIT = 1,
    DEC_SEQ_END = 2,
    ENC_SEQ_END = 2,
    PIC_RUN = 3,
    SET_FRAME_BUF = 4,
    ENCODE_HEADER = 5,
    ENC_PARA_SET = 6,
    DEC_PARA_SET = 7,
    DEC_BUF_FLUSH = 8,
    RC_CHANGE_PARAMETER    = 9,
    VPU_SLEEP = 10,
    VPU_WAKE = 11,
    ENC_ROI_INIT = 12,
    FIRMWARE_GET = 0xf
};


enum {
    SRC_BUFFER_EMPTY        = 0,    //!< source buffer doesn't allocated.
    SRC_BUFFER_ALLOCATED    = 1,    //!< source buffer has been allocated.
    SRC_BUFFER_USE_ENCODE   = 2     //!< source buffer was sent to VPU. but it was not used for encoding.
};
#define MAX(_a, _b)             (_a >= _b ? _a : _b)

#define HEVC_MAX_SUB_LAYER_ID   6

//#define API_DEBUG
#ifdef API_DEBUG
#ifdef _MSC_VER
#define APIDPRINT(_fmt, ...)            printf(_fmt, __VA_ARGS__)
#else
#define APIDPRINT(_fmt, ...)            printf(_fmt, ##__VA_ARGS__)
#endif
#else
#define APIDPRINT(_fmt, ...)
#endif

/**
 * PRODUCT: CODA960/CODA980/WAVE320
 */
typedef struct {
    union {
        struct {
            int             useBitEnable;
            int             useIpEnable;
            int             useDbkYEnable;
            int             useDbkCEnable;
            int             useOvlEnable;
            int             useBtpEnable;
            int             useMeEnable;        // used only for coda7q encoder
            PhysicalAddress bufBitUse;
            PhysicalAddress bufIpAcDcUse;
            PhysicalAddress bufDbkYUse;
            PhysicalAddress bufDbkCUse;
            PhysicalAddress bufOvlUse;
            PhysicalAddress bufBtpUse;
            PhysicalAddress bufMeUse;           // used only for coda7q encoder
        } coda9;
        struct {
            int             useIpEnable;
            int             useSclEnable;
            int             useSclPackedModeEnable;
            int             useLfRowEnable;
            int             useBitEnable;
            PhysicalAddress bufIp;
            PhysicalAddress bufLfRow;
            PhysicalAddress bufBit;
            PhysicalAddress bufScaler;
            PhysicalAddress bufScalerPackedData;
            int             useEncImdEnable;
            int             useEncRdoEnable;
            int             useEncLfEnable;
            PhysicalAddress bufImd;
            PhysicalAddress bufRdo;
            PhysicalAddress bufLf;
        } wave4;
    } u;
    int             bufSize;
    PhysicalAddress bufBase;
} SecAxiInfo;

#ifdef SUPPORT_AVC_FRAME_RC

#define CODA980_RC_CONST_QUALITY
//#define CODA980_RC_CONST_QUALITY_DEBUG
//#define RC_CONST_QUAL_QSCALE_SIMPLE
#include <float.h>
#include <math.h>

/*<---- Header Part------->*/

#define RC_MODE_CBR         1   // Constant bitrate mode
#define RC_MODE_ABR         2   // Average bitrate mode
#define RC_MODE_CQL         4   // Constant quality mode

#define RC_MIN_QP           12
#define RC_MAX_QP           51

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

//#define MIN min
//#define MAX max

#ifndef log2f
#define log2f(x) (logf(x)/0.693147180559945f)
#endif
#ifndef log2
#define log2(x) (log(x)/0.693147180559945)
#endif
#ifndef isfinite
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(WIN32) || defined(__MINGW32__)
#define isfinite _finite
#else

#define isfinite finite
#endif
#endif

typedef struct  {
    char *token;
    double value;
} rc_stat_t;


typedef struct
{
    double coeff_min;
    double coeff;
    double count;
    double decay;
    double offset;
} predictor_t;


typedef struct
{
    int rc_mode;
    int mb_num_pic;
    int pic_type_p;
    int frame_num;

    int bit_rate_kbps;
    int gop_num;                              // FPS if only first I
    int init_qp_i;

    //#ifdef RC_PIC_BIT
    int init_delay;
    int init_buf_level;
    int prev_pic_bit_cnt;
    //#endif
    int frame_rate;
    int pic_bit_cnt;
    int last_pic_avg_qp;

    unsigned int max_intra;
    int underflow_size;

    int qp_sum;                              // unsigned 16 bit (1620 * 40 < 65535) ==> signed 32 bit
    int avg_qp_sum_p;						  // Not used, should be removed.
    int cur_mb_qp;                           // MB level RC
    int cur_mb_qp_interval;                  // MB level RC
    int div_err_row;
    int div_err_pic;
    int bit_per_mb;                          // g_nBPF/numOfMB
    int bit_per_pic;                         // average bit / frame (signed 20 bit)
    int tar_pic_bit;                         // target picture bit (signed 20 bit)
    int buf_size;                            // encoder Buffer Size
    int buf_level;                           // encoder buffer level

    int field_flag;


    int skip_disable;
    int skip_flag;
    int last_pic_type_p;


#ifdef CODA980_RC_CONST_QUALITY
    predictor_t pred[3];    // For each picture type

    /* constants */
    double qcompress;

    /* current_frame */
    //ratecontrol_entry_t rce;

    /* Q scale management */
    int    init_pic_qp;         /* In order to monitor picture start QP */
    double last_qscale;         /* qscale with average picture qp of previous frame */
    double curr_qscale;         /* estimated qscale for current frame */
    double last_qscale_for[3];  /* last qscale for a specific pict type, used for max_diff & ipb factor stuff */
    double lstep;               /* max change (multiply) in qscale per frame */
    double overflow;            /* just for monitoring */
    double lmin;                /* just for monitoring */
    double lmax;                /* just for monitoring */
    double blurred_complexity;

    /* ABR stuff */
    int    last_satd;           /* scaled complexity of previous frame, (x264 uses current frame complexity of half resolution */
    int    bit_per_pic_norm;    /* scaled BPF of current sequence, normalized bit_per_pic */
    int    last_satd_org;       /* original complexity of previous frame with only P_SLICE */
    int    intra_cost_sum;      /* sum of intra cost with intra_penalty for computing IP_OFFSET */

    double last_rceq;
    double cplxr_sum;           /* sum of bits*qscale/rceq */
    double wanted_bits_window;  /* target bitrate * window */
    double cbr_decay;
    double short_term_cplxsum;
    double short_term_cplxcount;
    double ip_offset;

    int cplx_strength;          /* qscale strength */

    double frame_size_maximum;  /* Maximum frame size due to MinCR */
    double frame_size_planned;

    double last_satd_sft;          /* adjust shift value of last_satd according to image resolution not to overflow while calculating */
    int mb_num_pic_norm;        /* Normalized MB number */
    int last_pic_bits;
    int static_strength;        /* static scene check strength : 1, 2, 3 */

    int disp_debug_info;        /* Enable display debugging info at the end of each function of frame level RC */
#endif
} RC_INFO;
#endif

typedef struct {
    DecOpenParam    openParam;
    DecInitialInfo  initialInfo;
    DecInitialInfo  newSeqInfo;
    PhysicalAddress streamWrPtr;
    PhysicalAddress streamRdPtr;
    Int32           streamBufFull;
    int             streamEndflag;
    int             frameDisplayFlag;
    int             clearDisplayIndexes;
    int             setDisplayIndexes;
    PhysicalAddress streamRdPtrRegAddr;
    PhysicalAddress streamWrPtrRegAddr;
    PhysicalAddress streamBufStartAddr;
    PhysicalAddress streamBufEndAddr;
    PhysicalAddress frameDisplayFlagRegAddr;
    PhysicalAddress currentPC;
    PhysicalAddress busyFlagAddr;
    int             streamBufSize;
    FrameBuffer     frameBufPool[MAX_REG_FRAME];
    vpu_buffer_t    vbFrame;
    vpu_buffer_t    vbWTL;
    vpu_buffer_t    vbPPU;

    vpu_buffer_t    vbMV;
    vpu_buffer_t    vbFbcYTbl;
    vpu_buffer_t    vbFbcCTbl;

    //RTK
    vpu_buffer_t    extVbFbcYTbls[MAX_GDI_IDX];
    vpu_buffer_t    extVbFbcCTbls[MAX_GDI_IDX];
    int             fbcTblAllocExt;

    int             frameAllocExt;
    int             ppuAllocExt;
    int             numFrameBuffers;
    int             numFbsForDecoding;                  /*!<< number of framebuffers used in decoding */
    int             numFbsForWTL;                       /*!<< number of linear framebuffer for displaying when DecInfo::wtlEnable is set to 1 */
    int             stride;
    int             frameBufferHeight;
    int             rotationEnable;
    int             mirrorEnable;
    int             deringEnable;
    MirrorDirection mirrorDirection;
    int             rotationAngle;
    FrameBuffer     rotatorOutput;
    int             rotatorStride;
    int             rotatorOutputValid;
    int             initialInfoObtained;
    int             vc1BframeDisplayValid;
    int             mapType;
    int             tiled2LinearEnable;
    int             tiled2LinearMode;
    int             wtlEnable;
    int             wtlMode;
    FrameBufferFormat   wtlFormat;                      /*!<< default value: FORMAT_420 8bit */
    SecAxiInfo          secAxiInfo;
    MaverickCacheConfig cacheConfig;
    int chunkSize;
    int seqInitEscape;

    // Report Information
    PhysicalAddress userDataBufAddr;
    vpu_buffer_t    vbUserData;
    BOOL            userDataEnable;                    /* User Data Enable Flag
                                                          CODA9xx: TRUE or FALSE
                                                          WAVE4xx: Refer to H265_USERDATA_FLAG_xxx values in vpuapi.h */
    int             userDataBufSize;
    int             userDataReportMode;                // User Data report mode (0 : interrupt mode, 1 interrupt disable mode)

    BOOL            cuDataEnable;                       //!<< Enable reporting CU data
    int32_t         cuDataBufSize;                      //!<< Size of CU buffer in bytes
    PhysicalAddress cuDataBufAddr;                      //!<< Physical address


    LowDelayInfo    lowDelayInfo;
    int             frameStartPos;
    int             frameEndPos;
    int             frameDelay;
    vpu_buffer_t    vbSlice;            // AVC, VP8 only
    vpu_buffer_t    vbWork;
    vpu_buffer_t    vbTemp;
    vpu_buffer_t    vbReport;
    int             workBufferAllocExt;
    DecOutputInfo   decOutInfo[MAX_GDI_IDX];
    TiledMapConfig  mapCfg;
    int             reorderEnable;
    DRAMConfig      dramCfg;            //coda960 only
    int             thumbnailMode;
    int             seqChangeMask;      // WAVE410
    Uint32          prevFrameEndPos;      //!<< WAVE410v2: end position of previous frame
    BOOL            scalerEnable;
    Uint32          scaleWidth;
    Uint32          scaleHeight;
    Int32           targetSubLayerId;       //!<< H.265 temporal scalability

#ifdef SUPPORT_AFBCE
    BOOL            enableAfbce;
#endif
    BOOL            cancelPrescan;

    Uint32          div3Width;      // for CODA7Q
    Uint32          div3Height;     // for CODA7Q
} DecInfo;

typedef struct {
    EncOpenParam        openParam;
    EncInitialInfo      initialInfo;
    PhysicalAddress     streamRdPtr;
    PhysicalAddress     streamWrPtr;
    int                 streamEndflag;
    PhysicalAddress     streamRdPtrRegAddr;
    PhysicalAddress     streamWrPtrRegAddr;
    PhysicalAddress     streamBufStartAddr;
    PhysicalAddress     streamBufEndAddr;
    PhysicalAddress     currentPC;
    PhysicalAddress     busyFlagAddr;
    int                 streamBufSize;
    int                 linear2TiledEnable;
    int                 linear2TiledMode;    // coda980 only
    int                 mapType;
    int                 userMapEnable;
    FrameBuffer         frameBufPool[MAX_REG_FRAME];
    vpu_buffer_t        vbFrame;
    vpu_buffer_t        vbPPU;
    int                 frameAllocExt;
    int                 ppuAllocExt;
    vpu_buffer_t        vbSubSampFrame;         /*!<< CODA960 only */
    vpu_buffer_t        vbMvcSubSampFrame;      /*!<< CODA960 only */
    int                 numFrameBuffers;
    int                 stride;
    int                 frameBufferHeight;
    int                 rotationEnable;
    int                 mirrorEnable;
    MirrorDirection     mirrorDirection;
    int                 rotationAngle;
    int                 initialInfoObtained;
    int                 ringBufferEnable;
    SecAxiInfo          secAxiInfo;
    MaverickCacheConfig cacheConfig;
    EncSubFrameSyncConfig subFrameSyncConfig;   /*!<< CODA980 & WAVE320 */

    int                 sliceIntEnable;       /*!<< WAVE420 only */


    int                 ActivePPSIdx;           /*!<< CODA980 */
    int                 frameIdx;               /*!<< CODA980 */
    int                 fieldDone;              /*!<< CODA980 */
    int                 lineBufIntEn;
    vpu_buffer_t        vbWork;
    vpu_buffer_t        vbScratch;

    vpu_buffer_t        vbTemp;                 //!< Temp buffer (WAVE420)
    vpu_buffer_t        vbMV;                   //!< colMV buffer (WAVE420)
    vpu_buffer_t        vbFbcYTbl;              //!< FBC Luma table buffer (WAVE420)
    vpu_buffer_t        vbFbcCTbl;              //!< FBC Chroma table buffer (WAVE420)
    vpu_buffer_t        vbSubSamBuf;            //!< Sub-sampled buffer for ME (WAVE420)

    TiledMapConfig      mapCfg;
    DRAMConfig          dramCfg;                /*!<< CODA960 */

#ifdef SUPPORT_AVC_FRAME_RC
    RC_INFO rc_info;
    int slice_type;
#endif
    // user data sei encoding for HEVC
    int userDataEnable;
    int userDataSize;
    int userDataEncOrder;
    int userDataSuffixFlag;
    PhysicalAddress userDataAddr;
} EncInfo;



typedef struct CodecInst {
    Int32   inUse;
    Int32   instIndex;
    Int32   coreIdx;
    Int32   codecMode;
    Int32   codecModeAux;
    Int32   productId;
    Int32   loggingEnable;
    Uint32  isDecoder;
    //ENABLE_TEE_DRM_FLOW //For RTK DRM flow
    Uint32 isUseProtectBuffer;
    void *ta_handle;

    union {
        EncInfo encInfo;
        DecInfo decInfo;
    } CodecInfo;
} CodecInst;


/*******************************************************************************
 * H.265 USER DATA(SEI)                                                        *
 *******************************************************************************/
#define H265_MAX_DPB_SIZE        17
#define H265_MAX_NUM_SUB_LAYER   8
#define H265_MAX_NUM_ST_RPS      64
#define H265_MAX_CPB_CNT         32
#define H265_MAX_LUT_NUM_VAL 3
#define H265_MAX_LUT_NUM_VAL_MINUS1 33
#define H265_MAX_COLOUR_REMAP_COEFFS 3

#define H265_MAX_NUM_VERTICAL_FILTERS   5
#define H265_MAX_NUM_HORIZONTAL_FILTERS 3
#define H265_MAX_TAP_LENGTH             32
#define H265_MAX_NUM_KNEE_POINT         999

typedef struct
{
    uint32_t offset;
    uint32_t size;
} user_data_entry_t;

typedef struct
{
    int16_t left;
    int16_t right;
    int16_t top;
    int16_t bottom;
} win_t;


typedef struct  
{
    uint8_t     nal_hrd_param_present_flag;
    uint8_t     vcl_hrd_param_present_flag;
    uint8_t     sub_pic_hrd_params_present_flag;
    uint8_t  tick_divisor_minus2;
    int8_t   du_cpb_removal_delay_inc_length_minus1;
    int8_t   sub_pic_cpb_params_in_pic_timing_sei_flag;
    int8_t   dpb_output_delay_du_length_minus1;
    int8_t   bit_rate_scale;
    int8_t   cpb_size_scale;
    int8_t   initial_cpb_removal_delay_length_minus1;
    int8_t   cpb_removal_delay_length_minus1;
    int8_t   dpb_output_delay_length_minus1;

    uint8_t     fixed_pic_rate_gen_flag[H265_MAX_NUM_SUB_LAYER];
    uint8_t     fixed_pic_rate_within_cvs_flag[H265_MAX_NUM_SUB_LAYER];
    uint8_t     low_delay_hrd_flag[H265_MAX_NUM_SUB_LAYER];
    int8_t   cpb_cnt_minus1[H265_MAX_NUM_SUB_LAYER];
    int16_t  elemental_duration_in_tc_minus1[H265_MAX_NUM_SUB_LAYER];

    uint32_t nal_bit_rate_value_minus1[H265_MAX_NUM_SUB_LAYER][H265_MAX_CPB_CNT];
    uint32_t nal_cpb_size_value_minus1[H265_MAX_NUM_SUB_LAYER][H265_MAX_CPB_CNT];
    uint32_t nal_cpb_size_du_value_minus1[H265_MAX_NUM_SUB_LAYER];
    uint32_t nal_bit_rate_du_value_minus1[H265_MAX_NUM_SUB_LAYER];
    uint8_t     nal_cbr_flag[H265_MAX_NUM_SUB_LAYER][H265_MAX_CPB_CNT];

    uint32_t vcl_bit_rate_value_minus1[H265_MAX_NUM_SUB_LAYER][H265_MAX_CPB_CNT];
    uint32_t vcl_cpb_size_value_minus1[H265_MAX_NUM_SUB_LAYER][H265_MAX_CPB_CNT];
    uint32_t vcl_cpb_size_du_value_minus1[H265_MAX_NUM_SUB_LAYER];
    uint32_t vcl_bit_rate_du_value_minus1[H265_MAX_NUM_SUB_LAYER];
    uint8_t     vcl_cbr_flag[H265_MAX_NUM_SUB_LAYER][H265_MAX_CPB_CNT];
} h265_hrd_param_t;

typedef struct
{
    uint8_t    aspect_ratio_info_present_flag;
    uint8_t    aspect_ratio_idc;
    uint8_t    overscan_info_present_flag;
    uint8_t    overscan_appropriate_flag;

    uint8_t    video_signal_type_present_flag;
    int8_t     video_format;

    uint8_t    video_full_range_flag;
    uint8_t    colour_description_present_flag;

    uint16_t sar_width;
    uint16_t sar_height;

    uint8_t colour_primaries;
    uint8_t transfer_characteristics;
    uint8_t matrix_coefficients;

    uint8_t    chroma_loc_info_present_flag;
    int8_t  chroma_sample_loc_type_top_field;
    int8_t  chroma_sample_loc_type_bottom_field;

    uint8_t    neutral_chroma_indication_flag;

    uint8_t    field_seq_flag;

    uint8_t    frame_field_info_present_flag;
    uint8_t    default_display_window_flag;
    uint8_t    vui_timing_info_present_flag;
    uint8_t    vui_poc_proportional_to_timing_flag;

    uint32_t vui_num_units_in_tick;
    uint32_t vui_time_scale;

    uint8_t    vui_hrd_parameters_present_flag;
    uint8_t    bitstream_restriction_flag;

    uint8_t    tiles_fixed_structure_flag;
    uint8_t    motion_vectors_over_pic_boundaries_flag;
    uint8_t    restricted_ref_pic_lists_flag;
    int8_t  min_spatial_segmentation_idc;
    int8_t  max_bytes_per_pic_denom;
    int8_t  max_bits_per_mincu_denom;

    int16_t vui_num_ticks_poc_diff_one_minus1;
    int8_t  log2_max_mv_length_horizontal;
    int8_t  log2_max_mv_length_vertical;

    win_t   def_disp_win;
    h265_hrd_param_t hrd_param;
} h265_vui_param_t;

typedef struct
{
    uint32_t display_primaries_x[3];
    uint32_t display_primaries_y[3];
    uint32_t white_point_x                   : 16;
    uint32_t white_point_y                   : 16;
    uint32_t max_display_mastering_luminance : 32;
    uint32_t min_display_mastering_luminance : 32;
} h265_mastering_display_colour_volume_t;

typedef struct
{
    Uint32   ver_chroma_filter_idc               : 8;
    Uint32   hor_chroma_filter_idc               : 8;
    Uint32   ver_filtering_field_processing_flag : 1;
    Uint32   target_format_idc                   : 2;
    Uint32   num_vertical_filters                : 3;
    Uint32   num_horizontal_filters              : 3;
    Uint8    ver_tap_length_minus1[ H265_MAX_NUM_VERTICAL_FILTERS ];
    Uint8    hor_tap_length_minus1[ H265_MAX_NUM_HORIZONTAL_FILTERS ];
    Int32    ver_filter_coeff[ H265_MAX_NUM_VERTICAL_FILTERS ][ H265_MAX_TAP_LENGTH ];
    Int32    hor_filter_coeff[ H265_MAX_NUM_HORIZONTAL_FILTERS ][ H265_MAX_TAP_LENGTH ];
} h265_chroma_resampling_filter_hint_t;

typedef struct
{
    Uint32   knee_function_id;
    Uint8    knee_function_cancel_flag;

    Uint8    knee_function_persistence_flag;
    Uint32   input_disp_luminance;
    Uint32   input_d_range;
    Uint32   output_d_range;
    Uint32   output_disp_luminance;
    Uint16   num_knee_points_minus1;
    Uint16   input_knee_point[ H265_MAX_NUM_KNEE_POINT ];
    Uint16   output_knee_point[ H265_MAX_NUM_KNEE_POINT ];
} h265_knee_function_info_t;

typedef struct
{
    Uint16 max_content_light_level;
    Uint16 max_pic_average_light_level;
} h265_content_light_level_info_t;

typedef struct
{
    uint32_t colour_remap_id;
    uint8_t  colour_remap_cancel_flag;
    uint8_t  colour_remap_persistence_flag;
    uint8_t  colour_remap_video_signal_info_present_flag;
    uint8_t  colour_remap_full_range_flag;

    uint8_t colour_remap_primaries;
    uint8_t colour_remap_transfer_function;
    uint8_t colour_remap_matrix_coefficients;
    uint8_t colour_remap_input_bit_depth;

    uint8_t  colour_remap_bit_depth;
    uint8_t  pre_lut_num_val_minus1[H265_MAX_LUT_NUM_VAL];
    uint16_t pre_lut_coded_value[H265_MAX_LUT_NUM_VAL][H265_MAX_LUT_NUM_VAL_MINUS1];
    uint16_t pre_lut_target_value[H265_MAX_LUT_NUM_VAL][H265_MAX_LUT_NUM_VAL_MINUS1];

    uint8_t  colour_remap_matrix_present_flag;
    uint8_t  log2_matrix_denom;
    uint16_t  colour_remap_coeffs[H265_MAX_COLOUR_REMAP_COEFFS][H265_MAX_COLOUR_REMAP_COEFFS];

    uint8_t  post_lut_num_val_minus1[H265_MAX_LUT_NUM_VAL];
    uint16_t post_lut_coded_value[H265_MAX_LUT_NUM_VAL][H265_MAX_LUT_NUM_VAL_MINUS1];
    uint16_t post_lut_target_value[H265_MAX_LUT_NUM_VAL][H265_MAX_LUT_NUM_VAL_MINUS1];
} h265_colour_remapping_info_t;

#ifdef __cplusplus
extern "C" {
#endif


    RetCode InitCodecInstancePool(Uint32 coreIdx);
    RetCode GetCodecInstance(Uint32 coreIdx, CodecInst ** ppInst);
    void    FreeCodecInstance(CodecInst * pCodecInst);

    RetCode CheckDecOpenParam(DecOpenParam * pop);
    int     DecBitstreamBufEmpty(DecInfo * pDecInfo);
#ifdef DRAM_TEST
    RetCode ExecDRAMReadWriteTest(Uint32 coreIdx, Uint32* dram_source_addr, Uint32* dram_destination_addr, Uint32* dram_data_size);
#endif
    RetCode SetParaSet(DecHandle handle, int paraSetType, DecParamSet * para);
    void    DecSetHostParaAddr(Uint32 coreIdx, PhysicalAddress baseAddr, PhysicalAddress paraAddr);


    Int32 ConfigSecAXICoda9(
        Uint32      coreIdx,
        CodStd      codStd,
        SecAxiInfo* sa,
        Uint32      width,
        Uint32      height,
        Uint32      profile
    );

    Int32 ConfigSecAXIWave4(
        Uint32      coreIdx,
        CodStd      codStd,
        SecAxiInfo* sa,
        Uint32      width,
        Uint32      height,
        Uint32      profile,
        Uint32      level
    );

    Int32 ConfigSecAXICoda7(
        Uint32      coreIdx,
        CodStd      codStd,
        SecAxiInfo* sa,
        Uint32      width,
        Uint32      height,
        Uint32      profile
    );

    RetCode AllocateLinearFrameBuffer(
        TiledMapType            mapType,
        FrameBuffer*            fbArr,
        uint32_t                numOfFrameBuffers,
        uint32_t                sizeLuma,
        uint32_t                sizeChroma
    );

    RetCode AllocateTiledFrameBufferGdiV1(
        TiledMapType            mapType,
        PhysicalAddress         tiledBaseAddr,
        FrameBuffer*            fbArr,
        uint32_t                numOfFrameBuffers,
        uint32_t                sizeLuma,
        uint32_t                sizeChroma,
        DRAMConfig*             pDramCfg
    );

    RetCode AllocateTiledFrameBufferGdiV2(
        TiledMapType            mapType,
        FrameBuffer*            fbArr,
        uint32_t                numOfFrameBuffers,
        uint32_t                sizeLuma,
        uint32_t                sizeChroma
    );


    RetCode CheckEncInstanceValidity(EncHandle handle);
    RetCode CheckEncOpenParam(EncOpenParam * pop);
    RetCode CheckEncParam(EncHandle handle, EncParam * param);
    RetCode GetEncHeader(EncHandle handle, EncHeaderParam * encHeaderParam);
    RetCode EncParaSet(EncHandle handle, int paraSetType);
    RetCode SetGopNumber(EncHandle handle, Uint32 *gopNumber);
    RetCode SetIntraQp(EncHandle handle, Uint32 *intraQp);
    RetCode SetBitrate(EncHandle handle, Uint32 *bitrate);
    RetCode SetFramerate(EncHandle handle, Uint32 *frameRate);
    RetCode SetIntraRefreshNum(EncHandle handle, Uint32 *pIntraRefreshNum);
    RetCode SetSliceMode(EncHandle handle, EncSliceMode *pSliceMode);
    RetCode SetHecMode(EncHandle handle, int mode);
    void    EncSetHostParaAddr(Uint32 coreIdx, PhysicalAddress baseAddr, PhysicalAddress paraAddr);

    RetCode EnterLock(Uint32 coreIdx);
    RetCode LeaveLock(Uint32 coreIdx);
    RetCode SetClockGate(Uint32 coreIdx, Uint32 on);

    RetCode EnterDispFlagLock(Uint32 coreIdx);
    RetCode LeaveDispFlagLock(Uint32 coreIdx);

    void SetPendingInst(Uint32 coreIdx, CodecInst *inst);
    void ClearPendingInst(Uint32 coreIdx);
    CodecInst *GetPendingInst(Uint32 coreIdx);
    int GetPendingInstIdx(Uint32 coreIdx);

    Int32 MaverickCache2Config(
        MaverickCacheConfig* pCache,
        BOOL                decoder ,
        BOOL                interleave,
        uint32_t            bypass,
        uint32_t            burst,
        uint32_t            merge,
        TiledMapType        mapType,
        uint32_t            wayshape
    );

    int SetTiledMapType(Uint32 coreIdx, TiledMapConfig *pMapCfg, int mapType, int stride, int interleave, DRAMConfig *dramCfg);
    int GetXY2AXIAddr(TiledMapConfig *pMapCfg, int ycbcr, int posY, int posX, int stride, FrameBuffer *fb);
    Uint32 FIORead(Uint32 coreIdx, Uint32 addr);
    Int32 FIOWrite(Uint32 coreIdx, Uint32 addr, Uint32 data);
//for GDI 1.0
    void SetTiledFrameBase(Uint32 coreIdx, PhysicalAddress baseAddr);
    PhysicalAddress GetTiledFrameBase(Uint32 coreIdx, FrameBuffer *frame, int num);

    /**
     * \brief   It returns the stride of framebuffer in byte.
     *
     * \param   width           picture width in pixel.
     * \param   format          YUV format. see FrameBufferFormat structure in vpuapi.h
     * \param   cbcrInterleave
     * \param   mapType         map type. see TiledMapType in vpuapi.h
     */
    Int32 CalcStride(
        Uint32              width,
        Uint32              height,
        FrameBufferFormat   format,
        BOOL                cbcrInterleave,
        TiledMapType        mapType,
        BOOL                isVP9
    );

    Int32 CalcLumaSize(
        Int32               productId,
        Int32               stride,
        Int32               height,
        FrameBufferFormat   format,
        BOOL                cbcrIntl,
        TiledMapType        mapType,
        DRAMConfig*         pDramCfg
    );

    Int32 CalcChromaSize(
        Int32               productId,
        Int32               stride,
        Int32               height,
        FrameBufferFormat   format,
        BOOL                cbcrIntl,
        TiledMapType        mapType,
        DRAMConfig*         pDramCfg
    );

    int LevelCalculation(int MbNumX, int MbNumY, int frameRateInfo, int interlaceFlag, int BitRate, int SliceNum);

#ifdef SUPPORT_AVC_FRAME_RC
    void SetRcParam (EncOpenParam * pEncOp, EncInfo *pEncInfo);
    void SetRcChangeGopNum (int NewGop, RC_INFO *rc);
    void SetRcChangeBitRate (int BitRate, RC_INFO *rc);
    void SetRcChangeFrameRate (int FrameRate, RC_INFO *rc);
    int rc_const_quality_pic_init (int core_idx, int pic_type, RC_INFO *rc);
    int rc_const_quality_pic_end (int core_idx, RC_INFO *rc);
#endif
#ifdef __cplusplus
}
#endif

#endif // endif VPUAPI_UTIL_H_INCLUDED

