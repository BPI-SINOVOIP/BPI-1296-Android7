/*
 * rtk_cpu_spif.c - CPU Sustained Performance Interface
 *
 * Copyright (C) 2017, Realtek Semiconductor Corporation
 * Copyright (C) 2017, Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) "spif: " fmt

#include <linux/cpufreq.h>
#include <linux/cpumask.h>
#include <linux/kobject.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/platform_device.h>
#include <linux/sysfs.h>
#include <linux/uaccess.h>
#include <soc/realtek/rtk_cpu_sysfs.h>

#define RTK_SPIF_EVENT_DISABLE     0
#define RTK_SPIF_EVENT_ENABLE      1
#define RTK_SPIF_EVENT_BOOST_SET   2
#define RTK_SPIF_EVENT_BOOST_CLEAR 3

static int spif_notifier(struct notifier_block *nb, unsigned long action,
	void *data);

static int spif_enable;
static int spif_enable_changed;
static int spif_boost;
static int spif_boost_changed;
static struct kobject *spif_kobj;
static struct notifier_block spif_nb = {
	.notifier_call = spif_notifier,
};
static DEFINE_MUTEX(spif_lock);

static int spif_notifier(struct notifier_block *nb, unsigned long action,
	void *data)
{
	struct cpufreq_policy *policy = data;
	enum { BOOST, RESET, IGNORE } react;

	if (action != CPUFREQ_ADJUST)
		return NOTIFY_DONE;

	if (!spif_enable_changed && !spif_boost_changed)
		return NOTIFY_DONE;

	pr_debug("%s: act=%lu, gov=%s\n", __func__, action,
		policy->governor->name);

	if (spif_enable)
		react = spif_boost ? BOOST : RESET;
	else
		react = spif_enable_changed ? RESET : IGNORE;

	if (react == IGNORE)
		return NOTIFY_DONE;

	if (react == BOOST) {
		cpufreq_verify_within_limits(policy, policy->cpuinfo.max_freq,
			policy->max);
	} else {
		cpufreq_verify_within_limits(policy, policy->cpuinfo.min_freq,
			policy->max);
	}

	return NOTIFY_OK;
}

static void update_all_policies(void)
{
	struct cpumask cpus;
	int cpu;
	struct cpufreq_policy *policy;

	cpumask_clear(&cpus);
	for_each_online_cpu(cpu) {
		if (cpumask_test_cpu(cpu, &cpus))
			continue;

		/* acquire cpu dirver */
		policy = cpufreq_cpu_get(cpu);
		if (!policy)
			continue;

		cpufreq_update_policy(cpu);

		/* mark the all cpus shared the policy */
		cpumask_or(&cpus, &cpus, policy->cpus);

		/* release the cpu driver */
		cpufreq_cpu_put(policy);
	}
}

static void set_event(int event)
{
	int en = spif_enable;
	int bo = spif_boost;

	mutex_lock(&spif_lock);

	switch (event) {
	case RTK_SPIF_EVENT_ENABLE:
		spif_enable = 1;
		break;

	case RTK_SPIF_EVENT_DISABLE:
		spif_enable = 0;
		break;
	case RTK_SPIF_EVENT_BOOST_SET:
		spif_boost = 1;
		break;

	case RTK_SPIF_EVENT_BOOST_CLEAR:
		spif_boost = 0;
		break;
	}

	spif_enable_changed = (en != spif_enable);
	spif_boost_changed = (bo != spif_boost);

	if (spif_enable_changed || spif_boost_changed) {
		update_all_policies();
		spif_enable_changed = 0;
		spif_boost_changed = 0;
	}

	mutex_unlock(&spif_lock);
}

static ssize_t spif_enable_show(struct kobject *kobj,
	struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", spif_enable != 0);
}

static ssize_t spif_enable_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	bool val;

	ret = strtobool(buf, &val);
	if (ret)
		return ret;

	set_event(val ? RTK_SPIF_EVENT_ENABLE : RTK_SPIF_EVENT_DISABLE);

	return count;
}

static struct kobj_attribute spif_enable_attr =  __ATTR_RW(spif_enable);

static ssize_t spif_boost_show(struct kobject *kobj,
	struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", spif_boost != 0);
}

static ssize_t spif_boost_store(struct kobject *kobj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	int ret;
	bool val;

	ret = strtobool(buf, &val);
	if (ret)
		return ret;

	set_event(val ? RTK_SPIF_EVENT_BOOST_SET : RTK_SPIF_EVENT_BOOST_CLEAR);

	return count;
}

static struct kobj_attribute spif_boost_attr =  __ATTR_RW(spif_boost);

static struct attribute *spif_attrs[] = {
	&spif_enable_attr.attr,
	&spif_boost_attr.attr,
	NULL
};

static struct attribute_group spif_attr_group = {
	.attrs = spif_attrs,
};


static int __init rtk_spif_probe(struct platform_device *pdev)
{
	int ret;
	struct device *dev = &pdev->dev;

	dev_info(dev, "[SPIF] %s\n", __func__);

	if (!kernel_cpu_kobj)
		return -ENOENT;

	ret = sysfs_create_group(kernel_cpu_kobj, &spif_attr_group);
	if (ret) {
		dev_err(&pdev->dev, "failed to create sysfs files\n");
		return ret;
	}

	ret = cpufreq_register_notifier(&spif_nb, CPUFREQ_POLICY_NOTIFIER);
	if (ret) {
		dev_err(&pdev->dev, "failed to register cpufreq notifier\n");
		goto remove_group;
	}

	return 0;

remove_group:
	sysfs_remove_group(spif_kobj, &spif_attr_group);

	return ret;
}

static const struct of_device_id rtk_spif_match[] = {
	{.compatible = "realtek,cpu-spif"},
	{}
};

static struct platform_driver rtk_spif_driver = {
	.probe = rtk_spif_probe,
	.driver = {
		.name = "rtk-spif",
		.of_match_table = rtk_spif_match,
	},
};

static int __init rtk_spif_init(void)
{
	return platform_driver_register(&rtk_spif_driver);
}
late_initcall(rtk_spif_init);
