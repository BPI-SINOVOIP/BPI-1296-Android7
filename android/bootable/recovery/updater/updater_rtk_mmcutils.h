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

#ifndef _UPDATER_UPDATER_RTK_MMCUTILS_H_
#define _UPDATER_UPDATER_RTK_MMCUTILS_H_

#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

// Copied from rtk_mtd.h
#define MTD_ABSENT              0
#define MTD_RAM                 1
#define MTD_ROM                 2
#define MTD_NORFLASH            3
#define MTD_NANDFLASH           4
#define MTD_PEROM               5
#define MTD_DATAFLASH           6
#define MTD_EMMC 	            7
#define MTD_OTHER               14
#define MTD_UNKNOWN             15

#ifndef BLKRRPART
#define BLKRRPART  _IO(0x12,95) /* re-read partition table */
#endif
#ifndef BLKGETSIZE
#define BLKGETSIZE _IO(0x12,96) /* return device size in bytes*/
#endif
#ifndef BLKGETSIZE64
#define BLKGETSIZE64 _IOR(0x12,114,size_t) /* return device size in bytes (u64)*/
#endif

int rtk_open_block_dev(char** p);
void rtk_close_block_dev(int fd);
int rtk_mmc_write(unsigned int offset, char *data, int size);
int rtk_get_mbr(unsigned int offset);
int rtk_set_mbr(unsigned int blkoffset, char *mbr);
unsigned long rtk_get_size_emmc(void);

#endif
