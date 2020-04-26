#ifndef __C_DATA_OUTPUT_H__
#define __C_DATA_OUTPUT_H__

#include <OSAL.h>
#include <Util.h>
#include <IStream.h>
#include <CStream.h>
#include <FlowEvent.h>
#include <io/dvd_parse.h>
#include <io/dvd_write.h>

class CDataOutput 
{
public:
	CDataOutput(unsigned int bufferSize);
	virtual ~CDataOutput(void);

	HRESULT Receive(IMediaSample* pSample);
	HRESULT init(CBaseFilter* pFilter);
	HRESULT setTotalDataSize(unsigned int totalDataSize);
	HRESULT setFileHandle(void* pFileHandle);
	HRESULT setVOBHandle(dvd_file_t *pVOBHandle);
	HRESULT clearFileHandle();
	HRESULT setAllocator(IMemAllocator* pAllocator);
	HRESULT clearAllocator();

protected:
	virtual bool	myDataOutput(void) = 0;
	static void 	ThreadDataOutput(void* pData);

protected:
	void*			m_pFileHandle;
	dvd_file_t*     m_pVOBHandle;
	CThread*		m_pThread;
	IMemAllocator*	m_pAllocator;
	VoidArrSem		m_DataQueue;
	unsigned int	m_WriteDataSize;
	unsigned int	m_TotalDataSize;
	CBaseFilter*	m_pFilter;
};

#endif
