#ifndef __DPTX_CONTROLLER_H__
#define __DPTX_CONTROLLER_H__

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
#include "dptx/rtk_dptx.h"
#include "realtek_omx/osal_rtk/OSAL_RTK.h"
#if defined(__LINARO_SDK__)
//#include "HDMIAdapter/HDMIAdapter.h"
#endif

class DPTXController
{
public:
	DPTXController (void);
	~DPTXController(void);

#if defined(__LINARO_SDK__)
	//static HDMIAdapter* sHDMIDevice; //TODO
#else
	static struct dptx_device_t* sDPTXDevice;
#endif
	static HRESULT initDPTXController(void);
	static HRESULT deInitDPTXController(void);
	static HRESULT getSinkCapability(void);
	static HRESULT configTVSystem(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM argp);
	static bool isSinkConnected();
	static void setSinkConnected(bool status);
	static bool isEDIDReady();

private:
	static HRESULT initDPTXDevice(void);
	static HRESULT deInitDPTXDevice(void);
	static HRESULT extractVID(unsigned long long value, unsigned long long value2,  unsigned long long value2_420, unsigned char extended_value);
	static HRESULT checkVID(unsigned long long value, int vid);


public:
	// DPTX Sink status
	static bool bIsEDIDReady;
	static bool bDPTXSinkStatus;
	static int intHDMIStatus;
	static int intTVSupportDeepColor;
	static int intTVSupportYCbCr;
	static int intTVSupportxvYCC;
	static int intTVSupportDeepColorYCbCr444;
	static int intTVVidLength;
	static int iArrTVVid[132];	// We keep the vid the box supports.
	static bool b3DPresent;
	static unsigned char iArr3dStructure[2];
	static unsigned char ucArr3DVic[16];
	static unsigned short dptxSourceAddress;
#ifdef KYLIN_TARGET_BOARD
	static int iArrTVVid420[];
	static int intTVVid420Length;
	static int intTVSupportHDR;
	static int intTVMaxLuminace;
#endif	
};

#endif
