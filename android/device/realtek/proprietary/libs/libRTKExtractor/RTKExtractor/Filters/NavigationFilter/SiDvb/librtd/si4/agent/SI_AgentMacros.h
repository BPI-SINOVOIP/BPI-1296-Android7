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

#ifndef _SI_AGENT_MACROS_H_
#define _SI_AGENT_MACROS_H_


//------------------------------------------------------------------------------
//
//	General Macros
//
//------------------------------------------------------------------------------
// Memory


#ifdef _DEBUG_SIAGENT_MEM_
//use following to is more secure if makefile definition is in-consistence
//b'cos siagent2_dvr/ has it's own makefile flag, to prevent flag in-consistence, call CF_Memory_Alloc directly
#define SI_AGENT_MALLOC(size)					CF_Memory_Alloc(CF_MODID_SI_AGENT, (size))
#define SI_AGENT_FREE(p)  do{     \
            CF_Memory_Free(CF_MODID_SI_AGENT, (p));   \
             (p) = NULL ; \
	}while(0)

#else
	#define SI_AGENT_MALLOC(size)	malloc((size))
	#define SI_AGENT_FREE(p)   do {		\
                                         		   free(p) ;		\
                                            		(p) = NULL ;	\
      							} while(0)

#endif//


// Logger
#define SI_AGENT_LOG(logLevel, fmt, args...)	SI_LOG(AGENT, logLevel, fmt, ##args);

// Begin & Return
#define SI_AGENT_BEGIN()						SI_BEGIN(AGENT);
#define SI_AGENT_RETURN(var)					SI_RETURN(AGENT, var);

// new & destroy obj
#define SI_AGENT_NEW(obj, class)				SI_NEW(AGENT, obj, class);
#define SI_AGENT_DESTROY(obj)					SI_DESTROY(AGENT, obj);

//------------------------------------------------------------------------------
//
//	Particular Macros
//
//------------------------------------------------------------------------------


#endif


