//-----------------------------------------------------------------------------
// WinDisplayFrame.h
// Copyright (c)  Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef _WIN_DISPLAY_FRAME_H_
#define _WIN_DISPLAY_FRAME_H_

#include "BaseDisplayFrame.h"
#include "SysSurfaceCommon.h"
#include "BaseRenderer.h"
using namespace SonicSurface;

#include "SysSurface.h"

// New frame types need to be added to the end of this list.
typedef enum tagDisplayFrameType
{
    FRAME_TYPE_NEED_TO_INIT = 0,
    FRAME_TYPE_D3D_TEXTURE, // D3D Texture Type
    FRAME_TYPE_D3D_SURFACE, // D3D Surface Type
    FRAME_TYPE_ISURFACE     // ISurface Type
} DisplayFrameType;

class WinDisplayFrame : public BaseDisplayFrame
{
public:
    
    WinDisplayFrame(BaseRenderer* pRenderer, UInt32 width, UInt32 height, SurfaceFormat format);
                    
    
    virtual ~WinDisplayFrame();
	virtual void Destroy(void);
	
    HRESULT				GetResult();
    DisplayFrameType	GetType() const;
    void 				Clear(UInt8 alpha = 0, UInt8 red = 0, UInt8 green = 0, UInt8 blue = 0) {Clear(alpha, red, green, blue, -1, 0);}
    void 				Clear(UInt8 alpha, UInt8 red, UInt8 green, UInt8 blue, int paletteID, SonicSurface::ColorPalette * pPalette);
    DirectFBSurface*	GetSurface();
    

protected:

    void Init();
    
    DisplayFrameType    		m_type;
    ref_ptr<DirectFBSurface>	m_pSurface;
    
#ifdef REALTEK_MODIFY_IG_PARTIAL_UPDATE
public:
	short m_objectID[256]; 	// the max num of BOG is 256. index is BOG number
							// Keep objectID of each BOG to prevent an object
							// from blitting twice.
	int m_pageID;			// id of current active page. If change to new
							// page, display should  be cleared.
	
	inline void ForceRedrawBOG(int index) { m_objectID[index] = -1; }
	inline bool IsBOGNeededToRedraw(int index, short objectID) { return (m_objectID[index] != objectID); }
	inline void BOGIsRedrawn(int index, short objectID) { m_objectID[index] = objectID; }
#endif

#ifdef REALTEK_MODIFY_INDEX_DISPLAY_FRAME
public:	
	int m_paletteID;
	
	inline bool IsPaletteIDSet()	{return (m_paletteID != -1); }
	inline void SetPalette(int paletteID, SonicSurface::ColorPalette * pPalette)	
		        {
		        	m_paletteID = paletteID;
		        	m_pSurface->SetPalette(*pPalette, false);
		        }
		
	inline bool IsTheSamePalette(int paletteID) { return (m_paletteID == paletteID); }
#endif

public:	
	inline SurfaceFormat GetPixelFormat() {return m_pSurface->GetPixelFormat();}

};

#endif
