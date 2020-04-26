/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _UPDATER_UPDATER_RTK_COMMON_H_
#define _UPDATER_UPDATER_RTK_COMMON_H_

#include <stdio.h>
#include "updater_rtk_imgdesc.h"

#define uint8_t u8
#define u8 u_int8_t
#define U8 u_int8_t
#define UINT8 u_int8_t

#define uint16_t u16
#define u16 u_int16_t
#define U16 u_int16_t

#define uint32_t u32
#define u32 u_int32_t

#define uint64_t u64
#define u64 u_int64_t
#define UINT64 u_int64_t


//#define INSTALL_DEBUG
#define INSTALL_INFO
//#define INSTALL_LOG
#define INSTALL_ERROR

//get chip id info
#define RTD129x_CHIP_REVISION_A00       (0x00000000)
#define RTD129x_CHIP_REVISION_A01       (0x00010000)
#define RTD129x_CHIP_REVISION_B00       (0x00020000)
#define REG32( addr )		            (*(volatile unsigned int *)(addr))

#ifdef INSTALL_DEBUG
#define install_debug(f, a...) printf("[DEBUG][%s:%s():%d]" f, __FILE__, __FUNCTION__, __LINE__, ##a)
#else
#define install_debug(f, a...) /**/
#endif

#ifdef INSTALL_INFO
#define install_info(f, a...) printf("[INFO]" f, ##a)
#else
#define install_info(f, a...) /**/
#endif
	
#ifdef INSTALL_LOG
#define install_log(f, a...) printf(f, ##a)
#else
#define install_log(f, a...) /**/
#endif

#ifdef INSTALL_ERROR
#define install_err(f, a...) printf("[ERROR]" f, ##a) /**/
#else
#define install_err(f, a...) /**/
#endif

/*
*   Following code is from rtk_parameter.h
*/
int set_system_param(struct t_rtkimgdesc* prtkimg);
int get_parameter_value(const char* keystring, unsigned int *keyval);
int get_parameter_string(const char* keystring, char *string, int str_maxlen);
int rtk_file_to_string(const char* path, char* str);
int rtk_find_file_in_dir(const char *pdirpath, const char *pkeyword, char *filename, const int filename_maxlen);
int rtk_get_size_of_file(const char* file_path, unsigned int *file_size);
int rtk_command(const char* command, int line, const char* file, int show);
unsigned int get_chip_rev_id();

/*
*   Above code is from rtk_parameter.h
*/

/*
*   Following code is from rtk_common.h
*/
enum _error_code {
    _eRTK_SUCCESS = 0,
    _eRTK_GENERAL_FAIL = 1,
    _ePARSE_TAR_FAIL,
    _eFILL_RTK_PART_LIST_BY_CONFIG_FAIL,
    _eFILL_RTKIMGDESC_MEMINFO_FAIL,
    _eFILL_RTKIMGDESC_MEMINFO_FAIL_BOOTCODE,
    _eFILL_RTKIMGDESC_MEMINFO_FAIL_RESCUE,
    _eFILL_RTKIMGDESC_MEMINFO_FAIL_FACTORY,
    _eFILL_RTK_IMGDESC_LAYOUT_FAIL,
    _eFILL_RTK_IMGDESC_LAYOUT_FAIL_BOOTCODE_EXTRACT,
    _eFILL_RTK_IMGDESC_LAYOUT_FAIL_SIZE_NOT_ENOUGH,
    _eFILL_RTK_IMGDESC_LAYOUT_FAIL_ROOTFS_ETC_ZERO,
    _eRTK_BURN_BOOTCODE_MAC_FAIL,
    _eRTK_BURN_FACTORY_MAC_FAIL,
    _eRTK_TAG_UPDATE_ETC_FAIL,
    _eRTK_TAG_UPDATE_ETC_FAIL_CONFIG_SIZE,
    _eRTK_TAG_UPDATE_ETC_FAIL_BOOTTABLE_OFFSET,
    _eRTK_TAG_UPDATE_NL_FAIL,
    _eRTK_TAG_UPDATE_NL_ROOTFS_FAIL,
    _eRTK_TAG_UPDATE_AK_FAIL,
    _eRTK_TAG_UPDATE_VK_FAIL,
    _eRTK_TAG_UPDATE_VK2_FAIL,
    _eRTK_TAG_UPDATE_DALOGO_FAIL,
    _eRTK_TAG_UPDATE_DVLOGO_FAIL,
    _eUNKOWN_ERROR_CODE
};

//Rtk_def.h
#define CUSTOMER_AUDIO_LOGO_DEFAULT_TARGET_ADDRESS 0x83000000
#define CUSTOMER_VIDEO_LOGO_DEFAULT_TARGET_ADDRESS 0x83100000

#endif
