#include "CursorLayer.h"
#include "CursorBuffer.h"

CursorLayer::CursorLayer()
    : mGBInfo(NULL),
    mBufferCount(10),
    mPlane(VO_VIDEO_PLANE_CSR),
    mDisplayInfo(NULL)
{
    mGBInfo = new GraphicBufferInfo();
    setGraphicBufferInfo(mGBInfo);
}

CursorLayer::~CursorLayer() {
    delete mGBInfo;
}

BufferBase * CursorLayer::createBuffer() {
    CursorBuffer * buffer = new CursorBuffer();
    buffer->setCursorTexture(mTexture);
    buffer->setLayer(this);
    return buffer;
}

void CursorLayer::removeBuffer(BufferBase * buffer) {
    ((CursorBuffer *)buffer)->setCursorTexture();
    ((CursorBuffer *)buffer)->setLayer();
    delete buffer;
}

int CursorLayer::connect(enum VO_VIDEO_PLANE plane, int BufferCnt) {
    mPlane = plane;
    if (BufferCnt < 0 || BufferCnt > 100) {
        ALOGE("[%s] buffer count(%d) setting is unreasonable!!!!", __FUNCTION__, BufferCnt);
        mBufferCount = 10;
    } else
        mBufferCount = BufferCnt;
    ALOGI("[%s] mPlane:%d plane:%d BufferCnt:%d", __FUNCTION__, mPlane, plane, BufferCnt);
    mTexture = new CursorTexture();
    return LayerBase::connect();
}

int CursorLayer::disconnect() {
    int ret = LayerBase::disconnect();
    delete mTexture;
    mTexture = NULL;
    return ret;
}

int CursorLayer::render(buffer_handle_t handle, int *pFenceFd) {
    if (pFenceFd != NULL)
        *pFenceFd = -1;
    return LayerBase::render(handle);
};

int CursorLayer::validate(buffer_handle_t handle) {
    int ret = -1;

    GraphicBufferInfo::private_yuv_data * pYUVData = getYUVData();

    if (GraphicBufferInfo::Client::validate(handle) != 0)
        goto err;

    setBufferHandle(handle);
    if (pYUVData->format == HAL_PIXEL_FORMAT_RGBA_8888)
        ret = 0;
    else
        ALOGW("[%s] format:0x%x", "CursorLayer::validate", pYUVData->format);
#if 0
    if (pYUVData != NULL) {
        if ((pYUVData->format & HAL_PIXEL_FORMAT_RTK_P10_MAGIC_MASK) == HAL_PIXEL_FORMAT_RTK_P10_MAGIC)
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
                    ret = 0;
                    break;
                default:
                    break;
            }
    }
#endif
    clearBufferHandle();
err:
    if (ret != 0)
        ALOGE("[%s:%d] ERROR!", __FUNCTION__, __LINE__);
    return ret;
}

void CursorLayer::updateWindow() {
    int ret = -1;
    BufferBase * buffer = NULL;
    CursorBuffer::CoordinateObject * object = NULL;

    LayerBase::lock();

    if (mTexture == NULL)
        goto err;

    buffer = dequeueBuffer();
    if (buffer == NULL)
        goto err;

    object = new CursorBuffer::CoordinateObject();
    if (object == NULL)
        goto err;

    object->setWin(mX,mY);
    object->setHide(mHide);
    object->setPTS(0);

    if (mHide)
        ALOGI("[%s:%p:%d] mHide=%d", __FUNCTION__, this, __LINE__, mHide);

    ret = buffer->prepareBuffer(object);

    if (ret) {
        buffer->cancelBuffer();
        cancelBuffer(buffer);
    } else {
        buffer->queueBuffer();
        queueBuffer(buffer);
    }

err:
    LayerBase::unlock();
    if (ret) {
        if (object != NULL)
            delete object;
        ALOGE("[%s] buffer:%p", __FUNCTION__, buffer);
    }
    return;
}

void CursorLayer::dump(android::String8& buf, const char* prefix) {
    LayerBase::dump(buf, prefix);
    if (mTexture != NULL)
        mTexture->dump(buf, prefix);
}
