/*
 * rtk_cpu_core_cooling.c - cooling device by CPU core control with Hotplug
 *
 * Copyright (C) 2016, Realtek Semiconductor Corporation
 *
 * Author: Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include <linux/core_control.h>
#include <linux/cpumask.h>
#include <linux/thermal.h>
#include <linux/slab.h>
#include <linux/delay.h>

static inline unsigned int cpumask_last(const struct cpumask *srcp)
{
	return find_last_bit(cpumask_bits(srcp), nr_cpumask_bits);
}


struct cpu_core_cooling_priv {
	int id;
	struct cpumask available_cpus;
	struct thermal_cooling_device *cdev;
	struct core_controller *cc;
	struct core_controller *last_cc;
	unsigned long state;
	int off_cnt;
};

#define cdev_to_priv(_cdev) ((_cdev)->devdata)
static DEFINE_IDR(cpu_core_cooling_idr);
static DEFINE_MUTEX(cpu_core_cooling_lock);

static int get_idr(struct idr *idr, int *id)
{
	int ret;

	mutex_lock(&cpu_core_cooling_lock);
	ret = idr_alloc(idr, NULL, 0, 0, GFP_KERNEL);
	mutex_unlock(&cpu_core_cooling_lock);
	if (unlikely(ret < 0))
		return ret;
	*id = ret;

	return 0;
}

static void release_idr(struct idr *idr, int id)
{
	mutex_lock(&cpu_core_cooling_lock);
	idr_remove(idr, id);
	mutex_unlock(&cpu_core_cooling_lock);
}

static int cpu_core_cooling_get_cur_state(struct thermal_cooling_device *cdev,
	unsigned long *state)
{
	struct cpu_core_cooling_priv *priv = cdev_to_priv(cdev);
	struct cpumask curr;
	cpumask_andnot(&curr, &priv->available_cpus, cpu_online_mask);
	*state = cpumask_weight(&curr);
	return 0;
}

static int cpu_core_cooling_set_cur_state(struct thermal_cooling_device *cdev,
	unsigned long state)
{
	struct cpu_core_cooling_priv *priv = cdev_to_priv(cdev);
	struct cpumask curr;
	unsigned long curr_state;
	int cpu;
	int ret = 0;

	cpu_core_cooling_get_cur_state(cdev, &curr_state);
	priv->state = state;

	if (priv->off_cnt == 0) {
		struct core_controller *ccc = core_control_get_token_owner();
		if (ccc != priv->cc) {
			priv->last_cc = ccc;
			core_control_set_token_owner(priv->cc);
		}
	}

	pr_info("cpu_core_cooling: state %d -> %d\n", (int)curr_state, (int)state);
	if (state > curr_state) {
		while (state != curr_state) {
			cpumask_and(&curr, &priv->available_cpus, cpu_online_mask);
			cpu = cpumask_last(&curr);

			ret = core_control_set_cpu_offline(priv->cc, cpu);
			if (ret)
				break;

			priv->off_cnt ++;

			usleep_range(1, 2);
			cpu_core_cooling_get_cur_state(cdev, &curr_state);
		}
	} else if (state < curr_state) {
		while (state != curr_state && priv->off_cnt > 0) {
			cpumask_andnot(&curr, &priv->available_cpus, cpu_online_mask);
			cpu = cpumask_first(&curr);

			ret = core_control_set_cpu_online(priv->cc, cpu);
			if (ret)
				break;

			priv->off_cnt --;

			usleep_range(1, 2);

			cpu_core_cooling_get_cur_state(cdev, &curr_state);
		}
	}

	if (priv->off_cnt == 0) {
		if (priv->last_cc != NULL)
			core_control_set_token_owner(priv->last_cc);
	}

	return ret;
}

static int cpu_core_cooling_get_max_state(struct thermal_cooling_device *cdev,
	unsigned long *state)
{
	struct cpu_core_cooling_priv *priv = cdev_to_priv(cdev);
	*state = (unsigned long)cpumask_weight(&priv->available_cpus);
	return 0;
}

static struct thermal_cooling_device_ops cdev_ops = {
	.get_max_state = cpu_core_cooling_get_max_state,
	.get_cur_state = cpu_core_cooling_get_cur_state,
	.set_cur_state = cpu_core_cooling_set_cur_state,
};

struct thermal_cooling_device *cpu_core_cooling_register(struct cpumask *cpus)
{
	int ret;
	char dev_name[THERMAL_NAME_LENGTH];
	struct cpu_core_cooling_priv *priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return ERR_PTR(-ENOMEM);

	ret = get_idr(&cpu_core_cooling_idr, &priv->id);
	if (ret)
		goto free_priv;

	cpumask_copy(&priv->available_cpus, cpus);

	snprintf(dev_name, sizeof(dev_name), "thermal-cpu-core-%d", priv->id);

	priv->last_cc = NULL;
	priv->cc   = core_control_register_controller("thermal");
	priv->cdev = thermal_cooling_device_register(dev_name, priv, &cdev_ops);

	if (IS_ERR(priv->cdev)) {
		ret = PTR_ERR(priv->cdev);
		goto remove_idr;
	}

	return priv->cdev;

remove_idr:
	release_idr(&cpu_core_cooling_idr, priv->id);
free_priv:
	kfree(priv);

	return ERR_PTR(ret);
}

int cpu_core_cooling_unregister(struct thermal_cooling_device *cdev)
{
	struct cpu_core_cooling_priv *priv = cdev_to_priv(cdev);

	thermal_cooling_device_unregister(cdev);

	core_control_unregister_controller(priv->cc);

	release_idr(&cpu_core_cooling_idr, priv->id);

	kfree(priv);

	return 0;
}
