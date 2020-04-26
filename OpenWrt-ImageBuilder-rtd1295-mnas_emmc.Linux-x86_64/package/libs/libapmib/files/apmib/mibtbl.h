/*
 *      Include file of mibtbl.c
 *
 *      Authors: David Hsu	<davidhsu@realtek.com.tw>
 *
 *      $Id: mibtbl.h,v 1.9 2009/07/30 11:32:12 keith_huang Exp $
 *
 */

#ifndef INCLUDE_MIBTBL_H
#define INCLUDE_MIBTBL_H

#include "apmib.h"

#ifdef WIN32
#ifdef FIELD_OFFSET
#undef FIELD_OFFSET
#endif
#endif

#define FIELD_OFFSET(type, field)	((unsigned long)(long *)&(((type *)0)->field))
#define FIELD_SIZE(type, field)		sizeof(((type *)0)->field)

#define _OFFSET(field)			((int)FIELD_OFFSET(APMIB_T,field))
#define _SIZE(field)			sizeof(((APMIB_T *)0)->field)
#define _OFFSET_WLAN(field)		((int)FIELD_OFFSET(CONFIG_WLAN_SETTING_T,field))
#define _SIZE_WLAN(field)		sizeof(((CONFIG_WLAN_SETTING_T *)0)->field)

#define _OFFSET_HW(field)		((int)FIELD_OFFSET(HW_SETTING_T,field))
#define _SIZE_HW(field)			sizeof(((HW_SETTING_T *)0)->field)
#define _OFFSET_HW_WLAN(field)		((int)FIELD_OFFSET(HW_WLAN_SETTING_T,field))
#define _SIZE_HW_WLAN(field)		sizeof(((HW_WLAN_SETTING_T *)0)->field)

#if 1//def MIB_TLV
#define _TOTAL_SIZE(type, field)		sizeof(((type *)0)->field)
#define _UNIT_SIZE(field)		sizeof(field)

#define _MIBHWID_NAME(name) MIB_HW_##name, #name
#define _MIBID_NAME(name) MIB_##name, #name
#define _MIBWLANID_NAME(name) MIB_WLAN_##name, #name
#ifdef BLUETOOTH_HW_SETTING_SUPPORT
#define _MIB_BLUETOOTH_HWID_NAME(name) MIB_BLUETOOTH_HW_##name, #name
#endif

#ifdef CUSTOMER_HW_SETTING_SUPPORT
#define _MIB_CUSTOMER_HWID_NAME(name) MIB_CUSTOMER_HW_##name, #name
#endif

#define _OFFSET_SIZE_FIELD(type, field) \
	FIELD_OFFSET(type, field), \
	FIELD_SIZE(type, field), \
	_TOTAL_SIZE(type, field)
#endif //#ifdef MIB_TLV


#ifdef RTK_REINIT_SUPPORT
#define _REINIT_FIELD(decision_id,reinit_id) decision_id,reinit_id
#else
#define _REINIT_FIELD(decision_id,reinit_id)
#endif
// MIB value, id mapping table
typedef struct _mib_table_entry mib_table_entry_T;
struct _mib_table_entry {
	int id;
#if defined(CONFIG_RTL_8812_SUPPORT)
	char name[40];
#else
	char name[32];
#endif
	TYPE_T type;
	int offset;
	int size;
#ifdef MIB_TLV	
	unsigned short		total_size;
	unsigned short		unit_size;
	const unsigned char *default_value;
	mib_table_entry_T * next_mib_table;
#ifdef RTK_REINIT_SUPPORT
		MIB_CHANGE_FUNC_ID decision_func_id;
		MIB_CHANGE_FUNC_ID reinit_func_id;
#endif
#endif //#ifdef MIB_TLV	
};

extern mib_table_entry_T mib_table[], mib_wlan_table[], hwmib_table[], hwmib_wlan_table[];
#ifdef BLUETOOTH_HW_SETTING_SUPPORT
extern mib_table_entry_T bluetooth_hwmib_table[];
#endif
#ifdef CUSTOMER_HW_SETTING_SUPPORT
extern mib_table_entry_T customer_hwmib_table[];
#endif
extern mib_table_entry_T mib_waniface_tbl[];
#endif // INCLUDE_MIBTBL_H
