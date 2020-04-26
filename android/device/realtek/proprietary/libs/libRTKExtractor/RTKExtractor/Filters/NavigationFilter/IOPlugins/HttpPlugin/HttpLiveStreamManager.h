////////////////////////////////////////////////////////////////////////////////
// HTTP Live Streaming Manager
// 
// This class uses class HLSPlayList to parse and get playable files.
// It is a utility for HTTP IO Plugin to adapt HLS.
//
// Author: zackchiu@realtek.com
//
// Created: 10/18/2010
////////////////////////////////////////////////////////////////////////////////

#ifndef __HTTP_LIVE_STREAM_MANAGER__
#define __HTTP_LIVE_STREAM_MANAGER__

#include "HLSPlayList.h"
#include <Filters/NavigationFilter/NavPlugins.h>
#include <map>
#include "auto_xxx_ptr.h"
#include "CrossPlatformDecl.h"
#include <string>
#include "Platform_Lib/MCPControl/MCPUtil.h"
#include <list>

class PreloadClip;
class HLSClip;
class CachedClipList;

typedef struct
{
	pthread_mutex_t main_lock;
	pthread_mutex_t reentry_lock;
	rtk::parser::m3u::HLSPlayList *pHLSPlayList;
	PreloadClip* pPreloadClip;
	bool bstopAgent;
} HLSManager_AgentParam;

class HLSManager: public rtk::parser::m3u::HLSObserver
{
    public:
        HLSManager(IOPLUGIN& IoPlugin);
        ~HLSManager();
        
        bool SetPlayList(const char* pContentBuffer, UINT iBufSize, 
                         const char* pOrgURL, const char* pRedirectURL, const char* pUserAgent);

        NAVSUBTITLEINFO* GetSubtitleInfo();
        NAVTHUMBNAILINFO* GetThumbnailInfo();
        
        // The following functions are adapter for IO plguin.
        //
        int Read(BYTE* pBuffer, int iSize, NAVBUF* pEventReceived);
        void Close();
        void GetIoInfo(NAVIOINFO* pNavIoInfo);
        int Dispose();
        int GetBufferFullness(int* pFullness, int* pFlag);
        long long Seek(long long llOffset, int iOrigin);
        long long SeekByTime(long long timeOffset);
        bool GetClipInfo(int iTimeOffset, NAVIOCLIPINFO* pInfo);
        void SetBlockingMode(int iBlocking, int iTimeOut);

    public: // HLS Observer Implementation
        void ChangeBandWidth(UINT iNewBandWidth);
        
    private:
        bool LoadNextItem(rtk::parser::PlayListItem* pItem = NULL, bool bPreload = true);
        void CloseItem();
        bool IsEncrypted(rtk::parser::PlayListItem* pItem);
        bool IsKeyAlreadyDownload(rtk::parser::PlayListItem* pItem);
        bool LoadKey(rtk::parser::PlayListItem* pItem);
        BYTE* GetKey(rtk::parser::PlayListItem* pItem);
        void ReleaseCachedKey();
        void ReleaseSubtitleInfo();
        bool InitCipher(rtk::parser::PlayListItem* pItem);
        bool FreeCipher();
        int ReadEncryptedData(BYTE* pBuffer, int iSize, NAVBUF* pEventReceived);
        int DecryptData(BYTE* pBuffer, int iSize);
        void CalculateStreamRate(unsigned int iCurStreamRate);
		HLSClip* _GetNextItem_Blocking(PreloadClip* pPreloadClip, rtk::parser::PlayListItem* pItem);
		void _StoreClipInfo(HLSClip *pClip);
    private:
        typedef struct MEDIA_FILE_INFO
        {
            int64_t iFileSize;
            int iDuration; 
        } MediaFileInfo;

        typedef struct CLIP_INFO
        {
        	unsigned int iSeqID;
            unsigned int iSize;
            unsigned int iDuration;
        } ClipInfo;

    private:
        enum // Constants
        {
            AES_128_KEY_SIZE                      = 16,
            AES_128_IV_SIZE                       = 16,
            AES_128_CBC_PREVIOUS_CIPHER_TEXT_SIZE = 16,
            AES_128_ALIGNMENT_SIZE                = 16,
            AES_128_BLOCK_SIZE                    = 16,
            AES_128_ALIGNMENT_MASK                = 0xFFFFFFF0, // Align with 16 bytes
        };

    private:
        rtk::parser::m3u::HLSPlayList m_HLSPlayList;
        IOPLUGIN& m_IoPlugin;
        HLSClip* m_pClip;
        PreloadClip* m_pPreloadClip;
        unsigned int m_iStreamRate; // Bits per second.
        std::map<std::string, BYTE*> m_keyMap;
        NAVSUBTITLEINFO m_SubtitleInfo;
        MCP_CIPHER_CTX  m_CipherContext;
        bool m_bNeedPrevCipherText;
        BYTE m_CachedBuffer[AES_128_ALIGNMENT_SIZE];
        UINT m_iCachedDataSize;
        NAVTHUMBNAILINFO m_ThumbnailInfo;
        unsigned int m_iReadedDuration; // The duration of readed clips.
        unsigned int m_iReadedSeqID;
        std::list<ClipInfo> m_ClipInfoCachedList;
        std::list<unsigned int> m_StreamRateList;
        rtk::parser::PlayListItem* m_pCachedPreloadedItem;
        bool m_bUseCachedPreloadedItem;
		CThread *m_pHLSAgentThread;
		HLSManager_AgentParam m_HLSAgentParam;
		CachedClipList *m_CachedClipList;
};

#endif

