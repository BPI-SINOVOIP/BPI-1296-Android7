#include "ms12_cmd.h"
#include "ms12_getparam.h"
#include <stdlib.h>
#include <string.h>
#include <media/stagefright/OMXCodec.h>

const param_query a_main_input_param_query_table[MAX_RUNTIME_PARAMS_MAIN_INPUT] =
{{USER_PARAM_PCM_DMX_OUTPUT_ACTIVE,            "dmxon", PARAM_UPDATE_TYPE_INT, 1, 0, "Downmix output active"}
,{USER_PARAM_PCM_DMX_OUTPUT_DUAL_MONO_MODE,       "us", PARAM_UPDATE_TYPE_INT, 1, 0, "Downmix output dual mono reproduction"}
,{USER_PARAM_PCM_DMX_OUTPUT_COMPRESSION_MODE,    "drc", PARAM_UPDATE_TYPE_INT, 1, 0, "Downmix output compression mode"}
,{USER_PARAM_PCM_DMX_OUTPUT_DMX_TYPE,            "dmx", PARAM_UPDATE_TYPE_INT, 1, 0, "Downmix output downmix type"}
,{USER_PARAM_PCM_DMX_OUTPUT_DRC_CUT,              "cs", PARAM_UPDATE_TYPE_INT, 1, 0, "Downmix DRC cut factor"}
,{USER_PARAM_PCM_DMX_OUTPUT_DRC_BOOST,            "bs", PARAM_UPDATE_TYPE_INT, 1, 0, "Downmix DRC boost factor"}
,{USER_PARAM_PCM_MC_OUTPUT_ACTIVE,                "mc", PARAM_UPDATE_TYPE_INT, 1, 0, "Multichannel output active"}
,{USER_PARAM_PCM_MC_OUTPUT_DUAL_MONO_MODE,         "u", PARAM_UPDATE_TYPE_INT, 1, 0, "Multichannel output dual mono reproduction"}
,{USER_PARAM_PCM_MC_OUTPUT_COMPRESSION_MODE, "dap_drc", PARAM_UPDATE_TYPE_INT, 1, 0, "Multichannel output compression mode"}
,{USER_PARAM_PCM_MC_OUTPUT_DRC_CUT,                "c", PARAM_UPDATE_TYPE_INT, 1, 0, "Multichannel DRC cut factor"}
,{USER_PARAM_PCM_MC_OUTPUT_DRC_BOOST,              "b", PARAM_UPDATE_TYPE_INT, 1, 0, "Multichannel DRC boost factor"}
,{USER_PARAM_PCM_MC_OUTPUT_5_1_DMX,       "mc_5_1_dmx", PARAM_UPDATE_TYPE_INT, 1, 0, "Multichannel 7.1 to 5.1 downmix flag"}
,{MD_ID_DDP_SUBSTREAM_SELECT,                     "at", PARAM_UPDATE_TYPE_INT, 1, 0, "DD+ substream selection"}
,{MD_ID_AD_MIXING_ENABLED,                        "xa", PARAM_UPDATE_TYPE_INT, 1, 0, "AD mixing on/off"}
,{MD_ID_AD_MIXING_USER_PREF,                      "xu", PARAM_UPDATE_TYPE_INT, 1, 0, "AD mixing user preference"}
,{MD_ID_APP_MIXING_USER_PREF,                    "xsu", PARAM_UPDATE_TYPE_INT, 1, 0, "Application mixing user preference"}
,{MD_ID_SYSSOUND_MIXING_ENABLED,                  "xs", PARAM_UPDATE_TYPE_INT, 1, 0, "System sound mixing on/off"}
};

/*
const param_query a_extpcm_input_param_query_table[MAX_RUNTIME_PARAMS_EXTPCM_INPUT] =
{{USER_PARAM_EXTPCM_ACMOD,             "chp", PARAM_UPDATE_TYPE_INT, 1, 0, "Exernal PCM acmod"}
,{USER_PARAM_EXTPCM_LFEON,              "lp", PARAM_UPDATE_TYPE_INT, 1, 0, "Exernal PCM LFE flag"}
};*/

const param_query a_sys_sound_input_param_query_table[MAX_RUNTIME_PARAMS_SYSSOUND_INPUT] =
{{USER_PARAM_SYSSOUND_ACMOD,          "chs", PARAM_UPDATE_TYPE_INT, 1, 0, "System Sound acmod"}
,{USER_PARAM_SYSSOUND_LFEON,           "ls", PARAM_UPDATE_TYPE_INT, 1, 0, "System Sound LFE flag"}
};

const param_query a_app_sound_input_param_query_table[MAX_RUNTIME_PARAMS_APPSOUND_INPUT] =
{{USER_PARAM_APPSOUND_ACMOD,          "chas", PARAM_UPDATE_TYPE_INT, 1, 0, "Application Sound acmod"}
,{USER_PARAM_APPSOUND_LFEON,           "las", PARAM_UPDATE_TYPE_INT, 1, 0, "Application Sound LFE flag"}
};

const param_query a_dap_param_query_table[MAX_RUNTIME_PARAMS_DAP] =
{{MD_ID_DAP_SPEAKER_LFE,      "dap_speaker_lfe"            , PARAM_UPDATE_TYPE_INT, 1,  0, "DAP speaker LFE flag"}
,{MD_ID_DAP_DMX_TYPE,         "dap_dmx"                    , PARAM_UPDATE_TYPE_INT, 1,  0, "DAP downmix type"}
,{MD_ID_DAP_GAINS,            "dap_gains"                  , PARAM_UPDATE_TYPE_INT, NUM_DAP_GAIN_PARAM,  0, "DAP gains"}
,{MD_ID_DAP_SURR_DECODER,     "dap_surround_decoder_enable", PARAM_UPDATE_TYPE_INT, 1,  0, "DAP surround decoder enable flag"}
,{MD_ID_DAP_SURR_VIRTUALIZER, "dap_surround_virtualizer"   , PARAM_UPDATE_TYPE_INT, 5,  0, "DAP surround virtualizer"}
,{MD_ID_DAP_MI_STEERING,      "dap_mi_steering"            , PARAM_UPDATE_TYPE_INT, 4,  0, "DAP media intelligence steering flags"}
,{MD_ID_DAP_CAL_BOOST,        "dap_calibration_boost"      , PARAM_UPDATE_TYPE_INT, 1,  0, "DAP calibration boost"}
,{MD_ID_DAP_LEVELER,          "dap_leveler"                , PARAM_UPDATE_TYPE_INT, NUM_DAP_LEVELER_PARAM,  0, "DAP leveler"}
,{MD_ID_DAP_DIALOGUE_ENH,     "dap_dialogue_enhancer"      , PARAM_UPDATE_TYPE_INT, 3,  0, "DAP dialogue_enhancer"}
,{MD_ID_DAP_BASS_ENHANCER,    "dap_bass_enhancer"          , PARAM_UPDATE_TYPE_INT, 4,  0, "DAP bass enhancer"}
,{MD_ID_DAP_VIRTUAL_BASS,     "dap_virtual_bass"           , PARAM_UPDATE_TYPE_INT, 10,  0, "DAP virtual bass"}
,{MD_ID_DAP_IEQ,              "dap_ieq"                    , PARAM_UPDATE_TYPE_INT_VAR, 1, 0, "DAP intelligent equalizer"}
,{MD_ID_DAP_GEQ,              "dap_graphic_eq"             , PARAM_UPDATE_TYPE_INT_VAR, 1, 0, "DAP graphic equalizer"}
,{MD_ID_DAP_OPTIMIZER,        "dap_optimizer"              , PARAM_UPDATE_TYPE_INT_VAR, 1, 0, "DAP optimizer"}
,{MD_ID_DAP_REGULATOR,        "dap_regulator"              , PARAM_UPDATE_TYPE_INT_VAR, 1, 0, "DAP regulator"}
};


int param_update_query_mem(int num_nodes ,int max_int_vals ,unsigned int *p_mem_size)
{
    unsigned int mem_size = 0;

    //mem_size += sizeof(param_update);
    mem_size += sizeof(param_query*) * num_nodes;
    //mem_size += sizeof(node_handle) * num_nodes;
    //mem_size += sizeof(node_set_param_func_ptr) * num_nodes;
    mem_size += sizeof(int) * num_nodes; /* num table entries array */
    mem_size += max_int_vals * sizeof(long int);
    *p_mem_size = mem_size;

    return MSF_OK;
}

void param_update_open (param_update_handle h_param_update
                        ,int                 num_nodes
                        ,int                 max_int_vals
                        //,DLB_TIMESTAMP      *p_first_update
                        ,void               *p_mem
                        )
{
    //param_update_handle h_param_update;
    char *p_mem_ptr = (char*)p_mem;

    //h_param_update = (param_update_handle)p_mem_ptr;
    //p_mem_ptr += sizeof(param_update);

    //h_param_update->ah_node = (node_handle*)p_mem_ptr;
    //p_mem_ptr += sizeof(node_handle) * num_nodes;

    h_param_update->a_table_entries = (int*)p_mem_ptr;
    p_mem_ptr += sizeof(int) * num_nodes;

    h_param_update->ap_param_table = (const param_query**)p_mem_ptr;
    p_mem_ptr += sizeof(param_query*) * num_nodes;

    h_param_update->a_int_retriever = (long int*)p_mem_ptr;
    p_mem_ptr += sizeof(long int) * max_int_vals;

    //h_param_update->a_pfunc_set_param = (node_set_param_func_ptr*)p_mem_ptr;

    /* init defaults */
    h_param_update->curr_num_nodes = 0;
    h_param_update->max_num_nodes = num_nodes;
    h_param_update->max_int_array_len = max_int_vals;
    //h_param_update->h_get_param = h_get_param;
    //h_param_update->time_of_next_update.format = DLB_TIMESTAMP_SAMPLE;
/*
    if(p_first_update->format != DLB_TIMESTAMP_EOF)
    {
        dlb_getparam_timestamp_convert
            (h_param_update->h_get_param
            ,p_first_update
            ,&h_param_update->time_of_next_update
            ,48000
            ,FRAMESIZE
            );
    }
    else
    {
        h_param_update->time_of_next_update = *p_first_update;
    }

    return h_param_update;
*/
}

static int param_update_register_node(param_update_handle     h_param_update
                                  // ,node_handle             h_node
                                      ,int                     num_table_entries
                                      ,const param_query      *param_table
                                  //,node_set_param_func_ptr pfunc_set_param
                                      )
{
    if(h_param_update->curr_num_nodes + 1 > h_param_update->max_num_nodes)
    {
        return MSF_ERROR;
    }

    //h_param_update->ah_node[h_param_update->curr_num_nodes]           = h_node;
    h_param_update->a_table_entries[h_param_update->curr_num_nodes]   = num_table_entries;
    h_param_update->ap_param_table[h_param_update->curr_num_nodes]    = param_table;
    //h_param_update->a_pfunc_set_param[h_param_update->curr_num_nodes] = pfunc_set_param;

    h_param_update->curr_num_nodes++;

    return MSF_OK;
}

static void ms12_put_update_param(OMX_AUDIO_ms12_update_params *omx_update_params, int id, int param_type, int num_bytes, long int *a_int_retriever, DLB_TIMESTAMP *time_stamp)
{
    OMX_AUDIO_param_query *p_param_update =
        (OMX_AUDIO_param_query *)&omx_update_params->param_update_table[omx_update_params->num_param_update];

    long int *p_mem_pool = (long int*)&omx_update_params->static_mem_pool[omx_update_params->mem_pool_total_used_offset];

    p_param_update->apply_start_sample = time_stamp->value[0];
    p_param_update->id = id;
    p_param_update->param_type = param_type;
    p_param_update->num_bytes = num_bytes;
    p_param_update->mem_pool_start_offset = omx_update_params->mem_pool_total_used_offset;

    memcpy(p_mem_pool,a_int_retriever,num_bytes);

    omx_update_params->num_param_update++;
    omx_update_params->mem_pool_total_used_offset += (num_bytes >> 2);//number of integer

    if(omx_update_params->num_param_update > OMX_AUDIO_MS12_MAX_NUM_PARAM_UPDATE)
        printf("[MS12][Err] num param update exceeds thd(16)! %d\n",omx_update_params->num_param_update);
    if(omx_update_params->mem_pool_total_used_offset > OMX_AUDIO_MAX_MS12_UPDATE_PARAM_MEM_SIZE)
        printf("[MS12][Err] mem_pool_used_offset exceeds thd(205)! %d\n",omx_update_params->mem_pool_total_used_offset);

/*=====*
{
    int i;
    long int *p_table = (long int*)&omx_update_params->static_mem_pool[p_param_update->mem_pool_start_offset];
printf("[ms12_put_update_param] %d\n",p_param_update->mem_pool_start_offset);
printf("start sml %d id %d param type %d cnt %d\n",p_param_update->apply_start_sample,id,param_type,num_bytes>>2);
for(i = 0; i<(num_bytes>>2); i++)
printf("[%d] %d, %d\n",i,p_table[i],a_int_retriever[i]);
}
//=====*/
}


int retrieve_param(OMX_AUDIO_ms12_update_params *omx_update_params, param_update_handle h_param_update, int node_idx, DLB_TIMESTAMP *time_stamp)
{
    int curr_entry;
    int n;
    const param_query *p_curr_param_query;
    char *str_param;
    DLB_GETPARAM_RETVAL retval;
    //printf("[YLLIN]update param retrieve params, node %d, len %d\n",node_idx, h_param_update->a_table_entries[node_idx]);

    for(curr_entry = 0; curr_entry < h_param_update->a_table_entries[node_idx]; curr_entry++)
    {
        p_curr_param_query = &h_param_update->ap_param_table[node_idx][curr_entry];
        if(p_curr_param_query->num_values > h_param_update->max_int_array_len)
        {
            fprintf(stderr, "Number of comma seperated integers per parameter exceeds the maximum number of %d!\n", h_param_update->max_int_array_len);
            return MSF_ERROR;
        }
        retval = dlb_getparam_maxlenstring
                    (h_param_update->h_get_param
                    ,p_curr_param_query->switch_name
                    ,(const char**)&str_param
                    ,PARAM_UPDATE_MAX_LINE_LENGTH
                    );

        if((retval == DLB_GETPARAM_UNDEFINED_PARAM) && p_curr_param_query->b_mandatory)
        {
            fprintf(stderr, "%s (-%s) is required but missing!\n", p_curr_param_query->str_name, p_curr_param_query->switch_name);
            return MSF_ERROR;
        }
        else if(retval == DLB_GETPARAM_UNDEFINED_PARAM)
        {
            //printf("[YLLIN] retval = DLB_GETPARAM_UNDEFINED_PARAM\n");
            continue;
        }
        else if(retval == DLB_GETPARAM_ALREADY_GOT)
        {
            fprintf(stderr, "Parameter -%s already been retrieved by another node!\n", p_curr_param_query->switch_name);
            return MSF_ERROR;
        }
        else if(retval != DLB_GETPARAM_OK)
        {
            fprintf(stderr, "Error parsing the command line\n");
            return MSF_ERROR;
        }

        switch(p_curr_param_query->param_type)
        {
        case PARAM_UPDATE_TYPE_STRING:
            printf("[MS12][WARN] PARAM_UPDATE_TYPE_STRING, not implemented yet!!\n");
            //printf("[YLLIN] bf set param, len %d, id %d\n",(strlen(str_param) + 1)*sizeof(char),p_curr_param_query->id);
            //h_param_update->a_pfunc_set_param[node_idx](h_param_update->ah_node[node_idx], p_curr_param_query->id, (strlen(str_param) + 1)*sizeof(char), str_param);
            //ms12_put_update_param(omx_update_params, p_curr_param_query->id, p_curr_param_query->param_type, (strlen(str_param) + 1)*sizeof(char), str_param);
            break;
        case PARAM_UPDATE_TYPE_INT:
        {
            const char *tmpstr;
            char *endstr;
            printf("[YLLIN] PARAM_UPDATE_TYPE_INT\n");

            tmpstr = str_param;
            for(n=0; n<p_curr_param_query->num_values; n++)
            {
                h_param_update->a_int_retriever[n] = strtol(tmpstr, &endstr, 0);
                if(tmpstr == endstr)
                {
                    fprintf(stderr, "Error converting parameter %s to integer!\n", p_curr_param_query->switch_name);
                }
                tmpstr = endstr+1;
                printf("[%d] %d\n",n,(int)h_param_update->a_int_retriever[n]);
            }
            printf("[YLLIN] bf set param, cnt %d, id %d\n",p_curr_param_query->num_values,p_curr_param_query->id);
            //h_param_update->a_pfunc_set_param[node_idx](h_param_update->ah_node[node_idx], p_curr_param_query->id, sizeof(int)*p_curr_param_query->num_values, h_param_update->a_int_retriever);
            ms12_put_update_param(omx_update_params, p_curr_param_query->id, p_curr_param_query->param_type, sizeof(int)*p_curr_param_query->num_values, h_param_update->a_int_retriever, time_stamp);
        }
            break;
        case PARAM_UPDATE_TYPE_INT_VAR:
        {
            const char *tmpstr;
            char *endstr;
            int count = 0;
            printf("[YLLIN] PARAM_UPDATE_TYPE_INT_VAR\n");

            tmpstr = str_param;
            for(n=0; n<h_param_update->max_int_array_len; n++)
            {
                h_param_update->a_int_retriever[n] = strtol(tmpstr, &endstr, 0);
                if(tmpstr == endstr)
                {
                    break;
                }
                tmpstr = endstr+1;
                count++;
            }
            printf("[YLLIN] bf set param, cnt %d, id %d\n",count,p_curr_param_query->id);
            //h_param_update->a_pfunc_set_param[node_idx](h_param_update->ah_node[node_idx], p_curr_param_query->id, sizeof(int) * count, h_param_update->a_int_retriever);
            ms12_put_update_param(omx_update_params, p_curr_param_query->id, p_curr_param_query->param_type, sizeof(int) * count, h_param_update->a_int_retriever, time_stamp);
        }
            break;
        default:
            fprintf(stderr, "Unknown parameter type specified!\n");
            return MSF_ERROR;
        }
    }

    return MSF_OK;
}

void build_node_param_update_table(param_update_handle h_param_update, MS_USE_CASE *p_use_case)
{
    //TODO:
    //if file playback, pfunc_set_param = mp4_file_reader_set_param
    //node_set_param_func_ptr pfunc_set_param = &file_reader_set_param;

    param_update_register_node(h_param_update
                               //,(node_handle)h_parent_node
                               ,MAX_RUNTIME_PARAMS_MAIN_INPUT
                               ,a_main_input_param_query_table);
                               //,pfunc_set_param);

    if((int)p_use_case->dap_init_mode)
    {
        param_update_register_node(h_param_update
                                  //,(node_handle)h_parent_node
                                  ,MAX_RUNTIME_PARAMS_DAP
                                  ,a_dap_param_query_table);
                                  //,pfunc_set_param);
    }
    if(p_use_case->b_app_sounds)
    {
        param_update_register_node(h_param_update
                                   //,h_parent_node2
                                   ,MAX_RUNTIME_PARAMS_APPSOUND_INPUT
                                   ,a_app_sound_input_param_query_table);
                                   //,&file_reader_set_param);

    }


    if(p_use_case->b_system_sounds)
    {
        param_update_register_node(h_param_update
                                   //,h_parent_node3
                                   ,MAX_RUNTIME_PARAMS_SYSSOUND_INPUT
                                   ,a_sys_sound_input_param_query_table);
                                   //,&file_reader_set_param);

    }
}