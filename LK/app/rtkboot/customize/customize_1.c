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
#include <arch/ops.h>

#include <platform/irda.h>
#include <platform/io.h>
#include <platform/gpio.h>
#include <rtkboot.h>
#include <fw_info.h>
#include <rtk_ipc_shm.h>
#include <power.h>
#include <factorylib.h>

#define LOCAL_TRACE	0

#define PWR_KEY_IGPIO	23

unsigned int wake_up_src =
	fWAKEUP_ON_IR |
	fWAKEUP_ON_GPIO |
	fWAKEUP_ON_CEC;

int check_customized_boot_mode(void)
{
	struct RTK119X_ipc_shm_ir tmp_ipc_ir;
	int key_stat = getISOGPIO(PWR_KEY_IGPIO);

	printf("Power-Key stat:%d\n", key_stat);

	/* If boot from ACPU triggered pwr-up, boot up */
	if (!is_boot_from_suspend() && !is_boot_from_kernel()) {
		memset(&tmp_ipc_ir, 0, sizeof(tmp_ipc_ir));
		IR_init();
		IR_config_wakeup_keys(&tmp_ipc_ir);
		set_shared_memory_ir_tbl(&tmp_ipc_ir);
		set_suspend_wakeup_src(wake_up_src);
		set_suspend_gpio_wakeup_src(PWR_KEY_IGPIO, ACTIVE_LOW);
		power_down_soc_S5();
		boot_mode = BOOT_S5_MODE;
	}
        return 0;
}

#define BOOT_LOGO_COUNT			3
#define BOOT_AV_INFO_MAGICNO_CUST	(BOOT_AV_INFO_MAGICNO + 1)
#define FACTORY_ROTATE_PATH		FACTORY_HEADER_FILE_NAME"rotate.bin"

extern unsigned char boot_logo_enable;
extern unsigned int custom_logo_src_width;
extern unsigned int custom_logo_src_height;
extern unsigned int custom_logo_dst_width;
extern unsigned int custom_logo_dst_height;

void acpu_setup_boot_av(addr_t logo_addr)
{
	boot_av_info_t *boot_av;
	ssize_t logo_size = customize_data.logo_size_1 / BOOT_LOGO_COUNT;
	addr_t split_logo_addr[BOOT_LOGO_COUNT];
	char *fac_buf = NULL;
	int i, rotate, fac_len;

	boot_av = (boot_av_info_t *) MIPS_BOOT_AV_INFO_ADDR;
	boot_av->dwMagicNumber = BE32(BOOT_AV_INFO_MAGICNO_CUST);

	LTRACEF("LOGO addr:%p, size:%zu, count:%d", (void*)customize_data.logo_addr_1, logo_size, BOOT_LOGO_COUNT);

	for (i = 0 ; i < BOOT_LOGO_COUNT ; i++) {
		split_logo_addr[i] = customize_data.logo_addr_1 + (i * logo_size);
		LTRACEF("logo[%d] addr:%p\n", i, (void*)split_logo_addr[i]);
	}

	if (boot_logo_enable) {
		if (!factory_read(FACTORY_ROTATE_PATH, &fac_buf, &fac_len)) {
			rotate = strtol(fac_buf, NULL, 10);
			if (rotate < 0 || rotate >= BOOT_LOGO_COUNT)
				rotate = 0;
			printf("use logo %d\n", rotate);
		} else {
			printf("No rotate info, use logo 0\n");
			rotate = 0;
		}

		boot_av-> logo_enable = boot_logo_enable;
		boot_av-> logo_addr = BE32((unsigned)logo_addr);
		boot_av-> src_width = BE32(custom_logo_src_width);
		boot_av-> src_height = BE32(custom_logo_src_height);
		boot_av-> dst_width = BE32(custom_logo_dst_width);
		boot_av-> dst_height = BE32(custom_logo_dst_height);
		boot_av-> logo_addr_2 = BE32((unsigned)split_logo_addr[rotate]);
	}

	ipc_shm.pov_boot_av_info = BE32(PTR_UINT32(boot_av));
}
