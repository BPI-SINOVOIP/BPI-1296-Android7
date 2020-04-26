/** @ingroup msd_frontend */

/** @file
 *  @brief Multistream Decoder Front-end - Error Handling - code file
 */

/**** Module Dependencies ****/

/* ANSI C standard library headers */
#include <stdio.h>
#include <string.h>

/* executive headers */
#include "ms12_err.h"
#include "ms12_cmd.h"

/** This tables translates error codes of the multistream decoder into descriptive strings */
const MS_ERR ms_errmsgtab[] =
{
	/* EXECUTIVE LAYER ERRORS */
	{MS_ERR_INVALID_INPUT_FILE_TYPE,          "Unsupported input file type"},
	{MS_ERR_INVALID_INPUT_FILE,               "Invalid input file (-im / -ia)"},
	{MS_ERR_INCOMPATIBLE_INPUTS,              "Input files must have identical extensions (-im / -ia)"},
	{MS_ERR_UNKNOWN_ERRCODE,                  "Unknown error code"},
	{MS_ERR_INVALID_OUTPUT_FILE_TYPE,         "Unsupported output file type"},
	{MS_ERR_INVALID_OUTPUT_FILE,              "Invalid output file (-om / -oa)"},
	{MS_ERR_INVALID_DD_OUTPUT_FILE,           "Invalid DD output file (-od)"},
	{MS_ERR_INVALID_2CH_OUTPUT_FILE,          "Invalid 2-channel output file (-oms)"},
	{MS_ERR_INVALID_DRC_BOOST_FACTOR,         "Invalid DRC boost factor (-b / -bs)"},
	{MS_ERR_INVALID_DRC_CUT_FACTOR,           "Invalid DRC cut factor (-c / -cs)"},
	{MS_ERR_INVALID_DMX_MODE,                 "Invalid DMX mode (-dmx)"},
	{MS_ERR_INVALID_DRC_MODE,                 "Invalid DRC mode (-drc)"},
	{MS_ERR_INVALID_DUALMONO_MODE,            "Invalid dual-mono reproduction mode (-u)"},
	{MS_ERR_INVALID_VERBOSE_MODE,             "Invalid verbosity level (-v)"},
 	{MS_ERR_INVALID_ASSOCIATED_SUBSTREAM,     "Invalid DDPlus associated substream (-at) "},
	{MS_ERR_INVALID_DDP_DEBUG_VALUE,          "Invalid DDPlus debug value (-da / -dd / -df / -dm / -dp)"},
	{MS_ERR_INVALID_COMPRESSOR_PROFILE,       "Invalid compresor profile (-rp)"},
	{MS_ERR_INVALID_PROGRAM_REF_LEVEL,        "Invalid program reference level (-rr)"},
	{MS_ERR_ASSOCIATED_2CH_RESTRICT,          "Error restricting associated instance to 2 channels (-as: 0,1)"},
	{MS_ERR_INVALID_DIALNORM_VALUE,           "Invalid dialnorm value (-dn)"},
	{MS_ERR_INVALID_TRANSPORT_FORMAT,         "Invalid transport format (-t)"},
	{MS_ERR_INVALID_MULTICHANNEL_MODE,        "Invalid multichannel mode (-mc)"},
	{MS_ERR_INVALID_ASSOC_MIXING_VALUE,       "Invalid associated program mixing value (-xa)"},
	{MS_ERR_INVALID_SYSSOUND_MIXING_VALUE,    "Invalid system sound mixing value (-xs)"},
	{MS_ERR_INVALID_USER_BALANCE_VALUE,       "Invalid user balance adjustment value (-xu)"},
	{MS_ERR_INVALID_MIXER_BALANCE_VALUE,       "Invalid application mixing balance adjustment value (-xsu)"},
	{MS_ERR_UNSUPPORTED_PARAMETER,            ""},
	{MS_ERR_MISSING_INPUT_MAIN,               "Main input file must be specified (-im)"},
	{MS_ERR_MISSING_OUTPUT_MAIN,              "Main output file must be specified (-om)"},
	{MS_ERR_MISSING_COMMAND_FILE,             "The file \"command.txt\" was not found"},
	{MS_ERR_NOTAPP_MULTICHANNEL_MODE,         "Multichannel mode is not applicable for HE-AAC operation (-mc)"},
	{MS_ERR_OUTOFRANGE_DDP_DBG_FRAME,         "Dolby Digital Plus frame level debug value out-of-range (-df: 0x0 - 0x0000001F)"},
	{MS_ERR_OUTOFRANGE_DDP_DBG_DEC,           "Dolby Digital Plus frame level debug value out-of-range (-df: 0x0 - 0x004FFFFF)"},
	{MS_ERR_OUTOFRANGE_PCM_COMP_PROFILE,      "External PCM compressor profile value out-of-range (-rp: 0 - 5)"},
	{MS_ERR_OUTOFRANGE_PCM_PROG_REF_LEVEL,    "External PCM program reference level value out-of-range (-rr: 0-31)"},
	{MS_ERR_OUTOFRANGE_HEAAC_DIALNORM,        "HE-AAC dialnorm value out-of-range (-dn: 0 - 127)"},
	{MS_ERR_OUTOFRANGE_HEAAC_TRANSFMT,        "HE-AAC transport format value out-of-range (-t: 0 - 3)"},
	{MS_ERR_OUTOFRANGE_DUALMONO,              "Dual-mono value out-of-range (-u: 0 - 2)"},
	{MS_ERR_OUTOFRANGE_DRC_BOOST,             "DRC boost value out-of-range (-b / -bs: 0 - 100)"},
	{MS_ERR_OUTOFRANGE_DRC_CUT,               "DRC cut value out-of-range (-c / -cs: 0 - 100)"},
	{MS_ERR_OUTOFRANGE_ST_DOWNMIX,            "Stereo downmix value out-of-range (-dmx: 0-1 (Dolby Digital Plus), 0-2 (HE-AAC))"},
	{MS_ERR_OUTOFRANGE_DRC_MODE,              "DRC mode value out-of-range (-drc: 0-1)"},
	{MS_ERR_OUTOFRANGE_ASSOC_MIX,             "Associated mix value out-of-range (-xa: 0-1)"},
	{MS_ERR_OUTOFRANGE_SYSSOUND_MIX,          "System sound mix value out-of-range (-xs: 0-1)"},
	{MS_ERR_OUTOFRANGE_USER_BALANCE,          "User balance adjustment value out-of-range (-xu: -32 to 32)"},
	{MS_ERR_OUTOFRANGE_MIXER_BALANCE,         "Application mixing balance adjustment value out-of-range (-xsu: -32 to 32)"},
	{MS_ERR_OUTOFRANGE_ASSOCIATED_SUBSTREAM,  "Associated substream value out-of-range (-at: 1-3)"},
	{MS_ERR_INVALID_DDP_2CH_MODE,             "2-channel downmix mode out-of-range for Dolby Digital Plus (-dmx: 0-1)"},
	{MS_ERR_OUTOFRANGE_WAVFILEBITDEPTH,       "Bit-depth of output WAV file out-of-range (-w: 16, 24 or 32)"},
	{MS_ERR_INVALID_WAVFILEBITDEPTH,          "Bit-depth of output WAV file invalid (-w: 16, 24 or 32)"},
	{MS_ERR_OUTOFRANGE_EXTPCM_ACMOD,          "Channel configuration of the external PCM input out-of-range (-chp: 0, 1, 2 or 7)"},
	{MS_ERR_INVALID_EXTPCM_ACMOD,             "Channel configuration of the external PCM input invalid (-chp: 0, 1, 2 or 7)"},
	{MS_ERR_OUTOFRANGE_SYS_SOUNDS_ACMOD,      "Channel configuration of the system sounds input out-of-range (-chs: 0-7)"},
	{MS_ERR_INVALID_SYS_SOUNDS_ACMOD,         "Channel configuration of the system sounds input invalid (-chs: 0-7)"},
	{MS_ERR_INVALID_LFE,                      "LFE presence value invalid (-lp / -ls: 0-1)"},
        {MS_ERR_EXTPCM_ASSOCIATED_INPUT,          "Associated stream not supported for external PCM"},
	{MS_ERR_OUTOFRANGE_DAP_INIT_MODE,         "DAP init mode out of range (-dap_init_mode: 0, 1, 2 or 3)"},
	{MS_ERR_INVALID_DAP_INIT_MODE,            "DAP init mode is invalid (-dap_init_mode: 0, 1, 2 or 3)"},
	{MS_ERR_INVALID_DDP_OUTPUT_FILE,          "Invalid DD+ output file (-odp)"},
	{MS_ERR_OUTOFRANGE_APP_SOUNDS_ACMOD,      "Channel configuration of the application sounds input out-of-range (-chas: 0, 1, 2 or 7)"},
	{MS_ERR_INVALID_APP_SOUNDS_ACMOD,         "Channel configuration of the application sounds input invalid (-chas: 0, 1, 2 or 7)"},
	{MS_ERR_OUTOFRANGE_DAP_DMX_TYPE,          "DAP downmix type out of range (-dap_dmx: 0, 1)"},
	{MS_ERR_INVALID_DAP_DMX_TYPE,             "DAP downmix type is invalid (-dap_dmx: 0, 1)"},
	{MS_ERR_OUTOFRANGE_DAP_PARAM,             "DAP parameter out of range"},
	{MS_ERR_INVALID_DAP_VALUE,                "DAP parameter is invalid"},
	{MS_ERR_OUTOFRANGE_MAX_NUM_CHANNEL,       "Max number of channels out of range (-max_channels: 6 or 8)"},
	{MS_ERR_INVALID_MAX_NUM_CHANNEL_VALUE,    "Max number of channels is invalid (-max_channels: 6 or 8)"},
	{MS_ERR_OUTOFRANGE_MC_5_1_DMX,            "Downmix 7.1 to 5.1 on multichannel output flag is out of range (-mc_5_1_dmx: 0 or 1)"},
	{MS_ERR_INVALID_MC_5_1_DMX,               "Downmix 7.1 to 5.1 on multichannel output flag is invalid (-mc_5_1_dmx: 0 or 1)"},
        {MS_ERR_INVALID_PCM_INPUT_FILE_EXT,       "PCM Input files need to be WAVE files with an .wav suffix (-is, -ias)"},
	{MS_ERR_INVALID_PCM_PREC_FLAG,            "High PCM precision flag is invalid (-p: 0, 1)"},
	{MS_ERR_OUTOFRANGE_PCM_PREC_FLAG,         "High PCM precision flag is out of range (-p: 0, 1)"},
	{MS_ERR_OUTOFRANGE_DAP_DRC_TYPE,          "DAP DRC type out of range (-dap_drc: 0, 1)"},
	{MS_ERR_INVALID_DAP_DRC_TYPE,             "DAP DRC type is invalid (-dap_drc: 0, 1)"},
	{MS_ERR_INVALID_EVAL_MODE,                "Evaluation mode is invalid (-eval: 0, 1)"},
	{MS_ERR_INVALID_EXTPCM_2CH_MODE,          "2-channel downmix mode out-of-range for external PCM (-dmx: 0-1)"},
        {MS_ERR_INVALID_DAP_VB_ENABLE,            "DAP Virtual Bass enable flag invalid (-b_dap_vb_enable 0-1)"},
        {MS_ERR_DAP_VB_NOT_ENABLED,               "DAP Virtual Bass parameters set, but Virtual Bass enable flag not set (-b_dap_vb_enable 1)"},
        {MS_ERR_OUTOFRANGE_DAP_VB_ENABLE,         "DAP Virtual Bass enable flag out of range (-b_dap_vb_enable 0-1)"},
        {MS_ERR_DAP_GEQ_INVALID_NB_BANDS,         "DAP Graphical Equalizer number of bands exceeds the maximum of DAP_GEQ_MAX_BANDS"},
        {MS_ERR_DAP_IEQ_INVALID_NB_BANDS,         "DAP Intelligent Equalizer number of bands exceeds the maximum of DAP_IEQ_MAX_BANDS"},
        {MS_ERR_DAP_OPT_INVALID_NB_BANDS,         "DAP Optimizer number of bands exceeds the maximum of DAP_OPT_MAX_BANDS"},
        {MS_ERR_DAP_REG_INVALID_NB_BANDS,         "DAP Regulator number of bands exceeds the maximum of DAP_REG_MAX_BANDS"},
        {MS_ERR_LC_UNSUPPORTED_ASSOC_MIX,         "Associated stream mixing not supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_HEAAC,             "HE-AAC decoding not supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_MC_OUT,            "Main multichannel PCM output not supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_DAP_INIT_MODE,     "Only single instance DAP initialization mode supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_APP_SOUND_IN,      "Application Sound input not supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_TXT_OUT,           "Parameter text file output not supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_ENCODER,           "Encoding of PCM input not supported in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_SYSSND_CHCFG,      "Unsupported channel configuration for the system sounds input in low complexity mode"},
        {MS_ERR_LC_UNSUPPORTED_DMX,               "Downmixing of PCM input not supported in low complexity mode"},
        {MS_ERR_EOT,                              ""}
};


/*
 * Documentation resides in header file only
 */
int ms_handle_parser_retval(MS_RETVAL retval, int err_code)
{
  if(retval == MS_OK || retval == MS_HELPTEXT_REQUESTED){
    return retval;
  }
  else
  {
    const char *errmsg;

    if(ms_errorlookup(ms_errmsgtab, err_code, &errmsg) == MS_OK)
    {
      fprintf(stderr, "\n\nERROR: %s\n\n", errmsg);
    }
    else
    {
      fprintf(stderr, "\n\nUnknown error: %d\n\n", err_code);
    }
    fprintf(stderr, "Type -h for help text\n");
    fprintf(stderr, "Type -h [ddplus / he-aac / pcm] for codec-specific help text\n");
    fprintf(stderr, "Type -h lc for low complexity specific help text\n");

    return err_code;
  }
}

/*
 * Documentation resides in header file only
 */
int ms_errorlookup(
  const MS_ERR *p_msgtab,  /* input  */
	int               err,   /* input  */
	const char **p_errtext)  /* output */
{
	/* declare local variables */
	const MS_ERR *p_err;

	/* check input arguments */
	MS_ERR_CHKARG(p_errtext);
	MS_ERR_CHKARG(p_msgtab);

	/* search for error code in table */
	for (p_err = &p_msgtab[0]; p_err->err != MS_ERR_EOT; p_err++)
	{
		if (p_err->err == err)
		{
		  *p_errtext = p_err->p_msg;
			return MS_OK;
		}
	}
	return (MS_ERR_UNKNOWN_ERRCODE);
}

