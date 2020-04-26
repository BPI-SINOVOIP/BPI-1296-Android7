//-----------------------------------------------------------------------------
// HDMVDecoder_CompositionBuffer.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_COMPOSITIONBUFFER_H__
#define __HDMVDECODER_COMPOSITIONBUFFER_H__

#include "ref_ptr.h"
//#include "SonicBinarySemaphore.h"
//#include "IPacketSinkCallback.h"
#include "ColorPalette.h"
#include "BaseRenderer.h"
#include "BDROM_NavigatorTypes.h"
#include "HDMVDecoder_EB.h"
#include "HDMVDecoder_Types_Internal.h"

using namespace BDROM_HDMVDecoder;

class IGDecoder_StreamGraphicsProcessor;


//__________________________________________________________________________________________________________________________________________________
// SonicHDMVSysPalette
// Container for Palette data from the IG/PG stream, and its ColorPalette
//
class SonicHDMVSysPalette
{
public:
	SonicHDMVSysPalette();
	~SonicHDMVSysPalette();

	static void SetRenderer(ref_ptr<BaseRenderer> pRenderer)	{ m_pRenderer = pRenderer; }

public:
	hdmv_palette * GetHDMVPalette(void)								{ return &m_hdmvPalette; }
	ref_ptr<SonicSurface::ColorPalette> GetColorPalette(void)		{ return m_pColorPalette; }
	void CachePalette(UInt32 uiBasePaletteIndex);					// creates color palette and caches it with HD Renderer

protected:
	hdmv_palette							m_hdmvPalette;

	ref_ptr<SonicSurface::ColorPalette>		m_pColorPalette;
	SInt32									m_uiColorPaletteIndex;

protected:
	static ref_ptr<BaseRenderer>			m_pRenderer;
};


//__________________________________________________________________________________________________________________________________________________
// HDMVDecoder_CompositionBuffer
// Represents the CB in an IG/PG Decoder.
//
class HDMVDecoder_CompositionBuffer
{
public:
	HDMVDecoder_CompositionBuffer();
	virtual ~HDMVDecoder_CompositionBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 uiMaxPalettes, UInt32 uiBasePaletteIndex, UInt32 uiMaxWindows);
	virtual void Reset(void);

public:
	hdmv_palette_definition_segment * CreatePaletteDefinitionSegment(void);			// allocates memory for a PDS (but not the Palette it defines)
	void DestroyPaletteDefinitionSegment(hdmv_palette_definition_segment * pPDS);	// deletes a PDS (but not the palette it defines)

	hdmv_palette * CreateHDMVPalette(UInt8 uiPaletteId);							// returns a Palette (existing or newly created)
	void CachePalette(UInt8 uiPaletteId);											// caches a Palette with HD Renderer
	hdmv_palette * GetHDMVPalette(UInt8 uiPaletteId);								// returns a previously created Palette
	ref_ptr<SonicSurface::ColorPalette> GetColorPalette(UInt8 uiPaletteId);			// returns SonicHDMVSysPalette (Palette info container) for a HDMV stream palette
	SonicHDMVSysPalette * GetSysPalette(UInt8 uiPaletteId);							// returns ColorPalette for a HDMV stream palette
	void DestroyHDMVPalette(UInt8 uiPaletteId);										// deletes a HDMV palette (including its color palette)

	hdmv_object_definition_segment * CreateObjectDefinitionSegment(void);			// allocates memory for an ODS
	void DestroyObjectDefinitionSegment(hdmv_object_definition_segment * pODS);		// deletes an ODS, but not the objects created by it

	hdmv_window * CreateHDMVWindow(UInt32 uiWndId);	// gets existing window, if available. If not, it creates a new window.
	hdmv_window * GetHDMVWindow(UInt32 uiWndId);	// returns a window
	void ClearHDMVWindow(UInt32 uiWndId);			// clears window, but window is still available for updation.
	void DestroyHDMVWindow(UInt32 uiWndId);			// removes window and releases all associated memory

protected:
	SonicCriticalSection 				m_csLock;	///< Lock to serialize threads accessing/modifying intenal data

private:
	ref_ptr<BaseRenderer>				m_pRenderer;

	std::vector<hdmv_object_definition_segment*>	m_ODSArray;	// array to store all allocated ODSs
	UInt32											m_uiODSUniqueId;

	std::vector<hdmv_palette_definition_segment*>	m_PDSArray;	// array to store all allocated PDSs
	UInt32											m_uiPDSUniqueId;

	UInt32							m_uiBasePaletteIndex;	// palette id added to hdmv stream palette id to create unique palette id (for caching palette with HD Renderer)

	UInt32							m_uiMaxPalettes;		// max palettes allowed
	SonicHDMVSysPalette **			m_PaletteArray;			// array of possible palettes
	UInt32							m_uiNumPalettes;		// number of palettes available

	UInt32							m_uiMaxWindows;			// max windows allowed
	hdmv_window **					m_WindowArray;			// window array
	UInt32							m_uiNumWindows;			// number of windows available
};


#endif // __HDMVDECODER_COMPOSITIONBUFFER_H__
