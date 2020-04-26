////////////////////////////////////////////////////////////////////////////////
/// \file RpcCallbackDecl.h
/// \brief Remote Procedure Call - RPC Callback Declaration For Client Side<BR>
///
/// <BR>
/// These macros can help you to create your own RPC callback (for client side).<BR>
/// The max counts of parameters of RPC callback function is up to 5.<BR>
/// <BR>
/// The rule of using these macros:
/// <OL>
///     <LI>Please include this header file(RpcCallbackDecl.h) in your *.cpp.<BR>
///         Don't include it in your *.h.<BR>
///         Including this header file in your *.h maybe increase the dependency of
///         your module.<BR>
///         Please don't do it. Please.</LI>
///     <LI>If you need to know what type of parameters which are supported.<BR>
///         Please refer to rpc/RpcType.h for details.</LI>
///     <LI>Don't use these macros in any RpcXXX.h or RpcXXX.cpp to extend your 
///         callback function. You should not add any codes in origianl RPC module.</LI>
///     <LI>Don't use these macros in your namespace. Use them in global namespace.</LI>
///     <LI>Don't use these macros in *.c</LI>
///     <LI>Don't use these macros in extern "C" { ..... }</LI>
/// </OL>
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_CALLBACK_DECLARATION
#define INCLUDED_RPC_CALLBACK_DECLARATION

#ifndef INCLUDED_RPC_TYPE
    #include "RpcType.h"
#endif
#ifndef INCLUDED_RPC_PARAMETER_GET_SIZE_POLICY
    #include "RpcParamGetSizePolicy.h"
#endif
#ifndef INCLUDED_RPC_PARAMETER_CONVERT_POLICY
    #include "RpcParamConvertPolicy.h"
#endif
#ifndef INCLUDED_RPC_PARAMETER_RETRIEVE_POLICY
    #include "RpcParamRetrievePolicy.h"
#endif
#ifndef INCLUDED_RPC_COMMAND_DEMUXER
    #include "RpcCommandDemuxer.h"
#endif

namespace rtk {
namespace rpc {
    /// Callback Function Proxy Function Pointer Prototype
    typedef void (*fpCallbackProxy)(void*, RpcCommandDemuxer&);
    
    ////////////////////////////////////////////////////////////////////////////
    /// RPC Callback context.
    /// For modulizing RPC module, we need a way to fit any kinds of callback function prototype.
    /// So I delegate the real executing stuff to callback proxy.
    /// Callback proxy is one of Callback1Param, Callback2Param, Callback3Param, Callback4Param and
    /// Callback5Param template defined in below.
    ////////////////////////////////////////////////////////////////////////////
    struct RpcCallbackContext
    {
        void* pRealCallbackFunction;   ///< The original callback function assigned by user.
        fpCallbackProxy CallbackProxy; ///< The callback proxy for executing original callback function.
        
        RpcCallbackContext(): pRealCallbackFunction(0), CallbackProxy(0)
        {
        }
        
        template <class T> RpcCallbackContext& operator=(const T& callback)
        {
            pRealCallbackFunction = (void*)callback;
            return *this;
        }
    };

    ////////////////////////////////////////////////////////////////////////////
    

    ////////////////////////////////////////////////////////////////////////////
    // Callback Proxy Implementation
    ////////////////////////////////////////////////////////////////////////////
    template <class Fn, class T1> 
    inline void Callback1Param(void* fpRealCallback, RpcCommandDemuxer& demuxer)
    {
        // We start from index 1, since index 0 is the index of callback function.
        T1 param1 = demuxer.Get<T1>(1);
        Fn fp = (Fn)fpRealCallback;
        fp(param1);
    };
    
    template <class Fn, class T1, class T2> 
    inline void Callback2Param(void* fpRealCallback, RpcCommandDemuxer& demuxer)
    {
        T1 param1 = demuxer.Get<T1>(1);
        T2 param2 = demuxer.Get<T2>(2);
        Fn fp = (Fn)fpRealCallback;
        fp(param1, param2);
    };
    
    template <class Fn, class T1, class T2, class T3> 
    inline void Callback3Param(void* fpRealCallback, RpcCommandDemuxer& demuxer)
    {
        T1 param1 = demuxer.Get<T1>(1);
        T2 param2 = demuxer.Get<T2>(2);
        T3 param3 = demuxer.Get<T3>(3);
        Fn fp = (Fn)fpRealCallback;
        fp(param1, param2, param3);
    };
    
    template <class Fn, class T1, class T2, class T3, class T4> 
    inline void Callback4Param(void* fpRealCallback, RpcCommandDemuxer& demuxer)
    {
        T1 param1 = demuxer.Get<T1>(1);
        T2 param2 = demuxer.Get<T2>(2);
        T3 param3 = demuxer.Get<T3>(3);
        T4 param4 = demuxer.Get<T4>(4);
        Fn fp = (Fn)fpRealCallback;
        fp(param1, param2, param3, param4);
    };
    
    template <class Fn, class T1, class T2, class T3, class T4, class T5> 
    inline void Callback5Param(void* fpRealCallback, RpcCommandDemuxer& demuxer)
    {
        T1 param1 = demuxer.Get<T1>(1);
        T2 param2 = demuxer.Get<T2>(2);
        T3 param3 = demuxer.Get<T3>(3);
        T4 param4 = demuxer.Get<T4>(4);
        T5 param5 = demuxer.Get<T4>(5);
        Fn fp = (Fn)fpRealCallback;
        fp(param1, param2, param3, param4, param5);
    };
    
    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Convert Pilocy Implementation For Callback
    ////////////////////////////////////////////////////////////////////////////
    template <typename T, typename Fn, Fn CallbackProxy> class ConvertForCallback
    {
        public:
            int ConvertToBytes(T& value, char* pBuffer, int iBufSize)
            {
                typename GetSizePolicy<T>::Policy getSizePolicy;
                int iTypeSize = getSizePolicy.GetSize(value);
                if (iTypeSize > iBufSize) return 0;

                value.CallbackProxy = CallbackProxy;
                memcpy(pBuffer, &value, iTypeSize);
                return iTypeSize;
            };
    };
    
    ////////////////////////////////////////////////////////////////////////////
};
};

////////////////////////////////////////////////////////////////////////////////
/// Declare callback function with 1 parameter
///
/// @param callbackName The callback name. Just pick a name which you like.
/// @param param1 The type of parameter of callback function. For example: int, char.....
////////////////////////////////////////////////////////////////////////////////
#define DECLARE_RPC_CALLBACK_1_PARAM(callbackName, param1) \
    namespace rtk { \
    namespace rpc { \
        typedef void (*callbackName)(param1); \
        template <> struct ParamTypeTrait<callbackName>\
        {\
            enum { TYPE = TYPE_CALLBACK }; \
            typedef RpcCallbackContext RealType;\
        };\
        template <> struct ConvertPolicy<RpcCallbackContext, callbackName>\
        {\
            enum { DEFAULT_CONVERTER = NO };\
            typedef ConvertForCallback<RpcCallbackContext, fpCallbackProxy, Callback1Param<callbackName, param1> > Policy;\
        };\
    };\
    };
    
////////////////////////////////////////////////////////////////////////////////

    
////////////////////////////////////////////////////////////////////////////////
/// Declare callback function with 2 parameters
///
/// @param callbackName The callback name. Just pick a name which you like.
/// @param param1 The type of parameter of callback function. For example: int, char.....
/// @param param2 The type of parameter of callback function. For example: int, char.....
////////////////////////////////////////////////////////////////////////////////
#define DECLARE_RPC_CALLBACK_2_PARAM(callbackName, param1, param2) \
    namespace rtk { \
    namespace rpc { \
        typedef void (*callbackName)(param1, param2); \
        template <> struct ParamTypeTrait<callbackName>\
        {\
            enum { TYPE = TYPE_CALLBACK }; \
            typedef RpcCallbackContext RealType;\
        };\
        template <> struct ConvertPolicy<RpcCallbackContext, callbackName>\
        {\
            enum { DEFAULT_CONVERTER = NO };\
            typedef ConvertForCallback<RpcCallbackContext, fpCallbackProxy, Callback2Param<callbackName, param1, param2> > Policy;\
        };\
    };\
    };

////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// Declare callback function with 3 parameters
///
/// @param callbackName The callback name. Just pick a name which you like.
/// @param param1 The type of parameter of callback function. For example: int, char.....
/// @param param2 The type of parameter of callback function. For example: int, char.....
/// @param param3 The type of parameter of callback function. For example: int, char.....
////////////////////////////////////////////////////////////////////////////////
#define DECLARE_RPC_CALLBACK_3_PARAM(callbackName, param1, param2, param3) \
    namespace rtk { \
    namespace rpc { \
        typedef void (*callbackName)(param1, param2, param3); \
        template <> struct ParamTypeTrait<callbackName>\
        {\
            enum { TYPE = TYPE_CALLBACK }; \
            typedef RpcCallbackContext RealType;\
        };\
        template <> struct ConvertPolicy<RpcCallbackContext, callbackName>\
        {\
            enum { DEFAULT_CONVERTER = NO };\
            typedef ConvertForCallback<RpcCallbackContext, fpCallbackProxy, Callback3Param<callbackName, param1, param2, param3> > Policy;\
        };\
    };\
    };
    
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// Declare callback function with 4 parameters
///
/// @param callbackName The callback name. Just pick a name which you like.
/// @param param1 The type of parameter of callback function. For example: int, char.....
/// @param param2 The type of parameter of callback function. For example: int, char.....
/// @param param3 The type of parameter of callback function. For example: int, char.....
/// @param param4 The type of parameter of callback function. For example: int, char.....
////////////////////////////////////////////////////////////////////////////////
#define DECLARE_RPC_CALLBACK_4_PARAM(callbackName, param1, param2, param3, param4) \
    namespace rtk { \
    namespace rpc { \
        typedef void (*callbackName)(param1, param2, param3, param4); \
        template <> struct ParamTypeTrait<callbackName>\
        {\
            enum { TYPE = TYPE_CALLBACK }; \
            typedef RpcCallbackContext RealType;\
        };\
        template <> struct ConvertPolicy<RpcCallbackContext, callbackName>\
        {\
            enum { DEFAULT_CONVERTER = NO };\
            typedef ConvertForCallback<RpcCallbackContext, fpCallbackProxy, Callback4Param<callbackName, param1, param2, param3, param4> > Policy;\
        };\
    };\
    };
    
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// Declare callback function with 5 parameters
///
/// @param callbackName The callback name. Just pick a name which you like.
/// @param param1 The type of parameter of callback function. For example: int, char.....
/// @param param2 The type of parameter of callback function. For example: int, char.....
/// @param param3 The type of parameter of callback function. For example: int, char.....
/// @param param4 The type of parameter of callback function. For example: int, char.....
/// @param param5 The type of parameter of callback function. For example: int, char.....
////////////////////////////////////////////////////////////////////////////////
#define DECLARE_RPC_CALLBACK_5_PARAM(callbackName, param1, param2, param3, param4, param5) \
    namespace rtk { \
    namespace rpc { \
        typedef void (*callbackName)(param1, param2, param3, param4, param5); \
        template <> struct ParamTypeTrait<callbackName>\
        {\
            enum { TYPE = TYPE_CALLBACK }; \
            typedef RpcCallbackContext RealType;\
        };\
        template <> struct ConvertPolicy<RpcCallbackContext, callbackName>\
        {\
            enum { DEFAULT_CONVERTER = NO };\
            typedef ConvertForCallback<RpcCallbackContext, fpCallbackProxy, Callback5Param<callbackName, param1, param2, param3, param4, param5> > Policy;\
        };\
    };\
    };
    
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
/// Declare the callback void (*fpCallback)(int)
/// I put here only for backward compitable.
////////////////////////////////////////////////////////////////////////////////
DECLARE_RPC_CALLBACK_1_PARAM(fpCallback, int);

////////////////////////////////////////////////////////////////////////////////

#endif
