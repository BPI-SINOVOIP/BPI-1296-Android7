////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcServerObserver 
/// Inter-Process Communication Server Side Observer Interface
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_SERVER_OBSERVER
#define INCLUDED_IPC_SERVER_OBSERVER

namespace rtk {
namespace ipc {
    class IpcStreamer;
    class IpcServerObserver
    {
        public:
            /// When server is opened, it will notify all of observers by calling
            /// this function.
            ///
            /// @param pStrServerName The opened server name.
            ///
            virtual void Opened(const char* pStrServerName) = 0;
            
            /// When server accept a client, it will notify all of observers by 
            /// calling this function.
            /// NOTE: You should open the returned streamer immediately,
            ///       if you need this streamer right now or later.
            ///       Otherwise, the returned streamer may be released by 
            ///       server's garbage collector when this function returned.
            ///
            /// @param pStreamer The pointer points to new accepted client's streamer.
            ///
            /// @return Return true to accept this client.<BR>
            ///         Return false to reject this client.
            ///
            virtual bool Accepted(IpcStreamer* pStreamer) = 0;
            
            /// When server is closed, it will notify all of observers by calling
            /// this function.
            ///
            virtual void Closed() = 0;
    
        public:
            virtual ~IpcServerObserver() { ; };
    };
}; // namespace ipc
}; // namespace rtk

#endif
