#ifndef __RTK_HWC_HWLAYER_VIDEO_HDCPMESSAGE_H_
#define __RTK_HWC_HWLAYER_VIDEO_HDCPMESSAGE_H_
#include "hwlayer_utils.h"

class HDCPMessage {
public:
    class Client {
    public:
        Client():mService(NULL){};
        virtual ~Client() {};
        virtual void setHDCPMessage(HDCPMessage * service = NULL) {
            if (service)
                service->registerClient(this);
            else if (mService)
                mService->removeClient(this);
        };
        virtual void updataHDCPInfo(bool isHDCP) {
            if (mService)
                return mService->updataHDCPInfo(this, isHDCP);
        };
    private:
        friend class HDCPMessage;
        HDCPMessage * mService;
    };

    HDCPMessage():mHDCP(false) {};
    virtual ~HDCPMessage() {};

    virtual void registerClient     (Client * client){
	client->mService = this;
    };
    virtual void removeClient       (Client * client){
	client->mService = NULL;
    };

    virtual void updataHDCPInfo(Client * client, bool isHDCP);
    virtual bool getHDCPInfo() {return mHDCP;};
private:
    bool mHDCP;
};
#endif

