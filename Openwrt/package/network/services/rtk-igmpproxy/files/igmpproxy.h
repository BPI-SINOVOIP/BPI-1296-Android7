#ifndef _IGMPPROXY_H_
#define _IGMPPROXY_H_
#if defined(CONFIG_IGMPPROXY_MULTIWAN) || defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
#include "timeout.h"
#endif


#if defined(CONFIG_IGMPPROXY_MULTIWAN) || defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
#define  MAXWAN 4
#else 
#define  MAXWAN 1
#endif

#define VERSION_STR	"v1.2"

#define USE_STATIC_ENTRY_BUFFER 1
#define MAX_MFCT_ENTRY 128
#define MAX_MBR_ENTRY 512

#define DISPLAY_BANNER \
	printf("\nIGMP Proxy %s (%s).\n\n", VERSION_STR, BUILT_TIME)
	
// Enable to do the group-specific query periodically
#define PERIODICAL_QUERY
// Send Group-specific query periodically (keepalive polling)
#define PERIODICAL_SPECIFIC_QUERY
// Enable to maintain the group members in order to do immediately leave
#define KEEP_GROUP_MEMBER
//#define CONFIG_CHECK_MULTICASTROUTE
/* IGMP timer and default values */
#define STARTUP_GENERAL_QUERY_INTERVAL			3// second
#define PERIODICAL_GENERAL_QUERY_INTERVAL		30
#define GENERAL_QUERY_INTERVAL		15	// second

#define LINK_CHANGE_QUERY_INTERVAL					3
#define LINK_CHANGE_QUERY_TIMES						3

#define LAST_MEMBER_QUERY_INTERVAL	1	// second
#define LAST_MEMBER_QUERY_COUNT		2
// Kaohj --- group-specific query in periodical
#ifdef PERIODICAL_QUERY
#if defined (CONFIG_QUERIER_SELECTION)
#define MEMBER_QUERY_INTERVAL		150	// second
#else
#define MEMBER_QUERY_INTERVAL		(PERIODICAL_GENERAL_QUERY_INTERVAL*2+10)	// second
#endif
#define MEMBER_QUERY_COUNT		3
#endif

#if defined (CONFIG_QUERIER_SELECTION)
#define QUERIER_IS_ME 1
#define QUERIER_IS_OTHER 0
#define DEFAULT_OTHER_QUERIER_PRESENT_INTERVAL 255
#endif

/* IGMP group address */
#define ALL_SYSTEMS		htonl(0xE0000001)	// General Query - 224.0.0.1
#define ALL_ROUTERS		htonl(0xE0000002)	// Leave - 224.0.0.2	
#define ALL_ROUTERS_V3	htonl(0xE0000016)	// Leave - 224.0.0.22
#define ALL_PRINTER		htonl(0xEFFFFFFA)	// notify all printer - 239.255.255.250
#define CLASS_D_MASK	0xE0000000		// the mask that defines IP Class D
#define IPMULTI_MASK	0x007FFFFF		// to get the low-order 23 bits

/* header length */
#define MIN_IP_HEADER_LEN	20
#define IGMP_MINLEN			8

#ifdef CONFIG_CHECK_MULTICASTROUTE
/* Keep this in sync with /usr/src/linux/include/linux/route.h */
#define RTF_UP			0x0001          /* route usable                 */
#define RTF_GATEWAY		0x0002          /* destination is a gateway     */
#define	RTF_HOST	0x0004		/* host entry (net otherwise)	*/
#endif
static char *routefile = "/proc/net/route";

#define PERIODICAL_TIMER_TPYE 		1
#define LIMIT_RETRY_TIMER_TYPE 		2
#define BRCTL_SET_IGMPPROXY_PID 200

// Send Group-specific query on leave (fast leave)
//#define LEAVE_SPECIFIC_QUERY
// Maintain the group members in order to do immediately leave
#define KEEP_GROUP_MEMBER
// Send IGMP General Query periodically
#define SEND_GENERAL_QUERY

#define TIMER_GENERAL_QUERY		1
#define TIMER_DELAY_QUERY		2

/* IGMP group address */
#if 0 // defined before
#define ALL_SYSTEMS			htonl(0xE0000001)	// General Query - 224.0.0.1
#define ALL_ROUTERS			htonl(0xE0000002)	// Leave - 224.0.0.2	
#define ALL_ROUTERS_V3			htonl(0xE0000016)	// Leave - 224.0.0.22
#define ALL_PRINTER			htonl(0xEFFFFFFA)	// notify all printer - 239.255.255.250
#define CLASS_D_MASK			0xE0000000		// the mask that defines IP Class D
#define IPMULTI_MASK			0x007FFFFF		// to get the low-order 23 bits
#endif

/* header length */
#define MIN_IP_HEADER_LEN		20
#define IGMP_MINLEN			8

/* IGMP v3 type */
#define IGMP_HOST_V3_MEMBERSHIP_REPORT	0x22

#if defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
#define ADD_WAN_IF 0x1000000
#define DEL_WAN_IF 0x2000000
#define WAN_PIF_1  0x1    //eth1.1
#define WAN_PIF_2  0x2    //eth1.2
#define WAN_PIF_3  0x4    //eth1.3
#define WAN_PIF_4  0x8    //eth1.4
#define WAN_VIF_1  0x100  //ppp1
#define WAN_VIF_2  0x200  //ppp2
#define WAN_VIF_3  0x400  //ppp3
#define WAN_VIF_4  0x800  //ppp4
#define NEED_DEL_VIF 0x10000
#endif
// group member entry
struct mbr_entry {
	struct mbr_entry	*next;
	__u32			user_addr;
};

struct igmp_timer {
	int		type;			// timer type
	__u32		timerInterval;
	__u32		retry_left;		// retry counts left
	struct callout	ch;
#ifdef CONFIG_IGMPV3_SUPPORT
	unsigned int	lefttime;
#endif /*CONFIG_IGMPV3_SUPPORT*/
};

#ifdef CONFIG_IGMPV3_SUPPORT
struct src_entry {
	struct src_entry	*next;
	__u32			srcaddr;
	struct igmp_timer	timer;
};
#endif /*CONFIG_IGMPV3_SUPPORT*/

struct mcft_entry {
	struct mcft_entry 	*next;
	__u32			grp_addr;
	__u32			user_count;	// group member count
	struct mbr_entry 	*grp_mbr;
	struct igmp_timer	timer;
#ifdef CONFIG_IGMPV3_SUPPORT
	int			igmp_ver;
	int			filter_mode;
	struct src_entry	*srclist;
	int			mrt_state; //0:disable, 1: enable all sources
#endif /*CONFIG_IGMPV3_SUPPORT*/
};


#ifdef CONFIG_IGMPV3_SUPPORT
/*IGMP version*/
#define IGMP_VER_2		2
#define IGMP_VER_3		3
extern int igmpv3_accept(int recvlen, struct IfDesc *dp);
extern void igmpv3_timer(void);
#endif /*CONFIG_IGMPV3_SUPPORT*/

extern char igmp_down_if_name[IFNAMSIZ];
extern char igmp_down_if_idx;

#if defined(CONFIG_IGMPPROXY_MULTIWAN) || defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
extern char igmp_up_if_name[MAXWAN][IFNAMSIZ];
#else
extern char igmp_up_if_name[IFNAMSIZ];
#endif

#if defined(CONFIG_IGMPPROXY_MULTIWAN) || defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
extern char igmp_up_if_idx[MAXWAN];
#if defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
extern struct IfDesc IfDescVc[MAX_IF];
#endif
#else
extern char igmp_up_if_idx;
#endif

#if defined(CONFIG_IGMPPROXY_MULTIWAN) || defined(CONFIG_ISP_IGMPPROXY_MULTIWAN)
extern int igmp_up_if_num;
#endif

extern char *recv_buf;
extern char *send_buf;

extern struct mcft_entry *mcpq;

extern int igmp_query(__u32 dst, __u32 grp,__u8 mrt);

extern struct mcft_entry * add_mcft(__u32 grp_addr, __u32 src_addr);
extern int del_mcft(__u32 grp_addr);
extern int chk_mcft(__u32 grp_addr);
extern struct mcft_entry * get_mcft(__u32 grp_addr);


#endif /*_IGMPPROXY_H_*/
