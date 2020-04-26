////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcCommandDemuxer
/// Remote Procedure Call - RPC Command Demuxer<BR>
/// <BR>
/// Use this class to retrieve function parameter information.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_COMMAND_DEMUXER
#define INCLUDED_RPC_COMMAND_DEMUXER

#include "RpcCommandHeader.h"
#include "RpcParameter.h"

namespace rtk {
namespace rpc {
    class RpcCommandDemuxer
    {
        public:
            /// Constructor
            ///
            /// @param pBuffer The buffer holds the muxed data.<BR>
            ///                The caller should provide a buffer which want to be demuxed.
            /// @param iDataLen The length of the muxed data.
            ///
            RpcCommandDemuxer(char* pBuffer, int iDataLen);
            
            ~RpcCommandDemuxer();
            
            /// Get a parameter.
            ///
            /// @param iIndex The parameter's index (zero-based).
            ///
            /// @return Parameter.
            ///
            template <typename T> typename ParamTypeTrait<T>::RealType Get(int iIndex);
            
            /// Get parameter's type
            ///
            /// @param iIndex The parameter's index (zero-based).
            ///
            /// @return Parameter types are defined in RpcType.h (enum RpcParamType).
            ///
            RpcParamType GetParamType(int iIndex);
            
            /// Check if parameters contain specific type.
            ///
            /// @param paramType Check if contain paramType
            ///
            /// @return true --> Contained specific type.
            ///
            bool HasParamType(RpcParamType paramType);
            
            /// Get command type.
            ///
            /// @return All of RPC command types are defined in RpcType.h (enum RpcCommandType)
            ///
            int GetCommandType() const;
            
            /// Get command counts
            ///
            /// @return Command counts
            ///
            int GetCommandCounts() const;
            
            /// Get counts pf specific command
            ///
            /// @param paramType Type needs to count.
            ///
            /// @return Command counts
            ///
            int GetCommandCounts(RpcParamType paramType) const;
            
            /// Check if the data in buffer is a valid command.
            ///
            /// @return true --> Valid command, false --> Invalid command.
            ///
            bool IsValidCommand() const;
            
            /// Get remote function name.
            ///
            const char* GetFunctionName();
            
            /// Get total size of this call
            ///
            /// @return Total size(in bytes) of this call
            //
            int GetSize() const;
            
            /// Get the unique ID for this call
            ///
            unsigned int GetId() const;
            
            /// Dump content
            ///
            void Dump();

        private:
            char* m_pBuffer;
            int m_iDataLen;
    };

    template <typename T> typename ParamTypeTrait<T>::RealType RpcCommandDemuxer::Get(int iIndex)
    {
        if (iIndex >= GetCommandCounts() || iIndex < 0) return typename ParamTypeTrait<T>::RealType();

        RpcCommandHeader* pCmdHeader = (RpcCommandHeader*)m_pBuffer;
        int iTotalLen = pCmdHeader->iTotalLen;
        int iCmdCounts = pCmdHeader->iParamCounts;
        int iLen = iTotalLen - sizeof(RpcCommandHeader);
        int i = 0;
        char* pBuf = m_pBuffer + sizeof(RpcCommandHeader);
        while(iCmdCounts > 0 && iLen > 0)
        {
            RpcParamHeader* pParamHeader = (RpcParamHeader*)pBuf;
            if (i == iIndex)
            {
                RpcParameter<typename ParamTypeTrait<T>::RealType> rpcParam;
                int iRet = rpcParam.RetrieveFromBytes(pBuf + sizeof(RpcParamHeader), 
                                                      iLen - sizeof(RpcParamHeader));
                return iRet != rpcParam.GetSize() 
                            ? typename ParamTypeTrait<T>::RealType()
                            : rpcParam.GetValue();
            }
            
            i++;
            int iOffset = sizeof(RpcParamHeader) + pParamHeader->iRpcParamLen;
            iLen -= iOffset;
            pBuf += iOffset;
            iCmdCounts--;
        }
        return typename ParamTypeTrait<T>::RealType();
    };
}; // namespace rpc
}; // namespace rtk

#endif
