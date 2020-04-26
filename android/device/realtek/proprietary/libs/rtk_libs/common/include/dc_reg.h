/**************************************************************
// Spec Version                  : 0.0.8
// Parser Version                : DVR_Parser_5.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name_reg
// Firmware Header Generate Date : 2010/5/14
***************************************************************/


#ifndef _DC_SYS_REG_H_INCLUDED_
#define _DC_SYS_REG_H_INCLUDED_
#ifdef  _DC_SYS_USE_STRUCT
typedef struct 
{
unsigned int     pict_set_num:8;
unsigned int     pict_set_rd:1;
unsigned int     pict_width_scale:6;
unsigned int     pict_init_page:17;
}DC_PICT_SET;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     dram_8g_used:1;
unsigned int     dram_4g_used:1;
unsigned int     ddr3_512_used:1;
unsigned int     dram_2g_used:1;
unsigned int     pre_arb_scheme_sel:1;
unsigned int     ve_cmd_grp_disable:1;
unsigned int     ddr3_mapping_en:1;
unsigned int     reserved_1:1;
unsigned int     self_lookback_mode:1;
unsigned int     mem_num:1;
unsigned int     pict_offset_en:1;
unsigned int     page_size:2;
unsigned int     pre_arb_rd_cnt:8;
unsigned int     pre_arb_wr_cnt:8;
}DC_SYS_MISC;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     sb1_high_pri:11;
unsigned int     ao_pri:11;
}PRIORITY_CTRL_0;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     sb2_high_pri:11;
unsigned int     vo_high_pri:11;
}PRIORITY_CTRL_1;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     vo_low_pri:11;
unsigned int     sb3_pri:11;
}PRIORITY_CTRL_2;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     se_pri:11;
unsigned int     sb1_low_pri:11;
}PRIORITY_CTRL_3;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     ve_pri:11;
unsigned int     sb2_low_pri:11;
}PRIORITY_CTRL_4;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     gde_pri:11;
}PRIORITY_CTRL_5;

typedef struct 
{
unsigned int     split_wait:4;
unsigned int     arbit_los:4;
unsigned int     reserved_0:1;
unsigned int     short_lxy:7;
unsigned int     live_split:8;
unsigned int     reserved_1:6;
unsigned int     live_cflit:1;
unsigned int     queue_flush:1;
}DC_PROS_CTRL;

typedef struct 
{
unsigned int     pict_set_num:8;
unsigned int     pict_offset_rd:1;
unsigned int     reserved_0:3;
unsigned int     pict_set_offset_y:10;
unsigned int     pict_set_offset_x:10;
}DC_PICT_SET_OFFSET;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     req_pending:1;
unsigned int     req_id:11;
}DC_REQ_STATUS;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     full_cmd_count:3;
unsigned int     pre_tra_rd_cnt:10;
unsigned int     pre_tra_wr_cnt:10;
}DC_SYS_MISC2;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     pc_go:1;
}DC_PC_CTRL;

typedef struct 
{
unsigned int     pc_tota_mon_num:32;
}DC_PC_TOTA_MON_NUM;

typedef struct 
{
unsigned int     pc_tota_ack_num:32;
}DC_PC_TOTA_ACK_NUM;

typedef struct 
{
unsigned int     pc_tota_idl_num:32;
}DC_PC_TOTA_IDL_NUM;

typedef struct 
{
unsigned int     pc_ve_acc_lat:32;
}DC_PC_VE_ACC_LAT;

typedef struct 
{
unsigned int     pc_vo_acc_lat:32;
}DC_PC_VO_ACC_LAT;

typedef struct 
{
unsigned int     pc_sb2_acc_lat:32;
}DC_PC_SB2_ACC_LAT;

typedef struct 
{
unsigned int     pc_prog_0_acc_lat:32;
}DC_PC_PROG_0_ACC_LAT;

typedef struct 
{
unsigned int     pc_prog_1_acc_lat:32;
}DC_PC_PROG_1_ACC_LAT;

typedef struct 
{
unsigned int     pc_prog_2_acc_lat:32;
}DC_PC_PROG_2_ACC_LAT;

typedef struct 
{
unsigned int     pc_200_prog_0_acc_lat:32;
}DC_PC_200_PROG_0_ACC_LAT;

typedef struct 
{
unsigned int     pc_200_prog_1_acc_lat:32;
}DC_PC_200_PROG_1_ACC_LAT;

typedef struct 
{
unsigned int     pc_gpu_acc_lat:32;
}DC_PC_GPU_ACC_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_ve_max_lat:16;
}DC_PC_VE_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_vo_max_lat:16;
}DC_PC_VO_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_sb2_max_lat:16;
}DC_PC_SB2_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_prog_0_max_lat:16;
}DC_PC_PROG_0_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_prog_1_max_lat:16;
}DC_PC_PROG_1_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_prog_2_max_lat:16;
}DC_PC_PROG_2_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_gpu_max_lat:16;
}DC_PC_GPU_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_200_prog_0_max_lat:16;
}DC_PC_200_PROG_0_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pc_200_prog_1_max_lat:16;
}DC_PC_200_PROG_1_MAX_LAT;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_ve_req_num:24;
}DC_PC_VE_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_vo_req_num:24;
}DC_PC_VO_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_sb2_req_num:24;
}DC_PC_SB2_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_prog_0_req_num:24;
}DC_PC_PROG_0_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_prog_1_req_num:24;
}DC_PC_PROG_1_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_prog_2_req_num:24;
}DC_PC_PROG_2_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_gpu_req_num:24;
}DC_PC_GPU_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_200_prog_0_req_num:24;
}DC_PC_200_PROG_0_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     pc_200_prog_1_req_num:24;
}DC_PC_200_PROG_1_REQ_NUM;

typedef struct 
{
unsigned int     pc_ve_ack_num:32;
}DC_PC_VE_ACK_NUM;

typedef struct 
{
unsigned int     pc_vo_ack_num:32;
}DC_PC_VO_ACK_NUM;

typedef struct 
{
unsigned int     pc_sb2_ack_num:32;
}DC_PC_SB2_ACK_NUM;

typedef struct 
{
unsigned int     pc_prog_0_ack_num:32;
}DC_PC_PROG_0_ACK_NUM;

typedef struct 
{
unsigned int     pc_prog_1_ack_num:32;
}DC_PC_PROG_1_ACK_NUM;

typedef struct 
{
unsigned int     pc_prog_2_ack_num:32;
}DC_PC_PROG_2_ACK_NUM;

typedef struct 
{
unsigned int     pc_gpu_ack_num:32;
}DC_PC_GPU_ACK_NUM;

typedef struct 
{
unsigned int     pc_200_prog_0_ack_num:32;
}DC_PC_200_PROG_0_ACK_NUM;

typedef struct 
{
unsigned int     pc_200_prog_1_ack_num:32;
}DC_PC_200_PROG_1_ACK_NUM;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     write_en_6:1;
unsigned int     pc_prog_2_sel_dc_level:1;
unsigned int     write_en_5:1;
unsigned int     pc_prog_1_sel_dc_level:1;
unsigned int     write_en_4:1;
unsigned int     pc_prog_0_sel_dc_level:1;
unsigned int     reserved_1:1;
unsigned int     write_en_3:1;
unsigned int     pc_prog_2_sel:6;
unsigned int     reserved_2:1;
unsigned int     write_en_2:1;
unsigned int     pc_prog_1_sel:6;
unsigned int     reserved_3:1;
unsigned int     write_en_1:1;
unsigned int     pc_prog_0_sel:6;
}DC_PC_PROG_CTRL;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     write_en_2:1;
unsigned int     pc_200_prog_1_sel:6;
unsigned int     reserved_1:1;
unsigned int     write_en_1:1;
unsigned int     pc_200_prog_0_sel:6;
}DC_PC_200_PROG_CTRL;

typedef struct 
{
unsigned int     sfmode_pattern0:32;
}SFMODE_PATTERN0;

typedef struct 
{
unsigned int     sfmode_pattern1:32;
}SFMODE_PATTERN1;

typedef struct 
{
unsigned int     sfmode_pattern2:32;
}SFMODE_PATTERN2;

typedef struct 
{
unsigned int     sfmode_pattern3:32;
}SFMODE_PATTERN3;

typedef struct 
{
unsigned int     sfmode_cmp_rlt0:32;
}SFMODE_CMP_RLT0;

typedef struct 
{
unsigned int     sfmode_cmp_rlt1:32;
}SFMODE_CMP_RLT1;

typedef struct 
{
unsigned int     sfmode_cmp_rlt2:32;
}SFMODE_CMP_RLT2;

typedef struct 
{
unsigned int     sfmode_cmp_rlt3:32;
}SFMODE_CMP_RLT3;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     disable_2nd_channel:1;
unsigned int     full_transfer:1;
}DC_64_WRITE_BUF_CTRL;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     sync_cmd_cnt_threshold:3;
unsigned int     cmd_buf_threshold:4;
unsigned int     disable_2nd_channel:1;
unsigned int     ever_full:1;
unsigned int     ever_reach_threshold:1;
unsigned int     max_buf_level:7;
unsigned int     grant_stop_level:7;
}DC_64_READ_BUF_CTRL;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     pict_set_int_cpu:2;
unsigned int     int_enable:1;
unsigned int     ever_set_as_odd:1;
unsigned int     enable:1;
}DC_PIC_OFFSET_X_DETECTION;

typedef struct 
{
unsigned int     dummy_reg0:32;
}DUMMY_REG0;

typedef struct 
{
unsigned int     dummy_reg1:32;
}DUMMY_REG1;

typedef struct 
{
unsigned int     dummy_reg2:32;
}DUMMY_REG2;

typedef struct 
{
unsigned int     dummy_reg3:32;
}DUMMY_REG3;

typedef struct 
{
unsigned int     mem_type:2;
unsigned int     saddr:26;
unsigned int     reserved_0:4;
}DC_MT_SADDR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     eaddr:26;
unsigned int     reserved_1:5;
}DC_MT_EADDR;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     write_enable1:1;
unsigned int     int1:1;
unsigned int     write_enable2:1;
unsigned int     int2:1;
unsigned int     write_enable3:1;
unsigned int     int3:1;
unsigned int     write_enable4:1;
unsigned int     cpu:2;
unsigned int     int1_en:1;
unsigned int     int2_en:1;
unsigned int     int3_en:1;
unsigned int     write_enable5:1;
unsigned int     mode:2;
}DC_MT_MODE;

typedef struct 
{
unsigned int     module_ID_0:6;
unsigned int     access_type_0:2;
unsigned int     module_ID_1:6;
unsigned int     access_type_1:2;
unsigned int     module_ID_2:6;
unsigned int     access_type_2:2;
unsigned int     module_ID_3:6;
unsigned int     access_type_3:2;
}DC_MT_TABLE;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     ever_trap:1;
unsigned int     module_ID:6;
unsigned int     addcmd:14;
}DC_MT_ADDCMD_HI;

typedef struct 
{
unsigned int     addcmd:32;
}DC_MT_ADDCMD_LO;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     write_enable4:1;
unsigned int     error_cmd_int_cpu:2;
unsigned int     write_enable3:1;
unsigned int     error_cmd_int_en:1;
unsigned int     write_enable2:1;
unsigned int     error_cmd_swap_en:1;
unsigned int     write_enable1:1;
unsigned int     error_cmd_detection_en:1;
}DC_EC_CTRL;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     error_cmd_int:1;
unsigned int     error_cmd_ever_trap:1;
unsigned int     trap_case:8;
unsigned int     Addcmd_hi:20;
}DC_EC_ADDCMD_HI;

typedef struct 
{
unsigned int     Addcmd_lo:32;
}DC_EC_ADDCMD_LO;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     write_enable3:1;
unsigned int     tag_check_int_cpu:2;
unsigned int     write_enable2:1;
unsigned int     tag_check_int_en:1;
unsigned int     write_enable1:1;
unsigned int     tag_check_en:1;
}DC_RD_TAG_CHECK;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     rd_tag_mismatch_ever:1;
unsigned int     rd_tag_mismatch_int:1;
}DC_RD_TAG_CHECK_ST;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     mem_protect_en:1;
}DC_MEM_PROTECT_CTRL;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     saddr:29;
}DC_MEM_PROTECT_SADDR;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     eaddr:29;
}DC_MEM_PROTECT_EADDR;

#endif

#define DC_PICT_SET                                                                  0x18008000
#define DC_PICT_SET_reg_addr                                                         "0xB8008000"
#define DC_PICT_SET_reg                                                              0xB8008000
#define set_DC_PICT_SET_reg(data)   (*((volatile unsigned int*) DC_PICT_SET_reg)=data)
#define get_DC_PICT_SET_reg   (*((volatile unsigned int*) DC_PICT_SET_reg))
#define DC_PICT_SET_inst_adr                                                         "0x0000"
#define DC_PICT_SET_inst                                                             0x0000
#define DC_PICT_SET_pict_set_num_shift                                               (24)
#define DC_PICT_SET_pict_set_num_mask                                                (0xFF000000)
#define DC_PICT_SET_pict_set_num(data)                                               (0xFF000000&((data)<<24))
#define DC_PICT_SET_pict_set_num_src(data)                                           ((0xFF000000&(data))>>24)
#define DC_PICT_SET_get_pict_set_num(data)                                           ((0xFF000000&(data))>>24)
#define DC_PICT_SET_pict_set_rd_shift                                                (23)
#define DC_PICT_SET_pict_set_rd_mask                                                 (0x00800000)
#define DC_PICT_SET_pict_set_rd(data)                                                (0x00800000&((data)<<23))
#define DC_PICT_SET_pict_set_rd_src(data)                                            ((0x00800000&(data))>>23)
#define DC_PICT_SET_get_pict_set_rd(data)                                            ((0x00800000&(data))>>23)
#define DC_PICT_SET_pict_width_scale_shift                                           (17)
#define DC_PICT_SET_pict_width_scale_mask                                            (0x007E0000)
#define DC_PICT_SET_pict_width_scale(data)                                           (0x007E0000&((data)<<17))
#define DC_PICT_SET_pict_width_scale_src(data)                                       ((0x007E0000&(data))>>17)
#define DC_PICT_SET_get_pict_width_scale(data)                                       ((0x007E0000&(data))>>17)
#define DC_PICT_SET_pict_init_page_shift                                             (0)
#define DC_PICT_SET_pict_init_page_mask                                              (0x0001FFFF)
#define DC_PICT_SET_pict_init_page(data)                                             (0x0001FFFF&((data)<<0))
#define DC_PICT_SET_pict_init_page_src(data)                                         ((0x0001FFFF&(data))>>0)
#define DC_PICT_SET_get_pict_init_page(data)                                         ((0x0001FFFF&(data))>>0)


#define DC_SYS_MISC                                                                  0x18008004
#define DC_SYS_MISC_reg_addr                                                         "0xB8008004"
#define DC_SYS_MISC_reg                                                              0xB8008004
#define set_DC_SYS_MISC_reg(data)   (*((volatile unsigned int*) DC_SYS_MISC_reg)=data)
#define get_DC_SYS_MISC_reg   (*((volatile unsigned int*) DC_SYS_MISC_reg))
#define DC_SYS_MISC_inst_adr                                                         "0x0001"
#define DC_SYS_MISC_inst                                                             0x0001
#define DC_SYS_MISC_dram_8g_used_shift                                               (28)
#define DC_SYS_MISC_dram_8g_used_mask                                                (0x10000000)
#define DC_SYS_MISC_dram_8g_used(data)                                               (0x10000000&((data)<<28))
#define DC_SYS_MISC_dram_8g_used_src(data)                                           ((0x10000000&(data))>>28)
#define DC_SYS_MISC_get_dram_8g_used(data)                                           ((0x10000000&(data))>>28)
#define DC_SYS_MISC_dram_4g_used_shift                                               (27)
#define DC_SYS_MISC_dram_4g_used_mask                                                (0x08000000)
#define DC_SYS_MISC_dram_4g_used(data)                                               (0x08000000&((data)<<27))
#define DC_SYS_MISC_dram_4g_used_src(data)                                           ((0x08000000&(data))>>27)
#define DC_SYS_MISC_get_dram_4g_used(data)                                           ((0x08000000&(data))>>27)
#define DC_SYS_MISC_ddr3_512_used_shift                                              (26)
#define DC_SYS_MISC_ddr3_512_used_mask                                               (0x04000000)
#define DC_SYS_MISC_ddr3_512_used(data)                                              (0x04000000&((data)<<26))
#define DC_SYS_MISC_ddr3_512_used_src(data)                                          ((0x04000000&(data))>>26)
#define DC_SYS_MISC_get_ddr3_512_used(data)                                          ((0x04000000&(data))>>26)
#define DC_SYS_MISC_dram_2g_used_shift                                               (25)
#define DC_SYS_MISC_dram_2g_used_mask                                                (0x02000000)
#define DC_SYS_MISC_dram_2g_used(data)                                               (0x02000000&((data)<<25))
#define DC_SYS_MISC_dram_2g_used_src(data)                                           ((0x02000000&(data))>>25)
#define DC_SYS_MISC_get_dram_2g_used(data)                                           ((0x02000000&(data))>>25)
#define DC_SYS_MISC_pre_arb_scheme_sel_shift                                         (24)
#define DC_SYS_MISC_pre_arb_scheme_sel_mask                                          (0x01000000)
#define DC_SYS_MISC_pre_arb_scheme_sel(data)                                         (0x01000000&((data)<<24))
#define DC_SYS_MISC_pre_arb_scheme_sel_src(data)                                     ((0x01000000&(data))>>24)
#define DC_SYS_MISC_get_pre_arb_scheme_sel(data)                                     ((0x01000000&(data))>>24)
#define DC_SYS_MISC_ve_cmd_grp_disable_shift                                         (23)
#define DC_SYS_MISC_ve_cmd_grp_disable_mask                                          (0x00800000)
#define DC_SYS_MISC_ve_cmd_grp_disable(data)                                         (0x00800000&((data)<<23))
#define DC_SYS_MISC_ve_cmd_grp_disable_src(data)                                     ((0x00800000&(data))>>23)
#define DC_SYS_MISC_get_ve_cmd_grp_disable(data)                                     ((0x00800000&(data))>>23)
#define DC_SYS_MISC_ddr3_mapping_en_shift                                            (22)
#define DC_SYS_MISC_ddr3_mapping_en_mask                                             (0x00400000)
#define DC_SYS_MISC_ddr3_mapping_en(data)                                            (0x00400000&((data)<<22))
#define DC_SYS_MISC_ddr3_mapping_en_src(data)                                        ((0x00400000&(data))>>22)
#define DC_SYS_MISC_get_ddr3_mapping_en(data)                                        ((0x00400000&(data))>>22)
#define DC_SYS_MISC_self_lookback_mode_shift                                         (20)
#define DC_SYS_MISC_self_lookback_mode_mask                                          (0x00100000)
#define DC_SYS_MISC_self_lookback_mode(data)                                         (0x00100000&((data)<<20))
#define DC_SYS_MISC_self_lookback_mode_src(data)                                     ((0x00100000&(data))>>20)
#define DC_SYS_MISC_get_self_lookback_mode(data)                                     ((0x00100000&(data))>>20)
#define DC_SYS_MISC_mem_num_shift                                                    (19)
#define DC_SYS_MISC_mem_num_mask                                                     (0x00080000)
#define DC_SYS_MISC_mem_num(data)                                                    (0x00080000&((data)<<19))
#define DC_SYS_MISC_mem_num_src(data)                                                ((0x00080000&(data))>>19)
#define DC_SYS_MISC_get_mem_num(data)                                                ((0x00080000&(data))>>19)
#define DC_SYS_MISC_pict_offset_en_shift                                             (18)
#define DC_SYS_MISC_pict_offset_en_mask                                              (0x00040000)
#define DC_SYS_MISC_pict_offset_en(data)                                             (0x00040000&((data)<<18))
#define DC_SYS_MISC_pict_offset_en_src(data)                                         ((0x00040000&(data))>>18)
#define DC_SYS_MISC_get_pict_offset_en(data)                                         ((0x00040000&(data))>>18)
#define DC_SYS_MISC_page_size_shift                                                  (16)
#define DC_SYS_MISC_page_size_mask                                                   (0x00030000)
#define DC_SYS_MISC_page_size(data)                                                  (0x00030000&((data)<<16))
#define DC_SYS_MISC_page_size_src(data)                                              ((0x00030000&(data))>>16)
#define DC_SYS_MISC_get_page_size(data)                                              ((0x00030000&(data))>>16)
#define DC_SYS_MISC_pre_arb_rd_cnt_shift                                             (8)
#define DC_SYS_MISC_pre_arb_rd_cnt_mask                                              (0x0000FF00)
#define DC_SYS_MISC_pre_arb_rd_cnt(data)                                             (0x0000FF00&((data)<<8))
#define DC_SYS_MISC_pre_arb_rd_cnt_src(data)                                         ((0x0000FF00&(data))>>8)
#define DC_SYS_MISC_get_pre_arb_rd_cnt(data)                                         ((0x0000FF00&(data))>>8)
#define DC_SYS_MISC_pre_arb_wr_cnt_shift                                             (0)
#define DC_SYS_MISC_pre_arb_wr_cnt_mask                                              (0x000000FF)
#define DC_SYS_MISC_pre_arb_wr_cnt(data)                                             (0x000000FF&((data)<<0))
#define DC_SYS_MISC_pre_arb_wr_cnt_src(data)                                         ((0x000000FF&(data))>>0)
#define DC_SYS_MISC_get_pre_arb_wr_cnt(data)                                         ((0x000000FF&(data))>>0)


#define PRIORITY_CTRL_0                                                              0x18008008
#define PRIORITY_CTRL_0_reg_addr                                                     "0xB8008008"
#define PRIORITY_CTRL_0_reg                                                          0xB8008008
#define set_PRIORITY_CTRL_0_reg(data)   (*((volatile unsigned int*) PRIORITY_CTRL_0_reg)=data)
#define get_PRIORITY_CTRL_0_reg   (*((volatile unsigned int*) PRIORITY_CTRL_0_reg))
#define PRIORITY_CTRL_0_inst_adr                                                     "0x0002"
#define PRIORITY_CTRL_0_inst                                                         0x0002
#define PRIORITY_CTRL_0_sb1_high_pri_shift                                           (11)
#define PRIORITY_CTRL_0_sb1_high_pri_mask                                            (0x003FF800)
#define PRIORITY_CTRL_0_sb1_high_pri(data)                                           (0x003FF800&((data)<<11))
#define PRIORITY_CTRL_0_sb1_high_pri_src(data)                                       ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_0_get_sb1_high_pri(data)                                       ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_0_ao_pri_shift                                                 (0)
#define PRIORITY_CTRL_0_ao_pri_mask                                                  (0x000007FF)
#define PRIORITY_CTRL_0_ao_pri(data)                                                 (0x000007FF&((data)<<0))
#define PRIORITY_CTRL_0_ao_pri_src(data)                                             ((0x000007FF&(data))>>0)
#define PRIORITY_CTRL_0_get_ao_pri(data)                                             ((0x000007FF&(data))>>0)


#define PRIORITY_CTRL_1                                                              0x1800800C
#define PRIORITY_CTRL_1_reg_addr                                                     "0xB800800C"
#define PRIORITY_CTRL_1_reg                                                          0xB800800C
#define set_PRIORITY_CTRL_1_reg(data)   (*((volatile unsigned int*) PRIORITY_CTRL_1_reg)=data)
#define get_PRIORITY_CTRL_1_reg   (*((volatile unsigned int*) PRIORITY_CTRL_1_reg))
#define PRIORITY_CTRL_1_inst_adr                                                     "0x0003"
#define PRIORITY_CTRL_1_inst                                                         0x0003
#define PRIORITY_CTRL_1_sb2_high_pri_shift                                           (11)
#define PRIORITY_CTRL_1_sb2_high_pri_mask                                            (0x003FF800)
#define PRIORITY_CTRL_1_sb2_high_pri(data)                                           (0x003FF800&((data)<<11))
#define PRIORITY_CTRL_1_sb2_high_pri_src(data)                                       ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_1_get_sb2_high_pri(data)                                       ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_1_vo_high_pri_shift                                            (0)
#define PRIORITY_CTRL_1_vo_high_pri_mask                                             (0x000007FF)
#define PRIORITY_CTRL_1_vo_high_pri(data)                                            (0x000007FF&((data)<<0))
#define PRIORITY_CTRL_1_vo_high_pri_src(data)                                        ((0x000007FF&(data))>>0)
#define PRIORITY_CTRL_1_get_vo_high_pri(data)                                        ((0x000007FF&(data))>>0)


#define PRIORITY_CTRL_2                                                              0x18008010
#define PRIORITY_CTRL_2_reg_addr                                                     "0xB8008010"
#define PRIORITY_CTRL_2_reg                                                          0xB8008010
#define set_PRIORITY_CTRL_2_reg(data)   (*((volatile unsigned int*) PRIORITY_CTRL_2_reg)=data)
#define get_PRIORITY_CTRL_2_reg   (*((volatile unsigned int*) PRIORITY_CTRL_2_reg))
#define PRIORITY_CTRL_2_inst_adr                                                     "0x0004"
#define PRIORITY_CTRL_2_inst                                                         0x0004
#define PRIORITY_CTRL_2_vo_low_pri_shift                                             (11)
#define PRIORITY_CTRL_2_vo_low_pri_mask                                              (0x003FF800)
#define PRIORITY_CTRL_2_vo_low_pri(data)                                             (0x003FF800&((data)<<11))
#define PRIORITY_CTRL_2_vo_low_pri_src(data)                                         ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_2_get_vo_low_pri(data)                                         ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_2_sb3_pri_shift                                                (0)
#define PRIORITY_CTRL_2_sb3_pri_mask                                                 (0x000007FF)
#define PRIORITY_CTRL_2_sb3_pri(data)                                                (0x000007FF&((data)<<0))
#define PRIORITY_CTRL_2_sb3_pri_src(data)                                            ((0x000007FF&(data))>>0)
#define PRIORITY_CTRL_2_get_sb3_pri(data)                                            ((0x000007FF&(data))>>0)


#define PRIORITY_CTRL_3                                                              0x18008014
#define PRIORITY_CTRL_3_reg_addr                                                     "0xB8008014"
#define PRIORITY_CTRL_3_reg                                                          0xB8008014
#define set_PRIORITY_CTRL_3_reg(data)   (*((volatile unsigned int*) PRIORITY_CTRL_3_reg)=data)
#define get_PRIORITY_CTRL_3_reg   (*((volatile unsigned int*) PRIORITY_CTRL_3_reg))
#define PRIORITY_CTRL_3_inst_adr                                                     "0x0005"
#define PRIORITY_CTRL_3_inst                                                         0x0005
#define PRIORITY_CTRL_3_se_pri_shift                                                 (11)
#define PRIORITY_CTRL_3_se_pri_mask                                                  (0x003FF800)
#define PRIORITY_CTRL_3_se_pri(data)                                                 (0x003FF800&((data)<<11))
#define PRIORITY_CTRL_3_se_pri_src(data)                                             ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_3_get_se_pri(data)                                             ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_3_sb1_low_pri_shift                                            (0)
#define PRIORITY_CTRL_3_sb1_low_pri_mask                                             (0x000007FF)
#define PRIORITY_CTRL_3_sb1_low_pri(data)                                            (0x000007FF&((data)<<0))
#define PRIORITY_CTRL_3_sb1_low_pri_src(data)                                        ((0x000007FF&(data))>>0)
#define PRIORITY_CTRL_3_get_sb1_low_pri(data)                                        ((0x000007FF&(data))>>0)


#define PRIORITY_CTRL_4                                                              0x18008018
#define PRIORITY_CTRL_4_reg_addr                                                     "0xB8008018"
#define PRIORITY_CTRL_4_reg                                                          0xB8008018
#define set_PRIORITY_CTRL_4_reg(data)   (*((volatile unsigned int*) PRIORITY_CTRL_4_reg)=data)
#define get_PRIORITY_CTRL_4_reg   (*((volatile unsigned int*) PRIORITY_CTRL_4_reg))
#define PRIORITY_CTRL_4_inst_adr                                                     "0x0006"
#define PRIORITY_CTRL_4_inst                                                         0x0006
#define PRIORITY_CTRL_4_ve_pri_shift                                                 (11)
#define PRIORITY_CTRL_4_ve_pri_mask                                                  (0x003FF800)
#define PRIORITY_CTRL_4_ve_pri(data)                                                 (0x003FF800&((data)<<11))
#define PRIORITY_CTRL_4_ve_pri_src(data)                                             ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_4_get_ve_pri(data)                                             ((0x003FF800&(data))>>11)
#define PRIORITY_CTRL_4_sb2_low_pri_shift                                            (0)
#define PRIORITY_CTRL_4_sb2_low_pri_mask                                             (0x000007FF)
#define PRIORITY_CTRL_4_sb2_low_pri(data)                                            (0x000007FF&((data)<<0))
#define PRIORITY_CTRL_4_sb2_low_pri_src(data)                                        ((0x000007FF&(data))>>0)
#define PRIORITY_CTRL_4_get_sb2_low_pri(data)                                        ((0x000007FF&(data))>>0)


#define PRIORITY_CTRL_5                                                              0x1800801C
#define PRIORITY_CTRL_5_reg_addr                                                     "0xB800801C"
#define PRIORITY_CTRL_5_reg                                                          0xB800801C
#define set_PRIORITY_CTRL_5_reg(data)   (*((volatile unsigned int*) PRIORITY_CTRL_5_reg)=data)
#define get_PRIORITY_CTRL_5_reg   (*((volatile unsigned int*) PRIORITY_CTRL_5_reg))
#define PRIORITY_CTRL_5_inst_adr                                                     "0x0007"
#define PRIORITY_CTRL_5_inst                                                         0x0007
#define PRIORITY_CTRL_5_gde_pri_shift                                                (0)
#define PRIORITY_CTRL_5_gde_pri_mask                                                 (0x000007FF)
#define PRIORITY_CTRL_5_gde_pri(data)                                                (0x000007FF&((data)<<0))
#define PRIORITY_CTRL_5_gde_pri_src(data)                                            ((0x000007FF&(data))>>0)
#define PRIORITY_CTRL_5_get_gde_pri(data)                                            ((0x000007FF&(data))>>0)


#define DC_PROS_CTRL                                                                 0x180087EC
#define DC_PROS_CTRL_reg_addr                                                        "0xB80087EC"
#define DC_PROS_CTRL_reg                                                             0xB80087EC
#define set_DC_PROS_CTRL_reg(data)   (*((volatile unsigned int*) DC_PROS_CTRL_reg)=data)
#define get_DC_PROS_CTRL_reg   (*((volatile unsigned int*) DC_PROS_CTRL_reg))
#define DC_PROS_CTRL_inst_adr                                                        "0x00FB"
#define DC_PROS_CTRL_inst                                                            0x00FB
#define DC_PROS_CTRL_split_wait_shift                                                (28)
#define DC_PROS_CTRL_split_wait_mask                                                 (0xF0000000)
#define DC_PROS_CTRL_split_wait(data)                                                (0xF0000000&((data)<<28))
#define DC_PROS_CTRL_split_wait_src(data)                                            ((0xF0000000&(data))>>28)
#define DC_PROS_CTRL_get_split_wait(data)                                            ((0xF0000000&(data))>>28)
#define DC_PROS_CTRL_arbit_los_shift                                                 (24)
#define DC_PROS_CTRL_arbit_los_mask                                                  (0x0F000000)
#define DC_PROS_CTRL_arbit_los(data)                                                 (0x0F000000&((data)<<24))
#define DC_PROS_CTRL_arbit_los_src(data)                                             ((0x0F000000&(data))>>24)
#define DC_PROS_CTRL_get_arbit_los(data)                                             ((0x0F000000&(data))>>24)
#define DC_PROS_CTRL_short_lxy_shift                                                 (16)
#define DC_PROS_CTRL_short_lxy_mask                                                  (0x007F0000)
#define DC_PROS_CTRL_short_lxy(data)                                                 (0x007F0000&((data)<<16))
#define DC_PROS_CTRL_short_lxy_src(data)                                             ((0x007F0000&(data))>>16)
#define DC_PROS_CTRL_get_short_lxy(data)                                             ((0x007F0000&(data))>>16)
#define DC_PROS_CTRL_live_split_shift                                                (8)
#define DC_PROS_CTRL_live_split_mask                                                 (0x0000FF00)
#define DC_PROS_CTRL_live_split(data)                                                (0x0000FF00&((data)<<8))
#define DC_PROS_CTRL_live_split_src(data)                                            ((0x0000FF00&(data))>>8)
#define DC_PROS_CTRL_get_live_split(data)                                            ((0x0000FF00&(data))>>8)
#define DC_PROS_CTRL_live_cflit_shift                                                (1)
#define DC_PROS_CTRL_live_cflit_mask                                                 (0x00000002)
#define DC_PROS_CTRL_live_cflit(data)                                                (0x00000002&((data)<<1))
#define DC_PROS_CTRL_live_cflit_src(data)                                            ((0x00000002&(data))>>1)
#define DC_PROS_CTRL_get_live_cflit(data)                                            ((0x00000002&(data))>>1)
#define DC_PROS_CTRL_queue_flush_shift                                               (0)
#define DC_PROS_CTRL_queue_flush_mask                                                (0x00000001)
#define DC_PROS_CTRL_queue_flush(data)                                               (0x00000001&((data)<<0))
#define DC_PROS_CTRL_queue_flush_src(data)                                           ((0x00000001&(data))>>0)
#define DC_PROS_CTRL_get_queue_flush(data)                                           ((0x00000001&(data))>>0)


#define DC_PICT_SET_OFFSET                                                           0x18008020
#define DC_PICT_SET_OFFSET_reg_addr                                                  "0xB8008020"
#define DC_PICT_SET_OFFSET_reg                                                       0xB8008020
#define set_DC_PICT_SET_OFFSET_reg(data)   (*((volatile unsigned int*) DC_PICT_SET_OFFSET_reg)=data)
#define get_DC_PICT_SET_OFFSET_reg   (*((volatile unsigned int*) DC_PICT_SET_OFFSET_reg))
#define DC_PICT_SET_OFFSET_inst_adr                                                  "0x0008"
#define DC_PICT_SET_OFFSET_inst                                                      0x0008
#define DC_PICT_SET_OFFSET_pict_set_num_shift                                        (24)
#define DC_PICT_SET_OFFSET_pict_set_num_mask                                         (0xFF000000)
#define DC_PICT_SET_OFFSET_pict_set_num(data)                                        (0xFF000000&((data)<<24))
#define DC_PICT_SET_OFFSET_pict_set_num_src(data)                                    ((0xFF000000&(data))>>24)
#define DC_PICT_SET_OFFSET_get_pict_set_num(data)                                    ((0xFF000000&(data))>>24)
#define DC_PICT_SET_OFFSET_pict_offset_rd_shift                                      (23)
#define DC_PICT_SET_OFFSET_pict_offset_rd_mask                                       (0x00800000)
#define DC_PICT_SET_OFFSET_pict_offset_rd(data)                                      (0x00800000&((data)<<23))
#define DC_PICT_SET_OFFSET_pict_offset_rd_src(data)                                  ((0x00800000&(data))>>23)
#define DC_PICT_SET_OFFSET_get_pict_offset_rd(data)                                  ((0x00800000&(data))>>23)
#define DC_PICT_SET_OFFSET_pict_set_offset_y_shift                                   (10)
#define DC_PICT_SET_OFFSET_pict_set_offset_y_mask                                    (0x000FFC00)
#define DC_PICT_SET_OFFSET_pict_set_offset_y(data)                                   (0x000FFC00&((data)<<10))
#define DC_PICT_SET_OFFSET_pict_set_offset_y_src(data)                               ((0x000FFC00&(data))>>10)
#define DC_PICT_SET_OFFSET_get_pict_set_offset_y(data)                               ((0x000FFC00&(data))>>10)
#define DC_PICT_SET_OFFSET_pict_set_offset_x_shift                                   (0)
#define DC_PICT_SET_OFFSET_pict_set_offset_x_mask                                    (0x000003FF)
#define DC_PICT_SET_OFFSET_pict_set_offset_x(data)                                   (0x000003FF&((data)<<0))
#define DC_PICT_SET_OFFSET_pict_set_offset_x_src(data)                               ((0x000003FF&(data))>>0)
#define DC_PICT_SET_OFFSET_get_pict_set_offset_x(data)                               ((0x000003FF&(data))>>0)


#define DC_REQ_STATUS                                                                0x18008024
#define DC_REQ_STATUS_reg_addr                                                       "0xB8008024"
#define DC_REQ_STATUS_reg                                                            0xB8008024
#define set_DC_REQ_STATUS_reg(data)   (*((volatile unsigned int*) DC_REQ_STATUS_reg)=data)
#define get_DC_REQ_STATUS_reg   (*((volatile unsigned int*) DC_REQ_STATUS_reg))
#define DC_REQ_STATUS_inst_adr                                                       "0x0009"
#define DC_REQ_STATUS_inst                                                           0x0009
#define DC_REQ_STATUS_req_pending_shift                                              (11)
#define DC_REQ_STATUS_req_pending_mask                                               (0x00000800)
#define DC_REQ_STATUS_req_pending(data)                                              (0x00000800&((data)<<11))
#define DC_REQ_STATUS_req_pending_src(data)                                          ((0x00000800&(data))>>11)
#define DC_REQ_STATUS_get_req_pending(data)                                          ((0x00000800&(data))>>11)
#define DC_REQ_STATUS_req_id_shift                                                   (0)
#define DC_REQ_STATUS_req_id_mask                                                    (0x000007FF)
#define DC_REQ_STATUS_req_id(data)                                                   (0x000007FF&((data)<<0))
#define DC_REQ_STATUS_req_id_src(data)                                               ((0x000007FF&(data))>>0)
#define DC_REQ_STATUS_get_req_id(data)                                               ((0x000007FF&(data))>>0)


#define DC_SYS_MISC2                                                                 0x18008028
#define DC_SYS_MISC2_reg_addr                                                        "0xB8008028"
#define DC_SYS_MISC2_reg                                                             0xB8008028
#define set_DC_SYS_MISC2_reg(data)   (*((volatile unsigned int*) DC_SYS_MISC2_reg)=data)
#define get_DC_SYS_MISC2_reg   (*((volatile unsigned int*) DC_SYS_MISC2_reg))
#define DC_SYS_MISC2_inst_adr                                                        "0x000A"
#define DC_SYS_MISC2_inst                                                            0x000A
#define DC_SYS_MISC2_full_cmd_count_shift                                            (20)
#define DC_SYS_MISC2_full_cmd_count_mask                                             (0x00700000)
#define DC_SYS_MISC2_full_cmd_count(data)                                            (0x00700000&((data)<<20))
#define DC_SYS_MISC2_full_cmd_count_src(data)                                        ((0x00700000&(data))>>20)
#define DC_SYS_MISC2_get_full_cmd_count(data)                                        ((0x00700000&(data))>>20)
#define DC_SYS_MISC2_pre_tra_rd_cnt_shift                                            (10)
#define DC_SYS_MISC2_pre_tra_rd_cnt_mask                                             (0x000FFC00)
#define DC_SYS_MISC2_pre_tra_rd_cnt(data)                                            (0x000FFC00&((data)<<10))
#define DC_SYS_MISC2_pre_tra_rd_cnt_src(data)                                        ((0x000FFC00&(data))>>10)
#define DC_SYS_MISC2_get_pre_tra_rd_cnt(data)                                        ((0x000FFC00&(data))>>10)
#define DC_SYS_MISC2_pre_tra_wr_cnt_shift                                            (0)
#define DC_SYS_MISC2_pre_tra_wr_cnt_mask                                             (0x000003FF)
#define DC_SYS_MISC2_pre_tra_wr_cnt(data)                                            (0x000003FF&((data)<<0))
#define DC_SYS_MISC2_pre_tra_wr_cnt_src(data)                                        ((0x000003FF&(data))>>0)
#define DC_SYS_MISC2_get_pre_tra_wr_cnt(data)                                        ((0x000003FF&(data))>>0)


#define DC_PC_CTRL                                                                   0x18008030
#define DC_PC_CTRL_reg_addr                                                          "0xB8008030"
#define DC_PC_CTRL_reg                                                               0xB8008030
#define set_DC_PC_CTRL_reg(data)   (*((volatile unsigned int*) DC_PC_CTRL_reg)=data)
#define get_DC_PC_CTRL_reg   (*((volatile unsigned int*) DC_PC_CTRL_reg))
#define DC_PC_CTRL_inst_adr                                                          "0x000C"
#define DC_PC_CTRL_inst                                                              0x000C
#define DC_PC_CTRL_pc_go_shift                                                       (0)
#define DC_PC_CTRL_pc_go_mask                                                        (0x00000001)
#define DC_PC_CTRL_pc_go(data)                                                       (0x00000001&((data)<<0))
#define DC_PC_CTRL_pc_go_src(data)                                                   ((0x00000001&(data))>>0)
#define DC_PC_CTRL_get_pc_go(data)                                                   ((0x00000001&(data))>>0)


#define DC_PC_TOTA_MON_NUM                                                           0x18008034
#define DC_PC_TOTA_MON_NUM_reg_addr                                                  "0xB8008034"
#define DC_PC_TOTA_MON_NUM_reg                                                       0xB8008034
#define set_DC_PC_TOTA_MON_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_TOTA_MON_NUM_reg)=data)
#define get_DC_PC_TOTA_MON_NUM_reg   (*((volatile unsigned int*) DC_PC_TOTA_MON_NUM_reg))
#define DC_PC_TOTA_MON_NUM_inst_adr                                                  "0x000D"
#define DC_PC_TOTA_MON_NUM_inst                                                      0x000D
#define DC_PC_TOTA_MON_NUM_pc_tota_mon_num_shift                                     (0)
#define DC_PC_TOTA_MON_NUM_pc_tota_mon_num_mask                                      (0xFFFFFFFF)
#define DC_PC_TOTA_MON_NUM_pc_tota_mon_num(data)                                     (0xFFFFFFFF&((data)<<0))
#define DC_PC_TOTA_MON_NUM_pc_tota_mon_num_src(data)                                 ((0xFFFFFFFF&(data))>>0)
#define DC_PC_TOTA_MON_NUM_get_pc_tota_mon_num(data)                                 ((0xFFFFFFFF&(data))>>0)


#define DC_PC_TOTA_ACK_NUM                                                           0x18008038
#define DC_PC_TOTA_ACK_NUM_reg_addr                                                  "0xB8008038"
#define DC_PC_TOTA_ACK_NUM_reg                                                       0xB8008038
#define set_DC_PC_TOTA_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_TOTA_ACK_NUM_reg)=data)
#define get_DC_PC_TOTA_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_TOTA_ACK_NUM_reg))
#define DC_PC_TOTA_ACK_NUM_inst_adr                                                  "0x000E"
#define DC_PC_TOTA_ACK_NUM_inst                                                      0x000E
#define DC_PC_TOTA_ACK_NUM_pc_tota_ack_num_shift                                     (0)
#define DC_PC_TOTA_ACK_NUM_pc_tota_ack_num_mask                                      (0xFFFFFFFF)
#define DC_PC_TOTA_ACK_NUM_pc_tota_ack_num(data)                                     (0xFFFFFFFF&((data)<<0))
#define DC_PC_TOTA_ACK_NUM_pc_tota_ack_num_src(data)                                 ((0xFFFFFFFF&(data))>>0)
#define DC_PC_TOTA_ACK_NUM_get_pc_tota_ack_num(data)                                 ((0xFFFFFFFF&(data))>>0)


#define DC_PC_TOTA_IDL_NUM                                                           0x1800803C
#define DC_PC_TOTA_IDL_NUM_reg_addr                                                  "0xB800803C"
#define DC_PC_TOTA_IDL_NUM_reg                                                       0xB800803C
#define set_DC_PC_TOTA_IDL_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_TOTA_IDL_NUM_reg)=data)
#define get_DC_PC_TOTA_IDL_NUM_reg   (*((volatile unsigned int*) DC_PC_TOTA_IDL_NUM_reg))
#define DC_PC_TOTA_IDL_NUM_inst_adr                                                  "0x000F"
#define DC_PC_TOTA_IDL_NUM_inst                                                      0x000F
#define DC_PC_TOTA_IDL_NUM_pc_tota_idl_num_shift                                     (0)
#define DC_PC_TOTA_IDL_NUM_pc_tota_idl_num_mask                                      (0xFFFFFFFF)
#define DC_PC_TOTA_IDL_NUM_pc_tota_idl_num(data)                                     (0xFFFFFFFF&((data)<<0))
#define DC_PC_TOTA_IDL_NUM_pc_tota_idl_num_src(data)                                 ((0xFFFFFFFF&(data))>>0)
#define DC_PC_TOTA_IDL_NUM_get_pc_tota_idl_num(data)                                 ((0xFFFFFFFF&(data))>>0)


#define DC_PC_VE_ACC_LAT                                                             0x18008040
#define DC_PC_VE_ACC_LAT_reg_addr                                                    "0xB8008040"
#define DC_PC_VE_ACC_LAT_reg                                                         0xB8008040
#define set_DC_PC_VE_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_VE_ACC_LAT_reg)=data)
#define get_DC_PC_VE_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_VE_ACC_LAT_reg))
#define DC_PC_VE_ACC_LAT_inst_adr                                                    "0x0010"
#define DC_PC_VE_ACC_LAT_inst                                                        0x0010
#define DC_PC_VE_ACC_LAT_pc_ve_acc_lat_shift                                         (0)
#define DC_PC_VE_ACC_LAT_pc_ve_acc_lat_mask                                          (0xFFFFFFFF)
#define DC_PC_VE_ACC_LAT_pc_ve_acc_lat(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_VE_ACC_LAT_pc_ve_acc_lat_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_VE_ACC_LAT_get_pc_ve_acc_lat(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_VO_ACC_LAT                                                             0x18008050
#define DC_PC_VO_ACC_LAT_reg_addr                                                    "0xB8008050"
#define DC_PC_VO_ACC_LAT_reg                                                         0xB8008050
#define set_DC_PC_VO_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_VO_ACC_LAT_reg)=data)
#define get_DC_PC_VO_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_VO_ACC_LAT_reg))
#define DC_PC_VO_ACC_LAT_inst_adr                                                    "0x0014"
#define DC_PC_VO_ACC_LAT_inst                                                        0x0014
#define DC_PC_VO_ACC_LAT_pc_vo_acc_lat_shift                                         (0)
#define DC_PC_VO_ACC_LAT_pc_vo_acc_lat_mask                                          (0xFFFFFFFF)
#define DC_PC_VO_ACC_LAT_pc_vo_acc_lat(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_VO_ACC_LAT_pc_vo_acc_lat_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_VO_ACC_LAT_get_pc_vo_acc_lat(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_SB2_ACC_LAT                                                            0x18008060
#define DC_PC_SB2_ACC_LAT_reg_addr                                                   "0xB8008060"
#define DC_PC_SB2_ACC_LAT_reg                                                        0xB8008060
#define set_DC_PC_SB2_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_SB2_ACC_LAT_reg)=data)
#define get_DC_PC_SB2_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_SB2_ACC_LAT_reg))
#define DC_PC_SB2_ACC_LAT_inst_adr                                                   "0x0018"
#define DC_PC_SB2_ACC_LAT_inst                                                       0x0018
#define DC_PC_SB2_ACC_LAT_pc_sb2_acc_lat_shift                                       (0)
#define DC_PC_SB2_ACC_LAT_pc_sb2_acc_lat_mask                                        (0xFFFFFFFF)
#define DC_PC_SB2_ACC_LAT_pc_sb2_acc_lat(data)                                       (0xFFFFFFFF&((data)<<0))
#define DC_PC_SB2_ACC_LAT_pc_sb2_acc_lat_src(data)                                   ((0xFFFFFFFF&(data))>>0)
#define DC_PC_SB2_ACC_LAT_get_pc_sb2_acc_lat(data)                                   ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_0_ACC_LAT                                                         0x18008070
#define DC_PC_PROG_0_ACC_LAT_reg_addr                                                "0xB8008070"
#define DC_PC_PROG_0_ACC_LAT_reg                                                     0xB8008070
#define set_DC_PC_PROG_0_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_0_ACC_LAT_reg)=data)
#define get_DC_PC_PROG_0_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_PROG_0_ACC_LAT_reg))
#define DC_PC_PROG_0_ACC_LAT_inst_adr                                                "0x001C"
#define DC_PC_PROG_0_ACC_LAT_inst                                                    0x001C
#define DC_PC_PROG_0_ACC_LAT_pc_prog_0_acc_lat_shift                                 (0)
#define DC_PC_PROG_0_ACC_LAT_pc_prog_0_acc_lat_mask                                  (0xFFFFFFFF)
#define DC_PC_PROG_0_ACC_LAT_pc_prog_0_acc_lat(data)                                 (0xFFFFFFFF&((data)<<0))
#define DC_PC_PROG_0_ACC_LAT_pc_prog_0_acc_lat_src(data)                             ((0xFFFFFFFF&(data))>>0)
#define DC_PC_PROG_0_ACC_LAT_get_pc_prog_0_acc_lat(data)                             ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_1_ACC_LAT                                                         0x18008080
#define DC_PC_PROG_1_ACC_LAT_reg_addr                                                "0xB8008080"
#define DC_PC_PROG_1_ACC_LAT_reg                                                     0xB8008080
#define set_DC_PC_PROG_1_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_1_ACC_LAT_reg)=data)
#define get_DC_PC_PROG_1_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_PROG_1_ACC_LAT_reg))
#define DC_PC_PROG_1_ACC_LAT_inst_adr                                                "0x0020"
#define DC_PC_PROG_1_ACC_LAT_inst                                                    0x0020
#define DC_PC_PROG_1_ACC_LAT_pc_prog_1_acc_lat_shift                                 (0)
#define DC_PC_PROG_1_ACC_LAT_pc_prog_1_acc_lat_mask                                  (0xFFFFFFFF)
#define DC_PC_PROG_1_ACC_LAT_pc_prog_1_acc_lat(data)                                 (0xFFFFFFFF&((data)<<0))
#define DC_PC_PROG_1_ACC_LAT_pc_prog_1_acc_lat_src(data)                             ((0xFFFFFFFF&(data))>>0)
#define DC_PC_PROG_1_ACC_LAT_get_pc_prog_1_acc_lat(data)                             ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_2_ACC_LAT                                                         0x18008090
#define DC_PC_PROG_2_ACC_LAT_reg_addr                                                "0xB8008090"
#define DC_PC_PROG_2_ACC_LAT_reg                                                     0xB8008090
#define set_DC_PC_PROG_2_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_2_ACC_LAT_reg)=data)
#define get_DC_PC_PROG_2_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_PROG_2_ACC_LAT_reg))
#define DC_PC_PROG_2_ACC_LAT_inst_adr                                                "0x0024"
#define DC_PC_PROG_2_ACC_LAT_inst                                                    0x0024
#define DC_PC_PROG_2_ACC_LAT_pc_prog_2_acc_lat_shift                                 (0)
#define DC_PC_PROG_2_ACC_LAT_pc_prog_2_acc_lat_mask                                  (0xFFFFFFFF)
#define DC_PC_PROG_2_ACC_LAT_pc_prog_2_acc_lat(data)                                 (0xFFFFFFFF&((data)<<0))
#define DC_PC_PROG_2_ACC_LAT_pc_prog_2_acc_lat_src(data)                             ((0xFFFFFFFF&(data))>>0)
#define DC_PC_PROG_2_ACC_LAT_get_pc_prog_2_acc_lat(data)                             ((0xFFFFFFFF&(data))>>0)


#define DC_PC_200_PROG_0_ACC_LAT                                                     0x18008500
#define DC_PC_200_PROG_0_ACC_LAT_reg_addr                                            "0xB8008500"
#define DC_PC_200_PROG_0_ACC_LAT_reg                                                 0xB8008500
#define set_DC_PC_200_PROG_0_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_0_ACC_LAT_reg)=data)
#define get_DC_PC_200_PROG_0_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_200_PROG_0_ACC_LAT_reg))
#define DC_PC_200_PROG_0_ACC_LAT_inst_adr                                            "0x0040"
#define DC_PC_200_PROG_0_ACC_LAT_inst                                                0x0040
#define DC_PC_200_PROG_0_ACC_LAT_pc_200_prog_0_acc_lat_shift                         (0)
#define DC_PC_200_PROG_0_ACC_LAT_pc_200_prog_0_acc_lat_mask                          (0xFFFFFFFF)
#define DC_PC_200_PROG_0_ACC_LAT_pc_200_prog_0_acc_lat(data)                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_200_PROG_0_ACC_LAT_pc_200_prog_0_acc_lat_src(data)                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_200_PROG_0_ACC_LAT_get_pc_200_prog_0_acc_lat(data)                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_200_PROG_1_ACC_LAT                                                     0x18008504
#define DC_PC_200_PROG_1_ACC_LAT_reg_addr                                            "0xB8008504"
#define DC_PC_200_PROG_1_ACC_LAT_reg                                                 0xB8008504
#define set_DC_PC_200_PROG_1_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_1_ACC_LAT_reg)=data)
#define get_DC_PC_200_PROG_1_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_200_PROG_1_ACC_LAT_reg))
#define DC_PC_200_PROG_1_ACC_LAT_inst_adr                                            "0x0041"
#define DC_PC_200_PROG_1_ACC_LAT_inst                                                0x0041
#define DC_PC_200_PROG_1_ACC_LAT_pc_200_prog_1_acc_lat_shift                         (0)
#define DC_PC_200_PROG_1_ACC_LAT_pc_200_prog_1_acc_lat_mask                          (0xFFFFFFFF)
#define DC_PC_200_PROG_1_ACC_LAT_pc_200_prog_1_acc_lat(data)                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_200_PROG_1_ACC_LAT_pc_200_prog_1_acc_lat_src(data)                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_200_PROG_1_ACC_LAT_get_pc_200_prog_1_acc_lat(data)                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_GPU_ACC_LAT                                                            0x180080A0
#define DC_PC_GPU_ACC_LAT_reg_addr                                                   "0xB80080A0"
#define DC_PC_GPU_ACC_LAT_reg                                                        0xB80080A0
#define set_DC_PC_GPU_ACC_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_GPU_ACC_LAT_reg)=data)
#define get_DC_PC_GPU_ACC_LAT_reg   (*((volatile unsigned int*) DC_PC_GPU_ACC_LAT_reg))
#define DC_PC_GPU_ACC_LAT_inst_adr                                                   "0x0028"
#define DC_PC_GPU_ACC_LAT_inst                                                       0x0028
#define DC_PC_GPU_ACC_LAT_pc_gpu_acc_lat_shift                                       (0)
#define DC_PC_GPU_ACC_LAT_pc_gpu_acc_lat_mask                                        (0xFFFFFFFF)
#define DC_PC_GPU_ACC_LAT_pc_gpu_acc_lat(data)                                       (0xFFFFFFFF&((data)<<0))
#define DC_PC_GPU_ACC_LAT_pc_gpu_acc_lat_src(data)                                   ((0xFFFFFFFF&(data))>>0)
#define DC_PC_GPU_ACC_LAT_get_pc_gpu_acc_lat(data)                                   ((0xFFFFFFFF&(data))>>0)


#define DC_PC_VE_MAX_LAT                                                             0x18008044
#define DC_PC_VE_MAX_LAT_reg_addr                                                    "0xB8008044"
#define DC_PC_VE_MAX_LAT_reg                                                         0xB8008044
#define set_DC_PC_VE_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_VE_MAX_LAT_reg)=data)
#define get_DC_PC_VE_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_VE_MAX_LAT_reg))
#define DC_PC_VE_MAX_LAT_inst_adr                                                    "0x0011"
#define DC_PC_VE_MAX_LAT_inst                                                        0x0011
#define DC_PC_VE_MAX_LAT_pc_ve_max_lat_shift                                         (0)
#define DC_PC_VE_MAX_LAT_pc_ve_max_lat_mask                                          (0x0000FFFF)
#define DC_PC_VE_MAX_LAT_pc_ve_max_lat(data)                                         (0x0000FFFF&((data)<<0))
#define DC_PC_VE_MAX_LAT_pc_ve_max_lat_src(data)                                     ((0x0000FFFF&(data))>>0)
#define DC_PC_VE_MAX_LAT_get_pc_ve_max_lat(data)                                     ((0x0000FFFF&(data))>>0)


#define DC_PC_VO_MAX_LAT                                                             0x18008054
#define DC_PC_VO_MAX_LAT_reg_addr                                                    "0xB8008054"
#define DC_PC_VO_MAX_LAT_reg                                                         0xB8008054
#define set_DC_PC_VO_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_VO_MAX_LAT_reg)=data)
#define get_DC_PC_VO_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_VO_MAX_LAT_reg))
#define DC_PC_VO_MAX_LAT_inst_adr                                                    "0x0015"
#define DC_PC_VO_MAX_LAT_inst                                                        0x0015
#define DC_PC_VO_MAX_LAT_pc_vo_max_lat_shift                                         (0)
#define DC_PC_VO_MAX_LAT_pc_vo_max_lat_mask                                          (0x0000FFFF)
#define DC_PC_VO_MAX_LAT_pc_vo_max_lat(data)                                         (0x0000FFFF&((data)<<0))
#define DC_PC_VO_MAX_LAT_pc_vo_max_lat_src(data)                                     ((0x0000FFFF&(data))>>0)
#define DC_PC_VO_MAX_LAT_get_pc_vo_max_lat(data)                                     ((0x0000FFFF&(data))>>0)


#define DC_PC_SB2_MAX_LAT                                                            0x18008064
#define DC_PC_SB2_MAX_LAT_reg_addr                                                   "0xB8008064"
#define DC_PC_SB2_MAX_LAT_reg                                                        0xB8008064
#define set_DC_PC_SB2_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_SB2_MAX_LAT_reg)=data)
#define get_DC_PC_SB2_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_SB2_MAX_LAT_reg))
#define DC_PC_SB2_MAX_LAT_inst_adr                                                   "0x0019"
#define DC_PC_SB2_MAX_LAT_inst                                                       0x0019
#define DC_PC_SB2_MAX_LAT_pc_sb2_max_lat_shift                                       (0)
#define DC_PC_SB2_MAX_LAT_pc_sb2_max_lat_mask                                        (0x0000FFFF)
#define DC_PC_SB2_MAX_LAT_pc_sb2_max_lat(data)                                       (0x0000FFFF&((data)<<0))
#define DC_PC_SB2_MAX_LAT_pc_sb2_max_lat_src(data)                                   ((0x0000FFFF&(data))>>0)
#define DC_PC_SB2_MAX_LAT_get_pc_sb2_max_lat(data)                                   ((0x0000FFFF&(data))>>0)


#define DC_PC_PROG_0_MAX_LAT                                                         0x18008074
#define DC_PC_PROG_0_MAX_LAT_reg_addr                                                "0xB8008074"
#define DC_PC_PROG_0_MAX_LAT_reg                                                     0xB8008074
#define set_DC_PC_PROG_0_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_0_MAX_LAT_reg)=data)
#define get_DC_PC_PROG_0_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_PROG_0_MAX_LAT_reg))
#define DC_PC_PROG_0_MAX_LAT_inst_adr                                                "0x001D"
#define DC_PC_PROG_0_MAX_LAT_inst                                                    0x001D
#define DC_PC_PROG_0_MAX_LAT_pc_prog_0_max_lat_shift                                 (0)
#define DC_PC_PROG_0_MAX_LAT_pc_prog_0_max_lat_mask                                  (0x0000FFFF)
#define DC_PC_PROG_0_MAX_LAT_pc_prog_0_max_lat(data)                                 (0x0000FFFF&((data)<<0))
#define DC_PC_PROG_0_MAX_LAT_pc_prog_0_max_lat_src(data)                             ((0x0000FFFF&(data))>>0)
#define DC_PC_PROG_0_MAX_LAT_get_pc_prog_0_max_lat(data)                             ((0x0000FFFF&(data))>>0)


#define DC_PC_PROG_1_MAX_LAT                                                         0x18008084
#define DC_PC_PROG_1_MAX_LAT_reg_addr                                                "0xB8008084"
#define DC_PC_PROG_1_MAX_LAT_reg                                                     0xB8008084
#define set_DC_PC_PROG_1_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_1_MAX_LAT_reg)=data)
#define get_DC_PC_PROG_1_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_PROG_1_MAX_LAT_reg))
#define DC_PC_PROG_1_MAX_LAT_inst_adr                                                "0x0021"
#define DC_PC_PROG_1_MAX_LAT_inst                                                    0x0021
#define DC_PC_PROG_1_MAX_LAT_pc_prog_1_max_lat_shift                                 (0)
#define DC_PC_PROG_1_MAX_LAT_pc_prog_1_max_lat_mask                                  (0x0000FFFF)
#define DC_PC_PROG_1_MAX_LAT_pc_prog_1_max_lat(data)                                 (0x0000FFFF&((data)<<0))
#define DC_PC_PROG_1_MAX_LAT_pc_prog_1_max_lat_src(data)                             ((0x0000FFFF&(data))>>0)
#define DC_PC_PROG_1_MAX_LAT_get_pc_prog_1_max_lat(data)                             ((0x0000FFFF&(data))>>0)


#define DC_PC_PROG_2_MAX_LAT                                                         0x18008094
#define DC_PC_PROG_2_MAX_LAT_reg_addr                                                "0xB8008094"
#define DC_PC_PROG_2_MAX_LAT_reg                                                     0xB8008094
#define set_DC_PC_PROG_2_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_2_MAX_LAT_reg)=data)
#define get_DC_PC_PROG_2_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_PROG_2_MAX_LAT_reg))
#define DC_PC_PROG_2_MAX_LAT_inst_adr                                                "0x0025"
#define DC_PC_PROG_2_MAX_LAT_inst                                                    0x0025
#define DC_PC_PROG_2_MAX_LAT_pc_prog_2_max_lat_shift                                 (0)
#define DC_PC_PROG_2_MAX_LAT_pc_prog_2_max_lat_mask                                  (0x0000FFFF)
#define DC_PC_PROG_2_MAX_LAT_pc_prog_2_max_lat(data)                                 (0x0000FFFF&((data)<<0))
#define DC_PC_PROG_2_MAX_LAT_pc_prog_2_max_lat_src(data)                             ((0x0000FFFF&(data))>>0)
#define DC_PC_PROG_2_MAX_LAT_get_pc_prog_2_max_lat(data)                             ((0x0000FFFF&(data))>>0)


#define DC_PC_GPU_MAX_LAT                                                            0x180080A4
#define DC_PC_GPU_MAX_LAT_reg_addr                                                   "0xB80080A4"
#define DC_PC_GPU_MAX_LAT_reg                                                        0xB80080A4
#define set_DC_PC_GPU_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_GPU_MAX_LAT_reg)=data)
#define get_DC_PC_GPU_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_GPU_MAX_LAT_reg))
#define DC_PC_GPU_MAX_LAT_inst_adr                                                   "0x0029"
#define DC_PC_GPU_MAX_LAT_inst                                                       0x0029
#define DC_PC_GPU_MAX_LAT_pc_gpu_max_lat_shift                                       (0)
#define DC_PC_GPU_MAX_LAT_pc_gpu_max_lat_mask                                        (0x0000FFFF)
#define DC_PC_GPU_MAX_LAT_pc_gpu_max_lat(data)                                       (0x0000FFFF&((data)<<0))
#define DC_PC_GPU_MAX_LAT_pc_gpu_max_lat_src(data)                                   ((0x0000FFFF&(data))>>0)
#define DC_PC_GPU_MAX_LAT_get_pc_gpu_max_lat(data)                                   ((0x0000FFFF&(data))>>0)


#define DC_PC_200_PROG_0_MAX_LAT                                                     0x18008508
#define DC_PC_200_PROG_0_MAX_LAT_reg_addr                                            "0xB8008508"
#define DC_PC_200_PROG_0_MAX_LAT_reg                                                 0xB8008508
#define set_DC_PC_200_PROG_0_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_0_MAX_LAT_reg)=data)
#define get_DC_PC_200_PROG_0_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_200_PROG_0_MAX_LAT_reg))
#define DC_PC_200_PROG_0_MAX_LAT_inst_adr                                            "0x0042"
#define DC_PC_200_PROG_0_MAX_LAT_inst                                                0x0042
#define DC_PC_200_PROG_0_MAX_LAT_pc_200_prog_0_max_lat_shift                         (0)
#define DC_PC_200_PROG_0_MAX_LAT_pc_200_prog_0_max_lat_mask                          (0x0000FFFF)
#define DC_PC_200_PROG_0_MAX_LAT_pc_200_prog_0_max_lat(data)                         (0x0000FFFF&((data)<<0))
#define DC_PC_200_PROG_0_MAX_LAT_pc_200_prog_0_max_lat_src(data)                     ((0x0000FFFF&(data))>>0)
#define DC_PC_200_PROG_0_MAX_LAT_get_pc_200_prog_0_max_lat(data)                     ((0x0000FFFF&(data))>>0)


#define DC_PC_200_PROG_1_MAX_LAT                                                     0x1800850C
#define DC_PC_200_PROG_1_MAX_LAT_reg_addr                                            "0xB800850C"
#define DC_PC_200_PROG_1_MAX_LAT_reg                                                 0xB800850C
#define set_DC_PC_200_PROG_1_MAX_LAT_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_1_MAX_LAT_reg)=data)
#define get_DC_PC_200_PROG_1_MAX_LAT_reg   (*((volatile unsigned int*) DC_PC_200_PROG_1_MAX_LAT_reg))
#define DC_PC_200_PROG_1_MAX_LAT_inst_adr                                            "0x0043"
#define DC_PC_200_PROG_1_MAX_LAT_inst                                                0x0043
#define DC_PC_200_PROG_1_MAX_LAT_pc_200_prog_1_max_lat_shift                         (0)
#define DC_PC_200_PROG_1_MAX_LAT_pc_200_prog_1_max_lat_mask                          (0x0000FFFF)
#define DC_PC_200_PROG_1_MAX_LAT_pc_200_prog_1_max_lat(data)                         (0x0000FFFF&((data)<<0))
#define DC_PC_200_PROG_1_MAX_LAT_pc_200_prog_1_max_lat_src(data)                     ((0x0000FFFF&(data))>>0)
#define DC_PC_200_PROG_1_MAX_LAT_get_pc_200_prog_1_max_lat(data)                     ((0x0000FFFF&(data))>>0)


#define DC_PC_VE_REQ_NUM                                                             0x18008048
#define DC_PC_VE_REQ_NUM_reg_addr                                                    "0xB8008048"
#define DC_PC_VE_REQ_NUM_reg                                                         0xB8008048
#define set_DC_PC_VE_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_VE_REQ_NUM_reg)=data)
#define get_DC_PC_VE_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_VE_REQ_NUM_reg))
#define DC_PC_VE_REQ_NUM_inst_adr                                                    "0x0012"
#define DC_PC_VE_REQ_NUM_inst                                                        0x0012
#define DC_PC_VE_REQ_NUM_pc_ve_req_num_shift                                         (0)
#define DC_PC_VE_REQ_NUM_pc_ve_req_num_mask                                          (0x00FFFFFF)
#define DC_PC_VE_REQ_NUM_pc_ve_req_num(data)                                         (0x00FFFFFF&((data)<<0))
#define DC_PC_VE_REQ_NUM_pc_ve_req_num_src(data)                                     ((0x00FFFFFF&(data))>>0)
#define DC_PC_VE_REQ_NUM_get_pc_ve_req_num(data)                                     ((0x00FFFFFF&(data))>>0)


#define DC_PC_VO_REQ_NUM                                                             0x18008058
#define DC_PC_VO_REQ_NUM_reg_addr                                                    "0xB8008058"
#define DC_PC_VO_REQ_NUM_reg                                                         0xB8008058
#define set_DC_PC_VO_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_VO_REQ_NUM_reg)=data)
#define get_DC_PC_VO_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_VO_REQ_NUM_reg))
#define DC_PC_VO_REQ_NUM_inst_adr                                                    "0x0016"
#define DC_PC_VO_REQ_NUM_inst                                                        0x0016
#define DC_PC_VO_REQ_NUM_pc_vo_req_num_shift                                         (0)
#define DC_PC_VO_REQ_NUM_pc_vo_req_num_mask                                          (0x00FFFFFF)
#define DC_PC_VO_REQ_NUM_pc_vo_req_num(data)                                         (0x00FFFFFF&((data)<<0))
#define DC_PC_VO_REQ_NUM_pc_vo_req_num_src(data)                                     ((0x00FFFFFF&(data))>>0)
#define DC_PC_VO_REQ_NUM_get_pc_vo_req_num(data)                                     ((0x00FFFFFF&(data))>>0)


#define DC_PC_SB2_REQ_NUM                                                            0x18008068
#define DC_PC_SB2_REQ_NUM_reg_addr                                                   "0xB8008068"
#define DC_PC_SB2_REQ_NUM_reg                                                        0xB8008068
#define set_DC_PC_SB2_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_SB2_REQ_NUM_reg)=data)
#define get_DC_PC_SB2_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_SB2_REQ_NUM_reg))
#define DC_PC_SB2_REQ_NUM_inst_adr                                                   "0x001A"
#define DC_PC_SB2_REQ_NUM_inst                                                       0x001A
#define DC_PC_SB2_REQ_NUM_pc_sb2_req_num_shift                                       (0)
#define DC_PC_SB2_REQ_NUM_pc_sb2_req_num_mask                                        (0x00FFFFFF)
#define DC_PC_SB2_REQ_NUM_pc_sb2_req_num(data)                                       (0x00FFFFFF&((data)<<0))
#define DC_PC_SB2_REQ_NUM_pc_sb2_req_num_src(data)                                   ((0x00FFFFFF&(data))>>0)
#define DC_PC_SB2_REQ_NUM_get_pc_sb2_req_num(data)                                   ((0x00FFFFFF&(data))>>0)


#define DC_PC_PROG_0_REQ_NUM                                                         0x18008078
#define DC_PC_PROG_0_REQ_NUM_reg_addr                                                "0xB8008078"
#define DC_PC_PROG_0_REQ_NUM_reg                                                     0xB8008078
#define set_DC_PC_PROG_0_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_0_REQ_NUM_reg)=data)
#define get_DC_PC_PROG_0_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_PROG_0_REQ_NUM_reg))
#define DC_PC_PROG_0_REQ_NUM_inst_adr                                                "0x001E"
#define DC_PC_PROG_0_REQ_NUM_inst                                                    0x001E
#define DC_PC_PROG_0_REQ_NUM_pc_prog_0_req_num_shift                                 (0)
#define DC_PC_PROG_0_REQ_NUM_pc_prog_0_req_num_mask                                  (0x00FFFFFF)
#define DC_PC_PROG_0_REQ_NUM_pc_prog_0_req_num(data)                                 (0x00FFFFFF&((data)<<0))
#define DC_PC_PROG_0_REQ_NUM_pc_prog_0_req_num_src(data)                             ((0x00FFFFFF&(data))>>0)
#define DC_PC_PROG_0_REQ_NUM_get_pc_prog_0_req_num(data)                             ((0x00FFFFFF&(data))>>0)


#define DC_PC_PROG_1_REQ_NUM                                                         0x18008088
#define DC_PC_PROG_1_REQ_NUM_reg_addr                                                "0xB8008088"
#define DC_PC_PROG_1_REQ_NUM_reg                                                     0xB8008088
#define set_DC_PC_PROG_1_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_1_REQ_NUM_reg)=data)
#define get_DC_PC_PROG_1_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_PROG_1_REQ_NUM_reg))
#define DC_PC_PROG_1_REQ_NUM_inst_adr                                                "0x0022"
#define DC_PC_PROG_1_REQ_NUM_inst                                                    0x0022
#define DC_PC_PROG_1_REQ_NUM_pc_prog_1_req_num_shift                                 (0)
#define DC_PC_PROG_1_REQ_NUM_pc_prog_1_req_num_mask                                  (0x00FFFFFF)
#define DC_PC_PROG_1_REQ_NUM_pc_prog_1_req_num(data)                                 (0x00FFFFFF&((data)<<0))
#define DC_PC_PROG_1_REQ_NUM_pc_prog_1_req_num_src(data)                             ((0x00FFFFFF&(data))>>0)
#define DC_PC_PROG_1_REQ_NUM_get_pc_prog_1_req_num(data)                             ((0x00FFFFFF&(data))>>0)


#define DC_PC_PROG_2_REQ_NUM                                                         0x18008098
#define DC_PC_PROG_2_REQ_NUM_reg_addr                                                "0xB8008098"
#define DC_PC_PROG_2_REQ_NUM_reg                                                     0xB8008098
#define set_DC_PC_PROG_2_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_2_REQ_NUM_reg)=data)
#define get_DC_PC_PROG_2_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_PROG_2_REQ_NUM_reg))
#define DC_PC_PROG_2_REQ_NUM_inst_adr                                                "0x0026"
#define DC_PC_PROG_2_REQ_NUM_inst                                                    0x0026
#define DC_PC_PROG_2_REQ_NUM_pc_prog_2_req_num_shift                                 (0)
#define DC_PC_PROG_2_REQ_NUM_pc_prog_2_req_num_mask                                  (0x00FFFFFF)
#define DC_PC_PROG_2_REQ_NUM_pc_prog_2_req_num(data)                                 (0x00FFFFFF&((data)<<0))
#define DC_PC_PROG_2_REQ_NUM_pc_prog_2_req_num_src(data)                             ((0x00FFFFFF&(data))>>0)
#define DC_PC_PROG_2_REQ_NUM_get_pc_prog_2_req_num(data)                             ((0x00FFFFFF&(data))>>0)


#define DC_PC_GPU_REQ_NUM                                                            0x180080A8
#define DC_PC_GPU_REQ_NUM_reg_addr                                                   "0xB80080A8"
#define DC_PC_GPU_REQ_NUM_reg                                                        0xB80080A8
#define set_DC_PC_GPU_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_GPU_REQ_NUM_reg)=data)
#define get_DC_PC_GPU_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_GPU_REQ_NUM_reg))
#define DC_PC_GPU_REQ_NUM_inst_adr                                                   "0x002A"
#define DC_PC_GPU_REQ_NUM_inst                                                       0x002A
#define DC_PC_GPU_REQ_NUM_pc_gpu_req_num_shift                                       (0)
#define DC_PC_GPU_REQ_NUM_pc_gpu_req_num_mask                                        (0x00FFFFFF)
#define DC_PC_GPU_REQ_NUM_pc_gpu_req_num(data)                                       (0x00FFFFFF&((data)<<0))
#define DC_PC_GPU_REQ_NUM_pc_gpu_req_num_src(data)                                   ((0x00FFFFFF&(data))>>0)
#define DC_PC_GPU_REQ_NUM_get_pc_gpu_req_num(data)                                   ((0x00FFFFFF&(data))>>0)


#define DC_PC_200_PROG_0_REQ_NUM                                                     0x18008510
#define DC_PC_200_PROG_0_REQ_NUM_reg_addr                                            "0xB8008510"
#define DC_PC_200_PROG_0_REQ_NUM_reg                                                 0xB8008510
#define set_DC_PC_200_PROG_0_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_0_REQ_NUM_reg)=data)
#define get_DC_PC_200_PROG_0_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_200_PROG_0_REQ_NUM_reg))
#define DC_PC_200_PROG_0_REQ_NUM_inst_adr                                            "0x0044"
#define DC_PC_200_PROG_0_REQ_NUM_inst                                                0x0044
#define DC_PC_200_PROG_0_REQ_NUM_pc_200_prog_0_req_num_shift                         (0)
#define DC_PC_200_PROG_0_REQ_NUM_pc_200_prog_0_req_num_mask                          (0x00FFFFFF)
#define DC_PC_200_PROG_0_REQ_NUM_pc_200_prog_0_req_num(data)                         (0x00FFFFFF&((data)<<0))
#define DC_PC_200_PROG_0_REQ_NUM_pc_200_prog_0_req_num_src(data)                     ((0x00FFFFFF&(data))>>0)
#define DC_PC_200_PROG_0_REQ_NUM_get_pc_200_prog_0_req_num(data)                     ((0x00FFFFFF&(data))>>0)


#define DC_PC_200_PROG_1_REQ_NUM                                                     0x18008514
#define DC_PC_200_PROG_1_REQ_NUM_reg_addr                                            "0xB8008514"
#define DC_PC_200_PROG_1_REQ_NUM_reg                                                 0xB8008514
#define set_DC_PC_200_PROG_1_REQ_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_1_REQ_NUM_reg)=data)
#define get_DC_PC_200_PROG_1_REQ_NUM_reg   (*((volatile unsigned int*) DC_PC_200_PROG_1_REQ_NUM_reg))
#define DC_PC_200_PROG_1_REQ_NUM_inst_adr                                            "0x0045"
#define DC_PC_200_PROG_1_REQ_NUM_inst                                                0x0045
#define DC_PC_200_PROG_1_REQ_NUM_pc_200_prog_1_req_num_shift                         (0)
#define DC_PC_200_PROG_1_REQ_NUM_pc_200_prog_1_req_num_mask                          (0x00FFFFFF)
#define DC_PC_200_PROG_1_REQ_NUM_pc_200_prog_1_req_num(data)                         (0x00FFFFFF&((data)<<0))
#define DC_PC_200_PROG_1_REQ_NUM_pc_200_prog_1_req_num_src(data)                     ((0x00FFFFFF&(data))>>0)
#define DC_PC_200_PROG_1_REQ_NUM_get_pc_200_prog_1_req_num(data)                     ((0x00FFFFFF&(data))>>0)


#define DC_PC_VE_ACK_NUM                                                             0x1800804C
#define DC_PC_VE_ACK_NUM_reg_addr                                                    "0xB800804C"
#define DC_PC_VE_ACK_NUM_reg                                                         0xB800804C
#define set_DC_PC_VE_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_VE_ACK_NUM_reg)=data)
#define get_DC_PC_VE_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_VE_ACK_NUM_reg))
#define DC_PC_VE_ACK_NUM_inst_adr                                                    "0x0013"
#define DC_PC_VE_ACK_NUM_inst                                                        0x0013
#define DC_PC_VE_ACK_NUM_pc_ve_ack_num_shift                                         (0)
#define DC_PC_VE_ACK_NUM_pc_ve_ack_num_mask                                          (0xFFFFFFFF)
#define DC_PC_VE_ACK_NUM_pc_ve_ack_num(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_VE_ACK_NUM_pc_ve_ack_num_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_VE_ACK_NUM_get_pc_ve_ack_num(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_VO_ACK_NUM                                                             0x1800805C
#define DC_PC_VO_ACK_NUM_reg_addr                                                    "0xB800805C"
#define DC_PC_VO_ACK_NUM_reg                                                         0xB800805C
#define set_DC_PC_VO_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_VO_ACK_NUM_reg)=data)
#define get_DC_PC_VO_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_VO_ACK_NUM_reg))
#define DC_PC_VO_ACK_NUM_inst_adr                                                    "0x0017"
#define DC_PC_VO_ACK_NUM_inst                                                        0x0017
#define DC_PC_VO_ACK_NUM_pc_vo_ack_num_shift                                         (0)
#define DC_PC_VO_ACK_NUM_pc_vo_ack_num_mask                                          (0xFFFFFFFF)
#define DC_PC_VO_ACK_NUM_pc_vo_ack_num(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_VO_ACK_NUM_pc_vo_ack_num_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_VO_ACK_NUM_get_pc_vo_ack_num(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_SB2_ACK_NUM                                                            0x1800806C
#define DC_PC_SB2_ACK_NUM_reg_addr                                                   "0xB800806C"
#define DC_PC_SB2_ACK_NUM_reg                                                        0xB800806C
#define set_DC_PC_SB2_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_SB2_ACK_NUM_reg)=data)
#define get_DC_PC_SB2_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_SB2_ACK_NUM_reg))
#define DC_PC_SB2_ACK_NUM_inst_adr                                                   "0x001B"
#define DC_PC_SB2_ACK_NUM_inst                                                       0x001B
#define DC_PC_SB2_ACK_NUM_pc_sb2_ack_num_shift                                       (0)
#define DC_PC_SB2_ACK_NUM_pc_sb2_ack_num_mask                                        (0xFFFFFFFF)
#define DC_PC_SB2_ACK_NUM_pc_sb2_ack_num(data)                                       (0xFFFFFFFF&((data)<<0))
#define DC_PC_SB2_ACK_NUM_pc_sb2_ack_num_src(data)                                   ((0xFFFFFFFF&(data))>>0)
#define DC_PC_SB2_ACK_NUM_get_pc_sb2_ack_num(data)                                   ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_0_ACK_NUM                                                         0x1800807C
#define DC_PC_PROG_0_ACK_NUM_reg_addr                                                "0xB800807C"
#define DC_PC_PROG_0_ACK_NUM_reg                                                     0xB800807C
#define set_DC_PC_PROG_0_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_0_ACK_NUM_reg)=data)
#define get_DC_PC_PROG_0_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_PROG_0_ACK_NUM_reg))
#define DC_PC_PROG_0_ACK_NUM_inst_adr                                                "0x001F"
#define DC_PC_PROG_0_ACK_NUM_inst                                                    0x001F
#define DC_PC_PROG_0_ACK_NUM_pc_prog_0_ack_num_shift                                 (0)
#define DC_PC_PROG_0_ACK_NUM_pc_prog_0_ack_num_mask                                  (0xFFFFFFFF)
#define DC_PC_PROG_0_ACK_NUM_pc_prog_0_ack_num(data)                                 (0xFFFFFFFF&((data)<<0))
#define DC_PC_PROG_0_ACK_NUM_pc_prog_0_ack_num_src(data)                             ((0xFFFFFFFF&(data))>>0)
#define DC_PC_PROG_0_ACK_NUM_get_pc_prog_0_ack_num(data)                             ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_1_ACK_NUM                                                         0x1800808C
#define DC_PC_PROG_1_ACK_NUM_reg_addr                                                "0xB800808C"
#define DC_PC_PROG_1_ACK_NUM_reg                                                     0xB800808C
#define set_DC_PC_PROG_1_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_1_ACK_NUM_reg)=data)
#define get_DC_PC_PROG_1_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_PROG_1_ACK_NUM_reg))
#define DC_PC_PROG_1_ACK_NUM_inst_adr                                                "0x0023"
#define DC_PC_PROG_1_ACK_NUM_inst                                                    0x0023
#define DC_PC_PROG_1_ACK_NUM_pc_prog_1_ack_num_shift                                 (0)
#define DC_PC_PROG_1_ACK_NUM_pc_prog_1_ack_num_mask                                  (0xFFFFFFFF)
#define DC_PC_PROG_1_ACK_NUM_pc_prog_1_ack_num(data)                                 (0xFFFFFFFF&((data)<<0))
#define DC_PC_PROG_1_ACK_NUM_pc_prog_1_ack_num_src(data)                             ((0xFFFFFFFF&(data))>>0)
#define DC_PC_PROG_1_ACK_NUM_get_pc_prog_1_ack_num(data)                             ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_2_ACK_NUM                                                         0x1800809C
#define DC_PC_PROG_2_ACK_NUM_reg_addr                                                "0xB800809C"
#define DC_PC_PROG_2_ACK_NUM_reg                                                     0xB800809C
#define set_DC_PC_PROG_2_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_2_ACK_NUM_reg)=data)
#define get_DC_PC_PROG_2_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_PROG_2_ACK_NUM_reg))
#define DC_PC_PROG_2_ACK_NUM_inst_adr                                                "0x0027"
#define DC_PC_PROG_2_ACK_NUM_inst                                                    0x0027
#define DC_PC_PROG_2_ACK_NUM_pc_prog_2_ack_num_shift                                 (0)
#define DC_PC_PROG_2_ACK_NUM_pc_prog_2_ack_num_mask                                  (0xFFFFFFFF)
#define DC_PC_PROG_2_ACK_NUM_pc_prog_2_ack_num(data)                                 (0xFFFFFFFF&((data)<<0))
#define DC_PC_PROG_2_ACK_NUM_pc_prog_2_ack_num_src(data)                             ((0xFFFFFFFF&(data))>>0)
#define DC_PC_PROG_2_ACK_NUM_get_pc_prog_2_ack_num(data)                             ((0xFFFFFFFF&(data))>>0)


#define DC_PC_GPU_ACK_NUM                                                            0x180080AC
#define DC_PC_GPU_ACK_NUM_reg_addr                                                   "0xB80080AC"
#define DC_PC_GPU_ACK_NUM_reg                                                        0xB80080AC
#define set_DC_PC_GPU_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_GPU_ACK_NUM_reg)=data)
#define get_DC_PC_GPU_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_GPU_ACK_NUM_reg))
#define DC_PC_GPU_ACK_NUM_inst_adr                                                   "0x002B"
#define DC_PC_GPU_ACK_NUM_inst                                                       0x002B
#define DC_PC_GPU_ACK_NUM_pc_gpu_ack_num_shift                                       (0)
#define DC_PC_GPU_ACK_NUM_pc_gpu_ack_num_mask                                        (0xFFFFFFFF)
#define DC_PC_GPU_ACK_NUM_pc_gpu_ack_num(data)                                       (0xFFFFFFFF&((data)<<0))
#define DC_PC_GPU_ACK_NUM_pc_gpu_ack_num_src(data)                                   ((0xFFFFFFFF&(data))>>0)
#define DC_PC_GPU_ACK_NUM_get_pc_gpu_ack_num(data)                                   ((0xFFFFFFFF&(data))>>0)


#define DC_PC_200_PROG_0_ACK_NUM                                                     0x18008518
#define DC_PC_200_PROG_0_ACK_NUM_reg_addr                                            "0xB8008518"
#define DC_PC_200_PROG_0_ACK_NUM_reg                                                 0xB8008518
#define set_DC_PC_200_PROG_0_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_0_ACK_NUM_reg)=data)
#define get_DC_PC_200_PROG_0_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_200_PROG_0_ACK_NUM_reg))
#define DC_PC_200_PROG_0_ACK_NUM_inst_adr                                            "0x0046"
#define DC_PC_200_PROG_0_ACK_NUM_inst                                                0x0046
#define DC_PC_200_PROG_0_ACK_NUM_pc_200_prog_0_ack_num_shift                         (0)
#define DC_PC_200_PROG_0_ACK_NUM_pc_200_prog_0_ack_num_mask                          (0xFFFFFFFF)
#define DC_PC_200_PROG_0_ACK_NUM_pc_200_prog_0_ack_num(data)                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_200_PROG_0_ACK_NUM_pc_200_prog_0_ack_num_src(data)                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_200_PROG_0_ACK_NUM_get_pc_200_prog_0_ack_num(data)                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_200_PROG_1_ACK_NUM                                                     0x1800851C
#define DC_PC_200_PROG_1_ACK_NUM_reg_addr                                            "0xB800851C"
#define DC_PC_200_PROG_1_ACK_NUM_reg                                                 0xB800851C
#define set_DC_PC_200_PROG_1_ACK_NUM_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_1_ACK_NUM_reg)=data)
#define get_DC_PC_200_PROG_1_ACK_NUM_reg   (*((volatile unsigned int*) DC_PC_200_PROG_1_ACK_NUM_reg))
#define DC_PC_200_PROG_1_ACK_NUM_inst_adr                                            "0x0047"
#define DC_PC_200_PROG_1_ACK_NUM_inst                                                0x0047
#define DC_PC_200_PROG_1_ACK_NUM_pc_200_prog_1_ack_num_shift                         (0)
#define DC_PC_200_PROG_1_ACK_NUM_pc_200_prog_1_ack_num_mask                          (0xFFFFFFFF)
#define DC_PC_200_PROG_1_ACK_NUM_pc_200_prog_1_ack_num(data)                         (0xFFFFFFFF&((data)<<0))
#define DC_PC_200_PROG_1_ACK_NUM_pc_200_prog_1_ack_num_src(data)                     ((0xFFFFFFFF&(data))>>0)
#define DC_PC_200_PROG_1_ACK_NUM_get_pc_200_prog_1_ack_num(data)                     ((0xFFFFFFFF&(data))>>0)


#define DC_PC_PROG_CTRL                                                              0x180080B0
#define DC_PC_PROG_CTRL_reg_addr                                                     "0xB80080B0"
#define DC_PC_PROG_CTRL_reg                                                          0xB80080B0
#define set_DC_PC_PROG_CTRL_reg(data)   (*((volatile unsigned int*) DC_PC_PROG_CTRL_reg)=data)
#define get_DC_PC_PROG_CTRL_reg   (*((volatile unsigned int*) DC_PC_PROG_CTRL_reg))
#define DC_PC_PROG_CTRL_inst_adr                                                     "0x002C"
#define DC_PC_PROG_CTRL_inst                                                         0x002C
#define DC_PC_PROG_CTRL_write_en_6_shift                                             (29)
#define DC_PC_PROG_CTRL_write_en_6_mask                                              (0x20000000)
#define DC_PC_PROG_CTRL_write_en_6(data)                                             (0x20000000&((data)<<29))
#define DC_PC_PROG_CTRL_write_en_6_src(data)                                         ((0x20000000&(data))>>29)
#define DC_PC_PROG_CTRL_get_write_en_6(data)                                         ((0x20000000&(data))>>29)
#define DC_PC_PROG_CTRL_pc_prog_2_sel_dc_level_shift                                 (28)
#define DC_PC_PROG_CTRL_pc_prog_2_sel_dc_level_mask                                  (0x10000000)
#define DC_PC_PROG_CTRL_pc_prog_2_sel_dc_level(data)                                 (0x10000000&((data)<<28))
#define DC_PC_PROG_CTRL_pc_prog_2_sel_dc_level_src(data)                             ((0x10000000&(data))>>28)
#define DC_PC_PROG_CTRL_get_pc_prog_2_sel_dc_level(data)                             ((0x10000000&(data))>>28)
#define DC_PC_PROG_CTRL_write_en_5_shift                                             (27)
#define DC_PC_PROG_CTRL_write_en_5_mask                                              (0x08000000)
#define DC_PC_PROG_CTRL_write_en_5(data)                                             (0x08000000&((data)<<27))
#define DC_PC_PROG_CTRL_write_en_5_src(data)                                         ((0x08000000&(data))>>27)
#define DC_PC_PROG_CTRL_get_write_en_5(data)                                         ((0x08000000&(data))>>27)
#define DC_PC_PROG_CTRL_pc_prog_1_sel_dc_level_shift                                 (26)
#define DC_PC_PROG_CTRL_pc_prog_1_sel_dc_level_mask                                  (0x04000000)
#define DC_PC_PROG_CTRL_pc_prog_1_sel_dc_level(data)                                 (0x04000000&((data)<<26))
#define DC_PC_PROG_CTRL_pc_prog_1_sel_dc_level_src(data)                             ((0x04000000&(data))>>26)
#define DC_PC_PROG_CTRL_get_pc_prog_1_sel_dc_level(data)                             ((0x04000000&(data))>>26)
#define DC_PC_PROG_CTRL_write_en_4_shift                                             (25)
#define DC_PC_PROG_CTRL_write_en_4_mask                                              (0x02000000)
#define DC_PC_PROG_CTRL_write_en_4(data)                                             (0x02000000&((data)<<25))
#define DC_PC_PROG_CTRL_write_en_4_src(data)                                         ((0x02000000&(data))>>25)
#define DC_PC_PROG_CTRL_get_write_en_4(data)                                         ((0x02000000&(data))>>25)
#define DC_PC_PROG_CTRL_pc_prog_0_sel_dc_level_shift                                 (24)
#define DC_PC_PROG_CTRL_pc_prog_0_sel_dc_level_mask                                  (0x01000000)
#define DC_PC_PROG_CTRL_pc_prog_0_sel_dc_level(data)                                 (0x01000000&((data)<<24))
#define DC_PC_PROG_CTRL_pc_prog_0_sel_dc_level_src(data)                             ((0x01000000&(data))>>24)
#define DC_PC_PROG_CTRL_get_pc_prog_0_sel_dc_level(data)                             ((0x01000000&(data))>>24)
#define DC_PC_PROG_CTRL_write_en_3_shift                                             (22)
#define DC_PC_PROG_CTRL_write_en_3_mask                                              (0x00400000)
#define DC_PC_PROG_CTRL_write_en_3(data)                                             (0x00400000&((data)<<22))
#define DC_PC_PROG_CTRL_write_en_3_src(data)                                         ((0x00400000&(data))>>22)
#define DC_PC_PROG_CTRL_get_write_en_3(data)                                         ((0x00400000&(data))>>22)
#define DC_PC_PROG_CTRL_pc_prog_2_sel_shift                                          (16)
#define DC_PC_PROG_CTRL_pc_prog_2_sel_mask                                           (0x003F0000)
#define DC_PC_PROG_CTRL_pc_prog_2_sel(data)                                          (0x003F0000&((data)<<16))
#define DC_PC_PROG_CTRL_pc_prog_2_sel_src(data)                                      ((0x003F0000&(data))>>16)
#define DC_PC_PROG_CTRL_get_pc_prog_2_sel(data)                                      ((0x003F0000&(data))>>16)
#define DC_PC_PROG_CTRL_write_en_2_shift                                             (14)
#define DC_PC_PROG_CTRL_write_en_2_mask                                              (0x00004000)
#define DC_PC_PROG_CTRL_write_en_2(data)                                             (0x00004000&((data)<<14))
#define DC_PC_PROG_CTRL_write_en_2_src(data)                                         ((0x00004000&(data))>>14)
#define DC_PC_PROG_CTRL_get_write_en_2(data)                                         ((0x00004000&(data))>>14)
#define DC_PC_PROG_CTRL_pc_prog_1_sel_shift                                          (8)
#define DC_PC_PROG_CTRL_pc_prog_1_sel_mask                                           (0x00003F00)
#define DC_PC_PROG_CTRL_pc_prog_1_sel(data)                                          (0x00003F00&((data)<<8))
#define DC_PC_PROG_CTRL_pc_prog_1_sel_src(data)                                      ((0x00003F00&(data))>>8)
#define DC_PC_PROG_CTRL_get_pc_prog_1_sel(data)                                      ((0x00003F00&(data))>>8)
#define DC_PC_PROG_CTRL_write_en_1_shift                                             (6)
#define DC_PC_PROG_CTRL_write_en_1_mask                                              (0x00000040)
#define DC_PC_PROG_CTRL_write_en_1(data)                                             (0x00000040&((data)<<6))
#define DC_PC_PROG_CTRL_write_en_1_src(data)                                         ((0x00000040&(data))>>6)
#define DC_PC_PROG_CTRL_get_write_en_1(data)                                         ((0x00000040&(data))>>6)
#define DC_PC_PROG_CTRL_pc_prog_0_sel_shift                                          (0)
#define DC_PC_PROG_CTRL_pc_prog_0_sel_mask                                           (0x0000003F)
#define DC_PC_PROG_CTRL_pc_prog_0_sel(data)                                          (0x0000003F&((data)<<0))
#define DC_PC_PROG_CTRL_pc_prog_0_sel_src(data)                                      ((0x0000003F&(data))>>0)
#define DC_PC_PROG_CTRL_get_pc_prog_0_sel(data)                                      ((0x0000003F&(data))>>0)


#define DC_PC_200_PROG_CTRL                                                          0x180080B4
#define DC_PC_200_PROG_CTRL_reg_addr                                                 "0xB80080B4"
#define DC_PC_200_PROG_CTRL_reg                                                      0xB80080B4
#define set_DC_PC_200_PROG_CTRL_reg(data)   (*((volatile unsigned int*) DC_PC_200_PROG_CTRL_reg)=data)
#define get_DC_PC_200_PROG_CTRL_reg   (*((volatile unsigned int*) DC_PC_200_PROG_CTRL_reg))
#define DC_PC_200_PROG_CTRL_inst_adr                                                 "0x002D"
#define DC_PC_200_PROG_CTRL_inst                                                     0x002D
#define DC_PC_200_PROG_CTRL_write_en_2_shift                                         (14)
#define DC_PC_200_PROG_CTRL_write_en_2_mask                                          (0x00004000)
#define DC_PC_200_PROG_CTRL_write_en_2(data)                                         (0x00004000&((data)<<14))
#define DC_PC_200_PROG_CTRL_write_en_2_src(data)                                     ((0x00004000&(data))>>14)
#define DC_PC_200_PROG_CTRL_get_write_en_2(data)                                     ((0x00004000&(data))>>14)
#define DC_PC_200_PROG_CTRL_pc_200_prog_1_sel_shift                                  (8)
#define DC_PC_200_PROG_CTRL_pc_200_prog_1_sel_mask                                   (0x00003F00)
#define DC_PC_200_PROG_CTRL_pc_200_prog_1_sel(data)                                  (0x00003F00&((data)<<8))
#define DC_PC_200_PROG_CTRL_pc_200_prog_1_sel_src(data)                              ((0x00003F00&(data))>>8)
#define DC_PC_200_PROG_CTRL_get_pc_200_prog_1_sel(data)                              ((0x00003F00&(data))>>8)
#define DC_PC_200_PROG_CTRL_write_en_1_shift                                         (6)
#define DC_PC_200_PROG_CTRL_write_en_1_mask                                          (0x00000040)
#define DC_PC_200_PROG_CTRL_write_en_1(data)                                         (0x00000040&((data)<<6))
#define DC_PC_200_PROG_CTRL_write_en_1_src(data)                                     ((0x00000040&(data))>>6)
#define DC_PC_200_PROG_CTRL_get_write_en_1(data)                                     ((0x00000040&(data))>>6)
#define DC_PC_200_PROG_CTRL_pc_200_prog_0_sel_shift                                  (0)
#define DC_PC_200_PROG_CTRL_pc_200_prog_0_sel_mask                                   (0x0000003F)
#define DC_PC_200_PROG_CTRL_pc_200_prog_0_sel(data)                                  (0x0000003F&((data)<<0))
#define DC_PC_200_PROG_CTRL_pc_200_prog_0_sel_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PC_200_PROG_CTRL_get_pc_200_prog_0_sel(data)                              ((0x0000003F&(data))>>0)


#define SFMODE_PATTERN0                                                              0x180080C0
#define SFMODE_PATTERN0_reg_addr                                                     "0xB80080C0"
#define SFMODE_PATTERN0_reg                                                          0xB80080C0
#define set_SFMODE_PATTERN0_reg(data)   (*((volatile unsigned int*) SFMODE_PATTERN0_reg)=data)
#define get_SFMODE_PATTERN0_reg   (*((volatile unsigned int*) SFMODE_PATTERN0_reg))
#define SFMODE_PATTERN0_inst_adr                                                     "0x0030"
#define SFMODE_PATTERN0_inst                                                         0x0030
#define SFMODE_PATTERN0_sfmode_pattern0_shift                                        (0)
#define SFMODE_PATTERN0_sfmode_pattern0_mask                                         (0xFFFFFFFF)
#define SFMODE_PATTERN0_sfmode_pattern0(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_PATTERN0_sfmode_pattern0_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_PATTERN0_get_sfmode_pattern0(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_PATTERN1                                                              0x180080C4
#define SFMODE_PATTERN1_reg_addr                                                     "0xB80080C4"
#define SFMODE_PATTERN1_reg                                                          0xB80080C4
#define set_SFMODE_PATTERN1_reg(data)   (*((volatile unsigned int*) SFMODE_PATTERN1_reg)=data)
#define get_SFMODE_PATTERN1_reg   (*((volatile unsigned int*) SFMODE_PATTERN1_reg))
#define SFMODE_PATTERN1_inst_adr                                                     "0x0031"
#define SFMODE_PATTERN1_inst                                                         0x0031
#define SFMODE_PATTERN1_sfmode_pattern1_shift                                        (0)
#define SFMODE_PATTERN1_sfmode_pattern1_mask                                         (0xFFFFFFFF)
#define SFMODE_PATTERN1_sfmode_pattern1(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_PATTERN1_sfmode_pattern1_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_PATTERN1_get_sfmode_pattern1(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_PATTERN2                                                              0x180080C8
#define SFMODE_PATTERN2_reg_addr                                                     "0xB80080C8"
#define SFMODE_PATTERN2_reg                                                          0xB80080C8
#define set_SFMODE_PATTERN2_reg(data)   (*((volatile unsigned int*) SFMODE_PATTERN2_reg)=data)
#define get_SFMODE_PATTERN2_reg   (*((volatile unsigned int*) SFMODE_PATTERN2_reg))
#define SFMODE_PATTERN2_inst_adr                                                     "0x0032"
#define SFMODE_PATTERN2_inst                                                         0x0032
#define SFMODE_PATTERN2_sfmode_pattern2_shift                                        (0)
#define SFMODE_PATTERN2_sfmode_pattern2_mask                                         (0xFFFFFFFF)
#define SFMODE_PATTERN2_sfmode_pattern2(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_PATTERN2_sfmode_pattern2_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_PATTERN2_get_sfmode_pattern2(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_PATTERN3                                                              0x180080CC
#define SFMODE_PATTERN3_reg_addr                                                     "0xB80080CC"
#define SFMODE_PATTERN3_reg                                                          0xB80080CC
#define set_SFMODE_PATTERN3_reg(data)   (*((volatile unsigned int*) SFMODE_PATTERN3_reg)=data)
#define get_SFMODE_PATTERN3_reg   (*((volatile unsigned int*) SFMODE_PATTERN3_reg))
#define SFMODE_PATTERN3_inst_adr                                                     "0x0033"
#define SFMODE_PATTERN3_inst                                                         0x0033
#define SFMODE_PATTERN3_sfmode_pattern3_shift                                        (0)
#define SFMODE_PATTERN3_sfmode_pattern3_mask                                         (0xFFFFFFFF)
#define SFMODE_PATTERN3_sfmode_pattern3(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_PATTERN3_sfmode_pattern3_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_PATTERN3_get_sfmode_pattern3(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_CMP_RLT0                                                              0x180080D0
#define SFMODE_CMP_RLT0_reg_addr                                                     "0xB80080D0"
#define SFMODE_CMP_RLT0_reg                                                          0xB80080D0
#define set_SFMODE_CMP_RLT0_reg(data)   (*((volatile unsigned int*) SFMODE_CMP_RLT0_reg)=data)
#define get_SFMODE_CMP_RLT0_reg   (*((volatile unsigned int*) SFMODE_CMP_RLT0_reg))
#define SFMODE_CMP_RLT0_inst_adr                                                     "0x0034"
#define SFMODE_CMP_RLT0_inst                                                         0x0034
#define SFMODE_CMP_RLT0_sfmode_cmp_rlt0_shift                                        (0)
#define SFMODE_CMP_RLT0_sfmode_cmp_rlt0_mask                                         (0xFFFFFFFF)
#define SFMODE_CMP_RLT0_sfmode_cmp_rlt0(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_CMP_RLT0_sfmode_cmp_rlt0_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_CMP_RLT0_get_sfmode_cmp_rlt0(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_CMP_RLT1                                                              0x180080D4
#define SFMODE_CMP_RLT1_reg_addr                                                     "0xB80080D4"
#define SFMODE_CMP_RLT1_reg                                                          0xB80080D4
#define set_SFMODE_CMP_RLT1_reg(data)   (*((volatile unsigned int*) SFMODE_CMP_RLT1_reg)=data)
#define get_SFMODE_CMP_RLT1_reg   (*((volatile unsigned int*) SFMODE_CMP_RLT1_reg))
#define SFMODE_CMP_RLT1_inst_adr                                                     "0x0035"
#define SFMODE_CMP_RLT1_inst                                                         0x0035
#define SFMODE_CMP_RLT1_sfmode_cmp_rlt1_shift                                        (0)
#define SFMODE_CMP_RLT1_sfmode_cmp_rlt1_mask                                         (0xFFFFFFFF)
#define SFMODE_CMP_RLT1_sfmode_cmp_rlt1(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_CMP_RLT1_sfmode_cmp_rlt1_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_CMP_RLT1_get_sfmode_cmp_rlt1(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_CMP_RLT2                                                              0x180080D8
#define SFMODE_CMP_RLT2_reg_addr                                                     "0xB80080D8"
#define SFMODE_CMP_RLT2_reg                                                          0xB80080D8
#define set_SFMODE_CMP_RLT2_reg(data)   (*((volatile unsigned int*) SFMODE_CMP_RLT2_reg)=data)
#define get_SFMODE_CMP_RLT2_reg   (*((volatile unsigned int*) SFMODE_CMP_RLT2_reg))
#define SFMODE_CMP_RLT2_inst_adr                                                     "0x0036"
#define SFMODE_CMP_RLT2_inst                                                         0x0036
#define SFMODE_CMP_RLT2_sfmode_cmp_rlt2_shift                                        (0)
#define SFMODE_CMP_RLT2_sfmode_cmp_rlt2_mask                                         (0xFFFFFFFF)
#define SFMODE_CMP_RLT2_sfmode_cmp_rlt2(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_CMP_RLT2_sfmode_cmp_rlt2_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_CMP_RLT2_get_sfmode_cmp_rlt2(data)                                    ((0xFFFFFFFF&(data))>>0)


#define SFMODE_CMP_RLT3                                                              0x180080DC
#define SFMODE_CMP_RLT3_reg_addr                                                     "0xB80080DC"
#define SFMODE_CMP_RLT3_reg                                                          0xB80080DC
#define set_SFMODE_CMP_RLT3_reg(data)   (*((volatile unsigned int*) SFMODE_CMP_RLT3_reg)=data)
#define get_SFMODE_CMP_RLT3_reg   (*((volatile unsigned int*) SFMODE_CMP_RLT3_reg))
#define SFMODE_CMP_RLT3_inst_adr                                                     "0x0037"
#define SFMODE_CMP_RLT3_inst                                                         0x0037
#define SFMODE_CMP_RLT3_sfmode_cmp_rlt3_shift                                        (0)
#define SFMODE_CMP_RLT3_sfmode_cmp_rlt3_mask                                         (0xFFFFFFFF)
#define SFMODE_CMP_RLT3_sfmode_cmp_rlt3(data)                                        (0xFFFFFFFF&((data)<<0))
#define SFMODE_CMP_RLT3_sfmode_cmp_rlt3_src(data)                                    ((0xFFFFFFFF&(data))>>0)
#define SFMODE_CMP_RLT3_get_sfmode_cmp_rlt3(data)                                    ((0xFFFFFFFF&(data))>>0)


#define DC_64_WRITE_BUF_CTRL                                                         0x180080E0
#define DC_64_WRITE_BUF_CTRL_reg_addr                                                "0xB80080E0"
#define DC_64_WRITE_BUF_CTRL_reg                                                     0xB80080E0
#define set_DC_64_WRITE_BUF_CTRL_reg(data)   (*((volatile unsigned int*) DC_64_WRITE_BUF_CTRL_reg)=data)
#define get_DC_64_WRITE_BUF_CTRL_reg   (*((volatile unsigned int*) DC_64_WRITE_BUF_CTRL_reg))
#define DC_64_WRITE_BUF_CTRL_inst_adr                                                "0x0038"
#define DC_64_WRITE_BUF_CTRL_inst                                                    0x0038
#define DC_64_WRITE_BUF_CTRL_disable_2nd_channel_shift                               (1)
#define DC_64_WRITE_BUF_CTRL_disable_2nd_channel_mask                                (0x00000002)
#define DC_64_WRITE_BUF_CTRL_disable_2nd_channel(data)                               (0x00000002&((data)<<1))
#define DC_64_WRITE_BUF_CTRL_disable_2nd_channel_src(data)                           ((0x00000002&(data))>>1)
#define DC_64_WRITE_BUF_CTRL_get_disable_2nd_channel(data)                           ((0x00000002&(data))>>1)
#define DC_64_WRITE_BUF_CTRL_full_transfer_shift                                     (0)
#define DC_64_WRITE_BUF_CTRL_full_transfer_mask                                      (0x00000001)
#define DC_64_WRITE_BUF_CTRL_full_transfer(data)                                     (0x00000001&((data)<<0))
#define DC_64_WRITE_BUF_CTRL_full_transfer_src(data)                                 ((0x00000001&(data))>>0)
#define DC_64_WRITE_BUF_CTRL_get_full_transfer(data)                                 ((0x00000001&(data))>>0)


#define DC_64_READ_BUF_CTRL                                                          0x180080E4
#define DC_64_READ_BUF_CTRL_reg_addr                                                 "0xB80080E4"
#define DC_64_READ_BUF_CTRL_reg                                                      0xB80080E4
#define set_DC_64_READ_BUF_CTRL_reg(data)   (*((volatile unsigned int*) DC_64_READ_BUF_CTRL_reg)=data)
#define get_DC_64_READ_BUF_CTRL_reg   (*((volatile unsigned int*) DC_64_READ_BUF_CTRL_reg))
#define DC_64_READ_BUF_CTRL_inst_adr                                                 "0x0039"
#define DC_64_READ_BUF_CTRL_inst                                                     0x0039
#define DC_64_READ_BUF_CTRL_sync_cmd_cnt_threshold_shift                             (21)
#define DC_64_READ_BUF_CTRL_sync_cmd_cnt_threshold_mask                              (0x00E00000)
#define DC_64_READ_BUF_CTRL_sync_cmd_cnt_threshold(data)                             (0x00E00000&((data)<<21))
#define DC_64_READ_BUF_CTRL_sync_cmd_cnt_threshold_src(data)                         ((0x00E00000&(data))>>21)
#define DC_64_READ_BUF_CTRL_get_sync_cmd_cnt_threshold(data)                         ((0x00E00000&(data))>>21)
#define DC_64_READ_BUF_CTRL_cmd_buf_threshold_shift                                  (17)
#define DC_64_READ_BUF_CTRL_cmd_buf_threshold_mask                                   (0x001E0000)
#define DC_64_READ_BUF_CTRL_cmd_buf_threshold(data)                                  (0x001E0000&((data)<<17))
#define DC_64_READ_BUF_CTRL_cmd_buf_threshold_src(data)                              ((0x001E0000&(data))>>17)
#define DC_64_READ_BUF_CTRL_get_cmd_buf_threshold(data)                              ((0x001E0000&(data))>>17)
#define DC_64_READ_BUF_CTRL_disable_2nd_channel_shift                                (16)
#define DC_64_READ_BUF_CTRL_disable_2nd_channel_mask                                 (0x00010000)
#define DC_64_READ_BUF_CTRL_disable_2nd_channel(data)                                (0x00010000&((data)<<16))
#define DC_64_READ_BUF_CTRL_disable_2nd_channel_src(data)                            ((0x00010000&(data))>>16)
#define DC_64_READ_BUF_CTRL_get_disable_2nd_channel(data)                            ((0x00010000&(data))>>16)
#define DC_64_READ_BUF_CTRL_ever_full_shift                                          (15)
#define DC_64_READ_BUF_CTRL_ever_full_mask                                           (0x00008000)
#define DC_64_READ_BUF_CTRL_ever_full(data)                                          (0x00008000&((data)<<15))
#define DC_64_READ_BUF_CTRL_ever_full_src(data)                                      ((0x00008000&(data))>>15)
#define DC_64_READ_BUF_CTRL_get_ever_full(data)                                      ((0x00008000&(data))>>15)
#define DC_64_READ_BUF_CTRL_ever_reach_threshold_shift                               (14)
#define DC_64_READ_BUF_CTRL_ever_reach_threshold_mask                                (0x00004000)
#define DC_64_READ_BUF_CTRL_ever_reach_threshold(data)                               (0x00004000&((data)<<14))
#define DC_64_READ_BUF_CTRL_ever_reach_threshold_src(data)                           ((0x00004000&(data))>>14)
#define DC_64_READ_BUF_CTRL_get_ever_reach_threshold(data)                           ((0x00004000&(data))>>14)
#define DC_64_READ_BUF_CTRL_max_buf_level_shift                                      (7)
#define DC_64_READ_BUF_CTRL_max_buf_level_mask                                       (0x00003F80)
#define DC_64_READ_BUF_CTRL_max_buf_level(data)                                      (0x00003F80&((data)<<7))
#define DC_64_READ_BUF_CTRL_max_buf_level_src(data)                                  ((0x00003F80&(data))>>7)
#define DC_64_READ_BUF_CTRL_get_max_buf_level(data)                                  ((0x00003F80&(data))>>7)
#define DC_64_READ_BUF_CTRL_grant_stop_level_shift                                   (0)
#define DC_64_READ_BUF_CTRL_grant_stop_level_mask                                    (0x0000007F)
#define DC_64_READ_BUF_CTRL_grant_stop_level(data)                                   (0x0000007F&((data)<<0))
#define DC_64_READ_BUF_CTRL_grant_stop_level_src(data)                               ((0x0000007F&(data))>>0)
#define DC_64_READ_BUF_CTRL_get_grant_stop_level(data)                               ((0x0000007F&(data))>>0)


#define DC_PIC_OFFSET_X_DETECTION                                                    0x18008110
#define DC_PIC_OFFSET_X_DETECTION_reg_addr                                           "0xB8008110"
#define DC_PIC_OFFSET_X_DETECTION_reg                                                0xB8008110
#define set_DC_PIC_OFFSET_X_DETECTION_reg(data)   (*((volatile unsigned int*) DC_PIC_OFFSET_X_DETECTION_reg)=data)
#define get_DC_PIC_OFFSET_X_DETECTION_reg   (*((volatile unsigned int*) DC_PIC_OFFSET_X_DETECTION_reg))
#define DC_PIC_OFFSET_X_DETECTION_inst_adr                                           "0x0044"
#define DC_PIC_OFFSET_X_DETECTION_inst                                               0x0044
#define DC_PIC_OFFSET_X_DETECTION_pict_set_int_cpu_shift                             (3)
#define DC_PIC_OFFSET_X_DETECTION_pict_set_int_cpu_mask                              (0x00000018)
#define DC_PIC_OFFSET_X_DETECTION_pict_set_int_cpu(data)                             (0x00000018&((data)<<3))
#define DC_PIC_OFFSET_X_DETECTION_pict_set_int_cpu_src(data)                         ((0x00000018&(data))>>3)
#define DC_PIC_OFFSET_X_DETECTION_get_pict_set_int_cpu(data)                         ((0x00000018&(data))>>3)
#define DC_PIC_OFFSET_X_DETECTION_int_enable_shift                                   (2)
#define DC_PIC_OFFSET_X_DETECTION_int_enable_mask                                    (0x00000004)
#define DC_PIC_OFFSET_X_DETECTION_int_enable(data)                                   (0x00000004&((data)<<2))
#define DC_PIC_OFFSET_X_DETECTION_int_enable_src(data)                               ((0x00000004&(data))>>2)
#define DC_PIC_OFFSET_X_DETECTION_get_int_enable(data)                               ((0x00000004&(data))>>2)
#define DC_PIC_OFFSET_X_DETECTION_ever_set_as_odd_shift                              (1)
#define DC_PIC_OFFSET_X_DETECTION_ever_set_as_odd_mask                               (0x00000002)
#define DC_PIC_OFFSET_X_DETECTION_ever_set_as_odd(data)                              (0x00000002&((data)<<1))
#define DC_PIC_OFFSET_X_DETECTION_ever_set_as_odd_src(data)                          ((0x00000002&(data))>>1)
#define DC_PIC_OFFSET_X_DETECTION_get_ever_set_as_odd(data)                          ((0x00000002&(data))>>1)
#define DC_PIC_OFFSET_X_DETECTION_enable_shift                                       (0)
#define DC_PIC_OFFSET_X_DETECTION_enable_mask                                        (0x00000001)
#define DC_PIC_OFFSET_X_DETECTION_enable(data)                                       (0x00000001&((data)<<0))
#define DC_PIC_OFFSET_X_DETECTION_enable_src(data)                                   ((0x00000001&(data))>>0)
#define DC_PIC_OFFSET_X_DETECTION_get_enable(data)                                   ((0x00000001&(data))>>0)


#define DUMMY_REG0                                                                   0x180087F0
#define DUMMY_REG0_reg_addr                                                          "0xB80087F0"
#define DUMMY_REG0_reg                                                               0xB80087F0
#define set_DUMMY_REG0_reg(data)   (*((volatile unsigned int*) DUMMY_REG0_reg)=data)
#define get_DUMMY_REG0_reg   (*((volatile unsigned int*) DUMMY_REG0_reg))
#define DUMMY_REG0_inst_adr                                                          "0x00FC"
#define DUMMY_REG0_inst                                                              0x00FC
#define DUMMY_REG0_dummy_reg0_shift                                                  (0)
#define DUMMY_REG0_dummy_reg0_mask                                                   (0xFFFFFFFF)
#define DUMMY_REG0_dummy_reg0(data)                                                  (0xFFFFFFFF&((data)<<0))
#define DUMMY_REG0_dummy_reg0_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define DUMMY_REG0_get_dummy_reg0(data)                                              ((0xFFFFFFFF&(data))>>0)


#define DUMMY_REG1                                                                   0x180087F4
#define DUMMY_REG1_reg_addr                                                          "0xB80087F4"
#define DUMMY_REG1_reg                                                               0xB80087F4
#define set_DUMMY_REG1_reg(data)   (*((volatile unsigned int*) DUMMY_REG1_reg)=data)
#define get_DUMMY_REG1_reg   (*((volatile unsigned int*) DUMMY_REG1_reg))
#define DUMMY_REG1_inst_adr                                                          "0x00FD"
#define DUMMY_REG1_inst                                                              0x00FD
#define DUMMY_REG1_dummy_reg1_shift                                                  (0)
#define DUMMY_REG1_dummy_reg1_mask                                                   (0xFFFFFFFF)
#define DUMMY_REG1_dummy_reg1(data)                                                  (0xFFFFFFFF&((data)<<0))
#define DUMMY_REG1_dummy_reg1_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define DUMMY_REG1_get_dummy_reg1(data)                                              ((0xFFFFFFFF&(data))>>0)


#define DUMMY_REG2                                                                   0x180087F8
#define DUMMY_REG2_reg_addr                                                          "0xB80087F8"
#define DUMMY_REG2_reg                                                               0xB80087F8
#define set_DUMMY_REG2_reg(data)   (*((volatile unsigned int*) DUMMY_REG2_reg)=data)
#define get_DUMMY_REG2_reg   (*((volatile unsigned int*) DUMMY_REG2_reg))
#define DUMMY_REG2_inst_adr                                                          "0x00FE"
#define DUMMY_REG2_inst                                                              0x00FE
#define DUMMY_REG2_dummy_reg2_shift                                                  (0)
#define DUMMY_REG2_dummy_reg2_mask                                                   (0xFFFFFFFF)
#define DUMMY_REG2_dummy_reg2(data)                                                  (0xFFFFFFFF&((data)<<0))
#define DUMMY_REG2_dummy_reg2_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define DUMMY_REG2_get_dummy_reg2(data)                                              ((0xFFFFFFFF&(data))>>0)


#define DUMMY_REG3                                                                   0x180087FC
#define DUMMY_REG3_reg_addr                                                          "0xB80087FC"
#define DUMMY_REG3_reg                                                               0xB80087FC
#define set_DUMMY_REG3_reg(data)   (*((volatile unsigned int*) DUMMY_REG3_reg)=data)
#define get_DUMMY_REG3_reg   (*((volatile unsigned int*) DUMMY_REG3_reg))
#define DUMMY_REG3_inst_adr                                                          "0x00FF"
#define DUMMY_REG3_inst                                                              0x00FF
#define DUMMY_REG3_dummy_reg3_shift                                                  (0)
#define DUMMY_REG3_dummy_reg3_mask                                                   (0xFFFFFFFF)
#define DUMMY_REG3_dummy_reg3(data)                                                  (0xFFFFFFFF&((data)<<0))
#define DUMMY_REG3_dummy_reg3_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define DUMMY_REG3_get_dummy_reg3(data)                                              ((0xFFFFFFFF&(data))>>0)


#define DC_MT_SADDR_0                                                                0x18008200
#define DC_MT_SADDR_1                                                                0x18008204
#define DC_MT_SADDR_2                                                                0x18008208
#define DC_MT_SADDR_3                                                                0x1800820C
#define DC_MT_SADDR_4                                                                0x18008210
#define DC_MT_SADDR_5                                                                0x18008214
#define DC_MT_SADDR_6                                                                0x18008218
#define DC_MT_SADDR_7                                                                0x1800821C
#define DC_MT_SADDR_0_reg_addr                                                       "0xB8008200"
#define DC_MT_SADDR_1_reg_addr                                                       "0xB8008204"
#define DC_MT_SADDR_2_reg_addr                                                       "0xB8008208"
#define DC_MT_SADDR_3_reg_addr                                                       "0xB800820C"
#define DC_MT_SADDR_4_reg_addr                                                       "0xB8008210"
#define DC_MT_SADDR_5_reg_addr                                                       "0xB8008214"
#define DC_MT_SADDR_6_reg_addr                                                       "0xB8008218"
#define DC_MT_SADDR_7_reg_addr                                                       "0xB800821C"
#define DC_MT_SADDR_0_reg                                                            0xB8008200
#define DC_MT_SADDR_1_reg                                                            0xB8008204
#define DC_MT_SADDR_2_reg                                                            0xB8008208
#define DC_MT_SADDR_3_reg                                                            0xB800820C
#define DC_MT_SADDR_4_reg                                                            0xB8008210
#define DC_MT_SADDR_5_reg                                                            0xB8008214
#define DC_MT_SADDR_6_reg                                                            0xB8008218
#define DC_MT_SADDR_7_reg                                                            0xB800821C
#define set_DC_MT_SADDR_0_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_0_reg)=data)
#define set_DC_MT_SADDR_1_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_1_reg)=data)
#define set_DC_MT_SADDR_2_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_2_reg)=data)
#define set_DC_MT_SADDR_3_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_3_reg)=data)
#define set_DC_MT_SADDR_4_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_4_reg)=data)
#define set_DC_MT_SADDR_5_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_5_reg)=data)
#define set_DC_MT_SADDR_6_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_6_reg)=data)
#define set_DC_MT_SADDR_7_reg(data)   (*((volatile unsigned int*) DC_MT_SADDR_7_reg)=data)
#define get_DC_MT_SADDR_0_reg   (*((volatile unsigned int*) DC_MT_SADDR_0_reg))
#define get_DC_MT_SADDR_1_reg   (*((volatile unsigned int*) DC_MT_SADDR_1_reg))
#define get_DC_MT_SADDR_2_reg   (*((volatile unsigned int*) DC_MT_SADDR_2_reg))
#define get_DC_MT_SADDR_3_reg   (*((volatile unsigned int*) DC_MT_SADDR_3_reg))
#define get_DC_MT_SADDR_4_reg   (*((volatile unsigned int*) DC_MT_SADDR_4_reg))
#define get_DC_MT_SADDR_5_reg   (*((volatile unsigned int*) DC_MT_SADDR_5_reg))
#define get_DC_MT_SADDR_6_reg   (*((volatile unsigned int*) DC_MT_SADDR_6_reg))
#define get_DC_MT_SADDR_7_reg   (*((volatile unsigned int*) DC_MT_SADDR_7_reg))
#define DC_MT_SADDR_0_inst_adr                                                       "0x0080"
#define DC_MT_SADDR_1_inst_adr                                                       "0x0081"
#define DC_MT_SADDR_2_inst_adr                                                       "0x0082"
#define DC_MT_SADDR_3_inst_adr                                                       "0x0083"
#define DC_MT_SADDR_4_inst_adr                                                       "0x0084"
#define DC_MT_SADDR_5_inst_adr                                                       "0x0085"
#define DC_MT_SADDR_6_inst_adr                                                       "0x0086"
#define DC_MT_SADDR_7_inst_adr                                                       "0x0087"
#define DC_MT_SADDR_0_inst                                                           0x0080
#define DC_MT_SADDR_1_inst                                                           0x0081
#define DC_MT_SADDR_2_inst                                                           0x0082
#define DC_MT_SADDR_3_inst                                                           0x0083
#define DC_MT_SADDR_4_inst                                                           0x0084
#define DC_MT_SADDR_5_inst                                                           0x0085
#define DC_MT_SADDR_6_inst                                                           0x0086
#define DC_MT_SADDR_7_inst                                                           0x0087
#define DC_MT_SADDR_mem_type_shift                                                   (30)
#define DC_MT_SADDR_mem_type_mask                                                    (0xC0000000)
#define DC_MT_SADDR_mem_type(data)                                                   (0xC0000000&((data)<<30))
#define DC_MT_SADDR_mem_type_src(data)                                               ((0xC0000000&(data))>>30)
#define DC_MT_SADDR_get_mem_type(data)                                               ((0xC0000000&(data))>>30)
#define DC_MT_SADDR_saddr_shift                                                      (4)
#define DC_MT_SADDR_saddr_mask                                                       (0x3FFFFFF0)
#define DC_MT_SADDR_saddr(data)                                                      (0x3FFFFFF0&((data)<<4))
#define DC_MT_SADDR_saddr_src(data)                                                  ((0x3FFFFFF0&(data))>>4)
#define DC_MT_SADDR_get_saddr(data)                                                  ((0x3FFFFFF0&(data))>>4)


#define DC_MT_EADDR_0                                                                0x18008220
#define DC_MT_EADDR_1                                                                0x18008224
#define DC_MT_EADDR_2                                                                0x18008228
#define DC_MT_EADDR_3                                                                0x1800822C
#define DC_MT_EADDR_4                                                                0x18008230
#define DC_MT_EADDR_5                                                                0x18008234
#define DC_MT_EADDR_6                                                                0x18008238
#define DC_MT_EADDR_7                                                                0x1800823C
#define DC_MT_EADDR_0_reg_addr                                                       "0xB8008220"
#define DC_MT_EADDR_1_reg_addr                                                       "0xB8008224"
#define DC_MT_EADDR_2_reg_addr                                                       "0xB8008228"
#define DC_MT_EADDR_3_reg_addr                                                       "0xB800822C"
#define DC_MT_EADDR_4_reg_addr                                                       "0xB8008230"
#define DC_MT_EADDR_5_reg_addr                                                       "0xB8008234"
#define DC_MT_EADDR_6_reg_addr                                                       "0xB8008238"
#define DC_MT_EADDR_7_reg_addr                                                       "0xB800823C"
#define DC_MT_EADDR_0_reg                                                            0xB8008220
#define DC_MT_EADDR_1_reg                                                            0xB8008224
#define DC_MT_EADDR_2_reg                                                            0xB8008228
#define DC_MT_EADDR_3_reg                                                            0xB800822C
#define DC_MT_EADDR_4_reg                                                            0xB8008230
#define DC_MT_EADDR_5_reg                                                            0xB8008234
#define DC_MT_EADDR_6_reg                                                            0xB8008238
#define DC_MT_EADDR_7_reg                                                            0xB800823C
#define set_DC_MT_EADDR_0_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_0_reg)=data)
#define set_DC_MT_EADDR_1_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_1_reg)=data)
#define set_DC_MT_EADDR_2_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_2_reg)=data)
#define set_DC_MT_EADDR_3_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_3_reg)=data)
#define set_DC_MT_EADDR_4_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_4_reg)=data)
#define set_DC_MT_EADDR_5_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_5_reg)=data)
#define set_DC_MT_EADDR_6_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_6_reg)=data)
#define set_DC_MT_EADDR_7_reg(data)   (*((volatile unsigned int*) DC_MT_EADDR_7_reg)=data)
#define get_DC_MT_EADDR_0_reg   (*((volatile unsigned int*) DC_MT_EADDR_0_reg))
#define get_DC_MT_EADDR_1_reg   (*((volatile unsigned int*) DC_MT_EADDR_1_reg))
#define get_DC_MT_EADDR_2_reg   (*((volatile unsigned int*) DC_MT_EADDR_2_reg))
#define get_DC_MT_EADDR_3_reg   (*((volatile unsigned int*) DC_MT_EADDR_3_reg))
#define get_DC_MT_EADDR_4_reg   (*((volatile unsigned int*) DC_MT_EADDR_4_reg))
#define get_DC_MT_EADDR_5_reg   (*((volatile unsigned int*) DC_MT_EADDR_5_reg))
#define get_DC_MT_EADDR_6_reg   (*((volatile unsigned int*) DC_MT_EADDR_6_reg))
#define get_DC_MT_EADDR_7_reg   (*((volatile unsigned int*) DC_MT_EADDR_7_reg))
#define DC_MT_EADDR_0_inst_adr                                                       "0x0088"
#define DC_MT_EADDR_1_inst_adr                                                       "0x0089"
#define DC_MT_EADDR_2_inst_adr                                                       "0x008A"
#define DC_MT_EADDR_3_inst_adr                                                       "0x008B"
#define DC_MT_EADDR_4_inst_adr                                                       "0x008C"
#define DC_MT_EADDR_5_inst_adr                                                       "0x008D"
#define DC_MT_EADDR_6_inst_adr                                                       "0x008E"
#define DC_MT_EADDR_7_inst_adr                                                       "0x008F"
#define DC_MT_EADDR_0_inst                                                           0x0088
#define DC_MT_EADDR_1_inst                                                           0x0089
#define DC_MT_EADDR_2_inst                                                           0x008A
#define DC_MT_EADDR_3_inst                                                           0x008B
#define DC_MT_EADDR_4_inst                                                           0x008C
#define DC_MT_EADDR_5_inst                                                           0x008D
#define DC_MT_EADDR_6_inst                                                           0x008E
#define DC_MT_EADDR_7_inst                                                           0x008F
#define DC_MT_EADDR_eaddr_shift                                                      (5)
#define DC_MT_EADDR_eaddr_mask                                                       (0x7FFFFFE0)
#define DC_MT_EADDR_eaddr(data)                                                      (0x7FFFFFE0&((data)<<5))
#define DC_MT_EADDR_eaddr_src(data)                                                  ((0x7FFFFFE0&(data))>>5)
#define DC_MT_EADDR_get_eaddr(data)                                                  ((0x7FFFFFE0&(data))>>5)


#define DC_MT_MODE_0                                                                 0x18008240
#define DC_MT_MODE_1                                                                 0x18008244
#define DC_MT_MODE_2                                                                 0x18008248
#define DC_MT_MODE_3                                                                 0x1800824C
#define DC_MT_MODE_4                                                                 0x18008250
#define DC_MT_MODE_5                                                                 0x18008254
#define DC_MT_MODE_6                                                                 0x18008258
#define DC_MT_MODE_7                                                                 0x1800825C
#define DC_MT_MODE_0_reg_addr                                                        "0xB8008240"
#define DC_MT_MODE_1_reg_addr                                                        "0xB8008244"
#define DC_MT_MODE_2_reg_addr                                                        "0xB8008248"
#define DC_MT_MODE_3_reg_addr                                                        "0xB800824C"
#define DC_MT_MODE_4_reg_addr                                                        "0xB8008250"
#define DC_MT_MODE_5_reg_addr                                                        "0xB8008254"
#define DC_MT_MODE_6_reg_addr                                                        "0xB8008258"
#define DC_MT_MODE_7_reg_addr                                                        "0xB800825C"
#define DC_MT_MODE_0_reg                                                             0xB8008240
#define DC_MT_MODE_1_reg                                                             0xB8008244
#define DC_MT_MODE_2_reg                                                             0xB8008248
#define DC_MT_MODE_3_reg                                                             0xB800824C
#define DC_MT_MODE_4_reg                                                             0xB8008250
#define DC_MT_MODE_5_reg                                                             0xB8008254
#define DC_MT_MODE_6_reg                                                             0xB8008258
#define DC_MT_MODE_7_reg                                                             0xB800825C
#define set_DC_MT_MODE_0_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_0_reg)=data)
#define set_DC_MT_MODE_1_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_1_reg)=data)
#define set_DC_MT_MODE_2_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_2_reg)=data)
#define set_DC_MT_MODE_3_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_3_reg)=data)
#define set_DC_MT_MODE_4_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_4_reg)=data)
#define set_DC_MT_MODE_5_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_5_reg)=data)
#define set_DC_MT_MODE_6_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_6_reg)=data)
#define set_DC_MT_MODE_7_reg(data)   (*((volatile unsigned int*) DC_MT_MODE_7_reg)=data)
#define get_DC_MT_MODE_0_reg   (*((volatile unsigned int*) DC_MT_MODE_0_reg))
#define get_DC_MT_MODE_1_reg   (*((volatile unsigned int*) DC_MT_MODE_1_reg))
#define get_DC_MT_MODE_2_reg   (*((volatile unsigned int*) DC_MT_MODE_2_reg))
#define get_DC_MT_MODE_3_reg   (*((volatile unsigned int*) DC_MT_MODE_3_reg))
#define get_DC_MT_MODE_4_reg   (*((volatile unsigned int*) DC_MT_MODE_4_reg))
#define get_DC_MT_MODE_5_reg   (*((volatile unsigned int*) DC_MT_MODE_5_reg))
#define get_DC_MT_MODE_6_reg   (*((volatile unsigned int*) DC_MT_MODE_6_reg))
#define get_DC_MT_MODE_7_reg   (*((volatile unsigned int*) DC_MT_MODE_7_reg))
#define DC_MT_MODE_0_inst_adr                                                        "0x0090"
#define DC_MT_MODE_1_inst_adr                                                        "0x0091"
#define DC_MT_MODE_2_inst_adr                                                        "0x0092"
#define DC_MT_MODE_3_inst_adr                                                        "0x0093"
#define DC_MT_MODE_4_inst_adr                                                        "0x0094"
#define DC_MT_MODE_5_inst_adr                                                        "0x0095"
#define DC_MT_MODE_6_inst_adr                                                        "0x0096"
#define DC_MT_MODE_7_inst_adr                                                        "0x0097"
#define DC_MT_MODE_0_inst                                                            0x0090
#define DC_MT_MODE_1_inst                                                            0x0091
#define DC_MT_MODE_2_inst                                                            0x0092
#define DC_MT_MODE_3_inst                                                            0x0093
#define DC_MT_MODE_4_inst                                                            0x0094
#define DC_MT_MODE_5_inst                                                            0x0095
#define DC_MT_MODE_6_inst                                                            0x0096
#define DC_MT_MODE_7_inst                                                            0x0097
#define DC_MT_MODE_write_enable1_shift                                               (14)
#define DC_MT_MODE_write_enable1_mask                                                (0x00004000)
#define DC_MT_MODE_write_enable1(data)                                               (0x00004000&((data)<<14))
#define DC_MT_MODE_write_enable1_src(data)                                           ((0x00004000&(data))>>14)
#define DC_MT_MODE_get_write_enable1(data)                                           ((0x00004000&(data))>>14)
#define DC_MT_MODE_int1_shift                                                        (13)
#define DC_MT_MODE_int1_mask                                                         (0x00002000)
#define DC_MT_MODE_int1(data)                                                        (0x00002000&((data)<<13))
#define DC_MT_MODE_int1_src(data)                                                    ((0x00002000&(data))>>13)
#define DC_MT_MODE_get_int1(data)                                                    ((0x00002000&(data))>>13)
#define DC_MT_MODE_write_enable2_shift                                               (12)
#define DC_MT_MODE_write_enable2_mask                                                (0x00001000)
#define DC_MT_MODE_write_enable2(data)                                               (0x00001000&((data)<<12))
#define DC_MT_MODE_write_enable2_src(data)                                           ((0x00001000&(data))>>12)
#define DC_MT_MODE_get_write_enable2(data)                                           ((0x00001000&(data))>>12)
#define DC_MT_MODE_int2_shift                                                        (11)
#define DC_MT_MODE_int2_mask                                                         (0x00000800)
#define DC_MT_MODE_int2(data)                                                        (0x00000800&((data)<<11))
#define DC_MT_MODE_int2_src(data)                                                    ((0x00000800&(data))>>11)
#define DC_MT_MODE_get_int2(data)                                                    ((0x00000800&(data))>>11)
#define DC_MT_MODE_write_enable3_shift                                               (10)
#define DC_MT_MODE_write_enable3_mask                                                (0x00000400)
#define DC_MT_MODE_write_enable3(data)                                               (0x00000400&((data)<<10))
#define DC_MT_MODE_write_enable3_src(data)                                           ((0x00000400&(data))>>10)
#define DC_MT_MODE_get_write_enable3(data)                                           ((0x00000400&(data))>>10)
#define DC_MT_MODE_int3_shift                                                        (9)
#define DC_MT_MODE_int3_mask                                                         (0x00000200)
#define DC_MT_MODE_int3(data)                                                        (0x00000200&((data)<<9))
#define DC_MT_MODE_int3_src(data)                                                    ((0x00000200&(data))>>9)
#define DC_MT_MODE_get_int3(data)                                                    ((0x00000200&(data))>>9)
#define DC_MT_MODE_write_enable4_shift                                               (8)
#define DC_MT_MODE_write_enable4_mask                                                (0x00000100)
#define DC_MT_MODE_write_enable4(data)                                               (0x00000100&((data)<<8))
#define DC_MT_MODE_write_enable4_src(data)                                           ((0x00000100&(data))>>8)
#define DC_MT_MODE_get_write_enable4(data)                                           ((0x00000100&(data))>>8)
#define DC_MT_MODE_cpu_shift                                                         (6)
#define DC_MT_MODE_cpu_mask                                                          (0x000000C0)
#define DC_MT_MODE_cpu(data)                                                         (0x000000C0&((data)<<6))
#define DC_MT_MODE_cpu_src(data)                                                     ((0x000000C0&(data))>>6)
#define DC_MT_MODE_get_cpu(data)                                                     ((0x000000C0&(data))>>6)
#define DC_MT_MODE_int1_en_shift                                                     (5)
#define DC_MT_MODE_int1_en_mask                                                      (0x00000020)
#define DC_MT_MODE_int1_en(data)                                                     (0x00000020&((data)<<5))
#define DC_MT_MODE_int1_en_src(data)                                                 ((0x00000020&(data))>>5)
#define DC_MT_MODE_get_int1_en(data)                                                 ((0x00000020&(data))>>5)
#define DC_MT_MODE_int2_en_shift                                                     (4)
#define DC_MT_MODE_int2_en_mask                                                      (0x00000010)
#define DC_MT_MODE_int2_en(data)                                                     (0x00000010&((data)<<4))
#define DC_MT_MODE_int2_en_src(data)                                                 ((0x00000010&(data))>>4)
#define DC_MT_MODE_get_int2_en(data)                                                 ((0x00000010&(data))>>4)
#define DC_MT_MODE_int3_en_shift                                                     (3)
#define DC_MT_MODE_int3_en_mask                                                      (0x00000008)
#define DC_MT_MODE_int3_en(data)                                                     (0x00000008&((data)<<3))
#define DC_MT_MODE_int3_en_src(data)                                                 ((0x00000008&(data))>>3)
#define DC_MT_MODE_get_int3_en(data)                                                 ((0x00000008&(data))>>3)
#define DC_MT_MODE_write_enable5_shift                                               (2)
#define DC_MT_MODE_write_enable5_mask                                                (0x00000004)
#define DC_MT_MODE_write_enable5(data)                                               (0x00000004&((data)<<2))
#define DC_MT_MODE_write_enable5_src(data)                                           ((0x00000004&(data))>>2)
#define DC_MT_MODE_get_write_enable5(data)                                           ((0x00000004&(data))>>2)
#define DC_MT_MODE_mode_shift                                                        (0)
#define DC_MT_MODE_mode_mask                                                         (0x00000003)
#define DC_MT_MODE_mode(data)                                                        (0x00000003&((data)<<0))
#define DC_MT_MODE_mode_src(data)                                                    ((0x00000003&(data))>>0)
#define DC_MT_MODE_get_mode(data)                                                    ((0x00000003&(data))>>0)


#define DC_MT_TABLE_0                                                                0x18008260
#define DC_MT_TABLE_1                                                                0x18008264
#define DC_MT_TABLE_2                                                                0x18008268
#define DC_MT_TABLE_3                                                                0x1800826C
#define DC_MT_TABLE_4                                                                0x18008270
#define DC_MT_TABLE_5                                                                0x18008274
#define DC_MT_TABLE_6                                                                0x18008278
#define DC_MT_TABLE_7                                                                0x1800827C
#define DC_MT_TABLE_0_reg_addr                                                       "0xB8008260"
#define DC_MT_TABLE_1_reg_addr                                                       "0xB8008264"
#define DC_MT_TABLE_2_reg_addr                                                       "0xB8008268"
#define DC_MT_TABLE_3_reg_addr                                                       "0xB800826C"
#define DC_MT_TABLE_4_reg_addr                                                       "0xB8008270"
#define DC_MT_TABLE_5_reg_addr                                                       "0xB8008274"
#define DC_MT_TABLE_6_reg_addr                                                       "0xB8008278"
#define DC_MT_TABLE_7_reg_addr                                                       "0xB800827C"
#define DC_MT_TABLE_0_reg                                                            0xB8008260
#define DC_MT_TABLE_1_reg                                                            0xB8008264
#define DC_MT_TABLE_2_reg                                                            0xB8008268
#define DC_MT_TABLE_3_reg                                                            0xB800826C
#define DC_MT_TABLE_4_reg                                                            0xB8008270
#define DC_MT_TABLE_5_reg                                                            0xB8008274
#define DC_MT_TABLE_6_reg                                                            0xB8008278
#define DC_MT_TABLE_7_reg                                                            0xB800827C
#define set_DC_MT_TABLE_0_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_0_reg)=data)
#define set_DC_MT_TABLE_1_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_1_reg)=data)
#define set_DC_MT_TABLE_2_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_2_reg)=data)
#define set_DC_MT_TABLE_3_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_3_reg)=data)
#define set_DC_MT_TABLE_4_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_4_reg)=data)
#define set_DC_MT_TABLE_5_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_5_reg)=data)
#define set_DC_MT_TABLE_6_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_6_reg)=data)
#define set_DC_MT_TABLE_7_reg(data)   (*((volatile unsigned int*) DC_MT_TABLE_7_reg)=data)
#define get_DC_MT_TABLE_0_reg   (*((volatile unsigned int*) DC_MT_TABLE_0_reg))
#define get_DC_MT_TABLE_1_reg   (*((volatile unsigned int*) DC_MT_TABLE_1_reg))
#define get_DC_MT_TABLE_2_reg   (*((volatile unsigned int*) DC_MT_TABLE_2_reg))
#define get_DC_MT_TABLE_3_reg   (*((volatile unsigned int*) DC_MT_TABLE_3_reg))
#define get_DC_MT_TABLE_4_reg   (*((volatile unsigned int*) DC_MT_TABLE_4_reg))
#define get_DC_MT_TABLE_5_reg   (*((volatile unsigned int*) DC_MT_TABLE_5_reg))
#define get_DC_MT_TABLE_6_reg   (*((volatile unsigned int*) DC_MT_TABLE_6_reg))
#define get_DC_MT_TABLE_7_reg   (*((volatile unsigned int*) DC_MT_TABLE_7_reg))
#define DC_MT_TABLE_0_inst_adr                                                       "0x0098"
#define DC_MT_TABLE_1_inst_adr                                                       "0x0099"
#define DC_MT_TABLE_2_inst_adr                                                       "0x009A"
#define DC_MT_TABLE_3_inst_adr                                                       "0x009B"
#define DC_MT_TABLE_4_inst_adr                                                       "0x009C"
#define DC_MT_TABLE_5_inst_adr                                                       "0x009D"
#define DC_MT_TABLE_6_inst_adr                                                       "0x009E"
#define DC_MT_TABLE_7_inst_adr                                                       "0x009F"
#define DC_MT_TABLE_0_inst                                                           0x0098
#define DC_MT_TABLE_1_inst                                                           0x0099
#define DC_MT_TABLE_2_inst                                                           0x009A
#define DC_MT_TABLE_3_inst                                                           0x009B
#define DC_MT_TABLE_4_inst                                                           0x009C
#define DC_MT_TABLE_5_inst                                                           0x009D
#define DC_MT_TABLE_6_inst                                                           0x009E
#define DC_MT_TABLE_7_inst                                                           0x009F
#define DC_MT_TABLE_module_ID_0_shift                                                (26)
#define DC_MT_TABLE_module_ID_0_mask                                                 (0xFC000000)
#define DC_MT_TABLE_module_ID_0(data)                                                (0xFC000000&((data)<<26))
#define DC_MT_TABLE_module_ID_0_src(data)                                            ((0xFC000000&(data))>>26)
#define DC_MT_TABLE_get_module_ID_0(data)                                            ((0xFC000000&(data))>>26)
#define DC_MT_TABLE_access_type_0_shift                                              (24)
#define DC_MT_TABLE_access_type_0_mask                                               (0x03000000)
#define DC_MT_TABLE_access_type_0(data)                                              (0x03000000&((data)<<24))
#define DC_MT_TABLE_access_type_0_src(data)                                          ((0x03000000&(data))>>24)
#define DC_MT_TABLE_get_access_type_0(data)                                          ((0x03000000&(data))>>24)
#define DC_MT_TABLE_module_ID_1_shift                                                (18)
#define DC_MT_TABLE_module_ID_1_mask                                                 (0x00FC0000)
#define DC_MT_TABLE_module_ID_1(data)                                                (0x00FC0000&((data)<<18))
#define DC_MT_TABLE_module_ID_1_src(data)                                            ((0x00FC0000&(data))>>18)
#define DC_MT_TABLE_get_module_ID_1(data)                                            ((0x00FC0000&(data))>>18)
#define DC_MT_TABLE_access_type_1_shift                                              (16)
#define DC_MT_TABLE_access_type_1_mask                                               (0x00030000)
#define DC_MT_TABLE_access_type_1(data)                                              (0x00030000&((data)<<16))
#define DC_MT_TABLE_access_type_1_src(data)                                          ((0x00030000&(data))>>16)
#define DC_MT_TABLE_get_access_type_1(data)                                          ((0x00030000&(data))>>16)
#define DC_MT_TABLE_module_ID_2_shift                                                (10)
#define DC_MT_TABLE_module_ID_2_mask                                                 (0x0000FC00)
#define DC_MT_TABLE_module_ID_2(data)                                                (0x0000FC00&((data)<<10))
#define DC_MT_TABLE_module_ID_2_src(data)                                            ((0x0000FC00&(data))>>10)
#define DC_MT_TABLE_get_module_ID_2(data)                                            ((0x0000FC00&(data))>>10)
#define DC_MT_TABLE_access_type_2_shift                                              (8)
#define DC_MT_TABLE_access_type_2_mask                                               (0x00000300)
#define DC_MT_TABLE_access_type_2(data)                                              (0x00000300&((data)<<8))
#define DC_MT_TABLE_access_type_2_src(data)                                          ((0x00000300&(data))>>8)
#define DC_MT_TABLE_get_access_type_2(data)                                          ((0x00000300&(data))>>8)
#define DC_MT_TABLE_module_ID_3_shift                                                (2)
#define DC_MT_TABLE_module_ID_3_mask                                                 (0x000000FC)
#define DC_MT_TABLE_module_ID_3(data)                                                (0x000000FC&((data)<<2))
#define DC_MT_TABLE_module_ID_3_src(data)                                            ((0x000000FC&(data))>>2)
#define DC_MT_TABLE_get_module_ID_3(data)                                            ((0x000000FC&(data))>>2)
#define DC_MT_TABLE_access_type_3_shift                                              (0)
#define DC_MT_TABLE_access_type_3_mask                                               (0x00000003)
#define DC_MT_TABLE_access_type_3(data)                                              (0x00000003&((data)<<0))
#define DC_MT_TABLE_access_type_3_src(data)                                          ((0x00000003&(data))>>0)
#define DC_MT_TABLE_get_access_type_3(data)                                          ((0x00000003&(data))>>0)


#define DC_MT_ADDCMD_HI_0                                                            0x18008280
#define DC_MT_ADDCMD_HI_1                                                            0x18008284
#define DC_MT_ADDCMD_HI_2                                                            0x18008288
#define DC_MT_ADDCMD_HI_3                                                            0x1800828C
#define DC_MT_ADDCMD_HI_4                                                            0x18008290
#define DC_MT_ADDCMD_HI_5                                                            0x18008294
#define DC_MT_ADDCMD_HI_6                                                            0x18008298
#define DC_MT_ADDCMD_HI_7                                                            0x1800829C
#define DC_MT_ADDCMD_HI_0_reg_addr                                                   "0xB8008280"
#define DC_MT_ADDCMD_HI_1_reg_addr                                                   "0xB8008284"
#define DC_MT_ADDCMD_HI_2_reg_addr                                                   "0xB8008288"
#define DC_MT_ADDCMD_HI_3_reg_addr                                                   "0xB800828C"
#define DC_MT_ADDCMD_HI_4_reg_addr                                                   "0xB8008290"
#define DC_MT_ADDCMD_HI_5_reg_addr                                                   "0xB8008294"
#define DC_MT_ADDCMD_HI_6_reg_addr                                                   "0xB8008298"
#define DC_MT_ADDCMD_HI_7_reg_addr                                                   "0xB800829C"
#define DC_MT_ADDCMD_HI_0_reg                                                        0xB8008280
#define DC_MT_ADDCMD_HI_1_reg                                                        0xB8008284
#define DC_MT_ADDCMD_HI_2_reg                                                        0xB8008288
#define DC_MT_ADDCMD_HI_3_reg                                                        0xB800828C
#define DC_MT_ADDCMD_HI_4_reg                                                        0xB8008290
#define DC_MT_ADDCMD_HI_5_reg                                                        0xB8008294
#define DC_MT_ADDCMD_HI_6_reg                                                        0xB8008298
#define DC_MT_ADDCMD_HI_7_reg                                                        0xB800829C
#define set_DC_MT_ADDCMD_HI_0_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_0_reg)=data)
#define set_DC_MT_ADDCMD_HI_1_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_1_reg)=data)
#define set_DC_MT_ADDCMD_HI_2_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_2_reg)=data)
#define set_DC_MT_ADDCMD_HI_3_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_3_reg)=data)
#define set_DC_MT_ADDCMD_HI_4_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_4_reg)=data)
#define set_DC_MT_ADDCMD_HI_5_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_5_reg)=data)
#define set_DC_MT_ADDCMD_HI_6_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_6_reg)=data)
#define set_DC_MT_ADDCMD_HI_7_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_7_reg)=data)
#define get_DC_MT_ADDCMD_HI_0_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_0_reg))
#define get_DC_MT_ADDCMD_HI_1_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_1_reg))
#define get_DC_MT_ADDCMD_HI_2_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_2_reg))
#define get_DC_MT_ADDCMD_HI_3_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_3_reg))
#define get_DC_MT_ADDCMD_HI_4_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_4_reg))
#define get_DC_MT_ADDCMD_HI_5_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_5_reg))
#define get_DC_MT_ADDCMD_HI_6_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_6_reg))
#define get_DC_MT_ADDCMD_HI_7_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_HI_7_reg))
#define DC_MT_ADDCMD_HI_0_inst_adr                                                   "0x00A0"
#define DC_MT_ADDCMD_HI_1_inst_adr                                                   "0x00A1"
#define DC_MT_ADDCMD_HI_2_inst_adr                                                   "0x00A2"
#define DC_MT_ADDCMD_HI_3_inst_adr                                                   "0x00A3"
#define DC_MT_ADDCMD_HI_4_inst_adr                                                   "0x00A4"
#define DC_MT_ADDCMD_HI_5_inst_adr                                                   "0x00A5"
#define DC_MT_ADDCMD_HI_6_inst_adr                                                   "0x00A6"
#define DC_MT_ADDCMD_HI_7_inst_adr                                                   "0x00A7"
#define DC_MT_ADDCMD_HI_0_inst                                                       0x00A0
#define DC_MT_ADDCMD_HI_1_inst                                                       0x00A1
#define DC_MT_ADDCMD_HI_2_inst                                                       0x00A2
#define DC_MT_ADDCMD_HI_3_inst                                                       0x00A3
#define DC_MT_ADDCMD_HI_4_inst                                                       0x00A4
#define DC_MT_ADDCMD_HI_5_inst                                                       0x00A5
#define DC_MT_ADDCMD_HI_6_inst                                                       0x00A6
#define DC_MT_ADDCMD_HI_7_inst                                                       0x00A7
#define DC_MT_ADDCMD_HI_ever_trap_shift                                              (20)
#define DC_MT_ADDCMD_HI_ever_trap_mask                                               (0x00100000)
#define DC_MT_ADDCMD_HI_ever_trap(data)                                              (0x00100000&((data)<<20))
#define DC_MT_ADDCMD_HI_ever_trap_src(data)                                          ((0x00100000&(data))>>20)
#define DC_MT_ADDCMD_HI_get_ever_trap(data)                                          ((0x00100000&(data))>>20)
#define DC_MT_ADDCMD_HI_module_ID_shift                                              (14)
#define DC_MT_ADDCMD_HI_module_ID_mask                                               (0x000FC000)
#define DC_MT_ADDCMD_HI_module_ID(data)                                              (0x000FC000&((data)<<14))
#define DC_MT_ADDCMD_HI_module_ID_src(data)                                          ((0x000FC000&(data))>>14)
#define DC_MT_ADDCMD_HI_get_module_ID(data)                                          ((0x000FC000&(data))>>14)
#define DC_MT_ADDCMD_HI_addcmd_shift                                                 (0)
#define DC_MT_ADDCMD_HI_addcmd_mask                                                  (0x00003FFF)
#define DC_MT_ADDCMD_HI_addcmd(data)                                                 (0x00003FFF&((data)<<0))
#define DC_MT_ADDCMD_HI_addcmd_src(data)                                             ((0x00003FFF&(data))>>0)
#define DC_MT_ADDCMD_HI_get_addcmd(data)                                             ((0x00003FFF&(data))>>0)


#define DC_MT_ADDCMD_LO_0                                                            0x180082A0
#define DC_MT_ADDCMD_LO_1                                                            0x180082A4
#define DC_MT_ADDCMD_LO_2                                                            0x180082A8
#define DC_MT_ADDCMD_LO_3                                                            0x180082AC
#define DC_MT_ADDCMD_LO_4                                                            0x180082B0
#define DC_MT_ADDCMD_LO_5                                                            0x180082B4
#define DC_MT_ADDCMD_LO_6                                                            0x180082B8
#define DC_MT_ADDCMD_LO_7                                                            0x180082BC
#define DC_MT_ADDCMD_LO_0_reg_addr                                                   "0xB80082A0"
#define DC_MT_ADDCMD_LO_1_reg_addr                                                   "0xB80082A4"
#define DC_MT_ADDCMD_LO_2_reg_addr                                                   "0xB80082A8"
#define DC_MT_ADDCMD_LO_3_reg_addr                                                   "0xB80082AC"
#define DC_MT_ADDCMD_LO_4_reg_addr                                                   "0xB80082B0"
#define DC_MT_ADDCMD_LO_5_reg_addr                                                   "0xB80082B4"
#define DC_MT_ADDCMD_LO_6_reg_addr                                                   "0xB80082B8"
#define DC_MT_ADDCMD_LO_7_reg_addr                                                   "0xB80082BC"
#define DC_MT_ADDCMD_LO_0_reg                                                        0xB80082A0
#define DC_MT_ADDCMD_LO_1_reg                                                        0xB80082A4
#define DC_MT_ADDCMD_LO_2_reg                                                        0xB80082A8
#define DC_MT_ADDCMD_LO_3_reg                                                        0xB80082AC
#define DC_MT_ADDCMD_LO_4_reg                                                        0xB80082B0
#define DC_MT_ADDCMD_LO_5_reg                                                        0xB80082B4
#define DC_MT_ADDCMD_LO_6_reg                                                        0xB80082B8
#define DC_MT_ADDCMD_LO_7_reg                                                        0xB80082BC
#define set_DC_MT_ADDCMD_LO_0_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_0_reg)=data)
#define set_DC_MT_ADDCMD_LO_1_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_1_reg)=data)
#define set_DC_MT_ADDCMD_LO_2_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_2_reg)=data)
#define set_DC_MT_ADDCMD_LO_3_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_3_reg)=data)
#define set_DC_MT_ADDCMD_LO_4_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_4_reg)=data)
#define set_DC_MT_ADDCMD_LO_5_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_5_reg)=data)
#define set_DC_MT_ADDCMD_LO_6_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_6_reg)=data)
#define set_DC_MT_ADDCMD_LO_7_reg(data)   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_7_reg)=data)
#define get_DC_MT_ADDCMD_LO_0_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_0_reg))
#define get_DC_MT_ADDCMD_LO_1_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_1_reg))
#define get_DC_MT_ADDCMD_LO_2_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_2_reg))
#define get_DC_MT_ADDCMD_LO_3_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_3_reg))
#define get_DC_MT_ADDCMD_LO_4_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_4_reg))
#define get_DC_MT_ADDCMD_LO_5_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_5_reg))
#define get_DC_MT_ADDCMD_LO_6_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_6_reg))
#define get_DC_MT_ADDCMD_LO_7_reg   (*((volatile unsigned int*) DC_MT_ADDCMD_LO_7_reg))
#define DC_MT_ADDCMD_LO_0_inst_adr                                                   "0x00A8"
#define DC_MT_ADDCMD_LO_1_inst_adr                                                   "0x00A9"
#define DC_MT_ADDCMD_LO_2_inst_adr                                                   "0x00AA"
#define DC_MT_ADDCMD_LO_3_inst_adr                                                   "0x00AB"
#define DC_MT_ADDCMD_LO_4_inst_adr                                                   "0x00AC"
#define DC_MT_ADDCMD_LO_5_inst_adr                                                   "0x00AD"
#define DC_MT_ADDCMD_LO_6_inst_adr                                                   "0x00AE"
#define DC_MT_ADDCMD_LO_7_inst_adr                                                   "0x00AF"
#define DC_MT_ADDCMD_LO_0_inst                                                       0x00A8
#define DC_MT_ADDCMD_LO_1_inst                                                       0x00A9
#define DC_MT_ADDCMD_LO_2_inst                                                       0x00AA
#define DC_MT_ADDCMD_LO_3_inst                                                       0x00AB
#define DC_MT_ADDCMD_LO_4_inst                                                       0x00AC
#define DC_MT_ADDCMD_LO_5_inst                                                       0x00AD
#define DC_MT_ADDCMD_LO_6_inst                                                       0x00AE
#define DC_MT_ADDCMD_LO_7_inst                                                       0x00AF
#define DC_MT_ADDCMD_LO_addcmd_shift                                                 (0)
#define DC_MT_ADDCMD_LO_addcmd_mask                                                  (0xFFFFFFFF)
#define DC_MT_ADDCMD_LO_addcmd(data)                                                 (0xFFFFFFFF&((data)<<0))
#define DC_MT_ADDCMD_LO_addcmd_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define DC_MT_ADDCMD_LO_get_addcmd(data)                                             ((0xFFFFFFFF&(data))>>0)


#define DC_EC_CTRL                                                                   0x18008300
#define DC_EC_CTRL_reg_addr                                                          "0xB8008300"
#define DC_EC_CTRL_reg                                                               0xB8008300
#define set_DC_EC_CTRL_reg(data)   (*((volatile unsigned int*) DC_EC_CTRL_reg)=data)
#define get_DC_EC_CTRL_reg   (*((volatile unsigned int*) DC_EC_CTRL_reg))
#define DC_EC_CTRL_inst_adr                                                          "0x00C0"
#define DC_EC_CTRL_inst                                                              0x00C0
#define DC_EC_CTRL_write_enable4_shift                                               (8)
#define DC_EC_CTRL_write_enable4_mask                                                (0x00000100)
#define DC_EC_CTRL_write_enable4(data)                                               (0x00000100&((data)<<8))
#define DC_EC_CTRL_write_enable4_src(data)                                           ((0x00000100&(data))>>8)
#define DC_EC_CTRL_get_write_enable4(data)                                           ((0x00000100&(data))>>8)
#define DC_EC_CTRL_error_cmd_int_cpu_shift                                           (6)
#define DC_EC_CTRL_error_cmd_int_cpu_mask                                            (0x000000C0)
#define DC_EC_CTRL_error_cmd_int_cpu(data)                                           (0x000000C0&((data)<<6))
#define DC_EC_CTRL_error_cmd_int_cpu_src(data)                                       ((0x000000C0&(data))>>6)
#define DC_EC_CTRL_get_error_cmd_int_cpu(data)                                       ((0x000000C0&(data))>>6)
#define DC_EC_CTRL_write_enable3_shift                                               (5)
#define DC_EC_CTRL_write_enable3_mask                                                (0x00000020)
#define DC_EC_CTRL_write_enable3(data)                                               (0x00000020&((data)<<5))
#define DC_EC_CTRL_write_enable3_src(data)                                           ((0x00000020&(data))>>5)
#define DC_EC_CTRL_get_write_enable3(data)                                           ((0x00000020&(data))>>5)
#define DC_EC_CTRL_error_cmd_int_en_shift                                            (4)
#define DC_EC_CTRL_error_cmd_int_en_mask                                             (0x00000010)
#define DC_EC_CTRL_error_cmd_int_en(data)                                            (0x00000010&((data)<<4))
#define DC_EC_CTRL_error_cmd_int_en_src(data)                                        ((0x00000010&(data))>>4)
#define DC_EC_CTRL_get_error_cmd_int_en(data)                                        ((0x00000010&(data))>>4)
#define DC_EC_CTRL_write_enable2_shift                                               (3)
#define DC_EC_CTRL_write_enable2_mask                                                (0x00000008)
#define DC_EC_CTRL_write_enable2(data)                                               (0x00000008&((data)<<3))
#define DC_EC_CTRL_write_enable2_src(data)                                           ((0x00000008&(data))>>3)
#define DC_EC_CTRL_get_write_enable2(data)                                           ((0x00000008&(data))>>3)
#define DC_EC_CTRL_error_cmd_swap_en_shift                                           (2)
#define DC_EC_CTRL_error_cmd_swap_en_mask                                            (0x00000004)
#define DC_EC_CTRL_error_cmd_swap_en(data)                                           (0x00000004&((data)<<2))
#define DC_EC_CTRL_error_cmd_swap_en_src(data)                                       ((0x00000004&(data))>>2)
#define DC_EC_CTRL_get_error_cmd_swap_en(data)                                       ((0x00000004&(data))>>2)
#define DC_EC_CTRL_write_enable1_shift                                               (1)
#define DC_EC_CTRL_write_enable1_mask                                                (0x00000002)
#define DC_EC_CTRL_write_enable1(data)                                               (0x00000002&((data)<<1))
#define DC_EC_CTRL_write_enable1_src(data)                                           ((0x00000002&(data))>>1)
#define DC_EC_CTRL_get_write_enable1(data)                                           ((0x00000002&(data))>>1)
#define DC_EC_CTRL_error_cmd_detection_en_shift                                      (0)
#define DC_EC_CTRL_error_cmd_detection_en_mask                                       (0x00000001)
#define DC_EC_CTRL_error_cmd_detection_en(data)                                      (0x00000001&((data)<<0))
#define DC_EC_CTRL_error_cmd_detection_en_src(data)                                  ((0x00000001&(data))>>0)
#define DC_EC_CTRL_get_error_cmd_detection_en(data)                                  ((0x00000001&(data))>>0)


#define DC_EC_ADDCMD_HI                                                              0x18008304
#define DC_EC_ADDCMD_HI_reg_addr                                                     "0xB8008304"
#define DC_EC_ADDCMD_HI_reg                                                          0xB8008304
#define set_DC_EC_ADDCMD_HI_reg(data)   (*((volatile unsigned int*) DC_EC_ADDCMD_HI_reg)=data)
#define get_DC_EC_ADDCMD_HI_reg   (*((volatile unsigned int*) DC_EC_ADDCMD_HI_reg))
#define DC_EC_ADDCMD_HI_inst_adr                                                     "0x00C1"
#define DC_EC_ADDCMD_HI_inst                                                         0x00C1
#define DC_EC_ADDCMD_HI_error_cmd_int_shift                                          (29)
#define DC_EC_ADDCMD_HI_error_cmd_int_mask                                           (0x20000000)
#define DC_EC_ADDCMD_HI_error_cmd_int(data)                                          (0x20000000&((data)<<29))
#define DC_EC_ADDCMD_HI_error_cmd_int_src(data)                                      ((0x20000000&(data))>>29)
#define DC_EC_ADDCMD_HI_get_error_cmd_int(data)                                      ((0x20000000&(data))>>29)
#define DC_EC_ADDCMD_HI_error_cmd_ever_trap_shift                                    (28)
#define DC_EC_ADDCMD_HI_error_cmd_ever_trap_mask                                     (0x10000000)
#define DC_EC_ADDCMD_HI_error_cmd_ever_trap(data)                                    (0x10000000&((data)<<28))
#define DC_EC_ADDCMD_HI_error_cmd_ever_trap_src(data)                                ((0x10000000&(data))>>28)
#define DC_EC_ADDCMD_HI_get_error_cmd_ever_trap(data)                                ((0x10000000&(data))>>28)
#define DC_EC_ADDCMD_HI_trap_case_shift                                              (20)
#define DC_EC_ADDCMD_HI_trap_case_mask                                               (0x0FF00000)
#define DC_EC_ADDCMD_HI_trap_case(data)                                              (0x0FF00000&((data)<<20))
#define DC_EC_ADDCMD_HI_trap_case_src(data)                                          ((0x0FF00000&(data))>>20)
#define DC_EC_ADDCMD_HI_get_trap_case(data)                                          ((0x0FF00000&(data))>>20)
#define DC_EC_ADDCMD_HI_Addcmd_hi_shift                                              (0)
#define DC_EC_ADDCMD_HI_Addcmd_hi_mask                                               (0x000FFFFF)
#define DC_EC_ADDCMD_HI_Addcmd_hi(data)                                              (0x000FFFFF&((data)<<0))
#define DC_EC_ADDCMD_HI_Addcmd_hi_src(data)                                          ((0x000FFFFF&(data))>>0)
#define DC_EC_ADDCMD_HI_get_Addcmd_hi(data)                                          ((0x000FFFFF&(data))>>0)


#define DC_EC_ADDCMD_LO                                                              0x18008308
#define DC_EC_ADDCMD_LO_reg_addr                                                     "0xB8008308"
#define DC_EC_ADDCMD_LO_reg                                                          0xB8008308
#define set_DC_EC_ADDCMD_LO_reg(data)   (*((volatile unsigned int*) DC_EC_ADDCMD_LO_reg)=data)
#define get_DC_EC_ADDCMD_LO_reg   (*((volatile unsigned int*) DC_EC_ADDCMD_LO_reg))
#define DC_EC_ADDCMD_LO_inst_adr                                                     "0x00C2"
#define DC_EC_ADDCMD_LO_inst                                                         0x00C2
#define DC_EC_ADDCMD_LO_Addcmd_lo_shift                                              (0)
#define DC_EC_ADDCMD_LO_Addcmd_lo_mask                                               (0xFFFFFFFF)
#define DC_EC_ADDCMD_LO_Addcmd_lo(data)                                              (0xFFFFFFFF&((data)<<0))
#define DC_EC_ADDCMD_LO_Addcmd_lo_src(data)                                          ((0xFFFFFFFF&(data))>>0)
#define DC_EC_ADDCMD_LO_get_Addcmd_lo(data)                                          ((0xFFFFFFFF&(data))>>0)


#define DC_RD_TAG_CHECK                                                              0x1800830C
#define DC_RD_TAG_CHECK_reg_addr                                                     "0xB800830C"
#define DC_RD_TAG_CHECK_reg                                                          0xB800830C
#define set_DC_RD_TAG_CHECK_reg(data)   (*((volatile unsigned int*) DC_RD_TAG_CHECK_reg)=data)
#define get_DC_RD_TAG_CHECK_reg   (*((volatile unsigned int*) DC_RD_TAG_CHECK_reg))
#define DC_RD_TAG_CHECK_inst_adr                                                     "0x00C3"
#define DC_RD_TAG_CHECK_inst                                                         0x00C3
#define DC_RD_TAG_CHECK_write_enable3_shift                                          (6)
#define DC_RD_TAG_CHECK_write_enable3_mask                                           (0x00000040)
#define DC_RD_TAG_CHECK_write_enable3(data)                                          (0x00000040&((data)<<6))
#define DC_RD_TAG_CHECK_write_enable3_src(data)                                      ((0x00000040&(data))>>6)
#define DC_RD_TAG_CHECK_get_write_enable3(data)                                      ((0x00000040&(data))>>6)
#define DC_RD_TAG_CHECK_tag_check_int_cpu_shift                                      (4)
#define DC_RD_TAG_CHECK_tag_check_int_cpu_mask                                       (0x00000030)
#define DC_RD_TAG_CHECK_tag_check_int_cpu(data)                                      (0x00000030&((data)<<4))
#define DC_RD_TAG_CHECK_tag_check_int_cpu_src(data)                                  ((0x00000030&(data))>>4)
#define DC_RD_TAG_CHECK_get_tag_check_int_cpu(data)                                  ((0x00000030&(data))>>4)
#define DC_RD_TAG_CHECK_write_enable2_shift                                          (3)
#define DC_RD_TAG_CHECK_write_enable2_mask                                           (0x00000008)
#define DC_RD_TAG_CHECK_write_enable2(data)                                          (0x00000008&((data)<<3))
#define DC_RD_TAG_CHECK_write_enable2_src(data)                                      ((0x00000008&(data))>>3)
#define DC_RD_TAG_CHECK_get_write_enable2(data)                                      ((0x00000008&(data))>>3)
#define DC_RD_TAG_CHECK_tag_check_int_en_shift                                       (2)
#define DC_RD_TAG_CHECK_tag_check_int_en_mask                                        (0x00000004)
#define DC_RD_TAG_CHECK_tag_check_int_en(data)                                       (0x00000004&((data)<<2))
#define DC_RD_TAG_CHECK_tag_check_int_en_src(data)                                   ((0x00000004&(data))>>2)
#define DC_RD_TAG_CHECK_get_tag_check_int_en(data)                                   ((0x00000004&(data))>>2)
#define DC_RD_TAG_CHECK_write_enable1_shift                                          (1)
#define DC_RD_TAG_CHECK_write_enable1_mask                                           (0x00000002)
#define DC_RD_TAG_CHECK_write_enable1(data)                                          (0x00000002&((data)<<1))
#define DC_RD_TAG_CHECK_write_enable1_src(data)                                      ((0x00000002&(data))>>1)
#define DC_RD_TAG_CHECK_get_write_enable1(data)                                      ((0x00000002&(data))>>1)
#define DC_RD_TAG_CHECK_tag_check_en_shift                                           (0)
#define DC_RD_TAG_CHECK_tag_check_en_mask                                            (0x00000001)
#define DC_RD_TAG_CHECK_tag_check_en(data)                                           (0x00000001&((data)<<0))
#define DC_RD_TAG_CHECK_tag_check_en_src(data)                                       ((0x00000001&(data))>>0)
#define DC_RD_TAG_CHECK_get_tag_check_en(data)                                       ((0x00000001&(data))>>0)


#define DC_RD_TAG_CHECK_ST                                                           0x18008310
#define DC_RD_TAG_CHECK_ST_reg_addr                                                  "0xB8008310"
#define DC_RD_TAG_CHECK_ST_reg                                                       0xB8008310
#define set_DC_RD_TAG_CHECK_ST_reg(data)   (*((volatile unsigned int*) DC_RD_TAG_CHECK_ST_reg)=data)
#define get_DC_RD_TAG_CHECK_ST_reg   (*((volatile unsigned int*) DC_RD_TAG_CHECK_ST_reg))
#define DC_RD_TAG_CHECK_ST_inst_adr                                                  "0x00C4"
#define DC_RD_TAG_CHECK_ST_inst                                                      0x00C4
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_ever_shift                                (1)
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_ever_mask                                 (0x00000002)
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_ever(data)                                (0x00000002&((data)<<1))
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_ever_src(data)                            ((0x00000002&(data))>>1)
#define DC_RD_TAG_CHECK_ST_get_rd_tag_mismatch_ever(data)                            ((0x00000002&(data))>>1)
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_int_shift                                 (0)
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_int_mask                                  (0x00000001)
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_int(data)                                 (0x00000001&((data)<<0))
#define DC_RD_TAG_CHECK_ST_rd_tag_mismatch_int_src(data)                             ((0x00000001&(data))>>0)
#define DC_RD_TAG_CHECK_ST_get_rd_tag_mismatch_int(data)                             ((0x00000001&(data))>>0)


#define DC_MEM_PROTECT_CTRL                                                          0x18008400
#define DC_MEM_PROTECT_CTRL_reg_addr                                                 "0xB8008400"
#define DC_MEM_PROTECT_CTRL_reg                                                      0xB8008400
#define set_DC_MEM_PROTECT_CTRL_reg(data)   (*((volatile unsigned int*) DC_MEM_PROTECT_CTRL_reg)=data)
#define get_DC_MEM_PROTECT_CTRL_reg   (*((volatile unsigned int*) DC_MEM_PROTECT_CTRL_reg))
#define DC_MEM_PROTECT_CTRL_inst_adr                                                 "0x0000"
#define DC_MEM_PROTECT_CTRL_inst                                                     0x0000
#define DC_MEM_PROTECT_CTRL_mem_protect_en_shift                                     (0)
#define DC_MEM_PROTECT_CTRL_mem_protect_en_mask                                      (0x00000001)
#define DC_MEM_PROTECT_CTRL_mem_protect_en(data)                                     (0x00000001&((data)<<0))
#define DC_MEM_PROTECT_CTRL_mem_protect_en_src(data)                                 ((0x00000001&(data))>>0)
#define DC_MEM_PROTECT_CTRL_get_mem_protect_en(data)                                 ((0x00000001&(data))>>0)


#define DC_MEM_PROTECT_SADDR                                                         0x18008404
#define DC_MEM_PROTECT_SADDR_reg_addr                                                "0xB8008404"
#define DC_MEM_PROTECT_SADDR_reg                                                     0xB8008404
#define set_DC_MEM_PROTECT_SADDR_reg(data)   (*((volatile unsigned int*) DC_MEM_PROTECT_SADDR_reg)=data)
#define get_DC_MEM_PROTECT_SADDR_reg   (*((volatile unsigned int*) DC_MEM_PROTECT_SADDR_reg))
#define DC_MEM_PROTECT_SADDR_inst_adr                                                "0x0001"
#define DC_MEM_PROTECT_SADDR_inst                                                    0x0001
#define DC_MEM_PROTECT_SADDR_saddr_shift                                             (0)
#define DC_MEM_PROTECT_SADDR_saddr_mask                                              (0x1FFFFFFF)
#define DC_MEM_PROTECT_SADDR_saddr(data)                                             (0x1FFFFFFF&((data)<<0))
#define DC_MEM_PROTECT_SADDR_saddr_src(data)                                         ((0x1FFFFFFF&(data))>>0)
#define DC_MEM_PROTECT_SADDR_get_saddr(data)                                         ((0x1FFFFFFF&(data))>>0)


#define DC_MEM_PROTECT_EADDR                                                         0x18008408
#define DC_MEM_PROTECT_EADDR_reg_addr                                                "0xB8008408"
#define DC_MEM_PROTECT_EADDR_reg                                                     0xB8008408
#define set_DC_MEM_PROTECT_EADDR_reg(data)   (*((volatile unsigned int*) DC_MEM_PROTECT_EADDR_reg)=data)
#define get_DC_MEM_PROTECT_EADDR_reg   (*((volatile unsigned int*) DC_MEM_PROTECT_EADDR_reg))
#define DC_MEM_PROTECT_EADDR_inst_adr                                                "0x0002"
#define DC_MEM_PROTECT_EADDR_inst                                                    0x0002
#define DC_MEM_PROTECT_EADDR_eaddr_shift                                             (0)
#define DC_MEM_PROTECT_EADDR_eaddr_mask                                              (0x1FFFFFFF)
#define DC_MEM_PROTECT_EADDR_eaddr(data)                                             (0x1FFFFFFF&((data)<<0))
#define DC_MEM_PROTECT_EADDR_eaddr_src(data)                                         ((0x1FFFFFFF&(data))>>0)
#define DC_MEM_PROTECT_EADDR_get_eaddr(data)                                         ((0x1FFFFFFF&(data))>>0)


#endif
