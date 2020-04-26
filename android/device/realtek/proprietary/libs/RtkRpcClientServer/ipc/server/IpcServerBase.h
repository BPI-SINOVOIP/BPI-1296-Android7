////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcServerBase 
/// Inter-Process Communication Server Side - Basic Implementation<BR>
/// <BR>
/// The basic implementation implement register/unregister observer.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_SERVER_BASE
#define INCLUDED_IPC_SERVER_BASE

#include "ipc/server/IpcServer.h"

namespace rtk {
namespace ipc {
    class IpcServerObserver;
    class IpcStreamer;
    class IpcServerBase: public IpcServer
    {
        public: // Implementation of interface defined in IpcServer
            bool Start(const char* pStrServerName, void* pParam);
            bool Stop();
            bool RegisterObserver(IpcServerObserver* pObs);
            void UnregisterObserver(IpcServerObserver* pObs);
            const char* GetServerName();
            void* GetParameter();

        public:
            IpcServerBase();
            ~IpcServerBase();
        
        protected: // sub-class should implement all of functions to do real stuff.
            /// This function is called when user calling Start().
            /// Sub-class can do initialization stuff in here.
            //
            virtual bool DoStart(const char* pStrServerName, void* pParam) = 0;
            
            /// This function is called when user calling Stop().
            /// Sub-class can do finalization stuff in here.
            ///
            virtual bool DoStop() = 0;
            
            /// This function is called in an internal thread.
            /// Sub-class can do acception stuff in here.
            /// If sub-class get a client, wrap it to be an IpcStreamer and pass it
            /// to caller.
            ///
            virtual IpcStreamer* DoAccept() = 0;

            /// This function is used to free IpcStreamer internally.
            ///
            virtual void FreeIpcStreamer(IpcStreamer* pStreamer) = 0;
            
            /// Sub-class need to return true to tell IpcServerBase it needs 
            /// to shutdown server.
            ///
            /// @return true --> Shutdown server.
            ///
            virtual bool IsShutdownServer() = 0;
            
        private: // Used to notify observers
            void NotifyOpened(const char* pStrServerName);
            bool NotifyAccepted(IpcStreamer* pStreamer);
            void NotifyClosed();

        private:
            class PrivateImpl;
            PrivateImpl* m_pImpl;
    };
}; // namespace ipc
}; // namespace rtk

#endif
