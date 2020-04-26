////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcCommandMuxer
/// Remote Procedure Call - RPC Command Muxer<BR>
/// <BR>
/// Use this class to wrap information for remote functions
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_COMMAND_MUXER
#define INCLUDED_RPC_COMMAND_MUXER

#include "rpc/RpcCommandHeader.h"
#include "rpc/RpcParameter.h"

namespace rtk {
namespace rpc {
    class RpcCommandMuxer
    {
        public:
            /// Constructor
            ///
            /// @param pBuffer The buffer holds the muxed result.<BR>
            ///                The caller should provide a buffer to hold the muxed result.
            /// @param iBufSize The size of the buffer.
            ///
            RpcCommandMuxer(char* pBuffer, int iBufSize);
            
            ~RpcCommandMuxer();
            
            /// Push a parameter.
            /// 
            /// @param param The parameter which pass to remote side.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            template <typename T> bool Push(T param);
            
            /// Finish pushing parameter.
            /// Caller should call this function to let RpcCommandMuxer
            /// to do some finalization stuffs.
            ///
            /// @param pStrFuncName Remote function name.
            /// @param iId A unique ID to present this call
            ///
            /// @return Total size of command.
            ///
            int Finish(const char* pStrFuncName) { return Finish(TYPE_INVOKE, pStrFuncName, 0); };
            int Finish(const char* pStrFuncName, unsigned int iId) { return Finish(TYPE_INVOKE, pStrFuncName, iId); };
            int Finish(int iCmdType, unsigned int iId) { return Finish(iCmdType, 0, iId); };
            int Finish(unsigned int iId) { return Finish(TYPE_RESULT, 0, iId); };
            int Finish() { return Finish(TYPE_RESULT, 0, 0); };
            
            /// Check if this is a valid command
            ///
            /// @return true --> Valid command, false --> Invalid command.
            ///
            bool IsValidCommand() const;
            
            /// Get the muxed bytes
            ///
            /// @return Return the buffer pointer which hold the muxed data
            ///
            char* GetBytes() const { return m_pBuffer; };
            
            /// Get the size of muxed data.
            ///
            /// @return The size of muxed data (in bytes).
            ///
            int GetSize() const { return m_iDataSize; };

        private:
            char* m_pBuffer;
            int m_iBufSize;
            int m_iOffset;
            int m_iCounts;
            int m_iDataSize;
            
        private:
            RpcCommandMuxer();
            
        private:
            void Reset();
            int Finish(int iCmdType, const char* pStrFuncName, unsigned int iId);
    };

    template <typename T> bool RpcCommandMuxer::Push(T param)
    {
        if (!m_iOffset)
        {
            // Reserve space for command header.
            m_iOffset += sizeof(RpcCommandHeader);
            m_iDataSize = 0;
        }
        
        typedef typename ParamTypeTrait<T>::RealType RealType;
        RpcParameter<RealType, T, ConvertPolicy<RealType, T>::DEFAULT_CONVERTER> rpcParam(param);
        
        int iBufOffset = m_iOffset;
        int iRestBufSize = m_iBufSize - m_iOffset;
        int iTotalParamSize = rpcParam.GetSize() + sizeof(RpcParamHeader);
        if (iRestBufSize < iTotalParamSize)
        {
            Reset();
            return false;
        }
        
        RpcParamHeader paramHeader = { ParamTypeTrait<T>::TYPE, (unsigned int)rpcParam.GetSize() };
        memcpy(m_pBuffer + iBufOffset, &paramHeader, sizeof(RpcParamHeader));
        iBufOffset += sizeof(RpcParamHeader);
        iRestBufSize -= sizeof(RpcParamHeader);
        
        int iRet = rpcParam.ConvertToBytes(m_pBuffer + iBufOffset, iRestBufSize);
        if (iRet <= 0 || iRet != rpcParam.GetSize()) 
        {
            Reset();
            return false;
        }
        
        m_iOffset += iTotalParamSize;
        m_iCounts++;
        return true;
    };
}; // namespace rpc
}; // namespace rtk

#endif
