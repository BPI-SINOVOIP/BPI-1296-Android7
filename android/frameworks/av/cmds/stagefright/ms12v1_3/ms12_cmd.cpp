#include <string.h>
#include "ms12_cmd.h"

void CopyMS12IinitParamToOMXAudio(OMX_AUDIO_MS_INIT_PARAMS*init_args , MS_INIT_PARAMS *ms_init_args)
{
    int i;
    memset(init_args,0,sizeof(OMX_AUDIO_MS_INIT_PARAMS));

    init_args->verbose_mode = ms_init_args->verbose_mode;
    init_args->wav_bit_depth = ms_init_args->wav_bit_depth;
    init_args->max_num_channels = ms_init_args->max_num_channels;
    init_args->b_high_risc_precision = ms_init_args->b_high_risc_precision;
    init_args->extpcm_compressor_profile = ms_init_args->extpcm_compressor_profile;
    init_args->heaac_allow_partial_feed = ms_init_args->heaac_allow_partial_feed;
    init_args->dap_output_mode = ms_init_args->dap_output_mode;
    init_args->dbg_output_mask = ms_init_args->dbg_output_mask;
    for(i = 0; i < 4; i++) init_args->pcm_out_chans[i] = ms_init_args->pcm_out_chans[i];

    init_args->enc_chmod_locking_mode = ms_init_args->enc_chmod_locking_mode;

    //ALOGD("[YLLIN][MS12][InitCfg]%d %d\n",init_args->max_num_channels,init_args->dap_output_mode);
    //init_args->max_num_channels = 8;
    //init_args->dap_output_mode = 0;

    //memcpy(init_args, &omx_dec->ms12Cfg->param_struct.ms_init_args.pcm_out_chans[0], sizeof(AUDIO_MS_INIT_PARAMS));
}

void CopyMS12UseCaseToOMXAudio(OMX_AUDIO_MS_USE_CASE *use_case, MS_USE_CASE *ms_use_case)
{
    memset(use_case,0,sizeof(OMX_AUDIO_MS_USE_CASE));

    use_case->input_type = (OMX_AUDIO_MS_INPUT_TYPE)ms_use_case->input_type;
    use_case->b_low_complexity = ms_use_case->b_low_complexity;
    use_case->b_dual_input = ms_use_case->b_dual_input;
    use_case->b_file_playback = ms_use_case->b_file_playback;
    use_case->b_restricted_ad = ms_use_case->b_restricted_ad;
    use_case->b_mainonly = ms_use_case->b_mainonly;
    use_case->b_system_sounds = ms_use_case->b_system_sounds;
    use_case->b_app_sounds = ms_use_case->b_app_sounds;
    use_case->b_eval_mode = ms_use_case->b_eval_mode;
    use_case->bs_output_mode = (OMX_AUDIO_ms_bs_output_mode_t)ms_use_case->bs_output_mode;
    use_case->dap_init_mode = (OMX_AUDIO_ms_dap_mode_t)ms_use_case->dap_init_mode;
    use_case->b_dap_vb_enable = ms_use_case->b_dap_vb_enable;

    use_case->b_dual_main = ms_use_case->b_dual_main;
#ifdef MS12_AC4_SUPPORT
    use_case->ac4_mode = ms_use_case->ac4_mode;
#endif

}

void CopyMS12RuntimeParamToOMXAudio(OMX_AUDIO_MS_RUNTIME_PARAMS *args, MS_RUNTIME_PARAMS *pParams)
{
    int i;
    memset(args,0,sizeof(OMX_AUDIO_MS_RUNTIME_PARAMS));

    args->drc_cut_fac_mc = pParams->drc_cut_fac_mc;
    args->drc_boost_fac_mc = pParams->drc_boost_fac_mc;
    args->drc_cut_fac_2ch = pParams->drc_cut_fac_2ch;
    args->drc_boost_fac_2ch = pParams->drc_boost_fac_2ch;
    args->downmix_type = pParams->downmix_type;
    args->drc_mode = pParams->drc_mode;
    args->dual_mono = pParams->dual_mono;
    args->cmix_lev = pParams->cmix_lev;
    args->smix_lev = pParams->smix_lev;
    args->multichannel_enable = pParams->multichannel_enable;
    args->associated_audio_mixing = pParams->associated_audio_mixing;
    args->user_balance_adjustment = pParams->user_balance_adjustment;
    //args->mixer_balance_adjustment = pParams->mixer_balance_adjustment;

    memcpy(&args->input_mix_input1,&pParams->input_mix_input1,sizeof(OMX_AUDIO_mix_fade_t));
    memcpy(&args->input_mix_input2,&pParams->input_mix_input2,sizeof(OMX_AUDIO_mix_fade_t));
    memcpy(&args->syssound_mix_input1,&pParams->syssound_mix_input1,sizeof(OMX_AUDIO_mix_fade_t));
    memcpy(&args->syssound_mix_input2,&pParams->syssound_mix_input2,sizeof(OMX_AUDIO_mix_fade_t));
    memcpy(&args->syssound_mix_input3,&pParams->syssound_mix_input3,sizeof(OMX_AUDIO_mix_fade_t));

    args->b_sys_app_sound_mixing = pParams->b_sys_app_sound_mixing;
    args->extpcm_num_in_samples = pParams->extpcm_num_in_samples;
    args->extpcm_in_channel_config.acmod = pParams->extpcm_in_channel_config.acmod;
    args->extpcm_in_channel_config.b_lfe_present = pParams->extpcm_in_channel_config.b_lfe_present;
    args->extpcm_in_channel_config.dsurmod = pParams->extpcm_in_channel_config.dsurmod;

    args->sys_sounds_num_in_samples = pParams->sys_sounds_num_in_samples;;
    args->sys_sounds_channel_config.acmod = pParams->sys_sounds_channel_config.acmod;
    args->sys_sounds_channel_config.b_lfe_present = pParams->sys_sounds_channel_config.b_lfe_present;
    args->sys_sounds_channel_config.dsurmod = pParams->sys_sounds_channel_config.dsurmod;

    args->app_sounds_num_in_samples = pParams->app_sounds_num_in_samples;
    args->app_sounds_channel_config.acmod = pParams->app_sounds_channel_config.acmod;
    args->app_sounds_channel_config.b_lfe_present = pParams->app_sounds_channel_config.b_lfe_present;
    args->app_sounds_channel_config.dsurmod = pParams->app_sounds_channel_config.dsurmod;

    args->heaac_mixing_mode = pParams->heaac_mixing_mode;
    args->heaac_default_dialnorm = pParams->heaac_default_dialnorm;
    args->heaac_transport_format = pParams->heaac_transport_format;
    args->ddplus_associated_substream = pParams->ddplus_associated_substream;
    args->ddplus_outlfe = pParams->ddplus_outlfe;
    args->ddplus_outmode = pParams->ddplus_outmode;
    args->b_mc_5_1_dmx = pParams->b_mc_5_1_dmx;
    args->frame = pParams->frame;

#ifdef MS12_AC4_SUPPORT
  /* AC4 parameters */
    args->ac4_associated_type = pParams->ac4_associated_type;
    args->b_ac4_pref_assoc_type_over_lang = pParams->b_ac4_pref_assoc_type_over_lang;
    args->ac4_de_gain = pParams->ac4_de_gain;
    args->ac4_pres_index[0] = pParams->ac4_pres_index[0];
    args->ac4_pres_index[1] = pParams->ac4_pres_index[1];
    args->ac4_prog_id_type = pParams->ac4_prog_id_type;
    memcpy(&args->ac4_1st_pref_lang[0],&pParams->ac4_1st_pref_lang[0],MS_AC4_LANG_MAX_STRLEN);
    memcpy(&args->ac4_2nd_pref_lang[0],&pParams->ac4_2nd_pref_lang[0],MS_AC4_LANG_MAX_STRLEN);
    memcpy(&args->ac4_program_identifier_string[0],&pParams->ac4_program_identifier_string[0],(2*MS_PROGRAM_UUID_LEN_BYTES));
#endif

    //dap_param_t
    dap_params_t *pParamsDAP = (dap_params_t*) &pParams->dap_parameter;
    OMX_AUDIO_dap_params_t *argsDAP = (OMX_AUDIO_dap_params_t*) &args->dap_parameter;

    argsDAP->drc_type = pParamsDAP->drc_type;
    argsDAP->dmx_type = pParamsDAP->dmx_type;
    argsDAP->speaker_lfe = pParamsDAP->speaker_lfe;
    argsDAP->pregain = pParamsDAP->pregain;
    argsDAP->postgain = pParamsDAP->postgain;
    argsDAP->systemgain = pParamsDAP->systemgain;
    argsDAP->surround_decoder_enable = pParamsDAP->surround_decoder_enable;
    argsDAP->virtualizer_enable = pParamsDAP->virtualizer_enable;
    argsDAP->headphone_reverb = pParamsDAP->headphone_reverb;
    argsDAP->speaker_angle = pParamsDAP->speaker_angle;
    argsDAP->speaker_start = pParamsDAP->speaker_start;
    argsDAP->surround_boost = pParamsDAP->surround_boost;
    argsDAP->mi_ieq_enable = pParamsDAP->mi_ieq_enable;
    argsDAP->mi_dv_enable = pParamsDAP->mi_dv_enable;
    argsDAP->mi_de_enable = pParamsDAP->mi_de_enable;
    argsDAP->mi_surround_enable = pParamsDAP->mi_surround_enable;
    argsDAP->calibration_boost = pParamsDAP->calibration_boost;
    argsDAP->leveler_amount = pParamsDAP->leveler_amount;
    argsDAP->leveler_input = pParamsDAP->leveler_input;
    argsDAP->leveler_output = pParamsDAP->leveler_output;
    argsDAP->leveler_enable = pParamsDAP->leveler_enable;
    argsDAP->leveler_ignore_il = pParamsDAP->leveler_ignore_il;
    argsDAP->modeler_enable = pParamsDAP->modeler_enable;
    argsDAP->modeler_calibration = pParamsDAP->modeler_calibration;
    argsDAP->ieq_enable = pParamsDAP->ieq_enable;
    argsDAP->ieq_amount = pParamsDAP->ieq_amount;
    argsDAP->ieq_nb_bands = pParamsDAP->ieq_nb_bands;
    for(i = 0; i<OMX_AUDIO_DAP_IEQ_MAX_BANDS ; i++)
        argsDAP->a_ieq_band_center[i] = pParamsDAP->a_ieq_band_center[i];
    for(i = 0; i<OMX_AUDIO_DAP_IEQ_MAX_BANDS ; i++)
        argsDAP->a_ieq_band_target[i] = pParamsDAP->a_ieq_band_target[i];

    argsDAP->de_enable = pParamsDAP->de_enable;
    argsDAP->de_amount = pParamsDAP->de_amount;
    argsDAP->de_ducking = pParamsDAP->de_ducking;
    argsDAP->volmax_boost = pParamsDAP->volmax_boost;
    argsDAP->eq_enable = pParamsDAP->eq_enable;
    argsDAP->eq_nb_bands = pParamsDAP->eq_nb_bands;
    for(i = 0; i<OMX_AUDIO_DAP_GEQ_MAX_BANDS ; i++)
        argsDAP->a_geq_band_center[i] = pParamsDAP->a_geq_band_center[i];
    for(i = 0; i<OMX_AUDIO_DAP_GEQ_MAX_BANDS ; i++)
        argsDAP->a_geq_band_target[i] = pParamsDAP->a_geq_band_target[i];

    argsDAP->optimizer_enable = pParamsDAP->optimizer_enable;
    argsDAP->optimizer_nb_bands = pParamsDAP->optimizer_nb_bands;
    for(i = 0; i<OMX_AUDIO_DAP_OPT_MAX_BANDS ; i++)
        argsDAP->a_opt_band_center_freq[i] = pParamsDAP->a_opt_band_center_freq[i];

    //a_opt_band_gain[OMX_AUDIO_DAP_MAX_CHANNELS][OMX_AUDIO_DAP_OPT_MAX_BANDS];
    //memcpy(&argsDAP->a_opt_band_gain[0][0],&pParamsDAP->a_opt_band_gain[0][0],(OMX_AUDIO_DAP_MAX_CHANNELS*OMX_AUDIO_DAP_OPT_MAX_BANDS));
    int j;
    for(j = 0; j < OMX_AUDIO_DAP_MAX_CHANNELS; j++)
    {
        for(i = 0; i<OMX_AUDIO_DAP_OPT_MAX_BANDS ; i++)
            argsDAP->a_opt_band_gain[j][i] = pParamsDAP->a_opt_band_gain[j][i];
    }

    argsDAP->bass_enable = pParamsDAP->bass_enable;
    argsDAP->bass_boost = pParamsDAP->bass_boost;
    argsDAP->bass_cutoff = pParamsDAP->bass_cutoff;
    argsDAP->bass_width = pParamsDAP->bass_width;
    argsDAP->reg_nb_bands = pParamsDAP->reg_nb_bands;
    for(i = 0; i<OMX_AUDIO_DAP_REG_MAX_BANDS ; i++)
        argsDAP->a_reg_band_center[i] = pParamsDAP->a_reg_band_center[i];
    for(i = 0; i<OMX_AUDIO_DAP_REG_MAX_BANDS ; i++)
        argsDAP->a_reg_low_thresholds[i] = pParamsDAP->a_reg_low_thresholds[i];
    for(i = 0; i<OMX_AUDIO_DAP_REG_MAX_BANDS ; i++)
        argsDAP->a_reg_high_thresholds[i] = pParamsDAP->a_reg_high_thresholds[i];
    for(i = 0; i<OMX_AUDIO_DAP_REG_MAX_BANDS ; i++)
        argsDAP->a_reg_isolated_bands[i] = pParamsDAP->a_reg_isolated_bands[i];

    argsDAP->regulator_overdrive = pParamsDAP->regulator_overdrive;
    argsDAP->regulator_timbre = pParamsDAP->regulator_timbre;
    argsDAP->regulator_distortion = pParamsDAP->regulator_distortion;
    argsDAP->regulator_mode = pParamsDAP->regulator_mode;
    argsDAP->regulator_enable = pParamsDAP->regulator_enable;
    argsDAP->vb_mode = pParamsDAP->vb_mode;
    argsDAP->vb_low_src_freq = pParamsDAP->vb_low_src_freq;
    argsDAP->vb_high_src_freq = pParamsDAP->vb_high_src_freq;
    argsDAP->vb_overall_gain = pParamsDAP->vb_overall_gain;
    argsDAP->vb_slope_gain = pParamsDAP->vb_slope_gain;
    for(i = 0; i<3 ; i++)
        argsDAP->vb_subgain[i] = pParamsDAP->vb_subgain[i];
    argsDAP->vb_mix_low_freq = pParamsDAP->vb_mix_low_freq;
    argsDAP->vb_mix_high_freq = pParamsDAP->vb_mix_high_freq;

    //memcpy(args, &omx_dec->ms12Cfg->param_struct.ms_runtime_args, sizeof(OMX_AUDIO_MS_RUNTIME_PARAMS));
}