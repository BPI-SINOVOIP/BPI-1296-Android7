#ifndef __C_PIN_FILE_NAV_H__
#define __C_PIN_FILE_NAV_H__

#include <CStream.h>
//#include <Filters/PrivateInfo.h>


/*#include "CObject.h"
#include "CPin.h"

#include <Filters/FileNav/CFileNavPin.h>
*/
typedef enum {

	/* sending general info (for dubbing) */
	PRIVATEINFO_DUMP_GEN_INFO,
	
	/* sending vobu info (for dubbing) */
	PRIVATEINFO_DUMP_VOBU_INFO,
	
	/* sending cell info (for dubbing) */
	PRIVATEINFO_DUMP_CELL_INFO,
		
	/* sending vob info (for dubbing) */
	PRIVATEINFO_DUMP_VOB_INFO
	
} PRIVATEINFO_ID;

class COutFileNavPin: public CBaseOutputPin{
public:
								COutFileNavPin() {};
    virtual                     ~COutFileNavPin(void) {};
    HRESULT                     Connect(
                                    IPin*                   pReceivePin,
                                    const MEDIA_TYPE*       pmt
                                );
/*	HRESULT                     QueryAccept(
		                    const MEDIA_TYPE*       pmt
				);
	HRESULT                     EndOfStream(unsigned int eventID);
*/
protected:
};

class COutFileNavInfoPin: public CBaseOutputPin{
public:
	COutFileNavInfoPin() {};
    virtual                     ~COutFileNavInfoPin(void) {};
    HRESULT                     Connect(
                                    IPin*                   pReceivePin,
                                    const MEDIA_TYPE*       pmt
                                );
/*	HRESULT                     QueryAccept(
		                    const MEDIA_TYPE*       pmt
				);
	HRESULT                     EndOfStream(unsigned int eventID);
*/
protected:
};


#endif
