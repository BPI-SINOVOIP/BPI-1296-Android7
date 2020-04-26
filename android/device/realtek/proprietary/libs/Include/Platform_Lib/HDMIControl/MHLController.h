#ifndef __MHL_CONTROLLER_H__
#define __MHL_CONTROLLER_H__

#include <hresult.h>
#include "OSAL/Types.h"
#include <AudioRPC_Agent.h>
#include <VideoRPC_Agent.h>
#include <VideoRPC_System.h>
#include <VideoRPC_System_data.h>
#include <AudioRPC_System.h>
#include <AudioRPC_System_data.h>
#include <RPCBaseDS.h>
#include <hardware/hardware.h>
#ifdef MHL_HAL_ENABLED
#include "mhltx/rtk_mhl.h"
#endif
#include "realtek_omx/osal_rtk/OSAL_RTK.h"

/* for AVMute */
#define  HDMI_CLRAVM 0x00
#define  HDMI_SETAVM 0x01

#define  HDMI_SETAudioMute 0x01
#define  HDMI_CLRAudioMute 0x02

#define  HDMI_OFF 	0x00
#define  DVI_MODE 	0x01
#define  HDMI_MODE 	0x02
#define  AUTO_MODE 	0x03

#define  CTS_27M            0x01
#define  CTS_74M25          0x02
#define  CTS_148M5          0x03

#define  HDMI_PHY_UNKNOWN	0x00
#define  HDMI_PHY_ON 		0x01
#define  HDMI_PHY_OFF 		0x02

#define HDMI_MODE_UNDEF	0x00
#define HDMI_MODE_HDMI	0x01
#define HDMI_MODE_DVI		0x02

#define  DISPLAY_NONE			0x00
#define  DISPLAY_HDMI_TV_POWER_ON	0x01
#define  DISPLAY_HDMI_TV_POWER_OFF	0x02
#define  DISPLAY_DVI_HDCP_ON		0x03
#define  DISPLAY_DVI_HDCP_OFF		0x04

#define  HDMI_LS_0	0x00
#define  HDMI_LS_50	0x01
#define  HDMI_LS_100	0x02
#define  HDMI_LS_150	0x03
#define  HDMI_LS_200	0x04
#define  HDMI_LS_250	0x05
#define  HDMI_LS_300	0x06
#define  HDMI_LS_350	0x07
#define  HDMI_LS_400	0x08
#define  HDMI_LS_450	0x09
#define  HDMI_LS_500	0x10
#define  HDMI_LS_AUTO	0x512
#define  HDMI_LS_OFF	0x513

#define  HDMI_DC_AUTO	0x00
#define  HDMI_DC_12BIT	0x01
#define  HDMI_DC_10BIT	0x02
#define  HDMI_DC_OFF	0x03


#define  Format_SD      0x58
#define  Format_HD      0xa8

#define  No_Reptn       0x00
#define  Send_2T        0x01
#define  Send_3T        0x02
#define  Send_4T        0x03
#define  Send_5T        0x04
#define  Send_6T        0x05
#define  Send_7T        0x06
#define  Send_8T        0x07
#define  Send_9T        0x08
#define  Send_10T       0x09

/* for AUDIO or AUDIO infoframe */
#define  Input_I2S          0x01
#define  Input_SPDIF        0x02

#define  Type_Ref_Header    0x00
#define  Type_PCM           0x01
#define  Type_AC_3          0x02
#define  Type_MPEG1         0x03
#define  Type_MP3           0x04
#define  Type_MPEG2         0x05
#define  Type_AAC           0x06
#define  Type_DTS           0x07
#define  Type_ATRAC         0x08

/* for VIDEO type */
#define  VIDEO_525i         0x01
#define  VIDEO_525p         0x02
#define  VIDEO_625i         0x03
#define  VIDEO_625p         0x04
#define  VIDEO_720p_50Hz    0x05
#define  VIDEO_720p_60Hz    0x06
#define  VIDEO_1080i_50Hz   0x07
#define  VIDEO_1080i_60Hz   0x08
#define  VIDEO_1080p_50Hz   0x09
#define  VIDEO_1080p_60Hz   0x0a
#define  VIDEO_1080p_24Hz   0x0b

class MHLController
{
public:
	MHLController (void);
	~MHLController(void);

	static struct mhl_device_t* sMHLDevice;

	static HRESULT initMHLController(void);
	static HRESULT deInitMHLController(void);
	static HRESULT getSinkCapability(void);
	static int isSinkConnected(void);
	static HRESULT getEDID(unsigned char *EDIDBuffer);
	static HRESULT sendAVMute(int choice);
	static HRESULT configTVSystem(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM argp);
	static HRESULT configAVIInfoFrame(struct VIDEO_RPC_VOUT_CONFIG_HDMI_INFO_FRAME argp);
	static HRESULT setHDMIFrequency(int videoCode, int HDMIFrequency);
	static HRESULT sendAudioMute(int select);
	static HRESULT sendAudioVSDBData(unsigned char latency[4], int interlaced);
	static HRESULT sendAudioVSDBDataManual(int delay);
	static int checkLinkStatus(void);

	// The following functions are added by Kevin. They may call the HAL function.
	static int isEDIDReady(void);
	static int isMHLDevice(void);
	static int getTVSupportxvYCC(void);
	static int getTVSupportYCbCr(void);
	static int getTVSupportDeepColor(void);
	static int getTVSupportDeepColorYCbCr444(void);
	static int restartHDMI(void);
	static int getTVSupportHDCP(void);
	static HRESULT HDCPForceUnAuth(void);
	static void setTVSupportxvYCC(int value);
	static void setTVSupportYCbCr(int value);
	static void setTVSupportDeepColor(int value);
	static void setTVSupportDeepColorYCbCr444(int value);
	static void getTVSupport3D(unsigned char _3D_capability[2]);
	static void setTVSupport3D(unsigned char _3d_structure_high, unsigned char _3d_structure_low);
	static bool get3DPresent(void);

	static HRESULT HDCP_CkeckIfKeyExist(void);
	static HRESULT SaveEDIDToTmp(unsigned char* edidBuf);
	static HRESULT updateADBToAudio(int enable);
	static int readRCPMessage(unsigned char* keyID);

private:
	static HRESULT initMHLDevice(void);
	static HRESULT deInitMHLDevice(void);
	static HRESULT extractVID(unsigned long long value, unsigned char extended_value);
	static HRESULT checkVID(unsigned long long value, int vid);
	static void* RTKIon_alloc(OSAL_U32 size, OSAL_U8 **noncacheable, OSAL_U32 *phys_addr, ion_user_handle_t *ionhdl);
	static void RTKIon_free(void *addr, OSAL_U32 size, ion_user_handle_t *ionhdl);

public:
	// MHL Sink status
	static bool bIsEDIDReady;
	static int intHDMIStatus;
	static int intTVSupportDeepColor;
	static int intTVSupportYCbCr;
	static int intTVSupportxvYCC;
	static int intTVSupportDeepColorYCbCr444;
	static int intTVVidLength;
	static int iArrTVVid[68];	// We keep the vid the box supports.
	static bool b3DPresent;
	static unsigned char iArr3dStructure[2];
	static unsigned short hdmiSourceAddress;
};

#endif
