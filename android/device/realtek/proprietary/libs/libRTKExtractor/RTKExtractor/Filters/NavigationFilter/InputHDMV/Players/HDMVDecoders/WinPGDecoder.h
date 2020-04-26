//-----------------------------------------------------------------------------
// WinPGDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __WINPGDECODER_H__
#define __WINPGDECODER_H__


#include "HDMVDecoders_PacketSink.h"
#include "NotificationEventManager.h"

//__________________________________________________________________________________________________________________________________________________
// WinPGDecoder interface
//

class WinPGDecoder : public HDMVDecoders_PacketSink
{
public:
	WinPGDecoder() 	;
	virtual ~WinPGDecoder();

	virtual void StartOfStream(void) {}
	virtual void EndOfStream(void)	{}
	virtual void Flush(void);
	
	virtual void SetApplicationType(BDROM_ApplicationType appType) {}
	virtual void SetSyncOffsetPTS(SInt64 pts90kHz)	{}
	virtual void SetCharacterSet(BDROM_CharSet charSet)	{}
	
	virtual void Play(void) {}
	virtual void Pause(void){}
	virtual void Stop(void)	{}
	virtual void SetRate(double dRate) {}

	virtual void OnOff(bool bOn);

	
	virtual void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> pPacket)	{} // new style used by HD Nav
	virtual void ReceivePacket(UInt16 /*streamId*/, PACKET_DATA* /*pPacket*/)									{} // old style used by HD Demuxer

	void SetNotificationEventManager(ref_ptr<NotificationEventManager>	pNEManager) { m_pNEManager = pNEManager; }
	
private:
	ref_ptr<NotificationEventManager>	m_pNEManager;
	
};

#endif // __WINPGDECODER_H__
