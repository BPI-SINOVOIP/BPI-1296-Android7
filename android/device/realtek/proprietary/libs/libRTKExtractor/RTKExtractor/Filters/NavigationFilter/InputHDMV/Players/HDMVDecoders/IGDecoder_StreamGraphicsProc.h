//-----------------------------------------------------------------------------
// IGDecoder_StreamGraphicsProc.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __IGDECODER_STREAMGRAPHICSPROC_H__
#define __IGDECODER_STREAMGRAPHICSPROC_H__

#include "HDMVDecoder_StreamGraphicsProc.h"
#include "SonicSemaphore.h"
#include "BDROM_PTSAccess.h"

using namespace BDROM_HDMVDecoder;

class IGDecoder_CodedDataBuffer;
class IGDecoder_CompositionBuffer;
class IGDecoder_DecodedDataBuffer;
class IGDecoder_GraphicsController;


//__________________________________________________________________________________________________________________________________________________
// IGDecoder_StreamGraphicsProcessor
// IG decoder's SGP
//

class IGDecoder_StreamGraphicsProcessor : public HDMVDecoder_StreamGraphicsProcessor
{
public:
	IGDecoder_StreamGraphicsProcessor();
	virtual ~IGDecoder_StreamGraphicsProcessor();

	void Init(ref_ptr<BaseRenderer> pRenderer,
										IGDecoder_CodedDataBuffer * pCodedDataBuffer,
										IGDecoder_CompositionBuffer * pCompositionBuffer,
										IGDecoder_DecodedDataBuffer * pDecodedDataBuffer,
										IGDecoder_GraphicsController * pGraphicsController,
										BDROM_DiscType discType,
										ref_ptr<BDROM_PTSAccess> pPtsAccess);
	void Reset(void);

	// get info
	IGStreamModel GetStreamModel(void)					{ return m_StreamModel; }
	IGUserInterfaceModel GetUserInterfaceModel(void)	{ return m_UserInterfaceModel; }

	// Event processing
	//
public:
	void SegmentReceived(void);
	void StartOfStream(void);
	void EndOfStream(void);
	virtual void Flush(void);

protected:
	void TimerEvent(void);
	bool GetCurrentTS(Time90K & ts90kTimestamp);		// returns current timestamp clock value
	UInt32 GetSleepDuration(void);

	void ProcessSegmentReceived(void);
	void ProcessTimerEvent(void);
	void ProcessStartOfStream(void);
	void ProcessEndOfStream(void);
	void ProcessFlush(void);
	void ProcessRateChange(double dRate);

	SonicError DecodeSegment(HDMVSegmentType segmentType);

protected:
	typedef enum tagSGPState
	{
		// State						Description										Allowed transitions
		// -----					-------------------------------------------------	------------------------------------
		kSGPState_Idle,				// Idle. No decoding possible.						Idle -> Reset
		kSGPState_Reset,			// Reset and ready.									Reset -> DecodePrepare/Scan
		kSGPState_DecodePrepare,	// Prepare for decoding (look for ES/EC segment).	DecodePrepare -> Decode/Scan
		kSGPState_Decode,			// Decoding.										Decode -> EOS/Scan
		kSGPState_Scan,				// Scanning (dropping all data)						Scan -> DecodePrepare
		kSGPState_EndOfStream		// Finished decoding complete IG Stream.			EOS -> Reset/Idle
	} SGPState;

private:
	SGPState						m_State;
	IGStreamModel					m_StreamModel;			// IG stream type - Async or Sync
	IGUserInterfaceModel			m_UserInterfaceModel;	// IG User interace model - PopUp or AlwaysOn
	BDROM_DiscType					m_discType;				// BDROM or AVCHD

	bool							m_bEndOfStreamData;		// true when all IG data has been received
	SonicSemaphore			m_evtSGPEndOFStream;	// used to signal when SGP has finished decoding the whole IG stream

	UInt32							m_CurrIcsId;			// ID of current ICS being decoded.
	bool							m_bSleepNone;			// Used to indicate to GetSleepDuration to use no duration, instead of 60hz.

	// IG stream parsing
	//
private:
	void ParseInteractiveCompSegment(ig_interactive_composition_segment * pICS, HDMVDec_EBPtr & ebPtr);
	void ParseInteractiveComposition(ig_interactive_composition * pIC, HDMVDec_EBPtr & ebPtr);
	void ParsePage(ig_page * pPage, HDMVDec_EBPtr & ebPtr);
	void ParseEffectSequence(ig_effect_sequence * pEffectSeq, HDMVDec_EBPtr & ebPtr);
	void ParseEffect(ig_effect * pEffect, HDMVDec_EBPtr & ebPtr);
	void ParseButtonOverlapGroup(ig_button_overlap_group * pBOG, HDMVDec_EBPtr & ebPtr);
	void ParseButton(ig_button * pButton, HDMVDec_EBPtr & ebPtr);

	void GetCompositionState(HDMVCompositionState & compState, HDMVDec_EBPtr & ebPtr);
	void GetStreamModel(IGStreamModel & streamModel, HDMVDec_EBPtr & ebPtr);
	void GetUserInterfaceModel(IGUserInterfaceModel & userInterfaceModel, HDMVDec_EBPtr & ebPtr);

private:
	IGDecoder_StreamGraphicsProcessor(const IGDecoder_StreamGraphicsProcessor&); // no copying
	const IGDecoder_StreamGraphicsProcessor& operator=(const IGDecoder_StreamGraphicsProcessor&); // no copying
};

#endif // __IGDECODER_STREAMGRAPHICSPROC_H__
