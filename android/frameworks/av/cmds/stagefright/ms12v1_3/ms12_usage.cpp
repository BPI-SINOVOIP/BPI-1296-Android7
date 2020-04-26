/******************************************************************************
 * This program is protected under international and U.S. copyright laws as
 * an unpublished work. This program is confidential and proprietary to the
 * copyright owners. Reproduction or disclosure, in whole or in part, or the
 * production of derivative works therefrom without the express permission of
 * the copyright owners is prohibited.
 *
 *                Copyright (C) 2012-2016 by Dolby International AB.
 *                            All rights reserved.
 ******************************************************************************/
/** @ingroup msd_frontend */

/** @file
 *  @brief Multistream Decoder Front-end - Command-line usage and parameter setting printouts - code file
 */

#include <stdio.h>
#include "ms12_usage.h"


#define MS_MAJOR_VERSION      "1"
#define MS_MINOR_VERSION      "3"
#define MS_UPDATE_VERSION     "1"

const char ms_dolby_copyright[] =
    "\nUnpublished work.  Copyright 2003-2016 Dolby Laboratories, Inc. and" \
    "\nDolby Laboratories Licensing Corporation.  All Rights Reserved.\n\n"    \
    "USE OF THIS SOFTWARE IS SUBJECT TO A LEGAL AGREEMENT BETWEEN YOU AND DOLBY\n"    \
    "LABORATORIES. DO NOT USE THIS SOFTWARE UNLESS YOU AGREE TO THE TERMS AND \n"    \
    "CONDITIONS IN THE AGREEMENT.  BY USING THIS SOFTWARE, YOU ACKNOWLEDGE THAT \n"    \
    "YOU HAVE READ THE AGREEMENT AND THAT YOU AGREE TO BE BOUND BY ITS TERMS. \n\n";

static void ms_print_dap_parameter(dap_params_t *p_dap_params);

static const char *enable_to_str(int b_enable);

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
#ifdef MS12_AC4_SUPPORT
        case AC4:
            assert(0);
            break;
#endif
        case UNKNOWN:
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
            fprintf(stdout, "  DOLBY DIGITAL PLUS TRANSCODING - DUAL %sINPUT\n", p_use_case->b_dual_main ? "MAIN " : "");
        else
            fprintf(stdout, "  DOLBY DIGITAL PLUS TRANSCODING - SINGLE INPUT\n");
        break;
    case EXTERNAL_PCM:
        fprintf(stdout, "  EXTERNAL PCM REENCODING\n");
        break;
#ifdef MS12_AC4_SUPPORT
    case AC4:
        if(p_use_case->ac4_mode == AC4_MODE_DUAL_STREAM_DUAL_DECODE)
            fprintf(stdout, "  DOLBY AC4 TRANSCODING - DUAL INPUT - DUAL DECODE\n");
        else if(p_use_case->ac4_mode == AC4_MODE_SINGLE_STREAM_DUAL_DECODE_SINGLE_INSTANCE)
            fprintf(stdout, "  DOLBY AC4 TRANSCODING - SINGLE INPUT - DUAL DECODE - SINGLE DECODER INSTANCE\n");
        else if(p_use_case->ac4_mode == AC4_MODE_SINGLE_STREAM_DUAL_DECODE_DUAL_INSTANCE)
            fprintf(stdout, "  DOLBY AC4 TRANSCODING - SINGLE INPUT - DUAL DECODE - DUAL DECODER INSTANCE\n");
        else
            fprintf(stdout, "  DOLBY AC4 TRANSCODING - SINGLE INPUT - SINGLE DECODE\n");
        break;
#endif
    case UNKNOWN:
        fprintf(stdout, "  UNKNOWN\n");
        assert(0);
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
    else
    {
        fprintf(stdout, "  Input Wavefile Main:                %s\n", p_args->ms_init_args.input_filename[MS_MAIN_DECODER_IDX]);
    }
    if (p_use_case->b_dual_main)
    {
        fprintf(stdout, "  Input Bitstream Main2:              %s\n", p_args->ms_init_args.input_filename[MS_ASSOC_DECODER_IDX]);
    }
#ifdef MS12_AC4_SUPPORT
    if(p_use_case->ac4_mode == AC4_MODE_DUAL_STREAM_DUAL_DECODE)
        fprintf(stdout, "  Input Bitstream Associated:         %s\n", p_args->ms_init_args.input_filename[MS_ASSOC_DECODER_IDX]);
#else
    if(p_use_case->b_dual_input)
        fprintf(stdout, "  Input Bitstream Associated:         %s\n", p_args->ms_init_args.input_filename[MS_ASSOC_DECODER_IDX]);
#endif
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
    fprintf(stdout, "  Downmix type (2ch):                 %s\n", downmix_type2str[p_args->ms_runtime_args.downmix_type]);
    fprintf(stdout, "  DRC cut/boost factor (2ch):         %d%%, %d%%\n", p_args->ms_runtime_args.drc_cut_fac_2ch, p_args->ms_runtime_args.drc_boost_fac_2ch);
    if(p_use_case->b_low_complexity)
        fprintf(stdout, "  DRC cut/boost factor (dap output):  %d%%, %d%%\n", p_args->ms_runtime_args.drc_cut_fac_mc, p_args->ms_runtime_args.drc_boost_fac_mc);
    else
        fprintf(stdout, "  DRC cut/boost factor (6 or 8ch):    %d%%, %d%%\n", p_args->ms_runtime_args.drc_cut_fac_mc, p_args->ms_runtime_args.drc_boost_fac_mc);
    assert(p_args->ms_runtime_args.dual_mono < 3);
    fprintf(stdout, "  Dual Mono Reproduction:             %s\n", dual_mode2str[p_args->ms_runtime_args.dual_mono]);
    if (p_use_case->b_low_complexity)
    {
        fprintf(stdout, "  AD Mixing:                          n/a\n");
    }
    else if (!p_use_case->b_dual_main)
    {
        fprintf(stdout, "  AD Mixing:                          %s\n", p_args->ms_runtime_args.associated_audio_mixing ? "on" : "off");
    }
    fprintf(stdout, "  System/Appication sound  Mixing:    %s\n", p_args->ms_runtime_args.b_sys_app_sound_mixing ? "on" : "off");
    if (!p_use_case->b_low_complexity && !p_use_case->b_dual_main)
    {
        if(p_args->ms_runtime_args.user_balance_adjustment == 32)
            fprintf(stdout, "  AD Mixing User Balance:             +32 (associated only)\n");
        else if(p_args->ms_runtime_args.user_balance_adjustment == -32)
            fprintf(stdout, "  AD Mixing User Balance:             -32 (main only)\n");
        else
            fprintf(stdout, "  AD Mixing User Balance:             %ddB\n", p_args->ms_runtime_args.user_balance_adjustment);
    }

    if(p_use_case->input_type==EXTERNAL_PCM){
        fprintf(stdout, "  Input Samples per channel:          %d\n", p_args->ms_runtime_args.extpcm_num_in_samples);
        if(!p_use_case->b_low_complexity)
            fprintf(stdout, "  Compression Profile:                %s\n", profile2str[p_args->ms_init_args.extpcm_compressor_profile]);
    }
    else if(p_use_case->input_type==DOLBY_DIGITAL_PLUS){
        if(!p_use_case->b_mainonly && !p_use_case->b_dual_input && !p_use_case->b_low_complexity)
            fprintf(stdout, "  Selected Associated Substream:      %d\n", p_args->ms_runtime_args.ddplus_associated_substream);
    }
#ifdef MS12_AC4_SUPPORT
    else if(p_use_case->input_type==AC4
            )
    {
        fprintf(stdout, "  Prefer Assoc. Type over Language:   %s\n", p_args->ms_runtime_args.b_ac4_pref_assoc_type_over_lang ? "yes" : "no");
        if(p_args->ms_runtime_args.ac4_1st_pref_lang[0] != '\0')
        {
            fprintf(stdout, "  Preferred Language:                 %s\n", p_args->ms_runtime_args.ac4_1st_pref_lang);
        }
        if(p_args->ms_runtime_args.ac4_2nd_pref_lang[0] != '\0')
        {
            fprintf(stdout, "  2nd Preferred Language:             %s\n", p_args->ms_runtime_args.ac4_2nd_pref_lang);
        }
        if(p_use_case->b_dual_input)
        {
            fprintf(stdout, "  Preferred AD Type:                  %d\n", p_args->ms_runtime_args.ac4_associated_type);
        }
        if(p_args->ms_runtime_args.ac4_pres_index[0] > -1)
        {
            fprintf(stdout, "  Presentation Index:                 %d\n", p_args->ms_runtime_args.ac4_pres_index[0]);
        }
        if(p_args->ms_runtime_args.ac4_pres_index[1] > -1)
        {
            fprintf(stdout, "  2nd Presentation Index:             %d\n", p_args->ms_runtime_args.ac4_pres_index[1]);
        }

        fprintf(stdout, "  Program Identifier Type:            %d\n", p_args->ms_runtime_args.ac4_prog_id_type);
        if(p_args->ms_runtime_args.ac4_prog_id_type == 1)
        {
            fprintf(stdout, "  Short Program Identifier:           %s\n", p_args->ms_runtime_args.ac4_program_identifier_string);
        }
        else if(p_args->ms_runtime_args.ac4_prog_id_type == 2)
        {
            fprintf(stdout, "  UUID:                               %s\n", p_args->ms_runtime_args.ac4_program_identifier_string);
        }
    }
#endif
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


/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_brief(void)
{
  fprintf(stdout,
      "\nUSAGE: \n\n"
      "   * The following is an abridged version of the help text \n"
      "     showing basic usage, inputs and outputs \n\n"
      "   * Complete help text      : MultiStreamDecoder -h \n\n"
#ifdef MS12_AC4_SUPPORT
      "   * Codec-specific help text: MultiStreamDecoder -h <ddplus | ac4 | pcm | heaac> \n\n"
#else
      "   * Codec-specific help text: MultiStreamDecoder -h <ddplus | pcm | heaac> \n\n"
#endif
      "   * DAP-specific help text: MultiStreamDecoder -h dap \n\n"
      "   * Low complexity processing mode help text: MultiStreamDecoder -h lc \n\n"
      "   * Usage of the \"%s\" file for switching parameter during runtime \n\n"
      "Note: Run-time switchable parameters (user and system parameters) are marked with an \n"
      "      asterisk (*) and can be set using the \"%s\" file. \n\n"
      "CASE 1: INPUT TYPE - DDPLUS \n"
      "MultiStreamDecoder [OPTIONS] -im  * <main input> [REQUIRED]\n"
      "                             -ia    <associated input>\n"
      "                             -is    <system sounds input>\n"
      "                             -ias   <application sounds input>\n"
      "                             -om    <pcm multi-channel output>\n"
      "                             -oms   <pcm 2-ch downmix output>\n"
      "                             -odp   <DD+ output>\n"
      "                             -od    <DD output>\n"
      "CASE 2: INPUT TYPE - DUAL DDPLUS \n"
      "MultiStreamDecoder [OPTIONS] -im  * <main input> [REQUIRED]\n"
      "                             -im2 * <second main input>\n"
      "                             -is    <system sounds input>\n"
      "                             -ias   <application sounds input>\n"
      "                             -om    <pcm multi-channel output>\n"
      "                             -oms   <pcm 2-ch downmix output>\n"
      "                             -odp   <DD+ output>\n"
      "                             -od    <DD output>\n"
      "CASE 3: INPUT TYPE - PCM \n"
      "MultiStreamDecoder [OPTIONS] -im  * <pcm input> [REQUIRED]\n"
      "                             -is    <system sounds input>\n"
      "                             -ias   <application sounds input>\n"
      "                             -om    <pcm multi-channel output>\n"
      "                             -oms   <pcm 2-ch downmix output>\n"
      "                             -odp   <DD+ output>\n"
      "                             -od    <DD output>\n"
      "CASE 4: INPUT TYPE - HE-AAC  \n"
      "MultiStreamDecoder [OPTIONS] -im  * <main input> [REQUIRED]\n"
      "                             -ia    <associated input>\n"
      "                             -is    <system sounds input>\n"
      "                             -ias   <application sounds input>\n"
      "                             -om    <pcm multi-channel output>\n"
      "                             -oms   <pcm 2-ch downmix output>\n"
      "                             -odp   <DD+ output>\n"
      "                             -od    <DD output>\n"
#ifdef MS12_AC4_SUPPORT
      "CASE 5: INPUT TYPE - Dolby AC4  \n"
      "MultiStreamDecoder [OPTIONS] -im  * <main input> [REQUIRED]\n"
      "                             -ia    <associated input>\n"
      "                             -is    <system sounds input>\n"
      "                             -ias   <application sounds input>\n"
      "                             -om    <pcm multi-channel output>\n"
      "                             -oms   <pcm 2-ch downmix output>\n"
      "                             -odp   <DD+ output>\n"
      "                             -od    <DD output>\n"
#endif
        ,MS_COMMAND_FILE
        ,MS_COMMAND_FILE
        );
    return MS_OK;
}



/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_full(void)
{
    ms_show_usage_common_switches();
#ifdef MS12_AC4_SUPPORT
    ms_show_usage_context_ac4();
#endif
    ms_show_usage_context_ddplus();
    ms_show_usage_context_pcm();
    ms_show_usage_context_heaac();
    ms_show_usage_context_dap();
    ms_show_usage_command_txt();
    return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_lc(void)
{
    ms_show_usage_common_switches_lc();
    ms_show_usage_context_pcm_lc();
    ms_show_usage_context_dap();
    ms_show_usage_command_txt();
    return MS_OK;
}


/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_common_switches(void)
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
      "Switch           Data   Function \n\n"
      "-im  *           <str>  Main program input filename [REQUIRED]\n"
      "-im2 *           <str>  2nd Main program input filename \n"
      "-ia              <str>  Associated program input filename \n"
      "-is              <str>  System Sounds input filename \n"
      "-ias             <str>  Application Sounds input filename \n"
#ifdef MS12_AC4_SUPPORT
      "-it              <str>  Use case input type (DDP|AAC|AC4|PCM)\n"
#else
      "-it              <str>  Use case input type (DDP|AAC|PCM)\n"
#endif
      "                        default: derived from -im filename extension\n"
      "                        can be used to select the codec in mp4 container files\n"
      "-om              <str>  pcm multichannel program output filename \n"
      "-oms             <str>  pcm 2-channel downmix output filename \n"
      "-o_dap_speaker   <str>  DAPv2 'virtualized for speaker' output filename\n"
      "-o_dap_headphone <str>  DAPv2 'virtualized for headphone' output filename\n"
      "-od              <str>  Dolby Digital output filename \n"
      "-odp             <str>  Dolby Digital Plus output filename \n"
      "-ot              <str>  Output Parameter text filename \n\n"
      "Note: -im2 and -ia cannot both be specified.\n\n"
      "Note: Input filenames starting with \"dummy.\" can be used to specify \"no current input\".\n"
      "      This can be used for a delayed input start or for an input stop during run-time.\n\n"
    );

    fprintf(stdout,
      "Functional Switches \n\n"
      "Switch              Data    Function \n\n"
      "-lc                         Flag to activate Low Complexity Mode (Note: limited functionality.) \n"
      "                            use 'MultistreamDecoder -h lc' to get the low-complexity usage help context. \n"
      "-dmxon            * <int>   Run-time flag to activate the 2-channel downmix output (default:active)\n"
      "-mc               * <int>   Run-time flag to activate the multichannel output (default:active)\n"
      "-b                * <int>   Scale factor for incoming DRC boost value \n"
      "                            0 - 100; Default = 100 \n"
      "-c                * <int>   Scale factor for incoming DRC cut value \n"
      "                            0 - 100; Default = 100 \n"
      "-bs               * <int>   Scale factor for incoming DRC boost value for 2-channel downmix \n"
      "                            0 - 100; Default = 100 \n"
      "-cs               * <int>   Scale factor for incoming DRC cut value for 2-channel downmix \n"
      "                            0 - 100; Default = 100 \n"
      "-chas             * <int>   Channel configuration of Application Sounds input \n"
      "                            0 = reserved\n"
      "                            1 = 1/0 (C)\n"
      "                            2 = 2/0 (L, R)\n"
      "                            3 = 3/0 (L, R, C)\n"
      "                            4 = 2/1 (L, R, l)\n"
      "                            5 = 3/1 (L, R, C, l)\n"
      "                            6 = 2/2 (L, R, l, r)\n"
      "                            7 = 3/2 (L, R, C, l, r) (default)\n"
#ifdef MS12_7_1_SUPPORT
      "                           21 = 3/2/2 (L, C, R, l, r, Lrs, Rrs)\n"
#endif
      "-chs              * <int>   Channel configuration of System Sounds input \n"
      "                            0 = reserved\n"
      "                            1 = 1/0 (C)\n"
      "                            2 = 2/0 (L, R) (default)\n"
      "                            3 = 3/0 (L, R, C)\n"
      "                            4 = 2/1 (L, R, l)\n"
      "                            5 = 3/1 (L, R, C, l)\n"
      "                            6 = 2/2 (L, R, l, r)\n"
      "                            7 = 3/2 (L, R, C, l, r)\n"
#ifdef MS12_7_1_SUPPORT
      "                           21 = 3/2/2 (L, C, R, l, r, Lrs, Rrs)\n"
#endif
      "-dap_init_mode      <int>   DAPv2 initialisation mode\n"
      "                            0 = DAPv2 instances are not present (default)\n"
      "                            1 = DAPv2 content processing instance is present only\n"
      "                            2 = DAPv2 device processing instance is present only\n"
      "                            3 = DAPv2 both content and device processing instances are present\n"
      "                            4 = DAPv2 single instance processing (both content and device)\n"
      "-b_dap_vb_enable    <int>   DAPv2 Virtual Bass enable (additional delay)\n"
      "                            0 = DAPv2 Virtual Bass is not enabled (default)\n"
      "                            1 = DAPv2 Virtual Bass is enabled\n"
      "-dbgout             <int>   Bitmask to activate different debug wave output files (default: none)\n");
    fprintf(stdout,
      "                            0x%04X: Main decoder output\n"
      "                            0x%04X: Associated decoder output\n"
      "                            0x%04X: AD mixer output\n"
      "                            0x%04X: Resampler output\n"
      "                            0x%04X: DAP Content Processing output\n"
      "                            0x%04X: System Sound Mixer output\n"
      "                            0x%04X: DAP Device Processing input\n"
           , DBG_OUT_MAIN_DEC
           , DBG_OUT_ASSOC_DEC
           , DBG_OUT_AD_MIX
           , DBG_OUT_RSRC
           , DBG_OUT_DAP_CP
           , DBG_OUT_SYS_MIX
           , DBG_OUT_DAP_DP_IN
           );
    fprintf(stdout,
      "-drc              * <int>   DRC modes (for downmixed output) \n"
      "                            0 = Line (Default) \n"
      "                            1 = RF \n"
      "-dap_drc          * <int>   DAP DRC mode (for multichannel and DAP output) \n"
      "                            0 = Line (Default) \n"
      "                            1 = RF \n"
      "-dmx              * <int>   Downmix modes \n"
      "                            0 = Lt/Rt (Default) \n"
      "                            1 = Lo/Ro \n"
      "                            [he-aac] 2 = ARIB \n"
      "-eval               <int>   Evaluation Mode (periodically mutes the signal)\n"
      "                            0 = off (default)\n"
      "                            1 = on\n"
      "-h                          Help \n"
      "-las              * <int>   LFE present in Application Sounds input \n"
      "                            0 = off\n"
      "                            1 = on (default)\n"
      "-ls               * <int>   LFE present in System Sounds input \n"
      "                            0 = off (default)\n"
      "                            1 = on\n"
#ifdef MS12_7_1_SUPPORT
      "-max_channels       <int>   Maximum number of channels in the signal chain (6 or 8)\n"
      "-mc_5_1_dmx         <int>   Downmix 7.1 PCM signal to 5.1 on the multichannel outputs\n"
      "                            0 = off (default)\n"
      "                            1 = on\n"
#endif
      "-chmod_locking      <int>   Encoder channel mode locking mode:\n"
      "                            0 = Auto: Channel mode is driven by the input channel mode (default).\n"
      "                            1 = 5.1 Locked: Channel mode is always 5.1.\n"
      "-p                  <int>   RISC precision flag (fixpoint backends only)\n"
      "                            0 = PCM data has less precision (e.g. 16 bit for 16x32) \n"
      "                            1 = PCM data has high precision (default, e.g. 32 bit for 16x32) \n"
      "-u                * <int>   Dual-mono reproduction mode \n"
      "                            0 = Stereo (Default); \n"
      "                            1 = Left / first channel; \n"
      "                            2 = Right / second channel \n"
      "-v                  <int>   Verbosity \n"
      "                            0 = Silent Mode, only error messages are shown\n"
      "                            1 = Banner output + processing status + stream information + summary\n"
      "                            2 = the above + Parameter Settings + substream information\n"
      "                            3 = the above + output parameter in a \"grep friendly\" format\n"
      "-w                  <int>   Output WAV file bit-depth\n"
      "                            16, 24 or 32\n"
      "-xa               * <int>   Associated audio mixing \n"
      "                            0 = Off \n"
      "                            1 = On (Default) \n"
      "-xs               * <int>   System / Application audio mixing \n"
      "                            0 = Off \n"
      "                            1 = On (Default) \n"
      "-xu               * <int>   User control values for mixing main and associated audio \n"
      "                            Range: -32 (mute assoc) to 32 (mute main) in steps of 1 (dB) \n"
      "                            1 ... 31   - dB to favor associated (attenuate main) \n"
      "                            -1 ... -31 - dB to favor main (attenuate associated) \n"
    );
    fprintf(stdout,
      "-main1_mixgain    * <3 int> Input mixer gain values for Main program input \n"
      "                            - target gain at end of ramp in dB (range: -96..0) \n"
      "                            - duration of ramp in milliseconds (range: 0..60000) \n"
      "                            - shape of the ramp (0: linear, 1: in cube, 2: out cube) \n"
      "-main2_mixgain    * <3 int> Input mixer gain values for 2nd Main program input \n"
      "                            - target gain at end of ramp in dB (range: -96..0) \n"
      "                            - duration of ramp in milliseconds (range: 0..60000) \n"
      "                            - shape of the ramp (0: linear, 1: in cube, 2: out cube) \n"
      "-sys_prim_mixgain * <3 int> System sound mixer gain values for primary input (Input/AD mixer output) \n"
      "                            - target gain at end of ramp in dB (range: -96..0) \n"
      "                            - duration of ramp in milliseconds (range: 0..60000) \n"
      "                            - shape of the ramp (0: linear, 1: in cube, 2: out cube) \n"
      "-sys_apps_mixgain * <3 int> System sound mixer gain values for Application Sounds input \n"
      "                            - target gain at end of ramp in dB (range: -96..0) \n"
      "                            - duration of ramp in milliseconds (range: 0..60000) \n"
      "                            - shape of the ramp (0: linear, 1: in cube, 2: out cube) \n"
      "-sys_syss_mixgain * <3 int> System sound mixer gain values for System Sounds input \n"
      "                            - target gain at end of ramp in dB (range: -96..0) \n"
      "                            - duration of ramp in milliseconds (range: 0..60000) \n"
      "                            - shape of the ramp (0: linear, 1: in cube, 2: out cube) \n"
      "\n"
    );

    return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_common_switches_lc(void)
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
#ifdef MS12_AC4_SUPPORT
      "-it              <str>  Use case input type (DDP|AC4|PCM)\n"
#else
      "-it              <str>  Use case input type (DDP|PCM)\n"
#endif
      "                        default: derived from -im filename extension\n"
      "                        can be used to select the codec in mp4 container files\n"
      "-oms             <str>  pcm 2-channel downmix output filename \n"
      "-o_dap_speaker   <str>  DAPv2 'virtualized for speaker' output filename\n"
      "-o_dap_headphone <str>  DAPv2 'virtualized for headphone' output filename\n"
      "-od              <str>  Dolby Digital output filename \n"
      "-odp             <str>  Bitstream Pass-Through (Dolby Digital Plus) output filename \n\n"
      "Note: Input filenames starting with \"dummy.\" can be used to specify \"no current input\".\n"
      "      This can be used for a delayed input start or for an input stop during run-time.\n\n"
    );

    fprintf(stdout,
      "Functional Switches \n\n"
      "Switch   Data   Function \n\n"
      "-lc             Flag to activate Low Complexity Mode (Note: limited functionality) \n"
      "                use 'MultistreamDecoder -h lc' to get the low-complexity usage help context. \n"
      "-b     * <int>  Scale factor for incoming DRC boost value \n"
      "                0 - 100; Default = 100 \n"
      "-c     * <int>  Scale factor for incoming DRC cut value \n"
      "                0 - 100; Default = 100 \n"
      "-bs    * <int>  Scale factor for incoming DRC boost value for 2-channel downmix \n"
      "                0 - 100; Default = 100 \n"
      "-cs    * <int>  Scale factor for incoming DRC cut value for 2-channel downmix \n"
      "                0 - 100; Default = 100 \n"
      "-chs   * <int>  Channel configuration of System Sounds input \n"
      "                0 = reserved\n"
      "                1 = 1/0 (C)\n"
      "                2 = 2/0 (L, R) (default)\n"
      "-dbgout  <int>  Bitmask to activate different debug wave output files (default: none)\n");
    fprintf(stdout,
      "                0x%04X: Main decoder output\n", DBG_OUT_MAIN_DEC);
    fprintf(stdout,
      "-drc   * <int>  DRC modes \n"
      "                0 = Line (Default) \n"
      "                1 = RF \n"
      "-dmx   * <int>  Downmix modes \n"
      "                0 = Lt/Rt (Default) \n"
      "                1 = Lo/Ro \n"
      "-h              Help \n"
      "-p       <int>  RISC precision flag (fixpoint backends only)\n"
      "                0 = PCM data has less precision (e.g. 16 bit for 16x32) \n"
      "                1 = PCM data has high precision (default, e.g. 32 bit for 16x32) \n"
      "-u     * <int>  Dual-mono reproduction mode \n"
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
MS_RETVAL ms_show_usage_context_ddplus(void)
{
    /* Display all ddplus-specific switches */
    fprintf(stdout,
        "DDPLUS SWITCHES \n\n"
        "Switch   Data    Function \n\n"
        "-at   *  <int>   [ddplus] Associated substream selection \n"
        "                 1 - 3; No default \n"
        "\n"
  );

    return MS_OK;
}

#ifdef MS12_AC4_SUPPORT
MS_RETVAL ms_show_usage_context_ac4(void)
{
    /* Display all ac4-specific switches */
    fprintf(stdout,
        "DOLBY AC4 SWITCHES \n\n"
        "Switch                Data  Function \n\n"
        "-ac4_mode             <int> [ac4] AC4 decoding mode:\n"
        "                            0: Single input stream, single decode\n"
        "                            1: Single input stream, dual decode, single decoder instance \n"
        "                               (decoder internal mixing)\n"
        "                               NOTE: This is an experimental mode, and not tested!\n"
        "                            2: Single input stream, dual decode, dual decoder instance\n"
        "                               (decoder external mixing, default for single input)\n"
        "                            3: Dual input stream, dual decode (default for dual input)\n"
        "-lang               * <str> [ac4] 1st Preferred Language code (3 Letter ISO 639)\n"
        "-lang2              * <str> [ac4] 2nd Preferred Language code (3 Letter ISO 639)\n"
        "-at                 * <int> [ac4] Preferred associated type of service\n"
        "                            1: Visually Impaired (VI) (Default)\n"
        "                            2: Hearing Impaired (HI)\n"
        "                            3: Commentary\n"
        "-pat                * <int> [ac4] Prefer Presentation Selection by associated type over language.\n"
        "                            0: Prefer selection by language\n"
        "                            1: Prefer selection by associated type (default)\n"
        "-ac4_main_pres_idx  * <int> [ac4] Presentation index to be decoded. Overrides the presentation selection by preferred\n"
        "                                  language and associated type.\n"
        "                            0...%d: Presentation index\n"
        "                           -1:      Switch back to automatic selection by language and associated type (default)\n"
        "-ac4_assoc_pres_idx * <int> [ac4] Presentation index to be decoded by the associated instance.\n"
        "                            0...%d: Presentation index\n"
        "                           -1:      None. Decoder will decode the default presentation (default)\n"
        "-ac4_de             * <int> [ac4] Dialogue Enhancement gain that will be applied in the decoder\n"
        "                            Range: 0 to 12 dB (in 1 dB steps, default is 0 dB)\n"
        "-ac4_prog_id_type   * <int> [ac4] Determines the program identifer type to look for\n"
        "                            0: None (default)\n"
        "                            1: Short Program identifer (16 bit value)\n"
        "                            2: UUID: (128 bit, given as 16 8 bit values)\n"
        "-ac4_prog_id        * <var> [ac4] The program identifier is either an unsigned 16 bit value (ac4_prog_id_type==1)\n"
        "                            or 128 bit given as an 32 character hexadecimal string without the leading 0x (ac4_prog_id_type==2)\n"
        "                            Example for the UUID format: -ac4_prog_id 0BADCAFE0123456789ABCDEF01234567\n"
        "\n",
        MS_MAX_AC4_PRES_IDX, MS_MAX_AC4_PRES_IDX
        );

    return MS_OK;
}
#endif

/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_context_pcm(void)
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
      "                21 = 3/2/2 (L, C, R, l, r, Lrs, Rrs)\n"
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
      "\n"
  );
    return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_context_pcm_lc(void)
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
      "\n"
  );
    return MS_OK;
}


/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_context_heaac(void)
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
MS_RETVAL ms_show_usage_context_dap(void)
{
    /* Display all DAP-specific switches */
  fprintf(stdout,
      "DAP SWITCHES (device specific)\n"
      "  (Multiple values are expected as comma seperated values without spaces)\n\n"
      "Switch                         Data    Function \n\n"
      "-dap_calibration_boost       * <int>   (0...192, def: 0)\n"
      "-dap_dmx                     * <int>   DAP Downmix mode\n"
      "                                       0 = Lt/Rt (Default) \n"
      "                                       1 = Lo/Ro \n"
#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
      "-dap_gains                   * <3 int> \n"
#else
      "-dap_gains                   * <1 int> \n"
#endif
      "                                       - postgain (-2080...480, def: 0)\n"
#ifdef DAP_PRE_AND_SYSTEM_GAIN_SUPPORTED
      "                                       - pregain (-2080...480, def: 0)\n"
      "                                       - systemgain (-2080...480, def: 0)\n"
#endif
      "-dap_surround_decoder_enable * <int>   DAP surround decoder enable flag (Default 1)\n"
      "-dap_surround_virtualizer    * <5 int> Virtualizer Parameter\n"
      "                                       - virtualizer_enable (0,1, def: 1)\n"
      "                                       - headphone_reverb (-2080...192, def: 0)\n"
      "                                       - speaker_angle (5..30, def: 10)\n"
      "                                       - speaker_start (20...2000, def: 20)\n"
      "                                       - surround_boost (0...96, def: 96)\n"
      "-dap_graphic_eq              * <2+2*nb_bands int>\n"
      "                                       - eq_enable (0,1, def: 0)\n"
      "                                       - eq_nb_bands (1...20, def: 10)\n"
      "                                       - eq_band_center (20...20000, def: {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000})\n"
      "                                       - eq_band_target (-576...576, def: {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}\n"
      "-dap_optimizer               * <2+nb_bands+MAX_CHANNELS*nb_bands int>\n"
      "                                       - optimizer_enable (0,1, def: 0)\n"
      "                                       - opt_nb_bands (1...20, def: 10)\n"
      "                                       - opt_band_center_freq (20...20000, def: {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000})\n"
      "                                       - opt_band_gains[MAX_CHANNELS] (-480...480, def: {{10*0, 10*0, 10*0, 10*0, 10*0, 10*0, 10*0, 10*0})\n"
  );
    fprintf(stdout,
      "-dap_bass_enhancer           * <4 int>\n"
      "                                       - bass_enable (0,1, def: 0)\n"
      "                                       - bass_boost (0...384, def: 192)\n"
      "                                       - bass_cutoff (20...20000, def: 200)\n"
      "                                       - bass_width (2..64, def: 16)\n"
      "-dap_regulator               * <6+4*nb_bands int>\n"
      "                                       - regulator_enable (0,1, def: 1)\n"
      "                                       - regulator_mode (0,1, def: 0)\n"
      "                                       - regulator_overdrive (0...192, def: 0)\n"
      "                                       - regulator_timbre (0...16, def: 16)\n"
      "                                       - regulator_distortion (0...144, def: 96)\n"
      "                                       - reg_nb_bands (1...20, def: 2)\n"
      "                                       - reg_band_center (20...20000, def: {20,20000}\n"
      "                                       - reg_low_thresholds (-2080...0, def: {-192, -192})\n"
      "                                       - reg_high_thresholds (-2080...0, def: {0, 0})\n"
      "                                       - reg_isolated_bands (0,1 def: {0,0})\n"
      "-dap_virtual_bass            * <10 int>\n"
      "                                       - virtual_bass_mode (0...3, def: 0)\n"
      "                                       - virtual_bass_low_src_freq (30...90, def: 35)\n"
      "                                       - virtual_bass_high_src_freq (90...270, def: 160)\n"
      "                                       - virtual_bass_overall_gain(-480...0, def: 0)\n"
      "                                       - virtual_bass_slope_gain (-3...0, def: 0)\n"
      "                                       - virtual_bass_subgains[3] (-480...0, def: {-32,-144,-192})\n"
      "                                       - virtual_bass_low_mix_freq (0...375, def: 94)\n"
      "                                       - virtual_bass_high_mix_freq (281...938, def: 469)\n"
      "\n"
      );
  fprintf(stdout,
      "DAP SWITCHES (content specific)\n"
      "  (Multiple values are expected as comma seperated values without spaces)\n\n"
      "Switch                         Data    Function \n\n"
      "-dap_mi_steering             * <4 int> Media Intelligence Steering\n"
      "                                       - mi_ieq_enable (0,1, def: 0)\n"
      "                                       - mi_dv_enable (0,1, def: 0)\n"
      "                                       - mi_de_enable (0,1, def: 0)\n"
      "                                       - mi_surround_enable (0,1, def: 0)\n"
      "-dap_leveler                 * <3 int>\n"
      "                                       - leveler_enable (0,1, def: 0)\n"
      "                                       - leveler_amount (0...10, def: 7)\n"
      "                                       - leveler_ignore_il (0, 1, def: 0)\n"
      "-dap_ieq                     * <3+2*nb_bands int>\n"
      "                                       - ieq_enable (0,1, def: 0)\n"
      "                                       - ieq_amount (0...16, def: 10)\n"
      "                                       - ieq_nb_bands (1...20, def: 10)\n"
      "                                       - ieq_band_center (20...20000, def: {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000})\n"
      "                                       - ieq_band_target (-480...480, def: {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}\n"
      "-dap_dialogue_enhancer       * <3 int>\n"
      "                                       - de_enable (0,1, def: 0)\n"
      "                                       - de_amount (0...16, def: 0)\n"
      "                                       - de_ducking (0...16, def: 0)\n"
      "\n"
  );
    return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_usage_command_txt(void)
{
    fprintf(stdout,
      "USAGE OF THE \"%s\" FILE:\n"
      "\n"
      "Instead of passing arguments directly on the command line, they can be given in a textfile \"%s\" using the same syntax.\n"
      "Each line starts with a timestamp followed by the command line parameters to be changed at this point in time.\n"
      "The timestamp can be given in the following format:\n"
      "  samples    s<int>          (integer prefixed with s)\n"
      "  frames      <int>          (integer without prefix)\n"
      "  time       hh:mm:ss[.ss]\n"
      "Note that the timestamp is only evaluated on a frame by frame basis in the integration code. Arguments which are not settable during runtime\n"
      "can only be provided in the first line with timestamp equivalent to frame 0.\n"
      "\n"
      "A typical command.txt might look like the following example which turns off the leveler in frame 20 and turns it back on in frame 40:\n"
      "     0  -im input.ec3 -om out.wav -dap_init_mode 1 -dap_leveler 1,7,0\n"
      "     20 -dap_leveler 0,7,0\n"
      "     40 -dap_leveler 1,7,0\n"
      "\n"
      ,MS_COMMAND_FILE, MS_COMMAND_FILE
      );
    return MS_OK;
}

/*
 * Documentation resides in declaration only
 */
MS_RETVAL ms_show_banner(void)
{

    fprintf(stdout, "\n************** Dolby MultiStreamDecoder Version 1.3.1 **************\n");


    /* display copyright info */
    fprintf(stdout, "%s", ms_dolby_copyright);
    return MS_OK;
}




static const char *enable_to_str(int b_enable)
{
    if(b_enable <= 1)
        return enable2str[b_enable + 1];
    else
        return enable2str[2];
}
