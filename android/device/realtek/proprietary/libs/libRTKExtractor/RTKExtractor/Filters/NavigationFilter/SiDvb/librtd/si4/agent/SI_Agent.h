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

#ifndef _SI_AGENT_H_
#define _SI_AGENT_H_

//------------------------------------------------------------------------------
//
//	Include Files
//
//------------------------------------------------------------------------------

#include <system/si4/SITypes.h>
#include "./siagent2_dvr/siAgentCustomize.h"

typedef struct SI_AGENT_LIST_NODE		SI_AGENT_LIST_NODE;
struct SI_AGENT_LIST_NODE
{
	SI_AGENT_LIST_NODE					*next;
	SI_AGENT_LIST_NODE					*previous;
	SI_AGENT							*agent;
};


//------------------------------------------------------------------------------
//
//	Functions
//
//------------------------------------------------------------------------------
ErrCode SI_Agent_New
(
	SI_AGENT							**ppAgent,
	SI									*pSI,
	SI_DATABASE							*pDatabase,
	SI_COLLECTOR						*pCollector,
	void								*pTPHandle
);

ErrCode SI_Agent_Destroy
(
	SI_AGENT							**ppAgent
);

ErrCode SI_Agent_Start
(
	SI_AGENT							*pAgent
);

ErrCode SI_Agent_Stop
(
	SI_AGENT							*pAgent
);

ErrCode SI_Agent_ResetCollector
(
	SI_AGENT							*pAgent
);

ErrCode SI_Agent_ScanChannel
(
	SI_AGENT							*pAgent,
	unsigned int						frequency
);

ErrCode SI_Agent_ChangeChannel
(
	SI_AGENT							*pAgent,
	unsigned int						frequency,
	unsigned short						programNumber
);

ErrCode SI_Agent_RecordChannel
(
	SI_AGENT							*pAgent,
	unsigned int						frequency,
	unsigned short						programNumber
);

ErrCode SI_Agent_StopAllTask	// Fix the bug that after auto scan start, cancel auto scan immediately, siAgent still run auto scan tasks.
(
	SI_AGENT							*pAgent
);

//kjw todo add timeout setting interface
ErrCode SI_Agent_SetTimeout
(
	SI_AGENT							*pAgent,
	unsigned int						frequency,
	unsigned short						programNumber
);


#ifdef SI_DVB_ENABLE_SSU
ErrCode SI_Agent_SsuScan
(
	SI_AGENT				*pAgent,
	unsigned int				frequency
);

ErrCode SI_Agent_SsuDownload
(
	SI_AGENT		*pAgent,
	SI_SSU_SW_INFO 			*swInfo
);
#endif//SI_DVB_ENABLE_SSU

#endif

