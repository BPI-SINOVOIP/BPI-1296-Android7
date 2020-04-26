#ifndef _RTK_HWM_LOG_H
#define _RTK_HWM_LOG_H

#include<cutils/properties.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum Rtk_HWM_LogPriority {
	RTK_HWM_LOG_UNKNOWN = 0,
    RTK_HWM_LOG_DEFAULT,    /* only for SetMinPriority() */
    RTK_HWM_LOG_VERBOSE,
    RTK_HWM_LOG_DEBUG,
    RTK_HWM_LOG_INFO,
    RTK_HWM_LOG_WARN,
    RTK_HWM_LOG_ERROR,
    RTK_HWM_LOG_FATAL,
    RTK_HWM_LOG_SILENT,     /* only for SetMinPriority(); must be last */
} Rtk_HWM_LogPriority;

#ifndef RTK_HWM_LOGV
#define RTK_HWM_LOGV(...) __android_log_print(RTK_HWM_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#endif

#ifndef RTK_HWM_LOGD
#define RTK_HWM_LOGD(...) __android_log_print(RTK_HWM_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

#ifndef RTK_HWM_LOGI
#define RTK_HWM_LOGI(...) __android_log_print(RTK_HWM_LOG_INFO, LOG_TAG, __VA_ARGS__)
#endif

#ifndef RTK_HWM_LOGW
#define RTK_HWM_LOGW(...) __android_log_print(RTK_HWM_LOG_WARN, LOG_TAG, __VA_ARGS__)
#endif

#ifndef RTK_HWM_LOGE
#define RTK_HWM_LOGE(...) __android_log_print(RTK_HWM_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#endif

#ifndef RTK_HWM_LOGF
#define RTK_HWM_LOGF(...) __android_log_print(RTK_HWM_LOG_FATAL, LOG_TAG, __VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif

#endif
