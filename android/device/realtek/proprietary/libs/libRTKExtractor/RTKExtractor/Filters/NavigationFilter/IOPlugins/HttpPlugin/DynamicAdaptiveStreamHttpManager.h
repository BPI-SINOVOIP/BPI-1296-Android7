#ifndef __DYNAMIC_ADAPTIVE_STREAM_HTTP_MANAGER_H__
#define __DYNAMIC_ADAPTIVE_STREAM_HTTP_MANAGER_H__


#include "config.h"
#include "media_source_player.h"
#include "dummy_playback_interface.h"
#include "platform_playback_interface.h"

//#include "LiveStreamManager.h"
#include <Filters/NavigationFilter/NavPlugins.h>
#include "CrossPlatformDecl.h"
#include "MPDParser.h"
#include "SocketAVData.h"

using namespace WebCore;
using namespace platform;

typedef enum _SourceType
{
	INVALID = 0,
	AUDIO,
	VIDEO,
	HINT,
}SourceType;

class DASHManager//: public LiveStreamManager
{
	public:
		DASHManager(IOPLUGIN& IoPlugin);
		~DASHManager();
		bool SetPlayList(const char *pContentBuffer, 
				UINT iBufSize,
				char *pBaseURL,
				char *pOrgURL,
				const char *pUserAgent);
/*		NAVSUBTITLEINFO* GetSubtitleInfo();
		NAVTHUMBNAILINFO* GetThumbnailInfo();
*/
		// The following functions are adapter for IO plguin.
		//
		int Read(BYTE* pBuffer, int iSize, NAVBUF* pEventReceived);
		void Close();
		void GetIoInfo(NAVIOINFO* pNavIoInfo);
/*		int Dispose();
		int GetBufferFullness(int* pFullness, int* pFlag);
		long long Seek(long long llOffset, int iOrigin);
		long long SeekByTime(long long timeOffset);
		bool GetClipInfo(int iTimeOffset, NAVIOCLIPINFO* pInfo);
		void SetBlockingMode(int iBlocking, int iTimeOut);*/
	private:
		bool SetMPDParser(const char *pContentBuffer, 
				UINT iBufSize,
				char *pBaseURL,
				char *pOrgURL,
				const char *pUserAgent);
		void InitMediaSourcePlayer();
		bool CheckStreamRateAndFetchFragment(SourceType type, int iFragmenIndex, int *pFragmentSize, void **pFileHandler);
		bool CheckSourceAppendDone(SourceType type, int iAdaptIndex, int iReprIndex, int iIndex);
		bool CanSourceAppend(SourceType type);
		bool SourceAbort(SourceType type);
		bool SourceAppend(SourceType type, const unsigned char *data, unsigned length);
		bool WaitForTime(float second);
		void ReadAndAppendData();
		void GetFragmentSize(int iIndex);
		static void *StartReadAndAppendThread(void *);
		void SetSocketDataHeader(SOCKET_DATA_TYPE type, SOCKET_INFO_TYPE flag, long long pts, long size);
		void SetSocketDataEvent(NAVBUF *pEventReceived, NAV_BUF_ID type);
		int SetSocketDataAttribute(BYTE *pBuffer);
	private:
		IOPLUGIN& m_IoPlugin;
		MPDParser m_MPDParser;
		bool m_bMPDParsed;
		pthread_t m_Thread;
		bool m_bThreadStoppedFlag;
		MediaSourcePlayer *m_msp;
		UINT m_iStreamRate;
		bool m_bHasAudio;
		bool m_bHasVideo;
		bool m_bAudioInited;
		bool m_bVideoInited;
		bool m_bPlayerStarted;
		bool m_bStreamRateChanged;
		bool m_bAudioStreamEnd;
		bool m_bVideoStreamEnd;
		bool m_bSocketDataAttributeSetted;
		SOCKET_DATA_HEADER m_SocketDataHeader;
		Frame m_frame;
		UINT m_iRemainPlayloadSize;
		uint64_t m_i64CurrentPlayTime;
};
#endif // __DYNAMIC_ADAPTIVE_STREAM_HTTP_MANAGER_H__

