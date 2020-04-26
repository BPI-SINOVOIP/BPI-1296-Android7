////////////////////////////////////////////////////////////////////////////////
/// \class rtk::ipc::IpcStreamer 
/// Inter-Process Communication Streamer Interface
///
/// @author zackchiu@realtek.com
////////////////////////////////////////////////////////////////////////////////

#ifndef INCLUDED_IPC_STREAMER
#define INCLUDED_IPC_STREAMER

namespace rtk {
namespace ipc {
    class IpcStreamer
    {
        public:
            /// Error code for streamer
            enum ErrCode
            {
                FAILED      = -1, ///< Failed
                TIMEOUT     = -2, ///< Time out
            };

            /// Constants            
            enum
            {
                TIMEOUT_INFINITY = -1 ///< Time out infinity
            };
            
        public:
            /// Open this streamer.<BR>
            /// The caller should call this function before using this streamer.
            ///
            /// @return true --> Success, false --> failed.
            ///
            virtual bool Open() = 0;

            /// Set option for this streamer.
            ///
            /// @param iOption Option.
            /// @param iValie Option value.
            ///
            /// @return true --> Success, false --> Failed.
            ///
            virtual bool SetOption(int iOption, int iValue) = 0;
            
            /// Get option value.
            ///
            /// @param iOption Option
            ///
            /// @return Option value, error code FAILED means failed to query.
            ///
            virtual int GetOption(int iOption) = 0;
            
            /// Read to buffer from stream.
            ///
            /// @param pBuffer The buffer hold the read data.
            /// @param iBufLen Buffer length
            /// @param iTimeOut Timeout(in milli-seconds) for this read operation.<BR>
            ///                 Set to TIME_INFINITY means waiting forever.
            ///
            ///  @return The length of read data.<BR> 
            ///          < 0 means error.<BR> 
            ///          == 0 means end of stream.
            ///
            virtual int Read(char* pBuffer, int iBufLen, int iTimeOut) = 0;
            
            /// Write data to stream
            ///
            /// @param pBuffer The data want to write to stream
            /// @param piDataLen Data length.
            ///
            /// @return The length of written data. < 0 means error.
            ///
            virtual int Write(const char* pBuffer, int iDataLen) = 0;
            
            /// Close stream. <BR>
            /// The caller should call Close() to release the 
            /// resource when this streamer is not needed.
            ///
            virtual void Close() = 0;
            
            /// Check if this stream is opened.
            ///
            /// @return true --> opened. false --> not opened.
            ///
            virtual bool IsOpened() = 0;
    
        public:
            virtual ~IpcStreamer() { ; };
    };
}; // namespace ipc
}; // namespace rtk

#endif
