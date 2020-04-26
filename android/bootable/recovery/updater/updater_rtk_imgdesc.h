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

#ifndef _UPDATER_UPDATER_RTK_IMGDESC_H_
#define _UPDATER_UPDATER_RTK_IMGDESC_H_

#define CONFIGURATION_MAX_SIZE 10*1024

#define EMMC_SUPPORT

enum eRTK_IMAGE_STATUS
{
   rUNINSTALL,
   rUPGRADING,
   rCOMPLETED
};
typedef enum eRTK_IMAGE_STATUS eRTK_IMAGE_STATUS;

enum eIMAGE_STATUS
{
   iUNINSTALL,
   iINSTALLING,
   iCOMPLETED
};
typedef enum eIMAGE_STATUS eIMAGE_STATUS;

enum eMode
{
   _INSTALL,
   _UPGRAD,
};
typedef enum eMode eMode;

enum eFS
{
   FS_TYPE_JFFS2,
   FS_TYPE_YAFFS2,
   FS_TYPE_SQUASH,
   FS_TYPE_RAWFILE,
#ifdef EMMC_SUPPORT
   FS_TYPE_EXT4,
#endif
   FS_TYPE_NONE,
   FS_TYPE_UNKOWN
};
typedef enum eFS eFS;

enum eBURN
{
   BURN_BY_NANDWRITE,
   BURN_BY_MTDBLOCK,
#ifdef EMMC_SUPPORT
   BURN_BY_MMCBLOCK,
#endif
};
typedef enum eBURN eBURN;

enum FWTYPE
{
   FW_KERNEL=0,
   FW_RESCUE_DT,
   FW_KERNEL_DT,
   FW_RESCUE_ROOTFS,
   FW_KERNEL_ROOTFS,
   FW_NAS_KERNEL,
   FW_NAS_RESCUE_DT,
   FW_NAS_RESCUE_ROOTFS,
   FW_AUDIO,
   FW_TEE,
   FW_VIDEO, //10
   FW_VIDEO2,
   FW_ECPU,
   FW_AUDIO_BOOTFILE,
   FW_IMAGE_BOOTFILE,
   FW_VIDEO_BOOTFILE,
   FW_AUDIO_CLOGO1,
   FW_IMAGE_CLOGO1,
   FW_VIDEO_CLOGO1,
   FW_AUDIO_CLOGO2,
   FW_IMAGE_CLOGO2, //20
   FW_VIDEO_CLOGO2,
   FW_ROOTFS,
   FW_RES,
   FW_CACHE,
   FW_DATA,
   FW_SYSTEM,
   P_PARTITION1, //27
   P_PARTITION2,
   P_PARTITION3,
   P_PARTITION4,
   P_PARTITION5,
   P_PARTITION6,
   P_PARTITION7,
   P_PARTITION8,
   FW_USR_LOCAL_ETC,
   FW_BOOTCODE,
   FW_FACTORY,
   FW_FW_TBL,
   P_SROOTFS,
   FW_RESCUE, //40
   FW_P_SSU_WORKING,
   FW_P_FREE_SPACE,
   FW_NORBOOTCODE,
   FW_GOLD_RESCUE_DT, //44
   FW_GOLD_AUDIO,
   FW_GOLD_RESCUE_ROOTFS,
   FW_GOLD_KERNEL,
   FW_GOLD_FW_TBL,
   FW_UBOOT, //49
   FW_SE_STORAGE, //50
   FW_BL31,
   FW_UNKNOWN
};
typedef enum FWTYPE FWTYPE;

struct t_PARTDESC
{
   FWTYPE efwtype;
   char partition_name[32];
   char mount_point[32];
   eFS efs;
   char filename[32];
   unsigned long long min_size;
   char mount_dev[32];
};

struct t_FWDESC
{
   FWTYPE efwtype;
   char firmware_name[32];
   char filename[64];
   unsigned int target;
};

enum ePARTIAL_INSTALL
{
   PARTIAL_NOT_PASS=0,
   PARTIAL_PASS
};


enum eINSTALL_MODE
{
   eUPDATE = 0,
   eBACKUP,
   eSKIP,
   eFIRSTTIME,		// there was no this image before, and we need to something at the first time.
};

struct t_imgdesc
{
   enum FWTYPE efwtype;
   /* install status */
   enum ePARTIAL_INSTALL pass[2];
   /* current install offset */
   unsigned int install_offset;
   /* file posotion*/
   unsigned int tarfile_offset;
   unsigned int img_size;
   unsigned long long flash_allo_size;
   /* redundant info*/
   unsigned long long sector;
   /* flash position*/
   unsigned long long flash_offset;
   /* memory target position*/
   unsigned int mem_offset;
   /* checksum */
   unsigned int checksum;
   unsigned int extracted;

   //fw compress type
   char compress_type[16];

   //partition info
   char part_name[32];
   char fs_name[16];

   //mount point
   char mount_point[64];
   char dev_path[64];
   char filename[100];

   //mode
   enum eINSTALL_MODE install_mode;
   char backup_filename[100];
   unsigned char sha_hash[32];
};
struct t_rtkimgdesc
{
   enum eRTK_IMAGE_STATUS status;
   unsigned int magic;
   unsigned int ver;
   enum eMode mode;

   char* mtdblock_path;
   char* mtd_path;
   int next_ssu_work_part;
   unsigned int norflash_size_32MB;

   // config info
   // yes: 1    no: 0
   unsigned int bootcode;
   unsigned int factory;
   unsigned int gui;
   unsigned int fake_burn;
   unsigned int verify;
   unsigned int ignore_native_rescue;
   unsigned int test_tag;
   unsigned int ignore_img_cksum;
   unsigned int force_install;
   unsigned int update_etc;
   unsigned int erase;
   unsigned int partition_inverse;
   unsigned int only_factory;
   unsigned int safe_upgrade;
   unsigned int erase_free_space;
   unsigned int check_version;
   unsigned int jffs2_nocleanmarker;
   unsigned int dump_flash;
   unsigned int update_cavfile;
  
   // var
   int install_avfile_count;
   int install_avfile_audio_size; //size in KB
   int install_avfile_video_size; //size in KB
   
   int rba_percentage;

   // commands for installing factory
   char ifcmd0[512];
   char ifcmd1[512];

   // for customer_ap
   FILE *customer_fp;
   unsigned int progress;

   // system info
   unsigned int rescue_start;
   unsigned int rescue_size;
   unsigned int factory_start;
   unsigned int factory_section_size;
   unsigned int factory_size;
   unsigned int restrom_factory_size;
   unsigned int bootcode_start;
   unsigned int bootcode_size;
   // meminfo
   unsigned int flash_type;
   unsigned int mtd_erasesize;
   unsigned long long flash_size;
   unsigned int page_size;
   unsigned int oob_size;

   // flash layout fo nand
   unsigned int reserved_boot_size;
   unsigned int reserved_boottable_size;
   unsigned int etc_partition_size;
   unsigned int preserve_partition_size;
   unsigned int reserved_remapping_offset;
   unsigned int reserved_remapping_size;
   unsigned int reserved_remapping_sector;
   unsigned int startAddress;
   unsigned int endAddress;

   unsigned int secure_boot;

   enum eBURN eburn;

   struct t_imgdesc fw[FW_UNKNOWN];

   struct t_imgdesc bootloader_tar;
   struct t_imgdesc factory_tar;     // not used in android recovery
   struct t_imgdesc mkyaffs2image;
   struct t_imgdesc nandwrite;
   struct t_imgdesc flash_erase;
   struct t_imgdesc mkfs_jffs2;
   struct t_imgdesc rescue2;
   struct t_imgdesc video_firmware;
   // new version config framework
   struct t_imgdesc config;

   unsigned int checksum;
   unsigned char sha_hash[32];
};
typedef struct t_rtkimgdesc CRTKImgDesc;

#endif
