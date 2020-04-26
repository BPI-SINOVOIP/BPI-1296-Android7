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

#ifndef _SI_COLLECTOR_H_
#define _SI_COLLECTOR_H_

#include <system/si4/collector/SI_CollectorMacros.h>



ErrCode SI_Collector_New
(
	SI_COLLECTOR						**ppCollector,
	unsigned int						beanQueueNum,
	unsigned int						pidFilterNum
);

ErrCode SI_Collector_Destroy
(
	SI_COLLECTOR						**ppCollector
);

ErrCode SI_Collector_Reset
(
	SI_COLLECTOR						*pCollector
);

ErrCode SI_Collector_SetSection_Callback
(
	SI_COLLECTOR						*pCollector,
	SI_Collector_SectionCallback 			cb,
	SI_SECTIONCOLLECTOR_CBF_TYPE		type
);

//ErrCode SI_Collector_ResetTP
//(
//	SI_COLLECTOR						*pCollector
//);

ErrCode SI_Collector_ResetBeanCtrlInfo
(
	SI_COLLECTOR						*pCollector
);

ErrCode SI_Collector_ResetSectionBuf
(
	SI_COLLECTOR						*pCollector
);

ErrCode SI_Collector_ResetSectionBufAndKeepPIDSettings
(
	SI_COLLECTOR						*pCollector
);

ErrCode SI_Collector_ResetSectionBufByPID
(
	SI_COLLECTOR						*pCollector,
	unsigned int						pid
);

ErrCode SI_Collector_SetSectionBufByPID
(
	SI_COLLECTOR						*pCollector,
	unsigned int						pid
);

ErrCode SI_Collector_CollectSection
(
	SI_COLLECTOR						*pCollector,
	SI_DATABASE							*pDatabase,
	void								*tp
);



#endif

