//------------------------------------------------------------------------------
// BaseRenderer.h
// Copyright (c) 2005, Sonic Solutions.  All rights reserved.
//------------------------------------------------------------------------------

#ifndef BASE_RENDERER_H
#define BASE_RENDERER_H

#define REALTEK_MODIFY_ENABLE_CACHE
//#include "RendererTypes.h"
#include "ref_ptr.h"
#include "PiPoint.h"
#include "PiRect.h"
#include "PiRGBColor.h"
#include "ColorPalette.h"
#include "SonicCriticalSection.h"
//#include "SonicThread.h"
#include "SysSurface.h"
#include "SysSurfaceCommon.h"
//#include "SysHDSurfaceForward.h"
#include "UnicodeUtilities.h"
#ifndef REALTEK_MODIFY_ENABLE_CACHE
#include "ObjectCache.h"
#endif
#include "TimeValue.h"
#include "ImageRef.h"
#include "BasePrecisionClock.h"
#include "typedesc.h"
#include "parserfactory.h" // AddStream()
//#include "PlaybackModes.h"
#include "HDRInterfaceIDs.h"
#include "rt_stream.h"
//#include "BaseSprite.h"
#include <vector>
#include "RendererMediaEventType.h"

// Forward declarations
class BaseSprite;
//class BaseAudioSprite;
//class BaseGraphSprite;
//class BaseImageSprite;
//class BaseTextSprite;
//class BaseVideoSprite;
//class BaseStencilRect;
//class BaseClipRect;
class BasePacketCallback;
//class BaseSoundParser;
class DisplayLayer;
class BaseDisplayFrame;
//class BaseAVOutputPortCaps;
//class BaseSurfaceOps;
//class HDRSurface_BaseSurface;

//typedef std::vector< ref_ptr<BaseSprite> > SpriteArray;
//typedef std::vector< ref_ptr<BaseStencilRect> > StencilRectArray;
//typedef std::vector< ref_ptr<BaseClipRect> > ClipRectArray;

typedef UInt32 SoundID;
const SInt32 kInfiniteSoundLooping = -1;
class RENDERER_EXPORT BaseSoundFinishedCallback
{
public:
	BaseSoundFinishedCallback() { }
	virtual ~BaseSoundFinishedCallback() { }
	virtual void operator()	(SoundID /*id*/) { }
};

const PrecisionClockID kSystemClockID   = 1;  // added by base renderer, always available and always incrementing
const PrecisionClockID kMediaPtsClockID = 2;  // when supported, added and managed by derived renderers, 
                                              // returns pts of the media (MainPathPts) 
                                              // Depending on the target implementation, the derived
                                              // renderers may get the Media PTS from the Hw / drivers or
                                              // from the Player's actual streams

//-----------------------------------------------------------------------
// Error callback
//
//class HDRErrorCallback;
//typedef std::vector< HDRErrorCallback* >	HDRErrorCallbackArray;
//typedef HDRErrorCallbackArray::iterator		HDRErrorCallbackIter;

// Error Code Type
//
// Error Code Format:
// Bit  31		: Always 1
// Bits 28-30	: Severity (Warning, Error)
// Bits 24-27	: reserved
// Bits 23-16	: Category
// Bits 16-0	: unique number within category
//
typedef SInt32		HDRErrorType;

// Macros to make error codes
//

// Severity: All severity values to be defined here
#define HDRET_SEVERITY_WARNING		0
#define HDRET_SEVERITY_ERROR		1

// Category: derived classes can define more
// (values 0x00 - 0x3F reserved for common/base errors)
#define HDRET_CATEGORY_BASE			0

// Make unique error code
#define MAKE_HDRERRORTYPE(sev, cat, num)		(0x80000000 | ((sev & 0x7) << 28) | ((cat & 0xFF) << 16) | (num & 0xFFFF))


// Struct for carrying additional info.
// Derived class can define error structs per error
//struct HDRErrorInfo
//{
//};

// HDRErrorCallback: Clients can (or rather should) register a HDRErrorCallback with the HDR
// to be notified of a errors (like failure to re-connect a VMR on a video type change that requires
// a re-connect).
//
/*
class HDRErrorCallback
{
public:
	virtual void Error(HDRErrorType err, HDRErrorInfo * errData)	= 0;
};*/

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// PTS related enums, interfaces
//

// Notes on PTS notations:
// -----------------------
//	Encoded PTS		: This is the PTS extracted from the audio/video stream. E.g., PTS in a PES Packet
//	Platform PTS	: This is a time stamp in the time base being used by the playback chain inside the HDR. E.g., DShow Stream Time.
//					  It could be the same as the Encoded PTS - it depends on the platform. The Base Renderer layer assumes Encoded PTS = Platform PTS
//	Native PTS		: same as Encoded PTS
//	Presentation PTS: same as Platform PTS
//	Sprite Time		: This in the time base being used to render sprites. (See: BaseRenderer::Commit(), where this is system time)
//

// PTSConversionType: indicates a type of PTS conversion. (for usage, see ConvertPTS())
typedef enum
{
	kEncodedToPlatform,				// convert from Encoded PTS to Platform PTS
	kPlatformToEncoded				// convert from Platform PTS to Encoded PTS
} PTSConversionType;

// PTSType: type of PTS or time.  [for usage, see GetPTS()]
typedef enum
{
	kCurrentEncodedPTS,				// Encoded PTS of the current playback location
	kCurrentPlatformPTS,			// Platform PTS of the current playback location
	kLastVideoFrameEncodedPTS,		// Encoded PTS of the last video frame displayed
	kLastVideoFramePlatformPTS,		// Platform PTS of the last video frame displayed
	kCurrentSpriteTime				// Current sprite renderering time.
} PTSType;

// PTSCapability: PTS Capability flags. [for usage, see GetPTSCapability()]
struct PTSCapability
{
	bool bUsesEncodedPTS;						// Indicates if HDR uses Encoded PTS to sync video & display frames. true= uses Encoded PTS, false= uses Platform PTS.
	bool bCanConvertEncodedPTStoPlatformPTS;	// Indicates if HDR can convert Encoded PTS to Platform PTS.
	bool bCanConvertPlatformPTStoEncodedPTS;	// Indicates if HDR can convert Platform PTS to Encoded PTS.
	bool bCanProvideCurrentEncodedPTS;			// Indicates if GetPTS(kCurrentEncodedPTS, ...) calls are supported.
	bool bCanProvideCurrentPlatformPTS;			// Indicates if GetPTS(kCurrentPlatformPTS, ...) calls are supported.
	bool bCanProvideLastVideoFrameEncodedPTS;	// Indicates if GetPTS(kLastVideoFrameEncodedPTS, ...) calls are supported.
	bool bCanProvideLastVideoFramePlatformPTS;	// Indicates if GetPTS(kLastVideoFramePlatformPTS, ...) calls are supported.
	bool bCanProvideCurrentSpriteTime;			// Indicates if GetPTS(kCurrentSpriteTime, ...) calls are supported.
};

// PTSCallback: Interface to be implemented by HDR clients to register to receive PTS Callbacks
class PTSCallback
{
    public:
    
    virtual ~PTSCallback() {}
    virtual void NewPTS(TimeValue const & platformPts, TimeValue const & encodedPts, bool bInterpolated) = 0;
};

//-----------------------------------------------------------------------


//-----------------------------------------------------------------------
// MediaEvent related enums, structs
//

// Media Event Type.
/*
typedef enum tagEMediaEventType
{
	eMediaEvent_Stop,				// Playback Stopped
	eMediaEvent_Play,				// Playback has started (either from Stop or from Pause)
	eMediaEvent_Pause,				// Playback Paused
	eMediaEvent_SourceInfo,			// Info about media source, like if it is encrypted or not...
	eMediaEvent_NewSegment,			// New Segment (discontinious) about to start
	eMediaEvent_PlaybackRate		// Change in playback rate.
} EMediaEventType;*/

struct MediaSourceInfo
{
	bool	mEncrypted;			// Indicates if source (disc/stream) is Encrypted/Protected.

	MediaSourceInfo() : mEncrypted(false)	{ }
};
//-----------------------------------------------------------------------

/*
class RENDERER_EXPORT BaseRendererConfigData
{
public:
	UInt32 mWidth;
	UInt32 mHeight;
	TimeValue mFrameTime;
	SInt64 mImageRefCacheSize;
	SonicSurface::PiRGBColor mBackgroundColor;
	bool mRequiresIndexedSurfaceSupport;
	bool mManualVideoDraw; // Allows users of BaseRenderer to call DrawVideo()
	EPlaybackMode mPlaybackMode;

	std::vector< ref_ptr<STREAM_DESC> > mDVDOutputs;

	BaseRendererConfigData() :
	    mWidth(0),
	    mHeight(0),
	    mFrameTime(0),
	    mImageRefCacheSize(0),
	    mBackgroundColor(),
	    mRequiresIndexedSurfaceSupport(true),
	    mPlaybackMode(eDVDPlaybackMode)
	    { }
	virtual ~BaseRendererConfigData() {};
};
*/
// Main Renderer abstract base class
#ifdef REALTEK_MODIFY_DISABLE_SONICTHREAD
class RENDERER_EXPORT BaseRenderer
#else
class RENDERER_EXPORT BaseRenderer : public SonicThread
#endif	
{
protected:
	/*struct PaletteNode
	{
	public:
		PaletteNode() : mLogicalPalette(0), mCommittedPalette(0), mDirty(false) { }

		ref_ptr<SonicSurface::ColorPalette> mLogicalPalette;
		ref_ptr<SonicSurface::ColorPalette> mCommittedPalette;
		bool mDirty;
	};
	typedef std::vector<PaletteNode> PaletteArray;*/

	// Scheduled Operation Queue
	/*typedef enum 
	{
		OpType_SetVideoDstRect,
		OpType_SetVideoFullScreen,
		OpType_SetLumaKey,
		OpType_SegmentBreak
	} OpType;

	class ScheduledOp
	{
	public:
		ScheduledOp(OpType type, UInt32 id, TimeValue tvScheduledTime)
			: mType(type)
			, mId(id)
			, mtvScheduledTime(tvScheduledTime)
		{};
		virtual ~ScheduledOp(){};

		OpType mType;
		UInt32 mId;
		TimeValue mtvScheduledTime;
	};

	class ScheduledOp_SetLumaKey : public ScheduledOp
	{
	public:
		ScheduledOp_SetLumaKey(SInt32 luma, TimeValue tvScheduledTime)
			: ScheduledOp(OpType_SetLumaKey, 2, tvScheduledTime)
			, mLuma(luma)
		{};
		virtual ~ScheduledOp_SetLumaKey() {};

		SInt32 mLuma;
	};

	class ScheduledOp_SetVideoDstRect : public ScheduledOp
	{
	public:
		ScheduledOp_SetVideoDstRect(UInt32 id, const PiRect& dstRect, TimeValue tvScheduledTime)
			: ScheduledOp(OpType_SetVideoDstRect, id, tvScheduledTime)
			, mDstRect(dstRect)
		{};
		virtual ~ScheduledOp_SetVideoDstRect() {};

		PiRect mDstRect;
	};

	class ScheduledOp_SetVideoFullScreen : public ScheduledOp
	{
	public:
		ScheduledOp_SetVideoFullScreen(UInt32 id, bool flag, TimeValue tvScheduledTime)
			: ScheduledOp(OpType_SetVideoFullScreen, id, tvScheduledTime)
			, mbFlag(flag)
		{};
		virtual ~ScheduledOp_SetVideoFullScreen() {};

		bool mbFlag;
	};*/

public:

	// Startup and Shutdown
    BaseRenderer();
    virtual ~BaseRenderer();

	// NOTE: It is up to derived Renderer class to initialize the Renderer:
	//		including setting mDisplaySize, image ref cache, frame rate, background color, and render thread
//	virtual void Startup(const ref_ptr<BaseRendererConfigData>& pConfigData);
    //virtual void Shutdown();
	//virtual void SwitchResolution(int width, int height);
	//virtual void SetRendererConfigData(const ref_ptr<BaseRendererConfigData>& pConfigData);
	//virtual ref_ptr<BaseRendererConfigData> GetRendererConfigData();

	// Clock functions
	virtual ref_ptr<BasePrecisionClock> GetSystemClock() const;

	// Time/PTS functions
	//
	// ConvertPTS(): Converts between Encoded PTS value (as found in the stream) and a Platform PTS (e.g., DShow time). First param indicates direction of conversion.
	// GetPTS(): Retieves current PTS either as Encoded PTS or as Platform PTS. First param indicates which time is desired.
	// GetPTSCapability(): returns flags to indicate what PTS operations are supported.
	// GetPlatformPTSClockFrequency(): returns ticks (per second) of the platform PTS clock.
	//
	virtual bool ConvertPTS(PTSConversionType conversionType, const TimeValue inPts, TimeValue & outPts);
	virtual bool GetPTS(PTSType ptsType, TimeValue & pts) = 0;
	//virtual void GetPTSCapability(PTSCapability & ptsCap);
	virtual bool GetPlatformPTSClockFrequency(TimeValue & freq) = 0;

    // PTS callback function
    virtual void SetPTSCallback(PTSCallback * p, UInt32 framesPerCall) = 0;

	// Frame rate functions
	//virtual void SetFrameTime(TimeValue frameTime);
	//virtual UInt32 GetFrameRate();
	//virtual UInt32 GetFrameRateAvg();

	// Sprite functions
	virtual void AddSprite(const ref_ptr<BaseSprite>& pSprite);
	//virtual void AddStencil(const ref_ptr<BaseStencilRect>& pStencil);
	//virtual void AddClipRect(const ref_ptr<BaseClipRect>& pClipRect);
	virtual void Prepare();
	virtual void Commit();
	//virtual void SetDirtyRect(const PiRect& inDirty);

	// DVD playback functions
	//virtual bool IsDVDPlaybackMode();
	//virtual SInt32 SetDVDPlaybackMode();
	virtual void AddStream(UInt32 srcStreamIndex, PARSER_TYPE parserType, UInt32 allocBufSize, UInt32 allocBufCount, ref_ptr<rt_stream> pSrcStream) = 0;
	virtual void RemoveStream(UInt32 srcStreamIndex) = 0;
	virtual void SetActiveStream(UInt32 srcStreamIndex, UInt32 outputID, UInt16 streamID, UInt16 substreamID) = 0;
	virtual void ConfigureOutput(UInt32 outputID, STREAM_DESC *pStreamDesc) = 0;
	virtual void FlushOutput(UInt32 outputID) = 0;

	// Video Simulation playback functions (arbitrary asset playback)
//	virtual bool IsSimulationPlaybackMode();
	//virtual void SetSimulationPlaybackMode();
	//virtual void AddVideo(UInt32 id, ref_ptr<rt_stream> pSrcStream, ref_ptr<rt_stream> pAudioStream, PrecisionClockID titleClockID, TimeValue titleTime);
	//virtual void DeleteVideo( UInt32 id );

	// Audio playback functions (arbitrary asset playback)
//	virtual void AddAudio(UInt32 id, ref_ptr<rt_stream> pAudioStream, PrecisionClockID titleClockID, TimeValue titleTime);
//	virtual void DeleteAudio( UInt32 id );

	//virtual void Play(UInt32 id) {assert(0);}	
	virtual void Play(){}
	virtual bool IsPlaying(UInt32 id){ return true;}
	//virtual void Pause(UInt32 id){assert(0);}
	virtual void Pause(){}
	//virtual void Stop(UInt32 id){assert(0);}
	virtual void Stop(){}
	//virtual void GetVideoSourceTime(UInt32 id, TimeValue& currentTime, TimeValue& duration);
	//virtual void Seek(UInt32 id, TimeValue frame);

	// MediaEvent is an API for HDR clients to tell the HDR about events related to media on disc (like if it is protected or not) or
	// playback of media (like Play Started).
	//		mediaEventType				pMediaEventData value
	//		----------------------		-----------------------------------
	//		eMediaEvent_Play			-null-
	//		eMediaEvent_Pause			-null-
	//		eMediaEvent_Stop			-null-
	//		eMediaEvent_SourceInfo		ptr to a MediaSourceInfo structure.
	//		eMediaEvent_NewSegment		-null-
	//		eMediaEvent_PlaybackRate	ptr to a double
	// 
	virtual bool MediaEvent(EMediaEventType mediaEventType, const UInt32 mediaEventDataSize, void * pMediaEventData);

	// General Video functions
	virtual void SetVideoFullScreen(UInt32 id, bool flag);
    //virtual bool GetVideoFullScreen(UInt32 id);

    // Setting a video sprite to hidden means it is active, but won't be shown until
    // its first video frame is passed into the HDR from the decoder.
    virtual void SetVideoHidden(UInt32 id, bool flag);
    //virtual bool GetVideoHidden(UInt32 id);
    
	virtual bool HasVideoSource(UInt32 id);
	virtual bool IsVideoVisible(UInt32 id);
	//virtual UInt32 GetCurrentVideoCount();	//TODO: clarify whether the count is decremented during DeleteGraphSprite() or at the next Commit.

	//virtual PiSize GetVideoSize(UInt32 id);
	//virtual PiSize GetVideoNativeAspectRatio(UInt32 id);
	virtual PiRect GetVideoDstRect(UInt32 id);
	virtual void SetVideoDstRect(UInt32 id, const PiRect& dstRect);
	//virtual PiRect GetVideoDstDrawRect(UInt32 id);

//	virtual bool SupportsVideoSrcRect() = 0;
//	virtual PiRect GetVideoSrcRect(UInt32 id);
	//virtual void SetVideoSrcRect(UInt32 id, ref_ptr<PiRect>& pSrcRect);

	//virtual UInt8 GetVideoOpacity(UInt32 id);
	//virtual void SetVideoOpacity(UInt32 id, UInt8 opacity);

	//virtual SonicSurface::SurfaceAspectRatio GetVideoAspectRatio(UInt32 id);
	//virtual void SetVideoAspectRatio(UInt32 id, SonicSurface::SurfaceAspectRatio aspectRatio);

	//virtual void EnableSPU(UInt32 id, bool flag);

	//virtual UInt32 GetVideoCanvas(UInt32 id);
	//virtual void SetVideoCanvas(UInt32 id, UInt32 canvas);

	virtual void RegisterPacketCallback(UInt32 srcStreamIndex, BasePacketCallback * pCallback) = 0;
	virtual void UnregisterPacketCallback(UInt32 srcStreamIndex, BasePacketCallback * pCallback) = 0;

	//virtual ref_ptr<SonicSurface::SysSurface> GetCurrentVideoFrame(UInt32 id) = 0;
	
    //virtual void ChangeLayout(UInt32 id, SInt32 x, SInt32 y, UInt32 numerator, UInt32 denominator, UInt32 cropX, UInt32 cropY, UInt32 cropWidth, UInt32 cropHeight) = 0;

	// Miscellaneous
	//virtual SonicSurface::PiRGBColor GetBackgroundColor();
	//virtual void SetBackgroundColor(const SonicSurface::PiRGBColor& newColor);

	//virtual PiSize GetDisplaySize() const;

	virtual bool SupportsIndexedSurfaces() const = 0;
	//virtual ref_ptr<SonicSurface::ColorPalette> GetPalette(UInt32 index) const;
	virtual void SetPalette(UInt32 index, const ref_ptr<SonicSurface::ColorPalette>& palette) = 0;

/*	enum EPositionCaps
	{
		eFullPositioning = 0,
		eFullPositioningIfEntireVideoOnScreen,
		eFullPositioningEvenLines,
		eFullPositioningEvenLinesIfEntireVideoOnScreen,
		eOtherPositioningCaps,
	};*/
//	virtual EPositionCaps GetPositioningCapability() = 0;
	//virtual bool SupportsArbitraryHorizontalScaling(UInt32& minScale, UInt32& maxScale) = 0;
	//virtual std::vector<UInt32> GetHorizontalScalingFactors() = 0;
	//virtual bool SupportsArbitraryVerticalScaling(UInt32& minScale, UInt32& maxScale) = 0;
	//virtual std::vector<UInt32> GetVerticalScalingFactors() = 0;

	//virtual UInt32 CreateCanvas();
	//virtual PiSize GetCanvasVirtualCoords(UInt32 canvas);
	//virtual PiRect GetCanvasDisplayRect(UInt32 canvas);
	//virtual void GetCanvasTransforms(UInt32 canvas, float& outScaling, float& outOffsetX, float& outOffsetY);
	//virtual void SetCanvasVirtualCoords(UInt32 canvas, const PiSize& virtualCoords);
	//virtual void SetCanvasDisplayRect(UInt32 canvas, const PiRect& displayRect);

	// Cursor functions
	//virtual void SetCursorEnabled(bool flag);
	//virtual bool IsCursorEnabled() const;
	//virtual void SetCursorVisible(bool flag);
	//virtual bool IsCursorVisible() const;
	//virtual void SetCursorClipRect(const PiRect& clipRect);
	//virtual PiRect GetCursorClipRect() const;
	//virtual void ClearCursorClipRect();
	//virtual void SetCursorPosition(const PiPoint& pos);
	//virtual PiPoint GetCursorPosition() const;
	//virtual void SetCursor(ref_ptr<BaseImageRef>& pImageRef, const PiPoint& hotspot);
	//virtual ref_ptr<BaseImageRef> GetCursorImage() const;
	//virtual PiPoint GetCursorHotspot() const;
	//virtual void SetDefaultCursor(ref_ptr<BaseImageRef>& pImageRef, const PiPoint& hotspot);

	//virtual bool HasCursor() const;

	// Background Image support
	//virtual HDRSurface_BaseSurface* InitializeBackgroundSurface() = 0;
	//virtual void RenderBackgroundSurface(HDRSurface_BaseSurface* surface) = 0;
	//virtual void GetBackgroundSurfaceResolution(UInt32 &width, UInt32 &height);
	//virtual void ClearBackgroundSurface();
	//virtual void DeleteBackgroundSurface();
	//virtual void SetBackgroundCanvas(UInt32 canvas);

    // Luma control of PIP.
	// luma: upper limit for transparency (0 - luma is made transparent). Max valid value is 0xFF.
	//       Pass -1 to turn Off luma keying.
    virtual void SetLumaKey(SInt32 luma);
    
	// Surface functions
	//virtual void GetPrimarySurface(ref_ptr<SonicSurface::SysHDSurface>& pSurface);
	virtual void CreateSurface(UInt32 width, UInt32 height, SonicSurface::SurfaceFormat format, ref_ptr<SonicSurface::SysHDSurface>& pSurface) = 0;
	//mpc virtual void CreateTexture(UInt32 width, UInt32 height, SonicSurface::SurfaceFormat format, ref_ptr<SonicSurface::SysHDTexture>& pSurface) = 0; //mpc
	//virtual void CreateSurfaceFromFileInMemory(const void *pFileBuffer, size_t bufferSize, ref_ptr<SonicSurface::SysHDSurface>& pSurface) = 0;
#ifndef REALTEK_MODIFY_ENABLE_CACHE
	virtual CacheHandle CacheImage(ref_ptr<BaseImageRef>& pImageRef, ref_ptr<SonicSurface::SysHDSurface>& pSurface) = 0;
#endif	
	//virtual PiSize GetImageSourceSize(ref_ptr<BaseImageRef>& pImageRef);

	// Display frame functions
	virtual void CreateDisplayFrame(UInt32 width, UInt32 height, SonicSurface::SurfaceFormat format, BaseDisplayFrame *& pDisplayFrame) = 0;
	virtual void ClearDisplayFrame(BaseDisplayFrame *pDisplayFrame) = 0;

	// Display layer functions
	virtual bool AddDisplayLayer(DisplayLayer *pDisplayLayer, UInt32 zOrder) = 0;
	virtual bool RemoveDisplayLayer(DisplayLayer *pDisplayLayer) = 0;
	virtual bool Blt(SonicSurface::SysHDSurface *pSourceSurface,
		BaseDisplayFrame *pDisplayFrame,
		PiRect srcRect,
		PiPoint upperLeft) = 0;

	// Sound functions
	//virtual SoundID CreateSound(ref_ptr<BaseSoundParser> pSoundFile) = 0;
	virtual void PlaySound(SoundID iAudioId, unsigned long pDataPhyAddr, int chNum, UInt16 samplingFreq, UInt8 bitsPerSample, bool bBigEndian, long long durationIn90k) = 0;
	//virtual void StopSound(SoundID iAudioId) = 0;
	virtual bool IsSoundPlaying() = 0;
	virtual bool IsSoundPlaying(SoundID iAudioId) = 0;
	//virtual void CloseSound(SoundID soundId) = 0;

	//virtual UInt8 GetNumSoundChannels(SoundID /*soundId*/);
	//virtual void SetSoundVolume(UInt8 channel, UInt32 volume);
    //virtual void SetSoundVolume(UInt8 channel, UInt32 volume, SoundID soundID);
	//virtual UInt32 GetSoundVolume(UInt8 channel);
    //virtual UInt32 GetSoundVolume(UInt8 channel, SoundID soundID);
	//virtual void SetSoundHorizontalPosition(SoundID soundId, SInt32 pos);
	//virtual SInt32 GetSoundHorizontalPosition(SoundID /*soundId*/);
	//virtual void SetSoundDepthPosition(SoundID soundId, SInt32 pos);
	//virtual SInt32 GetSoundDepthPosition(SoundID /*soundId*/);
	//virtual void SetSoundLoopCount(SInt32 loopCount) = 0;
	//virtual void MuteSound(bool flag);
    //virtual void MuteSound(bool flag, SoundID soundID);
	//virtual bool IsSoundMuted();
    //virtual bool IsSoundMuted(SoundID soundID);

	//virtual bool  GetBDJGainChangeEventMute(UInt32 /*eventParameter*/);
	//virtual float GetBDJGainChangeEventLevel(UInt32 /*eventParameter*/);
	//virtual float GetBDJPanningChangeEventBalance(UInt32 /*eventParameter*/);
	//virtual float GetBDJPanningChangeEventFading(UInt32 /*eventParameter*/);

	// Audio level/balance/depth scaling methods [to/from specific platform]. No scaling by default.
	//virtual SInt32 SetScaledAudioLevel(SInt32 audioLevel);
	//virtual SInt32 GetScaledAudioLevel(SInt32 audioLevel);
	//virtual SInt32 SetScaledAudioDepthPosition(SInt32 audioDP);
	//virtual SInt32 GetScaledAudioDepthPosition(SInt32 audioDP);
	//virtual SInt32 SetScaledAudioHorizontalPosition(SInt32 audioHP);
	//virtual SInt32 GetScaledAudioHorizontalPosition(SInt32 audioHP);

	//virtual float SetScaledAudioLevel(float audioLevel);
	//virtual float GetScaledAudioLevel(float audioLevel);
	//virtual float SetScaledAudioDepthPosition(float audioDP);
	//virtual float GetScaledAudioDepthPosition(float audioDP);
	//virtual float SetScaledAudioHorizontalPosition(float audioHP);
	//virtual float GetScaledAudioHorizontalPosition(float audioHP);

	/*virtual void GetSoundMix(
		UInt8 &LtoL, UInt8 &LtoR, UInt8 &LtoC, UInt8 &LtoLs, UInt8 &LtoRs, UInt8 &LtoLb, UInt8 &LtoRb, UInt8 &LtoLfe,
		UInt8 &RtoL, UInt8 &RtoR, UInt8 &RtoC, UInt8 &RtoLs, UInt8 &RtoRs, UInt8 &RtoLb, UInt8 &RtoRb, UInt8 &RtoLfe) = 0;
	virtual void SetSoundMix(
		UInt8 LtoL, UInt8 LtoR, UInt8 LtoC, UInt8 LtoLs, UInt8 LtoRs, UInt8 LtoLb, UInt8 LtoRb, UInt8 LtoLfe,
		UInt8 RtoL, UInt8 RtoR, UInt8 RtoC, UInt8 RtoLs, UInt8 RtoRs, UInt8 RtoLb, UInt8 RtoRb, UInt8 RtoLfe) = 0;

	virtual UInt8 GetNumMasterAudioChannels();
	virtual void SetMasterAudioVolume(UInt8 channel, UInt32 volume);
	virtual UInt32 GetMasterAudioVolume(UInt8 channel);
	virtual void SetMasterAudioHorizontalPosition(SInt32 pos);
	virtual SInt32 GetMasterAudioHorizontalPosition();
	virtual void SetMasterAudioDepthPosition(SInt32 pos);
	virtual SInt32 GetMasterAudioDepthPosition();
	virtual void MuteMasterAudio(bool flag);
	virtual bool IsMasterAudioMuted();
*/
	//virtual UInt8 GetNumAudioChannels(UInt32 id)                            {/*id;*/ return 0;}
	//virtual void SetAudioVolume(UInt32 id, UInt32 channel, UInt32 volume)   {/*id; channel; volume;*/}
	//virtual UInt32 GetAudioVolume(UInt32 id, UInt32 channel)                {/*id; channel; */return 0;}
	//virtual void SetAudioHorizontalPosition(UInt32 id, SInt32 pos)          {/*id; pos;*/}
	//virtual SInt32 GetAudioHorizontalPosition(UInt32 id)                    {/*id;*/ return 0;}
	//virtual void SetAudioDepthPosition(UInt32 id, SInt32 pos)               {/*id; pos;*/}
	//virtual SInt32 GetAudioDepthPosition(UInt32 id)                         {/*id;*/ return 0;}
	//virtual void MuteAudio(UInt32 id, SoundID soundId, bool flag)           {/*id; soundId; flag;*/}
	// bool IsAudioMuted(UInt32 id, SoundID soundId)                   {/*id; soundId;*/ return true;}

	//virtual bool IsMetaDataProcessingOn();
	//virtual void SetMetaDataProcessingOn(bool on);

	virtual void ResetAudioMixer(bool bMixingMetadataOnly);

	/*virtual void GetAudioMix(UInt32 id,
		UInt8 &LtoL, UInt8 &LtoR, UInt8 &LtoC, UInt8 &LtoLs, UInt8 &LtoRs, UInt8 &LtoLb, UInt8 &LtoRb, UInt8 &LtoLfe,
		UInt8 &RtoL, UInt8 &RtoR, UInt8 &RtoC, UInt8 &RtoLs, UInt8 &RtoRs, UInt8 &RtoLb, UInt8 &RtoRb, UInt8 &RtoLfe) = 0;
	virtual void SetAudioMix(UInt32 id,
		UInt8 LtoL, UInt8 LtoR, UInt8 LtoC, UInt8 LtoLs, UInt8 LtoRs, UInt8 LtoLb, UInt8 LtoRb, UInt8 LtoLfe,
		UInt8 RtoL, UInt8 RtoR, UInt8 RtoC, UInt8 RtoLs, UInt8 RtoRs, UInt8 RtoLb, UInt8 RtoRb, UInt8 RtoLfe) = 0;

	virtual UInt32 GetPlayerVolume();
	virtual void SetPlayerVolume(UInt32 volume);
	virtual bool IsPlayerMuted();
	virtual void MutePlayer(bool flag);

	// Capability functions
	virtual bool SupportsHDSecondaryVideo() const = 0;

	virtual bool SupportsAnalogAudioOutput() const = 0;
	virtual UInt32 NumberOfAnalogAudioChannels() const = 0;

	virtual bool SupportsDigitalAudioSPDIFPCMOutput() const = 0;
	virtual bool SupportsDigitalAudioSPDIFDolbyDigitalOutput() const = 0;
	virtual bool SupportsDigitalAudioSPDIFDTSOutput() const = 0;
	virtual UInt32 NumberOfDigitalAudioSPDIFChannels() const = 0;

	virtual bool SupportsDigitalAudioHDMIPCMOutput() const = 0;
	virtual UInt32 NumberOfDigitalAudioHDMIChannels() const = 0;
	virtual bool IsDigitalAudioHDMIConnected() const = 0;

	virtual UInt32 SupportsAudioDecodeLPCM() const = 0;			// returns number of channels supported by each audio decoder
	virtual UInt32 SupportsAudioDecodeDDPlus() const = 0;
	virtual UInt32 SupportsAudioDecodeMPEG() const = 0;
	virtual UInt32 SupportsAudioDecodeDTSHD() const = 0;
	virtual UInt32 SupportsAudioDecodeMLP() const = 0;
	virtual UInt32 SupportsAudioDecodeAACv2() const = 0;
	virtual UInt32 SupportsAudioDecodeMP3() const = 0;
	virtual UInt32 SupportsAudioDecodeWMAPro() const = 0;

	// Functions to get and release services that HDR can provide
	virtual bool GetHDRInterface(HDRInterfaceID interfaceID, void ** ppInterface);
	virtual bool ReleaseHDRInterface(HDRInterfaceID interfaceID, void * pInterface);

	// Registering/UnRegistering Error Callbacks
	virtual bool RegisterHDRErrorCallback(HDRErrorCallback * errCallback);
	virtual bool UnregisterHDRErrorCallback(HDRErrorCallback * errCallback);
*/
	// Time Scheduled Operations
	virtual bool IsScheduledOperationPending();
	virtual void ClearScheduledOperations();
	virtual void ScheduleSetLumaKey(SInt32 luma, TimeValue tvScheduledTime);
	virtual void ScheduleSetVideoDstRect(UInt32 id, const PiRect& dstRect, TimeValue tvScheduledTime);
	virtual void ScheduleSetVideoFullScreen(UInt32 id, bool flag, TimeValue tvScheduledTime);
	virtual void ScheduleSegmentBreak(TimeValue tvScheduledTime);

	virtual void SetEndOfPlaybackCallback(unsigned long instanceID, UInt32 outputID, UInt32 callbackData) = 0;

	virtual void ClearPrimarySurface() = 0;
//protected:
	//virtual SonicError Run(void); // thread entry point


    // private methods
protected:

	enum ERenderThreadState
	{
		eNotStarted = 0,
		eRunning,
		eWaitingToStop,
		eFinished,
	};

	// playback state of playback chain in HDR. Value set when MediaEvents are send by HDR client
	enum EMediaPlaybackState
	{
		eMediaStopped,
		eMediaPlaying,
		eMediaPaused
	};

	//virtual void CreateSystemClock() = 0;

	//virtual void DeleteAllGraphSprites();
	//virtual void DeleteGraphSprite(BaseGraphSprite *);

	//virtual bool IsImageRefCacheInitialized();
#ifndef REALTEK_MODIFY_ENABLE_CACHE
	virtual void CreateImageRefCache(SInt64 maxMemorySize = kUnlimitedCacheSize);
	virtual void DestroyImageRefCache();
#endif
	//virtual void StartRenderingThread();			
	//virtual void StopRenderingThread();		

	//virtual void CommitNow();
	//virtual void LoadImageSprite(BaseImageSprite *pSprite);
	//virtual void LoadTextSprite(BaseTextSprite *pSprite);
	//virtual void UpdateVideoSprite(BaseVideoSprite *);
	//virtual void CommitPalette(UInt32 paletteIndex, PaletteNode& paletteNode);
	//virtual ref_ptr<SonicSurface::SysHDSurface> ConvertIndexedSurface(ref_ptr<SonicSurface::SysSurface> pSurface) = 0;
#ifdef REALTEK_MODIFY
	//virtual BaseGraphSprite* GetGraphSprite(UInt32 id, char const * pError = 0);
	//virtual BaseVideoSprite* GetVideoSprite(UInt32 id, char const * pError = 0);
	//virtual BaseAudioSprite* GetAudioSprite(UInt32 id, char const * pError = 0);
#else
	virtual BaseGraphSprite* GetGraphSprite(UInt32 id, SonicText const * pError = 0);
	virtual BaseVideoSprite* GetVideoSprite(UInt32 id, SonicText const * pError = 0);
	virtual BaseAudioSprite* GetAudioSprite(UInt32 id, SonicText const * pError = 0);
#endif
	//virtual BaseVideoSprite* CreateVideoSprite(UInt32 id) = 0;
	//virtual BaseAudioSprite* CreateAudioSprite(UInt32); // tolerate no implementation

	//virtual void Attach(BaseVideoSprite *);
	//virtual void Detach(BaseVideoSprite *);

	//virtual PiFPoint GetCanvasScaler(UInt32 canvas);
	//virtual bool CanvasNeedsUpdate(UInt32 canvas);
	//virtual PiRect ConvertCanvasRect(UInt32 canvas, const PiRect& canvasRect);

	///virtual void CreateCursor();
	//virtual void DestroyCursor();
	//virtual void UpdateCursor();
	//virtual void DrawCursor();
/*
	virtual void DrawFilledRect(const PiRect& fillRect, const SonicSurface::PiRGBColor& color) = 0;
	virtual void DrawSpriteBorders(BaseSprite *pSprite) = 0;
	virtual void DrawImageSprite(BaseSprite *pSprite) = 0;
	virtual void DrawTextSprite(BaseSprite *pSprite) = 0;
	virtual void DrawVideoSprite(BaseSprite *pSprite) = 0;
	virtual void DrawSprite(BaseSprite *pSprite);
	virtual void BeginDrawingSprites();
	virtual void FinishDrawingSprites();
	virtual bool DrawSprites();
	virtual bool DrawBackground();
	virtual void BeginScene() = 0;
	virtual void EndScene() = 0;
	virtual void DrawFrame();
	virtual void DrawFrameCheckTime(UInt8 checkTime, TimeValue tvPlatformPTS = -1);

	virtual void DrawStencilRect(BaseStencilRect * pStencil) = 0;
	virtual void ClearStencilRect(BaseStencilRect * pStencil) = 0;

	virtual void SetClipRect(BaseClipRect * pClipRect) = 0;
	virtual void ClearClipRect(BaseClipRect * pClipRect) = 0;

	virtual void UpdateRenderStates(UInt32 zDepth);

	virtual void HandleSoundUpdate() = 0;

	BaseRenderer& operator=(const BaseRenderer& r);
	BaseRenderer(BaseRenderer& r);

	// deprecated - use GraphSprite* methods instead
	virtual void DeleteAllVideo();
	virtual void DeleteVideo(BaseVideoSprite *);
	virtual BaseVideoSprite* GetSourceByID(UInt32 id);

	void ReportHDRError(HDRErrorType errType, HDRErrorInfo * errData);

	// Scheduled Ops
	virtual void InsertScheduledOp(ScheduledOp* pOp);
	virtual void ApplyScheduledOps(TimeValue tvTime);
	virtual void ClearScheduledOperationsByOpType(OpType type);
*/
    // private data 
protected:

	// Render Engine data
	//ref_ptr<BaseRendererConfigData> mpConfigData;		// Startup configuration data;
	//bool						mInitialized;			// Set to true once the Renderer has been initialized
	//SonicCriticalSection		mRenderLock;			// Thread safe access to render device creation/destruction, frame rate control, background color, and DrawFrame()
	//SonicCriticalSection		mDeviceLock;			// Thread safe access to Initialization, Render Thread status, and custom Allocator/Presenter functions
	//ERenderThreadState			mRenderThreadState;		// 0: not started, 1: running, 2: requested to stop, 3: stopped

	//SonicSurface::PiRGBColor	mBackgroundColor;		// Background fill color used to clear the screen prior to drawing any sprites
	//PiSize						mDisplaySize;			// Size of renderer's back buffer

	//PaletteArray				mPalettes;				// Array of global palettes registered with the Renderer
    //bool                        mbGraphicsThread;       // false == both HandleSoundUpdate() and DrawFrame() called in BaseRenderer::Run() thread proc.
                                                        // true == only HandleSoundUpdate() is called in BaseRenderer::Run() thread proc.
                                                        //         DrawFrame() will be called by derived class, most likely in a separate thread.

	//EPlaybackMode				mPlaybackMode;

	EMediaPlaybackState			mMediaPlaybackState;	// state of playback chain inside HDR
	MediaSourceInfo				mMediaSourceInfo;		// information about media source

	/*struct Canvas
	{
		PiRect mDisplayRect; // display rect relative to main Renderer.
		PiSize mVirtualCoords; // virtual coordinate system used by associated sprites for positioning and scaling
		PiFPoint mScaler; // ratio of display rect to virtual coordinate space
		bool mNeedsUpdate; // Need to update any associated sprites next time in the commit loop

		Canvas() : mDisplayRect(), mVirtualCoords(), mScaler(1.0f, 1.0f), mNeedsUpdate(false) { }
		~Canvas() { }
	};
	std::vector< Canvas >       mCanvases;*/

	// Timing info
	ref_ptr<BasePrecisionClock>	mpSystemClock;			// high-precision clock used for timing

	TimeValue					mRenderStartTime;		// Time when rendering started
	TimeValue					mCurFrameTime;			// Current frame timer
	TimeValue					mPrevFrameTime;			// Last time we rendered the scene

	UInt32						mCurrentFrameRate;		// Actual FPS
	TimeValue					mAvgTimePerFrame;		// average time between frames
	TimeValue					mElapsedFrameTime;		// last time between frames
	TimeValue					mDesiredTimePerFrame;	// Ideal time between frames
	UInt32						mTotalFramesDrawn;		// Total number of frames drawn since renderer started (used to calculate mAvgTimePerFrame)

	// Sprite data
	SonicCriticalSection		mSpriteLock;			// Prevent multiple threads from iterating and/or modifying sprite array at the same time
	TimeValue					mSpriteTime;			// Time at which sprite changes need to be committed (in real time)
	//SpriteArray					mSprites;				// Full array of video, image, and text sprites
	UInt32						mCurrentVideoCount;		// Current number of audio or video sources attached to renderer
//	StencilRectArray			mStencilRects;			// Array of stencil rects
	//ClipRectArray				mClipRects;				// Array of clip rects

	//bool						mDirtyRectSet;			// Are we just going to Commit() part of the screen?
	//PiRect						mDirtyRect;				// Only this area will be Commit()'d if it has been set.

	// Cursor data
	//bool mCursorEnabled;
	//PiPoint mCursorPosition;
	//SonicCriticalSection		mCursorLock;			// Prevent multiple threads from iterating and/or modifying cursor at the same time
	//ref_ptr<BaseImageSprite>	mpCursor;
	//ref_ptr<BaseClipRect>		mpCursorClipRect;
	//PiPoint                     mCursorHotspot;
	//ref_ptr<BaseImageRef>       mpCursorImageRef; // source cursor image reference (either compressed file buffer or user-created surface)
	//PiPoint                     mDefaultCursorHotspot;
	//ref_ptr<BaseImageRef>       mpDefaultCursorImageRef; // default cursor image reference (either compressed file buffer or user-created surface)

	// Background surface data
	//HDRSurface_BaseSurface*		mpBackgroundSurface;
	//UInt32                      mnBackgroundCanvas;
#ifndef REALTEK_MODIFY_ENABLE_CACHE
	// Image cache data
	ref_ptr<ObjectCache>		mpImageRefCache		// Image cache -- maps compressed image file buffers to decoded Direct3D textured/surfaces
#endif
	// Sound data
	SoundID						mCurSoundID;			// ID of current sound -- automatically incremented every time CreateSound() is called

	//UInt8						mNumSoundChannels;		// Number of channels for use by sound effect
	//std::vector<UInt32>			mSoundVolume;			// Current sound effect volume (for each channel)
	//PiPoint						mSoundPos;				// Current sound effect horizontal and depth placement
	//bool						mSoundMuted;			// true if sound effect is muted

	//UInt8						mNumMasterAudioChannels; // Number of channels for use by master audio volume control
	//std::vector<UInt32>			mMasterAudioVolume;		// Master audio volume (for each channel)
	//PiPoint						mMasterAudioPos;		// Master audio horizontal and depth placement
	//bool						mMasterAudioMuted;		// true if master audio control is muted

	//UInt32						mPlayerVolume;			// overall player volume control, analogous to volume control on TV, controls all audio
	//bool						mPlayerMuted;			// true if player volume is muted

	//bool						mMetaDataProcessingOn;	// true if MetaDataProcessing is on.

	//SonicCriticalSection        mInterfaceLock;		// lock to serialize GetHDRInterface and ReleaseHDRInterface calls

	// Interfaces provided by HDR
	//ref_ptr<BaseAVOutputPortCaps>	mpAVOutputPortCaps;
	//ref_ptr<BaseSurfaceOps>			mpSurfaceOps;

    //SInt32                       mLumaKey;

	// error reporting
	//SonicCriticalSection		mErrorCallbackLock;		// Thread safe access to Error Callbacks
	//HDRErrorCallbackArray		mErrorCallbacks;

	// Scheduled Ops
	//std::list<ScheduledOp*>		mScheduledOps;
	//std::list<ScheduledOp*>::iterator mLastScheduledSegmentBreak;
	//SonicCriticalSection		mcsScheduleLock;

};

#endif // BASE_RENDERER_H
