/*
 *	Ioctl handler
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

#include <linux/capability.h>
#include <linux/kernel.h>
#include <linux/if_bridge.h>
#include <linux/netdevice.h>
#include <linux/slab.h>
#include <linux/times.h>
#include <net/net_namespace.h>
#include <asm/uaccess.h>
#include "br_private.h"

#if defined (CONFIG_RTL_STP) || defined(CONFIG_RTL_HW_STP)
#include <net/rtl/rtl_nic.h>
#include <net/rtl/rtk_stp.h>
#endif

#ifdef CONFIG_RTK_GUEST_ZONE
#define panic_printk printk
#endif

#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
extern int br_register_igmpsnoopingmodule_process(struct net_bridge *br,int enable,int fldEnable);
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */


/* called with RTNL */
static int get_bridge_ifindices(struct net *net, int *indices, int num)
{
	struct net_device *dev;
	int i = 0;

	rcu_read_lock();
	for_each_netdev_rcu(net, dev) {
		if (i >= num)
			break;
		if (dev->priv_flags & IFF_EBRIDGE)
			indices[i++] = dev->ifindex;
	}
	rcu_read_unlock();

	return i;
}

/* called with RTNL */
static void get_port_ifindices(struct net_bridge *br, int *ifindices, int num)
{
	struct net_bridge_port *p;

	list_for_each_entry(p, &br->port_list, list) {
		if (p->port_no < num)
			ifindices[p->port_no] = p->dev->ifindex;
	}
}

/*
 * Format up to a page worth of forwarding table entries
 * userbuf -- where to copy result
 * maxnum  -- maximum number of entries desired
 *            (limited to a page for sanity)
 * offset  -- number of records to skip
 */
static int get_fdb_entries(struct net_bridge *br, void __user *userbuf,
			   unsigned long maxnum, unsigned long offset
#ifdef CONFIG_RTK_GUEST_ZONE
			   ,int for_guest
#endif
			   )
{
	int num;
	void *buf;
	size_t size;

	/* Clamp size to PAGE_SIZE, test maxnum to avoid overflow */
	if (maxnum > PAGE_SIZE/sizeof(struct __fdb_entry))
		maxnum = PAGE_SIZE/sizeof(struct __fdb_entry);

	size = maxnum * sizeof(struct __fdb_entry);

	buf = kmalloc(size, GFP_USER);
	if (!buf)
		return -ENOMEM;

#ifdef CONFIG_RTK_GUEST_ZONE
	num = br_fdb_fillbuf(br, buf, maxnum, offset, for_guest);
#else
	num = br_fdb_fillbuf(br, buf, maxnum, offset);
#endif
	if (num > 0) {
		if (copy_to_user(userbuf, buf, num*sizeof(struct __fdb_entry)))
			num = -EFAULT;
	}
	kfree(buf);

	return num;
}

/* called with RTNL */
static int add_del_if(struct net_bridge *br, int ifindex, int isadd)
{
	struct net *net = dev_net(br->dev);
	struct net_device *dev;
	int ret;

	if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
		return -EPERM;

	dev = __dev_get_by_index(net, ifindex);
	if (dev == NULL)
		return -EINVAL;

	if (isadd)
		ret = br_add_if(br, dev);
	else
		ret = br_del_if(br, dev);

	return ret;
}

/*
 * Legacy ioctl's through SIOCDEVPRIVATE
 * This interface is deprecated because it was too difficult to
 * to do the translation for 32/64bit ioctl compatibility.
 */
static int old_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct net_bridge *br = netdev_priv(dev);
	unsigned long args[4];

	if (copy_from_user(args, rq->ifr_data, sizeof(args)))
		return -EFAULT;

	switch (args[0]) {
	case BRCTL_ADD_IF:
	case BRCTL_DEL_IF:
		return add_del_if(br, args[1], args[0] == BRCTL_ADD_IF);

	case BRCTL_GET_BRIDGE_INFO:
	{
		struct __bridge_info b;

		memset(&b, 0, sizeof(struct __bridge_info));
		rcu_read_lock();
		memcpy(&b.designated_root, &br->designated_root, 8);
		memcpy(&b.bridge_id, &br->bridge_id, 8);
		b.root_path_cost = br->root_path_cost;
		b.max_age = jiffies_to_clock_t(br->max_age);
		b.hello_time = jiffies_to_clock_t(br->hello_time);
		b.forward_delay = br->forward_delay;
		b.bridge_max_age = br->bridge_max_age;
		b.bridge_hello_time = br->bridge_hello_time;
		b.bridge_forward_delay = jiffies_to_clock_t(br->bridge_forward_delay);
		b.topology_change = br->topology_change;
		b.topology_change_detected = br->topology_change_detected;
		b.root_port = br->root_port;

		b.stp_enabled = (br->stp_enabled != BR_NO_STP);
		b.ageing_time = jiffies_to_clock_t(br->ageing_time);
		b.hello_timer_value = br_timer_value(&br->hello_timer);
		b.tcn_timer_value = br_timer_value(&br->tcn_timer);
		b.topology_change_timer_value = br_timer_value(&br->topology_change_timer);
		b.gc_timer_value = br_timer_value(&br->gc_timer);
		rcu_read_unlock();

		if (copy_to_user((void __user *)args[1], &b, sizeof(b)))
			return -EFAULT;

		return 0;
	}

	case BRCTL_GET_PORT_LIST:
	{
		int num, *indices;

		num = args[2];
		if (num < 0)
			return -EINVAL;
		if (num == 0)
			num = 256;
		if (num > BR_MAX_PORTS)
			num = BR_MAX_PORTS;

		indices = kcalloc(num, sizeof(int), GFP_KERNEL);
		if (indices == NULL)
			return -ENOMEM;

		get_port_ifindices(br, indices, num);
		if (copy_to_user((void __user *)args[1], indices, num*sizeof(int)))
			num =  -EFAULT;
		kfree(indices);
		return num;
	}

	case BRCTL_SET_BRIDGE_FORWARD_DELAY:
		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		return br_set_forward_delay(br, args[1]);

	case BRCTL_SET_BRIDGE_HELLO_TIME:
		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		return br_set_hello_time(br, args[1]);

	case BRCTL_SET_BRIDGE_MAX_AGE:
		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		return br_set_max_age(br, args[1]);

	case BRCTL_SET_AGEING_TIME:
		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		br->ageing_time = clock_t_to_jiffies(args[1]);
		return 0;

	case BRCTL_GET_PORT_INFO:
	{
		struct __port_info p;
		struct net_bridge_port *pt;

		rcu_read_lock();
		if ((pt = br_get_port(br, args[2])) == NULL) {
			rcu_read_unlock();
			return -EINVAL;
		}

		memset(&p, 0, sizeof(struct __port_info));
		memcpy(&p.designated_root, &pt->designated_root, 8);
		memcpy(&p.designated_bridge, &pt->designated_bridge, 8);
		p.port_id = pt->port_id;
		p.designated_port = pt->designated_port;
		p.path_cost = pt->path_cost;
		p.designated_cost = pt->designated_cost;
		p.state = pt->state;
		p.top_change_ack = pt->topology_change_ack;
		p.config_pending = pt->config_pending;
		p.message_age_timer_value = br_timer_value(&pt->message_age_timer);
		p.forward_delay_timer_value = br_timer_value(&pt->forward_delay_timer);
		p.hold_timer_value = br_timer_value(&pt->hold_timer);

		rcu_read_unlock();

		if (copy_to_user((void __user *)args[1], &p, sizeof(p)))
			return -EFAULT;

		return 0;
	}

	case BRCTL_SET_BRIDGE_STP_STATE:
		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		br_stp_set_enabled(br, args[1]);

#if defined(CONFIG_RTL_STP) || defined(CONFIG_RTL_HW_STP)
		{
			int retval = 0;
			retval = rtl865x_setSpanningEnable(args[1] ? TRUE : FALSE);

			if (0 == retval)
				printk("RTL STP %s\n",args[1] ? "enabled" : "disabled");
			else
				printk("RTL STP set %s error=%d\n", args[1] ? "enabled" : "disabled", retval);
		}

#endif
#if 0//defined(CONFIG_RTL_HW_STP)
		{
			int retval = 0;
			retval = rtl865x_setSpanningEnable(args[1] ? TRUE : FALSE);
			if (0 == retval)
				printk("RTL STP %s\n", args[1] ? "enabled" : "disabled");
			else
				printk("RTL STP set %s error=%d", args[1] ? "enabled" : "disabled", retval);
		}
#endif

		return 0;

	case BRCTL_SET_BRIDGE_PRIORITY:
		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		br_stp_set_bridge_priority(br, args[1]);
		return 0;

	case BRCTL_SET_PORT_PRIORITY:
	{
		struct net_bridge_port *p;
		int ret;

		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		spin_lock_bh(&br->lock);
		if ((p = br_get_port(br, args[1])) == NULL)
			ret = -EINVAL;
		else
			ret = br_stp_set_port_priority(p, args[2]);
		spin_unlock_bh(&br->lock);
		return ret;
	}

	case BRCTL_SET_PATH_COST:
	{
		struct net_bridge_port *p;
		int ret;

		if (!ns_capable(dev_net(dev)->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		spin_lock_bh(&br->lock);
		if ((p = br_get_port(br, args[1])) == NULL)
			ret = -EINVAL;
		else
			ret = br_stp_set_path_cost(p, args[2]);
		spin_unlock_bh(&br->lock);

		return ret;
	}

	case BRCTL_GET_FDB_ENTRIES:
#ifdef CONFIG_RTK_GUEST_ZONE
		return get_fdb_entries(br, (void __user *)args[1],
				       args[2], args[3], 0);
#else
		return get_fdb_entries(br, (void __user *)args[1],
				       args[2], args[3]);
#endif

#ifdef CONFIG_RTK_GUEST_ZONE
	case 105:	// set zone
	{
		struct net_bridge_port *p;
		if ((p = br_get_port(br, args[1])) == NULL)
			return -EINVAL;
		p->zone_type = args[2];
#ifdef DEBUG_GUEST_ZONE
		GZDEBUG("set device=%s zone_type=%d\n", p->dev->name, p->zone_type);
#endif
		return 0;
	}

	case 106:	// set zone isolation
		br->is_zone_isolated = args[1];
#ifdef DEBUG_GUEST_ZONE
		GZDEBUG("set zone isolation=%d\n",	br->is_zone_isolated);
#endif
		return 0;

	case 107:	// set guest isolation
		br->is_guest_isolated = args[1];
#ifdef DEBUG_GUEST_ZONE
		GZDEBUG("set guest isolation=%d\n", br->is_guest_isolated);
#endif
		return 0;

	case 108:	// set lock mac list
	{
		unsigned char mac[6];
		int i;
		if (copy_from_user(mac, (unsigned long*)args[1], 6))
			return -EFAULT;
#ifdef DEBUG_GUEST_ZONE
		GZDEBUG("set lock client list=%02x:%02x:%02x:%02x:%02x:%02x\n",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif
		if (!memcmp(mac, "\x0\x0\x0\x0\x0\x0", 6)) {  // reset list
#ifdef DEBUG_GUEST_ZONE
			GZDEBUG("reset lock list!\n");
#endif
			br->lock_client_num = 0;
			return 0;
		}
		for (i = 0; i < br->lock_client_num; i++) {
			if (!memcmp(mac, br->lock_client_list[i], 6)) {
#ifdef DEBUG_GUEST_ZONE
				GZDEBUG("duplicated lock entry!\n");
#endif
				return 0;
			}
		}
		if (br->lock_client_num >= MAX_LOCK_CLIENT) {
#ifdef DEBUG_GUEST_ZONE
			GZDEBUG("Add failed, lock list table full!\n");
#endif
			return 0;
		}
		memcpy(br->lock_client_list[br->lock_client_num], mac, 6);
		br->lock_client_num++;
		return 0;
	}

	case 109:	// show guest info
	{
		int i;
		panic_printk("\n");
		panic_printk("  zone isolation: %d\n", br->is_zone_isolated);
		panic_printk("  guest isolation: %d\n", br->is_guest_isolated);
		i = 1;
		while (1) {
			struct net_bridge_port *p;
			if ((p = br_get_port(br, i++)) == NULL)
				break;
			switch (p->zone_type) {
			case ZONE_TYPE_HOST:
				panic_printk("  %s: host\n", p->dev->name);
				break;
			case ZONE_TYPE_GUEST:
				panic_printk("  %s: guest\n", p->dev->name);
				break;
			case ZONE_TYPE_GATEWAY:
				panic_printk("  %s: gateway\n", p->dev->name);
				break;
			default:
				panic_printk("  %s: unknown\n", p->dev->name);
				break;
			}
		}
		panic_printk("  locked client no: %d\n", br->lock_client_num);
		for (i = 0; i < br->lock_client_num; i++) {
			unsigned char *mac;
			mac = br->lock_client_list[i];
			panic_printk("    mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
				mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		}
		panic_printk("  gateway mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
			br->gateway_mac[0], br->gateway_mac[1], br->gateway_mac[2],
			br->gateway_mac[3], br->gateway_mac[4], br->gateway_mac[5]);
		panic_printk("\n");
		return 0;
	}

	case 110:
		return get_fdb_entries(br, (void __user *)args[1],
				args[2], args[3], 1);
	case 111:
	{
		unsigned char mac[6];

		if (copy_from_user(mac, (unsigned long*)args[1], 6))
			return -EFAULT;
#ifdef DEBUG_GUEST_ZONE
		GZDEBUG("set gateway mac=%02x:%02x:%02x:%02x:%02x:%02x\n",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif
		if (!memcmp(mac, "\x0\x0\x0\x0\x0\x0", 6)) {
#ifdef DEBUG_GUEST_ZONE
			GZDEBUG("reset gateway mac!\n");
#endif
			br->gateway_mac_set = 0;
		}
		else
			br->gateway_mac_set = 1;

		memcpy(br->gateway_mac, mac, 6);
		return 0;
	}
#endif	// CONFIG_RTK_GUEST_ZONE
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	case BRCTL_REGISTER_IGMPSNOOPING_MODULE:
	{
		br_register_igmpsnoopingmodule_process(br, args[1], args[2]);
		return 0;
	}
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */
	}

	return -EOPNOTSUPP;
}

#if defined(CONFIG_RTL_IGMP_SNOOPING)
extern int br_set_igmpProxy_pid(int pid);
#endif /* CONFIG_RTL_IGMP_SNOOPING */

static int old_deviceless(struct net *net, void __user *uarg)
{
	unsigned long args[3];

	if (copy_from_user(args, uarg, sizeof(args)))
		return -EFAULT;

	switch (args[0]) {
	case BRCTL_GET_VERSION:
		return BRCTL_VERSION;

	case BRCTL_GET_BRIDGES:
	{
		int *indices;
		int ret = 0;

		if (args[2] >= 2048)
			return -ENOMEM;
		indices = kcalloc(args[2], sizeof(int), GFP_KERNEL);
		if (indices == NULL)
			return -ENOMEM;

		args[2] = get_bridge_ifindices(net, indices, args[2]);

		ret = copy_to_user((void __user *)args[1], indices, args[2]*sizeof(int))
			? -EFAULT : args[2];

		kfree(indices);
		return ret;
	}

	case BRCTL_ADD_BRIDGE:
	case BRCTL_DEL_BRIDGE:
	{
		char buf[IFNAMSIZ];

		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(buf, (void __user *)args[1], IFNAMSIZ))
			return -EFAULT;

		buf[IFNAMSIZ-1] = 0;

		if (args[0] == BRCTL_ADD_BRIDGE)
			return br_add_bridge(net, buf);

		return br_del_bridge(net, buf);
	}
#if defined(CONFIG_RTL_IGMP_SNOOPING)
	case BRCTL_SET_IGMPPROXY_PID:
	{
		return br_set_igmpProxy_pid(args[1]);
	}
#endif /* CONFIG_RTL_IGMP_SNOOPING */

#if defined(CONFIG_RTK_MESH)
	case BRCTL_SET_MESH_PATHSELPID:
	{
		return br_set_meshpathsel_pid(args[1]);
	}
	case BRCTL_GET_MESH_PORTSTAT:
	{
		int ret = 0;
		ret = br_get_mesh_portstat();
		ret = copy_to_user((void __user *)args[1], &ret, sizeof(int))
					? -EFAULT : 0;
		return ret;
	}
#endif

	}

	return -EOPNOTSUPP;
}

int br_ioctl_deviceless_stub(struct net *net, unsigned int cmd, void __user *uarg)
{
	switch (cmd) {
	case SIOCGIFBR:
	case SIOCSIFBR:
		return old_deviceless(net, uarg);

	case SIOCBRADDBR:
	case SIOCBRDELBR:
	{
		char buf[IFNAMSIZ];

		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(buf, uarg, IFNAMSIZ))
			return -EFAULT;

		buf[IFNAMSIZ-1] = 0;
		if (cmd == SIOCBRADDBR)
			return br_add_bridge(net, buf);

		return br_del_bridge(net, buf);
	}
	}
	return -EOPNOTSUPP;
}

int br_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd)
{
	struct net_bridge *br = netdev_priv(dev);

	switch (cmd) {
	case SIOCDEVPRIVATE:
		return old_dev_ioctl(dev, rq, cmd);

	case SIOCBRADDIF:
	case SIOCBRDELIF:
		return add_del_if(br, rq->ifr_ifindex, cmd == SIOCBRADDIF);

	}

	br_debug(br, "Bridge does not support ioctl 0x%x\n", cmd);
	return -EOPNOTSUPP;
}
