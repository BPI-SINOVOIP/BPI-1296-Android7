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
#define LOG_TAG "MS12PLAYER"
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

#ifdef MS12_PLAYER_EN
#ifdef MS12V1_1_EN
#include "ms12/ms12_cmd.h"
#include "ms12/ms12_getparam.h"
#include "ms12/ms12_update_param.h"
#endif
#ifdef MS12V1_3_EN
#include "ms12v1_3/ms12_cmd.h"
#include "ms12v1_3/ms12_getparam.h"
#include "ms12v1_3/ms12_update_param.h"
#endif
#endif

using namespace android;

#ifdef MS12_PLAYER_EN

#define FRAMESIZE 1536

#define INPUT_MAIN_MASK   0x1
#define INPUT_ASSOC_MASK  0x2
#define INPUT_SYS_MASK    0x4
#define INPUT_APP_MASK    0x8


typedef struct process_graph_s
{
    MS_PARAMS           user_params;
    MS_USE_CASE         use_case;

    OMX_AUDIO_ms12_update_params  update_params;
#ifdef SWITCH_FILE_EN
    SWITCH_FILE_PARAM   switch_file_params;
#endif

#ifdef DDP_CONTINUOUS_TEST
    PauseParam pause_param;
#endif
} process_graph_t;



typedef struct play_source_input_t
{
    process_graph_t *process_graph;
    OMX_AUDIO_MS12_INPUT_PIN_TYPE  inputPinType;
    OMX_BOOL isListeningTest;
    OMX_U8   inputPinMap;
    OMX_U8   dummyInputMap;
} play_source_input_t;


static long gNumRepetitions;
static long gMaxNumFrames;  // 0 means decode all available.
static long gReproduceBug;  // if not -1.
static bool gPlaybackAudio;
static bool useMS12Flow;

//originally locally declare
static bool audioOnly;
//#endif

static sp<ANativeWindow> gSurface;

static int64_t getNowUs() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return (int64_t)tv.tv_usec + tv.tv_sec * 1000000ll;
}

//#ifdef MS12_PLAYER_EN
static void ms12_get_outputfile_info(MS_INIT_PARAMS *p_args, OMX_AUDIO_PARAM_MS12CFG *cfg)
{
    int i, len = 0;

    memset(&cfg->output_file_name[0][0],0,(OMX_AUDIO_MS_MAX_NUM_OUTPUTS*OMX_AUDIO_MS_MAX_OUTPUT_NAME_LEN)*sizeof(char));

    for(i = 0; i < OMX_AUDIO_MS_MAX_NUM_OUTPUTS; i++)
        cfg->output_name_len[i] = 0;

    if(p_args->bitstream_output_filename[MS_DD_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DD_OUT_IDX] = strlen(p_args->bitstream_output_filename[MS_DD_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DD_OUT_IDX][0],p_args->bitstream_output_filename[MS_DD_OUT_IDX],len);
        printf("[outfile DD] %s\n",p_args->bitstream_output_filename[MS_DD_OUT_IDX]);
    }

    if(p_args->bitstream_output_filename[MS_DDP_OUT_IDX])
    {
       len = cfg->output_name_len[OMX_AUDIO_MS_DDP_OUT_IDX] = strlen(p_args->bitstream_output_filename[MS_DDP_OUT_IDX]);
       memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DDP_OUT_IDX][0],p_args->bitstream_output_filename[MS_DDP_OUT_IDX],len);
       printf("[outfile DDP] %s\n",p_args->bitstream_output_filename[MS_DDP_OUT_IDX]);
    }

    if(p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DOWNMIX_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DOWNMIX_OUT_IDX][0],p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX],len);
        printf("[outfile Dwnmix] %s\n",p_args->pcm_output_filename[MS_DOWNMIX_OUT_IDX]);
    }

    if(p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_VIRTUALIZER_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_VIRTUALIZER_OUT_IDX],p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX],len);
        printf("[outfile Main] %s\n",p_args->pcm_output_filename[MS_VIRTUALIZER_OUT_IDX]);
    }

    if(p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DAP_HEADPHONE_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DAP_HEADPHONE_OUT_IDX],p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX],len);
        printf("[outfile DAP headphone] %s\n",p_args->pcm_output_filename[MS_DAP_HEADPHONE_OUT_IDX]);
    }

    if(p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX])
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_DAP_SPEAKER_OUT_IDX] = strlen(p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX]);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_DAP_SPEAKER_OUT_IDX],p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX],len);
        printf("[outfile DAP spk] %s\n",p_args->pcm_output_filename[MS_DAP_SPEAKER_OUT_IDX]);
    }

    if(p_args->pcmr_output_filename)
    {
        len = cfg->output_name_len[OMX_AUDIO_MS_PCMR_OUT_IDX] = strlen(p_args->pcmr_output_filename);
        memcpy(&cfg->output_file_name[OMX_AUDIO_MS_PCMR_OUT_IDX],p_args->pcmr_output_filename,len);
        printf("[outfile PCMR] %s\n",(char*)p_args->pcm_output_filename);
    }

}

static void playSource(OMXClient *client, sp<MediaSource> &source, play_source_input_t *input)
{
    process_graph_t *process_graph = input->process_graph;

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
            printf("Get MS12 param struct, size %d\n",(int)sizeof(OMX_AUDIO_PARAM_MS12CFG));

        //FIXME!!!
        //memcpy(&cfg->param_struct.ms_runtime_args,&process_graph->user_params.ms_runtime_args,sizeof(OMX_AUDIO_MS_RUNTIME_PARAMS));
        //memcpy(&cfg->param_struct.ms_init_args,&process_graph->user_params.ms_init_args,sizeof(OMX_AUDIO_MS_INIT_PARAMS));
        //memcpy(&cfg->param_struct.use_case,&process_graph->use_case,sizeof(OMX_AUDIO_MS_USE_CASE));
        CopyMS12RuntimeParamToOMXAudio((OMX_AUDIO_MS_RUNTIME_PARAMS*)&cfg->param_struct.ms_runtime_args, (MS_RUNTIME_PARAMS*)&process_graph->user_params.ms_runtime_args);
        CopyMS12IinitParamToOMXAudio((OMX_AUDIO_MS_INIT_PARAMS*)&cfg->param_struct.ms_init_args, (MS_INIT_PARAMS*)&process_graph->user_params.ms_init_args);
        CopyMS12UseCaseToOMXAudio((OMX_AUDIO_MS_USE_CASE*)&cfg->param_struct.use_case, (MS_USE_CASE*)&process_graph->use_case);

        memcpy(&cfg->update_param_struct,&process_graph->update_params,sizeof(OMX_AUDIO_ms12_update_params));
        ms12_get_outputfile_info((MS_INIT_PARAMS *)&process_graph->user_params.ms_init_args , cfg);

        //FIXME!!!
        cfg->dummyInputMap = input->dummyInputMap;
        cfg->inputPinType = input->inputPinType;
        cfg->isListeningTest = input->isListeningTest;
        cfg->inputPinMap = input->inputPinMap;
        printf("[inputPinType] %d\n",cfg->inputPinType);
        err1 = rawSource->setMS12FlowConfig((void*)cfg);

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

#ifdef  DDP_CONTINUOUS_TEST
        bool is_paused = false;
        int64_t play_start = getNowUs();
#endif

        player->start(true /* sourceAlreadyStarted */);
printf("[YLLIN][after player->start]\n");
        status_t finalStatus;
        while (!player->reachedEOS(&finalStatus))
        {

#ifdef  DDP_CONTINUOUS_TEST
            if (process_graph->pause_param.is_pause)
            {
                int64_t elapse_time = getNowUs() - play_start;

                if (elapse_time >= process_graph->pause_param.start_us
                    && elapse_time < process_graph->pause_param.end_us
                    && !is_paused)
                {
                    OMX_AUDIO_PARAM_MS12CFG config;
                    memset(&config, 0, sizeof(config));

                    config.dummyInputMap = 32;
                    rawSource->setMS12FlowConfig((void*)&config);

                    is_paused = true;
                }

                if (elapse_time > process_graph->pause_param.end_us && is_paused)
                {
                    OMX_AUDIO_PARAM_MS12CFG config;
                    memset(&config, 0, sizeof(config));

                    config.dummyInputMap = 64;
                    rawSource->setMS12FlowConfig((void*)&config);

                    is_paused = false;
                }
            }
#endif
            usleep(100000ll);
        }

        rawSource.clear();
        delete player;
        player = NULL;

        return;
    }
#if 0
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
#endif

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

    } else if (!strncasecmp("audio/", mime, 6)) {
        // Frame count makes less sense for audio, as the output buffer
        // sizes may be different across decoders.
        printf("avg. %.2f KB/sec\n", totalBytes / 1024 * 1E6 / delay);

        printf("decoded a total of %lld bytes\n", (long long)totalBytes);
    }
}
#endif
////////////////////////////////////////////////////////////////////////////////

#ifdef MS12_PLAYER_EN

int read_command_from_text_file(DLB_GETPARAM_HANDLE h_get_param, const char *cmd_file_name, DLB_TIMESTAMP *cur, DLB_TIMESTAMP *next)
{
    int err = 0;
    unsigned int b_cmd_file_exists = 1;
    //long numFrameNewParams = 0;

    err = dlb_getparam_parse_textfile(h_get_param, cmd_file_name, cur, next);

    if (err == DLB_GETPARAM_ERROR_CMDFILEOPEN)
    {      /* No cmd file, get options from cmd line */
		b_cmd_file_exists = 0;
		fprintf(stderr, "No cmd file, get options from cmd line\n");
		return DLB_GETPARAM_ERROR_CMDFILEOPEN;
    }
    else if (err != DLB_GETPARAM_OK)
    {      /* Error while reading command file */
        fprintf( stderr,"Cannot read command line from %s. (error code: %d) \n",cmd_file_name ,err);
        return err;
    }
    else
    {
        /* Command file parsed successfully */
        printf("\n--- Using command file %s! ---\n\n", cmd_file_name);
        if (next->format == DLB_TIMESTAMP_EOF)
        { /* There are no more parameters to read, so deactivate cmd file reading */
            b_cmd_file_exists = 0;
        }
//        else if (cur->format != DLB_TIMESTAMP_FRAME || next->format != DLB_TIMESTAMP_FRAME)
//        {
//            fprintf( stderr ,"Bad timestamp format found in %s! Found: %d, %d\n",cmd_file_name, cur->format, next->format);
//            return -1;
//        }
//        else
//        {   /* Next frame with new parameters */
//            numFrameNewParams = next->value[next->format];
//        }
    }    /* Parse command line switches */

    return DLB_GETPARAM_OK;
}

int check_runtime_parameters(process_graph_t *process_graph, param_update_handle h_param_update, const char *cmd_file_name, MS_USE_CASE *p_use_case)
{
    DLB_TIMESTAMP from, next, from_dst;
    DLB_GETPARAM_HANDLE h_get_param = h_param_update->h_get_param;
    //DLB_TIMESTAMP conv_timestamp;
    int retval;
    int node_idx;
    printf("[check_runtime_parameters]\n");

    if(h_get_param == NULL)
        return MS_GENERAL_ERROR;

    build_node_param_update_table(h_param_update, p_use_case);
    from.format = DLB_TIMESTAMP_FRAME;

    do
    {
        //printf("[YLLIN]update param parse textfile, %d\n",h_param_update->curr_num_nodes);
        next.format = DLB_TIMESTAMP_EOF;

        dlb_getparam_parse_textfile(h_get_param, cmd_file_name, &from, &next);
//        printf("[parse text] %d %d\n",from.format,next.format);

//        if(next.format == DLB_TIMESTAMP_EOF)
//        {
            //h_param_update->time_of_next_update.format = DLB_TIMESTAMP_EOF;
//            return DLB_TIMESTAMP_EOF;
//        }
  //      else
        {
            from.format = DLB_TIMESTAMP_HMS;
//            next_dst.format = DLB_TIMESTAMP_SAMPLE;
            from_dst.format = DLB_TIMESTAMP_SAMPLE;
/*
            dlb_getparam_timestamp_convert
                (h_get_param
                ,&next
                ,&next_dst
                ,48000
                ,FRAMESIZE
                );
*/
            dlb_getparam_timestamp_convert
                (&from
                ,&from_dst
                ,48000
                ,FRAMESIZE
                );
            printf("[convert time smpl] %d\n",(int)from_dst.value[0]);
        }

        for(node_idx=0; node_idx < h_param_update->curr_num_nodes; node_idx++)
        {
#ifdef SWITCH_FILE_EN
            retval = retrieve_param(&process_graph->update_params, h_param_update, node_idx, (DLB_TIMESTAMP*)&from_dst,&process_graph->switch_file_params);
#else
            retval = retrieve_param(&process_graph->update_params, h_param_update, node_idx, (DLB_TIMESTAMP*)&from_dst);
#endif
            if(retval != MSF_OK)
            {
                return MSF_ERROR;
            }
        }

        if(next.format == DLB_TIMESTAMP_EOF)
        {
            //h_param_update->time_of_next_update.format = DLB_TIMESTAMP_EOF;
            return DLB_TIMESTAMP_EOF;
        }
    }while(next.format != DLB_TIMESTAMP_EOF);

    return MS_OK;
}

char check_is_dummy_input(const char *p_filename)
{
    char ret = 0;

    if (!strncmp(p_filename, "dummy.", 6))
    {
        /* dummy input */
        ret = 1;
        printf("detect Dummy input!\n");
    }

    return ret;
}

char ms12_get_dummy_input_map(MS_INIT_PARAMS *p_args)
{
    const char *filename_main = p_args->input_filename[MS_MAIN_DECODER_IDX];
    const char *filename_assoc = p_args->input_filename[MS_ASSOC_DECODER_IDX];

    char isMainDummy = check_is_dummy_input(filename_main);
    char isAssocDummy = check_is_dummy_input(filename_assoc);

    char map = (isMainDummy) | (isAssocDummy << 1);

    return map;

}

int ms12_process_command(int argc, char **argv, process_graph_t *process_graph, param_update_handle h_param_update)
{
    int param_error_code = 0;
    int msd_error_returned = 0;
    unsigned long int extern_static_size;
    void *p_extern_static;
#ifdef MS12_PLAYER_USE_TXT_CMD
    const char cmd_file_name[] = "command.txt";
    DLB_TIMESTAMP cur, next;
    int err;
#else
    h_param_update = NULL;
#endif

    MS_RETVAL retval;
    /* Initialize the parameter struct */
    ms_init_params((MS_PARAMS *)&process_graph->user_params);
    process_graph->update_params.num_param_update = 0;
#ifdef MS12V1_3_EN
    process_graph->use_case.input_type = UNKNOWN;
#endif

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

#ifdef MS12_PLAYER_USE_TXT_CMD
    cur.format = DLB_TIMESTAMP_FRAME;
    next.format = DLB_TIMESTAMP_SAMPLE;
    err = read_command_from_text_file(process_graph->user_params.h_getparam, cmd_file_name, (DLB_TIMESTAMP *)&cur, (DLB_TIMESTAMP *)&next);
    //printf("next.value[0] %d\n",next.value[0]);
#endif

    /* Parse command line, detect use case, validate arguments */
    printf("[ms_parse_command_line] start\n");
    retval = ms_parse_command_line(argc, (const char **)argv, &process_graph->use_case, &process_graph->user_params, &param_error_code);
    printf("[ms_parse_command_line] end\n");
    msd_error_returned = ms_handle_parser_retval(retval, param_error_code);

    if (msd_error_returned)
        return msd_error_returned;

#ifdef MS12_PLAYER_USE_TXT_CMD
    if((err == DLB_GETPARAM_OK)&&(next.format != DLB_TIMESTAMP_EOF))
    {
        //TODO: (1)check substream select params 'at', need to do malloc at the beginning.
        printf("[read text file next line]\n");
        GETPARAM_MEM *h_getparam_update = (GETPARAM_MEM *)malloc(sizeof(GETPARAM_MEM));
        memset(h_getparam_update,0,sizeof(GETPARAM_MEM));
        memcpy(h_getparam_update,process_graph->user_params.h_getparam,sizeof(GETPARAM_MEM));
        //h_param_update->h_get_param = process_graph->user_params.h_getparam;//h_get_param_update;
        h_param_update->h_get_param = h_getparam_update;
        //if(next.format != DLB_TIMESTAMP_EOF)
        check_runtime_parameters(process_graph, h_param_update, cmd_file_name, &process_graph->use_case);
            //check_runtime_parameters(process_graph, h_param_update, cmd_file_name, &process_graph->use_case);
        if(h_getparam_update)
            free(h_getparam_update);
    }
#endif
    /* Print parameter settings */
    if(process_graph->user_params.ms_init_args.verbose_mode > 1){
        ms_print_parameter_settings(&process_graph->use_case, &process_graph->user_params);
    }

    return MS_OK;
}

static void *ms12_decode_thread(void *data)
{
    play_source_input_t *in = (play_source_input_t *)data;
    process_graph_t *process_graph = in->process_graph;
    int fn_idx = 0;


    DataSource::RegisterDefaultSniffers();

    OMXClient client;
    status_t err = client.connect();
    if(err)
    printf("[ms12] client.conenct() fails\n");

    //Get file name idx
    {
        if(in->inputPinType == OMX_AUDIO_MS12_INPUT_MAIN)
        {
            fn_idx = MS_MAIN_DECODER_IDX;
            printf("get fn_idx Main %d\n",fn_idx);
        }
        else if(in->inputPinType == OMX_AUDIO_MS12_INPUT_ASSOC)
        {
            fn_idx = MS_ASSOC_DECODER_IDX;
            printf("get fn_idx Assoc %d\n",fn_idx);
        }
        else if(in->inputPinType == OMX_AUDIO_MS12_INPUT_SYSTEM)
        {
            fn_idx = MS_SYSTEM_SOUNDS_IDX;
            printf("get fn_idx Sys %d\n",fn_idx);
        }
        else if(in->inputPinType == OMX_AUDIO_MS12_INPUT_APP)
        {
            fn_idx = MS_APPLICATION_SOUNDS_IDX;
            printf("get fn_idx APP %d\n",fn_idx);
        }
    }

    {
        bool syncInfoPresent = true;

        //const char *filename = argv[k];
        const char *filename;// = process_graph->user_params.ms_init_args.input_filename[fn_idx];
        //printf("[in file name][%d] %s\n",fn_idx,process_graph->user_params.ms_init_args.input_filename[fn_idx]);
#ifdef MS12V1_3_EN
        if((process_graph->use_case.b_dual_main == 1)
           &&((in->inputPinType == OMX_AUDIO_MS12_INPUT_MAIN)||(in->inputPinType == OMX_AUDIO_MS12_INPUT_ASSOC))
           )
        {
            char isMainDummy = (in->dummyInputMap & (1 << OMX_AUDIO_MS12_INPUT_MAIN));
            char isAssocDummy = (in->dummyInputMap & (1 << OMX_AUDIO_MS12_INPUT_ASSOC));
            if((isMainDummy)&&(isAssocDummy))
            {
                printf("[FATAL ERR] both Main and Main2 are dummy input!!!\n");
                return 0;
            }
            else if(isMainDummy)
                filename = process_graph->user_params.ms_init_args.input_filename[MS_ASSOC_DECODER_IDX];
            else if(isAssocDummy)
                filename = process_graph->user_params.ms_init_args.input_filename[MS_MAIN_DECODER_IDX];
            else
                filename = process_graph->user_params.ms_init_args.input_filename[fn_idx];
        }
        else
#endif
        {
            filename = process_graph->user_params.ms_init_args.input_filename[fn_idx];
        }
        printf("[in file name][%d] %s\n",fn_idx,filename);

        sp<DataSource> dataSource = DataSource::CreateFromURI(NULL /* httpService */, filename);;

        //size_t len = strlen(filename);


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
                    return 0;
                }

                mediaSource = extractor->getTrack(i);// XXXExtractor, ex. MP3Extractor::MediaSource
            }
        }

        {
            //playSource(&client, mediaSource,process_graph);
            playSource(&client, mediaSource,in);
        }
    }

    client.disconnect();


    return 0;
}

static OMX_U8 get_ms12_input_pin_map(process_graph_t *process_graph)
{
    OMX_U8 inputPinMap = 0;
    MS_INIT_PARAMS *p_args = (MS_INIT_PARAMS *)&process_graph->user_params.ms_init_args;

    if(p_args->input_filename[MS_MAIN_DECODER_IDX])
    {
        inputPinMap |= (1 << MS_MAIN_DECODER_IDX);
    }
    if(p_args->input_filename[MS_ASSOC_DECODER_IDX])
    {
        inputPinMap |= (1 << MS_ASSOC_DECODER_IDX);
    }
    if(p_args->input_filename[MS_SYSTEM_SOUNDS_IDX])
    {
        inputPinMap |= (1 << MS_SYSTEM_SOUNDS_IDX);
    }
    if(p_args->input_filename[MS_APPLICATION_SOUNDS_IDX])
    {
        inputPinMap |= (1 << MS_APPLICATION_SOUNDS_IDX);
    }

    return inputPinMap;
}

static void setup_ms12_decode_process(process_graph_t *process_graph)
{
    pthread_t thread_main, thread_assoc, thread_app, thread_sys;
    play_source_input_t in_main, in_assoc, in_app, in_sys;
    void *ret1, *ret2, *ret3, *ret4;
    MS_INIT_PARAMS *p_args = (MS_INIT_PARAMS *)&process_graph->user_params.ms_init_args;
    OMX_U8 inputPinMap = 0;
    OMX_U8 dummyInputMap = 0;
    OMX_BOOL isListen = OMX_TRUE;

#ifdef MS12_FILE_WRITE_EN
    isListen = OMX_FALSE;
#endif

    if(isListen == OMX_FALSE)
    {
        printf("[ms12] file write flow\n");
    }
    else
    {
        printf("[ms12] listening flow\n");
    }

    printf("[ms12] pthread start\n");
    inputPinMap = get_ms12_input_pin_map(process_graph);
#ifdef MS12V1_3_EN
    if(process_graph->use_case.b_dual_main == 1)
        dummyInputMap = (OMX_U8)ms12_get_dummy_input_map(p_args);
#endif

    if(p_args->input_filename[MS_MAIN_DECODER_IDX])
    {
        in_main.dummyInputMap = dummyInputMap;
        in_main.inputPinType = OMX_AUDIO_MS12_INPUT_MAIN;
        in_main.isListeningTest = isListen;
        in_main.inputPinMap = inputPinMap;
        in_main.process_graph = process_graph;
        printf("[create Main decoder thread]\n");
        if(dummyInputMap & 0x1)
        {
            printf("Main pin has dummy input\n");
        }
        pthread_create(&thread_main, NULL , ms12_decode_thread , (void*) &in_main);
    }
    if(p_args->input_filename[MS_ASSOC_DECODER_IDX])
    {
        in_assoc.dummyInputMap = dummyInputMap;
        in_assoc.inputPinType = OMX_AUDIO_MS12_INPUT_ASSOC;
        in_assoc.isListeningTest = isListen;
        in_assoc.inputPinMap = inputPinMap;
        in_assoc.process_graph = process_graph;
        printf("[create Assoc decoder thread]\n");
        if(dummyInputMap & 0x2)
        {
            printf("Assoc pin has dummy input\n");
        }
        pthread_create(&thread_assoc, NULL , ms12_decode_thread , (void*) &in_assoc);
    }
    if(p_args->input_filename[MS_SYSTEM_SOUNDS_IDX])
    {
        in_sys.dummyInputMap = dummyInputMap;
        in_sys.inputPinType = OMX_AUDIO_MS12_INPUT_SYSTEM;
        in_sys.isListeningTest = isListen;
        in_sys.inputPinMap = inputPinMap;
        in_sys.process_graph = process_graph;
        printf("[create Sys decoder thread]\n");
        pthread_create(&thread_sys, NULL , ms12_decode_thread , (void*) &in_sys);
    }
    if(p_args->input_filename[MS_APPLICATION_SOUNDS_IDX])
    {
        in_app.dummyInputMap = dummyInputMap;
        in_app.inputPinType = OMX_AUDIO_MS12_INPUT_APP;
        in_app.isListeningTest = isListen;
        in_app.inputPinMap = inputPinMap;
        in_app.process_graph = process_graph;
        printf("[create App decoder thread]\n");
        pthread_create(&thread_app, NULL , ms12_decode_thread , (void*) &in_app);
    }

    if(p_args->input_filename[MS_APPLICATION_SOUNDS_IDX])
        pthread_join( thread_app, &ret4);
    if(p_args->input_filename[MS_SYSTEM_SOUNDS_IDX])
        pthread_join( thread_sys, &ret3);
    if(p_args->input_filename[MS_ASSOC_DECODER_IDX])
        pthread_join( thread_assoc, &ret2);
    if(p_args->input_filename[MS_MAIN_DECODER_IDX])
        pthread_join( thread_main, &ret1);
}

bool ConvertToMicroSecond(unsigned long int &micro_second, char *p_string)
{
    if (p_string == NULL)
        return false;

    if (strlen(p_string) != 8)
        return false;

    int hour = (p_string[0] - '0') * 10 + (p_string[1] - '0');
    int min = (p_string[3] - '0') * 10 + (p_string[4] - '0');
    int sec = (p_string[6] - '0') * 10 + (p_string[7] - '0');

    micro_second = (hour * 3600 + min * 60 + sec) * 1000000;

    return true;
}

#ifdef MS12V1_3_EN
void ProcessPauseCommand(PauseParam &param)
{
    param.is_pause = false;

    const char file_name[] = "pause.txt";

    FILE *p_file = fopen(file_name, "r");

    if (p_file != NULL)
    {
        const int buffer_size = 128;
        char command_string[buffer_size] = {0};
        char start_string[buffer_size] = {0};
        char end_string[buffer_size] = {0};

        fscanf(p_file, "%s %s %s", command_string, start_string, end_string);

        unsigned long int converted_start = 0, converted_end = 0;
        bool is_start_ok = ConvertToMicroSecond(converted_start, start_string);
        bool is_end_ok = ConvertToMicroSecond(converted_end, end_string);

        if (strncmp(command_string, "-pausetime", 10) == 0
            && is_start_ok && is_end_ok && converted_end > converted_start)
        {
            param.start_us = converted_start;
            param.end_us = converted_end;
            param.is_pause = true;
        }

        fclose(p_file);
        p_file = NULL;
    }
}
#endif

#endif

#ifdef MS12_PLAYER_EN
int main(int argc, char **argv)
#else
int main(void)
#endif
{
    android::ProcessState::self()->startThreadPool();

#ifdef MS12_PLAYER_EN
    process_graph_t *process_graph = NULL;
    param_update_s *user_update_params = NULL;
    /* initialize local variables with 0 */
    process_graph = (process_graph_t*)malloc(sizeof(process_graph_t));
    user_update_params = (param_update_s*)malloc(sizeof(param_update_s));
    if(process_graph == NULL)
    {
        printf("[MS12]process_graph_t malloc fail\n");
        return 0;
    }

    memset(process_graph, 0, sizeof(process_graph_t));
    memset(user_update_params, 0, sizeof(param_update_s));
    useMS12Flow = true;
    gNumRepetitions = 1;
    gMaxNumFrames = 0;
    gReproduceBug = -1;


    audioOnly = true;
    gPlaybackAudio = true;

    sp<ALooper> looper;

    /* initialize runtime parameter updates */
    unsigned int mem_size = OMX_AUDIO_MAX_MS12_UPDATE_PARAM_MEM_SIZE;
    void *p_extern_static;
    //param_update_query_mem(PU_MAX_NUM_NODES,PU_MAX_NUM_INTVALS, &mem_size);
    p_extern_static = malloc(mem_size);
    //printf("[YLLIN]param_update_open\n");
    param_update_open (user_update_params, PU_MAX_NUM_NODES ,PU_MAX_NUM_INTVALS ,p_extern_static);

    ms12_process_command(argc,argv, process_graph, user_update_params);

#ifdef DDP_CONTINUOUS_TEST
    memset(&process_graph->pause_param, 0, sizeof(process_graph->pause_param));
    ProcessPauseCommand(process_graph->pause_param);
#endif

    argc -= optind;
    argv += optind;

    if(useMS12Flow)
    {
        printf("[YLLIN]use MS12 Flow\n");
    }


//=============================
#if 0
{
  pthread_t thread1, thread2;
  //IND in1, in2;
  play_source_input_t in1, in2;
   void *ret1, *ret2;
   in1.inputPinType = OMX_AUDIO_MS12_INPUT_MAIN;
   in1.isListeningTest = OMX_FALSE;
   in1.isLastPin = OMX_TRUE;
   in1.process_graph = process_graph;
   in2.inputPinType = OMX_AUDIO_MS12_INPUT_ASSOC;
   in2.isListeningTest = OMX_FALSE;
   in2.isLastPin = OMX_TRUE;
   in2.process_graph = process_graph;
printf("[ms12] pthread start\n");
   pthread_create(&thread1, NULL , ms12_decode_thread , (void*) &in1);
   pthread_create(&thread2, NULL , ms12_decode_thread , (void*) &in2);
   pthread_join( thread1, &ret1);
   pthread_join( thread2, &ret2);
printf("[ms12] pthread end\n");
}
#endif
{
    setup_ms12_decode_process(process_graph);
}
//=========================
    if(process_graph->user_params.h_getparam)
        free(process_graph->user_params.h_getparam);
    if(process_graph)
        free(process_graph);
    if(p_extern_static)
        free(p_extern_static);
    if(user_update_params)
        free(user_update_params);
#endif

    return 0;
}

