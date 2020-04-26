////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcStreamerPipeImpl
// Inter-Process Communication Streamer - Pipe Implementation
//
// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_STREAMER_PIPE_IMPL
#define INCLUDED_IPC_STREAMER_PIPE_IMPL

#include "ipc/general/pipe/IpcStreamerRefCnt.h"

namespace rtk {
namespace ipc {
    class IpcStreamerPipeImpl: public IpcStreamerRefCnt
    {
        public:
            /// A factory method for this class.<BR>
            /// <BR>
            /// You should use this function to create the instance of this class.
            ///
            /// @param pStrServerName The IPC server name.
            /// @param iPipeId Pipe ID.
            /// @param bServerSide true --> Server side pipe streamer.<BR>
            ///                    false--> Client side pipe streamer.
            ///
            /// @return The instance of this class.
            ///
            static IpcStreamerPipeImpl* Create(const char* pStrServerName, 
                                               int iPipeId, 
                                               bool bServerSide);
            
        public: // Implementation of interface of IpcStreamer
            bool SetOption(int iOption, int iValue);
            int GetOption(int iOption);
            int Read(char* pBuffer, int iBufLen, int iTimeOut);
            int Write(const char* pBuffer, int iDataLen);
            bool IsOpened();

        private:
            IpcStreamerPipeImpl(const char* pStrServerName, 
                                int iPipeId, bool bServerSide);
            ~IpcStreamerPipeImpl();
            
        private:
            bool DoOpen();
            void DoClose();

        private:
            void ReleaseAllBuffer();
            
        private:
            bool m_bServerSide;
            int m_iInputFd;
            int m_iOutputFd;
            int m_iId;
            char m_StrInputFifoName[256];
            char m_StrOutputFifoName[256];
    };
}; // namespace ipc
}; // namespace rtk

#endif
