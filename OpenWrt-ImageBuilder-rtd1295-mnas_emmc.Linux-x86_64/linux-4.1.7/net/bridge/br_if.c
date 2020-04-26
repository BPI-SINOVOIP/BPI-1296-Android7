/*
 *	Userspace interface
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/netpoll.h>
#include <linux/ethtool.h>
#include <linux/if_arp.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/rtnetlink.h>
#include <linux/if_ether.h>
#include <linux/slab.h>
#include <net/sock.h>
#include <linux/if_vlan.h>

#include "br_private.h"

#if defined(CONFIG_RTL_STP) || defined(CONFIG_RTL_HW_STP)
#include <net/rtl/rtk_stp.h>
#include "br_private_stp.h"
#endif

#if defined(CONFIG_RTL_IGMP_SNOOPING)
#include <net/rtl/rtl865x_igmpsnooping_glue.h>
#include <net/rtl/rtl865x_igmpsnooping.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>
struct net_bridge *bridge0 = NULL;
unsigned int brIgmpModuleIndex = 0xFFFFFFFF;
unsigned int br0SwFwdPortMask = 0xFFFFFFFF;
extern void br_mCastQueryTimerExpired(unsigned long arg);
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
struct net_bridge *bridge1 = NULL;
unsigned int brIgmpModuleIndex_2 = 0xFFFFFFFF;
unsigned int br1SwFwdPortMask = 0xFFFFFFFF;
#endif

#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
//#if defined(CONFIG_RTL_ISP_MULTIPLE_BR_SUPPORT)
struct igmg_register_info igmpRegInfo[RTL_IMGP_MAX_BRMODULE];
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#ifdef CONFIG_RTK_MESH
unsigned int mesh_port_no = 0xFFFFFFFF;
#endif

#endif /* CONFIG_RTL_IGMP_SNOOPING */


#if defined(CONFIG_BRIDGE_IGMP_SNOOPING)
#include <net/rtl/rtl_nic.h>
struct net_bridge *bridge0 = NULL;
#endif /* CONFIG_BRIDGE_IGMP_SNOOPING */

#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#if defined(CONFIG_RTL_IGMP_SNOOPING)
int rtl865x_generateBridgeDeviceInfo( struct net_bridge *br, rtl_multicastDeviceInfo_t *devInfo);
#endif /* CONFIG_RTL_IGMP_SNOOPING */
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */

/*
 * Determine initial path cost based on speed.
 * using recommendations from 802.1d standard
 *
 * Since driver might sleep need to not be holding any locks.
 */
static int port_cost(struct net_device *dev)
{
	struct ethtool_cmd ecmd;

	if (!__ethtool_get_settings(dev, &ecmd)) {
		switch (ethtool_cmd_speed(&ecmd)) {
		case SPEED_10000:
			return 2;
		case SPEED_1000:
			return 4;
		case SPEED_100:
			return 19;
		case SPEED_10:
			return 100;
		}
	}

	/* Old silly heuristics based on name */
	if (!strncmp(dev->name, "lec", 3))
		return 7;

	if (!strncmp(dev->name, "plip", 4))
		return 2500;

	return 100;	/* assume old 10Mbps */
}


/* Check for port carrier transitions. */
void br_port_carrier_check(struct net_bridge_port *p)
{
	struct net_device *dev = p->dev;
	struct net_bridge *br = p->br;

	if (!(p->flags & BR_ADMIN_COST) &&
		netif_running(dev) && netif_oper_up(dev))
		p->path_cost = port_cost(dev);

	if (!netif_running(br->dev))
		return;

	spin_lock_bh(&br->lock);
	if (netif_running(dev) && netif_oper_up(dev)) {
		if (p->state == BR_STATE_DISABLED)
			br_stp_enable_port(p);
	} else {
		if (p->state != BR_STATE_DISABLED)
			br_stp_disable_port(p);
	}
	spin_unlock_bh(&br->lock);
}

static void br_port_set_promisc(struct net_bridge_port *p)
{
	int err = 0;

	if (br_promisc_port(p))
		return;

	err = dev_set_promiscuity(p->dev, 1);
	if (err)
		return;

	br_fdb_unsync_static(p->br, p);
	p->flags |= BR_PROMISC;
}

static void br_port_clear_promisc(struct net_bridge_port *p)
{
	int err;

	/* Check if the port is already non-promisc or if it doesn't
	 * support UNICAST filtering.  Without unicast filtering support
	 * we'll end up re-enabling promisc mode anyway, so just check for
	 * it here.
	 */
	if (!br_promisc_port(p) || !(p->dev->priv_flags & IFF_UNICAST_FLT))
		return;

	/* Since we'll be clearing the promisc mode, program the port
	 * first so that we don't have interruption in traffic.
	 */
	err = br_fdb_sync_static(p->br, p);
	if (err)
		return;

	dev_set_promiscuity(p->dev, -1);
	p->flags &= ~BR_PROMISC;
}

/* When a port is added or removed or when certain port flags
 * change, this function is called to automatically manage
 * promiscuity setting of all the bridge ports.  We are always called
 * under RTNL so can skip using rcu primitives.
 */
void br_manage_promisc(struct net_bridge *br)
{
	struct net_bridge_port *p;
	bool set_all = false;

	/* If vlan filtering is disabled or bridge interface is placed
	 * into promiscuous mode, place all ports in promiscuous mode.
	 */
	if ((br->dev->flags & IFF_PROMISC) || !br_vlan_enabled(br))
		set_all = true;

	list_for_each_entry(p, &br->port_list, list) {
		if (set_all) {
			br_port_set_promisc(p);
		} else {
			/* If the number of auto-ports is <= 1, then all other
			 * ports will have their output configuration
			 * statically specified through fdbs.  Since ingress
			 * on the auto-port becomes forwarding/egress to other
			 * ports and egress configuration is statically known,
			 * we can say that ingress configuration of the
			 * auto-port is also statically known.
			 * This lets us disable promiscuous mode and write
			 * this config to hw.
			 */
			if (br->auto_cnt == 0 ||
				(br->auto_cnt == 1 && br_auto_port(p)))
				br_port_clear_promisc(p);
			else
				br_port_set_promisc(p);
		}
	}
}

static void nbp_update_port_count(struct net_bridge *br)
{
	struct net_bridge_port *p;
	u32 cnt = 0;

	list_for_each_entry(p, &br->port_list, list) {
		if (br_auto_port(p))
			cnt++;
	}
	if (br->auto_cnt != cnt) {
		br->auto_cnt = cnt;
		br_manage_promisc(br);
	}
}

static void nbp_delete_promisc(struct net_bridge_port *p)
{
	/* If port is currently promiscuous, unset promiscuity.
	 * Otherwise, it is a static port so remove all addresses
	 * from it.
	 */
	dev_set_allmulti(p->dev, -1);
	if (br_promisc_port(p))
		dev_set_promiscuity(p->dev, -1);
	else
		br_fdb_unsync_static(p->br, p);
}

static void release_nbp(struct kobject *kobj)
{
	struct net_bridge_port *p
		= container_of(kobj, struct net_bridge_port, kobj);
	kfree(p);
}

static struct kobj_type brport_ktype = {
#ifdef CONFIG_SYSFS
	.sysfs_ops = &brport_sysfs_ops,
#endif
	.release = release_nbp,
};

static void destroy_nbp(struct net_bridge_port *p)
{
	struct net_device *dev = p->dev;

	p->br = NULL;
	p->dev = NULL;
	dev_put(dev);

	kobject_put(&p->kobj);
}

static void destroy_nbp_rcu(struct rcu_head *head)
{
	struct net_bridge_port *p =
			container_of(head, struct net_bridge_port, rcu);
	destroy_nbp(p);
}

/* Delete port(interface) from bridge is done in two steps.
 * via RCU. First step, marks device as down. That deletes
 * all the timers and stops new packets from flowing through.
 *
 * Final cleanup doesn't occur until after all CPU's finished
 * processing packets.
 *
 * Protected from multiple admin operations by RTNL mutex
 */
static void del_nbp(struct net_bridge_port *p)
{
	struct net_bridge *br = p->br;
	struct net_device *dev = p->dev;

	sysfs_remove_link(br->ifobj, p->dev->name);

	nbp_delete_promisc(p);

	spin_lock_bh(&br->lock);
	br_stp_disable_port(p);
	spin_unlock_bh(&br->lock);

	br_ifinfo_notify(RTM_DELLINK, p);

	list_del_rcu(&p->list);

	nbp_vlan_flush(p);
	br_fdb_delete_by_port(br, p, 1);
	nbp_update_port_count(br);

	netdev_upper_dev_unlink(dev, br->dev);

	dev->priv_flags &= ~IFF_BRIDGE_PORT;

	netdev_rx_handler_unregister(dev);

	br_multicast_del_port(p);

	kobject_uevent(&p->kobj, KOBJ_REMOVE);
	kobject_del(&p->kobj);

	br_netpoll_disable(p);

	call_rcu(&p->rcu, destroy_nbp_rcu);
}

/* Delete bridge device */
void br_dev_delete(struct net_device *dev, struct list_head *head)
{
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *p, *n;

	list_for_each_entry_safe(p, n, &br->port_list, list) {
		del_nbp(p);
	}

	br_fdb_delete_by_port(br, NULL, 1);

	br_vlan_flush(br);
	del_timer_sync(&br->gc_timer);

	br_sysfs_delbr(br->dev);
	unregister_netdevice_queue(br->dev, head);
}

/* find an available port number */
static int find_portno(struct net_bridge *br)
{
	int index;
	struct net_bridge_port *p;
	unsigned long *inuse;

	inuse = kcalloc(BITS_TO_LONGS(BR_MAX_PORTS), sizeof(unsigned long),
			GFP_KERNEL);
	if (!inuse)
		return -ENOMEM;

	set_bit(0, inuse);	/* zero is reserved */
	list_for_each_entry(p, &br->port_list, list) {
		set_bit(p->port_no, inuse);
	}
	index = find_first_zero_bit(inuse, BR_MAX_PORTS);
	kfree(inuse);

	return (index >= BR_MAX_PORTS) ? -EXFULL : index;
}


#if defined(CONFIG_RTK_MESH) || defined(CONFIG_RTL_IGMP_SNOOPING)
struct net_bridge *find_br_by_name(char *name)
{
	struct net_bridge *br;
	struct net_device *dev = NULL;;

	dev = __dev_get_by_name(&init_net, name);
	if (dev == NULL)
	{
		return NULL;

	}
	else if (!(dev->priv_flags & IFF_EBRIDGE)) {

		return NULL;
	}
	else
	{
		br = netdev_priv(dev);
	}
	return br;
}
#endif

/* called with RTNL but without bridge lock */
static struct net_bridge_port *new_nbp(struct net_bridge *br,
					   struct net_device *dev)
{
	int index;
	struct net_bridge_port *p;

	index = find_portno(br);
	if (index < 0)
		return ERR_PTR(index);

	p = kzalloc(sizeof(*p), GFP_KERNEL);
	if (p == NULL)
		return ERR_PTR(-ENOMEM);

	p->br = br;
	dev_hold(dev);
	p->dev = dev;
	p->path_cost = port_cost(dev);
	p->priority = 0x8000 >> BR_PORT_BITS;
	p->port_no = index;
	p->flags = BR_LEARNING | BR_FLOOD;
	br_init_port(p);
	br_set_state(p, BR_STATE_DISABLED);

	#if defined(CONFIG_RTL_STP)
	rtl_setSpanningTreePortState(p, RTL8651_PORTSTA_DISABLED);
	#endif
	#if defined(CONFIG_RTL_HW_STP)
	rtl_sethwSpanningTreePortState(p, RTL8651_PORTSTA_DISABLED);
	#endif

	br_stp_port_timer_init(p);
	br_multicast_add_port(p);

	return p;
}

#if defined(CONFIG_RTL_IGMP_SNOOPING)
int br_set_igmpProxy_pid(int pid)
{
	struct net_device *dev;
	struct net_bridge *br = NULL;
	int ret = 0;

	dev = __dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
	if (dev == NULL)
	{
		ret = -ENXIO; 	/* Could not find device */

	}
	else if (!(dev->priv_flags & IFF_EBRIDGE)) {

		ret = -EPERM;
	}
	else
	{
		br = netdev_priv(dev);
	}

	if (br != NULL)
	{
		br->igmpProxy_pid = pid;
	}

	return 1;
}

#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
int rtl_check_br_igmpmodule(struct net_bridge *br)
{
	int ret = -1;
	int i = 0;

	if (br == NULL)
		return ret;


	for (i = 0;i < RTL_IMGP_MAX_BRMODULE; i++)
	{

		if (igmpRegInfo[i].valid)
		{

			if (igmpRegInfo[i].br == br)
			{
				ret = i;
				break;
			}
		}
	}
	return ret;
}

int rtl_find_valid_igmpmodule(struct net_bridge *br)
{
	unsigned int ret = 0xFFFFFFFF;
	int i = 0;

	for (i = 0;i < RTL_IMGP_MAX_BRMODULE; i++)
	{

		if (igmpRegInfo[i].valid == 0)
		{
			ret = i;
			break;
		}
	}

	return ret;
}

int rtl_check_brIgmpModuleName(char *name)
{
	int i = 0;
	int ret = -1;

	for (i = 0; i < RTL_IMGP_MAX_BRMODULE; i++)
	{

		if ((igmpRegInfo[i].valid) && (igmpRegInfo[i].br))
		{
			if (strcmp(igmpRegInfo[i].br->dev->name, name) == 0)
			{
				ret = i;
				break;
			}
		}
	}

	return ret;
}

int rtl_get_brIgmpModueIndx(struct net_bridge *br)
{
	int index = -1;

	index = rtl_check_br_igmpmodule(br);

	if ((index != -1) && (index < RTL_IMGP_MAX_BRMODULE))
	{
		return igmpRegInfo[index].moduleIndex;
	}
	return 0xFFFFFFFF;
}

int rtl_get_brIgmpModuleIndexbyName(char *name, int *index)
{
	int i = -1;

	i = rtl_check_brIgmpModuleName(name);
	*index = i;
//	printk("index:%x,[%s]:[%d].\n",index,__FUNCTION__,__LINE__);
	if ((i != -1) && (i < RTL_IMGP_MAX_BRMODULE))
	{
		return igmpRegInfo[i].moduleIndex;
	}
	return 0xFFFFFFFF;
}

int rtl_get_brIgmpModuleIndexbyId(int idx, char *name)
{
	int moduleIndex = 0xFFFFFFFF;
	struct net_bridge *br = NULL;

//	printk("index:%x,[%s]:[%d].\n",index,__FUNCTION__,__LINE__);
	if ((idx != -1) && idx < RTL_IMGP_MAX_BRMODULE)
	{
		if (igmpRegInfo[idx].valid) {
			moduleIndex = igmpRegInfo[idx].moduleIndex;
			br = igmpRegInfo[idx].br;
			if (br) {
				strcpy(name, br->dev->name);
			}
		}
	}
	return moduleIndex;
}

unsigned int rtl_get_brSwFwdPortMask(struct net_bridge *br)
{
	int index = -1;
	unsigned int brSwFwdPortMask = 0xFFFFFFFF;

	index = rtl_check_br_igmpmodule(br);
	if ((index != -1) && (index < RTL_IMGP_MAX_BRMODULE))
	{
		brSwFwdPortMask = igmpRegInfo[index].swFwdPortMask;
	}
	return brSwFwdPortMask;

}
unsigned int rtl_get_brSwFwdPortMaskByIndex(int index)
{
	unsigned int brSwFwdPortMask = 0xffffffff;

	if ((index != -1) && (index < RTL_IMGP_MAX_BRMODULE))
	{
		brSwFwdPortMask = igmpRegInfo[index].swFwdPortMask;
	}
	return brSwFwdPortMask;
}

struct net_bridge * rtl_get_brByIndex(int index, char *name)
{
	struct net_bridge *br = NULL;

	if ((index != -1) && (index < RTL_IMGP_MAX_BRMODULE))
	{
		if (igmpRegInfo[index].valid)
			br = igmpRegInfo[index].br;
		if (br)
			strcpy(name, br->dev->name);
	}
	return br;

}

int rtl_get_brqueryCntByName(char *name)
{
	int i = -1;

	i = rtl_check_brIgmpModuleName(name);
	//	printk("index:%x,[%s]:[%d].\n",index,__FUNCTION__,__LINE__);
	if ((i != -1) && (i < RTL_IMGP_MAX_BRMODULE))
	{
		return igmpRegInfo[i].mCastQueryTimerCnt;
	}
	return -1;

}
int rtl_set_brqueryCntByName(char *name, int cnt)
{
	int i = -1;

	i = rtl_check_brIgmpModuleName(name);
	//	printk("index:%x,[%s]:[%d].\n",index,__FUNCTION__,__LINE__);
	if ((i != -1) && (i < RTL_IMGP_MAX_BRMODULE))
	{
		igmpRegInfo[i].mCastQueryTimerCnt = cnt;
		return 1;
	}
	return 0;

}

int rtl_set_brSwFwdPortMask(struct net_bridge *br,unsigned int brSwFwdPortMask)
{
	int index = -1;

	index = rtl_check_br_igmpmodule(br);
	if ((index != -1) && (index < RTL_IMGP_MAX_BRMODULE))
	{
		igmpRegInfo[index].swFwdPortMask = brSwFwdPortMask;
	}
	return index;

}

int br_register_igmpsnoopingmodule_process(struct net_bridge *br,int igmpEnable, int fldEnable)
{
	int ret = -1;
	int index = 0xFFFFFFFF;
	rtl_multicastDeviceInfo_t brDevInfo;

	if (br == NULL)
		return ret;

	if (igmpEnable)
	{
		//register igmpsnooping module
		if (rtl_check_br_igmpmodule(br) != -1) //register before
			return ret;

		index = rtl_find_valid_igmpmodule(br);

		if (index >= RTL_IMGP_MAX_BRMODULE)
			return ret;
		igmpRegInfo[index].valid = 1;
		igmpRegInfo[index].br = br;
		//strcpy(igmpRegInfo[index].name,br->dev->name);
		memset(&brDevInfo, 0, sizeof(rtl_multicastDeviceInfo_t ));
		strcpy(brDevInfo.devName, br->dev->name);

		ret = rtl_registerIgmpSnoopingModule(&(igmpRegInfo[index].moduleIndex));
		#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
		if (ret == 0)
		{
			rtl_setIgmpSnoopingModuleDevInfo(igmpRegInfo[index].moduleIndex, &brDevInfo);
		}
		#endif

		if (fldEnable)
		{
			rtl_setIpv4UnknownMCastFloodMap(igmpRegInfo[index].moduleIndex, 0xFFFFFFFF);
			rtl_setIpv6UnknownMCastFloodMap(igmpRegInfo[index].moduleIndex, 0xFFFFFFFF);
		}
		else
		{
			rtl_setIpv4UnknownMCastFloodMap(igmpRegInfo[index].moduleIndex, 0x0);
			rtl_setIpv6UnknownMCastFloodMap(igmpRegInfo[index].moduleIndex, 0x0);
		}
		#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
		rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
		#endif
		rtl_setIgmpSnoopingModuleDevInfo(igmpRegInfo[index].moduleIndex, &brDevInfo);
		rtl_set_brSwFwdPortMask(br, brDevInfo.swPortMask);

		if (br != NULL)
		{
			init_timer(&br->mCastQuerytimer);
			br->mCastQuerytimer.data = br;
			//br->mCastQuerytimer.expires=jiffies+MCAST_QUERY_INTERVAL*HZ;

			br->mCastQuerytimer.expires = jiffies + 1 * HZ;
			br->mCastQuerytimer.function = (void*)br_mCastQueryTimerExpired;
			add_timer(&br->mCastQuerytimer);
		}



	}
	else
	{
		//unregister igmpsnooping module
		index = rtl_check_br_igmpmodule(br);
		if (index != -1)
		{
			rtl_unregisterIgmpSnoopingModule(igmpRegInfo[index].moduleIndex);
			#if defined(CONFIG_RTL_MLD_SNOOPING)
			if (br && timer_pending(&br->mCastQuerytimer))
			{
				del_timer(&br->mCastQuerytimer);
			}
			#endif
			igmpRegInfo[index].br = NULL;
			igmpRegInfo[index].valid = 0;
		}

	}

	return ret;
}

#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */

int br_add_bridge(struct net *net, const char *name)
{
	struct net_device *dev;
	int res;

	dev = alloc_netdev(sizeof(struct net_bridge), name, NET_NAME_UNKNOWN,
			   br_dev_setup);

	if (!dev)
		return -ENOMEM;
	dev_net_set(dev, net);
	dev->rtnl_link_ops = &br_link_ops;

	res = register_netdev(dev);
	if (res)
		free_netdev(dev);
#if defined(CONFIG_BRIDGE_IGMP_SNOOPING)
	if (res)
		return res;
	bridge0 = netdev_priv(dev);
#endif
#if defined(CONFIG_RTL_IGMP_SNOOPING)
	if (res)
		return res;
#if !defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)

	if (strcmp(name,RTL_PS_BR0_DEV_NAME) == 0)
	{
		rtl_multicastDeviceInfo_t devInfo;
		memset(&devInfo, 0, sizeof(rtl_multicastDeviceInfo_t ));
		strcpy(devInfo.devName, RTL_PS_BR0_DEV_NAME);

		res=rtl_registerIgmpSnoopingModule(&brIgmpModuleIndex);
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
		if (res==0)
		{
				rtl_setIgmpSnoopingModuleDevInfo(brIgmpModuleIndex, &devInfo);
		}
#endif
		bridge0 = netdev_priv(dev);
		if (bridge0 != NULL)
		{
			init_timer(&bridge0->mCastQuerytimer);
			bridge0->mCastQuerytimer.data = (unsigned long)bridge0;
			bridge0->mCastQuerytimer.expires = jiffies + MCAST_QUERY_INTERVAL * HZ;
			bridge0->mCastQuerytimer.function = (void*)br_mCastQueryTimerExpired;
			add_timer(&bridge0->mCastQuerytimer);
		}
		rtl_setIpv4UnknownMCastFloodMap(brIgmpModuleIndex, 0xFFFFFFFF);
#if defined(CONFIG_RTL_MLD_SNOOPING)
		rtl_setIpv6UnknownMCastFloodMap(brIgmpModuleIndex, 0xFFFFFFFF);
#endif
	}
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if (strcmp(name,RTL_PS_BR1_DEV_NAME) == 0)
	{
		rtl_multicastDeviceInfo_t devInfo2;
		memset(&devInfo2, 0, sizeof(rtl_multicastDeviceInfo_t ));
		strcpy(devInfo2.devName, RTL_PS_BR1_DEV_NAME);

		res = rtl_registerIgmpSnoopingModule(&brIgmpModuleIndex_2);
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
		if (res == 0)
		{
			 rtl_setIgmpSnoopingModuleDevInfo(brIgmpModuleIndex_2,&devInfo2);
		}
#endif
		bridge1 = netdev_priv(dev);
		if (bridge1 != NULL)
		{
			init_timer(&bridge1->mCastQuerytimer);
			bridge1->mCastQuerytimer.data = bridge1;
			bridge1->mCastQuerytimer.expires = jiffies + MCAST_QUERY_INTERVAL * HZ;
			bridge1->mCastQuerytimer.function = (void*)br_mCastQueryTimerExpired;
			add_timer(&bridge1->mCastQuerytimer);
		}
	}
#endif
#else /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
	br_register_igmpsnoopingmodule_process(netdev_priv(dev), 1, 1);
#endif /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */
	return res;
}

int br_del_bridge(struct net *net, const char *name)
{
	struct net_device *dev;
	int ret = 0;

	rtnl_lock();
	dev = __dev_get_by_name(net, name);
	if (dev == NULL)
		ret =  -ENXIO; 	/* Could not find device */

	else if (!(dev->priv_flags & IFF_EBRIDGE)) {
		/* Attempt to delete non bridge device! */
		ret = -EPERM;
	}

	else if (dev->flags & IFF_UP) {
		/* Not shutdown yet. */
		ret = -EBUSY;
	}

	else {
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
		br_register_igmpsnoopingmodule_process(netdev_priv(dev), 0, 0);
#endif
#endif
		br_dev_delete(dev, NULL);
	}
#if defined(CONFIG_BRIDGE_IGMP_SNOOPING)&&defined(CONFIG_RTL_HARDWARE_MULTICAST)
	bridge0 = NULL;
#endif
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if !defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)

	if (ret)
		goto outfree;
	if (strcmp(name,RTL_PS_BR0_DEV_NAME) == 0)
	{
		rtl_unregisterIgmpSnoopingModule(brIgmpModuleIndex);
#if defined(CONFIG_RTL_MLD_SNOOPING)
		if (bridge0 && timer_pending(&bridge0->mCastQuerytimer))
		{
			del_timer(&bridge0->mCastQuerytimer);
		}
#endif /* CONFIG_RTL_MLD_SNOOPING */
		bridge0 = NULL;
	}
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if (strcmp(name,RTL_PS_BR1_DEV_NAME) == 0)
	{
		rtl_unregisterIgmpSnoopingModule(brIgmpModuleIndex_2);
#if defined(CONFIG_RTL_MLD_SNOOPING)
		if (bridge1 && timer_pending(&bridge1->mCastQuerytimer))
		{
			del_timer(&bridge1->mCastQuerytimer);
		}
#endif
		bridge1=NULL;
	}
#endif
outfree:
#endif /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */

	rtnl_unlock();
	return ret;
}

/* MTU of the bridge pseudo-device: ETH_DATA_LEN or the minimum of the ports */
int br_min_mtu(const struct net_bridge *br)
{
	const struct net_bridge_port *p;
	int mtu = 0;

	ASSERT_RTNL();

	if (list_empty(&br->port_list))
		mtu = ETH_DATA_LEN;
	else {
		list_for_each_entry(p, &br->port_list, list) {
			if (!mtu  || p->dev->mtu < mtu)
				mtu = p->dev->mtu;
		}
	}
	return mtu;
}

/*
 * Recomputes features using slave's features
 */
netdev_features_t br_features_recompute(struct net_bridge *br,
	netdev_features_t features)
{
	struct net_bridge_port *p;
	netdev_features_t mask;

	if (list_empty(&br->port_list))
		return features;

	mask = features;
	features &= ~NETIF_F_ONE_FOR_ALL;

	list_for_each_entry(p, &br->port_list, list) {
		features = netdev_increment_features(features,
							 p->dev->features, mask);
	}
	features = netdev_add_tso_features(features, mask);

	return features;
}

/* called with RTNL */
int br_add_if(struct net_bridge *br, struct net_device *dev)
{
	struct net_bridge_port *p;
	int err = 0;
	bool changed_addr;
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	unsigned int igmpModuleIndex = 0xFFFFFFFF;
	rtl_multicastDeviceInfo_t brDevInfo;
#endif


	/* Don't allow bridging non-ethernet like devices, or DSA-enabled
	 * master network devices since the bridge layer rx_handler prevents
	 * the DSA fake ethertype handler to be invoked, so we do not strip off
	 * the DSA switch tag protocol header and the bridge layer just return
	 * RX_HANDLER_CONSUMED, stopping RX processing for these frames.
	 */
	if ((dev->flags & IFF_LOOPBACK) ||
		dev->type != ARPHRD_ETHER || dev->addr_len != ETH_ALEN ||
		!is_valid_ether_addr(dev->dev_addr) ||
		netdev_uses_dsa(dev))
		return -EINVAL;

	/* No bridging of bridges */
	if (dev->netdev_ops->ndo_start_xmit == br_dev_xmit)
		return -ELOOP;

	/* Device is already being bridged */
	if (br_port_exists(dev))
		return -EBUSY;

	/* No bridging devices that dislike that (e.g. wireless) */
	if (dev->priv_flags & IFF_DONT_BRIDGE)
		return -EOPNOTSUPP;

	p = new_nbp(br, dev);
	if (IS_ERR(p))
		return PTR_ERR(p);

	call_netdevice_notifiers(NETDEV_JOIN, dev);

	err = dev_set_allmulti(dev, 1);
	if (err)
		goto put_back;

	err = kobject_init_and_add(&p->kobj, &brport_ktype, &(dev->dev.kobj),
				   SYSFS_BRIDGE_PORT_ATTR);
	if (err)
		goto err1;

	err = br_sysfs_addif(p);
	if (err)
		goto err2;

	err = br_netpoll_enable(p);
	if (err)
		goto err3;

	err = netdev_rx_handler_register(dev, br_handle_frame, p);
	if (err)
		goto err4;

	dev->priv_flags |= IFF_BRIDGE_PORT;

	err = netdev_master_upper_dev_link(dev, br->dev);
	if (err)
		goto err5;

	dev_disable_lro(dev);

	list_add_rcu(&p->list, &br->port_list);

	nbp_update_port_count(br);

	netdev_update_features(br->dev);

	if (br->dev->needed_headroom < dev->needed_headroom)
		br->dev->needed_headroom = dev->needed_headroom;

	if (br_fdb_insert(br, p, dev->dev_addr, 0))
		netdev_err(dev, "failed insert local address bridge forwarding table\n");

	if (nbp_vlan_init(p))
		netdev_err(dev, "failed to initialize vlan filtering on this port\n");

	spin_lock_bh(&br->lock);
	changed_addr = br_stp_recalculate_bridge_id(br);

	if (netif_running(dev) && netif_oper_up(dev) &&
		(br->dev->flags & IFF_UP))
		br_stp_enable_port(p);
	spin_unlock_bh(&br->lock);

	br_ifinfo_notify(RTM_NEWLINK, p);

	if (changed_addr)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, br->dev);

	dev_set_mtu(br->dev, br_min_mtu(br));

	kobject_uevent(&p->kobj, KOBJ_ADD);
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if !defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)

	if (strcmp(br->dev->name, RTL_PS_BR0_DEV_NAME) == 0)
	{
		rtl_multicastDeviceInfo_t brDevInfo;
		rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
		if (brIgmpModuleIndex != 0xFFFFFFFF)
		{
			rtl_setIgmpSnoopingModuleDevInfo(brIgmpModuleIndex, &brDevInfo);
		}
		br0SwFwdPortMask = brDevInfo.swPortMask;
	}
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if (strcmp(br->dev->name, RTL_PS_BR1_DEV_NAME) == 0)
	{
		rtl_multicastDeviceInfo_t brDevInfo;
		rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
		if (brIgmpModuleIndex != 0xFFFFFFFF)
		{
			rtl_setIgmpSnoopingModuleDevInfo(brIgmpModuleIndex_2, &brDevInfo);
		}
		br1SwFwdPortMask = brDevInfo.swPortMask;
	}
#endif/*CONFIG_RTK_VLAN_WAN_TAG_SUPPORT*/
#else /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
//update swPortMask
	igmpModuleIndex = rtl_get_brIgmpModueIndx(br);
	if (igmpModuleIndex != 0xFFFFFFFF)
	{
		rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
		rtl_setIgmpSnoopingModuleDevInfo(igmpModuleIndex, &brDevInfo);
		rtl_set_brSwFwdPortMask(br, brDevInfo.swPortMask);
	}

#endif /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
extern int rtk_dev_name_netif_mapping(char *br_name, char *br_port_name, int flag);
rtk_dev_name_netif_mapping(br->dev->name, dev->name, 1);
#endif

#ifdef CONFIG_RTK_MESH
	if (strcmp(dev->name, "wlan-msh") == 0) {
		mesh_port_no = p->port_no;
	}
#endif

	return 0;

err5:
	dev->priv_flags &= ~IFF_BRIDGE_PORT;
	netdev_rx_handler_unregister(dev);
err4:
	br_netpoll_disable(p);
err3:
	sysfs_remove_link(br->ifobj, p->dev->name);
err2:
	kobject_put(&p->kobj);
	p = NULL; /* kobject_put frees */
err1:
	dev_set_allmulti(dev, -1);
put_back:
	dev_put(dev);
	kfree(p);
	return err;
}

/* called with RTNL */
int br_del_if(struct net_bridge *br, struct net_device *dev)
{
	struct net_bridge_port *p;
	bool changed_addr;
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	unsigned int igmpModuleIndex = 0xFFFFFFFF;
	rtl_multicastDeviceInfo_t brDevInfo;
#endif

	p = br_port_get_rtnl(dev);
	if (!p || p->br != br)
		return -EINVAL;



#if defined(CONFIG_RTK_MESH)
	if (strcmp(dev->name, "wlan-msh") == 0) {
		mesh_port_no = 0xFFFFFFFF;
	}
#endif



	/* Since more than one interface can be attached to a bridge,
	 * there still maybe an alternate path for netconsole to use;
	 * therefore there is no reason for a NETDEV_RELEASE event.
	 */
	del_nbp(p);

	dev_set_mtu(br->dev, br_min_mtu(br));

	spin_lock_bh(&br->lock);
	changed_addr = br_stp_recalculate_bridge_id(br);
	spin_unlock_bh(&br->lock);

	if (changed_addr)
		call_netdevice_notifiers(NETDEV_CHANGEADDR, br->dev);

	netdev_update_features(br->dev);
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if !defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)

	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) == 0)
	{
		rtl_multicastDeviceInfo_t brDevInfo;
		rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
		if (brIgmpModuleIndex != 0xFFFFFFFF)
		{
			rtl_setIgmpSnoopingModuleDevInfo(brIgmpModuleIndex, &brDevInfo);
		}
		br0SwFwdPortMask = brDevInfo.swPortMask;
	}
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
		if (strcmp(br->dev->name, RTL_PS_BR1_DEV_NAME)==0)
		{
			rtl_multicastDeviceInfo_t brDevInfo;
			rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
			if (brIgmpModuleIndex != 0xFFFFFFFF)
			{
				rtl_setIgmpSnoopingModuleDevInfo(brIgmpModuleIndex_2, &brDevInfo);
			}
			br1SwFwdPortMask = brDevInfo.swPortMask;
		}
#endif/*CONFIG_RTK_VLAN_WAN_TAG_SUPPORT*/
#else /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
	igmpModuleIndex = rtl_get_brIgmpModueIndx(br);
	if (igmpModuleIndex != 0xFFFFFFFF)
	{
		rtl865x_generateBridgeDeviceInfo(br, &brDevInfo);
		rtl_setIgmpSnoopingModuleDevInfo(igmpModuleIndex, &brDevInfo);
		rtl_set_brSwFwdPortMask(br, brDevInfo.swPortMask);
	}
#endif /* !CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	extern int rtk_dev_name_netif_mapping(char *br_name, char *br_port_name, int flag);
	rtk_dev_name_netif_mapping(br->dev->name, dev->name, 2);
#endif

	return 0;
}

void br_port_flags_change(struct net_bridge_port *p, unsigned long mask)
{
	struct net_bridge *br = p->br;

	if (mask & BR_AUTO_MASK)
		nbp_update_port_count(br);
}
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#if defined(CONFIG_RTL_IGMP_SNOOPING)
int rtl865x_generateBridgeDeviceInfo(struct net_bridge *br, rtl_multicastDeviceInfo_t *devInfo)
{
	struct net_bridge_port *p, *n;
	if ((br == NULL) || (devInfo == NULL))
	{
		return -1;
	}

	memset(devInfo, 0, sizeof(rtl_multicastDeviceInfo_t));
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	int index = -1;
	index = rtl_check_br_igmpmodule(br);
	//printk("br:%s,index:%x,[%s]:[%d].\n",br->dev->name,index,__FUNCTION__,__LINE__);
	if ((index == -1) || (index >= RTL_IMGP_MAX_BRMODULE))
#else /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) != 0 && strcmp(br->dev->name,RTL_PS_BR1_DEV_NAME) != 0 )
#else
	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) != 0)
#endif
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
	{
		return -1;
	}

	strcpy(devInfo->devName, br->dev->name);


	list_for_each_entry_safe(p, n, &br->port_list, list)
	{
		if (memcmp(p->dev->name, RTL_PS_ETH_NAME,3) != 0)
		{
			devInfo->swPortMask |= (1<<p->port_no);
		}
		devInfo->portMask |= (1 << p->port_no);
	}

	return 0;
}
#endif /* CONFIG_RTL_IGMP_SNOOPING */
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */

#if defined(CONFIG_RTK_MESH)
int br_set_meshpathsel_pid(int pid)
{
	struct net_device *dev = NULL;
	struct net_bridge *br = NULL;

	int ret = 0;

	dev = __dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
	if (dev == NULL)
	{
		ret = -ENXIO; 	/* Could not find device */

	}
	else if (!(dev->priv_flags & IFF_EBRIDGE)) {

		ret = -EPERM;
	}
	else
	{
		br = netdev_priv(dev);
	}

	if (br != NULL)
		br->mesh_pathsel_pid = pid;

	printk("Receive Pathsel daemon pid:%d\n", br->mesh_pathsel_pid);
	return ret;
}

int br_get_mesh_portstat(void)
{
	struct net_device *dev = NULL;
	struct net_bridge *br = NULL;
	struct net_bridge_port *p;

	int ret = 0;

	dev = __dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
	if (dev == NULL)
	{
		ret =  -ENXIO; 	/* Could not find device */

	}
	else if (!(dev->priv_flags & IFF_EBRIDGE)) {

		ret = -EPERM;
	}
	else
	{
		br = netdev_priv(dev);
	}

	if (br != NULL) {
		list_for_each_entry(p, &br->port_list, list){
			if (strstr(p->dev->name, "msh")) {
				return p->state;
			}
		}
	}
	return -1;
}


void br_signal_pathsel(struct net_bridge *br)
{
	struct task_struct *task;
	if (br == NULL || br->mesh_pathsel_pid == 0)
	{
		return;
	}

	read_lock(&tasklist_lock);
	task = find_task_by_vpid(br->mesh_pathsel_pid);
	read_unlock(&tasklist_lock);
	if (task) {
		printk("Send SIGUSR2 signal from kernel to pathsel\n");
		send_sig(SIGUSR2, task, 0);
	} else {
		printk("Path selection daemon pid: %d does not exist\n", br->mesh_pathsel_pid);
	}
}

#endif

