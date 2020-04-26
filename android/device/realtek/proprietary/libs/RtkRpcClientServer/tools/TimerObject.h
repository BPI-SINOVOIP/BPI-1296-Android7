////////////////////////////////////////////////////////////////////////////////
/// \class TimerObject
/// A tiny timer <BR>
///
/// @author: zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_TIMER_OBJECT__
#define __INCLUDED_TIMER_OBJECT__

namespace rtk {
namespace os  {
    class CEvent;
    class CThread;
};
};

namespace rtk    {
namespace tools  {

/// Prototype of timer callback function
///
/// @param pParam Parameter assigned to TimerObject::Start().
/// @param iShots How many shots triggered by this timer.
///
typedef void (*fpTimerCallback)(void* pParam, int iShots);

class TimerObject
{
    public:
        enum { INFINITE_SHOT = -1 };
    
        TimerObject();
        ~TimerObject();

        /// Start timer
        /// 
        /// @param iTimerInterval Timer interval(in milli-seconds)
        /// @param TimerCallback Timer callback.<BR>
        ///                      When timer interval is elapsed, timer callback 
        ///                      will be called.
        /// @param iShots How many shots will be triggered by this timer.
        ///               Pass INFINITE_SHOT means infinite shots.
        /// @param bImmediately true --> fpTimerCallback will be triggered immediately.
        /// @param pParam Parameter passed to TimerCallback.
        /// @param iThreadStackSize Thread stack size.
        ///
        /// @return true --> Success
        ///
        bool Start(unsigned int iTimerInterval, 
                   fpTimerCallback TimerCallback,
                   void* pParam,
                   int iShots = INFINITE_SHOT, 
                   bool bImmediately = false,
                   int iThreadStackSize = 0);

        /// Stop timer.
        ///
        void Stop();
        
        /// Check if this timer is running
        ///
        /// @return true --> This timer is running.
        ///
        bool IsRunning() const;
        
    private:
        static void ThreadProc(void* pParam);
        
    private:
        rtk::os::CEvent* m_pEvent;
        rtk::os::CThread* m_pThread;
        unsigned int m_iTimerInterval;
        fpTimerCallback m_fpTimerCallback;
        void* m_pParam;
        bool m_bStop;
        bool m_bThreadRunning;
        int m_iShots;
        bool m_bImmediately;
        bool m_bThreadStarted;
};

}; // namespace tools
}; // namespace rtk

#endif
