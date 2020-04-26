#ifndef _RVSD_ENTRY_H_
#define _RVSD_ENTRY_H_

#include <iostream>
#include <memory>

#include <utils/RefBase.h>
#ifdef ANDROID
#include <gui/Surface.h>
#endif

#include "eventListener.h"

#include "RtkTranscodeCommon.h"

#ifndef status_t
#define int32_t status_t
#endif

using namespace std;
using namespace android;

class RVSD_INFO;

class RVSD
{
public:
    RVSD(int handle = -1);
    virtual ~RVSD();

    status_t start();
    status_t stop();
    void setVdecType(int type);
#ifdef ANDROID
    void setVideoSurfaceTexture(const sp<Surface> &surface);
#endif
#ifdef ANDROID_6
    void registerDvdPlayerAgentID(const sp<AHandler> handler_id);
#else
    void registerDvdPlayerAgentID(const int32_t handler_id);
#endif
    void setTranscodeVOWriteBack(const int32_t flag);
    void setTranscodeOSDStatus(const int32_t flag);
    void setLayerOrderValue(const uint32_t value);
    void setPlaySpeedValue(const int32_t nSpeed);
    void setLibMode(const bool bEnable);
    void setStopSocket(const bool bEnable);
    void configureTargetFormat(AV_FORMAT_CONFIG &targetFmt);
    void setListener(const wp<eventListener> &listener);
    void notifyListener(int msg, int ext1 = 0, int ext2 = 0);
    long getVideoPlaneInstance();
    void getPlaybackPts(long long *videoPts, long long *audioPts);
    void getVideoBufferFullness(long* pSize, long* pFullness);
private:
    void init(int handle);
    void genSocketPortName();
    void release();

private:
    RVSD_INFO *pRvsdInfo;
    int mVdecType;
    int mState;
    char socketPortName[64];
    wp<eventListener> mListener;
};

#endif // _RVSD_ENTRY_H_
