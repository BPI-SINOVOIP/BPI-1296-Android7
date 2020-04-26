#ifndef __RTK_HWC_HWLAYER_VIDEO_LAYER_H_
#define __RTK_HWC_HWLAYER_VIDEO_LAYER_H_
#include <utils/LayerBase.h>
#include <utils/PLockBase.h>
#include <utils/HDCPMessage.h>
#include "VideoBuffer.h"

class VideoLayer : 
    public  LayerBase,
    virtual GraphicBufferInfo::Client
{
public:
    VideoLayer();
    virtual ~VideoLayer();

    virtual int                 connect(enum VO_VIDEO_PLANE plane, int BufferCnt, int audioHwSync = -1);
    virtual int                 disconnect();
    virtual int                 getAudioHwSyncFd() {return mAudioHwSyncFd;};
    virtual BufferBase *        createBuffer();
    virtual void                removeBuffer(BufferBase * buffer);
    virtual enum VO_VIDEO_PLANE getPlane() {return mPlane;};
    virtual int                 validate(buffer_handle_t handle);
    virtual int                 getMaxBufferCount() {return mBufferCount;};
    virtual void                dump(android::String8& buf, const char* prefix) {return LayerBase::dump(buf, prefix);};
    virtual bool                getHDCPInfo();
private:
    using LayerBase::connect;
    PLockBase * mPLock;
    GraphicBufferInfo * mGBInfo;
    HDCPMessage *mHDCPMessage;
    int mBufferCount;
    enum VO_VIDEO_PLANE mPlane;
    int mAudioHwSyncFd;
};
#endif /* End of __RTK_HWC_HWLAYER_VIDEO_LAYER_H_ */
