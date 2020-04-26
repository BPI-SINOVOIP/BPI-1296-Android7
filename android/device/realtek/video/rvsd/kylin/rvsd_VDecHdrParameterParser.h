
#include "rvsd_maincore.h"
#include "rvsd_VDecVPU.h"
#include "rvsd_dbg.h"
#include "rvsd_BitReader.h"
#include "rvsd_ion.h"

#undef LOG_TAG
#define LOG_TAG "HdrParameterParser"

#define TECHNICOLOR_1_4 1
#define SAVE_TECHNICOLOR_FILE 0

enum TECHNICOLOR_VERSION
{
    TECHNICOLOR_V1_3,
    TECHNICOLOR_V1_4
};

struct Ts103433Info
{
    int ts103433_spec_version;
    int ts103433_payload_mode;
};

struct ColourVolumeReconstructionInfo
{
    int cv_rec_id;
    bool cv_rec_cancel_flag;
    bool cv_rec_persistence_flag;
    bool cv_rec_target_info_flag;

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

#if defined(TECHNICOLOR_1_4) && defined(RVSD_SUPPORT_TECHNICOLOR)
struct sl_hdr_metadata_variables {
  int tmInputSignalBlackLevelOffset;
  int tmInputSignalWhiteLevelOffset;
  int shadowGain;
  int highlightGain;
  int midToneWidthAdjFactor;
  int tmOutputFineTuningNumVal;
  int tmOutputFineTuningX[10];
  int tmOutputFineTuningY[10];
  int saturationGainNumVal;
  int saturationGainX[6];
  int saturationGainY[6];
};

struct sl_hdr_metadata_tables {
  int luminanceMappingNumVal;
  int luminanceMappingX[65];
  int luminanceMappingY[65];
  int colourCorrectionNumVal;
  int colourCorrectionX[65];
  int colourCorrectionY[65];
};

struct sl_hdr_metadata {
  int partID;
  int majorSpecVersionID;
  int minorSpecVersionID;
  int payloadMode;
  int hdrPicColourSpace;
  int hdrDisplayColourSpace;
  int hdrDisplayMaxLuminance;
  int hdrDisplayMinLuminance;
  int sdrPicColourSpace;
  int sdrDisplayColourSpace;
  int sdrDisplayMaxLuminance;
  int sdrDisplayMinLuminance;
  int matrixCoefficient[4];
  int chromaToLumaInjection[2];
  int kCoefficient[3];
  union
  {
    struct sl_hdr_metadata_variables variables;
    struct sl_hdr_metadata_tables tables;
  } u;
};

class SignalReconstructParser
{
public:
    SignalReconstructParser();
    ~SignalReconstructParser();

    void Parse(BitReader &reader);
    void Print() const;
    void Get(sl_hdr_metadata &target) const;

private:
    void ParseMdcv(BitReader &reader);
    void ParseCvri(BitReader &reader);
    void ParseCri(BitReader &reader);

    sl_hdr_metadata m_data;
};
#endif

class HdrParameterParser
{
public:
    HdrParameterParser();
    ~HdrParameterParser();

    void Allocate(_DecConfigParam *p_vpu);
    void Release();

    void ParseHeaderPicRun();
    void ParseHeaderSeqInit();

    void InformSequenceChange();

    void GetHdr(YUV_STATE *p_state) const;

    void GetMasteringDisplay(YUV_STATE *p_state) const;
    void GetVui(YUV_STATE *p_state) const;

#if defined(RVSD_SUPPORT_TECHNICOLOR)
    void GetTechniColor(YUV_STATE *p_state, int display_index);
#endif

private:
    void Reset();

    bool IsH264(_DecConfigParam *p_vpu) const;
    bool IsHevc(_DecConfigParam *p_vpu) const;

    bool IsTechniColorDataExist() const;
    void Parse(Uint32 header);

    void ParseT35UserData(unsigned char *p_payload, int payload_size);
    void ParseH264Vui(const AvcVuiInfo &vui);

    void PrintVui() const;

#if defined(RVSD_SUPPORT_TECHNICOLOR)
    void SaveTechnicolorToBuffer(sl_hdr_metadata &data) const;
#endif

    static const int m_user_data_core = 1;
    static const size_t m_user_data_size = 512 * 1024;

    h265_mastering_display_colour_volume_t m_mastering;
    h265_vui_param_t m_vui;
    h265_colour_remapping_info_t m_remapping;
    h265_content_light_level_info_t m_light_level;

    Ts103433Info m_ts103433_info;
    ColourVolumeReconstructionInfo m_cvri;

    vpu_buffer_t m_vpu_buffer;
    Uint8 *mp_work_buffer;

    _DecConfigParam *mp_vpu;

    int m_query_type;

    unsigned int m_user_data_header;
    unsigned int m_t35_header;

    RvsdIonAllocator m_tch_buffer;
    RvsdIonAllocator m_hdr_buffer;

    bool m_is_endian_converted[MAX_REG_FRAME];

    TECHNICOLOR_VERSION m_version;

#if defined(RVSD_SUPPORT_TECHNICOLOR)
    SignalReconstructParser m_signal_reconstruct;
#endif

};

