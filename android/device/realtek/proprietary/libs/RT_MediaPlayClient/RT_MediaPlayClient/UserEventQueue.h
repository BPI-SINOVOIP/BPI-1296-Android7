#ifndef USE_EVENT_QUEUE_H
#define USE_EVENT_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <RT_MediaPlayClient/common.h>

class EventQueue
{
	typedef struct {
		int eventId;
		int eventData;
		int eventDataSize;
	} QueueElem;
	
public: 
	EventQueue(int size = 32)
	: m_size(size)
	, m_head(0)
	, m_tail(0)
	{
		m_pQueue = (QueueElem*) calloc(sizeof(QueueElem), size);
		MUTEX_INIT(&m_lock);
	}
	~EventQueue() 
	{
		if(m_pQueue)
		{
			free(m_pQueue);
			m_pQueue = 0;
		}
		MUTEX_DESTROY(&m_lock);
	}
	
	inline bool EnQueue(int eventId, int eventData = -1, int eventDataSize = 0)
	{
		MUTEX_LOCK(&m_lock);
		
		int usedSize = (m_tail - m_head);
		if(usedSize < 0)	usedSize += m_size;
		
		int freeSize = m_size - usedSize - 1;
		
		bool ret = false;
		if(freeSize > 0)
		{
			m_pQueue[m_tail].eventId = eventId;
			m_pQueue[m_tail].eventData = eventData;
			m_pQueue[m_tail].eventDataSize = eventDataSize;
			m_tail = (m_tail < m_size - 1) ? m_tail + 1 : 0;
			ret = true;
		}
		else
		{
			printf("[%s %d] event queue full\n", __FILE__, __LINE__);
		}
		
		MUTEX_UNLOCK(&m_lock);
		return ret;
	}
	
	inline bool DeQueue(int& eventId, int* pEventDataSize = 0, char* pEventData = 0, int inDataSize = 0)
	{
		#define __MIN__(a, b) ((a) <= (b) ? (a) : (b))
			
		MUTEX_LOCK(&m_lock);
		
		bool ret;
		if(m_head == m_tail) 
			ret = false;
		else
		{
			eventId = m_pQueue[m_head].eventId;
			if(pEventDataSize) *pEventDataSize = m_pQueue[m_head].eventDataSize;
			if(pEventData)	memcpy(pEventData, &m_pQueue[m_head].eventData, __MIN__(inDataSize, m_pQueue[m_head].eventDataSize));
				
			m_head = (m_head < m_size - 1) ? m_head + 1 : 0;
			
			ret = true;
		}
		
		MUTEX_UNLOCK(&m_lock);
		return ret;
		
		#undef __MIN__
	}
	
	inline void FlushQueue()
	{
		MUTEX_LOCK(&m_lock);
		
		m_head = m_tail;
		
		MUTEX_UNLOCK(&m_lock);
	}
	
private:
	pthread_mutex_t	m_lock;
	int 			m_size;
	QueueElem*		m_pQueue;
	int				m_head;
	int				m_tail;
};

#endif //USE_EVENT_QUEUE_H
