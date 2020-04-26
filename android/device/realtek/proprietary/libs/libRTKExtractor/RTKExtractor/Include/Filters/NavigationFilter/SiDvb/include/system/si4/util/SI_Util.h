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


#ifndef __SI_UTIL_H__
#define __SI_UTIL_H__

#define	DTV_CHNUM_BUF_SIZE	16
typedef struct _PHY_INFO {

	unsigned int    chNum;
	unsigned int    frequency;
	unsigned int    bandwidth;
	char            chNum_c[DTV_CHNUM_BUF_SIZE];
} PHY_INFO;



ErrCode SiApi_GetDtvFrequencyTable(PHY_INFO **freqTable, int *tableSize, SI_SPEC spec);
ErrCode SiApi_FreeFrequencyTable(PHY_INFO *freqTable);


BOOL SiApi_DealString(
	unsigned int							srcNameLen,
	unsigned char							*srcNameStr,
	unsigned int							*pDstNameLen,
	unsigned char							*pDstNameStr,
	unsigned int							maxLen,
	SI_SPEC									spec
);


 //used for si_load_service_rank()
int si_load_service_rank(SI_SPEC spec, SI_SERVICE_RANKING_TABLE** map);




#endif // __SI_UTIL_H__


