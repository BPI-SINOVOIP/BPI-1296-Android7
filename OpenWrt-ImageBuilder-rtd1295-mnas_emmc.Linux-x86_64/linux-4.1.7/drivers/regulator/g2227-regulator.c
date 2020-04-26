/*
 * g2227-regulator.c - GMT-G2227 Regulator Driver
 *
 * Copyright (C) 2016-2017 Realtek Semiconductor Corporation
 * Copyright (C) 2016-2017 Cheng-Yu Lee <cylee12@realtek.com>
 * Copyright (C) 2016 Simon Hsu <simon_hsu@realtek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define pr_fmt(fmt) "g2227: " fmt

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regmap.h>
#include <linux/suspend.h>
#include <linux/uaccess.h>
#include <soc/realtek/rtd129x_cpu.h>
#include <soc/realtek/rtk_regmap.h>
#include <dt-bindings/regulator/gmt,g2227.h>
#include "rtk-regulator.h"

struct pmic_device {
	struct i2c_client *client;
	struct device *dev;
	unsigned int num_regulators;
	struct regmap *regmap;
	int chip_version;
};

enum g2227_regulator_id {
	G2227_ID_DCDC1 = 0,
	G2227_ID_DCDC2,
	G2227_ID_DCDC3,
	G2227_ID_DCDC4,
	G2227_ID_DCDC5,
	G2227_ID_DCDC6,
	G2227_ID_LDO2,
	G2227_ID_LDO3,
};

#define IS_LDO(_desc) \
	((_desc)->id == G2227_ID_LDO2 || (_desc)->id == G2227_ID_LDO3)
#define RF_INVALID -1

enum {
	RF_LPOFF_TO_DO = 0,
	RF_TIME_LPOFF,
	RF_TIME_LP,
	RF_TIME_IT,
	RF_SOFTOFF,

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

	RF_DC4_ON,
	RF_DC4_NMODE,
	RF_DC4_SMODE,

	RF_DC5_ON,
	RF_DC5_NMODE,
	RF_DC5_SMODE,
	RF_DC5_NVO,
	RF_DC5_SVO,

	RF_DC6_ON,
	RF_DC6_NMODE,
	RF_DC6_SMODE,
	RF_DC6_NVO,
	RF_DC6_SVO,

	RF_LDO2_ON,
	RF_LDO2_NMODE,
	RF_LDO2_SMODE,
	RF_LDO2_NVO,
	RF_LDO2_SVO,

	RF_LDO3_ON,
	RF_LDO3_NMODE,
	RF_LDO3_SMODE,
	RF_LDO3_NVO,
	RF_LDO3_SVO,

	RF_CHIP_ID,
	RF_VERSION,

	RF_MAX
};

static const struct reg_field g2227_reg_fields[] __initconst = {
	[RF_LPOFF_TO_DO] = REG_FIELD(0x02, 7, 7),
	[RF_TIME_IT] = REG_FIELD(0x02, 4, 5),
	[RF_TIME_LP] = REG_FIELD(0x02, 2, 3),
	[RF_TIME_LPOFF] = REG_FIELD(0x02, 0, 1),
	[RF_SOFTOFF] = REG_FIELD(0x04, 7, 7),
	[RF_DC1_ON] = REG_FIELD(0x05, 7, 7),
	[RF_DC2_ON] = REG_FIELD(0x05, 6, 6),
	[RF_DC3_ON] = REG_FIELD(0x05, 5, 5),
	[RF_DC4_ON] = REG_FIELD(0x05, 4, 4),
	[RF_DC5_ON] = REG_FIELD(0x05, 3, 3),
	[RF_DC6_ON] = REG_FIELD(0x05, 2, 2),
	[RF_LDO2_ON] = REG_FIELD(0x05, 1, 1),
	[RF_LDO3_ON] = REG_FIELD(0x05, 0, 0),
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
	[RF_DC6_NMODE] = REG_FIELD(0x09, 2, 3),
	[RF_DC6_SMODE] = REG_FIELD(0x09, 0, 1),
	[RF_LDO2_NMODE] = REG_FIELD(0x0A, 6, 7),
	[RF_LDO2_SMODE] = REG_FIELD(0x0A, 4, 5),
	[RF_LDO3_NMODE] = REG_FIELD(0x0A, 2, 3),
	[RF_LDO3_SMODE] = REG_FIELD(0x0A, 0, 1),
	[RF_DC2_NVO] = REG_FIELD(0x10, 0, 4),
	[RF_DC3_NVO] = REG_FIELD(0x11, 0, 4),
	[RF_DC5_NVO] = REG_FIELD(0x12, 0, 4),
	[RF_DC1_NVO] = REG_FIELD(0x13, 6, 7),
	[RF_DC6_NVO] = REG_FIELD(0x13, 0, 4),
	[RF_LDO2_NVO] = REG_FIELD(0x14, 4, 7),
	[RF_LDO3_NVO] = REG_FIELD(0x14, 0, 3),
	[RF_DC2_SVO] = REG_FIELD(0x15, 0, 4),
	[RF_DC3_SVO] = REG_FIELD(0x16, 0, 4),
	[RF_DC5_SVO] = REG_FIELD(0x17, 0, 4),
	[RF_DC1_SVO] = REG_FIELD(0x18, 6, 7),
	[RF_DC6_SVO] = REG_FIELD(0x18, 0, 4),
	[RF_LDO2_SVO] = REG_FIELD(0x19, 4, 7),
	[RF_LDO3_SVO] = REG_FIELD(0x19, 0, 3),
	[RF_CHIP_ID] = REG_FIELD(0x20, 3, 7),
	[RF_VERSION] = REG_FIELD(0x20, 0, 2),
};

static const unsigned int g2227_dcdc1_vtbl[] = {
	3000000, 3100000, 3200000, 3300000,
};

static const unsigned int g2227_dcdc2_vtbl[] = {
	800000, 812500, 825000, 837500, 850000, 862500, 875000, 887500,
	900000, 912500, 925000, 937500, 950000, 962500, 975000, 987500,
	1000000, 1012500, 1025000, 1037500, 1050000, 1062500, 1075000, 1087500,
	1100000, 1112500, 1125000, 1137500, 1150000, 1162500, 1175000, 1187500,
};

static const unsigned int g2227_dcdc4_vtbl[] = {
	1800000,
};

static const unsigned int g2227_ldo_vtbl[] = {
	800000, 850000, 900000, 950000, 1000000, 1100000, 1200000, 1300000,
	1500000, 1600000, 1800000, 1900000, 2500000, 2600000, 3000000, 3100000,
};


static int g2227_regulator_set_mode(struct regulator_dev *rdev,
	unsigned int mode)
{
	struct rtk_desc *gd = to_rtk_desc(rdev->desc);
	unsigned int val;

	if (IS_LDO(rdev->desc))
		val = (mode & REGULATOR_MODE_IDLE) ? 2 : 0;
	else
		val =  (mode & REGULATOR_MODE_FAST) ? 2 : 0;

	return regmap_field_write(gd->fields[RRF_NMODE], val);
}

static unsigned int g2227_regulator_get_mode(struct regulator_dev *rdev)
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

static struct regulator_ops g2227_regulator_ops  = {
	.list_voltage = regulator_list_voltage_table,
	.map_voltage = regulator_map_voltage_iterate,
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
	.is_enabled = regulator_is_enabled_regmap,
	.get_mode = g2227_regulator_get_mode,
	.set_mode = g2227_regulator_set_mode,
	.set_voltage_time_sel = rtk_regulator_set_voltage_time_sel,
	.set_suspend_voltage = rtk_regulator_set_suspend_voltage,
	.set_suspend_enable = rtk_regulator_set_suspend_enable,
	.set_suspend_disable = rtk_regulator_set_suspend_disable,
};

static unsigned int g2227_regulator_dc_of_map_mode(unsigned int mode)
{
	if (mode == G2227_DC_MODE_FORCE_PWM)
		return REGULATOR_MODE_FAST;
	return REGULATOR_MODE_NORMAL;
}

static unsigned int g2227_regulator_ldo_of_map_mode(unsigned int mode)
{
	if (mode == G2227_LDO_MODE_ECO)
		return REGULATOR_MODE_IDLE;
	 return REGULATOR_MODE_NORMAL;
}

#define COMMON_DESC(_name, _id, _tbl, _map_cb, _vdelay) \
{ \
	.desc = { \
		.owner       = THIS_MODULE, \
		.name        = _name, \
		.id          = _id, \
		.ops         = &g2227_regulator_ops, \
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
	COMMON_DESC(_name, _id, _tbl, g2227_regulator_dc_of_map_mode, _vdelay)
#define LDO_DESC(_name, _id, _tbl, _vdelay) \
	COMMON_DESC(_name, _id, _tbl, g2227_regulator_ldo_of_map_mode, _vdelay)

static struct rtk_desc g2227_desc[] = {
	[G2227_ID_DCDC1] = DC_DESC("dcdc1",
		G2227_ID_DCDC1,
		g2227_dcdc1_vtbl,
		0),
	[G2227_ID_DCDC2] = DC_DESC("dcdc2",
		G2227_ID_DCDC2,
		g2227_dcdc2_vtbl,
		0),
	[G2227_ID_DCDC3] = DC_DESC("dcdc3",
		G2227_ID_DCDC3,
		g2227_dcdc2_vtbl,
		40),
	[G2227_ID_DCDC4] = DC_DESC("dcdc4",
		G2227_ID_DCDC4,
		g2227_dcdc4_vtbl,
		0),
	[G2227_ID_DCDC5] = DC_DESC("dcdc5",
		G2227_ID_DCDC5,
		g2227_dcdc2_vtbl,
		0),
	[G2227_ID_DCDC6] = DC_DESC("dcdc6",
		G2227_ID_DCDC6,
		g2227_dcdc2_vtbl,
		0),
	[G2227_ID_LDO2] = LDO_DESC("ldo2",
		G2227_ID_LDO2,
		g2227_ldo_vtbl,
		0),
	[G2227_ID_LDO3] = LDO_DESC("ldo3",
		G2227_ID_LDO3,
		g2227_ldo_vtbl,
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
static int g2227_rf_map[][RRF_MAX] = {
	[G2227_ID_DCDC1] = RRF_FULL(DC1),
	[G2227_ID_DCDC2] = RRF_FULL(DC2),
	[G2227_ID_DCDC3] = RRF_FULL(DC3),
	[G2227_ID_DCDC4] = RRF_NO_VOSEL(DC4),
	[G2227_ID_DCDC5] = RRF_FULL(DC5),
	[G2227_ID_DCDC6] = RRF_FULL(DC6),
	[G2227_ID_LDO2] = RRF_FULL(LDO2),
	[G2227_ID_LDO3] = RRF_FULL(LDO3),
};

static struct regmap_field *g2227_fields[RF_MAX];

static const struct reg_default g2227_regmap_defaults[] = {
	{ .reg = 0x02, .def = 0xc7, },
	{ .reg = 0x04, .def = 0x01, },
	/* on/off */
	{ .reg = 0x05, .def = 0xff, },
	/* mode */
	{ .reg = 0x07, .def = 0x22, },
	{ .reg = 0x08, .def = 0x22, },
	{ .reg = 0x09, .def = 0x22, },
	{ .reg = 0x0a, .def = 0x22, },
	/* nrm volt */
	{ .reg = 0x10, .def = 0x10, },
	{ .reg = 0x11, .def = 0x10, },
	{ .reg = 0x12, .def = 0x10, },
	{ .reg = 0x13, .def = 0xd0, },
	{ .reg = 0x14, .def = 0xa2, },
	/* slp volt */
	{ .reg = 0x15, .def = 0x10, },
	{ .reg = 0x16, .def = 0x10, },
	{ .reg = 0x17, .def = 0x10, },
	{ .reg = 0x18, .def = 0xd0, },
	{ .reg = 0x19, .def = 0xa2, },
	/* version */
	{ .reg = 0x20, .def = 0xd8, },
};

static bool g2227_regmap_writeable_reg(struct device *dev, unsigned int reg)
{
	return reg != 0;
}

static bool g2227_regmap_precious_reg(struct device *dev, unsigned int reg)
{
	return reg == 0;
}

static bool g2227_regmap_volatile_reg(struct device *dev, unsigned int reg)
{
	return reg == 0x20;
}

static const struct regmap_config g2227_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
	.max_register = 0x20,
	.cache_type   = REGCACHE_FLAT,
	.reg_defaults = g2227_regmap_defaults,
	.num_reg_defaults = ARRAY_SIZE(g2227_regmap_defaults),
	.writeable_reg = g2227_regmap_writeable_reg,
	.precious_reg = g2227_regmap_precious_reg,
	.volatile_reg = g2227_regmap_volatile_reg,
};

static void g2227_regulator_pm_power_off(void)
{
	int ret;

	pr_err("[PMIC] %s: pm_power_off\n", __func__);
	ret = regmap_field_write(g2227_fields[RF_SOFTOFF], 0x1);
	pr_err("returns %d\n", ret);
}

static int g2227_regulator_register(struct pmic_device *pmic_dev,
	struct rtk_desc *gd)
{
	struct regulator_config config = {};
	struct regulation_constraints *c;
	const struct reg_field *rf;
	int *ids = g2227_rf_map[gd->desc.id];
	int src;
	int i;

	/* using rf_map to get the relative regmap_field */
	for (i = 0; i < RRF_MAX; i++) {
		src = ids[i];
		if (src != RF_INVALID)
			gd->fields[i] = g2227_fields[src];
	}

	/* using regmap_field to fill desc */
	src = ids[RRF_ON];
	if (src != RF_INVALID) {
		rf = &g2227_reg_fields[src];
		gd->desc.enable_reg = rf->reg;
		gd->desc.enable_mask = GENMASK(rf->msb, rf->lsb);
	}

	/* using regmap_field to fill desc */
	src = ids[RRF_NVO];
	if (src != RF_INVALID) {
		rf = &g2227_reg_fields[src];
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

	/* workaround */
	if (gd->desc.id == G2227_ID_DCDC6 &&
		(pmic_dev->chip_version == RTD129x_CHIP_REVISION_A00 ||
		pmic_dev->chip_version == RTD129x_CHIP_REVISION_B00)) {
		gd->state_mem.enabled = true;
		gd->state_mem.disabled = false;
	}
	return 0;
}

static int g2227_regulator_probe(struct i2c_client *client,
	const struct i2c_device_id *id)
{
	struct device *dev = &client->dev;
	struct device_node *np = dev->of_node;
	struct pmic_device *pmic_dev;
	struct regmap *regmap;
	int i, ret;
	unsigned int val;

	pmic_dev = devm_kzalloc(dev, sizeof(*pmic_dev), GFP_KERNEL);
	if (!pmic_dev)
		return -ENOMEM;

	regmap = devm_rtk_regmap_i2c_init(dev, client, &g2227_regmap_config);
	if (IS_ERR(regmap))
		return -EINVAL;

	/* after regmap is ready, convert all reg_field to regmap_field */
	for (i = 0; i < RF_MAX; i++) {
		struct regmap_field *rf;

		rf = devm_regmap_field_alloc(dev, regmap, g2227_reg_fields[i]);
		if (IS_ERR(rf)) {
			ret = PTR_ERR(rf);
			dev_err(dev, "Failed to alloc regmap field %d: %d\n",
				i, ret);
			return ret;
		}
		g2227_fields[i] = rf;
	}

	pmic_dev->regmap = regmap;
	pmic_dev->client = client;
	pmic_dev->dev    = dev;
	pmic_dev->chip_version = get_rtd129x_cpu_revision();
	pmic_dev->num_regulators = ARRAY_SIZE(g2227_desc);

	ret = regmap_field_read(g2227_fields[RF_VERSION], &val);
	if (ret)
		dev_warn(dev, "failed to reader version: %d\n", ret);
	else
		dev_info(dev, "g2227 version %02x\n", val);

	for (i = 0; i < ARRAY_SIZE(g2227_desc); i++) {
		ret = g2227_regulator_register(pmic_dev, &g2227_desc[i]);
		if (ret) {
			dev_err(dev, "Failed to register %s: %d\n",
				rtk_desc_name(&g2227_desc[i]), ret);
			return ret;
		}
	}

	i2c_set_clientdata(client, pmic_dev);

	if (of_device_is_system_power_controller(client->dev.of_node)) {
		dev_info(dev, "is system-power-controller\n");
		pm_power_off = g2227_regulator_pm_power_off;
	}


	if (of_property_read_u32(np, "g2227,lpoff-act", &val))
		val = G2227_LPOFF_TO_SHUTDOWN;
	regmap_field_write(g2227_fields[RF_LPOFF_TO_DO], val);

	if (of_property_read_u32(np, "g2227,lpoff-time", &val))
		val = G2227_TIME_LPOFF_08S;
	regmap_field_write(g2227_fields[RF_TIME_LPOFF], val);

	return 0;
}

static int get_suspend_mode(void)
{
	struct file *fp;
	mm_segment_t oldfs;
	char buf[64];
	int ret = 0;

	oldfs = get_fs();
	set_fs(KERNEL_DS);

	fp = filp_open("/sys/kernel/suspend/mode", O_RDONLY, 0);
	if (fp) {
		fp->f_op->read(fp, buf, 64, &fp->f_pos);
		ret = strstr(buf, "=> coolboot") != NULL;
		filp_close(fp, NULL);
	}

	set_fs(oldfs);

	return ret;
}

static int g2227_regulator_suspend(struct device *dev)
{
	int i, mode = 0;

	dev_info(dev, "[PMIC] Enter %s\n", __func__);

	if (RTK_PM_STATE == PM_SUSPEND_STANDBY)
		goto done;

	mode = get_suspend_mode();

	for (i = 0; i < ARRAY_SIZE(g2227_desc); i++) {
		rtk_regulator_prepare_suspend_state(g2227_desc[i].rdev,
			mode);
	};

	regulator_suspend_prepare(PM_SUSPEND_MEM);

done:
	dev_info(dev, "[PMIC] Exit %s\n", __func__);
	return 0;
}

static int g2227_regulator_resume(struct device *dev)
{
	dev_info(dev, "[PMIC] Enter %s\n", __func__);
	dev_info(dev, "[PMIC] Exit %s\n", __func__);
	return 0;
}


static const struct dev_pm_ops g2227_regulator_pm_ops = {
	.suspend = g2227_regulator_suspend,
	.resume = g2227_regulator_resume,
};

static const struct i2c_device_id g2227_regulator_ids[] = {
	{"gmt-g2227", 0},
	{ },
};
MODULE_DEVICE_TABLE(i2c, g2227_regulator_ids);

static struct i2c_driver g2227_regulator_driver = {
	.driver = {
		.name = "gmt-g2227",
		.owner = THIS_MODULE,
		.pm = &g2227_regulator_pm_ops,
	},
	.id_table = g2227_regulator_ids,
	.probe    = g2227_regulator_probe,
};

module_i2c_driver(g2227_regulator_driver);

MODULE_DESCRIPTION("RTD1295 Giraffe PMU Regulator Driver");
MODULE_AUTHOR("Simon Hsu");
MODULE_AUTHOR("CY Lee");
MODULE_LICENSE("GPL");
