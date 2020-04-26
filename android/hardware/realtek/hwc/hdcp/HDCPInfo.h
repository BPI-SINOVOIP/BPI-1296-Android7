#ifndef __RTK_HWC_HWLAYER_VIDEO_HDCPINFO_H_
#define __RTK_HWC_HWLAYER_VIDEO_HDCPINFO_H_

#define MAX_DISPLAY_COUNT 2
#include <pthread.h>
#include <property/HWCProperty.h>

class HDCPInfo {
public:
    class Client {
    public:
        Client():mService(NULL){};
        virtual ~Client() {};
        virtual void setHDCPInfo(HDCPInfo* service = NULL, int displayId = 0) {
            mDisplayId = displayId;
            if (service)
                service->registerClient(this);
            else if (mService)
                mService->removeClient(this);
        };
        virtual void updateHDCPInfo(bool isHDCP) {
            if (mService)
                return mService->updateHDCPInfo(this, mDisplayId ,isHDCP);
        };
    private:
        friend class HDCPInfo;
        HDCPInfo* mService;
        int mDisplayId;
    };

    HDCPInfo();
    virtual ~HDCPInfo();

    virtual void registerClient     (Client * client){
	client->mService = this;
    };
    virtual void removeClient       (Client * client){
	client->mService = NULL;
    };

    static void *update_time(void *arg);
    virtual void updateHDCPInfo(Client * client, int displayId ,bool isHDCP);
    pthread_t mthread;
    bool isThreadRunning;
    HWCProperty mHWCProperty;
private:
    bool mHDCP[MAX_DISPLAY_COUNT];
};
#endif

