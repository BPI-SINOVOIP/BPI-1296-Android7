/** 
 * @file: sample_config.h
 *
 * @brief 
 * Header file for sample library to provide persistent storage for
 * adapter properties and notebook profiles via .ini files.
 * Applications wishing to use this library should include this header
 * file.
 *
 * INTEL CONFIDENTIAL
 * Copyright 2010 Intel Corporation All Rights Reserved.
 *
 * The source code contained or described herein and all documents related to 
 * the source code ("Material") are owned by Intel Corporation or its 
 * suppliers or licensors.  Title to the Material remains with Intel 
 * Corporation or its suppliers and licensors.  The Material contains trade 
 * secrets and proprietary and confidential information of Intel or its 
 * suppliers and licensors.  The Material is protected by worldwide copyright 
 * and trade secret laws and treaty provisions. No part of the Material may 
 * be used, copied, reproduced, modified, published, uploaded, posted, 
 * transmitted, distributed, or disclosed in any way without Intel's prior 
 * express written permission.
 *
 * No license under any patent, copyright, trade secret or other intellectual 
 * property right is granted to or conferred upon you by disclosure or 
 * delivery of the Materials,  either expressly, by implication, inducement, 
 * estoppel or otherwise.  Any license under such intellectual property 
 * rights must be express and approved by Intel in writing.
 */

#ifndef _CONFIG_FILE_H
#define _CONFIG_FILE_H

#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define IN
#define OUT

#define	MAX_FRIENDLY_NAME_BUF		19
#define	MAX_MODEL_NAME_BUF  		14
#define	MAX_VERSION_STR_BUF 		20
#define MAX_INTERFACE_LEN           32
#define MAX_DEVICE_ID_BUF           13
#define MAX_PRODUCT_ID_BUF          17

#define MAX_SSID_LEN                32
#define MAX_KEY_LEN                 64

// From sample_rds.h
// All of these have an extra byte for the NULL
#define RDS_DEFAULT_BUF_LEN       32
#define RDS_DEVICE_URL_BUF_LEN    257

typedef struct {
    uint8_t  in_use;
    char     ssid[ MAX_SSID_LEN ];
    char     key[ MAX_KEY_LEN ];
    uint8_t mac[ETH_ALEN];
} profile_config_t;

typedef struct _adapter_config {
    char     friendly_name[ MAX_FRIENDLY_NAME_BUF ];
    char     default_friendly_name[ MAX_FRIENDLY_NAME_BUF ];
	char     model_name[ MAX_MODEL_NAME_BUF ];
    char     device_id[ MAX_DEVICE_ID_BUF ];
    char     logo_file[ FILENAME_MAX ];

    char     manufacturer_name[ RDS_DEFAULT_BUF_LEN ];
	char     software_version[ MAX_VERSION_STR_BUF ];
	char     hardware_version[ MAX_VERSION_STR_BUF ];
    char     device_url[ RDS_DEVICE_URL_BUF_LEN ];
    char     product_id[ MAX_PRODUCT_ID_BUF ];

	char     interface_name[ MAX_INTERFACE_LEN ];
	char     p2p_interface_name[ MAX_INTERFACE_LEN ];
    uint16_t rtsp_sink_port;

    uint16_t hdcp2_port;        // set to 0 to disable HDCP2

    uint8_t  overscan_compensation_x;
    uint8_t  overscan_compensation_y;
} adapter_config_t;

/*
 * Prototypes for functions that are available to consumers of this
 * library.
 */
int config_load_file( IN const char * filename, 
        OUT adapter_config_t * adapter_config, OUT profile_config_t *profiles, 
        IN int max_profiles );
int config_save_file( IN const char * filename, 
        IN adapter_config_t * adapter_config, IN profile_config_t *profiles, 
        IN int num_profiles );

#ifdef __cplusplus
}
#endif

#endif
