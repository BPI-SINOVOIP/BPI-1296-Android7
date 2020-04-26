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

#ifndef _SI_ERRORS_H
#define _SI_ERRORS_H


//#define SI_MAKE_ERRCODE(code) RTD_MAKE_ERRCODE(RTD_LIBSI4, (code&0x00ff))
#define SI_MAKE_ERRCODE(code) (0x1d0000 | (code&0x00ff))


typedef enum
{
	SI_ERR_OK = 0,

	SI_ERR_ASSERTION				= SI_MAKE_ERRCODE(0x01),
	SI_ERR_DATAALREADYEXIST			= SI_MAKE_ERRCODE(0x02),
	SI_ERR_DATADOESNOTEXIST			= SI_MAKE_ERRCODE(0x03),
	SI_ERR_CANNOTINSERT				= SI_MAKE_ERRCODE(0x04),
	SI_ERR_CANNOTFINDTARGET			= SI_MAKE_ERRCODE(0x05),
	SI_ERR_CANNOTFINDDATA			= SI_MAKE_ERRCODE(0x06),
	SI_ERR_BADPARAMETER				= SI_MAKE_ERRCODE(0x07),
	SI_ERR_OUTOFPIDFILTER			= SI_MAKE_ERRCODE(0x08),

	SI_ERR_INVALIDSECTIONLENGTH	= SI_MAKE_ERRCODE(0x09),
	SI_ERR_SECTION_ALIGNMENT4	= SI_MAKE_ERRCODE(0x0a),	//section is not 4 byte aligned
	SI_ERR_CRC_ERROR				= SI_MAKE_ERRCODE(0x0b),
	SI_ERR_SECTION_TOO_SHORT		= SI_MAKE_ERRCODE(0x0c), //the section's section length is below threshold
	SI_ERR_SECTION_NOT_CHECKED	= SI_MAKE_ERRCODE(0x0d), //the section's section length is below threshold

	SI_ERR_INVALIDFILEPATH			= SI_MAKE_ERRCODE(0x20),
	SI_ERR_MALLOC					= SI_MAKE_ERRCODE(0x21),
	SI_ERR_PIDFULL					= SI_MAKE_ERRCODE(0x22),




	SI_ERR_CURRENT_CH_DISAPPEAR	= SI_MAKE_ERRCODE(0x31), //current channel is disappear
	SI_ERR_LAST_CH_APPEAR			= SI_MAKE_ERRCODE(0x32), //

	// not an error
	SI_ERR_IND_SURPLUSBUFFER		= SI_MAKE_ERRCODE(0x41),

	//descriptor related error
	SI_ERR_DS_BUF_FAIL				= SI_MAKE_ERRCODE(0x51),
	SI_ERR_DS_BUF_RELEASE_FAIL	= SI_MAKE_ERRCODE(0x52),


	SI_ERR_SF_ADD_FAIL				= SI_MAKE_ERRCODE(0xf8), //remove section filter fail
	SI_ERR_SF_DEL_FAIL				= SI_MAKE_ERRCODE(0xf9), //remove section filter fail
	SI_ERR_SF_INVALID				= SI_MAKE_ERRCODE(0xfa), //remove section filter fail
	SI_ERR_EMPTY_SECTION			= SI_MAKE_ERRCODE(0xfb),
	SI_ERR_SECTION_POOL_FULL		= SI_MAKE_ERRCODE(0xfc), //queued sections for gemstar exceed threshold
	SI_ERR_NULL_MPEG2_CB			= SI_MAKE_ERRCODE(0xfd),
	SI_ERR_MULTI_INIT				= SI_MAKE_ERRCODE(0xfe),

	SI_ERR_UNDEFINED				= SI_MAKE_ERRCODE(0xff)

} SI_ERRORCODE;


#endif//_SI_ERRORS_H
