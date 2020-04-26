////////////////////////////////////////////////////////////////////////////////
/// Remote Procedure Call - Command header<BR>
/// <BR>
/// Each transaction will contain one header.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_COMMAND_HEADER
#define INCLUDED_RPC_COMMAND_HEADER

namespace rtk {
namespace rpc {
    /// Each transaction contains:<BR>
    /// [RpcCommandHeader][RpcParamHeader + Parameter][RpcParamHeader + Parameter]......

    struct RpcCommandHeader
    {
        /// Define the command type of this transaction.
        /// All of RPC command types are defined in RpcType.h (enum RpcCommandType)
        ///
        unsigned int iRpcCommandType;
        
        /// A unique ID to present this call.
        ///
        unsigned int iId;
        
        /// The length of all of data of this transaction.
        /// This length is including the size of this header.
        ///
        unsigned int iTotalLen;
        
        /// The counts of parameters.
        ///
        unsigned int iParamCounts;
    };
    
    struct RpcParamHeader
    {
        /// Define the type of this parameter.
        /// All of RPC parameter types are defined in RpcType.h (enum RpcParamType)
        ///
        unsigned int iRpcParamType;
        
        /// The length of this parameter cascaded with this header.
        /// The length of this header is not included.
        ///
        unsigned int iRpcParamLen;
    };
}; // namespace rpc
}; // namespace rtk

#endif
