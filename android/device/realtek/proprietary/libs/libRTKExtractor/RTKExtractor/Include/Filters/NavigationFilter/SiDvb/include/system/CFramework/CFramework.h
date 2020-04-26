/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: C Framework
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04
 *
 *=======================================================================*/

#ifndef _CFRAMEWORK_H_
#define _CFRAMEWORK_H_

#ifdef USE_NEW_CF
#error "USE NEW CF"
#endif

#include <system/CFramework/CFModuleID.h>

#include <system/CFramework/memory/CF_Memory.h>
#include <system/CFramework/timer/CF_Timer.h>
#include <system/CFramework/logger/CF_Logger.h>

#ifndef NULL
#define NULL							(void*)0;
#endif


void CF_Init();

void CF_DeInit();



#endif

