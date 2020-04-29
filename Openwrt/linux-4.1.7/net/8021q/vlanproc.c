/******************************************************************************
 * vlanproc.c	VLAN Module. /proc filesystem interface.
 *
 *		This module is completely hardware-independent and provides
 *		access to the router using Linux /proc filesystem.
 *
 * Author:	Ben Greear, <greearb@candelatech.com> coppied from wanproc.c
 *               by: Gene Kozin	<genek@compuserve.com>
 *
 * Copyright:	(c) 1998 Ben Greear
 *
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 * ============================================================================
 * Jan 20, 1998        Ben Greear     Initial Version
 *****************************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include "vlanproc.h"
#include "vlan.h"
#ifdef CONFIG_RTL_VLAN_8021Q
#ifdef CONFIG_RTL_819X_SWCORE
#include <../../drivers/soc/realtek/rtd129x/hw_nat/common/rtl865x_vlan.h>
#include <../../drivers/soc/realtek/rtd129x/hw_nat/AsicDriver/rtl865x_asicCom.h>
#endif
#include <net/rtl/rtl865x_netif.h>
#endif

/****** Function Prototypes *************************************************/

/* Methods for preparing data for reading proc entries */
static int vlan_seq_show(struct seq_file *seq, void *v);
static void *vlan_seq_start(struct seq_file *seq, loff_t *pos);
static void *vlan_seq_next(struct seq_file *seq, void *v, loff_t *pos);
static void vlan_seq_stop(struct seq_file *seq, void *);
static int vlandev_seq_show(struct seq_file *seq, void *v);
#ifdef CONFIG_RTL_VLAN_8021Q
static int32 vlan_group_write( struct file *filp, const char *buff,unsigned long len,void *data);
static int32 vlan_group_read( struct seq_file *s, void *v);
static int32 vlan_pvid_write( struct file *filp, const char *buff,unsigned long len,void *data);
static int32 vlan_pvid_read(struct seq_file *s, void *v);
static int32 vlan_index_write( struct file *filp, const char *buff,unsigned long len,void *data);
static int32 vlan_index_read(struct seq_file *s, void *v);
static int32 vlan_ctl_write( struct file *filp, const char *buff,unsigned long len,void *data);
static int32 vlan_ctl_read(struct seq_file *s, void *v);
static int32 vlan_wan_vid_read(struct seq_file *s, void *v);
static int32 vlan_wan_vid_write( struct file *filp, const char *buff,unsigned long len,void *data);
static int32 vlan_hw_nat_lan_vid_read(struct seq_file *s, void *v);
static int32 vlan_hw_nat_lan_vid_write( struct file *filp, const char *buff,unsigned long len,void *data);
extern void rtk_default_eth_setting(void);
extern void rtk_init_11q_setting(void);
extern void rtk_add_eth_vlan(uint16 vid , uint32 portmask,uint32 tagmask,uint32 vlantype);
#if defined(CONFIG_RTD_1295_HWNAT)
extern unsigned int RTL_LANPORT_MASK;
#endif //defined(CONFIG_RTD_1295_HWNAT)

int vlan_group_single_open(struct inode *inode, struct file *file)
{
		return(single_open(file, vlan_group_read, PDE_DATA(inode)));
}
static ssize_t vlan_group_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return vlan_group_write(file, userbuf,count, off);
}
struct file_operations vlan_group_proc_fops= {
		.owner	 = THIS_MODULE,
		.open           = vlan_group_single_open,
		.write		    = vlan_group_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};

int vlan_pvid_single_open(struct inode *inode, struct file *file)
{
		return(single_open(file, vlan_pvid_read, PDE_DATA(inode)));
}
static ssize_t vlan_pvid_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return vlan_pvid_write(file, userbuf,count, off);
}
struct file_operations vlan_pvid_proc_fops= {
		.owner	 = THIS_MODULE,
		.open           = vlan_pvid_single_open,
		.write		    = vlan_pvid_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};

int vlan_index_single_open(struct inode *inode, struct file *file)
{
		return(single_open(file, vlan_index_read, PDE_DATA(inode)));
}
static ssize_t vlan_index_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return vlan_index_write(file, userbuf,count, off);
}
struct file_operations vlan_index_proc_fops= {
		.owner	 = THIS_MODULE,
		.open           = vlan_index_single_open,
		.write		    = vlan_index_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};

int vlan_ctl_single_open(struct inode *inode, struct file *file)
{
		return(single_open(file, vlan_ctl_read, PDE_DATA(inode)));
}
static ssize_t vlan_ctl_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return vlan_ctl_write(file, userbuf,count, off);
}
struct file_operations vlan_ctl_proc_fops= {
		.owner	 = THIS_MODULE,
		.open           = vlan_ctl_single_open,
		.write		    = vlan_ctl_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};

int vlan_wan_vid_single_open(struct inode *inode, struct file *file)
{
		return(single_open(file, vlan_wan_vid_read, PDE_DATA(inode)));
}
static ssize_t vlan_wan_vid_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return vlan_wan_vid_write(file, userbuf,count, off);
}
struct file_operations vlan_wan_vid_proc_fops= {
		.owner	 = THIS_MODULE,
		.open           = vlan_wan_vid_single_open,
		.write		    = vlan_wan_vid_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};

int vlan_hw_nat_lan_vid_single_open(struct inode *inode, struct file *file)
{
		return(single_open(file, vlan_hw_nat_lan_vid_read, PDE_DATA(inode)));
}
static ssize_t vlan_hw_nat_lan_vid_single_write(struct file * file, const char __user * userbuf,
		     size_t count, loff_t * off)
{
	    return vlan_hw_nat_lan_vid_write(file, userbuf,count, off);
}
struct file_operations vlan_hw_nat_lan_vid_proc_fops= {
		.owner	 = THIS_MODULE,
		.open           = vlan_hw_nat_lan_vid_single_open,
		.write		    = vlan_hw_nat_lan_vid_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};

#if defined(CONFIG_RTL_8367R_SUPPORT)
extern int rtk_vlan_portPvid_get(unsigned int port, unsigned int *pPvid, unsigned int *pPriority);
extern int rtk_vlan_portPvid_set(unsigned int port, unsigned int pvid, unsigned int priority);
extern int rtl_8367r_vlan_get(unsigned int i, unsigned int *mbrmsk, unsigned int *untagmsk, unsigned int *fid);
extern int rtl_vlan_RTL8367R_set(unsigned short vid, unsigned int tagmask, unsigned int mask, unsigned int fid);
#endif
extern int rtl865x_curOpMode;

linux_vlan_ctl_t vlan_ctl;
linux_vlan_ctl_t *vlan_ctl_p=&vlan_ctl;
static uint16 vlan_group_index;
int linux_vlan_enable;
int linux_vlan_hw_support_enable;
uint16 rtk_vlan_wan_vid = RTL_WANVLANID;
uint16 rtk_vlan_hw_nat_lan_vid = RTL_LANVLANID;
#define RTK_PHY_PORT_NUM  9

#define RTK_CPU_PORT_IDX 6
#define RTK_CPU_PORT_MASK  (1 << RTK_CPU_PORT_IDX )

#define GATEWAY_MODE 0
#define BRIDGE_MODE 1
#define WISP_MODE 2

EXPORT_SYMBOL(linux_vlan_enable);
EXPORT_SYMBOL(vlan_ctl_p);
EXPORT_SYMBOL(linux_vlan_hw_support_enable);
EXPORT_SYMBOL(rtk_vlan_wan_vid);
EXPORT_SYMBOL(rtk_vlan_hw_nat_lan_vid);
#endif
/*
 *	Global Data
 */


/*
 *	Names of the proc directory entries
 */

static const char name_root[]	 = "vlan";
static const char name_conf[]	 = "config";

/*
 *	Structures for interfacing with the /proc filesystem.
 *	VLAN creates its own directory /proc/net/vlan with the following
 *	entries:
 *	config		device status/configuration
 *	<device>	entry for each  device
 */

/*
 *	Generic /proc/net/vlan/<file> file and inode operations
 */

static const struct seq_operations vlan_seq_ops = {
	.start = vlan_seq_start,
	.next = vlan_seq_next,
	.stop = vlan_seq_stop,
	.show = vlan_seq_show,
};

static int vlan_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &vlan_seq_ops,
			sizeof(struct seq_net_private));
}

static const struct file_operations vlan_fops = {
	.owner	 = THIS_MODULE,
	.open    = vlan_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

/*
 *	/proc/net/vlan/<device> file and inode operations
 */

static int vlandev_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, vlandev_seq_show, PDE_DATA(inode));
}

static const struct file_operations vlandev_fops = {
	.owner = THIS_MODULE,
	.open    = vlandev_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

/*
 * Proc filesystem directory entries.
 */

/* Strings */
static const char *const vlan_name_type_str[VLAN_NAME_TYPE_HIGHEST] = {
    [VLAN_NAME_TYPE_RAW_PLUS_VID]        = "VLAN_NAME_TYPE_RAW_PLUS_VID",
    [VLAN_NAME_TYPE_PLUS_VID_NO_PAD]	 = "VLAN_NAME_TYPE_PLUS_VID_NO_PAD",
    [VLAN_NAME_TYPE_RAW_PLUS_VID_NO_PAD] = "VLAN_NAME_TYPE_RAW_PLUS_VID_NO_PAD",
    [VLAN_NAME_TYPE_PLUS_VID]		 = "VLAN_NAME_TYPE_PLUS_VID",
};
/*
 *	Interface functions
 */

/*
 *	Clean up /proc/net/vlan entries
 */

void vlan_proc_cleanup(struct net *net)
{
	struct vlan_net *vn = net_generic(net, vlan_net_id);

	if (vn->proc_vlan_conf)
		remove_proc_entry(name_conf, vn->proc_vlan_dir);

	if (vn->proc_vlan_dir)
		remove_proc_entry(name_root, net->proc_net);

	/* Dynamically added entries should be cleaned up as their vlan_device
	 * is removed, so we should not have to take care of it here...
	 */
}

/*
 *	Create /proc/net/vlan entries
 */

int __net_init vlan_proc_init(struct net *net)
{
	struct vlan_net *vn = net_generic(net, vlan_net_id);

	vn->proc_vlan_dir = proc_net_mkdir(net, name_root, net->proc_net);
	if (!vn->proc_vlan_dir)
		goto err;

	vn->proc_vlan_conf = proc_create(name_conf, S_IFREG|S_IRUSR|S_IWUSR,
				     vn->proc_vlan_dir, &vlan_fops);
	if (!vn->proc_vlan_conf)
		goto err;

	#ifdef CONFIG_RTL_VLAN_8021Q
	proc_create_data("vlanEnable",0,vn->proc_vlan_dir,&vlan_ctl_proc_fops,NULL);
	proc_create_data("vlanGroup",0,vn->proc_vlan_dir,&vlan_group_proc_fops,NULL);
	proc_create_data("groupIndex",0,vn->proc_vlan_dir,&vlan_index_proc_fops,NULL);
	proc_create_data("pvid",0,vn->proc_vlan_dir,&vlan_pvid_proc_fops,NULL);
	proc_create_data("wanVlanId",0,vn->proc_vlan_dir,&vlan_wan_vid_proc_fops,NULL);
	proc_create_data("hwNatLanVlanId",0,vn->proc_vlan_dir,&vlan_hw_nat_lan_vid_proc_fops,NULL);
	#endif

	return 0;

err:
	pr_err("can't create entry in proc filesystem!\n");
	vlan_proc_cleanup(net);
	return -ENOBUFS;
}

/*
 *	Add directory entry for VLAN device.
 */

int vlan_proc_add_dev(struct net_device *vlandev)
{
	struct vlan_dev_priv *vlan = vlan_dev_priv(vlandev);
	struct vlan_net *vn = net_generic(dev_net(vlandev), vlan_net_id);

	if (!strcmp(vlandev->name, name_conf))
		return -EINVAL;
	vlan->dent =
		proc_create_data(vlandev->name, S_IFREG|S_IRUSR|S_IWUSR,
				 vn->proc_vlan_dir, &vlandev_fops, vlandev);
	if (!vlan->dent)
		return -ENOBUFS;
	return 0;
}

/*
 *	Delete directory entry for VLAN device.
 */
int vlan_proc_rem_dev(struct net_device *vlandev)
{
	/** NOTE:  This will consume the memory pointed to by dent, it seems. */
	proc_remove(vlan_dev_priv(vlandev)->dent);
	vlan_dev_priv(vlandev)->dent = NULL;
	return 0;
}

/****** Proc filesystem entry points ****************************************/

/*
 * The following few functions build the content of /proc/net/vlan/config
 */

/* start read of /proc/net/vlan/config */
static void *vlan_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(rcu)
{
	struct net_device *dev;
	struct net *net = seq_file_net(seq);
	loff_t i = 1;

	rcu_read_lock();
	if (*pos == 0)
		return SEQ_START_TOKEN;

	for_each_netdev_rcu(net, dev) {
		if (!is_vlan_dev(dev))
			continue;

		if (i++ == *pos)
			return dev;
	}

	return  NULL;
}

static void *vlan_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	struct net_device *dev;
	struct net *net = seq_file_net(seq);

	++*pos;

	dev = v;
	if (v == SEQ_START_TOKEN)
		dev = net_device_entry(&net->dev_base_head);

	for_each_netdev_continue_rcu(net, dev) {
		if (!is_vlan_dev(dev))
			continue;

		return dev;
	}

	return NULL;
}

static void vlan_seq_stop(struct seq_file *seq, void *v)
	__releases(rcu)
{
	rcu_read_unlock();
}

static int vlan_seq_show(struct seq_file *seq, void *v)
{
	struct net *net = seq_file_net(seq);
	struct vlan_net *vn = net_generic(net, vlan_net_id);

	if (v == SEQ_START_TOKEN) {
		const char *nmtype = NULL;

		seq_puts(seq, "VLAN Dev name	 | VLAN ID\n");

		if (vn->name_type < ARRAY_SIZE(vlan_name_type_str))
		    nmtype =  vlan_name_type_str[vn->name_type];

		seq_printf(seq, "Name-Type: %s\n",
			   nmtype ? nmtype :  "UNKNOWN");
	} else {
		const struct net_device *vlandev = v;
		const struct vlan_dev_priv *vlan = vlan_dev_priv(vlandev);

		seq_printf(seq, "%-15s| %d  | %s\n",  vlandev->name,
			   vlan->vlan_id,    vlan->real_dev->name);
	}
	return 0;
}

static int vlandev_seq_show(struct seq_file *seq, void *offset)
{
	struct net_device *vlandev = (struct net_device *) seq->private;
	const struct vlan_dev_priv *vlan = vlan_dev_priv(vlandev);
	struct rtnl_link_stats64 temp;
	const struct rtnl_link_stats64 *stats;
	static const char fmt64[] = "%30s %12llu\n";
	int i;

	if (!is_vlan_dev(vlandev))
		return 0;

	stats = dev_get_stats(vlandev, &temp);
	seq_printf(seq,
		   "%s  VID: %d	 REORDER_HDR: %i  dev->priv_flags: %hx\n",
		   vlandev->name, vlan->vlan_id,
		   (int)(vlan->flags & 1), vlandev->priv_flags);

	seq_printf(seq, fmt64, "total frames received", stats->rx_packets);
	seq_printf(seq, fmt64, "total bytes received", stats->rx_bytes);
	seq_printf(seq, fmt64, "Broadcast/Multicast Rcvd", stats->multicast);
	seq_puts(seq, "\n");
	seq_printf(seq, fmt64, "total frames transmitted", stats->tx_packets);
	seq_printf(seq, fmt64, "total bytes transmitted", stats->tx_bytes);
	seq_printf(seq, "Device: %s", vlan->real_dev->name);
	/* now show all PRIORITY mappings relating to this VLAN */
	seq_printf(seq, "\nINGRESS priority mappings: "
			"0:%u  1:%u  2:%u  3:%u  4:%u  5:%u  6:%u 7:%u\n",
		   vlan->ingress_priority_map[0],
		   vlan->ingress_priority_map[1],
		   vlan->ingress_priority_map[2],
		   vlan->ingress_priority_map[3],
		   vlan->ingress_priority_map[4],
		   vlan->ingress_priority_map[5],
		   vlan->ingress_priority_map[6],
		   vlan->ingress_priority_map[7]);

	seq_printf(seq, " EGRESS priority mappings: ");
	for (i = 0; i < 16; i++) {
		const struct vlan_priority_tci_mapping *mp
			= vlan->egress_priority_map[i];
		while (mp) {
			seq_printf(seq, "%u:%hu ",
				   mp->priority, ((mp->vlan_qos >> 13) & 0x7));
			mp = mp->next;
		}
	}
	seq_puts(seq, "\n");

	return 0;
}

#ifdef CONFIG_RTL_VLAN_8021Q
static int32 vlan_pvid_read(struct seq_file *s, void *v)
{
	#if defined(CONFIG_RTL_8367R_SUPPORT)
	int i;
	unsigned int pvid = 0, priority = 0;
	#endif
	seq_printf(s, "e0~e8:%d,%d,%d,%d,%d,%d,%d,%d,%d\
		w0:%d,w0-va0~va3:%d,%d,%d,%d,w0-vxd:%d\
		w1:%d,w1-va0~va3:%d,%d,%d,%d,w1-vxd:%d\n",
		vlan_ctl_p->pvid[0],vlan_ctl_p->pvid[1],vlan_ctl_p->pvid[2],vlan_ctl_p->pvid[3],
		vlan_ctl_p->pvid[4],vlan_ctl_p->pvid[5],vlan_ctl_p->pvid[6],vlan_ctl_p->pvid[7],
		vlan_ctl_p->pvid[8],vlan_ctl_p->pvid[9],vlan_ctl_p->pvid[10],vlan_ctl_p->pvid[11],
		vlan_ctl_p->pvid[12],vlan_ctl_p->pvid[13],vlan_ctl_p->pvid[14],vlan_ctl_p->pvid[15],
		vlan_ctl_p->pvid[16],vlan_ctl_p->pvid[17],vlan_ctl_p->pvid[18],vlan_ctl_p->pvid[19],
		vlan_ctl_p->pvid[20]);
	#if defined(CONFIG_RTL_8367R_SUPPORT)
	seq_printf(s, "\n8367r asic %s\n", "pvid:");
	for(i=0;i<8;i++)
	{
		pvid = priority = 0;
		if (rtk_vlan_portPvid_get(i, &pvid, &priority)==0)
			seq_printf(s, "port %d pvid %u pri %u\n", i, pvid, priority);
	}
	#endif
	return 0;
}

static int32 vlan_index_read(struct seq_file *s, void *v)
{
	seq_printf(s, "%d\n",vlan_group_index);
	return 0;
}

static int32 vlan_ctl_read(struct seq_file *s, void *v)
{
	seq_printf(s, "vlan enable:%x hw support enable:%d\n",vlan_ctl_p->flag, linux_vlan_hw_support_enable);
	return 0;
}

static int32 vlan_group_read(struct seq_file *s, void *v)
{
	#if defined(CONFIG_RTL_8367R_SUPPORT)
	int i;
	unsigned int mbrmsk = 0, untagmsk = 0, fid = 0;
	#endif

	int vlan_id = 0;

	for(vlan_id=0; vlan_id<VLAN_GROUP_NUM; vlan_id++)
	{
		if(vlan_ctl_p->group[vlan_id].flag)
			seq_printf(s, "%d, %x, %x, %s\n",
			vlan_ctl_p->group[vlan_id].vid,
			vlan_ctl_p->group[vlan_id].memberMask,
			vlan_ctl_p->group[vlan_id].tagMemberMask,
			vlan_ctl_p->group[vlan_id].vlanType==VLAN_TYPE_NAT?"NAT":"Bridge"
			);
	}

	/*if(vlan_group_index<VLAN_GROUP_NUM)
		seq_printf(s, "%d,%x,%x,%d\n",
			vlan_ctl_p->group[vlan_group_index].flag,
			vlan_ctl_p->group[vlan_group_index].memberMask,
			vlan_ctl_p->group[vlan_group_index].tagMemberMask,
			vlan_ctl_p->group[vlan_group_index].vid);
	else
		seq_printf(s, "0,0,0,0\n");*/

	#if defined(CONFIG_RTL_8367R_SUPPORT)
	seq_printf(s, "\n%s\n", "8367r asic vlan:");
	#if 0
	for (i=0; i < 4096; i++)
	{
		mbrmsk = untagmsk = fid =0;
		if ((rtl_8367r_vlan_get(i, &mbrmsk, &untagmsk, &fid)== 0) && (mbrmsk != 0))
			//seq_printf(s, "vid %d mbrmsk 0x%x untagmsk 0x%x fid %u\n", i, mbrmsk, untagmsk, fid);
			printk("vid %d mbrmsk 0x%x untagmsk 0x%x fid %u\n", i, mbrmsk, untagmsk, fid);
	}
	#endif
	#endif
	return 0;
}

static int32 vlan_pvid_write( struct file *filp, const char *buff,unsigned long len,void *data)
{
	char tmpbuf[100];
	int num ;
	uint32 pvid[30];
	int i;
	if (buff && !copy_from_user(tmpbuf, buff, len))
	{
		num=sscanf(tmpbuf,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&pvid[0],&pvid[1],&pvid[2],&pvid[3],
			&pvid[4],&pvid[5],&pvid[6],&pvid[7],
			&pvid[8],&pvid[9],&pvid[10],&pvid[11],
			&pvid[12],&pvid[13],&pvid[14],&pvid[15],
			&pvid[16],&pvid[17],&pvid[18],&pvid[19],
			&pvid[20]);

		if(num>21){
			return len;
		}

		for(i=0;i<num;i++){
			vlan_ctl_p->pvid[i]=(uint16)pvid[i];

			#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
			if(rtl865x_curOpMode == GATEWAY_MODE)
			{
				if(!((1<<i) & RTL_WANPORT_MASK) && pvid[i]==rtk_vlan_wan_vid)
					pvid[i] = RTL_LANVLANID;
			}
			#endif

			/*add pvid info to switch*/
			if(i<9 &&  pvid[i])
				rtl8651_setAsicPvid(i, pvid[i]);
			#if defined(CONFIG_RTL_8367R_SUPPORT)
			if(i<9 &&  pvid[i]){
				rtk_vlan_portPvid_set(i, pvid[i], 0);
			}
			#endif
		}

	}
	return len;
}

static int32 vlan_index_write( struct file *filp, const char *buff,unsigned long len,void *data)
{
	char tmpbuf[20];
	int num ;
	uint32 index;

	if (buff && !copy_from_user(tmpbuf, buff, len))
	{
		num=sscanf(tmpbuf,"%d",&index);

		if(num!=1){
			return len;
		}

		vlan_group_index=index&0xfff;
	}
	return len;
}
static void rtk_init_vlan_tbl(void)
{
	int i, wan_port_idx;
   linux_vlan_group_t *vlan_group_p=NULL;

	 memset(&vlan_ctl,0,sizeof(linux_vlan_ctl_t));

	//default lan vlan
	  vlan_group_p=&vlan_ctl_p->group[RTL_LANVLANID];

	vlan_group_p->vid=(uint16)RTL_LANVLANID;
	vlan_group_p->memberMask= RTL_LANPORT_MASK;
	vlan_group_p->tagMemberMask= 0 ;
	vlan_group_p->vlanType = VLAN_TYPE_NAT;
	vlan_group_p->flag = 1;

	//default wan vlan
	  vlan_group_p=&vlan_ctl_p->group[RTL_WANVLANID];

	vlan_group_p->vid=(uint16)RTL_WANVLANID;
	vlan_group_p->memberMask= RTL_WANPORT_MASK;
	vlan_group_p->tagMemberMask= 0 ;
	vlan_group_p->vlanType = VLAN_TYPE_NAT;
	vlan_group_p->flag = 1;


	for( i=0; i< RTK_PHY_PORT_NUM ; i++)
		vlan_ctl_p->pvid[i] = RTL_LANVLANID;

	for(wan_port_idx=0; wan_port_idx<RTK_PHY_PORT_NUM; wan_port_idx++)
		if((1 << wan_port_idx) & RTL_WANPORT_MASK)
			break;

	if(wan_port_idx < RTK_PHY_PORT_NUM)
	{
		if(rtl865x_curOpMode == GATEWAY_MODE)
			vlan_ctl_p->pvid[wan_port_idx] = RTL_WANVLANID;
	}

	 vlan_ctl_p->pvid[RTK_CPU_PORT_IDX] = RTL_LANVLANID;   //mark_vc  , wan port pvid , wifi pvid ?
}

static int32 vlan_ctl_write( struct file *filp, const char *buff,unsigned long len,void *data)
{
	char tmpbuf[20];
	int num ;
	uint32 flag;
	uint32 vlanhwenable;

	if (buff && !copy_from_user(tmpbuf, buff, len))
	{
		num=sscanf(tmpbuf,"%d %d",&flag, &vlanhwenable);

		if(num!=2){
			return len;
		}

		//vlan_ctl_p->flag=(uint8)flag;
		linux_vlan_enable=flag;
		linux_vlan_hw_support_enable = vlanhwenable;

		rtk_init_vlan_tbl();
		vlan_ctl_p->flag=flag&0xff;

		rtk_vlan_wan_vid = RTL_WANVLANID;
		rtk_vlan_hw_nat_lan_vid = RTL_LANVLANID;

		rtk_default_eth_setting();
		rtl865xC_setNetDecisionPolicy(NETIF_PORT_BASED);

		//if vlan enable then reinit switch in rtk 11qvlan mode
		rtk_init_11q_setting();
	}

	return len;
}

static int32 vlan_group_write( struct file *filp, const char *buff,unsigned long len,void *data)
{

	char tmpbuf[100];
	int num ;
	uint32 flag;
	uint32 mask;
	uint32 tagMask;
	uint32 vid;
	uint32 vlanType;
	linux_vlan_group_t *vlan_group_p=NULL;

	if(!linux_vlan_enable)
	{
		printk("rtk 11q vlan support not enabled!!!\n");
		return len;
	}

	if (buff && !copy_from_user(tmpbuf, buff, len))
	{
		num=sscanf(tmpbuf,"%d,%x,%x,%d,%d",&flag,&mask,&tagMask,&vid,&vlanType);
		if(num!=5){
			return len;
		}

		if(vlan_group_index==(vid&0xfff)) {
			vlan_group_p=&vlan_ctl_p->group[vlan_group_index];

			/*clear the old one and delete old vlan group*/
			#ifdef CONFIG_RTL_819X_SWCORE
			if(vlan_group_p->vid && vlan_group_p->flag)
				rtl865x_delVlan(vlan_group_p->vid);
			#endif

			#if defined(CONFIG_RTL_8367R_SUPPORT)
			if(vlan_group_p->vid && vlan_group_p->flag)
			{
				if((vlan_group_p->memberMask &(1<<ETH_P4_MASK_BIT))&&(rtl865x_curOpMode == 0))//gate way mode
					rtl_vlan_RTL8367R_set(vlan_group_p->vid, 0, 0, 1);
				else
					rtl_vlan_RTL8367R_set(vlan_group_p->vid, 0, 0, 0);
			}
			#endif
			memset(vlan_group_p,0,sizeof(linux_vlan_group_t));

			#ifdef CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG
			if(rtl865x_curOpMode == GATEWAY_MODE)
				tagMask &= (RTL_WANPORT_MASK|RTK_CPU_PORT_MASK);
			#endif

			vlan_group_p->flag=(uint8)flag;
			vlan_group_p->memberMask=mask;
			vlan_group_p->tagMemberMask=tagMask;
			vlan_group_p->vid=(uint16)vid;
			vlan_group_p->vlanType = (uint8)vlanType;

			#ifdef CONFIG_RTL_819X_SWCORE
			if(vlan_group_p->flag) {
				if(!linux_vlan_hw_support_enable) {
					/*add vlan info to switch*/
					rtl865x_addVlan(vlan_group_p->vid);
					rtl865x_addVlanPortMember(vlan_group_p->vid,(mask&0x1ff)|0x100);
					rtl865x_setVlanPortTag(vlan_group_p->vid,tagMask&0x1ff, TRUE);

					if((mask&(1<<ETH_P4_MASK_BIT))&&(rtl865x_curOpMode == 0))//gate way mode
						rtl865x_setVlanFilterDatabase(vlan_group_p->vid,1);
					else
						rtl865x_setVlanFilterDatabase(vlan_group_p->vid,0);
					#if defined(CONFIG_RTL_8367R_SUPPORT)
					if((mask&(1<<ETH_P4_MASK_BIT))&&(rtl865x_curOpMode == 0))//gate way mode
						rtl_vlan_RTL8367R_set(vlan_group_p->vid, tagMask, mask, 1);
					else
						rtl_vlan_RTL8367R_set(vlan_group_p->vid, tagMask, mask, 0);
					#endif
				} else {
					rtk_add_eth_vlan((uint16)vid, mask&0x1ff, tagMask&0x1ff, vlanType);
				}
			}
			#endif
		}
	}

	return len;
}

static int32 vlan_wan_vid_read(struct seq_file *s, void *v)
{
	seq_printf(s, "rtk_wan_vid (NAT) = %d\n", rtk_vlan_wan_vid);
   return 0;
}

static int32 vlan_wan_vid_write( struct file *filp, const char *buff,unsigned long len,void *data)
{
	char tmpbuf[100];
	int num ;
	int vid;

	if(!linux_vlan_enable)
	{
		printk("rtk 11q vlan support not enabled!!!\n");
		return len;
	}

	if (buff && !copy_from_user(tmpbuf, buff, len))
	{
		num=sscanf(tmpbuf,"%d",&vid);
		if(num>1){
			return len;
		}
		if(vid)
			rtk_vlan_wan_vid = vid;
	}
	return len;
}

static int32 vlan_hw_nat_lan_vid_read(struct seq_file *s, void *v)
{
	seq_printf(s, "rtk_hw_nat_lan_vid = %d\n", rtk_vlan_hw_nat_lan_vid);
   return 0;
}

static int32 vlan_hw_nat_lan_vid_write( struct file *filp, const char *buff,unsigned long len,void *data)
{
	char tmpbuf[100];
	int num ;
	int vid;

	if(!linux_vlan_enable)
	{
		printk("rtk 11q vlan support not enabled!!!\n");
		return len;
	}

	if (buff && !copy_from_user(tmpbuf, buff, len))
	{
		num=sscanf(tmpbuf,"%d",&vid);
		if(num>1){
			return len;
		}
		if(vid)
			rtk_vlan_hw_nat_lan_vid = vid;
	}
	return len;
}

#endif
