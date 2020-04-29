/*
 * busfreq-rtd129x.c - DFS for bus_h & bus based on DDR/CPU
 *
 * Copyright (C) 2016-2017 Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) "busfreq: " fmt

#include <linux/clk.h>
#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/devfreq.h>
#include <linux/io.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/sysfs.h>

#define DC_PC_BASE            0x98008000
#define DC_PC_CTRL                 0x030
#define DC_PC_TOTA_MON_NUM_0       0x034
#define DC_PC_TOTA_ACK_NUM_0       0x038
#define DC_PC_TOTA_IDL_NUM_0       0x03c
#define DC_PC_TOTA_MON_NUM_1       0x040
#define DC_PC_TOTA_ACK_NUM_1       0x044
#define DC_PC_TOTA_IDL_NUM_1       0x048

static struct dc_pc_priv {
	void __iomem *base;
	int phy_off[2];
} dc_pc_priv;

static void dc_pc_init(void)
{
	dc_pc_priv.base = ioremap(DC_PC_BASE, 0x50);
	dc_pc_priv.phy_off[0] = 0x34;
	dc_pc_priv.phy_off[1] = 0x40;
}

static bool dc_pc_is_stopped(void)
{
	return readl(dc_pc_priv.base + DC_PC_CTRL) == 0;
}

static void dc_pc_start(void)
{
	writel(1, dc_pc_priv.base + DC_PC_CTRL);
}

static void dc_pc_stop(void)
{
	writel(0, dc_pc_priv.base + DC_PC_CTRL);
}

static unsigned int dc_pc_get_load(int phy)
{
	unsigned int offset = dc_pc_priv.phy_off[phy];
	unsigned long time, idle;
	unsigned int load;

	time = readl(dc_pc_priv.base + offset);
	idle = readl(dc_pc_priv.base + offset + 0x8);

	load = !time ? 0 : (1000 - idle * 1000 / time);
	return load;
}

static unsigned int dc_pc_get_time(int phy)
{
	unsigned int offset = dc_pc_priv.phy_off[phy];

	return readl(dc_pc_priv.base + offset);
}

static unsigned int dc_pc_get_max_load(void)
{
	unsigned int load_0, load_1;

	load_0 = dc_pc_get_load(0);
	load_1 = dc_pc_get_load(1);

	return load_0 > load_1 ? load_0 : load_1;
}

static DEFINE_PER_CPU(u64, prev_idle);
static DEFINE_PER_CPU(u64, prev_time);

static unsigned int cpu_get_load(int cpu)
{
	int load;
	u64 time, idle, d_idle, d_time;

	idle = get_cpu_idle_time(cpu, &time, 1);
	d_idle = idle - per_cpu(prev_idle, cpu);
	d_time = time - per_cpu(prev_time, cpu);
	if (d_time <= d_idle)
		load = 0;
	else
		load = div64_u64(100 * (d_time - d_idle), d_time);

	per_cpu(prev_idle, cpu) = idle;
	per_cpu(prev_time, cpu) = time;
	return load;
}

static unsigned int cpu_get_max_load(void)
{
	int cpu;
	int max_load = 0;
	int load;

	for_each_cpu(cpu, cpu_online_mask) {
		load = cpu_get_load(cpu);
		if (load > max_load)
			max_load = load;
	}

	return max_load;
}



#define BUSFREQ_MAX_STATE 2
#define BUSFREQ_MAX_CLK   2
#define BUSFREQ_STATE_LOW    0
#define BUSFREQ_STATE_HIGH   1
#define BUSFREQ_VIRT_FREQ_LOW     100000000
#define BUSFREQ_VIRT_FREQ_HIGH    200000000

struct busfreq_clk_data {
	struct clk *clk;
	int n_rates;
	unsigned int rates[BUSFREQ_MAX_STATE];
};

static struct busfreq_clk_data *of_busfreq_clk_init(struct device_node *np)
{
	struct busfreq_clk_data *data;

	data = kzalloc(sizeof(*data), GFP_KERNEL);
	if (!data)
		return NULL;

	data->clk = of_clk_get(np, 0);
	if (IS_ERR(data->clk)) {
		pr_err("%s: get clk fail\n", np->name);
		kfree(data);
		return NULL;
	}

	data->n_rates = of_property_count_u32_elems(np, "rate");
	if (data->n_rates > BUSFREQ_MAX_STATE) {
		pr_warn("%s: too many states\n", np->name);
		data->n_rates = BUSFREQ_MAX_STATE;
	}

	of_property_read_u32_array(np, "rate", data->rates, data->n_rates);

	return data;
}

static void busfreq_set_clk_rate(struct busfreq_clk_data *data, int index)
{
	if (!data)
		return;

	if (index >= data->n_rates)
		index =  data->n_rates - 1;
	clk_set_rate(data->clk, data->rates[index] * 1000000);
}

static __maybe_unused void busfreq_clk_destroy(struct busfreq_clk_data *data)
{
	clk_put(data->clk);
	kfree(data);
}

struct busfreq_priv {
	struct devfreq *devfreq;
	struct clk *ref;

	int load_cnt;
	unsigned int curr_freq;

	struct busfreq_clk_data *clkd[BUSFREQ_MAX_CLK];
};

static inline int __is_freq_high(struct busfreq_priv *priv)
{
	return priv->curr_freq != BUSFREQ_VIRT_FREQ_LOW;
}

#define BUSFREQ_FORCE_MODE_NONE   0
#define BUSFREQ_FORCE_MODE_LOW    1
#define BUSFREQ_FORCE_MODE_HIGH   2
#define BUSFREQ_FORCE_MODE_MAX    3

const char *busfreq_mode_strs[] = {
	[BUSFREQ_FORCE_MODE_NONE] = "none",
	[BUSFREQ_FORCE_MODE_LOW] = "low",
	[BUSFREQ_FORCE_MODE_HIGH] = "high"
};

static int busfreq_enabled = 1;
static int busfreq_force_mode = BUSFREQ_FORCE_MODE_NONE;

static ssize_t enable_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	return sprintf(buf, "%u\n", busfreq_enabled != 0);
}

static ssize_t enable_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	int ret;
	bool val;

	ret = strtobool(buf, &val);
	if (ret < 0)
		return ret;

	busfreq_enabled = val;

	return count;
}

static struct kobj_attribute enable_attr =  __ATTR_RW(enable);

static ssize_t force_mode_show(struct kobject *kobj,
	struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", busfreq_mode_strs[busfreq_force_mode]);
}

static ssize_t force_mode_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret = -EINVAL;
	int i;

	for (i = 0; i < BUSFREQ_FORCE_MODE_MAX; i++)
		if (!strncmp(buf, busfreq_mode_strs[i], count - 1)) {
			busfreq_force_mode = i;
			ret = 0;
			break;
		}

	return ret ? ret : count;
}

static struct kobj_attribute force_mode_attr =  __ATTR_RW(force_mode);

static struct attribute *busfreq_attrs[] = {
	&enable_attr.attr,
	&force_mode_attr.attr,
	NULL
};

static struct attribute_group busfreq_attr_group = {
	.attrs = busfreq_attrs,
};

static int rtd129x_busfreq_get_cur_freq(struct device *dev, unsigned long *freq)
{
	struct busfreq_priv *priv = dev_get_drvdata(dev);

	*freq = priv->curr_freq;
	return 0;
}

static int rtd129x_busfreq_target(struct device *dev, unsigned long *freq,
	u32 flags)
{
	struct busfreq_priv *priv = dev_get_drvdata(dev);
	int i;

	if (priv->curr_freq == *freq)
		return 0;

	if (*freq > BUSFREQ_VIRT_FREQ_LOW)
		*freq = BUSFREQ_VIRT_FREQ_HIGH;
	else
		*freq = BUSFREQ_VIRT_FREQ_LOW;

	priv->curr_freq = *freq;
	i = __is_freq_high(priv);

	busfreq_set_clk_rate(priv->clkd[0], i);
	busfreq_set_clk_rate(priv->clkd[1], i);

	return 0;
}

/* if DC_PC is used by multiple consumers, value of MON_NUM
 * will much less than clk_sysh
 */
static bool __delect_conflict(struct busfreq_priv *priv)
{
	unsigned long th = clk_get_rate(priv->ref);
	unsigned int ref_val = dc_pc_get_time(0);
	unsigned int polling = priv->devfreq->profile->polling_ms;

	th = th * polling / 1000;
	th = th / 10 * 9;

	if (th > ref_val) {
		pr_warn("disalbed. multiple DC_PC consumers!\n");
		return 1;
	}
	return 0;
}

static int rtd129x_busfreq_get_dev_status(struct device *dev,
	struct devfreq_dev_status *stat)
{
	struct busfreq_priv *priv = dev_get_drvdata(dev);
	unsigned dc_pc_load, cpu_load;
	int reset_load_cnt = 0;

	stat->current_frequency = priv->curr_freq;
	stat->busy_time  = 100;
	stat->total_time = 100;

	if (!busfreq_enabled)
		return 0;

	if (dc_pc_is_stopped()) {
		dc_pc_start();
		return 0;
	}

	dc_pc_stop();

	dc_pc_load = dc_pc_get_max_load();
	cpu_load = cpu_get_max_load();
	dev_dbg(dev, "load: dc_pc = %3d, cpu = %3d, cnt = %2d\n",
		dc_pc_load, cpu_load, priv->load_cnt);

	if (__delect_conflict(priv)) {
		busfreq_enabled = 0;
		return 0;
	}

	reset_load_cnt = 1;

	/* force mode */
	if (busfreq_force_mode == BUSFREQ_FORCE_MODE_LOW) {
		stat->busy_time = 0;
		goto done;
	} else if (busfreq_force_mode == BUSFREQ_FORCE_MODE_HIGH) {
		stat->busy_time = 100;
		goto done;
	}

	/* counter */
	if (__is_freq_high(priv)) {
		/* reset cnt if both loads are high */
		reset_load_cnt = dc_pc_load >= 65 && cpu_load >= 35;

		if (dc_pc_load < 55 && cpu_load < 25)
			priv->load_cnt++;

		if (priv->load_cnt > 30)
			stat->busy_time = 0;
	} else {
		reset_load_cnt = 1;
		stat->busy_time = (dc_pc_load > 100 || cpu_load > 35) ?
			100 : 0;
	}

done:
	if (reset_load_cnt)
		priv->load_cnt = 0;

	dc_pc_start();

	return 0;
}

static unsigned int freq_table[BUSFREQ_MAX_STATE] = {
	BUSFREQ_VIRT_FREQ_LOW, BUSFREQ_VIRT_FREQ_HIGH
};

static struct devfreq_dev_profile rtd129x_busfreq_profile = {
	.target         = rtd129x_busfreq_target,
	.get_dev_status = rtd129x_busfreq_get_dev_status,
	.get_cur_freq   = rtd129x_busfreq_get_cur_freq,
	.polling_ms     = 1000,
	.initial_freq   = BUSFREQ_VIRT_FREQ_HIGH,
	.freq_table     = freq_table,
	.max_state      = BUSFREQ_MAX_STATE,
};

#ifdef CONFIG_PM
static int rtd129x_busfreq_suspend(struct device *dev)
{
	struct busfreq_priv *priv = dev_get_drvdata(dev);
	int ret = 0;

	dev_info(dev, "[BUSFREQ] %s enter\n", __func__);
#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY) {
		priv->curr_freq = BUSFREQ_VIRT_FREQ_LOW;
		busfreq_set_clk_rate(priv->clkd[0], BUSFREQ_STATE_LOW);
		busfreq_set_clk_rate(priv->clkd[1], BUSFREQ_STATE_LOW);
		goto done;
	}
#endif

done:
	dev_info(dev, "[BUSFREQ] %s exit\n", __func__);
	return ret;
}

static int rtd129x_busfreq_resume(struct device *dev)
{
	struct busfreq_priv *priv = dev_get_drvdata(dev);
	int ret = 0;

	dev_info(dev, "[BUSFREQ] %s enter\n", __func__);
#ifdef CONFIG_SUSPEND
	if (RTK_PM_STATE == PM_SUSPEND_STANDBY) {
		priv->curr_freq = BUSFREQ_VIRT_FREQ_HIGH;
		busfreq_set_clk_rate(priv->clkd[0], BUSFREQ_STATE_HIGH);
		busfreq_set_clk_rate(priv->clkd[1], BUSFREQ_STATE_HIGH);
		goto done;
	}
#endif

done:
	dev_info(dev, "[BUSFREQ] %s exit\n", __func__);
	return ret;
}
#endif

static const struct dev_pm_ops rtd129x_busfreq_pm_ops = {
	SET_LATE_SYSTEM_SLEEP_PM_OPS(rtd129x_busfreq_suspend,
		rtd129x_busfreq_resume)
};

static int rtd129x_busfreq_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node, *child;
	struct busfreq_priv *priv;
	int ret;
	int i;

	dev_info(dev, "[BUSFREQ] %s\n", __func__);

	dc_pc_init();

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->curr_freq = rtd129x_busfreq_profile.initial_freq;
	priv->ref = devm_clk_get(dev, NULL);

	platform_set_drvdata(pdev, priv);

	/* virtual freqs */
	dev_pm_opp_add(dev, BUSFREQ_VIRT_FREQ_HIGH, 0);
	dev_pm_opp_add(dev, BUSFREQ_VIRT_FREQ_LOW, 0);

	/* The DC_PC maximum is limited by 32-bit register,
	 * so possible maximum polling time is 9 sec
	 */
	of_property_read_u32(np, "polling-ms",
		&rtd129x_busfreq_profile.polling_ms);

	if (of_property_read_u32(np, "enable", &busfreq_enabled))
		busfreq_enabled = 0;

	i = 0;
	for_each_child_of_node(np, child) {
		if (i >= BUSFREQ_MAX_CLK) {
			dev_err(dev, "too many child node\n");
			break;
		}
		priv->clkd[i] = of_busfreq_clk_init(child);
		i++;
	}

	priv->devfreq = devm_devfreq_add_device(dev, &rtd129x_busfreq_profile,
		"simple_ondemand", NULL);
	if (IS_ERR(priv->devfreq))
		return PTR_ERR(priv->devfreq);

	ret = sysfs_create_group(&dev->kobj, &busfreq_attr_group);
	if (ret)
		dev_warn(dev, "cannot create sysfs group. err:%d\n", ret);

	return 0;
}


static const struct of_device_id rtd129x_busfreq_ids[] = {
	{ .compatible = "realtek,rtd129x-busfreq", },
	{}
};

static struct platform_driver rtd129x_busfreq_driver = {
	.driver = {
		.name  = "rtd129x-busfreq",
		.of_match_table = rtd129x_busfreq_ids,
		.pm = &rtd129x_busfreq_pm_ops,
	},
	.probe = rtd129x_busfreq_probe,
};

static int __init rtd129x_busfreq_init(void)
{
	return platform_driver_register(&rtd129x_busfreq_driver);
}
late_initcall(rtd129x_busfreq_init);

