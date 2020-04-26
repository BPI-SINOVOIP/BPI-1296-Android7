#ifndef _AC_RTK_SYSTEM_INTERACTION_H_
#define _AC_RTK_SYSTEM_INTERACTION_H_

#include "CWAC.h"


CWBool cwRtkAcInit(void);
void cwRtkAcExit(void);
int cwRtkAcGetVendorId(void);
const char *cwRtkACGetSwVersion(void);


// ----------- Get/Set global system information (flash/driver) -------------
CWBool get_mib(const char *ifname, const char *mibname, void *result, int size);
CWBool cwRtkAcSystemEnabled(void);
CWBool cwRtkAcTransferWtpName2Id(const char *wtp_name, int *id);
CWBool cwRtkAcGetWTPDefaultFwId(char *fwImageIdentifier);

#ifdef X86_SIMULATION

#define _FAKE_AC_DB_

#else

CWBool cwRtkAcGetWtpConfig(const char *wtp_name, rtk_wtp_t *wtp);
CWBool cwRtkAcSetPowerScale(int wtp_id,int radio_id, rtk_power_scale_t power_scale);
CWBool cwRtkAcSetChannel(int wtp_id,int radio_id, unsigned char channel);
CWBool cwRtkAcSetFwId(int wtp_id, const char *fwId);

#endif


#endif
