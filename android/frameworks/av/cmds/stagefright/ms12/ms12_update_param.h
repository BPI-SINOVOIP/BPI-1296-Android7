#ifndef MS12_UPDATE_PARAM_H_
#define MS12_UPDATE_PARAM_H_


void param_update_open (param_update_handle h_param_update
                        ,int                 num_nodes
                        ,int                 max_int_vals
                        //,DLB_TIMESTAMP      *p_first_update
                        ,void               *p_mem
                        );

void build_node_param_update_table(param_update_handle h_param_update, MS_USE_CASE *p_use_case);
int retrieve_param(OMX_AUDIO_ms12_update_params *omx_update_params, param_update_handle h_param_update, int node_idx, DLB_TIMESTAMP *time_stamp);


#endif