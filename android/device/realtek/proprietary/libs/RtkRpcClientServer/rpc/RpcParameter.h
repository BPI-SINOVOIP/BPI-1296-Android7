////////////////////////////////////////////////////////////////////////////////
/// Remote Procedure Call - Parameter <BR>
/// <BR>
/// Using this template to convert a parameter to byte stream.<BR>
/// Using this template to retrieve a parameter from byte stream.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_PARAMETER
#define INCLUDED_RPC_PARAMETER

#ifndef INCLUDED_RPC_PARAMETER_GET_SIZE_POLICY
    #include "RpcParamGetSizePolicy.h"
#endif
#ifndef INCLUDED_RPC_PARAMETER_CONVERT_POLICY
    #include "RpcParamConvertPolicy.h"
#endif
#ifndef INCLUDED_RPC_PARAMETER_RETRIEVE_POLICY
    #include "RpcParamRetrievePolicy.h"
#endif
#ifndef INCLUDED_RPC_TYPE
    #include "RpcType.h"
#endif

namespace rtk {
namespace rpc {
    /// Dummy is the C++ template dummy trick.
    /// Dummy is not used in code, just used for selecting different 
    /// implementation in compile time.
    template <class T, class Dummy=void, int DefaultConverterFlag=YES> class RpcParameter;
    
    template <class T, class Dummy> class RpcParameter<T, Dummy, YES>
    {
        public:
            typedef T Type; ///< You can get the type by calling RpcParameter::Type
            
            RpcParameter(): m_Param() { ; };
            RpcParameter(const T& param) { m_Param = param; };
            template <class T2> RpcParameter(const T2& param) { m_Param = param; };
            
            /// Convert this parameter to bytes
            ///
            /// @param pBuffer The buffer to hold the converting result.
            /// @param iBufSize The size of the buffer.
            ///
            /// @return Return the number of converted bytes.
            ///
            int ConvertToBytes(char* pBuffer, int iBufSize)
            {
                typename ConvertPolicy<T>::Policy policy;
                return policy.ConvertToBytes(m_Param, pBuffer, iBufSize);
            };
            
            /// Retrieve parameter from byte stream.
            ///
            /// @param pBuffer The buffer contained bytes needs to be retrieve.
            /// @param iDataSize The size of data.
            ///
            /// @return Bytes number retrieved from byte stream.
            ///
            int RetrieveFromBytes(char* pBuffer, int iDataSize)
            {
                typename RetrievePolicy<T>::Policy policy;
                return policy.RetrieveFromBytes(m_Param, pBuffer, iDataSize);
            };
            
            /// Get the size of this parameter
            ///
            /// @return The size of this parameter
            ///
            int GetSize() const
            {
                typename GetSizePolicy<T>::Policy policy;
                return policy.GetSize(GetValue());
            };
            
            /// Get actaul value of this parameter.
            ///
            /// @return The actaul value of this parameter
            ///
            const T& GetValue() const { return m_Param; };
            
        private:
            T m_Param;
    };
    
    template <class T, class Dummy> class RpcParameter<T, Dummy, NO>
    {
        public:
            typedef T Type; ///< You can get the type by calling RpcParameter::Type
            
            RpcParameter(): m_Param() { ; };
            RpcParameter(const T& param) { m_Param = param; };
            template <class T2> RpcParameter(const T2& param) { m_Param = param; };
            
            /// Convert this parameter to bytes
            ///
            /// @param pBuffer The buffer to hold the converting result.
            /// @param iBufSize The size of the buffer.
            ///
            /// @return Return the number of converted bytes.
            ///
            int ConvertToBytes(char* pBuffer, int iBufSize)
            {
                // Dummy is the C++ template dummy trick.
                // Dummy is not used in code, just used for selecting different 
                // implementation in compile time.
                typename ConvertPolicy<T, Dummy>::Policy policy;
                return policy.ConvertToBytes(m_Param, pBuffer, iBufSize);
            };
            
            /// Retrieve parameter from byte stream.
            ///
            /// @param pBuffer The buffer contained bytes needs to be retrieve.
            /// @param iDataSize The size of data.
            ///
            /// @return Bytes number retrieved from byte stream.
            ///
            int RetrieveFromBytes(char* pBuffer, int iDataSize)
            {
                typename RetrievePolicy<T>::Policy policy;
                return policy.RetrieveFromBytes(m_Param, pBuffer, iDataSize);
            };
            
            /// Get the size of this parameter
            ///
            /// @return The size of this parameter
            ///
            int GetSize() const
            {
                typename GetSizePolicy<T>::Policy policy;
                return policy.GetSize(GetValue());
            };
            
            /// Get actaul value of this parameter.
            ///
            /// @return The actaul value of this parameter
            ///
            const T& GetValue() const { return m_Param; };
            
        private:
            T m_Param;
    };
}; // namespace rpc
}; // namespace rtk

#endif
