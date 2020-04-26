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

#ifndef _UPDATER_INSTALL_RTK_H_
#define _UPDATER_INSTALL_RTK_H_

#include "updater_rtk_bootable.h"
#include "updater_rtk_fwdesc.h"
#include "updater_rtk_common.h"
#include "updater_rtk_imgdesc.h"
#include "updater_rtk_mmcutils.h"
#include "include_rtk.h"

u32 get_checksum(u8 *p, u32 len);
bool getExtractFileinZip(ZipArchive* za, char* targetFilePath, char* extractPath);
int fill_rtk_fw(CRTKImgDesc* prtkimg, ZipArchive* za);
//int rtk_load_fw(char* inParseFilePath, CRTKImgDesc* prtkimg);

int Rtk_fill_imgdesc_meminfo(struct t_rtkimgdesc* prtkimg, unsigned int flash_type);
int Rtk_load_config(CRTKImgDesc* prtkimg, ZipArchive* za);
int Rtk_fill_imgdesc_layout(CRTKImgDesc* prtkimg, ZipArchive* za);
int Rtk_burn_fw_emmc(CRTKImgDesc* prtkimgdesc, ZipArchive* za, char* zipPath, State* state);
int Rtk_burn_fw_nand(CRTKImgDesc* prtkimgdesc, ZipArchive* za, char* zipPath, State* state);
int Rtk_burn_fwdesc_emmc(CRTKImgDesc* prtkimg,State* state);
int Rtk_burn_fwdesc_nand(CRTKImgDesc* prtkimg, State* state);
int Rtk_burn_factory(CRTKImgDesc* prtkimgdesc, ZipArchive* za, State* state);
int Rtk_load_fwdesc_emmc(CRTKImgDesc* prtkimg, State* state);
int rtk_file_checksum(char* sBuf, unsigned int imglen, unsigned char *checksum);
#endif

