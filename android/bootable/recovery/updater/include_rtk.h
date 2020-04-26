#ifndef _UPDATER_UPDATER_RTK_INCLUDE_H_
#define _UPDATER_UPDATER_RTK_INCLUDE_H_
#include "updater_rtk_bootable.h"
#include "updater_rtk_common.h"
#include "updater_rtk_fwdesc.h"
#include "updater_rtk_imgdesc.h"
#include "updater_rtk_mmcutils.h"


//#define FWDES_DUMP
//rtk_def.h
#define SYSTEM_PARAMETERS                         "/sys/realtek_boards/system_parameters"
#define PKG_TEMP                                  "/tmp"
#define FWDESCTABLE                               "fw_tbl.bin"

#define FLASHERASE_BIN                            "/sbin/flash_erase"
#define NANDWRITE_BIN                             "/sbin/nandwrite"
#define UBIFORMAT_BIN                             "/sbin/ubiformat"

#define BOOTCODE_TEMP                             PKG_TEMP"/bootloader"
#define FACTORY_TEMP                              PKG_TEMP"/factory"
#define FACTORY_INSTALL_TEMP                      PKG_TEMP"/install_factory"

#define RTK_NAND_HWSETTING_START_BLOCK            6
#define NAND_BOOT_BACKUP_COUNT                    4 // number of backup bootcode in NAND flash
//file in bootcode.tar
#define NOR_HWSETTING_START_ADDR                  0x3000
#define RESETROM_FILENAME                         "resetrom-new.bin"
#define HWSETTING_FILENAME                        "hw_setting.bin"
#define HASHTARGET_FILENAME                       "hash_target.bin"
#define NAND_RESCUE_FILENAME                      "rescue.bin"
#define RESCUE_FILENAME                           "vmlinux.rescue.macarthur.nonet.bin.lzma"
#define FIND_RESCUE_FILENAME                      "vmlinux.rescue"
#define UBOOT_FILENAME                            "uboot.bin"

#define MEMGETINFO       _IOR('M', 1, struct mtd_info_user)
#define MEMUNLOCK	 _IOW('M', 6, struct erase_info_user)


#define GETRBAPERCENTAGE  	_IOR('M', 34, unsigned int)

#ifdef RTD_298x
#define ECPU_OFFSET	0x1c000
#define ECPU_SIZE_CONSTRAIN_BYTE	16384
#define DEFAULT_RESCUE_START 0x100000			// @298x; but claire think this is not good enough. This should be more generic ( if sys_param.h not specify the addr, maybe burn continuously is better)
#endif

#endif

