/*
 * Copyright (c) 2008 Travis Geiselbrecht
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files
 * (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <debug.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lib/console.h>
#include <lib/sysparam.h>
#include <lib/bio.h>
#include <lib/mincrypt/sha256.h>
#include <platform/io.h>
#include <platform/setting.h>
#include <platform/rbus/emmc_reg.h>
#include <platform/emmc.h>
#include <rtkboot.h>
#include <fw_info.h>
#include <arch/ops.h>

#if HW_MCP
#include <platform/mcp.h>
#endif

#include <trace.h>

#define LOCAL_TRACE 0
//#define DUBUG_FW_DESC_TABLE
//#define FW_TABLE_SIGN 1
#define FW_TABLE_ADDR	(0x54000000)

BOOT_FLASH_T boot_flash_type;
bdev_t *system_flash;
static int flash_use_block_op = 0;
unsigned int flash_blk_size = 0;
unsigned int Auto_AFW_MEM_START = 0; /* Save the address of AFW from fw_desc */

addr_t fw_desc_table_start = 0; // Start address in block device
void *fw_desc_table_ddr_base = 0; // load address in DDR

int rtk_plat_read_fw_image(part_desc_entry_v1_t*,int,void*,int,uchar);

#define MAX_IMG_NUM	8 // Number of images per bootmode

const int boot_mode_img_table[BOOT_MODE_MAX][MAX_IMG_NUM] = {
	[BOOT_CONSOLE_MODE] = {0},
	[BOOT_RESCUE_MODE] = {
#ifdef VMX
		FW_TYPE_RESCUE_KERNEL,
#else
		FW_TYPE_KERNEL,
#endif
		FW_TYPE_RESCUE_DT,
		FW_TYPE_RESCUE_ROOTFS,
#ifdef VMX
		FW_TYPE_RESCUE_AUDIO,
#else
		FW_TYPE_AUDIO,
#endif
		FW_TYPE_IMAGE_FILE,
		0,
	},
	[BOOT_NORMAL_MODE] = {
		FW_TYPE_KERNEL,
		FW_TYPE_KERNEL_DT,
		FW_TYPE_KERNEL_ROOTFS,
		FW_TYPE_AUDIO,
		FW_TYPE_IMAGE_FILE,
		FW_TYPE_HYP,
		0,
	},
	[BOOT_MANUAL_MODE] = {
		FW_TYPE_KERNEL,
		FW_TYPE_KERNEL_DT,
		FW_TYPE_KERNEL_ROOTFS,
		FW_TYPE_AUDIO,
		FW_TYPE_IMAGE_FILE,
		FW_TYPE_HYP,
		0,
	},
	[BOOT_KERNEL_ONLY_MODE] = {
		FW_TYPE_KERNEL,
		FW_TYPE_KERNEL_DT,
		FW_TYPE_KERNEL_ROOTFS,
		FW_TYPE_HYP,
		0,
	},
	[BOOT_RECOVERY_MODE] = {
		FW_TYPE_KERNEL,
		FW_TYPE_RESCUE_DT,
		FW_TYPE_RESCUE_ROOTFS,
		FW_TYPE_AUDIO,
		FW_TYPE_IMAGE_FILE,
		0,
	},
	[BOOT_GOLD_MODE] = {
#ifdef NAS_DUAL
		FW_TYPE_KERNEL,
		FW_TYPE_KERNEL_DT,
		FW_TYPE_KERNEL_ROOTFS,
		FW_TYPE_AUDIO,
		FW_TYPE_IMAGE_FILE,
		FW_TYPE_HYP,
#else
		FW_TYPE_GOLD_KERNEL,
		FW_TYPE_GOLD_RESCUE_DT,
		FW_TYPE_GOLD_RESCUE_ROOTFS,
		FW_TYPE_GOLD_AUDIO,
#endif
		0,
	},
	[BOOT_FASTBOOT_MODE] = {0},
	[BOOT_S5_MODE] = {
		FW_TYPE_AUDIO,
		0,
	},
};

#define IMG_LOAD(fw_type) \
({ \
	int i=0, ret=0; \
	for (i = 0 ; i < MAX_IMG_NUM ; i++) { \
		if (boot_mode_img_table[boot_mode][i] == 0) \
			break; \
		if (boot_mode_img_table[boot_mode][i] == fw_type) { \
			ret = 1; \
			break; \
		} \
	} \
	ret; \
})

static int get_fwtbl_start_address(addr_t *factory_addr)
{
	*factory_addr = 0;

	if (boot_flash_type == EMMC_FLASH) {
		if (boot_mode == BOOT_GOLD_MODE)
			*factory_addr = EMMC_BOOTCODE_AREA_SIZE + FACTORY_SIZE + FW_TABLE_SIZE;
		else
			*factory_addr = EMMC_BOOTCODE_AREA_SIZE + FACTORY_SIZE;
	} else if (boot_flash_type == NAND_FLASH) {
#ifdef NAS_ENABLE
		int factory_blocks = FACTORY_BLOCKS;
#else
		int factory_blocks = 0;
#endif
		// NAS with NAND flash use 4 blocks as factory region instead of static size.
		if (factory_blocks == 0)
			factory_blocks = ALIGN(FACTORY_SIZE, flash_blk_size) / flash_blk_size;

		*factory_addr = NAND_TOTAL_RESERVE_BLOCKS * flash_blk_size + NAND_TOTAL_RESERVE_SIZE;
		*factory_addr += factory_blocks * flash_blk_size; // Add factory region offset

		if (boot_mode == BOOT_GOLD_MODE)
			*factory_addr += 1 * flash_blk_size; // Add normal FWTBL offset
	} else {
		printf("%s, boot_flash_type %d not support!!\n", __func__, boot_flash_type);
		return -1;
	}

	return 0;
}

void set_boot_flash_type(void)
{
	boot_flash_type = FLASH_TYPE;

	if (FLASH_TYPE == SPI_FLASH) {
		LTRACEF("BootFlash SPI\n");
		system_flash = bio_open(BIO_SPI_NAME);
	} else if (FLASH_TYPE == NAND_FLASH) {
		LTRACEF("BootFlash NAND\n");
		rtd_outl(EMMC_MUXPAD0,0x55555554);
		system_flash = bio_open(BIO_NAND_NAME);
		flash_blk_size = bio_ioctl(system_flash, BIO_IOCTL_PUT_MEM_MAP, NULL);
	} else if (FLASH_TYPE == EMMC_FLASH) {
		LTRACEF("BootFlash EMMC\n");
		rtd_maskl(EMMC_MUXPAD0, ~0xFFFF0FFF, 0xaaaa0aa8);
		system_flash = bio_open(BIO_MMC_NAME);
		flash_blk_size = EMMC_BLOCK_SIZE;
		flash_use_block_op = 1;
	} else {
		LTRACEF("!!! No FLASH TYPE assigned !!!!\n");
	}
}

#ifdef DUBUG_FW_DESC_TABLE
void dump_fw_desc_table_v1(fw_desc_table_v1_t *fw_desc_table_v1)
{
	if (fw_desc_table_v1 != NULL) {
		printf("## Fw Desc Table ##############################\n");
		printf("## fw_desc_table_v1                = %8p\n", fw_desc_table_v1);
		printf("## fw_desc_table_v1->signature     = %s\n", fw_desc_table_v1->signature);
		printf("## fw_desc_table_v1->checksum      = 0x%08x\n", fw_desc_table_v1->checksum);
		printf("## fw_desc_table_v1->version       = 0x%08x\n", fw_desc_table_v1->version);
#ifdef NAS_DUAL
		printf("## fw_desc_table_v1->seqnum        = 0x%08x\n", fw_desc_table_v1->seqnum);
#endif
		printf("## fw_desc_table_v1->paddings      = 0x%08x\n", fw_desc_table_v1->paddings);
		printf("## fw_desc_table_v1->part_list_len = 0x%08x\n", fw_desc_table_v1->part_list_len);
		printf("## fw_desc_table_v1->fw_list_len   = 0x%08x\n", fw_desc_table_v1->fw_list_len);
		printf("###############################################\n\n");
	} else {
		printf("[ERR] %s:%d fw_desc_table_v1 is NULL.\n", __FUNCTION__, __LINE__);
	}
}

void dump_part_desc_entry_v1(part_desc_entry_v1_t *part_entry)
{
	if (part_entry != NULL) {
		printf("## Part Desc Entry ############################\n");
		printf("## part_entry                      = %8p\n", part_entry);
		printf("## part_entry->type                = 0x%08x\n", part_entry->type);
		printf("## part_entry->ro                  = 0x%08x\n", part_entry->ro);
		printf("## part_entry->length              = 0x%08llx\n", part_entry->length);
		printf("## part_entry->fw_count            = 0x%08x\n", part_entry->fw_count);
		printf("## part_entry->fw_type             = 0x%08x\n", part_entry->fw_type);
		printf("###############################################\n\n");
	} else {
		printf("[ERR] %s:%d part_entry is NULL.\n", __FUNCTION__, __LINE__);
	}
}

void dump_fw_desc_entry_v1(fw_desc_entry_v1_t *fw_entry)
{
	if (fw_entry != NULL) {
		printf("## Fw Desc Entry ##############################\n");
		printf("## fw_entry                        = %8p\n", fw_entry);
		printf("## fw_entry->type                  = 0x%08x\n", fw_entry->type);
		printf("## fw_entry->lzma                  = 0x%08x\n", fw_entry->lzma);
		printf("## fw_entry->ro                    = 0x%08x\n", fw_entry->ro);
		printf("## fw_entry->version               = 0x%08x\n", fw_entry->version);
		printf("## fw_entry->target_addr           = 0x%08x\n", PTR_UINT32(fw_entry->target_addr));
		printf("## fw_entry->offset                = 0x%08x\n", fw_entry->offset);
		printf("## fw_entry->length                = 0x%08x\n", fw_entry->length);
		printf("## fw_entry->paddings              = 0x%08x\n", fw_entry->paddings);
		printf("## fw_entry->checksum              = 0x%08x\n", fw_entry->checksum);
		printf("###############################################\n\n");
	} else {
		printf("[ERR] %s:%d fw_entry is NULL.\n", __FUNCTION__, __LINE__);
	}
}

void dump_fw_desc_entry_v2(fw_desc_entry_v2_t *fw_entry)
{
	int i;

	if (fw_entry != NULL) {
		printf("## Fw Desc Entry ##############################\n");
		printf("## fw_entry                        = %8p\n", fw_entry);
		printf("## fw_entry->type                  = 0x%08x\n", fw_entry->type);
		printf("## fw_entry->lzma                  = 0x%08x\n", fw_entry->lzma);
		printf("## fw_entry->ro                    = 0x%08x\n", fw_entry->ro);
		printf("## fw_entry->version               = 0x%08x\n", fw_entry->version);
		printf("## fw_entry->target_addr           = 0x%08x\n", fw_entry->target_addr);
		printf("## fw_entry->offset                = 0x%08lx\n", (unsigned long)fw_entry->offset);
		printf("## fw_entry->length                = 0x%08x\n", fw_entry->length);
		printf("## fw_entry->paddings              = 0x%08x\n", fw_entry->paddings);
		printf("## fw_entry->sha_hash              = ");
		for (i = 0 ; i < 32 ; i++)
			printf("%02x", fw_entry->sha_hash[i]);
		printf("\n");
		printf("###############################################\n\n");
	} else {
		printf("[ERR] %s:%d fw_entry is NULL.\n", __FUNCTION__, __LINE__);
	}
}
#endif //DUBUG_FW_DESC_TABLE

#ifdef NAS_ENABLE
#define NAS_ROOT "/"
#define NAS_ETC "etc"
static int setup_nas_part(part_desc_entry_v1_t* part_entry, int part_count)
{
	const char *nas_root = NAS_ROOT;
	if(boot_mode == BOOT_RESCUE_MODE || boot_mode == BOOT_RECOVERY_MODE)
		return RTK_PLAT_ERR_OK;

	const char *device_path = NULL;
	if (boot_flash_type == EMMC_FLASH)
		device_path = "/dev/mmcblk0p";
	else if (boot_flash_type == NAND_FLASH)
		device_path = "/dev/mtdblock";

	if (!device_path)
		return RTK_PLAT_ERR_BOOT;

#ifdef NAS_DUAL
	if(boot_mode == BOOT_GOLD_MODE)
		nas_root = "/root";
#endif

	char *tmp_cmdline = NULL;
	tmp_cmdline = (char*)malloc(128);
	if (!tmp_cmdline) {
		printf("%s: Malloc failed\n", __func__);
		return RTK_PLAT_ERR_BOOT;
	}
	memset(tmp_cmdline, 0, 128);

	unsigned char empty_mount[sizeof(part_entry->mount_point)];
	memset(empty_mount, 0, sizeof(empty_mount));

	int i;
	for(i = 0; i < part_count; i++) {
		if (memcmp(empty_mount, part_entry[i].mount_point, sizeof(empty_mount)) != 0) {
			if (0 == strcmp((const char*)part_entry[i].mount_point, nas_root)){
				int minor_num = i;
				if (i >= 3 && boot_flash_type == EMMC_FLASH)
					minor_num += 1;

				strcpy(tmp_cmdline, "init=/etc/init ");

				int cmd_len = strlen(tmp_cmdline);
				const char *rootfstype = "";
				const char *opts = "";
				switch (part_entry[i].fw_type) {
					case FS_TYPE_SQUASH:
						rootfstype = "squashfs";
						if (boot_flash_type == NAND_FLASH) {
						opts = " ubi.mtd=/ ubi.block=0,rootfs";
						device_path = "/dev/ubiblock0_";
						minor_num = 0;
						}
						break;
					case FS_TYPE_EXT4:
						rootfstype = "ext4";
						opts = " ro";
						break;
					case FS_TYPE_UBIFS:
						rootfstype = "ubifs";
						opts = " rw ubi.mtd=/";
						break;
					default:
						free(tmp_cmdline);
						return RTK_PLAT_ERR_PARSE_FW_DESC;
				}

				if (FS_TYPE_UBIFS == part_entry[i].fw_type) {
					snprintf(tmp_cmdline+cmd_len, 127-cmd_len,
						"root=ubi0:rootfs%s rootfstype=%s ",
						opts,rootfstype);
				} else {
					snprintf(tmp_cmdline+cmd_len, 127-cmd_len,
						"root=%s%d%s rootfstype=%s rootwait ",
						device_path,minor_num,opts,rootfstype);
				}
				LTRACEF("NASROOT found. cmd:%s\n", tmp_cmdline);
			} else if (0 == strcmp((const char*)part_entry[i].mount_point, NAS_ETC)
					&& FS_TYPE_UBIFS == part_entry[i].fw_type){
				int cmd_len = strlen(tmp_cmdline);
				snprintf(tmp_cmdline+cmd_len, 127-cmd_len, "ubi.mtd=etc ");
				LTRACEF("NASETC found. cmd:%s\n", tmp_cmdline);
			}
		}
	}
	sysparam_add("nas_part", tmp_cmdline, strlen(tmp_cmdline));
	free(tmp_cmdline);

	return RTK_PLAT_ERR_OK;
}
#endif //NAS_ENABLE

unsigned int get_checksum(unsigned char *p, unsigned int len) {
	unsigned int checksum = 0;
	unsigned int i;
	for(i = 0; i < len; i++) {
		checksum += *(p+i);
	}
	return checksum;
}

int rtk_plat_prepare_fw_image()
{
	fw_desc_table_v1_t fw_desc_table_v1;
	fw_desc_entry_v1_t *fw_entry;
	part_desc_entry_v1_t *part_entry = NULL;
	int part_count = 0;
#ifndef VMX
	unsigned long bdev_addr, bdev_size;
#endif
	int fw_entry_num = 0;
	unsigned int checksum;
	int ret = 0;
    int FW_SIGN_OffSet = 0;
#ifdef NAS_DUAL
	BOOT_MODE boot_mode_bak;
	addr_t fw_desc_table_start2;
	fw_desc_table_v1_t fw_desc_table_v1_2;
	static int boot_fwdesc_seq = 0;
#endif

#ifdef NAS_DUAL
	if (boot_mode == BOOT_GOLD_MODE && !boot_fwdesc_seq)
#else
	if (boot_mode == BOOT_GOLD_MODE)
#endif
		printf("---------------LOAD  GOLD  FW  TABLE ---------------\n");
	else
#ifdef NAS_DUAL
	{
		printf("---------------LOAD  NORMAL FW  TABLE ---------------\n");
		if (boot_mode==BOOT_GOLD_MODE)
			boot_mode = BOOT_NORMAL_MODE;
	}
#else
		printf("---------------LOAD  NORMAL FW  TABLE ---------------\n");
#endif


	if (get_fwtbl_start_address(&fw_desc_table_start))
		return RTK_PLAT_ERR_READ_FW_IMG;

#ifndef VMX
	// Allocate size that is align to NAND page.
	if (!fw_desc_table_ddr_base) {
		if (boot_flash_type == NAND_FLASH)
			fw_desc_table_ddr_base = memalign(8, flash_blk_size);
		else
			fw_desc_table_ddr_base = memalign(8, 4096);
	}

	printf("fw_desc_table_start = 0x%08lx\n", fw_desc_table_start);
	printf("fw_desc_table_ddr_base = 0x%p\n", fw_desc_table_ddr_base);

	/* Firmware Description Table is right behind bootcode blocks */
#ifdef FW_TABLE_SIGN
#if PLATFORM_RTD1295
    FW_SIGN_OffSet = 520; // 4Bytes FW_TABLE_SIZE + 516 Bytes HW_RSA_Signed
#elif PLATFORM_RTD1395
    FW_SIGN_OffSet = 524; // 4Bytes FW_TABLE_SIZE + 520 Bytes HW_RSA_Signed
#endif
#endif

	bdev_addr_size(flash_use_block_op, flash_blk_size, fw_desc_table_start,
			sizeof(fw_desc_table_v1_t) + FW_SIGN_OffSet, &bdev_addr, &bdev_size);

	/* copy Firmware Description Table Header from flash */
	ret = BDEV_READ(flash_use_block_op, system_flash, fw_desc_table_ddr_base, bdev_addr, bdev_size);

	if (ret < (int)bdev_size) {
		printf("[ERR] %s:Read fw_desc_table_v1_t error! (0x%lx, 0x%lx, 0x%p)\n",
			__FUNCTION__, bdev_addr, bdev_size, fw_desc_table_ddr_base);
		return RTK_PLAT_ERR_PARSE_FW_DESC;
	}
#else
	fw_desc_table_ddr_base = (void *)FW_TABLE_ADDR;
	printf("fw_desc_table_ddr_base = 0x%p\n", fw_desc_table_ddr_base);
#endif

	/* copy Firmware Description Table Header from flash */
	memcpy(&fw_desc_table_v1, fw_desc_table_ddr_base + FW_SIGN_OffSet, sizeof(fw_desc_table_v1_t));

	/* Check signature first! */
	if (strncmp((const char *)fw_desc_table_v1.signature,
			FIRMWARE_DESCRIPTION_TABLE_SIGNATURE, sizeof(fw_desc_table_v1.signature)) != 0) {
		printf("[ERR] %s:Signature(%s) error!\n", __FUNCTION__, fw_desc_table_v1.signature);
		return RTK_PLAT_ERR_PARSE_FW_DESC;
	}

#ifdef DUBUG_FW_DESC_TABLE
	dump_fw_desc_table_v1(&fw_desc_table_v1);
#endif

#if defined(NAS_DUAL) && !defined(VMX)
	if (BOOT_GOLD_MODE == boot_mode)
		printf("[nas-dual]Golden fwdesc tbl seqnum: %d\n",fw_desc_table_v1.seqnum);
	else
		printf("[nas-dual]Normal fwdesc tbl seqnum: %d\n",fw_desc_table_v1.seqnum);

	if (boot_mode != BOOT_GOLD_MODE && !boot_fwdesc_seq) {
		boot_mode_bak = boot_mode;
		boot_mode = BOOT_GOLD_MODE;

		if (get_fwtbl_start_address(&fw_desc_table_start2))
			return RTK_PLAT_ERR_READ_FW_IMG;

		printf("fw_desc_table_start = 0x%08lx\n", fw_desc_table_start2);
		printf("fw_desc_table_ddr_base = 0x%p\n", fw_desc_table_ddr_base);

		bdev_addr_size(flash_use_block_op, flash_blk_size, fw_desc_table_start2,
				sizeof(fw_desc_table_v1_t) + FW_SIGN_OffSet, &bdev_addr, &bdev_size);

		/* copy Firmware Description Table Header from flash */
		ret = BDEV_READ(flash_use_block_op, system_flash, fw_desc_table_ddr_base, bdev_addr, bdev_size);

		if (ret < (int)bdev_size) {
			printf("[ERR] %s:Read fw_desc_table_v1_t error! (0x%lx, 0x%lx, 0x%p)\n",
				__FUNCTION__, bdev_addr, bdev_size, fw_desc_table_ddr_base);
			boot_mode = boot_mode_bak;
		}
		else {
			/* copy Firmware Description Table Header from flash */
			memcpy(&fw_desc_table_v1_2, fw_desc_table_ddr_base + FW_SIGN_OffSet, sizeof(fw_desc_table_v1_t));

			/* Check signature first! */
			if (strncmp((const char *)fw_desc_table_v1_2.signature,
					FIRMWARE_DESCRIPTION_TABLE_SIGNATURE, sizeof(fw_desc_table_v1_2.signature)) != 0) {
				printf("[ERR] %s:Signature(%s) error!\n", __FUNCTION__, fw_desc_table_v1_2.signature);
				boot_mode = boot_mode_bak;
			}
			else {
#ifdef DUBUG_FW_DESC_TABLE
				dump_fw_desc_table_v1(&fw_desc_table_v1_2);
#endif
			if ((fw_desc_table_v1_2.seqnum > fw_desc_table_v1.seqnum) || \
					(0 == fw_desc_table_v1_2.seqnum && 255 == fw_desc_table_v1.seqnum)) {
					printf("[nas-dual]Golden fwdesc tbl seqnum: %d\n",fw_desc_table_v1_2.seqnum);
					boot_fwdesc_seq = 1;
					memcpy(&fw_desc_table_v1, &fw_desc_table_v1_2, sizeof(fw_desc_table_v1_t));
					fw_desc_table_start = fw_desc_table_start2;
					/* other modes except NORMAL mode are allowed in 2nd Firmware Description Table */
					if (boot_mode_bak != BOOT_NORMAL_MODE)
						boot_mode = boot_mode_bak;
			}
			else
				boot_mode = boot_mode_bak;
			}
		}
	}
#endif //NAS_DUAL

#ifndef VMX
	/* copy Firmware Description/Partition/Fw_entry Tables from flash */
	bdev_addr_size(flash_use_block_op, flash_blk_size, fw_desc_table_start,
			fw_desc_table_v1.paddings + FW_SIGN_OffSet, &bdev_addr, &bdev_size);
	ret = BDEV_READ(flash_use_block_op, system_flash, fw_desc_table_ddr_base, bdev_addr, bdev_size);

	if (ret < (int)bdev_size) {
		printf("[ERR] %s:Read all fw tables error! (0x%lx, 0x%lx, 0x%p)\n",
			__FUNCTION__, bdev_addr, bdev_size, fw_desc_table_ddr_base);
		return RTK_PLAT_ERR_PARSE_FW_DESC;
	}
#endif

	/* Check checksum */
	checksum = get_checksum(fw_desc_table_ddr_base +
	            FW_SIGN_OffSet +
				offsetof(fw_desc_table_v1_t, version),
				sizeof(fw_desc_table_v1_t) -
				offsetof(fw_desc_table_v1_t, version) +
				fw_desc_table_v1.part_list_len +
				fw_desc_table_v1.fw_list_len);

	if (fw_desc_table_v1.checksum != checksum) {
		printf("[ERR] %s:Checksum not match(0x%x != 0x%x)\n", __FUNCTION__,
			fw_desc_table_v1.checksum, checksum);
		return RTK_PLAT_ERR_PARSE_FW_DESC;
	}


	/* Check partition existence */
	if (fw_desc_table_v1.part_list_len == 0) {
		printf("[ERR] %s:No partition found!\n", __FUNCTION__);
		//return RTK_PLAT_ERR_PARSE_FW_DESC;
	} else {
		part_count = fw_desc_table_v1.part_list_len / sizeof(part_desc_entry_v1_t);
		part_entry = (part_desc_entry_v1_t *)(fw_desc_table_ddr_base + FW_SIGN_OffSet + sizeof(fw_desc_table_v1_t));
	}

	// parse partition info if NAND flash used
	if (boot_flash_type == NAND_FLASH) {
		unsigned char empty_mount[sizeof(part_entry->mount_point)];
		char buf[64], tmp[16];
		char *tmp_cmdline = NULL;
		int param_len = 0;
		unsigned long size;
        unsigned long used_size = 0;
        unsigned long nand_size = 0;

		sysparam_remove("mtd_part");
		sysparam_add("mtd_part", "mtdparts=rtk_nand:", strlen("mtdparts=rtk_nand:") + 1); // Create a NULL mtd_part info

		memset(empty_mount, 0, sizeof(empty_mount));

        for(int i = 0; i < part_count; i++) {
            nand_size = nand_size + part_entry[i].length;
        }

		for(int i = 0; i < part_count - 1; i++) {
			//size = (part_entry[i].length >> 10);
			size = part_entry[i].length;
			memset(buf, 0, sizeof(buf));
			memset(tmp, 0, sizeof(tmp));
			snprintf(tmp, 16, "%lu", size);

            if (i == 0) {
                used_size = size;
                sprintf(buf, "%lum@0x0(rtk_nand)", nand_size >> 20);
            }
            else {
#ifdef NAS_ENABLE
                sprintf(buf, ",%lum@0x%lx(%s)", size >> 20, used_size, part_entry[i].mount_point);
#else
                sprintf(buf, ",%lum@0x%lx(part%d)", size >> 20, used_size, i);
#endif
                used_size = used_size + size;
            }
#if 0
			if (memcmp(empty_mount, part_entry[i].mount_point, sizeof(empty_mount)) != 0)
				sprintf(buf, "%sk(%s%s",tmp, part_entry[i].mount_point,i == part_count-1? ")" :"),");
			else
				sprintf(buf, "%s%s",tmp,i == part_count-1? "k" :"k,");
#endif
			LTRACEF("new mtd partition:%s\n", buf);
			if ((param_len = sysparam_length("mtd_part")) < 0) {
				printf("%s: Couldn't find sysparam 'mtd_part'\n", __func__);
				return RTK_PLAT_ERR_PARSE_FW_DESC;
			}

			tmp_cmdline = (char *)malloc(param_len + 60);

			if (!tmp_cmdline) {
				printf("%s: Malloc failed\n", __func__);
				return RTK_PLAT_ERR_PARSE_FW_DESC;
			} else {
				sysparam_read("mtd_part", tmp_cmdline, param_len);
				strcat(tmp_cmdline, buf);
				sysparam_add("mtd_part", tmp_cmdline, strlen(tmp_cmdline) + 1);
				LTRACEF("Curret mtd_part:%s\n", tmp_cmdline);
				free(tmp_cmdline);
			}
		}
	}

#ifdef DUBUG_FW_DESC_TABLE
	for (int i = 0 ; i < part_count ; i++) {
		dump_part_desc_entry_v1(&(part_entry[i]));
	}
#endif

	switch (fw_desc_table_v1.version) {
		case FW_DESC_TABLE_V1_T_VERSION_1:
			fw_entry_num = fw_desc_table_v1.fw_list_len / sizeof(fw_desc_entry_v1_t);
			break;
		case FW_DESC_TABLE_V1_T_VERSION_11:
			fw_entry_num = fw_desc_table_v1.fw_list_len / sizeof(fw_desc_entry_v11_t);
			break;
		case FW_DESC_TABLE_V1_T_VERSION_21:
			fw_entry_num = fw_desc_table_v1.fw_list_len / sizeof(fw_desc_entry_v21_t);
			break;
		case FW_DESC_TABLE_V2_T_VERSION_2:
			fw_entry_num = fw_desc_table_v1.fw_list_len / sizeof(fw_desc_entry_v2_t);
			break;
		case FW_DESC_TABLE_V2_T_VERSION_12:
			fw_entry_num = fw_desc_table_v1.fw_list_len / sizeof(fw_desc_entry_v12_t);
			break;
		case FW_DESC_TABLE_V2_T_VERSION_22:
			fw_entry_num = fw_desc_table_v1.fw_list_len / sizeof(fw_desc_entry_v22_t);
			break;
		default:
			printf("%s: unsupport FW DESC TBL version 0x%x\n", __func__, fw_desc_table_v1.version);
			return RTK_PLAT_ERR_PARSE_FW_DESC;
			break;
	}

	printf("[INFO] fw desc table base: 0x%08lx, count: %d\n", fw_desc_table_start, fw_entry_num);

	fw_entry = (fw_desc_entry_v1_t *)(fw_desc_table_ddr_base + FW_SIGN_OffSet +
		sizeof(fw_desc_table_v1_t) +
		fw_desc_table_v1.part_list_len);

	printf("[DEBUG] fw_entry = %8p\n", fw_entry);

#ifdef DUBUG_FW_DESC_TABLE
	unsigned int fw_entry_size = fw_desc_table_v1.fw_list_len / fw_entry_num;
	void *fw_desc_ptr = fw_entry;

	for (int i = 0 ; i < fw_entry_num ; i++) {
		if (FW_DESC_BASE_VERSION(fw_desc_table_v1.version) == 1)
			dump_fw_desc_entry_v1((fw_desc_entry_v1_t*)fw_desc_ptr);
		else if (FW_DESC_BASE_VERSION(fw_desc_table_v1.version) == 2)
			dump_fw_desc_entry_v2((fw_desc_entry_v2_t*)fw_desc_ptr);
		fw_desc_ptr += fw_entry_size;
	}
#endif
#ifdef NAS_ENABLE
	if((ret = setup_nas_part(part_entry, part_count)))
		return ret;
#endif

	printf("Normal boot fw flow...\n");
	ret = rtk_plat_read_fw_image(
		part_entry, part_count,
		fw_entry, fw_entry_num,
		fw_desc_table_v1.version);

	return ret;
}

int rtk_plat_read_fw_image(
	part_desc_entry_v1_t* part_entry, int part_count,
	void* fw_entry, int fw_count,
	uchar version)
{
	void *this_entry;
	unsigned int unit_len;
	int i, j, ret;
	char buff[ADDR_BUF_LEN] = "";
	uchar entry_type = 0;
	//uchar entry_lzma;
	uint entry_target_addr = 0;
	u64 entry_offset = 0;
	uint entry_length = 0;
	unsigned int fw_checksum;
	unsigned char sha_hash[32];
	unsigned long bdev_addr, bdev_size;
	addr_t rescue_rootfs_addr;

	if (sysparam_read("rootfs_loadaddr", buff, ADDR_BUF_LEN) < 0) {
		printf("%s, rootfs_loadaddr not found\n", __func__);
		return RTK_PLAT_ERR_READ_FW_IMG;
	}
	rescue_rootfs_addr = atoull(buff);

	/* find fw_entry structure according to version */
	switch (version) {
		case FW_DESC_TABLE_V1_T_VERSION_1:
			unit_len = sizeof(fw_desc_entry_v1_t);
			break;
		case FW_DESC_TABLE_V1_T_VERSION_11:
			unit_len = sizeof(fw_desc_entry_v11_t);
			break;
		case FW_DESC_TABLE_V1_T_VERSION_21:
			unit_len = sizeof(fw_desc_entry_v21_t);
			break;
		case FW_DESC_TABLE_V2_T_VERSION_2:
			unit_len = sizeof(fw_desc_entry_v2_t);
			break;
		case FW_DESC_TABLE_V2_T_VERSION_12:
			unit_len = sizeof(fw_desc_entry_v12_t);
			break;
		case FW_DESC_TABLE_V2_T_VERSION_22:
			unit_len = sizeof(fw_desc_entry_v22_t);
			break;
		default:
			printf("%s: FW_TBL version 0x%x invalid\n", __func__, version);
			return RTK_PLAT_ERR_READ_FW_IMG;
	}

	for (i = 0 ; i < fw_count ; i++) {
		this_entry = (fw_entry + unit_len * i);
		FW_ENTRY_MEMBER_GET(entry_target_addr, this_entry, target_addr, version);
		FW_ENTRY_MEMBER_GET(entry_type, this_entry, type, version);
		FW_ENTRY_MEMBER_GET(entry_length, this_entry, length, version);
		FW_ENTRY_MEMBER_GET(entry_offset, this_entry, offset, version);

		if (!entry_target_addr)
			continue;

		/* Check whether image need to load under this bootmode */
		if (!IMG_LOAD(entry_type))
			continue;
		
		if ( entry_type == FW_TYPE_AUDIO || entry_type == FW_TYPE_GOLD_AUDIO)
			Auto_AFW_MEM_START = entry_target_addr;
		/* Save the address of AFW from fw_desc */

		switch (entry_type) {
			case FW_TYPE_KERNEL:
			case FW_TYPE_RESCUE_KERNEL:
			case FW_TYPE_GOLD_KERNEL:
				snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)entry_target_addr);
				sysparam_add("kernel_loadaddr", buff, strlen(buff));
				printf("Kernel");
				break;
			case FW_TYPE_HYP:
				snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)entry_target_addr);
				sysparam_add("hyp_loadaddr", buff, strlen(buff));
				printf("Hypervisor");
				break;
			case FW_TYPE_RESCUE_DT:
			case FW_TYPE_KERNEL_DT:
			case FW_TYPE_GOLD_RESCUE_DT:
				snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)entry_target_addr);
				sysparam_add("fdt_loadaddr", buff, strlen(buff));
				printf("%sDT", entry_type == FW_TYPE_RESCUE_DT ? "Rescue-" : "");
				break;
			case FW_TYPE_RESCUE_ROOTFS:
				if (entry_target_addr != rescue_rootfs_addr) {
					if (!LK_FW_LOAD)
						memmove((void*)rescue_rootfs_addr, (void*)(u64)entry_target_addr, entry_length);

					printf("Rescue-ROOTFS: 0x%08llx -> 0x%08llx",
						(u64)entry_target_addr, (u64)rescue_rootfs_addr);
					entry_target_addr = rescue_rootfs_addr;
				} else {
					printf("Rescue-ROOTFS");
				}
				break;
			case FW_TYPE_KERNEL_ROOTFS:
			case FW_TYPE_GOLD_RESCUE_ROOTFS:
				snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)entry_target_addr);
				sysparam_add("rootfs_loadaddr", buff, strlen(buff));
				printf("ROOTFS");
				break;
			case FW_TYPE_AUDIO:
			case FW_TYPE_RESCUE_AUDIO:
			case FW_TYPE_GOLD_AUDIO:
				ipc_shm.audio_fw_entry_pt = SWAPEND32(entry_target_addr | MIPS_KSEG0BASE);
				snprintf(buff, ADDR_BUF_LEN, "0x%08llx", (unsigned long long)entry_target_addr);
				sysparam_add("audio_loadaddr", buff, strlen(buff));
				printf("Audio");
				break;
			case FW_TYPE_IMAGE_FILE:
				entry_target_addr = BOOT_LOGO_ADDR;
				printf("Address for boot logo from %x to %x will be reserved.\n", (unsigned int)BOOT_LOGO_ADDR,
							(unsigned int)(BOOT_LOGO_ADDR + BOOT_LOGO_SIZE));
				customize_data.logo_addr_1 = entry_target_addr;
				customize_data.logo_size_1 = entry_length;
				acpu_setup_boot_av(entry_target_addr);
				printf("Logo");
				break;
			default:
				printf("FW #%d, type:0x%x not support\n", i, entry_type);
				continue;
		}

		printf("\n");

#ifdef PLATFORM_RTD1395
        // at 1395, audio fw belong to trusted fw and need to be loaded by FSBL
        if (entry_type == FW_TYPE_AUDIO || entry_type == FW_TYPE_GOLD_AUDIO) {
            printf("\t FW Image preload on 0x%08x, size=0x%08x (0x%08x)\n",
                    entry_target_addr, entry_length, entry_target_addr + entry_length);
            continue;
        }
#endif

		// FW loading is performed else where(ex:fsbl) under Secure BOOT flow
		// LOGO image is not encryption file, so fsbl will not preload.
		// If define VMX, all FW need not to load.
#ifdef VMX
		if (!LK_FW_LOAD) {
			printf("\t FW Image preload on 0x%08x, size=0x%08x (0x%08x)\n",
				entry_target_addr, entry_length, entry_target_addr + entry_length);
			continue;
		}
#else
		if (!LK_FW_LOAD && entry_type != FW_TYPE_IMAGE_FILE ) {
			printf("\t FW Image preload on 0x%08x, size=0x%08x (0x%08x)\n",
				entry_target_addr, entry_length, entry_target_addr + entry_length);
			continue;
		}
#endif
		else {
			printf("\t FW Image to 0x%08x, size=0x%08x (0x%08x)\n",
				entry_target_addr, entry_length,
				entry_target_addr + entry_length);
			printf("\t FW Image fr 0x%08llx \n", entry_offset);
		}


		if (LK_FW_LOAD && FW_DESC_EXT_VERSION(version)) {
			printf("ERR, Doesn't support load img from Secure-BOOT!!\n");
			return RTK_PLAT_ERR_READ_FW_IMG;
		}

		bdev_addr_size(flash_use_block_op, flash_blk_size,
				entry_offset, entry_length, &bdev_addr, &bdev_size);
		ret = BDEV_READ(flash_use_block_op, system_flash, UINT32_PTR(entry_target_addr),
                                bdev_addr, bdev_size);

		if (ret < (int)bdev_size) {
			printf("[ERR] Read fw error, index = %d\n", i);
			return RTK_PLAT_ERR_READ_FW_IMG;
		}

		arch_clean_cache_range(entry_target_addr, entry_length);

		if (FW_DESC_BASE_VERSION(version) == FW_DESC_TABLE_V1_T_VERSION_1) {
			fw_checksum = get_checksum(UINT32_PTR(entry_target_addr), entry_length);
			if (((fw_desc_entry_v1_t*)this_entry)->checksum != fw_checksum) {
				printf("\t FW Image checksum FAILED\n");
				printf("\t FW Image entry  checksum=0x%08x\n",
					((fw_desc_entry_v1_t*)this_entry)->checksum);
				printf("\t FW Image result checksum=0x%08x\n", fw_checksum);
				return RTK_PLAT_ERR_READ_FW_IMG;
			}
		} else if (FW_DESC_BASE_VERSION(version) == FW_DESC_TABLE_V2_T_VERSION_2) {
#if HW_MCP
			arch_disable_ints();
			MCP_SHA256_hash(UINT32_PTR(entry_target_addr), entry_length, sha_hash, NULL);
			arch_enable_ints();
#else
			SHA256_hash(UINT32_PTR(entry_target_addr), entry_length, sha_hash);
#endif
			if (memcmp(((fw_desc_entry_v2_t*)this_entry)->sha_hash, sha_hash, SHA256_DIGEST_SIZE) != 0) {
				printf("\t FW Image sha FAILED\n");
				printf("\t FW Image entry sha256==>\n");
				for (j = 0 ; j < 32 ; j++)
					printf("%02x ", ((fw_desc_entry_v2_t*)this_entry)->sha_hash[j]);
				printf("\n\t FW Image result sha256==>\n");
				for (j = 0 ; j < 32 ; j++)
					printf("%02x ", sha_hash[j]);
				printf("\n");
				return RTK_PLAT_ERR_READ_FW_IMG;
			}
		}
	}

	return 0;
}
