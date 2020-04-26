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

#ifndef _SI_DATABASE_MACROS_H_
#define _SI_DATABASE_MACROS_H_


//------------------------------------------------------------------------------
//
//	General Macros
//
//------------------------------------------------------------------------------
// Memory
#define SI_DB_MALLOC(size)					SI_MALLOC(DB, (size));
#define SI_DB_FREE(p)						SI_FREE(DB, (p));

// Logger
#define SI_DB_LOG(logLevel, fmt, args...)	SI_LOG(DB, logLevel, fmt, ##args);

// Begin & Return
#define SI_DB_BEGIN()						SI_BEGIN(DB);
#define SI_DB_RETURN(var)					SI_RETURN(DB, var);

// new & destroy obj
#define SI_DB_NEW(obj, class)				SI_NEW(DB, obj, class);
#define SI_DB_DESTROY(obj)					SI_DESTROY(DB, obj);

#ifdef ADD_MULTISEC
#define SI_DB_DESTROY_MULTISEC(sec)	{	\
	int idx = 0;				\
	while (sec[idx]) {		\
		SI_DB_DESTROY(sec[idx]);	\
		sec[idx] = NULL;	\
		idx++;					\
	}}			
#endif

//------------------------------------------------------------------------------
//
//	Particular Macros
//
//------------------------------------------------------------------------------
#define SI_DB_GET_TSNODE_ADDR(db, ts, freq)						\
	do {															\
		ts = db->tsList;											\
		while(ts){if(ts->frequency==freq)break;ts=ts->next;}			\
	} while(0);

#define SI_DB_GET_VCNODE_ADDR(ts, vc, proNumber)						\
	do {																\
		vc = ts->vcList;												\
		while(vc){if((vc)->programNumber==(proNumber))break;vc=vc->next;}	\
	} while(0);

#define SI_DB_GEN_SECTION(secObj, secLength, secBuf)					\
	do {																\
		secObj = SI_DB_MALLOC(sizeof(SI_DATABASE_SECTION)+secLength);		\
		if(secObj) {														\
		secObj->sectionLength = secLength;								\
		memcpy(secObj->sectionRawBuf, secBuf, secLength);				\
		}																\
	} while(0);

#define SI_DB_GET_TABLEEXTENSION(secBuf, var)							\
	do {																\
		var = ((secBuf)[3]<<8) | (secBuf)[4];							\
	} while(0);

//compare crc only
#define SI_DB_COMPARE_SECOBJ_AND_SECBUF(secObj, secBuf, secBufLength)					\
	memcmp((secObj)->sectionRawBuf+(secObj)->sectionLength-SI_DB_SECTIONCRC_LEN,		\
	(secBuf)+secBufLength-SI_DB_SECTIONCRC_LEN,											\
	SI_DB_SECTIONCRC_LEN)

#ifdef _SI_CHECK_TS_NODE
	#define SI_DB_PRINT_ALL_TSNODE(db)		\
		do {									\
			SI_DATABASE_VC_NODE *tmpVc;	\
			SI_DATABASE_TS_NODE *tmpTs; 	\
			printf("TS info[%s:%d]\n", __FILE__, __LINE__); \
		printf("TS total:%d cur ts addr:%p\n", db->tsCount, db->currentTS);	\
			tmpTs = db->tsList;			\
			while(tmpTs) { 		\
				tmpVc = tmpTs->vcList;		\
				printf("TS: tsid:%x freq:%d addr:%p #VC:%d vc:\n", tmpTs->tsID, tmpTs->frequency, tmpTs, tmpTs->vcNum); \
				while(tmpVc){ printf("%d ", tmpVc->programNumber);tmpVc=tmpVc->next;}	\
				printf("\n");	\
				tmpTs=tmpTs->next;  \
			}	\
		} while(0);

	#define SI_DB_PRINT_TSNODE(ts)						\
		do {																\
			if(ts) { 		\
				SI_DATABASE_VC_NODE *vc;	\
				printf("Cur TS: tsid:%x freq:%d addr:%p curVC:%p VC:\n", ts->tsID, ts->frequency, ts, ts->currentVC); \
				vc = ts->vcList;		\
				while(vc){ printf("0x%02x addr:%p ", vc->programNumber, vc);vc=vc->next;}	\
				printf("\n");	\
			}	\
		} while(0);
#else
	#define SI_DB_PRINT_ALL_TSNODE(db)
	#define SI_DB_PRINT_TSNODE(ts)

#endif//

#define SI_DB_GET_SECTION_VER(buf) ((buf[5]>>1) & 0x1F)    //5 bits
#if 0
#define SI_DB_GetTableID(buf) (buf[0])                     //8 bits
#define SI_DB_GetSectionLength(buf) ( ((buf[1]&0x0F)<<8)|(buf[2]) ) //12 bits
#define SI_DB_GetTableID_Ext(buf) ((buf[3]<<8)|(buf[4]&0xff))               //1
#define SI_DB_GetVersionNumber(buf) ((buf[5]>>1) & 0x1F)    //5 bits
#define SI_DB_GetLastSectionNumber(buf) (buf[7])                //5 bits
#endif//0

#endif

