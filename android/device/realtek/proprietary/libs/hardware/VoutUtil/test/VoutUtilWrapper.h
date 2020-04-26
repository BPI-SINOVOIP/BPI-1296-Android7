#ifndef _VOUT_UTIL_WRAPPER_H_
#define _VOUT_UTIL_WRAPPER_H_
#include <Application/AppClass/VoutUtil.h>
#include "ErrorDef.h"

#define CALL(method, ...) Invoke_##method(pVoutUtil, ##__VA_ARGS__)

extern VoutUtil *pVoutUtil;
void createVOUtil();

HRESULT Invoke_SetVideoStandard(VoutUtil *pVoutUtil,
                                enum VO_STANDARD standard,
                                u_char  enProg,
                                u_char  enDIF,
                                u_char  enCompRGB,
                                enum VO_PEDESTAL_TYPE pedType,
                                u_int   dataInt0,
                                u_int   dataInt1,
                                bool    bHdmi,
                                int     format3d,
                                VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig,
                                VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *nandTvSystemConfig,
                                bool bUpdateVideo);

HRESULT Invoke_SetTvSystem(VoutUtil *pVoutUtil,
                           VIDEO_RPC_VOUT_CONFIG_VIDEO_STANDARD structSetVideoStandard,
                           bool bUpdateHdmi,
                           ENUM_VIDEO_SYSTEM hdmiSystem,
                           u_char enProg,
                           VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *tvSystemConfig = NULL,
                           VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *nandTvSystemConfig = NULL,
                           bool bUpdateVideo = true);

HRESULT Invoke_SetTVtype(VoutUtil *pVoutUtil,
                         VO_TV_TYPE tvType);

HRESULT Invoke_SetBackground(VoutUtil *pVoutUtil,
                             struct VO_COLOR bgColor,
                             u_char bgEnable);

HRESULT Invoke_SetClosedCaption(VoutUtil *pVoutUtil,
                                u_char enCC_odd,
                                u_char enCC_even);

HRESULT Invoke_SetAPS(VoutUtil *pVoutUtil,
                      u_char enExt,
                      enum VO_VBI_APS APS);

HRESULT Invoke_SetCopyMode(VoutUtil *pVoutUtil,
                           u_char enExt,
                           enum VO_VBI_COPY_MODE copyMode);

HRESULT Invoke_SetAspectRatio(VoutUtil *pVoutUtil,
                              u_char enExt,
                              enum VO_VBI_ASPECT_RATIO aspectRatio);

HRESULT Invoke_SetV2alpha(VoutUtil *pVoutUtil,
                          u_char v2Alpha);

HRESULT Invoke_SetRescaleMode(VoutUtil *pVoutUtil,
                              enum VO_VIDEO_PLANE videoPlane,
                              enum VO_RESCALE_MODE rescaleMode,
                              struct VO_RECTANGLE rescaleWindow);

HRESULT Invoke_SetDeintMode(VoutUtil *pVoutUtil,
                            enum VO_VIDEO_PLANE videoPlane,
                            enum VO_DEINT_MODE deintMode);

HRESULT Invoke_SetPeakingStrength(VoutUtil *pVoutUtil,
                                  u_char peakingStrength);

HRESULT Invoke_SetBrightness(VoutUtil *pVoutUtil,
                             u_char brightness);

HRESULT Invoke_SetContrast(VoutUtil *pVoutUtil,
                           u_char contrast);

HRESULT Invoke_SetHue(VoutUtil *pVoutUtil,
                      u_char hue);

HRESULT Invoke_SetSaturation(VoutUtil *pVoutUtil,
                             u_char saturation);

HRESULT Invoke_GetTvDim(VoutUtil *pVoutUtil,
                        VO_STANDARD standard,
                        int *pwidth,
                        int *pheight);

HRESULT Invoke_SetSubtitleYoffset(VoutUtil *pVoutUtil,
                                  short yOffset);

HRESULT Invoke_SetFormat3d(VoutUtil *pVoutUtil,
                           int format3d);

HRESULT Invoke_SetShiftOffset3d(VoutUtil *pVoutUtil,
                                bool exchange_eyeview,
                                bool sift_direction,
                                int delta_offset,
                                VO_VIDEO_PLANE targetPlane);

HRESULT Invoke_SetAnaglyphConversion(VoutUtil *pVideoUtil,
                                     long enable,
                                     long switchSrcEye = 0,
                                     VO_3D_SOURCE_FORMAT srcFormat = VO_SIDE_BY_SIDE);

HRESULT Invoke_ConfigFullWin3dMode(VoutUtil *pVoutUtil,
                                   int mode);

HRESULT Invoke_ConfigLowDelayMode(VoutUtil *pVoutUtil,
                                  int mode);

HRESULT Invoke_ConfigureDisplayWindow(VoutUtil *pVoutUtil,
                                      enum VO_VIDEO_PLANE videoPlane,
                                      struct VO_RECTANGLE videoWin,
                                      struct VO_RECTANGLE borderWin,
                                      struct VO_COLOR borderColor,
                                      u_char enBorder);

HRESULT Invoke_GetVoutRectangleSetup(VoutUtil *pVoutUtil,
									 VO_RECTANGLE &rec,
                                     VO_VIDEO_PLANE videoPlane,
                                     ENUM_VIDEO_SYSTEM videoSystem);


HRESULT Invoke_Configure_Z_Order(VoutUtil *pVoutUtil,
                              	 VIDEO_RPC_VOUT_SET_MIXER_ORDER * z_config);

HRESULT Invoke_QueryDisplayWin(VoutUtil *pVoutUtil,
							   VIDEO_RPC_VOUT_QUERY_DISP_WIN_IN display_plane_id,
							   VIDEO_RPC_VOUT_QUERY_DISP_WIN_OUT *out);

HRESULT Invoke_QueryGraphicWinInfo(VoutUtil *pVoutUtil,
                                   VIDEO_RPC_VOUT_QUERY_GRAPHIC_WIN_INFO_IN graphic_win_id,
                                   VIDEO_RPC_VOUT_QUERY_GRAPHIC_WIN_INFO_OUT *out);

#endif /* _VOUT_UTIL_WRAPPER_H_ */
