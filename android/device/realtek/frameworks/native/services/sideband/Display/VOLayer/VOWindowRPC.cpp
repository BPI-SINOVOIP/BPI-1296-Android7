
#include <cutils/log.h>

#include <VOWindowRPC.h>
#include <hresult.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#define DISPLAY_DEVICES "/dev/graphics/fb0"

#define ATRACE_TAG (ATRACE_TAG_VIDEO)
#include <utils/Timers.h>
#include <utils/Trace.h>

/**********************************************************************************************/
#include <cutils/properties.h>
#define HWC_PROPERTY_OSD_WIDTH                      "rtk.hwc.osd.w"
#define HWC_PROPERTY_OSD_HEIGHT                     "rtk.hwc.osd.h"
ARect get_hwc_osd_resolution() {
    char value[PROPERTY_VALUE_MAX];
    ARect rect;
    rect.left = rect.top = 0;
    property_get(HWC_PROPERTY_OSD_WIDTH,    value, "0"); rect.right     = atoi(value);
    property_get(HWC_PROPERTY_OSD_HEIGHT,   value, "0"); rect.bottom    = atoi(value);
    return rect;
}
/**********************************************************************************************/

VOWindowRPC::VOWindowRPC()
    :mClnt(prepareCLNT(BLOCK_MODE | USE_INTR_BUF | SEND_AUDIO_CPU, AUDIO_SYSTEM, VERSION)),
    mPlane(VO_VIDEO_PLANE_NONE),
    mAlpha(0xff),
    mOrder(0),
    mFlags(0),
    mHide(false),
    mDisplayFd(-1)
{
    //initRPCProxy(); ////////////////////////??
    mCrop.left = mCrop.top = mCrop.right = mCrop.bottom = 0;
    mWindow.left = mWindow.top = mWindow.right = mWindow.bottom = 0;
}
VOWindowRPC::~VOWindowRPC() {
    //TODO: hide window
    if (mDisplayFd > 0)
        close(mDisplayFd);
}

int VOWindowRPC::setPlane(enum VO_VIDEO_PLANE plane) {
    mPlane = plane;
    return 0;
}

int VOWindowRPC::setCrop(int32_t left, int32_t top, int32_t right, int32_t bottom) {
    mCrop.left      = left      ;
    mCrop.top       = top       ;
    mCrop.right     = right     ;
    mCrop.bottom    = bottom    ;
    return 0;
}

int VOWindowRPC::setPosition(float x, float y) {
    int32_t width = mWindow.left - mWindow.right;
    int32_t height = mWindow.bottom - mWindow.top;
    mWindow.left = x;
    mWindow.top = y;
    mWindow.right = mWindow.left + width;
    mWindow.bottom = mWindow.top + height;
    return 0;
}

int VOWindowRPC::setSize(uint32_t w, uint32_t h) {
    mWindow.right = mWindow.left + w;
    mWindow.bottom = mWindow.top + h;
    return 0;
}
int VOWindowRPC::setLayer(int32_t z) {
    //TODO::
    ALOGI("[%s] %d NOT READY!", __FUNCTION__, z);
    mOrder = z;
    return 0;
}
int VOWindowRPC::setFlags(uint32_t flags, uint32_t mask) {
    mFlags = (mFlags & ~mask) | (flags & mask);

    if ((mFlags & HIDDEN) && !mHide)
        hide();
    else if (!(mFlags & HIDDEN) && mHide)
        show();

    if ((mFlags & FRAME_READY) && (mFlags & REFRESH) && !mHide)
        WindowReSize();

    return 0;
}

int VOWindowRPC::hide() {
    if (!mHide)
        mFlags |= REFRESH;
    mHide = true;
    return 0;
}

int VOWindowRPC::show() {
    if (mHide)
        mFlags |= REFRESH;
    mHide = false;
    return 0;
}

int VOWindowRPC::setAlpha(float alpha) {
    //TODO
    ALOGI("[%s] %f NOT READY!", __FUNCTION__, alpha);
    mAlpha = alpha;
    return 0;
}
inline int WIDTH(const ARect & rect)    { return rect.right - rect.left; }
inline int HEIGHT(const ARect & rect)   { return rect.bottom - rect.top; }

int VOWindowRPC::WindowReSize() {
    int ret;
    ARect WindowBase, FBBase;
    VO_RECTANGLE zoomWin;
    zoomWin.x       = mWindow.left;
    zoomWin.y       = mWindow.top;
    zoomWin.width   = WIDTH(mWindow);
    zoomWin.height  = HEIGHT(mWindow);
    ret = getWindowBase(&WindowBase);
    if (ret)
        goto err;

    ret = getFramebufferBase(&FBBase);
    if (ret)
        goto err;

    {
        float w_ratio = (float) WIDTH (WindowBase) / WIDTH (FBBase);
        float h_ratio = (float) HEIGHT(WindowBase) / HEIGHT(FBBase);
        zoomWin.x *= w_ratio;
        zoomWin.y *= h_ratio;
        zoomWin.width *= w_ratio;
        zoomWin.height *= h_ratio;
    }
#if 0
    ALOGI("[%s] [x:%d y:%d w:%d h:%d] [%dx%d] => [%dx%d]", __FUNCTION__, zoomWin.x, zoomWin.y, zoomWin.width, zoomWin.height,
            WIDTH (FBBase), HEIGHT(FBBase),
            WIDTH (WindowBase),HEIGHT(WindowBase));
#endif

    if (WIDTH(mCrop) == 0 || HEIGHT(mCrop) == 0) {
        struct VO_COLOR borderColor = {.c1=0, .c2=0, .c3=0, .isRGB=1};
        struct VO_RECTANGLE cropZoom;
        cropZoom.x      = mCrop.left;
        cropZoom.y      = mCrop.top;
        cropZoom.width  = WIDTH (mCrop);
        cropZoom.height = HEIGHT(mCrop);
        ATRACE_NAME("ConfigureDisplayWindowZoomWin");
        ret = ConfigureDisplayWindowZoomWin(mPlane,
                zoomWin,cropZoom,zoomWin,borderColor,false);
    } else {
        struct VO_COLOR borderColor = {.c1=0, .c2=0, .c3=0, .isRGB=1};
        ATRACE_NAME("ConfigureDisplayWindow");
        ret = ConfigureDisplayWindow(mPlane,
                zoomWin,zoomWin,borderColor,false);
    }

    mFlags &= ~REFRESH;
    return 0;

err:
    return ret;
}

int VOWindowRPC::getWindowBase(ARect * rect) {
    VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN disp_plane_id = {mPlane};
    VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT out;
    ATRACE_NAME("QueryDisplayWin");
    HRESULT hr = QueryDisplayWin(disp_plane_id, &out);
    if (hr != E_FAIL) {
        rect->left = rect->top = 0;
        rect->right  = out.mix1_size.w;
        rect->bottom = out.mix1_size.h;
        return 0;
    }
    return -1;
}

int VOWindowRPC::getFramebufferBase(ARect * rect) {
    ARect hwc_osd_rect = get_hwc_osd_resolution();

    struct fb_var_screeninfo info;
    if (mDisplayFd < 0)
        mDisplayFd = open(DISPLAY_DEVICES, O_RDWR|O_CLOEXEC);

    if (mDisplayFd < 0)
        goto err;

    if (ioctl(mDisplayFd, FBIOGET_VSCREENINFO, &info) == -1)
        goto err;

    rect->left = rect->top = 0;
    rect->right = info.xres;
    rect->bottom = info.yres;

    if (    WIDTH(hwc_osd_rect)     > 0             &&
            HEIGHT(hwc_osd_rect)    > 0             &&
            WIDTH(hwc_osd_rect)     < WIDTH(*rect)  &&
            HEIGHT(hwc_osd_rect)    < HEIGHT(*rect) ) {
        *rect = hwc_osd_rect;
    }

    return 0;
err:
    return -1;
}

int VOWindowRPC::ConfigureDisplayWindowZoomWin(
    enum VO_VIDEO_PLANE videoPlane,
    struct VO_RECTANGLE videoWin,
    struct VO_RECTANGLE srcZoomWin,
    struct VO_RECTANGLE borderWin,
    struct VO_COLOR borderColor,
    u_char enBorder)
{
    /* Copy from VoutUtil */
    VIDEO_RPC_VOUT_CONFIG_DISP_WIN_ZOOM_WIN structConfigDispWin;

    structConfigDispWin.videoPlane = videoPlane;
    structConfigDispWin.videoWin = videoWin;
    structConfigDispWin.srcZoomWin = srcZoomWin;
    structConfigDispWin.borderWin = borderWin;
    structConfigDispWin.borderColor = borderColor;
    structConfigDispWin.enBorder = enBorder;

    ATRACE_NAME("VIDEO_RPC_VOUT_ToAgent_ConfigureDisplayWindowZoomWin_0");
    HRESULT *p_hr = (HRESULT *)VIDEO_RPC_VOUT_ToAgent_ConfigureDisplayWindowZoomWin_0(&structConfigDispWin, &mClnt);
    HRESULT hr = *p_hr;
    free(p_hr);
    //return hr;
    if (hr == S_OK)
        return 0;
    else
        return hr;
}

int VOWindowRPC::ConfigureDisplayWindow(
    enum VO_VIDEO_PLANE videoPlane,
    struct VO_RECTANGLE videoWin,
    struct VO_RECTANGLE borderWin,
    struct VO_COLOR borderColor,
    u_char enBorder)
{
    /* Copy from VoutUtil */
    VIDEO_RPC_VOUT_CONFIG_DISP_WIN structConfigDispWin;

    structConfigDispWin.videoPlane = videoPlane;
    structConfigDispWin.videoWin = videoWin;
    structConfigDispWin.borderWin = borderWin;
    structConfigDispWin.borderColor = borderColor;
    structConfigDispWin.enBorder = enBorder;

    ATRACE_NAME("VIDEO_RPC_VOUT_ToAgent_ConfigureDisplayWindow_0");
    HRESULT *p_hr = (HRESULT *)VIDEO_RPC_VOUT_ToAgent_ConfigureDisplayWindow_0(&structConfigDispWin, &mClnt);
    HRESULT hr = *p_hr;
    free(p_hr);
    //return hr;
    if (hr == S_OK)
        return 0;
    else
        return hr;
}

int VOWindowRPC::QueryDisplayWin(
        VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN disp_plane_id,
        VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT *out)
{
    /* Copy from VoutUtil */
    VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT *pDispWinInfo = VIDEO_RPC_VOUT_ToAgent_QueryDisplayWin_0(&disp_plane_id, &mClnt);
    memset(out, 0, sizeof(VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT));
    HRESULT hr = E_FAIL;
    if (NULL == pDispWinInfo)
        hr = E_FAIL;
    else if (-1 == pDispWinInfo->result || 0 == pDispWinInfo->result)
        hr = S_FALSE;
    else if (1 == pDispWinInfo->result)
        hr = S_OK;
    else
        hr = E_FAIL;
    if (pDispWinInfo) {
        memcpy(out, pDispWinInfo, sizeof(VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT));
        free(pDispWinInfo);
    }
#if 0
    if (hr == S_OK)
        return 0;
    else
#endif
        return hr;
}

void VOWindowRPC::dump(android::String8& buf, const char* prefix) const {
    android::String8 mPrefix(prefix);
    buf.appendFormat("%s", mPrefix.string());
    buf.appendFormat("VOWindowRPC %p mFlags:0x%x mAlpha:%.1f mOrder:%d mHide:%d mDisplayFd:%d\n",
            this, mFlags, mAlpha, mOrder, mHide, mDisplayFd);

    mPrefix.append("    ");

    buf.appendFormat("%s", mPrefix.string());
    buf.appendFormat("mCrop[%d %d %d %d] mWindow[%d %d %d %d]\n",
            mCrop.left, mCrop.top, mCrop.right, mCrop.bottom,
            mWindow.left, mWindow.top, mWindow.right, mWindow.bottom);
}
