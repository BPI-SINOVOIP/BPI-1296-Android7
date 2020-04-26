/**
 ** Copyright (C) 2002 DivXNetworks, all rights reserved.
 **
 ** DivXNetworks, Inc. Proprietary & Confidential
 **
 ** This source code and the algorithms implemented therein constitute
 ** confidential information and may comprise trade secrets of DivXNetworks
 ** or its associates, and any use thereof is subject to the terms and
 ** conditions of the Non-Disclosure Agreement pursuant to which this
 ** source code was originally received.
 **
 **/

/** $Id: options.h,v 1.5 2002/12/16 19:30:17 bfudge Exp $
 **
 **/

/*************************************************************************/

#ifndef _OPTIONS_H
#define _OPTIONS_H

#define MSTRING_MAX 12
#define FILENAME_LEN 256
#define HEX_MAX 16



typedef enum _e_option_type {
	otype_none, otype_string, otype_hex, otype_int, otype_float, otype_double, otype_char, otype_bool, otype_mstring, otype_flag, otype_help, otype_default, otype_last
} e_option_type;

struct t_option_table {
	char longname[64];
	char shortname;
	char description[128];
	void *option_ptr;
	e_option_type option_type;
	char modified;
};

#ifdef __cplusplus
extern "C" {
#endif


int set_options ();
int parse_options (int argc, char **argv);

// Must define these functions if libOptions is used.
extern void initialize_options();
extern int check_options();
extern int get_options (int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif /*  */
