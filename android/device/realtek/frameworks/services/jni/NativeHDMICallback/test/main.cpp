#define LOG_TAG "NativeHDMICallback-main"

#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <utils/Log.h>
#include <utils/String16.h>
#include <utils/String8.h>
#include <binder/Parcel.h>
#include <cutils/compiler.h>

#include "RtkHDMICallback.h"

using namespace android;

NativeRtkHDMICallback::NativeRtkHDMICallback()
{
    ALOGD("NativeRtkHDMICallback create");
}

void
NativeRtkHDMICallback::onHandleEvent(int32_t event)
{
    ALOGD("%s event:%d",__FUNCTION__,event);
}

static sp<IBinder> mHDMIServiceBinder = NULL;

static sp<IBinder> getHDMIServiceBinder()
{
    if (mHDMIServiceBinder == NULL) {
        sp<IServiceManager> sm = defaultServiceManager();
        do {
            mHDMIServiceBinder = sm->getService(String16("RtkHDMIService"));
            if (mHDMIServiceBinder != 0) {
                ALOGD("Got RtkHDMIService....");
                break;
            }
            ALOGD("Service RtkHDMIService is not published.");
        } while (0);
    }
    return mHDMIServiceBinder;
}

static void registerCallback(const sp<IRtkHDMICallback>& callback)
{
    Parcel data, reply;
    int transcation_id = IBinder::FIRST_CALL_TRANSACTION + 28;
    data.writeInterfaceToken(String16("com.realtek.hardware.IRtkHDMIService"));
    data.writeStrongBinder(IInterface::asBinder(callback));
    getHDMIServiceBinder()->transact(transcation_id, data, &reply);
}

static void unregisterCallback(const sp<IRtkHDMICallback>& callback)
{
    Parcel data, reply;
    int transcation_id = IBinder::FIRST_CALL_TRANSACTION + 29;
    data.writeInterfaceToken(String16("com.realtek.hardware.IRtkHDMIService"));
    data.writeStrongBinder(IInterface::asBinder(callback));
    getHDMIServiceBinder()->transact(transcation_id, data, &reply);
}

int main()
{
    ALOGD("main: E");
    ProcessState::self()->startThreadPool();

    sp<NativeRtkHDMICallback> callback = new NativeRtkHDMICallback();
    registerCallback(callback);

    while(1) {
        usleep(1E9);
    }

    return 0;
}
