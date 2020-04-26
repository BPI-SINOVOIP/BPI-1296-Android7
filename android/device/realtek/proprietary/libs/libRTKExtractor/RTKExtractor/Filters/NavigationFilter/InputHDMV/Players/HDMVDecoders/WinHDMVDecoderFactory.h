//-----------------------------------------------------------------------------
// WinHDMVDecoderFactory.h
// Copyright (c) 2007, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINHDMVDECODERFACTORY_H__
#define __WINHDMVDECODERFACTORY_H__

#include "ref_ptr.h"
#include "BaseRenderer.h"
#include "BDROM_SoundPlayer.h"
#include "BDROM_NavigatorTypes.h"
#include "DisplayLayer.h"
#include "BDROM_PTSAccess.h"


class WinIGDecoder;
class WinPGDecoder;
class WinTextSTDecoder;
class InteractiveGraphicsDecoder;
class PresentationGraphicsDecoder;
class TextSubtitleDecoder;
class HDMVDecoders_PacketSink;
class HDMVDecoders_DeviceLost;
class NotificationEventManager;
class BDROM_SonicPBCE_Registers;


class WinHDMVDecoderFactory
{
public:
	WinHDMVDecoderFactory()		{}
	~WinHDMVDecoderFactory()	{}

	// creation and deletion
	//

	static bool CreateIGDecoder(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas,
								ref_ptr<BDROM_SoundPlayer> pSoundPlayer, DisplayLayer * pDisplayLayer, BDROM_DiscType discType, ref_ptr<BDROM_PTSAccess> pPtsAccess);
	static bool CreatePGDecoder();
	static bool CreateTextSTDecoder(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas, DisplayLayer * pDisplayLayer, ref_ptr<BDROM_PTSAccess> pPtsAccess);

	static void SetIGNotificationEventManager(ref_ptr<NotificationEventManager> pEventManager);
	static void SetPGNotificationEventManager(ref_ptr<NotificationEventManager> pEventManager);
	static void SetTextSTNotificationEventManager(ref_ptr<NotificationEventManager> pEventManager);

	static void SetIGRegisters(ref_ptr<BDROM_SonicPBCE_Registers> pRegisters);

	static void DestroyIGDecoder(void);
	static void DestroyPGDecoder(void);
	static void DestroyTextSTDecoder(void);

	// query interfaces
	//

	static InteractiveGraphicsDecoder  * GetInteractiveGraphicsDecoder(void);
	static WinPGDecoder * GetPresentationGraphicsDecoder(void);
	static TextSubtitleDecoder * GetTextSubtitleDecoder(void);

	static HDMVDecoders_PacketSink * GetIGPacketSink(void);
	static HDMVDecoders_PacketSink * GetPGPacketSink(void);
	static HDMVDecoders_PacketSink * GetTextSTPacketSink(void);

	static HDMVDecoders_DeviceLost * GetIGDeviceLost(void);
	//static HDMVDecoders_DeviceLost * GetPGDeviceLost(void);
	//static HDMVDecoders_DeviceLost * GetTextSTDeviceLost(void);

protected:
	static WinIGDecoder     * m_pIGDecoder;
	static WinPGDecoder     * m_pPGDecoder;
	static WinTextSTDecoder * m_pTextSTDecoder;
};

#endif // __WINHDMVDECODERFACTORY_H__
