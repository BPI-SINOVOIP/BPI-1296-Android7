/*
 * g2237-regulator.c - GMT-G2237 Regulator driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 * Copyright (C) 2017 Cheng-Yu Lee <cylee12@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) "g2237: " fmt

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regmap.h>
#include <linux/suspend.h>
#include <linux/uaccess.h>
#include <linux/bitops.h>
#include <soc/realtek/rtk_regmap.h>
#include <dt-bindings/regulator/gmt,g2237.h>
#include "rtk-regulator.h"

struct pmic_device {
	struct i2c_client *client;
	struct device *dev;
	unsigned int num_regulators;
	struct regmap *regmap;
};

enum {
	G2237_ID_DC1 = 0,
	G2237_ID_DC2,
	G2237_ID_DC3,
	G2237_ID_DC4,
	G2237_ID_DC5,
	G2237_ID_LDO1,
};

#define IS_LDO(_desc) \
	((_desc)->id == G2237_ID_LDO1)
#define RF_INVALID -1

enum {
	RF_LPOFF_TO_DO = 0,
	RF_SOFTOFF,
	RF_DC4_ON,
	RF_DC4_NMODE,
	RF_DC4_SMODE,

	RF_DC1_ON,
	RF_DC1_NMODE,
	RF_DC1_SMODE,
	RF_DC1_NVO,
	RF_DC1_SVO,

	RF_DC2_ON,
	RF_DC2_NMODE,
	RF_DC2_SMODE,
	RF_DC2_NVO,
	RF_DC2_SVO,

	RF_DC3_ON,
	RF_DC3_NMODE,
	RF_DC3_SMODE,
	RF_DC3_NVO,
	RF_DC3_SVO,

	RF_DC5_ON,
	RF_DC5_NMODE,
	RF_DC5_SMODE,
	RF_DC5_NVO,
	RF_DC5_SVO,

	RF_LDO1_ON,
	RF_LDO1_NMODE,
	RF_LDO1_SMODE,
	RF_LDO1_NVO,
	RF_LDO1_SVO,

	RF_MAX
};

static const struct reg_field g2237_reg_fields[] __initconst = {
	[RF_LPOFF_TO_DO] = REG_FIELD(0x02, 7, 7),
	[RF_SOFTOFF] = REG_FIELD(0x04, 7, 7),
	[RF_DC1_ON] = REG_FIELD(0x05, 7, 7),
	[RF_DC2_ON] = REG_FIELD(0x05, 6, 6),
	[RF_DC3_ON] = REG_FIELD(0x05, 5, 5),
	[RF_DC4_ON] = REG_FIELD(0x05, 4, 4),
	[RF_DC5_ON] = REG_FIELD(0x05, 3, 3),
	[RF_LDO1_ON] = REG_FIELD(0x05, 0, 0),
	[RF_DC1_NMODE] = REG_FIELD(0x07, 6, 7),
	[RF_DC1_SMODE] = REG_FIELD(0x07, 4, 5),
	[RF_DC2_NMODE] = REG_FIELD(0x07, 2, 3),
	[RF_DC2_SMODE] = REG_FIELD(0x07, 0, 1),
	[RF_DC3_NMODE] = REG_FIELD(0x08, 6, 7),
	[RF_DC3_SMODE] = REG_FIELD(0x08, 4, 5),
	[RF_DC4_NMODE] = REG_FIELD(0x08, 2, 3),
	[RF_DC4_SMODE] = REG_FIELD(0x08, 0, 1),
	[RF_DC5_NMODE] = REG_FIELD(0x09, 6, 7),
	[RF_DC5_SMODE] = REG_FIELD(0x09, 4, 5),
	[RF_LDO1_NMODE] = REG_FIELD(0x09, 2, 3),
	[RF_LDO1_SMODE] = REG_FIELD(0x09, 0, 1),
	[RF_DC1_NVO] = REG_FIELD(0x0A, 0, 3),
	[RF_DC2_NVO] = REG_FIELD(0x0B, 0, 4),
	[RF_DC3_NVO] = REG_FIELD(0x0C, 0, 4),
	[RF_DC5_NVO] = REG_FIELD(0x0D, 0, 3),
	[RF_LDO1_NVO] = REG_FIELD(0x0E, 0, 3),
	[RF_DC1_SVO] = REG_FIELD(0x0F, 0, 3),
	[RF_DC2_SVO] = REG_FIELD(0x10, 0, 4),
	[RF_DC3_SVO] = REG_FIELD(0x11, 0, 4),
	[RF_DC5_SVO] = REG_FIELD(0x12, 0, 3),
	[RF_LDO1_SVO] = REG_FIELD(0x13, 0, 3),
};

static const unsigned int g2237_ldo1_vtbl[] = {
	2200000, 2300000, 2400000, 2500000,
	2600000, 2700000, 2800000, 2900000,
	3000000, 3000000, 3000000, 3000000,
	3000000, 3000000, 3000000, 3000000,
};

static const unsigned int g2237_dc1_vtbl[] = {
	2200000, 2300000, 2400000, 2500000,
	2600000, 2700000, 2800000, 2900000,
	3000000, 3100000, 3200000, 3300000,
	3400000, 3500000, 3600000, 3700000,
};

static const unsigned int g2237_dc2_vtbl[] = {
	 800000,  812500,  825000,  837500,
	 850000,  862500,  875000,  887500,
	 900000,  912500,  925000,  937500,
	 950000,  962500,  975000,  987500,
	1000000, 1012500, 1025000, 1037500,
	1050000, 1062500, 1075000, 1087500,
	1100000, 1112500, 1125000, 1137500,
	1150000, 1162500, 1175000, 1187500,
};

static const unsigned int g2237_dc4_vtbl[] = {
	1800000,
};

static const unsigned int g2237_dc5_vtbl[] = {
	 800000,  850000,  900000,  950000,
	1000000, 1050000, 1100000, 1200000,
	1300000, 1500000, 1600000, 1700000,
	1800000, 1900000, 2000000, 2500000,
};

static int g2237_regulator_set_mode_regmap(struct regulator_dev *rdev,
	unsigned int mode)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);
	unsigned int val = 0;

	if (IS_LDO(rdev->desc))
		val = (mode & REGULATOR_MODE_IDLE) ? 2 : 0;
	else
		val =  (mode & REGULATOR_MODE_FAST) ? 2 : 0;

	return regmap_field_write(gd->fields[RRF_NMODE], val);
}

static unsigned int g2237_regulator_get_mode_regmap(struct regulator_dev *rdev)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);
	unsigned int val;
	int ret;

	ret = regmap_field_read(gd->fields[RRF_NMODE], &val);
	if (ret)
		return 0;

	if (IS_LDO(rdev->desc) && val == 2)
		return REGULATOR_MODE_IDLE;
	else if (val == 2)
		return REGULATOR_MODE_FAST;
	return REGULATOR_MODE_NORMAL;
}

static struct regulator_ops g2237_regulator_ops  = {
	.list_voltage    = regulator_list_voltage_table,
	.map_voltage     = regulator_map_voltage_iterate,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.enable          = regulator_enable_regmap,
	.disable         = regulator_disable_regmap,
	.is_enabled      = regulator_is_enabled_regmap,
	.get_mode        = g2237_regulator_get_mode_regmap,
	.set_mode        = g2237_regulator_set_mode_regmap,
	.set_voltage_time_sel = rtk_regulator_set_voltage_time_sel,
	.set_suspend_voltage = rtk_regulator_set_suspend_voltage,
	.set_suspend_enable = rtk_regulator_set_suspend_enable,
	.set_suspend_disable = rtk_regulator_set_suspend_disable,
};

static unsigned int g2237_regulator_dc_of_map_mode(unsigned int mode)
{
	if (mode == G2237_DC_MODE_FORCE_PWM)
		return REGULATOR_MODE_FAST;
	return REGULATOR_MODE_NORMAL;
}

static unsigned int g2237_regulator_ldo_of_map_mode(unsigned int mode)
{
	if (mode == G2237_LDO_MODE_ECO)
		return REGULATOR_MODE_IDLE;
	 return REGULATOR_MODE_NORMAL;
}

#define COMMON_DESC(_name, _id, _tbl, _map_cb, _vdelay) \
{ \
	.desc = { \
		.owner       = THIS_MODULE, \
		.name        = _name, \
		.id          = _id, \
		.ops         = &g2237_regulator_ops, \
		.type        = REGULATOR_VOLTAGE, \
		.of_match    = _name, \
		.of_map_mode = _map_cb, \
		.of_parse_cb = rtk_regulator_of_parse_cb, \
		.volt_table  = _tbl, \
		.n_voltages  = ARRAY_SIZE(_tbl), \
	}, \
	.smode_enable_val = 0x1, \
	.smode_disable_val = 0x3, \
	.svo_smode_val = 0x2, \
	.volup_delay = _vdelay, \
	.flags = RTK_REGULATOR_SET_SVOLT_SMODE, \
}

#define DC_DESC(_name, _id, _tbl, _vdelay) \
	COMMON_DESC(_name, _id, _tbl, g2237_regulator_dc_of_map_mode, _vdelay)
#define LDO_DESC(_name, _id, _tbl, _vdelay) \
	COMMON_DESC(_name, _id, _tbl, g2237_regulator_ldo_of_map_mode, _vdelay)

static struct rtk_desc g2237_desc[] = {
	[G2237_ID_DC1] = DC_DESC("dc1",
		G2237_ID_DC1,
		g2237_dc1_vtbl,
		0),
	[G2237_ID_DC2] = DC_DESC("dc2",
		G2237_ID_DC2,
		g2237_dc2_vtbl,
		0),
	[G2237_ID_DC3] = DC_DESC("dc3",
		G2237_ID_DC3,
		g2237_dc2_vtbl,
		40),
	[G2237_ID_DC4] = DC_DESC("dc4",
		G2237_ID_DC4,
		g2237_dc4_vtbl,
		0),
	[G2237_ID_DC5] = DC_DESC("dc5",
		G2237_ID_DC5,
		g2237_dc5_vtbl,
		0),
	[G2237_ID_LDO1] = LDO_DESC("ldo2",
		G2237_ID_LDO1,
		g2237_ldo1_vtbl,
		0),
};

#define RRF_FULL(_p) \
{ \
	RF_ ##  _p ## _ON, \
	RF_ ##  _p ## _NMODE, \
	RF_ ##  _p ## _SMODE, \
	RF_ ##  _p ## _NVO, \
	RF_ ##  _p ## _SVO, \
}

#define RRF_NO_VOSEL(_p) \
{ \
	RF_ ##  _p ## _ON, \
	RF_ ##  _p ## _NMODE, \
	RF_ ##  _p ## _SMODE, \
	RF_INVALID, \
	RF_INVALID, \
}

/* map RRF_* to RF_* */
static int g2237_rf_map[][RRF_MAX] = {
	[G2237_ID_DC1] = RRF_FULL(DC1),
	[G2237_ID_DC2] = RRF_FULL(DC2),
	[G2237_ID_DC3] = RRF_FULL(DC3),
	[G2237_ID_DC4] = RRF_NO_VOSEL(DC4),
	[G2237_ID_DC5] = RRF_FULL(DC5),
	[G2237_ID_LDO1] = RRF_FULL(LDO1),
};

static struct regmap_field *g2237_fields[RF_MAX];

static const struct reg_default g2237_reg_defaults[] = {
	{ .reg = 0x02, .def = 0xC7, },
	{ .reg = 0x04, .def = 0x00, },
	/* on/off */
	{ .reg = 0x05, .def = 0xF9, },
	/* mode */
	{ .reg = 0x07, .def = 0x22, },
	{ .reg = 0x08, .def = 0x22, },
	{ .reg = 0x09, .def = 0x22, },
	/* nrm volt */
	{ .reg = 0x0A, .def = 0x0B, },
	{ .reg = 0x0B, .def = 0x10, },
	{ .reg = 0x0C, .def = 0x10, },
	{ .reg = 0x0D, .def = 0x0C, },
	{ .reg = 0x0E, .def = 0x03, },
	/* slp volt */
	{ .reg = 0x0F, .def = 0x0B, },
	{ .reg = 0x10, .def = 0x10, },
	{ .reg = 0x11, .def = 0x10, },
	{ .reg = 0x12, .def = 0x0C, },
	{ .reg = 0x13, .def = 0x03, },
};

static bool g2237_regulator_writeable_reg(struct device *dev, unsigned int reg)
{
	return reg != 0;
}

static bool g2237_regulator_precious_reg(struct device *dev, unsigned int reg)
{
	return reg == 0;
}

static const struct regmap_config g2237_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = 0x15,
	.cache_type   = REGCACHE_FLAT,
	.reg_defaults = g2237_reg_defaults,
	.num_reg_defaults = ARRAY_SIZE(g2237_reg_defaults),
	.writeable_reg = g2237_regulator_writeable_reg,
	.precious_reg = g2237_regulator_precious_reg,
};

static void g2237_regulator_pm_power_off(void)
{
	int ret;

	pr_err("[PMIC] %s: pm_power_off\n", __func__);
	ret = regmap_field_write(g2237_fields[RF_SOFTOFF], 0x1);
	pr_err("  returns %d\n", ret);
}

static int g2237_regulator_register(struct pmic_device *pmic_dev,
	struct rtk_desc *gd)
{
	struct regulator_config config = {};
	struct regulation_constraints *c;
	const struct reg_field *rf;
	int *ids = g2237_rf_map[gd->desc.id];
	int src;
	int i;

	/* using rf_map to get the relative regmap_field */
	for (i = 0; i < RRF_MAX; i++) {
		src = ids[i];
		if (src != RF_INVALID)
			gd->fields[i] = g2237_fields[src];
	}

	/* using regmap_field to fill desc */
	src = ids[RRF_ON];
	if (src != RF_INVALID) {
		rf = &g2237_reg_fields[src];
		gd->desc.enable_reg = rf->reg;
		gd->desc.enable_mask = GENMASK(rf->msb, rf->lsb);
	}

	/* using regmap_field to fill desc */
	src = ids[RRF_NVO];
	if (src != RF_INVALID) {
		rf = &g2237_reg_fields[src];
		gd->desc.vsel_reg = rf->reg;
		gd->desc.vsel_mask = GENMASK(rf->msb, rf->lsb);
	}

	config.dev = pmic_dev->dev;
	config.driver_data = gd;
	config.regmap = pmic_dev->regmap;

	gd->rdev = devm_regulator_register(pmic_dev->dev, &gd->desc, &config);
	if (IS_ERR(gd->rdev))
		return PTR_ERR(gd->rdev);

	c = gd->rdev->constraints;

	/* copy state_mem to gd for mem/coldboot switching. if there is no
	 * sleep uV and mode, set the state to enable
	 */
	gd->state_mem = c->state_mem;
	if (gd->state_mem.uV == 0 && gd->state_mem.disabled == 0)
		gd->state_mem.enabled = true;
	if (gd->state_coldboot.uV == 0 && gd->state_coldboot.disabled == 0)
		gd->state_coldboot.enabled = true;

	/* enable change mode */
	if (IS_LDO(&gd->desc))
		c->valid_modes_mask |= REGULATOR_MODE_NORMAL
			| REGULATOR_MODE_IDLE;
	else
		c->valid_modes_mask |= REGULATOR_MODE_NORMAL
			| REGULATOR_MODE_FAST;
	c->valid_ops_mask |= REGULATOR_CHANGE_MODE;

	return 0;
}

static int g2237_regulator_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct pmic_device *pmic_dev;
	struct regmap *regmap;
	int i, ret;

	dev_info(dev, "[PMIC] %s\n", __func__);

	pmic_dev = devm_kzalloc(dev, sizeof(*pmic_dev), GFP_KERNEL);
	if (!pmic_dev)
		return -ENOMEM;

	regmap = devm_rtk_regmap_i2c_init(dev, client, &g2237_regmap_config);
	if (IS_ERR(regmap))
		return -EINVAL;

	/* after regmap is ready, convert all reg_field to regmap_field */
	for (i = 0; i < RF_MAX; i++) {
		struct regmap_field *rf;

		rf = devm_regmap_field_alloc(dev, regmap, g2237_reg_fields[i]);
		if (IS_ERR(rf)) {
			ret = PTR_ERR(rf);
			dev_err(dev, "Failed to alloc regmap field %d: %d\n",
				i, ret);
			return ret;
		}
		g2237_fields[i] = rf;
	}

	pmic_dev->regmap = regmap;
	pmic_dev->client = client;
	pmic_dev->dev    = dev;
	pmic_dev->num_regulators = ARRAY_SIZE(g2237_desc);

	for (i = 0; i < ARRAY_SIZE(g2237_desc); i++) {
		ret = g2237_regulator_register(pmic_dev, &g2237_desc[i]);
		if (ret) {
			dev_err(dev, "Failed to register %s: %d\n",
				rtk_desc_name(&g2237_desc[i]), ret);
			return ret;
		}
	}

	i2c_set_clientdata(client, pmic_dev);

	if (of_device_is_system_power_controller(client->dev.of_node)) {
		dev_info(dev, "is system-power-controller\n");
		pm_power_off = g2237_regulator_pm_power_off;
	}

	return 0;
}

static inline int get_suspend_mode(void)
{
	WARN_ON(1);
	return 0;
}

static int g2237_regulator_suspend(struct device *dev)
{
	int mode;
	int i;

	dev_info(dev, "[PMIC] Enter %s\n", __func__);

	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		goto done;

	mode = get_suspend_mode();

	for (i = 0; i < ARRAY_SIZE(g2237_desc); i++) {
		rtk_regulator_prepare_suspend_state(g2237_desc[i].rdev,
			mode);
	};

	regulator_suspend_prepare(PM_SUSPEND_MEM);
done:
	dev_info(dev, "[PMIC] Exit %s\n", __func__);
	return 0;
}

static int g2237_regulator_resume(struct device *dev)
{
	dev_info(dev, "[PMIC] Enter %s\n", __func__);
	dev_info(dev, "[PMIC] Exit %s\n", __func__);
	return 0;
}


static const struct dev_pm_ops g2237_regulator_pm_ops = {
	.suspend = g2237_regulator_suspend,
	.resume = g2237_regulator_resume,
};

static const struct i2c_device_id g2237_regulator_ids[] = {
	{"gmt-g2237", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, g2237_regulator_ids);

static struct i2c_driver g2237_regulator_driver = {
	.driver = {
		.name = "gmt-g2237",
		.owner = THIS_MODULE,
		.pm = &g2237_regulator_pm_ops,
	},
	.id_table = g2237_regulator_ids,
	.probe    = g2237_regulator_probe,
};
module_i2c_driver(g2237_regulator_driver);
