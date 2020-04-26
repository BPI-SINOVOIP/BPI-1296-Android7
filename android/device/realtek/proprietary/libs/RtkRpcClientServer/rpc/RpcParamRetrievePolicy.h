////////////////////////////////////////////////////////////////////////////////
/// Remote Procedure Call - Parameter Retrieve Policy <BR>
/// <BR>
/// Used for retrieving parameter from bytes.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_PARAMETER_RETRIEVE_POLICY
#define INCLUDED_RPC_PARAMETER_RETRIEVE_POLICY

#ifndef INCLUDED_RPC_PARAMETER_GET_SIZE_POLICY
    #include "RpcParamGetSizePolicy.h"
#endif
#ifndef INCLUDED_RPC_TYPE
    #include "RpcType.h"
#endif

namespace rtk {
namespace rpc {
    ////////////////////////////////////////////////////////////////////////////
    // Retrieve Pilocy Implementation
    ////////////////////////////////////////////////////////////////////////////
    template <typename T> class RetrieveGeneric
    {
        // Generic implementation. Just using memcpy()
        
        public:
            int RetrieveFromBytes(T& value, char* pBuffer, int iDataSize)
            {
                typename GetSizePolicy<T>::Policy getSizePolicy;
                int iTypeSize = getSizePolicy.GetSize(value);
                if (iTypeSize > iDataSize) return 0;
                
                memcpy(&value, pBuffer, iTypeSize);
                return iTypeSize;
            };
    };
    
    class RetrieveString
    {
        public:
            int RetrieveFromBytes(const char*& value, char* pBuffer, int iDataSize)
            {
                if (!pBuffer || iDataSize <= 0) return 0;
                
                int i = 0;
                for (; i < iDataSize; i++) // Check if this is a valid string.
                    if (!pBuffer[i]) break;
                if (i == iDataSize) return 0;

                value = pBuffer; // Warning! We just copy pointer not content.
                return i + 1; // Return the string length + 1 '\0'
            }; 
    };
    
    template <typename T> class RetrieveNotSupport
    {
        public:
            int RetrieveFromBytes(T& value, char* pBuffer, int iDataSize)
            {
                assert("WARNING: POINTER NOT SUPPORT" == 0); 
                return 0;
            }; 
    };
    
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    // Trait for getting "retrieve policy"
    ////////////////////////////////////////////////////////////////////////////
    template <typename T> struct RetrievePolicy
    {
        typedef RetrieveGeneric<T> Policy;
    };

    template <> struct RetrievePolicy<const char*>
    {
        typedef RetrieveString Policy;
    };

    template <typename T> struct RetrieveNotSupport<T*>
    {
        typedef RetrieveNotSupport<T*> Policy;
    };

    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Helper macro for defining "retrieving parameter policy"
    ////////////////////////////////////////////////////////////////////////////
    #define DECL_RETRIEVE_PARAM_POLICY(newType, newTypePolicy)\
        namespace rtk {\
        namespace rpc {\
            template <> struct RetrievePolicy<newType>\
            {\
                typedef newTypePolicy Policy;\
            };\
        };\
        };
    
    ////////////////////////////////////////////////////////////////////////////
}; // namespace rpc
}; // namespace rtk

#endif

