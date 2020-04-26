#ifndef _CONVERT_WRAPPER_H_
#define _CONVERT_WRAPPER_H_

#include <libyuv/row.h>

namespace android {
#ifdef HAS_NV12TORGB565ROW_NEON
int _NV12ToRGB565(const uint8* src_y, int src_stride_y,
                 const uint8* src_uv, int src_stride_uv,
                 uint8* dst_rgb565, int dst_stride_rgb565,
                 int width, int height);
#endif
#ifdef HAS_I422TOARGBROW_NEON
int _I420ToRGB565(const uint8* src_y, int src_stride_y,
                 const uint8* src_u, int src_stride_u,
                 const uint8* src_v, int src_stride_v,
                 uint8* dst_rgb565, int dst_stride_rgb565,
                 int width, int height);
#endif

} //namespace android

#endif
