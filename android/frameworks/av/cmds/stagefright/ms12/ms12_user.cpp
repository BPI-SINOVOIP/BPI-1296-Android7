#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ms12_cmd.h"
#include "ms12_err.h"


#define MS_EXTPCM_IN_SAMPLES     1536

#define MS_MAX_STR_LEN           1024

#define MS_MAJOR_VERSION      "1"
#define MS_MINOR_VERSION      "1"
#define MS_UPDATE_VERSION     "0"

#define MS_DAP_MIN_VAL            (-32767)
#define MS_DAP_MAX_VAL            ( 32767)



/* Internal Function Declarations */
/** @brief This function retrieves the runtime dependant parameters from the parsed command line (or parsed command text file) */
static MS_RETVAL ms_get_runtime_params(MS_USE_CASE *p_use_case, MS_RUNTIME_PARAMS *p_runtime_params, DLB_GETPARAM_HANDLE h_getparam, int *err_code);
/** @brief This function retrieves the DAP parameters from the parsed command line (or parsed command text file) */
static MS_RETVAL ms_get_dap_params(dap_params_t *p_dap_params, DLB_GETPARAM_HANDLE h_getparam, int b_dap_vb_enable, int *err_code);
/** @brief Prints the complete help text to the command line */
static MS_RETVAL ms_show_usage_full(void);
/** @brief Prints the complete help text for low complexity mode to the command line */
static MS_RETVAL ms_show_usage_lc(void);
/** @brief Prints a brief version of the help text to the command line */
static MS_RETVAL ms_show_usage_brief(void);
/** @brief Prints the usage of the command line switches that are common to all use cases */
static MS_RETVAL ms_show_usage_common_switches(void);
/** @brief Prints the usage of the command line switches that are common to all use cases supported in low complexity processing mode */
static MS_RETVAL ms_show_usage_common_switches_lc(void);
/** @brief Prints out a banner message including version numbers and copyright */
static MS_RETVAL ms_show_banner(void);
/** @brief Prints the help text for the use cases with Dolby Digital Plus as input type */
static MS_RETVAL ms_show_usage_context_ddplus(void);
/** @brief Prints the help text for the external PCM use case */
static MS_RETVAL ms_show_usage_context_pcm(void);
/** @brief Prints the help text for the external PCM use case in low complexity processing mode */
static MS_RETVAL ms_show_usage_context_pcm_lc(void);
/** @brief Prints the help text for the use cases with HE-AAC as input type */
static MS_RETVAL ms_show_usage_context_heaac(void);
/** @brief Prints the help text for DAP specific switches */
static MS_RETVAL ms_show_usage_context_dap(void);
/** @brief Checks that the string has the specified suffix extension */
static MS_RETVAL ms_validate_extension(const char *param, const char *ext);
/** @brief Retrieves comma seperated integers from a string into an array */
static MS_RETVAL ms_get_int_array_from_str(char **p_csv_string, int num_el, int *p_vals);
/** @brief Retrieves integer from a string and returns address of next CSV integer */
static MS_RETVAL ms_get_int_from_str(char **p_csv_string, int *p_vals);

static MS_RETVAL set_int_check_gp_err(DLB_GETPARAM_RETVAL gp_err, int val, int *p_assign_val, int *err_code, int outofrange, int invalid);
static void ms_print_dap_parameter(dap_params_t *p_dap_params);

static const char *enable_to_str(int b_enable);
/** Conversion table from acmod values into strings */
const char* acmod2str[] = {"ONEPLUSONE", "MONO", "STEREO", "3/0", "2/1", "3/1", "2/2", "3/2", "", "", "", "", "", "", "", "", "", "", "", "", "", "3/4" };

/** Conversion table from dsurmod values into strings */
const char* dsurmod2str[] =  {"NOT INDICATED", "NOT SURROUND ENCODED", "SURROUND ENCODED"};

/** Conversion table for the instance index into string */
const char* instance2str[] = {"main", "associated"};

/** Conversion table from transport format values into strings */
static const char *transport_format2str[] = {"auto-detect", "ADTS", "LOAS", "RAW"};
/** Conversion table from downmix type values into strings */
static const char *downmix_type2str[] = {"LtRt", "LoRo", "ARIB"};
/** Conversion table from dual mono mode values into strings */
static const char *dual_mode2str[] = {"both channels","1st channel only","2nd channel only"};
/** Conversion table from compression profile values into strings */
static const char* profile2str[] = {"no compression", "film standard", "film light",
                                    "music standard", "music light", "speech"};

static const char *enable2str[] = {"not set", "off", "on"};

/*
 * Documentation resides in header file only
 */
void ms_print_parameter_settings(MS_USE_CASE *p_use_case, MS_PARAMS *p_args)
{
    uint16_t defDialnormMasked = p_args->ms_runtime_args.heaac_default_dialnorm & 0x0003;

    fprintf(stdout, "-----------------------------------------------------------\n");
    fprintf(stdout, "  USE CASE:\n");
    if (p_use_case->b_low_complexity)
    {
        fprintf(stdout, "  LOW COMPLEXITY PROCESSING MODE (DAP) \n");
        switch(p_use_case->input_type){
        case DOLBY_DIGITAL_PLUS:
            fprintf(stdout, "  DOLBY DIGITAL PLUS DECODING CONVERTING \n");
            break;
        case EXTERNAL_PCM:
            fprintf(stdout, "  EXTERNAL PCM PROCESSING\n");
            break;
        case HE_AAC:
            assert(0);
            break;
        }
    }
    else
    {
    switch(p_use_case->input_type){
    case HE_AAC:
        if(p_use_case->b_dual_input)
            fprintf(stdout, "  HE-AAC DUAL DECODING TRANSCODING\n");
        else
            fprintf(stdout, "  HE-AAC SINGLE DECODING TRANSCODING\n");
        break;
    case DOLBY_DIGITAL_PLUS:
        if(p_use_case->b_dual_input)
            fprintf(stdout, "  DOLBY DIGITAL PLUS TRANSCODING - DUAL INPUT\n");
        else
            fprintf(stdout, "  DOLBY DIGITAL PLUS TRANSCODING - SINGLE INPUT\n");
        break;
    case EXTERNAL_PCM:
        fprintf(stdout, "  EXTERNAL PCM REENCODING\n");
        break;
    }
    }
    fprintf(stdout, "-----------------------------------------------------------\n");
    fprintf(stdout, "  System Sound Mixing:              %s\n", p_use_case->b_system_sounds ? "on" : "off");
    if(p_use_case->b_low_complexity)
    {
        fprintf(stdout, "  Application Sound Mixing:         n/a\n");
    }
    else
    {
    fprintf(stdout, "  Application Sound Mixing:         %s\n", p_use_case->b_app_sounds ? "on" : "off");
    }
    if((int)p_use_case->dap_init_mode == MS_DAP_SI_PROC)
    {
        fprintf(stdout, "  DAP Single Instance Processing:   on\n");
    }
    else
    {
        if (!p_use_case->b_low_complexity)
        {
        fprintf(stdout, "  DAP Content Processing:           %s\n", ((int)p_use_case->dap_init_mode & MS_DAP_CONTENT_PROC) ? "on" : "off");
        fprintf(stdout, "  DAP Device Processing:            %s\n", ((int)p_use_case->dap_init_mode & MS_DAP_DEVICE_PROC) ? "on" : "off");
    }
    }
    fprintf(stdout, "  File playback:                    %s\n", p_use_case->b_file_playback ? "on" : "off");
    if(p_use_case->b_dual_input && (p_use_case->input_type==HE_AAC) && !p_use_case->b_low_complexity)
        fprintf(stdout, "  Associated 2 Channel Restriction: %s\n", p_use_case->b_restricted_ad ? "on" : "off");
    fprintf(stdout, "-----------------------------------------------------------\n");
    fprintf(stdout, "  Parameter Settings:\n");
    fprintf(stdout, "-----------------------------------------------------------\n");
    if(p_use_case->input_type != EXTERNAL_PCM)
    {
	fprintf(stdout, "  Input Bitstream Main:               %s\n", p_args->ms_init_args.input_filename[MS_MAIN_DECODER_IDX]);
    }
    else{
	fprintf(stdout, "  Input Wavefile Main:                %s\n", p_args->ms_init_args.input_filename[MS_MAIN_DECODER_IDX]);
    }
    if(p_use_case->b_dual_input)
        fprintf(stdout, "  Input Bitstream Associated:         %s\n", p_args->ms_init_args.input_filename[MS_ASSOC_DECODER_IDX]);
    if(p_use_case->b_system_sounds)
	fprintf(stdout, "  Input Wavefile System Sound:        %s\n", p_args->ms_init_args.input_filename[MS_SYSTEM_SOUNDS_IDX]);
    if(p_use_case->b_app_sounds)
	fprintf(stdout, "  Input Wavefile Application Sound:   %s\n", p_args->ms_init_args.input_filename[MS_APPLICATION_SOUNDS_IDX]);
    if(p_args->ms_init_args.bitstream_output_filename[MS_DD_OUT_IDX])
        fprintf(stdout, "  AC3 output bitstream:               %s\n", p_args->ms_init_args.bitstream_output_filename[MS_DD_OUT_IDX]);
    if(p_args->ms_init_args.bitstream_output_filename[MS_DDP_OUT_IDX])
        fprintf(stdout, "  EC3 output bitstream:               %s\n", p_args->ms_init_args.bitstream_output_filename[MS_DDP_OUT_IDX]);
    if(p_args->ms_init_args.pcm_output_filename[MS_DOWNMIX_OUT_IDX])
        fprintf(stdout, "  PCM Stereo Downmix Output Main:     %s\n", p_args->ms_init_args.pcm_output_filename[MS_DOWNMIX_OUT_IDX]);
    if(p_args->ms_init_args.pcm_output_filename[MS_VIRTUALIZER_OUT_IDX])
        fprintf(stdout, "  PCM MC Virtualizer Output:          %s\n", p_args->ms_init_args.pcm_output_filename[MS_VIRTUALIZER_OUT_IDX]);
    if (!p_use_case->b_low_complexity)
    fprintf(stdout, "  PCM MC Output is:                   %s\n", p_args->ms_runtime_args.multichannel_enable ? "active" : "not active");
    if(p_args->ms_init_args.pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX])
        fprintf(stdout, "  PCM DAP Headphone Output:           %s\n", p_args->ms_init_args.pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX]);
    if(p_args->ms_init_args.pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX])
        fprintf(stdout, "  PCM DAP Speaker Output:             %s\n", p_args->ms_init_args.pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX]);
    if(p_args->ms_init_args.pcmr_output_filename)
        fprintf(stdout, "  PCM Renderer output parameter file: %s\n", p_args->ms_init_args.pcmr_output_filename);
    fprintf(stdout, "  RF mode (affects 2ch only):         %s\n", p_args->ms_runtime_args.drc_mode ? "on" : "off");
    assert(p_args->ms_runtime_args.downmix_type < 3);
    fprintf(stdout, "  Max channels:                       %d\n", p_args->ms_init_args.max_num_channels);
    fprintf(stdout, "  Downmix type (2ch):                 %s\n", downmix_type2str[p_args->ms_runtime_args.downmix_type]);
    fprintf(stdout, "  DRC cut/boost factor (2ch):         %d%%, %d%%\n", p_args->ms_runtime_args.drc_cut_fac_2ch, p_args->ms_runtime_args.drc_boost_fac_2ch);
    if(p_use_case->b_low_complexity)
        fprintf(stdout, "  DRC cut/boost factor (dap output):  %d%%, %d%%\n", p_args->ms_runtime_args.drc_cut_fac_mc, p_args->ms_runtime_args.drc_boost_fac_mc);
    else
    fprintf(stdout, "  DRC cut/boost factor (6 or 8ch):    %d%%, %d%%\n", p_args->ms_runtime_args.drc_cut_fac_mc, p_args->ms_runtime_args.drc_boost_fac_mc);
    assert(p_args->ms_runtime_args.dual_mono < 3);
    fprintf(stdout, "  Dual Mono Reproduction:             %s\n", dual_mode2str[p_args->ms_runtime_args.dual_mono]);
    if (p_use_case->b_low_complexity){
        fprintf(stdout, "  AD Mixing:                          n/a\n");
    }
    else{
        fprintf(stdout, "  AD Mixing:                          %s\n", p_args->ms_runtime_args.associated_audio_mixing ? "on" : "off");
    }
    fprintf(stdout, "  System/Appication sound  Mixing:    %s\n", p_args->ms_runtime_args.b_sys_app_sound_mixing ? "on" : "off");
    if(!p_use_case->b_low_complexity)
    {
        if(p_args->ms_runtime_args.user_balance_adjustment == 32)
            fprintf(stdout, "  AD Mixing User Balance:             +32 (associated only)\n");
        else if(p_args->ms_runtime_args.user_balance_adjustment == -32)
            fprintf(stdout, "  AD Mixing User Balance:             -32 (main only)\n");
        else
            fprintf(stdout, "  AD Mixing User Balance:             %ddB\n", p_args->ms_runtime_args.user_balance_adjustment);


        if(p_args->ms_runtime_args.mixer_balance_adjustment == 32)
            fprintf(stdout, "  Application Mixing User Balance:    +32 (application audio only)\n");
        else if(p_args->ms_runtime_args.mixer_balance_adjustment == -32)
            fprintf(stdout, "  Application Mixing User Balance:    -32 (primary audio only)\n");
        else
            fprintf(stdout, "  Application Mixing User Balance:    %ddB\n", p_args->ms_runtime_args.mixer_balance_adjustment);
    }

    if(p_use_case->input_type==HE_AAC){
        fprintf(stdout, "  Mixing mode:                        %s\n", p_args->ms_runtime_args.heaac_mixing_mode ? "blocking" : "non-blocking");
        fprintf(stdout, "  HE-AAC Operation mode:               %s\n", p_args->ms_init_args.heaac_allow_partial_feed? "Push Mode" : "Pull Mode");
        assert(p_args->ms_runtime_args.heaac_transport_format < 4);
        fprintf(stdout, "  Transport Format:                   %s\n", transport_format2str[p_args->ms_runtime_args.heaac_transport_format]);
    }
    else if(p_use_case->input_type==EXTERNAL_PCM){
        fprintf(stdout, "  Input Samples per channel:          %d\n", p_args->ms_runtime_args.extpcm_num_in_samples);
        if(!p_use_case->b_low_complexity)
        fprintf(stdout, "  Compression Profile:                %s\n", profile2str[p_args->ms_init_args.extpcm_compressor_profile]);
    }
    else{
        if(!p_use_case->b_mainonly && !p_use_case->b_dual_input && !p_use_case->b_low_complexity)
            fprintf(stdout, "  Selected Associated Substream:      %d\n", p_args->ms_runtime_args.ddplus_associated_substream);
    }
    if(p_use_case->input_type==HE_AAC)
        fprintf(stdout, "  Default Dialnorm:                   %d.%02ddB\n", -(p_args->ms_runtime_args.heaac_default_dialnorm >> 2),  defDialnormMasked * 25);
    fprintf(stdout, "  Verbose mode:                       %d\n", p_args->ms_init_args.verbose_mode);
    fprintf(stdout, "  Evaluation mode:                    %s\n", p_use_case->b_eval_mode ? "on" : "off");
    if(p_args->ms_init_args.verbose_mode > 2)
    {
        ms_print_dap_parameter(&p_args->ms_runtime_args.dap_parameter);
    }
    fprintf(stdout, "\n");
}


static void ms_print_dap_parameter(dap_params_t *p_dap_params)
{
    int band, ch;

    fprintf(stdout, "-----------------------------------------------------------\n");
    fprintf(stdout, "  DAP Parameter Settings\n");
    fprintf(stdout, "-----------------------------------------------------------\n");
    if(p_dap_params->dmx_type != MS_DAP_PARAM_NOT_SET)
        fprintf(stdout, "  downmix type                         %s\n", downmix_type2str[p_dap_params->dmx_type]);
    if(p_dap_params->drc_type != MS_DAP_PARAM_NOT_SET)
        fprintf(stdout, "  DRC type                             %s\n", p_dap_params->drc_type ? "RF mode" : "Line mode");
    fprintf(stdout, "  postgain                             %d\n", p_dap_params->postgain);
#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
    fprintf(stdout, "  pregain                              %d\n", p_dap_params->pregain);
    fprintf(stdout, "  systemgain                           %d\n", p_dap_params->systemgain);
#endif
    fprintf(stdout, "  Surround decoding                    %s\n", enable_to_str(p_dap_params->surround_decoder_enable));
    fprintf(stdout, "  Virtualization enable                %s\n", enable_to_str(p_dap_params->virtualizer_enable));
    fprintf(stdout, "  headphone_reverb                     %d\n", p_dap_params->headphone_reverb);
    fprintf(stdout, "  speaker_angle                        %d\n", p_dap_params->speaker_angle);
    fprintf(stdout, "  speaker_start                        %d\n", p_dap_params->speaker_start);
    fprintf(stdout, "  surround_boost                       %d\n", p_dap_params->surround_boost);
    fprintf(stdout, "  mi_ieq_enable                        %s\n", enable_to_str(p_dap_params->mi_ieq_enable));
    fprintf(stdout, "  mi_dv_enable                         %s\n", enable_to_str(p_dap_params->mi_dv_enable));
    fprintf(stdout, "  mi_de_enable                         %s\n", enable_to_str(p_dap_params->mi_de_enable));
    fprintf(stdout, "  mi_surround_enable                   %s\n", enable_to_str(p_dap_params->mi_surround_enable));
    fprintf(stdout, "  calibration_boost                    %d\n", p_dap_params->calibration_boost);
    fprintf(stdout, "  leveler_enable                       %s\n", enable_to_str(p_dap_params->leveler_enable));
    fprintf(stdout, "  leveler_amount                       %d\n", p_dap_params->leveler_amount);
    fprintf(stdout, "  leveler_ignore_il                    %d\n", p_dap_params->leveler_ignore_il);
    fprintf(stdout, "  ieq_enable                           %s\n", enable_to_str(p_dap_params->ieq_enable));
    fprintf(stdout, "  ieq_amount                           %d\n", p_dap_params->ieq_amount);
    fprintf(stdout, "  ieq_nb_bands;                        %d\n", p_dap_params->ieq_nb_bands);
    fprintf(stdout, "  a_ieq_band_center[DAP_IEQ_MAX_BANDS]\n");
    for(band=0; band <DAP_IEQ_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_ieq_band_center[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  a_ieq_band_target[DAP_IEQ_MAX_BANDS]\n");
    for(band=0; band <DAP_IEQ_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_ieq_band_target[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  de_enable                            %s\n", enable_to_str(p_dap_params->de_enable));
    fprintf(stdout, "  de_amount                            %d\n", p_dap_params->de_amount);
    fprintf(stdout, "  de_ducking                           %d\n", p_dap_params->de_ducking);
    fprintf(stdout, "  eq_enable                            %s\n", enable_to_str(p_dap_params->eq_enable));
    fprintf(stdout, "  eq_nb_bands                          %d\n", p_dap_params->eq_nb_bands);
    fprintf(stdout, "  a_geq_band_center[DAP_GEQ_MAX_BANDS]\n");
    for(band=0; band <DAP_GEQ_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_geq_band_center[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  a_geq_band_target[DAP_GEQ_MAX_BANDS]\n");
    for(band=0; band <DAP_GEQ_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_geq_band_target[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  optimizer_enable                     %s\n", enable_to_str(p_dap_params->optimizer_enable));
    fprintf(stdout, "  optimizer_nb_bands                   %d\n", p_dap_params->optimizer_nb_bands);
    fprintf(stdout, "  a_opt_band_center_freq[DAP_OPT_MAX_BANDS]\n");
    for(band=0; band <DAP_OPT_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_opt_band_center_freq[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  a_opt_band_gain[DAP_MAX_CHANNELS][DAP_OPT_MAX_BANDS]\n");
    for(ch=0; ch<DAP_MAX_CHANNELS;ch++)
    {
        for(band=0; band <DAP_OPT_MAX_BANDS; band++)
        {
            fprintf(stdout, "  %d", p_dap_params->a_opt_band_gain[ch][band]);
        }
        fprintf(stdout, "\n");
    }
    fprintf(stdout, "  bass_enable                          %s\n", enable_to_str(p_dap_params->bass_enable));
    fprintf(stdout, "  bass_boost                           %d\n", p_dap_params->bass_boost);
    fprintf(stdout, "  bass_cutoff                          %d\n", p_dap_params->bass_cutoff);
    fprintf(stdout, "  bass_width                           %d\n", p_dap_params->bass_width);
    fprintf(stdout, "  regulator_enable                     %s\n", enable_to_str(p_dap_params->regulator_enable));
    fprintf(stdout, "  regulator_mode                       %d\n", p_dap_params->regulator_mode);
    fprintf(stdout, "  regulator_overdrive                  %d\n", p_dap_params->regulator_overdrive);
    fprintf(stdout, "  regulator_timbre                     %d\n", p_dap_params->regulator_timbre);
    fprintf(stdout, "  regulator_distortion                 %d\n", p_dap_params->regulator_distortion);
    fprintf(stdout, "  reg_nb_bands                         %d\n", p_dap_params->reg_nb_bands);
    fprintf(stdout, "  a_reg_band_center[DAP_REG_MAX_BANDS]\n");
    for(band=0; band <DAP_REG_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_reg_band_center[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  a_reg_low_thresholds[DAP_REG_MAX_BANDS]\n");
    for(band=0; band <DAP_REG_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_reg_low_thresholds[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  a_reg_high_thresholds[DAP_REG_MAX_BANDS]\n");
    for(band=0; band <DAP_REG_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_reg_high_thresholds[band]);
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "  a_reg_isolated_bands[DAP_REG_MAX_BANDS]\n");
    for(band=0; band <DAP_REG_MAX_BANDS; band++)
    {
        fprintf(stdout, "  %d", p_dap_params->a_reg_isolated_bands[band]);
    }
    fprintf(stdout, "\n");
    fflush(stdout);
}

static int ms_init_dap_params(dap_params_t *p_dap_params)
{
    memset(p_dap_params, MS_DAP_PARAM_NOT_SET, sizeof(dap_params_t));
    return 0;
}

/*
 * Documentation resides in header file only
 */
void ms_init_params(MS_PARAMS *p_params)
{
  /* set everything to 0 */
  memset(&p_params->ms_init_args, 0, sizeof(p_params->ms_init_args));
  memset(&p_params->ms_runtime_args, 0, sizeof(p_params->ms_runtime_args));
  p_params->h_getparam = 0;

  p_params->ms_init_args.max_num_channels = MS_MAX_MAX_CHANNEL;

  p_params->ms_init_args.extpcm_compressor_profile =  COMPPROF_FILMSTD;
  p_params->ms_init_args.pcm_out_chans[MS_DOWNMIX_OUT_IDX]       = 2;
  p_params->ms_init_args.pcm_out_chans[MS_VIRTUALIZER_OUT_IDX]   = MS_MAX_MAX_CHANNEL;
  p_params->ms_init_args.pcm_out_chans[MS_DAP_SPEAKER_OUT_IDX]   = 3;
  p_params->ms_init_args.pcm_out_chans[MS_DAP_HEADPHONE_OUT_IDX] = 2;

  p_params->ms_init_args.heaac_allow_partial_feed = 1;
  p_params->ms_init_args.verbose_mode = 2;

  p_params->ms_init_args.b_high_risc_precision = 1;

  p_params->ms_runtime_args.heaac_default_dialnorm = 27*4; /* -27dB in dB/4 format */

  p_params->ms_runtime_args.extpcm_num_in_samples = MS_EXTPCM_IN_SAMPLES;
  p_params->ms_runtime_args.extpcm_in_channel_config.acmod = 7; /* DD_3_2 */
  p_params->ms_runtime_args.extpcm_in_channel_config.b_lfe_present = 1;
  p_params->ms_runtime_args.extpcm_in_channel_config.dsurmod = DSURMOD_NO_INDICATION;

  p_params->ms_runtime_args.sys_sounds_num_in_samples = MS_EXTPCM_IN_SAMPLES;
  p_params->ms_runtime_args.sys_sounds_channel_config.acmod = 7; /* DD_3_2 */
  p_params->ms_runtime_args.sys_sounds_channel_config.b_lfe_present = 1;
  p_params->ms_runtime_args.sys_sounds_channel_config.dsurmod = DSURMOD_NO_INDICATION;

  p_params->ms_runtime_args.app_sounds_num_in_samples = MS_EXTPCM_IN_SAMPLES;
  p_params->ms_runtime_args.app_sounds_channel_config.acmod = 7; /* DD_3_2 */
  p_params->ms_runtime_args.app_sounds_channel_config.b_lfe_present = 1;
  p_params->ms_runtime_args.app_sounds_channel_config.dsurmod = DSURMOD_NO_INDICATION;

  p_params->ms_runtime_args.ddplus_outlfe = 1;
  p_params->ms_runtime_args.ddplus_outmode = 7;
  p_params->ms_runtime_args.drc_boost_fac_2ch = MS_MAX_DRC_BOOST;
  p_params->ms_runtime_args.drc_boost_fac_mc = MS_MAX_DRC_BOOST;
  p_params->ms_runtime_args.drc_cut_fac_2ch = MS_MAX_DRC_CUT;
  p_params->ms_runtime_args.drc_cut_fac_mc = MS_MAX_DRC_CUT;

  p_params->ms_runtime_args.heaac_mixing_mode = 1;  /* blocking by default */
  p_params->ms_runtime_args.multichannel_enable = 1; /* enable multichannel output per default */

  p_params->ms_init_args.dbg_output_mask = 0;

  ms_init_dap_params(&p_params->ms_runtime_args.dap_parameter);
}


/*
 * Documentation resides in header file only
 */
MS_RETVAL ms_parse_command_line(int argc, const char *argv[], MS_USE_CASE *p_use_case, MS_PARAMS *p_params, int *err_code)
{
  /*** BEGIN VARIABLE DECLARATIONS ***/

  /* Values read from switches */
  const char *gp_string;
  long int gp_value = 0;

  DLB_GETPARAM_RETVAL gp_err;

  /*** END VARIABLE DECLARATIONS ***/

  /* Initialize GetParam */
  if(argc > 1) {
    gp_err = dlb_getparam_parse(p_params->h_getparam, argc, argv);
    if (gp_err != DLB_GETPARAM_OK){
      ms_show_banner();
      ms_show_usage_brief();
      return MS_HELPTEXT_REQUESTED;
    }
  }
  else{
    fprintf(stdout, "get input from textfile\n");
  }

  /* Detect the verbosity  mode */
  gp_err = dlb_getparam_int(p_params->h_getparam, "v", &gp_value, MS_MIN_VERBOSITY, MS_MAX_VERBOSITY);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_params->ms_init_args.verbose_mode = (int16_t)gp_value;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    ms_show_banner();
    *err_code = MS_ERR_INVALID_VERBOSE_MODE;
    return MS_INVALID_PARAM;
  }

  if(p_params->ms_init_args.verbose_mode){
    ms_show_banner();
  }

  /* Start by searching for the -h flag */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "h", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
    if (strcmp(gp_string, "ddplus") == 0)
    {
      ms_show_usage_common_switches();
      ms_show_usage_context_ddplus();
    }
    else if (strcmp(gp_string, "pcm") == 0)
    {
      ms_show_usage_common_switches();
      ms_show_usage_context_pcm();
    }
    else if (strcmp(gp_string, "heaac") == 0)
    {
      ms_show_usage_common_switches();
      ms_show_usage_context_heaac();
    }
    else if (strcmp(gp_string, "dap") == 0)
    {
      ms_show_usage_context_dap();
    }
    else if (strcmp(gp_string, "lc") == 0)
    {
      ms_show_usage_lc();
    }
    else
    {
      ms_show_usage_full();
    }
    *err_code = 0;
    return MS_HELPTEXT_REQUESTED;
  }
  else if (gp_err == DLB_GETPARAM_NO_VALUE)
  {
      ms_show_usage_full();
      *err_code = 0;
      return MS_HELPTEXT_REQUESTED;
  }

  /* Check if the Processing Graph shall be initialized to low complexity mode */
  gp_err = dlb_getparam_bool(p_params->h_getparam, "lc", &p_use_case->b_low_complexity);

  if (gp_err != DLB_GETPARAM_OK)
  {
      return MS_INVALID_PARAM;
  }

  /* Read the name of the main file */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "im", &p_params->ms_init_args.input_filename[0], MS_MAX_STR_LEN);

  if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_MISSING_INPUT_MAIN;
    return MS_OPEN_FILE_ERROR;
  }
  else if (gp_err != DLB_GETPARAM_OK)
  {
    *err_code = MS_ERR_INVALID_INPUT_FILE;
    return MS_OPEN_FILE_ERROR;
  }
  else
  {
    /* Determine the extension and set the appropriate flag and use case */
    if ( (ms_validate_extension(p_params->ms_init_args.input_filename[0], ".ec3") == 0) ||
        (ms_validate_extension(p_params->ms_init_args.input_filename[0], ".ac3") == 0) )
    {
      p_use_case->input_type = DOLBY_DIGITAL_PLUS;
    }
    else if (ms_validate_extension(p_params->ms_init_args.input_filename[0], ".wav") == 0)
    {
      p_use_case->input_type = EXTERNAL_PCM;
    }
    else if ( (ms_validate_extension(p_params->ms_init_args.input_filename[0], "adts") == 0) ||
              (ms_validate_extension(p_params->ms_init_args.input_filename[0], "loas") == 0) ||
              (ms_validate_extension(p_params->ms_init_args.input_filename[0], ".mp4") == 0) )
    {
      p_use_case->input_type = HE_AAC;
      if (ms_validate_extension(p_params->ms_init_args.input_filename[0], ".mp4") == 0)
      {
        p_use_case->b_file_playback = 1;
      }
      if (p_use_case->b_low_complexity)
      {
          *err_code = MS_ERR_LC_UNSUPPORTED_HEAAC;
          return MS_OPEN_FILE_ERROR;
      }
    }
    else
    {
      *err_code = MS_ERR_INVALID_INPUT_FILE_TYPE;
      return MS_INVALID_PARAM;
    }
  }

  /* Now, read the remaining common input and output parameters */

  /* Associated input */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "ia", &p_params->ms_init_args.input_filename[1], MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM)
  {
    p_use_case->b_dual_input = 0;
  }
  else if (gp_err !=  DLB_GETPARAM_OK)
  {
    *err_code = MS_ERR_INVALID_INPUT_FILE;
    return MS_OPEN_FILE_ERROR;
  }
  else if (gp_err == DLB_GETPARAM_OK)
  {
    p_use_case->b_dual_input = 1;

    if(p_use_case->b_low_complexity)
    {
        *err_code = MS_ERR_LC_UNSUPPORTED_ASSOC_MIX;
        return MS_OPEN_FILE_ERROR;
    }

    /* Ensure that the main and associated inputs have the same file extensions */
    if (p_use_case->input_type == DOLBY_DIGITAL_PLUS)
    {
      if ((ms_validate_extension(p_params->ms_init_args.input_filename[1], ".ec3") != 0) &&
          (ms_validate_extension(p_params->ms_init_args.input_filename[1], ".ac3") != 0))
      {
        *err_code = MS_ERR_INCOMPATIBLE_INPUTS;
        return MS_OPEN_FILE_ERROR;
      }
    }
    else if (p_use_case->input_type == EXTERNAL_PCM)
    {
        /* no dual decoding for external PCM */
        *err_code = MS_ERR_EXTPCM_ASSOCIATED_INPUT;
        return MS_OPEN_FILE_ERROR;
    }
    else if ((p_use_case->input_type == HE_AAC) && (!p_use_case->b_file_playback))
    {
      if ((ms_validate_extension(p_params->ms_init_args.input_filename[1], "adts") != 0) &&
          (ms_validate_extension(p_params->ms_init_args.input_filename[1], ".aac")  != 0) &&
          (ms_validate_extension(p_params->ms_init_args.input_filename[1], "loas") != 0))
      {
        *err_code = MS_ERR_INCOMPATIBLE_INPUTS;
        return MS_OPEN_FILE_ERROR;
      }
    }
    else if ((p_use_case->input_type == HE_AAC) && (p_use_case->b_file_playback))
    {
      if (ms_validate_extension(p_params->ms_init_args.input_filename[1], ".mp4") != 0)
      {
        *err_code = MS_ERR_INCOMPATIBLE_INPUTS;
        return MS_OPEN_FILE_ERROR;
      }
    }
  }

  /* System sounds input */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "is", &p_params->ms_init_args.input_filename[MS_SYSTEM_SOUNDS_IDX], MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM)
  {
    p_use_case->b_system_sounds = 0;
  }
  else if (gp_err !=  DLB_GETPARAM_OK)
  {
    *err_code = MS_ERR_INVALID_INPUT_FILE;
    return MS_OPEN_FILE_ERROR;
  }
  else if (gp_err == DLB_GETPARAM_OK)
  {
    p_use_case->b_system_sounds = 1;
    if (ms_validate_extension(p_params->ms_init_args.input_filename[MS_SYSTEM_SOUNDS_IDX], ".wav") != 0)
    {
        *err_code = MS_ERR_INVALID_PCM_INPUT_FILE_EXT;
        return MS_OPEN_FILE_ERROR;
    }
  }

  /* Application sounds input */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "ias", &p_params->ms_init_args.input_filename[MS_APPLICATION_SOUNDS_IDX], MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM)
  {
    p_use_case->b_app_sounds = 0;
  }
  else if (gp_err !=  DLB_GETPARAM_OK)
  {
    *err_code = MS_ERR_INVALID_INPUT_FILE;
    return MS_OPEN_FILE_ERROR;
  }
  else if (gp_err == DLB_GETPARAM_OK)
  {
    p_use_case->b_app_sounds = 1;
    if(p_use_case->b_low_complexity)
    {
        *err_code = MS_ERR_LC_UNSUPPORTED_APP_SOUND_IN;
        return MS_OPEN_FILE_ERROR;
    }
    if (ms_validate_extension(p_params->ms_init_args.input_filename[MS_APPLICATION_SOUNDS_IDX], ".wav") != 0)
    {
        *err_code = MS_ERR_INVALID_PCM_INPUT_FILE_EXT;
        return MS_OPEN_FILE_ERROR;
    }
  }

  /* DD output */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "od", &p_params->ms_init_args.bitstream_output_filename[MS_DD_OUT_IDX], MS_MAX_STR_LEN);
  if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err !=  DLB_GETPARAM_OK))
  {
    *err_code = MS_ERR_INVALID_DD_OUTPUT_FILE;
    return MS_INVALID_PARAM;
  }
  else if (gp_err == DLB_GETPARAM_OK)
  {
    if (p_use_case->b_low_complexity && (p_use_case->input_type == EXTERNAL_PCM))
    {
      *err_code = MS_ERR_LC_UNSUPPORTED_ENCODER;
      return MS_INVALID_PARAM;
    }
    /* Ensure that the DD output file has the proper extension */
    if (ms_validate_extension(p_params->ms_init_args.bitstream_output_filename[MS_DD_OUT_IDX], ".ac3") != 0)
    {
      *err_code = MS_ERR_INVALID_DD_OUTPUT_FILE;
      return MS_INVALID_PARAM;
    }
  }

  /* DD+ output */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "odp", &p_params->ms_init_args.bitstream_output_filename[MS_DDP_OUT_IDX], MS_MAX_STR_LEN);
  if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err !=  DLB_GETPARAM_OK))
  {
    *err_code = MS_ERR_INVALID_DDP_OUTPUT_FILE;
    return MS_INVALID_PARAM;
  }
  else if (gp_err == DLB_GETPARAM_OK)
  {
    if (p_use_case->b_low_complexity && (p_use_case->input_type == EXTERNAL_PCM))
    {
      *err_code = MS_ERR_LC_UNSUPPORTED_ENCODER;
      return MS_INVALID_PARAM;
    }
    /* Ensure that the DD+ output file has the proper extension */
    if (ms_validate_extension(p_params->ms_init_args.bitstream_output_filename[MS_DDP_OUT_IDX], ".ec3") != 0)
    {
      *err_code = MS_ERR_INVALID_DDP_OUTPUT_FILE;
      return MS_INVALID_PARAM;
    }
  }

  if(p_params->ms_init_args.bitstream_output_filename[MS_DD_OUT_IDX] && p_params->ms_init_args.bitstream_output_filename[MS_DDP_OUT_IDX])
  {
      p_use_case->bs_output_mode = MS_BS_OUTPUT_MODE_SIM_DDP_DD;
  }
  else if(p_params->ms_init_args.bitstream_output_filename[MS_DD_OUT_IDX])
  {
      p_use_case->bs_output_mode = MS_BS_OUTPUT_MODE_DD;
  }
  else if(p_params->ms_init_args.bitstream_output_filename[MS_DDP_OUT_IDX])
  {
      p_use_case->bs_output_mode = MS_BS_OUTPUT_MODE_DDP;
  }
  else
  {
      p_use_case->bs_output_mode = MS_BS_OUTPUT_MODE_NONE;
  }


  /* 2-channel main output */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "oms", &p_params->ms_init_args.pcm_output_filename[MS_DOWNMIX_OUT_IDX], MS_MAX_STR_LEN);
  if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err !=  DLB_GETPARAM_OK))
  {
    *err_code = MS_ERR_INVALID_2CH_OUTPUT_FILE;
    return MS_INVALID_PARAM;
  }
  else if (gp_err == DLB_GETPARAM_OK)
  {
    if (p_use_case->b_low_complexity && (p_use_case->input_type == EXTERNAL_PCM))
    {
      *err_code = MS_ERR_LC_UNSUPPORTED_DMX;
      return MS_INVALID_PARAM;
    }

    /* Ensure that the 2-channel main output file has the proper extension */
    if (ms_validate_extension(p_params->ms_init_args.pcm_output_filename[MS_DOWNMIX_OUT_IDX], ".wav") != 0)
    {
      *err_code = MS_ERR_INVALID_2CH_OUTPUT_FILE;
      return MS_INVALID_PARAM;
    }

  }

  /* Output Parameter Text File */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "ot", &p_params->ms_init_args.pcmr_output_filename, MS_MAX_STR_LEN);
  if (gp_err !=  DLB_GETPARAM_OK)
  {
      p_params->ms_init_args.pcmr_output_filename = NULL;
  }
  else if (p_use_case->b_low_complexity)
  {
      *err_code = MS_ERR_LC_UNSUPPORTED_TXT_OUT;
      return MS_INVALID_PARAM;
  }

  if(p_use_case->input_type == DOLBY_DIGITAL_PLUS)
  {
    gp_err = dlb_getparam_int(p_params->h_getparam, "at", &gp_value, MS_MIN_ASSOC_SUBSTRM, MS_MAX_ASSOC_SUBSTRM);
    if((gp_err == DLB_GETPARAM_UNDEFINED_PARAM) && !p_use_case->b_dual_input){
      p_use_case->b_mainonly = 1;
    }
    else if(gp_err == DLB_GETPARAM_OK){
      p_params->ms_runtime_args.ddplus_associated_substream = (int16_t)gp_value;
    }
    else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
    {
      *err_code = MS_ERR_OUTOFRANGE_ASSOCIATED_SUBSTREAM;
      return MS_INVALID_PARAM;
    }
    else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
    {
      *err_code = MS_ERR_INVALID_ASSOCIATED_SUBSTREAM;
      return MS_INVALID_PARAM;
    }
  }

  /* Main multichannel output */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "om", &p_params->ms_init_args.pcm_output_filename[MS_VIRTUALIZER_OUT_IDX], MS_MAX_STR_LEN);
  if((gp_err !=  DLB_GETPARAM_OK) && (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
    *err_code = MS_ERR_INVALID_OUTPUT_FILE;
    return MS_INVALID_PARAM;
  }
  else if(gp_err == DLB_GETPARAM_OK)
  {
    if(p_use_case->b_low_complexity)
    {
      *err_code = MS_ERR_LC_UNSUPPORTED_MC_OUT;
      return MS_INVALID_PARAM;
    }
    /* Ensure that the main output file has the proper extension */
    if (ms_validate_extension(p_params->ms_init_args.pcm_output_filename[MS_VIRTUALIZER_OUT_IDX], ".wav") != 0)
    {
      *err_code = MS_ERR_INVALID_OUTPUT_FILE_TYPE;
      return MS_INVALID_PARAM;
    }
  }

  /* DAP speaker output (2.1 channels) */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "o_dap_speaker", &p_params->ms_init_args.pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX], MS_MAX_STR_LEN);
  if((gp_err !=  DLB_GETPARAM_OK) && (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
    *err_code = MS_ERR_INVALID_OUTPUT_FILE;
    return MS_INVALID_PARAM;
  }
  else if(gp_err == DLB_GETPARAM_OK)
  {
    p_params->ms_init_args.dap_output_mode |= MS_DAP_SPEAKER_OUT_ACTIVE;
    /* Ensure that the DAP speaker output file has the proper extension */
    if (ms_validate_extension(p_params->ms_init_args.pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX], ".wav") != 0)
    {
      *err_code = MS_ERR_INVALID_OUTPUT_FILE_TYPE;
      return MS_INVALID_PARAM;
    }
  }

  /* DAP headphone output (2 channels) */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "o_dap_headphone", &p_params->ms_init_args.pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX], MS_MAX_STR_LEN);
  if((gp_err !=  DLB_GETPARAM_OK) && (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
    *err_code = MS_ERR_INVALID_OUTPUT_FILE;
    return MS_INVALID_PARAM;
  }
  else if(gp_err == DLB_GETPARAM_OK)
  {
    p_params->ms_init_args.dap_output_mode |= MS_DAP_HEADPHONE_OUT_ACTIVE;
    /* Ensure that the DAP headphone output file has the proper extension */
    if (ms_validate_extension(p_params->ms_init_args.pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX], ".wav") != 0)
    {
      *err_code = MS_ERR_INVALID_OUTPUT_FILE_TYPE;
      return MS_INVALID_PARAM;
    }
  }


  /* Maximum number of channels in the processing chain */
  gp_err = dlb_getparam_int(p_params->h_getparam, "max_channels", &gp_value, MS_MIN_MAX_CHANNEL, MS_MAX_MAX_CHANNEL);
  if (gp_err == DLB_GETPARAM_OK)
  {
      if((int)gp_value == 7)
      {
          *err_code = MS_ERR_OUTOFRANGE_MAX_NUM_CHANNEL;
	  return MS_INVALID_PARAM;
      }
      p_params->ms_init_args.max_num_channels = (int)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_MAX_NUM_CHANNEL;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_MAX_NUM_CHANNEL_VALUE;
	  return MS_INVALID_PARAM;
  }

  if (p_use_case->b_low_complexity && (p_params->ms_init_args.max_num_channels > MS_MIN_MAX_CHANNEL))
  {
      fprintf(stderr,"Warning: Low Complexity processing mode is limited to six channels. Parameter max_channels is set to six.\n");
      p_params->ms_init_args.max_num_channels = MS_MIN_MAX_CHANNEL;
  }

  /* downmix 7.1 to 5.1 for the multichannel PCM output */
  gp_err = dlb_getparam_int(p_params->h_getparam, "mc_5_1_dmx", &gp_value, 0, 1);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_params->ms_runtime_args.b_mc_5_1_dmx = (int)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_MC_5_1_DMX;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_MC_5_1_DMX;
	  return MS_INVALID_PARAM;
  }


  /* Output WAV file bit-depth */
  gp_err = dlb_getparam_int(p_params->h_getparam, "w", &gp_value, MS_MIN_WAVFILEBITDEPTH, MS_MAX_WAVFILEBITDEPTH);
  if (gp_err == DLB_GETPARAM_OK)
  {
      if((int)gp_value % 8)
      {
	  *err_code = MS_ERR_OUTOFRANGE_WAVFILEBITDEPTH;
	  return MS_INVALID_PARAM;
      }

      p_params->ms_init_args.wav_bit_depth = (int)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_WAVFILEBITDEPTH;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_WAVFILEBITDEPTH;
	  return MS_INVALID_PARAM;
  }


  /* precision of PCM data (SFRACT or LFRACT) */
  gp_err = dlb_getparam_int(p_params->h_getparam, "p", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_params->ms_init_args.b_high_risc_precision = (int)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_PCM_PREC_FLAG;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_PCM_PREC_FLAG;
	  return MS_INVALID_PARAM;
  }



  /* DAP mode */
  gp_err = dlb_getparam_int(p_params->h_getparam, "dap_init_mode", &gp_value, MS_DAP_NO_PROC, MS_DAP_SI_PROC);
  if (gp_err == DLB_GETPARAM_OK)
  {
      p_use_case->dap_init_mode = (ms_dap_mode_t)gp_value;

  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
      *err_code = MS_ERR_OUTOFRANGE_DAP_INIT_MODE;
      return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_DAP_INIT_MODE;
      return MS_INVALID_PARAM;
  }
  else if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM && p_use_case->b_low_complexity)
  {
      p_use_case->dap_init_mode = MS_DAP_SI_PROC;
  }
  if (p_use_case->b_low_complexity && (p_use_case->dap_init_mode != MS_DAP_SI_PROC))
  {
      *err_code = MS_ERR_LC_UNSUPPORTED_DAP_INIT_MODE;
      return MS_INVALID_PARAM;
  }

  /* DAP mode */
  gp_err = dlb_getparam_int(p_params->h_getparam, "b_dap_vb_enable", &gp_value, 0, 1);
  if (gp_err == DLB_GETPARAM_OK)
  {
      p_use_case->b_dap_vb_enable = (ms_dap_mode_t)gp_value;

  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
      *err_code = MS_ERR_OUTOFRANGE_DAP_VB_ENABLE;
      return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_DAP_VB_ENABLE;
      return MS_INVALID_PARAM;
  }

  /* Read switches specific to external PCM reencoding */
  if ((!p_use_case->b_dual_input) && (p_use_case->input_type == EXTERNAL_PCM))
  {
      gp_err = dlb_getparam_int(p_params->h_getparam, "rp", &gp_value, MS_MIN_COMP_PROF, MS_MAX_COMP_PROF);
      if (gp_err == DLB_GETPARAM_OK)
      {
          if (p_use_case->b_low_complexity)
          {
              *err_code = MS_ERR_LC_UNSUPPORTED_ENCODER;
              return MS_INVALID_PARAM;
          }
          p_params->ms_init_args.extpcm_compressor_profile = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
          *err_code = MS_ERR_OUTOFRANGE_PCM_COMP_PROFILE;
          return MS_INVALID_PARAM;
      }
      else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
      {
          *err_code = MS_ERR_INVALID_COMPRESSOR_PROFILE;
          return MS_INVALID_PARAM;
      }
  }

  /* Read switches specific to HE-AAC */
  if (p_use_case->input_type == HE_AAC)
  {
    if (p_use_case->b_dual_input)
    {
        gp_err = dlb_getparam_int(p_params->h_getparam, "as", &gp_value, 0, 1);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_use_case->b_restricted_ad = (int)gp_value;
      }
      else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
      {
        *err_code = MS_ERR_ASSOCIATED_2CH_RESTRICT;
        return MS_INVALID_PARAM;
      }
    }

    gp_err = dlb_getparam_int(p_params->h_getparam, "dn", &gp_value, MS_MIN_DIALNORM, MS_MAX_DIALNORM);
    if (gp_err == DLB_GETPARAM_OK)
    {
		p_params->ms_runtime_args.heaac_default_dialnorm = (int16_t)gp_value;
    }
    else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
    {
      *err_code = MS_ERR_OUTOFRANGE_HEAAC_DIALNORM;
      return MS_INVALID_PARAM;
    }
    else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
    {
      *err_code = MS_ERR_INVALID_DIALNORM_VALUE;
      return MS_INVALID_PARAM;
    }

    gp_err = dlb_getparam_int(p_params->h_getparam, "t", &gp_value, MS_MIN_TRANSFMT, MS_MAX_TRANSFMT);
    if (gp_err == DLB_GETPARAM_OK)
    {
      p_params->ms_runtime_args.heaac_transport_format = (int16_t)gp_value;
    }
    else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
    {
      *err_code = MS_ERR_OUTOFRANGE_HEAAC_TRANSFMT;
      return MS_INVALID_PARAM;
    }
    else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
    {
      *err_code = MS_ERR_INVALID_TRANSPORT_FORMAT;
      return MS_INVALID_PARAM;
    }
  }

  /* Detect the evaluation  mode */
  gp_err = dlb_getparam_int(p_params->h_getparam, "eval", &gp_value, 0, 1);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_use_case->b_eval_mode = (int16_t)gp_value;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_EVAL_MODE;
    return MS_INVALID_PARAM;
  }

  /* Debug output mask */
  gp_err = dlb_getparam_int(p_params->h_getparam, "dbgout", &gp_value, 0, 0x0FFFF);
  if (gp_err == DLB_GETPARAM_OK)
  {
      p_params->ms_init_args.dbg_output_mask = (uint16_t)gp_value;
  }
  else if ((gp_err == DLB_GETPARAM_OUT_OF_RANGE) || (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
      *err_code = MS_ERR_UNKNOWN_ERRCODE;
      return MS_INVALID_PARAM;
  }

#ifdef MS12_PLAYER_CMD_FOR_DDPENC_LISTENING_TEST
    gp_err = dlb_getparam_int(p_params->h_getparam, "ddpenc_listen_acmod", &gp_value, 0, 21);
    if (gp_err == DLB_GETPARAM_OK)
    {
        p_use_case->b_system_sounds = (int)gp_value;
    }
    gp_err = dlb_getparam_int(p_params->h_getparam, "ddpenc_listen_datarate", &gp_value, 0, 1536);
    if (gp_err == DLB_GETPARAM_OK)
    {
        p_use_case->b_app_sounds = (int)gp_value;
    }
    gp_err = dlb_getparam_int(p_params->h_getparam, "ddpenc_listen_encodermode", &gp_value, 5, 13);
    if (gp_err == DLB_GETPARAM_OK)
    {
        p_use_case->b_eval_mode = (int)gp_value;
    }
#endif

  return ms_get_runtime_params(p_use_case, &p_params->ms_runtime_args, p_params->h_getparam, err_code);
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_get_runtime_params(MS_USE_CASE *p_use_case,
                                       MS_RUNTIME_PARAMS *p_runtime_params,
                                       DLB_GETPARAM_HANDLE h_getparam,
                                       int *err_code)
{

  DLB_GETPARAM_RETVAL gp_err;
  long int gp_value;

  /* Dual-mono mode */
  gp_err = dlb_getparam_int(h_getparam, "u", &gp_value, MS_MIN_DUALMONO, MS_MAX_DUALMONO);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_runtime_params->dual_mono = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
  *err_code = MS_ERR_OUTOFRANGE_DUALMONO;
  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DUALMONO_MODE;
    return MS_INVALID_PARAM;
  }

  /* Boost factor - multi-channel */
  gp_err = dlb_getparam_int(h_getparam, "b", &gp_value, MS_MIN_DRC_BOOST, MS_MAX_DRC_BOOST);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_runtime_params->drc_boost_fac_mc = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
  *err_code = MS_ERR_OUTOFRANGE_DRC_BOOST;
  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DRC_BOOST_FACTOR;
    return MS_INVALID_PARAM;
  }

  /* Boost factor - 2-ch */
  gp_err = dlb_getparam_int(h_getparam, "bs", &gp_value, MS_MIN_DRC_BOOST, MS_MAX_DRC_BOOST);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_runtime_params->drc_boost_fac_2ch = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
    *err_code = MS_ERR_OUTOFRANGE_DRC_BOOST;
    return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DRC_BOOST_FACTOR;
    return MS_INVALID_PARAM;
  }

  /* Cut factor - multi-channel */
  gp_err = dlb_getparam_int(h_getparam, "c", &gp_value, MS_MIN_DRC_CUT, MS_MAX_DRC_CUT);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_runtime_params->drc_cut_fac_mc = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
    *err_code = MS_ERR_OUTOFRANGE_DRC_CUT;
    return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DRC_CUT_FACTOR;
    return MS_INVALID_PARAM;
  }

  /* Cut factor - 2-channel */
  gp_err = dlb_getparam_int(h_getparam, "cs", &gp_value, MS_MIN_DRC_BOOST, MS_MAX_DRC_BOOST);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_runtime_params->drc_cut_fac_2ch = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
    *err_code = MS_ERR_OUTOFRANGE_DRC_CUT;
    return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DRC_CUT_FACTOR;
    return MS_INVALID_PARAM;
  }

  /* Channel configuration of the system sounds input */
  gp_err = dlb_getparam_int(h_getparam, "chs", &gp_value, MS_MIN_SYS_SOUNDS_ACMOD, MS_MAX_SYS_SOUNDS_ACMOD);
  if (gp_err == DLB_GETPARAM_OK)
  {
      if (p_use_case->b_low_complexity && (gp_value > 2))
      {
	    *err_code = MS_ERR_LC_UNSUPPORTED_SYSSND_CHCFG;
	    return MS_INVALID_PARAM;
      }
	  p_runtime_params->sys_sounds_channel_config.acmod = (uint16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_SYS_SOUNDS_ACMOD;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_SYS_SOUNDS_ACMOD;
	  return MS_INVALID_PARAM;
  }
  else if ((gp_err == DLB_GETPARAM_UNDEFINED_PARAM) && p_use_case->b_low_complexity)
  {
	  p_runtime_params->sys_sounds_channel_config.acmod = 2;
  }

  /* LFE presence - system sounds input */
  gp_err = dlb_getparam_int(h_getparam, "ls", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_runtime_params->sys_sounds_channel_config.b_lfe_present = (uint16_t)gp_value;
  }
  else if ((gp_err == DLB_GETPARAM_OUT_OF_RANGE) || (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
	  *err_code = MS_ERR_INVALID_LFE;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM && p_use_case->b_low_complexity)
  {
      p_runtime_params->sys_sounds_channel_config.b_lfe_present = 0;
  }
  if (p_use_case->b_low_complexity && p_runtime_params->sys_sounds_channel_config.b_lfe_present)
  {
	  *err_code = MS_ERR_LC_UNSUPPORTED_SYSSND_CHCFG;
	  return MS_INVALID_PARAM;
  }

  /* Channel configuration of the application sounds input */
  gp_err = dlb_getparam_int(h_getparam, "chas", &gp_value, MS_MIN_APP_SOUNDS_ACMOD, MS_MAX_APP_SOUNDS_ACMOD);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_runtime_params->app_sounds_channel_config.acmod = (uint16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_APP_SOUNDS_ACMOD;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_APP_SOUNDS_ACMOD;
	  return MS_INVALID_PARAM;
  }

  /* LFE presence - application sounds input */
  gp_err = dlb_getparam_int(h_getparam, "las", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_runtime_params->app_sounds_channel_config.b_lfe_present = (uint16_t)gp_value;
  }
  else if ((gp_err == DLB_GETPARAM_OUT_OF_RANGE) || (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
	  *err_code = MS_ERR_INVALID_LFE;
	  return MS_INVALID_PARAM;
  }

  /* Channel configuration of the external PCM input */
  gp_err = dlb_getparam_int(h_getparam, "chp", &gp_value, MS_MIN_EXTPCM_ACMOD, MS_MAX_EXTPCM_ACMOD);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_runtime_params->extpcm_in_channel_config.acmod = (uint16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
	  *err_code = MS_ERR_OUTOFRANGE_EXTPCM_ACMOD;
	  return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
	  *err_code = MS_ERR_INVALID_EXTPCM_ACMOD;
	  return MS_INVALID_PARAM;
  }

  /* LFE presence - external PCM input */
  gp_err = dlb_getparam_int(h_getparam, "lp", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
  if (gp_err == DLB_GETPARAM_OK)
  {
	  p_runtime_params->extpcm_in_channel_config.b_lfe_present = (uint16_t)gp_value;
  }
  else if ((gp_err == DLB_GETPARAM_OUT_OF_RANGE) || (gp_err != DLB_GETPARAM_UNDEFINED_PARAM))
  {
	  *err_code = MS_ERR_INVALID_LFE;
	  return MS_INVALID_PARAM;
  }

  /* Downmix mode */
  gp_err = dlb_getparam_int(h_getparam, "dmx", &gp_value, MS_MIN_DMX, MS_MAX_DMX);
  if (gp_err == DLB_GETPARAM_OK)
  {
    /* DD+ case: Only modes 0 (Lt/Rt) and 1 (Lo/Ro) are valid */
    if ((p_use_case->input_type == DOLBY_DIGITAL_PLUS) && ((int16_t)gp_value == MS_DMX_TYPE_ARIB))
    {
      *err_code = MS_ERR_INVALID_DDP_2CH_MODE;
      return MS_INVALID_PARAM;
    }
    else if ((p_use_case->input_type == EXTERNAL_PCM) && ((int16_t)gp_value == MS_DMX_TYPE_ARIB))
    {
      *err_code = MS_ERR_INVALID_EXTPCM_2CH_MODE;
      return MS_INVALID_PARAM;
    }
    else
    {
      p_runtime_params->downmix_type = (int16_t)gp_value;
    }
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
    *err_code = MS_ERR_OUTOFRANGE_ST_DOWNMIX;
    return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DMX_MODE;
    return MS_INVALID_PARAM;
  }

  /* DRC mode */
  gp_err = dlb_getparam_int(h_getparam, "drc", &gp_value, MS_MIN_DRC_MODE, MS_MAX_DRC_MODE);
  if (gp_err == DLB_GETPARAM_OK)
  {
    p_runtime_params->drc_mode = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
    *err_code = MS_ERR_OUTOFRANGE_DRC_MODE;
    return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_DRC_MODE;
    return MS_INVALID_PARAM;
  }

  /* Associated substream selection - DD+ use-case only */
  if(p_use_case->input_type == DOLBY_DIGITAL_PLUS)
  {
    /* only allow runtime switching of substream if we have substream selection used from the beginning */
    if(p_runtime_params->ddplus_associated_substream)
    {
      gp_err = dlb_getparam_int(h_getparam, "at", &gp_value, MS_MIN_ASSOC_SUBSTRM, MS_MAX_ASSOC_SUBSTRM);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_runtime_params->ddplus_associated_substream = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
        *err_code = MS_ERR_OUTOFRANGE_ASSOCIATED_SUBSTREAM;
        return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
        *err_code = MS_ERR_INVALID_ASSOCIATED_SUBSTREAM;
        return MS_INVALID_PARAM;
      }
    }
  }

  p_runtime_params->b_sys_app_sound_mixing = 1; /* set mixing enabled as default */
  gp_err = dlb_getparam_int(h_getparam, "xs", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
  if (gp_err == DLB_GETPARAM_OK)
  {
      p_runtime_params->b_sys_app_sound_mixing = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
      *err_code = MS_ERR_OUTOFRANGE_SYSSOUND_MIX;
      return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_SYSSOUND_MIXING_VALUE;
      return MS_INVALID_PARAM;
  }

  /* Associated program mixing and main/associated program user-balance */
  if (((!p_use_case->b_mainonly && (p_use_case->input_type == DOLBY_DIGITAL_PLUS)) || p_use_case->b_dual_input))
  {
    p_runtime_params->associated_audio_mixing = 1; /* set mixing enabled as default */
    gp_err = dlb_getparam_int(h_getparam, "xa", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
    if (gp_err == DLB_GETPARAM_OK)
    {
      p_runtime_params->associated_audio_mixing = (int16_t)gp_value;
    }
    else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
    {
      *err_code = MS_ERR_OUTOFRANGE_ASSOC_MIX;
      return MS_INVALID_PARAM;
    }
    else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
    {
      *err_code = MS_ERR_INVALID_ASSOC_MIXING_VALUE;
      return MS_INVALID_PARAM;
    }

    gp_err = dlb_getparam_int(h_getparam, "xu", &gp_value, MS_MIN_USERBAL, MS_MAX_USERBAL);
    if (gp_err == DLB_GETPARAM_OK)
    {
      p_runtime_params->user_balance_adjustment = (int16_t)gp_value;
    }
    else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
    {
      *err_code = MS_ERR_OUTOFRANGE_USER_BALANCE;
      return MS_INVALID_PARAM;
    }
    else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
    {
      *err_code = MS_ERR_INVALID_USER_BALANCE_VALUE;
      return MS_INVALID_PARAM;
    }
  }

  gp_err = dlb_getparam_int(h_getparam, "xsu", &gp_value, MS_MIN_USERBAL, MS_MAX_USERBAL);
  if (gp_err == DLB_GETPARAM_OK)
  {
      p_runtime_params->mixer_balance_adjustment = (int16_t)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
      *err_code = MS_ERR_OUTOFRANGE_MIXER_BALANCE;
      return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_MIXER_BALANCE_VALUE;
      return MS_INVALID_PARAM;
  }


  /* get DAP specific parameter */
  if(ms_get_dap_params(&p_runtime_params->dap_parameter, h_getparam, p_use_case->b_dap_vb_enable, err_code))
  {
      return MS_INVALID_PARAM;
  }

  /* Now we are at the very end of command line parsing. Check for unhandled parameters */
  if(dlb_getparam_left(h_getparam))
  {
    const char *name, *value;
    dlb_getparam_nextremaining(h_getparam, &name, &value);
    fprintf(stderr, "\n\nInapplicable or unsupported parameter - \"%s\"\n\n", name);
    *err_code = MS_ERR_UNSUPPORTED_PARAMETER;
    return MS_INVALID_PARAM;
  }

  return MS_OK;
}

static MS_RETVAL ms_get_dap_params(dap_params_t *p_dap_params, DLB_GETPARAM_HANDLE h_getparam, int b_dap_vb_enable, int *err_code)
{
    /* Values read from switches */
    const char *gp_string;
    long int gp_value = 0;
    DLB_GETPARAM_RETVAL gp_err;
    char *tmpstr;
    gp_err = dlb_getparam_int(h_getparam, "dap_calibration_boost", &gp_value, MS_DAP_MIN_VAL, MS_DAP_MAX_VAL);
    if(set_int_check_gp_err(gp_err, gp_value, &p_dap_params->calibration_boost, err_code, MS_ERR_OUTOFRANGE_DAP_PARAM, MS_ERR_INVALID_DAP_VALUE))
        return MS_INVALID_PARAM;
    gp_err = dlb_getparam_int(h_getparam, "dap_surround_decoder_enable", &gp_value, 0, 1);
    if(set_int_check_gp_err(gp_err, gp_value, &p_dap_params->surround_decoder_enable, err_code, MS_ERR_OUTOFRANGE_DAP_PARAM, MS_ERR_INVALID_DAP_VALUE))
        return MS_INVALID_PARAM;

    gp_err = dlb_getparam_int(h_getparam, "dap_dmx", &gp_value, MS_MIN_DAP_DMX_TYPE, MS_MAX_DAP_DMX_TYPE);
    if(set_int_check_gp_err(gp_err, gp_value, &p_dap_params->dmx_type, err_code, MS_ERR_OUTOFRANGE_DAP_DMX_TYPE, MS_ERR_INVALID_DAP_DMX_TYPE))
        return MS_INVALID_PARAM;

    gp_err = dlb_getparam_int(h_getparam, "dap_drc", &gp_value, MS_MIN_DAP_DRC_TYPE, MS_MAX_DAP_DRC_TYPE);
    if(set_int_check_gp_err(gp_err, gp_value, &p_dap_params->drc_type, err_code, MS_ERR_OUTOFRANGE_DAP_DRC_TYPE, MS_ERR_INVALID_DAP_DRC_TYPE))
        return MS_INVALID_PARAM;

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_bass_enhancer", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->bass_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->bass_boost);
        ms_get_int_from_str(&tmpstr, &p_dap_params->bass_cutoff);
        ms_get_int_from_str(&tmpstr, &p_dap_params->bass_width);
    }

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_dialogue_enhancer", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->de_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->de_amount);
        ms_get_int_from_str(&tmpstr, &p_dap_params->de_ducking);
    }
    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_graphic_eq", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->eq_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->eq_nb_bands);

        if(p_dap_params->eq_nb_bands > DAP_GEQ_MAX_BANDS)
        {
            *err_code = MS_ERR_DAP_GEQ_INVALID_NB_BANDS;
            return MS_INVALID_PARAM;
        }

        ms_get_int_array_from_str(&tmpstr, p_dap_params->eq_nb_bands, &p_dap_params->a_geq_band_center[0]);
        ms_get_int_array_from_str(&tmpstr, p_dap_params->eq_nb_bands, &p_dap_params->a_geq_band_target[0]);
    }
    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_ieq", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->ieq_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->ieq_amount);
        ms_get_int_from_str(&tmpstr, &p_dap_params->ieq_nb_bands);
        if(p_dap_params->ieq_nb_bands > DAP_IEQ_MAX_BANDS)
        {
            *err_code = MS_ERR_DAP_IEQ_INVALID_NB_BANDS;
            return MS_INVALID_PARAM;
        }

        ms_get_int_array_from_str(&tmpstr, p_dap_params->ieq_nb_bands, &p_dap_params->a_ieq_band_center[0]);
        ms_get_int_array_from_str(&tmpstr, p_dap_params->ieq_nb_bands, &p_dap_params->a_ieq_band_target[0]);
    }

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_gains", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->postgain);
#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
        ms_get_int_from_str(&tmpstr, &p_dap_params->pregain);
        ms_get_int_from_str(&tmpstr, &p_dap_params->systemgain);
#endif
    }

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_leveler", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->leveler_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->leveler_amount);
        ms_get_int_from_str(&tmpstr, &p_dap_params->leveler_ignore_il);
    }

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_mi_steering", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->mi_ieq_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->mi_dv_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->mi_de_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->mi_surround_enable);
    }
    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_surround_virtualizer", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->virtualizer_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->headphone_reverb);
        ms_get_int_from_str(&tmpstr, &p_dap_params->speaker_angle);
        ms_get_int_from_str(&tmpstr, &p_dap_params->speaker_start);
        ms_get_int_from_str(&tmpstr, &p_dap_params->surround_boost);
    }

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_optimizer", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        int ch;
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->optimizer_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->optimizer_nb_bands);

        if(p_dap_params->optimizer_nb_bands > DAP_OPT_MAX_BANDS)
        {
            *err_code = MS_ERR_DAP_OPT_INVALID_NB_BANDS;
            return MS_INVALID_PARAM;
        }

        ms_get_int_array_from_str(&tmpstr, p_dap_params->optimizer_nb_bands, &p_dap_params->a_opt_band_center_freq[0]);

        for(ch=0; ch<DAP_MAX_CHANNELS; ch++)
        {
            if(!strncmp(tmpstr, "NULL", 4))
            {
                tmpstr += 5;
            }
            else
            {
                ms_get_int_array_from_str(&tmpstr, p_dap_params->optimizer_nb_bands, &p_dap_params->a_opt_band_gain[ch][0]);
            }
        }
    }
    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_regulator", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        tmpstr = (char *)gp_string;
        ms_get_int_from_str(&tmpstr, &p_dap_params->regulator_enable);
        ms_get_int_from_str(&tmpstr, &p_dap_params->regulator_mode);
        ms_get_int_from_str(&tmpstr, &p_dap_params->regulator_overdrive);
        ms_get_int_from_str(&tmpstr, &p_dap_params->regulator_timbre);
        ms_get_int_from_str(&tmpstr, &p_dap_params->regulator_distortion);
        ms_get_int_from_str(&tmpstr, &p_dap_params->reg_nb_bands);
        if(p_dap_params->reg_nb_bands > DAP_REG_MAX_BANDS)
        {
            *err_code = MS_ERR_DAP_REG_INVALID_NB_BANDS;
            return MS_INVALID_PARAM;
        }

        ms_get_int_array_from_str(&tmpstr, p_dap_params->reg_nb_bands, &p_dap_params->a_reg_band_center[0]);
        ms_get_int_array_from_str(&tmpstr, p_dap_params->reg_nb_bands, &p_dap_params->a_reg_low_thresholds[0]);
        ms_get_int_array_from_str(&tmpstr, p_dap_params->reg_nb_bands, &p_dap_params->a_reg_high_thresholds[0]);
        ms_get_int_array_from_str(&tmpstr, p_dap_params->reg_nb_bands, &p_dap_params->a_reg_isolated_bands[0]);
    }

    gp_err = dlb_getparam_maxlenstring(h_getparam, "dap_virtual_bass", &gp_string, MS_MAX_STR_LEN);
    if(gp_err == DLB_GETPARAM_OK)
    {
        if(b_dap_vb_enable)
        {
            tmpstr = (char *)gp_string;
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_mode);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_low_src_freq);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_high_src_freq);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_overall_gain);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_slope_gain);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_subgain[0]);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_subgain[1]);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_subgain[2]);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_mix_low_freq);
            ms_get_int_from_str(&tmpstr, &p_dap_params->vb_mix_high_freq);
        }
        else
        {
            *err_code = MS_ERR_DAP_VB_NOT_ENABLED;
            return MS_INVALID_PARAM;
        }
    }


    return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_brief(void)
{
  fprintf(stdout,
      "\nUSAGE: \n\n"
      "   * The following is an abridged version of the help text \n"
      "     showing basic usage, inputs and outputs \n\n"
      "   * Complete help text      : MultiStreamDecoder -h \n\n"
      "   * Codec-specific help text: MultiStreamDecoder -h <ddplus | pcm | heaac> \n\n"
      "   * DAP-specific help text: MultiStreamDecoder -h dap \n\n"
      "   * Low complexity processing mode help text: MultiStreamDecoder -h lc \n\n"
      "Note: Run-time switchable parameters are marked with an asterisk (*) \n"
      "      and can be set using the \"%s\" file. \n\n"
          "CASE 1: INPUT TYPE - DDPLUS \n"
	  "MultiStreamDecoder [OPTIONS] -im   <main input> [REQUIRED]\n"
	  "                             -ia   <associated input> -is <system sounds input>\n"
	  "                             -ias  <application sounds input>\n"
          "                             -om   <main output> -oms  <main 2-ch downmix output>\n"
          "                             -odp  <DD+ output> -od  <DD output> \n"
          "CASE 2: INPUT TYPE - PCM \n"
	  "MultiStreamDecoder [OPTIONS] -im   <pcm input> [REQUIRED]\n"
	  "                             -is   <system sounds input> -ias  <application sounds input>\n"
          "                             -om   <main output> -oms  <main 2-ch downmix output>\n"
          "                             -odp  <DD+ output> -od  <DD output> \n"
          "CASE 3: INPUT TYPE - HE-AAC  \n"
	  "MultiStreamDecoder [OPTIONS] -im   <main input> [REQUIRED]\n"
	  "                             -ia   <associated input> -is <system sounds input>\n"
	  "                             -ias  <application sounds input>\n"
          "                             -om   <main output> -oms  <main 2-ch downmix output>\n"
          "                             -odp  <DD+ output> -od  <DD output> \n"
	    ,MS_COMMAND_FILE
	    );
	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_full(void)
{
	ms_show_usage_common_switches();
	ms_show_usage_context_ddplus();
	ms_show_usage_context_pcm();
	ms_show_usage_context_heaac();
        ms_show_usage_context_dap();
	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_lc(void)
{
	ms_show_usage_common_switches_lc();
	ms_show_usage_context_pcm_lc();
    ms_show_usage_context_dap();
	return MS_OK;
}


/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_common_switches(void)
{
	fprintf(stdout,
	  "\nUSAGE: \n\n"
	  "MultiStreamDecoder [OPTIONS]\n\n"
	  "Note: Run-time switchable parameters are marked with an asterisk (*) \n"
      "      and can be set using the \"%s\" file. \n\n"
      ,MS_COMMAND_FILE
	);

	fprintf(stdout,
      "INPUTS AND OUTPUTS \n\n"
      "Switch   Data   Function \n\n"
      "-im              <str>  Main program input filename [REQUIRED]\n"
      "-ia              <str>  Associated program input filename \n"
      "-is              <str>  System Sounds input filename \n"
      "-ias             <str>  Application Sounds input filename \n"
      "-om              <str>  Main program output filename \n"
      "-oms             <str>  Main program 2-channel downmix output filename \n"
      "-o_dap_speaker   <str>  DAPv2 Virtualized Speaker output filename\n"
      "-o_dap_headphone <str>  DAPv2 Virtualized Headphone output filename\n"
      "-od              <str>  Dolby Digital output filename \n"
      "-odp             <str>  Dolby Digital Plus output filename \n"
      "-ot              <str>  Output Parameter text filename \n\n"
	);

	fprintf(stdout,
      "Functional Switches \n\n"
      "Switch   Data     Function \n\n"
      "-lc             Flag to activate Low Complexity Mode (Note: limited functionality.) \n"
      "                use 'MultistreamDecoder -h lc' to get the low-complexity usage help context. \n"
      "-b    *  <int>  Scale factor for incoming DRC boost value \n"
      "                0 - 100; Default = 100 \n"
      "-c    *  <int>  Scale factor for incoming DRC cut value \n"
      "                0 - 100; Default = 100 \n"
      "-bs   *  <int>  Scale factor for incoming DRC boost value for 2-channel downmix \n"
      "                0 - 100; Default = 100 \n"
      "-cs   *  <int>  Scale factor for incoming DRC cut value for 2-channel downmix \n"
      "                0 - 100; Default = 100 \n"
      "-chas *  <int>  Channel configuration of Application Sounds input \n"
      "                0 = reserved\n"
      "                1 = 1/0 (C)\n"
      "                2 = 2/0 (L, R)\n"
      "                3 = 3/0 (L, R, C)\n"
      "                4 = 2/1 (L, R, l)\n"
      "                5 = 3/1 (L, R, C, l)\n"
      "                6 = 2/2 (L, R, l, r)\n"
      "                7 = 3/2 (L, R, C, l, r) (default)\n"
#ifdef MS12_7_1_SUPPORT
      "               21 = 3/2/2 (L, C, R, l, r, Lrs, Rrs)\n"
#endif
      "-chs  *  <int>  Channel configuration of System Sounds input \n"
      "                0 = reserved\n"
      "                1 = 1/0 (C)\n"
      "                2 = 2/0 (L, R)\n"
      "                3 = 3/0 (L, R, C)\n"
      "                4 = 2/1 (L, R, l)\n"
      "                5 = 3/1 (L, R, C, l)\n"
      "                6 = 2/2 (L, R, l, r)\n"
      "                7 = 3/2 (L, R, C, l, r) (default)\n"
#ifdef MS12_7_1_SUPPORT
      "               21 = 3/2/2 (L, C, R, l, r, Lrs, Rrs)\n"
#endif
      "-dap_init_mode <int> DAPv2 initialisation mode\n"
      "                     0 = DAPv2 instances are not present (default)\n"
      "                     1 = DAPv2 content processing instance is present only\n"
      "                     2 = DAPv2 device processing instance is present only\n"
      "                     3 = DAPv2 both content and device processing instances are present\n"
      "                     4 = DAPv2 single instance processing (both content and device)\n"
      "-b_dap_vb_enable <int> DAPv2 Virtual Bass enable (additional delay)\n"
      "                     0 = DAPv2 Virtual Bass is not enabled (default)\n"
      "                     1 = DAPv2 Virtual Bass is enabled\n"
      "-dbgout        <int>  Bitmask to activate different debug wave output files (default: none)\n");
        fprintf(stdout,
      "                  0x%04X: Main decoder output\n"
      "                  0x%04X: Associated decoder output\n"
      "                  0x%04X: AD mixer output\n"
      "                  0x%04X: Resampler output\n"
      "                  0x%04X: DAP Content Processing output\n"
      "                  0x%04X: System Sound Mixer output\n"
      "                  0x%04X: DAP Device Processing input\n"
                , DBG_OUT_MAIN_DEC, DBG_OUT_ASSOC_DEC, DBG_OUT_AD_MIX, DBG_OUT_RSRC, DBG_OUT_DAP_CP
                , DBG_OUT_SYS_MIX, DBG_OUT_DAP_DP_IN);
        fprintf(stdout,
      "-drc  *  <int>  DRC modes \n"
      "                0 = Line (Default) \n"
      "                1 = RF \n"
      "-dmx  *  <int>  Downmix modes \n"
      "                0 = Lt/Rt (Default) \n"
      "                1 = Lo/Ro \n"
      "                [he-aac] 2 = ARIB \n"
      "-eval    <int>  Evaluation Mode (periodically mutes the signal)\n"
      "                0 = off (default)\n"
      "                1 = on\n"
      "-h              Help \n"
      "-las  *  <int>  LFE present in Application Sounds input \n"
      "                0 = off\n"
      "                1 = on (default)\n"
      "-ls   *  <int>  LFE present in System Sounds input \n"
      "                0 = off\n"
      "                1 = on (default)\n"
#ifdef MS12_7_1_SUPPORT
      "-max_channels   <int> Maximum number of channels in the signal chain (6 or 8)\n"
      "-mc_5_1_dmx   * <int> Downmix 7.1 PCM signal to 5.1 on the multichannel outputs\n"
      "                      0 = off (default)\n"
      "                      1 = on\n"
#endif
      "-p       <int>  RISC precision flag (fixpoint backends only)\n"
      "                0 = PCM data has less precision (e.g. 16 bit for 16x32) \n"
      "                1 = PCM data has high precision (default, e.g. 32 bit for 16x32) \n"
      "-u    *  <int>  Dual-mono reproduction mode \n"
      "                0 = Stereo (Default); \n"
      "                1 = Left / first channel; \n"
      "                2 = Right / second channel \n"
      "-v       <int>  Verbosity \n"
      "                0 = Silent Mode, only error messages are shown\n"
      "                1 = Banner output + processing status + stream information + summary\n"
      "                2 = the above + Parameter Settings + substream information\n"
      "                3 = the above + output parameter in a \"grep friendly\" format\n"
      "-w       <int>  Output WAV file bit-depth\n"
      "                16, 24 or 32\n"
      "-xa   *  <int>  Associated audio mixing \n"
      "                0 = Off \n"
      "                1 = On (Default) \n"
      "-xs   *  <int>  System / Application audio mixing \n"
      "                0 = Off \n"
      "                1 = On (Default) \n"
      "-xu   *  <int>  User control values for mixing main and associated audio \n"
      "                Range: -32 (mute assoc) to 32 (mute main) in steps of 1 (dB) \n"
      "                1 ... 31   - dB to favor associated (attenuate main) \n"
      "                -1 ... -31 - dB to favor main (attenuate associated) \n"
      "-xsu  *  <int>  User control values for mixing primary and application audio \n"
      "                Range: -32 (mute application) to 32 (mute primary) in steps of 1 (dB) \n"
      "                1 ... 31   - dB to favor application (attenuate primary) \n"
      "                -1 ... -31 - dB to favor primary (attenuate application) \n"
	);

	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_common_switches_lc(void)
{
	fprintf(stdout,
	  "\nUSAGE: \n\n"
	  "MultiStreamDecoder [OPTIONS]\n\n"
      "LOW COMPLEXITY PROCESSING MODE (limited functionality): \n"
      "                - no HE-AAC decoding supported \n"
      "                - maximum number of processing channels limited to six \n"
      "                - no main/associated mixing supported \n"
      "                - no (re)encoding (only bitstream passthrough/conversion) \n"
      "                - only DAP single instance processing init mode supported \n"
      "                - no multichannel output supported \n"
      "                - no application sound input supported \n"
      "                - no output parameter text file supported \n"
      "                - system sounds input limited to 2-channel input \n"
	  "Note: Run-time switchable parameters are marked with an asterisk (*) \n"
      "      and can be set using the \"%s\" file. \n\n"
      ,MS_COMMAND_FILE
	);

	fprintf(stdout,
      "INPUTS AND OUTPUTS \n\n"
      "Switch   Data   Function \n\n"
      "-im              <str>  Main program input filename [REQUIRED]\n"
      "-is              <str>  System Sounds input filename \n"
      "-oms             <str>  2-channel downmix output filename \n"
      "-o_dap_speaker   <str>  DAPv2 Virtualized Speaker output filename\n"
      "-o_dap_headphone <str>  DAPv2 Virtualized Headphone output filename\n"
      "-od              <str>  Dolby Digital output filename \n"
      "-odp             <str>  Bitstream Pass-Through (Dolby Digital Plus) output filename \n"
	);

	fprintf(stdout,
      "Functional Switches \n\n"
      "Switch   Data     Function \n\n"
      "-lc             Flag to activate Low Complexity Mode (Note: limited functionality) \n"
      "                use 'MultistreamDecoder -h lc' to get the low-complexity usage help context. \n"
      "-b    *  <int>  Scale factor for incoming DRC boost value \n"
      "                0 - 100; Default = 100 \n"
      "-c    *  <int>  Scale factor for incoming DRC cut value \n"
      "                0 - 100; Default = 100 \n"
      "-bs   *  <int>  Scale factor for incoming DRC boost value for 2-channel downmix \n"
      "                0 - 100; Default = 100 \n"
      "-cs   *  <int>  Scale factor for incoming DRC cut value for 2-channel downmix \n"
      "                0 - 100; Default = 100 \n"
      "-chs  *  <int>  Channel configuration of System Sounds input \n"
      "                0 = reserved\n"
      "                1 = 1/0 (C)\n"
      "                2 = 2/0 (L, R) (default)\n"
      "-dbgout        <int>  Bitmask to activate different debug wave output files (default: none)\n");
        fprintf(stdout,
      "                  0x%04X: Main decoder output\n", DBG_OUT_MAIN_DEC);
        fprintf(stdout,
      "-drc  *  <int>  DRC modes \n"
      "                0 = Line (Default) \n"
      "                1 = RF \n"
      "-dmx  *  <int>  Downmix modes \n"
      "                0 = Lt/Rt (Default) \n"
      "                1 = Lo/Ro \n"
      "-h              Help \n"
      "-p       <int>  RISC precision flag (fixpoint backends only)\n"
      "                0 = PCM data has less precision (e.g. 16 bit for 16x32) \n"
      "                1 = PCM data has high precision (default, e.g. 32 bit for 16x32) \n"
      "-u    *  <int>  Dual-mono reproduction mode \n"
      "                0 = Stereo (Default); \n"
      "                1 = Left / first channel; \n"
      "                2 = Right / second channel \n"
      "-v       <int>  Verbosity \n"
      "                0 = Silent Mode, only error messages are shown\n"
      "                1 = Banner output + processing status + stream information + summary\n"
      "                2 = the above + Parameter Settings + substream information\n"
      "                3 = the above + output parameter in a \"grep friendly\" format\n"
      "-w       <int>  Output WAV file bit-depth\n"
      "                16, 24 or 32\n"
	);

	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_context_ddplus(void)
{
	/* Display all ddplus-specific switches */
  fprintf(stdout,
      "DDPLUS SWITCHES \n\n"
      "Switch   Data    Function \n\n"
      "-at   *  <int>   [ddplus] Associated substream selection \n"
      "                 1 - 3; No default \n"
  );

	return MS_OK;
}


/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_context_pcm(void)
{
	/* Display all pcm-specific switches */
  fprintf(stdout,
      "PCM SWITCHES  \n\n"
      "Switch   Data    Function \n\n"
      "-chp  *  <int>   Channel configuration of external PCM input \n"
      "                 0 = reserved\n"
      "                 1 = 1/0 (C)\n"
      "                 2 = 2/0 (L, R)\n"
      "                 7 = 3/2 (L, C, R, l, r) (default)\n"
#ifdef MS12_7_1_SUPPORT
      "               21 = 3/2/2 (L, C, R, l, r, Lrs, Rrs)\n"
#endif
      "-lp   *  <int>   LFE present in external PCM input \n"
      "                 0 = off\n"
      "                 1 = on (default)\n"
      "-rp      <int>   [pcm] Compressor profile \n"
      "                 0 [clipping protection only]\n"
      "                 1 [film-standard, default]\n"
      "                 2 [film-light]\n"
      "                 3 [music-standard]\n"
      "                 4 [music-light]\n"
      "                 5 [speech]\n"
  );
	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_context_pcm_lc(void)
{
	/* Display all pcm-specific switches */
  fprintf(stdout,
      "PCM SWITCHES  \n\n"
      "Switch   Data    Function \n\n"
      "-chp  *  <int>   Channel configuration of external PCM input \n"
      "                 0 = reserved\n"
      "                 1 = 1/0 (C)\n"
      "                 2 = 2/0 (L, R)\n"
      "                 7 = 3/2 (L, C, R, l, r) (default)\n"
      "-lp   *  <int>   LFE present in external PCM input \n"
      "                 0 = off\n"
      "                 1 = on (default)\n"
  );
	return MS_OK;
}


/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_context_heaac(void)
{
	/* Display all HE-AAC-specific switches */
  fprintf(stdout,
      "HE-AAC SWITCHES  \n\n"
      "Switch  Data   Function \n\n"
      "-as     <int>  [he-aac] Associated instance restricted to 2 channels \n"
      "-dn     <int>  [he-aac] Default dialnorm value (dB/4) \n"
      "               0 - 127; Default = 108 (-27dB) \n"
      "-t      <int>  [he-aac] Set transport format \n"
      "               0 = auto-detect (Default) \n"
      "               1 = ADTS \n"
      "               2 = LOAS  \n"
      "               3 = RAW (Default for file playback)  \n\n"
  );
	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_usage_context_dap(void)
{
	/* Display all DAP-specific switches */
  fprintf(stdout,
      "DAP SWITCHES\n"
      "  (Multiple values are expected as comma seperated values without spaces)\n\n"
      "Switch                       Data  Function \n\n"
      "-dap_drc                     <int> DAP DRC mode\n"
      "                                     0 = Line (Default) \n"
      "                                     1 = RF \n"
      "-dap_dmx                     <int> DAP Downmix mode\n"
      "                                     0 = Lt/Rt (Default) \n"
      "                                     1 = Lo/Ro \n"
#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
      "-dap_gains                 <3 int> \n"
#else
      "-dap_gains                 <1 int> \n"
#endif
      "                                   * postgain (-2080...480, def: 0)\n"
#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
      "                                   * pregain (-2080...480, def: 0)\n"
      "                                   * systemgain (-2080...480, def: 0)\n"
#endif
      "-dap_surround_decoder_enable <int> DAP surround decoder enable flag (Default 1)\n"
      "-dap_surround_virtualizer  <5 int> Virtualizer Parameter\n"
      "                                   * virtualizer_enable (0,1, def: 1)\n"
      "                                   * headphone_reverb (-2080...192, def: 0)\n"
      "                                   * speaker_angle (5..30, def: 10)\n"
      "                                   * speaker_start (20...2000, def: 20)\n"
      "                                   * surround_boost (0...96, def: 96)\n"
      "-dap_mi_steering           <4 int> * Media Intelligence Steering\n"
      "                                   * mi_ieq_enable (0,1, def: 0)\n"
      "                                   * mi_dv_enable (0,1, def: 0)\n"
      "                                   * mi_de_enable (0,1, def: 0)\n"
      "                                   * mi_surround_enable (0,1, def: 0)\n"
      "-dap_calibration_boost       <int> (0...192, def: 0)\n"
      "-dap_leveler               <3 int>\n"
      "                                   * leveler_enable (0,1, def: 0)\n"
      "                                   * leveler_amount (0...10, def: 7)\n"
      "                                   * leveler_ignore_il (0, 1, def: 0)\n"
      );
  fprintf(stdout,
      "-dap_ieq        <3+2*nb_bands int>\n"
      "                                   * ieq_enable (0,1, def: 0)\n"
      "                                   * ieq_amount (0...16, def: 10)\n"
      "                                   * ieq_nb_bands (1...20, def: 10)\n"
      "                                   * ieq_band_center (20...20000, def: {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000})\n"
      "                                   * ieq_band_target (-480...480, def: {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}\n"
      "-dap_dialogue_enhancer     <3 int>\n"
      "                                   * de_enable (0,1, def: 0)\n"
      "                                   * de_amount (0...16, def: 0)\n"
      "                                   * de_ducking (0...16, def: 0)\n"
      "-dap_graphic_eq <2+2*nb_bands int>\n"
      "                                   * eq_enable (0,1, def: 0)\n"
      "                                   * eq_nb_bands (1...20, def: 10)\n"
      "                                   * eq_band_center (20...20000, def: {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000})\n"
      "                                   * eq_band_target (-576...576, def: {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}\n"
      "-dap_optimizer <2+nb_bands+MAX_CHANNELS*nb_bands int>\n"
      "                                   * optimizer_enable (0,1, def: 0)\n"
      "                                   * opt_nb_bands (1...20, def: 10)\n"
      "                                   * opt_band_center_freq (20...20000, def: {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000})\n"
      "                                   * opt_band_gains[MAX_CHANNELS] (-480...480, def: {{10*0, 10*0, 10*0, 10*0, 10*0, 10*0, 10*0, 10*0})\n"
      "-dap_bass_enhancer         <4 int>\n"
      "                                   * bass_enable (0,1, def: 0)\n"
      "                                   * bass_boost (0...384, def: 192)\n"
      "                                   * bass_cutoff (20...20000, def: 200)\n"
      "                                   * bass_width (2..64, def: 16)\n"
      "-dap_regulator  <6+4*nb_bands int>\n"
      "                                   * regulator_enable (0,1, def: 1)\n"
      "                                   * regulator_mode (0,1, def: 0)\n"
      "                                   * regulator_overdrive (0...192, def: 0)\n"
      "                                   * regulator_timbre (0...16, def: 16)\n"
      "                                   * regulator_distortion (0...144, def: 96)\n"
      "                                   * reg_nb_bands (1...20, def: 2)\n"
      "                                   * reg_band_center (20...20000, def: {20,20000}\n"
      "                                   * reg_low_thresholds (-2080...0, def: {-192, -192})\n"
      "                                   * reg_high_thresholds (-2080...0, def: {0, 0})\n"
      "                                   * reg_isolated_bands (0,1 def: {0,0})\n"
      "-dap_virtual_bass  <10>\n"
      "                                   * virtual_bass_mode (0...3, def: 0)\n"
      "                                   * virtual_bass_low_src_freq (30...90, def: 35)\n"
      "                                   * virtual_bass_high_src_freq (90...270, def: 160)\n"
      "                                   * virtual_bass_overall_gain(-480...0, def: 0)\n"
      "                                   * virtual_bass_slope_gain (-3...0, def: 0)\n"
      "                                   * virtual_bass_subgains[3] (-480...0, def: {-32,-144,-192})\n"
      "                                   * virtual_bass_low_mix_freq (0...375, def: 94)\n"
      "                                   * virtual_bass_high_mix_freq (281...938, def: 469)\n"
      "\n"
  );
	return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_show_banner(void)
{

	fprintf(stdout, "\n************** Dolby MultiStreamDecoder Version 1.1.0 **************\n");


	/* display copyright info */
	//fprintf(stdout, "%s", ms_dolby_copyright);
	return MS_OK;
}



/*
 * Documentation resides in declaration only
 */
static MS_RETVAL ms_validate_extension(const char *param, const char *ext)
{
	int counter = 0;
	char temp[]="extn";

	if (strlen(param) > MS_EXT_LENGTH)
	{
	  for (counter = MS_EXT_LENGTH; counter > 0; counter--)
	  {
	    temp[MS_EXT_LENGTH-counter] = (char)tolower(param[strlen(param)-counter]);
	  }
	}

	if (strcmp(temp, ext) == 0)
	  return MS_OK;
	else
	  return MS_INVALID_PARAM;
}

static MS_RETVAL ms_get_int_array_from_str(char **p_csv_string, int num_el, int *p_vals)
{
    char *endstr;
    int i;

    for(i=0; i<num_el; i++)
    {
        p_vals[i] = strtol(*p_csv_string, &endstr, 0);
        if(*p_csv_string == endstr)
        {
            return MS_INVALID_PARAM;
        }
        *p_csv_string = endstr+1;
    }
    return MS_OK;
}

static MS_RETVAL ms_get_int_from_str(char **p_csv_string, int *p_value)
{
    char *endstr;

    *p_value = strtol(*p_csv_string, &endstr, 0);
    if(*p_csv_string == endstr)
    {
        return MS_INVALID_PARAM;
    }
    else
    {
        *p_csv_string = endstr+1;
        return MS_OK;
    }
}

static const char *enable_to_str(int b_enable)
{
    if(b_enable <= 1)
        return enable2str[b_enable + 1];
    else
        return enable2str[2];
}

static MS_RETVAL set_int_check_gp_err(DLB_GETPARAM_RETVAL gp_err, int val, int *p_assign_val, int *err_code, int outofrange, int invalid)
{
    if(gp_err == DLB_GETPARAM_OK)
    {
        *p_assign_val = val;
    }
    else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
    {
        *err_code = outofrange;
        return MS_INVALID_PARAM;
    }
    else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
    {
        *err_code = invalid;
        return MS_INVALID_PARAM;
    }
    return MS_OK;
}
