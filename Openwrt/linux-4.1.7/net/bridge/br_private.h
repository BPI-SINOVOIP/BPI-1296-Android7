/*
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

#ifndef _BR_PRIVATE_H
#define _BR_PRIVATE_H

#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/netpoll.h>
#include <linux/u64_stats_sync.h>
#include <net/route.h>
#include <linux/if_vlan.h>

#if defined(CONFIG_RTL_IGMP_SNOOPING) || defined(CONFIG_BRIDGE_IGMP_SNOOPING)
//#define DEBUG_PRINT(fmt, args...) printk(fmt, ## args)
#define DEBUG_PRINT(fmt, args...)

#define IGMP_EXPIRE_TIME (260 * HZ)
#define M2U_DELAY_DELETE_TIME (2 * HZ)
#endif /* CONFIG_RTL_IGMP_SNOOPING || CONFIG_BRIDGE_IGMP_SNOOPING */

#if defined(CONFIG_RTL_IGMP_SNOOPING)
#define MULTICAST_MAC(mac)	((mac[0] == 0x01) && (mac[1] == 0x00) && (mac[2] == 0x5e))
#define IPV6_MULTICAST_MAC(mac)	((mac[0] == 0x33) && (mac[1] == 0x33) && mac[2] != 0xff)
//#define CONFIG_BRIDGE_IGMPV3_SNOOPING

#define MCAST_TO_UNICAST

#if defined(MCAST_TO_UNICAST)
#define IPV6_MCAST_TO_UNICAST
#endif /* MCAST_TO_UNICAST */
#ifndef M2U_DELETE_CHECK
#define M2U_DELETE_CHECK
#endif
#define MLCST_FLTR_ENTRY	16
#define MLCST_MAC_ENTRY		64

extern int rtk_vlan_support_enable;
// interface to set multicast bandwidth control
//#define MULTICAST_BWCTRL

// interface to enable MAC clone function
//#define RTL_BRIDGE_MAC_CLONE
//#define RTL_BRIDGE_DEBUG

#define MCAST_QUERY_INTERVAL 30
#endif/*CONFIG_RTL_IGMP_SNOOPING*/

#ifdef CONFIG_RTL_LAYERED_DRIVER_L2
#define CONFIG_RTL865X_ETH
#endif

#define BR_HASH_BITS 8
#define BR_HASH_SIZE (1 << BR_HASH_BITS)

#define BR_HOLD_TIME (1*HZ)

#define BR_PORT_BITS	10
#define BR_MAX_PORTS	(1<<BR_PORT_BITS)
#define BR_VLAN_BITMAP_LEN	BITS_TO_LONGS(VLAN_N_VID)

#define BR_VERSION	"2.3"

/* Control of forwarding link local multicast */
#define BR_GROUPFWD_DEFAULT	0
/* Don't allow forwarding control protocols like STP and LLDP */
#define BR_GROUPFWD_RESTRICTED	0x4007u
/* The Nearest Customer Bridge Group Address, 01-80-C2-00-00-[00,0B,0C,0D,0F] */
#define BR_GROUPFWD_8021AD	0xB801u

/* Path to usermode spanning tree program */
#define BR_STP_PROG	"/sbin/bridge-stp"

typedef struct bridge_id bridge_id;
typedef struct mac_addr mac_addr;
typedef __u16 port_id;
#if defined(CONFIG_BRIDGE_IGMP_SNOOPING)
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)

#define DEBUG_PRINT(fmt, args...)
#define MULTICAST_MAC(mac)	((mac[0] == 0x01) && (mac[1] == 0x00) && (mac[2] == 0x5e))
#define IPV6_MULTICAST_MAC(mac)	((mac[0] == 0x33) && (mac[1] == 0x33) && mac[2] != 0xff)
#define MCAST_TO_UNICAST
#if defined(MCAST_TO_UNICAST)
#define IPV6_MCAST_TO_UNICAST
#endif /* MCAST_TO_UNICAST */
struct multicastFwdInfo
{
	unsigned int fwdPortMask;
	char toCpu;
};
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */
#endif /* CONFIG_BRIDGE_IGMP_SNOOPING */

//#define CONFIG_RTK_GUEST_ZONE
#ifdef CONFIG_RTK_GUEST_ZONE
	#define MAX_LOCK_CLIENT		32
//	#define DEBUG_GUEST_ZONE

	enum ZONE_TYPE {
		ZONE_TYPE_HOST,
		ZONE_TYPE_GUEST,
		ZONE_TYPE_GATEWAY
	};
#endif


//#define DEBUG_GUEST_ZONE
#ifdef DEBUG_GUEST_ZONE
#define GZDEBUG(fmt, args...) panic_printk("[%s %d]"fmt,__FUNCTION__,__LINE__,## args)
#else
#define GZDEBUG(fmt, args...) {}
#endif
#if defined(CONFIG_RTL_IGMP_SNOOPING) || defined(CONFIG_BRIDGE_IGMP_SNOOPING)
#define FDB_IGMP_EXT_NUM 8
struct fdb_igmp_ext_entry
{
	int valid;
	unsigned long ageing_time;
	unsigned char SrcMac[6];
	unsigned char port;

};

struct fdb_igmp_ext_array
{
	struct fdb_igmp_ext_entry igmp_fdb_arr[FDB_IGMP_EXT_NUM];
};

#endif /* CONFIG_RTL_IGMP_SNOOPING || CONFIG_BRIDGE_IGMP_SNOOPING */

struct bridge_id
{
	unsigned char	prio[2];
	unsigned char	addr[ETH_ALEN];
};

struct mac_addr
{
	unsigned char	addr[ETH_ALEN];
};

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
/* our own querier */
struct bridge_mcast_own_query {
	struct timer_list	timer;
	u32			startup_sent;
};

/* other querier */
struct bridge_mcast_other_query {
	struct timer_list		timer;
	unsigned long			delay_time;
};

/* selected querier */
struct bridge_mcast_querier {
	struct br_ip addr;
	struct net_bridge_port __rcu	*port;
};
#endif

struct net_port_vlans {
	u16				port_idx;
	u16				pvid;
	union {
		struct net_bridge_port		*port;
		struct net_bridge		*br;
	}				parent;
	struct rcu_head			rcu;
	unsigned long			vlan_bitmap[BR_VLAN_BITMAP_LEN];
	unsigned long			untagged_bitmap[BR_VLAN_BITMAP_LEN];
	u16				num_vlans;
};

struct net_bridge_fdb_entry
{
	struct hlist_node		hlist;
	struct net_bridge_port		*dst;

	struct rcu_head			rcu;
	unsigned long			updated;
#if defined(CONFIG_RTL_IGMP_SNOOPING) || defined(CONFIG_BRIDGE_IGMP_SNOOPING)
	unsigned short			group_src;
	unsigned char			igmpFlag;
	unsigned char			portlist;
	int				portUsedNum[8];	// be used with portlist, for record each port has how many client
	struct fdb_igmp_ext_entry igmp_fdb_arr[FDB_IGMP_EXT_NUM];
#endif /* CONFIG_RTL_IGMP_SNOOPING || CONFIG_BRIDGE_IGMP_SNOOPING */
	unsigned long			used;
	mac_addr			addr;
	unsigned char			is_local:1,
					is_static:1,
					added_by_user:1,
					added_by_external_learn:1;
	__u16				vlan_id;
};

struct net_bridge_port_group {
	struct net_bridge_port		*port;
	struct net_bridge_port_group __rcu *next;
	struct hlist_node		mglist;
	struct rcu_head			rcu;
	struct timer_list		timer;
	struct br_ip			addr;
	unsigned char			state;
};

struct net_bridge_mdb_entry
{
	struct hlist_node		hlist[2];
	struct net_bridge		*br;
	struct net_bridge_port_group __rcu *ports;
	struct rcu_head			rcu;
	struct timer_list		timer;
	struct br_ip			addr;
	bool				mglist;
};

struct net_bridge_mdb_htable
{
	struct hlist_head		*mhash;
	struct rcu_head			rcu;
	struct net_bridge_mdb_htable	*old;
	u32				size;
	u32				max;
	u32				secret;
	u32				ver;
};

struct net_bridge_port
{
	struct net_bridge		*br;
	struct net_device		*dev;
	struct list_head		list;

	/* STP */
	u8				priority;
	u8				state;
	u16				port_no;
	unsigned char			topology_change_ack;
	unsigned char			config_pending;
	port_id				port_id;
	port_id				designated_port;
	bridge_id			designated_root;
	bridge_id			designated_bridge;
	u32				path_cost;
	u32				designated_cost;
	unsigned long			designated_age;

	struct timer_list		forward_delay_timer;
	struct timer_list		hold_timer;
	struct timer_list		message_age_timer;
	struct kobject			kobj;
	struct rcu_head			rcu;

	unsigned long 			flags;

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	struct bridge_mcast_own_query	ip4_own_query;
#if IS_ENABLED(CONFIG_IPV6)
	struct bridge_mcast_own_query	ip6_own_query;
#endif /* IS_ENABLED(CONFIG_IPV6) */
	unsigned char			multicast_router;
	struct timer_list		multicast_router_timer;
	struct hlist_head		mglist;
	struct hlist_node		rlist;
#endif

#ifdef CONFIG_SYSFS
	char				sysfs_name[IFNAMSIZ];
#endif

#ifdef CONFIG_NET_POLL_CONTROLLER
	struct netpoll			*np;
#endif
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	struct net_port_vlans __rcu	*vlan_info;
#endif
#ifdef CONFIG_RTK_GUEST_ZONE
	int	zone_type;	// 0: host zone, 1: guest zone, 2: gateway zone
#endif
};

#define br_auto_port(p) ((p)->flags & BR_AUTO_MASK)
#define br_promisc_port(p) ((p)->flags & BR_PROMISC)

#define br_port_exists(dev) (dev->priv_flags & IFF_BRIDGE_PORT)

static inline struct net_bridge_port *br_port_get_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler_data);
}

static inline struct net_bridge_port *br_port_get_rtnl(const struct net_device *dev)
{
	return br_port_exists(dev) ?
		rtnl_dereference(dev->rx_handler_data) : NULL;
}

struct net_bridge
{
	spinlock_t			lock;
	struct list_head		port_list;
	struct net_device		*dev;

	struct pcpu_sw_netstats		__percpu *stats;
	spinlock_t			hash_lock;
	struct hlist_head		hash[BR_HASH_SIZE];
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	struct rtable 			fake_rtable;
	bool				nf_call_iptables;
	bool				nf_call_ip6tables;
	bool				nf_call_arptables;
#endif
	u16				group_fwd_mask;
	u16				group_fwd_mask_required;

	/* STP */
	bridge_id			designated_root;
	bridge_id			bridge_id;
	u32				root_path_cost;
	unsigned long			max_age;
	unsigned long			hello_time;
	unsigned long			forward_delay;
	unsigned long			bridge_max_age;
	unsigned long			ageing_time;
	unsigned long			bridge_hello_time;
	unsigned long			bridge_forward_delay;

	u8				group_addr[ETH_ALEN];
	bool				group_addr_set;
	u16				root_port;

	enum {
		BR_NO_STP, 		/* no spanning tree */
		BR_KERNEL_STP,		/* old STP in kernel */
		BR_USER_STP,		/* new RSTP in userspace */
	} stp_enabled;

	unsigned char			topology_change;
	unsigned char			topology_change_detected;

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	unsigned char			multicast_router;

	u8				multicast_disabled:1;
	u8				multicast_querier:1;
	u8				multicast_query_use_ifaddr:1;

	u32				hash_elasticity;
	u32				hash_max;

	u32				multicast_last_member_count;
	u32				multicast_startup_query_count;

	unsigned long			multicast_last_member_interval;
	unsigned long			multicast_membership_interval;
	unsigned long			multicast_querier_interval;
	unsigned long			multicast_query_interval;
	unsigned long			multicast_query_response_interval;
	unsigned long			multicast_startup_query_interval;

	spinlock_t			multicast_lock;
	struct net_bridge_mdb_htable __rcu *mdb;
	struct hlist_head		router_list;

	struct timer_list		multicast_router_timer;
	struct bridge_mcast_other_query	ip4_other_query;
	struct bridge_mcast_own_query	ip4_own_query;
	struct bridge_mcast_querier	ip4_querier;
#if IS_ENABLED(CONFIG_IPV6)
	struct bridge_mcast_other_query	ip6_other_query;
	struct bridge_mcast_own_query	ip6_own_query;
	struct bridge_mcast_querier	ip6_querier;
#endif /* IS_ENABLED(CONFIG_IPV6) */
#endif

	struct timer_list		hello_timer;
	struct timer_list		tcn_timer;
	struct timer_list		topology_change_timer;
	struct timer_list		gc_timer;
	struct kobject			*ifobj;
	u32				auto_cnt;
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	u8				vlan_enabled;
	__be16				vlan_proto;
	u16				default_pvid;
	struct net_port_vlans __rcu	*vlan_info;
#endif

#ifdef CONFIG_RTK_GUEST_ZONE
	int	is_guest_isolated;	// isolate guest when 1
	int	is_zone_isolated;	// isolate host and guest zone when 1
	int	lock_client_num;
	unsigned char lock_client_list[MAX_LOCK_CLIENT][6];
	int gateway_mac_set;
	unsigned char gateway_mac[6];
#endif

#if defined (CONFIG_RTL_IGMP_SNOOPING)
	int				igmpProxy_pid;
	struct timer_list		mCastQuerytimer;
#endif /* CONFIG_RTL_IGMP_SNOOPING */

};
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
#if defined(CONFIG_RTL_IGMP_SNOOPING)

struct igmg_register_info
{
	unsigned int			valid;
	unsigned int			moduleIndex;
	unsigned int			swFwdPortMask;
	struct net_bridge		*br;
	unsigned int			mCastQueryTimerCnt;
};

#endif /* CONFIG_RTL_IGMP_SNOOPING */
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */

struct br_input_skb_cb {
	struct net_device *brdev;

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
	int igmp;
	int mrouters_only;
#endif

	u16 frag_max_size;
	bool proxyarp_replied;

#ifdef CONFIG_BRIDGE_VLAN_FILTERING
	bool vlan_filtered;
#endif
};

#define BR_INPUT_SKB_CB(__skb)	((struct br_input_skb_cb *)(__skb)->cb)

#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
# define BR_INPUT_SKB_CB_MROUTERS_ONLY(__skb)	(BR_INPUT_SKB_CB(__skb)->mrouters_only)
#else
# define BR_INPUT_SKB_CB_MROUTERS_ONLY(__skb)	(0)
#endif

#define br_printk(level, br, format, args...)	\
	printk(level "%s: " format, (br)->dev->name, ##args)

#define br_err(__br, format, args...)			\
	br_printk(KERN_ERR, __br, format, ##args)
#define br_warn(__br, format, args...)			\
	br_printk(KERN_WARNING, __br, format, ##args)
#define br_notice(__br, format, args...)		\
	br_printk(KERN_NOTICE, __br, format, ##args)
#define br_info(__br, format, args...)			\
	br_printk(KERN_INFO, __br, format, ##args)

#define br_debug(br, format, args...)			\
	pr_debug("%s: " format,  (br)->dev->name, ##args)

/* called under bridge lock */
static inline int br_is_root_bridge(const struct net_bridge *br)
{
	return !memcmp(&br->bridge_id, &br->designated_root, 8);
}

/* br_device.c */
void br_dev_setup(struct net_device *dev);
void br_dev_delete(struct net_device *dev, struct list_head *list);
netdev_tx_t br_dev_xmit(struct sk_buff *skb, struct net_device *dev);
#ifdef CONFIG_NET_POLL_CONTROLLER
static inline void br_netpoll_send_skb(const struct net_bridge_port *p,
				       struct sk_buff *skb)
{
	struct netpoll *np = p->np;

	if (np)
		netpoll_send_skb(np, skb);
}

int br_netpoll_enable(struct net_bridge_port *p);
void br_netpoll_disable(struct net_bridge_port *p);
#else
static inline void br_netpoll_send_skb(const struct net_bridge_port *p,
				       struct sk_buff *skb)
{
}

static inline int br_netpoll_enable(struct net_bridge_port *p)
{
	return 0;
}

static inline void br_netpoll_disable(struct net_bridge_port *p)
{
}
#endif

/* br_fdb.c */
int br_fdb_init(void);
void br_fdb_fini(void);
void br_fdb_flush(struct net_bridge *br);
void br_fdb_find_delete_local(struct net_bridge *br,
			      const struct net_bridge_port *p,
			      const unsigned char *addr, u16 vid);
void br_fdb_changeaddr(struct net_bridge_port *p, const unsigned char *newaddr);
void br_fdb_change_mac_address(struct net_bridge *br, const u8 *newaddr);
void br_fdb_cleanup(unsigned long arg);
void br_fdb_delete_by_port(struct net_bridge *br,
			   const struct net_bridge_port *p, int do_all);
struct net_bridge_fdb_entry *__br_fdb_get(struct net_bridge *br,
					  const unsigned char *addr, __u16 vid);
int br_fdb_test_addr(struct net_device *dev, unsigned char *addr);
#ifdef CONFIG_RTK_GUEST_ZONE
extern int br_fdb_fillbuf(struct net_bridge *br, void *buf,
			  unsigned long count, unsigned long off, int for_guest);
#else
int br_fdb_fillbuf(struct net_bridge *br, void *buf, unsigned long count,
		   unsigned long off);
#endif

#ifdef A4_STA
extern struct net_bridge_fdb_entry *fdb_find_for_driver(struct net_bridge *br, const unsigned char *addr);
#endif
int br_fdb_insert(struct net_bridge *br, struct net_bridge_port *source,
		  const unsigned char *addr, u16 vid);
void br_fdb_update(struct net_bridge *br, struct net_bridge_port *source,
		   const unsigned char *addr, u16 vid, bool added_by_user);

int br_fdb_delete(struct ndmsg *ndm, struct nlattr *tb[],
		  struct net_device *dev, const unsigned char *addr, u16 vid);
int br_fdb_add(struct ndmsg *nlh, struct nlattr *tb[], struct net_device *dev,
	       const unsigned char *addr, u16 vid, u16 nlh_flags);
int br_fdb_dump(struct sk_buff *skb, struct netlink_callback *cb,
		struct net_device *dev, struct net_device *fdev, int idx);
int br_fdb_sync_static(struct net_bridge *br, struct net_bridge_port *p);
void br_fdb_unsync_static(struct net_bridge *br, struct net_bridge_port *p);
int br_fdb_external_learn_add(struct net_bridge *br, struct net_bridge_port *p,
			      const unsigned char *addr, u16 vid);
int br_fdb_external_learn_del(struct net_bridge *br, struct net_bridge_port *p,
			      const unsigned char *addr, u16 vid);

/* br_forward.c */
void br_deliver(const struct net_bridge_port *to, struct sk_buff *skb);
int br_dev_queue_push_xmit(struct sock *sk, struct sk_buff *skb);
#if defined(CONFIG_RTL_IGMP_SNOOPING)
extern void br_forward(const struct net_bridge_port *to,
		struct sk_buff *skb);
#else
void br_forward(const struct net_bridge_port *to,
		struct sk_buff *skb, struct sk_buff *skb0);
#endif /* CONFIG_RTL_IGMP_SNOOPING */
int br_forward_finish(struct sock *sk, struct sk_buff *skb);
void br_flood_deliver(struct net_bridge *br, struct sk_buff *skb, bool unicast);
#if defined(CONFIG_RTL_IGMP_SNOOPING)
extern void br_flood_forward(struct net_bridge *br, struct sk_buff *skb);
#else
void br_flood_forward(struct net_bridge *br, struct sk_buff *skb,
		      struct sk_buff *skb2, bool unicast);
#endif /* CONFIG_RTL_IGMP_SNOOPING */

/* br_if.c */
void br_port_carrier_check(struct net_bridge_port *p);
int br_add_bridge(struct net *net, const char *name);
int br_del_bridge(struct net *net, const char *name);
int br_add_if(struct net_bridge *br, struct net_device *dev);
int br_del_if(struct net_bridge *br, struct net_device *dev);
int br_min_mtu(const struct net_bridge *br);
netdev_features_t br_features_recompute(struct net_bridge *br,
					netdev_features_t features);
void br_port_flags_change(struct net_bridge_port *port, unsigned long mask);
void br_manage_promisc(struct net_bridge *br);

/* br_input.c */
int br_handle_frame_finish(struct sock *sk, struct sk_buff *skb);
rx_handler_result_t br_handle_frame(struct sk_buff **pskb);

static inline bool br_rx_handler_check_rcu(const struct net_device *dev)
{
	return rcu_dereference(dev->rx_handler) == br_handle_frame;
}

static inline struct net_bridge_port *br_port_get_check_rcu(const struct net_device *dev)
{
	return br_rx_handler_check_rcu(dev) ? br_port_get_rcu(dev) : NULL;
}

/* br_ioctl.c */
int br_dev_ioctl(struct net_device *dev, struct ifreq *rq, int cmd);
int br_ioctl_deviceless_stub(struct net *net, unsigned int cmd,
			     void __user *arg);

/* br_multicast.c */
#ifdef CONFIG_BRIDGE_IGMP_SNOOPING
extern unsigned int br_mdb_rehash_seq;
int br_multicast_rcv(struct net_bridge *br, struct net_bridge_port *port,
		     struct sk_buff *skb, u16 vid);
struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
					struct sk_buff *skb, u16 vid);
void br_multicast_add_port(struct net_bridge_port *port);
void br_multicast_del_port(struct net_bridge_port *port);
void br_multicast_enable_port(struct net_bridge_port *port);
void br_multicast_disable_port(struct net_bridge_port *port);
void br_multicast_init(struct net_bridge *br);
void br_multicast_open(struct net_bridge *br);
void br_multicast_stop(struct net_bridge *br);
void br_multicast_deliver(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb);
void br_multicast_forward(struct net_bridge_mdb_entry *mdst,
			  struct sk_buff *skb, struct sk_buff *skb2);
int br_multicast_set_router(struct net_bridge *br, unsigned long val);
int br_multicast_set_port_router(struct net_bridge_port *p, unsigned long val);
int br_multicast_toggle(struct net_bridge *br, unsigned long val);
int br_multicast_set_querier(struct net_bridge *br, unsigned long val);
int br_multicast_set_hash_max(struct net_bridge *br, unsigned long val);
struct net_bridge_mdb_entry *
br_mdb_ip_get(struct net_bridge_mdb_htable *mdb, struct br_ip *dst);
struct net_bridge_mdb_entry *
br_multicast_new_group(struct net_bridge *br, struct net_bridge_port *port,
		       struct br_ip *group);
void br_multicast_free_pg(struct rcu_head *head);
struct net_bridge_port_group *
br_multicast_new_port_group(struct net_bridge_port *port, struct br_ip *group,
			    struct net_bridge_port_group __rcu *next,
			    unsigned char state);
void br_mdb_init(void);
void br_mdb_uninit(void);
void br_mdb_notify(struct net_device *dev, struct net_bridge_port *port,
		   struct br_ip *group, int type);

#define mlock_dereference(X, br) \
	rcu_dereference_protected(X, lockdep_is_held(&br->multicast_lock))

static inline bool br_multicast_is_router(struct net_bridge *br)
{
	return br->multicast_router == 2 ||
	       (br->multicast_router == 1 &&
		timer_pending(&br->multicast_router_timer));
}

static inline bool
__br_multicast_querier_exists(struct net_bridge *br,
			      struct bridge_mcast_other_query *querier)
{
	return time_is_before_jiffies(querier->delay_time) &&
	       (br->multicast_querier || timer_pending(&querier->timer));
}

static inline bool br_multicast_querier_exists(struct net_bridge *br,
					       struct ethhdr *eth)
{
	switch (eth->h_proto) {
	case (htons(ETH_P_IP)):
		return __br_multicast_querier_exists(br, &br->ip4_other_query);
#if IS_ENABLED(CONFIG_IPV6)
	case (htons(ETH_P_IPV6)):
		return __br_multicast_querier_exists(br, &br->ip6_other_query);
#endif
	default:
		return false;
	}
}
#else
static inline int br_multicast_rcv(struct net_bridge *br,
				   struct net_bridge_port *port,
				   struct sk_buff *skb,
				   u16 vid)
{
	return 0;
}

static inline struct net_bridge_mdb_entry *br_mdb_get(struct net_bridge *br,
						      struct sk_buff *skb, u16 vid)
{
	return NULL;
}

static inline void br_multicast_add_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_del_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_enable_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_disable_port(struct net_bridge_port *port)
{
}

static inline void br_multicast_init(struct net_bridge *br)
{
}

static inline void br_multicast_open(struct net_bridge *br)
{
}

static inline void br_multicast_stop(struct net_bridge *br)
{
}
#if defined (CONFIG_RTL_IGMP_SNOOPING)
void br_multicast_deliver(struct net_bridge *br,
			  unsigned int fwdPortMask,
			  struct sk_buff *skb,
			  int clone);

void br_multicast_forward(struct net_bridge *br,
			  unsigned int fwdPortMask,
			  struct sk_buff *skb,
			  int clone);
#else
static inline void br_multicast_deliver(struct net_bridge_mdb_entry *mdst,
					struct sk_buff *skb)
{
}

static inline void br_multicast_forward(struct net_bridge_mdb_entry *mdst,
					struct sk_buff *skb,
					struct sk_buff *skb2)
{
}
#endif /* CONFIG_RTL_IGMP_SNOOPING */
static inline bool br_multicast_is_router(struct net_bridge *br)
{
	return 0;
}
static inline bool br_multicast_querier_exists(struct net_bridge *br,
					       struct ethhdr *eth)
{
	return false;
}
static inline void br_mdb_init(void)
{
}
static inline void br_mdb_uninit(void)
{
}
#endif

/* br_vlan.c */
#ifdef CONFIG_BRIDGE_VLAN_FILTERING
bool br_allowed_ingress(struct net_bridge *br, struct net_port_vlans *v,
			struct sk_buff *skb, u16 *vid);
bool br_allowed_egress(struct net_bridge *br, const struct net_port_vlans *v,
		       const struct sk_buff *skb);
bool br_should_learn(struct net_bridge_port *p, struct sk_buff *skb, u16 *vid);
struct sk_buff *br_handle_vlan(struct net_bridge *br,
			       const struct net_port_vlans *v,
			       struct sk_buff *skb);
int br_vlan_add(struct net_bridge *br, u16 vid, u16 flags);
int br_vlan_delete(struct net_bridge *br, u16 vid);
void br_vlan_flush(struct net_bridge *br);
bool br_vlan_find(struct net_bridge *br, u16 vid);
void br_recalculate_fwd_mask(struct net_bridge *br);
int br_vlan_filter_toggle(struct net_bridge *br, unsigned long val);
int br_vlan_set_proto(struct net_bridge *br, unsigned long val);
int br_vlan_init(struct net_bridge *br);
int br_vlan_set_default_pvid(struct net_bridge *br, unsigned long val);
int nbp_vlan_add(struct net_bridge_port *port, u16 vid, u16 flags);
int nbp_vlan_delete(struct net_bridge_port *port, u16 vid);
void nbp_vlan_flush(struct net_bridge_port *port);
bool nbp_vlan_find(struct net_bridge_port *port, u16 vid);
int nbp_vlan_init(struct net_bridge_port *port);

static inline struct net_port_vlans *br_get_vlan_info(
						const struct net_bridge *br)
{
	return rcu_dereference_rtnl(br->vlan_info);
}

static inline struct net_port_vlans *nbp_get_vlan_info(
						const struct net_bridge_port *p)
{
	return rcu_dereference_rtnl(p->vlan_info);
}

/* Since bridge now depends on 8021Q module, but the time bridge sees the
 * skb, the vlan tag will always be present if the frame was tagged.
 */
static inline int br_vlan_get_tag(const struct sk_buff *skb, u16 *vid)
{
	int err = 0;

	if (skb_vlan_tag_present(skb))
		*vid = skb_vlan_tag_get(skb) & VLAN_VID_MASK;
	else {
		*vid = 0;
		err = -EINVAL;
	}

	return err;
}

static inline u16 br_get_pvid(const struct net_port_vlans *v)
{
	if (!v)
		return 0;

	smp_rmb();
	return v->pvid;
}

static inline int br_vlan_enabled(struct net_bridge *br)
{
	return br->vlan_enabled;
}
#else
static inline bool br_allowed_ingress(struct net_bridge *br,
				      struct net_port_vlans *v,
				      struct sk_buff *skb,
				      u16 *vid)
{
	return true;
}

static inline bool br_allowed_egress(struct net_bridge *br,
				     const struct net_port_vlans *v,
				     const struct sk_buff *skb)
{
	return true;
}

static inline bool br_should_learn(struct net_bridge_port *p,
				   struct sk_buff *skb, u16 *vid)
{
	return true;
}

static inline struct sk_buff *br_handle_vlan(struct net_bridge *br,
					     const struct net_port_vlans *v,
					     struct sk_buff *skb)
{
	return skb;
}

static inline int br_vlan_add(struct net_bridge *br, u16 vid, u16 flags)
{
	return -EOPNOTSUPP;
}

static inline int br_vlan_delete(struct net_bridge *br, u16 vid)
{
	return -EOPNOTSUPP;
}

static inline void br_vlan_flush(struct net_bridge *br)
{
}

static inline bool br_vlan_find(struct net_bridge *br, u16 vid)
{
	return false;
}

static inline void br_recalculate_fwd_mask(struct net_bridge *br)
{
}

static inline int br_vlan_init(struct net_bridge *br)
{
	return 0;
}

static inline int nbp_vlan_add(struct net_bridge_port *port, u16 vid, u16 flags)
{
	return -EOPNOTSUPP;
}

static inline int nbp_vlan_delete(struct net_bridge_port *port, u16 vid)
{
	return -EOPNOTSUPP;
}

static inline void nbp_vlan_flush(struct net_bridge_port *port)
{
}

static inline struct net_port_vlans *br_get_vlan_info(
						const struct net_bridge *br)
{
	return NULL;
}
static inline struct net_port_vlans *nbp_get_vlan_info(
						const struct net_bridge_port *p)
{
	return NULL;
}

static inline bool nbp_vlan_find(struct net_bridge_port *port, u16 vid)
{
	return false;
}

static inline int nbp_vlan_init(struct net_bridge_port *port)
{
	return 0;
}

static inline u16 br_vlan_get_tag(const struct sk_buff *skb, u16 *tag)
{
	return 0;
}
static inline u16 br_get_pvid(const struct net_port_vlans *v)
{
	return 0;
}

static inline int br_vlan_enabled(struct net_bridge *br)
{
	return 0;
}
#endif

struct nf_br_ops {
	int (*br_dev_xmit_hook)(struct sk_buff *skb);
};
extern const struct nf_br_ops __rcu *nf_br_ops;

/* br_netfilter.c */
#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
int br_nf_core_init(void);
void br_nf_core_fini(void);
void br_netfilter_rtable_init(struct net_bridge *);
#else
static inline int br_nf_core_init(void) { return 0; }
static inline void br_nf_core_fini(void) {}
#define br_netfilter_rtable_init(x)
#endif

/* br_stp.c */
void br_log_state(const struct net_bridge_port *p);
void br_set_state(struct net_bridge_port *p, unsigned int state);
struct net_bridge_port *br_get_port(struct net_bridge *br, u16 port_no);
void br_init_port(struct net_bridge_port *p);
void br_become_designated_port(struct net_bridge_port *p);

void __br_set_forward_delay(struct net_bridge *br, unsigned long t);
int br_set_forward_delay(struct net_bridge *br, unsigned long x);
int br_set_hello_time(struct net_bridge *br, unsigned long x);
int br_set_max_age(struct net_bridge *br, unsigned long x);


/* br_stp_if.c */
void br_stp_enable_bridge(struct net_bridge *br);
void br_stp_disable_bridge(struct net_bridge *br);
void br_stp_set_enabled(struct net_bridge *br, unsigned long val);
void br_stp_enable_port(struct net_bridge_port *p);
void br_stp_disable_port(struct net_bridge_port *p);
bool br_stp_recalculate_bridge_id(struct net_bridge *br);
void br_stp_change_bridge_id(struct net_bridge *br, const unsigned char *a);
void br_stp_set_bridge_priority(struct net_bridge *br, u16 newprio);
int br_stp_set_port_priority(struct net_bridge_port *p, unsigned long newprio);
int br_stp_set_path_cost(struct net_bridge_port *p, unsigned long path_cost);
ssize_t br_show_bridge_id(char *buf, const struct bridge_id *id);

/* br_stp_bpdu.c */
struct stp_proto;
void br_stp_rcv(const struct stp_proto *proto, struct sk_buff *skb,
		struct net_device *dev);

/* br_stp_timer.c */
void br_stp_timer_init(struct net_bridge *br);
void br_stp_port_timer_init(struct net_bridge_port *p);
unsigned long br_timer_value(const struct timer_list *timer);

/* br.c */
#if IS_ENABLED(CONFIG_ATM_LANE)
extern int (*br_fdb_test_addr_hook)(struct net_device *dev, unsigned char *addr);
#endif

/* br_netlink.c */
extern struct rtnl_link_ops br_link_ops;
int br_netlink_init(void);
void br_netlink_fini(void);
void br_ifinfo_notify(int event, struct net_bridge_port *port);
int br_setlink(struct net_device *dev, struct nlmsghdr *nlmsg, u16 flags);
int br_dellink(struct net_device *dev, struct nlmsghdr *nlmsg, u16 flags);
int br_getlink(struct sk_buff *skb, u32 pid, u32 seq, struct net_device *dev,
	       u32 filter_mask, int nlflags);

#ifdef CONFIG_SYSFS
/* br_sysfs_if.c */
extern const struct sysfs_ops brport_sysfs_ops;
int br_sysfs_addif(struct net_bridge_port *p);
int br_sysfs_renameif(struct net_bridge_port *p);

/* br_sysfs_br.c */
int br_sysfs_addbr(struct net_device *dev);
void br_sysfs_delbr(struct net_device *dev);

#else

static inline int br_sysfs_addif(struct net_bridge_port *p) { return 0; }
static inline int br_sysfs_renameif(struct net_bridge_port *p) { return 0; }
static inline int br_sysfs_addbr(struct net_device *dev) { return 0; }
static inline void br_sysfs_delbr(struct net_device *dev) { return; }
#endif /* CONFIG_SYSFS */

#if defined CONFIG_RTL_IGMP_PROXY_MULTIWAN
extern int rtl_smux_downstream_port_mapping_check(struct sk_buff *skb);
#endif

#endif
