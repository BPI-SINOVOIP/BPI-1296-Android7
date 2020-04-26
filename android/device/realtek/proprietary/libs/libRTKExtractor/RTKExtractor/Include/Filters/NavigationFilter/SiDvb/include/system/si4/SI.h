/*=======================================================================
 * Copyright c				Realtek Semiconductor Corporation, 2007     *
 * All rights reserved.                                                 *
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*
 *
 * Module		: SI4
 *
 * Author		: ss@realtek.com.tw
 * Date			: 2007/04/23
 *
 *=======================================================================*/


#ifndef _SI_H_
#define _SI_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>

#ifdef _SI_PROFILE_TIME_
#include <sys/time.h>
#endif//_SI_PROFILE_TIME_


#ifdef USE_NEW_CF
#include <CFramework.h>
#else
#include <system/CFramework/CFramework.h>
#endif

// [ss] [20070828] : for pvr si porting
#include <system/si4/util/siMessage.h>


#include <system/si4/SITypes.h>
#include <system/si4/SIErrors.h>
#include <system/si4/SIMacros.h>

#include <system/si4/util/SI_Util.h>
#include <system/si4/channel/SI_Channel.h>
#include <system/si4/database/SI_Database.h>
#include <system/si4/collector/SI_Collector.h>
#include <system/si4/api/SI_Api.h>
#include <system/si4/tpInterface/TPInterface.h>

#endif
