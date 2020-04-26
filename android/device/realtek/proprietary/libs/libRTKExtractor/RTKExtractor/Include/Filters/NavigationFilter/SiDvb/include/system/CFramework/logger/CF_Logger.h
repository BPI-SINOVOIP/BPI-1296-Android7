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

#ifndef _CF_LOGGER_H_
#define _CF_LOGGER_H_

#include <stdio.h>
#include <stdarg.h>

#include <system/CFramework/CFModuleID.h>


typedef enum
{
	CF_LL_FATAL							= 10,
	CF_LL_ERROR							= 20,
	CF_LL_WARNING						= 30,
	CF_LL_TRACE							= 40,
	CF_LL_DEBUG							= 50,
	CF_LL_IGNORE						= 100,
} CF_LL;

// color definition
#if CF_LOGGER_COLOR
#define _EC								"\033[m"
#define GRY_							"\033[1;30m"
#define RED_							"\033[1;31m"
#define GRN_							"\033[1;32m"
#define YLW_							"\033[1;33m"
#define BLU_							"\033[1;34m"
#define U_WHT_							"\033[1;40;4m"
#else
#define _EC
#define GRY_
#define RED_
#define GRN_
#define YLW_
#define BLU_
#define U_WHT_
#endif

void CF_Logger_Init();
void CF_Logger_DeInit();
void CF_Logger_DisableAllLogger();

void CF_Logger_SetLogLevel
(
	CF_MODID							moduleID,
	CF_LL								logLevel
);

void CF_Logger_Log
(
	CF_MODID							moduleID,
	CF_LL								logLevel,
	char								*fmt,
	...
);

#endif

