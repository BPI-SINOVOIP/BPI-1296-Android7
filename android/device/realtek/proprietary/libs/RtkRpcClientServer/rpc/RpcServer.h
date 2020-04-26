////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcServer
/// Remote Procedure Call - Server Side
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_SERVER
#define INCLUDED_RPC_SERVER

namespace rtk {
namespace rpc {
    class RpcServer
    {
        public:
            RpcServer();
            ~RpcServer();
            
            /// Start a RPC server with a specific server name.
            ///
            /// @param pStrServerName This server name should be an unique name 
            ///                        in the system, otherwise, your request
            ///                        will be rejected.
            /// @param pParam The parameter pass to the implementation of server.
            ///
            /// @return true --> Success. false --> Failed.
            ///
            bool StartServer(const char* pStrServerName, void* pParam);
            
            /// Stop a specific RPC server.
            ///
            /// @param pStrServerName Server name which you want to stop.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            bool StopServer();
            
            /// Get current server name.
            ///
            /// @return Server name.
            ///
            const char* GetServerName() const;
            
        private:
            class PrivateImpl;
            PrivateImpl* m_pImpl;
    };
}; // namespace rpc
}; // namespace rtk

#endif
