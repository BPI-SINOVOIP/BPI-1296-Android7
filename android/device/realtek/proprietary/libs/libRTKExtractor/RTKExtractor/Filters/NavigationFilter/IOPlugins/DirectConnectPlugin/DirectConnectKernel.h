///////////////////////////////////////////////////////////////////////////////
// Direct Connect Kernel
// This class is for handling Air Play Protocol.
//
// Author: zackchiu@realtek.com
//
// Created: 3/16/2011
////////////////////////////////////////////////////////////////////////////////

#ifndef __DIRECT_CONNECT_KERNEL__
#define __DIRECT_CONNECT_KERNEL__

#include "Utility/CommandProcessor.h"
#include <stdint.h>

namespace rtk {
namespace net { 
    class CSocketClient; 
}; // namespace net
}; // namespace rtk

enum DirectConnectKernelErrCode
{
    DCK_ERR_SUCCESS = 0,
    DCK_ERR_RECV_COMMAND_OK = -100000,
    DCK_ERR_RECV_COMMAND_FAILED, 
    DCK_ERR_INVALID_COMMAND,
    DCK_ERR_INVALID_URL,
    DCK_ERR_OPEN_FAILED,
    DCK_ERR_SEEK_FAILED,
    DCK_ERR_READ_TIMEOUT,
    DCK_ERR_READ_FAILED,
    DCK_ERR_INVALID_ARGUMENT,
    DCK_ERR_WRITE_COMMAND_FAILED,
};

class DirectConnectKernel: public rtk::tools::CommandProcessor<int>
{
    public:
        DirectConnectKernel(rtk::net::CSocketClient* pControlSocket, 
                            rtk::net::CSocketClient* pDataSocket,
                            unsigned int iId);
        ~DirectConnectKernel();
        
        int Open(const char* pURL);
        int Read(char* pBuffer, unsigned int iBufferSize, int iFlag);
        int Write(const char* pBuffer, unsigned int iDataSize, int iFlag);
        long long Seek(long long iOffset, int iOrigin);
        long long GetContentLength();
        unsigned int GetId();
               
        // Update time status. 
        // This is called by media player to update playback time.
        //
        //  Parameters:
        //      1. iDuration: Current playback duration.
        //      2. iTotalTime: Total playback time.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool UpdateTimeStatus(unsigned int iDuration, unsigned int iTotalTime);
        
        // Update volume status. 
        // This is called by media player to update volume value.
        //
        //  Parameters:
        //      1. iVolume: Current volume value.
        //      2. iMaxVolume: The max volume value.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool UpdateVolumeStatus(unsigned int iVolume, unsigned int iMaxVolume);
        
        // Update version information to client.
        //
        //  Parameters:
        //      1. iVerMajor: Major verion number.
        //      2. iVerMinor: Minor verion number.
        //      3. iVerBuild: Build verion number.
        //      4. iVerRevision: Revision verion number.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool UpdateVersionInfo(unsigned int iVerMajor,
                               unsigned int iVerMinor,
                               unsigned int iVerBuild,
                               unsigned int iVerRevision);

        // Update capability information in bitmasks format to client.
        //
        //  Parameters:
        //      1. u8FmtVersion: Bitmasks format version number, 1 byte.
        //      2. u8ModelId: (reserved) Model id, 1 byte (refer to mapping table).
        //      3. u16CapBitmask1: Capability bitmasks value1, 16 bits, refer to mapping table (0~15).
        //      4. u32CapBitmask2: Capability bitmasks value2, 32 bits, refer to mapping table (16~47).
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool UpdateCapabilityInfo(uint8_t u8FmtVersion,
                                  uint8_t u8ModelId,
                                  uint16_t u16CapBitmask1,
                                  uint32_t u32CapBitmask2);
                               
        // Play media by index. 
        //
        //  Parameters:
        //      1. iIndex: The index of item in a playlist (zero-based)
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Play(unsigned int iIndex = 0);
        
        // Stop playing media.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Stop();
        
        // Pause playing media.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Pause();

        // Resume playing media.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Resume();


        // Close playing media.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Close();
        
        // Get the file nama in the file list.
        //
        //  Parameters:
        //      1. iIndex: The index of file name(zero-based)
        //
        //  Return:
        //      File name or NULL for failed.
        //
        const char* GetFileName(unsigned int iIndex);

        // Get file counts in the file list.
        //
        //  Return:
        //      File counts.
        //        
        unsigned int GetFileCounts();
        
        int SendCommand2(const char* pStrCmd, 
                        long long iValue);
                        
    private:
        struct DirectConnectRecvCmd;
        struct DirectConnectSendCmd;
        struct PrivateData;
        
    private:
        PrivateData* m_pData;

    private:
        enum
        {
            CMD_GET_COMMAND,
        };
        
     private:
        bool Execute(int iCmd);
        void Release();
        template <class T> int ReadCmd(T& value);
        int ReadCmd(DirectConnectRecvCmd& recvCmd);
        int ReadUntilCmdReceived(DirectConnectRecvCmd& recvCmd);

        // Send command to the connected peer.
        //
        //  Parameters:
        //      1. pStrCmd: Command string (4 characters in length).
        //      2. iValue: Send this value to the connected peer.
        //      3. pStrValue: Send this string (4 characters in length).
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        int SendCommand(const char* pStrCmd, 
                        long long iValue);
        int SendCommand(const char* pStrCmd, 
                        const char* pStrValue, 
                        int iValue = 0);
        int SendCommand(const char* pStrCmd,
                        unsigned int iValue1,
                        unsigned int iValue2);

        // This function will create a memory block to hold the extra data.
        // The caller has the duty to release this memory blocks.
        //
        char* ReadAndCopyExtraData(int& iExtraDataSize);
         
        bool ReadPlayList();
        bool ReadUpgradePath();
        bool IsInConnectionSession(unsigned int iConnectionType);

    private:
        DirectConnectKernel();
        DirectConnectKernel(const DirectConnectKernel&);
        DirectConnectKernel& operator=(const DirectConnectKernel&);
};

#endif
