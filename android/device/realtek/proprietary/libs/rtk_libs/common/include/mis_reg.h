/**************************************************************
// Spec Version                  : 0.0.8
// Parser Version                : DVR_Parser_6.0
// CModelGen Version             : 5.0 2009.04.09
// Naming Rule                   : Module_Register_Name
// Parse Option                  : Only Parse _op1
// Parse Address Region          : All Address Region 
// Firmware Header Generate Date : 2010/5/5 17:27:10
***************************************************************/


#ifndef _MIS_REG_H_INCLUDED_
#define _MIS_REG_H_INCLUDED_
#ifdef  _MIS_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     gspi_int:1;
unsigned int     reserved_1:6;
unsigned int     gpioda_int:1;
unsigned int     gpioa_int:1;
unsigned int     reserved_2:6;
unsigned int     rtc_date_int:1;
unsigned int     rtc_hour_int:1;
unsigned int     rtc_min_int:1;
unsigned int     rtc_hsec_int:1;
unsigned int     tc2_int:1;
unsigned int     tc1_int:1;
unsigned int     tc0_int:1;
unsigned int     reserved_3:3;
unsigned int     wdog_nmi_int:1;
unsigned int     ur1_to_int:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     gspi_int:1;
unsigned int     i2c1_int:1;
unsigned int     sc1_int:1;
unsigned int     sc0_int:1;
unsigned int     reserved_1:2;
unsigned int     pcmcia_int:1;
unsigned int     gpioda_int:1;
unsigned int     gpioa_int:1;
unsigned int     reserved_2:6;
unsigned int     rtc_date_int:1;
unsigned int     rtc_hour_int:1;
unsigned int     rtc_min_int:1;
unsigned int     rtc_hsec_int:1;
unsigned int     tc2_int:1;
unsigned int     tc1_int:1;
unsigned int     tc0_int:1;
unsigned int     reserved_3:1;
unsigned int     i2c0_int:1;
unsigned int     ur1_int:1;
unsigned int     reserved_4:1;
unsigned int     ur1_to_int:1;
unsigned int     write_data:1;
}MIS_ISR;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     write_enable3:1;
unsigned int     sel1:4;
unsigned int     write_enable2:1;
unsigned int     sel0:4;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}MIS_DBG;

typedef struct 
{
unsigned int     write_enable4:1;
unsigned int     rvd4:7;
unsigned int     write_enable3:1;
unsigned int     rvd3:7;
unsigned int     write_enable2:1;
unsigned int     rvd2:7;
unsigned int     write_enable1:1;
unsigned int     rvd1:7;
}MIS_DUMMY;

typedef struct 
{
unsigned int     int30:1;
unsigned int     int29:1;
unsigned int     int28:1;
unsigned int     int27:1;
unsigned int     int26:1;
unsigned int     int25:1;
unsigned int     int24:1;
unsigned int     int23:1;
unsigned int     int22:1;
unsigned int     int21:1;
unsigned int     int20:1;
unsigned int     int19:1;
unsigned int     int18:1;
unsigned int     int17:1;
unsigned int     int16:1;
unsigned int     int15:1;
unsigned int     int14:1;
unsigned int     int13:1;
unsigned int     int12:1;
unsigned int     int11:1;
unsigned int     int10:1;
unsigned int     int9:1;
unsigned int     int8:1;
unsigned int     int7:1;
unsigned int     int6:1;
unsigned int     int5:1;
unsigned int     int4:1;
unsigned int     int3:1;
unsigned int     int2:1;
unsigned int     int1:1;
unsigned int     int0:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP0A;

typedef struct 
{
unsigned int     int61:1;
unsigned int     int60:1;
unsigned int     int59:1;
unsigned int     int58:1;
unsigned int     int57:1;
unsigned int     int56:1;
unsigned int     int55:1;
unsigned int     int54:1;
unsigned int     int53:1;
unsigned int     int52:1;
unsigned int     int51:1;
unsigned int     int50:1;
unsigned int     int49:1;
unsigned int     int48:1;
unsigned int     int47:1;
unsigned int     int46:1;
unsigned int     int45:1;
unsigned int     int44:1;
unsigned int     int43:1;
unsigned int     int42:1;
unsigned int     int41:1;
unsigned int     int40:1;
unsigned int     int39:1;
unsigned int     int38:1;
unsigned int     int37:1;
unsigned int     int36:1;
unsigned int     int35:1;
unsigned int     int34:1;
unsigned int     int33:1;
unsigned int     int32:1;
unsigned int     int31:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP1A;

typedef struct 
{
unsigned int     int92:1;
unsigned int     int91:1;
unsigned int     int90:1;
unsigned int     int89:1;
unsigned int     int88:1;
unsigned int     int87:1;
unsigned int     int86:1;
unsigned int     int85:1;
unsigned int     int84:1;
unsigned int     int83:1;
unsigned int     int82:1;
unsigned int     int81:1;
unsigned int     int80:1;
unsigned int     int79:1;
unsigned int     int78:1;
unsigned int     int77:1;
unsigned int     int76:1;
unsigned int     int75:1;
unsigned int     int74:1;
unsigned int     int73:1;
unsigned int     int72:1;
unsigned int     int71:1;
unsigned int     int70:1;
unsigned int     int69:1;
unsigned int     int68:1;
unsigned int     int67:1;
unsigned int     int66:1;
unsigned int     int65:1;
unsigned int     int64:1;
unsigned int     int63:1;
unsigned int     int62:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP2A;

typedef struct 
{
unsigned int     int123:1;
unsigned int     int122:1;
unsigned int     int121:1;
unsigned int     int120:1;
unsigned int     int119:1;
unsigned int     int118:1;
unsigned int     int117:1;
unsigned int     int116:1;
unsigned int     int115:1;
unsigned int     int114:1;
unsigned int     int113:1;
unsigned int     int112:1;
unsigned int     int111:1;
unsigned int     int110:1;
unsigned int     int109:1;
unsigned int     int108:1;
unsigned int     int107:1;
unsigned int     int106:1;
unsigned int     int105:1;
unsigned int     int104:1;
unsigned int     int103:1;
unsigned int     int102:1;
unsigned int     int101:1;
unsigned int     int100:1;
unsigned int     int99:1;
unsigned int     int98:1;
unsigned int     int97:1;
unsigned int     int96:1;
unsigned int     int95:1;
unsigned int     int94:1;
unsigned int     int93:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP3A;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     int127:1;
unsigned int     int126:1;
unsigned int     int125:1;
unsigned int     int124:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP4A;

typedef struct 
{
unsigned int     int30:1;
unsigned int     int29:1;
unsigned int     int28:1;
unsigned int     int27:1;
unsigned int     int26:1;
unsigned int     int25:1;
unsigned int     int24:1;
unsigned int     int23:1;
unsigned int     int22:1;
unsigned int     int21:1;
unsigned int     int20:1;
unsigned int     int19:1;
unsigned int     int18:1;
unsigned int     int17:1;
unsigned int     int16:1;
unsigned int     int15:1;
unsigned int     int14:1;
unsigned int     int13:1;
unsigned int     int12:1;
unsigned int     int11:1;
unsigned int     int10:1;
unsigned int     int9:1;
unsigned int     int8:1;
unsigned int     int7:1;
unsigned int     int6:1;
unsigned int     int5:1;
unsigned int     int4:1;
unsigned int     int3:1;
unsigned int     int2:1;
unsigned int     int1:1;
unsigned int     int0:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP0DA;

typedef struct 
{
unsigned int     int61:1;
unsigned int     int60:1;
unsigned int     int59:1;
unsigned int     int58:1;
unsigned int     int57:1;
unsigned int     int56:1;
unsigned int     int55:1;
unsigned int     int54:1;
unsigned int     int53:1;
unsigned int     int52:1;
unsigned int     int51:1;
unsigned int     int50:1;
unsigned int     int49:1;
unsigned int     int48:1;
unsigned int     int47:1;
unsigned int     int46:1;
unsigned int     int45:1;
unsigned int     int44:1;
unsigned int     int43:1;
unsigned int     int42:1;
unsigned int     int41:1;
unsigned int     int40:1;
unsigned int     int39:1;
unsigned int     int38:1;
unsigned int     int37:1;
unsigned int     int36:1;
unsigned int     int35:1;
unsigned int     int34:1;
unsigned int     int33:1;
unsigned int     int32:1;
unsigned int     int31:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP1DA;

typedef struct 
{
unsigned int     int92:1;
unsigned int     int91:1;
unsigned int     int90:1;
unsigned int     int89:1;
unsigned int     int88:1;
unsigned int     int87:1;
unsigned int     int86:1;
unsigned int     int85:1;
unsigned int     int84:1;
unsigned int     int83:1;
unsigned int     int82:1;
unsigned int     int81:1;
unsigned int     int80:1;
unsigned int     int79:1;
unsigned int     int78:1;
unsigned int     int77:1;
unsigned int     int76:1;
unsigned int     int75:1;
unsigned int     int74:1;
unsigned int     int73:1;
unsigned int     int72:1;
unsigned int     int71:1;
unsigned int     int70:1;
unsigned int     int69:1;
unsigned int     int68:1;
unsigned int     int67:1;
unsigned int     int66:1;
unsigned int     int65:1;
unsigned int     int64:1;
unsigned int     int63:1;
unsigned int     int62:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP2DA;

typedef struct 
{
unsigned int     int123:1;
unsigned int     int122:1;
unsigned int     int121:1;
unsigned int     int120:1;
unsigned int     int119:1;
unsigned int     int118:1;
unsigned int     int117:1;
unsigned int     int116:1;
unsigned int     int115:1;
unsigned int     int114:1;
unsigned int     int113:1;
unsigned int     int112:1;
unsigned int     int111:1;
unsigned int     int110:1;
unsigned int     int109:1;
unsigned int     int108:1;
unsigned int     int107:1;
unsigned int     int106:1;
unsigned int     int105:1;
unsigned int     int104:1;
unsigned int     int103:1;
unsigned int     int102:1;
unsigned int     int101:1;
unsigned int     int100:1;
unsigned int     int99:1;
unsigned int     int98:1;
unsigned int     int97:1;
unsigned int     int96:1;
unsigned int     int95:1;
unsigned int     int94:1;
unsigned int     int93:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP3DA;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     int127:1;
unsigned int     int126:1;
unsigned int     int125:1;
unsigned int     int124:1;
unsigned int     write_data:1;
}MIS_UMSK_ISR_GP4DA;

typedef struct 
{
unsigned int     write_enable2:1;
unsigned int     rvd2:15;
unsigned int     write_enable1:1;
unsigned int     rvd1:15;
}MIS_DUMMY1;

typedef struct 
{
unsigned int     rvd3:4;
unsigned int     to_len:8;
unsigned int     to_int_en:1;
unsigned int     toauareq:1;
unsigned int     rvd1:1;
unsigned int     mdifen:1;
unsigned int     reserved_0:2;
unsigned int     txempthr:6;
unsigned int     reserved_1:2;
unsigned int     rxfulthr:6;
}MIS_UR_CTRL;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     rx_wrd_count:5;
unsigned int     reserved_1:3;
unsigned int     tx_wrd_count:5;
}MIS_UR_STS;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     gspi:1;
unsigned int     i2c1:1;
unsigned int     sc1:1;
unsigned int     sc0:1;
unsigned int     reserved_1:2;
unsigned int     pcmcia:1;
unsigned int     gpioda:1;
unsigned int     gpioa:1;
unsigned int     reserved_2:6;
unsigned int     rtc_date:1;
unsigned int     rtc_hour:1;
unsigned int     rtc_min:1;
unsigned int     rtc_hsec:1;
unsigned int     tc2:1;
unsigned int     tc1:1;
unsigned int     tc0:1;
unsigned int     reserved_3:1;
unsigned int     i2c0:1;
unsigned int     ur1:1;
unsigned int     reserved_4:1;
unsigned int     ur1_to:1;
unsigned int     reserved_5:1;
}MIS_SCPU_INT_EN;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     gspi:1;
unsigned int     i2c1:1;
unsigned int     sc1:1;
unsigned int     sc0:1;
unsigned int     reserved_1:2;
unsigned int     pcmcia:1;
unsigned int     gpioda:1;
unsigned int     gpioa:1;
unsigned int     reserved_2:6;
unsigned int     rtc_date:1;
unsigned int     rtc_hour:1;
unsigned int     rtc_min:1;
unsigned int     rtc_hsec:1;
unsigned int     tc2:1;
unsigned int     tc1:1;
unsigned int     tc0:1;
unsigned int     reserved_3:1;
unsigned int     i2c0:1;
unsigned int     ur1:1;
unsigned int     reserved_4:1;
unsigned int     ur1_to:1;
unsigned int     reserved_5:1;
}MIS_KCPU_INT_EN;

typedef struct 
{
unsigned int     dmy1:20;
unsigned int     reserved_0:3;
unsigned int     en:1;
unsigned int     reserved_1:3;
unsigned int     sel:5;
}MIS_I2C0_SDA_DEL;

typedef struct 
{
unsigned int     dmy1:20;
unsigned int     reserved_0:3;
unsigned int     en:1;
unsigned int     reserved_1:3;
unsigned int     sel:5;
}MIS_I2C1_SDA_DEL;

typedef struct 
{
unsigned int     gpdir:32;
}MIS_GP0DIR;

typedef struct 
{
unsigned int     gpdir:32;
}MIS_GP1DIR;

typedef struct 
{
unsigned int     gpdir:32;
}MIS_GP2DIR;

typedef struct 
{
unsigned int     gpdir:32;
}MIS_GP3DIR;

typedef struct 
{
unsigned int     gpdato:32;
}MIS_GP0DATO;

typedef struct 
{
unsigned int     gpdato:32;
}MIS_GP1DATO;

typedef struct 
{
unsigned int     gpdato:32;
}MIS_GP2DATO;

typedef struct 
{
unsigned int     gpdato:32;
}MIS_GP3DATO;

typedef struct 
{
unsigned int     gpdati:32;
}MIS_GP0DATI;

typedef struct 
{
unsigned int     gpdati:32;
}MIS_GP1DATI;

typedef struct 
{
unsigned int     gpdati:32;
}MIS_GP2DATI;

typedef struct 
{
unsigned int     gpdati:32;
}MIS_GP3DATI;

typedef struct 
{
unsigned int     gp:32;
}MIS_GP0IE;

typedef struct 
{
unsigned int     gp:32;
}MIS_GP1IE;

typedef struct 
{
unsigned int     gp:32;
}MIS_GP2IE;

typedef struct 
{
unsigned int     gp:32;
}MIS_GP3IE;

typedef struct 
{
unsigned int     gpha:32;
}MIS_GP0DP;

typedef struct 
{
unsigned int     gpha:32;
}MIS_GP1DP;

typedef struct 
{
unsigned int     gpha:32;
}MIS_GP2DP;

typedef struct 
{
unsigned int     gpha:32;
}MIS_GP3DP;

typedef struct 
{
unsigned int     write_enable8:1;
unsigned int     clk8:3;
unsigned int     write_enable7:1;
unsigned int     clk7:3;
unsigned int     write_enable6:1;
unsigned int     clk6:3;
unsigned int     write_enable5:1;
unsigned int     clk5:3;
unsigned int     write_enable4:1;
unsigned int     clk4:3;
unsigned int     write_enable3:1;
unsigned int     clk3:3;
unsigned int     write_enable2:1;
unsigned int     clk2:3;
unsigned int     write_enable1:1;
unsigned int     clk1:3;
}MIS_GPDEB;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     dll:8;
}MIS_U1RBR_THR_DLL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     dlh:8;
}MIS_U1IER_DLH;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     fifo16:2;
unsigned int     reserved_1:2;
unsigned int     iid:4;
}MIS_U1IIR_FCR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     dlab:1;
unsigned int     brk:1;
unsigned int     reserved_1:1;
unsigned int     eps:1;
unsigned int     pen:1;
unsigned int     stb:1;
unsigned int     wls:2;
}MIS_U1LCR;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     afce:1;
unsigned int     loop:1;
unsigned int     reserved_1:2;
unsigned int     rts:1;
unsigned int     dtr:1;
}MIS_U1MCR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rfe:1;
unsigned int     temt:1;
unsigned int     thre:1;
unsigned int     bi:1;
unsigned int     fe:1;
unsigned int     pe:1;
unsigned int     oe:1;
unsigned int     dr:1;
}MIS_U1LSR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     dcd:1;
unsigned int     ri:1;
unsigned int     dsr:1;
unsigned int     cts:1;
unsigned int     ddcd:1;
unsigned int     teri:1;
unsigned int     ddsr:1;
unsigned int     dcts:1;
}MIS_U1MSR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     scr:8;
}MIS_U1SCR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rbd:8;
}MIS_U1SRBR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     far:8;
}MIS_U1FAR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     far:8;
}MIS_U1TFR;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     rffe:1;
unsigned int     rfpf:1;
unsigned int     rfwd:8;
}MIS_U1RFW;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     rff:1;
unsigned int     rfne:1;
unsigned int     tfe:1;
unsigned int     tfnf:1;
unsigned int     busy:1;
}MIS_U1USR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tfl:8;
}MIS_U1TFL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rfl:8;
}MIS_U1RFL;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     xfr:1;
unsigned int     rfr:1;
unsigned int     ur:1;
}MIS_U1SRR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     sbcr:1;
}MIS_U1SBCR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     sdmam:1;
}MIS_U1SDMAM;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     sfe:1;
}MIS_U1SFE;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     srt:2;
}MIS_U1SRT;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     stet:2;
}MIS_U1STET;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     htx:1;
}MIS_U1HTX;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     dmasa:1;
}MIS_U1DMASA;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     fifo_mode:8;
unsigned int     reserved_1:2;
unsigned int     dma_extra:1;
unsigned int     uart_add_encoded_params:1;
unsigned int     shadow:1;
unsigned int     fifo_stat:1;
unsigned int     fifo_access:1;
unsigned int     additional_feat:1;
unsigned int     sir_lp_mode:1;
unsigned int     sir_mode:1;
unsigned int     thre_mode:1;
unsigned int     afce_mode:1;
unsigned int     reserved_2:2;
unsigned int     apb_data_width:2;
}MIS_U1CPR;

typedef struct 
{
unsigned int     ucv:32;
}MIS_U1UCV;

typedef struct 
{
unsigned int     ctr:32;
}MIS_U1CTR;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     ic_slave_disable:1;
unsigned int     ic_restart_en:1;
unsigned int     ic_10bitaddr_master:1;
unsigned int     ic_10bitaddr_slave:1;
unsigned int     speed:2;
unsigned int     master_mode:1;
}MIS_IC_CON;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     ic_10bitaddr_master:1;
unsigned int     special:1;
unsigned int     gc_or_start:1;
unsigned int     ic_tar:10;
}MIS_IC_TAR;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     ic_sar:10;
}MIS_IC_SAR;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     ic_hs_mar:3;
}MIS_IC_HS_MADDR;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cmd:1;
unsigned int     dat:8;
}MIS_IC_DATA_CMD;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_ss_scl_hcnt:16;
}MIS_IC_SS_SCL_HCNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_ss_scl_lcnt:16;
}MIS_IC_SS_SCL_LCNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_fs_scl_hcnt:16;
}MIS_IC_FS_SCL_HCNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_fs_scl_lcnt:16;
}MIS_IC_FS_SCL_LCNT;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     r_gen_call:1;
unsigned int     r_start_det:1;
unsigned int     r_stop_det:1;
unsigned int     r_activity:1;
unsigned int     r_rx_done:1;
unsigned int     r_tx_abrt:1;
unsigned int     r_rd_req:1;
unsigned int     r_tx_empty:1;
unsigned int     r_tx_over:1;
unsigned int     r_rx_full:1;
unsigned int     r_rx_over:1;
unsigned int     r_rx_under:1;
}MIS_IC_INTR_STAT;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m_gen_call:1;
unsigned int     m_start_det:1;
unsigned int     m_stop_det:1;
unsigned int     m_activity:1;
unsigned int     m_rx_done:1;
unsigned int     m_tx_abrt:1;
unsigned int     m_rd_req:1;
unsigned int     m_tx_empty:1;
unsigned int     m_tx_over:1;
unsigned int     m_rx_full:1;
unsigned int     m_rx_over:1;
unsigned int     m_rx_under:1;
}MIS_IC_INTR_MASK;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     gen_call:1;
unsigned int     start_det:1;
unsigned int     stop_det:1;
unsigned int     activity:1;
unsigned int     rx_done:1;
unsigned int     tx_abrt:1;
unsigned int     rd_req:1;
unsigned int     tx_empty:1;
unsigned int     tx_over:1;
unsigned int     rx_full:1;
unsigned int     rx_over:1;
unsigned int     rx_under:1;
}MIS_IC_RAW_INTR_STAT;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rx_tl:8;
}MIS_IC_RX_TL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tx_tl:8;
}MIS_IC_TX_TL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_intr:1;
}MIS_IC_CLR_INTR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rx_under:1;
}MIS_IC_CLR_RX_UNDER;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rx_over:1;
}MIS_IC_CLR_RX_OVER;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_tx_over:1;
}MIS_IC_CLR_TX_OVER;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rd_req:1;
}MIS_IC_CLR_RD_REQ;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_tx_abrt:1;
}MIS_IC_CLR_TX_ABRT;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rx_done:1;
}MIS_IC_CLR_RX_DONE;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_activity:1;
}MIS_IC_CLR_ACTIVITY;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_stop_det:1;
}MIS_IC_CLR_STOP_DET;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_start_det:1;
}MIS_IC_CLR_START_DET;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_gen_call:1;
}MIS_IC_CLR_GEN_CALL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     enable:1;
}MIS_IC_ENABLE;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     slv_activity:1;
unsigned int     mst_activity:1;
unsigned int     rff:1;
unsigned int     rfne:1;
unsigned int     tfe:1;
unsigned int     tfnf:1;
unsigned int     activity:1;
}MIS_IC_STATUS;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     txflr:4;
}MIS_IC_TXFLR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     rxflr:4;
}MIS_IC_RXFLR;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     abrt_slvrd_intx:1;
unsigned int     abrt_slv_arblost:1;
unsigned int     abrt_slvflush_txfifo:1;
unsigned int     arb_lost:1;
unsigned int     arb_master_dis:1;
unsigned int     abrt_10b_rd_norstrt:1;
unsigned int     abrt_sbyte_norstrt:1;
unsigned int     abrt_hs_norstrt:1;
unsigned int     abrt_sbyte_ackdet:1;
unsigned int     abrt_hs_ackdet:1;
unsigned int     abrt_gcall_read:1;
unsigned int     abrt_gcall_noack:1;
unsigned int     abrt_txdata_noack:1;
unsigned int     abrt_10addr2_noack:1;
unsigned int     abrt_10addr1_noack:1;
unsigned int     abrt_7b_addr_noack:1;
}MIS_IC_TX_ABRT_SOURCE;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     nack:1;
}MIS_IC_SLV_DATA_NACK_ONLY;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     tdmae:1;
unsigned int     rdmae:1;
}MIS_IC_DMA_CR;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     dmatdl:3;
}MIS_IC_DMA_TDLR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     dmardl:4;
}MIS_IC_DMA_RDLR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     sda_setup:8;
}MIS_IC_SDA_SETUP;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     ack_gen_call:1;
}MIS_IC_ACK_GENERAL_CALL;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     slv_rx_data_lost:1;
unsigned int     slv_disabled_whi:1;
unsigned int     ic_en:1;
}MIS_IC_ENABLE_STATUS;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     tx_buffer_depth:8;
unsigned int     rx_buffer_depth:8;
unsigned int     add_encoded_params:1;
unsigned int     has_dma:1;
unsigned int     intr_io:1;
unsigned int     hc_count_values:1;
unsigned int     max_speed_mode:2;
unsigned int     apb_data_width:2;
}MIS_IC_COMP_PARAM_1;

typedef struct 
{
unsigned int     ic_comp_version:32;
}MIS_IC_COMP_VERSION;

typedef struct 
{
unsigned int     ic_comp_type:32;
}MIS_IC_COMP_TYPE;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     ic_slave_disable:1;
unsigned int     ic_restart_en:1;
unsigned int     ic_10bitaddr_master:1;
unsigned int     ic_10bitaddr_slave:1;
unsigned int     speed:2;
unsigned int     master_mode:1;
}MIS_IC1_CON;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     special:1;
unsigned int     gc_or_start:1;
unsigned int     ic_tar:10;
}MIS_IC1_TAR;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     ic_sar:10;
}MIS_IC1_SAR;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     ic_hs_mar:3;
}MIS_IC1_HS_MADDR;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cmd:1;
unsigned int     dat:8;
}MIS_IC1_DATA_CMD;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_ss_scl_hcnt:16;
}MIS_IC1_SS_SCL_HCNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_ss_scl_lcnt:16;
}MIS_IC1_SS_SCL_LCNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_fs_scl_hcnt:16;
}MIS_IC1_FS_SCL_HCNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ic_fs_scl_lcnt:16;
}MIS_IC1_FS_SCL_LCNT;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     gen_call:1;
unsigned int     start_det:1;
unsigned int     stop_det:1;
unsigned int     activity:1;
unsigned int     rx_done:1;
unsigned int     tx_abrt:1;
unsigned int     rd_req:1;
unsigned int     tx_empty:1;
unsigned int     tx_over:1;
unsigned int     rx_full:1;
unsigned int     rx_over:1;
unsigned int     rx_under:1;
}MIS_IC1_RAW_INTR_STAT;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m_gen_call:1;
unsigned int     m_start_det:1;
unsigned int     m_stop_det:1;
unsigned int     m_activity:1;
unsigned int     m_rx_done:1;
unsigned int     m_tx_abrt:1;
unsigned int     m_rd_req:1;
unsigned int     m_tx_empty:1;
unsigned int     m_tx_over:1;
unsigned int     m_rx_full:1;
unsigned int     m_rx_over:1;
unsigned int     m_rx_under:1;
}MIS_IC1_INTR_MASK;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     r_gen_call:1;
unsigned int     r_start_det:1;
unsigned int     r_stop_det:1;
unsigned int     r_activity:1;
unsigned int     r_rx_done:1;
unsigned int     r_tx_abrt:1;
unsigned int     r_rd_req:1;
unsigned int     r_tx_empty:1;
unsigned int     r_tx_over:1;
unsigned int     r_rx_full:1;
unsigned int     r_rx_over:1;
unsigned int     r_rx_under:1;
}MIS_IC1_INTR_STAT;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rx_tl:8;
}MIS_IC1_RX_TL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tx_tl:8;
}MIS_IC1_TX_TL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_intr:1;
}MIS_IC1_CLR_INTR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rx_under:1;
}MIS_IC1_CLR_RX_UNDER;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rx_over:1;
}MIS_IC1_CLR_RX_OVER;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_tx_over:1;
}MIS_IC1_CLR_TX_OVER;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rd_req:1;
}MIS_IC1_CLR_RD_REQ;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_tx_abrt:1;
}MIS_IC1_CLR_TX_ABRT;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_rx_done:1;
}MIS_IC1_CLR_RX_DONE;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_activity:1;
}MIS_IC1_CLR_ACTIVITY;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_stop_det:1;
}MIS_IC1_CLR_STOP_DET;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_start_det:1;
}MIS_IC1_CLR_START_DET;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     clr_gen_call:1;
}MIS_IC1_CLR_GEN_CALL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     enable:1;
}MIS_IC1_ENABLE;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     rff:1;
unsigned int     rfne:1;
unsigned int     tfe:1;
unsigned int     tfnf:1;
unsigned int     activity:1;
}MIS_IC1_STATUS;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     txflr:4;
}MIS_IC1_TXFLR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     rxflr:4;
}MIS_IC1_RXFLR;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     abrt_slvrd_intx:1;
unsigned int     abrt_slv_arblost:1;
unsigned int     abrt_slvflush_txfifo:1;
unsigned int     arb_lost:1;
unsigned int     arb_master_dis:1;
unsigned int     abrt_10b_rd_norstrt:1;
unsigned int     abrt_sbyte_norstrt:1;
unsigned int     abrt_hs_norstrt:1;
unsigned int     abrt_sbyte_ackdet:1;
unsigned int     abrt_hs_ackdet:1;
unsigned int     abrt_gcall_read:1;
unsigned int     abrt_gcall_noack:1;
unsigned int     abrt_txdata_noack:1;
unsigned int     abrt_10addr2_noack:1;
unsigned int     abrt_10addr1_noack:1;
unsigned int     abrt_7b_addr_noack:1;
}MIS_IC1_TX_ABRT_SOURCE;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     tdmae:1;
unsigned int     rdmae:1;
}MIS_IC1_DMA_CR;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     dmatdl:3;
}MIS_IC1_DMA_TDLR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     dmardl:4;
}MIS_IC1_DMA_RDLR;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     tx_buffer_depth:8;
unsigned int     rx_buffer_depth:8;
unsigned int     add_encoded_params:1;
unsigned int     has_dma:1;
unsigned int     intr_io:1;
unsigned int     hc_count_values:1;
unsigned int     max_speed_mode:2;
unsigned int     apb_data_width:2;
}MIS_IC1_COMP_PARAM_1;

typedef struct 
{
unsigned int     ic_comp_version:32;
}MIS_IC1_COMP_VERSION;

typedef struct 
{
unsigned int     ic_comp_type:32;
}MIS_IC1_COMP_TYPE;

typedef struct 
{
unsigned int     3:8;
unsigned int     2:8;
unsigned int     1:8;
unsigned int     0:8;
}MIS_PWM_OCD;

typedef struct 
{
unsigned int     3:8;
unsigned int     2:8;
unsigned int     1:8;
unsigned int     0:8;
}MIS_PWM_CD;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     3:4;
unsigned int     2:4;
unsigned int     1:4;
unsigned int     0:4;
}MIS_PWM_CSD;

typedef struct 
{
unsigned int     tc0tvr:32;
}MIS_TC0TVR;

typedef struct 
{
unsigned int     tc1tvr:32;
}MIS_TC1TVR;

typedef struct 
{
unsigned int     tc2tvr:32;
}MIS_TC2TVR;

typedef struct 
{
unsigned int     tc0cvr:32;
}MIS_TC0CVR;

typedef struct 
{
unsigned int     tc1cvr:32;
}MIS_TC1CVR;

typedef struct 
{
unsigned int     tc2vr:32;
}MIS_TC2CVR;

typedef struct 
{
unsigned int     tc0en:1;
unsigned int     tc0mode:1;
unsigned int     tc0pause:1;
unsigned int     rvda:5;
unsigned int     reserved_0:24;
}MIS_TC0CR;

typedef struct 
{
unsigned int     tc1en:1;
unsigned int     tc1mode:1;
unsigned int     tc1pause:1;
unsigned int     rvda:5;
unsigned int     reserved_0:24;
}MIS_TC1CR;

typedef struct 
{
unsigned int     tc2en:1;
unsigned int     tc2mode:1;
unsigned int     tc2pause:1;
unsigned int     rvda:5;
unsigned int     reserved_0:24;
}MIS_TC2CR;

typedef struct 
{
unsigned int     tc0ie:1;
unsigned int     reserved_0:31;
}MIS_TC0ICR;

typedef struct 
{
unsigned int     tc1ie:1;
unsigned int     reserved_0:31;
}MIS_TC1ICR;

typedef struct 
{
unsigned int     tc2ie:1;
unsigned int     reserved_0:31;
}MIS_TC2ICR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     en:1;
}MIS_CLK90K_CTRL;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_SCPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_SCPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_SCPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_ACPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_ACPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_ACPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_VCPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_VCPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_VCPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_KCPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_KCPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_KCPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     en:1;
}MIS_PCR_CLK90K_CTRL;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_PCR_SCPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_PCR_SCPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_PCR_SCPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_PCR_ACPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_PCR_ACPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_PCR_ACPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_PCR_VCPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_PCR_VCPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_PCR_VCPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}MIS_PCR_KCPU_CLK27M_90K;

typedef struct 
{
unsigned int     val:32;
}MIS_PCR_KCPU_CLK90K_LO;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     val:16;
}MIS_PCR_KCPU_CLK90K_HI;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     nmic:4;
unsigned int     wdc:4;
unsigned int     wden:8;
}MIS_TCWCR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     wdclr:1;
}MIS_TCWTR;

typedef struct 
{
unsigned int     sel:32;
}MIS_TCWNMI;

typedef struct 
{
unsigned int     sel:32;
}MIS_TCWOV;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     rtcsec:7;
}MIS_RTCSEC;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     rtcmin:6;
}MIS_RTCMIN;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     rtchr:5;
}MIS_RTCHR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rtcdate1:8;
}MIS_RTCDATE1;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     rtcdate2:7;
}MIS_RTCDATE2;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     almmin:6;
}MIS_ALMMIN;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     almhr:5;
}MIS_ALMHR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     almdate1:8;
}MIS_ALMDATE1;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     almdate2:7;
}MIS_ALMDATE2;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     vref:2;
unsigned int     rtcstop:1;
}MIS_RTCSTOP;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rtcpwr:1;
unsigned int     co:2;
unsigned int     ci:2;
unsigned int     clksel:1;
unsigned int     bc:2;
}MIS_RTCACR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rtcen:8;
}MIS_RTCEN;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     rtcrst:1;
unsigned int     reserved_1:2;
unsigned int     dainte:1;
unsigned int     huinte:1;
unsigned int     muinte:1;
unsigned int     hsuinte:1;
}MIS_RTCCR;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     ct:1;
unsigned int     at:1;
unsigned int     pa:15;
unsigned int     df:8;
}MIS_PCMCIA_CMDFF;

typedef struct 
{
unsigned int     psr:1;
unsigned int     ce1_card1:1;
unsigned int     ce1_card2:1;
unsigned int     ce2_card1:1;
unsigned int     ce2_card2:1;
unsigned int     pcr1:1;
unsigned int     pcr2:1;
unsigned int     pcr1_oe:1;
unsigned int     pcr2_oe:1;
unsigned int     reserved_0:15;
unsigned int     piie1:1;
unsigned int     pciie1:1;
unsigned int     pcrie1:1;
unsigned int     piie2:1;
unsigned int     pciie2:1;
unsigned int     pcrie2:1;
unsigned int     afie:1;
unsigned int     apfie:1;
}MIS_PCMCIA_CR;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     pres1:1;
unsigned int     pres2:1;
unsigned int     pii1:1;
unsigned int     pcii1:1;
unsigned int     pcri1:1;
unsigned int     pii2:1;
unsigned int     pcii2:1;
unsigned int     pcri2:1;
unsigned int     afi:1;
unsigned int     apfi:1;
}MIS_PCMCIA_STS;

typedef struct 
{
unsigned int     twe:6;
unsigned int     thd:4;
unsigned int     taoe:5;
unsigned int     reserved_0:4;
unsigned int     thce:5;
unsigned int     reserved_1:3;
unsigned int     tsu:5;
}MIS_PCMCIA_AMTC;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     tdiord:5;
unsigned int     reserved_1:3;
unsigned int     tsuio:5;
unsigned int     tdinpack:4;
unsigned int     tdwt:4;
}MIS_PCMCIA_IOMTC;

typedef struct 
{
unsigned int     tc:8;
unsigned int     thdio:4;
unsigned int     reserved_0:3;
unsigned int     tcio:9;
unsigned int     reserved_1:2;
unsigned int     twiowr:6;
}MIS_PCMCIA_MATC;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     oe:15;
}MIS_PCMCIA_ACTRL;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     clk_en:1;
unsigned int     sc_clkdiv:6;
unsigned int     bauddiv2:2;
unsigned int     bauddiv1:8;
unsigned int     pre_clkdiv:8;
}MIS_SC0_FP;

typedef struct 
{
unsigned int     fifo_rst:1;
unsigned int     rst:1;
unsigned int     scen:1;
unsigned int     tx_go:1;
unsigned int     auto_atr:1;
unsigned int     conv:1;
unsigned int     clk_stop:1;
unsigned int     ps:1;
unsigned int     reserved_0:24;
}MIS_SC0_CR;

typedef struct 
{
unsigned int     txgrdt:8;
unsigned int     cwi:4;
unsigned int     bwi:4;
unsigned int     wwi:4;
unsigned int     bgt:5;
unsigned int     edc_en:1;
unsigned int     crc:1;
unsigned int     protocol_t:1;
unsigned int     t0rty:1;
unsigned int     t0rty_cnt:3;
}MIS_SC0_PCR;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     tx_fifo_full:1;
unsigned int     dat:8;
}MIS_SC0_TXFIFO;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     dat:8;
}MIS_SC0_RXFIFO;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rxlen:8;
}MIS_SC0_RXLENR;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     rxflow:1;
unsigned int     flow_en:1;
}MIS_SC0_FCR;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     pres:1;
unsigned int     cpres_int:1;
unsigned int     tx_flow_int:1;
unsigned int     txp_int:1;
unsigned int     txdone_int:1;
unsigned int     txempty_int:1;
unsigned int     edcerr_int:1;
unsigned int     rx_fover:1;
unsigned int     rxp_int:1;
unsigned int     atrs_int:1;
unsigned int     bgt_int:1;
unsigned int     cwt_int:1;
unsigned int     rlen_int:1;
unsigned int     wwt_int:1;
unsigned int     bwt_int:1;
unsigned int     rcv_int:1;
unsigned int     drdy_int:1;
}MIS_SC0_IRSR;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     cpres_en:1;
unsigned int     txflow_int_en:1;
unsigned int     txp_en:1;
unsigned int     txdone_en:1;
unsigned int     txempty_en:1;
unsigned int     edcerr_en:1;
unsigned int     rx_fover_en:1;
unsigned int     rxp_en:1;
unsigned int     atrs_en:1;
unsigned int     bgt_en:1;
unsigned int     cwt_en:1;
unsigned int     rlen_en:1;
unsigned int     wwt_en:1;
unsigned int     bwt_en:1;
unsigned int     rcv_en:1;
unsigned int     drdy_en:1;
}MIS_SC0_IRER;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     clk_en:1;
unsigned int     sc_clkdiv:6;
unsigned int     bauddiv2:2;
unsigned int     bauddiv1:8;
unsigned int     pre_clkdiv:8;
}MIS_SC1_FP;

typedef struct 
{
unsigned int     fifo_rst:1;
unsigned int     rst:1;
unsigned int     scen:1;
unsigned int     tx_go:1;
unsigned int     auto_atr:1;
unsigned int     conv:1;
unsigned int     clk_stop:1;
unsigned int     ps:1;
unsigned int     reserved_0:24;
}MIS_SC1_CR;

typedef struct 
{
unsigned int     txgrdt:8;
unsigned int     cwi:4;
unsigned int     bwi:4;
unsigned int     wwi:4;
unsigned int     bgt:5;
unsigned int     edc_en:1;
unsigned int     crc:1;
unsigned int     protocol_t:1;
unsigned int     t0rty:1;
unsigned int     t0rty_cnt:3;
}MIS_SC1_PCR;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     tx_fifo_full:1;
unsigned int     dat:8;
}MIS_SC1_TXFIFO;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     dat:8;
}MIS_SC1_RXFIFO;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rxlen:8;
}MIS_SC1_RXLENR;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     rxflow:1;
unsigned int     flow_en:1;
}MIS_SC1_FCR;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     pres:1;
unsigned int     cpres_int:1;
unsigned int     tx_flow_int:1;
unsigned int     txp_int:1;
unsigned int     txdone_int:1;
unsigned int     txempty_int:1;
unsigned int     edcerr_int:1;
unsigned int     rx_fover:1;
unsigned int     rxp_int:1;
unsigned int     atrs_int:1;
unsigned int     bgt_int:1;
unsigned int     cwt_int:1;
unsigned int     rlen_int:1;
unsigned int     wwt_int:1;
unsigned int     bwt_int:1;
unsigned int     rcv_int:1;
unsigned int     drdy_int:1;
}MIS_SC1_IRSR;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     cpres_en:1;
unsigned int     txflow_int_en:1;
unsigned int     txp_en:1;
unsigned int     txdone_en:1;
unsigned int     txempty_en:1;
unsigned int     edcerr_en:1;
unsigned int     rx_fover_en:1;
unsigned int     rxp_en:1;
unsigned int     atrs_en:1;
unsigned int     bgt_en:1;
unsigned int     cwt_en:1;
unsigned int     rlen_en:1;
unsigned int     wwt_en:1;
unsigned int     bwt_en:1;
unsigned int     rcv_en:1;
unsigned int     drdy_en:1;
}MIS_SC1_IRER;

typedef struct 
{
unsigned int     datao:32;
}MIS_EF_DATA;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     write_enable4:1;
unsigned int     en_pro_efuse:1;
unsigned int     write_enable3:1;
unsigned int     auto_rd_req:1;
unsigned int     reserved_1:1;
unsigned int     write_enable2:1;
unsigned int     tmrf:2;
unsigned int     write_enable1:1;
unsigned int     program:1;
unsigned int     read:1;
unsigned int     enb:1;
unsigned int     b:4;
unsigned int     a:8;
}MIS_EF_CTRL;

typedef struct 
{
unsigned int     comp_data:32;
}MIS_EJ_COM_DATA;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     ej_com_ok:1;
unsigned int     ej_com_en:1;
}MIS_EJ_COM;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     pro_efuse_act:1;
unsigned int     reserved_1:15;
unsigned int     crc_en:1;
}MIS_EF_CTRL_ST;

typedef struct 
{
unsigned int     crc_ini:16;
unsigned int     crc_out:16;
}MIS_EF_CRC;

#endif

#define MIS_UMSK_ISR                                                                 0x1801B008
#define MIS_UMSK_ISR_reg_addr                                                        "0xB801B008"
#define MIS_UMSK_ISR_reg                                                             0xB801B008
#define set_MIS_UMSK_ISR_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_reg)=data)
#define get_MIS_UMSK_ISR_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_reg))
#define MIS_UMSK_ISR_inst_adr                                                        "0x0002"
#define MIS_UMSK_ISR_inst                                                            0x0002
#define MIS_UMSK_ISR_gspi_int_shift                                                  (27)
#define MIS_UMSK_ISR_gspi_int_mask                                                   (0x08000000)
#define MIS_UMSK_ISR_gspi_int(data)                                                  (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_gspi_int_src(data)                                              ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_get_gspi_int(data)                                              ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_gpioda_int_shift                                                (20)
#define MIS_UMSK_ISR_gpioda_int_mask                                                 (0x00100000)
#define MIS_UMSK_ISR_gpioda_int(data)                                                (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_gpioda_int_src(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_get_gpioda_int(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_gpioa_int_shift                                                 (19)
#define MIS_UMSK_ISR_gpioa_int_mask                                                  (0x00080000)
#define MIS_UMSK_ISR_gpioa_int(data)                                                 (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_gpioa_int_src(data)                                             ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_get_gpioa_int(data)                                             ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_rtc_date_int_shift                                              (12)
#define MIS_UMSK_ISR_rtc_date_int_mask                                               (0x00001000)
#define MIS_UMSK_ISR_rtc_date_int(data)                                              (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_rtc_date_int_src(data)                                          ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_get_rtc_date_int(data)                                          ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_rtc_hour_int_shift                                              (11)
#define MIS_UMSK_ISR_rtc_hour_int_mask                                               (0x00000800)
#define MIS_UMSK_ISR_rtc_hour_int(data)                                              (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_rtc_hour_int_src(data)                                          ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_get_rtc_hour_int(data)                                          ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_rtc_min_int_shift                                               (10)
#define MIS_UMSK_ISR_rtc_min_int_mask                                                (0x00000400)
#define MIS_UMSK_ISR_rtc_min_int(data)                                               (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_rtc_min_int_src(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_get_rtc_min_int(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_rtc_hsec_int_shift                                              (9)
#define MIS_UMSK_ISR_rtc_hsec_int_mask                                               (0x00000200)
#define MIS_UMSK_ISR_rtc_hsec_int(data)                                              (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_rtc_hsec_int_src(data)                                          ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_get_rtc_hsec_int(data)                                          ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_tc2_int_shift                                                   (8)
#define MIS_UMSK_ISR_tc2_int_mask                                                    (0x00000100)
#define MIS_UMSK_ISR_tc2_int(data)                                                   (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_tc2_int_src(data)                                               ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_get_tc2_int(data)                                               ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_tc1_int_shift                                                   (7)
#define MIS_UMSK_ISR_tc1_int_mask                                                    (0x00000080)
#define MIS_UMSK_ISR_tc1_int(data)                                                   (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_tc1_int_src(data)                                               ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_get_tc1_int(data)                                               ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_tc0_int_shift                                                   (6)
#define MIS_UMSK_ISR_tc0_int_mask                                                    (0x00000040)
#define MIS_UMSK_ISR_tc0_int(data)                                                   (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_tc0_int_src(data)                                               ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_get_tc0_int(data)                                               ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_wdog_nmi_int_shift                                              (2)
#define MIS_UMSK_ISR_wdog_nmi_int_mask                                               (0x00000004)
#define MIS_UMSK_ISR_wdog_nmi_int(data)                                              (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_wdog_nmi_int_src(data)                                          ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_get_wdog_nmi_int(data)                                          ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_ur1_to_int_shift                                                (1)
#define MIS_UMSK_ISR_ur1_to_int_mask                                                 (0x00000002)
#define MIS_UMSK_ISR_ur1_to_int(data)                                                (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_ur1_to_int_src(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_get_ur1_to_int(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_write_data_shift                                                (0)
#define MIS_UMSK_ISR_write_data_mask                                                 (0x00000001)
#define MIS_UMSK_ISR_write_data(data)                                                (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_write_data_src(data)                                            ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_get_write_data(data)                                            ((0x00000001&(data))>>0)


#define MIS_ISR                                                                      0x1801B00C
#define MIS_ISR_reg_addr                                                             "0xB801B00C"
#define MIS_ISR_reg                                                                  0xB801B00C
#define set_MIS_ISR_reg(data)   (*((volatile unsigned int*) MIS_ISR_reg)=data)
#define get_MIS_ISR_reg   (*((volatile unsigned int*) MIS_ISR_reg))
#define MIS_ISR_inst_adr                                                             "0x0003"
#define MIS_ISR_inst                                                                 0x0003
#define MIS_ISR_gspi_int_shift                                                       (27)
#define MIS_ISR_gspi_int_mask                                                        (0x08000000)
#define MIS_ISR_gspi_int(data)                                                       (0x08000000&((data)<<27))
#define MIS_ISR_gspi_int_src(data)                                                   ((0x08000000&(data))>>27)
#define MIS_ISR_get_gspi_int(data)                                                   ((0x08000000&(data))>>27)
#define MIS_ISR_i2c1_int_shift                                                       (26)
#define MIS_ISR_i2c1_int_mask                                                        (0x04000000)
#define MIS_ISR_i2c1_int(data)                                                       (0x04000000&((data)<<26))
#define MIS_ISR_i2c1_int_src(data)                                                   ((0x04000000&(data))>>26)
#define MIS_ISR_get_i2c1_int(data)                                                   ((0x04000000&(data))>>26)
#define MIS_ISR_sc1_int_shift                                                        (25)
#define MIS_ISR_sc1_int_mask                                                         (0x02000000)
#define MIS_ISR_sc1_int(data)                                                        (0x02000000&((data)<<25))
#define MIS_ISR_sc1_int_src(data)                                                    ((0x02000000&(data))>>25)
#define MIS_ISR_get_sc1_int(data)                                                    ((0x02000000&(data))>>25)
#define MIS_ISR_sc0_int_shift                                                        (24)
#define MIS_ISR_sc0_int_mask                                                         (0x01000000)
#define MIS_ISR_sc0_int(data)                                                        (0x01000000&((data)<<24))
#define MIS_ISR_sc0_int_src(data)                                                    ((0x01000000&(data))>>24)
#define MIS_ISR_get_sc0_int(data)                                                    ((0x01000000&(data))>>24)
#define MIS_ISR_pcmcia_int_shift                                                     (21)
#define MIS_ISR_pcmcia_int_mask                                                      (0x00200000)
#define MIS_ISR_pcmcia_int(data)                                                     (0x00200000&((data)<<21))
#define MIS_ISR_pcmcia_int_src(data)                                                 ((0x00200000&(data))>>21)
#define MIS_ISR_get_pcmcia_int(data)                                                 ((0x00200000&(data))>>21)
#define MIS_ISR_gpioda_int_shift                                                     (20)
#define MIS_ISR_gpioda_int_mask                                                      (0x00100000)
#define MIS_ISR_gpioda_int(data)                                                     (0x00100000&((data)<<20))
#define MIS_ISR_gpioda_int_src(data)                                                 ((0x00100000&(data))>>20)
#define MIS_ISR_get_gpioda_int(data)                                                 ((0x00100000&(data))>>20)
#define MIS_ISR_gpioa_int_shift                                                      (19)
#define MIS_ISR_gpioa_int_mask                                                       (0x00080000)
#define MIS_ISR_gpioa_int(data)                                                      (0x00080000&((data)<<19))
#define MIS_ISR_gpioa_int_src(data)                                                  ((0x00080000&(data))>>19)
#define MIS_ISR_get_gpioa_int(data)                                                  ((0x00080000&(data))>>19)
#define MIS_ISR_rtc_date_int_shift                                                   (12)
#define MIS_ISR_rtc_date_int_mask                                                    (0x00001000)
#define MIS_ISR_rtc_date_int(data)                                                   (0x00001000&((data)<<12))
#define MIS_ISR_rtc_date_int_src(data)                                               ((0x00001000&(data))>>12)
#define MIS_ISR_get_rtc_date_int(data)                                               ((0x00001000&(data))>>12)
#define MIS_ISR_rtc_hour_int_shift                                                   (11)
#define MIS_ISR_rtc_hour_int_mask                                                    (0x00000800)
#define MIS_ISR_rtc_hour_int(data)                                                   (0x00000800&((data)<<11))
#define MIS_ISR_rtc_hour_int_src(data)                                               ((0x00000800&(data))>>11)
#define MIS_ISR_get_rtc_hour_int(data)                                               ((0x00000800&(data))>>11)
#define MIS_ISR_rtc_min_int_shift                                                    (10)
#define MIS_ISR_rtc_min_int_mask                                                     (0x00000400)
#define MIS_ISR_rtc_min_int(data)                                                    (0x00000400&((data)<<10))
#define MIS_ISR_rtc_min_int_src(data)                                                ((0x00000400&(data))>>10)
#define MIS_ISR_get_rtc_min_int(data)                                                ((0x00000400&(data))>>10)
#define MIS_ISR_rtc_hsec_int_shift                                                   (9)
#define MIS_ISR_rtc_hsec_int_mask                                                    (0x00000200)
#define MIS_ISR_rtc_hsec_int(data)                                                   (0x00000200&((data)<<9))
#define MIS_ISR_rtc_hsec_int_src(data)                                               ((0x00000200&(data))>>9)
#define MIS_ISR_get_rtc_hsec_int(data)                                               ((0x00000200&(data))>>9)
#define MIS_ISR_tc2_int_shift                                                        (8)
#define MIS_ISR_tc2_int_mask                                                         (0x00000100)
#define MIS_ISR_tc2_int(data)                                                        (0x00000100&((data)<<8))
#define MIS_ISR_tc2_int_src(data)                                                    ((0x00000100&(data))>>8)
#define MIS_ISR_get_tc2_int(data)                                                    ((0x00000100&(data))>>8)
#define MIS_ISR_tc1_int_shift                                                        (7)
#define MIS_ISR_tc1_int_mask                                                         (0x00000080)
#define MIS_ISR_tc1_int(data)                                                        (0x00000080&((data)<<7))
#define MIS_ISR_tc1_int_src(data)                                                    ((0x00000080&(data))>>7)
#define MIS_ISR_get_tc1_int(data)                                                    ((0x00000080&(data))>>7)
#define MIS_ISR_tc0_int_shift                                                        (6)
#define MIS_ISR_tc0_int_mask                                                         (0x00000040)
#define MIS_ISR_tc0_int(data)                                                        (0x00000040&((data)<<6))
#define MIS_ISR_tc0_int_src(data)                                                    ((0x00000040&(data))>>6)
#define MIS_ISR_get_tc0_int(data)                                                    ((0x00000040&(data))>>6)
#define MIS_ISR_i2c0_int_shift                                                       (4)
#define MIS_ISR_i2c0_int_mask                                                        (0x00000010)
#define MIS_ISR_i2c0_int(data)                                                       (0x00000010&((data)<<4))
#define MIS_ISR_i2c0_int_src(data)                                                   ((0x00000010&(data))>>4)
#define MIS_ISR_get_i2c0_int(data)                                                   ((0x00000010&(data))>>4)
#define MIS_ISR_ur1_int_shift                                                        (3)
#define MIS_ISR_ur1_int_mask                                                         (0x00000008)
#define MIS_ISR_ur1_int(data)                                                        (0x00000008&((data)<<3))
#define MIS_ISR_ur1_int_src(data)                                                    ((0x00000008&(data))>>3)
#define MIS_ISR_get_ur1_int(data)                                                    ((0x00000008&(data))>>3)
#define MIS_ISR_ur1_to_int_shift                                                     (1)
#define MIS_ISR_ur1_to_int_mask                                                      (0x00000002)
#define MIS_ISR_ur1_to_int(data)                                                     (0x00000002&((data)<<1))
#define MIS_ISR_ur1_to_int_src(data)                                                 ((0x00000002&(data))>>1)
#define MIS_ISR_get_ur1_to_int(data)                                                 ((0x00000002&(data))>>1)
#define MIS_ISR_write_data_shift                                                     (0)
#define MIS_ISR_write_data_mask                                                      (0x00000001)
#define MIS_ISR_write_data(data)                                                     (0x00000001&((data)<<0))
#define MIS_ISR_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define MIS_ISR_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define MIS_DBG                                                                      0x1801B02C
#define MIS_DBG_reg_addr                                                             "0xB801B02C"
#define MIS_DBG_reg                                                                  0xB801B02C
#define set_MIS_DBG_reg(data)   (*((volatile unsigned int*) MIS_DBG_reg)=data)
#define get_MIS_DBG_reg   (*((volatile unsigned int*) MIS_DBG_reg))
#define MIS_DBG_inst_adr                                                             "0x000B"
#define MIS_DBG_inst                                                                 0x000B
#define MIS_DBG_write_enable3_shift                                                  (11)
#define MIS_DBG_write_enable3_mask                                                   (0x00000800)
#define MIS_DBG_write_enable3(data)                                                  (0x00000800&((data)<<11))
#define MIS_DBG_write_enable3_src(data)                                              ((0x00000800&(data))>>11)
#define MIS_DBG_get_write_enable3(data)                                              ((0x00000800&(data))>>11)
#define MIS_DBG_sel1_shift                                                           (7)
#define MIS_DBG_sel1_mask                                                            (0x00000780)
#define MIS_DBG_sel1(data)                                                           (0x00000780&((data)<<7))
#define MIS_DBG_sel1_src(data)                                                       ((0x00000780&(data))>>7)
#define MIS_DBG_get_sel1(data)                                                       ((0x00000780&(data))>>7)
#define MIS_DBG_write_enable2_shift                                                  (6)
#define MIS_DBG_write_enable2_mask                                                   (0x00000040)
#define MIS_DBG_write_enable2(data)                                                  (0x00000040&((data)<<6))
#define MIS_DBG_write_enable2_src(data)                                              ((0x00000040&(data))>>6)
#define MIS_DBG_get_write_enable2(data)                                              ((0x00000040&(data))>>6)
#define MIS_DBG_sel0_shift                                                           (2)
#define MIS_DBG_sel0_mask                                                            (0x0000003C)
#define MIS_DBG_sel0(data)                                                           (0x0000003C&((data)<<2))
#define MIS_DBG_sel0_src(data)                                                       ((0x0000003C&(data))>>2)
#define MIS_DBG_get_sel0(data)                                                       ((0x0000003C&(data))>>2)
#define MIS_DBG_write_enable1_shift                                                  (1)
#define MIS_DBG_write_enable1_mask                                                   (0x00000002)
#define MIS_DBG_write_enable1(data)                                                  (0x00000002&((data)<<1))
#define MIS_DBG_write_enable1_src(data)                                              ((0x00000002&(data))>>1)
#define MIS_DBG_get_write_enable1(data)                                              ((0x00000002&(data))>>1)
#define MIS_DBG_enable_shift                                                         (0)
#define MIS_DBG_enable_mask                                                          (0x00000001)
#define MIS_DBG_enable(data)                                                         (0x00000001&((data)<<0))
#define MIS_DBG_enable_src(data)                                                     ((0x00000001&(data))>>0)
#define MIS_DBG_get_enable(data)                                                     ((0x00000001&(data))>>0)


#define MIS_DUMMY                                                                    0x1801B030
#define MIS_DUMMY_reg_addr                                                           "0xB801B030"
#define MIS_DUMMY_reg                                                                0xB801B030
#define set_MIS_DUMMY_reg(data)   (*((volatile unsigned int*) MIS_DUMMY_reg)=data)
#define get_MIS_DUMMY_reg   (*((volatile unsigned int*) MIS_DUMMY_reg))
#define MIS_DUMMY_inst_adr                                                           "0x000C"
#define MIS_DUMMY_inst                                                               0x000C
#define MIS_DUMMY_write_enable4_shift                                                (31)
#define MIS_DUMMY_write_enable4_mask                                                 (0x80000000)
#define MIS_DUMMY_write_enable4(data)                                                (0x80000000&((data)<<31))
#define MIS_DUMMY_write_enable4_src(data)                                            ((0x80000000&(data))>>31)
#define MIS_DUMMY_get_write_enable4(data)                                            ((0x80000000&(data))>>31)
#define MIS_DUMMY_rvd4_shift                                                         (24)
#define MIS_DUMMY_rvd4_mask                                                          (0x7F000000)
#define MIS_DUMMY_rvd4(data)                                                         (0x7F000000&((data)<<24))
#define MIS_DUMMY_rvd4_src(data)                                                     ((0x7F000000&(data))>>24)
#define MIS_DUMMY_get_rvd4(data)                                                     ((0x7F000000&(data))>>24)
#define MIS_DUMMY_write_enable3_shift                                                (23)
#define MIS_DUMMY_write_enable3_mask                                                 (0x00800000)
#define MIS_DUMMY_write_enable3(data)                                                (0x00800000&((data)<<23))
#define MIS_DUMMY_write_enable3_src(data)                                            ((0x00800000&(data))>>23)
#define MIS_DUMMY_get_write_enable3(data)                                            ((0x00800000&(data))>>23)
#define MIS_DUMMY_rvd3_shift                                                         (16)
#define MIS_DUMMY_rvd3_mask                                                          (0x007F0000)
#define MIS_DUMMY_rvd3(data)                                                         (0x007F0000&((data)<<16))
#define MIS_DUMMY_rvd3_src(data)                                                     ((0x007F0000&(data))>>16)
#define MIS_DUMMY_get_rvd3(data)                                                     ((0x007F0000&(data))>>16)
#define MIS_DUMMY_write_enable2_shift                                                (15)
#define MIS_DUMMY_write_enable2_mask                                                 (0x00008000)
#define MIS_DUMMY_write_enable2(data)                                                (0x00008000&((data)<<15))
#define MIS_DUMMY_write_enable2_src(data)                                            ((0x00008000&(data))>>15)
#define MIS_DUMMY_get_write_enable2(data)                                            ((0x00008000&(data))>>15)
#define MIS_DUMMY_rvd2_shift                                                         (8)
#define MIS_DUMMY_rvd2_mask                                                          (0x00007F00)
#define MIS_DUMMY_rvd2(data)                                                         (0x00007F00&((data)<<8))
#define MIS_DUMMY_rvd2_src(data)                                                     ((0x00007F00&(data))>>8)
#define MIS_DUMMY_get_rvd2(data)                                                     ((0x00007F00&(data))>>8)
#define MIS_DUMMY_write_enable1_shift                                                (7)
#define MIS_DUMMY_write_enable1_mask                                                 (0x00000080)
#define MIS_DUMMY_write_enable1(data)                                                (0x00000080&((data)<<7))
#define MIS_DUMMY_write_enable1_src(data)                                            ((0x00000080&(data))>>7)
#define MIS_DUMMY_get_write_enable1(data)                                            ((0x00000080&(data))>>7)
#define MIS_DUMMY_rvd1_shift                                                         (0)
#define MIS_DUMMY_rvd1_mask                                                          (0x0000007F)
#define MIS_DUMMY_rvd1(data)                                                         (0x0000007F&((data)<<0))
#define MIS_DUMMY_rvd1_src(data)                                                     ((0x0000007F&(data))>>0)
#define MIS_DUMMY_get_rvd1(data)                                                     ((0x0000007F&(data))>>0)


#define MIS_UMSK_ISR_GP0A                                                            0x1801B040
#define MIS_UMSK_ISR_GP0A_reg_addr                                                   "0xB801B040"
#define MIS_UMSK_ISR_GP0A_reg                                                        0xB801B040
#define set_MIS_UMSK_ISR_GP0A_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP0A_reg)=data)
#define get_MIS_UMSK_ISR_GP0A_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP0A_reg))
#define MIS_UMSK_ISR_GP0A_inst_adr                                                   "0x0010"
#define MIS_UMSK_ISR_GP0A_inst                                                       0x0010
#define MIS_UMSK_ISR_GP0A_int30_shift                                                (31)
#define MIS_UMSK_ISR_GP0A_int30_mask                                                 (0x80000000)
#define MIS_UMSK_ISR_GP0A_int30(data)                                                (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP0A_int30_src(data)                                            ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP0A_get_int30(data)                                            ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP0A_int29_shift                                                (30)
#define MIS_UMSK_ISR_GP0A_int29_mask                                                 (0x40000000)
#define MIS_UMSK_ISR_GP0A_int29(data)                                                (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP0A_int29_src(data)                                            ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP0A_get_int29(data)                                            ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP0A_int28_shift                                                (29)
#define MIS_UMSK_ISR_GP0A_int28_mask                                                 (0x20000000)
#define MIS_UMSK_ISR_GP0A_int28(data)                                                (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP0A_int28_src(data)                                            ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP0A_get_int28(data)                                            ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP0A_int27_shift                                                (28)
#define MIS_UMSK_ISR_GP0A_int27_mask                                                 (0x10000000)
#define MIS_UMSK_ISR_GP0A_int27(data)                                                (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP0A_int27_src(data)                                            ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP0A_get_int27(data)                                            ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP0A_int26_shift                                                (27)
#define MIS_UMSK_ISR_GP0A_int26_mask                                                 (0x08000000)
#define MIS_UMSK_ISR_GP0A_int26(data)                                                (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP0A_int26_src(data)                                            ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP0A_get_int26(data)                                            ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP0A_int25_shift                                                (26)
#define MIS_UMSK_ISR_GP0A_int25_mask                                                 (0x04000000)
#define MIS_UMSK_ISR_GP0A_int25(data)                                                (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP0A_int25_src(data)                                            ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP0A_get_int25(data)                                            ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP0A_int24_shift                                                (25)
#define MIS_UMSK_ISR_GP0A_int24_mask                                                 (0x02000000)
#define MIS_UMSK_ISR_GP0A_int24(data)                                                (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP0A_int24_src(data)                                            ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP0A_get_int24(data)                                            ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP0A_int23_shift                                                (24)
#define MIS_UMSK_ISR_GP0A_int23_mask                                                 (0x01000000)
#define MIS_UMSK_ISR_GP0A_int23(data)                                                (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP0A_int23_src(data)                                            ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP0A_get_int23(data)                                            ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP0A_int22_shift                                                (23)
#define MIS_UMSK_ISR_GP0A_int22_mask                                                 (0x00800000)
#define MIS_UMSK_ISR_GP0A_int22(data)                                                (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP0A_int22_src(data)                                            ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP0A_get_int22(data)                                            ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP0A_int21_shift                                                (22)
#define MIS_UMSK_ISR_GP0A_int21_mask                                                 (0x00400000)
#define MIS_UMSK_ISR_GP0A_int21(data)                                                (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP0A_int21_src(data)                                            ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP0A_get_int21(data)                                            ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP0A_int20_shift                                                (21)
#define MIS_UMSK_ISR_GP0A_int20_mask                                                 (0x00200000)
#define MIS_UMSK_ISR_GP0A_int20(data)                                                (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP0A_int20_src(data)                                            ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP0A_get_int20(data)                                            ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP0A_int19_shift                                                (20)
#define MIS_UMSK_ISR_GP0A_int19_mask                                                 (0x00100000)
#define MIS_UMSK_ISR_GP0A_int19(data)                                                (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP0A_int19_src(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP0A_get_int19(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP0A_int18_shift                                                (19)
#define MIS_UMSK_ISR_GP0A_int18_mask                                                 (0x00080000)
#define MIS_UMSK_ISR_GP0A_int18(data)                                                (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP0A_int18_src(data)                                            ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP0A_get_int18(data)                                            ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP0A_int17_shift                                                (18)
#define MIS_UMSK_ISR_GP0A_int17_mask                                                 (0x00040000)
#define MIS_UMSK_ISR_GP0A_int17(data)                                                (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP0A_int17_src(data)                                            ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP0A_get_int17(data)                                            ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP0A_int16_shift                                                (17)
#define MIS_UMSK_ISR_GP0A_int16_mask                                                 (0x00020000)
#define MIS_UMSK_ISR_GP0A_int16(data)                                                (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP0A_int16_src(data)                                            ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP0A_get_int16(data)                                            ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP0A_int15_shift                                                (16)
#define MIS_UMSK_ISR_GP0A_int15_mask                                                 (0x00010000)
#define MIS_UMSK_ISR_GP0A_int15(data)                                                (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP0A_int15_src(data)                                            ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP0A_get_int15(data)                                            ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP0A_int14_shift                                                (15)
#define MIS_UMSK_ISR_GP0A_int14_mask                                                 (0x00008000)
#define MIS_UMSK_ISR_GP0A_int14(data)                                                (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP0A_int14_src(data)                                            ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP0A_get_int14(data)                                            ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP0A_int13_shift                                                (14)
#define MIS_UMSK_ISR_GP0A_int13_mask                                                 (0x00004000)
#define MIS_UMSK_ISR_GP0A_int13(data)                                                (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP0A_int13_src(data)                                            ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP0A_get_int13(data)                                            ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP0A_int12_shift                                                (13)
#define MIS_UMSK_ISR_GP0A_int12_mask                                                 (0x00002000)
#define MIS_UMSK_ISR_GP0A_int12(data)                                                (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP0A_int12_src(data)                                            ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP0A_get_int12(data)                                            ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP0A_int11_shift                                                (12)
#define MIS_UMSK_ISR_GP0A_int11_mask                                                 (0x00001000)
#define MIS_UMSK_ISR_GP0A_int11(data)                                                (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP0A_int11_src(data)                                            ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP0A_get_int11(data)                                            ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP0A_int10_shift                                                (11)
#define MIS_UMSK_ISR_GP0A_int10_mask                                                 (0x00000800)
#define MIS_UMSK_ISR_GP0A_int10(data)                                                (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP0A_int10_src(data)                                            ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP0A_get_int10(data)                                            ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP0A_int9_shift                                                 (10)
#define MIS_UMSK_ISR_GP0A_int9_mask                                                  (0x00000400)
#define MIS_UMSK_ISR_GP0A_int9(data)                                                 (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP0A_int9_src(data)                                             ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP0A_get_int9(data)                                             ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP0A_int8_shift                                                 (9)
#define MIS_UMSK_ISR_GP0A_int8_mask                                                  (0x00000200)
#define MIS_UMSK_ISR_GP0A_int8(data)                                                 (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP0A_int8_src(data)                                             ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP0A_get_int8(data)                                             ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP0A_int7_shift                                                 (8)
#define MIS_UMSK_ISR_GP0A_int7_mask                                                  (0x00000100)
#define MIS_UMSK_ISR_GP0A_int7(data)                                                 (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP0A_int7_src(data)                                             ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP0A_get_int7(data)                                             ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP0A_int6_shift                                                 (7)
#define MIS_UMSK_ISR_GP0A_int6_mask                                                  (0x00000080)
#define MIS_UMSK_ISR_GP0A_int6(data)                                                 (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP0A_int6_src(data)                                             ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP0A_get_int6(data)                                             ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP0A_int5_shift                                                 (6)
#define MIS_UMSK_ISR_GP0A_int5_mask                                                  (0x00000040)
#define MIS_UMSK_ISR_GP0A_int5(data)                                                 (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP0A_int5_src(data)                                             ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP0A_get_int5(data)                                             ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP0A_int4_shift                                                 (5)
#define MIS_UMSK_ISR_GP0A_int4_mask                                                  (0x00000020)
#define MIS_UMSK_ISR_GP0A_int4(data)                                                 (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP0A_int4_src(data)                                             ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP0A_get_int4(data)                                             ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP0A_int3_shift                                                 (4)
#define MIS_UMSK_ISR_GP0A_int3_mask                                                  (0x00000010)
#define MIS_UMSK_ISR_GP0A_int3(data)                                                 (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP0A_int3_src(data)                                             ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP0A_get_int3(data)                                             ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP0A_int2_shift                                                 (3)
#define MIS_UMSK_ISR_GP0A_int2_mask                                                  (0x00000008)
#define MIS_UMSK_ISR_GP0A_int2(data)                                                 (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP0A_int2_src(data)                                             ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP0A_get_int2(data)                                             ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP0A_int1_shift                                                 (2)
#define MIS_UMSK_ISR_GP0A_int1_mask                                                  (0x00000004)
#define MIS_UMSK_ISR_GP0A_int1(data)                                                 (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP0A_int1_src(data)                                             ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP0A_get_int1(data)                                             ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP0A_int0_shift                                                 (1)
#define MIS_UMSK_ISR_GP0A_int0_mask                                                  (0x00000002)
#define MIS_UMSK_ISR_GP0A_int0(data)                                                 (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP0A_int0_src(data)                                             ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP0A_get_int0(data)                                             ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP0A_write_data_shift                                           (0)
#define MIS_UMSK_ISR_GP0A_write_data_mask                                            (0x00000001)
#define MIS_UMSK_ISR_GP0A_write_data(data)                                           (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP0A_write_data_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP0A_get_write_data(data)                                       ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP1A                                                            0x1801B044
#define MIS_UMSK_ISR_GP1A_reg_addr                                                   "0xB801B044"
#define MIS_UMSK_ISR_GP1A_reg                                                        0xB801B044
#define set_MIS_UMSK_ISR_GP1A_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP1A_reg)=data)
#define get_MIS_UMSK_ISR_GP1A_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP1A_reg))
#define MIS_UMSK_ISR_GP1A_inst_adr                                                   "0x0011"
#define MIS_UMSK_ISR_GP1A_inst                                                       0x0011
#define MIS_UMSK_ISR_GP1A_int61_shift                                                (31)
#define MIS_UMSK_ISR_GP1A_int61_mask                                                 (0x80000000)
#define MIS_UMSK_ISR_GP1A_int61(data)                                                (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP1A_int61_src(data)                                            ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP1A_get_int61(data)                                            ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP1A_int60_shift                                                (30)
#define MIS_UMSK_ISR_GP1A_int60_mask                                                 (0x40000000)
#define MIS_UMSK_ISR_GP1A_int60(data)                                                (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP1A_int60_src(data)                                            ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP1A_get_int60(data)                                            ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP1A_int59_shift                                                (29)
#define MIS_UMSK_ISR_GP1A_int59_mask                                                 (0x20000000)
#define MIS_UMSK_ISR_GP1A_int59(data)                                                (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP1A_int59_src(data)                                            ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP1A_get_int59(data)                                            ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP1A_int58_shift                                                (28)
#define MIS_UMSK_ISR_GP1A_int58_mask                                                 (0x10000000)
#define MIS_UMSK_ISR_GP1A_int58(data)                                                (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP1A_int58_src(data)                                            ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP1A_get_int58(data)                                            ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP1A_int57_shift                                                (27)
#define MIS_UMSK_ISR_GP1A_int57_mask                                                 (0x08000000)
#define MIS_UMSK_ISR_GP1A_int57(data)                                                (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP1A_int57_src(data)                                            ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP1A_get_int57(data)                                            ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP1A_int56_shift                                                (26)
#define MIS_UMSK_ISR_GP1A_int56_mask                                                 (0x04000000)
#define MIS_UMSK_ISR_GP1A_int56(data)                                                (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP1A_int56_src(data)                                            ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP1A_get_int56(data)                                            ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP1A_int55_shift                                                (25)
#define MIS_UMSK_ISR_GP1A_int55_mask                                                 (0x02000000)
#define MIS_UMSK_ISR_GP1A_int55(data)                                                (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP1A_int55_src(data)                                            ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP1A_get_int55(data)                                            ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP1A_int54_shift                                                (24)
#define MIS_UMSK_ISR_GP1A_int54_mask                                                 (0x01000000)
#define MIS_UMSK_ISR_GP1A_int54(data)                                                (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP1A_int54_src(data)                                            ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP1A_get_int54(data)                                            ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP1A_int53_shift                                                (23)
#define MIS_UMSK_ISR_GP1A_int53_mask                                                 (0x00800000)
#define MIS_UMSK_ISR_GP1A_int53(data)                                                (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP1A_int53_src(data)                                            ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP1A_get_int53(data)                                            ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP1A_int52_shift                                                (22)
#define MIS_UMSK_ISR_GP1A_int52_mask                                                 (0x00400000)
#define MIS_UMSK_ISR_GP1A_int52(data)                                                (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP1A_int52_src(data)                                            ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP1A_get_int52(data)                                            ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP1A_int51_shift                                                (21)
#define MIS_UMSK_ISR_GP1A_int51_mask                                                 (0x00200000)
#define MIS_UMSK_ISR_GP1A_int51(data)                                                (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP1A_int51_src(data)                                            ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP1A_get_int51(data)                                            ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP1A_int50_shift                                                (20)
#define MIS_UMSK_ISR_GP1A_int50_mask                                                 (0x00100000)
#define MIS_UMSK_ISR_GP1A_int50(data)                                                (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP1A_int50_src(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP1A_get_int50(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP1A_int49_shift                                                (19)
#define MIS_UMSK_ISR_GP1A_int49_mask                                                 (0x00080000)
#define MIS_UMSK_ISR_GP1A_int49(data)                                                (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP1A_int49_src(data)                                            ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP1A_get_int49(data)                                            ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP1A_int48_shift                                                (18)
#define MIS_UMSK_ISR_GP1A_int48_mask                                                 (0x00040000)
#define MIS_UMSK_ISR_GP1A_int48(data)                                                (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP1A_int48_src(data)                                            ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP1A_get_int48(data)                                            ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP1A_int47_shift                                                (17)
#define MIS_UMSK_ISR_GP1A_int47_mask                                                 (0x00020000)
#define MIS_UMSK_ISR_GP1A_int47(data)                                                (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP1A_int47_src(data)                                            ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP1A_get_int47(data)                                            ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP1A_int46_shift                                                (16)
#define MIS_UMSK_ISR_GP1A_int46_mask                                                 (0x00010000)
#define MIS_UMSK_ISR_GP1A_int46(data)                                                (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP1A_int46_src(data)                                            ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP1A_get_int46(data)                                            ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP1A_int45_shift                                                (15)
#define MIS_UMSK_ISR_GP1A_int45_mask                                                 (0x00008000)
#define MIS_UMSK_ISR_GP1A_int45(data)                                                (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP1A_int45_src(data)                                            ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP1A_get_int45(data)                                            ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP1A_int44_shift                                                (14)
#define MIS_UMSK_ISR_GP1A_int44_mask                                                 (0x00004000)
#define MIS_UMSK_ISR_GP1A_int44(data)                                                (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP1A_int44_src(data)                                            ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP1A_get_int44(data)                                            ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP1A_int43_shift                                                (13)
#define MIS_UMSK_ISR_GP1A_int43_mask                                                 (0x00002000)
#define MIS_UMSK_ISR_GP1A_int43(data)                                                (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP1A_int43_src(data)                                            ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP1A_get_int43(data)                                            ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP1A_int42_shift                                                (12)
#define MIS_UMSK_ISR_GP1A_int42_mask                                                 (0x00001000)
#define MIS_UMSK_ISR_GP1A_int42(data)                                                (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP1A_int42_src(data)                                            ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP1A_get_int42(data)                                            ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP1A_int41_shift                                                (11)
#define MIS_UMSK_ISR_GP1A_int41_mask                                                 (0x00000800)
#define MIS_UMSK_ISR_GP1A_int41(data)                                                (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP1A_int41_src(data)                                            ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP1A_get_int41(data)                                            ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP1A_int40_shift                                                (10)
#define MIS_UMSK_ISR_GP1A_int40_mask                                                 (0x00000400)
#define MIS_UMSK_ISR_GP1A_int40(data)                                                (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP1A_int40_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP1A_get_int40(data)                                            ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP1A_int39_shift                                                (9)
#define MIS_UMSK_ISR_GP1A_int39_mask                                                 (0x00000200)
#define MIS_UMSK_ISR_GP1A_int39(data)                                                (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP1A_int39_src(data)                                            ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP1A_get_int39(data)                                            ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP1A_int38_shift                                                (8)
#define MIS_UMSK_ISR_GP1A_int38_mask                                                 (0x00000100)
#define MIS_UMSK_ISR_GP1A_int38(data)                                                (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP1A_int38_src(data)                                            ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP1A_get_int38(data)                                            ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP1A_int37_shift                                                (7)
#define MIS_UMSK_ISR_GP1A_int37_mask                                                 (0x00000080)
#define MIS_UMSK_ISR_GP1A_int37(data)                                                (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP1A_int37_src(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP1A_get_int37(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP1A_int36_shift                                                (6)
#define MIS_UMSK_ISR_GP1A_int36_mask                                                 (0x00000040)
#define MIS_UMSK_ISR_GP1A_int36(data)                                                (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP1A_int36_src(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP1A_get_int36(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP1A_int35_shift                                                (5)
#define MIS_UMSK_ISR_GP1A_int35_mask                                                 (0x00000020)
#define MIS_UMSK_ISR_GP1A_int35(data)                                                (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP1A_int35_src(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP1A_get_int35(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP1A_int34_shift                                                (4)
#define MIS_UMSK_ISR_GP1A_int34_mask                                                 (0x00000010)
#define MIS_UMSK_ISR_GP1A_int34(data)                                                (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP1A_int34_src(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP1A_get_int34(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP1A_int33_shift                                                (3)
#define MIS_UMSK_ISR_GP1A_int33_mask                                                 (0x00000008)
#define MIS_UMSK_ISR_GP1A_int33(data)                                                (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP1A_int33_src(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP1A_get_int33(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP1A_int32_shift                                                (2)
#define MIS_UMSK_ISR_GP1A_int32_mask                                                 (0x00000004)
#define MIS_UMSK_ISR_GP1A_int32(data)                                                (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP1A_int32_src(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP1A_get_int32(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP1A_int31_shift                                                (1)
#define MIS_UMSK_ISR_GP1A_int31_mask                                                 (0x00000002)
#define MIS_UMSK_ISR_GP1A_int31(data)                                                (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP1A_int31_src(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP1A_get_int31(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP1A_write_data_shift                                           (0)
#define MIS_UMSK_ISR_GP1A_write_data_mask                                            (0x00000001)
#define MIS_UMSK_ISR_GP1A_write_data(data)                                           (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP1A_write_data_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP1A_get_write_data(data)                                       ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP2A                                                            0x1801B048
#define MIS_UMSK_ISR_GP2A_reg_addr                                                   "0xB801B048"
#define MIS_UMSK_ISR_GP2A_reg                                                        0xB801B048
#define set_MIS_UMSK_ISR_GP2A_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP2A_reg)=data)
#define get_MIS_UMSK_ISR_GP2A_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP2A_reg))
#define MIS_UMSK_ISR_GP2A_inst_adr                                                   "0x0012"
#define MIS_UMSK_ISR_GP2A_inst                                                       0x0012
#define MIS_UMSK_ISR_GP2A_int92_shift                                                (31)
#define MIS_UMSK_ISR_GP2A_int92_mask                                                 (0x80000000)
#define MIS_UMSK_ISR_GP2A_int92(data)                                                (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP2A_int92_src(data)                                            ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP2A_get_int92(data)                                            ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP2A_int91_shift                                                (30)
#define MIS_UMSK_ISR_GP2A_int91_mask                                                 (0x40000000)
#define MIS_UMSK_ISR_GP2A_int91(data)                                                (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP2A_int91_src(data)                                            ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP2A_get_int91(data)                                            ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP2A_int90_shift                                                (29)
#define MIS_UMSK_ISR_GP2A_int90_mask                                                 (0x20000000)
#define MIS_UMSK_ISR_GP2A_int90(data)                                                (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP2A_int90_src(data)                                            ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP2A_get_int90(data)                                            ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP2A_int89_shift                                                (28)
#define MIS_UMSK_ISR_GP2A_int89_mask                                                 (0x10000000)
#define MIS_UMSK_ISR_GP2A_int89(data)                                                (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP2A_int89_src(data)                                            ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP2A_get_int89(data)                                            ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP2A_int88_shift                                                (27)
#define MIS_UMSK_ISR_GP2A_int88_mask                                                 (0x08000000)
#define MIS_UMSK_ISR_GP2A_int88(data)                                                (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP2A_int88_src(data)                                            ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP2A_get_int88(data)                                            ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP2A_int87_shift                                                (26)
#define MIS_UMSK_ISR_GP2A_int87_mask                                                 (0x04000000)
#define MIS_UMSK_ISR_GP2A_int87(data)                                                (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP2A_int87_src(data)                                            ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP2A_get_int87(data)                                            ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP2A_int86_shift                                                (25)
#define MIS_UMSK_ISR_GP2A_int86_mask                                                 (0x02000000)
#define MIS_UMSK_ISR_GP2A_int86(data)                                                (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP2A_int86_src(data)                                            ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP2A_get_int86(data)                                            ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP2A_int85_shift                                                (24)
#define MIS_UMSK_ISR_GP2A_int85_mask                                                 (0x01000000)
#define MIS_UMSK_ISR_GP2A_int85(data)                                                (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP2A_int85_src(data)                                            ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP2A_get_int85(data)                                            ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP2A_int84_shift                                                (23)
#define MIS_UMSK_ISR_GP2A_int84_mask                                                 (0x00800000)
#define MIS_UMSK_ISR_GP2A_int84(data)                                                (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP2A_int84_src(data)                                            ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP2A_get_int84(data)                                            ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP2A_int83_shift                                                (22)
#define MIS_UMSK_ISR_GP2A_int83_mask                                                 (0x00400000)
#define MIS_UMSK_ISR_GP2A_int83(data)                                                (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP2A_int83_src(data)                                            ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP2A_get_int83(data)                                            ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP2A_int82_shift                                                (21)
#define MIS_UMSK_ISR_GP2A_int82_mask                                                 (0x00200000)
#define MIS_UMSK_ISR_GP2A_int82(data)                                                (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP2A_int82_src(data)                                            ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP2A_get_int82(data)                                            ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP2A_int81_shift                                                (20)
#define MIS_UMSK_ISR_GP2A_int81_mask                                                 (0x00100000)
#define MIS_UMSK_ISR_GP2A_int81(data)                                                (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP2A_int81_src(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP2A_get_int81(data)                                            ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP2A_int80_shift                                                (19)
#define MIS_UMSK_ISR_GP2A_int80_mask                                                 (0x00080000)
#define MIS_UMSK_ISR_GP2A_int80(data)                                                (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP2A_int80_src(data)                                            ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP2A_get_int80(data)                                            ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP2A_int79_shift                                                (18)
#define MIS_UMSK_ISR_GP2A_int79_mask                                                 (0x00040000)
#define MIS_UMSK_ISR_GP2A_int79(data)                                                (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP2A_int79_src(data)                                            ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP2A_get_int79(data)                                            ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP2A_int78_shift                                                (17)
#define MIS_UMSK_ISR_GP2A_int78_mask                                                 (0x00020000)
#define MIS_UMSK_ISR_GP2A_int78(data)                                                (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP2A_int78_src(data)                                            ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP2A_get_int78(data)                                            ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP2A_int77_shift                                                (16)
#define MIS_UMSK_ISR_GP2A_int77_mask                                                 (0x00010000)
#define MIS_UMSK_ISR_GP2A_int77(data)                                                (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP2A_int77_src(data)                                            ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP2A_get_int77(data)                                            ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP2A_int76_shift                                                (15)
#define MIS_UMSK_ISR_GP2A_int76_mask                                                 (0x00008000)
#define MIS_UMSK_ISR_GP2A_int76(data)                                                (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP2A_int76_src(data)                                            ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP2A_get_int76(data)                                            ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP2A_int75_shift                                                (14)
#define MIS_UMSK_ISR_GP2A_int75_mask                                                 (0x00004000)
#define MIS_UMSK_ISR_GP2A_int75(data)                                                (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP2A_int75_src(data)                                            ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP2A_get_int75(data)                                            ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP2A_int74_shift                                                (13)
#define MIS_UMSK_ISR_GP2A_int74_mask                                                 (0x00002000)
#define MIS_UMSK_ISR_GP2A_int74(data)                                                (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP2A_int74_src(data)                                            ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP2A_get_int74(data)                                            ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP2A_int73_shift                                                (12)
#define MIS_UMSK_ISR_GP2A_int73_mask                                                 (0x00001000)
#define MIS_UMSK_ISR_GP2A_int73(data)                                                (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP2A_int73_src(data)                                            ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP2A_get_int73(data)                                            ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP2A_int72_shift                                                (11)
#define MIS_UMSK_ISR_GP2A_int72_mask                                                 (0x00000800)
#define MIS_UMSK_ISR_GP2A_int72(data)                                                (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP2A_int72_src(data)                                            ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP2A_get_int72(data)                                            ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP2A_int71_shift                                                (10)
#define MIS_UMSK_ISR_GP2A_int71_mask                                                 (0x00000400)
#define MIS_UMSK_ISR_GP2A_int71(data)                                                (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP2A_int71_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP2A_get_int71(data)                                            ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP2A_int70_shift                                                (9)
#define MIS_UMSK_ISR_GP2A_int70_mask                                                 (0x00000200)
#define MIS_UMSK_ISR_GP2A_int70(data)                                                (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP2A_int70_src(data)                                            ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP2A_get_int70(data)                                            ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP2A_int69_shift                                                (8)
#define MIS_UMSK_ISR_GP2A_int69_mask                                                 (0x00000100)
#define MIS_UMSK_ISR_GP2A_int69(data)                                                (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP2A_int69_src(data)                                            ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP2A_get_int69(data)                                            ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP2A_int68_shift                                                (7)
#define MIS_UMSK_ISR_GP2A_int68_mask                                                 (0x00000080)
#define MIS_UMSK_ISR_GP2A_int68(data)                                                (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP2A_int68_src(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP2A_get_int68(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP2A_int67_shift                                                (6)
#define MIS_UMSK_ISR_GP2A_int67_mask                                                 (0x00000040)
#define MIS_UMSK_ISR_GP2A_int67(data)                                                (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP2A_int67_src(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP2A_get_int67(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP2A_int66_shift                                                (5)
#define MIS_UMSK_ISR_GP2A_int66_mask                                                 (0x00000020)
#define MIS_UMSK_ISR_GP2A_int66(data)                                                (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP2A_int66_src(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP2A_get_int66(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP2A_int65_shift                                                (4)
#define MIS_UMSK_ISR_GP2A_int65_mask                                                 (0x00000010)
#define MIS_UMSK_ISR_GP2A_int65(data)                                                (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP2A_int65_src(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP2A_get_int65(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP2A_int64_shift                                                (3)
#define MIS_UMSK_ISR_GP2A_int64_mask                                                 (0x00000008)
#define MIS_UMSK_ISR_GP2A_int64(data)                                                (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP2A_int64_src(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP2A_get_int64(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP2A_int63_shift                                                (2)
#define MIS_UMSK_ISR_GP2A_int63_mask                                                 (0x00000004)
#define MIS_UMSK_ISR_GP2A_int63(data)                                                (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP2A_int63_src(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP2A_get_int63(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP2A_int62_shift                                                (1)
#define MIS_UMSK_ISR_GP2A_int62_mask                                                 (0x00000002)
#define MIS_UMSK_ISR_GP2A_int62(data)                                                (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP2A_int62_src(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP2A_get_int62(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP2A_write_data_shift                                           (0)
#define MIS_UMSK_ISR_GP2A_write_data_mask                                            (0x00000001)
#define MIS_UMSK_ISR_GP2A_write_data(data)                                           (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP2A_write_data_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP2A_get_write_data(data)                                       ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP3A                                                            0x1801B04C
#define MIS_UMSK_ISR_GP3A_reg_addr                                                   "0xB801B04C"
#define MIS_UMSK_ISR_GP3A_reg                                                        0xB801B04C
#define set_MIS_UMSK_ISR_GP3A_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP3A_reg)=data)
#define get_MIS_UMSK_ISR_GP3A_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP3A_reg))
#define MIS_UMSK_ISR_GP3A_inst_adr                                                   "0x0013"
#define MIS_UMSK_ISR_GP3A_inst                                                       0x0013
#define MIS_UMSK_ISR_GP3A_int123_shift                                               (31)
#define MIS_UMSK_ISR_GP3A_int123_mask                                                (0x80000000)
#define MIS_UMSK_ISR_GP3A_int123(data)                                               (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP3A_int123_src(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP3A_get_int123(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP3A_int122_shift                                               (30)
#define MIS_UMSK_ISR_GP3A_int122_mask                                                (0x40000000)
#define MIS_UMSK_ISR_GP3A_int122(data)                                               (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP3A_int122_src(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP3A_get_int122(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP3A_int121_shift                                               (29)
#define MIS_UMSK_ISR_GP3A_int121_mask                                                (0x20000000)
#define MIS_UMSK_ISR_GP3A_int121(data)                                               (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP3A_int121_src(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP3A_get_int121(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP3A_int120_shift                                               (28)
#define MIS_UMSK_ISR_GP3A_int120_mask                                                (0x10000000)
#define MIS_UMSK_ISR_GP3A_int120(data)                                               (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP3A_int120_src(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP3A_get_int120(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP3A_int119_shift                                               (27)
#define MIS_UMSK_ISR_GP3A_int119_mask                                                (0x08000000)
#define MIS_UMSK_ISR_GP3A_int119(data)                                               (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP3A_int119_src(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP3A_get_int119(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP3A_int118_shift                                               (26)
#define MIS_UMSK_ISR_GP3A_int118_mask                                                (0x04000000)
#define MIS_UMSK_ISR_GP3A_int118(data)                                               (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP3A_int118_src(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP3A_get_int118(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP3A_int117_shift                                               (25)
#define MIS_UMSK_ISR_GP3A_int117_mask                                                (0x02000000)
#define MIS_UMSK_ISR_GP3A_int117(data)                                               (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP3A_int117_src(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP3A_get_int117(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP3A_int116_shift                                               (24)
#define MIS_UMSK_ISR_GP3A_int116_mask                                                (0x01000000)
#define MIS_UMSK_ISR_GP3A_int116(data)                                               (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP3A_int116_src(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP3A_get_int116(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP3A_int115_shift                                               (23)
#define MIS_UMSK_ISR_GP3A_int115_mask                                                (0x00800000)
#define MIS_UMSK_ISR_GP3A_int115(data)                                               (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP3A_int115_src(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP3A_get_int115(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP3A_int114_shift                                               (22)
#define MIS_UMSK_ISR_GP3A_int114_mask                                                (0x00400000)
#define MIS_UMSK_ISR_GP3A_int114(data)                                               (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP3A_int114_src(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP3A_get_int114(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP3A_int113_shift                                               (21)
#define MIS_UMSK_ISR_GP3A_int113_mask                                                (0x00200000)
#define MIS_UMSK_ISR_GP3A_int113(data)                                               (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP3A_int113_src(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP3A_get_int113(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP3A_int112_shift                                               (20)
#define MIS_UMSK_ISR_GP3A_int112_mask                                                (0x00100000)
#define MIS_UMSK_ISR_GP3A_int112(data)                                               (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP3A_int112_src(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP3A_get_int112(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP3A_int111_shift                                               (19)
#define MIS_UMSK_ISR_GP3A_int111_mask                                                (0x00080000)
#define MIS_UMSK_ISR_GP3A_int111(data)                                               (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP3A_int111_src(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP3A_get_int111(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP3A_int110_shift                                               (18)
#define MIS_UMSK_ISR_GP3A_int110_mask                                                (0x00040000)
#define MIS_UMSK_ISR_GP3A_int110(data)                                               (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP3A_int110_src(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP3A_get_int110(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP3A_int109_shift                                               (17)
#define MIS_UMSK_ISR_GP3A_int109_mask                                                (0x00020000)
#define MIS_UMSK_ISR_GP3A_int109(data)                                               (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP3A_int109_src(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP3A_get_int109(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP3A_int108_shift                                               (16)
#define MIS_UMSK_ISR_GP3A_int108_mask                                                (0x00010000)
#define MIS_UMSK_ISR_GP3A_int108(data)                                               (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP3A_int108_src(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP3A_get_int108(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP3A_int107_shift                                               (15)
#define MIS_UMSK_ISR_GP3A_int107_mask                                                (0x00008000)
#define MIS_UMSK_ISR_GP3A_int107(data)                                               (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP3A_int107_src(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP3A_get_int107(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP3A_int106_shift                                               (14)
#define MIS_UMSK_ISR_GP3A_int106_mask                                                (0x00004000)
#define MIS_UMSK_ISR_GP3A_int106(data)                                               (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP3A_int106_src(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP3A_get_int106(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP3A_int105_shift                                               (13)
#define MIS_UMSK_ISR_GP3A_int105_mask                                                (0x00002000)
#define MIS_UMSK_ISR_GP3A_int105(data)                                               (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP3A_int105_src(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP3A_get_int105(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP3A_int104_shift                                               (12)
#define MIS_UMSK_ISR_GP3A_int104_mask                                                (0x00001000)
#define MIS_UMSK_ISR_GP3A_int104(data)                                               (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP3A_int104_src(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP3A_get_int104(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP3A_int103_shift                                               (11)
#define MIS_UMSK_ISR_GP3A_int103_mask                                                (0x00000800)
#define MIS_UMSK_ISR_GP3A_int103(data)                                               (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP3A_int103_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP3A_get_int103(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP3A_int102_shift                                               (10)
#define MIS_UMSK_ISR_GP3A_int102_mask                                                (0x00000400)
#define MIS_UMSK_ISR_GP3A_int102(data)                                               (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP3A_int102_src(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP3A_get_int102(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP3A_int101_shift                                               (9)
#define MIS_UMSK_ISR_GP3A_int101_mask                                                (0x00000200)
#define MIS_UMSK_ISR_GP3A_int101(data)                                               (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP3A_int101_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP3A_get_int101(data)                                           ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP3A_int100_shift                                               (8)
#define MIS_UMSK_ISR_GP3A_int100_mask                                                (0x00000100)
#define MIS_UMSK_ISR_GP3A_int100(data)                                               (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP3A_int100_src(data)                                           ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP3A_get_int100(data)                                           ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP3A_int99_shift                                                (7)
#define MIS_UMSK_ISR_GP3A_int99_mask                                                 (0x00000080)
#define MIS_UMSK_ISR_GP3A_int99(data)                                                (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP3A_int99_src(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP3A_get_int99(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP3A_int98_shift                                                (6)
#define MIS_UMSK_ISR_GP3A_int98_mask                                                 (0x00000040)
#define MIS_UMSK_ISR_GP3A_int98(data)                                                (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP3A_int98_src(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP3A_get_int98(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP3A_int97_shift                                                (5)
#define MIS_UMSK_ISR_GP3A_int97_mask                                                 (0x00000020)
#define MIS_UMSK_ISR_GP3A_int97(data)                                                (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP3A_int97_src(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP3A_get_int97(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP3A_int96_shift                                                (4)
#define MIS_UMSK_ISR_GP3A_int96_mask                                                 (0x00000010)
#define MIS_UMSK_ISR_GP3A_int96(data)                                                (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP3A_int96_src(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP3A_get_int96(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP3A_int95_shift                                                (3)
#define MIS_UMSK_ISR_GP3A_int95_mask                                                 (0x00000008)
#define MIS_UMSK_ISR_GP3A_int95(data)                                                (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP3A_int95_src(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP3A_get_int95(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP3A_int94_shift                                                (2)
#define MIS_UMSK_ISR_GP3A_int94_mask                                                 (0x00000004)
#define MIS_UMSK_ISR_GP3A_int94(data)                                                (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP3A_int94_src(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP3A_get_int94(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP3A_int93_shift                                                (1)
#define MIS_UMSK_ISR_GP3A_int93_mask                                                 (0x00000002)
#define MIS_UMSK_ISR_GP3A_int93(data)                                                (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP3A_int93_src(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP3A_get_int93(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP3A_write_data_shift                                           (0)
#define MIS_UMSK_ISR_GP3A_write_data_mask                                            (0x00000001)
#define MIS_UMSK_ISR_GP3A_write_data(data)                                           (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP3A_write_data_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP3A_get_write_data(data)                                       ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP4A                                                            0x1801B050
#define MIS_UMSK_ISR_GP4A_reg_addr                                                   "0xB801B050"
#define MIS_UMSK_ISR_GP4A_reg                                                        0xB801B050
#define set_MIS_UMSK_ISR_GP4A_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP4A_reg)=data)
#define get_MIS_UMSK_ISR_GP4A_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP4A_reg))
#define MIS_UMSK_ISR_GP4A_inst_adr                                                   "0x0014"
#define MIS_UMSK_ISR_GP4A_inst                                                       0x0014
#define MIS_UMSK_ISR_GP4A_int127_shift                                               (4)
#define MIS_UMSK_ISR_GP4A_int127_mask                                                (0x00000010)
#define MIS_UMSK_ISR_GP4A_int127(data)                                               (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP4A_int127_src(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP4A_get_int127(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP4A_int126_shift                                               (3)
#define MIS_UMSK_ISR_GP4A_int126_mask                                                (0x00000008)
#define MIS_UMSK_ISR_GP4A_int126(data)                                               (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP4A_int126_src(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP4A_get_int126(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP4A_int125_shift                                               (2)
#define MIS_UMSK_ISR_GP4A_int125_mask                                                (0x00000004)
#define MIS_UMSK_ISR_GP4A_int125(data)                                               (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP4A_int125_src(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP4A_get_int125(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP4A_int124_shift                                               (1)
#define MIS_UMSK_ISR_GP4A_int124_mask                                                (0x00000002)
#define MIS_UMSK_ISR_GP4A_int124(data)                                               (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP4A_int124_src(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP4A_get_int124(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP4A_write_data_shift                                           (0)
#define MIS_UMSK_ISR_GP4A_write_data_mask                                            (0x00000001)
#define MIS_UMSK_ISR_GP4A_write_data(data)                                           (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP4A_write_data_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP4A_get_write_data(data)                                       ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP0DA                                                           0x1801B054
#define MIS_UMSK_ISR_GP0DA_reg_addr                                                  "0xB801B054"
#define MIS_UMSK_ISR_GP0DA_reg                                                       0xB801B054
#define set_MIS_UMSK_ISR_GP0DA_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP0DA_reg)=data)
#define get_MIS_UMSK_ISR_GP0DA_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP0DA_reg))
#define MIS_UMSK_ISR_GP0DA_inst_adr                                                  "0x0015"
#define MIS_UMSK_ISR_GP0DA_inst                                                      0x0015
#define MIS_UMSK_ISR_GP0DA_int30_shift                                               (31)
#define MIS_UMSK_ISR_GP0DA_int30_mask                                                (0x80000000)
#define MIS_UMSK_ISR_GP0DA_int30(data)                                               (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP0DA_int30_src(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP0DA_get_int30(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP0DA_int29_shift                                               (30)
#define MIS_UMSK_ISR_GP0DA_int29_mask                                                (0x40000000)
#define MIS_UMSK_ISR_GP0DA_int29(data)                                               (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP0DA_int29_src(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP0DA_get_int29(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP0DA_int28_shift                                               (29)
#define MIS_UMSK_ISR_GP0DA_int28_mask                                                (0x20000000)
#define MIS_UMSK_ISR_GP0DA_int28(data)                                               (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP0DA_int28_src(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP0DA_get_int28(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP0DA_int27_shift                                               (28)
#define MIS_UMSK_ISR_GP0DA_int27_mask                                                (0x10000000)
#define MIS_UMSK_ISR_GP0DA_int27(data)                                               (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP0DA_int27_src(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP0DA_get_int27(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP0DA_int26_shift                                               (27)
#define MIS_UMSK_ISR_GP0DA_int26_mask                                                (0x08000000)
#define MIS_UMSK_ISR_GP0DA_int26(data)                                               (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP0DA_int26_src(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP0DA_get_int26(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP0DA_int25_shift                                               (26)
#define MIS_UMSK_ISR_GP0DA_int25_mask                                                (0x04000000)
#define MIS_UMSK_ISR_GP0DA_int25(data)                                               (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP0DA_int25_src(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP0DA_get_int25(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP0DA_int24_shift                                               (25)
#define MIS_UMSK_ISR_GP0DA_int24_mask                                                (0x02000000)
#define MIS_UMSK_ISR_GP0DA_int24(data)                                               (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP0DA_int24_src(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP0DA_get_int24(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP0DA_int23_shift                                               (24)
#define MIS_UMSK_ISR_GP0DA_int23_mask                                                (0x01000000)
#define MIS_UMSK_ISR_GP0DA_int23(data)                                               (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP0DA_int23_src(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP0DA_get_int23(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP0DA_int22_shift                                               (23)
#define MIS_UMSK_ISR_GP0DA_int22_mask                                                (0x00800000)
#define MIS_UMSK_ISR_GP0DA_int22(data)                                               (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP0DA_int22_src(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP0DA_get_int22(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP0DA_int21_shift                                               (22)
#define MIS_UMSK_ISR_GP0DA_int21_mask                                                (0x00400000)
#define MIS_UMSK_ISR_GP0DA_int21(data)                                               (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP0DA_int21_src(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP0DA_get_int21(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP0DA_int20_shift                                               (21)
#define MIS_UMSK_ISR_GP0DA_int20_mask                                                (0x00200000)
#define MIS_UMSK_ISR_GP0DA_int20(data)                                               (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP0DA_int20_src(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP0DA_get_int20(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP0DA_int19_shift                                               (20)
#define MIS_UMSK_ISR_GP0DA_int19_mask                                                (0x00100000)
#define MIS_UMSK_ISR_GP0DA_int19(data)                                               (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP0DA_int19_src(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP0DA_get_int19(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP0DA_int18_shift                                               (19)
#define MIS_UMSK_ISR_GP0DA_int18_mask                                                (0x00080000)
#define MIS_UMSK_ISR_GP0DA_int18(data)                                               (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP0DA_int18_src(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP0DA_get_int18(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP0DA_int17_shift                                               (18)
#define MIS_UMSK_ISR_GP0DA_int17_mask                                                (0x00040000)
#define MIS_UMSK_ISR_GP0DA_int17(data)                                               (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP0DA_int17_src(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP0DA_get_int17(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP0DA_int16_shift                                               (17)
#define MIS_UMSK_ISR_GP0DA_int16_mask                                                (0x00020000)
#define MIS_UMSK_ISR_GP0DA_int16(data)                                               (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP0DA_int16_src(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP0DA_get_int16(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP0DA_int15_shift                                               (16)
#define MIS_UMSK_ISR_GP0DA_int15_mask                                                (0x00010000)
#define MIS_UMSK_ISR_GP0DA_int15(data)                                               (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP0DA_int15_src(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP0DA_get_int15(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP0DA_int14_shift                                               (15)
#define MIS_UMSK_ISR_GP0DA_int14_mask                                                (0x00008000)
#define MIS_UMSK_ISR_GP0DA_int14(data)                                               (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP0DA_int14_src(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP0DA_get_int14(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP0DA_int13_shift                                               (14)
#define MIS_UMSK_ISR_GP0DA_int13_mask                                                (0x00004000)
#define MIS_UMSK_ISR_GP0DA_int13(data)                                               (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP0DA_int13_src(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP0DA_get_int13(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP0DA_int12_shift                                               (13)
#define MIS_UMSK_ISR_GP0DA_int12_mask                                                (0x00002000)
#define MIS_UMSK_ISR_GP0DA_int12(data)                                               (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP0DA_int12_src(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP0DA_get_int12(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP0DA_int11_shift                                               (12)
#define MIS_UMSK_ISR_GP0DA_int11_mask                                                (0x00001000)
#define MIS_UMSK_ISR_GP0DA_int11(data)                                               (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP0DA_int11_src(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP0DA_get_int11(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP0DA_int10_shift                                               (11)
#define MIS_UMSK_ISR_GP0DA_int10_mask                                                (0x00000800)
#define MIS_UMSK_ISR_GP0DA_int10(data)                                               (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP0DA_int10_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP0DA_get_int10(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP0DA_int9_shift                                                (10)
#define MIS_UMSK_ISR_GP0DA_int9_mask                                                 (0x00000400)
#define MIS_UMSK_ISR_GP0DA_int9(data)                                                (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP0DA_int9_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP0DA_get_int9(data)                                            ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP0DA_int8_shift                                                (9)
#define MIS_UMSK_ISR_GP0DA_int8_mask                                                 (0x00000200)
#define MIS_UMSK_ISR_GP0DA_int8(data)                                                (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP0DA_int8_src(data)                                            ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP0DA_get_int8(data)                                            ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP0DA_int7_shift                                                (8)
#define MIS_UMSK_ISR_GP0DA_int7_mask                                                 (0x00000100)
#define MIS_UMSK_ISR_GP0DA_int7(data)                                                (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP0DA_int7_src(data)                                            ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP0DA_get_int7(data)                                            ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP0DA_int6_shift                                                (7)
#define MIS_UMSK_ISR_GP0DA_int6_mask                                                 (0x00000080)
#define MIS_UMSK_ISR_GP0DA_int6(data)                                                (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP0DA_int6_src(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP0DA_get_int6(data)                                            ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP0DA_int5_shift                                                (6)
#define MIS_UMSK_ISR_GP0DA_int5_mask                                                 (0x00000040)
#define MIS_UMSK_ISR_GP0DA_int5(data)                                                (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP0DA_int5_src(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP0DA_get_int5(data)                                            ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP0DA_int4_shift                                                (5)
#define MIS_UMSK_ISR_GP0DA_int4_mask                                                 (0x00000020)
#define MIS_UMSK_ISR_GP0DA_int4(data)                                                (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP0DA_int4_src(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP0DA_get_int4(data)                                            ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP0DA_int3_shift                                                (4)
#define MIS_UMSK_ISR_GP0DA_int3_mask                                                 (0x00000010)
#define MIS_UMSK_ISR_GP0DA_int3(data)                                                (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP0DA_int3_src(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP0DA_get_int3(data)                                            ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP0DA_int2_shift                                                (3)
#define MIS_UMSK_ISR_GP0DA_int2_mask                                                 (0x00000008)
#define MIS_UMSK_ISR_GP0DA_int2(data)                                                (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP0DA_int2_src(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP0DA_get_int2(data)                                            ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP0DA_int1_shift                                                (2)
#define MIS_UMSK_ISR_GP0DA_int1_mask                                                 (0x00000004)
#define MIS_UMSK_ISR_GP0DA_int1(data)                                                (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP0DA_int1_src(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP0DA_get_int1(data)                                            ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP0DA_int0_shift                                                (1)
#define MIS_UMSK_ISR_GP0DA_int0_mask                                                 (0x00000002)
#define MIS_UMSK_ISR_GP0DA_int0(data)                                                (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP0DA_int0_src(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP0DA_get_int0(data)                                            ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP0DA_write_data_shift                                          (0)
#define MIS_UMSK_ISR_GP0DA_write_data_mask                                           (0x00000001)
#define MIS_UMSK_ISR_GP0DA_write_data(data)                                          (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP0DA_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP0DA_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP1DA                                                           0x1801B058
#define MIS_UMSK_ISR_GP1DA_reg_addr                                                  "0xB801B058"
#define MIS_UMSK_ISR_GP1DA_reg                                                       0xB801B058
#define set_MIS_UMSK_ISR_GP1DA_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP1DA_reg)=data)
#define get_MIS_UMSK_ISR_GP1DA_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP1DA_reg))
#define MIS_UMSK_ISR_GP1DA_inst_adr                                                  "0x0016"
#define MIS_UMSK_ISR_GP1DA_inst                                                      0x0016
#define MIS_UMSK_ISR_GP1DA_int61_shift                                               (31)
#define MIS_UMSK_ISR_GP1DA_int61_mask                                                (0x80000000)
#define MIS_UMSK_ISR_GP1DA_int61(data)                                               (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP1DA_int61_src(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP1DA_get_int61(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP1DA_int60_shift                                               (30)
#define MIS_UMSK_ISR_GP1DA_int60_mask                                                (0x40000000)
#define MIS_UMSK_ISR_GP1DA_int60(data)                                               (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP1DA_int60_src(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP1DA_get_int60(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP1DA_int59_shift                                               (29)
#define MIS_UMSK_ISR_GP1DA_int59_mask                                                (0x20000000)
#define MIS_UMSK_ISR_GP1DA_int59(data)                                               (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP1DA_int59_src(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP1DA_get_int59(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP1DA_int58_shift                                               (28)
#define MIS_UMSK_ISR_GP1DA_int58_mask                                                (0x10000000)
#define MIS_UMSK_ISR_GP1DA_int58(data)                                               (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP1DA_int58_src(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP1DA_get_int58(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP1DA_int57_shift                                               (27)
#define MIS_UMSK_ISR_GP1DA_int57_mask                                                (0x08000000)
#define MIS_UMSK_ISR_GP1DA_int57(data)                                               (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP1DA_int57_src(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP1DA_get_int57(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP1DA_int56_shift                                               (26)
#define MIS_UMSK_ISR_GP1DA_int56_mask                                                (0x04000000)
#define MIS_UMSK_ISR_GP1DA_int56(data)                                               (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP1DA_int56_src(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP1DA_get_int56(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP1DA_int55_shift                                               (25)
#define MIS_UMSK_ISR_GP1DA_int55_mask                                                (0x02000000)
#define MIS_UMSK_ISR_GP1DA_int55(data)                                               (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP1DA_int55_src(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP1DA_get_int55(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP1DA_int54_shift                                               (24)
#define MIS_UMSK_ISR_GP1DA_int54_mask                                                (0x01000000)
#define MIS_UMSK_ISR_GP1DA_int54(data)                                               (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP1DA_int54_src(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP1DA_get_int54(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP1DA_int53_shift                                               (23)
#define MIS_UMSK_ISR_GP1DA_int53_mask                                                (0x00800000)
#define MIS_UMSK_ISR_GP1DA_int53(data)                                               (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP1DA_int53_src(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP1DA_get_int53(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP1DA_int52_shift                                               (22)
#define MIS_UMSK_ISR_GP1DA_int52_mask                                                (0x00400000)
#define MIS_UMSK_ISR_GP1DA_int52(data)                                               (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP1DA_int52_src(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP1DA_get_int52(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP1DA_int51_shift                                               (21)
#define MIS_UMSK_ISR_GP1DA_int51_mask                                                (0x00200000)
#define MIS_UMSK_ISR_GP1DA_int51(data)                                               (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP1DA_int51_src(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP1DA_get_int51(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP1DA_int50_shift                                               (20)
#define MIS_UMSK_ISR_GP1DA_int50_mask                                                (0x00100000)
#define MIS_UMSK_ISR_GP1DA_int50(data)                                               (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP1DA_int50_src(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP1DA_get_int50(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP1DA_int49_shift                                               (19)
#define MIS_UMSK_ISR_GP1DA_int49_mask                                                (0x00080000)
#define MIS_UMSK_ISR_GP1DA_int49(data)                                               (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP1DA_int49_src(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP1DA_get_int49(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP1DA_int48_shift                                               (18)
#define MIS_UMSK_ISR_GP1DA_int48_mask                                                (0x00040000)
#define MIS_UMSK_ISR_GP1DA_int48(data)                                               (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP1DA_int48_src(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP1DA_get_int48(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP1DA_int47_shift                                               (17)
#define MIS_UMSK_ISR_GP1DA_int47_mask                                                (0x00020000)
#define MIS_UMSK_ISR_GP1DA_int47(data)                                               (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP1DA_int47_src(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP1DA_get_int47(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP1DA_int46_shift                                               (16)
#define MIS_UMSK_ISR_GP1DA_int46_mask                                                (0x00010000)
#define MIS_UMSK_ISR_GP1DA_int46(data)                                               (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP1DA_int46_src(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP1DA_get_int46(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP1DA_int45_shift                                               (15)
#define MIS_UMSK_ISR_GP1DA_int45_mask                                                (0x00008000)
#define MIS_UMSK_ISR_GP1DA_int45(data)                                               (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP1DA_int45_src(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP1DA_get_int45(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP1DA_int44_shift                                               (14)
#define MIS_UMSK_ISR_GP1DA_int44_mask                                                (0x00004000)
#define MIS_UMSK_ISR_GP1DA_int44(data)                                               (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP1DA_int44_src(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP1DA_get_int44(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP1DA_int43_shift                                               (13)
#define MIS_UMSK_ISR_GP1DA_int43_mask                                                (0x00002000)
#define MIS_UMSK_ISR_GP1DA_int43(data)                                               (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP1DA_int43_src(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP1DA_get_int43(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP1DA_int42_shift                                               (12)
#define MIS_UMSK_ISR_GP1DA_int42_mask                                                (0x00001000)
#define MIS_UMSK_ISR_GP1DA_int42(data)                                               (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP1DA_int42_src(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP1DA_get_int42(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP1DA_int41_shift                                               (11)
#define MIS_UMSK_ISR_GP1DA_int41_mask                                                (0x00000800)
#define MIS_UMSK_ISR_GP1DA_int41(data)                                               (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP1DA_int41_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP1DA_get_int41(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP1DA_int40_shift                                               (10)
#define MIS_UMSK_ISR_GP1DA_int40_mask                                                (0x00000400)
#define MIS_UMSK_ISR_GP1DA_int40(data)                                               (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP1DA_int40_src(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP1DA_get_int40(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP1DA_int39_shift                                               (9)
#define MIS_UMSK_ISR_GP1DA_int39_mask                                                (0x00000200)
#define MIS_UMSK_ISR_GP1DA_int39(data)                                               (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP1DA_int39_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP1DA_get_int39(data)                                           ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP1DA_int38_shift                                               (8)
#define MIS_UMSK_ISR_GP1DA_int38_mask                                                (0x00000100)
#define MIS_UMSK_ISR_GP1DA_int38(data)                                               (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP1DA_int38_src(data)                                           ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP1DA_get_int38(data)                                           ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP1DA_int37_shift                                               (7)
#define MIS_UMSK_ISR_GP1DA_int37_mask                                                (0x00000080)
#define MIS_UMSK_ISR_GP1DA_int37(data)                                               (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP1DA_int37_src(data)                                           ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP1DA_get_int37(data)                                           ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP1DA_int36_shift                                               (6)
#define MIS_UMSK_ISR_GP1DA_int36_mask                                                (0x00000040)
#define MIS_UMSK_ISR_GP1DA_int36(data)                                               (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP1DA_int36_src(data)                                           ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP1DA_get_int36(data)                                           ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP1DA_int35_shift                                               (5)
#define MIS_UMSK_ISR_GP1DA_int35_mask                                                (0x00000020)
#define MIS_UMSK_ISR_GP1DA_int35(data)                                               (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP1DA_int35_src(data)                                           ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP1DA_get_int35(data)                                           ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP1DA_int34_shift                                               (4)
#define MIS_UMSK_ISR_GP1DA_int34_mask                                                (0x00000010)
#define MIS_UMSK_ISR_GP1DA_int34(data)                                               (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP1DA_int34_src(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP1DA_get_int34(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP1DA_int33_shift                                               (3)
#define MIS_UMSK_ISR_GP1DA_int33_mask                                                (0x00000008)
#define MIS_UMSK_ISR_GP1DA_int33(data)                                               (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP1DA_int33_src(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP1DA_get_int33(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP1DA_int32_shift                                               (2)
#define MIS_UMSK_ISR_GP1DA_int32_mask                                                (0x00000004)
#define MIS_UMSK_ISR_GP1DA_int32(data)                                               (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP1DA_int32_src(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP1DA_get_int32(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP1DA_int31_shift                                               (1)
#define MIS_UMSK_ISR_GP1DA_int31_mask                                                (0x00000002)
#define MIS_UMSK_ISR_GP1DA_int31(data)                                               (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP1DA_int31_src(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP1DA_get_int31(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP1DA_write_data_shift                                          (0)
#define MIS_UMSK_ISR_GP1DA_write_data_mask                                           (0x00000001)
#define MIS_UMSK_ISR_GP1DA_write_data(data)                                          (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP1DA_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP1DA_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP2DA                                                           0x1801B05C
#define MIS_UMSK_ISR_GP2DA_reg_addr                                                  "0xB801B05C"
#define MIS_UMSK_ISR_GP2DA_reg                                                       0xB801B05C
#define set_MIS_UMSK_ISR_GP2DA_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP2DA_reg)=data)
#define get_MIS_UMSK_ISR_GP2DA_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP2DA_reg))
#define MIS_UMSK_ISR_GP2DA_inst_adr                                                  "0x0017"
#define MIS_UMSK_ISR_GP2DA_inst                                                      0x0017
#define MIS_UMSK_ISR_GP2DA_int92_shift                                               (31)
#define MIS_UMSK_ISR_GP2DA_int92_mask                                                (0x80000000)
#define MIS_UMSK_ISR_GP2DA_int92(data)                                               (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP2DA_int92_src(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP2DA_get_int92(data)                                           ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP2DA_int91_shift                                               (30)
#define MIS_UMSK_ISR_GP2DA_int91_mask                                                (0x40000000)
#define MIS_UMSK_ISR_GP2DA_int91(data)                                               (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP2DA_int91_src(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP2DA_get_int91(data)                                           ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP2DA_int90_shift                                               (29)
#define MIS_UMSK_ISR_GP2DA_int90_mask                                                (0x20000000)
#define MIS_UMSK_ISR_GP2DA_int90(data)                                               (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP2DA_int90_src(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP2DA_get_int90(data)                                           ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP2DA_int89_shift                                               (28)
#define MIS_UMSK_ISR_GP2DA_int89_mask                                                (0x10000000)
#define MIS_UMSK_ISR_GP2DA_int89(data)                                               (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP2DA_int89_src(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP2DA_get_int89(data)                                           ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP2DA_int88_shift                                               (27)
#define MIS_UMSK_ISR_GP2DA_int88_mask                                                (0x08000000)
#define MIS_UMSK_ISR_GP2DA_int88(data)                                               (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP2DA_int88_src(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP2DA_get_int88(data)                                           ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP2DA_int87_shift                                               (26)
#define MIS_UMSK_ISR_GP2DA_int87_mask                                                (0x04000000)
#define MIS_UMSK_ISR_GP2DA_int87(data)                                               (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP2DA_int87_src(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP2DA_get_int87(data)                                           ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP2DA_int86_shift                                               (25)
#define MIS_UMSK_ISR_GP2DA_int86_mask                                                (0x02000000)
#define MIS_UMSK_ISR_GP2DA_int86(data)                                               (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP2DA_int86_src(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP2DA_get_int86(data)                                           ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP2DA_int85_shift                                               (24)
#define MIS_UMSK_ISR_GP2DA_int85_mask                                                (0x01000000)
#define MIS_UMSK_ISR_GP2DA_int85(data)                                               (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP2DA_int85_src(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP2DA_get_int85(data)                                           ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP2DA_int84_shift                                               (23)
#define MIS_UMSK_ISR_GP2DA_int84_mask                                                (0x00800000)
#define MIS_UMSK_ISR_GP2DA_int84(data)                                               (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP2DA_int84_src(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP2DA_get_int84(data)                                           ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP2DA_int83_shift                                               (22)
#define MIS_UMSK_ISR_GP2DA_int83_mask                                                (0x00400000)
#define MIS_UMSK_ISR_GP2DA_int83(data)                                               (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP2DA_int83_src(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP2DA_get_int83(data)                                           ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP2DA_int82_shift                                               (21)
#define MIS_UMSK_ISR_GP2DA_int82_mask                                                (0x00200000)
#define MIS_UMSK_ISR_GP2DA_int82(data)                                               (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP2DA_int82_src(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP2DA_get_int82(data)                                           ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP2DA_int81_shift                                               (20)
#define MIS_UMSK_ISR_GP2DA_int81_mask                                                (0x00100000)
#define MIS_UMSK_ISR_GP2DA_int81(data)                                               (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP2DA_int81_src(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP2DA_get_int81(data)                                           ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP2DA_int80_shift                                               (19)
#define MIS_UMSK_ISR_GP2DA_int80_mask                                                (0x00080000)
#define MIS_UMSK_ISR_GP2DA_int80(data)                                               (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP2DA_int80_src(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP2DA_get_int80(data)                                           ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP2DA_int79_shift                                               (18)
#define MIS_UMSK_ISR_GP2DA_int79_mask                                                (0x00040000)
#define MIS_UMSK_ISR_GP2DA_int79(data)                                               (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP2DA_int79_src(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP2DA_get_int79(data)                                           ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP2DA_int78_shift                                               (17)
#define MIS_UMSK_ISR_GP2DA_int78_mask                                                (0x00020000)
#define MIS_UMSK_ISR_GP2DA_int78(data)                                               (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP2DA_int78_src(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP2DA_get_int78(data)                                           ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP2DA_int77_shift                                               (16)
#define MIS_UMSK_ISR_GP2DA_int77_mask                                                (0x00010000)
#define MIS_UMSK_ISR_GP2DA_int77(data)                                               (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP2DA_int77_src(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP2DA_get_int77(data)                                           ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP2DA_int76_shift                                               (15)
#define MIS_UMSK_ISR_GP2DA_int76_mask                                                (0x00008000)
#define MIS_UMSK_ISR_GP2DA_int76(data)                                               (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP2DA_int76_src(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP2DA_get_int76(data)                                           ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP2DA_int75_shift                                               (14)
#define MIS_UMSK_ISR_GP2DA_int75_mask                                                (0x00004000)
#define MIS_UMSK_ISR_GP2DA_int75(data)                                               (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP2DA_int75_src(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP2DA_get_int75(data)                                           ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP2DA_int74_shift                                               (13)
#define MIS_UMSK_ISR_GP2DA_int74_mask                                                (0x00002000)
#define MIS_UMSK_ISR_GP2DA_int74(data)                                               (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP2DA_int74_src(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP2DA_get_int74(data)                                           ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP2DA_int73_shift                                               (12)
#define MIS_UMSK_ISR_GP2DA_int73_mask                                                (0x00001000)
#define MIS_UMSK_ISR_GP2DA_int73(data)                                               (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP2DA_int73_src(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP2DA_get_int73(data)                                           ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP2DA_int72_shift                                               (11)
#define MIS_UMSK_ISR_GP2DA_int72_mask                                                (0x00000800)
#define MIS_UMSK_ISR_GP2DA_int72(data)                                               (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP2DA_int72_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP2DA_get_int72(data)                                           ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP2DA_int71_shift                                               (10)
#define MIS_UMSK_ISR_GP2DA_int71_mask                                                (0x00000400)
#define MIS_UMSK_ISR_GP2DA_int71(data)                                               (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP2DA_int71_src(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP2DA_get_int71(data)                                           ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP2DA_int70_shift                                               (9)
#define MIS_UMSK_ISR_GP2DA_int70_mask                                                (0x00000200)
#define MIS_UMSK_ISR_GP2DA_int70(data)                                               (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP2DA_int70_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP2DA_get_int70(data)                                           ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP2DA_int69_shift                                               (8)
#define MIS_UMSK_ISR_GP2DA_int69_mask                                                (0x00000100)
#define MIS_UMSK_ISR_GP2DA_int69(data)                                               (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP2DA_int69_src(data)                                           ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP2DA_get_int69(data)                                           ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP2DA_int68_shift                                               (7)
#define MIS_UMSK_ISR_GP2DA_int68_mask                                                (0x00000080)
#define MIS_UMSK_ISR_GP2DA_int68(data)                                               (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP2DA_int68_src(data)                                           ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP2DA_get_int68(data)                                           ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP2DA_int67_shift                                               (6)
#define MIS_UMSK_ISR_GP2DA_int67_mask                                                (0x00000040)
#define MIS_UMSK_ISR_GP2DA_int67(data)                                               (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP2DA_int67_src(data)                                           ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP2DA_get_int67(data)                                           ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP2DA_int66_shift                                               (5)
#define MIS_UMSK_ISR_GP2DA_int66_mask                                                (0x00000020)
#define MIS_UMSK_ISR_GP2DA_int66(data)                                               (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP2DA_int66_src(data)                                           ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP2DA_get_int66(data)                                           ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP2DA_int65_shift                                               (4)
#define MIS_UMSK_ISR_GP2DA_int65_mask                                                (0x00000010)
#define MIS_UMSK_ISR_GP2DA_int65(data)                                               (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP2DA_int65_src(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP2DA_get_int65(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP2DA_int64_shift                                               (3)
#define MIS_UMSK_ISR_GP2DA_int64_mask                                                (0x00000008)
#define MIS_UMSK_ISR_GP2DA_int64(data)                                               (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP2DA_int64_src(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP2DA_get_int64(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP2DA_int63_shift                                               (2)
#define MIS_UMSK_ISR_GP2DA_int63_mask                                                (0x00000004)
#define MIS_UMSK_ISR_GP2DA_int63(data)                                               (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP2DA_int63_src(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP2DA_get_int63(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP2DA_int62_shift                                               (1)
#define MIS_UMSK_ISR_GP2DA_int62_mask                                                (0x00000002)
#define MIS_UMSK_ISR_GP2DA_int62(data)                                               (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP2DA_int62_src(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP2DA_get_int62(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP2DA_write_data_shift                                          (0)
#define MIS_UMSK_ISR_GP2DA_write_data_mask                                           (0x00000001)
#define MIS_UMSK_ISR_GP2DA_write_data(data)                                          (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP2DA_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP2DA_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP3DA                                                           0x1801B060
#define MIS_UMSK_ISR_GP3DA_reg_addr                                                  "0xB801B060"
#define MIS_UMSK_ISR_GP3DA_reg                                                       0xB801B060
#define set_MIS_UMSK_ISR_GP3DA_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP3DA_reg)=data)
#define get_MIS_UMSK_ISR_GP3DA_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP3DA_reg))
#define MIS_UMSK_ISR_GP3DA_inst_adr                                                  "0x0018"
#define MIS_UMSK_ISR_GP3DA_inst                                                      0x0018
#define MIS_UMSK_ISR_GP3DA_int123_shift                                              (31)
#define MIS_UMSK_ISR_GP3DA_int123_mask                                               (0x80000000)
#define MIS_UMSK_ISR_GP3DA_int123(data)                                              (0x80000000&((data)<<31))
#define MIS_UMSK_ISR_GP3DA_int123_src(data)                                          ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP3DA_get_int123(data)                                          ((0x80000000&(data))>>31)
#define MIS_UMSK_ISR_GP3DA_int122_shift                                              (30)
#define MIS_UMSK_ISR_GP3DA_int122_mask                                               (0x40000000)
#define MIS_UMSK_ISR_GP3DA_int122(data)                                              (0x40000000&((data)<<30))
#define MIS_UMSK_ISR_GP3DA_int122_src(data)                                          ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP3DA_get_int122(data)                                          ((0x40000000&(data))>>30)
#define MIS_UMSK_ISR_GP3DA_int121_shift                                              (29)
#define MIS_UMSK_ISR_GP3DA_int121_mask                                               (0x20000000)
#define MIS_UMSK_ISR_GP3DA_int121(data)                                              (0x20000000&((data)<<29))
#define MIS_UMSK_ISR_GP3DA_int121_src(data)                                          ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP3DA_get_int121(data)                                          ((0x20000000&(data))>>29)
#define MIS_UMSK_ISR_GP3DA_int120_shift                                              (28)
#define MIS_UMSK_ISR_GP3DA_int120_mask                                               (0x10000000)
#define MIS_UMSK_ISR_GP3DA_int120(data)                                              (0x10000000&((data)<<28))
#define MIS_UMSK_ISR_GP3DA_int120_src(data)                                          ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP3DA_get_int120(data)                                          ((0x10000000&(data))>>28)
#define MIS_UMSK_ISR_GP3DA_int119_shift                                              (27)
#define MIS_UMSK_ISR_GP3DA_int119_mask                                               (0x08000000)
#define MIS_UMSK_ISR_GP3DA_int119(data)                                              (0x08000000&((data)<<27))
#define MIS_UMSK_ISR_GP3DA_int119_src(data)                                          ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP3DA_get_int119(data)                                          ((0x08000000&(data))>>27)
#define MIS_UMSK_ISR_GP3DA_int118_shift                                              (26)
#define MIS_UMSK_ISR_GP3DA_int118_mask                                               (0x04000000)
#define MIS_UMSK_ISR_GP3DA_int118(data)                                              (0x04000000&((data)<<26))
#define MIS_UMSK_ISR_GP3DA_int118_src(data)                                          ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP3DA_get_int118(data)                                          ((0x04000000&(data))>>26)
#define MIS_UMSK_ISR_GP3DA_int117_shift                                              (25)
#define MIS_UMSK_ISR_GP3DA_int117_mask                                               (0x02000000)
#define MIS_UMSK_ISR_GP3DA_int117(data)                                              (0x02000000&((data)<<25))
#define MIS_UMSK_ISR_GP3DA_int117_src(data)                                          ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP3DA_get_int117(data)                                          ((0x02000000&(data))>>25)
#define MIS_UMSK_ISR_GP3DA_int116_shift                                              (24)
#define MIS_UMSK_ISR_GP3DA_int116_mask                                               (0x01000000)
#define MIS_UMSK_ISR_GP3DA_int116(data)                                              (0x01000000&((data)<<24))
#define MIS_UMSK_ISR_GP3DA_int116_src(data)                                          ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP3DA_get_int116(data)                                          ((0x01000000&(data))>>24)
#define MIS_UMSK_ISR_GP3DA_int115_shift                                              (23)
#define MIS_UMSK_ISR_GP3DA_int115_mask                                               (0x00800000)
#define MIS_UMSK_ISR_GP3DA_int115(data)                                              (0x00800000&((data)<<23))
#define MIS_UMSK_ISR_GP3DA_int115_src(data)                                          ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP3DA_get_int115(data)                                          ((0x00800000&(data))>>23)
#define MIS_UMSK_ISR_GP3DA_int114_shift                                              (22)
#define MIS_UMSK_ISR_GP3DA_int114_mask                                               (0x00400000)
#define MIS_UMSK_ISR_GP3DA_int114(data)                                              (0x00400000&((data)<<22))
#define MIS_UMSK_ISR_GP3DA_int114_src(data)                                          ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP3DA_get_int114(data)                                          ((0x00400000&(data))>>22)
#define MIS_UMSK_ISR_GP3DA_int113_shift                                              (21)
#define MIS_UMSK_ISR_GP3DA_int113_mask                                               (0x00200000)
#define MIS_UMSK_ISR_GP3DA_int113(data)                                              (0x00200000&((data)<<21))
#define MIS_UMSK_ISR_GP3DA_int113_src(data)                                          ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP3DA_get_int113(data)                                          ((0x00200000&(data))>>21)
#define MIS_UMSK_ISR_GP3DA_int112_shift                                              (20)
#define MIS_UMSK_ISR_GP3DA_int112_mask                                               (0x00100000)
#define MIS_UMSK_ISR_GP3DA_int112(data)                                              (0x00100000&((data)<<20))
#define MIS_UMSK_ISR_GP3DA_int112_src(data)                                          ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP3DA_get_int112(data)                                          ((0x00100000&(data))>>20)
#define MIS_UMSK_ISR_GP3DA_int111_shift                                              (19)
#define MIS_UMSK_ISR_GP3DA_int111_mask                                               (0x00080000)
#define MIS_UMSK_ISR_GP3DA_int111(data)                                              (0x00080000&((data)<<19))
#define MIS_UMSK_ISR_GP3DA_int111_src(data)                                          ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP3DA_get_int111(data)                                          ((0x00080000&(data))>>19)
#define MIS_UMSK_ISR_GP3DA_int110_shift                                              (18)
#define MIS_UMSK_ISR_GP3DA_int110_mask                                               (0x00040000)
#define MIS_UMSK_ISR_GP3DA_int110(data)                                              (0x00040000&((data)<<18))
#define MIS_UMSK_ISR_GP3DA_int110_src(data)                                          ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP3DA_get_int110(data)                                          ((0x00040000&(data))>>18)
#define MIS_UMSK_ISR_GP3DA_int109_shift                                              (17)
#define MIS_UMSK_ISR_GP3DA_int109_mask                                               (0x00020000)
#define MIS_UMSK_ISR_GP3DA_int109(data)                                              (0x00020000&((data)<<17))
#define MIS_UMSK_ISR_GP3DA_int109_src(data)                                          ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP3DA_get_int109(data)                                          ((0x00020000&(data))>>17)
#define MIS_UMSK_ISR_GP3DA_int108_shift                                              (16)
#define MIS_UMSK_ISR_GP3DA_int108_mask                                               (0x00010000)
#define MIS_UMSK_ISR_GP3DA_int108(data)                                              (0x00010000&((data)<<16))
#define MIS_UMSK_ISR_GP3DA_int108_src(data)                                          ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP3DA_get_int108(data)                                          ((0x00010000&(data))>>16)
#define MIS_UMSK_ISR_GP3DA_int107_shift                                              (15)
#define MIS_UMSK_ISR_GP3DA_int107_mask                                               (0x00008000)
#define MIS_UMSK_ISR_GP3DA_int107(data)                                              (0x00008000&((data)<<15))
#define MIS_UMSK_ISR_GP3DA_int107_src(data)                                          ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP3DA_get_int107(data)                                          ((0x00008000&(data))>>15)
#define MIS_UMSK_ISR_GP3DA_int106_shift                                              (14)
#define MIS_UMSK_ISR_GP3DA_int106_mask                                               (0x00004000)
#define MIS_UMSK_ISR_GP3DA_int106(data)                                              (0x00004000&((data)<<14))
#define MIS_UMSK_ISR_GP3DA_int106_src(data)                                          ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP3DA_get_int106(data)                                          ((0x00004000&(data))>>14)
#define MIS_UMSK_ISR_GP3DA_int105_shift                                              (13)
#define MIS_UMSK_ISR_GP3DA_int105_mask                                               (0x00002000)
#define MIS_UMSK_ISR_GP3DA_int105(data)                                              (0x00002000&((data)<<13))
#define MIS_UMSK_ISR_GP3DA_int105_src(data)                                          ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP3DA_get_int105(data)                                          ((0x00002000&(data))>>13)
#define MIS_UMSK_ISR_GP3DA_int104_shift                                              (12)
#define MIS_UMSK_ISR_GP3DA_int104_mask                                               (0x00001000)
#define MIS_UMSK_ISR_GP3DA_int104(data)                                              (0x00001000&((data)<<12))
#define MIS_UMSK_ISR_GP3DA_int104_src(data)                                          ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP3DA_get_int104(data)                                          ((0x00001000&(data))>>12)
#define MIS_UMSK_ISR_GP3DA_int103_shift                                              (11)
#define MIS_UMSK_ISR_GP3DA_int103_mask                                               (0x00000800)
#define MIS_UMSK_ISR_GP3DA_int103(data)                                              (0x00000800&((data)<<11))
#define MIS_UMSK_ISR_GP3DA_int103_src(data)                                          ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP3DA_get_int103(data)                                          ((0x00000800&(data))>>11)
#define MIS_UMSK_ISR_GP3DA_int102_shift                                              (10)
#define MIS_UMSK_ISR_GP3DA_int102_mask                                               (0x00000400)
#define MIS_UMSK_ISR_GP3DA_int102(data)                                              (0x00000400&((data)<<10))
#define MIS_UMSK_ISR_GP3DA_int102_src(data)                                          ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP3DA_get_int102(data)                                          ((0x00000400&(data))>>10)
#define MIS_UMSK_ISR_GP3DA_int101_shift                                              (9)
#define MIS_UMSK_ISR_GP3DA_int101_mask                                               (0x00000200)
#define MIS_UMSK_ISR_GP3DA_int101(data)                                              (0x00000200&((data)<<9))
#define MIS_UMSK_ISR_GP3DA_int101_src(data)                                          ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP3DA_get_int101(data)                                          ((0x00000200&(data))>>9)
#define MIS_UMSK_ISR_GP3DA_int100_shift                                              (8)
#define MIS_UMSK_ISR_GP3DA_int100_mask                                               (0x00000100)
#define MIS_UMSK_ISR_GP3DA_int100(data)                                              (0x00000100&((data)<<8))
#define MIS_UMSK_ISR_GP3DA_int100_src(data)                                          ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP3DA_get_int100(data)                                          ((0x00000100&(data))>>8)
#define MIS_UMSK_ISR_GP3DA_int99_shift                                               (7)
#define MIS_UMSK_ISR_GP3DA_int99_mask                                                (0x00000080)
#define MIS_UMSK_ISR_GP3DA_int99(data)                                               (0x00000080&((data)<<7))
#define MIS_UMSK_ISR_GP3DA_int99_src(data)                                           ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP3DA_get_int99(data)                                           ((0x00000080&(data))>>7)
#define MIS_UMSK_ISR_GP3DA_int98_shift                                               (6)
#define MIS_UMSK_ISR_GP3DA_int98_mask                                                (0x00000040)
#define MIS_UMSK_ISR_GP3DA_int98(data)                                               (0x00000040&((data)<<6))
#define MIS_UMSK_ISR_GP3DA_int98_src(data)                                           ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP3DA_get_int98(data)                                           ((0x00000040&(data))>>6)
#define MIS_UMSK_ISR_GP3DA_int97_shift                                               (5)
#define MIS_UMSK_ISR_GP3DA_int97_mask                                                (0x00000020)
#define MIS_UMSK_ISR_GP3DA_int97(data)                                               (0x00000020&((data)<<5))
#define MIS_UMSK_ISR_GP3DA_int97_src(data)                                           ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP3DA_get_int97(data)                                           ((0x00000020&(data))>>5)
#define MIS_UMSK_ISR_GP3DA_int96_shift                                               (4)
#define MIS_UMSK_ISR_GP3DA_int96_mask                                                (0x00000010)
#define MIS_UMSK_ISR_GP3DA_int96(data)                                               (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP3DA_int96_src(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP3DA_get_int96(data)                                           ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP3DA_int95_shift                                               (3)
#define MIS_UMSK_ISR_GP3DA_int95_mask                                                (0x00000008)
#define MIS_UMSK_ISR_GP3DA_int95(data)                                               (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP3DA_int95_src(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP3DA_get_int95(data)                                           ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP3DA_int94_shift                                               (2)
#define MIS_UMSK_ISR_GP3DA_int94_mask                                                (0x00000004)
#define MIS_UMSK_ISR_GP3DA_int94(data)                                               (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP3DA_int94_src(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP3DA_get_int94(data)                                           ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP3DA_int93_shift                                               (1)
#define MIS_UMSK_ISR_GP3DA_int93_mask                                                (0x00000002)
#define MIS_UMSK_ISR_GP3DA_int93(data)                                               (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP3DA_int93_src(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP3DA_get_int93(data)                                           ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP3DA_write_data_shift                                          (0)
#define MIS_UMSK_ISR_GP3DA_write_data_mask                                           (0x00000001)
#define MIS_UMSK_ISR_GP3DA_write_data(data)                                          (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP3DA_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP3DA_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define MIS_UMSK_ISR_GP4DA                                                           0x1801B064
#define MIS_UMSK_ISR_GP4DA_reg_addr                                                  "0xB801B064"
#define MIS_UMSK_ISR_GP4DA_reg                                                       0xB801B064
#define set_MIS_UMSK_ISR_GP4DA_reg(data)   (*((volatile unsigned int*) MIS_UMSK_ISR_GP4DA_reg)=data)
#define get_MIS_UMSK_ISR_GP4DA_reg   (*((volatile unsigned int*) MIS_UMSK_ISR_GP4DA_reg))
#define MIS_UMSK_ISR_GP4DA_inst_adr                                                  "0x0019"
#define MIS_UMSK_ISR_GP4DA_inst                                                      0x0019
#define MIS_UMSK_ISR_GP4DA_int127_shift                                              (4)
#define MIS_UMSK_ISR_GP4DA_int127_mask                                               (0x00000010)
#define MIS_UMSK_ISR_GP4DA_int127(data)                                              (0x00000010&((data)<<4))
#define MIS_UMSK_ISR_GP4DA_int127_src(data)                                          ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP4DA_get_int127(data)                                          ((0x00000010&(data))>>4)
#define MIS_UMSK_ISR_GP4DA_int126_shift                                              (3)
#define MIS_UMSK_ISR_GP4DA_int126_mask                                               (0x00000008)
#define MIS_UMSK_ISR_GP4DA_int126(data)                                              (0x00000008&((data)<<3))
#define MIS_UMSK_ISR_GP4DA_int126_src(data)                                          ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP4DA_get_int126(data)                                          ((0x00000008&(data))>>3)
#define MIS_UMSK_ISR_GP4DA_int125_shift                                              (2)
#define MIS_UMSK_ISR_GP4DA_int125_mask                                               (0x00000004)
#define MIS_UMSK_ISR_GP4DA_int125(data)                                              (0x00000004&((data)<<2))
#define MIS_UMSK_ISR_GP4DA_int125_src(data)                                          ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP4DA_get_int125(data)                                          ((0x00000004&(data))>>2)
#define MIS_UMSK_ISR_GP4DA_int124_shift                                              (1)
#define MIS_UMSK_ISR_GP4DA_int124_mask                                               (0x00000002)
#define MIS_UMSK_ISR_GP4DA_int124(data)                                              (0x00000002&((data)<<1))
#define MIS_UMSK_ISR_GP4DA_int124_src(data)                                          ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP4DA_get_int124(data)                                          ((0x00000002&(data))>>1)
#define MIS_UMSK_ISR_GP4DA_write_data_shift                                          (0)
#define MIS_UMSK_ISR_GP4DA_write_data_mask                                           (0x00000001)
#define MIS_UMSK_ISR_GP4DA_write_data(data)                                          (0x00000001&((data)<<0))
#define MIS_UMSK_ISR_GP4DA_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_UMSK_ISR_GP4DA_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define MIS_DUMMY1                                                                   0x1801B068
#define MIS_DUMMY1_reg_addr                                                          "0xB801B068"
#define MIS_DUMMY1_reg                                                               0xB801B068
#define set_MIS_DUMMY1_reg(data)   (*((volatile unsigned int*) MIS_DUMMY1_reg)=data)
#define get_MIS_DUMMY1_reg   (*((volatile unsigned int*) MIS_DUMMY1_reg))
#define MIS_DUMMY1_inst_adr                                                          "0x001A"
#define MIS_DUMMY1_inst                                                              0x001A
#define MIS_DUMMY1_write_enable2_shift                                               (31)
#define MIS_DUMMY1_write_enable2_mask                                                (0x80000000)
#define MIS_DUMMY1_write_enable2(data)                                               (0x80000000&((data)<<31))
#define MIS_DUMMY1_write_enable2_src(data)                                           ((0x80000000&(data))>>31)
#define MIS_DUMMY1_get_write_enable2(data)                                           ((0x80000000&(data))>>31)
#define MIS_DUMMY1_rvd2_shift                                                        (16)
#define MIS_DUMMY1_rvd2_mask                                                         (0x7FFF0000)
#define MIS_DUMMY1_rvd2(data)                                                        (0x7FFF0000&((data)<<16))
#define MIS_DUMMY1_rvd2_src(data)                                                    ((0x7FFF0000&(data))>>16)
#define MIS_DUMMY1_get_rvd2(data)                                                    ((0x7FFF0000&(data))>>16)
#define MIS_DUMMY1_write_enable1_shift                                               (15)
#define MIS_DUMMY1_write_enable1_mask                                                (0x00008000)
#define MIS_DUMMY1_write_enable1(data)                                               (0x00008000&((data)<<15))
#define MIS_DUMMY1_write_enable1_src(data)                                           ((0x00008000&(data))>>15)
#define MIS_DUMMY1_get_write_enable1(data)                                           ((0x00008000&(data))>>15)
#define MIS_DUMMY1_rvd1_shift                                                        (0)
#define MIS_DUMMY1_rvd1_mask                                                         (0x00007FFF)
#define MIS_DUMMY1_rvd1(data)                                                        (0x00007FFF&((data)<<0))
#define MIS_DUMMY1_rvd1_src(data)                                                    ((0x00007FFF&(data))>>0)
#define MIS_DUMMY1_get_rvd1(data)                                                    ((0x00007FFF&(data))>>0)


#define MIS_UR_CTRL                                                                  0x1801B070
#define MIS_UR_CTRL_reg_addr                                                         "0xB801B070"
#define MIS_UR_CTRL_reg                                                              0xB801B070
#define set_MIS_UR_CTRL_reg(data)   (*((volatile unsigned int*) MIS_UR_CTRL_reg)=data)
#define get_MIS_UR_CTRL_reg   (*((volatile unsigned int*) MIS_UR_CTRL_reg))
#define MIS_UR_CTRL_inst_adr                                                         "0x001C"
#define MIS_UR_CTRL_inst                                                             0x001C
#define MIS_UR_CTRL_rvd3_shift                                                       (28)
#define MIS_UR_CTRL_rvd3_mask                                                        (0xF0000000)
#define MIS_UR_CTRL_rvd3(data)                                                       (0xF0000000&((data)<<28))
#define MIS_UR_CTRL_rvd3_src(data)                                                   ((0xF0000000&(data))>>28)
#define MIS_UR_CTRL_get_rvd3(data)                                                   ((0xF0000000&(data))>>28)
#define MIS_UR_CTRL_to_len_shift                                                     (20)
#define MIS_UR_CTRL_to_len_mask                                                      (0x0FF00000)
#define MIS_UR_CTRL_to_len(data)                                                     (0x0FF00000&((data)<<20))
#define MIS_UR_CTRL_to_len_src(data)                                                 ((0x0FF00000&(data))>>20)
#define MIS_UR_CTRL_get_to_len(data)                                                 ((0x0FF00000&(data))>>20)
#define MIS_UR_CTRL_to_int_en_shift                                                  (19)
#define MIS_UR_CTRL_to_int_en_mask                                                   (0x00080000)
#define MIS_UR_CTRL_to_int_en(data)                                                  (0x00080000&((data)<<19))
#define MIS_UR_CTRL_to_int_en_src(data)                                              ((0x00080000&(data))>>19)
#define MIS_UR_CTRL_get_to_int_en(data)                                              ((0x00080000&(data))>>19)
#define MIS_UR_CTRL_toauareq_shift                                                   (18)
#define MIS_UR_CTRL_toauareq_mask                                                    (0x00040000)
#define MIS_UR_CTRL_toauareq(data)                                                   (0x00040000&((data)<<18))
#define MIS_UR_CTRL_toauareq_src(data)                                               ((0x00040000&(data))>>18)
#define MIS_UR_CTRL_get_toauareq(data)                                               ((0x00040000&(data))>>18)
#define MIS_UR_CTRL_rvd1_shift                                                       (17)
#define MIS_UR_CTRL_rvd1_mask                                                        (0x00020000)
#define MIS_UR_CTRL_rvd1(data)                                                       (0x00020000&((data)<<17))
#define MIS_UR_CTRL_rvd1_src(data)                                                   ((0x00020000&(data))>>17)
#define MIS_UR_CTRL_get_rvd1(data)                                                   ((0x00020000&(data))>>17)
#define MIS_UR_CTRL_mdifen_shift                                                     (16)
#define MIS_UR_CTRL_mdifen_mask                                                      (0x00010000)
#define MIS_UR_CTRL_mdifen(data)                                                     (0x00010000&((data)<<16))
#define MIS_UR_CTRL_mdifen_src(data)                                                 ((0x00010000&(data))>>16)
#define MIS_UR_CTRL_get_mdifen(data)                                                 ((0x00010000&(data))>>16)
#define MIS_UR_CTRL_txempthr_shift                                                   (8)
#define MIS_UR_CTRL_txempthr_mask                                                    (0x00003F00)
#define MIS_UR_CTRL_txempthr(data)                                                   (0x00003F00&((data)<<8))
#define MIS_UR_CTRL_txempthr_src(data)                                               ((0x00003F00&(data))>>8)
#define MIS_UR_CTRL_get_txempthr(data)                                               ((0x00003F00&(data))>>8)
#define MIS_UR_CTRL_rxfulthr_shift                                                   (0)
#define MIS_UR_CTRL_rxfulthr_mask                                                    (0x0000003F)
#define MIS_UR_CTRL_rxfulthr(data)                                                   (0x0000003F&((data)<<0))
#define MIS_UR_CTRL_rxfulthr_src(data)                                               ((0x0000003F&(data))>>0)
#define MIS_UR_CTRL_get_rxfulthr(data)                                               ((0x0000003F&(data))>>0)


#define MIS_UR_STS                                                                   0x1801B074
#define MIS_UR_STS_reg_addr                                                          "0xB801B074"
#define MIS_UR_STS_reg                                                               0xB801B074
#define set_MIS_UR_STS_reg(data)   (*((volatile unsigned int*) MIS_UR_STS_reg)=data)
#define get_MIS_UR_STS_reg   (*((volatile unsigned int*) MIS_UR_STS_reg))
#define MIS_UR_STS_inst_adr                                                          "0x001D"
#define MIS_UR_STS_inst                                                              0x001D
#define MIS_UR_STS_rx_wrd_count_shift                                                (8)
#define MIS_UR_STS_rx_wrd_count_mask                                                 (0x00001F00)
#define MIS_UR_STS_rx_wrd_count(data)                                                (0x00001F00&((data)<<8))
#define MIS_UR_STS_rx_wrd_count_src(data)                                            ((0x00001F00&(data))>>8)
#define MIS_UR_STS_get_rx_wrd_count(data)                                            ((0x00001F00&(data))>>8)
#define MIS_UR_STS_tx_wrd_count_shift                                                (0)
#define MIS_UR_STS_tx_wrd_count_mask                                                 (0x0000001F)
#define MIS_UR_STS_tx_wrd_count(data)                                                (0x0000001F&((data)<<0))
#define MIS_UR_STS_tx_wrd_count_src(data)                                            ((0x0000001F&(data))>>0)
#define MIS_UR_STS_get_tx_wrd_count(data)                                            ((0x0000001F&(data))>>0)


#define MIS_SCPU_INT_EN                                                              0x1801B080
#define MIS_SCPU_INT_EN_reg_addr                                                     "0xB801B080"
#define MIS_SCPU_INT_EN_reg                                                          0xB801B080
#define set_MIS_SCPU_INT_EN_reg(data)   (*((volatile unsigned int*) MIS_SCPU_INT_EN_reg)=data)
#define get_MIS_SCPU_INT_EN_reg   (*((volatile unsigned int*) MIS_SCPU_INT_EN_reg))
#define MIS_SCPU_INT_EN_inst_adr                                                     "0x0020"
#define MIS_SCPU_INT_EN_inst                                                         0x0020
#define MIS_SCPU_INT_EN_gspi_shift                                                   (27)
#define MIS_SCPU_INT_EN_gspi_mask                                                    (0x08000000)
#define MIS_SCPU_INT_EN_gspi(data)                                                   (0x08000000&((data)<<27))
#define MIS_SCPU_INT_EN_gspi_src(data)                                               ((0x08000000&(data))>>27)
#define MIS_SCPU_INT_EN_get_gspi(data)                                               ((0x08000000&(data))>>27)
#define MIS_SCPU_INT_EN_i2c1_shift                                                   (26)
#define MIS_SCPU_INT_EN_i2c1_mask                                                    (0x04000000)
#define MIS_SCPU_INT_EN_i2c1(data)                                                   (0x04000000&((data)<<26))
#define MIS_SCPU_INT_EN_i2c1_src(data)                                               ((0x04000000&(data))>>26)
#define MIS_SCPU_INT_EN_get_i2c1(data)                                               ((0x04000000&(data))>>26)
#define MIS_SCPU_INT_EN_sc1_shift                                                    (25)
#define MIS_SCPU_INT_EN_sc1_mask                                                     (0x02000000)
#define MIS_SCPU_INT_EN_sc1(data)                                                    (0x02000000&((data)<<25))
#define MIS_SCPU_INT_EN_sc1_src(data)                                                ((0x02000000&(data))>>25)
#define MIS_SCPU_INT_EN_get_sc1(data)                                                ((0x02000000&(data))>>25)
#define MIS_SCPU_INT_EN_sc0_shift                                                    (24)
#define MIS_SCPU_INT_EN_sc0_mask                                                     (0x01000000)
#define MIS_SCPU_INT_EN_sc0(data)                                                    (0x01000000&((data)<<24))
#define MIS_SCPU_INT_EN_sc0_src(data)                                                ((0x01000000&(data))>>24)
#define MIS_SCPU_INT_EN_get_sc0(data)                                                ((0x01000000&(data))>>24)
#define MIS_SCPU_INT_EN_pcmcia_shift                                                 (21)
#define MIS_SCPU_INT_EN_pcmcia_mask                                                  (0x00200000)
#define MIS_SCPU_INT_EN_pcmcia(data)                                                 (0x00200000&((data)<<21))
#define MIS_SCPU_INT_EN_pcmcia_src(data)                                             ((0x00200000&(data))>>21)
#define MIS_SCPU_INT_EN_get_pcmcia(data)                                             ((0x00200000&(data))>>21)
#define MIS_SCPU_INT_EN_gpioda_shift                                                 (20)
#define MIS_SCPU_INT_EN_gpioda_mask                                                  (0x00100000)
#define MIS_SCPU_INT_EN_gpioda(data)                                                 (0x00100000&((data)<<20))
#define MIS_SCPU_INT_EN_gpioda_src(data)                                             ((0x00100000&(data))>>20)
#define MIS_SCPU_INT_EN_get_gpioda(data)                                             ((0x00100000&(data))>>20)
#define MIS_SCPU_INT_EN_gpioa_shift                                                  (19)
#define MIS_SCPU_INT_EN_gpioa_mask                                                   (0x00080000)
#define MIS_SCPU_INT_EN_gpioa(data)                                                  (0x00080000&((data)<<19))
#define MIS_SCPU_INT_EN_gpioa_src(data)                                              ((0x00080000&(data))>>19)
#define MIS_SCPU_INT_EN_get_gpioa(data)                                              ((0x00080000&(data))>>19)
#define MIS_SCPU_INT_EN_rtc_date_shift                                               (12)
#define MIS_SCPU_INT_EN_rtc_date_mask                                                (0x00001000)
#define MIS_SCPU_INT_EN_rtc_date(data)                                               (0x00001000&((data)<<12))
#define MIS_SCPU_INT_EN_rtc_date_src(data)                                           ((0x00001000&(data))>>12)
#define MIS_SCPU_INT_EN_get_rtc_date(data)                                           ((0x00001000&(data))>>12)
#define MIS_SCPU_INT_EN_rtc_hour_shift                                               (11)
#define MIS_SCPU_INT_EN_rtc_hour_mask                                                (0x00000800)
#define MIS_SCPU_INT_EN_rtc_hour(data)                                               (0x00000800&((data)<<11))
#define MIS_SCPU_INT_EN_rtc_hour_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_SCPU_INT_EN_get_rtc_hour(data)                                           ((0x00000800&(data))>>11)
#define MIS_SCPU_INT_EN_rtc_min_shift                                                (10)
#define MIS_SCPU_INT_EN_rtc_min_mask                                                 (0x00000400)
#define MIS_SCPU_INT_EN_rtc_min(data)                                                (0x00000400&((data)<<10))
#define MIS_SCPU_INT_EN_rtc_min_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_SCPU_INT_EN_get_rtc_min(data)                                            ((0x00000400&(data))>>10)
#define MIS_SCPU_INT_EN_rtc_hsec_shift                                               (9)
#define MIS_SCPU_INT_EN_rtc_hsec_mask                                                (0x00000200)
#define MIS_SCPU_INT_EN_rtc_hsec(data)                                               (0x00000200&((data)<<9))
#define MIS_SCPU_INT_EN_rtc_hsec_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_SCPU_INT_EN_get_rtc_hsec(data)                                           ((0x00000200&(data))>>9)
#define MIS_SCPU_INT_EN_tc2_shift                                                    (8)
#define MIS_SCPU_INT_EN_tc2_mask                                                     (0x00000100)
#define MIS_SCPU_INT_EN_tc2(data)                                                    (0x00000100&((data)<<8))
#define MIS_SCPU_INT_EN_tc2_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_SCPU_INT_EN_get_tc2(data)                                                ((0x00000100&(data))>>8)
#define MIS_SCPU_INT_EN_tc1_shift                                                    (7)
#define MIS_SCPU_INT_EN_tc1_mask                                                     (0x00000080)
#define MIS_SCPU_INT_EN_tc1(data)                                                    (0x00000080&((data)<<7))
#define MIS_SCPU_INT_EN_tc1_src(data)                                                ((0x00000080&(data))>>7)
#define MIS_SCPU_INT_EN_get_tc1(data)                                                ((0x00000080&(data))>>7)
#define MIS_SCPU_INT_EN_tc0_shift                                                    (6)
#define MIS_SCPU_INT_EN_tc0_mask                                                     (0x00000040)
#define MIS_SCPU_INT_EN_tc0(data)                                                    (0x00000040&((data)<<6))
#define MIS_SCPU_INT_EN_tc0_src(data)                                                ((0x00000040&(data))>>6)
#define MIS_SCPU_INT_EN_get_tc0(data)                                                ((0x00000040&(data))>>6)
#define MIS_SCPU_INT_EN_i2c0_shift                                                   (4)
#define MIS_SCPU_INT_EN_i2c0_mask                                                    (0x00000010)
#define MIS_SCPU_INT_EN_i2c0(data)                                                   (0x00000010&((data)<<4))
#define MIS_SCPU_INT_EN_i2c0_src(data)                                               ((0x00000010&(data))>>4)
#define MIS_SCPU_INT_EN_get_i2c0(data)                                               ((0x00000010&(data))>>4)
#define MIS_SCPU_INT_EN_ur1_shift                                                    (3)
#define MIS_SCPU_INT_EN_ur1_mask                                                     (0x00000008)
#define MIS_SCPU_INT_EN_ur1(data)                                                    (0x00000008&((data)<<3))
#define MIS_SCPU_INT_EN_ur1_src(data)                                                ((0x00000008&(data))>>3)
#define MIS_SCPU_INT_EN_get_ur1(data)                                                ((0x00000008&(data))>>3)
#define MIS_SCPU_INT_EN_ur1_to_shift                                                 (1)
#define MIS_SCPU_INT_EN_ur1_to_mask                                                  (0x00000002)
#define MIS_SCPU_INT_EN_ur1_to(data)                                                 (0x00000002&((data)<<1))
#define MIS_SCPU_INT_EN_ur1_to_src(data)                                             ((0x00000002&(data))>>1)
#define MIS_SCPU_INT_EN_get_ur1_to(data)                                             ((0x00000002&(data))>>1)


#define MIS_KCPU_INT_EN                                                              0x1801B084
#define MIS_KCPU_INT_EN_reg_addr                                                     "0xB801B084"
#define MIS_KCPU_INT_EN_reg                                                          0xB801B084
#define set_MIS_KCPU_INT_EN_reg(data)   (*((volatile unsigned int*) MIS_KCPU_INT_EN_reg)=data)
#define get_MIS_KCPU_INT_EN_reg   (*((volatile unsigned int*) MIS_KCPU_INT_EN_reg))
#define MIS_KCPU_INT_EN_inst_adr                                                     "0x0021"
#define MIS_KCPU_INT_EN_inst                                                         0x0021
#define MIS_KCPU_INT_EN_gspi_shift                                                   (27)
#define MIS_KCPU_INT_EN_gspi_mask                                                    (0x08000000)
#define MIS_KCPU_INT_EN_gspi(data)                                                   (0x08000000&((data)<<27))
#define MIS_KCPU_INT_EN_gspi_src(data)                                               ((0x08000000&(data))>>27)
#define MIS_KCPU_INT_EN_get_gspi(data)                                               ((0x08000000&(data))>>27)
#define MIS_KCPU_INT_EN_i2c1_shift                                                   (26)
#define MIS_KCPU_INT_EN_i2c1_mask                                                    (0x04000000)
#define MIS_KCPU_INT_EN_i2c1(data)                                                   (0x04000000&((data)<<26))
#define MIS_KCPU_INT_EN_i2c1_src(data)                                               ((0x04000000&(data))>>26)
#define MIS_KCPU_INT_EN_get_i2c1(data)                                               ((0x04000000&(data))>>26)
#define MIS_KCPU_INT_EN_sc1_shift                                                    (25)
#define MIS_KCPU_INT_EN_sc1_mask                                                     (0x02000000)
#define MIS_KCPU_INT_EN_sc1(data)                                                    (0x02000000&((data)<<25))
#define MIS_KCPU_INT_EN_sc1_src(data)                                                ((0x02000000&(data))>>25)
#define MIS_KCPU_INT_EN_get_sc1(data)                                                ((0x02000000&(data))>>25)
#define MIS_KCPU_INT_EN_sc0_shift                                                    (24)
#define MIS_KCPU_INT_EN_sc0_mask                                                     (0x01000000)
#define MIS_KCPU_INT_EN_sc0(data)                                                    (0x01000000&((data)<<24))
#define MIS_KCPU_INT_EN_sc0_src(data)                                                ((0x01000000&(data))>>24)
#define MIS_KCPU_INT_EN_get_sc0(data)                                                ((0x01000000&(data))>>24)
#define MIS_KCPU_INT_EN_pcmcia_shift                                                 (21)
#define MIS_KCPU_INT_EN_pcmcia_mask                                                  (0x00200000)
#define MIS_KCPU_INT_EN_pcmcia(data)                                                 (0x00200000&((data)<<21))
#define MIS_KCPU_INT_EN_pcmcia_src(data)                                             ((0x00200000&(data))>>21)
#define MIS_KCPU_INT_EN_get_pcmcia(data)                                             ((0x00200000&(data))>>21)
#define MIS_KCPU_INT_EN_gpioda_shift                                                 (20)
#define MIS_KCPU_INT_EN_gpioda_mask                                                  (0x00100000)
#define MIS_KCPU_INT_EN_gpioda(data)                                                 (0x00100000&((data)<<20))
#define MIS_KCPU_INT_EN_gpioda_src(data)                                             ((0x00100000&(data))>>20)
#define MIS_KCPU_INT_EN_get_gpioda(data)                                             ((0x00100000&(data))>>20)
#define MIS_KCPU_INT_EN_gpioa_shift                                                  (19)
#define MIS_KCPU_INT_EN_gpioa_mask                                                   (0x00080000)
#define MIS_KCPU_INT_EN_gpioa(data)                                                  (0x00080000&((data)<<19))
#define MIS_KCPU_INT_EN_gpioa_src(data)                                              ((0x00080000&(data))>>19)
#define MIS_KCPU_INT_EN_get_gpioa(data)                                              ((0x00080000&(data))>>19)
#define MIS_KCPU_INT_EN_rtc_date_shift                                               (12)
#define MIS_KCPU_INT_EN_rtc_date_mask                                                (0x00001000)
#define MIS_KCPU_INT_EN_rtc_date(data)                                               (0x00001000&((data)<<12))
#define MIS_KCPU_INT_EN_rtc_date_src(data)                                           ((0x00001000&(data))>>12)
#define MIS_KCPU_INT_EN_get_rtc_date(data)                                           ((0x00001000&(data))>>12)
#define MIS_KCPU_INT_EN_rtc_hour_shift                                               (11)
#define MIS_KCPU_INT_EN_rtc_hour_mask                                                (0x00000800)
#define MIS_KCPU_INT_EN_rtc_hour(data)                                               (0x00000800&((data)<<11))
#define MIS_KCPU_INT_EN_rtc_hour_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_KCPU_INT_EN_get_rtc_hour(data)                                           ((0x00000800&(data))>>11)
#define MIS_KCPU_INT_EN_rtc_min_shift                                                (10)
#define MIS_KCPU_INT_EN_rtc_min_mask                                                 (0x00000400)
#define MIS_KCPU_INT_EN_rtc_min(data)                                                (0x00000400&((data)<<10))
#define MIS_KCPU_INT_EN_rtc_min_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_KCPU_INT_EN_get_rtc_min(data)                                            ((0x00000400&(data))>>10)
#define MIS_KCPU_INT_EN_rtc_hsec_shift                                               (9)
#define MIS_KCPU_INT_EN_rtc_hsec_mask                                                (0x00000200)
#define MIS_KCPU_INT_EN_rtc_hsec(data)                                               (0x00000200&((data)<<9))
#define MIS_KCPU_INT_EN_rtc_hsec_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_KCPU_INT_EN_get_rtc_hsec(data)                                           ((0x00000200&(data))>>9)
#define MIS_KCPU_INT_EN_tc2_shift                                                    (8)
#define MIS_KCPU_INT_EN_tc2_mask                                                     (0x00000100)
#define MIS_KCPU_INT_EN_tc2(data)                                                    (0x00000100&((data)<<8))
#define MIS_KCPU_INT_EN_tc2_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_KCPU_INT_EN_get_tc2(data)                                                ((0x00000100&(data))>>8)
#define MIS_KCPU_INT_EN_tc1_shift                                                    (7)
#define MIS_KCPU_INT_EN_tc1_mask                                                     (0x00000080)
#define MIS_KCPU_INT_EN_tc1(data)                                                    (0x00000080&((data)<<7))
#define MIS_KCPU_INT_EN_tc1_src(data)                                                ((0x00000080&(data))>>7)
#define MIS_KCPU_INT_EN_get_tc1(data)                                                ((0x00000080&(data))>>7)
#define MIS_KCPU_INT_EN_tc0_shift                                                    (6)
#define MIS_KCPU_INT_EN_tc0_mask                                                     (0x00000040)
#define MIS_KCPU_INT_EN_tc0(data)                                                    (0x00000040&((data)<<6))
#define MIS_KCPU_INT_EN_tc0_src(data)                                                ((0x00000040&(data))>>6)
#define MIS_KCPU_INT_EN_get_tc0(data)                                                ((0x00000040&(data))>>6)
#define MIS_KCPU_INT_EN_i2c0_shift                                                   (4)
#define MIS_KCPU_INT_EN_i2c0_mask                                                    (0x00000010)
#define MIS_KCPU_INT_EN_i2c0(data)                                                   (0x00000010&((data)<<4))
#define MIS_KCPU_INT_EN_i2c0_src(data)                                               ((0x00000010&(data))>>4)
#define MIS_KCPU_INT_EN_get_i2c0(data)                                               ((0x00000010&(data))>>4)
#define MIS_KCPU_INT_EN_ur1_shift                                                    (3)
#define MIS_KCPU_INT_EN_ur1_mask                                                     (0x00000008)
#define MIS_KCPU_INT_EN_ur1(data)                                                    (0x00000008&((data)<<3))
#define MIS_KCPU_INT_EN_ur1_src(data)                                                ((0x00000008&(data))>>3)
#define MIS_KCPU_INT_EN_get_ur1(data)                                                ((0x00000008&(data))>>3)
#define MIS_KCPU_INT_EN_ur1_to_shift                                                 (1)
#define MIS_KCPU_INT_EN_ur1_to_mask                                                  (0x00000002)
#define MIS_KCPU_INT_EN_ur1_to(data)                                                 (0x00000002&((data)<<1))
#define MIS_KCPU_INT_EN_ur1_to_src(data)                                             ((0x00000002&(data))>>1)
#define MIS_KCPU_INT_EN_get_ur1_to(data)                                             ((0x00000002&(data))>>1)


#define MIS_I2C0_SDA_DEL                                                             0x1801B088
#define MIS_I2C0_SDA_DEL_reg_addr                                                    "0xB801B088"
#define MIS_I2C0_SDA_DEL_reg                                                         0xB801B088
#define set_MIS_I2C0_SDA_DEL_reg(data)   (*((volatile unsigned int*) MIS_I2C0_SDA_DEL_reg)=data)
#define get_MIS_I2C0_SDA_DEL_reg   (*((volatile unsigned int*) MIS_I2C0_SDA_DEL_reg))
#define MIS_I2C0_SDA_DEL_inst_adr                                                    "0x0022"
#define MIS_I2C0_SDA_DEL_inst                                                        0x0022
#define MIS_I2C0_SDA_DEL_dmy1_shift                                                  (12)
#define MIS_I2C0_SDA_DEL_dmy1_mask                                                   (0xFFFFF000)
#define MIS_I2C0_SDA_DEL_dmy1(data)                                                  (0xFFFFF000&((data)<<12))
#define MIS_I2C0_SDA_DEL_dmy1_src(data)                                              ((0xFFFFF000&(data))>>12)
#define MIS_I2C0_SDA_DEL_get_dmy1(data)                                              ((0xFFFFF000&(data))>>12)
#define MIS_I2C0_SDA_DEL_en_shift                                                    (8)
#define MIS_I2C0_SDA_DEL_en_mask                                                     (0x00000100)
#define MIS_I2C0_SDA_DEL_en(data)                                                    (0x00000100&((data)<<8))
#define MIS_I2C0_SDA_DEL_en_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_I2C0_SDA_DEL_get_en(data)                                                ((0x00000100&(data))>>8)
#define MIS_I2C0_SDA_DEL_sel_shift                                                   (0)
#define MIS_I2C0_SDA_DEL_sel_mask                                                    (0x0000001F)
#define MIS_I2C0_SDA_DEL_sel(data)                                                   (0x0000001F&((data)<<0))
#define MIS_I2C0_SDA_DEL_sel_src(data)                                               ((0x0000001F&(data))>>0)
#define MIS_I2C0_SDA_DEL_get_sel(data)                                               ((0x0000001F&(data))>>0)


#define MIS_I2C1_SDA_DEL                                                             0x1801B08c
#define MIS_I2C1_SDA_DEL_reg_addr                                                    "0xB801B08C"
#define MIS_I2C1_SDA_DEL_reg                                                         0xB801B08C
#define set_MIS_I2C1_SDA_DEL_reg(data)   (*((volatile unsigned int*) MIS_I2C1_SDA_DEL_reg)=data)
#define get_MIS_I2C1_SDA_DEL_reg   (*((volatile unsigned int*) MIS_I2C1_SDA_DEL_reg))
#define MIS_I2C1_SDA_DEL_inst_adr                                                    "0x0023"
#define MIS_I2C1_SDA_DEL_inst                                                        0x0023
#define MIS_I2C1_SDA_DEL_dmy1_shift                                                  (12)
#define MIS_I2C1_SDA_DEL_dmy1_mask                                                   (0xFFFFF000)
#define MIS_I2C1_SDA_DEL_dmy1(data)                                                  (0xFFFFF000&((data)<<12))
#define MIS_I2C1_SDA_DEL_dmy1_src(data)                                              ((0xFFFFF000&(data))>>12)
#define MIS_I2C1_SDA_DEL_get_dmy1(data)                                              ((0xFFFFF000&(data))>>12)
#define MIS_I2C1_SDA_DEL_en_shift                                                    (8)
#define MIS_I2C1_SDA_DEL_en_mask                                                     (0x00000100)
#define MIS_I2C1_SDA_DEL_en(data)                                                    (0x00000100&((data)<<8))
#define MIS_I2C1_SDA_DEL_en_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_I2C1_SDA_DEL_get_en(data)                                                ((0x00000100&(data))>>8)
#define MIS_I2C1_SDA_DEL_sel_shift                                                   (0)
#define MIS_I2C1_SDA_DEL_sel_mask                                                    (0x0000001F)
#define MIS_I2C1_SDA_DEL_sel(data)                                                   (0x0000001F&((data)<<0))
#define MIS_I2C1_SDA_DEL_sel_src(data)                                               ((0x0000001F&(data))>>0)
#define MIS_I2C1_SDA_DEL_get_sel(data)                                               ((0x0000001F&(data))>>0)


#define MIS_GP0DIR                                                                   0x1801B100
#define MIS_GP0DIR_reg_addr                                                          "0xB801B100"
#define MIS_GP0DIR_reg                                                               0xB801B100
#define set_MIS_GP0DIR_reg(data)   (*((volatile unsigned int*) MIS_GP0DIR_reg)=data)
#define get_MIS_GP0DIR_reg   (*((volatile unsigned int*) MIS_GP0DIR_reg))
#define MIS_GP0DIR_inst_adr                                                          "0x0040"
#define MIS_GP0DIR_inst                                                              0x0040
#define MIS_GP0DIR_gpdir_shift                                                       (0)
#define MIS_GP0DIR_gpdir_mask                                                        (0xFFFFFFFF)
#define MIS_GP0DIR_gpdir(data)                                                       (0xFFFFFFFF&((data)<<0))
#define MIS_GP0DIR_gpdir_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define MIS_GP0DIR_get_gpdir(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define MIS_GP1DIR                                                                   0x1801B104
#define MIS_GP1DIR_reg_addr                                                          "0xB801B104"
#define MIS_GP1DIR_reg                                                               0xB801B104
#define set_MIS_GP1DIR_reg(data)   (*((volatile unsigned int*) MIS_GP1DIR_reg)=data)
#define get_MIS_GP1DIR_reg   (*((volatile unsigned int*) MIS_GP1DIR_reg))
#define MIS_GP1DIR_inst_adr                                                          "0x0041"
#define MIS_GP1DIR_inst                                                              0x0041
#define MIS_GP1DIR_gpdir_shift                                                       (0)
#define MIS_GP1DIR_gpdir_mask                                                        (0xFFFFFFFF)
#define MIS_GP1DIR_gpdir(data)                                                       (0xFFFFFFFF&((data)<<0))
#define MIS_GP1DIR_gpdir_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define MIS_GP1DIR_get_gpdir(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define MIS_GP2DIR                                                                   0x1801B108
#define MIS_GP2DIR_reg_addr                                                          "0xB801B108"
#define MIS_GP2DIR_reg                                                               0xB801B108
#define set_MIS_GP2DIR_reg(data)   (*((volatile unsigned int*) MIS_GP2DIR_reg)=data)
#define get_MIS_GP2DIR_reg   (*((volatile unsigned int*) MIS_GP2DIR_reg))
#define MIS_GP2DIR_inst_adr                                                          "0x0042"
#define MIS_GP2DIR_inst                                                              0x0042
#define MIS_GP2DIR_gpdir_shift                                                       (0)
#define MIS_GP2DIR_gpdir_mask                                                        (0xFFFFFFFF)
#define MIS_GP2DIR_gpdir(data)                                                       (0xFFFFFFFF&((data)<<0))
#define MIS_GP2DIR_gpdir_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define MIS_GP2DIR_get_gpdir(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define MIS_GP3DIR                                                                   0x1801B10C
#define MIS_GP3DIR_reg_addr                                                          "0xB801B10C"
#define MIS_GP3DIR_reg                                                               0xB801B10C
#define set_MIS_GP3DIR_reg(data)   (*((volatile unsigned int*) MIS_GP3DIR_reg)=data)
#define get_MIS_GP3DIR_reg   (*((volatile unsigned int*) MIS_GP3DIR_reg))
#define MIS_GP3DIR_inst_adr                                                          "0x0043"
#define MIS_GP3DIR_inst                                                              0x0043
#define MIS_GP3DIR_gpdir_shift                                                       (0)
#define MIS_GP3DIR_gpdir_mask                                                        (0xFFFFFFFF)
#define MIS_GP3DIR_gpdir(data)                                                       (0xFFFFFFFF&((data)<<0))
#define MIS_GP3DIR_gpdir_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define MIS_GP3DIR_get_gpdir(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define MIS_GP0DATO                                                                  0x1801B110
#define MIS_GP0DATO_reg_addr                                                         "0xB801B110"
#define MIS_GP0DATO_reg                                                              0xB801B110
#define set_MIS_GP0DATO_reg(data)   (*((volatile unsigned int*) MIS_GP0DATO_reg)=data)
#define get_MIS_GP0DATO_reg   (*((volatile unsigned int*) MIS_GP0DATO_reg))
#define MIS_GP0DATO_inst_adr                                                         "0x0044"
#define MIS_GP0DATO_inst                                                             0x0044
#define MIS_GP0DATO_gpdato_shift                                                     (0)
#define MIS_GP0DATO_gpdato_mask                                                      (0xFFFFFFFF)
#define MIS_GP0DATO_gpdato(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP0DATO_gpdato_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP0DATO_get_gpdato(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP1DATO                                                                  0x1801B114
#define MIS_GP1DATO_reg_addr                                                         "0xB801B114"
#define MIS_GP1DATO_reg                                                              0xB801B114
#define set_MIS_GP1DATO_reg(data)   (*((volatile unsigned int*) MIS_GP1DATO_reg)=data)
#define get_MIS_GP1DATO_reg   (*((volatile unsigned int*) MIS_GP1DATO_reg))
#define MIS_GP1DATO_inst_adr                                                         "0x0045"
#define MIS_GP1DATO_inst                                                             0x0045
#define MIS_GP1DATO_gpdato_shift                                                     (0)
#define MIS_GP1DATO_gpdato_mask                                                      (0xFFFFFFFF)
#define MIS_GP1DATO_gpdato(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP1DATO_gpdato_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP1DATO_get_gpdato(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP2DATO                                                                  0x1801B118
#define MIS_GP2DATO_reg_addr                                                         "0xB801B118"
#define MIS_GP2DATO_reg                                                              0xB801B118
#define set_MIS_GP2DATO_reg(data)   (*((volatile unsigned int*) MIS_GP2DATO_reg)=data)
#define get_MIS_GP2DATO_reg   (*((volatile unsigned int*) MIS_GP2DATO_reg))
#define MIS_GP2DATO_inst_adr                                                         "0x0046"
#define MIS_GP2DATO_inst                                                             0x0046
#define MIS_GP2DATO_gpdato_shift                                                     (0)
#define MIS_GP2DATO_gpdato_mask                                                      (0xFFFFFFFF)
#define MIS_GP2DATO_gpdato(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP2DATO_gpdato_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP2DATO_get_gpdato(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP3DATO                                                                  0x1801B11C
#define MIS_GP3DATO_reg_addr                                                         "0xB801B11C"
#define MIS_GP3DATO_reg                                                              0xB801B11C
#define set_MIS_GP3DATO_reg(data)   (*((volatile unsigned int*) MIS_GP3DATO_reg)=data)
#define get_MIS_GP3DATO_reg   (*((volatile unsigned int*) MIS_GP3DATO_reg))
#define MIS_GP3DATO_inst_adr                                                         "0x0047"
#define MIS_GP3DATO_inst                                                             0x0047
#define MIS_GP3DATO_gpdato_shift                                                     (0)
#define MIS_GP3DATO_gpdato_mask                                                      (0xFFFFFFFF)
#define MIS_GP3DATO_gpdato(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP3DATO_gpdato_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP3DATO_get_gpdato(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP0DATI                                                                  0x1801B120
#define MIS_GP0DATI_reg_addr                                                         "0xB801B120"
#define MIS_GP0DATI_reg                                                              0xB801B120
#define set_MIS_GP0DATI_reg(data)   (*((volatile unsigned int*) MIS_GP0DATI_reg)=data)
#define get_MIS_GP0DATI_reg   (*((volatile unsigned int*) MIS_GP0DATI_reg))
#define MIS_GP0DATI_inst_adr                                                         "0x0048"
#define MIS_GP0DATI_inst                                                             0x0048
#define MIS_GP0DATI_gpdati_shift                                                     (0)
#define MIS_GP0DATI_gpdati_mask                                                      (0xFFFFFFFF)
#define MIS_GP0DATI_gpdati(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP0DATI_gpdati_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP0DATI_get_gpdati(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP1DATI                                                                  0x1801B124
#define MIS_GP1DATI_reg_addr                                                         "0xB801B124"
#define MIS_GP1DATI_reg                                                              0xB801B124
#define set_MIS_GP1DATI_reg(data)   (*((volatile unsigned int*) MIS_GP1DATI_reg)=data)
#define get_MIS_GP1DATI_reg   (*((volatile unsigned int*) MIS_GP1DATI_reg))
#define MIS_GP1DATI_inst_adr                                                         "0x0049"
#define MIS_GP1DATI_inst                                                             0x0049
#define MIS_GP1DATI_gpdati_shift                                                     (0)
#define MIS_GP1DATI_gpdati_mask                                                      (0xFFFFFFFF)
#define MIS_GP1DATI_gpdati(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP1DATI_gpdati_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP1DATI_get_gpdati(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP2DATI                                                                  0x1801B128
#define MIS_GP2DATI_reg_addr                                                         "0xB801B128"
#define MIS_GP2DATI_reg                                                              0xB801B128
#define set_MIS_GP2DATI_reg(data)   (*((volatile unsigned int*) MIS_GP2DATI_reg)=data)
#define get_MIS_GP2DATI_reg   (*((volatile unsigned int*) MIS_GP2DATI_reg))
#define MIS_GP2DATI_inst_adr                                                         "0x004A"
#define MIS_GP2DATI_inst                                                             0x004A
#define MIS_GP2DATI_gpdati_shift                                                     (0)
#define MIS_GP2DATI_gpdati_mask                                                      (0xFFFFFFFF)
#define MIS_GP2DATI_gpdati(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP2DATI_gpdati_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP2DATI_get_gpdati(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP3DATI                                                                  0x1801B12C
#define MIS_GP3DATI_reg_addr                                                         "0xB801B12C"
#define MIS_GP3DATI_reg                                                              0xB801B12C
#define set_MIS_GP3DATI_reg(data)   (*((volatile unsigned int*) MIS_GP3DATI_reg)=data)
#define get_MIS_GP3DATI_reg   (*((volatile unsigned int*) MIS_GP3DATI_reg))
#define MIS_GP3DATI_inst_adr                                                         "0x004B"
#define MIS_GP3DATI_inst                                                             0x004B
#define MIS_GP3DATI_gpdati_shift                                                     (0)
#define MIS_GP3DATI_gpdati_mask                                                      (0xFFFFFFFF)
#define MIS_GP3DATI_gpdati(data)                                                     (0xFFFFFFFF&((data)<<0))
#define MIS_GP3DATI_gpdati_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define MIS_GP3DATI_get_gpdati(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define MIS_GP0IE                                                                    0x1801B130
#define MIS_GP0IE_reg_addr                                                           "0xB801B130"
#define MIS_GP0IE_reg                                                                0xB801B130
#define set_MIS_GP0IE_reg(data)   (*((volatile unsigned int*) MIS_GP0IE_reg)=data)
#define get_MIS_GP0IE_reg   (*((volatile unsigned int*) MIS_GP0IE_reg))
#define MIS_GP0IE_inst_adr                                                           "0x004C"
#define MIS_GP0IE_inst                                                               0x004C
#define MIS_GP0IE_gp_shift                                                           (0)
#define MIS_GP0IE_gp_mask                                                            (0xFFFFFFFF)
#define MIS_GP0IE_gp(data)                                                           (0xFFFFFFFF&((data)<<0))
#define MIS_GP0IE_gp_src(data)                                                       ((0xFFFFFFFF&(data))>>0)
#define MIS_GP0IE_get_gp(data)                                                       ((0xFFFFFFFF&(data))>>0)


#define MIS_GP1IE                                                                    0x1801B134
#define MIS_GP1IE_reg_addr                                                           "0xB801B134"
#define MIS_GP1IE_reg                                                                0xB801B134
#define set_MIS_GP1IE_reg(data)   (*((volatile unsigned int*) MIS_GP1IE_reg)=data)
#define get_MIS_GP1IE_reg   (*((volatile unsigned int*) MIS_GP1IE_reg))
#define MIS_GP1IE_inst_adr                                                           "0x004D"
#define MIS_GP1IE_inst                                                               0x004D
#define MIS_GP1IE_gp_shift                                                           (0)
#define MIS_GP1IE_gp_mask                                                            (0xFFFFFFFF)
#define MIS_GP1IE_gp(data)                                                           (0xFFFFFFFF&((data)<<0))
#define MIS_GP1IE_gp_src(data)                                                       ((0xFFFFFFFF&(data))>>0)
#define MIS_GP1IE_get_gp(data)                                                       ((0xFFFFFFFF&(data))>>0)


#define MIS_GP2IE                                                                    0x1801B138
#define MIS_GP2IE_reg_addr                                                           "0xB801B138"
#define MIS_GP2IE_reg                                                                0xB801B138
#define set_MIS_GP2IE_reg(data)   (*((volatile unsigned int*) MIS_GP2IE_reg)=data)
#define get_MIS_GP2IE_reg   (*((volatile unsigned int*) MIS_GP2IE_reg))
#define MIS_GP2IE_inst_adr                                                           "0x004E"
#define MIS_GP2IE_inst                                                               0x004E
#define MIS_GP2IE_gp_shift                                                           (0)
#define MIS_GP2IE_gp_mask                                                            (0xFFFFFFFF)
#define MIS_GP2IE_gp(data)                                                           (0xFFFFFFFF&((data)<<0))
#define MIS_GP2IE_gp_src(data)                                                       ((0xFFFFFFFF&(data))>>0)
#define MIS_GP2IE_get_gp(data)                                                       ((0xFFFFFFFF&(data))>>0)


#define MIS_GP3IE                                                                    0x1801B13C
#define MIS_GP3IE_reg_addr                                                           "0xB801B13C"
#define MIS_GP3IE_reg                                                                0xB801B13C
#define set_MIS_GP3IE_reg(data)   (*((volatile unsigned int*) MIS_GP3IE_reg)=data)
#define get_MIS_GP3IE_reg   (*((volatile unsigned int*) MIS_GP3IE_reg))
#define MIS_GP3IE_inst_adr                                                           "0x004F"
#define MIS_GP3IE_inst                                                               0x004F
#define MIS_GP3IE_gp_shift                                                           (0)
#define MIS_GP3IE_gp_mask                                                            (0xFFFFFFFF)
#define MIS_GP3IE_gp(data)                                                           (0xFFFFFFFF&((data)<<0))
#define MIS_GP3IE_gp_src(data)                                                       ((0xFFFFFFFF&(data))>>0)
#define MIS_GP3IE_get_gp(data)                                                       ((0xFFFFFFFF&(data))>>0)


#define MIS_GP0DP                                                                    0x1801B140
#define MIS_GP0DP_reg_addr                                                           "0xB801B140"
#define MIS_GP0DP_reg                                                                0xB801B140
#define set_MIS_GP0DP_reg(data)   (*((volatile unsigned int*) MIS_GP0DP_reg)=data)
#define get_MIS_GP0DP_reg   (*((volatile unsigned int*) MIS_GP0DP_reg))
#define MIS_GP0DP_inst_adr                                                           "0x0050"
#define MIS_GP0DP_inst                                                               0x0050
#define MIS_GP0DP_gpha_shift                                                         (0)
#define MIS_GP0DP_gpha_mask                                                          (0xFFFFFFFF)
#define MIS_GP0DP_gpha(data)                                                         (0xFFFFFFFF&((data)<<0))
#define MIS_GP0DP_gpha_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define MIS_GP0DP_get_gpha(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define MIS_GP1DP                                                                    0x1801B144
#define MIS_GP1DP_reg_addr                                                           "0xB801B144"
#define MIS_GP1DP_reg                                                                0xB801B144
#define set_MIS_GP1DP_reg(data)   (*((volatile unsigned int*) MIS_GP1DP_reg)=data)
#define get_MIS_GP1DP_reg   (*((volatile unsigned int*) MIS_GP1DP_reg))
#define MIS_GP1DP_inst_adr                                                           "0x0051"
#define MIS_GP1DP_inst                                                               0x0051
#define MIS_GP1DP_gpha_shift                                                         (0)
#define MIS_GP1DP_gpha_mask                                                          (0xFFFFFFFF)
#define MIS_GP1DP_gpha(data)                                                         (0xFFFFFFFF&((data)<<0))
#define MIS_GP1DP_gpha_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define MIS_GP1DP_get_gpha(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define MIS_GP2DP                                                                    0x1801B148
#define MIS_GP2DP_reg_addr                                                           "0xB801B148"
#define MIS_GP2DP_reg                                                                0xB801B148
#define set_MIS_GP2DP_reg(data)   (*((volatile unsigned int*) MIS_GP2DP_reg)=data)
#define get_MIS_GP2DP_reg   (*((volatile unsigned int*) MIS_GP2DP_reg))
#define MIS_GP2DP_inst_adr                                                           "0x0052"
#define MIS_GP2DP_inst                                                               0x0052
#define MIS_GP2DP_gpha_shift                                                         (0)
#define MIS_GP2DP_gpha_mask                                                          (0xFFFFFFFF)
#define MIS_GP2DP_gpha(data)                                                         (0xFFFFFFFF&((data)<<0))
#define MIS_GP2DP_gpha_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define MIS_GP2DP_get_gpha(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define MIS_GP3DP                                                                    0x1801B14C
#define MIS_GP3DP_reg_addr                                                           "0xB801B14C"
#define MIS_GP3DP_reg                                                                0xB801B14C
#define set_MIS_GP3DP_reg(data)   (*((volatile unsigned int*) MIS_GP3DP_reg)=data)
#define get_MIS_GP3DP_reg   (*((volatile unsigned int*) MIS_GP3DP_reg))
#define MIS_GP3DP_inst_adr                                                           "0x0053"
#define MIS_GP3DP_inst                                                               0x0053
#define MIS_GP3DP_gpha_shift                                                         (0)
#define MIS_GP3DP_gpha_mask                                                          (0xFFFFFFFF)
#define MIS_GP3DP_gpha(data)                                                         (0xFFFFFFFF&((data)<<0))
#define MIS_GP3DP_gpha_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define MIS_GP3DP_get_gpha(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define MIS_GPDEB                                                                    0x1801B150
#define MIS_GPDEB_reg_addr                                                           "0xB801B150"
#define MIS_GPDEB_reg                                                                0xB801B150
#define set_MIS_GPDEB_reg(data)   (*((volatile unsigned int*) MIS_GPDEB_reg)=data)
#define get_MIS_GPDEB_reg   (*((volatile unsigned int*) MIS_GPDEB_reg))
#define MIS_GPDEB_inst_adr                                                           "0x0054"
#define MIS_GPDEB_inst                                                               0x0054
#define MIS_GPDEB_write_enable8_shift                                                (31)
#define MIS_GPDEB_write_enable8_mask                                                 (0x80000000)
#define MIS_GPDEB_write_enable8(data)                                                (0x80000000&((data)<<31))
#define MIS_GPDEB_write_enable8_src(data)                                            ((0x80000000&(data))>>31)
#define MIS_GPDEB_get_write_enable8(data)                                            ((0x80000000&(data))>>31)
#define MIS_GPDEB_clk8_shift                                                         (28)
#define MIS_GPDEB_clk8_mask                                                          (0x70000000)
#define MIS_GPDEB_clk8(data)                                                         (0x70000000&((data)<<28))
#define MIS_GPDEB_clk8_src(data)                                                     ((0x70000000&(data))>>28)
#define MIS_GPDEB_get_clk8(data)                                                     ((0x70000000&(data))>>28)
#define MIS_GPDEB_write_enable7_shift                                                (27)
#define MIS_GPDEB_write_enable7_mask                                                 (0x08000000)
#define MIS_GPDEB_write_enable7(data)                                                (0x08000000&((data)<<27))
#define MIS_GPDEB_write_enable7_src(data)                                            ((0x08000000&(data))>>27)
#define MIS_GPDEB_get_write_enable7(data)                                            ((0x08000000&(data))>>27)
#define MIS_GPDEB_clk7_shift                                                         (24)
#define MIS_GPDEB_clk7_mask                                                          (0x07000000)
#define MIS_GPDEB_clk7(data)                                                         (0x07000000&((data)<<24))
#define MIS_GPDEB_clk7_src(data)                                                     ((0x07000000&(data))>>24)
#define MIS_GPDEB_get_clk7(data)                                                     ((0x07000000&(data))>>24)
#define MIS_GPDEB_write_enable6_shift                                                (23)
#define MIS_GPDEB_write_enable6_mask                                                 (0x00800000)
#define MIS_GPDEB_write_enable6(data)                                                (0x00800000&((data)<<23))
#define MIS_GPDEB_write_enable6_src(data)                                            ((0x00800000&(data))>>23)
#define MIS_GPDEB_get_write_enable6(data)                                            ((0x00800000&(data))>>23)
#define MIS_GPDEB_clk6_shift                                                         (20)
#define MIS_GPDEB_clk6_mask                                                          (0x00700000)
#define MIS_GPDEB_clk6(data)                                                         (0x00700000&((data)<<20))
#define MIS_GPDEB_clk6_src(data)                                                     ((0x00700000&(data))>>20)
#define MIS_GPDEB_get_clk6(data)                                                     ((0x00700000&(data))>>20)
#define MIS_GPDEB_write_enable5_shift                                                (19)
#define MIS_GPDEB_write_enable5_mask                                                 (0x00080000)
#define MIS_GPDEB_write_enable5(data)                                                (0x00080000&((data)<<19))
#define MIS_GPDEB_write_enable5_src(data)                                            ((0x00080000&(data))>>19)
#define MIS_GPDEB_get_write_enable5(data)                                            ((0x00080000&(data))>>19)
#define MIS_GPDEB_clk5_shift                                                         (16)
#define MIS_GPDEB_clk5_mask                                                          (0x00070000)
#define MIS_GPDEB_clk5(data)                                                         (0x00070000&((data)<<16))
#define MIS_GPDEB_clk5_src(data)                                                     ((0x00070000&(data))>>16)
#define MIS_GPDEB_get_clk5(data)                                                     ((0x00070000&(data))>>16)
#define MIS_GPDEB_write_enable4_shift                                                (15)
#define MIS_GPDEB_write_enable4_mask                                                 (0x00008000)
#define MIS_GPDEB_write_enable4(data)                                                (0x00008000&((data)<<15))
#define MIS_GPDEB_write_enable4_src(data)                                            ((0x00008000&(data))>>15)
#define MIS_GPDEB_get_write_enable4(data)                                            ((0x00008000&(data))>>15)
#define MIS_GPDEB_clk4_shift                                                         (12)
#define MIS_GPDEB_clk4_mask                                                          (0x00007000)
#define MIS_GPDEB_clk4(data)                                                         (0x00007000&((data)<<12))
#define MIS_GPDEB_clk4_src(data)                                                     ((0x00007000&(data))>>12)
#define MIS_GPDEB_get_clk4(data)                                                     ((0x00007000&(data))>>12)
#define MIS_GPDEB_write_enable3_shift                                                (11)
#define MIS_GPDEB_write_enable3_mask                                                 (0x00000800)
#define MIS_GPDEB_write_enable3(data)                                                (0x00000800&((data)<<11))
#define MIS_GPDEB_write_enable3_src(data)                                            ((0x00000800&(data))>>11)
#define MIS_GPDEB_get_write_enable3(data)                                            ((0x00000800&(data))>>11)
#define MIS_GPDEB_clk3_shift                                                         (8)
#define MIS_GPDEB_clk3_mask                                                          (0x00000700)
#define MIS_GPDEB_clk3(data)                                                         (0x00000700&((data)<<8))
#define MIS_GPDEB_clk3_src(data)                                                     ((0x00000700&(data))>>8)
#define MIS_GPDEB_get_clk3(data)                                                     ((0x00000700&(data))>>8)
#define MIS_GPDEB_write_enable2_shift                                                (7)
#define MIS_GPDEB_write_enable2_mask                                                 (0x00000080)
#define MIS_GPDEB_write_enable2(data)                                                (0x00000080&((data)<<7))
#define MIS_GPDEB_write_enable2_src(data)                                            ((0x00000080&(data))>>7)
#define MIS_GPDEB_get_write_enable2(data)                                            ((0x00000080&(data))>>7)
#define MIS_GPDEB_clk2_shift                                                         (4)
#define MIS_GPDEB_clk2_mask                                                          (0x00000070)
#define MIS_GPDEB_clk2(data)                                                         (0x00000070&((data)<<4))
#define MIS_GPDEB_clk2_src(data)                                                     ((0x00000070&(data))>>4)
#define MIS_GPDEB_get_clk2(data)                                                     ((0x00000070&(data))>>4)
#define MIS_GPDEB_write_enable1_shift                                                (3)
#define MIS_GPDEB_write_enable1_mask                                                 (0x00000008)
#define MIS_GPDEB_write_enable1(data)                                                (0x00000008&((data)<<3))
#define MIS_GPDEB_write_enable1_src(data)                                            ((0x00000008&(data))>>3)
#define MIS_GPDEB_get_write_enable1(data)                                            ((0x00000008&(data))>>3)
#define MIS_GPDEB_clk1_shift                                                         (0)
#define MIS_GPDEB_clk1_mask                                                          (0x00000007)
#define MIS_GPDEB_clk1(data)                                                         (0x00000007&((data)<<0))
#define MIS_GPDEB_clk1_src(data)                                                     ((0x00000007&(data))>>0)
#define MIS_GPDEB_get_clk1(data)                                                     ((0x00000007&(data))>>0)


#define MIS_U1RBR_THR_DLL                                                            0x1801BC00
#define MIS_U1RBR_THR_DLL_reg_addr                                                   "0xB801BC00"
#define MIS_U1RBR_THR_DLL_reg                                                        0xB801BC00
#define set_MIS_U1RBR_THR_DLL_reg(data)   (*((volatile unsigned int*) MIS_U1RBR_THR_DLL_reg)=data)
#define get_MIS_U1RBR_THR_DLL_reg   (*((volatile unsigned int*) MIS_U1RBR_THR_DLL_reg))
#define MIS_U1RBR_THR_DLL_inst_adr                                                   "0x0000"
#define MIS_U1RBR_THR_DLL_inst                                                       0x0000
#define MIS_U1RBR_THR_DLL_dll_shift                                                  (0)
#define MIS_U1RBR_THR_DLL_dll_mask                                                   (0x000000FF)
#define MIS_U1RBR_THR_DLL_dll(data)                                                  (0x000000FF&((data)<<0))
#define MIS_U1RBR_THR_DLL_dll_src(data)                                              ((0x000000FF&(data))>>0)
#define MIS_U1RBR_THR_DLL_get_dll(data)                                              ((0x000000FF&(data))>>0)


#define MIS_U1IER_DLH                                                                0x1801BC04
#define MIS_U1IER_DLH_reg_addr                                                       "0xB801BC04"
#define MIS_U1IER_DLH_reg                                                            0xB801BC04
#define set_MIS_U1IER_DLH_reg(data)   (*((volatile unsigned int*) MIS_U1IER_DLH_reg)=data)
#define get_MIS_U1IER_DLH_reg   (*((volatile unsigned int*) MIS_U1IER_DLH_reg))
#define MIS_U1IER_DLH_inst_adr                                                       "0x0001"
#define MIS_U1IER_DLH_inst                                                           0x0001
#define MIS_U1IER_DLH_dlh_shift                                                      (0)
#define MIS_U1IER_DLH_dlh_mask                                                       (0x000000FF)
#define MIS_U1IER_DLH_dlh(data)                                                      (0x000000FF&((data)<<0))
#define MIS_U1IER_DLH_dlh_src(data)                                                  ((0x000000FF&(data))>>0)
#define MIS_U1IER_DLH_get_dlh(data)                                                  ((0x000000FF&(data))>>0)


#define MIS_U1IIR_FCR                                                                0x1801BC08
#define MIS_U1IIR_FCR_reg_addr                                                       "0xB801BC08"
#define MIS_U1IIR_FCR_reg                                                            0xB801BC08
#define set_MIS_U1IIR_FCR_reg(data)   (*((volatile unsigned int*) MIS_U1IIR_FCR_reg)=data)
#define get_MIS_U1IIR_FCR_reg   (*((volatile unsigned int*) MIS_U1IIR_FCR_reg))
#define MIS_U1IIR_FCR_inst_adr                                                       "0x0002"
#define MIS_U1IIR_FCR_inst                                                           0x0002
#define MIS_U1IIR_FCR_fifo16_shift                                                   (6)
#define MIS_U1IIR_FCR_fifo16_mask                                                    (0x000000C0)
#define MIS_U1IIR_FCR_fifo16(data)                                                   (0x000000C0&((data)<<6))
#define MIS_U1IIR_FCR_fifo16_src(data)                                               ((0x000000C0&(data))>>6)
#define MIS_U1IIR_FCR_get_fifo16(data)                                               ((0x000000C0&(data))>>6)
#define MIS_U1IIR_FCR_iid_shift                                                      (0)
#define MIS_U1IIR_FCR_iid_mask                                                       (0x0000000F)
#define MIS_U1IIR_FCR_iid(data)                                                      (0x0000000F&((data)<<0))
#define MIS_U1IIR_FCR_iid_src(data)                                                  ((0x0000000F&(data))>>0)
#define MIS_U1IIR_FCR_get_iid(data)                                                  ((0x0000000F&(data))>>0)


#define MIS_U1LCR                                                                    0x1801BC0C
#define MIS_U1LCR_reg_addr                                                           "0xB801BC0C"
#define MIS_U1LCR_reg                                                                0xB801BC0C
#define set_MIS_U1LCR_reg(data)   (*((volatile unsigned int*) MIS_U1LCR_reg)=data)
#define get_MIS_U1LCR_reg   (*((volatile unsigned int*) MIS_U1LCR_reg))
#define MIS_U1LCR_inst_adr                                                           "0x0003"
#define MIS_U1LCR_inst                                                               0x0003
#define MIS_U1LCR_dlab_shift                                                         (7)
#define MIS_U1LCR_dlab_mask                                                          (0x00000080)
#define MIS_U1LCR_dlab(data)                                                         (0x00000080&((data)<<7))
#define MIS_U1LCR_dlab_src(data)                                                     ((0x00000080&(data))>>7)
#define MIS_U1LCR_get_dlab(data)                                                     ((0x00000080&(data))>>7)
#define MIS_U1LCR_brk_shift                                                          (6)
#define MIS_U1LCR_brk_mask                                                           (0x00000040)
#define MIS_U1LCR_brk(data)                                                          (0x00000040&((data)<<6))
#define MIS_U1LCR_brk_src(data)                                                      ((0x00000040&(data))>>6)
#define MIS_U1LCR_get_brk(data)                                                      ((0x00000040&(data))>>6)
#define MIS_U1LCR_eps_shift                                                          (4)
#define MIS_U1LCR_eps_mask                                                           (0x00000010)
#define MIS_U1LCR_eps(data)                                                          (0x00000010&((data)<<4))
#define MIS_U1LCR_eps_src(data)                                                      ((0x00000010&(data))>>4)
#define MIS_U1LCR_get_eps(data)                                                      ((0x00000010&(data))>>4)
#define MIS_U1LCR_pen_shift                                                          (3)
#define MIS_U1LCR_pen_mask                                                           (0x00000008)
#define MIS_U1LCR_pen(data)                                                          (0x00000008&((data)<<3))
#define MIS_U1LCR_pen_src(data)                                                      ((0x00000008&(data))>>3)
#define MIS_U1LCR_get_pen(data)                                                      ((0x00000008&(data))>>3)
#define MIS_U1LCR_stb_shift                                                          (2)
#define MIS_U1LCR_stb_mask                                                           (0x00000004)
#define MIS_U1LCR_stb(data)                                                          (0x00000004&((data)<<2))
#define MIS_U1LCR_stb_src(data)                                                      ((0x00000004&(data))>>2)
#define MIS_U1LCR_get_stb(data)                                                      ((0x00000004&(data))>>2)
#define MIS_U1LCR_wls_shift                                                          (0)
#define MIS_U1LCR_wls_mask                                                           (0x00000003)
#define MIS_U1LCR_wls(data)                                                          (0x00000003&((data)<<0))
#define MIS_U1LCR_wls_src(data)                                                      ((0x00000003&(data))>>0)
#define MIS_U1LCR_get_wls(data)                                                      ((0x00000003&(data))>>0)


#define MIS_U1MCR                                                                    0x1801BC10
#define MIS_U1MCR_reg_addr                                                           "0xB801BC10"
#define MIS_U1MCR_reg                                                                0xB801BC10
#define set_MIS_U1MCR_reg(data)   (*((volatile unsigned int*) MIS_U1MCR_reg)=data)
#define get_MIS_U1MCR_reg   (*((volatile unsigned int*) MIS_U1MCR_reg))
#define MIS_U1MCR_inst_adr                                                           "0x0004"
#define MIS_U1MCR_inst                                                               0x0004
#define MIS_U1MCR_afce_shift                                                         (5)
#define MIS_U1MCR_afce_mask                                                          (0x00000020)
#define MIS_U1MCR_afce(data)                                                         (0x00000020&((data)<<5))
#define MIS_U1MCR_afce_src(data)                                                     ((0x00000020&(data))>>5)
#define MIS_U1MCR_get_afce(data)                                                     ((0x00000020&(data))>>5)
#define MIS_U1MCR_loop_shift                                                         (4)
#define MIS_U1MCR_loop_mask                                                          (0x00000010)
#define MIS_U1MCR_loop(data)                                                         (0x00000010&((data)<<4))
#define MIS_U1MCR_loop_src(data)                                                     ((0x00000010&(data))>>4)
#define MIS_U1MCR_get_loop(data)                                                     ((0x00000010&(data))>>4)
#define MIS_U1MCR_rts_shift                                                          (1)
#define MIS_U1MCR_rts_mask                                                           (0x00000002)
#define MIS_U1MCR_rts(data)                                                          (0x00000002&((data)<<1))
#define MIS_U1MCR_rts_src(data)                                                      ((0x00000002&(data))>>1)
#define MIS_U1MCR_get_rts(data)                                                      ((0x00000002&(data))>>1)
#define MIS_U1MCR_dtr_shift                                                          (0)
#define MIS_U1MCR_dtr_mask                                                           (0x00000001)
#define MIS_U1MCR_dtr(data)                                                          (0x00000001&((data)<<0))
#define MIS_U1MCR_dtr_src(data)                                                      ((0x00000001&(data))>>0)
#define MIS_U1MCR_get_dtr(data)                                                      ((0x00000001&(data))>>0)


#define MIS_U1LSR                                                                    0x1801BC14
#define MIS_U1LSR_reg_addr                                                           "0xB801BC14"
#define MIS_U1LSR_reg                                                                0xB801BC14
#define set_MIS_U1LSR_reg(data)   (*((volatile unsigned int*) MIS_U1LSR_reg)=data)
#define get_MIS_U1LSR_reg   (*((volatile unsigned int*) MIS_U1LSR_reg))
#define MIS_U1LSR_inst_adr                                                           "0x0005"
#define MIS_U1LSR_inst                                                               0x0005
#define MIS_U1LSR_rfe_shift                                                          (7)
#define MIS_U1LSR_rfe_mask                                                           (0x00000080)
#define MIS_U1LSR_rfe(data)                                                          (0x00000080&((data)<<7))
#define MIS_U1LSR_rfe_src(data)                                                      ((0x00000080&(data))>>7)
#define MIS_U1LSR_get_rfe(data)                                                      ((0x00000080&(data))>>7)
#define MIS_U1LSR_temt_shift                                                         (6)
#define MIS_U1LSR_temt_mask                                                          (0x00000040)
#define MIS_U1LSR_temt(data)                                                         (0x00000040&((data)<<6))
#define MIS_U1LSR_temt_src(data)                                                     ((0x00000040&(data))>>6)
#define MIS_U1LSR_get_temt(data)                                                     ((0x00000040&(data))>>6)
#define MIS_U1LSR_thre_shift                                                         (5)
#define MIS_U1LSR_thre_mask                                                          (0x00000020)
#define MIS_U1LSR_thre(data)                                                         (0x00000020&((data)<<5))
#define MIS_U1LSR_thre_src(data)                                                     ((0x00000020&(data))>>5)
#define MIS_U1LSR_get_thre(data)                                                     ((0x00000020&(data))>>5)
#define MIS_U1LSR_bi_shift                                                           (4)
#define MIS_U1LSR_bi_mask                                                            (0x00000010)
#define MIS_U1LSR_bi(data)                                                           (0x00000010&((data)<<4))
#define MIS_U1LSR_bi_src(data)                                                       ((0x00000010&(data))>>4)
#define MIS_U1LSR_get_bi(data)                                                       ((0x00000010&(data))>>4)
#define MIS_U1LSR_fe_shift                                                           (3)
#define MIS_U1LSR_fe_mask                                                            (0x00000008)
#define MIS_U1LSR_fe(data)                                                           (0x00000008&((data)<<3))
#define MIS_U1LSR_fe_src(data)                                                       ((0x00000008&(data))>>3)
#define MIS_U1LSR_get_fe(data)                                                       ((0x00000008&(data))>>3)
#define MIS_U1LSR_pe_shift                                                           (2)
#define MIS_U1LSR_pe_mask                                                            (0x00000004)
#define MIS_U1LSR_pe(data)                                                           (0x00000004&((data)<<2))
#define MIS_U1LSR_pe_src(data)                                                       ((0x00000004&(data))>>2)
#define MIS_U1LSR_get_pe(data)                                                       ((0x00000004&(data))>>2)
#define MIS_U1LSR_oe_shift                                                           (1)
#define MIS_U1LSR_oe_mask                                                            (0x00000002)
#define MIS_U1LSR_oe(data)                                                           (0x00000002&((data)<<1))
#define MIS_U1LSR_oe_src(data)                                                       ((0x00000002&(data))>>1)
#define MIS_U1LSR_get_oe(data)                                                       ((0x00000002&(data))>>1)
#define MIS_U1LSR_dr_shift                                                           (0)
#define MIS_U1LSR_dr_mask                                                            (0x00000001)
#define MIS_U1LSR_dr(data)                                                           (0x00000001&((data)<<0))
#define MIS_U1LSR_dr_src(data)                                                       ((0x00000001&(data))>>0)
#define MIS_U1LSR_get_dr(data)                                                       ((0x00000001&(data))>>0)


#define MIS_U1MSR                                                                    0x1801BC18
#define MIS_U1MSR_reg_addr                                                           "0xB801BC18"
#define MIS_U1MSR_reg                                                                0xB801BC18
#define set_MIS_U1MSR_reg(data)   (*((volatile unsigned int*) MIS_U1MSR_reg)=data)
#define get_MIS_U1MSR_reg   (*((volatile unsigned int*) MIS_U1MSR_reg))
#define MIS_U1MSR_inst_adr                                                           "0x0006"
#define MIS_U1MSR_inst                                                               0x0006
#define MIS_U1MSR_dcd_shift                                                          (7)
#define MIS_U1MSR_dcd_mask                                                           (0x00000080)
#define MIS_U1MSR_dcd(data)                                                          (0x00000080&((data)<<7))
#define MIS_U1MSR_dcd_src(data)                                                      ((0x00000080&(data))>>7)
#define MIS_U1MSR_get_dcd(data)                                                      ((0x00000080&(data))>>7)
#define MIS_U1MSR_ri_shift                                                           (6)
#define MIS_U1MSR_ri_mask                                                            (0x00000040)
#define MIS_U1MSR_ri(data)                                                           (0x00000040&((data)<<6))
#define MIS_U1MSR_ri_src(data)                                                       ((0x00000040&(data))>>6)
#define MIS_U1MSR_get_ri(data)                                                       ((0x00000040&(data))>>6)
#define MIS_U1MSR_dsr_shift                                                          (5)
#define MIS_U1MSR_dsr_mask                                                           (0x00000020)
#define MIS_U1MSR_dsr(data)                                                          (0x00000020&((data)<<5))
#define MIS_U1MSR_dsr_src(data)                                                      ((0x00000020&(data))>>5)
#define MIS_U1MSR_get_dsr(data)                                                      ((0x00000020&(data))>>5)
#define MIS_U1MSR_cts_shift                                                          (4)
#define MIS_U1MSR_cts_mask                                                           (0x00000010)
#define MIS_U1MSR_cts(data)                                                          (0x00000010&((data)<<4))
#define MIS_U1MSR_cts_src(data)                                                      ((0x00000010&(data))>>4)
#define MIS_U1MSR_get_cts(data)                                                      ((0x00000010&(data))>>4)
#define MIS_U1MSR_ddcd_shift                                                         (3)
#define MIS_U1MSR_ddcd_mask                                                          (0x00000008)
#define MIS_U1MSR_ddcd(data)                                                         (0x00000008&((data)<<3))
#define MIS_U1MSR_ddcd_src(data)                                                     ((0x00000008&(data))>>3)
#define MIS_U1MSR_get_ddcd(data)                                                     ((0x00000008&(data))>>3)
#define MIS_U1MSR_teri_shift                                                         (2)
#define MIS_U1MSR_teri_mask                                                          (0x00000004)
#define MIS_U1MSR_teri(data)                                                         (0x00000004&((data)<<2))
#define MIS_U1MSR_teri_src(data)                                                     ((0x00000004&(data))>>2)
#define MIS_U1MSR_get_teri(data)                                                     ((0x00000004&(data))>>2)
#define MIS_U1MSR_ddsr_shift                                                         (1)
#define MIS_U1MSR_ddsr_mask                                                          (0x00000002)
#define MIS_U1MSR_ddsr(data)                                                         (0x00000002&((data)<<1))
#define MIS_U1MSR_ddsr_src(data)                                                     ((0x00000002&(data))>>1)
#define MIS_U1MSR_get_ddsr(data)                                                     ((0x00000002&(data))>>1)
#define MIS_U1MSR_dcts_shift                                                         (0)
#define MIS_U1MSR_dcts_mask                                                          (0x00000001)
#define MIS_U1MSR_dcts(data)                                                         (0x00000001&((data)<<0))
#define MIS_U1MSR_dcts_src(data)                                                     ((0x00000001&(data))>>0)
#define MIS_U1MSR_get_dcts(data)                                                     ((0x00000001&(data))>>0)


#define MIS_U1SCR                                                                    0x1801BC1C
#define MIS_U1SCR_reg_addr                                                           "0xB801BC1C"
#define MIS_U1SCR_reg                                                                0xB801BC1C
#define set_MIS_U1SCR_reg(data)   (*((volatile unsigned int*) MIS_U1SCR_reg)=data)
#define get_MIS_U1SCR_reg   (*((volatile unsigned int*) MIS_U1SCR_reg))
#define MIS_U1SCR_inst_adr                                                           "0x0007"
#define MIS_U1SCR_inst                                                               0x0007
#define MIS_U1SCR_scr_shift                                                          (0)
#define MIS_U1SCR_scr_mask                                                           (0x000000FF)
#define MIS_U1SCR_scr(data)                                                          (0x000000FF&((data)<<0))
#define MIS_U1SCR_scr_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_U1SCR_get_scr(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_U1SRBR_0                                                                 0x1801BC30
#define MIS_U1SRBR_1                                                                 0x1801BC34
#define MIS_U1SRBR_2                                                                 0x1801BC38
#define MIS_U1SRBR_3                                                                 0x1801BC3C
#define MIS_U1SRBR_4                                                                 0x1801BC40
#define MIS_U1SRBR_5                                                                 0x1801BC44
#define MIS_U1SRBR_6                                                                 0x1801BC48
#define MIS_U1SRBR_7                                                                 0x1801BC4C
#define MIS_U1SRBR_8                                                                 0x1801BC50
#define MIS_U1SRBR_9                                                                 0x1801BC54
#define MIS_U1SRBR_10                                                                 0x1801BC58
#define MIS_U1SRBR_11                                                                 0x1801BC5C
#define MIS_U1SRBR_12                                                                 0x1801BC60
#define MIS_U1SRBR_13                                                                 0x1801BC64
#define MIS_U1SRBR_14                                                                 0x1801BC68
#define MIS_U1SRBR_15                                                                 0x1801BC6C
#define MIS_U1SRBR_0_reg_addr                                                        "0xB801BC30"
#define MIS_U1SRBR_1_reg_addr                                                        "0xB801BC34"
#define MIS_U1SRBR_2_reg_addr                                                        "0xB801BC38"
#define MIS_U1SRBR_3_reg_addr                                                        "0xB801BC3C"
#define MIS_U1SRBR_4_reg_addr                                                        "0xB801BC40"
#define MIS_U1SRBR_5_reg_addr                                                        "0xB801BC44"
#define MIS_U1SRBR_6_reg_addr                                                        "0xB801BC48"
#define MIS_U1SRBR_7_reg_addr                                                        "0xB801BC4C"
#define MIS_U1SRBR_8_reg_addr                                                        "0xB801BC50"
#define MIS_U1SRBR_9_reg_addr                                                        "0xB801BC54"
#define MIS_U1SRBR_10_reg_addr                                                        "0xB801BC58"
#define MIS_U1SRBR_11_reg_addr                                                        "0xB801BC5C"
#define MIS_U1SRBR_12_reg_addr                                                        "0xB801BC60"
#define MIS_U1SRBR_13_reg_addr                                                        "0xB801BC64"
#define MIS_U1SRBR_14_reg_addr                                                        "0xB801BC68"
#define MIS_U1SRBR_15_reg_addr                                                        "0xB801BC6C"
#define MIS_U1SRBR_0_reg                                                             0xB801BC30
#define MIS_U1SRBR_1_reg                                                             0xB801BC34
#define MIS_U1SRBR_2_reg                                                             0xB801BC38
#define MIS_U1SRBR_3_reg                                                             0xB801BC3C
#define MIS_U1SRBR_4_reg                                                             0xB801BC40
#define MIS_U1SRBR_5_reg                                                             0xB801BC44
#define MIS_U1SRBR_6_reg                                                             0xB801BC48
#define MIS_U1SRBR_7_reg                                                             0xB801BC4C
#define MIS_U1SRBR_8_reg                                                             0xB801BC50
#define MIS_U1SRBR_9_reg                                                             0xB801BC54
#define MIS_U1SRBR_10_reg                                                             0xB801BC58
#define MIS_U1SRBR_11_reg                                                             0xB801BC5C
#define MIS_U1SRBR_12_reg                                                             0xB801BC60
#define MIS_U1SRBR_13_reg                                                             0xB801BC64
#define MIS_U1SRBR_14_reg                                                             0xB801BC68
#define MIS_U1SRBR_15_reg                                                             0xB801BC6C
#define set_MIS_U1SRBR_0_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_0_reg)=data)
#define set_MIS_U1SRBR_1_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_1_reg)=data)
#define set_MIS_U1SRBR_2_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_2_reg)=data)
#define set_MIS_U1SRBR_3_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_3_reg)=data)
#define set_MIS_U1SRBR_4_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_4_reg)=data)
#define set_MIS_U1SRBR_5_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_5_reg)=data)
#define set_MIS_U1SRBR_6_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_6_reg)=data)
#define set_MIS_U1SRBR_7_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_7_reg)=data)
#define set_MIS_U1SRBR_8_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_8_reg)=data)
#define set_MIS_U1SRBR_9_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_9_reg)=data)
#define set_MIS_U1SRBR_10_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_10_reg)=data)
#define set_MIS_U1SRBR_11_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_11_reg)=data)
#define set_MIS_U1SRBR_12_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_12_reg)=data)
#define set_MIS_U1SRBR_13_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_13_reg)=data)
#define set_MIS_U1SRBR_14_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_14_reg)=data)
#define set_MIS_U1SRBR_15_reg(data)   (*((volatile unsigned int*) MIS_U1SRBR_15_reg)=data)
#define get_MIS_U1SRBR_0_reg   (*((volatile unsigned int*) MIS_U1SRBR_0_reg))
#define get_MIS_U1SRBR_1_reg   (*((volatile unsigned int*) MIS_U1SRBR_1_reg))
#define get_MIS_U1SRBR_2_reg   (*((volatile unsigned int*) MIS_U1SRBR_2_reg))
#define get_MIS_U1SRBR_3_reg   (*((volatile unsigned int*) MIS_U1SRBR_3_reg))
#define get_MIS_U1SRBR_4_reg   (*((volatile unsigned int*) MIS_U1SRBR_4_reg))
#define get_MIS_U1SRBR_5_reg   (*((volatile unsigned int*) MIS_U1SRBR_5_reg))
#define get_MIS_U1SRBR_6_reg   (*((volatile unsigned int*) MIS_U1SRBR_6_reg))
#define get_MIS_U1SRBR_7_reg   (*((volatile unsigned int*) MIS_U1SRBR_7_reg))
#define get_MIS_U1SRBR_8_reg   (*((volatile unsigned int*) MIS_U1SRBR_8_reg))
#define get_MIS_U1SRBR_9_reg   (*((volatile unsigned int*) MIS_U1SRBR_9_reg))
#define get_MIS_U1SRBR_10_reg   (*((volatile unsigned int*) MIS_U1SRBR_10_reg))
#define get_MIS_U1SRBR_11_reg   (*((volatile unsigned int*) MIS_U1SRBR_11_reg))
#define get_MIS_U1SRBR_12_reg   (*((volatile unsigned int*) MIS_U1SRBR_12_reg))
#define get_MIS_U1SRBR_13_reg   (*((volatile unsigned int*) MIS_U1SRBR_13_reg))
#define get_MIS_U1SRBR_14_reg   (*((volatile unsigned int*) MIS_U1SRBR_14_reg))
#define get_MIS_U1SRBR_15_reg   (*((volatile unsigned int*) MIS_U1SRBR_15_reg))
#define MIS_U1SRBR_0_inst_adr                                                        "0x000C"
#define MIS_U1SRBR_1_inst_adr                                                        "0x000D"
#define MIS_U1SRBR_2_inst_adr                                                        "0x000E"
#define MIS_U1SRBR_3_inst_adr                                                        "0x000F"
#define MIS_U1SRBR_4_inst_adr                                                        "0x0010"
#define MIS_U1SRBR_5_inst_adr                                                        "0x0011"
#define MIS_U1SRBR_6_inst_adr                                                        "0x0012"
#define MIS_U1SRBR_7_inst_adr                                                        "0x0013"
#define MIS_U1SRBR_8_inst_adr                                                        "0x0014"
#define MIS_U1SRBR_9_inst_adr                                                        "0x0015"
#define MIS_U1SRBR_10_inst_adr                                                        "0x0016"
#define MIS_U1SRBR_11_inst_adr                                                        "0x0017"
#define MIS_U1SRBR_12_inst_adr                                                        "0x0018"
#define MIS_U1SRBR_13_inst_adr                                                        "0x0019"
#define MIS_U1SRBR_14_inst_adr                                                        "0x001A"
#define MIS_U1SRBR_15_inst_adr                                                        "0x001B"
#define MIS_U1SRBR_0_inst                                                            0x000C
#define MIS_U1SRBR_1_inst                                                            0x000D
#define MIS_U1SRBR_2_inst                                                            0x000E
#define MIS_U1SRBR_3_inst                                                            0x000F
#define MIS_U1SRBR_4_inst                                                            0x0010
#define MIS_U1SRBR_5_inst                                                            0x0011
#define MIS_U1SRBR_6_inst                                                            0x0012
#define MIS_U1SRBR_7_inst                                                            0x0013
#define MIS_U1SRBR_8_inst                                                            0x0014
#define MIS_U1SRBR_9_inst                                                            0x0015
#define MIS_U1SRBR_10_inst                                                            0x0016
#define MIS_U1SRBR_11_inst                                                            0x0017
#define MIS_U1SRBR_12_inst                                                            0x0018
#define MIS_U1SRBR_13_inst                                                            0x0019
#define MIS_U1SRBR_14_inst                                                            0x001A
#define MIS_U1SRBR_15_inst                                                            0x001B
#define MIS_U1SRBR_rbd_shift                                                         (0)
#define MIS_U1SRBR_rbd_mask                                                          (0x000000FF)
#define MIS_U1SRBR_rbd(data)                                                         (0x000000FF&((data)<<0))
#define MIS_U1SRBR_rbd_src(data)                                                     ((0x000000FF&(data))>>0)
#define MIS_U1SRBR_get_rbd(data)                                                     ((0x000000FF&(data))>>0)


#define MIS_U1FAR                                                                    0x1801BC70
#define MIS_U1FAR_reg_addr                                                           "0xB801BC70"
#define MIS_U1FAR_reg                                                                0xB801BC70
#define set_MIS_U1FAR_reg(data)   (*((volatile unsigned int*) MIS_U1FAR_reg)=data)
#define get_MIS_U1FAR_reg   (*((volatile unsigned int*) MIS_U1FAR_reg))
#define MIS_U1FAR_inst_adr                                                           "0x001C"
#define MIS_U1FAR_inst                                                               0x001C
#define MIS_U1FAR_far_shift                                                          (0)
#define MIS_U1FAR_far_mask                                                           (0x000000FF)
#define MIS_U1FAR_far(data)                                                          (0x000000FF&((data)<<0))
#define MIS_U1FAR_far_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_U1FAR_get_far(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_U1TFR                                                                    0x1801BC74
#define MIS_U1TFR_reg_addr                                                           "0xB801BC74"
#define MIS_U1TFR_reg                                                                0xB801BC74
#define set_MIS_U1TFR_reg(data)   (*((volatile unsigned int*) MIS_U1TFR_reg)=data)
#define get_MIS_U1TFR_reg   (*((volatile unsigned int*) MIS_U1TFR_reg))
#define MIS_U1TFR_inst_adr                                                           "0x001D"
#define MIS_U1TFR_inst                                                               0x001D
#define MIS_U1TFR_far_shift                                                          (0)
#define MIS_U1TFR_far_mask                                                           (0x000000FF)
#define MIS_U1TFR_far(data)                                                          (0x000000FF&((data)<<0))
#define MIS_U1TFR_far_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_U1TFR_get_far(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_U1RFW                                                                    0x1801BC78
#define MIS_U1RFW_reg_addr                                                           "0xB801BC78"
#define MIS_U1RFW_reg                                                                0xB801BC78
#define set_MIS_U1RFW_reg(data)   (*((volatile unsigned int*) MIS_U1RFW_reg)=data)
#define get_MIS_U1RFW_reg   (*((volatile unsigned int*) MIS_U1RFW_reg))
#define MIS_U1RFW_inst_adr                                                           "0x001E"
#define MIS_U1RFW_inst                                                               0x001E
#define MIS_U1RFW_rffe_shift                                                         (9)
#define MIS_U1RFW_rffe_mask                                                          (0x00000200)
#define MIS_U1RFW_rffe(data)                                                         (0x00000200&((data)<<9))
#define MIS_U1RFW_rffe_src(data)                                                     ((0x00000200&(data))>>9)
#define MIS_U1RFW_get_rffe(data)                                                     ((0x00000200&(data))>>9)
#define MIS_U1RFW_rfpf_shift                                                         (8)
#define MIS_U1RFW_rfpf_mask                                                          (0x00000100)
#define MIS_U1RFW_rfpf(data)                                                         (0x00000100&((data)<<8))
#define MIS_U1RFW_rfpf_src(data)                                                     ((0x00000100&(data))>>8)
#define MIS_U1RFW_get_rfpf(data)                                                     ((0x00000100&(data))>>8)
#define MIS_U1RFW_rfwd_shift                                                         (0)
#define MIS_U1RFW_rfwd_mask                                                          (0x000000FF)
#define MIS_U1RFW_rfwd(data)                                                         (0x000000FF&((data)<<0))
#define MIS_U1RFW_rfwd_src(data)                                                     ((0x000000FF&(data))>>0)
#define MIS_U1RFW_get_rfwd(data)                                                     ((0x000000FF&(data))>>0)


#define MIS_U1USR                                                                    0x1801BC7C
#define MIS_U1USR_reg_addr                                                           "0xB801BC7C"
#define MIS_U1USR_reg                                                                0xB801BC7C
#define set_MIS_U1USR_reg(data)   (*((volatile unsigned int*) MIS_U1USR_reg)=data)
#define get_MIS_U1USR_reg   (*((volatile unsigned int*) MIS_U1USR_reg))
#define MIS_U1USR_inst_adr                                                           "0x001F"
#define MIS_U1USR_inst                                                               0x001F
#define MIS_U1USR_rff_shift                                                          (4)
#define MIS_U1USR_rff_mask                                                           (0x00000010)
#define MIS_U1USR_rff(data)                                                          (0x00000010&((data)<<4))
#define MIS_U1USR_rff_src(data)                                                      ((0x00000010&(data))>>4)
#define MIS_U1USR_get_rff(data)                                                      ((0x00000010&(data))>>4)
#define MIS_U1USR_rfne_shift                                                         (3)
#define MIS_U1USR_rfne_mask                                                          (0x00000008)
#define MIS_U1USR_rfne(data)                                                         (0x00000008&((data)<<3))
#define MIS_U1USR_rfne_src(data)                                                     ((0x00000008&(data))>>3)
#define MIS_U1USR_get_rfne(data)                                                     ((0x00000008&(data))>>3)
#define MIS_U1USR_tfe_shift                                                          (2)
#define MIS_U1USR_tfe_mask                                                           (0x00000004)
#define MIS_U1USR_tfe(data)                                                          (0x00000004&((data)<<2))
#define MIS_U1USR_tfe_src(data)                                                      ((0x00000004&(data))>>2)
#define MIS_U1USR_get_tfe(data)                                                      ((0x00000004&(data))>>2)
#define MIS_U1USR_tfnf_shift                                                         (1)
#define MIS_U1USR_tfnf_mask                                                          (0x00000002)
#define MIS_U1USR_tfnf(data)                                                         (0x00000002&((data)<<1))
#define MIS_U1USR_tfnf_src(data)                                                     ((0x00000002&(data))>>1)
#define MIS_U1USR_get_tfnf(data)                                                     ((0x00000002&(data))>>1)
#define MIS_U1USR_busy_shift                                                         (0)
#define MIS_U1USR_busy_mask                                                          (0x00000001)
#define MIS_U1USR_busy(data)                                                         (0x00000001&((data)<<0))
#define MIS_U1USR_busy_src(data)                                                     ((0x00000001&(data))>>0)
#define MIS_U1USR_get_busy(data)                                                     ((0x00000001&(data))>>0)


#define MIS_U1TFL                                                                    0x1801BC80
#define MIS_U1TFL_reg_addr                                                           "0xB801BC80"
#define MIS_U1TFL_reg                                                                0xB801BC80
#define set_MIS_U1TFL_reg(data)   (*((volatile unsigned int*) MIS_U1TFL_reg)=data)
#define get_MIS_U1TFL_reg   (*((volatile unsigned int*) MIS_U1TFL_reg))
#define MIS_U1TFL_inst_adr                                                           "0x0020"
#define MIS_U1TFL_inst                                                               0x0020
#define MIS_U1TFL_tfl_shift                                                          (0)
#define MIS_U1TFL_tfl_mask                                                           (0x000000FF)
#define MIS_U1TFL_tfl(data)                                                          (0x000000FF&((data)<<0))
#define MIS_U1TFL_tfl_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_U1TFL_get_tfl(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_U1RFL                                                                    0x1801BC84
#define MIS_U1RFL_reg_addr                                                           "0xB801BC84"
#define MIS_U1RFL_reg                                                                0xB801BC84
#define set_MIS_U1RFL_reg(data)   (*((volatile unsigned int*) MIS_U1RFL_reg)=data)
#define get_MIS_U1RFL_reg   (*((volatile unsigned int*) MIS_U1RFL_reg))
#define MIS_U1RFL_inst_adr                                                           "0x0021"
#define MIS_U1RFL_inst                                                               0x0021
#define MIS_U1RFL_rfl_shift                                                          (0)
#define MIS_U1RFL_rfl_mask                                                           (0x000000FF)
#define MIS_U1RFL_rfl(data)                                                          (0x000000FF&((data)<<0))
#define MIS_U1RFL_rfl_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_U1RFL_get_rfl(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_U1SRR                                                                    0x1801BC88
#define MIS_U1SRR_reg_addr                                                           "0xB801BC88"
#define MIS_U1SRR_reg                                                                0xB801BC88
#define set_MIS_U1SRR_reg(data)   (*((volatile unsigned int*) MIS_U1SRR_reg)=data)
#define get_MIS_U1SRR_reg   (*((volatile unsigned int*) MIS_U1SRR_reg))
#define MIS_U1SRR_inst_adr                                                           "0x0022"
#define MIS_U1SRR_inst                                                               0x0022
#define MIS_U1SRR_xfr_shift                                                          (2)
#define MIS_U1SRR_xfr_mask                                                           (0x00000004)
#define MIS_U1SRR_xfr(data)                                                          (0x00000004&((data)<<2))
#define MIS_U1SRR_xfr_src(data)                                                      ((0x00000004&(data))>>2)
#define MIS_U1SRR_get_xfr(data)                                                      ((0x00000004&(data))>>2)
#define MIS_U1SRR_rfr_shift                                                          (1)
#define MIS_U1SRR_rfr_mask                                                           (0x00000002)
#define MIS_U1SRR_rfr(data)                                                          (0x00000002&((data)<<1))
#define MIS_U1SRR_rfr_src(data)                                                      ((0x00000002&(data))>>1)
#define MIS_U1SRR_get_rfr(data)                                                      ((0x00000002&(data))>>1)
#define MIS_U1SRR_ur_shift                                                           (0)
#define MIS_U1SRR_ur_mask                                                            (0x00000001)
#define MIS_U1SRR_ur(data)                                                           (0x00000001&((data)<<0))
#define MIS_U1SRR_ur_src(data)                                                       ((0x00000001&(data))>>0)
#define MIS_U1SRR_get_ur(data)                                                       ((0x00000001&(data))>>0)


#define MIS_U1SBCR                                                                   0x1801BC90
#define MIS_U1SBCR_reg_addr                                                          "0xB801BC90"
#define MIS_U1SBCR_reg                                                               0xB801BC90
#define set_MIS_U1SBCR_reg(data)   (*((volatile unsigned int*) MIS_U1SBCR_reg)=data)
#define get_MIS_U1SBCR_reg   (*((volatile unsigned int*) MIS_U1SBCR_reg))
#define MIS_U1SBCR_inst_adr                                                          "0x0024"
#define MIS_U1SBCR_inst                                                              0x0024
#define MIS_U1SBCR_sbcr_shift                                                        (0)
#define MIS_U1SBCR_sbcr_mask                                                         (0x00000001)
#define MIS_U1SBCR_sbcr(data)                                                        (0x00000001&((data)<<0))
#define MIS_U1SBCR_sbcr_src(data)                                                    ((0x00000001&(data))>>0)
#define MIS_U1SBCR_get_sbcr(data)                                                    ((0x00000001&(data))>>0)


#define MIS_U1SDMAM                                                                  0x1801BC94
#define MIS_U1SDMAM_reg_addr                                                         "0xB801BC94"
#define MIS_U1SDMAM_reg                                                              0xB801BC94
#define set_MIS_U1SDMAM_reg(data)   (*((volatile unsigned int*) MIS_U1SDMAM_reg)=data)
#define get_MIS_U1SDMAM_reg   (*((volatile unsigned int*) MIS_U1SDMAM_reg))
#define MIS_U1SDMAM_inst_adr                                                         "0x0025"
#define MIS_U1SDMAM_inst                                                             0x0025
#define MIS_U1SDMAM_sdmam_shift                                                      (0)
#define MIS_U1SDMAM_sdmam_mask                                                       (0x00000001)
#define MIS_U1SDMAM_sdmam(data)                                                      (0x00000001&((data)<<0))
#define MIS_U1SDMAM_sdmam_src(data)                                                  ((0x00000001&(data))>>0)
#define MIS_U1SDMAM_get_sdmam(data)                                                  ((0x00000001&(data))>>0)


#define MIS_U1SFE                                                                    0x1801BC98
#define MIS_U1SFE_reg_addr                                                           "0xB801BC98"
#define MIS_U1SFE_reg                                                                0xB801BC98
#define set_MIS_U1SFE_reg(data)   (*((volatile unsigned int*) MIS_U1SFE_reg)=data)
#define get_MIS_U1SFE_reg   (*((volatile unsigned int*) MIS_U1SFE_reg))
#define MIS_U1SFE_inst_adr                                                           "0x0026"
#define MIS_U1SFE_inst                                                               0x0026
#define MIS_U1SFE_sfe_shift                                                          (0)
#define MIS_U1SFE_sfe_mask                                                           (0x00000001)
#define MIS_U1SFE_sfe(data)                                                          (0x00000001&((data)<<0))
#define MIS_U1SFE_sfe_src(data)                                                      ((0x00000001&(data))>>0)
#define MIS_U1SFE_get_sfe(data)                                                      ((0x00000001&(data))>>0)


#define MIS_U1SRT                                                                    0x1801BC9C
#define MIS_U1SRT_reg_addr                                                           "0xB801BC9C"
#define MIS_U1SRT_reg                                                                0xB801BC9C
#define set_MIS_U1SRT_reg(data)   (*((volatile unsigned int*) MIS_U1SRT_reg)=data)
#define get_MIS_U1SRT_reg   (*((volatile unsigned int*) MIS_U1SRT_reg))
#define MIS_U1SRT_inst_adr                                                           "0x0027"
#define MIS_U1SRT_inst                                                               0x0027
#define MIS_U1SRT_srt_shift                                                          (0)
#define MIS_U1SRT_srt_mask                                                           (0x00000003)
#define MIS_U1SRT_srt(data)                                                          (0x00000003&((data)<<0))
#define MIS_U1SRT_srt_src(data)                                                      ((0x00000003&(data))>>0)
#define MIS_U1SRT_get_srt(data)                                                      ((0x00000003&(data))>>0)


#define MIS_U1STET                                                                   0x1801BCA0
#define MIS_U1STET_reg_addr                                                          "0xB801BCA0"
#define MIS_U1STET_reg                                                               0xB801BCA0
#define set_MIS_U1STET_reg(data)   (*((volatile unsigned int*) MIS_U1STET_reg)=data)
#define get_MIS_U1STET_reg   (*((volatile unsigned int*) MIS_U1STET_reg))
#define MIS_U1STET_inst_adr                                                          "0x0028"
#define MIS_U1STET_inst                                                              0x0028
#define MIS_U1STET_stet_shift                                                        (0)
#define MIS_U1STET_stet_mask                                                         (0x00000003)
#define MIS_U1STET_stet(data)                                                        (0x00000003&((data)<<0))
#define MIS_U1STET_stet_src(data)                                                    ((0x00000003&(data))>>0)
#define MIS_U1STET_get_stet(data)                                                    ((0x00000003&(data))>>0)


#define MIS_U1HTX                                                                    0x1801BCA4
#define MIS_U1HTX_reg_addr                                                           "0xB801BCA4"
#define MIS_U1HTX_reg                                                                0xB801BCA4
#define set_MIS_U1HTX_reg(data)   (*((volatile unsigned int*) MIS_U1HTX_reg)=data)
#define get_MIS_U1HTX_reg   (*((volatile unsigned int*) MIS_U1HTX_reg))
#define MIS_U1HTX_inst_adr                                                           "0x0029"
#define MIS_U1HTX_inst                                                               0x0029
#define MIS_U1HTX_htx_shift                                                          (0)
#define MIS_U1HTX_htx_mask                                                           (0x00000001)
#define MIS_U1HTX_htx(data)                                                          (0x00000001&((data)<<0))
#define MIS_U1HTX_htx_src(data)                                                      ((0x00000001&(data))>>0)
#define MIS_U1HTX_get_htx(data)                                                      ((0x00000001&(data))>>0)


#define MIS_U1DMASA                                                                  0x1801BCA8
#define MIS_U1DMASA_reg_addr                                                         "0xB801BCA8"
#define MIS_U1DMASA_reg                                                              0xB801BCA8
#define set_MIS_U1DMASA_reg(data)   (*((volatile unsigned int*) MIS_U1DMASA_reg)=data)
#define get_MIS_U1DMASA_reg   (*((volatile unsigned int*) MIS_U1DMASA_reg))
#define MIS_U1DMASA_inst_adr                                                         "0x002A"
#define MIS_U1DMASA_inst                                                             0x002A
#define MIS_U1DMASA_dmasa_shift                                                      (0)
#define MIS_U1DMASA_dmasa_mask                                                       (0x00000001)
#define MIS_U1DMASA_dmasa(data)                                                      (0x00000001&((data)<<0))
#define MIS_U1DMASA_dmasa_src(data)                                                  ((0x00000001&(data))>>0)
#define MIS_U1DMASA_get_dmasa(data)                                                  ((0x00000001&(data))>>0)


#define MIS_U1CPR                                                                    0x1801BCF4
#define MIS_U1CPR_reg_addr                                                           "0xB801BCF4"
#define MIS_U1CPR_reg                                                                0xB801BCF4
#define set_MIS_U1CPR_reg(data)   (*((volatile unsigned int*) MIS_U1CPR_reg)=data)
#define get_MIS_U1CPR_reg   (*((volatile unsigned int*) MIS_U1CPR_reg))
#define MIS_U1CPR_inst_adr                                                           "0x003D"
#define MIS_U1CPR_inst                                                               0x003D
#define MIS_U1CPR_fifo_mode_shift                                                    (16)
#define MIS_U1CPR_fifo_mode_mask                                                     (0x00FF0000)
#define MIS_U1CPR_fifo_mode(data)                                                    (0x00FF0000&((data)<<16))
#define MIS_U1CPR_fifo_mode_src(data)                                                ((0x00FF0000&(data))>>16)
#define MIS_U1CPR_get_fifo_mode(data)                                                ((0x00FF0000&(data))>>16)
#define MIS_U1CPR_dma_extra_shift                                                    (13)
#define MIS_U1CPR_dma_extra_mask                                                     (0x00002000)
#define MIS_U1CPR_dma_extra(data)                                                    (0x00002000&((data)<<13))
#define MIS_U1CPR_dma_extra_src(data)                                                ((0x00002000&(data))>>13)
#define MIS_U1CPR_get_dma_extra(data)                                                ((0x00002000&(data))>>13)
#define MIS_U1CPR_uart_add_encoded_params_shift                                      (12)
#define MIS_U1CPR_uart_add_encoded_params_mask                                       (0x00001000)
#define MIS_U1CPR_uart_add_encoded_params(data)                                      (0x00001000&((data)<<12))
#define MIS_U1CPR_uart_add_encoded_params_src(data)                                  ((0x00001000&(data))>>12)
#define MIS_U1CPR_get_uart_add_encoded_params(data)                                  ((0x00001000&(data))>>12)
#define MIS_U1CPR_shadow_shift                                                       (11)
#define MIS_U1CPR_shadow_mask                                                        (0x00000800)
#define MIS_U1CPR_shadow(data)                                                       (0x00000800&((data)<<11))
#define MIS_U1CPR_shadow_src(data)                                                   ((0x00000800&(data))>>11)
#define MIS_U1CPR_get_shadow(data)                                                   ((0x00000800&(data))>>11)
#define MIS_U1CPR_fifo_stat_shift                                                    (10)
#define MIS_U1CPR_fifo_stat_mask                                                     (0x00000400)
#define MIS_U1CPR_fifo_stat(data)                                                    (0x00000400&((data)<<10))
#define MIS_U1CPR_fifo_stat_src(data)                                                ((0x00000400&(data))>>10)
#define MIS_U1CPR_get_fifo_stat(data)                                                ((0x00000400&(data))>>10)
#define MIS_U1CPR_fifo_access_shift                                                  (9)
#define MIS_U1CPR_fifo_access_mask                                                   (0x00000200)
#define MIS_U1CPR_fifo_access(data)                                                  (0x00000200&((data)<<9))
#define MIS_U1CPR_fifo_access_src(data)                                              ((0x00000200&(data))>>9)
#define MIS_U1CPR_get_fifo_access(data)                                              ((0x00000200&(data))>>9)
#define MIS_U1CPR_additional_feat_shift                                              (8)
#define MIS_U1CPR_additional_feat_mask                                               (0x00000100)
#define MIS_U1CPR_additional_feat(data)                                              (0x00000100&((data)<<8))
#define MIS_U1CPR_additional_feat_src(data)                                          ((0x00000100&(data))>>8)
#define MIS_U1CPR_get_additional_feat(data)                                          ((0x00000100&(data))>>8)
#define MIS_U1CPR_sir_lp_mode_shift                                                  (7)
#define MIS_U1CPR_sir_lp_mode_mask                                                   (0x00000080)
#define MIS_U1CPR_sir_lp_mode(data)                                                  (0x00000080&((data)<<7))
#define MIS_U1CPR_sir_lp_mode_src(data)                                              ((0x00000080&(data))>>7)
#define MIS_U1CPR_get_sir_lp_mode(data)                                              ((0x00000080&(data))>>7)
#define MIS_U1CPR_sir_mode_shift                                                     (6)
#define MIS_U1CPR_sir_mode_mask                                                      (0x00000040)
#define MIS_U1CPR_sir_mode(data)                                                     (0x00000040&((data)<<6))
#define MIS_U1CPR_sir_mode_src(data)                                                 ((0x00000040&(data))>>6)
#define MIS_U1CPR_get_sir_mode(data)                                                 ((0x00000040&(data))>>6)
#define MIS_U1CPR_thre_mode_shift                                                    (5)
#define MIS_U1CPR_thre_mode_mask                                                     (0x00000020)
#define MIS_U1CPR_thre_mode(data)                                                    (0x00000020&((data)<<5))
#define MIS_U1CPR_thre_mode_src(data)                                                ((0x00000020&(data))>>5)
#define MIS_U1CPR_get_thre_mode(data)                                                ((0x00000020&(data))>>5)
#define MIS_U1CPR_afce_mode_shift                                                    (4)
#define MIS_U1CPR_afce_mode_mask                                                     (0x00000010)
#define MIS_U1CPR_afce_mode(data)                                                    (0x00000010&((data)<<4))
#define MIS_U1CPR_afce_mode_src(data)                                                ((0x00000010&(data))>>4)
#define MIS_U1CPR_get_afce_mode(data)                                                ((0x00000010&(data))>>4)
#define MIS_U1CPR_apb_data_width_shift                                               (0)
#define MIS_U1CPR_apb_data_width_mask                                                (0x00000003)
#define MIS_U1CPR_apb_data_width(data)                                               (0x00000003&((data)<<0))
#define MIS_U1CPR_apb_data_width_src(data)                                           ((0x00000003&(data))>>0)
#define MIS_U1CPR_get_apb_data_width(data)                                           ((0x00000003&(data))>>0)


#define MIS_U1UCV                                                                    0x1801BCF8
#define MIS_U1UCV_reg_addr                                                           "0xB801BCF8"
#define MIS_U1UCV_reg                                                                0xB801BCF8
#define set_MIS_U1UCV_reg(data)   (*((volatile unsigned int*) MIS_U1UCV_reg)=data)
#define get_MIS_U1UCV_reg   (*((volatile unsigned int*) MIS_U1UCV_reg))
#define MIS_U1UCV_inst_adr                                                           "0x003E"
#define MIS_U1UCV_inst                                                               0x003E
#define MIS_U1UCV_ucv_shift                                                          (0)
#define MIS_U1UCV_ucv_mask                                                           (0xFFFFFFFF)
#define MIS_U1UCV_ucv(data)                                                          (0xFFFFFFFF&((data)<<0))
#define MIS_U1UCV_ucv_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define MIS_U1UCV_get_ucv(data)                                                      ((0xFFFFFFFF&(data))>>0)


#define MIS_U1CTR                                                                    0x1801BCFC
#define MIS_U1CTR_reg_addr                                                           "0xB801BCFC"
#define MIS_U1CTR_reg                                                                0xB801BCFC
#define set_MIS_U1CTR_reg(data)   (*((volatile unsigned int*) MIS_U1CTR_reg)=data)
#define get_MIS_U1CTR_reg   (*((volatile unsigned int*) MIS_U1CTR_reg))
#define MIS_U1CTR_inst_adr                                                           "0x003F"
#define MIS_U1CTR_inst                                                               0x003F
#define MIS_U1CTR_ctr_shift                                                          (0)
#define MIS_U1CTR_ctr_mask                                                           (0xFFFFFFFF)
#define MIS_U1CTR_ctr(data)                                                          (0xFFFFFFFF&((data)<<0))
#define MIS_U1CTR_ctr_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define MIS_U1CTR_get_ctr(data)                                                      ((0xFFFFFFFF&(data))>>0)


#define MIS_IC_CON                                                                   0x1801B300
#define MIS_IC_CON_reg_addr                                                          "0xB801B300"
#define MIS_IC_CON_reg                                                               0xB801B300
#define set_MIS_IC_CON_reg(data)   (*((volatile unsigned int*) MIS_IC_CON_reg)=data)
#define get_MIS_IC_CON_reg   (*((volatile unsigned int*) MIS_IC_CON_reg))
#define MIS_IC_CON_inst_adr                                                          "0x00C0"
#define MIS_IC_CON_inst                                                              0x00C0
#define MIS_IC_CON_ic_slave_disable_shift                                            (6)
#define MIS_IC_CON_ic_slave_disable_mask                                             (0x00000040)
#define MIS_IC_CON_ic_slave_disable(data)                                            (0x00000040&((data)<<6))
#define MIS_IC_CON_ic_slave_disable_src(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC_CON_get_ic_slave_disable(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC_CON_ic_restart_en_shift                                               (5)
#define MIS_IC_CON_ic_restart_en_mask                                                (0x00000020)
#define MIS_IC_CON_ic_restart_en(data)                                               (0x00000020&((data)<<5))
#define MIS_IC_CON_ic_restart_en_src(data)                                           ((0x00000020&(data))>>5)
#define MIS_IC_CON_get_ic_restart_en(data)                                           ((0x00000020&(data))>>5)
#define MIS_IC_CON_ic_10bitaddr_master_shift                                         (4)
#define MIS_IC_CON_ic_10bitaddr_master_mask                                          (0x00000010)
#define MIS_IC_CON_ic_10bitaddr_master(data)                                         (0x00000010&((data)<<4))
#define MIS_IC_CON_ic_10bitaddr_master_src(data)                                     ((0x00000010&(data))>>4)
#define MIS_IC_CON_get_ic_10bitaddr_master(data)                                     ((0x00000010&(data))>>4)
#define MIS_IC_CON_ic_10bitaddr_slave_shift                                          (3)
#define MIS_IC_CON_ic_10bitaddr_slave_mask                                           (0x00000008)
#define MIS_IC_CON_ic_10bitaddr_slave(data)                                          (0x00000008&((data)<<3))
#define MIS_IC_CON_ic_10bitaddr_slave_src(data)                                      ((0x00000008&(data))>>3)
#define MIS_IC_CON_get_ic_10bitaddr_slave(data)                                      ((0x00000008&(data))>>3)
#define MIS_IC_CON_speed_shift                                                       (1)
#define MIS_IC_CON_speed_mask                                                        (0x00000006)
#define MIS_IC_CON_speed(data)                                                       (0x00000006&((data)<<1))
#define MIS_IC_CON_speed_src(data)                                                   ((0x00000006&(data))>>1)
#define MIS_IC_CON_get_speed(data)                                                   ((0x00000006&(data))>>1)
#define MIS_IC_CON_master_mode_shift                                                 (0)
#define MIS_IC_CON_master_mode_mask                                                  (0x00000001)
#define MIS_IC_CON_master_mode(data)                                                 (0x00000001&((data)<<0))
#define MIS_IC_CON_master_mode_src(data)                                             ((0x00000001&(data))>>0)
#define MIS_IC_CON_get_master_mode(data)                                             ((0x00000001&(data))>>0)


#define MIS_IC_TAR                                                                   0x1801B304
#define MIS_IC_TAR_reg_addr                                                          "0xB801B304"
#define MIS_IC_TAR_reg                                                               0xB801B304
#define set_MIS_IC_TAR_reg(data)   (*((volatile unsigned int*) MIS_IC_TAR_reg)=data)
#define get_MIS_IC_TAR_reg   (*((volatile unsigned int*) MIS_IC_TAR_reg))
#define MIS_IC_TAR_inst_adr                                                          "0x00C1"
#define MIS_IC_TAR_inst                                                              0x00C1
#define MIS_IC_TAR_ic_10bitaddr_master_shift                                         (12)
#define MIS_IC_TAR_ic_10bitaddr_master_mask                                          (0x00001000)
#define MIS_IC_TAR_ic_10bitaddr_master(data)                                         (0x00001000&((data)<<12))
#define MIS_IC_TAR_ic_10bitaddr_master_src(data)                                     ((0x00001000&(data))>>12)
#define MIS_IC_TAR_get_ic_10bitaddr_master(data)                                     ((0x00001000&(data))>>12)
#define MIS_IC_TAR_special_shift                                                     (11)
#define MIS_IC_TAR_special_mask                                                      (0x00000800)
#define MIS_IC_TAR_special(data)                                                     (0x00000800&((data)<<11))
#define MIS_IC_TAR_special_src(data)                                                 ((0x00000800&(data))>>11)
#define MIS_IC_TAR_get_special(data)                                                 ((0x00000800&(data))>>11)
#define MIS_IC_TAR_gc_or_start_shift                                                 (10)
#define MIS_IC_TAR_gc_or_start_mask                                                  (0x00000400)
#define MIS_IC_TAR_gc_or_start(data)                                                 (0x00000400&((data)<<10))
#define MIS_IC_TAR_gc_or_start_src(data)                                             ((0x00000400&(data))>>10)
#define MIS_IC_TAR_get_gc_or_start(data)                                             ((0x00000400&(data))>>10)
#define MIS_IC_TAR_ic_tar_shift                                                      (0)
#define MIS_IC_TAR_ic_tar_mask                                                       (0x000003FF)
#define MIS_IC_TAR_ic_tar(data)                                                      (0x000003FF&((data)<<0))
#define MIS_IC_TAR_ic_tar_src(data)                                                  ((0x000003FF&(data))>>0)
#define MIS_IC_TAR_get_ic_tar(data)                                                  ((0x000003FF&(data))>>0)


#define MIS_IC_SAR                                                                   0x1801B308
#define MIS_IC_SAR_reg_addr                                                          "0xB801B308"
#define MIS_IC_SAR_reg                                                               0xB801B308
#define set_MIS_IC_SAR_reg(data)   (*((volatile unsigned int*) MIS_IC_SAR_reg)=data)
#define get_MIS_IC_SAR_reg   (*((volatile unsigned int*) MIS_IC_SAR_reg))
#define MIS_IC_SAR_inst_adr                                                          "0x00C2"
#define MIS_IC_SAR_inst                                                              0x00C2
#define MIS_IC_SAR_ic_sar_shift                                                      (0)
#define MIS_IC_SAR_ic_sar_mask                                                       (0x000003FF)
#define MIS_IC_SAR_ic_sar(data)                                                      (0x000003FF&((data)<<0))
#define MIS_IC_SAR_ic_sar_src(data)                                                  ((0x000003FF&(data))>>0)
#define MIS_IC_SAR_get_ic_sar(data)                                                  ((0x000003FF&(data))>>0)


#define MIS_IC_HS_MADDR                                                              0x1801B30C
#define MIS_IC_HS_MADDR_reg_addr                                                     "0xB801B30C"
#define MIS_IC_HS_MADDR_reg                                                          0xB801B30C
#define set_MIS_IC_HS_MADDR_reg(data)   (*((volatile unsigned int*) MIS_IC_HS_MADDR_reg)=data)
#define get_MIS_IC_HS_MADDR_reg   (*((volatile unsigned int*) MIS_IC_HS_MADDR_reg))
#define MIS_IC_HS_MADDR_inst_adr                                                     "0x00C3"
#define MIS_IC_HS_MADDR_inst                                                         0x00C3
#define MIS_IC_HS_MADDR_ic_hs_mar_shift                                              (0)
#define MIS_IC_HS_MADDR_ic_hs_mar_mask                                               (0x00000007)
#define MIS_IC_HS_MADDR_ic_hs_mar(data)                                              (0x00000007&((data)<<0))
#define MIS_IC_HS_MADDR_ic_hs_mar_src(data)                                          ((0x00000007&(data))>>0)
#define MIS_IC_HS_MADDR_get_ic_hs_mar(data)                                          ((0x00000007&(data))>>0)


#define MIS_IC_DATA_CMD                                                              0x1801B310
#define MIS_IC_DATA_CMD_reg_addr                                                     "0xB801B310"
#define MIS_IC_DATA_CMD_reg                                                          0xB801B310
#define set_MIS_IC_DATA_CMD_reg(data)   (*((volatile unsigned int*) MIS_IC_DATA_CMD_reg)=data)
#define get_MIS_IC_DATA_CMD_reg   (*((volatile unsigned int*) MIS_IC_DATA_CMD_reg))
#define MIS_IC_DATA_CMD_inst_adr                                                     "0x00C4"
#define MIS_IC_DATA_CMD_inst                                                         0x00C4
#define MIS_IC_DATA_CMD_cmd_shift                                                    (8)
#define MIS_IC_DATA_CMD_cmd_mask                                                     (0x00000100)
#define MIS_IC_DATA_CMD_cmd(data)                                                    (0x00000100&((data)<<8))
#define MIS_IC_DATA_CMD_cmd_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_IC_DATA_CMD_get_cmd(data)                                                ((0x00000100&(data))>>8)
#define MIS_IC_DATA_CMD_dat_shift                                                    (0)
#define MIS_IC_DATA_CMD_dat_mask                                                     (0x000000FF)
#define MIS_IC_DATA_CMD_dat(data)                                                    (0x000000FF&((data)<<0))
#define MIS_IC_DATA_CMD_dat_src(data)                                                ((0x000000FF&(data))>>0)
#define MIS_IC_DATA_CMD_get_dat(data)                                                ((0x000000FF&(data))>>0)


#define MIS_IC_SS_SCL_HCNT                                                           0x1801B314
#define MIS_IC_SS_SCL_HCNT_reg_addr                                                  "0xB801B314"
#define MIS_IC_SS_SCL_HCNT_reg                                                       0xB801B314
#define set_MIS_IC_SS_SCL_HCNT_reg(data)   (*((volatile unsigned int*) MIS_IC_SS_SCL_HCNT_reg)=data)
#define get_MIS_IC_SS_SCL_HCNT_reg   (*((volatile unsigned int*) MIS_IC_SS_SCL_HCNT_reg))
#define MIS_IC_SS_SCL_HCNT_inst_adr                                                  "0x00C5"
#define MIS_IC_SS_SCL_HCNT_inst                                                      0x00C5
#define MIS_IC_SS_SCL_HCNT_ic_ss_scl_hcnt_shift                                      (0)
#define MIS_IC_SS_SCL_HCNT_ic_ss_scl_hcnt_mask                                       (0x0000FFFF)
#define MIS_IC_SS_SCL_HCNT_ic_ss_scl_hcnt(data)                                      (0x0000FFFF&((data)<<0))
#define MIS_IC_SS_SCL_HCNT_ic_ss_scl_hcnt_src(data)                                  ((0x0000FFFF&(data))>>0)
#define MIS_IC_SS_SCL_HCNT_get_ic_ss_scl_hcnt(data)                                  ((0x0000FFFF&(data))>>0)


#define MIS_IC_SS_SCL_LCNT                                                           0x1801B318
#define MIS_IC_SS_SCL_LCNT_reg_addr                                                  "0xB801B318"
#define MIS_IC_SS_SCL_LCNT_reg                                                       0xB801B318
#define set_MIS_IC_SS_SCL_LCNT_reg(data)   (*((volatile unsigned int*) MIS_IC_SS_SCL_LCNT_reg)=data)
#define get_MIS_IC_SS_SCL_LCNT_reg   (*((volatile unsigned int*) MIS_IC_SS_SCL_LCNT_reg))
#define MIS_IC_SS_SCL_LCNT_inst_adr                                                  "0x00C6"
#define MIS_IC_SS_SCL_LCNT_inst                                                      0x00C6
#define MIS_IC_SS_SCL_LCNT_ic_ss_scl_lcnt_shift                                      (0)
#define MIS_IC_SS_SCL_LCNT_ic_ss_scl_lcnt_mask                                       (0x0000FFFF)
#define MIS_IC_SS_SCL_LCNT_ic_ss_scl_lcnt(data)                                      (0x0000FFFF&((data)<<0))
#define MIS_IC_SS_SCL_LCNT_ic_ss_scl_lcnt_src(data)                                  ((0x0000FFFF&(data))>>0)
#define MIS_IC_SS_SCL_LCNT_get_ic_ss_scl_lcnt(data)                                  ((0x0000FFFF&(data))>>0)


#define MIS_IC_FS_SCL_HCNT                                                           0x1801B31C
#define MIS_IC_FS_SCL_HCNT_reg_addr                                                  "0xB801B31C"
#define MIS_IC_FS_SCL_HCNT_reg                                                       0xB801B31C
#define set_MIS_IC_FS_SCL_HCNT_reg(data)   (*((volatile unsigned int*) MIS_IC_FS_SCL_HCNT_reg)=data)
#define get_MIS_IC_FS_SCL_HCNT_reg   (*((volatile unsigned int*) MIS_IC_FS_SCL_HCNT_reg))
#define MIS_IC_FS_SCL_HCNT_inst_adr                                                  "0x00C7"
#define MIS_IC_FS_SCL_HCNT_inst                                                      0x00C7
#define MIS_IC_FS_SCL_HCNT_ic_fs_scl_hcnt_shift                                      (0)
#define MIS_IC_FS_SCL_HCNT_ic_fs_scl_hcnt_mask                                       (0x0000FFFF)
#define MIS_IC_FS_SCL_HCNT_ic_fs_scl_hcnt(data)                                      (0x0000FFFF&((data)<<0))
#define MIS_IC_FS_SCL_HCNT_ic_fs_scl_hcnt_src(data)                                  ((0x0000FFFF&(data))>>0)
#define MIS_IC_FS_SCL_HCNT_get_ic_fs_scl_hcnt(data)                                  ((0x0000FFFF&(data))>>0)


#define MIS_IC_FS_SCL_LCNT                                                           0x1801B320
#define MIS_IC_FS_SCL_LCNT_reg_addr                                                  "0xB801B320"
#define MIS_IC_FS_SCL_LCNT_reg                                                       0xB801B320
#define set_MIS_IC_FS_SCL_LCNT_reg(data)   (*((volatile unsigned int*) MIS_IC_FS_SCL_LCNT_reg)=data)
#define get_MIS_IC_FS_SCL_LCNT_reg   (*((volatile unsigned int*) MIS_IC_FS_SCL_LCNT_reg))
#define MIS_IC_FS_SCL_LCNT_inst_adr                                                  "0x00C8"
#define MIS_IC_FS_SCL_LCNT_inst                                                      0x00C8
#define MIS_IC_FS_SCL_LCNT_ic_fs_scl_lcnt_shift                                      (0)
#define MIS_IC_FS_SCL_LCNT_ic_fs_scl_lcnt_mask                                       (0x0000FFFF)
#define MIS_IC_FS_SCL_LCNT_ic_fs_scl_lcnt(data)                                      (0x0000FFFF&((data)<<0))
#define MIS_IC_FS_SCL_LCNT_ic_fs_scl_lcnt_src(data)                                  ((0x0000FFFF&(data))>>0)
#define MIS_IC_FS_SCL_LCNT_get_ic_fs_scl_lcnt(data)                                  ((0x0000FFFF&(data))>>0)


#define MIS_IC_INTR_STAT                                                             0x1801B32C
#define MIS_IC_INTR_STAT_reg_addr                                                    "0xB801B32C"
#define MIS_IC_INTR_STAT_reg                                                         0xB801B32C
#define set_MIS_IC_INTR_STAT_reg(data)   (*((volatile unsigned int*) MIS_IC_INTR_STAT_reg)=data)
#define get_MIS_IC_INTR_STAT_reg   (*((volatile unsigned int*) MIS_IC_INTR_STAT_reg))
#define MIS_IC_INTR_STAT_inst_adr                                                    "0x00CB"
#define MIS_IC_INTR_STAT_inst                                                        0x00CB
#define MIS_IC_INTR_STAT_r_gen_call_shift                                            (11)
#define MIS_IC_INTR_STAT_r_gen_call_mask                                             (0x00000800)
#define MIS_IC_INTR_STAT_r_gen_call(data)                                            (0x00000800&((data)<<11))
#define MIS_IC_INTR_STAT_r_gen_call_src(data)                                        ((0x00000800&(data))>>11)
#define MIS_IC_INTR_STAT_get_r_gen_call(data)                                        ((0x00000800&(data))>>11)
#define MIS_IC_INTR_STAT_r_start_det_shift                                           (10)
#define MIS_IC_INTR_STAT_r_start_det_mask                                            (0x00000400)
#define MIS_IC_INTR_STAT_r_start_det(data)                                           (0x00000400&((data)<<10))
#define MIS_IC_INTR_STAT_r_start_det_src(data)                                       ((0x00000400&(data))>>10)
#define MIS_IC_INTR_STAT_get_r_start_det(data)                                       ((0x00000400&(data))>>10)
#define MIS_IC_INTR_STAT_r_stop_det_shift                                            (9)
#define MIS_IC_INTR_STAT_r_stop_det_mask                                             (0x00000200)
#define MIS_IC_INTR_STAT_r_stop_det(data)                                            (0x00000200&((data)<<9))
#define MIS_IC_INTR_STAT_r_stop_det_src(data)                                        ((0x00000200&(data))>>9)
#define MIS_IC_INTR_STAT_get_r_stop_det(data)                                        ((0x00000200&(data))>>9)
#define MIS_IC_INTR_STAT_r_activity_shift                                            (8)
#define MIS_IC_INTR_STAT_r_activity_mask                                             (0x00000100)
#define MIS_IC_INTR_STAT_r_activity(data)                                            (0x00000100&((data)<<8))
#define MIS_IC_INTR_STAT_r_activity_src(data)                                        ((0x00000100&(data))>>8)
#define MIS_IC_INTR_STAT_get_r_activity(data)                                        ((0x00000100&(data))>>8)
#define MIS_IC_INTR_STAT_r_rx_done_shift                                             (7)
#define MIS_IC_INTR_STAT_r_rx_done_mask                                              (0x00000080)
#define MIS_IC_INTR_STAT_r_rx_done(data)                                             (0x00000080&((data)<<7))
#define MIS_IC_INTR_STAT_r_rx_done_src(data)                                         ((0x00000080&(data))>>7)
#define MIS_IC_INTR_STAT_get_r_rx_done(data)                                         ((0x00000080&(data))>>7)
#define MIS_IC_INTR_STAT_r_tx_abrt_shift                                             (6)
#define MIS_IC_INTR_STAT_r_tx_abrt_mask                                              (0x00000040)
#define MIS_IC_INTR_STAT_r_tx_abrt(data)                                             (0x00000040&((data)<<6))
#define MIS_IC_INTR_STAT_r_tx_abrt_src(data)                                         ((0x00000040&(data))>>6)
#define MIS_IC_INTR_STAT_get_r_tx_abrt(data)                                         ((0x00000040&(data))>>6)
#define MIS_IC_INTR_STAT_r_rd_req_shift                                              (5)
#define MIS_IC_INTR_STAT_r_rd_req_mask                                               (0x00000020)
#define MIS_IC_INTR_STAT_r_rd_req(data)                                              (0x00000020&((data)<<5))
#define MIS_IC_INTR_STAT_r_rd_req_src(data)                                          ((0x00000020&(data))>>5)
#define MIS_IC_INTR_STAT_get_r_rd_req(data)                                          ((0x00000020&(data))>>5)
#define MIS_IC_INTR_STAT_r_tx_empty_shift                                            (4)
#define MIS_IC_INTR_STAT_r_tx_empty_mask                                             (0x00000010)
#define MIS_IC_INTR_STAT_r_tx_empty(data)                                            (0x00000010&((data)<<4))
#define MIS_IC_INTR_STAT_r_tx_empty_src(data)                                        ((0x00000010&(data))>>4)
#define MIS_IC_INTR_STAT_get_r_tx_empty(data)                                        ((0x00000010&(data))>>4)
#define MIS_IC_INTR_STAT_r_tx_over_shift                                             (3)
#define MIS_IC_INTR_STAT_r_tx_over_mask                                              (0x00000008)
#define MIS_IC_INTR_STAT_r_tx_over(data)                                             (0x00000008&((data)<<3))
#define MIS_IC_INTR_STAT_r_tx_over_src(data)                                         ((0x00000008&(data))>>3)
#define MIS_IC_INTR_STAT_get_r_tx_over(data)                                         ((0x00000008&(data))>>3)
#define MIS_IC_INTR_STAT_r_rx_full_shift                                             (2)
#define MIS_IC_INTR_STAT_r_rx_full_mask                                              (0x00000004)
#define MIS_IC_INTR_STAT_r_rx_full(data)                                             (0x00000004&((data)<<2))
#define MIS_IC_INTR_STAT_r_rx_full_src(data)                                         ((0x00000004&(data))>>2)
#define MIS_IC_INTR_STAT_get_r_rx_full(data)                                         ((0x00000004&(data))>>2)
#define MIS_IC_INTR_STAT_r_rx_over_shift                                             (1)
#define MIS_IC_INTR_STAT_r_rx_over_mask                                              (0x00000002)
#define MIS_IC_INTR_STAT_r_rx_over(data)                                             (0x00000002&((data)<<1))
#define MIS_IC_INTR_STAT_r_rx_over_src(data)                                         ((0x00000002&(data))>>1)
#define MIS_IC_INTR_STAT_get_r_rx_over(data)                                         ((0x00000002&(data))>>1)
#define MIS_IC_INTR_STAT_r_rx_under_shift                                            (0)
#define MIS_IC_INTR_STAT_r_rx_under_mask                                             (0x00000001)
#define MIS_IC_INTR_STAT_r_rx_under(data)                                            (0x00000001&((data)<<0))
#define MIS_IC_INTR_STAT_r_rx_under_src(data)                                        ((0x00000001&(data))>>0)
#define MIS_IC_INTR_STAT_get_r_rx_under(data)                                        ((0x00000001&(data))>>0)


#define MIS_IC_INTR_MASK                                                             0x1801B330
#define MIS_IC_INTR_MASK_reg_addr                                                    "0xB801B330"
#define MIS_IC_INTR_MASK_reg                                                         0xB801B330
#define set_MIS_IC_INTR_MASK_reg(data)   (*((volatile unsigned int*) MIS_IC_INTR_MASK_reg)=data)
#define get_MIS_IC_INTR_MASK_reg   (*((volatile unsigned int*) MIS_IC_INTR_MASK_reg))
#define MIS_IC_INTR_MASK_inst_adr                                                    "0x00CC"
#define MIS_IC_INTR_MASK_inst                                                        0x00CC
#define MIS_IC_INTR_MASK_m_gen_call_shift                                            (11)
#define MIS_IC_INTR_MASK_m_gen_call_mask                                             (0x00000800)
#define MIS_IC_INTR_MASK_m_gen_call(data)                                            (0x00000800&((data)<<11))
#define MIS_IC_INTR_MASK_m_gen_call_src(data)                                        ((0x00000800&(data))>>11)
#define MIS_IC_INTR_MASK_get_m_gen_call(data)                                        ((0x00000800&(data))>>11)
#define MIS_IC_INTR_MASK_m_start_det_shift                                           (10)
#define MIS_IC_INTR_MASK_m_start_det_mask                                            (0x00000400)
#define MIS_IC_INTR_MASK_m_start_det(data)                                           (0x00000400&((data)<<10))
#define MIS_IC_INTR_MASK_m_start_det_src(data)                                       ((0x00000400&(data))>>10)
#define MIS_IC_INTR_MASK_get_m_start_det(data)                                       ((0x00000400&(data))>>10)
#define MIS_IC_INTR_MASK_m_stop_det_shift                                            (9)
#define MIS_IC_INTR_MASK_m_stop_det_mask                                             (0x00000200)
#define MIS_IC_INTR_MASK_m_stop_det(data)                                            (0x00000200&((data)<<9))
#define MIS_IC_INTR_MASK_m_stop_det_src(data)                                        ((0x00000200&(data))>>9)
#define MIS_IC_INTR_MASK_get_m_stop_det(data)                                        ((0x00000200&(data))>>9)
#define MIS_IC_INTR_MASK_m_activity_shift                                            (8)
#define MIS_IC_INTR_MASK_m_activity_mask                                             (0x00000100)
#define MIS_IC_INTR_MASK_m_activity(data)                                            (0x00000100&((data)<<8))
#define MIS_IC_INTR_MASK_m_activity_src(data)                                        ((0x00000100&(data))>>8)
#define MIS_IC_INTR_MASK_get_m_activity(data)                                        ((0x00000100&(data))>>8)
#define MIS_IC_INTR_MASK_m_rx_done_shift                                             (7)
#define MIS_IC_INTR_MASK_m_rx_done_mask                                              (0x00000080)
#define MIS_IC_INTR_MASK_m_rx_done(data)                                             (0x00000080&((data)<<7))
#define MIS_IC_INTR_MASK_m_rx_done_src(data)                                         ((0x00000080&(data))>>7)
#define MIS_IC_INTR_MASK_get_m_rx_done(data)                                         ((0x00000080&(data))>>7)
#define MIS_IC_INTR_MASK_m_tx_abrt_shift                                             (6)
#define MIS_IC_INTR_MASK_m_tx_abrt_mask                                              (0x00000040)
#define MIS_IC_INTR_MASK_m_tx_abrt(data)                                             (0x00000040&((data)<<6))
#define MIS_IC_INTR_MASK_m_tx_abrt_src(data)                                         ((0x00000040&(data))>>6)
#define MIS_IC_INTR_MASK_get_m_tx_abrt(data)                                         ((0x00000040&(data))>>6)
#define MIS_IC_INTR_MASK_m_rd_req_shift                                              (5)
#define MIS_IC_INTR_MASK_m_rd_req_mask                                               (0x00000020)
#define MIS_IC_INTR_MASK_m_rd_req(data)                                              (0x00000020&((data)<<5))
#define MIS_IC_INTR_MASK_m_rd_req_src(data)                                          ((0x00000020&(data))>>5)
#define MIS_IC_INTR_MASK_get_m_rd_req(data)                                          ((0x00000020&(data))>>5)
#define MIS_IC_INTR_MASK_m_tx_empty_shift                                            (4)
#define MIS_IC_INTR_MASK_m_tx_empty_mask                                             (0x00000010)
#define MIS_IC_INTR_MASK_m_tx_empty(data)                                            (0x00000010&((data)<<4))
#define MIS_IC_INTR_MASK_m_tx_empty_src(data)                                        ((0x00000010&(data))>>4)
#define MIS_IC_INTR_MASK_get_m_tx_empty(data)                                        ((0x00000010&(data))>>4)
#define MIS_IC_INTR_MASK_m_tx_over_shift                                             (3)
#define MIS_IC_INTR_MASK_m_tx_over_mask                                              (0x00000008)
#define MIS_IC_INTR_MASK_m_tx_over(data)                                             (0x00000008&((data)<<3))
#define MIS_IC_INTR_MASK_m_tx_over_src(data)                                         ((0x00000008&(data))>>3)
#define MIS_IC_INTR_MASK_get_m_tx_over(data)                                         ((0x00000008&(data))>>3)
#define MIS_IC_INTR_MASK_m_rx_full_shift                                             (2)
#define MIS_IC_INTR_MASK_m_rx_full_mask                                              (0x00000004)
#define MIS_IC_INTR_MASK_m_rx_full(data)                                             (0x00000004&((data)<<2))
#define MIS_IC_INTR_MASK_m_rx_full_src(data)                                         ((0x00000004&(data))>>2)
#define MIS_IC_INTR_MASK_get_m_rx_full(data)                                         ((0x00000004&(data))>>2)
#define MIS_IC_INTR_MASK_m_rx_over_shift                                             (1)
#define MIS_IC_INTR_MASK_m_rx_over_mask                                              (0x00000002)
#define MIS_IC_INTR_MASK_m_rx_over(data)                                             (0x00000002&((data)<<1))
#define MIS_IC_INTR_MASK_m_rx_over_src(data)                                         ((0x00000002&(data))>>1)
#define MIS_IC_INTR_MASK_get_m_rx_over(data)                                         ((0x00000002&(data))>>1)
#define MIS_IC_INTR_MASK_m_rx_under_shift                                            (0)
#define MIS_IC_INTR_MASK_m_rx_under_mask                                             (0x00000001)
#define MIS_IC_INTR_MASK_m_rx_under(data)                                            (0x00000001&((data)<<0))
#define MIS_IC_INTR_MASK_m_rx_under_src(data)                                        ((0x00000001&(data))>>0)
#define MIS_IC_INTR_MASK_get_m_rx_under(data)                                        ((0x00000001&(data))>>0)


#define MIS_IC_RAW_INTR_STAT                                                         0x1801B334
#define MIS_IC_RAW_INTR_STAT_reg_addr                                                "0xB801B334"
#define MIS_IC_RAW_INTR_STAT_reg                                                     0xB801B334
#define set_MIS_IC_RAW_INTR_STAT_reg(data)   (*((volatile unsigned int*) MIS_IC_RAW_INTR_STAT_reg)=data)
#define get_MIS_IC_RAW_INTR_STAT_reg   (*((volatile unsigned int*) MIS_IC_RAW_INTR_STAT_reg))
#define MIS_IC_RAW_INTR_STAT_inst_adr                                                "0x00CD"
#define MIS_IC_RAW_INTR_STAT_inst                                                    0x00CD
#define MIS_IC_RAW_INTR_STAT_gen_call_shift                                          (11)
#define MIS_IC_RAW_INTR_STAT_gen_call_mask                                           (0x00000800)
#define MIS_IC_RAW_INTR_STAT_gen_call(data)                                          (0x00000800&((data)<<11))
#define MIS_IC_RAW_INTR_STAT_gen_call_src(data)                                      ((0x00000800&(data))>>11)
#define MIS_IC_RAW_INTR_STAT_get_gen_call(data)                                      ((0x00000800&(data))>>11)
#define MIS_IC_RAW_INTR_STAT_start_det_shift                                         (10)
#define MIS_IC_RAW_INTR_STAT_start_det_mask                                          (0x00000400)
#define MIS_IC_RAW_INTR_STAT_start_det(data)                                         (0x00000400&((data)<<10))
#define MIS_IC_RAW_INTR_STAT_start_det_src(data)                                     ((0x00000400&(data))>>10)
#define MIS_IC_RAW_INTR_STAT_get_start_det(data)                                     ((0x00000400&(data))>>10)
#define MIS_IC_RAW_INTR_STAT_stop_det_shift                                          (9)
#define MIS_IC_RAW_INTR_STAT_stop_det_mask                                           (0x00000200)
#define MIS_IC_RAW_INTR_STAT_stop_det(data)                                          (0x00000200&((data)<<9))
#define MIS_IC_RAW_INTR_STAT_stop_det_src(data)                                      ((0x00000200&(data))>>9)
#define MIS_IC_RAW_INTR_STAT_get_stop_det(data)                                      ((0x00000200&(data))>>9)
#define MIS_IC_RAW_INTR_STAT_activity_shift                                          (8)
#define MIS_IC_RAW_INTR_STAT_activity_mask                                           (0x00000100)
#define MIS_IC_RAW_INTR_STAT_activity(data)                                          (0x00000100&((data)<<8))
#define MIS_IC_RAW_INTR_STAT_activity_src(data)                                      ((0x00000100&(data))>>8)
#define MIS_IC_RAW_INTR_STAT_get_activity(data)                                      ((0x00000100&(data))>>8)
#define MIS_IC_RAW_INTR_STAT_rx_done_shift                                           (7)
#define MIS_IC_RAW_INTR_STAT_rx_done_mask                                            (0x00000080)
#define MIS_IC_RAW_INTR_STAT_rx_done(data)                                           (0x00000080&((data)<<7))
#define MIS_IC_RAW_INTR_STAT_rx_done_src(data)                                       ((0x00000080&(data))>>7)
#define MIS_IC_RAW_INTR_STAT_get_rx_done(data)                                       ((0x00000080&(data))>>7)
#define MIS_IC_RAW_INTR_STAT_tx_abrt_shift                                           (6)
#define MIS_IC_RAW_INTR_STAT_tx_abrt_mask                                            (0x00000040)
#define MIS_IC_RAW_INTR_STAT_tx_abrt(data)                                           (0x00000040&((data)<<6))
#define MIS_IC_RAW_INTR_STAT_tx_abrt_src(data)                                       ((0x00000040&(data))>>6)
#define MIS_IC_RAW_INTR_STAT_get_tx_abrt(data)                                       ((0x00000040&(data))>>6)
#define MIS_IC_RAW_INTR_STAT_rd_req_shift                                            (5)
#define MIS_IC_RAW_INTR_STAT_rd_req_mask                                             (0x00000020)
#define MIS_IC_RAW_INTR_STAT_rd_req(data)                                            (0x00000020&((data)<<5))
#define MIS_IC_RAW_INTR_STAT_rd_req_src(data)                                        ((0x00000020&(data))>>5)
#define MIS_IC_RAW_INTR_STAT_get_rd_req(data)                                        ((0x00000020&(data))>>5)
#define MIS_IC_RAW_INTR_STAT_tx_empty_shift                                          (4)
#define MIS_IC_RAW_INTR_STAT_tx_empty_mask                                           (0x00000010)
#define MIS_IC_RAW_INTR_STAT_tx_empty(data)                                          (0x00000010&((data)<<4))
#define MIS_IC_RAW_INTR_STAT_tx_empty_src(data)                                      ((0x00000010&(data))>>4)
#define MIS_IC_RAW_INTR_STAT_get_tx_empty(data)                                      ((0x00000010&(data))>>4)
#define MIS_IC_RAW_INTR_STAT_tx_over_shift                                           (3)
#define MIS_IC_RAW_INTR_STAT_tx_over_mask                                            (0x00000008)
#define MIS_IC_RAW_INTR_STAT_tx_over(data)                                           (0x00000008&((data)<<3))
#define MIS_IC_RAW_INTR_STAT_tx_over_src(data)                                       ((0x00000008&(data))>>3)
#define MIS_IC_RAW_INTR_STAT_get_tx_over(data)                                       ((0x00000008&(data))>>3)
#define MIS_IC_RAW_INTR_STAT_rx_full_shift                                           (2)
#define MIS_IC_RAW_INTR_STAT_rx_full_mask                                            (0x00000004)
#define MIS_IC_RAW_INTR_STAT_rx_full(data)                                           (0x00000004&((data)<<2))
#define MIS_IC_RAW_INTR_STAT_rx_full_src(data)                                       ((0x00000004&(data))>>2)
#define MIS_IC_RAW_INTR_STAT_get_rx_full(data)                                       ((0x00000004&(data))>>2)
#define MIS_IC_RAW_INTR_STAT_rx_over_shift                                           (1)
#define MIS_IC_RAW_INTR_STAT_rx_over_mask                                            (0x00000002)
#define MIS_IC_RAW_INTR_STAT_rx_over(data)                                           (0x00000002&((data)<<1))
#define MIS_IC_RAW_INTR_STAT_rx_over_src(data)                                       ((0x00000002&(data))>>1)
#define MIS_IC_RAW_INTR_STAT_get_rx_over(data)                                       ((0x00000002&(data))>>1)
#define MIS_IC_RAW_INTR_STAT_rx_under_shift                                          (0)
#define MIS_IC_RAW_INTR_STAT_rx_under_mask                                           (0x00000001)
#define MIS_IC_RAW_INTR_STAT_rx_under(data)                                          (0x00000001&((data)<<0))
#define MIS_IC_RAW_INTR_STAT_rx_under_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_IC_RAW_INTR_STAT_get_rx_under(data)                                      ((0x00000001&(data))>>0)


#define MIS_IC_RX_TL                                                                 0x1801B338
#define MIS_IC_RX_TL_reg_addr                                                        "0xB801B338"
#define MIS_IC_RX_TL_reg                                                             0xB801B338
#define set_MIS_IC_RX_TL_reg(data)   (*((volatile unsigned int*) MIS_IC_RX_TL_reg)=data)
#define get_MIS_IC_RX_TL_reg   (*((volatile unsigned int*) MIS_IC_RX_TL_reg))
#define MIS_IC_RX_TL_inst_adr                                                        "0x00CE"
#define MIS_IC_RX_TL_inst                                                            0x00CE
#define MIS_IC_RX_TL_rx_tl_shift                                                     (0)
#define MIS_IC_RX_TL_rx_tl_mask                                                      (0x000000FF)
#define MIS_IC_RX_TL_rx_tl(data)                                                     (0x000000FF&((data)<<0))
#define MIS_IC_RX_TL_rx_tl_src(data)                                                 ((0x000000FF&(data))>>0)
#define MIS_IC_RX_TL_get_rx_tl(data)                                                 ((0x000000FF&(data))>>0)


#define MIS_IC_TX_TL                                                                 0x1801B33C
#define MIS_IC_TX_TL_reg_addr                                                        "0xB801B33C"
#define MIS_IC_TX_TL_reg                                                             0xB801B33C
#define set_MIS_IC_TX_TL_reg(data)   (*((volatile unsigned int*) MIS_IC_TX_TL_reg)=data)
#define get_MIS_IC_TX_TL_reg   (*((volatile unsigned int*) MIS_IC_TX_TL_reg))
#define MIS_IC_TX_TL_inst_adr                                                        "0x00CF"
#define MIS_IC_TX_TL_inst                                                            0x00CF
#define MIS_IC_TX_TL_tx_tl_shift                                                     (0)
#define MIS_IC_TX_TL_tx_tl_mask                                                      (0x000000FF)
#define MIS_IC_TX_TL_tx_tl(data)                                                     (0x000000FF&((data)<<0))
#define MIS_IC_TX_TL_tx_tl_src(data)                                                 ((0x000000FF&(data))>>0)
#define MIS_IC_TX_TL_get_tx_tl(data)                                                 ((0x000000FF&(data))>>0)


#define MIS_IC_CLR_INTR                                                              0x1801B340
#define MIS_IC_CLR_INTR_reg_addr                                                     "0xB801B340"
#define MIS_IC_CLR_INTR_reg                                                          0xB801B340
#define set_MIS_IC_CLR_INTR_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_INTR_reg)=data)
#define get_MIS_IC_CLR_INTR_reg   (*((volatile unsigned int*) MIS_IC_CLR_INTR_reg))
#define MIS_IC_CLR_INTR_inst_adr                                                     "0x00D0"
#define MIS_IC_CLR_INTR_inst                                                         0x00D0
#define MIS_IC_CLR_INTR_clr_intr_shift                                               (0)
#define MIS_IC_CLR_INTR_clr_intr_mask                                                (0x00000001)
#define MIS_IC_CLR_INTR_clr_intr(data)                                               (0x00000001&((data)<<0))
#define MIS_IC_CLR_INTR_clr_intr_src(data)                                           ((0x00000001&(data))>>0)
#define MIS_IC_CLR_INTR_get_clr_intr(data)                                           ((0x00000001&(data))>>0)


#define MIS_IC_CLR_RX_UNDER                                                          0x1801B344
#define MIS_IC_CLR_RX_UNDER_reg_addr                                                 "0xB801B344"
#define MIS_IC_CLR_RX_UNDER_reg                                                      0xB801B344
#define set_MIS_IC_CLR_RX_UNDER_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_RX_UNDER_reg)=data)
#define get_MIS_IC_CLR_RX_UNDER_reg   (*((volatile unsigned int*) MIS_IC_CLR_RX_UNDER_reg))
#define MIS_IC_CLR_RX_UNDER_inst_adr                                                 "0x00D1"
#define MIS_IC_CLR_RX_UNDER_inst                                                     0x00D1
#define MIS_IC_CLR_RX_UNDER_clr_rx_under_shift                                       (0)
#define MIS_IC_CLR_RX_UNDER_clr_rx_under_mask                                        (0x00000001)
#define MIS_IC_CLR_RX_UNDER_clr_rx_under(data)                                       (0x00000001&((data)<<0))
#define MIS_IC_CLR_RX_UNDER_clr_rx_under_src(data)                                   ((0x00000001&(data))>>0)
#define MIS_IC_CLR_RX_UNDER_get_clr_rx_under(data)                                   ((0x00000001&(data))>>0)


#define MIS_IC_CLR_RX_OVER                                                           0x1801B348
#define MIS_IC_CLR_RX_OVER_reg_addr                                                  "0xB801B348"
#define MIS_IC_CLR_RX_OVER_reg                                                       0xB801B348
#define set_MIS_IC_CLR_RX_OVER_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_RX_OVER_reg)=data)
#define get_MIS_IC_CLR_RX_OVER_reg   (*((volatile unsigned int*) MIS_IC_CLR_RX_OVER_reg))
#define MIS_IC_CLR_RX_OVER_inst_adr                                                  "0x00D2"
#define MIS_IC_CLR_RX_OVER_inst                                                      0x00D2
#define MIS_IC_CLR_RX_OVER_clr_rx_over_shift                                         (0)
#define MIS_IC_CLR_RX_OVER_clr_rx_over_mask                                          (0x00000001)
#define MIS_IC_CLR_RX_OVER_clr_rx_over(data)                                         (0x00000001&((data)<<0))
#define MIS_IC_CLR_RX_OVER_clr_rx_over_src(data)                                     ((0x00000001&(data))>>0)
#define MIS_IC_CLR_RX_OVER_get_clr_rx_over(data)                                     ((0x00000001&(data))>>0)


#define MIS_IC_CLR_TX_OVER                                                           0x1801B34C
#define MIS_IC_CLR_TX_OVER_reg_addr                                                  "0xB801B34C"
#define MIS_IC_CLR_TX_OVER_reg                                                       0xB801B34C
#define set_MIS_IC_CLR_TX_OVER_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_TX_OVER_reg)=data)
#define get_MIS_IC_CLR_TX_OVER_reg   (*((volatile unsigned int*) MIS_IC_CLR_TX_OVER_reg))
#define MIS_IC_CLR_TX_OVER_inst_adr                                                  "0x00D3"
#define MIS_IC_CLR_TX_OVER_inst                                                      0x00D3
#define MIS_IC_CLR_TX_OVER_clr_tx_over_shift                                         (0)
#define MIS_IC_CLR_TX_OVER_clr_tx_over_mask                                          (0x00000001)
#define MIS_IC_CLR_TX_OVER_clr_tx_over(data)                                         (0x00000001&((data)<<0))
#define MIS_IC_CLR_TX_OVER_clr_tx_over_src(data)                                     ((0x00000001&(data))>>0)
#define MIS_IC_CLR_TX_OVER_get_clr_tx_over(data)                                     ((0x00000001&(data))>>0)


#define MIS_IC_CLR_RD_REQ                                                            0x1801B350
#define MIS_IC_CLR_RD_REQ_reg_addr                                                   "0xB801B350"
#define MIS_IC_CLR_RD_REQ_reg                                                        0xB801B350
#define set_MIS_IC_CLR_RD_REQ_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_RD_REQ_reg)=data)
#define get_MIS_IC_CLR_RD_REQ_reg   (*((volatile unsigned int*) MIS_IC_CLR_RD_REQ_reg))
#define MIS_IC_CLR_RD_REQ_inst_adr                                                   "0x00D4"
#define MIS_IC_CLR_RD_REQ_inst                                                       0x00D4
#define MIS_IC_CLR_RD_REQ_clr_rd_req_shift                                           (0)
#define MIS_IC_CLR_RD_REQ_clr_rd_req_mask                                            (0x00000001)
#define MIS_IC_CLR_RD_REQ_clr_rd_req(data)                                           (0x00000001&((data)<<0))
#define MIS_IC_CLR_RD_REQ_clr_rd_req_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_IC_CLR_RD_REQ_get_clr_rd_req(data)                                       ((0x00000001&(data))>>0)


#define MIS_IC_CLR_TX_ABRT                                                           0x1801B354
#define MIS_IC_CLR_TX_ABRT_reg_addr                                                  "0xB801B354"
#define MIS_IC_CLR_TX_ABRT_reg                                                       0xB801B354
#define set_MIS_IC_CLR_TX_ABRT_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_TX_ABRT_reg)=data)
#define get_MIS_IC_CLR_TX_ABRT_reg   (*((volatile unsigned int*) MIS_IC_CLR_TX_ABRT_reg))
#define MIS_IC_CLR_TX_ABRT_inst_adr                                                  "0x00D5"
#define MIS_IC_CLR_TX_ABRT_inst                                                      0x00D5
#define MIS_IC_CLR_TX_ABRT_clr_tx_abrt_shift                                         (0)
#define MIS_IC_CLR_TX_ABRT_clr_tx_abrt_mask                                          (0x00000001)
#define MIS_IC_CLR_TX_ABRT_clr_tx_abrt(data)                                         (0x00000001&((data)<<0))
#define MIS_IC_CLR_TX_ABRT_clr_tx_abrt_src(data)                                     ((0x00000001&(data))>>0)
#define MIS_IC_CLR_TX_ABRT_get_clr_tx_abrt(data)                                     ((0x00000001&(data))>>0)


#define MIS_IC_CLR_RX_DONE                                                           0x1801B358
#define MIS_IC_CLR_RX_DONE_reg_addr                                                  "0xB801B358"
#define MIS_IC_CLR_RX_DONE_reg                                                       0xB801B358
#define set_MIS_IC_CLR_RX_DONE_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_RX_DONE_reg)=data)
#define get_MIS_IC_CLR_RX_DONE_reg   (*((volatile unsigned int*) MIS_IC_CLR_RX_DONE_reg))
#define MIS_IC_CLR_RX_DONE_inst_adr                                                  "0x00D6"
#define MIS_IC_CLR_RX_DONE_inst                                                      0x00D6
#define MIS_IC_CLR_RX_DONE_clr_rx_done_shift                                         (0)
#define MIS_IC_CLR_RX_DONE_clr_rx_done_mask                                          (0x00000001)
#define MIS_IC_CLR_RX_DONE_clr_rx_done(data)                                         (0x00000001&((data)<<0))
#define MIS_IC_CLR_RX_DONE_clr_rx_done_src(data)                                     ((0x00000001&(data))>>0)
#define MIS_IC_CLR_RX_DONE_get_clr_rx_done(data)                                     ((0x00000001&(data))>>0)


#define MIS_IC_CLR_ACTIVITY                                                          0x1801B35C
#define MIS_IC_CLR_ACTIVITY_reg_addr                                                 "0xB801B35C"
#define MIS_IC_CLR_ACTIVITY_reg                                                      0xB801B35C
#define set_MIS_IC_CLR_ACTIVITY_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_ACTIVITY_reg)=data)
#define get_MIS_IC_CLR_ACTIVITY_reg   (*((volatile unsigned int*) MIS_IC_CLR_ACTIVITY_reg))
#define MIS_IC_CLR_ACTIVITY_inst_adr                                                 "0x00D7"
#define MIS_IC_CLR_ACTIVITY_inst                                                     0x00D7
#define MIS_IC_CLR_ACTIVITY_clr_activity_shift                                       (0)
#define MIS_IC_CLR_ACTIVITY_clr_activity_mask                                        (0x00000001)
#define MIS_IC_CLR_ACTIVITY_clr_activity(data)                                       (0x00000001&((data)<<0))
#define MIS_IC_CLR_ACTIVITY_clr_activity_src(data)                                   ((0x00000001&(data))>>0)
#define MIS_IC_CLR_ACTIVITY_get_clr_activity(data)                                   ((0x00000001&(data))>>0)


#define MIS_IC_CLR_STOP_DET                                                          0x1801B360
#define MIS_IC_CLR_STOP_DET_reg_addr                                                 "0xB801B360"
#define MIS_IC_CLR_STOP_DET_reg                                                      0xB801B360
#define set_MIS_IC_CLR_STOP_DET_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_STOP_DET_reg)=data)
#define get_MIS_IC_CLR_STOP_DET_reg   (*((volatile unsigned int*) MIS_IC_CLR_STOP_DET_reg))
#define MIS_IC_CLR_STOP_DET_inst_adr                                                 "0x00D8"
#define MIS_IC_CLR_STOP_DET_inst                                                     0x00D8
#define MIS_IC_CLR_STOP_DET_clr_stop_det_shift                                       (0)
#define MIS_IC_CLR_STOP_DET_clr_stop_det_mask                                        (0x00000001)
#define MIS_IC_CLR_STOP_DET_clr_stop_det(data)                                       (0x00000001&((data)<<0))
#define MIS_IC_CLR_STOP_DET_clr_stop_det_src(data)                                   ((0x00000001&(data))>>0)
#define MIS_IC_CLR_STOP_DET_get_clr_stop_det(data)                                   ((0x00000001&(data))>>0)


#define MIS_IC_CLR_START_DET                                                         0x1801B364
#define MIS_IC_CLR_START_DET_reg_addr                                                "0xB801B364"
#define MIS_IC_CLR_START_DET_reg                                                     0xB801B364
#define set_MIS_IC_CLR_START_DET_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_START_DET_reg)=data)
#define get_MIS_IC_CLR_START_DET_reg   (*((volatile unsigned int*) MIS_IC_CLR_START_DET_reg))
#define MIS_IC_CLR_START_DET_inst_adr                                                "0x00D9"
#define MIS_IC_CLR_START_DET_inst                                                    0x00D9
#define MIS_IC_CLR_START_DET_clr_start_det_shift                                     (0)
#define MIS_IC_CLR_START_DET_clr_start_det_mask                                      (0x00000001)
#define MIS_IC_CLR_START_DET_clr_start_det(data)                                     (0x00000001&((data)<<0))
#define MIS_IC_CLR_START_DET_clr_start_det_src(data)                                 ((0x00000001&(data))>>0)
#define MIS_IC_CLR_START_DET_get_clr_start_det(data)                                 ((0x00000001&(data))>>0)


#define MIS_IC_CLR_GEN_CALL                                                          0x1801B368
#define MIS_IC_CLR_GEN_CALL_reg_addr                                                 "0xB801B368"
#define MIS_IC_CLR_GEN_CALL_reg                                                      0xB801B368
#define set_MIS_IC_CLR_GEN_CALL_reg(data)   (*((volatile unsigned int*) MIS_IC_CLR_GEN_CALL_reg)=data)
#define get_MIS_IC_CLR_GEN_CALL_reg   (*((volatile unsigned int*) MIS_IC_CLR_GEN_CALL_reg))
#define MIS_IC_CLR_GEN_CALL_inst_adr                                                 "0x00DA"
#define MIS_IC_CLR_GEN_CALL_inst                                                     0x00DA
#define MIS_IC_CLR_GEN_CALL_clr_gen_call_shift                                       (0)
#define MIS_IC_CLR_GEN_CALL_clr_gen_call_mask                                        (0x00000001)
#define MIS_IC_CLR_GEN_CALL_clr_gen_call(data)                                       (0x00000001&((data)<<0))
#define MIS_IC_CLR_GEN_CALL_clr_gen_call_src(data)                                   ((0x00000001&(data))>>0)
#define MIS_IC_CLR_GEN_CALL_get_clr_gen_call(data)                                   ((0x00000001&(data))>>0)


#define MIS_IC_ENABLE                                                                0x1801B36C
#define MIS_IC_ENABLE_reg_addr                                                       "0xB801B36C"
#define MIS_IC_ENABLE_reg                                                            0xB801B36C
#define set_MIS_IC_ENABLE_reg(data)   (*((volatile unsigned int*) MIS_IC_ENABLE_reg)=data)
#define get_MIS_IC_ENABLE_reg   (*((volatile unsigned int*) MIS_IC_ENABLE_reg))
#define MIS_IC_ENABLE_inst_adr                                                       "0x00DB"
#define MIS_IC_ENABLE_inst                                                           0x00DB
#define MIS_IC_ENABLE_enable_shift                                                   (0)
#define MIS_IC_ENABLE_enable_mask                                                    (0x00000001)
#define MIS_IC_ENABLE_enable(data)                                                   (0x00000001&((data)<<0))
#define MIS_IC_ENABLE_enable_src(data)                                               ((0x00000001&(data))>>0)
#define MIS_IC_ENABLE_get_enable(data)                                               ((0x00000001&(data))>>0)


#define MIS_IC_STATUS                                                                0x1801B370
#define MIS_IC_STATUS_reg_addr                                                       "0xB801B370"
#define MIS_IC_STATUS_reg                                                            0xB801B370
#define set_MIS_IC_STATUS_reg(data)   (*((volatile unsigned int*) MIS_IC_STATUS_reg)=data)
#define get_MIS_IC_STATUS_reg   (*((volatile unsigned int*) MIS_IC_STATUS_reg))
#define MIS_IC_STATUS_inst_adr                                                       "0x00DC"
#define MIS_IC_STATUS_inst                                                           0x00DC
#define MIS_IC_STATUS_slv_activity_shift                                             (6)
#define MIS_IC_STATUS_slv_activity_mask                                              (0x00000040)
#define MIS_IC_STATUS_slv_activity(data)                                             (0x00000040&((data)<<6))
#define MIS_IC_STATUS_slv_activity_src(data)                                         ((0x00000040&(data))>>6)
#define MIS_IC_STATUS_get_slv_activity(data)                                         ((0x00000040&(data))>>6)
#define MIS_IC_STATUS_mst_activity_shift                                             (5)
#define MIS_IC_STATUS_mst_activity_mask                                              (0x00000020)
#define MIS_IC_STATUS_mst_activity(data)                                             (0x00000020&((data)<<5))
#define MIS_IC_STATUS_mst_activity_src(data)                                         ((0x00000020&(data))>>5)
#define MIS_IC_STATUS_get_mst_activity(data)                                         ((0x00000020&(data))>>5)
#define MIS_IC_STATUS_rff_shift                                                      (4)
#define MIS_IC_STATUS_rff_mask                                                       (0x00000010)
#define MIS_IC_STATUS_rff(data)                                                      (0x00000010&((data)<<4))
#define MIS_IC_STATUS_rff_src(data)                                                  ((0x00000010&(data))>>4)
#define MIS_IC_STATUS_get_rff(data)                                                  ((0x00000010&(data))>>4)
#define MIS_IC_STATUS_rfne_shift                                                     (3)
#define MIS_IC_STATUS_rfne_mask                                                      (0x00000008)
#define MIS_IC_STATUS_rfne(data)                                                     (0x00000008&((data)<<3))
#define MIS_IC_STATUS_rfne_src(data)                                                 ((0x00000008&(data))>>3)
#define MIS_IC_STATUS_get_rfne(data)                                                 ((0x00000008&(data))>>3)
#define MIS_IC_STATUS_tfe_shift                                                      (2)
#define MIS_IC_STATUS_tfe_mask                                                       (0x00000004)
#define MIS_IC_STATUS_tfe(data)                                                      (0x00000004&((data)<<2))
#define MIS_IC_STATUS_tfe_src(data)                                                  ((0x00000004&(data))>>2)
#define MIS_IC_STATUS_get_tfe(data)                                                  ((0x00000004&(data))>>2)
#define MIS_IC_STATUS_tfnf_shift                                                     (1)
#define MIS_IC_STATUS_tfnf_mask                                                      (0x00000002)
#define MIS_IC_STATUS_tfnf(data)                                                     (0x00000002&((data)<<1))
#define MIS_IC_STATUS_tfnf_src(data)                                                 ((0x00000002&(data))>>1)
#define MIS_IC_STATUS_get_tfnf(data)                                                 ((0x00000002&(data))>>1)
#define MIS_IC_STATUS_activity_shift                                                 (0)
#define MIS_IC_STATUS_activity_mask                                                  (0x00000001)
#define MIS_IC_STATUS_activity(data)                                                 (0x00000001&((data)<<0))
#define MIS_IC_STATUS_activity_src(data)                                             ((0x00000001&(data))>>0)
#define MIS_IC_STATUS_get_activity(data)                                             ((0x00000001&(data))>>0)


#define MIS_IC_TXFLR                                                                 0x1801B374
#define MIS_IC_TXFLR_reg_addr                                                        "0xB801B374"
#define MIS_IC_TXFLR_reg                                                             0xB801B374
#define set_MIS_IC_TXFLR_reg(data)   (*((volatile unsigned int*) MIS_IC_TXFLR_reg)=data)
#define get_MIS_IC_TXFLR_reg   (*((volatile unsigned int*) MIS_IC_TXFLR_reg))
#define MIS_IC_TXFLR_inst_adr                                                        "0x00DD"
#define MIS_IC_TXFLR_inst                                                            0x00DD
#define MIS_IC_TXFLR_txflr_shift                                                     (0)
#define MIS_IC_TXFLR_txflr_mask                                                      (0x0000000F)
#define MIS_IC_TXFLR_txflr(data)                                                     (0x0000000F&((data)<<0))
#define MIS_IC_TXFLR_txflr_src(data)                                                 ((0x0000000F&(data))>>0)
#define MIS_IC_TXFLR_get_txflr(data)                                                 ((0x0000000F&(data))>>0)


#define MIS_IC_RXFLR                                                                 0x1801B378
#define MIS_IC_RXFLR_reg_addr                                                        "0xB801B378"
#define MIS_IC_RXFLR_reg                                                             0xB801B378
#define set_MIS_IC_RXFLR_reg(data)   (*((volatile unsigned int*) MIS_IC_RXFLR_reg)=data)
#define get_MIS_IC_RXFLR_reg   (*((volatile unsigned int*) MIS_IC_RXFLR_reg))
#define MIS_IC_RXFLR_inst_adr                                                        "0x00DE"
#define MIS_IC_RXFLR_inst                                                            0x00DE
#define MIS_IC_RXFLR_rxflr_shift                                                     (0)
#define MIS_IC_RXFLR_rxflr_mask                                                      (0x0000000F)
#define MIS_IC_RXFLR_rxflr(data)                                                     (0x0000000F&((data)<<0))
#define MIS_IC_RXFLR_rxflr_src(data)                                                 ((0x0000000F&(data))>>0)
#define MIS_IC_RXFLR_get_rxflr(data)                                                 ((0x0000000F&(data))>>0)


#define MIS_IC_TX_ABRT_SOURCE                                                        0x1801B380
#define MIS_IC_TX_ABRT_SOURCE_reg_addr                                               "0xB801B380"
#define MIS_IC_TX_ABRT_SOURCE_reg                                                    0xB801B380
#define set_MIS_IC_TX_ABRT_SOURCE_reg(data)   (*((volatile unsigned int*) MIS_IC_TX_ABRT_SOURCE_reg)=data)
#define get_MIS_IC_TX_ABRT_SOURCE_reg   (*((volatile unsigned int*) MIS_IC_TX_ABRT_SOURCE_reg))
#define MIS_IC_TX_ABRT_SOURCE_inst_adr                                               "0x00E0"
#define MIS_IC_TX_ABRT_SOURCE_inst                                                   0x00E0
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvrd_intx_shift                                  (15)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvrd_intx_mask                                   (0x00008000)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvrd_intx(data)                                  (0x00008000&((data)<<15))
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvrd_intx_src(data)                              ((0x00008000&(data))>>15)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_slvrd_intx(data)                              ((0x00008000&(data))>>15)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slv_arblost_shift                                 (14)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slv_arblost_mask                                  (0x00004000)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slv_arblost(data)                                 (0x00004000&((data)<<14))
#define MIS_IC_TX_ABRT_SOURCE_abrt_slv_arblost_src(data)                             ((0x00004000&(data))>>14)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_slv_arblost(data)                             ((0x00004000&(data))>>14)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvflush_txfifo_shift                             (13)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvflush_txfifo_mask                              (0x00002000)
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvflush_txfifo(data)                             (0x00002000&((data)<<13))
#define MIS_IC_TX_ABRT_SOURCE_abrt_slvflush_txfifo_src(data)                         ((0x00002000&(data))>>13)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_slvflush_txfifo(data)                         ((0x00002000&(data))>>13)
#define MIS_IC_TX_ABRT_SOURCE_arb_lost_shift                                         (12)
#define MIS_IC_TX_ABRT_SOURCE_arb_lost_mask                                          (0x00001000)
#define MIS_IC_TX_ABRT_SOURCE_arb_lost(data)                                         (0x00001000&((data)<<12))
#define MIS_IC_TX_ABRT_SOURCE_arb_lost_src(data)                                     ((0x00001000&(data))>>12)
#define MIS_IC_TX_ABRT_SOURCE_get_arb_lost(data)                                     ((0x00001000&(data))>>12)
#define MIS_IC_TX_ABRT_SOURCE_arb_master_dis_shift                                   (11)
#define MIS_IC_TX_ABRT_SOURCE_arb_master_dis_mask                                    (0x00000800)
#define MIS_IC_TX_ABRT_SOURCE_arb_master_dis(data)                                   (0x00000800&((data)<<11))
#define MIS_IC_TX_ABRT_SOURCE_arb_master_dis_src(data)                               ((0x00000800&(data))>>11)
#define MIS_IC_TX_ABRT_SOURCE_get_arb_master_dis(data)                               ((0x00000800&(data))>>11)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10b_rd_norstrt_shift                              (10)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10b_rd_norstrt_mask                               (0x00000400)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10b_rd_norstrt(data)                              (0x00000400&((data)<<10))
#define MIS_IC_TX_ABRT_SOURCE_abrt_10b_rd_norstrt_src(data)                          ((0x00000400&(data))>>10)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_10b_rd_norstrt(data)                          ((0x00000400&(data))>>10)
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_norstrt_shift                               (9)
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_norstrt_mask                                (0x00000200)
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_norstrt(data)                               (0x00000200&((data)<<9))
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_norstrt_src(data)                           ((0x00000200&(data))>>9)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_sbyte_norstrt(data)                           ((0x00000200&(data))>>9)
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_norstrt_shift                                  (8)
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_norstrt_mask                                   (0x00000100)
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_norstrt(data)                                  (0x00000100&((data)<<8))
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_norstrt_src(data)                              ((0x00000100&(data))>>8)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_hs_norstrt(data)                              ((0x00000100&(data))>>8)
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_ackdet_shift                                (7)
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_ackdet_mask                                 (0x00000080)
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_ackdet(data)                                (0x00000080&((data)<<7))
#define MIS_IC_TX_ABRT_SOURCE_abrt_sbyte_ackdet_src(data)                            ((0x00000080&(data))>>7)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_sbyte_ackdet(data)                            ((0x00000080&(data))>>7)
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_ackdet_shift                                   (6)
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_ackdet_mask                                    (0x00000040)
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_ackdet(data)                                   (0x00000040&((data)<<6))
#define MIS_IC_TX_ABRT_SOURCE_abrt_hs_ackdet_src(data)                               ((0x00000040&(data))>>6)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_hs_ackdet(data)                               ((0x00000040&(data))>>6)
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_read_shift                                  (5)
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_read_mask                                   (0x00000020)
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_read(data)                                  (0x00000020&((data)<<5))
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_read_src(data)                              ((0x00000020&(data))>>5)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_gcall_read(data)                              ((0x00000020&(data))>>5)
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_noack_shift                                 (4)
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_noack_mask                                  (0x00000010)
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_noack(data)                                 (0x00000010&((data)<<4))
#define MIS_IC_TX_ABRT_SOURCE_abrt_gcall_noack_src(data)                             ((0x00000010&(data))>>4)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_gcall_noack(data)                             ((0x00000010&(data))>>4)
#define MIS_IC_TX_ABRT_SOURCE_abrt_txdata_noack_shift                                (3)
#define MIS_IC_TX_ABRT_SOURCE_abrt_txdata_noack_mask                                 (0x00000008)
#define MIS_IC_TX_ABRT_SOURCE_abrt_txdata_noack(data)                                (0x00000008&((data)<<3))
#define MIS_IC_TX_ABRT_SOURCE_abrt_txdata_noack_src(data)                            ((0x00000008&(data))>>3)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_txdata_noack(data)                            ((0x00000008&(data))>>3)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr2_noack_shift                               (2)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr2_noack_mask                                (0x00000004)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr2_noack(data)                               (0x00000004&((data)<<2))
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr2_noack_src(data)                           ((0x00000004&(data))>>2)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_10addr2_noack(data)                           ((0x00000004&(data))>>2)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr1_noack_shift                               (1)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr1_noack_mask                                (0x00000002)
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr1_noack(data)                               (0x00000002&((data)<<1))
#define MIS_IC_TX_ABRT_SOURCE_abrt_10addr1_noack_src(data)                           ((0x00000002&(data))>>1)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_10addr1_noack(data)                           ((0x00000002&(data))>>1)
#define MIS_IC_TX_ABRT_SOURCE_abrt_7b_addr_noack_shift                               (0)
#define MIS_IC_TX_ABRT_SOURCE_abrt_7b_addr_noack_mask                                (0x00000001)
#define MIS_IC_TX_ABRT_SOURCE_abrt_7b_addr_noack(data)                               (0x00000001&((data)<<0))
#define MIS_IC_TX_ABRT_SOURCE_abrt_7b_addr_noack_src(data)                           ((0x00000001&(data))>>0)
#define MIS_IC_TX_ABRT_SOURCE_get_abrt_7b_addr_noack(data)                           ((0x00000001&(data))>>0)


#define MIS_IC_SLV_DATA_NACK_ONLY                                                    0x1801B384
#define MIS_IC_SLV_DATA_NACK_ONLY_reg_addr                                           "0xB801B384"
#define MIS_IC_SLV_DATA_NACK_ONLY_reg                                                0xB801B384
#define set_MIS_IC_SLV_DATA_NACK_ONLY_reg(data)   (*((volatile unsigned int*) MIS_IC_SLV_DATA_NACK_ONLY_reg)=data)
#define get_MIS_IC_SLV_DATA_NACK_ONLY_reg   (*((volatile unsigned int*) MIS_IC_SLV_DATA_NACK_ONLY_reg))
#define MIS_IC_SLV_DATA_NACK_ONLY_inst_adr                                           "0x00E1"
#define MIS_IC_SLV_DATA_NACK_ONLY_inst                                               0x00E1
#define MIS_IC_SLV_DATA_NACK_ONLY_nack_shift                                         (0)
#define MIS_IC_SLV_DATA_NACK_ONLY_nack_mask                                          (0x00000001)
#define MIS_IC_SLV_DATA_NACK_ONLY_nack(data)                                         (0x00000001&((data)<<0))
#define MIS_IC_SLV_DATA_NACK_ONLY_nack_src(data)                                     ((0x00000001&(data))>>0)
#define MIS_IC_SLV_DATA_NACK_ONLY_get_nack(data)                                     ((0x00000001&(data))>>0)


#define MIS_IC_DMA_CR                                                                0x1801B388
#define MIS_IC_DMA_CR_reg_addr                                                       "0xB801B388"
#define MIS_IC_DMA_CR_reg                                                            0xB801B388
#define set_MIS_IC_DMA_CR_reg(data)   (*((volatile unsigned int*) MIS_IC_DMA_CR_reg)=data)
#define get_MIS_IC_DMA_CR_reg   (*((volatile unsigned int*) MIS_IC_DMA_CR_reg))
#define MIS_IC_DMA_CR_inst_adr                                                       "0x00E2"
#define MIS_IC_DMA_CR_inst                                                           0x00E2
#define MIS_IC_DMA_CR_tdmae_shift                                                    (1)
#define MIS_IC_DMA_CR_tdmae_mask                                                     (0x00000002)
#define MIS_IC_DMA_CR_tdmae(data)                                                    (0x00000002&((data)<<1))
#define MIS_IC_DMA_CR_tdmae_src(data)                                                ((0x00000002&(data))>>1)
#define MIS_IC_DMA_CR_get_tdmae(data)                                                ((0x00000002&(data))>>1)
#define MIS_IC_DMA_CR_rdmae_shift                                                    (0)
#define MIS_IC_DMA_CR_rdmae_mask                                                     (0x00000001)
#define MIS_IC_DMA_CR_rdmae(data)                                                    (0x00000001&((data)<<0))
#define MIS_IC_DMA_CR_rdmae_src(data)                                                ((0x00000001&(data))>>0)
#define MIS_IC_DMA_CR_get_rdmae(data)                                                ((0x00000001&(data))>>0)


#define MIS_IC_DMA_TDLR                                                              0x1801B38C
#define MIS_IC_DMA_TDLR_reg_addr                                                     "0xB801B38C"
#define MIS_IC_DMA_TDLR_reg                                                          0xB801B38C
#define set_MIS_IC_DMA_TDLR_reg(data)   (*((volatile unsigned int*) MIS_IC_DMA_TDLR_reg)=data)
#define get_MIS_IC_DMA_TDLR_reg   (*((volatile unsigned int*) MIS_IC_DMA_TDLR_reg))
#define MIS_IC_DMA_TDLR_inst_adr                                                     "0x00E3"
#define MIS_IC_DMA_TDLR_inst                                                         0x00E3
#define MIS_IC_DMA_TDLR_dmatdl_shift                                                 (0)
#define MIS_IC_DMA_TDLR_dmatdl_mask                                                  (0x00000007)
#define MIS_IC_DMA_TDLR_dmatdl(data)                                                 (0x00000007&((data)<<0))
#define MIS_IC_DMA_TDLR_dmatdl_src(data)                                             ((0x00000007&(data))>>0)
#define MIS_IC_DMA_TDLR_get_dmatdl(data)                                             ((0x00000007&(data))>>0)


#define MIS_IC_DMA_RDLR                                                              0x1801B390
#define MIS_IC_DMA_RDLR_reg_addr                                                     "0xB801B390"
#define MIS_IC_DMA_RDLR_reg                                                          0xB801B390
#define set_MIS_IC_DMA_RDLR_reg(data)   (*((volatile unsigned int*) MIS_IC_DMA_RDLR_reg)=data)
#define get_MIS_IC_DMA_RDLR_reg   (*((volatile unsigned int*) MIS_IC_DMA_RDLR_reg))
#define MIS_IC_DMA_RDLR_inst_adr                                                     "0x00E4"
#define MIS_IC_DMA_RDLR_inst                                                         0x00E4
#define MIS_IC_DMA_RDLR_dmardl_shift                                                 (0)
#define MIS_IC_DMA_RDLR_dmardl_mask                                                  (0x0000000F)
#define MIS_IC_DMA_RDLR_dmardl(data)                                                 (0x0000000F&((data)<<0))
#define MIS_IC_DMA_RDLR_dmardl_src(data)                                             ((0x0000000F&(data))>>0)
#define MIS_IC_DMA_RDLR_get_dmardl(data)                                             ((0x0000000F&(data))>>0)


#define MIS_IC_SDA_SETUP                                                             0x1801B394
#define MIS_IC_SDA_SETUP_reg_addr                                                    "0xB801B394"
#define MIS_IC_SDA_SETUP_reg                                                         0xB801B394
#define set_MIS_IC_SDA_SETUP_reg(data)   (*((volatile unsigned int*) MIS_IC_SDA_SETUP_reg)=data)
#define get_MIS_IC_SDA_SETUP_reg   (*((volatile unsigned int*) MIS_IC_SDA_SETUP_reg))
#define MIS_IC_SDA_SETUP_inst_adr                                                    "0x00E5"
#define MIS_IC_SDA_SETUP_inst                                                        0x00E5
#define MIS_IC_SDA_SETUP_sda_setup_shift                                             (0)
#define MIS_IC_SDA_SETUP_sda_setup_mask                                              (0x000000FF)
#define MIS_IC_SDA_SETUP_sda_setup(data)                                             (0x000000FF&((data)<<0))
#define MIS_IC_SDA_SETUP_sda_setup_src(data)                                         ((0x000000FF&(data))>>0)
#define MIS_IC_SDA_SETUP_get_sda_setup(data)                                         ((0x000000FF&(data))>>0)


#define MIS_IC_ACK_GENERAL_CALL                                                      0x1801B398
#define MIS_IC_ACK_GENERAL_CALL_reg_addr                                             "0xB801B398"
#define MIS_IC_ACK_GENERAL_CALL_reg                                                  0xB801B398
#define set_MIS_IC_ACK_GENERAL_CALL_reg(data)   (*((volatile unsigned int*) MIS_IC_ACK_GENERAL_CALL_reg)=data)
#define get_MIS_IC_ACK_GENERAL_CALL_reg   (*((volatile unsigned int*) MIS_IC_ACK_GENERAL_CALL_reg))
#define MIS_IC_ACK_GENERAL_CALL_inst_adr                                             "0x00E6"
#define MIS_IC_ACK_GENERAL_CALL_inst                                                 0x00E6
#define MIS_IC_ACK_GENERAL_CALL_ack_gen_call_shift                                   (0)
#define MIS_IC_ACK_GENERAL_CALL_ack_gen_call_mask                                    (0x00000001)
#define MIS_IC_ACK_GENERAL_CALL_ack_gen_call(data)                                   (0x00000001&((data)<<0))
#define MIS_IC_ACK_GENERAL_CALL_ack_gen_call_src(data)                               ((0x00000001&(data))>>0)
#define MIS_IC_ACK_GENERAL_CALL_get_ack_gen_call(data)                               ((0x00000001&(data))>>0)


#define MIS_IC_ENABLE_STATUS                                                         0x1801B39C
#define MIS_IC_ENABLE_STATUS_reg_addr                                                "0xB801B39C"
#define MIS_IC_ENABLE_STATUS_reg                                                     0xB801B39C
#define set_MIS_IC_ENABLE_STATUS_reg(data)   (*((volatile unsigned int*) MIS_IC_ENABLE_STATUS_reg)=data)
#define get_MIS_IC_ENABLE_STATUS_reg   (*((volatile unsigned int*) MIS_IC_ENABLE_STATUS_reg))
#define MIS_IC_ENABLE_STATUS_inst_adr                                                "0x00E7"
#define MIS_IC_ENABLE_STATUS_inst                                                    0x00E7
#define MIS_IC_ENABLE_STATUS_slv_rx_data_lost_shift                                  (2)
#define MIS_IC_ENABLE_STATUS_slv_rx_data_lost_mask                                   (0x00000004)
#define MIS_IC_ENABLE_STATUS_slv_rx_data_lost(data)                                  (0x00000004&((data)<<2))
#define MIS_IC_ENABLE_STATUS_slv_rx_data_lost_src(data)                              ((0x00000004&(data))>>2)
#define MIS_IC_ENABLE_STATUS_get_slv_rx_data_lost(data)                              ((0x00000004&(data))>>2)
#define MIS_IC_ENABLE_STATUS_slv_disabled_whi_shift                                  (1)
#define MIS_IC_ENABLE_STATUS_slv_disabled_whi_mask                                   (0x00000002)
#define MIS_IC_ENABLE_STATUS_slv_disabled_whi(data)                                  (0x00000002&((data)<<1))
#define MIS_IC_ENABLE_STATUS_slv_disabled_whi_src(data)                              ((0x00000002&(data))>>1)
#define MIS_IC_ENABLE_STATUS_get_slv_disabled_whi(data)                              ((0x00000002&(data))>>1)
#define MIS_IC_ENABLE_STATUS_ic_en_shift                                             (0)
#define MIS_IC_ENABLE_STATUS_ic_en_mask                                              (0x00000001)
#define MIS_IC_ENABLE_STATUS_ic_en(data)                                             (0x00000001&((data)<<0))
#define MIS_IC_ENABLE_STATUS_ic_en_src(data)                                         ((0x00000001&(data))>>0)
#define MIS_IC_ENABLE_STATUS_get_ic_en(data)                                         ((0x00000001&(data))>>0)


#define MIS_IC_COMP_PARAM_1                                                          0x1801B3F4
#define MIS_IC_COMP_PARAM_1_reg_addr                                                 "0xB801B3F4"
#define MIS_IC_COMP_PARAM_1_reg                                                      0xB801B3F4
#define set_MIS_IC_COMP_PARAM_1_reg(data)   (*((volatile unsigned int*) MIS_IC_COMP_PARAM_1_reg)=data)
#define get_MIS_IC_COMP_PARAM_1_reg   (*((volatile unsigned int*) MIS_IC_COMP_PARAM_1_reg))
#define MIS_IC_COMP_PARAM_1_inst_adr                                                 "0x00FD"
#define MIS_IC_COMP_PARAM_1_inst                                                     0x00FD
#define MIS_IC_COMP_PARAM_1_tx_buffer_depth_shift                                    (16)
#define MIS_IC_COMP_PARAM_1_tx_buffer_depth_mask                                     (0x00FF0000)
#define MIS_IC_COMP_PARAM_1_tx_buffer_depth(data)                                    (0x00FF0000&((data)<<16))
#define MIS_IC_COMP_PARAM_1_tx_buffer_depth_src(data)                                ((0x00FF0000&(data))>>16)
#define MIS_IC_COMP_PARAM_1_get_tx_buffer_depth(data)                                ((0x00FF0000&(data))>>16)
#define MIS_IC_COMP_PARAM_1_rx_buffer_depth_shift                                    (8)
#define MIS_IC_COMP_PARAM_1_rx_buffer_depth_mask                                     (0x0000FF00)
#define MIS_IC_COMP_PARAM_1_rx_buffer_depth(data)                                    (0x0000FF00&((data)<<8))
#define MIS_IC_COMP_PARAM_1_rx_buffer_depth_src(data)                                ((0x0000FF00&(data))>>8)
#define MIS_IC_COMP_PARAM_1_get_rx_buffer_depth(data)                                ((0x0000FF00&(data))>>8)
#define MIS_IC_COMP_PARAM_1_add_encoded_params_shift                                 (7)
#define MIS_IC_COMP_PARAM_1_add_encoded_params_mask                                  (0x00000080)
#define MIS_IC_COMP_PARAM_1_add_encoded_params(data)                                 (0x00000080&((data)<<7))
#define MIS_IC_COMP_PARAM_1_add_encoded_params_src(data)                             ((0x00000080&(data))>>7)
#define MIS_IC_COMP_PARAM_1_get_add_encoded_params(data)                             ((0x00000080&(data))>>7)
#define MIS_IC_COMP_PARAM_1_has_dma_shift                                            (6)
#define MIS_IC_COMP_PARAM_1_has_dma_mask                                             (0x00000040)
#define MIS_IC_COMP_PARAM_1_has_dma(data)                                            (0x00000040&((data)<<6))
#define MIS_IC_COMP_PARAM_1_has_dma_src(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC_COMP_PARAM_1_get_has_dma(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC_COMP_PARAM_1_intr_io_shift                                            (5)
#define MIS_IC_COMP_PARAM_1_intr_io_mask                                             (0x00000020)
#define MIS_IC_COMP_PARAM_1_intr_io(data)                                            (0x00000020&((data)<<5))
#define MIS_IC_COMP_PARAM_1_intr_io_src(data)                                        ((0x00000020&(data))>>5)
#define MIS_IC_COMP_PARAM_1_get_intr_io(data)                                        ((0x00000020&(data))>>5)
#define MIS_IC_COMP_PARAM_1_hc_count_values_shift                                    (4)
#define MIS_IC_COMP_PARAM_1_hc_count_values_mask                                     (0x00000010)
#define MIS_IC_COMP_PARAM_1_hc_count_values(data)                                    (0x00000010&((data)<<4))
#define MIS_IC_COMP_PARAM_1_hc_count_values_src(data)                                ((0x00000010&(data))>>4)
#define MIS_IC_COMP_PARAM_1_get_hc_count_values(data)                                ((0x00000010&(data))>>4)
#define MIS_IC_COMP_PARAM_1_max_speed_mode_shift                                     (2)
#define MIS_IC_COMP_PARAM_1_max_speed_mode_mask                                      (0x0000000C)
#define MIS_IC_COMP_PARAM_1_max_speed_mode(data)                                     (0x0000000C&((data)<<2))
#define MIS_IC_COMP_PARAM_1_max_speed_mode_src(data)                                 ((0x0000000C&(data))>>2)
#define MIS_IC_COMP_PARAM_1_get_max_speed_mode(data)                                 ((0x0000000C&(data))>>2)
#define MIS_IC_COMP_PARAM_1_apb_data_width_shift                                     (0)
#define MIS_IC_COMP_PARAM_1_apb_data_width_mask                                      (0x00000003)
#define MIS_IC_COMP_PARAM_1_apb_data_width(data)                                     (0x00000003&((data)<<0))
#define MIS_IC_COMP_PARAM_1_apb_data_width_src(data)                                 ((0x00000003&(data))>>0)
#define MIS_IC_COMP_PARAM_1_get_apb_data_width(data)                                 ((0x00000003&(data))>>0)


#define MIS_IC_COMP_VERSION                                                          0x1801B3F8
#define MIS_IC_COMP_VERSION_reg_addr                                                 "0xB801B3F8"
#define MIS_IC_COMP_VERSION_reg                                                      0xB801B3F8
#define set_MIS_IC_COMP_VERSION_reg(data)   (*((volatile unsigned int*) MIS_IC_COMP_VERSION_reg)=data)
#define get_MIS_IC_COMP_VERSION_reg   (*((volatile unsigned int*) MIS_IC_COMP_VERSION_reg))
#define MIS_IC_COMP_VERSION_inst_adr                                                 "0x00FE"
#define MIS_IC_COMP_VERSION_inst                                                     0x00FE
#define MIS_IC_COMP_VERSION_ic_comp_version_shift                                    (0)
#define MIS_IC_COMP_VERSION_ic_comp_version_mask                                     (0xFFFFFFFF)
#define MIS_IC_COMP_VERSION_ic_comp_version(data)                                    (0xFFFFFFFF&((data)<<0))
#define MIS_IC_COMP_VERSION_ic_comp_version_src(data)                                ((0xFFFFFFFF&(data))>>0)
#define MIS_IC_COMP_VERSION_get_ic_comp_version(data)                                ((0xFFFFFFFF&(data))>>0)


#define MIS_IC_COMP_TYPE                                                             0x1801B3FC
#define MIS_IC_COMP_TYPE_reg_addr                                                    "0xB801B3FC"
#define MIS_IC_COMP_TYPE_reg                                                         0xB801B3FC
#define set_MIS_IC_COMP_TYPE_reg(data)   (*((volatile unsigned int*) MIS_IC_COMP_TYPE_reg)=data)
#define get_MIS_IC_COMP_TYPE_reg   (*((volatile unsigned int*) MIS_IC_COMP_TYPE_reg))
#define MIS_IC_COMP_TYPE_inst_adr                                                    "0x00FF"
#define MIS_IC_COMP_TYPE_inst                                                        0x00FF
#define MIS_IC_COMP_TYPE_ic_comp_type_shift                                          (0)
#define MIS_IC_COMP_TYPE_ic_comp_type_mask                                           (0xFFFFFFFF)
#define MIS_IC_COMP_TYPE_ic_comp_type(data)                                          (0xFFFFFFFF&((data)<<0))
#define MIS_IC_COMP_TYPE_ic_comp_type_src(data)                                      ((0xFFFFFFFF&(data))>>0)
#define MIS_IC_COMP_TYPE_get_ic_comp_type(data)                                      ((0xFFFFFFFF&(data))>>0)


#define MIS_IC1_CON                                                                  0x1801BB00
#define MIS_IC1_CON_reg_addr                                                         "0xB801BB00"
#define MIS_IC1_CON_reg                                                              0xB801BB00
#define set_MIS_IC1_CON_reg(data)   (*((volatile unsigned int*) MIS_IC1_CON_reg)=data)
#define get_MIS_IC1_CON_reg   (*((volatile unsigned int*) MIS_IC1_CON_reg))
#define MIS_IC1_CON_inst_adr                                                         "0x00C0"
#define MIS_IC1_CON_inst                                                             0x00C0
#define MIS_IC1_CON_ic_slave_disable_shift                                           (6)
#define MIS_IC1_CON_ic_slave_disable_mask                                            (0x00000040)
#define MIS_IC1_CON_ic_slave_disable(data)                                           (0x00000040&((data)<<6))
#define MIS_IC1_CON_ic_slave_disable_src(data)                                       ((0x00000040&(data))>>6)
#define MIS_IC1_CON_get_ic_slave_disable(data)                                       ((0x00000040&(data))>>6)
#define MIS_IC1_CON_ic_restart_en_shift                                              (5)
#define MIS_IC1_CON_ic_restart_en_mask                                               (0x00000020)
#define MIS_IC1_CON_ic_restart_en(data)                                              (0x00000020&((data)<<5))
#define MIS_IC1_CON_ic_restart_en_src(data)                                          ((0x00000020&(data))>>5)
#define MIS_IC1_CON_get_ic_restart_en(data)                                          ((0x00000020&(data))>>5)
#define MIS_IC1_CON_ic_10bitaddr_master_shift                                        (4)
#define MIS_IC1_CON_ic_10bitaddr_master_mask                                         (0x00000010)
#define MIS_IC1_CON_ic_10bitaddr_master(data)                                        (0x00000010&((data)<<4))
#define MIS_IC1_CON_ic_10bitaddr_master_src(data)                                    ((0x00000010&(data))>>4)
#define MIS_IC1_CON_get_ic_10bitaddr_master(data)                                    ((0x00000010&(data))>>4)
#define MIS_IC1_CON_ic_10bitaddr_slave_shift                                         (3)
#define MIS_IC1_CON_ic_10bitaddr_slave_mask                                          (0x00000008)
#define MIS_IC1_CON_ic_10bitaddr_slave(data)                                         (0x00000008&((data)<<3))
#define MIS_IC1_CON_ic_10bitaddr_slave_src(data)                                     ((0x00000008&(data))>>3)
#define MIS_IC1_CON_get_ic_10bitaddr_slave(data)                                     ((0x00000008&(data))>>3)
#define MIS_IC1_CON_speed_shift                                                      (1)
#define MIS_IC1_CON_speed_mask                                                       (0x00000006)
#define MIS_IC1_CON_speed(data)                                                      (0x00000006&((data)<<1))
#define MIS_IC1_CON_speed_src(data)                                                  ((0x00000006&(data))>>1)
#define MIS_IC1_CON_get_speed(data)                                                  ((0x00000006&(data))>>1)
#define MIS_IC1_CON_master_mode_shift                                                (0)
#define MIS_IC1_CON_master_mode_mask                                                 (0x00000001)
#define MIS_IC1_CON_master_mode(data)                                                (0x00000001&((data)<<0))
#define MIS_IC1_CON_master_mode_src(data)                                            ((0x00000001&(data))>>0)
#define MIS_IC1_CON_get_master_mode(data)                                            ((0x00000001&(data))>>0)


#define MIS_IC1_TAR                                                                  0x1801BB04
#define MIS_IC1_TAR_reg_addr                                                         "0xB801BB04"
#define MIS_IC1_TAR_reg                                                              0xB801BB04
#define set_MIS_IC1_TAR_reg(data)   (*((volatile unsigned int*) MIS_IC1_TAR_reg)=data)
#define get_MIS_IC1_TAR_reg   (*((volatile unsigned int*) MIS_IC1_TAR_reg))
#define MIS_IC1_TAR_inst_adr                                                         "0x00C1"
#define MIS_IC1_TAR_inst                                                             0x00C1
#define MIS_IC1_TAR_special_shift                                                    (11)
#define MIS_IC1_TAR_special_mask                                                     (0x00000800)
#define MIS_IC1_TAR_special(data)                                                    (0x00000800&((data)<<11))
#define MIS_IC1_TAR_special_src(data)                                                ((0x00000800&(data))>>11)
#define MIS_IC1_TAR_get_special(data)                                                ((0x00000800&(data))>>11)
#define MIS_IC1_TAR_gc_or_start_shift                                                (10)
#define MIS_IC1_TAR_gc_or_start_mask                                                 (0x00000400)
#define MIS_IC1_TAR_gc_or_start(data)                                                (0x00000400&((data)<<10))
#define MIS_IC1_TAR_gc_or_start_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_IC1_TAR_get_gc_or_start(data)                                            ((0x00000400&(data))>>10)
#define MIS_IC1_TAR_ic_tar_shift                                                     (0)
#define MIS_IC1_TAR_ic_tar_mask                                                      (0x000003FF)
#define MIS_IC1_TAR_ic_tar(data)                                                     (0x000003FF&((data)<<0))
#define MIS_IC1_TAR_ic_tar_src(data)                                                 ((0x000003FF&(data))>>0)
#define MIS_IC1_TAR_get_ic_tar(data)                                                 ((0x000003FF&(data))>>0)


#define MIS_IC1_SAR                                                                  0x1801BB08
#define MIS_IC1_SAR_reg_addr                                                         "0xB801BB08"
#define MIS_IC1_SAR_reg                                                              0xB801BB08
#define set_MIS_IC1_SAR_reg(data)   (*((volatile unsigned int*) MIS_IC1_SAR_reg)=data)
#define get_MIS_IC1_SAR_reg   (*((volatile unsigned int*) MIS_IC1_SAR_reg))
#define MIS_IC1_SAR_inst_adr                                                         "0x00C2"
#define MIS_IC1_SAR_inst                                                             0x00C2
#define MIS_IC1_SAR_ic_sar_shift                                                     (0)
#define MIS_IC1_SAR_ic_sar_mask                                                      (0x000003FF)
#define MIS_IC1_SAR_ic_sar(data)                                                     (0x000003FF&((data)<<0))
#define MIS_IC1_SAR_ic_sar_src(data)                                                 ((0x000003FF&(data))>>0)
#define MIS_IC1_SAR_get_ic_sar(data)                                                 ((0x000003FF&(data))>>0)


#define MIS_IC1_HS_MADDR                                                             0x1801BB0C
#define MIS_IC1_HS_MADDR_reg_addr                                                    "0xB801BB0C"
#define MIS_IC1_HS_MADDR_reg                                                         0xB801BB0C
#define set_MIS_IC1_HS_MADDR_reg(data)   (*((volatile unsigned int*) MIS_IC1_HS_MADDR_reg)=data)
#define get_MIS_IC1_HS_MADDR_reg   (*((volatile unsigned int*) MIS_IC1_HS_MADDR_reg))
#define MIS_IC1_HS_MADDR_inst_adr                                                    "0x00C3"
#define MIS_IC1_HS_MADDR_inst                                                        0x00C3
#define MIS_IC1_HS_MADDR_ic_hs_mar_shift                                             (0)
#define MIS_IC1_HS_MADDR_ic_hs_mar_mask                                              (0x00000007)
#define MIS_IC1_HS_MADDR_ic_hs_mar(data)                                             (0x00000007&((data)<<0))
#define MIS_IC1_HS_MADDR_ic_hs_mar_src(data)                                         ((0x00000007&(data))>>0)
#define MIS_IC1_HS_MADDR_get_ic_hs_mar(data)                                         ((0x00000007&(data))>>0)


#define MIS_IC1_DATA_CMD                                                             0x1801BB10
#define MIS_IC1_DATA_CMD_reg_addr                                                    "0xB801BB10"
#define MIS_IC1_DATA_CMD_reg                                                         0xB801BB10
#define set_MIS_IC1_DATA_CMD_reg(data)   (*((volatile unsigned int*) MIS_IC1_DATA_CMD_reg)=data)
#define get_MIS_IC1_DATA_CMD_reg   (*((volatile unsigned int*) MIS_IC1_DATA_CMD_reg))
#define MIS_IC1_DATA_CMD_inst_adr                                                    "0x00C4"
#define MIS_IC1_DATA_CMD_inst                                                        0x00C4
#define MIS_IC1_DATA_CMD_cmd_shift                                                   (8)
#define MIS_IC1_DATA_CMD_cmd_mask                                                    (0x00000100)
#define MIS_IC1_DATA_CMD_cmd(data)                                                   (0x00000100&((data)<<8))
#define MIS_IC1_DATA_CMD_cmd_src(data)                                               ((0x00000100&(data))>>8)
#define MIS_IC1_DATA_CMD_get_cmd(data)                                               ((0x00000100&(data))>>8)
#define MIS_IC1_DATA_CMD_dat_shift                                                   (0)
#define MIS_IC1_DATA_CMD_dat_mask                                                    (0x000000FF)
#define MIS_IC1_DATA_CMD_dat(data)                                                   (0x000000FF&((data)<<0))
#define MIS_IC1_DATA_CMD_dat_src(data)                                               ((0x000000FF&(data))>>0)
#define MIS_IC1_DATA_CMD_get_dat(data)                                               ((0x000000FF&(data))>>0)


#define MIS_IC1_SS_SCL_HCNT                                                          0x1801BB14
#define MIS_IC1_SS_SCL_HCNT_reg_addr                                                 "0xB801BB14"
#define MIS_IC1_SS_SCL_HCNT_reg                                                      0xB801BB14
#define set_MIS_IC1_SS_SCL_HCNT_reg(data)   (*((volatile unsigned int*) MIS_IC1_SS_SCL_HCNT_reg)=data)
#define get_MIS_IC1_SS_SCL_HCNT_reg   (*((volatile unsigned int*) MIS_IC1_SS_SCL_HCNT_reg))
#define MIS_IC1_SS_SCL_HCNT_inst_adr                                                 "0x00C5"
#define MIS_IC1_SS_SCL_HCNT_inst                                                     0x00C5
#define MIS_IC1_SS_SCL_HCNT_ic_ss_scl_hcnt_shift                                     (0)
#define MIS_IC1_SS_SCL_HCNT_ic_ss_scl_hcnt_mask                                      (0x0000FFFF)
#define MIS_IC1_SS_SCL_HCNT_ic_ss_scl_hcnt(data)                                     (0x0000FFFF&((data)<<0))
#define MIS_IC1_SS_SCL_HCNT_ic_ss_scl_hcnt_src(data)                                 ((0x0000FFFF&(data))>>0)
#define MIS_IC1_SS_SCL_HCNT_get_ic_ss_scl_hcnt(data)                                 ((0x0000FFFF&(data))>>0)


#define MIS_IC1_SS_SCL_LCNT                                                          0x1801BB18
#define MIS_IC1_SS_SCL_LCNT_reg_addr                                                 "0xB801BB18"
#define MIS_IC1_SS_SCL_LCNT_reg                                                      0xB801BB18
#define set_MIS_IC1_SS_SCL_LCNT_reg(data)   (*((volatile unsigned int*) MIS_IC1_SS_SCL_LCNT_reg)=data)
#define get_MIS_IC1_SS_SCL_LCNT_reg   (*((volatile unsigned int*) MIS_IC1_SS_SCL_LCNT_reg))
#define MIS_IC1_SS_SCL_LCNT_inst_adr                                                 "0x00C6"
#define MIS_IC1_SS_SCL_LCNT_inst                                                     0x00C6
#define MIS_IC1_SS_SCL_LCNT_ic_ss_scl_lcnt_shift                                     (0)
#define MIS_IC1_SS_SCL_LCNT_ic_ss_scl_lcnt_mask                                      (0x0000FFFF)
#define MIS_IC1_SS_SCL_LCNT_ic_ss_scl_lcnt(data)                                     (0x0000FFFF&((data)<<0))
#define MIS_IC1_SS_SCL_LCNT_ic_ss_scl_lcnt_src(data)                                 ((0x0000FFFF&(data))>>0)
#define MIS_IC1_SS_SCL_LCNT_get_ic_ss_scl_lcnt(data)                                 ((0x0000FFFF&(data))>>0)


#define MIS_IC1_FS_SCL_HCNT                                                          0x1801BB1C
#define MIS_IC1_FS_SCL_HCNT_reg_addr                                                 "0xB801BB1C"
#define MIS_IC1_FS_SCL_HCNT_reg                                                      0xB801BB1C
#define set_MIS_IC1_FS_SCL_HCNT_reg(data)   (*((volatile unsigned int*) MIS_IC1_FS_SCL_HCNT_reg)=data)
#define get_MIS_IC1_FS_SCL_HCNT_reg   (*((volatile unsigned int*) MIS_IC1_FS_SCL_HCNT_reg))
#define MIS_IC1_FS_SCL_HCNT_inst_adr                                                 "0x00C7"
#define MIS_IC1_FS_SCL_HCNT_inst                                                     0x00C7
#define MIS_IC1_FS_SCL_HCNT_ic_fs_scl_hcnt_shift                                     (0)
#define MIS_IC1_FS_SCL_HCNT_ic_fs_scl_hcnt_mask                                      (0x0000FFFF)
#define MIS_IC1_FS_SCL_HCNT_ic_fs_scl_hcnt(data)                                     (0x0000FFFF&((data)<<0))
#define MIS_IC1_FS_SCL_HCNT_ic_fs_scl_hcnt_src(data)                                 ((0x0000FFFF&(data))>>0)
#define MIS_IC1_FS_SCL_HCNT_get_ic_fs_scl_hcnt(data)                                 ((0x0000FFFF&(data))>>0)


#define MIS_IC1_FS_SCL_LCNT                                                          0x1801BB20
#define MIS_IC1_FS_SCL_LCNT_reg_addr                                                 "0xB801BB20"
#define MIS_IC1_FS_SCL_LCNT_reg                                                      0xB801BB20
#define set_MIS_IC1_FS_SCL_LCNT_reg(data)   (*((volatile unsigned int*) MIS_IC1_FS_SCL_LCNT_reg)=data)
#define get_MIS_IC1_FS_SCL_LCNT_reg   (*((volatile unsigned int*) MIS_IC1_FS_SCL_LCNT_reg))
#define MIS_IC1_FS_SCL_LCNT_inst_adr                                                 "0x00C8"
#define MIS_IC1_FS_SCL_LCNT_inst                                                     0x00C8
#define MIS_IC1_FS_SCL_LCNT_ic_fs_scl_lcnt_shift                                     (0)
#define MIS_IC1_FS_SCL_LCNT_ic_fs_scl_lcnt_mask                                      (0x0000FFFF)
#define MIS_IC1_FS_SCL_LCNT_ic_fs_scl_lcnt(data)                                     (0x0000FFFF&((data)<<0))
#define MIS_IC1_FS_SCL_LCNT_ic_fs_scl_lcnt_src(data)                                 ((0x0000FFFF&(data))>>0)
#define MIS_IC1_FS_SCL_LCNT_get_ic_fs_scl_lcnt(data)                                 ((0x0000FFFF&(data))>>0)


#define MIS_IC1_RAW_INTR_STAT                                                        0x1801BB34
#define MIS_IC1_RAW_INTR_STAT_reg_addr                                               "0xB801BB34"
#define MIS_IC1_RAW_INTR_STAT_reg                                                    0xB801BB34
#define set_MIS_IC1_RAW_INTR_STAT_reg(data)   (*((volatile unsigned int*) MIS_IC1_RAW_INTR_STAT_reg)=data)
#define get_MIS_IC1_RAW_INTR_STAT_reg   (*((volatile unsigned int*) MIS_IC1_RAW_INTR_STAT_reg))
#define MIS_IC1_RAW_INTR_STAT_inst_adr                                               "0x00CD"
#define MIS_IC1_RAW_INTR_STAT_inst                                                   0x00CD
#define MIS_IC1_RAW_INTR_STAT_gen_call_shift                                         (11)
#define MIS_IC1_RAW_INTR_STAT_gen_call_mask                                          (0x00000800)
#define MIS_IC1_RAW_INTR_STAT_gen_call(data)                                         (0x00000800&((data)<<11))
#define MIS_IC1_RAW_INTR_STAT_gen_call_src(data)                                     ((0x00000800&(data))>>11)
#define MIS_IC1_RAW_INTR_STAT_get_gen_call(data)                                     ((0x00000800&(data))>>11)
#define MIS_IC1_RAW_INTR_STAT_start_det_shift                                        (10)
#define MIS_IC1_RAW_INTR_STAT_start_det_mask                                         (0x00000400)
#define MIS_IC1_RAW_INTR_STAT_start_det(data)                                        (0x00000400&((data)<<10))
#define MIS_IC1_RAW_INTR_STAT_start_det_src(data)                                    ((0x00000400&(data))>>10)
#define MIS_IC1_RAW_INTR_STAT_get_start_det(data)                                    ((0x00000400&(data))>>10)
#define MIS_IC1_RAW_INTR_STAT_stop_det_shift                                         (9)
#define MIS_IC1_RAW_INTR_STAT_stop_det_mask                                          (0x00000200)
#define MIS_IC1_RAW_INTR_STAT_stop_det(data)                                         (0x00000200&((data)<<9))
#define MIS_IC1_RAW_INTR_STAT_stop_det_src(data)                                     ((0x00000200&(data))>>9)
#define MIS_IC1_RAW_INTR_STAT_get_stop_det(data)                                     ((0x00000200&(data))>>9)
#define MIS_IC1_RAW_INTR_STAT_activity_shift                                         (8)
#define MIS_IC1_RAW_INTR_STAT_activity_mask                                          (0x00000100)
#define MIS_IC1_RAW_INTR_STAT_activity(data)                                         (0x00000100&((data)<<8))
#define MIS_IC1_RAW_INTR_STAT_activity_src(data)                                     ((0x00000100&(data))>>8)
#define MIS_IC1_RAW_INTR_STAT_get_activity(data)                                     ((0x00000100&(data))>>8)
#define MIS_IC1_RAW_INTR_STAT_rx_done_shift                                          (7)
#define MIS_IC1_RAW_INTR_STAT_rx_done_mask                                           (0x00000080)
#define MIS_IC1_RAW_INTR_STAT_rx_done(data)                                          (0x00000080&((data)<<7))
#define MIS_IC1_RAW_INTR_STAT_rx_done_src(data)                                      ((0x00000080&(data))>>7)
#define MIS_IC1_RAW_INTR_STAT_get_rx_done(data)                                      ((0x00000080&(data))>>7)
#define MIS_IC1_RAW_INTR_STAT_tx_abrt_shift                                          (6)
#define MIS_IC1_RAW_INTR_STAT_tx_abrt_mask                                           (0x00000040)
#define MIS_IC1_RAW_INTR_STAT_tx_abrt(data)                                          (0x00000040&((data)<<6))
#define MIS_IC1_RAW_INTR_STAT_tx_abrt_src(data)                                      ((0x00000040&(data))>>6)
#define MIS_IC1_RAW_INTR_STAT_get_tx_abrt(data)                                      ((0x00000040&(data))>>6)
#define MIS_IC1_RAW_INTR_STAT_rd_req_shift                                           (5)
#define MIS_IC1_RAW_INTR_STAT_rd_req_mask                                            (0x00000020)
#define MIS_IC1_RAW_INTR_STAT_rd_req(data)                                           (0x00000020&((data)<<5))
#define MIS_IC1_RAW_INTR_STAT_rd_req_src(data)                                       ((0x00000020&(data))>>5)
#define MIS_IC1_RAW_INTR_STAT_get_rd_req(data)                                       ((0x00000020&(data))>>5)
#define MIS_IC1_RAW_INTR_STAT_tx_empty_shift                                         (4)
#define MIS_IC1_RAW_INTR_STAT_tx_empty_mask                                          (0x00000010)
#define MIS_IC1_RAW_INTR_STAT_tx_empty(data)                                         (0x00000010&((data)<<4))
#define MIS_IC1_RAW_INTR_STAT_tx_empty_src(data)                                     ((0x00000010&(data))>>4)
#define MIS_IC1_RAW_INTR_STAT_get_tx_empty(data)                                     ((0x00000010&(data))>>4)
#define MIS_IC1_RAW_INTR_STAT_tx_over_shift                                          (3)
#define MIS_IC1_RAW_INTR_STAT_tx_over_mask                                           (0x00000008)
#define MIS_IC1_RAW_INTR_STAT_tx_over(data)                                          (0x00000008&((data)<<3))
#define MIS_IC1_RAW_INTR_STAT_tx_over_src(data)                                      ((0x00000008&(data))>>3)
#define MIS_IC1_RAW_INTR_STAT_get_tx_over(data)                                      ((0x00000008&(data))>>3)
#define MIS_IC1_RAW_INTR_STAT_rx_full_shift                                          (2)
#define MIS_IC1_RAW_INTR_STAT_rx_full_mask                                           (0x00000004)
#define MIS_IC1_RAW_INTR_STAT_rx_full(data)                                          (0x00000004&((data)<<2))
#define MIS_IC1_RAW_INTR_STAT_rx_full_src(data)                                      ((0x00000004&(data))>>2)
#define MIS_IC1_RAW_INTR_STAT_get_rx_full(data)                                      ((0x00000004&(data))>>2)
#define MIS_IC1_RAW_INTR_STAT_rx_over_shift                                          (1)
#define MIS_IC1_RAW_INTR_STAT_rx_over_mask                                           (0x00000002)
#define MIS_IC1_RAW_INTR_STAT_rx_over(data)                                          (0x00000002&((data)<<1))
#define MIS_IC1_RAW_INTR_STAT_rx_over_src(data)                                      ((0x00000002&(data))>>1)
#define MIS_IC1_RAW_INTR_STAT_get_rx_over(data)                                      ((0x00000002&(data))>>1)
#define MIS_IC1_RAW_INTR_STAT_rx_under_shift                                         (0)
#define MIS_IC1_RAW_INTR_STAT_rx_under_mask                                          (0x00000001)
#define MIS_IC1_RAW_INTR_STAT_rx_under(data)                                         (0x00000001&((data)<<0))
#define MIS_IC1_RAW_INTR_STAT_rx_under_src(data)                                     ((0x00000001&(data))>>0)
#define MIS_IC1_RAW_INTR_STAT_get_rx_under(data)                                     ((0x00000001&(data))>>0)


#define MIS_IC1_INTR_MASK                                                            0x1801BB30
#define MIS_IC1_INTR_MASK_reg_addr                                                   "0xB801BB30"
#define MIS_IC1_INTR_MASK_reg                                                        0xB801BB30
#define set_MIS_IC1_INTR_MASK_reg(data)   (*((volatile unsigned int*) MIS_IC1_INTR_MASK_reg)=data)
#define get_MIS_IC1_INTR_MASK_reg   (*((volatile unsigned int*) MIS_IC1_INTR_MASK_reg))
#define MIS_IC1_INTR_MASK_inst_adr                                                   "0x00CC"
#define MIS_IC1_INTR_MASK_inst                                                       0x00CC
#define MIS_IC1_INTR_MASK_m_gen_call_shift                                           (11)
#define MIS_IC1_INTR_MASK_m_gen_call_mask                                            (0x00000800)
#define MIS_IC1_INTR_MASK_m_gen_call(data)                                           (0x00000800&((data)<<11))
#define MIS_IC1_INTR_MASK_m_gen_call_src(data)                                       ((0x00000800&(data))>>11)
#define MIS_IC1_INTR_MASK_get_m_gen_call(data)                                       ((0x00000800&(data))>>11)
#define MIS_IC1_INTR_MASK_m_start_det_shift                                          (10)
#define MIS_IC1_INTR_MASK_m_start_det_mask                                           (0x00000400)
#define MIS_IC1_INTR_MASK_m_start_det(data)                                          (0x00000400&((data)<<10))
#define MIS_IC1_INTR_MASK_m_start_det_src(data)                                      ((0x00000400&(data))>>10)
#define MIS_IC1_INTR_MASK_get_m_start_det(data)                                      ((0x00000400&(data))>>10)
#define MIS_IC1_INTR_MASK_m_stop_det_shift                                           (9)
#define MIS_IC1_INTR_MASK_m_stop_det_mask                                            (0x00000200)
#define MIS_IC1_INTR_MASK_m_stop_det(data)                                           (0x00000200&((data)<<9))
#define MIS_IC1_INTR_MASK_m_stop_det_src(data)                                       ((0x00000200&(data))>>9)
#define MIS_IC1_INTR_MASK_get_m_stop_det(data)                                       ((0x00000200&(data))>>9)
#define MIS_IC1_INTR_MASK_m_activity_shift                                           (8)
#define MIS_IC1_INTR_MASK_m_activity_mask                                            (0x00000100)
#define MIS_IC1_INTR_MASK_m_activity(data)                                           (0x00000100&((data)<<8))
#define MIS_IC1_INTR_MASK_m_activity_src(data)                                       ((0x00000100&(data))>>8)
#define MIS_IC1_INTR_MASK_get_m_activity(data)                                       ((0x00000100&(data))>>8)
#define MIS_IC1_INTR_MASK_m_rx_done_shift                                            (7)
#define MIS_IC1_INTR_MASK_m_rx_done_mask                                             (0x00000080)
#define MIS_IC1_INTR_MASK_m_rx_done(data)                                            (0x00000080&((data)<<7))
#define MIS_IC1_INTR_MASK_m_rx_done_src(data)                                        ((0x00000080&(data))>>7)
#define MIS_IC1_INTR_MASK_get_m_rx_done(data)                                        ((0x00000080&(data))>>7)
#define MIS_IC1_INTR_MASK_m_tx_abrt_shift                                            (6)
#define MIS_IC1_INTR_MASK_m_tx_abrt_mask                                             (0x00000040)
#define MIS_IC1_INTR_MASK_m_tx_abrt(data)                                            (0x00000040&((data)<<6))
#define MIS_IC1_INTR_MASK_m_tx_abrt_src(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC1_INTR_MASK_get_m_tx_abrt(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC1_INTR_MASK_m_rd_req_shift                                             (5)
#define MIS_IC1_INTR_MASK_m_rd_req_mask                                              (0x00000020)
#define MIS_IC1_INTR_MASK_m_rd_req(data)                                             (0x00000020&((data)<<5))
#define MIS_IC1_INTR_MASK_m_rd_req_src(data)                                         ((0x00000020&(data))>>5)
#define MIS_IC1_INTR_MASK_get_m_rd_req(data)                                         ((0x00000020&(data))>>5)
#define MIS_IC1_INTR_MASK_m_tx_empty_shift                                           (4)
#define MIS_IC1_INTR_MASK_m_tx_empty_mask                                            (0x00000010)
#define MIS_IC1_INTR_MASK_m_tx_empty(data)                                           (0x00000010&((data)<<4))
#define MIS_IC1_INTR_MASK_m_tx_empty_src(data)                                       ((0x00000010&(data))>>4)
#define MIS_IC1_INTR_MASK_get_m_tx_empty(data)                                       ((0x00000010&(data))>>4)
#define MIS_IC1_INTR_MASK_m_tx_over_shift                                            (3)
#define MIS_IC1_INTR_MASK_m_tx_over_mask                                             (0x00000008)
#define MIS_IC1_INTR_MASK_m_tx_over(data)                                            (0x00000008&((data)<<3))
#define MIS_IC1_INTR_MASK_m_tx_over_src(data)                                        ((0x00000008&(data))>>3)
#define MIS_IC1_INTR_MASK_get_m_tx_over(data)                                        ((0x00000008&(data))>>3)
#define MIS_IC1_INTR_MASK_m_rx_full_shift                                            (2)
#define MIS_IC1_INTR_MASK_m_rx_full_mask                                             (0x00000004)
#define MIS_IC1_INTR_MASK_m_rx_full(data)                                            (0x00000004&((data)<<2))
#define MIS_IC1_INTR_MASK_m_rx_full_src(data)                                        ((0x00000004&(data))>>2)
#define MIS_IC1_INTR_MASK_get_m_rx_full(data)                                        ((0x00000004&(data))>>2)
#define MIS_IC1_INTR_MASK_m_rx_over_shift                                            (1)
#define MIS_IC1_INTR_MASK_m_rx_over_mask                                             (0x00000002)
#define MIS_IC1_INTR_MASK_m_rx_over(data)                                            (0x00000002&((data)<<1))
#define MIS_IC1_INTR_MASK_m_rx_over_src(data)                                        ((0x00000002&(data))>>1)
#define MIS_IC1_INTR_MASK_get_m_rx_over(data)                                        ((0x00000002&(data))>>1)
#define MIS_IC1_INTR_MASK_m_rx_under_shift                                           (0)
#define MIS_IC1_INTR_MASK_m_rx_under_mask                                            (0x00000001)
#define MIS_IC1_INTR_MASK_m_rx_under(data)                                           (0x00000001&((data)<<0))
#define MIS_IC1_INTR_MASK_m_rx_under_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_IC1_INTR_MASK_get_m_rx_under(data)                                       ((0x00000001&(data))>>0)


#define MIS_IC1_INTR_STAT                                                            0x1801BB2C
#define MIS_IC1_INTR_STAT_reg_addr                                                   "0xB801BB2C"
#define MIS_IC1_INTR_STAT_reg                                                        0xB801BB2C
#define set_MIS_IC1_INTR_STAT_reg(data)   (*((volatile unsigned int*) MIS_IC1_INTR_STAT_reg)=data)
#define get_MIS_IC1_INTR_STAT_reg   (*((volatile unsigned int*) MIS_IC1_INTR_STAT_reg))
#define MIS_IC1_INTR_STAT_inst_adr                                                   "0x00CB"
#define MIS_IC1_INTR_STAT_inst                                                       0x00CB
#define MIS_IC1_INTR_STAT_r_gen_call_shift                                           (11)
#define MIS_IC1_INTR_STAT_r_gen_call_mask                                            (0x00000800)
#define MIS_IC1_INTR_STAT_r_gen_call(data)                                           (0x00000800&((data)<<11))
#define MIS_IC1_INTR_STAT_r_gen_call_src(data)                                       ((0x00000800&(data))>>11)
#define MIS_IC1_INTR_STAT_get_r_gen_call(data)                                       ((0x00000800&(data))>>11)
#define MIS_IC1_INTR_STAT_r_start_det_shift                                          (10)
#define MIS_IC1_INTR_STAT_r_start_det_mask                                           (0x00000400)
#define MIS_IC1_INTR_STAT_r_start_det(data)                                          (0x00000400&((data)<<10))
#define MIS_IC1_INTR_STAT_r_start_det_src(data)                                      ((0x00000400&(data))>>10)
#define MIS_IC1_INTR_STAT_get_r_start_det(data)                                      ((0x00000400&(data))>>10)
#define MIS_IC1_INTR_STAT_r_stop_det_shift                                           (9)
#define MIS_IC1_INTR_STAT_r_stop_det_mask                                            (0x00000200)
#define MIS_IC1_INTR_STAT_r_stop_det(data)                                           (0x00000200&((data)<<9))
#define MIS_IC1_INTR_STAT_r_stop_det_src(data)                                       ((0x00000200&(data))>>9)
#define MIS_IC1_INTR_STAT_get_r_stop_det(data)                                       ((0x00000200&(data))>>9)
#define MIS_IC1_INTR_STAT_r_activity_shift                                           (8)
#define MIS_IC1_INTR_STAT_r_activity_mask                                            (0x00000100)
#define MIS_IC1_INTR_STAT_r_activity(data)                                           (0x00000100&((data)<<8))
#define MIS_IC1_INTR_STAT_r_activity_src(data)                                       ((0x00000100&(data))>>8)
#define MIS_IC1_INTR_STAT_get_r_activity(data)                                       ((0x00000100&(data))>>8)
#define MIS_IC1_INTR_STAT_r_rx_done_shift                                            (7)
#define MIS_IC1_INTR_STAT_r_rx_done_mask                                             (0x00000080)
#define MIS_IC1_INTR_STAT_r_rx_done(data)                                            (0x00000080&((data)<<7))
#define MIS_IC1_INTR_STAT_r_rx_done_src(data)                                        ((0x00000080&(data))>>7)
#define MIS_IC1_INTR_STAT_get_r_rx_done(data)                                        ((0x00000080&(data))>>7)
#define MIS_IC1_INTR_STAT_r_tx_abrt_shift                                            (6)
#define MIS_IC1_INTR_STAT_r_tx_abrt_mask                                             (0x00000040)
#define MIS_IC1_INTR_STAT_r_tx_abrt(data)                                            (0x00000040&((data)<<6))
#define MIS_IC1_INTR_STAT_r_tx_abrt_src(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC1_INTR_STAT_get_r_tx_abrt(data)                                        ((0x00000040&(data))>>6)
#define MIS_IC1_INTR_STAT_r_rd_req_shift                                             (5)
#define MIS_IC1_INTR_STAT_r_rd_req_mask                                              (0x00000020)
#define MIS_IC1_INTR_STAT_r_rd_req(data)                                             (0x00000020&((data)<<5))
#define MIS_IC1_INTR_STAT_r_rd_req_src(data)                                         ((0x00000020&(data))>>5)
#define MIS_IC1_INTR_STAT_get_r_rd_req(data)                                         ((0x00000020&(data))>>5)
#define MIS_IC1_INTR_STAT_r_tx_empty_shift                                           (4)
#define MIS_IC1_INTR_STAT_r_tx_empty_mask                                            (0x00000010)
#define MIS_IC1_INTR_STAT_r_tx_empty(data)                                           (0x00000010&((data)<<4))
#define MIS_IC1_INTR_STAT_r_tx_empty_src(data)                                       ((0x00000010&(data))>>4)
#define MIS_IC1_INTR_STAT_get_r_tx_empty(data)                                       ((0x00000010&(data))>>4)
#define MIS_IC1_INTR_STAT_r_tx_over_shift                                            (3)
#define MIS_IC1_INTR_STAT_r_tx_over_mask                                             (0x00000008)
#define MIS_IC1_INTR_STAT_r_tx_over(data)                                            (0x00000008&((data)<<3))
#define MIS_IC1_INTR_STAT_r_tx_over_src(data)                                        ((0x00000008&(data))>>3)
#define MIS_IC1_INTR_STAT_get_r_tx_over(data)                                        ((0x00000008&(data))>>3)
#define MIS_IC1_INTR_STAT_r_rx_full_shift                                            (2)
#define MIS_IC1_INTR_STAT_r_rx_full_mask                                             (0x00000004)
#define MIS_IC1_INTR_STAT_r_rx_full(data)                                            (0x00000004&((data)<<2))
#define MIS_IC1_INTR_STAT_r_rx_full_src(data)                                        ((0x00000004&(data))>>2)
#define MIS_IC1_INTR_STAT_get_r_rx_full(data)                                        ((0x00000004&(data))>>2)
#define MIS_IC1_INTR_STAT_r_rx_over_shift                                            (1)
#define MIS_IC1_INTR_STAT_r_rx_over_mask                                             (0x00000002)
#define MIS_IC1_INTR_STAT_r_rx_over(data)                                            (0x00000002&((data)<<1))
#define MIS_IC1_INTR_STAT_r_rx_over_src(data)                                        ((0x00000002&(data))>>1)
#define MIS_IC1_INTR_STAT_get_r_rx_over(data)                                        ((0x00000002&(data))>>1)
#define MIS_IC1_INTR_STAT_r_rx_under_shift                                           (0)
#define MIS_IC1_INTR_STAT_r_rx_under_mask                                            (0x00000001)
#define MIS_IC1_INTR_STAT_r_rx_under(data)                                           (0x00000001&((data)<<0))
#define MIS_IC1_INTR_STAT_r_rx_under_src(data)                                       ((0x00000001&(data))>>0)
#define MIS_IC1_INTR_STAT_get_r_rx_under(data)                                       ((0x00000001&(data))>>0)


#define MIS_IC1_RX_TL                                                                0x1801BB38
#define MIS_IC1_RX_TL_reg_addr                                                       "0xB801BB38"
#define MIS_IC1_RX_TL_reg                                                            0xB801BB38
#define set_MIS_IC1_RX_TL_reg(data)   (*((volatile unsigned int*) MIS_IC1_RX_TL_reg)=data)
#define get_MIS_IC1_RX_TL_reg   (*((volatile unsigned int*) MIS_IC1_RX_TL_reg))
#define MIS_IC1_RX_TL_inst_adr                                                       "0x00CE"
#define MIS_IC1_RX_TL_inst                                                           0x00CE
#define MIS_IC1_RX_TL_rx_tl_shift                                                    (0)
#define MIS_IC1_RX_TL_rx_tl_mask                                                     (0x000000FF)
#define MIS_IC1_RX_TL_rx_tl(data)                                                    (0x000000FF&((data)<<0))
#define MIS_IC1_RX_TL_rx_tl_src(data)                                                ((0x000000FF&(data))>>0)
#define MIS_IC1_RX_TL_get_rx_tl(data)                                                ((0x000000FF&(data))>>0)


#define MIS_IC1_TX_TL                                                                0x1801BB3C
#define MIS_IC1_TX_TL_reg_addr                                                       "0xB801BB3C"
#define MIS_IC1_TX_TL_reg                                                            0xB801BB3C
#define set_MIS_IC1_TX_TL_reg(data)   (*((volatile unsigned int*) MIS_IC1_TX_TL_reg)=data)
#define get_MIS_IC1_TX_TL_reg   (*((volatile unsigned int*) MIS_IC1_TX_TL_reg))
#define MIS_IC1_TX_TL_inst_adr                                                       "0x00CF"
#define MIS_IC1_TX_TL_inst                                                           0x00CF
#define MIS_IC1_TX_TL_tx_tl_shift                                                    (0)
#define MIS_IC1_TX_TL_tx_tl_mask                                                     (0x000000FF)
#define MIS_IC1_TX_TL_tx_tl(data)                                                    (0x000000FF&((data)<<0))
#define MIS_IC1_TX_TL_tx_tl_src(data)                                                ((0x000000FF&(data))>>0)
#define MIS_IC1_TX_TL_get_tx_tl(data)                                                ((0x000000FF&(data))>>0)


#define MIS_IC1_CLR_INTR                                                             0x1801BB40
#define MIS_IC1_CLR_INTR_reg_addr                                                    "0xB801BB40"
#define MIS_IC1_CLR_INTR_reg                                                         0xB801BB40
#define set_MIS_IC1_CLR_INTR_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_INTR_reg)=data)
#define get_MIS_IC1_CLR_INTR_reg   (*((volatile unsigned int*) MIS_IC1_CLR_INTR_reg))
#define MIS_IC1_CLR_INTR_inst_adr                                                    "0x00D0"
#define MIS_IC1_CLR_INTR_inst                                                        0x00D0
#define MIS_IC1_CLR_INTR_clr_intr_shift                                              (0)
#define MIS_IC1_CLR_INTR_clr_intr_mask                                               (0x00000001)
#define MIS_IC1_CLR_INTR_clr_intr(data)                                              (0x00000001&((data)<<0))
#define MIS_IC1_CLR_INTR_clr_intr_src(data)                                          ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_INTR_get_clr_intr(data)                                          ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_RX_UNDER                                                         0x1801BB44
#define MIS_IC1_CLR_RX_UNDER_reg_addr                                                "0xB801BB44"
#define MIS_IC1_CLR_RX_UNDER_reg                                                     0xB801BB44
#define set_MIS_IC1_CLR_RX_UNDER_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_RX_UNDER_reg)=data)
#define get_MIS_IC1_CLR_RX_UNDER_reg   (*((volatile unsigned int*) MIS_IC1_CLR_RX_UNDER_reg))
#define MIS_IC1_CLR_RX_UNDER_inst_adr                                                "0x00D1"
#define MIS_IC1_CLR_RX_UNDER_inst                                                    0x00D1
#define MIS_IC1_CLR_RX_UNDER_clr_rx_under_shift                                      (0)
#define MIS_IC1_CLR_RX_UNDER_clr_rx_under_mask                                       (0x00000001)
#define MIS_IC1_CLR_RX_UNDER_clr_rx_under(data)                                      (0x00000001&((data)<<0))
#define MIS_IC1_CLR_RX_UNDER_clr_rx_under_src(data)                                  ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_RX_UNDER_get_clr_rx_under(data)                                  ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_RX_OVER                                                          0x1801BB48
#define MIS_IC1_CLR_RX_OVER_reg_addr                                                 "0xB801BB48"
#define MIS_IC1_CLR_RX_OVER_reg                                                      0xB801BB48
#define set_MIS_IC1_CLR_RX_OVER_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_RX_OVER_reg)=data)
#define get_MIS_IC1_CLR_RX_OVER_reg   (*((volatile unsigned int*) MIS_IC1_CLR_RX_OVER_reg))
#define MIS_IC1_CLR_RX_OVER_inst_adr                                                 "0x00D2"
#define MIS_IC1_CLR_RX_OVER_inst                                                     0x00D2
#define MIS_IC1_CLR_RX_OVER_clr_rx_over_shift                                        (0)
#define MIS_IC1_CLR_RX_OVER_clr_rx_over_mask                                         (0x00000001)
#define MIS_IC1_CLR_RX_OVER_clr_rx_over(data)                                        (0x00000001&((data)<<0))
#define MIS_IC1_CLR_RX_OVER_clr_rx_over_src(data)                                    ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_RX_OVER_get_clr_rx_over(data)                                    ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_TX_OVER                                                          0x1801BB4C
#define MIS_IC1_CLR_TX_OVER_reg_addr                                                 "0xB801BB4C"
#define MIS_IC1_CLR_TX_OVER_reg                                                      0xB801BB4C
#define set_MIS_IC1_CLR_TX_OVER_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_TX_OVER_reg)=data)
#define get_MIS_IC1_CLR_TX_OVER_reg   (*((volatile unsigned int*) MIS_IC1_CLR_TX_OVER_reg))
#define MIS_IC1_CLR_TX_OVER_inst_adr                                                 "0x00D3"
#define MIS_IC1_CLR_TX_OVER_inst                                                     0x00D3
#define MIS_IC1_CLR_TX_OVER_clr_tx_over_shift                                        (0)
#define MIS_IC1_CLR_TX_OVER_clr_tx_over_mask                                         (0x00000001)
#define MIS_IC1_CLR_TX_OVER_clr_tx_over(data)                                        (0x00000001&((data)<<0))
#define MIS_IC1_CLR_TX_OVER_clr_tx_over_src(data)                                    ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_TX_OVER_get_clr_tx_over(data)                                    ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_RD_REQ                                                           0x1801BB50
#define MIS_IC1_CLR_RD_REQ_reg_addr                                                  "0xB801BB50"
#define MIS_IC1_CLR_RD_REQ_reg                                                       0xB801BB50
#define set_MIS_IC1_CLR_RD_REQ_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_RD_REQ_reg)=data)
#define get_MIS_IC1_CLR_RD_REQ_reg   (*((volatile unsigned int*) MIS_IC1_CLR_RD_REQ_reg))
#define MIS_IC1_CLR_RD_REQ_inst_adr                                                  "0x00D4"
#define MIS_IC1_CLR_RD_REQ_inst                                                      0x00D4
#define MIS_IC1_CLR_RD_REQ_clr_rd_req_shift                                          (0)
#define MIS_IC1_CLR_RD_REQ_clr_rd_req_mask                                           (0x00000001)
#define MIS_IC1_CLR_RD_REQ_clr_rd_req(data)                                          (0x00000001&((data)<<0))
#define MIS_IC1_CLR_RD_REQ_clr_rd_req_src(data)                                      ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_RD_REQ_get_clr_rd_req(data)                                      ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_TX_ABRT                                                          0x1801BB54
#define MIS_IC1_CLR_TX_ABRT_reg_addr                                                 "0xB801BB54"
#define MIS_IC1_CLR_TX_ABRT_reg                                                      0xB801BB54
#define set_MIS_IC1_CLR_TX_ABRT_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_TX_ABRT_reg)=data)
#define get_MIS_IC1_CLR_TX_ABRT_reg   (*((volatile unsigned int*) MIS_IC1_CLR_TX_ABRT_reg))
#define MIS_IC1_CLR_TX_ABRT_inst_adr                                                 "0x00D5"
#define MIS_IC1_CLR_TX_ABRT_inst                                                     0x00D5
#define MIS_IC1_CLR_TX_ABRT_clr_tx_abrt_shift                                        (0)
#define MIS_IC1_CLR_TX_ABRT_clr_tx_abrt_mask                                         (0x00000001)
#define MIS_IC1_CLR_TX_ABRT_clr_tx_abrt(data)                                        (0x00000001&((data)<<0))
#define MIS_IC1_CLR_TX_ABRT_clr_tx_abrt_src(data)                                    ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_TX_ABRT_get_clr_tx_abrt(data)                                    ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_RX_DONE                                                          0x1801BB58
#define MIS_IC1_CLR_RX_DONE_reg_addr                                                 "0xB801BB58"
#define MIS_IC1_CLR_RX_DONE_reg                                                      0xB801BB58
#define set_MIS_IC1_CLR_RX_DONE_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_RX_DONE_reg)=data)
#define get_MIS_IC1_CLR_RX_DONE_reg   (*((volatile unsigned int*) MIS_IC1_CLR_RX_DONE_reg))
#define MIS_IC1_CLR_RX_DONE_inst_adr                                                 "0x00D6"
#define MIS_IC1_CLR_RX_DONE_inst                                                     0x00D6
#define MIS_IC1_CLR_RX_DONE_clr_rx_done_shift                                        (0)
#define MIS_IC1_CLR_RX_DONE_clr_rx_done_mask                                         (0x00000001)
#define MIS_IC1_CLR_RX_DONE_clr_rx_done(data)                                        (0x00000001&((data)<<0))
#define MIS_IC1_CLR_RX_DONE_clr_rx_done_src(data)                                    ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_RX_DONE_get_clr_rx_done(data)                                    ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_ACTIVITY                                                         0x1801BB5C
#define MIS_IC1_CLR_ACTIVITY_reg_addr                                                "0xB801BB5C"
#define MIS_IC1_CLR_ACTIVITY_reg                                                     0xB801BB5C
#define set_MIS_IC1_CLR_ACTIVITY_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_ACTIVITY_reg)=data)
#define get_MIS_IC1_CLR_ACTIVITY_reg   (*((volatile unsigned int*) MIS_IC1_CLR_ACTIVITY_reg))
#define MIS_IC1_CLR_ACTIVITY_inst_adr                                                "0x00D7"
#define MIS_IC1_CLR_ACTIVITY_inst                                                    0x00D7
#define MIS_IC1_CLR_ACTIVITY_clr_activity_shift                                      (0)
#define MIS_IC1_CLR_ACTIVITY_clr_activity_mask                                       (0x00000001)
#define MIS_IC1_CLR_ACTIVITY_clr_activity(data)                                      (0x00000001&((data)<<0))
#define MIS_IC1_CLR_ACTIVITY_clr_activity_src(data)                                  ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_ACTIVITY_get_clr_activity(data)                                  ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_STOP_DET                                                         0x1801BB60
#define MIS_IC1_CLR_STOP_DET_reg_addr                                                "0xB801BB60"
#define MIS_IC1_CLR_STOP_DET_reg                                                     0xB801BB60
#define set_MIS_IC1_CLR_STOP_DET_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_STOP_DET_reg)=data)
#define get_MIS_IC1_CLR_STOP_DET_reg   (*((volatile unsigned int*) MIS_IC1_CLR_STOP_DET_reg))
#define MIS_IC1_CLR_STOP_DET_inst_adr                                                "0x00D8"
#define MIS_IC1_CLR_STOP_DET_inst                                                    0x00D8
#define MIS_IC1_CLR_STOP_DET_clr_stop_det_shift                                      (0)
#define MIS_IC1_CLR_STOP_DET_clr_stop_det_mask                                       (0x00000001)
#define MIS_IC1_CLR_STOP_DET_clr_stop_det(data)                                      (0x00000001&((data)<<0))
#define MIS_IC1_CLR_STOP_DET_clr_stop_det_src(data)                                  ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_STOP_DET_get_clr_stop_det(data)                                  ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_START_DET                                                        0x1801BB64
#define MIS_IC1_CLR_START_DET_reg_addr                                               "0xB801BB64"
#define MIS_IC1_CLR_START_DET_reg                                                    0xB801BB64
#define set_MIS_IC1_CLR_START_DET_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_START_DET_reg)=data)
#define get_MIS_IC1_CLR_START_DET_reg   (*((volatile unsigned int*) MIS_IC1_CLR_START_DET_reg))
#define MIS_IC1_CLR_START_DET_inst_adr                                               "0x00D9"
#define MIS_IC1_CLR_START_DET_inst                                                   0x00D9
#define MIS_IC1_CLR_START_DET_clr_start_det_shift                                    (0)
#define MIS_IC1_CLR_START_DET_clr_start_det_mask                                     (0x00000001)
#define MIS_IC1_CLR_START_DET_clr_start_det(data)                                    (0x00000001&((data)<<0))
#define MIS_IC1_CLR_START_DET_clr_start_det_src(data)                                ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_START_DET_get_clr_start_det(data)                                ((0x00000001&(data))>>0)


#define MIS_IC1_CLR_GEN_CALL                                                         0x1801BB68
#define MIS_IC1_CLR_GEN_CALL_reg_addr                                                "0xB801BB68"
#define MIS_IC1_CLR_GEN_CALL_reg                                                     0xB801BB68
#define set_MIS_IC1_CLR_GEN_CALL_reg(data)   (*((volatile unsigned int*) MIS_IC1_CLR_GEN_CALL_reg)=data)
#define get_MIS_IC1_CLR_GEN_CALL_reg   (*((volatile unsigned int*) MIS_IC1_CLR_GEN_CALL_reg))
#define MIS_IC1_CLR_GEN_CALL_inst_adr                                                "0x00DA"
#define MIS_IC1_CLR_GEN_CALL_inst                                                    0x00DA
#define MIS_IC1_CLR_GEN_CALL_clr_gen_call_shift                                      (0)
#define MIS_IC1_CLR_GEN_CALL_clr_gen_call_mask                                       (0x00000001)
#define MIS_IC1_CLR_GEN_CALL_clr_gen_call(data)                                      (0x00000001&((data)<<0))
#define MIS_IC1_CLR_GEN_CALL_clr_gen_call_src(data)                                  ((0x00000001&(data))>>0)
#define MIS_IC1_CLR_GEN_CALL_get_clr_gen_call(data)                                  ((0x00000001&(data))>>0)


#define MIS_IC1_ENABLE                                                               0x1801BB6C
#define MIS_IC1_ENABLE_reg_addr                                                      "0xB801BB6C"
#define MIS_IC1_ENABLE_reg                                                           0xB801BB6C
#define set_MIS_IC1_ENABLE_reg(data)   (*((volatile unsigned int*) MIS_IC1_ENABLE_reg)=data)
#define get_MIS_IC1_ENABLE_reg   (*((volatile unsigned int*) MIS_IC1_ENABLE_reg))
#define MIS_IC1_ENABLE_inst_adr                                                      "0x00DB"
#define MIS_IC1_ENABLE_inst                                                          0x00DB
#define MIS_IC1_ENABLE_enable_shift                                                  (0)
#define MIS_IC1_ENABLE_enable_mask                                                   (0x00000001)
#define MIS_IC1_ENABLE_enable(data)                                                  (0x00000001&((data)<<0))
#define MIS_IC1_ENABLE_enable_src(data)                                              ((0x00000001&(data))>>0)
#define MIS_IC1_ENABLE_get_enable(data)                                              ((0x00000001&(data))>>0)


#define MIS_IC1_STATUS                                                               0x1801BB70
#define MIS_IC1_STATUS_reg_addr                                                      "0xB801BB70"
#define MIS_IC1_STATUS_reg                                                           0xB801BB70
#define set_MIS_IC1_STATUS_reg(data)   (*((volatile unsigned int*) MIS_IC1_STATUS_reg)=data)
#define get_MIS_IC1_STATUS_reg   (*((volatile unsigned int*) MIS_IC1_STATUS_reg))
#define MIS_IC1_STATUS_inst_adr                                                      "0x00DC"
#define MIS_IC1_STATUS_inst                                                          0x00DC
#define MIS_IC1_STATUS_rff_shift                                                     (4)
#define MIS_IC1_STATUS_rff_mask                                                      (0x00000010)
#define MIS_IC1_STATUS_rff(data)                                                     (0x00000010&((data)<<4))
#define MIS_IC1_STATUS_rff_src(data)                                                 ((0x00000010&(data))>>4)
#define MIS_IC1_STATUS_get_rff(data)                                                 ((0x00000010&(data))>>4)
#define MIS_IC1_STATUS_rfne_shift                                                    (3)
#define MIS_IC1_STATUS_rfne_mask                                                     (0x00000008)
#define MIS_IC1_STATUS_rfne(data)                                                    (0x00000008&((data)<<3))
#define MIS_IC1_STATUS_rfne_src(data)                                                ((0x00000008&(data))>>3)
#define MIS_IC1_STATUS_get_rfne(data)                                                ((0x00000008&(data))>>3)
#define MIS_IC1_STATUS_tfe_shift                                                     (2)
#define MIS_IC1_STATUS_tfe_mask                                                      (0x00000004)
#define MIS_IC1_STATUS_tfe(data)                                                     (0x00000004&((data)<<2))
#define MIS_IC1_STATUS_tfe_src(data)                                                 ((0x00000004&(data))>>2)
#define MIS_IC1_STATUS_get_tfe(data)                                                 ((0x00000004&(data))>>2)
#define MIS_IC1_STATUS_tfnf_shift                                                    (1)
#define MIS_IC1_STATUS_tfnf_mask                                                     (0x00000002)
#define MIS_IC1_STATUS_tfnf(data)                                                    (0x00000002&((data)<<1))
#define MIS_IC1_STATUS_tfnf_src(data)                                                ((0x00000002&(data))>>1)
#define MIS_IC1_STATUS_get_tfnf(data)                                                ((0x00000002&(data))>>1)
#define MIS_IC1_STATUS_activity_shift                                                (0)
#define MIS_IC1_STATUS_activity_mask                                                 (0x00000001)
#define MIS_IC1_STATUS_activity(data)                                                (0x00000001&((data)<<0))
#define MIS_IC1_STATUS_activity_src(data)                                            ((0x00000001&(data))>>0)
#define MIS_IC1_STATUS_get_activity(data)                                            ((0x00000001&(data))>>0)


#define MIS_IC1_TXFLR                                                                0x1801BB74
#define MIS_IC1_TXFLR_reg_addr                                                       "0xB801BB74"
#define MIS_IC1_TXFLR_reg                                                            0xB801BB74
#define set_MIS_IC1_TXFLR_reg(data)   (*((volatile unsigned int*) MIS_IC1_TXFLR_reg)=data)
#define get_MIS_IC1_TXFLR_reg   (*((volatile unsigned int*) MIS_IC1_TXFLR_reg))
#define MIS_IC1_TXFLR_inst_adr                                                       "0x00DD"
#define MIS_IC1_TXFLR_inst                                                           0x00DD
#define MIS_IC1_TXFLR_txflr_shift                                                    (0)
#define MIS_IC1_TXFLR_txflr_mask                                                     (0x0000000F)
#define MIS_IC1_TXFLR_txflr(data)                                                    (0x0000000F&((data)<<0))
#define MIS_IC1_TXFLR_txflr_src(data)                                                ((0x0000000F&(data))>>0)
#define MIS_IC1_TXFLR_get_txflr(data)                                                ((0x0000000F&(data))>>0)


#define MIS_IC1_RXFLR                                                                0x1801BB78
#define MIS_IC1_RXFLR_reg_addr                                                       "0xB801BB78"
#define MIS_IC1_RXFLR_reg                                                            0xB801BB78
#define set_MIS_IC1_RXFLR_reg(data)   (*((volatile unsigned int*) MIS_IC1_RXFLR_reg)=data)
#define get_MIS_IC1_RXFLR_reg   (*((volatile unsigned int*) MIS_IC1_RXFLR_reg))
#define MIS_IC1_RXFLR_inst_adr                                                       "0x00DE"
#define MIS_IC1_RXFLR_inst                                                           0x00DE
#define MIS_IC1_RXFLR_rxflr_shift                                                    (0)
#define MIS_IC1_RXFLR_rxflr_mask                                                     (0x0000000F)
#define MIS_IC1_RXFLR_rxflr(data)                                                    (0x0000000F&((data)<<0))
#define MIS_IC1_RXFLR_rxflr_src(data)                                                ((0x0000000F&(data))>>0)
#define MIS_IC1_RXFLR_get_rxflr(data)                                                ((0x0000000F&(data))>>0)


#define MIS_IC1_TX_ABRT_SOURCE                                                       0x1801BB80
#define MIS_IC1_TX_ABRT_SOURCE_reg_addr                                              "0xB801BB80"
#define MIS_IC1_TX_ABRT_SOURCE_reg                                                   0xB801BB80
#define set_MIS_IC1_TX_ABRT_SOURCE_reg(data)   (*((volatile unsigned int*) MIS_IC1_TX_ABRT_SOURCE_reg)=data)
#define get_MIS_IC1_TX_ABRT_SOURCE_reg   (*((volatile unsigned int*) MIS_IC1_TX_ABRT_SOURCE_reg))
#define MIS_IC1_TX_ABRT_SOURCE_inst_adr                                              "0x00E0"
#define MIS_IC1_TX_ABRT_SOURCE_inst                                                  0x00E0
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvrd_intx_shift                                 (15)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvrd_intx_mask                                  (0x00008000)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvrd_intx(data)                                 (0x00008000&((data)<<15))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvrd_intx_src(data)                             ((0x00008000&(data))>>15)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_slvrd_intx(data)                             ((0x00008000&(data))>>15)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slv_arblost_shift                                (14)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slv_arblost_mask                                 (0x00004000)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slv_arblost(data)                                (0x00004000&((data)<<14))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slv_arblost_src(data)                            ((0x00004000&(data))>>14)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_slv_arblost(data)                            ((0x00004000&(data))>>14)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvflush_txfifo_shift                            (13)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvflush_txfifo_mask                             (0x00002000)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvflush_txfifo(data)                            (0x00002000&((data)<<13))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_slvflush_txfifo_src(data)                        ((0x00002000&(data))>>13)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_slvflush_txfifo(data)                        ((0x00002000&(data))>>13)
#define MIS_IC1_TX_ABRT_SOURCE_arb_lost_shift                                        (12)
#define MIS_IC1_TX_ABRT_SOURCE_arb_lost_mask                                         (0x00001000)
#define MIS_IC1_TX_ABRT_SOURCE_arb_lost(data)                                        (0x00001000&((data)<<12))
#define MIS_IC1_TX_ABRT_SOURCE_arb_lost_src(data)                                    ((0x00001000&(data))>>12)
#define MIS_IC1_TX_ABRT_SOURCE_get_arb_lost(data)                                    ((0x00001000&(data))>>12)
#define MIS_IC1_TX_ABRT_SOURCE_arb_master_dis_shift                                  (11)
#define MIS_IC1_TX_ABRT_SOURCE_arb_master_dis_mask                                   (0x00000800)
#define MIS_IC1_TX_ABRT_SOURCE_arb_master_dis(data)                                  (0x00000800&((data)<<11))
#define MIS_IC1_TX_ABRT_SOURCE_arb_master_dis_src(data)                              ((0x00000800&(data))>>11)
#define MIS_IC1_TX_ABRT_SOURCE_get_arb_master_dis(data)                              ((0x00000800&(data))>>11)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10b_rd_norstrt_shift                             (10)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10b_rd_norstrt_mask                              (0x00000400)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10b_rd_norstrt(data)                             (0x00000400&((data)<<10))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10b_rd_norstrt_src(data)                         ((0x00000400&(data))>>10)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_10b_rd_norstrt(data)                         ((0x00000400&(data))>>10)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_norstrt_shift                              (9)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_norstrt_mask                               (0x00000200)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_norstrt(data)                              (0x00000200&((data)<<9))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_norstrt_src(data)                          ((0x00000200&(data))>>9)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_sbyte_norstrt(data)                          ((0x00000200&(data))>>9)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_norstrt_shift                                 (8)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_norstrt_mask                                  (0x00000100)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_norstrt(data)                                 (0x00000100&((data)<<8))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_norstrt_src(data)                             ((0x00000100&(data))>>8)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_hs_norstrt(data)                             ((0x00000100&(data))>>8)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_ackdet_shift                               (7)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_ackdet_mask                                (0x00000080)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_ackdet(data)                               (0x00000080&((data)<<7))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_sbyte_ackdet_src(data)                           ((0x00000080&(data))>>7)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_sbyte_ackdet(data)                           ((0x00000080&(data))>>7)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_ackdet_shift                                  (6)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_ackdet_mask                                   (0x00000040)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_ackdet(data)                                  (0x00000040&((data)<<6))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_hs_ackdet_src(data)                              ((0x00000040&(data))>>6)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_hs_ackdet(data)                              ((0x00000040&(data))>>6)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_read_shift                                 (5)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_read_mask                                  (0x00000020)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_read(data)                                 (0x00000020&((data)<<5))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_read_src(data)                             ((0x00000020&(data))>>5)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_gcall_read(data)                             ((0x00000020&(data))>>5)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_noack_shift                                (4)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_noack_mask                                 (0x00000010)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_noack(data)                                (0x00000010&((data)<<4))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_gcall_noack_src(data)                            ((0x00000010&(data))>>4)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_gcall_noack(data)                            ((0x00000010&(data))>>4)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_txdata_noack_shift                               (3)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_txdata_noack_mask                                (0x00000008)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_txdata_noack(data)                               (0x00000008&((data)<<3))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_txdata_noack_src(data)                           ((0x00000008&(data))>>3)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_txdata_noack(data)                           ((0x00000008&(data))>>3)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr2_noack_shift                              (2)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr2_noack_mask                               (0x00000004)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr2_noack(data)                              (0x00000004&((data)<<2))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr2_noack_src(data)                          ((0x00000004&(data))>>2)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_10addr2_noack(data)                          ((0x00000004&(data))>>2)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr1_noack_shift                              (1)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr1_noack_mask                               (0x00000002)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr1_noack(data)                              (0x00000002&((data)<<1))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_10addr1_noack_src(data)                          ((0x00000002&(data))>>1)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_10addr1_noack(data)                          ((0x00000002&(data))>>1)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_7b_addr_noack_shift                              (0)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_7b_addr_noack_mask                               (0x00000001)
#define MIS_IC1_TX_ABRT_SOURCE_abrt_7b_addr_noack(data)                              (0x00000001&((data)<<0))
#define MIS_IC1_TX_ABRT_SOURCE_abrt_7b_addr_noack_src(data)                          ((0x00000001&(data))>>0)
#define MIS_IC1_TX_ABRT_SOURCE_get_abrt_7b_addr_noack(data)                          ((0x00000001&(data))>>0)


#define MIS_IC1_DMA_CR                                                               0x1801BB88
#define MIS_IC1_DMA_CR_reg_addr                                                      "0xB801BB88"
#define MIS_IC1_DMA_CR_reg                                                           0xB801BB88
#define set_MIS_IC1_DMA_CR_reg(data)   (*((volatile unsigned int*) MIS_IC1_DMA_CR_reg)=data)
#define get_MIS_IC1_DMA_CR_reg   (*((volatile unsigned int*) MIS_IC1_DMA_CR_reg))
#define MIS_IC1_DMA_CR_inst_adr                                                      "0x00E2"
#define MIS_IC1_DMA_CR_inst                                                          0x00E2
#define MIS_IC1_DMA_CR_tdmae_shift                                                   (1)
#define MIS_IC1_DMA_CR_tdmae_mask                                                    (0x00000002)
#define MIS_IC1_DMA_CR_tdmae(data)                                                   (0x00000002&((data)<<1))
#define MIS_IC1_DMA_CR_tdmae_src(data)                                               ((0x00000002&(data))>>1)
#define MIS_IC1_DMA_CR_get_tdmae(data)                                               ((0x00000002&(data))>>1)
#define MIS_IC1_DMA_CR_rdmae_shift                                                   (0)
#define MIS_IC1_DMA_CR_rdmae_mask                                                    (0x00000001)
#define MIS_IC1_DMA_CR_rdmae(data)                                                   (0x00000001&((data)<<0))
#define MIS_IC1_DMA_CR_rdmae_src(data)                                               ((0x00000001&(data))>>0)
#define MIS_IC1_DMA_CR_get_rdmae(data)                                               ((0x00000001&(data))>>0)


#define MIS_IC1_DMA_TDLR                                                             0x1801BB8C
#define MIS_IC1_DMA_TDLR_reg_addr                                                    "0xB801BB8C"
#define MIS_IC1_DMA_TDLR_reg                                                         0xB801BB8C
#define set_MIS_IC1_DMA_TDLR_reg(data)   (*((volatile unsigned int*) MIS_IC1_DMA_TDLR_reg)=data)
#define get_MIS_IC1_DMA_TDLR_reg   (*((volatile unsigned int*) MIS_IC1_DMA_TDLR_reg))
#define MIS_IC1_DMA_TDLR_inst_adr                                                    "0x00E3"
#define MIS_IC1_DMA_TDLR_inst                                                        0x00E3
#define MIS_IC1_DMA_TDLR_dmatdl_shift                                                (0)
#define MIS_IC1_DMA_TDLR_dmatdl_mask                                                 (0x00000007)
#define MIS_IC1_DMA_TDLR_dmatdl(data)                                                (0x00000007&((data)<<0))
#define MIS_IC1_DMA_TDLR_dmatdl_src(data)                                            ((0x00000007&(data))>>0)
#define MIS_IC1_DMA_TDLR_get_dmatdl(data)                                            ((0x00000007&(data))>>0)


#define MIS_IC1_DMA_RDLR                                                             0x1801BB90
#define MIS_IC1_DMA_RDLR_reg_addr                                                    "0xB801BB90"
#define MIS_IC1_DMA_RDLR_reg                                                         0xB801BB90
#define set_MIS_IC1_DMA_RDLR_reg(data)   (*((volatile unsigned int*) MIS_IC1_DMA_RDLR_reg)=data)
#define get_MIS_IC1_DMA_RDLR_reg   (*((volatile unsigned int*) MIS_IC1_DMA_RDLR_reg))
#define MIS_IC1_DMA_RDLR_inst_adr                                                    "0x00E4"
#define MIS_IC1_DMA_RDLR_inst                                                        0x00E4
#define MIS_IC1_DMA_RDLR_dmardl_shift                                                (0)
#define MIS_IC1_DMA_RDLR_dmardl_mask                                                 (0x0000000F)
#define MIS_IC1_DMA_RDLR_dmardl(data)                                                (0x0000000F&((data)<<0))
#define MIS_IC1_DMA_RDLR_dmardl_src(data)                                            ((0x0000000F&(data))>>0)
#define MIS_IC1_DMA_RDLR_get_dmardl(data)                                            ((0x0000000F&(data))>>0)


#define MIS_IC1_COMP_PARAM_1                                                         0x1801BBF4
#define MIS_IC1_COMP_PARAM_1_reg_addr                                                "0xB801BBF4"
#define MIS_IC1_COMP_PARAM_1_reg                                                     0xB801BBF4
#define set_MIS_IC1_COMP_PARAM_1_reg(data)   (*((volatile unsigned int*) MIS_IC1_COMP_PARAM_1_reg)=data)
#define get_MIS_IC1_COMP_PARAM_1_reg   (*((volatile unsigned int*) MIS_IC1_COMP_PARAM_1_reg))
#define MIS_IC1_COMP_PARAM_1_inst_adr                                                "0x00FD"
#define MIS_IC1_COMP_PARAM_1_inst                                                    0x00FD
#define MIS_IC1_COMP_PARAM_1_tx_buffer_depth_shift                                   (16)
#define MIS_IC1_COMP_PARAM_1_tx_buffer_depth_mask                                    (0x00FF0000)
#define MIS_IC1_COMP_PARAM_1_tx_buffer_depth(data)                                   (0x00FF0000&((data)<<16))
#define MIS_IC1_COMP_PARAM_1_tx_buffer_depth_src(data)                               ((0x00FF0000&(data))>>16)
#define MIS_IC1_COMP_PARAM_1_get_tx_buffer_depth(data)                               ((0x00FF0000&(data))>>16)
#define MIS_IC1_COMP_PARAM_1_rx_buffer_depth_shift                                   (8)
#define MIS_IC1_COMP_PARAM_1_rx_buffer_depth_mask                                    (0x0000FF00)
#define MIS_IC1_COMP_PARAM_1_rx_buffer_depth(data)                                   (0x0000FF00&((data)<<8))
#define MIS_IC1_COMP_PARAM_1_rx_buffer_depth_src(data)                               ((0x0000FF00&(data))>>8)
#define MIS_IC1_COMP_PARAM_1_get_rx_buffer_depth(data)                               ((0x0000FF00&(data))>>8)
#define MIS_IC1_COMP_PARAM_1_add_encoded_params_shift                                (7)
#define MIS_IC1_COMP_PARAM_1_add_encoded_params_mask                                 (0x00000080)
#define MIS_IC1_COMP_PARAM_1_add_encoded_params(data)                                (0x00000080&((data)<<7))
#define MIS_IC1_COMP_PARAM_1_add_encoded_params_src(data)                            ((0x00000080&(data))>>7)
#define MIS_IC1_COMP_PARAM_1_get_add_encoded_params(data)                            ((0x00000080&(data))>>7)
#define MIS_IC1_COMP_PARAM_1_has_dma_shift                                           (6)
#define MIS_IC1_COMP_PARAM_1_has_dma_mask                                            (0x00000040)
#define MIS_IC1_COMP_PARAM_1_has_dma(data)                                           (0x00000040&((data)<<6))
#define MIS_IC1_COMP_PARAM_1_has_dma_src(data)                                       ((0x00000040&(data))>>6)
#define MIS_IC1_COMP_PARAM_1_get_has_dma(data)                                       ((0x00000040&(data))>>6)
#define MIS_IC1_COMP_PARAM_1_intr_io_shift                                           (5)
#define MIS_IC1_COMP_PARAM_1_intr_io_mask                                            (0x00000020)
#define MIS_IC1_COMP_PARAM_1_intr_io(data)                                           (0x00000020&((data)<<5))
#define MIS_IC1_COMP_PARAM_1_intr_io_src(data)                                       ((0x00000020&(data))>>5)
#define MIS_IC1_COMP_PARAM_1_get_intr_io(data)                                       ((0x00000020&(data))>>5)
#define MIS_IC1_COMP_PARAM_1_hc_count_values_shift                                   (4)
#define MIS_IC1_COMP_PARAM_1_hc_count_values_mask                                    (0x00000010)
#define MIS_IC1_COMP_PARAM_1_hc_count_values(data)                                   (0x00000010&((data)<<4))
#define MIS_IC1_COMP_PARAM_1_hc_count_values_src(data)                               ((0x00000010&(data))>>4)
#define MIS_IC1_COMP_PARAM_1_get_hc_count_values(data)                               ((0x00000010&(data))>>4)
#define MIS_IC1_COMP_PARAM_1_max_speed_mode_shift                                    (2)
#define MIS_IC1_COMP_PARAM_1_max_speed_mode_mask                                     (0x0000000C)
#define MIS_IC1_COMP_PARAM_1_max_speed_mode(data)                                    (0x0000000C&((data)<<2))
#define MIS_IC1_COMP_PARAM_1_max_speed_mode_src(data)                                ((0x0000000C&(data))>>2)
#define MIS_IC1_COMP_PARAM_1_get_max_speed_mode(data)                                ((0x0000000C&(data))>>2)
#define MIS_IC1_COMP_PARAM_1_apb_data_width_shift                                    (0)
#define MIS_IC1_COMP_PARAM_1_apb_data_width_mask                                     (0x00000003)
#define MIS_IC1_COMP_PARAM_1_apb_data_width(data)                                    (0x00000003&((data)<<0))
#define MIS_IC1_COMP_PARAM_1_apb_data_width_src(data)                                ((0x00000003&(data))>>0)
#define MIS_IC1_COMP_PARAM_1_get_apb_data_width(data)                                ((0x00000003&(data))>>0)


#define MIS_IC1_COMP_VERSION                                                         0x1801BBF8
#define MIS_IC1_COMP_VERSION_reg_addr                                                "0xB801BBF8"
#define MIS_IC1_COMP_VERSION_reg                                                     0xB801BBF8
#define set_MIS_IC1_COMP_VERSION_reg(data)   (*((volatile unsigned int*) MIS_IC1_COMP_VERSION_reg)=data)
#define get_MIS_IC1_COMP_VERSION_reg   (*((volatile unsigned int*) MIS_IC1_COMP_VERSION_reg))
#define MIS_IC1_COMP_VERSION_inst_adr                                                "0x00FE"
#define MIS_IC1_COMP_VERSION_inst                                                    0x00FE
#define MIS_IC1_COMP_VERSION_ic_comp_version_shift                                   (0)
#define MIS_IC1_COMP_VERSION_ic_comp_version_mask                                    (0xFFFFFFFF)
#define MIS_IC1_COMP_VERSION_ic_comp_version(data)                                   (0xFFFFFFFF&((data)<<0))
#define MIS_IC1_COMP_VERSION_ic_comp_version_src(data)                               ((0xFFFFFFFF&(data))>>0)
#define MIS_IC1_COMP_VERSION_get_ic_comp_version(data)                               ((0xFFFFFFFF&(data))>>0)


#define MIS_IC1_COMP_TYPE                                                            0x1801BBFC
#define MIS_IC1_COMP_TYPE_reg_addr                                                   "0xB801BBFC"
#define MIS_IC1_COMP_TYPE_reg                                                        0xB801BBFC
#define set_MIS_IC1_COMP_TYPE_reg(data)   (*((volatile unsigned int*) MIS_IC1_COMP_TYPE_reg)=data)
#define get_MIS_IC1_COMP_TYPE_reg   (*((volatile unsigned int*) MIS_IC1_COMP_TYPE_reg))
#define MIS_IC1_COMP_TYPE_inst_adr                                                   "0x00FF"
#define MIS_IC1_COMP_TYPE_inst                                                       0x00FF
#define MIS_IC1_COMP_TYPE_ic_comp_type_shift                                         (0)
#define MIS_IC1_COMP_TYPE_ic_comp_type_mask                                          (0xFFFFFFFF)
#define MIS_IC1_COMP_TYPE_ic_comp_type(data)                                         (0xFFFFFFFF&((data)<<0))
#define MIS_IC1_COMP_TYPE_ic_comp_type_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define MIS_IC1_COMP_TYPE_get_ic_comp_type(data)                                     ((0xFFFFFFFF&(data))>>0)


#define MIS_PWM_OCD                                                                  0x1801B400
#define MIS_PWM_OCD_reg_addr                                                         "0xB801B400"
#define MIS_PWM_OCD_reg                                                              0xB801B400
#define set_MIS_PWM_OCD_reg(data)   (*((volatile unsigned int*) MIS_PWM_OCD_reg)=data)
#define get_MIS_PWM_OCD_reg   (*((volatile unsigned int*) MIS_PWM_OCD_reg))
#define MIS_PWM_OCD_inst_adr                                                         "0x0000"
#define MIS_PWM_OCD_inst                                                             0x0000
#define MIS_PWM_OCD_3_shift                                                          (24)
#define MIS_PWM_OCD_3_mask                                                           (0xFF000000)
#define MIS_PWM_OCD_3(data)                                                          (0xFF000000&((data)<<24))
#define MIS_PWM_OCD_3_src(data)                                                      ((0xFF000000&(data))>>24)
#define MIS_PWM_OCD_get_3(data)                                                      ((0xFF000000&(data))>>24)
#define MIS_PWM_OCD_2_shift                                                          (16)
#define MIS_PWM_OCD_2_mask                                                           (0x00FF0000)
#define MIS_PWM_OCD_2(data)                                                          (0x00FF0000&((data)<<16))
#define MIS_PWM_OCD_2_src(data)                                                      ((0x00FF0000&(data))>>16)
#define MIS_PWM_OCD_get_2(data)                                                      ((0x00FF0000&(data))>>16)
#define MIS_PWM_OCD_1_shift                                                          (8)
#define MIS_PWM_OCD_1_mask                                                           (0x0000FF00)
#define MIS_PWM_OCD_1(data)                                                          (0x0000FF00&((data)<<8))
#define MIS_PWM_OCD_1_src(data)                                                      ((0x0000FF00&(data))>>8)
#define MIS_PWM_OCD_get_1(data)                                                      ((0x0000FF00&(data))>>8)
#define MIS_PWM_OCD_0_shift                                                          (0)
#define MIS_PWM_OCD_0_mask                                                           (0x000000FF)
#define MIS_PWM_OCD_0(data)                                                          (0x000000FF&((data)<<0))
#define MIS_PWM_OCD_0_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_PWM_OCD_get_0(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_PWM_CD                                                                   0x1801B404
#define MIS_PWM_CD_reg_addr                                                          "0xB801B404"
#define MIS_PWM_CD_reg                                                               0xB801B404
#define set_MIS_PWM_CD_reg(data)   (*((volatile unsigned int*) MIS_PWM_CD_reg)=data)
#define get_MIS_PWM_CD_reg   (*((volatile unsigned int*) MIS_PWM_CD_reg))
#define MIS_PWM_CD_inst_adr                                                          "0x0001"
#define MIS_PWM_CD_inst                                                              0x0001
#define MIS_PWM_CD_3_shift                                                           (24)
#define MIS_PWM_CD_3_mask                                                            (0xFF000000)
#define MIS_PWM_CD_3(data)                                                           (0xFF000000&((data)<<24))
#define MIS_PWM_CD_3_src(data)                                                       ((0xFF000000&(data))>>24)
#define MIS_PWM_CD_get_3(data)                                                       ((0xFF000000&(data))>>24)
#define MIS_PWM_CD_2_shift                                                           (16)
#define MIS_PWM_CD_2_mask                                                            (0x00FF0000)
#define MIS_PWM_CD_2(data)                                                           (0x00FF0000&((data)<<16))
#define MIS_PWM_CD_2_src(data)                                                       ((0x00FF0000&(data))>>16)
#define MIS_PWM_CD_get_2(data)                                                       ((0x00FF0000&(data))>>16)
#define MIS_PWM_CD_1_shift                                                           (8)
#define MIS_PWM_CD_1_mask                                                            (0x0000FF00)
#define MIS_PWM_CD_1(data)                                                           (0x0000FF00&((data)<<8))
#define MIS_PWM_CD_1_src(data)                                                       ((0x0000FF00&(data))>>8)
#define MIS_PWM_CD_get_1(data)                                                       ((0x0000FF00&(data))>>8)
#define MIS_PWM_CD_0_shift                                                           (0)
#define MIS_PWM_CD_0_mask                                                            (0x000000FF)
#define MIS_PWM_CD_0(data)                                                           (0x000000FF&((data)<<0))
#define MIS_PWM_CD_0_src(data)                                                       ((0x000000FF&(data))>>0)
#define MIS_PWM_CD_get_0(data)                                                       ((0x000000FF&(data))>>0)


#define MIS_PWM_CSD                                                                  0x1801B408
#define MIS_PWM_CSD_reg_addr                                                         "0xB801B408"
#define MIS_PWM_CSD_reg                                                              0xB801B408
#define set_MIS_PWM_CSD_reg(data)   (*((volatile unsigned int*) MIS_PWM_CSD_reg)=data)
#define get_MIS_PWM_CSD_reg   (*((volatile unsigned int*) MIS_PWM_CSD_reg))
#define MIS_PWM_CSD_inst_adr                                                         "0x0002"
#define MIS_PWM_CSD_inst                                                             0x0002
#define MIS_PWM_CSD_3_shift                                                          (12)
#define MIS_PWM_CSD_3_mask                                                           (0x0000F000)
#define MIS_PWM_CSD_3(data)                                                          (0x0000F000&((data)<<12))
#define MIS_PWM_CSD_3_src(data)                                                      ((0x0000F000&(data))>>12)
#define MIS_PWM_CSD_get_3(data)                                                      ((0x0000F000&(data))>>12)
#define MIS_PWM_CSD_2_shift                                                          (8)
#define MIS_PWM_CSD_2_mask                                                           (0x00000F00)
#define MIS_PWM_CSD_2(data)                                                          (0x00000F00&((data)<<8))
#define MIS_PWM_CSD_2_src(data)                                                      ((0x00000F00&(data))>>8)
#define MIS_PWM_CSD_get_2(data)                                                      ((0x00000F00&(data))>>8)
#define MIS_PWM_CSD_1_shift                                                          (4)
#define MIS_PWM_CSD_1_mask                                                           (0x000000F0)
#define MIS_PWM_CSD_1(data)                                                          (0x000000F0&((data)<<4))
#define MIS_PWM_CSD_1_src(data)                                                      ((0x000000F0&(data))>>4)
#define MIS_PWM_CSD_get_1(data)                                                      ((0x000000F0&(data))>>4)
#define MIS_PWM_CSD_0_shift                                                          (0)
#define MIS_PWM_CSD_0_mask                                                           (0x0000000F)
#define MIS_PWM_CSD_0(data)                                                          (0x0000000F&((data)<<0))
#define MIS_PWM_CSD_0_src(data)                                                      ((0x0000000F&(data))>>0)
#define MIS_PWM_CSD_get_0(data)                                                      ((0x0000000F&(data))>>0)


#define MIS_TC0TVR                                                                   0x1801B500
#define MIS_TC0TVR_reg_addr                                                          "0xB801B500"
#define MIS_TC0TVR_reg                                                               0xB801B500
#define set_MIS_TC0TVR_reg(data)   (*((volatile unsigned int*) MIS_TC0TVR_reg)=data)
#define get_MIS_TC0TVR_reg   (*((volatile unsigned int*) MIS_TC0TVR_reg))
#define MIS_TC0TVR_inst_adr                                                          "0x0040"
#define MIS_TC0TVR_inst                                                              0x0040
#define MIS_TC0TVR_tc0tvr_shift                                                      (0)
#define MIS_TC0TVR_tc0tvr_mask                                                       (0xFFFFFFFF)
#define MIS_TC0TVR_tc0tvr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define MIS_TC0TVR_tc0tvr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define MIS_TC0TVR_get_tc0tvr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define MIS_TC1TVR                                                                   0x1801B504
#define MIS_TC1TVR_reg_addr                                                          "0xB801B504"
#define MIS_TC1TVR_reg                                                               0xB801B504
#define set_MIS_TC1TVR_reg(data)   (*((volatile unsigned int*) MIS_TC1TVR_reg)=data)
#define get_MIS_TC1TVR_reg   (*((volatile unsigned int*) MIS_TC1TVR_reg))
#define MIS_TC1TVR_inst_adr                                                          "0x0041"
#define MIS_TC1TVR_inst                                                              0x0041
#define MIS_TC1TVR_tc1tvr_shift                                                      (0)
#define MIS_TC1TVR_tc1tvr_mask                                                       (0xFFFFFFFF)
#define MIS_TC1TVR_tc1tvr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define MIS_TC1TVR_tc1tvr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define MIS_TC1TVR_get_tc1tvr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define MIS_TC2TVR                                                                   0x1801B508
#define MIS_TC2TVR_reg_addr                                                          "0xB801B508"
#define MIS_TC2TVR_reg                                                               0xB801B508
#define set_MIS_TC2TVR_reg(data)   (*((volatile unsigned int*) MIS_TC2TVR_reg)=data)
#define get_MIS_TC2TVR_reg   (*((volatile unsigned int*) MIS_TC2TVR_reg))
#define MIS_TC2TVR_inst_adr                                                          "0x0042"
#define MIS_TC2TVR_inst                                                              0x0042
#define MIS_TC2TVR_tc2tvr_shift                                                      (0)
#define MIS_TC2TVR_tc2tvr_mask                                                       (0xFFFFFFFF)
#define MIS_TC2TVR_tc2tvr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define MIS_TC2TVR_tc2tvr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define MIS_TC2TVR_get_tc2tvr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define MIS_TC0CVR                                                                   0x1801B50C
#define MIS_TC0CVR_reg_addr                                                          "0xB801B50C"
#define MIS_TC0CVR_reg                                                               0xB801B50C
#define set_MIS_TC0CVR_reg(data)   (*((volatile unsigned int*) MIS_TC0CVR_reg)=data)
#define get_MIS_TC0CVR_reg   (*((volatile unsigned int*) MIS_TC0CVR_reg))
#define MIS_TC0CVR_inst_adr                                                          "0x0043"
#define MIS_TC0CVR_inst                                                              0x0043
#define MIS_TC0CVR_tc0cvr_shift                                                      (0)
#define MIS_TC0CVR_tc0cvr_mask                                                       (0xFFFFFFFF)
#define MIS_TC0CVR_tc0cvr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define MIS_TC0CVR_tc0cvr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define MIS_TC0CVR_get_tc0cvr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define MIS_TC1CVR                                                                   0x1801B510
#define MIS_TC1CVR_reg_addr                                                          "0xB801B510"
#define MIS_TC1CVR_reg                                                               0xB801B510
#define set_MIS_TC1CVR_reg(data)   (*((volatile unsigned int*) MIS_TC1CVR_reg)=data)
#define get_MIS_TC1CVR_reg   (*((volatile unsigned int*) MIS_TC1CVR_reg))
#define MIS_TC1CVR_inst_adr                                                          "0x0044"
#define MIS_TC1CVR_inst                                                              0x0044
#define MIS_TC1CVR_tc1cvr_shift                                                      (0)
#define MIS_TC1CVR_tc1cvr_mask                                                       (0xFFFFFFFF)
#define MIS_TC1CVR_tc1cvr(data)                                                      (0xFFFFFFFF&((data)<<0))
#define MIS_TC1CVR_tc1cvr_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define MIS_TC1CVR_get_tc1cvr(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define MIS_TC2CVR                                                                   0x1801B514
#define MIS_TC2CVR_reg_addr                                                          "0xB801B514"
#define MIS_TC2CVR_reg                                                               0xB801B514
#define set_MIS_TC2CVR_reg(data)   (*((volatile unsigned int*) MIS_TC2CVR_reg)=data)
#define get_MIS_TC2CVR_reg   (*((volatile unsigned int*) MIS_TC2CVR_reg))
#define MIS_TC2CVR_inst_adr                                                          "0x0045"
#define MIS_TC2CVR_inst                                                              0x0045
#define MIS_TC2CVR_tc2vr_shift                                                       (0)
#define MIS_TC2CVR_tc2vr_mask                                                        (0xFFFFFFFF)
#define MIS_TC2CVR_tc2vr(data)                                                       (0xFFFFFFFF&((data)<<0))
#define MIS_TC2CVR_tc2vr_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define MIS_TC2CVR_get_tc2vr(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define MIS_TC0CR                                                                    0x1801B518
#define MIS_TC0CR_reg_addr                                                           "0xB801B518"
#define MIS_TC0CR_reg                                                                0xB801B518
#define set_MIS_TC0CR_reg(data)   (*((volatile unsigned int*) MIS_TC0CR_reg)=data)
#define get_MIS_TC0CR_reg   (*((volatile unsigned int*) MIS_TC0CR_reg))
#define MIS_TC0CR_inst_adr                                                           "0x0046"
#define MIS_TC0CR_inst                                                               0x0046
#define MIS_TC0CR_tc0en_shift                                                        (31)
#define MIS_TC0CR_tc0en_mask                                                         (0x80000000)
#define MIS_TC0CR_tc0en(data)                                                        (0x80000000&((data)<<31))
#define MIS_TC0CR_tc0en_src(data)                                                    ((0x80000000&(data))>>31)
#define MIS_TC0CR_get_tc0en(data)                                                    ((0x80000000&(data))>>31)
#define MIS_TC0CR_tc0mode_shift                                                      (30)
#define MIS_TC0CR_tc0mode_mask                                                       (0x40000000)
#define MIS_TC0CR_tc0mode(data)                                                      (0x40000000&((data)<<30))
#define MIS_TC0CR_tc0mode_src(data)                                                  ((0x40000000&(data))>>30)
#define MIS_TC0CR_get_tc0mode(data)                                                  ((0x40000000&(data))>>30)
#define MIS_TC0CR_tc0pause_shift                                                     (29)
#define MIS_TC0CR_tc0pause_mask                                                      (0x20000000)
#define MIS_TC0CR_tc0pause(data)                                                     (0x20000000&((data)<<29))
#define MIS_TC0CR_tc0pause_src(data)                                                 ((0x20000000&(data))>>29)
#define MIS_TC0CR_get_tc0pause(data)                                                 ((0x20000000&(data))>>29)
#define MIS_TC0CR_rvda_shift                                                         (24)
#define MIS_TC0CR_rvda_mask                                                          (0x1F000000)
#define MIS_TC0CR_rvda(data)                                                         (0x1F000000&((data)<<24))
#define MIS_TC0CR_rvda_src(data)                                                     ((0x1F000000&(data))>>24)
#define MIS_TC0CR_get_rvda(data)                                                     ((0x1F000000&(data))>>24)


#define MIS_TC1CR                                                                    0x1801B51C
#define MIS_TC1CR_reg_addr                                                           "0xB801B51C"
#define MIS_TC1CR_reg                                                                0xB801B51C
#define set_MIS_TC1CR_reg(data)   (*((volatile unsigned int*) MIS_TC1CR_reg)=data)
#define get_MIS_TC1CR_reg   (*((volatile unsigned int*) MIS_TC1CR_reg))
#define MIS_TC1CR_inst_adr                                                           "0x0047"
#define MIS_TC1CR_inst                                                               0x0047
#define MIS_TC1CR_tc1en_shift                                                        (31)
#define MIS_TC1CR_tc1en_mask                                                         (0x80000000)
#define MIS_TC1CR_tc1en(data)                                                        (0x80000000&((data)<<31))
#define MIS_TC1CR_tc1en_src(data)                                                    ((0x80000000&(data))>>31)
#define MIS_TC1CR_get_tc1en(data)                                                    ((0x80000000&(data))>>31)
#define MIS_TC1CR_tc1mode_shift                                                      (30)
#define MIS_TC1CR_tc1mode_mask                                                       (0x40000000)
#define MIS_TC1CR_tc1mode(data)                                                      (0x40000000&((data)<<30))
#define MIS_TC1CR_tc1mode_src(data)                                                  ((0x40000000&(data))>>30)
#define MIS_TC1CR_get_tc1mode(data)                                                  ((0x40000000&(data))>>30)
#define MIS_TC1CR_tc1pause_shift                                                     (29)
#define MIS_TC1CR_tc1pause_mask                                                      (0x20000000)
#define MIS_TC1CR_tc1pause(data)                                                     (0x20000000&((data)<<29))
#define MIS_TC1CR_tc1pause_src(data)                                                 ((0x20000000&(data))>>29)
#define MIS_TC1CR_get_tc1pause(data)                                                 ((0x20000000&(data))>>29)
#define MIS_TC1CR_rvda_shift                                                         (24)
#define MIS_TC1CR_rvda_mask                                                          (0x1F000000)
#define MIS_TC1CR_rvda(data)                                                         (0x1F000000&((data)<<24))
#define MIS_TC1CR_rvda_src(data)                                                     ((0x1F000000&(data))>>24)
#define MIS_TC1CR_get_rvda(data)                                                     ((0x1F000000&(data))>>24)


#define MIS_TC2CR                                                                    0x1801B520
#define MIS_TC2CR_reg_addr                                                           "0xB801B520"
#define MIS_TC2CR_reg                                                                0xB801B520
#define set_MIS_TC2CR_reg(data)   (*((volatile unsigned int*) MIS_TC2CR_reg)=data)
#define get_MIS_TC2CR_reg   (*((volatile unsigned int*) MIS_TC2CR_reg))
#define MIS_TC2CR_inst_adr                                                           "0x0048"
#define MIS_TC2CR_inst                                                               0x0048
#define MIS_TC2CR_tc2en_shift                                                        (31)
#define MIS_TC2CR_tc2en_mask                                                         (0x80000000)
#define MIS_TC2CR_tc2en(data)                                                        (0x80000000&((data)<<31))
#define MIS_TC2CR_tc2en_src(data)                                                    ((0x80000000&(data))>>31)
#define MIS_TC2CR_get_tc2en(data)                                                    ((0x80000000&(data))>>31)
#define MIS_TC2CR_tc2mode_shift                                                      (30)
#define MIS_TC2CR_tc2mode_mask                                                       (0x40000000)
#define MIS_TC2CR_tc2mode(data)                                                      (0x40000000&((data)<<30))
#define MIS_TC2CR_tc2mode_src(data)                                                  ((0x40000000&(data))>>30)
#define MIS_TC2CR_get_tc2mode(data)                                                  ((0x40000000&(data))>>30)
#define MIS_TC2CR_tc2pause_shift                                                     (29)
#define MIS_TC2CR_tc2pause_mask                                                      (0x20000000)
#define MIS_TC2CR_tc2pause(data)                                                     (0x20000000&((data)<<29))
#define MIS_TC2CR_tc2pause_src(data)                                                 ((0x20000000&(data))>>29)
#define MIS_TC2CR_get_tc2pause(data)                                                 ((0x20000000&(data))>>29)
#define MIS_TC2CR_rvda_shift                                                         (24)
#define MIS_TC2CR_rvda_mask                                                          (0x1F000000)
#define MIS_TC2CR_rvda(data)                                                         (0x1F000000&((data)<<24))
#define MIS_TC2CR_rvda_src(data)                                                     ((0x1F000000&(data))>>24)
#define MIS_TC2CR_get_rvda(data)                                                     ((0x1F000000&(data))>>24)


#define MIS_TC0ICR                                                                   0x1801B524
#define MIS_TC0ICR_reg_addr                                                          "0xB801B524"
#define MIS_TC0ICR_reg                                                               0xB801B524
#define set_MIS_TC0ICR_reg(data)   (*((volatile unsigned int*) MIS_TC0ICR_reg)=data)
#define get_MIS_TC0ICR_reg   (*((volatile unsigned int*) MIS_TC0ICR_reg))
#define MIS_TC0ICR_inst_adr                                                          "0x0049"
#define MIS_TC0ICR_inst                                                              0x0049
#define MIS_TC0ICR_tc0ie_shift                                                       (31)
#define MIS_TC0ICR_tc0ie_mask                                                        (0x80000000)
#define MIS_TC0ICR_tc0ie(data)                                                       (0x80000000&((data)<<31))
#define MIS_TC0ICR_tc0ie_src(data)                                                   ((0x80000000&(data))>>31)
#define MIS_TC0ICR_get_tc0ie(data)                                                   ((0x80000000&(data))>>31)


#define MIS_TC1ICR                                                                   0x1801B528
#define MIS_TC1ICR_reg_addr                                                          "0xB801B528"
#define MIS_TC1ICR_reg                                                               0xB801B528
#define set_MIS_TC1ICR_reg(data)   (*((volatile unsigned int*) MIS_TC1ICR_reg)=data)
#define get_MIS_TC1ICR_reg   (*((volatile unsigned int*) MIS_TC1ICR_reg))
#define MIS_TC1ICR_inst_adr                                                          "0x004A"
#define MIS_TC1ICR_inst                                                              0x004A
#define MIS_TC1ICR_tc1ie_shift                                                       (31)
#define MIS_TC1ICR_tc1ie_mask                                                        (0x80000000)
#define MIS_TC1ICR_tc1ie(data)                                                       (0x80000000&((data)<<31))
#define MIS_TC1ICR_tc1ie_src(data)                                                   ((0x80000000&(data))>>31)
#define MIS_TC1ICR_get_tc1ie(data)                                                   ((0x80000000&(data))>>31)


#define MIS_TC2ICR                                                                   0x1801B52C
#define MIS_TC2ICR_reg_addr                                                          "0xB801B52C"
#define MIS_TC2ICR_reg                                                               0xB801B52C
#define set_MIS_TC2ICR_reg(data)   (*((volatile unsigned int*) MIS_TC2ICR_reg)=data)
#define get_MIS_TC2ICR_reg   (*((volatile unsigned int*) MIS_TC2ICR_reg))
#define MIS_TC2ICR_inst_adr                                                          "0x004B"
#define MIS_TC2ICR_inst                                                              0x004B
#define MIS_TC2ICR_tc2ie_shift                                                       (31)
#define MIS_TC2ICR_tc2ie_mask                                                        (0x80000000)
#define MIS_TC2ICR_tc2ie(data)                                                       (0x80000000&((data)<<31))
#define MIS_TC2ICR_tc2ie_src(data)                                                   ((0x80000000&(data))>>31)
#define MIS_TC2ICR_get_tc2ie(data)                                                   ((0x80000000&(data))>>31)


#define MIS_CLK90K_CTRL                                                              0x1801B538
#define MIS_CLK90K_CTRL_reg_addr                                                     "0xB801B538"
#define MIS_CLK90K_CTRL_reg                                                          0xB801B538
#define set_MIS_CLK90K_CTRL_reg(data)   (*((volatile unsigned int*) MIS_CLK90K_CTRL_reg)=data)
#define get_MIS_CLK90K_CTRL_reg   (*((volatile unsigned int*) MIS_CLK90K_CTRL_reg))
#define MIS_CLK90K_CTRL_inst_adr                                                     "0x004E"
#define MIS_CLK90K_CTRL_inst                                                         0x004E
#define MIS_CLK90K_CTRL_en_shift                                                     (0)
#define MIS_CLK90K_CTRL_en_mask                                                      (0x00000001)
#define MIS_CLK90K_CTRL_en(data)                                                     (0x00000001&((data)<<0))
#define MIS_CLK90K_CTRL_en_src(data)                                                 ((0x00000001&(data))>>0)
#define MIS_CLK90K_CTRL_get_en(data)                                                 ((0x00000001&(data))>>0)


#define MIS_SCPU_CLK27M_90K                                                          0x1801B53C
#define MIS_SCPU_CLK27M_90K_reg_addr                                                 "0xB801B53C"
#define MIS_SCPU_CLK27M_90K_reg                                                      0xB801B53C
#define set_MIS_SCPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_SCPU_CLK27M_90K_reg)=data)
#define get_MIS_SCPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_SCPU_CLK27M_90K_reg))
#define MIS_SCPU_CLK27M_90K_inst_adr                                                 "0x004F"
#define MIS_SCPU_CLK27M_90K_inst                                                     0x004F
#define MIS_SCPU_CLK27M_90K_cnt_shift                                                (0)
#define MIS_SCPU_CLK27M_90K_cnt_mask                                                 (0x000001FF)
#define MIS_SCPU_CLK27M_90K_cnt(data)                                                (0x000001FF&((data)<<0))
#define MIS_SCPU_CLK27M_90K_cnt_src(data)                                            ((0x000001FF&(data))>>0)
#define MIS_SCPU_CLK27M_90K_get_cnt(data)                                            ((0x000001FF&(data))>>0)


#define MIS_SCPU_CLK90K_LO                                                           0x1801B540
#define MIS_SCPU_CLK90K_LO_reg_addr                                                  "0xB801B540"
#define MIS_SCPU_CLK90K_LO_reg                                                       0xB801B540
#define set_MIS_SCPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_SCPU_CLK90K_LO_reg)=data)
#define get_MIS_SCPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_SCPU_CLK90K_LO_reg))
#define MIS_SCPU_CLK90K_LO_inst_adr                                                  "0x0050"
#define MIS_SCPU_CLK90K_LO_inst                                                      0x0050
#define MIS_SCPU_CLK90K_LO_val_shift                                                 (0)
#define MIS_SCPU_CLK90K_LO_val_mask                                                  (0xFFFFFFFF)
#define MIS_SCPU_CLK90K_LO_val(data)                                                 (0xFFFFFFFF&((data)<<0))
#define MIS_SCPU_CLK90K_LO_val_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define MIS_SCPU_CLK90K_LO_get_val(data)                                             ((0xFFFFFFFF&(data))>>0)


#define MIS_SCPU_CLK90K_HI                                                           0x1801B544
#define MIS_SCPU_CLK90K_HI_reg_addr                                                  "0xB801B544"
#define MIS_SCPU_CLK90K_HI_reg                                                       0xB801B544
#define set_MIS_SCPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_SCPU_CLK90K_HI_reg)=data)
#define get_MIS_SCPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_SCPU_CLK90K_HI_reg))
#define MIS_SCPU_CLK90K_HI_inst_adr                                                  "0x0051"
#define MIS_SCPU_CLK90K_HI_inst                                                      0x0051
#define MIS_SCPU_CLK90K_HI_val_shift                                                 (0)
#define MIS_SCPU_CLK90K_HI_val_mask                                                  (0x0000FFFF)
#define MIS_SCPU_CLK90K_HI_val(data)                                                 (0x0000FFFF&((data)<<0))
#define MIS_SCPU_CLK90K_HI_val_src(data)                                             ((0x0000FFFF&(data))>>0)
#define MIS_SCPU_CLK90K_HI_get_val(data)                                             ((0x0000FFFF&(data))>>0)


#define MIS_ACPU_CLK27M_90K                                                          0x1801B548
#define MIS_ACPU_CLK27M_90K_reg_addr                                                 "0xB801B548"
#define MIS_ACPU_CLK27M_90K_reg                                                      0xB801B548
#define set_MIS_ACPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_ACPU_CLK27M_90K_reg)=data)
#define get_MIS_ACPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_ACPU_CLK27M_90K_reg))
#define MIS_ACPU_CLK27M_90K_inst_adr                                                 "0x0052"
#define MIS_ACPU_CLK27M_90K_inst                                                     0x0052
#define MIS_ACPU_CLK27M_90K_cnt_shift                                                (0)
#define MIS_ACPU_CLK27M_90K_cnt_mask                                                 (0x000001FF)
#define MIS_ACPU_CLK27M_90K_cnt(data)                                                (0x000001FF&((data)<<0))
#define MIS_ACPU_CLK27M_90K_cnt_src(data)                                            ((0x000001FF&(data))>>0)
#define MIS_ACPU_CLK27M_90K_get_cnt(data)                                            ((0x000001FF&(data))>>0)


#define MIS_ACPU_CLK90K_LO                                                           0x1801B54C
#define MIS_ACPU_CLK90K_LO_reg_addr                                                  "0xB801B54C"
#define MIS_ACPU_CLK90K_LO_reg                                                       0xB801B54C
#define set_MIS_ACPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_ACPU_CLK90K_LO_reg)=data)
#define get_MIS_ACPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_ACPU_CLK90K_LO_reg))
#define MIS_ACPU_CLK90K_LO_inst_adr                                                  "0x0053"
#define MIS_ACPU_CLK90K_LO_inst                                                      0x0053
#define MIS_ACPU_CLK90K_LO_val_shift                                                 (0)
#define MIS_ACPU_CLK90K_LO_val_mask                                                  (0xFFFFFFFF)
#define MIS_ACPU_CLK90K_LO_val(data)                                                 (0xFFFFFFFF&((data)<<0))
#define MIS_ACPU_CLK90K_LO_val_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define MIS_ACPU_CLK90K_LO_get_val(data)                                             ((0xFFFFFFFF&(data))>>0)


#define MIS_ACPU_CLK90K_HI                                                           0x1801B550
#define MIS_ACPU_CLK90K_HI_reg_addr                                                  "0xB801B550"
#define MIS_ACPU_CLK90K_HI_reg                                                       0xB801B550
#define set_MIS_ACPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_ACPU_CLK90K_HI_reg)=data)
#define get_MIS_ACPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_ACPU_CLK90K_HI_reg))
#define MIS_ACPU_CLK90K_HI_inst_adr                                                  "0x0054"
#define MIS_ACPU_CLK90K_HI_inst                                                      0x0054
#define MIS_ACPU_CLK90K_HI_val_shift                                                 (0)
#define MIS_ACPU_CLK90K_HI_val_mask                                                  (0x0000FFFF)
#define MIS_ACPU_CLK90K_HI_val(data)                                                 (0x0000FFFF&((data)<<0))
#define MIS_ACPU_CLK90K_HI_val_src(data)                                             ((0x0000FFFF&(data))>>0)
#define MIS_ACPU_CLK90K_HI_get_val(data)                                             ((0x0000FFFF&(data))>>0)


#define MIS_VCPU_CLK27M_90K                                                          0x1801B554
#define MIS_VCPU_CLK27M_90K_reg_addr                                                 "0xB801B554"
#define MIS_VCPU_CLK27M_90K_reg                                                      0xB801B554
#define set_MIS_VCPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_VCPU_CLK27M_90K_reg)=data)
#define get_MIS_VCPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_VCPU_CLK27M_90K_reg))
#define MIS_VCPU_CLK27M_90K_inst_adr                                                 "0x0055"
#define MIS_VCPU_CLK27M_90K_inst                                                     0x0055
#define MIS_VCPU_CLK27M_90K_cnt_shift                                                (0)
#define MIS_VCPU_CLK27M_90K_cnt_mask                                                 (0x000001FF)
#define MIS_VCPU_CLK27M_90K_cnt(data)                                                (0x000001FF&((data)<<0))
#define MIS_VCPU_CLK27M_90K_cnt_src(data)                                            ((0x000001FF&(data))>>0)
#define MIS_VCPU_CLK27M_90K_get_cnt(data)                                            ((0x000001FF&(data))>>0)


#define MIS_VCPU_CLK90K_LO                                                           0x1801B558
#define MIS_VCPU_CLK90K_LO_reg_addr                                                  "0xB801B558"
#define MIS_VCPU_CLK90K_LO_reg                                                       0xB801B558
#define set_MIS_VCPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_VCPU_CLK90K_LO_reg)=data)
#define get_MIS_VCPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_VCPU_CLK90K_LO_reg))
#define MIS_VCPU_CLK90K_LO_inst_adr                                                  "0x0056"
#define MIS_VCPU_CLK90K_LO_inst                                                      0x0056
#define MIS_VCPU_CLK90K_LO_val_shift                                                 (0)
#define MIS_VCPU_CLK90K_LO_val_mask                                                  (0xFFFFFFFF)
#define MIS_VCPU_CLK90K_LO_val(data)                                                 (0xFFFFFFFF&((data)<<0))
#define MIS_VCPU_CLK90K_LO_val_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define MIS_VCPU_CLK90K_LO_get_val(data)                                             ((0xFFFFFFFF&(data))>>0)


#define MIS_VCPU_CLK90K_HI                                                           0x1801B55C
#define MIS_VCPU_CLK90K_HI_reg_addr                                                  "0xB801B55C"
#define MIS_VCPU_CLK90K_HI_reg                                                       0xB801B55C
#define set_MIS_VCPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_VCPU_CLK90K_HI_reg)=data)
#define get_MIS_VCPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_VCPU_CLK90K_HI_reg))
#define MIS_VCPU_CLK90K_HI_inst_adr                                                  "0x0057"
#define MIS_VCPU_CLK90K_HI_inst                                                      0x0057
#define MIS_VCPU_CLK90K_HI_val_shift                                                 (0)
#define MIS_VCPU_CLK90K_HI_val_mask                                                  (0x0000FFFF)
#define MIS_VCPU_CLK90K_HI_val(data)                                                 (0x0000FFFF&((data)<<0))
#define MIS_VCPU_CLK90K_HI_val_src(data)                                             ((0x0000FFFF&(data))>>0)
#define MIS_VCPU_CLK90K_HI_get_val(data)                                             ((0x0000FFFF&(data))>>0)


#define MIS_KCPU_CLK27M_90K                                                          0x1801B560
#define MIS_KCPU_CLK27M_90K_reg_addr                                                 "0xB801B560"
#define MIS_KCPU_CLK27M_90K_reg                                                      0xB801B560
#define set_MIS_KCPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_KCPU_CLK27M_90K_reg)=data)
#define get_MIS_KCPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_KCPU_CLK27M_90K_reg))
#define MIS_KCPU_CLK27M_90K_inst_adr                                                 "0x0058"
#define MIS_KCPU_CLK27M_90K_inst                                                     0x0058
#define MIS_KCPU_CLK27M_90K_cnt_shift                                                (0)
#define MIS_KCPU_CLK27M_90K_cnt_mask                                                 (0x000001FF)
#define MIS_KCPU_CLK27M_90K_cnt(data)                                                (0x000001FF&((data)<<0))
#define MIS_KCPU_CLK27M_90K_cnt_src(data)                                            ((0x000001FF&(data))>>0)
#define MIS_KCPU_CLK27M_90K_get_cnt(data)                                            ((0x000001FF&(data))>>0)


#define MIS_KCPU_CLK90K_LO                                                           0x1801B564
#define MIS_KCPU_CLK90K_LO_reg_addr                                                  "0xB801B564"
#define MIS_KCPU_CLK90K_LO_reg                                                       0xB801B564
#define set_MIS_KCPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_KCPU_CLK90K_LO_reg)=data)
#define get_MIS_KCPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_KCPU_CLK90K_LO_reg))
#define MIS_KCPU_CLK90K_LO_inst_adr                                                  "0x0059"
#define MIS_KCPU_CLK90K_LO_inst                                                      0x0059
#define MIS_KCPU_CLK90K_LO_val_shift                                                 (0)
#define MIS_KCPU_CLK90K_LO_val_mask                                                  (0xFFFFFFFF)
#define MIS_KCPU_CLK90K_LO_val(data)                                                 (0xFFFFFFFF&((data)<<0))
#define MIS_KCPU_CLK90K_LO_val_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define MIS_KCPU_CLK90K_LO_get_val(data)                                             ((0xFFFFFFFF&(data))>>0)


#define MIS_KCPU_CLK90K_HI                                                           0x1801B568
#define MIS_KCPU_CLK90K_HI_reg_addr                                                  "0xB801B568"
#define MIS_KCPU_CLK90K_HI_reg                                                       0xB801B568
#define set_MIS_KCPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_KCPU_CLK90K_HI_reg)=data)
#define get_MIS_KCPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_KCPU_CLK90K_HI_reg))
#define MIS_KCPU_CLK90K_HI_inst_adr                                                  "0x005A"
#define MIS_KCPU_CLK90K_HI_inst                                                      0x005A
#define MIS_KCPU_CLK90K_HI_val_shift                                                 (0)
#define MIS_KCPU_CLK90K_HI_val_mask                                                  (0x0000FFFF)
#define MIS_KCPU_CLK90K_HI_val(data)                                                 (0x0000FFFF&((data)<<0))
#define MIS_KCPU_CLK90K_HI_val_src(data)                                             ((0x0000FFFF&(data))>>0)
#define MIS_KCPU_CLK90K_HI_get_val(data)                                             ((0x0000FFFF&(data))>>0)


#define MIS_PCR_CLK90K_CTRL                                                          0x1801B570
#define MIS_PCR_CLK90K_CTRL_reg_addr                                                 "0xB801B570"
#define MIS_PCR_CLK90K_CTRL_reg                                                      0xB801B570
#define set_MIS_PCR_CLK90K_CTRL_reg(data)   (*((volatile unsigned int*) MIS_PCR_CLK90K_CTRL_reg)=data)
#define get_MIS_PCR_CLK90K_CTRL_reg   (*((volatile unsigned int*) MIS_PCR_CLK90K_CTRL_reg))
#define MIS_PCR_CLK90K_CTRL_inst_adr                                                 "0x005C"
#define MIS_PCR_CLK90K_CTRL_inst                                                     0x005C
#define MIS_PCR_CLK90K_CTRL_en_shift                                                 (0)
#define MIS_PCR_CLK90K_CTRL_en_mask                                                  (0x00000001)
#define MIS_PCR_CLK90K_CTRL_en(data)                                                 (0x00000001&((data)<<0))
#define MIS_PCR_CLK90K_CTRL_en_src(data)                                             ((0x00000001&(data))>>0)
#define MIS_PCR_CLK90K_CTRL_get_en(data)                                             ((0x00000001&(data))>>0)


#define MIS_PCR_SCPU_CLK27M_90K                                                      0x1801B574
#define MIS_PCR_SCPU_CLK27M_90K_reg_addr                                             "0xB801B574"
#define MIS_PCR_SCPU_CLK27M_90K_reg                                                  0xB801B574
#define set_MIS_PCR_SCPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_PCR_SCPU_CLK27M_90K_reg)=data)
#define get_MIS_PCR_SCPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_PCR_SCPU_CLK27M_90K_reg))
#define MIS_PCR_SCPU_CLK27M_90K_inst_adr                                             "0x005D"
#define MIS_PCR_SCPU_CLK27M_90K_inst                                                 0x005D
#define MIS_PCR_SCPU_CLK27M_90K_cnt_shift                                            (0)
#define MIS_PCR_SCPU_CLK27M_90K_cnt_mask                                             (0x000001FF)
#define MIS_PCR_SCPU_CLK27M_90K_cnt(data)                                            (0x000001FF&((data)<<0))
#define MIS_PCR_SCPU_CLK27M_90K_cnt_src(data)                                        ((0x000001FF&(data))>>0)
#define MIS_PCR_SCPU_CLK27M_90K_get_cnt(data)                                        ((0x000001FF&(data))>>0)


#define MIS_PCR_SCPU_CLK90K_LO                                                       0x1801B578
#define MIS_PCR_SCPU_CLK90K_LO_reg_addr                                              "0xB801B578"
#define MIS_PCR_SCPU_CLK90K_LO_reg                                                   0xB801B578
#define set_MIS_PCR_SCPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_PCR_SCPU_CLK90K_LO_reg)=data)
#define get_MIS_PCR_SCPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_PCR_SCPU_CLK90K_LO_reg))
#define MIS_PCR_SCPU_CLK90K_LO_inst_adr                                              "0x005E"
#define MIS_PCR_SCPU_CLK90K_LO_inst                                                  0x005E
#define MIS_PCR_SCPU_CLK90K_LO_val_shift                                             (0)
#define MIS_PCR_SCPU_CLK90K_LO_val_mask                                              (0xFFFFFFFF)
#define MIS_PCR_SCPU_CLK90K_LO_val(data)                                             (0xFFFFFFFF&((data)<<0))
#define MIS_PCR_SCPU_CLK90K_LO_val_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define MIS_PCR_SCPU_CLK90K_LO_get_val(data)                                         ((0xFFFFFFFF&(data))>>0)


#define MIS_PCR_SCPU_CLK90K_HI                                                       0x1801B57C
#define MIS_PCR_SCPU_CLK90K_HI_reg_addr                                              "0xB801B57C"
#define MIS_PCR_SCPU_CLK90K_HI_reg                                                   0xB801B57C
#define set_MIS_PCR_SCPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_PCR_SCPU_CLK90K_HI_reg)=data)
#define get_MIS_PCR_SCPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_PCR_SCPU_CLK90K_HI_reg))
#define MIS_PCR_SCPU_CLK90K_HI_inst_adr                                              "0x005F"
#define MIS_PCR_SCPU_CLK90K_HI_inst                                                  0x005F
#define MIS_PCR_SCPU_CLK90K_HI_val_shift                                             (0)
#define MIS_PCR_SCPU_CLK90K_HI_val_mask                                              (0x0000FFFF)
#define MIS_PCR_SCPU_CLK90K_HI_val(data)                                             (0x0000FFFF&((data)<<0))
#define MIS_PCR_SCPU_CLK90K_HI_val_src(data)                                         ((0x0000FFFF&(data))>>0)
#define MIS_PCR_SCPU_CLK90K_HI_get_val(data)                                         ((0x0000FFFF&(data))>>0)


#define MIS_PCR_ACPU_CLK27M_90K                                                      0x1801B580
#define MIS_PCR_ACPU_CLK27M_90K_reg_addr                                             "0xB801B580"
#define MIS_PCR_ACPU_CLK27M_90K_reg                                                  0xB801B580
#define set_MIS_PCR_ACPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_PCR_ACPU_CLK27M_90K_reg)=data)
#define get_MIS_PCR_ACPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_PCR_ACPU_CLK27M_90K_reg))
#define MIS_PCR_ACPU_CLK27M_90K_inst_adr                                             "0x0060"
#define MIS_PCR_ACPU_CLK27M_90K_inst                                                 0x0060
#define MIS_PCR_ACPU_CLK27M_90K_cnt_shift                                            (0)
#define MIS_PCR_ACPU_CLK27M_90K_cnt_mask                                             (0x000001FF)
#define MIS_PCR_ACPU_CLK27M_90K_cnt(data)                                            (0x000001FF&((data)<<0))
#define MIS_PCR_ACPU_CLK27M_90K_cnt_src(data)                                        ((0x000001FF&(data))>>0)
#define MIS_PCR_ACPU_CLK27M_90K_get_cnt(data)                                        ((0x000001FF&(data))>>0)


#define MIS_PCR_ACPU_CLK90K_LO                                                       0x1801B584
#define MIS_PCR_ACPU_CLK90K_LO_reg_addr                                              "0xB801B584"
#define MIS_PCR_ACPU_CLK90K_LO_reg                                                   0xB801B584
#define set_MIS_PCR_ACPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_PCR_ACPU_CLK90K_LO_reg)=data)
#define get_MIS_PCR_ACPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_PCR_ACPU_CLK90K_LO_reg))
#define MIS_PCR_ACPU_CLK90K_LO_inst_adr                                              "0x0061"
#define MIS_PCR_ACPU_CLK90K_LO_inst                                                  0x0061
#define MIS_PCR_ACPU_CLK90K_LO_val_shift                                             (0)
#define MIS_PCR_ACPU_CLK90K_LO_val_mask                                              (0xFFFFFFFF)
#define MIS_PCR_ACPU_CLK90K_LO_val(data)                                             (0xFFFFFFFF&((data)<<0))
#define MIS_PCR_ACPU_CLK90K_LO_val_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define MIS_PCR_ACPU_CLK90K_LO_get_val(data)                                         ((0xFFFFFFFF&(data))>>0)


#define MIS_PCR_ACPU_CLK90K_HI                                                       0x1801B588
#define MIS_PCR_ACPU_CLK90K_HI_reg_addr                                              "0xB801B588"
#define MIS_PCR_ACPU_CLK90K_HI_reg                                                   0xB801B588
#define set_MIS_PCR_ACPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_PCR_ACPU_CLK90K_HI_reg)=data)
#define get_MIS_PCR_ACPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_PCR_ACPU_CLK90K_HI_reg))
#define MIS_PCR_ACPU_CLK90K_HI_inst_adr                                              "0x0062"
#define MIS_PCR_ACPU_CLK90K_HI_inst                                                  0x0062
#define MIS_PCR_ACPU_CLK90K_HI_val_shift                                             (0)
#define MIS_PCR_ACPU_CLK90K_HI_val_mask                                              (0x0000FFFF)
#define MIS_PCR_ACPU_CLK90K_HI_val(data)                                             (0x0000FFFF&((data)<<0))
#define MIS_PCR_ACPU_CLK90K_HI_val_src(data)                                         ((0x0000FFFF&(data))>>0)
#define MIS_PCR_ACPU_CLK90K_HI_get_val(data)                                         ((0x0000FFFF&(data))>>0)


#define MIS_PCR_VCPU_CLK27M_90K                                                      0x1801B58C
#define MIS_PCR_VCPU_CLK27M_90K_reg_addr                                             "0xB801B58C"
#define MIS_PCR_VCPU_CLK27M_90K_reg                                                  0xB801B58C
#define set_MIS_PCR_VCPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_PCR_VCPU_CLK27M_90K_reg)=data)
#define get_MIS_PCR_VCPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_PCR_VCPU_CLK27M_90K_reg))
#define MIS_PCR_VCPU_CLK27M_90K_inst_adr                                             "0x0063"
#define MIS_PCR_VCPU_CLK27M_90K_inst                                                 0x0063
#define MIS_PCR_VCPU_CLK27M_90K_cnt_shift                                            (0)
#define MIS_PCR_VCPU_CLK27M_90K_cnt_mask                                             (0x000001FF)
#define MIS_PCR_VCPU_CLK27M_90K_cnt(data)                                            (0x000001FF&((data)<<0))
#define MIS_PCR_VCPU_CLK27M_90K_cnt_src(data)                                        ((0x000001FF&(data))>>0)
#define MIS_PCR_VCPU_CLK27M_90K_get_cnt(data)                                        ((0x000001FF&(data))>>0)


#define MIS_PCR_VCPU_CLK90K_LO                                                       0x1801B590
#define MIS_PCR_VCPU_CLK90K_LO_reg_addr                                              "0xB801B590"
#define MIS_PCR_VCPU_CLK90K_LO_reg                                                   0xB801B590
#define set_MIS_PCR_VCPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_PCR_VCPU_CLK90K_LO_reg)=data)
#define get_MIS_PCR_VCPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_PCR_VCPU_CLK90K_LO_reg))
#define MIS_PCR_VCPU_CLK90K_LO_inst_adr                                              "0x0064"
#define MIS_PCR_VCPU_CLK90K_LO_inst                                                  0x0064
#define MIS_PCR_VCPU_CLK90K_LO_val_shift                                             (0)
#define MIS_PCR_VCPU_CLK90K_LO_val_mask                                              (0xFFFFFFFF)
#define MIS_PCR_VCPU_CLK90K_LO_val(data)                                             (0xFFFFFFFF&((data)<<0))
#define MIS_PCR_VCPU_CLK90K_LO_val_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define MIS_PCR_VCPU_CLK90K_LO_get_val(data)                                         ((0xFFFFFFFF&(data))>>0)


#define MIS_PCR_VCPU_CLK90K_HI                                                       0x1801B594
#define MIS_PCR_VCPU_CLK90K_HI_reg_addr                                              "0xB801B594"
#define MIS_PCR_VCPU_CLK90K_HI_reg                                                   0xB801B594
#define set_MIS_PCR_VCPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_PCR_VCPU_CLK90K_HI_reg)=data)
#define get_MIS_PCR_VCPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_PCR_VCPU_CLK90K_HI_reg))
#define MIS_PCR_VCPU_CLK90K_HI_inst_adr                                              "0x0065"
#define MIS_PCR_VCPU_CLK90K_HI_inst                                                  0x0065
#define MIS_PCR_VCPU_CLK90K_HI_val_shift                                             (0)
#define MIS_PCR_VCPU_CLK90K_HI_val_mask                                              (0x0000FFFF)
#define MIS_PCR_VCPU_CLK90K_HI_val(data)                                             (0x0000FFFF&((data)<<0))
#define MIS_PCR_VCPU_CLK90K_HI_val_src(data)                                         ((0x0000FFFF&(data))>>0)
#define MIS_PCR_VCPU_CLK90K_HI_get_val(data)                                         ((0x0000FFFF&(data))>>0)


#define MIS_PCR_KCPU_CLK27M_90K                                                      0x1801B598
#define MIS_PCR_KCPU_CLK27M_90K_reg_addr                                             "0xB801B598"
#define MIS_PCR_KCPU_CLK27M_90K_reg                                                  0xB801B598
#define set_MIS_PCR_KCPU_CLK27M_90K_reg(data)   (*((volatile unsigned int*) MIS_PCR_KCPU_CLK27M_90K_reg)=data)
#define get_MIS_PCR_KCPU_CLK27M_90K_reg   (*((volatile unsigned int*) MIS_PCR_KCPU_CLK27M_90K_reg))
#define MIS_PCR_KCPU_CLK27M_90K_inst_adr                                             "0x0066"
#define MIS_PCR_KCPU_CLK27M_90K_inst                                                 0x0066
#define MIS_PCR_KCPU_CLK27M_90K_cnt_shift                                            (0)
#define MIS_PCR_KCPU_CLK27M_90K_cnt_mask                                             (0x000001FF)
#define MIS_PCR_KCPU_CLK27M_90K_cnt(data)                                            (0x000001FF&((data)<<0))
#define MIS_PCR_KCPU_CLK27M_90K_cnt_src(data)                                        ((0x000001FF&(data))>>0)
#define MIS_PCR_KCPU_CLK27M_90K_get_cnt(data)                                        ((0x000001FF&(data))>>0)


#define MIS_PCR_KCPU_CLK90K_LO                                                       0x1801B59C
#define MIS_PCR_KCPU_CLK90K_LO_reg_addr                                              "0xB801B59C"
#define MIS_PCR_KCPU_CLK90K_LO_reg                                                   0xB801B59C
#define set_MIS_PCR_KCPU_CLK90K_LO_reg(data)   (*((volatile unsigned int*) MIS_PCR_KCPU_CLK90K_LO_reg)=data)
#define get_MIS_PCR_KCPU_CLK90K_LO_reg   (*((volatile unsigned int*) MIS_PCR_KCPU_CLK90K_LO_reg))
#define MIS_PCR_KCPU_CLK90K_LO_inst_adr                                              "0x0067"
#define MIS_PCR_KCPU_CLK90K_LO_inst                                                  0x0067
#define MIS_PCR_KCPU_CLK90K_LO_val_shift                                             (0)
#define MIS_PCR_KCPU_CLK90K_LO_val_mask                                              (0xFFFFFFFF)
#define MIS_PCR_KCPU_CLK90K_LO_val(data)                                             (0xFFFFFFFF&((data)<<0))
#define MIS_PCR_KCPU_CLK90K_LO_val_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define MIS_PCR_KCPU_CLK90K_LO_get_val(data)                                         ((0xFFFFFFFF&(data))>>0)


#define MIS_PCR_KCPU_CLK90K_HI                                                       0x1801B5A0
#define MIS_PCR_KCPU_CLK90K_HI_reg_addr                                              "0xB801B5A0"
#define MIS_PCR_KCPU_CLK90K_HI_reg                                                   0xB801B5A0
#define set_MIS_PCR_KCPU_CLK90K_HI_reg(data)   (*((volatile unsigned int*) MIS_PCR_KCPU_CLK90K_HI_reg)=data)
#define get_MIS_PCR_KCPU_CLK90K_HI_reg   (*((volatile unsigned int*) MIS_PCR_KCPU_CLK90K_HI_reg))
#define MIS_PCR_KCPU_CLK90K_HI_inst_adr                                              "0x0068"
#define MIS_PCR_KCPU_CLK90K_HI_inst                                                  0x0068
#define MIS_PCR_KCPU_CLK90K_HI_val_shift                                             (0)
#define MIS_PCR_KCPU_CLK90K_HI_val_mask                                              (0x0000FFFF)
#define MIS_PCR_KCPU_CLK90K_HI_val(data)                                             (0x0000FFFF&((data)<<0))
#define MIS_PCR_KCPU_CLK90K_HI_val_src(data)                                         ((0x0000FFFF&(data))>>0)
#define MIS_PCR_KCPU_CLK90K_HI_get_val(data)                                         ((0x0000FFFF&(data))>>0)


#define MIS_TCWCR                                                                    0x1801B5B0
#define MIS_TCWCR_reg_addr                                                           "0xB801B5B0"
#define MIS_TCWCR_reg                                                                0xB801B5B0
#define set_MIS_TCWCR_reg(data)   (*((volatile unsigned int*) MIS_TCWCR_reg)=data)
#define get_MIS_TCWCR_reg   (*((volatile unsigned int*) MIS_TCWCR_reg))
#define MIS_TCWCR_inst_adr                                                           "0x006C"
#define MIS_TCWCR_inst                                                               0x006C
#define MIS_TCWCR_nmic_shift                                                         (12)
#define MIS_TCWCR_nmic_mask                                                          (0x0000F000)
#define MIS_TCWCR_nmic(data)                                                         (0x0000F000&((data)<<12))
#define MIS_TCWCR_nmic_src(data)                                                     ((0x0000F000&(data))>>12)
#define MIS_TCWCR_get_nmic(data)                                                     ((0x0000F000&(data))>>12)
#define MIS_TCWCR_wdc_shift                                                          (8)
#define MIS_TCWCR_wdc_mask                                                           (0x00000F00)
#define MIS_TCWCR_wdc(data)                                                          (0x00000F00&((data)<<8))
#define MIS_TCWCR_wdc_src(data)                                                      ((0x00000F00&(data))>>8)
#define MIS_TCWCR_get_wdc(data)                                                      ((0x00000F00&(data))>>8)
#define MIS_TCWCR_wden_shift                                                         (0)
#define MIS_TCWCR_wden_mask                                                          (0x000000FF)
#define MIS_TCWCR_wden(data)                                                         (0x000000FF&((data)<<0))
#define MIS_TCWCR_wden_src(data)                                                     ((0x000000FF&(data))>>0)
#define MIS_TCWCR_get_wden(data)                                                     ((0x000000FF&(data))>>0)


#define MIS_TCWTR                                                                    0x1801B5B4
#define MIS_TCWTR_reg_addr                                                           "0xB801B5B4"
#define MIS_TCWTR_reg                                                                0xB801B5B4
#define set_MIS_TCWTR_reg(data)   (*((volatile unsigned int*) MIS_TCWTR_reg)=data)
#define get_MIS_TCWTR_reg   (*((volatile unsigned int*) MIS_TCWTR_reg))
#define MIS_TCWTR_inst_adr                                                           "0x006D"
#define MIS_TCWTR_inst                                                               0x006D
#define MIS_TCWTR_wdclr_shift                                                        (0)
#define MIS_TCWTR_wdclr_mask                                                         (0x00000001)
#define MIS_TCWTR_wdclr(data)                                                        (0x00000001&((data)<<0))
#define MIS_TCWTR_wdclr_src(data)                                                    ((0x00000001&(data))>>0)
#define MIS_TCWTR_get_wdclr(data)                                                    ((0x00000001&(data))>>0)


#define MIS_TCWNMI                                                                   0x1801B5B8
#define MIS_TCWNMI_reg_addr                                                          "0xB801B5B8"
#define MIS_TCWNMI_reg                                                               0xB801B5B8
#define set_MIS_TCWNMI_reg(data)   (*((volatile unsigned int*) MIS_TCWNMI_reg)=data)
#define get_MIS_TCWNMI_reg   (*((volatile unsigned int*) MIS_TCWNMI_reg))
#define MIS_TCWNMI_inst_adr                                                          "0x006E"
#define MIS_TCWNMI_inst                                                              0x006E
#define MIS_TCWNMI_sel_shift                                                         (0)
#define MIS_TCWNMI_sel_mask                                                          (0xFFFFFFFF)
#define MIS_TCWNMI_sel(data)                                                         (0xFFFFFFFF&((data)<<0))
#define MIS_TCWNMI_sel_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define MIS_TCWNMI_get_sel(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define MIS_TCWOV                                                                    0x1801B5BC
#define MIS_TCWOV_reg_addr                                                           "0xB801B5BC"
#define MIS_TCWOV_reg                                                                0xB801B5BC
#define set_MIS_TCWOV_reg(data)   (*((volatile unsigned int*) MIS_TCWOV_reg)=data)
#define get_MIS_TCWOV_reg   (*((volatile unsigned int*) MIS_TCWOV_reg))
#define MIS_TCWOV_inst_adr                                                           "0x006F"
#define MIS_TCWOV_inst                                                               0x006F
#define MIS_TCWOV_sel_shift                                                          (0)
#define MIS_TCWOV_sel_mask                                                           (0xFFFFFFFF)
#define MIS_TCWOV_sel(data)                                                          (0xFFFFFFFF&((data)<<0))
#define MIS_TCWOV_sel_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define MIS_TCWOV_get_sel(data)                                                      ((0xFFFFFFFF&(data))>>0)


#define MIS_RTCSEC                                                                   0x1801B600
#define MIS_RTCSEC_reg_addr                                                          "0xB801B600"
#define MIS_RTCSEC_reg                                                               0xB801B600
#define set_MIS_RTCSEC_reg(data)   (*((volatile unsigned int*) MIS_RTCSEC_reg)=data)
#define get_MIS_RTCSEC_reg   (*((volatile unsigned int*) MIS_RTCSEC_reg))
#define MIS_RTCSEC_inst_adr                                                          "0x0080"
#define MIS_RTCSEC_inst                                                              0x0080
#define MIS_RTCSEC_rtcsec_shift                                                      (0)
#define MIS_RTCSEC_rtcsec_mask                                                       (0x0000007F)
#define MIS_RTCSEC_rtcsec(data)                                                      (0x0000007F&((data)<<0))
#define MIS_RTCSEC_rtcsec_src(data)                                                  ((0x0000007F&(data))>>0)
#define MIS_RTCSEC_get_rtcsec(data)                                                  ((0x0000007F&(data))>>0)


#define MIS_RTCMIN                                                                   0x1801B604
#define MIS_RTCMIN_reg_addr                                                          "0xB801B604"
#define MIS_RTCMIN_reg                                                               0xB801B604
#define set_MIS_RTCMIN_reg(data)   (*((volatile unsigned int*) MIS_RTCMIN_reg)=data)
#define get_MIS_RTCMIN_reg   (*((volatile unsigned int*) MIS_RTCMIN_reg))
#define MIS_RTCMIN_inst_adr                                                          "0x0081"
#define MIS_RTCMIN_inst                                                              0x0081
#define MIS_RTCMIN_rtcmin_shift                                                      (0)
#define MIS_RTCMIN_rtcmin_mask                                                       (0x0000003F)
#define MIS_RTCMIN_rtcmin(data)                                                      (0x0000003F&((data)<<0))
#define MIS_RTCMIN_rtcmin_src(data)                                                  ((0x0000003F&(data))>>0)
#define MIS_RTCMIN_get_rtcmin(data)                                                  ((0x0000003F&(data))>>0)


#define MIS_RTCHR                                                                    0x1801B608
#define MIS_RTCHR_reg_addr                                                           "0xB801B608"
#define MIS_RTCHR_reg                                                                0xB801B608
#define set_MIS_RTCHR_reg(data)   (*((volatile unsigned int*) MIS_RTCHR_reg)=data)
#define get_MIS_RTCHR_reg   (*((volatile unsigned int*) MIS_RTCHR_reg))
#define MIS_RTCHR_inst_adr                                                           "0x0082"
#define MIS_RTCHR_inst                                                               0x0082
#define MIS_RTCHR_rtchr_shift                                                        (0)
#define MIS_RTCHR_rtchr_mask                                                         (0x0000001F)
#define MIS_RTCHR_rtchr(data)                                                        (0x0000001F&((data)<<0))
#define MIS_RTCHR_rtchr_src(data)                                                    ((0x0000001F&(data))>>0)
#define MIS_RTCHR_get_rtchr(data)                                                    ((0x0000001F&(data))>>0)


#define MIS_RTCDATE1                                                                 0x1801B60C
#define MIS_RTCDATE1_reg_addr                                                        "0xB801B60C"
#define MIS_RTCDATE1_reg                                                             0xB801B60C
#define set_MIS_RTCDATE1_reg(data)   (*((volatile unsigned int*) MIS_RTCDATE1_reg)=data)
#define get_MIS_RTCDATE1_reg   (*((volatile unsigned int*) MIS_RTCDATE1_reg))
#define MIS_RTCDATE1_inst_adr                                                        "0x0083"
#define MIS_RTCDATE1_inst                                                            0x0083
#define MIS_RTCDATE1_rtcdate1_shift                                                  (0)
#define MIS_RTCDATE1_rtcdate1_mask                                                   (0x000000FF)
#define MIS_RTCDATE1_rtcdate1(data)                                                  (0x000000FF&((data)<<0))
#define MIS_RTCDATE1_rtcdate1_src(data)                                              ((0x000000FF&(data))>>0)
#define MIS_RTCDATE1_get_rtcdate1(data)                                              ((0x000000FF&(data))>>0)


#define MIS_RTCDATE2                                                                 0x1801B610
#define MIS_RTCDATE2_reg_addr                                                        "0xB801B610"
#define MIS_RTCDATE2_reg                                                             0xB801B610
#define set_MIS_RTCDATE2_reg(data)   (*((volatile unsigned int*) MIS_RTCDATE2_reg)=data)
#define get_MIS_RTCDATE2_reg   (*((volatile unsigned int*) MIS_RTCDATE2_reg))
#define MIS_RTCDATE2_inst_adr                                                        "0x0084"
#define MIS_RTCDATE2_inst                                                            0x0084
#define MIS_RTCDATE2_rtcdate2_shift                                                  (0)
#define MIS_RTCDATE2_rtcdate2_mask                                                   (0x0000007F)
#define MIS_RTCDATE2_rtcdate2(data)                                                  (0x0000007F&((data)<<0))
#define MIS_RTCDATE2_rtcdate2_src(data)                                              ((0x0000007F&(data))>>0)
#define MIS_RTCDATE2_get_rtcdate2(data)                                              ((0x0000007F&(data))>>0)


#define MIS_ALMMIN                                                                   0x1801B614
#define MIS_ALMMIN_reg_addr                                                          "0xB801B614"
#define MIS_ALMMIN_reg                                                               0xB801B614
#define set_MIS_ALMMIN_reg(data)   (*((volatile unsigned int*) MIS_ALMMIN_reg)=data)
#define get_MIS_ALMMIN_reg   (*((volatile unsigned int*) MIS_ALMMIN_reg))
#define MIS_ALMMIN_inst_adr                                                          "0x0085"
#define MIS_ALMMIN_inst                                                              0x0085
#define MIS_ALMMIN_almmin_shift                                                      (0)
#define MIS_ALMMIN_almmin_mask                                                       (0x0000003F)
#define MIS_ALMMIN_almmin(data)                                                      (0x0000003F&((data)<<0))
#define MIS_ALMMIN_almmin_src(data)                                                  ((0x0000003F&(data))>>0)
#define MIS_ALMMIN_get_almmin(data)                                                  ((0x0000003F&(data))>>0)


#define MIS_ALMHR                                                                    0x1801B618
#define MIS_ALMHR_reg_addr                                                           "0xB801B618"
#define MIS_ALMHR_reg                                                                0xB801B618
#define set_MIS_ALMHR_reg(data)   (*((volatile unsigned int*) MIS_ALMHR_reg)=data)
#define get_MIS_ALMHR_reg   (*((volatile unsigned int*) MIS_ALMHR_reg))
#define MIS_ALMHR_inst_adr                                                           "0x0086"
#define MIS_ALMHR_inst                                                               0x0086
#define MIS_ALMHR_almhr_shift                                                        (0)
#define MIS_ALMHR_almhr_mask                                                         (0x0000001F)
#define MIS_ALMHR_almhr(data)                                                        (0x0000001F&((data)<<0))
#define MIS_ALMHR_almhr_src(data)                                                    ((0x0000001F&(data))>>0)
#define MIS_ALMHR_get_almhr(data)                                                    ((0x0000001F&(data))>>0)


#define MIS_ALMDATE1                                                                 0x1801B61C
#define MIS_ALMDATE1_reg_addr                                                        "0xB801B61C"
#define MIS_ALMDATE1_reg                                                             0xB801B61C
#define set_MIS_ALMDATE1_reg(data)   (*((volatile unsigned int*) MIS_ALMDATE1_reg)=data)
#define get_MIS_ALMDATE1_reg   (*((volatile unsigned int*) MIS_ALMDATE1_reg))
#define MIS_ALMDATE1_inst_adr                                                        "0x0087"
#define MIS_ALMDATE1_inst                                                            0x0087
#define MIS_ALMDATE1_almdate1_shift                                                  (0)
#define MIS_ALMDATE1_almdate1_mask                                                   (0x000000FF)
#define MIS_ALMDATE1_almdate1(data)                                                  (0x000000FF&((data)<<0))
#define MIS_ALMDATE1_almdate1_src(data)                                              ((0x000000FF&(data))>>0)
#define MIS_ALMDATE1_get_almdate1(data)                                              ((0x000000FF&(data))>>0)


#define MIS_ALMDATE2                                                                 0x1801B620
#define MIS_ALMDATE2_reg_addr                                                        "0xB801B620"
#define MIS_ALMDATE2_reg                                                             0xB801B620
#define set_MIS_ALMDATE2_reg(data)   (*((volatile unsigned int*) MIS_ALMDATE2_reg)=data)
#define get_MIS_ALMDATE2_reg   (*((volatile unsigned int*) MIS_ALMDATE2_reg))
#define MIS_ALMDATE2_inst_adr                                                        "0x0088"
#define MIS_ALMDATE2_inst                                                            0x0088
#define MIS_ALMDATE2_almdate2_shift                                                  (0)
#define MIS_ALMDATE2_almdate2_mask                                                   (0x0000007F)
#define MIS_ALMDATE2_almdate2(data)                                                  (0x0000007F&((data)<<0))
#define MIS_ALMDATE2_almdate2_src(data)                                              ((0x0000007F&(data))>>0)
#define MIS_ALMDATE2_get_almdate2(data)                                              ((0x0000007F&(data))>>0)


#define MIS_RTCSTOP                                                                  0x1801B624
#define MIS_RTCSTOP_reg_addr                                                         "0xB801B624"
#define MIS_RTCSTOP_reg                                                              0xB801B624
#define set_MIS_RTCSTOP_reg(data)   (*((volatile unsigned int*) MIS_RTCSTOP_reg)=data)
#define get_MIS_RTCSTOP_reg   (*((volatile unsigned int*) MIS_RTCSTOP_reg))
#define MIS_RTCSTOP_inst_adr                                                         "0x0089"
#define MIS_RTCSTOP_inst                                                             0x0089
#define MIS_RTCSTOP_vref_shift                                                       (1)
#define MIS_RTCSTOP_vref_mask                                                        (0x00000006)
#define MIS_RTCSTOP_vref(data)                                                       (0x00000006&((data)<<1))
#define MIS_RTCSTOP_vref_src(data)                                                   ((0x00000006&(data))>>1)
#define MIS_RTCSTOP_get_vref(data)                                                   ((0x00000006&(data))>>1)
#define MIS_RTCSTOP_rtcstop_shift                                                    (0)
#define MIS_RTCSTOP_rtcstop_mask                                                     (0x00000001)
#define MIS_RTCSTOP_rtcstop(data)                                                    (0x00000001&((data)<<0))
#define MIS_RTCSTOP_rtcstop_src(data)                                                ((0x00000001&(data))>>0)
#define MIS_RTCSTOP_get_rtcstop(data)                                                ((0x00000001&(data))>>0)


#define MIS_RTCACR                                                                   0x1801B628
#define MIS_RTCACR_reg_addr                                                          "0xB801B628"
#define MIS_RTCACR_reg                                                               0xB801B628
#define set_MIS_RTCACR_reg(data)   (*((volatile unsigned int*) MIS_RTCACR_reg)=data)
#define get_MIS_RTCACR_reg   (*((volatile unsigned int*) MIS_RTCACR_reg))
#define MIS_RTCACR_inst_adr                                                          "0x008A"
#define MIS_RTCACR_inst                                                              0x008A
#define MIS_RTCACR_rtcpwr_shift                                                      (7)
#define MIS_RTCACR_rtcpwr_mask                                                       (0x00000080)
#define MIS_RTCACR_rtcpwr(data)                                                      (0x00000080&((data)<<7))
#define MIS_RTCACR_rtcpwr_src(data)                                                  ((0x00000080&(data))>>7)
#define MIS_RTCACR_get_rtcpwr(data)                                                  ((0x00000080&(data))>>7)
#define MIS_RTCACR_co_shift                                                          (5)
#define MIS_RTCACR_co_mask                                                           (0x00000060)
#define MIS_RTCACR_co(data)                                                          (0x00000060&((data)<<5))
#define MIS_RTCACR_co_src(data)                                                      ((0x00000060&(data))>>5)
#define MIS_RTCACR_get_co(data)                                                      ((0x00000060&(data))>>5)
#define MIS_RTCACR_ci_shift                                                          (3)
#define MIS_RTCACR_ci_mask                                                           (0x00000018)
#define MIS_RTCACR_ci(data)                                                          (0x00000018&((data)<<3))
#define MIS_RTCACR_ci_src(data)                                                      ((0x00000018&(data))>>3)
#define MIS_RTCACR_get_ci(data)                                                      ((0x00000018&(data))>>3)
#define MIS_RTCACR_clksel_shift                                                      (2)
#define MIS_RTCACR_clksel_mask                                                       (0x00000004)
#define MIS_RTCACR_clksel(data)                                                      (0x00000004&((data)<<2))
#define MIS_RTCACR_clksel_src(data)                                                  ((0x00000004&(data))>>2)
#define MIS_RTCACR_get_clksel(data)                                                  ((0x00000004&(data))>>2)
#define MIS_RTCACR_bc_shift                                                          (0)
#define MIS_RTCACR_bc_mask                                                           (0x00000003)
#define MIS_RTCACR_bc(data)                                                          (0x00000003&((data)<<0))
#define MIS_RTCACR_bc_src(data)                                                      ((0x00000003&(data))>>0)
#define MIS_RTCACR_get_bc(data)                                                      ((0x00000003&(data))>>0)


#define MIS_RTCEN                                                                    0x1801B62C
#define MIS_RTCEN_reg_addr                                                           "0xB801B62C"
#define MIS_RTCEN_reg                                                                0xB801B62C
#define set_MIS_RTCEN_reg(data)   (*((volatile unsigned int*) MIS_RTCEN_reg)=data)
#define get_MIS_RTCEN_reg   (*((volatile unsigned int*) MIS_RTCEN_reg))
#define MIS_RTCEN_inst_adr                                                           "0x008B"
#define MIS_RTCEN_inst                                                               0x008B
#define MIS_RTCEN_rtcen_shift                                                        (0)
#define MIS_RTCEN_rtcen_mask                                                         (0x000000FF)
#define MIS_RTCEN_rtcen(data)                                                        (0x000000FF&((data)<<0))
#define MIS_RTCEN_rtcen_src(data)                                                    ((0x000000FF&(data))>>0)
#define MIS_RTCEN_get_rtcen(data)                                                    ((0x000000FF&(data))>>0)


#define MIS_RTCCR                                                                    0x1801B630
#define MIS_RTCCR_reg_addr                                                           "0xB801B630"
#define MIS_RTCCR_reg                                                                0xB801B630
#define set_MIS_RTCCR_reg(data)   (*((volatile unsigned int*) MIS_RTCCR_reg)=data)
#define get_MIS_RTCCR_reg   (*((volatile unsigned int*) MIS_RTCCR_reg))
#define MIS_RTCCR_inst_adr                                                           "0x008C"
#define MIS_RTCCR_inst                                                               0x008C
#define MIS_RTCCR_rtcrst_shift                                                       (6)
#define MIS_RTCCR_rtcrst_mask                                                        (0x00000040)
#define MIS_RTCCR_rtcrst(data)                                                       (0x00000040&((data)<<6))
#define MIS_RTCCR_rtcrst_src(data)                                                   ((0x00000040&(data))>>6)
#define MIS_RTCCR_get_rtcrst(data)                                                   ((0x00000040&(data))>>6)
#define MIS_RTCCR_dainte_shift                                                       (3)
#define MIS_RTCCR_dainte_mask                                                        (0x00000008)
#define MIS_RTCCR_dainte(data)                                                       (0x00000008&((data)<<3))
#define MIS_RTCCR_dainte_src(data)                                                   ((0x00000008&(data))>>3)
#define MIS_RTCCR_get_dainte(data)                                                   ((0x00000008&(data))>>3)
#define MIS_RTCCR_huinte_shift                                                       (2)
#define MIS_RTCCR_huinte_mask                                                        (0x00000004)
#define MIS_RTCCR_huinte(data)                                                       (0x00000004&((data)<<2))
#define MIS_RTCCR_huinte_src(data)                                                   ((0x00000004&(data))>>2)
#define MIS_RTCCR_get_huinte(data)                                                   ((0x00000004&(data))>>2)
#define MIS_RTCCR_muinte_shift                                                       (1)
#define MIS_RTCCR_muinte_mask                                                        (0x00000002)
#define MIS_RTCCR_muinte(data)                                                       (0x00000002&((data)<<1))
#define MIS_RTCCR_muinte_src(data)                                                   ((0x00000002&(data))>>1)
#define MIS_RTCCR_get_muinte(data)                                                   ((0x00000002&(data))>>1)
#define MIS_RTCCR_hsuinte_shift                                                      (0)
#define MIS_RTCCR_hsuinte_mask                                                       (0x00000001)
#define MIS_RTCCR_hsuinte(data)                                                      (0x00000001&((data)<<0))
#define MIS_RTCCR_hsuinte_src(data)                                                  ((0x00000001&(data))>>0)
#define MIS_RTCCR_get_hsuinte(data)                                                  ((0x00000001&(data))>>0)


#define MIS_PCMCIA_CMDFF                                                             0x1801B700
#define MIS_PCMCIA_CMDFF_reg_addr                                                    "0xB801B700"
#define MIS_PCMCIA_CMDFF_reg                                                         0xB801B700
#define set_MIS_PCMCIA_CMDFF_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_CMDFF_reg)=data)
#define get_MIS_PCMCIA_CMDFF_reg   (*((volatile unsigned int*) MIS_PCMCIA_CMDFF_reg))
#define MIS_PCMCIA_CMDFF_inst_adr                                                    "0x00C0"
#define MIS_PCMCIA_CMDFF_inst                                                        0x00C0
#define MIS_PCMCIA_CMDFF_ct_shift                                                    (24)
#define MIS_PCMCIA_CMDFF_ct_mask                                                     (0x01000000)
#define MIS_PCMCIA_CMDFF_ct(data)                                                    (0x01000000&((data)<<24))
#define MIS_PCMCIA_CMDFF_ct_src(data)                                                ((0x01000000&(data))>>24)
#define MIS_PCMCIA_CMDFF_get_ct(data)                                                ((0x01000000&(data))>>24)
#define MIS_PCMCIA_CMDFF_at_shift                                                    (23)
#define MIS_PCMCIA_CMDFF_at_mask                                                     (0x00800000)
#define MIS_PCMCIA_CMDFF_at(data)                                                    (0x00800000&((data)<<23))
#define MIS_PCMCIA_CMDFF_at_src(data)                                                ((0x00800000&(data))>>23)
#define MIS_PCMCIA_CMDFF_get_at(data)                                                ((0x00800000&(data))>>23)
#define MIS_PCMCIA_CMDFF_pa_shift                                                    (8)
#define MIS_PCMCIA_CMDFF_pa_mask                                                     (0x007FFF00)
#define MIS_PCMCIA_CMDFF_pa(data)                                                    (0x007FFF00&((data)<<8))
#define MIS_PCMCIA_CMDFF_pa_src(data)                                                ((0x007FFF00&(data))>>8)
#define MIS_PCMCIA_CMDFF_get_pa(data)                                                ((0x007FFF00&(data))>>8)
#define MIS_PCMCIA_CMDFF_df_shift                                                    (0)
#define MIS_PCMCIA_CMDFF_df_mask                                                     (0x000000FF)
#define MIS_PCMCIA_CMDFF_df(data)                                                    (0x000000FF&((data)<<0))
#define MIS_PCMCIA_CMDFF_df_src(data)                                                ((0x000000FF&(data))>>0)
#define MIS_PCMCIA_CMDFF_get_df(data)                                                ((0x000000FF&(data))>>0)


#define MIS_PCMCIA_CR                                                                0x1801B704
#define MIS_PCMCIA_CR_reg_addr                                                       "0xB801B704"
#define MIS_PCMCIA_CR_reg                                                            0xB801B704
#define set_MIS_PCMCIA_CR_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_CR_reg)=data)
#define get_MIS_PCMCIA_CR_reg   (*((volatile unsigned int*) MIS_PCMCIA_CR_reg))
#define MIS_PCMCIA_CR_inst_adr                                                       "0x00C1"
#define MIS_PCMCIA_CR_inst                                                           0x00C1
#define MIS_PCMCIA_CR_psr_shift                                                      (31)
#define MIS_PCMCIA_CR_psr_mask                                                       (0x80000000)
#define MIS_PCMCIA_CR_psr(data)                                                      (0x80000000&((data)<<31))
#define MIS_PCMCIA_CR_psr_src(data)                                                  ((0x80000000&(data))>>31)
#define MIS_PCMCIA_CR_get_psr(data)                                                  ((0x80000000&(data))>>31)
#define MIS_PCMCIA_CR_ce1_card1_shift                                                (30)
#define MIS_PCMCIA_CR_ce1_card1_mask                                                 (0x40000000)
#define MIS_PCMCIA_CR_ce1_card1(data)                                                (0x40000000&((data)<<30))
#define MIS_PCMCIA_CR_ce1_card1_src(data)                                            ((0x40000000&(data))>>30)
#define MIS_PCMCIA_CR_get_ce1_card1(data)                                            ((0x40000000&(data))>>30)
#define MIS_PCMCIA_CR_ce1_card2_shift                                                (29)
#define MIS_PCMCIA_CR_ce1_card2_mask                                                 (0x20000000)
#define MIS_PCMCIA_CR_ce1_card2(data)                                                (0x20000000&((data)<<29))
#define MIS_PCMCIA_CR_ce1_card2_src(data)                                            ((0x20000000&(data))>>29)
#define MIS_PCMCIA_CR_get_ce1_card2(data)                                            ((0x20000000&(data))>>29)
#define MIS_PCMCIA_CR_ce2_card1_shift                                                (28)
#define MIS_PCMCIA_CR_ce2_card1_mask                                                 (0x10000000)
#define MIS_PCMCIA_CR_ce2_card1(data)                                                (0x10000000&((data)<<28))
#define MIS_PCMCIA_CR_ce2_card1_src(data)                                            ((0x10000000&(data))>>28)
#define MIS_PCMCIA_CR_get_ce2_card1(data)                                            ((0x10000000&(data))>>28)
#define MIS_PCMCIA_CR_ce2_card2_shift                                                (27)
#define MIS_PCMCIA_CR_ce2_card2_mask                                                 (0x08000000)
#define MIS_PCMCIA_CR_ce2_card2(data)                                                (0x08000000&((data)<<27))
#define MIS_PCMCIA_CR_ce2_card2_src(data)                                            ((0x08000000&(data))>>27)
#define MIS_PCMCIA_CR_get_ce2_card2(data)                                            ((0x08000000&(data))>>27)
#define MIS_PCMCIA_CR_pcr1_shift                                                     (26)
#define MIS_PCMCIA_CR_pcr1_mask                                                      (0x04000000)
#define MIS_PCMCIA_CR_pcr1(data)                                                     (0x04000000&((data)<<26))
#define MIS_PCMCIA_CR_pcr1_src(data)                                                 ((0x04000000&(data))>>26)
#define MIS_PCMCIA_CR_get_pcr1(data)                                                 ((0x04000000&(data))>>26)
#define MIS_PCMCIA_CR_pcr2_shift                                                     (25)
#define MIS_PCMCIA_CR_pcr2_mask                                                      (0x02000000)
#define MIS_PCMCIA_CR_pcr2(data)                                                     (0x02000000&((data)<<25))
#define MIS_PCMCIA_CR_pcr2_src(data)                                                 ((0x02000000&(data))>>25)
#define MIS_PCMCIA_CR_get_pcr2(data)                                                 ((0x02000000&(data))>>25)
#define MIS_PCMCIA_CR_pcr1_oe_shift                                                  (24)
#define MIS_PCMCIA_CR_pcr1_oe_mask                                                   (0x01000000)
#define MIS_PCMCIA_CR_pcr1_oe(data)                                                  (0x01000000&((data)<<24))
#define MIS_PCMCIA_CR_pcr1_oe_src(data)                                              ((0x01000000&(data))>>24)
#define MIS_PCMCIA_CR_get_pcr1_oe(data)                                              ((0x01000000&(data))>>24)
#define MIS_PCMCIA_CR_pcr2_oe_shift                                                  (23)
#define MIS_PCMCIA_CR_pcr2_oe_mask                                                   (0x00800000)
#define MIS_PCMCIA_CR_pcr2_oe(data)                                                  (0x00800000&((data)<<23))
#define MIS_PCMCIA_CR_pcr2_oe_src(data)                                              ((0x00800000&(data))>>23)
#define MIS_PCMCIA_CR_get_pcr2_oe(data)                                              ((0x00800000&(data))>>23)
#define MIS_PCMCIA_CR_piie1_shift                                                    (7)
#define MIS_PCMCIA_CR_piie1_mask                                                     (0x00000080)
#define MIS_PCMCIA_CR_piie1(data)                                                    (0x00000080&((data)<<7))
#define MIS_PCMCIA_CR_piie1_src(data)                                                ((0x00000080&(data))>>7)
#define MIS_PCMCIA_CR_get_piie1(data)                                                ((0x00000080&(data))>>7)
#define MIS_PCMCIA_CR_pciie1_shift                                                   (6)
#define MIS_PCMCIA_CR_pciie1_mask                                                    (0x00000040)
#define MIS_PCMCIA_CR_pciie1(data)                                                   (0x00000040&((data)<<6))
#define MIS_PCMCIA_CR_pciie1_src(data)                                               ((0x00000040&(data))>>6)
#define MIS_PCMCIA_CR_get_pciie1(data)                                               ((0x00000040&(data))>>6)
#define MIS_PCMCIA_CR_pcrie1_shift                                                   (5)
#define MIS_PCMCIA_CR_pcrie1_mask                                                    (0x00000020)
#define MIS_PCMCIA_CR_pcrie1(data)                                                   (0x00000020&((data)<<5))
#define MIS_PCMCIA_CR_pcrie1_src(data)                                               ((0x00000020&(data))>>5)
#define MIS_PCMCIA_CR_get_pcrie1(data)                                               ((0x00000020&(data))>>5)
#define MIS_PCMCIA_CR_piie2_shift                                                    (4)
#define MIS_PCMCIA_CR_piie2_mask                                                     (0x00000010)
#define MIS_PCMCIA_CR_piie2(data)                                                    (0x00000010&((data)<<4))
#define MIS_PCMCIA_CR_piie2_src(data)                                                ((0x00000010&(data))>>4)
#define MIS_PCMCIA_CR_get_piie2(data)                                                ((0x00000010&(data))>>4)
#define MIS_PCMCIA_CR_pciie2_shift                                                   (3)
#define MIS_PCMCIA_CR_pciie2_mask                                                    (0x00000008)
#define MIS_PCMCIA_CR_pciie2(data)                                                   (0x00000008&((data)<<3))
#define MIS_PCMCIA_CR_pciie2_src(data)                                               ((0x00000008&(data))>>3)
#define MIS_PCMCIA_CR_get_pciie2(data)                                               ((0x00000008&(data))>>3)
#define MIS_PCMCIA_CR_pcrie2_shift                                                   (2)
#define MIS_PCMCIA_CR_pcrie2_mask                                                    (0x00000004)
#define MIS_PCMCIA_CR_pcrie2(data)                                                   (0x00000004&((data)<<2))
#define MIS_PCMCIA_CR_pcrie2_src(data)                                               ((0x00000004&(data))>>2)
#define MIS_PCMCIA_CR_get_pcrie2(data)                                               ((0x00000004&(data))>>2)
#define MIS_PCMCIA_CR_afie_shift                                                     (1)
#define MIS_PCMCIA_CR_afie_mask                                                      (0x00000002)
#define MIS_PCMCIA_CR_afie(data)                                                     (0x00000002&((data)<<1))
#define MIS_PCMCIA_CR_afie_src(data)                                                 ((0x00000002&(data))>>1)
#define MIS_PCMCIA_CR_get_afie(data)                                                 ((0x00000002&(data))>>1)
#define MIS_PCMCIA_CR_apfie_shift                                                    (0)
#define MIS_PCMCIA_CR_apfie_mask                                                     (0x00000001)
#define MIS_PCMCIA_CR_apfie(data)                                                    (0x00000001&((data)<<0))
#define MIS_PCMCIA_CR_apfie_src(data)                                                ((0x00000001&(data))>>0)
#define MIS_PCMCIA_CR_get_apfie(data)                                                ((0x00000001&(data))>>0)


#define MIS_PCMCIA_STS                                                               0x1801B708
#define MIS_PCMCIA_STS_reg_addr                                                      "0xB801B708"
#define MIS_PCMCIA_STS_reg                                                           0xB801B708
#define set_MIS_PCMCIA_STS_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_STS_reg)=data)
#define get_MIS_PCMCIA_STS_reg   (*((volatile unsigned int*) MIS_PCMCIA_STS_reg))
#define MIS_PCMCIA_STS_inst_adr                                                      "0x00C2"
#define MIS_PCMCIA_STS_inst                                                          0x00C2
#define MIS_PCMCIA_STS_pres1_shift                                                   (9)
#define MIS_PCMCIA_STS_pres1_mask                                                    (0x00000200)
#define MIS_PCMCIA_STS_pres1(data)                                                   (0x00000200&((data)<<9))
#define MIS_PCMCIA_STS_pres1_src(data)                                               ((0x00000200&(data))>>9)
#define MIS_PCMCIA_STS_get_pres1(data)                                               ((0x00000200&(data))>>9)
#define MIS_PCMCIA_STS_pres2_shift                                                   (8)
#define MIS_PCMCIA_STS_pres2_mask                                                    (0x00000100)
#define MIS_PCMCIA_STS_pres2(data)                                                   (0x00000100&((data)<<8))
#define MIS_PCMCIA_STS_pres2_src(data)                                               ((0x00000100&(data))>>8)
#define MIS_PCMCIA_STS_get_pres2(data)                                               ((0x00000100&(data))>>8)
#define MIS_PCMCIA_STS_pii1_shift                                                    (7)
#define MIS_PCMCIA_STS_pii1_mask                                                     (0x00000080)
#define MIS_PCMCIA_STS_pii1(data)                                                    (0x00000080&((data)<<7))
#define MIS_PCMCIA_STS_pii1_src(data)                                                ((0x00000080&(data))>>7)
#define MIS_PCMCIA_STS_get_pii1(data)                                                ((0x00000080&(data))>>7)
#define MIS_PCMCIA_STS_pcii1_shift                                                   (6)
#define MIS_PCMCIA_STS_pcii1_mask                                                    (0x00000040)
#define MIS_PCMCIA_STS_pcii1(data)                                                   (0x00000040&((data)<<6))
#define MIS_PCMCIA_STS_pcii1_src(data)                                               ((0x00000040&(data))>>6)
#define MIS_PCMCIA_STS_get_pcii1(data)                                               ((0x00000040&(data))>>6)
#define MIS_PCMCIA_STS_pcri1_shift                                                   (5)
#define MIS_PCMCIA_STS_pcri1_mask                                                    (0x00000020)
#define MIS_PCMCIA_STS_pcri1(data)                                                   (0x00000020&((data)<<5))
#define MIS_PCMCIA_STS_pcri1_src(data)                                               ((0x00000020&(data))>>5)
#define MIS_PCMCIA_STS_get_pcri1(data)                                               ((0x00000020&(data))>>5)
#define MIS_PCMCIA_STS_pii2_shift                                                    (4)
#define MIS_PCMCIA_STS_pii2_mask                                                     (0x00000010)
#define MIS_PCMCIA_STS_pii2(data)                                                    (0x00000010&((data)<<4))
#define MIS_PCMCIA_STS_pii2_src(data)                                                ((0x00000010&(data))>>4)
#define MIS_PCMCIA_STS_get_pii2(data)                                                ((0x00000010&(data))>>4)
#define MIS_PCMCIA_STS_pcii2_shift                                                   (3)
#define MIS_PCMCIA_STS_pcii2_mask                                                    (0x00000008)
#define MIS_PCMCIA_STS_pcii2(data)                                                   (0x00000008&((data)<<3))
#define MIS_PCMCIA_STS_pcii2_src(data)                                               ((0x00000008&(data))>>3)
#define MIS_PCMCIA_STS_get_pcii2(data)                                               ((0x00000008&(data))>>3)
#define MIS_PCMCIA_STS_pcri2_shift                                                   (2)
#define MIS_PCMCIA_STS_pcri2_mask                                                    (0x00000004)
#define MIS_PCMCIA_STS_pcri2(data)                                                   (0x00000004&((data)<<2))
#define MIS_PCMCIA_STS_pcri2_src(data)                                               ((0x00000004&(data))>>2)
#define MIS_PCMCIA_STS_get_pcri2(data)                                               ((0x00000004&(data))>>2)
#define MIS_PCMCIA_STS_afi_shift                                                     (1)
#define MIS_PCMCIA_STS_afi_mask                                                      (0x00000002)
#define MIS_PCMCIA_STS_afi(data)                                                     (0x00000002&((data)<<1))
#define MIS_PCMCIA_STS_afi_src(data)                                                 ((0x00000002&(data))>>1)
#define MIS_PCMCIA_STS_get_afi(data)                                                 ((0x00000002&(data))>>1)
#define MIS_PCMCIA_STS_apfi_shift                                                    (0)
#define MIS_PCMCIA_STS_apfi_mask                                                     (0x00000001)
#define MIS_PCMCIA_STS_apfi(data)                                                    (0x00000001&((data)<<0))
#define MIS_PCMCIA_STS_apfi_src(data)                                                ((0x00000001&(data))>>0)
#define MIS_PCMCIA_STS_get_apfi(data)                                                ((0x00000001&(data))>>0)


#define MIS_PCMCIA_AMTC                                                              0x1801B70C
#define MIS_PCMCIA_AMTC_reg_addr                                                     "0xB801B70C"
#define MIS_PCMCIA_AMTC_reg                                                          0xB801B70C
#define set_MIS_PCMCIA_AMTC_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_AMTC_reg)=data)
#define get_MIS_PCMCIA_AMTC_reg   (*((volatile unsigned int*) MIS_PCMCIA_AMTC_reg))
#define MIS_PCMCIA_AMTC_inst_adr                                                     "0x00C3"
#define MIS_PCMCIA_AMTC_inst                                                         0x00C3
#define MIS_PCMCIA_AMTC_twe_shift                                                    (26)
#define MIS_PCMCIA_AMTC_twe_mask                                                     (0xFC000000)
#define MIS_PCMCIA_AMTC_twe(data)                                                    (0xFC000000&((data)<<26))
#define MIS_PCMCIA_AMTC_twe_src(data)                                                ((0xFC000000&(data))>>26)
#define MIS_PCMCIA_AMTC_get_twe(data)                                                ((0xFC000000&(data))>>26)
#define MIS_PCMCIA_AMTC_thd_shift                                                    (22)
#define MIS_PCMCIA_AMTC_thd_mask                                                     (0x03C00000)
#define MIS_PCMCIA_AMTC_thd(data)                                                    (0x03C00000&((data)<<22))
#define MIS_PCMCIA_AMTC_thd_src(data)                                                ((0x03C00000&(data))>>22)
#define MIS_PCMCIA_AMTC_get_thd(data)                                                ((0x03C00000&(data))>>22)
#define MIS_PCMCIA_AMTC_taoe_shift                                                   (17)
#define MIS_PCMCIA_AMTC_taoe_mask                                                    (0x003E0000)
#define MIS_PCMCIA_AMTC_taoe(data)                                                   (0x003E0000&((data)<<17))
#define MIS_PCMCIA_AMTC_taoe_src(data)                                               ((0x003E0000&(data))>>17)
#define MIS_PCMCIA_AMTC_get_taoe(data)                                               ((0x003E0000&(data))>>17)
#define MIS_PCMCIA_AMTC_thce_shift                                                   (8)
#define MIS_PCMCIA_AMTC_thce_mask                                                    (0x00001F00)
#define MIS_PCMCIA_AMTC_thce(data)                                                   (0x00001F00&((data)<<8))
#define MIS_PCMCIA_AMTC_thce_src(data)                                               ((0x00001F00&(data))>>8)
#define MIS_PCMCIA_AMTC_get_thce(data)                                               ((0x00001F00&(data))>>8)
#define MIS_PCMCIA_AMTC_tsu_shift                                                    (0)
#define MIS_PCMCIA_AMTC_tsu_mask                                                     (0x0000001F)
#define MIS_PCMCIA_AMTC_tsu(data)                                                    (0x0000001F&((data)<<0))
#define MIS_PCMCIA_AMTC_tsu_src(data)                                                ((0x0000001F&(data))>>0)
#define MIS_PCMCIA_AMTC_get_tsu(data)                                                ((0x0000001F&(data))>>0)


#define MIS_PCMCIA_IOMTC                                                             0x1801B710
#define MIS_PCMCIA_IOMTC_reg_addr                                                    "0xB801B710"
#define MIS_PCMCIA_IOMTC_reg                                                         0xB801B710
#define set_MIS_PCMCIA_IOMTC_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_IOMTC_reg)=data)
#define get_MIS_PCMCIA_IOMTC_reg   (*((volatile unsigned int*) MIS_PCMCIA_IOMTC_reg))
#define MIS_PCMCIA_IOMTC_inst_adr                                                    "0x00C4"
#define MIS_PCMCIA_IOMTC_inst                                                        0x00C4
#define MIS_PCMCIA_IOMTC_tdiord_shift                                                (16)
#define MIS_PCMCIA_IOMTC_tdiord_mask                                                 (0x001F0000)
#define MIS_PCMCIA_IOMTC_tdiord(data)                                                (0x001F0000&((data)<<16))
#define MIS_PCMCIA_IOMTC_tdiord_src(data)                                            ((0x001F0000&(data))>>16)
#define MIS_PCMCIA_IOMTC_get_tdiord(data)                                            ((0x001F0000&(data))>>16)
#define MIS_PCMCIA_IOMTC_tsuio_shift                                                 (8)
#define MIS_PCMCIA_IOMTC_tsuio_mask                                                  (0x00001F00)
#define MIS_PCMCIA_IOMTC_tsuio(data)                                                 (0x00001F00&((data)<<8))
#define MIS_PCMCIA_IOMTC_tsuio_src(data)                                             ((0x00001F00&(data))>>8)
#define MIS_PCMCIA_IOMTC_get_tsuio(data)                                             ((0x00001F00&(data))>>8)
#define MIS_PCMCIA_IOMTC_tdinpack_shift                                              (4)
#define MIS_PCMCIA_IOMTC_tdinpack_mask                                               (0x000000F0)
#define MIS_PCMCIA_IOMTC_tdinpack(data)                                              (0x000000F0&((data)<<4))
#define MIS_PCMCIA_IOMTC_tdinpack_src(data)                                          ((0x000000F0&(data))>>4)
#define MIS_PCMCIA_IOMTC_get_tdinpack(data)                                          ((0x000000F0&(data))>>4)
#define MIS_PCMCIA_IOMTC_tdwt_shift                                                  (0)
#define MIS_PCMCIA_IOMTC_tdwt_mask                                                   (0x0000000F)
#define MIS_PCMCIA_IOMTC_tdwt(data)                                                  (0x0000000F&((data)<<0))
#define MIS_PCMCIA_IOMTC_tdwt_src(data)                                              ((0x0000000F&(data))>>0)
#define MIS_PCMCIA_IOMTC_get_tdwt(data)                                              ((0x0000000F&(data))>>0)


#define MIS_PCMCIA_MATC                                                              0x1801B714
#define MIS_PCMCIA_MATC_reg_addr                                                     "0xB801B714"
#define MIS_PCMCIA_MATC_reg                                                          0xB801B714
#define set_MIS_PCMCIA_MATC_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_MATC_reg)=data)
#define get_MIS_PCMCIA_MATC_reg   (*((volatile unsigned int*) MIS_PCMCIA_MATC_reg))
#define MIS_PCMCIA_MATC_inst_adr                                                     "0x00C5"
#define MIS_PCMCIA_MATC_inst                                                         0x00C5
#define MIS_PCMCIA_MATC_tc_shift                                                     (24)
#define MIS_PCMCIA_MATC_tc_mask                                                      (0xFF000000)
#define MIS_PCMCIA_MATC_tc(data)                                                     (0xFF000000&((data)<<24))
#define MIS_PCMCIA_MATC_tc_src(data)                                                 ((0xFF000000&(data))>>24)
#define MIS_PCMCIA_MATC_get_tc(data)                                                 ((0xFF000000&(data))>>24)
#define MIS_PCMCIA_MATC_thdio_shift                                                  (20)
#define MIS_PCMCIA_MATC_thdio_mask                                                   (0x00F00000)
#define MIS_PCMCIA_MATC_thdio(data)                                                  (0x00F00000&((data)<<20))
#define MIS_PCMCIA_MATC_thdio_src(data)                                              ((0x00F00000&(data))>>20)
#define MIS_PCMCIA_MATC_get_thdio(data)                                              ((0x00F00000&(data))>>20)
#define MIS_PCMCIA_MATC_tcio_shift                                                   (8)
#define MIS_PCMCIA_MATC_tcio_mask                                                    (0x0001FF00)
#define MIS_PCMCIA_MATC_tcio(data)                                                   (0x0001FF00&((data)<<8))
#define MIS_PCMCIA_MATC_tcio_src(data)                                               ((0x0001FF00&(data))>>8)
#define MIS_PCMCIA_MATC_get_tcio(data)                                               ((0x0001FF00&(data))>>8)
#define MIS_PCMCIA_MATC_twiowr_shift                                                 (0)
#define MIS_PCMCIA_MATC_twiowr_mask                                                  (0x0000003F)
#define MIS_PCMCIA_MATC_twiowr(data)                                                 (0x0000003F&((data)<<0))
#define MIS_PCMCIA_MATC_twiowr_src(data)                                             ((0x0000003F&(data))>>0)
#define MIS_PCMCIA_MATC_get_twiowr(data)                                             ((0x0000003F&(data))>>0)


#define MIS_PCMCIA_ACTRL                                                             0x1801B718
#define MIS_PCMCIA_ACTRL_reg_addr                                                    "0xB801B718"
#define MIS_PCMCIA_ACTRL_reg                                                         0xB801B718
#define set_MIS_PCMCIA_ACTRL_reg(data)   (*((volatile unsigned int*) MIS_PCMCIA_ACTRL_reg)=data)
#define get_MIS_PCMCIA_ACTRL_reg   (*((volatile unsigned int*) MIS_PCMCIA_ACTRL_reg))
#define MIS_PCMCIA_ACTRL_inst_adr                                                    "0x00C6"
#define MIS_PCMCIA_ACTRL_inst                                                        0x00C6
#define MIS_PCMCIA_ACTRL_oe_shift                                                    (0)
#define MIS_PCMCIA_ACTRL_oe_mask                                                     (0x00007FFF)
#define MIS_PCMCIA_ACTRL_oe(data)                                                    (0x00007FFF&((data)<<0))
#define MIS_PCMCIA_ACTRL_oe_src(data)                                                ((0x00007FFF&(data))>>0)
#define MIS_PCMCIA_ACTRL_get_oe(data)                                                ((0x00007FFF&(data))>>0)


#define MIS_SC0_FP                                                                   0x1801BA00
#define MIS_SC0_FP_reg_addr                                                          "0xB801BA00"
#define MIS_SC0_FP_reg                                                               0xB801BA00
#define set_MIS_SC0_FP_reg(data)   (*((volatile unsigned int*) MIS_SC0_FP_reg)=data)
#define get_MIS_SC0_FP_reg   (*((volatile unsigned int*) MIS_SC0_FP_reg))
#define MIS_SC0_FP_inst_adr                                                          "0x0080"
#define MIS_SC0_FP_inst                                                              0x0080
#define MIS_SC0_FP_clk_en_shift                                                      (24)
#define MIS_SC0_FP_clk_en_mask                                                       (0x01000000)
#define MIS_SC0_FP_clk_en(data)                                                      (0x01000000&((data)<<24))
#define MIS_SC0_FP_clk_en_src(data)                                                  ((0x01000000&(data))>>24)
#define MIS_SC0_FP_get_clk_en(data)                                                  ((0x01000000&(data))>>24)
#define MIS_SC0_FP_sc_clkdiv_shift                                                   (18)
#define MIS_SC0_FP_sc_clkdiv_mask                                                    (0x00FC0000)
#define MIS_SC0_FP_sc_clkdiv(data)                                                   (0x00FC0000&((data)<<18))
#define MIS_SC0_FP_sc_clkdiv_src(data)                                               ((0x00FC0000&(data))>>18)
#define MIS_SC0_FP_get_sc_clkdiv(data)                                               ((0x00FC0000&(data))>>18)
#define MIS_SC0_FP_bauddiv2_shift                                                    (16)
#define MIS_SC0_FP_bauddiv2_mask                                                     (0x00030000)
#define MIS_SC0_FP_bauddiv2(data)                                                    (0x00030000&((data)<<16))
#define MIS_SC0_FP_bauddiv2_src(data)                                                ((0x00030000&(data))>>16)
#define MIS_SC0_FP_get_bauddiv2(data)                                                ((0x00030000&(data))>>16)
#define MIS_SC0_FP_bauddiv1_shift                                                    (8)
#define MIS_SC0_FP_bauddiv1_mask                                                     (0x0000FF00)
#define MIS_SC0_FP_bauddiv1(data)                                                    (0x0000FF00&((data)<<8))
#define MIS_SC0_FP_bauddiv1_src(data)                                                ((0x0000FF00&(data))>>8)
#define MIS_SC0_FP_get_bauddiv1(data)                                                ((0x0000FF00&(data))>>8)
#define MIS_SC0_FP_pre_clkdiv_shift                                                  (0)
#define MIS_SC0_FP_pre_clkdiv_mask                                                   (0x000000FF)
#define MIS_SC0_FP_pre_clkdiv(data)                                                  (0x000000FF&((data)<<0))
#define MIS_SC0_FP_pre_clkdiv_src(data)                                              ((0x000000FF&(data))>>0)
#define MIS_SC0_FP_get_pre_clkdiv(data)                                              ((0x000000FF&(data))>>0)


#define MIS_SC0_CR                                                                   0x1801BA04
#define MIS_SC0_CR_reg_addr                                                          "0xB801BA04"
#define MIS_SC0_CR_reg                                                               0xB801BA04
#define set_MIS_SC0_CR_reg(data)   (*((volatile unsigned int*) MIS_SC0_CR_reg)=data)
#define get_MIS_SC0_CR_reg   (*((volatile unsigned int*) MIS_SC0_CR_reg))
#define MIS_SC0_CR_inst_adr                                                          "0x0081"
#define MIS_SC0_CR_inst                                                              0x0081
#define MIS_SC0_CR_fifo_rst_shift                                                    (31)
#define MIS_SC0_CR_fifo_rst_mask                                                     (0x80000000)
#define MIS_SC0_CR_fifo_rst(data)                                                    (0x80000000&((data)<<31))
#define MIS_SC0_CR_fifo_rst_src(data)                                                ((0x80000000&(data))>>31)
#define MIS_SC0_CR_get_fifo_rst(data)                                                ((0x80000000&(data))>>31)
#define MIS_SC0_CR_rst_shift                                                         (30)
#define MIS_SC0_CR_rst_mask                                                          (0x40000000)
#define MIS_SC0_CR_rst(data)                                                         (0x40000000&((data)<<30))
#define MIS_SC0_CR_rst_src(data)                                                     ((0x40000000&(data))>>30)
#define MIS_SC0_CR_get_rst(data)                                                     ((0x40000000&(data))>>30)
#define MIS_SC0_CR_scen_shift                                                        (29)
#define MIS_SC0_CR_scen_mask                                                         (0x20000000)
#define MIS_SC0_CR_scen(data)                                                        (0x20000000&((data)<<29))
#define MIS_SC0_CR_scen_src(data)                                                    ((0x20000000&(data))>>29)
#define MIS_SC0_CR_get_scen(data)                                                    ((0x20000000&(data))>>29)
#define MIS_SC0_CR_tx_go_shift                                                       (28)
#define MIS_SC0_CR_tx_go_mask                                                        (0x10000000)
#define MIS_SC0_CR_tx_go(data)                                                       (0x10000000&((data)<<28))
#define MIS_SC0_CR_tx_go_src(data)                                                   ((0x10000000&(data))>>28)
#define MIS_SC0_CR_get_tx_go(data)                                                   ((0x10000000&(data))>>28)
#define MIS_SC0_CR_auto_atr_shift                                                    (27)
#define MIS_SC0_CR_auto_atr_mask                                                     (0x08000000)
#define MIS_SC0_CR_auto_atr(data)                                                    (0x08000000&((data)<<27))
#define MIS_SC0_CR_auto_atr_src(data)                                                ((0x08000000&(data))>>27)
#define MIS_SC0_CR_get_auto_atr(data)                                                ((0x08000000&(data))>>27)
#define MIS_SC0_CR_conv_shift                                                        (26)
#define MIS_SC0_CR_conv_mask                                                         (0x04000000)
#define MIS_SC0_CR_conv(data)                                                        (0x04000000&((data)<<26))
#define MIS_SC0_CR_conv_src(data)                                                    ((0x04000000&(data))>>26)
#define MIS_SC0_CR_get_conv(data)                                                    ((0x04000000&(data))>>26)
#define MIS_SC0_CR_clk_stop_shift                                                    (25)
#define MIS_SC0_CR_clk_stop_mask                                                     (0x02000000)
#define MIS_SC0_CR_clk_stop(data)                                                    (0x02000000&((data)<<25))
#define MIS_SC0_CR_clk_stop_src(data)                                                ((0x02000000&(data))>>25)
#define MIS_SC0_CR_get_clk_stop(data)                                                ((0x02000000&(data))>>25)
#define MIS_SC0_CR_ps_shift                                                          (24)
#define MIS_SC0_CR_ps_mask                                                           (0x01000000)
#define MIS_SC0_CR_ps(data)                                                          (0x01000000&((data)<<24))
#define MIS_SC0_CR_ps_src(data)                                                      ((0x01000000&(data))>>24)
#define MIS_SC0_CR_get_ps(data)                                                      ((0x01000000&(data))>>24)


#define MIS_SC0_PCR                                                                  0x1801BA08
#define MIS_SC0_PCR_reg_addr                                                         "0xB801BA08"
#define MIS_SC0_PCR_reg                                                              0xB801BA08
#define set_MIS_SC0_PCR_reg(data)   (*((volatile unsigned int*) MIS_SC0_PCR_reg)=data)
#define get_MIS_SC0_PCR_reg   (*((volatile unsigned int*) MIS_SC0_PCR_reg))
#define MIS_SC0_PCR_inst_adr                                                         "0x0082"
#define MIS_SC0_PCR_inst                                                             0x0082
#define MIS_SC0_PCR_txgrdt_shift                                                     (24)
#define MIS_SC0_PCR_txgrdt_mask                                                      (0xFF000000)
#define MIS_SC0_PCR_txgrdt(data)                                                     (0xFF000000&((data)<<24))
#define MIS_SC0_PCR_txgrdt_src(data)                                                 ((0xFF000000&(data))>>24)
#define MIS_SC0_PCR_get_txgrdt(data)                                                 ((0xFF000000&(data))>>24)
#define MIS_SC0_PCR_cwi_shift                                                        (20)
#define MIS_SC0_PCR_cwi_mask                                                         (0x00F00000)
#define MIS_SC0_PCR_cwi(data)                                                        (0x00F00000&((data)<<20))
#define MIS_SC0_PCR_cwi_src(data)                                                    ((0x00F00000&(data))>>20)
#define MIS_SC0_PCR_get_cwi(data)                                                    ((0x00F00000&(data))>>20)
#define MIS_SC0_PCR_bwi_shift                                                        (16)
#define MIS_SC0_PCR_bwi_mask                                                         (0x000F0000)
#define MIS_SC0_PCR_bwi(data)                                                        (0x000F0000&((data)<<16))
#define MIS_SC0_PCR_bwi_src(data)                                                    ((0x000F0000&(data))>>16)
#define MIS_SC0_PCR_get_bwi(data)                                                    ((0x000F0000&(data))>>16)
#define MIS_SC0_PCR_wwi_shift                                                        (12)
#define MIS_SC0_PCR_wwi_mask                                                         (0x0000F000)
#define MIS_SC0_PCR_wwi(data)                                                        (0x0000F000&((data)<<12))
#define MIS_SC0_PCR_wwi_src(data)                                                    ((0x0000F000&(data))>>12)
#define MIS_SC0_PCR_get_wwi(data)                                                    ((0x0000F000&(data))>>12)
#define MIS_SC0_PCR_bgt_shift                                                        (7)
#define MIS_SC0_PCR_bgt_mask                                                         (0x00000F80)
#define MIS_SC0_PCR_bgt(data)                                                        (0x00000F80&((data)<<7))
#define MIS_SC0_PCR_bgt_src(data)                                                    ((0x00000F80&(data))>>7)
#define MIS_SC0_PCR_get_bgt(data)                                                    ((0x00000F80&(data))>>7)
#define MIS_SC0_PCR_edc_en_shift                                                     (6)
#define MIS_SC0_PCR_edc_en_mask                                                      (0x00000040)
#define MIS_SC0_PCR_edc_en(data)                                                     (0x00000040&((data)<<6))
#define MIS_SC0_PCR_edc_en_src(data)                                                 ((0x00000040&(data))>>6)
#define MIS_SC0_PCR_get_edc_en(data)                                                 ((0x00000040&(data))>>6)
#define MIS_SC0_PCR_crc_shift                                                        (5)
#define MIS_SC0_PCR_crc_mask                                                         (0x00000020)
#define MIS_SC0_PCR_crc(data)                                                        (0x00000020&((data)<<5))
#define MIS_SC0_PCR_crc_src(data)                                                    ((0x00000020&(data))>>5)
#define MIS_SC0_PCR_get_crc(data)                                                    ((0x00000020&(data))>>5)
#define MIS_SC0_PCR_protocol_t_shift                                                 (4)
#define MIS_SC0_PCR_protocol_t_mask                                                  (0x00000010)
#define MIS_SC0_PCR_protocol_t(data)                                                 (0x00000010&((data)<<4))
#define MIS_SC0_PCR_protocol_t_src(data)                                             ((0x00000010&(data))>>4)
#define MIS_SC0_PCR_get_protocol_t(data)                                             ((0x00000010&(data))>>4)
#define MIS_SC0_PCR_t0rty_shift                                                      (3)
#define MIS_SC0_PCR_t0rty_mask                                                       (0x00000008)
#define MIS_SC0_PCR_t0rty(data)                                                      (0x00000008&((data)<<3))
#define MIS_SC0_PCR_t0rty_src(data)                                                  ((0x00000008&(data))>>3)
#define MIS_SC0_PCR_get_t0rty(data)                                                  ((0x00000008&(data))>>3)
#define MIS_SC0_PCR_t0rty_cnt_shift                                                  (0)
#define MIS_SC0_PCR_t0rty_cnt_mask                                                   (0x00000007)
#define MIS_SC0_PCR_t0rty_cnt(data)                                                  (0x00000007&((data)<<0))
#define MIS_SC0_PCR_t0rty_cnt_src(data)                                              ((0x00000007&(data))>>0)
#define MIS_SC0_PCR_get_t0rty_cnt(data)                                              ((0x00000007&(data))>>0)


#define MIS_SC0_TXFIFO                                                               0x1801BA0C
#define MIS_SC0_TXFIFO_reg_addr                                                      "0xB801BA0C"
#define MIS_SC0_TXFIFO_reg                                                           0xB801BA0C
#define set_MIS_SC0_TXFIFO_reg(data)   (*((volatile unsigned int*) MIS_SC0_TXFIFO_reg)=data)
#define get_MIS_SC0_TXFIFO_reg   (*((volatile unsigned int*) MIS_SC0_TXFIFO_reg))
#define MIS_SC0_TXFIFO_inst_adr                                                      "0x0083"
#define MIS_SC0_TXFIFO_inst                                                          0x0083
#define MIS_SC0_TXFIFO_tx_fifo_full_shift                                            (8)
#define MIS_SC0_TXFIFO_tx_fifo_full_mask                                             (0x00000100)
#define MIS_SC0_TXFIFO_tx_fifo_full(data)                                            (0x00000100&((data)<<8))
#define MIS_SC0_TXFIFO_tx_fifo_full_src(data)                                        ((0x00000100&(data))>>8)
#define MIS_SC0_TXFIFO_get_tx_fifo_full(data)                                        ((0x00000100&(data))>>8)
#define MIS_SC0_TXFIFO_dat_shift                                                     (0)
#define MIS_SC0_TXFIFO_dat_mask                                                      (0x000000FF)
#define MIS_SC0_TXFIFO_dat(data)                                                     (0x000000FF&((data)<<0))
#define MIS_SC0_TXFIFO_dat_src(data)                                                 ((0x000000FF&(data))>>0)
#define MIS_SC0_TXFIFO_get_dat(data)                                                 ((0x000000FF&(data))>>0)


#define MIS_SC0_RXFIFO                                                               0x1801BA14
#define MIS_SC0_RXFIFO_reg_addr                                                      "0xB801BA14"
#define MIS_SC0_RXFIFO_reg                                                           0xB801BA14
#define set_MIS_SC0_RXFIFO_reg(data)   (*((volatile unsigned int*) MIS_SC0_RXFIFO_reg)=data)
#define get_MIS_SC0_RXFIFO_reg   (*((volatile unsigned int*) MIS_SC0_RXFIFO_reg))
#define MIS_SC0_RXFIFO_inst_adr                                                      "0x0085"
#define MIS_SC0_RXFIFO_inst                                                          0x0085
#define MIS_SC0_RXFIFO_dat_shift                                                     (0)
#define MIS_SC0_RXFIFO_dat_mask                                                      (0x000000FF)
#define MIS_SC0_RXFIFO_dat(data)                                                     (0x000000FF&((data)<<0))
#define MIS_SC0_RXFIFO_dat_src(data)                                                 ((0x000000FF&(data))>>0)
#define MIS_SC0_RXFIFO_get_dat(data)                                                 ((0x000000FF&(data))>>0)


#define MIS_SC0_RXLENR                                                               0x1801BA18
#define MIS_SC0_RXLENR_reg_addr                                                      "0xB801BA18"
#define MIS_SC0_RXLENR_reg                                                           0xB801BA18
#define set_MIS_SC0_RXLENR_reg(data)   (*((volatile unsigned int*) MIS_SC0_RXLENR_reg)=data)
#define get_MIS_SC0_RXLENR_reg   (*((volatile unsigned int*) MIS_SC0_RXLENR_reg))
#define MIS_SC0_RXLENR_inst_adr                                                      "0x0086"
#define MIS_SC0_RXLENR_inst                                                          0x0086
#define MIS_SC0_RXLENR_rxlen_shift                                                   (0)
#define MIS_SC0_RXLENR_rxlen_mask                                                    (0x000000FF)
#define MIS_SC0_RXLENR_rxlen(data)                                                   (0x000000FF&((data)<<0))
#define MIS_SC0_RXLENR_rxlen_src(data)                                               ((0x000000FF&(data))>>0)
#define MIS_SC0_RXLENR_get_rxlen(data)                                               ((0x000000FF&(data))>>0)


#define MIS_SC0_FCR                                                                  0x1801BA1C
#define MIS_SC0_FCR_reg_addr                                                         "0xB801BA1C"
#define MIS_SC0_FCR_reg                                                              0xB801BA1C
#define set_MIS_SC0_FCR_reg(data)   (*((volatile unsigned int*) MIS_SC0_FCR_reg)=data)
#define get_MIS_SC0_FCR_reg   (*((volatile unsigned int*) MIS_SC0_FCR_reg))
#define MIS_SC0_FCR_inst_adr                                                         "0x0087"
#define MIS_SC0_FCR_inst                                                             0x0087
#define MIS_SC0_FCR_rxflow_shift                                                     (1)
#define MIS_SC0_FCR_rxflow_mask                                                      (0x00000002)
#define MIS_SC0_FCR_rxflow(data)                                                     (0x00000002&((data)<<1))
#define MIS_SC0_FCR_rxflow_src(data)                                                 ((0x00000002&(data))>>1)
#define MIS_SC0_FCR_get_rxflow(data)                                                 ((0x00000002&(data))>>1)
#define MIS_SC0_FCR_flow_en_shift                                                    (0)
#define MIS_SC0_FCR_flow_en_mask                                                     (0x00000001)
#define MIS_SC0_FCR_flow_en(data)                                                    (0x00000001&((data)<<0))
#define MIS_SC0_FCR_flow_en_src(data)                                                ((0x00000001&(data))>>0)
#define MIS_SC0_FCR_get_flow_en(data)                                                ((0x00000001&(data))>>0)


#define MIS_SC0_IRSR                                                                 0x1801BA20
#define MIS_SC0_IRSR_reg_addr                                                        "0xB801BA20"
#define MIS_SC0_IRSR_reg                                                             0xB801BA20
#define set_MIS_SC0_IRSR_reg(data)   (*((volatile unsigned int*) MIS_SC0_IRSR_reg)=data)
#define get_MIS_SC0_IRSR_reg   (*((volatile unsigned int*) MIS_SC0_IRSR_reg))
#define MIS_SC0_IRSR_inst_adr                                                        "0x0088"
#define MIS_SC0_IRSR_inst                                                            0x0088
#define MIS_SC0_IRSR_pres_shift                                                      (16)
#define MIS_SC0_IRSR_pres_mask                                                       (0x00010000)
#define MIS_SC0_IRSR_pres(data)                                                      (0x00010000&((data)<<16))
#define MIS_SC0_IRSR_pres_src(data)                                                  ((0x00010000&(data))>>16)
#define MIS_SC0_IRSR_get_pres(data)                                                  ((0x00010000&(data))>>16)
#define MIS_SC0_IRSR_cpres_int_shift                                                 (15)
#define MIS_SC0_IRSR_cpres_int_mask                                                  (0x00008000)
#define MIS_SC0_IRSR_cpres_int(data)                                                 (0x00008000&((data)<<15))
#define MIS_SC0_IRSR_cpres_int_src(data)                                             ((0x00008000&(data))>>15)
#define MIS_SC0_IRSR_get_cpres_int(data)                                             ((0x00008000&(data))>>15)
#define MIS_SC0_IRSR_tx_flow_int_shift                                               (14)
#define MIS_SC0_IRSR_tx_flow_int_mask                                                (0x00004000)
#define MIS_SC0_IRSR_tx_flow_int(data)                                               (0x00004000&((data)<<14))
#define MIS_SC0_IRSR_tx_flow_int_src(data)                                           ((0x00004000&(data))>>14)
#define MIS_SC0_IRSR_get_tx_flow_int(data)                                           ((0x00004000&(data))>>14)
#define MIS_SC0_IRSR_txp_int_shift                                                   (13)
#define MIS_SC0_IRSR_txp_int_mask                                                    (0x00002000)
#define MIS_SC0_IRSR_txp_int(data)                                                   (0x00002000&((data)<<13))
#define MIS_SC0_IRSR_txp_int_src(data)                                               ((0x00002000&(data))>>13)
#define MIS_SC0_IRSR_get_txp_int(data)                                               ((0x00002000&(data))>>13)
#define MIS_SC0_IRSR_txdone_int_shift                                                (12)
#define MIS_SC0_IRSR_txdone_int_mask                                                 (0x00001000)
#define MIS_SC0_IRSR_txdone_int(data)                                                (0x00001000&((data)<<12))
#define MIS_SC0_IRSR_txdone_int_src(data)                                            ((0x00001000&(data))>>12)
#define MIS_SC0_IRSR_get_txdone_int(data)                                            ((0x00001000&(data))>>12)
#define MIS_SC0_IRSR_txempty_int_shift                                               (11)
#define MIS_SC0_IRSR_txempty_int_mask                                                (0x00000800)
#define MIS_SC0_IRSR_txempty_int(data)                                               (0x00000800&((data)<<11))
#define MIS_SC0_IRSR_txempty_int_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_SC0_IRSR_get_txempty_int(data)                                           ((0x00000800&(data))>>11)
#define MIS_SC0_IRSR_edcerr_int_shift                                                (10)
#define MIS_SC0_IRSR_edcerr_int_mask                                                 (0x00000400)
#define MIS_SC0_IRSR_edcerr_int(data)                                                (0x00000400&((data)<<10))
#define MIS_SC0_IRSR_edcerr_int_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_SC0_IRSR_get_edcerr_int(data)                                            ((0x00000400&(data))>>10)
#define MIS_SC0_IRSR_rx_fover_shift                                                  (9)
#define MIS_SC0_IRSR_rx_fover_mask                                                   (0x00000200)
#define MIS_SC0_IRSR_rx_fover(data)                                                  (0x00000200&((data)<<9))
#define MIS_SC0_IRSR_rx_fover_src(data)                                              ((0x00000200&(data))>>9)
#define MIS_SC0_IRSR_get_rx_fover(data)                                              ((0x00000200&(data))>>9)
#define MIS_SC0_IRSR_rxp_int_shift                                                   (8)
#define MIS_SC0_IRSR_rxp_int_mask                                                    (0x00000100)
#define MIS_SC0_IRSR_rxp_int(data)                                                   (0x00000100&((data)<<8))
#define MIS_SC0_IRSR_rxp_int_src(data)                                               ((0x00000100&(data))>>8)
#define MIS_SC0_IRSR_get_rxp_int(data)                                               ((0x00000100&(data))>>8)
#define MIS_SC0_IRSR_atrs_int_shift                                                  (7)
#define MIS_SC0_IRSR_atrs_int_mask                                                   (0x00000080)
#define MIS_SC0_IRSR_atrs_int(data)                                                  (0x00000080&((data)<<7))
#define MIS_SC0_IRSR_atrs_int_src(data)                                              ((0x00000080&(data))>>7)
#define MIS_SC0_IRSR_get_atrs_int(data)                                              ((0x00000080&(data))>>7)
#define MIS_SC0_IRSR_bgt_int_shift                                                   (6)
#define MIS_SC0_IRSR_bgt_int_mask                                                    (0x00000040)
#define MIS_SC0_IRSR_bgt_int(data)                                                   (0x00000040&((data)<<6))
#define MIS_SC0_IRSR_bgt_int_src(data)                                               ((0x00000040&(data))>>6)
#define MIS_SC0_IRSR_get_bgt_int(data)                                               ((0x00000040&(data))>>6)
#define MIS_SC0_IRSR_cwt_int_shift                                                   (5)
#define MIS_SC0_IRSR_cwt_int_mask                                                    (0x00000020)
#define MIS_SC0_IRSR_cwt_int(data)                                                   (0x00000020&((data)<<5))
#define MIS_SC0_IRSR_cwt_int_src(data)                                               ((0x00000020&(data))>>5)
#define MIS_SC0_IRSR_get_cwt_int(data)                                               ((0x00000020&(data))>>5)
#define MIS_SC0_IRSR_rlen_int_shift                                                  (4)
#define MIS_SC0_IRSR_rlen_int_mask                                                   (0x00000010)
#define MIS_SC0_IRSR_rlen_int(data)                                                  (0x00000010&((data)<<4))
#define MIS_SC0_IRSR_rlen_int_src(data)                                              ((0x00000010&(data))>>4)
#define MIS_SC0_IRSR_get_rlen_int(data)                                              ((0x00000010&(data))>>4)
#define MIS_SC0_IRSR_wwt_int_shift                                                   (3)
#define MIS_SC0_IRSR_wwt_int_mask                                                    (0x00000008)
#define MIS_SC0_IRSR_wwt_int(data)                                                   (0x00000008&((data)<<3))
#define MIS_SC0_IRSR_wwt_int_src(data)                                               ((0x00000008&(data))>>3)
#define MIS_SC0_IRSR_get_wwt_int(data)                                               ((0x00000008&(data))>>3)
#define MIS_SC0_IRSR_bwt_int_shift                                                   (2)
#define MIS_SC0_IRSR_bwt_int_mask                                                    (0x00000004)
#define MIS_SC0_IRSR_bwt_int(data)                                                   (0x00000004&((data)<<2))
#define MIS_SC0_IRSR_bwt_int_src(data)                                               ((0x00000004&(data))>>2)
#define MIS_SC0_IRSR_get_bwt_int(data)                                               ((0x00000004&(data))>>2)
#define MIS_SC0_IRSR_rcv_int_shift                                                   (1)
#define MIS_SC0_IRSR_rcv_int_mask                                                    (0x00000002)
#define MIS_SC0_IRSR_rcv_int(data)                                                   (0x00000002&((data)<<1))
#define MIS_SC0_IRSR_rcv_int_src(data)                                               ((0x00000002&(data))>>1)
#define MIS_SC0_IRSR_get_rcv_int(data)                                               ((0x00000002&(data))>>1)
#define MIS_SC0_IRSR_drdy_int_shift                                                  (0)
#define MIS_SC0_IRSR_drdy_int_mask                                                   (0x00000001)
#define MIS_SC0_IRSR_drdy_int(data)                                                  (0x00000001&((data)<<0))
#define MIS_SC0_IRSR_drdy_int_src(data)                                              ((0x00000001&(data))>>0)
#define MIS_SC0_IRSR_get_drdy_int(data)                                              ((0x00000001&(data))>>0)


#define MIS_SC0_IRER                                                                 0x1801BA24
#define MIS_SC0_IRER_reg_addr                                                        "0xB801BA24"
#define MIS_SC0_IRER_reg                                                             0xB801BA24
#define set_MIS_SC0_IRER_reg(data)   (*((volatile unsigned int*) MIS_SC0_IRER_reg)=data)
#define get_MIS_SC0_IRER_reg   (*((volatile unsigned int*) MIS_SC0_IRER_reg))
#define MIS_SC0_IRER_inst_adr                                                        "0x0089"
#define MIS_SC0_IRER_inst                                                            0x0089
#define MIS_SC0_IRER_cpres_en_shift                                                  (15)
#define MIS_SC0_IRER_cpres_en_mask                                                   (0x00008000)
#define MIS_SC0_IRER_cpres_en(data)                                                  (0x00008000&((data)<<15))
#define MIS_SC0_IRER_cpres_en_src(data)                                              ((0x00008000&(data))>>15)
#define MIS_SC0_IRER_get_cpres_en(data)                                              ((0x00008000&(data))>>15)
#define MIS_SC0_IRER_txflow_int_en_shift                                             (14)
#define MIS_SC0_IRER_txflow_int_en_mask                                              (0x00004000)
#define MIS_SC0_IRER_txflow_int_en(data)                                             (0x00004000&((data)<<14))
#define MIS_SC0_IRER_txflow_int_en_src(data)                                         ((0x00004000&(data))>>14)
#define MIS_SC0_IRER_get_txflow_int_en(data)                                         ((0x00004000&(data))>>14)
#define MIS_SC0_IRER_txp_en_shift                                                    (13)
#define MIS_SC0_IRER_txp_en_mask                                                     (0x00002000)
#define MIS_SC0_IRER_txp_en(data)                                                    (0x00002000&((data)<<13))
#define MIS_SC0_IRER_txp_en_src(data)                                                ((0x00002000&(data))>>13)
#define MIS_SC0_IRER_get_txp_en(data)                                                ((0x00002000&(data))>>13)
#define MIS_SC0_IRER_txdone_en_shift                                                 (12)
#define MIS_SC0_IRER_txdone_en_mask                                                  (0x00001000)
#define MIS_SC0_IRER_txdone_en(data)                                                 (0x00001000&((data)<<12))
#define MIS_SC0_IRER_txdone_en_src(data)                                             ((0x00001000&(data))>>12)
#define MIS_SC0_IRER_get_txdone_en(data)                                             ((0x00001000&(data))>>12)
#define MIS_SC0_IRER_txempty_en_shift                                                (11)
#define MIS_SC0_IRER_txempty_en_mask                                                 (0x00000800)
#define MIS_SC0_IRER_txempty_en(data)                                                (0x00000800&((data)<<11))
#define MIS_SC0_IRER_txempty_en_src(data)                                            ((0x00000800&(data))>>11)
#define MIS_SC0_IRER_get_txempty_en(data)                                            ((0x00000800&(data))>>11)
#define MIS_SC0_IRER_edcerr_en_shift                                                 (10)
#define MIS_SC0_IRER_edcerr_en_mask                                                  (0x00000400)
#define MIS_SC0_IRER_edcerr_en(data)                                                 (0x00000400&((data)<<10))
#define MIS_SC0_IRER_edcerr_en_src(data)                                             ((0x00000400&(data))>>10)
#define MIS_SC0_IRER_get_edcerr_en(data)                                             ((0x00000400&(data))>>10)
#define MIS_SC0_IRER_rx_fover_en_shift                                               (9)
#define MIS_SC0_IRER_rx_fover_en_mask                                                (0x00000200)
#define MIS_SC0_IRER_rx_fover_en(data)                                               (0x00000200&((data)<<9))
#define MIS_SC0_IRER_rx_fover_en_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_SC0_IRER_get_rx_fover_en(data)                                           ((0x00000200&(data))>>9)
#define MIS_SC0_IRER_rxp_en_shift                                                    (8)
#define MIS_SC0_IRER_rxp_en_mask                                                     (0x00000100)
#define MIS_SC0_IRER_rxp_en(data)                                                    (0x00000100&((data)<<8))
#define MIS_SC0_IRER_rxp_en_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_SC0_IRER_get_rxp_en(data)                                                ((0x00000100&(data))>>8)
#define MIS_SC0_IRER_atrs_en_shift                                                   (7)
#define MIS_SC0_IRER_atrs_en_mask                                                    (0x00000080)
#define MIS_SC0_IRER_atrs_en(data)                                                   (0x00000080&((data)<<7))
#define MIS_SC0_IRER_atrs_en_src(data)                                               ((0x00000080&(data))>>7)
#define MIS_SC0_IRER_get_atrs_en(data)                                               ((0x00000080&(data))>>7)
#define MIS_SC0_IRER_bgt_en_shift                                                    (6)
#define MIS_SC0_IRER_bgt_en_mask                                                     (0x00000040)
#define MIS_SC0_IRER_bgt_en(data)                                                    (0x00000040&((data)<<6))
#define MIS_SC0_IRER_bgt_en_src(data)                                                ((0x00000040&(data))>>6)
#define MIS_SC0_IRER_get_bgt_en(data)                                                ((0x00000040&(data))>>6)
#define MIS_SC0_IRER_cwt_en_shift                                                    (5)
#define MIS_SC0_IRER_cwt_en_mask                                                     (0x00000020)
#define MIS_SC0_IRER_cwt_en(data)                                                    (0x00000020&((data)<<5))
#define MIS_SC0_IRER_cwt_en_src(data)                                                ((0x00000020&(data))>>5)
#define MIS_SC0_IRER_get_cwt_en(data)                                                ((0x00000020&(data))>>5)
#define MIS_SC0_IRER_rlen_en_shift                                                   (4)
#define MIS_SC0_IRER_rlen_en_mask                                                    (0x00000010)
#define MIS_SC0_IRER_rlen_en(data)                                                   (0x00000010&((data)<<4))
#define MIS_SC0_IRER_rlen_en_src(data)                                               ((0x00000010&(data))>>4)
#define MIS_SC0_IRER_get_rlen_en(data)                                               ((0x00000010&(data))>>4)
#define MIS_SC0_IRER_wwt_en_shift                                                    (3)
#define MIS_SC0_IRER_wwt_en_mask                                                     (0x00000008)
#define MIS_SC0_IRER_wwt_en(data)                                                    (0x00000008&((data)<<3))
#define MIS_SC0_IRER_wwt_en_src(data)                                                ((0x00000008&(data))>>3)
#define MIS_SC0_IRER_get_wwt_en(data)                                                ((0x00000008&(data))>>3)
#define MIS_SC0_IRER_bwt_en_shift                                                    (2)
#define MIS_SC0_IRER_bwt_en_mask                                                     (0x00000004)
#define MIS_SC0_IRER_bwt_en(data)                                                    (0x00000004&((data)<<2))
#define MIS_SC0_IRER_bwt_en_src(data)                                                ((0x00000004&(data))>>2)
#define MIS_SC0_IRER_get_bwt_en(data)                                                ((0x00000004&(data))>>2)
#define MIS_SC0_IRER_rcv_en_shift                                                    (1)
#define MIS_SC0_IRER_rcv_en_mask                                                     (0x00000002)
#define MIS_SC0_IRER_rcv_en(data)                                                    (0x00000002&((data)<<1))
#define MIS_SC0_IRER_rcv_en_src(data)                                                ((0x00000002&(data))>>1)
#define MIS_SC0_IRER_get_rcv_en(data)                                                ((0x00000002&(data))>>1)
#define MIS_SC0_IRER_drdy_en_shift                                                   (0)
#define MIS_SC0_IRER_drdy_en_mask                                                    (0x00000001)
#define MIS_SC0_IRER_drdy_en(data)                                                   (0x00000001&((data)<<0))
#define MIS_SC0_IRER_drdy_en_src(data)                                               ((0x00000001&(data))>>0)
#define MIS_SC0_IRER_get_drdy_en(data)                                               ((0x00000001&(data))>>0)


#define MIS_SC1_FP                                                                   0x1801BA80
#define MIS_SC1_FP_reg_addr                                                          "0xB801BA80"
#define MIS_SC1_FP_reg                                                               0xB801BA80
#define set_MIS_SC1_FP_reg(data)   (*((volatile unsigned int*) MIS_SC1_FP_reg)=data)
#define get_MIS_SC1_FP_reg   (*((volatile unsigned int*) MIS_SC1_FP_reg))
#define MIS_SC1_FP_inst_adr                                                          "0x00A0"
#define MIS_SC1_FP_inst                                                              0x00A0
#define MIS_SC1_FP_clk_en_shift                                                      (24)
#define MIS_SC1_FP_clk_en_mask                                                       (0x01000000)
#define MIS_SC1_FP_clk_en(data)                                                      (0x01000000&((data)<<24))
#define MIS_SC1_FP_clk_en_src(data)                                                  ((0x01000000&(data))>>24)
#define MIS_SC1_FP_get_clk_en(data)                                                  ((0x01000000&(data))>>24)
#define MIS_SC1_FP_sc_clkdiv_shift                                                   (18)
#define MIS_SC1_FP_sc_clkdiv_mask                                                    (0x00FC0000)
#define MIS_SC1_FP_sc_clkdiv(data)                                                   (0x00FC0000&((data)<<18))
#define MIS_SC1_FP_sc_clkdiv_src(data)                                               ((0x00FC0000&(data))>>18)
#define MIS_SC1_FP_get_sc_clkdiv(data)                                               ((0x00FC0000&(data))>>18)
#define MIS_SC1_FP_bauddiv2_shift                                                    (16)
#define MIS_SC1_FP_bauddiv2_mask                                                     (0x00030000)
#define MIS_SC1_FP_bauddiv2(data)                                                    (0x00030000&((data)<<16))
#define MIS_SC1_FP_bauddiv2_src(data)                                                ((0x00030000&(data))>>16)
#define MIS_SC1_FP_get_bauddiv2(data)                                                ((0x00030000&(data))>>16)
#define MIS_SC1_FP_bauddiv1_shift                                                    (8)
#define MIS_SC1_FP_bauddiv1_mask                                                     (0x0000FF00)
#define MIS_SC1_FP_bauddiv1(data)                                                    (0x0000FF00&((data)<<8))
#define MIS_SC1_FP_bauddiv1_src(data)                                                ((0x0000FF00&(data))>>8)
#define MIS_SC1_FP_get_bauddiv1(data)                                                ((0x0000FF00&(data))>>8)
#define MIS_SC1_FP_pre_clkdiv_shift                                                  (0)
#define MIS_SC1_FP_pre_clkdiv_mask                                                   (0x000000FF)
#define MIS_SC1_FP_pre_clkdiv(data)                                                  (0x000000FF&((data)<<0))
#define MIS_SC1_FP_pre_clkdiv_src(data)                                              ((0x000000FF&(data))>>0)
#define MIS_SC1_FP_get_pre_clkdiv(data)                                              ((0x000000FF&(data))>>0)


#define MIS_SC1_CR                                                                   0x1801BA84
#define MIS_SC1_CR_reg_addr                                                          "0xB801BA84"
#define MIS_SC1_CR_reg                                                               0xB801BA84
#define set_MIS_SC1_CR_reg(data)   (*((volatile unsigned int*) MIS_SC1_CR_reg)=data)
#define get_MIS_SC1_CR_reg   (*((volatile unsigned int*) MIS_SC1_CR_reg))
#define MIS_SC1_CR_inst_adr                                                          "0x00A1"
#define MIS_SC1_CR_inst                                                              0x00A1
#define MIS_SC1_CR_fifo_rst_shift                                                    (31)
#define MIS_SC1_CR_fifo_rst_mask                                                     (0x80000000)
#define MIS_SC1_CR_fifo_rst(data)                                                    (0x80000000&((data)<<31))
#define MIS_SC1_CR_fifo_rst_src(data)                                                ((0x80000000&(data))>>31)
#define MIS_SC1_CR_get_fifo_rst(data)                                                ((0x80000000&(data))>>31)
#define MIS_SC1_CR_rst_shift                                                         (30)
#define MIS_SC1_CR_rst_mask                                                          (0x40000000)
#define MIS_SC1_CR_rst(data)                                                         (0x40000000&((data)<<30))
#define MIS_SC1_CR_rst_src(data)                                                     ((0x40000000&(data))>>30)
#define MIS_SC1_CR_get_rst(data)                                                     ((0x40000000&(data))>>30)
#define MIS_SC1_CR_scen_shift                                                        (29)
#define MIS_SC1_CR_scen_mask                                                         (0x20000000)
#define MIS_SC1_CR_scen(data)                                                        (0x20000000&((data)<<29))
#define MIS_SC1_CR_scen_src(data)                                                    ((0x20000000&(data))>>29)
#define MIS_SC1_CR_get_scen(data)                                                    ((0x20000000&(data))>>29)
#define MIS_SC1_CR_tx_go_shift                                                       (28)
#define MIS_SC1_CR_tx_go_mask                                                        (0x10000000)
#define MIS_SC1_CR_tx_go(data)                                                       (0x10000000&((data)<<28))
#define MIS_SC1_CR_tx_go_src(data)                                                   ((0x10000000&(data))>>28)
#define MIS_SC1_CR_get_tx_go(data)                                                   ((0x10000000&(data))>>28)
#define MIS_SC1_CR_auto_atr_shift                                                    (27)
#define MIS_SC1_CR_auto_atr_mask                                                     (0x08000000)
#define MIS_SC1_CR_auto_atr(data)                                                    (0x08000000&((data)<<27))
#define MIS_SC1_CR_auto_atr_src(data)                                                ((0x08000000&(data))>>27)
#define MIS_SC1_CR_get_auto_atr(data)                                                ((0x08000000&(data))>>27)
#define MIS_SC1_CR_conv_shift                                                        (26)
#define MIS_SC1_CR_conv_mask                                                         (0x04000000)
#define MIS_SC1_CR_conv(data)                                                        (0x04000000&((data)<<26))
#define MIS_SC1_CR_conv_src(data)                                                    ((0x04000000&(data))>>26)
#define MIS_SC1_CR_get_conv(data)                                                    ((0x04000000&(data))>>26)
#define MIS_SC1_CR_clk_stop_shift                                                    (25)
#define MIS_SC1_CR_clk_stop_mask                                                     (0x02000000)
#define MIS_SC1_CR_clk_stop(data)                                                    (0x02000000&((data)<<25))
#define MIS_SC1_CR_clk_stop_src(data)                                                ((0x02000000&(data))>>25)
#define MIS_SC1_CR_get_clk_stop(data)                                                ((0x02000000&(data))>>25)
#define MIS_SC1_CR_ps_shift                                                          (24)
#define MIS_SC1_CR_ps_mask                                                           (0x01000000)
#define MIS_SC1_CR_ps(data)                                                          (0x01000000&((data)<<24))
#define MIS_SC1_CR_ps_src(data)                                                      ((0x01000000&(data))>>24)
#define MIS_SC1_CR_get_ps(data)                                                      ((0x01000000&(data))>>24)


#define MIS_SC1_PCR                                                                  0x1801BA88
#define MIS_SC1_PCR_reg_addr                                                         "0xB801BA88"
#define MIS_SC1_PCR_reg                                                              0xB801BA88
#define set_MIS_SC1_PCR_reg(data)   (*((volatile unsigned int*) MIS_SC1_PCR_reg)=data)
#define get_MIS_SC1_PCR_reg   (*((volatile unsigned int*) MIS_SC1_PCR_reg))
#define MIS_SC1_PCR_inst_adr                                                         "0x00A2"
#define MIS_SC1_PCR_inst                                                             0x00A2
#define MIS_SC1_PCR_txgrdt_shift                                                     (24)
#define MIS_SC1_PCR_txgrdt_mask                                                      (0xFF000000)
#define MIS_SC1_PCR_txgrdt(data)                                                     (0xFF000000&((data)<<24))
#define MIS_SC1_PCR_txgrdt_src(data)                                                 ((0xFF000000&(data))>>24)
#define MIS_SC1_PCR_get_txgrdt(data)                                                 ((0xFF000000&(data))>>24)
#define MIS_SC1_PCR_cwi_shift                                                        (20)
#define MIS_SC1_PCR_cwi_mask                                                         (0x00F00000)
#define MIS_SC1_PCR_cwi(data)                                                        (0x00F00000&((data)<<20))
#define MIS_SC1_PCR_cwi_src(data)                                                    ((0x00F00000&(data))>>20)
#define MIS_SC1_PCR_get_cwi(data)                                                    ((0x00F00000&(data))>>20)
#define MIS_SC1_PCR_bwi_shift                                                        (16)
#define MIS_SC1_PCR_bwi_mask                                                         (0x000F0000)
#define MIS_SC1_PCR_bwi(data)                                                        (0x000F0000&((data)<<16))
#define MIS_SC1_PCR_bwi_src(data)                                                    ((0x000F0000&(data))>>16)
#define MIS_SC1_PCR_get_bwi(data)                                                    ((0x000F0000&(data))>>16)
#define MIS_SC1_PCR_wwi_shift                                                        (12)
#define MIS_SC1_PCR_wwi_mask                                                         (0x0000F000)
#define MIS_SC1_PCR_wwi(data)                                                        (0x0000F000&((data)<<12))
#define MIS_SC1_PCR_wwi_src(data)                                                    ((0x0000F000&(data))>>12)
#define MIS_SC1_PCR_get_wwi(data)                                                    ((0x0000F000&(data))>>12)
#define MIS_SC1_PCR_bgt_shift                                                        (7)
#define MIS_SC1_PCR_bgt_mask                                                         (0x00000F80)
#define MIS_SC1_PCR_bgt(data)                                                        (0x00000F80&((data)<<7))
#define MIS_SC1_PCR_bgt_src(data)                                                    ((0x00000F80&(data))>>7)
#define MIS_SC1_PCR_get_bgt(data)                                                    ((0x00000F80&(data))>>7)
#define MIS_SC1_PCR_edc_en_shift                                                     (6)
#define MIS_SC1_PCR_edc_en_mask                                                      (0x00000040)
#define MIS_SC1_PCR_edc_en(data)                                                     (0x00000040&((data)<<6))
#define MIS_SC1_PCR_edc_en_src(data)                                                 ((0x00000040&(data))>>6)
#define MIS_SC1_PCR_get_edc_en(data)                                                 ((0x00000040&(data))>>6)
#define MIS_SC1_PCR_crc_shift                                                        (5)
#define MIS_SC1_PCR_crc_mask                                                         (0x00000020)
#define MIS_SC1_PCR_crc(data)                                                        (0x00000020&((data)<<5))
#define MIS_SC1_PCR_crc_src(data)                                                    ((0x00000020&(data))>>5)
#define MIS_SC1_PCR_get_crc(data)                                                    ((0x00000020&(data))>>5)
#define MIS_SC1_PCR_protocol_t_shift                                                 (4)
#define MIS_SC1_PCR_protocol_t_mask                                                  (0x00000010)
#define MIS_SC1_PCR_protocol_t(data)                                                 (0x00000010&((data)<<4))
#define MIS_SC1_PCR_protocol_t_src(data)                                             ((0x00000010&(data))>>4)
#define MIS_SC1_PCR_get_protocol_t(data)                                             ((0x00000010&(data))>>4)
#define MIS_SC1_PCR_t0rty_shift                                                      (3)
#define MIS_SC1_PCR_t0rty_mask                                                       (0x00000008)
#define MIS_SC1_PCR_t0rty(data)                                                      (0x00000008&((data)<<3))
#define MIS_SC1_PCR_t0rty_src(data)                                                  ((0x00000008&(data))>>3)
#define MIS_SC1_PCR_get_t0rty(data)                                                  ((0x00000008&(data))>>3)
#define MIS_SC1_PCR_t0rty_cnt_shift                                                  (0)
#define MIS_SC1_PCR_t0rty_cnt_mask                                                   (0x00000007)
#define MIS_SC1_PCR_t0rty_cnt(data)                                                  (0x00000007&((data)<<0))
#define MIS_SC1_PCR_t0rty_cnt_src(data)                                              ((0x00000007&(data))>>0)
#define MIS_SC1_PCR_get_t0rty_cnt(data)                                              ((0x00000007&(data))>>0)


#define MIS_SC1_TXFIFO                                                               0x1801BA8C
#define MIS_SC1_TXFIFO_reg_addr                                                      "0xB801BA8C"
#define MIS_SC1_TXFIFO_reg                                                           0xB801BA8C
#define set_MIS_SC1_TXFIFO_reg(data)   (*((volatile unsigned int*) MIS_SC1_TXFIFO_reg)=data)
#define get_MIS_SC1_TXFIFO_reg   (*((volatile unsigned int*) MIS_SC1_TXFIFO_reg))
#define MIS_SC1_TXFIFO_inst_adr                                                      "0x00A3"
#define MIS_SC1_TXFIFO_inst                                                          0x00A3
#define MIS_SC1_TXFIFO_tx_fifo_full_shift                                            (8)
#define MIS_SC1_TXFIFO_tx_fifo_full_mask                                             (0x00000100)
#define MIS_SC1_TXFIFO_tx_fifo_full(data)                                            (0x00000100&((data)<<8))
#define MIS_SC1_TXFIFO_tx_fifo_full_src(data)                                        ((0x00000100&(data))>>8)
#define MIS_SC1_TXFIFO_get_tx_fifo_full(data)                                        ((0x00000100&(data))>>8)
#define MIS_SC1_TXFIFO_dat_shift                                                     (0)
#define MIS_SC1_TXFIFO_dat_mask                                                      (0x000000FF)
#define MIS_SC1_TXFIFO_dat(data)                                                     (0x000000FF&((data)<<0))
#define MIS_SC1_TXFIFO_dat_src(data)                                                 ((0x000000FF&(data))>>0)
#define MIS_SC1_TXFIFO_get_dat(data)                                                 ((0x000000FF&(data))>>0)


#define MIS_SC1_RXFIFO                                                               0x1801BA94
#define MIS_SC1_RXFIFO_reg_addr                                                      "0xB801BA94"
#define MIS_SC1_RXFIFO_reg                                                           0xB801BA94
#define set_MIS_SC1_RXFIFO_reg(data)   (*((volatile unsigned int*) MIS_SC1_RXFIFO_reg)=data)
#define get_MIS_SC1_RXFIFO_reg   (*((volatile unsigned int*) MIS_SC1_RXFIFO_reg))
#define MIS_SC1_RXFIFO_inst_adr                                                      "0x00A5"
#define MIS_SC1_RXFIFO_inst                                                          0x00A5
#define MIS_SC1_RXFIFO_dat_shift                                                     (0)
#define MIS_SC1_RXFIFO_dat_mask                                                      (0x000000FF)
#define MIS_SC1_RXFIFO_dat(data)                                                     (0x000000FF&((data)<<0))
#define MIS_SC1_RXFIFO_dat_src(data)                                                 ((0x000000FF&(data))>>0)
#define MIS_SC1_RXFIFO_get_dat(data)                                                 ((0x000000FF&(data))>>0)


#define MIS_SC1_RXLENR                                                               0x1801BA98
#define MIS_SC1_RXLENR_reg_addr                                                      "0xB801BA98"
#define MIS_SC1_RXLENR_reg                                                           0xB801BA98
#define set_MIS_SC1_RXLENR_reg(data)   (*((volatile unsigned int*) MIS_SC1_RXLENR_reg)=data)
#define get_MIS_SC1_RXLENR_reg   (*((volatile unsigned int*) MIS_SC1_RXLENR_reg))
#define MIS_SC1_RXLENR_inst_adr                                                      "0x00A6"
#define MIS_SC1_RXLENR_inst                                                          0x00A6
#define MIS_SC1_RXLENR_rxlen_shift                                                   (0)
#define MIS_SC1_RXLENR_rxlen_mask                                                    (0x000000FF)
#define MIS_SC1_RXLENR_rxlen(data)                                                   (0x000000FF&((data)<<0))
#define MIS_SC1_RXLENR_rxlen_src(data)                                               ((0x000000FF&(data))>>0)
#define MIS_SC1_RXLENR_get_rxlen(data)                                               ((0x000000FF&(data))>>0)


#define MIS_SC1_FCR                                                                  0x1801BA9C
#define MIS_SC1_FCR_reg_addr                                                         "0xB801BA9C"
#define MIS_SC1_FCR_reg                                                              0xB801BA9C
#define set_MIS_SC1_FCR_reg(data)   (*((volatile unsigned int*) MIS_SC1_FCR_reg)=data)
#define get_MIS_SC1_FCR_reg   (*((volatile unsigned int*) MIS_SC1_FCR_reg))
#define MIS_SC1_FCR_inst_adr                                                         "0x00A7"
#define MIS_SC1_FCR_inst                                                             0x00A7
#define MIS_SC1_FCR_rxflow_shift                                                     (1)
#define MIS_SC1_FCR_rxflow_mask                                                      (0x00000002)
#define MIS_SC1_FCR_rxflow(data)                                                     (0x00000002&((data)<<1))
#define MIS_SC1_FCR_rxflow_src(data)                                                 ((0x00000002&(data))>>1)
#define MIS_SC1_FCR_get_rxflow(data)                                                 ((0x00000002&(data))>>1)
#define MIS_SC1_FCR_flow_en_shift                                                    (0)
#define MIS_SC1_FCR_flow_en_mask                                                     (0x00000001)
#define MIS_SC1_FCR_flow_en(data)                                                    (0x00000001&((data)<<0))
#define MIS_SC1_FCR_flow_en_src(data)                                                ((0x00000001&(data))>>0)
#define MIS_SC1_FCR_get_flow_en(data)                                                ((0x00000001&(data))>>0)


#define MIS_SC1_IRSR                                                                 0x1801BAA0
#define MIS_SC1_IRSR_reg_addr                                                        "0xB801BAA0"
#define MIS_SC1_IRSR_reg                                                             0xB801BAA0
#define set_MIS_SC1_IRSR_reg(data)   (*((volatile unsigned int*) MIS_SC1_IRSR_reg)=data)
#define get_MIS_SC1_IRSR_reg   (*((volatile unsigned int*) MIS_SC1_IRSR_reg))
#define MIS_SC1_IRSR_inst_adr                                                        "0x00A8"
#define MIS_SC1_IRSR_inst                                                            0x00A8
#define MIS_SC1_IRSR_pres_shift                                                      (16)
#define MIS_SC1_IRSR_pres_mask                                                       (0x00010000)
#define MIS_SC1_IRSR_pres(data)                                                      (0x00010000&((data)<<16))
#define MIS_SC1_IRSR_pres_src(data)                                                  ((0x00010000&(data))>>16)
#define MIS_SC1_IRSR_get_pres(data)                                                  ((0x00010000&(data))>>16)
#define MIS_SC1_IRSR_cpres_int_shift                                                 (15)
#define MIS_SC1_IRSR_cpres_int_mask                                                  (0x00008000)
#define MIS_SC1_IRSR_cpres_int(data)                                                 (0x00008000&((data)<<15))
#define MIS_SC1_IRSR_cpres_int_src(data)                                             ((0x00008000&(data))>>15)
#define MIS_SC1_IRSR_get_cpres_int(data)                                             ((0x00008000&(data))>>15)
#define MIS_SC1_IRSR_tx_flow_int_shift                                               (14)
#define MIS_SC1_IRSR_tx_flow_int_mask                                                (0x00004000)
#define MIS_SC1_IRSR_tx_flow_int(data)                                               (0x00004000&((data)<<14))
#define MIS_SC1_IRSR_tx_flow_int_src(data)                                           ((0x00004000&(data))>>14)
#define MIS_SC1_IRSR_get_tx_flow_int(data)                                           ((0x00004000&(data))>>14)
#define MIS_SC1_IRSR_txp_int_shift                                                   (13)
#define MIS_SC1_IRSR_txp_int_mask                                                    (0x00002000)
#define MIS_SC1_IRSR_txp_int(data)                                                   (0x00002000&((data)<<13))
#define MIS_SC1_IRSR_txp_int_src(data)                                               ((0x00002000&(data))>>13)
#define MIS_SC1_IRSR_get_txp_int(data)                                               ((0x00002000&(data))>>13)
#define MIS_SC1_IRSR_txdone_int_shift                                                (12)
#define MIS_SC1_IRSR_txdone_int_mask                                                 (0x00001000)
#define MIS_SC1_IRSR_txdone_int(data)                                                (0x00001000&((data)<<12))
#define MIS_SC1_IRSR_txdone_int_src(data)                                            ((0x00001000&(data))>>12)
#define MIS_SC1_IRSR_get_txdone_int(data)                                            ((0x00001000&(data))>>12)
#define MIS_SC1_IRSR_txempty_int_shift                                               (11)
#define MIS_SC1_IRSR_txempty_int_mask                                                (0x00000800)
#define MIS_SC1_IRSR_txempty_int(data)                                               (0x00000800&((data)<<11))
#define MIS_SC1_IRSR_txempty_int_src(data)                                           ((0x00000800&(data))>>11)
#define MIS_SC1_IRSR_get_txempty_int(data)                                           ((0x00000800&(data))>>11)
#define MIS_SC1_IRSR_edcerr_int_shift                                                (10)
#define MIS_SC1_IRSR_edcerr_int_mask                                                 (0x00000400)
#define MIS_SC1_IRSR_edcerr_int(data)                                                (0x00000400&((data)<<10))
#define MIS_SC1_IRSR_edcerr_int_src(data)                                            ((0x00000400&(data))>>10)
#define MIS_SC1_IRSR_get_edcerr_int(data)                                            ((0x00000400&(data))>>10)
#define MIS_SC1_IRSR_rx_fover_shift                                                  (9)
#define MIS_SC1_IRSR_rx_fover_mask                                                   (0x00000200)
#define MIS_SC1_IRSR_rx_fover(data)                                                  (0x00000200&((data)<<9))
#define MIS_SC1_IRSR_rx_fover_src(data)                                              ((0x00000200&(data))>>9)
#define MIS_SC1_IRSR_get_rx_fover(data)                                              ((0x00000200&(data))>>9)
#define MIS_SC1_IRSR_rxp_int_shift                                                   (8)
#define MIS_SC1_IRSR_rxp_int_mask                                                    (0x00000100)
#define MIS_SC1_IRSR_rxp_int(data)                                                   (0x00000100&((data)<<8))
#define MIS_SC1_IRSR_rxp_int_src(data)                                               ((0x00000100&(data))>>8)
#define MIS_SC1_IRSR_get_rxp_int(data)                                               ((0x00000100&(data))>>8)
#define MIS_SC1_IRSR_atrs_int_shift                                                  (7)
#define MIS_SC1_IRSR_atrs_int_mask                                                   (0x00000080)
#define MIS_SC1_IRSR_atrs_int(data)                                                  (0x00000080&((data)<<7))
#define MIS_SC1_IRSR_atrs_int_src(data)                                              ((0x00000080&(data))>>7)
#define MIS_SC1_IRSR_get_atrs_int(data)                                              ((0x00000080&(data))>>7)
#define MIS_SC1_IRSR_bgt_int_shift                                                   (6)
#define MIS_SC1_IRSR_bgt_int_mask                                                    (0x00000040)
#define MIS_SC1_IRSR_bgt_int(data)                                                   (0x00000040&((data)<<6))
#define MIS_SC1_IRSR_bgt_int_src(data)                                               ((0x00000040&(data))>>6)
#define MIS_SC1_IRSR_get_bgt_int(data)                                               ((0x00000040&(data))>>6)
#define MIS_SC1_IRSR_cwt_int_shift                                                   (5)
#define MIS_SC1_IRSR_cwt_int_mask                                                    (0x00000020)
#define MIS_SC1_IRSR_cwt_int(data)                                                   (0x00000020&((data)<<5))
#define MIS_SC1_IRSR_cwt_int_src(data)                                               ((0x00000020&(data))>>5)
#define MIS_SC1_IRSR_get_cwt_int(data)                                               ((0x00000020&(data))>>5)
#define MIS_SC1_IRSR_rlen_int_shift                                                  (4)
#define MIS_SC1_IRSR_rlen_int_mask                                                   (0x00000010)
#define MIS_SC1_IRSR_rlen_int(data)                                                  (0x00000010&((data)<<4))
#define MIS_SC1_IRSR_rlen_int_src(data)                                              ((0x00000010&(data))>>4)
#define MIS_SC1_IRSR_get_rlen_int(data)                                              ((0x00000010&(data))>>4)
#define MIS_SC1_IRSR_wwt_int_shift                                                   (3)
#define MIS_SC1_IRSR_wwt_int_mask                                                    (0x00000008)
#define MIS_SC1_IRSR_wwt_int(data)                                                   (0x00000008&((data)<<3))
#define MIS_SC1_IRSR_wwt_int_src(data)                                               ((0x00000008&(data))>>3)
#define MIS_SC1_IRSR_get_wwt_int(data)                                               ((0x00000008&(data))>>3)
#define MIS_SC1_IRSR_bwt_int_shift                                                   (2)
#define MIS_SC1_IRSR_bwt_int_mask                                                    (0x00000004)
#define MIS_SC1_IRSR_bwt_int(data)                                                   (0x00000004&((data)<<2))
#define MIS_SC1_IRSR_bwt_int_src(data)                                               ((0x00000004&(data))>>2)
#define MIS_SC1_IRSR_get_bwt_int(data)                                               ((0x00000004&(data))>>2)
#define MIS_SC1_IRSR_rcv_int_shift                                                   (1)
#define MIS_SC1_IRSR_rcv_int_mask                                                    (0x00000002)
#define MIS_SC1_IRSR_rcv_int(data)                                                   (0x00000002&((data)<<1))
#define MIS_SC1_IRSR_rcv_int_src(data)                                               ((0x00000002&(data))>>1)
#define MIS_SC1_IRSR_get_rcv_int(data)                                               ((0x00000002&(data))>>1)
#define MIS_SC1_IRSR_drdy_int_shift                                                  (0)
#define MIS_SC1_IRSR_drdy_int_mask                                                   (0x00000001)
#define MIS_SC1_IRSR_drdy_int(data)                                                  (0x00000001&((data)<<0))
#define MIS_SC1_IRSR_drdy_int_src(data)                                              ((0x00000001&(data))>>0)
#define MIS_SC1_IRSR_get_drdy_int(data)                                              ((0x00000001&(data))>>0)


#define MIS_SC1_IRER                                                                 0x1801BAA4
#define MIS_SC1_IRER_reg_addr                                                        "0xB801BAA4"
#define MIS_SC1_IRER_reg                                                             0xB801BAA4
#define set_MIS_SC1_IRER_reg(data)   (*((volatile unsigned int*) MIS_SC1_IRER_reg)=data)
#define get_MIS_SC1_IRER_reg   (*((volatile unsigned int*) MIS_SC1_IRER_reg))
#define MIS_SC1_IRER_inst_adr                                                        "0x00A9"
#define MIS_SC1_IRER_inst                                                            0x00A9
#define MIS_SC1_IRER_cpres_en_shift                                                  (15)
#define MIS_SC1_IRER_cpres_en_mask                                                   (0x00008000)
#define MIS_SC1_IRER_cpres_en(data)                                                  (0x00008000&((data)<<15))
#define MIS_SC1_IRER_cpres_en_src(data)                                              ((0x00008000&(data))>>15)
#define MIS_SC1_IRER_get_cpres_en(data)                                              ((0x00008000&(data))>>15)
#define MIS_SC1_IRER_txflow_int_en_shift                                             (14)
#define MIS_SC1_IRER_txflow_int_en_mask                                              (0x00004000)
#define MIS_SC1_IRER_txflow_int_en(data)                                             (0x00004000&((data)<<14))
#define MIS_SC1_IRER_txflow_int_en_src(data)                                         ((0x00004000&(data))>>14)
#define MIS_SC1_IRER_get_txflow_int_en(data)                                         ((0x00004000&(data))>>14)
#define MIS_SC1_IRER_txp_en_shift                                                    (13)
#define MIS_SC1_IRER_txp_en_mask                                                     (0x00002000)
#define MIS_SC1_IRER_txp_en(data)                                                    (0x00002000&((data)<<13))
#define MIS_SC1_IRER_txp_en_src(data)                                                ((0x00002000&(data))>>13)
#define MIS_SC1_IRER_get_txp_en(data)                                                ((0x00002000&(data))>>13)
#define MIS_SC1_IRER_txdone_en_shift                                                 (12)
#define MIS_SC1_IRER_txdone_en_mask                                                  (0x00001000)
#define MIS_SC1_IRER_txdone_en(data)                                                 (0x00001000&((data)<<12))
#define MIS_SC1_IRER_txdone_en_src(data)                                             ((0x00001000&(data))>>12)
#define MIS_SC1_IRER_get_txdone_en(data)                                             ((0x00001000&(data))>>12)
#define MIS_SC1_IRER_txempty_en_shift                                                (11)
#define MIS_SC1_IRER_txempty_en_mask                                                 (0x00000800)
#define MIS_SC1_IRER_txempty_en(data)                                                (0x00000800&((data)<<11))
#define MIS_SC1_IRER_txempty_en_src(data)                                            ((0x00000800&(data))>>11)
#define MIS_SC1_IRER_get_txempty_en(data)                                            ((0x00000800&(data))>>11)
#define MIS_SC1_IRER_edcerr_en_shift                                                 (10)
#define MIS_SC1_IRER_edcerr_en_mask                                                  (0x00000400)
#define MIS_SC1_IRER_edcerr_en(data)                                                 (0x00000400&((data)<<10))
#define MIS_SC1_IRER_edcerr_en_src(data)                                             ((0x00000400&(data))>>10)
#define MIS_SC1_IRER_get_edcerr_en(data)                                             ((0x00000400&(data))>>10)
#define MIS_SC1_IRER_rx_fover_en_shift                                               (9)
#define MIS_SC1_IRER_rx_fover_en_mask                                                (0x00000200)
#define MIS_SC1_IRER_rx_fover_en(data)                                               (0x00000200&((data)<<9))
#define MIS_SC1_IRER_rx_fover_en_src(data)                                           ((0x00000200&(data))>>9)
#define MIS_SC1_IRER_get_rx_fover_en(data)                                           ((0x00000200&(data))>>9)
#define MIS_SC1_IRER_rxp_en_shift                                                    (8)
#define MIS_SC1_IRER_rxp_en_mask                                                     (0x00000100)
#define MIS_SC1_IRER_rxp_en(data)                                                    (0x00000100&((data)<<8))
#define MIS_SC1_IRER_rxp_en_src(data)                                                ((0x00000100&(data))>>8)
#define MIS_SC1_IRER_get_rxp_en(data)                                                ((0x00000100&(data))>>8)
#define MIS_SC1_IRER_atrs_en_shift                                                   (7)
#define MIS_SC1_IRER_atrs_en_mask                                                    (0x00000080)
#define MIS_SC1_IRER_atrs_en(data)                                                   (0x00000080&((data)<<7))
#define MIS_SC1_IRER_atrs_en_src(data)                                               ((0x00000080&(data))>>7)
#define MIS_SC1_IRER_get_atrs_en(data)                                               ((0x00000080&(data))>>7)
#define MIS_SC1_IRER_bgt_en_shift                                                    (6)
#define MIS_SC1_IRER_bgt_en_mask                                                     (0x00000040)
#define MIS_SC1_IRER_bgt_en(data)                                                    (0x00000040&((data)<<6))
#define MIS_SC1_IRER_bgt_en_src(data)                                                ((0x00000040&(data))>>6)
#define MIS_SC1_IRER_get_bgt_en(data)                                                ((0x00000040&(data))>>6)
#define MIS_SC1_IRER_cwt_en_shift                                                    (5)
#define MIS_SC1_IRER_cwt_en_mask                                                     (0x00000020)
#define MIS_SC1_IRER_cwt_en(data)                                                    (0x00000020&((data)<<5))
#define MIS_SC1_IRER_cwt_en_src(data)                                                ((0x00000020&(data))>>5)
#define MIS_SC1_IRER_get_cwt_en(data)                                                ((0x00000020&(data))>>5)
#define MIS_SC1_IRER_rlen_en_shift                                                   (4)
#define MIS_SC1_IRER_rlen_en_mask                                                    (0x00000010)
#define MIS_SC1_IRER_rlen_en(data)                                                   (0x00000010&((data)<<4))
#define MIS_SC1_IRER_rlen_en_src(data)                                               ((0x00000010&(data))>>4)
#define MIS_SC1_IRER_get_rlen_en(data)                                               ((0x00000010&(data))>>4)
#define MIS_SC1_IRER_wwt_en_shift                                                    (3)
#define MIS_SC1_IRER_wwt_en_mask                                                     (0x00000008)
#define MIS_SC1_IRER_wwt_en(data)                                                    (0x00000008&((data)<<3))
#define MIS_SC1_IRER_wwt_en_src(data)                                                ((0x00000008&(data))>>3)
#define MIS_SC1_IRER_get_wwt_en(data)                                                ((0x00000008&(data))>>3)
#define MIS_SC1_IRER_bwt_en_shift                                                    (2)
#define MIS_SC1_IRER_bwt_en_mask                                                     (0x00000004)
#define MIS_SC1_IRER_bwt_en(data)                                                    (0x00000004&((data)<<2))
#define MIS_SC1_IRER_bwt_en_src(data)                                                ((0x00000004&(data))>>2)
#define MIS_SC1_IRER_get_bwt_en(data)                                                ((0x00000004&(data))>>2)
#define MIS_SC1_IRER_rcv_en_shift                                                    (1)
#define MIS_SC1_IRER_rcv_en_mask                                                     (0x00000002)
#define MIS_SC1_IRER_rcv_en(data)                                                    (0x00000002&((data)<<1))
#define MIS_SC1_IRER_rcv_en_src(data)                                                ((0x00000002&(data))>>1)
#define MIS_SC1_IRER_get_rcv_en(data)                                                ((0x00000002&(data))>>1)
#define MIS_SC1_IRER_drdy_en_shift                                                   (0)
#define MIS_SC1_IRER_drdy_en_mask                                                    (0x00000001)
#define MIS_SC1_IRER_drdy_en(data)                                                   (0x00000001&((data)<<0))
#define MIS_SC1_IRER_drdy_en_src(data)                                               ((0x00000001&(data))>>0)
#define MIS_SC1_IRER_get_drdy_en(data)                                               ((0x00000001&(data))>>0)


#define MIS_EF_DATA_0                                                                0x1801B800
#define MIS_EF_DATA_1                                                                0x1801B804
#define MIS_EF_DATA_2                                                                0x1801B808
#define MIS_EF_DATA_3                                                                0x1801B80C
#define MIS_EF_DATA_4                                                                0x1801B810
#define MIS_EF_DATA_5                                                                0x1801B814
#define MIS_EF_DATA_6                                                                0x1801B818
#define MIS_EF_DATA_7                                                                0x1801B81C
#define MIS_EF_DATA_8                                                                0x1801B820
#define MIS_EF_DATA_9                                                                0x1801B824
#define MIS_EF_DATA_10                                                                0x1801B828
#define MIS_EF_DATA_11                                                                0x1801B82C
#define MIS_EF_DATA_12                                                                0x1801B830
#define MIS_EF_DATA_13                                                                0x1801B834
#define MIS_EF_DATA_14                                                                0x1801B838
#define MIS_EF_DATA_15                                                                0x1801B83C
#define MIS_EF_DATA_16                                                                0x1801B840
#define MIS_EF_DATA_17                                                                0x1801B844
#define MIS_EF_DATA_18                                                                0x1801B848
#define MIS_EF_DATA_19                                                                0x1801B84C
#define MIS_EF_DATA_20                                                                0x1801B850
#define MIS_EF_DATA_21                                                                0x1801B854
#define MIS_EF_DATA_22                                                                0x1801B858
#define MIS_EF_DATA_23                                                                0x1801B85C
#define MIS_EF_DATA_24                                                                0x1801B860
#define MIS_EF_DATA_25                                                                0x1801B864
#define MIS_EF_DATA_26                                                                0x1801B868
#define MIS_EF_DATA_27                                                                0x1801B86C
#define MIS_EF_DATA_28                                                                0x1801B870
#define MIS_EF_DATA_29                                                                0x1801B874
#define MIS_EF_DATA_30                                                                0x1801B878
#define MIS_EF_DATA_31                                                                0x1801B87C
#define MIS_EF_DATA_32                                                                0x1801B880
#define MIS_EF_DATA_33                                                                0x1801B884
#define MIS_EF_DATA_34                                                                0x1801B888
#define MIS_EF_DATA_35                                                                0x1801B88C
#define MIS_EF_DATA_36                                                                0x1801B890
#define MIS_EF_DATA_37                                                                0x1801B894
#define MIS_EF_DATA_38                                                                0x1801B898
#define MIS_EF_DATA_39                                                                0x1801B89C
#define MIS_EF_DATA_40                                                                0x1801B8A0
#define MIS_EF_DATA_41                                                                0x1801B8A4
#define MIS_EF_DATA_42                                                                0x1801B8A8
#define MIS_EF_DATA_43                                                                0x1801B8AC
#define MIS_EF_DATA_44                                                                0x1801B8B0
#define MIS_EF_DATA_45                                                                0x1801B8B4
#define MIS_EF_DATA_46                                                                0x1801B8B8
#define MIS_EF_DATA_47                                                                0x1801B8BC
#define MIS_EF_DATA_48                                                                0x1801B8C0
#define MIS_EF_DATA_49                                                                0x1801B8C4
#define MIS_EF_DATA_50                                                                0x1801B8C8
#define MIS_EF_DATA_51                                                                0x1801B8CC
#define MIS_EF_DATA_52                                                                0x1801B8D0
#define MIS_EF_DATA_53                                                                0x1801B8D4
#define MIS_EF_DATA_54                                                                0x1801B8D8
#define MIS_EF_DATA_55                                                                0x1801B8DC
#define MIS_EF_DATA_56                                                                0x1801B8E0
#define MIS_EF_DATA_57                                                                0x1801B8E4
#define MIS_EF_DATA_58                                                                0x1801B8E8
#define MIS_EF_DATA_59                                                                0x1801B8EC
#define MIS_EF_DATA_60                                                                0x1801B8F0
#define MIS_EF_DATA_61                                                                0x1801B8F4
#define MIS_EF_DATA_62                                                                0x1801B8F8
#define MIS_EF_DATA_63                                                                0x1801B8FC
#define MIS_EF_DATA_64                                                                0x1801B900
#define MIS_EF_DATA_65                                                                0x1801B904
#define MIS_EF_DATA_66                                                                0x1801B908
#define MIS_EF_DATA_67                                                                0x1801B90C
#define MIS_EF_DATA_68                                                                0x1801B910
#define MIS_EF_DATA_69                                                                0x1801B914
#define MIS_EF_DATA_70                                                                0x1801B918
#define MIS_EF_DATA_71                                                                0x1801B91C
#define MIS_EF_DATA_72                                                                0x1801B920
#define MIS_EF_DATA_73                                                                0x1801B924
#define MIS_EF_DATA_0_reg_addr                                                       "0xB801B800"
#define MIS_EF_DATA_1_reg_addr                                                       "0xB801B804"
#define MIS_EF_DATA_2_reg_addr                                                       "0xB801B808"
#define MIS_EF_DATA_3_reg_addr                                                       "0xB801B80C"
#define MIS_EF_DATA_4_reg_addr                                                       "0xB801B810"
#define MIS_EF_DATA_5_reg_addr                                                       "0xB801B814"
#define MIS_EF_DATA_6_reg_addr                                                       "0xB801B818"
#define MIS_EF_DATA_7_reg_addr                                                       "0xB801B81C"
#define MIS_EF_DATA_8_reg_addr                                                       "0xB801B820"
#define MIS_EF_DATA_9_reg_addr                                                       "0xB801B824"
#define MIS_EF_DATA_10_reg_addr                                                       "0xB801B828"
#define MIS_EF_DATA_11_reg_addr                                                       "0xB801B82C"
#define MIS_EF_DATA_12_reg_addr                                                       "0xB801B830"
#define MIS_EF_DATA_13_reg_addr                                                       "0xB801B834"
#define MIS_EF_DATA_14_reg_addr                                                       "0xB801B838"
#define MIS_EF_DATA_15_reg_addr                                                       "0xB801B83C"
#define MIS_EF_DATA_16_reg_addr                                                       "0xB801B840"
#define MIS_EF_DATA_17_reg_addr                                                       "0xB801B844"
#define MIS_EF_DATA_18_reg_addr                                                       "0xB801B848"
#define MIS_EF_DATA_19_reg_addr                                                       "0xB801B84C"
#define MIS_EF_DATA_20_reg_addr                                                       "0xB801B850"
#define MIS_EF_DATA_21_reg_addr                                                       "0xB801B854"
#define MIS_EF_DATA_22_reg_addr                                                       "0xB801B858"
#define MIS_EF_DATA_23_reg_addr                                                       "0xB801B85C"
#define MIS_EF_DATA_24_reg_addr                                                       "0xB801B860"
#define MIS_EF_DATA_25_reg_addr                                                       "0xB801B864"
#define MIS_EF_DATA_26_reg_addr                                                       "0xB801B868"
#define MIS_EF_DATA_27_reg_addr                                                       "0xB801B86C"
#define MIS_EF_DATA_28_reg_addr                                                       "0xB801B870"
#define MIS_EF_DATA_29_reg_addr                                                       "0xB801B874"
#define MIS_EF_DATA_30_reg_addr                                                       "0xB801B878"
#define MIS_EF_DATA_31_reg_addr                                                       "0xB801B87C"
#define MIS_EF_DATA_32_reg_addr                                                       "0xB801B880"
#define MIS_EF_DATA_33_reg_addr                                                       "0xB801B884"
#define MIS_EF_DATA_34_reg_addr                                                       "0xB801B888"
#define MIS_EF_DATA_35_reg_addr                                                       "0xB801B88C"
#define MIS_EF_DATA_36_reg_addr                                                       "0xB801B890"
#define MIS_EF_DATA_37_reg_addr                                                       "0xB801B894"
#define MIS_EF_DATA_38_reg_addr                                                       "0xB801B898"
#define MIS_EF_DATA_39_reg_addr                                                       "0xB801B89C"
#define MIS_EF_DATA_40_reg_addr                                                       "0xB801B8A0"
#define MIS_EF_DATA_41_reg_addr                                                       "0xB801B8A4"
#define MIS_EF_DATA_42_reg_addr                                                       "0xB801B8A8"
#define MIS_EF_DATA_43_reg_addr                                                       "0xB801B8AC"
#define MIS_EF_DATA_44_reg_addr                                                       "0xB801B8B0"
#define MIS_EF_DATA_45_reg_addr                                                       "0xB801B8B4"
#define MIS_EF_DATA_46_reg_addr                                                       "0xB801B8B8"
#define MIS_EF_DATA_47_reg_addr                                                       "0xB801B8BC"
#define MIS_EF_DATA_48_reg_addr                                                       "0xB801B8C0"
#define MIS_EF_DATA_49_reg_addr                                                       "0xB801B8C4"
#define MIS_EF_DATA_50_reg_addr                                                       "0xB801B8C8"
#define MIS_EF_DATA_51_reg_addr                                                       "0xB801B8CC"
#define MIS_EF_DATA_52_reg_addr                                                       "0xB801B8D0"
#define MIS_EF_DATA_53_reg_addr                                                       "0xB801B8D4"
#define MIS_EF_DATA_54_reg_addr                                                       "0xB801B8D8"
#define MIS_EF_DATA_55_reg_addr                                                       "0xB801B8DC"
#define MIS_EF_DATA_56_reg_addr                                                       "0xB801B8E0"
#define MIS_EF_DATA_57_reg_addr                                                       "0xB801B8E4"
#define MIS_EF_DATA_58_reg_addr                                                       "0xB801B8E8"
#define MIS_EF_DATA_59_reg_addr                                                       "0xB801B8EC"
#define MIS_EF_DATA_60_reg_addr                                                       "0xB801B8F0"
#define MIS_EF_DATA_61_reg_addr                                                       "0xB801B8F4"
#define MIS_EF_DATA_62_reg_addr                                                       "0xB801B8F8"
#define MIS_EF_DATA_63_reg_addr                                                       "0xB801B8FC"
#define MIS_EF_DATA_64_reg_addr                                                       "0xB801B900"
#define MIS_EF_DATA_65_reg_addr                                                       "0xB801B904"
#define MIS_EF_DATA_66_reg_addr                                                       "0xB801B908"
#define MIS_EF_DATA_67_reg_addr                                                       "0xB801B90C"
#define MIS_EF_DATA_68_reg_addr                                                       "0xB801B910"
#define MIS_EF_DATA_69_reg_addr                                                       "0xB801B914"
#define MIS_EF_DATA_70_reg_addr                                                       "0xB801B918"
#define MIS_EF_DATA_71_reg_addr                                                       "0xB801B91C"
#define MIS_EF_DATA_72_reg_addr                                                       "0xB801B920"
#define MIS_EF_DATA_73_reg_addr                                                       "0xB801B924"
#define MIS_EF_DATA_0_reg                                                            0xB801B800
#define MIS_EF_DATA_1_reg                                                            0xB801B804
#define MIS_EF_DATA_2_reg                                                            0xB801B808
#define MIS_EF_DATA_3_reg                                                            0xB801B80C
#define MIS_EF_DATA_4_reg                                                            0xB801B810
#define MIS_EF_DATA_5_reg                                                            0xB801B814
#define MIS_EF_DATA_6_reg                                                            0xB801B818
#define MIS_EF_DATA_7_reg                                                            0xB801B81C
#define MIS_EF_DATA_8_reg                                                            0xB801B820
#define MIS_EF_DATA_9_reg                                                            0xB801B824
#define MIS_EF_DATA_10_reg                                                            0xB801B828
#define MIS_EF_DATA_11_reg                                                            0xB801B82C
#define MIS_EF_DATA_12_reg                                                            0xB801B830
#define MIS_EF_DATA_13_reg                                                            0xB801B834
#define MIS_EF_DATA_14_reg                                                            0xB801B838
#define MIS_EF_DATA_15_reg                                                            0xB801B83C
#define MIS_EF_DATA_16_reg                                                            0xB801B840
#define MIS_EF_DATA_17_reg                                                            0xB801B844
#define MIS_EF_DATA_18_reg                                                            0xB801B848
#define MIS_EF_DATA_19_reg                                                            0xB801B84C
#define MIS_EF_DATA_20_reg                                                            0xB801B850
#define MIS_EF_DATA_21_reg                                                            0xB801B854
#define MIS_EF_DATA_22_reg                                                            0xB801B858
#define MIS_EF_DATA_23_reg                                                            0xB801B85C
#define MIS_EF_DATA_24_reg                                                            0xB801B860
#define MIS_EF_DATA_25_reg                                                            0xB801B864
#define MIS_EF_DATA_26_reg                                                            0xB801B868
#define MIS_EF_DATA_27_reg                                                            0xB801B86C
#define MIS_EF_DATA_28_reg                                                            0xB801B870
#define MIS_EF_DATA_29_reg                                                            0xB801B874
#define MIS_EF_DATA_30_reg                                                            0xB801B878
#define MIS_EF_DATA_31_reg                                                            0xB801B87C
#define MIS_EF_DATA_32_reg                                                            0xB801B880
#define MIS_EF_DATA_33_reg                                                            0xB801B884
#define MIS_EF_DATA_34_reg                                                            0xB801B888
#define MIS_EF_DATA_35_reg                                                            0xB801B88C
#define MIS_EF_DATA_36_reg                                                            0xB801B890
#define MIS_EF_DATA_37_reg                                                            0xB801B894
#define MIS_EF_DATA_38_reg                                                            0xB801B898
#define MIS_EF_DATA_39_reg                                                            0xB801B89C
#define MIS_EF_DATA_40_reg                                                            0xB801B8A0
#define MIS_EF_DATA_41_reg                                                            0xB801B8A4
#define MIS_EF_DATA_42_reg                                                            0xB801B8A8
#define MIS_EF_DATA_43_reg                                                            0xB801B8AC
#define MIS_EF_DATA_44_reg                                                            0xB801B8B0
#define MIS_EF_DATA_45_reg                                                            0xB801B8B4
#define MIS_EF_DATA_46_reg                                                            0xB801B8B8
#define MIS_EF_DATA_47_reg                                                            0xB801B8BC
#define MIS_EF_DATA_48_reg                                                            0xB801B8C0
#define MIS_EF_DATA_49_reg                                                            0xB801B8C4
#define MIS_EF_DATA_50_reg                                                            0xB801B8C8
#define MIS_EF_DATA_51_reg                                                            0xB801B8CC
#define MIS_EF_DATA_52_reg                                                            0xB801B8D0
#define MIS_EF_DATA_53_reg                                                            0xB801B8D4
#define MIS_EF_DATA_54_reg                                                            0xB801B8D8
#define MIS_EF_DATA_55_reg                                                            0xB801B8DC
#define MIS_EF_DATA_56_reg                                                            0xB801B8E0
#define MIS_EF_DATA_57_reg                                                            0xB801B8E4
#define MIS_EF_DATA_58_reg                                                            0xB801B8E8
#define MIS_EF_DATA_59_reg                                                            0xB801B8EC
#define MIS_EF_DATA_60_reg                                                            0xB801B8F0
#define MIS_EF_DATA_61_reg                                                            0xB801B8F4
#define MIS_EF_DATA_62_reg                                                            0xB801B8F8
#define MIS_EF_DATA_63_reg                                                            0xB801B8FC
#define MIS_EF_DATA_64_reg                                                            0xB801B900
#define MIS_EF_DATA_65_reg                                                            0xB801B904
#define MIS_EF_DATA_66_reg                                                            0xB801B908
#define MIS_EF_DATA_67_reg                                                            0xB801B90C
#define MIS_EF_DATA_68_reg                                                            0xB801B910
#define MIS_EF_DATA_69_reg                                                            0xB801B914
#define MIS_EF_DATA_70_reg                                                            0xB801B918
#define MIS_EF_DATA_71_reg                                                            0xB801B91C
#define MIS_EF_DATA_72_reg                                                            0xB801B920
#define MIS_EF_DATA_73_reg                                                            0xB801B924
#define set_MIS_EF_DATA_0_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_0_reg)=data)
#define set_MIS_EF_DATA_1_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_1_reg)=data)
#define set_MIS_EF_DATA_2_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_2_reg)=data)
#define set_MIS_EF_DATA_3_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_3_reg)=data)
#define set_MIS_EF_DATA_4_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_4_reg)=data)
#define set_MIS_EF_DATA_5_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_5_reg)=data)
#define set_MIS_EF_DATA_6_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_6_reg)=data)
#define set_MIS_EF_DATA_7_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_7_reg)=data)
#define set_MIS_EF_DATA_8_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_8_reg)=data)
#define set_MIS_EF_DATA_9_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_9_reg)=data)
#define set_MIS_EF_DATA_10_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_10_reg)=data)
#define set_MIS_EF_DATA_11_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_11_reg)=data)
#define set_MIS_EF_DATA_12_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_12_reg)=data)
#define set_MIS_EF_DATA_13_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_13_reg)=data)
#define set_MIS_EF_DATA_14_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_14_reg)=data)
#define set_MIS_EF_DATA_15_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_15_reg)=data)
#define set_MIS_EF_DATA_16_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_16_reg)=data)
#define set_MIS_EF_DATA_17_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_17_reg)=data)
#define set_MIS_EF_DATA_18_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_18_reg)=data)
#define set_MIS_EF_DATA_19_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_19_reg)=data)
#define set_MIS_EF_DATA_20_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_20_reg)=data)
#define set_MIS_EF_DATA_21_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_21_reg)=data)
#define set_MIS_EF_DATA_22_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_22_reg)=data)
#define set_MIS_EF_DATA_23_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_23_reg)=data)
#define set_MIS_EF_DATA_24_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_24_reg)=data)
#define set_MIS_EF_DATA_25_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_25_reg)=data)
#define set_MIS_EF_DATA_26_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_26_reg)=data)
#define set_MIS_EF_DATA_27_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_27_reg)=data)
#define set_MIS_EF_DATA_28_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_28_reg)=data)
#define set_MIS_EF_DATA_29_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_29_reg)=data)
#define set_MIS_EF_DATA_30_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_30_reg)=data)
#define set_MIS_EF_DATA_31_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_31_reg)=data)
#define set_MIS_EF_DATA_32_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_32_reg)=data)
#define set_MIS_EF_DATA_33_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_33_reg)=data)
#define set_MIS_EF_DATA_34_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_34_reg)=data)
#define set_MIS_EF_DATA_35_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_35_reg)=data)
#define set_MIS_EF_DATA_36_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_36_reg)=data)
#define set_MIS_EF_DATA_37_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_37_reg)=data)
#define set_MIS_EF_DATA_38_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_38_reg)=data)
#define set_MIS_EF_DATA_39_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_39_reg)=data)
#define set_MIS_EF_DATA_40_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_40_reg)=data)
#define set_MIS_EF_DATA_41_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_41_reg)=data)
#define set_MIS_EF_DATA_42_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_42_reg)=data)
#define set_MIS_EF_DATA_43_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_43_reg)=data)
#define set_MIS_EF_DATA_44_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_44_reg)=data)
#define set_MIS_EF_DATA_45_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_45_reg)=data)
#define set_MIS_EF_DATA_46_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_46_reg)=data)
#define set_MIS_EF_DATA_47_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_47_reg)=data)
#define set_MIS_EF_DATA_48_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_48_reg)=data)
#define set_MIS_EF_DATA_49_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_49_reg)=data)
#define set_MIS_EF_DATA_50_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_50_reg)=data)
#define set_MIS_EF_DATA_51_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_51_reg)=data)
#define set_MIS_EF_DATA_52_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_52_reg)=data)
#define set_MIS_EF_DATA_53_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_53_reg)=data)
#define set_MIS_EF_DATA_54_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_54_reg)=data)
#define set_MIS_EF_DATA_55_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_55_reg)=data)
#define set_MIS_EF_DATA_56_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_56_reg)=data)
#define set_MIS_EF_DATA_57_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_57_reg)=data)
#define set_MIS_EF_DATA_58_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_58_reg)=data)
#define set_MIS_EF_DATA_59_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_59_reg)=data)
#define set_MIS_EF_DATA_60_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_60_reg)=data)
#define set_MIS_EF_DATA_61_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_61_reg)=data)
#define set_MIS_EF_DATA_62_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_62_reg)=data)
#define set_MIS_EF_DATA_63_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_63_reg)=data)
#define set_MIS_EF_DATA_64_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_64_reg)=data)
#define set_MIS_EF_DATA_65_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_65_reg)=data)
#define set_MIS_EF_DATA_66_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_66_reg)=data)
#define set_MIS_EF_DATA_67_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_67_reg)=data)
#define set_MIS_EF_DATA_68_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_68_reg)=data)
#define set_MIS_EF_DATA_69_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_69_reg)=data)
#define set_MIS_EF_DATA_70_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_70_reg)=data)
#define set_MIS_EF_DATA_71_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_71_reg)=data)
#define set_MIS_EF_DATA_72_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_72_reg)=data)
#define set_MIS_EF_DATA_73_reg(data)   (*((volatile unsigned int*) MIS_EF_DATA_73_reg)=data)
#define get_MIS_EF_DATA_0_reg   (*((volatile unsigned int*) MIS_EF_DATA_0_reg))
#define get_MIS_EF_DATA_1_reg   (*((volatile unsigned int*) MIS_EF_DATA_1_reg))
#define get_MIS_EF_DATA_2_reg   (*((volatile unsigned int*) MIS_EF_DATA_2_reg))
#define get_MIS_EF_DATA_3_reg   (*((volatile unsigned int*) MIS_EF_DATA_3_reg))
#define get_MIS_EF_DATA_4_reg   (*((volatile unsigned int*) MIS_EF_DATA_4_reg))
#define get_MIS_EF_DATA_5_reg   (*((volatile unsigned int*) MIS_EF_DATA_5_reg))
#define get_MIS_EF_DATA_6_reg   (*((volatile unsigned int*) MIS_EF_DATA_6_reg))
#define get_MIS_EF_DATA_7_reg   (*((volatile unsigned int*) MIS_EF_DATA_7_reg))
#define get_MIS_EF_DATA_8_reg   (*((volatile unsigned int*) MIS_EF_DATA_8_reg))
#define get_MIS_EF_DATA_9_reg   (*((volatile unsigned int*) MIS_EF_DATA_9_reg))
#define get_MIS_EF_DATA_10_reg   (*((volatile unsigned int*) MIS_EF_DATA_10_reg))
#define get_MIS_EF_DATA_11_reg   (*((volatile unsigned int*) MIS_EF_DATA_11_reg))
#define get_MIS_EF_DATA_12_reg   (*((volatile unsigned int*) MIS_EF_DATA_12_reg))
#define get_MIS_EF_DATA_13_reg   (*((volatile unsigned int*) MIS_EF_DATA_13_reg))
#define get_MIS_EF_DATA_14_reg   (*((volatile unsigned int*) MIS_EF_DATA_14_reg))
#define get_MIS_EF_DATA_15_reg   (*((volatile unsigned int*) MIS_EF_DATA_15_reg))
#define get_MIS_EF_DATA_16_reg   (*((volatile unsigned int*) MIS_EF_DATA_16_reg))
#define get_MIS_EF_DATA_17_reg   (*((volatile unsigned int*) MIS_EF_DATA_17_reg))
#define get_MIS_EF_DATA_18_reg   (*((volatile unsigned int*) MIS_EF_DATA_18_reg))
#define get_MIS_EF_DATA_19_reg   (*((volatile unsigned int*) MIS_EF_DATA_19_reg))
#define get_MIS_EF_DATA_20_reg   (*((volatile unsigned int*) MIS_EF_DATA_20_reg))
#define get_MIS_EF_DATA_21_reg   (*((volatile unsigned int*) MIS_EF_DATA_21_reg))
#define get_MIS_EF_DATA_22_reg   (*((volatile unsigned int*) MIS_EF_DATA_22_reg))
#define get_MIS_EF_DATA_23_reg   (*((volatile unsigned int*) MIS_EF_DATA_23_reg))
#define get_MIS_EF_DATA_24_reg   (*((volatile unsigned int*) MIS_EF_DATA_24_reg))
#define get_MIS_EF_DATA_25_reg   (*((volatile unsigned int*) MIS_EF_DATA_25_reg))
#define get_MIS_EF_DATA_26_reg   (*((volatile unsigned int*) MIS_EF_DATA_26_reg))
#define get_MIS_EF_DATA_27_reg   (*((volatile unsigned int*) MIS_EF_DATA_27_reg))
#define get_MIS_EF_DATA_28_reg   (*((volatile unsigned int*) MIS_EF_DATA_28_reg))
#define get_MIS_EF_DATA_29_reg   (*((volatile unsigned int*) MIS_EF_DATA_29_reg))
#define get_MIS_EF_DATA_30_reg   (*((volatile unsigned int*) MIS_EF_DATA_30_reg))
#define get_MIS_EF_DATA_31_reg   (*((volatile unsigned int*) MIS_EF_DATA_31_reg))
#define get_MIS_EF_DATA_32_reg   (*((volatile unsigned int*) MIS_EF_DATA_32_reg))
#define get_MIS_EF_DATA_33_reg   (*((volatile unsigned int*) MIS_EF_DATA_33_reg))
#define get_MIS_EF_DATA_34_reg   (*((volatile unsigned int*) MIS_EF_DATA_34_reg))
#define get_MIS_EF_DATA_35_reg   (*((volatile unsigned int*) MIS_EF_DATA_35_reg))
#define get_MIS_EF_DATA_36_reg   (*((volatile unsigned int*) MIS_EF_DATA_36_reg))
#define get_MIS_EF_DATA_37_reg   (*((volatile unsigned int*) MIS_EF_DATA_37_reg))
#define get_MIS_EF_DATA_38_reg   (*((volatile unsigned int*) MIS_EF_DATA_38_reg))
#define get_MIS_EF_DATA_39_reg   (*((volatile unsigned int*) MIS_EF_DATA_39_reg))
#define get_MIS_EF_DATA_40_reg   (*((volatile unsigned int*) MIS_EF_DATA_40_reg))
#define get_MIS_EF_DATA_41_reg   (*((volatile unsigned int*) MIS_EF_DATA_41_reg))
#define get_MIS_EF_DATA_42_reg   (*((volatile unsigned int*) MIS_EF_DATA_42_reg))
#define get_MIS_EF_DATA_43_reg   (*((volatile unsigned int*) MIS_EF_DATA_43_reg))
#define get_MIS_EF_DATA_44_reg   (*((volatile unsigned int*) MIS_EF_DATA_44_reg))
#define get_MIS_EF_DATA_45_reg   (*((volatile unsigned int*) MIS_EF_DATA_45_reg))
#define get_MIS_EF_DATA_46_reg   (*((volatile unsigned int*) MIS_EF_DATA_46_reg))
#define get_MIS_EF_DATA_47_reg   (*((volatile unsigned int*) MIS_EF_DATA_47_reg))
#define get_MIS_EF_DATA_48_reg   (*((volatile unsigned int*) MIS_EF_DATA_48_reg))
#define get_MIS_EF_DATA_49_reg   (*((volatile unsigned int*) MIS_EF_DATA_49_reg))
#define get_MIS_EF_DATA_50_reg   (*((volatile unsigned int*) MIS_EF_DATA_50_reg))
#define get_MIS_EF_DATA_51_reg   (*((volatile unsigned int*) MIS_EF_DATA_51_reg))
#define get_MIS_EF_DATA_52_reg   (*((volatile unsigned int*) MIS_EF_DATA_52_reg))
#define get_MIS_EF_DATA_53_reg   (*((volatile unsigned int*) MIS_EF_DATA_53_reg))
#define get_MIS_EF_DATA_54_reg   (*((volatile unsigned int*) MIS_EF_DATA_54_reg))
#define get_MIS_EF_DATA_55_reg   (*((volatile unsigned int*) MIS_EF_DATA_55_reg))
#define get_MIS_EF_DATA_56_reg   (*((volatile unsigned int*) MIS_EF_DATA_56_reg))
#define get_MIS_EF_DATA_57_reg   (*((volatile unsigned int*) MIS_EF_DATA_57_reg))
#define get_MIS_EF_DATA_58_reg   (*((volatile unsigned int*) MIS_EF_DATA_58_reg))
#define get_MIS_EF_DATA_59_reg   (*((volatile unsigned int*) MIS_EF_DATA_59_reg))
#define get_MIS_EF_DATA_60_reg   (*((volatile unsigned int*) MIS_EF_DATA_60_reg))
#define get_MIS_EF_DATA_61_reg   (*((volatile unsigned int*) MIS_EF_DATA_61_reg))
#define get_MIS_EF_DATA_62_reg   (*((volatile unsigned int*) MIS_EF_DATA_62_reg))
#define get_MIS_EF_DATA_63_reg   (*((volatile unsigned int*) MIS_EF_DATA_63_reg))
#define get_MIS_EF_DATA_64_reg   (*((volatile unsigned int*) MIS_EF_DATA_64_reg))
#define get_MIS_EF_DATA_65_reg   (*((volatile unsigned int*) MIS_EF_DATA_65_reg))
#define get_MIS_EF_DATA_66_reg   (*((volatile unsigned int*) MIS_EF_DATA_66_reg))
#define get_MIS_EF_DATA_67_reg   (*((volatile unsigned int*) MIS_EF_DATA_67_reg))
#define get_MIS_EF_DATA_68_reg   (*((volatile unsigned int*) MIS_EF_DATA_68_reg))
#define get_MIS_EF_DATA_69_reg   (*((volatile unsigned int*) MIS_EF_DATA_69_reg))
#define get_MIS_EF_DATA_70_reg   (*((volatile unsigned int*) MIS_EF_DATA_70_reg))
#define get_MIS_EF_DATA_71_reg   (*((volatile unsigned int*) MIS_EF_DATA_71_reg))
#define get_MIS_EF_DATA_72_reg   (*((volatile unsigned int*) MIS_EF_DATA_72_reg))
#define get_MIS_EF_DATA_73_reg   (*((volatile unsigned int*) MIS_EF_DATA_73_reg))
#define MIS_EF_DATA_0_inst_adr                                                       "0x0000"
#define MIS_EF_DATA_1_inst_adr                                                       "0x0001"
#define MIS_EF_DATA_2_inst_adr                                                       "0x0002"
#define MIS_EF_DATA_3_inst_adr                                                       "0x0003"
#define MIS_EF_DATA_4_inst_adr                                                       "0x0004"
#define MIS_EF_DATA_5_inst_adr                                                       "0x0005"
#define MIS_EF_DATA_6_inst_adr                                                       "0x0006"
#define MIS_EF_DATA_7_inst_adr                                                       "0x0007"
#define MIS_EF_DATA_8_inst_adr                                                       "0x0008"
#define MIS_EF_DATA_9_inst_adr                                                       "0x0009"
#define MIS_EF_DATA_10_inst_adr                                                       "0x000A"
#define MIS_EF_DATA_11_inst_adr                                                       "0x000B"
#define MIS_EF_DATA_12_inst_adr                                                       "0x000C"
#define MIS_EF_DATA_13_inst_adr                                                       "0x000D"
#define MIS_EF_DATA_14_inst_adr                                                       "0x000E"
#define MIS_EF_DATA_15_inst_adr                                                       "0x000F"
#define MIS_EF_DATA_16_inst_adr                                                       "0x0010"
#define MIS_EF_DATA_17_inst_adr                                                       "0x0011"
#define MIS_EF_DATA_18_inst_adr                                                       "0x0012"
#define MIS_EF_DATA_19_inst_adr                                                       "0x0013"
#define MIS_EF_DATA_20_inst_adr                                                       "0x0014"
#define MIS_EF_DATA_21_inst_adr                                                       "0x0015"
#define MIS_EF_DATA_22_inst_adr                                                       "0x0016"
#define MIS_EF_DATA_23_inst_adr                                                       "0x0017"
#define MIS_EF_DATA_24_inst_adr                                                       "0x0018"
#define MIS_EF_DATA_25_inst_adr                                                       "0x0019"
#define MIS_EF_DATA_26_inst_adr                                                       "0x001A"
#define MIS_EF_DATA_27_inst_adr                                                       "0x001B"
#define MIS_EF_DATA_28_inst_adr                                                       "0x001C"
#define MIS_EF_DATA_29_inst_adr                                                       "0x001D"
#define MIS_EF_DATA_30_inst_adr                                                       "0x001E"
#define MIS_EF_DATA_31_inst_adr                                                       "0x001F"
#define MIS_EF_DATA_32_inst_adr                                                       "0x0020"
#define MIS_EF_DATA_33_inst_adr                                                       "0x0021"
#define MIS_EF_DATA_34_inst_adr                                                       "0x0022"
#define MIS_EF_DATA_35_inst_adr                                                       "0x0023"
#define MIS_EF_DATA_36_inst_adr                                                       "0x0024"
#define MIS_EF_DATA_37_inst_adr                                                       "0x0025"
#define MIS_EF_DATA_38_inst_adr                                                       "0x0026"
#define MIS_EF_DATA_39_inst_adr                                                       "0x0027"
#define MIS_EF_DATA_40_inst_adr                                                       "0x0028"
#define MIS_EF_DATA_41_inst_adr                                                       "0x0029"
#define MIS_EF_DATA_42_inst_adr                                                       "0x002A"
#define MIS_EF_DATA_43_inst_adr                                                       "0x002B"
#define MIS_EF_DATA_44_inst_adr                                                       "0x002C"
#define MIS_EF_DATA_45_inst_adr                                                       "0x002D"
#define MIS_EF_DATA_46_inst_adr                                                       "0x002E"
#define MIS_EF_DATA_47_inst_adr                                                       "0x002F"
#define MIS_EF_DATA_48_inst_adr                                                       "0x0030"
#define MIS_EF_DATA_49_inst_adr                                                       "0x0031"
#define MIS_EF_DATA_50_inst_adr                                                       "0x0032"
#define MIS_EF_DATA_51_inst_adr                                                       "0x0033"
#define MIS_EF_DATA_52_inst_adr                                                       "0x0034"
#define MIS_EF_DATA_53_inst_adr                                                       "0x0035"
#define MIS_EF_DATA_54_inst_adr                                                       "0x0036"
#define MIS_EF_DATA_55_inst_adr                                                       "0x0037"
#define MIS_EF_DATA_56_inst_adr                                                       "0x0038"
#define MIS_EF_DATA_57_inst_adr                                                       "0x0039"
#define MIS_EF_DATA_58_inst_adr                                                       "0x003A"
#define MIS_EF_DATA_59_inst_adr                                                       "0x003B"
#define MIS_EF_DATA_60_inst_adr                                                       "0x003C"
#define MIS_EF_DATA_61_inst_adr                                                       "0x003D"
#define MIS_EF_DATA_62_inst_adr                                                       "0x003E"
#define MIS_EF_DATA_63_inst_adr                                                       "0x003F"
#define MIS_EF_DATA_64_inst_adr                                                       "0x0040"
#define MIS_EF_DATA_65_inst_adr                                                       "0x0041"
#define MIS_EF_DATA_66_inst_adr                                                       "0x0042"
#define MIS_EF_DATA_67_inst_adr                                                       "0x0043"
#define MIS_EF_DATA_68_inst_adr                                                       "0x0044"
#define MIS_EF_DATA_69_inst_adr                                                       "0x0045"
#define MIS_EF_DATA_70_inst_adr                                                       "0x0046"
#define MIS_EF_DATA_71_inst_adr                                                       "0x0047"
#define MIS_EF_DATA_72_inst_adr                                                       "0x0048"
#define MIS_EF_DATA_73_inst_adr                                                       "0x0049"
#define MIS_EF_DATA_0_inst                                                           0x0000
#define MIS_EF_DATA_1_inst                                                           0x0001
#define MIS_EF_DATA_2_inst                                                           0x0002
#define MIS_EF_DATA_3_inst                                                           0x0003
#define MIS_EF_DATA_4_inst                                                           0x0004
#define MIS_EF_DATA_5_inst                                                           0x0005
#define MIS_EF_DATA_6_inst                                                           0x0006
#define MIS_EF_DATA_7_inst                                                           0x0007
#define MIS_EF_DATA_8_inst                                                           0x0008
#define MIS_EF_DATA_9_inst                                                           0x0009
#define MIS_EF_DATA_10_inst                                                           0x000A
#define MIS_EF_DATA_11_inst                                                           0x000B
#define MIS_EF_DATA_12_inst                                                           0x000C
#define MIS_EF_DATA_13_inst                                                           0x000D
#define MIS_EF_DATA_14_inst                                                           0x000E
#define MIS_EF_DATA_15_inst                                                           0x000F
#define MIS_EF_DATA_16_inst                                                           0x0010
#define MIS_EF_DATA_17_inst                                                           0x0011
#define MIS_EF_DATA_18_inst                                                           0x0012
#define MIS_EF_DATA_19_inst                                                           0x0013
#define MIS_EF_DATA_20_inst                                                           0x0014
#define MIS_EF_DATA_21_inst                                                           0x0015
#define MIS_EF_DATA_22_inst                                                           0x0016
#define MIS_EF_DATA_23_inst                                                           0x0017
#define MIS_EF_DATA_24_inst                                                           0x0018
#define MIS_EF_DATA_25_inst                                                           0x0019
#define MIS_EF_DATA_26_inst                                                           0x001A
#define MIS_EF_DATA_27_inst                                                           0x001B
#define MIS_EF_DATA_28_inst                                                           0x001C
#define MIS_EF_DATA_29_inst                                                           0x001D
#define MIS_EF_DATA_30_inst                                                           0x001E
#define MIS_EF_DATA_31_inst                                                           0x001F
#define MIS_EF_DATA_32_inst                                                           0x0020
#define MIS_EF_DATA_33_inst                                                           0x0021
#define MIS_EF_DATA_34_inst                                                           0x0022
#define MIS_EF_DATA_35_inst                                                           0x0023
#define MIS_EF_DATA_36_inst                                                           0x0024
#define MIS_EF_DATA_37_inst                                                           0x0025
#define MIS_EF_DATA_38_inst                                                           0x0026
#define MIS_EF_DATA_39_inst                                                           0x0027
#define MIS_EF_DATA_40_inst                                                           0x0028
#define MIS_EF_DATA_41_inst                                                           0x0029
#define MIS_EF_DATA_42_inst                                                           0x002A
#define MIS_EF_DATA_43_inst                                                           0x002B
#define MIS_EF_DATA_44_inst                                                           0x002C
#define MIS_EF_DATA_45_inst                                                           0x002D
#define MIS_EF_DATA_46_inst                                                           0x002E
#define MIS_EF_DATA_47_inst                                                           0x002F
#define MIS_EF_DATA_48_inst                                                           0x0030
#define MIS_EF_DATA_49_inst                                                           0x0031
#define MIS_EF_DATA_50_inst                                                           0x0032
#define MIS_EF_DATA_51_inst                                                           0x0033
#define MIS_EF_DATA_52_inst                                                           0x0034
#define MIS_EF_DATA_53_inst                                                           0x0035
#define MIS_EF_DATA_54_inst                                                           0x0036
#define MIS_EF_DATA_55_inst                                                           0x0037
#define MIS_EF_DATA_56_inst                                                           0x0038
#define MIS_EF_DATA_57_inst                                                           0x0039
#define MIS_EF_DATA_58_inst                                                           0x003A
#define MIS_EF_DATA_59_inst                                                           0x003B
#define MIS_EF_DATA_60_inst                                                           0x003C
#define MIS_EF_DATA_61_inst                                                           0x003D
#define MIS_EF_DATA_62_inst                                                           0x003E
#define MIS_EF_DATA_63_inst                                                           0x003F
#define MIS_EF_DATA_64_inst                                                           0x0040
#define MIS_EF_DATA_65_inst                                                           0x0041
#define MIS_EF_DATA_66_inst                                                           0x0042
#define MIS_EF_DATA_67_inst                                                           0x0043
#define MIS_EF_DATA_68_inst                                                           0x0044
#define MIS_EF_DATA_69_inst                                                           0x0045
#define MIS_EF_DATA_70_inst                                                           0x0046
#define MIS_EF_DATA_71_inst                                                           0x0047
#define MIS_EF_DATA_72_inst                                                           0x0048
#define MIS_EF_DATA_73_inst                                                           0x0049
#define MIS_EF_DATA_datao_shift                                                      (0)
#define MIS_EF_DATA_datao_mask                                                       (0xFFFFFFFF)
#define MIS_EF_DATA_datao(data)                                                      (0xFFFFFFFF&((data)<<0))
#define MIS_EF_DATA_datao_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define MIS_EF_DATA_get_datao(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define MIS_EF_CTRL                                                                  0x1801B980
#define MIS_EF_CTRL_reg_addr                                                         "0xB801B980"
#define MIS_EF_CTRL_reg                                                              0xB801B980
#define set_MIS_EF_CTRL_reg(data)   (*((volatile unsigned int*) MIS_EF_CTRL_reg)=data)
#define get_MIS_EF_CTRL_reg   (*((volatile unsigned int*) MIS_EF_CTRL_reg))
#define MIS_EF_CTRL_inst_adr                                                         "0x0060"
#define MIS_EF_CTRL_inst                                                             0x0060
#define MIS_EF_CTRL_write_enable4_shift                                              (23)
#define MIS_EF_CTRL_write_enable4_mask                                               (0x00800000)
#define MIS_EF_CTRL_write_enable4(data)                                              (0x00800000&((data)<<23))
#define MIS_EF_CTRL_write_enable4_src(data)                                          ((0x00800000&(data))>>23)
#define MIS_EF_CTRL_get_write_enable4(data)                                          ((0x00800000&(data))>>23)
#define MIS_EF_CTRL_en_pro_efuse_shift                                               (22)
#define MIS_EF_CTRL_en_pro_efuse_mask                                                (0x00400000)
#define MIS_EF_CTRL_en_pro_efuse(data)                                               (0x00400000&((data)<<22))
#define MIS_EF_CTRL_en_pro_efuse_src(data)                                           ((0x00400000&(data))>>22)
#define MIS_EF_CTRL_get_en_pro_efuse(data)                                           ((0x00400000&(data))>>22)
#define MIS_EF_CTRL_write_enable3_shift                                              (21)
#define MIS_EF_CTRL_write_enable3_mask                                               (0x00200000)
#define MIS_EF_CTRL_write_enable3(data)                                              (0x00200000&((data)<<21))
#define MIS_EF_CTRL_write_enable3_src(data)                                          ((0x00200000&(data))>>21)
#define MIS_EF_CTRL_get_write_enable3(data)                                          ((0x00200000&(data))>>21)
#define MIS_EF_CTRL_auto_rd_req_shift                                                (20)
#define MIS_EF_CTRL_auto_rd_req_mask                                                 (0x00100000)
#define MIS_EF_CTRL_auto_rd_req(data)                                                (0x00100000&((data)<<20))
#define MIS_EF_CTRL_auto_rd_req_src(data)                                            ((0x00100000&(data))>>20)
#define MIS_EF_CTRL_get_auto_rd_req(data)                                            ((0x00100000&(data))>>20)
#define MIS_EF_CTRL_write_enable2_shift                                              (18)
#define MIS_EF_CTRL_write_enable2_mask                                               (0x00040000)
#define MIS_EF_CTRL_write_enable2(data)                                              (0x00040000&((data)<<18))
#define MIS_EF_CTRL_write_enable2_src(data)                                          ((0x00040000&(data))>>18)
#define MIS_EF_CTRL_get_write_enable2(data)                                          ((0x00040000&(data))>>18)
#define MIS_EF_CTRL_tmrf_shift                                                       (16)
#define MIS_EF_CTRL_tmrf_mask                                                        (0x00030000)
#define MIS_EF_CTRL_tmrf(data)                                                       (0x00030000&((data)<<16))
#define MIS_EF_CTRL_tmrf_src(data)                                                   ((0x00030000&(data))>>16)
#define MIS_EF_CTRL_get_tmrf(data)                                                   ((0x00030000&(data))>>16)
#define MIS_EF_CTRL_write_enable1_shift                                              (15)
#define MIS_EF_CTRL_write_enable1_mask                                               (0x00008000)
#define MIS_EF_CTRL_write_enable1(data)                                              (0x00008000&((data)<<15))
#define MIS_EF_CTRL_write_enable1_src(data)                                          ((0x00008000&(data))>>15)
#define MIS_EF_CTRL_get_write_enable1(data)                                          ((0x00008000&(data))>>15)
#define MIS_EF_CTRL_program_shift                                                    (14)
#define MIS_EF_CTRL_program_mask                                                     (0x00004000)
#define MIS_EF_CTRL_program(data)                                                    (0x00004000&((data)<<14))
#define MIS_EF_CTRL_program_src(data)                                                ((0x00004000&(data))>>14)
#define MIS_EF_CTRL_get_program(data)                                                ((0x00004000&(data))>>14)
#define MIS_EF_CTRL_read_shift                                                       (13)
#define MIS_EF_CTRL_read_mask                                                        (0x00002000)
#define MIS_EF_CTRL_read(data)                                                       (0x00002000&((data)<<13))
#define MIS_EF_CTRL_read_src(data)                                                   ((0x00002000&(data))>>13)
#define MIS_EF_CTRL_get_read(data)                                                   ((0x00002000&(data))>>13)
#define MIS_EF_CTRL_enb_shift                                                        (12)
#define MIS_EF_CTRL_enb_mask                                                         (0x00001000)
#define MIS_EF_CTRL_enb(data)                                                        (0x00001000&((data)<<12))
#define MIS_EF_CTRL_enb_src(data)                                                    ((0x00001000&(data))>>12)
#define MIS_EF_CTRL_get_enb(data)                                                    ((0x00001000&(data))>>12)
#define MIS_EF_CTRL_b_shift                                                          (8)
#define MIS_EF_CTRL_b_mask                                                           (0x00000F00)
#define MIS_EF_CTRL_b(data)                                                          (0x00000F00&((data)<<8))
#define MIS_EF_CTRL_b_src(data)                                                      ((0x00000F00&(data))>>8)
#define MIS_EF_CTRL_get_b(data)                                                      ((0x00000F00&(data))>>8)
#define MIS_EF_CTRL_a_shift                                                          (0)
#define MIS_EF_CTRL_a_mask                                                           (0x000000FF)
#define MIS_EF_CTRL_a(data)                                                          (0x000000FF&((data)<<0))
#define MIS_EF_CTRL_a_src(data)                                                      ((0x000000FF&(data))>>0)
#define MIS_EF_CTRL_get_a(data)                                                      ((0x000000FF&(data))>>0)


#define MIS_EJ_COM_DATA                                                              0x1801B984
#define MIS_EJ_COM_DATA_reg_addr                                                     "0xB801B984"
#define MIS_EJ_COM_DATA_reg                                                          0xB801B984
#define set_MIS_EJ_COM_DATA_reg(data)   (*((volatile unsigned int*) MIS_EJ_COM_DATA_reg)=data)
#define get_MIS_EJ_COM_DATA_reg   (*((volatile unsigned int*) MIS_EJ_COM_DATA_reg))
#define MIS_EJ_COM_DATA_inst_adr                                                     "0x0061"
#define MIS_EJ_COM_DATA_inst                                                         0x0061
#define MIS_EJ_COM_DATA_comp_data_shift                                              (0)
#define MIS_EJ_COM_DATA_comp_data_mask                                               (0xFFFFFFFF)
#define MIS_EJ_COM_DATA_comp_data(data)                                              (0xFFFFFFFF&((data)<<0))
#define MIS_EJ_COM_DATA_comp_data_src(data)                                          ((0xFFFFFFFF&(data))>>0)
#define MIS_EJ_COM_DATA_get_comp_data(data)                                          ((0xFFFFFFFF&(data))>>0)


#define MIS_EJ_COM                                                                   0x1801B988
#define MIS_EJ_COM_reg_addr                                                          "0xB801B988"
#define MIS_EJ_COM_reg                                                               0xB801B988
#define set_MIS_EJ_COM_reg(data)   (*((volatile unsigned int*) MIS_EJ_COM_reg)=data)
#define get_MIS_EJ_COM_reg   (*((volatile unsigned int*) MIS_EJ_COM_reg))
#define MIS_EJ_COM_inst_adr                                                          "0x0062"
#define MIS_EJ_COM_inst                                                              0x0062
#define MIS_EJ_COM_ej_com_ok_shift                                                   (1)
#define MIS_EJ_COM_ej_com_ok_mask                                                    (0x00000002)
#define MIS_EJ_COM_ej_com_ok(data)                                                   (0x00000002&((data)<<1))
#define MIS_EJ_COM_ej_com_ok_src(data)                                               ((0x00000002&(data))>>1)
#define MIS_EJ_COM_get_ej_com_ok(data)                                               ((0x00000002&(data))>>1)
#define MIS_EJ_COM_ej_com_en_shift                                                   (0)
#define MIS_EJ_COM_ej_com_en_mask                                                    (0x00000001)
#define MIS_EJ_COM_ej_com_en(data)                                                   (0x00000001&((data)<<0))
#define MIS_EJ_COM_ej_com_en_src(data)                                               ((0x00000001&(data))>>0)
#define MIS_EJ_COM_get_ej_com_en(data)                                               ((0x00000001&(data))>>0)


#define MIS_EF_CTRL_ST                                                               0x1801B990
#define MIS_EF_CTRL_ST_reg_addr                                                      "0xB801B990"
#define MIS_EF_CTRL_ST_reg                                                           0xB801B990
#define set_MIS_EF_CTRL_ST_reg(data)   (*((volatile unsigned int*) MIS_EF_CTRL_ST_reg)=data)
#define get_MIS_EF_CTRL_ST_reg   (*((volatile unsigned int*) MIS_EF_CTRL_ST_reg))
#define MIS_EF_CTRL_ST_inst_adr                                                      "0x0064"
#define MIS_EF_CTRL_ST_inst                                                          0x0064
#define MIS_EF_CTRL_ST_pro_efuse_act_shift                                           (16)
#define MIS_EF_CTRL_ST_pro_efuse_act_mask                                            (0x00010000)
#define MIS_EF_CTRL_ST_pro_efuse_act(data)                                           (0x00010000&((data)<<16))
#define MIS_EF_CTRL_ST_pro_efuse_act_src(data)                                       ((0x00010000&(data))>>16)
#define MIS_EF_CTRL_ST_get_pro_efuse_act(data)                                       ((0x00010000&(data))>>16)
#define MIS_EF_CTRL_ST_crc_en_shift                                                  (0)
#define MIS_EF_CTRL_ST_crc_en_mask                                                   (0x00000001)
#define MIS_EF_CTRL_ST_crc_en(data)                                                  (0x00000001&((data)<<0))
#define MIS_EF_CTRL_ST_crc_en_src(data)                                              ((0x00000001&(data))>>0)
#define MIS_EF_CTRL_ST_get_crc_en(data)                                              ((0x00000001&(data))>>0)


#define MIS_EF_CRC                                                                   0x1801B994
#define MIS_EF_CRC_reg_addr                                                          "0xB801B994"
#define MIS_EF_CRC_reg                                                               0xB801B994
#define set_MIS_EF_CRC_reg(data)   (*((volatile unsigned int*) MIS_EF_CRC_reg)=data)
#define get_MIS_EF_CRC_reg   (*((volatile unsigned int*) MIS_EF_CRC_reg))
#define MIS_EF_CRC_inst_adr                                                          "0x0065"
#define MIS_EF_CRC_inst                                                              0x0065
#define MIS_EF_CRC_crc_ini_shift                                                     (16)
#define MIS_EF_CRC_crc_ini_mask                                                      (0xFFFF0000)
#define MIS_EF_CRC_crc_ini(data)                                                     (0xFFFF0000&((data)<<16))
#define MIS_EF_CRC_crc_ini_src(data)                                                 ((0xFFFF0000&(data))>>16)
#define MIS_EF_CRC_get_crc_ini(data)                                                 ((0xFFFF0000&(data))>>16)
#define MIS_EF_CRC_crc_out_shift                                                     (0)
#define MIS_EF_CRC_crc_out_mask                                                      (0x0000FFFF)
#define MIS_EF_CRC_crc_out(data)                                                     (0x0000FFFF&((data)<<0))
#define MIS_EF_CRC_crc_out_src(data)                                                 ((0x0000FFFF&(data))>>0)
#define MIS_EF_CRC_get_crc_out(data)                                                 ((0x0000FFFF&(data))>>0)


#endif
