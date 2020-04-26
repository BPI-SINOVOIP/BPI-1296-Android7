#ifndef _MS12_METADATA_H_
#define _MS12_METADATA_H_

#define MD_TYPE_TIMED          0  /**< Metadata has a timestamp and length attached. */
#define MD_TYPE_TIMED_SINGULAR 1  /**< Metadata has a timestamp, but will not be split if accessed only partially.
                                       The node need to take care of storing it over multiple calls. */
#define MD_TYPE_STATIC         2  /**< Metadata is not related to a timestamp */


#define MSF_MD_DUALMONO_CHANNELS        2
#define MSF_MD_MAX_DD_BLOCKS_PER_FRAME  6
#define MSF_MD_MAX_AAC_BLOCKS_PER_FRAME 8

#define MSF_PCM_MIXING_NONE             0 /**< Value for #MD_ID_PCM_MIXING indicating that the signal has not been mixed. */
#define MSF_PCM_MIXING_LONG_TERM        1 /**< Value for #MD_ID_PCM_MIXING indicating that the signal has been mixed with a long term signal. */
#define MSF_PCM_MIXING_SHORT_TERM       2 /**< Value for #MD_ID_PCM_MIXING indicating that the signal has been mixed with a short term signal. */
#define MSF_PCM_MIXING_AGGREGATING      4 /**< Value for #MD_ID_PCM_MIXING indicating that the signal has been mixed using an aggregating mixer. */

#define MSF_AD_TYPE_NONE                0 /**< Value for #MD_ID_PREF_ASSOCIATED_TYPE indicating that no AD mixing content type is preferred. */
#define MSF_AD_TYPE_VISUALLY_IMPAIRED   1 /**< Value for #MD_ID_PREF_ASSOCIATED_TYPE indicating visually impaired content being preferred. */
#define MSF_AD_TYPE_HEARING_IMPAIRED    2 /**< Value for #MD_ID_PREF_ASSOCIATED_TYPE indicating hearing impaired content being preferred. */
#define MSF_AD_TYPE_COMMENTARY          3 /**< Value for #MD_ID_PREF_ASSOCIATED_TYPE indicating any commentary content being preferred. */

#define MD_ID_EOS                    1001  /**< Timed metadata indicating the end of the stream */

#define MD_ID_PCM_CHAN_MODE          1002  /**< Timed metadata, two integers: acmod, b_lfe */
#define MD_ID_PCM_SAMPLE_RATE        1003  /**< Timed metadata, one unsigned integer */

#define MD_ID_MIXER_MAIN_DMX         1101  /**< Timed metadata, indicates that main has been downmixed */
#define MD_ID_MIXER_MIX_MD_PES       1102  /**< Timed PES level mixing metadata, 5 integers:
                                                fade_byte, gain_byte_center, gain_byte_front, gain_byte_surround, pan_byte */
#define MD_ID_MIXER_MIX_MD_DDP       1103  /**< Timed metadata, #msf_udc_mix_md struct */

#define MD_ID_HEAAC_ASC              1300  /**<  Static metadata to configure the HE-AAC decoder for fileplayback:
                                                 array of char (bitstream containing the audio specific config */
#define MD_ID_HEAAC_DEF_DIALNORM     1301  /**<  Static metadata to set the default dialnorm: 1 integer */

#define MD_ID_DDP_DOWNMIX_MD         1400  /**< Timed metadata, see #msf_udc_pcmr_downmix_md struct */
#define MD_ID_DDP_DRC_COMPR_MD       1401  /**< Timed metadata, see #msf_udc_dynrng_compr_md struct */
#define MD_ID_DDP_2ND_DRC_COMPR_MD   1402  /**< Timed metadata, only for 2nd dual mono channel, see #msf_udc_dynrng_compr_md struct */

#define MD_ID_AAC_DOWNMIX_MD         1500  /**< Timed metadata, see msf_aac_downmix_md_s struct */
#define MD_ID_AAC_COMPR_DRC_MD       1501  /**< Timed metadata, see msf_aac_compr_drc_md_s struct */
#define MD_ID_AAC_2ND_COMPR_DRC_MD   1502  /**< Timed metadata, only for 2nd dual mono channel, see #msf_aac_compr_drc_md_s struct */

#define MD_ID_PCM_DMX_OUTPUT_USER_PARAMS 1600       /**< Static metadata, see #msf_pcm_output_user_params_s */
#define MD_ID_PCM_MC_OUTPUT_USER_PARAMS  1601       /**< Static metadata, see #msf_pcm_output_user_params_s */
#define MD_ID_DDP_SUBSTREAM_SELECT       1602       /**< Static metadata, 1 integer */
#define MD_ID_AD_MIXING_ENABLED          1603       /**< Static metadata, 1 integer, indicating whether AD mixing is enabled or not */
#define MD_ID_AD_MIXING_USER_PREF        1604       /**< Static metadata, 1 integer, indicating the user-controlled balance between main and associate audio after mixing. */
#define MD_ID_SYSSOUND_MIXING_ENABLED    1605       /**< Static metadata, 1 integer */

#define MD_ID_PCM_MIXING                 1606       /**< Timed metadata indicating if mixing happened. Possible values are
                                                       MSF_PCM_MIXING_NONE, MSF_PCM_MIXING_LONG_TERM or MSF_PCM_MIXING_SHORT_TERM. */
#define MD_ID_DELAY                      1608       /**< Timed metadata containing the current delay, 1 integer */

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

#define MD_ID_SERIALIZED_EVO             1630        /**< Evolution bitstream for transcoding */

#define MD_ID_PCM_LEVELED                1631        /**< Timed Metadata containing 2 integer indicating whether leveling [0]
                                                          or dial enhancement [1] has taken place */
#define MD_ID_DAP_VIRTUAL_BASS           1632

#define MD_ID_COMPRESSOR_PROFILE         1633        /**< Timed Metadata containg the DD+ compression profile signaled in he AC-4 bitstream used for reencoding to DD/DD+ as an single int. */
#define MD_ID_PREF_LANGUAGE_CODE         1634        /**< Static Metadata containing up to 3 characters (null terminated) for
                                                          indicating the preferred language to be selected. */
#define MD_ID_PREF_2ND_LANGUAGE_CODE     1635        /**< Static Metadata containing up to 3 characters (null terminated) for
                                                          indicating the preferred language to be selected, in case the preferred is not available. */
#define MD_ID_PREF_ASSOCIATED_TYPE       1636        /**< Static Metadata containing one integer indicating the user selected
                                                          preferred associated content type. */
#define MD_ID_AC4_DIALOGUE_ENH           1637        /**< Static Metadata containing one integer indicating the the amount of dialog enhancement to be applied by the AC4 decoder (0-12dB). */
#define MD_ID_AC4_MAIN_PRES_IDX          1638        /**< Static Metadata containing one integer indicating the presentation index to be
                                                          decoded by the primary instance. Overrides the presentation selection by language and assoc. type. */
#define MD_ID_AC4_ASSOC_PRES_IDX         1639        /**< Static Metadata containing one integer indicating the presentation index to be
                                                          decoded by the secondary instance. */

#define MD_ID_PCM_TRUSTED                1640        /**< Timed Metadata describing whether the PCM is properly leveled and dialog has been enhanced to prevent the same processing happening twice.
                                                          Payload is carried in msf_pcm_trust_indication_s. */

#define MD_ID_PREF_ASSOC_TYPE_OVER_LANGUAGE 1641     /**< Static Metadata containing one integer indicating priority for matching language vs matching associate type during presentation selection. */
#define MD_ID_SURROUNDS_PHASE90_SHIFTED     1642     /**< Timed Metadata describing whether a phase90 shift for the surrounds was already done in the encoder or decoder */
#define MD_ID_PROGRAM_ID_TYPE               1643     /**< Timed Metadata containing the program id type of the program to be decoded. The program identifier can be 0 - none, 1 - short ID or 2 - UUID. */
#define MD_ID_PROGRAM_ID                    1644     /**< Timed Metadata containing the program identifier to decode. Can be either the short program id (16bit=2bytes) or the UUID (128 bit, 16bytes).
                                                          The transported format is a string which either contains the 16bit value for the short program id or 32 characters in hexadecimal
                                                          format resulting in 16 bytes for the uuid. */

#define MD_ID_INPUT_MIXING_GAIN_INPUT1      1645     /**< Static Metadata containing a gain set for input 1 input mixing
                                                          in three integers: target_attenuation, duration_in_ms, type. */
#define MD_ID_INPUT_MIXING_GAIN_INPUT2      1646     /**< Static Metadata containing a gain set for input 2 input mixing
                                                          in three integers: target_attenuation, duration_in_ms, type. */
#define MD_ID_SYSSOUND_MIXING_GAIN_INPUT1   1647     /**< Static Metadata containing a gain set for input 1 input mixing (system/application sound mixer)
                                                          in three integers: target_attenuation, duration_in_ms, type. */
#define MD_ID_SYSSOUND_MIXING_GAIN_INPUT2   1648     /**< Static Metadata containing a gain set for input 2 input mixing (system/application sound mixer)
                                                          in three integers: target_attenuation, duration_in_ms, type. */
#define MD_ID_SYSSOUND_MIXING_GAIN_INPUT3   1649     /**< Static Metadata containing a gain set for input 3 input mixing (system/application sound mixer)
                                                          in three integers: target_attenuation, duration_in_ms, type. */
#define MD_ID_JOC_MD_PRESENT                1650     /**< Timed metadata, 1 integer */
#define MD_ID_INPUT_FILENAME                1651     /**< Static Metadata, string */

/* Helper Defines for assembling the PCM Renderer user parameters */
#define USER_PARAM_PCM_DMX_OUTPUT_ACTIVE           1  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DUAL_MONO_MODE   2  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_DMX_OUTPUT_COMPRESSION_MODE 3  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DMX_TYPE         4  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DRC_CUT          5  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_DMX_OUTPUT_DRC_BOOST        6  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_MC_OUTPUT_ACTIVE            7  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_MC_OUTPUT_DUAL_MONO_MODE    8  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_MC_OUTPUT_COMPRESSION_MODE  9  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_MC_OUTPUT_DRC_CUT          10  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_MC_OUTPUT_DRC_BOOST        11  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */
#define USER_PARAM_PCM_MC_OUTPUT_5_1_DMX          12  /**< internal set_param id for filling #msf_pcm_output_user_params_s struct */

#define USER_PARAM_EXTPCM_ACMOD                   13
#define USER_PARAM_EXTPCM_LFEON                   14
#define USER_PARAM_SYSSOUND_ACMOD                 15
#define USER_PARAM_SYSSOUND_LFEON                 16
#define USER_PARAM_APPSOUND_ACMOD                 17
#define USER_PARAM_APPSOUND_LFEON                 18


#endif /* _MS12_METADATA_H_ */
