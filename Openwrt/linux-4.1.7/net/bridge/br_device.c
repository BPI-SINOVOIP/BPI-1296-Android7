/*
 *	Device handling code
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
#include <linux/netpoll.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>
#include <linux/list.h>
#include <linux/netfilter_bridge.h>

#include <asm/uaccess.h>
#include "br_private.h"

#define COMMON_FEATURES (NETIF_F_SG | NETIF_F_FRAGLIST | NETIF_F_HIGHDMA | \
			 NETIF_F_GSO_MASK | NETIF_F_HW_CSUM)

const struct nf_br_ops __rcu *nf_br_ops __read_mostly;
EXPORT_SYMBOL_GPL(nf_br_ops);

static struct lock_class_key bridge_netdev_addr_lock_key;

#if defined(CONFIG_RTL_IGMP_SNOOPING)
/*2008-01-15,for porting igmp snooping to linux kernel 2.6*/
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/igmp.h>
#include <net/checksum.h>
#include <net/rtl/rtl865x_igmpsnooping_glue.h>
#include <net/rtl/rtl865x_igmpsnooping.h>
extern int igmpsnoopenabled;
#if defined(CONFIG_RTL_MLD_SNOOPING)
#include <linux/ipv6.h>
#include <linux/in6.h>
extern int mldSnoopEnabled;
#include <net/rtl/rtl_types.h>
#endif /* CONFIG_RTL_MLD_SNOOPING */
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#include <net/rtl/rtl865x_multicast.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>
#if defined(CONFIG_RTL_IGMP_PROXY_MULTIWAN)
#include <net/rtl/rtl_multi_wan.h>
#endif
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */
extern unsigned int br0SwFwdPortMask;
extern unsigned int brIgmpModuleIndex;
extern unsigned int nicIgmpModuleIndex;
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
#if defined(CONFIG_RTL_VLAN_8021Q) && defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
extern int linux_vlan_enable;
uint32 rtl865x_getVlanPortMask(uint32 vid);
unsigned int rtl865x_getPhyPortMapMaskbyVlan(struct net_bridge *br,struct sk_buff *skb);
#endif /* CONFIG_RTL_VLAN_8021Q && CONFIG_RTL_MULTICAST_PORT_MAPPING */
extern int rtl_get_brIgmpModueIndx(struct net_bridge *br);
extern unsigned int rtl_get_brSwFwdPortMask(struct net_bridge *br);
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#endif /* CONFIG_RTL_IGMP_SNOOPING */

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
extern uint32 rtl_hw_vlan_get_tagged_portmask(void);
#endif /* CONFIG_RTL_HW_VLAN_SUPPORT */

#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
extern unsigned int br1SwFwdPortMask;
extern unsigned int nicIgmpModuleIndex_2;
#endif

#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					unsigned int srcIpAddr, unsigned int destIpAddr, unsigned int mapPortMask);
#else /* CONFIG_RTL_MULTICAST_PORT_MAPPING */
int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					unsigned int srcIpAddr, unsigned int destIpAddr);
#endif /* CONFIG_RTL_MULTICAST_PORT_MAPPING */

#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
int rtl865x_ipv6MulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					struct in6_addr srcIpAddr,struct in6_addr destIpAddr, unsigned int mapPortMask);
#else /* CONFIG_RTL_MULTICAST_PORT_MAPPING */
int rtl865x_ipv6MulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					struct in6_addr srcIpAddr,struct in6_addr destIpAddr);
#endif /* CONFIG_RTL_MULTICAST_PORT_MAPPING */
#endif /* CONFIG_RTL_8198C || CONFIG_RTL_8197F */
#if defined(CONFIG_RTL_HW_MCAST_WIFI)
extern int hwwifiEnable;
#endif
#endif /* CONFIG_RTL_IGMP_SNOOPING */
#endif /* CONFIG_RTL_HARDWARE_MULTICAST */

#if defined(CONFIG_RTL_MLD_SNOOPING)
extern int re865x_getIpv6TransportProtocol(struct ipv6hdr* ipv6h);
#endif /* CONFIG_RTL_MLD_SNOOPING */

/* net device transmit always called with BH disabled */
netdev_tx_t br_dev_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	const unsigned char *dest = skb->data;
	struct net_bridge_fdb_entry *dst;
#ifndef CONFIG_RTL_IGMP_SNOOPING
	struct net_bridge_mdb_entry *mdst;
#endif /* CONFIG_RTL_IGMP_SNOOPING */
	struct pcpu_sw_netstats *brstats = this_cpu_ptr(br->stats);
	const struct nf_br_ops *nf_ops;
	u16 vid = 0;
#if defined(CONFIG_RTL_HARDWARE_MULTICAST) && defined(CONFIG_RTL_8198C)
#if 0 /*for compiler warning: unused variable 'i'*/
	int i = 0;
#endif
#endif
#if defined(CONFIG_RTL_IGMP_SNOOPING)
	struct iphdr *iph = NULL;
	unsigned char proto = 0;
	unsigned char reserved = 0;
#if defined(CONFIG_RTL_MLD_SNOOPING)
	struct ipv6hdr *ipv6h = NULL;
#endif
	struct rtl_multicastDataInfo multicastDataInfo;
	struct rtl_multicastFwdInfo multicastFwdInfo;
	int ret=FAILED;
#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
	unsigned int srcPort = skb->srcPort;
	unsigned int srcVlanId = skb->srcVlanId;
#if defined(CONFIG_RTL_IGMP_PROXY_MULTIWAN)
	struct smux_dev_info *dev_info;
#endif
#if defined(CONFIG_RTL_HW_MCAST_WIFI)
	rtl865x_tblDrv_mCast_t * existMulticastEntry;
#endif
#endif/*CONFIG_RTL_HARDWARE_MULTICAST*/
#endif/*CONFIG_RTL_IGMP_SNOOPING*/

	rcu_read_lock();
	nf_ops = rcu_dereference(nf_br_ops);
	if (nf_ops && nf_ops->br_dev_xmit_hook(skb)) {
		rcu_read_unlock();
		return NETDEV_TX_OK;
	}

	BR_INPUT_SKB_CB(skb)->brdev = dev;

	skb_reset_mac_header(skb);
	skb_pull(skb, ETH_HLEN);

	u64_stats_update_begin(&brstats->syncp);
	brstats->tx_packets++;
	/* Exclude ETH_HLEN from byte stats for consistency with Rx chain */
	brstats->tx_bytes += skb->len;
	u64_stats_update_end(&brstats->syncp);

	if (!br_allowed_ingress(br, br_get_vlan_info(br), skb, &vid))
		goto out;

#if defined(CONFIG_RTL_IGMP_SNOOPING)
	if (dest[0] & 1)
	{
		if (igmpsnoopenabled || mldSnoopEnabled)
		{
		#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
			unsigned int igmpModuleIndex = 0xFFFFFFFF;
			extern int rtl_get_brIgmpModueIndx(struct net_bridge *br);
			igmpModuleIndex = rtl_get_brIgmpModueIndx(br);
			unsigned int swFwdPortMask = 0xFFFFFFFF;
			swFwdPortMask = rtl_get_brSwFwdPortMask(br);
		#endif
		#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
			unsigned int mapPortMask = 0xFFFFFFFF;
		#endif
			if (MULTICAST_MAC(dest) && igmpsnoopenabled)
			{

				iph = (struct iphdr *)skb_network_header(skb);
				proto = iph->protocol;
			#if 0
				if(( iph->daddr&0xFFFFFF00)==0xE0000000)
				{
					reserved=1;
				}
			#endif


		#if defined(CONFIG_USB_UWIFI_HOST)
				if (iph->daddr == htonl(0xEFFFFFFA) || iph->daddr == htonl(0xE1010101))
		#else
				if (iph->daddr == htonl(0xEFFFFFFA) || iph->daddr == htonl(0xE00000FB))
		#endif
				{
					/*for microsoft upnp*/
					reserved=1;
				}

				if (((proto == IPPROTO_UDP) || (proto == IPPROTO_TCP) || (proto == Any_0_hop_protocl)) && (reserved == 0))
				{
					multicastDataInfo.ipVersion = 4;
					multicastDataInfo.sourceIp[0] = (uint32)(iph->saddr);
					multicastDataInfo.groupAddr[0] = (uint32)(iph->daddr);

					multicastDataInfo.sourceIp[0] = ntohl(multicastDataInfo.sourceIp[0]);
					multicastDataInfo.groupAddr[0] = ntohl(multicastDataInfo.groupAddr[0]);

				#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
					ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
				#else
					ret = rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
				#endif

				#if defined(CONFIG_RTL_HW_MCAST_WIFI)
					if(hwwifiEnable)
					{
						existMulticastEntry=rtl865x_findMCastEntry(multicastDataInfo.groupAddr[0], multicastDataInfo.sourceIp[0], (unsigned short)srcVlanId, (unsigned short)srcPort);
						if(existMulticastEntry!=NULL)
						{
							/*it's already in cache, only forward to wlan */
				#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
							multicastFwdInfo.fwdPortMask &= swFwdPortMask;
				#else
							multicastFwdInfo.fwdPortMask &= br0SwFwdPortMask;
				#endif
						}
					}
				#endif
					//printk("fwdPortMask = %x, [%s:%d]\n", multicastFwdInfo.fwdPortMask);
					br_multicast_deliver(br, multicastFwdInfo.fwdPortMask, skb, 0);

				#if defined(CONFIG_RTL_HW_MCAST_WIFI)
					if ((hwwifiEnable && ret == SUCCESS) ||
						(hwwifiEnable == 0 && ret == SUCCESS && multicastFwdInfo.cpuFlag == 0))
				#else
					if ((ret == SUCCESS) && (multicastFwdInfo.cpuFlag == 0))
				#endif
					{
					#if 0
						if(skb->from_dev)
						{
							printk("from dev flag:%x, dev flag:%x, vlan_member:%x, portmap:%d, [%s:%d]\n",
								skb->from_dev->priv_flags, skb->dev->priv_flags, skb->vlan_member,
								rtl_smux_downstream_port_mapping_check(skb), __FUNCTION__, __LINE__);
						}
						else
						{
							printk("[%s:%d]\n", __FUNCTION__, __LINE__);
						}
					#endif
					#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
						if ((srcVlanId != 0) && (srcPort != 0xFFFF))
						{
						#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
						#if defined(CONFIG_RTL_IGMP_PROXY_MULTIWAN)
							if(skb->from_dev)
							{
								dev_info = SMUX_DEV_INFO(skb->from_dev);
								mapPortMask = dev_info->member;
							}
							else
								mapPortMask = 0xFFFFFFFF;
						#elif defined(CONFIG_RTL_VLAN_8021Q)
							if(linux_vlan_enable)
								mapPortMask = rtl865x_getPhyPortMapMaskbyVlan(br, skb);
							else
								mapPortMask = 0xFFFFFFFF;
						#endif
							#if 0//defined(CONFIG_RTK_VLAN_SUPPORT)
							if(rtk_vlan_support_enable == 0)
							{
								rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0], mapPortMask);
							}
							#else
							rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0], mapPortMask);
							#endif

						#else /* CONFIG_RTL_MULTICAST_PORT_MAPPING */
							#if defined(CONFIG_RTK_VLAN_SUPPORT)
							if(rtk_vlan_support_enable == 0)
							{
								rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0]);
							}
							#else
							rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0]);
							#endif
						#endif /* CONFIG_RTL_MULTICAST_PORT_MAPPING */
						}
					#endif
					}

				}
				else
				{
					#if defined(CONFIG_RTD_1295_HWNAT)
					br_flood_deliver(br, skb, false);
					#else /* !CONFIG_RTD_1295_HWNAT */
					br_flood_deliver(br, skb);
					#endif /* CONFIG_RTD_1295_HWNAT */
				}


			}
#if defined(CONFIG_RTL_MLD_SNOOPING)
			else if (mldSnoopEnabled && IPV6_MULTICAST_MAC(dest))
			{
				ipv6h = (struct ipv6hdr *)skb_network_header(skb);
				proto = re865x_getIpv6TransportProtocol(ipv6h);
				if ((proto == IPPROTO_UDP) || (proto == IPPROTO_TCP) || (proto == Any_0_hop_protocl))
				{
					multicastDataInfo.ipVersion = 6;
					memcpy(&multicastDataInfo.sourceIp, &ipv6h->saddr, sizeof(struct in6_addr));
					memcpy(&multicastDataInfo.groupAddr, &ipv6h->daddr, sizeof(struct in6_addr));
					multicastDataInfo.sourceIp[0] = ntohl(multicastDataInfo.sourceIp[0]);
					multicastDataInfo.sourceIp[1] = ntohl(multicastDataInfo.sourceIp[1]);
					multicastDataInfo.sourceIp[2] = ntohl(multicastDataInfo.sourceIp[2]);
					multicastDataInfo.sourceIp[3] = ntohl(multicastDataInfo.sourceIp[3]);
					multicastDataInfo.groupAddr[0] = ntohl(multicastDataInfo.groupAddr[0]);
					multicastDataInfo.groupAddr[1] = ntohl(multicastDataInfo.groupAddr[1]);
					multicastDataInfo.groupAddr[2] = ntohl(multicastDataInfo.groupAddr[2]);
					multicastDataInfo.groupAddr[3] = ntohl(multicastDataInfo.groupAddr[3]);


				#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
					ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
				#else
					ret = rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
				#endif
					br_multicast_deliver(br, multicastFwdInfo.fwdPortMask, skb, 0);
					#if defined(CONFIG_RTL_HARDWARE_MULTICAST) && (defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F))
					if ((ret == SUCCESS) && (multicastFwdInfo.cpuFlag == 0))
					{
						struct in6_addr sip;
						struct in6_addr dip;
						memcpy(&sip,multicastDataInfo.sourceIp, sizeof(struct in6_addr));
						memcpy(&dip,multicastDataInfo.groupAddr, sizeof(struct in6_addr));
						if ((srcVlanId != 0) && (srcPort != 0xFFFF))
						{
					#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
						#if defined(CONFIG_RTL_IGMP_PROXY_MULTIWAN)
							if (skb->from_dev)
							{
								dev_info = SMUX_DEV_INFO(skb->from_dev);
								mapPortMask = dev_info->member;
							}
							else
								mapPortMask = 0xFFFFFFFF;
						#elif defined(CONFIG_RTL_VLAN_8021Q)
							if(linux_vlan_enable)
								mapPortMask = rtl865x_getPhyPortMapMaskbyVlan(br, skb);
							else
								mapPortMask = 0xFFFFFFFF;
						#endif
						rtl865x_ipv6MulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask, srcPort, srcVlanId, sip, dip, mapPortMask);
					#else
						#if defined(CONFIG_RTK_VLAN_SUPPORT)
							if (rtk_vlan_support_enable == 0)
							{
								rtl865x_ipv6MulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask, srcPort, srcVlanId, sip, dip);
							}
							#else
							rtl865x_ipv6MulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask, srcPort, srcVlanId, sip, dip);
						#endif
					#endif
						}
					}
					#endif
				}
				else
				{
					#if defined(CONFIG_RTD_1295_HWNAT)
					br_flood_deliver(br, skb, false);
					#else /* !CONFIG_RTD_1295_HWNAT */
					br_flood_deliver(br, skb);
					#endif /* CONFIG_RTD_1295_HWNAT */
				}
			}
#endif/*CONFIG_RTL_MLD_SNOOPING*/
			else
			{
				#if defined(CONFIG_RTD_1295_HWNAT)
				br_flood_deliver(br, skb, false);
				#else /* !CONFIG_RTD_1295_HWNAT */
				br_flood_deliver(br, skb);
				#endif /* CONFIG_RTD_1295_HWNAT */
			}

		}
		else
		{
			#if defined(CONFIG_RTD_1295_HWNAT)
			br_flood_deliver(br, skb, false);
			#else /* !CONFIG_RTD_1295_HWNAT */
			br_flood_deliver(br, skb);
			#endif /* CONFIG_RTD_1295_HWNAT */
		}
	}
	else if ((dst = __br_fdb_get(br, dest,vid)) != NULL)
		br_deliver(dst->dst, skb);
	else
		#if defined(CONFIG_RTD_1295_HWNAT)
		br_flood_deliver(br, skb, true);
		#else /* !CONFIG_RTD_1295_HWNAT */
		br_flood_deliver(br, skb);
		#endif /* CONFIG_RTD_1295_HWNAT */

#else/*CONFIG_RTL_IGMP_SNOOPING*/
	if (is_broadcast_ether_addr(dest))
		br_flood_deliver(br, skb, false);
	else if (is_multicast_ether_addr(dest)) {
		if (unlikely(netpoll_tx_running(dev))) {
			br_flood_deliver(br, skb, false);
			goto out;
		}
		if (br_multicast_rcv(br, NULL, skb, vid)) {
			kfree_skb(skb);
			goto out;
		}

		mdst = br_mdb_get(br, skb, vid);
		if ((mdst || BR_INPUT_SKB_CB_MROUTERS_ONLY(skb)) &&
		    br_multicast_querier_exists(br, eth_hdr(skb)))
			br_multicast_deliver(mdst, skb);
		else
			br_flood_deliver(br, skb, false);
	} else if ((dst = __br_fdb_get(br, dest, vid)) != NULL)
		br_deliver(dst->dst, skb);
	else
		br_flood_deliver(br, skb, true);

#endif/*CONFIG_RTL_IGMP_SNOOPING*/
out:
	rcu_read_unlock();
	return NETDEV_TX_OK;
}

static void br_set_lockdep_class(struct net_device *dev)
{
	lockdep_set_class(&dev->addr_list_lock, &bridge_netdev_addr_lock_key);
}

static int br_dev_init(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	int err;

	br->stats = netdev_alloc_pcpu_stats(struct pcpu_sw_netstats);
	if (!br->stats)
		return -ENOMEM;

	err = br_vlan_init(br);
	if (err)
		free_percpu(br->stats);
	br_set_lockdep_class(dev);

	return err;
}

static int br_dev_open(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	netdev_update_features(dev);
	netif_start_queue(dev);
	br_stp_enable_bridge(br);
	br_multicast_open(br);

	return 0;
}

static void br_dev_set_multicast_list(struct net_device *dev)
{
}

static void br_dev_change_rx_flags(struct net_device *dev, int change)
{
	if (change & IFF_PROMISC)
		br_manage_promisc(netdev_priv(dev));
}

static int br_dev_stop(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	br_stp_disable_bridge(br);
	br_multicast_stop(br);

	netif_stop_queue(dev);

	return 0;
}

static struct rtnl_link_stats64 *br_get_stats64(struct net_device *dev,
						struct rtnl_link_stats64 *stats)
{
	struct net_bridge *br = netdev_priv(dev);
	struct pcpu_sw_netstats tmp, sum = { 0 };
	unsigned int cpu;

	for_each_possible_cpu(cpu) {
		unsigned int start;
		const struct pcpu_sw_netstats *bstats
			= per_cpu_ptr(br->stats, cpu);
		do {
			start = u64_stats_fetch_begin_irq(&bstats->syncp);
			memcpy(&tmp, bstats, sizeof(tmp));
		} while (u64_stats_fetch_retry_irq(&bstats->syncp, start));
		sum.tx_bytes   += tmp.tx_bytes;
		sum.tx_packets += tmp.tx_packets;
		sum.rx_bytes   += tmp.rx_bytes;
		sum.rx_packets += tmp.rx_packets;
	}

	stats->tx_bytes   = sum.tx_bytes;
	stats->tx_packets = sum.tx_packets;
	stats->rx_bytes   = sum.rx_bytes;
	stats->rx_packets = sum.rx_packets;

	return stats;
}

static int br_change_mtu(struct net_device *dev, int new_mtu)
{
	struct net_bridge *br = netdev_priv(dev);
	if (new_mtu < 68 || new_mtu > br_min_mtu(br))
		return -EINVAL;

	dev->mtu = new_mtu;

#if IS_ENABLED(CONFIG_BRIDGE_NETFILTER)
	/* remember the MTU in the rtable for PMTU */
	dst_metric_set(&br->fake_rtable.dst, RTAX_MTU, new_mtu);
#endif

	return 0;
}

/* Allow setting mac address to any valid ethernet address. */
static int br_set_mac_address(struct net_device *dev, void *p)
{
	struct net_bridge *br = netdev_priv(dev);
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	spin_lock_bh(&br->lock);
	if (!ether_addr_equal(dev->dev_addr, addr->sa_data)) {
		/* Mac address will be changed in br_stp_change_bridge_id(). */
		br_stp_change_bridge_id(br, addr->sa_data);
	}
	spin_unlock_bh(&br->lock);

	return 0;
}

static void br_getinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strlcpy(info->driver, "bridge", sizeof(info->driver));
	strlcpy(info->version, BR_VERSION, sizeof(info->version));
	strlcpy(info->fw_version, "N/A", sizeof(info->fw_version));
	strlcpy(info->bus_info, "N/A", sizeof(info->bus_info));
}

static netdev_features_t br_fix_features(struct net_device *dev,
	netdev_features_t features)
{
	struct net_bridge *br = netdev_priv(dev);

	return br_features_recompute(br, features);
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void br_poll_controller(struct net_device *br_dev)
{
}

static void br_netpoll_cleanup(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *p;

	list_for_each_entry(p, &br->port_list, list)
		br_netpoll_disable(p);
}

static int __br_netpoll_enable(struct net_bridge_port *p)
{
	struct netpoll *np;
	int err;

	np = kzalloc(sizeof(*p->np), GFP_KERNEL);
	if (!np)
		return -ENOMEM;

	err = __netpoll_setup(np, p->dev);
	if (err) {
		kfree(np);
		return err;
	}

	p->np = np;
	return err;
}

int br_netpoll_enable(struct net_bridge_port *p)
{
	if (!p->br->dev->npinfo)
		return 0;

	return __br_netpoll_enable(p);
}

static int br_netpoll_setup(struct net_device *dev, struct netpoll_info *ni)
{
	struct net_bridge *br = netdev_priv(dev);
	struct net_bridge_port *p;
	int err = 0;

	list_for_each_entry(p, &br->port_list, list) {
		if (!p->dev)
			continue;
		err = __br_netpoll_enable(p);
		if (err)
			goto fail;
	}

out:
	return err;

fail:
	br_netpoll_cleanup(dev);
	goto out;
}

void br_netpoll_disable(struct net_bridge_port *p)
{
	struct netpoll *np = p->np;

	if (!np)
		return;

	p->np = NULL;

	__netpoll_free_async(np);
}

#endif

static int br_add_slave(struct net_device *dev, struct net_device *slave_dev)

{
	struct net_bridge *br = netdev_priv(dev);

	return br_add_if(br, slave_dev);
}

static int br_del_slave(struct net_device *dev, struct net_device *slave_dev)
{
	struct net_bridge *br = netdev_priv(dev);

	return br_del_if(br, slave_dev);
}

static const struct ethtool_ops br_ethtool_ops = {
	.get_drvinfo    = br_getinfo,
	.get_link	= ethtool_op_get_link,
};

static const struct net_device_ops br_netdev_ops = {
	.ndo_open		 = br_dev_open,
	.ndo_stop		 = br_dev_stop,
	.ndo_init		 = br_dev_init,
	.ndo_start_xmit		 = br_dev_xmit,
	.ndo_get_stats64	 = br_get_stats64,
	.ndo_set_mac_address	 = br_set_mac_address,
	.ndo_set_rx_mode	 = br_dev_set_multicast_list,
	.ndo_change_rx_flags	 = br_dev_change_rx_flags,
	.ndo_change_mtu		 = br_change_mtu,
	.ndo_do_ioctl		 = br_dev_ioctl,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_netpoll_setup	 = br_netpoll_setup,
	.ndo_netpoll_cleanup	 = br_netpoll_cleanup,
	.ndo_poll_controller	 = br_poll_controller,
#endif
	.ndo_add_slave		 = br_add_slave,
	.ndo_del_slave		 = br_del_slave,
	.ndo_fix_features        = br_fix_features,
	.ndo_fdb_add		 = br_fdb_add,
	.ndo_fdb_del		 = br_fdb_delete,
	.ndo_fdb_dump		 = br_fdb_dump,
	.ndo_bridge_getlink	 = br_getlink,
	.ndo_bridge_setlink	 = br_setlink,
	.ndo_bridge_dellink	 = br_dellink,
};

static void br_dev_free(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	free_percpu(br->stats);
	free_netdev(dev);
}

static struct device_type br_type = {
	.name	= "bridge",
};

void br_dev_setup(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	eth_hw_addr_random(dev);
	ether_setup(dev);

	dev->netdev_ops = &br_netdev_ops;
	dev->destructor = br_dev_free;
	dev->ethtool_ops = &br_ethtool_ops;
	SET_NETDEV_DEVTYPE(dev, &br_type);
	dev->tx_queue_len = 0;
	dev->priv_flags = IFF_EBRIDGE;

#if defined(CONFIG_RTL_USB_IP_HOST_SPEEDUP)
	dev->features = COMMON_FEATURES | NETIF_F_LLTX | NETIF_F_NETNS_LOCAL |
			NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX |
			NETIF_F_GSO | NETIF_F_GRO;

#else
	dev->features = COMMON_FEATURES | NETIF_F_LLTX | NETIF_F_NETNS_LOCAL |
			NETIF_F_HW_VLAN_CTAG_TX | NETIF_F_HW_VLAN_STAG_TX;
#endif
	dev->hw_features = COMMON_FEATURES | NETIF_F_HW_VLAN_CTAG_TX |
			   NETIF_F_HW_VLAN_STAG_TX;
	dev->vlan_features = COMMON_FEATURES;

	br->dev = dev;
	spin_lock_init(&br->lock);
	INIT_LIST_HEAD(&br->port_list);
	spin_lock_init(&br->hash_lock);

	br->bridge_id.prio[0] = 0x80;
	br->bridge_id.prio[1] = 0x00;

	ether_addr_copy(br->group_addr, eth_reserved_addr_base);

	br->stp_enabled = BR_NO_STP;
	br->group_fwd_mask = BR_GROUPFWD_DEFAULT;
	br->group_fwd_mask_required = BR_GROUPFWD_DEFAULT;

	br->designated_root = br->bridge_id;
	br->bridge_max_age = br->max_age = 20 * HZ;
	br->bridge_hello_time = br->hello_time = 2 * HZ;
#if defined(CONFIG_RTL_819X)
	br->bridge_forward_delay = br->forward_delay = 10 * HZ;
#else
	br->bridge_forward_delay = br->forward_delay = 15 * HZ;
#endif
	br->ageing_time = 300 * HZ;

	br_netfilter_rtable_init(br);
#if defined(CONFIG_RTL_IGMP_SNOOPING)
	br->igmpProxy_pid = 0;
#endif
	br_stp_timer_init(br);
	br_multicast_init(br);
}

#if defined(CONFIG_RTL_HARDWARE_MULTICAST)
#if defined(CONFIG_RTL_IGMP_SNOOPING)
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT) || defined(CONFIG_RTL_HW_MCAST_WIFI)
extern int rtl_get_brIgmpModuleIndexbyId(int idx,char *name);
#endif
#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING) && defined(CONFIG_RTL_VLAN_8021Q)
extern uint16 rtk_vlan_wan_vid;
extern int MCastVlanLeakyInfo[MAX_VLAN_NUM];
extern uint32 rtk_get_vlan_portmask(uint16 vid);
unsigned int rtl865x_getPhyPortMapMaskbyVlan(struct net_bridge *br,struct sk_buff *skb)
{
	unsigned int phyPortMapMask = 0;
	struct net_bridge_port *p, *n;
	struct dev_priv *devPriv = NULL;
	int j, k;

	int vid = 0;
	if (*(uint16*)skb->linux_vlan_src_tag == __constant_htons(ETH_P_8021Q))
	{
		vid = ntohs(*(short *)(skb->linux_vlan_src_tag + 2)) & 0x0fff;

		#if defined(CONFIG_RTL_8021Q_VLAN_SUPPORT_SRC_TAG)
		//nat vlan is not set, only bridge vlan set, acctually, this case is not support.
		if (vid == RTL_WANVLANID)
			vid = RTL_LANVLANID;
		#endif
	}


	if (br == NULL)
	{
		return 0;
	}

	list_for_each_entry_safe(p, n, &br->port_list, list)
	{

		devPriv = (struct dev_priv *)netdev_priv(p->dev);
		if (devPriv)
		{
			if (strncmp(p->dev->name, RTL_WLAN_NAME, 4) != 0)
			{
				if(p->dev->vlan_id == vid)
				{
					phyPortMapMask |= rtk_get_vlan_portmask(vid);
				}
			}

			//panic_printk("[%s:%d]%s,%x,\n",__FUNCTION__,__LINE__,p->dev->name,phyPortMapMask);

		}
	}


	for (j = 0; j < MAX_VLAN_NUM; j++)
	{
		if (vid == MCastVlanLeakyInfo[j])
			break;
	}

	if (j < MAX_VLAN_NUM)
	{
		//vid belong to multicast vlan leaky group
		for (k = 0; k < MAX_VLAN_NUM; k++)
		{
			if (MCastVlanLeakyInfo[k] != 0)
			{
				phyPortMapMask |= rtk_get_vlan_portmask(MCastVlanLeakyInfo[k]);
			}
		}
	}

	//remove cpu port
	if (phyPortMapMask != 0)
		phyPortMapMask &= ((1 << 6) - 1);
	else
		phyPortMapMask = 0xFFFFFFFF;
	return phyPortMapMask;
}
#endif /* CONFIG_RTL_MULTICAST_PORT_MAPPING && CONFIG_RTL_VLAN_8021Q */
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
unsigned int rtl865x_getPhyFwdPortMask(struct net_bridge *br,unsigned int brFwdPortMask)
{
	unsigned int phyPortMask = 0;
	unsigned int port_bitmask = 0;
	struct net_bridge_port *p, *n;
	struct dev_priv *devPriv = NULL;
	if (br == NULL)
	{
		return 0;
	}
	list_for_each_entry_safe(p, n, &br->port_list, list)
	{
		port_bitmask = 1 << p->port_no;
		if (port_bitmask&brFwdPortMask)
		{
			#ifdef CONFIG_RTL_VLAN_8021Q
			if(strncmp(p->dev->name, "eth", 3) == 0 &&
				p->dev->name[4] == '.')
			{
				uint8 devName[16];
				struct net_device *dev = NULL;

				memcpy(devName, p->dev->name, 4);
				devName[4] = '\0';
				dev = dev_get_by_name(&init_net, devName);
				if (dev)
					devPriv = (struct dev_priv *)netdev_priv(dev);
			}
			else
				devPriv = (struct dev_priv *)netdev_priv(p->dev);
			#else
			devPriv = (struct dev_priv *)netdev_priv(p->dev);
			#endif
			if (devPriv){

			#if defined(CONFIG_RTL_IGMP_PROXY_MULTIWAN)
				if(strncmp(p->dev->name, ALIASNAME_ETH_WAN, 5) == 0)
					phyPortMask |= RTL_WANPORT_MASK;
				else
			#endif
				if (strncmp(p->dev->name, RTL_WLAN_NAME, 4) != 0)
					phyPortMask |= devPriv->portmask;
				//printk("[%s:%d]%s,%x,\n",__FUNCTION__,__LINE__,p->dev->name,devPriv->portmask);
			}
		}
	}
	return phyPortMask;
}
#endif
#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					unsigned int srcIpAddr, unsigned int destIpAddr, unsigned int mapPortMask)
#else
int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					unsigned int srcIpAddr, unsigned int destIpAddr)
#endif
{
	int ret;
	unsigned int tagged_portmask = 0;
	struct rtl_multicastDataInfo multicastDataInfo = {0};
	struct rtl_multicastFwdInfo  multicastFwdInfo = {0};

	rtl865x_tblDrv_mCast_t *existMulticastEntry;
	rtl865x_mcast_fwd_descriptor_t fwdDescriptor;

#if defined(CONFIG_RTL_HW_MCAST_WIFI) && !defined(CONFIG_RTL_MULTI_LAN_DEV)
	rtl865x_mcast_fwd_descriptor_t fwdDescriptor2;
#endif

#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	unsigned int igmpModuleIndex = 0xFFFFFFFF;
	unsigned int swFwdPortMask = 0xFFFFFFFF;
	unsigned int igmpModuleIndex_tmp = 0xFFFFFFFF;
	unsigned int swFwdPortMask_tmp = 0xFFFFFFFF;
	int i = 0;
	unsigned char brName[16] = {0};
	int cpuFlag = 0;
	int joinBrNum = 0;
#endif

	#if 0
	printk("%s:%d,srcPort is %d,srcVlanId is %d,srcIpAddr is 0x%x,destIpAddr is 0x%x\n",__FUNCTION__,__LINE__,srcPort,srcVlanId,srcIpAddr,destIpAddr);
	#endif

#if defined(CONFIG_RTL_L2_HW_MULTICAST_SUPPORT)
	return -1;
#endif

#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	if (br == NULL)
		return -1;

	igmpModuleIndex = rtl_get_brIgmpModueIndx(br);
	swFwdPortMask = rtl_get_brSwFwdPortMask(br);

	if (igmpModuleIndex >= RTL_IMGP_MAX_BRMODULE)
		return -1;

#if defined(CONFIG_RTL_HW_MCAST_WIFI)
	if (hwwifiEnable == 0)
	{
		multicastDataInfo.ipVersion = 4;
		multicastDataInfo.sourceIp[0] = srcIpAddr;
		multicastDataInfo.groupAddr[0] = destIpAddr;

		for (i = 0;i < RTL_IMGP_MAX_BRMODULE; i++)
		{
			igmpModuleIndex_tmp = rtl_get_brIgmpModuleIndexbyId(i, brName);
			if (igmpModuleIndex_tmp == 0xFFFFFFFF)
				continue;

			ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex_tmp, &multicastDataInfo, &multicastFwdInfo);
			if (ret != 0)
			{
				//not normal known mc data pkts
				continue;
			}

			joinBrNum++;
			cpuFlag |= multicastFwdInfo.cpuFlag;
		}
		if (cpuFlag == 1 || joinBrNum == 0)
			return -1;

	}

#else
	multicastDataInfo.ipVersion = 4;
	multicastDataInfo.sourceIp[0] = srcIpAddr;
	multicastDataInfo.groupAddr[0] = destIpAddr;

	for (i = 0;i < RTL_IMGP_MAX_BRMODULE; i++)
	{
		igmpModuleIndex_tmp = rtl_get_brIgmpModuleIndexbyId(i, brName);
		if (igmpModuleIndex_tmp == 0xFFFFFFFF)
			continue;

		ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex_tmp, &multicastDataInfo, &multicastFwdInfo);
		if (ret != 0)
		{
			//not normal known mc data pkts
			continue;
		}

		joinBrNum++;
		cpuFlag |= multicastFwdInfo.cpuFlag;
	}
	if (cpuFlag == 1 || joinBrNum == 0)
		return -1;

#endif

#else /* CONFIG_RT_MULTIPLE_BR_SUPPORT */

#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT

	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) != 0 && strcmp(br->dev->name,RTL_PS_BR1_DEV_NAME) != 0)
	{
		return -1;
	}
	if (strcmp(br->dev->name, RTL_PS_BR0_DEV_NAME) == 0 && (brFwdPortMask & br0SwFwdPortMask))
	{
		return -1;
	}

	if (strcmp(br->dev->name, RTL_PS_BR1_DEV_NAME) == 0 && (brFwdPortMask & br1SwFwdPortMask))
	{
		return -1;
	}
#else
	if (strcmp(br->dev->name, RTL_PS_BR0_DEV_NAME) != 0)
	{
		//if(net_ratelimit())printk("[%s:%d]wrong name...\n",__FUNCTION__,__LINE__);
		return -1;
	}
#ifdef CONFIG_RTL_HW_MCAST_WIFI
	if (hwwifiEnable == 0)
	{
		if(brFwdPortMask & br0SwFwdPortMask)
			return -1;
	}
#else
	if (brFwdPortMask & br0SwFwdPortMask)
	{
		//if(net_ratelimit())printk("[%s:%d]brFwdPortMask&br0SwFwdPortMask!=0\n",__FUNCTION__,__LINE__);
		return -1;
	}
#endif
#endif
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
	//printk("%s:%d,destIpAddr is 0x%x, srcIpAddr is 0x%x, srcVlanId is %d, srcPort is %d\n",__FUNCTION__,__LINE__,destIpAddr, srcIpAddr, srcVlanId, srcPort);
	existMulticastEntry = rtl865x_findMCastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort);
	if (existMulticastEntry != NULL)
	{
		/*it's already in cache */
		//if(net_ratelimit())printk("[%s:%d]already in cache\n",__FUNCTION__,__LINE__);
		return 0;

	}

#if !defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	if (brFwdPortMask == 0)
	{
		rtl865x_blockMulticastFlow(srcVlanId, srcPort, srcIpAddr, destIpAddr);
		return 0;
	}
#endif

	multicastDataInfo.ipVersion = 4;
	multicastDataInfo.sourceIp[0] = srcIpAddr;
	multicastDataInfo.groupAddr[0] = destIpAddr;

/*add hardware multicast entry*/
#if defined(CONFIG_RTL_MULTI_LAN_DEV)
	memset(&fwdDescriptor, 0, sizeof(rtl865x_mcast_fwd_descriptor_t));
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
//#if defined(CONFIG_RTL_ISP_MULTIPLE_BR_SUPPORT)
	strcpy(fwdDescriptor.netifName, br->dev->name);
	ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
#else
	strcpy(fwdDescriptor.netifName,RTL_BR_NAME);
	ret = rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
#endif

	if (ret != 0)
		return -1;

	fwdDescriptor.fwdPortMask = rtl865x_getPhyFwdPortMask(br, brFwdPortMask) & (~(1 << srcPort));

	if(fwdDescriptor.fwdPortMask == 0)
		return -1;

	#if defined(CONFIG_RTL_HW_MCAST_WIFI)
	if (hwwifiEnable)
	{
		#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
		if(brFwdPortMask & swFwdPortMask)
		#else
		if(brFwdPortMask & br0SwFwdPortMask)
		#endif
		{
			fwdDescriptor.fwdPortMask |= BIT(6);
			fwdDescriptor.toCpu = 1;
		}
	}
	#endif

#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
	fwdDescriptor.fwdPortMask &= (mapPortMask | BIT(6));
#endif

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
	if (rtl_hw_vlan_ignore_tagged_mc == 1)
		tagged_portmask = rtl_hw_vlan_get_tagged_portmask();
#endif

	if (fwdDescriptor.fwdPortMask != 0 &&(fwdDescriptor.fwdPortMask & tagged_portmask) == 0)
	{
		#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
		ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
							&fwdDescriptor, 1, 0, 0, 0, mapPortMask);
		#else
		ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
							&fwdDescriptor, 1, 0, 0, 0);
		#endif
	}


#else /* CONFIG_RTL_MULTI_LAN_DEV */
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if (strcmp(br->dev->name, RTL_PS_BR0_DEV_NAME) == 0)
	{
		memset(&fwdDescriptor, 0, sizeof(rtl865x_mcast_fwd_descriptor_t));
		strcpy(fwdDescriptor.netifName, "eth*");
		fwdDescriptor.fwdPortMask = 0xFFFFFFFF;
		ret = rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
	}
	else if (strcmp(br->dev->name, RTL_PS_BR1_DEV_NAME) == 0)
	{
		memset(&fwdDescriptor, 0, sizeof(rtl865x_mcast_fwd_descriptor_t));
		strcpy(fwdDescriptor.netifName, "eth2");
		fwdDescriptor.fwdPortMask = 0xFFFFFFFF;
		ret = rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex_2, &multicastDataInfo, &multicastFwdInfo);
	}
#else /* CONFIG_RTK_VLAN_WAN_TAG_SUPPORT */
	memset(&fwdDescriptor, 0, sizeof(rtl865x_mcast_fwd_descriptor_t));
	strcpy(fwdDescriptor.netifName, "eth*");
	fwdDescriptor.fwdPortMask = 0xFFFFFFFF;

	ret = rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
#endif /* CONFIG_RTK_VLAN_WAN_TAG_SUPPORT */
	if (ret != 0)
	{
		//if(net_ratelimit())printk("[%s:%d]get nic fwd info failed.\n",__FUNCTION__,__LINE__);
		return -1;
	}
	else
	{
		if (multicastFwdInfo.cpuFlag)
		{
			fwdDescriptor.toCpu = 1;
		}
		fwdDescriptor.fwdPortMask = multicastFwdInfo.fwdPortMask & (~(1 << srcPort));
	}

#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
	fwdDescriptor.fwdPortMask &= mapPortMask;
#endif

	if (fwdDescriptor.fwdPortMask == 0)
		return -1;

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
	if (rtl_hw_vlan_ignore_tagged_mc == 1)
		tagged_portmask = rtl_hw_vlan_get_tagged_portmask();
#endif

	if ((fwdDescriptor.fwdPortMask & tagged_portmask) == 0)
	{
	#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
		ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
							&fwdDescriptor, 1, 0, 0, 0, mapPortMask);
	#else
		ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
							&fwdDescriptor, 1, 0, 0, 0);
	#endif
	}
#if defined(CONFIG_RTL_HW_MCAST_WIFI)
	if (hwwifiEnable)
	{
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
		for (i = 0;i < RTL_IMGP_MAX_BRMODULE; i++)
		{
			igmpModuleIndex = rtl_get_brIgmpModuleIndexbyId(i, brName);
			if(igmpModuleIndex == 0xFFFFFFFF)
				continue;

			memset(&fwdDescriptor2, 0, sizeof(rtl865x_mcast_fwd_descriptor_t));
			memset(&multicastFwdInfo, 0, sizeof(struct rtl_multicastFwdInfo));
			strcpy(fwdDescriptor2.netifName, brName);
			ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
			if(ret != 0)
			{
				//not normal known mc data pkts
				continue;
			}
			else
			{
				if(multicastFwdInfo.cpuFlag)
				{
					fwdDescriptor2.toCpu = 1;
				}
			}

			if(fwdDescriptor2.toCpu)
				fwdDescriptor2.fwdPortMask |= BIT(6);
			if((fwdDescriptor2.fwdPortMask & tagged_portmask) == 0)
			{
				//printk("%s:%d,br:%s,brFwdPortMask:%x,asicfwdPortmask:%x,srcPort is %d,srcVlanId is %d,srcIpAddr is 0x%x,destIpAddr is 0x%x\n",__FUNCTION__,__LINE__,brName,multicastFwdInfo.fwdPortMask,fwdDescriptor2.fwdPortMask,srcPort,srcVlanId,srcIpAddr,destIpAddr);
			#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
				ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
									&fwdDescriptor2, 0, 0, 0, 0, mapPortMask);
			#else
				ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
									&fwdDescriptor2, 0, 0, 0, 0);
			#endif

			}

		}

#else /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
		memset(&fwdDescriptor2, 0, sizeof(rtl865x_mcast_fwd_descriptor_t ));

		strcpy(fwdDescriptor2.netifName,br->dev->name);

		if (brFwdPortMask & br0SwFwdPortMask)

		{
			fwdDescriptor2.toCpu = 1;
		}
		if (fwdDescriptor2.toCpu)
			fwdDescriptor2.fwdPortMask |= BIT(6);

		if ((fwdDescriptor2.fwdPortMask & tagged_portmask) == 0)
		{

			//printk("%s:%d,br:%s,brFwdPortMask:%x,swFwdPortMask:%x,asicfwdPortmask:%x,srcPort is %d,srcVlanId is %d,srcIpAddr is 0x%x,destIpAddr is 0x%x\n",__FUNCTION__,__LINE__,br->dev->name,brFwdPortMask,swFwdPortMask,fwdDescriptor2.fwdPortMask,srcPort,srcVlanId,srcIpAddr,destIpAddr);
			ret = rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort,
									&fwdDescriptor2, 0, 0, 0, 0);
		}
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
	}
#endif /* CONFIG_RTL_HW_MCAST_WIFI */
#endif /* CONFIG_RTL_MULTI_LAN_DEV */

	return 0;
}

#if defined(CONFIG_RTL_8198C) || defined(CONFIG_RTL_8197F)
#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
int rtl865x_ipv6MulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					struct in6_addr srcIpAddr,struct in6_addr destIpAddr, unsigned int mapPortMask)

#else
int rtl865x_ipv6MulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
					unsigned int srcPort,unsigned int srcVlanId,
					struct in6_addr srcIpAddr,struct in6_addr destIpAddr)
#endif
{
#if 0
	if (net_ratelimit())
		printk("[%s:%d]\n",__FUNCTION__,__LINE__);
#endif
	int ret;
	unsigned int tagged_portmask = 0;
	struct rtl_multicastDataInfo multicastDataInfo = {0};
	struct rtl_multicastFwdInfo  multicastFwdInfo = {0};
	inv6_addr_t sip,dip;

	rtl8198c_tblDrv_mCastv6_t *existMulticastEntry;
	rtl8198c_mcast_fwd_descriptor6_t fwdDescriptor;

	#if 0
	printk("%s:%d,srcPort is %d,srcVlanId is %d,srcIpAddr is 0x%x,destIpAddr is 0x%x\n",__FUNCTION__,__LINE__,srcPort,srcVlanId,srcIpAddr,destIpAddr);
	#endif


#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	int i = 0, joinBrNum = 0, cpuFlag = 0;
	unsigned int igmpModuleIndex_tmp = 0xFFFFFFFF;
	unsigned char brName[16] = {0};
#endif

	multicastDataInfo.ipVersion = 6;
	memcpy(multicastDataInfo.sourceIp, &srcIpAddr, sizeof(struct in6_addr));
	memcpy(multicastDataInfo.groupAddr, &destIpAddr, sizeof(struct in6_addr));


#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	for (i = 0; i < RTL_IMGP_MAX_BRMODULE; i++)
	{
		igmpModuleIndex_tmp = rtl_get_brIgmpModuleIndexbyId(i,brName);
		if (igmpModuleIndex_tmp == 0xFFFFFFFF)
			continue;

		ret = rtl_getMulticastDataFwdInfo(igmpModuleIndex_tmp, &multicastDataInfo, &multicastFwdInfo);
		if (ret != 0)
		{
			//not normal known mc data pkts
			continue;
		}

		joinBrNum++;
		cpuFlag |= multicastFwdInfo.cpuFlag;
	}
	if (cpuFlag == 1 || joinBrNum == 0)
		return -1;

#else /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT

	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) !=0 &&strcmp(br->dev->name,RTL_PS_BR1_DEV_NAME) != 0)
	{
		return -1;
	}
	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) ==0 && (brFwdPortMask & br0SwFwdPortMask))
	{
		return -1;
	}

	if (strcmp(br->dev->name,RTL_PS_BR1_DEV_NAME) ==0 && (brFwdPortMask & br1SwFwdPortMask))
	{
		return -1;
	}
#else
	if (strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME) != 0)
	{
		return -1;
	}

	if (brFwdPortMask & br0SwFwdPortMask)
	{
		return -1;
	}
#endif
#endif /* CONFIG_RT_MULTIPLE_BR_SUPPORT */
	//printk("%s:%d,destIpAddr is 0x%x, srcIpAddr is 0x%x, srcVlanId is %d, srcPort is %d\n",__FUNCTION__,__LINE__,destIpAddr, srcIpAddr, srcVlanId, srcPort);
	memcpy(&sip,&srcIpAddr, sizeof(struct in6_addr));
	memcpy(&dip,&destIpAddr, sizeof(struct in6_addr));
	#if 0
	for (i = 0; i < 4; i++)
	{
		sip.v6_addr32[i] = srcIpAddr.s6_addr32[i];
		dip.v6_addr32[i] = destIpAddr.s6_addr32[i];
	}
	#endif
	existMulticastEntry = rtl8198C_findMCastv6Entry(dip,sip, (unsigned short)srcVlanId, (unsigned short)srcPort);
	if (existMulticastEntry != NULL)
	{
		/*it's already in cache */
#if 0
		if (net_ratelimit()) printk("[%s:%d]already in cache\n",__FUNCTION__,__LINE__);
#endif
		return 0;

	}

#if !defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	if (brFwdPortMask == 0)
	{
		rtl8198C_blockMulticastv6Flow(srcVlanId, srcPort, sip,dip);
		return 0;
	}
#endif


	#if 0
	for (i = 0; i < 4; i++)
	{
		multicastDataInfo.sourceIp[i] = srcIpAddr.in6_u.u6_addr32[i];
		multicastDataInfo.groupAddr[i] = destIpAddr.in6_u.u6_addr32[i];
	}
	#endif

	/*add hardware multicast entry*/
#if !defined(CONFIG_RTL_MULTI_LAN_DEV)
	#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
	if (strcmp(br->dev->name, RTL_PS_BR0_DEV_NAME) == 0)
	{
		memset(&fwdDescriptor, 0, sizeof(rtl8198c_mcast_fwd_descriptor6_t));
		strcpy(fwdDescriptor.netifName, "eth*");
		fwdDescriptor.fwdPortMask = 0xFFFFFFFF;
		ret = rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
	}
	else if (strcmp(br->dev->name,RTL_PS_BR1_DEV_NAME) == 0)
	{
		memset(&fwdDescriptor, 0, sizeof(rtl8198c_mcast_fwd_descriptor6_t));
		strcpy(fwdDescriptor.netifName, "eth2");
		fwdDescriptor.fwdPortMask = 0xFFFFFFFF;
		ret = rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex_2, &multicastDataInfo, &multicastFwdInfo);
	}
	#else
	memset(&fwdDescriptor, 0, sizeof(rtl8198c_mcast_fwd_descriptor6_t));
	strcpy(fwdDescriptor.netifName, "eth*");
	fwdDescriptor.fwdPortMask = 0xFFFFFFFF;

	ret = rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
	#endif
	if (ret != 0)
	{
		//if(net_ratelimit())printk("[%s:%d]get nic fwd info failed.\n",__FUNCTION__,__LINE__);
		return -1;
	}
	else
	{
		if (multicastFwdInfo.cpuFlag)
		{
			fwdDescriptor.toCpu = 1;
		}
		fwdDescriptor.fwdPortMask = multicastFwdInfo.fwdPortMask & (~(1 << srcPort));
	}
#else/*!CONFIG_RTL_MULTI_LAN_DEV*/
	memset(&fwdDescriptor, 0, sizeof(rtl8198c_mcast_fwd_descriptor6_t));
#if defined(CONFIG_RT_MULTIPLE_BR_SUPPORT)
	strcpy(fwdDescriptor.netifName, br->dev->name);
#else
	strcpy(fwdDescriptor.netifName, RTL_BR_NAME);
	if (multicastFwdInfo.cpuFlag)
	{
		fwdDescriptor.toCpu = 1;
	}
#endif
	fwdDescriptor.fwdPortMask = rtl865x_getPhyFwdPortMask(br, brFwdPortMask) & (~(1 << srcPort));
	//printk("[%s:%d]fwdDescriptor.fwdPortMask=%d.\n",__FUNCTION__,__LINE__,fwdDescriptor.fwdPortMask);
#endif/*!CONFIG_RTL_MULTI_LAN_DEV*/

#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
	fwdDescriptor.fwdPortMask &= mapPortMask;
#endif

	if (fwdDescriptor.fwdPortMask == 0)
		return -1;

#if defined(CONFIG_RTL_HW_VLAN_SUPPORT)
	if (rtl_hw_vlan_ignore_tagged_mc == 1)
		tagged_portmask = rtl_hw_vlan_get_tagged_portmask();
#endif
	if ((fwdDescriptor.fwdPortMask & tagged_portmask) == 0)
	{
#if defined(CONFIG_RTL_MULTICAST_PORT_MAPPING)
		ret = rtl8198C_addMulticastv6Entry(dip,sip, (unsigned short)srcVlanId, (unsigned short)srcPort,
							&fwdDescriptor, 1, 0, 0, 0, mapPortMask);

#else
		ret = rtl8198C_addMulticastv6Entry(dip,sip, (unsigned short)srcVlanId, (unsigned short)srcPort,
							&fwdDescriptor, 1, 0, 0, 0);
#endif
	}

	return 0;
}

#endif /* CONFIG_RTL_8198C || CONFIG_RTL_8197F */
#endif /*CONFIG_RTL_IGMP_SNOOPING*/
#endif /*CONFIG_RTL_HARDWARE_MULTICAST*/

#ifdef CONFIG_RTK_VLAN_WAN_TAG_SUPPORT
int rtl865x_same_root(struct net_device *dev1,struct net_device *dev2){

	struct net_bridge_port *p = br_port_get_rcu(dev1);
	struct net_bridge_port *p2 = br_port_get_rcu(dev2);
	return !strncmp(p->br->dev->name,p2->br->dev->name,3);
}
#endif
