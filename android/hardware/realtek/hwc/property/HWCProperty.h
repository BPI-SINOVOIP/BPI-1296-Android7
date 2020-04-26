#ifndef __RTK_HWC_PROPERTY_H_
#define __RTK_HWC_PROPERTY_H_
#include <hwc_utils.h>

#define HWC_PROPERTY_V1_X                           "rtk.hwc.v1.x"
#define HWC_PROPERTY_V1_Y                           "rtk.hwc.v1.y"
#define HWC_PROPERTY_V1_WIDTH                       "rtk.hwc.v1.width"
#define HWC_PROPERTY_V1_HEIGHT                      "rtk.hwc.v1.height"
#define HWC_PROPERTY_V1_RESET                       "rtk.hwc.v1.reset"
#define HWC_PROPERTY_V1_FREEZE                      "rtk.hwc.v1.freeze"
#define HWC_PROPERTY_V1_SCALE_BYPASS                "rtk.hwc.v1.scale.bypass"

#define HWC_PROPERTY_V2_X                           "rtk.hwc.v2.x"
#define HWC_PROPERTY_V2_Y                           "rtk.hwc.v2.y"
#define HWC_PROPERTY_V2_WIDTH                       "rtk.hwc.v2.width"
#define HWC_PROPERTY_V2_HEIGHT                      "rtk.hwc.v2.height"
#define HWC_PROPERTY_V2_RESET                       "rtk.hwc.v2.reset"
#define HWC_PROPERTY_V2_SCALE_BYPASS                "rtk.hwc.v2.scale.bypass"

#define HWC_PROPERTY_V1_FORCE_ENABLE                "rtk.hwc.v1.force.enable"
#define HWC_PROPERTY_V1_FORCE_FULLSCREEN            "rtk.hwc.v1.force.fullscreen"
#define HWC_PROPERTY_V1_FORCE_RESET                 "rtk.hwc.v1.force.reset"
#define HWC_PROPERTY_V1_FORCE_X                     "rtk.hwc.v1.force.x"
#define HWC_PROPERTY_V1_FORCE_Y                     "rtk.hwc.v1.force.y"
#define HWC_PROPERTY_V1_FORCE_WIDTH                 "rtk.hwc.v1.force.width"
#define HWC_PROPERTY_V1_FORCE_HEIGHT                "rtk.hwc.v1.force.height"

#define HWC_PROPERTY_V1_CROP_LEFT                   "rtk.hwc.v1.crop.left"
#define HWC_PROPERTY_V1_CROP_TOP                    "rtk.hwc.v1.crop.top"
#define HWC_PROPERTY_V1_CROP_RIGHT                  "rtk.hwc.v1.crop.right"
#define HWC_PROPERTY_V1_CROP_BOTTOM                 "rtk.hwc.v1.crop.bottom"

#define HWC_PROPERTY_V2_CROP_LEFT                   "rtk.hwc.v2.crop.left"
#define HWC_PROPERTY_V2_CROP_TOP                    "rtk.hwc.v2.crop.top"
#define HWC_PROPERTY_V2_CROP_RIGHT                  "rtk.hwc.v2.crop.right"
#define HWC_PROPERTY_V2_CROP_BOTTOM                 "rtk.hwc.v2.crop.bottom"

#define HWC_PROPERTY_VIDEO_MAGNIFICATION            "rtk.hwc.video.magnification"

#define HWC_PROPERTY_V1_FLIP                        "rtk.hwc.support.v1.flip"
#define HWC_PROPERTY_V2_FLIP                        "rtk.hwc.support.v2.flip"
#define HWC_PROPERTY_V1_FLIP_BUFFER_CNT             "rtk.hwc.v1.flip.buffer"

#define HWC_PROPERTY_SECODE_DISPLAY                 "persist.rtk.hdmitx.dpdiffsource"
#define HWC_PROPERTY_DISABLE_VO_WRITEBACK           "rtk.hwc.disable.vo.writeback"
#define HWC_PROPERTY_DISPLAY_FLAGS                  "persist.rtk.hwc.display.flags"
#define HWC_PROPERTY_DISPLAY_FLAGS_VIRTUAL_SKIP     (0x1 << 0)

#define HWC_PROPERTY_MIXER_CONTROL_DISABLE          "rtk.hwc.mixer.ctl.disable"

/* DP_PRIMARY default by SECODE_DISPLAY */
#define HWC_PROPERTY_DP_PRIMARY                     "persist.rtk.hwc.dp.primary"

#define HWC_PROPERTY_PLANE_V1                       0
#define HWC_PROPERTY_PLANE_V2                       1

#if 0 // unused
#define HWC_PROPERTY_SCALE_PLANE                    "rtk.hwc.scale.plane" /* 0:V1 1:V2 other:V1*/

#define HWC_PROPERTY_LINEAR_PLAVE_DEF               HWC_PROPERTY_PLANE_V1
#define HWC_PROPERTY_LINEAR_SETUP                   "persist.rtk.hwc.linear.setup"
#define HWC_PROPERTY_LINEAR_PLAVE                   "rtk.hwc.linear.plane" /* 0:V1 1:V2 other:V1 */

#define HWC_PROPERTY_LINEAR_PLAVE_EN                "rtk.hwc.linear.plane.en" /* 0:disable 1:enable */
#endif

#define HWC_PROPERTY_OSD_HIDE                       "rtk.hwc.osd.hide"
//#define HWC_PROPERTY_OSD_HIDE_DELAY                 "rtk.hwc.osd.hide.delay.ms"
#define HWC_PROPERTY_OSD_TOUT                       "rtk.hwc.osd.hide.ms"

#define HWC_PROPERTY_OSD_WIDTH                      "rtk.hwc.osd.w"
#define HWC_PROPERTY_OSD_HEIGHT                     "rtk.hwc.osd.h"

#define HWC_PROPERTY_SUSPEND_CTL                    "rtk.hwc.suspend.ctl"
#define HWC_PROPERTY_SUSPEND_DELAY_MS               "rtk.hwc.suspend.delay.ms"
#define HWC_PROPERTY_SUSPEND_WAIT_ENABLE            "rtk.hwc.suspend.wait.enable"
//#define HWC_PROPERTY_SUSPEND_WAIT_LOCK              "rtk.hwc.suspend.wait.lock"     /* 0: release state 1: suspend start 2: lock 3: unlock */
//#define HWC_PROPERTY_SUSPEND_BMP                    "rtk.hwc.suspend.bmp"
#define HWC_PROPERTY_SUSPEND_WATCHDOG_EN            "rtk.hwc.suspend.watchdog.en"
#define HWC_PROPERTY_SUSPEND_WATCHDOG_SEC           "rtk.hwc.suspend.watchdog.sec"

#define HWC_PROPERTY_CTS_TEST                       "persist.rtk.ctstest"

#define HWC_PROPERTY_VIDEO_MAX_ACQUIRED_COUNT       "persist.rtk.video.maxAcqCnt"

#define HWC_PROPERTY_CURSOR_SUPPORT                 "rtk.hwc.support.cursor"
#define HWC_PROPERTY_VIDEO_STANDARD                 "persist.rtk.display.standard"

#define HWC_PROPERTY_EXTERNAL_FORCE_WIDTH           "persist.rtk.hwc.ext.force.w"
#define HWC_PROPERTY_EXTERNAL_FORCE_HEIGHT          "persist.rtk.hwc.ext.force.h"
#define HWC_PROPERTY_RANDOM_VALUE                   "rtk.hwc.random.value"

class HWCProperty {
public:
    HWCProperty();
    virtual ~HWCProperty() {};

    bool        get_hwc_v1_force_enable(void);
    bool        get_hwc_v1_force_reset(void);
    bool        get_hwc_v1_force_fullscreen(void);
    void        clean_hwc_v1_force_enable(void);
    void        clean_hwc_v1_force_reset(void);
    void        clean_hwc_v1_force_fullscreen(void);
    hwc_rect_t  get_hwc_v1_force_rect(void);
    void        set_hwc_v1_force_rect(hwc_rect_t rect);
    void        clean_hwc_v1_force_rect(void);
    bool        get_hwc_v1_reset(void);
    void        clean_hwc_v1_reset(void);
    bool        get_hwc_v2_reset(void);
    void        clean_hwc_v2_reset(void);
    void        set_hwc_v1_frame(int x, int y, int w, int h);
    bool        get_hwc_v1_scale_bypass();
    void        set_hwc_v2_frame(int x, int y, int w, int h);
    bool        get_hwc_v2_scale_bypass();
    bool        get_hwc_v1_flip_support(void);
    void        init_hwc_v1_flip_support(void);
    int         get_hwc_v1_flip_buffer_count(void);
    void        init_hwc_v1_flip_buffer_count(void);
    int         get_hwc_v1_flip_freeze(void);
    void        clean_hwc_v1_flip_freeze(void);
    hwc_rect_t  get_hwc_v1_crop(void);
    void        clean_hwc_v1_crop(void);
    hwc_rect_t  get_hwc_v2_crop(void);
    void        clean_hwc_v2_crop(void);
    hwc_rect_t  get_hwc_osd_resolution();
    bool        get_hwc_osd_hide_enable();
    bool        get_hwc_osd_hide_continue();
    int         get_hwc_osd_hide_timeout_ms();
    void        set_hwc_osd_hide_timeout_ms(int timeoutMs);
    void        clear_hwc_osd_hide_enable();
    void        set_hwc_osd_hide_continue();
    bool        get_hwc_second_display_support(void);
    bool        get_hwc_dp_is_primary_display(void);
    void        disable_hwc_vo_writeback_support(bool enable);
    int         get_hwc_suspend_watchdog_sec(void);
    bool        get_hwc_suspend_watchdog_en(void);
    void        clean_hwc_suspend_watchdog_en(void);
    int         get_hwc_suspend_delay_ms(void);
    void        set_hwc_suspend_delay_ms(int delayMs);
    int         get_hwc_suspend_ctl(void);
    void        set_hwc_suspend_ctl(int ctl);
    bool        get_hwc_cts_test(void);
    bool        get_hwc_cursor_support(void);
    void        set_hwc_cursor_support(bool enable);
    int         get_hwc_video_max_acquired_count(void);
    int         get_hwc_video_standard(void);
    bool        get_hwc_v2_flip_support(void);
    void        init_hwc_v2_flip_support(void);
    hwc_rect_t  get_hwc_external_force_size(void);
    int         get_hwc_display_flags(void);
    bool        get_hwc_display_flags_virtual_skip(void);
    void        set_hwc_random_value(char *value);
    int         get_hwc_video_magnification(void);
    bool        get_hwc_mixer_control_disable(void);
#if 0
    void        set_hwc_scale_plane(int plane);
    int         get_hwc_scale_plane(void);
    void        init_hwc_linear_plane(void);
    int         get_hwc_linear_plane(void);
    void        set_hwc_linear_plane_enable(bool enable);
    bool        get_hwc_linear_plane_enable(void);
#endif
};

#endif /* End of __RTK_HWC_PROPERTY_H_ */
