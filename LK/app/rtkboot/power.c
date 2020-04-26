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

#include <platform/gpio.h>
#include <rtkboot.h>
#include <fw_info.h>
#include <rtk_ipc_shm.h>
#include <power.h>

#define LOCAL_TRACE	0


static struct power_saving_config * pconfig = NULL;

#define rtk_suspend_shm_func(_name, _offset, _def)					\
static void rtk_suspend_##_name##_set(unsigned int val)					\
{											\
	writel(SWAPEND32(SUSPEND_MAGIC_MASK | _def##_MASK(val)), &(ipc_shm._offset));	\
}											\
static unsigned int rtk_suspend_##_name##_get(void)						\
{											\
	unsigned int val = SWAPEND32(readl(&(ipc_shm._offset)));			\
	if (SUSPEND_MAGIC_GET(val) != SUSPEND_MAGIC_KEY) {				\
		printf("[RTD129x_PM] Error! val = 0x%08x\n", val);		\
		return -1;								\
	}										\
	return _def##_GET(val);								\
}

rtk_suspend_shm_func(wakeup_flags, suspend_wakeup_flag, WAKEUP_FLAGS);
rtk_suspend_shm_func(gpio_wakeup_en, gpio_wakeup_enable, GPIO_WAKEUP_EN);
rtk_suspend_shm_func(gpio_wakeup_act, gpio_wakeup_activity, GPIO_WAKEUP_ACT);
rtk_suspend_shm_func(gpio_wakeup_en2, gpio_wakeup_enable2, GPIO_WAKEUP_EN2);
rtk_suspend_shm_func(gpio_wakeup_act2, gpio_wakeup_activity2, GPIO_WAKEUP_ACT2);

void set_suspend_wakeup_src(unsigned int val)
{
	rtk_suspend_wakeup_flags_set(val);
}

unsigned int get_suspend_wakeup_src(void)
{
	return rtk_suspend_wakeup_flags_get();
}

/* Only support igpio for wakeup-src */
/* (0) active low (1) active high    */
int set_suspend_gpio_wakeup_src(int igpio, int activity)
{
	int orig_src, orig_activ, target_src, target_activ;
	int base_shift;
	void (*_gpio_wakeup_en_set)(unsigned int);
	unsigned int (*_gpio_wakeup_en_get)(void);
	void (*_gpio_wakeup_act_set)(unsigned int);
	unsigned int (*_gpio_wakeup_act_get)(void);

	if (igpio < 0 || igpio > ISO_GPIO_MAX) {
		printf("%s, invalid IGPIO:%d\n", __func__, igpio);
		return -1;
	}

	if (igpio < GPIO_WAKEUP_EN_BITS) {
		base_shift = 0;
		_gpio_wakeup_en_set = rtk_suspend_gpio_wakeup_en_set;
		_gpio_wakeup_en_get = rtk_suspend_gpio_wakeup_en_get;
		_gpio_wakeup_act_set = rtk_suspend_gpio_wakeup_act_set;
		_gpio_wakeup_act_get = rtk_suspend_gpio_wakeup_act_get;
	} else {
		base_shift = GPIO_WAKEUP_EN_BITS;
		_gpio_wakeup_en_set = rtk_suspend_gpio_wakeup_en2_set;
		_gpio_wakeup_en_get = rtk_suspend_gpio_wakeup_en2_get;
		_gpio_wakeup_act_set = rtk_suspend_gpio_wakeup_act2_set;
		_gpio_wakeup_act_get = rtk_suspend_gpio_wakeup_act2_get;
	}

	orig_src = _gpio_wakeup_en_get();
	orig_activ = _gpio_wakeup_act_get();

	if (orig_src == -1 || orig_activ == -1) {
		printf("%s, wakeup GPIO not config\n", __func__);
		target_src = 0;
		target_activ = 0;
	} else {
		target_src = orig_src;
		target_activ = orig_activ;
	}

	target_src |= (1 << (igpio - base_shift));
	if (activity)
		target_activ |=  (1 << (igpio - base_shift));
	else
		target_activ &=  ~(1 << (igpio - base_shift));

	_gpio_wakeup_en_set(target_src);
	_gpio_wakeup_act_set(target_activ);

	return 0;
}

__WEAK void disable_soc_full_reset(void)
{
}

void power_down_soc_S5(void)
{
	// First, disable pmic reset so dummy register could be passed
	disable_soc_full_reset();

	pconfig =(struct power_saving_config *)POWER_SAVING;
	pconfig->power_down_S5 = 1;
	arch_clean_cache_range((addr_t)pconfig, sizeof(struct power_saving_config));
	printf("%s, boot into S5\n", __func__);
}

static void power_saving_ipcshm_init(void)
{
	struct power_saving_config temp;
	pconfig =(struct power_saving_config *)POWER_SAVING;
	ipc_shm.power_saving_ptr = SWAPEND32(POWER_SAVING);

	memset(&temp,0x0,sizeof(struct power_saving_config));
	temp.magic = 0x65868380; //PSVA( power saving version A)

	memcpy(pconfig, &temp, sizeof(struct power_saving_config));
	arch_clean_cache_range((addr_t)pconfig, sizeof(struct power_saving_config));
}

void power_saving_init(void)
{
	printf("Default Power-Config\n");

	/* Add power down solution here */

	power_saving_ipcshm_init();
}
