#include "CursorBuffer.h"

CursorBuffer::CursorBuffer()
    : mType(tNONE),
    mReleaseContext(-1U),
    mLayer(NULL),
    mWindowRatioIsReady(false)
{
    mVOutCtrl = &VoutUtil::instance();
    resetCursorBufferState();
}

CursorBuffer::~CursorBuffer()
{
    resetCursorBufferState();
}

void CursorBuffer::removeBuffer() {
    resetCursorBufferState();
    BufferBase::removeBuffer();
}

int CursorBuffer::prepareBuffer_() {
    int ret = -1;
    if (isReplicaBuffer() /*&& isExtraBuffer()*/)
        goto done;

    if (isExtraBuffer())
        mType = tCOORDINATE;
    else if (isGrallocBuffer())
        mType = tPICTURE;

    switch (mType) {
        case tPICTURE :
            {
                ret = prepareBuffer_picture();
                break;
            }
        case tCOORDINATE :
            {
                ret = prepareBuffer_coordinate();
                break;
            }
        default:
            break;
    }

done:
    if (ret != 0 || mType == tNONE)
        ALOGE("[%s:%p:%d] ERROR! mType=%d ret=%d", __FUNCTION__, this, __LINE__, mType, ret);
    return ret;
}

int CursorBuffer::getWindowBase(hwc_rect_t* rect) {
    VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN disp_plane_id = {VO_VIDEO_PLANE_OSD1};
    VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT out;
    HRESULT hr= mVOutCtrl->QueryDisplayWin(disp_plane_id, &out);
    if (hr != E_FAIL) {
        rect->left = rect->top = 0;
        rect->right  = out.mix1_size.w;
        rect->bottom = out.mix1_size.h;
        return 0;
    }
    return -1;
}

int CursorBuffer::getFramebufferBase(hwc_rect_t* rect) {
    int ret = -1;

    if (rect == NULL)
        goto err;

    if (mLayer != NULL && mLayer->getDisplayInfo() != NULL) {
        rect->left = rect->top = 0;
        rect->right = mLayer->getDisplayInfo()->getWidth();
        rect->bottom = mLayer->getDisplayInfo()->getHeight();
        ret = 0;
    } else {
        rect->left = rect->top = 0;
        rect->right = 1920;
        rect->bottom = 1080;
        ret = -1;
        ALOGE("[%s:%d]", __FUNCTION__, __LINE__);
    }
err:
    return ret;
}

int CursorBuffer::updateWindowRatio() {
    hwc_rect_t WindowBase, FBBase;

    if (mWindowRatioIsReady)
        return 0;

    if (getFramebufferBase(&FBBase) == 0 && getWindowBase(&WindowBase) == 0) {
        mWidthRatio = (float) WIDTH (WindowBase) / WIDTH (FBBase);
        mHeightRatio = (float) HEIGHT(WindowBase) / HEIGHT(FBBase);
        mWindowRatioIsReady = true;
    } else {
        mWidthRatio = mHeightRatio = 1;
    }
    return 0;
}

enum ColorLookUpTable::PixelBuffer::ScalerType CursorBuffer::getScalerType() {
    enum ColorLookUpTable::PixelBuffer::ScalerType scalerType = ColorLookUpTable::PixelBuffer::ScalerType::tNORMAL;
    if (mWidthRatio < 0.7)
        scalerType = ColorLookUpTable::PixelBuffer::tSEMI;
    else if (mWidthRatio >= 1.5)
        scalerType = ColorLookUpTable::PixelBuffer::tDOUBLE;

    return scalerType;
}

int CursorBuffer::prepareBuffer_picture() {
    int ret = -1;
    GraphicBufferInfo::private_yuv_data * pYUVData = getYUVData();

    updateWindowRatio();

    ret = setCursorTextureBuffer(
            getVirAddr(),
            pYUVData->format,
            pYUVData->width,
            pYUVData->height,
            pYUVData->stride,
            getScalerType());

    if (ret)
        goto done;

    mReleaseContext = getRefClockUnusedContext();

    mPictureObject.context = mReleaseContext;
    mPictureObject.width = getCursorTextureWidth();
    mPictureObject.height = getCursorTextureHeight();

    mPictureObject.address      = (unsigned int) ((long)getCursorTexturePhyAddr() & -1U);
    mPictureObject.clut_address = (unsigned int) ((long)getCursorTextureTablePhyAddr() & -1U);

    mPictureObject.colorFmt     = 1;//INBAND_CMD_GRAPHIC_FORMAT_4BIT;
    mPictureObject.address_right = 0;
    mPictureObject.PTSH = 0;
    mPictureObject.PTSL = 0;
    ret = 0;
done:
    if (ret!=0)
        ALOGE("[%s:%p:%d] ERROR! %d", __FUNCTION__, this, __LINE__, ret);
    return ret;
}

int CursorBuffer::prepareBuffer_coordinate() {
    int ret = -1;
    updateWindowRatio();
    CoordinateObject * object = (CoordinateObject *) getExtBuffer();
    mReleaseContext = getRefClockUnusedContext();
    mCoordinateObject.context = mReleaseContext;
    mCoordinateObject.hide = object->mHide;
    mCoordinateObject.x = (unsigned int) ( mWidthRatio * (float)object->mX);
    mCoordinateObject.y = (unsigned int) ( mHeightRatio * (float)object->mY);
    mCoordinateObject.PTSH = (unsigned int) ((object->mTimestamp >> 32) & -1U);
    mCoordinateObject.PTSL = (unsigned int) ((object->mTimestamp >> 0) & -1U);

    ret = 0;
done:
    if (ret!=0)
        ALOGE("[%s:%p:%d] ERROR! %d", __FUNCTION__, this, __LINE__, ret);
    return ret;
}

void CursorBuffer::queueBuffer_() {
    switch (mType) {
        case tCOORDINATE :
            {
                InbandCmd(VIDEO_CURSOR_INBAND_CMD_TYPE_COORDINATE_OBJECT,
                        &mCoordinateObject, sizeof(mCoordinateObject));
                mType = tQUEUE;
                break;
            }
        case tPICTURE:
            {
                InbandCmd(VIDEO_CURSOR_INBAND_CMD_TYPE_PICTURE_OBJECT,
                        &mPictureObject, sizeof(mPictureObject));
                mType = tQUEUE;
                break;
            }
        default:
            break;
    }
}

void CursorBuffer::cancelBuffer_() {
    ALOGI("[%s:%p:%d]", __FUNCTION__, this, __LINE__);
    resetCursorBufferState();
}

bool CursorBuffer::releaseBufferCheck() {
    bool ret = false;

    if (mType != tQUEUE)
        goto done;

    if (getRefClockVideoContext() >= mReleaseContext)
        ret = true;

    if (getRefClockVideoContext() > (mReleaseContext + 3))
        ALOGI("[%s:%p] mType=%s mReleaseContext=%d getRefClockVideoContext()=%d", __FUNCTION__, this,
                (mType == tNONE)?"tNONE":
                (mType == tCOORDINATE)?"tCOORDINATE":
                (mType == tPICTURE)?"tPICTURE":
                (mType == tQUEUE)?"tQUEUE":"ERR"
                ,mReleaseContext,getRefClockVideoContext());
    if (ret)
        resetCursorBufferState();
done:
    return ret;
}

void CursorBuffer::resetCursorBufferState() {
    mType = tNONE;
    mReleaseContext = -1U;
    memset(&mPictureObject, 0, sizeof(mPictureObject));
    memset(&mCoordinateObject, 0, sizeof(mCoordinateObject));
}

void CursorBuffer::dump(android::String8& buf, const char* prefix) {
    android::String8 state;
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    state.appendFormat("mReleaseContext=%d,mType=%s,", mReleaseContext,
                (mType == tNONE)?"tNONE":
                (mType == tCOORDINATE)?"tCOORDINATE":
                (mType == tPICTURE)?"tPICTURE":
                (mType == tQUEUE)?"tQUEUE":"ERR");
    CursorTexture::Client::dump(state);
    state.append(",");
    BufferBase::dump(state);
    buf.appendFormat("%s", state.string());
    if (prefix != NULL)
        buf.append("\n");
}
