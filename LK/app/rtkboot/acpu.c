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
#include <trace.h>

#include <rtkboot.h>
#include <fw_info.h>
#include <rtk_ipc_shm.h>
#include <platform/setting.h>
#include <platform/md.h>
#include <platform/io.h>
#if PLATFORM_RTD1295
#include <platform/rtd1295.h>
#elif PLATFORM_RTD1395
#include <platform/rtd1395.h>
#endif
#include <platform/rbus/crt_sys_reg.h>
#include <platform/rbus/iso_reg.h>
#include <arch/ops.h>
#include <factorylib.h>

#define LOCAL_TRACE	0

#define BOOT_PARAM_FILE_NAME	FACTORY_HEADER_FILE_NAME"000BootParam.h"
#define BOOT_AV_INFO_MAGICNO	BOOT_AV_INFO_MAGICNO_RTK
#if PLATFORM_RTD1295
#define BOOT_AV_INFO_MAGICNO_RTK    0x2452544B  // $RTK
#elif PLATFORM_RTD1395
#define BOOT_AV_INFO_MAGICNO_RTK    0x2452544D  // $RTK
#endif

extern char _f_exc_redirect_img, _e_exc_redirect_img;
extern char _f_exc_dispatch_img, _e_exc_dispatch_img;
extern char _f_a_entry_img, _e_a_entry_img;
extern char _f_isrvideo_img, _e_isrvideo_img;
extern char _f_rosbootvector_img, _e_rosbootvector_img;

struct rtk_ipc_shm ipc_shm;
static struct RTK119X_ipc_shm_ir ipc_ir;
static int ipc_ir_set = 0;

unsigned char boot_logo_enable;
unsigned int custom_logo_src_width;
unsigned int custom_logo_src_height;
unsigned int custom_logo_dst_width;
unsigned int custom_logo_dst_height;

extern unsigned int parse_h(
	const char *target_str,
	char *src_addr,
	uint length,
	uint* pRetVal,
	const char **pRetaddr);

static void reset_shared_boot_av(void)
{
	boot_av_info_t *boot_av;

	boot_av = (boot_av_info_t *) MIPS_BOOT_AV_INFO_ADDR;
	memset(boot_av, 0, sizeof(boot_av_info_t));
}


static void set_shared_memory_for_communication_with_ACPU(void)
{
	memset(&ipc_shm,0x0,sizeof(ipc_shm));
	memset(&ipc_ir,0x0,sizeof(ipc_ir));

	// fill the UART register base address for A/V CPU.
	ipc_shm.sys_assign_serial = SWAPEND32(UART1_BASE);
	// VIDEO FORMAT PTR ADDRESS
	ipc_shm.pov_boot_vd_std_ptr = SWAPEND32(VO_RESOLUTION);
	// u-boot version MAGIC pass to FW 'svn\0'
	ipc_shm.u_boot_version_magic = SWAPEND32(0x73766e00);
	// u-boot SVN number pass to FW
	ipc_shm.u_boot_version_info = SWAPEND32(0x000e5606);
	// Audio FW address
	ipc_shm.audio_fw_entry_pt = SWAPEND32(MIPS_AUDIO_FW_ENTRY_ADDR | MIPS_KSEG0BASE);
}

void set_shared_memory_ir_tbl(struct RTK119X_ipc_shm_ir *tbl_addr)
{
	memcpy(&ipc_ir, tbl_addr, sizeof(ipc_ir));
	ipc_ir_set = 1;
}

static void get_bootparam(void)
{
	char *dst_addr;
	const char *retAddr;
	int dst_length;
	unsigned int retVal;

	if (factory_read(BOOT_PARAM_FILE_NAME, &dst_addr, &dst_length)) {
		printf("------------can't find %s\n", BOOT_PARAM_FILE_NAME);
	} else {
		printf("------------%s found\n", BOOT_PARAM_FILE_NAME);

		if (parse_h("BOOT_LOGO_ENABLE", dst_addr, (uint)dst_length, &retVal, &retAddr) == 0) {
			boot_logo_enable = retVal;
			LTRACEF("BOOT_LOGO_ENABLE=%d\n", retVal);
		} else {
			printf("can't find boot_logo_enable\n");
		}

		if (parse_h("CUSTOM_LOGO_SRC_WIDTH", dst_addr, (uint)dst_length, &retVal, &retAddr) == 0) {
			custom_logo_src_width = retVal;
			LTRACEF("CUSTOM_LOGO_SRC_WIDTH=%d\n", retVal);
		} else {
			printf("can't find custom_logo_src_width\n");
		}

		if (parse_h("CUSTOM_LOGO_SRC_HEIGHT", dst_addr, (uint)dst_length, &retVal, &retAddr) == 0) {
			custom_logo_src_height = retVal;
			LTRACEF("CUSTOM_LOGO_SRC_HEIGHT=%d\n", retVal);
		} else {
			printf("can't find custom_logo_src_height\n");
		}

		if (parse_h("CUSTOM_LOGO_DST_WIDTH", dst_addr, (uint)dst_length, &retVal, &retAddr) == 0) {
			custom_logo_dst_width = retVal;
			LTRACEF("CUSTOM_LOGO_DST_WIDTH=%d\n", retVal);
		} else {
			printf("can't find custom_logo_dst_width\n");
		}

		if (parse_h("CUSTOM_LOGO_DST_HEIGHT", dst_addr, (uint)dst_length, &retVal, &retAddr) == 0) {
			custom_logo_dst_height = retVal;
			LTRACEF("CUSTOM_LOGO_DST_HEIGHT=%d\n", retVal);
		} else {
			printf("can't find custom_logo_dst_height\n");
		}
	}

	if (boot_logo_enable)
		printf("[logo]src w/h=%d/%d dst w/h=%d/%d\n",
			custom_logo_src_width ,custom_logo_src_height,
			custom_logo_dst_width ,custom_logo_dst_height);
}

int acpu_bootcode_prepare(void)
{
	int ret = 0;
	char *src, *dst;

	unsigned int current_el = ARM64_READ_SYSREG(CURRENTEL) >> 2;

	//At EL3 able to use MD_KMQ to move these two images into AFW_ISR protect region (0x0~0x1000)
	if (current_el == 3){
		dst = &_e_exc_redirect_img;
		src = &_f_exc_redirect_img;
		ret |= md_memcpy((unsigned char *)MIPS_EXC_REDIRECT_CODE_ADDR, src, dst - src, 1);

		dst = &_e_exc_dispatch_img;
		src = &_f_exc_dispatch_img;
		ret |= md_memcpy((unsigned char *)MIPS_EXC_DISPATCH_CODE_ADDR, src, dst - src, 1);
	}
	

	dst = &_e_a_entry_img;
	src = &_f_a_entry_img;
	ret |= md_memcpy((unsigned char *)MIPS_A_ENTRY_CODE_ADDR, src, dst - src, 0);

	dst = &_e_isrvideo_img;
	src = &_f_isrvideo_img;
	ret |= md_memcpy((unsigned char *)MIPS_ISR_VIDEO_IMG_ADDR, src, dst - src, 0);

	dst = &_e_rosbootvector_img;
	src = &_f_rosbootvector_img;
	ret |= md_memcpy((unsigned char *)MIPS_ROS_BOOT_VECTOR_IMG_ADDR, src, dst - src, 0);

	reset_shared_boot_av();

	set_shared_memory_for_communication_with_ACPU();

	printf("Set ACPU share memory\n");

	get_bootparam();

	return ret;
}

/* Redfined this function in customize functions */
__WEAK void acpu_setup_boot_av(addr_t logo_addr)
{
	boot_av_info_t *boot_av;

	boot_av = (boot_av_info_t *) MIPS_BOOT_AV_INFO_ADDR;
	boot_av->dwMagicNumber = SWAPEND32(BOOT_AV_INFO_MAGICNO);
	
	/*	DHCDRM-689:
		ACPU needs 0x00005a00 bytes region for VO in secure boot.
	 */
	memset((void *)(uintptr_t)VO_SECURE_ADDR, 0x0, sizeof(char)*VO_SECURE_SIZE);
	boot_av->vo_secure_addr = SWAPEND32((unsigned int)(uintptr_t)VO_SECURE_ADDR);
	
	if (boot_logo_enable) {
		boot_av-> logo_enable = boot_logo_enable;
		boot_av-> logo_addr = SWAPEND32((unsigned)logo_addr);
		boot_av-> src_width = SWAPEND32(custom_logo_src_width);
		boot_av-> src_height = SWAPEND32(custom_logo_src_height);
		boot_av-> dst_width = SWAPEND32(custom_logo_dst_width);
		boot_av-> dst_height = SWAPEND32(custom_logo_dst_height);
	}

	ipc_shm.pov_boot_av_info = SWAPEND32(PTR_UINT32(boot_av));
}

void acpu_boot(void)
{
	int magic = SWAPEND32(0x16803001);
	int offset = SWAPEND32(MIPS_SHARED_MEMORY_ENTRY_ADDR);

	// if IR wakeup info is set...
	if (ipc_ir_set) {
		// cat ir info to the end of IPC_SHM with 16 byte align
		void *ir_tbl_addr = (void*)ALIGN((MIPS_SHARED_MEMORY_ENTRY_ADDR + 0xC4 + sizeof(ipc_shm)), 16);
		memcpy(ir_tbl_addr, &ipc_ir, sizeof(ipc_ir));
		ipc_shm.ir_extended_tbl_pt = SWAPEND32(PTR_UINT32(ir_tbl_addr));
	}

	printf("Booting ACPU...\n");
	memcpy((unsigned char *)(MIPS_SHARED_MEMORY_ENTRY_ADDR+0xC4), &ipc_shm, sizeof(ipc_shm));
	memcpy((unsigned char *)(MIPS_SHARED_MEMORY_ENTRY_ADDR), &magic, sizeof(magic));
	memcpy((unsigned char *)(MIPS_SHARED_MEMORY_ENTRY_ADDR +4), &offset, sizeof(offset));
	arch_clean_cache_range(MIPS_SHARED_MEMORY_ENTRY_ADDR, PAGE_SIZE);
#ifdef PLATFORM_RTD1395
	rtd_outl(ACPU_STARTUP_FLAG, ACPU_MAGIC1); //write magic1
#else
	rtd_outl(ISO_RESERVED_USE_3, MIPS_A_ENTRY_CODE_ADDR | MIPS_KSEG1BASE);
#endif
	sync();
	rtd_setbits(SYS_CLOCK_ENABLE2, BIT4);
}
