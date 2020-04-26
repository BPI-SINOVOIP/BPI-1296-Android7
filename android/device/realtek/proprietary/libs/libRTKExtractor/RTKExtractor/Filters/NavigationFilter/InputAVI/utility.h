#ifndef AVI_PARSER_UTILITY_FUNCTION
#define AVI_PARSER_UTILITY_FUNCTION
#include <stdlib.h>
#include <string.h>

#define DIVX_SPIC_GET_VALUE(var, offset) ((int)(*(var+offset) - '0'))



static inline HRESULT DivX_Subtitle_Header_Duration(unsigned char *duration, int64_t *startPTS, int64_t *endPTS) {
	int startHour, startMin, startSec, startMSec;
	int endHour, endMin, endSec, endMSec;

	/* DivX Subtitle Packet
	   [HH:MM:SS.XXX-hh:mm:ss.xxx] 
	   0....5....0....5....0....5. */

#undef STRICT_CHECKING
#ifdef STRICT_CHECKING
	if(*(duration) != '[')
		return E_FAIL;
	if(*(duration+3) != ':')
		return E_FAIL;
	if(*(duration+6) != ':')
		return E_FAIL;
	if(*(duration+9) != '.')
		return E_FAIL;
	if(*(duration+13) != '-')
		return E_FAIL;
	if(*(duration+16) != ':')
		return E_FAIL;
	if(*(duration+19) != ':')
		return E_FAIL;
	if(*(duration+22) != '.')
		return E_FAIL;
	if(*(duration+26) != ']')
		return E_FAIL;
#endif
	
	startHour = DIVX_SPIC_GET_VALUE(duration, 1) * 10 + DIVX_SPIC_GET_VALUE(duration, 2);
	startMin = DIVX_SPIC_GET_VALUE(duration, 4) * 10 + DIVX_SPIC_GET_VALUE(duration, 5);
	startSec = DIVX_SPIC_GET_VALUE(duration, 7) * 10 + DIVX_SPIC_GET_VALUE(duration, 8);
	startMSec = DIVX_SPIC_GET_VALUE(duration, 10) * 100
			+ DIVX_SPIC_GET_VALUE(duration, 11) * 10 
			+ DIVX_SPIC_GET_VALUE(duration, 12);

	endHour = DIVX_SPIC_GET_VALUE(duration, 14) * 10 + DIVX_SPIC_GET_VALUE(duration, 15);
	endMin = DIVX_SPIC_GET_VALUE(duration, 17) * 10 + DIVX_SPIC_GET_VALUE(duration, 18);
	endSec = DIVX_SPIC_GET_VALUE(duration, 20) * 10 + DIVX_SPIC_GET_VALUE(duration, 21);
	endMSec = DIVX_SPIC_GET_VALUE(duration, 23) * 100
			+ DIVX_SPIC_GET_VALUE(duration, 24) * 10 
			+ DIVX_SPIC_GET_VALUE(duration, 25);

	*startPTS = 90000LL * (startHour * 3600LL + startMin * 60 + startSec) + 90LL * startMSec;
	*endPTS = 90000LL * (endHour * 3600LL + endMin * 60 + endSec) + 90LL * endMSec;

	return S_OK;
};

static inline ENUM_MEDIA_TYPE getVideoType(uint32_t biCompression) {
	
	switch(biCompression) {
		case FCC('MPG1'):
		case FCC('mpg1'):
		case FCC('mpg2'):
		case FCC('MPG2'):
			return MEDIASUBTYPE_MPEG2_VIDEO;
		case FCC('DIV3'):
		case FCC('div3'):
			#if defined(DIVX_HOME_THEATER) || defined(DIVX_ULTRA) || defined(DIVX_HD) || defined(DIVX_HD_51) || defined(DIVX_PLUS)
			return MEDIASUBTYPE_DIVX3_VIDEO;
			#else
			printf("video disabled due to Divx CodecID\n");
			return MEDIATYPE_None;	
			#endif
		case FCC('MP43'):
		case FCC('mp43'):
			return MEDIATYPE_None;
		case FCC('DSVD'):
		case FCC('DVCS'):
		case FCC('DVXD'):
			return MEDIATYPE_DV;
		case FCC('DIV4'):
		case FCC('div4'):
		case FCC('DIVX'):
		case FCC('Divx'):
		case FCC('divx'):
		case FCC('DX50'):
		case FCC('dx50'):
		#if defined(DIVX_HOME_THEATER) || defined(DIVX_ULTRA) || defined(DIVX_HD) || defined(DIVX_HD_51) || defined(DIVX_PLUS)
			return MEDIASUBTYPE_DIVX_VIDEO;
		#else
			printf("video disabled due to Divx CodecID\n");
			return MEDIATYPE_None;
		#endif
		case FCC('M4S2'):
		case FCC('m4s2'):
		case FCC('MP4S'):
		case FCC('mp4s'):
		case FCC('XVID'):
		case FCC('xvid'):
		case FCC('FMP4'):
		case FCC('fmp4'):
		case FCC('RMP4'):
		case FCC('rmp4'):
			return MEDIASUBTYPE_MPEG4_VIDEO;
		case FCC('MPEG'):
		case FCC('mpeg'):
			return MEDIASUBTYPE_MPEG2_VIDEO;
		case FCC('MJPG'):
		case FCC('mjpg'):
			return MEDIASUBTYPE_MJPEG_VIDEO;
		case FCC('H263'):
			return MEDIATYPE_None;
#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
		case FCC('AVC1'):
		case FCC('avc1'):
		case FCC('X264'):
		case FCC('H264'):
		case FCC('h264'):
			return MEDIASUBTYPE_H264_VIDEO;
		case FCC('WMV1'):
#if IS_CHIP(SATURN)
			return MEDIASUBTYPE_WMV7_VIDEO;
#else
			return MEDIATYPE_None;
#endif
		case FCC('WMV2'):
#if IS_CHIP(SATURN)
			return MEDIASUBTYPE_WMV8_VIDEO;
#else
			return MEDIATYPE_None; // MEDIASUBTYPE_VC1_WINDOWSMEDIA_VIDEO is not supported
#endif
		case FCC('WMV3'):
			return MEDIASUBTYPE_VC1_VIDEO; // @TODO: Need to do MP@ML / MP@LL
		case FCC('WMVA'):
		case FCC('WVC1'):
			return MEDIASUBTYPE_VC1_ADVANCED_VIDEO;
		case FCC('VP6F'):
		case FCC('vp6f'):
			return MEDIASUBTYPE_VP6_VIDEO;
#endif
		case FCC('FLV1'):
			return MEDIASUBTYPE_H263_VIDEO;
		case FCC('MPG4'):
		case FCC('MP42'):
		default:
			return MEDIATYPE_None;
	}
};


#endif
