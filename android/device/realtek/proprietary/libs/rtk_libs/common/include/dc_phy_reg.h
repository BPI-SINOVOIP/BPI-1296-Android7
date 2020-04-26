/**************************************************************
// Spec Version                  : 0.0.1
// Parser Version                : DVR_Parser_6.1
// CModelGen Version             : 5.0 2009.04.09
// Naming Rule                   : Module_Register_Name_reg
// Parse Option                  : Only Parse _op1
// Parse Address Region          : All Address Region 
// Firmware Header Generate Date : 2010/6/18 0:27:50
***************************************************************/


#ifndef _DC_PHY_REG_H_INCLUDED_
#define _DC_PHY_REG_H_INCLUDED_
#ifdef  _DC_PHY_USE_STRUCT
typedef struct 
{
unsigned int     dram_2g_used:1;
unsigned int     sclk_sel:1;
unsigned int     tmras:5;
unsigned int     tmrc:6;
unsigned int     tmrcd:4;
unsigned int     tmrp:4;
unsigned int     tmrrd:3;
unsigned int     tmwr:4;
unsigned int     tmwtr:3;
unsigned int     ddr16:1;
}DC_PHY_TMCTRL;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     cal_rang:6;
unsigned int     cal_per:8;
}DC_PHY_TMCTRL2;

typedef struct 
{
unsigned int     exmod_reg:16;
unsigned int     mod_reg:16;
}DC_PHY_MOD_REG;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     ddr3_odt_dly_1T:1;
unsigned int     ddr3_odt_ext_1T:1;
unsigned int     ctrl_odt_en:1;
unsigned int     mars_odt_force_sel:1;
unsigned int     mars_odt_force_sig:1;
unsigned int     odt_force_sel:1;
unsigned int     odt_force_sig:1;
unsigned int     odt_en:1;
unsigned int     odt_dis:1;
unsigned int     exe_cal:1;
unsigned int     init:1;
unsigned int     sren:1;
unsigned int     srex:1;
unsigned int     exe_emr:1;
unsigned int     exe_mr:1;
}DC_PHY_TMCTRL2A;

typedef struct 
{
unsigned int     ddr3_512_used:1;
unsigned int     tmcl_rd_4:1;
unsigned int     reserved_0:2;
unsigned int     odt_en_lat_2:7;
unsigned int     rcd_not_eq:1;
unsigned int     tmrcdwr:4;
unsigned int     tmrp_all_add:2;
unsigned int     ap_wo_min_dly:1;
unsigned int     add_ht:1;
unsigned int     tmcl_rd:4;
unsigned int     odt_en_lat:8;
}DC_PHY_DC_MIS;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     pre_dis:1;
unsigned int     mars_BGA:1;
unsigned int     reserved_1:8;
unsigned int     lbk_en:1;
unsigned int     al:1;
unsigned int     ddr2:1;
}DC_PHY_DC_MISA;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     tmrfc_2:2;
unsigned int     tmrc_sel:1;
unsigned int     pasr:3;
unsigned int     srf:1;
unsigned int     trtp:3;
unsigned int     tmrfc:7;
unsigned int     refcycle:6;
unsigned int     tmmrd:3;
unsigned int     tmcl:5;
}DC_PHY_DC_MISB;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     tap_mask:1;
unsigned int     fw_set:1;
unsigned int     tap_sel:1;
unsigned int     reserved_1:24;
}DC_PHY_DC_MISC;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     clear_set_sys:1;
unsigned int     clear_trig:1;
unsigned int     test_mode_sel:1;
}DC_PHY_DC_TEST_MODE_SEL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tAOFD:4;
unsigned int     tAOND:4;
}DC_PHY_TMCTRL2B;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     ocd_dt:4;
}DC_PHY_DC_OCD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     write_en_14:1;
unsigned int     ddr3_swap:1;
unsigned int     write_en_13:1;
unsigned int     end_500us:1;
unsigned int     write_en_12:1;
unsigned int     end_200us:1;
unsigned int     write_en_11:1;
unsigned int     fast_prechg:1;
unsigned int     write_en_10:1;
unsigned int     dis_cti:1;
unsigned int     write_en_9:1;
unsigned int     dis_8bank:1;
unsigned int     write_en_8:1;
unsigned int     use_tmmrd:1;
unsigned int     write_en_7:1;
unsigned int     ddr3:1;
unsigned int     write_en_6:1;
unsigned int     ddr3_zqcl_short:1;
unsigned int     write_en_5:1;
unsigned int     ddr3_exe_zqcl:1;
unsigned int     write_en_4:1;
unsigned int     ddr3_exe_mr3:1;
unsigned int     write_en_3:1;
unsigned int     ddr3_exe_mr2:1;
unsigned int     write_en_2:1;
unsigned int     ddr3_exe_mr1:1;
unsigned int     write_en_1:1;
unsigned int     ddr3_exe_mr0:1;
unsigned int     write_en_0:1;
unsigned int     ddr3_init:1;
}DC_PHY_DC_DDR3_CTL;

typedef struct 
{
unsigned int     mod3_reg:16;
unsigned int     mod2_reg:16;
}DC_PHY_MOD23_REG;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     tmccd:3;
unsigned int     tmFAW:6;
unsigned int     tmmod:4;
}DC_PHY_TMCTRL3;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     wfifo_rdy_gated:1;
unsigned int     qfifo_dly_2T:3;
unsigned int     rdwr_use_full:1;
unsigned int     full_gated:1;
unsigned int     rfifo_thred:7;
}DC_PHY_RFIFO_CTL;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     err_cti_cmd:1;
unsigned int     err_active:1;
}DC_PHY_ERR_STATUS;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     exe_prea:1;
unsigned int     dis_refresh:1;
unsigned int     trig_wr_level:1;
unsigned int     wr_level_rdy:1;
}DC_PHY_DC_WRITE_LEVEL;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     int_err_cticmd_en:1;
unsigned int     int_err_active_en:1;
unsigned int     int_vcpu_en:1;
unsigned int     int_acpu_en:1;
unsigned int     int_scpu_en:1;
}DC_PHY_INT_ENABLE;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     non_valid_maxcnt:1;
unsigned int     rfifo_cnt_max:7;
}DC_PHY_RFIFO_MAXCNT;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     bank_act:8;
unsigned int     cmd_cnt:3;
unsigned int     par_st:10;
}DC_PHY_SM_STATUS;

typedef struct 
{
unsigned int     wfifo_sys_req_statistic_en:1;
unsigned int     wfifo_ddr_req_statistic_en:1;
unsigned int     clear_wfifo_record:1;
unsigned int     reserved_0:5;
unsigned int     wfifo_rd_cnt_et:8;
unsigned int     wfifo_rd_cnt_rl:8;
unsigned int     wfifo_wr_cnt_rl:8;
}DC_PHY_WFIFO_CNT;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     dbg_out_mod:3;
unsigned int     pad_tie_value:4;
unsigned int     shift_trig_mode:7;
unsigned int     rd_dbg_mod:3;
unsigned int     dbg_mod:6;
}DC_PHY_DC_DEBUG;

typedef struct 
{
unsigned int     post_trig_cnt:16;
unsigned int     pre_trig_cnt:16;
}DC_PHY_PRE_CNT;

typedef struct 
{
unsigned int     wfifo_sys_req_wait_total:32;
}DC_PHY_WFIFO_SYS_REQ_WAIT;

typedef struct 
{
unsigned int     wfifo_ddr_req_wait_total:32;
}DC_PHY_WFIFO_DDR_REQ_WAIT;

typedef struct 
{
unsigned int     wfifo_ddr_req_wait_max:16;
unsigned int     wfifo_sys_req_wait_max:16;
}DC_PHY_WFIFO_MAX_REQ_WAIT;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_1_0:6;
unsigned int     ph_sel_dq_pos_1_0:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_0_0:6;
unsigned int     ph_sel_dq_pos_0_0:6;
}DC_PHY_DQ0_1_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_3_0:6;
unsigned int     ph_sel_dq_pos_3_0:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_2_0:6;
unsigned int     ph_sel_dq_pos_2_0:6;
}DC_PHY_DQ2_3_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_5_0:6;
unsigned int     ph_sel_dq_pos_5_0:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_4_0:6;
unsigned int     ph_sel_dq_pos_4_0:6;
}DC_PHY_DQ4_5_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_7_0:6;
unsigned int     ph_sel_dq_pos_7_0:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_6_0:6;
unsigned int     ph_sel_dq_pos_6_0:6;
}DC_PHY_DQ6_7_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_1_1:6;
unsigned int     ph_sel_dq_pos_1_1:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_0_1:6;
unsigned int     ph_sel_dq_pos_0_1:6;
}DC_PHY_DQ8_9_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_3_1:6;
unsigned int     ph_sel_dq_pos_3_1:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_2_1:6;
unsigned int     ph_sel_dq_pos_2_1:6;
}DC_PHY_DQ10_11_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_5_1:6;
unsigned int     ph_sel_dq_pos_5_1:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_4_1:6;
unsigned int     ph_sel_dq_pos_4_1:6;
}DC_PHY_DQ12_13_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_7_1:6;
unsigned int     ph_sel_dq_pos_7_1:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_6_1:6;
unsigned int     ph_sel_dq_pos_6_1:6;
}DC_PHY_DQ14_15_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_1_2:6;
unsigned int     ph_sel_dq_pos_1_2:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_0_2:6;
unsigned int     ph_sel_dq_pos_0_2:6;
}DC_PHY_DQ16_17_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_3_2:6;
unsigned int     ph_sel_dq_pos_3_2:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_2_2:6;
unsigned int     ph_sel_dq_pos_2_2:6;
}DC_PHY_DQ18_19_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_5_2:6;
unsigned int     ph_sel_dq_pos_5_2:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_4_2:6;
unsigned int     ph_sel_dq_pos_4_2:6;
}DC_PHY_DQ20_21_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_7_2:6;
unsigned int     ph_sel_dq_pos_7_2:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_6_2:6;
unsigned int     ph_sel_dq_pos_6_2:6;
}DC_PHY_DQ22_23_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_1_3:6;
unsigned int     ph_sel_dq_pos_1_3:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_0_3:6;
unsigned int     ph_sel_dq_pos_0_3:6;
}DC_PHY_DQ24_25_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_3_3:6;
unsigned int     ph_sel_dq_pos_3_3:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_2_3:6;
unsigned int     ph_sel_dq_pos_2_3:6;
}DC_PHY_DQ26_27_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_5_3:6;
unsigned int     ph_sel_dq_pos_5_3:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_4_3:6;
unsigned int     ph_sel_dq_pos_4_3:6;
}DC_PHY_DQ28_29_DLYN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ph_sel_dq_neg_7_3:6;
unsigned int     ph_sel_dq_pos_7_3:6;
unsigned int     reserved_1:4;
unsigned int     ph_sel_dq_neg_6_3:6;
unsigned int     ph_sel_dq_pos_6_3:6;
}DC_PHY_DQ30_31_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq1_neg_dlyn_0:6;
unsigned int     cal_dq1_pos_dlyn_0:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq0_neg_dlyn_0:6;
unsigned int     cal_dq0_pos_dlyn_0:6;
}DC_PHY_CAL0_1_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq3_neg_dlyn_0:6;
unsigned int     cal_dq3_pos_dlyn_0:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq2_neg_dlyn_0:6;
unsigned int     cal_dq2_pos_dlyn_0:6;
}DC_PHY_CAL2_3_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq5_neg_dlyn_0:6;
unsigned int     cal_dq5_pos_dlyn_0:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq4_neg_dlyn_0:6;
unsigned int     cal_dq4_pos_dlyn_0:6;
}DC_PHY_CAL4_5_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq7_neg_dlyn_0:6;
unsigned int     cal_dq7_pos_dlyn_0:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq6_neg_dlyn_0:6;
unsigned int     cal_dq6_pos_dlyn_0:6;
}DC_PHY_CAL6_7_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq1_neg_dlyn_1:6;
unsigned int     cal_dq1_pos_dlyn_1:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq0_neg_dlyn_1:6;
unsigned int     cal_dq0_pos_dlyn_1:6;
}DC_PHY_CAL8_9_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq3_neg_dlyn_1:6;
unsigned int     cal_dq3_pos_dlyn_1:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq2_neg_dlyn_1:6;
unsigned int     cal_dq2_pos_dlyn_1:6;
}DC_PHY_CAL10_11_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq5_neg_dlyn_1:6;
unsigned int     cal_dq5_pos_dlyn_1:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq4_neg_dlyn_1:6;
unsigned int     cal_dq4_pos_dlyn_1:6;
}DC_PHY_CAL12_13_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq7_neg_dlyn_1:6;
unsigned int     cal_dq7_pos_dlyn_1:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq6_neg_dlyn_1:6;
unsigned int     cal_dq6_pos_dlyn_1:6;
}DC_PHY_CAL14_15_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq1_neg_dlyn_2:6;
unsigned int     cal_dq1_pos_dlyn_2:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq0_neg_dlyn_2:6;
unsigned int     cal_dq0_pos_dlyn_2:6;
}DC_PHY_CAL16_17_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq3_neg_dlyn_2:6;
unsigned int     cal_dq3_pos_dlyn_2:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq2_neg_dlyn_2:6;
unsigned int     cal_dq2_pos_dlyn_2:6;
}DC_PHY_CAL18_19_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq5_neg_dlyn_2:6;
unsigned int     cal_dq5_pos_dlyn_2:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq4_neg_dlyn_2:6;
unsigned int     cal_dq4_pos_dlyn_2:6;
}DC_PHY_CAL20_21_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq7_neg_dlyn_2:6;
unsigned int     cal_dq7_pos_dlyn_2:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq6_neg_dlyn_2:6;
unsigned int     cal_dq6_pos_dlyn_2:6;
}DC_PHY_CAL22_23_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq1_neg_dlyn_3:6;
unsigned int     cal_dq1_pos_dlyn_3:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq0_neg_dlyn_3:6;
unsigned int     cal_dq0_pos_dlyn_3:6;
}DC_PHY_CAL24_25_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq3_neg_dlyn_3:6;
unsigned int     cal_dq3_pos_dlyn_3:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq2_neg_dlyn_3:6;
unsigned int     cal_dq2_pos_dlyn_3:6;
}DC_PHY_CAL26_27_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq5_neg_dlyn_3:6;
unsigned int     cal_dq5_pos_dlyn_3:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq4_neg_dlyn_3:6;
unsigned int     cal_dq4_pos_dlyn_3:6;
}DC_PHY_CAL28_29_DLYN;

typedef struct 
{
unsigned int     non_valid:1;
unsigned int     reserved_0:3;
unsigned int     cal_dq7_neg_dlyn_3:6;
unsigned int     cal_dq7_pos_dlyn_3:6;
unsigned int     reserved_1:4;
unsigned int     cal_dq6_neg_dlyn_3:6;
unsigned int     cal_dq6_pos_dlyn_3:6;
}DC_PHY_CAL30_31_DLYN;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     read_lat:2;
unsigned int     ph_sel_read23:4;
unsigned int     ph_sel_read01:4;
unsigned int     reserved_1:3;
unsigned int     read_com_sel:1;
}DC_PHY_READ_COM_CTRL;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     pos_trig_sel:1;
unsigned int     pre_trig_sel:1;
unsigned int     auto_cal:1;
}DC_PHY_CAL_CTRL;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     dqsf_dlyn:2;
unsigned int     Tx_dlc_sel_dck:4;
}DC_PHY_FIFO_DLYN;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     grp2_dqm_oe:1;
unsigned int     grp0_dqm_oe:1;
unsigned int     grp1_bk_oe:1;
unsigned int     grp1_addr1_oe:1;
unsigned int     grp1_addr_oe:1;
unsigned int     grp1_cke_oe:1;
unsigned int     grp1_ck_oe:1;
unsigned int     grp1_ctl_oe:7;
}DC_PHY_PAD_OE_CTL;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     grp2_dqm_odt:1;
unsigned int     grp0_dqm_odt:1;
unsigned int     grp1_bk_odt:1;
unsigned int     grp1_addr1_odt:1;
unsigned int     grp1_addr_odt:1;
unsigned int     grp1_cke_odt:1;
unsigned int     grp1_ck_odt:1;
unsigned int     grp1_ctl_odt:7;
}DC_PHY_PAD_ODT_CTL;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     iddq_0:22;
}DC_PHY_PAD_IDDQ0;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     iddq_1:30;
}DC_PHY_PAD_IDDQ1;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     iddq_2:22;
}DC_PHY_PAD_IDDQ2;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     zctrl_ovrd_en:1;
unsigned int     zctrl_ovrd_data:20;
}DC_PHY_PAD_ZCTRL_OVRD;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     dzq_auto_up:1;
unsigned int     zctrl_clk_sel:1;
unsigned int     zctrl_start:1;
unsigned int     zprog:14;
}DC_PHY_PAD_CTRL_PROG;

typedef struct 
{
unsigned int     zctrl_status:32;
}DC_PHY_PAD_ZCTRL_STATUS;

typedef struct 
{
unsigned int     dq_dly_sel0:32;
}DC_PHY_DQ_DLY_SEL0;

typedef struct 
{
unsigned int     dq_dly_sel1:32;
}DC_PHY_DQ_DLY_SEL1;

typedef struct 
{
unsigned int     dq_dly_sel2:32;
}DC_PHY_DQ_DLY_SEL2;

typedef struct 
{
unsigned int     dq_dly_sel3:32;
}DC_PHY_DQ_DLY_SEL3;

typedef struct 
{
unsigned int     dummy_reg:32;
}DC_PHY_DUMMY_REG;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     half_cycle_cal_sel:1;
unsigned int     half_cycle_ref_thr:8;
unsigned int     half_cycle_res_thr:6;
unsigned int     half_cycle_res_mod:2;
unsigned int     half_cycle_cal_en:1;
}DC_PHY_HALF_CYCLE_CAL_EN;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     half_cycle_cal_setting_3:6;
unsigned int     half_cycle_cal_setting_2:6;
unsigned int     half_cycle_cal_setting_1:6;
unsigned int     half_cycle_cal_setting_0:6;
}DC_PHY_HALF_CYCLE_CAL_SET;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     pupd_dly_num_sel:2;
unsigned int     pupd_dly_num_3:5;
unsigned int     pupd_dly_num_2:5;
unsigned int     pupd_dly_num_1:5;
unsigned int     pupd_dly_num_0:5;
}DC_PHY_PUPD_DLY_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     dqs3_dlyn:6;
unsigned int     dqs2_dlyn:6;
unsigned int     dqs1_dlyn:6;
unsigned int     dqs0_dlyn:6;
}DC_PHY_DQS_DLYN;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cal_all_rang_noml:1;
unsigned int     cal_all_rang_init:1;
unsigned int     auto_cal_en_noml:1;
unsigned int     auto_cal_en_init:1;
unsigned int     dqs_val_sel:1;
unsigned int     cal_mode:3;
unsigned int     dq_sel:1;
}DC_PHY_CAL_MODE;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     cal_ba:3;
unsigned int     cal_col:10;
unsigned int     cal_row:13;
}DC_PHY_CAL_ADDR;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     end_done_01_p:1;
unsigned int     start_done_01_p:1;
unsigned int     best_dly_01_p:6;
unsigned int     end_pt_01_p:6;
unsigned int     start_pt_01_p:6;
}DC_PHY_RESULT_CAL_TAP_0;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     end_done_01_n:1;
unsigned int     start_done_01_n:1;
unsigned int     best_dly_01_n:6;
unsigned int     end_pt_01_n:6;
unsigned int     start_pt_01_n:6;
}DC_PHY_RESULT_CAL_TAP_1;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     end_done_23_p:1;
unsigned int     start_done_23_p:1;
unsigned int     best_dly_23_p:6;
unsigned int     end_pt_23_p:6;
unsigned int     start_pt_23_p:6;
}DC_PHY_RESULT_CAL_TAP_2;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     end_done_23_n:1;
unsigned int     start_done_23_n:1;
unsigned int     best_dly_23_n:6;
unsigned int     end_pt_23_n:6;
unsigned int     start_pt_23_n:6;
}DC_PHY_RESULT_CAL_TAP_3;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     wk_big_thr_23_n_odt:1;
unsigned int     wk_big_thr_23_p_odt:1;
unsigned int     wk_big_thr_01_n_odt:1;
unsigned int     wk_big_thr_01_p_odt:1;
unsigned int     wk_big_thr_23_n_dt:1;
unsigned int     wk_big_thr_23_p_dt:1;
unsigned int     wk_big_thr_01_n_dt:1;
unsigned int     wk_big_thr_01_p_dt:1;
}DC_PHY_RESULT_CAL_TAP_4;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     best_wk_01_n_odt:6;
unsigned int     reserved_1:2;
unsigned int     best_wk_01_p_odt:6;
unsigned int     reserved_2:2;
unsigned int     best_wk_01_n_dt:6;
unsigned int     reserved_3:2;
unsigned int     best_wk_01_p_dt:6;
}DC_PHY_RESULT_CAL_TAP_5;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     best_wk_23_n_odt:6;
unsigned int     reserved_1:2;
unsigned int     best_wk_23_p_odt:6;
unsigned int     reserved_2:2;
unsigned int     best_wk_23_n_dt:6;
unsigned int     reserved_3:2;
unsigned int     best_wk_23_p_dt:6;
}DC_PHY_RESULT_CAL_TAP_6;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     odt_diff_threshold:6;
unsigned int     reserved_1:2;
unsigned int     duty_diff_threshold:6;
unsigned int     cal_proc_done:1;
unsigned int     wk_threshold:6;
unsigned int     sync_rst_wk_thr:1;
}DC_PHY_SYNC_RST_WK;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     sync_rst_fifo_pt:1;
}DC_PHY_SYNC_RST_FIFO_PT;

typedef struct 
{
unsigned int     cal_out_data_1:16;
unsigned int     cal_out_data_0:16;
}DC_PHY_CAL_OUT_DATA_A;

typedef struct 
{
unsigned int     cal_out_data_3:16;
unsigned int     cal_out_data_2:16;
}DC_PHY_CAL_OUT_DATA_B;

typedef struct 
{
unsigned int     cal_gld_data_1:16;
unsigned int     cal_gld_data_0:16;
}DC_PHY_CAL_GLD_DATA_A;

typedef struct 
{
unsigned int     cal_gld_data_3:16;
unsigned int     cal_gld_data_2:16;
}DC_PHY_CAL_GLD_DATA_B;

typedef struct 
{
unsigned int     dt_set_0:32;
}DC_PHY_DT_SET0;

typedef struct 
{
unsigned int     dt_set_1:32;
}DC_PHY_DT_SET1;

typedef struct 
{
unsigned int     dt_set_2:32;
}DC_PHY_DT_SET2;

typedef struct 
{
unsigned int     dt_set_3:32;
}DC_PHY_DT_SET3;

typedef struct 
{
unsigned int     dt_set_4:32;
}DC_PHY_DT_SET4;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     update_dqs:1;
unsigned int     update_dq:1;
}DC_PHY_DT_CTL;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     odt_23_n:3;
unsigned int     odt_23_p:3;
unsigned int     odt_01_n:3;
unsigned int     odt_01_p:3;
unsigned int     indt_23_n:2;
unsigned int     indt_23_p:2;
unsigned int     indt_01_n:2;
unsigned int     indt_01_p:2;
}DC_PHY_CAL_DT_RESULT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     odt_ttc_set_0:16;
}DC_PHY_ODT_TTC_SET0;

typedef struct 
{
unsigned int     odt_ttc_set_1:32;
}DC_PHY_ODT_TTC_SET1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     odt_ttf_set_0:24;
}DC_PHY_ODT_TTF_SET0;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     odt_ttf_set_1:24;
}DC_PHY_ODT_TTF_SET1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     odt_ttf_set_2:24;
}DC_PHY_ODT_TTF_SET2;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ocd_set_0:28;
}DC_PHY_OCD_SET0;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ocd_set_1:28;
}DC_PHY_OCD_SET1;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ocd_set_2:28;
}DC_PHY_OCD_SET2;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     ocd_set_3:28;
}DC_PHY_OCD_SET3;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     pad_bus_0:29;
}DC_PHY_PAD_BUS_0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     pad_bus_1:25;
}DC_PHY_PAD_BUS_1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     pad_bus_2:25;
}DC_PHY_PAD_BUS_2;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     pad_bus_3:25;
}DC_PHY_PAD_BUS_3;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pad_bus_4:16;
}DC_PHY_PAD_BUS_4;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     pad_bus_5:16;
}DC_PHY_PAD_BUS_5;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     issue_cmd_dly_1T:1;
}DC_PHY_CMD_DLY_1T;

#endif

#define DC_PHY_TMCTRL                                                                0x18008800
#define DC_PHY_TMCTRL_reg_addr                                                       "0xB8008800"
#define DC_PHY_TMCTRL_reg                                                            0xB8008800
#define set_DC_PHY_TMCTRL_reg(data)   (*((volatile unsigned int*) DC_PHY_TMCTRL_reg)=data)
#define get_DC_PHY_TMCTRL_reg   (*((volatile unsigned int*) DC_PHY_TMCTRL_reg))
#define DC_PHY_TMCTRL_inst_adr                                                       "0x0000"
#define DC_PHY_TMCTRL_inst                                                           0x0000
#define DC_PHY_TMCTRL_dram_2g_used_shift                                             (31)
#define DC_PHY_TMCTRL_dram_2g_used_mask                                              (0x80000000)
#define DC_PHY_TMCTRL_dram_2g_used(data)                                             (0x80000000&((data)<<31))
#define DC_PHY_TMCTRL_dram_2g_used_src(data)                                         ((0x80000000&(data))>>31)
#define DC_PHY_TMCTRL_get_dram_2g_used(data)                                         ((0x80000000&(data))>>31)
#define DC_PHY_TMCTRL_sclk_sel_shift                                                 (30)
#define DC_PHY_TMCTRL_sclk_sel_mask                                                  (0x40000000)
#define DC_PHY_TMCTRL_sclk_sel(data)                                                 (0x40000000&((data)<<30))
#define DC_PHY_TMCTRL_sclk_sel_src(data)                                             ((0x40000000&(data))>>30)
#define DC_PHY_TMCTRL_get_sclk_sel(data)                                             ((0x40000000&(data))>>30)
#define DC_PHY_TMCTRL_tmras_shift                                                    (25)
#define DC_PHY_TMCTRL_tmras_mask                                                     (0x3E000000)
#define DC_PHY_TMCTRL_tmras(data)                                                    (0x3E000000&((data)<<25))
#define DC_PHY_TMCTRL_tmras_src(data)                                                ((0x3E000000&(data))>>25)
#define DC_PHY_TMCTRL_get_tmras(data)                                                ((0x3E000000&(data))>>25)
#define DC_PHY_TMCTRL_tmrc_shift                                                     (19)
#define DC_PHY_TMCTRL_tmrc_mask                                                      (0x01F80000)
#define DC_PHY_TMCTRL_tmrc(data)                                                     (0x01F80000&((data)<<19))
#define DC_PHY_TMCTRL_tmrc_src(data)                                                 ((0x01F80000&(data))>>19)
#define DC_PHY_TMCTRL_get_tmrc(data)                                                 ((0x01F80000&(data))>>19)
#define DC_PHY_TMCTRL_tmrcd_shift                                                    (15)
#define DC_PHY_TMCTRL_tmrcd_mask                                                     (0x00078000)
#define DC_PHY_TMCTRL_tmrcd(data)                                                    (0x00078000&((data)<<15))
#define DC_PHY_TMCTRL_tmrcd_src(data)                                                ((0x00078000&(data))>>15)
#define DC_PHY_TMCTRL_get_tmrcd(data)                                                ((0x00078000&(data))>>15)
#define DC_PHY_TMCTRL_tmrp_shift                                                     (11)
#define DC_PHY_TMCTRL_tmrp_mask                                                      (0x00007800)
#define DC_PHY_TMCTRL_tmrp(data)                                                     (0x00007800&((data)<<11))
#define DC_PHY_TMCTRL_tmrp_src(data)                                                 ((0x00007800&(data))>>11)
#define DC_PHY_TMCTRL_get_tmrp(data)                                                 ((0x00007800&(data))>>11)
#define DC_PHY_TMCTRL_tmrrd_shift                                                    (8)
#define DC_PHY_TMCTRL_tmrrd_mask                                                     (0x00000700)
#define DC_PHY_TMCTRL_tmrrd(data)                                                    (0x00000700&((data)<<8))
#define DC_PHY_TMCTRL_tmrrd_src(data)                                                ((0x00000700&(data))>>8)
#define DC_PHY_TMCTRL_get_tmrrd(data)                                                ((0x00000700&(data))>>8)
#define DC_PHY_TMCTRL_tmwr_shift                                                     (4)
#define DC_PHY_TMCTRL_tmwr_mask                                                      (0x000000F0)
#define DC_PHY_TMCTRL_tmwr(data)                                                     (0x000000F0&((data)<<4))
#define DC_PHY_TMCTRL_tmwr_src(data)                                                 ((0x000000F0&(data))>>4)
#define DC_PHY_TMCTRL_get_tmwr(data)                                                 ((0x000000F0&(data))>>4)
#define DC_PHY_TMCTRL_tmwtr_shift                                                    (1)
#define DC_PHY_TMCTRL_tmwtr_mask                                                     (0x0000000E)
#define DC_PHY_TMCTRL_tmwtr(data)                                                    (0x0000000E&((data)<<1))
#define DC_PHY_TMCTRL_tmwtr_src(data)                                                ((0x0000000E&(data))>>1)
#define DC_PHY_TMCTRL_get_tmwtr(data)                                                ((0x0000000E&(data))>>1)
#define DC_PHY_TMCTRL_ddr16_shift                                                    (0)
#define DC_PHY_TMCTRL_ddr16_mask                                                     (0x00000001)
#define DC_PHY_TMCTRL_ddr16(data)                                                    (0x00000001&((data)<<0))
#define DC_PHY_TMCTRL_ddr16_src(data)                                                ((0x00000001&(data))>>0)
#define DC_PHY_TMCTRL_get_ddr16(data)                                                ((0x00000001&(data))>>0)


#define DC_PHY_TMCTRL2                                                               0x18008804
#define DC_PHY_TMCTRL2_reg_addr                                                      "0xB8008804"
#define DC_PHY_TMCTRL2_reg                                                           0xB8008804
#define set_DC_PHY_TMCTRL2_reg(data)   (*((volatile unsigned int*) DC_PHY_TMCTRL2_reg)=data)
#define get_DC_PHY_TMCTRL2_reg   (*((volatile unsigned int*) DC_PHY_TMCTRL2_reg))
#define DC_PHY_TMCTRL2_inst_adr                                                      "0x0001"
#define DC_PHY_TMCTRL2_inst                                                          0x0001
#define DC_PHY_TMCTRL2_cal_rang_shift                                                (8)
#define DC_PHY_TMCTRL2_cal_rang_mask                                                 (0x00003F00)
#define DC_PHY_TMCTRL2_cal_rang(data)                                                (0x00003F00&((data)<<8))
#define DC_PHY_TMCTRL2_cal_rang_src(data)                                            ((0x00003F00&(data))>>8)
#define DC_PHY_TMCTRL2_get_cal_rang(data)                                            ((0x00003F00&(data))>>8)
#define DC_PHY_TMCTRL2_cal_per_shift                                                 (0)
#define DC_PHY_TMCTRL2_cal_per_mask                                                  (0x000000FF)
#define DC_PHY_TMCTRL2_cal_per(data)                                                 (0x000000FF&((data)<<0))
#define DC_PHY_TMCTRL2_cal_per_src(data)                                             ((0x000000FF&(data))>>0)
#define DC_PHY_TMCTRL2_get_cal_per(data)                                             ((0x000000FF&(data))>>0)


#define DC_PHY_MOD_REG                                                               0x18008808
#define DC_PHY_MOD_REG_reg_addr                                                      "0xB8008808"
#define DC_PHY_MOD_REG_reg                                                           0xB8008808
#define set_DC_PHY_MOD_REG_reg(data)   (*((volatile unsigned int*) DC_PHY_MOD_REG_reg)=data)
#define get_DC_PHY_MOD_REG_reg   (*((volatile unsigned int*) DC_PHY_MOD_REG_reg))
#define DC_PHY_MOD_REG_inst_adr                                                      "0x0002"
#define DC_PHY_MOD_REG_inst                                                          0x0002
#define DC_PHY_MOD_REG_exmod_reg_shift                                               (16)
#define DC_PHY_MOD_REG_exmod_reg_mask                                                (0xFFFF0000)
#define DC_PHY_MOD_REG_exmod_reg(data)                                               (0xFFFF0000&((data)<<16))
#define DC_PHY_MOD_REG_exmod_reg_src(data)                                           ((0xFFFF0000&(data))>>16)
#define DC_PHY_MOD_REG_get_exmod_reg(data)                                           ((0xFFFF0000&(data))>>16)
#define DC_PHY_MOD_REG_mod_reg_shift                                                 (0)
#define DC_PHY_MOD_REG_mod_reg_mask                                                  (0x0000FFFF)
#define DC_PHY_MOD_REG_mod_reg(data)                                                 (0x0000FFFF&((data)<<0))
#define DC_PHY_MOD_REG_mod_reg_src(data)                                             ((0x0000FFFF&(data))>>0)
#define DC_PHY_MOD_REG_get_mod_reg(data)                                             ((0x0000FFFF&(data))>>0)


#define DC_PHY_TMCTRL2A                                                              0x1800880C
#define DC_PHY_TMCTRL2A_reg_addr                                                     "0xB800880C"
#define DC_PHY_TMCTRL2A_reg                                                          0xB800880C
#define set_DC_PHY_TMCTRL2A_reg(data)   (*((volatile unsigned int*) DC_PHY_TMCTRL2A_reg)=data)
#define get_DC_PHY_TMCTRL2A_reg   (*((volatile unsigned int*) DC_PHY_TMCTRL2A_reg))
#define DC_PHY_TMCTRL2A_inst_adr                                                     "0x0003"
#define DC_PHY_TMCTRL2A_inst                                                         0x0003
#define DC_PHY_TMCTRL2A_ddr3_odt_dly_1T_shift                                        (14)
#define DC_PHY_TMCTRL2A_ddr3_odt_dly_1T_mask                                         (0x00004000)
#define DC_PHY_TMCTRL2A_ddr3_odt_dly_1T(data)                                        (0x00004000&((data)<<14))
#define DC_PHY_TMCTRL2A_ddr3_odt_dly_1T_src(data)                                    ((0x00004000&(data))>>14)
#define DC_PHY_TMCTRL2A_get_ddr3_odt_dly_1T(data)                                    ((0x00004000&(data))>>14)
#define DC_PHY_TMCTRL2A_ddr3_odt_ext_1T_shift                                        (13)
#define DC_PHY_TMCTRL2A_ddr3_odt_ext_1T_mask                                         (0x00002000)
#define DC_PHY_TMCTRL2A_ddr3_odt_ext_1T(data)                                        (0x00002000&((data)<<13))
#define DC_PHY_TMCTRL2A_ddr3_odt_ext_1T_src(data)                                    ((0x00002000&(data))>>13)
#define DC_PHY_TMCTRL2A_get_ddr3_odt_ext_1T(data)                                    ((0x00002000&(data))>>13)
#define DC_PHY_TMCTRL2A_ctrl_odt_en_shift                                            (12)
#define DC_PHY_TMCTRL2A_ctrl_odt_en_mask                                             (0x00001000)
#define DC_PHY_TMCTRL2A_ctrl_odt_en(data)                                            (0x00001000&((data)<<12))
#define DC_PHY_TMCTRL2A_ctrl_odt_en_src(data)                                        ((0x00001000&(data))>>12)
#define DC_PHY_TMCTRL2A_get_ctrl_odt_en(data)                                        ((0x00001000&(data))>>12)
#define DC_PHY_TMCTRL2A_mars_odt_force_sel_shift                                     (11)
#define DC_PHY_TMCTRL2A_mars_odt_force_sel_mask                                      (0x00000800)
#define DC_PHY_TMCTRL2A_mars_odt_force_sel(data)                                     (0x00000800&((data)<<11))
#define DC_PHY_TMCTRL2A_mars_odt_force_sel_src(data)                                 ((0x00000800&(data))>>11)
#define DC_PHY_TMCTRL2A_get_mars_odt_force_sel(data)                                 ((0x00000800&(data))>>11)
#define DC_PHY_TMCTRL2A_mars_odt_force_sig_shift                                     (10)
#define DC_PHY_TMCTRL2A_mars_odt_force_sig_mask                                      (0x00000400)
#define DC_PHY_TMCTRL2A_mars_odt_force_sig(data)                                     (0x00000400&((data)<<10))
#define DC_PHY_TMCTRL2A_mars_odt_force_sig_src(data)                                 ((0x00000400&(data))>>10)
#define DC_PHY_TMCTRL2A_get_mars_odt_force_sig(data)                                 ((0x00000400&(data))>>10)
#define DC_PHY_TMCTRL2A_odt_force_sel_shift                                          (9)
#define DC_PHY_TMCTRL2A_odt_force_sel_mask                                           (0x00000200)
#define DC_PHY_TMCTRL2A_odt_force_sel(data)                                          (0x00000200&((data)<<9))
#define DC_PHY_TMCTRL2A_odt_force_sel_src(data)                                      ((0x00000200&(data))>>9)
#define DC_PHY_TMCTRL2A_get_odt_force_sel(data)                                      ((0x00000200&(data))>>9)
#define DC_PHY_TMCTRL2A_odt_force_sig_shift                                          (8)
#define DC_PHY_TMCTRL2A_odt_force_sig_mask                                           (0x00000100)
#define DC_PHY_TMCTRL2A_odt_force_sig(data)                                          (0x00000100&((data)<<8))
#define DC_PHY_TMCTRL2A_odt_force_sig_src(data)                                      ((0x00000100&(data))>>8)
#define DC_PHY_TMCTRL2A_get_odt_force_sig(data)                                      ((0x00000100&(data))>>8)
#define DC_PHY_TMCTRL2A_odt_en_shift                                                 (7)
#define DC_PHY_TMCTRL2A_odt_en_mask                                                  (0x00000080)
#define DC_PHY_TMCTRL2A_odt_en(data)                                                 (0x00000080&((data)<<7))
#define DC_PHY_TMCTRL2A_odt_en_src(data)                                             ((0x00000080&(data))>>7)
#define DC_PHY_TMCTRL2A_get_odt_en(data)                                             ((0x00000080&(data))>>7)
#define DC_PHY_TMCTRL2A_odt_dis_shift                                                (6)
#define DC_PHY_TMCTRL2A_odt_dis_mask                                                 (0x00000040)
#define DC_PHY_TMCTRL2A_odt_dis(data)                                                (0x00000040&((data)<<6))
#define DC_PHY_TMCTRL2A_odt_dis_src(data)                                            ((0x00000040&(data))>>6)
#define DC_PHY_TMCTRL2A_get_odt_dis(data)                                            ((0x00000040&(data))>>6)
#define DC_PHY_TMCTRL2A_exe_cal_shift                                                (5)
#define DC_PHY_TMCTRL2A_exe_cal_mask                                                 (0x00000020)
#define DC_PHY_TMCTRL2A_exe_cal(data)                                                (0x00000020&((data)<<5))
#define DC_PHY_TMCTRL2A_exe_cal_src(data)                                            ((0x00000020&(data))>>5)
#define DC_PHY_TMCTRL2A_get_exe_cal(data)                                            ((0x00000020&(data))>>5)
#define DC_PHY_TMCTRL2A_init_shift                                                   (4)
#define DC_PHY_TMCTRL2A_init_mask                                                    (0x00000010)
#define DC_PHY_TMCTRL2A_init(data)                                                   (0x00000010&((data)<<4))
#define DC_PHY_TMCTRL2A_init_src(data)                                               ((0x00000010&(data))>>4)
#define DC_PHY_TMCTRL2A_get_init(data)                                               ((0x00000010&(data))>>4)
#define DC_PHY_TMCTRL2A_sren_shift                                                   (3)
#define DC_PHY_TMCTRL2A_sren_mask                                                    (0x00000008)
#define DC_PHY_TMCTRL2A_sren(data)                                                   (0x00000008&((data)<<3))
#define DC_PHY_TMCTRL2A_sren_src(data)                                               ((0x00000008&(data))>>3)
#define DC_PHY_TMCTRL2A_get_sren(data)                                               ((0x00000008&(data))>>3)
#define DC_PHY_TMCTRL2A_srex_shift                                                   (2)
#define DC_PHY_TMCTRL2A_srex_mask                                                    (0x00000004)
#define DC_PHY_TMCTRL2A_srex(data)                                                   (0x00000004&((data)<<2))
#define DC_PHY_TMCTRL2A_srex_src(data)                                               ((0x00000004&(data))>>2)
#define DC_PHY_TMCTRL2A_get_srex(data)                                               ((0x00000004&(data))>>2)
#define DC_PHY_TMCTRL2A_exe_emr_shift                                                (1)
#define DC_PHY_TMCTRL2A_exe_emr_mask                                                 (0x00000002)
#define DC_PHY_TMCTRL2A_exe_emr(data)                                                (0x00000002&((data)<<1))
#define DC_PHY_TMCTRL2A_exe_emr_src(data)                                            ((0x00000002&(data))>>1)
#define DC_PHY_TMCTRL2A_get_exe_emr(data)                                            ((0x00000002&(data))>>1)
#define DC_PHY_TMCTRL2A_exe_mr_shift                                                 (0)
#define DC_PHY_TMCTRL2A_exe_mr_mask                                                  (0x00000001)
#define DC_PHY_TMCTRL2A_exe_mr(data)                                                 (0x00000001&((data)<<0))
#define DC_PHY_TMCTRL2A_exe_mr_src(data)                                             ((0x00000001&(data))>>0)
#define DC_PHY_TMCTRL2A_get_exe_mr(data)                                             ((0x00000001&(data))>>0)


#define DC_PHY_DC_MIS                                                                0x18008818
#define DC_PHY_DC_MIS_reg_addr                                                       "0xB8008818"
#define DC_PHY_DC_MIS_reg                                                            0xB8008818
#define set_DC_PHY_DC_MIS_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_MIS_reg)=data)
#define get_DC_PHY_DC_MIS_reg   (*((volatile unsigned int*) DC_PHY_DC_MIS_reg))
#define DC_PHY_DC_MIS_inst_adr                                                       "0x0006"
#define DC_PHY_DC_MIS_inst                                                           0x0006
#define DC_PHY_DC_MIS_ddr3_512_used_shift                                            (31)
#define DC_PHY_DC_MIS_ddr3_512_used_mask                                             (0x80000000)
#define DC_PHY_DC_MIS_ddr3_512_used(data)                                            (0x80000000&((data)<<31))
#define DC_PHY_DC_MIS_ddr3_512_used_src(data)                                        ((0x80000000&(data))>>31)
#define DC_PHY_DC_MIS_get_ddr3_512_used(data)                                        ((0x80000000&(data))>>31)
#define DC_PHY_DC_MIS_tmcl_rd_4_shift                                                (30)
#define DC_PHY_DC_MIS_tmcl_rd_4_mask                                                 (0x40000000)
#define DC_PHY_DC_MIS_tmcl_rd_4(data)                                                (0x40000000&((data)<<30))
#define DC_PHY_DC_MIS_tmcl_rd_4_src(data)                                            ((0x40000000&(data))>>30)
#define DC_PHY_DC_MIS_get_tmcl_rd_4(data)                                            ((0x40000000&(data))>>30)
#define DC_PHY_DC_MIS_odt_en_lat_2_shift                                             (21)
#define DC_PHY_DC_MIS_odt_en_lat_2_mask                                              (0x0FE00000)
#define DC_PHY_DC_MIS_odt_en_lat_2(data)                                             (0x0FE00000&((data)<<21))
#define DC_PHY_DC_MIS_odt_en_lat_2_src(data)                                         ((0x0FE00000&(data))>>21)
#define DC_PHY_DC_MIS_get_odt_en_lat_2(data)                                         ((0x0FE00000&(data))>>21)
#define DC_PHY_DC_MIS_rcd_not_eq_shift                                               (20)
#define DC_PHY_DC_MIS_rcd_not_eq_mask                                                (0x00100000)
#define DC_PHY_DC_MIS_rcd_not_eq(data)                                               (0x00100000&((data)<<20))
#define DC_PHY_DC_MIS_rcd_not_eq_src(data)                                           ((0x00100000&(data))>>20)
#define DC_PHY_DC_MIS_get_rcd_not_eq(data)                                           ((0x00100000&(data))>>20)
#define DC_PHY_DC_MIS_tmrcdwr_shift                                                  (16)
#define DC_PHY_DC_MIS_tmrcdwr_mask                                                   (0x000F0000)
#define DC_PHY_DC_MIS_tmrcdwr(data)                                                  (0x000F0000&((data)<<16))
#define DC_PHY_DC_MIS_tmrcdwr_src(data)                                              ((0x000F0000&(data))>>16)
#define DC_PHY_DC_MIS_get_tmrcdwr(data)                                              ((0x000F0000&(data))>>16)
#define DC_PHY_DC_MIS_tmrp_all_add_shift                                             (14)
#define DC_PHY_DC_MIS_tmrp_all_add_mask                                              (0x0000C000)
#define DC_PHY_DC_MIS_tmrp_all_add(data)                                             (0x0000C000&((data)<<14))
#define DC_PHY_DC_MIS_tmrp_all_add_src(data)                                         ((0x0000C000&(data))>>14)
#define DC_PHY_DC_MIS_get_tmrp_all_add(data)                                         ((0x0000C000&(data))>>14)
#define DC_PHY_DC_MIS_ap_wo_min_dly_shift                                            (13)
#define DC_PHY_DC_MIS_ap_wo_min_dly_mask                                             (0x00002000)
#define DC_PHY_DC_MIS_ap_wo_min_dly(data)                                            (0x00002000&((data)<<13))
#define DC_PHY_DC_MIS_ap_wo_min_dly_src(data)                                        ((0x00002000&(data))>>13)
#define DC_PHY_DC_MIS_get_ap_wo_min_dly(data)                                        ((0x00002000&(data))>>13)
#define DC_PHY_DC_MIS_add_ht_shift                                                   (12)
#define DC_PHY_DC_MIS_add_ht_mask                                                    (0x00001000)
#define DC_PHY_DC_MIS_add_ht(data)                                                   (0x00001000&((data)<<12))
#define DC_PHY_DC_MIS_add_ht_src(data)                                               ((0x00001000&(data))>>12)
#define DC_PHY_DC_MIS_get_add_ht(data)                                               ((0x00001000&(data))>>12)
#define DC_PHY_DC_MIS_tmcl_rd_shift                                                  (8)
#define DC_PHY_DC_MIS_tmcl_rd_mask                                                   (0x00000F00)
#define DC_PHY_DC_MIS_tmcl_rd(data)                                                  (0x00000F00&((data)<<8))
#define DC_PHY_DC_MIS_tmcl_rd_src(data)                                              ((0x00000F00&(data))>>8)
#define DC_PHY_DC_MIS_get_tmcl_rd(data)                                              ((0x00000F00&(data))>>8)
#define DC_PHY_DC_MIS_odt_en_lat_shift                                               (0)
#define DC_PHY_DC_MIS_odt_en_lat_mask                                                (0x000000FF)
#define DC_PHY_DC_MIS_odt_en_lat(data)                                               (0x000000FF&((data)<<0))
#define DC_PHY_DC_MIS_odt_en_lat_src(data)                                           ((0x000000FF&(data))>>0)
#define DC_PHY_DC_MIS_get_odt_en_lat(data)                                           ((0x000000FF&(data))>>0)


#define DC_PHY_DC_MISA                                                               0x18008824
#define DC_PHY_DC_MISA_reg_addr                                                      "0xB8008824"
#define DC_PHY_DC_MISA_reg                                                           0xB8008824
#define set_DC_PHY_DC_MISA_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_MISA_reg)=data)
#define get_DC_PHY_DC_MISA_reg   (*((volatile unsigned int*) DC_PHY_DC_MISA_reg))
#define DC_PHY_DC_MISA_inst_adr                                                      "0x0009"
#define DC_PHY_DC_MISA_inst                                                          0x0009
#define DC_PHY_DC_MISA_pre_dis_shift                                                 (12)
#define DC_PHY_DC_MISA_pre_dis_mask                                                  (0x00001000)
#define DC_PHY_DC_MISA_pre_dis(data)                                                 (0x00001000&((data)<<12))
#define DC_PHY_DC_MISA_pre_dis_src(data)                                             ((0x00001000&(data))>>12)
#define DC_PHY_DC_MISA_get_pre_dis(data)                                             ((0x00001000&(data))>>12)
#define DC_PHY_DC_MISA_mars_BGA_shift                                                (11)
#define DC_PHY_DC_MISA_mars_BGA_mask                                                 (0x00000800)
#define DC_PHY_DC_MISA_mars_BGA(data)                                                (0x00000800&((data)<<11))
#define DC_PHY_DC_MISA_mars_BGA_src(data)                                            ((0x00000800&(data))>>11)
#define DC_PHY_DC_MISA_get_mars_BGA(data)                                            ((0x00000800&(data))>>11)
#define DC_PHY_DC_MISA_lbk_en_shift                                                  (2)
#define DC_PHY_DC_MISA_lbk_en_mask                                                   (0x00000004)
#define DC_PHY_DC_MISA_lbk_en(data)                                                  (0x00000004&((data)<<2))
#define DC_PHY_DC_MISA_lbk_en_src(data)                                              ((0x00000004&(data))>>2)
#define DC_PHY_DC_MISA_get_lbk_en(data)                                              ((0x00000004&(data))>>2)
#define DC_PHY_DC_MISA_al_shift                                                      (1)
#define DC_PHY_DC_MISA_al_mask                                                       (0x00000002)
#define DC_PHY_DC_MISA_al(data)                                                      (0x00000002&((data)<<1))
#define DC_PHY_DC_MISA_al_src(data)                                                  ((0x00000002&(data))>>1)
#define DC_PHY_DC_MISA_get_al(data)                                                  ((0x00000002&(data))>>1)
#define DC_PHY_DC_MISA_ddr2_shift                                                    (0)
#define DC_PHY_DC_MISA_ddr2_mask                                                     (0x00000001)
#define DC_PHY_DC_MISA_ddr2(data)                                                    (0x00000001&((data)<<0))
#define DC_PHY_DC_MISA_ddr2_src(data)                                                ((0x00000001&(data))>>0)
#define DC_PHY_DC_MISA_get_ddr2(data)                                                ((0x00000001&(data))>>0)


#define DC_PHY_DC_MISB                                                               0x18008828
#define DC_PHY_DC_MISB_reg_addr                                                      "0xB8008828"
#define DC_PHY_DC_MISB_reg                                                           0xB8008828
#define set_DC_PHY_DC_MISB_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_MISB_reg)=data)
#define get_DC_PHY_DC_MISB_reg   (*((volatile unsigned int*) DC_PHY_DC_MISB_reg))
#define DC_PHY_DC_MISB_inst_adr                                                      "0x000A"
#define DC_PHY_DC_MISB_inst                                                          0x000A
#define DC_PHY_DC_MISB_tmrfc_2_shift                                                 (29)
#define DC_PHY_DC_MISB_tmrfc_2_mask                                                  (0x60000000)
#define DC_PHY_DC_MISB_tmrfc_2(data)                                                 (0x60000000&((data)<<29))
#define DC_PHY_DC_MISB_tmrfc_2_src(data)                                             ((0x60000000&(data))>>29)
#define DC_PHY_DC_MISB_get_tmrfc_2(data)                                             ((0x60000000&(data))>>29)
#define DC_PHY_DC_MISB_tmrc_sel_shift                                                (28)
#define DC_PHY_DC_MISB_tmrc_sel_mask                                                 (0x10000000)
#define DC_PHY_DC_MISB_tmrc_sel(data)                                                (0x10000000&((data)<<28))
#define DC_PHY_DC_MISB_tmrc_sel_src(data)                                            ((0x10000000&(data))>>28)
#define DC_PHY_DC_MISB_get_tmrc_sel(data)                                            ((0x10000000&(data))>>28)
#define DC_PHY_DC_MISB_pasr_shift                                                    (25)
#define DC_PHY_DC_MISB_pasr_mask                                                     (0x0E000000)
#define DC_PHY_DC_MISB_pasr(data)                                                    (0x0E000000&((data)<<25))
#define DC_PHY_DC_MISB_pasr_src(data)                                                ((0x0E000000&(data))>>25)
#define DC_PHY_DC_MISB_get_pasr(data)                                                ((0x0E000000&(data))>>25)
#define DC_PHY_DC_MISB_srf_shift                                                     (24)
#define DC_PHY_DC_MISB_srf_mask                                                      (0x01000000)
#define DC_PHY_DC_MISB_srf(data)                                                     (0x01000000&((data)<<24))
#define DC_PHY_DC_MISB_srf_src(data)                                                 ((0x01000000&(data))>>24)
#define DC_PHY_DC_MISB_get_srf(data)                                                 ((0x01000000&(data))>>24)
#define DC_PHY_DC_MISB_trtp_shift                                                    (21)
#define DC_PHY_DC_MISB_trtp_mask                                                     (0x00E00000)
#define DC_PHY_DC_MISB_trtp(data)                                                    (0x00E00000&((data)<<21))
#define DC_PHY_DC_MISB_trtp_src(data)                                                ((0x00E00000&(data))>>21)
#define DC_PHY_DC_MISB_get_trtp(data)                                                ((0x00E00000&(data))>>21)
#define DC_PHY_DC_MISB_tmrfc_shift                                                   (14)
#define DC_PHY_DC_MISB_tmrfc_mask                                                    (0x001FC000)
#define DC_PHY_DC_MISB_tmrfc(data)                                                   (0x001FC000&((data)<<14))
#define DC_PHY_DC_MISB_tmrfc_src(data)                                               ((0x001FC000&(data))>>14)
#define DC_PHY_DC_MISB_get_tmrfc(data)                                               ((0x001FC000&(data))>>14)
#define DC_PHY_DC_MISB_refcycle_shift                                                (8)
#define DC_PHY_DC_MISB_refcycle_mask                                                 (0x00003F00)
#define DC_PHY_DC_MISB_refcycle(data)                                                (0x00003F00&((data)<<8))
#define DC_PHY_DC_MISB_refcycle_src(data)                                            ((0x00003F00&(data))>>8)
#define DC_PHY_DC_MISB_get_refcycle(data)                                            ((0x00003F00&(data))>>8)
#define DC_PHY_DC_MISB_tmmrd_shift                                                   (5)
#define DC_PHY_DC_MISB_tmmrd_mask                                                    (0x000000E0)
#define DC_PHY_DC_MISB_tmmrd(data)                                                   (0x000000E0&((data)<<5))
#define DC_PHY_DC_MISB_tmmrd_src(data)                                               ((0x000000E0&(data))>>5)
#define DC_PHY_DC_MISB_get_tmmrd(data)                                               ((0x000000E0&(data))>>5)
#define DC_PHY_DC_MISB_tmcl_shift                                                    (0)
#define DC_PHY_DC_MISB_tmcl_mask                                                     (0x0000001F)
#define DC_PHY_DC_MISB_tmcl(data)                                                    (0x0000001F&((data)<<0))
#define DC_PHY_DC_MISB_tmcl_src(data)                                                ((0x0000001F&(data))>>0)
#define DC_PHY_DC_MISB_get_tmcl(data)                                                ((0x0000001F&(data))>>0)


#define DC_PHY_DC_MISC                                                               0x1800882C
#define DC_PHY_DC_MISC_reg_addr                                                      "0xB800882C"
#define DC_PHY_DC_MISC_reg                                                           0xB800882C
#define set_DC_PHY_DC_MISC_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_MISC_reg)=data)
#define get_DC_PHY_DC_MISC_reg   (*((volatile unsigned int*) DC_PHY_DC_MISC_reg))
#define DC_PHY_DC_MISC_inst_adr                                                      "0x000B"
#define DC_PHY_DC_MISC_inst                                                          0x000B
#define DC_PHY_DC_MISC_tap_mask_shift                                                (26)
#define DC_PHY_DC_MISC_tap_mask_mask                                                 (0x04000000)
#define DC_PHY_DC_MISC_tap_mask(data)                                                (0x04000000&((data)<<26))
#define DC_PHY_DC_MISC_tap_mask_src(data)                                            ((0x04000000&(data))>>26)
#define DC_PHY_DC_MISC_get_tap_mask(data)                                            ((0x04000000&(data))>>26)
#define DC_PHY_DC_MISC_fw_set_shift                                                  (25)
#define DC_PHY_DC_MISC_fw_set_mask                                                   (0x02000000)
#define DC_PHY_DC_MISC_fw_set(data)                                                  (0x02000000&((data)<<25))
#define DC_PHY_DC_MISC_fw_set_src(data)                                              ((0x02000000&(data))>>25)
#define DC_PHY_DC_MISC_get_fw_set(data)                                              ((0x02000000&(data))>>25)
#define DC_PHY_DC_MISC_tap_sel_shift                                                 (24)
#define DC_PHY_DC_MISC_tap_sel_mask                                                  (0x01000000)
#define DC_PHY_DC_MISC_tap_sel(data)                                                 (0x01000000&((data)<<24))
#define DC_PHY_DC_MISC_tap_sel_src(data)                                             ((0x01000000&(data))>>24)
#define DC_PHY_DC_MISC_get_tap_sel(data)                                             ((0x01000000&(data))>>24)


#define DC_PHY_DC_TEST_MODE_SEL                                                      0x18008834
#define DC_PHY_DC_TEST_MODE_SEL_reg_addr                                             "0xB8008834"
#define DC_PHY_DC_TEST_MODE_SEL_reg                                                  0xB8008834
#define set_DC_PHY_DC_TEST_MODE_SEL_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_TEST_MODE_SEL_reg)=data)
#define get_DC_PHY_DC_TEST_MODE_SEL_reg   (*((volatile unsigned int*) DC_PHY_DC_TEST_MODE_SEL_reg))
#define DC_PHY_DC_TEST_MODE_SEL_inst_adr                                             "0x000D"
#define DC_PHY_DC_TEST_MODE_SEL_inst                                                 0x000D
#define DC_PHY_DC_TEST_MODE_SEL_clear_set_sys_shift                                  (2)
#define DC_PHY_DC_TEST_MODE_SEL_clear_set_sys_mask                                   (0x00000004)
#define DC_PHY_DC_TEST_MODE_SEL_clear_set_sys(data)                                  (0x00000004&((data)<<2))
#define DC_PHY_DC_TEST_MODE_SEL_clear_set_sys_src(data)                              ((0x00000004&(data))>>2)
#define DC_PHY_DC_TEST_MODE_SEL_get_clear_set_sys(data)                              ((0x00000004&(data))>>2)
#define DC_PHY_DC_TEST_MODE_SEL_clear_trig_shift                                     (1)
#define DC_PHY_DC_TEST_MODE_SEL_clear_trig_mask                                      (0x00000002)
#define DC_PHY_DC_TEST_MODE_SEL_clear_trig(data)                                     (0x00000002&((data)<<1))
#define DC_PHY_DC_TEST_MODE_SEL_clear_trig_src(data)                                 ((0x00000002&(data))>>1)
#define DC_PHY_DC_TEST_MODE_SEL_get_clear_trig(data)                                 ((0x00000002&(data))>>1)
#define DC_PHY_DC_TEST_MODE_SEL_test_mode_sel_shift                                  (0)
#define DC_PHY_DC_TEST_MODE_SEL_test_mode_sel_mask                                   (0x00000001)
#define DC_PHY_DC_TEST_MODE_SEL_test_mode_sel(data)                                  (0x00000001&((data)<<0))
#define DC_PHY_DC_TEST_MODE_SEL_test_mode_sel_src(data)                              ((0x00000001&(data))>>0)
#define DC_PHY_DC_TEST_MODE_SEL_get_test_mode_sel(data)                              ((0x00000001&(data))>>0)


#define DC_PHY_TMCTRL2B                                                              0x18008838
#define DC_PHY_TMCTRL2B_reg_addr                                                     "0xB8008838"
#define DC_PHY_TMCTRL2B_reg                                                          0xB8008838
#define set_DC_PHY_TMCTRL2B_reg(data)   (*((volatile unsigned int*) DC_PHY_TMCTRL2B_reg)=data)
#define get_DC_PHY_TMCTRL2B_reg   (*((volatile unsigned int*) DC_PHY_TMCTRL2B_reg))
#define DC_PHY_TMCTRL2B_inst_adr                                                     "0x000E"
#define DC_PHY_TMCTRL2B_inst                                                         0x000E
#define DC_PHY_TMCTRL2B_tAOFD_shift                                                  (4)
#define DC_PHY_TMCTRL2B_tAOFD_mask                                                   (0x000000F0)
#define DC_PHY_TMCTRL2B_tAOFD(data)                                                  (0x000000F0&((data)<<4))
#define DC_PHY_TMCTRL2B_tAOFD_src(data)                                              ((0x000000F0&(data))>>4)
#define DC_PHY_TMCTRL2B_get_tAOFD(data)                                              ((0x000000F0&(data))>>4)
#define DC_PHY_TMCTRL2B_tAOND_shift                                                  (0)
#define DC_PHY_TMCTRL2B_tAOND_mask                                                   (0x0000000F)
#define DC_PHY_TMCTRL2B_tAOND(data)                                                  (0x0000000F&((data)<<0))
#define DC_PHY_TMCTRL2B_tAOND_src(data)                                              ((0x0000000F&(data))>>0)
#define DC_PHY_TMCTRL2B_get_tAOND(data)                                              ((0x0000000F&(data))>>0)


#define DC_PHY_DC_OCD                                                                0x1800883C
#define DC_PHY_DC_OCD_reg_addr                                                       "0xB800883C"
#define DC_PHY_DC_OCD_reg                                                            0xB800883C
#define set_DC_PHY_DC_OCD_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_OCD_reg)=data)
#define get_DC_PHY_DC_OCD_reg   (*((volatile unsigned int*) DC_PHY_DC_OCD_reg))
#define DC_PHY_DC_OCD_inst_adr                                                       "0x000F"
#define DC_PHY_DC_OCD_inst                                                           0x000F
#define DC_PHY_DC_OCD_ocd_dt_shift                                                   (0)
#define DC_PHY_DC_OCD_ocd_dt_mask                                                    (0x0000000F)
#define DC_PHY_DC_OCD_ocd_dt(data)                                                   (0x0000000F&((data)<<0))
#define DC_PHY_DC_OCD_ocd_dt_src(data)                                               ((0x0000000F&(data))>>0)
#define DC_PHY_DC_OCD_get_ocd_dt(data)                                               ((0x0000000F&(data))>>0)


#define DC_PHY_DC_DDR3_CTL                                                           0x18008840
#define DC_PHY_DC_DDR3_CTL_reg_addr                                                  "0xB8008840"
#define DC_PHY_DC_DDR3_CTL_reg                                                       0xB8008840
#define set_DC_PHY_DC_DDR3_CTL_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_DDR3_CTL_reg)=data)
#define get_DC_PHY_DC_DDR3_CTL_reg   (*((volatile unsigned int*) DC_PHY_DC_DDR3_CTL_reg))
#define DC_PHY_DC_DDR3_CTL_inst_adr                                                  "0x0010"
#define DC_PHY_DC_DDR3_CTL_inst                                                      0x0010
#define DC_PHY_DC_DDR3_CTL_write_en_14_shift                                         (29)
#define DC_PHY_DC_DDR3_CTL_write_en_14_mask                                          (0x20000000)
#define DC_PHY_DC_DDR3_CTL_write_en_14(data)                                         (0x20000000&((data)<<29))
#define DC_PHY_DC_DDR3_CTL_write_en_14_src(data)                                     ((0x20000000&(data))>>29)
#define DC_PHY_DC_DDR3_CTL_get_write_en_14(data)                                     ((0x20000000&(data))>>29)
#define DC_PHY_DC_DDR3_CTL_ddr3_swap_shift                                           (28)
#define DC_PHY_DC_DDR3_CTL_ddr3_swap_mask                                            (0x10000000)
#define DC_PHY_DC_DDR3_CTL_ddr3_swap(data)                                           (0x10000000&((data)<<28))
#define DC_PHY_DC_DDR3_CTL_ddr3_swap_src(data)                                       ((0x10000000&(data))>>28)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_swap(data)                                       ((0x10000000&(data))>>28)
#define DC_PHY_DC_DDR3_CTL_write_en_13_shift                                         (27)
#define DC_PHY_DC_DDR3_CTL_write_en_13_mask                                          (0x08000000)
#define DC_PHY_DC_DDR3_CTL_write_en_13(data)                                         (0x08000000&((data)<<27))
#define DC_PHY_DC_DDR3_CTL_write_en_13_src(data)                                     ((0x08000000&(data))>>27)
#define DC_PHY_DC_DDR3_CTL_get_write_en_13(data)                                     ((0x08000000&(data))>>27)
#define DC_PHY_DC_DDR3_CTL_end_500us_shift                                           (26)
#define DC_PHY_DC_DDR3_CTL_end_500us_mask                                            (0x04000000)
#define DC_PHY_DC_DDR3_CTL_end_500us(data)                                           (0x04000000&((data)<<26))
#define DC_PHY_DC_DDR3_CTL_end_500us_src(data)                                       ((0x04000000&(data))>>26)
#define DC_PHY_DC_DDR3_CTL_get_end_500us(data)                                       ((0x04000000&(data))>>26)
#define DC_PHY_DC_DDR3_CTL_write_en_12_shift                                         (25)
#define DC_PHY_DC_DDR3_CTL_write_en_12_mask                                          (0x02000000)
#define DC_PHY_DC_DDR3_CTL_write_en_12(data)                                         (0x02000000&((data)<<25))
#define DC_PHY_DC_DDR3_CTL_write_en_12_src(data)                                     ((0x02000000&(data))>>25)
#define DC_PHY_DC_DDR3_CTL_get_write_en_12(data)                                     ((0x02000000&(data))>>25)
#define DC_PHY_DC_DDR3_CTL_end_200us_shift                                           (24)
#define DC_PHY_DC_DDR3_CTL_end_200us_mask                                            (0x01000000)
#define DC_PHY_DC_DDR3_CTL_end_200us(data)                                           (0x01000000&((data)<<24))
#define DC_PHY_DC_DDR3_CTL_end_200us_src(data)                                       ((0x01000000&(data))>>24)
#define DC_PHY_DC_DDR3_CTL_get_end_200us(data)                                       ((0x01000000&(data))>>24)
#define DC_PHY_DC_DDR3_CTL_write_en_11_shift                                         (23)
#define DC_PHY_DC_DDR3_CTL_write_en_11_mask                                          (0x00800000)
#define DC_PHY_DC_DDR3_CTL_write_en_11(data)                                         (0x00800000&((data)<<23))
#define DC_PHY_DC_DDR3_CTL_write_en_11_src(data)                                     ((0x00800000&(data))>>23)
#define DC_PHY_DC_DDR3_CTL_get_write_en_11(data)                                     ((0x00800000&(data))>>23)
#define DC_PHY_DC_DDR3_CTL_fast_prechg_shift                                         (22)
#define DC_PHY_DC_DDR3_CTL_fast_prechg_mask                                          (0x00400000)
#define DC_PHY_DC_DDR3_CTL_fast_prechg(data)                                         (0x00400000&((data)<<22))
#define DC_PHY_DC_DDR3_CTL_fast_prechg_src(data)                                     ((0x00400000&(data))>>22)
#define DC_PHY_DC_DDR3_CTL_get_fast_prechg(data)                                     ((0x00400000&(data))>>22)
#define DC_PHY_DC_DDR3_CTL_write_en_10_shift                                         (21)
#define DC_PHY_DC_DDR3_CTL_write_en_10_mask                                          (0x00200000)
#define DC_PHY_DC_DDR3_CTL_write_en_10(data)                                         (0x00200000&((data)<<21))
#define DC_PHY_DC_DDR3_CTL_write_en_10_src(data)                                     ((0x00200000&(data))>>21)
#define DC_PHY_DC_DDR3_CTL_get_write_en_10(data)                                     ((0x00200000&(data))>>21)
#define DC_PHY_DC_DDR3_CTL_dis_cti_shift                                             (20)
#define DC_PHY_DC_DDR3_CTL_dis_cti_mask                                              (0x00100000)
#define DC_PHY_DC_DDR3_CTL_dis_cti(data)                                             (0x00100000&((data)<<20))
#define DC_PHY_DC_DDR3_CTL_dis_cti_src(data)                                         ((0x00100000&(data))>>20)
#define DC_PHY_DC_DDR3_CTL_get_dis_cti(data)                                         ((0x00100000&(data))>>20)
#define DC_PHY_DC_DDR3_CTL_write_en_9_shift                                          (19)
#define DC_PHY_DC_DDR3_CTL_write_en_9_mask                                           (0x00080000)
#define DC_PHY_DC_DDR3_CTL_write_en_9(data)                                          (0x00080000&((data)<<19))
#define DC_PHY_DC_DDR3_CTL_write_en_9_src(data)                                      ((0x00080000&(data))>>19)
#define DC_PHY_DC_DDR3_CTL_get_write_en_9(data)                                      ((0x00080000&(data))>>19)
#define DC_PHY_DC_DDR3_CTL_dis_8bank_shift                                           (18)
#define DC_PHY_DC_DDR3_CTL_dis_8bank_mask                                            (0x00040000)
#define DC_PHY_DC_DDR3_CTL_dis_8bank(data)                                           (0x00040000&((data)<<18))
#define DC_PHY_DC_DDR3_CTL_dis_8bank_src(data)                                       ((0x00040000&(data))>>18)
#define DC_PHY_DC_DDR3_CTL_get_dis_8bank(data)                                       ((0x00040000&(data))>>18)
#define DC_PHY_DC_DDR3_CTL_write_en_8_shift                                          (17)
#define DC_PHY_DC_DDR3_CTL_write_en_8_mask                                           (0x00020000)
#define DC_PHY_DC_DDR3_CTL_write_en_8(data)                                          (0x00020000&((data)<<17))
#define DC_PHY_DC_DDR3_CTL_write_en_8_src(data)                                      ((0x00020000&(data))>>17)
#define DC_PHY_DC_DDR3_CTL_get_write_en_8(data)                                      ((0x00020000&(data))>>17)
#define DC_PHY_DC_DDR3_CTL_use_tmmrd_shift                                           (16)
#define DC_PHY_DC_DDR3_CTL_use_tmmrd_mask                                            (0x00010000)
#define DC_PHY_DC_DDR3_CTL_use_tmmrd(data)                                           (0x00010000&((data)<<16))
#define DC_PHY_DC_DDR3_CTL_use_tmmrd_src(data)                                       ((0x00010000&(data))>>16)
#define DC_PHY_DC_DDR3_CTL_get_use_tmmrd(data)                                       ((0x00010000&(data))>>16)
#define DC_PHY_DC_DDR3_CTL_write_en_7_shift                                          (15)
#define DC_PHY_DC_DDR3_CTL_write_en_7_mask                                           (0x00008000)
#define DC_PHY_DC_DDR3_CTL_write_en_7(data)                                          (0x00008000&((data)<<15))
#define DC_PHY_DC_DDR3_CTL_write_en_7_src(data)                                      ((0x00008000&(data))>>15)
#define DC_PHY_DC_DDR3_CTL_get_write_en_7(data)                                      ((0x00008000&(data))>>15)
#define DC_PHY_DC_DDR3_CTL_ddr3_shift                                                (14)
#define DC_PHY_DC_DDR3_CTL_ddr3_mask                                                 (0x00004000)
#define DC_PHY_DC_DDR3_CTL_ddr3(data)                                                (0x00004000&((data)<<14))
#define DC_PHY_DC_DDR3_CTL_ddr3_src(data)                                            ((0x00004000&(data))>>14)
#define DC_PHY_DC_DDR3_CTL_get_ddr3(data)                                            ((0x00004000&(data))>>14)
#define DC_PHY_DC_DDR3_CTL_write_en_6_shift                                          (13)
#define DC_PHY_DC_DDR3_CTL_write_en_6_mask                                           (0x00002000)
#define DC_PHY_DC_DDR3_CTL_write_en_6(data)                                          (0x00002000&((data)<<13))
#define DC_PHY_DC_DDR3_CTL_write_en_6_src(data)                                      ((0x00002000&(data))>>13)
#define DC_PHY_DC_DDR3_CTL_get_write_en_6(data)                                      ((0x00002000&(data))>>13)
#define DC_PHY_DC_DDR3_CTL_ddr3_zqcl_short_shift                                     (12)
#define DC_PHY_DC_DDR3_CTL_ddr3_zqcl_short_mask                                      (0x00001000)
#define DC_PHY_DC_DDR3_CTL_ddr3_zqcl_short(data)                                     (0x00001000&((data)<<12))
#define DC_PHY_DC_DDR3_CTL_ddr3_zqcl_short_src(data)                                 ((0x00001000&(data))>>12)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_zqcl_short(data)                                 ((0x00001000&(data))>>12)
#define DC_PHY_DC_DDR3_CTL_write_en_5_shift                                          (11)
#define DC_PHY_DC_DDR3_CTL_write_en_5_mask                                           (0x00000800)
#define DC_PHY_DC_DDR3_CTL_write_en_5(data)                                          (0x00000800&((data)<<11))
#define DC_PHY_DC_DDR3_CTL_write_en_5_src(data)                                      ((0x00000800&(data))>>11)
#define DC_PHY_DC_DDR3_CTL_get_write_en_5(data)                                      ((0x00000800&(data))>>11)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_zqcl_shift                                       (10)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_zqcl_mask                                        (0x00000400)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_zqcl(data)                                       (0x00000400&((data)<<10))
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_zqcl_src(data)                                   ((0x00000400&(data))>>10)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_exe_zqcl(data)                                   ((0x00000400&(data))>>10)
#define DC_PHY_DC_DDR3_CTL_write_en_4_shift                                          (9)
#define DC_PHY_DC_DDR3_CTL_write_en_4_mask                                           (0x00000200)
#define DC_PHY_DC_DDR3_CTL_write_en_4(data)                                          (0x00000200&((data)<<9))
#define DC_PHY_DC_DDR3_CTL_write_en_4_src(data)                                      ((0x00000200&(data))>>9)
#define DC_PHY_DC_DDR3_CTL_get_write_en_4(data)                                      ((0x00000200&(data))>>9)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr3_shift                                        (8)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr3_mask                                         (0x00000100)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr3(data)                                        (0x00000100&((data)<<8))
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr3_src(data)                                    ((0x00000100&(data))>>8)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_exe_mr3(data)                                    ((0x00000100&(data))>>8)
#define DC_PHY_DC_DDR3_CTL_write_en_3_shift                                          (7)
#define DC_PHY_DC_DDR3_CTL_write_en_3_mask                                           (0x00000080)
#define DC_PHY_DC_DDR3_CTL_write_en_3(data)                                          (0x00000080&((data)<<7))
#define DC_PHY_DC_DDR3_CTL_write_en_3_src(data)                                      ((0x00000080&(data))>>7)
#define DC_PHY_DC_DDR3_CTL_get_write_en_3(data)                                      ((0x00000080&(data))>>7)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr2_shift                                        (6)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr2_mask                                         (0x00000040)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr2(data)                                        (0x00000040&((data)<<6))
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr2_src(data)                                    ((0x00000040&(data))>>6)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_exe_mr2(data)                                    ((0x00000040&(data))>>6)
#define DC_PHY_DC_DDR3_CTL_write_en_2_shift                                          (5)
#define DC_PHY_DC_DDR3_CTL_write_en_2_mask                                           (0x00000020)
#define DC_PHY_DC_DDR3_CTL_write_en_2(data)                                          (0x00000020&((data)<<5))
#define DC_PHY_DC_DDR3_CTL_write_en_2_src(data)                                      ((0x00000020&(data))>>5)
#define DC_PHY_DC_DDR3_CTL_get_write_en_2(data)                                      ((0x00000020&(data))>>5)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr1_shift                                        (4)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr1_mask                                         (0x00000010)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr1(data)                                        (0x00000010&((data)<<4))
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr1_src(data)                                    ((0x00000010&(data))>>4)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_exe_mr1(data)                                    ((0x00000010&(data))>>4)
#define DC_PHY_DC_DDR3_CTL_write_en_1_shift                                          (3)
#define DC_PHY_DC_DDR3_CTL_write_en_1_mask                                           (0x00000008)
#define DC_PHY_DC_DDR3_CTL_write_en_1(data)                                          (0x00000008&((data)<<3))
#define DC_PHY_DC_DDR3_CTL_write_en_1_src(data)                                      ((0x00000008&(data))>>3)
#define DC_PHY_DC_DDR3_CTL_get_write_en_1(data)                                      ((0x00000008&(data))>>3)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr0_shift                                        (2)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr0_mask                                         (0x00000004)
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr0(data)                                        (0x00000004&((data)<<2))
#define DC_PHY_DC_DDR3_CTL_ddr3_exe_mr0_src(data)                                    ((0x00000004&(data))>>2)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_exe_mr0(data)                                    ((0x00000004&(data))>>2)
#define DC_PHY_DC_DDR3_CTL_write_en_0_shift                                          (1)
#define DC_PHY_DC_DDR3_CTL_write_en_0_mask                                           (0x00000002)
#define DC_PHY_DC_DDR3_CTL_write_en_0(data)                                          (0x00000002&((data)<<1))
#define DC_PHY_DC_DDR3_CTL_write_en_0_src(data)                                      ((0x00000002&(data))>>1)
#define DC_PHY_DC_DDR3_CTL_get_write_en_0(data)                                      ((0x00000002&(data))>>1)
#define DC_PHY_DC_DDR3_CTL_ddr3_init_shift                                           (0)
#define DC_PHY_DC_DDR3_CTL_ddr3_init_mask                                            (0x00000001)
#define DC_PHY_DC_DDR3_CTL_ddr3_init(data)                                           (0x00000001&((data)<<0))
#define DC_PHY_DC_DDR3_CTL_ddr3_init_src(data)                                       ((0x00000001&(data))>>0)
#define DC_PHY_DC_DDR3_CTL_get_ddr3_init(data)                                       ((0x00000001&(data))>>0)


#define DC_PHY_MOD23_REG                                                             0x18008844
#define DC_PHY_MOD23_REG_reg_addr                                                    "0xB8008844"
#define DC_PHY_MOD23_REG_reg                                                         0xB8008844
#define set_DC_PHY_MOD23_REG_reg(data)   (*((volatile unsigned int*) DC_PHY_MOD23_REG_reg)=data)
#define get_DC_PHY_MOD23_REG_reg   (*((volatile unsigned int*) DC_PHY_MOD23_REG_reg))
#define DC_PHY_MOD23_REG_inst_adr                                                    "0x0011"
#define DC_PHY_MOD23_REG_inst                                                        0x0011
#define DC_PHY_MOD23_REG_mod3_reg_shift                                              (16)
#define DC_PHY_MOD23_REG_mod3_reg_mask                                               (0xFFFF0000)
#define DC_PHY_MOD23_REG_mod3_reg(data)                                              (0xFFFF0000&((data)<<16))
#define DC_PHY_MOD23_REG_mod3_reg_src(data)                                          ((0xFFFF0000&(data))>>16)
#define DC_PHY_MOD23_REG_get_mod3_reg(data)                                          ((0xFFFF0000&(data))>>16)
#define DC_PHY_MOD23_REG_mod2_reg_shift                                              (0)
#define DC_PHY_MOD23_REG_mod2_reg_mask                                               (0x0000FFFF)
#define DC_PHY_MOD23_REG_mod2_reg(data)                                              (0x0000FFFF&((data)<<0))
#define DC_PHY_MOD23_REG_mod2_reg_src(data)                                          ((0x0000FFFF&(data))>>0)
#define DC_PHY_MOD23_REG_get_mod2_reg(data)                                          ((0x0000FFFF&(data))>>0)


#define DC_PHY_TMCTRL3                                                               0x18008848
#define DC_PHY_TMCTRL3_reg_addr                                                      "0xB8008848"
#define DC_PHY_TMCTRL3_reg                                                           0xB8008848
#define set_DC_PHY_TMCTRL3_reg(data)   (*((volatile unsigned int*) DC_PHY_TMCTRL3_reg)=data)
#define get_DC_PHY_TMCTRL3_reg   (*((volatile unsigned int*) DC_PHY_TMCTRL3_reg))
#define DC_PHY_TMCTRL3_inst_adr                                                      "0x0012"
#define DC_PHY_TMCTRL3_inst                                                          0x0012
#define DC_PHY_TMCTRL3_tmccd_shift                                                   (10)
#define DC_PHY_TMCTRL3_tmccd_mask                                                    (0x00001C00)
#define DC_PHY_TMCTRL3_tmccd(data)                                                   (0x00001C00&((data)<<10))
#define DC_PHY_TMCTRL3_tmccd_src(data)                                               ((0x00001C00&(data))>>10)
#define DC_PHY_TMCTRL3_get_tmccd(data)                                               ((0x00001C00&(data))>>10)
#define DC_PHY_TMCTRL3_tmFAW_shift                                                   (4)
#define DC_PHY_TMCTRL3_tmFAW_mask                                                    (0x000003F0)
#define DC_PHY_TMCTRL3_tmFAW(data)                                                   (0x000003F0&((data)<<4))
#define DC_PHY_TMCTRL3_tmFAW_src(data)                                               ((0x000003F0&(data))>>4)
#define DC_PHY_TMCTRL3_get_tmFAW(data)                                               ((0x000003F0&(data))>>4)
#define DC_PHY_TMCTRL3_tmmod_shift                                                   (0)
#define DC_PHY_TMCTRL3_tmmod_mask                                                    (0x0000000F)
#define DC_PHY_TMCTRL3_tmmod(data)                                                   (0x0000000F&((data)<<0))
#define DC_PHY_TMCTRL3_tmmod_src(data)                                               ((0x0000000F&(data))>>0)
#define DC_PHY_TMCTRL3_get_tmmod(data)                                               ((0x0000000F&(data))>>0)


#define DC_PHY_RFIFO_CTL                                                             0x1800884C
#define DC_PHY_RFIFO_CTL_reg_addr                                                    "0xB800884C"
#define DC_PHY_RFIFO_CTL_reg                                                         0xB800884C
#define set_DC_PHY_RFIFO_CTL_reg(data)   (*((volatile unsigned int*) DC_PHY_RFIFO_CTL_reg)=data)
#define get_DC_PHY_RFIFO_CTL_reg   (*((volatile unsigned int*) DC_PHY_RFIFO_CTL_reg))
#define DC_PHY_RFIFO_CTL_inst_adr                                                    "0x0013"
#define DC_PHY_RFIFO_CTL_inst                                                        0x0013
#define DC_PHY_RFIFO_CTL_wfifo_rdy_gated_shift                                       (12)
#define DC_PHY_RFIFO_CTL_wfifo_rdy_gated_mask                                        (0x00001000)
#define DC_PHY_RFIFO_CTL_wfifo_rdy_gated(data)                                       (0x00001000&((data)<<12))
#define DC_PHY_RFIFO_CTL_wfifo_rdy_gated_src(data)                                   ((0x00001000&(data))>>12)
#define DC_PHY_RFIFO_CTL_get_wfifo_rdy_gated(data)                                   ((0x00001000&(data))>>12)
#define DC_PHY_RFIFO_CTL_qfifo_dly_2T_shift                                          (9)
#define DC_PHY_RFIFO_CTL_qfifo_dly_2T_mask                                           (0x00000E00)
#define DC_PHY_RFIFO_CTL_qfifo_dly_2T(data)                                          (0x00000E00&((data)<<9))
#define DC_PHY_RFIFO_CTL_qfifo_dly_2T_src(data)                                      ((0x00000E00&(data))>>9)
#define DC_PHY_RFIFO_CTL_get_qfifo_dly_2T(data)                                      ((0x00000E00&(data))>>9)
#define DC_PHY_RFIFO_CTL_rdwr_use_full_shift                                         (8)
#define DC_PHY_RFIFO_CTL_rdwr_use_full_mask                                          (0x00000100)
#define DC_PHY_RFIFO_CTL_rdwr_use_full(data)                                         (0x00000100&((data)<<8))
#define DC_PHY_RFIFO_CTL_rdwr_use_full_src(data)                                     ((0x00000100&(data))>>8)
#define DC_PHY_RFIFO_CTL_get_rdwr_use_full(data)                                     ((0x00000100&(data))>>8)
#define DC_PHY_RFIFO_CTL_full_gated_shift                                            (7)
#define DC_PHY_RFIFO_CTL_full_gated_mask                                             (0x00000080)
#define DC_PHY_RFIFO_CTL_full_gated(data)                                            (0x00000080&((data)<<7))
#define DC_PHY_RFIFO_CTL_full_gated_src(data)                                        ((0x00000080&(data))>>7)
#define DC_PHY_RFIFO_CTL_get_full_gated(data)                                        ((0x00000080&(data))>>7)
#define DC_PHY_RFIFO_CTL_rfifo_thred_shift                                           (0)
#define DC_PHY_RFIFO_CTL_rfifo_thred_mask                                            (0x0000007F)
#define DC_PHY_RFIFO_CTL_rfifo_thred(data)                                           (0x0000007F&((data)<<0))
#define DC_PHY_RFIFO_CTL_rfifo_thred_src(data)                                       ((0x0000007F&(data))>>0)
#define DC_PHY_RFIFO_CTL_get_rfifo_thred(data)                                       ((0x0000007F&(data))>>0)


#define DC_PHY_ERR_STATUS                                                            0x18008850
#define DC_PHY_ERR_STATUS_reg_addr                                                   "0xB8008850"
#define DC_PHY_ERR_STATUS_reg                                                        0xB8008850
#define set_DC_PHY_ERR_STATUS_reg(data)   (*((volatile unsigned int*) DC_PHY_ERR_STATUS_reg)=data)
#define get_DC_PHY_ERR_STATUS_reg   (*((volatile unsigned int*) DC_PHY_ERR_STATUS_reg))
#define DC_PHY_ERR_STATUS_inst_adr                                                   "0x0014"
#define DC_PHY_ERR_STATUS_inst                                                       0x0014
#define DC_PHY_ERR_STATUS_err_cti_cmd_shift                                          (1)
#define DC_PHY_ERR_STATUS_err_cti_cmd_mask                                           (0x00000002)
#define DC_PHY_ERR_STATUS_err_cti_cmd(data)                                          (0x00000002&((data)<<1))
#define DC_PHY_ERR_STATUS_err_cti_cmd_src(data)                                      ((0x00000002&(data))>>1)
#define DC_PHY_ERR_STATUS_get_err_cti_cmd(data)                                      ((0x00000002&(data))>>1)
#define DC_PHY_ERR_STATUS_err_active_shift                                           (0)
#define DC_PHY_ERR_STATUS_err_active_mask                                            (0x00000001)
#define DC_PHY_ERR_STATUS_err_active(data)                                           (0x00000001&((data)<<0))
#define DC_PHY_ERR_STATUS_err_active_src(data)                                       ((0x00000001&(data))>>0)
#define DC_PHY_ERR_STATUS_get_err_active(data)                                       ((0x00000001&(data))>>0)


#define DC_PHY_DC_WRITE_LEVEL                                                        0x18008854
#define DC_PHY_DC_WRITE_LEVEL_reg_addr                                               "0xB8008854"
#define DC_PHY_DC_WRITE_LEVEL_reg                                                    0xB8008854
#define set_DC_PHY_DC_WRITE_LEVEL_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_WRITE_LEVEL_reg)=data)
#define get_DC_PHY_DC_WRITE_LEVEL_reg   (*((volatile unsigned int*) DC_PHY_DC_WRITE_LEVEL_reg))
#define DC_PHY_DC_WRITE_LEVEL_inst_adr                                               "0x0015"
#define DC_PHY_DC_WRITE_LEVEL_inst                                                   0x0015
#define DC_PHY_DC_WRITE_LEVEL_exe_prea_shift                                         (3)
#define DC_PHY_DC_WRITE_LEVEL_exe_prea_mask                                          (0x00000008)
#define DC_PHY_DC_WRITE_LEVEL_exe_prea(data)                                         (0x00000008&((data)<<3))
#define DC_PHY_DC_WRITE_LEVEL_exe_prea_src(data)                                     ((0x00000008&(data))>>3)
#define DC_PHY_DC_WRITE_LEVEL_get_exe_prea(data)                                     ((0x00000008&(data))>>3)
#define DC_PHY_DC_WRITE_LEVEL_dis_refresh_shift                                      (2)
#define DC_PHY_DC_WRITE_LEVEL_dis_refresh_mask                                       (0x00000004)
#define DC_PHY_DC_WRITE_LEVEL_dis_refresh(data)                                      (0x00000004&((data)<<2))
#define DC_PHY_DC_WRITE_LEVEL_dis_refresh_src(data)                                  ((0x00000004&(data))>>2)
#define DC_PHY_DC_WRITE_LEVEL_get_dis_refresh(data)                                  ((0x00000004&(data))>>2)
#define DC_PHY_DC_WRITE_LEVEL_trig_wr_level_shift                                    (1)
#define DC_PHY_DC_WRITE_LEVEL_trig_wr_level_mask                                     (0x00000002)
#define DC_PHY_DC_WRITE_LEVEL_trig_wr_level(data)                                    (0x00000002&((data)<<1))
#define DC_PHY_DC_WRITE_LEVEL_trig_wr_level_src(data)                                ((0x00000002&(data))>>1)
#define DC_PHY_DC_WRITE_LEVEL_get_trig_wr_level(data)                                ((0x00000002&(data))>>1)
#define DC_PHY_DC_WRITE_LEVEL_wr_level_rdy_shift                                     (0)
#define DC_PHY_DC_WRITE_LEVEL_wr_level_rdy_mask                                      (0x00000001)
#define DC_PHY_DC_WRITE_LEVEL_wr_level_rdy(data)                                     (0x00000001&((data)<<0))
#define DC_PHY_DC_WRITE_LEVEL_wr_level_rdy_src(data)                                 ((0x00000001&(data))>>0)
#define DC_PHY_DC_WRITE_LEVEL_get_wr_level_rdy(data)                                 ((0x00000001&(data))>>0)


#define DC_PHY_INT_ENABLE                                                            0x18008858
#define DC_PHY_INT_ENABLE_reg_addr                                                   "0xB8008858"
#define DC_PHY_INT_ENABLE_reg                                                        0xB8008858
#define set_DC_PHY_INT_ENABLE_reg(data)   (*((volatile unsigned int*) DC_PHY_INT_ENABLE_reg)=data)
#define get_DC_PHY_INT_ENABLE_reg   (*((volatile unsigned int*) DC_PHY_INT_ENABLE_reg))
#define DC_PHY_INT_ENABLE_inst_adr                                                   "0x0016"
#define DC_PHY_INT_ENABLE_inst                                                       0x0016
#define DC_PHY_INT_ENABLE_int_err_cticmd_en_shift                                    (4)
#define DC_PHY_INT_ENABLE_int_err_cticmd_en_mask                                     (0x00000010)
#define DC_PHY_INT_ENABLE_int_err_cticmd_en(data)                                    (0x00000010&((data)<<4))
#define DC_PHY_INT_ENABLE_int_err_cticmd_en_src(data)                                ((0x00000010&(data))>>4)
#define DC_PHY_INT_ENABLE_get_int_err_cticmd_en(data)                                ((0x00000010&(data))>>4)
#define DC_PHY_INT_ENABLE_int_err_active_en_shift                                    (3)
#define DC_PHY_INT_ENABLE_int_err_active_en_mask                                     (0x00000008)
#define DC_PHY_INT_ENABLE_int_err_active_en(data)                                    (0x00000008&((data)<<3))
#define DC_PHY_INT_ENABLE_int_err_active_en_src(data)                                ((0x00000008&(data))>>3)
#define DC_PHY_INT_ENABLE_get_int_err_active_en(data)                                ((0x00000008&(data))>>3)
#define DC_PHY_INT_ENABLE_int_vcpu_en_shift                                          (2)
#define DC_PHY_INT_ENABLE_int_vcpu_en_mask                                           (0x00000004)
#define DC_PHY_INT_ENABLE_int_vcpu_en(data)                                          (0x00000004&((data)<<2))
#define DC_PHY_INT_ENABLE_int_vcpu_en_src(data)                                      ((0x00000004&(data))>>2)
#define DC_PHY_INT_ENABLE_get_int_vcpu_en(data)                                      ((0x00000004&(data))>>2)
#define DC_PHY_INT_ENABLE_int_acpu_en_shift                                          (1)
#define DC_PHY_INT_ENABLE_int_acpu_en_mask                                           (0x00000002)
#define DC_PHY_INT_ENABLE_int_acpu_en(data)                                          (0x00000002&((data)<<1))
#define DC_PHY_INT_ENABLE_int_acpu_en_src(data)                                      ((0x00000002&(data))>>1)
#define DC_PHY_INT_ENABLE_get_int_acpu_en(data)                                      ((0x00000002&(data))>>1)
#define DC_PHY_INT_ENABLE_int_scpu_en_shift                                          (0)
#define DC_PHY_INT_ENABLE_int_scpu_en_mask                                           (0x00000001)
#define DC_PHY_INT_ENABLE_int_scpu_en(data)                                          (0x00000001&((data)<<0))
#define DC_PHY_INT_ENABLE_int_scpu_en_src(data)                                      ((0x00000001&(data))>>0)
#define DC_PHY_INT_ENABLE_get_int_scpu_en(data)                                      ((0x00000001&(data))>>0)


#define DC_PHY_RFIFO_MAXCNT                                                          0x1800885C
#define DC_PHY_RFIFO_MAXCNT_reg_addr                                                 "0xB800885C"
#define DC_PHY_RFIFO_MAXCNT_reg                                                      0xB800885C
#define set_DC_PHY_RFIFO_MAXCNT_reg(data)   (*((volatile unsigned int*) DC_PHY_RFIFO_MAXCNT_reg)=data)
#define get_DC_PHY_RFIFO_MAXCNT_reg   (*((volatile unsigned int*) DC_PHY_RFIFO_MAXCNT_reg))
#define DC_PHY_RFIFO_MAXCNT_inst_adr                                                 "0x0017"
#define DC_PHY_RFIFO_MAXCNT_inst                                                     0x0017
#define DC_PHY_RFIFO_MAXCNT_non_valid_maxcnt_shift                                   (7)
#define DC_PHY_RFIFO_MAXCNT_non_valid_maxcnt_mask                                    (0x00000080)
#define DC_PHY_RFIFO_MAXCNT_non_valid_maxcnt(data)                                   (0x00000080&((data)<<7))
#define DC_PHY_RFIFO_MAXCNT_non_valid_maxcnt_src(data)                               ((0x00000080&(data))>>7)
#define DC_PHY_RFIFO_MAXCNT_get_non_valid_maxcnt(data)                               ((0x00000080&(data))>>7)
#define DC_PHY_RFIFO_MAXCNT_rfifo_cnt_max_shift                                      (0)
#define DC_PHY_RFIFO_MAXCNT_rfifo_cnt_max_mask                                       (0x0000007F)
#define DC_PHY_RFIFO_MAXCNT_rfifo_cnt_max(data)                                      (0x0000007F&((data)<<0))
#define DC_PHY_RFIFO_MAXCNT_rfifo_cnt_max_src(data)                                  ((0x0000007F&(data))>>0)
#define DC_PHY_RFIFO_MAXCNT_get_rfifo_cnt_max(data)                                  ((0x0000007F&(data))>>0)


#define DC_PHY_SM_STATUS                                                             0x18008860
#define DC_PHY_SM_STATUS_reg_addr                                                    "0xB8008860"
#define DC_PHY_SM_STATUS_reg                                                         0xB8008860
#define set_DC_PHY_SM_STATUS_reg(data)   (*((volatile unsigned int*) DC_PHY_SM_STATUS_reg)=data)
#define get_DC_PHY_SM_STATUS_reg   (*((volatile unsigned int*) DC_PHY_SM_STATUS_reg))
#define DC_PHY_SM_STATUS_inst_adr                                                    "0x0018"
#define DC_PHY_SM_STATUS_inst                                                        0x0018
#define DC_PHY_SM_STATUS_bank_act_shift                                              (13)
#define DC_PHY_SM_STATUS_bank_act_mask                                               (0x001FE000)
#define DC_PHY_SM_STATUS_bank_act(data)                                              (0x001FE000&((data)<<13))
#define DC_PHY_SM_STATUS_bank_act_src(data)                                          ((0x001FE000&(data))>>13)
#define DC_PHY_SM_STATUS_get_bank_act(data)                                          ((0x001FE000&(data))>>13)
#define DC_PHY_SM_STATUS_cmd_cnt_shift                                               (10)
#define DC_PHY_SM_STATUS_cmd_cnt_mask                                                (0x00001C00)
#define DC_PHY_SM_STATUS_cmd_cnt(data)                                               (0x00001C00&((data)<<10))
#define DC_PHY_SM_STATUS_cmd_cnt_src(data)                                           ((0x00001C00&(data))>>10)
#define DC_PHY_SM_STATUS_get_cmd_cnt(data)                                           ((0x00001C00&(data))>>10)
#define DC_PHY_SM_STATUS_par_st_shift                                                (0)
#define DC_PHY_SM_STATUS_par_st_mask                                                 (0x000003FF)
#define DC_PHY_SM_STATUS_par_st(data)                                                (0x000003FF&((data)<<0))
#define DC_PHY_SM_STATUS_par_st_src(data)                                            ((0x000003FF&(data))>>0)
#define DC_PHY_SM_STATUS_get_par_st(data)                                            ((0x000003FF&(data))>>0)


#define DC_PHY_WFIFO_CNT                                                             0x18008864
#define DC_PHY_WFIFO_CNT_reg_addr                                                    "0xB8008864"
#define DC_PHY_WFIFO_CNT_reg                                                         0xB8008864
#define set_DC_PHY_WFIFO_CNT_reg(data)   (*((volatile unsigned int*) DC_PHY_WFIFO_CNT_reg)=data)
#define get_DC_PHY_WFIFO_CNT_reg   (*((volatile unsigned int*) DC_PHY_WFIFO_CNT_reg))
#define DC_PHY_WFIFO_CNT_inst_adr                                                    "0x0019"
#define DC_PHY_WFIFO_CNT_inst                                                        0x0019
#define DC_PHY_WFIFO_CNT_wfifo_sys_req_statistic_en_shift                            (31)
#define DC_PHY_WFIFO_CNT_wfifo_sys_req_statistic_en_mask                             (0x80000000)
#define DC_PHY_WFIFO_CNT_wfifo_sys_req_statistic_en(data)                            (0x80000000&((data)<<31))
#define DC_PHY_WFIFO_CNT_wfifo_sys_req_statistic_en_src(data)                        ((0x80000000&(data))>>31)
#define DC_PHY_WFIFO_CNT_get_wfifo_sys_req_statistic_en(data)                        ((0x80000000&(data))>>31)
#define DC_PHY_WFIFO_CNT_wfifo_ddr_req_statistic_en_shift                            (30)
#define DC_PHY_WFIFO_CNT_wfifo_ddr_req_statistic_en_mask                             (0x40000000)
#define DC_PHY_WFIFO_CNT_wfifo_ddr_req_statistic_en(data)                            (0x40000000&((data)<<30))
#define DC_PHY_WFIFO_CNT_wfifo_ddr_req_statistic_en_src(data)                        ((0x40000000&(data))>>30)
#define DC_PHY_WFIFO_CNT_get_wfifo_ddr_req_statistic_en(data)                        ((0x40000000&(data))>>30)
#define DC_PHY_WFIFO_CNT_clear_wfifo_record_shift                                    (29)
#define DC_PHY_WFIFO_CNT_clear_wfifo_record_mask                                     (0x20000000)
#define DC_PHY_WFIFO_CNT_clear_wfifo_record(data)                                    (0x20000000&((data)<<29))
#define DC_PHY_WFIFO_CNT_clear_wfifo_record_src(data)                                ((0x20000000&(data))>>29)
#define DC_PHY_WFIFO_CNT_get_clear_wfifo_record(data)                                ((0x20000000&(data))>>29)
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_et_shift                                       (16)
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_et_mask                                        (0x00FF0000)
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_et(data)                                       (0x00FF0000&((data)<<16))
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_et_src(data)                                   ((0x00FF0000&(data))>>16)
#define DC_PHY_WFIFO_CNT_get_wfifo_rd_cnt_et(data)                                   ((0x00FF0000&(data))>>16)
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_rl_shift                                       (8)
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_rl_mask                                        (0x0000FF00)
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_rl(data)                                       (0x0000FF00&((data)<<8))
#define DC_PHY_WFIFO_CNT_wfifo_rd_cnt_rl_src(data)                                   ((0x0000FF00&(data))>>8)
#define DC_PHY_WFIFO_CNT_get_wfifo_rd_cnt_rl(data)                                   ((0x0000FF00&(data))>>8)
#define DC_PHY_WFIFO_CNT_wfifo_wr_cnt_rl_shift                                       (0)
#define DC_PHY_WFIFO_CNT_wfifo_wr_cnt_rl_mask                                        (0x000000FF)
#define DC_PHY_WFIFO_CNT_wfifo_wr_cnt_rl(data)                                       (0x000000FF&((data)<<0))
#define DC_PHY_WFIFO_CNT_wfifo_wr_cnt_rl_src(data)                                   ((0x000000FF&(data))>>0)
#define DC_PHY_WFIFO_CNT_get_wfifo_wr_cnt_rl(data)                                   ((0x000000FF&(data))>>0)


#define DC_PHY_DC_DEBUG                                                              0x18008868
#define DC_PHY_DC_DEBUG_reg_addr                                                     "0xB8008868"
#define DC_PHY_DC_DEBUG_reg                                                          0xB8008868
#define set_DC_PHY_DC_DEBUG_reg(data)   (*((volatile unsigned int*) DC_PHY_DC_DEBUG_reg)=data)
#define get_DC_PHY_DC_DEBUG_reg   (*((volatile unsigned int*) DC_PHY_DC_DEBUG_reg))
#define DC_PHY_DC_DEBUG_inst_adr                                                     "0x001A"
#define DC_PHY_DC_DEBUG_inst                                                         0x001A
#define DC_PHY_DC_DEBUG_dbg_out_mod_shift                                            (20)
#define DC_PHY_DC_DEBUG_dbg_out_mod_mask                                             (0x00700000)
#define DC_PHY_DC_DEBUG_dbg_out_mod(data)                                            (0x00700000&((data)<<20))
#define DC_PHY_DC_DEBUG_dbg_out_mod_src(data)                                        ((0x00700000&(data))>>20)
#define DC_PHY_DC_DEBUG_get_dbg_out_mod(data)                                        ((0x00700000&(data))>>20)
#define DC_PHY_DC_DEBUG_pad_tie_value_shift                                          (16)
#define DC_PHY_DC_DEBUG_pad_tie_value_mask                                           (0x000F0000)
#define DC_PHY_DC_DEBUG_pad_tie_value(data)                                          (0x000F0000&((data)<<16))
#define DC_PHY_DC_DEBUG_pad_tie_value_src(data)                                      ((0x000F0000&(data))>>16)
#define DC_PHY_DC_DEBUG_get_pad_tie_value(data)                                      ((0x000F0000&(data))>>16)
#define DC_PHY_DC_DEBUG_shift_trig_mode_shift                                        (9)
#define DC_PHY_DC_DEBUG_shift_trig_mode_mask                                         (0x0000FE00)
#define DC_PHY_DC_DEBUG_shift_trig_mode(data)                                        (0x0000FE00&((data)<<9))
#define DC_PHY_DC_DEBUG_shift_trig_mode_src(data)                                    ((0x0000FE00&(data))>>9)
#define DC_PHY_DC_DEBUG_get_shift_trig_mode(data)                                    ((0x0000FE00&(data))>>9)
#define DC_PHY_DC_DEBUG_rd_dbg_mod_shift                                             (6)
#define DC_PHY_DC_DEBUG_rd_dbg_mod_mask                                              (0x000001C0)
#define DC_PHY_DC_DEBUG_rd_dbg_mod(data)                                             (0x000001C0&((data)<<6))
#define DC_PHY_DC_DEBUG_rd_dbg_mod_src(data)                                         ((0x000001C0&(data))>>6)
#define DC_PHY_DC_DEBUG_get_rd_dbg_mod(data)                                         ((0x000001C0&(data))>>6)
#define DC_PHY_DC_DEBUG_dbg_mod_shift                                                (0)
#define DC_PHY_DC_DEBUG_dbg_mod_mask                                                 (0x0000003F)
#define DC_PHY_DC_DEBUG_dbg_mod(data)                                                (0x0000003F&((data)<<0))
#define DC_PHY_DC_DEBUG_dbg_mod_src(data)                                            ((0x0000003F&(data))>>0)
#define DC_PHY_DC_DEBUG_get_dbg_mod(data)                                            ((0x0000003F&(data))>>0)


#define DC_PHY_PRE_CNT                                                               0x1800886C
#define DC_PHY_PRE_CNT_reg_addr                                                      "0xB800886C"
#define DC_PHY_PRE_CNT_reg                                                           0xB800886C
#define set_DC_PHY_PRE_CNT_reg(data)   (*((volatile unsigned int*) DC_PHY_PRE_CNT_reg)=data)
#define get_DC_PHY_PRE_CNT_reg   (*((volatile unsigned int*) DC_PHY_PRE_CNT_reg))
#define DC_PHY_PRE_CNT_inst_adr                                                      "0x001B"
#define DC_PHY_PRE_CNT_inst                                                          0x001B
#define DC_PHY_PRE_CNT_post_trig_cnt_shift                                           (16)
#define DC_PHY_PRE_CNT_post_trig_cnt_mask                                            (0xFFFF0000)
#define DC_PHY_PRE_CNT_post_trig_cnt(data)                                           (0xFFFF0000&((data)<<16))
#define DC_PHY_PRE_CNT_post_trig_cnt_src(data)                                       ((0xFFFF0000&(data))>>16)
#define DC_PHY_PRE_CNT_get_post_trig_cnt(data)                                       ((0xFFFF0000&(data))>>16)
#define DC_PHY_PRE_CNT_pre_trig_cnt_shift                                            (0)
#define DC_PHY_PRE_CNT_pre_trig_cnt_mask                                             (0x0000FFFF)
#define DC_PHY_PRE_CNT_pre_trig_cnt(data)                                            (0x0000FFFF&((data)<<0))
#define DC_PHY_PRE_CNT_pre_trig_cnt_src(data)                                        ((0x0000FFFF&(data))>>0)
#define DC_PHY_PRE_CNT_get_pre_trig_cnt(data)                                        ((0x0000FFFF&(data))>>0)


#define DC_PHY_WFIFO_SYS_REQ_WAIT                                                    0x18008870
#define DC_PHY_WFIFO_SYS_REQ_WAIT_reg_addr                                           "0xB8008870"
#define DC_PHY_WFIFO_SYS_REQ_WAIT_reg                                                0xB8008870
#define set_DC_PHY_WFIFO_SYS_REQ_WAIT_reg(data)   (*((volatile unsigned int*) DC_PHY_WFIFO_SYS_REQ_WAIT_reg)=data)
#define get_DC_PHY_WFIFO_SYS_REQ_WAIT_reg   (*((volatile unsigned int*) DC_PHY_WFIFO_SYS_REQ_WAIT_reg))
#define DC_PHY_WFIFO_SYS_REQ_WAIT_inst_adr                                           "0x001C"
#define DC_PHY_WFIFO_SYS_REQ_WAIT_inst                                               0x001C
#define DC_PHY_WFIFO_SYS_REQ_WAIT_wfifo_sys_req_wait_total_shift                     (0)
#define DC_PHY_WFIFO_SYS_REQ_WAIT_wfifo_sys_req_wait_total_mask                      (0xFFFFFFFF)
#define DC_PHY_WFIFO_SYS_REQ_WAIT_wfifo_sys_req_wait_total(data)                     (0xFFFFFFFF&((data)<<0))
#define DC_PHY_WFIFO_SYS_REQ_WAIT_wfifo_sys_req_wait_total_src(data)                 ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_WFIFO_SYS_REQ_WAIT_get_wfifo_sys_req_wait_total(data)                 ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_WFIFO_DDR_REQ_WAIT                                                    0x18008874
#define DC_PHY_WFIFO_DDR_REQ_WAIT_reg_addr                                           "0xB8008874"
#define DC_PHY_WFIFO_DDR_REQ_WAIT_reg                                                0xB8008874
#define set_DC_PHY_WFIFO_DDR_REQ_WAIT_reg(data)   (*((volatile unsigned int*) DC_PHY_WFIFO_DDR_REQ_WAIT_reg)=data)
#define get_DC_PHY_WFIFO_DDR_REQ_WAIT_reg   (*((volatile unsigned int*) DC_PHY_WFIFO_DDR_REQ_WAIT_reg))
#define DC_PHY_WFIFO_DDR_REQ_WAIT_inst_adr                                           "0x001D"
#define DC_PHY_WFIFO_DDR_REQ_WAIT_inst                                               0x001D
#define DC_PHY_WFIFO_DDR_REQ_WAIT_wfifo_ddr_req_wait_total_shift                     (0)
#define DC_PHY_WFIFO_DDR_REQ_WAIT_wfifo_ddr_req_wait_total_mask                      (0xFFFFFFFF)
#define DC_PHY_WFIFO_DDR_REQ_WAIT_wfifo_ddr_req_wait_total(data)                     (0xFFFFFFFF&((data)<<0))
#define DC_PHY_WFIFO_DDR_REQ_WAIT_wfifo_ddr_req_wait_total_src(data)                 ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_WFIFO_DDR_REQ_WAIT_get_wfifo_ddr_req_wait_total(data)                 ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_WFIFO_MAX_REQ_WAIT                                                    0x18008878
#define DC_PHY_WFIFO_MAX_REQ_WAIT_reg_addr                                           "0xB8008878"
#define DC_PHY_WFIFO_MAX_REQ_WAIT_reg                                                0xB8008878
#define set_DC_PHY_WFIFO_MAX_REQ_WAIT_reg(data)   (*((volatile unsigned int*) DC_PHY_WFIFO_MAX_REQ_WAIT_reg)=data)
#define get_DC_PHY_WFIFO_MAX_REQ_WAIT_reg   (*((volatile unsigned int*) DC_PHY_WFIFO_MAX_REQ_WAIT_reg))
#define DC_PHY_WFIFO_MAX_REQ_WAIT_inst_adr                                           "0x001E"
#define DC_PHY_WFIFO_MAX_REQ_WAIT_inst                                               0x001E
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_ddr_req_wait_max_shift                       (16)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_ddr_req_wait_max_mask                        (0xFFFF0000)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_ddr_req_wait_max(data)                       (0xFFFF0000&((data)<<16))
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_ddr_req_wait_max_src(data)                   ((0xFFFF0000&(data))>>16)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_get_wfifo_ddr_req_wait_max(data)                   ((0xFFFF0000&(data))>>16)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_sys_req_wait_max_shift                       (0)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_sys_req_wait_max_mask                        (0x0000FFFF)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_sys_req_wait_max(data)                       (0x0000FFFF&((data)<<0))
#define DC_PHY_WFIFO_MAX_REQ_WAIT_wfifo_sys_req_wait_max_src(data)                   ((0x0000FFFF&(data))>>0)
#define DC_PHY_WFIFO_MAX_REQ_WAIT_get_wfifo_sys_req_wait_max(data)                   ((0x0000FFFF&(data))>>0)


#define DC_PHY_DQ0_1_DLYN                                                            0x18008880
#define DC_PHY_DQ0_1_DLYN_reg_addr                                                   "0xB8008880"
#define DC_PHY_DQ0_1_DLYN_reg                                                        0xB8008880
#define set_DC_PHY_DQ0_1_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ0_1_DLYN_reg)=data)
#define get_DC_PHY_DQ0_1_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ0_1_DLYN_reg))
#define DC_PHY_DQ0_1_DLYN_inst_adr                                                   "0x0020"
#define DC_PHY_DQ0_1_DLYN_inst                                                       0x0020
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_1_0_shift                                    (22)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_1_0_mask                                     (0x0FC00000)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_1_0(data)                                    (0x0FC00000&((data)<<22))
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_1_0_src(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ0_1_DLYN_get_ph_sel_dq_neg_1_0(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_1_0_shift                                    (16)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_1_0_mask                                     (0x003F0000)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_1_0(data)                                    (0x003F0000&((data)<<16))
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_1_0_src(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ0_1_DLYN_get_ph_sel_dq_pos_1_0(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_0_0_shift                                    (6)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_0_0_mask                                     (0x00000FC0)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_0_0(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_neg_0_0_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ0_1_DLYN_get_ph_sel_dq_neg_0_0(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_0_0_shift                                    (0)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_0_0_mask                                     (0x0000003F)
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_0_0(data)                                    (0x0000003F&((data)<<0))
#define DC_PHY_DQ0_1_DLYN_ph_sel_dq_pos_0_0_src(data)                                ((0x0000003F&(data))>>0)
#define DC_PHY_DQ0_1_DLYN_get_ph_sel_dq_pos_0_0(data)                                ((0x0000003F&(data))>>0)


#define DC_PHY_DQ2_3_DLYN                                                            0x18008884
#define DC_PHY_DQ2_3_DLYN_reg_addr                                                   "0xB8008884"
#define DC_PHY_DQ2_3_DLYN_reg                                                        0xB8008884
#define set_DC_PHY_DQ2_3_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ2_3_DLYN_reg)=data)
#define get_DC_PHY_DQ2_3_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ2_3_DLYN_reg))
#define DC_PHY_DQ2_3_DLYN_inst_adr                                                   "0x0021"
#define DC_PHY_DQ2_3_DLYN_inst                                                       0x0021
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_3_0_shift                                    (22)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_3_0_mask                                     (0x0FC00000)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_3_0(data)                                    (0x0FC00000&((data)<<22))
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_3_0_src(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ2_3_DLYN_get_ph_sel_dq_neg_3_0(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_3_0_shift                                    (16)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_3_0_mask                                     (0x003F0000)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_3_0(data)                                    (0x003F0000&((data)<<16))
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_3_0_src(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ2_3_DLYN_get_ph_sel_dq_pos_3_0(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_2_0_shift                                    (6)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_2_0_mask                                     (0x00000FC0)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_2_0(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_neg_2_0_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ2_3_DLYN_get_ph_sel_dq_neg_2_0(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_2_0_shift                                    (0)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_2_0_mask                                     (0x0000003F)
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_2_0(data)                                    (0x0000003F&((data)<<0))
#define DC_PHY_DQ2_3_DLYN_ph_sel_dq_pos_2_0_src(data)                                ((0x0000003F&(data))>>0)
#define DC_PHY_DQ2_3_DLYN_get_ph_sel_dq_pos_2_0(data)                                ((0x0000003F&(data))>>0)


#define DC_PHY_DQ4_5_DLYN                                                            0x18008888
#define DC_PHY_DQ4_5_DLYN_reg_addr                                                   "0xB8008888"
#define DC_PHY_DQ4_5_DLYN_reg                                                        0xB8008888
#define set_DC_PHY_DQ4_5_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ4_5_DLYN_reg)=data)
#define get_DC_PHY_DQ4_5_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ4_5_DLYN_reg))
#define DC_PHY_DQ4_5_DLYN_inst_adr                                                   "0x0022"
#define DC_PHY_DQ4_5_DLYN_inst                                                       0x0022
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_5_0_shift                                    (22)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_5_0_mask                                     (0x0FC00000)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_5_0(data)                                    (0x0FC00000&((data)<<22))
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_5_0_src(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ4_5_DLYN_get_ph_sel_dq_neg_5_0(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_5_0_shift                                    (16)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_5_0_mask                                     (0x003F0000)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_5_0(data)                                    (0x003F0000&((data)<<16))
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_5_0_src(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ4_5_DLYN_get_ph_sel_dq_pos_5_0(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_4_0_shift                                    (6)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_4_0_mask                                     (0x00000FC0)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_4_0(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_neg_4_0_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ4_5_DLYN_get_ph_sel_dq_neg_4_0(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_4_0_shift                                    (0)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_4_0_mask                                     (0x0000003F)
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_4_0(data)                                    (0x0000003F&((data)<<0))
#define DC_PHY_DQ4_5_DLYN_ph_sel_dq_pos_4_0_src(data)                                ((0x0000003F&(data))>>0)
#define DC_PHY_DQ4_5_DLYN_get_ph_sel_dq_pos_4_0(data)                                ((0x0000003F&(data))>>0)


#define DC_PHY_DQ6_7_DLYN                                                            0x1800888C
#define DC_PHY_DQ6_7_DLYN_reg_addr                                                   "0xB800888C"
#define DC_PHY_DQ6_7_DLYN_reg                                                        0xB800888C
#define set_DC_PHY_DQ6_7_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ6_7_DLYN_reg)=data)
#define get_DC_PHY_DQ6_7_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ6_7_DLYN_reg))
#define DC_PHY_DQ6_7_DLYN_inst_adr                                                   "0x0023"
#define DC_PHY_DQ6_7_DLYN_inst                                                       0x0023
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_7_0_shift                                    (22)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_7_0_mask                                     (0x0FC00000)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_7_0(data)                                    (0x0FC00000&((data)<<22))
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_7_0_src(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ6_7_DLYN_get_ph_sel_dq_neg_7_0(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_7_0_shift                                    (16)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_7_0_mask                                     (0x003F0000)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_7_0(data)                                    (0x003F0000&((data)<<16))
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_7_0_src(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ6_7_DLYN_get_ph_sel_dq_pos_7_0(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_6_0_shift                                    (6)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_6_0_mask                                     (0x00000FC0)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_6_0(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_neg_6_0_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ6_7_DLYN_get_ph_sel_dq_neg_6_0(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_6_0_shift                                    (0)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_6_0_mask                                     (0x0000003F)
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_6_0(data)                                    (0x0000003F&((data)<<0))
#define DC_PHY_DQ6_7_DLYN_ph_sel_dq_pos_6_0_src(data)                                ((0x0000003F&(data))>>0)
#define DC_PHY_DQ6_7_DLYN_get_ph_sel_dq_pos_6_0(data)                                ((0x0000003F&(data))>>0)


#define DC_PHY_DQ8_9_DLYN                                                            0x18008890
#define DC_PHY_DQ8_9_DLYN_reg_addr                                                   "0xB8008890"
#define DC_PHY_DQ8_9_DLYN_reg                                                        0xB8008890
#define set_DC_PHY_DQ8_9_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ8_9_DLYN_reg)=data)
#define get_DC_PHY_DQ8_9_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ8_9_DLYN_reg))
#define DC_PHY_DQ8_9_DLYN_inst_adr                                                   "0x0024"
#define DC_PHY_DQ8_9_DLYN_inst                                                       0x0024
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_1_1_shift                                    (22)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_1_1_mask                                     (0x0FC00000)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_1_1(data)                                    (0x0FC00000&((data)<<22))
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_1_1_src(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ8_9_DLYN_get_ph_sel_dq_neg_1_1(data)                                ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_1_1_shift                                    (16)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_1_1_mask                                     (0x003F0000)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_1_1(data)                                    (0x003F0000&((data)<<16))
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_1_1_src(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ8_9_DLYN_get_ph_sel_dq_pos_1_1(data)                                ((0x003F0000&(data))>>16)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_0_1_shift                                    (6)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_0_1_mask                                     (0x00000FC0)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_0_1(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_neg_0_1_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ8_9_DLYN_get_ph_sel_dq_neg_0_1(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_0_1_shift                                    (0)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_0_1_mask                                     (0x0000003F)
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_0_1(data)                                    (0x0000003F&((data)<<0))
#define DC_PHY_DQ8_9_DLYN_ph_sel_dq_pos_0_1_src(data)                                ((0x0000003F&(data))>>0)
#define DC_PHY_DQ8_9_DLYN_get_ph_sel_dq_pos_0_1(data)                                ((0x0000003F&(data))>>0)


#define DC_PHY_DQ10_11_DLYN                                                          0x18008894
#define DC_PHY_DQ10_11_DLYN_reg_addr                                                 "0xB8008894"
#define DC_PHY_DQ10_11_DLYN_reg                                                      0xB8008894
#define set_DC_PHY_DQ10_11_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ10_11_DLYN_reg)=data)
#define get_DC_PHY_DQ10_11_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ10_11_DLYN_reg))
#define DC_PHY_DQ10_11_DLYN_inst_adr                                                 "0x0025"
#define DC_PHY_DQ10_11_DLYN_inst                                                     0x0025
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_3_1_shift                                  (22)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_3_1_mask                                   (0x0FC00000)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_3_1(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_3_1_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ10_11_DLYN_get_ph_sel_dq_neg_3_1(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_3_1_shift                                  (16)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_3_1_mask                                   (0x003F0000)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_3_1(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_3_1_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ10_11_DLYN_get_ph_sel_dq_pos_3_1(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_2_1_shift                                  (6)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_2_1_mask                                   (0x00000FC0)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_2_1(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_neg_2_1_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ10_11_DLYN_get_ph_sel_dq_neg_2_1(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_2_1_shift                                  (0)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_2_1_mask                                   (0x0000003F)
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_2_1(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ10_11_DLYN_ph_sel_dq_pos_2_1_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ10_11_DLYN_get_ph_sel_dq_pos_2_1(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ12_13_DLYN                                                          0x1800889C
#define DC_PHY_DQ12_13_DLYN_reg_addr                                                 "0xB800889C"
#define DC_PHY_DQ12_13_DLYN_reg                                                      0xB800889C
#define set_DC_PHY_DQ12_13_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ12_13_DLYN_reg)=data)
#define get_DC_PHY_DQ12_13_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ12_13_DLYN_reg))
#define DC_PHY_DQ12_13_DLYN_inst_adr                                                 "0x0027"
#define DC_PHY_DQ12_13_DLYN_inst                                                     0x0027
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_5_1_shift                                  (22)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_5_1_mask                                   (0x0FC00000)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_5_1(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_5_1_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ12_13_DLYN_get_ph_sel_dq_neg_5_1(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_5_1_shift                                  (16)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_5_1_mask                                   (0x003F0000)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_5_1(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_5_1_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ12_13_DLYN_get_ph_sel_dq_pos_5_1(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_4_1_shift                                  (6)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_4_1_mask                                   (0x00000FC0)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_4_1(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_neg_4_1_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ12_13_DLYN_get_ph_sel_dq_neg_4_1(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_4_1_shift                                  (0)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_4_1_mask                                   (0x0000003F)
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_4_1(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ12_13_DLYN_ph_sel_dq_pos_4_1_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ12_13_DLYN_get_ph_sel_dq_pos_4_1(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ14_15_DLYN                                                          0x180088A0
#define DC_PHY_DQ14_15_DLYN_reg_addr                                                 "0xB80088A0"
#define DC_PHY_DQ14_15_DLYN_reg                                                      0xB80088A0
#define set_DC_PHY_DQ14_15_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ14_15_DLYN_reg)=data)
#define get_DC_PHY_DQ14_15_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ14_15_DLYN_reg))
#define DC_PHY_DQ14_15_DLYN_inst_adr                                                 "0x0028"
#define DC_PHY_DQ14_15_DLYN_inst                                                     0x0028
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_7_1_shift                                  (22)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_7_1_mask                                   (0x0FC00000)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_7_1(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_7_1_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ14_15_DLYN_get_ph_sel_dq_neg_7_1(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_7_1_shift                                  (16)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_7_1_mask                                   (0x003F0000)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_7_1(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_7_1_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ14_15_DLYN_get_ph_sel_dq_pos_7_1(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_6_1_shift                                  (6)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_6_1_mask                                   (0x00000FC0)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_6_1(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_neg_6_1_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ14_15_DLYN_get_ph_sel_dq_neg_6_1(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_6_1_shift                                  (0)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_6_1_mask                                   (0x0000003F)
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_6_1(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ14_15_DLYN_ph_sel_dq_pos_6_1_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ14_15_DLYN_get_ph_sel_dq_pos_6_1(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ16_17_DLYN                                                          0x180088A4
#define DC_PHY_DQ16_17_DLYN_reg_addr                                                 "0xB80088A4"
#define DC_PHY_DQ16_17_DLYN_reg                                                      0xB80088A4
#define set_DC_PHY_DQ16_17_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ16_17_DLYN_reg)=data)
#define get_DC_PHY_DQ16_17_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ16_17_DLYN_reg))
#define DC_PHY_DQ16_17_DLYN_inst_adr                                                 "0x0029"
#define DC_PHY_DQ16_17_DLYN_inst                                                     0x0029
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_1_2_shift                                  (22)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_1_2_mask                                   (0x0FC00000)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_1_2(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_1_2_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ16_17_DLYN_get_ph_sel_dq_neg_1_2(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_1_2_shift                                  (16)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_1_2_mask                                   (0x003F0000)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_1_2(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_1_2_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ16_17_DLYN_get_ph_sel_dq_pos_1_2(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_0_2_shift                                  (6)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_0_2_mask                                   (0x00000FC0)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_0_2(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_neg_0_2_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ16_17_DLYN_get_ph_sel_dq_neg_0_2(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_0_2_shift                                  (0)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_0_2_mask                                   (0x0000003F)
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_0_2(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ16_17_DLYN_ph_sel_dq_pos_0_2_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ16_17_DLYN_get_ph_sel_dq_pos_0_2(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ18_19_DLYN                                                          0x180088A8
#define DC_PHY_DQ18_19_DLYN_reg_addr                                                 "0xB80088A8"
#define DC_PHY_DQ18_19_DLYN_reg                                                      0xB80088A8
#define set_DC_PHY_DQ18_19_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ18_19_DLYN_reg)=data)
#define get_DC_PHY_DQ18_19_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ18_19_DLYN_reg))
#define DC_PHY_DQ18_19_DLYN_inst_adr                                                 "0x002A"
#define DC_PHY_DQ18_19_DLYN_inst                                                     0x002A
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_3_2_shift                                  (22)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_3_2_mask                                   (0x0FC00000)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_3_2(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_3_2_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ18_19_DLYN_get_ph_sel_dq_neg_3_2(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_3_2_shift                                  (16)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_3_2_mask                                   (0x003F0000)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_3_2(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_3_2_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ18_19_DLYN_get_ph_sel_dq_pos_3_2(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_2_2_shift                                  (6)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_2_2_mask                                   (0x00000FC0)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_2_2(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_neg_2_2_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ18_19_DLYN_get_ph_sel_dq_neg_2_2(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_2_2_shift                                  (0)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_2_2_mask                                   (0x0000003F)
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_2_2(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ18_19_DLYN_ph_sel_dq_pos_2_2_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ18_19_DLYN_get_ph_sel_dq_pos_2_2(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ20_21_DLYN                                                          0x180088AC
#define DC_PHY_DQ20_21_DLYN_reg_addr                                                 "0xB80088AC"
#define DC_PHY_DQ20_21_DLYN_reg                                                      0xB80088AC
#define set_DC_PHY_DQ20_21_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ20_21_DLYN_reg)=data)
#define get_DC_PHY_DQ20_21_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ20_21_DLYN_reg))
#define DC_PHY_DQ20_21_DLYN_inst_adr                                                 "0x002B"
#define DC_PHY_DQ20_21_DLYN_inst                                                     0x002B
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_5_2_shift                                  (22)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_5_2_mask                                   (0x0FC00000)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_5_2(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_5_2_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ20_21_DLYN_get_ph_sel_dq_neg_5_2(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_5_2_shift                                  (16)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_5_2_mask                                   (0x003F0000)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_5_2(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_5_2_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ20_21_DLYN_get_ph_sel_dq_pos_5_2(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_4_2_shift                                  (6)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_4_2_mask                                   (0x00000FC0)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_4_2(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_neg_4_2_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ20_21_DLYN_get_ph_sel_dq_neg_4_2(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_4_2_shift                                  (0)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_4_2_mask                                   (0x0000003F)
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_4_2(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ20_21_DLYN_ph_sel_dq_pos_4_2_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ20_21_DLYN_get_ph_sel_dq_pos_4_2(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ22_23_DLYN                                                          0x180088B0
#define DC_PHY_DQ22_23_DLYN_reg_addr                                                 "0xB80088B0"
#define DC_PHY_DQ22_23_DLYN_reg                                                      0xB80088B0
#define set_DC_PHY_DQ22_23_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ22_23_DLYN_reg)=data)
#define get_DC_PHY_DQ22_23_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ22_23_DLYN_reg))
#define DC_PHY_DQ22_23_DLYN_inst_adr                                                 "0x002C"
#define DC_PHY_DQ22_23_DLYN_inst                                                     0x002C
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_7_2_shift                                  (22)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_7_2_mask                                   (0x0FC00000)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_7_2(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_7_2_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ22_23_DLYN_get_ph_sel_dq_neg_7_2(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_7_2_shift                                  (16)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_7_2_mask                                   (0x003F0000)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_7_2(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_7_2_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ22_23_DLYN_get_ph_sel_dq_pos_7_2(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_6_2_shift                                  (6)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_6_2_mask                                   (0x00000FC0)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_6_2(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_neg_6_2_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ22_23_DLYN_get_ph_sel_dq_neg_6_2(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_6_2_shift                                  (0)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_6_2_mask                                   (0x0000003F)
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_6_2(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ22_23_DLYN_ph_sel_dq_pos_6_2_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ22_23_DLYN_get_ph_sel_dq_pos_6_2(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ24_25_DLYN                                                          0x180088B4
#define DC_PHY_DQ24_25_DLYN_reg_addr                                                 "0xB80088B4"
#define DC_PHY_DQ24_25_DLYN_reg                                                      0xB80088B4
#define set_DC_PHY_DQ24_25_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ24_25_DLYN_reg)=data)
#define get_DC_PHY_DQ24_25_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ24_25_DLYN_reg))
#define DC_PHY_DQ24_25_DLYN_inst_adr                                                 "0x002D"
#define DC_PHY_DQ24_25_DLYN_inst                                                     0x002D
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_1_3_shift                                  (22)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_1_3_mask                                   (0x0FC00000)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_1_3(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_1_3_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ24_25_DLYN_get_ph_sel_dq_neg_1_3(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_1_3_shift                                  (16)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_1_3_mask                                   (0x003F0000)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_1_3(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_1_3_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ24_25_DLYN_get_ph_sel_dq_pos_1_3(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_0_3_shift                                  (6)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_0_3_mask                                   (0x00000FC0)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_0_3(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_neg_0_3_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ24_25_DLYN_get_ph_sel_dq_neg_0_3(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_0_3_shift                                  (0)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_0_3_mask                                   (0x0000003F)
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_0_3(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ24_25_DLYN_ph_sel_dq_pos_0_3_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ24_25_DLYN_get_ph_sel_dq_pos_0_3(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ26_27_DLYN                                                          0x180088B8
#define DC_PHY_DQ26_27_DLYN_reg_addr                                                 "0xB80088B8"
#define DC_PHY_DQ26_27_DLYN_reg                                                      0xB80088B8
#define set_DC_PHY_DQ26_27_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ26_27_DLYN_reg)=data)
#define get_DC_PHY_DQ26_27_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ26_27_DLYN_reg))
#define DC_PHY_DQ26_27_DLYN_inst_adr                                                 "0x002E"
#define DC_PHY_DQ26_27_DLYN_inst                                                     0x002E
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_3_3_shift                                  (22)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_3_3_mask                                   (0x0FC00000)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_3_3(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_3_3_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ26_27_DLYN_get_ph_sel_dq_neg_3_3(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_3_3_shift                                  (16)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_3_3_mask                                   (0x003F0000)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_3_3(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_3_3_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ26_27_DLYN_get_ph_sel_dq_pos_3_3(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_2_3_shift                                  (6)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_2_3_mask                                   (0x00000FC0)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_2_3(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_neg_2_3_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ26_27_DLYN_get_ph_sel_dq_neg_2_3(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_2_3_shift                                  (0)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_2_3_mask                                   (0x0000003F)
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_2_3(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ26_27_DLYN_ph_sel_dq_pos_2_3_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ26_27_DLYN_get_ph_sel_dq_pos_2_3(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ28_29_DLYN                                                          0x180088BC
#define DC_PHY_DQ28_29_DLYN_reg_addr                                                 "0xB80088BC"
#define DC_PHY_DQ28_29_DLYN_reg                                                      0xB80088BC
#define set_DC_PHY_DQ28_29_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ28_29_DLYN_reg)=data)
#define get_DC_PHY_DQ28_29_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ28_29_DLYN_reg))
#define DC_PHY_DQ28_29_DLYN_inst_adr                                                 "0x002F"
#define DC_PHY_DQ28_29_DLYN_inst                                                     0x002F
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_5_3_shift                                  (22)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_5_3_mask                                   (0x0FC00000)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_5_3(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_5_3_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ28_29_DLYN_get_ph_sel_dq_neg_5_3(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_5_3_shift                                  (16)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_5_3_mask                                   (0x003F0000)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_5_3(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_5_3_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ28_29_DLYN_get_ph_sel_dq_pos_5_3(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_4_3_shift                                  (6)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_4_3_mask                                   (0x00000FC0)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_4_3(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_neg_4_3_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ28_29_DLYN_get_ph_sel_dq_neg_4_3(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_4_3_shift                                  (0)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_4_3_mask                                   (0x0000003F)
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_4_3(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ28_29_DLYN_ph_sel_dq_pos_4_3_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ28_29_DLYN_get_ph_sel_dq_pos_4_3(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_DQ30_31_DLYN                                                          0x180088C0
#define DC_PHY_DQ30_31_DLYN_reg_addr                                                 "0xB80088C0"
#define DC_PHY_DQ30_31_DLYN_reg                                                      0xB80088C0
#define set_DC_PHY_DQ30_31_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ30_31_DLYN_reg)=data)
#define get_DC_PHY_DQ30_31_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQ30_31_DLYN_reg))
#define DC_PHY_DQ30_31_DLYN_inst_adr                                                 "0x0030"
#define DC_PHY_DQ30_31_DLYN_inst                                                     0x0030
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_7_3_shift                                  (22)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_7_3_mask                                   (0x0FC00000)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_7_3(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_7_3_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ30_31_DLYN_get_ph_sel_dq_neg_7_3(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_7_3_shift                                  (16)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_7_3_mask                                   (0x003F0000)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_7_3(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_7_3_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ30_31_DLYN_get_ph_sel_dq_pos_7_3(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_6_3_shift                                  (6)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_6_3_mask                                   (0x00000FC0)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_6_3(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_neg_6_3_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ30_31_DLYN_get_ph_sel_dq_neg_6_3(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_6_3_shift                                  (0)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_6_3_mask                                   (0x0000003F)
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_6_3(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_DQ30_31_DLYN_ph_sel_dq_pos_6_3_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_DQ30_31_DLYN_get_ph_sel_dq_pos_6_3(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_CAL0_1_DLYN                                                           0x180088C4
#define DC_PHY_CAL0_1_DLYN_reg_addr                                                  "0xB80088C4"
#define DC_PHY_CAL0_1_DLYN_reg                                                       0xB80088C4
#define set_DC_PHY_CAL0_1_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL0_1_DLYN_reg)=data)
#define get_DC_PHY_CAL0_1_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL0_1_DLYN_reg))
#define DC_PHY_CAL0_1_DLYN_inst_adr                                                  "0x0031"
#define DC_PHY_CAL0_1_DLYN_inst                                                      0x0031
#define DC_PHY_CAL0_1_DLYN_non_valid_shift                                           (31)
#define DC_PHY_CAL0_1_DLYN_non_valid_mask                                            (0x80000000)
#define DC_PHY_CAL0_1_DLYN_non_valid(data)                                           (0x80000000&((data)<<31))
#define DC_PHY_CAL0_1_DLYN_non_valid_src(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL0_1_DLYN_get_non_valid(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL0_1_DLYN_cal_dq1_neg_dlyn_0_shift                                  (22)
#define DC_PHY_CAL0_1_DLYN_cal_dq1_neg_dlyn_0_mask                                   (0x0FC00000)
#define DC_PHY_CAL0_1_DLYN_cal_dq1_neg_dlyn_0(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_CAL0_1_DLYN_cal_dq1_neg_dlyn_0_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL0_1_DLYN_get_cal_dq1_neg_dlyn_0(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL0_1_DLYN_cal_dq1_pos_dlyn_0_shift                                  (16)
#define DC_PHY_CAL0_1_DLYN_cal_dq1_pos_dlyn_0_mask                                   (0x003F0000)
#define DC_PHY_CAL0_1_DLYN_cal_dq1_pos_dlyn_0(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_CAL0_1_DLYN_cal_dq1_pos_dlyn_0_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL0_1_DLYN_get_cal_dq1_pos_dlyn_0(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL0_1_DLYN_cal_dq0_neg_dlyn_0_shift                                  (6)
#define DC_PHY_CAL0_1_DLYN_cal_dq0_neg_dlyn_0_mask                                   (0x00000FC0)
#define DC_PHY_CAL0_1_DLYN_cal_dq0_neg_dlyn_0(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_CAL0_1_DLYN_cal_dq0_neg_dlyn_0_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL0_1_DLYN_get_cal_dq0_neg_dlyn_0(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL0_1_DLYN_cal_dq0_pos_dlyn_0_shift                                  (0)
#define DC_PHY_CAL0_1_DLYN_cal_dq0_pos_dlyn_0_mask                                   (0x0000003F)
#define DC_PHY_CAL0_1_DLYN_cal_dq0_pos_dlyn_0(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_CAL0_1_DLYN_cal_dq0_pos_dlyn_0_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_CAL0_1_DLYN_get_cal_dq0_pos_dlyn_0(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_CAL2_3_DLYN                                                           0x180088C8
#define DC_PHY_CAL2_3_DLYN_reg_addr                                                  "0xB80088C8"
#define DC_PHY_CAL2_3_DLYN_reg                                                       0xB80088C8
#define set_DC_PHY_CAL2_3_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL2_3_DLYN_reg)=data)
#define get_DC_PHY_CAL2_3_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL2_3_DLYN_reg))
#define DC_PHY_CAL2_3_DLYN_inst_adr                                                  "0x0032"
#define DC_PHY_CAL2_3_DLYN_inst                                                      0x0032
#define DC_PHY_CAL2_3_DLYN_non_valid_shift                                           (31)
#define DC_PHY_CAL2_3_DLYN_non_valid_mask                                            (0x80000000)
#define DC_PHY_CAL2_3_DLYN_non_valid(data)                                           (0x80000000&((data)<<31))
#define DC_PHY_CAL2_3_DLYN_non_valid_src(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL2_3_DLYN_get_non_valid(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL2_3_DLYN_cal_dq3_neg_dlyn_0_shift                                  (22)
#define DC_PHY_CAL2_3_DLYN_cal_dq3_neg_dlyn_0_mask                                   (0x0FC00000)
#define DC_PHY_CAL2_3_DLYN_cal_dq3_neg_dlyn_0(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_CAL2_3_DLYN_cal_dq3_neg_dlyn_0_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL2_3_DLYN_get_cal_dq3_neg_dlyn_0(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL2_3_DLYN_cal_dq3_pos_dlyn_0_shift                                  (16)
#define DC_PHY_CAL2_3_DLYN_cal_dq3_pos_dlyn_0_mask                                   (0x003F0000)
#define DC_PHY_CAL2_3_DLYN_cal_dq3_pos_dlyn_0(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_CAL2_3_DLYN_cal_dq3_pos_dlyn_0_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL2_3_DLYN_get_cal_dq3_pos_dlyn_0(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL2_3_DLYN_cal_dq2_neg_dlyn_0_shift                                  (6)
#define DC_PHY_CAL2_3_DLYN_cal_dq2_neg_dlyn_0_mask                                   (0x00000FC0)
#define DC_PHY_CAL2_3_DLYN_cal_dq2_neg_dlyn_0(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_CAL2_3_DLYN_cal_dq2_neg_dlyn_0_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL2_3_DLYN_get_cal_dq2_neg_dlyn_0(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL2_3_DLYN_cal_dq2_pos_dlyn_0_shift                                  (0)
#define DC_PHY_CAL2_3_DLYN_cal_dq2_pos_dlyn_0_mask                                   (0x0000003F)
#define DC_PHY_CAL2_3_DLYN_cal_dq2_pos_dlyn_0(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_CAL2_3_DLYN_cal_dq2_pos_dlyn_0_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_CAL2_3_DLYN_get_cal_dq2_pos_dlyn_0(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_CAL4_5_DLYN                                                           0x180088CC
#define DC_PHY_CAL4_5_DLYN_reg_addr                                                  "0xB80088CC"
#define DC_PHY_CAL4_5_DLYN_reg                                                       0xB80088CC
#define set_DC_PHY_CAL4_5_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL4_5_DLYN_reg)=data)
#define get_DC_PHY_CAL4_5_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL4_5_DLYN_reg))
#define DC_PHY_CAL4_5_DLYN_inst_adr                                                  "0x0033"
#define DC_PHY_CAL4_5_DLYN_inst                                                      0x0033
#define DC_PHY_CAL4_5_DLYN_non_valid_shift                                           (31)
#define DC_PHY_CAL4_5_DLYN_non_valid_mask                                            (0x80000000)
#define DC_PHY_CAL4_5_DLYN_non_valid(data)                                           (0x80000000&((data)<<31))
#define DC_PHY_CAL4_5_DLYN_non_valid_src(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL4_5_DLYN_get_non_valid(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL4_5_DLYN_cal_dq5_neg_dlyn_0_shift                                  (22)
#define DC_PHY_CAL4_5_DLYN_cal_dq5_neg_dlyn_0_mask                                   (0x0FC00000)
#define DC_PHY_CAL4_5_DLYN_cal_dq5_neg_dlyn_0(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_CAL4_5_DLYN_cal_dq5_neg_dlyn_0_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL4_5_DLYN_get_cal_dq5_neg_dlyn_0(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL4_5_DLYN_cal_dq5_pos_dlyn_0_shift                                  (16)
#define DC_PHY_CAL4_5_DLYN_cal_dq5_pos_dlyn_0_mask                                   (0x003F0000)
#define DC_PHY_CAL4_5_DLYN_cal_dq5_pos_dlyn_0(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_CAL4_5_DLYN_cal_dq5_pos_dlyn_0_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL4_5_DLYN_get_cal_dq5_pos_dlyn_0(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL4_5_DLYN_cal_dq4_neg_dlyn_0_shift                                  (6)
#define DC_PHY_CAL4_5_DLYN_cal_dq4_neg_dlyn_0_mask                                   (0x00000FC0)
#define DC_PHY_CAL4_5_DLYN_cal_dq4_neg_dlyn_0(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_CAL4_5_DLYN_cal_dq4_neg_dlyn_0_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL4_5_DLYN_get_cal_dq4_neg_dlyn_0(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL4_5_DLYN_cal_dq4_pos_dlyn_0_shift                                  (0)
#define DC_PHY_CAL4_5_DLYN_cal_dq4_pos_dlyn_0_mask                                   (0x0000003F)
#define DC_PHY_CAL4_5_DLYN_cal_dq4_pos_dlyn_0(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_CAL4_5_DLYN_cal_dq4_pos_dlyn_0_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_CAL4_5_DLYN_get_cal_dq4_pos_dlyn_0(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_CAL6_7_DLYN                                                           0x180088D0
#define DC_PHY_CAL6_7_DLYN_reg_addr                                                  "0xB80088D0"
#define DC_PHY_CAL6_7_DLYN_reg                                                       0xB80088D0
#define set_DC_PHY_CAL6_7_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL6_7_DLYN_reg)=data)
#define get_DC_PHY_CAL6_7_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL6_7_DLYN_reg))
#define DC_PHY_CAL6_7_DLYN_inst_adr                                                  "0x0034"
#define DC_PHY_CAL6_7_DLYN_inst                                                      0x0034
#define DC_PHY_CAL6_7_DLYN_non_valid_shift                                           (31)
#define DC_PHY_CAL6_7_DLYN_non_valid_mask                                            (0x80000000)
#define DC_PHY_CAL6_7_DLYN_non_valid(data)                                           (0x80000000&((data)<<31))
#define DC_PHY_CAL6_7_DLYN_non_valid_src(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL6_7_DLYN_get_non_valid(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL6_7_DLYN_cal_dq7_neg_dlyn_0_shift                                  (22)
#define DC_PHY_CAL6_7_DLYN_cal_dq7_neg_dlyn_0_mask                                   (0x0FC00000)
#define DC_PHY_CAL6_7_DLYN_cal_dq7_neg_dlyn_0(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_CAL6_7_DLYN_cal_dq7_neg_dlyn_0_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL6_7_DLYN_get_cal_dq7_neg_dlyn_0(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL6_7_DLYN_cal_dq7_pos_dlyn_0_shift                                  (16)
#define DC_PHY_CAL6_7_DLYN_cal_dq7_pos_dlyn_0_mask                                   (0x003F0000)
#define DC_PHY_CAL6_7_DLYN_cal_dq7_pos_dlyn_0(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_CAL6_7_DLYN_cal_dq7_pos_dlyn_0_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL6_7_DLYN_get_cal_dq7_pos_dlyn_0(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL6_7_DLYN_cal_dq6_neg_dlyn_0_shift                                  (6)
#define DC_PHY_CAL6_7_DLYN_cal_dq6_neg_dlyn_0_mask                                   (0x00000FC0)
#define DC_PHY_CAL6_7_DLYN_cal_dq6_neg_dlyn_0(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_CAL6_7_DLYN_cal_dq6_neg_dlyn_0_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL6_7_DLYN_get_cal_dq6_neg_dlyn_0(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL6_7_DLYN_cal_dq6_pos_dlyn_0_shift                                  (0)
#define DC_PHY_CAL6_7_DLYN_cal_dq6_pos_dlyn_0_mask                                   (0x0000003F)
#define DC_PHY_CAL6_7_DLYN_cal_dq6_pos_dlyn_0(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_CAL6_7_DLYN_cal_dq6_pos_dlyn_0_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_CAL6_7_DLYN_get_cal_dq6_pos_dlyn_0(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_CAL8_9_DLYN                                                           0x180088D4
#define DC_PHY_CAL8_9_DLYN_reg_addr                                                  "0xB80088D4"
#define DC_PHY_CAL8_9_DLYN_reg                                                       0xB80088D4
#define set_DC_PHY_CAL8_9_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL8_9_DLYN_reg)=data)
#define get_DC_PHY_CAL8_9_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL8_9_DLYN_reg))
#define DC_PHY_CAL8_9_DLYN_inst_adr                                                  "0x0035"
#define DC_PHY_CAL8_9_DLYN_inst                                                      0x0035
#define DC_PHY_CAL8_9_DLYN_non_valid_shift                                           (31)
#define DC_PHY_CAL8_9_DLYN_non_valid_mask                                            (0x80000000)
#define DC_PHY_CAL8_9_DLYN_non_valid(data)                                           (0x80000000&((data)<<31))
#define DC_PHY_CAL8_9_DLYN_non_valid_src(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL8_9_DLYN_get_non_valid(data)                                       ((0x80000000&(data))>>31)
#define DC_PHY_CAL8_9_DLYN_cal_dq1_neg_dlyn_1_shift                                  (22)
#define DC_PHY_CAL8_9_DLYN_cal_dq1_neg_dlyn_1_mask                                   (0x0FC00000)
#define DC_PHY_CAL8_9_DLYN_cal_dq1_neg_dlyn_1(data)                                  (0x0FC00000&((data)<<22))
#define DC_PHY_CAL8_9_DLYN_cal_dq1_neg_dlyn_1_src(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL8_9_DLYN_get_cal_dq1_neg_dlyn_1(data)                              ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL8_9_DLYN_cal_dq1_pos_dlyn_1_shift                                  (16)
#define DC_PHY_CAL8_9_DLYN_cal_dq1_pos_dlyn_1_mask                                   (0x003F0000)
#define DC_PHY_CAL8_9_DLYN_cal_dq1_pos_dlyn_1(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_CAL8_9_DLYN_cal_dq1_pos_dlyn_1_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL8_9_DLYN_get_cal_dq1_pos_dlyn_1(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_CAL8_9_DLYN_cal_dq0_neg_dlyn_1_shift                                  (6)
#define DC_PHY_CAL8_9_DLYN_cal_dq0_neg_dlyn_1_mask                                   (0x00000FC0)
#define DC_PHY_CAL8_9_DLYN_cal_dq0_neg_dlyn_1(data)                                  (0x00000FC0&((data)<<6))
#define DC_PHY_CAL8_9_DLYN_cal_dq0_neg_dlyn_1_src(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL8_9_DLYN_get_cal_dq0_neg_dlyn_1(data)                              ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL8_9_DLYN_cal_dq0_pos_dlyn_1_shift                                  (0)
#define DC_PHY_CAL8_9_DLYN_cal_dq0_pos_dlyn_1_mask                                   (0x0000003F)
#define DC_PHY_CAL8_9_DLYN_cal_dq0_pos_dlyn_1(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_CAL8_9_DLYN_cal_dq0_pos_dlyn_1_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_CAL8_9_DLYN_get_cal_dq0_pos_dlyn_1(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_CAL10_11_DLYN                                                         0x180088D8
#define DC_PHY_CAL10_11_DLYN_reg_addr                                                "0xB80088D8"
#define DC_PHY_CAL10_11_DLYN_reg                                                     0xB80088D8
#define set_DC_PHY_CAL10_11_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL10_11_DLYN_reg)=data)
#define get_DC_PHY_CAL10_11_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL10_11_DLYN_reg))
#define DC_PHY_CAL10_11_DLYN_inst_adr                                                "0x0036"
#define DC_PHY_CAL10_11_DLYN_inst                                                    0x0036
#define DC_PHY_CAL10_11_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL10_11_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL10_11_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL10_11_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL10_11_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL10_11_DLYN_cal_dq3_neg_dlyn_1_shift                                (22)
#define DC_PHY_CAL10_11_DLYN_cal_dq3_neg_dlyn_1_mask                                 (0x0FC00000)
#define DC_PHY_CAL10_11_DLYN_cal_dq3_neg_dlyn_1(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL10_11_DLYN_cal_dq3_neg_dlyn_1_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL10_11_DLYN_get_cal_dq3_neg_dlyn_1(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL10_11_DLYN_cal_dq3_pos_dlyn_1_shift                                (16)
#define DC_PHY_CAL10_11_DLYN_cal_dq3_pos_dlyn_1_mask                                 (0x003F0000)
#define DC_PHY_CAL10_11_DLYN_cal_dq3_pos_dlyn_1(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL10_11_DLYN_cal_dq3_pos_dlyn_1_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL10_11_DLYN_get_cal_dq3_pos_dlyn_1(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL10_11_DLYN_cal_dq2_neg_dlyn_1_shift                                (6)
#define DC_PHY_CAL10_11_DLYN_cal_dq2_neg_dlyn_1_mask                                 (0x00000FC0)
#define DC_PHY_CAL10_11_DLYN_cal_dq2_neg_dlyn_1(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL10_11_DLYN_cal_dq2_neg_dlyn_1_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL10_11_DLYN_get_cal_dq2_neg_dlyn_1(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL10_11_DLYN_cal_dq2_pos_dlyn_1_shift                                (0)
#define DC_PHY_CAL10_11_DLYN_cal_dq2_pos_dlyn_1_mask                                 (0x0000003F)
#define DC_PHY_CAL10_11_DLYN_cal_dq2_pos_dlyn_1(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL10_11_DLYN_cal_dq2_pos_dlyn_1_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL10_11_DLYN_get_cal_dq2_pos_dlyn_1(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL12_13_DLYN                                                         0x180088DC
#define DC_PHY_CAL12_13_DLYN_reg_addr                                                "0xB80088DC"
#define DC_PHY_CAL12_13_DLYN_reg                                                     0xB80088DC
#define set_DC_PHY_CAL12_13_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL12_13_DLYN_reg)=data)
#define get_DC_PHY_CAL12_13_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL12_13_DLYN_reg))
#define DC_PHY_CAL12_13_DLYN_inst_adr                                                "0x0037"
#define DC_PHY_CAL12_13_DLYN_inst                                                    0x0037
#define DC_PHY_CAL12_13_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL12_13_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL12_13_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL12_13_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL12_13_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL12_13_DLYN_cal_dq5_neg_dlyn_1_shift                                (22)
#define DC_PHY_CAL12_13_DLYN_cal_dq5_neg_dlyn_1_mask                                 (0x0FC00000)
#define DC_PHY_CAL12_13_DLYN_cal_dq5_neg_dlyn_1(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL12_13_DLYN_cal_dq5_neg_dlyn_1_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL12_13_DLYN_get_cal_dq5_neg_dlyn_1(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL12_13_DLYN_cal_dq5_pos_dlyn_1_shift                                (16)
#define DC_PHY_CAL12_13_DLYN_cal_dq5_pos_dlyn_1_mask                                 (0x003F0000)
#define DC_PHY_CAL12_13_DLYN_cal_dq5_pos_dlyn_1(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL12_13_DLYN_cal_dq5_pos_dlyn_1_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL12_13_DLYN_get_cal_dq5_pos_dlyn_1(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL12_13_DLYN_cal_dq4_neg_dlyn_1_shift                                (6)
#define DC_PHY_CAL12_13_DLYN_cal_dq4_neg_dlyn_1_mask                                 (0x00000FC0)
#define DC_PHY_CAL12_13_DLYN_cal_dq4_neg_dlyn_1(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL12_13_DLYN_cal_dq4_neg_dlyn_1_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL12_13_DLYN_get_cal_dq4_neg_dlyn_1(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL12_13_DLYN_cal_dq4_pos_dlyn_1_shift                                (0)
#define DC_PHY_CAL12_13_DLYN_cal_dq4_pos_dlyn_1_mask                                 (0x0000003F)
#define DC_PHY_CAL12_13_DLYN_cal_dq4_pos_dlyn_1(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL12_13_DLYN_cal_dq4_pos_dlyn_1_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL12_13_DLYN_get_cal_dq4_pos_dlyn_1(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL14_15_DLYN                                                         0x180088E0
#define DC_PHY_CAL14_15_DLYN_reg_addr                                                "0xB80088E0"
#define DC_PHY_CAL14_15_DLYN_reg                                                     0xB80088E0
#define set_DC_PHY_CAL14_15_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL14_15_DLYN_reg)=data)
#define get_DC_PHY_CAL14_15_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL14_15_DLYN_reg))
#define DC_PHY_CAL14_15_DLYN_inst_adr                                                "0x0038"
#define DC_PHY_CAL14_15_DLYN_inst                                                    0x0038
#define DC_PHY_CAL14_15_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL14_15_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL14_15_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL14_15_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL14_15_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL14_15_DLYN_cal_dq7_neg_dlyn_1_shift                                (22)
#define DC_PHY_CAL14_15_DLYN_cal_dq7_neg_dlyn_1_mask                                 (0x0FC00000)
#define DC_PHY_CAL14_15_DLYN_cal_dq7_neg_dlyn_1(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL14_15_DLYN_cal_dq7_neg_dlyn_1_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL14_15_DLYN_get_cal_dq7_neg_dlyn_1(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL14_15_DLYN_cal_dq7_pos_dlyn_1_shift                                (16)
#define DC_PHY_CAL14_15_DLYN_cal_dq7_pos_dlyn_1_mask                                 (0x003F0000)
#define DC_PHY_CAL14_15_DLYN_cal_dq7_pos_dlyn_1(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL14_15_DLYN_cal_dq7_pos_dlyn_1_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL14_15_DLYN_get_cal_dq7_pos_dlyn_1(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL14_15_DLYN_cal_dq6_neg_dlyn_1_shift                                (6)
#define DC_PHY_CAL14_15_DLYN_cal_dq6_neg_dlyn_1_mask                                 (0x00000FC0)
#define DC_PHY_CAL14_15_DLYN_cal_dq6_neg_dlyn_1(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL14_15_DLYN_cal_dq6_neg_dlyn_1_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL14_15_DLYN_get_cal_dq6_neg_dlyn_1(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL14_15_DLYN_cal_dq6_pos_dlyn_1_shift                                (0)
#define DC_PHY_CAL14_15_DLYN_cal_dq6_pos_dlyn_1_mask                                 (0x0000003F)
#define DC_PHY_CAL14_15_DLYN_cal_dq6_pos_dlyn_1(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL14_15_DLYN_cal_dq6_pos_dlyn_1_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL14_15_DLYN_get_cal_dq6_pos_dlyn_1(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL16_17_DLYN                                                         0x180088E4
#define DC_PHY_CAL16_17_DLYN_reg_addr                                                "0xB80088E4"
#define DC_PHY_CAL16_17_DLYN_reg                                                     0xB80088E4
#define set_DC_PHY_CAL16_17_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL16_17_DLYN_reg)=data)
#define get_DC_PHY_CAL16_17_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL16_17_DLYN_reg))
#define DC_PHY_CAL16_17_DLYN_inst_adr                                                "0x0039"
#define DC_PHY_CAL16_17_DLYN_inst                                                    0x0039
#define DC_PHY_CAL16_17_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL16_17_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL16_17_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL16_17_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL16_17_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL16_17_DLYN_cal_dq1_neg_dlyn_2_shift                                (22)
#define DC_PHY_CAL16_17_DLYN_cal_dq1_neg_dlyn_2_mask                                 (0x0FC00000)
#define DC_PHY_CAL16_17_DLYN_cal_dq1_neg_dlyn_2(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL16_17_DLYN_cal_dq1_neg_dlyn_2_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL16_17_DLYN_get_cal_dq1_neg_dlyn_2(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL16_17_DLYN_cal_dq1_pos_dlyn_2_shift                                (16)
#define DC_PHY_CAL16_17_DLYN_cal_dq1_pos_dlyn_2_mask                                 (0x003F0000)
#define DC_PHY_CAL16_17_DLYN_cal_dq1_pos_dlyn_2(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL16_17_DLYN_cal_dq1_pos_dlyn_2_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL16_17_DLYN_get_cal_dq1_pos_dlyn_2(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL16_17_DLYN_cal_dq0_neg_dlyn_2_shift                                (6)
#define DC_PHY_CAL16_17_DLYN_cal_dq0_neg_dlyn_2_mask                                 (0x00000FC0)
#define DC_PHY_CAL16_17_DLYN_cal_dq0_neg_dlyn_2(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL16_17_DLYN_cal_dq0_neg_dlyn_2_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL16_17_DLYN_get_cal_dq0_neg_dlyn_2(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL16_17_DLYN_cal_dq0_pos_dlyn_2_shift                                (0)
#define DC_PHY_CAL16_17_DLYN_cal_dq0_pos_dlyn_2_mask                                 (0x0000003F)
#define DC_PHY_CAL16_17_DLYN_cal_dq0_pos_dlyn_2(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL16_17_DLYN_cal_dq0_pos_dlyn_2_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL16_17_DLYN_get_cal_dq0_pos_dlyn_2(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL18_19_DLYN                                                         0x180088E8
#define DC_PHY_CAL18_19_DLYN_reg_addr                                                "0xB80088E8"
#define DC_PHY_CAL18_19_DLYN_reg                                                     0xB80088E8
#define set_DC_PHY_CAL18_19_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL18_19_DLYN_reg)=data)
#define get_DC_PHY_CAL18_19_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL18_19_DLYN_reg))
#define DC_PHY_CAL18_19_DLYN_inst_adr                                                "0x003A"
#define DC_PHY_CAL18_19_DLYN_inst                                                    0x003A
#define DC_PHY_CAL18_19_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL18_19_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL18_19_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL18_19_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL18_19_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL18_19_DLYN_cal_dq3_neg_dlyn_2_shift                                (22)
#define DC_PHY_CAL18_19_DLYN_cal_dq3_neg_dlyn_2_mask                                 (0x0FC00000)
#define DC_PHY_CAL18_19_DLYN_cal_dq3_neg_dlyn_2(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL18_19_DLYN_cal_dq3_neg_dlyn_2_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL18_19_DLYN_get_cal_dq3_neg_dlyn_2(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL18_19_DLYN_cal_dq3_pos_dlyn_2_shift                                (16)
#define DC_PHY_CAL18_19_DLYN_cal_dq3_pos_dlyn_2_mask                                 (0x003F0000)
#define DC_PHY_CAL18_19_DLYN_cal_dq3_pos_dlyn_2(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL18_19_DLYN_cal_dq3_pos_dlyn_2_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL18_19_DLYN_get_cal_dq3_pos_dlyn_2(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL18_19_DLYN_cal_dq2_neg_dlyn_2_shift                                (6)
#define DC_PHY_CAL18_19_DLYN_cal_dq2_neg_dlyn_2_mask                                 (0x00000FC0)
#define DC_PHY_CAL18_19_DLYN_cal_dq2_neg_dlyn_2(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL18_19_DLYN_cal_dq2_neg_dlyn_2_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL18_19_DLYN_get_cal_dq2_neg_dlyn_2(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL18_19_DLYN_cal_dq2_pos_dlyn_2_shift                                (0)
#define DC_PHY_CAL18_19_DLYN_cal_dq2_pos_dlyn_2_mask                                 (0x0000003F)
#define DC_PHY_CAL18_19_DLYN_cal_dq2_pos_dlyn_2(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL18_19_DLYN_cal_dq2_pos_dlyn_2_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL18_19_DLYN_get_cal_dq2_pos_dlyn_2(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL20_21_DLYN                                                         0x180088EC
#define DC_PHY_CAL20_21_DLYN_reg_addr                                                "0xB80088EC"
#define DC_PHY_CAL20_21_DLYN_reg                                                     0xB80088EC
#define set_DC_PHY_CAL20_21_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL20_21_DLYN_reg)=data)
#define get_DC_PHY_CAL20_21_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL20_21_DLYN_reg))
#define DC_PHY_CAL20_21_DLYN_inst_adr                                                "0x003B"
#define DC_PHY_CAL20_21_DLYN_inst                                                    0x003B
#define DC_PHY_CAL20_21_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL20_21_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL20_21_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL20_21_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL20_21_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL20_21_DLYN_cal_dq5_neg_dlyn_2_shift                                (22)
#define DC_PHY_CAL20_21_DLYN_cal_dq5_neg_dlyn_2_mask                                 (0x0FC00000)
#define DC_PHY_CAL20_21_DLYN_cal_dq5_neg_dlyn_2(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL20_21_DLYN_cal_dq5_neg_dlyn_2_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL20_21_DLYN_get_cal_dq5_neg_dlyn_2(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL20_21_DLYN_cal_dq5_pos_dlyn_2_shift                                (16)
#define DC_PHY_CAL20_21_DLYN_cal_dq5_pos_dlyn_2_mask                                 (0x003F0000)
#define DC_PHY_CAL20_21_DLYN_cal_dq5_pos_dlyn_2(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL20_21_DLYN_cal_dq5_pos_dlyn_2_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL20_21_DLYN_get_cal_dq5_pos_dlyn_2(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL20_21_DLYN_cal_dq4_neg_dlyn_2_shift                                (6)
#define DC_PHY_CAL20_21_DLYN_cal_dq4_neg_dlyn_2_mask                                 (0x00000FC0)
#define DC_PHY_CAL20_21_DLYN_cal_dq4_neg_dlyn_2(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL20_21_DLYN_cal_dq4_neg_dlyn_2_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL20_21_DLYN_get_cal_dq4_neg_dlyn_2(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL20_21_DLYN_cal_dq4_pos_dlyn_2_shift                                (0)
#define DC_PHY_CAL20_21_DLYN_cal_dq4_pos_dlyn_2_mask                                 (0x0000003F)
#define DC_PHY_CAL20_21_DLYN_cal_dq4_pos_dlyn_2(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL20_21_DLYN_cal_dq4_pos_dlyn_2_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL20_21_DLYN_get_cal_dq4_pos_dlyn_2(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL22_23_DLYN                                                         0x180088F0
#define DC_PHY_CAL22_23_DLYN_reg_addr                                                "0xB80088F0"
#define DC_PHY_CAL22_23_DLYN_reg                                                     0xB80088F0
#define set_DC_PHY_CAL22_23_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL22_23_DLYN_reg)=data)
#define get_DC_PHY_CAL22_23_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL22_23_DLYN_reg))
#define DC_PHY_CAL22_23_DLYN_inst_adr                                                "0x003C"
#define DC_PHY_CAL22_23_DLYN_inst                                                    0x003C
#define DC_PHY_CAL22_23_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL22_23_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL22_23_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL22_23_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL22_23_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL22_23_DLYN_cal_dq7_neg_dlyn_2_shift                                (22)
#define DC_PHY_CAL22_23_DLYN_cal_dq7_neg_dlyn_2_mask                                 (0x0FC00000)
#define DC_PHY_CAL22_23_DLYN_cal_dq7_neg_dlyn_2(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL22_23_DLYN_cal_dq7_neg_dlyn_2_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL22_23_DLYN_get_cal_dq7_neg_dlyn_2(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL22_23_DLYN_cal_dq7_pos_dlyn_2_shift                                (16)
#define DC_PHY_CAL22_23_DLYN_cal_dq7_pos_dlyn_2_mask                                 (0x003F0000)
#define DC_PHY_CAL22_23_DLYN_cal_dq7_pos_dlyn_2(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL22_23_DLYN_cal_dq7_pos_dlyn_2_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL22_23_DLYN_get_cal_dq7_pos_dlyn_2(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL22_23_DLYN_cal_dq6_neg_dlyn_2_shift                                (6)
#define DC_PHY_CAL22_23_DLYN_cal_dq6_neg_dlyn_2_mask                                 (0x00000FC0)
#define DC_PHY_CAL22_23_DLYN_cal_dq6_neg_dlyn_2(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL22_23_DLYN_cal_dq6_neg_dlyn_2_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL22_23_DLYN_get_cal_dq6_neg_dlyn_2(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL22_23_DLYN_cal_dq6_pos_dlyn_2_shift                                (0)
#define DC_PHY_CAL22_23_DLYN_cal_dq6_pos_dlyn_2_mask                                 (0x0000003F)
#define DC_PHY_CAL22_23_DLYN_cal_dq6_pos_dlyn_2(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL22_23_DLYN_cal_dq6_pos_dlyn_2_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL22_23_DLYN_get_cal_dq6_pos_dlyn_2(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL24_25_DLYN                                                         0x180088F4
#define DC_PHY_CAL24_25_DLYN_reg_addr                                                "0xB80088F4"
#define DC_PHY_CAL24_25_DLYN_reg                                                     0xB80088F4
#define set_DC_PHY_CAL24_25_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL24_25_DLYN_reg)=data)
#define get_DC_PHY_CAL24_25_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL24_25_DLYN_reg))
#define DC_PHY_CAL24_25_DLYN_inst_adr                                                "0x003D"
#define DC_PHY_CAL24_25_DLYN_inst                                                    0x003D
#define DC_PHY_CAL24_25_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL24_25_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL24_25_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL24_25_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL24_25_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL24_25_DLYN_cal_dq1_neg_dlyn_3_shift                                (22)
#define DC_PHY_CAL24_25_DLYN_cal_dq1_neg_dlyn_3_mask                                 (0x0FC00000)
#define DC_PHY_CAL24_25_DLYN_cal_dq1_neg_dlyn_3(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL24_25_DLYN_cal_dq1_neg_dlyn_3_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL24_25_DLYN_get_cal_dq1_neg_dlyn_3(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL24_25_DLYN_cal_dq1_pos_dlyn_3_shift                                (16)
#define DC_PHY_CAL24_25_DLYN_cal_dq1_pos_dlyn_3_mask                                 (0x003F0000)
#define DC_PHY_CAL24_25_DLYN_cal_dq1_pos_dlyn_3(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL24_25_DLYN_cal_dq1_pos_dlyn_3_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL24_25_DLYN_get_cal_dq1_pos_dlyn_3(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL24_25_DLYN_cal_dq0_neg_dlyn_3_shift                                (6)
#define DC_PHY_CAL24_25_DLYN_cal_dq0_neg_dlyn_3_mask                                 (0x00000FC0)
#define DC_PHY_CAL24_25_DLYN_cal_dq0_neg_dlyn_3(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL24_25_DLYN_cal_dq0_neg_dlyn_3_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL24_25_DLYN_get_cal_dq0_neg_dlyn_3(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL24_25_DLYN_cal_dq0_pos_dlyn_3_shift                                (0)
#define DC_PHY_CAL24_25_DLYN_cal_dq0_pos_dlyn_3_mask                                 (0x0000003F)
#define DC_PHY_CAL24_25_DLYN_cal_dq0_pos_dlyn_3(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL24_25_DLYN_cal_dq0_pos_dlyn_3_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL24_25_DLYN_get_cal_dq0_pos_dlyn_3(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL26_27_DLYN                                                         0x180088F8
#define DC_PHY_CAL26_27_DLYN_reg_addr                                                "0xB80088F8"
#define DC_PHY_CAL26_27_DLYN_reg                                                     0xB80088F8
#define set_DC_PHY_CAL26_27_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL26_27_DLYN_reg)=data)
#define get_DC_PHY_CAL26_27_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL26_27_DLYN_reg))
#define DC_PHY_CAL26_27_DLYN_inst_adr                                                "0x003E"
#define DC_PHY_CAL26_27_DLYN_inst                                                    0x003E
#define DC_PHY_CAL26_27_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL26_27_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL26_27_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL26_27_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL26_27_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL26_27_DLYN_cal_dq3_neg_dlyn_3_shift                                (22)
#define DC_PHY_CAL26_27_DLYN_cal_dq3_neg_dlyn_3_mask                                 (0x0FC00000)
#define DC_PHY_CAL26_27_DLYN_cal_dq3_neg_dlyn_3(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL26_27_DLYN_cal_dq3_neg_dlyn_3_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL26_27_DLYN_get_cal_dq3_neg_dlyn_3(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL26_27_DLYN_cal_dq3_pos_dlyn_3_shift                                (16)
#define DC_PHY_CAL26_27_DLYN_cal_dq3_pos_dlyn_3_mask                                 (0x003F0000)
#define DC_PHY_CAL26_27_DLYN_cal_dq3_pos_dlyn_3(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL26_27_DLYN_cal_dq3_pos_dlyn_3_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL26_27_DLYN_get_cal_dq3_pos_dlyn_3(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL26_27_DLYN_cal_dq2_neg_dlyn_3_shift                                (6)
#define DC_PHY_CAL26_27_DLYN_cal_dq2_neg_dlyn_3_mask                                 (0x00000FC0)
#define DC_PHY_CAL26_27_DLYN_cal_dq2_neg_dlyn_3(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL26_27_DLYN_cal_dq2_neg_dlyn_3_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL26_27_DLYN_get_cal_dq2_neg_dlyn_3(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL26_27_DLYN_cal_dq2_pos_dlyn_3_shift                                (0)
#define DC_PHY_CAL26_27_DLYN_cal_dq2_pos_dlyn_3_mask                                 (0x0000003F)
#define DC_PHY_CAL26_27_DLYN_cal_dq2_pos_dlyn_3(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL26_27_DLYN_cal_dq2_pos_dlyn_3_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL26_27_DLYN_get_cal_dq2_pos_dlyn_3(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL28_29_DLYN                                                         0x180088FC
#define DC_PHY_CAL28_29_DLYN_reg_addr                                                "0xB80088FC"
#define DC_PHY_CAL28_29_DLYN_reg                                                     0xB80088FC
#define set_DC_PHY_CAL28_29_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL28_29_DLYN_reg)=data)
#define get_DC_PHY_CAL28_29_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL28_29_DLYN_reg))
#define DC_PHY_CAL28_29_DLYN_inst_adr                                                "0x003F"
#define DC_PHY_CAL28_29_DLYN_inst                                                    0x003F
#define DC_PHY_CAL28_29_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL28_29_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL28_29_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL28_29_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL28_29_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL28_29_DLYN_cal_dq5_neg_dlyn_3_shift                                (22)
#define DC_PHY_CAL28_29_DLYN_cal_dq5_neg_dlyn_3_mask                                 (0x0FC00000)
#define DC_PHY_CAL28_29_DLYN_cal_dq5_neg_dlyn_3(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL28_29_DLYN_cal_dq5_neg_dlyn_3_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL28_29_DLYN_get_cal_dq5_neg_dlyn_3(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL28_29_DLYN_cal_dq5_pos_dlyn_3_shift                                (16)
#define DC_PHY_CAL28_29_DLYN_cal_dq5_pos_dlyn_3_mask                                 (0x003F0000)
#define DC_PHY_CAL28_29_DLYN_cal_dq5_pos_dlyn_3(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL28_29_DLYN_cal_dq5_pos_dlyn_3_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL28_29_DLYN_get_cal_dq5_pos_dlyn_3(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL28_29_DLYN_cal_dq4_neg_dlyn_3_shift                                (6)
#define DC_PHY_CAL28_29_DLYN_cal_dq4_neg_dlyn_3_mask                                 (0x00000FC0)
#define DC_PHY_CAL28_29_DLYN_cal_dq4_neg_dlyn_3(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL28_29_DLYN_cal_dq4_neg_dlyn_3_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL28_29_DLYN_get_cal_dq4_neg_dlyn_3(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL28_29_DLYN_cal_dq4_pos_dlyn_3_shift                                (0)
#define DC_PHY_CAL28_29_DLYN_cal_dq4_pos_dlyn_3_mask                                 (0x0000003F)
#define DC_PHY_CAL28_29_DLYN_cal_dq4_pos_dlyn_3(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL28_29_DLYN_cal_dq4_pos_dlyn_3_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL28_29_DLYN_get_cal_dq4_pos_dlyn_3(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_CAL30_31_DLYN                                                         0x18008900
#define DC_PHY_CAL30_31_DLYN_reg_addr                                                "0xB8008900"
#define DC_PHY_CAL30_31_DLYN_reg                                                     0xB8008900
#define set_DC_PHY_CAL30_31_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL30_31_DLYN_reg)=data)
#define get_DC_PHY_CAL30_31_DLYN_reg   (*((volatile unsigned int*) DC_PHY_CAL30_31_DLYN_reg))
#define DC_PHY_CAL30_31_DLYN_inst_adr                                                "0x0040"
#define DC_PHY_CAL30_31_DLYN_inst                                                    0x0040
#define DC_PHY_CAL30_31_DLYN_non_valid_shift                                         (31)
#define DC_PHY_CAL30_31_DLYN_non_valid_mask                                          (0x80000000)
#define DC_PHY_CAL30_31_DLYN_non_valid(data)                                         (0x80000000&((data)<<31))
#define DC_PHY_CAL30_31_DLYN_non_valid_src(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL30_31_DLYN_get_non_valid(data)                                     ((0x80000000&(data))>>31)
#define DC_PHY_CAL30_31_DLYN_cal_dq7_neg_dlyn_3_shift                                (22)
#define DC_PHY_CAL30_31_DLYN_cal_dq7_neg_dlyn_3_mask                                 (0x0FC00000)
#define DC_PHY_CAL30_31_DLYN_cal_dq7_neg_dlyn_3(data)                                (0x0FC00000&((data)<<22))
#define DC_PHY_CAL30_31_DLYN_cal_dq7_neg_dlyn_3_src(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL30_31_DLYN_get_cal_dq7_neg_dlyn_3(data)                            ((0x0FC00000&(data))>>22)
#define DC_PHY_CAL30_31_DLYN_cal_dq7_pos_dlyn_3_shift                                (16)
#define DC_PHY_CAL30_31_DLYN_cal_dq7_pos_dlyn_3_mask                                 (0x003F0000)
#define DC_PHY_CAL30_31_DLYN_cal_dq7_pos_dlyn_3(data)                                (0x003F0000&((data)<<16))
#define DC_PHY_CAL30_31_DLYN_cal_dq7_pos_dlyn_3_src(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL30_31_DLYN_get_cal_dq7_pos_dlyn_3(data)                            ((0x003F0000&(data))>>16)
#define DC_PHY_CAL30_31_DLYN_cal_dq6_neg_dlyn_3_shift                                (6)
#define DC_PHY_CAL30_31_DLYN_cal_dq6_neg_dlyn_3_mask                                 (0x00000FC0)
#define DC_PHY_CAL30_31_DLYN_cal_dq6_neg_dlyn_3(data)                                (0x00000FC0&((data)<<6))
#define DC_PHY_CAL30_31_DLYN_cal_dq6_neg_dlyn_3_src(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL30_31_DLYN_get_cal_dq6_neg_dlyn_3(data)                            ((0x00000FC0&(data))>>6)
#define DC_PHY_CAL30_31_DLYN_cal_dq6_pos_dlyn_3_shift                                (0)
#define DC_PHY_CAL30_31_DLYN_cal_dq6_pos_dlyn_3_mask                                 (0x0000003F)
#define DC_PHY_CAL30_31_DLYN_cal_dq6_pos_dlyn_3(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_CAL30_31_DLYN_cal_dq6_pos_dlyn_3_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_CAL30_31_DLYN_get_cal_dq6_pos_dlyn_3(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_READ_COM_CTRL                                                         0x18008904
#define DC_PHY_READ_COM_CTRL_reg_addr                                                "0xB8008904"
#define DC_PHY_READ_COM_CTRL_reg                                                     0xB8008904
#define set_DC_PHY_READ_COM_CTRL_reg(data)   (*((volatile unsigned int*) DC_PHY_READ_COM_CTRL_reg)=data)
#define get_DC_PHY_READ_COM_CTRL_reg   (*((volatile unsigned int*) DC_PHY_READ_COM_CTRL_reg))
#define DC_PHY_READ_COM_CTRL_inst_adr                                                "0x0041"
#define DC_PHY_READ_COM_CTRL_inst                                                    0x0041
#define DC_PHY_READ_COM_CTRL_read_lat_shift                                          (12)
#define DC_PHY_READ_COM_CTRL_read_lat_mask                                           (0x00003000)
#define DC_PHY_READ_COM_CTRL_read_lat(data)                                          (0x00003000&((data)<<12))
#define DC_PHY_READ_COM_CTRL_read_lat_src(data)                                      ((0x00003000&(data))>>12)
#define DC_PHY_READ_COM_CTRL_get_read_lat(data)                                      ((0x00003000&(data))>>12)
#define DC_PHY_READ_COM_CTRL_ph_sel_read23_shift                                     (8)
#define DC_PHY_READ_COM_CTRL_ph_sel_read23_mask                                      (0x00000F00)
#define DC_PHY_READ_COM_CTRL_ph_sel_read23(data)                                     (0x00000F00&((data)<<8))
#define DC_PHY_READ_COM_CTRL_ph_sel_read23_src(data)                                 ((0x00000F00&(data))>>8)
#define DC_PHY_READ_COM_CTRL_get_ph_sel_read23(data)                                 ((0x00000F00&(data))>>8)
#define DC_PHY_READ_COM_CTRL_ph_sel_read01_shift                                     (4)
#define DC_PHY_READ_COM_CTRL_ph_sel_read01_mask                                      (0x000000F0)
#define DC_PHY_READ_COM_CTRL_ph_sel_read01(data)                                     (0x000000F0&((data)<<4))
#define DC_PHY_READ_COM_CTRL_ph_sel_read01_src(data)                                 ((0x000000F0&(data))>>4)
#define DC_PHY_READ_COM_CTRL_get_ph_sel_read01(data)                                 ((0x000000F0&(data))>>4)
#define DC_PHY_READ_COM_CTRL_read_com_sel_shift                                      (0)
#define DC_PHY_READ_COM_CTRL_read_com_sel_mask                                       (0x00000001)
#define DC_PHY_READ_COM_CTRL_read_com_sel(data)                                      (0x00000001&((data)<<0))
#define DC_PHY_READ_COM_CTRL_read_com_sel_src(data)                                  ((0x00000001&(data))>>0)
#define DC_PHY_READ_COM_CTRL_get_read_com_sel(data)                                  ((0x00000001&(data))>>0)


#define DC_PHY_CAL_CTRL                                                              0x18008908
#define DC_PHY_CAL_CTRL_reg_addr                                                     "0xB8008908"
#define DC_PHY_CAL_CTRL_reg                                                          0xB8008908
#define set_DC_PHY_CAL_CTRL_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_CTRL_reg)=data)
#define get_DC_PHY_CAL_CTRL_reg   (*((volatile unsigned int*) DC_PHY_CAL_CTRL_reg))
#define DC_PHY_CAL_CTRL_inst_adr                                                     "0x0042"
#define DC_PHY_CAL_CTRL_inst                                                         0x0042
#define DC_PHY_CAL_CTRL_pos_trig_sel_shift                                           (2)
#define DC_PHY_CAL_CTRL_pos_trig_sel_mask                                            (0x00000004)
#define DC_PHY_CAL_CTRL_pos_trig_sel(data)                                           (0x00000004&((data)<<2))
#define DC_PHY_CAL_CTRL_pos_trig_sel_src(data)                                       ((0x00000004&(data))>>2)
#define DC_PHY_CAL_CTRL_get_pos_trig_sel(data)                                       ((0x00000004&(data))>>2)
#define DC_PHY_CAL_CTRL_pre_trig_sel_shift                                           (1)
#define DC_PHY_CAL_CTRL_pre_trig_sel_mask                                            (0x00000002)
#define DC_PHY_CAL_CTRL_pre_trig_sel(data)                                           (0x00000002&((data)<<1))
#define DC_PHY_CAL_CTRL_pre_trig_sel_src(data)                                       ((0x00000002&(data))>>1)
#define DC_PHY_CAL_CTRL_get_pre_trig_sel(data)                                       ((0x00000002&(data))>>1)
#define DC_PHY_CAL_CTRL_auto_cal_shift                                               (0)
#define DC_PHY_CAL_CTRL_auto_cal_mask                                                (0x00000001)
#define DC_PHY_CAL_CTRL_auto_cal(data)                                               (0x00000001&((data)<<0))
#define DC_PHY_CAL_CTRL_auto_cal_src(data)                                           ((0x00000001&(data))>>0)
#define DC_PHY_CAL_CTRL_get_auto_cal(data)                                           ((0x00000001&(data))>>0)


#define DC_PHY_FIFO_DLYN                                                             0x1800890C
#define DC_PHY_FIFO_DLYN_reg_addr                                                    "0xB800890C"
#define DC_PHY_FIFO_DLYN_reg                                                         0xB800890C
#define set_DC_PHY_FIFO_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_FIFO_DLYN_reg)=data)
#define get_DC_PHY_FIFO_DLYN_reg   (*((volatile unsigned int*) DC_PHY_FIFO_DLYN_reg))
#define DC_PHY_FIFO_DLYN_inst_adr                                                    "0x0043"
#define DC_PHY_FIFO_DLYN_inst                                                        0x0043
#define DC_PHY_FIFO_DLYN_dqsf_dlyn_shift                                             (4)
#define DC_PHY_FIFO_DLYN_dqsf_dlyn_mask                                              (0x00000030)
#define DC_PHY_FIFO_DLYN_dqsf_dlyn(data)                                             (0x00000030&((data)<<4))
#define DC_PHY_FIFO_DLYN_dqsf_dlyn_src(data)                                         ((0x00000030&(data))>>4)
#define DC_PHY_FIFO_DLYN_get_dqsf_dlyn(data)                                         ((0x00000030&(data))>>4)
#define DC_PHY_FIFO_DLYN_Tx_dlc_sel_dck_shift                                        (0)
#define DC_PHY_FIFO_DLYN_Tx_dlc_sel_dck_mask                                         (0x0000000F)
#define DC_PHY_FIFO_DLYN_Tx_dlc_sel_dck(data)                                        (0x0000000F&((data)<<0))
#define DC_PHY_FIFO_DLYN_Tx_dlc_sel_dck_src(data)                                    ((0x0000000F&(data))>>0)
#define DC_PHY_FIFO_DLYN_get_Tx_dlc_sel_dck(data)                                    ((0x0000000F&(data))>>0)


#define DC_PHY_PAD_OE_CTL                                                            0x18008910
#define DC_PHY_PAD_OE_CTL_reg_addr                                                   "0xB8008910"
#define DC_PHY_PAD_OE_CTL_reg                                                        0xB8008910
#define set_DC_PHY_PAD_OE_CTL_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_OE_CTL_reg)=data)
#define get_DC_PHY_PAD_OE_CTL_reg   (*((volatile unsigned int*) DC_PHY_PAD_OE_CTL_reg))
#define DC_PHY_PAD_OE_CTL_inst_adr                                                   "0x0044"
#define DC_PHY_PAD_OE_CTL_inst                                                       0x0044
#define DC_PHY_PAD_OE_CTL_grp2_dqm_oe_shift                                          (13)
#define DC_PHY_PAD_OE_CTL_grp2_dqm_oe_mask                                           (0x00002000)
#define DC_PHY_PAD_OE_CTL_grp2_dqm_oe(data)                                          (0x00002000&((data)<<13))
#define DC_PHY_PAD_OE_CTL_grp2_dqm_oe_src(data)                                      ((0x00002000&(data))>>13)
#define DC_PHY_PAD_OE_CTL_get_grp2_dqm_oe(data)                                      ((0x00002000&(data))>>13)
#define DC_PHY_PAD_OE_CTL_grp0_dqm_oe_shift                                          (12)
#define DC_PHY_PAD_OE_CTL_grp0_dqm_oe_mask                                           (0x00001000)
#define DC_PHY_PAD_OE_CTL_grp0_dqm_oe(data)                                          (0x00001000&((data)<<12))
#define DC_PHY_PAD_OE_CTL_grp0_dqm_oe_src(data)                                      ((0x00001000&(data))>>12)
#define DC_PHY_PAD_OE_CTL_get_grp0_dqm_oe(data)                                      ((0x00001000&(data))>>12)
#define DC_PHY_PAD_OE_CTL_grp1_bk_oe_shift                                           (11)
#define DC_PHY_PAD_OE_CTL_grp1_bk_oe_mask                                            (0x00000800)
#define DC_PHY_PAD_OE_CTL_grp1_bk_oe(data)                                           (0x00000800&((data)<<11))
#define DC_PHY_PAD_OE_CTL_grp1_bk_oe_src(data)                                       ((0x00000800&(data))>>11)
#define DC_PHY_PAD_OE_CTL_get_grp1_bk_oe(data)                                       ((0x00000800&(data))>>11)
#define DC_PHY_PAD_OE_CTL_grp1_addr1_oe_shift                                        (10)
#define DC_PHY_PAD_OE_CTL_grp1_addr1_oe_mask                                         (0x00000400)
#define DC_PHY_PAD_OE_CTL_grp1_addr1_oe(data)                                        (0x00000400&((data)<<10))
#define DC_PHY_PAD_OE_CTL_grp1_addr1_oe_src(data)                                    ((0x00000400&(data))>>10)
#define DC_PHY_PAD_OE_CTL_get_grp1_addr1_oe(data)                                    ((0x00000400&(data))>>10)
#define DC_PHY_PAD_OE_CTL_grp1_addr_oe_shift                                         (9)
#define DC_PHY_PAD_OE_CTL_grp1_addr_oe_mask                                          (0x00000200)
#define DC_PHY_PAD_OE_CTL_grp1_addr_oe(data)                                         (0x00000200&((data)<<9))
#define DC_PHY_PAD_OE_CTL_grp1_addr_oe_src(data)                                     ((0x00000200&(data))>>9)
#define DC_PHY_PAD_OE_CTL_get_grp1_addr_oe(data)                                     ((0x00000200&(data))>>9)
#define DC_PHY_PAD_OE_CTL_grp1_cke_oe_shift                                          (8)
#define DC_PHY_PAD_OE_CTL_grp1_cke_oe_mask                                           (0x00000100)
#define DC_PHY_PAD_OE_CTL_grp1_cke_oe(data)                                          (0x00000100&((data)<<8))
#define DC_PHY_PAD_OE_CTL_grp1_cke_oe_src(data)                                      ((0x00000100&(data))>>8)
#define DC_PHY_PAD_OE_CTL_get_grp1_cke_oe(data)                                      ((0x00000100&(data))>>8)
#define DC_PHY_PAD_OE_CTL_grp1_ck_oe_shift                                           (7)
#define DC_PHY_PAD_OE_CTL_grp1_ck_oe_mask                                            (0x00000080)
#define DC_PHY_PAD_OE_CTL_grp1_ck_oe(data)                                           (0x00000080&((data)<<7))
#define DC_PHY_PAD_OE_CTL_grp1_ck_oe_src(data)                                       ((0x00000080&(data))>>7)
#define DC_PHY_PAD_OE_CTL_get_grp1_ck_oe(data)                                       ((0x00000080&(data))>>7)
#define DC_PHY_PAD_OE_CTL_grp1_ctl_oe_shift                                          (0)
#define DC_PHY_PAD_OE_CTL_grp1_ctl_oe_mask                                           (0x0000007F)
#define DC_PHY_PAD_OE_CTL_grp1_ctl_oe(data)                                          (0x0000007F&((data)<<0))
#define DC_PHY_PAD_OE_CTL_grp1_ctl_oe_src(data)                                      ((0x0000007F&(data))>>0)
#define DC_PHY_PAD_OE_CTL_get_grp1_ctl_oe(data)                                      ((0x0000007F&(data))>>0)


#define DC_PHY_PAD_ODT_CTL                                                           0x18008914
#define DC_PHY_PAD_ODT_CTL_reg_addr                                                  "0xB8008914"
#define DC_PHY_PAD_ODT_CTL_reg                                                       0xB8008914
#define set_DC_PHY_PAD_ODT_CTL_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_ODT_CTL_reg)=data)
#define get_DC_PHY_PAD_ODT_CTL_reg   (*((volatile unsigned int*) DC_PHY_PAD_ODT_CTL_reg))
#define DC_PHY_PAD_ODT_CTL_inst_adr                                                  "0x0045"
#define DC_PHY_PAD_ODT_CTL_inst                                                      0x0045
#define DC_PHY_PAD_ODT_CTL_grp2_dqm_odt_shift                                        (13)
#define DC_PHY_PAD_ODT_CTL_grp2_dqm_odt_mask                                         (0x00002000)
#define DC_PHY_PAD_ODT_CTL_grp2_dqm_odt(data)                                        (0x00002000&((data)<<13))
#define DC_PHY_PAD_ODT_CTL_grp2_dqm_odt_src(data)                                    ((0x00002000&(data))>>13)
#define DC_PHY_PAD_ODT_CTL_get_grp2_dqm_odt(data)                                    ((0x00002000&(data))>>13)
#define DC_PHY_PAD_ODT_CTL_grp0_dqm_odt_shift                                        (12)
#define DC_PHY_PAD_ODT_CTL_grp0_dqm_odt_mask                                         (0x00001000)
#define DC_PHY_PAD_ODT_CTL_grp0_dqm_odt(data)                                        (0x00001000&((data)<<12))
#define DC_PHY_PAD_ODT_CTL_grp0_dqm_odt_src(data)                                    ((0x00001000&(data))>>12)
#define DC_PHY_PAD_ODT_CTL_get_grp0_dqm_odt(data)                                    ((0x00001000&(data))>>12)
#define DC_PHY_PAD_ODT_CTL_grp1_bk_odt_shift                                         (11)
#define DC_PHY_PAD_ODT_CTL_grp1_bk_odt_mask                                          (0x00000800)
#define DC_PHY_PAD_ODT_CTL_grp1_bk_odt(data)                                         (0x00000800&((data)<<11))
#define DC_PHY_PAD_ODT_CTL_grp1_bk_odt_src(data)                                     ((0x00000800&(data))>>11)
#define DC_PHY_PAD_ODT_CTL_get_grp1_bk_odt(data)                                     ((0x00000800&(data))>>11)
#define DC_PHY_PAD_ODT_CTL_grp1_addr1_odt_shift                                      (10)
#define DC_PHY_PAD_ODT_CTL_grp1_addr1_odt_mask                                       (0x00000400)
#define DC_PHY_PAD_ODT_CTL_grp1_addr1_odt(data)                                      (0x00000400&((data)<<10))
#define DC_PHY_PAD_ODT_CTL_grp1_addr1_odt_src(data)                                  ((0x00000400&(data))>>10)
#define DC_PHY_PAD_ODT_CTL_get_grp1_addr1_odt(data)                                  ((0x00000400&(data))>>10)
#define DC_PHY_PAD_ODT_CTL_grp1_addr_odt_shift                                       (9)
#define DC_PHY_PAD_ODT_CTL_grp1_addr_odt_mask                                        (0x00000200)
#define DC_PHY_PAD_ODT_CTL_grp1_addr_odt(data)                                       (0x00000200&((data)<<9))
#define DC_PHY_PAD_ODT_CTL_grp1_addr_odt_src(data)                                   ((0x00000200&(data))>>9)
#define DC_PHY_PAD_ODT_CTL_get_grp1_addr_odt(data)                                   ((0x00000200&(data))>>9)
#define DC_PHY_PAD_ODT_CTL_grp1_cke_odt_shift                                        (8)
#define DC_PHY_PAD_ODT_CTL_grp1_cke_odt_mask                                         (0x00000100)
#define DC_PHY_PAD_ODT_CTL_grp1_cke_odt(data)                                        (0x00000100&((data)<<8))
#define DC_PHY_PAD_ODT_CTL_grp1_cke_odt_src(data)                                    ((0x00000100&(data))>>8)
#define DC_PHY_PAD_ODT_CTL_get_grp1_cke_odt(data)                                    ((0x00000100&(data))>>8)
#define DC_PHY_PAD_ODT_CTL_grp1_ck_odt_shift                                         (7)
#define DC_PHY_PAD_ODT_CTL_grp1_ck_odt_mask                                          (0x00000080)
#define DC_PHY_PAD_ODT_CTL_grp1_ck_odt(data)                                         (0x00000080&((data)<<7))
#define DC_PHY_PAD_ODT_CTL_grp1_ck_odt_src(data)                                     ((0x00000080&(data))>>7)
#define DC_PHY_PAD_ODT_CTL_get_grp1_ck_odt(data)                                     ((0x00000080&(data))>>7)
#define DC_PHY_PAD_ODT_CTL_grp1_ctl_odt_shift                                        (0)
#define DC_PHY_PAD_ODT_CTL_grp1_ctl_odt_mask                                         (0x0000007F)
#define DC_PHY_PAD_ODT_CTL_grp1_ctl_odt(data)                                        (0x0000007F&((data)<<0))
#define DC_PHY_PAD_ODT_CTL_grp1_ctl_odt_src(data)                                    ((0x0000007F&(data))>>0)
#define DC_PHY_PAD_ODT_CTL_get_grp1_ctl_odt(data)                                    ((0x0000007F&(data))>>0)


#define DC_PHY_PAD_IDDQ0                                                             0x18008918
#define DC_PHY_PAD_IDDQ0_reg_addr                                                    "0xB8008918"
#define DC_PHY_PAD_IDDQ0_reg                                                         0xB8008918
#define set_DC_PHY_PAD_IDDQ0_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_IDDQ0_reg)=data)
#define get_DC_PHY_PAD_IDDQ0_reg   (*((volatile unsigned int*) DC_PHY_PAD_IDDQ0_reg))
#define DC_PHY_PAD_IDDQ0_inst_adr                                                    "0x0046"
#define DC_PHY_PAD_IDDQ0_inst                                                        0x0046
#define DC_PHY_PAD_IDDQ0_iddq_0_shift                                                (0)
#define DC_PHY_PAD_IDDQ0_iddq_0_mask                                                 (0x003FFFFF)
#define DC_PHY_PAD_IDDQ0_iddq_0(data)                                                (0x003FFFFF&((data)<<0))
#define DC_PHY_PAD_IDDQ0_iddq_0_src(data)                                            ((0x003FFFFF&(data))>>0)
#define DC_PHY_PAD_IDDQ0_get_iddq_0(data)                                            ((0x003FFFFF&(data))>>0)


#define DC_PHY_PAD_IDDQ1                                                             0x1800891C
#define DC_PHY_PAD_IDDQ1_reg_addr                                                    "0xB800891C"
#define DC_PHY_PAD_IDDQ1_reg                                                         0xB800891C
#define set_DC_PHY_PAD_IDDQ1_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_IDDQ1_reg)=data)
#define get_DC_PHY_PAD_IDDQ1_reg   (*((volatile unsigned int*) DC_PHY_PAD_IDDQ1_reg))
#define DC_PHY_PAD_IDDQ1_inst_adr                                                    "0x0047"
#define DC_PHY_PAD_IDDQ1_inst                                                        0x0047
#define DC_PHY_PAD_IDDQ1_iddq_1_shift                                                (0)
#define DC_PHY_PAD_IDDQ1_iddq_1_mask                                                 (0x3FFFFFFF)
#define DC_PHY_PAD_IDDQ1_iddq_1(data)                                                (0x3FFFFFFF&((data)<<0))
#define DC_PHY_PAD_IDDQ1_iddq_1_src(data)                                            ((0x3FFFFFFF&(data))>>0)
#define DC_PHY_PAD_IDDQ1_get_iddq_1(data)                                            ((0x3FFFFFFF&(data))>>0)


#define DC_PHY_PAD_IDDQ2                                                             0x18008920
#define DC_PHY_PAD_IDDQ2_reg_addr                                                    "0xB8008920"
#define DC_PHY_PAD_IDDQ2_reg                                                         0xB8008920
#define set_DC_PHY_PAD_IDDQ2_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_IDDQ2_reg)=data)
#define get_DC_PHY_PAD_IDDQ2_reg   (*((volatile unsigned int*) DC_PHY_PAD_IDDQ2_reg))
#define DC_PHY_PAD_IDDQ2_inst_adr                                                    "0x0048"
#define DC_PHY_PAD_IDDQ2_inst                                                        0x0048
#define DC_PHY_PAD_IDDQ2_iddq_2_shift                                                (0)
#define DC_PHY_PAD_IDDQ2_iddq_2_mask                                                 (0x003FFFFF)
#define DC_PHY_PAD_IDDQ2_iddq_2(data)                                                (0x003FFFFF&((data)<<0))
#define DC_PHY_PAD_IDDQ2_iddq_2_src(data)                                            ((0x003FFFFF&(data))>>0)
#define DC_PHY_PAD_IDDQ2_get_iddq_2(data)                                            ((0x003FFFFF&(data))>>0)


#define DC_PHY_PAD_ZCTRL_OVRD                                                        0x18008924
#define DC_PHY_PAD_ZCTRL_OVRD_reg_addr                                               "0xB8008924"
#define DC_PHY_PAD_ZCTRL_OVRD_reg                                                    0xB8008924
#define set_DC_PHY_PAD_ZCTRL_OVRD_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_ZCTRL_OVRD_reg)=data)
#define get_DC_PHY_PAD_ZCTRL_OVRD_reg   (*((volatile unsigned int*) DC_PHY_PAD_ZCTRL_OVRD_reg))
#define DC_PHY_PAD_ZCTRL_OVRD_inst_adr                                               "0x0049"
#define DC_PHY_PAD_ZCTRL_OVRD_inst                                                   0x0049
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_en_shift                                    (20)
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_en_mask                                     (0x00100000)
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_en(data)                                    (0x00100000&((data)<<20))
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_en_src(data)                                ((0x00100000&(data))>>20)
#define DC_PHY_PAD_ZCTRL_OVRD_get_zctrl_ovrd_en(data)                                ((0x00100000&(data))>>20)
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_data_shift                                  (0)
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_data_mask                                   (0x000FFFFF)
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_data(data)                                  (0x000FFFFF&((data)<<0))
#define DC_PHY_PAD_ZCTRL_OVRD_zctrl_ovrd_data_src(data)                              ((0x000FFFFF&(data))>>0)
#define DC_PHY_PAD_ZCTRL_OVRD_get_zctrl_ovrd_data(data)                              ((0x000FFFFF&(data))>>0)


#define DC_PHY_PAD_CTRL_PROG                                                         0x18008928
#define DC_PHY_PAD_CTRL_PROG_reg_addr                                                "0xB8008928"
#define DC_PHY_PAD_CTRL_PROG_reg                                                     0xB8008928
#define set_DC_PHY_PAD_CTRL_PROG_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_CTRL_PROG_reg)=data)
#define get_DC_PHY_PAD_CTRL_PROG_reg   (*((volatile unsigned int*) DC_PHY_PAD_CTRL_PROG_reg))
#define DC_PHY_PAD_CTRL_PROG_inst_adr                                                "0x004A"
#define DC_PHY_PAD_CTRL_PROG_inst                                                    0x004A
#define DC_PHY_PAD_CTRL_PROG_dzq_auto_up_shift                                       (16)
#define DC_PHY_PAD_CTRL_PROG_dzq_auto_up_mask                                        (0x00010000)
#define DC_PHY_PAD_CTRL_PROG_dzq_auto_up(data)                                       (0x00010000&((data)<<16))
#define DC_PHY_PAD_CTRL_PROG_dzq_auto_up_src(data)                                   ((0x00010000&(data))>>16)
#define DC_PHY_PAD_CTRL_PROG_get_dzq_auto_up(data)                                   ((0x00010000&(data))>>16)
#define DC_PHY_PAD_CTRL_PROG_zctrl_clk_sel_shift                                     (15)
#define DC_PHY_PAD_CTRL_PROG_zctrl_clk_sel_mask                                      (0x00008000)
#define DC_PHY_PAD_CTRL_PROG_zctrl_clk_sel(data)                                     (0x00008000&((data)<<15))
#define DC_PHY_PAD_CTRL_PROG_zctrl_clk_sel_src(data)                                 ((0x00008000&(data))>>15)
#define DC_PHY_PAD_CTRL_PROG_get_zctrl_clk_sel(data)                                 ((0x00008000&(data))>>15)
#define DC_PHY_PAD_CTRL_PROG_zctrl_start_shift                                       (14)
#define DC_PHY_PAD_CTRL_PROG_zctrl_start_mask                                        (0x00004000)
#define DC_PHY_PAD_CTRL_PROG_zctrl_start(data)                                       (0x00004000&((data)<<14))
#define DC_PHY_PAD_CTRL_PROG_zctrl_start_src(data)                                   ((0x00004000&(data))>>14)
#define DC_PHY_PAD_CTRL_PROG_get_zctrl_start(data)                                   ((0x00004000&(data))>>14)
#define DC_PHY_PAD_CTRL_PROG_zprog_shift                                             (0)
#define DC_PHY_PAD_CTRL_PROG_zprog_mask                                              (0x00003FFF)
#define DC_PHY_PAD_CTRL_PROG_zprog(data)                                             (0x00003FFF&((data)<<0))
#define DC_PHY_PAD_CTRL_PROG_zprog_src(data)                                         ((0x00003FFF&(data))>>0)
#define DC_PHY_PAD_CTRL_PROG_get_zprog(data)                                         ((0x00003FFF&(data))>>0)


#define DC_PHY_PAD_ZCTRL_STATUS                                                      0x1800892C
#define DC_PHY_PAD_ZCTRL_STATUS_reg_addr                                             "0xB800892C"
#define DC_PHY_PAD_ZCTRL_STATUS_reg                                                  0xB800892C
#define set_DC_PHY_PAD_ZCTRL_STATUS_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_ZCTRL_STATUS_reg)=data)
#define get_DC_PHY_PAD_ZCTRL_STATUS_reg   (*((volatile unsigned int*) DC_PHY_PAD_ZCTRL_STATUS_reg))
#define DC_PHY_PAD_ZCTRL_STATUS_inst_adr                                             "0x004B"
#define DC_PHY_PAD_ZCTRL_STATUS_inst                                                 0x004B
#define DC_PHY_PAD_ZCTRL_STATUS_zctrl_status_shift                                   (0)
#define DC_PHY_PAD_ZCTRL_STATUS_zctrl_status_mask                                    (0xFFFFFFFF)
#define DC_PHY_PAD_ZCTRL_STATUS_zctrl_status(data)                                   (0xFFFFFFFF&((data)<<0))
#define DC_PHY_PAD_ZCTRL_STATUS_zctrl_status_src(data)                               ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_PAD_ZCTRL_STATUS_get_zctrl_status(data)                               ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DQ_DLY_SEL0                                                           0x18008930
#define DC_PHY_DQ_DLY_SEL0_reg_addr                                                  "0xB8008930"
#define DC_PHY_DQ_DLY_SEL0_reg                                                       0xB8008930
#define set_DC_PHY_DQ_DLY_SEL0_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL0_reg)=data)
#define get_DC_PHY_DQ_DLY_SEL0_reg   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL0_reg))
#define DC_PHY_DQ_DLY_SEL0_inst_adr                                                  "0x004C"
#define DC_PHY_DQ_DLY_SEL0_inst                                                      0x004C
#define DC_PHY_DQ_DLY_SEL0_dq_dly_sel0_shift                                         (0)
#define DC_PHY_DQ_DLY_SEL0_dq_dly_sel0_mask                                          (0xFFFFFFFF)
#define DC_PHY_DQ_DLY_SEL0_dq_dly_sel0(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DQ_DLY_SEL0_dq_dly_sel0_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DQ_DLY_SEL0_get_dq_dly_sel0(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DQ_DLY_SEL1                                                           0x18008934
#define DC_PHY_DQ_DLY_SEL1_reg_addr                                                  "0xB8008934"
#define DC_PHY_DQ_DLY_SEL1_reg                                                       0xB8008934
#define set_DC_PHY_DQ_DLY_SEL1_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL1_reg)=data)
#define get_DC_PHY_DQ_DLY_SEL1_reg   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL1_reg))
#define DC_PHY_DQ_DLY_SEL1_inst_adr                                                  "0x004D"
#define DC_PHY_DQ_DLY_SEL1_inst                                                      0x004D
#define DC_PHY_DQ_DLY_SEL1_dq_dly_sel1_shift                                         (0)
#define DC_PHY_DQ_DLY_SEL1_dq_dly_sel1_mask                                          (0xFFFFFFFF)
#define DC_PHY_DQ_DLY_SEL1_dq_dly_sel1(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DQ_DLY_SEL1_dq_dly_sel1_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DQ_DLY_SEL1_get_dq_dly_sel1(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DQ_DLY_SEL2                                                           0x18008938
#define DC_PHY_DQ_DLY_SEL2_reg_addr                                                  "0xB8008938"
#define DC_PHY_DQ_DLY_SEL2_reg                                                       0xB8008938
#define set_DC_PHY_DQ_DLY_SEL2_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL2_reg)=data)
#define get_DC_PHY_DQ_DLY_SEL2_reg   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL2_reg))
#define DC_PHY_DQ_DLY_SEL2_inst_adr                                                  "0x004E"
#define DC_PHY_DQ_DLY_SEL2_inst                                                      0x004E
#define DC_PHY_DQ_DLY_SEL2_dq_dly_sel2_shift                                         (0)
#define DC_PHY_DQ_DLY_SEL2_dq_dly_sel2_mask                                          (0xFFFFFFFF)
#define DC_PHY_DQ_DLY_SEL2_dq_dly_sel2(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DQ_DLY_SEL2_dq_dly_sel2_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DQ_DLY_SEL2_get_dq_dly_sel2(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DQ_DLY_SEL3                                                           0x1800893C
#define DC_PHY_DQ_DLY_SEL3_reg_addr                                                  "0xB800893C"
#define DC_PHY_DQ_DLY_SEL3_reg                                                       0xB800893C
#define set_DC_PHY_DQ_DLY_SEL3_reg(data)   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL3_reg)=data)
#define get_DC_PHY_DQ_DLY_SEL3_reg   (*((volatile unsigned int*) DC_PHY_DQ_DLY_SEL3_reg))
#define DC_PHY_DQ_DLY_SEL3_inst_adr                                                  "0x004F"
#define DC_PHY_DQ_DLY_SEL3_inst                                                      0x004F
#define DC_PHY_DQ_DLY_SEL3_dq_dly_sel3_shift                                         (0)
#define DC_PHY_DQ_DLY_SEL3_dq_dly_sel3_mask                                          (0xFFFFFFFF)
#define DC_PHY_DQ_DLY_SEL3_dq_dly_sel3(data)                                         (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DQ_DLY_SEL3_dq_dly_sel3_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DQ_DLY_SEL3_get_dq_dly_sel3(data)                                     ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DUMMY_REG                                                             0x18008940
#define DC_PHY_DUMMY_REG_reg_addr                                                    "0xB8008940"
#define DC_PHY_DUMMY_REG_reg                                                         0xB8008940
#define set_DC_PHY_DUMMY_REG_reg(data)   (*((volatile unsigned int*) DC_PHY_DUMMY_REG_reg)=data)
#define get_DC_PHY_DUMMY_REG_reg   (*((volatile unsigned int*) DC_PHY_DUMMY_REG_reg))
#define DC_PHY_DUMMY_REG_inst_adr                                                    "0x0050"
#define DC_PHY_DUMMY_REG_inst                                                        0x0050
#define DC_PHY_DUMMY_REG_dummy_reg_shift                                             (0)
#define DC_PHY_DUMMY_REG_dummy_reg_mask                                              (0xFFFFFFFF)
#define DC_PHY_DUMMY_REG_dummy_reg(data)                                             (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DUMMY_REG_dummy_reg_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DUMMY_REG_get_dummy_reg(data)                                         ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_HALF_CYCLE_CAL_EN                                                     0x18008944
#define DC_PHY_HALF_CYCLE_CAL_EN_reg_addr                                            "0xB8008944"
#define DC_PHY_HALF_CYCLE_CAL_EN_reg                                                 0xB8008944
#define set_DC_PHY_HALF_CYCLE_CAL_EN_reg(data)   (*((volatile unsigned int*) DC_PHY_HALF_CYCLE_CAL_EN_reg)=data)
#define get_DC_PHY_HALF_CYCLE_CAL_EN_reg   (*((volatile unsigned int*) DC_PHY_HALF_CYCLE_CAL_EN_reg))
#define DC_PHY_HALF_CYCLE_CAL_EN_inst_adr                                            "0x0051"
#define DC_PHY_HALF_CYCLE_CAL_EN_inst                                                0x0051
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_sel_shift                            (17)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_sel_mask                             (0x00020000)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_sel(data)                            (0x00020000&((data)<<17))
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_sel_src(data)                        ((0x00020000&(data))>>17)
#define DC_PHY_HALF_CYCLE_CAL_EN_get_half_cycle_cal_sel(data)                        ((0x00020000&(data))>>17)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_ref_thr_shift                            (9)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_ref_thr_mask                             (0x0001FE00)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_ref_thr(data)                            (0x0001FE00&((data)<<9))
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_ref_thr_src(data)                        ((0x0001FE00&(data))>>9)
#define DC_PHY_HALF_CYCLE_CAL_EN_get_half_cycle_ref_thr(data)                        ((0x0001FE00&(data))>>9)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_thr_shift                            (3)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_thr_mask                             (0x000001F8)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_thr(data)                            (0x000001F8&((data)<<3))
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_thr_src(data)                        ((0x000001F8&(data))>>3)
#define DC_PHY_HALF_CYCLE_CAL_EN_get_half_cycle_res_thr(data)                        ((0x000001F8&(data))>>3)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_mod_shift                            (1)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_mod_mask                             (0x00000006)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_mod(data)                            (0x00000006&((data)<<1))
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_res_mod_src(data)                        ((0x00000006&(data))>>1)
#define DC_PHY_HALF_CYCLE_CAL_EN_get_half_cycle_res_mod(data)                        ((0x00000006&(data))>>1)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_en_shift                             (0)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_en_mask                              (0x00000001)
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_en(data)                             (0x00000001&((data)<<0))
#define DC_PHY_HALF_CYCLE_CAL_EN_half_cycle_cal_en_src(data)                         ((0x00000001&(data))>>0)
#define DC_PHY_HALF_CYCLE_CAL_EN_get_half_cycle_cal_en(data)                         ((0x00000001&(data))>>0)


#define DC_PHY_HALF_CYCLE_CAL_SET                                                    0x18008948
#define DC_PHY_HALF_CYCLE_CAL_SET_reg_addr                                           "0xB8008948"
#define DC_PHY_HALF_CYCLE_CAL_SET_reg                                                0xB8008948
#define set_DC_PHY_HALF_CYCLE_CAL_SET_reg(data)   (*((volatile unsigned int*) DC_PHY_HALF_CYCLE_CAL_SET_reg)=data)
#define get_DC_PHY_HALF_CYCLE_CAL_SET_reg   (*((volatile unsigned int*) DC_PHY_HALF_CYCLE_CAL_SET_reg))
#define DC_PHY_HALF_CYCLE_CAL_SET_inst_adr                                           "0x0052"
#define DC_PHY_HALF_CYCLE_CAL_SET_inst                                               0x0052
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_3_shift                     (18)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_3_mask                      (0x00FC0000)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_3(data)                     (0x00FC0000&((data)<<18))
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_3_src(data)                 ((0x00FC0000&(data))>>18)
#define DC_PHY_HALF_CYCLE_CAL_SET_get_half_cycle_cal_setting_3(data)                 ((0x00FC0000&(data))>>18)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_2_shift                     (12)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_2_mask                      (0x0003F000)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_2(data)                     (0x0003F000&((data)<<12))
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_2_src(data)                 ((0x0003F000&(data))>>12)
#define DC_PHY_HALF_CYCLE_CAL_SET_get_half_cycle_cal_setting_2(data)                 ((0x0003F000&(data))>>12)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_1_shift                     (6)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_1_mask                      (0x00000FC0)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_1(data)                     (0x00000FC0&((data)<<6))
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_1_src(data)                 ((0x00000FC0&(data))>>6)
#define DC_PHY_HALF_CYCLE_CAL_SET_get_half_cycle_cal_setting_1(data)                 ((0x00000FC0&(data))>>6)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_0_shift                     (0)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_0_mask                      (0x0000003F)
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_0(data)                     (0x0000003F&((data)<<0))
#define DC_PHY_HALF_CYCLE_CAL_SET_half_cycle_cal_setting_0_src(data)                 ((0x0000003F&(data))>>0)
#define DC_PHY_HALF_CYCLE_CAL_SET_get_half_cycle_cal_setting_0(data)                 ((0x0000003F&(data))>>0)


#define DC_PHY_PUPD_DLY_NUM                                                          0x1800894C
#define DC_PHY_PUPD_DLY_NUM_reg_addr                                                 "0xB800894C"
#define DC_PHY_PUPD_DLY_NUM_reg                                                      0xB800894C
#define set_DC_PHY_PUPD_DLY_NUM_reg(data)   (*((volatile unsigned int*) DC_PHY_PUPD_DLY_NUM_reg)=data)
#define get_DC_PHY_PUPD_DLY_NUM_reg   (*((volatile unsigned int*) DC_PHY_PUPD_DLY_NUM_reg))
#define DC_PHY_PUPD_DLY_NUM_inst_adr                                                 "0x0053"
#define DC_PHY_PUPD_DLY_NUM_inst                                                     0x0053
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_sel_shift                                   (20)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_sel_mask                                    (0x00300000)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_sel(data)                                   (0x00300000&((data)<<20))
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_sel_src(data)                               ((0x00300000&(data))>>20)
#define DC_PHY_PUPD_DLY_NUM_get_pupd_dly_num_sel(data)                               ((0x00300000&(data))>>20)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_3_shift                                     (15)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_3_mask                                      (0x000F8000)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_3(data)                                     (0x000F8000&((data)<<15))
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_3_src(data)                                 ((0x000F8000&(data))>>15)
#define DC_PHY_PUPD_DLY_NUM_get_pupd_dly_num_3(data)                                 ((0x000F8000&(data))>>15)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_2_shift                                     (10)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_2_mask                                      (0x00007C00)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_2(data)                                     (0x00007C00&((data)<<10))
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_2_src(data)                                 ((0x00007C00&(data))>>10)
#define DC_PHY_PUPD_DLY_NUM_get_pupd_dly_num_2(data)                                 ((0x00007C00&(data))>>10)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_1_shift                                     (5)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_1_mask                                      (0x000003E0)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_1(data)                                     (0x000003E0&((data)<<5))
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_1_src(data)                                 ((0x000003E0&(data))>>5)
#define DC_PHY_PUPD_DLY_NUM_get_pupd_dly_num_1(data)                                 ((0x000003E0&(data))>>5)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_0_shift                                     (0)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_0_mask                                      (0x0000001F)
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_0(data)                                     (0x0000001F&((data)<<0))
#define DC_PHY_PUPD_DLY_NUM_pupd_dly_num_0_src(data)                                 ((0x0000001F&(data))>>0)
#define DC_PHY_PUPD_DLY_NUM_get_pupd_dly_num_0(data)                                 ((0x0000001F&(data))>>0)


#define DC_PHY_DQS_DLYN                                                              0x18008950
#define DC_PHY_DQS_DLYN_reg_addr                                                     "0xB8008950"
#define DC_PHY_DQS_DLYN_reg                                                          0xB8008950
#define set_DC_PHY_DQS_DLYN_reg(data)   (*((volatile unsigned int*) DC_PHY_DQS_DLYN_reg)=data)
#define get_DC_PHY_DQS_DLYN_reg   (*((volatile unsigned int*) DC_PHY_DQS_DLYN_reg))
#define DC_PHY_DQS_DLYN_inst_adr                                                     "0x0054"
#define DC_PHY_DQS_DLYN_inst                                                         0x0054
#define DC_PHY_DQS_DLYN_dqs3_dlyn_shift                                              (18)
#define DC_PHY_DQS_DLYN_dqs3_dlyn_mask                                               (0x00FC0000)
#define DC_PHY_DQS_DLYN_dqs3_dlyn(data)                                              (0x00FC0000&((data)<<18))
#define DC_PHY_DQS_DLYN_dqs3_dlyn_src(data)                                          ((0x00FC0000&(data))>>18)
#define DC_PHY_DQS_DLYN_get_dqs3_dlyn(data)                                          ((0x00FC0000&(data))>>18)
#define DC_PHY_DQS_DLYN_dqs2_dlyn_shift                                              (12)
#define DC_PHY_DQS_DLYN_dqs2_dlyn_mask                                               (0x0003F000)
#define DC_PHY_DQS_DLYN_dqs2_dlyn(data)                                              (0x0003F000&((data)<<12))
#define DC_PHY_DQS_DLYN_dqs2_dlyn_src(data)                                          ((0x0003F000&(data))>>12)
#define DC_PHY_DQS_DLYN_get_dqs2_dlyn(data)                                          ((0x0003F000&(data))>>12)
#define DC_PHY_DQS_DLYN_dqs1_dlyn_shift                                              (6)
#define DC_PHY_DQS_DLYN_dqs1_dlyn_mask                                               (0x00000FC0)
#define DC_PHY_DQS_DLYN_dqs1_dlyn(data)                                              (0x00000FC0&((data)<<6))
#define DC_PHY_DQS_DLYN_dqs1_dlyn_src(data)                                          ((0x00000FC0&(data))>>6)
#define DC_PHY_DQS_DLYN_get_dqs1_dlyn(data)                                          ((0x00000FC0&(data))>>6)
#define DC_PHY_DQS_DLYN_dqs0_dlyn_shift                                              (0)
#define DC_PHY_DQS_DLYN_dqs0_dlyn_mask                                               (0x0000003F)
#define DC_PHY_DQS_DLYN_dqs0_dlyn(data)                                              (0x0000003F&((data)<<0))
#define DC_PHY_DQS_DLYN_dqs0_dlyn_src(data)                                          ((0x0000003F&(data))>>0)
#define DC_PHY_DQS_DLYN_get_dqs0_dlyn(data)                                          ((0x0000003F&(data))>>0)


#define DC_PHY_CAL_MODE                                                              0x18008954
#define DC_PHY_CAL_MODE_reg_addr                                                     "0xB8008954"
#define DC_PHY_CAL_MODE_reg                                                          0xB8008954
#define set_DC_PHY_CAL_MODE_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_MODE_reg)=data)
#define get_DC_PHY_CAL_MODE_reg   (*((volatile unsigned int*) DC_PHY_CAL_MODE_reg))
#define DC_PHY_CAL_MODE_inst_adr                                                     "0x0055"
#define DC_PHY_CAL_MODE_inst                                                         0x0055
#define DC_PHY_CAL_MODE_cal_all_rang_noml_shift                                      (8)
#define DC_PHY_CAL_MODE_cal_all_rang_noml_mask                                       (0x00000100)
#define DC_PHY_CAL_MODE_cal_all_rang_noml(data)                                      (0x00000100&((data)<<8))
#define DC_PHY_CAL_MODE_cal_all_rang_noml_src(data)                                  ((0x00000100&(data))>>8)
#define DC_PHY_CAL_MODE_get_cal_all_rang_noml(data)                                  ((0x00000100&(data))>>8)
#define DC_PHY_CAL_MODE_cal_all_rang_init_shift                                      (7)
#define DC_PHY_CAL_MODE_cal_all_rang_init_mask                                       (0x00000080)
#define DC_PHY_CAL_MODE_cal_all_rang_init(data)                                      (0x00000080&((data)<<7))
#define DC_PHY_CAL_MODE_cal_all_rang_init_src(data)                                  ((0x00000080&(data))>>7)
#define DC_PHY_CAL_MODE_get_cal_all_rang_init(data)                                  ((0x00000080&(data))>>7)
#define DC_PHY_CAL_MODE_auto_cal_en_noml_shift                                       (6)
#define DC_PHY_CAL_MODE_auto_cal_en_noml_mask                                        (0x00000040)
#define DC_PHY_CAL_MODE_auto_cal_en_noml(data)                                       (0x00000040&((data)<<6))
#define DC_PHY_CAL_MODE_auto_cal_en_noml_src(data)                                   ((0x00000040&(data))>>6)
#define DC_PHY_CAL_MODE_get_auto_cal_en_noml(data)                                   ((0x00000040&(data))>>6)
#define DC_PHY_CAL_MODE_auto_cal_en_init_shift                                       (5)
#define DC_PHY_CAL_MODE_auto_cal_en_init_mask                                        (0x00000020)
#define DC_PHY_CAL_MODE_auto_cal_en_init(data)                                       (0x00000020&((data)<<5))
#define DC_PHY_CAL_MODE_auto_cal_en_init_src(data)                                   ((0x00000020&(data))>>5)
#define DC_PHY_CAL_MODE_get_auto_cal_en_init(data)                                   ((0x00000020&(data))>>5)
#define DC_PHY_CAL_MODE_dqs_val_sel_shift                                            (4)
#define DC_PHY_CAL_MODE_dqs_val_sel_mask                                             (0x00000010)
#define DC_PHY_CAL_MODE_dqs_val_sel(data)                                            (0x00000010&((data)<<4))
#define DC_PHY_CAL_MODE_dqs_val_sel_src(data)                                        ((0x00000010&(data))>>4)
#define DC_PHY_CAL_MODE_get_dqs_val_sel(data)                                        ((0x00000010&(data))>>4)
#define DC_PHY_CAL_MODE_cal_mode_shift                                               (1)
#define DC_PHY_CAL_MODE_cal_mode_mask                                                (0x0000000E)
#define DC_PHY_CAL_MODE_cal_mode(data)                                               (0x0000000E&((data)<<1))
#define DC_PHY_CAL_MODE_cal_mode_src(data)                                           ((0x0000000E&(data))>>1)
#define DC_PHY_CAL_MODE_get_cal_mode(data)                                           ((0x0000000E&(data))>>1)
#define DC_PHY_CAL_MODE_dq_sel_shift                                                 (0)
#define DC_PHY_CAL_MODE_dq_sel_mask                                                  (0x00000001)
#define DC_PHY_CAL_MODE_dq_sel(data)                                                 (0x00000001&((data)<<0))
#define DC_PHY_CAL_MODE_dq_sel_src(data)                                             ((0x00000001&(data))>>0)
#define DC_PHY_CAL_MODE_get_dq_sel(data)                                             ((0x00000001&(data))>>0)


#define DC_PHY_CAL_ADDR                                                              0x18008958
#define DC_PHY_CAL_ADDR_reg_addr                                                     "0xB8008958"
#define DC_PHY_CAL_ADDR_reg                                                          0xB8008958
#define set_DC_PHY_CAL_ADDR_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_ADDR_reg)=data)
#define get_DC_PHY_CAL_ADDR_reg   (*((volatile unsigned int*) DC_PHY_CAL_ADDR_reg))
#define DC_PHY_CAL_ADDR_inst_adr                                                     "0x0056"
#define DC_PHY_CAL_ADDR_inst                                                         0x0056
#define DC_PHY_CAL_ADDR_cal_ba_shift                                                 (23)
#define DC_PHY_CAL_ADDR_cal_ba_mask                                                  (0x03800000)
#define DC_PHY_CAL_ADDR_cal_ba(data)                                                 (0x03800000&((data)<<23))
#define DC_PHY_CAL_ADDR_cal_ba_src(data)                                             ((0x03800000&(data))>>23)
#define DC_PHY_CAL_ADDR_get_cal_ba(data)                                             ((0x03800000&(data))>>23)
#define DC_PHY_CAL_ADDR_cal_col_shift                                                (13)
#define DC_PHY_CAL_ADDR_cal_col_mask                                                 (0x007FE000)
#define DC_PHY_CAL_ADDR_cal_col(data)                                                (0x007FE000&((data)<<13))
#define DC_PHY_CAL_ADDR_cal_col_src(data)                                            ((0x007FE000&(data))>>13)
#define DC_PHY_CAL_ADDR_get_cal_col(data)                                            ((0x007FE000&(data))>>13)
#define DC_PHY_CAL_ADDR_cal_row_shift                                                (0)
#define DC_PHY_CAL_ADDR_cal_row_mask                                                 (0x00001FFF)
#define DC_PHY_CAL_ADDR_cal_row(data)                                                (0x00001FFF&((data)<<0))
#define DC_PHY_CAL_ADDR_cal_row_src(data)                                            ((0x00001FFF&(data))>>0)
#define DC_PHY_CAL_ADDR_get_cal_row(data)                                            ((0x00001FFF&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_0                                                      0x1800895c
#define DC_PHY_RESULT_CAL_TAP_0_reg_addr                                             "0xB800895C"
#define DC_PHY_RESULT_CAL_TAP_0_reg                                                  0xB800895C
#define set_DC_PHY_RESULT_CAL_TAP_0_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_0_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_0_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_0_reg))
#define DC_PHY_RESULT_CAL_TAP_0_inst_adr                                             "0x0057"
#define DC_PHY_RESULT_CAL_TAP_0_inst                                                 0x0057
#define DC_PHY_RESULT_CAL_TAP_0_end_done_01_p_shift                                  (19)
#define DC_PHY_RESULT_CAL_TAP_0_end_done_01_p_mask                                   (0x00080000)
#define DC_PHY_RESULT_CAL_TAP_0_end_done_01_p(data)                                  (0x00080000&((data)<<19))
#define DC_PHY_RESULT_CAL_TAP_0_end_done_01_p_src(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_0_get_end_done_01_p(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_0_start_done_01_p_shift                                (18)
#define DC_PHY_RESULT_CAL_TAP_0_start_done_01_p_mask                                 (0x00040000)
#define DC_PHY_RESULT_CAL_TAP_0_start_done_01_p(data)                                (0x00040000&((data)<<18))
#define DC_PHY_RESULT_CAL_TAP_0_start_done_01_p_src(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_0_get_start_done_01_p(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_0_best_dly_01_p_shift                                  (12)
#define DC_PHY_RESULT_CAL_TAP_0_best_dly_01_p_mask                                   (0x0003F000)
#define DC_PHY_RESULT_CAL_TAP_0_best_dly_01_p(data)                                  (0x0003F000&((data)<<12))
#define DC_PHY_RESULT_CAL_TAP_0_best_dly_01_p_src(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_0_get_best_dly_01_p(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_0_end_pt_01_p_shift                                    (6)
#define DC_PHY_RESULT_CAL_TAP_0_end_pt_01_p_mask                                     (0x00000FC0)
#define DC_PHY_RESULT_CAL_TAP_0_end_pt_01_p(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_RESULT_CAL_TAP_0_end_pt_01_p_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_0_get_end_pt_01_p(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_0_start_pt_01_p_shift                                  (0)
#define DC_PHY_RESULT_CAL_TAP_0_start_pt_01_p_mask                                   (0x0000003F)
#define DC_PHY_RESULT_CAL_TAP_0_start_pt_01_p(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_0_start_pt_01_p_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_0_get_start_pt_01_p(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_1                                                      0x18008960
#define DC_PHY_RESULT_CAL_TAP_1_reg_addr                                             "0xB8008960"
#define DC_PHY_RESULT_CAL_TAP_1_reg                                                  0xB8008960
#define set_DC_PHY_RESULT_CAL_TAP_1_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_1_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_1_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_1_reg))
#define DC_PHY_RESULT_CAL_TAP_1_inst_adr                                             "0x0058"
#define DC_PHY_RESULT_CAL_TAP_1_inst                                                 0x0058
#define DC_PHY_RESULT_CAL_TAP_1_end_done_01_n_shift                                  (19)
#define DC_PHY_RESULT_CAL_TAP_1_end_done_01_n_mask                                   (0x00080000)
#define DC_PHY_RESULT_CAL_TAP_1_end_done_01_n(data)                                  (0x00080000&((data)<<19))
#define DC_PHY_RESULT_CAL_TAP_1_end_done_01_n_src(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_1_get_end_done_01_n(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_1_start_done_01_n_shift                                (18)
#define DC_PHY_RESULT_CAL_TAP_1_start_done_01_n_mask                                 (0x00040000)
#define DC_PHY_RESULT_CAL_TAP_1_start_done_01_n(data)                                (0x00040000&((data)<<18))
#define DC_PHY_RESULT_CAL_TAP_1_start_done_01_n_src(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_1_get_start_done_01_n(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_1_best_dly_01_n_shift                                  (12)
#define DC_PHY_RESULT_CAL_TAP_1_best_dly_01_n_mask                                   (0x0003F000)
#define DC_PHY_RESULT_CAL_TAP_1_best_dly_01_n(data)                                  (0x0003F000&((data)<<12))
#define DC_PHY_RESULT_CAL_TAP_1_best_dly_01_n_src(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_1_get_best_dly_01_n(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_1_end_pt_01_n_shift                                    (6)
#define DC_PHY_RESULT_CAL_TAP_1_end_pt_01_n_mask                                     (0x00000FC0)
#define DC_PHY_RESULT_CAL_TAP_1_end_pt_01_n(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_RESULT_CAL_TAP_1_end_pt_01_n_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_1_get_end_pt_01_n(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_1_start_pt_01_n_shift                                  (0)
#define DC_PHY_RESULT_CAL_TAP_1_start_pt_01_n_mask                                   (0x0000003F)
#define DC_PHY_RESULT_CAL_TAP_1_start_pt_01_n(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_1_start_pt_01_n_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_1_get_start_pt_01_n(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_2                                                      0x18008964
#define DC_PHY_RESULT_CAL_TAP_2_reg_addr                                             "0xB8008964"
#define DC_PHY_RESULT_CAL_TAP_2_reg                                                  0xB8008964
#define set_DC_PHY_RESULT_CAL_TAP_2_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_2_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_2_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_2_reg))
#define DC_PHY_RESULT_CAL_TAP_2_inst_adr                                             "0x0059"
#define DC_PHY_RESULT_CAL_TAP_2_inst                                                 0x0059
#define DC_PHY_RESULT_CAL_TAP_2_end_done_23_p_shift                                  (19)
#define DC_PHY_RESULT_CAL_TAP_2_end_done_23_p_mask                                   (0x00080000)
#define DC_PHY_RESULT_CAL_TAP_2_end_done_23_p(data)                                  (0x00080000&((data)<<19))
#define DC_PHY_RESULT_CAL_TAP_2_end_done_23_p_src(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_2_get_end_done_23_p(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_2_start_done_23_p_shift                                (18)
#define DC_PHY_RESULT_CAL_TAP_2_start_done_23_p_mask                                 (0x00040000)
#define DC_PHY_RESULT_CAL_TAP_2_start_done_23_p(data)                                (0x00040000&((data)<<18))
#define DC_PHY_RESULT_CAL_TAP_2_start_done_23_p_src(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_2_get_start_done_23_p(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_2_best_dly_23_p_shift                                  (12)
#define DC_PHY_RESULT_CAL_TAP_2_best_dly_23_p_mask                                   (0x0003F000)
#define DC_PHY_RESULT_CAL_TAP_2_best_dly_23_p(data)                                  (0x0003F000&((data)<<12))
#define DC_PHY_RESULT_CAL_TAP_2_best_dly_23_p_src(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_2_get_best_dly_23_p(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_2_end_pt_23_p_shift                                    (6)
#define DC_PHY_RESULT_CAL_TAP_2_end_pt_23_p_mask                                     (0x00000FC0)
#define DC_PHY_RESULT_CAL_TAP_2_end_pt_23_p(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_RESULT_CAL_TAP_2_end_pt_23_p_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_2_get_end_pt_23_p(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_2_start_pt_23_p_shift                                  (0)
#define DC_PHY_RESULT_CAL_TAP_2_start_pt_23_p_mask                                   (0x0000003F)
#define DC_PHY_RESULT_CAL_TAP_2_start_pt_23_p(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_2_start_pt_23_p_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_2_get_start_pt_23_p(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_3                                                      0x18008968
#define DC_PHY_RESULT_CAL_TAP_3_reg_addr                                             "0xB8008968"
#define DC_PHY_RESULT_CAL_TAP_3_reg                                                  0xB8008968
#define set_DC_PHY_RESULT_CAL_TAP_3_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_3_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_3_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_3_reg))
#define DC_PHY_RESULT_CAL_TAP_3_inst_adr                                             "0x005A"
#define DC_PHY_RESULT_CAL_TAP_3_inst                                                 0x005A
#define DC_PHY_RESULT_CAL_TAP_3_end_done_23_n_shift                                  (19)
#define DC_PHY_RESULT_CAL_TAP_3_end_done_23_n_mask                                   (0x00080000)
#define DC_PHY_RESULT_CAL_TAP_3_end_done_23_n(data)                                  (0x00080000&((data)<<19))
#define DC_PHY_RESULT_CAL_TAP_3_end_done_23_n_src(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_3_get_end_done_23_n(data)                              ((0x00080000&(data))>>19)
#define DC_PHY_RESULT_CAL_TAP_3_start_done_23_n_shift                                (18)
#define DC_PHY_RESULT_CAL_TAP_3_start_done_23_n_mask                                 (0x00040000)
#define DC_PHY_RESULT_CAL_TAP_3_start_done_23_n(data)                                (0x00040000&((data)<<18))
#define DC_PHY_RESULT_CAL_TAP_3_start_done_23_n_src(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_3_get_start_done_23_n(data)                            ((0x00040000&(data))>>18)
#define DC_PHY_RESULT_CAL_TAP_3_best_dly_23_n_shift                                  (12)
#define DC_PHY_RESULT_CAL_TAP_3_best_dly_23_n_mask                                   (0x0003F000)
#define DC_PHY_RESULT_CAL_TAP_3_best_dly_23_n(data)                                  (0x0003F000&((data)<<12))
#define DC_PHY_RESULT_CAL_TAP_3_best_dly_23_n_src(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_3_get_best_dly_23_n(data)                              ((0x0003F000&(data))>>12)
#define DC_PHY_RESULT_CAL_TAP_3_end_pt_23_n_shift                                    (6)
#define DC_PHY_RESULT_CAL_TAP_3_end_pt_23_n_mask                                     (0x00000FC0)
#define DC_PHY_RESULT_CAL_TAP_3_end_pt_23_n(data)                                    (0x00000FC0&((data)<<6))
#define DC_PHY_RESULT_CAL_TAP_3_end_pt_23_n_src(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_3_get_end_pt_23_n(data)                                ((0x00000FC0&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_3_start_pt_23_n_shift                                  (0)
#define DC_PHY_RESULT_CAL_TAP_3_start_pt_23_n_mask                                   (0x0000003F)
#define DC_PHY_RESULT_CAL_TAP_3_start_pt_23_n(data)                                  (0x0000003F&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_3_start_pt_23_n_src(data)                              ((0x0000003F&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_3_get_start_pt_23_n(data)                              ((0x0000003F&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_4                                                      0x1800896c
#define DC_PHY_RESULT_CAL_TAP_4_reg_addr                                             "0xB800896C"
#define DC_PHY_RESULT_CAL_TAP_4_reg                                                  0xB800896C
#define set_DC_PHY_RESULT_CAL_TAP_4_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_4_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_4_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_4_reg))
#define DC_PHY_RESULT_CAL_TAP_4_inst_adr                                             "0x005B"
#define DC_PHY_RESULT_CAL_TAP_4_inst                                                 0x005B
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_odt_shift                            (7)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_odt_mask                             (0x00000080)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_odt(data)                            (0x00000080&((data)<<7))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_odt_src(data)                        ((0x00000080&(data))>>7)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_23_n_odt(data)                        ((0x00000080&(data))>>7)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_odt_shift                            (6)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_odt_mask                             (0x00000040)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_odt(data)                            (0x00000040&((data)<<6))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_odt_src(data)                        ((0x00000040&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_23_p_odt(data)                        ((0x00000040&(data))>>6)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_odt_shift                            (5)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_odt_mask                             (0x00000020)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_odt(data)                            (0x00000020&((data)<<5))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_odt_src(data)                        ((0x00000020&(data))>>5)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_01_n_odt(data)                        ((0x00000020&(data))>>5)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_odt_shift                            (4)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_odt_mask                             (0x00000010)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_odt(data)                            (0x00000010&((data)<<4))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_odt_src(data)                        ((0x00000010&(data))>>4)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_01_p_odt(data)                        ((0x00000010&(data))>>4)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_dt_shift                             (3)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_dt_mask                              (0x00000008)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_dt(data)                             (0x00000008&((data)<<3))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_n_dt_src(data)                         ((0x00000008&(data))>>3)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_23_n_dt(data)                         ((0x00000008&(data))>>3)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_dt_shift                             (2)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_dt_mask                              (0x00000004)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_dt(data)                             (0x00000004&((data)<<2))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_23_p_dt_src(data)                         ((0x00000004&(data))>>2)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_23_p_dt(data)                         ((0x00000004&(data))>>2)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_dt_shift                             (1)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_dt_mask                              (0x00000002)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_dt(data)                             (0x00000002&((data)<<1))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_n_dt_src(data)                         ((0x00000002&(data))>>1)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_01_n_dt(data)                         ((0x00000002&(data))>>1)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_dt_shift                             (0)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_dt_mask                              (0x00000001)
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_dt(data)                             (0x00000001&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_4_wk_big_thr_01_p_dt_src(data)                         ((0x00000001&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_4_get_wk_big_thr_01_p_dt(data)                         ((0x00000001&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_5                                                      0x18008970
#define DC_PHY_RESULT_CAL_TAP_5_reg_addr                                             "0xB8008970"
#define DC_PHY_RESULT_CAL_TAP_5_reg                                                  0xB8008970
#define set_DC_PHY_RESULT_CAL_TAP_5_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_5_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_5_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_5_reg))
#define DC_PHY_RESULT_CAL_TAP_5_inst_adr                                             "0x005C"
#define DC_PHY_RESULT_CAL_TAP_5_inst                                                 0x005C
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_odt_shift                               (24)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_odt_mask                                (0x3F000000)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_odt(data)                               (0x3F000000&((data)<<24))
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_odt_src(data)                           ((0x3F000000&(data))>>24)
#define DC_PHY_RESULT_CAL_TAP_5_get_best_wk_01_n_odt(data)                           ((0x3F000000&(data))>>24)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_odt_shift                               (16)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_odt_mask                                (0x003F0000)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_odt(data)                               (0x003F0000&((data)<<16))
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_odt_src(data)                           ((0x003F0000&(data))>>16)
#define DC_PHY_RESULT_CAL_TAP_5_get_best_wk_01_p_odt(data)                           ((0x003F0000&(data))>>16)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_dt_shift                                (8)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_dt_mask                                 (0x00003F00)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_dt(data)                                (0x00003F00&((data)<<8))
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_n_dt_src(data)                            ((0x00003F00&(data))>>8)
#define DC_PHY_RESULT_CAL_TAP_5_get_best_wk_01_n_dt(data)                            ((0x00003F00&(data))>>8)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_dt_shift                                (0)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_dt_mask                                 (0x0000003F)
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_dt(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_5_best_wk_01_p_dt_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_5_get_best_wk_01_p_dt(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_RESULT_CAL_TAP_6                                                      0x18008974
#define DC_PHY_RESULT_CAL_TAP_6_reg_addr                                             "0xB8008974"
#define DC_PHY_RESULT_CAL_TAP_6_reg                                                  0xB8008974
#define set_DC_PHY_RESULT_CAL_TAP_6_reg(data)   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_6_reg)=data)
#define get_DC_PHY_RESULT_CAL_TAP_6_reg   (*((volatile unsigned int*) DC_PHY_RESULT_CAL_TAP_6_reg))
#define DC_PHY_RESULT_CAL_TAP_6_inst_adr                                             "0x005D"
#define DC_PHY_RESULT_CAL_TAP_6_inst                                                 0x005D
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_odt_shift                               (24)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_odt_mask                                (0x3F000000)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_odt(data)                               (0x3F000000&((data)<<24))
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_odt_src(data)                           ((0x3F000000&(data))>>24)
#define DC_PHY_RESULT_CAL_TAP_6_get_best_wk_23_n_odt(data)                           ((0x3F000000&(data))>>24)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_odt_shift                               (16)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_odt_mask                                (0x003F0000)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_odt(data)                               (0x003F0000&((data)<<16))
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_odt_src(data)                           ((0x003F0000&(data))>>16)
#define DC_PHY_RESULT_CAL_TAP_6_get_best_wk_23_p_odt(data)                           ((0x003F0000&(data))>>16)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_dt_shift                                (8)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_dt_mask                                 (0x00003F00)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_dt(data)                                (0x00003F00&((data)<<8))
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_n_dt_src(data)                            ((0x00003F00&(data))>>8)
#define DC_PHY_RESULT_CAL_TAP_6_get_best_wk_23_n_dt(data)                            ((0x00003F00&(data))>>8)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_dt_shift                                (0)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_dt_mask                                 (0x0000003F)
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_dt(data)                                (0x0000003F&((data)<<0))
#define DC_PHY_RESULT_CAL_TAP_6_best_wk_23_p_dt_src(data)                            ((0x0000003F&(data))>>0)
#define DC_PHY_RESULT_CAL_TAP_6_get_best_wk_23_p_dt(data)                            ((0x0000003F&(data))>>0)


#define DC_PHY_SYNC_RST_WK                                                           0x18008978
#define DC_PHY_SYNC_RST_WK_reg_addr                                                  "0xB8008978"
#define DC_PHY_SYNC_RST_WK_reg                                                       0xB8008978
#define set_DC_PHY_SYNC_RST_WK_reg(data)   (*((volatile unsigned int*) DC_PHY_SYNC_RST_WK_reg)=data)
#define get_DC_PHY_SYNC_RST_WK_reg   (*((volatile unsigned int*) DC_PHY_SYNC_RST_WK_reg))
#define DC_PHY_SYNC_RST_WK_inst_adr                                                  "0x005E"
#define DC_PHY_SYNC_RST_WK_inst                                                      0x005E
#define DC_PHY_SYNC_RST_WK_odt_diff_threshold_shift                                  (16)
#define DC_PHY_SYNC_RST_WK_odt_diff_threshold_mask                                   (0x003F0000)
#define DC_PHY_SYNC_RST_WK_odt_diff_threshold(data)                                  (0x003F0000&((data)<<16))
#define DC_PHY_SYNC_RST_WK_odt_diff_threshold_src(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_SYNC_RST_WK_get_odt_diff_threshold(data)                              ((0x003F0000&(data))>>16)
#define DC_PHY_SYNC_RST_WK_duty_diff_threshold_shift                                 (8)
#define DC_PHY_SYNC_RST_WK_duty_diff_threshold_mask                                  (0x00003F00)
#define DC_PHY_SYNC_RST_WK_duty_diff_threshold(data)                                 (0x00003F00&((data)<<8))
#define DC_PHY_SYNC_RST_WK_duty_diff_threshold_src(data)                             ((0x00003F00&(data))>>8)
#define DC_PHY_SYNC_RST_WK_get_duty_diff_threshold(data)                             ((0x00003F00&(data))>>8)
#define DC_PHY_SYNC_RST_WK_cal_proc_done_shift                                       (7)
#define DC_PHY_SYNC_RST_WK_cal_proc_done_mask                                        (0x00000080)
#define DC_PHY_SYNC_RST_WK_cal_proc_done(data)                                       (0x00000080&((data)<<7))
#define DC_PHY_SYNC_RST_WK_cal_proc_done_src(data)                                   ((0x00000080&(data))>>7)
#define DC_PHY_SYNC_RST_WK_get_cal_proc_done(data)                                   ((0x00000080&(data))>>7)
#define DC_PHY_SYNC_RST_WK_wk_threshold_shift                                        (1)
#define DC_PHY_SYNC_RST_WK_wk_threshold_mask                                         (0x0000007E)
#define DC_PHY_SYNC_RST_WK_wk_threshold(data)                                        (0x0000007E&((data)<<1))
#define DC_PHY_SYNC_RST_WK_wk_threshold_src(data)                                    ((0x0000007E&(data))>>1)
#define DC_PHY_SYNC_RST_WK_get_wk_threshold(data)                                    ((0x0000007E&(data))>>1)
#define DC_PHY_SYNC_RST_WK_sync_rst_wk_thr_shift                                     (0)
#define DC_PHY_SYNC_RST_WK_sync_rst_wk_thr_mask                                      (0x00000001)
#define DC_PHY_SYNC_RST_WK_sync_rst_wk_thr(data)                                     (0x00000001&((data)<<0))
#define DC_PHY_SYNC_RST_WK_sync_rst_wk_thr_src(data)                                 ((0x00000001&(data))>>0)
#define DC_PHY_SYNC_RST_WK_get_sync_rst_wk_thr(data)                                 ((0x00000001&(data))>>0)


#define DC_PHY_SYNC_RST_FIFO_PT                                                      0x1800897C
#define DC_PHY_SYNC_RST_FIFO_PT_reg_addr                                             "0xB800897C"
#define DC_PHY_SYNC_RST_FIFO_PT_reg                                                  0xB800897C
#define set_DC_PHY_SYNC_RST_FIFO_PT_reg(data)   (*((volatile unsigned int*) DC_PHY_SYNC_RST_FIFO_PT_reg)=data)
#define get_DC_PHY_SYNC_RST_FIFO_PT_reg   (*((volatile unsigned int*) DC_PHY_SYNC_RST_FIFO_PT_reg))
#define DC_PHY_SYNC_RST_FIFO_PT_inst_adr                                             "0x005F"
#define DC_PHY_SYNC_RST_FIFO_PT_inst                                                 0x005F
#define DC_PHY_SYNC_RST_FIFO_PT_sync_rst_fifo_pt_shift                               (0)
#define DC_PHY_SYNC_RST_FIFO_PT_sync_rst_fifo_pt_mask                                (0x00000001)
#define DC_PHY_SYNC_RST_FIFO_PT_sync_rst_fifo_pt(data)                               (0x00000001&((data)<<0))
#define DC_PHY_SYNC_RST_FIFO_PT_sync_rst_fifo_pt_src(data)                           ((0x00000001&(data))>>0)
#define DC_PHY_SYNC_RST_FIFO_PT_get_sync_rst_fifo_pt(data)                           ((0x00000001&(data))>>0)


#define DC_PHY_CAL_OUT_DATA_A                                                        0x18008980
#define DC_PHY_CAL_OUT_DATA_A_reg_addr                                               "0xB8008980"
#define DC_PHY_CAL_OUT_DATA_A_reg                                                    0xB8008980
#define set_DC_PHY_CAL_OUT_DATA_A_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_OUT_DATA_A_reg)=data)
#define get_DC_PHY_CAL_OUT_DATA_A_reg   (*((volatile unsigned int*) DC_PHY_CAL_OUT_DATA_A_reg))
#define DC_PHY_CAL_OUT_DATA_A_inst_adr                                               "0x0060"
#define DC_PHY_CAL_OUT_DATA_A_inst                                                   0x0060
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_1_shift                                   (16)
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_1_mask                                    (0xFFFF0000)
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_1(data)                                   (0xFFFF0000&((data)<<16))
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_1_src(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_OUT_DATA_A_get_cal_out_data_1(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_0_shift                                   (0)
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_0_mask                                    (0x0000FFFF)
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_0(data)                                   (0x0000FFFF&((data)<<0))
#define DC_PHY_CAL_OUT_DATA_A_cal_out_data_0_src(data)                               ((0x0000FFFF&(data))>>0)
#define DC_PHY_CAL_OUT_DATA_A_get_cal_out_data_0(data)                               ((0x0000FFFF&(data))>>0)


#define DC_PHY_CAL_OUT_DATA_B                                                        0x18008984
#define DC_PHY_CAL_OUT_DATA_B_reg_addr                                               "0xB8008984"
#define DC_PHY_CAL_OUT_DATA_B_reg                                                    0xB8008984
#define set_DC_PHY_CAL_OUT_DATA_B_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_OUT_DATA_B_reg)=data)
#define get_DC_PHY_CAL_OUT_DATA_B_reg   (*((volatile unsigned int*) DC_PHY_CAL_OUT_DATA_B_reg))
#define DC_PHY_CAL_OUT_DATA_B_inst_adr                                               "0x0061"
#define DC_PHY_CAL_OUT_DATA_B_inst                                                   0x0061
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_3_shift                                   (16)
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_3_mask                                    (0xFFFF0000)
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_3(data)                                   (0xFFFF0000&((data)<<16))
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_3_src(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_OUT_DATA_B_get_cal_out_data_3(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_2_shift                                   (0)
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_2_mask                                    (0x0000FFFF)
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_2(data)                                   (0x0000FFFF&((data)<<0))
#define DC_PHY_CAL_OUT_DATA_B_cal_out_data_2_src(data)                               ((0x0000FFFF&(data))>>0)
#define DC_PHY_CAL_OUT_DATA_B_get_cal_out_data_2(data)                               ((0x0000FFFF&(data))>>0)


#define DC_PHY_CAL_GLD_DATA_A                                                        0x18008988
#define DC_PHY_CAL_GLD_DATA_A_reg_addr                                               "0xB8008988"
#define DC_PHY_CAL_GLD_DATA_A_reg                                                    0xB8008988
#define set_DC_PHY_CAL_GLD_DATA_A_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_GLD_DATA_A_reg)=data)
#define get_DC_PHY_CAL_GLD_DATA_A_reg   (*((volatile unsigned int*) DC_PHY_CAL_GLD_DATA_A_reg))
#define DC_PHY_CAL_GLD_DATA_A_inst_adr                                               "0x0062"
#define DC_PHY_CAL_GLD_DATA_A_inst                                                   0x0062
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_1_shift                                   (16)
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_1_mask                                    (0xFFFF0000)
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_1(data)                                   (0xFFFF0000&((data)<<16))
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_1_src(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_GLD_DATA_A_get_cal_gld_data_1(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_0_shift                                   (0)
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_0_mask                                    (0x0000FFFF)
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_0(data)                                   (0x0000FFFF&((data)<<0))
#define DC_PHY_CAL_GLD_DATA_A_cal_gld_data_0_src(data)                               ((0x0000FFFF&(data))>>0)
#define DC_PHY_CAL_GLD_DATA_A_get_cal_gld_data_0(data)                               ((0x0000FFFF&(data))>>0)


#define DC_PHY_CAL_GLD_DATA_B                                                        0x1800898C
#define DC_PHY_CAL_GLD_DATA_B_reg_addr                                               "0xB800898C"
#define DC_PHY_CAL_GLD_DATA_B_reg                                                    0xB800898C
#define set_DC_PHY_CAL_GLD_DATA_B_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_GLD_DATA_B_reg)=data)
#define get_DC_PHY_CAL_GLD_DATA_B_reg   (*((volatile unsigned int*) DC_PHY_CAL_GLD_DATA_B_reg))
#define DC_PHY_CAL_GLD_DATA_B_inst_adr                                               "0x0063"
#define DC_PHY_CAL_GLD_DATA_B_inst                                                   0x0063
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_3_shift                                   (16)
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_3_mask                                    (0xFFFF0000)
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_3(data)                                   (0xFFFF0000&((data)<<16))
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_3_src(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_GLD_DATA_B_get_cal_gld_data_3(data)                               ((0xFFFF0000&(data))>>16)
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_2_shift                                   (0)
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_2_mask                                    (0x0000FFFF)
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_2(data)                                   (0x0000FFFF&((data)<<0))
#define DC_PHY_CAL_GLD_DATA_B_cal_gld_data_2_src(data)                               ((0x0000FFFF&(data))>>0)
#define DC_PHY_CAL_GLD_DATA_B_get_cal_gld_data_2(data)                               ((0x0000FFFF&(data))>>0)


#define DC_PHY_DT_SET0                                                               0x18008a00
#define DC_PHY_DT_SET0_reg_addr                                                      "0xB8008A00"
#define DC_PHY_DT_SET0_reg                                                           0xB8008A00
#define set_DC_PHY_DT_SET0_reg(data)   (*((volatile unsigned int*) DC_PHY_DT_SET0_reg)=data)
#define get_DC_PHY_DT_SET0_reg   (*((volatile unsigned int*) DC_PHY_DT_SET0_reg))
#define DC_PHY_DT_SET0_inst_adr                                                      "0x0080"
#define DC_PHY_DT_SET0_inst                                                          0x0080
#define DC_PHY_DT_SET0_dt_set_0_shift                                                (0)
#define DC_PHY_DT_SET0_dt_set_0_mask                                                 (0xFFFFFFFF)
#define DC_PHY_DT_SET0_dt_set_0(data)                                                (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DT_SET0_dt_set_0_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DT_SET0_get_dt_set_0(data)                                            ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DT_SET1                                                               0x18008a04
#define DC_PHY_DT_SET1_reg_addr                                                      "0xB8008A04"
#define DC_PHY_DT_SET1_reg                                                           0xB8008A04
#define set_DC_PHY_DT_SET1_reg(data)   (*((volatile unsigned int*) DC_PHY_DT_SET1_reg)=data)
#define get_DC_PHY_DT_SET1_reg   (*((volatile unsigned int*) DC_PHY_DT_SET1_reg))
#define DC_PHY_DT_SET1_inst_adr                                                      "0x0081"
#define DC_PHY_DT_SET1_inst                                                          0x0081
#define DC_PHY_DT_SET1_dt_set_1_shift                                                (0)
#define DC_PHY_DT_SET1_dt_set_1_mask                                                 (0xFFFFFFFF)
#define DC_PHY_DT_SET1_dt_set_1(data)                                                (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DT_SET1_dt_set_1_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DT_SET1_get_dt_set_1(data)                                            ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DT_SET2                                                               0x18008a08
#define DC_PHY_DT_SET2_reg_addr                                                      "0xB8008A08"
#define DC_PHY_DT_SET2_reg                                                           0xB8008A08
#define set_DC_PHY_DT_SET2_reg(data)   (*((volatile unsigned int*) DC_PHY_DT_SET2_reg)=data)
#define get_DC_PHY_DT_SET2_reg   (*((volatile unsigned int*) DC_PHY_DT_SET2_reg))
#define DC_PHY_DT_SET2_inst_adr                                                      "0x0082"
#define DC_PHY_DT_SET2_inst                                                          0x0082
#define DC_PHY_DT_SET2_dt_set_2_shift                                                (0)
#define DC_PHY_DT_SET2_dt_set_2_mask                                                 (0xFFFFFFFF)
#define DC_PHY_DT_SET2_dt_set_2(data)                                                (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DT_SET2_dt_set_2_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DT_SET2_get_dt_set_2(data)                                            ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DT_SET3                                                               0x18008a0c
#define DC_PHY_DT_SET3_reg_addr                                                      "0xB8008A0C"
#define DC_PHY_DT_SET3_reg                                                           0xB8008A0C
#define set_DC_PHY_DT_SET3_reg(data)   (*((volatile unsigned int*) DC_PHY_DT_SET3_reg)=data)
#define get_DC_PHY_DT_SET3_reg   (*((volatile unsigned int*) DC_PHY_DT_SET3_reg))
#define DC_PHY_DT_SET3_inst_adr                                                      "0x0083"
#define DC_PHY_DT_SET3_inst                                                          0x0083
#define DC_PHY_DT_SET3_dt_set_3_shift                                                (0)
#define DC_PHY_DT_SET3_dt_set_3_mask                                                 (0xFFFFFFFF)
#define DC_PHY_DT_SET3_dt_set_3(data)                                                (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DT_SET3_dt_set_3_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DT_SET3_get_dt_set_3(data)                                            ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DT_SET4                                                               0x18008a10
#define DC_PHY_DT_SET4_reg_addr                                                      "0xB8008A10"
#define DC_PHY_DT_SET4_reg                                                           0xB8008A10
#define set_DC_PHY_DT_SET4_reg(data)   (*((volatile unsigned int*) DC_PHY_DT_SET4_reg)=data)
#define get_DC_PHY_DT_SET4_reg   (*((volatile unsigned int*) DC_PHY_DT_SET4_reg))
#define DC_PHY_DT_SET4_inst_adr                                                      "0x0084"
#define DC_PHY_DT_SET4_inst                                                          0x0084
#define DC_PHY_DT_SET4_dt_set_4_shift                                                (0)
#define DC_PHY_DT_SET4_dt_set_4_mask                                                 (0xFFFFFFFF)
#define DC_PHY_DT_SET4_dt_set_4(data)                                                (0xFFFFFFFF&((data)<<0))
#define DC_PHY_DT_SET4_dt_set_4_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_DT_SET4_get_dt_set_4(data)                                            ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_DT_CTL                                                                0x18008a14
#define DC_PHY_DT_CTL_reg_addr                                                       "0xB8008A14"
#define DC_PHY_DT_CTL_reg                                                            0xB8008A14
#define set_DC_PHY_DT_CTL_reg(data)   (*((volatile unsigned int*) DC_PHY_DT_CTL_reg)=data)
#define get_DC_PHY_DT_CTL_reg   (*((volatile unsigned int*) DC_PHY_DT_CTL_reg))
#define DC_PHY_DT_CTL_inst_adr                                                       "0x0085"
#define DC_PHY_DT_CTL_inst                                                           0x0085
#define DC_PHY_DT_CTL_update_dqs_shift                                               (1)
#define DC_PHY_DT_CTL_update_dqs_mask                                                (0x00000002)
#define DC_PHY_DT_CTL_update_dqs(data)                                               (0x00000002&((data)<<1))
#define DC_PHY_DT_CTL_update_dqs_src(data)                                           ((0x00000002&(data))>>1)
#define DC_PHY_DT_CTL_get_update_dqs(data)                                           ((0x00000002&(data))>>1)
#define DC_PHY_DT_CTL_update_dq_shift                                                (0)
#define DC_PHY_DT_CTL_update_dq_mask                                                 (0x00000001)
#define DC_PHY_DT_CTL_update_dq(data)                                                (0x00000001&((data)<<0))
#define DC_PHY_DT_CTL_update_dq_src(data)                                            ((0x00000001&(data))>>0)
#define DC_PHY_DT_CTL_get_update_dq(data)                                            ((0x00000001&(data))>>0)


#define DC_PHY_CAL_DT_RESULT                                                         0x18008a18
#define DC_PHY_CAL_DT_RESULT_reg_addr                                                "0xB8008A18"
#define DC_PHY_CAL_DT_RESULT_reg                                                     0xB8008A18
#define set_DC_PHY_CAL_DT_RESULT_reg(data)   (*((volatile unsigned int*) DC_PHY_CAL_DT_RESULT_reg)=data)
#define get_DC_PHY_CAL_DT_RESULT_reg   (*((volatile unsigned int*) DC_PHY_CAL_DT_RESULT_reg))
#define DC_PHY_CAL_DT_RESULT_inst_adr                                                "0x0086"
#define DC_PHY_CAL_DT_RESULT_inst                                                    0x0086
#define DC_PHY_CAL_DT_RESULT_odt_23_n_shift                                          (17)
#define DC_PHY_CAL_DT_RESULT_odt_23_n_mask                                           (0x000E0000)
#define DC_PHY_CAL_DT_RESULT_odt_23_n(data)                                          (0x000E0000&((data)<<17))
#define DC_PHY_CAL_DT_RESULT_odt_23_n_src(data)                                      ((0x000E0000&(data))>>17)
#define DC_PHY_CAL_DT_RESULT_get_odt_23_n(data)                                      ((0x000E0000&(data))>>17)
#define DC_PHY_CAL_DT_RESULT_odt_23_p_shift                                          (14)
#define DC_PHY_CAL_DT_RESULT_odt_23_p_mask                                           (0x0001C000)
#define DC_PHY_CAL_DT_RESULT_odt_23_p(data)                                          (0x0001C000&((data)<<14))
#define DC_PHY_CAL_DT_RESULT_odt_23_p_src(data)                                      ((0x0001C000&(data))>>14)
#define DC_PHY_CAL_DT_RESULT_get_odt_23_p(data)                                      ((0x0001C000&(data))>>14)
#define DC_PHY_CAL_DT_RESULT_odt_01_n_shift                                          (11)
#define DC_PHY_CAL_DT_RESULT_odt_01_n_mask                                           (0x00003800)
#define DC_PHY_CAL_DT_RESULT_odt_01_n(data)                                          (0x00003800&((data)<<11))
#define DC_PHY_CAL_DT_RESULT_odt_01_n_src(data)                                      ((0x00003800&(data))>>11)
#define DC_PHY_CAL_DT_RESULT_get_odt_01_n(data)                                      ((0x00003800&(data))>>11)
#define DC_PHY_CAL_DT_RESULT_odt_01_p_shift                                          (8)
#define DC_PHY_CAL_DT_RESULT_odt_01_p_mask                                           (0x00000700)
#define DC_PHY_CAL_DT_RESULT_odt_01_p(data)                                          (0x00000700&((data)<<8))
#define DC_PHY_CAL_DT_RESULT_odt_01_p_src(data)                                      ((0x00000700&(data))>>8)
#define DC_PHY_CAL_DT_RESULT_get_odt_01_p(data)                                      ((0x00000700&(data))>>8)
#define DC_PHY_CAL_DT_RESULT_indt_23_n_shift                                         (6)
#define DC_PHY_CAL_DT_RESULT_indt_23_n_mask                                          (0x000000C0)
#define DC_PHY_CAL_DT_RESULT_indt_23_n(data)                                         (0x000000C0&((data)<<6))
#define DC_PHY_CAL_DT_RESULT_indt_23_n_src(data)                                     ((0x000000C0&(data))>>6)
#define DC_PHY_CAL_DT_RESULT_get_indt_23_n(data)                                     ((0x000000C0&(data))>>6)
#define DC_PHY_CAL_DT_RESULT_indt_23_p_shift                                         (4)
#define DC_PHY_CAL_DT_RESULT_indt_23_p_mask                                          (0x00000030)
#define DC_PHY_CAL_DT_RESULT_indt_23_p(data)                                         (0x00000030&((data)<<4))
#define DC_PHY_CAL_DT_RESULT_indt_23_p_src(data)                                     ((0x00000030&(data))>>4)
#define DC_PHY_CAL_DT_RESULT_get_indt_23_p(data)                                     ((0x00000030&(data))>>4)
#define DC_PHY_CAL_DT_RESULT_indt_01_n_shift                                         (2)
#define DC_PHY_CAL_DT_RESULT_indt_01_n_mask                                          (0x0000000C)
#define DC_PHY_CAL_DT_RESULT_indt_01_n(data)                                         (0x0000000C&((data)<<2))
#define DC_PHY_CAL_DT_RESULT_indt_01_n_src(data)                                     ((0x0000000C&(data))>>2)
#define DC_PHY_CAL_DT_RESULT_get_indt_01_n(data)                                     ((0x0000000C&(data))>>2)
#define DC_PHY_CAL_DT_RESULT_indt_01_p_shift                                         (0)
#define DC_PHY_CAL_DT_RESULT_indt_01_p_mask                                          (0x00000003)
#define DC_PHY_CAL_DT_RESULT_indt_01_p(data)                                         (0x00000003&((data)<<0))
#define DC_PHY_CAL_DT_RESULT_indt_01_p_src(data)                                     ((0x00000003&(data))>>0)
#define DC_PHY_CAL_DT_RESULT_get_indt_01_p(data)                                     ((0x00000003&(data))>>0)


#define DC_PHY_ODT_TTC_SET0                                                          0x18008a20
#define DC_PHY_ODT_TTC_SET0_reg_addr                                                 "0xB8008A20"
#define DC_PHY_ODT_TTC_SET0_reg                                                      0xB8008A20
#define set_DC_PHY_ODT_TTC_SET0_reg(data)   (*((volatile unsigned int*) DC_PHY_ODT_TTC_SET0_reg)=data)
#define get_DC_PHY_ODT_TTC_SET0_reg   (*((volatile unsigned int*) DC_PHY_ODT_TTC_SET0_reg))
#define DC_PHY_ODT_TTC_SET0_inst_adr                                                 "0x0088"
#define DC_PHY_ODT_TTC_SET0_inst                                                     0x0088
#define DC_PHY_ODT_TTC_SET0_odt_ttc_set_0_shift                                      (0)
#define DC_PHY_ODT_TTC_SET0_odt_ttc_set_0_mask                                       (0x0000FFFF)
#define DC_PHY_ODT_TTC_SET0_odt_ttc_set_0(data)                                      (0x0000FFFF&((data)<<0))
#define DC_PHY_ODT_TTC_SET0_odt_ttc_set_0_src(data)                                  ((0x0000FFFF&(data))>>0)
#define DC_PHY_ODT_TTC_SET0_get_odt_ttc_set_0(data)                                  ((0x0000FFFF&(data))>>0)


#define DC_PHY_ODT_TTC_SET1                                                          0x18008a24
#define DC_PHY_ODT_TTC_SET1_reg_addr                                                 "0xB8008A24"
#define DC_PHY_ODT_TTC_SET1_reg                                                      0xB8008A24
#define set_DC_PHY_ODT_TTC_SET1_reg(data)   (*((volatile unsigned int*) DC_PHY_ODT_TTC_SET1_reg)=data)
#define get_DC_PHY_ODT_TTC_SET1_reg   (*((volatile unsigned int*) DC_PHY_ODT_TTC_SET1_reg))
#define DC_PHY_ODT_TTC_SET1_inst_adr                                                 "0x0089"
#define DC_PHY_ODT_TTC_SET1_inst                                                     0x0089
#define DC_PHY_ODT_TTC_SET1_odt_ttc_set_1_shift                                      (0)
#define DC_PHY_ODT_TTC_SET1_odt_ttc_set_1_mask                                       (0xFFFFFFFF)
#define DC_PHY_ODT_TTC_SET1_odt_ttc_set_1(data)                                      (0xFFFFFFFF&((data)<<0))
#define DC_PHY_ODT_TTC_SET1_odt_ttc_set_1_src(data)                                  ((0xFFFFFFFF&(data))>>0)
#define DC_PHY_ODT_TTC_SET1_get_odt_ttc_set_1(data)                                  ((0xFFFFFFFF&(data))>>0)


#define DC_PHY_ODT_TTF_SET0                                                          0x18008a28
#define DC_PHY_ODT_TTF_SET0_reg_addr                                                 "0xB8008A28"
#define DC_PHY_ODT_TTF_SET0_reg                                                      0xB8008A28
#define set_DC_PHY_ODT_TTF_SET0_reg(data)   (*((volatile unsigned int*) DC_PHY_ODT_TTF_SET0_reg)=data)
#define get_DC_PHY_ODT_TTF_SET0_reg   (*((volatile unsigned int*) DC_PHY_ODT_TTF_SET0_reg))
#define DC_PHY_ODT_TTF_SET0_inst_adr                                                 "0x008A"
#define DC_PHY_ODT_TTF_SET0_inst                                                     0x008A
#define DC_PHY_ODT_TTF_SET0_odt_ttf_set_0_shift                                      (0)
#define DC_PHY_ODT_TTF_SET0_odt_ttf_set_0_mask                                       (0x00FFFFFF)
#define DC_PHY_ODT_TTF_SET0_odt_ttf_set_0(data)                                      (0x00FFFFFF&((data)<<0))
#define DC_PHY_ODT_TTF_SET0_odt_ttf_set_0_src(data)                                  ((0x00FFFFFF&(data))>>0)
#define DC_PHY_ODT_TTF_SET0_get_odt_ttf_set_0(data)                                  ((0x00FFFFFF&(data))>>0)


#define DC_PHY_ODT_TTF_SET1                                                          0x18008a2c
#define DC_PHY_ODT_TTF_SET1_reg_addr                                                 "0xB8008A2C"
#define DC_PHY_ODT_TTF_SET1_reg                                                      0xB8008A2C
#define set_DC_PHY_ODT_TTF_SET1_reg(data)   (*((volatile unsigned int*) DC_PHY_ODT_TTF_SET1_reg)=data)
#define get_DC_PHY_ODT_TTF_SET1_reg   (*((volatile unsigned int*) DC_PHY_ODT_TTF_SET1_reg))
#define DC_PHY_ODT_TTF_SET1_inst_adr                                                 "0x008B"
#define DC_PHY_ODT_TTF_SET1_inst                                                     0x008B
#define DC_PHY_ODT_TTF_SET1_odt_ttf_set_1_shift                                      (0)
#define DC_PHY_ODT_TTF_SET1_odt_ttf_set_1_mask                                       (0x00FFFFFF)
#define DC_PHY_ODT_TTF_SET1_odt_ttf_set_1(data)                                      (0x00FFFFFF&((data)<<0))
#define DC_PHY_ODT_TTF_SET1_odt_ttf_set_1_src(data)                                  ((0x00FFFFFF&(data))>>0)
#define DC_PHY_ODT_TTF_SET1_get_odt_ttf_set_1(data)                                  ((0x00FFFFFF&(data))>>0)


#define DC_PHY_ODT_TTF_SET2                                                          0x18008a30
#define DC_PHY_ODT_TTF_SET2_reg_addr                                                 "0xB8008A30"
#define DC_PHY_ODT_TTF_SET2_reg                                                      0xB8008A30
#define set_DC_PHY_ODT_TTF_SET2_reg(data)   (*((volatile unsigned int*) DC_PHY_ODT_TTF_SET2_reg)=data)
#define get_DC_PHY_ODT_TTF_SET2_reg   (*((volatile unsigned int*) DC_PHY_ODT_TTF_SET2_reg))
#define DC_PHY_ODT_TTF_SET2_inst_adr                                                 "0x008C"
#define DC_PHY_ODT_TTF_SET2_inst                                                     0x008C
#define DC_PHY_ODT_TTF_SET2_odt_ttf_set_2_shift                                      (0)
#define DC_PHY_ODT_TTF_SET2_odt_ttf_set_2_mask                                       (0x00FFFFFF)
#define DC_PHY_ODT_TTF_SET2_odt_ttf_set_2(data)                                      (0x00FFFFFF&((data)<<0))
#define DC_PHY_ODT_TTF_SET2_odt_ttf_set_2_src(data)                                  ((0x00FFFFFF&(data))>>0)
#define DC_PHY_ODT_TTF_SET2_get_odt_ttf_set_2(data)                                  ((0x00FFFFFF&(data))>>0)


#define DC_PHY_OCD_SET0                                                              0x18008a34
#define DC_PHY_OCD_SET0_reg_addr                                                     "0xB8008A34"
#define DC_PHY_OCD_SET0_reg                                                          0xB8008A34
#define set_DC_PHY_OCD_SET0_reg(data)   (*((volatile unsigned int*) DC_PHY_OCD_SET0_reg)=data)
#define get_DC_PHY_OCD_SET0_reg   (*((volatile unsigned int*) DC_PHY_OCD_SET0_reg))
#define DC_PHY_OCD_SET0_inst_adr                                                     "0x008D"
#define DC_PHY_OCD_SET0_inst                                                         0x008D
#define DC_PHY_OCD_SET0_ocd_set_0_shift                                              (0)
#define DC_PHY_OCD_SET0_ocd_set_0_mask                                               (0x0FFFFFFF)
#define DC_PHY_OCD_SET0_ocd_set_0(data)                                              (0x0FFFFFFF&((data)<<0))
#define DC_PHY_OCD_SET0_ocd_set_0_src(data)                                          ((0x0FFFFFFF&(data))>>0)
#define DC_PHY_OCD_SET0_get_ocd_set_0(data)                                          ((0x0FFFFFFF&(data))>>0)


#define DC_PHY_OCD_SET1                                                              0x18008a38
#define DC_PHY_OCD_SET1_reg_addr                                                     "0xB8008A38"
#define DC_PHY_OCD_SET1_reg                                                          0xB8008A38
#define set_DC_PHY_OCD_SET1_reg(data)   (*((volatile unsigned int*) DC_PHY_OCD_SET1_reg)=data)
#define get_DC_PHY_OCD_SET1_reg   (*((volatile unsigned int*) DC_PHY_OCD_SET1_reg))
#define DC_PHY_OCD_SET1_inst_adr                                                     "0x008E"
#define DC_PHY_OCD_SET1_inst                                                         0x008E
#define DC_PHY_OCD_SET1_ocd_set_1_shift                                              (0)
#define DC_PHY_OCD_SET1_ocd_set_1_mask                                               (0x0FFFFFFF)
#define DC_PHY_OCD_SET1_ocd_set_1(data)                                              (0x0FFFFFFF&((data)<<0))
#define DC_PHY_OCD_SET1_ocd_set_1_src(data)                                          ((0x0FFFFFFF&(data))>>0)
#define DC_PHY_OCD_SET1_get_ocd_set_1(data)                                          ((0x0FFFFFFF&(data))>>0)


#define DC_PHY_OCD_SET2                                                              0x18008a3c
#define DC_PHY_OCD_SET2_reg_addr                                                     "0xB8008A3C"
#define DC_PHY_OCD_SET2_reg                                                          0xB8008A3C
#define set_DC_PHY_OCD_SET2_reg(data)   (*((volatile unsigned int*) DC_PHY_OCD_SET2_reg)=data)
#define get_DC_PHY_OCD_SET2_reg   (*((volatile unsigned int*) DC_PHY_OCD_SET2_reg))
#define DC_PHY_OCD_SET2_inst_adr                                                     "0x008F"
#define DC_PHY_OCD_SET2_inst                                                         0x008F
#define DC_PHY_OCD_SET2_ocd_set_2_shift                                              (0)
#define DC_PHY_OCD_SET2_ocd_set_2_mask                                               (0x0FFFFFFF)
#define DC_PHY_OCD_SET2_ocd_set_2(data)                                              (0x0FFFFFFF&((data)<<0))
#define DC_PHY_OCD_SET2_ocd_set_2_src(data)                                          ((0x0FFFFFFF&(data))>>0)
#define DC_PHY_OCD_SET2_get_ocd_set_2(data)                                          ((0x0FFFFFFF&(data))>>0)


#define DC_PHY_OCD_SET3                                                              0x18008a40
#define DC_PHY_OCD_SET3_reg_addr                                                     "0xB8008A40"
#define DC_PHY_OCD_SET3_reg                                                          0xB8008A40
#define set_DC_PHY_OCD_SET3_reg(data)   (*((volatile unsigned int*) DC_PHY_OCD_SET3_reg)=data)
#define get_DC_PHY_OCD_SET3_reg   (*((volatile unsigned int*) DC_PHY_OCD_SET3_reg))
#define DC_PHY_OCD_SET3_inst_adr                                                     "0x0090"
#define DC_PHY_OCD_SET3_inst                                                         0x0090
#define DC_PHY_OCD_SET3_ocd_set_3_shift                                              (0)
#define DC_PHY_OCD_SET3_ocd_set_3_mask                                               (0x0FFFFFFF)
#define DC_PHY_OCD_SET3_ocd_set_3(data)                                              (0x0FFFFFFF&((data)<<0))
#define DC_PHY_OCD_SET3_ocd_set_3_src(data)                                          ((0x0FFFFFFF&(data))>>0)
#define DC_PHY_OCD_SET3_get_ocd_set_3(data)                                          ((0x0FFFFFFF&(data))>>0)


#define DC_PHY_PAD_BUS_0                                                             0x18008b00
#define DC_PHY_PAD_BUS_0_reg_addr                                                    "0xB8008B00"
#define DC_PHY_PAD_BUS_0_reg                                                         0xB8008B00
#define set_DC_PHY_PAD_BUS_0_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_BUS_0_reg)=data)
#define get_DC_PHY_PAD_BUS_0_reg   (*((volatile unsigned int*) DC_PHY_PAD_BUS_0_reg))
#define DC_PHY_PAD_BUS_0_inst_adr                                                    "0x00C0"
#define DC_PHY_PAD_BUS_0_inst                                                        0x00C0
#define DC_PHY_PAD_BUS_0_pad_bus_0_shift                                             (0)
#define DC_PHY_PAD_BUS_0_pad_bus_0_mask                                              (0x1FFFFFFF)
#define DC_PHY_PAD_BUS_0_pad_bus_0(data)                                             (0x1FFFFFFF&((data)<<0))
#define DC_PHY_PAD_BUS_0_pad_bus_0_src(data)                                         ((0x1FFFFFFF&(data))>>0)
#define DC_PHY_PAD_BUS_0_get_pad_bus_0(data)                                         ((0x1FFFFFFF&(data))>>0)


#define DC_PHY_PAD_BUS_1                                                             0x18008b04
#define DC_PHY_PAD_BUS_1_reg_addr                                                    "0xB8008B04"
#define DC_PHY_PAD_BUS_1_reg                                                         0xB8008B04
#define set_DC_PHY_PAD_BUS_1_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_BUS_1_reg)=data)
#define get_DC_PHY_PAD_BUS_1_reg   (*((volatile unsigned int*) DC_PHY_PAD_BUS_1_reg))
#define DC_PHY_PAD_BUS_1_inst_adr                                                    "0x00C1"
#define DC_PHY_PAD_BUS_1_inst                                                        0x00C1
#define DC_PHY_PAD_BUS_1_pad_bus_1_shift                                             (0)
#define DC_PHY_PAD_BUS_1_pad_bus_1_mask                                              (0x01FFFFFF)
#define DC_PHY_PAD_BUS_1_pad_bus_1(data)                                             (0x01FFFFFF&((data)<<0))
#define DC_PHY_PAD_BUS_1_pad_bus_1_src(data)                                         ((0x01FFFFFF&(data))>>0)
#define DC_PHY_PAD_BUS_1_get_pad_bus_1(data)                                         ((0x01FFFFFF&(data))>>0)


#define DC_PHY_PAD_BUS_2                                                             0x18008b08
#define DC_PHY_PAD_BUS_2_reg_addr                                                    "0xB8008B08"
#define DC_PHY_PAD_BUS_2_reg                                                         0xB8008B08
#define set_DC_PHY_PAD_BUS_2_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_BUS_2_reg)=data)
#define get_DC_PHY_PAD_BUS_2_reg   (*((volatile unsigned int*) DC_PHY_PAD_BUS_2_reg))
#define DC_PHY_PAD_BUS_2_inst_adr                                                    "0x00C2"
#define DC_PHY_PAD_BUS_2_inst                                                        0x00C2
#define DC_PHY_PAD_BUS_2_pad_bus_2_shift                                             (0)
#define DC_PHY_PAD_BUS_2_pad_bus_2_mask                                              (0x01FFFFFF)
#define DC_PHY_PAD_BUS_2_pad_bus_2(data)                                             (0x01FFFFFF&((data)<<0))
#define DC_PHY_PAD_BUS_2_pad_bus_2_src(data)                                         ((0x01FFFFFF&(data))>>0)
#define DC_PHY_PAD_BUS_2_get_pad_bus_2(data)                                         ((0x01FFFFFF&(data))>>0)


#define DC_PHY_PAD_BUS_3                                                             0x18008b0c
#define DC_PHY_PAD_BUS_3_reg_addr                                                    "0xB8008B0C"
#define DC_PHY_PAD_BUS_3_reg                                                         0xB8008B0C
#define set_DC_PHY_PAD_BUS_3_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_BUS_3_reg)=data)
#define get_DC_PHY_PAD_BUS_3_reg   (*((volatile unsigned int*) DC_PHY_PAD_BUS_3_reg))
#define DC_PHY_PAD_BUS_3_inst_adr                                                    "0x00C3"
#define DC_PHY_PAD_BUS_3_inst                                                        0x00C3
#define DC_PHY_PAD_BUS_3_pad_bus_3_shift                                             (0)
#define DC_PHY_PAD_BUS_3_pad_bus_3_mask                                              (0x01FFFFFF)
#define DC_PHY_PAD_BUS_3_pad_bus_3(data)                                             (0x01FFFFFF&((data)<<0))
#define DC_PHY_PAD_BUS_3_pad_bus_3_src(data)                                         ((0x01FFFFFF&(data))>>0)
#define DC_PHY_PAD_BUS_3_get_pad_bus_3(data)                                         ((0x01FFFFFF&(data))>>0)


#define DC_PHY_PAD_BUS_4                                                             0x18008b10
#define DC_PHY_PAD_BUS_4_reg_addr                                                    "0xB8008B10"
#define DC_PHY_PAD_BUS_4_reg                                                         0xB8008B10
#define set_DC_PHY_PAD_BUS_4_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_BUS_4_reg)=data)
#define get_DC_PHY_PAD_BUS_4_reg   (*((volatile unsigned int*) DC_PHY_PAD_BUS_4_reg))
#define DC_PHY_PAD_BUS_4_inst_adr                                                    "0x00C4"
#define DC_PHY_PAD_BUS_4_inst                                                        0x00C4
#define DC_PHY_PAD_BUS_4_pad_bus_4_shift                                             (0)
#define DC_PHY_PAD_BUS_4_pad_bus_4_mask                                              (0x0000FFFF)
#define DC_PHY_PAD_BUS_4_pad_bus_4(data)                                             (0x0000FFFF&((data)<<0))
#define DC_PHY_PAD_BUS_4_pad_bus_4_src(data)                                         ((0x0000FFFF&(data))>>0)
#define DC_PHY_PAD_BUS_4_get_pad_bus_4(data)                                         ((0x0000FFFF&(data))>>0)


#define DC_PHY_PAD_BUS_5                                                             0x18008b14
#define DC_PHY_PAD_BUS_5_reg_addr                                                    "0xB8008B14"
#define DC_PHY_PAD_BUS_5_reg                                                         0xB8008B14
#define set_DC_PHY_PAD_BUS_5_reg(data)   (*((volatile unsigned int*) DC_PHY_PAD_BUS_5_reg)=data)
#define get_DC_PHY_PAD_BUS_5_reg   (*((volatile unsigned int*) DC_PHY_PAD_BUS_5_reg))
#define DC_PHY_PAD_BUS_5_inst_adr                                                    "0x00C5"
#define DC_PHY_PAD_BUS_5_inst                                                        0x00C5
#define DC_PHY_PAD_BUS_5_pad_bus_5_shift                                             (0)
#define DC_PHY_PAD_BUS_5_pad_bus_5_mask                                              (0x0000FFFF)
#define DC_PHY_PAD_BUS_5_pad_bus_5(data)                                             (0x0000FFFF&((data)<<0))
#define DC_PHY_PAD_BUS_5_pad_bus_5_src(data)                                         ((0x0000FFFF&(data))>>0)
#define DC_PHY_PAD_BUS_5_get_pad_bus_5(data)                                         ((0x0000FFFF&(data))>>0)


#define DC_PHY_CMD_DLY_1T                                                            0x18008f00
#define DC_PHY_CMD_DLY_1T_reg_addr                                                   "0xB8008F00"
#define DC_PHY_CMD_DLY_1T_reg                                                        0xB8008F00
#define set_DC_PHY_CMD_DLY_1T_reg(data)   (*((volatile unsigned int*) DC_PHY_CMD_DLY_1T_reg)=data)
#define get_DC_PHY_CMD_DLY_1T_reg   (*((volatile unsigned int*) DC_PHY_CMD_DLY_1T_reg))
#define DC_PHY_CMD_DLY_1T_inst_adr                                                   "0x00C0"
#define DC_PHY_CMD_DLY_1T_inst                                                       0x00C0
#define DC_PHY_CMD_DLY_1T_issue_cmd_dly_1T_shift                                     (0)
#define DC_PHY_CMD_DLY_1T_issue_cmd_dly_1T_mask                                      (0x00000001)
#define DC_PHY_CMD_DLY_1T_issue_cmd_dly_1T(data)                                     (0x00000001&((data)<<0))
#define DC_PHY_CMD_DLY_1T_issue_cmd_dly_1T_src(data)                                 ((0x00000001&(data))>>0)
#define DC_PHY_CMD_DLY_1T_get_issue_cmd_dly_1T(data)                                 ((0x00000001&(data))>>0)


#endif
