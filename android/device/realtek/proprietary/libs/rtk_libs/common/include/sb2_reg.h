/**************************************************************
// Spec Version                  : 0.0.5
// Parser Version                : DVR_Parser_5.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name
// Firmware Header Generate Date : 2010/4/16
***************************************************************/


#ifndef _SB2_REG_H_INCLUDED_
#define _SB2_REG_H_INCLUDED_
#ifdef  _SB2_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     Semaphore:1;
}SB2_HD_SEM;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     kcivairq_en:1;
unsigned int     To_en1:1;
unsigned int     To_en:1;
unsigned int     acivairq_en:1;
unsigned int     vcivairq_en:1;
unsigned int     scivairq_en:1;
unsigned int     write_data:1;
}SB2_INV_INTEN;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     Kciva_int:1;
unsigned int     Aciva_int:1;
unsigned int     Vciva_int:1;
unsigned int     Sciva_int:1;
unsigned int     write_data:1;
}SB2_INV_INTSTAT;

typedef struct 
{
unsigned int     Inv_adr:32;
}SB2_INV_ADDR;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     To_thr1:3;
unsigned int     To_thr:2;
}SB2_DEBUG_REG;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     Wr_hold_cyc:4;
unsigned int     reserved_1:3;
unsigned int     Wwr_cyc:5;
unsigned int     Wr_cyc:5;
unsigned int     Rd_cyc:5;
}SB2_FLASH_CTRL;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     write_en5:1;
unsigned int     kc_in_order:1;
unsigned int     kc_bypass:1;
unsigned int     kc_merge:1;
unsigned int     reserved_1:2;
unsigned int     write_en4:1;
unsigned int     mult_wr_en:1;
unsigned int     write_en3:1;
unsigned int     ac_in_order:1;
unsigned int     ac_bypass:1;
unsigned int     ac_merge:1;
unsigned int     write_en2:1;
unsigned int     vc_in_order:1;
unsigned int     vc_bypass:1;
unsigned int     vc_merge:1;
unsigned int     write_en1:1;
unsigned int     sc_in_order:1;
unsigned int     sc_bypass:1;
unsigned int     sc_merge:1;
}SB2_WRAPPER_CTRL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     enable:1;
}SB2_SCPU_INT_CHECK;

typedef struct 
{
unsigned int     Sync:32;
}SB2_SYNC;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     Dbg_mode:5;
}SB2_DBG;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     write_enable3:1;
unsigned int     sel1:5;
unsigned int     write_enable2:1;
unsigned int     sel0:5;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}SB2_DEBUG;

typedef struct 
{
unsigned int     addr:32;
}SB2_PCI_BASE;

typedef struct 
{
unsigned int     addr:32;
}SB2_PCI_MASK;

typedef struct 
{
unsigned int     addr:32;
}SB2_PCI_TRANS;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     write_enable4:1;
unsigned int     mode3:1;
unsigned int     en3:1;
unsigned int     write_enable3:1;
unsigned int     mode2:1;
unsigned int     en2:1;
unsigned int     write_enable2:1;
unsigned int     mode1:1;
unsigned int     en1:1;
unsigned int     write_enable1:1;
unsigned int     mode0:1;
unsigned int     en0:1;
}SB2_PCI_CTRL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     speed_up:1;
}SB2_LEXRA_CTRL;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     sample_byte_en_kcpu:1;
unsigned int     sample_byte_en_scpu:1;
}SB2_CPU_CTRL;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     si_endian_kcpu:1;
unsigned int     si_endian:1;
}SB2_ENDIAN;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     int_vk:1;
unsigned int     int_ak:1;
unsigned int     int_sk:1;
unsigned int     int_kv:1;
unsigned int     int_ka:1;
unsigned int     int_ks:1;
unsigned int     int_vs:1;
unsigned int     int_va:1;
unsigned int     int_av:1;
unsigned int     int_as:1;
unsigned int     int_sv:1;
unsigned int     int_sa:1;
unsigned int     write_data:1;
}SB2_CPU_INT;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     int_vk_en:1;
unsigned int     int_ak_en:1;
unsigned int     int_sk_en:1;
unsigned int     int_kv_en:1;
unsigned int     int_ka_en:1;
unsigned int     int_ks_en:1;
unsigned int     int_vs_en:1;
unsigned int     int_va_en:1;
unsigned int     int_av_en:1;
unsigned int     int_as_en:1;
unsigned int     int_sv_en:1;
unsigned int     int_sa_en:1;
unsigned int     write_data:1;
}SB2_CPU_INT_EN;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     m3_run:1;
unsigned int     m3_sel:4;
unsigned int     reserved_1:3;
unsigned int     m2_run:1;
unsigned int     m2_sel:4;
unsigned int     reserved_2:3;
unsigned int     m1_run:1;
unsigned int     m1_sel:4;
unsigned int     reserved_3:3;
unsigned int     m0_run:1;
unsigned int     m0_sel:4;
}SB2_PM_CTRL;

typedef struct 
{
unsigned int     pm_cnt:32;
}SB2_PM_CNT;

typedef struct 
{
unsigned int     pm_cnt_thr:32;
}SB2_PM_THR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     write_en4:1;
unsigned int     pm3_int:1;
unsigned int     write_en3:1;
unsigned int     pm2_int:1;
unsigned int     write_en2:1;
unsigned int     pm1_int:1;
unsigned int     write_en1:1;
unsigned int     pm0_int:1;
}SB2_PM_INT_STAT;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     write_en4:1;
unsigned int     pm3_int_en:1;
unsigned int     write_en3:1;
unsigned int     pm2_int_en:1;
unsigned int     write_en2:1;
unsigned int     pm1_int_en:1;
unsigned int     write_en1:1;
unsigned int     pm0_int_en:1;
}SB2_PM_INT_EN;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     pending:1;
unsigned int     reserved_1:1;
unsigned int     mask_id:4;
}SB2_DREQ_MASK;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     Chip_id:16;
}SB2_CHIP_ID;

typedef struct 
{
unsigned int     Revise_id:16;
unsigned int     reserved_0:16;
}SB2_CHIP_INFO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     write_en8:1;
unsigned int     kcpu_rbus_go:1;
unsigned int     write_en7:1;
unsigned int     kcpu_dbus_go:1;
unsigned int     write_en6:1;
unsigned int     acpu_rbus_go:1;
unsigned int     write_en5:1;
unsigned int     vcpu_rbus_go:1;
unsigned int     write_en4:1;
unsigned int     scpu_rbus_go:1;
unsigned int     write_en3:1;
unsigned int     acpu_dbus_go:1;
unsigned int     write_en2:1;
unsigned int     vcpu_dbus_go:1;
unsigned int     write_en1:1;
unsigned int     scpu_dbus_go:1;
}SB2_PC_CTRL;

typedef struct 
{
unsigned int     kcpu_dbus_acc:32;
}SB2_KCPU_DACC;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     kcpu_dreq_num:24;
}SB2_KCPU_DREQ_NUM;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     kcpu_dreq_max:16;
}SB2_KCPU_DREQ_MAX;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     kcpu_dreq_ack:26;
}SB2_KCPU_DREQ_ACK;

typedef struct 
{
unsigned int     kcpu_rbus_acc:32;
}SB2_KCPU_RACC;

typedef struct 
{
unsigned int     kcpu_rreq_max:8;
unsigned int     kcpu_rreq_num:24;
}SB2_KCPU_RREQ_RLT;

typedef struct 
{
unsigned int     scpu_dbus_acc:32;
}SB2_SCPU_DACC;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     scpu_dreq_num:24;
}SB2_SCPU_DREQ_NUM;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     scpu_dreq_max:16;
}SB2_SCPU_DREQ_MAX;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     scpu_dreq_ack:26;
}SB2_SCPU_DREQ_ACK;

typedef struct 
{
unsigned int     vcpu_dbus_acc:32;
}SB2_VCPU_DACC;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     vcpu_dreq_num:24;
}SB2_VCPU_DREQ_NUM;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     vcpu_dreq_max:16;
}SB2_VCPU_DREQ_MAX;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     vcpu_dreq_ack:26;
}SB2_VCPU_DREQ_ACK;

typedef struct 
{
unsigned int     acpu_dbus_acc:32;
}SB2_ACPU_DACC;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     acpu_dreq_num:24;
}SB2_ACPU_DREQ_NUM;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     acpu_dreq_max:16;
}SB2_ACPU_DREQ_MAX;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     acpu_dreq_ack:26;
}SB2_ACPU_DREQ_ACK;

typedef struct 
{
unsigned int     scpu_rbus_acc:32;
}SB2_SCPU_RACC;

typedef struct 
{
unsigned int     scpu_rreq_max:8;
unsigned int     scpu_rreq_num:24;
}SB2_SCPU_RREQ_RLT;

typedef struct 
{
unsigned int     vcpu_rbus_acc:32;
}SB2_VCPU_RACC;

typedef struct 
{
unsigned int     vcpu_rreq_max:8;
unsigned int     vcpu_rreq_num:24;
}SB2_VCPU_RREQ_RLT;

typedef struct 
{
unsigned int     acpu_rbus_acc:32;
}SB2_ACPU_RACC;

typedef struct 
{
unsigned int     acpu_rreq_max:8;
unsigned int     acpu_rreq_num:24;
}SB2_ACPU_RREQ_RLT;

typedef struct 
{
unsigned int     Dbg_start_addr:32;
}SB2_DBG_START;

typedef struct 
{
unsigned int     Dbg_end_addr:32;
}SB2_DBG_END;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     write_en7:1;
unsigned int     Dbg_kcpu_chk_en:1;
unsigned int     write_en6:1;
unsigned int     Dbg_acpu_chk_en:1;
unsigned int     write_en5:1;
unsigned int     Dbg_vcpu_chk_en:1;
unsigned int     write_en4:1;
unsigned int     Dbg_scpu_chk_en:1;
unsigned int     write_en3:1;
unsigned int     Dbg_wr_chk:2;
unsigned int     write_en2:1;
unsigned int     Dbg_id_chk:2;
unsigned int     write_en1:1;
unsigned int     Dbg_en:1;
}SB2_DBG_CTRL;

typedef struct 
{
unsigned int     dbg_addr:32;
}SB2_DBG_ADDR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     vcpu_dbg_write:1;
unsigned int     vcpu_dbg_dacc:1;
unsigned int     acpu_dbg_write:1;
unsigned int     acpu_dbg_dacc:1;
unsigned int     scpu_dbg_write:1;
unsigned int     scpu_dbg_dacc:1;
unsigned int     kcpu_dbg_write:1;
unsigned int     kcpu_dbg_dacc:1;
}SB2_DBG_ADDR1;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     kcpu_int:1;
unsigned int     reserved_1:2;
unsigned int     kcpu_int_en:1;
unsigned int     reserved_2:2;
unsigned int     kcpu_neg_int:1;
unsigned int     reserved_3:2;
unsigned int     kcpu_neg_int_en:1;
unsigned int     write_data:1;
}SB2_DBG_INT_KCPU;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     acpu_int:1;
unsigned int     vcpu_int:1;
unsigned int     scpu_int:1;
unsigned int     acpu_int_en:1;
unsigned int     vcpu_int_en:1;
unsigned int     scpu_int_en:1;
unsigned int     acpu_neg_int:1;
unsigned int     vcpu_neg_int:1;
unsigned int     scpu_neg_int:1;
unsigned int     acpu_neg_int_en:1;
unsigned int     vcpu_neg_int_en:1;
unsigned int     scpu_neg_int_en:1;
unsigned int     write_data:1;
}SB2_DBG_INT;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     Gcra_en:1;
}SB2_GCRA_CTRL;

typedef struct 
{
unsigned int     Gcra_tim_cnt:32;
}SB2_GCRA_TIM_CNT;

typedef struct 
{
unsigned int     Gcra_tim_thr:32;
}SB2_GCRA_TIM_THR;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     Gcra_req_num:24;
}SB2_GCRA_REQ_NUM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     Gcra_req_thr:24;
}SB2_GCRA_REQ_THR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     addr:4;
}SB2_GREG_START;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     addr:4;
}SB2_GREG_END;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     offsetB:4;
unsigned int     offsetA:4;
}SB2_GREG;

typedef struct 
{
unsigned int     data:32;
}SB2_DUMMY;

typedef struct 
{
unsigned int     data:32;
}SB2_SHADOW;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     Semaphore:1;
}SB2_HD_SEM_NEW;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     dc_int_en:1;
unsigned int     gpu_int_en:1;
unsigned int     pcie_int_en:1;
unsigned int     md_int_en:1;
unsigned int     tp_int_en:1;
unsigned int     hdmi_int_en:1;
unsigned int     etn_int_en:1;
unsigned int     usb_int_en:1;
unsigned int     cr_int_en:1;
unsigned int     nf_int_en:1;
unsigned int     sata_int_en:1;
unsigned int     se_int_en:1;
unsigned int     cp_int_en:1;
}SB2_SCPU_INT_MUX;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     dc_int_en:1;
unsigned int     gpu_int_en:1;
unsigned int     pcie_int_en:1;
unsigned int     md_int_en:1;
unsigned int     tp_int_en:1;
unsigned int     hdmi_int_en:1;
unsigned int     etn_int_en:1;
unsigned int     usb_int_en:1;
unsigned int     cr_int_en:1;
unsigned int     nf_int_en:1;
unsigned int     sata_int_en:1;
unsigned int     se_int_en:1;
unsigned int     cp_int_en:1;
}SB2_KCPU_INT_MUX;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     write_en2:1;
unsigned int     tp_sec_reg_ctrl:1;
unsigned int     write_en1:1;
unsigned int     cp_sec_reg_ctrl:1;
}SB2_SECURITY_KCPU;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     inva_wr_sec2_en:1;
unsigned int     reserved_1:1;
unsigned int     inva_wr_sec1_en:1;
}SB2_RBUS_INVA_ACC_CTRL;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     sec1_start:16;
unsigned int     reserved_1:2;
}SB2_RBUS_INVA_ACC_SEC1_START;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     sec1_end:16;
unsigned int     reserved_1:2;
}SB2_RBUS_INVA_ACC_SEC1_END;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     sec2_start:16;
unsigned int     reserved_1:2;
}SB2_RBUS_INVA_ACC_SEC2_START;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     sec2_end:16;
unsigned int     reserved_1:2;
}SB2_RBUS_INVA_ACC_SEC2_END;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     crc_cmp_latch_en:1;
}SB2_RBUS_CMP_CTRL;

typedef struct 
{
unsigned int     crc_cmp_expect:32;
}SB2_RBUS_CMP_DATA;

typedef struct 
{
unsigned int     crc_cmp_result:32;
}SB2_RBUS_CMP_RESULT;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     write_enable3:1;
unsigned int     sel1:5;
unsigned int     write_enable2:1;
unsigned int     sel0:5;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}SB2_SCPU_DBG;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     adr4byte_en:1;
unsigned int     dual_mode:2;
unsigned int     fat_mode:1;
unsigned int     opc:8;
}SB2_SFC_OPCODE;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     reserved_1:11;
unsigned int     dataen:1;
unsigned int     adren:1;
unsigned int     dmycnt:3;
}SB2_SFC_CTL;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     reserved_1:7;
unsigned int     mode:1;
unsigned int     fdiv:8;
}SB2_SFC_SCK;

typedef struct 
{
unsigned int     tdt:16;
unsigned int     Phcnt:8;
unsigned int     Plcnt:8;
}SB2_SFC_CE;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     write_en5:1;
unsigned int     rdy:1;
unsigned int     reserved_1:24;
unsigned int     write_en1:1;
unsigned int     wp_n:1;
}SB2_SFC_WP;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     pos_latch:1;
}SB2_SFC_POS_LATCH;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     wt_prog_done:1;
unsigned int     rdsr_op:8;
}SB2_SFC_WAIT_WR;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     wt_prog_en:1;
unsigned int     wr_en_op:8;
}SB2_SFC_EN_WR;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     sf_dclk_sel:1;
unsigned int     fifo_write_tap:3;
unsigned int     fast_read_en:1;
}SB2_SFC_FAST_RD;

typedef struct 
{
unsigned int     delay_clk_tap:32;
}SB2_SFC_SCK_TAP;

#endif

#define SB2_HD_SEM                                                                   0x1801A000
#define SB2_HD_SEM_reg_addr                                                          "0xB801A000"
#define SB2_HD_SEM_reg                                                               0xB801A000
#define set_SB2_HD_SEM_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_reg)=data)
#define get_SB2_HD_SEM_reg   (*((volatile unsigned int*) SB2_HD_SEM_reg))
#define SB2_HD_SEM_inst_adr                                                          "0x0000"
#define SB2_HD_SEM_inst                                                              0x0000
#define SB2_HD_SEM_Semaphore_shift                                                   (0)
#define SB2_HD_SEM_Semaphore_mask                                                    (0x00000001)
#define SB2_HD_SEM_Semaphore(data)                                                   (0x00000001&((data)<<0))
#define SB2_HD_SEM_Semaphore_src(data)                                               ((0x00000001&(data))>>0)
#define SB2_HD_SEM_get_Semaphore(data)                                               ((0x00000001&(data))>>0)


#define SB2_INV_INTEN                                                                0x1801A004
#define SB2_INV_INTEN_reg_addr                                                       "0xB801A004"
#define SB2_INV_INTEN_reg                                                            0xB801A004
#define set_SB2_INV_INTEN_reg(data)   (*((volatile unsigned int*) SB2_INV_INTEN_reg)=data)
#define get_SB2_INV_INTEN_reg   (*((volatile unsigned int*) SB2_INV_INTEN_reg))
#define SB2_INV_INTEN_inst_adr                                                       "0x0001"
#define SB2_INV_INTEN_inst                                                           0x0001
#define SB2_INV_INTEN_kcivairq_en_shift                                              (6)
#define SB2_INV_INTEN_kcivairq_en_mask                                               (0x00000040)
#define SB2_INV_INTEN_kcivairq_en(data)                                              (0x00000040&((data)<<6))
#define SB2_INV_INTEN_kcivairq_en_src(data)                                          ((0x00000040&(data))>>6)
#define SB2_INV_INTEN_get_kcivairq_en(data)                                          ((0x00000040&(data))>>6)
#define SB2_INV_INTEN_To_en1_shift                                                   (5)
#define SB2_INV_INTEN_To_en1_mask                                                    (0x00000020)
#define SB2_INV_INTEN_To_en1(data)                                                   (0x00000020&((data)<<5))
#define SB2_INV_INTEN_To_en1_src(data)                                               ((0x00000020&(data))>>5)
#define SB2_INV_INTEN_get_To_en1(data)                                               ((0x00000020&(data))>>5)
#define SB2_INV_INTEN_To_en_shift                                                    (4)
#define SB2_INV_INTEN_To_en_mask                                                     (0x00000010)
#define SB2_INV_INTEN_To_en(data)                                                    (0x00000010&((data)<<4))
#define SB2_INV_INTEN_To_en_src(data)                                                ((0x00000010&(data))>>4)
#define SB2_INV_INTEN_get_To_en(data)                                                ((0x00000010&(data))>>4)
#define SB2_INV_INTEN_acivairq_en_shift                                              (3)
#define SB2_INV_INTEN_acivairq_en_mask                                               (0x00000008)
#define SB2_INV_INTEN_acivairq_en(data)                                              (0x00000008&((data)<<3))
#define SB2_INV_INTEN_acivairq_en_src(data)                                          ((0x00000008&(data))>>3)
#define SB2_INV_INTEN_get_acivairq_en(data)                                          ((0x00000008&(data))>>3)
#define SB2_INV_INTEN_vcivairq_en_shift                                              (2)
#define SB2_INV_INTEN_vcivairq_en_mask                                               (0x00000004)
#define SB2_INV_INTEN_vcivairq_en(data)                                              (0x00000004&((data)<<2))
#define SB2_INV_INTEN_vcivairq_en_src(data)                                          ((0x00000004&(data))>>2)
#define SB2_INV_INTEN_get_vcivairq_en(data)                                          ((0x00000004&(data))>>2)
#define SB2_INV_INTEN_scivairq_en_shift                                              (1)
#define SB2_INV_INTEN_scivairq_en_mask                                               (0x00000002)
#define SB2_INV_INTEN_scivairq_en(data)                                              (0x00000002&((data)<<1))
#define SB2_INV_INTEN_scivairq_en_src(data)                                          ((0x00000002&(data))>>1)
#define SB2_INV_INTEN_get_scivairq_en(data)                                          ((0x00000002&(data))>>1)
#define SB2_INV_INTEN_write_data_shift                                               (0)
#define SB2_INV_INTEN_write_data_mask                                                (0x00000001)
#define SB2_INV_INTEN_write_data(data)                                               (0x00000001&((data)<<0))
#define SB2_INV_INTEN_write_data_src(data)                                           ((0x00000001&(data))>>0)
#define SB2_INV_INTEN_get_write_data(data)                                           ((0x00000001&(data))>>0)


#define SB2_INV_INTSTAT                                                              0x1801A008
#define SB2_INV_INTSTAT_reg_addr                                                     "0xB801A008"
#define SB2_INV_INTSTAT_reg                                                          0xB801A008
#define set_SB2_INV_INTSTAT_reg(data)   (*((volatile unsigned int*) SB2_INV_INTSTAT_reg)=data)
#define get_SB2_INV_INTSTAT_reg   (*((volatile unsigned int*) SB2_INV_INTSTAT_reg))
#define SB2_INV_INTSTAT_inst_adr                                                     "0x0002"
#define SB2_INV_INTSTAT_inst                                                         0x0002
#define SB2_INV_INTSTAT_Kciva_int_shift                                              (4)
#define SB2_INV_INTSTAT_Kciva_int_mask                                               (0x00000010)
#define SB2_INV_INTSTAT_Kciva_int(data)                                              (0x00000010&((data)<<4))
#define SB2_INV_INTSTAT_Kciva_int_src(data)                                          ((0x00000010&(data))>>4)
#define SB2_INV_INTSTAT_get_Kciva_int(data)                                          ((0x00000010&(data))>>4)
#define SB2_INV_INTSTAT_Aciva_int_shift                                              (3)
#define SB2_INV_INTSTAT_Aciva_int_mask                                               (0x00000008)
#define SB2_INV_INTSTAT_Aciva_int(data)                                              (0x00000008&((data)<<3))
#define SB2_INV_INTSTAT_Aciva_int_src(data)                                          ((0x00000008&(data))>>3)
#define SB2_INV_INTSTAT_get_Aciva_int(data)                                          ((0x00000008&(data))>>3)
#define SB2_INV_INTSTAT_Vciva_int_shift                                              (2)
#define SB2_INV_INTSTAT_Vciva_int_mask                                               (0x00000004)
#define SB2_INV_INTSTAT_Vciva_int(data)                                              (0x00000004&((data)<<2))
#define SB2_INV_INTSTAT_Vciva_int_src(data)                                          ((0x00000004&(data))>>2)
#define SB2_INV_INTSTAT_get_Vciva_int(data)                                          ((0x00000004&(data))>>2)
#define SB2_INV_INTSTAT_Sciva_int_shift                                              (1)
#define SB2_INV_INTSTAT_Sciva_int_mask                                               (0x00000002)
#define SB2_INV_INTSTAT_Sciva_int(data)                                              (0x00000002&((data)<<1))
#define SB2_INV_INTSTAT_Sciva_int_src(data)                                          ((0x00000002&(data))>>1)
#define SB2_INV_INTSTAT_get_Sciva_int(data)                                          ((0x00000002&(data))>>1)
#define SB2_INV_INTSTAT_write_data_shift                                             (0)
#define SB2_INV_INTSTAT_write_data_mask                                              (0x00000001)
#define SB2_INV_INTSTAT_write_data(data)                                             (0x00000001&((data)<<0))
#define SB2_INV_INTSTAT_write_data_src(data)                                         ((0x00000001&(data))>>0)
#define SB2_INV_INTSTAT_get_write_data(data)                                         ((0x00000001&(data))>>0)


#define SB2_INV_ADDR                                                                 0x1801A00C
#define SB2_INV_ADDR_reg_addr                                                        "0xB801A00C"
#define SB2_INV_ADDR_reg                                                             0xB801A00C
#define set_SB2_INV_ADDR_reg(data)   (*((volatile unsigned int*) SB2_INV_ADDR_reg)=data)
#define get_SB2_INV_ADDR_reg   (*((volatile unsigned int*) SB2_INV_ADDR_reg))
#define SB2_INV_ADDR_inst_adr                                                        "0x0003"
#define SB2_INV_ADDR_inst                                                            0x0003
#define SB2_INV_ADDR_Inv_adr_shift                                                   (0)
#define SB2_INV_ADDR_Inv_adr_mask                                                    (0xFFFFFFFF)
#define SB2_INV_ADDR_Inv_adr(data)                                                   (0xFFFFFFFF&((data)<<0))
#define SB2_INV_ADDR_Inv_adr_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define SB2_INV_ADDR_get_Inv_adr(data)                                               ((0xFFFFFFFF&(data))>>0)


#define SB2_DEBUG_REG                                                                0x1801A010
#define SB2_DEBUG_REG_reg_addr                                                       "0xB801A010"
#define SB2_DEBUG_REG_reg                                                            0xB801A010
#define set_SB2_DEBUG_REG_reg(data)   (*((volatile unsigned int*) SB2_DEBUG_REG_reg)=data)
#define get_SB2_DEBUG_REG_reg   (*((volatile unsigned int*) SB2_DEBUG_REG_reg))
#define SB2_DEBUG_REG_inst_adr                                                       "0x0004"
#define SB2_DEBUG_REG_inst                                                           0x0004
#define SB2_DEBUG_REG_To_thr1_shift                                                  (2)
#define SB2_DEBUG_REG_To_thr1_mask                                                   (0x0000001C)
#define SB2_DEBUG_REG_To_thr1(data)                                                  (0x0000001C&((data)<<2))
#define SB2_DEBUG_REG_To_thr1_src(data)                                              ((0x0000001C&(data))>>2)
#define SB2_DEBUG_REG_get_To_thr1(data)                                              ((0x0000001C&(data))>>2)
#define SB2_DEBUG_REG_To_thr_shift                                                   (0)
#define SB2_DEBUG_REG_To_thr_mask                                                    (0x00000003)
#define SB2_DEBUG_REG_To_thr(data)                                                   (0x00000003&((data)<<0))
#define SB2_DEBUG_REG_To_thr_src(data)                                               ((0x00000003&(data))>>0)
#define SB2_DEBUG_REG_get_To_thr(data)                                               ((0x00000003&(data))>>0)


#define SB2_FLASH_CTRL                                                               0x1801A014
#define SB2_FLASH_CTRL_reg_addr                                                      "0xB801A014"
#define SB2_FLASH_CTRL_reg                                                           0xB801A014
#define set_SB2_FLASH_CTRL_reg(data)   (*((volatile unsigned int*) SB2_FLASH_CTRL_reg)=data)
#define get_SB2_FLASH_CTRL_reg   (*((volatile unsigned int*) SB2_FLASH_CTRL_reg))
#define SB2_FLASH_CTRL_inst_adr                                                      "0x0005"
#define SB2_FLASH_CTRL_inst                                                          0x0005
#define SB2_FLASH_CTRL_Wr_hold_cyc_shift                                             (18)
#define SB2_FLASH_CTRL_Wr_hold_cyc_mask                                              (0x003C0000)
#define SB2_FLASH_CTRL_Wr_hold_cyc(data)                                             (0x003C0000&((data)<<18))
#define SB2_FLASH_CTRL_Wr_hold_cyc_src(data)                                         ((0x003C0000&(data))>>18)
#define SB2_FLASH_CTRL_get_Wr_hold_cyc(data)                                         ((0x003C0000&(data))>>18)
#define SB2_FLASH_CTRL_Wwr_cyc_shift                                                 (10)
#define SB2_FLASH_CTRL_Wwr_cyc_mask                                                  (0x00007C00)
#define SB2_FLASH_CTRL_Wwr_cyc(data)                                                 (0x00007C00&((data)<<10))
#define SB2_FLASH_CTRL_Wwr_cyc_src(data)                                             ((0x00007C00&(data))>>10)
#define SB2_FLASH_CTRL_get_Wwr_cyc(data)                                             ((0x00007C00&(data))>>10)
#define SB2_FLASH_CTRL_Wr_cyc_shift                                                  (5)
#define SB2_FLASH_CTRL_Wr_cyc_mask                                                   (0x000003E0)
#define SB2_FLASH_CTRL_Wr_cyc(data)                                                  (0x000003E0&((data)<<5))
#define SB2_FLASH_CTRL_Wr_cyc_src(data)                                              ((0x000003E0&(data))>>5)
#define SB2_FLASH_CTRL_get_Wr_cyc(data)                                              ((0x000003E0&(data))>>5)
#define SB2_FLASH_CTRL_Rd_cyc_shift                                                  (0)
#define SB2_FLASH_CTRL_Rd_cyc_mask                                                   (0x0000001F)
#define SB2_FLASH_CTRL_Rd_cyc(data)                                                  (0x0000001F&((data)<<0))
#define SB2_FLASH_CTRL_Rd_cyc_src(data)                                              ((0x0000001F&(data))>>0)
#define SB2_FLASH_CTRL_get_Rd_cyc(data)                                              ((0x0000001F&(data))>>0)


#define SB2_WRAPPER_CTRL                                                             0x1801A018
#define SB2_WRAPPER_CTRL_reg_addr                                                    "0xB801A018"
#define SB2_WRAPPER_CTRL_reg                                                         0xB801A018
#define set_SB2_WRAPPER_CTRL_reg(data)   (*((volatile unsigned int*) SB2_WRAPPER_CTRL_reg)=data)
#define get_SB2_WRAPPER_CTRL_reg   (*((volatile unsigned int*) SB2_WRAPPER_CTRL_reg))
#define SB2_WRAPPER_CTRL_inst_adr                                                    "0x0006"
#define SB2_WRAPPER_CTRL_inst                                                        0x0006
#define SB2_WRAPPER_CTRL_write_en5_shift                                             (19)
#define SB2_WRAPPER_CTRL_write_en5_mask                                              (0x00080000)
#define SB2_WRAPPER_CTRL_write_en5(data)                                             (0x00080000&((data)<<19))
#define SB2_WRAPPER_CTRL_write_en5_src(data)                                         ((0x00080000&(data))>>19)
#define SB2_WRAPPER_CTRL_get_write_en5(data)                                         ((0x00080000&(data))>>19)
#define SB2_WRAPPER_CTRL_kc_in_order_shift                                           (18)
#define SB2_WRAPPER_CTRL_kc_in_order_mask                                            (0x00040000)
#define SB2_WRAPPER_CTRL_kc_in_order(data)                                           (0x00040000&((data)<<18))
#define SB2_WRAPPER_CTRL_kc_in_order_src(data)                                       ((0x00040000&(data))>>18)
#define SB2_WRAPPER_CTRL_get_kc_in_order(data)                                       ((0x00040000&(data))>>18)
#define SB2_WRAPPER_CTRL_kc_bypass_shift                                             (17)
#define SB2_WRAPPER_CTRL_kc_bypass_mask                                              (0x00020000)
#define SB2_WRAPPER_CTRL_kc_bypass(data)                                             (0x00020000&((data)<<17))
#define SB2_WRAPPER_CTRL_kc_bypass_src(data)                                         ((0x00020000&(data))>>17)
#define SB2_WRAPPER_CTRL_get_kc_bypass(data)                                         ((0x00020000&(data))>>17)
#define SB2_WRAPPER_CTRL_kc_merge_shift                                              (16)
#define SB2_WRAPPER_CTRL_kc_merge_mask                                               (0x00010000)
#define SB2_WRAPPER_CTRL_kc_merge(data)                                              (0x00010000&((data)<<16))
#define SB2_WRAPPER_CTRL_kc_merge_src(data)                                          ((0x00010000&(data))>>16)
#define SB2_WRAPPER_CTRL_get_kc_merge(data)                                          ((0x00010000&(data))>>16)
#define SB2_WRAPPER_CTRL_write_en4_shift                                             (13)
#define SB2_WRAPPER_CTRL_write_en4_mask                                              (0x00002000)
#define SB2_WRAPPER_CTRL_write_en4(data)                                             (0x00002000&((data)<<13))
#define SB2_WRAPPER_CTRL_write_en4_src(data)                                         ((0x00002000&(data))>>13)
#define SB2_WRAPPER_CTRL_get_write_en4(data)                                         ((0x00002000&(data))>>13)
#define SB2_WRAPPER_CTRL_mult_wr_en_shift                                            (12)
#define SB2_WRAPPER_CTRL_mult_wr_en_mask                                             (0x00001000)
#define SB2_WRAPPER_CTRL_mult_wr_en(data)                                            (0x00001000&((data)<<12))
#define SB2_WRAPPER_CTRL_mult_wr_en_src(data)                                        ((0x00001000&(data))>>12)
#define SB2_WRAPPER_CTRL_get_mult_wr_en(data)                                        ((0x00001000&(data))>>12)
#define SB2_WRAPPER_CTRL_write_en3_shift                                             (11)
#define SB2_WRAPPER_CTRL_write_en3_mask                                              (0x00000800)
#define SB2_WRAPPER_CTRL_write_en3(data)                                             (0x00000800&((data)<<11))
#define SB2_WRAPPER_CTRL_write_en3_src(data)                                         ((0x00000800&(data))>>11)
#define SB2_WRAPPER_CTRL_get_write_en3(data)                                         ((0x00000800&(data))>>11)
#define SB2_WRAPPER_CTRL_ac_in_order_shift                                           (10)
#define SB2_WRAPPER_CTRL_ac_in_order_mask                                            (0x00000400)
#define SB2_WRAPPER_CTRL_ac_in_order(data)                                           (0x00000400&((data)<<10))
#define SB2_WRAPPER_CTRL_ac_in_order_src(data)                                       ((0x00000400&(data))>>10)
#define SB2_WRAPPER_CTRL_get_ac_in_order(data)                                       ((0x00000400&(data))>>10)
#define SB2_WRAPPER_CTRL_ac_bypass_shift                                             (9)
#define SB2_WRAPPER_CTRL_ac_bypass_mask                                              (0x00000200)
#define SB2_WRAPPER_CTRL_ac_bypass(data)                                             (0x00000200&((data)<<9))
#define SB2_WRAPPER_CTRL_ac_bypass_src(data)                                         ((0x00000200&(data))>>9)
#define SB2_WRAPPER_CTRL_get_ac_bypass(data)                                         ((0x00000200&(data))>>9)
#define SB2_WRAPPER_CTRL_ac_merge_shift                                              (8)
#define SB2_WRAPPER_CTRL_ac_merge_mask                                               (0x00000100)
#define SB2_WRAPPER_CTRL_ac_merge(data)                                              (0x00000100&((data)<<8))
#define SB2_WRAPPER_CTRL_ac_merge_src(data)                                          ((0x00000100&(data))>>8)
#define SB2_WRAPPER_CTRL_get_ac_merge(data)                                          ((0x00000100&(data))>>8)
#define SB2_WRAPPER_CTRL_write_en2_shift                                             (7)
#define SB2_WRAPPER_CTRL_write_en2_mask                                              (0x00000080)
#define SB2_WRAPPER_CTRL_write_en2(data)                                             (0x00000080&((data)<<7))
#define SB2_WRAPPER_CTRL_write_en2_src(data)                                         ((0x00000080&(data))>>7)
#define SB2_WRAPPER_CTRL_get_write_en2(data)                                         ((0x00000080&(data))>>7)
#define SB2_WRAPPER_CTRL_vc_in_order_shift                                           (6)
#define SB2_WRAPPER_CTRL_vc_in_order_mask                                            (0x00000040)
#define SB2_WRAPPER_CTRL_vc_in_order(data)                                           (0x00000040&((data)<<6))
#define SB2_WRAPPER_CTRL_vc_in_order_src(data)                                       ((0x00000040&(data))>>6)
#define SB2_WRAPPER_CTRL_get_vc_in_order(data)                                       ((0x00000040&(data))>>6)
#define SB2_WRAPPER_CTRL_vc_bypass_shift                                             (5)
#define SB2_WRAPPER_CTRL_vc_bypass_mask                                              (0x00000020)
#define SB2_WRAPPER_CTRL_vc_bypass(data)                                             (0x00000020&((data)<<5))
#define SB2_WRAPPER_CTRL_vc_bypass_src(data)                                         ((0x00000020&(data))>>5)
#define SB2_WRAPPER_CTRL_get_vc_bypass(data)                                         ((0x00000020&(data))>>5)
#define SB2_WRAPPER_CTRL_vc_merge_shift                                              (4)
#define SB2_WRAPPER_CTRL_vc_merge_mask                                               (0x00000010)
#define SB2_WRAPPER_CTRL_vc_merge(data)                                              (0x00000010&((data)<<4))
#define SB2_WRAPPER_CTRL_vc_merge_src(data)                                          ((0x00000010&(data))>>4)
#define SB2_WRAPPER_CTRL_get_vc_merge(data)                                          ((0x00000010&(data))>>4)
#define SB2_WRAPPER_CTRL_write_en1_shift                                             (3)
#define SB2_WRAPPER_CTRL_write_en1_mask                                              (0x00000008)
#define SB2_WRAPPER_CTRL_write_en1(data)                                             (0x00000008&((data)<<3))
#define SB2_WRAPPER_CTRL_write_en1_src(data)                                         ((0x00000008&(data))>>3)
#define SB2_WRAPPER_CTRL_get_write_en1(data)                                         ((0x00000008&(data))>>3)
#define SB2_WRAPPER_CTRL_sc_in_order_shift                                           (2)
#define SB2_WRAPPER_CTRL_sc_in_order_mask                                            (0x00000004)
#define SB2_WRAPPER_CTRL_sc_in_order(data)                                           (0x00000004&((data)<<2))
#define SB2_WRAPPER_CTRL_sc_in_order_src(data)                                       ((0x00000004&(data))>>2)
#define SB2_WRAPPER_CTRL_get_sc_in_order(data)                                       ((0x00000004&(data))>>2)
#define SB2_WRAPPER_CTRL_sc_bypass_shift                                             (1)
#define SB2_WRAPPER_CTRL_sc_bypass_mask                                              (0x00000002)
#define SB2_WRAPPER_CTRL_sc_bypass(data)                                             (0x00000002&((data)<<1))
#define SB2_WRAPPER_CTRL_sc_bypass_src(data)                                         ((0x00000002&(data))>>1)
#define SB2_WRAPPER_CTRL_get_sc_bypass(data)                                         ((0x00000002&(data))>>1)
#define SB2_WRAPPER_CTRL_sc_merge_shift                                              (0)
#define SB2_WRAPPER_CTRL_sc_merge_mask                                               (0x00000001)
#define SB2_WRAPPER_CTRL_sc_merge(data)                                              (0x00000001&((data)<<0))
#define SB2_WRAPPER_CTRL_sc_merge_src(data)                                          ((0x00000001&(data))>>0)
#define SB2_WRAPPER_CTRL_get_sc_merge(data)                                          ((0x00000001&(data))>>0)


#define SB2_SCPU_INT_CHECK                                                           0x1801A01C
#define SB2_SCPU_INT_CHECK_reg_addr                                                  "0xB801A01C"
#define SB2_SCPU_INT_CHECK_reg                                                       0xB801A01C
#define set_SB2_SCPU_INT_CHECK_reg(data)   (*((volatile unsigned int*) SB2_SCPU_INT_CHECK_reg)=data)
#define get_SB2_SCPU_INT_CHECK_reg   (*((volatile unsigned int*) SB2_SCPU_INT_CHECK_reg))
#define SB2_SCPU_INT_CHECK_inst_adr                                                  "0x0007"
#define SB2_SCPU_INT_CHECK_inst                                                      0x0007
#define SB2_SCPU_INT_CHECK_enable_shift                                              (0)
#define SB2_SCPU_INT_CHECK_enable_mask                                               (0x00000001)
#define SB2_SCPU_INT_CHECK_enable(data)                                              (0x00000001&((data)<<0))
#define SB2_SCPU_INT_CHECK_enable_src(data)                                          ((0x00000001&(data))>>0)
#define SB2_SCPU_INT_CHECK_get_enable(data)                                          ((0x00000001&(data))>>0)


#define SB2_SYNC                                                                     0x1801A020
#define SB2_SYNC_reg_addr                                                            "0xB801A020"
#define SB2_SYNC_reg                                                                 0xB801A020
#define set_SB2_SYNC_reg(data)   (*((volatile unsigned int*) SB2_SYNC_reg)=data)
#define get_SB2_SYNC_reg   (*((volatile unsigned int*) SB2_SYNC_reg))
#define SB2_SYNC_inst_adr                                                            "0x0008"
#define SB2_SYNC_inst                                                                0x0008
#define SB2_SYNC_Sync_shift                                                          (0)
#define SB2_SYNC_Sync_mask                                                           (0xFFFFFFFF)
#define SB2_SYNC_Sync(data)                                                          (0xFFFFFFFF&((data)<<0))
#define SB2_SYNC_Sync_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define SB2_SYNC_get_Sync(data)                                                      ((0xFFFFFFFF&(data))>>0)


#define SB2_DBG                                                                      0x1801A024
#define SB2_DBG_reg_addr                                                             "0xB801A024"
#define SB2_DBG_reg                                                                  0xB801A024
#define set_SB2_DBG_reg(data)   (*((volatile unsigned int*) SB2_DBG_reg)=data)
#define get_SB2_DBG_reg   (*((volatile unsigned int*) SB2_DBG_reg))
#define SB2_DBG_inst_adr                                                             "0x0009"
#define SB2_DBG_inst                                                                 0x0009
#define SB2_DBG_Dbg_mode_shift                                                       (0)
#define SB2_DBG_Dbg_mode_mask                                                        (0x0000001F)
#define SB2_DBG_Dbg_mode(data)                                                       (0x0000001F&((data)<<0))
#define SB2_DBG_Dbg_mode_src(data)                                                   ((0x0000001F&(data))>>0)
#define SB2_DBG_get_Dbg_mode(data)                                                   ((0x0000001F&(data))>>0)


#define SB2_DEBUG                                                                    0x1801A028
#define SB2_DEBUG_reg_addr                                                           "0xB801A028"
#define SB2_DEBUG_reg                                                                0xB801A028
#define set_SB2_DEBUG_reg(data)   (*((volatile unsigned int*) SB2_DEBUG_reg)=data)
#define get_SB2_DEBUG_reg   (*((volatile unsigned int*) SB2_DEBUG_reg))
#define SB2_DEBUG_inst_adr                                                           "0x000A"
#define SB2_DEBUG_inst                                                               0x000A
#define SB2_DEBUG_write_enable3_shift                                                (13)
#define SB2_DEBUG_write_enable3_mask                                                 (0x00002000)
#define SB2_DEBUG_write_enable3(data)                                                (0x00002000&((data)<<13))
#define SB2_DEBUG_write_enable3_src(data)                                            ((0x00002000&(data))>>13)
#define SB2_DEBUG_get_write_enable3(data)                                            ((0x00002000&(data))>>13)
#define SB2_DEBUG_sel1_shift                                                         (8)
#define SB2_DEBUG_sel1_mask                                                          (0x00001F00)
#define SB2_DEBUG_sel1(data)                                                         (0x00001F00&((data)<<8))
#define SB2_DEBUG_sel1_src(data)                                                     ((0x00001F00&(data))>>8)
#define SB2_DEBUG_get_sel1(data)                                                     ((0x00001F00&(data))>>8)
#define SB2_DEBUG_write_enable2_shift                                                (7)
#define SB2_DEBUG_write_enable2_mask                                                 (0x00000080)
#define SB2_DEBUG_write_enable2(data)                                                (0x00000080&((data)<<7))
#define SB2_DEBUG_write_enable2_src(data)                                            ((0x00000080&(data))>>7)
#define SB2_DEBUG_get_write_enable2(data)                                            ((0x00000080&(data))>>7)
#define SB2_DEBUG_sel0_shift                                                         (2)
#define SB2_DEBUG_sel0_mask                                                          (0x0000007C)
#define SB2_DEBUG_sel0(data)                                                         (0x0000007C&((data)<<2))
#define SB2_DEBUG_sel0_src(data)                                                     ((0x0000007C&(data))>>2)
#define SB2_DEBUG_get_sel0(data)                                                     ((0x0000007C&(data))>>2)
#define SB2_DEBUG_write_enable1_shift                                                (1)
#define SB2_DEBUG_write_enable1_mask                                                 (0x00000002)
#define SB2_DEBUG_write_enable1(data)                                                (0x00000002&((data)<<1))
#define SB2_DEBUG_write_enable1_src(data)                                            ((0x00000002&(data))>>1)
#define SB2_DEBUG_get_write_enable1(data)                                            ((0x00000002&(data))>>1)
#define SB2_DEBUG_enable_shift                                                       (0)
#define SB2_DEBUG_enable_mask                                                        (0x00000001)
#define SB2_DEBUG_enable(data)                                                       (0x00000001&((data)<<0))
#define SB2_DEBUG_enable_src(data)                                                   ((0x00000001&(data))>>0)
#define SB2_DEBUG_get_enable(data)                                                   ((0x00000001&(data))>>0)


#define SB2_PCI_BASE_0                                                               0x1801A030
#define SB2_PCI_BASE_1                                                               0x1801A034
#define SB2_PCI_BASE_2                                                               0x1801A038
#define SB2_PCI_BASE_3                                                               0x1801A03C
#define SB2_PCI_BASE_0_reg_addr                                                      "0xB801A030"
#define SB2_PCI_BASE_1_reg_addr                                                      "0xB801A034"
#define SB2_PCI_BASE_2_reg_addr                                                      "0xB801A038"
#define SB2_PCI_BASE_3_reg_addr                                                      "0xB801A03C"
#define SB2_PCI_BASE_0_reg                                                           0xB801A030
#define SB2_PCI_BASE_1_reg                                                           0xB801A034
#define SB2_PCI_BASE_2_reg                                                           0xB801A038
#define SB2_PCI_BASE_3_reg                                                           0xB801A03C
#define set_SB2_PCI_BASE_0_reg(data)   (*((volatile unsigned int*) SB2_PCI_BASE_0_reg)=data)
#define set_SB2_PCI_BASE_1_reg(data)   (*((volatile unsigned int*) SB2_PCI_BASE_1_reg)=data)
#define set_SB2_PCI_BASE_2_reg(data)   (*((volatile unsigned int*) SB2_PCI_BASE_2_reg)=data)
#define set_SB2_PCI_BASE_3_reg(data)   (*((volatile unsigned int*) SB2_PCI_BASE_3_reg)=data)
#define get_SB2_PCI_BASE_0_reg   (*((volatile unsigned int*) SB2_PCI_BASE_0_reg))
#define get_SB2_PCI_BASE_1_reg   (*((volatile unsigned int*) SB2_PCI_BASE_1_reg))
#define get_SB2_PCI_BASE_2_reg   (*((volatile unsigned int*) SB2_PCI_BASE_2_reg))
#define get_SB2_PCI_BASE_3_reg   (*((volatile unsigned int*) SB2_PCI_BASE_3_reg))
#define SB2_PCI_BASE_0_inst_adr                                                      "0x000C"
#define SB2_PCI_BASE_1_inst_adr                                                      "0x000D"
#define SB2_PCI_BASE_2_inst_adr                                                      "0x000E"
#define SB2_PCI_BASE_3_inst_adr                                                      "0x000F"
#define SB2_PCI_BASE_0_inst                                                          0x000C
#define SB2_PCI_BASE_1_inst                                                          0x000D
#define SB2_PCI_BASE_2_inst                                                          0x000E
#define SB2_PCI_BASE_3_inst                                                          0x000F
#define SB2_PCI_BASE_addr_shift                                                      (0)
#define SB2_PCI_BASE_addr_mask                                                       (0xFFFFFFFF)
#define SB2_PCI_BASE_addr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define SB2_PCI_BASE_addr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define SB2_PCI_BASE_get_addr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define SB2_PCI_MASK_0                                                               0x1801A040
#define SB2_PCI_MASK_1                                                               0x1801A044
#define SB2_PCI_MASK_2                                                               0x1801A048
#define SB2_PCI_MASK_3                                                               0x1801A04C
#define SB2_PCI_MASK_0_reg_addr                                                      "0xB801A040"
#define SB2_PCI_MASK_1_reg_addr                                                      "0xB801A044"
#define SB2_PCI_MASK_2_reg_addr                                                      "0xB801A048"
#define SB2_PCI_MASK_3_reg_addr                                                      "0xB801A04C"
#define SB2_PCI_MASK_0_reg                                                           0xB801A040
#define SB2_PCI_MASK_1_reg                                                           0xB801A044
#define SB2_PCI_MASK_2_reg                                                           0xB801A048
#define SB2_PCI_MASK_3_reg                                                           0xB801A04C
#define set_SB2_PCI_MASK_0_reg(data)   (*((volatile unsigned int*) SB2_PCI_MASK_0_reg)=data)
#define set_SB2_PCI_MASK_1_reg(data)   (*((volatile unsigned int*) SB2_PCI_MASK_1_reg)=data)
#define set_SB2_PCI_MASK_2_reg(data)   (*((volatile unsigned int*) SB2_PCI_MASK_2_reg)=data)
#define set_SB2_PCI_MASK_3_reg(data)   (*((volatile unsigned int*) SB2_PCI_MASK_3_reg)=data)
#define get_SB2_PCI_MASK_0_reg   (*((volatile unsigned int*) SB2_PCI_MASK_0_reg))
#define get_SB2_PCI_MASK_1_reg   (*((volatile unsigned int*) SB2_PCI_MASK_1_reg))
#define get_SB2_PCI_MASK_2_reg   (*((volatile unsigned int*) SB2_PCI_MASK_2_reg))
#define get_SB2_PCI_MASK_3_reg   (*((volatile unsigned int*) SB2_PCI_MASK_3_reg))
#define SB2_PCI_MASK_0_inst_adr                                                      "0x0010"
#define SB2_PCI_MASK_1_inst_adr                                                      "0x0011"
#define SB2_PCI_MASK_2_inst_adr                                                      "0x0012"
#define SB2_PCI_MASK_3_inst_adr                                                      "0x0013"
#define SB2_PCI_MASK_0_inst                                                          0x0010
#define SB2_PCI_MASK_1_inst                                                          0x0011
#define SB2_PCI_MASK_2_inst                                                          0x0012
#define SB2_PCI_MASK_3_inst                                                          0x0013
#define SB2_PCI_MASK_addr_shift                                                      (0)
#define SB2_PCI_MASK_addr_mask                                                       (0xFFFFFFFF)
#define SB2_PCI_MASK_addr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define SB2_PCI_MASK_addr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define SB2_PCI_MASK_get_addr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define SB2_PCI_TRANS_0                                                              0x1801A050
#define SB2_PCI_TRANS_1                                                              0x1801A054
#define SB2_PCI_TRANS_2                                                              0x1801A058
#define SB2_PCI_TRANS_3                                                              0x1801A05C
#define SB2_PCI_TRANS_0_reg_addr                                                     "0xB801A050"
#define SB2_PCI_TRANS_1_reg_addr                                                     "0xB801A054"
#define SB2_PCI_TRANS_2_reg_addr                                                     "0xB801A058"
#define SB2_PCI_TRANS_3_reg_addr                                                     "0xB801A05C"
#define SB2_PCI_TRANS_0_reg                                                          0xB801A050
#define SB2_PCI_TRANS_1_reg                                                          0xB801A054
#define SB2_PCI_TRANS_2_reg                                                          0xB801A058
#define SB2_PCI_TRANS_3_reg                                                          0xB801A05C
#define set_SB2_PCI_TRANS_0_reg(data)   (*((volatile unsigned int*) SB2_PCI_TRANS_0_reg)=data)
#define set_SB2_PCI_TRANS_1_reg(data)   (*((volatile unsigned int*) SB2_PCI_TRANS_1_reg)=data)
#define set_SB2_PCI_TRANS_2_reg(data)   (*((volatile unsigned int*) SB2_PCI_TRANS_2_reg)=data)
#define set_SB2_PCI_TRANS_3_reg(data)   (*((volatile unsigned int*) SB2_PCI_TRANS_3_reg)=data)
#define get_SB2_PCI_TRANS_0_reg   (*((volatile unsigned int*) SB2_PCI_TRANS_0_reg))
#define get_SB2_PCI_TRANS_1_reg   (*((volatile unsigned int*) SB2_PCI_TRANS_1_reg))
#define get_SB2_PCI_TRANS_2_reg   (*((volatile unsigned int*) SB2_PCI_TRANS_2_reg))
#define get_SB2_PCI_TRANS_3_reg   (*((volatile unsigned int*) SB2_PCI_TRANS_3_reg))
#define SB2_PCI_TRANS_0_inst_adr                                                     "0x0014"
#define SB2_PCI_TRANS_1_inst_adr                                                     "0x0015"
#define SB2_PCI_TRANS_2_inst_adr                                                     "0x0016"
#define SB2_PCI_TRANS_3_inst_adr                                                     "0x0017"
#define SB2_PCI_TRANS_0_inst                                                         0x0014
#define SB2_PCI_TRANS_1_inst                                                         0x0015
#define SB2_PCI_TRANS_2_inst                                                         0x0016
#define SB2_PCI_TRANS_3_inst                                                         0x0017
#define SB2_PCI_TRANS_addr_shift                                                     (0)
#define SB2_PCI_TRANS_addr_mask                                                      (0xFFFFFFFF)
#define SB2_PCI_TRANS_addr(data)                                                     (0xFFFFFFFF&((data)<<0))
#define SB2_PCI_TRANS_addr_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define SB2_PCI_TRANS_get_addr(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define SB2_PCI_CTRL                                                                 0x1801A060
#define SB2_PCI_CTRL_reg_addr                                                        "0xB801A060"
#define SB2_PCI_CTRL_reg                                                             0xB801A060
#define set_SB2_PCI_CTRL_reg(data)   (*((volatile unsigned int*) SB2_PCI_CTRL_reg)=data)
#define get_SB2_PCI_CTRL_reg   (*((volatile unsigned int*) SB2_PCI_CTRL_reg))
#define SB2_PCI_CTRL_inst_adr                                                        "0x0018"
#define SB2_PCI_CTRL_inst                                                            0x0018
#define SB2_PCI_CTRL_write_enable4_shift                                             (11)
#define SB2_PCI_CTRL_write_enable4_mask                                              (0x00000800)
#define SB2_PCI_CTRL_write_enable4(data)                                             (0x00000800&((data)<<11))
#define SB2_PCI_CTRL_write_enable4_src(data)                                         ((0x00000800&(data))>>11)
#define SB2_PCI_CTRL_get_write_enable4(data)                                         ((0x00000800&(data))>>11)
#define SB2_PCI_CTRL_mode3_shift                                                     (10)
#define SB2_PCI_CTRL_mode3_mask                                                      (0x00000400)
#define SB2_PCI_CTRL_mode3(data)                                                     (0x00000400&((data)<<10))
#define SB2_PCI_CTRL_mode3_src(data)                                                 ((0x00000400&(data))>>10)
#define SB2_PCI_CTRL_get_mode3(data)                                                 ((0x00000400&(data))>>10)
#define SB2_PCI_CTRL_en3_shift                                                       (9)
#define SB2_PCI_CTRL_en3_mask                                                        (0x00000200)
#define SB2_PCI_CTRL_en3(data)                                                       (0x00000200&((data)<<9))
#define SB2_PCI_CTRL_en3_src(data)                                                   ((0x00000200&(data))>>9)
#define SB2_PCI_CTRL_get_en3(data)                                                   ((0x00000200&(data))>>9)
#define SB2_PCI_CTRL_write_enable3_shift                                             (8)
#define SB2_PCI_CTRL_write_enable3_mask                                              (0x00000100)
#define SB2_PCI_CTRL_write_enable3(data)                                             (0x00000100&((data)<<8))
#define SB2_PCI_CTRL_write_enable3_src(data)                                         ((0x00000100&(data))>>8)
#define SB2_PCI_CTRL_get_write_enable3(data)                                         ((0x00000100&(data))>>8)
#define SB2_PCI_CTRL_mode2_shift                                                     (7)
#define SB2_PCI_CTRL_mode2_mask                                                      (0x00000080)
#define SB2_PCI_CTRL_mode2(data)                                                     (0x00000080&((data)<<7))
#define SB2_PCI_CTRL_mode2_src(data)                                                 ((0x00000080&(data))>>7)
#define SB2_PCI_CTRL_get_mode2(data)                                                 ((0x00000080&(data))>>7)
#define SB2_PCI_CTRL_en2_shift                                                       (6)
#define SB2_PCI_CTRL_en2_mask                                                        (0x00000040)
#define SB2_PCI_CTRL_en2(data)                                                       (0x00000040&((data)<<6))
#define SB2_PCI_CTRL_en2_src(data)                                                   ((0x00000040&(data))>>6)
#define SB2_PCI_CTRL_get_en2(data)                                                   ((0x00000040&(data))>>6)
#define SB2_PCI_CTRL_write_enable2_shift                                             (5)
#define SB2_PCI_CTRL_write_enable2_mask                                              (0x00000020)
#define SB2_PCI_CTRL_write_enable2(data)                                             (0x00000020&((data)<<5))
#define SB2_PCI_CTRL_write_enable2_src(data)                                         ((0x00000020&(data))>>5)
#define SB2_PCI_CTRL_get_write_enable2(data)                                         ((0x00000020&(data))>>5)
#define SB2_PCI_CTRL_mode1_shift                                                     (4)
#define SB2_PCI_CTRL_mode1_mask                                                      (0x00000010)
#define SB2_PCI_CTRL_mode1(data)                                                     (0x00000010&((data)<<4))
#define SB2_PCI_CTRL_mode1_src(data)                                                 ((0x00000010&(data))>>4)
#define SB2_PCI_CTRL_get_mode1(data)                                                 ((0x00000010&(data))>>4)
#define SB2_PCI_CTRL_en1_shift                                                       (3)
#define SB2_PCI_CTRL_en1_mask                                                        (0x00000008)
#define SB2_PCI_CTRL_en1(data)                                                       (0x00000008&((data)<<3))
#define SB2_PCI_CTRL_en1_src(data)                                                   ((0x00000008&(data))>>3)
#define SB2_PCI_CTRL_get_en1(data)                                                   ((0x00000008&(data))>>3)
#define SB2_PCI_CTRL_write_enable1_shift                                             (2)
#define SB2_PCI_CTRL_write_enable1_mask                                              (0x00000004)
#define SB2_PCI_CTRL_write_enable1(data)                                             (0x00000004&((data)<<2))
#define SB2_PCI_CTRL_write_enable1_src(data)                                         ((0x00000004&(data))>>2)
#define SB2_PCI_CTRL_get_write_enable1(data)                                         ((0x00000004&(data))>>2)
#define SB2_PCI_CTRL_mode0_shift                                                     (1)
#define SB2_PCI_CTRL_mode0_mask                                                      (0x00000002)
#define SB2_PCI_CTRL_mode0(data)                                                     (0x00000002&((data)<<1))
#define SB2_PCI_CTRL_mode0_src(data)                                                 ((0x00000002&(data))>>1)
#define SB2_PCI_CTRL_get_mode0(data)                                                 ((0x00000002&(data))>>1)
#define SB2_PCI_CTRL_en0_shift                                                       (0)
#define SB2_PCI_CTRL_en0_mask                                                        (0x00000001)
#define SB2_PCI_CTRL_en0(data)                                                       (0x00000001&((data)<<0))
#define SB2_PCI_CTRL_en0_src(data)                                                   ((0x00000001&(data))>>0)
#define SB2_PCI_CTRL_get_en0(data)                                                   ((0x00000001&(data))>>0)


#define SB2_LEXRA_CTRL                                                               0x1801A070
#define SB2_LEXRA_CTRL_reg_addr                                                      "0xB801A070"
#define SB2_LEXRA_CTRL_reg                                                           0xB801A070
#define set_SB2_LEXRA_CTRL_reg(data)   (*((volatile unsigned int*) SB2_LEXRA_CTRL_reg)=data)
#define get_SB2_LEXRA_CTRL_reg   (*((volatile unsigned int*) SB2_LEXRA_CTRL_reg))
#define SB2_LEXRA_CTRL_inst_adr                                                      "0x001C"
#define SB2_LEXRA_CTRL_inst                                                          0x001C
#define SB2_LEXRA_CTRL_speed_up_shift                                                (0)
#define SB2_LEXRA_CTRL_speed_up_mask                                                 (0x00000001)
#define SB2_LEXRA_CTRL_speed_up(data)                                                (0x00000001&((data)<<0))
#define SB2_LEXRA_CTRL_speed_up_src(data)                                            ((0x00000001&(data))>>0)
#define SB2_LEXRA_CTRL_get_speed_up(data)                                            ((0x00000001&(data))>>0)


#define SB2_CPU_CTRL                                                                 0x1801A074
#define SB2_CPU_CTRL_reg_addr                                                        "0xB801A074"
#define SB2_CPU_CTRL_reg                                                             0xB801A074
#define set_SB2_CPU_CTRL_reg(data)   (*((volatile unsigned int*) SB2_CPU_CTRL_reg)=data)
#define get_SB2_CPU_CTRL_reg   (*((volatile unsigned int*) SB2_CPU_CTRL_reg))
#define SB2_CPU_CTRL_inst_adr                                                        "0x001D"
#define SB2_CPU_CTRL_inst                                                            0x001D
#define SB2_CPU_CTRL_sample_byte_en_kcpu_shift                                       (1)
#define SB2_CPU_CTRL_sample_byte_en_kcpu_mask                                        (0x00000002)
#define SB2_CPU_CTRL_sample_byte_en_kcpu(data)                                       (0x00000002&((data)<<1))
#define SB2_CPU_CTRL_sample_byte_en_kcpu_src(data)                                   ((0x00000002&(data))>>1)
#define SB2_CPU_CTRL_get_sample_byte_en_kcpu(data)                                   ((0x00000002&(data))>>1)
#define SB2_CPU_CTRL_sample_byte_en_scpu_shift                                       (0)
#define SB2_CPU_CTRL_sample_byte_en_scpu_mask                                        (0x00000001)
#define SB2_CPU_CTRL_sample_byte_en_scpu(data)                                       (0x00000001&((data)<<0))
#define SB2_CPU_CTRL_sample_byte_en_scpu_src(data)                                   ((0x00000001&(data))>>0)
#define SB2_CPU_CTRL_get_sample_byte_en_scpu(data)                                   ((0x00000001&(data))>>0)


#define SB2_ENDIAN                                                                   0x1801A100
#define SB2_ENDIAN_reg_addr                                                          "0xB801A100"
#define SB2_ENDIAN_reg                                                               0xB801A100
#define set_SB2_ENDIAN_reg(data)   (*((volatile unsigned int*) SB2_ENDIAN_reg)=data)
#define get_SB2_ENDIAN_reg   (*((volatile unsigned int*) SB2_ENDIAN_reg))
#define SB2_ENDIAN_inst_adr                                                          "0x0040"
#define SB2_ENDIAN_inst                                                              0x0040
#define SB2_ENDIAN_si_endian_kcpu_shift                                              (1)
#define SB2_ENDIAN_si_endian_kcpu_mask                                               (0x00000002)
#define SB2_ENDIAN_si_endian_kcpu(data)                                              (0x00000002&((data)<<1))
#define SB2_ENDIAN_si_endian_kcpu_src(data)                                          ((0x00000002&(data))>>1)
#define SB2_ENDIAN_get_si_endian_kcpu(data)                                          ((0x00000002&(data))>>1)
#define SB2_ENDIAN_si_endian_shift                                                   (0)
#define SB2_ENDIAN_si_endian_mask                                                    (0x00000001)
#define SB2_ENDIAN_si_endian(data)                                                   (0x00000001&((data)<<0))
#define SB2_ENDIAN_si_endian_src(data)                                               ((0x00000001&(data))>>0)
#define SB2_ENDIAN_get_si_endian(data)                                               ((0x00000001&(data))>>0)


#define SB2_CPU_INT                                                                  0x1801A104
#define SB2_CPU_INT_reg_addr                                                         "0xB801A104"
#define SB2_CPU_INT_reg                                                              0xB801A104
#define set_SB2_CPU_INT_reg(data)   (*((volatile unsigned int*) SB2_CPU_INT_reg)=data)
#define get_SB2_CPU_INT_reg   (*((volatile unsigned int*) SB2_CPU_INT_reg))
#define SB2_CPU_INT_inst_adr                                                         "0x0041"
#define SB2_CPU_INT_inst                                                             0x0041
#define SB2_CPU_INT_int_vk_shift                                                     (12)
#define SB2_CPU_INT_int_vk_mask                                                      (0x00001000)
#define SB2_CPU_INT_int_vk(data)                                                     (0x00001000&((data)<<12))
#define SB2_CPU_INT_int_vk_src(data)                                                 ((0x00001000&(data))>>12)
#define SB2_CPU_INT_get_int_vk(data)                                                 ((0x00001000&(data))>>12)
#define SB2_CPU_INT_int_ak_shift                                                     (11)
#define SB2_CPU_INT_int_ak_mask                                                      (0x00000800)
#define SB2_CPU_INT_int_ak(data)                                                     (0x00000800&((data)<<11))
#define SB2_CPU_INT_int_ak_src(data)                                                 ((0x00000800&(data))>>11)
#define SB2_CPU_INT_get_int_ak(data)                                                 ((0x00000800&(data))>>11)
#define SB2_CPU_INT_int_sk_shift                                                     (10)
#define SB2_CPU_INT_int_sk_mask                                                      (0x00000400)
#define SB2_CPU_INT_int_sk(data)                                                     (0x00000400&((data)<<10))
#define SB2_CPU_INT_int_sk_src(data)                                                 ((0x00000400&(data))>>10)
#define SB2_CPU_INT_get_int_sk(data)                                                 ((0x00000400&(data))>>10)
#define SB2_CPU_INT_int_kv_shift                                                     (9)
#define SB2_CPU_INT_int_kv_mask                                                      (0x00000200)
#define SB2_CPU_INT_int_kv(data)                                                     (0x00000200&((data)<<9))
#define SB2_CPU_INT_int_kv_src(data)                                                 ((0x00000200&(data))>>9)
#define SB2_CPU_INT_get_int_kv(data)                                                 ((0x00000200&(data))>>9)
#define SB2_CPU_INT_int_ka_shift                                                     (8)
#define SB2_CPU_INT_int_ka_mask                                                      (0x00000100)
#define SB2_CPU_INT_int_ka(data)                                                     (0x00000100&((data)<<8))
#define SB2_CPU_INT_int_ka_src(data)                                                 ((0x00000100&(data))>>8)
#define SB2_CPU_INT_get_int_ka(data)                                                 ((0x00000100&(data))>>8)
#define SB2_CPU_INT_int_ks_shift                                                     (7)
#define SB2_CPU_INT_int_ks_mask                                                      (0x00000080)
#define SB2_CPU_INT_int_ks(data)                                                     (0x00000080&((data)<<7))
#define SB2_CPU_INT_int_ks_src(data)                                                 ((0x00000080&(data))>>7)
#define SB2_CPU_INT_get_int_ks(data)                                                 ((0x00000080&(data))>>7)
#define SB2_CPU_INT_int_vs_shift                                                     (6)
#define SB2_CPU_INT_int_vs_mask                                                      (0x00000040)
#define SB2_CPU_INT_int_vs(data)                                                     (0x00000040&((data)<<6))
#define SB2_CPU_INT_int_vs_src(data)                                                 ((0x00000040&(data))>>6)
#define SB2_CPU_INT_get_int_vs(data)                                                 ((0x00000040&(data))>>6)
#define SB2_CPU_INT_int_va_shift                                                     (5)
#define SB2_CPU_INT_int_va_mask                                                      (0x00000020)
#define SB2_CPU_INT_int_va(data)                                                     (0x00000020&((data)<<5))
#define SB2_CPU_INT_int_va_src(data)                                                 ((0x00000020&(data))>>5)
#define SB2_CPU_INT_get_int_va(data)                                                 ((0x00000020&(data))>>5)
#define SB2_CPU_INT_int_av_shift                                                     (4)
#define SB2_CPU_INT_int_av_mask                                                      (0x00000010)
#define SB2_CPU_INT_int_av(data)                                                     (0x00000010&((data)<<4))
#define SB2_CPU_INT_int_av_src(data)                                                 ((0x00000010&(data))>>4)
#define SB2_CPU_INT_get_int_av(data)                                                 ((0x00000010&(data))>>4)
#define SB2_CPU_INT_int_as_shift                                                     (3)
#define SB2_CPU_INT_int_as_mask                                                      (0x00000008)
#define SB2_CPU_INT_int_as(data)                                                     (0x00000008&((data)<<3))
#define SB2_CPU_INT_int_as_src(data)                                                 ((0x00000008&(data))>>3)
#define SB2_CPU_INT_get_int_as(data)                                                 ((0x00000008&(data))>>3)
#define SB2_CPU_INT_int_sv_shift                                                     (2)
#define SB2_CPU_INT_int_sv_mask                                                      (0x00000004)
#define SB2_CPU_INT_int_sv(data)                                                     (0x00000004&((data)<<2))
#define SB2_CPU_INT_int_sv_src(data)                                                 ((0x00000004&(data))>>2)
#define SB2_CPU_INT_get_int_sv(data)                                                 ((0x00000004&(data))>>2)
#define SB2_CPU_INT_int_sa_shift                                                     (1)
#define SB2_CPU_INT_int_sa_mask                                                      (0x00000002)
#define SB2_CPU_INT_int_sa(data)                                                     (0x00000002&((data)<<1))
#define SB2_CPU_INT_int_sa_src(data)                                                 ((0x00000002&(data))>>1)
#define SB2_CPU_INT_get_int_sa(data)                                                 ((0x00000002&(data))>>1)
#define SB2_CPU_INT_write_data_shift                                                 (0)
#define SB2_CPU_INT_write_data_mask                                                  (0x00000001)
#define SB2_CPU_INT_write_data(data)                                                 (0x00000001&((data)<<0))
#define SB2_CPU_INT_write_data_src(data)                                             ((0x00000001&(data))>>0)
#define SB2_CPU_INT_get_write_data(data)                                             ((0x00000001&(data))>>0)


#define SB2_CPU_INT_EN                                                               0x1801A108
#define SB2_CPU_INT_EN_reg_addr                                                      "0xB801A108"
#define SB2_CPU_INT_EN_reg                                                           0xB801A108
#define set_SB2_CPU_INT_EN_reg(data)   (*((volatile unsigned int*) SB2_CPU_INT_EN_reg)=data)
#define get_SB2_CPU_INT_EN_reg   (*((volatile unsigned int*) SB2_CPU_INT_EN_reg))
#define SB2_CPU_INT_EN_inst_adr                                                      "0x0042"
#define SB2_CPU_INT_EN_inst                                                          0x0042
#define SB2_CPU_INT_EN_int_vk_en_shift                                               (12)
#define SB2_CPU_INT_EN_int_vk_en_mask                                                (0x00001000)
#define SB2_CPU_INT_EN_int_vk_en(data)                                               (0x00001000&((data)<<12))
#define SB2_CPU_INT_EN_int_vk_en_src(data)                                           ((0x00001000&(data))>>12)
#define SB2_CPU_INT_EN_get_int_vk_en(data)                                           ((0x00001000&(data))>>12)
#define SB2_CPU_INT_EN_int_ak_en_shift                                               (11)
#define SB2_CPU_INT_EN_int_ak_en_mask                                                (0x00000800)
#define SB2_CPU_INT_EN_int_ak_en(data)                                               (0x00000800&((data)<<11))
#define SB2_CPU_INT_EN_int_ak_en_src(data)                                           ((0x00000800&(data))>>11)
#define SB2_CPU_INT_EN_get_int_ak_en(data)                                           ((0x00000800&(data))>>11)
#define SB2_CPU_INT_EN_int_sk_en_shift                                               (10)
#define SB2_CPU_INT_EN_int_sk_en_mask                                                (0x00000400)
#define SB2_CPU_INT_EN_int_sk_en(data)                                               (0x00000400&((data)<<10))
#define SB2_CPU_INT_EN_int_sk_en_src(data)                                           ((0x00000400&(data))>>10)
#define SB2_CPU_INT_EN_get_int_sk_en(data)                                           ((0x00000400&(data))>>10)
#define SB2_CPU_INT_EN_int_kv_en_shift                                               (9)
#define SB2_CPU_INT_EN_int_kv_en_mask                                                (0x00000200)
#define SB2_CPU_INT_EN_int_kv_en(data)                                               (0x00000200&((data)<<9))
#define SB2_CPU_INT_EN_int_kv_en_src(data)                                           ((0x00000200&(data))>>9)
#define SB2_CPU_INT_EN_get_int_kv_en(data)                                           ((0x00000200&(data))>>9)
#define SB2_CPU_INT_EN_int_ka_en_shift                                               (8)
#define SB2_CPU_INT_EN_int_ka_en_mask                                                (0x00000100)
#define SB2_CPU_INT_EN_int_ka_en(data)                                               (0x00000100&((data)<<8))
#define SB2_CPU_INT_EN_int_ka_en_src(data)                                           ((0x00000100&(data))>>8)
#define SB2_CPU_INT_EN_get_int_ka_en(data)                                           ((0x00000100&(data))>>8)
#define SB2_CPU_INT_EN_int_ks_en_shift                                               (7)
#define SB2_CPU_INT_EN_int_ks_en_mask                                                (0x00000080)
#define SB2_CPU_INT_EN_int_ks_en(data)                                               (0x00000080&((data)<<7))
#define SB2_CPU_INT_EN_int_ks_en_src(data)                                           ((0x00000080&(data))>>7)
#define SB2_CPU_INT_EN_get_int_ks_en(data)                                           ((0x00000080&(data))>>7)
#define SB2_CPU_INT_EN_int_vs_en_shift                                               (6)
#define SB2_CPU_INT_EN_int_vs_en_mask                                                (0x00000040)
#define SB2_CPU_INT_EN_int_vs_en(data)                                               (0x00000040&((data)<<6))
#define SB2_CPU_INT_EN_int_vs_en_src(data)                                           ((0x00000040&(data))>>6)
#define SB2_CPU_INT_EN_get_int_vs_en(data)                                           ((0x00000040&(data))>>6)
#define SB2_CPU_INT_EN_int_va_en_shift                                               (5)
#define SB2_CPU_INT_EN_int_va_en_mask                                                (0x00000020)
#define SB2_CPU_INT_EN_int_va_en(data)                                               (0x00000020&((data)<<5))
#define SB2_CPU_INT_EN_int_va_en_src(data)                                           ((0x00000020&(data))>>5)
#define SB2_CPU_INT_EN_get_int_va_en(data)                                           ((0x00000020&(data))>>5)
#define SB2_CPU_INT_EN_int_av_en_shift                                               (4)
#define SB2_CPU_INT_EN_int_av_en_mask                                                (0x00000010)
#define SB2_CPU_INT_EN_int_av_en(data)                                               (0x00000010&((data)<<4))
#define SB2_CPU_INT_EN_int_av_en_src(data)                                           ((0x00000010&(data))>>4)
#define SB2_CPU_INT_EN_get_int_av_en(data)                                           ((0x00000010&(data))>>4)
#define SB2_CPU_INT_EN_int_as_en_shift                                               (3)
#define SB2_CPU_INT_EN_int_as_en_mask                                                (0x00000008)
#define SB2_CPU_INT_EN_int_as_en(data)                                               (0x00000008&((data)<<3))
#define SB2_CPU_INT_EN_int_as_en_src(data)                                           ((0x00000008&(data))>>3)
#define SB2_CPU_INT_EN_get_int_as_en(data)                                           ((0x00000008&(data))>>3)
#define SB2_CPU_INT_EN_int_sv_en_shift                                               (2)
#define SB2_CPU_INT_EN_int_sv_en_mask                                                (0x00000004)
#define SB2_CPU_INT_EN_int_sv_en(data)                                               (0x00000004&((data)<<2))
#define SB2_CPU_INT_EN_int_sv_en_src(data)                                           ((0x00000004&(data))>>2)
#define SB2_CPU_INT_EN_get_int_sv_en(data)                                           ((0x00000004&(data))>>2)
#define SB2_CPU_INT_EN_int_sa_en_shift                                               (1)
#define SB2_CPU_INT_EN_int_sa_en_mask                                                (0x00000002)
#define SB2_CPU_INT_EN_int_sa_en(data)                                               (0x00000002&((data)<<1))
#define SB2_CPU_INT_EN_int_sa_en_src(data)                                           ((0x00000002&(data))>>1)
#define SB2_CPU_INT_EN_get_int_sa_en(data)                                           ((0x00000002&(data))>>1)
#define SB2_CPU_INT_EN_write_data_shift                                              (0)
#define SB2_CPU_INT_EN_write_data_mask                                               (0x00000001)
#define SB2_CPU_INT_EN_write_data(data)                                              (0x00000001&((data)<<0))
#define SB2_CPU_INT_EN_write_data_src(data)                                          ((0x00000001&(data))>>0)
#define SB2_CPU_INT_EN_get_write_data(data)                                          ((0x00000001&(data))>>0)


#define SB2_PM_CTRL                                                                  0x1801A10C
#define SB2_PM_CTRL_reg_addr                                                         "0xB801A10C"
#define SB2_PM_CTRL_reg                                                              0xB801A10C
#define set_SB2_PM_CTRL_reg(data)   (*((volatile unsigned int*) SB2_PM_CTRL_reg)=data)
#define get_SB2_PM_CTRL_reg   (*((volatile unsigned int*) SB2_PM_CTRL_reg))
#define SB2_PM_CTRL_inst_adr                                                         "0x0043"
#define SB2_PM_CTRL_inst                                                             0x0043
#define SB2_PM_CTRL_m3_run_shift                                                     (28)
#define SB2_PM_CTRL_m3_run_mask                                                      (0x10000000)
#define SB2_PM_CTRL_m3_run(data)                                                     (0x10000000&((data)<<28))
#define SB2_PM_CTRL_m3_run_src(data)                                                 ((0x10000000&(data))>>28)
#define SB2_PM_CTRL_get_m3_run(data)                                                 ((0x10000000&(data))>>28)
#define SB2_PM_CTRL_m3_sel_shift                                                     (24)
#define SB2_PM_CTRL_m3_sel_mask                                                      (0x0F000000)
#define SB2_PM_CTRL_m3_sel(data)                                                     (0x0F000000&((data)<<24))
#define SB2_PM_CTRL_m3_sel_src(data)                                                 ((0x0F000000&(data))>>24)
#define SB2_PM_CTRL_get_m3_sel(data)                                                 ((0x0F000000&(data))>>24)
#define SB2_PM_CTRL_m2_run_shift                                                     (20)
#define SB2_PM_CTRL_m2_run_mask                                                      (0x00100000)
#define SB2_PM_CTRL_m2_run(data)                                                     (0x00100000&((data)<<20))
#define SB2_PM_CTRL_m2_run_src(data)                                                 ((0x00100000&(data))>>20)
#define SB2_PM_CTRL_get_m2_run(data)                                                 ((0x00100000&(data))>>20)
#define SB2_PM_CTRL_m2_sel_shift                                                     (16)
#define SB2_PM_CTRL_m2_sel_mask                                                      (0x000F0000)
#define SB2_PM_CTRL_m2_sel(data)                                                     (0x000F0000&((data)<<16))
#define SB2_PM_CTRL_m2_sel_src(data)                                                 ((0x000F0000&(data))>>16)
#define SB2_PM_CTRL_get_m2_sel(data)                                                 ((0x000F0000&(data))>>16)
#define SB2_PM_CTRL_m1_run_shift                                                     (12)
#define SB2_PM_CTRL_m1_run_mask                                                      (0x00001000)
#define SB2_PM_CTRL_m1_run(data)                                                     (0x00001000&((data)<<12))
#define SB2_PM_CTRL_m1_run_src(data)                                                 ((0x00001000&(data))>>12)
#define SB2_PM_CTRL_get_m1_run(data)                                                 ((0x00001000&(data))>>12)
#define SB2_PM_CTRL_m1_sel_shift                                                     (8)
#define SB2_PM_CTRL_m1_sel_mask                                                      (0x00000F00)
#define SB2_PM_CTRL_m1_sel(data)                                                     (0x00000F00&((data)<<8))
#define SB2_PM_CTRL_m1_sel_src(data)                                                 ((0x00000F00&(data))>>8)
#define SB2_PM_CTRL_get_m1_sel(data)                                                 ((0x00000F00&(data))>>8)
#define SB2_PM_CTRL_m0_run_shift                                                     (4)
#define SB2_PM_CTRL_m0_run_mask                                                      (0x00000010)
#define SB2_PM_CTRL_m0_run(data)                                                     (0x00000010&((data)<<4))
#define SB2_PM_CTRL_m0_run_src(data)                                                 ((0x00000010&(data))>>4)
#define SB2_PM_CTRL_get_m0_run(data)                                                 ((0x00000010&(data))>>4)
#define SB2_PM_CTRL_m0_sel_shift                                                     (0)
#define SB2_PM_CTRL_m0_sel_mask                                                      (0x0000000F)
#define SB2_PM_CTRL_m0_sel(data)                                                     (0x0000000F&((data)<<0))
#define SB2_PM_CTRL_m0_sel_src(data)                                                 ((0x0000000F&(data))>>0)
#define SB2_PM_CTRL_get_m0_sel(data)                                                 ((0x0000000F&(data))>>0)


#define SB2_PM_CNT_0                                                                 0x1801A110
#define SB2_PM_CNT_1                                                                 0x1801A114
#define SB2_PM_CNT_2                                                                 0x1801A118
#define SB2_PM_CNT_3                                                                 0x1801A11C
#define SB2_PM_CNT_0_reg_addr                                                        "0xB801A110"
#define SB2_PM_CNT_1_reg_addr                                                        "0xB801A114"
#define SB2_PM_CNT_2_reg_addr                                                        "0xB801A118"
#define SB2_PM_CNT_3_reg_addr                                                        "0xB801A11C"
#define SB2_PM_CNT_0_reg                                                             0xB801A110
#define SB2_PM_CNT_1_reg                                                             0xB801A114
#define SB2_PM_CNT_2_reg                                                             0xB801A118
#define SB2_PM_CNT_3_reg                                                             0xB801A11C
#define set_SB2_PM_CNT_0_reg(data)   (*((volatile unsigned int*) SB2_PM_CNT_0_reg)=data)
#define set_SB2_PM_CNT_1_reg(data)   (*((volatile unsigned int*) SB2_PM_CNT_1_reg)=data)
#define set_SB2_PM_CNT_2_reg(data)   (*((volatile unsigned int*) SB2_PM_CNT_2_reg)=data)
#define set_SB2_PM_CNT_3_reg(data)   (*((volatile unsigned int*) SB2_PM_CNT_3_reg)=data)
#define get_SB2_PM_CNT_0_reg   (*((volatile unsigned int*) SB2_PM_CNT_0_reg))
#define get_SB2_PM_CNT_1_reg   (*((volatile unsigned int*) SB2_PM_CNT_1_reg))
#define get_SB2_PM_CNT_2_reg   (*((volatile unsigned int*) SB2_PM_CNT_2_reg))
#define get_SB2_PM_CNT_3_reg   (*((volatile unsigned int*) SB2_PM_CNT_3_reg))
#define SB2_PM_CNT_0_inst_adr                                                        "0x0044"
#define SB2_PM_CNT_1_inst_adr                                                        "0x0045"
#define SB2_PM_CNT_2_inst_adr                                                        "0x0046"
#define SB2_PM_CNT_3_inst_adr                                                        "0x0047"
#define SB2_PM_CNT_0_inst                                                            0x0044
#define SB2_PM_CNT_1_inst                                                            0x0045
#define SB2_PM_CNT_2_inst                                                            0x0046
#define SB2_PM_CNT_3_inst                                                            0x0047
#define SB2_PM_CNT_pm_cnt_shift                                                      (0)
#define SB2_PM_CNT_pm_cnt_mask                                                       (0xFFFFFFFF)
#define SB2_PM_CNT_pm_cnt(data)                                                      (0xFFFFFFFF&((data)<<0))
#define SB2_PM_CNT_pm_cnt_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define SB2_PM_CNT_get_pm_cnt(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define SB2_PM_THR_0                                                                 0x1801A120
#define SB2_PM_THR_1                                                                 0x1801A124
#define SB2_PM_THR_2                                                                 0x1801A128
#define SB2_PM_THR_3                                                                 0x1801A12C
#define SB2_PM_THR_0_reg_addr                                                        "0xB801A120"
#define SB2_PM_THR_1_reg_addr                                                        "0xB801A124"
#define SB2_PM_THR_2_reg_addr                                                        "0xB801A128"
#define SB2_PM_THR_3_reg_addr                                                        "0xB801A12C"
#define SB2_PM_THR_0_reg                                                             0xB801A120
#define SB2_PM_THR_1_reg                                                             0xB801A124
#define SB2_PM_THR_2_reg                                                             0xB801A128
#define SB2_PM_THR_3_reg                                                             0xB801A12C
#define set_SB2_PM_THR_0_reg(data)   (*((volatile unsigned int*) SB2_PM_THR_0_reg)=data)
#define set_SB2_PM_THR_1_reg(data)   (*((volatile unsigned int*) SB2_PM_THR_1_reg)=data)
#define set_SB2_PM_THR_2_reg(data)   (*((volatile unsigned int*) SB2_PM_THR_2_reg)=data)
#define set_SB2_PM_THR_3_reg(data)   (*((volatile unsigned int*) SB2_PM_THR_3_reg)=data)
#define get_SB2_PM_THR_0_reg   (*((volatile unsigned int*) SB2_PM_THR_0_reg))
#define get_SB2_PM_THR_1_reg   (*((volatile unsigned int*) SB2_PM_THR_1_reg))
#define get_SB2_PM_THR_2_reg   (*((volatile unsigned int*) SB2_PM_THR_2_reg))
#define get_SB2_PM_THR_3_reg   (*((volatile unsigned int*) SB2_PM_THR_3_reg))
#define SB2_PM_THR_0_inst_adr                                                        "0x0048"
#define SB2_PM_THR_1_inst_adr                                                        "0x0049"
#define SB2_PM_THR_2_inst_adr                                                        "0x004A"
#define SB2_PM_THR_3_inst_adr                                                        "0x004B"
#define SB2_PM_THR_0_inst                                                            0x0048
#define SB2_PM_THR_1_inst                                                            0x0049
#define SB2_PM_THR_2_inst                                                            0x004A
#define SB2_PM_THR_3_inst                                                            0x004B
#define SB2_PM_THR_pm_cnt_thr_shift                                                  (0)
#define SB2_PM_THR_pm_cnt_thr_mask                                                   (0xFFFFFFFF)
#define SB2_PM_THR_pm_cnt_thr(data)                                                  (0xFFFFFFFF&((data)<<0))
#define SB2_PM_THR_pm_cnt_thr_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define SB2_PM_THR_get_pm_cnt_thr(data)                                              ((0xFFFFFFFF&(data))>>0)


#define SB2_PM_INT_STAT                                                              0x1801A130
#define SB2_PM_INT_STAT_reg_addr                                                     "0xB801A130"
#define SB2_PM_INT_STAT_reg                                                          0xB801A130
#define set_SB2_PM_INT_STAT_reg(data)   (*((volatile unsigned int*) SB2_PM_INT_STAT_reg)=data)
#define get_SB2_PM_INT_STAT_reg   (*((volatile unsigned int*) SB2_PM_INT_STAT_reg))
#define SB2_PM_INT_STAT_inst_adr                                                     "0x004C"
#define SB2_PM_INT_STAT_inst                                                         0x004C
#define SB2_PM_INT_STAT_write_en4_shift                                              (7)
#define SB2_PM_INT_STAT_write_en4_mask                                               (0x00000080)
#define SB2_PM_INT_STAT_write_en4(data)                                              (0x00000080&((data)<<7))
#define SB2_PM_INT_STAT_write_en4_src(data)                                          ((0x00000080&(data))>>7)
#define SB2_PM_INT_STAT_get_write_en4(data)                                          ((0x00000080&(data))>>7)
#define SB2_PM_INT_STAT_pm3_int_shift                                                (6)
#define SB2_PM_INT_STAT_pm3_int_mask                                                 (0x00000040)
#define SB2_PM_INT_STAT_pm3_int(data)                                                (0x00000040&((data)<<6))
#define SB2_PM_INT_STAT_pm3_int_src(data)                                            ((0x00000040&(data))>>6)
#define SB2_PM_INT_STAT_get_pm3_int(data)                                            ((0x00000040&(data))>>6)
#define SB2_PM_INT_STAT_write_en3_shift                                              (5)
#define SB2_PM_INT_STAT_write_en3_mask                                               (0x00000020)
#define SB2_PM_INT_STAT_write_en3(data)                                              (0x00000020&((data)<<5))
#define SB2_PM_INT_STAT_write_en3_src(data)                                          ((0x00000020&(data))>>5)
#define SB2_PM_INT_STAT_get_write_en3(data)                                          ((0x00000020&(data))>>5)
#define SB2_PM_INT_STAT_pm2_int_shift                                                (4)
#define SB2_PM_INT_STAT_pm2_int_mask                                                 (0x00000010)
#define SB2_PM_INT_STAT_pm2_int(data)                                                (0x00000010&((data)<<4))
#define SB2_PM_INT_STAT_pm2_int_src(data)                                            ((0x00000010&(data))>>4)
#define SB2_PM_INT_STAT_get_pm2_int(data)                                            ((0x00000010&(data))>>4)
#define SB2_PM_INT_STAT_write_en2_shift                                              (3)
#define SB2_PM_INT_STAT_write_en2_mask                                               (0x00000008)
#define SB2_PM_INT_STAT_write_en2(data)                                              (0x00000008&((data)<<3))
#define SB2_PM_INT_STAT_write_en2_src(data)                                          ((0x00000008&(data))>>3)
#define SB2_PM_INT_STAT_get_write_en2(data)                                          ((0x00000008&(data))>>3)
#define SB2_PM_INT_STAT_pm1_int_shift                                                (2)
#define SB2_PM_INT_STAT_pm1_int_mask                                                 (0x00000004)
#define SB2_PM_INT_STAT_pm1_int(data)                                                (0x00000004&((data)<<2))
#define SB2_PM_INT_STAT_pm1_int_src(data)                                            ((0x00000004&(data))>>2)
#define SB2_PM_INT_STAT_get_pm1_int(data)                                            ((0x00000004&(data))>>2)
#define SB2_PM_INT_STAT_write_en1_shift                                              (1)
#define SB2_PM_INT_STAT_write_en1_mask                                               (0x00000002)
#define SB2_PM_INT_STAT_write_en1(data)                                              (0x00000002&((data)<<1))
#define SB2_PM_INT_STAT_write_en1_src(data)                                          ((0x00000002&(data))>>1)
#define SB2_PM_INT_STAT_get_write_en1(data)                                          ((0x00000002&(data))>>1)
#define SB2_PM_INT_STAT_pm0_int_shift                                                (0)
#define SB2_PM_INT_STAT_pm0_int_mask                                                 (0x00000001)
#define SB2_PM_INT_STAT_pm0_int(data)                                                (0x00000001&((data)<<0))
#define SB2_PM_INT_STAT_pm0_int_src(data)                                            ((0x00000001&(data))>>0)
#define SB2_PM_INT_STAT_get_pm0_int(data)                                            ((0x00000001&(data))>>0)


#define SB2_PM_INT_EN                                                                0x1801A134
#define SB2_PM_INT_EN_reg_addr                                                       "0xB801A134"
#define SB2_PM_INT_EN_reg                                                            0xB801A134
#define set_SB2_PM_INT_EN_reg(data)   (*((volatile unsigned int*) SB2_PM_INT_EN_reg)=data)
#define get_SB2_PM_INT_EN_reg   (*((volatile unsigned int*) SB2_PM_INT_EN_reg))
#define SB2_PM_INT_EN_inst_adr                                                       "0x004D"
#define SB2_PM_INT_EN_inst                                                           0x004D
#define SB2_PM_INT_EN_write_en4_shift                                                (7)
#define SB2_PM_INT_EN_write_en4_mask                                                 (0x00000080)
#define SB2_PM_INT_EN_write_en4(data)                                                (0x00000080&((data)<<7))
#define SB2_PM_INT_EN_write_en4_src(data)                                            ((0x00000080&(data))>>7)
#define SB2_PM_INT_EN_get_write_en4(data)                                            ((0x00000080&(data))>>7)
#define SB2_PM_INT_EN_pm3_int_en_shift                                               (6)
#define SB2_PM_INT_EN_pm3_int_en_mask                                                (0x00000040)
#define SB2_PM_INT_EN_pm3_int_en(data)                                               (0x00000040&((data)<<6))
#define SB2_PM_INT_EN_pm3_int_en_src(data)                                           ((0x00000040&(data))>>6)
#define SB2_PM_INT_EN_get_pm3_int_en(data)                                           ((0x00000040&(data))>>6)
#define SB2_PM_INT_EN_write_en3_shift                                                (5)
#define SB2_PM_INT_EN_write_en3_mask                                                 (0x00000020)
#define SB2_PM_INT_EN_write_en3(data)                                                (0x00000020&((data)<<5))
#define SB2_PM_INT_EN_write_en3_src(data)                                            ((0x00000020&(data))>>5)
#define SB2_PM_INT_EN_get_write_en3(data)                                            ((0x00000020&(data))>>5)
#define SB2_PM_INT_EN_pm2_int_en_shift                                               (4)
#define SB2_PM_INT_EN_pm2_int_en_mask                                                (0x00000010)
#define SB2_PM_INT_EN_pm2_int_en(data)                                               (0x00000010&((data)<<4))
#define SB2_PM_INT_EN_pm2_int_en_src(data)                                           ((0x00000010&(data))>>4)
#define SB2_PM_INT_EN_get_pm2_int_en(data)                                           ((0x00000010&(data))>>4)
#define SB2_PM_INT_EN_write_en2_shift                                                (3)
#define SB2_PM_INT_EN_write_en2_mask                                                 (0x00000008)
#define SB2_PM_INT_EN_write_en2(data)                                                (0x00000008&((data)<<3))
#define SB2_PM_INT_EN_write_en2_src(data)                                            ((0x00000008&(data))>>3)
#define SB2_PM_INT_EN_get_write_en2(data)                                            ((0x00000008&(data))>>3)
#define SB2_PM_INT_EN_pm1_int_en_shift                                               (2)
#define SB2_PM_INT_EN_pm1_int_en_mask                                                (0x00000004)
#define SB2_PM_INT_EN_pm1_int_en(data)                                               (0x00000004&((data)<<2))
#define SB2_PM_INT_EN_pm1_int_en_src(data)                                           ((0x00000004&(data))>>2)
#define SB2_PM_INT_EN_get_pm1_int_en(data)                                           ((0x00000004&(data))>>2)
#define SB2_PM_INT_EN_write_en1_shift                                                (1)
#define SB2_PM_INT_EN_write_en1_mask                                                 (0x00000002)
#define SB2_PM_INT_EN_write_en1(data)                                                (0x00000002&((data)<<1))
#define SB2_PM_INT_EN_write_en1_src(data)                                            ((0x00000002&(data))>>1)
#define SB2_PM_INT_EN_get_write_en1(data)                                            ((0x00000002&(data))>>1)
#define SB2_PM_INT_EN_pm0_int_en_shift                                               (0)
#define SB2_PM_INT_EN_pm0_int_en_mask                                                (0x00000001)
#define SB2_PM_INT_EN_pm0_int_en(data)                                               (0x00000001&((data)<<0))
#define SB2_PM_INT_EN_pm0_int_en_src(data)                                           ((0x00000001&(data))>>0)
#define SB2_PM_INT_EN_get_pm0_int_en(data)                                           ((0x00000001&(data))>>0)


#define SB2_DREQ_MASK                                                                0x1801A138
#define SB2_DREQ_MASK_reg_addr                                                       "0xB801A138"
#define SB2_DREQ_MASK_reg                                                            0xB801A138
#define set_SB2_DREQ_MASK_reg(data)   (*((volatile unsigned int*) SB2_DREQ_MASK_reg)=data)
#define get_SB2_DREQ_MASK_reg   (*((volatile unsigned int*) SB2_DREQ_MASK_reg))
#define SB2_DREQ_MASK_inst_adr                                                       "0x004E"
#define SB2_DREQ_MASK_inst                                                           0x004E
#define SB2_DREQ_MASK_pending_shift                                                  (5)
#define SB2_DREQ_MASK_pending_mask                                                   (0x00000020)
#define SB2_DREQ_MASK_pending(data)                                                  (0x00000020&((data)<<5))
#define SB2_DREQ_MASK_pending_src(data)                                              ((0x00000020&(data))>>5)
#define SB2_DREQ_MASK_get_pending(data)                                              ((0x00000020&(data))>>5)
#define SB2_DREQ_MASK_mask_id_shift                                                  (0)
#define SB2_DREQ_MASK_mask_id_mask                                                   (0x0000000F)
#define SB2_DREQ_MASK_mask_id(data)                                                  (0x0000000F&((data)<<0))
#define SB2_DREQ_MASK_mask_id_src(data)                                              ((0x0000000F&(data))>>0)
#define SB2_DREQ_MASK_get_mask_id(data)                                              ((0x0000000F&(data))>>0)


#define SB2_CHIP_ID                                                                  0x1801A200
#define SB2_CHIP_ID_reg_addr                                                         "0xB801A200"
#define SB2_CHIP_ID_reg                                                              0xB801A200
#define set_SB2_CHIP_ID_reg(data)   (*((volatile unsigned int*) SB2_CHIP_ID_reg)=data)
#define get_SB2_CHIP_ID_reg   (*((volatile unsigned int*) SB2_CHIP_ID_reg))
#define SB2_CHIP_ID_inst_adr                                                         "0x0080"
#define SB2_CHIP_ID_inst                                                             0x0080
#define SB2_CHIP_ID_Chip_id_shift                                                    (0)
#define SB2_CHIP_ID_Chip_id_mask                                                     (0x0000FFFF)
#define SB2_CHIP_ID_Chip_id(data)                                                    (0x0000FFFF&((data)<<0))
#define SB2_CHIP_ID_Chip_id_src(data)                                                ((0x0000FFFF&(data))>>0)
#define SB2_CHIP_ID_get_Chip_id(data)                                                ((0x0000FFFF&(data))>>0)


#define SB2_CHIP_INFO                                                                0x1801A204
#define SB2_CHIP_INFO_reg_addr                                                       "0xB801A204"
#define SB2_CHIP_INFO_reg                                                            0xB801A204
#define set_SB2_CHIP_INFO_reg(data)   (*((volatile unsigned int*) SB2_CHIP_INFO_reg)=data)
#define get_SB2_CHIP_INFO_reg   (*((volatile unsigned int*) SB2_CHIP_INFO_reg))
#define SB2_CHIP_INFO_inst_adr                                                       "0x0081"
#define SB2_CHIP_INFO_inst                                                           0x0081
#define SB2_CHIP_INFO_Revise_id_shift                                                (16)
#define SB2_CHIP_INFO_Revise_id_mask                                                 (0xFFFF0000)
#define SB2_CHIP_INFO_Revise_id(data)                                                (0xFFFF0000&((data)<<16))
#define SB2_CHIP_INFO_Revise_id_src(data)                                            ((0xFFFF0000&(data))>>16)
#define SB2_CHIP_INFO_get_Revise_id(data)                                            ((0xFFFF0000&(data))>>16)


#define SB2_PC_CTRL                                                                  0x1801A400
#define SB2_PC_CTRL_reg_addr                                                         "0xB801A400"
#define SB2_PC_CTRL_reg                                                              0xB801A400
#define set_SB2_PC_CTRL_reg(data)   (*((volatile unsigned int*) SB2_PC_CTRL_reg)=data)
#define get_SB2_PC_CTRL_reg   (*((volatile unsigned int*) SB2_PC_CTRL_reg))
#define SB2_PC_CTRL_inst_adr                                                         "0x0000"
#define SB2_PC_CTRL_inst                                                             0x0000
#define SB2_PC_CTRL_write_en8_shift                                                  (15)
#define SB2_PC_CTRL_write_en8_mask                                                   (0x00008000)
#define SB2_PC_CTRL_write_en8(data)                                                  (0x00008000&((data)<<15))
#define SB2_PC_CTRL_write_en8_src(data)                                              ((0x00008000&(data))>>15)
#define SB2_PC_CTRL_get_write_en8(data)                                              ((0x00008000&(data))>>15)
#define SB2_PC_CTRL_kcpu_rbus_go_shift                                               (14)
#define SB2_PC_CTRL_kcpu_rbus_go_mask                                                (0x00004000)
#define SB2_PC_CTRL_kcpu_rbus_go(data)                                               (0x00004000&((data)<<14))
#define SB2_PC_CTRL_kcpu_rbus_go_src(data)                                           ((0x00004000&(data))>>14)
#define SB2_PC_CTRL_get_kcpu_rbus_go(data)                                           ((0x00004000&(data))>>14)
#define SB2_PC_CTRL_write_en7_shift                                                  (13)
#define SB2_PC_CTRL_write_en7_mask                                                   (0x00002000)
#define SB2_PC_CTRL_write_en7(data)                                                  (0x00002000&((data)<<13))
#define SB2_PC_CTRL_write_en7_src(data)                                              ((0x00002000&(data))>>13)
#define SB2_PC_CTRL_get_write_en7(data)                                              ((0x00002000&(data))>>13)
#define SB2_PC_CTRL_kcpu_dbus_go_shift                                               (12)
#define SB2_PC_CTRL_kcpu_dbus_go_mask                                                (0x00001000)
#define SB2_PC_CTRL_kcpu_dbus_go(data)                                               (0x00001000&((data)<<12))
#define SB2_PC_CTRL_kcpu_dbus_go_src(data)                                           ((0x00001000&(data))>>12)
#define SB2_PC_CTRL_get_kcpu_dbus_go(data)                                           ((0x00001000&(data))>>12)
#define SB2_PC_CTRL_write_en6_shift                                                  (11)
#define SB2_PC_CTRL_write_en6_mask                                                   (0x00000800)
#define SB2_PC_CTRL_write_en6(data)                                                  (0x00000800&((data)<<11))
#define SB2_PC_CTRL_write_en6_src(data)                                              ((0x00000800&(data))>>11)
#define SB2_PC_CTRL_get_write_en6(data)                                              ((0x00000800&(data))>>11)
#define SB2_PC_CTRL_acpu_rbus_go_shift                                               (10)
#define SB2_PC_CTRL_acpu_rbus_go_mask                                                (0x00000400)
#define SB2_PC_CTRL_acpu_rbus_go(data)                                               (0x00000400&((data)<<10))
#define SB2_PC_CTRL_acpu_rbus_go_src(data)                                           ((0x00000400&(data))>>10)
#define SB2_PC_CTRL_get_acpu_rbus_go(data)                                           ((0x00000400&(data))>>10)
#define SB2_PC_CTRL_write_en5_shift                                                  (9)
#define SB2_PC_CTRL_write_en5_mask                                                   (0x00000200)
#define SB2_PC_CTRL_write_en5(data)                                                  (0x00000200&((data)<<9))
#define SB2_PC_CTRL_write_en5_src(data)                                              ((0x00000200&(data))>>9)
#define SB2_PC_CTRL_get_write_en5(data)                                              ((0x00000200&(data))>>9)
#define SB2_PC_CTRL_vcpu_rbus_go_shift                                               (8)
#define SB2_PC_CTRL_vcpu_rbus_go_mask                                                (0x00000100)
#define SB2_PC_CTRL_vcpu_rbus_go(data)                                               (0x00000100&((data)<<8))
#define SB2_PC_CTRL_vcpu_rbus_go_src(data)                                           ((0x00000100&(data))>>8)
#define SB2_PC_CTRL_get_vcpu_rbus_go(data)                                           ((0x00000100&(data))>>8)
#define SB2_PC_CTRL_write_en4_shift                                                  (7)
#define SB2_PC_CTRL_write_en4_mask                                                   (0x00000080)
#define SB2_PC_CTRL_write_en4(data)                                                  (0x00000080&((data)<<7))
#define SB2_PC_CTRL_write_en4_src(data)                                              ((0x00000080&(data))>>7)
#define SB2_PC_CTRL_get_write_en4(data)                                              ((0x00000080&(data))>>7)
#define SB2_PC_CTRL_scpu_rbus_go_shift                                               (6)
#define SB2_PC_CTRL_scpu_rbus_go_mask                                                (0x00000040)
#define SB2_PC_CTRL_scpu_rbus_go(data)                                               (0x00000040&((data)<<6))
#define SB2_PC_CTRL_scpu_rbus_go_src(data)                                           ((0x00000040&(data))>>6)
#define SB2_PC_CTRL_get_scpu_rbus_go(data)                                           ((0x00000040&(data))>>6)
#define SB2_PC_CTRL_write_en3_shift                                                  (5)
#define SB2_PC_CTRL_write_en3_mask                                                   (0x00000020)
#define SB2_PC_CTRL_write_en3(data)                                                  (0x00000020&((data)<<5))
#define SB2_PC_CTRL_write_en3_src(data)                                              ((0x00000020&(data))>>5)
#define SB2_PC_CTRL_get_write_en3(data)                                              ((0x00000020&(data))>>5)
#define SB2_PC_CTRL_acpu_dbus_go_shift                                               (4)
#define SB2_PC_CTRL_acpu_dbus_go_mask                                                (0x00000010)
#define SB2_PC_CTRL_acpu_dbus_go(data)                                               (0x00000010&((data)<<4))
#define SB2_PC_CTRL_acpu_dbus_go_src(data)                                           ((0x00000010&(data))>>4)
#define SB2_PC_CTRL_get_acpu_dbus_go(data)                                           ((0x00000010&(data))>>4)
#define SB2_PC_CTRL_write_en2_shift                                                  (3)
#define SB2_PC_CTRL_write_en2_mask                                                   (0x00000008)
#define SB2_PC_CTRL_write_en2(data)                                                  (0x00000008&((data)<<3))
#define SB2_PC_CTRL_write_en2_src(data)                                              ((0x00000008&(data))>>3)
#define SB2_PC_CTRL_get_write_en2(data)                                              ((0x00000008&(data))>>3)
#define SB2_PC_CTRL_vcpu_dbus_go_shift                                               (2)
#define SB2_PC_CTRL_vcpu_dbus_go_mask                                                (0x00000004)
#define SB2_PC_CTRL_vcpu_dbus_go(data)                                               (0x00000004&((data)<<2))
#define SB2_PC_CTRL_vcpu_dbus_go_src(data)                                           ((0x00000004&(data))>>2)
#define SB2_PC_CTRL_get_vcpu_dbus_go(data)                                           ((0x00000004&(data))>>2)
#define SB2_PC_CTRL_write_en1_shift                                                  (1)
#define SB2_PC_CTRL_write_en1_mask                                                   (0x00000002)
#define SB2_PC_CTRL_write_en1(data)                                                  (0x00000002&((data)<<1))
#define SB2_PC_CTRL_write_en1_src(data)                                              ((0x00000002&(data))>>1)
#define SB2_PC_CTRL_get_write_en1(data)                                              ((0x00000002&(data))>>1)
#define SB2_PC_CTRL_scpu_dbus_go_shift                                               (0)
#define SB2_PC_CTRL_scpu_dbus_go_mask                                                (0x00000001)
#define SB2_PC_CTRL_scpu_dbus_go(data)                                               (0x00000001&((data)<<0))
#define SB2_PC_CTRL_scpu_dbus_go_src(data)                                           ((0x00000001&(data))>>0)
#define SB2_PC_CTRL_get_scpu_dbus_go(data)                                           ((0x00000001&(data))>>0)


#define SB2_KCPU_DACC                                                                0x1801A3D0
#define SB2_KCPU_DACC_reg_addr                                                       "0xB801A3D0"
#define SB2_KCPU_DACC_reg                                                            0xB801A3D0
#define set_SB2_KCPU_DACC_reg(data)   (*((volatile unsigned int*) SB2_KCPU_DACC_reg)=data)
#define get_SB2_KCPU_DACC_reg   (*((volatile unsigned int*) SB2_KCPU_DACC_reg))
#define SB2_KCPU_DACC_inst_adr                                                       "0x00F4"
#define SB2_KCPU_DACC_inst                                                           0x00F4
#define SB2_KCPU_DACC_kcpu_dbus_acc_shift                                            (0)
#define SB2_KCPU_DACC_kcpu_dbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_KCPU_DACC_kcpu_dbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_KCPU_DACC_kcpu_dbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_KCPU_DACC_get_kcpu_dbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_KCPU_DREQ_NUM                                                            0x1801A3D4
#define SB2_KCPU_DREQ_NUM_reg_addr                                                   "0xB801A3D4"
#define SB2_KCPU_DREQ_NUM_reg                                                        0xB801A3D4
#define set_SB2_KCPU_DREQ_NUM_reg(data)   (*((volatile unsigned int*) SB2_KCPU_DREQ_NUM_reg)=data)
#define get_SB2_KCPU_DREQ_NUM_reg   (*((volatile unsigned int*) SB2_KCPU_DREQ_NUM_reg))
#define SB2_KCPU_DREQ_NUM_inst_adr                                                   "0x00F5"
#define SB2_KCPU_DREQ_NUM_inst                                                       0x00F5
#define SB2_KCPU_DREQ_NUM_kcpu_dreq_num_shift                                        (0)
#define SB2_KCPU_DREQ_NUM_kcpu_dreq_num_mask                                         (0x00FFFFFF)
#define SB2_KCPU_DREQ_NUM_kcpu_dreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_KCPU_DREQ_NUM_kcpu_dreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_KCPU_DREQ_NUM_get_kcpu_dreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_KCPU_DREQ_MAX                                                            0x1801A3D8
#define SB2_KCPU_DREQ_MAX_reg_addr                                                   "0xB801A3D8"
#define SB2_KCPU_DREQ_MAX_reg                                                        0xB801A3D8
#define set_SB2_KCPU_DREQ_MAX_reg(data)   (*((volatile unsigned int*) SB2_KCPU_DREQ_MAX_reg)=data)
#define get_SB2_KCPU_DREQ_MAX_reg   (*((volatile unsigned int*) SB2_KCPU_DREQ_MAX_reg))
#define SB2_KCPU_DREQ_MAX_inst_adr                                                   "0x00F6"
#define SB2_KCPU_DREQ_MAX_inst                                                       0x00F6
#define SB2_KCPU_DREQ_MAX_kcpu_dreq_max_shift                                        (0)
#define SB2_KCPU_DREQ_MAX_kcpu_dreq_max_mask                                         (0x0000FFFF)
#define SB2_KCPU_DREQ_MAX_kcpu_dreq_max(data)                                        (0x0000FFFF&((data)<<0))
#define SB2_KCPU_DREQ_MAX_kcpu_dreq_max_src(data)                                    ((0x0000FFFF&(data))>>0)
#define SB2_KCPU_DREQ_MAX_get_kcpu_dreq_max(data)                                    ((0x0000FFFF&(data))>>0)


#define SB2_KCPU_DREQ_ACK                                                            0x1801A3DC
#define SB2_KCPU_DREQ_ACK_reg_addr                                                   "0xB801A3DC"
#define SB2_KCPU_DREQ_ACK_reg                                                        0xB801A3DC
#define set_SB2_KCPU_DREQ_ACK_reg(data)   (*((volatile unsigned int*) SB2_KCPU_DREQ_ACK_reg)=data)
#define get_SB2_KCPU_DREQ_ACK_reg   (*((volatile unsigned int*) SB2_KCPU_DREQ_ACK_reg))
#define SB2_KCPU_DREQ_ACK_inst_adr                                                   "0x00F7"
#define SB2_KCPU_DREQ_ACK_inst                                                       0x00F7
#define SB2_KCPU_DREQ_ACK_kcpu_dreq_ack_shift                                        (0)
#define SB2_KCPU_DREQ_ACK_kcpu_dreq_ack_mask                                         (0x03FFFFFF)
#define SB2_KCPU_DREQ_ACK_kcpu_dreq_ack(data)                                        (0x03FFFFFF&((data)<<0))
#define SB2_KCPU_DREQ_ACK_kcpu_dreq_ack_src(data)                                    ((0x03FFFFFF&(data))>>0)
#define SB2_KCPU_DREQ_ACK_get_kcpu_dreq_ack(data)                                    ((0x03FFFFFF&(data))>>0)


#define SB2_KCPU_RACC                                                                0x1801A3E0
#define SB2_KCPU_RACC_reg_addr                                                       "0xB801A3E0"
#define SB2_KCPU_RACC_reg                                                            0xB801A3E0
#define set_SB2_KCPU_RACC_reg(data)   (*((volatile unsigned int*) SB2_KCPU_RACC_reg)=data)
#define get_SB2_KCPU_RACC_reg   (*((volatile unsigned int*) SB2_KCPU_RACC_reg))
#define SB2_KCPU_RACC_inst_adr                                                       "0x00F8"
#define SB2_KCPU_RACC_inst                                                           0x00F8
#define SB2_KCPU_RACC_kcpu_rbus_acc_shift                                            (0)
#define SB2_KCPU_RACC_kcpu_rbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_KCPU_RACC_kcpu_rbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_KCPU_RACC_kcpu_rbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_KCPU_RACC_get_kcpu_rbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_KCPU_RREQ_RLT                                                            0x1801A3E4
#define SB2_KCPU_RREQ_RLT_reg_addr                                                   "0xB801A3E4"
#define SB2_KCPU_RREQ_RLT_reg                                                        0xB801A3E4
#define set_SB2_KCPU_RREQ_RLT_reg(data)   (*((volatile unsigned int*) SB2_KCPU_RREQ_RLT_reg)=data)
#define get_SB2_KCPU_RREQ_RLT_reg   (*((volatile unsigned int*) SB2_KCPU_RREQ_RLT_reg))
#define SB2_KCPU_RREQ_RLT_inst_adr                                                   "0x00F9"
#define SB2_KCPU_RREQ_RLT_inst                                                       0x00F9
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_max_shift                                        (24)
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_max_mask                                         (0xFF000000)
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_max(data)                                        (0xFF000000&((data)<<24))
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_max_src(data)                                    ((0xFF000000&(data))>>24)
#define SB2_KCPU_RREQ_RLT_get_kcpu_rreq_max(data)                                    ((0xFF000000&(data))>>24)
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_num_shift                                        (0)
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_num_mask                                         (0x00FFFFFF)
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_KCPU_RREQ_RLT_kcpu_rreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_KCPU_RREQ_RLT_get_kcpu_rreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_SCPU_DACC                                                                0x1801A404
#define SB2_SCPU_DACC_reg_addr                                                       "0xB801A404"
#define SB2_SCPU_DACC_reg                                                            0xB801A404
#define set_SB2_SCPU_DACC_reg(data)   (*((volatile unsigned int*) SB2_SCPU_DACC_reg)=data)
#define get_SB2_SCPU_DACC_reg   (*((volatile unsigned int*) SB2_SCPU_DACC_reg))
#define SB2_SCPU_DACC_inst_adr                                                       "0x0001"
#define SB2_SCPU_DACC_inst                                                           0x0001
#define SB2_SCPU_DACC_scpu_dbus_acc_shift                                            (0)
#define SB2_SCPU_DACC_scpu_dbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_SCPU_DACC_scpu_dbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_SCPU_DACC_scpu_dbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_SCPU_DACC_get_scpu_dbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_SCPU_DREQ_NUM                                                            0x1801A408
#define SB2_SCPU_DREQ_NUM_reg_addr                                                   "0xB801A408"
#define SB2_SCPU_DREQ_NUM_reg                                                        0xB801A408
#define set_SB2_SCPU_DREQ_NUM_reg(data)   (*((volatile unsigned int*) SB2_SCPU_DREQ_NUM_reg)=data)
#define get_SB2_SCPU_DREQ_NUM_reg   (*((volatile unsigned int*) SB2_SCPU_DREQ_NUM_reg))
#define SB2_SCPU_DREQ_NUM_inst_adr                                                   "0x0002"
#define SB2_SCPU_DREQ_NUM_inst                                                       0x0002
#define SB2_SCPU_DREQ_NUM_scpu_dreq_num_shift                                        (0)
#define SB2_SCPU_DREQ_NUM_scpu_dreq_num_mask                                         (0x00FFFFFF)
#define SB2_SCPU_DREQ_NUM_scpu_dreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_SCPU_DREQ_NUM_scpu_dreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_SCPU_DREQ_NUM_get_scpu_dreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_SCPU_DREQ_MAX                                                            0x1801A40C
#define SB2_SCPU_DREQ_MAX_reg_addr                                                   "0xB801A40C"
#define SB2_SCPU_DREQ_MAX_reg                                                        0xB801A40C
#define set_SB2_SCPU_DREQ_MAX_reg(data)   (*((volatile unsigned int*) SB2_SCPU_DREQ_MAX_reg)=data)
#define get_SB2_SCPU_DREQ_MAX_reg   (*((volatile unsigned int*) SB2_SCPU_DREQ_MAX_reg))
#define SB2_SCPU_DREQ_MAX_inst_adr                                                   "0x0003"
#define SB2_SCPU_DREQ_MAX_inst                                                       0x0003
#define SB2_SCPU_DREQ_MAX_scpu_dreq_max_shift                                        (0)
#define SB2_SCPU_DREQ_MAX_scpu_dreq_max_mask                                         (0x0000FFFF)
#define SB2_SCPU_DREQ_MAX_scpu_dreq_max(data)                                        (0x0000FFFF&((data)<<0))
#define SB2_SCPU_DREQ_MAX_scpu_dreq_max_src(data)                                    ((0x0000FFFF&(data))>>0)
#define SB2_SCPU_DREQ_MAX_get_scpu_dreq_max(data)                                    ((0x0000FFFF&(data))>>0)


#define SB2_SCPU_DREQ_ACK                                                            0x1801A410
#define SB2_SCPU_DREQ_ACK_reg_addr                                                   "0xB801A410"
#define SB2_SCPU_DREQ_ACK_reg                                                        0xB801A410
#define set_SB2_SCPU_DREQ_ACK_reg(data)   (*((volatile unsigned int*) SB2_SCPU_DREQ_ACK_reg)=data)
#define get_SB2_SCPU_DREQ_ACK_reg   (*((volatile unsigned int*) SB2_SCPU_DREQ_ACK_reg))
#define SB2_SCPU_DREQ_ACK_inst_adr                                                   "0x0004"
#define SB2_SCPU_DREQ_ACK_inst                                                       0x0004
#define SB2_SCPU_DREQ_ACK_scpu_dreq_ack_shift                                        (0)
#define SB2_SCPU_DREQ_ACK_scpu_dreq_ack_mask                                         (0x03FFFFFF)
#define SB2_SCPU_DREQ_ACK_scpu_dreq_ack(data)                                        (0x03FFFFFF&((data)<<0))
#define SB2_SCPU_DREQ_ACK_scpu_dreq_ack_src(data)                                    ((0x03FFFFFF&(data))>>0)
#define SB2_SCPU_DREQ_ACK_get_scpu_dreq_ack(data)                                    ((0x03FFFFFF&(data))>>0)


#define SB2_VCPU_DACC                                                                0x1801A418
#define SB2_VCPU_DACC_reg_addr                                                       "0xB801A418"
#define SB2_VCPU_DACC_reg                                                            0xB801A418
#define set_SB2_VCPU_DACC_reg(data)   (*((volatile unsigned int*) SB2_VCPU_DACC_reg)=data)
#define get_SB2_VCPU_DACC_reg   (*((volatile unsigned int*) SB2_VCPU_DACC_reg))
#define SB2_VCPU_DACC_inst_adr                                                       "0x0006"
#define SB2_VCPU_DACC_inst                                                           0x0006
#define SB2_VCPU_DACC_vcpu_dbus_acc_shift                                            (0)
#define SB2_VCPU_DACC_vcpu_dbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_VCPU_DACC_vcpu_dbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_VCPU_DACC_vcpu_dbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_VCPU_DACC_get_vcpu_dbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_VCPU_DREQ_NUM                                                            0x1801A41C
#define SB2_VCPU_DREQ_NUM_reg_addr                                                   "0xB801A41C"
#define SB2_VCPU_DREQ_NUM_reg                                                        0xB801A41C
#define set_SB2_VCPU_DREQ_NUM_reg(data)   (*((volatile unsigned int*) SB2_VCPU_DREQ_NUM_reg)=data)
#define get_SB2_VCPU_DREQ_NUM_reg   (*((volatile unsigned int*) SB2_VCPU_DREQ_NUM_reg))
#define SB2_VCPU_DREQ_NUM_inst_adr                                                   "0x0007"
#define SB2_VCPU_DREQ_NUM_inst                                                       0x0007
#define SB2_VCPU_DREQ_NUM_vcpu_dreq_num_shift                                        (0)
#define SB2_VCPU_DREQ_NUM_vcpu_dreq_num_mask                                         (0x00FFFFFF)
#define SB2_VCPU_DREQ_NUM_vcpu_dreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_VCPU_DREQ_NUM_vcpu_dreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_VCPU_DREQ_NUM_get_vcpu_dreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_VCPU_DREQ_MAX                                                            0x1801A420
#define SB2_VCPU_DREQ_MAX_reg_addr                                                   "0xB801A420"
#define SB2_VCPU_DREQ_MAX_reg                                                        0xB801A420
#define set_SB2_VCPU_DREQ_MAX_reg(data)   (*((volatile unsigned int*) SB2_VCPU_DREQ_MAX_reg)=data)
#define get_SB2_VCPU_DREQ_MAX_reg   (*((volatile unsigned int*) SB2_VCPU_DREQ_MAX_reg))
#define SB2_VCPU_DREQ_MAX_inst_adr                                                   "0x0008"
#define SB2_VCPU_DREQ_MAX_inst                                                       0x0008
#define SB2_VCPU_DREQ_MAX_vcpu_dreq_max_shift                                        (0)
#define SB2_VCPU_DREQ_MAX_vcpu_dreq_max_mask                                         (0x0000FFFF)
#define SB2_VCPU_DREQ_MAX_vcpu_dreq_max(data)                                        (0x0000FFFF&((data)<<0))
#define SB2_VCPU_DREQ_MAX_vcpu_dreq_max_src(data)                                    ((0x0000FFFF&(data))>>0)
#define SB2_VCPU_DREQ_MAX_get_vcpu_dreq_max(data)                                    ((0x0000FFFF&(data))>>0)


#define SB2_VCPU_DREQ_ACK                                                            0x1801A424
#define SB2_VCPU_DREQ_ACK_reg_addr                                                   "0xB801A424"
#define SB2_VCPU_DREQ_ACK_reg                                                        0xB801A424
#define set_SB2_VCPU_DREQ_ACK_reg(data)   (*((volatile unsigned int*) SB2_VCPU_DREQ_ACK_reg)=data)
#define get_SB2_VCPU_DREQ_ACK_reg   (*((volatile unsigned int*) SB2_VCPU_DREQ_ACK_reg))
#define SB2_VCPU_DREQ_ACK_inst_adr                                                   "0x0009"
#define SB2_VCPU_DREQ_ACK_inst                                                       0x0009
#define SB2_VCPU_DREQ_ACK_vcpu_dreq_ack_shift                                        (0)
#define SB2_VCPU_DREQ_ACK_vcpu_dreq_ack_mask                                         (0x03FFFFFF)
#define SB2_VCPU_DREQ_ACK_vcpu_dreq_ack(data)                                        (0x03FFFFFF&((data)<<0))
#define SB2_VCPU_DREQ_ACK_vcpu_dreq_ack_src(data)                                    ((0x03FFFFFF&(data))>>0)
#define SB2_VCPU_DREQ_ACK_get_vcpu_dreq_ack(data)                                    ((0x03FFFFFF&(data))>>0)


#define SB2_ACPU_DACC                                                                0x1801A42C
#define SB2_ACPU_DACC_reg_addr                                                       "0xB801A42C"
#define SB2_ACPU_DACC_reg                                                            0xB801A42C
#define set_SB2_ACPU_DACC_reg(data)   (*((volatile unsigned int*) SB2_ACPU_DACC_reg)=data)
#define get_SB2_ACPU_DACC_reg   (*((volatile unsigned int*) SB2_ACPU_DACC_reg))
#define SB2_ACPU_DACC_inst_adr                                                       "0x000B"
#define SB2_ACPU_DACC_inst                                                           0x000B
#define SB2_ACPU_DACC_acpu_dbus_acc_shift                                            (0)
#define SB2_ACPU_DACC_acpu_dbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_ACPU_DACC_acpu_dbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_ACPU_DACC_acpu_dbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_ACPU_DACC_get_acpu_dbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_ACPU_DREQ_NUM                                                            0x1801A430
#define SB2_ACPU_DREQ_NUM_reg_addr                                                   "0xB801A430"
#define SB2_ACPU_DREQ_NUM_reg                                                        0xB801A430
#define set_SB2_ACPU_DREQ_NUM_reg(data)   (*((volatile unsigned int*) SB2_ACPU_DREQ_NUM_reg)=data)
#define get_SB2_ACPU_DREQ_NUM_reg   (*((volatile unsigned int*) SB2_ACPU_DREQ_NUM_reg))
#define SB2_ACPU_DREQ_NUM_inst_adr                                                   "0x000C"
#define SB2_ACPU_DREQ_NUM_inst                                                       0x000C
#define SB2_ACPU_DREQ_NUM_acpu_dreq_num_shift                                        (0)
#define SB2_ACPU_DREQ_NUM_acpu_dreq_num_mask                                         (0x00FFFFFF)
#define SB2_ACPU_DREQ_NUM_acpu_dreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_ACPU_DREQ_NUM_acpu_dreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_ACPU_DREQ_NUM_get_acpu_dreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_ACPU_DREQ_MAX                                                            0x1801A434
#define SB2_ACPU_DREQ_MAX_reg_addr                                                   "0xB801A434"
#define SB2_ACPU_DREQ_MAX_reg                                                        0xB801A434
#define set_SB2_ACPU_DREQ_MAX_reg(data)   (*((volatile unsigned int*) SB2_ACPU_DREQ_MAX_reg)=data)
#define get_SB2_ACPU_DREQ_MAX_reg   (*((volatile unsigned int*) SB2_ACPU_DREQ_MAX_reg))
#define SB2_ACPU_DREQ_MAX_inst_adr                                                   "0x000D"
#define SB2_ACPU_DREQ_MAX_inst                                                       0x000D
#define SB2_ACPU_DREQ_MAX_acpu_dreq_max_shift                                        (0)
#define SB2_ACPU_DREQ_MAX_acpu_dreq_max_mask                                         (0x0000FFFF)
#define SB2_ACPU_DREQ_MAX_acpu_dreq_max(data)                                        (0x0000FFFF&((data)<<0))
#define SB2_ACPU_DREQ_MAX_acpu_dreq_max_src(data)                                    ((0x0000FFFF&(data))>>0)
#define SB2_ACPU_DREQ_MAX_get_acpu_dreq_max(data)                                    ((0x0000FFFF&(data))>>0)


#define SB2_ACPU_DREQ_ACK                                                            0x1801A438
#define SB2_ACPU_DREQ_ACK_reg_addr                                                   "0xB801A438"
#define SB2_ACPU_DREQ_ACK_reg                                                        0xB801A438
#define set_SB2_ACPU_DREQ_ACK_reg(data)   (*((volatile unsigned int*) SB2_ACPU_DREQ_ACK_reg)=data)
#define get_SB2_ACPU_DREQ_ACK_reg   (*((volatile unsigned int*) SB2_ACPU_DREQ_ACK_reg))
#define SB2_ACPU_DREQ_ACK_inst_adr                                                   "0x000E"
#define SB2_ACPU_DREQ_ACK_inst                                                       0x000E
#define SB2_ACPU_DREQ_ACK_acpu_dreq_ack_shift                                        (0)
#define SB2_ACPU_DREQ_ACK_acpu_dreq_ack_mask                                         (0x03FFFFFF)
#define SB2_ACPU_DREQ_ACK_acpu_dreq_ack(data)                                        (0x03FFFFFF&((data)<<0))
#define SB2_ACPU_DREQ_ACK_acpu_dreq_ack_src(data)                                    ((0x03FFFFFF&(data))>>0)
#define SB2_ACPU_DREQ_ACK_get_acpu_dreq_ack(data)                                    ((0x03FFFFFF&(data))>>0)


#define SB2_SCPU_RACC                                                                0x1801A440
#define SB2_SCPU_RACC_reg_addr                                                       "0xB801A440"
#define SB2_SCPU_RACC_reg                                                            0xB801A440
#define set_SB2_SCPU_RACC_reg(data)   (*((volatile unsigned int*) SB2_SCPU_RACC_reg)=data)
#define get_SB2_SCPU_RACC_reg   (*((volatile unsigned int*) SB2_SCPU_RACC_reg))
#define SB2_SCPU_RACC_inst_adr                                                       "0x0010"
#define SB2_SCPU_RACC_inst                                                           0x0010
#define SB2_SCPU_RACC_scpu_rbus_acc_shift                                            (0)
#define SB2_SCPU_RACC_scpu_rbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_SCPU_RACC_scpu_rbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_SCPU_RACC_scpu_rbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_SCPU_RACC_get_scpu_rbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_SCPU_RREQ_RLT                                                            0x1801A444
#define SB2_SCPU_RREQ_RLT_reg_addr                                                   "0xB801A444"
#define SB2_SCPU_RREQ_RLT_reg                                                        0xB801A444
#define set_SB2_SCPU_RREQ_RLT_reg(data)   (*((volatile unsigned int*) SB2_SCPU_RREQ_RLT_reg)=data)
#define get_SB2_SCPU_RREQ_RLT_reg   (*((volatile unsigned int*) SB2_SCPU_RREQ_RLT_reg))
#define SB2_SCPU_RREQ_RLT_inst_adr                                                   "0x0011"
#define SB2_SCPU_RREQ_RLT_inst                                                       0x0011
#define SB2_SCPU_RREQ_RLT_scpu_rreq_max_shift                                        (24)
#define SB2_SCPU_RREQ_RLT_scpu_rreq_max_mask                                         (0xFF000000)
#define SB2_SCPU_RREQ_RLT_scpu_rreq_max(data)                                        (0xFF000000&((data)<<24))
#define SB2_SCPU_RREQ_RLT_scpu_rreq_max_src(data)                                    ((0xFF000000&(data))>>24)
#define SB2_SCPU_RREQ_RLT_get_scpu_rreq_max(data)                                    ((0xFF000000&(data))>>24)
#define SB2_SCPU_RREQ_RLT_scpu_rreq_num_shift                                        (0)
#define SB2_SCPU_RREQ_RLT_scpu_rreq_num_mask                                         (0x00FFFFFF)
#define SB2_SCPU_RREQ_RLT_scpu_rreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_SCPU_RREQ_RLT_scpu_rreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_SCPU_RREQ_RLT_get_scpu_rreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_VCPU_RACC                                                                0x1801A448
#define SB2_VCPU_RACC_reg_addr                                                       "0xB801A448"
#define SB2_VCPU_RACC_reg                                                            0xB801A448
#define set_SB2_VCPU_RACC_reg(data)   (*((volatile unsigned int*) SB2_VCPU_RACC_reg)=data)
#define get_SB2_VCPU_RACC_reg   (*((volatile unsigned int*) SB2_VCPU_RACC_reg))
#define SB2_VCPU_RACC_inst_adr                                                       "0x0012"
#define SB2_VCPU_RACC_inst                                                           0x0012
#define SB2_VCPU_RACC_vcpu_rbus_acc_shift                                            (0)
#define SB2_VCPU_RACC_vcpu_rbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_VCPU_RACC_vcpu_rbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_VCPU_RACC_vcpu_rbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_VCPU_RACC_get_vcpu_rbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_VCPU_RREQ_RLT                                                            0x1801A44C
#define SB2_VCPU_RREQ_RLT_reg_addr                                                   "0xB801A44C"
#define SB2_VCPU_RREQ_RLT_reg                                                        0xB801A44C
#define set_SB2_VCPU_RREQ_RLT_reg(data)   (*((volatile unsigned int*) SB2_VCPU_RREQ_RLT_reg)=data)
#define get_SB2_VCPU_RREQ_RLT_reg   (*((volatile unsigned int*) SB2_VCPU_RREQ_RLT_reg))
#define SB2_VCPU_RREQ_RLT_inst_adr                                                   "0x0013"
#define SB2_VCPU_RREQ_RLT_inst                                                       0x0013
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_max_shift                                        (24)
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_max_mask                                         (0xFF000000)
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_max(data)                                        (0xFF000000&((data)<<24))
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_max_src(data)                                    ((0xFF000000&(data))>>24)
#define SB2_VCPU_RREQ_RLT_get_vcpu_rreq_max(data)                                    ((0xFF000000&(data))>>24)
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_num_shift                                        (0)
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_num_mask                                         (0x00FFFFFF)
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_VCPU_RREQ_RLT_vcpu_rreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_VCPU_RREQ_RLT_get_vcpu_rreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_ACPU_RACC                                                                0x1801A450
#define SB2_ACPU_RACC_reg_addr                                                       "0xB801A450"
#define SB2_ACPU_RACC_reg                                                            0xB801A450
#define set_SB2_ACPU_RACC_reg(data)   (*((volatile unsigned int*) SB2_ACPU_RACC_reg)=data)
#define get_SB2_ACPU_RACC_reg   (*((volatile unsigned int*) SB2_ACPU_RACC_reg))
#define SB2_ACPU_RACC_inst_adr                                                       "0x0014"
#define SB2_ACPU_RACC_inst                                                           0x0014
#define SB2_ACPU_RACC_acpu_rbus_acc_shift                                            (0)
#define SB2_ACPU_RACC_acpu_rbus_acc_mask                                             (0xFFFFFFFF)
#define SB2_ACPU_RACC_acpu_rbus_acc(data)                                            (0xFFFFFFFF&((data)<<0))
#define SB2_ACPU_RACC_acpu_rbus_acc_src(data)                                        ((0xFFFFFFFF&(data))>>0)
#define SB2_ACPU_RACC_get_acpu_rbus_acc(data)                                        ((0xFFFFFFFF&(data))>>0)


#define SB2_ACPU_RREQ_RLT                                                            0x1801A454
#define SB2_ACPU_RREQ_RLT_reg_addr                                                   "0xB801A454"
#define SB2_ACPU_RREQ_RLT_reg                                                        0xB801A454
#define set_SB2_ACPU_RREQ_RLT_reg(data)   (*((volatile unsigned int*) SB2_ACPU_RREQ_RLT_reg)=data)
#define get_SB2_ACPU_RREQ_RLT_reg   (*((volatile unsigned int*) SB2_ACPU_RREQ_RLT_reg))
#define SB2_ACPU_RREQ_RLT_inst_adr                                                   "0x0015"
#define SB2_ACPU_RREQ_RLT_inst                                                       0x0015
#define SB2_ACPU_RREQ_RLT_acpu_rreq_max_shift                                        (24)
#define SB2_ACPU_RREQ_RLT_acpu_rreq_max_mask                                         (0xFF000000)
#define SB2_ACPU_RREQ_RLT_acpu_rreq_max(data)                                        (0xFF000000&((data)<<24))
#define SB2_ACPU_RREQ_RLT_acpu_rreq_max_src(data)                                    ((0xFF000000&(data))>>24)
#define SB2_ACPU_RREQ_RLT_get_acpu_rreq_max(data)                                    ((0xFF000000&(data))>>24)
#define SB2_ACPU_RREQ_RLT_acpu_rreq_num_shift                                        (0)
#define SB2_ACPU_RREQ_RLT_acpu_rreq_num_mask                                         (0x00FFFFFF)
#define SB2_ACPU_RREQ_RLT_acpu_rreq_num(data)                                        (0x00FFFFFF&((data)<<0))
#define SB2_ACPU_RREQ_RLT_acpu_rreq_num_src(data)                                    ((0x00FFFFFF&(data))>>0)
#define SB2_ACPU_RREQ_RLT_get_acpu_rreq_num(data)                                    ((0x00FFFFFF&(data))>>0)


#define SB2_DBG_START_0                                                              0x1801A458
#define SB2_DBG_START_1                                                              0x1801A45C
#define SB2_DBG_START_2                                                              0x1801A460
#define SB2_DBG_START_3                                                              0x1801A464
#define SB2_DBG_START_4                                                              0x1801A468
#define SB2_DBG_START_5                                                              0x1801A46C
#define SB2_DBG_START_6                                                              0x1801A470
#define SB2_DBG_START_7                                                              0x1801A474
#define SB2_DBG_START_0_reg_addr                                                     "0xB801A458"
#define SB2_DBG_START_1_reg_addr                                                     "0xB801A45C"
#define SB2_DBG_START_2_reg_addr                                                     "0xB801A460"
#define SB2_DBG_START_3_reg_addr                                                     "0xB801A464"
#define SB2_DBG_START_4_reg_addr                                                     "0xB801A468"
#define SB2_DBG_START_5_reg_addr                                                     "0xB801A46C"
#define SB2_DBG_START_6_reg_addr                                                     "0xB801A470"
#define SB2_DBG_START_7_reg_addr                                                     "0xB801A474"
#define SB2_DBG_START_0_reg                                                          0xB801A458
#define SB2_DBG_START_1_reg                                                          0xB801A45C
#define SB2_DBG_START_2_reg                                                          0xB801A460
#define SB2_DBG_START_3_reg                                                          0xB801A464
#define SB2_DBG_START_4_reg                                                          0xB801A468
#define SB2_DBG_START_5_reg                                                          0xB801A46C
#define SB2_DBG_START_6_reg                                                          0xB801A470
#define SB2_DBG_START_7_reg                                                          0xB801A474
#define set_SB2_DBG_START_0_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_0_reg)=data)
#define set_SB2_DBG_START_1_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_1_reg)=data)
#define set_SB2_DBG_START_2_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_2_reg)=data)
#define set_SB2_DBG_START_3_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_3_reg)=data)
#define set_SB2_DBG_START_4_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_4_reg)=data)
#define set_SB2_DBG_START_5_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_5_reg)=data)
#define set_SB2_DBG_START_6_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_6_reg)=data)
#define set_SB2_DBG_START_7_reg(data)   (*((volatile unsigned int*) SB2_DBG_START_7_reg)=data)
#define get_SB2_DBG_START_0_reg   (*((volatile unsigned int*) SB2_DBG_START_0_reg))
#define get_SB2_DBG_START_1_reg   (*((volatile unsigned int*) SB2_DBG_START_1_reg))
#define get_SB2_DBG_START_2_reg   (*((volatile unsigned int*) SB2_DBG_START_2_reg))
#define get_SB2_DBG_START_3_reg   (*((volatile unsigned int*) SB2_DBG_START_3_reg))
#define get_SB2_DBG_START_4_reg   (*((volatile unsigned int*) SB2_DBG_START_4_reg))
#define get_SB2_DBG_START_5_reg   (*((volatile unsigned int*) SB2_DBG_START_5_reg))
#define get_SB2_DBG_START_6_reg   (*((volatile unsigned int*) SB2_DBG_START_6_reg))
#define get_SB2_DBG_START_7_reg   (*((volatile unsigned int*) SB2_DBG_START_7_reg))
#define SB2_DBG_START_0_inst_adr                                                     "0x0016"
#define SB2_DBG_START_1_inst_adr                                                     "0x0017"
#define SB2_DBG_START_2_inst_adr                                                     "0x0018"
#define SB2_DBG_START_3_inst_adr                                                     "0x0019"
#define SB2_DBG_START_4_inst_adr                                                     "0x001A"
#define SB2_DBG_START_5_inst_adr                                                     "0x001B"
#define SB2_DBG_START_6_inst_adr                                                     "0x001C"
#define SB2_DBG_START_7_inst_adr                                                     "0x001D"
#define SB2_DBG_START_0_inst                                                         0x0016
#define SB2_DBG_START_1_inst                                                         0x0017
#define SB2_DBG_START_2_inst                                                         0x0018
#define SB2_DBG_START_3_inst                                                         0x0019
#define SB2_DBG_START_4_inst                                                         0x001A
#define SB2_DBG_START_5_inst                                                         0x001B
#define SB2_DBG_START_6_inst                                                         0x001C
#define SB2_DBG_START_7_inst                                                         0x001D
#define SB2_DBG_START_Dbg_start_addr_shift                                           (0)
#define SB2_DBG_START_Dbg_start_addr_mask                                            (0xFFFFFFFF)
#define SB2_DBG_START_Dbg_start_addr(data)                                           (0xFFFFFFFF&((data)<<0))
#define SB2_DBG_START_Dbg_start_addr_src(data)                                       ((0xFFFFFFFF&(data))>>0)
#define SB2_DBG_START_get_Dbg_start_addr(data)                                       ((0xFFFFFFFF&(data))>>0)


#define SB2_DBG_END_0                                                                0x1801A478
#define SB2_DBG_END_1                                                                0x1801A47C
#define SB2_DBG_END_2                                                                0x1801A480
#define SB2_DBG_END_3                                                                0x1801A484
#define SB2_DBG_END_4                                                                0x1801A488
#define SB2_DBG_END_5                                                                0x1801A48C
#define SB2_DBG_END_6                                                                0x1801A490
#define SB2_DBG_END_7                                                                0x1801A494
#define SB2_DBG_END_0_reg_addr                                                       "0xB801A478"
#define SB2_DBG_END_1_reg_addr                                                       "0xB801A47C"
#define SB2_DBG_END_2_reg_addr                                                       "0xB801A480"
#define SB2_DBG_END_3_reg_addr                                                       "0xB801A484"
#define SB2_DBG_END_4_reg_addr                                                       "0xB801A488"
#define SB2_DBG_END_5_reg_addr                                                       "0xB801A48C"
#define SB2_DBG_END_6_reg_addr                                                       "0xB801A490"
#define SB2_DBG_END_7_reg_addr                                                       "0xB801A494"
#define SB2_DBG_END_0_reg                                                            0xB801A478
#define SB2_DBG_END_1_reg                                                            0xB801A47C
#define SB2_DBG_END_2_reg                                                            0xB801A480
#define SB2_DBG_END_3_reg                                                            0xB801A484
#define SB2_DBG_END_4_reg                                                            0xB801A488
#define SB2_DBG_END_5_reg                                                            0xB801A48C
#define SB2_DBG_END_6_reg                                                            0xB801A490
#define SB2_DBG_END_7_reg                                                            0xB801A494
#define set_SB2_DBG_END_0_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_0_reg)=data)
#define set_SB2_DBG_END_1_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_1_reg)=data)
#define set_SB2_DBG_END_2_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_2_reg)=data)
#define set_SB2_DBG_END_3_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_3_reg)=data)
#define set_SB2_DBG_END_4_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_4_reg)=data)
#define set_SB2_DBG_END_5_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_5_reg)=data)
#define set_SB2_DBG_END_6_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_6_reg)=data)
#define set_SB2_DBG_END_7_reg(data)   (*((volatile unsigned int*) SB2_DBG_END_7_reg)=data)
#define get_SB2_DBG_END_0_reg   (*((volatile unsigned int*) SB2_DBG_END_0_reg))
#define get_SB2_DBG_END_1_reg   (*((volatile unsigned int*) SB2_DBG_END_1_reg))
#define get_SB2_DBG_END_2_reg   (*((volatile unsigned int*) SB2_DBG_END_2_reg))
#define get_SB2_DBG_END_3_reg   (*((volatile unsigned int*) SB2_DBG_END_3_reg))
#define get_SB2_DBG_END_4_reg   (*((volatile unsigned int*) SB2_DBG_END_4_reg))
#define get_SB2_DBG_END_5_reg   (*((volatile unsigned int*) SB2_DBG_END_5_reg))
#define get_SB2_DBG_END_6_reg   (*((volatile unsigned int*) SB2_DBG_END_6_reg))
#define get_SB2_DBG_END_7_reg   (*((volatile unsigned int*) SB2_DBG_END_7_reg))
#define SB2_DBG_END_0_inst_adr                                                       "0x001E"
#define SB2_DBG_END_1_inst_adr                                                       "0x001F"
#define SB2_DBG_END_2_inst_adr                                                       "0x0020"
#define SB2_DBG_END_3_inst_adr                                                       "0x0021"
#define SB2_DBG_END_4_inst_adr                                                       "0x0022"
#define SB2_DBG_END_5_inst_adr                                                       "0x0023"
#define SB2_DBG_END_6_inst_adr                                                       "0x0024"
#define SB2_DBG_END_7_inst_adr                                                       "0x0025"
#define SB2_DBG_END_0_inst                                                           0x001E
#define SB2_DBG_END_1_inst                                                           0x001F
#define SB2_DBG_END_2_inst                                                           0x0020
#define SB2_DBG_END_3_inst                                                           0x0021
#define SB2_DBG_END_4_inst                                                           0x0022
#define SB2_DBG_END_5_inst                                                           0x0023
#define SB2_DBG_END_6_inst                                                           0x0024
#define SB2_DBG_END_7_inst                                                           0x0025
#define SB2_DBG_END_Dbg_end_addr_shift                                               (0)
#define SB2_DBG_END_Dbg_end_addr_mask                                                (0xFFFFFFFF)
#define SB2_DBG_END_Dbg_end_addr(data)                                               (0xFFFFFFFF&((data)<<0))
#define SB2_DBG_END_Dbg_end_addr_src(data)                                           ((0xFFFFFFFF&(data))>>0)
#define SB2_DBG_END_get_Dbg_end_addr(data)                                           ((0xFFFFFFFF&(data))>>0)


#define SB2_DBG_CTRL_0                                                               0x1801A498
#define SB2_DBG_CTRL_1                                                               0x1801A49C
#define SB2_DBG_CTRL_2                                                               0x1801A4A0
#define SB2_DBG_CTRL_3                                                               0x1801A4A4
#define SB2_DBG_CTRL_4                                                               0x1801A4A8
#define SB2_DBG_CTRL_5                                                               0x1801A4AC
#define SB2_DBG_CTRL_6                                                               0x1801A4B0
#define SB2_DBG_CTRL_7                                                               0x1801A4B4
#define SB2_DBG_CTRL_0_reg_addr                                                      "0xB801A498"
#define SB2_DBG_CTRL_1_reg_addr                                                      "0xB801A49C"
#define SB2_DBG_CTRL_2_reg_addr                                                      "0xB801A4A0"
#define SB2_DBG_CTRL_3_reg_addr                                                      "0xB801A4A4"
#define SB2_DBG_CTRL_4_reg_addr                                                      "0xB801A4A8"
#define SB2_DBG_CTRL_5_reg_addr                                                      "0xB801A4AC"
#define SB2_DBG_CTRL_6_reg_addr                                                      "0xB801A4B0"
#define SB2_DBG_CTRL_7_reg_addr                                                      "0xB801A4B4"
#define SB2_DBG_CTRL_0_reg                                                           0xB801A498
#define SB2_DBG_CTRL_1_reg                                                           0xB801A49C
#define SB2_DBG_CTRL_2_reg                                                           0xB801A4A0
#define SB2_DBG_CTRL_3_reg                                                           0xB801A4A4
#define SB2_DBG_CTRL_4_reg                                                           0xB801A4A8
#define SB2_DBG_CTRL_5_reg                                                           0xB801A4AC
#define SB2_DBG_CTRL_6_reg                                                           0xB801A4B0
#define SB2_DBG_CTRL_7_reg                                                           0xB801A4B4
#define set_SB2_DBG_CTRL_0_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_0_reg)=data)
#define set_SB2_DBG_CTRL_1_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_1_reg)=data)
#define set_SB2_DBG_CTRL_2_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_2_reg)=data)
#define set_SB2_DBG_CTRL_3_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_3_reg)=data)
#define set_SB2_DBG_CTRL_4_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_4_reg)=data)
#define set_SB2_DBG_CTRL_5_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_5_reg)=data)
#define set_SB2_DBG_CTRL_6_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_6_reg)=data)
#define set_SB2_DBG_CTRL_7_reg(data)   (*((volatile unsigned int*) SB2_DBG_CTRL_7_reg)=data)
#define get_SB2_DBG_CTRL_0_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_0_reg))
#define get_SB2_DBG_CTRL_1_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_1_reg))
#define get_SB2_DBG_CTRL_2_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_2_reg))
#define get_SB2_DBG_CTRL_3_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_3_reg))
#define get_SB2_DBG_CTRL_4_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_4_reg))
#define get_SB2_DBG_CTRL_5_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_5_reg))
#define get_SB2_DBG_CTRL_6_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_6_reg))
#define get_SB2_DBG_CTRL_7_reg   (*((volatile unsigned int*) SB2_DBG_CTRL_7_reg))
#define SB2_DBG_CTRL_0_inst_adr                                                      "0x0026"
#define SB2_DBG_CTRL_1_inst_adr                                                      "0x0027"
#define SB2_DBG_CTRL_2_inst_adr                                                      "0x0028"
#define SB2_DBG_CTRL_3_inst_adr                                                      "0x0029"
#define SB2_DBG_CTRL_4_inst_adr                                                      "0x002A"
#define SB2_DBG_CTRL_5_inst_adr                                                      "0x002B"
#define SB2_DBG_CTRL_6_inst_adr                                                      "0x002C"
#define SB2_DBG_CTRL_7_inst_adr                                                      "0x002D"
#define SB2_DBG_CTRL_0_inst                                                          0x0026
#define SB2_DBG_CTRL_1_inst                                                          0x0027
#define SB2_DBG_CTRL_2_inst                                                          0x0028
#define SB2_DBG_CTRL_3_inst                                                          0x0029
#define SB2_DBG_CTRL_4_inst                                                          0x002A
#define SB2_DBG_CTRL_5_inst                                                          0x002B
#define SB2_DBG_CTRL_6_inst                                                          0x002C
#define SB2_DBG_CTRL_7_inst                                                          0x002D
#define SB2_DBG_CTRL_write_en7_shift                                                 (15)
#define SB2_DBG_CTRL_write_en7_mask                                                  (0x00008000)
#define SB2_DBG_CTRL_write_en7(data)                                                 (0x00008000&((data)<<15))
#define SB2_DBG_CTRL_write_en7_src(data)                                             ((0x00008000&(data))>>15)
#define SB2_DBG_CTRL_get_write_en7(data)                                             ((0x00008000&(data))>>15)
#define SB2_DBG_CTRL_Dbg_kcpu_chk_en_shift                                           (14)
#define SB2_DBG_CTRL_Dbg_kcpu_chk_en_mask                                            (0x00004000)
#define SB2_DBG_CTRL_Dbg_kcpu_chk_en(data)                                           (0x00004000&((data)<<14))
#define SB2_DBG_CTRL_Dbg_kcpu_chk_en_src(data)                                       ((0x00004000&(data))>>14)
#define SB2_DBG_CTRL_get_Dbg_kcpu_chk_en(data)                                       ((0x00004000&(data))>>14)
#define SB2_DBG_CTRL_write_en6_shift                                                 (13)
#define SB2_DBG_CTRL_write_en6_mask                                                  (0x00002000)
#define SB2_DBG_CTRL_write_en6(data)                                                 (0x00002000&((data)<<13))
#define SB2_DBG_CTRL_write_en6_src(data)                                             ((0x00002000&(data))>>13)
#define SB2_DBG_CTRL_get_write_en6(data)                                             ((0x00002000&(data))>>13)
#define SB2_DBG_CTRL_Dbg_acpu_chk_en_shift                                           (12)
#define SB2_DBG_CTRL_Dbg_acpu_chk_en_mask                                            (0x00001000)
#define SB2_DBG_CTRL_Dbg_acpu_chk_en(data)                                           (0x00001000&((data)<<12))
#define SB2_DBG_CTRL_Dbg_acpu_chk_en_src(data)                                       ((0x00001000&(data))>>12)
#define SB2_DBG_CTRL_get_Dbg_acpu_chk_en(data)                                       ((0x00001000&(data))>>12)
#define SB2_DBG_CTRL_write_en5_shift                                                 (11)
#define SB2_DBG_CTRL_write_en5_mask                                                  (0x00000800)
#define SB2_DBG_CTRL_write_en5(data)                                                 (0x00000800&((data)<<11))
#define SB2_DBG_CTRL_write_en5_src(data)                                             ((0x00000800&(data))>>11)
#define SB2_DBG_CTRL_get_write_en5(data)                                             ((0x00000800&(data))>>11)
#define SB2_DBG_CTRL_Dbg_vcpu_chk_en_shift                                           (10)
#define SB2_DBG_CTRL_Dbg_vcpu_chk_en_mask                                            (0x00000400)
#define SB2_DBG_CTRL_Dbg_vcpu_chk_en(data)                                           (0x00000400&((data)<<10))
#define SB2_DBG_CTRL_Dbg_vcpu_chk_en_src(data)                                       ((0x00000400&(data))>>10)
#define SB2_DBG_CTRL_get_Dbg_vcpu_chk_en(data)                                       ((0x00000400&(data))>>10)
#define SB2_DBG_CTRL_write_en4_shift                                                 (9)
#define SB2_DBG_CTRL_write_en4_mask                                                  (0x00000200)
#define SB2_DBG_CTRL_write_en4(data)                                                 (0x00000200&((data)<<9))
#define SB2_DBG_CTRL_write_en4_src(data)                                             ((0x00000200&(data))>>9)
#define SB2_DBG_CTRL_get_write_en4(data)                                             ((0x00000200&(data))>>9)
#define SB2_DBG_CTRL_Dbg_scpu_chk_en_shift                                           (8)
#define SB2_DBG_CTRL_Dbg_scpu_chk_en_mask                                            (0x00000100)
#define SB2_DBG_CTRL_Dbg_scpu_chk_en(data)                                           (0x00000100&((data)<<8))
#define SB2_DBG_CTRL_Dbg_scpu_chk_en_src(data)                                       ((0x00000100&(data))>>8)
#define SB2_DBG_CTRL_get_Dbg_scpu_chk_en(data)                                       ((0x00000100&(data))>>8)
#define SB2_DBG_CTRL_write_en3_shift                                                 (7)
#define SB2_DBG_CTRL_write_en3_mask                                                  (0x00000080)
#define SB2_DBG_CTRL_write_en3(data)                                                 (0x00000080&((data)<<7))
#define SB2_DBG_CTRL_write_en3_src(data)                                             ((0x00000080&(data))>>7)
#define SB2_DBG_CTRL_get_write_en3(data)                                             ((0x00000080&(data))>>7)
#define SB2_DBG_CTRL_Dbg_wr_chk_shift                                                (5)
#define SB2_DBG_CTRL_Dbg_wr_chk_mask                                                 (0x00000060)
#define SB2_DBG_CTRL_Dbg_wr_chk(data)                                                (0x00000060&((data)<<5))
#define SB2_DBG_CTRL_Dbg_wr_chk_src(data)                                            ((0x00000060&(data))>>5)
#define SB2_DBG_CTRL_get_Dbg_wr_chk(data)                                            ((0x00000060&(data))>>5)
#define SB2_DBG_CTRL_write_en2_shift                                                 (4)
#define SB2_DBG_CTRL_write_en2_mask                                                  (0x00000010)
#define SB2_DBG_CTRL_write_en2(data)                                                 (0x00000010&((data)<<4))
#define SB2_DBG_CTRL_write_en2_src(data)                                             ((0x00000010&(data))>>4)
#define SB2_DBG_CTRL_get_write_en2(data)                                             ((0x00000010&(data))>>4)
#define SB2_DBG_CTRL_Dbg_id_chk_shift                                                (2)
#define SB2_DBG_CTRL_Dbg_id_chk_mask                                                 (0x0000000C)
#define SB2_DBG_CTRL_Dbg_id_chk(data)                                                (0x0000000C&((data)<<2))
#define SB2_DBG_CTRL_Dbg_id_chk_src(data)                                            ((0x0000000C&(data))>>2)
#define SB2_DBG_CTRL_get_Dbg_id_chk(data)                                            ((0x0000000C&(data))>>2)
#define SB2_DBG_CTRL_write_en1_shift                                                 (1)
#define SB2_DBG_CTRL_write_en1_mask                                                  (0x00000002)
#define SB2_DBG_CTRL_write_en1(data)                                                 (0x00000002&((data)<<1))
#define SB2_DBG_CTRL_write_en1_src(data)                                             ((0x00000002&(data))>>1)
#define SB2_DBG_CTRL_get_write_en1(data)                                             ((0x00000002&(data))>>1)
#define SB2_DBG_CTRL_Dbg_en_shift                                                    (0)
#define SB2_DBG_CTRL_Dbg_en_mask                                                     (0x00000001)
#define SB2_DBG_CTRL_Dbg_en(data)                                                    (0x00000001&((data)<<0))
#define SB2_DBG_CTRL_Dbg_en_src(data)                                                ((0x00000001&(data))>>0)
#define SB2_DBG_CTRL_get_Dbg_en(data)                                                ((0x00000001&(data))>>0)


#define SB2_DBG_ADDR_0                                                               0x1801A4B8
#define SB2_DBG_ADDR_1                                                               0x1801A4BC
#define SB2_DBG_ADDR_2                                                               0x1801A4C0
#define SB2_DBG_ADDR_3                                                               0x1801A4C4
#define SB2_DBG_ADDR_0_reg_addr                                                      "0xB801A4B8"
#define SB2_DBG_ADDR_1_reg_addr                                                      "0xB801A4BC"
#define SB2_DBG_ADDR_2_reg_addr                                                      "0xB801A4C0"
#define SB2_DBG_ADDR_3_reg_addr                                                      "0xB801A4C4"
#define SB2_DBG_ADDR_0_reg                                                           0xB801A4B8
#define SB2_DBG_ADDR_1_reg                                                           0xB801A4BC
#define SB2_DBG_ADDR_2_reg                                                           0xB801A4C0
#define SB2_DBG_ADDR_3_reg                                                           0xB801A4C4
#define set_SB2_DBG_ADDR_0_reg(data)   (*((volatile unsigned int*) SB2_DBG_ADDR_0_reg)=data)
#define set_SB2_DBG_ADDR_1_reg(data)   (*((volatile unsigned int*) SB2_DBG_ADDR_1_reg)=data)
#define set_SB2_DBG_ADDR_2_reg(data)   (*((volatile unsigned int*) SB2_DBG_ADDR_2_reg)=data)
#define set_SB2_DBG_ADDR_3_reg(data)   (*((volatile unsigned int*) SB2_DBG_ADDR_3_reg)=data)
#define get_SB2_DBG_ADDR_0_reg   (*((volatile unsigned int*) SB2_DBG_ADDR_0_reg))
#define get_SB2_DBG_ADDR_1_reg   (*((volatile unsigned int*) SB2_DBG_ADDR_1_reg))
#define get_SB2_DBG_ADDR_2_reg   (*((volatile unsigned int*) SB2_DBG_ADDR_2_reg))
#define get_SB2_DBG_ADDR_3_reg   (*((volatile unsigned int*) SB2_DBG_ADDR_3_reg))
#define SB2_DBG_ADDR_0_inst_adr                                                      "0x002E"
#define SB2_DBG_ADDR_1_inst_adr                                                      "0x002F"
#define SB2_DBG_ADDR_2_inst_adr                                                      "0x0030"
#define SB2_DBG_ADDR_3_inst_adr                                                      "0x0031"
#define SB2_DBG_ADDR_0_inst                                                          0x002E
#define SB2_DBG_ADDR_1_inst                                                          0x002F
#define SB2_DBG_ADDR_2_inst                                                          0x0030
#define SB2_DBG_ADDR_3_inst                                                          0x0031
#define SB2_DBG_ADDR_dbg_addr_shift                                                  (0)
#define SB2_DBG_ADDR_dbg_addr_mask                                                   (0xFFFFFFFF)
#define SB2_DBG_ADDR_dbg_addr(data)                                                  (0xFFFFFFFF&((data)<<0))
#define SB2_DBG_ADDR_dbg_addr_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define SB2_DBG_ADDR_get_dbg_addr(data)                                              ((0xFFFFFFFF&(data))>>0)


#define SB2_DBG_ADDR1                                                                0x1801A4C8
#define SB2_DBG_ADDR1_reg_addr                                                       "0xB801A4C8"
#define SB2_DBG_ADDR1_reg                                                            0xB801A4C8
#define set_SB2_DBG_ADDR1_reg(data)   (*((volatile unsigned int*) SB2_DBG_ADDR1_reg)=data)
#define get_SB2_DBG_ADDR1_reg   (*((volatile unsigned int*) SB2_DBG_ADDR1_reg))
#define SB2_DBG_ADDR1_inst_adr                                                       "0x0032"
#define SB2_DBG_ADDR1_inst                                                           0x0032
#define SB2_DBG_ADDR1_vcpu_dbg_write_shift                                           (7)
#define SB2_DBG_ADDR1_vcpu_dbg_write_mask                                            (0x00000080)
#define SB2_DBG_ADDR1_vcpu_dbg_write(data)                                           (0x00000080&((data)<<7))
#define SB2_DBG_ADDR1_vcpu_dbg_write_src(data)                                       ((0x00000080&(data))>>7)
#define SB2_DBG_ADDR1_get_vcpu_dbg_write(data)                                       ((0x00000080&(data))>>7)
#define SB2_DBG_ADDR1_vcpu_dbg_dacc_shift                                            (6)
#define SB2_DBG_ADDR1_vcpu_dbg_dacc_mask                                             (0x00000040)
#define SB2_DBG_ADDR1_vcpu_dbg_dacc(data)                                            (0x00000040&((data)<<6))
#define SB2_DBG_ADDR1_vcpu_dbg_dacc_src(data)                                        ((0x00000040&(data))>>6)
#define SB2_DBG_ADDR1_get_vcpu_dbg_dacc(data)                                        ((0x00000040&(data))>>6)
#define SB2_DBG_ADDR1_acpu_dbg_write_shift                                           (5)
#define SB2_DBG_ADDR1_acpu_dbg_write_mask                                            (0x00000020)
#define SB2_DBG_ADDR1_acpu_dbg_write(data)                                           (0x00000020&((data)<<5))
#define SB2_DBG_ADDR1_acpu_dbg_write_src(data)                                       ((0x00000020&(data))>>5)
#define SB2_DBG_ADDR1_get_acpu_dbg_write(data)                                       ((0x00000020&(data))>>5)
#define SB2_DBG_ADDR1_acpu_dbg_dacc_shift                                            (4)
#define SB2_DBG_ADDR1_acpu_dbg_dacc_mask                                             (0x00000010)
#define SB2_DBG_ADDR1_acpu_dbg_dacc(data)                                            (0x00000010&((data)<<4))
#define SB2_DBG_ADDR1_acpu_dbg_dacc_src(data)                                        ((0x00000010&(data))>>4)
#define SB2_DBG_ADDR1_get_acpu_dbg_dacc(data)                                        ((0x00000010&(data))>>4)
#define SB2_DBG_ADDR1_scpu_dbg_write_shift                                           (3)
#define SB2_DBG_ADDR1_scpu_dbg_write_mask                                            (0x00000008)
#define SB2_DBG_ADDR1_scpu_dbg_write(data)                                           (0x00000008&((data)<<3))
#define SB2_DBG_ADDR1_scpu_dbg_write_src(data)                                       ((0x00000008&(data))>>3)
#define SB2_DBG_ADDR1_get_scpu_dbg_write(data)                                       ((0x00000008&(data))>>3)
#define SB2_DBG_ADDR1_scpu_dbg_dacc_shift                                            (2)
#define SB2_DBG_ADDR1_scpu_dbg_dacc_mask                                             (0x00000004)
#define SB2_DBG_ADDR1_scpu_dbg_dacc(data)                                            (0x00000004&((data)<<2))
#define SB2_DBG_ADDR1_scpu_dbg_dacc_src(data)                                        ((0x00000004&(data))>>2)
#define SB2_DBG_ADDR1_get_scpu_dbg_dacc(data)                                        ((0x00000004&(data))>>2)
#define SB2_DBG_ADDR1_kcpu_dbg_write_shift                                           (1)
#define SB2_DBG_ADDR1_kcpu_dbg_write_mask                                            (0x00000002)
#define SB2_DBG_ADDR1_kcpu_dbg_write(data)                                           (0x00000002&((data)<<1))
#define SB2_DBG_ADDR1_kcpu_dbg_write_src(data)                                       ((0x00000002&(data))>>1)
#define SB2_DBG_ADDR1_get_kcpu_dbg_write(data)                                       ((0x00000002&(data))>>1)
#define SB2_DBG_ADDR1_kcpu_dbg_dacc_shift                                            (0)
#define SB2_DBG_ADDR1_kcpu_dbg_dacc_mask                                             (0x00000001)
#define SB2_DBG_ADDR1_kcpu_dbg_dacc(data)                                            (0x00000001&((data)<<0))
#define SB2_DBG_ADDR1_kcpu_dbg_dacc_src(data)                                        ((0x00000001&(data))>>0)
#define SB2_DBG_ADDR1_get_kcpu_dbg_dacc(data)                                        ((0x00000001&(data))>>0)


#define SB2_DBG_INT_KCPU                                                             0x1801A4CC
#define SB2_DBG_INT_KCPU_reg_addr                                                    "0xB801A4CC"
#define SB2_DBG_INT_KCPU_reg                                                         0xB801A4CC
#define set_SB2_DBG_INT_KCPU_reg(data)   (*((volatile unsigned int*) SB2_DBG_INT_KCPU_reg)=data)
#define get_SB2_DBG_INT_KCPU_reg   (*((volatile unsigned int*) SB2_DBG_INT_KCPU_reg))
#define SB2_DBG_INT_KCPU_inst_adr                                                    "0x0033"
#define SB2_DBG_INT_KCPU_inst                                                        0x0033
#define SB2_DBG_INT_KCPU_kcpu_int_shift                                              (10)
#define SB2_DBG_INT_KCPU_kcpu_int_mask                                               (0x00000400)
#define SB2_DBG_INT_KCPU_kcpu_int(data)                                              (0x00000400&((data)<<10))
#define SB2_DBG_INT_KCPU_kcpu_int_src(data)                                          ((0x00000400&(data))>>10)
#define SB2_DBG_INT_KCPU_get_kcpu_int(data)                                          ((0x00000400&(data))>>10)
#define SB2_DBG_INT_KCPU_kcpu_int_en_shift                                           (7)
#define SB2_DBG_INT_KCPU_kcpu_int_en_mask                                            (0x00000080)
#define SB2_DBG_INT_KCPU_kcpu_int_en(data)                                           (0x00000080&((data)<<7))
#define SB2_DBG_INT_KCPU_kcpu_int_en_src(data)                                       ((0x00000080&(data))>>7)
#define SB2_DBG_INT_KCPU_get_kcpu_int_en(data)                                       ((0x00000080&(data))>>7)
#define SB2_DBG_INT_KCPU_kcpu_neg_int_shift                                          (4)
#define SB2_DBG_INT_KCPU_kcpu_neg_int_mask                                           (0x00000010)
#define SB2_DBG_INT_KCPU_kcpu_neg_int(data)                                          (0x00000010&((data)<<4))
#define SB2_DBG_INT_KCPU_kcpu_neg_int_src(data)                                      ((0x00000010&(data))>>4)
#define SB2_DBG_INT_KCPU_get_kcpu_neg_int(data)                                      ((0x00000010&(data))>>4)
#define SB2_DBG_INT_KCPU_kcpu_neg_int_en_shift                                       (1)
#define SB2_DBG_INT_KCPU_kcpu_neg_int_en_mask                                        (0x00000002)
#define SB2_DBG_INT_KCPU_kcpu_neg_int_en(data)                                       (0x00000002&((data)<<1))
#define SB2_DBG_INT_KCPU_kcpu_neg_int_en_src(data)                                   ((0x00000002&(data))>>1)
#define SB2_DBG_INT_KCPU_get_kcpu_neg_int_en(data)                                   ((0x00000002&(data))>>1)
#define SB2_DBG_INT_KCPU_write_data_shift                                            (0)
#define SB2_DBG_INT_KCPU_write_data_mask                                             (0x00000001)
#define SB2_DBG_INT_KCPU_write_data(data)                                            (0x00000001&((data)<<0))
#define SB2_DBG_INT_KCPU_write_data_src(data)                                        ((0x00000001&(data))>>0)
#define SB2_DBG_INT_KCPU_get_write_data(data)                                        ((0x00000001&(data))>>0)


#define SB2_DBG_INT                                                                  0x1801A4E0
#define SB2_DBG_INT_reg_addr                                                         "0xB801A4E0"
#define SB2_DBG_INT_reg                                                              0xB801A4E0
#define set_SB2_DBG_INT_reg(data)   (*((volatile unsigned int*) SB2_DBG_INT_reg)=data)
#define get_SB2_DBG_INT_reg   (*((volatile unsigned int*) SB2_DBG_INT_reg))
#define SB2_DBG_INT_inst_adr                                                         "0x0038"
#define SB2_DBG_INT_inst                                                             0x0038
#define SB2_DBG_INT_acpu_int_shift                                                   (12)
#define SB2_DBG_INT_acpu_int_mask                                                    (0x00001000)
#define SB2_DBG_INT_acpu_int(data)                                                   (0x00001000&((data)<<12))
#define SB2_DBG_INT_acpu_int_src(data)                                               ((0x00001000&(data))>>12)
#define SB2_DBG_INT_get_acpu_int(data)                                               ((0x00001000&(data))>>12)
#define SB2_DBG_INT_vcpu_int_shift                                                   (11)
#define SB2_DBG_INT_vcpu_int_mask                                                    (0x00000800)
#define SB2_DBG_INT_vcpu_int(data)                                                   (0x00000800&((data)<<11))
#define SB2_DBG_INT_vcpu_int_src(data)                                               ((0x00000800&(data))>>11)
#define SB2_DBG_INT_get_vcpu_int(data)                                               ((0x00000800&(data))>>11)
#define SB2_DBG_INT_scpu_int_shift                                                   (10)
#define SB2_DBG_INT_scpu_int_mask                                                    (0x00000400)
#define SB2_DBG_INT_scpu_int(data)                                                   (0x00000400&((data)<<10))
#define SB2_DBG_INT_scpu_int_src(data)                                               ((0x00000400&(data))>>10)
#define SB2_DBG_INT_get_scpu_int(data)                                               ((0x00000400&(data))>>10)
#define SB2_DBG_INT_acpu_int_en_shift                                                (9)
#define SB2_DBG_INT_acpu_int_en_mask                                                 (0x00000200)
#define SB2_DBG_INT_acpu_int_en(data)                                                (0x00000200&((data)<<9))
#define SB2_DBG_INT_acpu_int_en_src(data)                                            ((0x00000200&(data))>>9)
#define SB2_DBG_INT_get_acpu_int_en(data)                                            ((0x00000200&(data))>>9)
#define SB2_DBG_INT_vcpu_int_en_shift                                                (8)
#define SB2_DBG_INT_vcpu_int_en_mask                                                 (0x00000100)
#define SB2_DBG_INT_vcpu_int_en(data)                                                (0x00000100&((data)<<8))
#define SB2_DBG_INT_vcpu_int_en_src(data)                                            ((0x00000100&(data))>>8)
#define SB2_DBG_INT_get_vcpu_int_en(data)                                            ((0x00000100&(data))>>8)
#define SB2_DBG_INT_scpu_int_en_shift                                                (7)
#define SB2_DBG_INT_scpu_int_en_mask                                                 (0x00000080)
#define SB2_DBG_INT_scpu_int_en(data)                                                (0x00000080&((data)<<7))
#define SB2_DBG_INT_scpu_int_en_src(data)                                            ((0x00000080&(data))>>7)
#define SB2_DBG_INT_get_scpu_int_en(data)                                            ((0x00000080&(data))>>7)
#define SB2_DBG_INT_acpu_neg_int_shift                                               (6)
#define SB2_DBG_INT_acpu_neg_int_mask                                                (0x00000040)
#define SB2_DBG_INT_acpu_neg_int(data)                                               (0x00000040&((data)<<6))
#define SB2_DBG_INT_acpu_neg_int_src(data)                                           ((0x00000040&(data))>>6)
#define SB2_DBG_INT_get_acpu_neg_int(data)                                           ((0x00000040&(data))>>6)
#define SB2_DBG_INT_vcpu_neg_int_shift                                               (5)
#define SB2_DBG_INT_vcpu_neg_int_mask                                                (0x00000020)
#define SB2_DBG_INT_vcpu_neg_int(data)                                               (0x00000020&((data)<<5))
#define SB2_DBG_INT_vcpu_neg_int_src(data)                                           ((0x00000020&(data))>>5)
#define SB2_DBG_INT_get_vcpu_neg_int(data)                                           ((0x00000020&(data))>>5)
#define SB2_DBG_INT_scpu_neg_int_shift                                               (4)
#define SB2_DBG_INT_scpu_neg_int_mask                                                (0x00000010)
#define SB2_DBG_INT_scpu_neg_int(data)                                               (0x00000010&((data)<<4))
#define SB2_DBG_INT_scpu_neg_int_src(data)                                           ((0x00000010&(data))>>4)
#define SB2_DBG_INT_get_scpu_neg_int(data)                                           ((0x00000010&(data))>>4)
#define SB2_DBG_INT_acpu_neg_int_en_shift                                            (3)
#define SB2_DBG_INT_acpu_neg_int_en_mask                                             (0x00000008)
#define SB2_DBG_INT_acpu_neg_int_en(data)                                            (0x00000008&((data)<<3))
#define SB2_DBG_INT_acpu_neg_int_en_src(data)                                        ((0x00000008&(data))>>3)
#define SB2_DBG_INT_get_acpu_neg_int_en(data)                                        ((0x00000008&(data))>>3)
#define SB2_DBG_INT_vcpu_neg_int_en_shift                                            (2)
#define SB2_DBG_INT_vcpu_neg_int_en_mask                                             (0x00000004)
#define SB2_DBG_INT_vcpu_neg_int_en(data)                                            (0x00000004&((data)<<2))
#define SB2_DBG_INT_vcpu_neg_int_en_src(data)                                        ((0x00000004&(data))>>2)
#define SB2_DBG_INT_get_vcpu_neg_int_en(data)                                        ((0x00000004&(data))>>2)
#define SB2_DBG_INT_scpu_neg_int_en_shift                                            (1)
#define SB2_DBG_INT_scpu_neg_int_en_mask                                             (0x00000002)
#define SB2_DBG_INT_scpu_neg_int_en(data)                                            (0x00000002&((data)<<1))
#define SB2_DBG_INT_scpu_neg_int_en_src(data)                                        ((0x00000002&(data))>>1)
#define SB2_DBG_INT_get_scpu_neg_int_en(data)                                        ((0x00000002&(data))>>1)
#define SB2_DBG_INT_write_data_shift                                                 (0)
#define SB2_DBG_INT_write_data_mask                                                  (0x00000001)
#define SB2_DBG_INT_write_data(data)                                                 (0x00000001&((data)<<0))
#define SB2_DBG_INT_write_data_src(data)                                             ((0x00000001&(data))>>0)
#define SB2_DBG_INT_get_write_data(data)                                             ((0x00000001&(data))>>0)


#define SB2_GCRA_CTRL                                                                0x1801A4E4
#define SB2_GCRA_CTRL_reg_addr                                                       "0xB801A4E4"
#define SB2_GCRA_CTRL_reg                                                            0xB801A4E4
#define set_SB2_GCRA_CTRL_reg(data)   (*((volatile unsigned int*) SB2_GCRA_CTRL_reg)=data)
#define get_SB2_GCRA_CTRL_reg   (*((volatile unsigned int*) SB2_GCRA_CTRL_reg))
#define SB2_GCRA_CTRL_inst_adr                                                       "0x0039"
#define SB2_GCRA_CTRL_inst                                                           0x0039
#define SB2_GCRA_CTRL_Gcra_en_shift                                                  (0)
#define SB2_GCRA_CTRL_Gcra_en_mask                                                   (0x00000001)
#define SB2_GCRA_CTRL_Gcra_en(data)                                                  (0x00000001&((data)<<0))
#define SB2_GCRA_CTRL_Gcra_en_src(data)                                              ((0x00000001&(data))>>0)
#define SB2_GCRA_CTRL_get_Gcra_en(data)                                              ((0x00000001&(data))>>0)


#define SB2_GCRA_TIM_CNT                                                             0x1801A4E8
#define SB2_GCRA_TIM_CNT_reg_addr                                                    "0xB801A4E8"
#define SB2_GCRA_TIM_CNT_reg                                                         0xB801A4E8
#define set_SB2_GCRA_TIM_CNT_reg(data)   (*((volatile unsigned int*) SB2_GCRA_TIM_CNT_reg)=data)
#define get_SB2_GCRA_TIM_CNT_reg   (*((volatile unsigned int*) SB2_GCRA_TIM_CNT_reg))
#define SB2_GCRA_TIM_CNT_inst_adr                                                    "0x003A"
#define SB2_GCRA_TIM_CNT_inst                                                        0x003A
#define SB2_GCRA_TIM_CNT_Gcra_tim_cnt_shift                                          (0)
#define SB2_GCRA_TIM_CNT_Gcra_tim_cnt_mask                                           (0xFFFFFFFF)
#define SB2_GCRA_TIM_CNT_Gcra_tim_cnt(data)                                          (0xFFFFFFFF&((data)<<0))
#define SB2_GCRA_TIM_CNT_Gcra_tim_cnt_src(data)                                      ((0xFFFFFFFF&(data))>>0)
#define SB2_GCRA_TIM_CNT_get_Gcra_tim_cnt(data)                                      ((0xFFFFFFFF&(data))>>0)


#define SB2_GCRA_TIM_THR                                                             0x1801A4EC
#define SB2_GCRA_TIM_THR_reg_addr                                                    "0xB801A4EC"
#define SB2_GCRA_TIM_THR_reg                                                         0xB801A4EC
#define set_SB2_GCRA_TIM_THR_reg(data)   (*((volatile unsigned int*) SB2_GCRA_TIM_THR_reg)=data)
#define get_SB2_GCRA_TIM_THR_reg   (*((volatile unsigned int*) SB2_GCRA_TIM_THR_reg))
#define SB2_GCRA_TIM_THR_inst_adr                                                    "0x003B"
#define SB2_GCRA_TIM_THR_inst                                                        0x003B
#define SB2_GCRA_TIM_THR_Gcra_tim_thr_shift                                          (0)
#define SB2_GCRA_TIM_THR_Gcra_tim_thr_mask                                           (0xFFFFFFFF)
#define SB2_GCRA_TIM_THR_Gcra_tim_thr(data)                                          (0xFFFFFFFF&((data)<<0))
#define SB2_GCRA_TIM_THR_Gcra_tim_thr_src(data)                                      ((0xFFFFFFFF&(data))>>0)
#define SB2_GCRA_TIM_THR_get_Gcra_tim_thr(data)                                      ((0xFFFFFFFF&(data))>>0)


#define SB2_GCRA_REQ_NUM                                                             0x1801A4F0
#define SB2_GCRA_REQ_NUM_reg_addr                                                    "0xB801A4F0"
#define SB2_GCRA_REQ_NUM_reg                                                         0xB801A4F0
#define set_SB2_GCRA_REQ_NUM_reg(data)   (*((volatile unsigned int*) SB2_GCRA_REQ_NUM_reg)=data)
#define get_SB2_GCRA_REQ_NUM_reg   (*((volatile unsigned int*) SB2_GCRA_REQ_NUM_reg))
#define SB2_GCRA_REQ_NUM_inst_adr                                                    "0x003C"
#define SB2_GCRA_REQ_NUM_inst                                                        0x003C
#define SB2_GCRA_REQ_NUM_Gcra_req_num_shift                                          (0)
#define SB2_GCRA_REQ_NUM_Gcra_req_num_mask                                           (0x00FFFFFF)
#define SB2_GCRA_REQ_NUM_Gcra_req_num(data)                                          (0x00FFFFFF&((data)<<0))
#define SB2_GCRA_REQ_NUM_Gcra_req_num_src(data)                                      ((0x00FFFFFF&(data))>>0)
#define SB2_GCRA_REQ_NUM_get_Gcra_req_num(data)                                      ((0x00FFFFFF&(data))>>0)


#define SB2_GCRA_REQ_THR                                                             0x1801A4F4
#define SB2_GCRA_REQ_THR_reg_addr                                                    "0xB801A4F4"
#define SB2_GCRA_REQ_THR_reg                                                         0xB801A4F4
#define set_SB2_GCRA_REQ_THR_reg(data)   (*((volatile unsigned int*) SB2_GCRA_REQ_THR_reg)=data)
#define get_SB2_GCRA_REQ_THR_reg   (*((volatile unsigned int*) SB2_GCRA_REQ_THR_reg))
#define SB2_GCRA_REQ_THR_inst_adr                                                    "0x003D"
#define SB2_GCRA_REQ_THR_inst                                                        0x003D
#define SB2_GCRA_REQ_THR_Gcra_req_thr_shift                                          (0)
#define SB2_GCRA_REQ_THR_Gcra_req_thr_mask                                           (0x00FFFFFF)
#define SB2_GCRA_REQ_THR_Gcra_req_thr(data)                                          (0x00FFFFFF&((data)<<0))
#define SB2_GCRA_REQ_THR_Gcra_req_thr_src(data)                                      ((0x00FFFFFF&(data))>>0)
#define SB2_GCRA_REQ_THR_get_Gcra_req_thr(data)                                      ((0x00FFFFFF&(data))>>0)


#define SB2_GREG_START                                                               0x1801A500
#define SB2_GREG_START_reg_addr                                                      "0xB801A500"
#define SB2_GREG_START_reg                                                           0xB801A500
#define set_SB2_GREG_START_reg(data)   (*((volatile unsigned int*) SB2_GREG_START_reg)=data)
#define get_SB2_GREG_START_reg   (*((volatile unsigned int*) SB2_GREG_START_reg))
#define SB2_GREG_START_inst_adr                                                      "0x0040"
#define SB2_GREG_START_inst                                                          0x0040
#define SB2_GREG_START_addr_shift                                                    (0)
#define SB2_GREG_START_addr_mask                                                     (0x0000000F)
#define SB2_GREG_START_addr(data)                                                    (0x0000000F&((data)<<0))
#define SB2_GREG_START_addr_src(data)                                                ((0x0000000F&(data))>>0)
#define SB2_GREG_START_get_addr(data)                                                ((0x0000000F&(data))>>0)


#define SB2_GREG_END                                                                 0x1801A504
#define SB2_GREG_END_reg_addr                                                        "0xB801A504"
#define SB2_GREG_END_reg                                                             0xB801A504
#define set_SB2_GREG_END_reg(data)   (*((volatile unsigned int*) SB2_GREG_END_reg)=data)
#define get_SB2_GREG_END_reg   (*((volatile unsigned int*) SB2_GREG_END_reg))
#define SB2_GREG_END_inst_adr                                                        "0x0041"
#define SB2_GREG_END_inst                                                            0x0041
#define SB2_GREG_END_addr_shift                                                      (0)
#define SB2_GREG_END_addr_mask                                                       (0x0000000F)
#define SB2_GREG_END_addr(data)                                                      (0x0000000F&((data)<<0))
#define SB2_GREG_END_addr_src(data)                                                  ((0x0000000F&(data))>>0)
#define SB2_GREG_END_get_addr(data)                                                  ((0x0000000F&(data))>>0)


#define SB2_GREG                                                                     0x1801A508
#define SB2_GREG_reg_addr                                                            "0xB801A508"
#define SB2_GREG_reg                                                                 0xB801A508
#define set_SB2_GREG_reg(data)   (*((volatile unsigned int*) SB2_GREG_reg)=data)
#define get_SB2_GREG_reg   (*((volatile unsigned int*) SB2_GREG_reg))
#define SB2_GREG_inst_adr                                                            "0x0042"
#define SB2_GREG_inst                                                                0x0042
#define SB2_GREG_offsetB_shift                                                       (4)
#define SB2_GREG_offsetB_mask                                                        (0x000000F0)
#define SB2_GREG_offsetB(data)                                                       (0x000000F0&((data)<<4))
#define SB2_GREG_offsetB_src(data)                                                   ((0x000000F0&(data))>>4)
#define SB2_GREG_get_offsetB(data)                                                   ((0x000000F0&(data))>>4)
#define SB2_GREG_offsetA_shift                                                       (0)
#define SB2_GREG_offsetA_mask                                                        (0x0000000F)
#define SB2_GREG_offsetA(data)                                                       (0x0000000F&((data)<<0))
#define SB2_GREG_offsetA_src(data)                                                   ((0x0000000F&(data))>>0)
#define SB2_GREG_get_offsetA(data)                                                   ((0x0000000F&(data))>>0)


#define SB2_DUMMY_0                                                                  0x1801A600
#define SB2_DUMMY_1                                                                  0x1801A604
#define SB2_DUMMY_2                                                                  0x1801A608
#define SB2_DUMMY_3                                                                  0x1801A60C
#define SB2_DUMMY_0_reg_addr                                                         "0xB801A600"
#define SB2_DUMMY_1_reg_addr                                                         "0xB801A604"
#define SB2_DUMMY_2_reg_addr                                                         "0xB801A608"
#define SB2_DUMMY_3_reg_addr                                                         "0xB801A60C"
#define SB2_DUMMY_0_reg                                                              0xB801A600
#define SB2_DUMMY_1_reg                                                              0xB801A604
#define SB2_DUMMY_2_reg                                                              0xB801A608
#define SB2_DUMMY_3_reg                                                              0xB801A60C
#define set_SB2_DUMMY_0_reg(data)   (*((volatile unsigned int*) SB2_DUMMY_0_reg)=data)
#define set_SB2_DUMMY_1_reg(data)   (*((volatile unsigned int*) SB2_DUMMY_1_reg)=data)
#define set_SB2_DUMMY_2_reg(data)   (*((volatile unsigned int*) SB2_DUMMY_2_reg)=data)
#define set_SB2_DUMMY_3_reg(data)   (*((volatile unsigned int*) SB2_DUMMY_3_reg)=data)
#define get_SB2_DUMMY_0_reg   (*((volatile unsigned int*) SB2_DUMMY_0_reg))
#define get_SB2_DUMMY_1_reg   (*((volatile unsigned int*) SB2_DUMMY_1_reg))
#define get_SB2_DUMMY_2_reg   (*((volatile unsigned int*) SB2_DUMMY_2_reg))
#define get_SB2_DUMMY_3_reg   (*((volatile unsigned int*) SB2_DUMMY_3_reg))
#define SB2_DUMMY_0_inst_adr                                                         "0x0080"
#define SB2_DUMMY_1_inst_adr                                                         "0x0081"
#define SB2_DUMMY_2_inst_adr                                                         "0x0082"
#define SB2_DUMMY_3_inst_adr                                                         "0x0083"
#define SB2_DUMMY_0_inst                                                             0x0080
#define SB2_DUMMY_1_inst                                                             0x0081
#define SB2_DUMMY_2_inst                                                             0x0082
#define SB2_DUMMY_3_inst                                                             0x0083
#define SB2_DUMMY_data_shift                                                         (0)
#define SB2_DUMMY_data_mask                                                          (0xFFFFFFFF)
#define SB2_DUMMY_data(data)                                                         (0xFFFFFFFF&((data)<<0))
#define SB2_DUMMY_data_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define SB2_DUMMY_get_data(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define SB2_SHADOW_0                                                                 0x1801A610
#define SB2_SHADOW_1                                                                 0x1801A614
#define SB2_SHADOW_2                                                                 0x1801A618
#define SB2_SHADOW_3                                                                 0x1801A61C
#define SB2_SHADOW_0_reg_addr                                                        "0xB801A610"
#define SB2_SHADOW_1_reg_addr                                                        "0xB801A614"
#define SB2_SHADOW_2_reg_addr                                                        "0xB801A618"
#define SB2_SHADOW_3_reg_addr                                                        "0xB801A61C"
#define SB2_SHADOW_0_reg                                                             0xB801A610
#define SB2_SHADOW_1_reg                                                             0xB801A614
#define SB2_SHADOW_2_reg                                                             0xB801A618
#define SB2_SHADOW_3_reg                                                             0xB801A61C
#define set_SB2_SHADOW_0_reg(data)   (*((volatile unsigned int*) SB2_SHADOW_0_reg)=data)
#define set_SB2_SHADOW_1_reg(data)   (*((volatile unsigned int*) SB2_SHADOW_1_reg)=data)
#define set_SB2_SHADOW_2_reg(data)   (*((volatile unsigned int*) SB2_SHADOW_2_reg)=data)
#define set_SB2_SHADOW_3_reg(data)   (*((volatile unsigned int*) SB2_SHADOW_3_reg)=data)
#define get_SB2_SHADOW_0_reg   (*((volatile unsigned int*) SB2_SHADOW_0_reg))
#define get_SB2_SHADOW_1_reg   (*((volatile unsigned int*) SB2_SHADOW_1_reg))
#define get_SB2_SHADOW_2_reg   (*((volatile unsigned int*) SB2_SHADOW_2_reg))
#define get_SB2_SHADOW_3_reg   (*((volatile unsigned int*) SB2_SHADOW_3_reg))
#define SB2_SHADOW_0_inst_adr                                                        "0x0084"
#define SB2_SHADOW_1_inst_adr                                                        "0x0085"
#define SB2_SHADOW_2_inst_adr                                                        "0x0086"
#define SB2_SHADOW_3_inst_adr                                                        "0x0087"
#define SB2_SHADOW_0_inst                                                            0x0084
#define SB2_SHADOW_1_inst                                                            0x0085
#define SB2_SHADOW_2_inst                                                            0x0086
#define SB2_SHADOW_3_inst                                                            0x0087
#define SB2_SHADOW_data_shift                                                        (0)
#define SB2_SHADOW_data_mask                                                         (0xFFFFFFFF)
#define SB2_SHADOW_data(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB2_SHADOW_data_src(data)                                                    ((0xFFFFFFFF&(data))>>0)
#define SB2_SHADOW_get_data(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB2_HD_SEM_NEW_0                                                             0x1801A620
#define SB2_HD_SEM_NEW_1                                                             0x1801A624
#define SB2_HD_SEM_NEW_2                                                             0x1801A628
#define SB2_HD_SEM_NEW_3                                                             0x1801A62C
#define SB2_HD_SEM_NEW_4                                                             0x1801A630
#define SB2_HD_SEM_NEW_5                                                             0x1801A634
#define SB2_HD_SEM_NEW_6                                                             0x1801A638
#define SB2_HD_SEM_NEW_7                                                             0x1801A63C
#define SB2_HD_SEM_NEW_0_reg_addr                                                    "0xB801A620"
#define SB2_HD_SEM_NEW_1_reg_addr                                                    "0xB801A624"
#define SB2_HD_SEM_NEW_2_reg_addr                                                    "0xB801A628"
#define SB2_HD_SEM_NEW_3_reg_addr                                                    "0xB801A62C"
#define SB2_HD_SEM_NEW_4_reg_addr                                                    "0xB801A630"
#define SB2_HD_SEM_NEW_5_reg_addr                                                    "0xB801A634"
#define SB2_HD_SEM_NEW_6_reg_addr                                                    "0xB801A638"
#define SB2_HD_SEM_NEW_7_reg_addr                                                    "0xB801A63C"
#define SB2_HD_SEM_NEW_0_reg                                                         0xB801A620
#define SB2_HD_SEM_NEW_1_reg                                                         0xB801A624
#define SB2_HD_SEM_NEW_2_reg                                                         0xB801A628
#define SB2_HD_SEM_NEW_3_reg                                                         0xB801A62C
#define SB2_HD_SEM_NEW_4_reg                                                         0xB801A630
#define SB2_HD_SEM_NEW_5_reg                                                         0xB801A634
#define SB2_HD_SEM_NEW_6_reg                                                         0xB801A638
#define SB2_HD_SEM_NEW_7_reg                                                         0xB801A63C
#define set_SB2_HD_SEM_NEW_0_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_0_reg)=data)
#define set_SB2_HD_SEM_NEW_1_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_1_reg)=data)
#define set_SB2_HD_SEM_NEW_2_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_2_reg)=data)
#define set_SB2_HD_SEM_NEW_3_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_3_reg)=data)
#define set_SB2_HD_SEM_NEW_4_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_4_reg)=data)
#define set_SB2_HD_SEM_NEW_5_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_5_reg)=data)
#define set_SB2_HD_SEM_NEW_6_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_6_reg)=data)
#define set_SB2_HD_SEM_NEW_7_reg(data)   (*((volatile unsigned int*) SB2_HD_SEM_NEW_7_reg)=data)
#define get_SB2_HD_SEM_NEW_0_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_0_reg))
#define get_SB2_HD_SEM_NEW_1_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_1_reg))
#define get_SB2_HD_SEM_NEW_2_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_2_reg))
#define get_SB2_HD_SEM_NEW_3_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_3_reg))
#define get_SB2_HD_SEM_NEW_4_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_4_reg))
#define get_SB2_HD_SEM_NEW_5_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_5_reg))
#define get_SB2_HD_SEM_NEW_6_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_6_reg))
#define get_SB2_HD_SEM_NEW_7_reg   (*((volatile unsigned int*) SB2_HD_SEM_NEW_7_reg))
#define SB2_HD_SEM_NEW_0_inst_adr                                                    "0x0088"
#define SB2_HD_SEM_NEW_1_inst_adr                                                    "0x0089"
#define SB2_HD_SEM_NEW_2_inst_adr                                                    "0x008A"
#define SB2_HD_SEM_NEW_3_inst_adr                                                    "0x008B"
#define SB2_HD_SEM_NEW_4_inst_adr                                                    "0x008C"
#define SB2_HD_SEM_NEW_5_inst_adr                                                    "0x008D"
#define SB2_HD_SEM_NEW_6_inst_adr                                                    "0x008E"
#define SB2_HD_SEM_NEW_7_inst_adr                                                    "0x008F"
#define SB2_HD_SEM_NEW_0_inst                                                        0x0088
#define SB2_HD_SEM_NEW_1_inst                                                        0x0089
#define SB2_HD_SEM_NEW_2_inst                                                        0x008A
#define SB2_HD_SEM_NEW_3_inst                                                        0x008B
#define SB2_HD_SEM_NEW_4_inst                                                        0x008C
#define SB2_HD_SEM_NEW_5_inst                                                        0x008D
#define SB2_HD_SEM_NEW_6_inst                                                        0x008E
#define SB2_HD_SEM_NEW_7_inst                                                        0x008F
#define SB2_HD_SEM_NEW_Semaphore_shift                                               (0)
#define SB2_HD_SEM_NEW_Semaphore_mask                                                (0x00000001)
#define SB2_HD_SEM_NEW_Semaphore(data)                                               (0x00000001&((data)<<0))
#define SB2_HD_SEM_NEW_Semaphore_src(data)                                           ((0x00000001&(data))>>0)
#define SB2_HD_SEM_NEW_get_Semaphore(data)                                           ((0x00000001&(data))>>0)


#define SB2_SCPU_INT_MUX                                                             0x1801A640
#define SB2_SCPU_INT_MUX_reg_addr                                                    "0xB801A640"
#define SB2_SCPU_INT_MUX_reg                                                         0xB801A640
#define set_SB2_SCPU_INT_MUX_reg(data)   (*((volatile unsigned int*) SB2_SCPU_INT_MUX_reg)=data)
#define get_SB2_SCPU_INT_MUX_reg   (*((volatile unsigned int*) SB2_SCPU_INT_MUX_reg))
#define SB2_SCPU_INT_MUX_inst_adr                                                    "0x0090"
#define SB2_SCPU_INT_MUX_inst                                                        0x0090
#define SB2_SCPU_INT_MUX_dc_int_en_shift                                             (12)
#define SB2_SCPU_INT_MUX_dc_int_en_mask                                              (0x00001000)
#define SB2_SCPU_INT_MUX_dc_int_en(data)                                             (0x00001000&((data)<<12))
#define SB2_SCPU_INT_MUX_dc_int_en_src(data)                                         ((0x00001000&(data))>>12)
#define SB2_SCPU_INT_MUX_get_dc_int_en(data)                                         ((0x00001000&(data))>>12)
#define SB2_SCPU_INT_MUX_gpu_int_en_shift                                            (11)
#define SB2_SCPU_INT_MUX_gpu_int_en_mask                                             (0x00000800)
#define SB2_SCPU_INT_MUX_gpu_int_en(data)                                            (0x00000800&((data)<<11))
#define SB2_SCPU_INT_MUX_gpu_int_en_src(data)                                        ((0x00000800&(data))>>11)
#define SB2_SCPU_INT_MUX_get_gpu_int_en(data)                                        ((0x00000800&(data))>>11)
#define SB2_SCPU_INT_MUX_pcie_int_en_shift                                           (10)
#define SB2_SCPU_INT_MUX_pcie_int_en_mask                                            (0x00000400)
#define SB2_SCPU_INT_MUX_pcie_int_en(data)                                           (0x00000400&((data)<<10))
#define SB2_SCPU_INT_MUX_pcie_int_en_src(data)                                       ((0x00000400&(data))>>10)
#define SB2_SCPU_INT_MUX_get_pcie_int_en(data)                                       ((0x00000400&(data))>>10)
#define SB2_SCPU_INT_MUX_md_int_en_shift                                             (9)
#define SB2_SCPU_INT_MUX_md_int_en_mask                                              (0x00000200)
#define SB2_SCPU_INT_MUX_md_int_en(data)                                             (0x00000200&((data)<<9))
#define SB2_SCPU_INT_MUX_md_int_en_src(data)                                         ((0x00000200&(data))>>9)
#define SB2_SCPU_INT_MUX_get_md_int_en(data)                                         ((0x00000200&(data))>>9)
#define SB2_SCPU_INT_MUX_tp_int_en_shift                                             (8)
#define SB2_SCPU_INT_MUX_tp_int_en_mask                                              (0x00000100)
#define SB2_SCPU_INT_MUX_tp_int_en(data)                                             (0x00000100&((data)<<8))
#define SB2_SCPU_INT_MUX_tp_int_en_src(data)                                         ((0x00000100&(data))>>8)
#define SB2_SCPU_INT_MUX_get_tp_int_en(data)                                         ((0x00000100&(data))>>8)
#define SB2_SCPU_INT_MUX_hdmi_int_en_shift                                           (7)
#define SB2_SCPU_INT_MUX_hdmi_int_en_mask                                            (0x00000080)
#define SB2_SCPU_INT_MUX_hdmi_int_en(data)                                           (0x00000080&((data)<<7))
#define SB2_SCPU_INT_MUX_hdmi_int_en_src(data)                                       ((0x00000080&(data))>>7)
#define SB2_SCPU_INT_MUX_get_hdmi_int_en(data)                                       ((0x00000080&(data))>>7)
#define SB2_SCPU_INT_MUX_etn_int_en_shift                                            (6)
#define SB2_SCPU_INT_MUX_etn_int_en_mask                                             (0x00000040)
#define SB2_SCPU_INT_MUX_etn_int_en(data)                                            (0x00000040&((data)<<6))
#define SB2_SCPU_INT_MUX_etn_int_en_src(data)                                        ((0x00000040&(data))>>6)
#define SB2_SCPU_INT_MUX_get_etn_int_en(data)                                        ((0x00000040&(data))>>6)
#define SB2_SCPU_INT_MUX_usb_int_en_shift                                            (5)
#define SB2_SCPU_INT_MUX_usb_int_en_mask                                             (0x00000020)
#define SB2_SCPU_INT_MUX_usb_int_en(data)                                            (0x00000020&((data)<<5))
#define SB2_SCPU_INT_MUX_usb_int_en_src(data)                                        ((0x00000020&(data))>>5)
#define SB2_SCPU_INT_MUX_get_usb_int_en(data)                                        ((0x00000020&(data))>>5)
#define SB2_SCPU_INT_MUX_cr_int_en_shift                                             (4)
#define SB2_SCPU_INT_MUX_cr_int_en_mask                                              (0x00000010)
#define SB2_SCPU_INT_MUX_cr_int_en(data)                                             (0x00000010&((data)<<4))
#define SB2_SCPU_INT_MUX_cr_int_en_src(data)                                         ((0x00000010&(data))>>4)
#define SB2_SCPU_INT_MUX_get_cr_int_en(data)                                         ((0x00000010&(data))>>4)
#define SB2_SCPU_INT_MUX_nf_int_en_shift                                             (3)
#define SB2_SCPU_INT_MUX_nf_int_en_mask                                              (0x00000008)
#define SB2_SCPU_INT_MUX_nf_int_en(data)                                             (0x00000008&((data)<<3))
#define SB2_SCPU_INT_MUX_nf_int_en_src(data)                                         ((0x00000008&(data))>>3)
#define SB2_SCPU_INT_MUX_get_nf_int_en(data)                                         ((0x00000008&(data))>>3)
#define SB2_SCPU_INT_MUX_sata_int_en_shift                                           (2)
#define SB2_SCPU_INT_MUX_sata_int_en_mask                                            (0x00000004)
#define SB2_SCPU_INT_MUX_sata_int_en(data)                                           (0x00000004&((data)<<2))
#define SB2_SCPU_INT_MUX_sata_int_en_src(data)                                       ((0x00000004&(data))>>2)
#define SB2_SCPU_INT_MUX_get_sata_int_en(data)                                       ((0x00000004&(data))>>2)
#define SB2_SCPU_INT_MUX_se_int_en_shift                                             (1)
#define SB2_SCPU_INT_MUX_se_int_en_mask                                              (0x00000002)
#define SB2_SCPU_INT_MUX_se_int_en(data)                                             (0x00000002&((data)<<1))
#define SB2_SCPU_INT_MUX_se_int_en_src(data)                                         ((0x00000002&(data))>>1)
#define SB2_SCPU_INT_MUX_get_se_int_en(data)                                         ((0x00000002&(data))>>1)
#define SB2_SCPU_INT_MUX_cp_int_en_shift                                             (0)
#define SB2_SCPU_INT_MUX_cp_int_en_mask                                              (0x00000001)
#define SB2_SCPU_INT_MUX_cp_int_en(data)                                             (0x00000001&((data)<<0))
#define SB2_SCPU_INT_MUX_cp_int_en_src(data)                                         ((0x00000001&(data))>>0)
#define SB2_SCPU_INT_MUX_get_cp_int_en(data)                                         ((0x00000001&(data))>>0)


#define SB2_KCPU_INT_MUX                                                             0x1801A644
#define SB2_KCPU_INT_MUX_reg_addr                                                    "0xB801A644"
#define SB2_KCPU_INT_MUX_reg                                                         0xB801A644
#define set_SB2_KCPU_INT_MUX_reg(data)   (*((volatile unsigned int*) SB2_KCPU_INT_MUX_reg)=data)
#define get_SB2_KCPU_INT_MUX_reg   (*((volatile unsigned int*) SB2_KCPU_INT_MUX_reg))
#define SB2_KCPU_INT_MUX_inst_adr                                                    "0x0091"
#define SB2_KCPU_INT_MUX_inst                                                        0x0091
#define SB2_KCPU_INT_MUX_dc_int_en_shift                                             (12)
#define SB2_KCPU_INT_MUX_dc_int_en_mask                                              (0x00001000)
#define SB2_KCPU_INT_MUX_dc_int_en(data)                                             (0x00001000&((data)<<12))
#define SB2_KCPU_INT_MUX_dc_int_en_src(data)                                         ((0x00001000&(data))>>12)
#define SB2_KCPU_INT_MUX_get_dc_int_en(data)                                         ((0x00001000&(data))>>12)
#define SB2_KCPU_INT_MUX_gpu_int_en_shift                                            (11)
#define SB2_KCPU_INT_MUX_gpu_int_en_mask                                             (0x00000800)
#define SB2_KCPU_INT_MUX_gpu_int_en(data)                                            (0x00000800&((data)<<11))
#define SB2_KCPU_INT_MUX_gpu_int_en_src(data)                                        ((0x00000800&(data))>>11)
#define SB2_KCPU_INT_MUX_get_gpu_int_en(data)                                        ((0x00000800&(data))>>11)
#define SB2_KCPU_INT_MUX_pcie_int_en_shift                                           (10)
#define SB2_KCPU_INT_MUX_pcie_int_en_mask                                            (0x00000400)
#define SB2_KCPU_INT_MUX_pcie_int_en(data)                                           (0x00000400&((data)<<10))
#define SB2_KCPU_INT_MUX_pcie_int_en_src(data)                                       ((0x00000400&(data))>>10)
#define SB2_KCPU_INT_MUX_get_pcie_int_en(data)                                       ((0x00000400&(data))>>10)
#define SB2_KCPU_INT_MUX_md_int_en_shift                                             (9)
#define SB2_KCPU_INT_MUX_md_int_en_mask                                              (0x00000200)
#define SB2_KCPU_INT_MUX_md_int_en(data)                                             (0x00000200&((data)<<9))
#define SB2_KCPU_INT_MUX_md_int_en_src(data)                                         ((0x00000200&(data))>>9)
#define SB2_KCPU_INT_MUX_get_md_int_en(data)                                         ((0x00000200&(data))>>9)
#define SB2_KCPU_INT_MUX_tp_int_en_shift                                             (8)
#define SB2_KCPU_INT_MUX_tp_int_en_mask                                              (0x00000100)
#define SB2_KCPU_INT_MUX_tp_int_en(data)                                             (0x00000100&((data)<<8))
#define SB2_KCPU_INT_MUX_tp_int_en_src(data)                                         ((0x00000100&(data))>>8)
#define SB2_KCPU_INT_MUX_get_tp_int_en(data)                                         ((0x00000100&(data))>>8)
#define SB2_KCPU_INT_MUX_hdmi_int_en_shift                                           (7)
#define SB2_KCPU_INT_MUX_hdmi_int_en_mask                                            (0x00000080)
#define SB2_KCPU_INT_MUX_hdmi_int_en(data)                                           (0x00000080&((data)<<7))
#define SB2_KCPU_INT_MUX_hdmi_int_en_src(data)                                       ((0x00000080&(data))>>7)
#define SB2_KCPU_INT_MUX_get_hdmi_int_en(data)                                       ((0x00000080&(data))>>7)
#define SB2_KCPU_INT_MUX_etn_int_en_shift                                            (6)
#define SB2_KCPU_INT_MUX_etn_int_en_mask                                             (0x00000040)
#define SB2_KCPU_INT_MUX_etn_int_en(data)                                            (0x00000040&((data)<<6))
#define SB2_KCPU_INT_MUX_etn_int_en_src(data)                                        ((0x00000040&(data))>>6)
#define SB2_KCPU_INT_MUX_get_etn_int_en(data)                                        ((0x00000040&(data))>>6)
#define SB2_KCPU_INT_MUX_usb_int_en_shift                                            (5)
#define SB2_KCPU_INT_MUX_usb_int_en_mask                                             (0x00000020)
#define SB2_KCPU_INT_MUX_usb_int_en(data)                                            (0x00000020&((data)<<5))
#define SB2_KCPU_INT_MUX_usb_int_en_src(data)                                        ((0x00000020&(data))>>5)
#define SB2_KCPU_INT_MUX_get_usb_int_en(data)                                        ((0x00000020&(data))>>5)
#define SB2_KCPU_INT_MUX_cr_int_en_shift                                             (4)
#define SB2_KCPU_INT_MUX_cr_int_en_mask                                              (0x00000010)
#define SB2_KCPU_INT_MUX_cr_int_en(data)                                             (0x00000010&((data)<<4))
#define SB2_KCPU_INT_MUX_cr_int_en_src(data)                                         ((0x00000010&(data))>>4)
#define SB2_KCPU_INT_MUX_get_cr_int_en(data)                                         ((0x00000010&(data))>>4)
#define SB2_KCPU_INT_MUX_nf_int_en_shift                                             (3)
#define SB2_KCPU_INT_MUX_nf_int_en_mask                                              (0x00000008)
#define SB2_KCPU_INT_MUX_nf_int_en(data)                                             (0x00000008&((data)<<3))
#define SB2_KCPU_INT_MUX_nf_int_en_src(data)                                         ((0x00000008&(data))>>3)
#define SB2_KCPU_INT_MUX_get_nf_int_en(data)                                         ((0x00000008&(data))>>3)
#define SB2_KCPU_INT_MUX_sata_int_en_shift                                           (2)
#define SB2_KCPU_INT_MUX_sata_int_en_mask                                            (0x00000004)
#define SB2_KCPU_INT_MUX_sata_int_en(data)                                           (0x00000004&((data)<<2))
#define SB2_KCPU_INT_MUX_sata_int_en_src(data)                                       ((0x00000004&(data))>>2)
#define SB2_KCPU_INT_MUX_get_sata_int_en(data)                                       ((0x00000004&(data))>>2)
#define SB2_KCPU_INT_MUX_se_int_en_shift                                             (1)
#define SB2_KCPU_INT_MUX_se_int_en_mask                                              (0x00000002)
#define SB2_KCPU_INT_MUX_se_int_en(data)                                             (0x00000002&((data)<<1))
#define SB2_KCPU_INT_MUX_se_int_en_src(data)                                         ((0x00000002&(data))>>1)
#define SB2_KCPU_INT_MUX_get_se_int_en(data)                                         ((0x00000002&(data))>>1)
#define SB2_KCPU_INT_MUX_cp_int_en_shift                                             (0)
#define SB2_KCPU_INT_MUX_cp_int_en_mask                                              (0x00000001)
#define SB2_KCPU_INT_MUX_cp_int_en(data)                                             (0x00000001&((data)<<0))
#define SB2_KCPU_INT_MUX_cp_int_en_src(data)                                         ((0x00000001&(data))>>0)
#define SB2_KCPU_INT_MUX_get_cp_int_en(data)                                         ((0x00000001&(data))>>0)


#define SB2_SECURITY_KCPU                                                            0x1801A648
#define SB2_SECURITY_KCPU_reg_addr                                                   "0xB801A648"
#define SB2_SECURITY_KCPU_reg                                                        0xB801A648
#define set_SB2_SECURITY_KCPU_reg(data)   (*((volatile unsigned int*) SB2_SECURITY_KCPU_reg)=data)
#define get_SB2_SECURITY_KCPU_reg   (*((volatile unsigned int*) SB2_SECURITY_KCPU_reg))
#define SB2_SECURITY_KCPU_inst_adr                                                   "0x0092"
#define SB2_SECURITY_KCPU_inst                                                       0x0092
#define SB2_SECURITY_KCPU_write_en2_shift                                            (3)
#define SB2_SECURITY_KCPU_write_en2_mask                                             (0x00000008)
#define SB2_SECURITY_KCPU_write_en2(data)                                            (0x00000008&((data)<<3))
#define SB2_SECURITY_KCPU_write_en2_src(data)                                        ((0x00000008&(data))>>3)
#define SB2_SECURITY_KCPU_get_write_en2(data)                                        ((0x00000008&(data))>>3)
#define SB2_SECURITY_KCPU_tp_sec_reg_ctrl_shift                                      (2)
#define SB2_SECURITY_KCPU_tp_sec_reg_ctrl_mask                                       (0x00000004)
#define SB2_SECURITY_KCPU_tp_sec_reg_ctrl(data)                                      (0x00000004&((data)<<2))
#define SB2_SECURITY_KCPU_tp_sec_reg_ctrl_src(data)                                  ((0x00000004&(data))>>2)
#define SB2_SECURITY_KCPU_get_tp_sec_reg_ctrl(data)                                  ((0x00000004&(data))>>2)
#define SB2_SECURITY_KCPU_write_en1_shift                                            (1)
#define SB2_SECURITY_KCPU_write_en1_mask                                             (0x00000002)
#define SB2_SECURITY_KCPU_write_en1(data)                                            (0x00000002&((data)<<1))
#define SB2_SECURITY_KCPU_write_en1_src(data)                                        ((0x00000002&(data))>>1)
#define SB2_SECURITY_KCPU_get_write_en1(data)                                        ((0x00000002&(data))>>1)
#define SB2_SECURITY_KCPU_cp_sec_reg_ctrl_shift                                      (0)
#define SB2_SECURITY_KCPU_cp_sec_reg_ctrl_mask                                       (0x00000001)
#define SB2_SECURITY_KCPU_cp_sec_reg_ctrl(data)                                      (0x00000001&((data)<<0))
#define SB2_SECURITY_KCPU_cp_sec_reg_ctrl_src(data)                                  ((0x00000001&(data))>>0)
#define SB2_SECURITY_KCPU_get_cp_sec_reg_ctrl(data)                                  ((0x00000001&(data))>>0)


#define SB2_RBUS_INVA_ACC_CTRL                                                       0x1801A650
#define SB2_RBUS_INVA_ACC_CTRL_reg_addr                                              "0xB801A650"
#define SB2_RBUS_INVA_ACC_CTRL_reg                                                   0xB801A650
#define set_SB2_RBUS_INVA_ACC_CTRL_reg(data)   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_CTRL_reg)=data)
#define get_SB2_RBUS_INVA_ACC_CTRL_reg   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_CTRL_reg))
#define SB2_RBUS_INVA_ACC_CTRL_inst_adr                                              "0x0094"
#define SB2_RBUS_INVA_ACC_CTRL_inst                                                  0x0094
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec2_en_shift                                 (2)
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec2_en_mask                                  (0x00000004)
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec2_en(data)                                 (0x00000004&((data)<<2))
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec2_en_src(data)                             ((0x00000004&(data))>>2)
#define SB2_RBUS_INVA_ACC_CTRL_get_inva_wr_sec2_en(data)                             ((0x00000004&(data))>>2)
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec1_en_shift                                 (0)
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec1_en_mask                                  (0x00000001)
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec1_en(data)                                 (0x00000001&((data)<<0))
#define SB2_RBUS_INVA_ACC_CTRL_inva_wr_sec1_en_src(data)                             ((0x00000001&(data))>>0)
#define SB2_RBUS_INVA_ACC_CTRL_get_inva_wr_sec1_en(data)                             ((0x00000001&(data))>>0)


#define SB2_RBUS_INVA_ACC_SEC1_START                                                 0x1801A654
#define SB2_RBUS_INVA_ACC_SEC1_START_reg_addr                                        "0xB801A654"
#define SB2_RBUS_INVA_ACC_SEC1_START_reg                                             0xB801A654
#define set_SB2_RBUS_INVA_ACC_SEC1_START_reg(data)   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC1_START_reg)=data)
#define get_SB2_RBUS_INVA_ACC_SEC1_START_reg   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC1_START_reg))
#define SB2_RBUS_INVA_ACC_SEC1_START_inst_adr                                        "0x0095"
#define SB2_RBUS_INVA_ACC_SEC1_START_inst                                            0x0095
#define SB2_RBUS_INVA_ACC_SEC1_START_sec1_start_shift                                (2)
#define SB2_RBUS_INVA_ACC_SEC1_START_sec1_start_mask                                 (0x0003FFFC)
#define SB2_RBUS_INVA_ACC_SEC1_START_sec1_start(data)                                (0x0003FFFC&((data)<<2))
#define SB2_RBUS_INVA_ACC_SEC1_START_sec1_start_src(data)                            ((0x0003FFFC&(data))>>2)
#define SB2_RBUS_INVA_ACC_SEC1_START_get_sec1_start(data)                            ((0x0003FFFC&(data))>>2)


#define SB2_RBUS_INVA_ACC_SEC1_END                                                   0x1801A658
#define SB2_RBUS_INVA_ACC_SEC1_END_reg_addr                                          "0xB801A658"
#define SB2_RBUS_INVA_ACC_SEC1_END_reg                                               0xB801A658
#define set_SB2_RBUS_INVA_ACC_SEC1_END_reg(data)   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC1_END_reg)=data)
#define get_SB2_RBUS_INVA_ACC_SEC1_END_reg   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC1_END_reg))
#define SB2_RBUS_INVA_ACC_SEC1_END_inst_adr                                          "0x0096"
#define SB2_RBUS_INVA_ACC_SEC1_END_inst                                              0x0096
#define SB2_RBUS_INVA_ACC_SEC1_END_sec1_end_shift                                    (2)
#define SB2_RBUS_INVA_ACC_SEC1_END_sec1_end_mask                                     (0x0003FFFC)
#define SB2_RBUS_INVA_ACC_SEC1_END_sec1_end(data)                                    (0x0003FFFC&((data)<<2))
#define SB2_RBUS_INVA_ACC_SEC1_END_sec1_end_src(data)                                ((0x0003FFFC&(data))>>2)
#define SB2_RBUS_INVA_ACC_SEC1_END_get_sec1_end(data)                                ((0x0003FFFC&(data))>>2)


#define SB2_RBUS_INVA_ACC_SEC2_START                                                 0x1801A65C
#define SB2_RBUS_INVA_ACC_SEC2_START_reg_addr                                        "0xB801A65C"
#define SB2_RBUS_INVA_ACC_SEC2_START_reg                                             0xB801A65C
#define set_SB2_RBUS_INVA_ACC_SEC2_START_reg(data)   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC2_START_reg)=data)
#define get_SB2_RBUS_INVA_ACC_SEC2_START_reg   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC2_START_reg))
#define SB2_RBUS_INVA_ACC_SEC2_START_inst_adr                                        "0x0097"
#define SB2_RBUS_INVA_ACC_SEC2_START_inst                                            0x0097
#define SB2_RBUS_INVA_ACC_SEC2_START_sec2_start_shift                                (2)
#define SB2_RBUS_INVA_ACC_SEC2_START_sec2_start_mask                                 (0x0003FFFC)
#define SB2_RBUS_INVA_ACC_SEC2_START_sec2_start(data)                                (0x0003FFFC&((data)<<2))
#define SB2_RBUS_INVA_ACC_SEC2_START_sec2_start_src(data)                            ((0x0003FFFC&(data))>>2)
#define SB2_RBUS_INVA_ACC_SEC2_START_get_sec2_start(data)                            ((0x0003FFFC&(data))>>2)


#define SB2_RBUS_INVA_ACC_SEC2_END                                                   0x1801A660
#define SB2_RBUS_INVA_ACC_SEC2_END_reg_addr                                          "0xB801A660"
#define SB2_RBUS_INVA_ACC_SEC2_END_reg                                               0xB801A660
#define set_SB2_RBUS_INVA_ACC_SEC2_END_reg(data)   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC2_END_reg)=data)
#define get_SB2_RBUS_INVA_ACC_SEC2_END_reg   (*((volatile unsigned int*) SB2_RBUS_INVA_ACC_SEC2_END_reg))
#define SB2_RBUS_INVA_ACC_SEC2_END_inst_adr                                          "0x0098"
#define SB2_RBUS_INVA_ACC_SEC2_END_inst                                              0x0098
#define SB2_RBUS_INVA_ACC_SEC2_END_sec2_end_shift                                    (2)
#define SB2_RBUS_INVA_ACC_SEC2_END_sec2_end_mask                                     (0x0003FFFC)
#define SB2_RBUS_INVA_ACC_SEC2_END_sec2_end(data)                                    (0x0003FFFC&((data)<<2))
#define SB2_RBUS_INVA_ACC_SEC2_END_sec2_end_src(data)                                ((0x0003FFFC&(data))>>2)
#define SB2_RBUS_INVA_ACC_SEC2_END_get_sec2_end(data)                                ((0x0003FFFC&(data))>>2)


#define SB2_RBUS_CMP_CTRL                                                            0x1801A664
#define SB2_RBUS_CMP_CTRL_reg_addr                                                   "0xB801A664"
#define SB2_RBUS_CMP_CTRL_reg                                                        0xB801A664
#define set_SB2_RBUS_CMP_CTRL_reg(data)   (*((volatile unsigned int*) SB2_RBUS_CMP_CTRL_reg)=data)
#define get_SB2_RBUS_CMP_CTRL_reg   (*((volatile unsigned int*) SB2_RBUS_CMP_CTRL_reg))
#define SB2_RBUS_CMP_CTRL_inst_adr                                                   "0x0099"
#define SB2_RBUS_CMP_CTRL_inst                                                       0x0099
#define SB2_RBUS_CMP_CTRL_crc_cmp_latch_en_shift                                     (0)
#define SB2_RBUS_CMP_CTRL_crc_cmp_latch_en_mask                                      (0x00000001)
#define SB2_RBUS_CMP_CTRL_crc_cmp_latch_en(data)                                     (0x00000001&((data)<<0))
#define SB2_RBUS_CMP_CTRL_crc_cmp_latch_en_src(data)                                 ((0x00000001&(data))>>0)
#define SB2_RBUS_CMP_CTRL_get_crc_cmp_latch_en(data)                                 ((0x00000001&(data))>>0)


#define SB2_RBUS_CMP_DATA                                                            0x1801A668
#define SB2_RBUS_CMP_DATA_reg_addr                                                   "0xB801A668"
#define SB2_RBUS_CMP_DATA_reg                                                        0xB801A668
#define set_SB2_RBUS_CMP_DATA_reg(data)   (*((volatile unsigned int*) SB2_RBUS_CMP_DATA_reg)=data)
#define get_SB2_RBUS_CMP_DATA_reg   (*((volatile unsigned int*) SB2_RBUS_CMP_DATA_reg))
#define SB2_RBUS_CMP_DATA_inst_adr                                                   "0x009A"
#define SB2_RBUS_CMP_DATA_inst                                                       0x009A
#define SB2_RBUS_CMP_DATA_crc_cmp_expect_shift                                       (0)
#define SB2_RBUS_CMP_DATA_crc_cmp_expect_mask                                        (0xFFFFFFFF)
#define SB2_RBUS_CMP_DATA_crc_cmp_expect(data)                                       (0xFFFFFFFF&((data)<<0))
#define SB2_RBUS_CMP_DATA_crc_cmp_expect_src(data)                                   ((0xFFFFFFFF&(data))>>0)
#define SB2_RBUS_CMP_DATA_get_crc_cmp_expect(data)                                   ((0xFFFFFFFF&(data))>>0)


#define SB2_RBUS_CMP_RESULT                                                          0x1801A66C
#define SB2_RBUS_CMP_RESULT_reg_addr                                                 "0xB801A66C"
#define SB2_RBUS_CMP_RESULT_reg                                                      0xB801A66C
#define set_SB2_RBUS_CMP_RESULT_reg(data)   (*((volatile unsigned int*) SB2_RBUS_CMP_RESULT_reg)=data)
#define get_SB2_RBUS_CMP_RESULT_reg   (*((volatile unsigned int*) SB2_RBUS_CMP_RESULT_reg))
#define SB2_RBUS_CMP_RESULT_inst_adr                                                 "0x009B"
#define SB2_RBUS_CMP_RESULT_inst                                                     0x009B
#define SB2_RBUS_CMP_RESULT_crc_cmp_result_shift                                     (0)
#define SB2_RBUS_CMP_RESULT_crc_cmp_result_mask                                      (0xFFFFFFFF)
#define SB2_RBUS_CMP_RESULT_crc_cmp_result(data)                                     (0xFFFFFFFF&((data)<<0))
#define SB2_RBUS_CMP_RESULT_crc_cmp_result_src(data)                                 ((0xFFFFFFFF&(data))>>0)
#define SB2_RBUS_CMP_RESULT_get_crc_cmp_result(data)                                 ((0xFFFFFFFF&(data))>>0)


#define SB2_SCPU_DBG                                                                 0x1801A670
#define SB2_SCPU_DBG_reg_addr                                                        "0xB801A670"
#define SB2_SCPU_DBG_reg                                                             0xB801A670
#define set_SB2_SCPU_DBG_reg(data)   (*((volatile unsigned int*) SB2_SCPU_DBG_reg)=data)
#define get_SB2_SCPU_DBG_reg   (*((volatile unsigned int*) SB2_SCPU_DBG_reg))
#define SB2_SCPU_DBG_inst_adr                                                        "0x009C"
#define SB2_SCPU_DBG_inst                                                            0x009C
#define SB2_SCPU_DBG_write_enable3_shift                                             (13)
#define SB2_SCPU_DBG_write_enable3_mask                                              (0x00002000)
#define SB2_SCPU_DBG_write_enable3(data)                                             (0x00002000&((data)<<13))
#define SB2_SCPU_DBG_write_enable3_src(data)                                         ((0x00002000&(data))>>13)
#define SB2_SCPU_DBG_get_write_enable3(data)                                         ((0x00002000&(data))>>13)
#define SB2_SCPU_DBG_sel1_shift                                                      (8)
#define SB2_SCPU_DBG_sel1_mask                                                       (0x00001F00)
#define SB2_SCPU_DBG_sel1(data)                                                      (0x00001F00&((data)<<8))
#define SB2_SCPU_DBG_sel1_src(data)                                                  ((0x00001F00&(data))>>8)
#define SB2_SCPU_DBG_get_sel1(data)                                                  ((0x00001F00&(data))>>8)
#define SB2_SCPU_DBG_write_enable2_shift                                             (7)
#define SB2_SCPU_DBG_write_enable2_mask                                              (0x00000080)
#define SB2_SCPU_DBG_write_enable2(data)                                             (0x00000080&((data)<<7))
#define SB2_SCPU_DBG_write_enable2_src(data)                                         ((0x00000080&(data))>>7)
#define SB2_SCPU_DBG_get_write_enable2(data)                                         ((0x00000080&(data))>>7)
#define SB2_SCPU_DBG_sel0_shift                                                      (2)
#define SB2_SCPU_DBG_sel0_mask                                                       (0x0000007C)
#define SB2_SCPU_DBG_sel0(data)                                                      (0x0000007C&((data)<<2))
#define SB2_SCPU_DBG_sel0_src(data)                                                  ((0x0000007C&(data))>>2)
#define SB2_SCPU_DBG_get_sel0(data)                                                  ((0x0000007C&(data))>>2)
#define SB2_SCPU_DBG_write_enable1_shift                                             (1)
#define SB2_SCPU_DBG_write_enable1_mask                                              (0x00000002)
#define SB2_SCPU_DBG_write_enable1(data)                                             (0x00000002&((data)<<1))
#define SB2_SCPU_DBG_write_enable1_src(data)                                         ((0x00000002&(data))>>1)
#define SB2_SCPU_DBG_get_write_enable1(data)                                         ((0x00000002&(data))>>1)
#define SB2_SCPU_DBG_enable_shift                                                    (0)
#define SB2_SCPU_DBG_enable_mask                                                     (0x00000001)
#define SB2_SCPU_DBG_enable(data)                                                    (0x00000001&((data)<<0))
#define SB2_SCPU_DBG_enable_src(data)                                                ((0x00000001&(data))>>0)
#define SB2_SCPU_DBG_get_enable(data)                                                ((0x00000001&(data))>>0)


#define SB2_SFC_OPCODE                                                               0x1801A800
#define SB2_SFC_OPCODE_reg_addr                                                      "0xB801A800"
#define SB2_SFC_OPCODE_reg                                                           0xB801A800
#define set_SB2_SFC_OPCODE_reg(data)   (*((volatile unsigned int*) SB2_SFC_OPCODE_reg)=data)
#define get_SB2_SFC_OPCODE_reg   (*((volatile unsigned int*) SB2_SFC_OPCODE_reg))
#define SB2_SFC_OPCODE_inst_adr                                                      "0x0000"
#define SB2_SFC_OPCODE_inst                                                          0x0000
#define SB2_SFC_OPCODE_adr4byte_en_shift                                             (11)
#define SB2_SFC_OPCODE_adr4byte_en_mask                                              (0x00000800)
#define SB2_SFC_OPCODE_adr4byte_en(data)                                             (0x00000800&((data)<<11))
#define SB2_SFC_OPCODE_adr4byte_en_src(data)                                         ((0x00000800&(data))>>11)
#define SB2_SFC_OPCODE_get_adr4byte_en(data)                                         ((0x00000800&(data))>>11)
#define SB2_SFC_OPCODE_dual_mode_shift                                               (9)
#define SB2_SFC_OPCODE_dual_mode_mask                                                (0x00000600)
#define SB2_SFC_OPCODE_dual_mode(data)                                               (0x00000600&((data)<<9))
#define SB2_SFC_OPCODE_dual_mode_src(data)                                           ((0x00000600&(data))>>9)
#define SB2_SFC_OPCODE_get_dual_mode(data)                                           ((0x00000600&(data))>>9)
#define SB2_SFC_OPCODE_fat_mode_shift                                                (8)
#define SB2_SFC_OPCODE_fat_mode_mask                                                 (0x00000100)
#define SB2_SFC_OPCODE_fat_mode(data)                                                (0x00000100&((data)<<8))
#define SB2_SFC_OPCODE_fat_mode_src(data)                                            ((0x00000100&(data))>>8)
#define SB2_SFC_OPCODE_get_fat_mode(data)                                            ((0x00000100&(data))>>8)
#define SB2_SFC_OPCODE_opc_shift                                                     (0)
#define SB2_SFC_OPCODE_opc_mask                                                      (0x000000FF)
#define SB2_SFC_OPCODE_opc(data)                                                     (0x000000FF&((data)<<0))
#define SB2_SFC_OPCODE_opc_src(data)                                                 ((0x000000FF&(data))>>0)
#define SB2_SFC_OPCODE_get_opc(data)                                                 ((0x000000FF&(data))>>0)


#define SB2_SFC_CTL                                                                  0x1801A804
#define SB2_SFC_CTL_reg_addr                                                         "0xB801A804"
#define SB2_SFC_CTL_reg                                                              0xB801A804
#define set_SB2_SFC_CTL_reg(data)   (*((volatile unsigned int*) SB2_SFC_CTL_reg)=data)
#define get_SB2_SFC_CTL_reg   (*((volatile unsigned int*) SB2_SFC_CTL_reg))
#define SB2_SFC_CTL_inst_adr                                                         "0x0001"
#define SB2_SFC_CTL_inst                                                             0x0001
#define SB2_SFC_CTL_dataen_shift                                                     (4)
#define SB2_SFC_CTL_dataen_mask                                                      (0x00000010)
#define SB2_SFC_CTL_dataen(data)                                                     (0x00000010&((data)<<4))
#define SB2_SFC_CTL_dataen_src(data)                                                 ((0x00000010&(data))>>4)
#define SB2_SFC_CTL_get_dataen(data)                                                 ((0x00000010&(data))>>4)
#define SB2_SFC_CTL_adren_shift                                                      (3)
#define SB2_SFC_CTL_adren_mask                                                       (0x00000008)
#define SB2_SFC_CTL_adren(data)                                                      (0x00000008&((data)<<3))
#define SB2_SFC_CTL_adren_src(data)                                                  ((0x00000008&(data))>>3)
#define SB2_SFC_CTL_get_adren(data)                                                  ((0x00000008&(data))>>3)
#define SB2_SFC_CTL_dmycnt_shift                                                     (0)
#define SB2_SFC_CTL_dmycnt_mask                                                      (0x00000007)
#define SB2_SFC_CTL_dmycnt(data)                                                     (0x00000007&((data)<<0))
#define SB2_SFC_CTL_dmycnt_src(data)                                                 ((0x00000007&(data))>>0)
#define SB2_SFC_CTL_get_dmycnt(data)                                                 ((0x00000007&(data))>>0)


#define SB2_SFC_SCK                                                                  0x1801A808
#define SB2_SFC_SCK_reg_addr                                                         "0xB801A808"
#define SB2_SFC_SCK_reg                                                              0xB801A808
#define set_SB2_SFC_SCK_reg(data)   (*((volatile unsigned int*) SB2_SFC_SCK_reg)=data)
#define get_SB2_SFC_SCK_reg   (*((volatile unsigned int*) SB2_SFC_SCK_reg))
#define SB2_SFC_SCK_inst_adr                                                         "0x0002"
#define SB2_SFC_SCK_inst                                                             0x0002
#define SB2_SFC_SCK_mode_shift                                                       (8)
#define SB2_SFC_SCK_mode_mask                                                        (0x00000100)
#define SB2_SFC_SCK_mode(data)                                                       (0x00000100&((data)<<8))
#define SB2_SFC_SCK_mode_src(data)                                                   ((0x00000100&(data))>>8)
#define SB2_SFC_SCK_get_mode(data)                                                   ((0x00000100&(data))>>8)
#define SB2_SFC_SCK_fdiv_shift                                                       (0)
#define SB2_SFC_SCK_fdiv_mask                                                        (0x000000FF)
#define SB2_SFC_SCK_fdiv(data)                                                       (0x000000FF&((data)<<0))
#define SB2_SFC_SCK_fdiv_src(data)                                                   ((0x000000FF&(data))>>0)
#define SB2_SFC_SCK_get_fdiv(data)                                                   ((0x000000FF&(data))>>0)


#define SB2_SFC_CE                                                                   0x1801A80C
#define SB2_SFC_CE_reg_addr                                                          "0xB801A80C"
#define SB2_SFC_CE_reg                                                               0xB801A80C
#define set_SB2_SFC_CE_reg(data)   (*((volatile unsigned int*) SB2_SFC_CE_reg)=data)
#define get_SB2_SFC_CE_reg   (*((volatile unsigned int*) SB2_SFC_CE_reg))
#define SB2_SFC_CE_inst_adr                                                          "0x0003"
#define SB2_SFC_CE_inst                                                              0x0003
#define SB2_SFC_CE_tdt_shift                                                         (16)
#define SB2_SFC_CE_tdt_mask                                                          (0xFFFF0000)
#define SB2_SFC_CE_tdt(data)                                                         (0xFFFF0000&((data)<<16))
#define SB2_SFC_CE_tdt_src(data)                                                     ((0xFFFF0000&(data))>>16)
#define SB2_SFC_CE_get_tdt(data)                                                     ((0xFFFF0000&(data))>>16)
#define SB2_SFC_CE_Phcnt_shift                                                       (8)
#define SB2_SFC_CE_Phcnt_mask                                                        (0x0000FF00)
#define SB2_SFC_CE_Phcnt(data)                                                       (0x0000FF00&((data)<<8))
#define SB2_SFC_CE_Phcnt_src(data)                                                   ((0x0000FF00&(data))>>8)
#define SB2_SFC_CE_get_Phcnt(data)                                                   ((0x0000FF00&(data))>>8)
#define SB2_SFC_CE_Plcnt_shift                                                       (0)
#define SB2_SFC_CE_Plcnt_mask                                                        (0x000000FF)
#define SB2_SFC_CE_Plcnt(data)                                                       (0x000000FF&((data)<<0))
#define SB2_SFC_CE_Plcnt_src(data)                                                   ((0x000000FF&(data))>>0)
#define SB2_SFC_CE_get_Plcnt(data)                                                   ((0x000000FF&(data))>>0)


#define SB2_SFC_WP                                                                   0x1801A810
#define SB2_SFC_WP_reg_addr                                                          "0xB801A810"
#define SB2_SFC_WP_reg                                                               0xB801A810
#define set_SB2_SFC_WP_reg(data)   (*((volatile unsigned int*) SB2_SFC_WP_reg)=data)
#define get_SB2_SFC_WP_reg   (*((volatile unsigned int*) SB2_SFC_WP_reg))
#define SB2_SFC_WP_inst_adr                                                          "0x0004"
#define SB2_SFC_WP_inst                                                              0x0004
#define SB2_SFC_WP_write_en5_shift                                                   (27)
#define SB2_SFC_WP_write_en5_mask                                                    (0x08000000)
#define SB2_SFC_WP_write_en5(data)                                                   (0x08000000&((data)<<27))
#define SB2_SFC_WP_write_en5_src(data)                                               ((0x08000000&(data))>>27)
#define SB2_SFC_WP_get_write_en5(data)                                               ((0x08000000&(data))>>27)
#define SB2_SFC_WP_rdy_shift                                                         (26)
#define SB2_SFC_WP_rdy_mask                                                          (0x04000000)
#define SB2_SFC_WP_rdy(data)                                                         (0x04000000&((data)<<26))
#define SB2_SFC_WP_rdy_src(data)                                                     ((0x04000000&(data))>>26)
#define SB2_SFC_WP_get_rdy(data)                                                     ((0x04000000&(data))>>26)
#define SB2_SFC_WP_write_en1_shift                                                   (1)
#define SB2_SFC_WP_write_en1_mask                                                    (0x00000002)
#define SB2_SFC_WP_write_en1(data)                                                   (0x00000002&((data)<<1))
#define SB2_SFC_WP_write_en1_src(data)                                               ((0x00000002&(data))>>1)
#define SB2_SFC_WP_get_write_en1(data)                                               ((0x00000002&(data))>>1)
#define SB2_SFC_WP_wp_n_shift                                                        (0)
#define SB2_SFC_WP_wp_n_mask                                                         (0x00000001)
#define SB2_SFC_WP_wp_n(data)                                                        (0x00000001&((data)<<0))
#define SB2_SFC_WP_wp_n_src(data)                                                    ((0x00000001&(data))>>0)
#define SB2_SFC_WP_get_wp_n(data)                                                    ((0x00000001&(data))>>0)


#define SB2_SFC_POS_LATCH                                                            0x1801A814
#define SB2_SFC_POS_LATCH_reg_addr                                                   "0xB801A814"
#define SB2_SFC_POS_LATCH_reg                                                        0xB801A814
#define set_SB2_SFC_POS_LATCH_reg(data)   (*((volatile unsigned int*) SB2_SFC_POS_LATCH_reg)=data)
#define get_SB2_SFC_POS_LATCH_reg   (*((volatile unsigned int*) SB2_SFC_POS_LATCH_reg))
#define SB2_SFC_POS_LATCH_inst_adr                                                   "0x0005"
#define SB2_SFC_POS_LATCH_inst                                                       0x0005
#define SB2_SFC_POS_LATCH_pos_latch_shift                                            (0)
#define SB2_SFC_POS_LATCH_pos_latch_mask                                             (0x00000001)
#define SB2_SFC_POS_LATCH_pos_latch(data)                                            (0x00000001&((data)<<0))
#define SB2_SFC_POS_LATCH_pos_latch_src(data)                                        ((0x00000001&(data))>>0)
#define SB2_SFC_POS_LATCH_get_pos_latch(data)                                        ((0x00000001&(data))>>0)


#define SB2_SFC_WAIT_WR                                                              0x1801A818
#define SB2_SFC_WAIT_WR_reg_addr                                                     "0xB801A818"
#define SB2_SFC_WAIT_WR_reg                                                          0xB801A818
#define set_SB2_SFC_WAIT_WR_reg(data)   (*((volatile unsigned int*) SB2_SFC_WAIT_WR_reg)=data)
#define get_SB2_SFC_WAIT_WR_reg   (*((volatile unsigned int*) SB2_SFC_WAIT_WR_reg))
#define SB2_SFC_WAIT_WR_inst_adr                                                     "0x0006"
#define SB2_SFC_WAIT_WR_inst                                                         0x0006
#define SB2_SFC_WAIT_WR_wt_prog_done_shift                                           (8)
#define SB2_SFC_WAIT_WR_wt_prog_done_mask                                            (0x00000100)
#define SB2_SFC_WAIT_WR_wt_prog_done(data)                                           (0x00000100&((data)<<8))
#define SB2_SFC_WAIT_WR_wt_prog_done_src(data)                                       ((0x00000100&(data))>>8)
#define SB2_SFC_WAIT_WR_get_wt_prog_done(data)                                       ((0x00000100&(data))>>8)
#define SB2_SFC_WAIT_WR_rdsr_op_shift                                                (0)
#define SB2_SFC_WAIT_WR_rdsr_op_mask                                                 (0x000000FF)
#define SB2_SFC_WAIT_WR_rdsr_op(data)                                                (0x000000FF&((data)<<0))
#define SB2_SFC_WAIT_WR_rdsr_op_src(data)                                            ((0x000000FF&(data))>>0)
#define SB2_SFC_WAIT_WR_get_rdsr_op(data)                                            ((0x000000FF&(data))>>0)


#define SB2_SFC_EN_WR                                                                0x1801A81C
#define SB2_SFC_EN_WR_reg_addr                                                       "0xB801A81C"
#define SB2_SFC_EN_WR_reg                                                            0xB801A81C
#define set_SB2_SFC_EN_WR_reg(data)   (*((volatile unsigned int*) SB2_SFC_EN_WR_reg)=data)
#define get_SB2_SFC_EN_WR_reg   (*((volatile unsigned int*) SB2_SFC_EN_WR_reg))
#define SB2_SFC_EN_WR_inst_adr                                                       "0x0007"
#define SB2_SFC_EN_WR_inst                                                           0x0007
#define SB2_SFC_EN_WR_wt_prog_en_shift                                               (8)
#define SB2_SFC_EN_WR_wt_prog_en_mask                                                (0x00000100)
#define SB2_SFC_EN_WR_wt_prog_en(data)                                               (0x00000100&((data)<<8))
#define SB2_SFC_EN_WR_wt_prog_en_src(data)                                           ((0x00000100&(data))>>8)
#define SB2_SFC_EN_WR_get_wt_prog_en(data)                                           ((0x00000100&(data))>>8)
#define SB2_SFC_EN_WR_wr_en_op_shift                                                 (0)
#define SB2_SFC_EN_WR_wr_en_op_mask                                                  (0x000000FF)
#define SB2_SFC_EN_WR_wr_en_op(data)                                                 (0x000000FF&((data)<<0))
#define SB2_SFC_EN_WR_wr_en_op_src(data)                                             ((0x000000FF&(data))>>0)
#define SB2_SFC_EN_WR_get_wr_en_op(data)                                             ((0x000000FF&(data))>>0)


#define SB2_SFC_FAST_RD                                                              0x1801A820
#define SB2_SFC_FAST_RD_reg_addr                                                     "0xB801A820"
#define SB2_SFC_FAST_RD_reg                                                          0xB801A820
#define set_SB2_SFC_FAST_RD_reg(data)   (*((volatile unsigned int*) SB2_SFC_FAST_RD_reg)=data)
#define get_SB2_SFC_FAST_RD_reg   (*((volatile unsigned int*) SB2_SFC_FAST_RD_reg))
#define SB2_SFC_FAST_RD_inst_adr                                                     "0x0008"
#define SB2_SFC_FAST_RD_inst                                                         0x0008
#define SB2_SFC_FAST_RD_sf_dclk_sel_shift                                            (4)
#define SB2_SFC_FAST_RD_sf_dclk_sel_mask                                             (0x00000010)
#define SB2_SFC_FAST_RD_sf_dclk_sel(data)                                            (0x00000010&((data)<<4))
#define SB2_SFC_FAST_RD_sf_dclk_sel_src(data)                                        ((0x00000010&(data))>>4)
#define SB2_SFC_FAST_RD_get_sf_dclk_sel(data)                                        ((0x00000010&(data))>>4)
#define SB2_SFC_FAST_RD_fifo_write_tap_shift                                         (1)
#define SB2_SFC_FAST_RD_fifo_write_tap_mask                                          (0x0000000E)
#define SB2_SFC_FAST_RD_fifo_write_tap(data)                                         (0x0000000E&((data)<<1))
#define SB2_SFC_FAST_RD_fifo_write_tap_src(data)                                     ((0x0000000E&(data))>>1)
#define SB2_SFC_FAST_RD_get_fifo_write_tap(data)                                     ((0x0000000E&(data))>>1)
#define SB2_SFC_FAST_RD_fast_read_en_shift                                           (0)
#define SB2_SFC_FAST_RD_fast_read_en_mask                                            (0x00000001)
#define SB2_SFC_FAST_RD_fast_read_en(data)                                           (0x00000001&((data)<<0))
#define SB2_SFC_FAST_RD_fast_read_en_src(data)                                       ((0x00000001&(data))>>0)
#define SB2_SFC_FAST_RD_get_fast_read_en(data)                                       ((0x00000001&(data))>>0)


#define SB2_SFC_SCK_TAP                                                              0x1801A824
#define SB2_SFC_SCK_TAP_reg_addr                                                     "0xB801A824"
#define SB2_SFC_SCK_TAP_reg                                                          0xB801A824
#define set_SB2_SFC_SCK_TAP_reg(data)   (*((volatile unsigned int*) SB2_SFC_SCK_TAP_reg)=data)
#define get_SB2_SFC_SCK_TAP_reg   (*((volatile unsigned int*) SB2_SFC_SCK_TAP_reg))
#define SB2_SFC_SCK_TAP_inst_adr                                                     "0x0009"
#define SB2_SFC_SCK_TAP_inst                                                         0x0009
#define SB2_SFC_SCK_TAP_delay_clk_tap_shift                                          (0)
#define SB2_SFC_SCK_TAP_delay_clk_tap_mask                                           (0xFFFFFFFF)
#define SB2_SFC_SCK_TAP_delay_clk_tap(data)                                          (0xFFFFFFFF&((data)<<0))
#define SB2_SFC_SCK_TAP_delay_clk_tap_src(data)                                      ((0xFFFFFFFF&(data))>>0)
#define SB2_SFC_SCK_TAP_get_delay_clk_tap(data)                                      ((0xFFFFFFFF&(data))>>0)


#endif
