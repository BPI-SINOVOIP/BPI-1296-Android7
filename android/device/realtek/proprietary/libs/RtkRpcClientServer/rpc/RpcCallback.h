////////////////////////////////////////////////////////////////////////////////
/// \class rtk::rpc::RpcCallback
/// Remote Procedure Call - RPC Callback Interface - Server Side Only<BR>
/// <BR>
/// This interface is used to present a callback function based on 
/// client's request.<BR>
/// <BR>
/// This object will be passed to executor, executor need to keep the instance
/// for using in the future.<BR>
/// When the local callback it triggered, executor need to use 
/// RpcCallback::Callback() to pass the result to remote client.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_RPC_CALLBACK
#define INCLUDED_RPC_CALLBACK

#include "RpcCommandMuxer.h"
#include <assert.h>

namespace rtk {
namespace rpc {
    class RpcCallback
    {
        public:
            /// When the local callback triggered, executor need to 
            /// use this function to pass the result to remote client.
            ///
            /// @param iIndex The index(zero-based) of callback function.<BR>
            ///               Since there maybe exist several callback functions.
            /// @param param1 Parameter passed to client's callback function.
            ///
            /// @return true --> Success.<BR>
            ///         false--> Failed, because of disconnection of remote client.
            ///                  You should stop local callback, and then call Release()
            ///                  to release the resource occupied by this class.
            ///
            template <class T1> 
            bool Callback(int iIndex, T1 param1);

            /// When the local callback triggered, executor need to 
            /// use this function to pass the result to remote client.
            ///
            /// @param iIndex The index(zero-based) of callback function.<BR>
            ///               Since there maybe exist several callback functions.
            /// @param param1 Parameter passed to client's callback function.
            /// @param param2 Parameter passed to client's callback function.
            ///
            /// @return true --> Success.<BR>
            ///         false--> Failed, because of disconnection of remote client.
            ///                  You should stop local callback, and then call Release()
            ///                  to release the resource occupied by this class.
            ///
            template <class T1, class T2> 
            bool Callback(int iIndex, T1 param1, T2 param2);

            /// When the local callback triggered, executor need to 
            /// use this function to pass the result to remote client.
            ///
            /// @param iIndex The index(zero-based) of callback function.<BR>
            ///               Since there maybe exist several callback functions.
            /// @param param1 Parameter passed to client's callback function.
            /// @param param2 Parameter passed to client's callback function.
            /// @param param3 Parameter passed to client's callback function.
            ///
            /// @return true --> Success.<BR>
            ///         false--> Failed, because of disconnection of remote client.
            ///                  You should stop local callback, and then call Release()
            ///                  to release the resource occupied by this class.
            ///
            template <class T1, class T2, class T3> 
            bool Callback(int iIndex, T1 param1, T2 param2, T3 param3);

            /// When the local callback triggered, executor need to 
            /// use this function to pass the result to remote client.
            ///
            /// @param iIndex The index(zero-based) of callback function.<BR>
            ///               Since there maybe exist several callback functions.
            /// @param param1 Parameter passed to client's callback function.
            /// @param param2 Parameter passed to client's callback function.
            /// @param param3 Parameter passed to client's callback function.
            /// @param param4 Parameter passed to client's callback function.
            ///
            /// @return true --> Success.<BR>
            ///         false--> Failed, because of disconnection of remote client.
            ///                  You should stop local callback, and then call Release()
            ///                  to release the resource occupied by this class.
            ///
            template <class T1, class T2, class T3, class T4> 
            bool Callback(int iIndex, T1 param1, T2 param2, T3 param3, T4 param4);

            /// When the local callback triggered, executor need to 
            /// use this function to pass the result to remote client.
            ///
            /// @param iIndex The index(zero-based) of callback function.<BR>
            ///               Since there maybe exist several callback functions.
            /// @param param1 Parameter passed to client's callback function.
            /// @param param2 Parameter passed to client's callback function.
            /// @param param3 Parameter passed to client's callback function.
            /// @param param4 Parameter passed to client's callback function.
            /// @param param5 Parameter passed to client's callback function.
            ///
            /// @return true --> Success.<BR>
            ///         false--> Failed, because of disconnection of remote client.
            ///                  You should stop local callback, and then call Release()
            ///                  to release the resource occupied by this class.
            ///
            template <class T1, class T2, class T3, class T4, class T5> 
            bool Callback(int iIndex, T1 param1, T2 param2, T3 param3, T4 param4, T5 param5);
            
            /// Get the original function name to trigger this callback.
            ///
            virtual const char* GetFunctionName() = 0;
            
            /// This class is managed by reference counter.
            /// When this class is created, the counter set to 1 by default.
            /// Call AddRef() to increase counter by 1.
            /// Call Release() to decrease counter by 1.
            /// When the counter is decrease to 0, this class will be released by itself.
            ///
            /// @return The value of current counter.
            ///
            virtual unsigned int AddRef() = 0;
            virtual unsigned int Release() = 0;
            
            /// Check if remote client disconnected.
            ///
            /// @return true --> Disconnected.
            ///
            virtual bool IsRemoteDisconnected() = 0;
            
            /// Get the unique ID for this callback object
            ///
            /// @return The unique ID for this callback object
            ///
            virtual unsigned int GetId() = 0;

            /// Get the total counts of callback functions in this object.<BR>
            /// Since there maybe exist several callback functions.
            ///
            /// @return The total counts of callback functions in this object.
            ///
            virtual unsigned int GetCounts() = 0;

        protected: // Sub-class need to implement following functions to do callback stuffs
            virtual bool DoCallback(RpcCommandMuxer& muxer) = 0;
            
        public:
            virtual ~RpcCallback() { ; };
            
        private:
            enum { BUFFER_SIZE = 8 * 1024 };
    };
    
    template <class T1> 
    bool RpcCallback::Callback(int iIndex, T1 param1)
    {
        assert(iIndex >= 0 && iIndex < (int)GetCounts());
        if (IsRemoteDisconnected()) return false;
            
        char tmpBuf[BUFFER_SIZE] = {0, };
        RpcCommandMuxer muxer(tmpBuf, BUFFER_SIZE);
        muxer.Push(iIndex);
        muxer.Push(param1);
        muxer.Finish(TYPE_CALLBACK_RESULT, GetId());
        return DoCallback(muxer);
    };
    
    template <class T1, class T2> 
    bool RpcCallback::Callback(int iIndex, T1 param1, T2 param2)
    {
        assert(iIndex >= 0 && iIndex < GetCounts());
        if (IsRemoteDisconnected()) return false;
        
        char tmpBuf[BUFFER_SIZE] = {0, };
        RpcCommandMuxer muxer(tmpBuf, BUFFER_SIZE);
        muxer.Push(iIndex);
        muxer.Push(param1);
        muxer.Push(param2);
        muxer.Finish(TYPE_CALLBACK_RESULT, GetId());
        return DoCallback(muxer);
    };
    
    template <class T1, class T2, class T3> 
    bool RpcCallback::Callback(int iIndex, T1 param1, T2 param2, T3 param3)
    {
        assert(iIndex >= 0 && iIndex < GetCounts());
        if (IsRemoteDisconnected()) return false;
        
        char tmpBuf[BUFFER_SIZE] = {0, };
        RpcCommandMuxer muxer(tmpBuf, BUFFER_SIZE);
        muxer.Push(iIndex);
        muxer.Push(param1);
        muxer.Push(param2);
        muxer.Push(param3);
        muxer.Finish(TYPE_CALLBACK_RESULT, GetId());
        return DoCallback(muxer);
    };
    
    template <class T1, class T2, class T3, class T4> 
    bool RpcCallback::Callback(int iIndex, T1 param1, T2 param2, T3 param3, T4 param4)
    {
        assert(iIndex >= 0 && iIndex < GetCounts());
        if (IsRemoteDisconnected()) return false;
        
        char tmpBuf[BUFFER_SIZE] = {0, };
        RpcCommandMuxer muxer(tmpBuf, BUFFER_SIZE);
        muxer.Push(iIndex);
        muxer.Push(param1);
        muxer.Push(param2);
        muxer.Push(param3);
        muxer.Push(param4);
        muxer.Finish(TYPE_CALLBACK_RESULT, GetId());
        return DoCallback(muxer);
    };
    
    template <class T1, class T2, class T3, class T4, class T5> 
    bool RpcCallback::Callback(int iIndex, T1 param1, T2 param2, T3 param3, T4 param4, T5 param5)
    {
        assert(iIndex >= 0 && iIndex < GetCounts());
        if (IsRemoteDisconnected()) return false;
        
        char tmpBuf[BUFFER_SIZE] = {0, };
        RpcCommandMuxer muxer(tmpBuf, BUFFER_SIZE);
        muxer.Push(iIndex);
        muxer.Push(param1);
        muxer.Push(param2);
        muxer.Push(param3);
        muxer.Push(param4);
        muxer.Push(param5);
        muxer.Finish(TYPE_CALLBACK_RESULT, GetId());
        return DoCallback(muxer);
    };
}; // namespace rpc
}; // namespace rtk

#endif
