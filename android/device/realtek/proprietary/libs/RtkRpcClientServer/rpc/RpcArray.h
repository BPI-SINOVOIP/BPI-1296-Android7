////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcArray
/// Remote Procedure Call - RPC Array<BR>
/// This class will help you to transfer a block of memory or array.  
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_ARRAY
#define INCLUDED_RPC_ARRAY

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
#include <assert.h>

namespace rtk {
namespace rpc {
    class RpcArray
    {
        public: // Exported functions
            /// Get array.
            /// 
            /// @param pBuf Pointer of the array.<BR>
            ///             When this function returned, pBuf will point to the array.
            ///
            /// @return Total size of array in bytes.
            ///
            template <class T> int GetArray(T*& pBuf) const 
            {
                pBuf = (T*)m_pArray;
                return m_iTotalSize; 
            };
            
            /// Get the counts of element in array.
            ///
            /// @return The counts of element in array.
            ///
            int GetElementCounts() const;

            /// Get total size in bytes.
            ///
            /// @return Total size in bytes.
            ///
            int GetTotalSizeInBytes() const;
    
        public: // Implementation of GetSize/Convert/Retrieve policy.
            int GetSize() const;
            int Convert(char* pBuffer, int iBufSize);
            int Retrieve(char* pBuffer, int iDataSize);

        public: // Constructor
            template <class T> 
            explicit RpcArray(T* pArray, int iTotalSizeInBytes): m_pArray((char*)pArray),
                                                                 m_iTotalSize(iTotalSizeInBytes)
            {
                assert(pArray != 0 && iTotalSizeInBytes > 0);
                typename GetSizePolicy<T>::Policy policy;
                m_iElemCounts = iTotalSizeInBytes / policy.GetSize(pArray[0]);
            }
            
            RpcArray();
            RpcArray(const RpcArray& rpcArray);
            
        private:
            friend bool operator==(const RpcArray& rpcArray1, const RpcArray& rpcArray2);
            
            int GetHeaderSize() const;
            
        private:
            char* m_pArray;
            int m_iElemCounts;
            int m_iTotalSize;
    };

    /// Create RpcArray object for specific memory block.
    ///
    /// @param The pointer of memory block
    /// @param iTotalSizeInBytes Total size of this memory block in bytes.
    ///
    /// @return RpcArray
    /// 
    template <class T> RpcArray RpcMakeArray(T* pArray, int iTotalSizeInBytes)
    {
        return RpcArray(pArray, iTotalSizeInBytes);
    };

    bool operator==(const RpcArray& rpcArray1, const RpcArray& rpcArray2);

    template <> struct ParamTypeTrait<RpcArray>
    {
        enum { TYPE = TYPE_ARRAY };
        typedef RpcArray RealType;
        typedef RpcArray Type;
    };

    class GetArrayLength
    {       
        public:
            int GetSize(const RpcArray& rpcArray) const 
            { 
                return rpcArray.GetSize();
            };
    };

    class ConvertArray
    {
        public:
            int ConvertToBytes(RpcArray& array, char* pBuffer, int iBufSize)
            {
                return array.Convert(pBuffer, iBufSize);
            };
    };

    class RetrieveArray
    {
        public:
            int RetrieveFromBytes(RpcArray& array, char* pBuffer, int iDataSize)
            {
                return array.Retrieve(pBuffer, iDataSize);
            }; 
    };

    template <> struct RetrievePolicy<RpcArray>
    {
        typedef RetrieveArray Policy;
    };

    template <> struct ConvertPolicy<RpcArray>
    {
        enum { DEFAULT_CONVERTER = YES };
        typedef ConvertArray Policy;
    };

    template <> struct GetSizePolicy<RpcArray>
    {
        typedef GetArrayLength Policy;
    };
}; // namespace rpc
}; // namespace rtk

#endif
