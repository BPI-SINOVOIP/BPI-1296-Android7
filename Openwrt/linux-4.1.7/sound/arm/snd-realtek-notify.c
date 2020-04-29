/**
 * snd-realtek-notify.c - Realtek alsa driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#define pr_fmt(fmt) "snd-rtk-an: " fmt

#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <soc/realtek/kernel-rpc.h>
#include <sound/asound.h>
#include <RPCDriver.h>
#include "snd-realtek.h"

extern struct ion_client *alsa_client;

static inline struct ion_client *__get_ion_client(void)
{
	return alsa_client;
}

enum {
	ENUM_DT_AI_EN = 0, /* this is used by acpu */
	ENUM_DT_AI_AIN, /* always set this bit if using AI */
	ENUM_DT_AI_ADC,
	ENUM_DT_AI_ANALOG_IN,
};

enum {
	ENUM_AI_I2S_STATUS = 0,
	ENUM_AI_I2S_PIN_SHARE_BIT, /* pin-share:1, pin-dependent:0 */
	ENUM_AI_I2S_AI_MASTER_BIT, /* AI-master:1, AI-slave:0 */
};

enum {
	ENUM_DT_AO_DAC    = 0,
	ENUM_DT_AO_I2S    = 1,
	ENUM_DT_AO_SPDIF  = 2,
	ENUM_DT_AO_HDMI   = 3,
	ENUM_DT_AO_GLOBAL = 4,
};

/* privateInfo for AI */
#define AN_AI_DEVICE_ON         0
#define AN_AI_DEVICE_I2S_CONF   1

/* privateInfo for AO */
#define AN_AO_DEVICE_OFF   0
#define AN_AO_I2S_CH       1

static int __send_rpc(struct ion_client *client,
	AUDIO_ENUM_AIO_PRIVAETINFO info, unsigned int *data, int size)
{
	struct ion_handle *handle;
	ion_phys_addr_t phys;
	size_t len;
	AUDIO_RPC_AIO_PRIVATEINFO_PARAMETERS *arg;
	HRESULT RPC_ret;
	int off;
	int ret = -EINVAL;
	int i;

	if (!client)
		return -EINVAL;

	handle = ion_alloc(client, 4096, 1024,
		RTK_PHOENIX_ION_HEAP_AUDIO_MASK, AUDIO_ION_FLAG);
	if (IS_ERR(handle)) {
		ret = PTR_ERR(handle);
		goto exit;
	}

	ret = ion_phys(client, handle, &phys, &len);
	if (ret)
		goto exit;

	arg = ion_map_kernel(client, handle);
	memset(arg, 0, sizeof(*arg));

	arg->type = htonl(info);
	for (i = 0; i < size; i++)
		arg->argateInfo[i] = htonl(data[i]);
	off = get_rpc_alignment_offset(sizeof(*arg));

	if (send_rpc_command(RPC_AUDIO,
		ENUM_KERNEL_RPC_AIO_PRIVATEINFO,
		CONVERT_FOR_AVCPU(phys),
		CONVERT_FOR_AVCPU(phys + off),
		&RPC_ret)) {
		ret = -EINVAL;
		goto exit;
	}

	ret = RPC_ret != S_OK ? -EINVAL : 0;

exit:
	if (handle) {
		ion_unmap_kernel(client, handle);
		ion_free(client, handle);
	}
	return ret;
}

struct rpc_data {
	AUDIO_ENUM_AIO_PRIVAETINFO info;
	unsigned int data[16];
	int size;
	int (*init)(struct device *dev, struct rpc_data *rpcd);
};

static bool rpc_data_is_empty(struct rpc_data *rpcd)
{
	int i;

	for (i = 0; i < rpcd->size; i++)
		if (rpcd->data[i] != 0)
			return true;
	return false;
}

#define U32_PTR(_v) ((void *)(_v))
#define PTR_U32(_p) ((u32)(unsigned long)(_p))

static int send_rpc(struct device *dev, struct rpc_data *rpcd)
{
	int ret;
	struct ion_client *client = __get_ion_client();

	if (rpcd->size > 16)
		return -E2BIG;

	if (IS_ENABLED(AN_DEBUG)) {
		int i;

		dev_err(dev, "data\n");
		for (i = 0; i < rpcd->size; i++)
			dev_err(dev, "    [%2d] = 0x%08x\n", i, rpcd->data[i]);
	}

	ret = __send_rpc(client, rpcd->info, rpcd->data, rpcd->size);
	if (ret) {
		dev_err(dev, "failed to notify ACPU: %d\n", ret);
		return ret;
	}

	return 0;
}

static int __init rtk_audio_notifier_ai_init(struct device *dev,
	struct rpc_data *rpcd)
{
	struct device_node *np = dev->of_node;
	unsigned int val;

	/* DISP_MUXPAD0 */
	if (!of_property_read_u32(np, "muxpad0", &val)) {
		dev_err(dev, "muxpad0 must not set directly, use pinctrl\n");
	}

	if (of_property_match_string(np, "ai,type", "i2s") >= 0) {
		rpcd->data[AN_AI_DEVICE_I2S_CONF] |=
			BIT(ENUM_AI_I2S_STATUS);

		if (of_find_property(np, "ai,i2s-pin-shared", NULL))
			rpcd->data[AN_AI_DEVICE_I2S_CONF] |=
				BIT(ENUM_AI_I2S_PIN_SHARE_BIT);
		if (of_find_property(np, "ai,i2s-master", NULL))
			rpcd->data[AN_AI_DEVICE_I2S_CONF] |=
				BIT(ENUM_AI_I2S_AI_MASTER_BIT);
	}

	return rpc_data_is_empty(rpcd) ? send_rpc(dev, rpcd) : 0;
}

static const struct of_device_id rtk_ao_matches[] = {
	{.compatible = "dac",    .data = U32_PTR(ENUM_DT_AO_DAC)},
	{.compatible = "i2s",    .data = U32_PTR(ENUM_DT_AO_I2S)},
	{.compatible = "spdif",  .data = U32_PTR(ENUM_DT_AO_SPDIF)},
	{.compatible = "hdmi",   .data = U32_PTR(ENUM_DT_AO_HDMI)},
	{.compatible = "global", .data = U32_PTR(ENUM_DT_AO_GLOBAL)},
	{}
};

static int __init rtk_audio_notifier_ao_init(struct device *dev,
	struct rpc_data *rpcd)
{
	struct device_node *np = dev->of_node;
	struct device_node *child;
	unsigned int mask = BIT(ENUM_DT_AO_DAC) |
		BIT(ENUM_DT_AO_SPDIF) |
		BIT(ENUM_DT_AO_I2S) |
		BIT(ENUM_DT_AO_HDMI) |
		BIT(ENUM_DT_AO_GLOBAL);

	for_each_child_of_node(np, child) {
		const struct of_device_id *match;

		match = of_match_node(rtk_ao_matches, child);
		if (!match)
			continue;

		if (!of_device_is_available(child))
			continue;

		if (!strcmp(match->compatible, "i2s")) {
			int ch;

			if (of_property_read_u32(child, "channel", &ch))
				ch = 0;

			rpcd->data[AN_AO_I2S_CH] = ch == 8 ? 1 : 0;
		}

		mask &= ~BIT(PTR_U32(match->data));
		dev_info(dev, "mask = 0x%08x by %s\n", mask, match->compatible);
	}

	rpcd->data[AN_AO_DEVICE_OFF] = mask;

	return rpc_data_is_empty(rpcd) ? send_rpc(dev, rpcd) : 0;
}

static struct rpc_data ai_data = {
	.info = ENUM_PRIVATEINFO_AIO_AI_INTERFACE_SWITCH_CONTROL,
	.data = {
		[AN_AI_DEVICE_ON] = BIT(ENUM_DT_AI_AIN) | BIT(ENUM_DT_AI_ADC),
		[AN_AI_DEVICE_I2S_CONF] = 0,
	},
	.size = 2,
	.init = rtk_audio_notifier_ai_init,
};

static struct rpc_data ai_analog_data = {
	.info = ENUM_PRIVATEINFO_AIO_AI_INTERFACE_SWITCH_CONTROL,
	.data = {
		[AN_AI_DEVICE_ON] = BIT(ENUM_DT_AI_AIN) | BIT(ENUM_DT_AI_ANALOG_IN),
		[AN_AI_DEVICE_I2S_CONF] = 0,
	},
	.size = 2,
	.init = rtk_audio_notifier_ai_init,
};

static struct rpc_data ao_data = {
	.info = ENUM_PRIVATEINFO_AIO_AO_INTERFACE_SWITCH_CONTROL,
	.data = {
		[AN_AO_DEVICE_OFF] = 0,
		[AN_AO_I2S_CH] = 0,
	},
	.size = 2,
	.init = rtk_audio_notifier_ao_init,
};

static const struct of_device_id rtk_audio_dev_ids[] = {
	{.compatible = "alc5279", .data = &ai_data},
	{.compatible = "wm8782", .data = &ai_data},
	{.compatible = "realtek,audio-in", .data = &ai_data},
	{.compatible = "realtek,audio-analog-in", .data = &ai_analog_data},
	{.compatible = "realtek,audio-out", .data = &ao_data},
	{}
};

static void rtk_audio_notifier_of_get_gpios(struct device *dev)
{
	struct device_node *np = dev->of_node;
	int num;
	int i;

	num = of_gpio_count(np);
	for (i = 0; i < num; i++) {
		int gpio = of_get_gpio(np, i);

		if (gpio < 0) {
			dev_err(dev, "failed to get gpio[%d]\n", i);
			continue;
		}

		if (!gpio_is_valid(gpio)) {
			dev_err(dev, "invalid gpio num %d\n", gpio);
			continue;
		}

		if (gpio_request(gpio, dev_name(dev))) {
			dev_err(dev, "failed to request gpio %d\n", gpio);
			continue;
		}
	}
}

static int rtk_audio_notifier_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	const struct of_device_id *match;
	struct rpc_data *rpcd;

	dev_info(&pdev->dev, "[AUDIO-NOTIFY] %s\n", __func__);

	match = of_match_node(rtk_audio_dev_ids, dev->of_node);
	if (!match) {
		dev_warn(dev, "no drv data\n");
		return -EINVAL;
	}

	rtk_audio_notifier_of_get_gpios(dev);

	rpcd = (struct rpc_data *)match->data;
	rpcd->init(dev, rpcd);

	return 0;
}

static struct platform_driver rtk_audio_notifier_drv = {
	.driver = {
		.name   = "audio-notifier",
		.of_match_table = rtk_audio_dev_ids,
	},
	.probe = rtk_audio_notifier_probe,
};

static int __init rtk_audio_notifier_init(void)
{
	return platform_driver_register(&rtk_audio_notifier_drv);
}
late_initcall(rtk_audio_notifier_init);
