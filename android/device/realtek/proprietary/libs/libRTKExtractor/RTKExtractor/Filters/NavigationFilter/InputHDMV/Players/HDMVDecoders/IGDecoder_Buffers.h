//-----------------------------------------------------------------------------
// IGDecoder_Buffers.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __IGDECODER_BUFFERS_H__
#define __IGDECODER_BUFFERS_H__

#include "ref_ptr.h"
//#include "SonicBinarySemaphore.h"
//#include "IPacketSinkCallback.h"
#include "BaseRenderer.h"
#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_EB.h"
#include "HDMVDecoder_Types_Internal.h"
#include "HDMVDecoder_CodedDataBuffer.h"
#include "HDMVDecoder_CompositionBuffer.h"
#include "HDMVDecoder_DecodedDataBuffer.h"

using namespace BDROM_HDMVDecoder;

class IGDecoder_StreamGraphicsProcessor;


//__________________________________________________________________________________________________________________________________________________
// IGDecoder_CodedDataBuffer
// Represents the EB in an IG Decoder
//
class IGDecoder_CodedDataBuffer : public HDMVDecoder_CodedDataBuffer
{
public:
	IGDecoder_CodedDataBuffer();
	virtual ~IGDecoder_CodedDataBuffer();

	void Init(IGDecoder_StreamGraphicsProcessor * pStreamGraphicsProcessor, ref_ptr<BaseRenderer> pRenderer);

protected:
	virtual SonicError ReceivePacket(ref_ptr<PACKET_DATA> packet, bool bHavePlatformDTSPTS = false, SInt64 platformDTS = 0, SInt64 platformPTS = 0);

private:
	IGDecoder_CodedDataBuffer(const IGDecoder_CodedDataBuffer &);
	IGDecoder_CodedDataBuffer& operator=(const IGDecoder_CodedDataBuffer &);

	bool					m_bDisplaySetStarted;
};



//__________________________________________________________________________________________________________________________________________________
// IGDecoder_CompositionBuffer
// Represents the CB in an IG Decoder.
//
class IGDecoder_CompositionBuffer : public HDMVDecoder_CompositionBuffer
{
public:
	IGDecoder_CompositionBuffer();
	~IGDecoder_CompositionBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer);
	virtual void Reset(void);

public:
	ig_interactive_composition_segment * CreateInteractiveCompositionSegment(void);			// allocates memory for an ICS
	UInt32 GetNumberOfICS(void);
	ig_interactive_composition_segment * GetICS(UInt32 icsId);								// get an ICS
	void DestroyInteractiveCompositionSegment(ig_interactive_composition_segment * pICS);	// deletes an ICS

	UInt32 GetICSid(UInt32 uiPageId);			// get ICS id for a given Page id.

	ig_page * CreatePage(UInt32 uiPageId);		// gets existing page, if available. If not, it creates a new page.
	ig_page * GetPage(UInt32 uiPageId);			// returns a page
	void ClearPage(UInt32 uiPageId);			// removes all buttons and their associated memory from page, but page is still available for updation.
	void DestroyPage(UInt32 uiPageId);			// removes page, its buttons and releases all associated memory

	void CreateButtonOverlapGroups(ig_page * pPage, UInt32 uiNumBOGs);							// allocate memory for button overlap groups in a page
	void CreateEffectSequenceWindows(ig_effect_sequence * pEffectSeq, UInt32 uiNumWindows);		// allocate memory for windows defined by an effect sequence
	void CreateEffects(ig_effect_sequence * pEffectSeq, UInt32 uiNumEffects);					// allocate memory for effects defined by an effect sequence
	void CreateEffectCompositionObjects(ig_effect * pEffect, UInt32 uiNumCompObjs);				// allocate memory for composition objects defined by an effect
	void CreateButtons(ig_button_overlap_group * pBOG, UInt32 uiNumButtons);					// allocate memory for buttons in a button overlap group
	void CreateNavigationCommands(ig_button * pButton, UInt32 uiNumNavCmds);					// allocate memory for navigation commands defined by a button

private:
	void DestroyButtonOverlapGroup(ig_button_overlap_group * pBOG);		// releases all memory associated with a button overlap group
	void DestroyButton(ig_button * pButton);								// releases all memory associated with a button

private:
	std::vector<ig_interactive_composition_segment*>	m_ICSArray;			// array to store all allocated ICSs
	UInt32												m_uiICSUniqueId;	// next unique ICS id

	UInt32				m_uiMaxPages;		// max pages allowed
	ig_page **			m_PageArray;		// page array
	UInt32				m_uiNumPages;		// number of pages available

private:
	IGDecoder_CompositionBuffer(const IGDecoder_CompositionBuffer&); // no copying
	const IGDecoder_CompositionBuffer& operator=(const IGDecoder_CompositionBuffer&); // no copying
};




//__________________________________________________________________________________________________________________________________________________
// IGDecoder_DecodedDataBuffer
// Represents the DB in an IG Decoder.
//
class IGDecoder_DecodedDataBuffer : public HDMVDecoder_DecodedDataBuffer
{
public:
	IGDecoder_DecodedDataBuffer();
	virtual ~IGDecoder_DecodedDataBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas);
	virtual void Reset(void);

private:
	IGDecoder_DecodedDataBuffer(const IGDecoder_DecodedDataBuffer&); // no copying
	const IGDecoder_DecodedDataBuffer& operator=(const IGDecoder_DecodedDataBuffer&); // no copying
};

#endif // __IGDECODER_BUFFERS_H__
