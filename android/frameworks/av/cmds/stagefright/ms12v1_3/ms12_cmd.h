#ifndef MS12_CMD_H_
#define MS12_CMD_H_

#include "ms12_getparam.h"
#include "ms12_err.h"
//#include "dlb_intrinsics.h"
#include <media/openmax/OMX_Audio.h>

#define MS_PROGRAM_UUID_LEN_BYTES  16

#ifdef MS12_7_1_SUPPORT
#define MS_MAX_AC_MOD 21
#else
#define MS_MAX_AC_MOD  7
#endif

#define MS_DAP_PARAM_NOT_SET      (-1)

/**{ */
#define MSF_OK              0   /**< Operation was successfull. */
#define MSF_NEED_INPUT      1   /**< A node needs more input data. */
#define MSF_NEED_OUTPUT     2   /**< A node needs more output buffer space. */
#define MSF_END_OF_STREAM   4   /**< Indicates that the node has ended processing. */
#define MSF_FRAME_COMPLETE  8   /**< Indicates that a frame is complete */
#define MSF_ERROR          -1   /**< General error occured. */
#define MSF_FATAL_ERROR    -2   /**< An fatal error occured. Stop processing */
/**} */

#define PU_MAX_NUM_NODES     3
#define PU_MAX_NUM_INTVALS 202

#define MS_MAIN_DECODER_IDX         0
#define MS_ASSOC_DECODER_IDX        1
#define MS_SYSTEM_SOUNDS_IDX        2
#define MS_APPLICATION_SOUNDS_IDX   3
#define MS_MAX_NUM_INPUTS           4

#define MS_DOWNMIX_OUT_IDX          0
#define MS_VIRTUALIZER_OUT_IDX      1
#define MS_DAP_SPEAKER_OUT_IDX      2
#define MS_DAP_HEADPHONE_OUT_IDX    3
#define MS_MAX_NUM_PCM_OUTPUTS      4

#define MS_DAP_SPEAKER_OUT_ACTIVE   1
#define MS_DAP_HEADPHONE_OUT_ACTIVE 2
#define MS_DAP_BOTH_OUT_ACTIVE      3

#define MS_DD_OUT_IDX               0
#define MS_DDP_OUT_IDX              1
#define MS_MAX_NUM_BS_OUTPUTS       2

#define DBG_OUT_MAIN_DEC         0x0001
#define DBG_OUT_ASSOC_DEC        0x0002
#define DBG_OUT_AD_MIX           0x0004
#define DBG_OUT_RSRC             0x0008
#define DBG_OUT_DAP_CP           0x0010
#define DBG_OUT_SYS_MIX          0x0020
#define DBG_OUT_DAP_DP_IN        0x0040

/** Compression profile */
#define COMPPROF_NONE (0)       /*!< Compression profile 0: none                       */
#define COMPPROF_FILMSTD (1)    /*!< Compression profile 1: film standard compression  */
#define COMPPROF_FILMLIGHT (2)  /*!< Compression profile 2: film light compression     */
#define COMPPROF_MUSICSTD (3)   /*!< Compression profile 3: music standard compression */
#define COMPPROF_MUSICLIGHT (4) /*!< Compression profile 4: music light compression    */
#define COMPPROF_SPEECH (5)     /*!< Compression profile 5: speech compression         */

/** dsurmod */
#define  DSURMOD_NO_INDICATION    (0) /*!< dsurmod not indicated           */
#define  DSURMOD_NOT_SURR_ENCODED (1) /*!< signal is not surround encoded  */
#define  DSURMOD_SURR_ENCODED     (2) /*!< signal is surround encoded      */
#define  DSURMOD_RESERVED         (3) /*!< reserved                        */


/* List of defines used in ms_user.c */
#define MS_EXT_LENGTH           4                 /**< Number of characters expected in file extensions */
#define MS_MIN_VERBOSITY        0                 /**< Minimum verbosity mode                           */
#define MS_MAX_VERBOSITY        3                 /**< Maximum verbosity mode                           */
#define MS_MIN_COMP_PROF        0                 /**< Minimum external PCM compression profile         */
#define MS_MAX_COMP_PROF        5                 /**< Maximum external PCM compression profile         */
#define MS_MIN_PROG_REFLEV      0                 /**< Minimum external PCM program reference profile   */
#define MS_MAX_PROG_REFLEV     31                 /**< Maximum external PCM program reference profile   */
#define MS_MIN_DIALNORM         0                 /**< Minimum HE-AAC dialnorm value                    */
#define MS_MAX_DIALNORM       127                 /**< Maximum HE-AAC dialnorm value                    */
#define MS_MIN_TRANSFMT         0                 /**< Minimum HE-AAC transport format value            */
#define MS_MAX_TRANSFMT         3                 /**< Maximum HE-AAC transport format value            */
#define MS_MIN_MULTCH_MODE      0                 /**< Minimum multichannel mode value                  */
#define MS_MAX_MULTCH_MODE      1                 /**< Minimum multichannel mode value                  */
#define MS_MIN_DUALMONO         0                 /**< Minimum dual-mono mode                           */
#define MS_MAX_DUALMONO         2                 /**< Minimum dual-mono mode                           */
#define MS_MIN_DRC_BOOST        0                 /**< Minimum DRC boost value                          */
#define MS_MAX_DRC_BOOST      100                 /**< Maximum DRC boost value                          */
#define MS_MIN_DRC_CUT          0                 /**< Minimum DRC cut value                            */
#define MS_MAX_DRC_CUT        100                 /**< Maximum DRC cut value                            */
#define MS_MIN_DMX              0                 /**< Minimum stereo downmix value                     */
#define MS_MAX_DMX              2                 /**< Minimum stereo downmix value                     */
#define MS_MIN_DRC_MODE         0                 /**< Minimum DRC mode                                 */
#define MS_MAX_DRC_MODE         1                 /**< Maximum DRC mode                                 */
#define MS_MIN_BOOL_VAL         0                 /**< Minimum value allowed for a boolean */
#define MS_MAX_BOOL_VAL         1                 /**< Maximum value allowed for a boolean */
#define MS_MIN_USERBAL        -32                 /**< Minimum user balance value                       */
#define MS_MAX_USERBAL         32                 /**< Maximum user balance value                       */
#define MS_MIN_BS_PROC_MODE     0                 /**< Minimum Bitstream processing mode                */
#define MS_MAX_BS_PROC_MODE     2                 /**< Maximum Bitstream processing mode                */
#define MS_MIN_ASSOC_SUBSTRM    1                 /**< Minimum Associated program substream value       */
#define MS_MAX_ASSOC_SUBSTRM    3                 /**< Maximum Associated program substream value       */
#define MS_MIN_WAVFILEBITDEPTH 16                 /**< Minimum WAV file bit-depth                       */
#define MS_MAX_WAVFILEBITDEPTH 32                 /**< Maximum WAV file bit-depth                       */
#define MS_MIN_EXTPCM_ACMOD     0                 /**< Minimum channel configuration for external PCM   */
#define MS_MAX_EXTPCM_ACMOD     MS_MAX_AC_MOD     /**< Maximum channel configuration for external PCM   */
#define MS_MIN_SYS_SOUNDS_ACMOD 0                 /**< Minimum channel configuration for system sounds  */
#define MS_MAX_SYS_SOUNDS_ACMOD MS_MAX_AC_MOD     /**< Maximum channel configuration for system sounds  */
#define MS_MIN_APP_SOUNDS_ACMOD 0                 /**< Minimum channel configuration for app. sounds    */
#define MS_MAX_APP_SOUNDS_ACMOD MS_MAX_AC_MOD     /**< Maximum channel configuration for app. sounds    */
#ifdef MS12_7_1_SUPPORT
#define MS_MIN_MAX_CHANNEL      6                 /**< Minimum value for max. number of channels        */
#define MS_MAX_MAX_CHANNEL      8                 /**< Maximum value for max. number of channels        */
#else
#define MS_MIN_MAX_CHANNEL      6                 /**< Minimum value for max. number of channels        */
#define MS_MAX_MAX_CHANNEL      6                 /**< Maximum value for max. number of channels        */
#endif
#define MS_MIN_ENC_CHMOD_LOCK   0                 /**< Minimum value for encoder channel mode locking.  */
#define MS_MAX_ENC_CHMOD_LOCK   1                 /**< Maximum value for encoder channel mode locking.  */
#define MS_MIN_DAP_DMX_TYPE     0                 /**< Minimum value for DAP downmix mode               */
#define MS_MAX_DAP_DMX_TYPE     1                 /**< Maximum value for DAP downmix mode               */
#define MS_MIN_DAP_DRC_TYPE     0                 /**< Minimum value for DAP DRC mode               */
#define MS_MAX_DAP_DRC_TYPE     1                 /**< Maximum value for DAP DRC mode               */
#ifdef MS12_AC4_SUPPORT
#define MS_MIN_AC4_DE_GAIN      0                 /**< Minimum value for AC4 Dialogue enhancement gain   */
#define MS_MAX_AC4_DE_GAIN      12                /**< Maximum value for AC4 Dialogue enhancement gain   */
#define MS_MIN_AC4_ASSOC_TYPE   1                 /**< Minimum associated content type preference index  */
#define MS_MAX_AC4_ASSOC_TYPE   3                 /**< Maximum associated content type preference index  */
#define MS_AC4_NUM_LANG_CHAR    3                 /**< Number of characters used for language selection. */
#define MS_AC4_LANG_MAX_STRLEN  (MS_AC4_NUM_LANG_CHAR+1)
#define AC4_MODE_SINGLE_STREAM_SINGLE_DECODE               0
#define AC4_MODE_SINGLE_STREAM_DUAL_DECODE_SINGLE_INSTANCE 1
#define AC4_MODE_SINGLE_STREAM_DUAL_DECODE_DUAL_INSTANCE   2
#define AC4_MODE_DUAL_STREAM_DUAL_DECODE                   3
#define MS_MIN_AC4_MODE         AC4_MODE_SINGLE_STREAM_SINGLE_DECODE
#define MS_MAX_AC4_MODE         AC4_MODE_DUAL_STREAM_DUAL_DECODE
#define MS_MIN_AC4_PRES_IDX     -1                       /**< Minimum value for AC4 presentation index */
#define MS_MAX_AC4_PRES_IDX      63                      /**< Maximum value for AC4 presentation index */
#define MS_MIN_AC4_PROG_ID_TYPE  0                       /**< Minimum value for AC4 program id type */
#define MS_MAX_AC4_PROG_ID_TYPE  2                       /**< Maximum value for AC4 program id type */
#define MS_MIN_AC4_SHORT_PROG_ID       0                 /**< Minimum value for AC4 short program id */
#define MS_MAX_AC4_SHORT_PROG_ID   65536                 /**< Maximum value for AC4 short program id */
#endif

#define MS_COMMAND_FILE "command.txt" /**< Command Filename to read arguments from          */

#define MS_LOW_COMPLEXITY_MAX_CHANNEL  6   /**< Maximum number of channels supported in low complexity mode */
#define MS_AC4_MAX_CHANNEL             6   /**< Maximum number of channels supported in AC4 decoder output */

#define MS_DMX_TYPE_LTRT       (0)
#define MS_DMX_TYPE_LORO       (1)
#define MS_DMX_TYPE_ARIB       (2)

#define DAP_MAX_BANDS          (20)
#define DAP_IEQ_MAX_BANDS      DAP_MAX_BANDS
#define DAP_GEQ_MAX_BANDS      DAP_MAX_BANDS
#define DAP_REG_MAX_BANDS      DAP_MAX_BANDS
#define DAP_OPT_MAX_BANDS      DAP_MAX_BANDS
#define DAP_MAX_CHANNELS       (MS_MAX_MAX_CHANNEL)

#define MAX_RUNTIME_PARAMS_MAIN_INPUT 17
#define MAX_RUNTIME_PARAMS_EXTPCM_INPUT 2
#define MAX_RUNTIME_PARAMS_SYSSOUND_INPUT 2
#define MAX_RUNTIME_PARAMS_APPSOUND_INPUT 2

#define DAP_VOLMAX_PARAMS 0
#define DAP_MODELER_PARAMS 0
#define DAP_GAIN_PARAMS 1
#define MAX_RUNTIME_PARAMS_DAP 14 + DAP_MODELER_PARAMS + DAP_VOLMAX_PARAMS + DAP_GAIN_PARAMS

#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
#define NUM_DAP_GAIN_PARAM 3
#else
#define NUM_DAP_GAIN_PARAM 1
#endif

#define NUM_DAP_LEVELER_PARAM 3

#define PARAM_UPDATE_TYPE_INT     0
#define PARAM_UPDATE_TYPE_STRING  1
#define PARAM_UPDATE_TYPE_INT_VAR 2

#if 0
/* Helper Defines for assembling the PCM Renderer user parameters */
#define USER_PARAM_PCM_DMX_OUTPUT_ACTIVE           1  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DUAL_MONO_MODE   2  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_DMX_OUTPUT_COMPRESSION_MODE 3  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DMX_TYPE         4  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DRC_CUT          5  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DRC_BOOST        6  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_MC_OUTPUT_ACTIVE            7  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_MC_OUTPUT_DUAL_MONO_MODE    8  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_MC_OUTPUT_COMPRESSION_MODE  9  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_MC_OUTPUT_DRC_CUT          10  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_MC_OUTPUT_DRC_BOOST        11  /**< internal set_param id for filling msf_pcm_output_user_params struct */
#define USER_PARAM_PCM_MC_OUTPUT_5_1_DMX          12  /**< internal set_param id for filling msf_pcm_output_user_params struct */

#define USER_PARAM_EXTPCM_ACMOD                   13
#define USER_PARAM_EXTPCM_LFEON                   14
#define USER_PARAM_SYSSOUND_ACMOD                 15
#define USER_PARAM_SYSSOUND_LFEON                 16
#define USER_PARAM_APPSOUND_ACMOD                 17
#define USER_PARAM_APPSOUND_LFEON                 18

#define MD_ID_DDP_SUBSTREAM_SELECT       1602       /**< Static metadata, 1 integer */
#define MD_ID_AD_MIXING_ENABLED          1603       /**< Static metadata, 1 integer */
#define MD_ID_AD_MIXING_USER_PREF        1604       /**< Static metadata, 1 integer */
#define MD_ID_SYSSOUND_MIXING_ENABLED    1605       /**< Static metadata, 1 integer */
#define MD_ID_APP_MIXING_USER_PREF       1607       /**< Static metadata, 1 integer */

#define MD_ID_DAP_MI_INFO                1609       /**< Timed metadata containing dap media inteligence information */
#define MD_ID_DAP_MI_INFO2               1610       /**< Timed metadata containing dap media inteligence information for 2nd dual mono channel */
#define MD_ID_DAP_DMX_DATA               1611       /**< Timed metadata containing dap downmix data */

#define MD_ID_DAP_SPEAKER_LFE            1612
#define MD_ID_DAP_GAINS                  1613
#define MD_ID_DAP_SURR_DECODER           1614
#define MD_ID_DAP_SURR_VIRTUALIZER       1615
#define MD_ID_DAP_MI_STEERING            1616
#define MD_ID_DAP_CAL_BOOST              1617
#define MD_ID_DAP_LEVELER                1618
#ifdef DAP_MODELER_SUPPORT
#define MD_ID_DAP_MODELER                1619
#endif
#define MD_ID_DAP_DIALOGUE_ENH           1620
#ifdef DAP_VOLMAX_SUPPORT
#define MD_ID_DAP_VOLMAX_BOOST           1621
#endif
#define MD_ID_DAP_BASS_ENHANCER          1622
#define MD_ID_DAP_IEQ                    1623
#define MD_ID_DAP_GEQ                    1624
#define MD_ID_DAP_OPTIMIZER              1625
#define MD_ID_DAP_REGULATOR              1626

#define MD_ID_DAP_DRC                    1627
#define MD_ID_DAP_DMX_TYPE               1628
#define MD_ID_DAP_VIRTUAL_BASS           1632
#endif


typedef enum ms_bs_output_mode_t
{
    MS_BS_OUTPUT_MODE_NONE,
    MS_BS_OUTPUT_MODE_DD,
    MS_BS_OUTPUT_MODE_DDP,
    MS_BS_OUTPUT_MODE_SIM_DDP_DD
} ms_bs_output_mode_t;

typedef enum ms_bs_proc_mode_t
{
    MS_BS_PROC_MODE_TRANSCODE,
    MS_BS_PROC_MODE_PASSTHROUGH,
    MS_BS_PROC_MODE_CONVERSION,
} ms_bs_proc_mode_t;

typedef enum ms_dap_mode_t
{
    MS_DAP_NO_PROC = 0,
    MS_DAP_CONTENT_PROC = 1,
    MS_DAP_DEVICE_PROC = 2,
    MS_DAP_DEVICE_CONTENT_PROC = MS_DAP_DEVICE_PROC | MS_DAP_CONTENT_PROC,
    MS_DAP_SI_PROC = 4,
} ms_dap_mode_t;


/**
 * @brief See dap_cpdp.h in the DAPv2 CIDK for a detailed description of the parameter.
 */
typedef struct dap_params_s
{
    int drc_type;
    int dmx_type;
    int speaker_lfe;
    int pregain;
    int postgain;
    int systemgain;
    int surround_decoder_enable;
    int virtualizer_enable;
    int headphone_reverb;
    int speaker_angle;
    int speaker_start;
    int surround_boost;
    int mi_ieq_enable;
    int mi_dv_enable;
    int mi_de_enable;
    int mi_surround_enable;

    int calibration_boost;

    int leveler_amount;
    int leveler_input;
    int leveler_output;
    int leveler_enable;
    int leveler_ignore_il;

    int modeler_enable;
    int modeler_calibration;

    int ieq_enable;
    int ieq_amount;
    int ieq_nb_bands;
    int a_ieq_band_center[DAP_IEQ_MAX_BANDS];
    int a_ieq_band_target[DAP_IEQ_MAX_BANDS];

    int de_enable;
    int de_amount;
    int de_ducking;

    int volmax_boost;

    int eq_enable;
    int eq_nb_bands;
    int a_geq_band_center[DAP_GEQ_MAX_BANDS];
    int a_geq_band_target[DAP_GEQ_MAX_BANDS];

    int optimizer_enable;
    int optimizer_nb_bands;
    int a_opt_band_center_freq[DAP_OPT_MAX_BANDS];
    int a_opt_band_gain[DAP_MAX_CHANNELS][DAP_OPT_MAX_BANDS];

    int bass_enable;
    int bass_boost;
    int bass_cutoff;
    int bass_width;

    int reg_nb_bands;
    int a_reg_band_center[DAP_REG_MAX_BANDS];
    int a_reg_low_thresholds[DAP_REG_MAX_BANDS];
    int a_reg_high_thresholds[DAP_REG_MAX_BANDS];
    int a_reg_isolated_bands[DAP_REG_MAX_BANDS];

    int regulator_overdrive;
    int regulator_timbre;
    int regulator_distortion;
    int regulator_mode;
    int regulator_enable;

    int vb_mode;
    int vb_low_src_freq;
    int vb_high_src_freq;
    int vb_overall_gain;
    int vb_slope_gain;
    int vb_subgain[3];
    int vb_mix_low_freq;
    int vb_mix_high_freq;
} dap_params_t;

//extern const char* acmod2str[];
//extern const char* dsurmod2str[];
//extern const char* instance2str[];

/*! Enumeration of possible input types beeing processed by the multistream decoder */
typedef enum MS_INPUT_TYPE
{
    DOLBY_DIGITAL_PLUS,
    HE_AAC,
#ifdef MS12_AC4_SUPPORT
    AC4,
#endif
    EXTERNAL_PCM,
    UNKNOWN    // merge for v1.3.1, merge here for backward compatible
} MS_INPUT_TYPE;

/*! Struct defining the current use case, not all permutations are allowed */
typedef struct MS_USE_CASE_S
{
    MS_INPUT_TYPE               input_type; /*!< Either HE-AAC, Dolby Digital Plus or external PCM */
    int                   b_low_complexity; /*!< Flag indicating if the Processing Graph is initialized in low complexity mode */
    int                       b_dual_input; /*!< Flag indicating if two input streams are present (n/a for external PCM) */
    int                        b_dual_main; /*!< Flag indicating if two main input streams are present (DDP only).
                                                 b_dual_main is a special case of b_dual_input. */
    int                    b_file_playback; /*!< Flag indicating file playback (HE-AAC with single input only) */
    int                    b_restricted_ad; /*!< Flag indicating that the associated instance is restricted to 2 channels (HE-AAC only) */
    int                         b_mainonly; /*!< Flag indicating that the UDC shall handle only the main program */
    int                    b_system_sounds; /*!< Flag indicating that system sounds are present */
    int                       b_app_sounds; /*!< Flag indicating that application sounds are present */
    int                        b_eval_mode; /*!< Flag indicating that evaluation mode is enabled */
    ms_bs_output_mode_t     bs_output_mode; /*!< Either DD, DD+ or both */
    ms_dap_mode_t            dap_init_mode; /*!< DAP configuration:
                                                  - 0: no DAP
                                                  - 1: content processing only
                                                  - 2: device processing only
                                                  - 3: both with two DAP instances
                                                  - 4: both with single instance DAP processing */
    int                    b_dap_vb_enable; /*!< DAP virtual bass configuration */
#ifdef MS12_AC4_SUPPORT
    int                           ac4_mode; /**< AC4 decoding mode:
                                                  - 0: single stream, single decode
                                                  - 1: single stream, dual decode, single instance (mixing inside decoder)
                                                  - 2: single stream, dual decode, dual instance (mixing outside decoder)
                                                  - 3: dual stream, dual decode, dual instance (mixing outside decoder) */
#endif
} MS_USE_CASE;

typedef struct CHANNEL_CONFIG_S
{
  uint16_t acmod;         /*!< Audio coding mode as defined for Dolby Digital.
                               The audio coding mode can be ONEPLUSONE=0, MONO=1, STEREO=2, or 3_2=7. */
  uint16_t b_lfe_present; /*!< Flag indicating presence of the low frequency enhancement channel. */
  uint16_t dsurmod;       /*!< Indicates if the signal is dolby surround encoded (only possible for stereo signals) */
} CHANNEL_CONFIG;


/**
 * @brief Mixer fade event parameter.
 */
typedef struct mix_fade_s
{
    int target_attenuation;  /**< Target attenuation at end of ramp in dB (range: -96...0). */
    int duration_in_ms;      /**< Duration of ramp in milliseconds (range: 0...60000). */
    int type;                /**< Shape of the ramp (0: linear, 1: in cube, 2: out cube). */
} mix_fade_t;


/** This struct holds all parameters that may change during runtime without re-initialization */
typedef struct MS_RUNTIME_PARAMS_S
{

  /* Parameters common to DD+, external PCM and HE-AAC */
  int16_t drc_cut_fac_mc;           /**< Determines the percentage of application of DRC
                                         attenuation factors for the multichannel channel output.    */
  int16_t drc_boost_fac_mc;         /**< Determines the percentage of application of DRC
                                         boost factors for the multichannel channel output.          */
  int16_t drc_cut_fac_2ch;          /**< Determines the percentage of application of DRC
                                         attenuation factors for the 2 channel output.               */
  int16_t drc_boost_fac_2ch;        /**< Determines the percentage of application of DRC
                                         boost factors for the 2 channel output.                     */
  int16_t downmix_type;             /**< Indicates which downmix type is used (LtRt,LoRo,ARIB)       */
  int16_t drc_mode;                 /**< Indicates whether to operate in RF or Line mode             */
  int16_t dual_mono;                /**< Dual Mono reproduction mode, (both, left, right)            */
  int16_t cmix_lev;                 /**< Index in center mix level table (-3dB,-4.5dB,-6dB)          */
  int16_t smix_lev;                 /**< Index in surround mix level table (-3dB,-4.5dB,-6dB)        */
  int16_t multichannel_enable;      /**< Indicates if the signal in the multichannel output is valid */
  int16_t associated_audio_mixing;  /**< Enables/Disables mixing in dual decoding use cases          */
  int16_t user_balance_adjustment;  /**< User defined balance between main and associated signal.
                                         Value range is between -32 and +32 (in dB),
                                         -32dB indicates main only (mute associated)
                                         +32dB indicates associated only (mute main)                 */
  int16_t b_sys_app_sound_mixing;   /**< Enables/Disables mixing of systen and application sounds    */

  mix_fade_t input_mix_input1;      /**< Input mixer input1 gain/fade event.                         */
  mix_fade_t input_mix_input2;      /**< Input mixer input2 gain/fade event.                         */
  mix_fade_t syssound_mix_input1;   /**< System/application sound mixer input1 gain/fade event.      */
  mix_fade_t syssound_mix_input2;   /**< System/application sound mixer input2 gain/fade event.      */
  mix_fade_t syssound_mix_input3;   /**< System/application sound mixer input3 gain/fade event.      */

  /* External PCM parameters */
  int                       extpcm_num_in_samples;          /**< Number of input samples to process  */
  CHANNEL_CONFIG            extpcm_in_channel_config;       /**< Input channel configuration         */

  /* System Sounds PCM parameters */
  int                       sys_sounds_num_in_samples;      /**< Number of input samples to process  */
  CHANNEL_CONFIG            sys_sounds_channel_config;      /**< Input channel configuration         */

  /* Application Sounds PCM parameters */
  int                       app_sounds_num_in_samples;      /**< Number of input samples to process  */
  CHANNEL_CONFIG            app_sounds_channel_config;      /**< Input channel configuration         */

  /* HE-AAC parameters */
  int16_t heaac_mixing_mode;          /**< Blocking or non-blocking mixing, i.e. does main wait for associated */
  int16_t heaac_default_dialnorm;     /**< Default dialnorm (dB/4) that is used until a valid dialnorm is found in the bitstream */
  int16_t heaac_transport_format;     /**< Specifies the HE-AAC transport format
                                           (can increase robustness compared to auto-detect)   */

  /* DDPlus parameters */
  int16_t ddplus_associated_substream; /**< ID of the substream which should be mixed with the main program */
  int16_t ddplus_outlfe;               /**< Determines if LFE will be present in the DD+ decoded signal  */
  int16_t ddplus_outmode;              /**< Determines which acmod will be set in the DD+ decoded signal */

  dap_params_t  dap_parameter;         /**< Contains all configurable DAP parameter */
  int           b_mc_5_1_dmx;          /**< Determines if 7.1 signals shall be dowmixed to 5.1 for the multichannel PCM output */

#ifdef MS12_AC4_SUPPORT
  /* AC4 parameters */
  char          ac4_1st_pref_lang[MS_AC4_LANG_MAX_STRLEN]; /**< String containing the preferred language selection */
  char          ac4_2nd_pref_lang[MS_AC4_LANG_MAX_STRLEN]; /**< String containing the preferred language selection */
  int           ac4_associated_type;                       /**< Indication of the preferred associated content type */
  int           b_ac4_pref_assoc_type_over_lang;           /**< Preferred filter priority (selection of associated content type over language or vice versa) */
  int           ac4_de_gain;                               /**< Indicates gain that will be applied to dialog enhancement. Range: 0 to 12 dB (in 1 dB steps) */
  int           ac4_pres_index[2];                         /**< Primary and secondary Presentation index to be decoded. Overrides presentation selection by language and assoc. type. */
  int           ac4_prog_id_type;                                           /**< Determines whether the program_identifier is present or not and whether it's the short or the long version. */
  char          ac4_program_identifier_string[2*MS_PROGRAM_UUID_LEN_BYTES]; /**< Either the short program identifier (16 bit) or the UUID (128 bit) as a string copied from the command line */
#endif

  /* parameter update status variables */
  int32_t    frame;                    /**< Current Frame number (necessary to apply runtime changes) */
 // DLB_TIMESTAMP  next_update;          /**< Indicates the next frame when parameters shall be updated */
} MS_RUNTIME_PARAMS;

/** This struct holds all parameters that are set at initialization time */
typedef struct MS_INIT_PARAMS_S
{
  /* Input and output filenames */
  const char *input_filename[MS_MAX_NUM_INPUTS];               /**< Names of the input file(s) - main, associated, sys sounds, app sounds */
  const char *pcm_output_filename[MS_MAX_NUM_PCM_OUTPUTS];     /**< Names of the PCM output files - downmix, multichannel, dap speaker and headphone  */
  const char *bitstream_output_filename[MS_MAX_NUM_BS_OUTPUTS];/**< Name of the DD and DD+ bitstream output file. */
  const char *pcmr_output_filename;                            /**< Name of the PCMR output text file. */
  int         pcm_out_chans[MS_MAX_NUM_PCM_OUTPUTS];           /**< Number of channels of the PCM output files */

  int16_t     verbose_mode;                 /**< Determines the amout of information that is written to the command line */
  int         wav_bit_depth;                /**< Output WAV file bit-depth */
  int         max_num_channels;             /**< Determines the maximum number of channels in the signal processing chain */
  int         enc_chmod_locking_mode;       /**< Determines whether the encoded channel mode is driven by the input or locked. */
  int         b_high_risc_precision;        /**< Determines whether LFRACT or SFRACT data type is used for PCM representation */

  /* PCM parameters */
  int16_t     extpcm_compressor_profile;    /**< The compressor profile for external PCM reencoding */

  /* HE-AAC parameters */
  uint16_t heaac_allow_partial_feed;        /**< Flag enabling feeding of less input data than necessary to complete an output frame */

  /* DAP parameters */
  uint16_t dap_output_mode;                 /**< 2-Bit Bitmask to indicate which of both supported DAP outputs is active */

  int         dbg_output_mask;              /**< Used to activate different debug outputs */
} MS_INIT_PARAMS;


/** This struct contains all multistream decoder parameters separated into init time and runtime settable parameters */
typedef struct MS_PARAMS_S
{
  MS_RUNTIME_PARAMS   ms_runtime_args; /**< Structure containing all runtime dependant arguments */
  MS_INIT_PARAMS      ms_init_args;    /**< Structure containing all initialization dependant arguments */
  DLB_GETPARAM_HANDLE h_getparam;      /**< Handle to the getparam static memory struct */
} MS_PARAMS;

/*
typedef struct process_graph_s
{
    MS_PARAMS           user_params;
    MS_USE_CASE         use_case;

    OMX_AUDIO_ms12_update_params  update_params;

} process_graph_t;
*/
/*! \brief Struct holding a parameter and its value as string */
struct GETPARAM_SWITCH
{
  const char *name;      /*!< Parameter name */
  const char *value;     /*!< Parameter value stored as string */
  int got;               /*!< Indicates if the parameter has been accessed by the user */
};

/*! \brief This struct holds all information that needs to persist in between calls to the GetParam functions */
struct GETPARAM_MEM
{
#ifdef DLB_GETPARAM_TIMESTAMP
  FILE *pfile;                                                        /*!< File handle to the runtime command file */
  char cmdline[DLB_GETPARAM_MAXLEN + DLB_GETPARAM_MAXTSLEN * 2 + 3];  /*!< String containing the last command line read from the
                                                                           command file */
  const char *_argv[DLB_GETPARAM_MAX_SWITCHES * 2 + 1];               /*!< Array of strings to emulate argv when reading from command
                                                                           file */
  int b_stop_line;                                                    /*!< Flag for DLB_GETPARAM_STOP_LINE detected */
#endif
#ifdef DLB_GETPARAM_TEXTFILE
  FILE *pfile;                                                        /*!< File handle to the runtime command file */
  char cmdline[DLB_GETPARAM_MAXLEN];                                  /*!< String containing the last command line read from the
                                                                           command file */
  const char *_argv[DLB_GETPARAM_MAX_SWITCHES * 2 + 1];               /*!< Array of strings to emulate argv when reading from command
                                                                           file */
#endif
#ifdef DLB_GETPARAM_HELP
  int help_count;                                                     /*!< Number of help texts in the help array */
  struct GETPARAM_HELP help[DLB_GETPARAM_MAX_SWITCHES];               /*!< Array holding all parameters and their helptext set so far */
#endif
  int switches_count;                                                 /*!< Number of command line switches found */
  struct GETPARAM_SWITCH switches[DLB_GETPARAM_MAX_SWITCHES];         /*!< Array with all command line parameters and their values */
};

#define PARAM_UPDATE_MAX_LINE_LENGTH 4096

/**
 * @file
 *
 * This module provides the possibility to update parameter for registered nodes
 * during runtime. It builds upon the dlb_clif library and it's runtime update
 * mechanism via command.txt files. The nodes will be notified by means of a
 * set_param callback mechanism in case they are registered.
 */

typedef struct param_query_s
{
    int   id;                       /** Identifies the parameter in the set_param function. */
    const char *switch_name;              /** Name of the command line switch without the leading -. */
    int   param_type;               /** Either PARAM_UPDATE_TYPE_INT, PARAM_UPDATE_TYPE_STRING or PARAM_UPDATE_TYPE_INT_VAR*/
    int   num_values;               /** Number of integers (PARAM_UPDATE_TYPE_INT only) */
    int   b_mandatory;              /** Determines if the parameter is optional or mandatory */
    int   b_multiple_notifications; /** The parameter might be registered for multiple nodes */
    const char *str_name;                 /** Name of the parameter to be used in error messages */
} param_query;

typedef struct param_update_s
{
   // node_handle              *ah_node;
    int                      *a_table_entries;
    const param_query       **ap_param_table;
    //node_set_param_func_ptr  *a_pfunc_set_param;
    int                       curr_num_nodes;
    int                       max_num_nodes;
    long int                 *a_int_retriever;
    int                       max_int_array_len;

    DLB_TIMESTAMP             time_of_next_update;
    DLB_GETPARAM_HANDLE       h_get_param;
} param_update;

typedef struct param_update_s *param_update_handle;

typedef struct PauseParam
{
    bool is_pause;
    unsigned long int start_us;
    unsigned long int end_us;
} PauseParam;

/***********for switch file*************************/
#define MAX_SWITCH_FILE_PARAM_NUM       8
#define MAX_SWITCH_FILE_PARAM_MEM_SIZE  256

typedef struct switch_file_query_s
{
    int                 apply_start_sample;
    int                 apply_pin_idx;
    int                 num_bytes;
    int                 mem_pool_start_offset;
} SWITCH_FILE_QUERY;

typedef struct switch_file_params_s
{
    int                 num_param_update;
    int                 mem_pool_total_used_offset;
    SWITCH_FILE_QUERY   param_update_table[MAX_SWITCH_FILE_PARAM_NUM];
    unsigned char       static_mem_pool[MAX_SWITCH_FILE_PARAM_MEM_SIZE];
}SWITCH_FILE_PARAM;

/***********for switch file end*********************/
/**
 *  @brief This function parses the command line arguments and converts those in API parameters
 *
 *  This function parses the command line arguments provided by the user. It checks if all mandatory
 *  parameters have been given and derives the current use from the parameters. The argument
 *  ranges are validated and converted to the format required by the API parameter if necessary.
 *  A help text is presented to the user if requested or in case of invalid arguments or invalid argument combinations.
*/
MS_RETVAL ms_parse_command_line(int argc,                /**< [in] Number of command line arguments */
                                const char *argv[],      /**< [in] Array of command line argument strings */
                                MS_USE_CASE *p_use_case, /**< [out] Struct containing all information about the current use case */
                                MS_PARAMS *p_params,     /**< [out] Struct containing all parameters */
                                int *err_code            /**< [out] Indicates what went wrong, 0 on ok */
                                );

/**
 * @brief This function updates all runtime parameters if necessary.
 *
 * This function reads from a configuration file which parameters needs to be updated during runtime. The
 * parameter ranges are validated and converted to the format required by the API parameter if necessary.
 * A warning is presented to the user in case of invalid arguments or invalid argument combinations.
 */
MS_RETVAL ms_parse_runtime_arguments(MS_USE_CASE *p_use_case,             /**< [in] Current use case */
                                     MS_RUNTIME_PARAMS *p_runtime_params, /**< [out] Runtime dependent parameters */
                                     DLB_GETPARAM_HANDLE h_getparam,      /**< [in, out] Handle to the getparam static memory struct */
                                     int *err_code                        /**< [out] Indicates what went wrong, 0 on ok */
                                     );

/**
 * @brief This function initializes all parameters with their defaults
 */
void ms_init_params(MS_PARAMS *p_params    /**< [out] Struct containing all parameters */
                    );


void ms_print_parameter_settings(MS_USE_CASE *p_use_case, MS_PARAMS *p_args);

void CopyMS12IinitParamToOMXAudio(OMX_AUDIO_MS_INIT_PARAMS*init_args , MS_INIT_PARAMS *ms_init_args);
void CopyMS12UseCaseToOMXAudio(OMX_AUDIO_MS_USE_CASE *use_case, MS_USE_CASE *ms_use_case);
void CopyMS12RuntimeParamToOMXAudio(OMX_AUDIO_MS_RUNTIME_PARAMS *args, MS_RUNTIME_PARAMS *pParams);



#endif
