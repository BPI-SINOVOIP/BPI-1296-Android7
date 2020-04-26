#ifndef __SMART_CLASS_H__
#define __SMART_CLASS_H__

#define __USE_OSAL_API__
#ifndef __USE_OSAL_API__

// CriticalSection
class CCritSec{
public:
    CCritSec();
    ~CCritSec(void);
    void Lock(void);
    void Unlock(void);

private:
    // make copy constructor and assignment operator unavailabe to anyone(including itself
        CCritSec(const CCritSec &refCritSec);
        CCritSec &operator=(const CCritSec &refCritSec);
private:
        void *m_pCritSec;
};

class CAutoLock{
public:
        CAutoLock(CCritSec* pLock);
        ~CAutoLock(void);
private:
        // make copy constructor and assignment operator inaccessible
        CAutoLock(const CAutoLock &refAutoLock);
        CAutoLock &operator=(const CAutoLock &refAutoLock);
protected:
    CCritSec*                   m_pLock;
};


class CSemaphore {
        void *m_pSem;
        char *m_name;
public:
        CSemaphore();
        ~CSemaphore();
        // Init :: initial internal values for the semaphore, 
        // if name==NULL, it is unnamed semaphore. For cross process semaphore, use named semaphore
        void Init(char *name, unsigned long initValue);
        void Reset(unsigned long resetValue);
        void Wait(); // each wait will decrease the semaphore by 1, until the semaphore reach 0
        void Post(); // release the semaphore
        bool TimedWait(unsigned long milliseconds); // function will return false if semaphore still 0 after it try for the time 
};

#else

#include <hresult.h>
#include <OSAL/Threads.h>
#ifndef TIME_INFINITY
#define TIME_INFINITY (-1)
#endif
class CCritSec
{
    osal_mutex_t m_mutex;

public:

    inline CCritSec()    { osal_MutexCreate(&m_mutex);  }
    inline ~CCritSec()   { osal_MutexDestroy(&m_mutex); }
    inline void Lock()   { osal_MutexLock(&m_mutex);    }
    inline void Unlock() { osal_MutexUnlock(&m_mutex);  }
};

class CAutoLock
{
    CCritSec* m_pLock;

public:

    inline CAutoLock(CCritSec* pLock) { m_pLock = pLock; m_pLock->Lock(); }
    inline ~CAutoLock()               { m_pLock->Unlock();                }
};

class CSemaphore
{
    osal_sem_t m_sem;

public:

    inline CSemaphore()  { osal_SemCreate(0x7fffffff, 0, &m_sem); }
    inline ~CSemaphore() { osal_SemDestroy(&m_sem);               }

    inline void Init(char */*name*/, unsigned long initValue)
    {
        osal_SemSetCount(&m_sem, initValue);
    }

    inline void Reset(unsigned long resetValue)
    {
        osal_SemSetCount(&m_sem, resetValue);
    }

    inline void Wait()
    {
        osal_SemWait(&m_sem, TIME_INFINITY);
    }
        
    inline void Post()
    {
        osal_SemGive(&m_sem);
    }

    inline bool TimedWait(unsigned long milliseconds) // return false on time-out
    {
        return (osal_SemWait(&m_sem,milliseconds) == S_OK)? true : false;
    }
};

#endif

class CPolling{
public:
                                CPolling(
                                    long                    TotalTime,
                                    long                    SleepSlot
                                );
                                ~CPolling(void);
    void                        Sleep(void);
    bool                        IsTimeOut(void);
protected:
    long                        m_milliSleepSlot;
    void*                       m_pTimeOut;
};

class CThread{
public:
                                CThread(
                                    void (*start_address)(void*),
                                    void* pThreadData
                                );
                                ~CThread(void);
    bool                        Run(void);
    bool                        IsRun(void);
    void                        Exit(bool bWaitForExit);
    bool                        IsAskToExit(void);
protected:
    void                        (*m_start_address)(void*);
    void*                       m_pThreadData;
    osal_thread_t               m_thread;
    unsigned char               m_Flag;
};

class CEvent
{
    public:
        CEvent(bool bSignaled = false) { osal_EventCreate(&m_Event); if (bSignaled) Set(); };
        ~CEvent() { osal_EventDestroy(&m_Event); };
        
        // Set event to signaled.
        //
        void Set() { osal_EventSet(&m_Event); };
        
        // Set event to non-signaled.
        //
        void Reset() { osal_EventReset(&m_Event); };
        
        // Wait for event to be signaled.
        //
        //  Parameters:
        //      1. iTimeOut: Time out in milli-seconds
        //
        //  Return:
        //      true --> Success
        //      false--> Time out.
        //
        bool Wait(long iTimeOut) 
        { 
            return osal_EventWait(&m_Event, iTimeOut) == S_OK; 
        };
        
    private:
        osal_event_t m_Event;
};

#endif
