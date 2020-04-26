/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMRESULTS_H__
#define __DRMRESULTS_H__

#ifdef __cplusplus
extern "C" {
#endif



/* These are DRM_RESULTS.  DRM_RESULTS is intended to mirror the use of HRESULTS */

#if SIXTEEN_BIT_ADDRESSING
#define DRM_FAILED(Status) (((DRM_RESULT)(Status)&0x80000000)!=0)
#define DRM_SUCCEEDED(Status) (((DRM_RESULT)(Status)&0x80000000)==0)
#else
#define DRM_FAILED(Status) ((DRM_RESULT)(Status)<0)
#define DRM_SUCCEEDED(Status) ((DRM_RESULT)(Status) >= 0)
#endif

/*Success return code */

/*
*	MessageId:	
*	Message Meaning:	
*	Hex Value:	
*/

/* Generic success return value */
#define DRM_SUCCESS ((DRM_RESULT)0x00000000L)
#define DRM_S_FALSE  ((DRM_RESULT)0x00000001L)


/* Fail return codes */
/* Standard return codes copied from winerror.h */
#define DRM_E_FAIL				((DRM_RESULT)0x80004005L)
#define DRM_E_INVALIDARG		((DRM_RESULT)0x80070057L)
#define DRM_E_OUTOFMEMORY		((DRM_RESULT)0x80000002L)
#define DRM_E_FILENOTFOUND		((DRM_RESULT)0x80030002L)
#define DRM_E_FILEOPEN          ((DRM_RESULT)0x8003006EL)    
#define DRM_E_BUFFERTOOSMALL	((DRM_RESULT)0x8007007AL)
#define DRM_E_NOTIMPL			((DRM_RESULT)0x80004001L)
#define DRM_E_NOMORE			((DRM_RESULT)0x80070103L) /* End of enum, or no more data available */
#define DRM_E_ARITHMETIC_OVERFLOW        DRM_RESULT_FROM_WIN32( 534L )

#define DRM_E_PARAMETERS_MISMATCHED     ((DRM_RESULT)0xC00D272FL)
#define DRM_E_FAILED_TO_STORE_LICENSE   ((DRM_RESULT)0xC00D2712L)
#define DRM_E_NOT_ALL_STORED            ((DRM_RESULT)0xC00D275FL)


#define DRM_SEVERITY_SUCCESS				0uL
#define DRM_SEVERITY_ERROR					1uL
#define DRM_FACILITY_ITF                    4uL
#define DRM_FACILITY_WIN32                  7uL
#define DRM_S_BASECODE		                0xC000
#define DRM_E_BASECODE						0xC000

#define MAKE_DRM_RESULT(sev,fac,code) \
    ((DRM_RESULT) (((unsigned long)(sev)<<31) | ((unsigned long)(fac)<<16) | ((unsigned long)(code))) )


#define DRM_RESULT_FROM_WIN32(x) ((DRM_RESULT)(x) <= 0 ? ((DRM_RESULT)(x)) : ((DRM_RESULT) (((x) & 0x0000FFFF) | (DRM_FACILITY_WIN32 << 16) | 0x80000000)))
/* DRM specific error codes */


/*
*	MessageId:	DRM_E_DEVICENOTINIT
*	Message Meaning:
*		This device has not been initialized against a DRM init service
*	Hex Value:	0x8004c001
*/

#define DRM_E_DEVICENOTINIT				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1)

/*
*	MessageId:	DRM_E_DRMNOTINIT
*	Message Meaning:	
*		The app has not call DRM_Init properly
*	Hex Value:	0x8004c002
*/

#define DRM_E_DRMNOTINIT					MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+2)


/* Errors related to problems with a license */

/*
*	MessageId:	DRM_E_INVALIDRIGHT
*	Message Meaning:	
*		A right in the license in invalid
*	Hex Value:	0x8004c003
*/

#define DRM_E_INVALIDRIGHT					MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+3)

/*
*	MessageId:	DRM_E_INCOMPATABLELICENSESIZE
*	Message Meaning:	
*		The size of the license is incompatable.  DRM doesn't understand this license
*	Hex Value:	0x8004c004
*/

#define DRM_E_INCOMPATABLELICENSESIZE	MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+4)

/*
*	MessageId:	DRM_E_INVALIDLICENSEFLAGS
*	Message Meaning:	
*		The flags in the license are invalid.  DRM either doesn't understand them or they are conflicting
*	Hex Value:	0x8004c005
*/


#define DRM_E_INVALIDLICENSEFLAGS			MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+5)

/*
*	MessageId:	DRM_E_INVALIDLICENSE
*	Message Meaning:	
*		 The license is invalid
*	Hex Value:	0x8004c006
*/

#define DRM_E_INVALIDLICENSE				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+6)

/*
*	MessageId:	DRM_E_CONDITIONFAIL 
*	Message Meaning:	
*		A condition in the license failed to pass
*	Hex Value:	0x8004c007
*/

#define DRM_E_CONDITIONFAIL				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+7)

/*
*	MessageId:	DRM_E_CONDITIONNOTSUPPORTED 
*	Message Meaning:	
*		A condition in the license is not supported by this verison of DRM
*	Hex Value:	0x8004c008
*/

#define DRM_E_CONDITIONNOTSUPPORTED		MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+8)

/*
*	MessageId:	DRM_E_LICENSEEXPIRED 
*	Message Meaning:	
*		The license has expired either by depleting a play count or via an end time.
*	Hex Value:	0x8004c009
*/

#define DRM_E_LICENSEEXPIRED				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+9)

/*
*	MessageId:	DRM_E_LICENSENOTYETVALID 
*	Message Meaning:	
*		The license start time had not come to pass yet.
*	Hex Value:	0x8004c00a
*/

#define DRM_E_LICENSENOTYETVALID			MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+10)

/*
*	MessageId:	DRM_E_RIGHTSNOTAVAILABLE 
*	Message Meaning:	
*		The rights the app has requested are not available in the license
*	Hex Value:	0x8004c00b
*/

#define DRM_E_RIGHTSNOTAVAILABLE			MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+11)

/*
*	MessageId:	DRM_E_LICENSEMISMATCH 
*	Message Meaning:	
*		The license content id/ sku id doesn't match that requested by the app
*	Hex Value:	0x8004c00c
*/

#define DRM_E_LICENSEMISMATCH				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+12)

/*
*	MessageId:	DRM_E_WRONGTOKENTYPE
*	Message Meaning:	
*		The token parameter was of an incompatible type.
*	Hex Value:	0x8004c00d
*/

#define DRM_E_WRONGTOKENTYPE	            MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+13)

/* Other errors */

/*
*	MessageId:	DRM_E_NORIGHTSREQUESTED 
*	Message Meaning:	
*		The app has not requested any rights before trying to bind
*	Hex Value:	0x8004c00e
*/

#define DRM_E_NORIGHTSREQUESTED			    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+14)

/*
*	MessageId:	DRM_E_LICENSENOTBOUND 
*	Message Meaning:	
*		A license has not been bound to.  Decrypt can not happen without a successful bind call
*	Hex Value:	0x8004c00f
*/

#define DRM_E_LICENSENOTBOUND				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+15)

/*
*	MessageId:	DRM_E_HASHMISMATCH 
*	Message Meaning:	
*		A Keyed Hash check failed.
*	Hex Value:	0x8004c010
*/

#define DRM_E_HASHMISMATCH				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+16)

/*
*	MessageId:	DRM_E_INVALIDTIME
*	Message Meaning:	
*		A time structure is invalid 
*	Hex Value:	0x8004c011
*/

#define DRM_E_INVALIDTIME					MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+17)

/*
*	MessageId:	DRM_E_LICENSESTORENOTFOUND
*	Message Meaning:	
*		The external license store was not found
*	Hex Value:	0x8004c012
*/

#define DRM_E_LICENSESTORENOTFOUND		MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+18)

/*
*	MessageId:	DRM_E_LICENSENOTFOUND
*	Message Meaning:	
*		A license was not found in the external license store
*	Hex Value:	0x8004c013
*/

#define DRM_E_LICENSENOTFOUND				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+19)

/*
*	MessageId:	DRM_E_LICENSEVERSIONNOTSUPPORTED
*	Message Meaning:	
*		The DRM license version is not supported by the DRM version on the device.
*	Hex Value:	0x8004c014
*/

#define DRM_E_LICENSEVERSIONNOTSUPPORTED	MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+20)

/*
*	MessageId:	DRM_E_INVALIDBINDID
*	Message Meaning:	
*		The bind id is invalid.
*	Hex Value:	0x8004c015
*/

#define DRM_E_INVALIDBINDID				MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+21)

/*
*	MessageId:	DRM_E_UNSUPPORTEDALGORITHM
*	Message Meaning:	
*		The encryption algorithm required for this operation is not supported.
*	Hex Value:	0x8004c016
*/

#define DRM_E_UNSUPPORTEDALGORITHM		MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+22)

/*
*	MessageId:	DRM_E_ALGORITHMNOTSET
*	Message Meaning:	
*		The encryption algorithm required for this operation is not supported.
*	Hex Value:	0x8004c017
*/

#define DRM_E_ALGORITHMNOTSET		MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+23)

/*
*	MessageId:	DRM_E_LICENSESERVERNEEDSKEY
*	Message Meaning:	
*		The license server needs a version of the device bind key from the init service.
*	Hex Value:	0x8004c018
*/

#define DRM_E_LICENSESERVERNEEDSKEY		MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+24)

/*
*	MessageId:	DRM_E_INVALIDLICENSESTORE
*	Message Meaning:	
*		The license store version number is incorrect, or the store is invalid in some other way.
*	Hex Value:	0x8004c019
*/

#define DRM_E_INVALIDLICENSESTORE   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+25)

/*
*	MessageId:	DRM_E_FILEREADERROR
*	Message Meaning:	
*		There was an error reading a file.
*	Hex Value:	0x8004c01a
*/

#define DRM_E_FILEREADERROR         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+26)

/*
*	MessageId:	DRM_E_FILEWRITEERROR
*	Message Meaning:	
*		There was an error writing a file.
*	Hex Value:	0x8004c01b
*/

#define DRM_E_FILEWRITEERROR        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+27)


/*
*	MessageId:	DRM_E_CLIENTTIMEINVALID
*	Message Meaning:	
*		The time/clock on the device is not in sync with the license server within tolerance.
*	Hex Value:	0x8004c01c
*/

#define DRM_E_CLIENTTIMEINVALID     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+28)

/*
*	MessageId:	DRM_E_HDSSTOREFULL
*	Message Meaning:	
*		HDS Data file is FULL.
*	Hex Value:	0x8004c01d
*/

#define DRM_E_HDSSTOREFULL     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+29)

/*
*	MessageId:	DRM_E_NOXMLOPENTAG
*	Message Meaning:	
*		XML open tag not found
*	Hex Value:	0x8004c01e
*/
#define DRM_E_NOXMLOPENTAG          MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+30)


/*
*	MessageId:	DRM_E_NOXMLCLOSETAG
*	Message Meaning:	
*		XML close tag not found
*	Hex Value:	0x8004c01f
*/
#define DRM_E_NOXMLCLOSETAG        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+31)

/*
*	MessageId:	DRM_E_INVALIDXMLTAG
*	Message Meaning:	
*		Invalid XML tag
*	Hex Value:	0x8004c020
*/
#define DRM_E_INVALIDXMLTAG			MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+32)

/*
*	MessageId:	DRM_E_NOXMLCDATA
*	Message Meaning:	
*		No XML CDATA found
*	Hex Value:	0x8004c021
*/
#define DRM_E_NOXMLCDATA			MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+33)

/*
*	MessageId:	DRM_E_HDSNAMESPACEFULL
*	Message Meaning:	
*		No more room for HDS Namespace
*	Hex Value:	0x8004c022
*/
#define DRM_E_HDSNAMESPACEFULL      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+34)

/*
*	MessageId:	DRM_E_HDSNAMESPACENOTFOUND
*	Message Meaning:	
*		No HDS Namespace found
*	Hex Value:	0x8004c023
*/
#define DRM_E_HDSNAMESPACENOTFOUND      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+35)

/*
*	MessageId:	DRM_E_HDSSLOTNOTFOUND
*	Message Meaning:	
*		No HDS HDS Dataslot not found
*	Hex Value:	0x8004c024
*/
#define DRM_E_HDSSLOTNOTFOUND      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+36)

/*
*	MessageId:	DRM_E_HDSSLOTEXIST
*	Message Meaning:	
*		No HDS HDS Dataslot already exist
*	Hex Value:	0x8004c025
*/
#define DRM_E_HDSSLOTEXIST      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+37)

/*
*	MessageId:	DRM_E_HDSFILECORRUPTED
*	Message Meaning:	
*		HDS File is corrupted
*	Hex Value:	0x8004c026
*/
#define DRM_E_HDSFILECORRUPTED      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+38)

/*
*	MessageId:	DRM_E_HDSSEEKERROR
*	Message Meaning:	
*		There was an error attempting to seek in the HDS file
*	Hex Value:	0x8004c027
*/
#define DRM_E_HDSSEEKERROR      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+39)

/*
*	MessageId:	DRM_E_HDSNAMESPACEINUSE
*	Message Meaning:	
*		No HDS Namespace in use
*	Hex Value:	0x8004c028
*/
#define DRM_E_HDSNAMESPACEINUSE      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+40)


/* Errors related to the secure store */

/*
*	MessageId:	DRM_E_INVALID_SECURESTORE_PASSWORD
*	Message Meaning:	
*		The password used to open the secure store key was not able to validate the secure store hash.
*	Hex Value:	0x8004c029
*/

#define DRM_E_INVALID_SECURESTORE_PASSWORD MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+41)

/*
*	MessageId:	DRM_E_SECURESTORE_CORRUPT
*	Message Meaning:	
*		The secure store is corrupt
*	Hex Value:	0x8004c02a
*/

#define DRM_E_SECURESTORE_CORRUPT MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+42)

/*
*	MessageId:	DRM_E_SECURESTORE_FULL
*	Message Meaning:	
*		The current secure store key is full.  No more data can be added.
*	Hex Value:	0x8004c02b
*/

#define DRM_E_SECURESTORE_FULL MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+43)


/*
*	MessageId:	DRM_E_NOACTIONINLICENSEREQUEST
*	Message Meaning:	
*		No action(s) added for license request
*	Hex Value:	0x8004c02c
*/
#define DRM_E_NOACTIONINLICENSEREQUEST     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+44)


/*
*	MessageId:	DRM_E_DUPLICATEDHEADERATTRIBUTE
*	Message Meaning:	
*		Duplicated attribute in Header
*	Hex Value:	0x8004c02d
*/
#define DRM_E_DUPLICATEDHEADERATTRIBUTE     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+45)

/*
*	MessageId:	DRM_E_NOKIDINHEADER
*	Message Meaning:	
*		No KID attribute in Header
*	Hex Value:	0x8004c02e
*/
#define DRM_E_NOKIDINHEADER     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+46)

/*
*	MessageId:	DRM_E_NOLAINFOINHEADER
*	Message Meaning:	
*		No LAINFO attribute in Header
*	Hex Value:	0x8004c02f
*/
#define DRM_E_NOLAINFOINHEADER     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+47)

/*
*	MessageId:	DRM_E_NOCHECKSUMINHEADER
*	Message Meaning:	
*		No Checksum attribute in Header
*	Hex Value:	0x8004c030
*/
#define DRM_E_NOCHECKSUMINHEADER     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+48)

/*
*	MessageId:	DRM_E_HDSBLOCKMISMATCH
*	Message Meaning:	
*		No Checksum attribute in Header
*	Hex Value:	0x8004c031
*/
#define DRM_E_HDSBLOCKMISMATCH     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+49)

/*
*   MessageId:  DRM_E_BACKUP_EXISTS
*   Message Meaning:    
*       Backup file already exist.
*   Hex Value:  0x8004c032
*/
#define DRM_E_BACKUP_EXISTS     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+50)

/*
*   MessageId:  DRM_E_LICENSE_TOOLONG
*   Message Meaning:    
*       License size is too long 
*   Hex Value:  0x8004c032
*/
#define DRM_E_LICENSE_TOOLONG     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+51)

/*
*	MessageId:	DRM_E_HDSFILEEXISTS
*	Message Meaning:	
*		HDS File already exists
*	Hex Value:	0x8004c034
*/
#define DRM_E_HDSFILEEXISTS      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+52)

/*
*	MessageId:	DRM_E_INVALIDDEVICECERTIFICATE
*	Message Meaning:	
*		The device certificate is invalid.
*	Hex Value:	0x8004c035
*/
#define DRM_E_INVALIDDEVICECERTIFICATE      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+53)

/*
*	MessageId:	DRM_E_HDSLOCKFAILED
*	Message Meaning:	
*		Locking a segement of the HDS file failed.
*	Hex Value:	0x8004c036
*/
#define DRM_E_HDSLOCKFAILED      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+54)

/*
*	MessageId:	DRM_E_FILESEEKERROR
*	Message Meaning:	
*		File Seek Error
*	Hex Value:	0x8004c037
*/
#define DRM_E_FILESEEKERROR      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+55)

/*
*	MessageId:	DRM_E_HDSNOTLOCKEDEXCLUSIVE
*	Message Meaning:	
*		existing lock is not exclusive
*	Hex Value:	0x8004c038
*/
#define DRM_E_HDSNOTLOCKEDEXCLUSIVE      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+56)

/*
*	MessageId:	DRM_E_HDSEXCLUSIVELOCKONLY
*	Message Meaning:	
*		only exclusive lock is accepted
*	Hex Value:	0x8004c039
*/
#define DRM_E_HDSEXCLUSIVELOCKONLY      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+57)


/*
*   MessageId:  DRM_E_HDSRESERVEDKEYDETECTED
*   Message Meaning:    
*       HDS reserved key value detected in UniqueKey
*   Hex Value:  0x8004c03A
*/
#define DRM_E_HDSRESERVEDKEYDETECTED      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+58)


/*
*   MessageId:  DRM_E_V1_NOT_SUPPORTED
*   Message Meaning:    
*       V1 Lic Acquisition is not supported
*   Hex Value:  0x8004c03B
*/
#define DRM_E_V1_NOT_SUPPORTED           MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+59)


/*
*   MessageId:  DRM_E_HEADER_NOT_SET
*   Message Meaning:    
*       A V2 content header is not set 
*   Hex Value:  0x8004c03C
*/
#define DRM_E_HEADER_NOT_SET           MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+60)

/*
*	MessageId:	DRM_E_NEEDDEVCERTINDIV
*	Message Meaning:	
*		The device certificate is template. It need Devcert Indiv
*	Hex Value:	0x8004c03d
*/

#define DRM_E_NEEDDEVCERTINDIV       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+61)

/*
*	MessageId:	DRM_E_MACHINEIDMISMATCH
*	Message Meaning:	
*		The device has Machine Id different from that in devcert.
*	Hex Value:	0x8004c03e
*/

#define DRM_E_MACHINEIDMISMATCH       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+62)

/*
*	MessageId:	DRM_E_CLK_INVALID_RESPONSE
*	Message Meaning:	
*		The secure clock response is invalid.
*	Hex Value:	0x8004c03f
*/

#define DRM_E_CLK_INVALID_RESPONSE       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+63)
/*
*	MessageId:	DRM_E_CLK_INVALID_DATE
*	Message Meaning:	
*		The secure clock response is invalid.
*	Hex Value:	0x8004c040
*/

#define DRM_E_CLK_INVALID_DATE       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+64)


/*
*	MessageId:	DRM_E_CLK_UNSUPPORTED_VALUE
*	Message Meaning:	
*		The secure clock response has unsupported value.
*	Hex Value:	0x8004c041
*/

#define DRM_E_CLK_UNSUPPORTED_VALUE       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+65)

/*
*	MessageId:	DRM_E_INVALIDDEVICECERTIFICATETEMPLATE
*	Message Meaning:	
*		The device certificate is invalid.
*	Hex Value:	0x8004c042
*/
#define DRM_E_INVALIDDEVICECERTIFICATETEMPLATE      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+66)

/*
*	MessageId:	DRM_E_DEVCERTEXCEEDSSIZELIMIT
*	Message Meaning:	
*		The device certificate exceeds max size
*	Hex Value:	0x8004c043
*/

#define DRM_E_DEVCERTEXCEEDSIZELIMIT       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+67)

/*
*	MessageId:	DRM_E_DEVCERTTEMPLATEEXCEEDSSIZELIMIT
*	Message Meaning:	
*		The device certificate template exceeds max size
*	Hex Value:	0x8004c044
*/

#define DRM_E_DEVCERTTEMPLATEEXCEEDSSIZELIMIT       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+68)

/*
*	MessageId:	DRM_E_DEVCERTREADERROR
*	Message Meaning:	
*		Can't get the device certificate
*	Hex Value:	0x8004c045
*/

#define DRM_E_DEVCERTREADERROR        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+69)
/*
*	MessageId:	DRM_E_DEVCERTWRITEERROR
*	Message Meaning:	
*		Can't store the device certificate
*	Hex Value:	0x8004c046
*/

#define DRM_E_DEVCERTWRITEERROR        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+70)

/*
*	MessageId:	DRM_E_PRIVKEYREADERROR
*	Message Meaning:	
*		Can't get device private key
*	Hex Value:	0x8004c047
*/

#define DRM_E_PRIVKEYREADERROR        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+71)

/*
*	MessageId:	DRM_E_PRIVKEYWRITEERROR
*	Message Meaning:	
*		Can't store device private key
*	Hex Value:	0x8004c048
*/

#define DRM_E_PRIVKEYWRITEERROR        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+72)

/*
*	MessageId:	DRM_E_DEVCERTTEMPLATEREADERROR
*	Message Meaning:	
*		Can't get the device certificate template
*	Hex Value:	0x8004c049
*/

#define DRM_E_DEVCERTTEMPLATEREADERROR        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+73)


/*
*	MessageId:	DRM_E_CLK_NOT_SUPPORTED
*	Message Meaning:	
*		The secure clock is not supported.
*	Hex Value:	0x8004c04a
*/

#define DRM_E_CLK_NOT_SUPPORTED       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+74)

/*
*	MessageId:	DRM_E_DEVCERTINDIV_NOT_SUPPORTED
*	Message Meaning:	
*		The Devcert Indiv is not supported.
*	Hex Value:	0x8004c04b
*/

#define DRM_E_DEVCERTINDIV_NOT_SUPPORTED       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+75)

/*
*	MessageId:	DRM_E_METERING_NOT_SUPPORTED
*	Message Meaning:	
*		The Metering is not supported.
*	Hex Value:	0x8004c04c
*/

#define DRM_E_METERING_NOT_SUPPORTED       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+76)

/*
*	MessageId:	DRM_E_CLK_RESETSTATEREADERROR
*	Message Meaning:	
*		Can not read Secure clock Reset State.
*	Hex Value:	0x8004c04d
*/

#define DRM_E_CLK_RESETSTATEREADERROR      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+77)

/*
*	MessageId:	DRM_E_CLK_RESETSTATEWRITEERROR
*	Message Meaning:	
*		Can not write Secure clock Reset State.
*	Hex Value:	0x8004c04e
*/

#define DRM_E_CLK_RESETSTATEWRITEERROR      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+78)

/*
*	MessageId:	DRM_E_XMLNOTFOUND
*	Message Meaning:	
*		a required XML tag was not found
*	Hex Value:	0X8004C04F
*/

#define DRM_E_XMLNOTFOUND                   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+79)

/*
*   MessageId:  DRM_E_METERING_WRONG_TID
*   Message Meaning:    
*       wrong TID sent on metering response
*   Hex Value:  0X8004C050
*/
#define DRM_E_METERING_WRONG_TID            MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+80)

/*
*   MessageId:  DRM_E_METERING_INVALID_COMMAND
*   Message Meaning:    
*       wrong command sent on metering response
*   Hex Value:  0X8004C051
*/
#define DRM_E_METERING_INVALID_COMMAND      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+81)

/*
*   MessageId:  DRM_E_METERING_STORE_CORRUPT
*   Message Meaning:    
*       The metering store is corrupt
*   Hex Value:  0X8004C052
*/
#define DRM_E_METERING_STORE_CORRUPT        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+82)


/*
*   MessageId:  DRM_E_CERTIFICATE_REVOKED
*   Message Meaning:    
*       A certificate given to DRM was revoked.
*   Hex Value:  0X8004C053
*/
#define DRM_E_CERTIFICATE_REVOKED         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+83)


/*
*   MessageId:  DRM_E_CRYPTO_FAILED
*   Message Meaning:    
*       A cryptographic operation failed.
*   Hex Value:  0X8004C054
*/
#define DRM_E_CRYPTO_FAILED               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+84)

/*
*   MessageId:  DRM_E_STACK_CORRUPT
*   Message Meaning:    
*       The stack allocator context is corrupt.  Likely a buffer overrun problem.
*   Hex Value:  0X8004C055
*/
#define DRM_E_STACK_CORRUPT               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+85)

/*
*	MessageId:	DRM_E_UNKNOWN_BINDING_KEY
*	Message Meaning:	
*		A matching binding key could not be found for the license.
*	Hex Value:	0x8004C056
*/
#define DRM_E_UNKNOWN_BINDING_KEY         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+86)

/*
*   MessageId:  DRM_E_V1_LICENSE_CHAIN_NOT_SUPPORTED
*   Message Meaning:    
*       License chaining with V1 content is not supported.
*   Hex Value:  0x8004C057
*/
#define DRM_E_V1_LICENSE_CHAIN_NOT_SUPPORTED           MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+87)


/*
*   MessageId:  DRM_E_WRONG_TOKEN_TYPE
*   Message Meaning:    
*       The wrong type of token was used.
*   Hex Value:  0x8004C058
*/
#define DRM_E_WRONG_TOKEN_TYPE           MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+88)


/*
*   MessageId:  DRM_E_POLICY_METERING_DISABLED
*   Message Meaning:    
*       Metering code was called but metering is disabled by group or user policy
*   Hex Value:  0x8004C059
*/
#define DRM_E_POLICY_METERING_DISABLED MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+89)


/*
*   MessageId:  DRM_E_POLICY_ONLINE_DISABLED
*   Message Meaning:    
*       online communication is disabled by group policy
*   Hex Value:  0x8004C05A
*/
#define DRM_E_POLICY_ONLINE_DISABLED MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+90)


/*
*    MessageId:    DRM_E_CLK_NOT_SET
*    Message Meaning:    
*        License may be there but can not be used as secure clock not set.
*    Hex Value:    0x8004C05B
*/

#define DRM_E_CLK_NOT_SET       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+91)

/*
*    MessageId:    DRM_E_NO_CLK_SUPPORTED
*    Message Meaning:    
*        This device does not support any Clock. So time bound licenses can not be played
*    Hex Value:    0x8004C05C
*/

#define DRM_E_NO_CLK_SUPPORTED       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+92)

/*
*    MessageId:    DRM_E_NO_URL
*    Message Meaning:    
*        Can not find URL info.
*    Hex Value:    0x8004C05D
*/

#define DRM_E_NO_URL       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+93)

/*
*    MessageId:    DRM_E_UNKNOWN_PROPERTY
*    Message Meaning:    
*        Unknown device property.
*    Hex Value:    0x8004C05E
*/

#define DRM_E_UNKNOWN_PROPERTY       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+94)

/*
*   MessageId:  DRM_E_METERING_MID_MISMATCH
*   Message Meaning:    
*       The metering ID is not same in Metering Cert and metering response data
*   Hex Value:  0X8004C05F
*/
#define DRM_E_METERING_MID_MISMATCH        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+95)

/*
*   MessageId:  DRM_E_METERING_RESPONSE_DECRYPT_FAILED
*   Message Meaning:    
*       The encrypted section of metering response can not be decrypted
*   Hex Value:  0X8004C060
*/
#define DRM_E_METERING_RESPONSE_DECRYPT_FAILED        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+96)

/*
*   MessageId:  DRM_E_RIV_TOO_SMALL
*   Message Meaning:
*       RIV on the machine is too small.
*   Hex Value:  0X8004C063
*/
#define DRM_E_RIV_TOO_SMALL               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+99)

/*
**  License revocation errors; error codes from DRM_E_BASECODE+0xA0 to
**  DRM_E_BASECODE+0xA0 are reserved for license revocation errors
*/
#define DRM_E_LRB_BASECODE                      DRM_E_BASECODE+0xA0

/*
*    MessageId:    DRM_E_LRB_NOLGPUBKEY
*    Message Meaning:    
*        LRB does not contain a valid LGPUBKEY.
*    Hex Value:    0x8004c070
*/

#define DRM_E_LRB_NOLGPUBKEY                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LRB_BASECODE+0)

/*
*    MessageId:    DRM_E_LRB_INVALIDSIGNATURE
*    Message Meaning:    
*        Signature inside LRB is invalid.
*    Hex Value:    0x8004c071
*/

#define DRM_E_LRB_INVALIDSIGNATURE          MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LRB_BASECODE+1)

/*
*    MessageId:    DRM_E_LRB_LGPUBKEY_MISMATCH
*    Message Meaning:    
*        LRB is signed with a pubkey different from LGPUBKEY
*    Hex Value:    0x8004c072
*/

#define DRM_E_LRB_LGPUBKEY_MISMATCH         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LRB_BASECODE+2)

/*
*    MessageId:    DRM_E_LRB_INVALIDLICENSEDATA
*    Message Meaning:    
*        LRB is signed with a pubkey different from LGPUBKEY
*    Hex Value:    0x8004c073
*/

#define DRM_E_LRB_INVALIDLICENSEDATA        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LRB_BASECODE+3)

/*
**  Error codes returned from License evaluator
*/
#define DRM_E_LICEVAL_BASECODE                  DRM_E_BASECODE+0xC0

/*
*    MessageId:    DRM_E_LICEVAL_LICENSE_NOT_SUPPLIED
*    Message Meaning:  License not supplied in the liceval context  
*        
*    Hex Value:    0x8004c0c0
*/
#define DRM_E_LICEVAL_LICENSE_NOT_SUPPLIED      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LICEVAL_BASECODE+0)

/*
*    MessageId:   DRM_E_LICEVAL_KID_MISMATCH 
*    Message Meaning:  Mismatch between KID from header and the one inside license  
*        
*    Hex Value:    0x8004c0c1
*/
#define DRM_E_LICEVAL_KID_MISMATCH              MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LICEVAL_BASECODE+1)

/*
*    MessageId:   DRM_E_LICEVAL_LICENSE_REVOKED 
*    Message Meaning:  License for this content has been revoked  
*        
*    Hex Value:    0x8004c0c2
*/
#define DRM_E_LICEVAL_LICENSE_REVOKED           MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LICEVAL_BASECODE+2)

/*
*    MessageId:    DRM_E_LICEVAL_UPDATE_FAILURE
*    Message Meaning:   Failed to update content revocation 
*        
*    Hex Value:    0x8004c0c3
*/
#define DRM_E_LICEVAL_UPDATE_FAILURE            MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LICEVAL_BASECODE+3)

/*
*    MessageId:    DRM_E_LICEVAL_REQUIRED_REVOCATION_LIST_NOT_AVAILABLE
*    Message Meaning:   Failed to update content revocation 
*        
*    Hex Value:    0x8004c0c4
*/
#define DRM_E_LICEVAL_REQUIRED_REVOCATION_LIST_NOT_AVAILABLE MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_LICEVAL_BASECODE+4)


/*
   IContentHeader Events
*/
#define DRM_E_CH_BASECODE                ((DRM_RESULT)0x80041100L)

/*
*   MessageId:  DRM_E_CH_VERSION_MISSING
*   Message Meaning:    
*       Missing content header version.
*   Hex Value:  0x80041103L
*/

#define DRM_E_CH_VERSION_MISSING         ((DRM_RESULT)0x80041103L)

/*
*   MessageId:  DRM_E_CH_KID_MISSING
*   Message Meaning:    
*       Missing KID attribute in content header.
*   Hex Value:  0x80041104L
*/
#define DRM_E_CH_KID_MISSING             ((DRM_RESULT)0x80041104L)

/*
*   MessageId:  DRM_E_CH_LAINFO_MISSING
*   Message Meaning:    
*       Missing LAINFO attribute in content header.
*   Hex Value:  0x80041105L
*/
#define DRM_E_CH_LAINFO_MISSING          ((DRM_RESULT)0x80041105L)

/*
*   MessageId:  DRM_E_CH_CHECKSUM_MISSING
*   Message Meaning:    
*       Missing content header checksum.
*   Hex Value:  0x80041106L
*/
#define DRM_E_CH_CHECKSUM_MISSING        ((DRM_RESULT)0x80041106L)

/*
*   MessageId:  DRM_E_CH_ATTR_MISSING
*   Message Meaning:    
*       Missing content header attribute.
*   Hex Value:  0x80041107L
*/
#define DRM_E_CH_ATTR_MISSING            ((DRM_RESULT)0x80041107L)

/*
*   MessageId:  DRM_E_CH_INVALID_HEADER
*   Message Meaning:    
*       Invalid content header.
*   Hex Value:  0x80041108L
*/
#define DRM_E_CH_INVALID_HEADER          ((DRM_RESULT)0x80041108L)

/*
*   MessageId:  DRM_E_CH_UNABLE_TO_VERIFY
*   Message Meaning:    
*       Unable to verify signature of content header.
*   Hex Value:  0x8004110AL
*/
#define DRM_E_CH_UNABLE_TO_VERIFY        ((DRM_RESULT)0x8004110AL)

/*
*   MessageId:  DRM_E_CH_UNSUPPORTED_VERSION
*   Message Meaning:    
*       Unsupported content header version.
*   Hex Value:  0x8004110BL
*/
#define DRM_E_CH_UNSUPPORTED_VERSION     ((DRM_RESULT)0x8004110BL)

/*
*   MessageId:  DRM_E_CH_UNSUPPORTED_HASH_ALGORITHM
*   Message Meaning:    
*       Unsupported hash algorithm.
*   Hex Value:  0x8004110CL
*/
#define DRM_E_CH_UNSUPPORTED_HASH_ALGORITHM ((DRM_RESULT)0x8004110CL)

/*
*   MessageId:  DRM_E_CH_UNSUPPORTED_SIGN_ALGORITHM
*   Message Meaning:    
*       Unsupported signature algorithm.
*   Hex Value:  0x8004110DL
*/
#define DRM_E_CH_UNSUPPORTED_SIGN_ALGORITHM ((DRM_RESULT)0x8004110DL)

/*
*   MessageId:  DRM_E_CH_BAD_KEY
*   Message Meaning:    
*       Invalid key
*   Hex Value:  0x8004110EL
*/
#define DRM_E_CH_BAD_KEY                 ((DRM_RESULT)0x8004110EL)

/*
*   MessageId:  DRM_E_CH_NOT_SIGNED
*   Message Meaning:    
*       The header was not signed
*   Hex Value:  0x80041113L
*/
#define DRM_E_CH_NOT_SIGNED              ((DRM_RESULT)0x80041113L)

/*
*   MessageId:  DRM_E_CH_UNKNOWN_ERROR
*   Message Meaning:    
*       Unknown Error
*   Hex Value:  0x80041116L
*/
#define DRM_E_CH_UNKNOWN_ERROR           ((DRM_RESULT)0x80041116L)


/*
*   MessageId:  DRM_E_LIC_KEY_DECODE_FAILURE
*   Message Meaning:    
*       Key decode failure.%0
*   Hex Value:  0x80048007L
*/
#define DRM_E_LIC_KEY_DECODE_FAILURE     0x80048007L


/*
*   MessageId:  DRM_E_LIC_SIGNATURE_FAILURE
*   Message Meaning:    
*       License signature failure.%0
*   Hex Value:  0x80048008L
*/
#define DRM_E_LIC_SIGNATURE_FAILURE      0x80048008L

/*
*   MessageId:  DRM_E_LIC_KEY_AND_CERT_MISMATCH
*   Message Meaning:    
*       Key and cert mismatch.%0
*   Hex Value:  0x80048013L
*/
#define DRM_E_LIC_KEY_AND_CERT_MISMATCH      0x80048013L

/*
*   MessageId:  DRM_E_KEY_MISMATCH
*   Message Meaning:    
*       A public/private keypair is mismatched.%0
*   Hex Value:  0x80048013L
*/
#define DRM_E_KEY_MISMATCH                   0x80048014L

/*
*   MessageId:  DRM_E_INVALID_SIGNATURE
*   Message Meaning:    
*       License signature failure.%0
*   Hex Value:  0x800480CFL
*/
#define DRM_E_INVALID_SIGNATURE      0x800480CFL

/*
*   MessageId:  DRM_E_SYNC_ENTRYNOTFOUND
*   Message Meaning:    
*       an entry was not found in the sync store
*   Hex Value:  0x800480D0L
*/

#define DRM_E_SYNC_ENTRYNOTFOUND     0x800480D0L

/*
*   MessageId:  DRM_E_STACKTOOSMALL
*   Message Meaning:    
*       The stack supplied to the DRM API was too small
*   Hex Value:  0x800480D1L
*/

#define DRM_E_STACKTOOSMALL     0x800480D1L

/*
*   MessageId:  DRM_E_CIPHER_NOTINITIALIZED
*   Message Meaning:    
*       The DRM Cipher routines were not correctly initialized before calling
*       encryption/decryption routines.
*   Hex Value:  0x800480D2L
*/

#define DRM_E_CIPHER_NOTINITIALIZED     0x800480D2L

/*
*   MessageId:  DRM_E_DECRYPT_NOTINITIALIZED
*   Message Meaning:    
*       The DRM decrypt routines were not correctly initialized before trying to
*       decrypt data.
*   Hex Value:  0x800480D3L
*/
#define DRM_E_DECRYPT_NOTINITIALIZED     0x800480D3L

/*
*   MessageId:  DRM_E_SECURESTORE_LOCKNOTOBTAINED
*   Message Meaning:    
*       Before reading or writing data to securestore in raw mode, first the 
*       lock must be obtained using DRM_SST_OpenData.
*   Hex Value:  0x800480D4L
*/
#define DRM_E_SECURESTORE_LOCKNOTOBTAINED     0x800480D4L

/*
*   MessageId:  DRM_E_PKCRYPTO_FAILURE
*   Message Meaning:    
*       An error occured in an asymmetric cryptographic operation.
*   Hex Value:  0x800480D5L
*/
#define DRM_E_PKCRYPTO_FAILURE     0x800480D5L

/*
*   MessageId:  DRM_E_INVALID_HDSSLOTSIZE
*   Message Meaning:    
*       Invald HDS slot size is specified.
*   Hex Value:  0x800480D6L
*/
#define DRM_E_INVALID_HDSSLOTSIZE     0x800480D6L


/*
*   MessageId:  DRM_E_VERIFICATION_FAILURE
*   Message Meaning:
*       Validation of a Longhorn certificate failed
*   Hex Value:  0x80040E80L
*/
#define DRM_E_VERIFICATION_FAILURE    0x80040E80L

/*
*   MessageId:  DRM_E_RSA_ERROR
*   Message Meaning:
*       Error in an RSA call
*   Hex Value:  0x80040E82L
*/
#define DRM_E_RSA_ERROR             0x80040E82L

/*
*   MessageId:  DRM_E_BAD_RSA_EXPONENT
*   Message Meaning:
*       An incorrect RSA exponent was supplied for a public key
*   Hex Value:  0x80040E86L
*/
#define DRM_E_BAD_RSA_EXPONENT       0x80040E86L

/*
*   MessageId:  DRM_E_LOGICERR
*   Message Meaning:    
*        DRM code has a logic error in it.  This result should never be returned.  There is an unhandled code path if it is returned
*   Hex Value:  0x8004c3e8
*/
#define DRM_E_LOGICERR                      MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1000)

/*
*   MessageId:  DRM_E_INVALIDREVINFO
*   Message Meaning:    
*       The device certificate is invalid.
*   Hex Value:  0x8004c3e9
*/
#define DRM_E_INVALIDREVINFO                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1001)

/*
*   MessageId:  DRM_E_REVOCATION_BUFFERTOOSMALL
*   Message Meaning:    
*       The revocation buffer is too small, set a bigger one with DRM_MGR_SetRevocationBuffer
*   Hex Value:  0x8004c3eb
*/
#define DRM_E_REVOCATION_BUFFERTOOSMALL                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1003)

/*
*   MessageId:  DRM_E_DEVICE_ALREADY_REGISTERED
*   Message Meaning:    
*       There exists already a device in the device store with the same DEVICEID that was given.
*   Hex Value:  0x8004c3ec
*/
#define DRM_E_DEVICE_ALREADY_REGISTERED                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1004)

/*
*   MessageId:  DRM_E_DEVICE_NOT_REGISTERED
*   Message Meaning:    
*       The DEVICEID does not exist in the device store
*   Hex Value:  0x8004c3ed
*/
#define DRM_E_DEVICE_NOT_REGISTERED                    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1005)

/*
*   MessageId:  DRM_E_TOO_MANY_INCLUSION_GUIDS
*   Message Meaning:    
*       The license contained more than DRM_MAX_INCLUSION_GUIDS entries in its inclusion list
*   Hex Value:  0x8004c3ee
*/
#define DRM_E_TOO_MANY_INCLUSION_GUIDS                 MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRM_E_BASECODE+1006)

/* Range of expression evaluator results */

/*****************ERROR CODES ********************************/

#define CPRMEXP_BASECODE                        0x1400
#define CPRMEXP_NOERROR                         MAKE_DRM_RESULT(DRM_SEVERITY_SUCCESS,DRM_FACILITY_ITF, CPRMEXP_BASECODE)
#define CPRMEXP_PARAM_NOT_OPTIONAL              MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+1)
#define CPRMEXP_MEMORY_ALLOCATION_ERROR         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+2)   
#define CPRMEXP_NO_OPERANDS_IN_EXPRESSION       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+3)
#define CPRMEXP_INVALID_TOKEN                   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+4)
#define CPRMEXP_INVALID_CONSTANT                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+5)   
#define CPRMEXP_INVALID_VARIABLE                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+6)
#define CPRMEXP_INVALID_FUNCTION                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+7)
#define CPRMEXP_INVALID_ARGUMENT                MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+8)
#define CPRMEXP_INVALID_CONTEXT                 MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+9)
#define CPRMEXP_ENDOFBUFFER                     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+10)
#define CPRMEXP_MISSING_OPERAND                 MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+11)
#define CPRMEXP_OVERFLOW                        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+12)
#define CPRMEXP_UNDERFLOW                       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+13)
#define CPRMEXP_INCORRECT_NUM_ARGS              MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+14)
#define CPRMEXP_VARIABLE_EXPECTED               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+15)
#define CPRMEXP_RETRIEVAL_FAILURE               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+16)
#define CPRMEXP_UPDATE_FAILURE                  MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+17)
#define CPRMEXP_STRING_UNTERMINATED             MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+18)   
#define CPRMEXP_UPDATE_UNSUPPORTED              MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+19)   
#define CPRMEXP_ISOLATED_OPERAND_OR_OPERATOR    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+20)   
#define CPRMEXP_UNMATCHED                       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+21)
#define CPRMEXP_WRONG_TYPE_OPERAND              MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+22)
#define CPRMEXP_TOO_MANY_OPERANDS               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+23)
#define CPRMEXP_UNKNOWN_PARSE_ERROR             MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+24)
#define CPRMEXP_UNSUPPORTED_FUNCTION            MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+25)
#define CPRMEXP_CLOCK_REQUIRED                  MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, CPRMEXP_BASECODE+26)


#define DRMUTIL_BASECODE            0x9000
#define DRMUTIL_UNSUPPORTED_VERSION MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRMUTIL_BASECODE+5)
#define DRMUTIL_EXPIRED_CERT        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRMUTIL_BASECODE+6)
#define DRMUTIL_INVALID_CERT        MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, DRMUTIL_BASECODE+7)


/* Liceval Error Codes */
#define LICEVAL_BASECODE                        0x1300
#define LICEVAL_PARAM_NOT_OPTIONAL              MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+1)
#define LICEVAL_MEMORY_ALLOCATION_ERROR         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+2)
#define LICEVAL_INIT_FAILURE                    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+3)

#define LICEVAL_LICENSE_NOT_SUPPLIED            MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+4)
#define LICEVAL_UNSUPPORTED_LIC_VERSION         MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+5)
#define LICEVAL_INVALID_LICENSE                 MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+6)
#define LICEVAL_LICENSE_NOT_VALIDATED           MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+7)
#define LICEVAL_EVENT_MISSING                   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+8)
#define LICEVAL_FIELD_MISSING                   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+9)

#define LICEVAL_CONTENT_HEADER_NOT_SUPPLIED     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+10)
#define LICEVAL_UNSUPPORTED_CONTENT_VERSION     MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+11)
#define LICEVAL_INVALID_CONTENT_HEADER          MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+12)
#define LICEVAL_CONTENT_HEADER_NOT_VALIDATED    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+13)
#define LICEVAL_KID_MISMATCH                    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+14)
#define LICEVAL_CONTENT_SIGNATURE_FAILURE       MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+15)
#define LICEVAL_UNKNOWN_ERROR                   MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+16)
#define LICEVAL_LICENSE_REVOKED                 MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+17)
#define LICEVAL_RETRIEVAL_FAILURE               MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+18)
#define LICEVAL_UPDATE_FAILURE                  MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+19)
#define LICEVAL_BAD_CONTENT_REVOCATION          MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+20)
#define LICEVAL_EXPIRED_CERT                    MAKE_DRM_RESULT(DRM_SEVERITY_ERROR,  DRM_FACILITY_ITF, LICEVAL_BASECODE+21)


#ifdef __cplusplus
}
#endif


#endif /*__DRMRESULTS_H__ */
