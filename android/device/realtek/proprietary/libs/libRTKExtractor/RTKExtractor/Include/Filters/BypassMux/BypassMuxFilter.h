#ifndef BYPASS_MUX_FILTER_H
#define BYPASS_MUX_FILTER_H

#include <OSAL.h>
#include <CStream.h>
#include <Application/AppClass/VideoConferenceGetData.h>

#define BYPASS_MUXER_VIDEO_PIN_INDEX	VIDEO_CONFERENCE_DATA_TYPE_VIDEO
#define BYPASS_MUXER_AUDIO_PIN_INDEX	VIDEO_CONFERENCE_DATA_TYPE_AUDIO
#define BYPASS_MUXER_MAX_PIN_NUM		VIDEO_CONFERENCE_DATA_TYPE_BOTH
	
class BypassMuxInPin : public CBaseInputPin
{
	friend class BypassMuxFilter;
public:
	BypassMuxInPin(long streamBufSize);
	~BypassMuxInPin();
	HRESULT ReceiveConnection(IPin* pConnector, const MEDIA_TYPE* pmt);
	HRESULT Receive(BYTE* pWritePointer, long WriteBufferSize);
	HRESULT PrivateInfo(BYTE* pInfo, int length);
		
	HRESULT GetData(BYTE** pData, long* pSize ,long long* pPTS);
	HRESULT ReleaseData(BYTE* pReadPtr, long readSize);
			
protected:
	struct {
		unsigned long bufBase;	//ring buffer base address
		unsigned long bufLimit;	//ring buffer end address
		long		  bufSize;  //ring buffer size in byte
		long		  pinID;	//audio pin or video pin
		bool		  bConnectedToVI;	//If it is audio pin, this has no meaning.
									//If it is video pin, it means upstream filter is video in or not.
		BYTE*		pCurrReadPtr;
		long		readSize;
		
		BYTE*		pReserved;		//It is used in the case of ring buf wrap-around.
									//pointer to the rest data
		long		reservedSize;
		long long	pts;
		
	} m_ringBufInfo;
	
	struct {
		BYTE*	bufBase;
		BYTE* 	bufLimit;
		long	bufSize;
		
	} m_cmdBufInfo;
	
	FILE*			m_pVideoOutFile;	
	FILE*			m_pAudioOutFile;	
	
	CMemRingAllocator* m_pCMDAllocator;
private:
    BYTE* m_pCachedAddr;

};

class BypassMuxFilter: public CBaseFilter
{
		
public:
	BypassMuxFilter();
	~BypassMuxFilter();
	
	HRESULT Run(void);
	HRESULT Stop(void);
	HRESULT Pause(FILTER_STATE State);
	
	
	HRESULT GetData(BYTE** pData, long* pSize, long long* pPTS, long pinID);
	HRESULT ReleaseData(long pinID, BYTE* pReadPtr, long readSize);
	bool 	HasReservedData(long pinID);
	
private:
	BypassMuxInPin* m_pInPin[BYPASS_MUXER_MAX_PIN_NUM];
		
};


#endif
