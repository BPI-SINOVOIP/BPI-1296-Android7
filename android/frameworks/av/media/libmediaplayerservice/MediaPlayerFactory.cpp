/*
**
** Copyright 2012, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#define LOG_TAG "MediaPlayerFactory"
#include <utils/Log.h>

#include <cutils/properties.h>
#include <media/IMediaPlayer.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/FileSource.h>
#include <media/stagefright/foundation/ADebug.h>
#include <utils/Errors.h>
#include <utils/misc.h>
#include <../libstagefright/include/WVMExtractor.h>

#include "MediaPlayerFactory.h"

#include "TestPlayerStub.h"
#include "nuplayer/NuPlayerDriver.h"
#ifdef USE_RT_MEDIA_PLAYER
#include <RTMediaPlayer.h>
#include <RTDtvMediaPlayer.h>
#include <audioParser.h>

#define DTS_PARSER_SIZE       (144 * 1024)

#define ENDIAN_CHANGE(x) (((x&0xff000000)>>24)|((x&0x00ff0000)>>8)|((x&0x0000ff00)<<8)|((x&0x000000ff)<<24))

#endif // USE_RT_MEDIA_PLAYER

//#define HTTP_USE_NUPLAYER 1

namespace android {

Mutex MediaPlayerFactory::sLock;
MediaPlayerFactory::tFactoryMap MediaPlayerFactory::sFactoryMap;
bool MediaPlayerFactory::sInitComplete = false;
#ifdef USE_RT_MEDIA_PLAYER
int MediaPlayerFactory::mForceUseRTMediaPlayer = FORCE_NONE;
#endif // USE_RT_MEDIA_PLAYER

status_t MediaPlayerFactory::registerFactory_l(IFactory* factory,
                                               player_type type) {
    if (NULL == factory) {
        ALOGE("Failed to register MediaPlayerFactory of type %d, factory is"
              " NULL.", type);
        return BAD_VALUE;
    }

    if (sFactoryMap.indexOfKey(type) >= 0) {
        ALOGE("Failed to register MediaPlayerFactory of type %d, type is"
              " already registered.", type);
        return ALREADY_EXISTS;
    }

    if (sFactoryMap.add(type, factory) < 0) {
        ALOGE("Failed to register MediaPlayerFactory of type %d, failed to add"
              " to map.", type);
        return UNKNOWN_ERROR;
    }

    return OK;
}

static player_type getDefaultPlayerType() {
// Roll back NU_PLAYER as default, due to Youtube need to use this player, and persist.media.USE_RTMediaPlayer is true now
#if 0//def USE_RT_MEDIA_PLAYER
    char value[PROPERTY_VALUE_MAX];
    if (property_get("persist.media.USE_RTMediaPlayer", value, NULL)
            && (!strcmp("1", value) || !strcasecmp("true", value))) {
        return REALTEK_PLAYER;
    }
#endif // USE_RT_MEDIA_PLAYER
    return NU_PLAYER;
}

status_t MediaPlayerFactory::registerFactory(IFactory* factory,
                                             player_type type) {
    Mutex::Autolock lock_(&sLock);
    return registerFactory_l(factory, type);
}

void MediaPlayerFactory::unregisterFactory(player_type type) {
    Mutex::Autolock lock_(&sLock);
    sFactoryMap.removeItem(type);
}

#define GET_PLAYER_TYPE_IMPL(a...)                      \
    Mutex::Autolock lock_(&sLock);                      \
                                                        \
    player_type ret = STAGEFRIGHT_PLAYER;               \
    float bestScore = 0.0;                              \
                                                        \
    for (size_t i = 0; i < sFactoryMap.size(); ++i) {   \
                                                        \
        IFactory* v = sFactoryMap.valueAt(i);           \
        float thisScore;                                \
        CHECK(v != NULL);                               \
        thisScore = v->scoreFactory(a, bestScore);      \
        if (thisScore > bestScore) {                    \
            ret = sFactoryMap.keyAt(i);                 \
            bestScore = thisScore;                      \
        }                                               \
    }                                                   \
                                                        \
    if (0.0 == bestScore) {                             \
        ret = getDefaultPlayerType();                   \
    }                                                   \
                                                        \
    return ret;

player_type MediaPlayerFactory::getPlayerType(const sp<IMediaPlayer>& client,
                                              const char* url) {
#ifdef USE_RT_MEDIA_PLAYER
    int forceUseMediaPlayer = MediaPlayerFactory::forceUseMediaPlayer();
	if(forceUseMediaPlayer!=-1) {
	    return (player_type)forceUseMediaPlayer;
	}
#endif // USE_RT_MEDIA_PLAYER
    GET_PLAYER_TYPE_IMPL(client, url);
}

player_type MediaPlayerFactory::getPlayerType(const sp<IMediaPlayer>& client,
                                              int fd,
                                              int64_t offset,
                                              int64_t length) {
#ifdef USE_RT_MEDIA_PLAYER
    int forceUseMediaPlayer = MediaPlayerFactory::forceUseMediaPlayer();
	if(forceUseMediaPlayer!=-1) {
	    return (player_type)forceUseMediaPlayer;
	}
#endif // USE_RT_MEDIA_PLAYER
    GET_PLAYER_TYPE_IMPL(client, fd, offset, length);
}

player_type MediaPlayerFactory::getPlayerType(const sp<IMediaPlayer>& client,
                                              const sp<IStreamSource> &source) {
#ifdef USE_RT_MEDIA_PLAYER
    int forceUseMediaPlayer = MediaPlayerFactory::forceUseMediaPlayer();
	if(forceUseMediaPlayer!=-1) {
	    return (player_type)forceUseMediaPlayer;
	}
#endif // USE_RT_MEDIA_PLAYER
    GET_PLAYER_TYPE_IMPL(client, source);
}

player_type MediaPlayerFactory::getPlayerType(const sp<IMediaPlayer>& client,
                                              const sp<DataSource> &source) {
#ifdef USE_RT_MEDIA_PLAYER
    int forceUseMediaPlayer = MediaPlayerFactory::forceUseMediaPlayer();
	if(forceUseMediaPlayer!=-1) {
	    return (player_type)forceUseMediaPlayer;
	}
#endif // USE_RT_MEDIA_PLAYER
    GET_PLAYER_TYPE_IMPL(client, source);
}

#undef GET_PLAYER_TYPE_IMPL

sp<MediaPlayerBase> MediaPlayerFactory::createPlayer(
        player_type playerType,
        void* cookie,
        notify_callback_f notifyFunc,
        pid_t pid) {
    sp<MediaPlayerBase> p;
    IFactory* factory;
    status_t init_result;
    Mutex::Autolock lock_(&sLock);

    mForceUseRTMediaPlayer = FORCE_NONE;
    if (sFactoryMap.indexOfKey(playerType) < 0) {
        ALOGE("Failed to create player object of type %d, no registered"
              " factory", playerType);
        return p;
    }

    factory = sFactoryMap.valueFor(playerType);
    CHECK(NULL != factory);
    p = factory->createPlayer(pid);

    if (p == NULL) {
        ALOGE("Failed to create player object of type %d, create failed",
               playerType);
        return p;
    }

    init_result = p->initCheck();
    if (init_result == NO_ERROR) {
        p->setNotifyCallback(cookie, notifyFunc);
    } else {
        ALOGE("Failed to create player object of type %d, initCheck failed"
              " (res = %d)", playerType, init_result);
        p.clear();
    }

    return p;
}

#ifdef USE_RT_MEDIA_PLAYER
status_t MediaPlayerFactory::useRTMediaPlayer(int forceType)
{
    ALOGV("MediaPlayerFactory::useRTMediaPlayer...: %d", forceType);
    mForceUseRTMediaPlayer = forceType;
    return OK;
}

int MediaPlayerFactory::forceUseMediaPlayer()
{
    ALOGV("MediaPlayerFactory::forceUseMediaPlayer::%d..." , MediaPlayerFactory::mForceUseRTMediaPlayer);
    if(MediaPlayerFactory::mForceUseRTMediaPlayer==FORCE_RT_MEDIAPLAYER) {
        ALOGV("MediaPlayerFactory::forceUseMediaPlayer::FORCE_RT_MEDIAPLAYER...");
        return REALTEK_PLAYER;
    }
    else if(MediaPlayerFactory::mForceUseRTMediaPlayer==FORCE_ANDROID_MEDIAPLAYER) {
        ALOGV("MediaPlayerFactory::forceUseMediaPlayer::FORCE_DEFAULT_MEDIAPLAYER...");
        return getDefaultPlayerType();
    }
    return -1;
}
#endif // USE_RT_MEDIA_PLAYER

/*****************************************************************************
 *                                                                           *
 *                     Built-In Factory Implementations                      *
 *                                                                           *
 *****************************************************************************/

class NuPlayerFactory : public MediaPlayerFactory::IFactory {
  public:
    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const char* url,
                               float curScore) {
        static const float kOurScore = 0.8;
        char value[PROPERTY_VALUE_MAX];

        if (kOurScore <= curScore)
            return 0.0;
/*
        if (property_get("persist.media.USE_RTMediaPlayer", value, NULL)
                && (!strcmp("0", value) || !strcasecmp("false", value)))
        {
            if (property_get("persist.media.USE_NuPlayer", value, NULL)
                    && (!strcmp("1", value) || !strcasecmp("true", value)))
            {
                ALOGD("persist.media.USE_NuPlayer::true ~~~~~~ \n");
                return kOurScore;
            }
            else if (property_get("persist.media.USE_NuPlayer", value, NULL)
                    && (!strcmp("0", value) || !strcasecmp("false", value)))
            {
                ALOGD("persist.media.USE_NuPlayer::false ~~~~~~ \n");
                return 0.0;
            }
        }
*/
        if (!strncasecmp("http://", url, 7)
                || !strncasecmp("https://", url, 8)
                || !strncasecmp("file://", url, 7)) {
            size_t len = strlen(url);

            if (property_get("persist.media.USE_RTMediaPlayer", value, NULL) && (!strcmp("0", value) || !strcasecmp("false", value)))
            {
                if (len >= 5 && !strcasecmp(".m3u8", &url[len - 5])) {
                    //ALOGD("\033[0;32;32m%s:%d url=%s  kOurScore=%f\033[m\n", __FILE__, __LINE__, url, kOurScore);
                    return kOurScore;
                }

                //if (strstr(url,"m3u8")) {
                if (strstr(url,"m3u8") || strstr(url,"type=hls")) {
                    //ALOGD("\033[0;32;32m%s:%d url=%s  kOurScore=%f\033[m\n", __FILE__, __LINE__, url, kOurScore);
                    return kOurScore;
                }
            }
            else
            {
                if (strstr(url, "googlevideo.com") && (strstr(url,"m3u8") || strstr(url,"type=hls")))
                {
                    //ALOGD("\033[0;32;32m%s:%d url=%s  kOurScore=%f\033[m\n", __FILE__, __LINE__, url, kOurScore);
                    return kOurScore;
                }
            }

            if ((len >= 4 && !strcasecmp(".sdp", &url[len - 4])) || strstr(url, ".sdp?")) {
                return kOurScore;
            }

#ifdef HTTP_USE_NUPLAYER
            return kOurScore;
#endif
        }

        if (!strncasecmp("rtsp://", url, 7)) {
            return kOurScore;
        }

        return 0.0;
    }

    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               int /*fd*/,
                               int64_t /*offset*/,
                               int64_t /*length*/,
                               float /*curScore*/) {
/*
        static const float kOurScore = 0.8;
        char value[PROPERTY_VALUE_MAX];
        if (property_get("persist.media.USE_RTMediaPlayer", value, NULL)
                && (!strcmp("0", value) || !strcasecmp("false", value)))
        {
            if (property_get("persist.media.USE_NuPlayer", value, NULL)
                    && (!strcmp("1", value) || !strcasecmp("true", value)))
            {
                ALOGD("persist.media.USE_NuPlayer::true ~~~~~~ \n");
                return kOurScore;
            }
            else if (property_get("persist.media.USE_NuPlayer", value, NULL)
                    && (!strcmp("0", value) || !strcasecmp("false", value)))
            {
                ALOGD("persist.media.USE_NuPlayer::false ~~~~~~ \n");
                return 0.0;
            }
        }
*/
        return 0.0;
    }

    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const sp<IStreamSource>& /*source*/,
                               float /*curScore*/) {
        return 1.0;
    }

    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const sp<DataSource>& /*source*/,
                               float /*curScore*/) {
        // Only NuPlayer supports setting a DataSource source directly.
        return 1.0;
    }

    virtual sp<MediaPlayerBase> createPlayer(pid_t pid) {
        ALOGV(" create NuPlayer");
        return new NuPlayerDriver(pid);
    }
};

class TestPlayerFactory : public MediaPlayerFactory::IFactory {
  public:
    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const char* url,
                               float /*curScore*/) {
        if (TestPlayerStub::canBeUsed(url)) {
            return 1.0;
        }

        return 0.0;
    }

    virtual sp<MediaPlayerBase> createPlayer(pid_t /* pid */) {
        ALOGV("Create Test Player stub");
        return new TestPlayerStub();
    }
};

#ifdef USE_RT_MEDIA_PLAYER
class RTMediaPlayerFactory : public MediaPlayerFactory::IFactory {
  public:
    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const char* url,
                               float /*curScore*/) {
        char value[PROPERTY_VALUE_MAX];
        if (0 == strncmp(url, "rtsp://", 7) || 0 == strncmp(url, "rtp://", 6)) {
            if (property_get("persist.media.RTSP_USE_RTPLAYER", value, NULL)
                    && (!strcmp("1", value) || !strcasecmp("true", value)))
                return 1.1;
            else
                return 0.0;
        }
        if(strncmp(url, "mms://", 6) == 0 ||
            strncmp(url, "mmst://", 7) == 0 ||
            strncmp(url, "mmsth://", 7) == 0) {
            if (property_get("persist.media.MMS_USE_RTPLAYER", value, NULL)
                    && (!strcmp("1", value) || !strcasecmp("true", value)))
                return 1.1;
            else
                return 0.0;
        }
        if(strncmp(url, "rtmp://", 7) == 0 || strncmp(url, "udp://", 6) == 0) {
            if ((property_get("persist.media.USE_RTMediaPlayer", value, NULL)
                    && (!strcmp("1", value) || !strcasecmp("true", value))) || MediaPlayerFactory::mForceUseRTMediaPlayer == FORCE_BY_SCORE)
            {
                //ALOGD("\033[0;32;32m%s:%d url=%s  \033[m\n", __FILE__, __LINE__, url);
                return 1.1;
            }
            else
                return 0.0;
        }

        if (strncmp(url, "widevine://", 11) == 0)
        {
            ALOGD("\033[0;32m Detect widevine, don't used RTMediaPlayer %s:%d\033[m", __FUNCTION__, __LINE__);
            return 0.0;
        }

        if ((property_get("persist.media.USE_RTMediaPlayer", value, NULL)
            && (!strcmp("1", value) || !strcasecmp("true", value))) || MediaPlayerFactory::mForceUseRTMediaPlayer == FORCE_BY_SCORE)
        {
            size_t len = strlen(url);
            if (strncmp(url, "http://", 7) && strncmp(url, "https://", 8) && strcasecmp(".ogg", &url[len - 4]))
            {
                //ALOGD("\033[0;32;32m%s:%d url=%s  \033[m\n", __FILE__, __LINE__, url);
                return 1.1; // 1.1 > 1.0 (max value of Stagefright player)
            }
            else
            {
                if ((!strncmp(url, "http://", 7) || !strncmp(url, "https://", 8))  && !strstr(url, "youtube") && !strstr(url, "googlevideo.com") && strcasecmp(".mp3", &url[len - 4]))
                // workaround for youtube, switch to use RTMediaPlayer for demo, due to StagefrightPlayer need to fix later
                //if (!strncmp(url, "http://", 7))
                {
                    //ALOGD("\033[0;32;32m%s:%d url=%s  \033[m\n", __FILE__, __LINE__, url);
                    return 1.1;
                }
                //ALOGD("\033[0;32;32m%s:%d url=%s  \033[m\n", __FILE__, __LINE__, url);
                return 0.0;
            }
        }
        else
        {
            //ALOGD("\033[0;32;32m%s:%d url=%s  \033[m\n", __FILE__, __LINE__, url);
            return 0.0;
        }
    }

    bool switchToDvdPlayer()
    {
        bool res = false;
        char value[PROPERTY_VALUE_MAX];
        unsigned int forceDvdplayer = 0;
        unsigned int hdmiOutput = 1;
        unsigned int spdifOutput = 1;
        unsigned int agcMode = 0;

        if (property_get("rtk.audio.output", value, NULL) > 0) {
            forceDvdplayer = atoi(value);
            if(forceDvdplayer > 0)
                return true;
        }
/*
        if ((property_get("persist.media.USE_RTMediaPlayer", value, NULL)
          && (!strcmp("0", value) || !strcasecmp("false", value))) && MediaPlayerFactory::mForceUseRTMediaPlayer != FORCE_BY_SCORE) {
            ALOGE("[%s][%d]USE_RTMediaPlayer set false return.\n", __FUNCTION__, __LINE__);
            return false;
        }
*/
        if (property_get("rtk.hdmi.output", value, "1") > 0) {
            //0:RAW 1:2_CH 2:Multi_CH 3:Auto
            hdmiOutput = atoi(value);
            //ALOGD("HDMI output %d\n", hdmiOutput);
        }
        if (property_get("rtk.spdif.output", value, NULL) > 0) {
            spdifOutput = atoi(value);
            //ALOGD("SPDIF output %d\n", spdifOutput);
        }
        if (property_get("rtk.agc.mode", value, NULL) > 0) {
            agcMode = atoi(value);
            //ALOGD("AGC MODE %d\n", agcMOde);
        }
        if(hdmiOutput != 1 || spdifOutput == 0 || agcMode != 0)
            res = true;

        return res;
    }

    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               int fd,
                               int64_t offset,
                               int64_t /*length*/,
                               float /*curScore*/) {
        static const float kOurScore = 1.2;

        const int checkSize = 512;
        //ALOGD("RTMediaPlayer scoreFactory offset %lld", offset);;

        //check file header lager than 512 bytes
        //if(length < checkSize)
        //    return 0.0;

        char header[checkSize] = "";
        lseek(fd, offset, SEEK_SET);
        read(fd, header, sizeof(header));

        //if audio with following case , switch to RTMediaPlayer
        //sample rate >= 96khz(.ogg, .wav, .flac)
        //dts audio
        //aac adif
        //alac
        //adpcm
        //ac3
        if(memcmp(header, "OggS", 4) == 0) {
            //ALOGD("recognize Oggs\n");
            if(memcmp(&header[29], "vorbis", 6) == 0) {
                //we still use stagefright even when HDMI RAW out or SPDIF RAW out setting
                lseek(fd, offset, SEEK_SET);
                return 0.0;
            }
        }
        if(memcmp(header, "DSD", 3) == 0) {
            //ALOGD("recognize DSD\n");
            if(memcmp(&header[28], "fmt", 3) == 0) {
                //we still use stagefright even when HDMI RAW out or SPDIF RAW out setting
                lseek(fd, offset, SEEK_SET);
                return 0.0;
            }
        }

        if(switchToDvdPlayer()) {
            lseek(fd, offset, SEEK_SET);
            return kOurScore;
        }

        if((memcmp(header, "RIFF", 4) == 0) && (memcmp(&header[8], "WAVE", 4) == 0)) {
            int compress = 0;
            memcpy(&compress, &header[20], 2);
            //ALOGD("wav audio compress %x\n", compress);
            //android only support WAV FORMAT PCM, ALAW, MULAW, MSGSM, EXTENSIBLE
            if(compress != 0X0001 && compress != 0x0006 && compress != 0x0007 &&
                compress != 0x0031 && compress != 0xFFFE) {
                lseek(fd, offset, SEEK_SET);
                return kOurScore;
            }

            //ALOGD("recognize RIFF / WAVE\n");
            char dataBuf[DTS_PARSER_SIZE] = "";
            char swapBuf[DTS_PARSER_SIZE] = "";
            unsigned int sampleRate = 0;

            //skip 44 bytes
            //lseek(fd, offset, SEEK_SET);
            lseek(fd, offset + 44, SEEK_SET);
            read(fd, dataBuf, DTS_PARSER_SIZE);

            //two byte swap
            for(int i = 0; i < (DTS_PARSER_SIZE/2); i+=2) {
                swapBuf[i] = dataBuf[i+1];
                swapBuf[i+1] = dataBuf[i];
            }

            if(check_audio_bitstream(swapBuf, DTS_PARSER_SIZE, &sampleRate)) {
                //ALOGD("DTS stream sampleRate %d\n", sampleRate);
                lseek(fd, offset, SEEK_SET);
                return kOurScore;
            }
        } else if(memcmp("ADIF", header, 4) == 0) {
            //ALOGD("recognize ADIF\n");
            lseek(fd, offset, SEEK_SET);
            return kOurScore;
        } else if(strstr(header, "#EXTM3U")) {
            //ALOGD("recognize m3u8\n");
            lseek(fd, offset, SEEK_SET);
            return kOurScore;
        } else {
            char fd_buff[256] = "";
            char link_buff[4096] = "";
            sprintf(fd_buff, "/proc/self/fd/%d", fd);
            ssize_t len = readlink(fd_buff, link_buff, sizeof(link_buff) - 1);
            link_buff[len] = '\0';

            // check the codec is RV format
            if ((!strncasecmp(link_buff + strlen(link_buff) - strlen(".rmvb"), ".rmvb", strlen(".rmvb"))) ||
              (!strncasecmp(link_buff + strlen(link_buff) - strlen(".rv"), ".rv", strlen(".rv")))) {
                lseek(fd, offset, SEEK_SET);
                //ALOGD("\033[0;32m%s:%d   Is RV or RMVB....\033[m\n", __FILE__, __LINE__);
                return kOurScore;
            }

            //check .flac with mkv container
            if(!strncasecmp(link_buff + strlen(link_buff) - strlen(".flac"), ".flac", strlen(".flac"))) {
                for(int i = 0; i <= (checkSize - 8); i++) {
                    if(memcmp(&header[i], "matroska", 8) == 0) {
                        //ALOGD("recognize matroska\n");
                        lseek(fd, offset, SEEK_SET);
                        return kOurScore;
                    }
                }
            }

            static const char* const FILE_EXTS[] = { ".wav",
                                                     ".mp3",
                                                     ".m4a",
                                                     ".alac" };

            // check .wav and .mp3 first, then check .m4a and .alac later
            int lenURL = strlen(link_buff);
            for(int i = 0; i < (NELEM(FILE_EXTS) - 2); ++i) {
                int len2 = strlen(FILE_EXTS[i]);
                int start = lenURL - len2;
                if(start > 0) {
                    if(!strncasecmp(link_buff + start, FILE_EXTS[i], len2)) {
                        char dataBuf[DTS_PARSER_SIZE] = "";
                        int dataSize = 0;
                        unsigned int sampleRate = 0;
                        lseek(fd, offset, SEEK_SET);
                        dataSize = read(fd, dataBuf, DTS_PARSER_SIZE);
                        //check dts raw audio without wav header
                        if(check_audio_bitstream(dataBuf, dataSize, &sampleRate)) {
                            //ALOGD("DTS stream in .wav/.mp3");
                            lseek(fd, offset, SEEK_SET);
                            return kOurScore;
                        }
                    }
                }
            }
            //check the codec is ALAC format
            //check first 512bytes
            for(int i = 0; i <= (checkSize - 4); i++){
                if(memcmp(&header[i], "alac", 4) == 0) {
                    //ALOGD("recognize ALAC %d\n", i);
                    lseek(fd, offset, SEEK_SET);
                    return kOurScore;
                }
            }
            //to parser header in .m4a/.alac file
            // m4a and alac starts on FILE_EXTS[2]
            for(int i = 2; i < NELEM(FILE_EXTS) ; ++i) {
                int len2 = strlen(FILE_EXTS[i]);
                int start = lenURL - len2;
                if(start > 0) {
                    if(!strncasecmp(link_buff + start, FILE_EXTS[i], len2)) {
                        bool flag_get_moov = 0;
                        long length = 0;           //atom_size
                        const int header_size = 8; //header_size = 4Bytes(atom_szie) + 4Bytes(atom_type)
                        unsigned int offset_header = 0;
                        unsigned int offset_alac = 0;
                        int ret = 1;
                        while(1) {
                        //to find atom_type = "moov"
                            if(flag_get_moov == 0) {
                                if(memcmp(&header[offset_header+4], "moov", 4) == 0) {
                                    flag_get_moov = 1;
                                    length = ((header[offset_header+0]<<24) | (header[offset_header+1]<<16)
                                                | (header[offset_header+2]<<8) | (header[offset_header+3]));
                                } else {
                                    length = ((header[offset_header+0]<<24) | (header[offset_header+1]<<16)
                                                | (header[offset_header+2]<<8) | (header[offset_header+3]));
                                    //do NOT read again if next atom_type inside checkSize
                                    if((offset_header + length) < (checkSize - header_size)) {
                                        offset_header += length;
                                    } else {
                                        offset_alac += offset_header + length;
                                        offset_header = 0;
                                        lseek(fd, offset + offset_alac, SEEK_SET);
                                        ret = read(fd, header, sizeof(header));
                                    }
                                }
                                // EOF or length = 0 to stop
                                if(length == 0 || ret <= 0 ) {
                                    ALOGD("Read alac/ma4 fail#1 offset_alac=%d \n",offset_alac);
                                    break;
                                }
                            } else {
                                //parser atom_moov data to find alac
                                for(int i = 0; i <= (checkSize - 4); i++) {
                                    if(memcmp(&header[i], "alac", 4) == 0) {
                                        //ALOGD("recognize ALAC %d\n", i);
                                        lseek(fd, offset, SEEK_SET);
                                        return kOurScore;
                                    }
                                }
                                //avoid the string in the boundary to read the last 3 Bytes again
                                lseek(fd, -3 , SEEK_CUR );
                                ret = read(fd, header, sizeof(header));
                                //EOF or out of "moov" data
                                if(ret <= 0 || length < 0) {
                                    //ALOGD("Read alac/ma4 fail#2 offset_alac=%d \n",offset_alac);
                                    break;
                                }
                                length -= (checkSize - 3);
                            }
                        }
                    }
                }
            }
        }

        lseek(fd, offset, SEEK_SET);
        return 0.0;
    }

    virtual sp<MediaPlayerBase> createPlayer(pid_t /*pid*/) {
        ALOGV("Create RTMediaPlayer");
        return new RTMediaPlayer();
    }
};

class RTDtvMediaPlayerFactory : public MediaPlayerFactory::IFactory {
  public:
    virtual float scoreFactory(const sp<IMediaPlayer>& /*client*/,
                               const char* url,
                               float /*curScore*/) {
        if (!strncmp(url, "stb://0", 7))
            return 2.0;
        else
            return 0.0;
    }

    virtual sp<MediaPlayerBase> createPlayer(pid_t /*pid*/) {
        ALOGV("Create RTDtvMediaPlayer");
        return new RTDtvMediaPlayer();
    }
};
#endif // USE_RT_MEDIA_PLAYER

void MediaPlayerFactory::registerBuiltinFactories() {
    Mutex::Autolock lock_(&sLock);

    if (sInitComplete)
        return;

    registerFactory_l(new NuPlayerFactory(), NU_PLAYER);
    registerFactory_l(new TestPlayerFactory(), TEST_PLAYER);
#ifdef USE_RT_MEDIA_PLAYER
    registerFactory_l(new RTMediaPlayerFactory(), REALTEK_PLAYER);
    registerFactory_l(new RTDtvMediaPlayerFactory(), REALTEK_DTV_PLAYER);
#endif // USE_RT_MEDIA_PLAYER
    sInitComplete = true;
}

}  // namespace android
