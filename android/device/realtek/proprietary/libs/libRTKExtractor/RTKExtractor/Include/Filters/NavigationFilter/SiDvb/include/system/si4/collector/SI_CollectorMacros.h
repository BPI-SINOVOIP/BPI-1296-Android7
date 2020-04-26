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

#ifndef _SI_COLLECTOR_MACROS_H_
#define _SI_COLLECTOR_MACROS_H_

//------------------------------------------------------------------------------
//
//	General Macros
//
//------------------------------------------------------------------------------
// Memory
#define SI_COLLECTOR_MALLOC(size)					SI_MALLOC(COLLECTOR, (size));
#define SI_COLLECTOR_FREE(p)						SI_FREE(COLLECTOR, (p));

// Logger
#ifdef WIN32
#define SI_COLLECTOR_LOG
#else
#define SI_COLLECTOR_LOG(logLevel, fmt, args...)	SI_LOG(COLLECTOR, logLevel, fmt, ##args);
#endif

// Begin & Return
#define SI_COLLECTOR_BEGIN()						SI_BEGIN(COLLECTOR);
#define SI_COLLECTOR_RETURN(var)					SI_RETURN(COLLECTOR, var);

// new & destroy obj
#define SI_COLLECTOR_NEW(obj, class)				SI_NEW(COLLECTOR, obj, class);
#define SI_COLLECTOR_DESTROY(obj)					SI_DESTROY(COLLECTOR, obj);

//------------------------------------------------------------------------------
//
//	Particular Macros
//
//------------------------------------------------------------------------------


#endif


