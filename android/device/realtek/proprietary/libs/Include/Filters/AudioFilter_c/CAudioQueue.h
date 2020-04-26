
#ifndef __C_AUDIO_QUEUE_H__
#define __C_AUDIO_QUEUE_H__

#include <CStream.h>
#include <Filters/PrivateInfo.h>
#include <OSAL.h>

/** 
**********************************************************************************
class CAudioPackQueue
**********************************************************************************
*/

typedef struct{
    Buffer_T                    EStreamBuffer;
    int64_t                     PTS;
} EStream_T;


class CAudioPackQueue : public ulCircularQueue<EStream_T> 
{
public:
	CAudioPackQueue(const unsigned int QueueSize)
		: ulCircularQueue<EStream_T>(QueueSize) {};

	virtual ~CAudioPackQueue() {};
	
	bool                        GetHeadPtr(EStream_T** LHS);

};


class CAudioPackSemQueue : public ulSemCirQueue<EStream_T> 
{
public:
	CAudioPackSemQueue(const unsigned int QueueSize)
		: ulSemCirQueue<EStream_T>(QueueSize) {};

	virtual ~CAudioPackSemQueue() {};
	
	bool						GetHeadPtr(EStream_T** LHS, long milliTimeOut);

};


////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

class CAudioRingReadSizeControl
{
public:
	CAudioRingReadSizeControl();
	virtual ~CAudioRingReadSizeControl();
	
	void AddSize(int size);
	void MinusSize(int size);
	HRESULT RequestSize(int requestSize, long MilliTimeOut);
	void Reset(void);

private:

	int				m_Size;
	int				m_RequestSize;
	bool			m_bNeedSizeNotifier;
	CSemaphore		m_SemRequestSize;
	CCritSec		m_Lock;
};


#endif

