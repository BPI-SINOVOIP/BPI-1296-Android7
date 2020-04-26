////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcClient
/// Inter-Process Communication Client Side Interface
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_CLIENT
#define INCLUDED_IPC_CLIENT

namespace rtk {
namespace ipc {
    class IpcStreamer;
    class IpcClient
    {
        public:
            /// Connect to server.
            ///
            /// @param pStrServerName Server name.
            /// @param pParam The parameter used to connect.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            virtual bool Connect(const char* pStrServerName, void* pParam) = 0;
            
            /// Disconnect.
            ///
            virtual void Disconnect() = 0;
            
            /// Get streamer to read/write data to server.
            ///
            /// @return The pointer of instance of IpcStreamer.
            ///
            virtual IpcStreamer* GetStreamer() = 0;
    
        public:
            virtual ~IpcClient() { ; };
    };
}; // namespace ipc
}; // namespace rtk

#endif
