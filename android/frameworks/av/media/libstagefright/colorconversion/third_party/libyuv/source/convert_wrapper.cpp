#include <libyuv.h>
#include <libyuv/row.h>
using namespace libyuv;

namespace android {

int _NV12ToRGB565(const uint8* src_y, int src_stride_y,
                 const uint8* src_uv, int src_stride_uv,
                 uint8* dst_rgb565, int dst_stride_rgb565,
                 int width, int height) {

  int y;
  void (*NV12ToRGB565Row)(const uint8* y_buf,
                          const uint8* uv_buf,
                          uint8* rgb_buf,
                          int width) = NV12ToRGB565Row_C;

  if (!src_y || !src_uv || !dst_rgb565 ||
      width <= 0 || height == 0) {
    return -1;
  }

  // Negative height means invert the image.
  if (height < 0) {
    height = -height;
    dst_rgb565 = dst_rgb565 + (height - 1) * dst_stride_rgb565;
    dst_stride_rgb565 = -dst_stride_rgb565;
  }

  if (width >= 8) {
    NV12ToRGB565Row = NV12ToRGB565Row_Any_NEON;
    if (IS_ALIGNED(width, 8)) {
      NV12ToRGB565Row = NV12ToRGB565Row_NEON;
    }
  }

  for (y = 0; y < height; ++y) {
    NV12ToRGB565Row(src_y, src_uv, dst_rgb565, width);
    dst_rgb565 += dst_stride_rgb565;
    src_y += src_stride_y;
    if (y & 1) {
      src_uv += src_stride_uv;
    }
  }

  return 0;
}

int _I420ToRGB565(const uint8* src_y, int src_stride_y,
                 const uint8* src_u, int src_stride_u,
                 const uint8* src_v, int src_stride_v,
                 uint8* dst_rgb565, int dst_stride_rgb565,
                 int width, int height) {
  int y;
  void (*I422ToRGB565Row)(const uint8* y_buf,
                          const uint8* u_buf,
                          const uint8* v_buf,
                          uint8* rgb_buf,
                          int width) = I422ToRGB565Row_C;
  if (!src_y || !src_u || !src_v || !dst_rgb565 ||
      width <= 0 || height == 0) {
    return -1;
  }
  // Negative height means invert the image.
  if (height < 0) {
    height = -height;
    dst_rgb565 = dst_rgb565 + (height - 1) * dst_stride_rgb565;
    dst_stride_rgb565 = -dst_stride_rgb565;
  }
  if (width >= 8) {
    I422ToRGB565Row = I422ToRGB565Row_Any_NEON;
    if (IS_ALIGNED(width, 8)) {
      I422ToRGB565Row = I422ToRGB565Row_NEON;
    }
  }

  for (y = 0; y < height; ++y) {
    I422ToRGB565Row(src_y, src_u, src_v, dst_rgb565, width);
    dst_rgb565 += dst_stride_rgb565;
    src_y += src_stride_y;
    if (y & 1) {
      src_u += src_stride_u;
      src_v += src_stride_v;
    }
  }
  return 0;
}

}
