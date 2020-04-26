///////////////////////////////////////////////////////////////////////////////
// Direct Connect Manager. 
// This is for starting/stopping Direct Connect Server.
// It will spawn DirectConnectKernel to handle protocol stuffs.
//
// Author: zackchiu@realtek.com
//
// Created: 3/16/2011
////////////////////////////////////////////////////////////////////////////////

#ifndef __DIRECT_CONNECT_MANAGER__
#define __DIRECT_CONNECT_MANAGER__

namespace rtk {
namespace net {
    class CSocketServer;
}; // namespace net
}; // namespace rtk

class DirectConnectKernel;
class DirectConnectManager
{
    public:
        static DirectConnectManager& GetInstance();

        // Start Direct Connect Server
        //
        //  Return:
        //      true --> Success.
        //        
        bool StartServer();
        
        // Stop Direct Connect Server
        //
        //  Return:
        //      true --> Success.
        //      
        bool StopServer();

        // Get the DirectConnectKernel
        // DirectConnectIOPlugin will get a kernel ID from URL.
        // The URL is the form "directconnect://F?seq=XX&size=XX&kernelID=XX"
        // DirectConnectIOPlugin can use this ID to get the DirectConnectKernel.
        //
        //  Parameters:
        //      1. iKernelId: A kernel ID from URL
        //
        //  Return:
        //      The pointer of DirectConnectKernel
        //
        DirectConnectKernel* GetDirectConnectKernel(unsigned int iKernelId);
        
        // When the DirectConnectKernel is not needed.
        // The caller can use this function to release it.
        //
        void ReleaseDirectConnectKernel(DirectConnectKernel* pKernel);
        void ReleaseDirectConnectKernel(unsigned int iKernelId);
        
        // Update time status. 
        // This is called by media player to update playback time.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //      2. iDuration: Current playback duration.
        //      3. iTotalTime: Total playback time.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool UpdateTimeStatus(unsigned int iKernelId, 
                              unsigned int iDuration,
                              unsigned int iTotalTime);

        // Update volume status. 
        // This is called by media player to update volume value.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //      2. iVolume: Current volume value.
        //      3. iMaxVolume: The max volume value.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool UpdateVolumeStatus(unsigned int iKernelId, 
                                unsigned int iVolume, 
                                unsigned int iMaxVolume);
                              
        // Play media by index. 
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //      2. iIndex: The index of item in a playlist (zero-based)
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Play(unsigned int iKernelId, unsigned int iIndex = 0);
        
        // Stop playing media.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Stop(unsigned int iKernelId);
        
        // Pause playing media.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Pause(unsigned int iKernelId);

        // Resume playing media.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //
        bool Resume(unsigned int iKernelId);

        // Close the playing media.
        //  
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //
        //  Return:
        //      true --> Success, false --> Failed.
        //      
        bool Close(unsigned int iKernelId);

        // Used to parse URL
        //
        //  Parameters:
        //      1. pURL: The URL wants to be parsed.
        //      2. pMark: Hold the mark when function returned.
        //      3. pSeqNum: Hold the sequence number when function returned.
        //      4. 
        bool ParseDirectConnectURL(const char* pURL, 
                                   char* pMark = 0, 
                                   unsigned int* pSeqNum = 0, 
                                   unsigned long long* pSize = 0,
                                   unsigned int* pKernelId = 0,
                                   unsigned int* pIndex = 0,
                                   unsigned int* pSubType = 0);
                                   
        // Get the file nama in the file list.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //      2. iIndex: The index of file name(zero-based)
        //
        //  Return:
        //      File name or NULL for failed.
        //
        const char* GetFileName(unsigned int iKernelId, unsigned int iIndex);

        // Get file counts in the file list.
        //
        //  Parameters:
        //      1. iKernelId: Kernel ID. The ID can be obtained from URL.
        //
        //  Return:
        //      File counts.
        //        
        unsigned int GetFileCounts(unsigned int iKernelId);
        int SendCommand2(unsigned int iKernelId,const char* pStrCmd, long long iValue);

    private:
        class DirectConnectServerObserver;
        
    private:
        rtk::net::CSocketServer* m_pServer;
        DirectConnectServerObserver* m_pObs;
        
    private:
        DirectConnectManager();
        ~DirectConnectManager();
        DirectConnectManager(const DirectConnectManager&);
        DirectConnectManager& operator=(const DirectConnectManager&);
};

#endif
