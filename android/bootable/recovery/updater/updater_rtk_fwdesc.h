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

#ifndef _UPDATER_UPDATER_RTK_FWDESC_H_
#define _UPDATER_UPDATER_RTK_FWDESC_H_

#include <stdio.h>
#include "minzip/Zip.h"
#include "updater_rtk_common.h"

#define SIZE_ALIGN_BOUNDARY_LESS(len, size)     ((len) & ~((size) - 1))
#define SIZE_ALIGN_BOUNDARY_MORE(len, size)     (((len - 1) & ~((size) - 1)) + size)

#define UPGRAD_SIG "UPGRAD__"
#define VERONA_INSTALLER_SIGNATURE		"Verona Installation Packages"
#define FIRMWARE_DESCRIPTION_TABLE_SIGNATURE	"VERONA__"	//Max 8 bytes length.

// for old FW_Table version control
#define FIRMWARE_DESCRIPTION_TABLE_VERSION_01	0x01		//1 byte table version.
#define FIRMWARE_DESCRIPTION_TABLE_VERSION_11	0x11		//1 byte table version.
#define FIRMWARE_DESCRIPTION_TABLE_VERSION_21	0x21		//1 byte table version.
// for new FW_Table version control
#define FIRMWARE_DESCRIPTION_TABLE_VERSION_02	0x02		//1 byte table version.
#define FIRMWARE_DESCRIPTION_TABLE_VERSION_12	0x12		//1 byte table version.
#define FIRMWARE_DESCRIPTION_TABLE_VERSION_22	0x22		//1 byte table version.

typedef struct {
   u8   signature[8];
   u32  checksum;
   u8   version; //0x01
   u8   reserved[7];
   u32  paddings;
   u32  part_list_len;
   u32  fw_list_len;
} __attribute__((packed)) fw_desc_table_t;

typedef struct {
   u8 type;
#if BYTE_ORDER == LITTLE_ENDIAN
   u8 reserved:6,
      lzma:1,
      ro:1;
#elif BYTE_ORDER == BIG_ENDIAN
   u8 ro:1,
      lzma:1,
      reserved:6;
#else
#error "Please fix <asm/byteorder.h>"
#endif
   u32  version;
   u32  target_addr;
   u64  offset;
   u32  length;
   u32  paddings;
   u8   sha_hash[32];
   u8   reserved_1[6];
} __attribute__((packed)) fw_desc_entry_v2_t;

typedef struct {
    fw_desc_entry_v2_t v2;
    u32 act_size;
    u8  hash[32];
    u8  part_num;
    u8  reserved[27];
} __attribute__((packed)) fw_desc_entry_v12_t;

typedef struct {
    fw_desc_entry_v2_t v2;
    u32 act_size;
    u8  part_num;
    u8  RSA_sign[256];
    u8  reserved[27];
} __attribute__((packed)) fw_desc_entry_v22_t;

typedef enum {
   PART_TYPE_RESERVED = 0,
   PART_TYPE_FW,
   PART_TYPE_FS,
} part_type_code_t;

typedef struct {
   u8 type;
#if BYTE_ORDER == LITTLE_ENDIAN
   u8 reserved:7,
      ro:1;
#elif BYTE_ORDER == BIG_ENDIAN
   u8 ro:1,
      reserved:7;
#else
#error "Please fix <asm/byteorder.h>"
#endif
   u64  length;
   u8   fw_count;
   u8   fw_type;
   u8   reserved_1[4];
   u8   mount_point[32];
} __attribute__((packed)) part_desc_entry_v2_t;

typedef struct {
   u8 type;
#if BYTE_ORDER == LITTLE_ENDIAN
   u8 reserved:6,
      lzma:1,
      ro:1;
#elif BYTE_ORDER == BIG_ENDIAN
   u8 ro:1,
      lzma:1,
      reserved:6;
#else
#error "Please fix <asm/byteorder.h>"
#endif
   u32  version;
   u32  target_addr;
   u64  offset;
   u32  length;
   u32  paddings;
   u8   sha_hash[32];
   u8   reserved_1[6];

    u32 act_size;
    u8  part_num;
    u8  RSA_sign[256];
    u8  reserveded[27];
} __attribute__((packed)) rtk_fw_desc_entry_v22_t;

struct rtk_fw_header
{
   fw_desc_table_t fw_tab;
   part_desc_entry_v2_t part_desc[10];
   rtk_fw_desc_entry_v22_t fw_desc[15];
   unsigned int fw_intex[15];

   unsigned int kernel_index;
   unsigned int audio_index;
   unsigned int video_index;
   unsigned int rootfs_index;
   unsigned int resource_index;

   unsigned int secure_boot;
   unsigned int part_count;
   unsigned int fw_count;
   unsigned int valid;
};
typedef struct rtk_fw_header CRtkFWHeader;

typedef enum {
   FW_TYPE_RESERVED = 0,
   FW_TYPE_BOOTCODE,
   FW_TYPE_KERNEL,
   FW_TYPE_RESCUE_DT,
   FW_TYPE_KERNEL_DT,
   FW_TYPE_RESCUE_ROOTFS, //5
   FW_TYPE_KERNEL_ROOTFS,
   FW_TYPE_AUDIO,
   FW_TYPE_AUDIO_FILE,
   FW_TYPE_VIDEO_FILE,
   FW_TYPE_EXT4, //10
   FW_TYPE_UBIFS,
   FW_TYPE_SQUASH,
   FW_TYPE_EXT3,
   FW_TYPE_ODD,
   FW_TYPE_YAFFS2, //15
   FW_TYPE_ISO,
   FW_TYPE_SWAP,
   FW_TYPE_NTFS,
   FW_TYPE_JFFS2,
   FW_TYPE_IMAGE_FILE, //20
   FW_TYPE_IMAGE_FILE1,
   FW_TYPE_IMAGE_FILE2,
   FW_TYPE_AUDIO_FILE1,
   FW_TYPE_AUDIO_FILE2,
   FW_TYPE_VIDEO_FILE1, //25
   FW_TYPE_VIDEO_FILE2,
   FW_TYPE_VIDEO,
   FW_TYPE_VIDEO2,
   FW_TYPE_ECPU,
   FW_TYPE_TEE, //30
   FW_TYPE_GOLD_KERNEL,
   FW_TYPE_GOLD_RESCUE_DT,
   FW_TYPE_GOLD_RESCUE_ROOTFS,
   FW_TYPE_GOLD_AUDIO,
   FW_TYPE_GOLD_TEE, //35
   FW_TYPE_CONFIG,
   FW_TYPE_UBOOT,
   FW_TYPE_BL31,
   FW_TYPE_UNKNOWN
} fw_type_code_t;


#endif
