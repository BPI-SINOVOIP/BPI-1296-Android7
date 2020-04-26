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

//#define LOG_NDEBUG 0
#define LOG_TAG "stagefright"
#include <media/stagefright/foundation/ADebug.h>

#include <sys/time.h>

#include <stdlib.h>
#include <string.h>


#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/IMediaPlayerService.h>
#include <media/IMediaHTTPService.h>
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

#include <fcntl.h>

#include <gui/GLConsumer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>

#ifdef MS12_FLOW_EN
#include "ms12/ms12_cmd.h"
#include "ms12/ms12_getparam.h"
//#include "ms12/ms12_err.h"
#endif

using namespace android;

#ifdef MS12_FLOW_EN
static long gNumRepetitions;
static long gMaxNumFrames;  // 0 means decode all available.
static long gReproduceBug;  // if not -1.
static bool gPreferSoftwareCodec;
static bool gForceToUseHardwareCodec;
static bool gPlaybackAudio;
static bool gWriteMP4;
static bool gDisplayHistogram;
static String8 gWriteMP4Filename;
static bool useMS12Flow;

//originally locally declare
static bool audioOnly;
static bool listComponents;
static bool dumpProfiles;
static bool extractThumbnail;
static bool seekTest;
static bool useSurfaceAlloc;
static bool useSurfaceTexAlloc;
static bool dumpStream;
static bool dumpPCMStream;
static String8 dumpStreamFilename;

static sp<ANativeWindow> gSurface;

static int64_t getNowUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (int64_t)tv.tv_usec + tv.tv_sec * 1000000ll;
}

static int CompareIncreasing(const int64_t *a, const int64_t *b) {
    return (*a) < (*b) ? -1 : (*a) > (*b) ? 1 : 0;
}

static void displayDecodeHistogram(Vector<int64_t> *decodeTimesUs) {
    printf("decode times:\n");

    decodeTimesUs->sort(CompareIncreasing);

    size_t n = decodeTimesUs->size();
    int64_t minUs = decodeTimesUs->itemAt(0);
    int64_t maxUs = decodeTimesUs->itemAt(n - 1);

    printf("min decode time %lld us (%.2f secs)\n", minUs, minUs / 1E6);
    printf("max decode time %lld us (%.2f secs)\n", maxUs, maxUs / 1E6);

    size_t counts[100];
    for (size_t i = 0; i < 100; ++i) {
        counts[i] = 0;
    }

    for (size_t i = 0; i < n; ++i) {
        int64_t x = decodeTimesUs->itemAt(i);

        size_t slot = ((x - minUs) * 100) / (maxUs - minUs);
        if (slot == 100) { slot = 99; }

        ++counts[slot];
    }

    for (size_t i = 0; i < 100; ++i) {
        int64_t slotUs = minUs + (i * (maxUs - minUs) / 100);

        double fps = 1E6 / slotUs;
        printf("[%.2f fps]: %d\n", fps, counts[i]);
    }
}

static void dumpSource(const sp<MediaSource> &source, const String8 &filename)
{
    //FILE *out = fopen(filename.string(), "wb");
    FILE *out = fopen(filename.string(), "wb");

    CHECK_EQ((status_t)OK, source->start());

    status_t err;
    for (;;) {
        MediaBuffer *mbuf;
        err = source->read(&mbuf);

        if (err == INFO_FORMAT_CHANGED) {
            continue;
        } else if (err != OK) {
            break;
        }

        CHECK_EQ(
                fwrite((const uint8_t *)mbuf->data() + mbuf->range_offset(),
                       1,
                       mbuf->range_length(),
                       out),
                (ssize_t)mbuf->range_length());

        mbuf->release();
        mbuf = NULL;
    }

    CHECK_EQ((status_t)OK, source->stop());

    fclose(out);
    out = NULL;
}

//#ifdef MS12_FLOW_EN
static void ms12_get_outputfile_info(MS_INIT_PARAMS *p_args, OMX_AUDIO_PARAM_MS12CFG *cfg)
{
    int i, len;

    memset(&cfg->output_file_name[0][0],0,(OMX_AUDIO_MS_MAX_NUM_OUTPUTS*OMX_AUDIO_MS_MAX_OUTPUT_NAME_LEN)*sizeof(char));

    for(i = 0; i < OMX_AUDIO_MS_MAX_NUM_OUTPUTS; i++)
        cfg->output_name_len[i] = 0;

    if(p_args->bitstream_output_filename[MS_DD_OUT_IDX])
    {
        //memset(&cfg->output_file_name[OMX_AUDIO_MS_DD_OUT_IDX][0],0,OMX_AUDIO_MS_MAX_OUTPUT_NAME_LEN*sizeof(char));
        len = cfg->output_name_len[OMX_AUDIO_MS_DD_OUT_IDX] = strlen(p_args->bitstream_output_filename[MS_DD_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DD_OUT_IDX][0],p_args->bitstream_output_filename[MS_DD_OUT_IDX],len);
    }

    if(p_args->bitstream_output_filename[MS_DDP_OUT_IDX])
    {
       //memset(&cfg->output_file_name[OMX_AUDIO_MS_DDP_OUT_IDX][0],0,256);
       len = cfg->output_name_len[OMX_AUDIO_MS_DDP_OUT_IDX] = strlen(p_args->bitstream_output_filename[MS_DDP_OUT_IDX]);
       memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DDP_OUT_IDX][0],p_args->bitstream_output_filename[MS_DDP_OUT_IDX],len);
    }

    if(p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DOWNMIX_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DOWNMIX_OUT_IDX][0],p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX],len);
    }

    if(p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_VIRTUALIZER_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_VIRTUALIZER_OUT_IDX],p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX],len);
    }

    if(p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DAP_HEADPHONE_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DAP_HEADPHONE_OUT_IDX],p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX],len);
    }

    if(p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DAP_SPEAKER_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DAP_SPEAKER_OUT_IDX],p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX],len);
    }

    if(p_args->pcmr_output_filename)
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_PCMR_OUT_IDX] = strlen(p_args->pcmr_output_filename);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_PCMR_OUT_IDX],p_args->pcmr_output_filename,len);
    }

}

static void playSource(OMXClient *client, sp<MediaSource> &source, process_graph_t *process_graph)
{
    sp<MetaData> meta = source->getFormat();

    const char *mime;
    CHECK(meta->findCString(kKeyMIMEType, &mime));

    sp<MediaSource> rawSource;
    if (!strcasecmp(MEDIA_MIMETYPE_AUDIO_RAW, mime))
    {
        rawSource = source;
    }
    else
    {
        int flags = 0;

        if (gPreferSoftwareCodec)
        {
            flags |= OMXCodec::kPreferSoftwareCodecs;
        }
        if (gForceToUseHardwareCodec)
        {
            CHECK(!gPreferSoftwareCodec);
            flags |= OMXCodec::kHardwareCodecsOnly;
        }
        rawSource = OMXCodec::Create(
            client->interface(), meta, false /* createEncoder */, source,
            NULL /* matchComponentName */,
            flags,
            gSurface);

        if (rawSource == NULL)
        {
            fprintf(stderr, "Failed to instantiate decoder for '%s'.\n", mime);
            return;
        }
    }

    source.clear();
    useMS12Flow = true;

    if(useMS12Flow)
    {
        status_t err1;
        printf("[play source]ms12 flow start\n");
        OMX_AUDIO_PARAM_MS12CFG *cfg = NULL;
        cfg = (OMX_AUDIO_PARAM_MS12CFG *)malloc(sizeof(OMX_AUDIO_PARAM_MS12CFG));
        if(cfg == NULL)
            printf("MS12 param struct malloc fail!\n");
        else
            printf("Get MS12 param struct %x, size %d\n",(unsigned int)cfg,sizeof(OMX_AUDIO_PARAM_MS12CFG));

        memcpy(&cfg->param_struct.ms_runtime_args,&process_graph->user_params.ms_runtime_args,sizeof(OMX_AUDIO_MS_RUNTIME_PARAMS));
        memcpy(&cfg->param_struct.ms_init_args,&process_graph->user_params.ms_init_args,sizeof(OMX_AUDIO_MS_INIT_PARAMS));
        memcpy(&cfg->param_struct.use_case,&process_graph->use_case,sizeof(OMX_AUDIO_MS_USE_CASE));
        ms12_get_outputfile_info((MS_INIT_PARAMS *)&process_graph->user_params.ms_init_args , cfg);

        //FIXME!!!
        cfg->inputPinType = OMX_AUDIO_MS12_INPUT_MAIN;
        cfg->isListeningTest = OMX_FALSE;
        cfg->inputPinMap = 1;
        err1 = rawSource->setMS12FlowConfig((uint32_t)cfg);

        printf("[play source]ms12 flow end\n");
        if(err1 == ERROR_UNSUPPORTED)
            printf("[play source]ms12 flow end,ERROR_UNSUPPORTED \n");

        if(cfg) free(cfg);
    }

    status_t err = rawSource->start();//rawSource ->OMXCodec

    if (err != OK) {
        fprintf(stderr, "rawSource returned error %d (0x%08x)\n", err, err);
        return;
    }

    if (gPlaybackAudio)
    {
        AudioPlayer *player = new AudioPlayer(NULL);
        player->setSource(rawSource);
        rawSource.clear();

        player->start(true /* sourceAlreadyStarted */);
printf("[YLLIN][after player->start]\n");
        status_t finalStatus;
        while (!player->reachedEOS(&finalStatus))
        {
            usleep(100000ll);
        }

        delete player;
        player = NULL;

        return;
    }
    else if (gReproduceBug >= 3 && gReproduceBug <= 5)
    {
        int64_t durationUs;
        CHECK(meta->findInt64(kKeyDuration, &durationUs));

        status_t err;
        MediaBuffer *buffer;
        MediaSource::ReadOptions options;
        int64_t seekTimeUs = -1;
        for (;;)
        {
            err = rawSource->read(&buffer, &options);
            options.clearSeekTo();

            bool shouldSeek = false;
            if (err == INFO_FORMAT_CHANGED)
            {
                CHECK(buffer == NULL);

                printf("format changed.\n");
                continue;
            }
            else if (err != OK)
            {
                printf("reached EOF.\n");

                shouldSeek = true;
            }
            else
            {
                int64_t timestampUs;
                CHECK(buffer->meta_data()->findInt64(kKeyTime, &timestampUs));

                bool failed = false;

                if (seekTimeUs >= 0) {
                    int64_t diff = timestampUs - seekTimeUs;

                    if (diff < 0) {
                        diff = -diff;
                    }

                    if ((gReproduceBug == 4 && diff > 500000)
                        || (gReproduceBug == 5 && timestampUs < 0)) {
                        printf("wanted: %.2f secs, got: %.2f secs\n",
                               seekTimeUs / 1E6, timestampUs / 1E6);

                        printf("ERROR: ");
                        failed = true;
                    }
                }

                printf("buffer has timestamp %lld us (%.2f secs)\n",
                       timestampUs, timestampUs / 1E6);

                buffer->release();
                buffer = NULL;

                if (failed) {
                    break;
                }

                shouldSeek = ((double)rand() / RAND_MAX) < 0.1;

                if (gReproduceBug == 3) {
                    shouldSeek = false;
                }
            }

            seekTimeUs = -1;

            if (shouldSeek) {
                seekTimeUs = (rand() * (float)durationUs) / RAND_MAX;
                options.setSeekTo(seekTimeUs);

                printf("seeking to %lld us (%.2f secs)\n",
                       seekTimeUs, seekTimeUs / 1E6);
            }
        }

        rawSource->stop();

        return;
    }

    int n = 0;
    int64_t startTime = getNowUs();

    long numIterationsLeft = gNumRepetitions;
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
                if (gDisplayHistogram && n > 0) {
                    // Ignore the first time since it includes some setup
                    // cost.
                    decodeTimesUs.push(delayDecodeUs);
                }

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
            if (gMaxNumFrames > 0 && numFrames == gMaxNumFrames) {
                break;
            }

            if (gReproduceBug == 1 && numFrames == 40) {
                printf("seeking past the end now.");
                options.setSeekTo(0x7fffffffL);
            } else if (gReproduceBug == 2 && numFrames == 40) {
                printf("seeking to 5 secs.");
                options.setSeekTo(5000000);
            }
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

        if (gDisplayHistogram) {
            displayDecodeHistogram(&decodeTimesUs);
        }
    } else if (!strncasecmp("audio/", mime, 6)) {
        // Frame count makes less sense for audio, as the output buffer
        // sizes may be different across decoders.
        printf("avg. %.2f KB/sec\n", totalBytes / 1024 * 1E6 / delay);

        printf("decoded a total of %lld bytes\n", totalBytes);
    }
}
//#endif
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

static void writeSourcesToMP4(
        Vector<sp<MediaSource> > &sources, bool syncInfoPresent) {
#if 0
    sp<MPEG4Writer> writer =
        new MPEG4Writer(gWriteMP4Filename.string());
#else
    sp<MPEG2TSWriter> writer =
        new MPEG2TSWriter(gWriteMP4Filename.string());
#endif

    // at most one minute.
    writer->setMaxFileDuration(60000000ll);

    for (size_t i = 0; i < sources.size(); ++i) {
        sp<MediaSource> source = sources.editItemAt(i);

        CHECK_EQ(writer->addSource(
                    syncInfoPresent ? source : new DetectSyncSource(source)),
                (status_t)OK);
    }

    sp<MetaData> params = new MetaData;
    params->setInt32(kKeyRealTimeRecording, false);
    CHECK_EQ(writer->start(params.get()), (status_t)OK);

    while (!writer->reachedEOS()) {
        usleep(100000);
    }
    writer->stop();
}

//#ifdef MS12_FLOW_EN
int ms12_process_command(int argc, const char **argv, process_graph_t *process_graph)
{
    int param_error_code = 0;
    int msd_error_returned = 0;
    unsigned long int extern_static_size;
    void *p_extern_static;

    MS_RETVAL retval;
    /* Initialize the parameter struct */
    ms_init_params((MS_PARAMS *)&process_graph->user_params);

    /* Allocate and open the command line parser */
    dlb_getparam_mem_query(&extern_static_size);
    printf("[extern static size %d]\n",(int)extern_static_size);
    p_extern_static = malloc(extern_static_size);
    if(p_extern_static == 0){
        return MS_OUT_OF_MEMORY;
    }
    process_graph->user_params.h_getparam = dlb_getparam_open(p_extern_static);
    if(!process_graph->user_params.h_getparam){
        free(p_extern_static);
        return MS_GENERAL_ERROR;
    }

    /* Parse command line, detect use case, validate arguments */
    printf("[ms_parse_command_line] start\n");
    retval = ms_parse_command_line(argc, argv, &process_graph->use_case, &process_graph->user_params, &param_error_code);
    printf("[ms_parse_command_line] end\n");
    msd_error_returned = ms_handle_parser_retval(retval, param_error_code);

    if (msd_error_returned)
        return msd_error_returned;

    /* Print parameter settings */
    if(process_graph->user_params.ms_init_args.verbose_mode > 1){
        ms_print_parameter_settings(&process_graph->use_case, &process_graph->user_params);
    }

    return MS_OK;
}
#endif

#if 1

#ifdef MS12_FLOW_EN
int main(int argc, char **argv)
#else
int main()
#endif
{
    android::ProcessState::self()->startThreadPool();
#ifdef MS12_FLOW_EN
    listComponents = false;
    dumpProfiles = false;
    extractThumbnail = false;
    seekTest = false;
    useSurfaceAlloc = false;
    useSurfaceTexAlloc = false;
    dumpStream = false;
    dumpPCMStream = false;


    process_graph_t *process_graph = NULL;
    /* initialize local variables with 0 */
    process_graph = (process_graph_t*)malloc(sizeof(process_graph_t));
    if(process_graph == NULL)
    {
        printf("[MS12]process_graph_t malloc fail\n");
        return 0;
    }

    memset(process_graph, 0, sizeof(process_graph_t));
    useMS12Flow = true;
    //String8 dumpStreamFilename;
    gNumRepetitions = 1;
    gMaxNumFrames = 0;
    gReproduceBug = -1;
    gPreferSoftwareCodec = false;
    gForceToUseHardwareCodec = false;
    gPlaybackAudio = false;
    gWriteMP4 = false;
    gDisplayHistogram = false;

    audioOnly = true;
    gPlaybackAudio = true;

    sp<ALooper> looper;

#if 1
    ms12_process_command(argc,(const char**)argv, process_graph);
    //return 0;
    //process_command(argc,argv);
#endif

    argc -= optind;
    argv += optind;

    if(useMS12Flow)
    {
        printf("[YLLIN]use MS12 Flow\n");
    }

    DataSource::RegisterDefaultSniffers();

    OMXClient client;
    status_t err = client.connect();

    {
        bool syncInfoPresent = true;

        //const char *filename = argv[k];
        const char *filename = process_graph->user_params.ms_init_args.input_filename[0];

        sp<DataSource> dataSource = DataSource::CreateFromURI(NULL /* httpService */, filename);;

        size_t len = strlen(filename);


        Vector<sp<MediaSource> > mediaSources;
        sp<MediaSource> mediaSource;

        {
            sp<MediaExtractor> extractor = MediaExtractor::Create(dataSource);

            if (extractor == NULL) {
                fprintf(stderr, "could not create extractor.\n");
                return -1;
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
                for (i = 0; i < numTracks; ++i)
                {
                    meta = extractor->getTrackMetaData(
                            i, MediaExtractor::kIncludeExtensiveMetaData);

                    const char *mime;
                    meta->findCString(kKeyMIMEType, &mime);

                    if (audioOnly && !strncasecmp(mime, "audio/", 6)) {
                        break;
                    }

                    meta = NULL;
                }

                if (meta == NULL)
                {
                    fprintf(stderr,
                            "No suitable %s track found. The '-a' option will "
                            "target audio tracks only, the default is to target "
                            "video tracks only.\n",
                            audioOnly ? "audio" : "video");
                    return -1;
                }

                mediaSource = extractor->getTrack(i);// XXXExtractor, ex. MP3Extractor::MediaSource
            }
        }

        if (dumpStream)
        {
            dumpSource(mediaSource, dumpStreamFilename);
        }
        else if (dumpPCMStream)
        {
            OMXClient client;
            CHECK_EQ(client.connect(), (status_t)OK);

            sp<MediaSource> decSource =
                OMXCodec::Create(
                        client.interface(),
                        mediaSource->getFormat(),
                        false,
                        mediaSource,
                        0,
                        0);

            dumpSource(decSource, dumpStreamFilename);
        //} else if (seekTest) {
        //    performSeekTest(mediaSource);
        } else {
            playSource(&client, mediaSource,process_graph);
        }
    }

    client.disconnect();

    if(process_graph->user_params.h_getparam)
        free(process_graph->user_params.h_getparam);
    if(process_graph)
        free(process_graph);
#endif

    return 0;
}

#endif

#if 0
void process_command(int argc, char **argv)
{
    int res;
    while ((res = getopt(argc, argv, "hMan:lm:b:ptsrow:kxSTd:D:")) >= 0) {
        switch (res) {
            case 'a':
            {
                audioOnly = true;
                break;
            }

            case 'd':
            {
                dumpStream = true;
                dumpStreamFilename.setTo(optarg);
                break;
            }

            case 'D':
            {
                dumpPCMStream = true;
                audioOnly = true;
                dumpStreamFilename.setTo(optarg);
                break;
            }

            case 'l':
            {
                listComponents = true;
                break;
            }

            case 'm':
            case 'n':
            case 'b':
            {
                char *end;
                long x = strtol(optarg, &end, 10);

                if (*end != '\0' || end == optarg || x <= 0) {
                    x = 1;
                }

                if (res == 'n') {
                    gNumRepetitions = x;
                } else if (res == 'm') {
                    gMaxNumFrames = x;
                } else {
                    CHECK_EQ(res, 'b');
                    gReproduceBug = x;
                }
                break;
            }

            case 'w':
            {
                gWriteMP4 = true;
                gWriteMP4Filename.setTo(optarg);
                break;
            }

            case 'p':
            {
                dumpProfiles = true;
                break;
            }

            case 't':
            {
                extractThumbnail = true;
                break;
            }

            case 's':
            {
                gPreferSoftwareCodec = true;
                break;
            }

            case 'r':
            {
                gForceToUseHardwareCodec = true;
                break;
            }

            case 'o':
            {
                gPlaybackAudio = true;
                break;
            }

            case 'k':
            {
                seekTest = true;
                break;
            }

            case 'x':
            {
                gDisplayHistogram = true;
                break;
            }

            case 'S':
            {
                useSurfaceAlloc = true;
                break;
            }

            case 'T':
            {
                useSurfaceTexAlloc = true;
                break;
            }

            case 'M':
            {
                useMS12Flow = true;
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
}
#endif

#if 0
int main(int argc, char **argv) {
    android::ProcessState::self()->startThreadPool();

    audioOnly = false;
    listComponents = false;
    dumpProfiles = false;
    extractThumbnail = false;
    seekTest = false;
    useSurfaceAlloc = false;
    useSurfaceTexAlloc = false;
    dumpStream = false;
    dumpPCMStream = false;

#ifdef MS12_FLOW_EN
    process_graph_t process_graph;
    /* initialize local variables with 0 */
    memset(&process_graph, 0, sizeof(process_graph));
#endif
    useMS12Flow = false;
    //String8 dumpStreamFilename;
    gNumRepetitions = 1;
    gMaxNumFrames = 0;
    gReproduceBug = -1;
    gPreferSoftwareCodec = false;
    gForceToUseHardwareCodec = false;
    gPlaybackAudio = false;
    gWriteMP4 = false;
    gDisplayHistogram = false;

    sp<ALooper> looper;

#if 0
    int res;
    while ((res = getopt(argc, argv, "hMan:lm:b:ptsrow:kxSTd:D:")) >= 0) {
        switch (res) {
            case 'a':
            {
                audioOnly = true;
                break;
            }

            case 'd':
            {
                dumpStream = true;
                dumpStreamFilename.setTo(optarg);
                break;
            }

            case 'D':
            {
                dumpPCMStream = true;
                audioOnly = true;
                dumpStreamFilename.setTo(optarg);
                break;
            }

            case 'l':
            {
                listComponents = true;
                break;
            }

            case 'm':
            case 'n':
            case 'b':
            {
                char *end;
                long x = strtol(optarg, &end, 10);

                if (*end != '\0' || end == optarg || x <= 0) {
                    x = 1;
                }

                if (res == 'n') {
                    gNumRepetitions = x;
                } else if (res == 'm') {
                    gMaxNumFrames = x;
                } else {
                    CHECK_EQ(res, 'b');
                    gReproduceBug = x;
                }
                break;
            }

            case 'w':
            {
                gWriteMP4 = true;
                gWriteMP4Filename.setTo(optarg);
                break;
            }

            case 'p':
            {
                dumpProfiles = true;
                break;
            }

            case 't':
            {
                extractThumbnail = true;
                break;
            }

            case 's':
            {
                gPreferSoftwareCodec = true;
                break;
            }

            case 'r':
            {
                gForceToUseHardwareCodec = true;
                break;
            }

            case 'o':
            {
                gPlaybackAudio = true;
                break;
            }

            case 'k':
            {
                seekTest = true;
                break;
            }

            case 'x':
            {
                gDisplayHistogram = true;
                break;
            }

            case 'S':
            {
                useSurfaceAlloc = true;
                break;
            }

            case 'T':
            {
                useSurfaceTexAlloc = true;
                break;
            }

            case 'M':
            {
                useMS12Flow = true;
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
#endif

#if 1
    ms12_process_command(argc,(const char**)argv, &process_graph);
    process_command(argc,argv);
#endif

    if (gPlaybackAudio && !audioOnly) {
        // This doesn't make any sense if we're decoding the video track.
        gPlaybackAudio = false;
    }

    argc -= optind;
    argv += optind;

    if(useMS12Flow)
    {
        printf("[YLLIN]use MS12 Flow\n");
    }

    //sp<SurfaceComposerClient> composerClient;
    //sp<SurfaceControl> control;

    DataSource::RegisterDefaultSniffers();

    OMXClient client;
    status_t err = client.connect();

    for (int k = 0; k < argc; ++k)
    {
        bool syncInfoPresent = true;

        const char *filename = argv[k];

        sp<DataSource> dataSource = DataSource::CreateFromURI(filename);

        bool isJPEG = false;

        size_t len = strlen(filename);


        Vector<sp<MediaSource> > mediaSources;
        sp<MediaSource> mediaSource;

        {
            sp<MediaExtractor> extractor = MediaExtractor::Create(dataSource);

            if (extractor == NULL) {
                fprintf(stderr, "could not create extractor.\n");
                return -1;
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
                for (i = 0; i < numTracks; ++i)
                {
                    meta = extractor->getTrackMetaData(
                            i, MediaExtractor::kIncludeExtensiveMetaData);

                    const char *mime;
                    meta->findCString(kKeyMIMEType, &mime);

                    if (audioOnly && !strncasecmp(mime, "audio/", 6)) {
                        break;
                    }

                    meta = NULL;
                }

                if (meta == NULL)
                {
                    fprintf(stderr,
                            "No suitable %s track found. The '-a' option will "
                            "target audio tracks only, the default is to target "
                            "video tracks only.\n",
                            audioOnly ? "audio" : "video");
                    return -1;
                }

                mediaSource = extractor->getTrack(i);// XXXExtractor, ex. MP3Extractor::MediaSource
            }
        }

        if (dumpStream)
        {
            dumpSource(mediaSource, dumpStreamFilename);
        }
        else if (dumpPCMStream)
        {
            OMXClient client;
            CHECK_EQ(client.connect(), (status_t)OK);

            sp<MediaSource> decSource =
                OMXCodec::Create(
                        client.interface(),
                        mediaSource->getFormat(),
                        false,
                        mediaSource,
                        0,
                        0);

            dumpSource(decSource, dumpStreamFilename);
        } else if (seekTest) {
            performSeekTest(mediaSource);
        } else {
            playSource(&client, mediaSource);
        }
    }

    client.disconnect();

#ifdef MS12_FLOW_EN
    //free(process_graph.h_scheduler);
    //free(process_graph.h_param_update);
    free(process_graph.user_params.h_getparam);
#endif

    return 0;
}
#endif
