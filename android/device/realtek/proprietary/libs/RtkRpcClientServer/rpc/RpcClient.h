////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcClient
/// Remote Procedure Call - Client Side
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_CLIENT
#define INCLUDED_RPC_CLIENT

#include "rpc/RpcCommandMuxer.h"
#include "rpc/RpcCommandDemuxer.h"
#include <map>
#include <vector>

namespace rtk {
namespace ipc {
    class IpcStreamer;
    class IpcClient;
};
};

namespace rtk {
namespace os  {
    class CCritSec;
    class CThread;
    class CEvent;
};
};

namespace rtk {
namespace rpc {
    struct RpcCallbackContext;
    
    class RpcClient
    {
        public:
            RpcClient();
            ~RpcClient();
            
            /// Connect to server.
            ///
            /// @param pStrServerName Server name.
            /// @param pParam The parameter used to connect.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            bool Connect(const char* pStrServerName, void* pParam);
            
            /// Disconnect.
            ///
            void Disconnect();
            
            /// Get current connected server name.
            ///
            /// @return Current connected server name.
            ///
            const char* GetServerName() const;
            
        public: // APIs for invoking remote functions.
            /// Push a parameter.
            /// 
            /// @param param The parameter which pass to remote side.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            template <typename T> inline bool Push(T param);
            
            /// Invoke a remote function.<BR>
            /// You need to push the correct parameters before calling this function.
            ///
            /// @param pStrFuncName The remote function name.
            ///
            /// @return true --> Success. false--> Failed.
            ///
            bool Invoke(const char* pStrFuncName);
            
            /// Get returned value.<BR>
            /// After calling Invoke(), you can call GetReturn() to get the 
            /// returned value from remote function call.
            ///
            /// @return Returned value from remote function call.
            ///
            template <typename T> inline T GetReturn(); 
            template <typename T> inline T Get(unsigned char i);          

        private:
            struct CallContext;
            typedef std::map<unsigned long, CallContext*> CallContextMap;
            typedef std::map<unsigned long, CallContext*>::iterator CallContextMapItor;
            typedef std::vector<RpcCallbackContext> CallbackList;
            typedef std::map<unsigned int, CallbackList*> CallbackMap;
            typedef std::map<unsigned int, CallbackList*>::iterator CallbackMapItor;
            
        private:
            CallContext* GetCallContext();
            CallContext* GetCallContext(unsigned int iId);
            CallContext* GetAndSwapCallContext();
            RpcCommandMuxer* GetCommandMuxer();
            RpcCommandDemuxer* GetCommandDemuxer();
            void RemoveAllCachedCallContext();
            void RemoveAllCachedCallbackFunctions();
            void ReleaseResource();
            inline void WaitForThread(bool bWaitForStop);
            static void ThreadProc(void* pParam);

        private:
            char* m_pStrServerName;
            rtk::ipc::IpcStreamer* m_pStreamer;
            rtk::ipc::IpcClient* m_pIpcClient;
            CallContextMap m_CallCtxMap;
            CallbackMap m_CallbackMap;
            rtk::os::CCritSec* m_pCS;
            rtk::os::CThread* m_pThread;
            unsigned long m_iCounter;
            bool m_bIsThreadRunning;
            bool m_bStop;
            
        private:
            class CallbackHandler;
            CallbackHandler* m_pCallbackHandler;
    };

    template <typename T> inline bool RpcClient::Push(T param)
    {
        RpcCommandMuxer* pMuxer = GetCommandMuxer();
        return pMuxer ? pMuxer->Push(param) : false;
    }

    template <typename T> inline T RpcClient::GetReturn()
    {
        RpcCommandDemuxer* pDemuxer = GetCommandDemuxer();
        return pDemuxer ? pDemuxer->Get<T>(0) : T();
    }
    
    template <typename T> inline T RpcClient::Get(unsigned char i)
    {
        RpcCommandDemuxer* pDemuxer = GetCommandDemuxer();
        return pDemuxer ? pDemuxer->Get<T>(i) : T();
    }
}; // namespace rpc
}; // namespace rtk

#endif
