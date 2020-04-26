#include "BaseRenderer.h"
#include "CEStreamDescriptor.h"
#include "IDFB.h"
#include "SonicCriticalSection.h"
//#include "SonicSemaphore.h"
#include "InputPluginOut.h"
#include "DisplayLayer.h"

#include <AudioInbandAPI.h>

class CERenderer : public BaseRenderer
{
	typedef std::vector<StreamDescriptor*>	StreamArray;
	typedef std::vector<DisplayLayer *>		DisplayLayerArray;
		

public:
	CERenderer(INPUT_HDMV_OUT inputPlugin);
	virtual ~CERenderer();
	
	bool ConvertPTS(PTSConversionType conversionType, const TimeValue inPts, TimeValue & outPts);
	bool GetPTS(PTSType ptsType, TimeValue & pts);
	bool GetPlatformPTSClockFrequency(TimeValue & freq);
	void SetPTSCallback(PTSCallback * p, UInt32 framesPerCall)
	{
		m_PTSCallbackHandler.SetPTSCallback(p);
	}
	
	void Prepare();
	void Commit();
	
	void AddStream(UInt32 srcStreamIndex, PARSER_TYPE parserType, UInt32 allocBufSize, UInt32 allocBufCount, ref_ptr<rt_stream> pSrcStream) ;
	void RemoveStream(UInt32 srcStreamIndex) ;
	void SetActiveStream(UInt32 srcStreamIndex, UInt32 outputID, UInt16 streamID, UInt16 substreamID) ;
	void ConfigureOutput(UInt32 outputID, STREAM_DESC *pStreamDesc) ;
	void FlushOutput(UInt32 outputID) ;
	
	bool MediaEvent(EMediaEventType mediaEventType, const UInt32 mediaEventDataSize, void * pMediaEventData);
	
	void RegisterPacketCallback(UInt32 srcStreamIndex, BasePacketCallback * pCallback);
	void UnregisterPacketCallback(UInt32 srcStreamIndex, BasePacketCallback * pCallback);

	bool SupportsIndexedSurfaces() const;
	void SetPalette(UInt32 index, const ref_ptr<SonicSurface::ColorPalette>& palette);
		
	
	void CreateSurface(UInt32 width, UInt32 height, SonicSurface::SurfaceFormat format, ref_ptr<SonicSurface::SysHDSurface>& pSurface);
	void CreateDisplayFrame(UInt32 width, UInt32 height, SonicSurface::SurfaceFormat format, BaseDisplayFrame *& pDisplayFrame);
	void ClearDisplayFrame(BaseDisplayFrame *pDisplayFrame);
	
	bool AddDisplayLayer(DisplayLayer *pDisplayLayer, UInt32 zOrder);
	bool RemoveDisplayLayer(DisplayLayer *pDisplayLayer);
	bool Blt(SonicSurface::SysHDSurface *pSourceSurface, BaseDisplayFrame *pDisplayFrame, PiRect srcRect, PiPoint upperLeft);
	bool DrawDisplayLayers(TimeValue curPtsValue);
	bool DrawDisplayFrame(BaseDisplayLayer* pDisplayLayer, BaseDisplayFrame* pDisplayFrame);
	
	
	void PlaySound(SoundID iAudioId, unsigned long pDataPhyAddr, int chNum, UInt16 samplingFreq, UInt8 bitsPerSample, bool bBigEndian, long long durationIn90k);
	bool IsSoundPlaying();
	bool IsSoundPlaying(SoundID iAudioId);
	
	void SetEndOfPlaybackCallback(unsigned long pbEndCallback, UInt32 outputID, UInt32 callbackData);
	void ClearPrimarySurface();
	
	//____________________________________________________
	// called by InputHDMV
	//____________________________________________________
	void UpdatePTS(long long const platformPTS, long long const encodedPTS)
	{
		m_PTSCallbackHandler.SetPTS(platformPTS, encodedPTS);
	}
	rt_stream*	GetSrcStreamByIndex(UInt32 streamIndex);
	

private:
	void DirectFBStartup();
	void DirectFBShutDown();
			
	void CreateAO();
	void DestroyAO();
			
	StreamDescriptor* GetStreamDescriptorByIndex(UInt32 streamIndex);
	StreamDescriptor* GetStreamDescriptorByOutputID(int outputID);
	SI_CODEC_TYPE ConvertTD_SUBTYPEToSI_CODEC_TYPE(TD_SUBTYPE type);
	bool ConvertTD_SUBTYPEToSI_CODEC_TYPE(TD_SUBTYPE type, SI_CODEC_TYPE* pReturnType/*, void** pPrivateData, int* pPrivateDataSize*/);
		
	void DumpSurface(FILE* fp, SonicSurface::SysHDSurface* pSysHDSurface, char* pPrintMessage);

private:
	SonicCriticalSection		m_primarySurfaceLock;
	IDFB						m_IDFB;
	IDFBDisplayLayer*           m_pIDFBDisplayLayer;
	IDFBSurface*				m_pIDFBPrimarySurface;
	bool						m_bUseTripleBuf;
	//SonicSemaphore				m_SpriteTimeEvent;
	//TimeValue					m_mapTime;	            // Native/Encoded PTS in 90KHz units
    //TimeValue                   m_mapTimeDuration;      // Duration of frame corresponding to m_mapTime
    //LPCM_SETTING				m_lpcmSetting;
    StreamArray					m_pSrcStreams;
	INPUT_HDMV_OUT				m_inputPlugin;
	
	DisplayLayerArray       	m_displayLayers;
	
	FILE*						m_pSurfaceLog;
	
	long						m_audioOutID;		//audio out agent id; -1 means not created
	SoundID						m_currentSoundID;	//button id which playing now
	long long					m_currentSoundStopTimeIn90k;
	
	//PTSCallback *				m_PTSCallback;		// may be null
	
	//__________________________________________________________________________________________________
	//
	//__________________________________________________________________________________________________

	enum {
		UNKNOWN,
		YES,
		NO
	} m_supportIndexedSurfaces;

	//__________________________________________________________________________________________________
	// data structure for playback end callback
	//__________________________________________________________________________________________________
	
	struct {
		int callbackData;
		unsigned long pbEndCallback;	// 0 means CallBack is cleared.

	} m_pbEndCBData[4]; //[0] : primary video, [1] : primary audio, [2] : secondary video, [3] : secondary audio
	
	//__________________________________________________________________________________________________
	// PTS callback
	//__________________________________________________________________________________________________

    class PTSCallbackHandler
    {
	public:
		PTSCallbackHandler();
		void SetPTSCallback(PTSCallback * pPTSCallback);
		

		void Pause(void);
		void Run(void);
		void SetRate(double dRate);

		void SetPTS(long long const platformPTS, long long const encodedPTS);
		void InterpolatePTS(void);
	
	private:
		PTSCallback*			m_pPTSCallback;		// may be null
		
		
		bool					m_bRunning;
		TimeValue				m_rate;				// stored as real rate * 100
		long long				m_nativePTS;		// platform pts when presentation pts arrived
		long long				m_lastPresentTime;	// most recently video or audio presentation pts
		
	} m_PTSCallbackHandler;
	
	
};
