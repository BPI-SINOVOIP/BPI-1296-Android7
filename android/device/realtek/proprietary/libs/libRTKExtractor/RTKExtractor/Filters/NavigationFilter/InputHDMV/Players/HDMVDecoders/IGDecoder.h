//-----------------------------------------------------------------------------
// IGDecoder.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __IGDECODER_H__
#define __IGDECODER_H__

#include "ref_ptr.h"
#include "BaseRenderer.h"
#ifdef USE_MM_TIMER
#include "BaseTimer.h"
#endif
//#include "BaseVideoSprite.h"
#include "HDMVDecoders_PacketSink.h"
#include "InteractiveGraphicsDecoder.h"
#include "SonicPBCE_Registers.h"
#include "BDROM_SoundPlayer.h"
#include "BDROM_NavigatorTypes.h"
#include "BDROM_PTSAccess.h"
#include "HDMVDecoders_DeviceLost.h"
#include "IGDecoder_GraphicsController.h"

class IGDecoder_CodedDataBuffer;
class IGDecoder_CompositionBuffer;
class IGDecoder_DecodedDataBuffer;
class IGDecoder_StreamGraphicsProcessor;

#ifdef USE_MM_TIMER
class BaseTimer;
#endif

//__________________________________________________________________________________________________________________________________________________
// IGTimerNotifiee
// Notifiee for IG Timer
//

#ifdef USE_MM_TIMER
class IGTimerNotifiee : public SonicTimerNotifiee
{
public:
	IGTimerNotifiee(IGDecoder * pDecoder);
	~IGTimerNotifiee();

	// override for receiving timer notifications
	virtual void OnTimerExpired();

private:
	IGDecoder * m_pDecoder;
};
#endif


//__________________________________________________________________________________________________________________________________________________
// IGDecoder
// Sonic's implementation of a BDROM IG Decoder
//

class IGDecoder : public InteractiveGraphicsDecoder, public HDMVDecoders_PacketSink
{
public:
	IGDecoder();
	virtual ~IGDecoder();

#ifdef USE_MM_TIMER
	void Init(ref_ptr<BaseRenderer> pRenderer,
									UInt32 rendererCanvas,
									ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
									ref_ptr<BaseTimer> pTimer,
									BDROM_DiscType discType,
									ref_ptr<BDROM_PTSAccess> pPtsAccess);
#else  // USE_MM_TIMER
	void Init(ref_ptr<BaseRenderer> pRenderer,
									UInt32 rendererCanvas,
									ref_ptr<BDROM_SoundPlayer> pSoundPlayer,
									BDROM_DiscType discType,
									ref_ptr<BDROM_PTSAccess> pPtsAccess);
#endif // USE_MM_TIMER

	// setup event notification channel, registers
	//
	virtual void SetNotificationEventManager(ref_ptr<NotificationEventManager>	pNEManager);
	void SetRegisters(ref_ptr<BDROM_SonicPBCE_Registers> pRegisters);

	// Interactive User Operations
	//
	virtual SonicError MoveUpSelectedButton(void);
	virtual SonicError MoveDownSelectedButton(void);
	virtual SonicError MoveLeftSelectedButton(void);
	virtual SonicError MoveRightSelectedButton(void);
	virtual SonicError SelectButton(UInt32 uiButtonNumber);
	virtual SonicError ActivateButton(void);
	virtual SonicError SelectButtonAndActivate(UInt32 uiButtonNum);
	virtual bool       IsPopUpAvailable(void);
	virtual bool       IsPopUpOn(void);
	virtual SonicError PopUpMenu(void);
	virtual SonicError PopUpOn(void);
	virtual SonicError PopUpOff(void);

	// Mouse input functions
	//
	virtual SonicError MousePosition(SInt32 x, SInt32 y, bool & bOnClickableButton);
	virtual SonicError MouseClick(SInt32 x, SInt32 y);

	// Button commands related functions (called by Module Manager)
	//
	virtual SonicError SetButtonPage(bool bSetPage, UInt8 uiPageId, bool bCurrPageOutEffect, bool bSetButton, UInt16 uiButtonId);
	virtual SonicError EnableButton(UInt32 uiButtonId);
	virtual SonicError DisableButton(UInt32 uiButtonId);
	virtual void       BtnCmdsExecuted(ButtonCmdsExecutionInfo & btnCmdsExecInfo);					// Btn cmd execution notification

	// timer notifications
	//
	void OnTimerExpired();

	// device lost handling
	//
	virtual void DeviceLost(void);
	virtual void DeviceNew(void);

	// Packet Sink interface implementation
	//
	virtual void Play(void);
	virtual void Pause(void);
	virtual void Stop(void);
	virtual void SetRate(double dRate);

	virtual void StartOfStream(void);
	virtual void EndOfStream(void);
	virtual void Flush(void);

	virtual void SetApplicationType(BDROM_ApplicationType appType);
	virtual void SetSyncOffsetPTS(SInt64 pts);
	virtual void SetCharacterSet(BDROM_CharSet /*charSet*/)		{}		// nothing to be done in IG Decoder

	virtual void ReceivePacket(UInt32 id, UInt16 streamId, UInt16 subStreamId, ref_ptr<PACKET_DATA> packet);
	virtual void ReceivePacket(UInt16 streamId, PACKET_DATA* pPacket);		// old style

	virtual void OnOff(bool bOn);

	virtual int	GetTotalNumOfButton() {	return (m_pController ? m_pController->GetTotalButtonNumOfCurPage() : 0); }
	virtual bool IsIGDisplay() { return (m_pController ? m_pController->IsPopUpOn() : 0); }
	
protected:
	// creation and initialization of IG Decoder components. Derived clases should override necessary functions if their creation/init is different.
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
	typedef enum tagIGDecoderState
	{
		kIGDecState_Stopped,		// Stopped state (i.e. is reset)
		kIGDecState_Paused,			// Paused
		kIGDecState_Playing			// Playing
	} IGDecoderState;

protected:
	ref_ptr<BaseRenderer>					m_pRenderer;
	UInt32									m_RendererCanvas;
	ref_ptr<BDROM_SoundPlayer>				m_pSoundPlayer;
	BDROM_DiscType							m_DiscType;
	ref_ptr<BDROM_PTSAccess>				m_pPtsAccess;

#ifdef USE_MM_TIMER
	ref_ptr<BaseTimer>						m_pTimer;
	IGTimerNotifiee *						m_pTimerNotifiee;
#endif

	IGDecoderState							m_State;
	double									m_dRate;

	IGDecoder_CodedDataBuffer			*	m_pCodedDataBuffer;
	IGDecoder_CompositionBuffer			*	m_pCompositionBuffer;
	IGDecoder_DecodedDataBuffer			*	m_pDecodedDataBuffer;

	IGDecoder_GraphicsController		*	m_pController;
	IGDecoder_StreamGraphicsProcessor	*	m_pProcessor;

	SonicCriticalSection					m_csLock;				// to ensure thread safety for stream control (i.e. StartOfStream, Flush, Stop, ReceivePacket, ...)

	// device lost handling
	DeviceState								m_RenderDeviceState;

private:
	IGDecoder(const IGDecoder&); // no copying
	const IGDecoder& operator=(const IGDecoder&); // no copying
};

#endif
