////////////////////////////////////////////////////////////////////////////////
/// \class CommandQueue
/// A command queue. This queue will spawn a thread to process all of commands.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_COMMAND_QUEUE__
#define __INCLUDED_COMMAND_QUEUE__

#include "SafeQueue.h"
#include "CommandProcessor.h"
#include "os/OSAL.h"
#include "tools/MemoryTracer.h"

namespace rtk    {
namespace tools  {

enum CommandQueuePolicy
{
    /// This policy will wait for command execution finished.
    /// If the execution result is failed(m_pCmdProcessor->Execute() return false), 
    /// the CommandQueue will stop launching commands.
    ///
    Policy1,
    
    /// This policy will wait for command execution finished.
    /// If the execution result is failed(m_pCmdProcessor->Execute() return false), 
    /// the failed command will be re-queued.
    ///
    Policy2,
};

template <typename T, CommandQueuePolicy Policy = Policy1, int ThreadStackSize = 0> class CommandQueue
{
    public:
        CommandQueue(CommandProcessor<T>* pCmdProcessor): m_pThread(NULL),
                                                          m_pCmdProcessor(pCmdProcessor),
                                                          m_bStopThread(false),
                                                          m_pItemEvent(new rtk::os::CEvent)
        { 
        };
        
        ~CommandQueue() 
        { 
            if (m_pThread)
            {        
                m_bStopThread = true;
                m_pItemEvent->Set();
                m_pThread->Exit(true);
                delete m_pThread;
            }
            
            ClearCommand();
            
            if (m_pItemEvent) 
                delete m_pItemEvent;
        };
        
        void AddCommand(T cmd) 
        { 
            if (!m_pThread && m_pCmdProcessor)
            {
                m_pThread = new rtk::os::CThread(GetPolicyProc(), this);
                m_pThread->Run(ThreadStackSize);
            }
            m_CmdQueue.Push(cmd);
            m_pItemEvent->Set();
        };
        
        void ClearCommand() 
        { 
            T item;
            while(m_CmdQueue.GetCounts()) 
            {
                m_CmdQueue.Pop(item);
                m_pCmdProcessor->Remove(item);
            }
        };
        
        bool IsExistCommand(T cmd)
        {
            return m_CmdQueue.IsInQueue(cmd);
        }
        
    private:
        SafeQueue<T> m_CmdQueue;
        rtk::os::CThread* m_pThread;
        CommandProcessor<T>* m_pCmdProcessor;
        bool m_bStopThread;
        rtk::os::CEvent* m_pItemEvent;
        
    private:
        static void Policy1Proc(void* pParam)
        {
            CommandQueue<T, Policy, ThreadStackSize>* pSelf = (CommandQueue<T, Policy, ThreadStackSize>*) pParam;
            if (!pSelf->m_pCmdProcessor) return;
            
            while (!pSelf->m_bStopThread)
            {
                pSelf->m_pItemEvent->Wait(rtk::os::TIME_INFINITY);
                if (pSelf->m_bStopThread) break;
                
                T cmd;
                while (!pSelf->m_bStopThread && 
                       pSelf->m_CmdQueue.Peek(cmd) &&
                       pSelf->m_pCmdProcessor->Execute(cmd) &&
                       pSelf->m_CmdQueue.Pop(cmd)) 
                {
                    pSelf->m_pCmdProcessor->Remove(cmd);
                }
                pSelf->m_pItemEvent->Reset();
            }
        };

        static void Policy2Proc(void* pParam)
        {
            CommandQueue<T, Policy, ThreadStackSize>* pSelf = (CommandQueue<T, Policy, ThreadStackSize>*) pParam;
            if (!pSelf->m_pCmdProcessor) return;
            
            while (!pSelf->m_bStopThread)
            {
                pSelf->m_pItemEvent->Wait(rtk::os::TIME_INFINITY);
                if (pSelf->m_bStopThread) break;
                
                T cmd;
                size_t iCounts = pSelf->m_CmdQueue.GetCounts();
                while (!pSelf->m_bStopThread && iCounts)
                {
                    size_t iReQueueCounts = 0;
                    for(size_t i = 0; i < iCounts; i++)
                    {
                        if (pSelf->m_bStopThread || 
                            !pSelf->m_CmdQueue.Peek(cmd)) 
                            break;
                        if (!pSelf->m_pCmdProcessor->Execute(cmd))
                        {
                            pSelf->m_CmdQueue.Push(cmd); // Re-queue this command
                            iReQueueCounts++;
                        }
                        pSelf->m_CmdQueue.Pop(cmd);
                    }
                    
                    // The existed commands are all re-queued, we sleep awhile 
                    // before going to the next round.
                    //
                    iCounts = pSelf->m_CmdQueue.GetCounts();
                    if (iCounts && iCounts == iReQueueCounts) rtk::os::osal_Sleep(100);
                }
                pSelf->m_pItemEvent->Reset();
            }
        };

        typedef void (*fpPolicyProc)(void*);
        fpPolicyProc GetPolicyProc()
        {
            switch(Policy)
            {
                case Policy1: return Policy1Proc;
                case Policy2: return Policy2Proc;
                default: return NULL;
            };
            return NULL;
        };
};

}; // namespace tools
}; // namespace rtk

#endif
