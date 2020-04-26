///////////////////////////////////////////////////////////////////////////////
/// \class UDP Receiver
/// UDP Receiver<BR>
/// This class is used to receive UDP packets.<BR>
///
/// @author: zackchiu@realtek.com
///////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_UDP_RECEIVER__
#define __INCLUDED_UDP_RECEIVER__

namespace rtk {
namespace net {
    class UDPReceiver
    {
        public:
            /// Constructor
            ///
            UDPReceiver();
            
            /// Destructor
            ///
            ~UDPReceiver();
            
            /// Open the UDP port and start to receive UPD data.
            /// 
            /// @param pStrIP IP address
            /// @param iPort Port number.
            /// @param bMulticast true --> Using multicast
            ///
            /// @return true --> Success.
            ///
            bool Open(const char* pStrIP, int iPort, bool bMulticast);

            /// Check if there exists available data.
            ///
            /// @param iTimeout Timeout(unit is milli-seconds).
            ///
            /// @return true --> There exists available data.
            ///            
            bool IsDataAvailable(unsigned int iTimeout);
    
            /// Read UDP data.
            ///
            /// @param pBuf Buffer to receive data.
            /// @param iBufSize The size of pBuf
            ///
            /// @return < 0 Error.<BR>
            ///         >=0 The length of read data.
            ///
            int Read(char* pBuf, int iBufSize);
            
            /// Close 
            ///
            void Close();

            /// Dump received data to specified path.<BR>
            /// Please calling this function after Open().
            ///
            /// @param pStrFilePath The path of dumpped received data.
            ///
            void DumpReceivedData(const char* pStrFilePath);
                        
        private:
            int m_iPort;
            int m_iRecvSock;
            void* m_pDumppedFile;
            char* m_pTmpBuffer;
            int m_iTmpDataSize;
    };
}; // namespace net
}; // namespace rtk

#endif
