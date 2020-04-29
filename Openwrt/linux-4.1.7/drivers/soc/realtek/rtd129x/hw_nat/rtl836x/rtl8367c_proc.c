#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#include "smi.h"
#include "port.h"
#include "l2.h"
#include "rtk_error.h"
#include "interrupt.h"
#include "vlan.h"
#include "rtl8367c_asicdrv.h"
#include "rtk_switch.h"
#include "rtk_types.h"
#include "cpu.h"
#include "rtl8367c_asicdrv_port.h"
#include "port.h"
#include "stat.h"

#define RTL836X_PROC_DIR_NAME           "rtl836x"
#define RTL836X_PROC_VLAN_DIR_NAME      "rtl836x/vlan"
#define RTL836X_PROC_CPUTAG_DIR_NAME    "rtl836x/cpu_tag"
#define RTL836X_PROC_PORT_DIR_NAME      "rtl836x/port"

static void rtk_vlan_test(void);
static void rtk_vlan_test2(void);

extern struct proc_dir_entry proc_root;
static struct proc_dir_entry *rtl836x_proc_dir = NULL;
static struct proc_dir_entry *rtl836x_proc_vlan_dir = NULL;
static struct proc_dir_entry *rtl836x_proc_cputag_dir = NULL;
static struct proc_dir_entry *rtl836x_proc_port_dir = NULL;

/* test */
static int test_read(struct seq_file *s, void *v)
{
    unsigned int value = 0x0;

    smi_read(0x1202, &value);
    seq_printf(s, "Test print default value of reg.0x1202:[%x]\n", value);

    smi_write(0x1202, 0x1234);
    seq_printf(s, "Write 0x1234 to reg.0x1202.\n");

    value = 0x0;
    smi_read(0x1202, &value);
    seq_printf(s, "Read value from reg.0x1202:[%x]\n", value);

    smi_write(0x1202, 0x88a8);

    return 0;
}

static int test_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[64] = {0};

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';
        printk(KERN_ERR "tmpbuf:[%s]\n", tmpbuf);
    }

    return len;
}

static int test_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, test_read, NULL));
}

static ssize_t test_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
    return test_write(file, userbuf, count, off);
}

static struct file_operations test_proc_fops = {
    .open = test_single_open,
    .write = test_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* port status */
static int port_status_read(struct seq_file *s, void *v)
{
    rtk_port_mac_ability_t Portstatus;
    unsigned int i = 0;
    rtk_stat_counter_t rxcntr, txcntr;

    for(i=0; i<5; i++) {
        memset(&Portstatus, 0x0, sizeof(rtk_port_mac_ability_t));
        if( rtk_port_macStatus_get(i, &Portstatus) == RT_ERR_OK )
        {
            seq_printf(s, "Port %d status:\n", i);
            seq_printf(s, "   forcemode:[%x]\n", Portstatus.forcemode);
            seq_printf(s, "   speed    :[%x]\n", Portstatus.speed);
            seq_printf(s, "   duplex   :[%x]\n", Portstatus.duplex);
            seq_printf(s, "   link     :[%x]\n", Portstatus.link);
            seq_printf(s, "   nway     :[%x]\n", Portstatus.nway);
            seq_printf(s, "   txpause  :[%x]\n", Portstatus.txpause);
            seq_printf(s, "   rxpause  :[%x]\n\n", Portstatus.rxpause);
        }

        rxcntr = 0x0;
        txcntr = 0x0;
        rtk_stat_port_get(i, STAT_IfInOctets, &rxcntr);
        rtk_stat_port_get(i, STAT_IfOutOctets, &txcntr);
        seq_printf(s, "   tx count :[%x]\n", txcntr);
        seq_printf(s, "   rx count :[%x]\n", rxcntr);

        rxcntr = 0x0;
        rtk_stat_port_get(i, STAT_Dot3StatsFCSErrors, &rxcntr);
        seq_printf(s, "   rx FCS error count :[%x]\n", rxcntr);

        rxcntr = 0x0;
        rtk_stat_port_get(i, STAT_Dot3StatsSymbolErrors, &rxcntr);
        seq_printf(s, "   rx data symbol errors count:[%x]\n", rxcntr);

        rxcntr = 0x0;
        rtk_stat_port_get(i, STAT_EtherStatsFragments, &rxcntr);
        seq_printf(s, "   rx EtherStatsFragments count:[%x]\n", rxcntr);

        rxcntr = 0x0;
        rtk_stat_port_get(i, STAT_EtherStatsJabbers, &rxcntr);
        seq_printf(s, "   rx EtherStatsJabbers count:[%x]\n", rxcntr);

        seq_printf(s, "\n");
    }

    seq_printf(s, "Port rgmii status:\n", i);

    rxcntr = 0x0;
    txcntr = 0x0;
    rtk_stat_port_get(EXT_PORT0, STAT_IfInOctets, &rxcntr);
    rtk_stat_port_get(EXT_PORT0, STAT_IfOutOctets, &txcntr);
    seq_printf(s, "   tx count :[%x]\n", txcntr);
    seq_printf(s, "   rx count :[%x]\n", rxcntr);

    rxcntr = 0x0;
    rtk_stat_port_get(EXT_PORT0, STAT_Dot3StatsFCSErrors, &rxcntr);
    seq_printf(s, "   rx FCS error count :[%x]\n", rxcntr);

    rxcntr = 0x0;
    rtk_stat_port_get(EXT_PORT0, STAT_Dot3StatsSymbolErrors, &rxcntr);
    seq_printf(s, "   rx data symbol errors count:[%x]\n", rxcntr);

    rxcntr = 0x0;
    rtk_stat_port_get(EXT_PORT0, STAT_EtherStatsFragments, &rxcntr);
    seq_printf(s, "   rx EtherStatsFragments count:[%x]\n", rxcntr);

    rxcntr = 0x0;
    rtk_stat_port_get(EXT_PORT0, STAT_EtherStatsJabbers, &rxcntr);
    seq_printf(s, "   rx EtherStatsJabbers count:[%x]\n", rxcntr);

    return 0;
}

static int port_status_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, port_status_read, NULL));
}

static struct file_operations port_status_proc_fops = {
    .open = port_status_single_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* L2 table */
static int l2_table_read(struct seq_file *s, void *v)
{
    unsigned int i = 0;
    rtk_l2_addr_table_t *L2_entry;

    L2_entry = kmalloc(sizeof(rtk_l2_addr_table_t), GFP_KERNEL);

    seq_printf(s, "L2 table:\n");
    if(L2_entry) {
        for(i=0; i<20; i++) {
            memset(L2_entry, 0x0, sizeof(rtk_l2_addr_table_t));
            L2_entry->index = i;
            rtk_l2_entry_get(L2_entry);
            seq_printf(s, "index:[%d] sip:[%x] dip:[%x] mac:[%x]\n", L2_entry->index, L2_entry->sip, L2_entry->dip, L2_entry->mac);
        }

        kfree(L2_entry);
    }

    return 0;
}

static int l2_table_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, l2_table_read, NULL));
}

static struct file_operations l2_table_proc_fops = {
    .open = l2_table_single_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* interrupt */
static int interrupt_read(struct seq_file *s, void *v)
{
    rtk_int_status_t statusMask;
  
    rtk_int_status_get(&statusMask);

    seq_printf(s, "statusMask:[%x]\n", statusMask);

    return 0;
}

static int interrupt_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, interrupt_read, NULL));
}

static int interrupt_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[64] = {0};
    rtk_int_status_t statusMask;

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        if(strstr(tmpbuf, "clear") != NULL) {
            statusMask.value[0] = 0x1;
            rtk_int_status_set(&statusMask);
        }
    }

    return len;
}


static ssize_t interrupt_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return interrupt_write(file, userbuf, count, off);
}


static struct file_operations interrupt_proc_fops = {
    .open = interrupt_single_open,
    .write = interrupt_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* vlan enale */
static int vlanEnable_read(struct seq_file *s, void *v)
{
#if 0
    unsigned int i = 0;
    rtk_vlan_cfg_t vlan;
    rtk_vlan_t vid;

    for(i=0; i<RTL8367C_CVIDXNO; i++) {
        memset(&vlan, 0x00, sizeof(rtk_vlan_cfg_t));

        vid = rtk_getVid_by_mbrCfgUsage(i);
        if(vid != 0){
            rtk_vlan_get(vid, &vlan);
            
            seq_printf(s, "vid:%d\n", vid);
            seq_printf(s, "mbr:[%x] untag:[%x] ivl_en:[%x] fid_msti:[%x] envlanpol:[%x] meteridx:[%x] vbpen:[%x] vbpri:[%x]\n", 
                                    vlan.mbr, vlan.untag, vlan.ivl_en, vlan.fid_msti, vlan.envlanpol, vlan.meteridx, vlan.vbpen, vlan.vbpri);

        }
    }
#endif
    return 0;
}

static int vlanEnable_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, vlanEnable_read, NULL));
}


static void rtk_vlan_test()
{
    rtk_vlan_cfg_t vlan;

    memset(&vlan, 0x00, sizeof(rtk_vlan_cfg_t));

    RTK_PORTMASK_PORT_SET(vlan.mbr, UTP_PORT0);
    RTK_PORTMASK_PORT_SET(vlan.mbr, UTP_PORT1);
    RTK_PORTMASK_PORT_SET(vlan.mbr, UTP_PORT4);

    RTK_PORTMASK_PORT_SET(vlan.untag, UTP_PORT0);
    RTK_PORTMASK_PORT_SET(vlan.untag, UTP_PORT1);

    rtk_vlan_set(100, &vlan);

    memset(&vlan, 0x00, sizeof(rtk_vlan_cfg_t));
    RTK_PORTMASK_PORT_SET(vlan.mbr, UTP_PORT2);
    RTK_PORTMASK_PORT_SET(vlan.mbr, UTP_PORT3);
    RTK_PORTMASK_PORT_SET(vlan.mbr, UTP_PORT4);

    RTK_PORTMASK_PORT_SET(vlan.untag, UTP_PORT2);
    RTK_PORTMASK_PORT_SET(vlan.untag, UTP_PORT3);

    rtk_vlan_set(200, &vlan);
}

static void rtk_vlan_test2()
{
    rtk_vlan_portPvid_set(UTP_PORT0, 100, 0);
    rtk_vlan_portPvid_set(UTP_PORT1, 100, 0);
    rtk_vlan_portPvid_set(UTP_PORT2, 200, 0);
    rtk_vlan_portPvid_set(UTP_PORT3, 200, 0);
    rtk_vlan_portPvid_set(UTP_PORT4, 100, 0);

}

static int vlanEnable_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[64] = {0};
    unsigned int enable = 0;
    unsigned int num = 0;

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        num = sscanf(tmpbuf,"%d", &enable);
        if(num != 1) {
            return len;
        }

        if(enable == 1) {
            rtk_vlan_init();
        }
#if 1
        else if(enable == 2) {
            rtk_vlan_test();
            rtk_vlan_test2();
        }
        else if(enable == 3) {
            rtk_vlan_test();
        }
        else if(enable == 4) {
            rtk_vlan_test2();
        }
#endif
    }

    return len;
}

static ssize_t vlanEnable_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return vlanEnable_write(file, userbuf, count, off);
}


static struct file_operations vlanEnable_proc_fops = {
    .open = vlanEnable_single_open,
    .write = vlanEnable_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* vlan group */
static int vlanGroup_read(struct seq_file *s, void *v)
{
    unsigned int i = 0;
    rtk_vlan_cfg_t vlan;
    rtk_vlan_t vid;

    for(i=0; i<RTL8367C_CVIDXNO; i++) {
        memset(&vlan, 0x00, sizeof(rtk_vlan_cfg_t));

        vid = rtk_getVid_by_mbrCfgUsage(i);
        if(vid != 0){
            rtk_vlan_get(vid, &vlan);

            seq_printf(s, "vid:[%d] mbr:[%x] untag:[%x] ivl_en:[%x] fid_msti:[%x] envlanpol:[%x] meteridx:[%x] vbpen:[%x] vbpri:[%x]\n",
                                    vid, vlan.mbr, vlan.untag, vlan.ivl_en, vlan.fid_msti, vlan.envlanpol, vlan.meteridx, vlan.vbpen, vlan.vbpri);

        }
    }

    return 0;
}

static int vlanGroup_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, vlanGroup_read, NULL));
}

static int vlanGroup_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[128] = {0};
    unsigned int num = 0;
    unsigned int mask = 0x0;
    unsigned int tagMask = 0x0;
    unsigned int vid = 0x0;
    rtk_vlan_cfg_t vlan;
    unsigned int i = 0;


    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        num = sscanf(tmpbuf,"%x,%x,%d", &mask, &tagMask, &vid);

        //printk(KERN_ERR "mask:[%x] tagMask:[%x] vid:[%d]\n", mask, tagMask, vid);

        memset(&vlan, 0x0, sizeof(rtk_vlan_cfg_t));
        
        for( i = 0; i < RTK_SWITCH_PORT_NUM; i++)
        {
            if( (rtk_switch_phyPortMask_get() & (0x00000001 << i)))
            {
                //printk(KERN_ERR "set port %d ", i);
                if(mask & (0x00000001 << i)) {
                    printk(KERN_ERR "in mbr");
                    RTK_PORTMASK_PORT_SET(vlan.mbr, i);

                    if(!(tagMask & (0x00000001 << i))) {
                        //printk(KERN_ERR " and untag.");
                        RTK_PORTMASK_PORT_SET(vlan.untag, i);
                    } 
                    //else 
                    //{
                        //printk(KERN_ERR " and tag.");
                    //}
                }
                //printk(KERN_ERR "\n");
            }
        }

        rtk_vlan_set(vid, &vlan);
    }

    return len;
}

static ssize_t vlanGroup_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return vlanGroup_write(file, userbuf, count, off);
}


static struct file_operations vlanGroup_proc_fops = {
    .open = vlanGroup_single_open,
    .write = vlanGroup_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* vlan group */
static int portPvid_read(struct seq_file *s, void *v)
{
    rtk_uint32 port;
    rtk_vlan_t pvid;
    rtk_pri_t priority; 
    
    for( port = 0; port < RTK_SWITCH_PORT_NUM; port++)
    {
        if( (rtk_switch_phyPortMask_get() & (0x00000001 << port)))
        {
            rtk_vlan_portPvid_get(port, &pvid, &priority);
            seq_printf(s, "port:[%d] pvid:[%d] priority:[%d]\n", port, pvid, priority);
        }
    }

    return 0;
}

static int portPvid_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, portPvid_read, NULL));
}

static int portPvid_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[128] = {0};
    unsigned int num = 0;
    unsigned int port = 0x0;
    unsigned int vid = 0x0;


    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        num = sscanf(tmpbuf,"%d,%d", &port, &vid);

        rtk_vlan_portPvid_set(port, vid, 0);
    }

    return len;
}

static ssize_t portPvid_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return portPvid_write(file, userbuf, count, off);
}


static struct file_operations portPvid_proc_fops = {
    .open = portPvid_single_open,
    .write = portPvid_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* cpu tag - enable/disable */
static int cputagEnable_read(struct seq_file *s, void *v)
{
    rtk_enable_t enable = 0;

    if(rtk_cpu_enable_get(&enable) != RT_ERR_OK) {
        seq_printf(s, "Get cpu tag enable status fail.\n");
    }else{
        seq_printf(s, "%d\n", enable);
    }

    return 0;
}

static int cputagEnable_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, cputagEnable_read, NULL));
}

static int cputagEnable_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[8] = {0};
    unsigned int num = 0;
    unsigned int enable = 0;

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        num = sscanf(tmpbuf,"%d", &enable);

        rtk_cpu_enable_set(enable);
    }

    return len;
}

static ssize_t cputagEnable_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return cputagEnable_write(file, userbuf, count, off);
}

static struct file_operations cputagEnable_proc_fops = {
    .open = cputagEnable_single_open,
    .write = cputagEnable_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* cpu tag port setting */
static int cputagPort_read(struct seq_file *s, void *v)
{
    rtk_port_t port = EXT_PORT0;
    rtk_cpu_insert_t mode;

    rtk_cpu_tagPort_get(&port, &mode);

    switch(mode)
    {
        case CPU_INSERT_TO_ALL:
            seq_printf(s, "CPU_INSERT_TO_ALL\n");
            break;
        case CPU_INSERT_TO_TRAPPING:
            seq_printf(s, "CPU_INSERT_TO_TRAPPING\n");
            break;
        case CPU_INSERT_TO_NONE:
            seq_printf(s, "CPU_INSERT_TO_NONE\n");
            break;
        default:
            seq_printf(s, "UNKNOW_MODE\n");
    }

    return 0;
}

static int cputagPort_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, cputagPort_read, NULL));
}

static int cputagPort_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[8] = {0};
    unsigned int num = 0;
    unsigned int mode = 0;

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        num = sscanf(tmpbuf,"%d", &mode);
        if((mode >= 0) && (mode < 2)) {
            rtk_cpu_tagPort_set(EXT_PORT0, mode) ;
        }
    }

    return len;
}

static ssize_t cputagPort_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return cputagEnable_write(file, userbuf, count, off);
}

static struct file_operations cputagPort_proc_fops = {
    .open = cputagPort_single_open,
    .write = cputagPort_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* rgmiidelay setting */
static int rgmiidelay_read(struct seq_file *s, void *v)
{
    rtk_data_t txDelay, rxDelay;

    txDelay = 0;
    rxDelay = 0;
    rtk_port_rgmiiDelayExt_get(EXT_PORT0, &txDelay, &rxDelay);
    seq_printf(s, "EXT_PORT0 -- txDelay:[%d] rxDelay:[%d]\n", txDelay, rxDelay);

    txDelay = 0;
    rxDelay = 0;
    rtk_port_rgmiiDelayExt_get(EXT_PORT1, &txDelay, &rxDelay);
    seq_printf(s, "EXT_PORT1 -- txDelay:[%d] rxDelay:[%d]\n", txDelay, rxDelay);

    return 0;
}

static int rgmiidelay_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, rgmiidelay_read, NULL));
}

static int rgmiidelay_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[8] = {0};
    unsigned int num = 0;
    unsigned int port = 0;
    rtk_data_t txDelay, rxDelay;
    rtk_api_ret_t ret;

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';

        num = sscanf(tmpbuf,"%d,%d,%d", &port, &txDelay, &rxDelay);
        if(port == 0) {
            port = EXT_PORT0;
        }else if(port == 1) {
            port = EXT_PORT1;
        }else{
            port = EXT_PORT0;
        }
        
        ret = rtk_port_rgmiiDelayExt_set(port, txDelay, rxDelay);
        pr_err("rgmiiDelay: port %d, tx delay %d, rx delay %d, ret %d\n",
		port, txDelay, rxDelay, ret);
    }

    return len;
}

static ssize_t rgmiidelay_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return rgmiidelay_write(file, userbuf, count, off);
}

static struct file_operations rgmiidelay_proc_fops = {
    .open = rgmiidelay_single_open,
    .write = rgmiidelay_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

/* rgmiiInfo setting */
static int rgmiiInfo_read(struct seq_file *s, void *v)
{
    rtk_mode_ext_t mode;
    rtk_port_mac_ability_t mac_cfg;

    rtk_port_macForceLinkExt_get(EXT_PORT0, &mode, &mac_cfg);

    seq_printf(s, "EXT_PORT0:\n");
    seq_printf(s, "    mode:     [%d]\n", mode);
    seq_printf(s, "    forcemode:[%d]\n", mac_cfg.forcemode);
    seq_printf(s, "    speed:    [%d]\n", mac_cfg.speed);
    seq_printf(s, "    duplex:   [%d]\n", mac_cfg.duplex);
    seq_printf(s, "    link:     [%d]\n", mac_cfg.link);
    seq_printf(s, "    nway:     [%d]\n", mac_cfg.nway);
    seq_printf(s, "    txpause:  [%d]\n", mac_cfg.txpause);
    seq_printf(s, "    rxpause:  [%d]\n", mac_cfg.rxpause);

    return 0;
}

static int rgmiiInfo_single_open(struct inode *inode, struct file *file)
{
    return (single_open(file, rgmiiInfo_read, NULL));
}

static int rgmiiInfo_write(struct file *filp, const char *buff,
    unsigned long len, void *data)
{
    char tmpbuf[8] = {0};

    if (buff && !copy_from_user(tmpbuf, buff, len)) {
        tmpbuf[len - 1] = '\0';
    }

    return len;
}

static ssize_t rgmiiInfo_single_write(struct file *file,
    const char __user *userbuf, size_t count, loff_t *off)
{
        return rgmiiInfo_write(file, userbuf, count, off);
}

static struct file_operations rgmiiInfo_proc_fops = {
    .open = rgmiiInfo_single_open,
    .write = rgmiiInfo_single_write,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};
/* proc init function */
void rtl836x_proc_init(void)
{
    rtl836x_proc_dir = proc_mkdir(RTL836X_PROC_DIR_NAME, &proc_root);
    if (rtl836x_proc_dir) {
        proc_create_data("test", 0, rtl836x_proc_dir, &test_proc_fops, NULL);
        proc_create_data("port_status", 0, rtl836x_proc_dir, &port_status_proc_fops, NULL);
        proc_create_data("l2", 0, rtl836x_proc_dir, &l2_table_proc_fops, NULL);
        //proc_create_data("interrupt", 0, rtl836x_proc_dir, &interrupt_proc_fops, NULL);
    }

    rtl836x_proc_vlan_dir = proc_mkdir(RTL836X_PROC_VLAN_DIR_NAME, &proc_root);
    if(rtl836x_proc_vlan_dir) {
        proc_create_data("vlanEnable", 0, rtl836x_proc_vlan_dir, &vlanEnable_proc_fops, NULL);
        proc_create_data("vlanGroup", 0, rtl836x_proc_vlan_dir, &vlanGroup_proc_fops, NULL);
        proc_create_data("portPvid", 0, rtl836x_proc_vlan_dir, &portPvid_proc_fops, NULL);
    }

    rtl836x_proc_cputag_dir = proc_mkdir(RTL836X_PROC_CPUTAG_DIR_NAME, &proc_root);
    if(rtl836x_proc_cputag_dir) {
        proc_create_data("Enable", 0, rtl836x_proc_cputag_dir, &cputagEnable_proc_fops, NULL);
        proc_create_data("tagPort", 0, rtl836x_proc_cputag_dir, &cputagPort_proc_fops, NULL);
    }

    rtl836x_proc_port_dir = proc_mkdir(RTL836X_PROC_PORT_DIR_NAME, &proc_root);
    if(rtl836x_proc_port_dir) {
        proc_create_data("RgmiiDelay", 0, rtl836x_proc_port_dir, &rgmiidelay_proc_fops, NULL);
        proc_create_data("RgmiiInfo", 0, rtl836x_proc_port_dir, &rgmiiInfo_proc_fops, NULL);
    }
}

