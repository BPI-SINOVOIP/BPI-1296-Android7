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

#ifndef _SI_API_MACROS_H_
#define _SI_API_MACROS_H_

//------------------------------------------------------------------------------
//
//	General Macros
//
//------------------------------------------------------------------------------
// Memory
#define SI_API_MALLOC(size)					SI_MALLOC(API, (size));
#define SI_API_FREE(p)						SI_FREE(API, (p));

// Logger
#ifdef WIN32
#define SI_API_LOG
#else
#define SI_API_LOG(logLevel, fmt, args...)	SI_LOG(API, logLevel, fmt, ##args);
#endif

// Begin & Return
#define SI_API_BEGIN()						SI_BEGIN(API);
#define SI_API_RETURN(var)					SI_RETURN(API, var);

// new & destroy obj
#define SI_API_NEW(obj, class)				SI_NEW(API, obj, class);
#define SI_API_DESTROY(obj)					SI_DESTROY(API, obj);

//------------------------------------------------------------------------------
//
//	Particular Macros
//
//------------------------------------------------------------------------------

#ifdef WIN32
#define SI_PRINT_TIME
#else
#ifdef _SI_PROFILE_TIME_

	#define SI_PRINT_TIME(args...) \
	do {	 \
		struct timeval CH_CHANGE_TIME;	\
		gettimeofday(&CH_CHANGE_TIME, NULL);	\
		printf(":%ld\n",##args, (time_t)(CH_CHANGE_TIME.tv_sec*1000+ ((CH_CHANGE_TIME.tv_usec)/1000)) );	 \
	} while(0)
#else
	#define SI_PRINT_TIME(args...)
#endif//_SI_PROFILE_TIME_
#endif

#endif


