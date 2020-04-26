#include <utils/Log.h>
#include <string.h>
#include <media/stagefright/MediaDefs.h>

namespace android {

static const char audioPcm[] = "PCM";
//static const char audioLpcm[] = "LPCM";
//static const char audioMpeg1[] = "MPEG1";
//static const char audioMpeg2[] = "MPEG2";
static const char audioDolbyDigital[] = "Dolby Digital";
static const char audioDolbyDigitalPlus[] = "Dolby Digital Plus";
static const char audioMpeg[] = "MPEG";
static const char audioDts[] = "DTS";
static const char audioDtsHD[] = "DTS-HD";
static const char audioDolbyTrueHD[] = "Dolby TrueHD";
//static const char audioSdds[] = "SDDS";
static const char audioDv[] = "DV";
static const char audioAac[] = "AAC";
static const char audioWma[] = "WMA";
static const char audioWmaPro[] = "WMA Pro";
static const char audioAdpcm[] = "ADPCM";
//static const char audioAiff[] = "AIFF";
static const char audioOgg[] = "OGG";
//static const char audioMp3[] = "MP3";
//static const char audioWave[] = "WAVE";
static const char audioAmr[] = "AMR";

const char* getAudioTypeString(const char *mimeType)
{
    const char *pRet = NULL;
    if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_AMR_NB) == 0)
        pRet = audioAmr;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_AMR_WB) == 0)
        pRet = audioAmr;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_MPEG) == 0)
        pRet = audioMpeg;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_I) == 0)
        pRet = audioMpeg;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_MPEG_LAYER_II) == 0)
        pRet = audioMpeg;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_MIDI) == 0)
        pRet = "MIDI";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_AAC) == 0)
        pRet = audioAac;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_QCELP) == 0)
        pRet = "QCELP";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_VORBIS) == 0)
        pRet = audioOgg;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_G711_ALAW) == 0)
        pRet = "G711_ALAW";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_G711_MLAW) == 0)
        pRet = "G711_MLAW";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_RAW) == 0)
        pRet = audioPcm;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_FLAC) == 0)
        pRet = "FLAC";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_AAC_ADTS) == 0)
        pRet = audioAac;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_AAC_LATM) == 0)
        pRet = audioAac;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_AC3) == 0)
        pRet = audioDolbyDigital;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_EAC3) == 0)
        pRet = audioDolbyDigital;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_DDP) == 0)
        pRet = audioDolbyDigitalPlus;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_DTS) == 0)
        pRet = audioDts;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_DTS_HD) == 0)
        pRet = audioDtsHD;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_WMA) == 0)
        pRet = audioWma;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_WMAPRO) == 0)
        pRet = audioWmaPro;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_RA_COOK) == 0)
        pRet = "RA_COOK";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_RA_LSD) == 0)
        pRet = "RA_LSD";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_RA_RA1) == 0)
        pRet = "RA_RA1";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_DV) == 0)
        pRet = audioDv;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_MLP) == 0)
        pRet = audioDolbyTrueHD;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_SILK) == 0)
        pRet = "SILK";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_G729) == 0)
        pRet = "G729";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_APE) == 0)
        pRet = "APE";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_ADPCM) == 0)
        pRet = audioAdpcm;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_ALAC) == 0)
        pRet = "ALAC";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_MSGSM) == 0)
        pRet = "MSGSM";
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_PCM) == 0)
        pRet = audioPcm;
    else if(strcasecmp(mimeType, MEDIA_MIMETYPE_AUDIO_DSD) == 0)
        pRet = "DSD";
    else
        pRet = mimeType;
    return pRet;
}
}
