/******************************************************************************
 * This program is protected under international and U.S. copyright laws as
 * an unpublished work. This program is confidential and proprietary to the
 * copyright owners. Reproduction or disclosure, in whole or in part, or the
 * production of derivative works therefrom without the express permission of
 * the copyright owners is prohibited.
 *
 *                Copyright (C) 2015-2016 by Dolby International AB.
 *                            All rights reserved.
 ******************************************************************************/
/** @ingroup msd_frontend */

/** @file
 *  @brief Multistream Decoder Front-end - Command-line help text and parameter setting printouts- header file
 */

#ifndef MS_USAGE_H_
#define MS_USAGE_H_

#include "ms12_cmd.h"

/** Prints the parameter settings given by the command line interface */
void ms_print_parameter_settings(MS_USE_CASE *p_use_case, MS_PARAMS *p_args);

/** @brief Prints the complete help text to the command line */
MS_RETVAL ms_show_usage_full(void);
/** @brief Prints the complete help text for low complexity mode to the command line */
MS_RETVAL ms_show_usage_lc(void);
/** @brief Prints a brief version of the help text to the command line */
MS_RETVAL ms_show_usage_brief(void);
/** @brief Prints the usage of the command line switches that are common to all use cases */
MS_RETVAL ms_show_usage_common_switches(void);
/** @brief Prints the usage of the command line switches that are common to all use cases supported in low complexity processing mode */
MS_RETVAL ms_show_usage_common_switches_lc(void);
/** @brief Prints out a banner message including version numbers and copyright */
MS_RETVAL ms_show_banner(void);
/** @brief Prints the help text for the use cases with Dolby Digital Plus as input type */
MS_RETVAL ms_show_usage_context_ddplus(void);
#ifdef MS12_AC4_SUPPORT
/** @brief Prints the help text for the use cases with AC4 as input type */
MS_RETVAL ms_show_usage_context_ac4(void);
#endif
/** @brief Prints the help text for the external PCM use case */
MS_RETVAL ms_show_usage_context_pcm(void);
/** @brief Prints the help text for the external PCM use case in low complexity processing mode */
MS_RETVAL ms_show_usage_context_pcm_lc(void);
/** @brief Prints the help text for the use cases with HE-AAC as input type */
MS_RETVAL ms_show_usage_context_heaac(void);
/** @brief Prints the help text for DAP specific switches */
MS_RETVAL ms_show_usage_context_dap(void);
/** @brief Prints the help text for using the command.txt file */
MS_RETVAL ms_show_usage_command_txt(void);

#endif /* MS_USAGE_H_ */
