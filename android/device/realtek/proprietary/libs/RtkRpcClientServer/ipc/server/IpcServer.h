////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcServer 
/// Inter-Process Communication Server Side Interface
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_SERVER
#define INCLUDED_IPC_SERVER

namespace rtk {
namespace ipc {
    class IpcServerObserver;
    class IpcServer
    {
        public:
            /// Start server.
            ///
            /// @param pStrServerName Server name. This name should be unique.
            /// @param pParam The parameter pass to the implementation of server.
            ///
            /// @return true --> Success.
            ///
            virtual bool Start(const char* pStrServerName, void* pParam) = 0;
            
            /// Stop server.
            ///
            virtual bool Stop() = 0;
            
            /// Register a server observer.
            ///
            /// @param pObs Point to a instance of observer.
            ///
            /// @return true --> Success.
            ///
            virtual bool RegisterObserver(IpcServerObserver* pObs) = 0;
            
            /// Unregister a server observer.
            ///
            /// @param pObs Point to a instance of observer.
            ///
            /// @return true --> Success.
            ///
            virtual void UnregisterObserver(IpcServerObserver* pObs) = 0;

            /// Query server name.
            ///
            /// @return Server name.
            ///            
            virtual const char* GetServerName() = 0;

            /// Query the parameter passed to Start()
            ///
            /// @return The parameter passed to Start()
            ///
            virtual void* GetParameter() = 0;
    
        public:
            virtual ~IpcServer() { ; };
    };
}; // namespace ipc
}; // namespace rtk

#endif
