////////////////////////////////////////////////////////////////////////////////
/// A thread-safe queue
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_SAFE_QUEUE__
#define __INCLUDED_SAFE_QUEUE__

#include <list>
#include "os/OSAL.h"

namespace rtk    {
namespace tools  {

template <typename T> class SafeQueue
{
    public:
        SafeQueue(): m_iMaxAllowedItemNum(-1)
        {
            m_MaxItemNumEvent.Set();
        };
        
        ~SafeQueue() { m_MaxItemNumEvent.Set(); };
    
        void Push(T item)
        {
            m_CritSec.Lock();
            if (IsExceedMaxItemCounts()) 
            {
                // We should unlock mutex before we go to waiting state.
                // Otherwise, the next calling to Pop() will be blocked.
                // Mantis 24688
                m_CritSec.Unlock();
                m_MaxItemNumEvent.Wait(rtk::os::TIME_INFINITY);
                m_CritSec.Lock();
            }
            m_Queue.push_back(item);
            if (IsExceedMaxItemCounts()) m_MaxItemNumEvent.Reset();
            m_CritSec.Unlock();
        };

        // Like Pop(), return the first item in the queue. 
        // Peek() will not remove the item, but Pop() will.
        // 
        bool Peek(T& item)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            if (!m_Queue.size()) return false;
            item = m_Queue.front();
            return true;
        };
        
        bool Pop(T& item)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            if (!m_Queue.size()) return false;
            
            item = m_Queue.front();
            m_Queue.pop_front();
            
            if (!IsExceedMaxItemCounts()) m_MaxItemNumEvent.Set();
            return true;
        };
        
        bool IsInQueue(T item)
        {
            return Find(item);
        };
        
        size_t GetCounts()
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            return m_Queue.size();
        };

        void Clear()
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            m_Queue.clear();
        };
        
        bool Find(T& item, bool bRemoveIfExist = false)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            if (!m_Queue.size()) return false;
            
            typename std::list<T>::iterator itor, itorEnd = m_Queue.end();
            for (itor = m_Queue.begin(); itor != itorEnd; ++itor)
            {
                // Please ensure that your T supports operator "=="
                if (*itor == item) 
                { 
                    item = *itor; // Copy data.
                    if (bRemoveIfExist) 
                    {
                        m_Queue.erase(itor);
                        if (!IsExceedMaxItemCounts()) m_MaxItemNumEvent.Set(); 
                    } 
                    return true; 
                }
            }
            return false;
        };
        
        void SetMaxAllowedItemCounts(int iCounts)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            m_iMaxAllowedItemNum = iCounts;
        };

    private:
        inline bool IsExceedMaxItemCounts()
        {
            return m_iMaxAllowedItemNum > 0 && 
                   (int)m_Queue.size() >= m_iMaxAllowedItemNum;
        };
        
    private:
        std::list<T> m_Queue;
        rtk::os::CCritSec m_CritSec;
        rtk::os::CEvent m_MaxItemNumEvent;
        int m_iMaxAllowedItemNum;
};

template <> class SafeQueue<void*>
{
    public:
        SafeQueue(): m_iMaxAllowedItemNum(-1)
        {
            m_MaxItemNumEvent.Set();
        };
        
        ~SafeQueue() { m_MaxItemNumEvent.Set(); };
        
        void Push(void* pItem)
        {
            m_CritSec.Lock();
            if (IsExceedMaxItemCounts()) 
            {
                // We should unlock mutex before we go to waiting state.
                // Otherwise, the next calling to Pop() will be blocked.
                // Mantis 24688
                m_CritSec.Unlock();
                m_MaxItemNumEvent.Wait(rtk::os::TIME_INFINITY);
                m_CritSec.Lock();
            }
            m_Queue.push_back(pItem);
            if (IsExceedMaxItemCounts()) m_MaxItemNumEvent.Reset();
            m_CritSec.Unlock();
        };
        
        // Like Pop(), return the first item in the queue. 
        // Peek() will not remove the item, but Pop() will.
        // 
        bool Peek(void*& pItem)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            pItem = NULL;
            if (!m_Queue.size()) return false;
            pItem = m_Queue.front();
            return true;
        };
        
        bool Pop(void*& pItem)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            pItem = NULL;
            if (!m_Queue.size()) return false;
            pItem = m_Queue.front();
            m_Queue.pop_front();
            
            if (!IsExceedMaxItemCounts()) m_MaxItemNumEvent.Set();
            return true;
        };

        bool IsInQueue(void* pItem)
        {
            return Find(pItem);
        };

        size_t GetCounts()
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            return m_Queue.size();
        };       

        void Clear()
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            m_Queue.clear();
        };

        bool Find(void* pItem, bool bRemoveIfExist = false)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            if (!m_Queue.size()) return false;
            
            std::list<void*>::iterator itor, itorEnd = m_Queue.end();
            for (itor = m_Queue.begin(); itor != itorEnd; ++itor)
            {
                if (*itor == pItem) 
                {
                    if (bRemoveIfExist) 
                    {
                        m_Queue.erase(itor);
                        if (!IsExceedMaxItemCounts()) m_MaxItemNumEvent.Set(); 
                    }
                    return true;
                }
            }
            return false;
        };

        void SetMaxAllowedItemCounts(int iCounts)
        {
            rtk::os::CAutoLock autoLock(&m_CritSec);
            m_iMaxAllowedItemNum = iCounts;
        };
        
    private:
        inline bool IsExceedMaxItemCounts()
        {
            return m_iMaxAllowedItemNum > 0 && 
                   (int)m_Queue.size() >= m_iMaxAllowedItemNum;
        };
        
    private:
        std::list<void*> m_Queue;
        rtk::os::CCritSec m_CritSec;
        rtk::os::CEvent m_MaxItemNumEvent;
        int m_iMaxAllowedItemNum;
};

template <typename T> class SafeQueue<T*>
{
    public:
        void Push(T* pItem)
        {
            m_Queue.Push(pItem);
        };
        
        // Like Pop(), return the first item in the queue. 
        // Peek() will not remove the item, but Pop() will.
        // 
        bool Peek(T*& pItem)
        {
            void* pTmp = NULL;
            pItem = NULL;
            if (!m_Queue.Peek(pTmp)) return false;
            pItem = (T*)pTmp;
            return true;
        };
        
        bool Pop(T*& pItem)
        {
            void* pTmp = NULL;
            pItem = NULL;
            if (!m_Queue.Pop(pTmp)) return false;
            pItem = (T*)pTmp;
            return true;
        };

        bool IsInQueue(T* pItem)
        {
            return m_Queue.IsInQueue(pItem);
        };

        size_t GetCounts()
        {
            return m_Queue.GetCounts();
        }; 

        void Clear()
        {
            m_Queue.Clear();
        };

        bool Find(T* pItem, bool bRemoveIfExist = false)
        {
            return m_Queue.Find(pItem, bRemoveIfExist);
        };
        
        void SetMaxAllowedItemCounts(int iCounts)
        {
            m_Queue.SetMaxAllowedItemCounts(iCounts);
        };
        
    private:
        SafeQueue<void*> m_Queue;
};

}; // namespace tools
}; // namespace rtk

#endif
