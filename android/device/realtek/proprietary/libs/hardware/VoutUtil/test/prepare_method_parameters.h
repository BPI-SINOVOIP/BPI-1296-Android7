#ifndef _PREPARE_METHOD_PARAMETERS_H_
#define _PREPARE_METHOD_PARAMETERS_H_

#include "vout_util_cmdline.h"

bool handle_SetVideoStandard(gengetopt_args_info &args_info);
bool handle_SetTvSystem(gengetopt_args_info &args_info);
bool handle_SetTVtype(gengetopt_args_info &args_info);
bool handle_SetBackground(gengetopt_args_info &args_info);
bool handle_SetClosedCaption(gengetopt_args_info &args_info);
bool handle_SetAPS(gengetopt_args_info &args_info);
bool handle_SetCopyMode(gengetopt_args_info &args_info);
bool handle_SetAspectRatio(gengetopt_args_info &args_info);
bool handle_SetV2alpha(gengetopt_args_info &args_info);
bool handle_SetRescaleMode(gengetopt_args_info &args_info);
bool handle_SetDeintMode(gengetopt_args_info &args_info);
bool handle_SetPeakingStrength(gengetopt_args_info &args_info);
bool handle_SetBrightness(gengetopt_args_info &args_info);
bool handle_SetContrast(gengetopt_args_info &args_info);
bool handle_SetHue(gengetopt_args_info &args_info);
bool handle_SetSaturation(gengetopt_args_info &args_info);
bool handle_GetTvDim(gengetopt_args_info &args_info, int *pwidth, int *pheight);
bool handle_SetSubtitleYoffset(gengetopt_args_info &args_info);
bool handle_SetFormat3d(gengetopt_args_info &args_info);
bool handle_SetShiftOffset3d(gengetopt_args_info &args_info);
bool handle_SetAnaglyphConversion(gengetopt_args_info &args_info);
bool handle_ConfigFullWin3dMode(gengetopt_args_info &args_info);
bool handle_ConfigLowDelayMode(gengetopt_args_info &args_info);
bool handle_ConfigureDisplayWindow(gengetopt_args_info &args_info);
bool handle_GetVoutRectangleSetup(gengetopt_args_info &args_info);
bool handle_Configure_Z_Order(gengetopt_args_info &args_info);
bool handle_QueryDisplayWin(gengetopt_args_info &args_info);
bool handle_QueryGraphicWinInfo(gengetopt_args_info &args_info);

#endif // _PREPARE_METHOD_PARAMETERS_H_
