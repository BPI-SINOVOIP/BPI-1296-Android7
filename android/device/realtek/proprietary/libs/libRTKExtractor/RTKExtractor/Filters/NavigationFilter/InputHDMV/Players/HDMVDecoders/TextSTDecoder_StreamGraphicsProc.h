//-----------------------------------------------------------------------------
// TextSTDecoder_StreamGraphicsProc.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __TEXTSTDECODER_STREAMGRAPHICSPROC_H__
#define __TEXTSTDECODER_STREAMGRAPHICSPROC_H__

#include "HDMVDecoder_StreamGraphicsProc.h"
#include "BDROM_PTSAccess.h"

using namespace BDROM_HDMVDecoder;

class TextSTDecoder_CodedDataBuffer;
class TextSTDecoder_CompositionBuffer;
class TextSTDecoder_DecodedDataBuffer;
class TextSTDecoder_GraphicsController;


//__________________________________________________________________________________________________________________________________________________
// TextSTDecoder_StreamGraphicsProcessor
// IG decoder's SGP
//

class TextSTDecoder_StreamGraphicsProcessor : public HDMVDecoder_StreamGraphicsProcessor
{
public:
	TextSTDecoder_StreamGraphicsProcessor();
	~TextSTDecoder_StreamGraphicsProcessor();

	void Init(	ref_ptr<BaseRenderer> pRenderer,
				TextSTDecoder_CodedDataBuffer * pCodedDataBuffer,
				TextSTDecoder_CompositionBuffer * pCompositionBuffer,
				TextSTDecoder_DecodedDataBuffer * pDecodedDataBuffer,
				TextSTDecoder_GraphicsController * pGraphicsController,
				ref_ptr<BDROM_PTSAccess> pPtsAccess);
	void Reset(void);

	// BDROM TextST Decoder API
	//
public:
	SonicError GetNumberOfUserStyles(UInt32 & numStyles);

	// Event processing
	//
public:
	void SegmentReceived(void);
	void StartOfStream(void);
	void EndOfStream(void);
	void Play(void);
	void Stop(void);
	void Flush(void);
	void SetSyncOffsetPTS(SInt64 pts);
	void EnableDisplay(bool bOn);

protected:
	void TimerEvent(void);
	virtual bool GetCurrentTS(Time90K & ts90kTimestamp);		// returns current timestamp clock value
	UInt32 GetSleepDuration(void);

	void ProcessSegmentReceived(void);
	void ProcessTimerEvent(void);
	void ProcessStartOfStream(void);
	void ProcessEndOfStream(void);
	void ProcessFlush(void);
	void ProcessRateChange(double dRate);
	void ProcessEnableDisplay(bool bOn);

	UInt32 LocateDPS(UInt64 pts90kHz);
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

	// TextST stream parsing
	//
private:
	void ParseDialogStyleSegment(txt_dialog_style_segment * pDSS, HDMVDec_EBPtr & ebPtr);
	void ParseDialogPresentationSegment(txt_dialog_presentation_segment * pDPS, HDMVDec_EBPtr & ebPtr);
	void ParseRegionStyle(txt_region_style * pRGS, HDMVDec_EBPtr & ebPtr);
	void ParseUserControlStyle(txt_user_control_style * pUCS, HDMVDec_EBPtr & ebPtr);
	void ParseTextSTPalette(hdmv_palette * pPalette, HDMVDec_EBPtr & ebPtr);
	void ParseDialogRegion(txt_dialog_region * pDlgRegion, HDMVDec_EBPtr & ebPtr);
	void ParseRegionSubtitle(txt_region_subtitle_data * pSubtitle, HDMVDec_EBPtr & ebPtr);

	void ProcessGapAfterCurrentDPS(void);

private:
	SGPState						m_State;

//	UInt64							m_pts90kStartPTS;		// 90KHz PTS to start decoding from (Main Path PTS)
	bool							m_bDSSdecoded;			// true if DSS has already been decoded.
	UInt32							m_uiCurrDPSnum;			// DPS number of DPS being decoded or the next DPS to be decoded
	bool							m_bAllDataRcvd;			// true if all Text data has been received

	// TextST stream parsing
	//
private:
	void GetCompositionState(HDMVCompositionState & compState, HDMVDec_EBPtr & ebPtr);

private:
	TextSTDecoder_StreamGraphicsProcessor(const TextSTDecoder_StreamGraphicsProcessor&); // no copying
	const TextSTDecoder_StreamGraphicsProcessor& operator=(const TextSTDecoder_StreamGraphicsProcessor&); // no copying
};

#endif // __TEXTSTDECODER_STREAMGRAPHICSPROC_H__
