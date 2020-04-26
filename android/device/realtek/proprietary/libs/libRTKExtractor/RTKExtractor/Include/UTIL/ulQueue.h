#ifndef __UL_QUEUE_H__
#define __UL_QUEUE_H__

#include <stdlib.h>
#include <OSAL/SmartClass.h>
#include <UTIL/ulDebug.h>
// 04/23/04
// This queue does NOT require mutex for critical section management
// because EnQueue & DeQueue modify m_Tail & m_Head respectively

#define _IS_QUEUE_FULL()                (Size() >= m_ArraySize - 1)
#define _IS_QUEUE_EMPTY()               ((m_Head)== (m_Tail))


template <class T>
class ulCircularQueue{
public:
                                ulCircularQueue(
                                    const unsigned int      QueueSize,
                                    const bool              bCritSect= false
				);
				~ulCircularQueue(void);
    bool                        IsFull(void) const;
    bool                        IsEmpty(void) const;
    bool                        EnQueue(
                                    const T&                RHS
				);
    bool                        DeQueue(
                                    T&                      LHS
                                );
    bool                        Head(
                                    T&                      LHS
                                );
    unsigned int                Size(void) const;
    unsigned int                QueueSize(void) const;
    void                        Reset(void);

protected:
    T*                          m_pArray;
    unsigned int                m_ArraySize;
    unsigned int                m_Head;
    unsigned int                m_Tail;

    CCritSec*                   m_pLock;
    // unsigned char               m_flag;

private:
    // hidden copy constructor
                                ulCircularQueue(const ulCircularQueue&);
    // hidden assignment operator
    const ulCircularQueue&      operator= (const ulCircularQueue&);
};

template <class T>
class ulSemCirQueue{
public:
                                ulSemCirQueue(
                                    const unsigned int      QueueSize
				);
								~ulSemCirQueue(void){};

    bool                        IsFull(void) const;
    bool                        IsEmpty(void) const;
    bool                        EnQueue(
                                    const T&                RHS,
                                    long                    milliTimeOut
				);
    bool                        DeQueue(
                                    T&                      LHS,
                                    long                    milliTimeOut
                                );
    bool                        Head(
                                    T&                      LHS,
                                    long                    milliTimeOut
                                );

    unsigned int                Size(void) const;
    void                        Reset(void);

protected:
    ulCircularQueue<T>          m_Queue;
    CSemaphore                  m_SemFreeSpace;
    CSemaphore                  m_SemAvailElement;

private:
    // hidden copy constructor
                                ulSemCirQueue(const ulSemCirQueue&);
    // hidden assignment operator
    const ulSemCirQueue&        operator= (const ulSemCirQueue&);
};

typedef ulCircularQueue<void*>          VoidArr;
typedef ulSemCirQueue<void*>            VoidArrSem;

template <class T>
inline
ulCircularQueue<T>::ulCircularQueue(
    const unsigned int      QueueSize,
    const bool              bCritSect):
    // m_flag(0),
    m_ArraySize(QueueSize+ 1),
    m_Head(QueueSize),
    m_Tail(QueueSize)
{
    DASSERT(QueueSize> 0);
    m_pArray= (T*)malloc(m_ArraySize*sizeof(T));

    if (bCritSect)
        m_pLock=        new CCritSec;
    else
        m_pLock=        NULL;
}

template <class T>
inline
ulCircularQueue<T>::~ulCircularQueue(void)
{
    if (m_pArray) free(m_pArray);
    if (m_pLock) delete m_pLock;
}

template <class T>
bool  
ulCircularQueue<T>::IsFull(void) const
{
    return _IS_QUEUE_FULL();
}

template <class T>
bool  
ulCircularQueue<T>::IsEmpty(void) const
{
    return _IS_QUEUE_EMPTY();
}

template <class T>
unsigned int
ulCircularQueue<T>::Size(void) const
{
    int                 _size= m_Tail- m_Head;

    return (_size>= 0)? (unsigned int)_size: (unsigned int)(_size+ m_ArraySize);
}

template <class T>
unsigned int
ulCircularQueue<T>::QueueSize(void) const
{
    return m_ArraySize- 1;
}

template <class T>
bool  
ulCircularQueue<T>::EnQueue(const T& RHS)
{
    if (_IS_QUEUE_FULL())
        return false;

    if (m_pLock)
        m_pLock->Lock();
#if 0
    if (++_Tail>= m_ArraySize)
        _Tail-= m_ArraySize;
#else
    // _Tail> m_ArraySize should not happen
    if (++m_Tail== m_ArraySize)
        m_Tail= 0;
#endif
    
    m_pArray[m_Tail]= RHS;
    if (m_pLock)
        m_pLock->Unlock();
    return true;
}

template <class T>
bool  
ulCircularQueue<T>::DeQueue(T& LHS)
{
    if (_IS_QUEUE_EMPTY())
        return false;

    if (m_pLock)
        m_pLock->Lock();

#if 0
    if (++_Head>= m_ArraySize)
        _Head-= m_ArraySize;
#else
    // _Head> m_ArraySize should not happen
    if (++m_Head== m_ArraySize)
        m_Head= 0;
#endif

    LHS= m_pArray[m_Head];
    if (m_pLock)
        m_pLock->Unlock();
    return true;
}

template <class T>
bool  
ulCircularQueue<T>::Head(T& LHS)
{
    if (_IS_QUEUE_EMPTY())
        return false;

/*
#if 0
    if (++_Head>= m_ArraySize)
        _Head-= m_ArraySize;
#else
    // _Head> m_ArraySize should not happen
    if (++m_Head== m_ArraySize)
        m_Head= 0;
#endif
*/

    unsigned int        _Head= m_Head;
    if (++_Head== m_ArraySize)
        _Head= 0;
    LHS= m_pArray[_Head];
    return true;
}

template <class T>
void  
ulCircularQueue<T>::Reset(void)
{
    m_Tail=     m_Head=         m_ArraySize- 1;
    memset((void*)m_pArray, 0, m_ArraySize*sizeof(T));
}

/*****************************************************************************/
/* Implementation of ulSemCirQueue                                           */
/*****************************************************************************/
template <class T>
inline
ulSemCirQueue<T>::ulSemCirQueue(
    const unsigned int      QueueSize):
    m_Queue(QueueSize)
{
    m_SemFreeSpace.Init(0, QueueSize);
    m_SemAvailElement.Init(0, 0);
}

template <class T>
bool  
ulSemCirQueue<T>::IsFull(void) const
{
    return m_Queue.IsFull();
}

template <class T>
unsigned int
ulSemCirQueue<T>::Size(void) const
{
    return m_Queue.Size();
}

template <class T>
bool  
ulSemCirQueue<T>::IsEmpty(void) const
{
    return m_Queue.IsEmpty();
}

template <class T>
bool  
ulSemCirQueue<T>::EnQueue(
    const T&                RHS,
    long                    milliTimeOut)
{
    if (milliTimeOut< 0)
        m_SemFreeSpace.Wait();
    else{
        if (m_SemFreeSpace.TimedWait(milliTimeOut) != true){
            // return Time Out; //????
            return false;
        }
    }

    if (m_Queue.EnQueue(RHS)== true){
        m_SemAvailElement.Post();
        return true;
    }
    DASSERT(0);
    return false;
}

template <class T>
bool  
ulSemCirQueue<T>::DeQueue(
    T&                      LHS,
    long                    milliTimeOut)
{
    if (milliTimeOut< 0)
        m_SemAvailElement.Wait();
    else{
        if (m_SemAvailElement.TimedWait(milliTimeOut)!= true){
            // return Time Out; //????
            return false;
        }
    }

    if (m_Queue.DeQueue(LHS)== true){
        m_SemFreeSpace.Post();
        return true;
    }
    DASSERT(0);
    return false;
}

template <class T>
bool  
ulSemCirQueue<T>::Head(
    T&                      LHS,
    long                    milliTimeOut)
{
    if (milliTimeOut< 0)
        m_SemAvailElement.Wait();
    else{
        if (m_SemAvailElement.TimedWait(milliTimeOut)!= true){
            // return Time Out; //????
            return false;
        }
    }

    if (m_Queue.Head(LHS)== true){
        // Head didn't remove the element from the queue.
        // Therefore, Post() is call to make balance sine a Wait() is call ahead
        m_SemAvailElement.Post();
        return true;
    }
    DASSERT(0);
    return false;
}

template <class T>
void  
ulSemCirQueue<T>::Reset(void)
{
    m_SemFreeSpace.Init(0, m_Queue.QueueSize());
    m_SemAvailElement.Init(0, 0);
    m_Queue.Reset();
}

////////////////////////////////////////////////////////////////////////////////
// CVariableSizeQueue

struct CVariableSizeQueueItem {
    long dataID;
    long dataIndex;
    long dataSize;
};

template <long POOL_SIZE, long MAX_ITEMS>
class CVariableSizeQueue
{
protected:

    long m_dataPool[POOL_SIZE];
    long m_indexHead;
    long m_indexTail;

    ulSemCirQueue<CVariableSizeQueueItem> m_refQueue;
    CCritSec*                   m_pLock;

public:

    CVariableSizeQueue() : m_refQueue(MAX_ITEMS) {
        m_indexHead = m_indexTail = 0;
        m_pLock=        new CCritSec;
    }

    ~CVariableSizeQueue() {
        if (m_pLock)    delete m_pLock;
    }

    bool EnQueue(
        long dataID,
        long *pData,
        long dataSize,
        long milliTimeOut) {

        assert(dataSize >= 0);
        assert(dataSize == 0 || pData != NULL);

        if (m_pLock)
            m_pLock->Lock();

		if(dataSize > 0) {
			if(m_indexTail+dataSize > POOL_SIZE) {
                if(m_indexHead == 0) {
                    if (m_pLock)
                        m_pLock->Unlock();
                    assert(false); // run out of space, unexpected
                    return false;
                }
                m_indexTail = 0;
            }

            if(m_indexHead > m_indexTail && m_indexTail+dataSize >= m_indexHead) {
                if (m_pLock)
                    m_pLock->Unlock();
        		assert(false); // run out of space, unexpected
                return false;
            }

            memcpy(&m_dataPool[m_indexTail], pData, dataSize*sizeof(long));
        }

        CVariableSizeQueueItem item;
        item.dataID = dataID;
        item.dataIndex = m_indexTail;
        item.dataSize = dataSize;

        bool res = m_refQueue.EnQueue(item, milliTimeOut);

        if(res) {
            m_indexTail += dataSize;
            if(m_indexTail == POOL_SIZE)
                m_indexTail = 0;
        }

        if (m_pLock)
            m_pLock->Unlock();		
        return res;
    }

    bool Peek(
        long *pDataID,
        long **ppData,
        long *pDataSize,
        long milliTimeOut) {
        CVariableSizeQueueItem item;

        if(!m_refQueue.Head(item, milliTimeOut)) 
            return false;

        if(pDataID)
            *pDataID = item.dataID;
        if(ppData)
            *ppData = (item.dataSize==0)? NULL : &m_dataPool[item.dataIndex];
        if(pDataSize)
            *pDataSize = item.dataSize;

        return true;
    }

    bool DeQueue() {
        CVariableSizeQueueItem item;

        if (m_pLock)
            m_pLock->Lock();

        if(!m_refQueue.DeQueue(item, -1)) {
            if (m_pLock)
                m_pLock->Unlock();
            return false;
        }

        m_indexHead = item.dataIndex + item.dataSize;

        if (m_pLock)
            m_pLock->Unlock();
        return true;
    }

    void Reset() {
        m_refQueue.Reset();
        m_indexHead = m_indexTail = 0;
    }
};

#endif
