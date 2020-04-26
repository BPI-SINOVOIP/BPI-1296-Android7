/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define ALIGN(x, y)     ((x + y - 1) & ~(y - 1))
#define ALIGN8(x)       ALIGN(x, 8)
#define ALIGN16(x)      ALIGN(x, 16)
#define ALIGN32(x)      ALIGN(x, 32)

//#define LOG_NDEBUG 0
#define LOG_TAG "ColorConverter"
#include <utils/Log.h>

#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/ColorConverter.h>
#include <media/stagefright/MediaErrors.h>
#include <system/graphics.h>
#include <utils/CallStack.h>

#ifdef LIBYUV_NEON
#include <libyuv.h>
#include <libyuv/row.h>
#include <libyuv/convert_wrapper.h>
using namespace libyuv;
#endif
//#define TRACE_CONVERT_TIME	// debug LIBYUV_NEON convert time

//#define DUMP_DECODE_FRAME // Dump decode frame
#ifdef DUMP_DECODE_FRAME
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif /* end of DUMP_DECODE_FRAME */

#include "libyuv/convert_from.h"

#define USE_LIBYUV

namespace android {
#ifdef TRACE_CONVERT_TIME
static void check_time(struct timeval *tv_start, struct timeval *tv_end) {
    // check total time 
    int diff_sec, diff_usec;
    float total_time;
    diff_sec = tv_end->tv_sec - tv_start->tv_sec;
    if (tv_end->tv_usec >= tv_start->tv_usec)
    {
            diff_usec = tv_end->tv_usec - tv_start->tv_usec;
    }
    else
    {
            diff_usec = 1000000 - tv_start->tv_usec + tv_end->tv_usec;
            diff_sec--;
    }
    total_time = ((float)diff_sec) + ((float)diff_usec)/1000000;
    ALOGD("#### total time --  %10.6f secs\n", total_time );
}

#define _DBG_CheckTime_Init() \
    static struct timeval tv_start, tv_end;
#define _DBG_CheckTime_Begin() \
    { gettimeofday(&tv_start, 0); }
#define _DBG_CheckTime_End() \
    { gettimeofday(&tv_end, 0); \
      check_time(&tv_start, &tv_end); }
#else
#define _DBG_CheckTime_Init() {}
#define _DBG_CheckTime_Begin() {}
#define _DBG_CheckTime_End() {}
      
#endif

ColorConverter::ColorConverter(
        OMX_COLOR_FORMATTYPE from, OMX_COLOR_FORMATTYPE to)
    : mSrcFormat(from),
      mDstFormat(to),
      mClip(NULL) {
}

ColorConverter::~ColorConverter() {
    delete[] mClip;
    mClip = NULL;
}

bool ColorConverter::isValid() const {
    if (mDstFormat != OMX_COLOR_Format16bitRGB565) {
        return false;
    }

    switch ((int)mSrcFormat) {
        case HAL_PIXEL_FORMAT_YCbCr_420_SP: // Workaround, for bug 41956
        case HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_32_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H16_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H16_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H32_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H32_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H8_YCbCr_420_SP:
            ALOGD("workaround, 420SP mSrcFormat:%d", mSrcFormat);
        case HAL_PIXEL_FORMAT_RTK_W16_H16_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H16_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H32_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H32_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H8_YCrCb_420_SP:
        case OMX_COLOR_FormatYUV420Planar:
        case OMX_COLOR_FormatCbYCrY:
        case OMX_QCOM_COLOR_FormatYVU420SemiPlanar:
        case OMX_COLOR_FormatYUV420SemiPlanar:
        case OMX_TI_COLOR_FormatYUV420PackedSemiPlanar:
            return true;

        default:
            ALOGD("%s:%d received not support format:%d-%x %d:%d:%d:%d", __FUNCTION__, __LINE__, mSrcFormat, mSrcFormat, HAL_PIXEL_FORMAT_YCbCr_420_SP, HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP, HAL_PIXEL_FORMAT_RTK_32_YCbCr_420_SP, OMX_COLOR_FormatYUV420Planar);
            return false;
    }
}

ColorConverter::BitmapParams::BitmapParams(
        void *bits,
        size_t width, size_t height,
        size_t cropLeft, size_t cropTop,
        size_t cropRight, size_t cropBottom)
    : mBits(bits),
      mWidth(width),
      mHeight(height),
      mCropLeft(cropLeft),
      mCropTop(cropTop),
      mCropRight(cropRight),
      mCropBottom(cropBottom) {
}

size_t ColorConverter::BitmapParams::cropWidth() const {
    return mCropRight - mCropLeft + 1;
}

size_t ColorConverter::BitmapParams::cropHeight() const {
    return mCropBottom - mCropTop + 1;
}

status_t ColorConverter::convert(
        const void *srcBits,
        size_t srcWidth, size_t srcHeight,
        size_t srcCropLeft, size_t srcCropTop,
        size_t srcCropRight, size_t srcCropBottom,
        void *dstBits,
        size_t dstWidth, size_t dstHeight,
        size_t dstCropLeft, size_t dstCropTop,
        size_t dstCropRight, size_t dstCropBottom) {

    //CallStack stack(LOG_TAG);
    if (mDstFormat != OMX_COLOR_Format16bitRGB565) {
        return ERROR_UNSUPPORTED;
    }

    size_t newSrcWidth =  0;
    size_t newSrcHeight =  0;
#ifdef DUMP_DECODE_FRAME // Dump decode frame
    static int thmubnailCovFd = 0;
    static int thmubnailCovCnt = 0;
    char OrgFileName[128];
    memset(OrgFileName, 0, 128);
    int ret = 0;
    int size = 0;
    if (1 || srcWidth == 1920)
    {
        ret = snprintf(OrgFileName, 128, "/cache/thumbmailCov_%d_%d_%d.422.yuv", srcWidth, srcHeight, thmubnailCovCnt);
    }
    else
        OrgFileName[0] = 0;
    if (OrgFileName[0] != 0)
    {
        ALOGD("\033[0;32m In %s:%d Src:WxH:%dx:%d L-R :%d:%d T-B: %d:%d Dst:%dx%d OrgFileName:%s thmubnailCovFd:%d \033[m", __FUNCTION__, __LINE__, srcWidth, srcHeight, srcCropLeft, srcCropRight, srcCropTop, srcCropBottom, dstWidth, dstHeight, OrgFileName, thmubnailCovFd);
        if (thmubnailCovFd == 0)
        {
            thmubnailCovFd = open(OrgFileName, O_RDWR| O_APPEND|O_CREAT, 0777); //in mediaserver, no permission to write /data or /storage
            if (thmubnailCovFd <= 0)
                ALOGD("== Open %s failed, %d-%s==", OrgFileName, errno, strerror(errno));
            else
                ALOGD("== Open %s OK,==", OrgFileName);
        }

        if(thmubnailCovFd >= 0){
            size = srcWidth * srcHeight * 3 / 2 ;
            ret = write(thmubnailCovFd, srcBits, size);
            ALOGD("----- In %s:%d Orig [%d] idx[%d] ret:%d ----", __FUNCTION__, __LINE__, size, thmubnailCovCnt, ret);
            if ( ret != size)
                ALOGD("\033[0;32m In %s:%d write frame failed %d-%s \033[m", __FUNCTION__, __LINE__, errno, strerror(errno));
            thmubnailCovCnt++;
        }

        if (thmubnailCovFd > 0 )
        {
            close(thmubnailCovFd);
            thmubnailCovFd = 0;
        }
        if(thmubnailCovCnt >= 360){
            thmubnailCovFd = -1;
        }
    }
#endif /* end of DUMP_DECODE_FRAME */
    if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W16_H16_YCbCr_420_SP)
    {
        newSrcWidth     = ALIGN16(srcWidth);
        newSrcHeight    = ALIGN16(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W32_H16_YCbCr_420_SP)
    {
        newSrcWidth     = ALIGN32(srcWidth);
        newSrcHeight    = ALIGN16(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W16_H32_YCbCr_420_SP)
    {
        newSrcWidth     = ALIGN16(srcWidth);
        newSrcHeight    = ALIGN32(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W32_H32_YCbCr_420_SP)
    {
        newSrcWidth     = ALIGN32(srcWidth);
        newSrcHeight    = ALIGN32(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W16_H8_YCbCr_420_SP)
    {
        newSrcWidth     = ALIGN16(srcWidth);
        newSrcHeight    = ALIGN8(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W16_H16_YCrCb_420_SP)
    {
        newSrcWidth     = ALIGN16(srcWidth);
        newSrcHeight    = ALIGN16(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W32_H16_YCrCb_420_SP)
    {
        newSrcWidth     = ALIGN32(srcWidth);
        newSrcHeight    = ALIGN16(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W16_H32_YCrCb_420_SP)
    {
        newSrcWidth     = ALIGN16(srcWidth);
        newSrcHeight    = ALIGN32(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W32_H32_YCrCb_420_SP)
    {
        newSrcWidth     = ALIGN32(srcWidth);
        newSrcHeight    = ALIGN32(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_W16_H8_YCrCb_420_SP)
    {
        newSrcWidth     = ALIGN16(srcWidth);
        newSrcHeight    = ALIGN8(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP)
    {
        newSrcWidth = ALIGN16(srcWidth);
        newSrcHeight = ALIGN16(srcHeight);
    }
    else if (mSrcFormat == (OMX_COLOR_FORMATTYPE)HAL_PIXEL_FORMAT_RTK_32_YCbCr_420_SP)
    {
        newSrcWidth = ALIGN32(srcWidth);
        newSrcHeight = ALIGN16(srcHeight);
    }
    else
    {
        newSrcWidth =  srcWidth;
        newSrcHeight = srcHeight;
    }

    BitmapParams src(
            const_cast<void *>(srcBits),
            newSrcWidth, newSrcHeight,
            srcCropLeft, srcCropTop, srcCropRight, srcCropBottom);

    BitmapParams dst(
            dstBits,
            dstWidth, dstHeight,
            dstCropLeft, dstCropTop, dstCropRight, dstCropBottom);

    status_t err;

    switch ((int)mSrcFormat) {
        case OMX_COLOR_FormatYUV420Planar:
#ifdef USE_LIBYUV
            err = convertYUV420PlanarUseLibYUV(src, dst);
#else
            err = convertYUV420Planar(src, dst);
#endif
            break;

        case OMX_COLOR_FormatCbYCrY:
            err = convertCbYCrY(src, dst);
            break;

        case OMX_QCOM_COLOR_FormatYVU420SemiPlanar:
            err = convertQCOMYUV420SemiPlanar(src, dst);
            break;

        case OMX_COLOR_FormatYUV420SemiPlanar:
        case HAL_PIXEL_FORMAT_YCbCr_420_SP: // workaround for bug 41956
        case HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_32_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H16_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H16_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H32_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H32_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H8_YCbCr_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H16_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H16_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H32_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W32_H32_YCrCb_420_SP:
        case HAL_PIXEL_FORMAT_RTK_W16_H8_YCrCb_420_SP:
            err = convertYUV420SemiPlanar(src, dst);
            break;

        case OMX_TI_COLOR_FormatYUV420PackedSemiPlanar:
            err = convertTIYUV420PackedSemiPlanar(src, dst);
            break;

        default:
        {
            CHECK(!"Should not be here. Unknown color conversion.");
            break;
        }
    }

    return err;
}

status_t ColorConverter::convertCbYCrY(
        const BitmapParams &src, const BitmapParams &dst) {
    // XXX Untested

    uint8_t *kAdjustedClip = initClip();

    if (!((src.mCropLeft & 1) == 0
        && src.cropWidth() == dst.cropWidth()
        && src.cropHeight() == dst.cropHeight())) {
        return ERROR_UNSUPPORTED;
    }

    uint16_t *dst_ptr = (uint16_t *)dst.mBits
        + dst.mCropTop * dst.mWidth + dst.mCropLeft;

    const uint8_t *src_ptr = (const uint8_t *)src.mBits
        + (src.mCropTop * dst.mWidth + src.mCropLeft) * 2;

    for (size_t y = 0; y < src.cropHeight(); ++y) {
        for (size_t x = 0; x < src.cropWidth(); x += 2) {
            signed y1 = (signed)src_ptr[2 * x + 1] - 16;
            signed y2 = (signed)src_ptr[2 * x + 3] - 16;
            signed u = (signed)src_ptr[2 * x] - 128;
            signed v = (signed)src_ptr[2 * x + 2] - 128;

            signed u_b = u * 517;
            signed u_g = -u * 100;
            signed v_g = -v * 208;
            signed v_r = v * 409;

            signed tmp1 = y1 * 298;
            signed b1 = (tmp1 + u_b) / 256;
            signed g1 = (tmp1 + v_g + u_g) / 256;
            signed r1 = (tmp1 + v_r) / 256;

            signed tmp2 = y2 * 298;
            signed b2 = (tmp2 + u_b) / 256;
            signed g2 = (tmp2 + v_g + u_g) / 256;
            signed r2 = (tmp2 + v_r) / 256;

            uint32_t rgb1 =
                ((kAdjustedClip[r1] >> 3) << 11)
                | ((kAdjustedClip[g1] >> 2) << 5)
                | (kAdjustedClip[b1] >> 3);

            uint32_t rgb2 =
                ((kAdjustedClip[r2] >> 3) << 11)
                | ((kAdjustedClip[g2] >> 2) << 5)
                | (kAdjustedClip[b2] >> 3);

            if (x + 1 < src.cropWidth()) {
                *(uint32_t *)(&dst_ptr[x]) = (rgb2 << 16) | rgb1;
            } else {
                dst_ptr[x] = rgb1;
            }
        }

        src_ptr += src.mWidth * 2;
        dst_ptr += dst.mWidth;
    }

    return OK;
}

status_t ColorConverter::convertYUV420PlanarUseLibYUV(
        const BitmapParams &src, const BitmapParams &dst) {
    if (!((src.mCropLeft & 1) == 0
            && src.cropWidth() == dst.cropWidth()
            && src.cropHeight() == dst.cropHeight())) {
        return ERROR_UNSUPPORTED;
    }

    uint16_t *dst_ptr = (uint16_t *)dst.mBits
        + dst.mCropTop * dst.mWidth + dst.mCropLeft;

    const uint8_t *src_y =
        (const uint8_t *)src.mBits + src.mCropTop * src.mWidth + src.mCropLeft;

    const uint8_t *src_u =
        (const uint8_t *)src_y + src.mWidth * src.mHeight
        + src.mCropTop * (src.mWidth / 2) + src.mCropLeft / 2;

    const uint8_t *src_v =
        src_u + (src.mWidth / 2) * (src.mHeight / 2);


    libyuv::I420ToRGB565(src_y, src.mWidth, src_u, src.mWidth / 2, src_v, src.mWidth / 2,
            (uint8 *)dst_ptr, dst.mWidth * 2, dst.mWidth, dst.mHeight);

    return OK;
}

status_t ColorConverter::convertYUV420Planar(
        const BitmapParams &src, const BitmapParams &dst) {
    if (!((src.mCropLeft & 1) == 0
            && src.cropWidth() == dst.cropWidth()
            && src.cropHeight() == dst.cropHeight())) {
        return ERROR_UNSUPPORTED;
    }

    uint8_t *kAdjustedClip = initClip();

    uint16_t *dst_ptr = (uint16_t *)dst.mBits
        + dst.mCropTop * dst.mWidth + dst.mCropLeft;

    const uint8_t *src_y =
        (const uint8_t *)src.mBits + src.mCropTop * src.mWidth + src.mCropLeft;

    const uint8_t *src_u =
        (const uint8_t *)src_y + src.mWidth * src.mHeight
        + src.mCropTop * (src.mWidth / 2) + src.mCropLeft / 2;

    const uint8_t *src_v =
        src_u + (src.mWidth / 2) * (src.mHeight / 2);

    //ALOGD("%s%d src.cropWidth()=%d src.cropHeight()=%d\n", __func__, __LINE__, src.cropWidth(), src.cropHeight());
    _DBG_CheckTime_Init()
    _DBG_CheckTime_Begin()

#ifdef LIBYUV_NEON
    _I420ToRGB565(src_y, src.mWidth, src_u, (src.mWidth / 2), src_v, (src.mWidth / 2), 
            (uint8*)dst_ptr, dst.mWidth*2, dst.mWidth, dst.mHeight);
#else
    for (size_t y = 0; y < src.cropHeight(); ++y) {
        for (size_t x = 0; x < src.cropWidth(); x += 2) {
            // B = 1.164 * (Y - 16) + 2.018 * (U - 128)
            // G = 1.164 * (Y - 16) - 0.813 * (V - 128) - 0.391 * (U - 128)
            // R = 1.164 * (Y - 16) + 1.596 * (V - 128)

            // B = 298/256 * (Y - 16) + 517/256 * (U - 128)
            // G = .................. - 208/256 * (V - 128) - 100/256 * (U - 128)
            // R = .................. + 409/256 * (V - 128)

            // min_B = (298 * (- 16) + 517 * (- 128)) / 256 = -277
            // min_G = (298 * (- 16) - 208 * (255 - 128) - 100 * (255 - 128)) / 256 = -172
            // min_R = (298 * (- 16) + 409 * (- 128)) / 256 = -223

            // max_B = (298 * (255 - 16) + 517 * (255 - 128)) / 256 = 534
            // max_G = (298 * (255 - 16) - 208 * (- 128) - 100 * (- 128)) / 256 = 432
            // max_R = (298 * (255 - 16) + 409 * (255 - 128)) / 256 = 481

            // clip range -278 .. 535

            signed y1 = (signed)src_y[x] - 16;
            signed y2 = (signed)src_y[x + 1] - 16;

            signed u = (signed)src_u[x / 2] - 128;
            signed v = (signed)src_v[x / 2] - 128;

            signed u_b = u * 517;
            signed u_g = -u * 100;
            signed v_g = -v * 208;
            signed v_r = v * 409;

            signed tmp1 = y1 * 298;
            signed b1 = (tmp1 + u_b) / 256;
            signed g1 = (tmp1 + v_g + u_g) / 256;
            signed r1 = (tmp1 + v_r) / 256;

            signed tmp2 = y2 * 298;
            signed b2 = (tmp2 + u_b) / 256;
            signed g2 = (tmp2 + v_g + u_g) / 256;
            signed r2 = (tmp2 + v_r) / 256;

            uint32_t rgb1 =
                ((kAdjustedClip[r1] >> 3) << 11)
                | ((kAdjustedClip[g1] >> 2) << 5)
                | (kAdjustedClip[b1] >> 3);

            uint32_t rgb2 =
                ((kAdjustedClip[r2] >> 3) << 11)
                | ((kAdjustedClip[g2] >> 2) << 5)
                | (kAdjustedClip[b2] >> 3);

            if (x + 1 < src.cropWidth()) {
                *(uint32_t *)(&dst_ptr[x]) = (rgb2 << 16) | rgb1;
            } else {
                dst_ptr[x] = rgb1;
            }
        }

        src_y += src.mWidth;

        if (y & 1) {
            src_u += src.mWidth / 2;
            src_v += src.mWidth / 2;
        }

        dst_ptr += dst.mWidth;
    }
#endif

    _DBG_CheckTime_End()

    return OK;
}

status_t ColorConverter::convertQCOMYUV420SemiPlanar(
        const BitmapParams &src, const BitmapParams &dst) {
    uint8_t *kAdjustedClip = initClip();

    if (!((src.mCropLeft & 1) == 0
            && src.cropWidth() == dst.cropWidth()
            && src.cropHeight() == dst.cropHeight())) {
        return ERROR_UNSUPPORTED;
    }

    uint16_t *dst_ptr = (uint16_t *)dst.mBits
        + dst.mCropTop * dst.mWidth + dst.mCropLeft;

    const uint8_t *src_y =
        (const uint8_t *)src.mBits + src.mCropTop * src.mWidth + src.mCropLeft;

    const uint8_t *src_u =
        (const uint8_t *)src_y + src.mWidth * src.mHeight
        + src.mCropTop * src.mWidth + src.mCropLeft;

    for (size_t y = 0; y < src.cropHeight(); ++y) {
        for (size_t x = 0; x < src.cropWidth(); x += 2) {
            signed y1 = (signed)src_y[x] - 16;
            signed y2 = (signed)src_y[x + 1] - 16;

            signed u = (signed)src_u[x & ~1] - 128;
            signed v = (signed)src_u[(x & ~1) + 1] - 128;

            signed u_b = u * 517;
            signed u_g = -u * 100;
            signed v_g = -v * 208;
            signed v_r = v * 409;

            signed tmp1 = y1 * 298;
            signed b1 = (tmp1 + u_b) / 256;
            signed g1 = (tmp1 + v_g + u_g) / 256;
            signed r1 = (tmp1 + v_r) / 256;

            signed tmp2 = y2 * 298;
            signed b2 = (tmp2 + u_b) / 256;
            signed g2 = (tmp2 + v_g + u_g) / 256;
            signed r2 = (tmp2 + v_r) / 256;

            uint32_t rgb1 =
                ((kAdjustedClip[b1] >> 3) << 11)
                | ((kAdjustedClip[g1] >> 2) << 5)
                | (kAdjustedClip[r1] >> 3);

            uint32_t rgb2 =
                ((kAdjustedClip[b2] >> 3) << 11)
                | ((kAdjustedClip[g2] >> 2) << 5)
                | (kAdjustedClip[r2] >> 3);

            if (x + 1 < src.cropWidth()) {
                *(uint32_t *)(&dst_ptr[x]) = (rgb2 << 16) | rgb1;
            } else {
                dst_ptr[x] = rgb1;
            }
        }

        src_y += src.mWidth;

        if (y & 1) {
            src_u += src.mWidth;
        }

        dst_ptr += dst.mWidth;
    }

    return OK;
}

status_t ColorConverter::convertYUV420SemiPlanar(
        const BitmapParams &src, const BitmapParams &dst) {
    // XXX Untested

    uint8_t *kAdjustedClip = initClip();

    if (!((src.mCropLeft & 1) == 0
            && src.cropWidth() == dst.cropWidth()
            && src.cropHeight() == dst.cropHeight())) {
        return ERROR_UNSUPPORTED;
    }

#ifdef DUMP_DECODE_FRAME // Dump decode frame
    static int thmubnailOrgFd = 0;
    static int thmubnailDstFd = 0;
    static int thmubnailCnt = 0;
    char OrgFileName[128];
    char DstFileName[128];
    memset(OrgFileName, 0, 128);
    memset(DstFileName, 0, 128);
    int ret = 0;
    int size = 0;
    if (1 || src.mWidth == 1920)
    {
        ret = snprintf(OrgFileName, 128, "/cache/thumbmailOrg_%d_%d_%d.422.yuv", src.mWidth, src.mHeight, thmubnailCnt);
        ret = snprintf(DstFileName, 128, "/cache/thumbmailDst_%d_%d_%d.422.yuv", dst.mWidth, dst.mHeight, thmubnailCnt);
    }
    else
        OrgFileName[0] = 0;
    if (OrgFileName[0] != 0)
    {
        ALOGD("\033[0;32m In %s:%d Src:WxH:%dx:%d L-R :%d:%d T-B: %d:%d Dst:%dx%d OrgFileName:%s DstFileName:%s thmubnailOrgFd:%d thmubnailDstFd:%d \033[m", __FUNCTION__, __LINE__, src.mWidth, src.mHeight, src.mCropLeft, src.mCropRight, src.mCropTop, src.mCropBottom, dst.mWidth, dst.mHeight, OrgFileName, DstFileName, thmubnailOrgFd, thmubnailDstFd);
        if (thmubnailOrgFd == 0)
        {
            thmubnailOrgFd = open(OrgFileName, O_RDWR| O_APPEND|O_CREAT, 0777); //in mediaserver, no permission to write /data or /storage
            if (thmubnailOrgFd <= 0)
                ALOGD("== Open %s failed, %d-%s==", OrgFileName, errno, strerror(errno));
            else
                ALOGD("== Open %s OK,==", OrgFileName);
        }
        if (thmubnailDstFd == 0)
        {
            thmubnailDstFd = open(DstFileName, O_RDWR|O_APPEND|O_CREAT, 0777); //in mediaserver, no permission to write /data or /storage
            if (thmubnailDstFd <= 0)
                ALOGD("== Open %s failed, %d-%s==", DstFileName, errno, strerror(errno));
            else
                ALOGD("== Open %s OK,==", DstFileName);
        }

        if(thmubnailOrgFd >= 0){
            size = src.mWidth*src.mWidth*3/2;
            ret = write(thmubnailOrgFd, src.mBits, size);
            ALOGD("----- In %s:%d Orig [%d] idx[%d] ret:%d ----", __FUNCTION__, __LINE__, size, thmubnailCnt, ret);
            if ( ret != size)
                ALOGD("\033[0;32m In %s:%d write frame failed %d-%s \033[m", __FUNCTION__, __LINE__, errno, strerror(errno));
            thmubnailCnt++;
        }
        if (thmubnailOrgFd > 0 )
        {
            close(thmubnailOrgFd);
            thmubnailOrgFd = 0;
        }
        if (thmubnailDstFd > 0)
        {
            close(thmubnailDstFd);
            thmubnailDstFd = 0;
        }
        if(thmubnailCnt >= 360){
            thmubnailOrgFd = -1;
            thmubnailDstFd = -1;
        }
    }
#endif /* end of DUMP_DECODE_FRAME */

    uint16_t *dst_ptr = (uint16_t *)dst.mBits
        + dst.mCropTop * dst.mWidth + dst.mCropLeft;

    const uint8_t *src_y =
        (const uint8_t *)src.mBits + src.mCropTop * src.mWidth + src.mCropLeft;

    const uint8_t *src_u =
        (const uint8_t *)src_y + src.mWidth * src.mHeight
        + src.mCropTop * src.mWidth + src.mCropLeft;

    //ALOGD("%s%d src.cropWidth()=%d src.cropHeight()=%d\n", __func__, __LINE__, src.cropWidth(), src.cropHeight());
    _DBG_CheckTime_Init()
    _DBG_CheckTime_Begin()

#ifdef LIBYUV_NEON
    _NV12ToRGB565(src_y, src.mWidth, 
                        src_u, src.mWidth,
                        (uint8*)dst_ptr, dst.mWidth*2,
                        dst.mWidth, dst.mHeight);
#else
    for (size_t y = 0; y < src.cropHeight(); ++y) {
        for (size_t x = 0; x < src.cropWidth(); x += 2) {
            signed y1 = (signed)src_y[x] - 16;
            signed y2 = (signed)src_y[x + 1] - 16;

            signed v = (signed)src_u[x & ~1] - 128;
            signed u = (signed)src_u[(x & ~1) + 1] - 128;

            signed u_b = u * 517;
            signed u_g = -u * 100;
            signed v_g = -v * 208;
            signed v_r = v * 409;

            signed tmp1 = y1 * 298;
            signed b1 = (tmp1 + u_b) / 256;
            signed g1 = (tmp1 + v_g + u_g) / 256;
            signed r1 = (tmp1 + v_r) / 256;

            signed tmp2 = y2 * 298;
            signed b2 = (tmp2 + u_b) / 256;
            signed g2 = (tmp2 + v_g + u_g) / 256;
            signed r2 = (tmp2 + v_r) / 256;

            uint32_t rgb1 =
                ((kAdjustedClip[b1] >> 3) << 11)
                | ((kAdjustedClip[g1] >> 2) << 5)
                | (kAdjustedClip[r1] >> 3);

            uint32_t rgb2 =
                ((kAdjustedClip[b2] >> 3) << 11)
                | ((kAdjustedClip[g2] >> 2) << 5)
                | (kAdjustedClip[r2] >> 3);

            if (x + 1 < src.cropWidth()) {
                *(uint32_t *)(&dst_ptr[x]) = (rgb2 << 16) | rgb1;
            } else {
                dst_ptr[x] = rgb1;
            }
        }

        src_y += src.mWidth;

        if (y & 1) {
            src_u += src.mWidth;
        }

        dst_ptr += dst.mWidth;
    }
#endif

    _DBG_CheckTime_End()

    return OK;
}

status_t ColorConverter::convertTIYUV420PackedSemiPlanar(
        const BitmapParams &src, const BitmapParams &dst) {
    uint8_t *kAdjustedClip = initClip();

    if (!((src.mCropLeft & 1) == 0
            && src.cropWidth() == dst.cropWidth()
            && src.cropHeight() == dst.cropHeight())) {
        return ERROR_UNSUPPORTED;
    }

    uint16_t *dst_ptr = (uint16_t *)dst.mBits
        + dst.mCropTop * dst.mWidth + dst.mCropLeft;

    const uint8_t *src_y = (const uint8_t *)src.mBits;

    const uint8_t *src_u =
        (const uint8_t *)src_y + src.mWidth * (src.mHeight - src.mCropTop / 2);

    for (size_t y = 0; y < src.cropHeight(); ++y) {
        for (size_t x = 0; x < src.cropWidth(); x += 2) {
            signed y1 = (signed)src_y[x] - 16;
            signed y2 = (signed)src_y[x + 1] - 16;

            signed u = (signed)src_u[x & ~1] - 128;
            signed v = (signed)src_u[(x & ~1) + 1] - 128;

            signed u_b = u * 517;
            signed u_g = -u * 100;
            signed v_g = -v * 208;
            signed v_r = v * 409;

            signed tmp1 = y1 * 298;
            signed b1 = (tmp1 + u_b) / 256;
            signed g1 = (tmp1 + v_g + u_g) / 256;
            signed r1 = (tmp1 + v_r) / 256;

            signed tmp2 = y2 * 298;
            signed b2 = (tmp2 + u_b) / 256;
            signed g2 = (tmp2 + v_g + u_g) / 256;
            signed r2 = (tmp2 + v_r) / 256;

            uint32_t rgb1 =
                ((kAdjustedClip[r1] >> 3) << 11)
                | ((kAdjustedClip[g1] >> 2) << 5)
                | (kAdjustedClip[b1] >> 3);

            uint32_t rgb2 =
                ((kAdjustedClip[r2] >> 3) << 11)
                | ((kAdjustedClip[g2] >> 2) << 5)
                | (kAdjustedClip[b2] >> 3);

            if (x + 1 < src.cropWidth()) {
                *(uint32_t *)(&dst_ptr[x]) = (rgb2 << 16) | rgb1;
            } else {
                dst_ptr[x] = rgb1;
            }
        }

        src_y += src.mWidth;

        if (y & 1) {
            src_u += src.mWidth;
        }

        dst_ptr += dst.mWidth;
    }

    return OK;
}

uint8_t *ColorConverter::initClip() {
    static const signed kClipMin = -278;
    static const signed kClipMax = 535;

    if (mClip == NULL) {
        mClip = new uint8_t[kClipMax - kClipMin + 1];

        for (signed i = kClipMin; i <= kClipMax; ++i) {
            mClip[i - kClipMin] = (i < 0) ? 0 : (i > 255) ? 255 : (uint8_t)i;
        }
    }

    return &mClip[-kClipMin];
}

}  // namespace android
