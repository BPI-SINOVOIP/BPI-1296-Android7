#include "VideoLayer.h"
#include "VideoBuffer.h"

VideoLayer::VideoLayer()
    : mGBInfo(NULL),
    mBufferCount(10),
    mPlane(VO_VIDEO_PLANE_V1),
    mAudioHwSyncFd(-1),
    mHDCPMessage(NULL)
{
    mHDCPMessage = new HDCPMessage();
    mGBInfo = new GraphicBufferInfo();
    setGraphicBufferInfo(mGBInfo);
}

VideoLayer::~VideoLayer() {
    delete mHDCPMessage;
    mHDCPMessage = NULL;
    delete mGBInfo;
}

BufferBase * VideoLayer::createBuffer() {
    VideoBuffer * buffer = new VideoBuffer();
    buffer->setPLockBase(mPLock);
    buffer->setHDCPMessage(mHDCPMessage);
    return buffer;
}

void VideoLayer::removeBuffer(BufferBase * buffer) {
    ((VideoBuffer *)buffer)->setPLockBase();
    delete buffer;
}

int VideoLayer::connect(enum VO_VIDEO_PLANE plane, int BufferCnt, int audioHwSync) {
    mAudioHwSyncFd = audioHwSync;
    mPlane = plane;
    if (BufferCnt < 0 || BufferCnt > 100) {
        ALOGE("[%s] buffer count(%d) setting is unreasonable!!!!", __FUNCTION__, BufferCnt);
        mBufferCount = 10;
    } else
        mBufferCount = BufferCnt;
    ALOGI("[%s] mPlane:%d plane:%d BufferCnt:%d", __FUNCTION__, mPlane, plane, BufferCnt);
    mPLock = new PLockBase(mBufferCount, VideoBuffer::PLOCK_VERSION);
    return LayerBase::connect();
}

int VideoLayer::disconnect() {
    int ret = LayerBase::disconnect();
    delete mPLock;
    mPLock = NULL;
    mAudioHwSyncFd = -1;
    return ret;
}

int VideoLayer::validate(buffer_handle_t handle) {
    int ret = -1;

    GraphicBufferInfo::private_yuv_data * pYUVData = getYUVData();

    if (GraphicBufferInfo::Client::validate(handle) != 0)
        return ret;

    setBufferHandle(handle);
    if (pYUVData != NULL) {
        if (!(pYUVData->usage & GRALLOC_USAGE_HW_ION))
            ret = -2;
        else if ((pYUVData->format & HAL_PIXEL_FORMAT_RTK_P10_MAGIC_MASK) == HAL_PIXEL_FORMAT_RTK_P10_MAGIC)
            ret = 0;
        else
            switch (pYUVData->format) {
                case HAL_PIXEL_FORMAT_YCrCb_420_SP :
                case HAL_PIXEL_FORMAT_YV12 :
                case HAL_PIXEL_FORMAT_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_YCbCr_420_P :
                case HAL_PIXEL_FORMAT_YCbCr_422_I :
                case HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_RTK_32_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W16_H16_YCbCr_420_SP:
                case HAL_PIXEL_FORMAT_RTK_W32_H16_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W16_H32_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W32_H32_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W16_H8_YCbCr_420_SP :
                case HAL_PIXEL_FORMAT_RTK_P10_COMPRESSED :
                case HAL_PIXEL_FORMAT_RTK_P10_LINEAR :
                case HAL_PIXEL_FORMAT_RTK_P8_COMPRESSED :
                case HAL_PIXEL_FORMAT_RTK_W32_H32_P10_COMPRESSED :
                case HAL_PIXEL_FORMAT_RTK_W32_H32_P8_COMPRESSED :
                case HAL_PIXEL_FORMAT_RTK_W16_H16_YCrCb_420_SP:
                case HAL_PIXEL_FORMAT_RTK_W32_H16_YCrCb_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W16_H32_YCrCb_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W32_H32_YCrCb_420_SP :
                case HAL_PIXEL_FORMAT_RTK_W16_H8_YCrCb_420_SP :
                    ret = 0;
                    break;
                default:
                    break;
            }
    }
    clearBufferHandle();
    return ret;
}

bool VideoLayer::getHDCPInfo(void) {
    if(mHDCPMessage != NULL)
        return mHDCPMessage->getHDCPInfo();
    else
        return false;
}

