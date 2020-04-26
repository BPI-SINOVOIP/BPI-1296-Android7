///////////////////////////////////////////////////////////////////////////////
//
// Author:
//
// Created:
////////////////////////////////////////////////////////////////////////////////

#ifndef __AIRFUN_IPC__
#define __AIRFUN_IPC__

#include <pthread.h>

typedef bool(*AirfunNotifyFunc)(char*);

struct AirFunPlaybackInfo
{
	int state;
	int duration;
	int timePosition;
	int mute;
	int volume;
	int repeat;
	int playType;
    int videoState;
    int audioState;
    int imageState;
};

struct AirFunMetaData
{
	char title[128];
	char album[128];
	char artist[128];
};

class AirFunIpc
{
	private:
        AirFunIpc(void);
		~AirFunIpc(void);
		bool CreateServer(void);
		bool CreateConnection(void);

	public:
		void SetNotifyFunc(AirfunNotifyFunc fptr);
		void UpdatePlaybackStatus(int typeId, int value);
        void UpdatePlaybackStatusByMime(int mimeId, int typeId, int value);
		bool OpenAndDownloadImage(const char *url, char buf[], int bufLen);
		bool ExtractMetaData(const char *url, AirFunMetaData *metaData);
		static AirFunIpc *GetInstance(void);
		static void DelInstance(void);

	private:
		int m_socket, m_newsocket;
		int m_socketClient;
		pthread_t m_ipcThread;
		AirFunPlaybackInfo m_playbackInfo;
		AirfunNotifyFunc m_NotifyFunc;
		char m_filename[128];
        pthread_mutex_t m_mutex;
		static AirFunIpc* m_singleton;
		static void* IpcThread(void *args);
        static void processCommand(AirFunIpc *afIpc, char *pCmdStr);
};

#endif // __AIRFUN_IPC__
