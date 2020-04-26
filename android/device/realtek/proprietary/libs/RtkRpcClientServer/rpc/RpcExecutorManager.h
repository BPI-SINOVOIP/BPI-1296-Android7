////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcExecutorManager
/// Remote Procedure Call - RPC Executor Manager<BR>
/// <BR>
/// Manage PRC executor
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_EXECUTOR_MANAGER
#define INCLUDED_RPC_EXECUTOR_MANAGER

namespace rtk {
namespace rpc {
    class RpcExecutor;
    class RpcCommandMuxer;
    class RpcCommandDemuxer;
    class RpcCallback;
    
    class RpcExecutorManager
    {
        public:
            /// Error code.
            enum ErrCode
            {
                ERR_FAILED = -1, ///< Failed.
            };
            
        public:
            /// Accessor for this class.
            static RpcExecutorManager& GetInstance();
            
            /// Register an executor
            ///
            /// @param pExecutor The executor wants to be registered.
            ///
            /// @return true --> Success.
            ///
            bool RegisterExecutor(RpcExecutor* pExecutor);
            
            /// Unregister an executor
            ///
            /// @param pExecutor The executor wants to be registered.
            ///
            /// @return true --> Success.
            ///
            bool UnregisterExecutor(RpcExecutor* pExecutor);
            
            /// Execute a command  based on command object RpcCommandDemuxer
            ///
            /// @param pCmdDemuxer The command object contained remote command.
            /// @param pRpcCallback The callback object for this remote command.
            /// @param pBuffer The buffer for holding the returned value.
            /// @param iBufSize The size (in bytes) of pBuffer.
            ///
            /// @return > 0: The length (in bytes) of returned value.<BR>
            ///         < 0: Error code. 
            /// 
            int Execute(RpcCommandDemuxer* pCmdDemuxer,
                        RpcCallback* pRpcCallback,
                        char* pBuffer, int iBufSize);

        private:
            struct PrivateImpl;
            PrivateImpl* m_pImpl;

        private:
            RpcExecutorManager();
            ~RpcExecutorManager();
            RpcExecutorManager(const RpcExecutorManager&);
            RpcExecutorManager& operator=(const RpcExecutorManager&);
    };

}; // namespace rpc
}; // namespace rtk

#endif
