///////////////////////////////////////////////////////////////////////////////
/// \class PerformanceTimer
/// Performance Timer<BR>
/// This class is for calculating the elapsed time with high resolution timer.
/// 
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDE_PERFORMANCE_TIMER__
#define __INCLUDE_PERFORMANCE_TIMER__

namespace rtk    {
namespace tools  {
    class PerformanceTimer
    {
        public:
            /// Start to record time.<BR>
            /// You should call this function to get a start point and then call
            /// ElapsedTime() to get the elapsed time from this start point.
            /// 
            void Start();
            
            /// Get elapsed time in milli-seconds.
            ///
            /// @return Elapsed time in milli-seconds.
            ///
            double ElapsedTimeInMs();
            
            PerformanceTimer();
            ~PerformanceTimer();
            
        private:
            class PerfTimerImpl;
            PerfTimerImpl* m_pTimerImpl;
    };
};
};

#endif