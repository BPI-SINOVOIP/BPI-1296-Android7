/*

   	Header file for AVI Parser : 
	extensions defined in DivX File Format Function Specification
			   
	Start Date: 2006/03/10
	Maintainer: Chih-pin Wu <wucp@realtek.com.tw>

	Revision 0.1
	2006/03/10	: add strd, strn part for multiple audio/multiple subpicture support
	2006/08/29	: Prepare for supporting DivX Ultra


*/
#ifndef DIVX_EX_H
#define DIVX_EX_H 

#ifndef AVIRIFF_H
#include "aviriff.h"
#endif

#if defined(DIVX_HOME_THEATER) || defined(DIVX_ULTRA)
	// DivX CE SDK v3.0.2
	#include "libDrmCommon/src/DrmTypes.h"
	#include "libDrmCommon/src/DrmErrors.h"
	#include "libDrmDecrypt/src/DrmAdpApi.h"
	#define ADP_SUCCESS DRM_ERROR_NONE
#elif defined(DIVX_HD)
	// DivX CE SDK v5.0.0
	#include "libDrmDevice/DrmApi.h"
	#include "libDrmDevice/DrmTypes.h"
	#define ADP_SUCCESS DRM_SUCCESS
#elif defined(DIVX_HD_51) || defined(DIVX_HOME_THEATER_3)
	// DivX CE SDK v5.1
	#include "libDrmDevice51/DrmApi.h"
	#include "libDrmDevice51/DrmTypes.h"
	#define ADP_SUCCESS DRM_SUCCESS
#elif defined(DIVX_PLUS)
	// DivX CE SDK v6.0
	#include "Device/DrmApi.h"
	#include "Device/DrmTypes.h"
	#include "DeviceLocal/DrmSystem.h"
	#define ADP_SUCCESS DRM_SUCCESS
#endif

#ifndef ckidSTREAMDRM
#define ckidSTREAMDRM FCC('strd')
#endif

#ifndef ckidSTREAMNAME
#define ckidSTREAMNAME FCC('strn')
#endif

typedef struct _subtitlechunk {
	uint32_t  fcc;       // '##sb'
	uint32_t  cb;        // size
	char*      *subPacket;
} SUBTITLECHUNK;

typedef struct _strd_chunk {
	uint32_t 			fcc;
	uint32_t 			cb;
	uint32_t 			dwVersion;
	uint32_t 			dwSize;
#if defined(DIVX_HD_51) || defined(DIVX_HOME_THEATER_3) || defined(DIVX_PLUS)
	char*				drmHeader;
#elif defined(DIVX_HD)
	drmHeader_t			drmHeader;
#elif defined(DIVX_HOME_THEATER) || defined(DIVX_ULTRA)
	DrmHeader 			drmHeader;
#endif
#if defined DIVX_HOME_THEATER || defined DIVX_ULTRA || defined DIVX_HD
#endif
} STRD_CHUNK;

#if defined DIVX_HOME_THEATER || defined DIVX_ULTRA
#elif defined(DIVX_HD) || defined(DIVX_HD_51) || defined(DIVX_HOME_THEATER_3) || defined(DIVX_PLUS)
// Internal errors. (ADP-4.0.0 Only)
#define ADP_BAD_PARAMETER                   10
#define DRM_ERROR_DECRYPTION_FAILED			11  // from: drmDecryptFrame
#define DRM_ERROR_UNRECOGNIZED_DRM_MODE		14  // from: drmInitPlaybackContext
#define DRM_ERROR_NOT_AUTH_USER             15  // from: drmQueryRentalStatus, drmCommitPlayback
#define DRM_ERROR_RENTAL_EXPIRED            16  // from: drmQueryRentalStatus, drmCommitPlayback
#define DRM_ERROR_NEED_MORE_RANDOM_SAMPLE	17  // from: drmGetRegistrationCodeString, drmCommitPlayback
#define DRM_ERROR_NOT_INITIALIZED			18  // from: drmQueryRentalStatus
#define DRM_ERROR_RESERVED               	19
#define DRM_ERROR_NOT_COMMITTED				20  // from: drmDecryptFrame
#define DRM_ERROR_NOT_RENTAL_QUERIED		21  // from: drmQueryCgmsa
#define DRM_ERROR_BAD_SLOT_NUMBER           22  // from: drmQueryRentalStatus
#define DRM_ERROR_NULL_GUARD_SET_SIGNAL		23	// from: drmCommitPlayback (for certification testing support)
#define DRM_ERROR_INVALID_ALIGNMENT			24	// from: drmInitPlaybackContext
#define DRM_ERROR_NOT_CGMSA_QUERIED         25  // from: drmQueryAcptb
#define DRM_ERROR_NOT_ACPTB_QUERIED         26  // from: drmQueryDigitalProtection
#define DRM_ERROR_NOT_DIGITAL_QUERIED       27  // from: drmCommitPlayback
#define DRM_ERROR_SIGNATURE_MISMATCH        28  // from: drmInitPlaybackContext
#endif
//#include "divx_ultra.h"

#endif
