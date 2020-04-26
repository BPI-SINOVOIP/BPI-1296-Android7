////////////////////////////////////////////////////////////////////////////////
// Remote Procedure Call - Parameter Get Size Policy<BR>
// <BR>
// Used for getting parameter's size.<BR>
//
// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_PARAMETER_GET_SIZE_POLICY
#define INCLUDED_RPC_PARAMETER_GET_SIZE_POLICY

#include <string.h>
#include <assert.h>
#ifndef INCLUDED_RPC_TYPE
    #include "RpcType.h"
#endif

namespace rtk {
namespace rpc {
    ////////////////////////////////////////////////////////////////////////////
    // Get Size Pilocy Implementation
    ////////////////////////////////////////////////////////////////////////////
    template <typename T> class GetSizeGeneric
    {
        // Generic implementation. Just using sizeof()
        
        public:
            int GetSize(const T&) const { return sizeof(T); };
    };
    
    class GetStringLength
    {       
        public:
            int GetSize(const char* pStr) const 
            { 
                return (pStr ? strlen(pStr) : 0) + 1; //  Include '\0'
            };
    };
    
    template <typename T> class GetSizeNotSupport
    {
        public:
            int GetSize(T) const 
            { 
                assert("WARNING: POINTER NOT SUPPORT" == 0);
                return 0;
            };
    };
    
    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    // Trait for getting "get size policy"
    ////////////////////////////////////////////////////////////////////////////
    template <typename T> struct GetSizePolicy
    {
        typedef GetSizeGeneric<T> Policy;
    };
    
    template <> struct GetSizePolicy<const char*>
    {
        typedef GetStringLength Policy;
    };
    
    template <typename T> struct GetSizePolicy<T*>
    {
        typedef GetSizeNotSupport<T*> Policy;
    };
    
    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Helper macro for defining "get size policy"
    ////////////////////////////////////////////////////////////////////////////
    #define DECL_GET_SIZE_POLICY(newType, newTypePolicy)\
        namespace rtk {\
        namespace rpc {\
            template <> struct GetSizePolicy<newType>\
            {\
                typedef newTypePolicy Policy;\
            };\
        };\
        };
    
    ////////////////////////////////////////////////////////////////////////////
}; // namespace rpc
}; // namespace rtk

#endif

