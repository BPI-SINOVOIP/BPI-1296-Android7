#ifndef __C_SIMPLE_NAV_FILTER_H__
#define __C_SIMPLE_NAV_FILTER_H__

#include <StreamClass/CFilter.h>
#include <StreamClass/CPin.h>
#include <StreamClass/CMemAllocator.h>
#include <StreamClass/CReferenceClock.h>

class   IPin;

class CSimpleNavFilter: public CBaseFilter{
	
	
public:
	CSimpleNavFilter();
	~CSimpleNavFilter();
	
	HRESULT				Pause(FILTER_STATE State);
	HRESULT				Run(void);
	HRESULT				Stop(void);

//	HRESULT				GetBSBuffer(BYTE** ptr, int *size);
	HRESULT				GetICQBuffer(BYTE** ptr, int *size);
	HRESULT				StartDecocde(void);
	HRESULT				StopDecode(void);
//	HRESULT				DeliverInBandCommand(BYTE* Data, long Size);
	HRESULT				DeliverFlush(void);
	HRESULT				DeliverPTS(int64_t PTS);
	HRESULT				DeliverNewSegment(void);
	HRESULT				DeliverSegmentEnd(void);
	HRESULT				DeliverAudioFormat(void);
	HRESULT				DeliverDecodeCommand(int64_t relativePTS, int64_t duration);
	HRESULT				DeliverPrivateInfo(int infoId, BYTE* pInfo, int length);
//	HRESULT				GetBSBuffer(BYTE **pLower, BYTE **pUpper, long *pnSize);
	HRESULT				RequestWriteSpace(BYTE **pWritePtr, long *pnFreeSpace, long nRequestSize);
	HRESULT				PutData(BYTE *pData, long Size);

private:
	void				xGetBSBuffer(void);
//	void				xGetICQBuffer(void);
	void				MyMemcpy(unsigned char* pSrc, unsigned char* pDes, long size);
	
protected:
	CBaseOutputPin		*m_pOutPin;
	//CBaseOutputPin		m_infoPin;
	
	IMemAllocator		*m_pBSAllocator;
    BYTE				*m_pBSBufferLower;
    BYTE				*m_pBSBufferUpper;
    long				m_nBSBufferSize;
    IReferenceClock		*m_pRefClock;
/*    
    IMemAllocator		*m_pICQAllocator;
    BYTE				*m_pICQBufferLower;
    BYTE				*m_pICQBufferUpper;
    long				m_nICQBufferSize;
*/    
    //int					cachedAddrOffset;
    
public:
    AUDIOFORMAT 		m_audioFormat;
};

#endif	//__C_SIMPLE_NAV_FILTER_H__

