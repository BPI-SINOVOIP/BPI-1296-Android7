  

/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CBinaryFileReader.h
//
//	@brief Definition for DVD-VENUS Binary File Reader Filter Class
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_BINARY_FILE_READER_H__
#define __C_BINARY_FILE_READER_H__

#include <CStream.h>
#include <Util.h>

class CAudioInOutputPin;

class CAudioInFilter : public CBaseFilter
						//,public IAudioOutControl
{

public:

	CAudioInFilter();
	virtual ~CAudioInFilter();

	HRESULT							Run(void);
	HRESULT							Pause(FILTER_STATE State);
	HRESULT							Stop(void);

protected:

	static void 					MainProcessThread(void* pData);
    virtual bool					MainProcess(void) PURE;

protected:

	CThread*						m_pThread;
	CAudioInOutputPin*				m_pOutPin;
	CAudioInOutputPin*				m_pExtOutPin;
	CMemRingAllocator*				m_pOutputAllocator;
	CMemRingAllocator*				m_pExtOutputAllocator;

private:

};

///////////////////////////////////////////////////////////////

class CAudioInOutputPin : public  CBaseOutputPin
{
public:

	CAudioInOutputPin() {};
	virtual ~CAudioInOutputPin() {};
	HRESULT							EndOfStream(unsigned int eventID);
	
protected:

};


class CBinaryFileReader: public CAudioInFilter
{

	friend class				CBinaryFileReaderOutPin;

public:

    CBinaryFileReader(char*	pFileName);
	virtual						~CBinaryFileReader(void);

	HRESULT						Stop();

	HRESULT						OpenFile(char*	pFileName);
	HRESULT						CloseFile();

protected:
    // re-write this function for user's purpose
	bool                        MainProcess(void);

private:
    FILE*                       m_pFileHandle;
    char*                       m_pFileName;

};



class CBinaryFileReaderOutPin: public CAudioInOutputPin
{
public:
	CBinaryFileReaderOutPin() {};
	virtual ~CBinaryFileReaderOutPin() {};

};
#endif
