////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcServerPipeImpl 
// Inter-Process Communication Server Side - Pipe Implementation
//
// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_SERVER_PIPE_IMPL
#define INCLUDED_IPC_SERVER_PIPE_IMPL

#include "ipc/server/IpcServerBase.h"

namespace rtk {
namespace ipc {
    class IpcServerPipeImpl: public IpcServerBase
    {   
        public:
            IpcServerPipeImpl();
            ~IpcServerPipeImpl();

        private: // Implementation of interface in IpcServerBase
            bool DoStart(const char* pStrServerName, void* pParam);
            bool DoStop();
            IpcStreamer* DoAccept();
            void FreeIpcStreamer(IpcStreamer* pStreamer);
            bool IsShutdownServer();
            
        private:
            struct PrivateImpl;
            PrivateImpl* m_pImpl;
    };
}; // namespace ipc
}; // namespace rtk

#endif
