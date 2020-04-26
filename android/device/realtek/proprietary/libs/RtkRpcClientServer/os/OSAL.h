#ifndef __INCLUDE_OSAL__
#define __INCLUDE_OSAL__    

namespace rtk {
namespace os  {
    enum
    {
        TIME_INFINITY = -1,
    };

    typedef struct tag_osal_thread_t 
    {
        unsigned char dummy[96];
    }osal_thread_t; 

    typedef struct tag_osal_mutex_t 
    {
        unsigned char dummy[32];
    }osal_mutex_t;

    typedef struct tag_osal_event_t 
    {
        unsigned char dummy[80];
    }osal_event_t;

    typedef struct tag_osal_sem_t 
    {
        unsigned char dummy[88];
    }osal_sem_t;

    void osal_Sleep(long millisecondsSleep);

    class CCritSec
    {
        public:
            CCritSec();
            ~CCritSec(void);
            void Lock(void);
            void Unlock(void);

        private:
            CCritSec(const CCritSec &refCritSec);
            CCritSec &operator=(const CCritSec &refCritSec);
            
        private:
            void *m_pCritSec;
    };

    class CAutoLock
    {
        public:
            CAutoLock(CCritSec* pLock);
            ~CAutoLock(void);
            
        private:
            CAutoLock(const CAutoLock &refAutoLock);
            CAutoLock &operator=(const CAutoLock &refAutoLock);
            
        protected:
            CCritSec* m_pLock;
    };

    class CThread
    {
        public:
            CThread(void (*start_address)(void*), void* pThreadData);
            ~CThread(void);
            bool Run(int stackSize = 0);
            bool IsRun(void);
            void Exit(bool bWaitForExit);
            bool IsAskToExit(void);
            
        protected:
            void (*m_start_address)(void*);
            void* m_pThreadData;
            osal_thread_t m_thread;
            unsigned char m_Flag;
    };

    class CEvent
    {
        public:
            CEvent(bool bSignaled = false);
            ~CEvent();
            
            // Set event to signaled.
            //
            void Set();
            
            // Set event to non-signaled.
            //
            void Reset();
            
            // Wait for event to be signaled.
            //
            //  Parameters:
            //      1. iTimeOut: Time out in milli-seconds
            //
            //  Return:
            //      true --> Success
            //      false--> Time out.
            //
            bool Wait(long iTimeOut);
            
        private:
            osal_event_t m_Event;
    };
}; // namespace os
}; // namespace rtk

#endif
