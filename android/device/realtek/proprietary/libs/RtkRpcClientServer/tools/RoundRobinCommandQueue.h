////////////////////////////////////////////////////////////////////////////////
/// \class RoundRobinCommandQueue
/// A set of CommandQueue which process commands in a round-robin way.<BR>
/// This class is based on template class CommandQueue.
///
/// @see CommandQueue
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDE_ROUND_ROBIN_COMMAND_QUEUE__
#define __INCLUDE_ROUND_ROBIN_COMMAND_QUEUE__

#include "CommandQueue.h"

namespace rtk    {
namespace tools  {
    template <class T, int ThreadCounts, int ThreadStackSize = 0>
    class RoundRobinCommandQueue
    {
        private:
            typedef CommandQueue<T, Policy1, ThreadStackSize> CmdQue;
            
        public:
            RoundRobinCommandQueue(CommandProcessor<T>* pCmdProcessor): m_iCmdQueIndex(0)
            {
                for (int i = 0; i < ThreadCounts; i++)
                    m_CmdQue[i] = new CmdQue(pCmdProcessor);
            }
            
            ~RoundRobinCommandQueue()
            {
                for (int i = 0; i < ThreadCounts; i++)
                    delete m_CmdQue[i];
            }
        
            void AddCommand(const T& cmd)
            {
                int i = 0;
                {
                    rtk::os::CAutoLock autoLock(&m_CS);
                    i = m_iCmdQueIndex++;
                    if (m_iCmdQueIndex >= ThreadCounts)
                        m_iCmdQueIndex = 0;
                }
                m_CmdQue[i]->AddCommand(cmd);
            }
            
        private:
            CmdQue* m_CmdQue[ThreadCounts];
            int m_iCmdQueIndex;
            rtk::os::CCritSec m_CS;
    };
}; // namespace tools
}; // namespace rtk

#endif