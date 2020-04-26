//-----------------------------------------------------------------------------
// TextSTDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __TEXTSTDECODER_H__
#define __TEXTSTDECODER_H__

#include "ref_ptr.h"
#include "BaseRenderer.h"
#include "HDMVDecoders_PacketSink.h"
#include "TextSubtitleDecoder.h"
#include "SonicPBCE_Registers.h"
#include "NotificationEventManager.h"
#include "BDROM_PTSAccess.h"
#include "HDMVDecoders_DeviceLost.h"

class TextSTDecoder_CodedDataBuffer;
class TextSTDecoder_CompositionBuffer;
class TextSTDecoder_DecodedDataBuffer;
class TextSTDecoder_GraphicsController;
class TextSTDecoder_StreamGraphicsProcessor;


//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder
// Sonic's implementation of a BDROM TextST Decoder
//

class TextSTDecoder : public TextSubtitleDecoder, public HDMVDecoders_PacketSink
{
public:
	TextSTDecoder();
	virtual ~TextSTDecoder();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas, ref_ptr<BDROM_PTSAccess> pPtsAccess);

	// setup event notification channel, registers
	//
	virtual void SetNotificationEventManager(ref_ptr<NotificationEventManager>	pNEManager);
	void SetRegisters(ref_ptr<BDROM_SonicPBCE_Registers> pRegisters);

	// TextST User Operations
	//
	SonicError SelectUserStyleNumber(UInt32 styleNum);
	SonicError GetNumberOfUserStyles(UInt32 & numStyles);
	SonicError EnableDisplay(bool bOn);

	// timer notifications
	//
	void OnTimerExpired();

	// device lost handling
	//
	void DeviceLost(void);
	void DeviceNew(void);

	// media control & Packet Sink interface implementation
	//
	void Play(void);
	void Pause(void);
	void Stop(void);
	void SetRate(double dRate);

	void StartOfStream(void);
	void EndOfStream(void);
	void Flush(void);

	// HDMVStreamTimes will have In_location, Out_location, In_time, and Out_time
	// This data becomes valid at In_location.
	virtual void SetStreamExtent(HDMVStreamExtent & extent);

	// This is the start location for the stream. Location is specified as a start location in the main path
	virtual void SetStartLocation(HDMVStreamPlaylistLocation & startLocation);

	virtual void SetApplicationType(BDROM_ApplicationType appType);
	virtual void SetSyncOffsetPTS(SInt64 pts);
	virtual void SetCharacterSet(BDROM_CharSet charSet);

	// primary video size
	virtual void SetVideoSize(UInt32 width, UInt32 height);

	void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> packet);
	void ReceivePacket(UInt16 streamId, PACKET_DATA* pPacket);		// old style
	void OnOff(bool bOn);

protected:
	// creation and initialization of TextST Decoder components. Derived clases should override necessary functions if their creation/init is different.
	//
	virtual void CreateCodedDataBuffer(void);
	virtual void CreateCompositionBuffer(void);
	virtual void CreateDecodedDataBuffer(void);
	virtual void CreateStreamGraphicsProcessor(void);
	virtual void CreateGraphicsController(void);
	virtual void InitCodedDataBuffer(void);
	virtual void InitCompositionBuffer(void);
	virtual void InitDecodedDataBuffer(void);
	virtual void InitStreamGraphicsProcessor(void);
	virtual void InitGraphicsController(void);

protected:
	typedef enum tagTextSTDecoderState
	{
		kTextSTDecState_Stopped,		// Stopped state (i.e. is reset)
		kTextSTDecState_Paused,			// Paused
		kTextSTDecState_Playing			// Playing
	} TextSTDecoderState;

protected:
	ref_ptr<BaseRenderer>						m_pRenderer;
	UInt32										m_RendererCanvas;
	ref_ptr<BDROM_PTSAccess>					m_pPtsAccess;

	TextSTDecoderState							m_State;

	TextSTDecoder_CodedDataBuffer			*	m_pCodedDataBuffer;
	TextSTDecoder_CompositionBuffer			*	m_pCompositionBuffer;
	TextSTDecoder_DecodedDataBuffer			*	m_pDecodedDataBuffer;

	TextSTDecoder_GraphicsController		*	m_pController;
	TextSTDecoder_StreamGraphicsProcessor	*	m_pProcessor;

	SonicCriticalSection						m_csLock;		// to ensure thread safety for stream control (i.e. StartOfStream, Flush, Stop, ReceivePacket, ...)

	// device lost handling
	DeviceState									m_RenderDeviceState;

private:
	TextSTDecoder(const TextSTDecoder&); // no copying
	const TextSTDecoder& operator=(const TextSTDecoder&); // no copying
};

#endif // __TEXTSTDECODER_H__
