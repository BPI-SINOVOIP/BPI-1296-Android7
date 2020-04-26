#ifndef RTK_WIFI_DISPLAY_SINK_H_
#define RTK_WIFI_DISPLAY_SINK_H_

#include "wifi_rtsp.h"
#define MIRACAST_KEY ((key_t) 0x9f1176)
#define t_wfdmsgsize 512
typedef struct {
	int left;
	int top;
	int width;
	int height;
	int show;
}t_myrect;

typedef struct
{
	long mtype;
	char buffer[t_wfdmsgsize];
}t_wfdmsg;

struct RTKWifiDisplaySinkCB {
    RTKWifiDisplaySinkCB();
    virtual ~RTKWifiDisplaySinkCB();
	virtual void OnStateChange(WIFI_SOURCE_STATE oldstate, WIFI_SOURCE_STATE state);
};

struct RTKWifiDisplaySink {
    RTKWifiDisplaySink();
    virtual ~RTKWifiDisplaySink();

	static RTKWifiDisplaySink* GetInstance();
	static void ReleaseInstance();

	bool Init(char* path, int port, RTKWifiDisplaySinkCB* callback);

    bool Start();
	bool Stop();
	bool SendIdr();
	int IsRunning();
	int IsWFDM7();
	WIFI_SOURCE_STATE GetState();	
public:
	void SendUIBC(WFDWindowEvent evt);
	pthread_mutex_t m_thread_mutex;
	RTKWifiDisplaySinkCB* cb;
	char m_path[512];
	int m_port;
};

#endif  // RTK_WIFI_DISPLAY_SINK_H_
