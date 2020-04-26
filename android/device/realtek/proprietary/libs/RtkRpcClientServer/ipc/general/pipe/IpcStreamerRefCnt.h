////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcStreamerRefCnt
/// Inter-Process Communication Streamer Interface - Reference counter<BR>
/// <BR>
/// An implementation based on refernece counter.<BR>
/// If the reference counter decrease to zero, the instance will be deleted.
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_STREAMER_REF_CNT
#define INCLUDED_IPC_STREAMER_REF_CNT

#include "ipc/IpcStreamer.h"

namespace rtk {
namespace ipc {
    class IpcStreamerRefCnt: public IpcStreamer
    {
        public:
            bool Open();
            void Close();

        protected: // Sub-class should implement these functions.
            /// This function will be called when initializing to 
            /// do some initialization stuff.
            /// It depends on the implementation of sub-class
            ///
            /// @return Return false to reject this open.
            ///
            virtual bool DoOpen() = 0;
            
            /// This function will be called when the reference counter decrease 
            /// to zero to release this streamer.
            /// It depends on the implementation of sub-class
            ///
            virtual void DoClose() = 0;
        
        public:
            unsigned int GetRefCounter() const { return m_iRefCnt; };
        
        protected:
            IpcStreamerRefCnt();
            ~IpcStreamerRefCnt();
            
        private:
            unsigned int m_iRefCnt;
            bool m_bInit;
    };
}; // namespace ipc
}; // namespace rtk

#endif
