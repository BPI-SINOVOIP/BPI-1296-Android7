////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcExecutor
/// Remote Procedure Call - RPC Executor Interface<BR>
/// <BR>
/// This interface is used to execute the local function based on 
/// remote request.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_EXECUTOR
#define INCLUDED_RPC_EXECUTOR

namespace rtk {
namespace rpc {
    class RpcCommandMuxer;
    class RpcCommandDemuxer;
    class RpcCallback;
    
    class RpcExecutor
    {
        public:
            /// Excute a local function call based on RpcCommandDemuxer.<BR>
            /// The returned result of local function call is wrapped by RpcCommandMuxer.
            ///
            /// @param pCmdDemuxer Contain the parameters passed to local function.
            /// @param pRpcCallback A callback object to descript remote callback,
            ///                     please see the header file of RpcCallback for details.
            ///
            /// @return Returned result.
            ///
            virtual RpcCommandMuxer* Execute(RpcCommandDemuxer* pCmdDemuxer,
                                             RpcCallback* pRpcCallback) = 0;
    
        public:
            virtual ~RpcExecutor() { ; };
    };

}; // namespace rpc
}; // namespace rtk

#endif
