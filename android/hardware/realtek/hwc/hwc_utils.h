#ifndef __RTK_HWC_UTILS_H_
#define __RTK_HWC_UTILS_H_
#include <hardware/hwcomposer.h>
#include <cutils/log.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <math.h>

#include <cstring>
#include <utils/String8.h>
#include <utils/Timers.h>
#include <cutils/properties.h>
#include <cutils/log.h>

#ifdef USE_RTK_VOUT_UTIL
#include <VideoRPCBaseDS_data.h>
#include <VideoRPC_System_data.h>
#include <VideoRPCBaseDS_data.h>
#include <RPCBaseDS_data.h>
#include <VoutUtil.h>
#endif

#include <sync/sync.h>
#include <sw_sync.h>

inline int WIDTH (const hwc_rect_t & rect)    { return rect.right - rect.left; }
inline int HEIGHT(const hwc_rect_t & rect)   { return rect.bottom - rect.top; }

#define DC2VO_IOC_MAGIC         'd'
#define DC2VO_WAIT_FOR_VSYNC    _IO (DC2VO_IOC_MAGIC,  0)

typedef struct {
    unsigned int enable ;
    unsigned int width ;
    unsigned int height ;
    unsigned int reserve[16-3];
} DC_BUFFER_INFO ;
#define DC2VO_SET_BUFFER_INFO            _IO    (DC2VO_IOC_MAGIC, 18)

#endif /* End of __RTK_HWC_UTILS_H_ */
