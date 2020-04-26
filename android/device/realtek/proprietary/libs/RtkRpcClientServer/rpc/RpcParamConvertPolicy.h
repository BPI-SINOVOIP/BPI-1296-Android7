////////////////////////////////////////////////////////////////////////////////
/// Remote Procedure Call - Parameter Convert Policy<BR>
/// <BR>
/// Used for converting parameter to bytes.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_PARAMETER_CONVERT_POLICY
#define INCLUDED_RPC_PARAMETER_CONVERT_POLICY

#ifndef INCLUDED_RPC_PARAMETER_GET_SIZE_POLICY
    #include "RpcParamGetSizePolicy.h"
#endif
#ifndef INCLUDED_RPC_TYPE
    #include "RpcType.h"
#endif

namespace rtk {
namespace rpc {
    ////////////////////////////////////////////////////////////////////////////
    // Convert Pilocy Implementation
    ////////////////////////////////////////////////////////////////////////////
    template <typename T> class ConvertGeneric
    {
        // Generic implementation. Just using memcpy()
        
        public:
            int ConvertToBytes(T& value, char* pBuffer, int iBufSize)
            {
                typename GetSizePolicy<T>::Policy getSizePolicy;
                int iTypeSize = getSizePolicy.GetSize(value);
                if (iTypeSize > iBufSize) return 0;
                
                memcpy(pBuffer, &value, iTypeSize);
                return iTypeSize;
            };
    };
    
    class ConvertString
    {
        public:
            int ConvertToBytes(const char* value, char* pBuffer, int iBufSize)
            {
                int iStrLen = value ? strlen(value) : 0;
                if (iStrLen >= iBufSize) return 0;
                if (iStrLen > 0) strcpy(pBuffer, value);
                pBuffer[iStrLen] = 0;
                return iStrLen + 1; // Including '\0'
            };
    };
    
    template <typename T> class ConvertNotSupport
    {
        public:
            int ConvertToBytes(T& value, char* pBuffer, int iBufSize)
            {
                assert("WARNING: POINTER NOT SUPPORT" == 0);
                return 0;
            };
    };
    
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    // Trait for getting "convert policy"
    ////////////////////////////////////////////////////////////////////////////
    /// Dummy is the C++ template dummy trick.
    /// Dummy is not used in code, just used for selecting different 
    /// implementation in compile time.
    template <typename T, typename Dummy=T> struct ConvertPolicy
    {
        /// By default, we use default converter.
        /// If DEFAULT_CONVERTER = NO, we can use Dummy to select the other 
        /// implementation.
        enum { DEFAULT_CONVERTER = YES };
        typedef ConvertGeneric<T> Policy;
    };
    
    template <> struct ConvertPolicy<const char*>
    {
        enum { DEFAULT_CONVERTER = YES };
        typedef ConvertString Policy;
    };
    
    template <typename T> struct ConvertPolicy<T*>
    {
        enum { DEFAULT_CONVERTER = YES };
        typedef ConvertNotSupport<T*> Policy;
    };

    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    // Helper macro for defining "converting parameter policy"
    ////////////////////////////////////////////////////////////////////////////
    #define DECL_CONVERT_PARAM_POLICY(newType, newTypePolicy)\
        namespace rtk {\
        namespace rpc {\
            template <> struct ConvertPolicy<newType>\
            {\
                enum { DEFAULT_CONVERTER = YES };\
                typedef newTypePolicy Policy;\
            };\
        };\
        };
        
    ////////////////////////////////////////////////////////////////////////////
}; // namespace rpc
}; // namespace rtk

#endif

