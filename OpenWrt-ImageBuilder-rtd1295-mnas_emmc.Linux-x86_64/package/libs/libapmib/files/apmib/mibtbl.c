/*
 *      MIB table declaration
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: mibtbl.c,v 1.30 2009/09/03 06:58:11 keith_huang Exp $
 *
 */

/* Include files */
#include "apmib.h"
#include "mibtbl.h"

/* Global variable definitions */

/*
 * When using flash (set/get/all) command to access the MIB of below table,
 * it needs append a keyword "DEF_" in ahead of mib name when access
 * default setting MIB.
 */


#if defined(MULTI_WAN_SUPPORT)
mib_table_entry_T mib_waniface_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl , decision_id, reinit_id) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl,_REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl , decision_id, reinit_id) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_WANIFACE_IMPORT
#include "mibdef.h"
#undef MIB_WANIFACE_IMPORT

#undef MIBDEF
{0}
};

#endif//#if defined(MULTI_WAN_SUPPORT)

mib_table_entry_T mib_dhcpRsvdIp_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_DHCPRSVDIP_IMPORT
#include "mibdef.h"
#undef MIB_DHCPRSVDIP_IMPORT

#undef MIBDEF
{0}
};

#ifdef WLAN_PROFILE
mib_table_entry_T mib_wlan_profile_tbl1[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_WLAN_PROFILE_IMPORT
#include "mibdef.h"
#undef MIB_WLAN_PROFILE_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T mib_wlan_profile_tbl2[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_WLAN_PROFILE_IMPORT
#include "mibdef.h"
#undef MIB_WLAN_PROFILE_IMPORT

#undef MIBDEF
{0}
};

#endif


mib_table_entry_T mib_schedule_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
	{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_SCHEDULE_IMPORT
#include "mibdef.h"
#undef MIB_SCHEDULE_IMPORT

#undef MIBDEF
{0}
};

#if defined(VLAN_CONFIG_SUPPORTED)
mib_table_entry_T mib_vlanconfig_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_VLAN_CONFIG_IMPORT
#include "mibdef.h"
#undef MIB_VLAN_CONFIG_IMPORT

#undef MIBDEF
{0}
};
#endif

#if defined(CONFIG_8021Q_VLAN_SUPPORTED)
mib_table_entry_T mib_vlan_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_VLAN_CONFIG_IMPORT
#include "mibdef.h"
#undef MIB_VLAN_CONFIG_IMPORT

#undef MIBDEF
{0}
};
#endif


#if defined(CONFIG_RTL_ETH_802DOT1X_SUPPORT)
mib_table_entry_T mib_ethdot1xconfig_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_ETH_DOT1X_IMPORT
#include "mibdef.h"
#undef MIB_ETH_DOT1X_IMPORT

#undef MIBDEF
{0}
};
#endif


#ifdef _SUPPORT_CAPTIVEPORTAL_PROFILE_
mib_table_entry_T mib_capPortalAllow_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_CAP_PORTAL_IMPORT
#include "mibdef.h"
#undef MIB_CAP_PORTAL_IMPORT

#undef MIBDEF
{0}
};	
#endif/****_SUPPORT_CAPTIVEPORTAL_PROFILE_*****/

#ifdef HOME_GATEWAY
mib_table_entry_T mib_portfw_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_PORTFW_IMPORT
#include "mibdef.h"
#undef MIB_PORTFW_IMPORT

#undef MIBDEF
{0}
};

#if defined(WLAN_SUPPORT)
mib_table_entry_T mib_cwmp_wlanconf_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_CWMP_WLANCONF_IMPORT
#include "mibdef.h"
#undef MIB_CWMP_WLANCONF_IMPORT

#undef MIBDEF
{0}
};
#endif //#if defined(WLAN_SUPPORT)



mib_table_entry_T mib_ipfilter_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_IPFILTER_IMPORT
#include "mibdef.h"
#undef MIB_IPFILTER_IMPORT

#undef MIBDEF
{0}
};

#ifdef CONFIG_RTL_AIRTIME
mib_table_entry_T mib_airtime_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_AIRTIME_IMPORT
#include "mibdef.h"
#undef MIB_AIRTIME_IMPORT

#undef MIBDEF
{0}
};
#endif /* CONFIG_RTL_AIRTIME */


#ifdef SAMBA_WEB_SUPPORT
mib_table_entry_T mib_storage_user_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_STORAGE_USER_IMPORT
#include "mibdef.h"
#undef MIB_STORAGE_USER_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T mib_storage_group_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_STORAGE_GROUP_IMPORT
#include "mibdef.h"
#undef MIB_STORAGE_GROUP_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T mib_storage_shareinfo_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_STORAGE_SHAREINFO_IMPORT
#include "mibdef.h"
#undef MIB_STORAGE_SHAREINFO_IMPORT

#undef MIBDEF
{0}
};

#endif



mib_table_entry_T mib_portfilter_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_PORTFILTER_IMPORT
#include "mibdef.h"
#undef MIB_PORTFILTER_IMPORT

#undef MIBDEF
{0}
};
	
mib_table_entry_T mib_macfilter_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_MACFILTER_IMPORT
#include "mibdef.h"
#undef MIB_MACFILTER_IMPORT

#undef MIBDEF
{0}
};
	
mib_table_entry_T mib_triggerport_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_TRIGGERPORT_IMPORT
#include "mibdef.h"
#undef MIB_TRIGGERPORT_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T mib_urlfilter_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_URLFILTER_IMPORT
#include "mibdef.h"
#undef MIB_URLFILTER_IMPORT

#undef MIBDEF
{0}
};


#ifdef ROUTE_SUPPORT
mib_table_entry_T mib_staticroute_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_STATICROUTE_IMPORT
#include "mibdef.h"
#undef MIB_STATICROUTE_IMPORT

#undef MIBDEF
{0}
};	
#endif

#if defined(_PRMT_X_TELEFONICA_ES_DHCPOPTION_)
mib_table_entry_T mib_dhcpServerOption_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif


#define MIB_DHCPDOPTION_IMPORT
#include "mibdef.h"
#undef MIB_DHCPDOPTION_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T mib_dhcpClientOption_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif


#define MIB_DHCPCOPTION_IMPORT
#include "mibdef.h"
#undef MIB_DHCPCOPTION_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T mib_dhcpsServingPool_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif


#define MIB_DHCPS_SERVING_POOL_IMPORT
#include "mibdef.h"
#undef MIB_DHCPS_SERVING_POOL_IMPORT

#undef MIBDEF
{0}
};
#endif /* #if defined(_PRMT_X_TELEFONICA_ES_DHCPOPTION_) */



#ifdef VPN_SUPPORT
mib_table_entry_T mib_ipsectunnel_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_IPSECTUNNEL_IMPORT
#include "mibdef.h"
#undef MIB_IPSECTUNNEL_IMPORT

#undef MIBDEF
{0}
};
#endif

#endif // #ifdef HOME_GATEWAY

#ifdef RTK_CAPWAP

mib_table_entry_T mib_capwap_wtp_config_tbl[]={

#ifdef MIB_TLV
#define MIBDEF(_ctype,  _cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl,decision_id, reinit_id ) \
                {_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,  _cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl,decision_id, reinit_id  ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_CAPWAP_WTP_CONFIG_IMPORT
#include "mibdef.h"
#undef MIB_CAPWAP_WTP_CONFIG_IMPORT

#undef MIBDEF
{0}
};
#endif


#ifdef TLS_CLIENT
mib_table_entry_T mib_certroot_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_CERTROOT_IMPORT
#include "mibdef.h"
#undef MIB_CERTROOT_IMPORT

#undef MIBDEF
{0}
};	

mib_table_entry_T mib_certuser_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_CERTUSER_IMPORT
#include "mibdef.h"
#undef MIB_CERTUSER_IMPORT
{0}
};		
#endif

#if defined(GW_QOS_ENGINE)
mib_table_entry_T mib_qos_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_QOS_IMPORT
#include "mibdef.h"
#undef MIB_QOS_IMPORT
{0}
};	
#endif
#if defined(TR181_SUPPORT)
#ifdef CONFIG_IPV6
mib_table_entry_T mib_ipv6DhcpcSendOpt_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_IPV6_DHCPC_SENDOPT_IMPORT
#include "mibdef.h"
#undef MIB_IPV6_DHCPC_SENDOPT_IMPORT
{0}
};	
#endif
mib_table_entry_T mib_dnsClientServer_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_DNS_CLIENT_SERVER_IMPORT
#include "mibdef.h"
#undef MIB_DNS_CLIENT_SERVER_IMPORT
{0}
};
#endif


#if defined(QOS_BY_BANDWIDTH)
mib_table_entry_T mib_qos_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_IPQOS_IMPORT
#include "mibdef.h"
#undef MIB_IPQOS_IMPORT
{0}
};	
#endif


#ifdef QOS_OF_TR069
mib_table_entry_T mib_qos_queue_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_QOSQUEUE_IMPORT
#include "mibdef.h"
#undef MIB_QOSQUEUE_IMPORT
{0}
};

mib_table_entry_T mib_qos_class_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_QOSCLASS_IMPORT
#include "mibdef.h"
#undef MIB_QOSCLASS_IMPORT
{0}
};

mib_table_entry_T mib_tr098_app_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_TR098_APP_CONFIG_IMPORT
#include "mibdef.h"
#undef MIB_TR098_APP_CONFIG_IMPORT
#undef MIBDEF

{0}
};

mib_table_entry_T mib_tr098_flow_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_TR098_FLOW_CONFIG_IMPORT
#include "mibdef.h"
#undef MIB_TR098_FLOW_CONFIG_IMPORT
#undef MIBDEF

{0}
};

//mark_qos
mib_table_entry_T mib_qos_policer_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_QOSPOLICER_IMPORT
#include "mibdef.h"
#undef MIB_QOSPOLICER_IMPORT
{0}
};

mib_table_entry_T mib_qos_queuestats_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_QOSQUEUESTATS_IMPORT
#include "mibdef.h"
#undef MIB_QOSQUEUESTATS_IMPORT
{0}
};

#endif
//#ifdef CONFIG_RTK_MESH Keith remove
//new feature:Mesh enable/disable

//#ifdef	_MESH_ACL_ENABLE_ Keith remove
mib_table_entry_T mib_mech_acl_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_MESH_MACFILTER_IMPORT
#include "mibdef.h"
#undef MIB_MESH_MACFILTER_IMPORT
{0}
};	
//#endif Keith remove

#ifdef MIB_TLV
mib_table_entry_T mib_root_table[]={
{MIB_ROOT, "MIB_ROOT",		TABLE_LIST_T, 	0, _UNIT_SIZE(APMIB_T), _UNIT_SIZE(APMIB_T), _UNIT_SIZE(APMIB_T), 0, mib_table,0,0},
{0}
};
#else
mib_table_entry_T mib_root_table[]={
{MIB_ROOT, "MIB_ROOT",		TABLE_LIST_T, 	0, _UNIT_SIZE(APMIB_T)},
{0}
};

#endif

mib_table_entry_T mib_table[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_IMPORT
#include "mibdef.h"
#undef MIB_IMPORT

#undef MIBDEF
{0}
};


mib_table_entry_T wlan_acl_addr_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_WLAN_MACFILTER_IMPORT
#include "mibdef.h"
#undef MIB_WLAN_MACFILTER_IMPORT

#undef MIBDEF
{0}
};

mib_table_entry_T wlan_wds_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_WDS_IMPORT
#include "mibdef.h"
#undef MIB_WDS_IMPORT

#undef MIBDEF
{0}
};

#ifdef FAST_BSS_TRANSITION
mib_table_entry_T wlan_ftkh_tbl[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_FTKH_IMPORT
#include "mibdef.h"
#undef MIB_FTKH_IMPORT

#undef MIBDEF
{0}
};
#endif

/*
 * When using flash (set/get/all) command to access the MIB of below table,
 * it needs append a keyword "WLANx_" in ahead of mib name.
 * When access default setting, it needs appened a keyword "DEF_" in front of
 * "WLANx_" keyword.
 */

mib_table_entry_T mib_wlan_table[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBWLANID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBWLANID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif

#define MIB_CONFIG_WLAN_SETTING_IMPORT
#include "mibdef.h"
#undef MIB_CONFIG_WLAN_SETTING_IMPORT

#undef MIBDEF

{0}
};

mib_table_entry_T hwmib_root_table[]={
#ifdef MIB_TLV
{_MIBID_NAME(HW_ROOT),		TABLE_LIST_T, 	0, _UNIT_SIZE(HW_SETTING_T), _UNIT_SIZE(HW_SETTING_T), _UNIT_SIZE(HW_SETTING_T), 0, hwmib_table,_REINIT_FIELD(0,0)},
#else
{_MIBID_NAME(HW_ROOT),		TABLE_LIST_T, 	0, _UNIT_SIZE(HW_SETTING_T)},
#endif
{0}
};

/*
 * When using flash (set/get/all) command to access the MIB of below table,
 * it needs append a keyword "HW_" in ahead of mib name.
 */

mib_table_entry_T hwmib_table[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBHWID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBHWID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif
#define MIB_HW_IMPORT
#include "mibdef.h"
#undef MIB_HW_IMPORT

#undef MIBDEF
{0}
};


/*
 * When using flash (set/get/all) command to access the MIB of below table,
 * it needs append a keyword "HW_WLANx_" in ahead of mib name.
 */

mib_table_entry_T hwmib_wlan_table[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
		{_MIBHWID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl, decision_id, reinit_id ) \
			{_MIBHWID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif
#define MIB_HW_WLAN_IMPORT
#include "mibdef.h"
#undef MIB_HW_WLAN_IMPORT

#undef MIBDEF
{0}
};
#ifdef BLUETOOTH_HW_SETTING_SUPPORT

mib_table_entry_T bluetooth_hwmib_root_table[]={
#ifdef MIB_TLV
{_MIBID_NAME(BLUETOOTH_HW_ROOT),		TABLE_LIST_T, 	0, _UNIT_SIZE(BLUETOOTH_HW_SETTING_T), _UNIT_SIZE(BLUETOOTH_HW_SETTING_T), _UNIT_SIZE(BLUETOOTH_HW_SETTING_T), 0, bluetooth_hwmib_table,0,0},
#else
{_MIBID_NAME(BLUETOOTH_HW_ROOT),		TABLE_LIST_T, 	0, _UNIT_SIZE(BLUETOOTH_HW_SETTING_T)},
#endif
{0}
};


mib_table_entry_T bluetooth_hwmib_table[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl,decision_id, reinit_id ) \
		{_MIB_BLUETOOTH_HWID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl,decision_id, reinit_id ) \
			{_MIB_BLUETOOTH_HWID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif
#define MIB_BLUETOOTH_HW_IMPORT
#include "mibdef.h"
#undef MIB_BLUETOOTH_HW_IMPORT

#undef MIBDEF
{0}
};

#endif

#ifdef CUSTOMER_HW_SETTING_SUPPORT

mib_table_entry_T customer_hwmib_root_table[]={
#ifdef MIB_TLV
{_MIBID_NAME(CUSTOMER_HW_ROOT),		TABLE_LIST_T, 	0, _UNIT_SIZE(CUSTOMER_HW_SETTING_T), _UNIT_SIZE(CUSTOMER_HW_SETTING_T), _UNIT_SIZE(CUSTOMER_HW_SETTING_T), 0, customer_hwmib_table,0,0},
#else
{_MIBID_NAME(CUSTOMER_HW_ROOT),		TABLE_LIST_T, 	0, _UNIT_SIZE(CUSTOMER_HW_SETTING_T)},
#endif
{0}
};


mib_table_entry_T customer_hwmib_table[]={
#ifdef MIB_TLV
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl,decision_id, reinit_id ) \
		{_MIB_CUSTOMER_HWID_NAME(_mib_name), _mib_type, _OFFSET_SIZE_FIELD(_mib_parents_ctype, _cname), _UNIT_SIZE(_ctype), _default_value, _next_tbl, _REINIT_FIELD(decision_id,reinit_id)},
#else
#define MIBDEF(_ctype,	_cname, _crepeat, _mib_name, _mib_type, _mib_parents_ctype, _default_value, _next_tbl,decision_id, reinit_id ) \
			{_MIB_CUSTOMER_HWID_NAME(_mib_name), _mib_type, FIELD_OFFSET(_mib_parents_ctype, _cname), FIELD_SIZE(_mib_parents_ctype, _cname)},
#endif
#define MIB_CUSTOMER_HW_IMPORT
#include "mibdef.h"
#undef MIB_CUSTOMER_HW_IMPORT

#undef MIBDEF
{0}
};

#endif

#ifdef MIB_TLV

mib_table_entry_T* mib_get_table(CONFIG_DATA_T type)
{
	switch(type)
	{
		case HW_SETTING:
			return hwmib_root_table;
#ifdef BLUETOOTH_HW_SETTING_SUPPORT
		case BLUETOOTH_HW_SETTING:
			return bluetooth_hwmib_root_table;
#endif
#ifdef CUSTOMER_HW_SETTING_SUPPORT
		case CUSTOMER_HW_SETTING:
			return customer_hwmib_root_table;
#endif
		case DEFAULT_SETTING:
		case CURRENT_SETTING:
			return mib_root_table;
		default:
			return 0;
	}
	
}
#endif
