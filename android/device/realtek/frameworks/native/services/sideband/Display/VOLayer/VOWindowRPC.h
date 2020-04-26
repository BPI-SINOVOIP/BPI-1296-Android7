#ifndef __RTK_SB_VO_WINDOW_RPC_H__
#define __RTK_SB_VO_WINDOW_RPC_H__

#include <RPCstruct.h>
#include <VideoRPC_System_data.h>
//#include "../realtek_omx/osal/OSAL.h"
#include <rpc_common.h>
#include <utils/Mutex.h>
#include <pthread.h>
#include <utils/String8.h>
#include <ion.h>

#include <utils/RefBase.h>
#include <android/rect.h>

#include <WindowAPI.h>

class VOWindowRPC : public android::RefBase {
public:
    enum flags_t {
        HIDDEN          = android::SidebandWindowAPI::HIDDEN,
        OPAQUE          = android::SidebandWindowAPI::OPAQUE,
        REFRESH         = android::SidebandWindowAPI::REFRESH,

        FRAME_READY     = android::SidebandWindowAPI::FRAME_READY,
    };

    VOWindowRPC();
    ~VOWindowRPC();
    int setPlane(enum VO_VIDEO_PLANE plane);
    int setCrop(int32_t left, int32_t top, int32_t right, int32_t bottom);
    int setPosition(float x, float y);
    int setSize(uint32_t w, uint32_t h);
    int setLayer(int32_t z);
    int setFlags(uint32_t flags, uint32_t mask);
    int setAlpha(float alpha);
    virtual void dump(android::String8& buf, const char* prefix) const;

private:
    int WindowReSize();
    int getWindowBase(ARect * rect);
    int getFramebufferBase(ARect * rect);
    int hide();
    int show();

    /* Copy from VoutUtil */
    int ConfigureDisplayWindow(
            enum VO_VIDEO_PLANE videoPlane,
            struct VO_RECTANGLE videoWin,
            struct VO_RECTANGLE borderWin,
            struct VO_COLOR borderColor,
            u_char enBorder);
    int ConfigureDisplayWindowZoomWin(
            enum VO_VIDEO_PLANE videoPlane,
            struct VO_RECTANGLE videoWin,
            struct VO_RECTANGLE srcZoomWin,
            struct VO_RECTANGLE borderWin,
            struct VO_COLOR borderColor,
            u_char enBorder);
    int QueryDisplayWin(
            VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN disp_plane_id,
            VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT *out);

    CLNT_STRUCT                     mClnt;
    ARect                           mCrop;
    ARect                           mWindow;
    enum VO_VIDEO_PLANE             mPlane;
    float                           mAlpha;
    int                             mOrder;
    volatile uint32_t               mFlags;
    bool                            mHide;
    int                             mDisplayFd;

};
#endif /* End of __RTK_SB_VO_WINDOW_RPC_H__ */
