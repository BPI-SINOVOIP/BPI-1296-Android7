////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcClientPipeImpl
/// Inter-Process Communication Client Side - Pipe Implementation
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_CLIENT_PIPE_IMPL
#define INCLUDED_IPC_CLIENT_PIPE_IMPL

#include "ipc/client/IpcClient.h"

namespace rtk {
namespace ipc {
    class IpcStreamer;
    class IpcClientPipeImpl: public IpcClient
    {
        public: // The implementation of IpcClient.
            bool Connect(const char* pStrServerName, void* pParam);
            void Disconnect();
            IpcStreamer* GetStreamer();
            
        public:
            IpcClientPipeImpl();
            ~IpcClientPipeImpl();
            
        private:
            IpcStreamer* m_pStreamer;
    };
}; // namespace ipc
}; // namespace rtk

#endif
