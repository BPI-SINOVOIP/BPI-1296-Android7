/*
  excerpt from STB's siDBDescriptorDVB.h


*/

#ifndef _SI_DESCRIPTOR_H
#define _SI_DESCRIPTOR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <system/si4/SITypes.h>  //for SI_DESCRIPTOR_TAG
#include <database/SI_db.h>
#include "../SITypes_local.h"

/**
 * common structure for parsed descriptor
 */
typedef struct SI_DESCRIPTOR_				SI_DESCRIPTOR_T;		///< common structure of Descriptor object
typedef struct SI_DESCRIPTOR_				*SI_DESCRIPTOR_P;	///< common pointer of Descriptor object

struct SI_DESCRIPTOR_
{
	UINT8								tag;				///< descriptor tag
	UINT8								length;				///< descriptor length excluding first two bytes
	SI_DESCRIPTOR_P						next;				///< pointer to next Descriptor object
};


/**
* structure of CA Identifier Descriptor
*/
typedef struct SI_DESCRIPTOR_CA_IDENTIFIER_
{
	SI_DESCRIPTOR_T						header;				///< common structure for parsed descriptor
	INT32								caSystemIDNum;		///< number of CA System ID
	UINT16								caSystemID[0];		///< pointer of CA System ID
} SI_DESCRIPTOR_CA_IDENTIFIER_T, *SI_DESCRIPTOR_CA_IDENTIFIER_P;


/**
* 13818 REGISTERATION descriptor
*/
typedef struct SI_DESCRIPTOR_REGISTRATION_
{
	SI_DESCRIPTOR_T						header;				///< common structure for parsed descriptor
	UINT32								format;		///
	UINT32								_addInfo_length;		///not in spec
	UINT8								addInfo[1];		//additional identification info, length is tag length - 4
} SI_DESCRIPTOR_REGISTRATION_T, *SI_DESCRIPTOR_REGISTRATION_P;


typedef struct SI_DESCRIPTOR_DATA_BROADCAST_ID_
{
	SI_DESCRIPTOR_T						header;				///< common structure for parsed descriptor
	UINT16								dataBroadcastID;	///< indicate the data broadcast specification
	INT32								idSelectorByteLen;	///< length of ID Selector Byte
	UINT8								idSelectorByte[0];	///< pointer of ID Selector Byte
} SI_DESCRIPTOR_DATA_BROADCAST_ID_T, *SI_DESCRIPTOR_DATA_BROADCAST_ID_P;


/*
updateType(profile):
	0x0 proprietary update solution
	0x1 standard update carousel (i.e. without notification table) via broadcast
	0x2 system software update with notification table (UNT) via broadcast
	0x3 system software update using return channel with UNT
	0x4-0xF reserved for future use
	(0x02, 0x03 => UNT)
*/
typedef struct SI_DESCRIPTOR_DATA_BROADCAST_ID_LIBRA_
{
	SI_DESCRIPTOR_T					header;				///< common structure for parsed descriptor
	UINT16							dataBroadcastID;	///< indicate the data broadcast specification
	UINT8							ouiDataLength;
	UINT32 							oui ;				//24 bits
	UINT8							updateType;			//ie. profile
	UINT8							updateVerFlag; // 1 bit
	UINT8							updateVer;
	UINT8							selectorLeng;
	UINT16	 						device_model;
	UINT16							device_ver;
	UINT8							modelNameLeng;
	UINT8							modelName[1];
} SI_DESCRIPTOR_DATA_BROADCAST_ID_LIBRA_T, *SI_DESCRIPTOR_DATA_BROADCAST_ID_LIBRA_P;


ErrCode SI_Descr_ParseRegisteration
(
	unsigned char 				*buf,
	unsigned char					**reData
);

ErrCode SI_Descr_Parse_DATA_BROADCAST_ID_LIBRA
(
	unsigned char 				*buf,
	unsigned char					**reData
);


ErrCode SI_Descr_CloseParse
(
	unsigned char			*descrBuf
);
	
#ifdef FIX_DTV_LABS_HK_ISSUE_46	
ErrCode SI_Descr_Content(unsigned int descLoopLen, unsigned char *raw, CONTENTDESC *desc);
#endif

#ifdef __cplusplus
}
#endif

#endif//_SI_DESCRIPTOR_H

