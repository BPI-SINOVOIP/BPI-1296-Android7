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

#ifndef _CF_MODULEID_H_
#define _CF_MODULEID_H_

#ifdef USE_NEW_CF
#error "USE NEW CF"
#endif

//representation name is store in cfModuleName
typedef enum
{
	CF_MODID_CFRAMEWORK,

	CF_MODID_SI_DB,
	CF_MODID_SI_CH,
	CF_MODID_SI_COLLECTOR,
	CF_MODID_SI_AGENT,
	CF_MODID_SI_API,
	CF_MODID_APPLICATION,
	
	_CF_MODID_NUM				// number
} CF_MODID;

#endif


