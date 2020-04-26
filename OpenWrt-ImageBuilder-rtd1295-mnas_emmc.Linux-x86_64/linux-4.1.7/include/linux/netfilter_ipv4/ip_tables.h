/*
 * 25-Jul-1998 Major changes to allow for ip chain table
 *
 * 3-Jan-2000 Named tables to allow packet selection for different uses.
 */

/*
 * 	Format of an IP firewall descriptor
 *
 * 	src, dst, src_mask, dst_mask are always stored in network byte order.
 * 	flags are stored in host byte order (of course).
 * 	Port numbers are stored in HOST byte order.
 */
#ifndef _IPTABLES_H
#define _IPTABLES_H

#include <linux/if.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/skbuff.h>

#include <linux/init.h>
#include <uapi/linux/netfilter_ipv4/ip_tables.h>

#if defined(CONFIG_RTL_IPTABLES_RULE_2_ACL)
#define IPT_MATCH_NUMBER(e) \
({					\
	unsigned int __i;		\
	int __ret = 0;			\
	struct xt_entry_match *__match;	\
	for(__i = sizeof(struct ipt_entry);	\
		__i < (e)->target_offset;		\
		__i += __match->u.match_size)	\
		{		\
		__match = (void*)(e) + __i;		\
		__ret++;		\
		}		\
		__ret;	\
})

#define RTL865X_CHAINLIST_NUMBER_PER_TBL 5
#define RTL865x_CHAINLIST_PRIORITY_LEVEL_0 0
#define RTL865x_CHAINLIST_PRIORITY_LEVEL_1 1
#define RTL865x_CHAINLIST_PRIORITY_LEVEL_2 2
#define RTL865x_CHAINLIST_PRIORITY_LEVEL_3 3
#define RTL865x_CHAINLIST_PRIORITY_LEVEL_4 4

typedef struct _rtl865x_iptRule2Acl_tbl_
{
	struct list_head list;
	/*chain list priority: 0 > 1 > ...> 4*/
	struct list_head chainList[RTL865X_CHAINLIST_NUMBER_PER_TBL]; /*chain list head in this table*/

	int32_t priority; /*table priority*/
	char tblName[32];
}rtl865x_iptRule2Acl_tbl;
//extern struct list_head rtl865x_iptRule2Acl_tbl_list;
//extern struct list_head rtl865x_iptRule2Acl_def_rule_list;
//extern struct list_head match_to_acl_rule_list;
//extern struct list_head def_rule_list;
#endif


extern void ipt_init(void) __init;

extern struct xt_table *ipt_register_table(struct net *net,
					   const struct xt_table *table,
					   const struct ipt_replace *repl);
extern void ipt_unregister_table(struct net *net, struct xt_table *table);

/* Standard entry. */
struct ipt_standard {
	struct ipt_entry entry;
	struct xt_standard_target target;
};

struct ipt_error {
	struct ipt_entry entry;
	struct xt_error_target target;
};

#define IPT_ENTRY_INIT(__size)						       \
{									       \
	.target_offset	= sizeof(struct ipt_entry),			       \
	.next_offset	= (__size),					       \
}

#define IPT_STANDARD_INIT(__verdict)					       \
{									       \
	.entry		= IPT_ENTRY_INIT(sizeof(struct ipt_standard)),	       \
	.target		= XT_TARGET_INIT(XT_STANDARD_TARGET,		       \
					 sizeof(struct xt_standard_target)),   \
	.target.verdict	= -(__verdict) - 1,				       \
}

#define IPT_ERROR_INIT							       \
{									       \
	.entry		= IPT_ENTRY_INIT(sizeof(struct ipt_error)),	       \
	.target		= XT_TARGET_INIT(XT_ERROR_TARGET,		       \
					 sizeof(struct xt_error_target)),      \
	.target.errorname = "ERROR",					       \
}

extern void *ipt_alloc_initial_table(const struct xt_table *);
extern unsigned int ipt_do_table(struct sk_buff *skb,
				 unsigned int hook,
				 const struct nf_hook_state *state,
				 struct xt_table *table);

#ifdef CONFIG_COMPAT
#include <net/compat.h>

struct compat_ipt_entry {
	struct ipt_ip ip;
	compat_uint_t nfcache;
	__u16 target_offset;
	__u16 next_offset;
	compat_uint_t comefrom;
	struct compat_xt_counters counters;
	unsigned char elems[0];
};

/* Helper functions */
static inline struct xt_entry_target *
compat_ipt_get_target(struct compat_ipt_entry *e)
{
	return (void *)e + e->target_offset;
}

#endif /* CONFIG_COMPAT */

#if defined (CONFIG_RTL_IGMP_SNOOPING) && defined (CONFIG_NETFILTER)
extern unsigned int (*IgmpRxFilter_Hook)(struct sk_buff *skb,
	     unsigned int hook,
	     const struct net_device *in,
	     const struct net_device *out,
	     struct xt_table *table);
#endif

#endif /* _IPTABLES_H */
