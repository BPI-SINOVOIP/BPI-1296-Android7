#ifndef MS12_ERR_H_
#define MS12_ERR_H_

/** @ingroup msd_frontend */

/** @file
 *  @brief Multistream Decoder Front-end - Error Handling - header file
 */

#include <assert.h>

#define MS_ERR_CHKARG(a) assert(a) /**< Macro used to assert arguments */

/**** Module Error Codes ****/
#define    MS_ERR_INVALID_INPUT_FILE_TYPE            1
#define    MS_ERR_INVALID_INPUT_FILE                 2
#define    MS_ERR_INCOMPATIBLE_INPUTS                3
#define    MS_ERR_UNKNOWN_ERRCODE                    4
#define    MS_ERR_INVALID_OUTPUT_FILE_TYPE           5
#define    MS_ERR_INVALID_OUTPUT_FILE                6
#define    MS_ERR_INVALID_DD_OUTPUT_FILE             7
#define    MS_ERR_INVALID_2CH_OUTPUT_FILE            8
#define    MS_ERR_INVALID_DRC_BOOST_FACTOR           9
#define    MS_ERR_INVALID_DRC_CUT_FACTOR            10
#define    MS_ERR_INVALID_DMX_MODE                  11
#define    MS_ERR_INVALID_DRC_MODE                  12
#define    MS_ERR_INVALID_DUALMONO_MODE             15
#define    MS_ERR_INVALID_VERBOSE_MODE              16
#define    MS_ERR_INVALID_ASSOCIATED_SUBSTREAM      20
#define    MS_ERR_INVALID_DDP_DEBUG_VALUE           21
#define    MS_ERR_INVALID_COMPRESSOR_PROFILE        22
#define    MS_ERR_INVALID_PROGRAM_REF_LEVEL         23
#define    MS_ERR_ASSOCIATED_2CH_RESTRICT           24
#define    MS_ERR_INVALID_HEAAC_DEBUG_VALUE         25
#define    MS_ERR_INVALID_DIALNORM_VALUE            26
#define    MS_ERR_INVALID_TRANSPORT_FORMAT          27
#define    MS_ERR_INVALID_MULTICHANNEL_MODE         28
#define    MS_ERR_INVALID_ASSOC_MIXING_VALUE        29
#define    MS_ERR_INVALID_USER_BALANCE_VALUE        30
#define    MS_ERR_UNSUPPORTED_PARAMETER             31
#define    MS_ERR_MISSING_INPUT_MAIN                32
#define    MS_ERR_MISSING_OUTPUT_MAIN               33
#define    MS_ERR_MISSING_COMMAND_FILE              34
#define    MS_ERR_MISSING_INPUT_ASSOCIATED          37
#define    MS_ERR_NOTAPP_MULTICHANNEL_MODE          39
#define    MS_ERR_NOTAPP_VIRTUALIZER_OUTPUT         40
#define    MS_ERR_OUTOFRANGE_DDP_DBG_FRAME          42
#define    MS_ERR_OUTOFRANGE_DDP_DBG_DEC            43
#define    MS_ERR_OUTOFRANGE_PCM_COMP_PROFILE       44
#define    MS_ERR_OUTOFRANGE_PCM_PROG_REF_LEVEL     45
#define    MS_ERR_OUTOFRANGE_HEAAC_DBG              46
#define    MS_ERR_OUTOFRANGE_HEAAC_DIALNORM         47
#define    MS_ERR_OUTOFRANGE_HEAAC_TRANSFMT         48
#define    MS_ERR_OUTOFRANGE_DUALMONO               49
#define    MS_ERR_OUTOFRANGE_DRC_BOOST              50
#define    MS_ERR_OUTOFRANGE_DRC_CUT                51
#define    MS_ERR_OUTOFRANGE_ST_DOWNMIX             52
#define    MS_ERR_OUTOFRANGE_DRC_MODE               53
#define    MS_ERR_OUTOFRANGE_ASSOC_MIX              54
#define    MS_ERR_OUTOFRANGE_USER_BALANCE           55
#define    MS_ERR_OUTOFRANGE_ASSOCIATED_SUBSTREAM   56
#define    MS_ERR_INVALID_DDP_2CH_MODE              57
#define    MS_ERR_OUTOFRANGE_WAVFILEBITDEPTH        58
#define    MS_ERR_INVALID_WAVFILEBITDEPTH           59
#define    MS_ERR_OUTOFRANGE_EXTPCM_ACMOD           60
#define    MS_ERR_INVALID_EXTPCM_ACMOD              61
#define    MS_ERR_OUTOFRANGE_SYS_SOUNDS_ACMOD       62
#define    MS_ERR_INVALID_SYS_SOUNDS_ACMOD          63
#define    MS_ERR_INVALID_LFE                       64
#define    MS_ERR_EXTPCM_ASSOCIATED_INPUT           65
#define    MS_ERR_OUTOFRANGE_DAP_INIT_MODE          66
#define    MS_ERR_INVALID_DAP_INIT_MODE             67
#define    MS_ERR_INVALID_DDP_OUTPUT_FILE           68
#define    MS_ERR_OUTOFRANGE_APP_SOUNDS_ACMOD       69
#define    MS_ERR_INVALID_APP_SOUNDS_ACMOD          70
#define    MS_ERR_OUTOFRANGE_DAP_DMX_TYPE           71
#define    MS_ERR_INVALID_DAP_DMX_TYPE              72
#define    MS_ERR_OUTOFRANGE_DAP_PARAM              73
#define    MS_ERR_INVALID_DAP_VALUE                 74
#define    MS_ERR_OUTOFRANGE_MAX_NUM_CHANNEL        75
#define    MS_ERR_INVALID_MAX_NUM_CHANNEL_VALUE     76
#define    MS_ERR_OUTOFRANGE_MC_5_1_DMX             77
#define    MS_ERR_INVALID_MC_5_1_DMX                78
#define    MS_ERR_INVALID_PCM_INPUT_FILE_EXT        79
#define    MS_ERR_INVALID_PCM_PREC_FLAG             80
#define    MS_ERR_OUTOFRANGE_PCM_PREC_FLAG          81
#define    MS_ERR_OUTOFRANGE_DAP_DRC_TYPE           82
#define    MS_ERR_INVALID_DAP_DRC_TYPE              83
#define    MS_ERR_INVALID_EVAL_MODE                 84
#define    MS_ERR_INVALID_EXTPCM_2CH_MODE           85
#define    MS_ERR_INVALID_BS_PROC_MODE              86
#define    MS_ERR_LC_UNSUPPORTED_ASSOC_MIX          87
#define    MS_ERR_LC_UNSUPPORTED_HEAAC              88
#define    MS_ERR_LC_UNSUPPORTED_MC_OUT             89
#define    MS_ERR_LC_UNSUPPORTED_DAP_INIT_MODE      90
#define    MS_ERR_LC_UNSUPPORTED_APP_SOUND_IN       91
#define    MS_ERR_LC_UNSUPPORTED_TXT_OUT            92
#define    MS_ERR_LC_UNSUPPORTED_ENCODER            93
#define    MS_ERR_LC_UNSUPPORTED_SYSSND_CHCFG       94
#define    MS_ERR_INVALID_SYSSOUND_MIXING_VALUE     95
#define    MS_ERR_OUTOFRANGE_SYSSOUND_MIX           96
#define    MS_ERR_INVALID_DAP_VB_ENABLE             97
#define    MS_ERR_OUTOFRANGE_DAP_VB_ENABLE          98
#define    MS_ERR_DAP_VB_NOT_ENABLED                99
#define    MS_ERR_DAP_GEQ_INVALID_NB_BANDS         100
#define    MS_ERR_DAP_IEQ_INVALID_NB_BANDS         101
#define    MS_ERR_DAP_OPT_INVALID_NB_BANDS         102
#define    MS_ERR_DAP_REG_INVALID_NB_BANDS         103
#define    MS_ERR_LC_UNSUPPORTED_DMX               104
#define    MS_ERR_INVALID_MIXER_BALANCE_VALUE      105
#define    MS_ERR_OUTOFRANGE_MIXER_BALANCE         106
#define    MS_ERR_EOT                               -1

typedef enum MS_RETVAL
{
    MS_OK=0,
    MS_GENERAL_ERROR,
    MS_OUT_OF_MEMORY,
    MS_READ_ERROR,
    MS_OPEN_FILE_ERROR,
    MS_FILE_IO_ERROR,
    MS_HELPTEXT_REQUESTED,
    MS_INVALID_PARAM,
    MS_PROCESS_ERROR,
    MS_SET_PARAM_ERROR,
} MS_RETVAL;


/**** Module Structures ****/
/** This structure holds an error code and a string with a corresponding error message */
typedef struct
{
    int err;           /**< Error code */
    const char* p_msg; /**< String describing the error associated with the error code above */
} MS_ERR;


/**** Module Functions ****/

/**
 * @brief This function evaluates the return value from the ms_parse_command_line() function
 *         and prints out the appropriate error message if necessary.
 */
int ms_handle_parser_retval(MS_RETVAL retval,  /**< Return value of ms_parse_command_line() */
                            int err_code       /**< Parameter specific error code */
                            );


/**
*    @brief This function looks for a human-readable text string to display
*           for a given application error code.
*
*    @return ERR_NO_ERROR (0) if string found, nonzero otherwise
*/
int ms_errorlookup(
                   const MS_ERR *p_msgtab,   /**< [in]  Pointer to table which translates error codes into error messages */
                   int           err,        /**< [in]  Error code returned during processing */
                   const char  **p_errtext   /**< [out] User-supplied additional text */
                   );

#endif /* MS12_ERR_H_ */
