/*
 * Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 * MdService.h
 *
 *  Created on: June 25, 2015
 *      Author: calvinkuo
 */

#ifndef __RTK_MDSERVICE_H__
#define __RTK_MDSERVICE_H__

#include <utils/Vector.h>
#include <binder/IBinder.h>
#include <binder/BinderService.h>

#include "RtkIpc/IMdService.h"    //necessary for register.cpp

namespace android {

class Mutex;

class MdService :
    public BinderService<MdService>,
    public BnMdService
{
    friend class BinderService<MdService>;

public:
    MdService();
    virtual             ~MdService();

    // Implementation of BinderService<T>
    static char const* getServiceName() { return "vendor.realtek.md"; }

    //Implementation of IMdService
    virtual unsigned int mdInit(sp<IBinder> &binder);
    virtual void mdUninit(sp<IBinder> &binder);
    virtual int mdMemcpy(unsigned int dstPAddr, unsigned int srcPAddr, int size, unsigned int hQueue);

    virtual int mdlibExecCmd(int byCommandQueue);
    virtual int mdlibWaitCmdDone(int byCommandQueue);
#if 0
    virtual int mdlibCopy(int byCommandQueue, void *pMdDestinationProcInfo, void *pMdSourceProcInfo);
#endif

private:
    class DeathNotifier : public IBinder::DeathRecipient
    {
    public:
        DeathNotifier(MdService *service, unsigned int hQueue);
        virtual ~DeathNotifier();
        virtual void binderDied(const wp<IBinder> &who);
        unsigned int getHandler() { return mHandler; }
    private:
        MdService *mService;
        sp<IBinder> mClient;
        unsigned int mHandler;
    };

    friend class DeathNotifier;

    void clientDied(DeathNotifier *notifier, const wp<IBinder> &who);

    Mutex mLock;
    KeyedVector< sp<IBinder>, sp<DeathNotifier> > mMap;
};

} // namespace android

#endif //__RTK_MDSERVICE_H__
