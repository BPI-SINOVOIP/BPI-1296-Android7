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

#ifndef _SI_CHANNEL_MACROS_H_
#define _SI_CHANNEL_MACROS_H_

//------------------------------------------------------------------------------
//
//	General Macros
//
//------------------------------------------------------------------------------
// Memory
#define SI_CH_MALLOC(size)					SI_MALLOC(CH, (size));
#define SI_CH_FREE(p)						SI_FREE(CH, (p));

// Logger
#define SI_CH_LOG(logLevel, fmt, args...)	SI_LOG(CH, logLevel, fmt, ##args);

// Begin & Return
#define SI_CH_BEGIN()						SI_BEGIN(CH);
#define SI_CH_RETURN(var)					SI_RETURN(CH, var);

// new & destroy obj
#define SI_CH_NEW(obj, class)				SI_NEW(CH, obj, class);
#define SI_CH_DESTROY(obj)					SI_DESTROY(CH, obj);

//------------------------------------------------------------------------------
//
//	Particular Macros
//
//------------------------------------------------------------------------------

#define SI_CH_GET_CHANNEL_ADDR(chmgr, ch, freq, proNumber)				\
	do {																\
		ch = chmgr->chList;												\
		while(ch){if(ch->frequency==freq&&ch->programNumber==proNumber)break;ch=ch->next;}	\
	} while(0);

#define SI_CH_GET_CHANNEL_ADDR_BY_LCN(chmgr, ch, LCN)					\
	do {																\
		ch = chmgr->chList;												\
		while(ch){if(ch->super.logicalChNum==LCN)break;ch=ch->next;}	\
	} while(0);

#define SI_CH_PRINT_ALL_CHANNEL(chmgr)						\
	do {		\
		SI_CHANNEL_EXT * chX;	\
		chX = chmgr->chList;	\
		printf("#Ch: %d", chmgr->channelNum);	\
		while(chX){ printf(" ch:%d(0x%0x,%d) name:%s",chX->super.logicalChNum, chX->programNumber, chX->frequency, chX->oriChannelName);chX=chX->next;}	\
		printf("\n");	\
	} while(0);

#if 1
#define SI_CH_PRINT_CHANNEL(ch)		\
	do {		\
		printf("ch prog:0%x(%d) lcn:%d vPid:0x%x\n", (ch)->programNumber, (ch)->frequency, (ch)->super.logicalChNum, (ch)->videos[0].videoPid);	\
	} while(0);

#else
//scramble 
#define SI_CH_PRINT_CHANNEL(ch)		\
	do {		\
		printf("ch prog:0x%x(%d) lcn:%d vPid:0x%x scramble:0x%x\n", (ch)->programNumber, (ch)->frequency, (ch)->super.logicalChNum, (ch)->videos[0].videoPid, ((ch)->channelFlags & SI_CHANNEL_FLAG_IS_SCRAMBLED) );	\
	} while(0);
	
#endif//0

#define SI_CH_SET_FLAG(ch, bitfields) (((ch)->channelFlags) |= (bitfields))
#define SI_CH_RESET_FLAG(ch, bitfields) (((ch)->channelFlags) &= ~(bitfields))
#define SI_CH_HAS_FLAG(ch, bitfields)	((((ch)->channelFlags) & (bitfields)) == (bitfields))
//#define SI_CH_TEST_FLAG(ch, bitfields) (((ch)->channelFlags)) & (bitfields))

#define SI_CH_SET_BITS(data, bitfields) ((data) |= (bitfields))
#define SI_CH_RESET_BITS(data, bitfields) ((data) &= ~(bitfields))
#define SI_CH_HAS_BITS(data, bitfields)  (((data) & (bitfields)) == (bitfields))


#define SI_CH_LCN_COMPARE(ca, cb) \
	do {	\
		if( (*ca)->super.logicalChNum != (*cb)->super.logicalChNum )  {	\
			if( (*ca)->super.logicalChNum == 0 ) return 1;		\
			if( (*cb)->super.logicalChNum == 0 ) return -1;	\
			if( (*ca)->super.logicalChNum > (*cb)->super.logicalChNum) 	return 1;	\
			else if((*ca)->super.logicalChNum < (*cb)->super.logicalChNum) 	return -1; \
		} \
	} while(0);

//don't care the case where a.onid and b.ond are not equal to user onid
#define SI_CH_ONID_COMPARE(ca, cb) \
	do {	\
		if( (*ca)->onid != (*cb)->onid )  {	\
			if( (*ca)->onid == USER_REGION_ONID ) return -1;		\
			if( (*cb)->onid == USER_REGION_ONID ) return 1;	\
		} \
	} while(0);


#endif


