
#include <stdlib.h>
#include <kernel/thread.h>
#include <lib/sysparam.h>
#include <lib/console.h>
#include <sys/types.h>
#include <string.h>
#include <debug.h>
#include <trace.h>

#include <platform/setting.h>
#include <platform.h>
#include <rtkboot.h>
#include <lib/bio.h>
#include <lib/bootimg.h>
#include <lib/fastboot.h>

#if WITH_EXTERNAL_LIB_RTK_STORAGE_LAYOUT
#include <lib/rtk_storage_layout.h>
#endif

#define LOCAL_TRACE CRITICAL /* level: CRITICAL, ALWAYS, INFO, SPEW*/

#define PAGE_MASK 2047

#define ROUND_TO_PAGE(x) (((x) + PAGE_MASK) & (~PAGE_MASK))

#define KERNEL_MAX_SIZE 0x1400000

static bool set_flash_bootcode = false;
static bool set_manual_boot = false;

enum image_type {
	UNKNOW_IMAGE = 0,
	KERNEL_IMAGE = 1,
	DTB_IMAGE,
	ROOTFS_IMAGE,
	AUDIO_IMAGE,
};

static enum image_type load_image_type = UNKNOW_IMAGE;

struct Kernel_Img_Header {
	uint32_t	instruction0;
	uint32_t	instruction1;
	uint64_t	image_offset;
	uint64_t	image_size;
	uint64_t	ignr1;
	uint64_t	ignr2;
	uint64_t	ignr3;
	uint64_t	ignr4;
	uint32_t	magic;
	uint32_t	ignr5;
};

static unsigned hex2unsigned(const char *x)
{
    unsigned n = 0;

    while(*x) {
        switch(*x) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            n = (n << 4) | (*x - '0');
            break;
        case 'a': case 'b': case 'c':
        case 'd': case 'e': case 'f':
            n = (n << 4) | (*x - 'a' + 10);
            break;
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
            n = (n << 4) | (*x - 'A' + 10);
            break;
        default:
            return n;
        }
        x++;
    }

    return n;
}

static int load_kernel_image(void *addr, unsigned int size)
{
	addr_t img_addr;
	int param_size = 0;
	char buff[ADDR_BUF_LEN] = "";

	if ((param_size = sysparam_length("kernel_loadaddr")) < 0) {
		dprintf(CRITICAL, "%s, kernel_loadaddr not found\n", __func__);
		return -1;
	}

	if (sysparam_read("kernel_loadaddr", buff, param_size) < 0) {
		dprintf(CRITICAL, "%s, kernel_loadaddr read error\n", __func__);
		return -1;
	}

	img_addr = atoull(buff);

	dprintf(INFO, "kernel_loadaddr: %p, from addr: %p, size: %d\n",
		    (void *) img_addr, addr, size);

	memmove((void*)img_addr, (void*)addr, size);
	arch_clean_cache_range(img_addr, size);

	return 0;
}

static int load_fdt_image(void *addr, unsigned int size)
{
	addr_t img_addr;
	int param_size = 0;
	char buff[ADDR_BUF_LEN] = "";

	if ((param_size = sysparam_length("fdt_loadaddr")) < 0) {
		dprintf(CRITICAL, "%s, fdt_loadaddr not found\n", __func__);
		return -1;
	}

	if (sysparam_read("fdt_loadaddr", buff, param_size) < 0) {
		dprintf(CRITICAL, "%s, fdt_loadaddr read error\n", __func__);
		return -1;
	}

	img_addr = atoull(buff);
	dprintf(INFO, "fdt_loadaddr: %p, from addr: %p, size: %d\n",
		    (void *) img_addr, addr, size);

	memmove((void*)img_addr, (void*)addr, size);
	arch_clean_cache_range(img_addr, size);

	return 0;
}

static int load_rootfs_image(void *addr, unsigned int size)
{
	addr_t img_addr;
	int param_size = 0;
	char buff[ADDR_BUF_LEN] = "";

	if ((param_size = sysparam_length("rootfs_loadaddr")) < 0) {
		dprintf(CRITICAL, "%s, rootfs_loadaddr not found\n", __func__);
		return -1;
	}

	if (sysparam_read("rootfs_loadaddr", buff, param_size) < 0) {
		dprintf(CRITICAL, "%s, rootfs_loadaddr read error\n", __func__);
		return -1;
	}

	img_addr = atoull(buff);
	dprintf(INFO, "rootfs_loadaddr: %p, from addr: %p, size: %d\n",
		    (void *) img_addr, addr, size);

	memmove((void*)img_addr, (void*)addr, size);
	arch_clean_cache_range(img_addr, size);

	return 0;
}

static int load_audio_image(void *addr, unsigned int size)
{
	addr_t img_addr;
	int param_size = 0;
	char buff[ADDR_BUF_LEN] = "";

	if ((param_size = sysparam_length("audio_loadaddr")) < 0) {
		dprintf(CRITICAL, "%s, audio_loadaddr not found\n", __func__);
		return -1;
	}

	if (sysparam_read("audio_loadaddr", buff, param_size) < 0) {
		dprintf(CRITICAL, "%s, audio_loadaddr read error\n", __func__);
		return -1;
	}

	img_addr = atoull(buff);
	dprintf(INFO, "audio_loadaddr: %p, from addr: %p, size: %d\n",
		    (void *) img_addr, addr, size);

	memmove((void*)img_addr, (void*)addr, size);
	arch_clean_cache_range(img_addr, size);

	return 0;
}

static int load_bootcode_image(void *addr, unsigned int size) {
	addr_t img_addr = 0x1500000;
	dprintf(INFO, "%s, load bootcode to %p, from addr: %p, size: %d\n",
		    __func__, (void *) img_addr, addr, size);
	memmove((void*)img_addr, (void*)addr, size);
	arch_clean_cache_range(img_addr, size);
	return 0;
}

static int flash_bootcode(void) {
	dprintf(INFO, "%s, flash bootcode from 0x1500000\n", __func__);
	console_run_script_locked("chain 0x1500000\n");
	return 0;
}

static void cmd_boot(const char *arg, void *data, unsigned sz)
{
	unsigned kernel_actual;
	unsigned ramdisk_actual;
	unsigned second_actual;
	static struct boot_img_hdr hdr;
	struct Kernel_Img_Header *kernel_img_header = NULL;
	char *ptr = ((char*) data);

	dprintf(INFO, "fastboot: cmd_boot\n");

	if (sz < sizeof(hdr)) {
		fastboot_fail("invalid bootimage header");
		return;
	}

	memcpy(&hdr, data, sizeof(hdr));

	/* ensure commandline is terminated */
	hdr.cmdline[BOOT_ARGS_SIZE-1] = 0;

	kernel_actual = ROUND_TO_PAGE(hdr.kernel_size);
	ramdisk_actual = ROUND_TO_PAGE(hdr.ramdisk_size);
	second_actual = ROUND_TO_PAGE(hdr.second_size);

	if (2048 + kernel_actual + ramdisk_actual + second_actual < sz) {
		fastboot_fail("incomplete bootimage");
		return;
	}

	kernel_img_header = (struct Kernel_Img_Header*)(ptr + 2048);

	dprintf(INFO, "Bootimg header magic %s kernel_size %d ramdisk_size %d "
		    "second_size %d\n",
		    hdr.magic, hdr.kernel_size, hdr.ramdisk_size, hdr.second_size);

	if (set_flash_bootcode && hdr.kernel_size > 0) {
		load_bootcode_image((void*) (ptr + 2048), hdr.kernel_size);
		fastboot_info("To flash bootcode");
	} else if (set_manual_boot && hdr.kernel_size > 0) {
		switch (load_image_type) {
		case KERNEL_IMAGE:
			load_kernel_image((void*) (ptr + 2048), hdr.kernel_size);
			fastboot_info("Load Kernel Image, "
				    "use \"fastboot continue\" to boot");
			break;
		case DTB_IMAGE:
			load_fdt_image((void*) (ptr + 2048), hdr.kernel_size);
			fastboot_info("Load dtb Image, "
				    "use \"fastboot continue\" to boot");
			break;
		case ROOTFS_IMAGE:
			load_rootfs_image((void*) (ptr + 2048), hdr.kernel_size);
			fastboot_info("Load rootfs Image, "
				    "use \"fastboot continue\" to boot");
			break;
		case AUDIO_IMAGE:
			load_audio_image((void*) (ptr + 2048), hdr.kernel_size);
			fastboot_info("Load Audio Image, "
				    "use \"fastboot continue\" to boot");
			break;
		default:
			fastboot_info("UNKNOW_IMAGE");
			break;
		}
	} else {
		if (hdr.kernel_size > 0)
			load_kernel_image((void*) (ptr + 2048),
				    kernel_img_header->image_size);
		if (hdr.kernel_size > KERNEL_MAX_SIZE)
			load_fdt_image((void*) (ptr + 2048 + KERNEL_MAX_SIZE),
				    hdr.kernel_size - KERNEL_MAX_SIZE);
		if (hdr.ramdisk_size > 0)
			load_rootfs_image((void*) (ptr + 2048 + kernel_actual),
				    hdr.ramdisk_size);
		if (hdr.second_size > 0)
			load_audio_image((void *) (ptr + 2048 + kernel_actual +
				    ramdisk_actual), hdr.second_size);
		fastboot_info("To load bootimg");
	}

	fastboot_okay("");

	*(volatile unsigned int*)(0x98015934) = 0x0;

	if (set_flash_bootcode) {
		fastboot_usb_stop();
		flash_bootcode();
	} else if (set_manual_boot) {
		// Do nothing
	} else {
		fastboot_usb_stop();

		thread_sleep(250);
		if (sysparam_length("audio_loadaddr") > 0)
			acpu_boot();
		rtk_boot_kernel();
	}
}

static void cmd_continue(const char *arg, void *data, unsigned sz)
{
	if (boot_mode == BOOT_NORMAL_MODE) {
		fastboot_info("boot normal images");
	} if (boot_mode == BOOT_RESCUE_MODE) {
		fastboot_info("boot resuce images");
	}

	fastboot_okay("");
	fastboot_usb_stop();

	thread_sleep(250);
	if (sysparam_length("audio_loadaddr") > 0)
		acpu_boot();
	rtk_boot_kernel();
}

static void cmd_erase(const char *arg, void *data, unsigned sz)
{
	fastboot_fail("No support erase command");
}

extern int parse_sparse_image(const char *arg, void *data, size_t sz,
		int (*flash_image)(const char *arg, uint64_t file_size, uint64_t offset,
			void *data, unsigned size));
extern int process_tar_image(void *data, size_t sz,
		int (*flash_image)(const char *arg, uint64_t file_size, uint64_t offset,
			void *data, unsigned size));

int flash_image(const char *arg, uint64_t file_size, uint64_t offset,
		void *data, unsigned sz)
{
	int ret = 0;
	static uint32_t type = ORIGIN;

	LTRACEF("To flash:%s offset 0x%llx/0x%llx (data@%p size %x)\n",
			arg, offset, file_size, data, sz);

	if (!strcmp(arg, "img")) {
		/* process install.img */
		LTRACEF("To flash install.img\n");
		type = BUILD_MBR;
		process_tar_image(data, sz, flash_image);

		return 0;
	}
	if (!strcmp(arg, "gimg")) {
		LTRACEF("To flash gpt install.img\n");
		type = BUILD_GPT;
		process_tar_image(data, sz, flash_image);

		return 0;
	}

	if (data == NULL) {
		LTRACEF("NO data to flash:%s offset 0x%llx (data@%p size %x)\n",
			arg, offset, data, sz);
		return 0;
	}

#if WITH_EXTERNAL_LIB_RTK_STORAGE_LAYOUT
	ret = write_to_storage(arg, type, file_size, offset, data, sz);
#endif

	return ret;
}

static void cmd_flash(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_flash\n");

	parse_sparse_image(arg, data, sz, flash_image);

	fastboot_okay("");
}

static void cmd_reboot(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_reboot\n");
	fastboot_info("rebooting the device\n");
	fastboot_okay("");

	thread_sleep(250);
	platform_halt(HALT_ACTION_REBOOT, HALT_REASON_SW_RESET);
}

static void cmd_reboot_bootloader(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_reboot_bootloader\n");
	fastboot_info("rebooting the device to fastboot mode\n");
	fastboot_okay("");

	writel(REBOOT_MAGIC | RESET_ACTION_FASTBOOT, REBOOT_ACTION_ADDR);
	writel(0x0, 0x980076C4);

	thread_sleep(250);
	platform_halt(HALT_ACTION_REBOOT, HALT_REASON_SW_RESET);
}

static void cmd_load_default_images(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_load_default_images\n");
	set_manual_boot = true;
	boot_mode = BOOT_NORMAL_MODE;
	rtk_plat_prepare_fw_image();

	fastboot_info("load_default_images");

	fastboot_okay("");
}

static void cmd_load_rescue_images(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_load_rescue_images\n");
	set_manual_boot = true;
	boot_mode = BOOT_RESCUE_MODE;
	rtk_plat_prepare_fw_image();

	fastboot_info("load_rescue_images");

	fastboot_okay("");
}

static void cmd_set_load_kernel(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_set_load_kernel\n");
	set_manual_boot = true;
	load_image_type = KERNEL_IMAGE;
	fastboot_info("boot command set to load kernel");
	fastboot_info("run \"fastboot boot kernel\"");

	fastboot_okay("");
}

static void cmd_set_load_dtb(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_set_load_dtb\n");
	set_manual_boot = true;
	load_image_type = DTB_IMAGE;
	fastboot_info("boot command set to load dtb");
	fastboot_info("run \"fastboot boot dtb\"");

	fastboot_okay("");
}

static void cmd_set_load_rootfs(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_set_load_rootfs\n");
	set_manual_boot = true;
	load_image_type = ROOTFS_IMAGE;
	fastboot_info("boot command set to load rootfs");
	fastboot_info("run \"fastboot boot rootfs\"");

	fastboot_okay("");
}

static void cmd_set_load_audio(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_set_load_audio\n");
	set_manual_boot = true;
	load_image_type = AUDIO_IMAGE;
	fastboot_info("boot command set to load audio");
	fastboot_info("run \"fastboot boot audio\"");

	fastboot_okay("");
}

static void cmd_set_flash_bootcode(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_set_flash_bootcode\n");
	boot_mode = BOOT_FASTBOOT_MODE;
	set_flash_bootcode = true;

	fastboot_info("boot command set to flash bootcode");
	fastboot_info("run \"fastboot boot dvrboot.exe.bin\"");
	fastboot_okay("");
}

static void cmd_go_a(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_go_a\n");
	fastboot_info("Boot audio FW");

	fastboot_okay("");

	fastboot_usb_stop();

	thread_sleep(250);
	if (sysparam_length("audio_loadaddr") > 0)
		acpu_boot();
}

static void cmd_go_k(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_go_k\n");
	fastboot_info("Boot kernel only");

	fastboot_okay("");
	fastboot_usb_stop();

	thread_sleep(250);
	rtk_boot_kernel();
}

static void cmd_go_all(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_go_all\n");
	fastboot_info("Boot kernel and audio");

	fastboot_okay("");

	fastboot_usb_stop();

	thread_sleep(250);
	if (sysparam_length("audio_loadaddr") > 0)
		acpu_boot();
	rtk_boot_kernel();
}

static void cmd_get_emmc_layout(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_get_emmc_layout\n");
	size_t count = 0;
	size_t buf_sz = 1024*20;
	char *buf = malloc(buf_sz);
	char *line = NULL;

	count = dump_fw_layout(buf, buf_sz);

	dprintf(INFO, "fastboot: count = %d\n", (int)count);

	line = strtok(buf, "\n");

	while (line != NULL) {
		fastboot_info(line);
		line = strtok(NULL, "\n");
	}

	free(buf);
	fastboot_okay("");
}

static void cmd_get_part_info(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_get_part_info\n");
	size_t count = 0;
	size_t buf_sz = 1024*20;
	char *buf = malloc(buf_sz);
	char *line = NULL;

	count = dump_part_info(buf, buf_sz);

	dprintf(INFO, "fastboot: count = %d\n", (int)count);

	line = strtok(buf, "\n");

	while (line != NULL) {
		fastboot_info(line);
		line = strtok(NULL, "\n");
	}

	free(buf);
	fastboot_okay("");
}


static void cmd_get_fw_info(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_get_fw_info\n");
	size_t count = 0;
	size_t buf_sz = 1024*20;
	char *buf = malloc(buf_sz);
	char *line = NULL;

	count = dump_fw_info(buf, buf_sz);

	dprintf(INFO, "fastboot: count = %d\n", (int)count);

	line = strtok(buf, "\n");

	while (line != NULL) {
		fastboot_info(line);
		line = strtok(NULL, "\n");
	}

	free(buf);
	fastboot_okay("");
}

size_t dump_help(void *buf, unsigned buf_sz)
{
	size_t count = 0;

	count += snprintf(buf + count, buf_sz - count,
		    "List RTK LK fastboot command usage:\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Update LK by fastboot:\n"
		    "    fastboot oem set_flash_bootcode or set_flash_lk\n"
			"    fastboot boot dvrboot.ext.bin\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Load Normal(default)/resuce Images\n"
		    "    fastboot oem load_default_images\n"
		    "    fastboot oem load_rescue_images\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Use boot command to boot resuce.boot.img\n"
		    "    fastboot boot resuce.boot.img\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Use boot command to load Kernel/dtb/rootfs/audio_fw image\n"
		    "    fastboot oem load_default_images\n"
		    "    fastboot oem set_load_kernel\n"
		    "    fastboot boot emmc.uImage\n"
		    "    fastboot oem set_load_dtb\n"
		    "    fastboot boot android.emmc.dtb\n"
		    "    fastboot oem set_load_rootfs\n"
		    "    fastboot boot android.root.emmc.cpio.gz_pad.img\n"
		    "    fastboot oem set_load_audio\n"
		    "    fastboot boot bluecore.audio\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Command continue to boot\n"
		    "    fastboot.exe continue\n"
		    "    fastboot oem go_k\n"
		    "    fastboot oem go_a\n"
		    "    fastboot oem go_all\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Command to reboot\n"
		    "    fastboot reboot-bootloader\n"
		    "    fastboot reboot\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Command for debug\n"
		    "    fastboot oem get_emmc_layout\n"
		    "    fastboot oem get_part_info\n"
		    "    fastboot oem get_fw_info\n");
	count += snprintf(buf + count, buf_sz - count,
		    "Command usage for flash:\n"
		    "    fastboot flash img install.img\n"
		    "    fastboot flash gimg install.img\n"
		    "    fastboot flash kernelDT android.emmc.dtb\n"
		    "    fastboot flash rescueDT rescue.emmc.dtb\n"
		    "    fastboot flash rescueRootFS rescue.root.emmc.cpio.gz_pad.img\n"
		    "    fastboot flash kernelRootFS "
		    "android.root.emmc.cpio.gz_pad.img\n"
		    "    fastboot flash linuxKernel emmc.uImage\n"
		    "    fastboot flash audioKernel bluecore.audio\n"
		    "    fastboot flash imageFile bootfile.image\n"
		    "    fastboot flash system system.bin\n"
		    "    fastboot flash data data.bin\n"
		    "    fastboot flash cache cache.bin\n"
		    "    fastboot flash vendor vendor.bin\n");
	return count;
}

static void cmd_help(const char *arg, void *data, unsigned sz)
{
	dprintf(INFO, "fastboot: cmd_help\n");
	size_t count = 0;
	size_t buf_sz = 1024*20;
	char *buf = malloc(buf_sz);
	char *line = NULL;

	count = dump_help(buf, buf_sz);

	dprintf(INFO, "fastboot: count = %d (buf_sz=%d)\n",
		    (int)count, (int)buf_sz);

	line = strtok(buf, "\n");

	while (line != NULL) {
		fastboot_info(line);
		line = strtok(NULL, "\n");
	}

	free(buf);
	fastboot_okay("");
}

#if 0
static void cmd_get_boot_mode(const char *arg, void *data, unsigned sz)
{
	char temp_buf[32];
	dprintf(INFO,"fastboot: cmd_get_boot_mode\n");
	dprintf(INFO, "boot mode %d\n", boot_mode);
	sprintf(temp_buf, "boot mode %d", boot_mode);
	fastboot_info(temp_buf);
	dprintf(INFO,"fastboot: response fastboot_okay\n");
	fastboot_okay("");
}

static void cmd_set_boot_mode(const char *arg, void *data, unsigned sz)
{
	unsigned value = -1;
	char *_arg = (char*)arg;
	dprintf(INFO,"fastboot: cmd_set_boot_mode\n");
	if (arg && strcmp(_arg, "") != 0) {
		const char *delim = " ";
		char *pch;

		pch = strtok(_arg, delim);
		if (!pch) {
			fastboot_fail("\tset boot mode error");
			return;
		}
		dprintf(INFO,"fastboot: cmd_set_boot_mode ==> %s\n", pch);
		value = hex2unsigned(pch);
	}
	if (value > 0) {
		dprintf(INFO, "set boot mode %d\n", boot_mode);
		boot_mode = value;
		fastboot_okay("");
	} else {
		fastboot_fail("\tset boot mode error");
	}
}
#endif

int fastboot_cmd_init(void) {

	fastboot_register("boot", cmd_boot);
	fastboot_register("erase:", cmd_erase);
	fastboot_register("flash:", cmd_flash);
	fastboot_register("continue", cmd_continue);
	fastboot_register("reboot", cmd_reboot);
	fastboot_register("reboot-bootloader", cmd_reboot_bootloader);

	fastboot_register("oem load_default_images", cmd_load_default_images);
	fastboot_register("oem load_rescue_images", cmd_load_rescue_images);

	fastboot_register("oem set_load_kernel", cmd_set_load_kernel);
	fastboot_register("oem set_load_dtb", cmd_set_load_dtb);
	fastboot_register("oem set_load_rootfs", cmd_set_load_rootfs);
	fastboot_register("oem set_load_audio", cmd_set_load_audio);
	fastboot_register("oem set_flash_lk", cmd_set_flash_bootcode);
	fastboot_register("oem set_flash_bootcode", cmd_set_flash_bootcode);

	fastboot_register("oem go_a", cmd_go_a);
	fastboot_register("oem go_k", cmd_go_k);
	fastboot_register("oem go_all", cmd_go_all);

	fastboot_register("oem get_emmc_layout", cmd_get_emmc_layout);
	fastboot_register("oem get_part_info", cmd_get_part_info);
	fastboot_register("oem get_fw_info", cmd_get_fw_info);

	fastboot_register("oem help", cmd_help);

	fastboot_publish("product", "kylin");
	fastboot_publish("kernel", "lk");
	return 0;
}
