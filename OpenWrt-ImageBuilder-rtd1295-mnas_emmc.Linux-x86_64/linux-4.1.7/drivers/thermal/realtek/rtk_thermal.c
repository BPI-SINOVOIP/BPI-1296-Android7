/*
 *  Copyright (C) 2015-2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#define pr_fmt(fmt) "thermal: " fmt

#include <linux/device.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/thermal.h>
#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/cpu_cooling.h>
#include <linux/slab.h>
#include <linux/core_control.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/pm.h>

#include "rtk_thermal.h"
#include "rtk_thermal_sensor.h"

static inline int _ABS(int x)
{
	return x >= 0 ? x : -x;
}

static int rtk_thermal_get_temp(struct thermal_zone_device *tz, unsigned long *temp)
{
	struct rtk_thermal_priv *priv = tz->devdata;
	int t = 0;
	int t_cr;

	t = thermal_sensor_get_temp(priv->reg);
	if (t < -3000 || t > 150000 || thermal_sensor_check_status_error(priv->reg)) {

		t_cr = thermal_sensor_get_temp(priv->reg_cr);
		if (_ABS(t_cr - t) < 10)
			goto done;

		pr_err("reset thermal sensor\n");
		thermal_sensor_dump_reg(priv->reg, "SCPU");
		thermal_sensor_dump_reg(priv->reg_cr, "CR");

		thermal_sensor_hw_reset(priv->reg);
		thermal_sensor_hw_reset(priv->reg_cr);

		return -EAGAIN;
	}

done:
	*(long *)temp = t;

	return 0;
}

static int rtk_thermal_get_trip_type(struct thermal_zone_device *tz,
		int trip, enum thermal_trip_type *type)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	if (trip >= priv->n_trips)
		return -EINVAL;

	*type = priv->trips[trip].type;

	return 0;
}


static int rtk_thermal_get_trip_temp(struct thermal_zone_device *tz,
		int trip, unsigned long *temp)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	if (trip >= priv->n_trips)
		return -EINVAL;

	*temp = priv->trips[trip].temp;

	return 0;
}

static int rtk_thermal_set_trip_temp(struct thermal_zone_device *tz,
		int trip, unsigned long temp)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	if (trip >= priv->n_trips)
		return -EINVAL;

	priv->trips[trip].temp = (int)temp;

	return 0;
}

static int rtk_thermal_get_trip_hyst(struct thermal_zone_device *tz,
		int trip, unsigned long *hyst)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	if (trip >= priv->n_trips)
		return -EINVAL;

	*hyst = priv->trips[trip].hyst;

	return 0;
}

static int rtk_thermal_set_trip_hyst(struct thermal_zone_device *tz,
		int trip, unsigned long hyst)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	if (trip >= priv->n_trips)
		return -EINVAL;

	priv->trips[trip].hyst = (int)hyst;

	return 0;
}

static int rtk_thermal_get_trend(struct thermal_zone_device *tz,
		int trip, enum thermal_trend *trend)
{
	struct rtk_thermal_priv *priv = tz->devdata;
	unsigned long trip_temp;
	int ret;

	rtk_thermal_eoh_handle_restore(priv);

	ret = rtk_thermal_get_trip_temp(tz, trip, &trip_temp);
	if (ret < 0)
		return ret;

	if (tz->temperature >= trip_temp)
		if (priv->flags & TREND_URGENT)
			*trend = THERMAL_TREND_RAISE_FULL;
		else
			*trend = THERMAL_TREND_RAISING;
	else
		if (priv->flags & TREND_URGENT)
			*trend = THERMAL_TREND_DROP_FULL;
		else
			*trend = THERMAL_TREND_DROPPING;

	rtk_thermal_eoh_handle_overheat(priv, trip, *trend);

	if (tz->temperature >= trip_temp) {
		dev_dbg(&tz->device, "temperature(%d C) tripTemp(%lu C) \n", tz->temperature/1000, trip_temp/1000);
	}

	return 0;
}

static enum rtk_cdev_id rtk_thermal_get_cdev_id(struct thermal_cooling_device *cdev)
{
	if (!strncmp(cdev->type, "thermal-cpufreq", 15))
		return CDEV_CPUFREQ;

	if (!strncmp(cdev->type, "thermal-cpu-core", 16))
		return CDEV_CPU_CORE_CTRL;

	if (!strncmp(cdev->type, "thermal-devfreq-0", 17))
		return CDEV_DEVFREQ_GPU;

	return CDEV_INVALID;
}


static int rtk_thermal_bind(struct thermal_zone_device *tz,
		struct thermal_cooling_device *cdev)
{
	struct rtk_thermal_priv *priv = tz->devdata;
	unsigned long upper;
	unsigned long lower;
	int ret;
	int i;
	enum rtk_cdev_id cdev_id;

	cdev_id = rtk_thermal_get_cdev_id(cdev);

	for (i = 0; i < priv->n_trips; i++) {
		if (priv->trips[i].cdev_id != cdev_id)
			continue;

		switch (cdev_id) {
		case CDEV_CPUFREQ:
			lower = RTK_FREQ_GET_LOWER(priv->freqs, priv->trips[i].cdev_idx);
			upper = RTK_FREQ_GET_UPPER(priv->freqs, priv->trips[i].cdev_idx);
			break;

		case CDEV_CPU_CORE_CTRL:
			lower = 1;
			cdev->ops->get_max_state(cdev, &upper);
			break;

		case CDEV_DEVFREQ_GPU:
			lower = upper =  priv->trips[i].cdev_idx;
			break;

		default:
			lower = upper = 0;
			WARN_ON(1);
			break;
		}

		ret = thermal_zone_bind_cooling_device(tz, i, cdev, upper, lower, THERMAL_WEIGHT_DEFAULT);
		dev_info(&cdev->device, "bind %s to TP%d: %lu -> %lu, ret = %d\n", cdev->type, i,
			lower, upper, ret);
	}

	return 0;
}

static int rtk_thermal_unbind(struct thermal_zone_device *tz,
		struct thermal_cooling_device *cdev)
{
	struct rtk_thermal_priv *priv = tz->devdata;
	int ret = 0;
	int i;
	enum rtk_cdev_id cdev_id;

	cdev_id = rtk_thermal_get_cdev_id(cdev);

	for (i = 0; i < priv->n_trips; i++) {
		if (priv->trips[i].cdev_id != cdev_id)
			continue;

		ret = thermal_zone_unbind_cooling_device(tz, i, cdev);
		if (ret)
			dev_err(&cdev->device, "Failed to unbind %s/TP%d\n",  cdev->type, i);
	}

	return ret;
}

static int rtk_thermal_get_mode(struct thermal_zone_device *tz,
		enum thermal_device_mode *mode)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	*mode = priv->mode;

	return 0;
}

static int rtk_thermal_set_mode(struct thermal_zone_device *tz,
		enum thermal_device_mode mode)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	mutex_lock(&tz->lock);

	if (mode == THERMAL_DEVICE_ENABLED) {
		tz->polling_delay = priv->monitoringRateMs;
	} else {
		tz->polling_delay = 0;
	}

	mutex_unlock(&tz->lock);

	priv->mode = mode;

	thermal_zone_device_update(priv->tz_dev);
	dev_info(&tz->device, "thermal polling set for duration = %d msec\n",
			priv->tz_dev->polling_delay);

	if (mode == THERMAL_DEVICE_DISABLED)
		thermal_zone_reset_cdev(tz);

	return 0;
}

static __maybe_unused int rtk_thermal_get_crit_temp(struct thermal_zone_device *tz,
		unsigned long *temp)
{
	struct rtk_thermal_priv *priv = tz->devdata;

	/* shutdown zone */
	return rtk_thermal_get_trip_temp(tz, priv->crit_idx, temp);
}

static struct thermal_zone_device_ops rtk_tz_ops = {
	.get_temp       = rtk_thermal_get_temp,
	.get_trend      = rtk_thermal_get_trend,
	.bind           = rtk_thermal_bind,
	.unbind         = rtk_thermal_unbind,
	.get_mode       = rtk_thermal_get_mode,
	.set_mode       = rtk_thermal_set_mode,
	.get_trip_type  = rtk_thermal_get_trip_type,
	.get_trip_temp  = rtk_thermal_get_trip_temp,
	.get_trip_hyst  = rtk_thermal_get_trip_hyst,
	.set_trip_temp  = rtk_thermal_set_trip_temp,
	.set_trip_hyst  = rtk_thermal_set_trip_hyst,
#ifdef CONFIG_THERMAL_HWMON
	.get_crit_temp  = rtk_thermal_get_crit_temp,
#endif
};

static ssize_t limit_cpu_show(struct kobject *kobj, struct kobj_attribute *attr,
	char *buf)
{
	struct rtk_thermal_priv *priv = container_of(attr, struct rtk_thermal_priv, limit_cpu_attr);

	return sprintf(buf, "%u\n", priv->bind_limit_cpu == 1);
}

static ssize_t limit_cpu_store(struct kobject *kobj, struct kobj_attribute *attr,
	const char *buf, size_t count)
{
	struct rtk_thermal_priv *priv = container_of(attr, struct rtk_thermal_priv, limit_cpu_attr);
	struct thermal_zone_device *tz = priv->tz_dev;
	struct thermal_cooling_device *cdev = priv->cdev_cpufreq;
	bool val;
	int ret;
	int i;

	ret = strtobool(buf, &val);

	if (val && priv->bind_limit_cpu == 0) {
		int cnt = 0;

		for (i = 0; i < priv->n_trips; i++) {
			unsigned long upper;
			unsigned long lower;

			if (priv->trips[i].cdev_id != CDEV_CPUFREQ_DUMMY)
				continue;

			lower = RTK_FREQ_GET_LOWER(priv->freqs, priv->trips[i].cdev_idx);
			upper = RTK_FREQ_GET_UPPER(priv->freqs, priv->trips[i].cdev_idx);

			ret = thermal_zone_bind_cooling_device(tz, i, cdev,
				upper, lower, THERMAL_WEIGHT_DEFAULT);
			dev_info(&cdev->device, "bind %s to TP%d: %lu -> %lu, ret = %d\n", cdev->type, i,
				lower, upper, ret);
			if (!ret)
				cnt ++;
		}

		if (cnt)
			priv->bind_limit_cpu = 1;

	} else if (!val && priv->bind_limit_cpu == 1) {

		/* prepare for unbind */
		for (i = 0; i < priv->n_trips; i++) {
			struct thermal_instance *ins;

			if (priv->trips[i].cdev_id != CDEV_CPUFREQ_DUMMY)
				continue;

			 ins = get_thermal_instance(tz, cdev, i);
			 ins->target = THERMAL_NO_TARGET;
			 cdev->updated = false;
		}
		thermal_cdev_update(cdev);

		/* unbind */
		for (i = 0; i < priv->n_trips; i++) {
			if (priv->trips[i].cdev_id != CDEV_CPUFREQ_DUMMY)
				continue;

			ret = thermal_zone_unbind_cooling_device(tz, i, cdev);
			if (ret)
				dev_err(&cdev->device, "Failed to unbind %s/TP%d\n",  cdev->type, i);
		}

		priv->bind_limit_cpu = 0;
	}

	return count;
}

#define INIT_SYSFS_ATTR_RW(_tar, _name) \
do { \
	struct kobj_attribute tmp = __ATTR_RW(_name); \
	_tar = tmp; \
} while (0);


void modifyFlags(struct rtk_thermal_priv *priv, unsigned int value, enum FlagMode mode)
{
	switch (mode) {
		case SET:
			priv->flags |= value;
			break;
		case CLEAR:
			priv->flags &= ~value;
			break;
		case ASSIGN:
			priv->flags = value;
			break;
		default:
			THERMAL_ERROR("[%s] value = 0x%x, mode = %d",__FUNCTION__,value,mode);
	}
}

static int rtk_thermal_parse_dt(struct rtk_thermal_priv *priv, struct platform_device *pdev)
{
	struct device_node *node = pdev->dev.of_node;
	const u32 *prop;
	int size;
	int i;
	int ret;

#define TRIP_POINT_COLUMN 4
	prop = of_get_property(node, "trip-points", &size);
	size /= sizeof(u32);
	if (prop && (size % TRIP_POINT_COLUMN) == 0) {
		priv->n_trips = size / TRIP_POINT_COLUMN;
		priv->crit_idx = -1;
		priv->trips = devm_kzalloc(&pdev->dev, sizeof(struct rtk_trip_point) * priv->n_trips, GFP_KERNEL);
		for (i = 0; i < priv->n_trips; i++) {
			priv->trips[i].temp     = of_read_number(prop++, 1) * 1000;
			priv->trips[i].hyst     = of_read_number(prop++, 1) * 1000;
			priv->trips[i].cdev_id  = (enum rtk_cdev_id)of_read_number(prop++, 1);
			priv->trips[i].cdev_idx = of_read_number(prop++, 1);
			priv->trips[i].type     = priv->trips[i].cdev_id == CDEV_SHUTDOWN ?
				THERMAL_TRIP_CRITICAL : THERMAL_TRIP_PASSIVE;

			if (priv->trips[i].type == THERMAL_TRIP_CRITICAL)
				 priv->crit_idx = i;
		}

		if (priv->crit_idx == -1)
			priv->crit_idx = i - 1;
	} else {
		priv->n_trips = 0;
		dev_err(&pdev->dev, "[%s] thermal-table ERROR! array_size = %d\n", __func__, size);
		return -EINVAL;
	}
#undef TRIP_POINT_COLUMN

	/* cpufreq cooling */
	prop = of_get_property(node, "cpufreq,freqs", &size);
	size /= sizeof(u32);
	if (size) {
		struct cpufreq_policy * policy = cpufreq_cpu_get(0);
		int max_lv;

		if (policy) {
			max_lv = cpufreq_frequency_table_get_index(policy, policy->cpuinfo.max_freq);
			priv->n_freqs = size;
			priv->freqs = devm_kzalloc(&pdev->dev, sizeof(struct rtk_freq) * priv->n_freqs, GFP_KERNEL);

			for (i = 0; i < priv->n_freqs; i++) {
				priv->freqs[i].freq = of_read_number(prop++, 1);
				priv->freqs[i].lv = max_lv - cpufreq_frequency_table_get_index(policy, priv->freqs[i].freq);
			}

			cpufreq_cpu_put(policy);
		}
	}


	/* cpu core cooling */
	ret = of_core_control_read_cpumask(pdev->dev.of_node,
		"cpu-core,cpu-list", &priv->core_control_mask);
	if (!ret)
		core_control_get_available_cpus(&priv->core_control_mask);


	prop = of_get_property(node, "thermal-polling-ms", &size);
	if (prop) {
		unsigned int temp = of_read_number(prop,1);
		priv->monitoringRateMs = temp;
	}
	else
		priv->monitoringRateMs = 500;

	prop = of_get_property(node, "thermal-trend-urgent", &size);
	if (prop) {
		unsigned int temp = of_read_number(prop,1);
		if (temp)
			modifyFlags(priv, TREND_URGENT, SET);
	}

	THERMAL_DEBUG("[%s] thermal flags is 0x%lx", __func__, priv->flags);

	return 0;
}

static int rtk_thermal_core_control_notifier(struct notifier_block *nb,
	unsigned long event, void *data)
{
	struct rtk_thermal_priv *priv = container_of(nb, struct rtk_thermal_priv, cc_nb);

	if (event == CORE_CONTROL_ENABLE) {
		if (priv->cdev_cc != NULL) {
			pr_warning("CDEV_CPU_CORE_CTRL is already registered.\n");
			return NOTIFY_OK;
		}

		priv->cdev_cc = cpu_core_cooling_register(&priv->core_control_mask);
		if (IS_ERR_OR_NULL(priv->cdev_cc)) {
			pr_err("Failed to register CDEV_CPU_CORE_CTRL.\n");
			priv->cdev_cc = NULL;
		}
	} else if (event == CORE_CONTROL_DISABLE) {
		if (priv->cdev_cc == NULL) {
			pr_warning("CDEV_CPU_CORE_CTRL is already unregister.\n");
			return NOTIFY_OK;
		}

		cpu_core_cooling_unregister(priv->cdev_cc);
		priv->cdev_cc = NULL;
	}
	return NOTIFY_DONE;
}

static int rtk_thermal_resume(struct device *dev)
{
	struct thermal_zone_device *tz = dev_get_drvdata(dev);
	struct rtk_thermal_priv *priv = tz->devdata;

	dev_info(dev, "[THERMAL] Enter %s\n", __func__);

	thermal_sensor_hw_reset_no_delay(priv->reg);
	thermal_sensor_hw_reset_no_delay(priv->reg_cr);
	usleep_range(25000, 26000);

	dev_info(dev, "[THERMAL] Exit %s\n", __func__);
	return 0;
}

static const struct dev_pm_ops rtk_thermal_pm_ops = {
	.resume = rtk_thermal_resume,
};

static int rtk_thermal_probe(struct platform_device *pdev)
{
	struct device_node *node = NULL;
	struct rtk_thermal_priv *priv;
	int ret;
	int mask;

	dev_dbg(&pdev->dev, "%s", __func__);
	node = pdev->dev.of_node;
	if (!node) {
		return -ENODEV;
	}

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv) {
		return -ENOMEM;
	}

	priv->dev = &pdev->dev;
	priv->reg = of_iomap(node, 0);
	priv->reg_cr = of_iomap(node, 1);

	thermal_sensor_hw_reset_no_delay(priv->reg);
	thermal_sensor_hw_reset_no_delay(priv->reg_cr);
	usleep_range(25000, 26000);

	ret = rtk_thermal_parse_dt(priv, pdev);
	if (ret < 0) {
		dev_err(&pdev->dev, "Fail to parse device tree node.\n");
		return ret;
	}

	mask = BIT(priv->n_trips) - 1;
	priv->tz_dev = thermal_zone_device_register("rtk_thermal", priv->n_trips, mask,
				priv, &rtk_tz_ops, NULL, priv->monitoringRateMs, priv->monitoringRateMs);
	if (IS_ERR(priv->tz_dev)) {
		ret = PTR_ERR(priv->tz_dev);
		dev_err(&pdev->dev, "Failed to register thermal_zone device: %d\n", ret);
		return ret;
	}

	rtk_thermal_add_eoh(priv);

	/* cpufreq cooling */
	if (priv->freqs) {
		struct cpumask cpufreq_mask;

		cpumask_setall(&cpufreq_mask);
		priv->cdev_cpufreq = cpufreq_cooling_register(&cpufreq_mask);
		if (IS_ERR_OR_NULL(priv->cdev_cpufreq)) {
	        dev_err(&pdev->dev, "Failed to register CDEV_CPUFREQ.\n");
			priv->cdev_cpufreq = NULL;
		}
	}

	/* cpu core cooling */
	if (core_control_is_enabled()) {
		priv->cdev_cc = cpu_core_cooling_register(&priv->core_control_mask);
		if (IS_ERR_OR_NULL(priv->cdev_cc)) {
			dev_err(&pdev->dev, "Failed to register CDEV_CPU_CORE_CTRL.\n");
			priv->cdev_cc = NULL;
		}
	}
	priv->cc_nb.notifier_call = rtk_thermal_core_control_notifier;
	register_core_control_notifier(&priv->cc_nb);

	priv->mode = THERMAL_DEVICE_ENABLED;

	platform_set_drvdata(pdev, priv->tz_dev);

	INIT_SYSFS_ATTR_RW(priv->limit_cpu_attr, limit_cpu);
	ret = sysfs_create_file(&pdev->dev.kobj, &priv->limit_cpu_attr.attr);
	if (ret)
		pr_warn("Failed to sysfs_create_files(): %d\n", ret);

	dev_dbg(&pdev->dev, "%s done", __func__);
	return 0;
}


static int rtk_thermal_remove(struct platform_device *pdev)
{
	struct thermal_zone_device *tz = platform_get_drvdata(pdev);
	struct rtk_thermal_priv *priv = tz->devdata;

	sysfs_remove_file(&pdev->dev.kobj, &priv->limit_cpu_attr.attr);

	if (priv->cdev_cpufreq)
		cpufreq_cooling_unregister(priv->cdev_cpufreq);

	unregister_core_control_notifier(&priv->cc_nb);
	if (priv->cdev_cc)
		cpu_core_cooling_unregister(priv->cdev_cc);

	rtk_thermal_remove_eoh(priv);

	thermal_zone_device_unregister(priv->tz_dev);

	platform_set_drvdata(pdev, NULL);
	return 0;
}

static const struct of_device_id rtk_thermal_of_match[] = {
	{.compatible = "Realtek,rtd1295-thermal",},
	{ /* Sentinel */ },
};

static struct platform_driver rtk_thermal_driver = {
	.probe	= rtk_thermal_probe,
	.remove	= rtk_thermal_remove,
	.driver = {
		.name	= "realtek_thermal",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(rtk_thermal_of_match),
		.pm = &rtk_thermal_pm_ops,
	},
};

static int __init rtk_thermal_probe_init(void)
{
	return platform_driver_probe(&rtk_thermal_driver, rtk_thermal_probe);
}

late_initcall(rtk_thermal_probe_init);

