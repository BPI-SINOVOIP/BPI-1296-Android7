#ifndef _SI_TYPES_H_
#define _SI_TYPES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
	#include "rtd_types.h"
	#include "rtd_messages.h"
	#include "siOs.h"
	#include "siApi.h"
	#include "tpapi.h"
	#include "drv_model.h"
	#include "SiEngine.h"
#elif defined(DVR)
	#include "rtd_types.h"
	#include "rtd_messages.h"
	#include "siOs.h"
	#include "siApi.h"
	#include "SiEngine.h"
#else
	#include <rtd_types.h>
	#include <rtd_messages.h>
	#include <si3/siOs.h>
	#include <si3/siApi.h>
	#include <si3/tpapi.h>
#endif


#ifdef DVR
	#define TP_MOD_COUNT		3
#else
	#define TP_MOD_COUNT		2
#endif

#ifdef WIN32
	#define SI_INFO 
	#define SI_WARNING	SiUtil_PrintWarning
	#define SI_ERROR	SiUtil_PrintError
//	#define SI_VERIFY	
	#define SI_VERIFY	SiUtil_Verify
#else
	#define SI_INFO(format, args...)
	#define SI_WARNING(format, args...)
	#define SI_ERROR(format, args...)
#ifdef DEBUG_SI
	//#define SI_VERIFY(format, args...) printf(format, ##args)
	#define SI_VERIFY	SiUtil_Verify
#else
	#define SI_VERIFY(format, args...)
#endif
	/*
	#define SI_INFO(format, args...) CLR_LOG_INFO(format, ##args)
	#define SI_WARNING(format, args...) CLR_LOG_WARN(format, ##args)
	#define SI_ERROR(format, args...) CLR_LOG_ERROR(format, ##args)
	#define SI_VERIFY
	*/
#endif



#ifdef TEST_MODE
	#ifdef TEST_CACHE_MONITOR
		#define MAX_PMT_COUNT				64
		#define MAX_EIT_COUNT				128
		#define MAX_ETT_COUNT				128
	#else
		#define MAX_PMT_COUNT				4		// no less than MAX_ACTIVE_PMT
		#define MAX_EIT_COUNT				6		// no less than MAX_ACTIVE_EVENT
		#define MAX_ETT_COUNT				6		// no less than MAX_ACTIVE_EVENT
	#endif
#else
	#define MAX_PMT_COUNT					64
	#define MAX_EIT_COUNT					128
	#define MAX_ETT_COUNT					128
#endif



#define GPS_UTC_BASE					315964800
#define MAX_RESERVATION_EVENT			15


#define DTV_FACTORY_TABLE				"dtvFactoryTable.txt"
#define DTV_TERRA_CHANNEL_LIST			"dtvChannelT.txt"
#define DTV_CABLE_CHANNEL_LIST			"dtvChannelC.txt"
#define DTV_FAV_CHANNEL					"dtvFavChannel"
#define DTV_RESERVATION					"dtvReservation.txt"
#define DTV_USER_CHANNEL				"dtvUserCh"
#define DTV_RRT_TABLE					"dtvRrt.txt"
#define DTV_SI_LOG						"siLog.txt"
#define DTV_CONFIG_TABLE				"SiConfigTable.txt"


#if defined(WIN32) || defined(TEST_CORE) || defined(TEST_BEANPACK)
	#define LOAD_DIR					""
	#define SAVE_DIR					""
#elif defined(DVR) || AP_CONFIG_WITH(APCFG_PRJ_DRAGON)
	#define LOAD_DIR					"./Resource/dtv_table/"
	#define LOAD_DIR2					"./resource/"
	#define SAVE_DIR					"/usr/local/etc/dvdplayer/"
#else
	#define LOAD_DIR					"/data/save/"
	#define SAVE_DIR					"/data/save/"
#endif


#define SiMalloc(ptr, size) {*ptr = malloc(size); if (*ptr != NULL) memset(*ptr, 0, size);}




typedef enum _TS_SOURCE {
	TSSRC_DEMOD0,///< Demodulator 0
	TSSRC_DEMOD1,///< Demodulator 1
	TSSRC_DMA,	 ///< From DMA.
	TSSRC_NUM
} TS_SOURCE;

typedef enum _SI_ERRCODE {
	SI_ERR_NULL = 0,
	SI_ERR_SUCCESS,
	SI_ERR_FAIL,
	SI_ERR_INTERRUPTED
} SI_ERRCODE;

typedef enum _SI_PES_TYPE {
	SI_PES_PCR,			// Use the dedicated PidFilter for PCR.
	SI_PES_VIDEO,		// Use the dedicated PidFilter for video.
	SI_PES_AUDIO,		// Use the dedicated PidFilter for audio.
	SI_PES_AUXAUDIO,	// Use the dedicated PidFilter for ancillary audio.
	SI_PES_SUBTITLE,	// Use the dedicated PidFilter for subtitle.
	SI_PES_TELETEXT,	// Use the private PidFilter.
	SI_PES_VBIDATA,		// Use the private PidFilter.
	SI_PES_DSMCC,		// Use the private PidFilter.
	SI_PES_PRIVATE,		// Private PES, other than VBI data, teletext and DSM-CC.
	  					// Only 'one' of this kind private PES can be filter at a time.
	SI_PES_TYPENUM		// The number of PES types
} SI_PES_TYPE;

typedef enum _SI_AUDIO_TYPE {
	SI_AT_UNDEFINED = 0,
	SI_AT_CLEAN_EFFECTS,
	SI_AT_HEARING_IMPAIRED,
	SI_AT_VISUAL_IMPAIRED
} SI_AUDIO_TYPE;

typedef enum _PMT_STREAM_TYPE {
	STYPE_ISO_11172_VIDEO = 0,
	STYPE_ISO_13818_VIDEO,
	STYPE_ISO_14496,		// MP4
	STYPE_ISO_11172_AUDIO,
	STYPE_ISO_13818_AUDIO,
	STYPE_ISO_13818_PRIVATE,
	STYPE_ISO_13818_PES_PRIVATE,
	STYPE_ISO_13522_MHEG,
	STYPE_ISO_13818_DSMCC,
	STYPE_ISO_13818_TYPE_A,
	STYPE_ISO_13818_TYPE_B,
	STYPE_ISO_13818_TYPE_C,
	STYPE_ISO_13818_TYPE_D,
	STYPE_ISO_13818_AUX,
	STYPE_ISO_13818_PART7_AUDIO,
	STYPE_ISO_14496_PART2_VIDEO,
	STYPE_ISO_14496_PART3_AUDIO,
	STYPE_ISO_14496_PART10_VIDEO,
	STYPE_H264,
	STYPE_VC1,
	STYPE_ATSC_AC3_AUDIO
} PMT_STREAM_TYPE;

typedef enum _TABLE_PID {
	//MPEG PSI
	PID_PAT = 0x0000,
	PID_CAT = 0x0001,
	PID_TSDT= 0x0002,
	//DVB SI
	PID_NIT = 0x0010,
	PID_SDT = 0x0011,
	PID_BAT = 0x0011,
	PID_EIT = 0x0012,
	PID_RST = 0x0013,
	PID_TDT = 0x0014,
	PID_TOT = 0x0014,
	//ATSC PSIP
	PID_ATSC= 0x1FFB,

	PID_NULL_PACKET = 0x1FFF	// 8191
} TABLE_PID;

typedef enum _TABLE_TID {

	//MPEG PSI
	TID_PAT		= 0x00,		// Program Association Table
	TID_CAT		= 0x01,		// Controll Access Table
	TID_PMT		= 0x02,		// Program Map Table
	TID_TSDT	= 0x03,		//

	// ATSC data download table Id.
	TID_SDDS1	= 0x3B,
	TID_SDDS2	= 0x3C,

	//DVB SI
	TID_NITa	= 0x40,		// actual Network Information Table
	TID_NITo	= 0x41,		// other Network Information Table
	TID_SDTa	= 0x42,		// actual Service Description Table
	TID_SDTo	= 0x46,		// other Service Description Table
	TID_BAT		= 0x4A,		// Bouquet Association Table
	TID_EITapf	= 0x4E,		// actual, present-following Event Information Table
	TID_EITopf	= 0x4F,		// other, present-following EIT
	TID_EITasd	= 0x50,		// 0x50 - 0x5F	actual, schedule EIT
	TID_EITosd	= 0x60,		// 0x60 - 0x6F	other, schedule EIT
	TID_TDT		= 0x70,		//
	TID_RST		= 0x71,		//
	TID_TOT		= 0x73,		//

	//ATSC PSIP
	TID_MGT		= 0xC7,		
	TID_TVCT	= 0xC8,		// Terristrial Virtual Channel Table.
	TID_CVCT	= 0xC9,		// Cable Virtual Channel Table.
	TID_RRT		= 0xCA,		// Region Rating Table.
	TID_EIT		= 0xCB,
	TID_ETT		= 0xCC,
	TID_STT		= 0xCD,
	TID_DCCT	= 0xD3,

	TID_EAM 	= 0xD8,

	TID_RESERVED= 0xFF

} TABLE_TID;

typedef enum _DESCRIPTOR_DID {

	DID_UNKNOWN = 0,
	DID_CONDITION_ACCESS,
	DID_SERVICE_LOCATION,
	DID_EXTENDED_NAME,
	DID_CLOSED_CAPTION,
	DID_CONTENT_ADVISORY,
	DID_AC3,
	DID_CATEGORY,
	DID_COMPONENT_NAME,
	DID_LANGUAGE,
	DID_TIMESHIFT

} DESCRIPTOR_DID;

typedef enum _SI_STREAM_TYPE {
	SI_STREAM_VIDEO				= 0x02,
	SI_STREAM_SYNC_DATA			= 0x06,
	SI_STREAM_ASYNC_DATA		= 0x0b,
	SI_STREAM_ADDRESSABLE		= 0x0d,
	SI_STREAM_SEC_CONTAIN_DATA	= 0x14,
	SI_STREAM_AUDIO				= 0x81,
	SI_STREAM_SEC_CONVERY_DATA	= 0x95,	
	SI_STREAM_SYNC_DATA2		= 0xc2
} SI_STREAM_TYPE;



typedef struct _SECTION_HEADER {

	UINT32	packetId;
	UINT8	tableId;
	UINT8	sectionNum;
	UINT8	lastSectionNum;
	UINT8	versionNum:5;
	UINT8	currentNext:1;
	UINT8	reserved1:2;

	UINT16	syntaxIndicator:1;
	UINT16	privateIndicator:1;
	UINT16	reserved2:2; 
	UINT16	sectionLen:12;
	UINT16	tableIdExt;

} SECTION_HEADER;

typedef struct _PARSED_SECTION {

	SECTION_HEADER	header;
	void *			pContent;

} PARSED_SECTION;

typedef enum {
	FILTER_IMMEDIATE_ENABLE,// enable PES/PSI filter immediatly after setting.
	FILTER_NOT_ENABLE		// not enable PES/PSI after setting, call EnablePESFilter()/EnablePSIFilter() to enable it.
} FilterEnable;


typedef enum _EF_EVENT {
	EF_NORMAL_DATA_ARRIV,	// Normal data arrival.
	EF_CCERR_DATA_ARRIV,	// Data arrival with CC-error.
	EF_DATA_DISCARD,		// Data discard because SIDB run out of memory
	EF_CALLBACK_CANCEL,		// This callback is cancelled.
	EF_HANDLE_INACTIVE		// This state of the corresponding EFhandle is changed to EF_INACTIVE. (Not implemented)
} EF_EVENT;

typedef enum _EF_STATE {
	EF_INACTIVE,			// The initial state. Does not get a pEs Filter yet.
	EF_READY,				// Already get a pEs Filter. Can disable it or get its handle.
	EF_RUN,					// The pEs Filter is running.
	EF_NULL
} EF_STATE;

typedef struct _EF_HANDLE{
	TPMOD		tpmod;		// Main or Sub TP module.
	SI_PES_TYPE estype;		// The PES type for this PES filter.
	UINT16		pid;		// The pid value to be filtered.
	EF_STATE	estat;		// The state of this handle.
	INT32		tpfehd;		// The assigned handle of TP_FilterEntry.
} EF_HANDLE;

typedef void(*EFcallback)(EF_HANDLE*, EF_EVENT , UINT8* , UINT32 , UINT8*);

/** ATSC modulation mode. (A65/B Table 6.5 Modulation Modes.) */
typedef enum _ATSC_MODULATION {
	amAnalog = 0x01,		// standard analog methods for analog TV.
	amSCTEmode1,			// Mode 1. symbol rate: 5.057 Msps, for 64-QAM typically.
	amSCTEmode2,			// Mode 2. symbol rate: 5.361 Msps, for 256-QAM typically.
	amATSC8VSB,				// 8-VSB modulation method.
	amATSC16VSB				// 16-VSB modulation method.
} ATSC_MODULATION;


typedef enum {
	SI_STRING_TYPE_ASCII = 0x100,
	SI_STRING_TYPE_UNICODE = 0x800
} SI_STRING_TYPE;

typedef enum {
	NOT_INDICATE = 0,
	OTHER_SURMOD = 1,
	DOLBY_SURMOD = 2,
	RESERVED = 3
} SI_SURROUND_MODE;

typedef struct _SI_STRING
{
	INT8 			iso639[3];	// ISO_639_language_code.
	SI_STRING_TYPE	type;
	UINT32 			len;		// length of the string.
	UINT8*			strBytes;	// string data.
} SI_STRING;


typedef struct _SI_MULTIPLE_STRING
{
	UINT16		stringCount;
	UINT16		stringMaxCount;
	SI_STRING*	stringList;
} SI_MULTIPLE_STRING;



//-------------------- Descriptor structs ---------------------------
typedef struct _SI_ATSC_EVENT_RATING {
	UINT8				dimIndex;		// rating_dimension_j
	UINT8				valueIndex:4;	// rating_value
	UINT8				reserved:4;	// rating_value
} SI_ATSC_EVENT_RATING;

typedef struct _SI_RATING_REGION {
	UINT8					regionId;	// rating_region
	UINT8					dimCount;	// rated_dimensions, shall not larger 											   than dimensions_defined in RRT.
	SI_ATSC_EVENT_RATING*	dimList;
	SI_MULTIPLE_STRING		ratingText;	// rating_description_text()
} SI_RATING_REGION;

typedef struct _SI_CONTENT_ADVISORY {
	UINT8				regionCount;	// Rating Region Count.
	SI_RATING_REGION*	regionList;		// Rating Region List.
} SI_CONTENT_ADVISORY;


typedef enum _CC_TYPE {
	CC_TYPE_NTSC = 0, 				// a line-21 closed caption service
	CC_TYPE_ATSC   				// an ATSC closed caption service
} CC_TYPE;

typedef struct _SI_CAPTION {
	char lang[3];			// ISO_639_language_code
	CC_TYPE ccType;// caption type

	UINT8 field :1;		// field1(0) or field(2) if ccType==cc_line21
	UINT8 ccNum :6;		// caption service number
	UINT8 easyReader :1;	// easy to reader?
	UINT8 wideAspRat :1;	// wide aspect ratio?
} SI_CAPTION;

typedef struct _SI_CAPTION_SERVICE {
	UINT8		ccCount;
	SI_CAPTION*	ccList;
} SI_CAPTION_SERVICE;


typedef struct _AC3_AUDIO {
	UINT8				sampleRateCode :3; 
	UINT8				bsid :5;			 			// 0:3-7 bsid, the same as it in AC-3 ES
	SI_SURROUND_MODE	surroundMode;					// surround_mode (TableA5 dsuimod Table)
	UINT8				bsmod :3;						// bsmod, the same as it in AC-3 ES
	UINT8				numChan :4;						// num_channels
	INT8				outChan;						// used for counting EIT 
	char				langcod1;			 			// langcod, the same as it in AC-3 ES
	char				langcod2;			 			// langcod2 if (num_channels==0)
} AC3_AUDIO;

typedef struct _SI_AC3 {
	UINT8			ac3Num;
	AC3_AUDIO*		ac3List;
} SI_AC3;

//-------------------- PAT table structs ---------------------------
typedef struct _PMT_INFO {

	int			progNum;		// Program Number.
	int			pmtPid;			// PMT pid.

} PMT_INFO;


typedef struct _PAT_CONTENT {

	UINT16		tsId;			// Transport Stream Id.
	UINT8		pmtCount;		// Number of PMT in pmtList.
	UINT8		version:5;
	UINT8		reservedBitError:3;	// reserved bit got set.
	PMT_INFO*	pmtList;

} PAT_CONTENT;


//-------------------- CAT table structs ---------------------------

typedef struct _CA_INFO {

	UINT16		systemId;
	UINT16		pid;
	UINT8		dataLen;
	UINT8*		privateDataByte;

} CA_INFO; 

typedef struct _CAT_CONTENT {

	CA_INFO		caInfo;			// Just allow 1 CA for now.

} CAT_CONTENT;


//-------------------- PMT table structs ---------------------------
/*
typedef struct _AC3_AUDIO {
	//UINT8 sampleRateCode :3; ///< 0:0-2 sample_rate_code
	enum atscAC3SampleRate sampleRate;
	UINT8 bsid :5;			 			// 0:3-7 bsid, the same as it in AC-3 ES
	//UINT8 bitRateCode :6;				// bit_rate_code
	enum atscBitRateCode bitRateClass;	// MSB exact(0)/upper_limit(1)
	enum atscBitRateTable bitRate;	  	// Bit Rate (kbit/s)
	UINT8 surroundMode :2;				// surround_mode (TableA5 dsuimod Table)
	UINT8 bsmod :3;						// bsmod, the same as it in AC-3 ES
	UINT8 numChan :4;					// num_channels
	UINT8 fullSvc :1;		 			// full_svc
	char langcod1;			 			// langcod, the same as it in AC-3 ES
	char langcod2;			 			// langcod2 if (num_channels==0)
	union {
		UINT8 mainid :3;				// if (bsmod<2)
		UINT8 asvcflags;				// else
	} audioService;
	UINT8 textLen :7; 		 			// text_code_length
	UINT8 textCod :1;		 			// text_code
	char *text;				 			// text()
	UINT8 addInfoLen;		 			// additional_info_length
	char *addInfo;			 			// additional_info
} AC3_AUDIO;
*/

typedef struct _PMT_AUDIO {
	UINT16				pid;
	UINT8				cmpTag;
	SI_MULTIPLE_STRING	atscCmpName;// ATSC PSIP Component Name
	char				lang[4];
	SI_AUDIO_TYPE		type;		// above 2 from ISO_639_language_descriptor
	SI_AC3				ac3;		// AC-3 audio descriptor.
	SI_AUDIO_CODEC		codecType;
	LPCM_SETTING		lpcm;
} PMT_AUDIO;

typedef struct _PMT_VIDEO {
	UINT16				pid;
	UINT8				cmpTag;
	SI_MULTIPLE_STRING	atscCmpName;	// ATSC PSIP Component Name
	char				lang[4];
} PMT_VIDEO;


typedef struct _PMT_CONTENT {
	UINT16				pid;
	UINT16				progNum;
	UINT16				pcrPid;

	UINT8				version;
	UINT8				audioCount;		// This records the number of audio elements in audioList (audioCount < audioMaxCount)
	UINT8				audioMaxCount;	// This records the max size of audioList.
	UINT8				redistributionControl:1;	// TRUE/FALSE.
	UINT8				reserved:7;		// not used.

	UINT8				videoStreamType;
	UINT8				vc1Profile;
	UINT8				isHDMV;			// Blueray stuff.

	PMT_VIDEO			video;
	PMT_AUDIO*			audioList;
	SI_SDDS_LIST		sdds;			// for storing software upgrade PID list.

#ifdef DVR
	UINT8				subtitleCount;		// This records the number of subtitle elements in subtitleList
	UINT8				subtitleMaxCount;	// This records the max size of subtitleList.
	UINT8				teletextCount;		// This records the number of teletext elements in teletextList
	UINT8				teletextMaxCount;	// This records the max size of teletextList.
	
	SI_TELETEXT*		teletextList;
	SI_SUBTITLE*		subtitleList;
#endif

	CA_INFO				ca;				// allow only 1 CA for now.
	SI_CONTENT_ADVISORY	contentAdvisory;	
	SI_CAPTION_SERVICE	captionService;
} PMT_CONTENT;


//-------------------- VCT table structs ---------------------------
typedef struct _SI_TIMESHIFTED_SERVICE {
	UINT32				minuteShifted;
	UINT16				majorNum;
	UINT16				minorNum;
} SI_TIMESHIFTED_SERVICE;

typedef struct _SI_TIMESHIFTED_SERVICE_LIST {
	int						count;
	SI_TIMESHIFTED_SERVICE*	list;
} SI_TIMESHIFTED_SERVICE_LIST;


typedef struct _SI_ELEMENT {
	SI_STREAM_TYPE	streamType;
	UINT16			pid;
	char			iso639[3];
} SI_ELEMENT;

typedef struct _SI_LOCATION {
	UINT16 				pcrPid;
	UINT8				elementCount;	// Service Location descriptor
	SI_ELEMENT*			elementList;	// Service Location descriptor
} SI_LOCATION;

typedef struct _CHANNEL_INFO {

	UINT8				shortName[CH_SHORTNAME_LEN+2];	// Channel short name.
	UINT16				chNumMajor;		// Major channel number.
	UINT16				chNumMinor;		// Minor channel number.

	UINT32				carrierFreq;	// carrier_frequency (Recommended value=0)
	UINT16				chanTsId;		// transport_stream_id or analog TSID
	UINT16				progNum;		// 0xffff(analog); 0x0000(inactive)

	UINT8				etmLocation:2;	// the existence/location of an ETM. use enum?
	UINT8				accessControl:1;// events associated with this virtual channel is access controlled or not.
	UINT8				hidden:1;		// this channel can be accessed by vChanNum or not.	Applications: test signals, NVOD services.
	UINT8				hideGuide:1;	// valid only when hidden bit is '1'; indicates	if the channel and its events appear in EPG.
	UINT8				path:1;			// Cable only.
	UINT8				outOfBand:1;	// Cable only.
	UINT8				reserved:1;

	UINT16				timeshiftBaseMajor; // This is the base channel for timeshift
	UINT16				timeshiftBaseMinor;

	SI_TIMESHIFTED_SERVICE_LIST timeShiftedList;
	UINT16				minuteShifted;	// From time shifted service.
	UINT16 				sourceId;		// the programming source associated with the virtual channel.	
	SI_SERVICE			atscService;	// ATSC Service Type
	ATSC_MODULATION		modulation;		// modulation_mode
	SI_LOCATION			location;		// Service Location descriptor

	SI_MULTIPLE_STRING	longName; 		// Extended Channel Name descriptor

} CHANNEL_INFO;

typedef struct _VCT_CONTENT {

	UINT16				tsId;			// Transport Stream Id.
	UINT16				tableId;		// TVCT or CVCT.
	UINT8				chCount;		// number of channels
	UINT8				version;
	CHANNEL_INFO*		chList;			// virtual channels in this transport stream

} VCT_CONTENT;


//-------------------- MGT table structs ---------------------------
typedef struct _MGT_TABLE {
	UINT16				type;			// table_type, e.g. TVCT_current, EIT-0. enum?
	UINT16				pid:13;			// table_type_PID
	UINT32				size;			// number_bytes
	UINT8				version:5;		// table_type_version_number
	UINT8				received:3;		// used to indicate if we have received this table.
} MGT_TABLE;

typedef struct _MGT_CONTENT {
	UINT8				eitCount;
	UINT8				ettCount;
	UINT8				version;
	UINT8				drrtCount;		// for RRT between 0x303 to 0x3FF
	MGT_TABLE*			eitList;		// eit table list (sorted)
	MGT_TABLE*			ettList;		// eit table list (sorted)
	MGT_TABLE*			chEtt;			// channel ett table.
} MGT_CONTENT;

//-------------------- EIT table structs ---------------------------

typedef struct _SI_ATSC_EVENT {
	UINT16				eventId;		// MSB 2 bits are always '11'
	UINT32				startTime;		// start_time
	UINT32				duration;		// length_in_seconds
	UINT8				etmLocation:2;	// use enum?
	UINT8				redistributionControl:1;
	UINT8				reserved;
	SI_CONTENT_ADVISORY	contentAdvisory;// loop in Content Advisory Descriptor.
	SI_CAPTION_SERVICE	captionService;	// caption services in Caption Service Descriptor.
	SI_CATEGORICAL_CODE categoryCode;	// category code in Genre Descriptor.
	UINT8				titleLen;		// title_length
	SI_MULTIPLE_STRING	title;			// title_text()
//	UINT8				rcInfoLen; 		// descriptor_length in Redistribution Control Descriptor 
//	UINT8*				rcInfo;			// rc_information() in Redistribution Control Descriptor
//	char				durationString[9]; //Is 8-byte enough to store time?
//	struct atscAC3audio *ac3dp;			// AC-3 audio descriptor.
	SI_AC3				ac3Audio;		// AC-3 audio descriptor.
} ATSC_EVENT;

typedef struct _ATSC_EIT_CONTENT {
	UINT16		sourceId;		// source_id
	UINT16		eventCount;		// num_events_in_section
	ATSC_EVENT*	eventList;		// events in this section.
} ATSC_EIT_CONTENT;


//-------------------- ETT table structs ---------------------------

typedef struct _ETT_CONTENT {
	UINT16				pid;
	UINT16				tidExt;		// ETT_table_id_extension, used as ETT instance ID.
	UINT32				etmId;		// ETM_id, unique identifier of this ETM.
	SI_MULTIPLE_STRING	etmMessage;	// extended_text_message(), i.e. ETM.
} ETT_CONTENT;

//-------------------- STT table structs ---------------------------
typedef struct _STT_CONTENT {
	UINT32				gpsTime;
	UINT8				leapSecond;
	UINT16				dayLightSavings;
} STT_CONTENT;


//-------------------- RRT table structs ---------------------------
typedef struct _SI_RATING_VALUE {
	SI_MULTIPLE_STRING	abbrev;		// abbrev_rating_value_text()
	SI_MULTIPLE_STRING	full;		// rating_value_text()
} SI_RATING_VALUE;

/** rating dimension */
typedef struct _SI_RATE_DIMENSION {
	SI_MULTIPLE_STRING	name;				// dimension_name_text()
	UINT8				reserved:3;			// reserved
	UINT8				graduatedScale:1;	// graduated_scale
	UINT8				valueNameCount:4;	// values_defined
	SI_RATING_VALUE*	valueNameList;		// rating value text 
} SI_RATE_DIMENSION;

typedef struct _RRT_CONTENT {
	UINT8				regionId; 		// rating_region
	UINT8				dimensionCount;	// the # of dimensions_defined
	SI_MULTIPLE_STRING	regionName; 	// rating_region_name_text()
	SI_RATE_DIMENSION*	dimensionList;	// rating dimensions
} RRT_CONTENT;


//-------------------- EAM table structs ---------------------------
#ifdef ENABLE_EAM
typedef struct _EAM_LOCATION {
 	UINT8				stateCode;
 	UINT16				countySubdivision:4;
 	UINT16				reserved:2;
 	UINT16				countryCode:10;
} EAM_LOCATION;

typedef struct _EXCEPTION_REFERENCE {
	UINT8				in_band_ref;
	UINT16				majorChNum;
	UINT16				minorChNum;
	UINT16				sourceID;
} EXCEPTION_REFERENCE;

typedef struct _EAM_CONTENT {
	UINT8					gotFromOOBFlag;

	UINT8					secNum;		// version detect

	UINT16					eventID;
	UINT8					originatorCode[3];
	UINT32					eventCodeLen:8;
	UINT8*					eventCodeList;
//	UINT8					activationTextLen;
//	UINT8*					activationText;
	SI_MULTIPLE_STRING		activationText;
	UINT8					msgRemainTime;
	UINT32					startTime;
	UINT16					duration;
	UINT8					alertPriority;
	UINT16					detailOOBSourceID;
	UINT16					detailMajorNum;
	UINT16					detailMinorNum;
	UINT16					audioSourceID;
//	UINT16					alertTextLen;
//	UINT8*					alertText;
	SI_MULTIPLE_STRING		alertText;
	UINT8					locationCodeCount;
	UINT8					exceptionCount;

	EAM_LOCATION*			locationCodeList;
	EXCEPTION_REFERENCE*	exceptionList;
} EAM_CONTENT;
#endif // ENABLE_EAM

//-------------------- EPG Reservation structs ---------------------------
typedef struct _SI_RESERVATION {
	UINT32				minBeginTime;
	SI_EVENT*			eventList[MAX_RESERVATION_EVENT];
	SI_EVENT*			expiredEvent;	// used for UI to query
} SI_RESERVATION;


typedef struct _SI_CHANNEL_STATUS {
	UINT32			frequency;
	INT16			majorNum;
	INT16			minorNum;

	UINT8			scrambled:1;
	UINT8			locked:1;
	UINT8			deleted:1;
	UINT8			ignored:1;

	BOOL			enable;
} SI_CHANNEL_STATUS;

//-------------------- SI Configuration structs ----------------------
typedef struct _SI_CONFIG {
	UINT8				enableDebugLog;
	UINT8			enableTableLogging;
	UINT8			tableLogFilePath[250];

	UINT8			checkCurrentNext;
	UINT8			checkPrivateIndicator;
	UINT8			checkSectionSyntax;
	UINT8			checkProtocolVersion;
	UINT8			checkReservedBits;
	UINT8			checkSttCrc;
	UINT8			checkServiceType;
	UINT8			checkTsId;
	UINT8			saveStreamChange;
	UINT8				updateDefaultRrt;
	UINT8				defaultLangId;

	UINT32			patTerraTimeOut;
	UINT32			pmtTerraTimeOut;
	UINT32			vctTerraTimeOut;
	UINT32			patCableTimeOut;
	UINT32			pmtCableTimeOut;
	UINT32			vctCableTimeOut;
	UINT8*				freqTablePath;
	
	SI_TIMEZONE		timezone;
} SI_CONFIG;

//-------------------- Global data structs ---------------------------
typedef struct _SI_GLOBAL_DATA {
		
	int 				tsId;				// Transport stream Id for current stream.
	UINT16 				physicalChIndex;	// Physical channel index for current frequency.
	UINT32 				frequency;			// Current frequency.
	UINT32				bandwidth;
	SI_MODULATION		modulation;
	UINT16				majorChNum;
	UINT16				minorChNum;
	UINT16				videoPid;			// Video pid that user is currently watching.
	UINT16				audioPid;			// Audio pid that user is currently listening.

	UINT16				eit0Pid;			// Used for rapid pid comparison on filtering.
	UINT16				ett0Pid;
	UINT8				mode:7;				// inactive, monitor, scan, watch
	UINT8				databaseIsDirty:1;
	UINT8				beanInterrupted:1;	// This flag forces parser to stop parsing.
	UINT8				stopParsing:1;
	UINT8				gotNewEit0:1;		// get ready to send out EIT0 message when we receive STT.
	UINT8				pmtHasAtscCC:1;
	UINT8				pmtHasNtscCC:1;
	UINT8				eitHasAtscCC:1;
	UINT8				eitHasNtscCC:1;
	UINT8				streamChanging:1;
	UINT8				broadcastResetMsg:1;// informs UI if channel reseting is required.
	UINT8				manualScan:1;			
	
	UINT8				tvMode;				// Terrestrial(0) or Open Cable(1) or File (2)
	UINT8				actionType;			// Normal, timeshift, live pause.
	UINT16				activeProgNum;
	UINT32 				alarmTime;			// Alarm set by UI when time is up.	
	UINT32				nextProgramTime;	// this records the time for the end of current program watching.

#ifdef ENABLE_SDDS
	UINT16				sddsPid;
#endif

//	UINT8				defaultLangId;		// Used to search for default audio language.
	UINT8				useUtf8:7;			// When TRUE, UTF8 strings are returned in SiApi functions.
	UINT8				useFileIFO:1;			// This means channel information is given from inputDTV filter.
	UINT16				pmtCount;
	PMT_CONTENT*		pmtContentList[MAX_PMT_COUNT];
	PAT_CONTENT*		patContent;
	VCT_CONTENT*		vctContent;
	MGT_CONTENT*		mgtContent;
	MGT_CONTENT*		mgtSection0;		// used as temporary storage for section 0.
	MGT_CONTENT*		mgtSection1;		// used as temporary storage for section 1.
	STT_CONTENT			sttContent;
	ATSC_EIT_CONTENT*	eit0Content;
	
#ifdef ENABLE_EAM
	EAM_CONTENT*		eamContent;
#endif

	SI_MULTI_RATING		multiRating;
	SI_RESERVATION		reservation;		// Used by UI to reserve programs to watch.
} SI_GLOBAL_DATA;


extern SI_GLOBAL_DATA siGlobal[TP_MOD_COUNT];
extern SI_CONFIG		siConfig;

#endif
