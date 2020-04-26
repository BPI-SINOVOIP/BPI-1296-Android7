#ifndef __SOCKET_AV_DATA_H__
#define __SOCKET_AV_DATA_H__

typedef enum 
{
	SOCKET_DATA_NONE            = 0,
	SOCKET_DATA_VIDEO           = 1,
	SOCKET_DATA_AUDIO           = 2,
	SOCKET_DATA_SPIC            = 3,
	SOCKET_DATA_MEDIA_ATTRIBUTE = 4,
	SOCKET_DATA_INFO            = 5,
	SOCKET_DATA_SS_PACKET       = 6,	//standard specific packet
	
} SOCKET_DATA_TYPE;

typedef enum
{
	S_MEDIA_TYPE_NONE			= 0,
	S_MEDIA_TYPE_VIDEO_VC1		= 1,
	S_MEDIA_TYPE_VIDEO_MPEG,
	S_MEDIA_TYPE_VIDEO_H264,
	S_MEDIA_TYPE_VIDEO_FJPEG,
	S_MEDIA_TYPE_VIDEO_MPEG4,	//MUST set isDIVX4/ptsIncrement which affect frame pts interpolation.
	S_MEDIA_TYPE_VIDEO_WMV3,	//MUST set profile/pictureWidth/pictureHeight
	S_MEDIA_TYPE_VIDEO_WMV7,	//MUST to set profile/pictureWidth/pictureHeight
	S_MEDIA_TYPE_VIDEO_VP6,		//have to set ptsIncrement. if it is not available, a default value is used.
	S_MEDIA_TYPE_VIDEO_MJPEG,	//MUST set pictureWidth/pictureHeight
	S_MEDIA_TYPE_VIDEO_AVS,
	S_MEDIA_TYPE_VIDEO_H265,
	
	S_MEDIA_TYPE_AUDIO_MPEG		= 0x00001000,
	S_MEDIA_TYPE_AUDIO_HE_AAC,
	S_MEDIA_TYPE_AUDIO_AC3,
	S_MEDIA_TYPE_AUDIO_DDP,
	S_MEDIA_TYPE_AUDIO_LATM_AAC,
	S_MEDIA_TYPE_AUDIO_LPCM,	//little endian
	S_MEDIA_TYPE_AUDIO_WFD_LPCM,
	S_MEDIA_TYPE_AUDIO_AAC,		//has header
	S_MEDIA_TYPE_AUDIO_RAW_AAC,	//NO header, MUST set sbr flag/channel number/sample rate etc
								//and should insert sync word 0x30307762 in the beginning of each frame when deliver ES data
	S_MEDIA_TYPE_AUDIO_ALAW,
	S_MEDIA_TYPE_AUDIO_ADPCM,	//MUST set blockAlignSize/codecID 
	S_MEDIA_TYPE_AUDIO_DTS,
	S_MEDIA_TYPE_AUDIO_VORBIS,	//have to insert 8 bytes in the beginning of each packet when deliver ES data.
								//4 byte [sync_system] + 4 byte [packet_size] where sync_system = 0x30307762
	S_MEDIA_TYPE_AUDIO_WMAPRO,

	S_MEDIA_TYPE_SPU_TXT		= 0x00010000,
	
} SOCKET_MEDIA_TYPE;

typedef enum 
{
	SOCKET_INFO_NONE  = 0,
	SOCKET_INFO_EOS   = 1,		// no info data
	SOCKET_INFO_FLUSH = 2,		// long[2] - [0]: dataType, [1]: stamp
	SOCKET_INFO_IV    = 3,		// initial vector : char[16]
	SOCKET_INFO_KEYS  = 4,		// char[2][16]
	SOCKET_INFO_CLIENT_RESOLUTION = 5,	// long[2] - [0]: width, [1]: height
	SOCKET_INFO_CLEAR_SCREEN = 6,	// no info data
	SOCKET_INFO_PIC_INFO = 7,		// SOCKET_JPEG_PIC_INFO	
	SOCKET_INFO_DES_CONFIG = 8,		//descramble configuration : SOCKET_TS_DESC_CONFIG
	SOCKET_INFO_IV_OFFSET = 9,		// SOCKET_CW_OFFSET
	SOCKET_INFO_KEYS_OFFSET = 10,	// SOCKET_CW_OFFSET
	SOCKET_INFO_SPU_FRAME = 11,		// SOCKET_SPU_FRAME_INFO
	
} SOCKET_INFO_TYPE;

typedef enum
{
	SOCKET_CONTAINER_UNCHANGED = -1,
	SOCKET_CONTAINER_TS = 0,
	SOCKET_CONTAINER_ES = 1,
	SOCKET_CONTAINER_TS_AUTO_PARSE = 2,
	SOCKET_CONTAINER_PS = 3,
	SOCKET_CONTAINER_TS_OVER_RTP = 4,
} SOCKET_CONTAINER_TYPE;

typedef enum
{
	SOCKET_APP_NONE = 0,
	SOCKET_APP_WIRELESS_DISPLAY = 1,
	SOCKET_APP_WIRELESS_DISPLAY_WITHOUT_AUDIO = 2,
	SOCKET_APP_WIRELESS_DISPLAY_WITH_LOW_DELAY = 3,
	SOCKET_APP_MIRROROP_DISPLAY_H264 = 4,
	SOCKET_APP_YOUTUBE_DASH_DISPLAY = 5,
	SOCKET_APP_YOUTUBE_DASH_DISPLAY_VIDEOONLY = 6,
	SOCKET_APP_YOUTUBE_DASH_DISPLAY_AUDIOONLY = 7,
	SOCKET_APP_HLS_DISPLAY = 8,
	SOCKET_APP_SWPLAYER = 9,
} SOCKET_APP_TYPE;

typedef struct 
{
	SOCKET_DATA_TYPE type;		// if type is SOCKET_DATA_INFO and payloadSize > 0, it means payload is info data instead of av data
	SOCKET_INFO_TYPE flag;
	long long        pts;		// -1 means no PTS
	long             payloadSize;
	unsigned int	 rtp_marker_bit;
} SOCKET_DATA_HEADER;

typedef struct 
{
	long              videoAttrSize;	//number of bytes immediately following this field and up to audioAttrSize.
										//0 means no video attribute
	SOCKET_MEDIA_TYPE videoType;
	long              videoPid;			// -1 means unknown
	long              isDIVX4;			// -1 means unknown
	long              ptsIncrement;		// -1 means unknown
	long              profile;			// -1 means unknown
	long              pictureWidth;		// -1 means unknown
	long              pictureHeight;	// -1 means unknown
	
	
	long              audioAttrSize;	//number of bytes immediately following this field and up to spicAttrSize.
										//0 means no audio attribute
	SOCKET_MEDIA_TYPE audioType;
	long              channelNum;		// -1 means unknown
	long              sampleRate;		// -1 means unknown
	long              bitsPerSample;	// -1 means unknown
	long              audioPid;			// -1 means unknown
	long              sbrFlag;			// if audioType is S_MEDIA_TYPE_AUDIO_RAW_AAC, 1 means HE AAC
	long              codecID;			// WAVE form wFormatTag IDs. ref: http://wiki.multimedia.cx/index.php?title=TwoCC
	long              blockAlignSize;   // in bytes
	
	long              spicAttrSize;		//number of bytes immediately following this field.
										//0 means no spic attribute
	SOCKET_MEDIA_TYPE spicType;
	long              spicPid;			// -1 means unknown
	unsigned int      spicLangCode;		//ISO-639 language code
	
	long              contentAttrSize;
	long              packetSize;		//packet size for ts container
	SOCKET_CONTAINER_TYPE containerType;
	SOCKET_APP_TYPE	  appType;
	long              bMonitorPAT;
	long			  pcrPid;
	
} SOCKET_MEDIA_ATTRIBUTE;

typedef struct
{
	unsigned long   beginPhyAddr;
	unsigned long 	endPhyAddr;
	long 			size;
	unsigned long	readPtr;
	unsigned long	writePtr;
	unsigned long	readPtr2;	// currently, it used on togo DTV.
	
} SOCKET_SHARED_MEM;

typedef struct
{
	long sn;
	long x;
	long y;
	long width;
	long height;
	long picSize;
	long renderFlag;	// 0 means decode only while 1 means decode and render this picture.
	long long pts;
} SOCKET_JPEG_PIC_INFO;

typedef enum
{
	S_TS_NO_DESCRAMBLED = 0,
	S_TSP_LEVEL_DESCRAMBLED = 1,
	S_PES_LEVEL_DESCRAMBLED = 2,
} SOCKET_TS_DES_MODE;

typedef enum
{
	STDA_AES_CBC = 0,
	STDA_HDCP2_AES_CTR = 1,
	
}SOCKET_TS_DES_ALGO;

typedef struct
{
	SOCKET_TS_DES_MODE mode;
	SOCKET_TS_DES_ALGO algorithm;
	int round;	//this is reserved for multi2 
} SOCKET_TS_DESC_CONFIG;

typedef struct
{
	long evenKeyOffset;
	long oddKeyOffset;
} SOCKET_CW_OFFSET;

typedef struct
{
	unsigned int startTime;	//in milli seconds
	unsigned int duration;	//in milli seconds

	unsigned int textEncoding; //refer to Include/Platform_Lib/FontRenderer/charset_def.h
	
} SOCKET_SPU_FRAME_INFO;

#endif//__SOCKET_AV_DATA_H__
