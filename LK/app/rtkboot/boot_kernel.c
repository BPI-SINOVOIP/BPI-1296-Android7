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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <compiler.h>
#include <lib/console.h>
#include <lib/sysparam.h>
#include <arch/ops.h>
#include <arch/arm64/mmu.h>

#include <rtkboot.h>
#include <fw_info.h>
#include <libfdt.h>
#include <platform/sb2.h>

#include <trace.h>
#define LOCAL_TRACE 0

#define LINUX_ARM64_IMAGE_MAGIC 0x644d5241

struct Bootimg_Header {
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

addr_t fdt_addr = 0;
addr_t bootimg_addr = 0;
extern unsigned int Auto_AFW_MEM_START;
extern void eth_parse_enetaddr(const char *addr, uchar *enetaddr);

static int fdt_modify(void)
{
	int ret;
	unsigned int offset;
	void *fdt = (void *)(unsigned long)fdt_addr;
	char *val = NULL;
	int val_len;

	
	if(!Auto_AFW_MEM_START)
		Auto_AFW_MEM_START = AFW_MEM_START;
	/* If AFW load failed, using default address of AFW. */

	if ((ret = fdt_open_into(fdt, fdt, fdt_totalsize(fdt)))) {
		printf("%s, fdt_open_into fail!!\n", __func__);
		goto err;
	}

	/* remove node /psic */
	ret = fdt_path_offset(fdt, "/psci");
	if (ret > 0) {
		offset = ret;
		ret = fdt_del_node(fdt, offset);
		printf("[FDT] remove node /psci\n");
	}

	/* set boot-secondary-addr in /cpus/cpu@1 */
	ret = fdt_path_offset(fdt, "/cpus/cpu@1");
	if (ret > 0) {
		offset = ret;
		ret = fdt_setprop_u32(fdt, offset, "boot-secondary-addr", 0x00020000);
		printf("[FDT] add boot-secondary-addr to /cpus/cpu@1\n");
	}

#if PLATFORM_RTD1295	
	if (get_chip_rev() >= KYLIN_REV_A01){
#endif		
		//total size need to be >= offset_dt_strings + size_dt_strings + new node size
		struct fdt_header *fdth = (struct fdt_header*)fdt; 
		fdth->totalsize= cpu_to_fdt32(fdt_totalsize(fdt) + 32 * sizeof(struct fdt_reserve_entry));
		ret = fdt_add_mem_rsv((void*)fdt_addr, TEE_MEM_START, TEE_MEM_SIZE);
		if (ret) {
			printf("[FDT] add tee mem rsv fail (ret = %d)\n", ret);
			goto err;
		}
		ret = fdt_add_mem_rsv((void*)fdt_addr, Auto_AFW_MEM_START, AFW_MEM_SIZE);
		if (ret) {
			printf("[FDT] add afw mem rsv fail (ret = %d)\n", ret);
			goto err;
		}
		ret = fdt_add_mem_rsv((void*)fdt_addr, BOOT_LOGO_ADDR, BOOT_LOGO_SIZE);
		if (ret) {
			printf("[FDT] add BOOT_LOGO_ADDR rsv fail (ret = %d)\n", ret);
			goto err;
		}
#if PLATFORM_RTD1295		
	}
#endif

#define BOOTARGS_MAX_LEN	1024
	if (!(val = (char*)malloc(BOOTARGS_MAX_LEN))) {
		printf("%s: couldn't allocate val buffer\n", __func__);
		ret = -1;
		goto err;
	}

	if ((sysparam_length("bootargs")) > 0) {
		ret = fdt_path_offset(fdt, "/chosen");
		offset = ret;
		if (offset > 0) {
			memset(val, 0, BOOTARGS_MAX_LEN);
			sysparam_read("bootargs", val, BOOTARGS_MAX_LEN - 1);
			ret = fdt_setprop(fdt, offset, "bootargs", val, strlen(val) + 1);
		}
	}
	
	/*
	 *  Set the reserved address information for boot logo in device tree.
	 */
	ret = fdt_path_offset(fdt, "/chosen");
	offset = ret;
	if (offset > 0){
		ret = fdt_setprop_u32(fdt, offset, "logo-area", BOOT_LOGO_ADDR);
		if (ret < 0)
			printf("WARNING: could not set logo-area %s.\n",
				fdt_strerror(ret));
		ret = fdt_appendprop_u32(fdt, offset, "logo-area", BOOT_LOGO_SIZE);
		if (ret < 0)
			printf("WARNING: could not set logo-area size %s.\n",
				fdt_strerror(ret));
	}

	// Append NAND flash info
	int nandstr_len = sysparam_length("nand_info");
	if (nandstr_len > 0) {
		ret = fdt_path_offset(fdt, "/chosen");
		offset = ret;
		if (offset > 0) {
			int arg_len = 0;
			const void *arg_ptr = NULL;
			char nand_str[BOOTARGS_MAX_LEN / 2] = "";

			sysparam_read("nand_info", nand_str, BOOTARGS_MAX_LEN / 2);
			arg_ptr = fdt_getprop(fdt, offset, "bootargs", &arg_len);
			if (arg_len < 0) {
				printf("%s: get bootargs error with errorcode %d\n", __func__, arg_len);
				ret = arg_len;
				goto err;
			}

			if (arg_len + nandstr_len > BOOTARGS_MAX_LEN) {
				printf("bootarg_len:%d + nandstr_len:%d exceed BOOTARGS_MAX_LEN:%d\n",
					arg_len, nandstr_len, BOOTARGS_MAX_LEN);
				ret = -1;
				goto err;
			}

			strncpy(val, arg_ptr, arg_len);
			strcat(val, " ");
			strncat(val, nand_str, BOOTARGS_MAX_LEN / 2);
			LTRACEF("nand_part:%s\n", nand_str);
			LTRACEF("bootargs:%s\n", val);
			ret = fdt_setprop(fdt, offset, "bootargs", val, strlen(val) + 1);
			LTRACEF("ret:%d\n", ret);
		}
	}

	// Append NAND partition info
	int mtdstr_len = sysparam_length("mtd_part");
	if (mtdstr_len > 0) {
		ret = fdt_path_offset(fdt, "/chosen");
		offset = ret;
		LTRACEF("detect mtd_part\n");
		if (offset > 0) {
			int bootarg_len = 0;
			const void *bootarg_ptr = NULL;
			char mtd_str[BOOTARGS_MAX_LEN / 2] = "";

			sysparam_read("mtd_part", mtd_str, BOOTARGS_MAX_LEN);
			bootarg_ptr = fdt_getprop(fdt, offset, "bootargs", &bootarg_len);
			if (bootarg_len < 0) {
				printf("%s: get bootargs error with errorcode %d\n", __func__, bootarg_len);
				ret = bootarg_len;
				goto err;
			}

			if (bootarg_len + mtdstr_len > BOOTARGS_MAX_LEN) {
				printf("bootarg_len:%d + mtdstr_len:%d exceed BOOTARGS_MAX_LEN:%d\n",
					bootarg_len, mtdstr_len, BOOTARGS_MAX_LEN);
				ret = -1;
				goto err;
			}

			strncpy(val, bootarg_ptr, bootarg_len);
			strcat(val, " ");
			strncat(val, mtd_str, BOOTARGS_MAX_LEN / 2);
			LTRACEF("mtd_part:%s\n", mtd_str);
			LTRACEF("bootargs:%s\n", val);
			ret = fdt_setprop(fdt, offset, "bootargs", val, strlen(val) + 1);
			LTRACEF("ret:%d\n", ret);
		}
	}

#ifdef NAS_ENABLE
	if ((sysparam_length("nas_part")) > 0) {
		ret = fdt_path_offset(fdt, "/chosen");
		offset = ret;
		if (offset > 0) {
			memset(val, 0, BOOTARGS_MAX_LEN);
			sysparam_read("nas_part", val, BOOTARGS_MAX_LEN - 1);
			if ((sysparam_length("nasargs")) > 0) {
				ret = fdt_setprop(fdt, offset, "nasargs", val, strlen(val));
			}
			else {
				ret = fdt_setprop_string(fdt, offset, "nasargs", val);
			}
		}
		if (ret) {
			printf("Could not set nasargs\n");
			goto err;
		}
	}
	if ((sysparam_length("nasargs")) > 0) {
		ret = fdt_path_offset(fdt, "/chosen");
		offset = ret;
		if (offset > 0) {
			memset(val, 0, BOOTARGS_MAX_LEN);
			sysparam_read("nasargs", val, BOOTARGS_MAX_LEN - 1);
			ret = fdt_appendprop_string(fdt, offset, "nasargs", val);
		}
		if (ret) {
			printf("Could not append nasargs\n");
			goto err;
		}
	}
#endif

#ifndef DEVICE_NO_ETHERNET
	/* add MAC address */
	if ((val_len = sysparam_length("ethaddr")) > 0) {
		unsigned char mac[6];

		sysparam_read("ethaddr", val, val_len + 1);
		eth_parse_enetaddr(val, mac);

		/* ETN */
		ret = fdt_path_offset(fdt, "/gmac@98016000");
		if (ret > 0) {
			offset = ret;
			ret = fdt_setprop(fdt, offset, "local-mac-address", mac, 6);
		}
#if PLATFORM_RTD1295
		/* NAT */
		ret = fdt_path_offset(fdt, "/gmac@0x98060000");
		if (ret > 0) {
			offset = ret;
			ret = fdt_setprop(fdt, offset, "local-mac-address", mac, 6);
		}
#endif
		printf("[FDT] update MAC address\n");
	}

#if PLATFORM_RTD1395
	/* ETN */
	ret = fdt_path_offset(fdt, "/gmac@98016000");
	if (ret > 0) {
		int ptr_len = 0;
		const void *ptr = NULL;

		offset = ret;
		ptr = fdt_getprop(fdt, offset, "output-mode", &ptr_len);
		if (ptr_len > 0 && *(uint32_t *)ptr == 0) {
			fdt_setprop_u32(fdt, offset, "force-Gb-off", 1);
		}
	}
#endif
#else
	(void)val_len;
#endif
	ret = fdt_pack(fdt);
err:
	if (val)
		free(val);
	return ret;
}


static int fdt_prepare(void)
{
	int param_size = 0;
	char buff[ADDR_BUF_LEN] = "";

	if ((param_size = sysparam_length("fdt_loadaddr")) < 0) {
		printf("%s, fdt_loadaddr not found\n", __func__);
		return RTK_PLAT_ERR_BOOT;
	}

	if (sysparam_read("fdt_loadaddr", buff, param_size) < 0) {
		printf("%s, fdt_loadaddr read error\n", __func__);
		return RTK_PLAT_ERR_BOOT;
	}

	fdt_addr = atoull(buff);
	printf("fdt_loadaddr : 0x%08llx\n", (unsigned long long)fdt_addr);

	if (fdt_check_header((void*)fdt_addr)) {
		printf("%s, FDT header error!!\n", __func__);
		return RTK_PLAT_ERR_BOOT;
	}

	if (fdt_modify())
		return RTK_PLAT_ERR_BOOT;

	return RTK_PLAT_ERR_OK;
}

#ifndef NAS_DUAL
#define SKIP_HYPERVISOR \
( \
	(boot_mode == BOOT_RESCUE_MODE) || \
	(boot_mode == BOOT_GOLD_MODE) || \
	(boot_mode == BOOT_RECOVERY_MODE) \
)
#else
#define SKIP_HYPERVISOR \
( \
	(boot_mode == BOOT_RESCUE_MODE) || \
	(boot_mode == BOOT_RECOVERY_MODE) \
)
#endif

static int boot_image_prepare(void)
{
	struct Bootimg_Header *bootimg_header = NULL;
	unsigned int img_size = 0;
	addr_t img_addr = 0;
	int param_size = 0;
	char buff[ADDR_BUF_LEN] = "";

	/* Hypervisor should only be booted under Normal mode */
	if (!SKIP_HYPERVISOR && ((param_size = sysparam_length("hyp_loadaddr")) > 0)) {
		/* Hypervisor Detect */
		if (sysparam_read("hyp_loadaddr", buff, param_size) < 0) {
			printf("%s, hyp_loadaddr read error\n", __func__);
			return RTK_PLAT_ERR_READ_KERNEL_IMG;
		}
	} else {
		/* Kernel only, no HYP */
		if ((param_size = sysparam_length("kernel_loadaddr")) < 0) {
			printf("%s, kernel_loadaddr not found\n", __func__);
			return RTK_PLAT_ERR_READ_KERNEL_IMG;
		}

		if (sysparam_read("kernel_loadaddr", buff, param_size) < 0) {
			printf("%s, kernel_loadaddr read error\n", __func__);
			return RTK_PLAT_ERR_READ_KERNEL_IMG;
		}
	}

	img_addr = atoull(buff);
	bootimg_header = (struct Bootimg_Header*)img_addr;

	printf("Boot image target addr:0x%08llx, size:0x%08llx\n", bootimg_header->image_offset, bootimg_header->image_size);

	if (bootimg_header->magic != LINUX_ARM64_IMAGE_MAGIC) {
		printf("Bad BootIMG MAGIC!!\n");
		return RTK_PLAT_ERR_READ_KERNEL_IMG;
	}

	if ((img_size = bootimg_header->image_size) == 0) {
		printf("Bad BootIMG size!!\n");
		return RTK_PLAT_ERR_READ_KERNEL_IMG;
	}

	memmove((void*)bootimg_header->image_offset, (void*)img_addr, img_size);
	bootimg_addr = (addr_t)bootimg_header->image_offset;

	return 0;
}

static void hw_boot_config(void)
{
	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;
	/* turn-off all IRQ/FIQ */
	arch_disable_ints();
	arch_disable_fiqs();
	ISB;

	/* turn-off CACHE/MMU */
	arch_disable_mmu();
	arch_disable_cache(DCACHE);
	arch_clean_invalidate_cache_range(MEMBASE, MEMSIZE);
	invalidate_tlb_all();
	// we turn off ICACHE in last place for speed
	arch_disable_cache(ICACHE);
	arch_invalidate_icache_all();

	/* Switch EL */
	if (current_el == 3)
		arm64_el3_to_el2();
}

int rtk_boot_kernel(void)
{
	void (*kernel_entry)(void *fdt_addr, void *reserve0, void *reserve1, void *reserve2);
	int ret = 0;

	printf("Booting Kernel...\n");

	/* Prepare FDT */
	if((ret = fdt_prepare()) != RTK_PLAT_ERR_OK) {
		return ret;
	}

	/* Prepare Boot image */
	if((ret = boot_image_prepare()) != RTK_PLAT_ERR_OK) {
		return ret;
	}

	kernel_entry = (void(*)(void*,void*,void*,void*))bootimg_addr;

	/* Proceed the final HW config before jump to bootimage */
	hw_boot_config();	

	kernel_entry((void*)fdt_addr, NULL, NULL, NULL);

	/* should never reach here */
	return RTK_PLAT_ERR_BOOT;
}
