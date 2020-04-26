#include "VideoBuffer.h"

VideoBuffer::VideoBuffer()
    :mFlags(0)
{
    resetWin();
}

VideoBuffer::~VideoBuffer()
{
    ALOGI("[%s] %p : (mFlags:0x%x)", __FUNCTION__, this, mFlags);
    updataHDCPInfo(false);
#if 0
    if (mFlags & FILLYUV)
        resetWin();

    if (mFlags & QUEUE_BUFFER) {
        int wait = 3;
        while (!releaseBufferCheck() && wait-- && (mFlags & QUEUE_BUFFER))
            usleep(16000);

        if (wait == 0) {
            ALOGE("[%s] releaseBufferCheck wait too long!", __FUNCTION__);
            releaseBuffer_();
        }
    }
#endif
}

void VideoBuffer::removeBuffer() {
    if (mFlags & FILLYUV)
        resetWin();

    if (mFlags & QUEUE_BUFFER) {
        int wait = 3;
        while (BufferBase::releaseBuffer() != 0 && wait-- && (mFlags & QUEUE_BUFFER))
            usleep(16000);

        if (wait == 0) {
            ALOGE("[%s] releaseBufferCheck wait too long!", __FUNCTION__);
            mFlags &= ~QUEUE_BUFFER;
        }
    }

    BufferBase::removeBuffer();
}

void VideoBuffer::updateHDCP() {
    GraphicBufferInfo::private_rtk_data * pPrivData = getPrivData();

    if(pPrivData->hdmi_flag.isProtect)
        updataHDCPInfo(true);
    else
        updataHDCPInfo(false);
}

int VideoBuffer::prepareBuffer_() {
    int ret = -1;
    GraphicBufferInfo::private_yuv_data * pYUVData = getYUVData();
    GraphicBufferInfo::private_rtk_data * pPrivData = getPrivData();
    unsigned int phyBase = -1U;

    if (pYUVData == NULL) {
        ALOGE("[%s] pYUVData:%p", __FUNCTION__, pYUVData);
        goto err;
    }

    if (getPLockVersion() != PLOCK_VERSION) {
        ALOGE("[%s] getPLockVersion return 0x%x", __FUNCTION__, getPLockVersion());
        goto err;
    }

    if (isExtraBuffer()) {
        ALOGE("[%s] not support the extra buffer!", __FUNCTION__);
        goto err;
    }

    updateHDCP();

    if (isReplicaBuffer()) {
#if 0 // [ATM-1180] Do not modify the pts of replica buffer to avoid video stuck
        pYUVData->pts[0] = pYUVData->pts[1] = 0;
#endif
        pYUVData->delay_mode = RingBufferBase::LowDelayDisplay;
        pPrivData->lumaOffTblAddr       = -1U;
        pPrivData->chromaOffTblAddr     = -1U;
        pPrivData->lumaOffTblAddrR      = -1U;
        pPrivData->chromaOffTblAddrR    = -1U;
    }

    if (    pPrivData->display_width  != 0 &&
            pPrivData->display_height != 0 &&
            pPrivData->display_stride != 0) {
        /* overwrite the display size */
        pYUVData->width             = pPrivData->display_width;
        pYUVData->height            = pPrivData->display_height;
        pYUVData->stride            = pPrivData->display_stride;
    }

    mYUVWin.version         = getPLockVersion();//PLOCK_VERSION;
    mYUVWin.IsForceDIBobMode = pYUVData->IsForceDIBobMode;

    if ((pYUVData->format & HAL_PIXEL_FORMAT_RTK_P10_MAGIC_MASK) == HAL_PIXEL_FORMAT_RTK_P10_MAGIC) {
        mYUVWin.Y_addr          = (unsigned int) ((long) getPhyAddr(0) & -1U);
        mYUVWin.Y_addr          += pYUVData->p10_compressed_offset[0];
        mYUVWin.U_addr          = (unsigned int) ((long) getPhyAddr(0) & -1U);
        mYUVWin.U_addr          += pYUVData->p10_compressed_offset[1];
        mYUVWin.width           = HAL_PIXEL_FORMAT_RTK_P10_WIDTH_GET(pYUVData->format);
        mYUVWin.height          = HAL_PIXEL_FORMAT_RTK_P10_HEIGHT_GET(pYUVData->format);
        mYUVWin.Y_pitch         = pYUVData->p10_compressed_stride[0];
        mYUVWin.C_pitch         = pYUVData->p10_compressed_stride[0];
    } else {
        mYUVWin.Y_addr          = (unsigned int) ((long) getPhyAddr(0) & -1U);
        mYUVWin.U_addr          = (unsigned int) ((long) getPhyAddr(1) & -1U);
        mYUVWin.width           = (pPrivData->display_width  != 0) ? pPrivData->display_width  : pYUVData->width;
        mYUVWin.height          = (pPrivData->display_height != 0) ? pPrivData->display_height : pYUVData->height;
        mYUVWin.Y_pitch         = pYUVData->stride;
        mYUVWin.C_pitch         = 0;
    }

    mYUVWin.pLock           = (unsigned int) ((long) getPLockPhyAddr() & -1U);
    mYUVWin.context         = 0;//mContext;
    mYUVWin.mode            = pYUVData->yuv_mode;// CONSECUTIVE_FRAME;
    switch (pYUVData->format) {
        case HAL_PIXEL_FORMAT_RTK_W16_H16_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H16_YCrCb_420_SP :
        case HAL_PIXEL_FORMAT_RTK_W16_H32_YCrCb_420_SP :
        case HAL_PIXEL_FORMAT_RTK_W32_H32_YCrCb_420_SP :
        case HAL_PIXEL_FORMAT_RTK_W16_H8_YCrCb_420_SP :
            mYUVWin.mode |= VO_NV21_MASK;
            break;
    }
    mYUVWin.pixelAR_hor     = 0;
    mYUVWin.pixelAR_ver     = 0;
    mYUVWin.Y_addr_Right    = 0xffffffff;
    mYUVWin.U_addr_Right    = 0xffffffff;
    mYUVWin.pLock_Right     = 0xffffffff;
    mYUVWin.mvc             = 0;
    mYUVWin.subPicOffset    = 0;

    mYUVWin.PTSH2           = (unsigned int) (pYUVData->pts[1] >> 32);
    mYUVWin.RPTSH2          = (unsigned int) (pYUVData->pts[1] >> 32);
    mYUVWin.PTSL2           = (unsigned int) (pYUVData->pts[1] & 0xffffffffLL);
    mYUVWin.RPTSL2          = (unsigned int) (pYUVData->pts[1] & 0xffffffffLL);
    mYUVWin.PTSH            = (unsigned int) (pYUVData->pts[0] >> 32);
    mYUVWin.PTSL            = (unsigned int) (pYUVData->pts[0] & 0xffffffffLL);
    mYUVWin.RPTSH           = (unsigned int) (pYUVData->pts[0] >> 32);
    mYUVWin.RPTSL           = (unsigned int) (pYUVData->pts[0] & 0xffffffffLL);

    /* For HEVC 10bit HDR */
    phyBase = (unsigned int) ((long) getPhyAddr(0) & -1U);

    mYUVWin.lumaOffTblAddr =
        (pPrivData->lumaOffTblAddr      != -1U) ? pPrivData->lumaOffTblAddr :
        (pYUVData->lumaTableOffset      != -1U) ? (phyBase + pYUVData->lumaTableOffset ): -1U;

    mYUVWin.chromaOffTblAddr =
        (pPrivData->chromaOffTblAddr    != -1U) ? pPrivData->chromaOffTblAddr :
        (pYUVData->chromaTableOffset    != -1U) ? (phyBase + pYUVData->chromaTableOffset ): -1U;

    mYUVWin.lumaOffTblAddrR =
        (pPrivData->lumaOffTblAddrR     != -1U) ? pPrivData->lumaOffTblAddrR :
        (pYUVData->lumaTableOffsetR     != -1U) ? (phyBase + pYUVData->lumaTableOffsetR ): -1U;

    mYUVWin.chromaOffTblAddrR =
        (pPrivData->chromaOffTblAddrR   != -1U) ? pPrivData->chromaOffTblAddrR :
        (pYUVData->chromaTableOffsetR   != -1U) ? (phyBase + pYUVData->chromaTableOffsetR ): -1U;

#if 0
    mYUVWin.lumaOffTblAddr                     = pPrivData->lumaOffTblAddr;
    mYUVWin.chromaOffTblAddr                   = pPrivData->chromaOffTblAddr;
    mYUVWin.lumaOffTblAddrR                    = 0xffffffff;
    mYUVWin.chromaOffTblAddrR                  = 0xffffffff;
#endif
    mYUVWin.bufBitDepth                        = pPrivData->bufBitDepth;
    mYUVWin.bufFormat                          = pPrivData->bufFormat;
    mYUVWin.transferCharacteristics            = pPrivData->transferCharacteristics;
    mYUVWin.display_primaries_x0               = (unsigned short)pPrivData->display_primaries_x0;
    mYUVWin.display_primaries_y0               = (unsigned short)pPrivData->display_primaries_y0;
    mYUVWin.display_primaries_x1               = (unsigned short)pPrivData->display_primaries_x1;
    mYUVWin.display_primaries_y1               = (unsigned short)pPrivData->display_primaries_y1;
    mYUVWin.display_primaries_x2               = (unsigned short)pPrivData->display_primaries_x2;
    mYUVWin.display_primaries_y2               = (unsigned short)pPrivData->display_primaries_y2;
    mYUVWin.white_point_x                      = (unsigned short)pPrivData->white_point_x;
    mYUVWin.white_point_y                      = (unsigned short)pPrivData->white_point_y;
    mYUVWin.max_display_mastering_luminance    = pPrivData->max_display_mastering_luminance;
    mYUVWin.min_display_mastering_luminance    = pPrivData->min_display_mastering_luminance;

    mFlags |= FILLYUV;

    if (    mYUVWin.Y_addr  == 0    ||
            mYUVWin.width   == 0    ||
            mYUVWin.height  == 0    ||
            mYUVWin.Y_pitch == 0) {
        ALOGE("[%s] w:%d h:%d Yaddr:0x%08x Ypitch:%d", __FUNCTION__, mYUVWin.width, mYUVWin.height,
                mYUVWin.Y_addr, mYUVWin.Y_pitch);
        goto err;
    }

    ret = 0;
err:
    return ret;
}

void VideoBuffer::queueBuffer_() {
    GraphicBufferInfo::private_yuv_data * pYUVData = getYUVData();
    if (pYUVData == NULL) {
        ALOGE("[%s] pYUVData:%p", __FUNCTION__, pYUVData);
        return;
    }

    if (!(mFlags & FILLYUV)) {
        ALOGE("[%s] mFlags:0x%x", __FUNCTION__, mFlags);
        return;
    }

    if (mFlags & QUEUE_BUFFER) {
        ALOGE("[%s] mFlags:0x%x", __FUNCTION__, mFlags);
        return;
    }

    ConfigLowDelay((enum RingBufferBase::LowDelayMode) pYUVData->delay_mode,
            pYUVData->delay_depth, pYUVData->init_frame);

    setPLockStatus(PLockBase::PLOCK_STATUS_Q_PEND);

    SetDeintFlag(pYUVData->deintflag);

    InbandCmd(VIDEO_VO_INBAND_CMD_TYPE_OBJ_PIC, &mYUVWin, sizeof(mYUVWin));

    mFlags &= ~FILLYUV;
    mFlags |= QUEUE_BUFFER;
}

void VideoBuffer::cancelBuffer_() {
    if (mFlags & QUEUE_BUFFER)
        ALOGE("[%s] mFlags:0x%x", __FUNCTION__, mFlags);
    else
        resetWin();
}

bool VideoBuffer::releaseBufferCheck() {
    bool release = false;
    if (!(mFlags & QUEUE_BUFFER)) {
        ALOGE("[%s] mFlags:0x%x", __FUNCTION__, mFlags);
        release = true;
    }

    if (!release && getPLockStatus() == PLockBase::PLOCK_STATUS_VO_UNLOCK)
        release = true;
#if 0
    else
        ALOGI("[%s] getPLockStatus:%x", __FUNCTION__, getPLockStatus());
#endif

    if (release)
        releaseBuffer_();

    return release;
}

void VideoBuffer::releaseBuffer_() {
    if (!(mFlags & QUEUE_BUFFER)) {
        ALOGE("[%s] mFlags:0x%x", __FUNCTION__, mFlags);
        return;
    }
    setPLockStatus(PLockBase::PLOCK_STATUS_RESET);
    mFlags &= ~QUEUE_BUFFER;
}

void VideoBuffer::resetWin() {
    memset((void *) &mYUVWin, 0, sizeof(mYUVWin));
    mFlags &= ~FILLYUV;
}

void VideoBuffer::dump(android::String8& buf, const char* prefix) {
    android::String8 state;
    if (prefix != NULL)
        buf.appendFormat("%s", prefix);
    PLockBase::Client::dump(state);
    state.append(",");
    BufferBase::dump(state);
    buf.appendFormat("%s", state.string());
    if (prefix != NULL)
        buf.append("\n");
}
