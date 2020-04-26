#include "DLNA_DMR.h"
#include "ILibThreadPool.h"
#include "CdsDidlSerializer.h"

#include "DMR_mapping_table.h"

#ifdef DLNADMRCTT
#include "DLNA_DMP.h"
char g_playcontaiercid[128];
int g_bplaycontainer;
#endif

#define MAX_VOLUME_NUMBER		100
#define MIN_VOLUME_NUMBER		0

#define MAX_BRIGHTNESS_VALUE	64
#define MIN_BRIGHTNESS_VALUE	0

#define MAX_CONTRAST_VALUE		64
#define MIN_CONTRAST_VALUE		0

#define DMR_DEFAULT_PORT		61000

static struct CdsObject *m_pNextURIMetadata = NULL;

//TODO: check the protocol info list
//TODO: there may be "DMR_SUBR_NOTHING" in playlist, this should be considered!
//      SeekTrack !!!
//      RendererType with extension should be modify !!!
//TODO: fix warning
//TODO: LPCM
//TODO: all DMR_StateChange_XXX input values modify by (Cds_Object*)data->Res->.....
//TODO: re design the architecture member variables!!!
//TODO: re design the state machine
//TODO: output flows about DMR!!!
//TODO: think about the display information
const char* ProtocolInfoList[] =
{
    /* Image Formats */
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    /*MCVT Image Format */
    "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:image/png:DLNA.ORG_PN=PNG_LRG",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
#ifdef INCLUDE_FEATURE_PLAYSINGLEURI
    "playsingle-http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
//        "playsingle-http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
#endif
    /* Audio Formats */
    "http-get:*:audio/L16;rate=44100;channels=1:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:audio/L16;rate=44100;channels=2:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:audio/L16;rate=48000;channels=1:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:audio/L16;rate=48000;channels=2:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
#ifdef INCLUDE_FEATURE_PLAYSINGLEURI
    "playsingle-http-get:*:audio/L16;channels=1;rate=44100:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:audio/L16;channels=2;rate=44100:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:audio/L16;channels=1;rate=48000:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:audio/L16;channels=2;rate=48000:DLNA.ORG_PN=LPCM",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
#endif
    /*MCVT Audio Formats */
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMABASE;DLNA.ORG_FLAGS=9D100000000000000000000000000000",
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMAFULL;DLNA.ORG_FLAGS=9D100000000000000000000000000000",
    "http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMAPRO;DLNA.ORG_FLAGS=9D100000000000000000000000000000",
    "http-get:*:audio/vnd.dlna.adts:DLNA.ORG_PN=AAC_ADTS_320",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO_320",//;DLNA.ORG_FLAGS=8D100000000000000000000000000000",
    "http-get:*:audio/mpeg:DLNA.ORG_PN=MP3;DLNA.ORG_FLAGS=9D100000000000000000000000000000",

    /* VIDEO: PS */
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_NTSC",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
#ifdef INCLUDE_FEATURE_PLAYSINGLEURI
    "playsingle-http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_NTSC",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
#endif
    /* VIDEO: TS */
//"http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_NA",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_NA_ISO",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_NA_T",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_KO",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_KO_T",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_KO_ISO",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",

    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU_T",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_EU_ISO",//;DLNA.ORG_FLAGS=3120000000000000000000000000000",

#ifdef INCLUDE_FEATURE_PLAYSINGLEURI
    "playsingle-http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_NA",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_NA_T",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
    "playsingle-http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA_T",//;DLNA.ORG_FLAGS=2120000000000000000000000000000",
#endif
#ifdef ENABLE_DTCP_IP
#ifndef DLNADMRCTT
    // LPTT cannot recognize these PNs
    // These are added for sharp BD-W2000
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_AC3_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_JP_MPEG1_L2_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_60_L2_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_AC3_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_MPEG1_L2_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_L2_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_60_LPCM_T",
#endif
#endif

    /*MCVT Video Format */
    "http-get:*:video/mp4:DLNA.ORG_PN=AVC_MP4_BL_CIF15_AAC_520",
#ifdef ENABLE_DTCP_IP
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_JP_AAC_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_JP_AAC_T;ARIB.OR.JP_PN=MPEG_TTS_CP",
#endif
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA_T",
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_NA_ISO",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_KO",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_KO_T",
    "http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_KO_ISO",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_JP_T",
#ifdef ENABLE_DTCP_IP
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_JP_T;ARIB.OR.JP_PN=MPEG_TTS_CP",
#endif
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHIGH_FULL",
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHIGH_PRO",
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_BASE",
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_FULL",
    "http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_PRO",
#ifdef ENABLE_DTCP_IP
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_AC3_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_24_AC3_T",
    "http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=AVC_TS_HD_60_LPCM_T",
#endif
#if !defined(DLNADMRCTT) // For UCTT: AV1-MR:1-AVT:1-12.1 Seek() action
    "http-get:*:audio/mpegurl:*",
#endif
    "http-get:*:audio/wav:*",
    "http-get:*:audio/l16:*", //upnplay wav
    "http-get:*:audio/mp3:*",
    "http-get:*:audio/mpeg:*",
    "http-get:*:audio/x-ms-wma:*",
    "http-get:*:audio/wma:*",
    "http-get:*:audio/vnd.dlna.adts:*",
    "http-get:*:audio/mp4:*",
    "http-get:*:audio/mpeg3:*",
    "http-get:*:audio/x-aiff:*",
    "http-get:*:audio/x-flac:*",
    "http-get:*:audio/x-ape:*",
    "http-get:*:audio/x-matroska:*",
    "http-get:*:application/ogg:*",
    "http-get:*:video/mpeg:*",
    "http-get:*:video/x-ms-wmv:*",
    "http-get:*:video/x-ms-asf:*",
    "http-get:*:video/x-ms-avi:*",
    "http-get:*:video/x-msvideo:*",
    "http-get:*:video/mpeg4:*",
    "http-get:*:video/avi:*",
    "http-get:*:video/h264:*",
    "http-get:*:video/mp4:*",
    "http-get:*:video/mp4v-es:*",  // for upnplay mp4
    "http-get:*:video/3gpp:*",
    "http-get:*:video/flv:*",
    "http-get:*:video/x-matroska:*",
    "http-get:*:video/quicktime:*",
    "http-get:*:application/x-ndsvgdrm:*",
    "http-get:*:application/vnd.rn-realmedia:*",
    "http-get:*:image/jpeg:*",
    "http-get:*:image/png:*",
    "http-get:*:image/tiff:*",
    "http-get:*:image/gif:*",

    "\0"
};

struct DMRConfig {
	const char *Manufacturer;
	const char *ManufacturerURL;
	const char *ModelDescription;
	const char *ModelName;
	const char *ModelNumber;
	const char *ModelURL;
	const char *ProductCode;
};
struct DMRConfig g_DMRConfig = {
	"Realtek Semiconductor Corp.",	// Manufacturer
	"http://www.realtek.com.tw",	// ManufacturerURL
	"Realtek DLNA DMR", 			// ModelDescription
	"Realtek DLNA DMR",				// ModelName
	"RTDXXXX",						// ModelNumber
	"http://www.realtek.com.tw",	// ModelURL
	"",								// ProductCode
};

#define FREE_MEMORY(memory) \
    if(memory != NULL) { \
        free(memory) ; \
        memory = NULL ; \
    }

#define DELETE_OBJECT(object) \
    if(object != NULL) { \
        delete object ; \
        object = NULL ; \
    }

//TODO, becareful racing codition, mutex_lock m_PlayState
#define RETURN_ERROR(error) \
    { \
        if( m_PlayState != DMR_PS_NoMedia ) \
            m_PlayState = DMR_PS_Stopped; \
        if( ShowStatus != NULL ) \
            ShowStatus(m_PlayState, NULL); \
        DMR_StateChange_TransportPlayState(instance, m_PlayState); \
        ULOGE("DMR RETURN_ERROR (%d) in %s:%d at %ld", error, __FILE__, __LINE__, time(NULL)); \
        return error; \
    }

#define RETURN_ERROR_STOP(error) \
    { \
        if( m_psubRenderer ) \
            m_psubRenderer->Stop(); \
        m_PlayState = DMR_PS_Stopped; \
        if( ShowStatus != NULL ) \
            ShowStatus(m_PlayState, NULL); \
        DMR_StateChange_TransportPlayState(instance, m_PlayState); \
        ULOGE("DMR RETURN_ERROR_STOP (%d) in %s:%d at %ld", error, __FILE__, __LINE__, time(NULL)); \
        return error; \
    }

#define DELETE_CDS_OBJECT(object)	\
    if (object) {					\
        CDS_ObjRef_Release(object);	\
        object = NULL;				\
    }

#define ASSIGN_CDS_OBJECT(left, right)	\
    if (right) {						\
        left = right;					\
        CDS_ObjRef_Add(left);			\
    }

#define BEGIN_THREAD_INIT()		if (m_pfnInitialize) {m_pfnInitialize(true);}
#define END_THREAD_INT()		if (m_pfnInitialize) {m_pfnInitialize(false);}

#ifndef HAS_FLAG
#define HAS_FLAG(flag, bit)    ((flag) & (bit))
#define SET_FLAG(flag, bit)    ((flag)|= (bit))
#define RESET_FLAG(flag, bit)  ((flag)&= (~(bit)))
#define CLEAR_FLAG(flag)       (flag = 0)
#endif

#define TIMEOUT_TO_FORCE_PLAY	10

#define S_OK	0
#define S_FALSE 1
#define E_FAIL	-1

//#define NORMALIZE_VOLUME(volume) ((int)MAX_VOLUME_NUMBER*volume/100)

char* BuildProtocolInfo(const char* infoList[])
{
    int counter;
    int length = 0;
    char* result = NULL;
    char* p;
#ifdef ENABLE_DTCP_IP
#define DTCP_MIMETYPE_PREFIX  "application/x-dtcp1;CONTENTFORMAT=\""
#define DTCP_MIMETYPE_POSTFIX "\""
#define DTCP_PROFILEID_PREFIX "DTCP_"
#define DTCP_PROFILEID_NAME "DLNA.ORG_PN="
#define DLNA_ORG_FLAGS      "DLNA.ORG_FLAGS="
#endif

    if(infoList == NULL)
    {
        return NULL;
    }

    counter = 0;
    p = (char*)infoList[counter];
    while(p[0] != '\0')
    {
        int pinfo_len = strlen(p)+1;
        length += pinfo_len;

#ifdef ENABLE_DTCP_IP
        if(strstr(p, DTCP_PROFILEID_NAME))
            length += pinfo_len + strlen(DTCP_PROFILEID_PREFIX) + strlen(DTCP_MIMETYPE_PREFIX) + strlen(DTCP_MIMETYPE_POSTFIX);
        else
            length += pinfo_len + strlen(DTCP_MIMETYPE_PREFIX) + strlen(DTCP_MIMETYPE_POSTFIX);
#endif

        p = (char*)infoList[++counter];
    }

    if(length == 0)
        return NULL;

    result = (char*)malloc(length);
    result[0] = 0;

    counter = 0;
    p = (char*)infoList[counter];

    char* curPos = result;
    while(1)
    {
        length = strlen(p);

        memcpy(curPos, p, length);
        curPos += length;

#ifdef ENABLE_DTCP_IP
        /* add DTCP protocolinfo here
         * e.g.
         *   add
         *     http-get:*:application/x-dtcp1;CONTENTFORMAT="video/vnd.dlna.mpeg-tts":DLNA.ORG_PN=DTCP_MPEG_TS_HD_KO_T
         *   for
         *     http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_KO_T
         *
         * Seg1=> http-get:*:
         * MimeType=> video/vnd.dlna.mpeg-tts
         * Seg2=> :DLNA.ORG_PN=
         * Seg3=> MPEG_TS_HD_KO_T
         */
        char* startPos;
        do
        {
            char *p_mimetype = strstr(p, ":*:");
            startPos = curPos;
            if(p_mimetype)
            {
                int SegLen1, SegLen2, SegLen3, MimeTypeLen;
                char *p_Seg2, *p_Seg3, *p_flags;

                p_mimetype += strlen(":*:");

                SegLen1 = p_mimetype-p;

                p_Seg2 = strstr(p_mimetype, ":");
                if(!p_Seg2)
                {
                    // should not enter here...
                    // protocolinfo format error??

                    // skip this protocolinfo
                    break;
                }
                MimeTypeLen = p_Seg2 - p_mimetype;

                p_Seg3 = strstr(p_Seg2, DTCP_PROFILEID_NAME);
                if(p_Seg3)
                {
                    p_Seg3 += strlen(DTCP_PROFILEID_NAME);
                    SegLen2 = p_Seg3 - p_Seg2;
                    SegLen3 = length - SegLen1 - MimeTypeLen - SegLen2;
                }
                else
                {
                    SegLen2 = length - SegLen1 - MimeTypeLen;
                    SegLen3 = 0;
                }


                // delimiter
                *curPos = ',';
                curPos++;

                // Seg1
                memcpy(curPos, p, SegLen1);
                curPos += SegLen1;

                // MimeType
                memcpy(curPos, DTCP_MIMETYPE_PREFIX, strlen(DTCP_MIMETYPE_PREFIX));
                curPos += strlen(DTCP_MIMETYPE_PREFIX);
                memcpy(curPos, p_mimetype, MimeTypeLen);
                curPos += MimeTypeLen;
                memcpy(curPos, DTCP_MIMETYPE_POSTFIX, strlen(DTCP_MIMETYPE_POSTFIX));
                curPos += strlen(DTCP_MIMETYPE_POSTFIX);;

                // Seg2
                memcpy(curPos, p_Seg2, SegLen2);
                curPos += SegLen2;

                // Seg3
                if(p_Seg3)
                {
                    memcpy(curPos, DTCP_PROFILEID_PREFIX, strlen(DTCP_PROFILEID_PREFIX));
                    curPos += strlen(DTCP_PROFILEID_PREFIX);
                    memcpy(curPos, p_Seg3, SegLen3);
                    curPos += SegLen3;
                }
                *curPos = 0;

                //set flags in  DLNA.ORG_FLAGS
                p_flags = strstr(startPos, DLNA_ORG_FLAGS);
                if(p_flags)
                {
                    char val;
                    // LP flag, bit 16
                    p_flags += strlen(DLNA_ORG_FLAGS) + 3; // bit 16 is in the 4th byte, mask = 0x1
                    val = *p_flags;
                    if((val >= '0' && val <= '9' && ((val-'0')&1)==0) ||
                            (val >= 'a' && val <= 'e' && ((val-'a'+10)&1)==0) ||
                            (val >= 'A' && val <= 'E' && ((val-'A'+10)&1)==0))
                    {
                        *p_flags = val+1;
                    }

                    // cleartextbyteseek-full flag, bit 15
                    p_flags++; // bit 15 is in the 5th byte, mask = 0x8
                    val = *p_flags;
                    if(val >= '0' && val <= '7')
                    {
                        val -= '0';
                        val += 8;
                        if(val <= 9)
                            *p_flags = val+'0';
                        else
                            *p_flags = val-10+'A';
                    }
                }
            }
        }
        while(0);

#endif

        p = (char*)infoList[++counter];
        if(p[0] == '\0')
            break;

        *curPos = ',';
        curPos++;
    }
    *curPos = 0;
    return result;
}

void* RTK_DLNA_DMR::m_pDMR_MicroStack_Chain;
subRenderer* RTK_DLNA_DMR::m_psubRenderer = NULL;
void* RTK_DLNA_DMR::m_pDMR_ILib_Pool;
char* RTK_DLNA_DMR::m_pDMR_mediaProtocolInfo;
char* RTK_DLNA_DMR::m_pDMR_ProtocolInfo;
char* RTK_DLNA_DMR::m_pDMR_mediaMimeTypeProtocol;
struct _DMR*      RTK_DLNA_DMR::m_pDMR_MicroStack;
char*             RTK_DLNA_DMR::m_pSetAVURI;
SUBRENDERER_TYPE  RTK_DLNA_DMR::m_SETAVType;
ENUM_MEDIA_TYPE   RTK_DLNA_DMR::m_mediaType;
int				  RTK_DLNA_DMR::m_dialogState;
unsigned int      RTK_DLNA_DMR::m_NumberOfTracks;
unsigned int      RTK_DLNA_DMR::m_CurrentTrack;
char**            RTK_DLNA_DMR::m_ppMediaTrackURI;
char**			  RTK_DLNA_DMR::m_ppProtocolInfo;

SUBRENDERER_TYPE* RTK_DLNA_DMR::m_pMediaType;
long              RTK_DLNA_DMR::m_totalTime;
struct CdsObject *RTK_DLNA_DMR::m_pMediaMetadata;

pthread_t         RTK_DLNA_DMR::m_DMRthread;
pthread_t         RTK_DLNA_DMR::m_DMRMonitorthread;
DMR_PlayState     RTK_DLNA_DMR::m_PlayState;
DMR_MediaPlayMode RTK_DLNA_DMR::m_PlayMode;
int               RTK_DLNA_DMR::m_Speed;
pthread_mutex_t	  RTK_DLNA_DMR::m_mutexSubRenderer;
bool              RTK_DLNA_DMR::m_bIsInternalStop;
bool              RTK_DLNA_DMR::m_bIsDMRBusy;
bool              RTK_DLNA_DMR::m_bIsDMRChainAlive;
bool			  RTK_DLNA_DMR::m_bIsDMRMonitorThreadRunning;

void*          RTK_DLNA_DMR::m_pDMR_Monitor;
int            RTK_DLNA_DMR::m_ipAddressLength;
int *          RTK_DLNA_DMR::m_pIpAddressList;

int				RTK_DLNA_DMR::nBitPerSample;
int				RTK_DLNA_DMR::nChannel;
int				RTK_DLNA_DMR::nSampleRate;


int (*(RTK_DLNA_DMR::QueryForConnection))     (int, void *); // (int eventID, void* eventData)
int (*(RTK_DLNA_DMR::PrepareForConnection))   (int, void *); // (int eventID, void* eventData)
int (*(RTK_DLNA_DMR::PrepareForDisconnection))(int, void *);
int (*(RTK_DLNA_DMR::SetBrightness))          (int , void*);
int (*(RTK_DLNA_DMR::SetContrast))            (int, void *);
int (*(RTK_DLNA_DMR::ShowStatus))             (int, void *);
int (*(RTK_DLNA_DMR::UpdateMediaInfo))        (int, void *);

int (*(RTK_DLNA_DMR::ShowVolumeStatus))        (int, void *);
int (*(RTK_DLNA_DMR::ShowDialog))             (int, void *);
int (*(RTK_DLNA_DMR::RestartDMR))             (int, void *);
int (*(RTK_DLNA_DMR::EventNotify))				(int, void *) = NULL;

DMR_Init_Fn RTK_DLNA_DMR::m_pfnInitialize = NULL;

#define STATUS_SET_AV_TRANSPORT_URI		(1L << 0)
#define STATUS_MESSAGE_DATA_EXIST		(1L << 1)
#define STATUS_DESTROY					(1L << 2)
#define STATUS_REQUEST_FOR_PLAY			(1L << 3)

long			statusFlag;

DMRMessageData		g_DMRMessageData;

static int fake_DMR_Start = 0;
RTK_DLNA_DMR::RTK_DLNA_DMR(char *friendlyName, char *deviceUUID, DMR_Init_Fn pInitFunc)
{
	ULOGD("Start as [%s] - (%s)", friendlyName, deviceUUID);

    int ignoreIPAddress = 0;
#if 0	// The P2P is not in Main yet
    if(SetupClass::GetInstance()->GetNetMode()==NET_MODE_P2P_CONCURRENT)
    {
        IPV4_2 ip;
        memset(&ip, 0, sizeof(ip));
        if (NetworkSet::GetInstance()->getIPAddr(NetworkSet::GetInstance()->getCurrentP2PIface(), IP_TYPE_IP, &ip) == 0)
        {
            char IPAddr[16];
            sprintf(IPAddr, "%d.%d.%d.%d", ip.ip[0], ip.ip[1], ip.ip[2], ip.ip[3]);

            ULOGD("DMR ignore IP list %s", IPAddr);
            int resultIP = inet_addr(IPAddr);
            if (resultIP != INADDR_NONE)
                ignoreIPAddress = resultIP;
        }
    }
#endif
    ILibSetLocalIgnoreIPAddress(ignoreIPAddress);
//	char *m_pDMR_ProtocolInfo = BuildProtocolInfo(ProtocolInfoList);
	DMR_GetConfiguration()->Manufacturer = g_DMRConfig.Manufacturer;
	DMR_GetConfiguration()->ManufacturerURL = g_DMRConfig.ManufacturerURL;
	DMR_GetConfiguration()->ModelDescription = g_DMRConfig.ModelDescription;
	DMR_GetConfiguration()->ModelName = g_DMRConfig.ModelName;
	DMR_GetConfiguration()->ModelNumber = g_DMRConfig.ModelNumber;
	DMR_GetConfiguration()->ModelURL = g_DMRConfig.ModelURL;
	DMR_GetConfiguration()->ProductCode = g_DMRConfig.ProductCode;

    m_pDMR_ProtocolInfo     = BuildProtocolInfo(ProtocolInfoList);
    m_pDMR_MicroStack_Chain = ILibCreateChain();
    m_pDMR_ILib_Pool        = ILibThreadPool_Create();
    m_pDMR_MicroStack       = DMR_Method_Create(m_pDMR_MicroStack_Chain, DMR_DEFAULT_PORT, friendlyName, "0000001", deviceUUID, m_pDMR_ProtocolInfo, m_pDMR_ILib_Pool);
    m_pDMR_mediaProtocolInfo     = NULL;
    m_pDMR_mediaMimeTypeProtocol = NULL;

    pthread_mutex_init(&m_mutexSubRenderer, 0);
    m_psubRenderer = NULL;
    m_SETAVType    = DMR_SUBR_NOTHING;
    m_mediaType    = MEDIATYPE_None;

    m_PlayState    = DMR_PS_NoMedia;
    m_PlayMode     = DMR_MPM_Normal;
    m_Speed        = 1;

    m_pSetAVURI       = NULL;
    m_NumberOfTracks  = 0;
    m_CurrentTrack    = 1;
    m_ppMediaTrackURI = NULL;
    m_ppProtocolInfo  = NULL;
    m_pMediaType      = NULL;
    m_totalTime       = 0;
    m_pMediaMetadata  = NULL;

// set the event functions
    m_pDMR_MicroStack->Event_SetAVTransportURI = &RTKMR_AVTransport_SetAVTransportURI;
    m_pDMR_MicroStack->Event_SetNextAVTransportURI = &RTKMR_AVTransport_SetNextAVTransportURI;

    m_pDMR_MicroStack->Event_GetAVProtocolInfo = &RTKMR_ConnectionManager_GetAVProtocolInfo;
    m_pDMR_MicroStack->Event_SetPlayMode       = &RTKMR_AVTransport_SetPlayMode;
    m_pDMR_MicroStack->Event_Stop              = &RTKMR_AVTransport_Stop;
    m_pDMR_MicroStack->Event_Play              = &RTKMR_AVTransport_Play;
    m_pDMR_MicroStack->Event_Pause             = &RTKMR_AVTransport_Pause;
    m_pDMR_MicroStack->Event_SeekTrack         = &RTKMR_AVTransport_SeekTrack;
    m_pDMR_MicroStack->Event_SeekTrackPosition = &RTKMR_AVTransport_SeekTrackPosition;
    m_pDMR_MicroStack->Event_SeekMediaPosition = &RTKMR_AVTransport_SeekMediaPosition;
    m_pDMR_MicroStack->Event_Next              = &RTKMR_AVTransport_Next;
    m_pDMR_MicroStack->Event_Previous          = &RTKMR_AVTransport_Previous;
    m_pDMR_MicroStack->Event_SelectPreset      = &RTKMR_RenderingControl_SelectPreset;
#if defined(INCLUDE_FEATURE_VOLUME)
    m_pDMR_MicroStack->Event_SetVolume         = &RTKMR_RenderingControl_SetVolume;
    m_pDMR_MicroStack->Event_SetMute           = &RTKMR_RenderingControl_SetMute;
#endif /* INCLUDE_FEATURE_VOLUME */
#if defined(INCLUDE_FEATURE_DISPLAY)
    m_pDMR_MicroStack->Event_SetContrast       = &RTKMR_RenderingControl_SetContrast;
    m_pDMR_MicroStack->Event_SetBrightness     = &RTKMR_RenderingControl_SetBrightness;
#endif /* INCLUDE_FEATURE_DISPLAY */
    QueryForConnection      = NULL;
    PrepareForConnection    = NULL;
    PrepareForDisconnection = NULL;
    SetBrightness           = NULL;
    SetContrast             = NULL;
    ShowStatus              = NULL;
    ShowDialog				= NULL;
    UpdateMediaInfo         = NULL;
    ShowVolumeStatus        = NULL;
    RestartDMR              = NULL;
    EventNotify				= NULL;
    m_pfnInitialize         = pInitFunc;
    m_bIsInternalStop       = false;
    m_bIsDMRBusy            = false;
    m_bIsDMRChainAlive      = false;
    m_bIsDMRMonitorThreadRunning = false;

    m_ipAddressLength = 0;
    m_pIpAddressList  = NULL;
    fake_DMR_Start    = 0;
    m_ipAddressLength = ILibGetLocalIPAddressList(&m_pIpAddressList);
    ULOGD("m_ipaddesslist leng=%d",m_ipAddressLength);
    for(int i=0; i<m_ipAddressLength; i++)
    {
        struct in_addr DMR__inaddr;
        DMR__inaddr.s_addr = m_pIpAddressList[i];
        ULOGD("m_ipaddr[%d]=%s",i, inet_ntoa(DMR__inaddr));
    }
    //m_pDMR_Monitor    = (void *)ILibCreateLifeTime(m_pDMR_MicroStack_Chain);

    //ILibLifeTime_Add(m_pDMR_Monitor, NULL, 4, &DMR_Network_Monitor, NULL);

    if(m_ipAddressLength == 0)
    {
        fake_DMR_Start = 1;
        ULOGD("DMR Fake Start Only create Monitor Thread");
    }

    pthread_create(&m_DMRthread, NULL, &Run, NULL);
    while(!m_bIsDMRChainAlive)
        usleep(100000);

    pthread_create(&m_DMRMonitorthread, NULL, &DMR_Network_Monitor, NULL);
    while(!m_bIsDMRMonitorThreadRunning)
        usleep(100000);

	ULOGD("DMR Create done.");

    CLEAR_FLAG(statusFlag);
}

RTK_DLNA_DMR::~RTK_DLNA_DMR()
{
	SET_FLAG(statusFlag, STATUS_DESTROY);
    DlnaDmrInternalStop();

    m_bIsDMRMonitorThreadRunning = false;
    pthread_join(m_DMRMonitorthread, 0);

    Terminate();

    FREE_MEMORY(m_pDMR_mediaProtocolInfo);
    FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);

    //TODO, check ....
    if( m_pDMR_ILib_Pool )
    {
        while(m_bIsDMRChainAlive)
        {
            usleep(100);
        }

        ILibThreadPool_Destroy( m_pDMR_ILib_Pool );
    }

    m_pDMR_Monitor = NULL;

    pthread_join(m_DMRthread, 0);
    //pthread_join(m_DMRMonitorthread, 0);

    if( m_pSetAVURI != NULL )
    {
        free(m_pSetAVURI);
        m_pSetAVURI = NULL;
    }

    if( m_ppMediaTrackURI != NULL )  	//m_ppMediaTrackURI[m_NumberOfTracks];
    {
        unsigned int i = 0;
        for( i = 0; i < m_NumberOfTracks; i++)
        {
            if( m_ppMediaTrackURI[i] != NULL )
            {
                free(m_ppMediaTrackURI[i]);
                m_ppMediaTrackURI[i] = NULL;
            }
        }
        delete [] m_ppMediaTrackURI;
        m_ppMediaTrackURI = NULL;
    }

    if( m_ppProtocolInfo != NULL )
    {
        unsigned int i = 0;
        for( i = 0; i < m_NumberOfTracks; i++)
        {
            if( m_ppProtocolInfo[i] != NULL )
            {
                free(m_ppProtocolInfo[i]);
                m_ppProtocolInfo[i] = NULL;
            }
        }
        delete [] m_ppProtocolInfo;
        m_ppProtocolInfo = NULL;
    }



    if( m_pMediaType != NULL )  	//m_pMediaType[m_NumberOfTracks];
    {
        delete [] m_pMediaType;
        m_pMediaType = NULL;
    }

    if(m_ipAddressLength != 0)
    {
        free(m_pIpAddressList);
        m_pIpAddressList = NULL;
    }

    if(m_pDMR_ProtocolInfo != 0)
    {
        free(m_pDMR_ProtocolInfo);
        m_pDMR_ProtocolInfo = NULL;
    }

    DELETE_CDS_OBJECT(m_pMediaMetadata);
    DELETE_CDS_OBJECT(m_pNextURIMetadata);

    pthread_mutex_destroy(&m_mutexSubRenderer);
}

void *RTK_DLNA_DMR::Run(void* arg)
{
	ULOGD("Thread:DMR::Run Start");
    m_bIsDMRChainAlive = true;
    if(fake_DMR_Start == 1)
    {
        ULOGD("Thread:DMR::Run End - DMR Fake Start");
        return NULL;
    }

    BEGIN_THREAD_INIT();

    ILibStartChain(m_pDMR_MicroStack_Chain);
    m_bIsDMRChainAlive = false;

    END_THREAD_INT();

    ULOGD("Thread:DMR::Run END");
    return NULL;
}

void* RTK_DLNA_DMR::DMR_Network_Monitor(void *)
{
	ULOGD("Thread:DMR::DMR_Network_Monitor Start");
    int length;
    int *list;
    int count=0;

    m_bIsDMRMonitorThreadRunning = true;

    BEGIN_THREAD_INIT();

    while(m_bIsDMRMonitorThreadRunning)
    {
        if(count <= 4)
        {
            count++;
            sleep(1);
        }
        else
        {
            count = 0;
            length = ILibGetLocalIPAddressList(&list);

            if( length != m_ipAddressLength || memcmp((void*)list, (void*)m_pIpAddressList, sizeof(int)*length)!=0)
            {
                struct in_addr DMR__inaddr;
                ULOGD("m_ipaddessleng=%d newleng=%d",m_ipAddressLength,length);
                for(int i=0; i<m_ipAddressLength; i++)
                {
                    DMR__inaddr.s_addr = m_pIpAddressList[i];
                    ULOGD("m_ipaddr[%d]=%s",i,inet_ntoa(DMR__inaddr));
                }

                for(int j=0; j<length; j++)
                {
                    DMR__inaddr.s_addr = list[j];
                    ULOGD("newaddr[%d]=%s",j,inet_ntoa(DMR__inaddr));
                }

                int bNeedRestart = 1;
                for(int i=0; i<m_ipAddressLength; i++)
                    for(int j=0; j<length; j++)
                    {
                        if(m_pIpAddressList[i] == list[j])
                        {
                            DMR__inaddr.s_addr = m_pIpAddressList[i];
                            if(m_pSetAVURI != NULL)
                            {
                                char ipaddr[64];
                                char *p,*tmp,*tmp1,*tmp2,*tmp3;
                                if(p = strstr(m_pSetAVURI,"http://"))
                                {
                                    tmp = p + 7;
                                    if(tmp1 = strchr(tmp,'.'))
                                    {
                                        if(tmp2 = strchr(tmp1+1,'.'))
                                        {
                                            if(tmp3 = strchr(tmp2+1,'.'))
                                            {
                                                memset(ipaddr,0,64);
                                                strncpy(ipaddr,tmp,tmp3-tmp);
                                                if(strstr(inet_ntoa(DMR__inaddr),ipaddr))
                                                {
                                                    ULOGD("%s contains  URI addr=%s",inet_ntoa(DMR__inaddr),ipaddr);
                                                    bNeedRestart = 0;
                                                }

                                            }
                                        }
                                    }
                                }

                            }
                        }
                    }

                if(m_ipAddressLength != 0)
                {
                    free(m_pIpAddressList);
                    m_pIpAddressList = NULL;
                }
                m_ipAddressLength = ILibGetLocalIPAddressList(&m_pIpAddressList);
                //m_ipAddressLength = length;
                //m_pIpAddressList = list;
                DMR_Method_NotifyMicrostackOfIPAddressChange(m_pDMR_MicroStack);
                ULOGD("%s, %s, %d System IPs changes!!!", __FILE__, __func__, __LINE__);

                if(bNeedRestart)
                {
                    ULOGD("DMR Need to Restart");
                    if(RestartDMR != NULL)RestartDMR(0,NULL);
                }
            }
            free(list);
        }
    }

    END_THREAD_INT();

    ULOGD("Thread:DMR::DMR_Network_Monitor END");
    return NULL;
}

void* RTK_DLNA_DMR::PlayTrackThread(void *arg)
{
    BEGIN_THREAD_INIT();

    SetDMRBlock();
    int nResult = UpdateSubRender(m_pMediaType[m_CurrentTrack-1]);
    if (nResult != 0)
    {
		SetDMRUnBlock();
    	return 0;;	// this should not happen !!!
    }
    SetDMRUnBlock();

    m_PlayState = DMR_PS_Transitioning;

    if(m_ppProtocolInfo)
        m_pMediaType[m_CurrentTrack-1] = RendererType(m_ppProtocolInfo[m_CurrentTrack-1],1);

    if(m_pMediaType[m_CurrentTrack-1] == DMR_SUBR_NOTHING)
    {
        char *extension = strrchr(m_ppMediaTrackURI[m_CurrentTrack-1], '.');
        if( extension != NULL )
            m_pMediaType[m_CurrentTrack-1] = RendererType(extension);
    }

    RTKMR_AVTransport_Play(m_pDMR_MicroStack, NULL, "1");

    END_THREAD_INT();

    return 0;
}

void RTK_DLNA_DMR::Start()
{
    ;
}

void RTK_DLNA_DMR::Terminate()
{
    if(fake_DMR_Start)
    {
        ULOGD("#########Fake DMR Stop");
        m_bIsDMRChainAlive = false;
        return;
    }
    ULOGD("#############Stoping DMR Chain");
    ILibStopChain(m_pDMR_MicroStack_Chain);
}


int RTK_DLNA_DMR::RTKMR_AVTransport_SetAVTransportURI(DMR instance, void *session, char *uriSource, struct CdsObject *data)
{
    /**
    		!! lock the device !!
    		!! check uri simply, loadMedia already yet...!!
    	0. check the dmr instance
    	1. check the uri
    	2. reset the related variables
    	3. Get the AVTURI type and AVProtocolInfo
    	4. set NumberOfTracsks, m_CurrentTrack, m_ppMediaTrackURI[][], m_pMediatype[],
     	   according to m_SETAVType
    	5. store URI, and update related variables to DMR.c

    		!! TODO racing concerned!! / Sync ??
    		!!                                !!
    		!! TODO check session is the same ??
    */
    ULOGD("[DMR event] AVTransport SetAVTransportURI [%s] at %ld", uriSource, time(NULL));
    unsigned int i = 0;
    int duration = 0;

    char uri[1024];
    sprintf(uri,"%s", uriSource);

    int nSize = strlen(uri);
    if (ILibInPlaceXmlUnEscape(uri) != nSize)
    {
        ULOGD("[DMR avRenderer] loadMedia changes to (%s)", uri);
    }

#ifdef PPTV
    if( (!strncmp(uri,"pptv://",strlen("pptv://"))) && (DMR_StateGet_PlayState(instance) != DMR_PS_Stopped) )
    {
        RTKMR_AVTransport_Stop(instance,session);
    }
#endif

    SetDMRBlock();

	if (HAS_FLAG(statusFlag, STATUS_DESTROY))
	{
		ULOGE("[DMR event] AVTransport SetAVTransportURI : DMR is destroy");
        SetDMRUnBlock();
        RETURN_ERROR(701);
	}

    statusFlag = 0;
    SET_FLAG(statusFlag, STATUS_SET_AV_TRANSPORT_URI);

#ifdef DMR_ENABLE_FORCE_PLAYBACK
    if (m_PlayState != DMR_PS_Stopped && m_PlayState != DMR_PS_NoMedia) {
        ULOGD("[DMR event] SetAVTransportURI is called not in stop mode.  PlayState(%d)", m_PlayState);
        SET_FLAG(statusFlag, STATUS_REQUEST_FOR_PLAY);
    }
#endif

    if( checkDMRIsInternalCommand() )
    {
        CLEAR_FLAG(statusFlag);
        SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_pDMR_MicroStack != instance )
    {
        ULOGE("[DMR event] AVTransport SetAVTransportURI : Invalid instanceID");
        CLEAR_FLAG(statusFlag);
        SetDMRUnBlock();
        RETURN_ERROR(718);// Invalid InstanceID //return DMR_ERROR_BADTHIS;
    }

    if( /*uri == NULL ||*/ strlen(uri) == 0 || strcmp(uri, "")==0)
    {
        ULOGD("[DMR event] AVTransport SetAVTransportURI\n\tNULL uri");
        //TODO, this should be re-check
        m_PlayState = DMR_PS_NoMedia;	// <--- this is strange, UPnP define only initialization state set to DMR_PS_NoMedia state...., but DLNA CTT DMR, MM part expected set to DMR_PS_NoMedia
        m_SETAVType = DMR_SUBR_NOTHING;
        m_mediaType = MEDIATYPE_None;
        FREE_MEMORY(m_pDMR_mediaProtocolInfo);
        FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);
        FREE_MEMORY(m_pSetAVURI);
        DELETE_OBJECT(m_psubRenderer);
        DELETE_OBJECT(m_pMediaType);
        DELETE_CDS_OBJECT(m_pMediaMetadata);
        if( m_ppMediaTrackURI != NULL )
        {
            for( i = 0; i < m_NumberOfTracks; i++)
            {
                if( m_ppMediaTrackURI[i] != NULL )
                {
                    free(m_ppMediaTrackURI[i]);
                    m_ppMediaTrackURI[i] = NULL;
                }
            }
            delete [] m_ppMediaTrackURI;
            m_ppMediaTrackURI = NULL;
        }

        if( m_ppProtocolInfo != NULL )
        {
            unsigned int i = 0;
            for( i = 0; i < m_NumberOfTracks; i++)
            {
                if( m_ppProtocolInfo[i] != NULL )
                {
                    free(m_ppProtocolInfo[i]);
                    m_ppProtocolInfo[i] = NULL;
                }
            }
            delete [] m_ppProtocolInfo;
            m_ppProtocolInfo = NULL;
        }

        DMR_StateChange_AVTransportURI(instance, uri);
        DMR_StateChange_AVTransportURIMetaData(instance, data);
        DMR_StateChange_TransportPlayState(instance, m_PlayState);
        DMR_StateChange_NumberOfTracks(instance, 0);
        DMR_StateChange_CurrentTrack(instance, 0);
        DMR_StateChange_CurrentTrackURI(instance, NULL);
        DMR_StateChange_CurrentMediaDuration(instance, duration);
        DMR_StateChange_CurrentTrackDuration(instance, duration);
        DMR_StateChange_CurrentTransportActions(instance, DMR_ATS_Reset);

        CLEAR_FLAG(statusFlag);
        SetDMRUnBlock();

        if(UpdateMediaInfo != NULL)
            UpdateMediaInfo(0, NULL);

        return DMR_ERROR_OK;
    }

    //reset something
    m_SETAVType = DMR_SUBR_NOTHING;
    m_mediaType = MEDIATYPE_None;
    FREE_MEMORY(m_pDMR_mediaProtocolInfo);
    FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);
    FREE_MEMORY(m_pSetAVURI);
    DELETE_OBJECT(m_psubRenderer);
    DELETE_OBJECT(m_pMediaType);
    DELETE_CDS_OBJECT(m_pMediaMetadata);

    DMR_StateChange_Reset(instance);

    if( m_ppMediaTrackURI != NULL )
    {
        for( i = 0; i < m_NumberOfTracks; i++)
        {
            if( m_ppMediaTrackURI[i] != NULL )
            {
                free(m_ppMediaTrackURI[i]);
                m_ppMediaTrackURI[i] = NULL;
            }
        }
        delete [] m_ppMediaTrackURI;
        m_ppMediaTrackURI = NULL;
    }
    m_CurrentTrack = 1;
    if( m_ppProtocolInfo != NULL )
    {
        unsigned int i = 0;
        for( i = 0; i < m_NumberOfTracks; i++)
        {
            if( m_ppProtocolInfo[i] != NULL )
            {
                free(m_ppProtocolInfo[i]);
                m_ppProtocolInfo[i] = NULL;
            }
        }
        delete [] m_ppProtocolInfo;
        m_ppProtocolInfo = NULL;
    }

    nChannel = 1;
    nSampleRate = 44100;
    nBitPerSample = 16;

    // a. Get the renderer type
    // b. Get the AVProtocolInfo
    if( data != NULL)  	// just fetch the info
    {
        m_SETAVType = RendererType(data);
        if(data->Res)
        {
            nChannel = data->Res->NrAudioChannels;
            nSampleRate = data->Res->SampleFrequency;
            nBitPerSample = data->Res->BitsPerSample;
        }
    }

#ifdef DLNADMRCTT
    g_bplaycontainer = 0;
#endif
    if( m_SETAVType == DMR_SUBR_NOTHING )  	// while (data==NULL), parsing the file extension
    {
        ULOGD("[DMR event] the metadata input is null");
#ifdef DLNADMRCTT
        if(!strncmp(uri,"dlna-playcontainer://",strlen("dlna-playcontainer://")))
        {
            ULOGD("dlna-playcontainer uri");
            g_bplaycontainer = 1;
            m_SETAVType = DMR_SUBR_PLAYLIST;
        }
        else
#endif

        {
            char *extension = strrchr(uri, '.');

            if( extension == NULL )
            {
                CLEAR_FLAG(statusFlag);
                SetDMRUnBlock();
                //RETURN_ERROR(DMR_ERROR_BADURI)
                RETURN_ERROR(714);
            }

            m_SETAVType = RendererType(extension);
#ifndef DLNADMRCTT
            if (m_SETAVType == DMR_SUBR_NOTHING) {
                ULOGD("[DMR event] Force to set the default type to video.");
                m_SETAVType = DMR_SUBR_VIDEO;
                FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);
                m_pDMR_mediaMimeTypeProtocol = strdup("video/*");
            }
#endif
        }
    }
    else if(m_mediaType == MEDIATYPE_None)
    {
        char *extension = strrchr(uri, '.');
        if((extension != NULL) && !strcmp(extension,".pcm"))
            m_mediaType = MEDIASUBTYPE_PCM;

    }

#ifdef PPTV
    if(!strncmp(uri,"pptv://",strlen("pptv://")))
    {
        m_SETAVType = DMR_SUBR_VIDEO;
        m_mediaType = MEDIATYPE_PPTV;
    }
#endif

	if(ExecEventNotify(DLNA_DMR_NOTIFY_ID_QUERYFORCONNECTION)!=0)
    {
		if( QueryForConnection == NULL || QueryForConnection(0, NULL)!= S_OK)
	    {
	        CLEAR_FLAG(statusFlag);
	        SetDMRUnBlock();
	        RETURN_ERROR(602);
	    }
	}

    // new renderer by renderer type
    // and related settings: NumberOfTracsks, m_CurrentTrack, m_ppMediaTrackURI[][], m_pMediatype[]
    // loadMedia for setting
    int nResult = UpdateSubRender(m_SETAVType);
    if (nResult != 0)
    {
        CLEAR_FLAG(statusFlag);
        SetDMRUnBlock();

        if (nResult == -1)
        {
            RETURN_ERROR(714);//"Illegal MIME-Type"
        }
        else
        {
        	RETURN_ERROR(nResult);
        }
    }

#ifdef ENABLE_DTCP_IP
    if(data && data->Res && data->Res->ProtocolInfo && m_psubRenderer)
    {
        bool bDTCP = false;
        if(strstr(data->Res->ProtocolInfo, "DTCP1HOST") != NULL)
            bDTCP = true;
        else
        {
            char *p = strstr(uri, "?");
            if(p && strstr(p, "CONTENTPROTECTIONTYPE=DTCP1") != NULL)
                bDTCP = true;
        }

        if(bDTCP)
        {
            char *pinfo_tmp, *pEnd, *pinfo;
            int len, sub=0;

            pinfo = data->Res->ProtocolInfo;
            len = strlen(pinfo);

            pinfo_tmp = strdup(pinfo);

            if(pinfo_tmp)
            {
                pEnd = pinfo_tmp+len;

                while(pEnd > pinfo_tmp && *pEnd != ':')
                {
                    pEnd--;
                    sub++;
                }
                if(sub>=2)
                {
                    *(pEnd+1) = '*';
                    *(pEnd+2) = 0;
                    len -= sub-2;
                    pinfo = pinfo_tmp;
                    ULOGD("protocolinfo = %s", pinfo);
                }
            }
            m_psubRenderer->SetProtocolInfo(pinfo);

            if(pinfo_tmp)
                free(pinfo_tmp);
        }
    }
#endif /* ENABLE_DTCP_IP */

    int hr = m_psubRenderer->preParse(uri, &m_NumberOfTracks, &m_ppMediaTrackURI, &m_pMediaType, &m_totalTime,&m_ppProtocolInfo);
    if(hr == S_OK)
    {
        ULOGD("preparse succeed");

        //unsigned int i = 0;
        m_CurrentTrack = 1;
        m_pSetAVURI = strdup(uri);

        // for Playlist....
        // fill up MediaType
        if(m_SETAVType == DMR_SUBR_PLAYLIST)
        {
            //m_pSetAVURI = strdup(m_ppMediaTrackURI[m_CurrentTrack-1]);
#if 1
            for( i = 0; i < m_NumberOfTracks; i++)
            {
#ifdef DLNADMRCTT
                if(!strncmp(m_ppMediaTrackURI[i],"dlna-playsingle://",strlen("dlna-playsingle://")))
                {
                    char iid[128];
                    char uuid[64];
                    char*	cur = NULL;
                    char*	next= NULL;
                    unsigned long ret;
                    ULOGD("Starting playsingle URL convert");

                    if( (cur = strstr( m_ppMediaTrackURI[i], "uuid:"))!= NULL )
                    {
                        memset(uuid,0,64);
                        cur += 5;
                        strncpy(uuid,cur,36);
                    }
                    else if( (cur = strstr( m_ppMediaTrackURI[i], "uuid%3A"))!= NULL )
                    {
                        memset(uuid,0,64);
                        cur += 7;
                        strncpy(uuid,cur,36);
                    }
                    else if( (cur = strstr( m_ppMediaTrackURI[i], "uuid%3a"))!= NULL )
                    {
                        memset(uuid,0,64);
                        cur += 7;
                        strncpy(uuid,cur,36);
                    }

                    cur = NULL;
                    next = NULL;
                    if( (cur = strstr(m_ppMediaTrackURI[i], "iid="))!= NULL )
                    {
                        memset(iid,0,128);
                        cur += 4;
                        if( (next = strchr( cur, '&'))!= NULL )
                        {
                            strncpy(iid,cur,next-cur);
                        }
                        else
                            strcpy(iid,cur);
                    }
                    else
                    {
                        CLEAR_FLAG(statusFlag);
                        SetDMRUnBlock();
                        RETURN_ERROR(704);
                    }

                    ULOGD("xxxxxxxxxxxxxxxxx uuid=%s iid=%s",uuid,iid);
                    RTK_DLNA_DMP *m_pUpnpDmp = RTK_DLNA_DMP_Singleton::GetInstance();
                    if(m_pUpnpDmp)m_pUpnpDmp->unsetMediaServer();
                    if(m_pUpnpDmp)m_pUpnpDmp->Start();
                    sleep(5);

                    if(m_pUpnpDmp && !m_pUpnpDmp->setMediaServerByUDN(uuid))
                    {
                        ULOGE("DMP setMediaServerByUDN Error");
                        CLEAR_FLAG(statusFlag);
                        SetDMRUnBlock();
                        if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
                        RETURN_ERROR(704);
                    }

                    if(!m_pUpnpDmp->UPnPServiceBrowse("0"))
                    {
                        ULOGE("DMP UPnPServiceBrowse iid Error");
                        CLEAR_FLAG(statusFlag);
                        SetDMRUnBlock();
                        if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
                        RETURN_ERROR(704);
                    }

                    //get all item
                    int totalcount=m_pUpnpDmp->MediaContainerObjectSizes();
                    ULOGD("DMP container totalcount=%d",totalcount);

                    struct UPnPObjInfo *pInfo = (struct UPnPObjInfo *)malloc(sizeof(struct UPnPObjInfo));
                    if(!pInfo)
                    {
                        CLEAR_FLAG(statusFlag);
                        SetDMRUnBlock();
                        if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
                        RETURN_ERROR(704);
                    }
                    for(int i=0; i<totalcount; i++)
                    {
                        memset(pInfo, 0, sizeof(struct UPnPObjInfo));
                        m_pUpnpDmp->queryContainerObjInfoByIndex(i, pInfo);
                        if(strcmp(pInfo->pUniqueCharID,g_playcontaiercid))
                            break;
                    }

                    ULOGD("DMP get container id=%s",pInfo->pUniqueCharID);

                    if(!m_pUpnpDmp->UPnPServiceBrowse(pInfo->pUniqueCharID))
                    {
                        ULOGE("DMP UPnPServiceBrowse id Error");
                        CLEAR_FLAG(statusFlag);
                        SetDMRUnBlock();
                        if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
                        RETURN_ERROR(704);
                    }

                    free(pInfo);

                    if(m_pUpnpDmp->queryResourceByID(iid,  UPNP_DMP_RES_URI, &ret))
                    {
                        free(m_ppMediaTrackURI[i]);
                        m_ppMediaTrackURI[i] = strdup((char*)ret);

                    }
                    else
                    {
                        CLEAR_FLAG(statusFlag);
                        SetDMRUnBlock();
                        if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
                        RETURN_ERROR(704);
                    }
                    if(m_pUpnpDmp)m_pUpnpDmp->Terminate();

                }
#endif

                if(m_ppProtocolInfo)
                    m_pMediaType[i] = RendererType(m_ppProtocolInfo[i],1);

                if((m_pMediaType[i] == DMR_SUBR_NOTHING) ||(m_pMediaType[i] == DMR_SUBR_UNKNOWN))
                {
                    char *extension = strrchr(m_ppMediaTrackURI[i], '.');
                    if( extension != NULL )
                        m_pMediaType[i] = RendererType(extension);
                }
            }

            if (m_NumberOfTracks > 0)   // Set the current play item to fist one
            {
                if(m_ppProtocolInfo)
                    m_pMediaType[0] = RendererType(m_ppProtocolInfo[0], 1);
                if((m_pMediaType[0] == DMR_SUBR_NOTHING) ||(m_pMediaType[0] == DMR_SUBR_UNKNOWN))
                {
                    char *extension = strrchr(m_ppMediaTrackURI[i], '.');
                    if( extension != NULL )
                        m_pMediaType[0] = RendererType(extension);
                }
            }
#endif
        }



        //TODO, search a appropriate track index
        if( m_SETAVType != m_pMediaType[m_CurrentTrack-1] )
        {
		    int nResult = UpdateSubRender(m_pMediaType[m_CurrentTrack-1]);
		    if (nResult != 0)
            {
                ULOGE("playlist DMR_SUBR_Nothing");
                CLEAR_FLAG(statusFlag);
                SetDMRUnBlock();
                RETURN_ERROR(704);
            }
        }

#ifdef DLNADMRCTT
        if((m_pMediaType[m_CurrentTrack-1] == DMR_SUBR_AUDIO)||
           (m_pMediaType[m_CurrentTrack-1] == DMR_SUBR_VIDEO))
           DMR_Set_PlayingAV(1);
        else
           DMR_Set_PlayingAV(0);
#endif

    }
    else
    {
        ULOGE("preparse failed");
        DELETE_OBJECT(m_psubRenderer);
        CLEAR_FLAG(statusFlag);
        SetDMRUnBlock();
        RETURN_ERROR(704);
    }

    if(ExecEventNotify(DLNA_DMR_NOTIFY_ID_SETAVTRANSPORTURI)!=0)
    {
		if( PrepareForConnection == NULL || PrepareForConnection(0, NULL)!= S_OK)
	    {
	        CLEAR_FLAG(statusFlag);
	        SetDMRUnBlock();
	        if(UpdateMediaInfo != NULL)
	            UpdateMediaInfo(0, NULL);

	        RETURN_ERROR(602);
	    }
	}

    m_PlayState = DMR_PS_Stopped;

    DMR_StateChange_AVTransportURI(instance, uri);
    DMR_StateChange_AVTransportURIMetaData(instance, data);
    DMR_StateChange_CurrentTrackMetaData(instance, m_pMediaMetadata);
    DMR_StateChange_TransportPlayState(instance, m_PlayState);

    DMR_StateChange_NumberOfTracks(instance, m_NumberOfTracks);
    DMR_StateChange_CurrentTrack(instance, m_CurrentTrack);//for DCTT 7.3.100.3
    DMR_StateChange_CurrentTrackURI(instance, m_ppMediaTrackURI[m_CurrentTrack-1]);
    if( data != NULL && data->Res != NULL && data->Res->Duration >= 0 )
        duration = data->Res->Duration*1000;
    else
        ULOGD("[DMR event] duration does not exist !!!");


    ULOGD("m_PlayState=%d m_CurrentTrack=%d NumberofTrack=%d trackURI=%s duration=%d totaltime=%ld",m_PlayState,m_CurrentTrack,m_NumberOfTracks,m_ppMediaTrackURI[m_CurrentTrack-1],duration,m_totalTime);

    if(m_totalTime > 0 )
        duration = m_totalTime;

    DMR_StateChange_CurrentMediaDuration(instance, duration);
    DMR_StateChange_CurrentTrackDuration(instance, duration);

    unsigned short allowedActions = DMR_ATS_Play|DMR_ATS_Stop|DMR_ATS_Pause|DMR_ATS_Next|DMR_ATS_Previous|DMR_ATS_Seek;

    if (m_pDMR_mediaProtocolInfo)
    {
        char * cur = NULL;
        if( (cur = strstr(m_pDMR_mediaProtocolInfo, "DLNA.ORG_OP"))!= NULL )
        {
            cur += 12; // 12 = strlen("DLNA.ORG_OP=")
            if(cur[1] == '0')
            {
                allowedActions &= ~DMR_ATS_Seek;
            }
        }
    }

    if (m_SETAVType != DMR_SUBR_AUDIO &&
             m_SETAVType != DMR_SUBR_VIDEO) {
         allowedActions &= ~DMR_ATS_Seek;
    }


    DMR_StateChange_CurrentTransportActions(instance, allowedActions);

    if(UpdateMediaInfo != NULL) UpdateMediaInfo(0, NULL);

    RESET_FLAG(statusFlag, STATUS_SET_AV_TRANSPORT_URI);
    SetDMRUnBlock();

    if (HAS_FLAG(statusFlag, STATUS_REQUEST_FOR_PLAY))
    {
    	if (!m_pDMR_Monitor)
    	{
    		m_pDMR_Monitor = (void *)ILibCreateLifeTime(m_pDMR_MicroStack_Chain);
    	}

    	ULOGD("[DMR event] Set force to play in %d seconds !!!", TIMEOUT_TO_FORCE_PLAY);

    	ILibLifeTime_Remove(m_pDMR_Monitor, &statusFlag);
    	ILibLifeTime_Add(m_pDMR_Monitor, &statusFlag, TIMEOUT_TO_FORCE_PLAY, &DMR_Monitor, NULL);
    }

    //DMR_StateChange_ResetLastChangeMask(instance, FALSE);

    ULOGD("[DMR event] AVTransport SetAVTransportURI DONE at %ld", time(NULL));

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_SetNextAVTransportURI(DMR instance, void *session, char *uri, struct CdsObject *data)
{
    ULOGD("[DMR event] AVTransport SetNextAVTransportURI");

    if( m_pDMR_MicroStack != instance )
    {
        ULOGE("[DMR event] AVTransport SetNextAVTransportURI\n\tInvalid instanceID");
        return 718;
    }

    if( uri != NULL && strcmp(uri, "")!=0)
    {
        ULOGD("uri=%s", uri);
        char *extension = strrchr(uri, '.');
        if( extension == NULL )
        {
            return 714;
        }
        SUBRENDERER_TYPE type = RendererType(extension);
        if(type == DMR_SUBR_NOTHING)
            return 714;

        DMR_StateChange_NextAVTransportURI(instance, uri);
        DMR_StateChange_NextAVTransportURIMetaData(instance, data);

        DELETE_CDS_OBJECT(m_pNextURIMetadata);
        ASSIGN_CDS_OBJECT(m_pNextURIMetadata, data);
    }
    return DMR_ERROR_OK;
}



int RTK_DLNA_DMR::RTKMR_ConnectionManager_GetAVProtocolInfo(DMR instance, void *session, char **protocolInfo)
{
    ULOGD("[DMR event] %s is called", __func__);
    if( m_pDMR_mediaProtocolInfo != NULL)
//		protocolInfo = &m_pDMR_mediaProtocolInfo;
        *protocolInfo = strdup(m_pDMR_mediaProtocolInfo);
//	if( m_pDMR_ProtocolInfo != NULL )//BuildProtocolInfo(ProtocolInfoList);
//		protocolInfo = &m_pDMR_ProtocolInfo;
//	*protocolInfo = BuildProtocolInfo(ProtocolInfoList);
    return DMR_ERROR_OK;
//	return 0;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_Stop(DMR instance, void *session)
{
    ULOGD("[DMR event] AVTransport Stop...at %ld", time(NULL));

    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    SetDMRBlock();

    if( checkDMRIsInternalCommand() )
    {
        SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_psubRenderer )
    {
        m_psubRenderer->Stop();
    }
    else
    {
        SetDMRUnBlock();
        RETURN_ERROR(701);//"Transition Not Available"
    }

    m_PlayState = DMR_PS_Stopped;
    DMR_StateChange_TransportPlayState(instance, m_PlayState);
    if( ShowStatus != NULL )
        ShowStatus((int)m_PlayState, NULL);

    SetDMRUnBlock();

    if(m_pSetAVURI)
    {
        free(m_pSetAVURI);
        m_pSetAVURI = NULL;
    }

#ifndef DLNADMRCTT
    DMR_StateChange_AbsoluteTimePosition(instance, 0xffffffff);
    DMR_StateChange_RelativeTimePosition(instance, 0);
#else
    DMR_StateChange_AbsoluteTimePosition(instance, 0);
    DMR_StateChange_RelativeTimePosition(instance, 0);
#endif

    ULOGD("[DMR event] AVTransport Stop END...at %ld", time(NULL));

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_Play(DMR instance, void *session, char *playSpeed)
{
    /**
    	0. check the dmr instance
    	 . check the PlayState
    	1. translate the playSpeed
    	2  check if there is AVTransportURI ?
    	3. Run
    	4. Update the CurrentTransportAction, and PlayState
    */
    ULOGD("[DMR event] AVTransport Play speed=%s at %ld",playSpeed, time(NULL));

    int speed = 0;
    float f_speed = 0;
    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    if( (DMR_StateGet_PlayState(instance) != DMR_PS_Stopped) &&
            (DMR_StateGet_PlayState(instance) != DMR_PS_Paused) &&
            (DMR_StateGet_PlayState(instance) != DMR_PS_Transitioning) &&
            (DMR_StateGet_PlayState(instance) != DMR_PS_Playing)
      )
        RETURN_ERROR(701);//"Transition Not Available"

    SetDMRBlock();

    if (m_pDMR_Monitor)
    {
        ILibLifeTime_Remove(m_pDMR_Monitor, &statusFlag);
    }
    RESET_FLAG(statusFlag, STATUS_REQUEST_FOR_PLAY);

	if (HAS_FLAG(statusFlag, STATUS_DESTROY))
	{
		ULOGE("[DMR event] AVTransport Play : DMR is destroy");
        SetDMRUnBlock();
        RETURN_ERROR(701);
	}

    if( checkDMRIsInternalCommand() )
    {
    	SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_psubRenderer == NULL || m_ppMediaTrackURI == NULL )
    {
        do
        {
            if (m_CurrentTrack > 0 && m_ppMediaTrackURI)
            {
            	struct CdsObject *pData = NULL;
            	ASSIGN_CDS_OBJECT(pData, m_pMediaMetadata);
                char * uri = strdup(m_ppMediaTrackURI[m_CurrentTrack-1]);
                RTKMR_AVTransport_Stop(m_pDMR_MicroStack,NULL);
                bool bDone = uri && *uri != '\0' && RTKMR_AVTransport_SetAVTransportURI(m_pDMR_MicroStack, NULL, uri, pData) == DMR_ERROR_OK;
				DELETE_CDS_OBJECT(pData);
				FREE_MEMORY(uri);
                if (bDone)
					break;
            }

            SetDMRUnBlock();
            RETURN_ERROR(702);// "No Contents"
        }
        while(1);
    }

	DMR_StateChange_ResetLastChangeMask(instance, FALSE);

    if( playSpeed != NULL )
    {
        f_speed = atoi(playSpeed);
        if( (f_speed <= 4.0) && (f_speed >= 0.5) )
            speed = (int)(f_speed*256);
        else if( (f_speed >= -2.0) && (f_speed <= -0.5) )
            speed = (int)(f_speed*256);
        else
        {
            SetDMRUnBlock();
            RETURN_ERROR(717);//"Play Speed Not Supported"
        }
    }
    else
    {
#ifdef DLNADMRCTT
        SetDMRUnBlock();
        RETURN_ERROR(717);//"Play Speed Not Supported"
#else
        speed = m_Speed*256;
#endif
    }

    if(m_PlayState == DMR_PS_Paused)   // && m_psubRenderer->Pause() != S_OK)
    {
        ULOGD("[DMR event] Come in de-Pause()");
        m_psubRenderer->Pause(false);
	    m_PlayState = DMR_PS_Playing;
    }
    else if(m_PlayState == DMR_PS_Playing)
    {
        ULOGD("[DMR event] It is already in play mode");
        if (speed != 256) {
             m_psubRenderer->SetRate(speed / 256);
        }
    }
    else
    {
	    int res = m_psubRenderer->Play(m_ppMediaTrackURI[m_CurrentTrack-1], speed);
		if (res == S_OK)
		{
		    m_PlayState = DMR_PS_Playing;
		}
		else if (res == S_FALSE)
		{
		    m_PlayState = DMR_PS_Transitioning;
		}
		else if(res != S_OK)
	    {
	#ifdef DLNADMRCTT
	        // for LPTT 7.3.10.1
	#ifdef ENABLE_DTCP_IP
	        //if(bDtcpIpContent)
	        {
	            struct stat st;
	            if(stat("/tmp/http.error.403",&st) == 0)
	            {
	                SetDMRUnBlock();
	                RETURN_ERROR(722);
	            }
	        }
	#endif /* ENABLE_DTCP_IP */
	#else
	        ULOGE("Play Return Error");
	        SetDMRUnBlock();
	        RETURN_ERROR(716);//"Resource Not Found"
	#endif
	    }
	}

    if( playSpeed )
        DMR_StateChange_TransportPlaySpeed(instance, playSpeed);
    DMR_StateChange_CurrentTrack(instance, m_CurrentTrack); //for DCTT 7.3.100.3
    DMR_StateChange_CurrentTrackURI(instance, m_ppMediaTrackURI[m_CurrentTrack-1]);
    DMR_StateChange_CurrentTrackMetaData(instance, m_pMediaMetadata); //?? how to generate the MetaData??

	UpdateDMRPlayState(m_PlayState);

	SetDMRUnBlock();

    ULOGD("[DMR event] AVTransport Play END at %ld", time(NULL));
    return DMR_ERROR_OK;
}

void RTK_DLNA_DMR::RTKMR_AVTransport_PlayNext(DMR instance, void *session)
{
    unsigned int nextTrack = 0;

#ifdef PPTV
    if( ( MEDIATYPE_PPTV == m_mediaType ) && (DMR_StateGet_PlayState(instance) != DMR_PS_Stopped) )
    {
        ULOGD("%s %d ",__FILE__,__LINE__);

        RTKMR_AVTransport_Stop(instance,session);
    }
#endif

    if( checkDMRIsInternalCommand() )
        return;

    switch( m_PlayMode )
    {
        case DMR_MPM_Normal:
            nextTrack = m_CurrentTrack+1;
            break;
        case DMR_MPM_Repeat_All:
            nextTrack = m_CurrentTrack+1;
            if( nextTrack > m_NumberOfTracks )
                nextTrack = 1;
            break;
        case DMR_MPM_Repeat_One:
            nextTrack = m_CurrentTrack;
            break;
        case DMR_MPM_Random:
        case DMR_MPM_Shuffle:
        case DMR_MPM_Intro:
            //TODO
        case DMR_MPM_Direct_One:
        default:
            nextTrack = m_CurrentTrack+1;
            break;
    }

    ULOGD("PlayNext nextTrack=%d, m_NumberOfTracks(%d)",nextTrack, m_NumberOfTracks);

    if( nextTrack > m_NumberOfTracks )
        RTKMR_AVTransport_Stop(instance, session);
    else
    {
        // determinate if subRenderer_Type is the same?!
        // change the subRenderer
        //TODO, search a appropriate track index
        //deadlock bug using a self terminated thread to do it
        if(m_PlayMode != DMR_MPM_Repeat_One)
            RTKMR_AVTransport_Stop(instance, session);

        m_CurrentTrack = nextTrack;

        pthread_t tid;
        pthread_attr_t  attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, &attr, PlayTrackThread, (void*)NULL);
        pthread_attr_destroy(&attr);

        ULOGD("PlayNext nextTracktype=%d",m_pMediaType[nextTrack-1]);

    }
}

int RTK_DLNA_DMR::RTKMR_AVTransport_SeekTrack(DMR instance, void *session, unsigned int trackIndex)
{
    ULOGD("[DMR event] %s trackIndex:%d ", __func__, trackIndex);
// think of if there is some error, should "RTKMR_AVTransport_Stop()" ??!!!
//	RETURN_ERROR(710);// Seek mode not supported



    int speed = 0;
    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    SetDMRBlock();

    if( checkDMRIsInternalCommand() )
    {
    	SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_psubRenderer == NULL )
    {
        SetDMRUnBlock();
        RETURN_ERROR(702);// "No Contents"
    }

    if( m_NumberOfTracks == 1 )
    {
        if( trackIndex == 0 )		// fast forward
            speed = 2;
        else if( trackIndex == 1 )		// fast reverse
            speed = -2;
        else
        {
            SetDMRUnBlock();
            RETURN_ERROR(711);// Illegal seek target
        }
        if( S_OK == m_psubRenderer->SetRate(speed*256))
        {
            char strSpeed[8] = {0};
            m_Speed = speed;	// ?? or do not need to stored?
            snprintf(strSpeed, 8, "%d", m_Speed);
            DMR_StateChange_TransportPlaySpeed(instance, strSpeed);
        }
        else
        {
            SetDMRUnBlock();
            RETURN_ERROR(710);// Seek mode not supported
        }
        SetDMRUnBlock();
    }
    else
    {
        SetDMRUnBlock();
        if( trackIndex < 1 || trackIndex > m_NumberOfTracks )
            RETURN_ERROR(711);// Illegal seek target
        //TODO, search a appropriate track index
        //mediaType may be not the same, check first, and find the appropriate next one
        m_CurrentTrack = trackIndex;
        DMR_StateChange_CurrentTrack(instance, m_CurrentTrack);
        DMR_StateChange_CurrentTrackURI(instance, m_ppMediaTrackURI[m_CurrentTrack-1]);
        /*
        if((DMR_StateGet_PlayState(instance) == DMR_PS_Paused) ||
        (DMR_StateGet_PlayState(instance) == DMR_PS_Playing))
        {
        	RTKMR_AVTransport_Play(instance, session, NULL);
        }
        */
        pthread_t tid;
        pthread_attr_t  attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, &attr, PlayTrackThread, (void*)NULL);
        pthread_attr_destroy(&attr);
    }

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_SeekTrackPosition(DMR instance, void *session, long position)
{
    ULOGD("[DMR event] %s position:%ld ", __func__, position);
//	RETURN_ERROR(710);// Seek mode not supported
    int ret;
    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    SetDMRBlock();

#ifndef DLNADMRCTT
    if (m_PlayState==DMR_PS_NoMedia || m_PlayState==DMR_PS_Stopped)
    {
        SetDMRUnBlock();
        ULOGD("[DMR event] %s m_PlayState:%d ", __func__, m_PlayState);
        RETURN_ERROR(710); // Seek mode not supported
    }
#endif

    if( checkDMRIsInternalCommand() )
    {
        SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_psubRenderer == NULL )
    {
        SetDMRUnBlock();
        RETURN_ERROR(702);// "No Contents"
    }


    //TODO: see if position is out of range
    if( m_NumberOfTracks == 1)
    {
        ret = m_psubRenderer->SeekMediaPosition(-1, position);
    }
    else
        ret = m_psubRenderer->SeekMediaPosition(m_CurrentTrack-1, position);


    if( ShowStatus != NULL )
        ShowStatus((int)DMR_PS_Transitioning, NULL);

    SetDMRUnBlock();

    if(ret == S_OK)
        return DMR_ERROR_OK;

    ULOGE("[DMR event] SeekTrackPosition error!!!");

    //RETURN_ERROR(710);	// Illegal seek target
    return 711;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_SeekMediaPosition(DMR instance, void *session, long position)
{

    ULOGD("[DMR event] %s position:%ld ", __func__, position);

    /**
    	TODO: This edition temporarily considers "1 title" !!!
    */

//	RETURN_ERROR(710);// Seek mode not supported

    int ret;
    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    SetDMRBlock();

#ifndef DLNADMRCTT
    if (m_PlayState==DMR_PS_NoMedia || m_PlayState==DMR_PS_Stopped)
    {
        SetDMRUnBlock();
        ULOGD("[DMR event] %s m_PlayState:%d ", __func__, m_PlayState);
        RETURN_ERROR(710); // Seek mode not supported
    }
#endif

    if( checkDMRIsInternalCommand() )
    {
        SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_psubRenderer == NULL )
    {
        SetDMRUnBlock();
        RETURN_ERROR(702);// "No Contents"
    }

    ULOGD(">>>>>>>>>>>>>>>>>> seek position=%ld", position);
    //TODO: see if position is out of range
    ret = m_psubRenderer->SeekMediaPosition(-1, position);

    if( ShowStatus != NULL )
        ShowStatus((int)DMR_PS_Transitioning, NULL);

    SetDMRUnBlock();

    if(ret == S_OK)
        return DMR_ERROR_OK;

    ULOGE("[DMR event] SeekMediaPosition error!!!");

    return 711;	// Illegal seek target
}

int RTK_DLNA_DMR::RTKMR_AVTransport_Next(DMR instance, void *session)
{
	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    if( m_NumberOfTracks != 1 && m_CurrentTrack < m_NumberOfTracks)
        return RTKMR_AVTransport_SeekTrack(instance, session, m_CurrentTrack+1);

    if(m_PlayMode == DMR_MPM_Repeat_One)
        RTK_DLNA_DMR::RTKMR_AVTransport_PlayNext(instance, session);

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_Previous(DMR instance, void *session)
{
	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        RETURN_ERROR(718);// Invalid InstanceID

    if( m_NumberOfTracks != 1 && m_CurrentTrack > 1)
        return RTKMR_AVTransport_SeekTrack(instance, session, m_CurrentTrack-1);

    if(m_PlayMode == DMR_MPM_Repeat_One)
        RTK_DLNA_DMR::RTKMR_AVTransport_PlayNext(instance, session);

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_RenderingControl_SelectPreset(DMR instance, void *session, char *presetName)
{
	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        return 718;// Invalid InstanceID

    if( presetName != NULL &&
            ( strncmp(presetName, "FactoryDefaults", strlen("FactoryDefaults")) ||
              strncmp(presetName, "InstallationDefaults", strlen("InstallationDefaults")) ) )
    {
        // reset
        // brightness, contrast,
        // Mute, Volume, VolumeDB, Loudness
        // there are not in our system:
        // sharpness, colorTemperature, HorizontalKeystone, VerticalKeystone
        ExecEventNotify(DLNA_DMR_NOTIFY_ID_FACTORY_DEFAULT);
    }

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_Pause(DMR instance, void *session)
{

	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        return 718;// Invalid InstanceID

    SetDMRBlock();

    if( checkDMRIsInternalCommand() )
    {
        SetDMRUnBlock();
        RETURN_ERROR(402);
    }

    if( m_psubRenderer == NULL )
    {
        SetDMRUnBlock();
        RETURN_ERROR(702);// "No Contents"
    }

    //yunfeng_han
    if((m_PlayState == DMR_PS_Paused)||(m_PlayState == DMR_PS_Stopped))
    {
        SetDMRUnBlock();
        ULOGE("m_playstate=%d",m_PlayState);
        return 701;//Transition Not Available
    }

    if( m_psubRenderer->Pause() == S_OK )
    {
        m_PlayState = DMR_PS_Paused;
        DMR_StateChange_TransportPlayState(instance, m_PlayState);
        if( ShowStatus != NULL )
            ShowStatus((int)m_PlayState, NULL);
        SetDMRUnBlock();
        return DMR_ERROR_OK;
    }

    SetDMRUnBlock();
    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_AVTransport_SetPlayMode(DMR instance, void *session, DMR_MediaPlayMode playMode)
{
	ULOGD("[DMR event] %s is called", __func__);
    if( m_pDMR_MicroStack != instance )
        return 718;// Invalid InstanceID

    m_PlayMode = playMode;
    switch( playMode )
    {
        case DMR_MPM_Normal:
        case DMR_MPM_Repeat_One:
        case DMR_MPM_Repeat_All:
        case DMR_MPM_Random:
        case DMR_MPM_Shuffle:
        case DMR_MPM_Intro:
            break;
        case DMR_MPM_Direct_One:
        default:
            m_PlayMode = DMR_MPM_Normal;
            DMR_StateChange_CurrentPlayMode(instance, m_PlayMode);
            return 712;// play mode not supported
    }
    DMR_StateChange_CurrentPlayMode(instance, m_PlayMode);
    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_RenderingControl_SetVolume(DMR instance, void *session, unsigned char volume)
{
   	ULOGD("[DMR event] %s , Volume : %d", __func__, volume);

    int nVolume = (unsigned short) volume;
    int vol = 0;
	if( m_pDMR_MicroStack != instance )
		return 718;// Invalid InstanceID

	if( (vol > MAX_VOLUME_NUMBER) || (vol < MIN_VOLUME_NUMBER) )
		return 601;// Argument Value Out of Range

	RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_SETVOLUME, &nVolume);
	DMR_StateChange_Volume(instance, volume);
	DMR_StateChange_Mute(instance, false);
    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_RenderingControl_SetMute(DMR instance, void *session, int mute)
{
	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        return 718;// Invalid InstanceID

	RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_SETMUTE, &mute);
	DMR_StateChange_Mute(instance, mute);

    return DMR_ERROR_OK;
}
#if defined(INCLUDE_FEATURE_DISPLAY)
int RTK_DLNA_DMR::RTKMR_RenderingControl_SetContrast(DMR instance, void *session, unsigned char contrast)
{
	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        return 718;// Invalid InstanceID

    if( contrast > MAX_CONTRAST_VALUE )
        return 601;// Argument Value Out of Range

	int nValue = (int)contrast;
	RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_SETCONTRAST, &nValue);

    //State update
    DMR_StateChange_Contrast(instance, contrast);

    return DMR_ERROR_OK;
}

int RTK_DLNA_DMR::RTKMR_RenderingControl_SetBrightness(DMR instance, void *session, unsigned char brightness)
{
	ULOGD("[DMR event] %s Method is called", __func__);
    if( m_pDMR_MicroStack != instance )
        return 718;// Invalid InstanceID

    if( brightness > MAX_BRIGHTNESS_VALUE )
        return 601;// Argument Value Out of Range

	int nValue = (int)brightness;
	RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID_SETBRIGHTNESS, &nValue);

    //State update
    DMR_StateChange_Brightness(instance, brightness);

    return DMR_ERROR_OK;
}
#endif
SUBRENDERER_TYPE RTK_DLNA_DMR::RendererType(char *str,int type)
{
    unsigned int i = 0;
    if(type == 0)   //extension
    {
        for( i = 0; i < sizeof(extension_protocolInfo_mapping)/sizeof(EXTTORENDERERTYPE); i++)
        {
            if( !strncasecmp( str, extension_protocolInfo_mapping[i].mediaExtension, strlen(extension_protocolInfo_mapping[i].mediaExtension)) )
            {
                FREE_MEMORY(m_pDMR_mediaProtocolInfo);
                m_pDMR_mediaProtocolInfo = strdup(extension_protocolInfo_mapping[i].mediaProtocolInfo);
                FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);
                m_pDMR_mediaMimeTypeProtocol = strdup(extension_protocolInfo_mapping[i].mimeTypeProtocolInfo);
                m_mediaType = extension_protocolInfo_mapping[i].mediaType;
                ULOGD("\t-------------------------------");
                ULOGD("\t[%d] media protocol info is set by %s, index:%d", __LINE__, __func__, i);
                ULOGD("\t-------------------------------");
                return extension_protocolInfo_mapping[i].subrendererType;
            }

        }
    }
    else   //protocolinfo
    {
        for( i = 0; i < sizeof(extension_protocolInfo_mapping)/sizeof(EXTTORENDERERTYPE); i++)
        {
            if( strstr(str, extension_protocolInfo_mapping[i].mimeTypeProtocolInfo))
            {
                FREE_MEMORY(m_pDMR_mediaProtocolInfo);
                m_pDMR_mediaProtocolInfo = strdup(str);
                FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);
                m_pDMR_mediaMimeTypeProtocol = strdup(extension_protocolInfo_mapping[i].mimeTypeProtocolInfo);
                m_mediaType = extension_protocolInfo_mapping[i].mediaType;
                ULOGD("\t-------------------------------");
                ULOGD("\t[%d] media protocol info is set by %s, index:%d", __LINE__, __func__, i);
                ULOGD("\t-------------------------------");
                return extension_protocolInfo_mapping[i].subrendererType;
            }
        }

    }
    return DMR_SUBR_NOTHING;
}


bool RTK_DLNA_DMR::CheckPlayContainerURI()
{
    bool ret = false;
    if(m_pSetAVURI && !strncmp(m_pSetAVURI,"dlna-playcontainer://",strlen("dlna-playcontainer://")))
        ret = true;
    return ret;
}

void RTK_DLNA_DMR::RegisterDlnaDmrCallbackFunc( int (*updateFuncPtr)(int, void*), void *pParam ,DLNA_DMR_CB_FUNC func_type)
{
    switch( func_type )
    {
        case DLNA_DMR_CB_QUERYFORCONNECT:
            QueryForConnection      = updateFuncPtr;
        case DLNA_DMR_CB_PREPAREFORCONNECT:
            PrepareForConnection    = updateFuncPtr;
            break;
        case DLNA_DMR_CB_PREPAREFORDISCONNECT:
            PrepareForDisconnection = updateFuncPtr;
            break;
        case DLNA_DMR_CB_SETBRIGHTNESS:
            SetBrightness           = updateFuncPtr;
            break;
        case DLNA_DMR_CB_SETCONTRAST:
            SetContrast             = updateFuncPtr;
            break;
        case DLNA_DMR_CB_SHOWSTATUS:
            ShowStatus              = updateFuncPtr;
            break;
        case DLNA_DMR_CB_UPDATEINFO:
            UpdateMediaInfo         = updateFuncPtr;
            break;
        case DLNA_DMR_CB_SHOWVOLUMESTATUS:
            ShowVolumeStatus        = updateFuncPtr;
            break;
        case DLNA_DMR_CB_SHOWDIALOG:
            ShowDialog				= updateFuncPtr;
        case DLNA_DMR_CB_RESTART:
            RestartDMR              = updateFuncPtr;
            break;
        case DLNA_DMR_CB_EVENT:
        	EventNotify				= updateFuncPtr;
        	break;
        case DLNA_DMR_CB_NONE:
        default:
            break;
    }
}

void RTK_DLNA_DMR::DlnaDmrInternalStop()
{
    bool bDestroyRender = !HAS_FLAG(statusFlag, STATUS_SET_AV_TRANSPORT_URI);
    pthread_mutex_lock(&m_mutexSubRenderer);

    SetDMRInternalCommandInit();

    if( m_psubRenderer )
    {
        m_psubRenderer->Stop();
        m_PlayState = DMR_PS_Stopped;
        DMR_StateChange_TransportPlayState(m_pDMR_MicroStack, m_PlayState);
    }

    if (bDestroyRender)
    {
        ULOGD("subRenderer destroy in DlnaDmrInternalStop");
        DELETE_OBJECT(m_psubRenderer);
        FREE_MEMORY(m_pSetAVURI);
    }

    DMR_StateChange_AbsoluteTimePosition(m_pDMR_MicroStack, 0);
    DMR_StateChange_RelativeTimePosition(m_pDMR_MicroStack, 0);

    SetDMRInternalCommandDone();

    pthread_mutex_unlock(&m_mutexSubRenderer);
    //TODO, reset state
}

void RTK_DLNA_DMR::DlnaDmrInternalPause(bool bUpdateState)
{
    if(checkDMRIsBusy())
        return;

    SetDMRInternalCommandInit();
    if( m_psubRenderer != NULL && m_psubRenderer->Pause() == S_OK && bUpdateState)
    {
        m_PlayState = DMR_PS_Paused;
        DMR_StateChange_TransportPlayState(m_pDMR_MicroStack, m_PlayState);
    }
    SetDMRInternalCommandDone();
    return;
}

void RTK_DLNA_DMR::DlnaDmrInternalPlayNextURI()
{
    if(checkDMRIsBusy())
        return;

    char * uri = DMR_StateGet_NextAVTransportURI(m_pDMR_MicroStack);
    RTKMR_AVTransport_Stop(m_pDMR_MicroStack,NULL);
    if(uri && *uri != '\0' && RTKMR_AVTransport_SetAVTransportURI(m_pDMR_MicroStack,NULL,uri,m_pNextURIMetadata) == DMR_ERROR_OK)
        RTKMR_AVTransport_Play(m_pDMR_MicroStack,NULL,"1");
}

void RTK_DLNA_DMR::DlnaDmrInternalUnPause(bool bUpdateState)
{
    if(checkDMRIsBusy())
        return;

    SetDMRInternalCommandInit();
    if( m_psubRenderer != NULL && m_psubRenderer->Pause(false) == S_OK && bUpdateState)
    {
       	m_PlayState = DMR_PS_Playing;
       	DMR_StateChange_TransportPlayState(m_pDMR_MicroStack, m_PlayState);
    }
    SetDMRInternalCommandDone();
}

int RTK_DLNA_DMR::DlnaDmrInternalSeek(int seconds)
{
    if((m_PlayState != DMR_PS_Playing))
        return -1;

    if(checkDMRIsBusy())
        return -1;

    SetDMRInternalCommandInit();
    if( m_psubRenderer != NULL )
    {
        int ret = m_psubRenderer->SeekMediaPosition(-1, seconds);
        if(ret != S_OK)
        {
            SetDMRInternalCommandDone();
            return -1;
        }

        m_PlayState = DMR_PS_Playing;
        DMR_StateChange_TransportPlayState(m_pDMR_MicroStack, m_PlayState);
    }
    SetDMRInternalCommandDone();
    return 0;
}

void RTK_DLNA_DMR::DlnaDmrSyncRendererVar(unsigned char brightness, unsigned char contrast, unsigned char volume, bool mute)
{
#if defined(INCLUDE_FEATURE_DISPLAY)
	DMR_StateChange_Brightness(m_pDMR_MicroStack, brightness);
    DMR_StateChange_Contrast(m_pDMR_MicroStack, contrast);
#endif
    DMR_StateChange_Volume(m_pDMR_MicroStack, volume);
    //TODO: check DMR_StateChange_Volume(instance, (int)(misc_default.m_volume*100/MAX_VOLUME_NUMBER));
    DMR_StateChange_Mute(m_pDMR_MicroStack, mute);
}

SUBRENDERER_TYPE RTK_DLNA_DMR::RendererType(struct CdsObject *data)
{
    unsigned int i = 0;
    ASSIGN_CDS_OBJECT(m_pMediaMetadata, data);

    if( data->Res != NULL && data->Res->ProtocolInfo != NULL && strlen(data->Res->ProtocolInfo)!= 0)
    {
        FREE_MEMORY(m_pDMR_mediaProtocolInfo);
        m_pDMR_mediaProtocolInfo = strdup(data->Res->ProtocolInfo);
        for( i = 0; i < sizeof(extension_protocolInfo_mapping)/sizeof(EXTTORENDERERTYPE); i++)
        {
            if( strstr(data->Res->ProtocolInfo, extension_protocolInfo_mapping[i].mimeTypeProtocolInfo))
            {
                FREE_MEMORY(m_pDMR_mediaMimeTypeProtocol);
                m_pDMR_mediaMimeTypeProtocol = strdup(extension_protocolInfo_mapping[i].mimeTypeProtocolInfo);
                m_mediaType = extension_protocolInfo_mapping[i].mediaType;
                ULOGD("\t-------------------------------");
                ULOGD("\t[%d] media protocol info is set by %s\n\tprotocolInfo:%s", __LINE__, __func__, m_pDMR_mediaProtocolInfo);
                ULOGD("\t-------------------------------");
                return extension_protocolInfo_mapping[i].subrendererType;
            }
        }
    }
#if 1
    unsigned int objectMajorType = CDS_CLASS_MASK_MAJOR & data->MediaClass;
    for( i = 0; i < sizeof(cds_renderertype_mapping)/sizeof(CDSTORENDERERTYPE); i++)
    {
        if( cds_renderertype_mapping[i].objectMajorType == objectMajorType )
        {
            if( data->Res != NULL && data->Res->ProtocolInfo != NULL )
            {
                FREE_MEMORY(m_pDMR_mediaProtocolInfo);
                m_pDMR_mediaProtocolInfo = strdup(data->Res->ProtocolInfo);
            }
            ULOGD("\t-------------------------------");
            ULOGD("\t[%d] media protocol info is set by %s\n\objectMajorType:%d", __LINE__, __func__, objectMajorType);
            ULOGD("\t-------------------------------");
            m_pDMR_mediaMimeTypeProtocol = strdup(cds_renderertype_mapping[i].mimeTypeProtocolInfo);
            return cds_renderertype_mapping[i].subrendererType;
        }
    }
#endif


    return DMR_SUBR_NOTHING;
}


SUBRENDERER_TYPE RTK_DLNA_DMR::GetRendererType()//{return m_psubRenderer->GetRendererType();};
{
    if( m_psubRenderer != NULL )
        return m_psubRenderer->GetRendererType();
    else
        return DMR_SUBR_NOTHING;
}

char* RTK_DLNA_DMR::GetRenderMediaFilename()
{
    if(m_pMediaMetadata != NULL)
    {
        return m_pMediaMetadata->Title;
    }
    return NULL;
}

char* RTK_DLNA_DMR::GetRenderMediaFullname()
{
    if(m_ppMediaTrackURI && (m_NumberOfTracks >= 1) && (m_CurrentTrack >= 1) && (m_CurrentTrack <= m_NumberOfTracks))
    {
        return m_ppMediaTrackURI[m_CurrentTrack -1];
    }
    else if(m_pSetAVURI)
    {
        return m_pSetAVURI;
    }
    return NULL;
}

long RTK_DLNA_DMR::GetRenderMediaFilesize()
{
    if(m_pMediaMetadata != NULL && m_pMediaMetadata->Res != NULL)
    {
        return m_pMediaMetadata->Res->Size;
    }
    return 0;
}

long RTK_DLNA_DMR::GetRenderMediaDuration()
{
    if(m_pMediaMetadata != NULL && m_pMediaMetadata->Res != NULL)
    {
        if(m_pMediaMetadata->Res->Duration >= 0)
            return m_pMediaMetadata->Res->Duration;
        else
            return 0;
    }
    return 0;
}

int RTK_DLNA_DMR::GetRenderMediaResolutinoX()
{
    if(m_pMediaMetadata != NULL && m_pMediaMetadata->Res != NULL)
    {
        if((m_pMediaMetadata->Res->ResolutionX < 0) && m_psubRenderer)
        {
            int x,y;
            if(m_psubRenderer->GetResolution(&x,&y) == S_OK)
                return x;
        }
        return m_pMediaMetadata->Res->ResolutionX;
    }
    return 0;
}

int RTK_DLNA_DMR::GetRenderMediaResolutionY()
{
    if(m_pMediaMetadata != NULL && m_pMediaMetadata->Res != NULL)
    {
        if((m_pMediaMetadata->Res->ResolutionY < 0) && m_psubRenderer)
        {
            int x,y;
            if(m_psubRenderer->GetResolution(&x,&y) == S_OK)
                return y;
        }
        return m_pMediaMetadata->Res->ResolutionY;
    }
    return 0;
}

int RTK_DLNA_DMR::GetRenderMediaColorDepth()
{
    if(m_pMediaMetadata != NULL && m_pMediaMetadata->Res != NULL)
    {
        return m_pMediaMetadata->Res->ColorDepth;
    }
    return 0;
}

bool RTK_DLNA_DMR::isURIAlive()
{
    return false;
}

void RTK_DLNA_DMR::SetDMRMessageData(DMRMessageData &msgData)
{
    SetDMRBlock();
    SET_FLAG(statusFlag, STATUS_MESSAGE_DATA_EXIST);
    g_DMRMessageData = msgData;
    SetDMRUnBlock();
}

bool RTK_DLNA_DMR::GetDMRMessageData(DMRMessageData &msgData)
{
    bool res = false;
    if(!checkDMRIsBusy())
    {
        SetDMRBlock();
        if (HAS_FLAG(statusFlag, STATUS_MESSAGE_DATA_EXIST))
        {
            msgData = g_DMRMessageData;
            RESET_FLAG(statusFlag, STATUS_MESSAGE_DATA_EXIST);
            res= true;

        }
        SetDMRUnBlock();
    }

    return res;
}

void RTK_DLNA_DMR::SetDMRRendererVar(DLNA_DMR_RENDERERVAR type, unsigned char value)
{
    switch (type)
    {
        case DLNA_DMR_RENDERERVAR_VOL_NUMBER:
            DMR_StateChange_Volume(m_pDMR_MicroStack, value);
            break;
        case DLNA_DMR_RENDERERVAR_VOL_MUTE:
            DMR_StateChange_Mute(m_pDMR_MicroStack, (BOOL)value);
            break;
    }
}

int RTK_DLNA_DMR::UpdateSubRender(SUBRENDERER_TYPE renderType)
{
	DELETE_OBJECT(m_psubRenderer);
    switch( renderType )
    {
        case DMR_SUBR_AUDIO:
        case DMR_SUBR_VIDEO:
        case DMR_SUBR_IMAGE:
            m_psubRenderer = new mmRenderer(renderType);
            break;
        case DMR_SUBR_TEXT:
            ULOGD("[DMR event] this is text file\n");
            return 602; //"Optional Action Not Implemented"
        case DMR_SUBR_PLAYLIST:
            ULOGD("[DMR event] this is a playlist...\n");
            m_psubRenderer = new playlistRenderer();
            break;
        default:
			ULOGE("[DMR event] this is nothing, or not support format.....");
       		return -1;
    }

    return 0;
}

int RTK_DLNA_DMR::ExecEventNotify(DLNA_DMR_NOTIFY_ID id, void* pData)
{
	if(EventNotify)
    {
    	return EventNotify(id, pData);
	}

	return -1;
}

void RTK_DLNA_DMR::UpdateStatus(DMR_UPDATE_STATUS_TYPE type, long value)
{
	if (!m_pDMR_MicroStack)
		return;

    if (m_psubRenderer)
    {
        m_psubRenderer->UpdateStatus(type, value);
    }

	char *pTypeString = NULL;

	switch (type)
	{
	case DMR_UPDATE_STATUS_CURRENTPLAYMODE:
		pTypeString = "Current Play Mode";
		m_PlayMode = (DMR_MediaPlayMode) value;
		DMR_StateChange_CurrentPlayMode(m_pDMR_MicroStack, m_PlayMode);
		break;
#if defined(INCLUDE_FEATURE_DISPLAY)
	case DMR_UPDATE_STATUS_CONTRAST:
		pTypeString = "Contrast";
		DMR_StateChange_Contrast(m_pDMR_MicroStack, (unsigned char)value);
		break;
	case DMR_UPDATE_STATUS_BRIGHTNESS:
		pTypeString = "Brightness";
		DMR_StateChange_Brightness(m_pDMR_MicroStack, (unsigned char)value);
		break;
#endif
	case DMR_UPDATE_STATUS_VOLUME:
		pTypeString = "Volume";
		DMR_StateChange_Volume(m_pDMR_MicroStack, (unsigned char)value);
		DMR_StateChange_Mute(m_pDMR_MicroStack, FALSE);
		break;
	case DMR_UPDATE_STATUS_MUTE:
		pTypeString = "Mute";
		DMR_StateChange_Mute(m_pDMR_MicroStack, (BOOL)value);
		break;
	case DMR_UPDATE_STATUS_TRANSPORTSTATE:
		pTypeString = "Transport State";
		if (m_PlayState != (DMR_PlayState) value)
		{
			m_PlayState = (DMR_PlayState) value;
			DMR_StateChange_TransportPlayState(m_pDMR_MicroStack, m_PlayState);
			if (m_PlayState == DMR_PS_Stopped)
			{
				DMR_StateChange_AbsoluteTimePosition(m_pDMR_MicroStack, 0);
				DMR_StateChange_RelativeTimePosition(m_pDMR_MicroStack, 0);
				RTKMR_AVTransport_PlayNext(m_pDMR_MicroStack, NULL);
			}
		}
		break;
	case DMR_UPDATE_STATUS_DURATION:
        if (m_PlayState == DMR_PS_Playing)
        {
            pTypeString = "Duration";
            DMR_StateChange_CurrentTrackDuration(m_pDMR_MicroStack, value);
            DMR_StateChange_CurrentMediaDuration(m_pDMR_MicroStack, value);
        }
		break;
	case DMR_UPDATE_STATUS_SEEK:
		pTypeString = "Seek";
		if (value)
		{
			DMR_StateChange_UpdateCurrentTransportActions(m_pDMR_MicroStack, DMR_ATS_Seek, 0);
		}
		else
		{
			DMR_StateChange_UpdateCurrentTransportActions(m_pDMR_MicroStack, 0, DMR_ATS_Seek);
		}
		break;
	case DMR_UPDATE_STATUS_TIMEPOSITION:
		if (m_PlayState == DMR_PS_Playing)
		{
			pTypeString = "Time Position";
			DMR_StateChange_AbsoluteTimePosition(m_pDMR_MicroStack, value);
			DMR_StateChange_RelativeTimePosition(m_pDMR_MicroStack, value);
		}
		break;
	}

	if (pTypeString)
		ULOGD("UpdateStatus Type [%s] - (%d)", pTypeString, value);

}

void RTK_DLNA_DMR::UpdateDMRPlayState(DMR_PlayState state)
{
	m_PlayState = state;
    DMR_StateChange_TransportPlayState(m_pDMR_MicroStack, m_PlayState);
	if(ShowStatus != NULL)
	    ShowStatus(m_PlayState, NULL);
}

void RTK_DLNA_DMR::DMR_Monitor(void *pData)
{
    long *pStatusFlag = (long*)pData;

    ULOGD("RTK_DLNA_DMR::DMR_Monitor is called for %ld", *pStatusFlag);

    if (HAS_FLAG(statusFlag, STATUS_REQUEST_FOR_PLAY))
    {
        RTKMR_AVTransport_Play(m_pDMR_MicroStack, NULL, "1");
    }
}

