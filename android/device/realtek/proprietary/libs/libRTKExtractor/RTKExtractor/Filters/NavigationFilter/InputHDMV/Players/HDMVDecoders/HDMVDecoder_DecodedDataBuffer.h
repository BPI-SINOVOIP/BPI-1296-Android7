//-----------------------------------------------------------------------------
// HDMVDecoder_DecodedDataBuffer.h
// Copyright (c) 2000 - 2005, Sonic Solutions.  All rights reserved.
//-----------------------------------------------------------------------------

#ifndef __HDMVDECODER_DECODEDDATABUFFER_H__
#define __HDMVDECODER_DECODEDDATABUFFER_H__

#include "ref_ptr.h"
#include "SysImageSprite.h"
#include "ColorPalette.h"
#include "BaseRenderer.h"
#include "HDMVDecoder_Types_Internal.h"
#include "HDMVDecoders_DeviceLost.h"	//DeviceState

using namespace BDROM_HDMVDecoder;

#define DBG_LOG_VISIBILITY_CHANGE			(WIN32 && _DEBUG && 0)
#if DBG_LOG_VISIBILITY_CHANGE
static char g_dbgl_vis[256];
#endif

//__________________________________________________________________________________________________________________________________________________
// HDMVSysObject
// Container for Object data description, Decoded object data, its Spite and its Surface
//
class HDMVSysObject
{
public:
	HDMVSysObject();
	virtual ~HDMVSysObject();

	static void SetRenderer(ref_ptr<BaseRenderer> pRenderer)			{ m_pRenderer = pRenderer; }
	static void SetRendererCanvas(UInt32 rendererCanvas)				{ m_RendererCanvas = rendererCanvas; }
	static void SetRenderDeviceState(DeviceState renderDeviceState)		{ m_RenderDeviceState = renderDeviceState; }

	// device lost handling
	void ReleaseDeviceResources(void);
	void ObtainDeviceResources(UInt32 uiZdepth);

public:
	hdmv_object * CreateHDMVObject(void);					// creates the hdmv_object for storing parsed object description
	virtual bool CreateHDMVObjectDecodeBuffer(void);		// creates the decode buffer into which the RLE data will be decoded
	bool CreateHDMVObjectBitmapBuffer(void);				// Creates surface for source bitmap to be created.
	virtual SonicError CacheHDMVObject(UInt32 uiZdepth);	// caches created object with the renderer (creates surface, sprite, ...)
	hdmv_object * GetHDMVObject(void)									{ return m_object; }
	void DestroyHDMVObject(void);							// releases hdmv_object, decode buffer, sprites, surface.

	virtual bool IsValid(void)											{ return (m_pSprite.get() != 0); }

	SonicSurface::SysHDSurface * GetSurface(void)						{ return m_pSurface.get(); }

	virtual void SetPalette(UInt16 uiObjId, UInt8 uiObjVersion, UInt8 uiPaletteId, UInt8 uiPaletteVersion, SInt32 siPaletteIndex, ref_ptr<SonicSurface::ColorPalette> pPalette);
	virtual void SetPosition(UInt32 uiTop, UInt32 uiLeft)				{ m_uiLeft = (UInt16)uiLeft; m_uiTop = (UInt16)uiTop;
																		  if (m_pSprite)
																		  {
																			m_pSprite->SetDstRect(PiRect(PiPoint(uiLeft, uiTop),
																						   PiSize(m_object->mobject_width, m_object->mobject_height)));
																		  }
																		}
	void SetVisibility(bool bVisible)
																		{
																		#if DBG_LOG_VISIBILITY_CHANGE
																		  sprintf(g_dbgl_vis, "VisChg: Obj_id= %d, vis %c -> %c\n",
																							m_object->mobject_id, m_bVisible ? 'Y' : 'N', bVisible ? 'Y' : 'N');
																		  OutputDebugString(g_dbgl_vis);
																		#endif
																		  m_bVisible = bVisible;
																		  if (m_pSprite)
																			m_pSprite->SetVisibility(bVisible);
																		}
	void SetZDepth(UInt32 uiZdepth)										{ if (m_pSprite) m_pSprite->SetZDepth(uiZdepth); }
	virtual void SetDstRect(PiRect & dstRect)							{ if (m_pSprite) m_pSprite->SetDstRect(dstRect); }
	virtual void SetSrcRect(UInt32 left, UInt32 top, UInt32 right, UInt32 bottom);
	void CommitChanges(void)											{ }

	void   GetDisplayRect(PiRect & dispRect)							{ dispRect = PiRect(PiPoint(m_uiLeft, m_uiTop),
																							PiSize(m_object->mobject_width, m_object->mobject_height)); }
	UInt32 GetWidth(void)												{ return m_uiObjDataWidth;					}
	UInt32 GetHeight(void)												{ return m_uiObjDataHeight;					}
	void GetObjId(UInt16 & uiObjId, UInt8 & uiObjVersion)				{ uiObjId = m_uiObjId;	uiObjVersion = m_uiObjVersion; }
	void GetPaletteId(UInt8 & uiPaletteId, UInt8 & uiPaletteVersion)	{ uiPaletteId = m_uiPaletteId;	uiPaletteVersion = m_uiPaletteVersion; }
	bool IsVisible(void)												{ return m_bVisible;						}

protected:
	void ReleaseSysObject(void);								// releases sprite and surface created for object
	void DestroyHDMVDecodeObjectBuffer(void);					// releases decode buffer. also releases sprites, surface.

	virtual void DrawObjectToSurface(SonicSurface::SurfaceFormat destFormat, SonicSurface::ColorPalette * pPalette);	// updates image with palette (draws onto the sprite surface if necessary)

	virtual void RendererPrepare()										{ m_pRenderer->Prepare(); }
	virtual void RendererCommit()										{ m_pRenderer->Commit(); }

protected:
	hdmv_object *							m_object;			// object data description and decoded data

	ref_ptr<SysImageSprite>					m_pSprite;			// sprite for the object
	ref_ptr<SonicSurface::SysHDSurface>		m_pSurface;			// surface used by sprite, object
	ref_ptr<BaseImageRef>					m_pImageRef;		// image ref used to bind surface to sprite
	bool									m_bHasSprite;		// if true, then a sprite is created and associated with surface. if not, only surface is created

	UInt16									m_uiLeft;
	UInt16									m_uiTop;
	UInt16									m_uiObjDataWidth;	// width of object data last allocated
	UInt16									m_uiObjDataHeight;	// height of object data last allocated
	UInt16									m_uiObjId;			// ID of object last used.
	UInt8									m_uiObjVersion;		// version number of last object used.
	UInt8									m_uiPaletteId;		// ID of palette last used.
	UInt8									m_uiPaletteVersion;	// version number of last palette used.

	PiRect									m_srcRect;

	bool									m_bVisible;			// visibility status (temporary. TODO: find better way)

protected:
	static ref_ptr<BaseRenderer>			m_pRenderer;
	static UInt32							m_RendererCanvas;	// canvas that all sprites should be associated with
	static DeviceState						m_RenderDeviceState;
};


//__________________________________________________________________________________________________________________________________________________
// HDMVDecoder_DecodedDataBuffer
// Represents the DB in an IG/PG Decoder.
//
class HDMVDecoder_DecodedDataBuffer
{
public:
	HDMVDecoder_DecodedDataBuffer();
	virtual ~HDMVDecoder_DecodedDataBuffer();

	void Init(ref_ptr<BaseRenderer> pRenderer, UInt32 rendererCanvas, UInt32 uiMaxObjects, UInt32 uiZdepth);
	virtual void Reset(void);

	virtual void SetRenderDeviceState(DeviceState renderDeviceState);

public:
	hdmv_object * CreateHDMVObject(UInt32 uiObjectId);		// creates new or returns existing object for given object_id
	void CreateHDMVObjectDecodeBuffer(UInt32 uiObjectId);	// gets memory for RLE data to be decoded into. CreateObject() must have been called & hdmv_object initialized.

	// 2 special functions to allow TextST decoder to use the SysHDMVObject to store text bitmap
	// * call the following CreateHDMVObject() instead of the regular CreateHDMVObject()
	// * call CreateHDMVBitmapBuffer() instead of CreateHDMVObjectDecodeBuffer() to store the text bitmap. (since no RLE images are involved in TextST)
	//   set object width and height before calling this function.
	//
	bool CreateHDMVObject(hdmv_object *& pObject, UInt32 & uiObjectId);	// creates new object (always) and returns its ptr & object_id
	bool CreateHDMVObjectBitmapBuffer(UInt32 uiObjectId);				// Creates surface for source bitmap to be created. CreateObject() must have 
																		// been called & hdmv_object initialized.

	virtual SonicError CacheHDMVObject(UInt32 uiObjectId);				// creates sprite and caches it with the HD Renderer.

	hdmv_object * GetHDMVObject(UInt32 uiObjectId);						// returns a hdmv_object
	HDMVSysObject * GetSysObject(UInt32 uiObjectId);					// returns a object container

//	void DestroyHDMVDecodeObjectBuffer(UInt32 uiObjectId);				// releases decoded data memory for an object
	void DestroyHDMVObject(UInt32 uiObjectId);							// releases all resources used by an object (including the sprite)

protected:
	// creates a single instance of HDMVSysObject. derived class should override this if they need to create their own version of HDMVSysObject
	virtual HDMVSysObject * CreateHDMVSysObject(void)		{ HDMVSysObject * pObj = new HDMVSysObject(); return pObj; }
	virtual void RendererPrepare()							{ m_pRenderer->Prepare(); }
	virtual void RendererCommit()							{ m_pRenderer->Commit(); }

protected:
	UInt32							m_uiZdepth;			///< Z-depth for all object managed by this DecodedDataBuffer

	SonicCriticalSection 			m_csLock;			///< Lock to serialize threads accessing/modifying intenal data

	ref_ptr<BaseRenderer>			m_pRenderer;

	UInt32							m_uiMaxObjects;		///< max objects allowed
	HDMVSysObject **				m_ObjectArray;		///< array of object pointers. To be indexed using object id.
	UInt32							m_uiNumObjects;		///< number of objects available
	//improve loops by enabling the use this var
	//UInt32						m_CeilingObjectIndex;	// largest index in m_ObjectArray that contains a valid SysHDMVObject.

	DeviceState						m_RenderDeviceState;	// state of render device. used to manage creation of resources like surfaces...
};

#endif // __HDMVDECODER_DECODEDDATABUFFER_H__
