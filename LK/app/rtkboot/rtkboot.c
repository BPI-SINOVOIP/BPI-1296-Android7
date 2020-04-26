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
#include <app.h>
#include <debug.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <compiler.h>
#include <lib/console.h>
#include <lib/sysparam.h>
#include <rtkboot.h>
#include <kernel/thread.h>
#include <fw_info.h>
#include <arch/arm64/mmu.h>
#include <platform/emmc.h>
#include <platform/io.h>
#include <platform/setting.h>
#include <platform/gpio.h>
#include <factorylib.h>
#include <power.h>
#include <lib/bio.h>
#include <gpt_parser.h>

#ifdef HDMITX_MODE
extern void set_hdmi_off(void);
#endif
extern int sink_capability_handler(int set);
extern int dptx_sink_capability_handler(int set);


BOOT_MODE boot_mode = BOOT_NORMAL_MODE;
static int boot_from_suspend = 0;
static int boot_from_kernel = 0;

customize_data_t customize_data;

const char *const rtk_suspend_resume_states[RESUME_MAX_STATE] = {
	[RESUME_NONE]		= "none",
	[RESUME_UNKNOW]		= "unknow",
	[RESUME_IR]		= "ir",
	[RESUME_GPIO]		= "gpio",
	[RESUME_LAN]		= "lan",
	[RESUME_ALARM]		= "alarm",
	[RESUME_TIMER]		= "timer",
	[RESUME_CEC]		= "cec",
};

int is_boot_from_suspend(void)
{
	return boot_from_suspend;
}

int is_boot_from_kernel(void)
{
	return boot_from_kernel;
}

static inline int keyboard_detect(void) {
	char c;
	int err;

	err = platform_pgetc(&c, false);
	if (err < 0)
		return err;
	return (unsigned char)c;
}

#ifdef ANDROID_REBOOT_CMD
// BCB block, sync with <android>/bootable/recovery/bootloader.h
struct bootloader_message {
	char command[32];
	char status[32];
	char recovery[768];
	char stage[32];
	char reserved[224];
};

bool misc_BCB_update_success(void)
{
	bdev_t *system_flash;
	int ret;
	void *pt_tbl = NULL;
	struct partition_info misc_info;
	struct bootloader_message *bcb = NULL;
	unsigned int write_block, read_block;
	bool write_misc_success = false;

	system_flash = bio_open(BIO_MMC_NAME);
	if (!system_flash) {
		printf("%s, couldn't open blockdevice:%s\n", __func__, BIO_MMC_NAME);
		goto out;
	}

        if (!partition_gpt_exists()) {
		ret = partition_read_table(system_flash, &pt_tbl);
		if (ret || pt_tbl == NULL) {
			printf("%s, reading partition table failed\n", __func__);
			goto out;
		}
        }

	if (!partition_gpt_exists()) {
		printf("%s, GPT doesn't exist\n", __func__);
		goto out;
	}

	misc_info = partition_get_info(system_flash, "misc");
	if (misc_info.offset == 0 && misc_info.size == 0)
	{
		goto out;
	}

	bcb = malloc(sizeof(struct bootloader_message));
	if (bcb == NULL)
		goto out;

	memset(bcb, 0, sizeof(struct bootloader_message));
	read_block = bio_read_block(system_flash, bcb, (misc_info.offset/system_flash->block_size), sizeof(struct bootloader_message)/system_flash->block_size);
	if (read_block != (sizeof(struct bootloader_message)/system_flash->block_size))
	{
		dprintf(CRITICAL, "Failed to read misc partition\n");
		goto out;
	}

	if (strlen(bcb->command) != 0 && strlen(bcb->recovery) != 0)
		goto out_success;

	memset(bcb->command, 0, sizeof(bcb->command));
	memset(bcb->recovery, 0, sizeof(bcb->recovery));
	snprintf(bcb->command, 32, "boot-recovery");
	snprintf(bcb->recovery, 768, "recovery\n--update_package=BACKUP:update.zip\n--locale=en_GB");
	write_block = bio_write_block(system_flash, bcb, (misc_info.offset/system_flash->block_size), sizeof(struct bootloader_message)/system_flash->block_size);
        if (write_block != (sizeof(struct bootloader_message)/system_flash->block_size))
	{
		dprintf(CRITICAL, "Failed to write misc partition\n");
		goto out;
	}

out_success:
	write_misc_success = true;
out:
	if (bcb != NULL)
		free(bcb);
	if (system_flash)
		bio_close(system_flash);
	if (pt_tbl)
		free(pt_tbl);

	return write_misc_success;
}

#define FACTORY_BCB_PATH	FACTORY_HEADER_FILE_NAME"recovery"
#ifdef RTK_BCB_IN_FACTORY
void factory_BCB_update(void)
{
	struct bootloader_message *bcb = NULL;
	char *fac_buf = NULL;
	int fac_len = 0;

	if (!factory_read(FACTORY_BCB_PATH, &fac_buf, &fac_len)) {
		printf("Factory BCB deteted, skip update\n");
		return;
	}

	bcb = malloc(sizeof(struct bootloader_message));
	memset(bcb, 0, sizeof(struct bootloader_message));
	snprintf(bcb->command, 32, "boot-recovery");
	snprintf(bcb->recovery, 768, "recovery\n--update_package=BACKUP:update.zip\n--locale=en_GB");

	if (factory_write(FACTORY_BCB_PATH, (char*)bcb, sizeof(struct bootloader_message))) {
		printf("!!FAIL to update Factory BCB!!\n");
		free(bcb);
		return;
	}

	if (factory_save())
		printf("!!Fail to SAVE BCB to Factory!!\n");

	free(bcb);
}
#else
void factory_BCB_update(void) {};
#endif

static int check_reboot_action(void)
{
	unsigned int reboot_action;
	char *fac_buf = NULL;
	int fac_len = 0;

	reboot_action = rtd_inl(REBOOT_ACTION_ADDR);

	if (!factory_read(FACTORY_BCB_PATH, &fac_buf, &fac_len)) {
                printf("Legacy rescue-mode flow\n");
                reboot_action = RESET_ACTION_RECOVERY;
                boot_from_kernel = 1;
//                return 0;
	} else if (REBOOT_ACTION_VALID(reboot_action)) {
		printf("*** Reboot-Action : 0x%08x ***\n", reboot_action);
		boot_from_kernel = 1;
	} else if (ACPU_BOOT_REASON_VALID(reboot_action)) {
		printf("=== Boot from suspend : %s ===\n", rtk_suspend_resume_states[ACPU_BOOT_REASON(reboot_action)]);
		boot_from_suspend = 1;
		return 0;
	} else {
		return 0;
	}

	switch (reboot_action & REBOOT_ACTION_MASK) {
		case RESET_ACTION_FASTBOOT:
			printf("REBOOT_ACTION : ENTER fastboot mode\n");
			boot_mode = BOOT_FASTBOOT_MODE;
			break;
		case RESET_ACTION_RECOVERY:
			printf("REBOOT_ACTION : ENTER recovery mode\n");
			boot_mode = BOOT_RECOVERY_MODE;
			if (misc_BCB_update_success() == false)
				factory_BCB_update();
			break;
		case RESET_ACTION_GOLDEN:
			printf("REBOOT_ACTION : ENTER GOLDEN mode\n");
			boot_mode = BOOT_GOLD_MODE;
			break;
		case RESET_ACTION_RESCUE:
			printf("REBOOT_ACTION : ENTER rescue mode\n");
			boot_mode = BOOT_RESCUE_MODE;
			break;
		case RESET_ACTION_NO_ACTION:
			printf("REBOOT_ACTION : No Action\n");
			break;
		default:
			break;
	}

	rtd_outl(REBOOT_ACTION_ADDR, REBOOT_MAGIC);
	return 0;
}
#endif //ANDROID_REBOOT_CMD

void rtk_boot_ru(void)
{
#ifdef SUPPORT_BOOT_RU_CMD
	printf(VT100_YELLOW"Booting from USB....\n"VT100_NONE);
	console_run_script_locked("usb start\n");
	console_run_script_locked("fatload usb 0:1 0x02100000 rescue.emmc.dtb\n");
	console_run_script_locked("fatload usb 0:1 0x02200000 rescue.root.emmc.cpio.gz_pad.img\n");
	console_run_script_locked("fatload usb 0:1 0x03000000 emmc.uImage\n");
	console_run_script_locked("fatload usb 0:1 0x0f900000 bluecore.audio\n");
	acpu_boot();
	rtk_boot_kernel();
#else
	printf(VT100_LIGHT_RED"Booting into USB-RESCUE mode, Not supported yet\n"VT100_NONE);
#endif
}

int rtk_boot(int argc, const cmd_args *argv)
{
	if (argc != 2) {
		printf("usage:\n");
		printf("\t%s a\n", argv[0].str);
		printf("\t%s k\n", argv[0].str);
		printf("\t%s all\n", argv[0].str);
		printf("\t%s r\n", argv[0].str);
		printf("\t%s ru\n", argv[0].str);
		printf("\t%s m\n", argv[0].str);
		return -1;
	}

	if (strcmp(argv[1].str, "a") == 0) {
		acpu_boot();
	} else if (strcmp(argv[1].str, "k") == 0) {
		rtk_boot_kernel();
	} else if (strcmp(argv[1].str, "all") == 0) {
		acpu_boot();
		rtk_boot_kernel();
	} else if (strcmp(argv[1].str, "r") == 0) {
		boot_mode = BOOT_RESCUE_MODE;
		rtk_plat_prepare_fw_image();
		acpu_boot();
		rtk_boot_kernel();
	} else if (strcmp(argv[1].str, "ru") == 0) {
		rtk_boot_ru();
	} else if (strcmp(argv[1].str, "m") == 0) {
		boot_mode = BOOT_MANUAL_MODE;
		rtk_plat_prepare_fw_image();
	} else {
		printf("argument %s NOT support\n", argv[1].str);
		return -1;
	}

	return 0;
}

__WEAK int check_customized_boot_mode(void)
{
	return 0;
}

__WEAK void partition_adjust(void)
{
	BOOT_FLASH_T boot_flash_type = FLASH_TYPE;
	bdev_t *system_flash;
	void *pt_tbl = NULL;
	int pt_num = 0, idx = 0;
	int ret;
	struct partition_info info;
	uint64_t last_usable_lba = 0;
	int secondary_gpt_blocks = 0;
	uint64_t data_end_lba = 0;

	if (FLASH_TYPE == EMMC_FLASH) {
		system_flash = bio_open(BIO_MMC_NAME);
		if (!system_flash) {
			printf("%s, couldn't open blockdevice:%s\n", __func__, BIO_MMC_NAME);
			goto out;
		}

		ret = partition_read_table(system_flash, &pt_tbl);
		if (ret || pt_tbl == NULL) {
			printf("%s, reading partition table failed\n", __func__);
			goto out;
		}

		if (!partition_gpt_exists()) {
			printf("%s, GPT doesn't exist\n", __func__);
			goto out;
		}

		pt_num = partition_get_num();
		idx = partition_get_index("data");
		info = partition_get_info(system_flash, "data");
		/* Header + partition array body */
		secondary_gpt_blocks = (ENTRY_SIZE * NUM_PARTITIONS) / system_flash->block_size + 1;
		last_usable_lba = (system_flash->total_size / system_flash->block_size) - (1 + secondary_gpt_blocks);

		if (idx != pt_num - 1) {
			printf("%s, data partition not at end, skip\n", __func__);
			goto out;
		}

		data_end_lba = (info.offset + info.size) / system_flash->block_size - 1;

		if (data_end_lba != last_usable_lba) {
			printf("%s, last_usable_lba:%llu, data_end_lba:%llu\n", __func__, last_usable_lba, data_end_lba);
			write_partition(system_flash, 2 * (GPT_HEADER_SIZE + ENTRY_SIZE * NUM_PARTITIONS), pt_tbl);
		}
	} else {
		system_flash = NULL;
		printf("%s, doesn't support flash type %d\n", __func__, boot_flash_type);
	}
out:
	if (system_flash)
		bio_close(system_flash);

	if (pt_tbl)
		free(pt_tbl);
}

static void rtk_boot_prepare(const struct app_descriptor *app)
{
	int console_key = keyboard_detect();
	int ret;

	console_init();
	memset(&customize_data, 0, sizeof(customize_data_t));

	acpu_bootcode_prepare();
	power_saving_init();
	set_boot_flash_type();

#ifdef ANDROID_REBOOT_CMD
	check_reboot_action();
#endif //ANDROID_REBOOT_CMD

	#ifdef FT_TOOL
	printf("**** ENTER FT ***\n");
	boot_mode = BOOT_FT_MODE;
	#endif //FT_TOOL

	/* Decide Boot Mode */
	switch (console_key) {
		case _TAB:
			printf("\n*** TAB Pressed ***\n\n");
			boot_mode = BOOT_RESCUE_MODE;
			break;
		case _ENTER:
			printf("\n*** Enter Pressed ***\n\n");
			break;
		case _ESC:
			printf("\n*** ESC Pressed ***\n\n");
			boot_mode = BOOT_CONSOLE_MODE;
			break;
		case _SPACE:
			printf("\n*** SPACE Pressed ***\n\n");
			boot_mode = BOOT_MANUAL_MODE;
			break;
		case 'f':
			printf("**** ENTER fastboot ***\n");
			boot_mode = BOOT_FASTBOOT_MODE;
			break;
	}

#ifdef INSTALL_BUTTON_GPIO_NUM
#if PLATFORM_RTD1295
	if (!getGPIO(INSTALL_BUTTON_GPIO_NUM)) {
		printf("\nPress Install Button\n");
		boot_mode = BOOT_RESCUE_MODE;
	}
#elif PLATFORM_RTD1395
	setISOGPIO_pullsel(INSTALL_BUTTON_GPIO_NUM, PULL_UP);
	if (!getISOGPIO(INSTALL_BUTTON_GPIO_NUM)) {
		printf("\nPress Install Button\n");
		boot_mode = BOOT_RESCUE_MODE;
	}
#endif
#endif //INSTALL_BUTTON_GPIO_NUM

	/* Check if any customize boot-flow involved */
	if (boot_mode == BOOT_NORMAL_MODE) {
		ret = check_customized_boot_mode();
		if (ret) {
			printf("!!! Customize boot-flow fail, go into console...\n");
			boot_mode = BOOT_CONSOLE_MODE;
		}
	}

	partition_adjust();

#ifdef HDMITX_MODE
	if(HDMITX_MODE==0)/* HDMI TX always off */
		set_hdmi_off();
	else
		sink_capability_handler(1);
#endif
#ifdef DPTX_MODE
	if(DPTX_MODE==1)
		dptx_sink_capability_handler(1);
#endif

}

#ifdef FT_TOOL
extern void ft_test_0(void);
#endif //FT_TOOL

static void rtk_boot_system(const struct app_descriptor *app, void *args)
{
	int ret = RTK_PLAT_ERR_OK;
	//if (boot_mode == BOOT_NORMAL_MODE)
	//	console_run_script_locked("boot all\n");
	switch (boot_mode) {
		case BOOT_RESCUE_MODE:
		case BOOT_RECOVERY_MODE:
		case BOOT_NORMAL_MODE:
		case BOOT_GOLD_MODE:
#if defined(SUPPORT_BOOT_RU_CMD) && defined(FORCE_BOOT_FROM_USB)
			if(boot_mode==BOOT_NORMAL_MODE) {
				rtk_boot_ru();
				// go to console if boot from usb failed
				ret = RTK_PLAT_ERR_OK;
				boot_mode = BOOT_CONSOLE_MODE;
				break;
			}
#endif
			ret = rtk_plat_prepare_fw_image();
			if (ret == RTK_PLAT_ERR_OK) {
				acpu_boot();
				ret = rtk_boot_kernel();
			}
			break;
		case BOOT_MANUAL_MODE:
			ret = rtk_plat_prepare_fw_image();
			break;
		case BOOT_KERNEL_ONLY_MODE:
			ret = rtk_plat_prepare_fw_image();
			if (ret == RTK_PLAT_ERR_OK)
				ret = rtk_boot_kernel();
			break;
		case BOOT_CONSOLE_MODE:
			printf("Enter Console...\n");
			break;
		case BOOT_FASTBOOT_MODE:
			printf("Enter fastboot...\n");
			break;
		case BOOT_S5_MODE:
			printf("Enter S5 standby...\n");
			ret = rtk_plat_prepare_fw_image();
			if (ret == RTK_PLAT_ERR_OK) {
				power_down_soc_S5();
				acpu_boot();
			}
			break;
		#ifdef FT_TOOL
		case BOOT_FT_MODE:
			printf("Enter Final Test...\n");
			ft_test_0();
			break;
		#endif //FT_TOOL
		default:
			printf("!!! BootMode 0x%x not support\n", boot_mode);
			break;
	}

	/* Enter Rescue Mode if normal boot flow get error */
	if (ret != RTK_PLAT_ERR_OK && boot_mode != BOOT_RESCUE_MODE) {
		boot_mode = BOOT_RESCUE_MODE;
		ret = rtk_plat_prepare_fw_image();

		if (ret == RTK_PLAT_ERR_OK) {
			acpu_boot();
			rtk_boot_kernel();
		}
		// Now, try Golden FW if rescue is corrupt
		boot_mode = BOOT_GOLD_MODE;
		ret = rtk_plat_prepare_fw_image();

		if (ret == RTK_PLAT_ERR_OK) {
			acpu_boot();
			rtk_boot_kernel();
		}
	}

	if (boot_mode == BOOT_FASTBOOT_MODE) {
		console_run_script_locked("fastboot\n");
	} else
	// Finally get to console if boot not performed
	console_start();
}

STATIC_COMMAND_START
STATIC_COMMAND("boot", "platform boot command", (console_cmd)&rtk_boot)
STATIC_COMMAND_END(rtkboot);

APP_START(rtkboot)
	.init = rtk_boot_prepare,
	.entry = rtk_boot_system,
	.flags = 0,
APP_END

