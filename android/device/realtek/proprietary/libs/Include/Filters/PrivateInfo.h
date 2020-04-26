#ifndef __PRIVATE_INFO_H__
#define __PRIVATE_INFO_H__

/* Private Info Definition - To Exchange Information Between Filters */

#include <OSAL.h>
#include <StreamClass/EType.h>
#include <AudioInbandAPI.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Private Info Identifiers ***************************************************/

typedef enum {

    /* to indicate there is no private info  0 */
    PRIVATEINFO_NONE,

    /* sending PTS information (int64_t)  1*/
    PRIVATEINFO_PTS,

    /* sending payload information (PAYLOADINFO)  2*/
    PRIVATEINFO_PAYLOAD_INFO,

    /* sending demux the video target information (DEMUXTARGET) 3*/
    PRIVATEINFO_DEMUX_VIDEO_TARGET,

    /* sending demux the audio target information (DEMUXTARGET) 4*/
    PRIVATEINFO_DEMUX_AUDIO_TARGET,

    /* sending demux the SPU target information (DEMUXTARGET) 5 */
    PRIVATEINFO_DEMUX_SPU_TARGET,

    /* sending demux the updated STC offset (int64_t) 6*/
    PRIVATEINFO_DEMUX_STC_OFFSET,

    /* sending SPU button information in DVD-Video compatible format (SPUDVDVBUTTON) 7*/
    PRIVATEINFO_SPU_DVDV_BUTTON,

    /* sending SPU palette in DVD-Video compatible format (SPUDVDVPALETTE) 8*/
    PRIVATEINFO_SPU_DVDV_PALETTE,

    /* to turn on/off SPU (int, 1:ON ; 0:OFF) 9*/
    PRIVATEINFO_SPU_ON_OFF,

    /* to set SPU configuration (coding type, screen size) (SPUCONFIG) 10*/
    PRIVATEINFO_SPU_CONFIG,

    /* to flush SPU alone (no argument) 11*/
    PRIVATEINFO_SPU_FLUSH,

    /* sending audio format information (AUDIOFORMAT) 12*/
    PRIVATEINFO_AUDIO_FORMAT,

    /* sending audio context information for AOUT feedback (int) 13*/
    PRIVATEINFO_AUDIO_CONTEXT,

    /* audio decoding command (AUDIODECODECOMMAND) 14*/
    PRIVATEINFO_AUDIO_DECODE_COMMAND,

    /* to flush audio alone (no argument) 15*/
    PRIVATEINFO_AUDIO_FLUSH,

	/* add by stream : to mark the position that data is discontinuity 16*/
	PRIVATEINFO_AUDIO_DATA_DISCONTINUITY,

    /* sending video end-of-sequence (no argument) 17*/
    PRIVATEINFO_VIDEO_END_OF_SEQUENCE,

    /* sending VOBU information from DVD navigation (VIDEODVDVOBUINFO) 18*/
    PRIVATEINFO_VIDEO_DVD_VOBU_INFO,

    /* sending DCI_CCI information from DVD navigation (VIDEODVDDCICCI) 19*/
    PRIVATEINFO_VIDEO_DVD_DCI_CCI,

    /* sending V_ATR information from DVD navigation (unsigned int) 20*/
    PRIVATEINFO_VIDEO_DVD_V_ATR,

    /* sending video presentation mode (NAV_VIDEO_PRES_MODE, normal/wide/letter-box/pan-scan) 21*/
    PRIVATEINFO_VIDEO_PRESENTATION_MODE,

    /* sending video context information for VOUT feedback (int) 22*/
    PRIVATEINFO_VIDEO_CONTEXT,

    /* video decoding command (VIDEODECODECOMMAND) 23*/
    PRIVATEINFO_VIDEO_DECODE_COMMAND,

    /* video decoding command for output to buffer (VIDEODECODETOBUFFER) 24*/
    PRIVATEINFO_VIDEO_DECODE_TO_BUFFER,

    /* sending background color setting for video mixer (VIDEOMIXERBGCOLOR) 25*/
    PRIVATEINFO_VIDEO_MIXER_BG_COLOR,

    /* setting RPTS of video mixer (int64_t) 26*/
    PRIVATEINFO_VIDEO_MIXER_RPTS,

    /* sending blending command for video mixer (VIDEOMIXERBLEND) 27*/
    PRIVATEINFO_VIDEO_MIXER_BLEND,

    /* sending video format information (VIDEOFORMAT) 28*/
    PRIVATEINFO_VIDEO_FORMAT,

	/* sending DV auxilary information (VIDEODVVAUX)  :29*/
	PRIVATEINFO_VIDEO_DV_VAUX,

	/* sending DV auxilary information (VIDEODVVAUX)  :30*/
	PRIVATEINFO_VIDEO_DV_FF,

	/* sending DV auxilary information (VIDEODVVAUX)  :31*/
	PRIVATEINFO_VIDEO_DV_BUSRESET,

	/* sending DV auxilary information (VIDEODVVAUX)  :32*/
	PRIVATEINFO_VIDEO_DV_ENDSEGMENT,

	/* sending to MPEG4 decoder to indicate it's DIVX or not (VIDEODIVX4) 33*/
	PRIVATEINFO_VIDEO_MPEG4_DIVX4,

    /* setting video decoder error concealment level (int, 0~256) 34*/
    PRIVATEINFO_VIDEO_ERROR_CONCEAL_LEVEL,

    /* sending audio OGG_vorbis information (AUDIOVORBISPKTINFO) 35*/
    PRIVATEINFO_AUDIO_OGGVORBIS_COMMAND,

    /* 36*/
    PRIVATEINFO_SPU_PAGE_ID,

    /* sending audio out to drop sample by PTS 37*/
    PRIVATEINFO_AUDIO_DROPSAMPLE_COMMAND,

    /* sending demux the target information for transport stream (DEMUXTARGET_TS) 38*/
    PRIVATEINFO_DEMUX_TRANSPORT_TARGET,

    /* sending demux the transport stream packet size (int) 39*/
    PRIVATEINFO_DEMUX_TRANSPORT_PACKET_SIZE,

    /* to flush teletext alone (no argument) 40*/
    PRIVATEINFO_TELETEXT_FLUSH,

    /* sending video decoder the RM format info (VIDEORMFORMAT) 41*/
    PRIVATEINFO_VIDEO_RM_FORMAT,

    /* sending video decoder the RM frame info (VIDEORMFRAMEINFO) 42*/
    PRIVATEINFO_VIDEO_RM_FRAME_INFO,

	/* sending video decoder the RM segment info (VIDEORMSEGMENT) 43*/
    PRIVATEINFO_VIDEO_RM_SEGMENT_INFO,

    /* sending audio CGMS info (CGMS_INFO, enum) 44*/
    PRIVATEINFO_AUDIO_CGMS,

    /* to indicate the start of the packet (long) 45*/
    PRIVATEINFO_AUDIO_RM_PACKET_START,

/* send wma DRC settings (WMAPRODRC) 46 */
    PRIVATEINFO_AUDIO_WMAPRO_DRC,

    /* send numbers of row and column of wma down mix martix (WMAPRODOWNMIXMATRIX) 47 */
    PRIVATEINFO_AUDIO_WMAPRO_DOWNMIX_MATRIX,

    /* send video vc1 info (VIDEOVC1SEQINFO) 48 */
    PRIVATEINFO_VIDEO_VC1_INFO,

    /* send DV Display Information 49 */
    PRIVATEINFO_VIDEO_DV_DISPLAY_CONTROL,

    /* send h264 video usability information 50 */
    PRIVATEINFO_VIDEO_USABILITY_INFO,

	/* send mpeg4 video usability information 51 */
	PRIVATEINFO_VIDEO_MPEG4_USABILITY_INFO,

	/* start delivering data after start pts 52 */
	PRIVATEINFO_DEMUX_START_PTS,

	/*deliver both video and audio context 53 */
	PRIVATEINFO_AV_CONTEXT,

	/*deliver New Video Format*/
	PRIVATEINFO_NEW_VIDEO_FORMAT,

	/*set offset to physical address 55 (NAVIOBUFMAPINFO)*/
	PRIVATEINFO_OFFSET_TO_PHYSICAL_ADDR,

	/* send video vc1 info with only 32-integered extraData(VIDEOVC1SEQINFO) 56 */
    PRIVATEINFO_VIDEO_VC1_MINI_INFO,

    /* send 3D subtitle offset sequence id to video decoder (unsigned char) 57*/
    PRIVATEINFO_VIDEO_OFFSET_SEQ_ID,

    /* sending audio out to enable/disable dropping sample by fullness 58 (for skype)*/
    PRIVATEINFO_AUDIO_DROPSAMPLE_BYFULLNESS_COMMAND,

    /* send Initial Vector to TS demux for content decryption (unsigned char[16]) 58 */
    PRIVATEINFO_DEMUX_TRANSPORT_IV,

    /* send keys to TS demux for content decryption 59 (NAVDEMUXKEY)*/
    PRIVATEINFO_DEMUX_TRANSPORT_KEYS,

    /* send client resolution 60 (VIDEOFORMAT) */
    PRIVATEINFO_VIDEO_MJPG_RESOLUTION,

    /*clear screen 61 */
    PRIVATEINFO_VIDEO_FJPEG_CLEAR_SCREEN,

    /* send picture info 62 (VIDEOFJPEGPICINFO)*/
    PRIVATEINFO_VIDEO_FJPEG_PICINFO,

    /* notify video in DPB by pass mode 63 */
    PRIVATEINFO_VIDEO_DPBBYPASS_MODE,

    /* pass ts descramble info to software descrambler 64 (DESCRAMBLE_CONFIG)*/
    PRIVATEINFO_DEMUX_TS_DESCRAMBLE_INFO,

    /* In Widi MPEG2 need ignore aspect ratio */
    PRIVATEINFO_VIDEO_DEC_IGNORE_BS_ASPECT_RATIO,

    /* set key offset in CW 66 (NAVDEMUXCWOFFSET)*/
    PRIVATEINFO_DEMUX_TRANSPORT_KEYS_OFFSET,

    /* set iv offset in CW 67 (NAVDEMUXCWOFFSET)*/
    PRIVATEINFO_DEMUX_TRANSPORT_IV_OFFSET,

    /* Use this infoId to deliver skip info */
    PRIVATEINFO_AUDIO_APE_SKIPBITS,
	
	/* notify video fw the marker bit in RTP header */
	PRIVATEINFO_RTP_HEADER_MARKER_BIT,

	/* notify video fw to do the rotation */
	PRIVATEINFO_VIDEO_ROTATION,

} PRIVATEINFO_IDENTIFIER;

/* Private Info Format Definitions ********************************************/

/* payload information (PRIVATEINFO_PAYLOAD_INFO) */
typedef struct _tagPAYLOADINFO {
    unsigned long byteCount;    /* counting total number of bytes fed before this payload */
    unsigned long startAddress; /* start address of input unit containing this payload */
    unsigned long endAddress;   /* end address of input unit containing this payload */
} PAYLOADINFO;

/* demux target information (PRIVATEINFO_DEMUX_xxxx_TARGET) */
typedef struct _tagDEMUXTARGET {
    bool bActive; /* false means we don't deliver anything to the target */
    int id;       /* stream id, special cases enumerated as below */
    int subId;    /* sub stream id, -1 means there is no such id to match */
    int subId2;   /* sub stream id2, -1 means there is no such id to match */
} DEMUXTARGET;

typedef enum {
    DEMUXTARGET_AUTO_SELECT = -1001
} DEMUXTARGET_SPECIAL_ID;

/* demux target information for transport stream (PRIVATEINFO_DEMUX_TRANSPORT_TARGET) */
typedef struct _tagDEMUXTARGET_TS_ENTRY {
    int  pid;                /* PID of the transport stream packet of interest */
    int  type;               /* SI_FILTER_TYPE (video/audio/subtitle/teletext or si), defined in "./NavigationFilter/SiEngine.h" */
    int  codecType;          /* SI_CODEC_TYPE (mp1/mp2/mp4/ac3/dts/aac ...), defined in "./NavigationFilter/SiEngine.h" */
    bool bKeepPES;           /* true means the PES header will be kept, otherwise stripped */
    bool bCheckContiCounter; /* true means the continuity counter will be checked */
    bool bNewTarget;         /* true means it's a new demux target just enabled */
	bool bActive;
} DEMUXTARGET_TS_ENTRY;

typedef struct _tagDEMUXTARGET_TS {
    DEMUXTARGET_TS_ENTRY* entries;
    unsigned int number_of_entries;
    int pcr_pid; /* PID where demux can find PCR and feedback to navigator (-1 means demux is not requested to do so) */
    ENUM_MEDIA_TYPE sourceType;		/* MEDIATYPE_FILE for file without IFO while MEDIATYPE_STB for tuner or file with IFO */
} DEMUXTARGET_TS;

/* SPU button information (PRIVATEINFO_SPU_DVDV_BUTTON) */
typedef struct _tagSPUDVDVBUTTON {
    int64_t        ptmStart;       /* button display start PTS */
    int64_t        ptmEnd;         /* button display end PTS, -1 means INFINITE */
    unsigned short xStart;         /* button area coordinates */
    unsigned short xEnd;
    unsigned short yStart;
    unsigned short yEnd;
    unsigned char  colorEmphasis1; /* button color & contrast */
    unsigned char  colorEmphasis2;
    unsigned char  colorPattern;
    unsigned char  colorBackground;
    unsigned char  contrastEmphasis1;
    unsigned char  contrastEmphasis2;
    unsigned char  contrastPattern;
    unsigned char  contrastBackground;
    bool           bEnable;        /* true if highlight is enabled */
} SPUDVDVBUTTON;

/* SPU palette (PRIVATEINFO_SPU_DVDV_PALETTE)    */
/* see PGC_SP_PLT in DVD-Video spec, page VI4-84 */
typedef struct _tagSPUDVDVPALETTE {
    unsigned int color[16];
} SPUDVDVPALETTE;

/* SPU configuration (PRIVATEINFO_SPU_CONFIG) */
typedef struct _tagSPUCONFIG {
    unsigned int   type; /* refer to InbandAPI.h */
    unsigned short screen_width;
    unsigned short screen_height;
} SPUCONFIG;

/* SPU page id slection(PRIVATEINFO_SPU_PAGE_ID) */
typedef struct _tagSPUPAGEID{
    unsigned short page_id_composition;
    unsigned short page_id_ancillary;
} SPUPAGEID;

/* audio format information (PRIVATEINFO_AUDIO_FORMAT) */
typedef struct _tagAUDIOFORMAT {
    ENUM_MEDIA_TYPE type;          /* format code, see "StreamClass/EType.h" */
    ENUM_MEDIA_TYPE systemType;    /* system format code, see "StreamClass/EType.h" */
    unsigned char   emphasis;      /* non-zero (true) means emphasis ON */
    unsigned char   mute;          /* non-zero (true) means mute ON */
    unsigned char   bitsPerSample;
    unsigned char   numberOfChannels;
    unsigned int    samplingRate;
    unsigned int    dynamicRange;  /* follow DVD-V spec, Page VI5-22 */
    int privateData[6];//add by yllin 2007.02.013, change size 2 to 6 by taro 2010.05.17
} AUDIOFORMAT;

/* audio decoding command for one segment (PRIVATEINFO_AUDIO_DECODE_COMMAND) */
typedef struct _tagAUDIODECODECOMMAND {
    unsigned int mode;   /* refer to struct DEC_MODE in AudioInbandAPI.h */
    int64_t relativePTS; /* decode start relative PTS */
    int64_t duration;    /* PTS duration of decoded segment */
} AUDIODECODECOMMAND;

/* VOBU information from DVD navigation (PRIVATEINFO_VIDEO_DVD_VOBU_INFO) */
typedef struct _tagVIDEODVDVOBUINFO {
    int64_t startPTM;
    int64_t endPTM;
    int64_t RPTS;
    bool    bStillVOBU;
} VIDEODVDVOBUINFO;

/* DCI_CCI information from DVD navigation (PRIVATEINFO_VIDEO_DVD_DCI_CCI) */
typedef struct _tagVIDEODVDDCICCI {
    unsigned int DCI;
    unsigned int CCI;
} VIDEODVDDCICCI;

/* video decoding command for one segment (PRIVATEINFO_VIDEO_DECODE_COMMAND) */
typedef struct _tagVIDEODECODECOMMAND {
    unsigned int mode;   /* refer to struct DECODE_MODE in InbandAPI.h */
    int64_t relativePTS; /* decode start relative PTS */
    int64_t duration;    /* PTS duration of decoded segment */
    long skip_GOP;       /* skipped GOP before decode start */
} VIDEODECODECOMMAND;

/* decoding command for output to buffer (PRIVATEINFO_VIDEO_DECODE_TO_BUFFER) */
typedef struct _tagVIDEODECODETOBUFFER {
    unsigned int colorFormat;      /* 0: IC_RGB332 , 1: IC_RGB565, 2: IC_RGBA8888, 3: IC_ARGB8888, 4:IC_YUV  */
    unsigned char* pTargetLuma;    /* target buffer address for luma, or while frame for RGB mode */
    unsigned char* pTargetChroma;  /* target buffer address for chroma, for YUV mode only */
    unsigned int pitch;            /* number of bytes to advance from beginning of one line to the next */
    unsigned int targetRectX;      /* coordinate X in pixels for the left-upper corner of target rect */
    unsigned int targetRectY;      /* coordinate Y in pixels for the left-upper corner of target rect */
    unsigned int targetRectWidth;  /* target rectangle width in pixels */
    unsigned int targetRectHeight; /* target rectangle height in pixels */
	unsigned int rotation;         /* 0: 0, 1: 90, 2: 180, 3: 270 */
	unsigned int alpha;
    unsigned int ID;
} VIDEODECODETOBUFFER;

/* background color setting for video mixer (PRIVATEINFO_VIDEO_MIXER_BG_COLOR) */
typedef struct _tagVIDEOMIXERBGCOLOR {
    unsigned char Y;
    unsigned char U;
    unsigned char V;
} VIDEOMIXERBGCOLOR;

/* blending command for video mixer (PRIVATEINFO_VIDEO_MIXER_BLEND) */
typedef struct _tagVIDEOMIXERBLEND {
    unsigned int mode;       /* 0: reserved ; 1: A only ; 2: B only ; 3: AB */
    int64_t duration;        /* PTS duration of blended segment */
    unsigned char alphaA_Y;  /* Ignored in BlendMode 0, 0 to 255 */
    unsigned char alphaA_UV; /* Ignored in BlendMode 0, 0 to 255 */
    unsigned char alphaB_Y;  /* Ignored in BlendMode 1, 0 to 255 */
    unsigned char alphaB_UV; /* Ignored in BlendMode 1, 0 to 255 */
} VIDEOMIXERBLEND;

/* video format information (PRIVATEINFO_VIDEO_FORMAT) */
typedef struct _tagVIDEOFORMAT {
    unsigned int    dwWidth;
    unsigned int    dwHeight;
} VIDEOFORMAT;

/* DV auxilary information */
typedef struct _tagVIDEODVVAUX {
	long  VS;
	long  VSC;
        long  REC_DATE ;
        long  REC_TIME ;
} VIDEODVVAUX;

/* DV Speed information */
typedef struct _tagVIDEODVFF {
	long  Speed;	//speed =0  normal play or slow play; speed =1 : fast forward/backward
} VIDEODVFF;

/* DV Display Control information */
typedef struct _tagVIDEODVDISPLAYCTRL {
    unsigned int   Ctrl;
} VIDEODVDISPLAYCTRL;

#define DISPLAY_RECORD_DATE_TIME       (1)

/* MPEG4::DIVX4 information */
typedef struct _tagVIDEODIVX4 {
	long DivX4_Detected;
} VIDEODIVX4;

/* OggVorbis packet information */
typedef struct _tagAUDIOVORBISPKTINFO{
    unsigned int packetSize;
    unsigned int packetState;
} AUDIOVORBISPKTINFO;

typedef struct _tagAUDIOVORBIS{
	int                         vorbisPktType;
	unsigned int                backShift;
	AUDIO_INBAND_CMD_PKT_HEADER cmdHeader;
	AUDIOVORBISPKTINFO          vorbisPacketListInfo[255];
} AUDIOVORBIS;

/* RM video format */
typedef struct _tagVIDEORMFORMAT{

	unsigned int  MOFTag;					/* media object format (4CC) */
	unsigned int  subMOFTag;				/* codec id */
	unsigned int  width;					/* frame width */
	unsigned int  height;					/* frame heigth */
	unsigned int  bitCount;					/* number of bits per pixel */
	unsigned int  padWidth;					/* Padded width, in pixels, of a video frame */
	unsigned int  padHeight;				/* Padded height, in pixels, of a video frame */
	unsigned int  framesPerSecond;			/* frame rate */
	unsigned int  SPOExtra;					/* SPO extra flags */
	unsigned int  streamVersion;			/* release version of the bitstream */
	unsigned int  majorBitstreamVersion;	/* major version of the bitstream */
	unsigned int  minorBitstreamVersion;	/* minor version of the bitstream */
  	unsigned int  numResampledImageSizes;
	unsigned int  largestPels;
	unsigned int  largestLines;
	unsigned int  pDimensions[2*(8+1)];
}VIDEORMFORMAT;

/* RM video frame info */
typedef struct _tagVIDEORMFRAMEINFO
{
	unsigned int          sequenceNum;
  	unsigned int          numSegments;

}VIDEORMFRAMEINFO;

/* RM video segment info */
typedef struct _tagVIDEORMSEGMENTINFO
{
	unsigned int          isValid;
  	unsigned int          offset;

}VIDEORMSEGMENTINFO;

typedef struct _tagVIDEORMSEGMENT
{
	unsigned int		numSegments;
	VIDEORMSEGMENTINFO 	segment[128];

}VIDEORMSEGMENT;


typedef enum {
  CGMS_COPY_FREE,        // (0,0)
  CGMS_COPY_RESERVED,    // (0,1)
  CGMS_COPY_ONCE,        // (1,0)
  CGMS_COPY_NEVER        // (1,1)
} CGMS_INFO;


typedef struct _tagWMAPRODRC
{
	unsigned long peakReference;
	unsigned long peakTarget;
	unsigned long avgReference;
	unsigned long avgTarget;

}WMAPRODRC;


typedef struct _tagWMAPRODOWNMIXMATRIX
{
	unsigned long nameLen;
	unsigned long dataLen;

}WMAPRODOWNMIXMATRIX;

typedef struct _tagVIDEOVC1SEQINFO
{
	unsigned int profile;
	unsigned int displayWidth;
	unsigned int displayHeight;
	unsigned char extraData[128];

}VIDEOVC1SEQINFO;

typedef struct _tagVIDEOUSABILITYINFO
{
    unsigned int displayRatioHorizontal;
    unsigned int displayRatioVertical;
    unsigned int frameDurationInPTS;
} VIDEOUSABILITYINFO;

typedef struct _tagVIDEOMPEG4USABILITYINFO
{
	unsigned int pixel_width ;
	unsigned int pixel_height  ;
	unsigned int display_width ;
	unsigned int display_height ;
	unsigned int interlaced ;
	unsigned int frame_rate ;
} VIDEOMPEG4USABILITYINFO;

typedef struct _tagNAVIOBUFMAPINFO
{
	bool bDDRCopyCapability;	//offsetToPhysicalAddr is valid only when bDDRCopyCapability is true
	long offsetToPhysicalAddr; //(non)cached address + offsetToPhysicalAddr = physical address
} NAVIOBUFMAPINFO;

typedef struct _tagNAVDEMUXKEY
{
	unsigned char oddKey[16];
	unsigned char evenKey[16];
} NAVDEMUXKEY;

typedef struct _tagVIDEOFJPEGPICINFO
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	unsigned int picSize;
	unsigned int lastPicOfFrame;
	int64_t pts;

} VIDEOFJPEGPICINFO;

typedef struct
{
	long evenKeyOffset;
	long oddKeyOffset;
} NAVDEMUXCWOFFSET;

typedef struct _APESKIPBITSINFO
{
	int skipBytes;
	int skipBits;
}APESKIPBITSINFO;

typedef struct _tagVIDEOROTATION {
	int rotateAngle;
} VIDEOROTATION;

typedef struct
{
	unsigned int errNoticeTH;	// must less than errNoticeSpan
	unsigned int errNoticeSpan;	// 0~319
} VIDEODPBBYPASSPARAM;

typedef struct _tagVIDEOERRORLEVEL {
	long errConcealmentLevel;
	long errNotificationLevel;
} VIDEOERRORLEVEL;

#ifdef __cplusplus
}
#endif

#endif /*__PRIVATE_INFO_H__*/
