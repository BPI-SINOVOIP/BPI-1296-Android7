/*
 * rtd129x_efuse.c - Realtek eFuse driver
 *
 * Copyright (C) 2016 Realtek Semiconductor Corporation
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
#define pr_fmt(fmt) "efuse: " fmt

#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/module.h>
#include <linux/sysfs.h>
#include <linux/string.h>
#include <asm/io.h>
#include <soc/realtek/rtd129x_efuse.h>


#define _EFUSE_DATA_GET(_v)     ((((_v) >> 16) ^ (_v)) & 0xffff)
#define EFUSE_DATA_GET(_i)      _EFUSE_DATA_GET(efuse_data[_i])
#define EFUSE_DATA_GET_RAW(_i)  (efuse_data[_i])
#define EFUSE_GET_ENTRY(_id)    EFUSE_DATA_GET_RAW((_id) + efuse_entry_offset)
#define EFUSE_GET_ENTRY_HI(_id) (EFUSE_GET_ENTRY(_id) >> 16)
#define EFUSE_GET_ENTRY_LO(_id) (EFUSE_GET_ENTRY(_id) & 0xffff)

struct efuse_entry_config {
    int offset;
    int shift;
    int size;
};

#define DEFINE_ENTRY(_id, _off, _shift, _size) \
    [_id] = {.offset = _off, .shift = _shift, .size = _size }

struct efuse_entry_config efuse_entry_configs [] = {
    DEFINE_ENTRY(EFUSE_UUID,              0x1a4, 0,  96),
    DEFINE_ENTRY(EFUSE_ETN_PHY,           0x1b9, 2,  64),
    DEFINE_ENTRY(EFUSE_ETN_PHY_RC,        0x1bf, 5,   4),
    DEFINE_ENTRY(EFUSE_ETN_PHY_R,         0x1bf, 1,   4),
    DEFINE_ENTRY(EFUSE_ETN_PHY_AMP,       0x1b9, 2,  16),
    DEFINE_ENTRY(EFUSE_ETN_PHY_ADC,       0x1bb, 2,  16),
    DEFINE_ENTRY(EFUSE_PWR_ID,            0x1cc, 6,   2),
    DEFINE_ENTRY(EFUSE_PACKAGE_ID,        0x1d8, 0,   2),
    DEFINE_ENTRY(EFUSE_USB_CC_EN,         0x1d8, 2,   1),
    DEFINE_ENTRY(EFUSE_USB_CC1_4_7_K_CAL, 0x1d8, 3,   4),
    DEFINE_ENTRY(EFUSE_USB_CC1_12_K_CAL,  0x1d8, 7,   4),
    DEFINE_ENTRY(EFUSE_USB_CC2_4_7_K_CAL, 0x1d9, 3,   4),
    DEFINE_ENTRY(EFUSE_USB_CC2_12_K_CAL,  0x1d9, 7,   4),
    DEFINE_ENTRY(EFUSE_TOP_IDDQ,          0x1da, 3,   6),
    DEFINE_ENTRY(EFUSE_CPU_IDDQ,          0x1db, 1,   6),
    DEFINE_ENTRY(EFUSE_USB_DP_DM,         0x1dc, 0,   4),
    DEFINE_ENTRY(EFUSE_CC_CAL,            0x1dc, 4,  24),
    DEFINE_ENTRY(EFUSE_CHIP_ID,           0x200, 0, 128),
    DEFINE_ENTRY(EFUSE_PROG_RVD,          0x3fc, 0,  32),
};

#define ID_TO_STR(_id, _str) \
    [_id] = _str

const char * efuse_names [] = {
    ID_TO_STR(EFUSE_UUID, "uuid"),
    ID_TO_STR(EFUSE_ETN_PHY, "etn_phy"),
    ID_TO_STR(EFUSE_ETN_PHY_RC, "  RC"),
    ID_TO_STR(EFUSE_ETN_PHY_R, "  R"),
    ID_TO_STR(EFUSE_ETN_PHY_AMP, "  Amp"),
    ID_TO_STR(EFUSE_ETN_PHY_ADC, "  ADC"),
    ID_TO_STR(EFUSE_PWR_ID, "pwr_id"),
    ID_TO_STR(EFUSE_PACKAGE_ID, "package_id"),
    ID_TO_STR(EFUSE_USB_CC_EN, "usb_cc_en"),
    ID_TO_STR(EFUSE_USB_CC1_4_7_K_CAL, "usb_cc1_4.7k_cal"),
    ID_TO_STR(EFUSE_USB_CC1_12_K_CAL, "usb_cc1_12k_cal"),
    ID_TO_STR(EFUSE_USB_CC2_4_7_K_CAL, "usb_cc2_4.7k_cal"),
    ID_TO_STR(EFUSE_USB_CC2_12_K_CAL, "usb_cc2_12k_cal"),
    ID_TO_STR(EFUSE_TOP_IDDQ, "top_iddq"),
    ID_TO_STR(EFUSE_CPU_IDDQ, "cpu_iddq"),
    ID_TO_STR(EFUSE_USB_DP_DM, "usb_dp_dm"),
    ID_TO_STR(EFUSE_CC_CAL, "cc_cal"),
    ID_TO_STR(EFUSE_CHIP_ID, "chip_id"),
    ID_TO_STR(EFUSE_PROG_RVD, "prog_rvd"),
};

#define EFUSE_SHOW_ENTRY_BIT   0
#define EFUSE_SHOW_NAME        2

static int efuse_enabled = 0;    
static void __iomem *efuse_base;
static struct kobject *efuse_kobj;
static unsigned int *efuse_data;
static unsigned int efuse_entry_offset;
static unsigned int efuse_entry_count;

/*
 * returns only if in efuse ranage
 */
#define DEFINE_EFUSE_READ_FUNC(_type, _suffix)                \
_type efuse_read##_suffix(unsigned long offset)               \
{                                                             \
    if (offset < EFUSE_MEM_SIZE)                         \
        return read##_suffix(efuse_base + offset);            \
    return 0;                                                 \
}                                                             \
EXPORT_SYMBOL_GPL(efuse_read##_suffix);

DEFINE_EFUSE_READ_FUNC(u8, b);
DEFINE_EFUSE_READ_FUNC(u16, w);
DEFINE_EFUSE_READ_FUNC(u32, l);
DEFINE_EFUSE_READ_FUNC(u64, q);

static unsigned int __efuse_extract_data(unsigned int v)
{
    unsigned int a, b, c, d, o = 1;
    a = EFUSE_DATA_GET(3);
    b = EFUSE_DATA_GET(0);
    c = EFUSE_DATA_GET(1);
    d = EFUSE_DATA_GET(2);
    
    v ^= a;
    while (c --)
        o = (o * v) % b; 
    return o ^ d;
}

static void __efuse_read_entry(int id, unsigned char *buf)
{
    int shift = efuse_entry_configs[id].shift;
    int size =  efuse_entry_configs[id].size;
    int offset =  efuse_entry_configs[id].offset;
    int cur_size;

    pr_debug("--- id = %d\n", id);
    while (size > 0) {
        unsigned char val = efuse_readb(offset);
        pr_debug("offset = %04x, val = %02x\n", offset, (unsigned int)val);

        cur_size = size >= (8 - shift) ? 8 - shift : size;
        *buf = val & ((BIT(cur_size) - 1) << shift);
        shift = 0;

        size -= cur_size;
        buf ++;
        offset ++;
    }

    *buf = 0;
}

static void __efuse_read_entry_align(int id, unsigned char *buf)
{
    int shift = efuse_entry_configs[id].shift;
    int size =  efuse_entry_configs[id].size;

    __efuse_read_entry(id, buf);

    if (shift == 0)
        return;

    while (size > 0) {
        *buf >>= shift;
        *buf |= *(buf+1) << (8-shift);

        buf += 1;
        size -= 8;  
    }
}

/**
 * efuse_read_pattren - read efuse data through the predifined entries
 * @id: pattern id
 * @buf: output data
 * @size: input/size of buf, output/read bit count
 *
 * Returns 0 on sueccess, -EINVAL if invalid argument, -EFBIG if buf
 * it too small.
 */
int efuse_read_pattern(int id, unsigned char *buf, int *size)
{
    int shift = efuse_entry_configs[id].shift;
    int en_size =  efuse_entry_configs[id].size;
    int min_size = en_size + shift;

    if (!efuse_enabled)
        return -ENODEV;

    if (id < 0 || !buf || !size)
        return -EINVAL;
    
    if ((*size * 8) < min_size)
        return -ENOBUFS;    

    __efuse_read_entry_align(id, buf);

    *size = en_size;

    return 0;
}
EXPORT_SYMBOL_GPL(efuse_read_pattern);

static int rtd129x_efuse_extract_iomap(void)
{
    unsigned int a = 0, s = 0;
    a |= __efuse_extract_data(EFUSE_GET_ENTRY_HI(-4)) << 24;
    a |= __efuse_extract_data(EFUSE_GET_ENTRY_LO(-4)) << 16;
    a |= __efuse_extract_data(EFUSE_GET_ENTRY_HI(-3)) << 8;
    a |= __efuse_extract_data(EFUSE_GET_ENTRY_LO(-3));
    s |= __efuse_extract_data(EFUSE_GET_ENTRY_HI(-2)) << 8;
    s |= __efuse_extract_data(EFUSE_GET_ENTRY_LO(-2));

    if (s != EFUSE_MEM_SIZE && s != EFUSE_OTP_SIZE)
        return -EINVAL;

    iounmap(efuse_base);
    efuse_base = ioremap(a, s);
    return 0;
}

static int __bit_snprintf(char * buf, int size, char byte, int cnt)
{
    int len = 0;
    while (cnt-- > 0) {
        len += snprintf(buf + len, size - len, "%d", ((byte >> cnt) & 0x1));
    }
    return len;
}

static int __rtd129x_efuse_entry_show(int id, char *buf, int mode) 
{   
    unsigned char xbuf[EFUSE_MAX_ENTRY_SIZE / 8 + 1];
    int len = 0;
    int en_size = efuse_entry_configs[id].size;
    int size = en_size / 8 + ((en_size % 8) ? 1 : 0);
    int i;
    int no_data = 1;

    __efuse_read_entry_align(id, xbuf);

    if (mode & EFUSE_SHOW_NAME) {
       len += snprintf(buf + len, PAGE_SIZE - len, "%s: ", efuse_names[id]);
    }
    
    for (i = size - 1; i >= 0; i--) {
        unsigned char mask = 0xff;

        if (i == (size - 1)) {
            int bc =  en_size % 8;
            if (bc != 0) 
                mask = BIT(bc) - 1;
        }

        if ((xbuf[i] & mask) != 0) 
            no_data = 0;
    }

    if (no_data) {
        len += snprintf(buf + len, PAGE_SIZE - len, "%s", "no data\n");
    } else {
        for (i = size - 1; i >=0; i--) {
            int bit_cnt = 8;

            if (i == (size - 1)) {
                int bc = en_size % 8;
                if (bc != 0)
                    bit_cnt = bc;
            }          

            len += __bit_snprintf(buf + len, PAGE_SIZE - len, xbuf[i], bit_cnt);              
            
            if (i != 0)
                len += snprintf(buf + len, PAGE_SIZE - len, " ");
        }
            
        len += snprintf(buf + len, PAGE_SIZE - len, "\n");
    }

    return len;
}

static ssize_t summary_show(struct kobject *kobj,
    struct kobj_attribute *attr, char *buf)
{
    int len = 0;
    int i;
    for (i = 0; i <= EFUSE_PROG_RVD; i++) {
        len += __rtd129x_efuse_entry_show(i, buf+len, EFUSE_SHOW_ENTRY_BIT | EFUSE_SHOW_NAME);
        
        if (len >= PAGE_SIZE)
            return PAGE_SIZE;
    }
    return len;
}
static struct kobj_attribute summary_attr = __ATTR_RO(summary);

#if 0
#define DEFINE_SYSFS_EFUSE_ENTRY(_name, _id) \
    static ssize_t _name##_show(struct kobject *kobj, \
       struct kobj_attribute *attr, char *buf) \
    { \
        return __rtd129x_efuse_entry_show(_id, buf, EFUSE_SHOW_ENTRY_BTYE); \
    } \
    static struct kobj_attribute _name##_attr = \
        __ATTR_RO(_name);

DEFINE_SYSFS_EFUSE_ENTRY(uuid, EFUSE_UUID);
DEFINE_SYSFS_EFUSE_ENTRY(etn_phy, EFUSE_ETN_PHY);
DEFINE_SYSFS_EFUSE_ENTRY(pwr_id, EFUSE_PWR_ID);
DEFINE_SYSFS_EFUSE_ENTRY(package_id, EFUSE_PACKAGE_ID);
DEFINE_SYSFS_EFUSE_ENTRY(usb_cc_en, EFUSE_USB_CC_EN);
DEFINE_SYSFS_EFUSE_ENTRY(usb_cc1_4_7_k_cal, EFUSE_USB_CC1_4_7_K_CAL);
DEFINE_SYSFS_EFUSE_ENTRY(usb_cc1_12_k_cal, EFUSE_USB_CC1_12_K_CAL);
DEFINE_SYSFS_EFUSE_ENTRY(usb_cc2_4_7_k_cal, EFUSE_USB_CC2_4_7_K_CAL);
DEFINE_SYSFS_EFUSE_ENTRY(usb_cc2_12_k_cal, EFUSE_USB_CC2_12_K_CAL);
DEFINE_SYSFS_EFUSE_ENTRY(top_iddq, EFUSE_TOP_IDDQ);
DEFINE_SYSFS_EFUSE_ENTRY(cpu_iddq, EFUSE_CPU_IDDQ);
DEFINE_SYSFS_EFUSE_ENTRY(usb_dp_dm, EFUSE_USB_DP_DM);
DEFINE_SYSFS_EFUSE_ENTRY(cc_cal, EFUSE_CC_CAL);
DEFINE_SYSFS_EFUSE_ENTRY(chip_id, EFUSE_CHIP_ID);
DEFINE_SYSFS_EFUSE_ENTRY(prog_rvd, EFUSE_PROG_RVD);
#endif

static struct attribute *efuse_attrs[] = {
    &summary_attr.attr,
#if 0
    &uuid_attr.attr,
    &etn_phy_attr.attr,
    &pwr_id_attr.attr,
    &package_id_attr.attr,
    &usb_cc_en_attr.attr,
    &usb_cc1_4_7_k_cal_attr.attr,
    &usb_cc1_12_k_cal_attr.attr,
    &usb_cc2_4_7_k_cal_attr.attr,
    &usb_cc2_12_k_cal_attr.attr,
    &top_iddq_attr.attr,
    &cpu_iddq_attr.attr,
    &usb_dp_dm_attr.attr,
    &cc_cal_attr.attr,
    &chip_id_attr.attr,
    &prog_rvd_attr.attr,
#endif
    NULL
};

static struct attribute_group efuse_attr_group = {
    .attrs = efuse_attrs,
};

static __init int rtk129x_efuse_add_sysfs_nodes(struct device *dev)
{
    int ret = 0;

    efuse_kobj = kobject_create_and_add("efuse", kernel_kobj);
    if (!efuse_kobj) {
        dev_err(dev, "cannot create kobj\n");
        return -ENOMEM;
    }

    ret = sysfs_create_group(efuse_kobj, &efuse_attr_group);
    if (ret) {
        dev_err(dev, "cannot create sysfs group. err:%d\n", ret);
        goto error;
    }
    return 0;

error:
    if (efuse_kobj)
        kobject_del(efuse_kobj);
    return ret;
}

static int rtk129x_efuse_probe(struct platform_device *pdev)
{
    struct device_node *np = pdev->dev.of_node;
    const char *prop;
    int ret;
    int data_size;

    efuse_base = of_iomap(np, 0);

    prop = "efuse,header";
    ret = of_property_read_u32(np, prop, &efuse_entry_offset);
    if (ret) {
        dev_err(&pdev->dev, "Failed to read %s from DT: %d\n", prop, ret);
        return ret; 
    }    
    
    prop = "efuse,entry";
    ret = of_property_read_u32(np, prop, &efuse_entry_count);
    if (ret) {
        dev_err(&pdev->dev, "Failed to read %s from DT: %d\n", prop, ret);
        return ret;
    }

    data_size = efuse_entry_offset + efuse_entry_count;

    efuse_data = devm_kzalloc(&pdev->dev, sizeof(*efuse_data) * data_size, GFP_KERNEL);
    if (!efuse_data)
        return -ENOMEM;

    prop = "efuse,data";
    ret = of_property_read_u32_array(np, prop, efuse_data, data_size);
    if (ret) {
        dev_err(&pdev->dev, "Failed to read %s from DT: %d\n", prop, ret);
        return ret;
    }

    ret = rtd129x_efuse_extract_iomap();
    if (ret) {
        dev_err(&pdev->dev, "Failed to ioremap: %d\n", ret);
        return ret;
    }

    efuse_enabled = 1;

    ret = rtk129x_efuse_add_sysfs_nodes(&pdev->dev);    

    return ret;
}

static int rtk129x_efuse_remove(struct platform_device *pdev)
{
    sysfs_remove_group(efuse_kobj, &efuse_attr_group);

    kobject_del(efuse_kobj);

    iounmap(efuse_base);

    return 0;
}

static const struct of_device_id rtk_efuse_match[] = {
    {.compatible = "Realtek,rtk-efuse"},
    {},
};

static struct platform_driver rtk129x_efuse_driver = {
    .probe = rtk129x_efuse_probe,
    .remove = rtk129x_efuse_remove,
    .driver = {
        .name = "rtk129x-efuse",
        .owner = THIS_MODULE,
        .of_match_table = of_match_ptr(rtk_efuse_match),
    },
};
module_platform_driver(rtk129x_efuse_driver);

MODULE_DESCRIPTION("Realtek eFuse driver");
MODULE_ALIAS("platform:rtk129x-efuse");
MODULE_LICENSE("GPL");
