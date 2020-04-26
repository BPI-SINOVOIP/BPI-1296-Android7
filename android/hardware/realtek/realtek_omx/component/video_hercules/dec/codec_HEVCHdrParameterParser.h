#include <port.h>
#include <OSAL_RTK.h>
#include <string.h> // memset, memcpy
#include "util.h"
#include "dbgtrace.h"
#include <InbandAPI.h>
#include <sys/mman.h>

//RTK VE1 header file
#include "vpuconfig.h"
#include "vpuapi.h"
#include "vpuapifunc.h"
#include "coda9/coda9_regdefine.h"
#include "coda9/coda9_vpuconfig.h"
#include "wave/wave5_regdefine.h"
#include "vpuerror.h"


struct Ts103433Info
{
    int ts103433_spec_version;
    int ts103433_payload_mode;
};

struct ColourVolumeReconstructionInfo
{
    int cv_rec_id;
    OMX_BOOL cv_rec_cancel_flag;
    OMX_BOOL cv_rec_persistence_flag;
    OMX_BOOL cv_rec_target_info_flag;

    int cv_rec_primaries;
    int cv_rec_matrix_coefficients;

    int tone_mapping_input_signal_black_level_offset;
    int tone_mapping_input_signal_white_level_offset;

    int shadow_gain_control;
    int highlight_gain_control;
    int mid_tone_width_adjustment_factor;
    int tone_mapping_output_fine_tuning_num_val;
    int tone_mapping_output_fine_tuning_x[10];
    int tone_mapping_output_fine_tuning_y[10];

    int saturation_gain_num_val;
    int saturation_gain_x[6];
    int saturation_gain_y[6];
};

typedef struct HdrParameterParser
{
    DecHandle handle;
    CodStd bitstreamFormat;
    int coreIdx;

    h265_mastering_display_colour_volume_t m_mastering;
    h265_vui_param_t m_vui;
    h265_colour_remapping_info_t m_remapping;

    struct Ts103433Info m_ts103433_info;
    struct ColourVolumeReconstructionInfo m_cvri;

    vpu_buffer_t m_vpu_buffer;
    Uint8 *mp_work_buffer;

    int m_query_type;
    unsigned int m_user_data_header;
    unsigned int m_t35_header;

    struct tch_metadata *mp_technicolor_buffer;
} HdrParameterParser;


    void HdrAllocate(HdrParameterParser *pHdr, DecHandle handle, CodStd bitstreamFormat, int coreIdx);
    void HdrRelease(HdrParameterParser *pHdr);

    void HdrParseHeaderPicRun(HdrParameterParser *pHdr, DecOutputInfo outputInfo);
    void HdrParseHeaderSeqInit(HdrParameterParser *pHdr, DecInitialInfo initialInfo);

    void HdrInformSequenceChange(HdrParameterParser *pHdr);

    void HdrGetMasteringDisplay(HdrParameterParser *pHdr, private_rtk_v1_data *p_state);
    void HdrGetVui(HdrParameterParser *pHdr, private_rtk_v1_data *p_state);

    void HdrGetLumaChromaOffsetTableAddr(HdrParameterParser *pHdr, int myIndex, int width, int height, unsigned int *pLumaOffTblAddr, unsigned int *pChromeOffTblAddr);