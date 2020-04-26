#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ms12_cmd.h"
#include "ms12_err.h"
#include "ms12_usage.h"


#define MS_EXTPCM_IN_SAMPLES     1536

#define MS_MAX_STR_LEN           1024

#define MS_MAJOR_VERSION      "1"
#define MS_MINOR_VERSION      "3"
#define MS_UPDATE_VERSION     "1"

#define MS_DAP_MIN_VAL            (-32767)
#define MS_DAP_MAX_VAL            ( 32767)

/* Internal Function Declarations */
/** @brief This function retrieves the runtime dependant parameters from the parsed command line (or parsed command text file) */
static MS_RETVAL ms_get_runtime_params(MS_USE_CASE *p_use_case, MS_RUNTIME_PARAMS *p_runtime_params, DLB_GETPARAM_HANDLE h_getparam, int *err_code);
/** @brief This function retrieves the DAP parameters from the parsed command line (or parsed command text file) */
static MS_RETVAL ms_get_dap_params(dap_params_t *p_dap_params, DLB_GETPARAM_HANDLE h_getparam, int b_dap_vb_enable, int *err_code);
/** @brief Checks that the string has the specified suffix extension */
static MS_RETVAL ms_validate_extension(const char *param, const char *ext);
/** @brief Retrieves comma seperated integers from a string into an array */
static MS_RETVAL ms_get_int_array_from_str(char **p_csv_string, int num_el, int *p_vals);
/** @brief Retrieves integer from a string and returns address of next CSV integer */
static MS_RETVAL ms_get_int_from_str(char **p_csv_string, int *p_vals);

static MS_RETVAL set_int_check_gp_err(DLB_GETPARAM_RETVAL gp_err, int val, int *p_assign_val, int *err_code, int outofrange, int invalid);
static MS_RETVAL ms_validate_ac4trp(const char *param);


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

  int b_max_channels_set = 0;

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
#ifdef MS12_AC4_SUPPORT
    else if (strcmp(gp_string, "ac4") == 0)
    {
      ms_show_usage_common_switches();
      ms_show_usage_context_ac4();
    }
#endif
    else if (strcmp(gp_string, "dap") == 0)
    {
      ms_show_usage_context_dap();
    }
    else if (strcmp(gp_string, "lc") == 0)
    {
      ms_show_usage_lc();
    }
    else if (strcmp(gp_string, "command.txt") == 0)
    {
        ms_show_usage_command_txt();
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

  /* Check for input type indication */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "it", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
      if (strcmp(gp_string, "DDP") == 0)
      {
          p_use_case->input_type = DOLBY_DIGITAL_PLUS;
      }
      else if (strcmp(gp_string, "AAC") == 0)
      {
          p_use_case->input_type = HE_AAC;
      }
#ifdef MS12_AC4_SUPPORT
      else if (strcmp(gp_string, "AC4") == 0)
      {
          p_use_case->input_type = AC4;
      }
#endif
      else if (strcmp(gp_string, "PCM") == 0)
      {
          p_use_case->input_type = EXTERNAL_PCM;
      }
      else
      {
          *err_code = MS_ERR_INVALID_INPUT_TYPE;
          return MS_INVALID_PARAM;
      }
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_INVALID_INPUT_TYPE;
    return MS_INVALID_PARAM;
  }
  /* Read the name of the main file */
  gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "im", &p_params->ms_init_args.input_filename[0], MS_MAX_STR_LEN);

  if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM)
  {
    *err_code = MS_ERR_MISSING_INPUT_MAIN;
     return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_OK)
  {
    *err_code = MS_ERR_INVALID_INPUT_FILE;
    return MS_OPEN_FILE_ERROR;
  }
  else if (p_use_case->input_type == UNKNOWN)
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
    }
#ifdef MS12_AC4_SUPPORT
    else if ((ms_validate_extension(p_params->ms_init_args.input_filename[0], ".ac4") == 0)
             || (ms_validate_ac4trp(p_params->ms_init_args.input_filename[0]) == 0) )
    {
      p_use_case->input_type = AC4;
    }
#endif
    else
    {
      *err_code = MS_ERR_INVALID_INPUT_FILE_TYPE;
      return MS_INVALID_PARAM;
    }
  }

  if ((ms_validate_extension(p_params->ms_init_args.input_filename[0], ".mp4") == 0)
      ||(ms_validate_extension(p_params->ms_init_args.input_filename[0], ".trp") == 0) )
  {
      p_use_case->b_file_playback = 1;
  }

  if ( (p_use_case->input_type == HE_AAC) && p_use_case->b_low_complexity )
  {
      *err_code = MS_ERR_LC_UNSUPPORTED_HEAAC;
      return MS_OPEN_FILE_ERROR;
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
#ifdef MS12_AC4_SUPPORT
    else if ((p_use_case->input_type == AC4) && (!p_use_case->b_file_playback))
    {
      if (ms_validate_extension(p_params->ms_init_args.input_filename[1], ".ac4") != 0)
      {
        *err_code = MS_ERR_INCOMPATIBLE_INPUTS;
        return MS_OPEN_FILE_ERROR;
      }
    }
    else if ((p_use_case->input_type == AC4) && (p_use_case->b_file_playback))
    {
      if ((ms_validate_extension(p_params->ms_init_args.input_filename[1], ".mp4") != 0)
          && (ms_validate_extension(p_params->ms_init_args.input_filename[1], ".trp") != 0) )
      {
        *err_code = MS_ERR_INCOMPATIBLE_INPUTS;
        return MS_OPEN_FILE_ERROR;
      }
    }
#endif
  }

  /* 2nd main input instead of associated input */
  if (  (p_use_case->input_type == DOLBY_DIGITAL_PLUS)  /* 2nd main input is only available for DDP */
     && (p_use_case->b_dual_input == 0)                 /* 2nd main input and associate input cannot be specified together */
     && (p_use_case->b_low_complexity == 0)             /* 2nd main input is not available in low complexity mode */
     )
  {
      gp_err = dlb_getparam_maxlenstring(p_params->h_getparam, "im2", &p_params->ms_init_args.input_filename[1], MS_MAX_STR_LEN);
      if (gp_err == DLB_GETPARAM_UNDEFINED_PARAM)
      {
          /* no 2nd main input */
      }
      else if (gp_err != DLB_GETPARAM_OK)
      {
          *err_code = MS_ERR_INVALID_INPUT_FILE;
          return MS_OPEN_FILE_ERROR;
      }
      else if (gp_err == DLB_GETPARAM_OK)
      {
          p_use_case->b_dual_input = 1;
          p_use_case->b_dual_main = 1;

          /* Ensure that the 2nd main input has a DDP file extensions */
          if ((ms_validate_extension(p_params->ms_init_args.input_filename[1], ".ec3") != 0) &&
             (ms_validate_extension(p_params->ms_init_args.input_filename[1], ".ac3") != 0))
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
#ifdef MS12_AC4_SUPPORT
  /* Associated type selection - AC4 use-case only */
  if(p_use_case->input_type == AC4)
  {
      p_params->ms_runtime_args.ac4_1st_pref_lang[0] = '\0';
      p_params->ms_runtime_args.ac4_2nd_pref_lang[0] = '\0';
      p_params->ms_runtime_args.ac4_pres_index[MS_MAIN_DECODER_IDX] = -1;
      p_params->ms_runtime_args.ac4_pres_index[MS_ASSOC_DECODER_IDX] = -1;
      p_params->ms_runtime_args.ac4_associated_type = 1;
      p_params->ms_runtime_args.b_ac4_pref_assoc_type_over_lang = 1;
      p_params->ms_runtime_args.ac4_prog_id_type  = 0;
      memset(p_params->ms_runtime_args.ac4_program_identifier_string, 0, sizeof(p_params->ms_runtime_args.ac4_program_identifier_string));

      gp_err = dlb_getparam_int(p_params->h_getparam, "at", &gp_value, MS_MIN_AC4_ASSOC_TYPE, MS_MAX_AC4_ASSOC_TYPE);
      if (gp_err == DLB_GETPARAM_OK)
      {
          p_params->ms_runtime_args.ac4_associated_type = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
          *err_code = MS_ERR_OUTOFRANGE_ASSOCIATED_TYPE;
          return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
          *err_code = MS_ERR_INVALID_ASSOCIATED_TYPE;
          return MS_INVALID_PARAM;
      }

      /* set default decoding mode depending on the number of input bitstreams. */
      if(p_use_case->b_dual_input)
      {
          p_use_case->ac4_mode = AC4_MODE_DUAL_STREAM_DUAL_DECODE;
      }
      else
      {
          p_use_case->ac4_mode = AC4_MODE_SINGLE_STREAM_DUAL_DECODE_DUAL_INSTANCE;
      }

      gp_err = dlb_getparam_int(p_params->h_getparam, "ac4_mode", &gp_value, MS_MIN_AC4_MODE, MS_MAX_AC4_MODE);
      if (gp_err == DLB_GETPARAM_OK)
      {
          p_use_case->ac4_mode = (int)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
          *err_code = MS_ERR_OUTOFRANGE_AC4_MODE;
          return MS_INVALID_PARAM;
      }
      else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
      {
          *err_code = MS_ERR_INVALID_AC4_MODE;
          return MS_INVALID_PARAM;
      }

      if (  ( p_use_case->b_dual_input && p_use_case->ac4_mode != AC4_MODE_DUAL_STREAM_DUAL_DECODE)
         || (!p_use_case->b_dual_input && p_use_case->ac4_mode == AC4_MODE_DUAL_STREAM_DUAL_DECODE))
      {
          *err_code = MS_ERR_INVALID_AC4_MODE;
          return MS_INVALID_PARAM;
      }

      if(p_use_case->ac4_mode == AC4_MODE_SINGLE_STREAM_SINGLE_DECODE)
      {
          p_use_case->b_mainonly = 1;
      }

      /* Implement single PID dual decoding by reading from the main input bitstream twice */
      if(p_use_case->ac4_mode == AC4_MODE_SINGLE_STREAM_DUAL_DECODE_DUAL_INSTANCE)
      {
          p_use_case->b_dual_input = 1;
          p_params->ms_init_args.input_filename[1] = p_params->ms_init_args.input_filename[0];
      }

      /* associated is restricted to stereo */
      p_use_case->b_restricted_ad = 1;
  }
#endif

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
      b_max_channels_set = 1;
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

  /* Encoder channel mode locking */
  gp_err = dlb_getparam_int(p_params->h_getparam, "chmod_locking", &gp_value, MS_MIN_ENC_CHMOD_LOCK, MS_MAX_ENC_CHMOD_LOCK);
  if (gp_err == DLB_GETPARAM_OK)
  {
      p_params->ms_init_args.enc_chmod_locking_mode = (int)gp_value;
  }
  else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
  {
      *err_code = MS_ERR_OUTOFRANGE_ENC_CHMOD_LOCK;
      return MS_INVALID_PARAM;
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_ENC_CHMOD_LOCK_VALUE;
      return MS_INVALID_PARAM;
  }


  if (p_use_case->b_low_complexity && (p_params->ms_init_args.max_num_channels > MS_LOW_COMPLEXITY_MAX_CHANNEL))
  {
      if(b_max_channels_set)
      {
          fprintf(stderr,"Warning: Low Complexity processing mode is limited to six channels. Parameter max_channels is set to six.\n");
      }
      p_params->ms_init_args.max_num_channels = MS_LOW_COMPLEXITY_MAX_CHANNEL;
  }

#ifdef MS12_AC4_SUPPORT
  if ( (p_use_case->input_type == AC4
       )
     && (p_params->ms_init_args.max_num_channels > MS_AC4_MAX_CHANNEL)
     )
  {
      if(b_max_channels_set)
      {
          fprintf(stderr,"Warning: AC4 decoding is limited to six output channels. Parameter max_channels is set to six.\n");
      }
      p_params->ms_init_args.max_num_channels = MS_AC4_MAX_CHANNEL;
  }
#endif

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
  const char *gp_string;

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
    if ( ( (p_use_case->input_type == DOLBY_DIGITAL_PLUS)
        )
      && ( (int16_t)gp_value == MS_DMX_TYPE_ARIB) )
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

#ifdef MS12_AC4_SUPPORT
  /* Associated type selection - AC4 use-case only */
  if(p_use_case->input_type == AC4)
  {

      gp_err = dlb_getparam_int(h_getparam, "at", &gp_value, MS_MIN_AC4_ASSOC_TYPE, MS_MAX_AC4_ASSOC_TYPE);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_runtime_params->ac4_associated_type = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
        *err_code = MS_ERR_OUTOFRANGE_ASSOCIATED_TYPE;
        return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
        *err_code = MS_ERR_INVALID_ASSOCIATED_TYPE;
        return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_int(h_getparam, "pat", &gp_value, MS_MIN_BOOL_VAL, MS_MAX_BOOL_VAL);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_runtime_params->b_ac4_pref_assoc_type_over_lang = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
        *err_code = MS_ERR_OUTOFRANGE_PREFER_ASSOCIATED_TYPE;
        return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
        *err_code = MS_ERR_INVALID_PREFER_ASSOCIATED_TYPE;
        return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_maxlenstring(h_getparam, "lang", &gp_string, MS_MAX_STR_LEN);
      if (gp_err == DLB_GETPARAM_OK)
      {
          int n;
          for(n=0; n<MS_AC4_LANG_MAX_STRLEN; n++)
          {
              p_runtime_params->ac4_1st_pref_lang[n] = gp_string[n];
              if(gp_string[n] == '\0')
              {
                  break;
              }
          }
          p_runtime_params->ac4_1st_pref_lang[MS_AC4_LANG_MAX_STRLEN-1] = '\0';
      }
      else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
      {
          *err_code = MS_ERR_MISSING_LANG_SELECTION;
          return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_maxlenstring(h_getparam, "lang2", &gp_string, MS_MAX_STR_LEN);
      if (gp_err == DLB_GETPARAM_OK)
      {
          int n;
          for(n=0; n<MS_AC4_LANG_MAX_STRLEN; n++)
          {
              p_runtime_params->ac4_2nd_pref_lang[n] = gp_string[n];
              if(gp_string[n] == '\0')
              {
                  break;
              }
          }
          p_runtime_params->ac4_2nd_pref_lang[MS_AC4_LANG_MAX_STRLEN-1] = '\0';
      }
      else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
      {
          *err_code = MS_ERR_MISSING_LANG_SELECTION;
          return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_int(h_getparam, "ac4_de", &gp_value, MS_MIN_AC4_DE_GAIN, MS_MAX_AC4_DE_GAIN);
      if (gp_err == DLB_GETPARAM_OK)
      {
          p_runtime_params->ac4_de_gain = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
          *err_code = MS_ERR_OUTOFRANGE_AC4_DE_GAIN;
          return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
          *err_code = MS_ERR_INVALID_AC4_DE_GAIN;
          return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_int(h_getparam, "ac4_main_pres_idx", &gp_value, MS_MIN_AC4_PRES_IDX, MS_MAX_AC4_PRES_IDX);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_runtime_params->ac4_pres_index[MS_MAIN_DECODER_IDX] = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
        *err_code = MS_ERR_OUTOFRANGE_AC4_PRES_IDX;
        return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
        *err_code = MS_ERR_INVALID_AC4_PRES_IDX;
        return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_int(h_getparam, "ac4_assoc_pres_idx", &gp_value, MS_MIN_AC4_PRES_IDX, MS_MAX_AC4_PRES_IDX);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_runtime_params->ac4_pres_index[MS_ASSOC_DECODER_IDX] = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
        *err_code = MS_ERR_OUTOFRANGE_AC4_PRES_IDX;
        return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
        *err_code = MS_ERR_INVALID_AC4_PRES_IDX;
        return MS_INVALID_PARAM;
      }

      gp_err = dlb_getparam_int(h_getparam, "ac4_prog_id_type", &gp_value, MS_MIN_AC4_PROG_ID_TYPE, MS_MAX_AC4_PROG_ID_TYPE);
      if (gp_err == DLB_GETPARAM_OK)
      {
        p_runtime_params->ac4_prog_id_type = (int16_t)gp_value;
      }
      else if (gp_err == DLB_GETPARAM_OUT_OF_RANGE)
      {
        *err_code = MS_ERR_OUTOFRANGE_AC4_PROG_ID_TYPE;
        return MS_INVALID_PARAM;
      }
      else if ((gp_err != DLB_GETPARAM_UNDEFINED_PARAM) && (gp_err != DLB_GETPARAM_ALREADY_GOT))
      {
        *err_code = MS_ERR_INVALID_AC4_PROG_ID_TYPE;
        return MS_INVALID_PARAM;
      }


      gp_err = dlb_getparam_maxlenstring(h_getparam, "ac4_prog_id", &gp_string, MS_MAX_STR_LEN);
      if (gp_err == DLB_GETPARAM_OK)
      {
          if(p_runtime_params->ac4_prog_id_type == 1)
          {
              long val = strtol(gp_string, NULL, 0);
              if((val < MS_MIN_AC4_SHORT_PROG_ID) || (val > MS_MAX_AC4_SHORT_PROG_ID))
              {
                  *err_code = MS_ERR_OUTOFRANGE_AC4_SHORT_PROG_ID;
                  return MS_INVALID_PARAM;
              }
          }
          else if(p_runtime_params->ac4_prog_id_type == 2)
          {
              if(strlen(gp_string) != MS_PROGRAM_UUID_LEN_BYTES*2)
              {
                  *err_code = MS_ERR_INVALID_UUID_PROG_ID;
                  return MS_INVALID_PARAM;
              }
          }

          strncpy(p_runtime_params->ac4_program_identifier_string, gp_string, MS_PROGRAM_UUID_LEN_BYTES*2);
      }
      else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
      {
          if(p_runtime_params->ac4_prog_id_type == 1)
          {
              *err_code = MS_ERR_INVALID_AC4_SHORT_PROG_ID;
          }
          else if(p_runtime_params->ac4_prog_id_type == 2)
          {
              *err_code = MS_ERR_INVALID_UUID_PROG_ID;
          }
          return MS_INVALID_PARAM;
      }
  }
#endif

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

  /* Associated program mixing and main/associated program user-balance - only for relevant use cases */
  p_runtime_params->associated_audio_mixing = p_use_case->b_mainonly ? 0 : 1;
  if ( ( (!p_use_case->b_mainonly && (p_use_case->input_type == DOLBY_DIGITAL_PLUS)) || p_use_case->b_dual_input )
     && !p_use_case->b_dual_main
     )
  {
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

  /* Mixer gains and fading ramps */
  gp_err = dlb_getparam_maxlenstring(h_getparam, "main1_mixgain", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
      char *tmpstr = (char *)gp_string;
      ms_get_int_from_str(&tmpstr, &p_runtime_params->input_mix_input1.target_attenuation);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->input_mix_input1.duration_in_ms);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->input_mix_input1.type);
      if (  (p_runtime_params->input_mix_input1.target_attenuation < -96) || (p_runtime_params->input_mix_input1.target_attenuation > 0)
         || (p_runtime_params->input_mix_input1.duration_in_ms < 0)       || (p_runtime_params->input_mix_input1.duration_in_ms > 60000)
         || (p_runtime_params->input_mix_input1.type < 0)                 || (p_runtime_params->input_mix_input1.type > 2)
         )
      {
          *err_code = MS_ERR_OUTOFRANGE_MIXER_FADE;
          return MS_INVALID_PARAM;
      }
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_MIXER_FADE_VALUES;
      return MS_INVALID_PARAM;
  }

  gp_err = dlb_getparam_maxlenstring(h_getparam, "main2_mixgain", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
      char *tmpstr = (char *)gp_string;
      ms_get_int_from_str(&tmpstr, &p_runtime_params->input_mix_input2.target_attenuation);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->input_mix_input2.duration_in_ms);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->input_mix_input2.type);
      if (  (p_runtime_params->input_mix_input2.target_attenuation < -96) || (p_runtime_params->input_mix_input2.target_attenuation > 0)
         || (p_runtime_params->input_mix_input2.duration_in_ms < 0)       || (p_runtime_params->input_mix_input2.duration_in_ms > 60000)
         || (p_runtime_params->input_mix_input2.type < 0)                 || (p_runtime_params->input_mix_input2.type > 2)
         )
      {
          *err_code = MS_ERR_OUTOFRANGE_MIXER_FADE;
          return MS_INVALID_PARAM;
      }
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_MIXER_FADE_VALUES;
      return MS_INVALID_PARAM;
  }

  gp_err = dlb_getparam_maxlenstring(h_getparam, "sys_prim_mixgain", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
      char *tmpstr = (char *)gp_string;
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input1.target_attenuation);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input1.duration_in_ms);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input1.type);
      if (  (p_runtime_params->syssound_mix_input1.target_attenuation < -96) || (p_runtime_params->syssound_mix_input1.target_attenuation > 0)
         || (p_runtime_params->syssound_mix_input1.duration_in_ms < 0)       || (p_runtime_params->syssound_mix_input1.duration_in_ms > 60000)
         || (p_runtime_params->syssound_mix_input1.type < 0)                 || (p_runtime_params->syssound_mix_input1.type > 2)
         )
      {
          *err_code = MS_ERR_OUTOFRANGE_MIXER_FADE;
          return MS_INVALID_PARAM;
      }
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_MIXER_FADE_VALUES;
      return MS_INVALID_PARAM;
  }

  gp_err = dlb_getparam_maxlenstring(h_getparam, "sys_apps_mixgain", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
      char *tmpstr = (char *)gp_string;
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input2.target_attenuation);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input2.duration_in_ms);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input2.type);
      if (  (p_runtime_params->syssound_mix_input2.target_attenuation < -96) || (p_runtime_params->syssound_mix_input2.target_attenuation > 0)
         || (p_runtime_params->syssound_mix_input2.duration_in_ms < 0)       || (p_runtime_params->syssound_mix_input2.duration_in_ms > 60000)
         || (p_runtime_params->syssound_mix_input2.type < 0)                 || (p_runtime_params->syssound_mix_input2.type > 2)
         )
      {
          *err_code = MS_ERR_OUTOFRANGE_MIXER_FADE;
          return MS_INVALID_PARAM;
      }
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_MIXER_FADE_VALUES;
      return MS_INVALID_PARAM;
  }

  gp_err = dlb_getparam_maxlenstring(h_getparam, "sys_syss_mixgain", &gp_string, MS_MAX_STR_LEN);
  if (gp_err == DLB_GETPARAM_OK)
  {
      char *tmpstr = (char *)gp_string;
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input3.target_attenuation);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input3.duration_in_ms);
      ms_get_int_from_str(&tmpstr, &p_runtime_params->syssound_mix_input3.type);
      if (  (p_runtime_params->syssound_mix_input3.target_attenuation < -96) || (p_runtime_params->syssound_mix_input3.target_attenuation > 0)
         || (p_runtime_params->syssound_mix_input3.duration_in_ms < 0)       || (p_runtime_params->syssound_mix_input3.duration_in_ms > 60000)
         || (p_runtime_params->syssound_mix_input3.type < 0)                 || (p_runtime_params->syssound_mix_input3.type > 2)
         )
      {
          *err_code = MS_ERR_OUTOFRANGE_MIXER_FADE;
          return MS_INVALID_PARAM;
      }
  }
  else if (gp_err != DLB_GETPARAM_UNDEFINED_PARAM)
  {
      *err_code = MS_ERR_INVALID_MIXER_FADE_VALUES;
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


static MS_RETVAL ms_init_dap_params(dap_params_t *p_dap_params)
{
    memset(p_dap_params, MS_DAP_PARAM_NOT_SET, sizeof(dap_params_t));
    return MS_OK;
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
  p_params->ms_init_args.enc_chmod_locking_mode = 0;

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
  p_params->ms_runtime_args.sys_sounds_channel_config.acmod = 2; /* DD_2_0 */
  p_params->ms_runtime_args.sys_sounds_channel_config.b_lfe_present = 0;
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

static MS_RETVAL ms_validate_ac4trp(const char *param)
{
    MS_RETVAL ret = MS_INVALID_PARAM;

    char dst_extension[] = ".ac4.trp";
    const int dst_length = strlen(dst_extension);
    const int src_length = strlen(param);

    if (src_length > dst_length)
    {
        const char *p_extension_begin = param + src_length - dst_length;

        if (strncmp(p_extension_begin, dst_extension, dst_length) == 0)
        {
            ret = MS_OK;
        }
    }

    return ret;
}

