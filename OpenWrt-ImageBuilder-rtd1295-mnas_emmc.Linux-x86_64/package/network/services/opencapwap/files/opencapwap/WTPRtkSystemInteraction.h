#ifndef __CAPWAP_RTKDriverInteraction_HEADER__
#define __CAPWAP_RTKDriverInteraction_HEADER__

#include "CWWTP.h"


CWBool cwRtkWtpInit(void);
int cwRtkWtpExit(void);
int cwRtkWtpGetVendorId(void);
int cwRtkWtpGetVendorId(void);
int cwRtkWtpGetSerialNo(void);
int cwRtkWtpGetModelNo(void);
int cwRtkWtpGetHwVersion(void);
//const char *cwRtkWtpGetSwVersion(void);
int cwRtkWtpGetBootVersion(void);


#ifdef KM_BY_AC
int cwRtkWtpRcvFrame(uint8_t *buffer, uint32_t buffer_len, uint8_t **frame_pos, uint32_t *frame_len);
#endif

// ----------- firmware upgrade and reboot ----------
const char *cwRtkWtpGetCurrentFwId(void);
const char *cwRtkWtpGetTempFwId(void);

/* step 1 of download fw */
//void cwRtkWtpSetTempFwId(const char *version, const CWProtocolImageInformationValues* image_info, CWBool with_image_info);
void cwRtkWtpPrepareFwDownload(const char *fwId, const CWProtocolImageInformationValues* image_info, CWBool with_image_info);

/* step 2 of download fw */
CWBool cwRtkWtpSaveFirmwareDataPiece(const CWProtocolImageDataValues *image_data, CWBool *last);
/* step 3 (final step) of download fw */
CWBool cwRtkWtpCheckFirmware(void);

CWBool cwRtkWtpFirmwareUpgradeReboot(void);

// ----------- set to driver only ----------------
#ifdef KM_BY_AC
// not use flash configuration to set wlan
CWProtocolResultCode cwRtkWtpAddWlan(CWProtocolAddWLANValues* pAddWlan, unsigned char *bssid);
CWBool cwRtkWtpLookupWlanByMac(const unsigned char *mac_address, int *radio_id, int *wlan_idx); // by global variable which is set when calling cwRtkWtpAddWlanDriver()
#endif
CWProtocolResultCode cwRtkWtpDeleteStation(int radio_id, unsigned char *sta_mac); // by driver
CWProtocolResultCode cwRtkWtpFreeStation(int radio_id, unsigned char *sta_mac);	//by driver
CWProtocolResultCode cwRtkWtpSilentStation(int radio_id, unsigned char *sta_mac); // by driver
CWProtocolResultCode cwRtkWtpUnsilentStation(int radio_id, unsigned char *sta_mac); // by driver
CWProtocolResultCode cwRtkWtpDot11vStation(int radio_id, unsigned char *sta_mac); // by driver
CWProtocolResultCode cwRtkWtpDualStation(int radio_id, unsigned char *sta_mac);	//by driver
CWProtocolResultCode cwRtkWtpSetQos(const CWProtocolWtpQosValues* wtp_qos); // by driver
CWBool cwRtkWtpSiteSurvey(int radio_id, int maxNum, int *numBss, rtk_bss_t *bssInfo);
CWBool cwRtkWtpGetWlanStations(int radio_id, int wlan_id, int maxNum, int *numStation, CWRtkStationValues *staInfo);

// ----------- Get/Set global system information (flash/driver) -------------
CWBool cwRtkWtpSystemEnabled(void);
const char *cwRtkWtpGetApName(void);
int cwRtkWtpGetWtpId(void);
unsigned int cwRtkWtpGetAcIp(void);
int cwRtkWtpGetMaxRadios(void);
int cwRtkWtpGetMaxWlanPerRadio(void);
CWBool cwRtkWtpGetRadioInfo(CWRadiosInformation *valPtr); // band support
#ifdef RTK_SMART_ROAMING
CWBool cwRtkWtpGetConfiguration(CWWTPWlanConfiguration *valPtr);
#endif

#ifdef RTK_AUTO_AC
typedef enum { IP_ADDR, DST_IP_ADDR, SUBNET_MASK, DEFAULT_GATEWAY, HW_ADDR } ADDR_T;
int getInAddr( char *interface, ADDR_T type, void *pAddr );
void iptoint(unsigned char *bytes, char str[]);
#endif

/* description: read config from flash.
 * Inputs: 
 * Outputs: nWlans, pWlanConfig: please pre-allocate the memories for the two variables.
 * return: 
 */
CWBool cwRtkWtpGetWlans(int maxNum, int *nWlans, CWProtocolRtkWlanConfigValues *pWlanConfig);
//CWBool cwRtkWtpGetRadios(int *nRadios, CWProtocolRtkRadioValues *pRadioConfig);
CWBool cwRtkWtpGetChannel(int radio_id, CWProtocolRtkChannelValues *pRadioConfig);
CWBool cwRtkWtpGetPowerScale(int radio_id, CWProtocolRtkPowerScaleValues *pRadioConfig);


/* description: write config into flash.
 * Inputs: nWlans, pWlanConfig, 
 *		invalid_all_others - disable those not listed.
 *		reload_if_changed - set config from flash to driver
 * Outputs: needReloadLater - flash and driver are unsync; need to do reload later to sync them.
 * return: configuration has been activated.
 */
CWBool cwRtkWtpCfgWlans(int nWlans, const CWProtocolRtkWlanConfigValues *pWlanConfig, 
						CWBool invalid_all_others, CWBool reload_if_changed,
						CWBool *needReloadLater);
CWBool cwRtkWtpCfgPowerScales(int nRadios, const CWProtocolRtkPowerScaleValues *pRadioConfig, CWBool reload_if_changed, CWBool *needReloadLater);
CWBool cwRtkWtpCfgChannels(int nRadios, const CWProtocolRtkChannelValues *pRadioConfig, CWBool reload_if_changed, CWBool *needReloadLater);
void cwRtkWtpActivateFlashCfg(void);
#endif
