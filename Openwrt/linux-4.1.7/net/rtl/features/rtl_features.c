/*
 * Copyright (C) 2017 Realtek Semiconductor Corporation.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/types.h>
#include <linux/netfilter.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/moduleparam.h>
#include <linux/notifier.h>
#include <linux/kernel.h>
#include <linux/inetdevice.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#if defined(CONFIG_PROC_FS)
#include <linux/proc_fs.h>
#endif

#if defined(CONFIG_RTL_HARDWARE_NAT) && (defined(CONFIG_RTL_MULTIPLE_WAN) || defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN))
#include <net/arp.h>
#endif

#if defined(CONFIG_RTL_AP_PACKAGE) && !defined(CONFIG_NETFILTER)
#else
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_helper.h>
#endif

#if defined(CONFIG_RTL_HARDWARE_NAT)
#include <net/ip_fib.h>
#include <net/ip_vs.h>
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_nat_core.h>
#endif
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
#include <linux/interrupt.h>
#include <linux/netfilter.h>
#endif
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl_nic.h>
#if defined(CONFIG_RTL_HARDWARE_NAT)
#include <net/rtl/rtl865x_nat.h>
#endif

#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
#include <net/rtl/rtl865x_ppp.h>
#include <net/rtl/rtl865x_route_api.h>
#include <net/rtl/rtl865x_arp_api.h>
#endif

#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
#include <net/rtl/rtl8198c_route_ipv6_api.h>
#include <net/rtl/rtl8198c_arp_ipv6_api.h>
#include <net/ip6_fib.h>
#endif

#ifdef CONFIG_RTL_LAYERED_DRIVER_L2
#include <net/rtl/rtl865x_fdb_api.h>
#endif

#include <net/rtl/features/rtl_features.h>
#include <linux/version.h>

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
#include <linux/netfilter/nf_conntrack_tcp.h>
#endif

#if defined(CONFIG_RTL_FAST_IPV6)
#include <linux/in6.h>
#include <linux/icmpv6.h>
#include <linux/mroute6.h>

#include <net/ipv6.h>
#include <net/transp_v6.h>
#include <net/rawv6.h>
#include <net/ndisc.h>
#include <net/ip6_route.h>
#include <net/addrconf.h>
#include <net/xfrm.h>
#endif
#include <net/rtl/fastpath/fastpath_core.h>

#if defined(CONFIG_RTL_LOCAL_PUBLIC) || defined(CONFIG_RTL_MULTIPLE_WAN) || defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN) || (defined(CONFIG_NET_SCHED) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)) || defined(CONFIG_RTL_HW_QOS_SUPPORT)
#include <net/rtl/rtl865x_netif.h>
#endif

#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
#include <net/rtl/rtl865x_outputQueue.h>
#endif

#if defined (CONFIG_RTL_LOCAL_PUBLIC)
#include <net/rtl/rtl865x_localPublic.h>
extern int rtl865x_curOpMode;
#endif

#if defined(CONFIG_RTL_FAST_BRIDGE)
#include <net/rtl/fastpath/fast_br.h>
#endif

#if defined(CONFIG_BRIDGE)
#include <bridge/br_private.h>
#endif

#if defined(CONFIG_NET_SCHED)
__DRAM_GEN int gQosEnabled;
#endif

#ifdef CONFIG_RTL_HARDWARE_NAT
/*2007-12-19*/
#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
#include <net/rtl/rtl865x_ip_api.h>
#endif
#endif

#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv4/ip_tables.h>

#if defined(FAST_PATH_SPI_ENABLED)
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
#include <net/netfilter/nf_conntrack_zones.h>
#endif
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
#include <linux/seq_file.h>
extern struct proc_dir_entry proc_root;
#endif

#if defined(CONFIG_FAST_PATH_MODULE) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)
#include <fast_l2tp_core.h>
enum LR_RESULT (*FastPath_hook4) (rtl_fp_napt_entry * fpNaptEntry) = NULL;
enum LR_RESULT (*FastPath_hook6) (rtl_fp_napt_entry * fpNaptEntry,
#if defined(IMPROVE_QOS)
	struct sk_buff * pskb, struct nf_conn * ct,
#endif
	enum NP_FLAGS flags) = NULL;

#include <linux/version.h>
enum LR_RESULT (*FastPath_hook11) (rtl_fp_napt_entry * fpNaptEntry, uint32 interval) = NULL;
int (*fast_path_hook) (struct sk_buff ** pskb) = NULL;

EXPORT_SYMBOL(FastPath_hook4);
EXPORT_SYMBOL(FastPath_hook6);
EXPORT_SYMBOL(FastPath_hook11);
EXPORT_SYMBOL(fast_path_hook);
#endif

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
extern int fast_nat_fw;
#endif

#ifdef FAST_PPTP
void (*sync_tx_pptp_gre_seqno_hook) (void *skb) = NULL;

#ifdef CONFIG_FAST_PATH_MODULE
EXPORT_SYMBOL(sync_tx_pptp_gre_seqno_hook);
#endif
#endif

#if defined CONFIG_RTL_8367_QOS_SUPPORT
extern void rtl8367_setFlowControl(int qosEnable);
#endif
void htonl_array(u32 *org_buf, u32 *dst_buf, unsigned int words)
{
	int i = 0;

	if (!org_buf || !dst_buf)
		return;
	while (words--) {
		dst_buf[i] = htonl(org_buf[i]);
		i++;
	}

	return;
}

#if 0
void ntohl_array(u32 *org_buf, u32 *dst_buf, unsigned int words)
{
	int i = 0;

	if (!org_buf || !dst_buf)
		return;
	while (words--) {
		dst_buf[i] = ntohl(org_buf[i]);
		i++;
	}

	return;
}
#endif

#if defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
extern int rtl_getMaxDecisionBitMap(unsigned int *maxDecision);
#endif

#if !defined(CONFIG_RTK_VLAN_SUPPORT)
extern int32 rtl8651_setAsicOperationLayer(uint32 layer);
#endif

int routerTypeFlag = 0;

#if defined(CONFIG_RTL_HARDWARE_NAT)
__DRAM_GEN int gHwNatEnabled;

EXPORT_SYMBOL(gHwNatEnabled);
#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
static int rtl_isWlanPkt(struct nf_conn *ct);
int extPortEnabled;
#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */

#if defined(CONFIG_RTL_VLAN_8021Q)
static int rtl_isHwNatLanVlan(struct nf_conn *ct);
extern int linux_vlan_enable;
extern int linux_vlan_hw_support_enable;
#endif

int32 rtl865x_handle_nat(struct nf_conn *ct, int act, struct sk_buff *skb)
{
	struct nf_conn_nat *nat;
	u_int32_t sip, dip, gip;
	u_int16_t sp, dp, gp, proto = 0;
	int timeval;
	int rc = 0;

	#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
	uint32 ret;
	struct net_device *lanDev, *wanDev;
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT */

	#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
	rtl865x_napt_entry rtl865xNaptEntry;
	rtl865x_priority rtl865xPrio;

	#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	int i;
	int cnt = 0;
	int first_match = 0;
	int maxDownPriority = 0;
	int maxUpPriority = 0;
	unsigned int maxDecisionBitMap = 0;
	unsigned int downPriority[max_decision_priority];
	unsigned int upPriority[max_decision_priority];
	unsigned int maxDecision[max_decision_priority];
	rtl865x_qos_mark rtl865xQosMark[max_decision_priority];
	#else
	rtl865x_qos_mark rtl865xQosMark;
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
	#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
	#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
	uint32 isExtNaptEntry = 0;
	#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */

	if (gHwNatEnabled != 1)
		return -1;

	#if defined(CONFIG_RTL_VLAN_8021Q)
	if (linux_vlan_enable && linux_vlan_hw_support_enable &&
		rtl_isHwNatLanVlan(ct) == FALSE)
		return -1;
	#endif /* CONFIG_RTL_VLAN_8021Q */

	proto = (ct->tuplehash[0].tuple.dst.protonum == IPPROTO_TCP) ? RTL865X_PROTOCOL_TCP : RTL865X_PROTOCOL_UDP;

	if (ct->status & IPS_SRC_NAT) {	/* outbound flow */
		sip = ct->tuplehash[0].tuple.src.u3.ip;
		dip = ct->tuplehash[0].tuple.dst.u3.ip;
		gip = ct->tuplehash[1].tuple.dst.u3.ip;
		sp = (proto) ? ct->tuplehash[0].tuple.src.u.tcp.port : ct->tuplehash[0].tuple.src.u.udp.port;
		dp = (proto) ? ct->tuplehash[0].tuple.dst.u.tcp.port : ct->tuplehash[0].tuple.dst.u.udp.port;
		gp = (proto) ? ct->tuplehash[1].tuple.dst.u.tcp.port : ct->tuplehash[1].tuple.dst.u.udp.port;
	} else if (ct->status & IPS_DST_NAT) {	/* inbound flow */
		sip = ct->tuplehash[1].tuple.src.u3.ip;
		dip = ct->tuplehash[1].tuple.dst.u3.ip;
		gip = ct->tuplehash[0].tuple.dst.u3.ip;
		sp = (proto) ? ct->tuplehash[1].tuple.src.u.tcp.port : ct->tuplehash[1].tuple.src.u.udp.port;
		dp = (proto) ? ct->tuplehash[1].tuple.dst.u.tcp.port : ct->tuplehash[1].tuple.dst.u.udp.port;
		gp = (proto) ? ct->tuplehash[0].tuple.dst.u.tcp.port : ct->tuplehash[0].tuple.dst.u.udp.port;
	} else
		return -1;

	/* do not add hardware NAPT table if protocol is UDP and source IP address is equal to gateway IP address */
	if ((act == 1) && (proto == 0) && (sip == gip))
		return -1;

	/* for TZO DDNS */
	if ((act == 1) && (proto == 1) && (dp == htons(21347))) {
		return -1;
	}

	if (act == 2) {
		/* query for idle */
		timeval = 0;
		#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
		rtl865xNaptEntry.protocol = proto;
		rtl865xNaptEntry.intIp = sip;
		rtl865xNaptEntry.intPort = sp;
		rtl865xNaptEntry.extIp = gip;
		rtl865xNaptEntry.extPort = gp;
		rtl865xNaptEntry.remIp = dip;
		rtl865xNaptEntry.remPort = dp;

		timeval = rtl865x_naptSync(&rtl865xNaptEntry, 0);
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
		return timeval;
	} else if (act == 0) {
		/* delete */
		rc = 0;
		#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
		rtl865xNaptEntry.protocol = proto;
		rtl865xNaptEntry.intIp = sip;
		rtl865xNaptEntry.intPort = sp;
		rtl865xNaptEntry.extIp = gip;
		rtl865xNaptEntry.extPort = gp;
		rtl865xNaptEntry.remIp = dip;
		rtl865xNaptEntry.remPort = dp;

		rc = rtl865x_delNaptConnection(&rtl865xNaptEntry);
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
	} else {
		#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
		if (NULL != skb) {
			rtl865xNaptEntry.protocol = proto;
			rtl865xNaptEntry.intIp = sip;
			rtl865xNaptEntry.intPort = sp;
			rtl865xNaptEntry.extIp = gip;
			rtl865xNaptEntry.extPort = gp;
			rtl865xNaptEntry.remIp = dip;
			rtl865xNaptEntry.remPort = dp;

			#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
			memset(upPriority, 0, sizeof(upPriority));
			memset(downPriority, 0, sizeof(downPriority));
			memset(maxDecision, 0, sizeof(maxDecision));
			memset(rtl865xQosMark, 0, sizeof(rtl865xQosMark));

			ret = rtl_qosGetSkbMarkByNaptEntry(&rtl865xNaptEntry,
				rtl865xQosMark, skb);
			lanDev = rtl865x_getLanDev();
			wanDev = rtl865x_getWanDev();
			for (i = port_decision_priority; i < max_decision_priority; i++) {
				if (rtl865xQosMark[i].downlinkMark) {
					downPriority[i] =rtl_qosGetPriorityByMark(lanDev->name,
						rtl865xQosMark[i].downlinkMark);
				}

				if (rtl865xQosMark[i].uplinkMark) {
					upPriority[i] =rtl_qosGetPriorityByMark(wanDev->name,
						rtl865xQosMark[i].uplinkMark);
				}
			}

			rtl_getMaxDecisionBitMap(&maxDecisionBitMap);
			for (i = 0; i < 5; i++) {
				if (maxDecisionBitMap & (1 << i)) {
					if (++cnt == 1)
						first_match = i;
					maxDecision[i] = 1;
				}
			}

			if (cnt == 1) {	//only one decision priority is max, so just use this decision
				rtl865xPrio.downlinkPrio =downPriority[first_match];
				rtl865xPrio.uplinkPrio =upPriority[first_match];
				/*Should I update skb->mark here? */
			} else if (cnt > 1) {	//more than one decision priority are max, use the highest priority
				for (i = 0; i < 5; i++) {
					if ((maxDecision[i] == 1)) {
						if (downPriority[i] > maxDownPriority)
							maxDownPriority =downPriority[i];

						if (upPriority[i] > maxUpPriority)
							maxUpPriority =upPriority[i];
					}
				}
				rtl865xPrio.downlinkPrio = maxDownPriority;
				rtl865xPrio.uplinkPrio = maxUpPriority;
			}
			#else
			rtl865xQosMark.downlinkMark = 0;	//Initial
			rtl865xQosMark.uplinkMark = 0;		//Initial
			ret = rtl_qosGetSkbMarkByNaptEntry(&rtl865xNaptEntry,
				&rtl865xQosMark, skb);

			lanDev = rtl865x_getLanDev();
			wanDev = rtl865x_getWanDev();
			rtl865xPrio.downlinkPrio =rtl_qosGetPriorityByMark(lanDev->name,
				rtl865xQosMark.downlinkMark);
			rtl865xPrio.uplinkPrio =rtl_qosGetPriorityByMark(wanDev->name,
				rtl865xQosMark.uplinkMark);
			#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

			if (lanDev)
				dev_put(lanDev);

			if (wanDev)
				dev_put(wanDev);
		} else
		#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
		{
			rtl865xPrio.downlinkPrio = 0;
			rtl865xPrio.uplinkPrio = 0;
		}

		#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
		rtl865xNaptEntry.protocol = proto;
		rtl865xNaptEntry.intIp = sip;
		rtl865xNaptEntry.intPort = sp;
		rtl865xNaptEntry.extIp = gip;
		rtl865xNaptEntry.extPort = gp;
		rtl865xNaptEntry.remIp = dip;
		rtl865xNaptEntry.remPort = dp;
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
		#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
		if (extPortEnabled) {
			if (rtl865x_checkL3L2Valid(sip) == 0) {
				return -1;
			}
			if (rtl865x_checkL3L2Valid(dip) == 0) {
				return -1;
			}

			if (rtl_isWlanPkt(ct) == TRUE) {
				isExtNaptEntry = 1;
			}
			#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
			rtl865xNaptEntry.isExtNaptEntry = isExtNaptEntry;
			#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
		} else {
			#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
			rtl865xNaptEntry.isExtNaptEntry = 0;
			#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */

		}
		#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */
		/* add */
		#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED) && !defined(CONFIG_RTL_HW_QOS_SUPPORT)
		if (gQosEnabled == 0) {
			#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
			rc = rtl865x_addNaptConnection(&rtl865xNaptEntry,
				&rtl865xPrio);
			#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
		} else {
			act = 0;
		}
		#else
		#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
		rc = rtl865x_addNaptConnection(&rtl865xNaptEntry, &rtl865xPrio);
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */

		#endif /* CONFIG_PROC_FS && CONFIG_NET_SCHED && !CONFIG_RTL_HW_QOS_SUPPORT */
	}

	nat = nfct_nat(ct);
	if (!rc && nat && act == 1)	/* mark it as an asic entry */
		nat->hw_acc = 1;
	if (!rc && nat && act == 0)	/* unmark it */
		nat->hw_acc = 0;

	#ifdef CONFIG_HARDWARE_NAT_DEBUG
	/*2007-12-19 */
	pr_err("%s:%d:(%s): errno=%d\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
		__FUNCTION__, __LINE__, ((act == 1) ? "add_nat" : "del_nat"), rc, ((proto) ? "tcp" : "udp"),
		NIPQUAD(sip), sp, NIPQUAD(dip), dp, NIPQUAD(gip), gp);
	#endif /* CONFIG_HARDWARE_NAT_DEBUG */

	return 0;
}

/* return value:
	FAILED:		ct should be delete
	SUCCESS:	ct should NOT be delete.
*/
int rtl_hwnat_timer_update(struct nf_conn *ct)
{
	unsigned long expires, now, elasped;
	struct nf_conn_nat *nat;

	if (gHwNatEnabled != 1)
		return FAILED;

	nat = nfct_nat(ct);
	if (nat == NULL || nat->hw_acc != 1)
		return FAILED;

	now = jiffies;
	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
		if (ct->status & IPS_SEEN_REPLY) {
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
			expires =udp_get_timeouts_by_state(UDP_CT_REPLIED,
				(void *)ct, 1);
			#else
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
			expires = udp_get_timeouts_by_state(UDP_CT_REPLIED);
			#else
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0))
			//TBD
			#else
			expires = nf_ct_udp_timeout_stream;
			#endif /* 3.3.0 > version >= 3.2.0 */
			#endif /* version >= 3.4.0 */
			#endif /* version >= 3.10.0 */
		} else {
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
			expires =udp_get_timeouts_by_state(UDP_CT_UNREPLIED,
				(void *)ct, 1);
			#else
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
			expires = udp_get_timeouts_by_state(UDP_CT_UNREPLIED);
			#else
			#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0))
			//TBD
			#else
			expires = nf_ct_udp_timeout;
			#endif /* 3.3.0 > version >= 3.2.0 */
			#endif /* version >= 3.4.0 */
			#endif /* version >= 3.10.0 */
		}
	} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum ==IPPROTO_TCP && ct->proto.tcp.state < TCP_CONNTRACK_LAST_ACK) {
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
		expires =tcp_get_timeouts_by_state(ct->proto.tcp.state,
			(void *)ct, 1);
		#else
		#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)) && (LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0))
		//TBD
		#else
		expires = tcp_get_timeouts_by_state(ct->proto.tcp.state);
		#endif /* 3.3.0 > version >= 3.2.0 */
		#endif /* version >= 3.10.0 */
	} else {
		return FAILED;
	}

	elasped = rtl865x_handle_nat(ct, 2, NULL);
	if (elasped >= 0 && (elasped * HZ) < expires) {
		/* update ct expires time */
		ct->timeout.expires = now + (expires - (elasped * HZ));
		rtl_check_for_acc(ct, ct->timeout.expires);
		return SUCCESS;
	} else {
		return FAILED;
	}

}
#endif

int get_dev_ip_mask(const char *name, unsigned int *ip, unsigned int *mask)
{
	struct in_device *in_dev;
	struct net_device *landev;
	struct in_ifaddr *ifap = NULL;

	if ((name == NULL) || (ip == NULL) || (mask == NULL)) {
		return -1;
	}

	if ((landev = __dev_get_by_name(&init_net, name)) != NULL) {
		in_dev = (struct in_device *)(landev->ip_ptr);
		if (in_dev != NULL) {
			for (ifap = in_dev->ifa_list; ifap != NULL;
				ifap = ifap->ifa_next) {
				if (strcmp(name, ifap->ifa_label) == 0) {
					*ip = ifap->ifa_address;
					*mask = ifap->ifa_mask;
					return 0;
				}
			}

		}
	}

	return -1;
}

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_WLAN_DOS_FILTER) || defined(CONFIG_RTL_BATTLENET_ALG) || defined(CONFIG_RTL_USB_IP_HOST_SPEEDUP) || defined(CONFIG_HTTP_FILE_SERVER_SUPPORT) || defined(CONFIG_RTL_USB_UWIFI_HOST_SPEEDUP)
unsigned int _br0_ip;
unsigned int _br0_mask;

#ifndef CONFIG_RTL8686_GMAC
static void get_br0_ip_mask(void)
#else
void get_br0_ip_mask(void)
#endif /* !CONFIG_RTL8686_GMAC */
{
	get_dev_ip_mask(RTL_PS_BR0_DEV_NAME, &_br0_ip, &_br0_mask);
}
#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT || CONFIG_RTL_WLAN_DOS_FILTER || CONFIG_RTL_BATTLENET_ALG || CONFIG_RTL_USB_IP_HOST_SPEEDUP || CONFIG_HTTP_FILE_SERVER_SUPPORT || CONFIG_RTL_USB_UWIFI_HOST_SPEEDUP */

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
int smart_count = 0;
unsigned long smart_count_start_timer;

/* return value:
	FAILED:			ct should be delete
	SUCCESS:		ct should NOT be delete.
*/
void rtl_delConnCache(struct nf_conn *ct)
{
	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	enum NP_PROTOCOL protocol;
	rtl_fp_napt_entry rtlFpNaptEntry;
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	#ifdef CONFIG_RTL_HARDWARE_NAT
	struct nf_conn_nat *nat;
	#endif /* CONFIG_RTL_HARDWARE_NAT */
	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	if (ct->removed == 1)	// this ct's fastpath entry was already deleted.
		return;
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */
	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		protocol = NP_TCP;
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum ==IPPROTO_UDP) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		protocol = NP_UDP;
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	}
	#if defined(CONFIG_RTL_FAST_GRE)
	else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum ==IPPROTO_GRE) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		protocol = NP_GRE;
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	}
	#endif /* CONFIG_RTL_FAST_GRE */
	else {
		return;
	}

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	#else
	spin_lock_bh(&nf_conntrack_lock);
	#endif /* version >= 3.10.0 */
	#if defined(CONFIG_RTL_HARDWARE_NAT)
	nat = nfct_nat(ct);
	if ((nat != NULL) && (nat->hw_acc == 1)) {
		rtl865x_handle_nat(ct, 0, NULL);
	}
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	if (!fast_nat_fw)
		return;

	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip
		== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip) {
		/*case  WAN->LAN(BC->AB) use C|A-B */

		rtlFpNaptEntry.protocol = protocol;
		rtlFpNaptEntry.intIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
		rtlFpNaptEntry.intPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
		rtlFpNaptEntry.extIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
		rtlFpNaptEntry.extPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;
		rtlFpNaptEntry.remIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		rtlFpNaptEntry.remPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;

		#ifdef CONFIG_FAST_PATH_MODULE
		if (FastPath_hook4 != NULL) {
			FastPath_hook4(&rtlFpNaptEntry);
		}
		#else
		rtk_delNaptConnection(&rtlFpNaptEntry);
		#endif /* CONFIG_FAST_PATH_MODULE */
	} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip
		== ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) {
		/*case  LAN->WAN(AB->BC) use A|C-B */

		rtlFpNaptEntry.protocol = protocol;
		rtlFpNaptEntry.intIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		rtlFpNaptEntry.intPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
		rtlFpNaptEntry.extIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
		rtlFpNaptEntry.extPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
		rtlFpNaptEntry.remIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
		rtlFpNaptEntry.remPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;

		#ifdef CONFIG_FAST_PATH_MODULE
		if (FastPath_hook4 != NULL) {
			FastPath_hook4(&rtlFpNaptEntry);
		}
		#else
		rtk_delNaptConnection(&rtlFpNaptEntry);
		#endif /* CONFIG_FAST_PATH_MODULE */
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	ct->removed = 1;	// set this ct has delete fastpath entry
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	#else
	spin_unlock_bh(&nf_conntrack_lock);
	#endif /* version >= 3.10.0 */
}

void rtl_check_for_acc(struct nf_conn *ct, unsigned long expires)
{
	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	int newstate;
	struct list_head *state_hash;

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	#else
	write_lock_bh(&nf_conntrack_lock);
	#endif /* version >= 3.10.0 */
	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_TCP) {
		newstate = ct->proto.tcp.state;
		state_hash = Tcp_State_Hash_Head[newstate].state_hash;
	} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum ==IPPROTO_UDP) {
		if (ct->status & IPS_SEEN_REPLY)
			newstate = 1;
		else
			newstate = 0;
		state_hash = Udp_State_Hash_Head[newstate].state_hash;
	} else {
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
		#else
		write_unlock_bh(&nf_conntrack_lock);
		#endif /* version >= 3.10.0 */
		return;
	}

	list_move_tail(&ct->state_tuple, state_hash);
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	#else
	write_unlock_bh(&nf_conntrack_lock);
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */
}

int32 rtl_connCache_timer_update(struct nf_conn *ct)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	local_bh_disable();
	#else /* !CONFIG_RTD_1295_HWNAT */
	spin_lock_bh(&nf_conntrack_lock);
	#endif /* CONFIG_RTD_1295_HWNAT */
	if (time_after_eq(jiffies, ct->timeout.expires)) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if ((fast_nat_fw)
			&& (SUCCESS == rtl_fpTimer_update((void *)ct))) {
			add_timer(&ct->timeout);
			#if defined(CONFIG_RTD_1295_HWNAT)
			local_bh_enable();
			#else /* !CONFIG_RTD_1295_HWNAT */
			spin_unlock_bh(&nf_conntrack_lock);
			#endif /* CONFIG_RTD_1295_HWNAT */
			return SUCCESS;
		}
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

		#if defined(CONFIG_RTL_HARDWARE_NAT)
		if ((gHwNatEnabled) && (SUCCESS == rtl_hwnat_timer_update(ct))) {
			add_timer(&ct->timeout);
			#if defined(CONFIG_RTD_1295_HWNAT)
			local_bh_enable();
			#else /* !CONFIG_RTD_1295_HWNAT */
			spin_unlock_bh(&nf_conntrack_lock);
			#endif /* CONFIG_RTD_1295_HWNAT */
			return SUCCESS;
		}
		#endif /* CONFIG_RTL_HARDWARE_NAT */
	}
	#if defined(CONFIG_RTD_1295_HWNAT)
	local_bh_enable();
	#else /* !CONFIG_RTD_1295_HWNAT */
	spin_unlock_bh(&nf_conntrack_lock);
	#endif /* CONFIG_RTD_1295_HWNAT */
	return FAILED;
}

#ifdef CONFIG_RTL_FAST_IPV6
int rtl_V6_connCache_timer_update(struct nf_conn *ct)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	local_bh_disable();
	#else /* !CONFIG_RTD_1295_HWNAT */
	spin_lock_bh(&nf_conntrack_lock);
	#endif /* CONFIG_RTD_1295_HWNAT */
	if (time_after_eq(jiffies, ct->timeout.expires)) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if (SUCCESS == rtl_V6_Cache_Timer_update((void *)ct)) {
			add_timer(&ct->timeout);
			#if defined(CONFIG_RTD_1295_HWNAT)
			local_bh_enable();
			#else /* !CONFIG_RTD_1295_HWNAT */
			spin_unlock_bh(&nf_conntrack_lock);
			#endif /* CONFIG_RTD_1295_HWNAT */
			return SUCCESS;
		}
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	}
	#if defined(CONFIG_RTD_1295_HWNAT)
	local_bh_enable();
	#else /* !CONFIG_RTD_1295_HWNAT */
	spin_unlock_bh(&nf_conntrack_lock);
	#endif /* CONFIG_RTD_1295_HWNAT */
	return FAILED;

}
#endif /* CONFIG_RTL_FAST_IPV6 */

#if defined(IMPROVE_QOS)
/*
 * ### for iperf application test ###
 * the behavior of iperf UDP test is LAN PC (client) will burst UDP from LAN to WAN (by one way),
 * WAN PC (server) will only send one UDP packet (statistics) at the end of test.
 * so the fastpath or hardware NAT will create link at the end of test.
 *
 * the purpose for adding the following code is to create fastpath or hardware NAT link
 * when we only get one packet from LAN to WAN in UDP case.
 */
static inline int32 rtl_addConnCheck(struct nf_conn *ct, struct iphdr *iph,
	struct sk_buff *skb)
{
	extern unsigned int _br0_ip;
	extern unsigned int _br0_mask;
	extern int smart_count;
	extern unsigned long smart_count_start_timer;
	struct tcphdr *tcph;
	uint32 sip, dip;
	int32 create_conn;

	sip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
	dip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
	create_conn = FALSE;

	if (((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip
			 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) ||
			(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip
			 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))
	#if defined(UNNUMBER_IP)
		&& (is_unnumber_ip(dip) == FALSE)
	#endif /* UNNUMBER_IP */
	) {
		/* UDP and "LAN to WAN" */
		/* ignore some cases:
		 *	1. sip = br0's ip -----> (ex. sip 192.168.1.254 ==> dip 239.255.255.250)
		 *	2. (sip & br0's mask) != (br0's ip & br0's mask) -----> sip is not in br0's subnet
		 *	3. (dip & br0's mask) =  (br0's ip & br0's mask) -----> dip is in br0's subnet
		 *	4. dip != multicast IP address
		 *	5. sip != gip
		 */
		if (iph->protocol == IPPROTO_UDP) {
			if ((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip
					 == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)&&
				(sip != _br0_ip) &&
				((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) != (_br0_ip & _br0_mask)) &&
				((dip & htonl(0xf0000000)) != htonl(0xe0000000)) &&
				(sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))) {
				create_conn = TRUE;
			} else if ((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip
					 	== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip) &&
					 ((sip & _br0_mask) != (_br0_ip & _br0_mask)) &&
					 ((dip & _br0_mask) == (_br0_ip & _br0_mask)) &&
					 (iph->daddr ==dip)) {
				create_conn = TRUE;
			}
		} else if (iph->protocol == IPPROTO_TCP) {
			tcph = (void *)iph + iph->ihl * 4;
			if (!tcph->fin && !tcph->syn && !tcph->rst &&
				#if !defined(CONFIG_RTL_URL_PATCH)
				tcph->psh == 1 &&
				#endif /* CONFIG_RTL_URL_PATCH */
				tcph->ack == 1 &&
				(((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip == ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) &&
				(iph->daddr !=_br0_ip) && ((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) != (_br0_ip & _br0_mask)) && (sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip))) ||
				((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip==ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip) &&
				((sip & _br0_mask) != (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) == (_br0_ip & _br0_mask)) && (sip == iph->saddr)))) {

				#if defined(CONFIG_RTL_URL_PATCH)
				if((ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) == URL_PROTO_PORT) ||
					((ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) != URL_PROTO_PORT) &&
					 (tcph->psh==1)))
				#endif /* CONFIG_RTL_URL_PATCH */
				{
					if (smart_count == 0) {
						smart_count_start_timer = jiffies + HZ;
					}

				if (time_after(jiffies, smart_count_start_timer)) {
						smart_count_start_timer = jiffies + HZ;
						smart_count = 0;
					}

					smart_count++;
					if (smart_count > 810) {
						create_conn = TRUE;
					}
				}
			}
		} else {
			return create_conn;
		}

		#if defined(UNNUMBER_IP)
		if ((!create_conn) && (is_unnumber_ip(sip) == TRUE))) {
			create_conn = TRUE;
		}
		#endif /* UNNUMBER_IP */
	}

	return create_conn;
}

#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
static int rtl_isWlanPkt(struct nf_conn *ct) {
	int ret = FALSE;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	u16 vid = 0;
	#endif /* version >= 3.10.0 */

	#if defined(CONFIG_BRIDGE)
	struct net_device *lan_dev =__dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
	struct net_bridge *br = netdev_priv(lan_dev);
	struct net_bridge_fdb_entry *dst;
	unsigned char Mac[6];
	__be32 intIp;

	if (ct->status & IPS_SRC_NAT) {
		intIp = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
	} else if (ct->status & IPS_DST_NAT) {
		intIp = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
	}

	if ((intIp & _br0_mask) == (_br0_ip & _br0_mask)) {
		if (arp_req_get_ha(intIp, lan_dev, Mac) == 0) {
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
			#if defined(CONFIG_BRIDGE_VLAN_FILTERING)
			/*Fix jwj: todo... */
			#else
			vid = 0;
			#endif /* CONFIG_BRIDGE_VLAN_FILTERING */
			if ((dst = __br_fdb_get(br, Mac, vid)) != NULL)
			#else
			if ((dst = __br_fdb_get(br, Mac)) != NULL)
			#endif /* version >= 3.10.0 */
			{
				if (!memcmp(dst->dst->dev->name, "wlan",
						strlen("wlan")))
					return TRUE;
			}
		}
	}
	#endif /* CONFIG_BRIDGE */

	return ret;
}
#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */

#ifdef CONFIG_RTL_VLAN_8021Q
extern uint16 rtk_vlan_hw_nat_lan_vid;
extern uint16 rtk_vlan_wan_vid;

static int rtl_isHwNatLanVlan(struct nf_conn *ct) {
	int ret = FALSE;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	u16 vid = 0;
	#endif /* version >= 3.10.0 */

	#if defined(CONFIG_BRIDGE)
	struct net_device *lan_dev =__dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
	struct net_bridge *br = netdev_priv(lan_dev);
	struct net_bridge_fdb_entry *dst;
	unsigned char Mac[6];
	__be32 intIp = 0;

	if (ct->status & IPS_SRC_NAT) {
		intIp = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
	} else if (ct->status & IPS_DST_NAT) {
		intIp = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
	}

	if ((intIp & _br0_mask) == (_br0_ip & _br0_mask)) {
		if (arp_req_get_ha(intIp, lan_dev, Mac) == 0) {
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
			#if defined(CONFIG_BRIDGE_VLAN_FILTERING)
			/*Fix jwj: todo... */
			#else
			vid = 0;
			#endif /* CONFIG_BRIDGE_VLAN_FILTERING */
			if ((dst = __br_fdb_get(br, Mac, vid)) != NULL)
			#else
			if ((dst = __br_fdb_get(br, Mac)) != NULL)
			#endif /* version >= 3.10.0 */
			{
				if (dst->dst->dev->vlan_id ==rtk_vlan_hw_nat_lan_vid)
					return TRUE;
			}
		}
	}
	#endif /* CONFIG_BRIDGE */

	return ret;
}
#endif /* CONFIG_RTL_VLAN_8021Q */

#if defined(CONFIG_RTL_AVOID_ADDING_WAN_SUBNET_PKT_TO_HW_NAT)
extern int rtl_is_gw_ip_by_dev_name(char *name, ipaddr_t ip);
static int rtl_checkWanIp(struct nf_conn *ct, char *name) {
	__be32 wanIp = 0;
	__be32 wanDevIp = 0;
	__be32 wanDevMask = 0;

	if (strncmp(name, "ppp", 3) == 0)
		return SUCCESS;

	 get_dev_ip_mask(name, &wanDevIp, &wanDevMask);

	if (ct->status & IPS_SRC_NAT) {
		wanIp = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
	} else if (ct->status & IPS_DST_NAT) {
		wanIp = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
	} else {
		return FAILED;
	}

	if (((wanIp & wanDevMask) == (wanDevIp & wanDevMask))
		&& (rtl865x_isEthArp(htonl(wanIp)) == FALSE)
		&& (rtl_is_gw_ip_by_dev_name(name, wanIp) == FALSE))
		return FAILED;

	return SUCCESS;
}
#endif /* CONFIG_RTL_AVOID_ADDING_WAN_SUBNET_PKT_TO_HW_NAT */

#if defined(CONFIG_RTL_HARDWARE_NAT)
static int rtl_checkLanIp(struct nf_conn *ct) {

	__be32 lanIp = 0;

	if (ct->status & IPS_SRC_NAT) {
		lanIp = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
	} else if (ct->status & IPS_DST_NAT) {
		lanIp = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
	} else {
		return FAILED;
	}

	if (rtl865x_isEthArp(htonl(lanIp)) == TRUE) {
		return SUCCESS;
	}

	return FAILED;
}

#endif /* CONFIG_RTL_HARDWARE_NAT */

#ifdef CONFIG_FP_BYPASS_PACKET
#define BYPASS_FAILED -1
#define BYPASS_OK 0
struct port_range {
	unsigned int startport;
	char separator;
	unsigned int endport;
};

static unsigned int hw_nat_add = 1;
static unsigned int tcp_bypass_port_num = 0;
static unsigned int udp_bypass_port_num = 0;
static struct port_range *packet_bypassed_tcpport_set = NULL;
static unsigned char *packet_bypassed_tcpport_set_buffer = NULL;
static struct port_range *packet_bypassed_udpport_set = NULL;
static unsigned char *packet_bypassed_udpport_set_buffer = NULL;

int proc_hwnat_packet_bypassed_tcpport_read_proc(char *page, char **start,
	off_t off, int count, int *eof, void *data)
{
	int len = 0;
	int i = 0;
	len += sprintf(page + len,
		"hwnat bypassed tcp port,tcp_bypass_port_num %d:\n",
		tcp_bypass_port_num);

	for (; i < tcp_bypass_port_num; i++)
		len += sprintf(page + len, "%d%c%d ",
			packet_bypassed_tcpport_set[i].startport,
			packet_bypassed_tcpport_set[i].separator,
			packet_bypassed_tcpport_set[i].endport);

	 len += sprintf(page + len, "\n");
	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	 len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}

static unsigned int countArg_num(char *buffer, unsigned long count)
{
	int i = 0;
	unsigned first = 1;
	int arg_count = 0;

	if (!buffer)
		 return 0;

	while (i < count) {
		/*input rest arg */
		if (!first && i > 0 && *(buffer + i) != ' '
			&& *(buffer + i - 1) == ' ' && *(buffer + i) != '\n'
			&& *(buffer + i) != '\r') {
			arg_count++;
		}

		/* input first arg */
		if (*(buffer + i) != ' ' && first) {
			first = 0;
			arg_count++;
		}
		i++;
	}
	return arg_count;

}

static unsigned int fill_bypass_port_single_num(char *buffer,
	unsigned long count, unsigned int *udportcp_buffer)
{
	int i = 0;
	unsigned first = 1;
	int arg_count = 0;

	if (!udportcp_buffer || !buffer)
		 return 0;

	while (i < count) {
		/*input rest arg */
		if (!first && i > 0 && *(buffer + i) != ' '
			&& *(buffer + i - 1) == ' ' && *(buffer + i) != '\n'
			&& *(buffer + i) != '\r') {
			sscanf((buffer + i), "%d",
				&(udportcp_buffer[arg_count]));
			arg_count++;
		}

		/* input first arg */
		if (*(buffer + i) != ' ' && first) {
			first = 0;
			sscanf((buffer + i), "%d",
				&(udportcp_buffer[arg_count]));
			arg_count++;
		}
		i++;
	}
	return arg_count;
}

static unsigned int fill_bypass_port(char *buffer, unsigned long count,
	struct port_range *udportcp_buffer)
{
	int i = 0;
	unsigned first = 1;
	int arg_count = 0;

	if (!udportcp_buffer || !buffer)
		 return 0;

	while (i < count) {
		/* input rest arg */
		if (!first && i > 0 && *(buffer + i) != ' '
			&& *(buffer + i - 1) == ' ' && *(buffer + i) != '\n'
			&& *(buffer + i) != '\r') {
			sscanf((buffer + i), "%d%c%d",
				&((udportcp_buffer[arg_count]).startport),
				&((udportcp_buffer[arg_count]).separator),
				&((udportcp_buffer[arg_count]).endport));
			if ((udportcp_buffer[arg_count]).separator != ',') {
				printk("write failed : wrong separator should use ',' to separator port range \"startport,endport\"\n");
				return BYPASS_FAILED;
			} else if ((udportcp_buffer[arg_count]).startport >
				(udportcp_buffer[arg_count]).endport) {
				printk("write failed : wrong port range the %d count portrange set error, should be startport < endport\n", arg_count);
				return BYPASS_FAILED;
			}
			arg_count++;
		}

		/* input first arg */
		if (*(buffer + i) != ' ' && first) {
			first = 0;
			sscanf((buffer + i), "%d%c%d",
				&((udportcp_buffer[arg_count]).startport),
				&((udportcp_buffer[arg_count]).separator),
				&((udportcp_buffer[arg_count]).endport));
			if ((udportcp_buffer[arg_count]).separator != ',') {
				printk("write failed : wrong separator should use ',' to separator port range \"startport,endport\"\n");
				return BYPASS_FAILED;
			} else if ((udportcp_buffer[arg_count]).startport >
				(udportcp_buffer[arg_count]).endport) {
				printk("write failed : wrong port range the %d count portrange set error, should be startport < endport\n", arg_count);
				return BYPASS_FAILED;
			}
			arg_count++;
		}
		i++;
	}
	return arg_count;
}

int proc_hwnat_packet_bypassed_tcpport_write_proc(struct file *file,
	const char *buffer, unsigned long count, void *data)
{
	unsigned int arg_count = 0;
	if (count < 1)
		return -EFAULT;
	packet_bypassed_tcpport_set_buffer = (unsigned char *)kmalloc(count, GFP_ATOMIC);
	if (!packet_bypassed_tcpport_set_buffer) {
		printk("not enough memory\n");
		return -ENOMEM;
	}
	memset(packet_bypassed_tcpport_set_buffer, 0, count);

	if (buffer && !copy_from_user(packet_bypassed_tcpport_set_buffer, buffer, count)) {
		tcp_bypass_port_num =countArg_num(packet_bypassed_tcpport_set_buffer, count);
		if (packet_bypassed_tcpport_set)
			kfree(packet_bypassed_tcpport_set);
		packet_bypassed_tcpport_set =kmalloc(tcp_bypass_port_num * sizeof(struct port_range), GFP_ATOMIC);
		if (!packet_bypassed_tcpport_set) {
			printk("not enough memory\n");
			return -ENOMEM;
		}
		memset(packet_bypassed_tcpport_set, 0, tcp_bypass_port_num * sizeof(struct port_range));
		arg_count = fill_bypass_port(packet_bypassed_tcpport_set_buffer,
			count, packet_bypassed_tcpport_set);
		if ((arg_count == BYPASS_FAILED) || (arg_count == 1
				&& packet_bypassed_tcpport_set[0].startport == 0
				&& packet_bypassed_tcpport_set[0].endport ==0)) {
			tcp_bypass_port_num = 0;
			kfree(packet_bypassed_tcpport_set);
			packet_bypassed_tcpport_set = NULL;
		}
	}

	kfree(packet_bypassed_tcpport_set_buffer);
	return count;
}

int proc_hwnat_packet_bypassed_udpport_read_proc(char *page, char **start,
	off_t off, int count, int *eof, void *data)
{
	int len = 0;
	int i = 0;
	len += sprintf(page + len,
		"hwnat bypassed udp port,udp_bypass_port_num %d:\n",
		udp_bypass_port_num);

	for (; i < udp_bypass_port_num; i++)
		len += sprintf(page + len, "%d%c%d ",
			packet_bypassed_udpport_set[i].startport,
			packet_bypassed_udpport_set[i].separator,
			packet_bypassed_udpport_set[i].endport);

	len += sprintf(page + len, "\n");
	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		 len = count;
	if (len < 0)
		 len = 0;
	return len;
}

int proc_hwnat_packet_bypassed_udpport_write_proc(struct file *file,
	const char *buffer, unsigned long count, void *data)
{
	unsigned int arg_count = 0;
	if (count < 1)
		return -EFAULT;
	packet_bypassed_udpport_set_buffer = (unsigned char *)kmalloc(count, GFP_ATOMIC);
	if (!packet_bypassed_udpport_set_buffer) {
		printk("not enough memory\n");
		return -ENOMEM;
	}
	memset(packet_bypassed_udpport_set_buffer, 0, count);

	if (buffer && !copy_from_user(packet_bypassed_udpport_set_buffer, buffer, count)) {
		udp_bypass_port_num =countArg_num(packet_bypassed_udpport_set_buffer, count);
		if (packet_bypassed_udpport_set)
			kfree(packet_bypassed_udpport_set);
		packet_bypassed_udpport_set =kmalloc(udp_bypass_port_num * sizeof(struct port_range), GFP_ATOMIC);
		if (!packet_bypassed_udpport_set) {
			printk("not enough memory\n");
			return -ENOMEM;
		}
		memset(packet_bypassed_udpport_set, 0, udp_bypass_port_num * sizeof(struct port_range));
		arg_count =fill_bypass_port(packet_bypassed_udpport_set_buffer,
			count, packet_bypassed_udpport_set);
		if ((arg_count == BYPASS_FAILED) || (arg_count == 1
				&& packet_bypassed_udpport_set[0].startport == 0
				&& packet_bypassed_udpport_set[0].endport ==0)) {
			udp_bypass_port_num = 0;
			kfree(packet_bypassed_udpport_set);
			packet_bypassed_udpport_set = NULL;
		}
	}
	kfree(packet_bypassed_udpport_set_buffer);
	return count;
}
#endif /* CONFIG_FP_BYPASS_PACKET */

void rtl_addConnCache(struct nf_conn *ct, struct sk_buff *skb)
{
	int assured;
	int create_conn;
	struct iphdr *iph;
	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_URL_PATCH)
	enum NP_PROTOCOL protocol;
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_URL_PATCH */
	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	rtl_fp_napt_entry rtlFpNaptEntry;
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	if (nfct_help(ct))
		return;

	iph = ip_hdr(skb);
	if (iph->protocol == IPPROTO_TCP) {
		assured = ((ct->proto.tcp.state == TCP_CONNTRACK_ESTABLISHED) &&
			(test_bit(IPS_DST_NAT_DONE_BIT, &ct->status) ||
				test_bit(IPS_SRC_NAT_DONE_BIT, &ct->status)));

		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_URL_PATCH)
		protocol = NP_TCP;
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_URL_PATCH */
	} else if (iph->protocol == IPPROTO_UDP) {
		assured = (test_bit(IPS_DST_NAT_DONE_BIT, &ct->status) ||
			test_bit(IPS_SRC_NAT_DONE_BIT, &ct->status));

		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_URL_PATCH)
		protocol = NP_UDP;
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_URL_PATCH */
	}
	#if defined(CONFIG_RTL_FAST_GRE)
	else if (iph->protocol == IPPROTO_GRE) {
		assured = (test_bit(IPS_DST_NAT_DONE_BIT, &ct->status) ||
			test_bit(IPS_SRC_NAT_DONE_BIT, &ct->status));

		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_URL_PATCH)
		protocol = NP_GRE;
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_URL_PATCH */
	}
	#endif /* CONFIG_RTL_FAST_GRE */
	else {
		return;
	}

	#if defined(CONFIG_RTL_URL_PATCH)
	if ((protocol == NP_TCP)
		&& (ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) ==URL_PROTO_PORT) && assured)
		assured = 0;
	#endif /* CONFIG_RTL_URL_PATCH */
	#ifdef CONFIG_FP_BYPASS_PACKET
	unsigned int count = 0;

	hw_nat_add = 1;		/*ensure hw_nat_add = 1 */
	if (protocol == NP_TCP) {
		count = 0;
		for (; count < tcp_bypass_port_num; count++) {
			if (ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) <=packet_bypassed_tcpport_set[count].endport
				&& (ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) >=packet_bypassed_tcpport_set[count].startport)) {
				hw_nat_add = 0;
				break;
			} else
				hw_nat_add = 1;
		}
	} else if (protocol == NP_UDP) {
		count = 0;
		for (; count < udp_bypass_port_num; count++) {
			if (ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) <=packet_bypassed_tcpport_set[count].endport
				&& (ntohs(ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all) >=packet_bypassed_tcpport_set[count].startport)) {
				hw_nat_add = 0;
				break;
			} else
				hw_nat_add = 1;
		}
	}
	#endif /* CONFIG_FP_BYPASS_PACKET */
	if (!assured) {
		create_conn = rtl_addConnCheck(ct, iph, skb);
	} else
		create_conn = 0;

	#if defined(CONFIG_RTL_HW_NAT_BYPASS_PKT)
	if (ct->count++ < RTL_HW_NAT_BYPASS_PKT_NUM) {
		assured = 0;
		create_conn = 0;
	}
	#endif /* CONFIG_RTL_HW_NAT_BYPASS_PKT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	/* 1.add "!(ct->helper)" to fix ftp-cmd type packet
	 * 2.add identify case LAN->WAN(AB->BC) or WAN->LAN(BC->AB)
	 * 3.add !(ct->nat.info.helper) for best ALG avoid
	 */
	if ((fast_nat_fw) && (assured || create_conn)) {
		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip
			== ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip) {
			/*case BC->AB */
			/* wan->lan */

			rtlFpNaptEntry.protocol = protocol;
			rtlFpNaptEntry.intIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
			rtlFpNaptEntry.intPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
			rtlFpNaptEntry.extIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
			rtlFpNaptEntry.extPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;
			rtlFpNaptEntry.remIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
			rtlFpNaptEntry.remPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;

			#ifdef CONFIG_FAST_PATH_MODULE
			if (FastPath_hook6 != NULL) {
				FastPath_hook6(&rtlFpNaptEntry,
					skb, ct, NP_NONE);
			}
			#else
			rtk_addNaptConnection(&rtlFpNaptEntry,
				(void *)skb, (void *)ct, NP_NONE);
			#endif /* CONFIG_FAST_PATH_MODULE */
		} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip
			== ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip) {
			/*case AB->BC */
			/* lan->wan */

			rtlFpNaptEntry.protocol = protocol;
			rtlFpNaptEntry.intIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
			rtlFpNaptEntry.intPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
			rtlFpNaptEntry.extIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
			rtlFpNaptEntry.extPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
			rtlFpNaptEntry.remIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
			rtlFpNaptEntry.remPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;

			#ifdef CONFIG_FAST_PATH_MODULE
			if (FastPath_hook6 != NULL) {
				FastPath_hook6(&rtlFpNaptEntry,
					skb, ct, NP_NONE);
			}
			#else
			rtk_addNaptConnection(&rtlFpNaptEntry,
				skb, ct, NP_NONE);
			#endif /* CONFIG_FAST_PATH_MODULE */
		}
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	#if defined(CONFIG_RTL_FAST_GRE)
	if (protocol == NP_GRE)
		return;
	#endif /* CONFIG_RTL_FAST_GRE */
	#if defined(CONFIG_RTL_HARDWARE_NAT)
	/*2007-12-19 */
	#if !defined(CONFIG_RTL865x_TCPFLOW_NONE_STATUS_CHECK)
	if (assured || create_conn)
	#endif /* !CONFIG_RTL865x_TCPFLOW_NONE_STATUS_CHECK */
	{
		#ifdef CONFIG_FP_BYPASS_PACKET
		if (hw_nat_add) {
		#endif /* CONFIG_FP_BYPASS_PACKET */
		#if defined(CONFIG_RTL_AVOID_ADDING_WLAN_PKT_TO_HW_NAT) || defined (CONFIG_RTL_EXT_PORT_SUPPORT)
		#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
		if (extPortEnabled) {
			rtl865x_handle_nat(ct, 1, skb);
		} else
		#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */
		{
		if (rtl_checkLanIp(ct) == SUCCESS) {
			#if defined(CONFIG_RTL_AVOID_ADDING_WAN_SUBNET_PKT_TO_HW_NAT)
			if (rtl_checkWanIp(ct, skb->dev->name) ==SUCCESS)
			#endif /* CONFIG_RTL_AVOID_ADDING_WAN_SUBNET_PKT_TO_HW_NAT */
				rtl865x_handle_nat(ct, 1, skb);
		}
		}
		#else
		#if defined(CONFIG_RTL_AVOID_ADDING_WAN_SUBNET_PKT_TO_HW_NAT)
		if (rtl_checkWanIp(ct, skb->dev->name) == SUCCESS)
		#endif /* CONFIG_RTL_AVOID_ADDING_WAN_SUBNET_PKT_TO_HW_NAT */
			rtl865x_handle_nat(ct, 1, skb);
		#ifdef CONFIG_FP_BYPASS_PACKET
		}
		#endif /* CONFIG_FP_BYPASS_PACKET */
		#endif /* CONFIG_RTL_AVOID_ADDING_WLAN_PKT_TO_HW_NAT || CONFIG_RTL_EXT_PORT_SUPPORT */

	}
	#endif /* CONFIG_RTL_HARDWARE_NAT */
}

#endif /* defined(IMPROVE_QOS) */

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
#if !defined(IMPROVE_QOS) || defined(CONFIG_RTL_ROUTER_FAST_PATH)
/*
 * ### for iperf application test ###
 * the behavior of iperf UDP test is LAN PC (client) will burst UDP from LAN to WAN (by one way),
 * WAN PC (server) will only send one UDP packet (statistics) at the end of test.
 * so the fastpath or hardware NAT will create link at the end of test.
 *
 * the purpose for adding the following code is to create fastpath or hardware NAT link
 * when we only get one packet from LAN to WAN in UDP case.
 */
static int32 rtl_fpAddConnCheck(struct nf_conn *ct, struct iphdr *iph,
	struct sk_buff *skb)
{
	extern unsigned int _br0_ip;
	extern unsigned int _br0_mask;
	struct tcphdr *tcph;
	uint32 sip, dip;
	int32 create_conn;

	sip = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
	dip = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
	create_conn = FALSE;

	if ((ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip
		== ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip)
	#if defined(UNNUMBER_IP)
		&& (is_unnumber_ip(dip) == FALSE)
	#endif /* UNNUMBER_IP */
		) {
		/* lan -> wan */
		if (iph->protocol == IPPROTO_UDP &&
			(sip != _br0_ip) &&
			((((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
			((dip & _br0_mask) != (_br0_ip & _br0_mask))) ||
			(((sip & _br0_mask) != (_br0_ip & _br0_mask)) &&
			((dip & _br0_mask) == (_br0_ip & _br0_mask)))) &&
			(!IS_CLASSD_ADDR(ntohl(dip))) &&
			((routerTypeFlag == 1) || (sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)))
			) {
			create_conn = TRUE;
			/* copied from last 2 line of this function */
			set_bit(IPS_SEEN_REPLY_BIT, &ct->status);
			/* next 3 lines are copied from udp_packet() in ip_conntrack_proto_udp.c */
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
			nf_ct_refresh(ct, skb, udp_get_timeouts_by_state(UDP_CT_REPLIED, (void *)ct, 1));
			#else
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
			nf_ct_refresh(ct, skb, udp_get_timeouts_by_state(UDP_CT_REPLIED));
			#else
			nf_ct_refresh(ct, skb, nf_ct_udp_timeout_stream);
			#endif /* version >= 3.2.0 */
			#endif /* version >= 3.10.0 */
			/* Also, more likely to be important, and not a probe */
			set_bit(IPS_ASSURED_BIT, &ct->status);
		} else if (iph->protocol == IPPROTO_TCP) {
			tcph = (void *)iph + iph->ihl * 4;
			if (!tcph->fin && !tcph->syn && !tcph->rst && tcph->psh == 1 &&
				tcph->ack == TRUE &&  (iph->daddr !=_br0_ip) &&
				((((sip & _br0_mask) == (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) != (_br0_ip & _br0_mask))) ||
				(((sip & _br0_mask) != (_br0_ip & _br0_mask)) &&
				((dip & _br0_mask) == (_br0_ip & _br0_mask)))) &&
				((routerTypeFlag == 1) || (sip != (ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip)))
				) {
				if (smart_count == 0) {
					smart_count_start_timer = jiffies + HZ;
				}

				if (time_after(jiffies, smart_count_start_timer)) {
					smart_count_start_timer = jiffies + HZ;
					smart_count = 0;
				}

				smart_count++;
				if (smart_count > 810) {
					create_conn = TRUE;
				}
			}
		} else {
			return FALSE;
		}

		#if defined(UNNUMBER_IP)
		if ((!create_conn) && (is_unnumber_ip(sip) == TRUE))) {
			create_conn = TRUE;
		}
		#endif /* UNNUMBER_IP */
	}

	return create_conn;
}

void rtl_fpAddConnCache(struct nf_conn *ct, struct sk_buff *skb)
{
	int assured;
	int create_conn;
	struct iphdr *iph;
	enum NP_PROTOCOL protocol;
	rtl_fp_napt_entry rtlFpNaptEntry;

	if (nfct_help(ct))
		return;

	iph = ip_hdr(skb);
	create_conn = rtl_fpAddConnCheck(ct, iph, skb);
	assured = test_bit(IPS_ASSURED_BIT, &ct->status);

	/* 1.add "!(ct->helper)" to fix ftp-cmd type packet
	 * 2.add identify case LAN->WAN(AB->BC) or WAN->LAN(BC->AB)
	 * 3.add !(ct->nat.info.helper) for best ALG avoid
	 */

	if ((assured) || (TRUE == create_conn)) {
		if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum ==IPPROTO_TCP) {
			protocol = NP_TCP;
		} else if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum == IPPROTO_UDP) {
			protocol = NP_UDP;
		}

		#if !defined(IMPROVE_QOS)
		if (((!rtl_isRouterType(ct)) && rtl_isNatTypeWantoLan(ct))
			|| rtl_isRouterTypeWantoLan(ct))
		#else
		if (rtl_isRouterTypeWantoLan(ct))
		#endif /* !IMPROVE_QOS */
		{
			/*case BC->AB */
			/* wan->lan */

			rtlFpNaptEntry.protocol = protocol;
			rtlFpNaptEntry.intIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
			rtlFpNaptEntry.intPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
			rtlFpNaptEntry.extIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
			rtlFpNaptEntry.extPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;
			rtlFpNaptEntry.remIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
			rtlFpNaptEntry.remPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;

			#ifdef CONFIG_FAST_PATH_MODULE
			if (FastPath_hook6 != NULL) {
				FastPath_hook6(&rtlFpNaptEntry,
					#if defined(IMPROVE_QOS)
					skb, ct,
					#endif /* IMPROVE_QOS */
					NP_NONE);
			}
			#else
			rtk_addNaptConnection(&rtlFpNaptEntry,
				#if defined(IMPROVE_QOS)
				(void *)skb, (void *)ct,
				#endif /* IMPROVE_QOS */
				NP_NONE);
			#endif /* CONFIG_FAST_PATH_MODULE */
		}
		#if !defined(IMPROVE_QOS)
		else if (((!rtl_isRouterType(ct)) && rtl_isNatTypeLantoWan(ct))
			|| rtl_isRouterTypeLantoWan(ct))
		#else
		else if (rtl_isRouterTypeLantoWan(ct))
		#endif /* !IMPROVE_QOS */
		{
			/*case AB->BC */
			/* lan->wan */

			rtlFpNaptEntry.protocol = protocol;
			rtlFpNaptEntry.intIp =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
			rtlFpNaptEntry.intPort =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
			rtlFpNaptEntry.extIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
			rtlFpNaptEntry.extPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
			rtlFpNaptEntry.remIp =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
			rtlFpNaptEntry.remPort =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;

			#ifdef CONFIG_FAST_PATH_MODULE
			if (FastPath_hook6 != NULL) {
				FastPath_hook6(&rtlFpNaptEntry,
					#if defined(IMPROVE_QOS)
					skb, ct,
					#endif /* IMPROVE_QOS */
					NP_NONE);
			}
			#else
			rtk_addNaptConnection(&rtlFpNaptEntry,
				#if defined(IMPROVE_QOS)
				skb, ct,
				#endif /* IMPROVE_QOS */
				NP_NONE);
			#endif /* CONFIG_FAST_PATH_MODULE */
		}
	}

}
#endif /* !IMPROVE_QOS || CONFIG_RTL_ROUTER_FAST_PATH */
#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT */

#ifdef CONFIG_RTL_FAST_IPV6
#define CON_SESSION_SINGLE_DIRECTION 1
#ifdef CON_SESSION_SINGLE_DIRECTION
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
extern int ipv6_chk_prefix(const struct in6_addr *addr, struct net_device *dev);
extern int ipv6_chk_addr(struct net *net, const struct in6_addr *addr,
	const struct net_device *dev, int strict);
#else
extern int ipv6_chk_prefix(struct in6_addr *addr, struct net_device *dev);
extern int ipv6_chk_addr(struct net *net, struct in6_addr *addr,
	struct net_device *dev, int strict);
#endif /* version >= 3.10.0 */
#endif /* CON_SESSION_SINGLE_DIRECTION */
static inline int32 rtl_addV6ConnCheck(struct nf_conn *ct, struct ipv6hdr *iph, struct tcphdr *tcph,
	struct sk_buff *skb)
{
	#ifdef CON_SESSION_SINGLE_DIRECTION
	int onlink = 0;
	int addr_chk = 0;
	int create_conn = 0;
	struct in6_addr s6addr;
	struct in6_addr d6addr;
	struct net_device *dev = skb->dev;
	struct inet6_ifaddr *addr_list = NULL;

	//struct inet6_dev *idev = in6_dev_get(dev);
	#endif /* CON_SESSION_SINGLE_DIRECTION */

	if (iph->nexthdr == IPPROTO_TCP) {
		if (!tcph->fin && !tcph->syn && !tcph->rst && tcph->ack == 1) {
			enum ip_conntrack_info ctinfo;

			ctinfo = (enum ip_conntrack_info)(skb->nfctinfo);
			return 1;
		}
	} else if (iph->nexthdr == IPPROTO_UDP) {
		enum ip_conntrack_info ctinfo;

		ctinfo = (enum ip_conntrack_info)(skb->nfctinfo);
		if (CTINFO2DIR(ctinfo) == IP_CT_DIR_REPLY) {
			// receive reply udp ptk!
			return 1;
		}
	}
#ifdef CON_SESSION_SINGLE_DIRECTION
	enum ip_conntrack_info ctinfo;

	ctinfo = (enum ip_conntrack_info)(skb->nfctinfo);
	if ((dev != NULL) && (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL)) {
		s6addr = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.in6;
		d6addr = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.in6;
		onlink = ipv6_chk_prefix(&s6addr, dev);	/*2 */
		addr_chk = ipv6_chk_addr(&init_net, &s6addr, dev, 0);	/*1 */
		create_conn = onlink && !addr_chk;
		onlink = ipv6_chk_prefix(&d6addr, dev);	/*3 */
		create_conn = (create_conn && !onlink && !ipv6_addr_is_multicast(&d6addr));	/*4 */
		#ifdef CONFIG_RTL_FAST_IPV6_DEBUG
		panic_printk("[%s:] %d, devname:%s, create_conn:%d\n", __func__,
			__LINE__, dev->name, create_conn);
		#endif /* CONFIG_RTL_FAST_IPV6_DEBUG */
	}
	return create_conn;
#else
	return 0;
#endif /* CON_SESSION_SINGLE_DIRECTION */
}

int rtl_AddV6ConnCache(struct nf_conn *ct, struct sk_buff *skb) {
	int assured = 0;
	int create_conn = 0;
	struct ipv6hdr *iph = ipv6_hdr(skb);
	struct tcphdr *tcph =(struct tcphdr *)(((unsigned char *)iph) +
		sizeof(struct ipv6hdr));

	if (iph->version != 6)
		return 0;

	if (skb->dev == NULL)
		return 0;

	if ((iph->nexthdr != IPPROTO_TCP) && (iph->nexthdr != IPPROTO_UDP))
		return 0;

	create_conn = rtl_addV6ConnCheck(ct, iph, tcph, skb);

	if (create_conn) {
		struct V6_Tuple V6PathEntry;
		enum ip_conntrack_info ctinfo;

		ctinfo = (enum ip_conntrack_info)(skb->nfctinfo);
		memset(&V6PathEntry, 0, sizeof(struct V6_Tuple));
		if (CTINFO2DIR(ctinfo) == IP_CT_DIR_ORIGINAL) {
			V6PathEntry.saddr =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.in6;
			V6PathEntry.daddr =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.in6;
			V6PathEntry.protocol = iph->nexthdr;
			V6PathEntry.sport =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
			V6PathEntry.dport =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;
		} else if (CTINFO2DIR(ctinfo) == IP_CT_DIR_REPLY) {
			V6PathEntry.saddr =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.in6;
			V6PathEntry.daddr =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.in6;
			V6PathEntry.protocol = iph->nexthdr;
			V6PathEntry.sport =ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
			V6PathEntry.dport =ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
		} else
			return 0;

		rtk_addV6Connection(&V6PathEntry, skb, ct);
	}
}

int rtl_DelV6ConnCache(struct nf_conn *ct) {
	struct V6_Tuple V6PathEntry;

	if (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum != IPPROTO_TCP
		&& ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum !=IPPROTO_UDP) {
		return 0;
	}
	//delete original dir
	memset(&V6PathEntry, 0, sizeof(struct V6_Tuple));
	V6PathEntry.saddr = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.in6;
	V6PathEntry.daddr = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.in6;
	V6PathEntry.protocol =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum;
	V6PathEntry.sport = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
	V6PathEntry.dport = ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;
	rtk_delV6Connection(&V6PathEntry);

	//delelte reply dir
	memset(&V6PathEntry, 0, sizeof(struct V6_Tuple));
	V6PathEntry.saddr = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.in6;
	V6PathEntry.daddr = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.in6;
	V6PathEntry.protocol =ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum;
	V6PathEntry.sport = ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
	V6PathEntry.dport = ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
	rtk_delV6Connection(&V6PathEntry);
	return 1;
}
#endif /* CONFIG_RTL_FAST_IPV6 */

#if defined(CONFIG_PROC_FS) && defined(CONFIG_RTL_HARDWARE_NAT)
static struct proc_dir_entry *proc_hw_nat = NULL;
static char gHwNatSetting[16];

//extern unsigned int ldst, lmask, wdst, wmask;

static int hw_nat_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(&gHwNatSetting, buffer, 8)) {
		if ((gHwNatSetting[0] == '-') && (gHwNatSetting[1] == '1')) {	/* hardware NAT disabled, operation mode = gateway */
			gHwNatEnabled = 0;
			rtl865x_nat_reinit();
			rtl865x_reChangeOpMode();
			//rtl8651_setAsicOperationLayer(4);
		} else if (gHwNatSetting[0] == '0') {	/* hardware NAT disabled, operation mode = gateway */
			gHwNatEnabled = 0;
			rtl865x_nat_reinit();
			rtl865x_changeOpMode(GATEWAY_MODE);
			#if !defined(CONFIG_RTK_VLAN_SUPPORT)
			rtl8651_setAsicOperationLayer(4);
			#endif /* !CONFIG_RTK_VLAN_SUPPORT */
		} else if (gHwNatSetting[0] == '1') {	/* hardware NAT enabled, operation mode = gateway */

			rtl865x_changeOpMode(GATEWAY_MODE);
			#if !defined(CONFIG_RTK_VLAN_SUPPORT)
			rtl8651_setAsicOperationLayer(4);
			#endif /* !CONFIG_RTK_VLAN_SUPPORT */
			gHwNatEnabled = 1;
		} else if (gHwNatSetting[0] == '2') {	/* hardware NAT enabled, operation mode = bridge mode */
			rtl865x_changeOpMode(BRIDGE_MODE);
			gHwNatEnabled = 2;
		} else if (gHwNatSetting[0] == '3') {	/* hardware NAT enabled, operation mode = wisp mode */
			rtl865x_changeOpMode(WISP_MODE);
			gHwNatEnabled = 3;
		} else if ((gHwNatSetting[0] == '8') &&
			   ((gHwNatSetting[1] == '5') /* L2TP */ ||
			    (gHwNatSetting[1] == '4') /* PPTP */ )) {
			gHwNatEnabled = 0;
		}
		#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_BATTLENET_ALG)
		else if (gHwNatSetting[0] == '9') {
			get_br0_ip_mask();
			#if defined(RTL_DROP_OUT_OF_BR_SUBNET_FRAME)
			extern int rtl_add_Acl_for_out_of_br_subnet(void);

			rtl_add_Acl_for_out_of_br_subnet();
			#endif /* RTL_DROP_OUT_OF_BR_SUBNET_FRAME */
		} else if (gHwNatSetting[0] == '7') {
			routerTypeFlag = 1;
		}
		#endif /* CONFIG_RTL_HARDWARE_NAT || CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_BATTLENET_ALG */
		#if defined(CONFIG_RTD_1295_HWNAT)
		else if (gHwNatSetting[0] == '6') {
			extern int32 rtl8651_clearAsicNaptTable(void);

			rtl8651_clearAsicNaptTable();
		}
		#endif /* defined(CONFIG_RTD_1295_HWNAT) */

		if ((count > 0) && (count < sizeof(gHwNatSetting))) {
			gHwNatSetting[count] = 0;
		}
		return count;
	}
	return -EFAULT;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static int hw_nat_read_proc(struct seq_file *s, void *v)
{
	seq_printf(s, "(%s) %d\n", gHwNatSetting, gHwNatEnabled);

	return 0;
}

int hw_nat_single_open(struct inode *inode, struct file *file)
{
	return (single_open(file, hw_nat_read_proc, NULL));
}

static ssize_t hw_nat_single_write(struct file *file,
	const char __user *userbuf, size_t count, loff_t *off)
{
	return hw_nat_write_proc(file, userbuf, count, off);
}

struct file_operations hw_nat_proc_fops = {
		.open           = hw_nat_single_open,
		.write          = hw_nat_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};
#else
static int hw_nat_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%s\n", gHwNatSetting);

	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}
#endif /* version >= 3.10.0 */

#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
static struct proc_dir_entry *proc_ext_port = NULL;

static int ext_port_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	unsigned char tmp;
	uint32 oriValue = extPortEnabled;

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(&tmp, buffer, 1)) {
		extPortEnabled = tmp - '0';
		if (extPortEnabled) {
			extPortEnabled = 1;
		} else {
			extPortEnabled = 0;
		}
		if (oriValue && (extPortEnabled == 0)) {
			rtl865x_delExtFDBEntry();
		}
		return count;
	}
	return -EFAULT;
}

int rtl_get_ext_port_enabled(void)
{
	return extPortEnabled;
}

int rtl_set_ext_port_enabled(int value)
{
	extPortEnabled = value;
	return 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
static int ext_port_read_proc(struct seq_file *s, void *v)
{
	seq_printf(s, "%d\n", extPortEnabled);

	return 0;
}

int ext_port_single_open(struct inode *inode, struct file *file)
{
	return (single_open(file, ext_port_read_proc, NULL));
}

static ssize_t ext_port_single_write(struct file *file,
	const char __user *userbuf, size_t count, loff_t *off)
{
	return ext_port_write_proc(file, userbuf, count, off);
}

struct file_operations ext_port_proc_fops = {
		.open           = ext_port_single_open,
	 	.write          = ext_port_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};
#else
static int ext_port_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "%d\n", extPortEnabled);

	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}
#endif /* version >= 3.10.0 */
#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */

#endif /* CONFIG_PROC_FS && CONFIG_RTL_HARDWARE_NAT */

#if defined(CONFIG_RTL_819X_SWCORE) && (defined(CONFIG_PROC_FS) && !defined(CONFIG_RTL_HARDWARE_NAT))
static struct proc_dir_entry *proc_sw_nat = NULL;
static char gSwNatSetting[16];

static int sw_nat_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(&gSwNatSetting, buffer, 8)) {
		if (gSwNatSetting[0] == '0') {	/* operation mode = GATEWAY */
			//SoftNAT_OP_Mode(2);
			rtl865x_changeOpMode(GATEWAY_MODE);
			//rtl8651_setAsicOperationLayer(2);
		} else if (gSwNatSetting[0] == '1') {	/* operation mode = BRIDGE */
			//SoftNAT_OP_Mode(1);
			rtl865x_changeOpMode(BRIDGE_MODE);
			//rtl8651_setAsicOperationLayer(2);
		} else if (gSwNatSetting[0] == '2') {	/* operation mode = WISP */
			rtl865x_changeOpMode(WISP_MODE);
			//rtl8651_setAsicOperationLayer(2);
		}
		#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_WLAN_DOS_FILTER) || defined(CONFIG_RTL_BATTLENET_ALG)
		else if (gSwNatSetting[0] == '9') {
			get_br0_ip_mask();
			#if defined(RTL_DROP_OUT_OF_BR_SUBNET_FRAME)
			extern int rtl_add_Acl_for_out_of_br_subnet(void);

			rtl_add_Acl_for_out_of_br_subnet();
			#endif /* RTL_DROP_OUT_OF_BR_SUBNET_FRAME */
		} else if (gSwNatSetting[0] == '7') {
			routerTypeFlag = 1;
		}
		#endif /* CONFIG_RTL_HARDWARE_NAT || CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_WLAN_DOS_FILTER || CONFIG_RTL_BATTLENET_ALG */

		#ifdef CONFIG_RTL_WLAN_DOS_FILTER
		else if (gSwNatSetting[0] == 'a') {
			extern int wlan_dos_filter_enabled;

			wlan_dos_filter_enabled = 0;
		} else if (gSwNatSetting[0] == 'b') {
			extern int wlan_dos_filter_enabled;

			wlan_dos_filter_enabled = 1;
		}
		#endif /* CONFIG_RTL_WLAN_DOS_FILTER */

		return count;
	}
	return -EFAULT;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static int sw_nat_read_proc(struct seq_file *s, void *v)
{
	seq_printf(s, "%s\n", gSwNatSetting);

	return 0;
}

int sw_nat_single_open(struct inode *inode, struct file *file)
{
	return (single_open(file, sw_nat_read_proc, NULL));
}

static ssize_t sw_nat_single_write(struct file *file,
	const char __user *userbuf, size_t count, loff_t *off)
{
	return sw_nat_write_proc(file, userbuf, count, off);
}

struct file_operations sw_nat_proc_fops = {
		.open           = sw_nat_single_open,
		.write          = sw_nat_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};
#else
static int sw_nat_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data) {
	int len;

	len = sprintf(page, "%s\n", gSwNatSetting);

	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}
#endif /* version >= 3.10.0 */

#endif /* CONFIG_RTL_819X && (CONFIG_PROC_FS && !CONFIG_RTL_HARDWARE_NAT) */

#if defined(CONFIG_RTL_819X_SWCORE)
int32 rtl_nat_init(void)
{
	#if defined(CONFIG_PROC_FS) && defined(CONFIG_RTL_HARDWARE_NAT)
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	proc_hw_nat = proc_create_data("hw_nat", 0, &proc_root,
		&hw_nat_proc_fops, NULL);
	#else
	proc_hw_nat = create_proc_entry("hw_nat", 0, NULL);
	if (proc_hw_nat) {
		proc_hw_nat->read_proc = hw_nat_read_proc;
		proc_hw_nat->write_proc = hw_nat_write_proc;
	}
	#endif /* version >= 3.10.0 */

	#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	proc_ext_port = proc_create_data("ext_port", 0, &proc_root,
		&ext_port_proc_fops, NULL);
	#else
	proc_ext_port = create_proc_entry("ext_port", 0, NULL);
	if (proc_ext_port) {
		proc_ext_port->read_proc = ext_port_read_proc;
		proc_ext_port->write_proc = ext_port_write_proc;
	}
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */

	#endif /* CONFIG_PROC_FS && CONFIG_RTL_HARDWARE_NAT */
	#if defined(CONFIG_PROC_FS) && !defined(CONFIG_RTL_HARDWARE_NAT)
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	proc_sw_nat = proc_create_data("sw_nat", 0, &proc_root,
		&sw_nat_proc_fops, NULL);
	#else
	proc_sw_nat = create_proc_entry("sw_nat", 0, NULL);
	if (proc_sw_nat) {
		proc_sw_nat->read_proc = sw_nat_read_proc;
		proc_sw_nat->write_proc = sw_nat_write_proc;
	}
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_PROC_FS && !CONFIG_RTL_HARDWARE_NAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	gHwNatEnabled = 1;
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return SUCCESS;
}
#endif /* CONFIG_RTL_819X_SWCORE */

#if defined(CONFIG_RTL_LOCAL_PUBLIC) || defined(CONFIG_RTL_MULTIPLE_WAN) || defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN) || (defined(CONFIG_NET_SCHED) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)) || defined(CONFIG_RTD_1295_HWNAT)
extern int rtl865x_curOpMode;

struct net_device *rtl865x_getWanDev(void)
{
	struct net_device *wanDev = NULL;

	if (rtl865x_curOpMode == GATEWAY_MODE) {
		#if defined(CONFIG_RTL_PUBLIC_SSID)
		wanDev = dev_get_by_name(&init_net, RTL_GW_WAN_DEVICE_NAME);
		#else
		//Try ppp0 first
		wanDev = dev_get_by_name(&init_net, RTL_PS_PPP0_DEV_NAME);
		if (wanDev == NULL) {
			//Try eth1 then
			wanDev = dev_get_by_name(&init_net,
				RTL_PS_WAN0_DEV_NAME);
		}
		#endif /* CONFIG_RTL_PUBLIC_SSID */
	} else if (rtl865x_curOpMode == WISP_MODE) {
		#if defined(CONFIG_RTL_PUBLIC_SSID)
		wanDev = dev_get_by_name(&init_net, RTL_WISP_WAN_DEVICE_NAME);
		#else
		wanDev = dev_get_by_name(&init_net, RTL_PS_WLAN0_DEV_NAME);
		#endif /* CONFIG_RTL_PUBLIC_SSID */
	} else if (rtl865x_curOpMode == BRIDGE_MODE) {
		#if defined(CONFIG_RTL_PUBLIC_SSID)
		wanDev = dev_get_by_name(&init_net, RTL_BR_WAN_DEVICE_NAME);
		#else
		wanDev = dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
		#endif /* CONFIG_RTL_PUBLIC_SSID */
	}

	return wanDev;
}

struct net_device *rtl865x_getLanDev(void)
{
	struct net_device *lanDev = NULL;

	lanDev = dev_get_by_name(&init_net, RTL_PS_BR0_DEV_NAME);
	return lanDev;
}
#endif /* CONFIG_RTL_LOCAL_PUBLIC || CONFIG_RTL_MULTIPLE_WAN || CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN || (CONFIG_NET_SCHED && CONFIG_RTL_IPTABLES_FAST_PATH) || CONFIG_RTD_1295_HWNAT */

#if defined(CONFIG_RTL_LOCAL_PUBLIC) || defined(CONFIG_RTL_HW_QOS_SUPPORT)
int rtl865x_attainDevType(unsigned char *devName, unsigned int *isLanDev,
	unsigned int *isWanDev)
{
	*isLanDev = 0;
	*isWanDev = 0;
	if (rtl865x_curOpMode == GATEWAY_MODE) {
		#if defined(CONFIG_RTL_PUBLIC_SSID)
		if (strncmp(devName, RTL_PS_WAN0_DEV_NAME, 4) == 0
			|| strncmp(devName, RTL_GW_WAN_DEVICE_NAME, 3) == 0
			|| rtl865x_from_public_ssid_device(devName)
			)
		#else
		if (strncmp(devName, RTL_PS_WAN0_DEV_NAME, 4) == 0)
		#endif /* CONFIG_RTL_PUBLIC_SSID */
		{
			*isWanDev = 1;
		} else if ((strncmp(devName, RTL_PS_BR0_DEV_NAME, 3) == 0) ||
			(strncmp(devName, RTL_PS_ETH_NAME, 3) == 0) ||
			(strncmp(devName, RTL_PS_WLAN_NAME, 4) == 0)) {
			*isLanDev = 1;
		} else {
			return -1;
		}

	} else if (rtl865x_curOpMode == WISP_MODE) {
		if (strncmp(devName, RTL_PS_WAN0_DEV_NAME, 4) == 0) {
			*isWanDev = 1;
		} else if ((strncmp(devName, RTL_PS_BR0_DEV_NAME, 3) == 0) ||
			(strncmp(devName, RTL_PS_ETH_NAME, 3) == 0)) {
			*isLanDev = 1;
		} else {
			return -1;
		}
	} else if (rtl865x_curOpMode == BRIDGE_MODE) {
		if ((strncmp(devName, RTL_PS_BR0_DEV_NAME, 3) == 0) ||
			(strncmp(devName, RTL_PS_ETH_NAME, 3) == 0) ||
			(strncmp(devName, RTL_PS_WLAN_NAME, 4) == 0)) {
			*isLanDev = 0;
		} else {
			return -1;
		}
	} else {
		return -1;
	}

	return 0;
}
#endif /* CONFIG_RTL_LOCAL_PUBLIC || CONFIG_RTL_HW_QOS_SUPPORT */

#if defined(CONFIG_RTL_LOCAL_PUBLIC)
int rtl865x_localPublicRx(struct sk_buff *skb)
{
	struct rtl865x_pktInfo pktInfo;
	unsigned int rxFromLan, rxFromWan;

	if (rtl865x_localPublicEnabled() == 0) {
		goto end_of_local_public_rx;
	}

	if (rtl865x_attainDevType(skb->dev->name, &rxFromLan, &rxFromWan)) {
		return NET_RX_SUCCESS;
	}

	if (skb->localPublicFlags == 0x1) {
		//local public has done
		return NET_RX_SUCCESS;
	}
	if (rxFromWan) {
		/*rx from ethernet wan */
		/*direction WAN--->LAN */
		//pktInfo.data=skb->mac.raw;
		pktInfo.data = skb_mac_header(skb);
		pktInfo.action = RX_WAN_PACKET;
		rtl865x_checkLocalPublic(&pktInfo);

		if (pktInfo.fromLocalPublic == 1) {
			kfree_skb(skb);
			return NET_RX_DROP;
		} else if (pktInfo.toLocalPublic == 1) {

			skb->pkt_type = PACKET_HOST;
			memcpy(eth_hdr(skb)->h_dest, skb->dev->dev_addr, 6);
		}

	} else if (rxFromLan) {
		/*rx from ethernet lan */
		pktInfo.data = skb_mac_header(skb);
		pktInfo.port = skb->srcPort;
		pktInfo.action = RX_LAN_PACKET;
		strcpy(pktInfo.dev, skb->dev->name);
		rtl865x_checkLocalPublic(&pktInfo);
		if (pktInfo.fromLocalPublic == 1) {
			skb->fromLocalPublic = 1;
			skb->srcLocalPublicIp = pktInfo.srcIp;
			skb->pkt_type = PACKET_HOST;
			memcpy(eth_hdr(skb)->h_dest, skb->dev->dev_addr, 6);
		}

	}
	skb->localPublicFlags = 0x1;

end_of_local_public_rx:
	return NET_RX_SUCCESS;
}

int rtl865x_localPublicTx(struct sk_buff *skb, struct net_device *dev)
{
	unsigned int txToLan, txToWan;

	if (rtl865x_attainDevType(skb->dev->name, &txToLan, &txToWan)) {
		return NET_RX_SUCCESS;
	}

	if (txToWan) {
		if ((skb->fromLocalPublic == 1) && (skb->srcLocalPublicIp != 0)) {
			rtl865x_getLocalPublicMac(skb->srcLocalPublicIp,
				eth_hdr(skb)->h_source);
		}

	} else if (txToLan) {
		/*needn't do anything */

	}
	return NET_RX_SUCCESS;
}

#endif /* CONFIG_RTL_LOCAL_PUBLIC */

int rtl865x_getDevIpAndNetmask(struct net_device *dev, unsigned int *ipAddr,
	unsigned int *netMask)
{
	struct in_device *in_dev;
	struct in_ifaddr *ifap = NULL;

	if ((dev == NULL) || (ipAddr == NULL) || (netMask == NULL)) {
		return FAILED;
	}

	*ipAddr = 0;
	*netMask = 0;

	in_dev = (struct in_device *)(dev->ip_ptr);
	if (in_dev != NULL) {
		for (ifap = in_dev->ifa_list; ifap != NULL;
			ifap = ifap->ifa_next) {
			if (strcmp(dev->name, ifap->ifa_label) == 0) {
				if (strncmp(dev->name, "ppp",
						strlen("ppp")) == 0)
					*ipAddr = ifap->ifa_local;
				else
					*ipAddr = ifap->ifa_address;
				*netMask = ifap->ifa_mask;
				return SUCCESS;
			}
		}

	}

	return FAILED;

}

#if defined(CONFIG_PROC_FS) && defined(CONFIG_NET_SCHED)
#define	QOS_CLASSIFY_INFO_LEN	16
typedef struct {
	/*      classify        */
	unsigned int protocol;
	struct in_addr local_ip;
	struct in_addr remote_ip;
	unsigned short lo_port;
	unsigned short re_port;

	/*      tc      */
	uint32 mark;
	unsigned char prio;
} rtl865x_qos_cache_t;

static struct proc_dir_entry *proc_qos = NULL;

static char *gQosSetting = NULL;

#ifdef CONFIG_FAST_PATH_MODULE
EXPORT_SYMBOL(gQosEnabled);
#endif /* CONFIG_FAST_PATH_MODULE */

static int qos_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	if (gQosSetting == NULL || count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(gQosSetting, buffer, count)) {
		gQosSetting[count - 1] = 0;	/* remove 0x0a */
		if (gQosSetting[0] == '0')
			gQosEnabled = 0;
		#if defined CONFIG_RTL_HW_QOS_SUPPORT
		else if (gQosSetting[0] == '2') {
			rtl865x_qosFlushBandwidth(RTL_PS_BR0_DEV_NAME);
			rtl865x_qosFlushBandwidth(RTL_PS_WAN0_DEV_NAME);
			rtl865x_closeQos(RTL_PS_BR0_DEV_NAME);
			rtl865x_closeQos(RTL_PS_WAN0_DEV_NAME);
		}
		#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
		else
			gQosEnabled = 1;

		#if defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_QOS_PATCH)
		if (gQosEnabled == 1) {
			rtl865x_reinitOutputQueuePatchForQoS(TRUE);
		} else {
			rtl865x_reinitOutputQueuePatchForQoS(FALSE);
		}
		#endif /* CONFIG_RTL_HW_QOS_SUPPORT && CONFIG_RTL_QOS_PATCH */

		#if defined CONFIG_RTL_8367_QOS_SUPPORT
		rtl8367_setFlowControl(gQosEnabled);
		#endif /* CONFIG_RTL_8367_QOS_SUPPORT */

		return count;
	}
	return -EFAULT;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static int qos_read_proc(struct seq_file *s, void *v)
{
	seq_printf(s, "%s\n", gQosSetting);

	return 0;
}

static int qos_single_open(struct inode *inode, struct file *file)
{
	return (single_open(file, qos_read_proc, NULL));
}

static ssize_t qos_single_write(struct file *file, const char __user *userbuf,
	size_t count, loff_t *off)
{
	return qos_write_proc(file, userbuf, count, off);
}

struct file_operations qos_proc_fops = {
	.open           = qos_single_open,
	.write          = qos_single_write,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release        = single_release,
};
#else
static int qos_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data) {
	int len;

	len = sprintf(page, "%s\n", gQosSetting);

	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}
#endif /* version >= 3.10.0 */

#endif /* CONFIG_PROC_FS && CONFIG_NET_SCHED */

#if defined(CONFIG_NET_SCHED)
#define	RTL_QOS_PROC_MAX_LEN		1024
int32 rtl_qos_init(void)
{
	#if defined(CONFIG_PROC_FS)
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	proc_qos = proc_create_data("qos", 0, &proc_root,
				    &qos_proc_fops, NULL);
	#else
	proc_qos = create_proc_entry("qos", 0, NULL);
	if (proc_qos) {
		proc_qos->read_proc = qos_read_proc;
		proc_qos->write_proc = qos_write_proc;
	}
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_PROC_FS */
	gQosSetting = kmalloc(RTL_QOS_PROC_MAX_LEN, GFP_ATOMIC);
	memset(gQosSetting, 0, RTL_QOS_PROC_MAX_LEN);
	gQosEnabled = 0;

	return SUCCESS;
}

int32 rtl_qos_cleanup(void)
{
	kfree(gQosSetting);

	return SUCCESS;
}
#endif

#if defined(CONFIG_RTL_FAST_BRIDGE)
int32 rtl_fb_add_br_entry(skb)
{
	struct net_bridge_fdb_entry *dst;
	const unsigned char *dest = eth_hdr(skb)->h_dest;

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	u16 vid = 0;
	#endif /* version >= 3.10.0 */
	if (!is_multicast_ether_addr(dest)) {
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
		#if defined(CONFIG_BRIDGE_VLAN_FILTERING)
		/*Fix jwj: todo... */
		#else
		vid = 0;
		#endif /* CONFIG_BRIDGE_VLAN_FILTERING */

		dst = __br_fdb_get(skb->dev->br_port->br, dest, vid);
		#else
		dst = __br_fdb_get(skb->dev->br_port->br, dest);
		#endif /* version >= 3.10.0 */
		if (dst != NULL && dst->dst->dev == skb->dev) {
			fast_br_cache_entry entry;

			memset(&entry, 0, sizeof(fast_br_cache_entry));
			memcpy(&entry.mac_addr, dest, 6);
			entry.ageing_timer = jiffies;
			entry.to_dev = skb->dev;
			entry.valid = 1;

			rtl_add_fast_br_entry(&entry);
		}
	}
}
#endif /* CONFIG_RTL_FAST_BRIDGE */

#if defined(FAST_PATH_SPI_ENABLED)
enum tcp_bit_set {
	TCP_SYN_SET,
	TCP_SYNACK_SET,
	TCP_FIN_SET,
	TCP_ACK_SET,
	TCP_RST_SET,
	TCP_NONE_SET,
};

static unsigned int rtl_spi_get_conntrack_index(const struct tcphdr *tcph)
{
	if (tcph->rst)
		return TCP_RST_SET;
	else if (tcph->syn)
		return (tcph->ack ? TCP_SYNACK_SET : TCP_SYN_SET);
	else if (tcph->fin)
		return TCP_FIN_SET;
	else if (tcph->ack)
		return TCP_ACK_SET;
	else
		return TCP_NONE_SET;
}

extern bool tcp_in_window(const struct nf_conn *ct,
			  struct ip_ct_tcp *state,
			  enum ip_conntrack_dir dir,
			  unsigned int index,
			  const struct sk_buff *skb,
			  unsigned int dataoff,
			  const struct tcphdr *tcph,
			  u_int8_t pf);

static inline struct nf_conn *
rtl_resolve_normal_ct(struct net *net,
		      struct sk_buff *skb,
		      unsigned int dataoff,
		      u_int16_t l3num,
		      u_int8_t protonum,
		      struct nf_conntrack_l3proto *l3proto,
		      struct nf_conntrack_l4proto *l4proto,
		      int *set_reply,
		      enum ip_conntrack_info *ctinfo)
{
	struct nf_conntrack_tuple tuple;
	struct nf_conntrack_tuple_hash *h;
	struct nf_conn *ct;

	if (!nf_ct_get_tuple(skb, skb_network_offset(skb),
			     dataoff, l3num, protonum, &tuple, l3proto,
			     l4proto)) {
		pr_debug("resolve_normal_ct: Can't get tuple\n");
		return NULL;
	}

	/* look for tuple match */
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	h = nf_conntrack_find_get(net, NF_CT_DEFAULT_ZONE, &tuple);
	#else
	h = nf_conntrack_find_get(net, &tuple);
	#endif /* version >= 3.10.0 */

	if (!h) {
		return NULL;
	}
	ct = nf_ct_tuplehash_to_ctrack(h);

	/* It exists; we have (non-exclusive) reference. */
	if (NF_CT_DIRECTION(h) == IP_CT_DIR_REPLY) {
		*ctinfo = IP_CT_ESTABLISHED + IP_CT_IS_REPLY;
	} else {
		/* Once we've had two way comms, always ESTABLISHED. */
		if (test_bit(IPS_SEEN_REPLY_BIT, &ct->status)) {
			pr_debug("nf_conntrack_in: normal packet for %p\n", ct);
			*ctinfo = IP_CT_ESTABLISHED;
		} else if (test_bit(IPS_EXPECTED_BIT, &ct->status)) {
			pr_debug("nf_conntrack_in: related packet for %p\n",
				 ct);
			*ctinfo = IP_CT_RELATED;
		} else {
			pr_debug("nf_conntrack_in: new packet for %p\n", ct);
			*ctinfo = IP_CT_NEW;
		}
	}
	skb->nfct = &ct->ct_general;
	skb->nfctinfo = *ctinfo;
	return ct;
}

unsigned int
rtl_nf_conntrack_in(struct net *net, unsigned int dataoff,
	unsigned int hooknum, struct sk_buff *skb)
{
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	struct nf_conntrack_l3proto *l3proto;
	struct nf_conntrack_l4proto *l4proto;
	int set_reply = 0;
	enum ip_conntrack_dir dir;
	const struct tcphdr *th;
	struct tcphdr _tcph;
	unsigned int index;

	/* Previously seen (loopback or untracked)?  Ignore. */
	if (skb->nfct) {
		NF_CT_STAT_INC_ATOMIC(net, ignore);
		return NF_ACCEPT;
	}

	/* rcu_read_lock()ed by nf_hook_slow */
	l3proto = rcu_dereference(nf_ct_l3protos[PF_INET]);

	l4proto = rcu_dereference(nf_ct_protos[PF_INET][IPPROTO_TCP]);

	ct = rtl_resolve_normal_ct(net, skb, dataoff, PF_INET, IPPROTO_TCP,
				   l3proto, l4proto, &set_reply, &ctinfo);

	if (!ct) {
		/* Not valid part of a connection */
		return NF_ACCEPT;
	}

	if (IS_ERR(ct)) {
		/* Too stressed to deal. */
		return NF_DROP;
	}

	NF_CT_ASSERT(skb->nfct);

	th = skb_header_pointer(skb, dataoff, sizeof(_tcph), &_tcph);
	BUG_ON(th == NULL);

	dir = CTINFO2DIR(ctinfo);

	index = rtl_spi_get_conntrack_index(th);

	NF_CT_ASSERT(skb->nfct);

	if (!tcp_in_window(ct, &ct->proto.tcp, dir, index,
			skb, dataoff, th, PF_INET)) {
		return NF_DROP;
	}
	return NF_ACCEPT;

}
#endif /* FAST_PATH_SPI_ENABLED */

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL) || defined(CONFIG_RTL_HARDWARE_NAT)
/*get the chain number of the ipt_entry*/
int get_hookNum(struct ipt_entry *e, unsigned char *base,
	const unsigned int valid_hooks, const unsigned int *hook_entries)
{
	int h;
	unsigned int offset = 0;
	int hook_num = -1;

	offset = (void *)e - (void *)base;
	for (h = 0; h < NF_INET_NUMHOOKS; h++) {
		if (!(valid_hooks & (1 << h)))
			continue;

		if (offset >= hook_entries[h])
			hook_num = h;
	}

	return hook_num;
}
#endif /* CONFIG_RTL_IPTABLES_RULE_2_ACL || CONFIG_RTL_HARDWARE_NAT */

#if defined(CONFIG_RTL_HARDWARE_NAT) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)

int syn_asic_arp(struct neighbour *n, int add)
{
	if (!net_eq(dev_net(n->dev), &init_net)) {
		/* Skip if it is not root network namespace */
		return 0;
	}

	#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
	{
		int rc;
		u32 arp_ip = htonl(*((u32 *)n->primary_key));

		#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
		/*ipv4 lookup in arp_tbl */
		if (n->tbl && (n->tbl->family == AF_INET)
			&& (n->tbl->key_len == 4))
		#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
			rc = add ? rtl865x_addArp(arp_ip,
				(void *)n->ha) : rtl865x_delArp(arp_ip);
	}
	#endif /* CONFIG_RTL_LAYERED_DRIVER_L3 */
	#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	{
		int rc_ipv6;
		inv6_addr_t nd_ip = *((inv6_addr_t *)n->primary_key);
		inv6_addr_t tmp_ip;

		//change ipv6 address to big endian
		htonl_array((u32 *)&nd_ip.v6_addr32[0],
			(u32 *)&tmp_ip.v6_addr32[0], 4);
		/*ipv6 lookup in nd_tbl */
		if (n->tbl && (n->tbl->family == AF_INET6)
			&& (n->tbl->key_len == sizeof(inv6_addr_t)))
			rc_ipv6 =add ? rtl8198c_addIpv6Arp(tmp_ip,
				(void *)n->ha) : rtl8198c_delIpv6Arp(tmp_ip);
	}

	#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	return 0;
}
#endif /* CONFIG_RTL_HARDWARE_NAT || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

#if defined(CONFIG_RTL_HARDWARE_NAT)
#if defined(CONFIG_RTL_MULTIPLE_WAN) || defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN)
int rtl_get_ps_arp_mapping(u32 ip, void *arp_entry_tmp)
{
	rtl865x_arpMapping_entry_t *arp_entry;
	struct neighbour *dst_n;
	struct net_device *wanDev;

	wanDev = rtl865x_getWanDev();

	if (wanDev == NULL)
		return FAILED;

	arp_entry = (rtl865x_arpMapping_entry_t *)arp_entry_tmp;

	dst_n = neigh_lookup(&arp_tbl, &ip, wanDev);
	dev_put(wanDev);
	if (dst_n != NULL) {
		arp_entry->ip = ip;
		memcpy(arp_entry->mac.octet, dst_n->ha, ETHER_ADDR_LEN);
		return SUCCESS;
	}

	return FAILED;
}
#endif /* CONFIG_RTL_MULTIPLE_WAN || CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN */

rtl_masq_if rtl_masq_info[RTL_MULTIPLE_WAN_NUM];

rtl_masq_if *rtl_get_masq_info_by_devName(const char *name)
{
	int i;

	if (name == NULL)
		return NULL;

	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		if (rtl_masq_info[i].valid == 1
			&& strcmp(rtl_masq_info[i].ifName, name) == 0)
			return &rtl_masq_info[i];
	}

	return NULL;
}

int rtl_add_masq_info(const char *name, int ipAddr)
{
	int i;

	if (strlen(name) >= IFNAMSIZ || ipAddr == 0)
		return FAILED;

	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		if (rtl_masq_info[i].valid == 0)
			break;
	}

	if (i == RTL_MULTIPLE_WAN_NUM)
		return FAILED;

	rtl_masq_info[i].valid = 1;
	rtl_masq_info[i].ipAddr = ipAddr;
	memcpy(rtl_masq_info[i].ifName, name, strlen(name));
	rtl_masq_info[i].ifName[strlen(name)] = '\0';
	return SUCCESS;
}

int rtl_init_masq_info(void)
{
	int i;

	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		memset(&rtl_masq_info[i], 0, sizeof(rtl_masq_if));
	}

	return SUCCESS;
}

/* Performance critical */
static inline struct ipt_entry *get_entry(void *base, unsigned int offset)
{
	return (struct ipt_entry *)(base + offset);
}

#ifdef CONFIG_NETFILTER_DEBUG
#define IP_NF_ASSERT(x)						\
do {								\
	if (!(x))						\
		printk("IP_NF_ASSERT: %s:%s:%u\n",		\
		       __func__, __FILE__, __LINE__);	\
} while(0)
#else
#define IP_NF_ASSERT(x)
#endif /* CONFIG_NETFILTER_DEBUG */

static int rtl_get_masquerade_netif(struct xt_table_info *private,
	struct ipt_entry *masq_entry)
{
	void *table_base;
	struct ipt_entry *e, *back;

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	struct xt_entry_target *t;
	#else
	struct ipt_entry_target *t;
	#endif /* version >= 3.2.0 */
	struct net_device *dev;

	char masq_name[IFNAMSIZ]={'\0'};

	if (masq_entry && masq_entry->ip.outiface[0] != '\0') {
		memcpy(masq_name, masq_entry->ip.outiface, IFNAMSIZ);
		//return 0;
	} else {
		#if defined(CONFIG_RTD_1295_HWNAT)
		smp_read_barrier_depends();
		table_base = private->entries[raw_smp_processor_id()];
		#else /* CONFIG_RTD_1295_HWNAT */
		table_base = private->entries[smp_processor_id()];
		#endif /* CONFIG_RTD_1295_HWNAT */
		e = get_entry(table_base,
			private->hook_entry[NF_INET_POST_ROUTING]);
		back = get_entry(table_base,
			private->underflow[NF_INET_POST_ROUTING]);

		//clear masq_name;
		memset(masq_name, 0, IFNAMSIZ);
		while (e) {

			if (e == masq_entry)
				break;

			//record the entry's outif name
			if (e->ip.outiface[0] != '\0')
				memcpy(masq_name, e->ip.outiface, IFNAMSIZ);

			//target
			t = ipt_get_target(e);
			IP_NF_ASSERT(t->u.kernel.target);

			//if error target
			if (strcmp(t->u.kernel.target->name, "ERROR") == 0) {
				memset(masq_name, 0, IFNAMSIZ);
				break;
			}

			/* Standard target? */
			if (!t->u.kernel.target->target) {
				int v;

				#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
				v = ((struct xt_standard_target *)t)->verdict;
				#else
				v = ((struct ipt_standard_target *)t)->verdict;
				#endif /* version >= 3.2.0 */

				if (v < 0) {
					#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
					if (v == XT_RETURN)
					#else
					if (v == IPT_RETURN)
					#endif /* version >= 3.2.0 */
					{
						e = back;
						back = get_entry(table_base,
							back->comefrom);
					} else {
						e = (void *)e + e->next_offset;
					}

					continue;
				}
				//jump ?
				if (table_base + v != (void *)e + e->next_offset
					&& !(e->ip.flags & IPT_F_GOTO)) {
					/* Save old back ptr in next entry */
					struct ipt_entry *next
						= (void *)e + e->next_offset;
					next->comefrom
						= (void *)back - table_base;
					/* set back pointer to next entry */
					back = next;
				}
				e = get_entry(table_base, v);
				continue;
			}

			/*user define target? */
			e = (void *)e + e->next_offset;
		}

	}

	if (masq_name[0] != '\0') {
		struct in_ifaddr *ina;

		dev = __dev_get_by_name(&init_net, masq_name);
		if ((dev) && (dev->ip_ptr)) {

			ina = (struct in_ifaddr *)(((struct in_device *)(dev->ip_ptr))->ifa_list);
			if (ina != NULL) {
				rtl_add_masq_info(masq_name, ina->ifa_local);
			}
		}
	}

	return 0;
}

static int rtl_check_for_masquerade_entry(struct ipt_entry *e,
	unsigned char *base,
	const char *name,
	unsigned int size,
	const unsigned int valid_hooks,
	const unsigned int *hook_entries, struct xt_table_info *private)
{
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	struct xt_entry_target *t;
	#else
	struct ipt_entry_target *t;
	#endif /* version >= 3.2.0 */
	unsigned int hook;
	int ret = 0;

	t = ipt_get_target(e);
	if (!t) {
		goto err;
	}

	hook = get_hookNum(e, base, valid_hooks, hook_entries);
	if ((hook == NF_INET_POST_ROUTING) &&
		((strcmp(t->u.kernel.target->name, "MASQUERADE") == 0))) {
		rtl_get_masquerade_netif(private, e);
	}
err:
	return ret;
}

int rtl_check_for_extern_ip(const char *name,
	unsigned int valid_hooks,
	struct xt_table_info *newinfo,
	void *entry0,
	unsigned int size,
	unsigned int number,
	const unsigned int *hook_entries, const unsigned int *underflows)
{
	int i;

	#if !defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN)
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	int ret;
	struct ipt_entry *iter;

	xt_entry_foreach(iter, entry0, size) {
		ret = rtl_check_for_masquerade_entry(iter, entry0, name, size,
			valid_hooks, hook_entries, newinfo);
	}
	#else
	IPT_ENTRY_ITERATE(entry0, size, rtl_check_for_masquerade_entry, entry0,
		name, size, valid_hooks, hook_entries, newinfo);
	#endif /* version >= 3.2.0 */
	#endif /* !CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN */
	//found masq entry
	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		if (rtl_masq_info[i].valid == 1 && rtl_masq_info[i].ipAddr) {
			rtl865x_addIp(0, htonl(rtl_masq_info[i].ipAddr),
				IP_TYPE_NAPT);
		}
	}

	return 0;
}

int rtl_flush_extern_ip(void)
{
	int i;

	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		if (rtl_masq_info[i].valid == 1) {
			//delete....
			rtl865x_delIp(htonl(rtl_masq_info[i].ipAddr));
		}
	}

	return SUCCESS;
}

#if defined(CONFIG_RTL_HARDWARE_NAT)
int rtl819x_retore_hw_ip(void) {
	int i;

	//found masq entry
	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		if (rtl_masq_info[i].valid == 1 && rtl_masq_info[i].ipAddr) {
			rtl865x_addIp(0, htonl(rtl_masq_info[i].ipAddr),
				IP_TYPE_NAPT);
		}
	}

	return SUCCESS;
}
#endif /* CONFIG_RTL_HARDWARE_NAT */

#if defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN)
void rtl_sync_extIp_to_masq_info(const char *name,
	unsigned int valid_hooks,
	struct xt_table_info *newinfo,
	void *entry0,
	unsigned int size,
	unsigned int number,
	const unsigned int *hook_entries, const unsigned int *underflows)
{
	rtl_init_masq_info();

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	int ret;
	struct ipt_entry *iter;

	xt_entry_foreach(iter, entry0, size) {
		ret = rtl_check_for_masquerade_entry(iter, entry0, name, size,
			valid_hooks, hook_entries, newinfo);
	}
	#else
	IPT_ENTRY_ITERATE(entry0, size, rtl_check_for_masquerade_entry, entry0,
		name, size, valid_hooks, hook_entries, newinfo);
	#endif /* version >= 3.2.0 */
}

int rtl_check_extIp_exist_in_ps(uint32 extIp)
{
	int i;

	for (i = 0; i < RTL_MULTIPLE_WAN_NUM; i++) {
		if ((rtl_masq_info[i].valid == 1)
			&& (rtl_masq_info[i].ipAddr == extIp)) {
			return SUCCESS;
		}
	}

	return FAILED;
}
#endif /* CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN */

int32 rtl_update_ip_tables(char *name, unsigned long event,
	struct in_ifaddr *ina)
{
	rtl_masq_if *entry;

	/*2007-12-19 */
	#ifdef CONFIG_HARDWARE_NAT_DEBUG
	/*2007-12-19 */
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	#endif /* CONFIG_HARDWARE_NAT_DEBUG */
	if (ina == NULL)
		return SUCCESS;
	#if defined(CONFIG_RTD_1295_HWNAT)
	if (event > NETDEV_DOWN)
		return SUCCESS;
	#endif /* CONFIG_RTD_1295_HWNAT */

	entry = rtl_get_masq_info_by_devName(name);
	if (entry != NULL) {
		if (event == NETDEV_UP) {
			rtl865x_addIp(0, htonl(ina->ifa_local), IP_TYPE_NAPT);
			//update the ip address
			entry->ipAddr = ina->ifa_local;
		} else if (event == NETDEV_DOWN) {
			if (rtl865x_delIp(htonl(ina->ifa_local)) == SUCCESS) {
				rtl865x_nat_init();
			}
		}
	}

	return SUCCESS;
}

int32 rtl_fn_insert(struct fib_table *tb, struct fib_config *cfg,
	struct fib_info *fi)
{
	int rc;
	unsigned int ipDst, ipMask, ipGw;
	unsigned int srcIp, srcMask;
	struct net_device *netif;
	char *dev_t;

	/*2007-12-19 */
	if ((tb->tb_id == RT_TABLE_MAIN) && (gHwNatEnabled == 1)) {
		if (cfg->fc_oif) {
			netif = __dev_get_by_index(&init_net, cfg->fc_oif);
		} else {
			netif = NULL;
		}

		dev_t = (netif) ? netif->name : NULL;
		ipDst = cfg->fc_dst;
		ipMask = inet_make_mask(cfg->fc_dst_len);
		ipGw = cfg->fc_gw;

		if (!net_eq(cfg->fc_nlinfo.nl_net, &init_net)) {
			/* Skip if it is not root network namespace */
			return SUCCESS;
		}

		rtl865x_getDevIpAndNetmask(netif, &srcIp, &srcMask);
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
		if (!ipDst || (!ipv4_is_multicast(ipDst)
				&& !ipv4_is_loopback(ipDst)
				&& (ipDst != 0xffffffff)))
		#else
		if (!ipDst || (!MULTICAST(ipDst) && !LOOPBACK(ipDst)
				&& (ipDst != 0xffffffff)))
		#endif /* version >= 3.2.0 */
		{
			#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
			rc = rtl865x_addRoute(htonl(ipDst), htonl(ipMask),
				htonl(ipGw), dev_t, srcIp);
			#endif /* CONFIG_RTL_LAYERED_DRIVER_L3 */
			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			/*2007-12-19 */
			printk("%s:%d:(%s): dst:%u.%u.%u.%u/%u, gw:%u.%u.%u.%u, dev: %s, errno=%d\n", __FUNCTION__, __LINE__, "add_rt", NIPQUAD(ipDst), cfg->fc_dst_len, NIPQUAD(ipGw), dev_t ? dev_t : "null", rc);
			#endif /* CONFIG_HARDWARE_NAT_DEBUG */

		}
	}

	return SUCCESS;
}

int32 rtl_fn_delete(struct fib_table *tb, struct fib_config *cfg)
{
	int rc;
	unsigned int ipDst, ipMask;

	/*2007-12-19 */
	if (tb->tb_id == RT_TABLE_MAIN) {
		ipDst = cfg->fc_dst;
		ipMask = inet_make_mask(cfg->fc_dst_len);

		if (!net_eq(cfg->fc_nlinfo.nl_net, &init_net)) {
			/* Skip if it is not root network namespace */
			return SUCCESS;
		}

		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
		if (!ipDst || (!ipv4_is_multicast(ipDst)
				&& !ipv4_is_loopback(ipDst)
				&& (ipDst != 0xffffffff)))
		#else
		if (!ipDst || (!MULTICAST(ipDst) && !LOOPBACK(ipDst)
				&& (ipDst != 0xffffffff)))
		#endif /* version >= 3.2.0 */
		{
			rc = 0;
		#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
			rc = rtl865x_delRoute(htonl(ipDst), htonl(ipMask));
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L3 */
		}
	}

	return SUCCESS;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
int32 rtl_fib_flush(u32 tb_id, u32 fn_key, u32 ip_mask) {
	int rc;
	unsigned int ipDst, ipMask;

	/*2007-12-19 */
	if (tb_id == RT_TABLE_MAIN) {
		ipDst = fn_key;
		ipMask = ip_mask;

		if (!ipDst || (!ipv4_is_multicast(ipDst)
				&& !ipv4_is_loopback(ipDst)
				&& (ipDst != 0xffffffff))) {
			rc = 0;
			#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
			rc = rtl865x_delRoute(htonl(ipDst), htonl(ipMask));
			#endif /* CONFIG_RTL_LAYERED_DRIVER_L3 */
		}
	}

	return SUCCESS;
}
#endif /* version >= 3.2.0 */

int32 rtl_fn_flush(int fz_order, int idx, u32 tb_id, u32 fn_key)
{
	#if LINUX_VERSION_CODE < KERNEL_VERSION(3, 2, 0)
	int rc;
	unsigned int ipDst, ipMask;

	/*2007-12-19 */
	if (tb_id == RT_TABLE_MAIN) {
		ipDst = fn_key;
		ipMask = inet_make_mask(fz_order);

		if (!ipDst || (!MULTICAST(ipDst) && !LOOPBACK(ipDst)
				&& (ipDst != 0xffffffff))) {
			rc = 0;
			#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
			rc = rtl865x_delRoute(htonl(ipDst), htonl(ipMask));
			#endif /* CONFIG_RTL_LAYERED_DRIVER_L3 */
		}
	}
	#endif /* version >= 3.2.0 */
	return SUCCESS;
}

int32 rtl_ip_vs_conn_expire_check(struct ip_vs_conn *cp) {
	int elapsed;
	u_int32_t expires;
	rtl865x_napt_entry rtl865xNaptEntry;

	if (cp->protocol == IPPROTO_UDP) {
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
		expires =udp_get_timeouts_by_state(UDP_CT_UNREPLIED, (void *)cp,
			0);
		#else
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 4, 0)
		expires = udp_get_timeouts_by_state(UDP_CT_UNREPLIED);
		#else
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) && (LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0))
		//TBD
		#else
		expires = nf_ct_udp_timeout;
		#endif /* 3.3.0 > version >= 3.2.0 */
		#endif /* version >= 3.4.0 */
		#endif /* version >= 3.10.0 */
	} else if (cp->protocol == IPPROTO_TCP) {
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)
		expires = tcp_get_timeouts_by_state(cp->state, (void *)cp, 0);	/* does cp->state right here? */
		#else
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0) && (LINUX_VERSION_CODE < KERNEL_VERSION(3, 3, 0))
		//TBD
		#else
		expires = tcp_get_timeouts_by_state(cp->state);	/* does cp->state right here? */
		#endif /* 3.3.0 > version >= 3.2.0 */
		#endif /* version >= 3.10.0 */
	} else {
		return FAILED;
	}

	if (cp->hw_acc) {
		rtl865xNaptEntry.protocol =((cp->protocol == IPPROTO_TCP) ? 1 : 0);
		rtl865xNaptEntry.intIp = cp->daddr.ip;
		rtl865xNaptEntry.intPort = cp->dport;
		rtl865xNaptEntry.extIp = cp->vaddr.ip;
		rtl865xNaptEntry.extPort = cp->vport;
		rtl865xNaptEntry.remIp = cp->caddr.ip;
		rtl865xNaptEntry.remPort = cp->cport;

		elapsed = rtl865x_naptSync(&rtl865xNaptEntry, 0);

		if (elapsed >= 0 && (cp->protocol == IPPROTO_UDP)) {
			cp->timer.expires = jiffies + (expires - elapsed) * HZ;
			add_timer(&cp->timer);
			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			/*2007-12-19*/
			#if defined(CONFIG_RTD_1295_HWNAT)
			pr_err("%s:%d:(%s): expired time = %ld\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
					__FUNCTION__,__LINE__,"poll_nat", cp->timer.expires, (cp->protocol==IPPROTO_TCP)? "tcp": "udp",
					NIPQUAD(cp->daddr), cp->dport, NIPQUAD(cp->caddr), cp->cport,
					NIPQUAD(cp->vaddr), cp->vport
			);
			#else /* CONFIG_RTD_1295_HWNAT */
			printk("%s:%d:(%s): expired time = %d\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
					__FUNCTION__,__LINE__,"poll_nat", timeval, (cp->protocol==IPPROTO_TCP)? "tcp": "udp",
					NIPQUAD(cp->daddr), cp->dport, NIPQUAD(cp->caddr), cp->cport,
					NIPQUAD(cp->vaddr), cp->vport
			);
			#endif /* CONFIG_RTD_1295_HWNAT */
			#endif /* CONFIG_HARDWARE_NAT_DEBUG */

			return FAILED;
		}
	}

	return SUCCESS;
}

int32 rtl_ip_vs_conn_expire_check_delete(struct ip_vs_conn *cp)
{
	rtl865x_napt_entry rtl865xNaptEntry;

	if (cp->hw_acc) {
		int rc;

		rtl865xNaptEntry.protocol =((cp->protocol == IPPROTO_TCP) ? 1 : 0);
		rtl865xNaptEntry.intIp = cp->daddr.ip;
		rtl865xNaptEntry.intPort = cp->dport;
		rtl865xNaptEntry.extIp = cp->vaddr.ip;
		rtl865xNaptEntry.extPort = cp->vport;
		rtl865xNaptEntry.remIp = cp->caddr.ip;
		rtl865xNaptEntry.remPort = cp->cport;

		rc = rtl865x_delNaptConnection(&rtl865xNaptEntry);

		#ifdef CONFIG_HARDWARE_NAT_DEBUG
		/*2007-12-19*/
		printk("%s:%d:(%s): errno=%d\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
			__FUNCTION__,__LINE__,"del_nat", rc, (cp->protocol==IPPROTO_TCP)? "tcp": "udp",
			NIPQUAD(cp->daddr.ip), cp->dport, NIPQUAD(cp->caddr.ip), cp->cport,
			NIPQUAD(cp->vaddr.ip), cp->vport
		);
		#endif /* CONFIG_HARDWARE_NAT_DEBUG */
	}

	return SUCCESS;
}

void rtl_tcp_state_transition_check(struct ip_vs_conn *cp, int direction,
	const struct sk_buff *skb, struct ip_vs_protocol *pp)
{
	#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
	uint32 ret;
	struct net_device *lanDev, *wanDev;
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT */

	#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
	rtl865x_napt_entry rtl865xNaptEntry;
	rtl865x_priority rtl865xPrio;

	#if defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	int i;
	int cnt = 0;
	int first_match = 0;
	int maxDownPriority = 0;
	int maxUpPriority = 0;
	unsigned int maxDecisionBitMap = 0;
	unsigned int downPriority[max_decision_priority];
	unsigned int upPriority[max_decision_priority];
	unsigned int maxDecision[max_decision_priority];
	rtl865x_qos_mark rtl865xQosMark[max_decision_priority];
	#else
	#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
	rtl865x_qos_mark rtl865xQosMark;
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT && CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */

	/*2007-12-19 */
	if (!cp->hw_acc && !cp->app && cp->state == IP_VS_TCP_S_ESTABLISHED)
	{
		int rc;

		rc = 0;

		#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
		if (NULL != skb) {
			rtl865xNaptEntry.protocol = RTL865X_PROTOCOL_TCP;
			rtl865xNaptEntry.intIp = cp->daddr.ip;
			rtl865xNaptEntry.intPort = cp->dport;
			rtl865xNaptEntry.extIp = cp->vaddr.ip;
			rtl865xNaptEntry.extPort = cp->vport;
			rtl865xNaptEntry.remIp = cp->caddr.ip;
			rtl865xNaptEntry.remPort = cp->cport;
			#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
			memset(upPriority, 0, sizeof(upPriority));
			memset(downPriority, 0, sizeof(downPriority));
			memset(maxDecision, 0, sizeof(maxDecision));
			memset(rtl865xQosMark, 0, sizeof(rtl865xQosMark));

			ret = rtl_qosGetSkbMarkByNaptEntry(&rtl865xNaptEntry,
				rtl865xQosMark, skb);
			lanDev = rtl865x_getLanDev();
			wanDev = rtl865x_getWanDev();
			for (i = port_decision_priority;
				i < max_decision_priority; i++) {
				if (rtl865xQosMark[i].downlinkMark) {
					downPriority[i] =rtl_qosGetPriorityByMark(lanDev->name,
						rtl865xQosMark[i].downlinkMark);
				}

				if (rtl865xQosMark[i].uplinkMark) {
					upPriority[i] =rtl_qosGetPriorityByMark(wanDev->name,
						rtl865xQosMark[i].uplinkMark);
				}
			}

			rtl_getMaxDecisionBitMap(&maxDecisionBitMap);
			for (i = 0; i < 5; i++) {
				if (maxDecisionBitMap & (1 << i)) {
					if (++cnt == 1)
						first_match = i;
					maxDecision[i] = 1;
				}
			}

			if (cnt == 1) {	//only one decision priority is max, so just use this decision
				rtl865xPrio.downlinkPrio =downPriority[first_match];
				rtl865xPrio.uplinkPrio =upPriority[first_match];
				/*Should I update skb->mark here? */
			} else if (cnt > 1) {	//more than one decision priority are max, use the highest priority
				for (i = 0; i < 5; i++) {
					if ((maxDecision[i] == 1)) {
						if (downPriority[i] > maxDownPriority)
							maxDownPriority =downPriority[i];

						if (upPriority[i] > maxUpPriority)
							maxUpPriority =upPriority[i];
					}
				}
				rtl865xPrio.downlinkPrio = maxDownPriority;
				rtl865xPrio.uplinkPrio = maxUpPriority;
			}
			#else
			rtl865xQosMark.downlinkMark = 0;	//Initial
			rtl865xQosMark.uplinkMark = 0;	//Initial
			ret = rtl_qosGetSkbMarkByNaptEntry(&rtl865xNaptEntry,
				&rtl865xQosMark, skb);

			lanDev = rtl865x_getLanDev();
			wanDev = rtl865x_getWanDev();
			rtl865xPrio.downlinkPrio =rtl_qosGetPriorityByMark(lanDev->name,
				rtl865xQosMark.downlinkMark);
			rtl865xPrio.uplinkPrio =rtl_qosGetPriorityByMark(wanDev->name,
				rtl865xQosMark.uplinkMark);
			#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
			if (lanDev)
				dev_put(lanDev);

			if (wanDev)
				dev_put(wanDev);
		} else
		#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
		{
			rtl865xPrio.downlinkPrio = 0;
			rtl865xPrio.uplinkPrio = 0;
		}

		#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
		rtl865xNaptEntry.protocol = RTL865X_PROTOCOL_TCP;
		rtl865xNaptEntry.intIp = cp->daddr.ip;
		rtl865xNaptEntry.intPort = cp->dport;
		rtl865xNaptEntry.extIp = cp->vaddr.ip;
		rtl865xNaptEntry.extPort = cp->vport;
		rtl865xNaptEntry.remIp = cp->caddr.ip;
		rtl865xNaptEntry.remPort = cp->cport;
		rc = rtl865x_addNaptConnection(&rtl865xNaptEntry, &rtl865xPrio);
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
		if (!rc)
			cp->hw_acc = 1;
		#ifdef CONFIG_HARDWARE_NAT_DEBUG
		/*2007-12-19*/
		printk("%s:%d:(%s): errno=%d\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
				__FUNCTION__,__LINE__,"add_nat", rc, "tcp", NIPQUAD(cp->daddr), cp->dport,
				NIPQUAD(cp->caddr), cp->cport, NIPQUAD(cp->vaddr), cp->vport
		);
		#endif /* CONFIG_HARDWARE_NAT_DEBUG */
	}
}

void rtl_udp_state_transition_check(struct ip_vs_conn *cp, int direction,
	const struct sk_buff *skb, struct ip_vs_protocol *pp)
{
	#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
	uint32 ret;
	struct net_device *lanDev, *wanDev;
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
	#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
	rtl865x_napt_entry rtl865xNaptEntry;
	rtl865x_priority rtl865xPrio;

	#if defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	int i;
	int cnt = 0;
	int first_match = 0;
	int maxDownPriority = 0;
	int maxUpPriority = 0;
	unsigned int maxDecisionBitMap = 0;
	unsigned int downPriority[max_decision_priority];
	unsigned int upPriority[max_decision_priority];
	unsigned int maxDecision[max_decision_priority];
	rtl865x_qos_mark rtl865xQosMark[max_decision_priority];
	#else
	#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
	rtl865x_qos_mark rtl865xQosMark;
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
	#endif /* CONFIG_RTL_HW_QOS_SUPPORT && CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */

	/*2007-12-19 */
	if (!cp->hw_acc && !cp->app) {
		int rc;

		rc = 0;
		#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
		if (NULL != skb) {
			rtl865xNaptEntry.protocol = RTL865X_PROTOCOL_UDP;
			rtl865xNaptEntry.intIp = cp->daddr.ip;
			rtl865xNaptEntry.intPort = cp->dport;
			rtl865xNaptEntry.extIp = cp->vaddr.ip;
			rtl865xNaptEntry.extPort = cp->vport;
			rtl865xNaptEntry.remIp = cp->caddr.ip;
			rtl865xNaptEntry.remPort = cp->cport;
			#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
			memset(upPriority, 0, sizeof(upPriority));
			memset(downPriority, 0, sizeof(downPriority));
			memset(maxDecision, 0, sizeof(maxDecision));
			memset(rtl865xQosMark, 0, sizeof(rtl865xQosMark));

			ret = rtl_qosGetSkbMarkByNaptEntry(&rtl865xNaptEntry,
				rtl865xQosMark, skb);
			lanDev = rtl865x_getLanDev();
			wanDev = rtl865x_getWanDev();
			for (i = port_decision_priority;
				i < max_decision_priority; i++) {
				if (rtl865xQosMark[i].downlinkMark) {
					downPriority[i] =rtl_qosGetPriorityByMark(lanDev->name,
						rtl865xQosMark[i].downlinkMark);
				}

				if (rtl865xQosMark[i].uplinkMark) {
					upPriority[i] =rtl_qosGetPriorityByMark(wanDev->name,
						rtl865xQosMark[i].uplinkMark);
				}
			}

			rtl_getMaxDecisionBitMap(&maxDecisionBitMap);
			for (i = 0; i < 5; i++) {
				if (maxDecisionBitMap & (1 << i)) {
					if ((++cnt) == 1)
						first_match = i;
					maxDecision[i] = 1;
				}
			}

			if (cnt == 1) {	//only one decision priority is max, so just use this decision
				rtl865xPrio.downlinkPrio =downPriority[first_match];
				rtl865xPrio.uplinkPrio =upPriority[first_match];
				/*Should I update skb->mark here? */
			} else if (cnt > 1) {	//more than one decision priority are max, use the highest priority
				for (i = 0; i < 5; i++) {
					if ((maxDecision[i] == 1)) {
						if (downPriority[i] > maxDownPriority)
							maxDownPriority =downPriority[i];

						if (upPriority[i] > maxUpPriority)
							maxUpPriority =upPriority[i];
					}
				}
				rtl865xPrio.downlinkPrio = maxDownPriority;
				rtl865xPrio.uplinkPrio = maxUpPriority;
			}
			#else
			rtl865xQosMark.downlinkMark = 0;	//Initial
			rtl865xQosMark.uplinkMark = 0;	//Initial
			ret = rtl_qosGetSkbMarkByNaptEntry(&rtl865xNaptEntry,
				&rtl865xQosMark, skb);

			lanDev = rtl865x_getLanDev();
			wanDev = rtl865x_getWanDev();
			rtl865xPrio.downlinkPrio =rtl_qosGetPriorityByMark(lanDev->name,
				rtl865xQosMark.downlinkMark);
			rtl865xPrio.uplinkPrio =rtl_qosGetPriorityByMark(wanDev->name,
				rtl865xQosMark.uplinkMark);
			#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
			if (lanDev)
				dev_put(lanDev);

			if (wanDev)
				dev_put(wanDev);
		} else
		#endif /* CONFIG_RTL_HW_QOS_SUPPORT */
		{
			rtl865xPrio.downlinkPrio = 0;
			rtl865xPrio.uplinkPrio = 0;
		}

		#if defined(CONFIG_RTL_LAYERED_DRIVER_L4)
		rtl865xNaptEntry.protocol = RTL865X_PROTOCOL_UDP;
		rtl865xNaptEntry.intIp = cp->daddr.ip;
		rtl865xNaptEntry.intPort = cp->dport;
		rtl865xNaptEntry.extIp = cp->vaddr.ip;
		rtl865xNaptEntry.extPort = cp->vport;
		rtl865xNaptEntry.remIp = cp->caddr.ip;
		rtl865xNaptEntry.remPort = cp->cport;

		rc = rtl865x_addNaptConnection(&rtl865xNaptEntry, &rtl865xPrio);
		#endif /* CONFIG_RTL_LAYERED_DRIVER_L4 */
		if (!rc)
			cp->hw_acc = 1;
		#ifdef CONFIG_HARDWARE_NAT_DEBUG
		/*2007-12-19*/
		printk("%s:%d:(%s): errno=%d\n %s (%u.%u.%u.%u:%u -> %u.%u.%u.%u:%u) g:(%u.%u.%u.%u:%u)\n",
				__FUNCTION__,__LINE__,"add_nat", rc, "udp", NIPQUAD(cp->daddr), cp->dport,
				NIPQUAD(cp->caddr), cp->cport, NIPQUAD(cp->vaddr), cp->vport
		);
		#endif /* CONFIG_HARDWARE_NAT_DEBUG */
	}
}

#endif /* CONFIG_RTL_HARDWARE_NAT */

#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
int32 rtl8198c_ipv6_router_add(struct rt6_info *rt)
{
	char *dev_t;
	int rc, fc_dst_len, addr_type;
	inv6_addr_t ipDst, ipGw, tmp_dst, tmp_gw;
	struct net_device *netif;
	uint32 tb6_id = 0;

	if (rt && rt->rt6i_table) {
		//read_lock_bh(&rt->rt6i_table->tb6_lock);
		tb6_id = rt->rt6i_table->tb6_id;
		//read_unlock_bh(&rt->rt6i_table->tb6_lock);
	};

	if (tb6_id == RT6_TABLE_MAIN) {
		if (rt->dst.dev) {
			netif = rt->dst.dev;
			if (!net_eq(dev_net(rt->dst.dev), &init_net)) {
				/* Skip if it is not root network namespace */
				return SUCCESS;
			}
		} else {
			netif = NULL;
		}

		dev_t = (netif) ? netif->name : NULL;
		memcpy(&ipDst, &rt->rt6i_dst.addr, sizeof(inv6_addr_t));
		fc_dst_len = rt->rt6i_dst.plen;
		memcpy(&ipGw, &rt->rt6i_gateway, sizeof(inv6_addr_t));

		//change ipv6 address to CPU endian, 98C big endian, 97F little endian
		htonl_array((u32 *)&ipDst.v6_addr32[0],
			(u32 *)&tmp_dst.v6_addr32[0], 4);
		htonl_array((u32 *)&ipGw.v6_addr32[0],
			(u32 *)&tmp_gw.v6_addr32[0], 4);

		addr_type = ipv6_addr_type((struct in6_addr *)&ipDst);
		if ((((ipDst.v6_addr32[0] == 0) && (ipDst.v6_addr32[1] == 0)
					&& (ipDst.v6_addr32[2] == 0)
					&& (ipDst.v6_addr32[3] == 0))
				|| ((!(addr_type & IPV6_ADDR_MULTICAST))
					&& (!(addr_type & IPV6_ADDR_LOOPBACK))))
			&& (fc_dst_len != 128)) {
			rc = rtl8198c_addIpv6Route(tmp_dst, fc_dst_len, tmp_gw,
				dev_t);
			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			printk("%s:%d:(%s): dst: 0x%8x%8x%8x%8x, fc_dst_len is %d, gw:  0x%8x%8x%8x%8x, dev: %s, errno=%d\n",
					__FUNCTION__, __LINE__,"add_ipv6_rt",
					ipDst.v6_addr32[0], ipDst.v6_addr32[1] ,ipDst.v6_addr32[2], ipDst.v6_addr32[3],fc_dst_len,
					ipGw.v6_addr32[0], ipGw.v6_addr32[1] ,ipGw.v6_addr32[2], ipGw.v6_addr32[3],
					 dev_t? dev_t: "null", rc);
			#endif /* CONFIG_HARDWARE_NAT_DEBUG */
		}
	}

	return SUCCESS;
}

int32 rtl8198c_ipv6_router_del(struct rt6_info *rt)
{
	int rc, fc_dst_len, addr_type;
	inv6_addr_t ipDst, tmp_ip;
	uint32 tb6_id = 0;

	if (rt && rt->rt6i_table) {
		//read_lock_bh(&rt->rt6i_table->tb6_lock);
		tb6_id = rt->rt6i_table->tb6_id;
		//read_unlock_bh(&rt->rt6i_table->tb6_lock);
	};

	if (tb6_id == RT6_TABLE_MAIN) {
		if (rt->dst.dev != NULL && !net_eq(dev_net(rt->dst.dev), &init_net)) {
			/* Skip if it is not root network namespace */
			return SUCCESS;
		}
		memcpy(&ipDst, &rt->rt6i_dst.addr, sizeof(inv6_addr_t));
		fc_dst_len = rt->rt6i_dst.plen;

		//change ipv6 address to big endian
		htonl_array((u32 *)&ipDst.v6_addr32[0],
			(u32 *)&tmp_ip.v6_addr32[0], 4);

		addr_type = ipv6_addr_type((struct in6_addr *)&ipDst);
		if (((ipDst.v6_addr32[0] == 0) && (ipDst.v6_addr32[1] == 0)
				&& (ipDst.v6_addr32[2] == 0)
				&& (ipDst.v6_addr32[3] == 0))
			|| ((!(addr_type & IPV6_ADDR_MULTICAST))
				&& (!(addr_type & IPV6_ADDR_LOOPBACK)))) {
			rc = rtl8198c_delIpv6Route(tmp_ip, fc_dst_len);
		}
	}

	return SUCCESS;
}
#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

#if defined(CONFIG_PROC_FS) && defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
static struct proc_dir_entry *proc_gc_overflow_timout = NULL;

static int gc_overflow_timout_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	uint32 tmpBuf[32];

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmpBuf, buffer, count)) {
		tmpBuf[count - 1] = 0;
		rtl_gc_overflow_timout =simple_strtol((const char *)tmpBuf, NULL, 0);

		return count;
	}
	return -EFAULT;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static int gc_overflow_timout_read_proc(struct seq_file *s, void *v)
{
	seq_printf(s, "rtl_gc_overflow_timout(%d), HZ(%d)\n",
		rtl_gc_overflow_timout, HZ);

	return 0;
}

int gc_overflow_timout_single_open(struct inode *inode, struct file *file)
{
	return (single_open(file, gc_overflow_timout_read_proc, NULL));
}

static ssize_t gc_overflow_timout_single_write(struct file *file,
	const char __user *userbuf, size_t count, loff_t *off)
{
	return gc_overflow_timout_write_proc(file, userbuf, count, off);
}

struct file_operations gc_overflow_timout_proc_fops = {
		.open           = gc_overflow_timout_single_open,
		.write          = gc_overflow_timout_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};
#else
static int gc_overflow_timout_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data)
{
	int len;

	len = sprintf(page, "rtl_gc_overflow_timout(%d), HZ(%d)\n",
		rtl_gc_overflow_timout, HZ);

	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}
#endif /* version >= 3.10.0 */

void gc_overflow_timout_proc_init(void)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	proc_gc_overflow_timout =proc_create_data("gc_overflow_timout", 0, &proc_root,
		&gc_overflow_timout_proc_fops, NULL);
#else
	proc_gc_overflow_timout =create_proc_entry("gc_overflow_timout", 0, NULL);
	if (proc_gc_overflow_timout) {
		proc_gc_overflow_timout->read_proc =gc_overflow_timout_read_proc;
		proc_gc_overflow_timout->write_proc =gc_overflow_timout_write_proc;
	}
#endif /* version >= 3.10.0 */
}
#endif /* CONFIG_PROC_FS && CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

#if defined(CONFIG_RTL_LOG_DEBUG)
struct RTL_LOG_PRINT_MASK
/*{
	uint32 ERROR:1;
	uint32 WARN:1;
	uint32 INFO:1;
}*/
 RTL_LogTypeMask;
struct RTL_LOG_ERROR_MASK
/*{
	uint32 MEM:1;
	uint32 SKB:1;
}*/
 RTL_LogErrorMask;
struct RTL_LOG_MODULE_MASK
/*{
	uint8 NIC:1;
	uint8 WIRELESS:1;
	uint8 PROSTACK:1;
}*/
 RTL_LogModuleMask;
uint32 RTL_LogRatelimit = 1;

static struct proc_dir_entry *proc_log_print_control = NULL;
static struct proc_dir_entry *proc_printMask = NULL;
static struct proc_dir_entry *proc_errMask = NULL;
static struct proc_dir_entry *proc_printModule = NULL;
static struct proc_dir_entry *proc_print_rateLimit = NULL;
const char *print_Mask_ID = "typeMask";
const char *print_errMask_ID = "errMask";
const char *print_rateLimit_ID = "rateLimit_enable";
const char *print_module_ID = "module_mask";

static int print_log_write_proc(struct file *file, const char *buffer,
	unsigned long count, void *data)
{
	unsigned char tmpBuf[32];
	int tmp;

	if (count < 2)
		return -EFAULT;

	if (!buffer || copy_from_user(tmpBuf, buffer, count)) {
		return -EFAULT;
	}

	if (0 == strcmp(data, print_Mask_ID)) {
		sscanf(tmpBuf, "%x", &tmp);
		*(uint32 *)&RTL_LogTypeMask = tmp;
	} else if (0 == strcmp(data, print_errMask_ID)) {
		sscanf(tmpBuf, "%x", &tmp);
		*(uint32 *)&RTL_LogErrorMask = tmp;
	} else if (0 == strcmp(data, print_rateLimit_ID)) {
		tmpBuf[count - 1] = 0;
		RTL_LogRatelimit = simple_strtol((const char *)tmpBuf, NULL, 0);
	} else if (0 == strcmp(data, print_module_ID)) {
		sscanf(tmpBuf, "%x", &tmp);
		*(uint32 *)&RTL_LogModuleMask = tmp;
	}

	return count;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static int print_log_read_proc(struct seq_file *s, void *v)
{
	char *data = (char *)s->private;

	if (0 == strcmp(data, print_Mask_ID)) {
		seq_printf(s, "RTL_LogTypeMask(0x%x)\n\tbit means:\n\t ERROR %d\n\t WARN %d \n\t INFO %d \n",
			*(uint32 *)&RTL_LogTypeMask, RTL_LogTypeMask.ERROR,
			RTL_LogTypeMask.WARN, RTL_LogTypeMask.INFO);
	} else if (0 == strcmp(data, print_errMask_ID)) {
		seq_printf(s, "RTL_LogErrorMask(0x%x)\n\tbit means:\n\t MEM %d\n\t SKB %d \n",
			*(uint32 *)&RTL_LogErrorMask, RTL_LogErrorMask.MEM,
			RTL_LogErrorMask.SKB);
	} else if (0 == strcmp(data, print_rateLimit_ID)) {
		seq_printf(s, "rate_limit %d \n", RTL_LogRatelimit);
	} else if (0 == strcmp(data, print_module_ID)) {
		seq_printf(s, "RTL_LogModuleMask(0x%x)\n\tbit means:\n\t NIC %d\n\t WIRELESS %d \n\t PROSTACK %d \n",
			*(uint32 *)&RTL_LogModuleMask, RTL_LogModuleMask.NIC,
			RTL_LogModuleMask.WIRELESS, RTL_LogModuleMask.PROSTACK);
	}

	return 0;
}

int print_log_single_open(struct inode *inode, struct file *file)
{
	return (single_open(file, print_log_read_proc, PDE_DATA(inode)));
}

static ssize_t print_log_single_write(struct file *file,
	const char __user *userbuf, size_t count, loff_t *off)
{
	return print_log_write_proc(file, userbuf, count, off);
}

struct file_operations print_log_proc_fops = {
		.open           = print_log_single_open,
		.write          = print_log_single_write,
		.read           = seq_read,
		.llseek         = seq_lseek,
		.release        = single_release,
};
#else
static int print_log_read_proc(char *page, char **start, off_t off,
	int count, int *eof, void *data) {
	int len;

	if (0 == strcmp(data, print_Mask_ID)) {
		len = sprintf(page, "RTL_LogTypeMask(0x%x)\n\tbit means:\n\t ERROR %d\n\t WARN %d \n\t INFO %d \n",
			*(uint32 *)&RTL_LogTypeMask, RTL_LogTypeMask.ERROR,
			RTL_LogTypeMask.WARN, RTL_LogTypeMask.INFO);
	} else if (0 == strcmp(data, print_errMask_ID)) {
		len = sprintf(page, "RTL_LogErrorMask(0x%x)\n\tbit means:\n\t MEM %d\n\t SKB %d \n",
			*(uint32 *)&RTL_LogErrorMask, RTL_LogErrorMask.MEM,
			RTL_LogErrorMask.SKB);
	} else if (0 == strcmp(data, print_rateLimit_ID)) {
		len = sprintf(page, "rate_limit %d \n", RTL_LogRatelimit);
	} else if (0 == strcmp(data, print_module_ID)) {
		len = sprintf(page, "RTL_LogModuleMask(0x%x)\n\tbit means:\n\t NIC %d\n\t WIRELESS %d \n\t PROSTACK %d \n",
			*(uint32 *)&RTL_LogModuleMask, RTL_LogModuleMask.NIC,
			RTL_LogModuleMask.WIRELESS, RTL_LogModuleMask.PROSTACK);
	}

	if (len <= off + count)
		*eof = 1;
	*start = page + off;
	len -= off;
	if (len > count)
		len = count;
	if (len < 0)
		len = 0;
	return len;
}
#endif /* version >= 3.10.0 */

void log_print_proc_init(void)
{
	RTL_LogTypeMask.ERROR = 1;
	RTL_LogErrorMask.MEM = 1;
	RTL_LogErrorMask.SKB = 1;

	RTL_LogTypeMask.WARN = 1;
	RTL_LogTypeMask.INFO = 0;

	RTL_LogModuleMask.NIC = 1;
	RTL_LogModuleMask.WIRELESS = 1;
	RTL_LogModuleMask.PROSTACK = 1;

	RTL_LogRatelimit = 1;

	proc_log_print_control = proc_mkdir("log_print_control", NULL);
	if (proc_log_print_control) {
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
		proc_printMask =proc_create_data(print_Mask_ID, 0,
			proc_log_print_control, &print_log_proc_fops,
			print_Mask_ID);

		proc_printMask =proc_create_data(print_errMask_ID, 0,
			proc_log_print_control, &print_log_proc_fops,
			print_errMask_ID);

		proc_printMask =proc_create_data(print_rateLimit_ID, 0,
			proc_log_print_control, &print_log_proc_fops,
			print_rateLimit_ID);

		proc_printMask =proc_create_data(print_module_ID, 0,
			proc_log_print_control, &print_log_proc_fops,
			print_module_ID);
		#else
		proc_printMask =create_proc_entry(print_Mask_ID, 0,
			proc_log_print_control);
		if (proc_printMask) {
			proc_printMask->read_proc = print_log_read_proc;
			proc_printMask->write_proc = print_log_write_proc;
			proc_printMask->data = (void *)print_Mask_ID;
		}

		proc_errMask =create_proc_entry(print_errMask_ID, 0,
			proc_log_print_control);
		if (proc_errMask) {
			proc_errMask->read_proc = print_log_read_proc;
			proc_errMask->write_proc = print_log_write_proc;
			proc_errMask->data = (void *)print_errMask_ID;
		}

		proc_print_rateLimit =create_proc_entry(print_rateLimit_ID, 0,
			proc_log_print_control);
		if (proc_print_rateLimit) {
			proc_print_rateLimit->read_proc = print_log_read_proc;
			proc_print_rateLimit->write_proc = print_log_write_proc;
			proc_print_rateLimit->data = (void *)print_rateLimit_ID;
		}

		proc_printModule =create_proc_entry(print_module_ID, 0,
			proc_log_print_control);
		if (proc_printModule) {
			proc_printModule->read_proc = print_log_read_proc;
			proc_printModule->write_proc = print_log_write_proc;
			proc_printModule->data = (void *)print_module_ID;
		}
		#endif /* version >= 3.10.0 */
	}
}
#endif /* CONFIG_RTL_LOG_DEBUG */

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
int rtl_gc_threshold_check(struct net *net)
{
	int ret = FAILED;

	if (net == NULL) {
		if (atomic_read(&(init_net.ct.count)) > rtl_nf_conntrack_threshold)
			ret = SUCCESS;
	} else {
		if (atomic_read(&net->ct.count) > rtl_nf_conntrack_threshold)
			ret = SUCCESS;
	}

	return ret;
}

void rtl_list_del(struct nf_conn *ct)
{
	return list_del(&ct->state_tuple);
}

void rtl_hlist_nulls_del_rcu(struct nf_conn *ct, enum ip_conntrack_dir dir)
{
	return hlist_nulls_del_rcu(&ct->tuplehash[dir].hnnode);
}

void rtl_list_add_tail(struct nf_conn *ct, int proto, int flag)
{
	if (proto == PROT_UDP)
		return list_add_tail(&ct->state_tuple,
			Udp_State_Hash_Head[flag].state_hash);
	else if (proto == PROT_TCP)
		return list_add_tail(&ct->state_tuple,
			Tcp_State_Hash_Head[ct->proto.tcp.state].state_hash);
}

int rtl_test_bit(struct nf_conn *ct, int num)
{
	if (test_bit(num, &ct->status))
		return SUCCESS;
	else
		return FAILED;
}

int rtl_del_ct_timer(struct nf_conn *ct)
{
	return del_timer(&ct->timeout);
}

void rtl_add_ct_timer(struct nf_conn *ct)
{
	return add_timer(&ct->timeout);
}

void rtl_list_move_tail(struct nf_conn *ct, int proto, int state)
{
	if (proto == PROT_UDP)
		return list_move_tail(&ct->state_tuple,
			Udp_State_Hash_Head[state].state_hash);
	else if (proto == PROT_TCP)
		return list_move_tail(&ct->state_tuple,
			Tcp_State_Hash_Head[(enum tcp_conntrack)state].state_hash);
}

unsigned long rtl_get_ct_timer_expires(struct nf_conn *ct)
{
	return ct->timeout.expires;
}

void rtl_nf_ct_stat_inc(struct net *net)
{
	NF_CT_STAT_INC(net, delete_list);

	return;
}

int rtl_skb_network_offset(struct sk_buff *skb)
{
	return skb_network_offset(skb);
}

u_int8_t rtl_new_gc_get_ct_protonum(void *ct_ptr, enum ip_conntrack_dir dir)
{
	struct nf_conn *ct = (struct nf_conn *)ct_ptr;

	return ct->tuplehash[dir].tuple.dst.protonum;
}

struct iphdr *rtl_new_gc_ip_hdr(struct sk_buff *skb)
{
	return ip_hdr(skb);
}

__be16 rtl_new_gc_get_skb_protocol(struct sk_buff * skb)
{
	struct iphdr *iph;

	iph = ip_hdr(skb);

	return iph->protocol;
	//return skb->protocol;
}

unsigned long rtl_new_gc_get_ct_udp_status(void *ct_ptr)
{
	struct nf_conn *ct = (struct nf_conn *)ct_ptr;

	return ct->status;
}

u_int8_t rtl_new_gc_get_ct_tcp_state(void *ct_ptr)
{
	struct nf_conn *ct = (struct nf_conn *)ct_ptr;

	return ct->proto.tcp.state;
}

void rtl_new_gc_set_ct_timeout_expires(void *ct_ptr, unsigned long value)
{
	struct nf_conn *ct = (struct nf_conn *)ct_ptr;

	ct->timeout.expires = value;

	return;
}

/*flag = 0 for src; flag = 1 for dst*/
__be32 rtl_new_gc_get_ct_ip_by_dir(void *ct_ptr, enum ip_conntrack_dir dir,
	int flag)
{
	struct nf_conn *ct = (struct nf_conn *)ct_ptr;

	if (dir == IP_CT_DIR_ORIGINAL) {
		if (flag == 0)
			return ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u3.ip;
		else if (flag == 1)
			return ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u3.ip;
	} else if (dir == IP_CT_DIR_REPLY) {
		if (flag == 0)
			return ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u3.ip;
		else if (flag == 1)
			return ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u3.ip;
	}
}

/*flag = 0 for src; flag = 1 for dst*/
__be16 rtl_new_gc_get_ct_port_by_dir(void *ct_ptr, enum ip_conntrack_dir dir,
	int flag)
{
	struct nf_conn *ct = (struct nf_conn *)ct_ptr;

	if (dir == IP_CT_DIR_ORIGINAL) {
		if (flag == 0)
			return ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.src.u.all;
		else if (flag == 1)
			return ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.u.all;

	} else if (dir == IP_CT_DIR_REPLY) {
		if (flag == 0)
			return ct->tuplehash[IP_CT_DIR_REPLY].tuple.src.u.all;
		else if (flag == 1)
			return ct->tuplehash[IP_CT_DIR_REPLY].tuple.dst.u.all;
	}
}

#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */
#if defined(CONFIG_RTD_1295_HWNAT) && !defined(CONFIG_RTL_IPTABLES_FAST_PATH)
#include <net/arp.h>
#include <net/rtl/rtl865x_netif.h>
/* To query hardware address based on IP through arp table of dev */
int arp_req_get_ha(__be32 queryIP, struct net_device *dev,
	unsigned char *resHwAddr)
{
	__be32 ip = queryIP;
	struct neighbour *neigh;
	int err = -ENXIO;

	neigh = neigh_lookup(&arp_tbl, &ip, dev);
	if (neigh) {
		read_lock_bh(&neigh->lock);
		memcpy(resHwAddr, neigh->ha, dev->addr_len);
		read_unlock_bh(&neigh->lock);
		neigh_release(neigh);
		err = 0;
	}

	return err;
}

struct dst_entry *dst_tmp = NULL;

void rtl_store_skb_dst(struct sk_buff *skb)
{
	dst_tmp = rtl_skb_dst(skb);

	skb->_skb_refdst = 0UL;
	return;
}

void rtl_set_skb_dst(struct sk_buff *skb)
{
	skb_dst_set(skb, dst_tmp);
	return;
}

#if defined(CONFIG_RTL_HW_QOS_SUPPORT)
int32 rtl_qosGetSkbMarkByNaptEntry(rtl865x_napt_entry * naptEntry,
	rtl865x_qos_mark * qosMark, struct sk_buff *pskb)
{
	struct iphdr *iph;
	struct tcphdr *tcphupuh;	/* just keep one , don't care tcp or udp */
	u_int ori_saddr, ori_daddr;
	u_short ori_sport, ori_dport;
	struct net_device *lanDev, *wanDev;
	unsigned char oriSrcMac[6], oriDstMac[6], resMac[14];
	u_short proto;
	unsigned char pppProto[2], ipProto[2];
	__u32 oriSkbMark;

	#if !defined(CONFIG_RTD_1295_HWNAT)
	unsigned long irq_flags;
	#endif /* !CONFIG_RTD_1295_HWNAT */
	uint32 preMark, postMark;

	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	uint32 preMarkExt[max_decision_priority];
	uint32 postMarkExt[max_decision_priority];
	uint32 oriSkbMarkExt[max_decision_priority];
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	struct nf_hook_state state;

	if (pskb == NULL)
		return FAILED;

	/* initial */
	pppProto[0] = 0x00;
	pppProto[1] = 0x21;
	ipProto[0] = 0x08;
	ipProto[1] = 0x00;

	lanDev = rtl865x_getLanDev();
	wanDev = rtl865x_getWanDev();
	memset(&state, 0, sizeof(struct nf_hook_state));
	state.in = lanDev;
	state.out = wanDev;
	proto = ntohs(pskb->protocol);
	iph = ip_hdr(pskb);
	if (iph == NULL) {
		return FAILED;
	}
	tcphupuh = (struct tcphdr *)((__u32 *) iph + iph->ihl);

	if (eth_hdr(pskb) == NULL) {
		return FAILED;
	}
	/* To bak origal protol mac */
	memcpy(oriSrcMac, eth_hdr(pskb)->h_source, ETH_ALEN);
	memcpy(oriDstMac, eth_hdr(pskb)->h_dest, ETH_ALEN);

	/* Bak orignal skb mark */
	oriSkbMark = pskb->mark;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memcpy(oriSkbMarkExt, pskb->mark_ext, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	/* check ip-based qos rule at iptables mangle table */
	/* To record original info */
	ori_saddr = iph->saddr;
	ori_sport = tcphupuh->source;
	ori_daddr = iph->daddr;
	ori_dport = tcphupuh->dest;

	/* for dst mac match, please refer to the xt_mac.c */
	rtl_store_skb_dst(pskb);
	/* For uplink */
	#if defined(CONFIG_NET_SCHED)
	preMark = 0;
	postMark = 0;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memset(preMarkExt, 0, sizeof(preMarkExt));
	memset(postMarkExt, 0, sizeof(postMarkExt));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	/* Replace source addr to check uplink mark */
	iph->saddr = naptEntry->intIp;
	tcphupuh->source = naptEntry->intPort;
	iph->daddr = naptEntry->remIp;
	tcphupuh->dest = naptEntry->remPort;

	memset(resMac, 0, 14);
	if ((lanDev != NULL)
		&& (arp_req_get_ha(naptEntry->intIp, lanDev, resMac) == 0)) {
		/* Patch for pppoe wantype: run udp chariot */
		/* bak skb mac header */
		if ((memcmp(pskb->data - 2, pppProto, 2) == 0)	/* equal 0x0021 */
			&&(skb_mac_header_was_set(pskb) == 1)
			&& (eth_hdr(pskb)->h_proto != ntohs(0x0800))) {	/* not equal to 0x0800 */
			skb_set_mac_header(pskb, -22);
		}

		/* Replace source mac addr to check uplink mark */
		memcpy(eth_hdr(pskb)->h_source, resMac, ETH_ALEN);
		memcpy(eth_hdr(pskb)->h_dest, lanDev->dev_addr, ETH_ALEN);
	}

	pskb->mark = 0;	/* initial */
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memset(pskb->mark_ext, 0, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	if (proto == ETH_P_IP) {
		state.in = lanDev;
		state.out = wanDev;
		(list_empty(&nf_hooks[PF_INET][NF_IP_PRE_ROUTING]))?: \
			ipt_do_table(pskb, NF_IP_PRE_ROUTING, &state,\
			dev_net(lanDev)->ipv4.iptable_mangle);
	}

	DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__,
		__LINE__, lanDev ? lanDev->name : "NULL",
		wanDev ? wanDev->name : "NULL",
		pskb->inDev ? pskb->inDev->name : "NULL",
		pskb->dev ? pskb->dev->name : "NULL", pskb->mark);
	preMark = pskb->mark;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memcpy(preMarkExt, pskb->mark_ext, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	/* Replace dest addr to check uplink mark */
	iph->saddr = naptEntry->extIp;
	tcphupuh->source = naptEntry->extPort;
	iph->daddr = naptEntry->remIp;
	tcphupuh->dest = naptEntry->remPort;

	memset(resMac, 0, 14);
	if ((wanDev != NULL)
		&& (arp_req_get_ha(naptEntry->remIp, wanDev, resMac) == 0)) {
		/* Patch for pppoe wantype: run udp chariot */
		/* bak skb mac header */
		if ((memcmp(pskb->data - 2, pppProto, 2) == 0)	/* equal 0x0021 */
			&&(skb_mac_header_was_set(pskb) == 1)
			&& (eth_hdr(pskb)->h_proto != ntohs(0x0800))) {	/* not equal to 0x0800 */
			skb_set_mac_header(pskb, -22);
		}

		/* Replace source mac addr to check uplink mark */
		memcpy(eth_hdr(pskb)->h_dest, resMac, ETH_ALEN);
		memcpy(eth_hdr(pskb)->h_source, wanDev->dev_addr, ETH_ALEN);
	}

	pskb->mark = 0;	/* initial */
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memset(pskb->mark_ext, 0, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	if (proto == ETH_P_IP) {
		state.in = lanDev;
		state.out = wanDev;
		(list_empty(&nf_hooks[PF_INET][NF_IP_POST_ROUTING]))?: \
			ipt_do_table(pskb, NF_IP_POST_ROUTING, &state,\
			dev_net(wanDev)->ipv4.iptable_mangle);
	}
	DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__,
		__LINE__, lanDev ? lanDev->name : "NULL",
		wanDev ? wanDev->name : "NULL",
		pskb->inDev ? pskb->inDev->name : "NULL",
		pskb->dev ? pskb->dev->name : "NULL", pskb->mark);
	postMark = pskb->mark;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memcpy(postMark, pskb->mark_ext, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	qosMark[port_decision_priority].uplinkMark =(postMarkExt[port_decision_priority] ?
		postMarkExt[port_decision_priority] :
		preMarkExt[port_decision_priority]);
	qosMark[vlan_decision_priority].uplinkMark =(postMarkExt[vlan_decision_priority] ?
		postMarkExt[vlan_decision_priority] :
		preMarkExt[vlan_decision_priority]);
	qosMark[dscp_decision_priority].uplinkMark =(postMarkExt[dscp_decision_priority] ?
		postMarkExt[dscp_decision_priority] :
		preMarkExt[dscp_decision_priority]);
	qosMark[nat_decision_priority].uplinkMark = (postMark ? postMark : preMark);	/*nat decision mark is store in skb->mark */
	#else
	qosMark->uplinkMark = (postMark ? postMark : preMark);
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	#endif /* CONFIG_NET_SCHED */

	/* for downlink */
	#if defined(CONFIG_NET_SCHED)
	preMark = 0;
	postMark = 0;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memset(preMarkExt, 0, sizeof(preMarkExt));
	memset(postMarkExt, 0, sizeof(postMarkExt));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	//Replace source addr to check uplink mark
	iph->saddr = naptEntry->remIp;
	tcphupuh->source = naptEntry->remPort;
	iph->daddr = naptEntry->extIp;
	tcphupuh->dest = naptEntry->extPort;

	memset(resMac, 0, 14);
	if ((wanDev != NULL)
		&& (arp_req_get_ha(naptEntry->remIp, wanDev, resMac) == 0)) {
		/* Patch for pppoe wantype: run udp chariot */
		if ((memcmp(pskb->data - 2, pppProto, 2) == 0)	/* equal 0x0021 */
			&&(skb_mac_header_was_set(pskb) == 1)
			&& (eth_hdr(pskb)->h_proto != ntohs(0x0800))) {	/* not equal to 0x0800 */
			skb_set_mac_header(pskb, -22);
		}

		/* Replace source mac addr to check uplink mark */
		memcpy(eth_hdr(pskb)->h_source, resMac, ETH_ALEN);
		memcpy(eth_hdr(pskb)->h_dest, wanDev->dev_addr, ETH_ALEN);
	}

	pskb->mark = 0;	/* initial */
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memset(pskb->mark_ext, 0, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	if (proto == ETH_P_IP) {
		state.in = wanDev;
		state.out = lanDev;
		(list_empty(&nf_hooks[PF_INET][NF_IP_PRE_ROUTING]))?: \
			ipt_do_table(pskb, NF_IP_PRE_ROUTING, &state,\
			dev_net(wanDev)->ipv4.iptable_mangle);
	}
	DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__,
		__LINE__, lanDev ? lanDev->name : "NULL",
		wanDev ? wanDev->name : "NULL",
		pskb->inDev ? pskb->inDev->name : "NULL",
		pskb->dev ? pskb->dev->name : "NULL", pskb->mark);
	preMark = pskb->mark;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memcpy(preMarkExt, pskb->mark_ext, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	/* Replace dest addr to check uplink mark */
	iph->saddr = naptEntry->remIp;
	tcphupuh->source = naptEntry->remPort;
	iph->daddr = naptEntry->intIp;
	tcphupuh->dest = naptEntry->intPort;

	memset(resMac, 0, 14);
	if ((lanDev != NULL)
		&& (arp_req_get_ha(naptEntry->intIp, lanDev, resMac) == 0)) {
		/* Patch for pppoe wantype: run udp chariot */
		if ((memcmp(pskb->data - 2, pppProto, 2) == 0)	/* equal 0x0021 */
			&&(skb_mac_header_was_set(pskb) == 1)
			&& (eth_hdr(pskb)->h_proto != ntohs(0x0800))) {	/* not equal to 0x0800 */
			skb_set_mac_header(pskb, -22);
		}
		/* Replace dest mac addr and  hh data mac to check uplink mark */
		memcpy(eth_hdr(pskb)->h_dest, resMac, ETH_ALEN);
		memcpy(eth_hdr(pskb)->h_source, lanDev->dev_addr, ETH_ALEN);
	}
	pskb->mark = 0;	/* initial */
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memset(pskb->mark_ext, 0, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	if (proto == ETH_P_IP) {
		state.in = wanDev;
		state.out = lanDev;
		(list_empty(&nf_hooks[PF_INET][NF_IP_POST_ROUTING]))?: \
			ipt_do_table(pskb, NF_IP_POST_ROUTING, &state,\
			dev_net(lanDev)->ipv4.iptable_mangle);
	}
	DEBUGP_API("[%s][%d]:[%s][%s][%s][%s][%d]\n", __FUNCTION__,
		__LINE__, lanDev ? lanDev->name : "NULL",
		wanDev ? wanDev->name : "NULL",
		pskb->inDev ? pskb->inDev->name : "NULL",
		pskb->dev ? pskb->dev->name : "NULL", pskb->mark);
	postMark = pskb->mark;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memcpy(postMark, pskb->mark_ext, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	qosMark[port_decision_priority].downlinkMark =(postMarkExt[port_decision_priority] ?
		postMarkExt[port_decision_priority] :
		preMarkExt[port_decision_priority]);
	qosMark[vlan_decision_priority].downlinkMark =(postMarkExt[vlan_decision_priority] ?
		postMarkExt[vlan_decision_priority] :
		preMarkExt[vlan_decision_priority]);
	qosMark[dscp_decision_priority].downlinkMark =(postMarkExt[dscp_decision_priority] ?
		postMarkExt[dscp_decision_priority] :
		preMarkExt[dscp_decision_priority]);
	qosMark[nat_decision_priority].downlinkMark = (postMark ? postMark : preMark);	/*nat decision mark is store in skb->mark */
	#else
	qosMark->downlinkMark = (postMark ? postMark : preMark);
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */
	#endif /* CONFIG_NET_SCHED */

	/* Back to original value */
	/* Back to orignal protol mac */
	memcpy(eth_hdr(pskb)->h_source, oriSrcMac, ETH_ALEN);
	memcpy(eth_hdr(pskb)->h_dest, oriDstMac, ETH_ALEN);

	/* Back to original skb mark */
	pskb->mark = oriSkbMark;
	#if defined(CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY)
	memcpy(pskb->mark_ext, oriSkbMarkExt, sizeof(pskb->mark_ext));
	#endif /* CONFIG_RTL_SW_QUEUE_DECISION_PRIORITY */

	/* back to original info */
	iph->saddr = ori_saddr;
	tcphupuh->source = ori_sport;
	iph->daddr = ori_daddr;
	tcphupuh->dest = ori_dport;

	rtl_set_skb_dst(pskb);
	if (lanDev)
		dev_put(lanDev);

	if (wanDev)
		dev_put(wanDev);

	return SUCCESS;
}
#endif /* defined(CONFIG_RTL_HW_QOS_SUPPORT) */
#endif /* defined(CONFIG_RTD_1295_HWNAT) &&
        * !defined(CONFIG_RTL_IPTABLES_FAST_PATH) */
