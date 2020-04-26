////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcServerManager 
/// Inter-Process Communication Server Manager
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_SERVER_MANAGER
#define INCLUDED_IPC_SERVER_MANAGER

namespace rtk {
namespace ipc {
    class IpcServerObserver;
    class IpcServerManager
    {
        public:
            /// Constants
            enum 
            { 
                TIME_OUT_INFINITY = -1 ///< Time out infinity
            };
            
        public:
            static IpcServerManager& GetInstance();
            
            /// Start a IPC server with a specific server name.
            ///
            /// @param pStrServerName This server name should be an unique name 
            ///                       in the system, otherwise, your request
            ///                       will be rejected.
            /// @param iIpcImplMode IpcManager will choose an implementation of IPC 
            ///                     depended on the mode you selected.
            ///                     The modes are define in IpcMode.h
            /// @param pParam The parameter pass to the implementation of server.
            ///
            /// @return true --> Success. false --> Failed.
            ///
            bool StartServer(const char* pStrServerName, int iIpcImplMode, void* pParam);
            
            /// Stop a specific IPC server.
            ///
            /// @param pStrServerName Server name which you want to stop.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            bool StopServer(const char* pStrServerName);

            /// Stop all of existed servers.<BR>
            /// When this function returned, all of servers are stopped.
            ///
            void StopAllServers();

            /// Wait for all of servers to be stopped(with a timeout).
            ///
            /// @param iTimeout Time out value(in milli-seconds) or 
            ///                 IpcManager::TIME_OUT_INFINITY to wait forever.
            ///
            void WaitForAllServerStop(int iTimeout);
            
            /// Register a server observer.
            ///
            /// @param pStrServerName The name of the server which wants to be monitored.
            /// @param pObs Point to a instance of observer.
            ///
            /// @return true --> Success.
            ///
            bool RegisterObserver(const char* pStrServerName, IpcServerObserver* pObs);
            
            /// Unregister a server observer.
            ///
            /// @param pStrServerName The name of the server which wants to be monitored.
            /// @param pObs Point to a instance of observer.
            ///
            /// @return true --> Success.
            ///
            void UnregisterObserver(const char* pStrServerName, IpcServerObserver* pObs);
            
        private:
            IpcServerManager();
            IpcServerManager(const IpcServerManager&);
            IpcServerManager& operator=(const IpcServerManager&);
            ~IpcServerManager();
            
        private:
            struct PrivateImpl;
            PrivateImpl* m_pImpl;
    };
}; // namespace ipc
}; // namespace rtk

#endif
