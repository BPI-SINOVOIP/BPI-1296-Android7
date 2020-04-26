/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <inttypes.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

//#define LOG_NDEBUG 0
#define LOG_TAG "stagefright"
#include <media/stagefright/foundation/ADebug.h>

#include "jpeg.h"
#include "SineSource.h"

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/IMediaHTTPService.h>
#include <media/IMediaPlayerService.h>
#include <media/stagefright/foundation/ALooper.h>
#include "include/NuCachedSource2.h"
#include <media/stagefright/AudioPlayer.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/JPEGSource.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/MediaErrors.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/stagefright/MediaSource.h>
#include <media/stagefright/MetaData.h>
#include <media/stagefright/OMXClient.h>
#include <media/stagefright/OMXCodec.h>
#include <media/mediametadataretriever.h>

#include <media/stagefright/foundation/hexdump.h>
#include <media/stagefright/MPEG2TSWriter.h>
#include <media/stagefright/MPEG4Writer.h>

#include <private/media/VideoFrame.h>

#include <gui/GLConsumer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

using namespace android;
static bool gPlaybackAudio;

static const char *filename[2] = {NULL, NULL};
static int inputPinMap = 0;
static int substreamID = 0;
static int outmode = 21;

static sp<ANativeWindow> gSurface;

static int64_t getNowUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (int64_t)tv.tv_usec + tv.tv_sec * 1000000ll;
}

static void playSource(OMXClient *client, sp<MediaSource> &source, int fileType) {
    sp<MetaData> meta = source->getFormat();

    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    sp<MediaSource> rawSource;
    if (!strcasecmp(MEDIA_MIMETYPE_AUDIO_RAW, mime)) {
        rawSource = source;
    } else {
        int flags = 0;
        rawSource = OMXCodec::Create(
            client->interface(), meta, false /* createEncoder */, source,
            NULL /* matchComponentName */,
            flags,
            gSurface);

        if (rawSource == NULL) {
            fprintf(stderr, "Failed to instantiate decoder for '%s'.\n", mime);
            return;
        }
    }

    source.clear();

    {
        status_t err1;
        printf("[play source]parameter setting start\n");
        OMX_AUDIO_PARAM_MS12CFG *cfg = NULL;
        cfg = (OMX_AUDIO_PARAM_MS12CFG *)malloc(sizeof(OMX_AUDIO_PARAM_MS12CFG));
        if(cfg == NULL)
            printf("MS12 param struct malloc fail!\n");
        else
            printf("Get MS12 param struct, size %d\n",(int)sizeof(OMX_AUDIO_PARAM_MS12CFG));

        memset(cfg,0, sizeof(OMX_AUDIO_PARAM_MS12CFG));

        cfg->param_struct.ms_runtime_args.ddplus_associated_substream = substreamID;
        cfg->param_struct.ms_runtime_args.ddplus_outmode = outmode;
        cfg->inputPinType = (fileType == 0)? OMX_AUDIO_MS12_INPUT_MAIN: OMX_AUDIO_MS12_INPUT_ASSOC;
        cfg->inputPinMap  = inputPinMap;
        printf("[inputPinType] %d [inputPinMap] %d streaID %d outmode %d\n",fileType, inputPinMap, substreamID,outmode);
        err1 = rawSource->setMS12FlowConfig((void*)cfg);

        printf("[play source]parameter settting end\n");
        if(err1 == ERROR_UNSUPPORTED)
            printf("[play source]parameter ,ERROR_UNSUPPORTED \n");

        if(cfg) free(cfg);
    }

    status_t err = rawSource->start();

    if (err != OK) {
        fprintf(stderr, "rawSource returned error %d (0x%08x)\n", err, err);
        return;
    }

    if (gPlaybackAudio) {
        AudioPlayer *player = new AudioPlayer(NULL);
        player->setSource(rawSource);
        rawSource.clear();

        player->start(true /* sourceAlreadyStarted */);

        status_t finalStatus;
        while (!player->reachedEOS(&finalStatus)) {
            usleep(100000ll);
        }

        delete player;
        player = NULL;

        return;
    }

    int n = 0;
    int64_t startTime = getNowUs();

    long numIterationsLeft = 1;
    MediaSource::ReadOptions options;

    int64_t sumDecodeUs = 0;
    int64_t totalBytes = 0;

    Vector<int64_t> decodeTimesUs;

    while (numIterationsLeft-- > 0) {
        long numFrames = 0;

        MediaBuffer *buffer;

        for (;;) {
            int64_t startDecodeUs = getNowUs();
            status_t err = rawSource->read(&buffer, &options);
            int64_t delayDecodeUs = getNowUs() - startDecodeUs;

            options.clearSeekTo();

            if (err != OK) {
                CHECK(buffer == NULL);

                if (err == INFO_FORMAT_CHANGED) {
                    printf("format changed.\n");
                    continue;
                }

                break;
            }

            if (buffer->range_length() > 0) {

                if ((n++ % 16) == 0) {
                    printf(".");
                    fflush(stdout);
                }
            }

            sumDecodeUs += delayDecodeUs;
            totalBytes += buffer->range_length();

            buffer->release();
            buffer = NULL;

            ++numFrames;
        }

        printf("$");
        fflush(stdout);

        options.setSeekTo(0);
    }

    rawSource->stop();
    printf("\n");

    int64_t delay = getNowUs() - startTime;
    if (!strncasecmp("video/", mime, 6)) {
        printf("avg. %.2f fps\n", n * 1E6 / delay);

        printf("avg. time to decode one buffer %.2f usecs\n",
               (double)sumDecodeUs / n);

        printf("decoded a total of %d frame(s).\n", n);

    } else if (!strncasecmp("audio/", mime, 6)) {
        // Frame count makes less sense for audio, as the output buffer
        // sizes may be different across decoders.
        printf("avg. %.2f KB/sec\n", totalBytes / 1024 * 1E6 / delay);

        printf("decoded a total of %" PRId64 " bytes\n", totalBytes);
    }
}

////////////////////////////////////////////////////////////////////////////////

struct DetectSyncSource : public MediaSource {
    DetectSyncSource(const sp<MediaSource> &source);

    virtual status_t start(MetaData *params = NULL);
    virtual status_t stop();
    virtual sp<MetaData> getFormat();

    virtual status_t read(
            MediaBuffer **buffer, const ReadOptions *options);

private:
    enum StreamType {
        AVC,
        MPEG4,
        H263,
        OTHER,
    };

    sp<MediaSource> mSource;
    StreamType mStreamType;
    bool mSawFirstIDRFrame;

    DISALLOW_EVIL_CONSTRUCTORS(DetectSyncSource);
};

DetectSyncSource::DetectSyncSource(const sp<MediaSource> &source)
    : mSource(source),
      mStreamType(OTHER),
      mSawFirstIDRFrame(false) {
    const char *mime;
    CHECK(mSource->getFormat()->findCString(kKeyMIMEType, &mime));

    if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_AVC)) {
        mStreamType = AVC;
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_MPEG4)) {
        mStreamType = MPEG4;
        CHECK(!"sync frame detection not implemented yet for MPEG4");
    } else if (!strcasecmp(mime, MEDIA_MIMETYPE_VIDEO_H263)) {
        mStreamType = H263;
        CHECK(!"sync frame detection not implemented yet for H.263");
    }
}

status_t DetectSyncSource::start(MetaData *params) {
    mSawFirstIDRFrame = false;

    return mSource->start(params);
}

status_t DetectSyncSource::stop() {
    return mSource->stop();
}

sp<MetaData> DetectSyncSource::getFormat() {
    return mSource->getFormat();
}

static bool isIDRFrame(MediaBuffer *buffer) {
    const uint8_t *data =
        (const uint8_t *)buffer->data() + buffer->range_offset();
    size_t size = buffer->range_length();
    for (size_t i = 0; i + 3 < size; ++i) {
        if (!memcmp("\x00\x00\x01", &data[i], 3)) {
            uint8_t nalType = data[i + 3] & 0x1f;
            if (nalType == 5) {
                return true;
            }
        }
    }

    return false;
}

status_t DetectSyncSource::read(
        MediaBuffer **buffer, const ReadOptions *options) {
    for (;;) {
        status_t err = mSource->read(buffer, options);

        if (err != OK) {
            return err;
        }

        if (mStreamType == AVC) {
            bool isIDR = isIDRFrame(*buffer);
            (*buffer)->meta_data()->setInt32(kKeyIsSyncFrame, isIDR);
            if (isIDR) {
                mSawFirstIDRFrame = true;
            }
        } else {
            (*buffer)->meta_data()->setInt32(kKeyIsSyncFrame, true);
        }

        if (mStreamType != AVC || mSawFirstIDRFrame) {
            break;
        }

        // Ignore everything up to the first IDR frame.
        (*buffer)->release();
        *buffer = NULL;
    }

    return OK;
}

////////////////////////////////////////////////////////////////////////////////

static void usage(const char *me) {
    fprintf(stderr, "usage: %s [options] [input_filename]\n", me);
    fprintf(stderr, "       -h(elp)\n");
    fprintf(stderr, "       -a(udio)\n");
    fprintf(stderr, "       -n substreamId\n");
    fprintf(stderr, "       -m outmode(default 21)\n");
    fprintf(stderr, "       -o playback audio\n");
}



static void* decode_thread(void* pIdx)
{

    int idx = *(int*)pIdx;

    DataSource::RegisterDefaultSniffers();

    OMXClient client;
    status_t err = client.connect();

    if(err)
        printf("[udcplayer] client.conenct() fails\n");

    bool syncInfoPresent = true;

    sp<DataSource> dataSource =
        DataSource::CreateFromURI(NULL /* httpService */, filename[idx]);

    if (strncasecmp(filename[idx], "sine:", 5) && dataSource == NULL) {
        fprintf(stderr, "Unable to create data source.\n");
        exit(1);
    }

    printf("udcplayer playfile: %s\n",filename[idx]);
    Vector<sp<MediaSource> > mediaSources;
    sp<MediaSource> mediaSource;

    {
        sp<MediaExtractor> extractor = MediaExtractor::Create(dataSource);

        if (extractor == NULL) {
            fprintf(stderr, "could not create extractor.\n");
            return 0;
        }

        sp<MetaData> meta = extractor->getMetaData();

        if (meta != NULL) {
            const char *mime;
            CHECK(meta->findCString(kKeyMIMEType, &mime));

            if (!strcasecmp(mime, MEDIA_MIMETYPE_CONTAINER_MPEG2TS)) {
                syncInfoPresent = false;
            }
        }

        size_t numTracks = extractor->countTracks();
        {
            sp<MetaData> meta;
            size_t i;
            for (i = 0; i < numTracks; ++i) {
                meta = extractor->getTrackMetaData(
                        i, MediaExtractor::kIncludeExtensiveMetaData);

                const char *mime;
                meta->findCString(kKeyMIMEType, &mime);

                if (!strncasecmp(mime, "audio/", 6)) {
                    break;
                }

                meta = NULL;
            }

            if (meta == NULL) {
                fprintf(stderr,
                        "No suitable %s track found. The '-a' option will "
                        "target audio tracks only, the default is to target "
                        "video tracks only.\n",
                        "audio");
                return 0;
            }

            int64_t thumbTimeUs;
            if (meta->findInt64(kKeyThumbnailTime, &thumbTimeUs)) {
                printf("thumbnailTime: %" PRId64 " us (%.2f secs)\n",
                       thumbTimeUs, thumbTimeUs / 1E6);
            }

            mediaSource = extractor->getTrack(i);
        }

    }

    playSource(&client, mediaSource,idx);


    client.disconnect();
    return 0;
}

int main(int argc, char **argv) {
    android::ProcessState::self()->startThreadPool();

    void *ret1, *ret2;
    pthread_t thread_main, thread_assoc;
    int in_main, in_assco;
    bool audioOnly = false;
    gPlaybackAudio = false;
    substreamID = 0;

    sp<ALooper> looper;

    int res;
    while ((res = getopt(argc, argv, "han:lm:b:ptsrow:kxSTd:D:")) >= 0) {
        switch (res) {
            case 'a':
            {
                audioOnly = true;
                break;
            }

            case 'o':
            {
                gPlaybackAudio = true;
                break;
            }
            case 'n':
            {
                char *end;
                int x = strtol(optarg, &end, 10);
                if (*end != '\0' || end == optarg || x <= 0) {
                    x = 1;
                }
                substreamID = x;
                break;
            }
            case 'm':
            {
                char *end;
                int x = strtol(optarg, &end, 10);
                if (*end != '\0' || end == optarg || x <= 0) {
                    x = 1;
                }
                outmode = x;
                break;
            }
            case '?':
            case 'h':
            default:
            {
                usage(argv[0]);
                exit(1);
                break;
            }
        }
    }

    if (gPlaybackAudio && !audioOnly) {
        // This doesn't make any sense if we're decoding the video track.
        gPlaybackAudio = false;
    }

    argc -= optind;
    argv += optind;


    for (int i = 0, k = 0; k < argc && i < 2; ++k, ++i) {
        filename[i] = argv[k];
        inputPinMap |= 1<<i;
    }

    if (filename[0] != NULL)
    {
        in_main = 0;
        pthread_create(&thread_main, NULL , decode_thread , (void*) &in_main);
    }


    if (filename[1] != NULL)
    {
        in_assco = 1;
        pthread_create(&thread_assoc, NULL , decode_thread , (void*) &in_assco);
    }

    if (filename[1] != NULL)
        pthread_join( thread_assoc, &ret2);

    if (filename[0] != NULL)
        pthread_join( thread_main, &ret1);


    return 0;
}
