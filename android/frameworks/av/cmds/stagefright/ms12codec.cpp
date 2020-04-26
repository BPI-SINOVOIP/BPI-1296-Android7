/*
 * Copyright (C) 2012 The Android Open Source Project
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
#define LOG_TAG "ms12codec"
#include <inttypes.h>
#include <utils/Log.h>

#include "SimplePlayer.h"

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <media/ICrypto.h>
#include <media/IMediaHTTPService.h>
#include <media/IMediaPlayerService.h>
#include <media/stagefright/foundation/ABuffer.h>
#include <media/stagefright/foundation/ADebug.h>
#include <media/stagefright/foundation/ALooper.h>
#include <media/stagefright/foundation/AMessage.h>
#include <media/stagefright/foundation/AString.h>
#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaCodec.h>
#include <media/stagefright/MediaCodecList.h>
#include <media/stagefright/MediaDefs.h>
#include <media/stagefright/NuMediaExtractor.h>
#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/Surface.h>
#include <ui/DisplayInfo.h>

#ifdef MS12V1_3_EN
#include "ms12v1_3/ms12_cmd.h"
#include "ms12v1_3/ms12_getparam.h"
#include "ms12v1_3/ms12_update_param.h"
#endif


namespace android {

struct CodecState {
    sp<MediaCodec> mCodec;
    Vector<sp<ABuffer> > mInBuffers;
    Vector<sp<ABuffer> > mOutBuffers;
    bool mSignalledInputEOS;
    bool mSawOutputEOS;
    int64_t mNumBuffersDecoded;
    int64_t mNumBytesDecoded;
    bool mIsAudio;
};

}  // namespace android

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

    OMX_BOOL useAudio;
    OMX_BOOL useVideo;
    OMX_BOOL renderSurface;
    OMX_BOOL useTimestamp;
} play_source_input_t;


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

static void set_ms12_parameter(process_graph_t *process_graph, play_source_input_t *input)
{
    using namespace android;

    status_t err1 = 0;
    printf("[play source]ms12 flow start\n");
    OMX_AUDIO_PARAM_MS12CFG *cfg = NULL;
    cfg = (OMX_AUDIO_PARAM_MS12CFG *)malloc(sizeof(OMX_AUDIO_PARAM_MS12CFG));
    if(cfg == NULL)
        printf("MS12 param struct malloc fail!\n");
    else
        printf("Get MS12 param struct, size %d\n",(int)sizeof(OMX_AUDIO_PARAM_MS12CFG));

    CopyMS12RuntimeParamToOMXAudio((OMX_AUDIO_MS_RUNTIME_PARAMS*)&cfg->param_struct.ms_runtime_args, (MS_RUNTIME_PARAMS*)&process_graph->user_params.ms_runtime_args);
    CopyMS12IinitParamToOMXAudio((OMX_AUDIO_MS_INIT_PARAMS*)&cfg->param_struct.ms_init_args, (MS_INIT_PARAMS*)&process_graph->user_params.ms_init_args);
    CopyMS12UseCaseToOMXAudio((OMX_AUDIO_MS_USE_CASE*)&cfg->param_struct.use_case, (MS_USE_CASE*)&process_graph->use_case);

    memcpy(&cfg->update_param_struct,&process_graph->update_params,sizeof(OMX_AUDIO_ms12_update_params));
    ms12_get_outputfile_info((MS_INIT_PARAMS *)&process_graph->user_params.ms_init_args , cfg);

    cfg->dummyInputMap = input->dummyInputMap;
    cfg->inputPinType = input->inputPinType;
    cfg->isListeningTest = input->isListeningTest;
    cfg->inputPinMap = input->inputPinMap;
    printf("[inputPinType] %d\n",cfg->inputPinType);
    //FIXME!!!!!!!!!!!!!! set parameter through MediaCodec here!!!
    //FIXME!!!!!!!!!!!!!! set parameter through MediaCodec here!!!
    //err1 = rawSource->setMS12FlowConfig((void*)cfg);

    printf("[play source]ms12 flow end\n");
    if(err1 == ERROR_UNSUPPORTED)
        printf("[play source]ms12 flow end,ERROR_UNSUPPORTED \n");

    if(cfg) free(cfg);

}

static void *ms12_decode_thread(void *data)
{
    using namespace android;

    play_source_input_t *in = (play_source_input_t *)data;
    process_graph_t *process_graph = in->process_graph;
    bool useAudio = in->useAudio;
    bool useVideo = in->useVideo;
    bool renderSurface = in->renderSurface;
    bool useTimestamp = in->useTimestamp;
    const char *path = process_graph->user_params.ms_init_args.input_filename[MS_MAIN_DECODER_IDX];;

    static int64_t kTimeout = 500ll;

    DataSource::RegisterDefaultSniffers();

    sp<ALooper> looper = new ALooper;
    looper->start();

    sp<Surface> surface;

    sp<NuMediaExtractor> extractor = new NuMediaExtractor;

    if (extractor->setDataSource(NULL /* httpService */, path) != OK) {
        fprintf(stderr, "unable to instantiate extractor.\n");
        return 0;
    }

    KeyedVector<size_t, CodecState> stateByTrack;

    bool haveAudio = false;
    bool haveVideo = false;
    for (size_t i = 0; i < extractor->countTracks(); ++i)
    {
        sp<AMessage> format;
        status_t err = extractor->getTrackFormat(i, &format);
        CHECK_EQ(err, (status_t)OK);

        AString mime;
        CHECK(format->findString("mime", &mime));

        bool isAudio = !strncasecmp(mime.c_str(), "audio/", 6);
        bool isVideo = !strncasecmp(mime.c_str(), "video/", 6);

        if (useAudio && !haveAudio && isAudio)
            haveAudio = true;
        else if (useVideo && !haveVideo && isVideo)
            haveVideo = true;
        else
            continue;


        ALOGV("selecting track %zu", i);

        err = extractor->selectTrack(i);
        CHECK_EQ(err, (status_t)OK);

        CodecState *state =
            &stateByTrack.editValueAt(stateByTrack.add(i, CodecState()));

        state->mNumBytesDecoded = 0;
        state->mNumBuffersDecoded = 0;
        state->mIsAudio = isAudio;

        state->mCodec = MediaCodec::CreateByType(
                looper, mime.c_str(), false /* encoder */);

        CHECK(state->mCodec != NULL);
ALOGD("[state->mCodec->configure] start\n");
        err = state->mCodec->configure(
                format, isVideo ? surface : NULL,
                NULL /* crypto */,
                0 /* flags */);
ALOGD("[state->mCodec->configure] end\n");
        CHECK_EQ(err, (status_t)OK);

        state->mSignalledInputEOS = false;
        state->mSawOutputEOS = false;

        if(isAudio == true)
        {
            ALOGD("[set_ms12_parameter]\n");
            set_ms12_parameter(process_graph, in);
        }

    }

    CHECK(!stateByTrack.isEmpty());

    int64_t startTimeUs = ALooper::GetNowUs();
    int64_t startTimeRender = -1;

    for (size_t i = 0; i < stateByTrack.size(); ++i)
    {
        CodecState *state = &stateByTrack.editValueAt(i);

        sp<MediaCodec> codec = state->mCodec;

        CHECK_EQ((status_t)OK, codec->start());
        CHECK_EQ((status_t)OK, codec->getInputBuffers(&state->mInBuffers));
        CHECK_EQ((status_t)OK, codec->getOutputBuffers(&state->mOutBuffers));

        ALOGV("got %zu input and %zu output buffers",
              state->mInBuffers.size(), state->mOutBuffers.size());
    }

    bool sawInputEOS = false;

    for (;;)
    {
        if (!sawInputEOS)
        {
            size_t trackIndex;
            status_t err = extractor->getSampleTrackIndex(&trackIndex);

            if (err != OK)
            {
                ALOGV("saw input eos");
                sawInputEOS = true;
            }
            else
            {
                CodecState *state = &stateByTrack.editValueFor(trackIndex);

                size_t index;
                err = state->mCodec->dequeueInputBuffer(&index, kTimeout);

                if (err == OK)
                {
                    ALOGV("filling input buffer %zu", index);

                    const sp<ABuffer> &buffer = state->mInBuffers.itemAt(index);

                    err = extractor->readSampleData(buffer);
                    CHECK_EQ(err, (status_t)OK);

                    int64_t timeUs;
                    err = extractor->getSampleTime(&timeUs);
                    CHECK_EQ(err, (status_t)OK);

                    uint32_t bufferFlags = 0;

                    err = state->mCodec->queueInputBuffer(
                            index,
                            0 /* offset */,
                            buffer->size(),
                            timeUs,
                            bufferFlags);

                    CHECK_EQ(err, (status_t)OK);

                    extractor->advance();
                }
                else
                {
                    CHECK_EQ(err, -EAGAIN);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < stateByTrack.size(); ++i)
            {
                CodecState *state = &stateByTrack.editValueAt(i);

                if (!state->mSignalledInputEOS)
                {
                    size_t index;
                    status_t err =
                        state->mCodec->dequeueInputBuffer(&index, kTimeout);

                    if (err == OK)
                    {
                        ALOGV("signalling input EOS on track %zu", i);

                        err = state->mCodec->queueInputBuffer(
                                index,
                                0 /* offset */,
                                0 /* size */,
                                0ll /* timeUs */,
                                MediaCodec::BUFFER_FLAG_EOS);

                        CHECK_EQ(err, (status_t)OK);

                        state->mSignalledInputEOS = true;
                    }
                    else
                    {
                        CHECK_EQ(err, -EAGAIN);
                    }
                }
            }
        }

        bool sawOutputEOSOnAllTracks = true;
        for (size_t i = 0; i < stateByTrack.size(); ++i)
        {
            CodecState *state = &stateByTrack.editValueAt(i);
            if (!state->mSawOutputEOS)
            {
                sawOutputEOSOnAllTracks = false;
                break;
            }
        }

        if (sawOutputEOSOnAllTracks)
        {
            break;
        }

        for (size_t i = 0; i < stateByTrack.size(); ++i)
        {
            CodecState *state = &stateByTrack.editValueAt(i);

            if (state->mSawOutputEOS)
            {
                continue;
            }

            size_t index;
            size_t offset;
            size_t size;
            int64_t presentationTimeUs;
            uint32_t flags;
            status_t err = state->mCodec->dequeueOutputBuffer(
                    &index, &offset, &size, &presentationTimeUs, &flags,
                    kTimeout);

            if (err == OK)
            {
                ALOGV("draining output buffer %zu, time = %lld us",
                      index, (long long)presentationTimeUs);

                ++state->mNumBuffersDecoded;
                state->mNumBytesDecoded += size;

                if (surface == NULL || !renderSurface)
                {
                    err = state->mCodec->releaseOutputBuffer(index);
                }
                else if (useTimestamp)
                {
                    if (startTimeRender == -1)
                    {
                        // begin rendering 2 vsyncs (~33ms) after first decode
                        startTimeRender =
                                systemTime(SYSTEM_TIME_MONOTONIC) + 33000000
                                - (presentationTimeUs * 1000);
                    }
                    presentationTimeUs =
                            (presentationTimeUs * 1000) + startTimeRender;
                    err = state->mCodec->renderOutputBufferAndRelease(
                            index, presentationTimeUs);
                }
                else
                {
                    err = state->mCodec->renderOutputBufferAndRelease(index);
                }

                CHECK_EQ(err, (status_t)OK);

                if (flags & MediaCodec::BUFFER_FLAG_EOS)
                {
                    ALOGV("reached EOS on output.");

                    state->mSawOutputEOS = true;
                }
            }
            else if (err == INFO_OUTPUT_BUFFERS_CHANGED)
            {
                ALOGV("INFO_OUTPUT_BUFFERS_CHANGED");
                CHECK_EQ((status_t)OK,
                         state->mCodec->getOutputBuffers(&state->mOutBuffers));

                ALOGV("got %zu output buffers", state->mOutBuffers.size());
            }
            else if (err == INFO_FORMAT_CHANGED)
            {
                sp<AMessage> format;
                CHECK_EQ((status_t)OK, state->mCodec->getOutputFormat(&format));

                ALOGV("INFO_FORMAT_CHANGED: %s", format->debugString().c_str());
            }
            else
            {
                CHECK_EQ(err, -EAGAIN);
            }
        }
    }


    int64_t elapsedTimeUs = ALooper::GetNowUs() - startTimeUs;

    for (size_t i = 0; i < stateByTrack.size(); ++i)
    {
        CodecState *state = &stateByTrack.editValueAt(i);

        CHECK_EQ((status_t)OK, state->mCodec->release());

        if (state->mIsAudio)
        {
            printf("track %zu: %lld bytes received. %.2f KB/sec\n",
                   i,
                   (long long)state->mNumBytesDecoded,
                   state->mNumBytesDecoded * 1E6 / 1024 / elapsedTimeUs);
        }
        else
        {
            printf("track %zu: %lld frames decoded, %.2f fps. %lld"
                    " bytes received. %.2f KB/sec\n",
                   i,
                   (long long)state->mNumBuffersDecoded,
                   state->mNumBuffersDecoded * 1E6 / elapsedTimeUs,
                   (long long)state->mNumBytesDecoded,
                   state->mNumBytesDecoded * 1E6 / 1024 / elapsedTimeUs);
        }
    }


    looper->stop();

    return 0;
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

    OMX_BOOL useAudio = OMX_TRUE;
    OMX_BOOL useVideo = OMX_FALSE;
    //OMX_BOOL playback = OMX_FALSE;
    //OMX_BOOL useSurface = OMX_FALSE;
    OMX_BOOL renderSurface = OMX_FALSE;
    OMX_BOOL useTimestamp = OMX_FALSE;

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

        in_main.useAudio = useAudio;
        in_main.useVideo = useVideo;
        in_main.renderSurface = renderSurface;
        in_main.useTimestamp = useTimestamp;
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

        in_assoc.useAudio = useAudio;
        in_assoc.useVideo = useVideo;
        in_assoc.renderSurface = renderSurface;
        in_assoc.useTimestamp = useTimestamp;
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

        in_sys.useAudio = useAudio;
        in_sys.useVideo = useVideo;
        in_sys.renderSurface = renderSurface;
        in_sys.useTimestamp = useTimestamp;
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

        in_app.useAudio = useAudio;
        in_app.useVideo = useVideo;
        in_app.renderSurface = renderSurface;
        in_app.useTimestamp = useTimestamp;
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

int main(int argc, char **argv)
{
    using namespace android;

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

//====================================//

    ProcessState::self()->startThreadPool();
/*
    DataSource::RegisterDefaultSniffers();

    sp<ALooper> looper = new ALooper;
    looper->start();

//    sp<SurfaceComposerClient> composerClient;
//    sp<SurfaceControl> control;
    sp<Surface> surface;
*/
    //const char *filename = process_graph->user_params.ms_init_args.input_filename[MS_MAIN_DECODER_IDX];


/*
    if (playback) {
        sp<SimplePlayer> player = new SimplePlayer;
        looper->registerHandler(player);

        player->setDataSource(argv[0]);
        //player->setSurface(surface->getIGraphicBufferProducer());
        player->start();
        sleep(60);
        player->stop();
        player->reset();
    } else {

        decode(looper, filename, useAudio, useVideo, surface, renderSurface,
                useTimestamp);
    }
*/

    setup_ms12_decode_process(process_graph);


    if(process_graph->user_params.h_getparam)
        free(process_graph->user_params.h_getparam);
    if(process_graph)
        free(process_graph);
    if(p_extern_static)
        free(p_extern_static);
    if(user_update_params)
        free(user_update_params);

    return 0;
}
