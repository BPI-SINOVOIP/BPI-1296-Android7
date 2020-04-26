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
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/inetdevice.h>

#include <net/ip_fib.h>

#if defined(CONFIG_RTL_AP_PACKAGE) && !defined(CONFIG_NETFILTER)
#else
#include <net/netfilter/nf_nat.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_core.h>
#endif

#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,4,0)
#if defined(CONFIG_RTL_AP_PACKAGE) && !defined(CONFIG_NETFILTER)
#else
#include <net/netfilter/nf_nat.h>
#endif
#else
#if defined(CONFIG_RTL_AP_PACKAGE) && !defined(CONFIG_NETFILTER)
#else
#include <net/netfilter/nf_nat_protocol.h>
#endif
#endif

#include <net/ip_vs.h>
#include <linux/ip_vs.h>

#ifdef CONFIG_BRIDGE
#include <bridge/br_private.h>
#endif

#include <net/rtl/rtl_types.h>
#include <net/rtl/fastpath/fastpath_core.h>
#include <net/rtl/features/rtl_features.h>
#include <net/rtl/features/rtl_ps_hooks.h>

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

#if defined(CONFIG_RTL_HW_QOS_SUPPORT) && defined(CONFIG_RTL_HARDWARE_NAT)
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl865x_outputQueue.h>
#endif

#if defined (CONFIG_RTL865X_LANPORT_RESTRICTION)
#include <net/rtl/features/lan_restrict.h>
#endif

#ifdef CONFIG_RTL_HARDWARE_NAT
//#define CONFIG_HARDWARE_NAT_DEBUG
#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
#include <net/rtl/rtl865x_ppp.h>
#include <net/rtl/rtl865x_route_api.h>
#endif
//unsigned int ldst, lmask, wdst, wmask;
extern int gHwNatEnabled;
#endif

#ifdef CONFIG_RTL_HARDWARE_NAT
/*2007-12-19*/
#ifdef CONFIG_RTL_LAYERED_DRIVER_L3
#include <net/rtl/rtl865x_ip_api.h>
#endif
#ifdef CONFIG_RTL_LAYERED_DRIVER_L4
#include <net/rtl/rtl865x_nat.h>
#endif
extern char masq_if[];
extern unsigned int hw_napt_ip;
#endif

#if defined(CONFIG_RTD_1295_HWNAT)
#include <linux/seqlock.h>
#endif /* CONFIG_RTD_1295_HWNAT */

#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) && defined(CONFIG_FAST_PATH_MODULE)
enum LR_RESULT (*FastPath_hook5)(ipaddr_t ip, ether_addr_t* mac, enum ARP_FLAGS flags) = NULL;
enum LR_RESULT (*FastPath_hook7)(ipaddr_t ip) = NULL;
enum LR_RESULT (*FastPath_hook8)(ipaddr_t ip, ether_addr_t* mac, enum ARP_FLAGS flags) = NULL;
EXPORT_SYMBOL(FastPath_hook8);
EXPORT_SYMBOL(FastPath_hook7);
EXPORT_SYMBOL(FastPath_hook5);
#endif

#if defined(CONFIG_RTD_1295_HWNAT)
bool rtl_hwnat_enable = false;

EXPORT_SYMBOL(rtl_hwnat_enable);
#define CHECK_IF_HWNAT_READY if (!rtl_hwnat_enable) return RTL_PS_HOOKS_CONTINUE
#define CHECK_IF_HWNAT_READY2 if (!rtl_hwnat_enable) return
#endif /* CONFIG_RTD_1295_HWNAT */

int32 rtl_nf_conntrack_in_hooks(rtl_nf_conntrack_inso_s *info)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	#if !defined(IMPROVE_QOS)
	rtl_fpAddConnCache(info->ct, info->skb);
	#elif defined(CONFIG_RTL_ROUTER_FAST_PATH)
	if (routerTypeFlag == 1)
		rtl_fpAddConnCache(info->ct, info->skb);
	#endif /* !IMPROVE_QOS */
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	#ifdef CONFIG_RTL_FAST_IPV6
	if (fast_ipv6_fw)
		rtl_AddV6ConnCache(info->ct, info->skb);
	#endif /* CONFIG_RTL_FAST_IPV6 */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_nf_conntrack_death_by_timeout_hooks(rtl_nf_conntrack_inso_s *info)
{
	int ret;

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	ret = RTL_PS_HOOKS_CONTINUE;
	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
	if (rtl_connCache_timer_update(info->ct) == SUCCESS) {
		ret = RTL_PS_HOOKS_RETURN;
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT */

	#ifdef CONFIG_RTL_FAST_IPV6
	if (fast_ipv6_fw && rtl_V6_connCache_timer_update(info->ct) == SUCCESS) {
		ret = RTL_PS_HOOKS_RETURN;
	}
	#endif /* CONFIG_RTL_FAST_IPV6 */
	return ret;
}

int32 rtl_nf_conntrack_destroy_hooks(rtl_nf_conntrack_inso_s *info)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
	#if defined(CONFIG_RTD_1295_HWNAT)
	local_bh_disable();

	info->double_lock(info);
	#else /* CONFIG_RTD_1295_HWNAT */
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	spin_lock_bh(&nf_conntrack_lock);
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_RTD_1295_HWNAT */

	rtl_delConnCache(info->ct);

	#if defined(CONFIG_RTD_1295_HWNAT)
	info->double_unlock(info);
	local_bh_enable();
	#else /* CONFIG_RTD_1295_HWNAT */
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	spin_unlock_bh(&nf_conntrack_lock);
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_RTD_1295_HWNAT */
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT */

	#ifdef CONFIG_RTL_FAST_IPV6
	if (fast_ipv6_fw)
		rtl_DelV6ConnCache(info->ct);
	#endif /* CONFIG_RTL_FAST_IPV6 */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_nf_conntrack_confirm_hooks(rtl_nf_conntrack_inso_s *info)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	rtl_connGC_addList((void *)info->skb, (void *)info->ct);
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_nf_init_conntrack_hooks(rtl_nf_conntrack_inso_s *info)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	INIT_LIST_HEAD(&info->ct->state_tuple);
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_nf_conntrack_init_hooks(void)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	rtl_nf_conn_GC_init();
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_tcp_packet_hooks(rtl_nf_conntrack_inso_s *info)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	if (info->new_state == TCP_CONNTRACK_LAST_ACK) {
		rtl865x_handle_nat(info->ct, 0, NULL);
	}
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_nf_nat_packet_hooks(rtl_nf_conntrack_inso_s *info)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if (defined(IMPROVE_QOS) && defined(CONFIG_RTL_IPTABLES_FAST_PATH)) || defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_addConnCache(info->ct, info->skb);
	#endif /* (IMPROVE_QOS && CONFIG_RTL_IPTABLES_FAST_PATH) || CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_nf_nat_packet_hooks);

#ifdef CONFIG_RTL_WLAN_DOS_FILTER
#define RTL_WLAN_NAME			"wlan"
#define TCP_SYN				2
#define _MAX_SYN_THRESHOLD		400
#define _WLAN_BLOCK_TIME		20	// unit: seconds

int wlan_syn_cnt = 0;
int wlan_block = 0, wlan_block_count = 0;
unsigned int dbg_wlan_dos_block_pkt_num = 0;
unsigned char block_source_mac[6];
int wlan_dos_filter_enabled = 1;

extern unsigned int _br0_ip;

#ifdef CONFIG_RTL8192CD
extern int issue_disassoc_from_kernel(
	void *priv,
	unsigned char *mac);
#endif /* CONFIG_RTL8192CD */

static struct timer_list wlan_dos_timer;
static void wlan_dos_timer_fn(unsigned long arg)
{
	wlan_syn_cnt = 0;

	if (wlan_block_count >= _WLAN_BLOCK_TIME) {
		wlan_block = 0;
		wlan_block_count = 0;
	}
	if (wlan_block == 1)
		wlan_block_count++;

	mod_timer(&wlan_dos_timer, jiffies + HZ);
}

#endif /* CONFIG_RTL_WLAN_DOS_FILTER */

int32 rtl_nat_init_hooks(void)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#ifdef CONFIG_RTL_819X_SWCORE
	rtl_nat_init();
	#endif /* CONFIG_RTL_819X_SWCORE */

	#if defined(CONFIG_NET_SCHED)
	rtl_qos_init();
	#endif /* CONFIG_NET_SCHED */

	#ifdef CONFIG_RTL_WLAN_DOS_FILTER
	init_timer(&wlan_dos_timer);
	wlan_dos_timer.expires = jiffies + HZ;
	wlan_dos_timer.data = 0L;
	wlan_dos_timer.function = wlan_dos_timer_fn;
	mod_timer(&wlan_dos_timer, jiffies + HZ);
	#endif /* CONFIG_RTL_WLAN_DOS_FILTER */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_nat_init_hooks);

int32 rtl_nat_cleanup_hooks(void)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_NET_SCHED)
	rtl_qos_cleanup();
	#endif /* CONFIG_NET_SCHED */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_nat_cleanup_hooks);

int32 rtl_fn_hash_insert_hooks(
	struct fib_table *tb,
	struct fib_config *cfg,
	struct fib_info *fi)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined( CONFIG_RTL_IPTABLES_FAST_PATH)
	#if defined(CONFIG_FAST_PATH_MODULE)
	if (FastPath_hook3 != NULL) {
		FastPath_hook3(cfg->fc_dst ? cfg->fc_dst : 0,
			inet_make_mask(cfg->fc_dst_len),
			cfg->fc_gw ? cfg->fc_gw : 0, (uint8 *)fi->fib_dev->name,
			RT_NONE);
	}
	#else
	rtk_addRoute(cfg->fc_dst ? cfg->fc_dst : 0,
		inet_make_mask(cfg->fc_dst_len), cfg->fc_gw ? cfg->fc_gw : 0,
		(uint8 *)fi->fib_dev->name, RT_NONE);
	#endif /* CONFIG_FAST_PATH_MODULE */
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_fn_insert(tb, cfg, fi);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_fn_hash_delete_hooks(struct fib_table *tb, struct fib_config *cfg)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTK_IPTABLES_FAST_PATH)
	#ifdef CONFIG_FAST_PATH_MODULE
	if (FastPath_hook1 != NULL) {
		FastPath_hook1(cfg->fc_dst ? cfg->fc_dst : 0,
			inet_make_mask(cfg->fc_dst_len));
	}
	#else
	rtk_delRoute(cfg->fc_dst ? cfg->fc_dst : 0,
		inet_make_mask(cfg->fc_dst_len));
	#endif /* CONFIG_FAST_PATH_MODULE */
	#endif /* CONFIG_RTK_IPTABLES_FAST_PATH */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_fn_delete(tb, cfg);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_fn_flush_list_hooks(
	int fz_order,
	int idx,
	u32 tb_id,
	u32 fn_key)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_fn_flush(fz_order, idx, tb_id, fn_key);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
int32 rtl_fib_flush_list_hooks(u32 tb_id, u32 fn_key, u32 ip_mask)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_fib_flush(tb_id, fn_key, ip_mask);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}
#endif /* version >= 3.10.0 */

int32 rtl_fn_hash_replace_hooks(
	struct fib_table *tb,
	struct fib_config *cfg,
	struct fib_info *fi)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	#if defined(CONFIG_FAST_PATH_MODULE)
	if (FastPath_hook2 != NULL) {
		FastPath_hook2(cfg->fc_dst ? cfg->fc_dst : 0,
			inet_make_mask(cfg->fc_dst_len),
			cfg->fc_gw ? cfg->fc_gw : 0, (uint8 *)fi->fib_dev->name,
			RT_NONE);
	}
	#else
	rtk_modifyRoute(cfg->fc_dst ? cfg->fc_dst : 0,
		inet_make_mask(cfg->fc_dst_len), cfg->fc_gw ? cfg->fc_gw : 0,
		(uint8 *)fi->fib_dev->name, RT_NONE);
	#endif /* CONFIG_FAST_PATH_MODULE */
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl8198c_fib6_add_hooks(void *rt)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	rtl8198c_ipv6_router_add((struct rt6_info *)rt);
	#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl8198c_fib6_del_hooks(void *rt)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	rtl8198c_ipv6_router_del((struct rt6_info *)rt);
	#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_dev_queue_xmit_hooks(struct sk_buff *skb, struct net_device *dev)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	rtl_fp_dev_queue_xmit_check(skb, dev);
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_dev_hard_start_xmit_hooks(
	struct sk_buff *skb,
	struct net_device *dev,
	struct netdev_queue *txq)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	rtl_fp_dev_hard_start_xmit_check(skb, dev, txq);
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	return RTL_PS_HOOKS_CONTINUE;
}

#ifdef CONFIG_RTL_WLAN_DOS_FILTER

static int filter_dos_wlan(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	unsigned char *tflag;
	int ret = NF_ACCEPT;

	iph = ip_hdr(skb);
	tcph = (void *)iph + iph->ihl * 4;
	tflag = (void *) tcph + 13;

	if ((skb->dev) && (!strncmp(skb->dev->name, RTL_WLAN_NAME, 4)))	// wlan0, wlan1, wlan0-va0, ... and so on
	{
		if ((iph->protocol == IPPROTO_TCP) && ((*tflag & 0x3f) == TCP_SYN) && (iph->daddr == _br0_ip))	// xdos.exe 192.168.1.254 0-65535
		{
			if ((wlan_block == 1)
				&& (memcmp(block_source_mac, &(skb->mac_header[6]), 6) == 0)) {
				dbg_wlan_dos_block_pkt_num++;
				ret = NF_DROP;
			} else {
				wlan_syn_cnt++;

				if (wlan_syn_cnt > _MAX_SYN_THRESHOLD) {
					wlan_block = 1;

					#ifdef CONFIG_RTL8192CD
					issue_disassoc_from_kernel((void *) skb->dev->priv, &(skb->mac_header[6]));
					#endif /* CONFIG_RTL8192CD */
					memcpy(block_source_mac, &(skb->mac_header[6]), 6);
				}
			}
		}

	}

	return (ret);
}

int filter_dos_wlan_enter(struct sk_buff **pskb)
{
	int ret;
	struct sk_buff *skb;

	skb = *pskb;
	skb->transport_header = skb->data;
	skb->network_header = skb->data;

	ret = filter_dos_wlan((void *)skb);
	if (ret == NF_DROP) {
		kfree_skb(skb);
		ret = NET_RX_DROP;
	} else {
		ret = NET_RX_SUCCESS;
	}

	return ret;
}
#endif /* CONFIG_RTL_WLAN_DOS_FILTER */

#if (LINUX_VERSION_CODE>=KERNEL_VERSION(3,10,0)) && defined(CONFIG_RTL_VLAN_8021Q)
extern struct net_device *rtl_fastpath_vlan_find_dev(
	struct net_device *real_dev,
	__be16 vlan_proto,
	u16 vlan_id);
extern int linux_vlan_enable;

static int32 rtl_fastpath_vlan_untag(struct sk_buff *skb)
{
	struct vlan_hdr *vhdr;

	skb = skb_share_check(skb, GFP_ATOMIC);
	if (unlikely(skb == NULL))
		goto err_free;

	if (unlikely(!pskb_may_pull(skb, VLAN_HLEN)))
		goto err_free;

	vhdr = (struct vlan_hdr *)skb->data;
	vlan_set_encap_proto(skb, vhdr);

	skb_pull(skb, VLAN_HLEN);

	if (unlikely(skb_cow(skb, skb_headroom(skb)) < 0))
		goto err_free;

	memmove(skb->data - ETH_HLEN, skb->data - VLAN_ETH_HLEN, ETH_ALEN << 1);

	skb->mac_header += VLAN_HLEN;
	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	return 0;

err_free:
	kfree_skb(skb);
	return -1;
}

static int32 rtl_fastpath_vlan_put_tag(
	struct sk_buff *skb,
	char *saved_vlan_tag)
{
	if (unlikely(skb_cow_head(skb, VLAN_HLEN) < 0))
		goto err_free;

	skb->protocol = htons(ETH_P_8021Q);

	memmove(skb->data - VLAN_ETH_HLEN, skb->data - ETH_HLEN, ETH_ALEN << 1);
	skb_push(skb, VLAN_HLEN);

	memcpy(skb->data - 2, (void *)saved_vlan_tag, VLAN_HLEN);

	skb->mac_header -= VLAN_HLEN;
	skb_reset_network_header(skb);
	skb_reset_transport_header(skb);
	skb_reset_mac_len(skb);

	return 0;

err_free:
	kfree_skb(skb);
	return -1;
}
#endif

int32 rtl_netif_receive_skb_hooks(struct sk_buff **pskb)
{
	int ret;

	#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3, 10, 0)) && defined(CONFIG_RTL_VLAN_8021Q)
	char saved_vlan_tag[VLAN_HLEN] = { 0 };
	struct net_device *orig_dev = NULL;
	int16 vlan_id = 0;

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	memset(saved_vlan_tag, 0, VLAN_HLEN);

	#ifdef CONFIG_HARDWARE_NAT_DEBUG
	pr_err("%s:%d: pskb protocol = 0x%x, mac[0] = 0x%x, pkt_type = 0x%x\n",
		__func__, __LINE__, (*pskb)->protocol, skb_mac_header(*pskb)[0],
		(*pskb)->pkt_type);
	#endif /* CONFIG_HARDWARE_NAT_DEBUG */

	#if defined(CONFIG_RTD_1295_HWNAT)
	if (linux_vlan_enable &&
		((*pskb)->protocol==cpu_to_be16(ETH_P_8021Q) || (*pskb)->protocol==cpu_to_be16(ETH_P_8021AD)) &&
		!(skb_mac_header(*pskb)[0] & 0x01) &&
		(*pskb)->pkt_type == PACKET_HOST)
	#else /* CONFIG_RTD_1295_HWNAT */
	if (linux_vlan_enable &&
		((*pskb)->protocol==cpu_to_be16(ETH_P_8021Q) || (*pskb)->protocol==cpu_to_be16(ETH_P_8021AD)) &&
		!((*pskb)->mac_header[0] & 0x01) &&
		(*pskb)->pkt_type == PACKET_HOST)
	#endif /* CONFIG_RTD_1295_HWNAT */
	{
		memcpy((void *)saved_vlan_tag, (*pskb)->data - 2, VLAN_HLEN);

		orig_dev = (*pskb)->dev;
		vlan_id = ntohs(*(short *)(saved_vlan_tag + 2)) & 0x0fff;

		#ifdef CONFIG_HARDWARE_NAT_DEBUG
		pr_err("%s:%d: dev name (%s), vlan_id 0x%x\n", __func__,
			__LINE__, (*pskb)->dev ? (*pskb)->dev->name : "null",
			vlan_id);
		#endif /* CONFIG_HARDWARE_NAT_DEBUG */

		(*pskb)->dev = rtl_fastpath_vlan_find_dev((*pskb)->dev,
			(*pskb)->protocol, vlan_id);
		if ((*pskb)->dev == NULL) {
			kfree_skb(*pskb);
			return RTL_PS_HOOKS_RETURN;
		}

		ret = rtl_fastpath_vlan_untag(*pskb);
		if (ret < 0)
			return RTL_PS_HOOKS_RETURN;
		pr_err("%s:%d: done\n", __func__, __LINE__);
	}
	#endif /* version >= 3.10.0 && CONFIG_RTL_VLAN_8021Q */

	#if defined(CONFIG_RTL_FAST_BRIDGE)
	if (skb->dev->br_port
		&& (rtl_fast_br_forwarding(*pskb) == RTL_FAST_BR_SUCCESS)) {
		ret = RTL_PS_HOOKS_RETURN;
	} else
	#endif /* CONFIG_RTL_FAST_BRIDGE */

	#ifdef CONFIG_RTL_WLAN_DOS_FILTER
	if (wlan_dos_filter_enabled
		&& filter_dos_wlan_enter(pskb) == NET_RX_DROP) {
		ret = RTL_PS_HOOKS_RETURN;
	} else
	#endif /* CONFIG_RTL_WLAN_DOS_FILTER */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	if (FastPath_Enter(pskb) == NET_RX_DROP) {
		ret = RTL_PS_HOOKS_RETURN;
	} else
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	{
		ret = RTL_PS_HOOKS_CONTINUE;
	}

	#if (LINUX_VERSION_CODE>=KERNEL_VERSION(3,10,0)) && defined(CONFIG_RTL_VLAN_8021Q)
	if (linux_vlan_enable && ret != RTL_PS_HOOKS_RETURN &&
		(*pskb) != NULL && saved_vlan_tag[0] != 0) {
		(*pskb)->dev = orig_dev;
		ret = rtl_fastpath_vlan_put_tag(*pskb, saved_vlan_tag);
		if (ret < 0)
			return RTL_PS_HOOKS_RETURN;
	}
	#endif /* version >= 3.10.0 && CONFIG_RTL_VLAN_8021Q */

	return ret;
}

int32 rtl_br_dev_queue_push_xmit_before_xmit_hooks(struct sk_buff *skb)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_FAST_BRIDGE)
	rtl_fb_add_br_entry(skb);
	#endif /* CONFIG_RTL_FAST_BRIDGE */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_forced_gc_hooks(struct neigh_table *tbl, struct neighbour *n)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	syn_asic_arp(n, 0);
	#endif /* (CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_flush_dev_hooks(
	struct neigh_table *tbl,
	struct net_device *dev,
	struct neighbour *n)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	if (n->nud_state & NUD_VALID) {
		#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
		syn_asic_arp(n, 0);
		#endif /* (CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		#ifdef CONFIG_FAST_PATH_MODULE
		if (FastPath_hook7 != NULL) {
			FastPath_hook7(*(u32 *)n->primary_key);
		}
		#else
		rtk_delArp(*(u32 *)n->primary_key);
		#endif /* CONFIG_FAST_PATH_MODULE */
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	}
	#endif /* (CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_destroy_hooks(struct neighbour *n)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	syn_asic_arp(n, 0);
	#endif /* (CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_connect_hooks(struct neighbour *neigh)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	if (neigh->nud_state & NUD_REACHABLE) {
		syn_asic_arp(neigh, 1);
	}
	#endif /* (CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_update_hooks(
	struct neighbour *neigh,
	const u8 *lladdr,
	uint8 old)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	if (old & NUD_VALID) {
		#ifdef CONFIG_FAST_PATH_MODULE
		if (FastPath_hook8 != NULL) {
			FastPath_hook8(*(u32 *)neigh->primary_key,
				(ether_addr_t *)lladdr, ARP_NONE);
		}
		#else
		rtk_modifyArp(*(u32 *)neigh->primary_key,
			(ether_addr_t *)lladdr, ARP_NONE);
		#endif /* CONFIG_FAST_PATH_MODULE */
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	syn_asic_arp(neigh, 1);
	#endif /* (CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_update_post_hooks(
	struct neighbour *neigh,
	const u8 *lladdr,
	uint8 old)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	if ((neigh->nud_state & NUD_VALID) && !(old & NUD_VALID)) {
		#if defined(CONFIG_FAST_PATH_MODULE)
		if (FastPath_hook5 != NULL) {
			FastPath_hook5(*(u32 *)neigh->primary_key,
				(ether_addr_t *)neigh->ha, ARP_NONE);
		}
		#else
		rtk_addArp(*(u32 *)neigh->primary_key,
			(ether_addr_t *)neigh->ha, ARP_NONE);
		#endif /* CONFIG_FAST_PATH_MODULE */
	} else if ((old & NUD_VALID) && !(neigh->nud_state & NUD_VALID)) {
		#if defined(CONFIG_FAST_PATH_MODULE)
		if (FastPath_hook7 != NULL) {
			FastPath_hook7(*(u32 *)neigh->primary_key);
		}
		#else
		rtk_delArp(*(u32 *)neigh->primary_key);
		#endif /* CONFIG_FAST_PATH_MODULE */
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

	return RTL_PS_HOOKS_CONTINUE;
}

static __inline__ int neigh_max_probes(
	struct neighbour *n)
{
	struct neigh_parms *p = n->parms;

	#if defined(CONFIG_RTD_1295_HWNAT)
	return NEIGH_VAR(p, UCAST_PROBES) + NEIGH_VAR(p, APP_PROBES) +
		(n->nud_state & NUD_PROBE ? NEIGH_VAR(p, MCAST_REPROBES) :
		NEIGH_VAR(p, MCAST_PROBES));
	#else /* CONFIG_RTD_1295_HWNAT */
	return (n->nud_state & NUD_PROBE ?
		p->ucast_probes :
		p->ucast_probes + p->app_probes + p->mcast_probes);
	#endif /* CONFIG_RTD_1295_HWNAT */
}

int32 rtl_neigh_periodic_timer_hooks(
	struct neighbour *n,
	unsigned int refresh)
{
	int ret;

	#if defined(RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT)
	int tval = 0;
	#endif /* RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT */

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	if (!(n->nud_state & NUD_VALID))
		return RTL_PS_HOOKS_CONTINUE;

	ret = RTL_PS_HOOKS_CONTINUE;
	#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	if (n->tbl && (n->tbl->family == AF_INET6)
		&& (n->tbl->key_len == sizeof(inv6_addr_t))) {
		if (rtl8198c_ipv6ArpSync(*((inv6_addr_t *)n->primary_key),
				refresh) > 0) {
			n->used = jiffies;
			n->dead = 0;
			ret = RTL_PS_HOOKS_BREAK;
		}
	} else if (n->tbl && (n->tbl->family == AF_INET)
		&& (n->tbl->key_len == 4))
	#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	{
		#if defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)
		#if defined(RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT)
		if (rtl865x_check_hw_nat_by_ip(htonl(*((u32 *)n->primary_key))) == SUCCESS)
			tval = rtl865x_arpSync(htonl(*((u32 *)n->primary_key)), 1);
		else
			tval = rtl865x_arpSync(htonl(*((u32 *)n->primary_key)), refresh);

		if (tval > 0)
		#else
		if (rtl865x_arpSync(htonl(*((u32 *)n->primary_key)), refresh) > 0)
		#endif /* RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT */
		{
			n->used = jiffies;
			n->dead = 0;
			ret = RTL_PS_HOOKS_BREAK;
		} else
		#endif /* CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3 */

		{
			#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
			#ifdef CONFIG_FAST_PATH_MODULE
			if (FastPath_hook7 != NULL) {
				FastPath_hook7(*(u32 *)n->primary_key);
			}
			#else
			rtk_delArp(*(u32 *)n->primary_key);
			#endif /* CONFIG_FAST_PATH_MODULE */
			#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
		}
	}

	return ret;
}

int32 rtl_neigh_timer_handler_pre_update_hooks(
	struct neighbour *neigh,
	unsigned state)
{
	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) || defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	int32 tval;
	#endif /* ( CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	if (state & NUD_REACHABLE) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if (neigh->nud_state & NUD_VALID) {
			#ifdef CONFIG_FAST_PATH_MODULE
			if (FastPath_hook7 != NULL) {
				FastPath_hook7(*(u32 *)neigh->primary_key);
			}
			#else
			rtk_delArp(*(u32 *)neigh->primary_key);
			#endif /* CONFIG_FAST_PATH_MODULE */
		}
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

		#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT) && !defined(CONFIG_RTL_IPV6READYLOGO)
		if (neigh->tbl && (neigh->tbl->family == AF_INET6)
			&& (neigh->tbl->key_len == sizeof(inv6_addr_t))) {
			tval = rtl8198c_ipv6ArpSync(*((inv6_addr_t *)
					neigh->primary_key), 0);
			if (tval > 0)
				neigh->confirmed = jiffies;
		} else if (neigh->tbl && (neigh->tbl->family == AF_INET)
			&& (neigh->tbl->key_len == 4))
		#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT && !CONFIG_RTL_IPV6READYLOGO */
		{
			#if defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)
			#if defined(RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT)
			if (rtl865x_check_hw_nat_by_ip(htonl(*((u32 *)neigh->primary_key))) == SUCCESS)
				tval = rtl865x_arpSync(htonl(*((u32 *)neigh->primary_key)), 1);
			else
			#endif /* RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT */
				tval = rtl865x_arpSync(htonl(*((u32 *)neigh->primary_key)), 0);
			if (tval > 0)
				neigh->confirmed = jiffies;

			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			printk("%s:%d: ip:%u.%u.%u.%u, mac:%x:%x:%x:%x:%x:%x, tval is %d\n",
				__FUNCTION__, __LINE__,
				NIPQUAD(htonl(*((u32 *)neigh->primary_key))),
				neigh->ha[0], neigh->ha[1],
				neigh->ha[2], neigh->ha[3],
				neigh->ha[4], neigh->ha[5],tval);
			#endif /* CONFIG_HARDWARE_NAT_DEBUG */
			#endif /* CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3 */
		}

	} else if (state & NUD_DELAY) {
		#if defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT) && !defined(CONFIG_RTL_IPV6READYLOGO)
		if (neigh->tbl && (neigh->tbl->family == AF_INET6)
			&& (neigh->tbl->key_len == sizeof(inv6_addr_t))) {
			tval = rtl8198c_ipv6ArpSync(*((inv6_addr_t *)
					neigh->primary_key), 0);
			if (tval > 0)
				neigh->confirmed = jiffies;
		} else if (neigh->tbl && (neigh->tbl->family == AF_INET)
			&& (neigh->tbl->key_len == 4))
		#endif /* CONFIG_RTL_HARDWARE_IPV6_SUPPORT && !CONFIG_RTL_IPV6READYLOGO */
		{
			#if defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)
			#if defined(RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT)
			if (rtl865x_check_hw_nat_by_ip(htonl(*((u32 *)neigh->primary_key))) == SUCCESS)
				tval = rtl865x_arpSync(htonl(*((u32 *)neigh->primary_key)), 1);
			else
			#endif /* RTL_REFRESH_HW_L2_ENTRY_DECIDE_BY_HW_NAT */
				tval = rtl865x_arpSync(htonl(*((u32 *)neigh->primary_key)), 0);
			if (tval > 0)
				neigh->confirmed = jiffies;

			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			printk("%s:%d: ip:%u.%u.%u.%u, mac:%x:%x:%x:%x:%x:%x, tval is %d",
				__FUNCTION__, __LINE__,
				NIPQUAD(htonl(*((u32 *)neigh->primary_key))),
				neigh->ha[0], neigh->ha[1],
				neigh->ha[2], neigh->ha[3],
				neigh->ha[4], neigh->ha[5],tval);
			#endif /* CONFIG_HARDWARE_NAT_DEBUG */
			#endif /* CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3 */
		}
	}

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_timer_handler_during_update_hooks(
	struct neighbour *neigh,
	unsigned state)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if (defined(CONFIG_RTL_HARDWARE_NAT) && defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L3)) ||defined(CONFIG_RTL_HARDWARE_IPV6_SUPPORT)
	if ((neigh->nud_state & (NUD_INCOMPLETE | NUD_PROBE)) &&
		atomic_read(&neigh->probes) >= neigh_max_probes(neigh)) {
		if (neigh->nud_state & NUD_VALID) {
			/*delete asic arp entry */
			syn_asic_arp(neigh, 0);
			#ifdef CONFIG_HARDWARE_NAT_DEBUG
			printk("%s:%d: ip:%u.%u.%u.%u, mac:%x:%x:%x:%x:%x:%x\n",
				__FUNCTION__, __LINE__,
				NIPQUAD(htonl(*((u32 *)neigh->primary_key))),
				neigh->ha[0], neigh->ha[1], neigh->ha[2],
				neigh->ha[3], neigh->ha[4], neigh->ha[5]);
			#endif /* CONFIG_HARDWARE_NAT_DEBUG */
		}
	}
	#endif /* ( CONFIG_RTL_HARDWARE_NAT && CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L3) || CONFIG_RTL_HARDWARE_IPV6_SUPPORT */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_timer_handler_post_update_hooks(
	struct neighbour *neigh,
	unsigned state)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	if ((neigh->nud_state & NUD_VALID) && !(state & NUD_VALID)) {
		#ifdef CONFIG_FAST_PATH_MODULE
		if (FastPath_hook5 != NULL) {
			FastPath_hook5(*(u32 *)neigh->primary_key,
				(ether_addr_t *)neigh->ha, ARP_NONE);
		}
		#else
		rtk_addArp(*(u32 *)neigh->primary_key,
			(ether_addr_t *)neigh->ha, ARP_NONE);
		#endif /* CONFIG_FAST_PATH_MODULE */
	} else if ((state & NUD_VALID) && !(neigh->nud_state & NUD_VALID)) {
		#ifdef CONFIG_FAST_PATH_MODULE
		if (FastPath_hook7 != NULL) {
			FastPath_hook7(*(u32 *)neigh->primary_key);
		}
		#else
		rtk_delArp(*(u32 *)neigh->primary_key);
		#endif /* CONFIG_FAST_PATH_MODULE */
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl___neigh_event_send_pre_hooks(
	struct neighbour *neigh,
	struct sk_buff *skb)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
	if (!(neigh->nud_state & (NUD_STALE | NUD_INCOMPLETE))) {
		if (neigh->nud_state & NUD_VALID) {
			#if defined(CONFIG_FAST_PATH_MODULE)
			if (FastPath_hook7 != NULL) {
				FastPath_hook7(*(u32 *)neigh->primary_key);
			}
			#else
			rtk_delArp(*(u32 *)neigh->primary_key);
			#endif /* CONFIG_FAST_PATH_MODULE */
		}
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl___neigh_event_send_post_hooks(
	struct neighbour *neigh,
	struct sk_buff *skb)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_neigh_init_hooks(void)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_MULTIPLE_WAN) || defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN)
	rtl_set_callback_for_ps_arp(rtl_get_ps_arp_mapping);
	#endif /* CONFIG_RTL_MULTIPLE_WAN || CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN */
	return RTL_PS_HOOKS_CONTINUE;
}

#if defined(CONFIG_BRIDGE)
int32 rtl___br_fdb_get_timeout_hooks(
	struct net_bridge *br,
	struct net_bridge_fdb_entry *fdb,
	const unsigned char *addr)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_FASTBRIDGE)
	if (rtl_br_fdb_time_update((void *)br, (void *)fdb, addr) == FAILED) {
		return RTL_PS_HOOKS_BREAK;
	}
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_FASTBRIDGE */

	return RTL_PS_HOOKS_CONTINUE;
}
#endif /* CONFIG_BRIDGE */

int32 rtl_masq_device_event_hooks(
	struct notifier_block *this,
	struct net_device *dev,
	unsigned long event)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	if (dev != NULL && dev->ip_ptr != NULL)
		rtl_update_ip_tables(dev->name, event,
			(struct in_ifaddr *)(((struct in_device *)(dev->ip_ptr))->ifa_list));
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_masq_device_event_hooks);

int32 rtl_masq_inet_event_hooks(
	struct notifier_block *this,
	unsigned long event,
	void *ptr)
{
	#if defined(CONFIG_RTL_HARDWARE_NAT)
	struct in_ifaddr *ina;

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	ina = (struct in_ifaddr *)ptr;

	#ifdef CONFIG_HARDWARE_NAT_DEBUG
	/*2007-12-19 */
	printk("%s:%d\n", __FUNCTION__, __LINE__);
	printk("ptr->ifa_dev->dev->name is %s\n", ina->ifa_dev->dev->name);
	#endif /* CONFIG_HARDWARE_NAT_DEBUG */

	rtl_update_ip_tables(ina->ifa_dev->dev->name, event, ina);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_masq_inet_event_hooks);

#ifndef CONFIG_RTL_AP_PACKAGE
int32 rtl_translate_table_hooks(
	const char *name,
	unsigned int valid_hooks,
	struct xt_table_info *newinfo,
	void *entry0,
	unsigned int size,
	unsigned int number,
	const unsigned int *hook_entries,
	const unsigned int *underflows)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	//hyking:check masquerade and add ip
	if (strcmp(name, "nat") == 0) {
		#if defined(CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN)
		rtl_sync_extIp_to_masq_info(name, valid_hooks, newinfo, entry0,
			size, number, hook_entries, underflows);
		rtl_del_extIp_not_exist_in_ps();
		#else
		rtl_flush_extern_ip();
		rtl_init_masq_info();
		#endif /* CONFIG_RTL_REDIRECT_ACL_SUPPORT_FOR_ISP_MULTI_WAN */
		rtl_check_for_extern_ip(name, valid_hooks, newinfo, entry0,
			size, number, hook_entries, underflows);
	}
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_translate_table_hooks);
#endif

int32 rtl_ip_tables_init_hooks(void)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_init_masq_info();
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_ip_tables_init_hooks);

int32 rtl_ip_vs_conn_expire_hooks1(struct ip_vs_conn *cp)
{
	int32 ret;

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	ret = RTL_PS_HOOKS_CONTINUE;
	#if defined(CONFIG_RTL_HARDWARE_NAT)
	if (FAILED == rtl_ip_vs_conn_expire_check(cp)) {
		ret = RTL_PS_HOOKS_RETURN;
	}
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return ret;
}

EXPORT_SYMBOL(rtl_ip_vs_conn_expire_hooks1);

int32 rtl_ip_vs_conn_expire_hooks2(struct ip_vs_conn *cp)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_ip_vs_conn_expire_check_delete(cp);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

EXPORT_SYMBOL(rtl_ip_vs_conn_expire_hooks2);

#if defined(CONFIG_IP_VS_PROTO_TCP)
int32 rtl_tcp_state_transition_hooks(
	struct ip_vs_conn *cp,
	int direction,
	const struct sk_buff *skb,
	struct ip_vs_protocol *pp)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_tcp_state_transition_check(cp, direction, skb, pp);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}
#endif /* CONFIG_IP_VS_PROTO_TCP */

#if defined(CONFIG_IP_VS_PROTO_UDP)
int32 rtl_udp_state_transition_hooks(
	struct ip_vs_conn *cp,
	int direction,
	const struct sk_buff *skb,
	struct ip_vs_protocol *pp)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_udp_state_transition_check(cp, direction, skb, pp);
	#endif /* CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}
#endif /* CONFIG_IP_VS_PROTO_UDP */

#ifdef CONFIG_PROC_FS
int rtl_ct_seq_show_hooks(
	struct seq_file *s,
	struct nf_conn *ct)
{
	//hyking add for hw use
	#if defined(CONFIG_RTL_HARDWARE_NAT)
	struct nf_conn_nat *nat;
	char *state[2] = { "Hardware", "software" };

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	nat = nfct_nat(ct);
	if (nat == NULL) {
	#if defined(CONFIG_RTD_1295_HWNAT)
		seq_printf(s, "[%s] ", state[1]);
	#else /* CONFIG_RTD_1295_HWNAT */
		if (seq_printf(s, "[%s] ", state[1]) != 0)
			return RTL_PS_HOOKS_BREAK;
	#endif /* CONFIG_RTD_1295_HWNAT */

		return RTL_PS_HOOKS_CONTINUE;
	}
	#if defined(CONFIG_RTD_1295_HWNAT)
	seq_printf(s, "[%s] ", nat->hw_acc ? state[0] : state[1]);
	#else /* CONFIG_RTD_1295_HWNAT */
	if (seq_printf(s, "[%s] ", nat->hw_acc ? state[0] : state[1]) != 0)
		return RTL_PS_HOOKS_BREAK;
	#endif /* CONFIG_RTD_1295_HWNAT */
	#endif /* CONFIG_RTL_HARDWARE_NAT */
	return RTL_PS_HOOKS_CONTINUE;
}
#endif /* CONFIG_PROC_FS */

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
//When do garbage collection and dst cache overflow, should not do garbage collection for rtl_gc_overflow_timout(default 3s)
//Note: rtl_gc_overflow_timout can be modified by user via /proc/gc_overflow_timout
unsigned long rtl_gc_overflow_timout = 3 * HZ;
static unsigned long rtl_gc_overflow_timeout = 0;
#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

//Before do garbage collection, do rtl_dst_alloc_gc_pre_check_hooks
int32 rtl_dst_alloc_gc_pre_check_hooks(struct dst_ops *ops)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	if ((rtl_gc_overflow_timeout > 0)
		&& time_after_eq(rtl_gc_overflow_timeout, jiffies)) {
		return RTL_PS_HOOKS_RETURN;
	}
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

	return RTL_PS_HOOKS_CONTINUE;
}

//After do garbage collection and dst cache overflow, do rtl_dst_alloc_gc_post_check1_hooks
int32 rtl_dst_alloc_gc_post_check1_hooks(struct dst_ops *ops)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	rtl_gc_overflow_timeout = jiffies + rtl_gc_overflow_timout;
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

	return RTL_PS_HOOKS_CONTINUE;
}

//After [do garbage collection and dst cache not overflow] and dst alloc success, do rtl_dst_alloc_gc_post_check2_hooks
int32 rtl_dst_alloc_gc_post_check2_hooks(
	struct dst_ops *ops,
	struct dst_entry *dst)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
	rtl_gc_overflow_timeout = 0;
	#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

	return RTL_PS_HOOKS_CONTINUE;
}

#if defined(CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW)
// hooks in clean_from_lists at rtl_nf_connGC.c
int32 clean_from_lists_hooks(
	struct nf_conn *ct,
	struct net *net)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
	rtl_delConnCache(ct);
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT */
	#ifdef CONFIG_RTL_FAST_IPV6
	if (fast_ipv6_fw)
		rtl_DelV6ConnCache(ct);
	#endif /* CONFIG_RTL_FAST_IPV6 */

	return RTL_PS_HOOKS_CONTINUE;
}

// hooks in __nf_ct_refresh_acct_proto at rtl_nf_connGC.c
int32 __nf_ct_refresh_acct_proto_hooks(
	struct nf_conn *ct,
	enum ip_conntrack_info ctinfo,
	const struct sk_buff *skb,
	int do_acct,
	int *event)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#ifdef CONFIG_IP_NF_CT_ACCT
	if (do_acct) {
		ct->counters[CTINFO2DIR(ctinfo)].packets++;
		ct->counters[CTINFO2DIR(ctinfo)].bytes +=
			ntohs(skb->nh.iph->tot_len);
		if ((ct->counters[CTINFO2DIR(ctinfo)].packets & 0x80000000)
			|| (ct->counters[CTINFO2DIR(ctinfo)].bytes & 0x80000000))
			*event |= IPCT_COUNTER_FILLING;
	}
	#endif /* CONFIG_IP_NF_CT_ACCT */

	return RTL_PS_HOOKS_CONTINUE;
}

// hooks1 in __drop_one_conntrack_process at rtl_nf_connGC.c
int32 __drop_one_conntrack_process_hooks1(
	struct nf_conn *ct,
	int dropPrioIdx,
	int factor,
	int checkFlags,
	int tcpUdpState)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
	if (checkFlags == TRUE
		&& drop_priority[dropPrioIdx].state == tcpUdpState) {
		#if defined(CONFIG_RTL_IPTABLES_FAST_PATH)
		if (FAILED == rtl_fpTimer_update((void *)ct)) {
			#if defined(CONFIG_RTD_1295_HWNAT)
			local_bh_enable();
			#else /* !CONFIG_RTD_1295_HWNAT */
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
			spin_unlock_bh(&nf_conntrack_lock);
			#else
			read_unlock_bh(&nf_conntrack_lock);
			#endif /* version >= 3.10.0 */
			#endif /* CONFIG_RTD_1295_HWNAT */
			rtl_death_action((void *)ct);
			return RTL_PS_HOOKS_RETURN;
		}
		#endif /* CONFIG_RTL_IPTABLES_FAST_PATH */

		#if defined(CONFIG_RTL_HARDWARE_NAT)
		if (FAILED == rtl_hwnat_timer_update(ct)) {
			#if defined(CONFIG_RTD_1295_HWNAT)
			local_bh_enable();
			#else /* !CONFIG_RTD_1295_HWNAT */
			#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
			spin_unlock_bh(&nf_conntrack_lock);
			#else
			read_unlock_bh(&nf_conntrack_lock);
			#endif /* version >= 3.10.0 */
			#endif /* CONFIG_RTD_1295_HWNAT */
			rtl_death_action((void *)ct);
			return RTL_PS_HOOKS_RETURN;
		}
		#endif /* CONFIG_RTL_HARDWARE_NAT */
	}
	#if (HZ==100)
	if (((ct->timeout.expires - jiffies) >> (factor + 7)) <=
		drop_priority[dropPrioIdx].threshold)
	#elif (HZ==1000)
	if (((ct->timeout.expires - jiffies) >> (factor + 10)) <=
		drop_priority[dropPrioIdx].threshold)
	#else
	#error "Please Check the HZ defination."
	#endif
	{
		#if defined(CONFIG_RTD_1295_HWNAT)
		local_bh_enable();
		#else /* !CONFIG_RTD_1295_HWNAT */
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
		spin_unlock_bh(&nf_conntrack_lock);
		#else
		read_unlock_bh(&nf_conntrack_lock);
		#endif /* version >= 3.10.0 */
		#endif /* CONFIG_RTD_1295_HWNAT */
		rtl_death_action((void *)ct);
		return RTL_PS_HOOKS_RETURN;
	}

	return RTL_PS_HOOKS_CONTINUE;
	#else
	return RTL_PS_HOOKS_BREAK;
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT */
}

// hooks2 in __drop_one_conntrack_process at rtl_nf_connGC.c
int32 __drop_one_conntrack_process_hooks2(
	struct nf_conn *ct,
	int dropPrioIdx,
	int factor,
	int checkFlags,
	int tcpUdpState)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_IPTABLES_FAST_PATH) || defined(CONFIG_RTL_HARDWARE_NAT)
	add_timer(&ct->timeout);
	#endif /* CONFIG_RTL_IPTABLES_FAST_PATH || CONFIG_RTL_HARDWARE_NAT */

	return RTL_PS_HOOKS_CONTINUE;
}

// hooks in rtl_nf_conn_GC_init at rtl_nf_connGC.c
int32 rtl_nf_conn_GC_init_hooks(void)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_PROC_FS)
	gc_overflow_timout_proc_init();
	#endif /* CONFIG_PROC_FS */

	return RTL_PS_HOOKS_CONTINUE;
}
#endif /* CONFIG_RTL_NF_CONNTRACK_GARBAGE_NEW */

void rtl_update_ps_neigh_confirm(struct neighbour *neigh)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY2;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(RTL_UPDATE_PS_NEIGH_CONFIRM)
	unsigned long now = jiffies;

	/* avoid dirtying neighbour */
	/*jwj: As rtl_update_ps_neigh_confirm will be called in ip_finish_output2,
	 * so we do not need to add more lock protect. */
	if (neigh->confirmed != now)
		neigh->confirmed = now;
	#endif /* RTL_UPDATE_PS_NEIGH_CONFIRM */

	return;
}

#if defined(CONFIG_BRIDGE)
int32 rtl_fdb_create_hooks(
	struct net_bridge_fdb_entry *fdb,
	const unsigned char *addr)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
	#if defined (CONFIG_RTL865X_LANPORT_RESTRICTION)
	if (fdb->is_static == 0) {
		rtl865x_addAuthFDBEntry_hooks(addr);
	}
	#else

	#if defined(CONFIG_RTL_EXT_PORT_SUPPORT)
	if (extPortEnabled && (!memcmp(fdb->dst->dev->name, "wlan", 4))
		&& ((addr[0] & 0x1) == 0)) {
		rtl865x_addExtFDBEntry(addr, PKTHDR_EXTPORT_P3);
	} else
	#endif /* CONFIG_RTL_EXT_PORT_SUPPORT */
		rtl865x_addFDBEntry(addr);
	#endif /* CONFIG_RTL865X_LANPORT_RESTRICTION */
	#endif /* CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L2 */

	return RTL_PS_HOOKS_CONTINUE;

}

int32 rtl_fdb_delete_hooks(struct net_bridge_fdb_entry *f)
{
	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
	#if defined (CONFIG_RTL865X_LANPORT_RESTRICTION)
	rtl865x_delAuthLanFDBEntry(RTL865x_L2_TYPEII, f->addr.addr);
	#else
	rtl865x_delLanFDBEntry(RTL865x_L2_TYPEII, f->addr.addr);
	#endif /* CONFIG_RTL865X_LANPORT_RESTRICTION */
	#endif /* CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L2 && CONFIG_RTL865X_SYNC_L2 */

	#if defined(CONFIG_RTL_FASTBRIDGE)
	rtl_fb_del_entry(f->addr.addr);
	#endif /* CONFIG_RTL_FASTBRIDGE */
	return RTL_PS_HOOKS_CONTINUE;
}

int32 rtl_br_fdb_cleanup_hooks(
	struct net_bridge *br,
	struct net_bridge_fdb_entry *f,
	unsigned long delay)
{
	#if defined(CONFIG_RTL_FASTBRIDGE)
	unsigned long fb_aging;
	#endif
	#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2) && defined(CONFIG_RTL865X_SYNC_L2)
	int32 port_num;
	unsigned long hw_aging = 0;
	#endif /* CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L2 && CONFIG_RTL865X_SYNC_L2 */
	int ret;

	#if defined(CONFIG_RTD_1295_HWNAT)
	CHECK_IF_HWNAT_READY;
	#endif /* CONFIG_RTD_1295_HWNAT */

	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	if (time_after(f->updated, jiffies)) {
		printk("\nf->ageing_timer AFTER jiffies:addr is :%x,%x,%x,%x,%x,%x\n", f->addr.addr[0], f->addr.addr[1], f->addr.addr[2], f->addr.addr[3], f->addr.addr[4], f->addr.addr[5]);
		#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2) && defined(CONFIG_RTL865X_SYNC_L2)
		printk("time list:jiffies:%ld,hw_aging:%ld,f->ageing_timer:%ld\n",
			jiffies / HZ, hw_aging / HZ, f->updated / HZ);
		#else
		printk("time list:jiffies:%ld,f->ageing_timer:%ld\n",
			jiffies / HZ, f->updated / HZ);
		#endif /* CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L2 && CONFIG_RTL865X_SYNC_L2 */
		return RTL_PS_HOOKS_BREAK;
	}
	#else
	if (time_after(f->ageing_timer, jiffies)) {
		DEBUG_PRINT("\nf->ageing_timer AFTER jiffies:addr is :%x,%x,%x,%x,%x,%x\n",
			f->addr.addr[0], f->addr.addr[1], f->addr.addr[2],
			f->addr.addr[3], f->addr.addr[4], f->addr.addr[5]);
		DEBUG_PRINT("time list:jiffies:%ld,hw_aging:%ld,f->ageing_timer:%ld\n",
			jiffies / HZ, hw_aging / HZ, f->ageing_timer / HZ);
		return RTL_PS_HOOKS_BREAK;
	}
	#endif /* version >= 3.2.0 */

	#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2) && defined(CONFIG_RTL865X_SYNC_L2)
	port_num = -100;
	ret = rtl865x_arrangeFdbEntry(f->addr.addr, &port_num);

	switch (ret) {
	case RTL865X_FDBENTRY_450SEC:
		hw_aging = jiffies;
		break;
	case RTL865X_FDBENTRY_300SEC:
		hw_aging = jiffies - 150 * HZ;
		break;
	case RTL865X_FDBENTRY_150SEC:
		hw_aging = jiffies - 300 * HZ;
		break;
	case RTL865X_FDBENTRY_TIMEOUT:
	case FAILED:
	default:
		hw_aging = jiffies - 450 * HZ;
		break;
	}

	ret = 0;
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 2, 0)
	if (time_before_eq(f->updated, hw_aging)) {
		/*fresh f->ageing_timer */
		f->updated = hw_aging;
	}
	#else
	if (time_before_eq(f->ageing_timer, hw_aging)) {
		/*fresh f->ageing_timer */
		f->ageing_timer = hw_aging;
	}
	#endif /* version >= 3.2.0 */
	#endif /* CONFIG_RTL_LAYERED_DRIVER && CONFIG_RTL_LAYERED_DRIVER_L2 && CONFIG_RTL865X_SYNC_L2 */

	#if defined(CONFIG_RTL_FASTBRIDGE)
	fb_aging = rtl_fb_get_entry_lastused(f->addr.addr);
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
	if (time_before_eq(f->updated, fb_aging)) {
		f->updated = fb_aging;
	}
	#else
	if (time_before_eq(f->ageing_timer, fb_aging)) {
		f->ageing_timer = fb_aging;
	}
	#endif /* version >= 3.10.0 */
	#endif /* CONFIG_RTL_FASTBRIDGE */

	if (ret == 0) {
		return RTL_PS_HOOKS_CONTINUE;
	} else {
		return RTL_PS_HOOKS_BREAK;
	}
}

#endif /* CONFIG_BRIDGE */
