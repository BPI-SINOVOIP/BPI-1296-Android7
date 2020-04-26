
#ifndef __TT_DECODE_FILTER_H__
#define __TT_DECODE_FILTER_H__


#include <OSAL.h>
#include <CStream.h>
#include <Filters/TTDecodeFilter/TTDecoderAPI.h>
#include <Filters/TTDecodeFilter/TTInputPin.h>
#include <Filters/TTDecodeFilter/TTOutputPin.h>

class TTDecodeFilter: public CBaseFilter
{
	friend class TTInputPin;
	friend class TTOutputPin;
public:
	TTInputPin* 	m_pInputPin;
	TTOutputPin* 	m_pOutputPin;
	
	TTDecodeFilter();		
	~TTDecodeFilter();
	
    HRESULT		Run(void);												
	HRESULT		Stop(void);												
	HRESULT		Pause(FILTER_STATE State);		

	HRESULT		TTSetFontPath(const unsigned char* pPath);
	HRESULT		TTShowScreen(char bEnable);
	HRESULT		TTSetSubtitleMagAndPageNum(unsigned char magazineNumber, unsigned char pageNumber);
	HRESULT		TTSetOutPutRegion(TT_DECODER_OUTPUT_REGION* pRegion);
	HRESULT		TTSetMode(ENUM_TT_DECODER_MODE mode);
	HRESULT		TTEnable(char bEnable);
	HRESULT		TTSetUserLabel(unsigned char magNum, unsigned char pageTens, unsigned char pageUnits);
	HRESULT		TTSetUserNation(unsigned int userNation);
	HRESULT		TTSetUserPageAddr();
	HRESULT		TTSetUserAction(ENUM_TT_DECODER_USER_ACTION action);
	HRESULT 	TTSetUIHandle(SURFACEHANDLE hSurface1, SURFACEHANDLE hSurface2);
	HRESULT 	TTUnSetUIHandle();
	HRESULT		TTFlush(void);

	static int64_t TTGetVideoSystemPTS();
		
private:
	ENUM_TT_DECODER_MODE 	m_mode;
	
	char				m_bEnableTT;
	
	
	TT_DECODER_OUTPUT_REGION m_OutRegion;
	unsigned char 		m_magNum;
	unsigned char 		m_pageTens;
	unsigned char		m_pageUnits;
	
	static void ThreadEntry(void* pData);
	
protected:
	long				m_id;
	static IReferenceClock*	m_pRefClock;
	
	
};

#endif
