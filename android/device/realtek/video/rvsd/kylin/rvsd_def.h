#ifndef RVSD_DEF_H
#define RVSD_DEF_H


////////////
// Define //
////////////
#ifndef ANDROID /* Fuchun 20141112 Fix later for multiple instance */
#define USE_SINGLE_THREAD 1	//don't remove it
#endif
#define DROP_ALL_AUDIO_INBAND 1 //Robert comment: default is disabled

#define AUDIO_DECODE_ACODEC 1

#define RVSD_SOCKET_BUFFER_SIZE 16384

#define PTS_UNIT 90000  // 90000 pts/sec

// RM
#define RVSD_RM_CALCULATE_PTS 1 // Reference 1186, re-calculate PTS and deliver to VO.
#define RVSD_RM_ENABLE 0    // RVSD stuffing header of RM, RVSD_WRITE_HEADER_ENABLE(rvsd_VDecRtk.h) also need defined.
#define AP_RM_ENABLE 1      // AP stuffing header of RM

//MJPEG
#if !defined(__LINARO_SDK__)
#ifdef RVSD_USE_JPU_API
#define RVSD_JDEC_ENABLE
#endif
#endif

#define MVC_USERDATA_BUF_SIZE   2048
/* SEI payload types */
typedef enum {
  SEI_BUFFERING_PERIOD = 0,
  SEI_PIC_TIMING,
  SEI_PAN_SCAN_RECT,
  SEI_FILLER_PAYLOAD,
  SEI_USER_DATA_REGISTERED_ITU_T_T35,
  SEI_USER_DATA_UNREGISTERED,
  SEI_RECOVERY_POINT,
  SEI_DEC_REF_PIC_MARKING_REPETITION,
  SEI_SPARE_PIC,
  SEI_SCENE_INFO,
  SEI_SUB_SEQ_INFO,
  SEI_SUB_SEQ_LAYER_CHARACTERISTICS,
  SEI_SUB_SEQ_CHARACTERISTICS,
  SEI_FULL_FRAME_FREEZE,
  SEI_FULL_FRAME_FREEZE_RELEASE,
  SEI_FULL_FRAME_SNAPSHOT,
  SEI_PROGRESSIVE_REFINEMENT_SEGMENT_START,
  SEI_PROGRESSIVE_REFINEMENT_SEGMENT_END,
  SEI_MOTION_CONSTRAINED_SLICE_GROUP_SET,
  SEI_FILM_GRAIN_CHARACTERISTICS,
  SEI_DEBLOCKING_FILTER_DISPLAY_PREFERENCE,
  SEI_STEREO_VIDEO_INFO,
  SEI_POST_FILTER_HINTS,
  SEI_TONE_MAPPING,
  SEI_SCALABILITY_INFO,
  SEI_SUB_PIC_SCALABLE_LAYER,
  SEI_NON_REQUIRED_LAYER_REP,
  SEI_PRIORITY_LAYER_INFO,
  SEI_LAYERS_NOT_PRESENT,
  SEI_LAYER_DEPENDENCY_CHANGE,
  SEI_SCALABLE_NESTING,
  SEI_BASE_LAYER_TEMPORAL_HRD,
  SEI_QUALITY_LAYER_INTEGRITY_CHECK,
  SEI_REDUNDANT_PIC_PROPERTY,
  SEI_TL0_DEP_REP_INDEX,
  SEI_TL_SWITCHING_POINT,
  SEI_PARALLEL_DECODING_INFO,
  SEI_MVC_SCALABLE_NESTING,
  SEI_VIEW_SCALABILITY_INFO,
  SEI_MULTIVIEW_SCENE_INFO,
  SEI_MULTIVIEW_ACQUISITION_INFO,
  SEI_NON_REQUIRED_VIEW_COMPONENT,
  SEI_VIEW_DEPENDENCY_CHANGE,
  SEI_OPERATION_POINTS_NOT_PRESENT,
  SEI_BASE_VIEW_TEMPORAL_HRD,
  SEI_FRAME_PACKING_ARRANGEMENT,

  SEI_MAX_ELEMENTS  //!< number of maximum syntax elements
} SEI_type;

///////////
// Macro //
///////////
#define RVSD_ENDIAN_CHANGE(x)    ((((x)&0xff000000)>>24)|(((x)&0x00ff0000)>>8)|(((x)&0x0000ff00)<<8)|(((x)&0x000000ff)<<24))
#define RVSD_ENDIAN_CHANGE_16(x) ((((x)&0xff00)>>8) | (((x)&0x00ff)<<8))
#define UNUSED(x) (void)(x)
/* Coda980 */
#define USE_BIT_INTERNAL_BUF        1
#define USE_IP_INTERNAL_BUF         1
#define USE_DBKY_INTERNAL_BUF       1
#define USE_DBKC_INTERNAL_BUF       1
#define USE_OVL_INTERNAL_BUF        1
#define USE_BTP_INTERNAL_BUF        1
#define USE_ME_INTERNAL_BUF         1
/* WAVE410 only */
#define USE_BPU_INTERNAL_BUF        1
#define USE_VCE_IP_INTERNAL_BUF     1
#define USE_VCE_LF_ROW_INTERNAL_BUF 1

namespace android {

typedef enum {
    ENUM_RESET_STATE = 0,
    ENUM_RUN_STATE,
    ENUM_FINISH_STATE,
    ENUM_SHUTDOWN_STATE
} ENUM_SERVICE_STATUS;

typedef enum {
    ENUM_VIDEO_RESET = 0,
    ENUM_VIDEO_WAIT_TO_INIT,
    ENUM_VIDEO_READY_TO_RUN,
    ENUM_VIDEO_RUN,
    ENUM_VIDEO_RUN_DEC,
    ENUM_VIDEO_READY_TO_UNINIT,
    ENUM_VIDEO_WAIT_TO_UNINIT,
    ENUM_VIDEO_UNINIT_DONE,
    ENUM_VIDEO_FATAL_ERR
} ENUM_VIDEO_DEC_STATUS;

}   // namespace android {

struct DecodeStats
{
    unsigned int frameRate;
    unsigned int decodedFrames;
    unsigned int lostFrames;
    unsigned int concealedFrames;
    unsigned int IDecodedFrames;
    unsigned int ILostFrames;
    unsigned int IConcealedFrames;
    unsigned int PDecodedFrames;
    unsigned int PLostFrames;
    unsigned int PConcealedFrames;
    unsigned int BDecodedFrames;
    unsigned int BLostFrames;
    unsigned int BConcealedFrames;
};

#endif

