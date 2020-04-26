////////////////////////////////////////////////////////////////////////////////
/// Remote Procedure Call - RPC Type Definition
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_TYPE
#define INCLUDED_RPC_TYPE

namespace rtk {
namespace rpc {
    ////////////////////////////////////////////////////////////////////////////
    // RPC Command Type
    ////////////////////////////////////////////////////////////////////////////
    enum RpcCommandType
    {
        TYPE_INVALID            = 10000,
        TYPE_INVOKE             = 10001,
        TYPE_RESULT             = 10002,
        TYPE_CALLBACK_RESULT    = 10003
    };
    
    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    // RPC Parameter Type
    ////////////////////////////////////////////////////////////////////////////
    enum RpcParamType
    {
        TYPE_UNKNOWN        = 0,
    
        // Basic type
        TYPE_BOOL           = 1,
        TYPE_INT            = 2,
        TYPE_UINT           = 3,
        TYPE_CHAR           = 4,
        TYPE_UCHAR          = 5,
        TYPE_LONGLONG       = 6,
        TYPE_ULONGLONG      = 7,
        TYPE_SHORT          = 8,
        TYPE_USHORT         = 9,
        TYPE_LONG           = 10,
        TYPE_ULONG          = 11,
        TYPE_DOUBLE         = 12,
        TYPE_FLOAT          = 13,
        
        // Composite type
        TYPE_STRING         = 50,
        TYPE_CLASS          = 51,
        TYPE_STRUCTURE      = TYPE_CLASS, // We consider struct and class as same thing.
        TYPE_ARRAY          = 52,
        
        // Misc.
        TYPE_CALLBACK       = 60,
    };

    ////////////////////////////////////////////////////////////////////////////


    ////////////////////////////////////////////////////////////////////////////
    // Trait flag values
    ////////////////////////////////////////////////////////////////////////////
    enum RpcTraitFlag
    {
        NO  = 0,
        YES = 1
    };
    
    ////////////////////////////////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////////////////////////////////
    // Trait for getting parameter type value
    ////////////////////////////////////////////////////////////////////////////
    template <typename T> struct ParamTypeTrait
    {
        enum { TYPE = TYPE_STRUCTURE };
        typedef T RealType; ///< The real type for presenting T
        typedef T Type;     ///< Original type defined by user
    };
    
    template <> struct ParamTypeTrait<bool>
    {
        enum { TYPE = TYPE_BOOL };
        typedef bool RealType;
        typedef bool Type;
    };
    
    template <> struct ParamTypeTrait<int>
    {
        enum { TYPE = TYPE_INT };
        typedef int RealType;
        typedef int Type;
    };
    
    template <> struct ParamTypeTrait<unsigned int>
    {
        enum { TYPE = TYPE_UINT };
        typedef unsigned int RealType;
        typedef unsigned int Type;
    };
    
    template <> struct ParamTypeTrait<char>
    {
        enum { TYPE = TYPE_CHAR };
        typedef char RealType;
        typedef char Type;
    };
    
    template <> struct ParamTypeTrait<unsigned char>
    {
        enum { TYPE = TYPE_UCHAR };
        typedef unsigned char RealType;
        typedef unsigned char Type;
    };

    template <> struct ParamTypeTrait<long long>
    {
        enum { TYPE = TYPE_LONGLONG };
        typedef long long RealType;
        typedef long long Type;
    };
    
    template <> struct ParamTypeTrait<unsigned long long>
    {
        enum { TYPE = TYPE_ULONGLONG };
        typedef unsigned long long RealType;
        typedef unsigned long long Type;
    };
    
    template <> struct ParamTypeTrait<short>
    {
        enum { TYPE = TYPE_SHORT };
        typedef short RealType;
        typedef short Type;
    };
    
    template <> struct ParamTypeTrait<unsigned short>
    {
        enum { TYPE = TYPE_USHORT };
        typedef unsigned short RealType;
        typedef unsigned short Type;
    };
    
    template <> struct ParamTypeTrait<long>
    {
        enum { TYPE = TYPE_LONG };
        typedef long RealType;
        typedef long Type;
    };
    
    template <> struct ParamTypeTrait<unsigned long>
    {
        enum { TYPE = TYPE_ULONG };
        typedef unsigned long RealType;
        typedef unsigned long Type;
    };
    
    template <> struct ParamTypeTrait<double>
    {
        enum { TYPE = TYPE_DOUBLE };
        typedef double RealType;
        typedef double Type;
    };
    
    template <> struct ParamTypeTrait<float>
    {
        enum { TYPE = TYPE_FLOAT };
        typedef float RealType;
        typedef float Type;
    };
    
    template <> struct ParamTypeTrait<const char*>
    {
        enum { TYPE = TYPE_STRING };
        typedef const char* RealType;
        typedef const char* Type;
    };
    
    template <typename T> struct ParamTypeTrait<T*>
    {
        // We don't support pointer
    };
    
    ////////////////////////////////////////////////////////////////////////////
}; // namespace rpc
}; // namespace rtk

#endif
