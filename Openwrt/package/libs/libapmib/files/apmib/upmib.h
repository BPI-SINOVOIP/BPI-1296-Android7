#ifndef INCLUDE_UPMIB_H
#define INCLUDE_UPMIB_H
#include "apmib.h"
#include "mibtbl.h"

/*
 * The 'id' in 'struct upmib' is the same as the defination in apmib.h.
 *eg:it should be 'MIB_MIB_VER' according to "#define MIB_MIB_VER 655 " in apmib.h
 *
 * The 'name' in 'struct upmib' is the same as the defination in mibdef.h
 *eg:it should be 'MIB_VER' according to "MIBDEF(unsigned char,   mibVer, , MIB_VER,    BYTE_T, APMIB_T, 5, 0)" 
 *defined in mibdef.h
 *
 * If you want to update MIB current setting value,you must change MIB VERSION in 'UPMIB_T update_mib[]' first.
 * Add the new MIB in 'UPMIB_T new_mib'. 
 * Add the MIB that you want to update its value in 'UPMIB_T update_mib[]'.
 *
 */


 
typedef struct upmib{
	int id;
#if defined(CONFIG_APP_APPLE_MFI_WAC)
	unsigned char name[128];
#else
	unsigned char name[32];
#endif
	unsigned char value[128];	//the mib value
	TYPE_T type;
}UPMIB_T, *UPMIB_Tp;


 
UPMIB_T update_mib[] = {
//add mib info here
#if defined(CONFIG_APP_APPLE_MFI_WAC)
	{MIB_MIB_VER,           "MIB_VER",              "2"},
	{MIB_MIB_VER,           "DEF_MIB_VER",          "2"},
	{MIB_OP_MODE,           "OP_MODE",              "1"},
	{MIB_OP_MODE,           "DEF_OP_MODE",          "1"},
	{MIB_WLAN_WSC_DISABLE,   "WLAN0_WSC_DISABLE",              "1"},
	{MIB_WLAN_WSC_DISABLE,   "DEF_WLAN0_WSC_DISABLE",          "1"},
	{MIB_WLAN_IAPP_DISABLED, "WLAN0_IAPP_DISABLED",            "1"},
	{MIB_WLAN_IAPP_DISABLED, "DEF_WLAN0_IAPP_DISABLED",        "1"},
#else
	{MIB_MIB_VER,           "MIB_VER",              "1", BYTE_T},
#endif
	{0}
};
 
UPMIB_T new_mib[] = {
//add mib info here
#if defined(CONFIG_APP_APPLE_MFI_WAC)
	{MIB_MFI_WAC_DEVICE_NAME,           "MFI_WAC_DEVICE_NAME",              "RTL8196E"},
	{MIB_MFI_WAC_DEVICE_NAME,           "DEF_MFI_WAC_DEVICE_NAME",              "RTL8196E"},
	{MIB_WLAN_MFIWAC_CONFIGURED,           "DEF_WLAN0_MFIWAC_CONFIGURED",              "0"},
	{MIB_WLAN_MFIWAC_CONFIGURED,           "WLAN0_MFIWAC_CONFIGURED",              "0"},
#endif
	{0}
};


#endif
