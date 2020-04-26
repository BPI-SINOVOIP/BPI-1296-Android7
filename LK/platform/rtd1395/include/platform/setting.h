#ifndef __PLATFORM_SETTING_H__
#define __PLATFORM_SETTING_H__

#include <pow2.h>
#include <platform/rbus/iso_reg.h>

#define ARM_ROMCODE_SIZE (124*1024)

#define MIPS_KSEG0BASE		0x80000000
#define MIPS_KSEG1BASE		0xa0000000
#define MIPS_KSEG_MSK		0xe0000000

#define MIPS_EXC_REDIRECT_CODE_ADDR		(0x00000080)
#define MIPS_AUDIO_EXC_VECTOR_ADDR		(0x00000094)
#define MIPS_EXC_DISPATCH_CODE_ADDR		(0x00000100)
#define MIPS_ISR_VIDEO_IMG_ADDR			(0x00001500)
#define MIPS_ROS_BOOT_VECTOR_IMG_ADDR		(0x00001900)
#define MIPS_A_ENTRY_CODE_ADDR			(0x00002000)

#define MIPS_SHARED_MEMORY_ENTRY_ADDR	(0x2F000) // 192K - 0x1000
#define MIPS_UART_REG_BASE_ADDR			(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x000000c4)
#define MIPS_ONE_STEP_INFO_ADDR			(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x000000c8)
#define MIPS_BOOT_AV_INFO_PTR_ADDR		(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x000000cc)
#define MIPS_AUDIO_FLAG_PTR_ADDR		(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x000000d0)

#define MIPS_AUDIO_FW_ENTRY_PTR_ADDR	(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x000000e0)
#define MIPS_SHARED_PRINTK_BUFFER_ADDR	(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x000000e8)  // 0x0000b0e8~0x0000b0ff
#define MIPS_SHARED_PRINTK_BUFFER_SIZE	(0x18)
#define MIPS_BOOT_AV_INFO_ADDR			(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x00000600)
#define VO_RESOLUTION					(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x00000800)  // VO_RESOLUTION_INFO
#define POWER_SAVING					(MIPS_SHARED_MEMORY_ENTRY_ADDR+0x00000900)  // power saving config for fw

#define MIPS_AUDIO_FW_ENTRY_ADDR		(AUDIO_LOADADDR)

#define FW_TABLE_SIZE    0x8000
#define FIRMWARE_DESCRIPTION_TABLE_ADDR                   (0x06400000)
#define EMMC_BLOCK_SIZE				0x200
#define EMMC_BOOTCODE_AREA_SIZE			0x220000

#define SPI_BLOCK_SIZE					0x1

#define EMMC_FLASH	BOOT_EMMC
#define NAND_FLASH	BOOT_NAND
#define SPI_FLASH	BOOT_NOR_SERIAL

//avcpu 
//VO secure dummy 0x98081340~0x9808134C
//only SWC and ACPU can access
#define ACPU_STARTUP_FLAG			(0x98081340)		/* register which represent AVCPU secure startup flag */
#define ACPU_MAGIC1					(0xacd1)


typedef enum {
	BOOT_NOR_SERIAL,
	BOOT_NAND,
	BOOT_EMMC
} BOOT_FLASH_T;

#define BIO_MMC_NAME	"mmc0"
#define BIO_NAND_NAME	"nand0"
#define BIO_SPI_NAME	"spi0"

#define SIZE_TO_BLKS(sz, blk_sz)	(((sz) >> log2_uint(blk_sz)) + !!modpow2((sz), log2_uint(blk_sz)))

#define PLATFORM_REBOOT_ACTION_ADDR	(ISO_NORST_0)

/********* NAND Flash Setting *************/
/*
         ----------------
        |  NF Profile +  |
        |      BBT       | ==> 6 blocks
         ----------------
        | HW Setting x 4 | ==> 4 blocks
         ----------------
        |   Boot-Code    |
        |   (768 KB) x 4 |
        |                | ==> 12 blocks (256 KB ) or 24 blocks (128 KB)
         ----------------
        |    FSBL x 4    | ==> 4 blocks
         ----------------
        |      TEE       |
        |   (768 KB) x 4 |
        |                | ==> 12 blocks (256 KB ) or 24 blocks (128 KB)
         ----------------
        |    BL31 x 4    | ==> 4 blocks
         ----------------
        |   RSA_FW x 4   | ==> 4 blocks
         ----------------
        |   RSA_TEE x 4  | ==> 4 blocks
         ----------------
        |                |
        |    FACTORY     |
        |  4MB (dpeneds) |
        |                |
         ----------------
        |     FW_TBL     |
        |     32 KB      |
         ----------------
*/

#define NAND_NFBBT_BLOCKS               (6)
#define NAND_HWSETTING_BLOCKS           (4)
#define NAND_BOOTCODE_SIZE              (4 * 768 * 1024)
#define NAND_FSBL_BLOCKS                (4)
#define NAND_TEE_SIZE                   (4 * 768 * 1024)
#define NAND_BL31_BLOCKS                (4)
#define NAND_RSAFW_BLOCKS               (4)
#define NAND_RSATEE_BLOCKS              (4)

#define NAND_TOTAL_RESERVE_BLOCKS       \
        (NAND_NFBBT_BLOCKS +            \
         NAND_HWSETTING_BLOCKS +        \
         NAND_FSBL_BLOCKS +             \
         NAND_BL31_BLOCKS +             \
         NAND_RSAFW_BLOCKS +            \
         NAND_RSATEE_BLOCKS)

#define NAND_TOTAL_RESERVE_SIZE         \
        (NAND_BOOTCODE_SIZE +           \
         NAND_TEE_SIZE)

// NAND R/W is performed in PAGE alignment (2K or 4K page)
// Default use 4K
#define NAND_RW_ALIGNMENT               4096

/************* NAND End *******************/

#endif
