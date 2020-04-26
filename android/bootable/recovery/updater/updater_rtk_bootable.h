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

#ifndef _UPDATER_UPDATER_RTK_BOOTABLE_H_
#define _UPDATER_UPDATER_RTK_BOOTABLE_H_

#include <stdio.h>
#include "minzip/Zip.h"

#define EMMC_SUPPORT

enum E_FWTYPE
{
   FWTYPE_KERNEL=0,  // Normal Kernel
   FWTYPE_AKERNEL,      // Audio Kernel
   FWTYPE_VKERNEL,      // Video Kernel
   FWTYPE_VKERNEL2,      // Video2 Kernel
   FWTYPE_ECPU,			 // 8051 Kernel   
   FWTYPE_DALOGO,    // Default LOGO
   FWTYPE_DILOGO,    // Default LOGO(Image)
   FWTYPE_DVLOGO,    // Default LOGO(Video)
   FWTYPE_CALOGO1,    // Customer LOGO1
   FWTYPE_CVLOGO1,    // Customer LOGO1
   FWTYPE_CALOGO2,    // Customer LOGO2
   FWTYPE_CVLOGO2,    // Customer LOGO2 
   FWTYPE_NRESCUE,      // Native Rescue
   FWTYPE_RESCUE,    // Rescue
   FWTYPE_SROOTFS,      // Small rootfs
   FWTYPE_END
};

enum E_BOOTTYPE
{
   BOOTTYPE_UNKNOWN_BOOTTYPE=0,
   BOOTTYPE_NL_BIG_ROOTFS,
   BOOTTYPE_NL_SMALL_ROOTFS,
   BOOTTYPE_TMP_RESCUE,
   BOOTTYPE_NATIVE_RESCUE,
   BOOTTYPE_COMPLETE,
   BOOTTYPE_END
};

struct S_IMGLOCATION
{
   unsigned int offset;
   unsigned int size;
   unsigned int target;
   char type[16];
   // maybe we have more info in the future, ex:checksum
};

struct S_IMGDESC
{
    char imgname[32];
    struct S_IMGLOCATION loc;
};

struct S_IMGLIST
{
   unsigned int fwcount;
   struct S_IMGDESC list[FWTYPE_END];
};

enum E_FS
{
   FS_SQUASH,
   FS_JFFS2,
   FS_YAFFS2,
   FS_RAWFILE,
   FS_END
};
struct S_PARTDESC
{
   char partname[32];
   char mount_point[64];
   char type[16];
   enum E_FS efs;
   struct S_IMGLOCATION loc;
};

struct S_PARTLIST
{
   unsigned int partcount;
   struct S_PARTDESC list[20];
};

enum E_TAG
{
   TAG_UNKNOWN = 0,
   TAG_BOOTCODE,
   TAG_FACTORY,
   TAG_UPDATE_ETC,
   TAG_UPDATE_NL,
   TAG_UPDATE_NL_ROOTFS,
   TAG_UPDATE_AK,
   TAG_UPDATE_VK,
   TAG_UPDATE_DALOGO,
   TAG_UPDATE_DVLOGO,
   TAG_UPDATE_OTHER_PARTITION,
   TAG_COMPLETE,
};

/*const char TAG_PART_NAME[][64] = {
   "UNKNOWN",
   "BOOTCODE",
   "FACTORY",
   "ETC",
   "NORMAL_LINUX",
   "NORMAL_ROOTFS",
   "AUDIO_FW",
   "VIDEO_FW",
   "DALOGO",
   "DVLOGO",
   "OTHER_PARTITION"
   "COMPLETE"
};*/



struct S_BOOTTABLE
{
   int boottype; // map to E_BOOTTYPE
   int  tag;  // map to E_TAG
   char imgcksum[36];
   char url[256];
   int ssu_work_part;
   struct S_IMGLIST fw;
   struct S_PARTLIST part;
#ifdef EMMC_SUPPORT
   int mbr_matched;
#endif
};
typedef struct S_BOOTTABLE CBootable;
    
#ifdef EMMC_SUPPORT
struct S_MBR_PARTITION_ENTRY
{
    unsigned char active;
    unsigned char head_start;
    unsigned char sector_start;
    unsigned char cylinder_start;
    unsigned char id;
    unsigned char head_end;
    unsigned char sector_end;
    unsigned char cylinder_end;
    unsigned int rel_sector_start;
    unsigned int sector_num;
};

struct __attribute__((packed)) S_MBR_TABLE
{
	unsigned char codes[446];
	struct S_MBR_PARTITION_ENTRY part[4];
	unsigned short signature;
};
#endif
#endif
