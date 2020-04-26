/**************************************************************
// Spec Version                  : 0.0.9
// Parser Version                : DVR_Parser_6.0
// CModelGen Version             : 5.0 2009.04.09
// Naming Rule                   : Module_Register_Name
// Parse Option                  : Only Parse _op1
// Parse Address Region          : All Address Region 
// Firmware Header Generate Date : 2010/5/16 14:56:22
***************************************************************/


#ifndef _ISO_REG_H_INCLUDED_
#define _ISO_REG_H_INCLUDED_
#ifdef  _ISO_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     CECRxINT:1;
unsigned int     CECTxINT:1;
unsigned int     reserved_1:1;
unsigned int     GPIODA_INT:1;
unsigned int     GPIOA_INT:1;
unsigned int     VFD_ARDSWDA_INT:1;
unsigned int     VFD_ARDSWA_INT:1;
unsigned int     VFD_ARDKPADDA_INT:1;
unsigned int     VFD_ARDKPADA_INT:1;
unsigned int     VFD_WDONE_INT:1;
unsigned int     RTC_ALARM_INT:1;
unsigned int     RTC_HSEC_INT:1;
unsigned int     reserved_2:6;
unsigned int     IRDA_INT:1;
unsigned int     reserved_3:2;
unsigned int     UR0_INT:1;
unsigned int     TC3_INT:1;
unsigned int     write_data:1;
}ISO_ISR;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     CECRxINT:1;
unsigned int     CECTxINT:1;
unsigned int     reserved_1:1;
unsigned int     GPIODA_INT:1;
unsigned int     GPIOA_INT:1;
unsigned int     VFD_ARDSWDA_INT:1;
unsigned int     VFD_ARDSWA_INT:1;
unsigned int     VFD_ARDKPADDA_INT:1;
unsigned int     VFD_ARDKPADA_INT:1;
unsigned int     VFD_WDONE_INT:1;
unsigned int     RTC_ALARM_INT:1;
unsigned int     RTC_HSEC_INT:1;
unsigned int     reserved_2:6;
unsigned int     IRDA_INT:1;
unsigned int     reserved_3:3;
unsigned int     TC3_INT:1;
unsigned int     write_data:1;
}ISO_UMSK_ISR;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     INT10_DA:1;
unsigned int     INT9_DA:1;
unsigned int     INT8_DA:1;
unsigned int     INT7_DA:1;
unsigned int     INT6_DA:1;
unsigned int     INT5_DA:1;
unsigned int     INT4_DA:1;
unsigned int     INT3_DA:1;
unsigned int     INT2_DA:1;
unsigned int     INT1_DA:1;
unsigned int     INT0_DA:1;
unsigned int     reserved_1:4;
unsigned int     INT10_A:1;
unsigned int     INT9_A:1;
unsigned int     INT8_A:1;
unsigned int     INT7_A:1;
unsigned int     INT6_A:1;
unsigned int     INT5_A:1;
unsigned int     INT4_A:1;
unsigned int     INT3_A:1;
unsigned int     INT2_A:1;
unsigned int     INT1_A:1;
unsigned int     INT0_A:1;
unsigned int     write_data:1;
}ISO_UMSK_ISR_GP;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     INT47:1;
unsigned int     INT46:1;
unsigned int     INT45:1;
unsigned int     INT44:1;
unsigned int     INT43:1;
unsigned int     INT42:1;
unsigned int     INT41:1;
unsigned int     INT40:1;
unsigned int     INT39:1;
unsigned int     INT38:1;
unsigned int     INT37:1;
unsigned int     INT36:1;
unsigned int     INT35:1;
unsigned int     INT34:1;
unsigned int     INT33:1;
unsigned int     INT32:1;
unsigned int     INT31:1;
unsigned int     INT30:1;
unsigned int     INT29:1;
unsigned int     INT28:1;
unsigned int     reserved_1:3;
unsigned int     write_data:1;
}ISO_UMSK_ISR_KPADAH;

typedef struct 
{
unsigned int     INT27:1;
unsigned int     INT26:1;
unsigned int     INT25:1;
unsigned int     INT24:1;
unsigned int     INT23:1;
unsigned int     INT22:1;
unsigned int     INT21:1;
unsigned int     INT20:1;
unsigned int     INT19:1;
unsigned int     INT18:1;
unsigned int     INT17:1;
unsigned int     INT16:1;
unsigned int     INT15:1;
unsigned int     INT14:1;
unsigned int     INT13:1;
unsigned int     INT12:1;
unsigned int     INT11:1;
unsigned int     INT10:1;
unsigned int     INT9:1;
unsigned int     INT8:1;
unsigned int     INT7:1;
unsigned int     INT6:1;
unsigned int     INT5:1;
unsigned int     INT4:1;
unsigned int     INT3:1;
unsigned int     INT2:1;
unsigned int     INT1:1;
unsigned int     INT0:1;
unsigned int     reserved_0:3;
unsigned int     write_data:1;
}ISO_UMSK_ISR_KPADAL;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     INT47:1;
unsigned int     INT46:1;
unsigned int     INT45:1;
unsigned int     INT44:1;
unsigned int     INT43:1;
unsigned int     INT42:1;
unsigned int     INT41:1;
unsigned int     INT40:1;
unsigned int     INT39:1;
unsigned int     INT38:1;
unsigned int     INT37:1;
unsigned int     INT36:1;
unsigned int     INT35:1;
unsigned int     INT34:1;
unsigned int     INT33:1;
unsigned int     INT32:1;
unsigned int     INT31:1;
unsigned int     INT30:1;
unsigned int     INT29:1;
unsigned int     INT28:1;
unsigned int     reserved_1:3;
unsigned int     write_data:1;
}ISO_UMSK_ISR_KPADDAH;

typedef struct 
{
unsigned int     INT27:1;
unsigned int     INT26:1;
unsigned int     INT25:1;
unsigned int     INT24:1;
unsigned int     INT23:1;
unsigned int     INT22:1;
unsigned int     INT21:1;
unsigned int     INT20:1;
unsigned int     INT19:1;
unsigned int     INT18:1;
unsigned int     INT17:1;
unsigned int     INT16:1;
unsigned int     INT15:1;
unsigned int     INT14:1;
unsigned int     INT13:1;
unsigned int     INT12:1;
unsigned int     INT11:1;
unsigned int     INT10:1;
unsigned int     INT9:1;
unsigned int     INT8:1;
unsigned int     INT7:1;
unsigned int     INT6:1;
unsigned int     INT5:1;
unsigned int     INT4:1;
unsigned int     INT3:1;
unsigned int     INT2:1;
unsigned int     INT1:1;
unsigned int     INT0:1;
unsigned int     reserved_0:3;
unsigned int     write_data:1;
}ISO_UMSK_ISR_KPADDAL;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     DA_INT3:1;
unsigned int     DA_INT2:1;
unsigned int     DA_INT1:1;
unsigned int     DA_INT0:1;
unsigned int     A_INT3:1;
unsigned int     A_INT2:1;
unsigned int     A_INT1:1;
unsigned int     A_INT0:1;
unsigned int     reserved_1:3;
unsigned int     write_data:1;
}ISO_UMSK_ISR_SW;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     write_en5:1;
unsigned int     standby_dbg_sel:5;
unsigned int     write_en4:1;
unsigned int     standby_dbg_en:1;
unsigned int     write_en3:1;
unsigned int     sel1:4;
unsigned int     write_en2:1;
unsigned int     sel0:4;
unsigned int     write_en1:1;
unsigned int     enable:1;
}ISO_DBG;

typedef struct 
{
unsigned int     rvd31:1;
unsigned int     rvd30:1;
unsigned int     rvd29:1;
unsigned int     rvd28:1;
unsigned int     rvd27:1;
unsigned int     rvd26:1;
unsigned int     rvd25:1;
unsigned int     rvd24:1;
unsigned int     rvd23:1;
unsigned int     rvd22:1;
unsigned int     rvd21:1;
unsigned int     rvd20:1;
unsigned int     rvd19:1;
unsigned int     rvd18:1;
unsigned int     rvd17:1;
unsigned int     rvd16:1;
unsigned int     rvd15:1;
unsigned int     rvd14:1;
unsigned int     rvd13:1;
unsigned int     rvd12:1;
unsigned int     rvd11:1;
unsigned int     rvd10:1;
unsigned int     rvd9:1;
unsigned int     rvd8:1;
unsigned int     rvd7:1;
unsigned int     rvd6:1;
unsigned int     rvd5:1;
unsigned int     rvd4:1;
unsigned int     rvd3:1;
unsigned int     rvd2:1;
unsigned int     rvd1:1;
unsigned int     rvd0:1;
}ISO_DUMMY1;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     testmode:1;
unsigned int     nf_sel:1;
unsigned int     reserved_1:1;
unsigned int     bound_opt:11;
}ISO_CHIP_INFO1;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     pow_latch:18;
}ISO_CHIP_INFO2;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     write_en8:1;
unsigned int     dc_phy_cke_pd:1;
unsigned int     write_en7:1;
unsigned int     iso_mis_clk_en:1;
unsigned int     write_en6:1;
unsigned int     iso_rbus_sel:1;
unsigned int     write_en5:1;
unsigned int     iso_rst_mask:1;
unsigned int     write_en4:1;
unsigned int     clk_osc_on_en:1;
unsigned int     write_en3:1;
unsigned int     crt_sw_rstn:1;
unsigned int     write_en2:1;
unsigned int     iso_clk_sel:1;
unsigned int     write_en1:1;
unsigned int     ejtag_en:1;
}ISO_CTRL;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     hsec_int_en:1;
unsigned int     alarm_int_en:1;
}ISO_RTC;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     iso_ctrl_en1:1;
unsigned int     iso_ctrl_en0:1;
}ISO_CELL;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     v1_on:1;
}ISO_POWER;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     CECRx:1;
unsigned int     CECTx:1;
unsigned int     reserved_1:1;
unsigned int     GPIODA:1;
unsigned int     GPIOA:1;
unsigned int     VFD_ARDSWDA:1;
unsigned int     VFD_ARDSWA:1;
unsigned int     VFD_ARDKPADDA:1;
unsigned int     VFD_ARDKPADA:1;
unsigned int     VFD_WDONE:1;
unsigned int     RTC_ALARM:1;
unsigned int     RTC_HSEC:1;
unsigned int     reserved_2:6;
unsigned int     IRDA:1;
unsigned int     reserved_3:2;
unsigned int     UR0:1;
unsigned int     reserved_4:2;
}ISO_SCPU_INT_EN;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     CECRx:1;
unsigned int     CECTx:1;
unsigned int     reserved_1:1;
unsigned int     GPIODA:1;
unsigned int     GPIOA:1;
unsigned int     VFD_ARDSWDA:1;
unsigned int     VFD_ARDSWA:1;
unsigned int     VFD_ARDKPADDA:1;
unsigned int     VFD_ARDKPADA:1;
unsigned int     VFD_WDONE:1;
unsigned int     RTC_ALARM:1;
unsigned int     RTC_HSEC:1;
unsigned int     reserved_2:6;
unsigned int     IRDA:1;
unsigned int     reserved_3:2;
unsigned int     UR0:1;
unsigned int     reserved_4:2;
}ISO_KCPU_INT_EN;

typedef struct 
{
unsigned int     kcpu_boot_info:32;
}ISO_CPU_ST1;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     kcpu_boot_info_valid:1;
}ISO_CPU_ST1V;

typedef struct 
{
unsigned int     rvd31:1;
unsigned int     rvd30:1;
unsigned int     rvd29:1;
unsigned int     rvd28:1;
unsigned int     rvd27:1;
unsigned int     rvd26:1;
unsigned int     rvd25:1;
unsigned int     rvd24:1;
unsigned int     rvd23:1;
unsigned int     rvd22:1;
unsigned int     rvd21:1;
unsigned int     rvd20:1;
unsigned int     rvd19:1;
unsigned int     rvd18:1;
unsigned int     rvd17:1;
unsigned int     rvd16:1;
unsigned int     rvd15:1;
unsigned int     rvd14:1;
unsigned int     rvd13:1;
unsigned int     rvd12:1;
unsigned int     rvd11:1;
unsigned int     rvd10:1;
unsigned int     rvd9:1;
unsigned int     rvd8:1;
unsigned int     rvd7:1;
unsigned int     rvd6:1;
unsigned int     rvd5:1;
unsigned int     rvd4:1;
unsigned int     rvd3:1;
unsigned int     rvd2:1;
unsigned int     rvd1:1;
unsigned int     rvd0:1;
}ISO_DUMMY2;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     GPDIR:11;
}ISO_GPDIR;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     GPDATO:11;
}ISO_GPDATO;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     GPDATI:11;
}ISO_GPDATI;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     GP:11;
}ISO_GPIE;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     GPHA:11;
}ISO_GPDP;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     write_en1:1;
unsigned int     CLK1:3;
}ISO_GPDEB;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     KPADEN:1;
unsigned int     SWEN:1;
unsigned int     CLKSEL:1;
unsigned int     ENVFD:1;
}ISO_VFD_CTL;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     NACT_CSN:1;
unsigned int     B3C:1;
unsigned int     B2C:1;
unsigned int     B1C:1;
unsigned int     B0C:1;
unsigned int     ENB3:1;
unsigned int     ENB2:1;
unsigned int     ENB1:1;
unsigned int     ENB0:1;
unsigned int     reserved_1:2;
unsigned int     WD:1;
unsigned int     WDIE:1;
}ISO_VFD_WRCTL;

typedef struct 
{
unsigned int     BYTE3:8;
unsigned int     BYTE2:8;
unsigned int     BYTE1:8;
unsigned int     BYTE0:8;
}ISO_VFDO;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     KPADBN:3;
unsigned int     reserved_1:2;
unsigned int     SWHA:1;
unsigned int     KPADHA:1;
unsigned int     reserved_2:2;
unsigned int     ARDP:2;
}ISO_VFD_ARDCTL;

typedef struct 
{
unsigned int     KPADIE:32;
}ISO_VFD_KPADLIE;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     KPADIE:16;
}ISO_VFD_KPADHIE;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     SWIE:4;
}ISO_VFD_SWIE;

typedef struct 
{
unsigned int     ST_31_24:8;
unsigned int     ST_23_16:8;
unsigned int     ST_15_8:8;
unsigned int     ST_7_0:8;
}ISO_VFD_ARDKPADL;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ST_15_8:8;
unsigned int     ST_7_0:8;
}ISO_VFD_ARDKPADH;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     RvdA:12;
unsigned int     ST:4;
}ISO_VFD_ARDSW;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     CLKSEL_1:1;
unsigned int     Tclk_csn:8;
unsigned int     Twait1:8;
}ISO_VFD_CTL1;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     standby_smt:1;
unsigned int     standby_e2:1;
unsigned int     standby_pud_en:1;
unsigned int     standby_pud_sel:1;
unsigned int     vfd_d_smt:1;
unsigned int     vfd_d_e2:1;
unsigned int     vfd_d_pud_en:1;
unsigned int     vfd_d_pud_sel:1;
unsigned int     vfd_clk_smt:1;
unsigned int     vfd_clk_e2:1;
unsigned int     vfd_clk_pud_en:1;
unsigned int     vfd_clk_pud_sel:1;
unsigned int     vfd_cs_n_smt:1;
unsigned int     vfd_cs_n_e2:1;
unsigned int     vfd_cs_n_pud_en:1;
unsigned int     vfd_cs_n_pud_sel:1;
unsigned int     ir_in_smt:1;
unsigned int     ir_in_e2:1;
unsigned int     ir_in_pud_en:1;
unsigned int     ir_in_pud_sel:1;
unsigned int     usb_ovrcur_flag_n_1_smt:1;
unsigned int     usb_ovrcur_flag_n_1_e2:1;
unsigned int     usb_ovrcur_flag_n_1_pud_en:1;
unsigned int     usb_ovrcur_flag_n_1_pud_sel:1;
unsigned int     usb_ovrcur_flag_n_0_smt:1;
unsigned int     usb_ovrcur_flag_n_0_e2:1;
unsigned int     usb_ovrcur_flag_n_0_pud_en:1;
unsigned int     usb_ovrcur_flag_n_0_pud_sel:1;
}ISO_PFUNC1;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     ur0_tx_smt:1;
unsigned int     ur0_tx_e2:1;
unsigned int     ur0_tx_pud_en:1;
unsigned int     ur0_tx_pud_sel:1;
unsigned int     ur0_rx_smt:1;
unsigned int     ur0_rx_e2:1;
unsigned int     ur0_rx_pud_en:1;
unsigned int     ur0_rx_pud_sel:1;
unsigned int     etn_led_link_smt:1;
unsigned int     etn_led_link_e2:1;
unsigned int     etn_led_link_pud_en:1;
unsigned int     etn_led_link_pud_sel:1;
unsigned int     etn_led_rxtx_smt:1;
unsigned int     etn_led_rxtx_e2:1;
unsigned int     etn_led_rxtx_pud_en:1;
unsigned int     etn_led_rxtx_pud_sel:1;
}ISO_PFUNC2;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     ur0_tx:2;
unsigned int     ur0_rx:2;
unsigned int     etn_led_link:2;
unsigned int     etn_led_rxtx:2;
unsigned int     usb1:2;
unsigned int     usb0:2;
unsigned int     ir_in:2;
unsigned int     vfd_cs_n:2;
unsigned int     vfd_clk:2;
unsigned int     vfd_d:2;
}ISO_MUXPAD;

typedef struct 
{
unsigned int     IRRBL:8;
unsigned int     IRRML:8;
unsigned int     IRRDZL:8;
unsigned int     IRRDOL:8;
}ISO_IR_PSR;

typedef struct 
{
unsigned int     reserved_0:10;
unsigned int     IRRDL:6;
unsigned int     IRRRL:8;
unsigned int     IRRSL:8;
}ISO_IR_PER;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     IRFD:16;
}ISO_IR_SF;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     IRIOTCDP:16;
}ISO_IR_DPIR;

typedef struct 
{
unsigned int     IRSR:1;
unsigned int     reserved_0:6;
unsigned int     toshiba_en:1;
unsigned int     IREDNM:1;
unsigned int     reserved_1:1;
unsigned int     IREDN:6;
unsigned int     reserved_2:1;
unsigned int     raw_fifo_ov:1;
unsigned int     raw_fifo_val:1;
unsigned int     RAW_EN:1;
unsigned int     MLAE:1;
unsigned int     IRIE:1;
unsigned int     IRRES:1;
unsigned int     IRUE:1;
unsigned int     IRCM:1;
unsigned int     IRBME:1;
unsigned int     IRDPM:1;
unsigned int     IRDN:5;
}ISO_IR_CR;

typedef struct 
{
unsigned int     IRRP:32;
}ISO_IR_RP;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     rmt_big:1;
unsigned int     raw_fifo_ov:1;
unsigned int     raw_fifo_val:1;
unsigned int     IRRF:1;
unsigned int     IRDVF:1;
}ISO_IR_SR;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     write_en2:1;
unsigned int     stop_sample:1;
unsigned int     stop_time:16;
unsigned int     reserved_1:1;
unsigned int     write_en1:1;
unsigned int     fifo_thr:6;
}ISO_IR_RAW_CTRL;

typedef struct 
{
unsigned int     DAT:32;
}ISO_IR_RAW_FF;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     CNT:16;
}ISO_IR_RAW_SAMPLE_TIME;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     VAL:6;
}ISO_IR_RAW_WL;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     LEN:8;
unsigned int     CLK:16;
}ISO_IR_RAW_DEB;

typedef struct 
{
unsigned int     IRRBL:8;
unsigned int     IRRML:8;
unsigned int     IRRDZL:8;
unsigned int     IRRDOL:8;
}ISO_IR_PSR_UP;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     IRRRL:8;
unsigned int     IRRSL:8;
}ISO_IR_PER_UP;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     RC6_EN:1;
unsigned int     IRTR:8;
}ISO_IR_CTRL_RC6;

typedef struct 
{
unsigned int     IRRP:32;
}ISO_IR_RP2;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     cec_mode:2;
unsigned int     broadcast_addr:1;
unsigned int     dac_en:1;
unsigned int     logical_addr:4;
}ISO_CEC_CR0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     timer_div:8;
}ISO_CEC_CR1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     pre_div:8;
}ISO_CEC_CR2;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     unreg_ack_en:1;
unsigned int     reserved_1:1;
unsigned int     Cdc_arbitration_en:1;
unsigned int     DACDataIn:5;
}ISO_CEC_CR3;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     cec_pad_in:1;
unsigned int     wt_cnt:6;
}ISO_CEC_RT0;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     lattest:1;
unsigned int     retry_no:4;
}ISO_CEC_RT1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rx_en:1;
unsigned int     rx_rst:1;
unsigned int     rx_continuous:1;
unsigned int     rx_int_en:1;
unsigned int     init_addr:4;
}ISO_CEC_RX0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rx_eom:1;
unsigned int     rx_int:1;
unsigned int     rx_fifo_ov:1;
unsigned int     rx_fifo_cnt:5;
}ISO_CEC_RX1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tx_en:1;
unsigned int     tx_rst:1;
unsigned int     tx_continuous:1;
unsigned int     tx_int_en:1;
unsigned int     dest_addr:4;
}ISO_CEC_TX0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tx_eom:1;
unsigned int     tx_int:1;
unsigned int     tx_fifo_ud:1;
unsigned int     tx_fifo_cnt:5;
}ISO_CEC_TX1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     tx_dat:8;
}ISO_CEC_TX_FIFO;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     rx_dat:8;
}ISO_CEC_RX_FIFO;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     low:8;
}ISO_CEC_RX_START0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     period:8;
}ISO_CEC_RX_START1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     sample:8;
}ISO_CEC_RX_DATA0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     period:8;
}ISO_CEC_RX_DATA1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     low:8;
}ISO_CEC_TX_START0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     high:8;
}ISO_CEC_TX_START1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     low:8;
}ISO_CEC_TX_DATA0;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     01:8;
}ISO_CEC_TX_DATA1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     high:8;
}ISO_CEC_TX_DATA2;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     tx_addr_en:1;
unsigned int     tx_addr:4;
}ISO_CEC_TX_LBK;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     SET:1;
}ISO_DRM_ST;

typedef struct 
{
unsigned int     VAL:32;
}ISO_DRM_SECURE_CLK;

typedef struct 
{
unsigned int     VAL:32;
}ISO_DRM_ELAPSED;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST1;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST2;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     clk_div_en:1;
unsigned int     clk_sel:1;
}ISO_DRM_CTRL;

typedef struct 
{
unsigned int     init:32;
}ISO_DRM_CLK_DIV;

typedef struct 
{
unsigned int     TC3TVR:32;
}ISO_TC3TVR;

typedef struct 
{
unsigned int     TC3CVR:32;
}ISO_TC3CVR;

typedef struct 
{
unsigned int     TC3En:1;
unsigned int     TC3Mode:1;
unsigned int     TC3Pause:1;
unsigned int     RvdA:5;
unsigned int     reserved_0:24;
}ISO_TC3CR;

typedef struct 
{
unsigned int     TC3IE:1;
unsigned int     reserved_0:31;
}ISO_TC3ICR;

typedef struct 
{
unsigned int     dat:32;
}ISO_NORST;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST3;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST4;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST5;

typedef struct 
{
unsigned int     VAL:32;
}ISO_COLD_RST6;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     dvse_300:1;
unsigned int     dvs_300:4;
}ISO_KCPU_SRAM_CTRL;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     resume_cycle_sel:1;
unsigned int     kcpu_bist1_en:1;
unsigned int     kcpu_drf1_bist_en:1;
unsigned int     kcpu_bist0_en:1;
unsigned int     kcpu_drf0_bist_en:1;
unsigned int     write_data:1;
}ISO_KCPU_BIST_CTRL;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     kcpu_bist0_fail3:1;
unsigned int     kcpu_bist0_fail2:1;
unsigned int     kcpu_bist0_fail1:1;
unsigned int     kcpu_bist0_fail0:1;
unsigned int     kcpu_bist0_done:1;
}ISO_KCPU_BIST0_ST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     kcpu_bist1_fail0:1;
unsigned int     kcpu_bist1_done:1;
}ISO_KCPU_BIST1_ST;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     kcpu_drf1_start_pause:1;
unsigned int     kcpu_drf1_test_resume:1;
unsigned int     kcpu_drf0_start_pause:1;
unsigned int     kcpu_drf0_test_resume:1;
}ISO_DRF_CTRL;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     kcpu_drf0_bist_fail3:1;
unsigned int     kcpu_drf0_bist_fail2:1;
unsigned int     kcpu_drf0_bist_fail1:1;
unsigned int     kcpu_drf0_bist_fail0:1;
unsigned int     kcpu_drf0_bist_done:1;
}ISO_KCPU_DRF0_BIST_ST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     kcpu_drf1_bist_fail0:1;
unsigned int     kcpu_drf1_bist_done:1;
}ISO_KCPU_DRF1_BIST_ST;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     kcpu_bisr_run1_pwr_rstn:1;
unsigned int     kcpu_bisr_pwr_rstn:1;
unsigned int     kcpu_bisr_hold_remap:1;
unsigned int     reserved_1:2;
unsigned int     bisr_run:1;
unsigned int     write_data:1;
}ISO_KCPU_BISR_CTRL;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     kcpu_bisr_fail4:1;
unsigned int     kcpu_bisr_fail3:1;
unsigned int     kcpu_bisr_fail2:1;
unsigned int     kcpu_bisr_fail1:1;
unsigned int     kcpu_bisr_fail0:1;
unsigned int     kcpu_bisr_done:1;
}ISO_KCPU_BISR_ST1;

typedef struct 
{
unsigned int     kcpu_bisr_so4:7;
unsigned int     kcpu_bisr_so3:7;
unsigned int     kcpu_bisr_so2:6;
unsigned int     kcpu_bisr_so1:6;
unsigned int     kcpu_bisr_so0:6;
}ISO_KCPU_BISR_ST2;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     kcpu_bisr_repair_2:1;
unsigned int     kcpu_bisr_repair_1:1;
}ISO_KCPU_BISR_ST3;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     kcpu_drf_bisr_mode:1;
unsigned int     write_data:1;
}ISO_KCPU_BISR_ST4;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     kcpu_drf_bisr_start_pause:1;
unsigned int     kcpu_drf_bisr_test_resume:1;
}ISO_KCPU_BISR_ST5;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     kcpu_drf_bisr_fail4:1;
unsigned int     kcpu_drf_bisr_fail3:1;
unsigned int     kcpu_drf_bisr_fail2:1;
unsigned int     kcpu_drf_bisr_fail1:1;
unsigned int     kcpu_drf_bisr_fail0:1;
unsigned int     kcpu_drf_bisr_done:1;
}ISO_KCPU_BISR_ST6;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     scpu_boot_info_valid:1;
}ISO_CPU_ST2V;

typedef struct 
{
unsigned int     scpu_boot_info:32;
}ISO_CPU_ST2;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     write_enable3:1;
unsigned int     sel1:5;
unsigned int     write_enable2:1;
unsigned int     sel0:5;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}ISO_KCPU_DBG;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     analogy_mode_in:1;
}ISO_ANLG;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     DLL:8;
}ISO_U0RBR_THR_DLL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     DLH:8;
}ISO_U0IER_DLH;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     FIFO16:2;
unsigned int     reserved_1:2;
unsigned int     IID:4;
}ISO_U0IIR_FCR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     DLAB:1;
unsigned int     BRK:1;
unsigned int     reserved_1:1;
unsigned int     EPS:1;
unsigned int     PEN:1;
unsigned int     STB:1;
unsigned int     WLS:2;
}ISO_U0LCR;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     AFCE:1;
unsigned int     LOOP:1;
unsigned int     reserved_1:2;
unsigned int     RTS:1;
unsigned int     DTR:1;
}ISO_U0MCR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     RFE:1;
unsigned int     TEMT:1;
unsigned int     THRE:1;
unsigned int     BI:1;
unsigned int     FE:1;
unsigned int     PE:1;
unsigned int     OE:1;
unsigned int     DR:1;
}ISO_U0LSR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     DCD:1;
unsigned int     RI:1;
unsigned int     DSR:1;
unsigned int     CTS:1;
unsigned int     DDCD:1;
unsigned int     TERI:1;
unsigned int     DDSR:1;
unsigned int     DCTS:1;
}ISO_U0MSR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     SCR:8;
}ISO_U0SCR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     RBD:8;
}ISO_U0SRBR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     FAR:8;
}ISO_U0FAR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     FAR:8;
}ISO_U0TFR;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     RFFE:1;
unsigned int     RFPF:1;
unsigned int     RFWD:8;
}ISO_U0RFW;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     RFF:1;
unsigned int     RFNE:1;
unsigned int     TFE:1;
unsigned int     TFNF:1;
unsigned int     BUSY:1;
}ISO_U0USR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     TFL:8;
}ISO_U0TFL;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     RFL:8;
}ISO_U0RFL;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     XFR:1;
unsigned int     RFR:1;
unsigned int     UR:1;
}ISO_U0SRR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     SBCR:1;
}ISO_U0SBCR;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     SDMAM:1;
}ISO_U0SDMAM;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     SFE:1;
}ISO_U0SFE;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     SRT:2;
}ISO_U0SRT;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     STET:2;
}ISO_U0STET;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     HTX:1;
}ISO_U0HTX;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     DMASA:1;
}ISO_U0DMASA;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     FIFO_MODE:8;
unsigned int     reserved_1:2;
unsigned int     DMA_EXTRA:1;
unsigned int     UART_ADD_ENCODED_PARAMS:1;
unsigned int     SHADOW:1;
unsigned int     FIFO_STAT:1;
unsigned int     FIFO_ACCESS:1;
unsigned int     ADDITIONAL_FEAT:1;
unsigned int     SIR_LP_MODE:1;
unsigned int     SIR_MODE:1;
unsigned int     THRE_MODE:1;
unsigned int     AFCE_MODE:1;
unsigned int     reserved_2:2;
unsigned int     APB_DATA_WIDTH:2;
}ISO_U0CPR;

typedef struct 
{
unsigned int     UCV:32;
}ISO_U0UCV;

typedef struct 
{
unsigned int     CTR:32;
}ISO_U0CTR;

#endif

#define ISO_ISR                                                                      0x18007000
#define ISO_ISR_reg_addr                                                             "0xB8007000"
#define ISO_ISR_reg                                                                  0xB8007000
#define set_ISO_ISR_reg(data)   (*((volatile unsigned int*) ISO_ISR_reg)=data)
#define get_ISO_ISR_reg   (*((volatile unsigned int*) ISO_ISR_reg))
#define ISO_ISR_inst_adr                                                             "0x0000"
#define ISO_ISR_inst                                                                 0x0000
#define ISO_ISR_CECRxINT_shift                                                       (23)
#define ISO_ISR_CECRxINT_mask                                                        (0x00800000)
#define ISO_ISR_CECRxINT(data)                                                       (0x00800000&((data)<<23))
#define ISO_ISR_CECRxINT_src(data)                                                   ((0x00800000&(data))>>23)
#define ISO_ISR_get_CECRxINT(data)                                                   ((0x00800000&(data))>>23)
#define ISO_ISR_CECTxINT_shift                                                       (22)
#define ISO_ISR_CECTxINT_mask                                                        (0x00400000)
#define ISO_ISR_CECTxINT(data)                                                       (0x00400000&((data)<<22))
#define ISO_ISR_CECTxINT_src(data)                                                   ((0x00400000&(data))>>22)
#define ISO_ISR_get_CECTxINT(data)                                                   ((0x00400000&(data))>>22)
#define ISO_ISR_GPIODA_INT_shift                                                     (20)
#define ISO_ISR_GPIODA_INT_mask                                                      (0x00100000)
#define ISO_ISR_GPIODA_INT(data)                                                     (0x00100000&((data)<<20))
#define ISO_ISR_GPIODA_INT_src(data)                                                 ((0x00100000&(data))>>20)
#define ISO_ISR_get_GPIODA_INT(data)                                                 ((0x00100000&(data))>>20)
#define ISO_ISR_GPIOA_INT_shift                                                      (19)
#define ISO_ISR_GPIOA_INT_mask                                                       (0x00080000)
#define ISO_ISR_GPIOA_INT(data)                                                      (0x00080000&((data)<<19))
#define ISO_ISR_GPIOA_INT_src(data)                                                  ((0x00080000&(data))>>19)
#define ISO_ISR_get_GPIOA_INT(data)                                                  ((0x00080000&(data))>>19)
#define ISO_ISR_VFD_ARDSWDA_INT_shift                                                (18)
#define ISO_ISR_VFD_ARDSWDA_INT_mask                                                 (0x00040000)
#define ISO_ISR_VFD_ARDSWDA_INT(data)                                                (0x00040000&((data)<<18))
#define ISO_ISR_VFD_ARDSWDA_INT_src(data)                                            ((0x00040000&(data))>>18)
#define ISO_ISR_get_VFD_ARDSWDA_INT(data)                                            ((0x00040000&(data))>>18)
#define ISO_ISR_VFD_ARDSWA_INT_shift                                                 (17)
#define ISO_ISR_VFD_ARDSWA_INT_mask                                                  (0x00020000)
#define ISO_ISR_VFD_ARDSWA_INT(data)                                                 (0x00020000&((data)<<17))
#define ISO_ISR_VFD_ARDSWA_INT_src(data)                                             ((0x00020000&(data))>>17)
#define ISO_ISR_get_VFD_ARDSWA_INT(data)                                             ((0x00020000&(data))>>17)
#define ISO_ISR_VFD_ARDKPADDA_INT_shift                                              (16)
#define ISO_ISR_VFD_ARDKPADDA_INT_mask                                               (0x00010000)
#define ISO_ISR_VFD_ARDKPADDA_INT(data)                                              (0x00010000&((data)<<16))
#define ISO_ISR_VFD_ARDKPADDA_INT_src(data)                                          ((0x00010000&(data))>>16)
#define ISO_ISR_get_VFD_ARDKPADDA_INT(data)                                          ((0x00010000&(data))>>16)
#define ISO_ISR_VFD_ARDKPADA_INT_shift                                               (15)
#define ISO_ISR_VFD_ARDKPADA_INT_mask                                                (0x00008000)
#define ISO_ISR_VFD_ARDKPADA_INT(data)                                               (0x00008000&((data)<<15))
#define ISO_ISR_VFD_ARDKPADA_INT_src(data)                                           ((0x00008000&(data))>>15)
#define ISO_ISR_get_VFD_ARDKPADA_INT(data)                                           ((0x00008000&(data))>>15)
#define ISO_ISR_VFD_WDONE_INT_shift                                                  (14)
#define ISO_ISR_VFD_WDONE_INT_mask                                                   (0x00004000)
#define ISO_ISR_VFD_WDONE_INT(data)                                                  (0x00004000&((data)<<14))
#define ISO_ISR_VFD_WDONE_INT_src(data)                                              ((0x00004000&(data))>>14)
#define ISO_ISR_get_VFD_WDONE_INT(data)                                              ((0x00004000&(data))>>14)
#define ISO_ISR_RTC_ALARM_INT_shift                                                  (13)
#define ISO_ISR_RTC_ALARM_INT_mask                                                   (0x00002000)
#define ISO_ISR_RTC_ALARM_INT(data)                                                  (0x00002000&((data)<<13))
#define ISO_ISR_RTC_ALARM_INT_src(data)                                              ((0x00002000&(data))>>13)
#define ISO_ISR_get_RTC_ALARM_INT(data)                                              ((0x00002000&(data))>>13)
#define ISO_ISR_RTC_HSEC_INT_shift                                                   (12)
#define ISO_ISR_RTC_HSEC_INT_mask                                                    (0x00001000)
#define ISO_ISR_RTC_HSEC_INT(data)                                                   (0x00001000&((data)<<12))
#define ISO_ISR_RTC_HSEC_INT_src(data)                                               ((0x00001000&(data))>>12)
#define ISO_ISR_get_RTC_HSEC_INT(data)                                               ((0x00001000&(data))>>12)
#define ISO_ISR_IRDA_INT_shift                                                       (5)
#define ISO_ISR_IRDA_INT_mask                                                        (0x00000020)
#define ISO_ISR_IRDA_INT(data)                                                       (0x00000020&((data)<<5))
#define ISO_ISR_IRDA_INT_src(data)                                                   ((0x00000020&(data))>>5)
#define ISO_ISR_get_IRDA_INT(data)                                                   ((0x00000020&(data))>>5)
#define ISO_ISR_UR0_INT_shift                                                        (2)
#define ISO_ISR_UR0_INT_mask                                                         (0x00000004)
#define ISO_ISR_UR0_INT(data)                                                        (0x00000004&((data)<<2))
#define ISO_ISR_UR0_INT_src(data)                                                    ((0x00000004&(data))>>2)
#define ISO_ISR_get_UR0_INT(data)                                                    ((0x00000004&(data))>>2)
#define ISO_ISR_TC3_INT_shift                                                        (1)
#define ISO_ISR_TC3_INT_mask                                                         (0x00000002)
#define ISO_ISR_TC3_INT(data)                                                        (0x00000002&((data)<<1))
#define ISO_ISR_TC3_INT_src(data)                                                    ((0x00000002&(data))>>1)
#define ISO_ISR_get_TC3_INT(data)                                                    ((0x00000002&(data))>>1)
#define ISO_ISR_write_data_shift                                                     (0)
#define ISO_ISR_write_data_mask                                                      (0x00000001)
#define ISO_ISR_write_data(data)                                                     (0x00000001&((data)<<0))
#define ISO_ISR_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define ISO_ISR_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR                                                                 0x18007004
#define ISO_UMSK_ISR_reg_addr                                                        "0xB8007004"
#define ISO_UMSK_ISR_reg                                                             0xB8007004
#define set_ISO_UMSK_ISR_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_reg)=data)
#define get_ISO_UMSK_ISR_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_reg))
#define ISO_UMSK_ISR_inst_adr                                                        "0x0001"
#define ISO_UMSK_ISR_inst                                                            0x0001
#define ISO_UMSK_ISR_CECRxINT_shift                                                  (23)
#define ISO_UMSK_ISR_CECRxINT_mask                                                   (0x00800000)
#define ISO_UMSK_ISR_CECRxINT(data)                                                  (0x00800000&((data)<<23))
#define ISO_UMSK_ISR_CECRxINT_src(data)                                              ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_get_CECRxINT(data)                                              ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_CECTxINT_shift                                                  (22)
#define ISO_UMSK_ISR_CECTxINT_mask                                                   (0x00400000)
#define ISO_UMSK_ISR_CECTxINT(data)                                                  (0x00400000&((data)<<22))
#define ISO_UMSK_ISR_CECTxINT_src(data)                                              ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_get_CECTxINT(data)                                              ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_GPIODA_INT_shift                                                (20)
#define ISO_UMSK_ISR_GPIODA_INT_mask                                                 (0x00100000)
#define ISO_UMSK_ISR_GPIODA_INT(data)                                                (0x00100000&((data)<<20))
#define ISO_UMSK_ISR_GPIODA_INT_src(data)                                            ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_get_GPIODA_INT(data)                                            ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_GPIOA_INT_shift                                                 (19)
#define ISO_UMSK_ISR_GPIOA_INT_mask                                                  (0x00080000)
#define ISO_UMSK_ISR_GPIOA_INT(data)                                                 (0x00080000&((data)<<19))
#define ISO_UMSK_ISR_GPIOA_INT_src(data)                                             ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_get_GPIOA_INT(data)                                             ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_VFD_ARDSWDA_INT_shift                                           (18)
#define ISO_UMSK_ISR_VFD_ARDSWDA_INT_mask                                            (0x00040000)
#define ISO_UMSK_ISR_VFD_ARDSWDA_INT(data)                                           (0x00040000&((data)<<18))
#define ISO_UMSK_ISR_VFD_ARDSWDA_INT_src(data)                                       ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_get_VFD_ARDSWDA_INT(data)                                       ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_VFD_ARDSWA_INT_shift                                            (17)
#define ISO_UMSK_ISR_VFD_ARDSWA_INT_mask                                             (0x00020000)
#define ISO_UMSK_ISR_VFD_ARDSWA_INT(data)                                            (0x00020000&((data)<<17))
#define ISO_UMSK_ISR_VFD_ARDSWA_INT_src(data)                                        ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_get_VFD_ARDSWA_INT(data)                                        ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_VFD_ARDKPADDA_INT_shift                                         (16)
#define ISO_UMSK_ISR_VFD_ARDKPADDA_INT_mask                                          (0x00010000)
#define ISO_UMSK_ISR_VFD_ARDKPADDA_INT(data)                                         (0x00010000&((data)<<16))
#define ISO_UMSK_ISR_VFD_ARDKPADDA_INT_src(data)                                     ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_get_VFD_ARDKPADDA_INT(data)                                     ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_VFD_ARDKPADA_INT_shift                                          (15)
#define ISO_UMSK_ISR_VFD_ARDKPADA_INT_mask                                           (0x00008000)
#define ISO_UMSK_ISR_VFD_ARDKPADA_INT(data)                                          (0x00008000&((data)<<15))
#define ISO_UMSK_ISR_VFD_ARDKPADA_INT_src(data)                                      ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_get_VFD_ARDKPADA_INT(data)                                      ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_VFD_WDONE_INT_shift                                             (14)
#define ISO_UMSK_ISR_VFD_WDONE_INT_mask                                              (0x00004000)
#define ISO_UMSK_ISR_VFD_WDONE_INT(data)                                             (0x00004000&((data)<<14))
#define ISO_UMSK_ISR_VFD_WDONE_INT_src(data)                                         ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_get_VFD_WDONE_INT(data)                                         ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_RTC_ALARM_INT_shift                                             (13)
#define ISO_UMSK_ISR_RTC_ALARM_INT_mask                                              (0x00002000)
#define ISO_UMSK_ISR_RTC_ALARM_INT(data)                                             (0x00002000&((data)<<13))
#define ISO_UMSK_ISR_RTC_ALARM_INT_src(data)                                         ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_get_RTC_ALARM_INT(data)                                         ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_RTC_HSEC_INT_shift                                              (12)
#define ISO_UMSK_ISR_RTC_HSEC_INT_mask                                               (0x00001000)
#define ISO_UMSK_ISR_RTC_HSEC_INT(data)                                              (0x00001000&((data)<<12))
#define ISO_UMSK_ISR_RTC_HSEC_INT_src(data)                                          ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_get_RTC_HSEC_INT(data)                                          ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_IRDA_INT_shift                                                  (5)
#define ISO_UMSK_ISR_IRDA_INT_mask                                                   (0x00000020)
#define ISO_UMSK_ISR_IRDA_INT(data)                                                  (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_IRDA_INT_src(data)                                              ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_get_IRDA_INT(data)                                              ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_TC3_INT_shift                                                   (1)
#define ISO_UMSK_ISR_TC3_INT_mask                                                    (0x00000002)
#define ISO_UMSK_ISR_TC3_INT(data)                                                   (0x00000002&((data)<<1))
#define ISO_UMSK_ISR_TC3_INT_src(data)                                               ((0x00000002&(data))>>1)
#define ISO_UMSK_ISR_get_TC3_INT(data)                                               ((0x00000002&(data))>>1)
#define ISO_UMSK_ISR_write_data_shift                                                (0)
#define ISO_UMSK_ISR_write_data_mask                                                 (0x00000001)
#define ISO_UMSK_ISR_write_data(data)                                                (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_write_data_src(data)                                            ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_get_write_data(data)                                            ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR_GP                                                              0x18007008
#define ISO_UMSK_ISR_GP_reg_addr                                                     "0xB8007008"
#define ISO_UMSK_ISR_GP_reg                                                          0xB8007008
#define set_ISO_UMSK_ISR_GP_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_GP_reg)=data)
#define get_ISO_UMSK_ISR_GP_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_GP_reg))
#define ISO_UMSK_ISR_GP_inst_adr                                                     "0x0002"
#define ISO_UMSK_ISR_GP_inst                                                         0x0002
#define ISO_UMSK_ISR_GP_INT10_DA_shift                                               (26)
#define ISO_UMSK_ISR_GP_INT10_DA_mask                                                (0x04000000)
#define ISO_UMSK_ISR_GP_INT10_DA(data)                                               (0x04000000&((data)<<26))
#define ISO_UMSK_ISR_GP_INT10_DA_src(data)                                           ((0x04000000&(data))>>26)
#define ISO_UMSK_ISR_GP_get_INT10_DA(data)                                           ((0x04000000&(data))>>26)
#define ISO_UMSK_ISR_GP_INT9_DA_shift                                                (25)
#define ISO_UMSK_ISR_GP_INT9_DA_mask                                                 (0x02000000)
#define ISO_UMSK_ISR_GP_INT9_DA(data)                                                (0x02000000&((data)<<25))
#define ISO_UMSK_ISR_GP_INT9_DA_src(data)                                            ((0x02000000&(data))>>25)
#define ISO_UMSK_ISR_GP_get_INT9_DA(data)                                            ((0x02000000&(data))>>25)
#define ISO_UMSK_ISR_GP_INT8_DA_shift                                                (24)
#define ISO_UMSK_ISR_GP_INT8_DA_mask                                                 (0x01000000)
#define ISO_UMSK_ISR_GP_INT8_DA(data)                                                (0x01000000&((data)<<24))
#define ISO_UMSK_ISR_GP_INT8_DA_src(data)                                            ((0x01000000&(data))>>24)
#define ISO_UMSK_ISR_GP_get_INT8_DA(data)                                            ((0x01000000&(data))>>24)
#define ISO_UMSK_ISR_GP_INT7_DA_shift                                                (23)
#define ISO_UMSK_ISR_GP_INT7_DA_mask                                                 (0x00800000)
#define ISO_UMSK_ISR_GP_INT7_DA(data)                                                (0x00800000&((data)<<23))
#define ISO_UMSK_ISR_GP_INT7_DA_src(data)                                            ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_GP_get_INT7_DA(data)                                            ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_GP_INT6_DA_shift                                                (22)
#define ISO_UMSK_ISR_GP_INT6_DA_mask                                                 (0x00400000)
#define ISO_UMSK_ISR_GP_INT6_DA(data)                                                (0x00400000&((data)<<22))
#define ISO_UMSK_ISR_GP_INT6_DA_src(data)                                            ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_GP_get_INT6_DA(data)                                            ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_GP_INT5_DA_shift                                                (21)
#define ISO_UMSK_ISR_GP_INT5_DA_mask                                                 (0x00200000)
#define ISO_UMSK_ISR_GP_INT5_DA(data)                                                (0x00200000&((data)<<21))
#define ISO_UMSK_ISR_GP_INT5_DA_src(data)                                            ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_GP_get_INT5_DA(data)                                            ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_GP_INT4_DA_shift                                                (20)
#define ISO_UMSK_ISR_GP_INT4_DA_mask                                                 (0x00100000)
#define ISO_UMSK_ISR_GP_INT4_DA(data)                                                (0x00100000&((data)<<20))
#define ISO_UMSK_ISR_GP_INT4_DA_src(data)                                            ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_GP_get_INT4_DA(data)                                            ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_GP_INT3_DA_shift                                                (19)
#define ISO_UMSK_ISR_GP_INT3_DA_mask                                                 (0x00080000)
#define ISO_UMSK_ISR_GP_INT3_DA(data)                                                (0x00080000&((data)<<19))
#define ISO_UMSK_ISR_GP_INT3_DA_src(data)                                            ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_GP_get_INT3_DA(data)                                            ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_GP_INT2_DA_shift                                                (18)
#define ISO_UMSK_ISR_GP_INT2_DA_mask                                                 (0x00040000)
#define ISO_UMSK_ISR_GP_INT2_DA(data)                                                (0x00040000&((data)<<18))
#define ISO_UMSK_ISR_GP_INT2_DA_src(data)                                            ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_GP_get_INT2_DA(data)                                            ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_GP_INT1_DA_shift                                                (17)
#define ISO_UMSK_ISR_GP_INT1_DA_mask                                                 (0x00020000)
#define ISO_UMSK_ISR_GP_INT1_DA(data)                                                (0x00020000&((data)<<17))
#define ISO_UMSK_ISR_GP_INT1_DA_src(data)                                            ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_GP_get_INT1_DA(data)                                            ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_GP_INT0_DA_shift                                                (16)
#define ISO_UMSK_ISR_GP_INT0_DA_mask                                                 (0x00010000)
#define ISO_UMSK_ISR_GP_INT0_DA(data)                                                (0x00010000&((data)<<16))
#define ISO_UMSK_ISR_GP_INT0_DA_src(data)                                            ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_GP_get_INT0_DA(data)                                            ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_GP_INT10_A_shift                                                (11)
#define ISO_UMSK_ISR_GP_INT10_A_mask                                                 (0x00000800)
#define ISO_UMSK_ISR_GP_INT10_A(data)                                                (0x00000800&((data)<<11))
#define ISO_UMSK_ISR_GP_INT10_A_src(data)                                            ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_GP_get_INT10_A(data)                                            ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_GP_INT9_A_shift                                                 (10)
#define ISO_UMSK_ISR_GP_INT9_A_mask                                                  (0x00000400)
#define ISO_UMSK_ISR_GP_INT9_A(data)                                                 (0x00000400&((data)<<10))
#define ISO_UMSK_ISR_GP_INT9_A_src(data)                                             ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_GP_get_INT9_A(data)                                             ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_GP_INT8_A_shift                                                 (9)
#define ISO_UMSK_ISR_GP_INT8_A_mask                                                  (0x00000200)
#define ISO_UMSK_ISR_GP_INT8_A(data)                                                 (0x00000200&((data)<<9))
#define ISO_UMSK_ISR_GP_INT8_A_src(data)                                             ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_GP_get_INT8_A(data)                                             ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_GP_INT7_A_shift                                                 (8)
#define ISO_UMSK_ISR_GP_INT7_A_mask                                                  (0x00000100)
#define ISO_UMSK_ISR_GP_INT7_A(data)                                                 (0x00000100&((data)<<8))
#define ISO_UMSK_ISR_GP_INT7_A_src(data)                                             ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_GP_get_INT7_A(data)                                             ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_GP_INT6_A_shift                                                 (7)
#define ISO_UMSK_ISR_GP_INT6_A_mask                                                  (0x00000080)
#define ISO_UMSK_ISR_GP_INT6_A(data)                                                 (0x00000080&((data)<<7))
#define ISO_UMSK_ISR_GP_INT6_A_src(data)                                             ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_GP_get_INT6_A(data)                                             ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_GP_INT5_A_shift                                                 (6)
#define ISO_UMSK_ISR_GP_INT5_A_mask                                                  (0x00000040)
#define ISO_UMSK_ISR_GP_INT5_A(data)                                                 (0x00000040&((data)<<6))
#define ISO_UMSK_ISR_GP_INT5_A_src(data)                                             ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_GP_get_INT5_A(data)                                             ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_GP_INT4_A_shift                                                 (5)
#define ISO_UMSK_ISR_GP_INT4_A_mask                                                  (0x00000020)
#define ISO_UMSK_ISR_GP_INT4_A(data)                                                 (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_GP_INT4_A_src(data)                                             ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_GP_get_INT4_A(data)                                             ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_GP_INT3_A_shift                                                 (4)
#define ISO_UMSK_ISR_GP_INT3_A_mask                                                  (0x00000010)
#define ISO_UMSK_ISR_GP_INT3_A(data)                                                 (0x00000010&((data)<<4))
#define ISO_UMSK_ISR_GP_INT3_A_src(data)                                             ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_GP_get_INT3_A(data)                                             ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_GP_INT2_A_shift                                                 (3)
#define ISO_UMSK_ISR_GP_INT2_A_mask                                                  (0x00000008)
#define ISO_UMSK_ISR_GP_INT2_A(data)                                                 (0x00000008&((data)<<3))
#define ISO_UMSK_ISR_GP_INT2_A_src(data)                                             ((0x00000008&(data))>>3)
#define ISO_UMSK_ISR_GP_get_INT2_A(data)                                             ((0x00000008&(data))>>3)
#define ISO_UMSK_ISR_GP_INT1_A_shift                                                 (2)
#define ISO_UMSK_ISR_GP_INT1_A_mask                                                  (0x00000004)
#define ISO_UMSK_ISR_GP_INT1_A(data)                                                 (0x00000004&((data)<<2))
#define ISO_UMSK_ISR_GP_INT1_A_src(data)                                             ((0x00000004&(data))>>2)
#define ISO_UMSK_ISR_GP_get_INT1_A(data)                                             ((0x00000004&(data))>>2)
#define ISO_UMSK_ISR_GP_INT0_A_shift                                                 (1)
#define ISO_UMSK_ISR_GP_INT0_A_mask                                                  (0x00000002)
#define ISO_UMSK_ISR_GP_INT0_A(data)                                                 (0x00000002&((data)<<1))
#define ISO_UMSK_ISR_GP_INT0_A_src(data)                                             ((0x00000002&(data))>>1)
#define ISO_UMSK_ISR_GP_get_INT0_A(data)                                             ((0x00000002&(data))>>1)
#define ISO_UMSK_ISR_GP_write_data_shift                                             (0)
#define ISO_UMSK_ISR_GP_write_data_mask                                              (0x00000001)
#define ISO_UMSK_ISR_GP_write_data(data)                                             (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_GP_write_data_src(data)                                         ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_GP_get_write_data(data)                                         ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR_KPADAH                                                          0x1800700C
#define ISO_UMSK_ISR_KPADAH_reg_addr                                                 "0xB800700C"
#define ISO_UMSK_ISR_KPADAH_reg                                                      0xB800700C
#define set_ISO_UMSK_ISR_KPADAH_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADAH_reg)=data)
#define get_ISO_UMSK_ISR_KPADAH_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADAH_reg))
#define ISO_UMSK_ISR_KPADAH_inst_adr                                                 "0x0003"
#define ISO_UMSK_ISR_KPADAH_inst                                                     0x0003
#define ISO_UMSK_ISR_KPADAH_INT47_shift                                              (23)
#define ISO_UMSK_ISR_KPADAH_INT47_mask                                               (0x00800000)
#define ISO_UMSK_ISR_KPADAH_INT47(data)                                              (0x00800000&((data)<<23))
#define ISO_UMSK_ISR_KPADAH_INT47_src(data)                                          ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADAH_get_INT47(data)                                          ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADAH_INT46_shift                                              (22)
#define ISO_UMSK_ISR_KPADAH_INT46_mask                                               (0x00400000)
#define ISO_UMSK_ISR_KPADAH_INT46(data)                                              (0x00400000&((data)<<22))
#define ISO_UMSK_ISR_KPADAH_INT46_src(data)                                          ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADAH_get_INT46(data)                                          ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADAH_INT45_shift                                              (21)
#define ISO_UMSK_ISR_KPADAH_INT45_mask                                               (0x00200000)
#define ISO_UMSK_ISR_KPADAH_INT45(data)                                              (0x00200000&((data)<<21))
#define ISO_UMSK_ISR_KPADAH_INT45_src(data)                                          ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADAH_get_INT45(data)                                          ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADAH_INT44_shift                                              (20)
#define ISO_UMSK_ISR_KPADAH_INT44_mask                                               (0x00100000)
#define ISO_UMSK_ISR_KPADAH_INT44(data)                                              (0x00100000&((data)<<20))
#define ISO_UMSK_ISR_KPADAH_INT44_src(data)                                          ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADAH_get_INT44(data)                                          ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADAH_INT43_shift                                              (19)
#define ISO_UMSK_ISR_KPADAH_INT43_mask                                               (0x00080000)
#define ISO_UMSK_ISR_KPADAH_INT43(data)                                              (0x00080000&((data)<<19))
#define ISO_UMSK_ISR_KPADAH_INT43_src(data)                                          ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADAH_get_INT43(data)                                          ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADAH_INT42_shift                                              (18)
#define ISO_UMSK_ISR_KPADAH_INT42_mask                                               (0x00040000)
#define ISO_UMSK_ISR_KPADAH_INT42(data)                                              (0x00040000&((data)<<18))
#define ISO_UMSK_ISR_KPADAH_INT42_src(data)                                          ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADAH_get_INT42(data)                                          ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADAH_INT41_shift                                              (17)
#define ISO_UMSK_ISR_KPADAH_INT41_mask                                               (0x00020000)
#define ISO_UMSK_ISR_KPADAH_INT41(data)                                              (0x00020000&((data)<<17))
#define ISO_UMSK_ISR_KPADAH_INT41_src(data)                                          ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADAH_get_INT41(data)                                          ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADAH_INT40_shift                                              (16)
#define ISO_UMSK_ISR_KPADAH_INT40_mask                                               (0x00010000)
#define ISO_UMSK_ISR_KPADAH_INT40(data)                                              (0x00010000&((data)<<16))
#define ISO_UMSK_ISR_KPADAH_INT40_src(data)                                          ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADAH_get_INT40(data)                                          ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADAH_INT39_shift                                              (15)
#define ISO_UMSK_ISR_KPADAH_INT39_mask                                               (0x00008000)
#define ISO_UMSK_ISR_KPADAH_INT39(data)                                              (0x00008000&((data)<<15))
#define ISO_UMSK_ISR_KPADAH_INT39_src(data)                                          ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADAH_get_INT39(data)                                          ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADAH_INT38_shift                                              (14)
#define ISO_UMSK_ISR_KPADAH_INT38_mask                                               (0x00004000)
#define ISO_UMSK_ISR_KPADAH_INT38(data)                                              (0x00004000&((data)<<14))
#define ISO_UMSK_ISR_KPADAH_INT38_src(data)                                          ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADAH_get_INT38(data)                                          ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADAH_INT37_shift                                              (13)
#define ISO_UMSK_ISR_KPADAH_INT37_mask                                               (0x00002000)
#define ISO_UMSK_ISR_KPADAH_INT37(data)                                              (0x00002000&((data)<<13))
#define ISO_UMSK_ISR_KPADAH_INT37_src(data)                                          ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADAH_get_INT37(data)                                          ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADAH_INT36_shift                                              (12)
#define ISO_UMSK_ISR_KPADAH_INT36_mask                                               (0x00001000)
#define ISO_UMSK_ISR_KPADAH_INT36(data)                                              (0x00001000&((data)<<12))
#define ISO_UMSK_ISR_KPADAH_INT36_src(data)                                          ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADAH_get_INT36(data)                                          ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADAH_INT35_shift                                              (11)
#define ISO_UMSK_ISR_KPADAH_INT35_mask                                               (0x00000800)
#define ISO_UMSK_ISR_KPADAH_INT35(data)                                              (0x00000800&((data)<<11))
#define ISO_UMSK_ISR_KPADAH_INT35_src(data)                                          ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADAH_get_INT35(data)                                          ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADAH_INT34_shift                                              (10)
#define ISO_UMSK_ISR_KPADAH_INT34_mask                                               (0x00000400)
#define ISO_UMSK_ISR_KPADAH_INT34(data)                                              (0x00000400&((data)<<10))
#define ISO_UMSK_ISR_KPADAH_INT34_src(data)                                          ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADAH_get_INT34(data)                                          ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADAH_INT33_shift                                              (9)
#define ISO_UMSK_ISR_KPADAH_INT33_mask                                               (0x00000200)
#define ISO_UMSK_ISR_KPADAH_INT33(data)                                              (0x00000200&((data)<<9))
#define ISO_UMSK_ISR_KPADAH_INT33_src(data)                                          ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADAH_get_INT33(data)                                          ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADAH_INT32_shift                                              (8)
#define ISO_UMSK_ISR_KPADAH_INT32_mask                                               (0x00000100)
#define ISO_UMSK_ISR_KPADAH_INT32(data)                                              (0x00000100&((data)<<8))
#define ISO_UMSK_ISR_KPADAH_INT32_src(data)                                          ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADAH_get_INT32(data)                                          ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADAH_INT31_shift                                              (7)
#define ISO_UMSK_ISR_KPADAH_INT31_mask                                               (0x00000080)
#define ISO_UMSK_ISR_KPADAH_INT31(data)                                              (0x00000080&((data)<<7))
#define ISO_UMSK_ISR_KPADAH_INT31_src(data)                                          ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADAH_get_INT31(data)                                          ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADAH_INT30_shift                                              (6)
#define ISO_UMSK_ISR_KPADAH_INT30_mask                                               (0x00000040)
#define ISO_UMSK_ISR_KPADAH_INT30(data)                                              (0x00000040&((data)<<6))
#define ISO_UMSK_ISR_KPADAH_INT30_src(data)                                          ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADAH_get_INT30(data)                                          ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADAH_INT29_shift                                              (5)
#define ISO_UMSK_ISR_KPADAH_INT29_mask                                               (0x00000020)
#define ISO_UMSK_ISR_KPADAH_INT29(data)                                              (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_KPADAH_INT29_src(data)                                          ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADAH_get_INT29(data)                                          ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADAH_INT28_shift                                              (4)
#define ISO_UMSK_ISR_KPADAH_INT28_mask                                               (0x00000010)
#define ISO_UMSK_ISR_KPADAH_INT28(data)                                              (0x00000010&((data)<<4))
#define ISO_UMSK_ISR_KPADAH_INT28_src(data)                                          ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADAH_get_INT28(data)                                          ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADAH_write_data_shift                                         (0)
#define ISO_UMSK_ISR_KPADAH_write_data_mask                                          (0x00000001)
#define ISO_UMSK_ISR_KPADAH_write_data(data)                                         (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_KPADAH_write_data_src(data)                                     ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_KPADAH_get_write_data(data)                                     ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR_KPADAL                                                          0x18007010
#define ISO_UMSK_ISR_KPADAL_reg_addr                                                 "0xB8007010"
#define ISO_UMSK_ISR_KPADAL_reg                                                      0xB8007010
#define set_ISO_UMSK_ISR_KPADAL_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADAL_reg)=data)
#define get_ISO_UMSK_ISR_KPADAL_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADAL_reg))
#define ISO_UMSK_ISR_KPADAL_inst_adr                                                 "0x0004"
#define ISO_UMSK_ISR_KPADAL_inst                                                     0x0004
#define ISO_UMSK_ISR_KPADAL_INT27_shift                                              (31)
#define ISO_UMSK_ISR_KPADAL_INT27_mask                                               (0x80000000)
#define ISO_UMSK_ISR_KPADAL_INT27(data)                                              (0x80000000&((data)<<31))
#define ISO_UMSK_ISR_KPADAL_INT27_src(data)                                          ((0x80000000&(data))>>31)
#define ISO_UMSK_ISR_KPADAL_get_INT27(data)                                          ((0x80000000&(data))>>31)
#define ISO_UMSK_ISR_KPADAL_INT26_shift                                              (30)
#define ISO_UMSK_ISR_KPADAL_INT26_mask                                               (0x40000000)
#define ISO_UMSK_ISR_KPADAL_INT26(data)                                              (0x40000000&((data)<<30))
#define ISO_UMSK_ISR_KPADAL_INT26_src(data)                                          ((0x40000000&(data))>>30)
#define ISO_UMSK_ISR_KPADAL_get_INT26(data)                                          ((0x40000000&(data))>>30)
#define ISO_UMSK_ISR_KPADAL_INT25_shift                                              (29)
#define ISO_UMSK_ISR_KPADAL_INT25_mask                                               (0x20000000)
#define ISO_UMSK_ISR_KPADAL_INT25(data)                                              (0x20000000&((data)<<29))
#define ISO_UMSK_ISR_KPADAL_INT25_src(data)                                          ((0x20000000&(data))>>29)
#define ISO_UMSK_ISR_KPADAL_get_INT25(data)                                          ((0x20000000&(data))>>29)
#define ISO_UMSK_ISR_KPADAL_INT24_shift                                              (28)
#define ISO_UMSK_ISR_KPADAL_INT24_mask                                               (0x10000000)
#define ISO_UMSK_ISR_KPADAL_INT24(data)                                              (0x10000000&((data)<<28))
#define ISO_UMSK_ISR_KPADAL_INT24_src(data)                                          ((0x10000000&(data))>>28)
#define ISO_UMSK_ISR_KPADAL_get_INT24(data)                                          ((0x10000000&(data))>>28)
#define ISO_UMSK_ISR_KPADAL_INT23_shift                                              (27)
#define ISO_UMSK_ISR_KPADAL_INT23_mask                                               (0x08000000)
#define ISO_UMSK_ISR_KPADAL_INT23(data)                                              (0x08000000&((data)<<27))
#define ISO_UMSK_ISR_KPADAL_INT23_src(data)                                          ((0x08000000&(data))>>27)
#define ISO_UMSK_ISR_KPADAL_get_INT23(data)                                          ((0x08000000&(data))>>27)
#define ISO_UMSK_ISR_KPADAL_INT22_shift                                              (26)
#define ISO_UMSK_ISR_KPADAL_INT22_mask                                               (0x04000000)
#define ISO_UMSK_ISR_KPADAL_INT22(data)                                              (0x04000000&((data)<<26))
#define ISO_UMSK_ISR_KPADAL_INT22_src(data)                                          ((0x04000000&(data))>>26)
#define ISO_UMSK_ISR_KPADAL_get_INT22(data)                                          ((0x04000000&(data))>>26)
#define ISO_UMSK_ISR_KPADAL_INT21_shift                                              (25)
#define ISO_UMSK_ISR_KPADAL_INT21_mask                                               (0x02000000)
#define ISO_UMSK_ISR_KPADAL_INT21(data)                                              (0x02000000&((data)<<25))
#define ISO_UMSK_ISR_KPADAL_INT21_src(data)                                          ((0x02000000&(data))>>25)
#define ISO_UMSK_ISR_KPADAL_get_INT21(data)                                          ((0x02000000&(data))>>25)
#define ISO_UMSK_ISR_KPADAL_INT20_shift                                              (24)
#define ISO_UMSK_ISR_KPADAL_INT20_mask                                               (0x01000000)
#define ISO_UMSK_ISR_KPADAL_INT20(data)                                              (0x01000000&((data)<<24))
#define ISO_UMSK_ISR_KPADAL_INT20_src(data)                                          ((0x01000000&(data))>>24)
#define ISO_UMSK_ISR_KPADAL_get_INT20(data)                                          ((0x01000000&(data))>>24)
#define ISO_UMSK_ISR_KPADAL_INT19_shift                                              (23)
#define ISO_UMSK_ISR_KPADAL_INT19_mask                                               (0x00800000)
#define ISO_UMSK_ISR_KPADAL_INT19(data)                                              (0x00800000&((data)<<23))
#define ISO_UMSK_ISR_KPADAL_INT19_src(data)                                          ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADAL_get_INT19(data)                                          ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADAL_INT18_shift                                              (22)
#define ISO_UMSK_ISR_KPADAL_INT18_mask                                               (0x00400000)
#define ISO_UMSK_ISR_KPADAL_INT18(data)                                              (0x00400000&((data)<<22))
#define ISO_UMSK_ISR_KPADAL_INT18_src(data)                                          ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADAL_get_INT18(data)                                          ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADAL_INT17_shift                                              (21)
#define ISO_UMSK_ISR_KPADAL_INT17_mask                                               (0x00200000)
#define ISO_UMSK_ISR_KPADAL_INT17(data)                                              (0x00200000&((data)<<21))
#define ISO_UMSK_ISR_KPADAL_INT17_src(data)                                          ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADAL_get_INT17(data)                                          ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADAL_INT16_shift                                              (20)
#define ISO_UMSK_ISR_KPADAL_INT16_mask                                               (0x00100000)
#define ISO_UMSK_ISR_KPADAL_INT16(data)                                              (0x00100000&((data)<<20))
#define ISO_UMSK_ISR_KPADAL_INT16_src(data)                                          ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADAL_get_INT16(data)                                          ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADAL_INT15_shift                                              (19)
#define ISO_UMSK_ISR_KPADAL_INT15_mask                                               (0x00080000)
#define ISO_UMSK_ISR_KPADAL_INT15(data)                                              (0x00080000&((data)<<19))
#define ISO_UMSK_ISR_KPADAL_INT15_src(data)                                          ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADAL_get_INT15(data)                                          ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADAL_INT14_shift                                              (18)
#define ISO_UMSK_ISR_KPADAL_INT14_mask                                               (0x00040000)
#define ISO_UMSK_ISR_KPADAL_INT14(data)                                              (0x00040000&((data)<<18))
#define ISO_UMSK_ISR_KPADAL_INT14_src(data)                                          ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADAL_get_INT14(data)                                          ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADAL_INT13_shift                                              (17)
#define ISO_UMSK_ISR_KPADAL_INT13_mask                                               (0x00020000)
#define ISO_UMSK_ISR_KPADAL_INT13(data)                                              (0x00020000&((data)<<17))
#define ISO_UMSK_ISR_KPADAL_INT13_src(data)                                          ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADAL_get_INT13(data)                                          ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADAL_INT12_shift                                              (16)
#define ISO_UMSK_ISR_KPADAL_INT12_mask                                               (0x00010000)
#define ISO_UMSK_ISR_KPADAL_INT12(data)                                              (0x00010000&((data)<<16))
#define ISO_UMSK_ISR_KPADAL_INT12_src(data)                                          ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADAL_get_INT12(data)                                          ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADAL_INT11_shift                                              (15)
#define ISO_UMSK_ISR_KPADAL_INT11_mask                                               (0x00008000)
#define ISO_UMSK_ISR_KPADAL_INT11(data)                                              (0x00008000&((data)<<15))
#define ISO_UMSK_ISR_KPADAL_INT11_src(data)                                          ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADAL_get_INT11(data)                                          ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADAL_INT10_shift                                              (14)
#define ISO_UMSK_ISR_KPADAL_INT10_mask                                               (0x00004000)
#define ISO_UMSK_ISR_KPADAL_INT10(data)                                              (0x00004000&((data)<<14))
#define ISO_UMSK_ISR_KPADAL_INT10_src(data)                                          ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADAL_get_INT10(data)                                          ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADAL_INT9_shift                                               (13)
#define ISO_UMSK_ISR_KPADAL_INT9_mask                                                (0x00002000)
#define ISO_UMSK_ISR_KPADAL_INT9(data)                                               (0x00002000&((data)<<13))
#define ISO_UMSK_ISR_KPADAL_INT9_src(data)                                           ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADAL_get_INT9(data)                                           ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADAL_INT8_shift                                               (12)
#define ISO_UMSK_ISR_KPADAL_INT8_mask                                                (0x00001000)
#define ISO_UMSK_ISR_KPADAL_INT8(data)                                               (0x00001000&((data)<<12))
#define ISO_UMSK_ISR_KPADAL_INT8_src(data)                                           ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADAL_get_INT8(data)                                           ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADAL_INT7_shift                                               (11)
#define ISO_UMSK_ISR_KPADAL_INT7_mask                                                (0x00000800)
#define ISO_UMSK_ISR_KPADAL_INT7(data)                                               (0x00000800&((data)<<11))
#define ISO_UMSK_ISR_KPADAL_INT7_src(data)                                           ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADAL_get_INT7(data)                                           ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADAL_INT6_shift                                               (10)
#define ISO_UMSK_ISR_KPADAL_INT6_mask                                                (0x00000400)
#define ISO_UMSK_ISR_KPADAL_INT6(data)                                               (0x00000400&((data)<<10))
#define ISO_UMSK_ISR_KPADAL_INT6_src(data)                                           ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADAL_get_INT6(data)                                           ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADAL_INT5_shift                                               (9)
#define ISO_UMSK_ISR_KPADAL_INT5_mask                                                (0x00000200)
#define ISO_UMSK_ISR_KPADAL_INT5(data)                                               (0x00000200&((data)<<9))
#define ISO_UMSK_ISR_KPADAL_INT5_src(data)                                           ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADAL_get_INT5(data)                                           ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADAL_INT4_shift                                               (8)
#define ISO_UMSK_ISR_KPADAL_INT4_mask                                                (0x00000100)
#define ISO_UMSK_ISR_KPADAL_INT4(data)                                               (0x00000100&((data)<<8))
#define ISO_UMSK_ISR_KPADAL_INT4_src(data)                                           ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADAL_get_INT4(data)                                           ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADAL_INT3_shift                                               (7)
#define ISO_UMSK_ISR_KPADAL_INT3_mask                                                (0x00000080)
#define ISO_UMSK_ISR_KPADAL_INT3(data)                                               (0x00000080&((data)<<7))
#define ISO_UMSK_ISR_KPADAL_INT3_src(data)                                           ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADAL_get_INT3(data)                                           ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADAL_INT2_shift                                               (6)
#define ISO_UMSK_ISR_KPADAL_INT2_mask                                                (0x00000040)
#define ISO_UMSK_ISR_KPADAL_INT2(data)                                               (0x00000040&((data)<<6))
#define ISO_UMSK_ISR_KPADAL_INT2_src(data)                                           ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADAL_get_INT2(data)                                           ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADAL_INT1_shift                                               (5)
#define ISO_UMSK_ISR_KPADAL_INT1_mask                                                (0x00000020)
#define ISO_UMSK_ISR_KPADAL_INT1(data)                                               (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_KPADAL_INT1_src(data)                                           ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADAL_get_INT1(data)                                           ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADAL_INT0_shift                                               (4)
#define ISO_UMSK_ISR_KPADAL_INT0_mask                                                (0x00000010)
#define ISO_UMSK_ISR_KPADAL_INT0(data)                                               (0x00000010&((data)<<4))
#define ISO_UMSK_ISR_KPADAL_INT0_src(data)                                           ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADAL_get_INT0(data)                                           ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADAL_write_data_shift                                         (0)
#define ISO_UMSK_ISR_KPADAL_write_data_mask                                          (0x00000001)
#define ISO_UMSK_ISR_KPADAL_write_data(data)                                         (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_KPADAL_write_data_src(data)                                     ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_KPADAL_get_write_data(data)                                     ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR_KPADDAH                                                         0x18007014
#define ISO_UMSK_ISR_KPADDAH_reg_addr                                                "0xB8007014"
#define ISO_UMSK_ISR_KPADDAH_reg                                                     0xB8007014
#define set_ISO_UMSK_ISR_KPADDAH_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADDAH_reg)=data)
#define get_ISO_UMSK_ISR_KPADDAH_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADDAH_reg))
#define ISO_UMSK_ISR_KPADDAH_inst_adr                                                "0x0005"
#define ISO_UMSK_ISR_KPADDAH_inst                                                    0x0005
#define ISO_UMSK_ISR_KPADDAH_INT47_shift                                             (23)
#define ISO_UMSK_ISR_KPADDAH_INT47_mask                                              (0x00800000)
#define ISO_UMSK_ISR_KPADDAH_INT47(data)                                             (0x00800000&((data)<<23))
#define ISO_UMSK_ISR_KPADDAH_INT47_src(data)                                         ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADDAH_get_INT47(data)                                         ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADDAH_INT46_shift                                             (22)
#define ISO_UMSK_ISR_KPADDAH_INT46_mask                                              (0x00400000)
#define ISO_UMSK_ISR_KPADDAH_INT46(data)                                             (0x00400000&((data)<<22))
#define ISO_UMSK_ISR_KPADDAH_INT46_src(data)                                         ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADDAH_get_INT46(data)                                         ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADDAH_INT45_shift                                             (21)
#define ISO_UMSK_ISR_KPADDAH_INT45_mask                                              (0x00200000)
#define ISO_UMSK_ISR_KPADDAH_INT45(data)                                             (0x00200000&((data)<<21))
#define ISO_UMSK_ISR_KPADDAH_INT45_src(data)                                         ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADDAH_get_INT45(data)                                         ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADDAH_INT44_shift                                             (20)
#define ISO_UMSK_ISR_KPADDAH_INT44_mask                                              (0x00100000)
#define ISO_UMSK_ISR_KPADDAH_INT44(data)                                             (0x00100000&((data)<<20))
#define ISO_UMSK_ISR_KPADDAH_INT44_src(data)                                         ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADDAH_get_INT44(data)                                         ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADDAH_INT43_shift                                             (19)
#define ISO_UMSK_ISR_KPADDAH_INT43_mask                                              (0x00080000)
#define ISO_UMSK_ISR_KPADDAH_INT43(data)                                             (0x00080000&((data)<<19))
#define ISO_UMSK_ISR_KPADDAH_INT43_src(data)                                         ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADDAH_get_INT43(data)                                         ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADDAH_INT42_shift                                             (18)
#define ISO_UMSK_ISR_KPADDAH_INT42_mask                                              (0x00040000)
#define ISO_UMSK_ISR_KPADDAH_INT42(data)                                             (0x00040000&((data)<<18))
#define ISO_UMSK_ISR_KPADDAH_INT42_src(data)                                         ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADDAH_get_INT42(data)                                         ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADDAH_INT41_shift                                             (17)
#define ISO_UMSK_ISR_KPADDAH_INT41_mask                                              (0x00020000)
#define ISO_UMSK_ISR_KPADDAH_INT41(data)                                             (0x00020000&((data)<<17))
#define ISO_UMSK_ISR_KPADDAH_INT41_src(data)                                         ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADDAH_get_INT41(data)                                         ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADDAH_INT40_shift                                             (16)
#define ISO_UMSK_ISR_KPADDAH_INT40_mask                                              (0x00010000)
#define ISO_UMSK_ISR_KPADDAH_INT40(data)                                             (0x00010000&((data)<<16))
#define ISO_UMSK_ISR_KPADDAH_INT40_src(data)                                         ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADDAH_get_INT40(data)                                         ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADDAH_INT39_shift                                             (15)
#define ISO_UMSK_ISR_KPADDAH_INT39_mask                                              (0x00008000)
#define ISO_UMSK_ISR_KPADDAH_INT39(data)                                             (0x00008000&((data)<<15))
#define ISO_UMSK_ISR_KPADDAH_INT39_src(data)                                         ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADDAH_get_INT39(data)                                         ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADDAH_INT38_shift                                             (14)
#define ISO_UMSK_ISR_KPADDAH_INT38_mask                                              (0x00004000)
#define ISO_UMSK_ISR_KPADDAH_INT38(data)                                             (0x00004000&((data)<<14))
#define ISO_UMSK_ISR_KPADDAH_INT38_src(data)                                         ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADDAH_get_INT38(data)                                         ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADDAH_INT37_shift                                             (13)
#define ISO_UMSK_ISR_KPADDAH_INT37_mask                                              (0x00002000)
#define ISO_UMSK_ISR_KPADDAH_INT37(data)                                             (0x00002000&((data)<<13))
#define ISO_UMSK_ISR_KPADDAH_INT37_src(data)                                         ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADDAH_get_INT37(data)                                         ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADDAH_INT36_shift                                             (12)
#define ISO_UMSK_ISR_KPADDAH_INT36_mask                                              (0x00001000)
#define ISO_UMSK_ISR_KPADDAH_INT36(data)                                             (0x00001000&((data)<<12))
#define ISO_UMSK_ISR_KPADDAH_INT36_src(data)                                         ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADDAH_get_INT36(data)                                         ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADDAH_INT35_shift                                             (11)
#define ISO_UMSK_ISR_KPADDAH_INT35_mask                                              (0x00000800)
#define ISO_UMSK_ISR_KPADDAH_INT35(data)                                             (0x00000800&((data)<<11))
#define ISO_UMSK_ISR_KPADDAH_INT35_src(data)                                         ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADDAH_get_INT35(data)                                         ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADDAH_INT34_shift                                             (10)
#define ISO_UMSK_ISR_KPADDAH_INT34_mask                                              (0x00000400)
#define ISO_UMSK_ISR_KPADDAH_INT34(data)                                             (0x00000400&((data)<<10))
#define ISO_UMSK_ISR_KPADDAH_INT34_src(data)                                         ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADDAH_get_INT34(data)                                         ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADDAH_INT33_shift                                             (9)
#define ISO_UMSK_ISR_KPADDAH_INT33_mask                                              (0x00000200)
#define ISO_UMSK_ISR_KPADDAH_INT33(data)                                             (0x00000200&((data)<<9))
#define ISO_UMSK_ISR_KPADDAH_INT33_src(data)                                         ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADDAH_get_INT33(data)                                         ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADDAH_INT32_shift                                             (8)
#define ISO_UMSK_ISR_KPADDAH_INT32_mask                                              (0x00000100)
#define ISO_UMSK_ISR_KPADDAH_INT32(data)                                             (0x00000100&((data)<<8))
#define ISO_UMSK_ISR_KPADDAH_INT32_src(data)                                         ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADDAH_get_INT32(data)                                         ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADDAH_INT31_shift                                             (7)
#define ISO_UMSK_ISR_KPADDAH_INT31_mask                                              (0x00000080)
#define ISO_UMSK_ISR_KPADDAH_INT31(data)                                             (0x00000080&((data)<<7))
#define ISO_UMSK_ISR_KPADDAH_INT31_src(data)                                         ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADDAH_get_INT31(data)                                         ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADDAH_INT30_shift                                             (6)
#define ISO_UMSK_ISR_KPADDAH_INT30_mask                                              (0x00000040)
#define ISO_UMSK_ISR_KPADDAH_INT30(data)                                             (0x00000040&((data)<<6))
#define ISO_UMSK_ISR_KPADDAH_INT30_src(data)                                         ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADDAH_get_INT30(data)                                         ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADDAH_INT29_shift                                             (5)
#define ISO_UMSK_ISR_KPADDAH_INT29_mask                                              (0x00000020)
#define ISO_UMSK_ISR_KPADDAH_INT29(data)                                             (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_KPADDAH_INT29_src(data)                                         ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADDAH_get_INT29(data)                                         ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADDAH_INT28_shift                                             (4)
#define ISO_UMSK_ISR_KPADDAH_INT28_mask                                              (0x00000010)
#define ISO_UMSK_ISR_KPADDAH_INT28(data)                                             (0x00000010&((data)<<4))
#define ISO_UMSK_ISR_KPADDAH_INT28_src(data)                                         ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADDAH_get_INT28(data)                                         ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADDAH_write_data_shift                                        (0)
#define ISO_UMSK_ISR_KPADDAH_write_data_mask                                         (0x00000001)
#define ISO_UMSK_ISR_KPADDAH_write_data(data)                                        (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_KPADDAH_write_data_src(data)                                    ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_KPADDAH_get_write_data(data)                                    ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR_KPADDAL                                                         0x18007018
#define ISO_UMSK_ISR_KPADDAL_reg_addr                                                "0xB8007018"
#define ISO_UMSK_ISR_KPADDAL_reg                                                     0xB8007018
#define set_ISO_UMSK_ISR_KPADDAL_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADDAL_reg)=data)
#define get_ISO_UMSK_ISR_KPADDAL_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_KPADDAL_reg))
#define ISO_UMSK_ISR_KPADDAL_inst_adr                                                "0x0006"
#define ISO_UMSK_ISR_KPADDAL_inst                                                    0x0006
#define ISO_UMSK_ISR_KPADDAL_INT27_shift                                             (31)
#define ISO_UMSK_ISR_KPADDAL_INT27_mask                                              (0x80000000)
#define ISO_UMSK_ISR_KPADDAL_INT27(data)                                             (0x80000000&((data)<<31))
#define ISO_UMSK_ISR_KPADDAL_INT27_src(data)                                         ((0x80000000&(data))>>31)
#define ISO_UMSK_ISR_KPADDAL_get_INT27(data)                                         ((0x80000000&(data))>>31)
#define ISO_UMSK_ISR_KPADDAL_INT26_shift                                             (30)
#define ISO_UMSK_ISR_KPADDAL_INT26_mask                                              (0x40000000)
#define ISO_UMSK_ISR_KPADDAL_INT26(data)                                             (0x40000000&((data)<<30))
#define ISO_UMSK_ISR_KPADDAL_INT26_src(data)                                         ((0x40000000&(data))>>30)
#define ISO_UMSK_ISR_KPADDAL_get_INT26(data)                                         ((0x40000000&(data))>>30)
#define ISO_UMSK_ISR_KPADDAL_INT25_shift                                             (29)
#define ISO_UMSK_ISR_KPADDAL_INT25_mask                                              (0x20000000)
#define ISO_UMSK_ISR_KPADDAL_INT25(data)                                             (0x20000000&((data)<<29))
#define ISO_UMSK_ISR_KPADDAL_INT25_src(data)                                         ((0x20000000&(data))>>29)
#define ISO_UMSK_ISR_KPADDAL_get_INT25(data)                                         ((0x20000000&(data))>>29)
#define ISO_UMSK_ISR_KPADDAL_INT24_shift                                             (28)
#define ISO_UMSK_ISR_KPADDAL_INT24_mask                                              (0x10000000)
#define ISO_UMSK_ISR_KPADDAL_INT24(data)                                             (0x10000000&((data)<<28))
#define ISO_UMSK_ISR_KPADDAL_INT24_src(data)                                         ((0x10000000&(data))>>28)
#define ISO_UMSK_ISR_KPADDAL_get_INT24(data)                                         ((0x10000000&(data))>>28)
#define ISO_UMSK_ISR_KPADDAL_INT23_shift                                             (27)
#define ISO_UMSK_ISR_KPADDAL_INT23_mask                                              (0x08000000)
#define ISO_UMSK_ISR_KPADDAL_INT23(data)                                             (0x08000000&((data)<<27))
#define ISO_UMSK_ISR_KPADDAL_INT23_src(data)                                         ((0x08000000&(data))>>27)
#define ISO_UMSK_ISR_KPADDAL_get_INT23(data)                                         ((0x08000000&(data))>>27)
#define ISO_UMSK_ISR_KPADDAL_INT22_shift                                             (26)
#define ISO_UMSK_ISR_KPADDAL_INT22_mask                                              (0x04000000)
#define ISO_UMSK_ISR_KPADDAL_INT22(data)                                             (0x04000000&((data)<<26))
#define ISO_UMSK_ISR_KPADDAL_INT22_src(data)                                         ((0x04000000&(data))>>26)
#define ISO_UMSK_ISR_KPADDAL_get_INT22(data)                                         ((0x04000000&(data))>>26)
#define ISO_UMSK_ISR_KPADDAL_INT21_shift                                             (25)
#define ISO_UMSK_ISR_KPADDAL_INT21_mask                                              (0x02000000)
#define ISO_UMSK_ISR_KPADDAL_INT21(data)                                             (0x02000000&((data)<<25))
#define ISO_UMSK_ISR_KPADDAL_INT21_src(data)                                         ((0x02000000&(data))>>25)
#define ISO_UMSK_ISR_KPADDAL_get_INT21(data)                                         ((0x02000000&(data))>>25)
#define ISO_UMSK_ISR_KPADDAL_INT20_shift                                             (24)
#define ISO_UMSK_ISR_KPADDAL_INT20_mask                                              (0x01000000)
#define ISO_UMSK_ISR_KPADDAL_INT20(data)                                             (0x01000000&((data)<<24))
#define ISO_UMSK_ISR_KPADDAL_INT20_src(data)                                         ((0x01000000&(data))>>24)
#define ISO_UMSK_ISR_KPADDAL_get_INT20(data)                                         ((0x01000000&(data))>>24)
#define ISO_UMSK_ISR_KPADDAL_INT19_shift                                             (23)
#define ISO_UMSK_ISR_KPADDAL_INT19_mask                                              (0x00800000)
#define ISO_UMSK_ISR_KPADDAL_INT19(data)                                             (0x00800000&((data)<<23))
#define ISO_UMSK_ISR_KPADDAL_INT19_src(data)                                         ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADDAL_get_INT19(data)                                         ((0x00800000&(data))>>23)
#define ISO_UMSK_ISR_KPADDAL_INT18_shift                                             (22)
#define ISO_UMSK_ISR_KPADDAL_INT18_mask                                              (0x00400000)
#define ISO_UMSK_ISR_KPADDAL_INT18(data)                                             (0x00400000&((data)<<22))
#define ISO_UMSK_ISR_KPADDAL_INT18_src(data)                                         ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADDAL_get_INT18(data)                                         ((0x00400000&(data))>>22)
#define ISO_UMSK_ISR_KPADDAL_INT17_shift                                             (21)
#define ISO_UMSK_ISR_KPADDAL_INT17_mask                                              (0x00200000)
#define ISO_UMSK_ISR_KPADDAL_INT17(data)                                             (0x00200000&((data)<<21))
#define ISO_UMSK_ISR_KPADDAL_INT17_src(data)                                         ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADDAL_get_INT17(data)                                         ((0x00200000&(data))>>21)
#define ISO_UMSK_ISR_KPADDAL_INT16_shift                                             (20)
#define ISO_UMSK_ISR_KPADDAL_INT16_mask                                              (0x00100000)
#define ISO_UMSK_ISR_KPADDAL_INT16(data)                                             (0x00100000&((data)<<20))
#define ISO_UMSK_ISR_KPADDAL_INT16_src(data)                                         ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADDAL_get_INT16(data)                                         ((0x00100000&(data))>>20)
#define ISO_UMSK_ISR_KPADDAL_INT15_shift                                             (19)
#define ISO_UMSK_ISR_KPADDAL_INT15_mask                                              (0x00080000)
#define ISO_UMSK_ISR_KPADDAL_INT15(data)                                             (0x00080000&((data)<<19))
#define ISO_UMSK_ISR_KPADDAL_INT15_src(data)                                         ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADDAL_get_INT15(data)                                         ((0x00080000&(data))>>19)
#define ISO_UMSK_ISR_KPADDAL_INT14_shift                                             (18)
#define ISO_UMSK_ISR_KPADDAL_INT14_mask                                              (0x00040000)
#define ISO_UMSK_ISR_KPADDAL_INT14(data)                                             (0x00040000&((data)<<18))
#define ISO_UMSK_ISR_KPADDAL_INT14_src(data)                                         ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADDAL_get_INT14(data)                                         ((0x00040000&(data))>>18)
#define ISO_UMSK_ISR_KPADDAL_INT13_shift                                             (17)
#define ISO_UMSK_ISR_KPADDAL_INT13_mask                                              (0x00020000)
#define ISO_UMSK_ISR_KPADDAL_INT13(data)                                             (0x00020000&((data)<<17))
#define ISO_UMSK_ISR_KPADDAL_INT13_src(data)                                         ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADDAL_get_INT13(data)                                         ((0x00020000&(data))>>17)
#define ISO_UMSK_ISR_KPADDAL_INT12_shift                                             (16)
#define ISO_UMSK_ISR_KPADDAL_INT12_mask                                              (0x00010000)
#define ISO_UMSK_ISR_KPADDAL_INT12(data)                                             (0x00010000&((data)<<16))
#define ISO_UMSK_ISR_KPADDAL_INT12_src(data)                                         ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADDAL_get_INT12(data)                                         ((0x00010000&(data))>>16)
#define ISO_UMSK_ISR_KPADDAL_INT11_shift                                             (15)
#define ISO_UMSK_ISR_KPADDAL_INT11_mask                                              (0x00008000)
#define ISO_UMSK_ISR_KPADDAL_INT11(data)                                             (0x00008000&((data)<<15))
#define ISO_UMSK_ISR_KPADDAL_INT11_src(data)                                         ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADDAL_get_INT11(data)                                         ((0x00008000&(data))>>15)
#define ISO_UMSK_ISR_KPADDAL_INT10_shift                                             (14)
#define ISO_UMSK_ISR_KPADDAL_INT10_mask                                              (0x00004000)
#define ISO_UMSK_ISR_KPADDAL_INT10(data)                                             (0x00004000&((data)<<14))
#define ISO_UMSK_ISR_KPADDAL_INT10_src(data)                                         ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADDAL_get_INT10(data)                                         ((0x00004000&(data))>>14)
#define ISO_UMSK_ISR_KPADDAL_INT9_shift                                              (13)
#define ISO_UMSK_ISR_KPADDAL_INT9_mask                                               (0x00002000)
#define ISO_UMSK_ISR_KPADDAL_INT9(data)                                              (0x00002000&((data)<<13))
#define ISO_UMSK_ISR_KPADDAL_INT9_src(data)                                          ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADDAL_get_INT9(data)                                          ((0x00002000&(data))>>13)
#define ISO_UMSK_ISR_KPADDAL_INT8_shift                                              (12)
#define ISO_UMSK_ISR_KPADDAL_INT8_mask                                               (0x00001000)
#define ISO_UMSK_ISR_KPADDAL_INT8(data)                                              (0x00001000&((data)<<12))
#define ISO_UMSK_ISR_KPADDAL_INT8_src(data)                                          ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADDAL_get_INT8(data)                                          ((0x00001000&(data))>>12)
#define ISO_UMSK_ISR_KPADDAL_INT7_shift                                              (11)
#define ISO_UMSK_ISR_KPADDAL_INT7_mask                                               (0x00000800)
#define ISO_UMSK_ISR_KPADDAL_INT7(data)                                              (0x00000800&((data)<<11))
#define ISO_UMSK_ISR_KPADDAL_INT7_src(data)                                          ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADDAL_get_INT7(data)                                          ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_KPADDAL_INT6_shift                                              (10)
#define ISO_UMSK_ISR_KPADDAL_INT6_mask                                               (0x00000400)
#define ISO_UMSK_ISR_KPADDAL_INT6(data)                                              (0x00000400&((data)<<10))
#define ISO_UMSK_ISR_KPADDAL_INT6_src(data)                                          ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADDAL_get_INT6(data)                                          ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_KPADDAL_INT5_shift                                              (9)
#define ISO_UMSK_ISR_KPADDAL_INT5_mask                                               (0x00000200)
#define ISO_UMSK_ISR_KPADDAL_INT5(data)                                              (0x00000200&((data)<<9))
#define ISO_UMSK_ISR_KPADDAL_INT5_src(data)                                          ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADDAL_get_INT5(data)                                          ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_KPADDAL_INT4_shift                                              (8)
#define ISO_UMSK_ISR_KPADDAL_INT4_mask                                               (0x00000100)
#define ISO_UMSK_ISR_KPADDAL_INT4(data)                                              (0x00000100&((data)<<8))
#define ISO_UMSK_ISR_KPADDAL_INT4_src(data)                                          ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADDAL_get_INT4(data)                                          ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_KPADDAL_INT3_shift                                              (7)
#define ISO_UMSK_ISR_KPADDAL_INT3_mask                                               (0x00000080)
#define ISO_UMSK_ISR_KPADDAL_INT3(data)                                              (0x00000080&((data)<<7))
#define ISO_UMSK_ISR_KPADDAL_INT3_src(data)                                          ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADDAL_get_INT3(data)                                          ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_KPADDAL_INT2_shift                                              (6)
#define ISO_UMSK_ISR_KPADDAL_INT2_mask                                               (0x00000040)
#define ISO_UMSK_ISR_KPADDAL_INT2(data)                                              (0x00000040&((data)<<6))
#define ISO_UMSK_ISR_KPADDAL_INT2_src(data)                                          ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADDAL_get_INT2(data)                                          ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_KPADDAL_INT1_shift                                              (5)
#define ISO_UMSK_ISR_KPADDAL_INT1_mask                                               (0x00000020)
#define ISO_UMSK_ISR_KPADDAL_INT1(data)                                              (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_KPADDAL_INT1_src(data)                                          ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADDAL_get_INT1(data)                                          ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_KPADDAL_INT0_shift                                              (4)
#define ISO_UMSK_ISR_KPADDAL_INT0_mask                                               (0x00000010)
#define ISO_UMSK_ISR_KPADDAL_INT0(data)                                              (0x00000010&((data)<<4))
#define ISO_UMSK_ISR_KPADDAL_INT0_src(data)                                          ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADDAL_get_INT0(data)                                          ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_KPADDAL_write_data_shift                                        (0)
#define ISO_UMSK_ISR_KPADDAL_write_data_mask                                         (0x00000001)
#define ISO_UMSK_ISR_KPADDAL_write_data(data)                                        (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_KPADDAL_write_data_src(data)                                    ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_KPADDAL_get_write_data(data)                                    ((0x00000001&(data))>>0)


#define ISO_UMSK_ISR_SW                                                              0x1800701C
#define ISO_UMSK_ISR_SW_reg_addr                                                     "0xB800701C"
#define ISO_UMSK_ISR_SW_reg                                                          0xB800701C
#define set_ISO_UMSK_ISR_SW_reg(data)   (*((volatile unsigned int*) ISO_UMSK_ISR_SW_reg)=data)
#define get_ISO_UMSK_ISR_SW_reg   (*((volatile unsigned int*) ISO_UMSK_ISR_SW_reg))
#define ISO_UMSK_ISR_SW_inst_adr                                                     "0x0007"
#define ISO_UMSK_ISR_SW_inst                                                         0x0007
#define ISO_UMSK_ISR_SW_DA_INT3_shift                                                (11)
#define ISO_UMSK_ISR_SW_DA_INT3_mask                                                 (0x00000800)
#define ISO_UMSK_ISR_SW_DA_INT3(data)                                                (0x00000800&((data)<<11))
#define ISO_UMSK_ISR_SW_DA_INT3_src(data)                                            ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_SW_get_DA_INT3(data)                                            ((0x00000800&(data))>>11)
#define ISO_UMSK_ISR_SW_DA_INT2_shift                                                (10)
#define ISO_UMSK_ISR_SW_DA_INT2_mask                                                 (0x00000400)
#define ISO_UMSK_ISR_SW_DA_INT2(data)                                                (0x00000400&((data)<<10))
#define ISO_UMSK_ISR_SW_DA_INT2_src(data)                                            ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_SW_get_DA_INT2(data)                                            ((0x00000400&(data))>>10)
#define ISO_UMSK_ISR_SW_DA_INT1_shift                                                (9)
#define ISO_UMSK_ISR_SW_DA_INT1_mask                                                 (0x00000200)
#define ISO_UMSK_ISR_SW_DA_INT1(data)                                                (0x00000200&((data)<<9))
#define ISO_UMSK_ISR_SW_DA_INT1_src(data)                                            ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_SW_get_DA_INT1(data)                                            ((0x00000200&(data))>>9)
#define ISO_UMSK_ISR_SW_DA_INT0_shift                                                (8)
#define ISO_UMSK_ISR_SW_DA_INT0_mask                                                 (0x00000100)
#define ISO_UMSK_ISR_SW_DA_INT0(data)                                                (0x00000100&((data)<<8))
#define ISO_UMSK_ISR_SW_DA_INT0_src(data)                                            ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_SW_get_DA_INT0(data)                                            ((0x00000100&(data))>>8)
#define ISO_UMSK_ISR_SW_A_INT3_shift                                                 (7)
#define ISO_UMSK_ISR_SW_A_INT3_mask                                                  (0x00000080)
#define ISO_UMSK_ISR_SW_A_INT3(data)                                                 (0x00000080&((data)<<7))
#define ISO_UMSK_ISR_SW_A_INT3_src(data)                                             ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_SW_get_A_INT3(data)                                             ((0x00000080&(data))>>7)
#define ISO_UMSK_ISR_SW_A_INT2_shift                                                 (6)
#define ISO_UMSK_ISR_SW_A_INT2_mask                                                  (0x00000040)
#define ISO_UMSK_ISR_SW_A_INT2(data)                                                 (0x00000040&((data)<<6))
#define ISO_UMSK_ISR_SW_A_INT2_src(data)                                             ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_SW_get_A_INT2(data)                                             ((0x00000040&(data))>>6)
#define ISO_UMSK_ISR_SW_A_INT1_shift                                                 (5)
#define ISO_UMSK_ISR_SW_A_INT1_mask                                                  (0x00000020)
#define ISO_UMSK_ISR_SW_A_INT1(data)                                                 (0x00000020&((data)<<5))
#define ISO_UMSK_ISR_SW_A_INT1_src(data)                                             ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_SW_get_A_INT1(data)                                             ((0x00000020&(data))>>5)
#define ISO_UMSK_ISR_SW_A_INT0_shift                                                 (4)
#define ISO_UMSK_ISR_SW_A_INT0_mask                                                  (0x00000010)
#define ISO_UMSK_ISR_SW_A_INT0(data)                                                 (0x00000010&((data)<<4))
#define ISO_UMSK_ISR_SW_A_INT0_src(data)                                             ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_SW_get_A_INT0(data)                                             ((0x00000010&(data))>>4)
#define ISO_UMSK_ISR_SW_write_data_shift                                             (0)
#define ISO_UMSK_ISR_SW_write_data_mask                                              (0x00000001)
#define ISO_UMSK_ISR_SW_write_data(data)                                             (0x00000001&((data)<<0))
#define ISO_UMSK_ISR_SW_write_data_src(data)                                         ((0x00000001&(data))>>0)
#define ISO_UMSK_ISR_SW_get_write_data(data)                                         ((0x00000001&(data))>>0)


#define ISO_DBG                                                                      0x18007020
#define ISO_DBG_reg_addr                                                             "0xB8007020"
#define ISO_DBG_reg                                                                  0xB8007020
#define set_ISO_DBG_reg(data)   (*((volatile unsigned int*) ISO_DBG_reg)=data)
#define get_ISO_DBG_reg   (*((volatile unsigned int*) ISO_DBG_reg))
#define ISO_DBG_inst_adr                                                             "0x0008"
#define ISO_DBG_inst                                                                 0x0008
#define ISO_DBG_write_en5_shift                                                      (19)
#define ISO_DBG_write_en5_mask                                                       (0x00080000)
#define ISO_DBG_write_en5(data)                                                      (0x00080000&((data)<<19))
#define ISO_DBG_write_en5_src(data)                                                  ((0x00080000&(data))>>19)
#define ISO_DBG_get_write_en5(data)                                                  ((0x00080000&(data))>>19)
#define ISO_DBG_standby_dbg_sel_shift                                                (14)
#define ISO_DBG_standby_dbg_sel_mask                                                 (0x0007C000)
#define ISO_DBG_standby_dbg_sel(data)                                                (0x0007C000&((data)<<14))
#define ISO_DBG_standby_dbg_sel_src(data)                                            ((0x0007C000&(data))>>14)
#define ISO_DBG_get_standby_dbg_sel(data)                                            ((0x0007C000&(data))>>14)
#define ISO_DBG_write_en4_shift                                                      (13)
#define ISO_DBG_write_en4_mask                                                       (0x00002000)
#define ISO_DBG_write_en4(data)                                                      (0x00002000&((data)<<13))
#define ISO_DBG_write_en4_src(data)                                                  ((0x00002000&(data))>>13)
#define ISO_DBG_get_write_en4(data)                                                  ((0x00002000&(data))>>13)
#define ISO_DBG_standby_dbg_en_shift                                                 (12)
#define ISO_DBG_standby_dbg_en_mask                                                  (0x00001000)
#define ISO_DBG_standby_dbg_en(data)                                                 (0x00001000&((data)<<12))
#define ISO_DBG_standby_dbg_en_src(data)                                             ((0x00001000&(data))>>12)
#define ISO_DBG_get_standby_dbg_en(data)                                             ((0x00001000&(data))>>12)
#define ISO_DBG_write_en3_shift                                                      (11)
#define ISO_DBG_write_en3_mask                                                       (0x00000800)
#define ISO_DBG_write_en3(data)                                                      (0x00000800&((data)<<11))
#define ISO_DBG_write_en3_src(data)                                                  ((0x00000800&(data))>>11)
#define ISO_DBG_get_write_en3(data)                                                  ((0x00000800&(data))>>11)
#define ISO_DBG_sel1_shift                                                           (7)
#define ISO_DBG_sel1_mask                                                            (0x00000780)
#define ISO_DBG_sel1(data)                                                           (0x00000780&((data)<<7))
#define ISO_DBG_sel1_src(data)                                                       ((0x00000780&(data))>>7)
#define ISO_DBG_get_sel1(data)                                                       ((0x00000780&(data))>>7)
#define ISO_DBG_write_en2_shift                                                      (6)
#define ISO_DBG_write_en2_mask                                                       (0x00000040)
#define ISO_DBG_write_en2(data)                                                      (0x00000040&((data)<<6))
#define ISO_DBG_write_en2_src(data)                                                  ((0x00000040&(data))>>6)
#define ISO_DBG_get_write_en2(data)                                                  ((0x00000040&(data))>>6)
#define ISO_DBG_sel0_shift                                                           (2)
#define ISO_DBG_sel0_mask                                                            (0x0000003C)
#define ISO_DBG_sel0(data)                                                           (0x0000003C&((data)<<2))
#define ISO_DBG_sel0_src(data)                                                       ((0x0000003C&(data))>>2)
#define ISO_DBG_get_sel0(data)                                                       ((0x0000003C&(data))>>2)
#define ISO_DBG_write_en1_shift                                                      (1)
#define ISO_DBG_write_en1_mask                                                       (0x00000002)
#define ISO_DBG_write_en1(data)                                                      (0x00000002&((data)<<1))
#define ISO_DBG_write_en1_src(data)                                                  ((0x00000002&(data))>>1)
#define ISO_DBG_get_write_en1(data)                                                  ((0x00000002&(data))>>1)
#define ISO_DBG_enable_shift                                                         (0)
#define ISO_DBG_enable_mask                                                          (0x00000001)
#define ISO_DBG_enable(data)                                                         (0x00000001&((data)<<0))
#define ISO_DBG_enable_src(data)                                                     ((0x00000001&(data))>>0)
#define ISO_DBG_get_enable(data)                                                     ((0x00000001&(data))>>0)


#define ISO_DUMMY1                                                                   0x18007024
#define ISO_DUMMY1_reg_addr                                                          "0xB8007024"
#define ISO_DUMMY1_reg                                                               0xB8007024
#define set_ISO_DUMMY1_reg(data)   (*((volatile unsigned int*) ISO_DUMMY1_reg)=data)
#define get_ISO_DUMMY1_reg   (*((volatile unsigned int*) ISO_DUMMY1_reg))
#define ISO_DUMMY1_inst_adr                                                          "0x0009"
#define ISO_DUMMY1_inst                                                              0x0009
#define ISO_DUMMY1_rvd31_shift                                                       (31)
#define ISO_DUMMY1_rvd31_mask                                                        (0x80000000)
#define ISO_DUMMY1_rvd31(data)                                                       (0x80000000&((data)<<31))
#define ISO_DUMMY1_rvd31_src(data)                                                   ((0x80000000&(data))>>31)
#define ISO_DUMMY1_get_rvd31(data)                                                   ((0x80000000&(data))>>31)
#define ISO_DUMMY1_rvd30_shift                                                       (30)
#define ISO_DUMMY1_rvd30_mask                                                        (0x40000000)
#define ISO_DUMMY1_rvd30(data)                                                       (0x40000000&((data)<<30))
#define ISO_DUMMY1_rvd30_src(data)                                                   ((0x40000000&(data))>>30)
#define ISO_DUMMY1_get_rvd30(data)                                                   ((0x40000000&(data))>>30)
#define ISO_DUMMY1_rvd29_shift                                                       (29)
#define ISO_DUMMY1_rvd29_mask                                                        (0x20000000)
#define ISO_DUMMY1_rvd29(data)                                                       (0x20000000&((data)<<29))
#define ISO_DUMMY1_rvd29_src(data)                                                   ((0x20000000&(data))>>29)
#define ISO_DUMMY1_get_rvd29(data)                                                   ((0x20000000&(data))>>29)
#define ISO_DUMMY1_rvd28_shift                                                       (28)
#define ISO_DUMMY1_rvd28_mask                                                        (0x10000000)
#define ISO_DUMMY1_rvd28(data)                                                       (0x10000000&((data)<<28))
#define ISO_DUMMY1_rvd28_src(data)                                                   ((0x10000000&(data))>>28)
#define ISO_DUMMY1_get_rvd28(data)                                                   ((0x10000000&(data))>>28)
#define ISO_DUMMY1_rvd27_shift                                                       (27)
#define ISO_DUMMY1_rvd27_mask                                                        (0x08000000)
#define ISO_DUMMY1_rvd27(data)                                                       (0x08000000&((data)<<27))
#define ISO_DUMMY1_rvd27_src(data)                                                   ((0x08000000&(data))>>27)
#define ISO_DUMMY1_get_rvd27(data)                                                   ((0x08000000&(data))>>27)
#define ISO_DUMMY1_rvd26_shift                                                       (26)
#define ISO_DUMMY1_rvd26_mask                                                        (0x04000000)
#define ISO_DUMMY1_rvd26(data)                                                       (0x04000000&((data)<<26))
#define ISO_DUMMY1_rvd26_src(data)                                                   ((0x04000000&(data))>>26)
#define ISO_DUMMY1_get_rvd26(data)                                                   ((0x04000000&(data))>>26)
#define ISO_DUMMY1_rvd25_shift                                                       (25)
#define ISO_DUMMY1_rvd25_mask                                                        (0x02000000)
#define ISO_DUMMY1_rvd25(data)                                                       (0x02000000&((data)<<25))
#define ISO_DUMMY1_rvd25_src(data)                                                   ((0x02000000&(data))>>25)
#define ISO_DUMMY1_get_rvd25(data)                                                   ((0x02000000&(data))>>25)
#define ISO_DUMMY1_rvd24_shift                                                       (24)
#define ISO_DUMMY1_rvd24_mask                                                        (0x01000000)
#define ISO_DUMMY1_rvd24(data)                                                       (0x01000000&((data)<<24))
#define ISO_DUMMY1_rvd24_src(data)                                                   ((0x01000000&(data))>>24)
#define ISO_DUMMY1_get_rvd24(data)                                                   ((0x01000000&(data))>>24)
#define ISO_DUMMY1_rvd23_shift                                                       (23)
#define ISO_DUMMY1_rvd23_mask                                                        (0x00800000)
#define ISO_DUMMY1_rvd23(data)                                                       (0x00800000&((data)<<23))
#define ISO_DUMMY1_rvd23_src(data)                                                   ((0x00800000&(data))>>23)
#define ISO_DUMMY1_get_rvd23(data)                                                   ((0x00800000&(data))>>23)
#define ISO_DUMMY1_rvd22_shift                                                       (22)
#define ISO_DUMMY1_rvd22_mask                                                        (0x00400000)
#define ISO_DUMMY1_rvd22(data)                                                       (0x00400000&((data)<<22))
#define ISO_DUMMY1_rvd22_src(data)                                                   ((0x00400000&(data))>>22)
#define ISO_DUMMY1_get_rvd22(data)                                                   ((0x00400000&(data))>>22)
#define ISO_DUMMY1_rvd21_shift                                                       (21)
#define ISO_DUMMY1_rvd21_mask                                                        (0x00200000)
#define ISO_DUMMY1_rvd21(data)                                                       (0x00200000&((data)<<21))
#define ISO_DUMMY1_rvd21_src(data)                                                   ((0x00200000&(data))>>21)
#define ISO_DUMMY1_get_rvd21(data)                                                   ((0x00200000&(data))>>21)
#define ISO_DUMMY1_rvd20_shift                                                       (20)
#define ISO_DUMMY1_rvd20_mask                                                        (0x00100000)
#define ISO_DUMMY1_rvd20(data)                                                       (0x00100000&((data)<<20))
#define ISO_DUMMY1_rvd20_src(data)                                                   ((0x00100000&(data))>>20)
#define ISO_DUMMY1_get_rvd20(data)                                                   ((0x00100000&(data))>>20)
#define ISO_DUMMY1_rvd19_shift                                                       (19)
#define ISO_DUMMY1_rvd19_mask                                                        (0x00080000)
#define ISO_DUMMY1_rvd19(data)                                                       (0x00080000&((data)<<19))
#define ISO_DUMMY1_rvd19_src(data)                                                   ((0x00080000&(data))>>19)
#define ISO_DUMMY1_get_rvd19(data)                                                   ((0x00080000&(data))>>19)
#define ISO_DUMMY1_rvd18_shift                                                       (18)
#define ISO_DUMMY1_rvd18_mask                                                        (0x00040000)
#define ISO_DUMMY1_rvd18(data)                                                       (0x00040000&((data)<<18))
#define ISO_DUMMY1_rvd18_src(data)                                                   ((0x00040000&(data))>>18)
#define ISO_DUMMY1_get_rvd18(data)                                                   ((0x00040000&(data))>>18)
#define ISO_DUMMY1_rvd17_shift                                                       (17)
#define ISO_DUMMY1_rvd17_mask                                                        (0x00020000)
#define ISO_DUMMY1_rvd17(data)                                                       (0x00020000&((data)<<17))
#define ISO_DUMMY1_rvd17_src(data)                                                   ((0x00020000&(data))>>17)
#define ISO_DUMMY1_get_rvd17(data)                                                   ((0x00020000&(data))>>17)
#define ISO_DUMMY1_rvd16_shift                                                       (16)
#define ISO_DUMMY1_rvd16_mask                                                        (0x00010000)
#define ISO_DUMMY1_rvd16(data)                                                       (0x00010000&((data)<<16))
#define ISO_DUMMY1_rvd16_src(data)                                                   ((0x00010000&(data))>>16)
#define ISO_DUMMY1_get_rvd16(data)                                                   ((0x00010000&(data))>>16)
#define ISO_DUMMY1_rvd15_shift                                                       (15)
#define ISO_DUMMY1_rvd15_mask                                                        (0x00008000)
#define ISO_DUMMY1_rvd15(data)                                                       (0x00008000&((data)<<15))
#define ISO_DUMMY1_rvd15_src(data)                                                   ((0x00008000&(data))>>15)
#define ISO_DUMMY1_get_rvd15(data)                                                   ((0x00008000&(data))>>15)
#define ISO_DUMMY1_rvd14_shift                                                       (14)
#define ISO_DUMMY1_rvd14_mask                                                        (0x00004000)
#define ISO_DUMMY1_rvd14(data)                                                       (0x00004000&((data)<<14))
#define ISO_DUMMY1_rvd14_src(data)                                                   ((0x00004000&(data))>>14)
#define ISO_DUMMY1_get_rvd14(data)                                                   ((0x00004000&(data))>>14)
#define ISO_DUMMY1_rvd13_shift                                                       (13)
#define ISO_DUMMY1_rvd13_mask                                                        (0x00002000)
#define ISO_DUMMY1_rvd13(data)                                                       (0x00002000&((data)<<13))
#define ISO_DUMMY1_rvd13_src(data)                                                   ((0x00002000&(data))>>13)
#define ISO_DUMMY1_get_rvd13(data)                                                   ((0x00002000&(data))>>13)
#define ISO_DUMMY1_rvd12_shift                                                       (12)
#define ISO_DUMMY1_rvd12_mask                                                        (0x00001000)
#define ISO_DUMMY1_rvd12(data)                                                       (0x00001000&((data)<<12))
#define ISO_DUMMY1_rvd12_src(data)                                                   ((0x00001000&(data))>>12)
#define ISO_DUMMY1_get_rvd12(data)                                                   ((0x00001000&(data))>>12)
#define ISO_DUMMY1_rvd11_shift                                                       (11)
#define ISO_DUMMY1_rvd11_mask                                                        (0x00000800)
#define ISO_DUMMY1_rvd11(data)                                                       (0x00000800&((data)<<11))
#define ISO_DUMMY1_rvd11_src(data)                                                   ((0x00000800&(data))>>11)
#define ISO_DUMMY1_get_rvd11(data)                                                   ((0x00000800&(data))>>11)
#define ISO_DUMMY1_rvd10_shift                                                       (10)
#define ISO_DUMMY1_rvd10_mask                                                        (0x00000400)
#define ISO_DUMMY1_rvd10(data)                                                       (0x00000400&((data)<<10))
#define ISO_DUMMY1_rvd10_src(data)                                                   ((0x00000400&(data))>>10)
#define ISO_DUMMY1_get_rvd10(data)                                                   ((0x00000400&(data))>>10)
#define ISO_DUMMY1_rvd9_shift                                                        (9)
#define ISO_DUMMY1_rvd9_mask                                                         (0x00000200)
#define ISO_DUMMY1_rvd9(data)                                                        (0x00000200&((data)<<9))
#define ISO_DUMMY1_rvd9_src(data)                                                    ((0x00000200&(data))>>9)
#define ISO_DUMMY1_get_rvd9(data)                                                    ((0x00000200&(data))>>9)
#define ISO_DUMMY1_rvd8_shift                                                        (8)
#define ISO_DUMMY1_rvd8_mask                                                         (0x00000100)
#define ISO_DUMMY1_rvd8(data)                                                        (0x00000100&((data)<<8))
#define ISO_DUMMY1_rvd8_src(data)                                                    ((0x00000100&(data))>>8)
#define ISO_DUMMY1_get_rvd8(data)                                                    ((0x00000100&(data))>>8)
#define ISO_DUMMY1_rvd7_shift                                                        (7)
#define ISO_DUMMY1_rvd7_mask                                                         (0x00000080)
#define ISO_DUMMY1_rvd7(data)                                                        (0x00000080&((data)<<7))
#define ISO_DUMMY1_rvd7_src(data)                                                    ((0x00000080&(data))>>7)
#define ISO_DUMMY1_get_rvd7(data)                                                    ((0x00000080&(data))>>7)
#define ISO_DUMMY1_rvd6_shift                                                        (6)
#define ISO_DUMMY1_rvd6_mask                                                         (0x00000040)
#define ISO_DUMMY1_rvd6(data)                                                        (0x00000040&((data)<<6))
#define ISO_DUMMY1_rvd6_src(data)                                                    ((0x00000040&(data))>>6)
#define ISO_DUMMY1_get_rvd6(data)                                                    ((0x00000040&(data))>>6)
#define ISO_DUMMY1_rvd5_shift                                                        (5)
#define ISO_DUMMY1_rvd5_mask                                                         (0x00000020)
#define ISO_DUMMY1_rvd5(data)                                                        (0x00000020&((data)<<5))
#define ISO_DUMMY1_rvd5_src(data)                                                    ((0x00000020&(data))>>5)
#define ISO_DUMMY1_get_rvd5(data)                                                    ((0x00000020&(data))>>5)
#define ISO_DUMMY1_rvd4_shift                                                        (4)
#define ISO_DUMMY1_rvd4_mask                                                         (0x00000010)
#define ISO_DUMMY1_rvd4(data)                                                        (0x00000010&((data)<<4))
#define ISO_DUMMY1_rvd4_src(data)                                                    ((0x00000010&(data))>>4)
#define ISO_DUMMY1_get_rvd4(data)                                                    ((0x00000010&(data))>>4)
#define ISO_DUMMY1_rvd3_shift                                                        (3)
#define ISO_DUMMY1_rvd3_mask                                                         (0x00000008)
#define ISO_DUMMY1_rvd3(data)                                                        (0x00000008&((data)<<3))
#define ISO_DUMMY1_rvd3_src(data)                                                    ((0x00000008&(data))>>3)
#define ISO_DUMMY1_get_rvd3(data)                                                    ((0x00000008&(data))>>3)
#define ISO_DUMMY1_rvd2_shift                                                        (2)
#define ISO_DUMMY1_rvd2_mask                                                         (0x00000004)
#define ISO_DUMMY1_rvd2(data)                                                        (0x00000004&((data)<<2))
#define ISO_DUMMY1_rvd2_src(data)                                                    ((0x00000004&(data))>>2)
#define ISO_DUMMY1_get_rvd2(data)                                                    ((0x00000004&(data))>>2)
#define ISO_DUMMY1_rvd1_shift                                                        (1)
#define ISO_DUMMY1_rvd1_mask                                                         (0x00000002)
#define ISO_DUMMY1_rvd1(data)                                                        (0x00000002&((data)<<1))
#define ISO_DUMMY1_rvd1_src(data)                                                    ((0x00000002&(data))>>1)
#define ISO_DUMMY1_get_rvd1(data)                                                    ((0x00000002&(data))>>1)
#define ISO_DUMMY1_rvd0_shift                                                        (0)
#define ISO_DUMMY1_rvd0_mask                                                         (0x00000001)
#define ISO_DUMMY1_rvd0(data)                                                        (0x00000001&((data)<<0))
#define ISO_DUMMY1_rvd0_src(data)                                                    ((0x00000001&(data))>>0)
#define ISO_DUMMY1_get_rvd0(data)                                                    ((0x00000001&(data))>>0)


#define ISO_CHIP_INFO1                                                               0x18007028
#define ISO_CHIP_INFO1_reg_addr                                                      "0xB8007028"
#define ISO_CHIP_INFO1_reg                                                           0xB8007028
#define set_ISO_CHIP_INFO1_reg(data)   (*((volatile unsigned int*) ISO_CHIP_INFO1_reg)=data)
#define get_ISO_CHIP_INFO1_reg   (*((volatile unsigned int*) ISO_CHIP_INFO1_reg))
#define ISO_CHIP_INFO1_inst_adr                                                      "0x000A"
#define ISO_CHIP_INFO1_inst                                                          0x000A
#define ISO_CHIP_INFO1_testmode_shift                                                (13)
#define ISO_CHIP_INFO1_testmode_mask                                                 (0x00002000)
#define ISO_CHIP_INFO1_testmode(data)                                                (0x00002000&((data)<<13))
#define ISO_CHIP_INFO1_testmode_src(data)                                            ((0x00002000&(data))>>13)
#define ISO_CHIP_INFO1_get_testmode(data)                                            ((0x00002000&(data))>>13)
#define ISO_CHIP_INFO1_nf_sel_shift                                                  (12)
#define ISO_CHIP_INFO1_nf_sel_mask                                                   (0x00001000)
#define ISO_CHIP_INFO1_nf_sel(data)                                                  (0x00001000&((data)<<12))
#define ISO_CHIP_INFO1_nf_sel_src(data)                                              ((0x00001000&(data))>>12)
#define ISO_CHIP_INFO1_get_nf_sel(data)                                              ((0x00001000&(data))>>12)
#define ISO_CHIP_INFO1_bound_opt_shift                                               (0)
#define ISO_CHIP_INFO1_bound_opt_mask                                                (0x000007FF)
#define ISO_CHIP_INFO1_bound_opt(data)                                               (0x000007FF&((data)<<0))
#define ISO_CHIP_INFO1_bound_opt_src(data)                                           ((0x000007FF&(data))>>0)
#define ISO_CHIP_INFO1_get_bound_opt(data)                                           ((0x000007FF&(data))>>0)


#define ISO_CHIP_INFO2                                                               0x1800702C
#define ISO_CHIP_INFO2_reg_addr                                                      "0xB800702C"
#define ISO_CHIP_INFO2_reg                                                           0xB800702C
#define set_ISO_CHIP_INFO2_reg(data)   (*((volatile unsigned int*) ISO_CHIP_INFO2_reg)=data)
#define get_ISO_CHIP_INFO2_reg   (*((volatile unsigned int*) ISO_CHIP_INFO2_reg))
#define ISO_CHIP_INFO2_inst_adr                                                      "0x000B"
#define ISO_CHIP_INFO2_inst                                                          0x000B
#define ISO_CHIP_INFO2_pow_latch_shift                                               (0)
#define ISO_CHIP_INFO2_pow_latch_mask                                                (0x0003FFFF)
#define ISO_CHIP_INFO2_pow_latch(data)                                               (0x0003FFFF&((data)<<0))
#define ISO_CHIP_INFO2_pow_latch_src(data)                                           ((0x0003FFFF&(data))>>0)
#define ISO_CHIP_INFO2_get_pow_latch(data)                                           ((0x0003FFFF&(data))>>0)


#define ISO_CTRL                                                                     0x18007030
#define ISO_CTRL_reg_addr                                                            "0xB8007030"
#define ISO_CTRL_reg                                                                 0xB8007030
#define set_ISO_CTRL_reg(data)   (*((volatile unsigned int*) ISO_CTRL_reg)=data)
#define get_ISO_CTRL_reg   (*((volatile unsigned int*) ISO_CTRL_reg))
#define ISO_CTRL_inst_adr                                                            "0x000C"
#define ISO_CTRL_inst                                                                0x000C
#define ISO_CTRL_write_en8_shift                                                     (15)
#define ISO_CTRL_write_en8_mask                                                      (0x00008000)
#define ISO_CTRL_write_en8(data)                                                     (0x00008000&((data)<<15))
#define ISO_CTRL_write_en8_src(data)                                                 ((0x00008000&(data))>>15)
#define ISO_CTRL_get_write_en8(data)                                                 ((0x00008000&(data))>>15)
#define ISO_CTRL_dc_phy_cke_pd_shift                                                 (14)
#define ISO_CTRL_dc_phy_cke_pd_mask                                                  (0x00004000)
#define ISO_CTRL_dc_phy_cke_pd(data)                                                 (0x00004000&((data)<<14))
#define ISO_CTRL_dc_phy_cke_pd_src(data)                                             ((0x00004000&(data))>>14)
#define ISO_CTRL_get_dc_phy_cke_pd(data)                                             ((0x00004000&(data))>>14)
#define ISO_CTRL_write_en7_shift                                                     (13)
#define ISO_CTRL_write_en7_mask                                                      (0x00002000)
#define ISO_CTRL_write_en7(data)                                                     (0x00002000&((data)<<13))
#define ISO_CTRL_write_en7_src(data)                                                 ((0x00002000&(data))>>13)
#define ISO_CTRL_get_write_en7(data)                                                 ((0x00002000&(data))>>13)
#define ISO_CTRL_iso_mis_clk_en_shift                                                (12)
#define ISO_CTRL_iso_mis_clk_en_mask                                                 (0x00001000)
#define ISO_CTRL_iso_mis_clk_en(data)                                                (0x00001000&((data)<<12))
#define ISO_CTRL_iso_mis_clk_en_src(data)                                            ((0x00001000&(data))>>12)
#define ISO_CTRL_get_iso_mis_clk_en(data)                                            ((0x00001000&(data))>>12)
#define ISO_CTRL_write_en6_shift                                                     (11)
#define ISO_CTRL_write_en6_mask                                                      (0x00000800)
#define ISO_CTRL_write_en6(data)                                                     (0x00000800&((data)<<11))
#define ISO_CTRL_write_en6_src(data)                                                 ((0x00000800&(data))>>11)
#define ISO_CTRL_get_write_en6(data)                                                 ((0x00000800&(data))>>11)
#define ISO_CTRL_iso_rbus_sel_shift                                                  (10)
#define ISO_CTRL_iso_rbus_sel_mask                                                   (0x00000400)
#define ISO_CTRL_iso_rbus_sel(data)                                                  (0x00000400&((data)<<10))
#define ISO_CTRL_iso_rbus_sel_src(data)                                              ((0x00000400&(data))>>10)
#define ISO_CTRL_get_iso_rbus_sel(data)                                              ((0x00000400&(data))>>10)
#define ISO_CTRL_write_en5_shift                                                     (9)
#define ISO_CTRL_write_en5_mask                                                      (0x00000200)
#define ISO_CTRL_write_en5(data)                                                     (0x00000200&((data)<<9))
#define ISO_CTRL_write_en5_src(data)                                                 ((0x00000200&(data))>>9)
#define ISO_CTRL_get_write_en5(data)                                                 ((0x00000200&(data))>>9)
#define ISO_CTRL_iso_rst_mask_shift                                                  (8)
#define ISO_CTRL_iso_rst_mask_mask                                                   (0x00000100)
#define ISO_CTRL_iso_rst_mask(data)                                                  (0x00000100&((data)<<8))
#define ISO_CTRL_iso_rst_mask_src(data)                                              ((0x00000100&(data))>>8)
#define ISO_CTRL_get_iso_rst_mask(data)                                              ((0x00000100&(data))>>8)
#define ISO_CTRL_write_en4_shift                                                     (7)
#define ISO_CTRL_write_en4_mask                                                      (0x00000080)
#define ISO_CTRL_write_en4(data)                                                     (0x00000080&((data)<<7))
#define ISO_CTRL_write_en4_src(data)                                                 ((0x00000080&(data))>>7)
#define ISO_CTRL_get_write_en4(data)                                                 ((0x00000080&(data))>>7)
#define ISO_CTRL_clk_osc_on_en_shift                                                 (6)
#define ISO_CTRL_clk_osc_on_en_mask                                                  (0x00000040)
#define ISO_CTRL_clk_osc_on_en(data)                                                 (0x00000040&((data)<<6))
#define ISO_CTRL_clk_osc_on_en_src(data)                                             ((0x00000040&(data))>>6)
#define ISO_CTRL_get_clk_osc_on_en(data)                                             ((0x00000040&(data))>>6)
#define ISO_CTRL_write_en3_shift                                                     (5)
#define ISO_CTRL_write_en3_mask                                                      (0x00000020)
#define ISO_CTRL_write_en3(data)                                                     (0x00000020&((data)<<5))
#define ISO_CTRL_write_en3_src(data)                                                 ((0x00000020&(data))>>5)
#define ISO_CTRL_get_write_en3(data)                                                 ((0x00000020&(data))>>5)
#define ISO_CTRL_crt_sw_rstn_shift                                                   (4)
#define ISO_CTRL_crt_sw_rstn_mask                                                    (0x00000010)
#define ISO_CTRL_crt_sw_rstn(data)                                                   (0x00000010&((data)<<4))
#define ISO_CTRL_crt_sw_rstn_src(data)                                               ((0x00000010&(data))>>4)
#define ISO_CTRL_get_crt_sw_rstn(data)                                               ((0x00000010&(data))>>4)
#define ISO_CTRL_write_en2_shift                                                     (3)
#define ISO_CTRL_write_en2_mask                                                      (0x00000008)
#define ISO_CTRL_write_en2(data)                                                     (0x00000008&((data)<<3))
#define ISO_CTRL_write_en2_src(data)                                                 ((0x00000008&(data))>>3)
#define ISO_CTRL_get_write_en2(data)                                                 ((0x00000008&(data))>>3)
#define ISO_CTRL_iso_clk_sel_shift                                                   (2)
#define ISO_CTRL_iso_clk_sel_mask                                                    (0x00000004)
#define ISO_CTRL_iso_clk_sel(data)                                                   (0x00000004&((data)<<2))
#define ISO_CTRL_iso_clk_sel_src(data)                                               ((0x00000004&(data))>>2)
#define ISO_CTRL_get_iso_clk_sel(data)                                               ((0x00000004&(data))>>2)
#define ISO_CTRL_write_en1_shift                                                     (1)
#define ISO_CTRL_write_en1_mask                                                      (0x00000002)
#define ISO_CTRL_write_en1(data)                                                     (0x00000002&((data)<<1))
#define ISO_CTRL_write_en1_src(data)                                                 ((0x00000002&(data))>>1)
#define ISO_CTRL_get_write_en1(data)                                                 ((0x00000002&(data))>>1)
#define ISO_CTRL_ejtag_en_shift                                                      (0)
#define ISO_CTRL_ejtag_en_mask                                                       (0x00000001)
#define ISO_CTRL_ejtag_en(data)                                                      (0x00000001&((data)<<0))
#define ISO_CTRL_ejtag_en_src(data)                                                  ((0x00000001&(data))>>0)
#define ISO_CTRL_get_ejtag_en(data)                                                  ((0x00000001&(data))>>0)


#define ISO_RTC                                                                      0x18007034
#define ISO_RTC_reg_addr                                                             "0xB8007034"
#define ISO_RTC_reg                                                                  0xB8007034
#define set_ISO_RTC_reg(data)   (*((volatile unsigned int*) ISO_RTC_reg)=data)
#define get_ISO_RTC_reg   (*((volatile unsigned int*) ISO_RTC_reg))
#define ISO_RTC_inst_adr                                                             "0x000D"
#define ISO_RTC_inst                                                                 0x000D
#define ISO_RTC_hsec_int_en_shift                                                    (1)
#define ISO_RTC_hsec_int_en_mask                                                     (0x00000002)
#define ISO_RTC_hsec_int_en(data)                                                    (0x00000002&((data)<<1))
#define ISO_RTC_hsec_int_en_src(data)                                                ((0x00000002&(data))>>1)
#define ISO_RTC_get_hsec_int_en(data)                                                ((0x00000002&(data))>>1)
#define ISO_RTC_alarm_int_en_shift                                                   (0)
#define ISO_RTC_alarm_int_en_mask                                                    (0x00000001)
#define ISO_RTC_alarm_int_en(data)                                                   (0x00000001&((data)<<0))
#define ISO_RTC_alarm_int_en_src(data)                                               ((0x00000001&(data))>>0)
#define ISO_RTC_get_alarm_int_en(data)                                               ((0x00000001&(data))>>0)


#define ISO_CELL                                                                     0x18007038
#define ISO_CELL_reg_addr                                                            "0xB8007038"
#define ISO_CELL_reg                                                                 0xB8007038
#define set_ISO_CELL_reg(data)   (*((volatile unsigned int*) ISO_CELL_reg)=data)
#define get_ISO_CELL_reg   (*((volatile unsigned int*) ISO_CELL_reg))
#define ISO_CELL_inst_adr                                                            "0x000E"
#define ISO_CELL_inst                                                                0x000E
#define ISO_CELL_iso_ctrl_en1_shift                                                  (1)
#define ISO_CELL_iso_ctrl_en1_mask                                                   (0x00000002)
#define ISO_CELL_iso_ctrl_en1(data)                                                  (0x00000002&((data)<<1))
#define ISO_CELL_iso_ctrl_en1_src(data)                                              ((0x00000002&(data))>>1)
#define ISO_CELL_get_iso_ctrl_en1(data)                                              ((0x00000002&(data))>>1)
#define ISO_CELL_iso_ctrl_en0_shift                                                  (0)
#define ISO_CELL_iso_ctrl_en0_mask                                                   (0x00000001)
#define ISO_CELL_iso_ctrl_en0(data)                                                  (0x00000001&((data)<<0))
#define ISO_CELL_iso_ctrl_en0_src(data)                                              ((0x00000001&(data))>>0)
#define ISO_CELL_get_iso_ctrl_en0(data)                                              ((0x00000001&(data))>>0)


#define ISO_POWER                                                                    0x1800703C
#define ISO_POWER_reg_addr                                                           "0xB800703C"
#define ISO_POWER_reg                                                                0xB800703C
#define set_ISO_POWER_reg(data)   (*((volatile unsigned int*) ISO_POWER_reg)=data)
#define get_ISO_POWER_reg   (*((volatile unsigned int*) ISO_POWER_reg))
#define ISO_POWER_inst_adr                                                           "0x000F"
#define ISO_POWER_inst                                                               0x000F
#define ISO_POWER_v1_on_shift                                                        (0)
#define ISO_POWER_v1_on_mask                                                         (0x00000001)
#define ISO_POWER_v1_on(data)                                                        (0x00000001&((data)<<0))
#define ISO_POWER_v1_on_src(data)                                                    ((0x00000001&(data))>>0)
#define ISO_POWER_get_v1_on(data)                                                    ((0x00000001&(data))>>0)


#define ISO_SCPU_INT_EN                                                              0x18007040
#define ISO_SCPU_INT_EN_reg_addr                                                     "0xB8007040"
#define ISO_SCPU_INT_EN_reg                                                          0xB8007040
#define set_ISO_SCPU_INT_EN_reg(data)   (*((volatile unsigned int*) ISO_SCPU_INT_EN_reg)=data)
#define get_ISO_SCPU_INT_EN_reg   (*((volatile unsigned int*) ISO_SCPU_INT_EN_reg))
#define ISO_SCPU_INT_EN_inst_adr                                                     "0x0010"
#define ISO_SCPU_INT_EN_inst                                                         0x0010
#define ISO_SCPU_INT_EN_CECRx_shift                                                  (23)
#define ISO_SCPU_INT_EN_CECRx_mask                                                   (0x00800000)
#define ISO_SCPU_INT_EN_CECRx(data)                                                  (0x00800000&((data)<<23))
#define ISO_SCPU_INT_EN_CECRx_src(data)                                              ((0x00800000&(data))>>23)
#define ISO_SCPU_INT_EN_get_CECRx(data)                                              ((0x00800000&(data))>>23)
#define ISO_SCPU_INT_EN_CECTx_shift                                                  (22)
#define ISO_SCPU_INT_EN_CECTx_mask                                                   (0x00400000)
#define ISO_SCPU_INT_EN_CECTx(data)                                                  (0x00400000&((data)<<22))
#define ISO_SCPU_INT_EN_CECTx_src(data)                                              ((0x00400000&(data))>>22)
#define ISO_SCPU_INT_EN_get_CECTx(data)                                              ((0x00400000&(data))>>22)
#define ISO_SCPU_INT_EN_GPIODA_shift                                                 (20)
#define ISO_SCPU_INT_EN_GPIODA_mask                                                  (0x00100000)
#define ISO_SCPU_INT_EN_GPIODA(data)                                                 (0x00100000&((data)<<20))
#define ISO_SCPU_INT_EN_GPIODA_src(data)                                             ((0x00100000&(data))>>20)
#define ISO_SCPU_INT_EN_get_GPIODA(data)                                             ((0x00100000&(data))>>20)
#define ISO_SCPU_INT_EN_GPIOA_shift                                                  (19)
#define ISO_SCPU_INT_EN_GPIOA_mask                                                   (0x00080000)
#define ISO_SCPU_INT_EN_GPIOA(data)                                                  (0x00080000&((data)<<19))
#define ISO_SCPU_INT_EN_GPIOA_src(data)                                              ((0x00080000&(data))>>19)
#define ISO_SCPU_INT_EN_get_GPIOA(data)                                              ((0x00080000&(data))>>19)
#define ISO_SCPU_INT_EN_VFD_ARDSWDA_shift                                            (18)
#define ISO_SCPU_INT_EN_VFD_ARDSWDA_mask                                             (0x00040000)
#define ISO_SCPU_INT_EN_VFD_ARDSWDA(data)                                            (0x00040000&((data)<<18))
#define ISO_SCPU_INT_EN_VFD_ARDSWDA_src(data)                                        ((0x00040000&(data))>>18)
#define ISO_SCPU_INT_EN_get_VFD_ARDSWDA(data)                                        ((0x00040000&(data))>>18)
#define ISO_SCPU_INT_EN_VFD_ARDSWA_shift                                             (17)
#define ISO_SCPU_INT_EN_VFD_ARDSWA_mask                                              (0x00020000)
#define ISO_SCPU_INT_EN_VFD_ARDSWA(data)                                             (0x00020000&((data)<<17))
#define ISO_SCPU_INT_EN_VFD_ARDSWA_src(data)                                         ((0x00020000&(data))>>17)
#define ISO_SCPU_INT_EN_get_VFD_ARDSWA(data)                                         ((0x00020000&(data))>>17)
#define ISO_SCPU_INT_EN_VFD_ARDKPADDA_shift                                          (16)
#define ISO_SCPU_INT_EN_VFD_ARDKPADDA_mask                                           (0x00010000)
#define ISO_SCPU_INT_EN_VFD_ARDKPADDA(data)                                          (0x00010000&((data)<<16))
#define ISO_SCPU_INT_EN_VFD_ARDKPADDA_src(data)                                      ((0x00010000&(data))>>16)
#define ISO_SCPU_INT_EN_get_VFD_ARDKPADDA(data)                                      ((0x00010000&(data))>>16)
#define ISO_SCPU_INT_EN_VFD_ARDKPADA_shift                                           (15)
#define ISO_SCPU_INT_EN_VFD_ARDKPADA_mask                                            (0x00008000)
#define ISO_SCPU_INT_EN_VFD_ARDKPADA(data)                                           (0x00008000&((data)<<15))
#define ISO_SCPU_INT_EN_VFD_ARDKPADA_src(data)                                       ((0x00008000&(data))>>15)
#define ISO_SCPU_INT_EN_get_VFD_ARDKPADA(data)                                       ((0x00008000&(data))>>15)
#define ISO_SCPU_INT_EN_VFD_WDONE_shift                                              (14)
#define ISO_SCPU_INT_EN_VFD_WDONE_mask                                               (0x00004000)
#define ISO_SCPU_INT_EN_VFD_WDONE(data)                                              (0x00004000&((data)<<14))
#define ISO_SCPU_INT_EN_VFD_WDONE_src(data)                                          ((0x00004000&(data))>>14)
#define ISO_SCPU_INT_EN_get_VFD_WDONE(data)                                          ((0x00004000&(data))>>14)
#define ISO_SCPU_INT_EN_RTC_ALARM_shift                                              (13)
#define ISO_SCPU_INT_EN_RTC_ALARM_mask                                               (0x00002000)
#define ISO_SCPU_INT_EN_RTC_ALARM(data)                                              (0x00002000&((data)<<13))
#define ISO_SCPU_INT_EN_RTC_ALARM_src(data)                                          ((0x00002000&(data))>>13)
#define ISO_SCPU_INT_EN_get_RTC_ALARM(data)                                          ((0x00002000&(data))>>13)
#define ISO_SCPU_INT_EN_RTC_HSEC_shift                                               (12)
#define ISO_SCPU_INT_EN_RTC_HSEC_mask                                                (0x00001000)
#define ISO_SCPU_INT_EN_RTC_HSEC(data)                                               (0x00001000&((data)<<12))
#define ISO_SCPU_INT_EN_RTC_HSEC_src(data)                                           ((0x00001000&(data))>>12)
#define ISO_SCPU_INT_EN_get_RTC_HSEC(data)                                           ((0x00001000&(data))>>12)
#define ISO_SCPU_INT_EN_IRDA_shift                                                   (5)
#define ISO_SCPU_INT_EN_IRDA_mask                                                    (0x00000020)
#define ISO_SCPU_INT_EN_IRDA(data)                                                   (0x00000020&((data)<<5))
#define ISO_SCPU_INT_EN_IRDA_src(data)                                               ((0x00000020&(data))>>5)
#define ISO_SCPU_INT_EN_get_IRDA(data)                                               ((0x00000020&(data))>>5)
#define ISO_SCPU_INT_EN_UR0_shift                                                    (2)
#define ISO_SCPU_INT_EN_UR0_mask                                                     (0x00000004)
#define ISO_SCPU_INT_EN_UR0(data)                                                    (0x00000004&((data)<<2))
#define ISO_SCPU_INT_EN_UR0_src(data)                                                ((0x00000004&(data))>>2)
#define ISO_SCPU_INT_EN_get_UR0(data)                                                ((0x00000004&(data))>>2)


#define ISO_KCPU_INT_EN                                                              0x18007044
#define ISO_KCPU_INT_EN_reg_addr                                                     "0xB8007044"
#define ISO_KCPU_INT_EN_reg                                                          0xB8007044
#define set_ISO_KCPU_INT_EN_reg(data)   (*((volatile unsigned int*) ISO_KCPU_INT_EN_reg)=data)
#define get_ISO_KCPU_INT_EN_reg   (*((volatile unsigned int*) ISO_KCPU_INT_EN_reg))
#define ISO_KCPU_INT_EN_inst_adr                                                     "0x0011"
#define ISO_KCPU_INT_EN_inst                                                         0x0011
#define ISO_KCPU_INT_EN_CECRx_shift                                                  (23)
#define ISO_KCPU_INT_EN_CECRx_mask                                                   (0x00800000)
#define ISO_KCPU_INT_EN_CECRx(data)                                                  (0x00800000&((data)<<23))
#define ISO_KCPU_INT_EN_CECRx_src(data)                                              ((0x00800000&(data))>>23)
#define ISO_KCPU_INT_EN_get_CECRx(data)                                              ((0x00800000&(data))>>23)
#define ISO_KCPU_INT_EN_CECTx_shift                                                  (22)
#define ISO_KCPU_INT_EN_CECTx_mask                                                   (0x00400000)
#define ISO_KCPU_INT_EN_CECTx(data)                                                  (0x00400000&((data)<<22))
#define ISO_KCPU_INT_EN_CECTx_src(data)                                              ((0x00400000&(data))>>22)
#define ISO_KCPU_INT_EN_get_CECTx(data)                                              ((0x00400000&(data))>>22)
#define ISO_KCPU_INT_EN_GPIODA_shift                                                 (20)
#define ISO_KCPU_INT_EN_GPIODA_mask                                                  (0x00100000)
#define ISO_KCPU_INT_EN_GPIODA(data)                                                 (0x00100000&((data)<<20))
#define ISO_KCPU_INT_EN_GPIODA_src(data)                                             ((0x00100000&(data))>>20)
#define ISO_KCPU_INT_EN_get_GPIODA(data)                                             ((0x00100000&(data))>>20)
#define ISO_KCPU_INT_EN_GPIOA_shift                                                  (19)
#define ISO_KCPU_INT_EN_GPIOA_mask                                                   (0x00080000)
#define ISO_KCPU_INT_EN_GPIOA(data)                                                  (0x00080000&((data)<<19))
#define ISO_KCPU_INT_EN_GPIOA_src(data)                                              ((0x00080000&(data))>>19)
#define ISO_KCPU_INT_EN_get_GPIOA(data)                                              ((0x00080000&(data))>>19)
#define ISO_KCPU_INT_EN_VFD_ARDSWDA_shift                                            (18)
#define ISO_KCPU_INT_EN_VFD_ARDSWDA_mask                                             (0x00040000)
#define ISO_KCPU_INT_EN_VFD_ARDSWDA(data)                                            (0x00040000&((data)<<18))
#define ISO_KCPU_INT_EN_VFD_ARDSWDA_src(data)                                        ((0x00040000&(data))>>18)
#define ISO_KCPU_INT_EN_get_VFD_ARDSWDA(data)                                        ((0x00040000&(data))>>18)
#define ISO_KCPU_INT_EN_VFD_ARDSWA_shift                                             (17)
#define ISO_KCPU_INT_EN_VFD_ARDSWA_mask                                              (0x00020000)
#define ISO_KCPU_INT_EN_VFD_ARDSWA(data)                                             (0x00020000&((data)<<17))
#define ISO_KCPU_INT_EN_VFD_ARDSWA_src(data)                                         ((0x00020000&(data))>>17)
#define ISO_KCPU_INT_EN_get_VFD_ARDSWA(data)                                         ((0x00020000&(data))>>17)
#define ISO_KCPU_INT_EN_VFD_ARDKPADDA_shift                                          (16)
#define ISO_KCPU_INT_EN_VFD_ARDKPADDA_mask                                           (0x00010000)
#define ISO_KCPU_INT_EN_VFD_ARDKPADDA(data)                                          (0x00010000&((data)<<16))
#define ISO_KCPU_INT_EN_VFD_ARDKPADDA_src(data)                                      ((0x00010000&(data))>>16)
#define ISO_KCPU_INT_EN_get_VFD_ARDKPADDA(data)                                      ((0x00010000&(data))>>16)
#define ISO_KCPU_INT_EN_VFD_ARDKPADA_shift                                           (15)
#define ISO_KCPU_INT_EN_VFD_ARDKPADA_mask                                            (0x00008000)
#define ISO_KCPU_INT_EN_VFD_ARDKPADA(data)                                           (0x00008000&((data)<<15))
#define ISO_KCPU_INT_EN_VFD_ARDKPADA_src(data)                                       ((0x00008000&(data))>>15)
#define ISO_KCPU_INT_EN_get_VFD_ARDKPADA(data)                                       ((0x00008000&(data))>>15)
#define ISO_KCPU_INT_EN_VFD_WDONE_shift                                              (14)
#define ISO_KCPU_INT_EN_VFD_WDONE_mask                                               (0x00004000)
#define ISO_KCPU_INT_EN_VFD_WDONE(data)                                              (0x00004000&((data)<<14))
#define ISO_KCPU_INT_EN_VFD_WDONE_src(data)                                          ((0x00004000&(data))>>14)
#define ISO_KCPU_INT_EN_get_VFD_WDONE(data)                                          ((0x00004000&(data))>>14)
#define ISO_KCPU_INT_EN_RTC_ALARM_shift                                              (13)
#define ISO_KCPU_INT_EN_RTC_ALARM_mask                                               (0x00002000)
#define ISO_KCPU_INT_EN_RTC_ALARM(data)                                              (0x00002000&((data)<<13))
#define ISO_KCPU_INT_EN_RTC_ALARM_src(data)                                          ((0x00002000&(data))>>13)
#define ISO_KCPU_INT_EN_get_RTC_ALARM(data)                                          ((0x00002000&(data))>>13)
#define ISO_KCPU_INT_EN_RTC_HSEC_shift                                               (12)
#define ISO_KCPU_INT_EN_RTC_HSEC_mask                                                (0x00001000)
#define ISO_KCPU_INT_EN_RTC_HSEC(data)                                               (0x00001000&((data)<<12))
#define ISO_KCPU_INT_EN_RTC_HSEC_src(data)                                           ((0x00001000&(data))>>12)
#define ISO_KCPU_INT_EN_get_RTC_HSEC(data)                                           ((0x00001000&(data))>>12)
#define ISO_KCPU_INT_EN_IRDA_shift                                                   (5)
#define ISO_KCPU_INT_EN_IRDA_mask                                                    (0x00000020)
#define ISO_KCPU_INT_EN_IRDA(data)                                                   (0x00000020&((data)<<5))
#define ISO_KCPU_INT_EN_IRDA_src(data)                                               ((0x00000020&(data))>>5)
#define ISO_KCPU_INT_EN_get_IRDA(data)                                               ((0x00000020&(data))>>5)
#define ISO_KCPU_INT_EN_UR0_shift                                                    (2)
#define ISO_KCPU_INT_EN_UR0_mask                                                     (0x00000004)
#define ISO_KCPU_INT_EN_UR0(data)                                                    (0x00000004&((data)<<2))
#define ISO_KCPU_INT_EN_UR0_src(data)                                                ((0x00000004&(data))>>2)
#define ISO_KCPU_INT_EN_get_UR0(data)                                                ((0x00000004&(data))>>2)


#define ISO_CPU_ST1                                                                  0x18007048
#define ISO_CPU_ST1_reg_addr                                                         "0xB8007048"
#define ISO_CPU_ST1_reg                                                              0xB8007048
#define set_ISO_CPU_ST1_reg(data)   (*((volatile unsigned int*) ISO_CPU_ST1_reg)=data)
#define get_ISO_CPU_ST1_reg   (*((volatile unsigned int*) ISO_CPU_ST1_reg))
#define ISO_CPU_ST1_inst_adr                                                         "0x0012"
#define ISO_CPU_ST1_inst                                                             0x0012
#define ISO_CPU_ST1_kcpu_boot_info_shift                                             (0)
#define ISO_CPU_ST1_kcpu_boot_info_mask                                              (0xFFFFFFFF)
#define ISO_CPU_ST1_kcpu_boot_info(data)                                             (0xFFFFFFFF&((data)<<0))
#define ISO_CPU_ST1_kcpu_boot_info_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define ISO_CPU_ST1_get_kcpu_boot_info(data)                                         ((0xFFFFFFFF&(data))>>0)


#define ISO_CPU_ST1V                                                                 0x1800704C
#define ISO_CPU_ST1V_reg_addr                                                        "0xB800704C"
#define ISO_CPU_ST1V_reg                                                             0xB800704C
#define set_ISO_CPU_ST1V_reg(data)   (*((volatile unsigned int*) ISO_CPU_ST1V_reg)=data)
#define get_ISO_CPU_ST1V_reg   (*((volatile unsigned int*) ISO_CPU_ST1V_reg))
#define ISO_CPU_ST1V_inst_adr                                                        "0x0013"
#define ISO_CPU_ST1V_inst                                                            0x0013
#define ISO_CPU_ST1V_kcpu_boot_info_valid_shift                                      (0)
#define ISO_CPU_ST1V_kcpu_boot_info_valid_mask                                       (0x00000001)
#define ISO_CPU_ST1V_kcpu_boot_info_valid(data)                                      (0x00000001&((data)<<0))
#define ISO_CPU_ST1V_kcpu_boot_info_valid_src(data)                                  ((0x00000001&(data))>>0)
#define ISO_CPU_ST1V_get_kcpu_boot_info_valid(data)                                  ((0x00000001&(data))>>0)


#define ISO_DUMMY2                                                                   0x18007050
#define ISO_DUMMY2_reg_addr                                                          "0xB8007050"
#define ISO_DUMMY2_reg                                                               0xB8007050
#define set_ISO_DUMMY2_reg(data)   (*((volatile unsigned int*) ISO_DUMMY2_reg)=data)
#define get_ISO_DUMMY2_reg   (*((volatile unsigned int*) ISO_DUMMY2_reg))
#define ISO_DUMMY2_inst_adr                                                          "0x0014"
#define ISO_DUMMY2_inst                                                              0x0014
#define ISO_DUMMY2_rvd31_shift                                                       (31)
#define ISO_DUMMY2_rvd31_mask                                                        (0x80000000)
#define ISO_DUMMY2_rvd31(data)                                                       (0x80000000&((data)<<31))
#define ISO_DUMMY2_rvd31_src(data)                                                   ((0x80000000&(data))>>31)
#define ISO_DUMMY2_get_rvd31(data)                                                   ((0x80000000&(data))>>31)
#define ISO_DUMMY2_rvd30_shift                                                       (30)
#define ISO_DUMMY2_rvd30_mask                                                        (0x40000000)
#define ISO_DUMMY2_rvd30(data)                                                       (0x40000000&((data)<<30))
#define ISO_DUMMY2_rvd30_src(data)                                                   ((0x40000000&(data))>>30)
#define ISO_DUMMY2_get_rvd30(data)                                                   ((0x40000000&(data))>>30)
#define ISO_DUMMY2_rvd29_shift                                                       (29)
#define ISO_DUMMY2_rvd29_mask                                                        (0x20000000)
#define ISO_DUMMY2_rvd29(data)                                                       (0x20000000&((data)<<29))
#define ISO_DUMMY2_rvd29_src(data)                                                   ((0x20000000&(data))>>29)
#define ISO_DUMMY2_get_rvd29(data)                                                   ((0x20000000&(data))>>29)
#define ISO_DUMMY2_rvd28_shift                                                       (28)
#define ISO_DUMMY2_rvd28_mask                                                        (0x10000000)
#define ISO_DUMMY2_rvd28(data)                                                       (0x10000000&((data)<<28))
#define ISO_DUMMY2_rvd28_src(data)                                                   ((0x10000000&(data))>>28)
#define ISO_DUMMY2_get_rvd28(data)                                                   ((0x10000000&(data))>>28)
#define ISO_DUMMY2_rvd27_shift                                                       (27)
#define ISO_DUMMY2_rvd27_mask                                                        (0x08000000)
#define ISO_DUMMY2_rvd27(data)                                                       (0x08000000&((data)<<27))
#define ISO_DUMMY2_rvd27_src(data)                                                   ((0x08000000&(data))>>27)
#define ISO_DUMMY2_get_rvd27(data)                                                   ((0x08000000&(data))>>27)
#define ISO_DUMMY2_rvd26_shift                                                       (26)
#define ISO_DUMMY2_rvd26_mask                                                        (0x04000000)
#define ISO_DUMMY2_rvd26(data)                                                       (0x04000000&((data)<<26))
#define ISO_DUMMY2_rvd26_src(data)                                                   ((0x04000000&(data))>>26)
#define ISO_DUMMY2_get_rvd26(data)                                                   ((0x04000000&(data))>>26)
#define ISO_DUMMY2_rvd25_shift                                                       (25)
#define ISO_DUMMY2_rvd25_mask                                                        (0x02000000)
#define ISO_DUMMY2_rvd25(data)                                                       (0x02000000&((data)<<25))
#define ISO_DUMMY2_rvd25_src(data)                                                   ((0x02000000&(data))>>25)
#define ISO_DUMMY2_get_rvd25(data)                                                   ((0x02000000&(data))>>25)
#define ISO_DUMMY2_rvd24_shift                                                       (24)
#define ISO_DUMMY2_rvd24_mask                                                        (0x01000000)
#define ISO_DUMMY2_rvd24(data)                                                       (0x01000000&((data)<<24))
#define ISO_DUMMY2_rvd24_src(data)                                                   ((0x01000000&(data))>>24)
#define ISO_DUMMY2_get_rvd24(data)                                                   ((0x01000000&(data))>>24)
#define ISO_DUMMY2_rvd23_shift                                                       (23)
#define ISO_DUMMY2_rvd23_mask                                                        (0x00800000)
#define ISO_DUMMY2_rvd23(data)                                                       (0x00800000&((data)<<23))
#define ISO_DUMMY2_rvd23_src(data)                                                   ((0x00800000&(data))>>23)
#define ISO_DUMMY2_get_rvd23(data)                                                   ((0x00800000&(data))>>23)
#define ISO_DUMMY2_rvd22_shift                                                       (22)
#define ISO_DUMMY2_rvd22_mask                                                        (0x00400000)
#define ISO_DUMMY2_rvd22(data)                                                       (0x00400000&((data)<<22))
#define ISO_DUMMY2_rvd22_src(data)                                                   ((0x00400000&(data))>>22)
#define ISO_DUMMY2_get_rvd22(data)                                                   ((0x00400000&(data))>>22)
#define ISO_DUMMY2_rvd21_shift                                                       (21)
#define ISO_DUMMY2_rvd21_mask                                                        (0x00200000)
#define ISO_DUMMY2_rvd21(data)                                                       (0x00200000&((data)<<21))
#define ISO_DUMMY2_rvd21_src(data)                                                   ((0x00200000&(data))>>21)
#define ISO_DUMMY2_get_rvd21(data)                                                   ((0x00200000&(data))>>21)
#define ISO_DUMMY2_rvd20_shift                                                       (20)
#define ISO_DUMMY2_rvd20_mask                                                        (0x00100000)
#define ISO_DUMMY2_rvd20(data)                                                       (0x00100000&((data)<<20))
#define ISO_DUMMY2_rvd20_src(data)                                                   ((0x00100000&(data))>>20)
#define ISO_DUMMY2_get_rvd20(data)                                                   ((0x00100000&(data))>>20)
#define ISO_DUMMY2_rvd19_shift                                                       (19)
#define ISO_DUMMY2_rvd19_mask                                                        (0x00080000)
#define ISO_DUMMY2_rvd19(data)                                                       (0x00080000&((data)<<19))
#define ISO_DUMMY2_rvd19_src(data)                                                   ((0x00080000&(data))>>19)
#define ISO_DUMMY2_get_rvd19(data)                                                   ((0x00080000&(data))>>19)
#define ISO_DUMMY2_rvd18_shift                                                       (18)
#define ISO_DUMMY2_rvd18_mask                                                        (0x00040000)
#define ISO_DUMMY2_rvd18(data)                                                       (0x00040000&((data)<<18))
#define ISO_DUMMY2_rvd18_src(data)                                                   ((0x00040000&(data))>>18)
#define ISO_DUMMY2_get_rvd18(data)                                                   ((0x00040000&(data))>>18)
#define ISO_DUMMY2_rvd17_shift                                                       (17)
#define ISO_DUMMY2_rvd17_mask                                                        (0x00020000)
#define ISO_DUMMY2_rvd17(data)                                                       (0x00020000&((data)<<17))
#define ISO_DUMMY2_rvd17_src(data)                                                   ((0x00020000&(data))>>17)
#define ISO_DUMMY2_get_rvd17(data)                                                   ((0x00020000&(data))>>17)
#define ISO_DUMMY2_rvd16_shift                                                       (16)
#define ISO_DUMMY2_rvd16_mask                                                        (0x00010000)
#define ISO_DUMMY2_rvd16(data)                                                       (0x00010000&((data)<<16))
#define ISO_DUMMY2_rvd16_src(data)                                                   ((0x00010000&(data))>>16)
#define ISO_DUMMY2_get_rvd16(data)                                                   ((0x00010000&(data))>>16)
#define ISO_DUMMY2_rvd15_shift                                                       (15)
#define ISO_DUMMY2_rvd15_mask                                                        (0x00008000)
#define ISO_DUMMY2_rvd15(data)                                                       (0x00008000&((data)<<15))
#define ISO_DUMMY2_rvd15_src(data)                                                   ((0x00008000&(data))>>15)
#define ISO_DUMMY2_get_rvd15(data)                                                   ((0x00008000&(data))>>15)
#define ISO_DUMMY2_rvd14_shift                                                       (14)
#define ISO_DUMMY2_rvd14_mask                                                        (0x00004000)
#define ISO_DUMMY2_rvd14(data)                                                       (0x00004000&((data)<<14))
#define ISO_DUMMY2_rvd14_src(data)                                                   ((0x00004000&(data))>>14)
#define ISO_DUMMY2_get_rvd14(data)                                                   ((0x00004000&(data))>>14)
#define ISO_DUMMY2_rvd13_shift                                                       (13)
#define ISO_DUMMY2_rvd13_mask                                                        (0x00002000)
#define ISO_DUMMY2_rvd13(data)                                                       (0x00002000&((data)<<13))
#define ISO_DUMMY2_rvd13_src(data)                                                   ((0x00002000&(data))>>13)
#define ISO_DUMMY2_get_rvd13(data)                                                   ((0x00002000&(data))>>13)
#define ISO_DUMMY2_rvd12_shift                                                       (12)
#define ISO_DUMMY2_rvd12_mask                                                        (0x00001000)
#define ISO_DUMMY2_rvd12(data)                                                       (0x00001000&((data)<<12))
#define ISO_DUMMY2_rvd12_src(data)                                                   ((0x00001000&(data))>>12)
#define ISO_DUMMY2_get_rvd12(data)                                                   ((0x00001000&(data))>>12)
#define ISO_DUMMY2_rvd11_shift                                                       (11)
#define ISO_DUMMY2_rvd11_mask                                                        (0x00000800)
#define ISO_DUMMY2_rvd11(data)                                                       (0x00000800&((data)<<11))
#define ISO_DUMMY2_rvd11_src(data)                                                   ((0x00000800&(data))>>11)
#define ISO_DUMMY2_get_rvd11(data)                                                   ((0x00000800&(data))>>11)
#define ISO_DUMMY2_rvd10_shift                                                       (10)
#define ISO_DUMMY2_rvd10_mask                                                        (0x00000400)
#define ISO_DUMMY2_rvd10(data)                                                       (0x00000400&((data)<<10))
#define ISO_DUMMY2_rvd10_src(data)                                                   ((0x00000400&(data))>>10)
#define ISO_DUMMY2_get_rvd10(data)                                                   ((0x00000400&(data))>>10)
#define ISO_DUMMY2_rvd9_shift                                                        (9)
#define ISO_DUMMY2_rvd9_mask                                                         (0x00000200)
#define ISO_DUMMY2_rvd9(data)                                                        (0x00000200&((data)<<9))
#define ISO_DUMMY2_rvd9_src(data)                                                    ((0x00000200&(data))>>9)
#define ISO_DUMMY2_get_rvd9(data)                                                    ((0x00000200&(data))>>9)
#define ISO_DUMMY2_rvd8_shift                                                        (8)
#define ISO_DUMMY2_rvd8_mask                                                         (0x00000100)
#define ISO_DUMMY2_rvd8(data)                                                        (0x00000100&((data)<<8))
#define ISO_DUMMY2_rvd8_src(data)                                                    ((0x00000100&(data))>>8)
#define ISO_DUMMY2_get_rvd8(data)                                                    ((0x00000100&(data))>>8)
#define ISO_DUMMY2_rvd7_shift                                                        (7)
#define ISO_DUMMY2_rvd7_mask                                                         (0x00000080)
#define ISO_DUMMY2_rvd7(data)                                                        (0x00000080&((data)<<7))
#define ISO_DUMMY2_rvd7_src(data)                                                    ((0x00000080&(data))>>7)
#define ISO_DUMMY2_get_rvd7(data)                                                    ((0x00000080&(data))>>7)
#define ISO_DUMMY2_rvd6_shift                                                        (6)
#define ISO_DUMMY2_rvd6_mask                                                         (0x00000040)
#define ISO_DUMMY2_rvd6(data)                                                        (0x00000040&((data)<<6))
#define ISO_DUMMY2_rvd6_src(data)                                                    ((0x00000040&(data))>>6)
#define ISO_DUMMY2_get_rvd6(data)                                                    ((0x00000040&(data))>>6)
#define ISO_DUMMY2_rvd5_shift                                                        (5)
#define ISO_DUMMY2_rvd5_mask                                                         (0x00000020)
#define ISO_DUMMY2_rvd5(data)                                                        (0x00000020&((data)<<5))
#define ISO_DUMMY2_rvd5_src(data)                                                    ((0x00000020&(data))>>5)
#define ISO_DUMMY2_get_rvd5(data)                                                    ((0x00000020&(data))>>5)
#define ISO_DUMMY2_rvd4_shift                                                        (4)
#define ISO_DUMMY2_rvd4_mask                                                         (0x00000010)
#define ISO_DUMMY2_rvd4(data)                                                        (0x00000010&((data)<<4))
#define ISO_DUMMY2_rvd4_src(data)                                                    ((0x00000010&(data))>>4)
#define ISO_DUMMY2_get_rvd4(data)                                                    ((0x00000010&(data))>>4)
#define ISO_DUMMY2_rvd3_shift                                                        (3)
#define ISO_DUMMY2_rvd3_mask                                                         (0x00000008)
#define ISO_DUMMY2_rvd3(data)                                                        (0x00000008&((data)<<3))
#define ISO_DUMMY2_rvd3_src(data)                                                    ((0x00000008&(data))>>3)
#define ISO_DUMMY2_get_rvd3(data)                                                    ((0x00000008&(data))>>3)
#define ISO_DUMMY2_rvd2_shift                                                        (2)
#define ISO_DUMMY2_rvd2_mask                                                         (0x00000004)
#define ISO_DUMMY2_rvd2(data)                                                        (0x00000004&((data)<<2))
#define ISO_DUMMY2_rvd2_src(data)                                                    ((0x00000004&(data))>>2)
#define ISO_DUMMY2_get_rvd2(data)                                                    ((0x00000004&(data))>>2)
#define ISO_DUMMY2_rvd1_shift                                                        (1)
#define ISO_DUMMY2_rvd1_mask                                                         (0x00000002)
#define ISO_DUMMY2_rvd1(data)                                                        (0x00000002&((data)<<1))
#define ISO_DUMMY2_rvd1_src(data)                                                    ((0x00000002&(data))>>1)
#define ISO_DUMMY2_get_rvd1(data)                                                    ((0x00000002&(data))>>1)
#define ISO_DUMMY2_rvd0_shift                                                        (0)
#define ISO_DUMMY2_rvd0_mask                                                         (0x00000001)
#define ISO_DUMMY2_rvd0(data)                                                        (0x00000001&((data)<<0))
#define ISO_DUMMY2_rvd0_src(data)                                                    ((0x00000001&(data))>>0)
#define ISO_DUMMY2_get_rvd0(data)                                                    ((0x00000001&(data))>>0)


#define ISO_GPDIR                                                                    0x18007100
#define ISO_GPDIR_reg_addr                                                           "0xB8007100"
#define ISO_GPDIR_reg                                                                0xB8007100
#define set_ISO_GPDIR_reg(data)   (*((volatile unsigned int*) ISO_GPDIR_reg)=data)
#define get_ISO_GPDIR_reg   (*((volatile unsigned int*) ISO_GPDIR_reg))
#define ISO_GPDIR_inst_adr                                                           "0x0040"
#define ISO_GPDIR_inst                                                               0x0040
#define ISO_GPDIR_GPDIR_shift                                                        (0)
#define ISO_GPDIR_GPDIR_mask                                                         (0x000007FF)
#define ISO_GPDIR_GPDIR(data)                                                        (0x000007FF&((data)<<0))
#define ISO_GPDIR_GPDIR_src(data)                                                    ((0x000007FF&(data))>>0)
#define ISO_GPDIR_get_GPDIR(data)                                                    ((0x000007FF&(data))>>0)


#define ISO_GPDATO                                                                   0x18007104
#define ISO_GPDATO_reg_addr                                                          "0xB8007104"
#define ISO_GPDATO_reg                                                               0xB8007104
#define set_ISO_GPDATO_reg(data)   (*((volatile unsigned int*) ISO_GPDATO_reg)=data)
#define get_ISO_GPDATO_reg   (*((volatile unsigned int*) ISO_GPDATO_reg))
#define ISO_GPDATO_inst_adr                                                          "0x0041"
#define ISO_GPDATO_inst                                                              0x0041
#define ISO_GPDATO_GPDATO_shift                                                      (0)
#define ISO_GPDATO_GPDATO_mask                                                       (0x000007FF)
#define ISO_GPDATO_GPDATO(data)                                                      (0x000007FF&((data)<<0))
#define ISO_GPDATO_GPDATO_src(data)                                                  ((0x000007FF&(data))>>0)
#define ISO_GPDATO_get_GPDATO(data)                                                  ((0x000007FF&(data))>>0)


#define ISO_GPDATI                                                                   0x18007108
#define ISO_GPDATI_reg_addr                                                          "0xB8007108"
#define ISO_GPDATI_reg                                                               0xB8007108
#define set_ISO_GPDATI_reg(data)   (*((volatile unsigned int*) ISO_GPDATI_reg)=data)
#define get_ISO_GPDATI_reg   (*((volatile unsigned int*) ISO_GPDATI_reg))
#define ISO_GPDATI_inst_adr                                                          "0x0042"
#define ISO_GPDATI_inst                                                              0x0042
#define ISO_GPDATI_GPDATI_shift                                                      (0)
#define ISO_GPDATI_GPDATI_mask                                                       (0x000007FF)
#define ISO_GPDATI_GPDATI(data)                                                      (0x000007FF&((data)<<0))
#define ISO_GPDATI_GPDATI_src(data)                                                  ((0x000007FF&(data))>>0)
#define ISO_GPDATI_get_GPDATI(data)                                                  ((0x000007FF&(data))>>0)


#define ISO_GPIE                                                                     0x1800710C
#define ISO_GPIE_reg_addr                                                            "0xB800710C"
#define ISO_GPIE_reg                                                                 0xB800710C
#define set_ISO_GPIE_reg(data)   (*((volatile unsigned int*) ISO_GPIE_reg)=data)
#define get_ISO_GPIE_reg   (*((volatile unsigned int*) ISO_GPIE_reg))
#define ISO_GPIE_inst_adr                                                            "0x0043"
#define ISO_GPIE_inst                                                                0x0043
#define ISO_GPIE_GP_shift                                                            (0)
#define ISO_GPIE_GP_mask                                                             (0x000007FF)
#define ISO_GPIE_GP(data)                                                            (0x000007FF&((data)<<0))
#define ISO_GPIE_GP_src(data)                                                        ((0x000007FF&(data))>>0)
#define ISO_GPIE_get_GP(data)                                                        ((0x000007FF&(data))>>0)


#define ISO_GPDP                                                                     0x18007110
#define ISO_GPDP_reg_addr                                                            "0xB8007110"
#define ISO_GPDP_reg                                                                 0xB8007110
#define set_ISO_GPDP_reg(data)   (*((volatile unsigned int*) ISO_GPDP_reg)=data)
#define get_ISO_GPDP_reg   (*((volatile unsigned int*) ISO_GPDP_reg))
#define ISO_GPDP_inst_adr                                                            "0x0044"
#define ISO_GPDP_inst                                                                0x0044
#define ISO_GPDP_GPHA_shift                                                          (0)
#define ISO_GPDP_GPHA_mask                                                           (0x000007FF)
#define ISO_GPDP_GPHA(data)                                                          (0x000007FF&((data)<<0))
#define ISO_GPDP_GPHA_src(data)                                                      ((0x000007FF&(data))>>0)
#define ISO_GPDP_get_GPHA(data)                                                      ((0x000007FF&(data))>>0)


#define ISO_GPDEB                                                                    0x18007114
#define ISO_GPDEB_reg_addr                                                           "0xB8007114"
#define ISO_GPDEB_reg                                                                0xB8007114
#define set_ISO_GPDEB_reg(data)   (*((volatile unsigned int*) ISO_GPDEB_reg)=data)
#define get_ISO_GPDEB_reg   (*((volatile unsigned int*) ISO_GPDEB_reg))
#define ISO_GPDEB_inst_adr                                                           "0x0045"
#define ISO_GPDEB_inst                                                               0x0045
#define ISO_GPDEB_write_en1_shift                                                    (3)
#define ISO_GPDEB_write_en1_mask                                                     (0x00000008)
#define ISO_GPDEB_write_en1(data)                                                    (0x00000008&((data)<<3))
#define ISO_GPDEB_write_en1_src(data)                                                ((0x00000008&(data))>>3)
#define ISO_GPDEB_get_write_en1(data)                                                ((0x00000008&(data))>>3)
#define ISO_GPDEB_CLK1_shift                                                         (0)
#define ISO_GPDEB_CLK1_mask                                                          (0x00000007)
#define ISO_GPDEB_CLK1(data)                                                         (0x00000007&((data)<<0))
#define ISO_GPDEB_CLK1_src(data)                                                     ((0x00000007&(data))>>0)
#define ISO_GPDEB_get_CLK1(data)                                                     ((0x00000007&(data))>>0)


#define ISO_VFD_CTL                                                                  0x18007200
#define ISO_VFD_CTL_reg_addr                                                         "0xB8007200"
#define ISO_VFD_CTL_reg                                                              0xB8007200
#define set_ISO_VFD_CTL_reg(data)   (*((volatile unsigned int*) ISO_VFD_CTL_reg)=data)
#define get_ISO_VFD_CTL_reg   (*((volatile unsigned int*) ISO_VFD_CTL_reg))
#define ISO_VFD_CTL_inst_adr                                                         "0x0080"
#define ISO_VFD_CTL_inst                                                             0x0080
#define ISO_VFD_CTL_KPADEN_shift                                                     (3)
#define ISO_VFD_CTL_KPADEN_mask                                                      (0x00000008)
#define ISO_VFD_CTL_KPADEN(data)                                                     (0x00000008&((data)<<3))
#define ISO_VFD_CTL_KPADEN_src(data)                                                 ((0x00000008&(data))>>3)
#define ISO_VFD_CTL_get_KPADEN(data)                                                 ((0x00000008&(data))>>3)
#define ISO_VFD_CTL_SWEN_shift                                                       (2)
#define ISO_VFD_CTL_SWEN_mask                                                        (0x00000004)
#define ISO_VFD_CTL_SWEN(data)                                                       (0x00000004&((data)<<2))
#define ISO_VFD_CTL_SWEN_src(data)                                                   ((0x00000004&(data))>>2)
#define ISO_VFD_CTL_get_SWEN(data)                                                   ((0x00000004&(data))>>2)
#define ISO_VFD_CTL_CLKSEL_shift                                                     (1)
#define ISO_VFD_CTL_CLKSEL_mask                                                      (0x00000002)
#define ISO_VFD_CTL_CLKSEL(data)                                                     (0x00000002&((data)<<1))
#define ISO_VFD_CTL_CLKSEL_src(data)                                                 ((0x00000002&(data))>>1)
#define ISO_VFD_CTL_get_CLKSEL(data)                                                 ((0x00000002&(data))>>1)
#define ISO_VFD_CTL_ENVFD_shift                                                      (0)
#define ISO_VFD_CTL_ENVFD_mask                                                       (0x00000001)
#define ISO_VFD_CTL_ENVFD(data)                                                      (0x00000001&((data)<<0))
#define ISO_VFD_CTL_ENVFD_src(data)                                                  ((0x00000001&(data))>>0)
#define ISO_VFD_CTL_get_ENVFD(data)                                                  ((0x00000001&(data))>>0)


#define ISO_VFD_WRCTL                                                                0x18007204
#define ISO_VFD_WRCTL_reg_addr                                                       "0xB8007204"
#define ISO_VFD_WRCTL_reg                                                            0xB8007204
#define set_ISO_VFD_WRCTL_reg(data)   (*((volatile unsigned int*) ISO_VFD_WRCTL_reg)=data)
#define get_ISO_VFD_WRCTL_reg   (*((volatile unsigned int*) ISO_VFD_WRCTL_reg))
#define ISO_VFD_WRCTL_inst_adr                                                       "0x0081"
#define ISO_VFD_WRCTL_inst                                                           0x0081
#define ISO_VFD_WRCTL_NACT_CSN_shift                                                 (12)
#define ISO_VFD_WRCTL_NACT_CSN_mask                                                  (0x00001000)
#define ISO_VFD_WRCTL_NACT_CSN(data)                                                 (0x00001000&((data)<<12))
#define ISO_VFD_WRCTL_NACT_CSN_src(data)                                             ((0x00001000&(data))>>12)
#define ISO_VFD_WRCTL_get_NACT_CSN(data)                                             ((0x00001000&(data))>>12)
#define ISO_VFD_WRCTL_B3C_shift                                                      (11)
#define ISO_VFD_WRCTL_B3C_mask                                                       (0x00000800)
#define ISO_VFD_WRCTL_B3C(data)                                                      (0x00000800&((data)<<11))
#define ISO_VFD_WRCTL_B3C_src(data)                                                  ((0x00000800&(data))>>11)
#define ISO_VFD_WRCTL_get_B3C(data)                                                  ((0x00000800&(data))>>11)
#define ISO_VFD_WRCTL_B2C_shift                                                      (10)
#define ISO_VFD_WRCTL_B2C_mask                                                       (0x00000400)
#define ISO_VFD_WRCTL_B2C(data)                                                      (0x00000400&((data)<<10))
#define ISO_VFD_WRCTL_B2C_src(data)                                                  ((0x00000400&(data))>>10)
#define ISO_VFD_WRCTL_get_B2C(data)                                                  ((0x00000400&(data))>>10)
#define ISO_VFD_WRCTL_B1C_shift                                                      (9)
#define ISO_VFD_WRCTL_B1C_mask                                                       (0x00000200)
#define ISO_VFD_WRCTL_B1C(data)                                                      (0x00000200&((data)<<9))
#define ISO_VFD_WRCTL_B1C_src(data)                                                  ((0x00000200&(data))>>9)
#define ISO_VFD_WRCTL_get_B1C(data)                                                  ((0x00000200&(data))>>9)
#define ISO_VFD_WRCTL_B0C_shift                                                      (8)
#define ISO_VFD_WRCTL_B0C_mask                                                       (0x00000100)
#define ISO_VFD_WRCTL_B0C(data)                                                      (0x00000100&((data)<<8))
#define ISO_VFD_WRCTL_B0C_src(data)                                                  ((0x00000100&(data))>>8)
#define ISO_VFD_WRCTL_get_B0C(data)                                                  ((0x00000100&(data))>>8)
#define ISO_VFD_WRCTL_ENB3_shift                                                     (7)
#define ISO_VFD_WRCTL_ENB3_mask                                                      (0x00000080)
#define ISO_VFD_WRCTL_ENB3(data)                                                     (0x00000080&((data)<<7))
#define ISO_VFD_WRCTL_ENB3_src(data)                                                 ((0x00000080&(data))>>7)
#define ISO_VFD_WRCTL_get_ENB3(data)                                                 ((0x00000080&(data))>>7)
#define ISO_VFD_WRCTL_ENB2_shift                                                     (6)
#define ISO_VFD_WRCTL_ENB2_mask                                                      (0x00000040)
#define ISO_VFD_WRCTL_ENB2(data)                                                     (0x00000040&((data)<<6))
#define ISO_VFD_WRCTL_ENB2_src(data)                                                 ((0x00000040&(data))>>6)
#define ISO_VFD_WRCTL_get_ENB2(data)                                                 ((0x00000040&(data))>>6)
#define ISO_VFD_WRCTL_ENB1_shift                                                     (5)
#define ISO_VFD_WRCTL_ENB1_mask                                                      (0x00000020)
#define ISO_VFD_WRCTL_ENB1(data)                                                     (0x00000020&((data)<<5))
#define ISO_VFD_WRCTL_ENB1_src(data)                                                 ((0x00000020&(data))>>5)
#define ISO_VFD_WRCTL_get_ENB1(data)                                                 ((0x00000020&(data))>>5)
#define ISO_VFD_WRCTL_ENB0_shift                                                     (4)
#define ISO_VFD_WRCTL_ENB0_mask                                                      (0x00000010)
#define ISO_VFD_WRCTL_ENB0(data)                                                     (0x00000010&((data)<<4))
#define ISO_VFD_WRCTL_ENB0_src(data)                                                 ((0x00000010&(data))>>4)
#define ISO_VFD_WRCTL_get_ENB0(data)                                                 ((0x00000010&(data))>>4)
#define ISO_VFD_WRCTL_WD_shift                                                       (1)
#define ISO_VFD_WRCTL_WD_mask                                                        (0x00000002)
#define ISO_VFD_WRCTL_WD(data)                                                       (0x00000002&((data)<<1))
#define ISO_VFD_WRCTL_WD_src(data)                                                   ((0x00000002&(data))>>1)
#define ISO_VFD_WRCTL_get_WD(data)                                                   ((0x00000002&(data))>>1)
#define ISO_VFD_WRCTL_WDIE_shift                                                     (0)
#define ISO_VFD_WRCTL_WDIE_mask                                                      (0x00000001)
#define ISO_VFD_WRCTL_WDIE(data)                                                     (0x00000001&((data)<<0))
#define ISO_VFD_WRCTL_WDIE_src(data)                                                 ((0x00000001&(data))>>0)
#define ISO_VFD_WRCTL_get_WDIE(data)                                                 ((0x00000001&(data))>>0)


#define ISO_VFDO                                                                     0x18007208
#define ISO_VFDO_reg_addr                                                            "0xB8007208"
#define ISO_VFDO_reg                                                                 0xB8007208
#define set_ISO_VFDO_reg(data)   (*((volatile unsigned int*) ISO_VFDO_reg)=data)
#define get_ISO_VFDO_reg   (*((volatile unsigned int*) ISO_VFDO_reg))
#define ISO_VFDO_inst_adr                                                            "0x0082"
#define ISO_VFDO_inst                                                                0x0082
#define ISO_VFDO_BYTE3_shift                                                         (24)
#define ISO_VFDO_BYTE3_mask                                                          (0xFF000000)
#define ISO_VFDO_BYTE3(data)                                                         (0xFF000000&((data)<<24))
#define ISO_VFDO_BYTE3_src(data)                                                     ((0xFF000000&(data))>>24)
#define ISO_VFDO_get_BYTE3(data)                                                     ((0xFF000000&(data))>>24)
#define ISO_VFDO_BYTE2_shift                                                         (16)
#define ISO_VFDO_BYTE2_mask                                                          (0x00FF0000)
#define ISO_VFDO_BYTE2(data)                                                         (0x00FF0000&((data)<<16))
#define ISO_VFDO_BYTE2_src(data)                                                     ((0x00FF0000&(data))>>16)
#define ISO_VFDO_get_BYTE2(data)                                                     ((0x00FF0000&(data))>>16)
#define ISO_VFDO_BYTE1_shift                                                         (8)
#define ISO_VFDO_BYTE1_mask                                                          (0x0000FF00)
#define ISO_VFDO_BYTE1(data)                                                         (0x0000FF00&((data)<<8))
#define ISO_VFDO_BYTE1_src(data)                                                     ((0x0000FF00&(data))>>8)
#define ISO_VFDO_get_BYTE1(data)                                                     ((0x0000FF00&(data))>>8)
#define ISO_VFDO_BYTE0_shift                                                         (0)
#define ISO_VFDO_BYTE0_mask                                                          (0x000000FF)
#define ISO_VFDO_BYTE0(data)                                                         (0x000000FF&((data)<<0))
#define ISO_VFDO_BYTE0_src(data)                                                     ((0x000000FF&(data))>>0)
#define ISO_VFDO_get_BYTE0(data)                                                     ((0x000000FF&(data))>>0)


#define ISO_VFD_ARDCTL                                                               0x1800720C
#define ISO_VFD_ARDCTL_reg_addr                                                      "0xB800720C"
#define ISO_VFD_ARDCTL_reg                                                           0xB800720C
#define set_ISO_VFD_ARDCTL_reg(data)   (*((volatile unsigned int*) ISO_VFD_ARDCTL_reg)=data)
#define get_ISO_VFD_ARDCTL_reg   (*((volatile unsigned int*) ISO_VFD_ARDCTL_reg))
#define ISO_VFD_ARDCTL_inst_adr                                                      "0x0083"
#define ISO_VFD_ARDCTL_inst                                                          0x0083
#define ISO_VFD_ARDCTL_KPADBN_shift                                                  (8)
#define ISO_VFD_ARDCTL_KPADBN_mask                                                   (0x00000700)
#define ISO_VFD_ARDCTL_KPADBN(data)                                                  (0x00000700&((data)<<8))
#define ISO_VFD_ARDCTL_KPADBN_src(data)                                              ((0x00000700&(data))>>8)
#define ISO_VFD_ARDCTL_get_KPADBN(data)                                              ((0x00000700&(data))>>8)
#define ISO_VFD_ARDCTL_SWHA_shift                                                    (5)
#define ISO_VFD_ARDCTL_SWHA_mask                                                     (0x00000020)
#define ISO_VFD_ARDCTL_SWHA(data)                                                    (0x00000020&((data)<<5))
#define ISO_VFD_ARDCTL_SWHA_src(data)                                                ((0x00000020&(data))>>5)
#define ISO_VFD_ARDCTL_get_SWHA(data)                                                ((0x00000020&(data))>>5)
#define ISO_VFD_ARDCTL_KPADHA_shift                                                  (4)
#define ISO_VFD_ARDCTL_KPADHA_mask                                                   (0x00000010)
#define ISO_VFD_ARDCTL_KPADHA(data)                                                  (0x00000010&((data)<<4))
#define ISO_VFD_ARDCTL_KPADHA_src(data)                                              ((0x00000010&(data))>>4)
#define ISO_VFD_ARDCTL_get_KPADHA(data)                                              ((0x00000010&(data))>>4)
#define ISO_VFD_ARDCTL_ARDP_shift                                                    (0)
#define ISO_VFD_ARDCTL_ARDP_mask                                                     (0x00000003)
#define ISO_VFD_ARDCTL_ARDP(data)                                                    (0x00000003&((data)<<0))
#define ISO_VFD_ARDCTL_ARDP_src(data)                                                ((0x00000003&(data))>>0)
#define ISO_VFD_ARDCTL_get_ARDP(data)                                                ((0x00000003&(data))>>0)


#define ISO_VFD_KPADLIE                                                              0x18007210
#define ISO_VFD_KPADLIE_reg_addr                                                     "0xB8007210"
#define ISO_VFD_KPADLIE_reg                                                          0xB8007210
#define set_ISO_VFD_KPADLIE_reg(data)   (*((volatile unsigned int*) ISO_VFD_KPADLIE_reg)=data)
#define get_ISO_VFD_KPADLIE_reg   (*((volatile unsigned int*) ISO_VFD_KPADLIE_reg))
#define ISO_VFD_KPADLIE_inst_adr                                                     "0x0084"
#define ISO_VFD_KPADLIE_inst                                                         0x0084
#define ISO_VFD_KPADLIE_KPADIE_shift                                                 (0)
#define ISO_VFD_KPADLIE_KPADIE_mask                                                  (0xFFFFFFFF)
#define ISO_VFD_KPADLIE_KPADIE(data)                                                 (0xFFFFFFFF&((data)<<0))
#define ISO_VFD_KPADLIE_KPADIE_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define ISO_VFD_KPADLIE_get_KPADIE(data)                                             ((0xFFFFFFFF&(data))>>0)


#define ISO_VFD_KPADHIE                                                              0x18007214
#define ISO_VFD_KPADHIE_reg_addr                                                     "0xB8007214"
#define ISO_VFD_KPADHIE_reg                                                          0xB8007214
#define set_ISO_VFD_KPADHIE_reg(data)   (*((volatile unsigned int*) ISO_VFD_KPADHIE_reg)=data)
#define get_ISO_VFD_KPADHIE_reg   (*((volatile unsigned int*) ISO_VFD_KPADHIE_reg))
#define ISO_VFD_KPADHIE_inst_adr                                                     "0x0085"
#define ISO_VFD_KPADHIE_inst                                                         0x0085
#define ISO_VFD_KPADHIE_KPADIE_shift                                                 (0)
#define ISO_VFD_KPADHIE_KPADIE_mask                                                  (0x0000FFFF)
#define ISO_VFD_KPADHIE_KPADIE(data)                                                 (0x0000FFFF&((data)<<0))
#define ISO_VFD_KPADHIE_KPADIE_src(data)                                             ((0x0000FFFF&(data))>>0)
#define ISO_VFD_KPADHIE_get_KPADIE(data)                                             ((0x0000FFFF&(data))>>0)


#define ISO_VFD_SWIE                                                                 0x18007218
#define ISO_VFD_SWIE_reg_addr                                                        "0xB8007218"
#define ISO_VFD_SWIE_reg                                                             0xB8007218
#define set_ISO_VFD_SWIE_reg(data)   (*((volatile unsigned int*) ISO_VFD_SWIE_reg)=data)
#define get_ISO_VFD_SWIE_reg   (*((volatile unsigned int*) ISO_VFD_SWIE_reg))
#define ISO_VFD_SWIE_inst_adr                                                        "0x0086"
#define ISO_VFD_SWIE_inst                                                            0x0086
#define ISO_VFD_SWIE_SWIE_shift                                                      (0)
#define ISO_VFD_SWIE_SWIE_mask                                                       (0x0000000F)
#define ISO_VFD_SWIE_SWIE(data)                                                      (0x0000000F&((data)<<0))
#define ISO_VFD_SWIE_SWIE_src(data)                                                  ((0x0000000F&(data))>>0)
#define ISO_VFD_SWIE_get_SWIE(data)                                                  ((0x0000000F&(data))>>0)


#define ISO_VFD_ARDKPADL                                                             0x1800721C
#define ISO_VFD_ARDKPADL_reg_addr                                                    "0xB800721C"
#define ISO_VFD_ARDKPADL_reg                                                         0xB800721C
#define set_ISO_VFD_ARDKPADL_reg(data)   (*((volatile unsigned int*) ISO_VFD_ARDKPADL_reg)=data)
#define get_ISO_VFD_ARDKPADL_reg   (*((volatile unsigned int*) ISO_VFD_ARDKPADL_reg))
#define ISO_VFD_ARDKPADL_inst_adr                                                    "0x0087"
#define ISO_VFD_ARDKPADL_inst                                                        0x0087
#define ISO_VFD_ARDKPADL_ST_31_24_shift                                              (24)
#define ISO_VFD_ARDKPADL_ST_31_24_mask                                               (0xFF000000)
#define ISO_VFD_ARDKPADL_ST_31_24(data)                                              (0xFF000000&((data)<<24))
#define ISO_VFD_ARDKPADL_ST_31_24_src(data)                                          ((0xFF000000&(data))>>24)
#define ISO_VFD_ARDKPADL_get_ST_31_24(data)                                          ((0xFF000000&(data))>>24)
#define ISO_VFD_ARDKPADL_ST_23_16_shift                                              (16)
#define ISO_VFD_ARDKPADL_ST_23_16_mask                                               (0x00FF0000)
#define ISO_VFD_ARDKPADL_ST_23_16(data)                                              (0x00FF0000&((data)<<16))
#define ISO_VFD_ARDKPADL_ST_23_16_src(data)                                          ((0x00FF0000&(data))>>16)
#define ISO_VFD_ARDKPADL_get_ST_23_16(data)                                          ((0x00FF0000&(data))>>16)
#define ISO_VFD_ARDKPADL_ST_15_8_shift                                               (8)
#define ISO_VFD_ARDKPADL_ST_15_8_mask                                                (0x0000FF00)
#define ISO_VFD_ARDKPADL_ST_15_8(data)                                               (0x0000FF00&((data)<<8))
#define ISO_VFD_ARDKPADL_ST_15_8_src(data)                                           ((0x0000FF00&(data))>>8)
#define ISO_VFD_ARDKPADL_get_ST_15_8(data)                                           ((0x0000FF00&(data))>>8)
#define ISO_VFD_ARDKPADL_ST_7_0_shift                                                (0)
#define ISO_VFD_ARDKPADL_ST_7_0_mask                                                 (0x000000FF)
#define ISO_VFD_ARDKPADL_ST_7_0(data)                                                (0x000000FF&((data)<<0))
#define ISO_VFD_ARDKPADL_ST_7_0_src(data)                                            ((0x000000FF&(data))>>0)
#define ISO_VFD_ARDKPADL_get_ST_7_0(data)                                            ((0x000000FF&(data))>>0)


#define ISO_VFD_ARDKPADH                                                             0x18007220
#define ISO_VFD_ARDKPADH_reg_addr                                                    "0xB8007220"
#define ISO_VFD_ARDKPADH_reg                                                         0xB8007220
#define set_ISO_VFD_ARDKPADH_reg(data)   (*((volatile unsigned int*) ISO_VFD_ARDKPADH_reg)=data)
#define get_ISO_VFD_ARDKPADH_reg   (*((volatile unsigned int*) ISO_VFD_ARDKPADH_reg))
#define ISO_VFD_ARDKPADH_inst_adr                                                    "0x0088"
#define ISO_VFD_ARDKPADH_inst                                                        0x0088
#define ISO_VFD_ARDKPADH_ST_15_8_shift                                               (8)
#define ISO_VFD_ARDKPADH_ST_15_8_mask                                                (0x0000FF00)
#define ISO_VFD_ARDKPADH_ST_15_8(data)                                               (0x0000FF00&((data)<<8))
#define ISO_VFD_ARDKPADH_ST_15_8_src(data)                                           ((0x0000FF00&(data))>>8)
#define ISO_VFD_ARDKPADH_get_ST_15_8(data)                                           ((0x0000FF00&(data))>>8)
#define ISO_VFD_ARDKPADH_ST_7_0_shift                                                (0)
#define ISO_VFD_ARDKPADH_ST_7_0_mask                                                 (0x000000FF)
#define ISO_VFD_ARDKPADH_ST_7_0(data)                                                (0x000000FF&((data)<<0))
#define ISO_VFD_ARDKPADH_ST_7_0_src(data)                                            ((0x000000FF&(data))>>0)
#define ISO_VFD_ARDKPADH_get_ST_7_0(data)                                            ((0x000000FF&(data))>>0)


#define ISO_VFD_ARDSW                                                                0x18007224
#define ISO_VFD_ARDSW_reg_addr                                                       "0xB8007224"
#define ISO_VFD_ARDSW_reg                                                            0xB8007224
#define set_ISO_VFD_ARDSW_reg(data)   (*((volatile unsigned int*) ISO_VFD_ARDSW_reg)=data)
#define get_ISO_VFD_ARDSW_reg   (*((volatile unsigned int*) ISO_VFD_ARDSW_reg))
#define ISO_VFD_ARDSW_inst_adr                                                       "0x0089"
#define ISO_VFD_ARDSW_inst                                                           0x0089
#define ISO_VFD_ARDSW_RvdA_shift                                                     (4)
#define ISO_VFD_ARDSW_RvdA_mask                                                      (0x0000FFF0)
#define ISO_VFD_ARDSW_RvdA(data)                                                     (0x0000FFF0&((data)<<4))
#define ISO_VFD_ARDSW_RvdA_src(data)                                                 ((0x0000FFF0&(data))>>4)
#define ISO_VFD_ARDSW_get_RvdA(data)                                                 ((0x0000FFF0&(data))>>4)
#define ISO_VFD_ARDSW_ST_shift                                                       (0)
#define ISO_VFD_ARDSW_ST_mask                                                        (0x0000000F)
#define ISO_VFD_ARDSW_ST(data)                                                       (0x0000000F&((data)<<0))
#define ISO_VFD_ARDSW_ST_src(data)                                                   ((0x0000000F&(data))>>0)
#define ISO_VFD_ARDSW_get_ST(data)                                                   ((0x0000000F&(data))>>0)


#define ISO_VFD_CTL1                                                                 0x18007228
#define ISO_VFD_CTL1_reg_addr                                                        "0xB8007228"
#define ISO_VFD_CTL1_reg                                                             0xB8007228
#define set_ISO_VFD_CTL1_reg(data)   (*((volatile unsigned int*) ISO_VFD_CTL1_reg)=data)
#define get_ISO_VFD_CTL1_reg   (*((volatile unsigned int*) ISO_VFD_CTL1_reg))
#define ISO_VFD_CTL1_inst_adr                                                        "0x008A"
#define ISO_VFD_CTL1_inst                                                            0x008A
#define ISO_VFD_CTL1_CLKSEL_1_shift                                                  (16)
#define ISO_VFD_CTL1_CLKSEL_1_mask                                                   (0x00010000)
#define ISO_VFD_CTL1_CLKSEL_1(data)                                                  (0x00010000&((data)<<16))
#define ISO_VFD_CTL1_CLKSEL_1_src(data)                                              ((0x00010000&(data))>>16)
#define ISO_VFD_CTL1_get_CLKSEL_1(data)                                              ((0x00010000&(data))>>16)
#define ISO_VFD_CTL1_Tclk_csn_shift                                                  (8)
#define ISO_VFD_CTL1_Tclk_csn_mask                                                   (0x0000FF00)
#define ISO_VFD_CTL1_Tclk_csn(data)                                                  (0x0000FF00&((data)<<8))
#define ISO_VFD_CTL1_Tclk_csn_src(data)                                              ((0x0000FF00&(data))>>8)
#define ISO_VFD_CTL1_get_Tclk_csn(data)                                              ((0x0000FF00&(data))>>8)
#define ISO_VFD_CTL1_Twait1_shift                                                    (0)
#define ISO_VFD_CTL1_Twait1_mask                                                     (0x000000FF)
#define ISO_VFD_CTL1_Twait1(data)                                                    (0x000000FF&((data)<<0))
#define ISO_VFD_CTL1_Twait1_src(data)                                                ((0x000000FF&(data))>>0)
#define ISO_VFD_CTL1_get_Twait1(data)                                                ((0x000000FF&(data))>>0)


#define ISO_PFUNC1                                                                   0x18007300
#define ISO_PFUNC1_reg_addr                                                          "0xB8007300"
#define ISO_PFUNC1_reg                                                               0xB8007300
#define set_ISO_PFUNC1_reg(data)   (*((volatile unsigned int*) ISO_PFUNC1_reg)=data)
#define get_ISO_PFUNC1_reg   (*((volatile unsigned int*) ISO_PFUNC1_reg))
#define ISO_PFUNC1_inst_adr                                                          "0x00C0"
#define ISO_PFUNC1_inst                                                              0x00C0
#define ISO_PFUNC1_standby_smt_shift                                                 (27)
#define ISO_PFUNC1_standby_smt_mask                                                  (0x08000000)
#define ISO_PFUNC1_standby_smt(data)                                                 (0x08000000&((data)<<27))
#define ISO_PFUNC1_standby_smt_src(data)                                             ((0x08000000&(data))>>27)
#define ISO_PFUNC1_get_standby_smt(data)                                             ((0x08000000&(data))>>27)
#define ISO_PFUNC1_standby_e2_shift                                                  (26)
#define ISO_PFUNC1_standby_e2_mask                                                   (0x04000000)
#define ISO_PFUNC1_standby_e2(data)                                                  (0x04000000&((data)<<26))
#define ISO_PFUNC1_standby_e2_src(data)                                              ((0x04000000&(data))>>26)
#define ISO_PFUNC1_get_standby_e2(data)                                              ((0x04000000&(data))>>26)
#define ISO_PFUNC1_standby_pud_en_shift                                              (25)
#define ISO_PFUNC1_standby_pud_en_mask                                               (0x02000000)
#define ISO_PFUNC1_standby_pud_en(data)                                              (0x02000000&((data)<<25))
#define ISO_PFUNC1_standby_pud_en_src(data)                                          ((0x02000000&(data))>>25)
#define ISO_PFUNC1_get_standby_pud_en(data)                                          ((0x02000000&(data))>>25)
#define ISO_PFUNC1_standby_pud_sel_shift                                             (24)
#define ISO_PFUNC1_standby_pud_sel_mask                                              (0x01000000)
#define ISO_PFUNC1_standby_pud_sel(data)                                             (0x01000000&((data)<<24))
#define ISO_PFUNC1_standby_pud_sel_src(data)                                         ((0x01000000&(data))>>24)
#define ISO_PFUNC1_get_standby_pud_sel(data)                                         ((0x01000000&(data))>>24)
#define ISO_PFUNC1_vfd_d_smt_shift                                                   (23)
#define ISO_PFUNC1_vfd_d_smt_mask                                                    (0x00800000)
#define ISO_PFUNC1_vfd_d_smt(data)                                                   (0x00800000&((data)<<23))
#define ISO_PFUNC1_vfd_d_smt_src(data)                                               ((0x00800000&(data))>>23)
#define ISO_PFUNC1_get_vfd_d_smt(data)                                               ((0x00800000&(data))>>23)
#define ISO_PFUNC1_vfd_d_e2_shift                                                    (22)
#define ISO_PFUNC1_vfd_d_e2_mask                                                     (0x00400000)
#define ISO_PFUNC1_vfd_d_e2(data)                                                    (0x00400000&((data)<<22))
#define ISO_PFUNC1_vfd_d_e2_src(data)                                                ((0x00400000&(data))>>22)
#define ISO_PFUNC1_get_vfd_d_e2(data)                                                ((0x00400000&(data))>>22)
#define ISO_PFUNC1_vfd_d_pud_en_shift                                                (21)
#define ISO_PFUNC1_vfd_d_pud_en_mask                                                 (0x00200000)
#define ISO_PFUNC1_vfd_d_pud_en(data)                                                (0x00200000&((data)<<21))
#define ISO_PFUNC1_vfd_d_pud_en_src(data)                                            ((0x00200000&(data))>>21)
#define ISO_PFUNC1_get_vfd_d_pud_en(data)                                            ((0x00200000&(data))>>21)
#define ISO_PFUNC1_vfd_d_pud_sel_shift                                               (20)
#define ISO_PFUNC1_vfd_d_pud_sel_mask                                                (0x00100000)
#define ISO_PFUNC1_vfd_d_pud_sel(data)                                               (0x00100000&((data)<<20))
#define ISO_PFUNC1_vfd_d_pud_sel_src(data)                                           ((0x00100000&(data))>>20)
#define ISO_PFUNC1_get_vfd_d_pud_sel(data)                                           ((0x00100000&(data))>>20)
#define ISO_PFUNC1_vfd_clk_smt_shift                                                 (19)
#define ISO_PFUNC1_vfd_clk_smt_mask                                                  (0x00080000)
#define ISO_PFUNC1_vfd_clk_smt(data)                                                 (0x00080000&((data)<<19))
#define ISO_PFUNC1_vfd_clk_smt_src(data)                                             ((0x00080000&(data))>>19)
#define ISO_PFUNC1_get_vfd_clk_smt(data)                                             ((0x00080000&(data))>>19)
#define ISO_PFUNC1_vfd_clk_e2_shift                                                  (18)
#define ISO_PFUNC1_vfd_clk_e2_mask                                                   (0x00040000)
#define ISO_PFUNC1_vfd_clk_e2(data)                                                  (0x00040000&((data)<<18))
#define ISO_PFUNC1_vfd_clk_e2_src(data)                                              ((0x00040000&(data))>>18)
#define ISO_PFUNC1_get_vfd_clk_e2(data)                                              ((0x00040000&(data))>>18)
#define ISO_PFUNC1_vfd_clk_pud_en_shift                                              (17)
#define ISO_PFUNC1_vfd_clk_pud_en_mask                                               (0x00020000)
#define ISO_PFUNC1_vfd_clk_pud_en(data)                                              (0x00020000&((data)<<17))
#define ISO_PFUNC1_vfd_clk_pud_en_src(data)                                          ((0x00020000&(data))>>17)
#define ISO_PFUNC1_get_vfd_clk_pud_en(data)                                          ((0x00020000&(data))>>17)
#define ISO_PFUNC1_vfd_clk_pud_sel_shift                                             (16)
#define ISO_PFUNC1_vfd_clk_pud_sel_mask                                              (0x00010000)
#define ISO_PFUNC1_vfd_clk_pud_sel(data)                                             (0x00010000&((data)<<16))
#define ISO_PFUNC1_vfd_clk_pud_sel_src(data)                                         ((0x00010000&(data))>>16)
#define ISO_PFUNC1_get_vfd_clk_pud_sel(data)                                         ((0x00010000&(data))>>16)
#define ISO_PFUNC1_vfd_cs_n_smt_shift                                                (15)
#define ISO_PFUNC1_vfd_cs_n_smt_mask                                                 (0x00008000)
#define ISO_PFUNC1_vfd_cs_n_smt(data)                                                (0x00008000&((data)<<15))
#define ISO_PFUNC1_vfd_cs_n_smt_src(data)                                            ((0x00008000&(data))>>15)
#define ISO_PFUNC1_get_vfd_cs_n_smt(data)                                            ((0x00008000&(data))>>15)
#define ISO_PFUNC1_vfd_cs_n_e2_shift                                                 (14)
#define ISO_PFUNC1_vfd_cs_n_e2_mask                                                  (0x00004000)
#define ISO_PFUNC1_vfd_cs_n_e2(data)                                                 (0x00004000&((data)<<14))
#define ISO_PFUNC1_vfd_cs_n_e2_src(data)                                             ((0x00004000&(data))>>14)
#define ISO_PFUNC1_get_vfd_cs_n_e2(data)                                             ((0x00004000&(data))>>14)
#define ISO_PFUNC1_vfd_cs_n_pud_en_shift                                             (13)
#define ISO_PFUNC1_vfd_cs_n_pud_en_mask                                              (0x00002000)
#define ISO_PFUNC1_vfd_cs_n_pud_en(data)                                             (0x00002000&((data)<<13))
#define ISO_PFUNC1_vfd_cs_n_pud_en_src(data)                                         ((0x00002000&(data))>>13)
#define ISO_PFUNC1_get_vfd_cs_n_pud_en(data)                                         ((0x00002000&(data))>>13)
#define ISO_PFUNC1_vfd_cs_n_pud_sel_shift                                            (12)
#define ISO_PFUNC1_vfd_cs_n_pud_sel_mask                                             (0x00001000)
#define ISO_PFUNC1_vfd_cs_n_pud_sel(data)                                            (0x00001000&((data)<<12))
#define ISO_PFUNC1_vfd_cs_n_pud_sel_src(data)                                        ((0x00001000&(data))>>12)
#define ISO_PFUNC1_get_vfd_cs_n_pud_sel(data)                                        ((0x00001000&(data))>>12)
#define ISO_PFUNC1_ir_in_smt_shift                                                   (11)
#define ISO_PFUNC1_ir_in_smt_mask                                                    (0x00000800)
#define ISO_PFUNC1_ir_in_smt(data)                                                   (0x00000800&((data)<<11))
#define ISO_PFUNC1_ir_in_smt_src(data)                                               ((0x00000800&(data))>>11)
#define ISO_PFUNC1_get_ir_in_smt(data)                                               ((0x00000800&(data))>>11)
#define ISO_PFUNC1_ir_in_e2_shift                                                    (10)
#define ISO_PFUNC1_ir_in_e2_mask                                                     (0x00000400)
#define ISO_PFUNC1_ir_in_e2(data)                                                    (0x00000400&((data)<<10))
#define ISO_PFUNC1_ir_in_e2_src(data)                                                ((0x00000400&(data))>>10)
#define ISO_PFUNC1_get_ir_in_e2(data)                                                ((0x00000400&(data))>>10)
#define ISO_PFUNC1_ir_in_pud_en_shift                                                (9)
#define ISO_PFUNC1_ir_in_pud_en_mask                                                 (0x00000200)
#define ISO_PFUNC1_ir_in_pud_en(data)                                                (0x00000200&((data)<<9))
#define ISO_PFUNC1_ir_in_pud_en_src(data)                                            ((0x00000200&(data))>>9)
#define ISO_PFUNC1_get_ir_in_pud_en(data)                                            ((0x00000200&(data))>>9)
#define ISO_PFUNC1_ir_in_pud_sel_shift                                               (8)
#define ISO_PFUNC1_ir_in_pud_sel_mask                                                (0x00000100)
#define ISO_PFUNC1_ir_in_pud_sel(data)                                               (0x00000100&((data)<<8))
#define ISO_PFUNC1_ir_in_pud_sel_src(data)                                           ((0x00000100&(data))>>8)
#define ISO_PFUNC1_get_ir_in_pud_sel(data)                                           ((0x00000100&(data))>>8)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_smt_shift                                     (7)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_smt_mask                                      (0x00000080)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_smt(data)                                     (0x00000080&((data)<<7))
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_smt_src(data)                                 ((0x00000080&(data))>>7)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_1_smt(data)                                 ((0x00000080&(data))>>7)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_e2_shift                                      (6)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_e2_mask                                       (0x00000040)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_e2(data)                                      (0x00000040&((data)<<6))
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_e2_src(data)                                  ((0x00000040&(data))>>6)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_1_e2(data)                                  ((0x00000040&(data))>>6)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_en_shift                                  (5)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_en_mask                                   (0x00000020)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_en(data)                                  (0x00000020&((data)<<5))
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_en_src(data)                              ((0x00000020&(data))>>5)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_1_pud_en(data)                              ((0x00000020&(data))>>5)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_sel_shift                                 (4)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_sel_mask                                  (0x00000010)
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_sel(data)                                 (0x00000010&((data)<<4))
#define ISO_PFUNC1_usb_ovrcur_flag_n_1_pud_sel_src(data)                             ((0x00000010&(data))>>4)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_1_pud_sel(data)                             ((0x00000010&(data))>>4)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_smt_shift                                     (3)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_smt_mask                                      (0x00000008)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_smt(data)                                     (0x00000008&((data)<<3))
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_smt_src(data)                                 ((0x00000008&(data))>>3)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_0_smt(data)                                 ((0x00000008&(data))>>3)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_e2_shift                                      (2)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_e2_mask                                       (0x00000004)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_e2(data)                                      (0x00000004&((data)<<2))
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_e2_src(data)                                  ((0x00000004&(data))>>2)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_0_e2(data)                                  ((0x00000004&(data))>>2)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_en_shift                                  (1)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_en_mask                                   (0x00000002)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_en(data)                                  (0x00000002&((data)<<1))
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_en_src(data)                              ((0x00000002&(data))>>1)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_0_pud_en(data)                              ((0x00000002&(data))>>1)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_sel_shift                                 (0)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_sel_mask                                  (0x00000001)
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_sel(data)                                 (0x00000001&((data)<<0))
#define ISO_PFUNC1_usb_ovrcur_flag_n_0_pud_sel_src(data)                             ((0x00000001&(data))>>0)
#define ISO_PFUNC1_get_usb_ovrcur_flag_n_0_pud_sel(data)                             ((0x00000001&(data))>>0)


#define ISO_PFUNC2                                                                   0x18007304
#define ISO_PFUNC2_reg_addr                                                          "0xB8007304"
#define ISO_PFUNC2_reg                                                               0xB8007304
#define set_ISO_PFUNC2_reg(data)   (*((volatile unsigned int*) ISO_PFUNC2_reg)=data)
#define get_ISO_PFUNC2_reg   (*((volatile unsigned int*) ISO_PFUNC2_reg))
#define ISO_PFUNC2_inst_adr                                                          "0x00C1"
#define ISO_PFUNC2_inst                                                              0x00C1
#define ISO_PFUNC2_ur0_tx_smt_shift                                                  (15)
#define ISO_PFUNC2_ur0_tx_smt_mask                                                   (0x00008000)
#define ISO_PFUNC2_ur0_tx_smt(data)                                                  (0x00008000&((data)<<15))
#define ISO_PFUNC2_ur0_tx_smt_src(data)                                              ((0x00008000&(data))>>15)
#define ISO_PFUNC2_get_ur0_tx_smt(data)                                              ((0x00008000&(data))>>15)
#define ISO_PFUNC2_ur0_tx_e2_shift                                                   (14)
#define ISO_PFUNC2_ur0_tx_e2_mask                                                    (0x00004000)
#define ISO_PFUNC2_ur0_tx_e2(data)                                                   (0x00004000&((data)<<14))
#define ISO_PFUNC2_ur0_tx_e2_src(data)                                               ((0x00004000&(data))>>14)
#define ISO_PFUNC2_get_ur0_tx_e2(data)                                               ((0x00004000&(data))>>14)
#define ISO_PFUNC2_ur0_tx_pud_en_shift                                               (13)
#define ISO_PFUNC2_ur0_tx_pud_en_mask                                                (0x00002000)
#define ISO_PFUNC2_ur0_tx_pud_en(data)                                               (0x00002000&((data)<<13))
#define ISO_PFUNC2_ur0_tx_pud_en_src(data)                                           ((0x00002000&(data))>>13)
#define ISO_PFUNC2_get_ur0_tx_pud_en(data)                                           ((0x00002000&(data))>>13)
#define ISO_PFUNC2_ur0_tx_pud_sel_shift                                              (12)
#define ISO_PFUNC2_ur0_tx_pud_sel_mask                                               (0x00001000)
#define ISO_PFUNC2_ur0_tx_pud_sel(data)                                              (0x00001000&((data)<<12))
#define ISO_PFUNC2_ur0_tx_pud_sel_src(data)                                          ((0x00001000&(data))>>12)
#define ISO_PFUNC2_get_ur0_tx_pud_sel(data)                                          ((0x00001000&(data))>>12)
#define ISO_PFUNC2_ur0_rx_smt_shift                                                  (11)
#define ISO_PFUNC2_ur0_rx_smt_mask                                                   (0x00000800)
#define ISO_PFUNC2_ur0_rx_smt(data)                                                  (0x00000800&((data)<<11))
#define ISO_PFUNC2_ur0_rx_smt_src(data)                                              ((0x00000800&(data))>>11)
#define ISO_PFUNC2_get_ur0_rx_smt(data)                                              ((0x00000800&(data))>>11)
#define ISO_PFUNC2_ur0_rx_e2_shift                                                   (10)
#define ISO_PFUNC2_ur0_rx_e2_mask                                                    (0x00000400)
#define ISO_PFUNC2_ur0_rx_e2(data)                                                   (0x00000400&((data)<<10))
#define ISO_PFUNC2_ur0_rx_e2_src(data)                                               ((0x00000400&(data))>>10)
#define ISO_PFUNC2_get_ur0_rx_e2(data)                                               ((0x00000400&(data))>>10)
#define ISO_PFUNC2_ur0_rx_pud_en_shift                                               (9)
#define ISO_PFUNC2_ur0_rx_pud_en_mask                                                (0x00000200)
#define ISO_PFUNC2_ur0_rx_pud_en(data)                                               (0x00000200&((data)<<9))
#define ISO_PFUNC2_ur0_rx_pud_en_src(data)                                           ((0x00000200&(data))>>9)
#define ISO_PFUNC2_get_ur0_rx_pud_en(data)                                           ((0x00000200&(data))>>9)
#define ISO_PFUNC2_ur0_rx_pud_sel_shift                                              (8)
#define ISO_PFUNC2_ur0_rx_pud_sel_mask                                               (0x00000100)
#define ISO_PFUNC2_ur0_rx_pud_sel(data)                                              (0x00000100&((data)<<8))
#define ISO_PFUNC2_ur0_rx_pud_sel_src(data)                                          ((0x00000100&(data))>>8)
#define ISO_PFUNC2_get_ur0_rx_pud_sel(data)                                          ((0x00000100&(data))>>8)
#define ISO_PFUNC2_etn_led_link_smt_shift                                            (7)
#define ISO_PFUNC2_etn_led_link_smt_mask                                             (0x00000080)
#define ISO_PFUNC2_etn_led_link_smt(data)                                            (0x00000080&((data)<<7))
#define ISO_PFUNC2_etn_led_link_smt_src(data)                                        ((0x00000080&(data))>>7)
#define ISO_PFUNC2_get_etn_led_link_smt(data)                                        ((0x00000080&(data))>>7)
#define ISO_PFUNC2_etn_led_link_e2_shift                                             (6)
#define ISO_PFUNC2_etn_led_link_e2_mask                                              (0x00000040)
#define ISO_PFUNC2_etn_led_link_e2(data)                                             (0x00000040&((data)<<6))
#define ISO_PFUNC2_etn_led_link_e2_src(data)                                         ((0x00000040&(data))>>6)
#define ISO_PFUNC2_get_etn_led_link_e2(data)                                         ((0x00000040&(data))>>6)
#define ISO_PFUNC2_etn_led_link_pud_en_shift                                         (5)
#define ISO_PFUNC2_etn_led_link_pud_en_mask                                          (0x00000020)
#define ISO_PFUNC2_etn_led_link_pud_en(data)                                         (0x00000020&((data)<<5))
#define ISO_PFUNC2_etn_led_link_pud_en_src(data)                                     ((0x00000020&(data))>>5)
#define ISO_PFUNC2_get_etn_led_link_pud_en(data)                                     ((0x00000020&(data))>>5)
#define ISO_PFUNC2_etn_led_link_pud_sel_shift                                        (4)
#define ISO_PFUNC2_etn_led_link_pud_sel_mask                                         (0x00000010)
#define ISO_PFUNC2_etn_led_link_pud_sel(data)                                        (0x00000010&((data)<<4))
#define ISO_PFUNC2_etn_led_link_pud_sel_src(data)                                    ((0x00000010&(data))>>4)
#define ISO_PFUNC2_get_etn_led_link_pud_sel(data)                                    ((0x00000010&(data))>>4)
#define ISO_PFUNC2_etn_led_rxtx_smt_shift                                            (3)
#define ISO_PFUNC2_etn_led_rxtx_smt_mask                                             (0x00000008)
#define ISO_PFUNC2_etn_led_rxtx_smt(data)                                            (0x00000008&((data)<<3))
#define ISO_PFUNC2_etn_led_rxtx_smt_src(data)                                        ((0x00000008&(data))>>3)
#define ISO_PFUNC2_get_etn_led_rxtx_smt(data)                                        ((0x00000008&(data))>>3)
#define ISO_PFUNC2_etn_led_rxtx_e2_shift                                             (2)
#define ISO_PFUNC2_etn_led_rxtx_e2_mask                                              (0x00000004)
#define ISO_PFUNC2_etn_led_rxtx_e2(data)                                             (0x00000004&((data)<<2))
#define ISO_PFUNC2_etn_led_rxtx_e2_src(data)                                         ((0x00000004&(data))>>2)
#define ISO_PFUNC2_get_etn_led_rxtx_e2(data)                                         ((0x00000004&(data))>>2)
#define ISO_PFUNC2_etn_led_rxtx_pud_en_shift                                         (1)
#define ISO_PFUNC2_etn_led_rxtx_pud_en_mask                                          (0x00000002)
#define ISO_PFUNC2_etn_led_rxtx_pud_en(data)                                         (0x00000002&((data)<<1))
#define ISO_PFUNC2_etn_led_rxtx_pud_en_src(data)                                     ((0x00000002&(data))>>1)
#define ISO_PFUNC2_get_etn_led_rxtx_pud_en(data)                                     ((0x00000002&(data))>>1)
#define ISO_PFUNC2_etn_led_rxtx_pud_sel_shift                                        (0)
#define ISO_PFUNC2_etn_led_rxtx_pud_sel_mask                                         (0x00000001)
#define ISO_PFUNC2_etn_led_rxtx_pud_sel(data)                                        (0x00000001&((data)<<0))
#define ISO_PFUNC2_etn_led_rxtx_pud_sel_src(data)                                    ((0x00000001&(data))>>0)
#define ISO_PFUNC2_get_etn_led_rxtx_pud_sel(data)                                    ((0x00000001&(data))>>0)


#define ISO_MUXPAD                                                                   0x18007308
#define ISO_MUXPAD_reg_addr                                                          "0xB8007308"
#define ISO_MUXPAD_reg                                                               0xB8007308
#define set_ISO_MUXPAD_reg(data)   (*((volatile unsigned int*) ISO_MUXPAD_reg)=data)
#define get_ISO_MUXPAD_reg   (*((volatile unsigned int*) ISO_MUXPAD_reg))
#define ISO_MUXPAD_inst_adr                                                          "0x00C2"
#define ISO_MUXPAD_inst                                                              0x00C2
#define ISO_MUXPAD_ur0_tx_shift                                                      (18)
#define ISO_MUXPAD_ur0_tx_mask                                                       (0x000C0000)
#define ISO_MUXPAD_ur0_tx(data)                                                      (0x000C0000&((data)<<18))
#define ISO_MUXPAD_ur0_tx_src(data)                                                  ((0x000C0000&(data))>>18)
#define ISO_MUXPAD_get_ur0_tx(data)                                                  ((0x000C0000&(data))>>18)
#define ISO_MUXPAD_ur0_rx_shift                                                      (16)
#define ISO_MUXPAD_ur0_rx_mask                                                       (0x00030000)
#define ISO_MUXPAD_ur0_rx(data)                                                      (0x00030000&((data)<<16))
#define ISO_MUXPAD_ur0_rx_src(data)                                                  ((0x00030000&(data))>>16)
#define ISO_MUXPAD_get_ur0_rx(data)                                                  ((0x00030000&(data))>>16)
#define ISO_MUXPAD_etn_led_link_shift                                                (14)
#define ISO_MUXPAD_etn_led_link_mask                                                 (0x0000C000)
#define ISO_MUXPAD_etn_led_link(data)                                                (0x0000C000&((data)<<14))
#define ISO_MUXPAD_etn_led_link_src(data)                                            ((0x0000C000&(data))>>14)
#define ISO_MUXPAD_get_etn_led_link(data)                                            ((0x0000C000&(data))>>14)
#define ISO_MUXPAD_etn_led_rxtx_shift                                                (12)
#define ISO_MUXPAD_etn_led_rxtx_mask                                                 (0x00003000)
#define ISO_MUXPAD_etn_led_rxtx(data)                                                (0x00003000&((data)<<12))
#define ISO_MUXPAD_etn_led_rxtx_src(data)                                            ((0x00003000&(data))>>12)
#define ISO_MUXPAD_get_etn_led_rxtx(data)                                            ((0x00003000&(data))>>12)
#define ISO_MUXPAD_usb1_shift                                                        (10)
#define ISO_MUXPAD_usb1_mask                                                         (0x00000C00)
#define ISO_MUXPAD_usb1(data)                                                        (0x00000C00&((data)<<10))
#define ISO_MUXPAD_usb1_src(data)                                                    ((0x00000C00&(data))>>10)
#define ISO_MUXPAD_get_usb1(data)                                                    ((0x00000C00&(data))>>10)
#define ISO_MUXPAD_usb0_shift                                                        (8)
#define ISO_MUXPAD_usb0_mask                                                         (0x00000300)
#define ISO_MUXPAD_usb0(data)                                                        (0x00000300&((data)<<8))
#define ISO_MUXPAD_usb0_src(data)                                                    ((0x00000300&(data))>>8)
#define ISO_MUXPAD_get_usb0(data)                                                    ((0x00000300&(data))>>8)
#define ISO_MUXPAD_ir_in_shift                                                       (6)
#define ISO_MUXPAD_ir_in_mask                                                        (0x000000C0)
#define ISO_MUXPAD_ir_in(data)                                                       (0x000000C0&((data)<<6))
#define ISO_MUXPAD_ir_in_src(data)                                                   ((0x000000C0&(data))>>6)
#define ISO_MUXPAD_get_ir_in(data)                                                   ((0x000000C0&(data))>>6)
#define ISO_MUXPAD_vfd_cs_n_shift                                                    (4)
#define ISO_MUXPAD_vfd_cs_n_mask                                                     (0x00000030)
#define ISO_MUXPAD_vfd_cs_n(data)                                                    (0x00000030&((data)<<4))
#define ISO_MUXPAD_vfd_cs_n_src(data)                                                ((0x00000030&(data))>>4)
#define ISO_MUXPAD_get_vfd_cs_n(data)                                                ((0x00000030&(data))>>4)
#define ISO_MUXPAD_vfd_clk_shift                                                     (2)
#define ISO_MUXPAD_vfd_clk_mask                                                      (0x0000000C)
#define ISO_MUXPAD_vfd_clk(data)                                                     (0x0000000C&((data)<<2))
#define ISO_MUXPAD_vfd_clk_src(data)                                                 ((0x0000000C&(data))>>2)
#define ISO_MUXPAD_get_vfd_clk(data)                                                 ((0x0000000C&(data))>>2)
#define ISO_MUXPAD_vfd_d_shift                                                       (0)
#define ISO_MUXPAD_vfd_d_mask                                                        (0x00000003)
#define ISO_MUXPAD_vfd_d(data)                                                       (0x00000003&((data)<<0))
#define ISO_MUXPAD_vfd_d_src(data)                                                   ((0x00000003&(data))>>0)
#define ISO_MUXPAD_get_vfd_d(data)                                                   ((0x00000003&(data))>>0)


#define ISO_IR_PSR                                                                   0x18007400
#define ISO_IR_PSR_reg_addr                                                          "0xB8007400"
#define ISO_IR_PSR_reg                                                               0xB8007400
#define set_ISO_IR_PSR_reg(data)   (*((volatile unsigned int*) ISO_IR_PSR_reg)=data)
#define get_ISO_IR_PSR_reg   (*((volatile unsigned int*) ISO_IR_PSR_reg))
#define ISO_IR_PSR_inst_adr                                                          "0x0000"
#define ISO_IR_PSR_inst                                                              0x0000
#define ISO_IR_PSR_IRRBL_shift                                                       (24)
#define ISO_IR_PSR_IRRBL_mask                                                        (0xFF000000)
#define ISO_IR_PSR_IRRBL(data)                                                       (0xFF000000&((data)<<24))
#define ISO_IR_PSR_IRRBL_src(data)                                                   ((0xFF000000&(data))>>24)
#define ISO_IR_PSR_get_IRRBL(data)                                                   ((0xFF000000&(data))>>24)
#define ISO_IR_PSR_IRRML_shift                                                       (16)
#define ISO_IR_PSR_IRRML_mask                                                        (0x00FF0000)
#define ISO_IR_PSR_IRRML(data)                                                       (0x00FF0000&((data)<<16))
#define ISO_IR_PSR_IRRML_src(data)                                                   ((0x00FF0000&(data))>>16)
#define ISO_IR_PSR_get_IRRML(data)                                                   ((0x00FF0000&(data))>>16)
#define ISO_IR_PSR_IRRDZL_shift                                                      (8)
#define ISO_IR_PSR_IRRDZL_mask                                                       (0x0000FF00)
#define ISO_IR_PSR_IRRDZL(data)                                                      (0x0000FF00&((data)<<8))
#define ISO_IR_PSR_IRRDZL_src(data)                                                  ((0x0000FF00&(data))>>8)
#define ISO_IR_PSR_get_IRRDZL(data)                                                  ((0x0000FF00&(data))>>8)
#define ISO_IR_PSR_IRRDOL_shift                                                      (0)
#define ISO_IR_PSR_IRRDOL_mask                                                       (0x000000FF)
#define ISO_IR_PSR_IRRDOL(data)                                                      (0x000000FF&((data)<<0))
#define ISO_IR_PSR_IRRDOL_src(data)                                                  ((0x000000FF&(data))>>0)
#define ISO_IR_PSR_get_IRRDOL(data)                                                  ((0x000000FF&(data))>>0)


#define ISO_IR_PER                                                                   0x18007404
#define ISO_IR_PER_reg_addr                                                          "0xB8007404"
#define ISO_IR_PER_reg                                                               0xB8007404
#define set_ISO_IR_PER_reg(data)   (*((volatile unsigned int*) ISO_IR_PER_reg)=data)
#define get_ISO_IR_PER_reg   (*((volatile unsigned int*) ISO_IR_PER_reg))
#define ISO_IR_PER_inst_adr                                                          "0x0001"
#define ISO_IR_PER_inst                                                              0x0001
#define ISO_IR_PER_IRRDL_shift                                                       (16)
#define ISO_IR_PER_IRRDL_mask                                                        (0x003F0000)
#define ISO_IR_PER_IRRDL(data)                                                       (0x003F0000&((data)<<16))
#define ISO_IR_PER_IRRDL_src(data)                                                   ((0x003F0000&(data))>>16)
#define ISO_IR_PER_get_IRRDL(data)                                                   ((0x003F0000&(data))>>16)
#define ISO_IR_PER_IRRRL_shift                                                       (8)
#define ISO_IR_PER_IRRRL_mask                                                        (0x0000FF00)
#define ISO_IR_PER_IRRRL(data)                                                       (0x0000FF00&((data)<<8))
#define ISO_IR_PER_IRRRL_src(data)                                                   ((0x0000FF00&(data))>>8)
#define ISO_IR_PER_get_IRRRL(data)                                                   ((0x0000FF00&(data))>>8)
#define ISO_IR_PER_IRRSL_shift                                                       (0)
#define ISO_IR_PER_IRRSL_mask                                                        (0x000000FF)
#define ISO_IR_PER_IRRSL(data)                                                       (0x000000FF&((data)<<0))
#define ISO_IR_PER_IRRSL_src(data)                                                   ((0x000000FF&(data))>>0)
#define ISO_IR_PER_get_IRRSL(data)                                                   ((0x000000FF&(data))>>0)


#define ISO_IR_SF                                                                    0x18007408
#define ISO_IR_SF_reg_addr                                                           "0xB8007408"
#define ISO_IR_SF_reg                                                                0xB8007408
#define set_ISO_IR_SF_reg(data)   (*((volatile unsigned int*) ISO_IR_SF_reg)=data)
#define get_ISO_IR_SF_reg   (*((volatile unsigned int*) ISO_IR_SF_reg))
#define ISO_IR_SF_inst_adr                                                           "0x0002"
#define ISO_IR_SF_inst                                                               0x0002
#define ISO_IR_SF_IRFD_shift                                                         (0)
#define ISO_IR_SF_IRFD_mask                                                          (0x0000FFFF)
#define ISO_IR_SF_IRFD(data)                                                         (0x0000FFFF&((data)<<0))
#define ISO_IR_SF_IRFD_src(data)                                                     ((0x0000FFFF&(data))>>0)
#define ISO_IR_SF_get_IRFD(data)                                                     ((0x0000FFFF&(data))>>0)


#define ISO_IR_DPIR                                                                  0x1800740C
#define ISO_IR_DPIR_reg_addr                                                         "0xB800740C"
#define ISO_IR_DPIR_reg                                                              0xB800740C
#define set_ISO_IR_DPIR_reg(data)   (*((volatile unsigned int*) ISO_IR_DPIR_reg)=data)
#define get_ISO_IR_DPIR_reg   (*((volatile unsigned int*) ISO_IR_DPIR_reg))
#define ISO_IR_DPIR_inst_adr                                                         "0x0003"
#define ISO_IR_DPIR_inst                                                             0x0003
#define ISO_IR_DPIR_IRIOTCDP_shift                                                   (0)
#define ISO_IR_DPIR_IRIOTCDP_mask                                                    (0x0000FFFF)
#define ISO_IR_DPIR_IRIOTCDP(data)                                                   (0x0000FFFF&((data)<<0))
#define ISO_IR_DPIR_IRIOTCDP_src(data)                                               ((0x0000FFFF&(data))>>0)
#define ISO_IR_DPIR_get_IRIOTCDP(data)                                               ((0x0000FFFF&(data))>>0)


#define ISO_IR_CR                                                                    0x18007410
#define ISO_IR_CR_reg_addr                                                           "0xB8007410"
#define ISO_IR_CR_reg                                                                0xB8007410
#define set_ISO_IR_CR_reg(data)   (*((volatile unsigned int*) ISO_IR_CR_reg)=data)
#define get_ISO_IR_CR_reg   (*((volatile unsigned int*) ISO_IR_CR_reg))
#define ISO_IR_CR_inst_adr                                                           "0x0004"
#define ISO_IR_CR_inst                                                               0x0004
#define ISO_IR_CR_IRSR_shift                                                         (31)
#define ISO_IR_CR_IRSR_mask                                                          (0x80000000)
#define ISO_IR_CR_IRSR(data)                                                         (0x80000000&((data)<<31))
#define ISO_IR_CR_IRSR_src(data)                                                     ((0x80000000&(data))>>31)
#define ISO_IR_CR_get_IRSR(data)                                                     ((0x80000000&(data))>>31)
#define ISO_IR_CR_toshiba_en_shift                                                   (24)
#define ISO_IR_CR_toshiba_en_mask                                                    (0x01000000)
#define ISO_IR_CR_toshiba_en(data)                                                   (0x01000000&((data)<<24))
#define ISO_IR_CR_toshiba_en_src(data)                                               ((0x01000000&(data))>>24)
#define ISO_IR_CR_get_toshiba_en(data)                                               ((0x01000000&(data))>>24)
#define ISO_IR_CR_IREDNM_shift                                                       (23)
#define ISO_IR_CR_IREDNM_mask                                                        (0x00800000)
#define ISO_IR_CR_IREDNM(data)                                                       (0x00800000&((data)<<23))
#define ISO_IR_CR_IREDNM_src(data)                                                   ((0x00800000&(data))>>23)
#define ISO_IR_CR_get_IREDNM(data)                                                   ((0x00800000&(data))>>23)
#define ISO_IR_CR_IREDN_shift                                                        (16)
#define ISO_IR_CR_IREDN_mask                                                         (0x003F0000)
#define ISO_IR_CR_IREDN(data)                                                        (0x003F0000&((data)<<16))
#define ISO_IR_CR_IREDN_src(data)                                                    ((0x003F0000&(data))>>16)
#define ISO_IR_CR_get_IREDN(data)                                                    ((0x003F0000&(data))>>16)
#define ISO_IR_CR_raw_fifo_ov_shift                                                  (14)
#define ISO_IR_CR_raw_fifo_ov_mask                                                   (0x00004000)
#define ISO_IR_CR_raw_fifo_ov(data)                                                  (0x00004000&((data)<<14))
#define ISO_IR_CR_raw_fifo_ov_src(data)                                              ((0x00004000&(data))>>14)
#define ISO_IR_CR_get_raw_fifo_ov(data)                                              ((0x00004000&(data))>>14)
#define ISO_IR_CR_raw_fifo_val_shift                                                 (13)
#define ISO_IR_CR_raw_fifo_val_mask                                                  (0x00002000)
#define ISO_IR_CR_raw_fifo_val(data)                                                 (0x00002000&((data)<<13))
#define ISO_IR_CR_raw_fifo_val_src(data)                                             ((0x00002000&(data))>>13)
#define ISO_IR_CR_get_raw_fifo_val(data)                                             ((0x00002000&(data))>>13)
#define ISO_IR_CR_RAW_EN_shift                                                       (12)
#define ISO_IR_CR_RAW_EN_mask                                                        (0x00001000)
#define ISO_IR_CR_RAW_EN(data)                                                       (0x00001000&((data)<<12))
#define ISO_IR_CR_RAW_EN_src(data)                                                   ((0x00001000&(data))>>12)
#define ISO_IR_CR_get_RAW_EN(data)                                                   ((0x00001000&(data))>>12)
#define ISO_IR_CR_MLAE_shift                                                         (11)
#define ISO_IR_CR_MLAE_mask                                                          (0x00000800)
#define ISO_IR_CR_MLAE(data)                                                         (0x00000800&((data)<<11))
#define ISO_IR_CR_MLAE_src(data)                                                     ((0x00000800&(data))>>11)
#define ISO_IR_CR_get_MLAE(data)                                                     ((0x00000800&(data))>>11)
#define ISO_IR_CR_IRIE_shift                                                         (10)
#define ISO_IR_CR_IRIE_mask                                                          (0x00000400)
#define ISO_IR_CR_IRIE(data)                                                         (0x00000400&((data)<<10))
#define ISO_IR_CR_IRIE_src(data)                                                     ((0x00000400&(data))>>10)
#define ISO_IR_CR_get_IRIE(data)                                                     ((0x00000400&(data))>>10)
#define ISO_IR_CR_IRRES_shift                                                        (9)
#define ISO_IR_CR_IRRES_mask                                                         (0x00000200)
#define ISO_IR_CR_IRRES(data)                                                        (0x00000200&((data)<<9))
#define ISO_IR_CR_IRRES_src(data)                                                    ((0x00000200&(data))>>9)
#define ISO_IR_CR_get_IRRES(data)                                                    ((0x00000200&(data))>>9)
#define ISO_IR_CR_IRUE_shift                                                         (8)
#define ISO_IR_CR_IRUE_mask                                                          (0x00000100)
#define ISO_IR_CR_IRUE(data)                                                         (0x00000100&((data)<<8))
#define ISO_IR_CR_IRUE_src(data)                                                     ((0x00000100&(data))>>8)
#define ISO_IR_CR_get_IRUE(data)                                                     ((0x00000100&(data))>>8)
#define ISO_IR_CR_IRCM_shift                                                         (7)
#define ISO_IR_CR_IRCM_mask                                                          (0x00000080)
#define ISO_IR_CR_IRCM(data)                                                         (0x00000080&((data)<<7))
#define ISO_IR_CR_IRCM_src(data)                                                     ((0x00000080&(data))>>7)
#define ISO_IR_CR_get_IRCM(data)                                                     ((0x00000080&(data))>>7)
#define ISO_IR_CR_IRBME_shift                                                        (6)
#define ISO_IR_CR_IRBME_mask                                                         (0x00000040)
#define ISO_IR_CR_IRBME(data)                                                        (0x00000040&((data)<<6))
#define ISO_IR_CR_IRBME_src(data)                                                    ((0x00000040&(data))>>6)
#define ISO_IR_CR_get_IRBME(data)                                                    ((0x00000040&(data))>>6)
#define ISO_IR_CR_IRDPM_shift                                                        (5)
#define ISO_IR_CR_IRDPM_mask                                                         (0x00000020)
#define ISO_IR_CR_IRDPM(data)                                                        (0x00000020&((data)<<5))
#define ISO_IR_CR_IRDPM_src(data)                                                    ((0x00000020&(data))>>5)
#define ISO_IR_CR_get_IRDPM(data)                                                    ((0x00000020&(data))>>5)
#define ISO_IR_CR_IRDN_shift                                                         (0)
#define ISO_IR_CR_IRDN_mask                                                          (0x0000001F)
#define ISO_IR_CR_IRDN(data)                                                         (0x0000001F&((data)<<0))
#define ISO_IR_CR_IRDN_src(data)                                                     ((0x0000001F&(data))>>0)
#define ISO_IR_CR_get_IRDN(data)                                                     ((0x0000001F&(data))>>0)


#define ISO_IR_RP                                                                    0x18007414
#define ISO_IR_RP_reg_addr                                                           "0xB8007414"
#define ISO_IR_RP_reg                                                                0xB8007414
#define set_ISO_IR_RP_reg(data)   (*((volatile unsigned int*) ISO_IR_RP_reg)=data)
#define get_ISO_IR_RP_reg   (*((volatile unsigned int*) ISO_IR_RP_reg))
#define ISO_IR_RP_inst_adr                                                           "0x0005"
#define ISO_IR_RP_inst                                                               0x0005
#define ISO_IR_RP_IRRP_shift                                                         (0)
#define ISO_IR_RP_IRRP_mask                                                          (0xFFFFFFFF)
#define ISO_IR_RP_IRRP(data)                                                         (0xFFFFFFFF&((data)<<0))
#define ISO_IR_RP_IRRP_src(data)                                                     ((0xFFFFFFFF&(data))>>0)
#define ISO_IR_RP_get_IRRP(data)                                                     ((0xFFFFFFFF&(data))>>0)


#define ISO_IR_SR                                                                    0x18007418
#define ISO_IR_SR_reg_addr                                                           "0xB8007418"
#define ISO_IR_SR_reg                                                                0xB8007418
#define set_ISO_IR_SR_reg(data)   (*((volatile unsigned int*) ISO_IR_SR_reg)=data)
#define get_ISO_IR_SR_reg   (*((volatile unsigned int*) ISO_IR_SR_reg))
#define ISO_IR_SR_inst_adr                                                           "0x0006"
#define ISO_IR_SR_inst                                                               0x0006
#define ISO_IR_SR_rmt_big_shift                                                      (4)
#define ISO_IR_SR_rmt_big_mask                                                       (0x00000010)
#define ISO_IR_SR_rmt_big(data)                                                      (0x00000010&((data)<<4))
#define ISO_IR_SR_rmt_big_src(data)                                                  ((0x00000010&(data))>>4)
#define ISO_IR_SR_get_rmt_big(data)                                                  ((0x00000010&(data))>>4)
#define ISO_IR_SR_raw_fifo_ov_shift                                                  (3)
#define ISO_IR_SR_raw_fifo_ov_mask                                                   (0x00000008)
#define ISO_IR_SR_raw_fifo_ov(data)                                                  (0x00000008&((data)<<3))
#define ISO_IR_SR_raw_fifo_ov_src(data)                                              ((0x00000008&(data))>>3)
#define ISO_IR_SR_get_raw_fifo_ov(data)                                              ((0x00000008&(data))>>3)
#define ISO_IR_SR_raw_fifo_val_shift                                                 (2)
#define ISO_IR_SR_raw_fifo_val_mask                                                  (0x00000004)
#define ISO_IR_SR_raw_fifo_val(data)                                                 (0x00000004&((data)<<2))
#define ISO_IR_SR_raw_fifo_val_src(data)                                             ((0x00000004&(data))>>2)
#define ISO_IR_SR_get_raw_fifo_val(data)                                             ((0x00000004&(data))>>2)
#define ISO_IR_SR_IRRF_shift                                                         (1)
#define ISO_IR_SR_IRRF_mask                                                          (0x00000002)
#define ISO_IR_SR_IRRF(data)                                                         (0x00000002&((data)<<1))
#define ISO_IR_SR_IRRF_src(data)                                                     ((0x00000002&(data))>>1)
#define ISO_IR_SR_get_IRRF(data)                                                     ((0x00000002&(data))>>1)
#define ISO_IR_SR_IRDVF_shift                                                        (0)
#define ISO_IR_SR_IRDVF_mask                                                         (0x00000001)
#define ISO_IR_SR_IRDVF(data)                                                        (0x00000001&((data)<<0))
#define ISO_IR_SR_IRDVF_src(data)                                                    ((0x00000001&(data))>>0)
#define ISO_IR_SR_get_IRDVF(data)                                                    ((0x00000001&(data))>>0)


#define ISO_IR_RAW_CTRL                                                              0x1800741C
#define ISO_IR_RAW_CTRL_reg_addr                                                     "0xB800741C"
#define ISO_IR_RAW_CTRL_reg                                                          0xB800741C
#define set_ISO_IR_RAW_CTRL_reg(data)   (*((volatile unsigned int*) ISO_IR_RAW_CTRL_reg)=data)
#define get_ISO_IR_RAW_CTRL_reg   (*((volatile unsigned int*) ISO_IR_RAW_CTRL_reg))
#define ISO_IR_RAW_CTRL_inst_adr                                                     "0x0007"
#define ISO_IR_RAW_CTRL_inst                                                         0x0007
#define ISO_IR_RAW_CTRL_write_en2_shift                                              (25)
#define ISO_IR_RAW_CTRL_write_en2_mask                                               (0x02000000)
#define ISO_IR_RAW_CTRL_write_en2(data)                                              (0x02000000&((data)<<25))
#define ISO_IR_RAW_CTRL_write_en2_src(data)                                          ((0x02000000&(data))>>25)
#define ISO_IR_RAW_CTRL_get_write_en2(data)                                          ((0x02000000&(data))>>25)
#define ISO_IR_RAW_CTRL_stop_sample_shift                                            (24)
#define ISO_IR_RAW_CTRL_stop_sample_mask                                             (0x01000000)
#define ISO_IR_RAW_CTRL_stop_sample(data)                                            (0x01000000&((data)<<24))
#define ISO_IR_RAW_CTRL_stop_sample_src(data)                                        ((0x01000000&(data))>>24)
#define ISO_IR_RAW_CTRL_get_stop_sample(data)                                        ((0x01000000&(data))>>24)
#define ISO_IR_RAW_CTRL_stop_time_shift                                              (8)
#define ISO_IR_RAW_CTRL_stop_time_mask                                               (0x00FFFF00)
#define ISO_IR_RAW_CTRL_stop_time(data)                                              (0x00FFFF00&((data)<<8))
#define ISO_IR_RAW_CTRL_stop_time_src(data)                                          ((0x00FFFF00&(data))>>8)
#define ISO_IR_RAW_CTRL_get_stop_time(data)                                          ((0x00FFFF00&(data))>>8)
#define ISO_IR_RAW_CTRL_write_en1_shift                                              (6)
#define ISO_IR_RAW_CTRL_write_en1_mask                                               (0x00000040)
#define ISO_IR_RAW_CTRL_write_en1(data)                                              (0x00000040&((data)<<6))
#define ISO_IR_RAW_CTRL_write_en1_src(data)                                          ((0x00000040&(data))>>6)
#define ISO_IR_RAW_CTRL_get_write_en1(data)                                          ((0x00000040&(data))>>6)
#define ISO_IR_RAW_CTRL_fifo_thr_shift                                               (0)
#define ISO_IR_RAW_CTRL_fifo_thr_mask                                                (0x0000003F)
#define ISO_IR_RAW_CTRL_fifo_thr(data)                                               (0x0000003F&((data)<<0))
#define ISO_IR_RAW_CTRL_fifo_thr_src(data)                                           ((0x0000003F&(data))>>0)
#define ISO_IR_RAW_CTRL_get_fifo_thr(data)                                           ((0x0000003F&(data))>>0)


#define ISO_IR_RAW_FF                                                                0x18007420
#define ISO_IR_RAW_FF_reg_addr                                                       "0xB8007420"
#define ISO_IR_RAW_FF_reg                                                            0xB8007420
#define set_ISO_IR_RAW_FF_reg(data)   (*((volatile unsigned int*) ISO_IR_RAW_FF_reg)=data)
#define get_ISO_IR_RAW_FF_reg   (*((volatile unsigned int*) ISO_IR_RAW_FF_reg))
#define ISO_IR_RAW_FF_inst_adr                                                       "0x0008"
#define ISO_IR_RAW_FF_inst                                                           0x0008
#define ISO_IR_RAW_FF_DAT_shift                                                      (0)
#define ISO_IR_RAW_FF_DAT_mask                                                       (0xFFFFFFFF)
#define ISO_IR_RAW_FF_DAT(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_IR_RAW_FF_DAT_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_IR_RAW_FF_get_DAT(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_IR_RAW_SAMPLE_TIME                                                       0x18007424
#define ISO_IR_RAW_SAMPLE_TIME_reg_addr                                              "0xB8007424"
#define ISO_IR_RAW_SAMPLE_TIME_reg                                                   0xB8007424
#define set_ISO_IR_RAW_SAMPLE_TIME_reg(data)   (*((volatile unsigned int*) ISO_IR_RAW_SAMPLE_TIME_reg)=data)
#define get_ISO_IR_RAW_SAMPLE_TIME_reg   (*((volatile unsigned int*) ISO_IR_RAW_SAMPLE_TIME_reg))
#define ISO_IR_RAW_SAMPLE_TIME_inst_adr                                              "0x0009"
#define ISO_IR_RAW_SAMPLE_TIME_inst                                                  0x0009
#define ISO_IR_RAW_SAMPLE_TIME_CNT_shift                                             (0)
#define ISO_IR_RAW_SAMPLE_TIME_CNT_mask                                              (0x0000FFFF)
#define ISO_IR_RAW_SAMPLE_TIME_CNT(data)                                             (0x0000FFFF&((data)<<0))
#define ISO_IR_RAW_SAMPLE_TIME_CNT_src(data)                                         ((0x0000FFFF&(data))>>0)
#define ISO_IR_RAW_SAMPLE_TIME_get_CNT(data)                                         ((0x0000FFFF&(data))>>0)


#define ISO_IR_RAW_WL                                                                0x18007428
#define ISO_IR_RAW_WL_reg_addr                                                       "0xB8007428"
#define ISO_IR_RAW_WL_reg                                                            0xB8007428
#define set_ISO_IR_RAW_WL_reg(data)   (*((volatile unsigned int*) ISO_IR_RAW_WL_reg)=data)
#define get_ISO_IR_RAW_WL_reg   (*((volatile unsigned int*) ISO_IR_RAW_WL_reg))
#define ISO_IR_RAW_WL_inst_adr                                                       "0x000A"
#define ISO_IR_RAW_WL_inst                                                           0x000A
#define ISO_IR_RAW_WL_VAL_shift                                                      (0)
#define ISO_IR_RAW_WL_VAL_mask                                                       (0x0000003F)
#define ISO_IR_RAW_WL_VAL(data)                                                      (0x0000003F&((data)<<0))
#define ISO_IR_RAW_WL_VAL_src(data)                                                  ((0x0000003F&(data))>>0)
#define ISO_IR_RAW_WL_get_VAL(data)                                                  ((0x0000003F&(data))>>0)


#define ISO_IR_RAW_DEB                                                               0x1800742C
#define ISO_IR_RAW_DEB_reg_addr                                                      "0xB800742C"
#define ISO_IR_RAW_DEB_reg                                                           0xB800742C
#define set_ISO_IR_RAW_DEB_reg(data)   (*((volatile unsigned int*) ISO_IR_RAW_DEB_reg)=data)
#define get_ISO_IR_RAW_DEB_reg   (*((volatile unsigned int*) ISO_IR_RAW_DEB_reg))
#define ISO_IR_RAW_DEB_inst_adr                                                      "0x000B"
#define ISO_IR_RAW_DEB_inst                                                          0x000B
#define ISO_IR_RAW_DEB_LEN_shift                                                     (16)
#define ISO_IR_RAW_DEB_LEN_mask                                                      (0x00FF0000)
#define ISO_IR_RAW_DEB_LEN(data)                                                     (0x00FF0000&((data)<<16))
#define ISO_IR_RAW_DEB_LEN_src(data)                                                 ((0x00FF0000&(data))>>16)
#define ISO_IR_RAW_DEB_get_LEN(data)                                                 ((0x00FF0000&(data))>>16)
#define ISO_IR_RAW_DEB_CLK_shift                                                     (0)
#define ISO_IR_RAW_DEB_CLK_mask                                                      (0x0000FFFF)
#define ISO_IR_RAW_DEB_CLK(data)                                                     (0x0000FFFF&((data)<<0))
#define ISO_IR_RAW_DEB_CLK_src(data)                                                 ((0x0000FFFF&(data))>>0)
#define ISO_IR_RAW_DEB_get_CLK(data)                                                 ((0x0000FFFF&(data))>>0)


#define ISO_IR_PSR_UP                                                                0x18007430
#define ISO_IR_PSR_UP_reg_addr                                                       "0xB8007430"
#define ISO_IR_PSR_UP_reg                                                            0xB8007430
#define set_ISO_IR_PSR_UP_reg(data)   (*((volatile unsigned int*) ISO_IR_PSR_UP_reg)=data)
#define get_ISO_IR_PSR_UP_reg   (*((volatile unsigned int*) ISO_IR_PSR_UP_reg))
#define ISO_IR_PSR_UP_inst_adr                                                       "0x000C"
#define ISO_IR_PSR_UP_inst                                                           0x000C
#define ISO_IR_PSR_UP_IRRBL_shift                                                    (24)
#define ISO_IR_PSR_UP_IRRBL_mask                                                     (0xFF000000)
#define ISO_IR_PSR_UP_IRRBL(data)                                                    (0xFF000000&((data)<<24))
#define ISO_IR_PSR_UP_IRRBL_src(data)                                                ((0xFF000000&(data))>>24)
#define ISO_IR_PSR_UP_get_IRRBL(data)                                                ((0xFF000000&(data))>>24)
#define ISO_IR_PSR_UP_IRRML_shift                                                    (16)
#define ISO_IR_PSR_UP_IRRML_mask                                                     (0x00FF0000)
#define ISO_IR_PSR_UP_IRRML(data)                                                    (0x00FF0000&((data)<<16))
#define ISO_IR_PSR_UP_IRRML_src(data)                                                ((0x00FF0000&(data))>>16)
#define ISO_IR_PSR_UP_get_IRRML(data)                                                ((0x00FF0000&(data))>>16)
#define ISO_IR_PSR_UP_IRRDZL_shift                                                   (8)
#define ISO_IR_PSR_UP_IRRDZL_mask                                                    (0x0000FF00)
#define ISO_IR_PSR_UP_IRRDZL(data)                                                   (0x0000FF00&((data)<<8))
#define ISO_IR_PSR_UP_IRRDZL_src(data)                                               ((0x0000FF00&(data))>>8)
#define ISO_IR_PSR_UP_get_IRRDZL(data)                                               ((0x0000FF00&(data))>>8)
#define ISO_IR_PSR_UP_IRRDOL_shift                                                   (0)
#define ISO_IR_PSR_UP_IRRDOL_mask                                                    (0x000000FF)
#define ISO_IR_PSR_UP_IRRDOL(data)                                                   (0x000000FF&((data)<<0))
#define ISO_IR_PSR_UP_IRRDOL_src(data)                                               ((0x000000FF&(data))>>0)
#define ISO_IR_PSR_UP_get_IRRDOL(data)                                               ((0x000000FF&(data))>>0)


#define ISO_IR_PER_UP                                                                0x18007434
#define ISO_IR_PER_UP_reg_addr                                                       "0xB8007434"
#define ISO_IR_PER_UP_reg                                                            0xB8007434
#define set_ISO_IR_PER_UP_reg(data)   (*((volatile unsigned int*) ISO_IR_PER_UP_reg)=data)
#define get_ISO_IR_PER_UP_reg   (*((volatile unsigned int*) ISO_IR_PER_UP_reg))
#define ISO_IR_PER_UP_inst_adr                                                       "0x000D"
#define ISO_IR_PER_UP_inst                                                           0x000D
#define ISO_IR_PER_UP_IRRRL_shift                                                    (8)
#define ISO_IR_PER_UP_IRRRL_mask                                                     (0x0000FF00)
#define ISO_IR_PER_UP_IRRRL(data)                                                    (0x0000FF00&((data)<<8))
#define ISO_IR_PER_UP_IRRRL_src(data)                                                ((0x0000FF00&(data))>>8)
#define ISO_IR_PER_UP_get_IRRRL(data)                                                ((0x0000FF00&(data))>>8)
#define ISO_IR_PER_UP_IRRSL_shift                                                    (0)
#define ISO_IR_PER_UP_IRRSL_mask                                                     (0x000000FF)
#define ISO_IR_PER_UP_IRRSL(data)                                                    (0x000000FF&((data)<<0))
#define ISO_IR_PER_UP_IRRSL_src(data)                                                ((0x000000FF&(data))>>0)
#define ISO_IR_PER_UP_get_IRRSL(data)                                                ((0x000000FF&(data))>>0)


#define ISO_IR_CTRL_RC6                                                              0x18007438
#define ISO_IR_CTRL_RC6_reg_addr                                                     "0xB8007438"
#define ISO_IR_CTRL_RC6_reg                                                          0xB8007438
#define set_ISO_IR_CTRL_RC6_reg(data)   (*((volatile unsigned int*) ISO_IR_CTRL_RC6_reg)=data)
#define get_ISO_IR_CTRL_RC6_reg   (*((volatile unsigned int*) ISO_IR_CTRL_RC6_reg))
#define ISO_IR_CTRL_RC6_inst_adr                                                     "0x000E"
#define ISO_IR_CTRL_RC6_inst                                                         0x000E
#define ISO_IR_CTRL_RC6_RC6_EN_shift                                                 (8)
#define ISO_IR_CTRL_RC6_RC6_EN_mask                                                  (0x00000100)
#define ISO_IR_CTRL_RC6_RC6_EN(data)                                                 (0x00000100&((data)<<8))
#define ISO_IR_CTRL_RC6_RC6_EN_src(data)                                             ((0x00000100&(data))>>8)
#define ISO_IR_CTRL_RC6_get_RC6_EN(data)                                             ((0x00000100&(data))>>8)
#define ISO_IR_CTRL_RC6_IRTR_shift                                                   (0)
#define ISO_IR_CTRL_RC6_IRTR_mask                                                    (0x000000FF)
#define ISO_IR_CTRL_RC6_IRTR(data)                                                   (0x000000FF&((data)<<0))
#define ISO_IR_CTRL_RC6_IRTR_src(data)                                               ((0x000000FF&(data))>>0)
#define ISO_IR_CTRL_RC6_get_IRTR(data)                                               ((0x000000FF&(data))>>0)


#define ISO_IR_RP2                                                                   0x1800743C
#define ISO_IR_RP2_reg_addr                                                          "0xB800743C"
#define ISO_IR_RP2_reg                                                               0xB800743C
#define set_ISO_IR_RP2_reg(data)   (*((volatile unsigned int*) ISO_IR_RP2_reg)=data)
#define get_ISO_IR_RP2_reg   (*((volatile unsigned int*) ISO_IR_RP2_reg))
#define ISO_IR_RP2_inst_adr                                                          "0x000F"
#define ISO_IR_RP2_inst                                                              0x000F
#define ISO_IR_RP2_IRRP_shift                                                        (0)
#define ISO_IR_RP2_IRRP_mask                                                         (0xFFFFFFFF)
#define ISO_IR_RP2_IRRP(data)                                                        (0xFFFFFFFF&((data)<<0))
#define ISO_IR_RP2_IRRP_src(data)                                                    ((0xFFFFFFFF&(data))>>0)
#define ISO_IR_RP2_get_IRRP(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define ISO_CEC_CR0                                                                  0x18007500
#define ISO_CEC_CR0_reg_addr                                                         "0xB8007500"
#define ISO_CEC_CR0_reg                                                              0xB8007500
#define set_ISO_CEC_CR0_reg(data)   (*((volatile unsigned int*) ISO_CEC_CR0_reg)=data)
#define get_ISO_CEC_CR0_reg   (*((volatile unsigned int*) ISO_CEC_CR0_reg))
#define ISO_CEC_CR0_inst_adr                                                         "0x0040"
#define ISO_CEC_CR0_inst                                                             0x0040
#define ISO_CEC_CR0_cec_mode_shift                                                   (6)
#define ISO_CEC_CR0_cec_mode_mask                                                    (0x000000C0)
#define ISO_CEC_CR0_cec_mode(data)                                                   (0x000000C0&((data)<<6))
#define ISO_CEC_CR0_cec_mode_src(data)                                               ((0x000000C0&(data))>>6)
#define ISO_CEC_CR0_get_cec_mode(data)                                               ((0x000000C0&(data))>>6)
#define ISO_CEC_CR0_broadcast_addr_shift                                             (5)
#define ISO_CEC_CR0_broadcast_addr_mask                                              (0x00000020)
#define ISO_CEC_CR0_broadcast_addr(data)                                             (0x00000020&((data)<<5))
#define ISO_CEC_CR0_broadcast_addr_src(data)                                         ((0x00000020&(data))>>5)
#define ISO_CEC_CR0_get_broadcast_addr(data)                                         ((0x00000020&(data))>>5)
#define ISO_CEC_CR0_dac_en_shift                                                     (4)
#define ISO_CEC_CR0_dac_en_mask                                                      (0x00000010)
#define ISO_CEC_CR0_dac_en(data)                                                     (0x00000010&((data)<<4))
#define ISO_CEC_CR0_dac_en_src(data)                                                 ((0x00000010&(data))>>4)
#define ISO_CEC_CR0_get_dac_en(data)                                                 ((0x00000010&(data))>>4)
#define ISO_CEC_CR0_logical_addr_shift                                               (0)
#define ISO_CEC_CR0_logical_addr_mask                                                (0x0000000F)
#define ISO_CEC_CR0_logical_addr(data)                                               (0x0000000F&((data)<<0))
#define ISO_CEC_CR0_logical_addr_src(data)                                           ((0x0000000F&(data))>>0)
#define ISO_CEC_CR0_get_logical_addr(data)                                           ((0x0000000F&(data))>>0)


#define ISO_CEC_CR1                                                                  0x18007504
#define ISO_CEC_CR1_reg_addr                                                         "0xB8007504"
#define ISO_CEC_CR1_reg                                                              0xB8007504
#define set_ISO_CEC_CR1_reg(data)   (*((volatile unsigned int*) ISO_CEC_CR1_reg)=data)
#define get_ISO_CEC_CR1_reg   (*((volatile unsigned int*) ISO_CEC_CR1_reg))
#define ISO_CEC_CR1_inst_adr                                                         "0x0041"
#define ISO_CEC_CR1_inst                                                             0x0041
#define ISO_CEC_CR1_timer_div_shift                                                  (0)
#define ISO_CEC_CR1_timer_div_mask                                                   (0x000000FF)
#define ISO_CEC_CR1_timer_div(data)                                                  (0x000000FF&((data)<<0))
#define ISO_CEC_CR1_timer_div_src(data)                                              ((0x000000FF&(data))>>0)
#define ISO_CEC_CR1_get_timer_div(data)                                              ((0x000000FF&(data))>>0)


#define ISO_CEC_CR2                                                                  0x18007508
#define ISO_CEC_CR2_reg_addr                                                         "0xB8007508"
#define ISO_CEC_CR2_reg                                                              0xB8007508
#define set_ISO_CEC_CR2_reg(data)   (*((volatile unsigned int*) ISO_CEC_CR2_reg)=data)
#define get_ISO_CEC_CR2_reg   (*((volatile unsigned int*) ISO_CEC_CR2_reg))
#define ISO_CEC_CR2_inst_adr                                                         "0x0042"
#define ISO_CEC_CR2_inst                                                             0x0042
#define ISO_CEC_CR2_pre_div_shift                                                    (0)
#define ISO_CEC_CR2_pre_div_mask                                                     (0x000000FF)
#define ISO_CEC_CR2_pre_div(data)                                                    (0x000000FF&((data)<<0))
#define ISO_CEC_CR2_pre_div_src(data)                                                ((0x000000FF&(data))>>0)
#define ISO_CEC_CR2_get_pre_div(data)                                                ((0x000000FF&(data))>>0)


#define ISO_CEC_CR3                                                                  0x1800750C
#define ISO_CEC_CR3_reg_addr                                                         "0xB800750C"
#define ISO_CEC_CR3_reg                                                              0xB800750C
#define set_ISO_CEC_CR3_reg(data)   (*((volatile unsigned int*) ISO_CEC_CR3_reg)=data)
#define get_ISO_CEC_CR3_reg   (*((volatile unsigned int*) ISO_CEC_CR3_reg))
#define ISO_CEC_CR3_inst_adr                                                         "0x0043"
#define ISO_CEC_CR3_inst                                                             0x0043
#define ISO_CEC_CR3_unreg_ack_en_shift                                               (7)
#define ISO_CEC_CR3_unreg_ack_en_mask                                                (0x00000080)
#define ISO_CEC_CR3_unreg_ack_en(data)                                               (0x00000080&((data)<<7))
#define ISO_CEC_CR3_unreg_ack_en_src(data)                                           ((0x00000080&(data))>>7)
#define ISO_CEC_CR3_get_unreg_ack_en(data)                                           ((0x00000080&(data))>>7)
#define ISO_CEC_CR3_Cdc_arbitration_en_shift                                         (5)
#define ISO_CEC_CR3_Cdc_arbitration_en_mask                                          (0x00000020)
#define ISO_CEC_CR3_Cdc_arbitration_en(data)                                         (0x00000020&((data)<<5))
#define ISO_CEC_CR3_Cdc_arbitration_en_src(data)                                     ((0x00000020&(data))>>5)
#define ISO_CEC_CR3_get_Cdc_arbitration_en(data)                                     ((0x00000020&(data))>>5)
#define ISO_CEC_CR3_DACDataIn_shift                                                  (0)
#define ISO_CEC_CR3_DACDataIn_mask                                                   (0x0000001F)
#define ISO_CEC_CR3_DACDataIn(data)                                                  (0x0000001F&((data)<<0))
#define ISO_CEC_CR3_DACDataIn_src(data)                                              ((0x0000001F&(data))>>0)
#define ISO_CEC_CR3_get_DACDataIn(data)                                              ((0x0000001F&(data))>>0)


#define ISO_CEC_RT0                                                                  0x18007510
#define ISO_CEC_RT0_reg_addr                                                         "0xB8007510"
#define ISO_CEC_RT0_reg                                                              0xB8007510
#define set_ISO_CEC_RT0_reg(data)   (*((volatile unsigned int*) ISO_CEC_RT0_reg)=data)
#define get_ISO_CEC_RT0_reg   (*((volatile unsigned int*) ISO_CEC_RT0_reg))
#define ISO_CEC_RT0_inst_adr                                                         "0x0044"
#define ISO_CEC_RT0_inst                                                             0x0044
#define ISO_CEC_RT0_cec_pad_in_shift                                                 (6)
#define ISO_CEC_RT0_cec_pad_in_mask                                                  (0x00000040)
#define ISO_CEC_RT0_cec_pad_in(data)                                                 (0x00000040&((data)<<6))
#define ISO_CEC_RT0_cec_pad_in_src(data)                                             ((0x00000040&(data))>>6)
#define ISO_CEC_RT0_get_cec_pad_in(data)                                             ((0x00000040&(data))>>6)
#define ISO_CEC_RT0_wt_cnt_shift                                                     (0)
#define ISO_CEC_RT0_wt_cnt_mask                                                      (0x0000003F)
#define ISO_CEC_RT0_wt_cnt(data)                                                     (0x0000003F&((data)<<0))
#define ISO_CEC_RT0_wt_cnt_src(data)                                                 ((0x0000003F&(data))>>0)
#define ISO_CEC_RT0_get_wt_cnt(data)                                                 ((0x0000003F&(data))>>0)


#define ISO_CEC_RT1                                                                  0x18007514
#define ISO_CEC_RT1_reg_addr                                                         "0xB8007514"
#define ISO_CEC_RT1_reg                                                              0xB8007514
#define set_ISO_CEC_RT1_reg(data)   (*((volatile unsigned int*) ISO_CEC_RT1_reg)=data)
#define get_ISO_CEC_RT1_reg   (*((volatile unsigned int*) ISO_CEC_RT1_reg))
#define ISO_CEC_RT1_inst_adr                                                         "0x0045"
#define ISO_CEC_RT1_inst                                                             0x0045
#define ISO_CEC_RT1_lattest_shift                                                    (4)
#define ISO_CEC_RT1_lattest_mask                                                     (0x00000010)
#define ISO_CEC_RT1_lattest(data)                                                    (0x00000010&((data)<<4))
#define ISO_CEC_RT1_lattest_src(data)                                                ((0x00000010&(data))>>4)
#define ISO_CEC_RT1_get_lattest(data)                                                ((0x00000010&(data))>>4)
#define ISO_CEC_RT1_retry_no_shift                                                   (0)
#define ISO_CEC_RT1_retry_no_mask                                                    (0x0000000F)
#define ISO_CEC_RT1_retry_no(data)                                                   (0x0000000F&((data)<<0))
#define ISO_CEC_RT1_retry_no_src(data)                                               ((0x0000000F&(data))>>0)
#define ISO_CEC_RT1_get_retry_no(data)                                               ((0x0000000F&(data))>>0)


#define ISO_CEC_RX0                                                                  0x18007518
#define ISO_CEC_RX0_reg_addr                                                         "0xB8007518"
#define ISO_CEC_RX0_reg                                                              0xB8007518
#define set_ISO_CEC_RX0_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX0_reg)=data)
#define get_ISO_CEC_RX0_reg   (*((volatile unsigned int*) ISO_CEC_RX0_reg))
#define ISO_CEC_RX0_inst_adr                                                         "0x0046"
#define ISO_CEC_RX0_inst                                                             0x0046
#define ISO_CEC_RX0_rx_en_shift                                                      (7)
#define ISO_CEC_RX0_rx_en_mask                                                       (0x00000080)
#define ISO_CEC_RX0_rx_en(data)                                                      (0x00000080&((data)<<7))
#define ISO_CEC_RX0_rx_en_src(data)                                                  ((0x00000080&(data))>>7)
#define ISO_CEC_RX0_get_rx_en(data)                                                  ((0x00000080&(data))>>7)
#define ISO_CEC_RX0_rx_rst_shift                                                     (6)
#define ISO_CEC_RX0_rx_rst_mask                                                      (0x00000040)
#define ISO_CEC_RX0_rx_rst(data)                                                     (0x00000040&((data)<<6))
#define ISO_CEC_RX0_rx_rst_src(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_RX0_get_rx_rst(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_RX0_rx_continuous_shift                                              (5)
#define ISO_CEC_RX0_rx_continuous_mask                                               (0x00000020)
#define ISO_CEC_RX0_rx_continuous(data)                                              (0x00000020&((data)<<5))
#define ISO_CEC_RX0_rx_continuous_src(data)                                          ((0x00000020&(data))>>5)
#define ISO_CEC_RX0_get_rx_continuous(data)                                          ((0x00000020&(data))>>5)
#define ISO_CEC_RX0_rx_int_en_shift                                                  (4)
#define ISO_CEC_RX0_rx_int_en_mask                                                   (0x00000010)
#define ISO_CEC_RX0_rx_int_en(data)                                                  (0x00000010&((data)<<4))
#define ISO_CEC_RX0_rx_int_en_src(data)                                              ((0x00000010&(data))>>4)
#define ISO_CEC_RX0_get_rx_int_en(data)                                              ((0x00000010&(data))>>4)
#define ISO_CEC_RX0_init_addr_shift                                                  (0)
#define ISO_CEC_RX0_init_addr_mask                                                   (0x0000000F)
#define ISO_CEC_RX0_init_addr(data)                                                  (0x0000000F&((data)<<0))
#define ISO_CEC_RX0_init_addr_src(data)                                              ((0x0000000F&(data))>>0)
#define ISO_CEC_RX0_get_init_addr(data)                                              ((0x0000000F&(data))>>0)


#define ISO_CEC_RX1                                                                  0x1800751C
#define ISO_CEC_RX1_reg_addr                                                         "0xB800751C"
#define ISO_CEC_RX1_reg                                                              0xB800751C
#define set_ISO_CEC_RX1_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX1_reg)=data)
#define get_ISO_CEC_RX1_reg   (*((volatile unsigned int*) ISO_CEC_RX1_reg))
#define ISO_CEC_RX1_inst_adr                                                         "0x0047"
#define ISO_CEC_RX1_inst                                                             0x0047
#define ISO_CEC_RX1_rx_eom_shift                                                     (7)
#define ISO_CEC_RX1_rx_eom_mask                                                      (0x00000080)
#define ISO_CEC_RX1_rx_eom(data)                                                     (0x00000080&((data)<<7))
#define ISO_CEC_RX1_rx_eom_src(data)                                                 ((0x00000080&(data))>>7)
#define ISO_CEC_RX1_get_rx_eom(data)                                                 ((0x00000080&(data))>>7)
#define ISO_CEC_RX1_rx_int_shift                                                     (6)
#define ISO_CEC_RX1_rx_int_mask                                                      (0x00000040)
#define ISO_CEC_RX1_rx_int(data)                                                     (0x00000040&((data)<<6))
#define ISO_CEC_RX1_rx_int_src(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_RX1_get_rx_int(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_RX1_rx_fifo_ov_shift                                                 (5)
#define ISO_CEC_RX1_rx_fifo_ov_mask                                                  (0x00000020)
#define ISO_CEC_RX1_rx_fifo_ov(data)                                                 (0x00000020&((data)<<5))
#define ISO_CEC_RX1_rx_fifo_ov_src(data)                                             ((0x00000020&(data))>>5)
#define ISO_CEC_RX1_get_rx_fifo_ov(data)                                             ((0x00000020&(data))>>5)
#define ISO_CEC_RX1_rx_fifo_cnt_shift                                                (0)
#define ISO_CEC_RX1_rx_fifo_cnt_mask                                                 (0x0000001F)
#define ISO_CEC_RX1_rx_fifo_cnt(data)                                                (0x0000001F&((data)<<0))
#define ISO_CEC_RX1_rx_fifo_cnt_src(data)                                            ((0x0000001F&(data))>>0)
#define ISO_CEC_RX1_get_rx_fifo_cnt(data)                                            ((0x0000001F&(data))>>0)


#define ISO_CEC_TX0                                                                  0x18007520
#define ISO_CEC_TX0_reg_addr                                                         "0xB8007520"
#define ISO_CEC_TX0_reg                                                              0xB8007520
#define set_ISO_CEC_TX0_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX0_reg)=data)
#define get_ISO_CEC_TX0_reg   (*((volatile unsigned int*) ISO_CEC_TX0_reg))
#define ISO_CEC_TX0_inst_adr                                                         "0x0048"
#define ISO_CEC_TX0_inst                                                             0x0048
#define ISO_CEC_TX0_tx_en_shift                                                      (7)
#define ISO_CEC_TX0_tx_en_mask                                                       (0x00000080)
#define ISO_CEC_TX0_tx_en(data)                                                      (0x00000080&((data)<<7))
#define ISO_CEC_TX0_tx_en_src(data)                                                  ((0x00000080&(data))>>7)
#define ISO_CEC_TX0_get_tx_en(data)                                                  ((0x00000080&(data))>>7)
#define ISO_CEC_TX0_tx_rst_shift                                                     (6)
#define ISO_CEC_TX0_tx_rst_mask                                                      (0x00000040)
#define ISO_CEC_TX0_tx_rst(data)                                                     (0x00000040&((data)<<6))
#define ISO_CEC_TX0_tx_rst_src(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_TX0_get_tx_rst(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_TX0_tx_continuous_shift                                              (5)
#define ISO_CEC_TX0_tx_continuous_mask                                               (0x00000020)
#define ISO_CEC_TX0_tx_continuous(data)                                              (0x00000020&((data)<<5))
#define ISO_CEC_TX0_tx_continuous_src(data)                                          ((0x00000020&(data))>>5)
#define ISO_CEC_TX0_get_tx_continuous(data)                                          ((0x00000020&(data))>>5)
#define ISO_CEC_TX0_tx_int_en_shift                                                  (4)
#define ISO_CEC_TX0_tx_int_en_mask                                                   (0x00000010)
#define ISO_CEC_TX0_tx_int_en(data)                                                  (0x00000010&((data)<<4))
#define ISO_CEC_TX0_tx_int_en_src(data)                                              ((0x00000010&(data))>>4)
#define ISO_CEC_TX0_get_tx_int_en(data)                                              ((0x00000010&(data))>>4)
#define ISO_CEC_TX0_dest_addr_shift                                                  (0)
#define ISO_CEC_TX0_dest_addr_mask                                                   (0x0000000F)
#define ISO_CEC_TX0_dest_addr(data)                                                  (0x0000000F&((data)<<0))
#define ISO_CEC_TX0_dest_addr_src(data)                                              ((0x0000000F&(data))>>0)
#define ISO_CEC_TX0_get_dest_addr(data)                                              ((0x0000000F&(data))>>0)


#define ISO_CEC_TX1                                                                  0x18007524
#define ISO_CEC_TX1_reg_addr                                                         "0xB8007524"
#define ISO_CEC_TX1_reg                                                              0xB8007524
#define set_ISO_CEC_TX1_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX1_reg)=data)
#define get_ISO_CEC_TX1_reg   (*((volatile unsigned int*) ISO_CEC_TX1_reg))
#define ISO_CEC_TX1_inst_adr                                                         "0x0049"
#define ISO_CEC_TX1_inst                                                             0x0049
#define ISO_CEC_TX1_tx_eom_shift                                                     (7)
#define ISO_CEC_TX1_tx_eom_mask                                                      (0x00000080)
#define ISO_CEC_TX1_tx_eom(data)                                                     (0x00000080&((data)<<7))
#define ISO_CEC_TX1_tx_eom_src(data)                                                 ((0x00000080&(data))>>7)
#define ISO_CEC_TX1_get_tx_eom(data)                                                 ((0x00000080&(data))>>7)
#define ISO_CEC_TX1_tx_int_shift                                                     (6)
#define ISO_CEC_TX1_tx_int_mask                                                      (0x00000040)
#define ISO_CEC_TX1_tx_int(data)                                                     (0x00000040&((data)<<6))
#define ISO_CEC_TX1_tx_int_src(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_TX1_get_tx_int(data)                                                 ((0x00000040&(data))>>6)
#define ISO_CEC_TX1_tx_fifo_ud_shift                                                 (5)
#define ISO_CEC_TX1_tx_fifo_ud_mask                                                  (0x00000020)
#define ISO_CEC_TX1_tx_fifo_ud(data)                                                 (0x00000020&((data)<<5))
#define ISO_CEC_TX1_tx_fifo_ud_src(data)                                             ((0x00000020&(data))>>5)
#define ISO_CEC_TX1_get_tx_fifo_ud(data)                                             ((0x00000020&(data))>>5)
#define ISO_CEC_TX1_tx_fifo_cnt_shift                                                (0)
#define ISO_CEC_TX1_tx_fifo_cnt_mask                                                 (0x0000001F)
#define ISO_CEC_TX1_tx_fifo_cnt(data)                                                (0x0000001F&((data)<<0))
#define ISO_CEC_TX1_tx_fifo_cnt_src(data)                                            ((0x0000001F&(data))>>0)
#define ISO_CEC_TX1_get_tx_fifo_cnt(data)                                            ((0x0000001F&(data))>>0)


#define ISO_CEC_TX_FIFO                                                              0x18007528
#define ISO_CEC_TX_FIFO_reg_addr                                                     "0xB8007528"
#define ISO_CEC_TX_FIFO_reg                                                          0xB8007528
#define set_ISO_CEC_TX_FIFO_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_FIFO_reg)=data)
#define get_ISO_CEC_TX_FIFO_reg   (*((volatile unsigned int*) ISO_CEC_TX_FIFO_reg))
#define ISO_CEC_TX_FIFO_inst_adr                                                     "0x004A"
#define ISO_CEC_TX_FIFO_inst                                                         0x004A
#define ISO_CEC_TX_FIFO_tx_dat_shift                                                 (0)
#define ISO_CEC_TX_FIFO_tx_dat_mask                                                  (0x000000FF)
#define ISO_CEC_TX_FIFO_tx_dat(data)                                                 (0x000000FF&((data)<<0))
#define ISO_CEC_TX_FIFO_tx_dat_src(data)                                             ((0x000000FF&(data))>>0)
#define ISO_CEC_TX_FIFO_get_tx_dat(data)                                             ((0x000000FF&(data))>>0)


#define ISO_CEC_RX_FIFO                                                              0x1800752C
#define ISO_CEC_RX_FIFO_reg_addr                                                     "0xB800752C"
#define ISO_CEC_RX_FIFO_reg                                                          0xB800752C
#define set_ISO_CEC_RX_FIFO_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX_FIFO_reg)=data)
#define get_ISO_CEC_RX_FIFO_reg   (*((volatile unsigned int*) ISO_CEC_RX_FIFO_reg))
#define ISO_CEC_RX_FIFO_inst_adr                                                     "0x004B"
#define ISO_CEC_RX_FIFO_inst                                                         0x004B
#define ISO_CEC_RX_FIFO_rx_dat_shift                                                 (0)
#define ISO_CEC_RX_FIFO_rx_dat_mask                                                  (0x000000FF)
#define ISO_CEC_RX_FIFO_rx_dat(data)                                                 (0x000000FF&((data)<<0))
#define ISO_CEC_RX_FIFO_rx_dat_src(data)                                             ((0x000000FF&(data))>>0)
#define ISO_CEC_RX_FIFO_get_rx_dat(data)                                             ((0x000000FF&(data))>>0)


#define ISO_CEC_RX_START0                                                            0x18007530
#define ISO_CEC_RX_START0_reg_addr                                                   "0xB8007530"
#define ISO_CEC_RX_START0_reg                                                        0xB8007530
#define set_ISO_CEC_RX_START0_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX_START0_reg)=data)
#define get_ISO_CEC_RX_START0_reg   (*((volatile unsigned int*) ISO_CEC_RX_START0_reg))
#define ISO_CEC_RX_START0_inst_adr                                                   "0x004C"
#define ISO_CEC_RX_START0_inst                                                       0x004C
#define ISO_CEC_RX_START0_low_shift                                                  (0)
#define ISO_CEC_RX_START0_low_mask                                                   (0x000000FF)
#define ISO_CEC_RX_START0_low(data)                                                  (0x000000FF&((data)<<0))
#define ISO_CEC_RX_START0_low_src(data)                                              ((0x000000FF&(data))>>0)
#define ISO_CEC_RX_START0_get_low(data)                                              ((0x000000FF&(data))>>0)


#define ISO_CEC_RX_START1                                                            0x18007534
#define ISO_CEC_RX_START1_reg_addr                                                   "0xB8007534"
#define ISO_CEC_RX_START1_reg                                                        0xB8007534
#define set_ISO_CEC_RX_START1_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX_START1_reg)=data)
#define get_ISO_CEC_RX_START1_reg   (*((volatile unsigned int*) ISO_CEC_RX_START1_reg))
#define ISO_CEC_RX_START1_inst_adr                                                   "0x004D"
#define ISO_CEC_RX_START1_inst                                                       0x004D
#define ISO_CEC_RX_START1_period_shift                                               (0)
#define ISO_CEC_RX_START1_period_mask                                                (0x000000FF)
#define ISO_CEC_RX_START1_period(data)                                               (0x000000FF&((data)<<0))
#define ISO_CEC_RX_START1_period_src(data)                                           ((0x000000FF&(data))>>0)
#define ISO_CEC_RX_START1_get_period(data)                                           ((0x000000FF&(data))>>0)


#define ISO_CEC_RX_DATA0                                                             0x18007538
#define ISO_CEC_RX_DATA0_reg_addr                                                    "0xB8007538"
#define ISO_CEC_RX_DATA0_reg                                                         0xB8007538
#define set_ISO_CEC_RX_DATA0_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX_DATA0_reg)=data)
#define get_ISO_CEC_RX_DATA0_reg   (*((volatile unsigned int*) ISO_CEC_RX_DATA0_reg))
#define ISO_CEC_RX_DATA0_inst_adr                                                    "0x004E"
#define ISO_CEC_RX_DATA0_inst                                                        0x004E
#define ISO_CEC_RX_DATA0_sample_shift                                                (0)
#define ISO_CEC_RX_DATA0_sample_mask                                                 (0x000000FF)
#define ISO_CEC_RX_DATA0_sample(data)                                                (0x000000FF&((data)<<0))
#define ISO_CEC_RX_DATA0_sample_src(data)                                            ((0x000000FF&(data))>>0)
#define ISO_CEC_RX_DATA0_get_sample(data)                                            ((0x000000FF&(data))>>0)


#define ISO_CEC_RX_DATA1                                                             0x1800753C
#define ISO_CEC_RX_DATA1_reg_addr                                                    "0xB800753C"
#define ISO_CEC_RX_DATA1_reg                                                         0xB800753C
#define set_ISO_CEC_RX_DATA1_reg(data)   (*((volatile unsigned int*) ISO_CEC_RX_DATA1_reg)=data)
#define get_ISO_CEC_RX_DATA1_reg   (*((volatile unsigned int*) ISO_CEC_RX_DATA1_reg))
#define ISO_CEC_RX_DATA1_inst_adr                                                    "0x004F"
#define ISO_CEC_RX_DATA1_inst                                                        0x004F
#define ISO_CEC_RX_DATA1_period_shift                                                (0)
#define ISO_CEC_RX_DATA1_period_mask                                                 (0x000000FF)
#define ISO_CEC_RX_DATA1_period(data)                                                (0x000000FF&((data)<<0))
#define ISO_CEC_RX_DATA1_period_src(data)                                            ((0x000000FF&(data))>>0)
#define ISO_CEC_RX_DATA1_get_period(data)                                            ((0x000000FF&(data))>>0)


#define ISO_CEC_TX_START0                                                            0x18007540
#define ISO_CEC_TX_START0_reg_addr                                                   "0xB8007540"
#define ISO_CEC_TX_START0_reg                                                        0xB8007540
#define set_ISO_CEC_TX_START0_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_START0_reg)=data)
#define get_ISO_CEC_TX_START0_reg   (*((volatile unsigned int*) ISO_CEC_TX_START0_reg))
#define ISO_CEC_TX_START0_inst_adr                                                   "0x0050"
#define ISO_CEC_TX_START0_inst                                                       0x0050
#define ISO_CEC_TX_START0_low_shift                                                  (0)
#define ISO_CEC_TX_START0_low_mask                                                   (0x000000FF)
#define ISO_CEC_TX_START0_low(data)                                                  (0x000000FF&((data)<<0))
#define ISO_CEC_TX_START0_low_src(data)                                              ((0x000000FF&(data))>>0)
#define ISO_CEC_TX_START0_get_low(data)                                              ((0x000000FF&(data))>>0)


#define ISO_CEC_TX_START1                                                            0x18007544
#define ISO_CEC_TX_START1_reg_addr                                                   "0xB8007544"
#define ISO_CEC_TX_START1_reg                                                        0xB8007544
#define set_ISO_CEC_TX_START1_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_START1_reg)=data)
#define get_ISO_CEC_TX_START1_reg   (*((volatile unsigned int*) ISO_CEC_TX_START1_reg))
#define ISO_CEC_TX_START1_inst_adr                                                   "0x0051"
#define ISO_CEC_TX_START1_inst                                                       0x0051
#define ISO_CEC_TX_START1_high_shift                                                 (0)
#define ISO_CEC_TX_START1_high_mask                                                  (0x000000FF)
#define ISO_CEC_TX_START1_high(data)                                                 (0x000000FF&((data)<<0))
#define ISO_CEC_TX_START1_high_src(data)                                             ((0x000000FF&(data))>>0)
#define ISO_CEC_TX_START1_get_high(data)                                             ((0x000000FF&(data))>>0)


#define ISO_CEC_TX_DATA0                                                             0x18007548
#define ISO_CEC_TX_DATA0_reg_addr                                                    "0xB8007548"
#define ISO_CEC_TX_DATA0_reg                                                         0xB8007548
#define set_ISO_CEC_TX_DATA0_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_DATA0_reg)=data)
#define get_ISO_CEC_TX_DATA0_reg   (*((volatile unsigned int*) ISO_CEC_TX_DATA0_reg))
#define ISO_CEC_TX_DATA0_inst_adr                                                    "0x0052"
#define ISO_CEC_TX_DATA0_inst                                                        0x0052
#define ISO_CEC_TX_DATA0_low_shift                                                   (0)
#define ISO_CEC_TX_DATA0_low_mask                                                    (0x000000FF)
#define ISO_CEC_TX_DATA0_low(data)                                                   (0x000000FF&((data)<<0))
#define ISO_CEC_TX_DATA0_low_src(data)                                               ((0x000000FF&(data))>>0)
#define ISO_CEC_TX_DATA0_get_low(data)                                               ((0x000000FF&(data))>>0)


#define ISO_CEC_TX_DATA1                                                             0x1800754C
#define ISO_CEC_TX_DATA1_reg_addr                                                    "0xB800754C"
#define ISO_CEC_TX_DATA1_reg                                                         0xB800754C
#define set_ISO_CEC_TX_DATA1_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_DATA1_reg)=data)
#define get_ISO_CEC_TX_DATA1_reg   (*((volatile unsigned int*) ISO_CEC_TX_DATA1_reg))
#define ISO_CEC_TX_DATA1_inst_adr                                                    "0x0053"
#define ISO_CEC_TX_DATA1_inst                                                        0x0053
#define ISO_CEC_TX_DATA1_01_shift                                                    (0)
#define ISO_CEC_TX_DATA1_01_mask                                                     (0x000000FF)
#define ISO_CEC_TX_DATA1_01(data)                                                    (0x000000FF&((data)<<0))
#define ISO_CEC_TX_DATA1_01_src(data)                                                ((0x000000FF&(data))>>0)
#define ISO_CEC_TX_DATA1_get_01(data)                                                ((0x000000FF&(data))>>0)


#define ISO_CEC_TX_DATA2                                                             0x18007550
#define ISO_CEC_TX_DATA2_reg_addr                                                    "0xB8007550"
#define ISO_CEC_TX_DATA2_reg                                                         0xB8007550
#define set_ISO_CEC_TX_DATA2_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_DATA2_reg)=data)
#define get_ISO_CEC_TX_DATA2_reg   (*((volatile unsigned int*) ISO_CEC_TX_DATA2_reg))
#define ISO_CEC_TX_DATA2_inst_adr                                                    "0x0054"
#define ISO_CEC_TX_DATA2_inst                                                        0x0054
#define ISO_CEC_TX_DATA2_high_shift                                                  (0)
#define ISO_CEC_TX_DATA2_high_mask                                                   (0x000000FF)
#define ISO_CEC_TX_DATA2_high(data)                                                  (0x000000FF&((data)<<0))
#define ISO_CEC_TX_DATA2_high_src(data)                                              ((0x000000FF&(data))>>0)
#define ISO_CEC_TX_DATA2_get_high(data)                                              ((0x000000FF&(data))>>0)


#define ISO_CEC_TX_LBK                                                               0x18007554
#define ISO_CEC_TX_LBK_reg_addr                                                      "0xB8007554"
#define ISO_CEC_TX_LBK_reg                                                           0xB8007554
#define set_ISO_CEC_TX_LBK_reg(data)   (*((volatile unsigned int*) ISO_CEC_TX_LBK_reg)=data)
#define get_ISO_CEC_TX_LBK_reg   (*((volatile unsigned int*) ISO_CEC_TX_LBK_reg))
#define ISO_CEC_TX_LBK_inst_adr                                                      "0x0055"
#define ISO_CEC_TX_LBK_inst                                                          0x0055
#define ISO_CEC_TX_LBK_tx_addr_en_shift                                              (4)
#define ISO_CEC_TX_LBK_tx_addr_en_mask                                               (0x00000010)
#define ISO_CEC_TX_LBK_tx_addr_en(data)                                              (0x00000010&((data)<<4))
#define ISO_CEC_TX_LBK_tx_addr_en_src(data)                                          ((0x00000010&(data))>>4)
#define ISO_CEC_TX_LBK_get_tx_addr_en(data)                                          ((0x00000010&(data))>>4)
#define ISO_CEC_TX_LBK_tx_addr_shift                                                 (0)
#define ISO_CEC_TX_LBK_tx_addr_mask                                                  (0x0000000F)
#define ISO_CEC_TX_LBK_tx_addr(data)                                                 (0x0000000F&((data)<<0))
#define ISO_CEC_TX_LBK_tx_addr_src(data)                                             ((0x0000000F&(data))>>0)
#define ISO_CEC_TX_LBK_get_tx_addr(data)                                             ((0x0000000F&(data))>>0)


#define ISO_DRM_ST                                                                   0x18007600
#define ISO_DRM_ST_reg_addr                                                          "0xB8007600"
#define ISO_DRM_ST_reg                                                               0xB8007600
#define set_ISO_DRM_ST_reg(data)   (*((volatile unsigned int*) ISO_DRM_ST_reg)=data)
#define get_ISO_DRM_ST_reg   (*((volatile unsigned int*) ISO_DRM_ST_reg))
#define ISO_DRM_ST_inst_adr                                                          "0x0080"
#define ISO_DRM_ST_inst                                                              0x0080
#define ISO_DRM_ST_SET_shift                                                         (0)
#define ISO_DRM_ST_SET_mask                                                          (0x00000001)
#define ISO_DRM_ST_SET(data)                                                         (0x00000001&((data)<<0))
#define ISO_DRM_ST_SET_src(data)                                                     ((0x00000001&(data))>>0)
#define ISO_DRM_ST_get_SET(data)                                                     ((0x00000001&(data))>>0)


#define ISO_DRM_SECURE_CLK                                                           0x18007604
#define ISO_DRM_SECURE_CLK_reg_addr                                                  "0xB8007604"
#define ISO_DRM_SECURE_CLK_reg                                                       0xB8007604
#define set_ISO_DRM_SECURE_CLK_reg(data)   (*((volatile unsigned int*) ISO_DRM_SECURE_CLK_reg)=data)
#define get_ISO_DRM_SECURE_CLK_reg   (*((volatile unsigned int*) ISO_DRM_SECURE_CLK_reg))
#define ISO_DRM_SECURE_CLK_inst_adr                                                  "0x0081"
#define ISO_DRM_SECURE_CLK_inst                                                      0x0081
#define ISO_DRM_SECURE_CLK_VAL_shift                                                 (0)
#define ISO_DRM_SECURE_CLK_VAL_mask                                                  (0xFFFFFFFF)
#define ISO_DRM_SECURE_CLK_VAL(data)                                                 (0xFFFFFFFF&((data)<<0))
#define ISO_DRM_SECURE_CLK_VAL_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define ISO_DRM_SECURE_CLK_get_VAL(data)                                             ((0xFFFFFFFF&(data))>>0)


#define ISO_DRM_ELAPSED                                                              0x18007608
#define ISO_DRM_ELAPSED_reg_addr                                                     "0xB8007608"
#define ISO_DRM_ELAPSED_reg                                                          0xB8007608
#define set_ISO_DRM_ELAPSED_reg(data)   (*((volatile unsigned int*) ISO_DRM_ELAPSED_reg)=data)
#define get_ISO_DRM_ELAPSED_reg   (*((volatile unsigned int*) ISO_DRM_ELAPSED_reg))
#define ISO_DRM_ELAPSED_inst_adr                                                     "0x0082"
#define ISO_DRM_ELAPSED_inst                                                         0x0082
#define ISO_DRM_ELAPSED_VAL_shift                                                    (0)
#define ISO_DRM_ELAPSED_VAL_mask                                                     (0xFFFFFFFF)
#define ISO_DRM_ELAPSED_VAL(data)                                                    (0xFFFFFFFF&((data)<<0))
#define ISO_DRM_ELAPSED_VAL_src(data)                                                ((0xFFFFFFFF&(data))>>0)
#define ISO_DRM_ELAPSED_get_VAL(data)                                                ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST                                                                 0x1800760C
#define ISO_COLD_RST_reg_addr                                                        "0xB800760C"
#define ISO_COLD_RST_reg                                                             0xB800760C
#define set_ISO_COLD_RST_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST_reg)=data)
#define get_ISO_COLD_RST_reg   (*((volatile unsigned int*) ISO_COLD_RST_reg))
#define ISO_COLD_RST_inst_adr                                                        "0x0083"
#define ISO_COLD_RST_inst                                                            0x0083
#define ISO_COLD_RST_VAL_shift                                                       (0)
#define ISO_COLD_RST_VAL_mask                                                        (0xFFFFFFFF)
#define ISO_COLD_RST_VAL(data)                                                       (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST_VAL_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST_get_VAL(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST1                                                                0x18007610
#define ISO_COLD_RST1_reg_addr                                                       "0xB8007610"
#define ISO_COLD_RST1_reg                                                            0xB8007610
#define set_ISO_COLD_RST1_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST1_reg)=data)
#define get_ISO_COLD_RST1_reg   (*((volatile unsigned int*) ISO_COLD_RST1_reg))
#define ISO_COLD_RST1_inst_adr                                                       "0x0084"
#define ISO_COLD_RST1_inst                                                           0x0084
#define ISO_COLD_RST1_VAL_shift                                                      (0)
#define ISO_COLD_RST1_VAL_mask                                                       (0xFFFFFFFF)
#define ISO_COLD_RST1_VAL(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST1_VAL_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST1_get_VAL(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST2                                                                0x18007614
#define ISO_COLD_RST2_reg_addr                                                       "0xB8007614"
#define ISO_COLD_RST2_reg                                                            0xB8007614
#define set_ISO_COLD_RST2_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST2_reg)=data)
#define get_ISO_COLD_RST2_reg   (*((volatile unsigned int*) ISO_COLD_RST2_reg))
#define ISO_COLD_RST2_inst_adr                                                       "0x0085"
#define ISO_COLD_RST2_inst                                                           0x0085
#define ISO_COLD_RST2_VAL_shift                                                      (0)
#define ISO_COLD_RST2_VAL_mask                                                       (0xFFFFFFFF)
#define ISO_COLD_RST2_VAL(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST2_VAL_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST2_get_VAL(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_DRM_CTRL                                                                 0x18007618
#define ISO_DRM_CTRL_reg_addr                                                        "0xB8007618"
#define ISO_DRM_CTRL_reg                                                             0xB8007618
#define set_ISO_DRM_CTRL_reg(data)   (*((volatile unsigned int*) ISO_DRM_CTRL_reg)=data)
#define get_ISO_DRM_CTRL_reg   (*((volatile unsigned int*) ISO_DRM_CTRL_reg))
#define ISO_DRM_CTRL_inst_adr                                                        "0x0086"
#define ISO_DRM_CTRL_inst                                                            0x0086
#define ISO_DRM_CTRL_clk_div_en_shift                                                (1)
#define ISO_DRM_CTRL_clk_div_en_mask                                                 (0x00000002)
#define ISO_DRM_CTRL_clk_div_en(data)                                                (0x00000002&((data)<<1))
#define ISO_DRM_CTRL_clk_div_en_src(data)                                            ((0x00000002&(data))>>1)
#define ISO_DRM_CTRL_get_clk_div_en(data)                                            ((0x00000002&(data))>>1)
#define ISO_DRM_CTRL_clk_sel_shift                                                   (0)
#define ISO_DRM_CTRL_clk_sel_mask                                                    (0x00000001)
#define ISO_DRM_CTRL_clk_sel(data)                                                   (0x00000001&((data)<<0))
#define ISO_DRM_CTRL_clk_sel_src(data)                                               ((0x00000001&(data))>>0)
#define ISO_DRM_CTRL_get_clk_sel(data)                                               ((0x00000001&(data))>>0)


#define ISO_DRM_CLK_DIV                                                              0x1800761c
#define ISO_DRM_CLK_DIV_reg_addr                                                     "0xB800761C"
#define ISO_DRM_CLK_DIV_reg                                                          0xB800761C
#define set_ISO_DRM_CLK_DIV_reg(data)   (*((volatile unsigned int*) ISO_DRM_CLK_DIV_reg)=data)
#define get_ISO_DRM_CLK_DIV_reg   (*((volatile unsigned int*) ISO_DRM_CLK_DIV_reg))
#define ISO_DRM_CLK_DIV_inst_adr                                                     "0x0087"
#define ISO_DRM_CLK_DIV_inst                                                         0x0087
#define ISO_DRM_CLK_DIV_init_shift                                                   (0)
#define ISO_DRM_CLK_DIV_init_mask                                                    (0xFFFFFFFF)
#define ISO_DRM_CLK_DIV_init(data)                                                   (0xFFFFFFFF&((data)<<0))
#define ISO_DRM_CLK_DIV_init_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define ISO_DRM_CLK_DIV_get_init(data)                                               ((0xFFFFFFFF&(data))>>0)


#define ISO_TC3TVR                                                                   0x18007620
#define ISO_TC3TVR_reg_addr                                                          "0xB8007620"
#define ISO_TC3TVR_reg                                                               0xB8007620
#define set_ISO_TC3TVR_reg(data)   (*((volatile unsigned int*) ISO_TC3TVR_reg)=data)
#define get_ISO_TC3TVR_reg   (*((volatile unsigned int*) ISO_TC3TVR_reg))
#define ISO_TC3TVR_inst_adr                                                          "0x0088"
#define ISO_TC3TVR_inst                                                              0x0088
#define ISO_TC3TVR_TC3TVR_shift                                                      (0)
#define ISO_TC3TVR_TC3TVR_mask                                                       (0xFFFFFFFF)
#define ISO_TC3TVR_TC3TVR(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_TC3TVR_TC3TVR_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_TC3TVR_get_TC3TVR(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_TC3CVR                                                                   0x18007624
#define ISO_TC3CVR_reg_addr                                                          "0xB8007624"
#define ISO_TC3CVR_reg                                                               0xB8007624
#define set_ISO_TC3CVR_reg(data)   (*((volatile unsigned int*) ISO_TC3CVR_reg)=data)
#define get_ISO_TC3CVR_reg   (*((volatile unsigned int*) ISO_TC3CVR_reg))
#define ISO_TC3CVR_inst_adr                                                          "0x0089"
#define ISO_TC3CVR_inst                                                              0x0089
#define ISO_TC3CVR_TC3CVR_shift                                                      (0)
#define ISO_TC3CVR_TC3CVR_mask                                                       (0xFFFFFFFF)
#define ISO_TC3CVR_TC3CVR(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_TC3CVR_TC3CVR_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_TC3CVR_get_TC3CVR(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_TC3CR                                                                    0x18007628
#define ISO_TC3CR_reg_addr                                                           "0xB8007628"
#define ISO_TC3CR_reg                                                                0xB8007628
#define set_ISO_TC3CR_reg(data)   (*((volatile unsigned int*) ISO_TC3CR_reg)=data)
#define get_ISO_TC3CR_reg   (*((volatile unsigned int*) ISO_TC3CR_reg))
#define ISO_TC3CR_inst_adr                                                           "0x008A"
#define ISO_TC3CR_inst                                                               0x008A
#define ISO_TC3CR_TC3En_shift                                                        (31)
#define ISO_TC3CR_TC3En_mask                                                         (0x80000000)
#define ISO_TC3CR_TC3En(data)                                                        (0x80000000&((data)<<31))
#define ISO_TC3CR_TC3En_src(data)                                                    ((0x80000000&(data))>>31)
#define ISO_TC3CR_get_TC3En(data)                                                    ((0x80000000&(data))>>31)
#define ISO_TC3CR_TC3Mode_shift                                                      (30)
#define ISO_TC3CR_TC3Mode_mask                                                       (0x40000000)
#define ISO_TC3CR_TC3Mode(data)                                                      (0x40000000&((data)<<30))
#define ISO_TC3CR_TC3Mode_src(data)                                                  ((0x40000000&(data))>>30)
#define ISO_TC3CR_get_TC3Mode(data)                                                  ((0x40000000&(data))>>30)
#define ISO_TC3CR_TC3Pause_shift                                                     (29)
#define ISO_TC3CR_TC3Pause_mask                                                      (0x20000000)
#define ISO_TC3CR_TC3Pause(data)                                                     (0x20000000&((data)<<29))
#define ISO_TC3CR_TC3Pause_src(data)                                                 ((0x20000000&(data))>>29)
#define ISO_TC3CR_get_TC3Pause(data)                                                 ((0x20000000&(data))>>29)
#define ISO_TC3CR_RvdA_shift                                                         (24)
#define ISO_TC3CR_RvdA_mask                                                          (0x1F000000)
#define ISO_TC3CR_RvdA(data)                                                         (0x1F000000&((data)<<24))
#define ISO_TC3CR_RvdA_src(data)                                                     ((0x1F000000&(data))>>24)
#define ISO_TC3CR_get_RvdA(data)                                                     ((0x1F000000&(data))>>24)


#define ISO_TC3ICR                                                                   0x1800762C
#define ISO_TC3ICR_reg_addr                                                          "0xB800762C"
#define ISO_TC3ICR_reg                                                               0xB800762C
#define set_ISO_TC3ICR_reg(data)   (*((volatile unsigned int*) ISO_TC3ICR_reg)=data)
#define get_ISO_TC3ICR_reg   (*((volatile unsigned int*) ISO_TC3ICR_reg))
#define ISO_TC3ICR_inst_adr                                                          "0x008B"
#define ISO_TC3ICR_inst                                                              0x008B
#define ISO_TC3ICR_TC3IE_shift                                                       (31)
#define ISO_TC3ICR_TC3IE_mask                                                        (0x80000000)
#define ISO_TC3ICR_TC3IE(data)                                                       (0x80000000&((data)<<31))
#define ISO_TC3ICR_TC3IE_src(data)                                                   ((0x80000000&(data))>>31)
#define ISO_TC3ICR_get_TC3IE(data)                                                   ((0x80000000&(data))>>31)


#define ISO_NORST_0                                                                  0x18007640
#define ISO_NORST_1                                                                  0x18007644
#define ISO_NORST_2                                                                  0x18007648
#define ISO_NORST_3                                                                  0x1800764C
#define ISO_NORST_4                                                                  0x18007650
#define ISO_NORST_5                                                                  0x18007654
#define ISO_NORST_6                                                                  0x18007658
#define ISO_NORST_7                                                                  0x1800765C
#define ISO_NORST_0_reg_addr                                                         "0xB8007640"
#define ISO_NORST_1_reg_addr                                                         "0xB8007644"
#define ISO_NORST_2_reg_addr                                                         "0xB8007648"
#define ISO_NORST_3_reg_addr                                                         "0xB800764C"
#define ISO_NORST_4_reg_addr                                                         "0xB8007650"
#define ISO_NORST_5_reg_addr                                                         "0xB8007654"
#define ISO_NORST_6_reg_addr                                                         "0xB8007658"
#define ISO_NORST_7_reg_addr                                                         "0xB800765C"
#define ISO_NORST_0_reg                                                              0xB8007640
#define ISO_NORST_1_reg                                                              0xB8007644
#define ISO_NORST_2_reg                                                              0xB8007648
#define ISO_NORST_3_reg                                                              0xB800764C
#define ISO_NORST_4_reg                                                              0xB8007650
#define ISO_NORST_5_reg                                                              0xB8007654
#define ISO_NORST_6_reg                                                              0xB8007658
#define ISO_NORST_7_reg                                                              0xB800765C
#define set_ISO_NORST_0_reg(data)   (*((volatile unsigned int*) ISO_NORST_0_reg)=data)
#define set_ISO_NORST_1_reg(data)   (*((volatile unsigned int*) ISO_NORST_1_reg)=data)
#define set_ISO_NORST_2_reg(data)   (*((volatile unsigned int*) ISO_NORST_2_reg)=data)
#define set_ISO_NORST_3_reg(data)   (*((volatile unsigned int*) ISO_NORST_3_reg)=data)
#define set_ISO_NORST_4_reg(data)   (*((volatile unsigned int*) ISO_NORST_4_reg)=data)
#define set_ISO_NORST_5_reg(data)   (*((volatile unsigned int*) ISO_NORST_5_reg)=data)
#define set_ISO_NORST_6_reg(data)   (*((volatile unsigned int*) ISO_NORST_6_reg)=data)
#define set_ISO_NORST_7_reg(data)   (*((volatile unsigned int*) ISO_NORST_7_reg)=data)
#define get_ISO_NORST_0_reg   (*((volatile unsigned int*) ISO_NORST_0_reg))
#define get_ISO_NORST_1_reg   (*((volatile unsigned int*) ISO_NORST_1_reg))
#define get_ISO_NORST_2_reg   (*((volatile unsigned int*) ISO_NORST_2_reg))
#define get_ISO_NORST_3_reg   (*((volatile unsigned int*) ISO_NORST_3_reg))
#define get_ISO_NORST_4_reg   (*((volatile unsigned int*) ISO_NORST_4_reg))
#define get_ISO_NORST_5_reg   (*((volatile unsigned int*) ISO_NORST_5_reg))
#define get_ISO_NORST_6_reg   (*((volatile unsigned int*) ISO_NORST_6_reg))
#define get_ISO_NORST_7_reg   (*((volatile unsigned int*) ISO_NORST_7_reg))
#define ISO_NORST_0_inst_adr                                                         "0x0090"
#define ISO_NORST_1_inst_adr                                                         "0x0091"
#define ISO_NORST_2_inst_adr                                                         "0x0092"
#define ISO_NORST_3_inst_adr                                                         "0x0093"
#define ISO_NORST_4_inst_adr                                                         "0x0094"
#define ISO_NORST_5_inst_adr                                                         "0x0095"
#define ISO_NORST_6_inst_adr                                                         "0x0096"
#define ISO_NORST_7_inst_adr                                                         "0x0097"
#define ISO_NORST_0_inst                                                             0x0090
#define ISO_NORST_1_inst                                                             0x0091
#define ISO_NORST_2_inst                                                             0x0092
#define ISO_NORST_3_inst                                                             0x0093
#define ISO_NORST_4_inst                                                             0x0094
#define ISO_NORST_5_inst                                                             0x0095
#define ISO_NORST_6_inst                                                             0x0096
#define ISO_NORST_7_inst                                                             0x0097
#define ISO_NORST_dat_shift                                                          (0)
#define ISO_NORST_dat_mask                                                           (0xFFFFFFFF)
#define ISO_NORST_dat(data)                                                          (0xFFFFFFFF&((data)<<0))
#define ISO_NORST_dat_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define ISO_NORST_get_dat(data)                                                      ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST3                                                                0x18007660
#define ISO_COLD_RST3_reg_addr                                                       "0xB8007660"
#define ISO_COLD_RST3_reg                                                            0xB8007660
#define set_ISO_COLD_RST3_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST3_reg)=data)
#define get_ISO_COLD_RST3_reg   (*((volatile unsigned int*) ISO_COLD_RST3_reg))
#define ISO_COLD_RST3_inst_adr                                                       "0x0098"
#define ISO_COLD_RST3_inst                                                           0x0098
#define ISO_COLD_RST3_VAL_shift                                                      (0)
#define ISO_COLD_RST3_VAL_mask                                                       (0xFFFFFFFF)
#define ISO_COLD_RST3_VAL(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST3_VAL_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST3_get_VAL(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST4                                                                0x18007664
#define ISO_COLD_RST4_reg_addr                                                       "0xB8007664"
#define ISO_COLD_RST4_reg                                                            0xB8007664
#define set_ISO_COLD_RST4_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST4_reg)=data)
#define get_ISO_COLD_RST4_reg   (*((volatile unsigned int*) ISO_COLD_RST4_reg))
#define ISO_COLD_RST4_inst_adr                                                       "0x0099"
#define ISO_COLD_RST4_inst                                                           0x0099
#define ISO_COLD_RST4_VAL_shift                                                      (0)
#define ISO_COLD_RST4_VAL_mask                                                       (0xFFFFFFFF)
#define ISO_COLD_RST4_VAL(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST4_VAL_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST4_get_VAL(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST5                                                                0x18007668
#define ISO_COLD_RST5_reg_addr                                                       "0xB8007668"
#define ISO_COLD_RST5_reg                                                            0xB8007668
#define set_ISO_COLD_RST5_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST5_reg)=data)
#define get_ISO_COLD_RST5_reg   (*((volatile unsigned int*) ISO_COLD_RST5_reg))
#define ISO_COLD_RST5_inst_adr                                                       "0x009A"
#define ISO_COLD_RST5_inst                                                           0x009A
#define ISO_COLD_RST5_VAL_shift                                                      (0)
#define ISO_COLD_RST5_VAL_mask                                                       (0xFFFFFFFF)
#define ISO_COLD_RST5_VAL(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST5_VAL_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST5_get_VAL(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_COLD_RST6                                                                0x1800766C
#define ISO_COLD_RST6_reg_addr                                                       "0xB800766C"
#define ISO_COLD_RST6_reg                                                            0xB800766C
#define set_ISO_COLD_RST6_reg(data)   (*((volatile unsigned int*) ISO_COLD_RST6_reg)=data)
#define get_ISO_COLD_RST6_reg   (*((volatile unsigned int*) ISO_COLD_RST6_reg))
#define ISO_COLD_RST6_inst_adr                                                       "0x009B"
#define ISO_COLD_RST6_inst                                                           0x009B
#define ISO_COLD_RST6_VAL_shift                                                      (0)
#define ISO_COLD_RST6_VAL_mask                                                       (0xFFFFFFFF)
#define ISO_COLD_RST6_VAL(data)                                                      (0xFFFFFFFF&((data)<<0))
#define ISO_COLD_RST6_VAL_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define ISO_COLD_RST6_get_VAL(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define ISO_KCPU_SRAM_CTRL                                                           0x18007700
#define ISO_KCPU_SRAM_CTRL_reg_addr                                                  "0xB8007700"
#define ISO_KCPU_SRAM_CTRL_reg                                                       0xB8007700
#define set_ISO_KCPU_SRAM_CTRL_reg(data)   (*((volatile unsigned int*) ISO_KCPU_SRAM_CTRL_reg)=data)
#define get_ISO_KCPU_SRAM_CTRL_reg   (*((volatile unsigned int*) ISO_KCPU_SRAM_CTRL_reg))
#define ISO_KCPU_SRAM_CTRL_inst_adr                                                  "0x00C0"
#define ISO_KCPU_SRAM_CTRL_inst                                                      0x00C0
#define ISO_KCPU_SRAM_CTRL_dvse_300_shift                                            (4)
#define ISO_KCPU_SRAM_CTRL_dvse_300_mask                                             (0x00000010)
#define ISO_KCPU_SRAM_CTRL_dvse_300(data)                                            (0x00000010&((data)<<4))
#define ISO_KCPU_SRAM_CTRL_dvse_300_src(data)                                        ((0x00000010&(data))>>4)
#define ISO_KCPU_SRAM_CTRL_get_dvse_300(data)                                        ((0x00000010&(data))>>4)
#define ISO_KCPU_SRAM_CTRL_dvs_300_shift                                             (0)
#define ISO_KCPU_SRAM_CTRL_dvs_300_mask                                              (0x0000000F)
#define ISO_KCPU_SRAM_CTRL_dvs_300(data)                                             (0x0000000F&((data)<<0))
#define ISO_KCPU_SRAM_CTRL_dvs_300_src(data)                                         ((0x0000000F&(data))>>0)
#define ISO_KCPU_SRAM_CTRL_get_dvs_300(data)                                         ((0x0000000F&(data))>>0)


#define ISO_KCPU_BIST_CTRL                                                           0x18007704
#define ISO_KCPU_BIST_CTRL_reg_addr                                                  "0xB8007704"
#define ISO_KCPU_BIST_CTRL_reg                                                       0xB8007704
#define set_ISO_KCPU_BIST_CTRL_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BIST_CTRL_reg)=data)
#define get_ISO_KCPU_BIST_CTRL_reg   (*((volatile unsigned int*) ISO_KCPU_BIST_CTRL_reg))
#define ISO_KCPU_BIST_CTRL_inst_adr                                                  "0x00C1"
#define ISO_KCPU_BIST_CTRL_inst                                                      0x00C1
#define ISO_KCPU_BIST_CTRL_resume_cycle_sel_shift                                    (5)
#define ISO_KCPU_BIST_CTRL_resume_cycle_sel_mask                                     (0x00000020)
#define ISO_KCPU_BIST_CTRL_resume_cycle_sel(data)                                    (0x00000020&((data)<<5))
#define ISO_KCPU_BIST_CTRL_resume_cycle_sel_src(data)                                ((0x00000020&(data))>>5)
#define ISO_KCPU_BIST_CTRL_get_resume_cycle_sel(data)                                ((0x00000020&(data))>>5)
#define ISO_KCPU_BIST_CTRL_kcpu_bist1_en_shift                                       (4)
#define ISO_KCPU_BIST_CTRL_kcpu_bist1_en_mask                                        (0x00000010)
#define ISO_KCPU_BIST_CTRL_kcpu_bist1_en(data)                                       (0x00000010&((data)<<4))
#define ISO_KCPU_BIST_CTRL_kcpu_bist1_en_src(data)                                   ((0x00000010&(data))>>4)
#define ISO_KCPU_BIST_CTRL_get_kcpu_bist1_en(data)                                   ((0x00000010&(data))>>4)
#define ISO_KCPU_BIST_CTRL_kcpu_drf1_bist_en_shift                                   (3)
#define ISO_KCPU_BIST_CTRL_kcpu_drf1_bist_en_mask                                    (0x00000008)
#define ISO_KCPU_BIST_CTRL_kcpu_drf1_bist_en(data)                                   (0x00000008&((data)<<3))
#define ISO_KCPU_BIST_CTRL_kcpu_drf1_bist_en_src(data)                               ((0x00000008&(data))>>3)
#define ISO_KCPU_BIST_CTRL_get_kcpu_drf1_bist_en(data)                               ((0x00000008&(data))>>3)
#define ISO_KCPU_BIST_CTRL_kcpu_bist0_en_shift                                       (2)
#define ISO_KCPU_BIST_CTRL_kcpu_bist0_en_mask                                        (0x00000004)
#define ISO_KCPU_BIST_CTRL_kcpu_bist0_en(data)                                       (0x00000004&((data)<<2))
#define ISO_KCPU_BIST_CTRL_kcpu_bist0_en_src(data)                                   ((0x00000004&(data))>>2)
#define ISO_KCPU_BIST_CTRL_get_kcpu_bist0_en(data)                                   ((0x00000004&(data))>>2)
#define ISO_KCPU_BIST_CTRL_kcpu_drf0_bist_en_shift                                   (1)
#define ISO_KCPU_BIST_CTRL_kcpu_drf0_bist_en_mask                                    (0x00000002)
#define ISO_KCPU_BIST_CTRL_kcpu_drf0_bist_en(data)                                   (0x00000002&((data)<<1))
#define ISO_KCPU_BIST_CTRL_kcpu_drf0_bist_en_src(data)                               ((0x00000002&(data))>>1)
#define ISO_KCPU_BIST_CTRL_get_kcpu_drf0_bist_en(data)                               ((0x00000002&(data))>>1)
#define ISO_KCPU_BIST_CTRL_write_data_shift                                          (0)
#define ISO_KCPU_BIST_CTRL_write_data_mask                                           (0x00000001)
#define ISO_KCPU_BIST_CTRL_write_data(data)                                          (0x00000001&((data)<<0))
#define ISO_KCPU_BIST_CTRL_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define ISO_KCPU_BIST_CTRL_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define ISO_KCPU_BIST0_ST                                                            0x18007708
#define ISO_KCPU_BIST0_ST_reg_addr                                                   "0xB8007708"
#define ISO_KCPU_BIST0_ST_reg                                                        0xB8007708
#define set_ISO_KCPU_BIST0_ST_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BIST0_ST_reg)=data)
#define get_ISO_KCPU_BIST0_ST_reg   (*((volatile unsigned int*) ISO_KCPU_BIST0_ST_reg))
#define ISO_KCPU_BIST0_ST_inst_adr                                                   "0x00C2"
#define ISO_KCPU_BIST0_ST_inst                                                       0x00C2
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail3_shift                                     (4)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail3_mask                                      (0x00000010)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail3(data)                                     (0x00000010&((data)<<4))
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail3_src(data)                                 ((0x00000010&(data))>>4)
#define ISO_KCPU_BIST0_ST_get_kcpu_bist0_fail3(data)                                 ((0x00000010&(data))>>4)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail2_shift                                     (3)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail2_mask                                      (0x00000008)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail2(data)                                     (0x00000008&((data)<<3))
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail2_src(data)                                 ((0x00000008&(data))>>3)
#define ISO_KCPU_BIST0_ST_get_kcpu_bist0_fail2(data)                                 ((0x00000008&(data))>>3)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail1_shift                                     (2)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail1_mask                                      (0x00000004)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail1(data)                                     (0x00000004&((data)<<2))
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail1_src(data)                                 ((0x00000004&(data))>>2)
#define ISO_KCPU_BIST0_ST_get_kcpu_bist0_fail1(data)                                 ((0x00000004&(data))>>2)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail0_shift                                     (1)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail0_mask                                      (0x00000002)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail0(data)                                     (0x00000002&((data)<<1))
#define ISO_KCPU_BIST0_ST_kcpu_bist0_fail0_src(data)                                 ((0x00000002&(data))>>1)
#define ISO_KCPU_BIST0_ST_get_kcpu_bist0_fail0(data)                                 ((0x00000002&(data))>>1)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_done_shift                                      (0)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_done_mask                                       (0x00000001)
#define ISO_KCPU_BIST0_ST_kcpu_bist0_done(data)                                      (0x00000001&((data)<<0))
#define ISO_KCPU_BIST0_ST_kcpu_bist0_done_src(data)                                  ((0x00000001&(data))>>0)
#define ISO_KCPU_BIST0_ST_get_kcpu_bist0_done(data)                                  ((0x00000001&(data))>>0)


#define ISO_KCPU_BIST1_ST                                                            0x1800770C
#define ISO_KCPU_BIST1_ST_reg_addr                                                   "0xB800770C"
#define ISO_KCPU_BIST1_ST_reg                                                        0xB800770C
#define set_ISO_KCPU_BIST1_ST_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BIST1_ST_reg)=data)
#define get_ISO_KCPU_BIST1_ST_reg   (*((volatile unsigned int*) ISO_KCPU_BIST1_ST_reg))
#define ISO_KCPU_BIST1_ST_inst_adr                                                   "0x00C3"
#define ISO_KCPU_BIST1_ST_inst                                                       0x00C3
#define ISO_KCPU_BIST1_ST_kcpu_bist1_fail0_shift                                     (1)
#define ISO_KCPU_BIST1_ST_kcpu_bist1_fail0_mask                                      (0x00000002)
#define ISO_KCPU_BIST1_ST_kcpu_bist1_fail0(data)                                     (0x00000002&((data)<<1))
#define ISO_KCPU_BIST1_ST_kcpu_bist1_fail0_src(data)                                 ((0x00000002&(data))>>1)
#define ISO_KCPU_BIST1_ST_get_kcpu_bist1_fail0(data)                                 ((0x00000002&(data))>>1)
#define ISO_KCPU_BIST1_ST_kcpu_bist1_done_shift                                      (0)
#define ISO_KCPU_BIST1_ST_kcpu_bist1_done_mask                                       (0x00000001)
#define ISO_KCPU_BIST1_ST_kcpu_bist1_done(data)                                      (0x00000001&((data)<<0))
#define ISO_KCPU_BIST1_ST_kcpu_bist1_done_src(data)                                  ((0x00000001&(data))>>0)
#define ISO_KCPU_BIST1_ST_get_kcpu_bist1_done(data)                                  ((0x00000001&(data))>>0)


#define ISO_DRF_CTRL                                                                 0x18007710
#define ISO_DRF_CTRL_reg_addr                                                        "0xB8007710"
#define ISO_DRF_CTRL_reg                                                             0xB8007710
#define set_ISO_DRF_CTRL_reg(data)   (*((volatile unsigned int*) ISO_DRF_CTRL_reg)=data)
#define get_ISO_DRF_CTRL_reg   (*((volatile unsigned int*) ISO_DRF_CTRL_reg))
#define ISO_DRF_CTRL_inst_adr                                                        "0x00C4"
#define ISO_DRF_CTRL_inst                                                            0x00C4
#define ISO_DRF_CTRL_kcpu_drf1_start_pause_shift                                     (3)
#define ISO_DRF_CTRL_kcpu_drf1_start_pause_mask                                      (0x00000008)
#define ISO_DRF_CTRL_kcpu_drf1_start_pause(data)                                     (0x00000008&((data)<<3))
#define ISO_DRF_CTRL_kcpu_drf1_start_pause_src(data)                                 ((0x00000008&(data))>>3)
#define ISO_DRF_CTRL_get_kcpu_drf1_start_pause(data)                                 ((0x00000008&(data))>>3)
#define ISO_DRF_CTRL_kcpu_drf1_test_resume_shift                                     (2)
#define ISO_DRF_CTRL_kcpu_drf1_test_resume_mask                                      (0x00000004)
#define ISO_DRF_CTRL_kcpu_drf1_test_resume(data)                                     (0x00000004&((data)<<2))
#define ISO_DRF_CTRL_kcpu_drf1_test_resume_src(data)                                 ((0x00000004&(data))>>2)
#define ISO_DRF_CTRL_get_kcpu_drf1_test_resume(data)                                 ((0x00000004&(data))>>2)
#define ISO_DRF_CTRL_kcpu_drf0_start_pause_shift                                     (1)
#define ISO_DRF_CTRL_kcpu_drf0_start_pause_mask                                      (0x00000002)
#define ISO_DRF_CTRL_kcpu_drf0_start_pause(data)                                     (0x00000002&((data)<<1))
#define ISO_DRF_CTRL_kcpu_drf0_start_pause_src(data)                                 ((0x00000002&(data))>>1)
#define ISO_DRF_CTRL_get_kcpu_drf0_start_pause(data)                                 ((0x00000002&(data))>>1)
#define ISO_DRF_CTRL_kcpu_drf0_test_resume_shift                                     (0)
#define ISO_DRF_CTRL_kcpu_drf0_test_resume_mask                                      (0x00000001)
#define ISO_DRF_CTRL_kcpu_drf0_test_resume(data)                                     (0x00000001&((data)<<0))
#define ISO_DRF_CTRL_kcpu_drf0_test_resume_src(data)                                 ((0x00000001&(data))>>0)
#define ISO_DRF_CTRL_get_kcpu_drf0_test_resume(data)                                 ((0x00000001&(data))>>0)


#define ISO_KCPU_DRF0_BIST_ST                                                        0x18007714
#define ISO_KCPU_DRF0_BIST_ST_reg_addr                                               "0xB8007714"
#define ISO_KCPU_DRF0_BIST_ST_reg                                                    0xB8007714
#define set_ISO_KCPU_DRF0_BIST_ST_reg(data)   (*((volatile unsigned int*) ISO_KCPU_DRF0_BIST_ST_reg)=data)
#define get_ISO_KCPU_DRF0_BIST_ST_reg   (*((volatile unsigned int*) ISO_KCPU_DRF0_BIST_ST_reg))
#define ISO_KCPU_DRF0_BIST_ST_inst_adr                                               "0x00C5"
#define ISO_KCPU_DRF0_BIST_ST_inst                                                   0x00C5
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail3_shift                             (4)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail3_mask                              (0x00000010)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail3(data)                             (0x00000010&((data)<<4))
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail3_src(data)                         ((0x00000010&(data))>>4)
#define ISO_KCPU_DRF0_BIST_ST_get_kcpu_drf0_bist_fail3(data)                         ((0x00000010&(data))>>4)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail2_shift                             (3)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail2_mask                              (0x00000008)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail2(data)                             (0x00000008&((data)<<3))
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail2_src(data)                         ((0x00000008&(data))>>3)
#define ISO_KCPU_DRF0_BIST_ST_get_kcpu_drf0_bist_fail2(data)                         ((0x00000008&(data))>>3)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail1_shift                             (2)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail1_mask                              (0x00000004)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail1(data)                             (0x00000004&((data)<<2))
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail1_src(data)                         ((0x00000004&(data))>>2)
#define ISO_KCPU_DRF0_BIST_ST_get_kcpu_drf0_bist_fail1(data)                         ((0x00000004&(data))>>2)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail0_shift                             (1)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail0_mask                              (0x00000002)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail0(data)                             (0x00000002&((data)<<1))
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_fail0_src(data)                         ((0x00000002&(data))>>1)
#define ISO_KCPU_DRF0_BIST_ST_get_kcpu_drf0_bist_fail0(data)                         ((0x00000002&(data))>>1)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_done_shift                              (0)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_done_mask                               (0x00000001)
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_done(data)                              (0x00000001&((data)<<0))
#define ISO_KCPU_DRF0_BIST_ST_kcpu_drf0_bist_done_src(data)                          ((0x00000001&(data))>>0)
#define ISO_KCPU_DRF0_BIST_ST_get_kcpu_drf0_bist_done(data)                          ((0x00000001&(data))>>0)


#define ISO_KCPU_DRF1_BIST_ST                                                        0x18007718
#define ISO_KCPU_DRF1_BIST_ST_reg_addr                                               "0xB8007718"
#define ISO_KCPU_DRF1_BIST_ST_reg                                                    0xB8007718
#define set_ISO_KCPU_DRF1_BIST_ST_reg(data)   (*((volatile unsigned int*) ISO_KCPU_DRF1_BIST_ST_reg)=data)
#define get_ISO_KCPU_DRF1_BIST_ST_reg   (*((volatile unsigned int*) ISO_KCPU_DRF1_BIST_ST_reg))
#define ISO_KCPU_DRF1_BIST_ST_inst_adr                                               "0x00C6"
#define ISO_KCPU_DRF1_BIST_ST_inst                                                   0x00C6
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_fail0_shift                             (1)
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_fail0_mask                              (0x00000002)
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_fail0(data)                             (0x00000002&((data)<<1))
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_fail0_src(data)                         ((0x00000002&(data))>>1)
#define ISO_KCPU_DRF1_BIST_ST_get_kcpu_drf1_bist_fail0(data)                         ((0x00000002&(data))>>1)
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_done_shift                              (0)
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_done_mask                               (0x00000001)
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_done(data)                              (0x00000001&((data)<<0))
#define ISO_KCPU_DRF1_BIST_ST_kcpu_drf1_bist_done_src(data)                          ((0x00000001&(data))>>0)
#define ISO_KCPU_DRF1_BIST_ST_get_kcpu_drf1_bist_done(data)                          ((0x00000001&(data))>>0)


#define ISO_KCPU_BISR_CTRL                                                           0x1800771C
#define ISO_KCPU_BISR_CTRL_reg_addr                                                  "0xB800771C"
#define ISO_KCPU_BISR_CTRL_reg                                                       0xB800771C
#define set_ISO_KCPU_BISR_CTRL_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_CTRL_reg)=data)
#define get_ISO_KCPU_BISR_CTRL_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_CTRL_reg))
#define ISO_KCPU_BISR_CTRL_inst_adr                                                  "0x00C7"
#define ISO_KCPU_BISR_CTRL_inst                                                      0x00C7
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_run1_pwr_rstn_shift                             (6)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_run1_pwr_rstn_mask                              (0x00000040)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_run1_pwr_rstn(data)                             (0x00000040&((data)<<6))
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_run1_pwr_rstn_src(data)                         ((0x00000040&(data))>>6)
#define ISO_KCPU_BISR_CTRL_get_kcpu_bisr_run1_pwr_rstn(data)                         ((0x00000040&(data))>>6)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_pwr_rstn_shift                                  (5)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_pwr_rstn_mask                                   (0x00000020)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_pwr_rstn(data)                                  (0x00000020&((data)<<5))
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_pwr_rstn_src(data)                              ((0x00000020&(data))>>5)
#define ISO_KCPU_BISR_CTRL_get_kcpu_bisr_pwr_rstn(data)                              ((0x00000020&(data))>>5)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_hold_remap_shift                                (4)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_hold_remap_mask                                 (0x00000010)
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_hold_remap(data)                                (0x00000010&((data)<<4))
#define ISO_KCPU_BISR_CTRL_kcpu_bisr_hold_remap_src(data)                            ((0x00000010&(data))>>4)
#define ISO_KCPU_BISR_CTRL_get_kcpu_bisr_hold_remap(data)                            ((0x00000010&(data))>>4)
#define ISO_KCPU_BISR_CTRL_bisr_run_shift                                            (1)
#define ISO_KCPU_BISR_CTRL_bisr_run_mask                                             (0x00000002)
#define ISO_KCPU_BISR_CTRL_bisr_run(data)                                            (0x00000002&((data)<<1))
#define ISO_KCPU_BISR_CTRL_bisr_run_src(data)                                        ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_CTRL_get_bisr_run(data)                                        ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_CTRL_write_data_shift                                          (0)
#define ISO_KCPU_BISR_CTRL_write_data_mask                                           (0x00000001)
#define ISO_KCPU_BISR_CTRL_write_data(data)                                          (0x00000001&((data)<<0))
#define ISO_KCPU_BISR_CTRL_write_data_src(data)                                      ((0x00000001&(data))>>0)
#define ISO_KCPU_BISR_CTRL_get_write_data(data)                                      ((0x00000001&(data))>>0)


#define ISO_KCPU_BISR_ST1                                                            0x18007720
#define ISO_KCPU_BISR_ST1_reg_addr                                                   "0xB8007720"
#define ISO_KCPU_BISR_ST1_reg                                                        0xB8007720
#define set_ISO_KCPU_BISR_ST1_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_ST1_reg)=data)
#define get_ISO_KCPU_BISR_ST1_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_ST1_reg))
#define ISO_KCPU_BISR_ST1_inst_adr                                                   "0x00C8"
#define ISO_KCPU_BISR_ST1_inst                                                       0x00C8
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail4_shift                                      (5)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail4_mask                                       (0x00000020)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail4(data)                                      (0x00000020&((data)<<5))
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail4_src(data)                                  ((0x00000020&(data))>>5)
#define ISO_KCPU_BISR_ST1_get_kcpu_bisr_fail4(data)                                  ((0x00000020&(data))>>5)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail3_shift                                      (4)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail3_mask                                       (0x00000010)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail3(data)                                      (0x00000010&((data)<<4))
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail3_src(data)                                  ((0x00000010&(data))>>4)
#define ISO_KCPU_BISR_ST1_get_kcpu_bisr_fail3(data)                                  ((0x00000010&(data))>>4)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail2_shift                                      (3)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail2_mask                                       (0x00000008)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail2(data)                                      (0x00000008&((data)<<3))
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail2_src(data)                                  ((0x00000008&(data))>>3)
#define ISO_KCPU_BISR_ST1_get_kcpu_bisr_fail2(data)                                  ((0x00000008&(data))>>3)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail1_shift                                      (2)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail1_mask                                       (0x00000004)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail1(data)                                      (0x00000004&((data)<<2))
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail1_src(data)                                  ((0x00000004&(data))>>2)
#define ISO_KCPU_BISR_ST1_get_kcpu_bisr_fail1(data)                                  ((0x00000004&(data))>>2)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail0_shift                                      (1)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail0_mask                                       (0x00000002)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail0(data)                                      (0x00000002&((data)<<1))
#define ISO_KCPU_BISR_ST1_kcpu_bisr_fail0_src(data)                                  ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST1_get_kcpu_bisr_fail0(data)                                  ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_done_shift                                       (0)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_done_mask                                        (0x00000001)
#define ISO_KCPU_BISR_ST1_kcpu_bisr_done(data)                                       (0x00000001&((data)<<0))
#define ISO_KCPU_BISR_ST1_kcpu_bisr_done_src(data)                                   ((0x00000001&(data))>>0)
#define ISO_KCPU_BISR_ST1_get_kcpu_bisr_done(data)                                   ((0x00000001&(data))>>0)


#define ISO_KCPU_BISR_ST2                                                            0x18007724
#define ISO_KCPU_BISR_ST2_reg_addr                                                   "0xB8007724"
#define ISO_KCPU_BISR_ST2_reg                                                        0xB8007724
#define set_ISO_KCPU_BISR_ST2_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_ST2_reg)=data)
#define get_ISO_KCPU_BISR_ST2_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_ST2_reg))
#define ISO_KCPU_BISR_ST2_inst_adr                                                   "0x00C9"
#define ISO_KCPU_BISR_ST2_inst                                                       0x00C9
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so4_shift                                        (25)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so4_mask                                         (0xFE000000)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so4(data)                                        (0xFE000000&((data)<<25))
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so4_src(data)                                    ((0xFE000000&(data))>>25)
#define ISO_KCPU_BISR_ST2_get_kcpu_bisr_so4(data)                                    ((0xFE000000&(data))>>25)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so3_shift                                        (18)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so3_mask                                         (0x01FC0000)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so3(data)                                        (0x01FC0000&((data)<<18))
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so3_src(data)                                    ((0x01FC0000&(data))>>18)
#define ISO_KCPU_BISR_ST2_get_kcpu_bisr_so3(data)                                    ((0x01FC0000&(data))>>18)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so2_shift                                        (12)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so2_mask                                         (0x0003F000)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so2(data)                                        (0x0003F000&((data)<<12))
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so2_src(data)                                    ((0x0003F000&(data))>>12)
#define ISO_KCPU_BISR_ST2_get_kcpu_bisr_so2(data)                                    ((0x0003F000&(data))>>12)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so1_shift                                        (6)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so1_mask                                         (0x00000FC0)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so1(data)                                        (0x00000FC0&((data)<<6))
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so1_src(data)                                    ((0x00000FC0&(data))>>6)
#define ISO_KCPU_BISR_ST2_get_kcpu_bisr_so1(data)                                    ((0x00000FC0&(data))>>6)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so0_shift                                        (0)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so0_mask                                         (0x0000003F)
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so0(data)                                        (0x0000003F&((data)<<0))
#define ISO_KCPU_BISR_ST2_kcpu_bisr_so0_src(data)                                    ((0x0000003F&(data))>>0)
#define ISO_KCPU_BISR_ST2_get_kcpu_bisr_so0(data)                                    ((0x0000003F&(data))>>0)


#define ISO_KCPU_BISR_ST3                                                            0x18007728
#define ISO_KCPU_BISR_ST3_reg_addr                                                   "0xB8007728"
#define ISO_KCPU_BISR_ST3_reg                                                        0xB8007728
#define set_ISO_KCPU_BISR_ST3_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_ST3_reg)=data)
#define get_ISO_KCPU_BISR_ST3_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_ST3_reg))
#define ISO_KCPU_BISR_ST3_inst_adr                                                   "0x00CA"
#define ISO_KCPU_BISR_ST3_inst                                                       0x00CA
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_2_shift                                   (1)
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_2_mask                                    (0x00000002)
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_2(data)                                   (0x00000002&((data)<<1))
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_2_src(data)                               ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST3_get_kcpu_bisr_repair_2(data)                               ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_1_shift                                   (0)
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_1_mask                                    (0x00000001)
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_1(data)                                   (0x00000001&((data)<<0))
#define ISO_KCPU_BISR_ST3_kcpu_bisr_repair_1_src(data)                               ((0x00000001&(data))>>0)
#define ISO_KCPU_BISR_ST3_get_kcpu_bisr_repair_1(data)                               ((0x00000001&(data))>>0)


#define ISO_KCPU_BISR_ST4                                                            0x1800772C
#define ISO_KCPU_BISR_ST4_reg_addr                                                   "0xB800772C"
#define ISO_KCPU_BISR_ST4_reg                                                        0xB800772C
#define set_ISO_KCPU_BISR_ST4_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_ST4_reg)=data)
#define get_ISO_KCPU_BISR_ST4_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_ST4_reg))
#define ISO_KCPU_BISR_ST4_inst_adr                                                   "0x00CB"
#define ISO_KCPU_BISR_ST4_inst                                                       0x00CB
#define ISO_KCPU_BISR_ST4_kcpu_drf_bisr_mode_shift                                   (1)
#define ISO_KCPU_BISR_ST4_kcpu_drf_bisr_mode_mask                                    (0x00000002)
#define ISO_KCPU_BISR_ST4_kcpu_drf_bisr_mode(data)                                   (0x00000002&((data)<<1))
#define ISO_KCPU_BISR_ST4_kcpu_drf_bisr_mode_src(data)                               ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST4_get_kcpu_drf_bisr_mode(data)                               ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST4_write_data_shift                                           (0)
#define ISO_KCPU_BISR_ST4_write_data_mask                                            (0x00000001)
#define ISO_KCPU_BISR_ST4_write_data(data)                                           (0x00000001&((data)<<0))
#define ISO_KCPU_BISR_ST4_write_data_src(data)                                       ((0x00000001&(data))>>0)
#define ISO_KCPU_BISR_ST4_get_write_data(data)                                       ((0x00000001&(data))>>0)


#define ISO_KCPU_BISR_ST5                                                            0x18007730
#define ISO_KCPU_BISR_ST5_reg_addr                                                   "0xB8007730"
#define ISO_KCPU_BISR_ST5_reg                                                        0xB8007730
#define set_ISO_KCPU_BISR_ST5_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_ST5_reg)=data)
#define get_ISO_KCPU_BISR_ST5_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_ST5_reg))
#define ISO_KCPU_BISR_ST5_inst_adr                                                   "0x00CC"
#define ISO_KCPU_BISR_ST5_inst                                                       0x00CC
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_start_pause_shift                            (1)
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_start_pause_mask                             (0x00000002)
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_start_pause(data)                            (0x00000002&((data)<<1))
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_start_pause_src(data)                        ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST5_get_kcpu_drf_bisr_start_pause(data)                        ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_test_resume_shift                            (0)
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_test_resume_mask                             (0x00000001)
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_test_resume(data)                            (0x00000001&((data)<<0))
#define ISO_KCPU_BISR_ST5_kcpu_drf_bisr_test_resume_src(data)                        ((0x00000001&(data))>>0)
#define ISO_KCPU_BISR_ST5_get_kcpu_drf_bisr_test_resume(data)                        ((0x00000001&(data))>>0)


#define ISO_KCPU_BISR_ST6                                                            0x18007734
#define ISO_KCPU_BISR_ST6_reg_addr                                                   "0xB8007734"
#define ISO_KCPU_BISR_ST6_reg                                                        0xB8007734
#define set_ISO_KCPU_BISR_ST6_reg(data)   (*((volatile unsigned int*) ISO_KCPU_BISR_ST6_reg)=data)
#define get_ISO_KCPU_BISR_ST6_reg   (*((volatile unsigned int*) ISO_KCPU_BISR_ST6_reg))
#define ISO_KCPU_BISR_ST6_inst_adr                                                   "0x00CD"
#define ISO_KCPU_BISR_ST6_inst                                                       0x00CD
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail4_shift                                  (5)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail4_mask                                   (0x00000020)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail4(data)                                  (0x00000020&((data)<<5))
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail4_src(data)                              ((0x00000020&(data))>>5)
#define ISO_KCPU_BISR_ST6_get_kcpu_drf_bisr_fail4(data)                              ((0x00000020&(data))>>5)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail3_shift                                  (4)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail3_mask                                   (0x00000010)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail3(data)                                  (0x00000010&((data)<<4))
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail3_src(data)                              ((0x00000010&(data))>>4)
#define ISO_KCPU_BISR_ST6_get_kcpu_drf_bisr_fail3(data)                              ((0x00000010&(data))>>4)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail2_shift                                  (3)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail2_mask                                   (0x00000008)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail2(data)                                  (0x00000008&((data)<<3))
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail2_src(data)                              ((0x00000008&(data))>>3)
#define ISO_KCPU_BISR_ST6_get_kcpu_drf_bisr_fail2(data)                              ((0x00000008&(data))>>3)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail1_shift                                  (2)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail1_mask                                   (0x00000004)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail1(data)                                  (0x00000004&((data)<<2))
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail1_src(data)                              ((0x00000004&(data))>>2)
#define ISO_KCPU_BISR_ST6_get_kcpu_drf_bisr_fail1(data)                              ((0x00000004&(data))>>2)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail0_shift                                  (1)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail0_mask                                   (0x00000002)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail0(data)                                  (0x00000002&((data)<<1))
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_fail0_src(data)                              ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST6_get_kcpu_drf_bisr_fail0(data)                              ((0x00000002&(data))>>1)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_done_shift                                   (0)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_done_mask                                    (0x00000001)
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_done(data)                                   (0x00000001&((data)<<0))
#define ISO_KCPU_BISR_ST6_kcpu_drf_bisr_done_src(data)                               ((0x00000001&(data))>>0)
#define ISO_KCPU_BISR_ST6_get_kcpu_drf_bisr_done(data)                               ((0x00000001&(data))>>0)


#define ISO_CPU_ST2V                                                                 0x18007738
#define ISO_CPU_ST2V_reg_addr                                                        "0xB8007738"
#define ISO_CPU_ST2V_reg                                                             0xB8007738
#define set_ISO_CPU_ST2V_reg(data)   (*((volatile unsigned int*) ISO_CPU_ST2V_reg)=data)
#define get_ISO_CPU_ST2V_reg   (*((volatile unsigned int*) ISO_CPU_ST2V_reg))
#define ISO_CPU_ST2V_inst_adr                                                        "0x00CE"
#define ISO_CPU_ST2V_inst                                                            0x00CE
#define ISO_CPU_ST2V_scpu_boot_info_valid_shift                                      (0)
#define ISO_CPU_ST2V_scpu_boot_info_valid_mask                                       (0x00000001)
#define ISO_CPU_ST2V_scpu_boot_info_valid(data)                                      (0x00000001&((data)<<0))
#define ISO_CPU_ST2V_scpu_boot_info_valid_src(data)                                  ((0x00000001&(data))>>0)
#define ISO_CPU_ST2V_get_scpu_boot_info_valid(data)                                  ((0x00000001&(data))>>0)


#define ISO_CPU_ST2                                                                  0x1800773C
#define ISO_CPU_ST2_reg_addr                                                         "0xB800773C"
#define ISO_CPU_ST2_reg                                                              0xB800773C
#define set_ISO_CPU_ST2_reg(data)   (*((volatile unsigned int*) ISO_CPU_ST2_reg)=data)
#define get_ISO_CPU_ST2_reg   (*((volatile unsigned int*) ISO_CPU_ST2_reg))
#define ISO_CPU_ST2_inst_adr                                                         "0x00CF"
#define ISO_CPU_ST2_inst                                                             0x00CF
#define ISO_CPU_ST2_scpu_boot_info_shift                                             (0)
#define ISO_CPU_ST2_scpu_boot_info_mask                                              (0xFFFFFFFF)
#define ISO_CPU_ST2_scpu_boot_info(data)                                             (0xFFFFFFFF&((data)<<0))
#define ISO_CPU_ST2_scpu_boot_info_src(data)                                         ((0xFFFFFFFF&(data))>>0)
#define ISO_CPU_ST2_get_scpu_boot_info(data)                                         ((0xFFFFFFFF&(data))>>0)


#define ISO_KCPU_DBG                                                                 0x18007740
#define ISO_KCPU_DBG_reg_addr                                                        "0xB8007740"
#define ISO_KCPU_DBG_reg                                                             0xB8007740
#define set_ISO_KCPU_DBG_reg(data)   (*((volatile unsigned int*) ISO_KCPU_DBG_reg)=data)
#define get_ISO_KCPU_DBG_reg   (*((volatile unsigned int*) ISO_KCPU_DBG_reg))
#define ISO_KCPU_DBG_inst_adr                                                        "0x00D0"
#define ISO_KCPU_DBG_inst                                                            0x00D0
#define ISO_KCPU_DBG_write_enable3_shift                                             (13)
#define ISO_KCPU_DBG_write_enable3_mask                                              (0x00002000)
#define ISO_KCPU_DBG_write_enable3(data)                                             (0x00002000&((data)<<13))
#define ISO_KCPU_DBG_write_enable3_src(data)                                         ((0x00002000&(data))>>13)
#define ISO_KCPU_DBG_get_write_enable3(data)                                         ((0x00002000&(data))>>13)
#define ISO_KCPU_DBG_sel1_shift                                                      (8)
#define ISO_KCPU_DBG_sel1_mask                                                       (0x00001F00)
#define ISO_KCPU_DBG_sel1(data)                                                      (0x00001F00&((data)<<8))
#define ISO_KCPU_DBG_sel1_src(data)                                                  ((0x00001F00&(data))>>8)
#define ISO_KCPU_DBG_get_sel1(data)                                                  ((0x00001F00&(data))>>8)
#define ISO_KCPU_DBG_write_enable2_shift                                             (7)
#define ISO_KCPU_DBG_write_enable2_mask                                              (0x00000080)
#define ISO_KCPU_DBG_write_enable2(data)                                             (0x00000080&((data)<<7))
#define ISO_KCPU_DBG_write_enable2_src(data)                                         ((0x00000080&(data))>>7)
#define ISO_KCPU_DBG_get_write_enable2(data)                                         ((0x00000080&(data))>>7)
#define ISO_KCPU_DBG_sel0_shift                                                      (2)
#define ISO_KCPU_DBG_sel0_mask                                                       (0x0000007C)
#define ISO_KCPU_DBG_sel0(data)                                                      (0x0000007C&((data)<<2))
#define ISO_KCPU_DBG_sel0_src(data)                                                  ((0x0000007C&(data))>>2)
#define ISO_KCPU_DBG_get_sel0(data)                                                  ((0x0000007C&(data))>>2)
#define ISO_KCPU_DBG_write_enable1_shift                                             (1)
#define ISO_KCPU_DBG_write_enable1_mask                                              (0x00000002)
#define ISO_KCPU_DBG_write_enable1(data)                                             (0x00000002&((data)<<1))
#define ISO_KCPU_DBG_write_enable1_src(data)                                         ((0x00000002&(data))>>1)
#define ISO_KCPU_DBG_get_write_enable1(data)                                         ((0x00000002&(data))>>1)
#define ISO_KCPU_DBG_enable_shift                                                    (0)
#define ISO_KCPU_DBG_enable_mask                                                     (0x00000001)
#define ISO_KCPU_DBG_enable(data)                                                    (0x00000001&((data)<<0))
#define ISO_KCPU_DBG_enable_src(data)                                                ((0x00000001&(data))>>0)
#define ISO_KCPU_DBG_get_enable(data)                                                ((0x00000001&(data))>>0)

#define ISO_ANLG                                                                     0x18007744
#define ISO_ANLG_reg_addr                                                            "0xB8007744"
#define ISO_ANLG_reg                                                                 0xB8007744
#define set_ISO_ANLG_reg(data)   (*((volatile unsigned int*) ISO_ANLG_reg)=data)
#define get_ISO_ANLG_reg   (*((volatile unsigned int*) ISO_ANLG_reg))
#define ISO_ANLG_inst_adr                                                            "0x00D1"
#define ISO_ANLG_inst                                                                0x00D1
#define ISO_ANLG_analogy_mode_in_shift                                               (0)
#define ISO_ANLG_analogy_mode_in_mask                                                (0x00000001)
#define ISO_ANLG_analogy_mode_in(data)                                               (0x00000001&((data)<<0))
#define ISO_ANLG_analogy_mode_in_src(data)                                           ((0x00000001&(data))>>0)
#define ISO_ANLG_get_analogy_mode_in(data)                                           ((0x00000001&(data))>>0)


#define ISO_U0RBR_THR_DLL                                                            0x18007800
#define ISO_U0RBR_THR_DLL_reg_addr                                                   "0xB8007800"
#define ISO_U0RBR_THR_DLL_reg                                                        0xB8007800
#define set_ISO_U0RBR_THR_DLL_reg(data)   (*((volatile unsigned int*) ISO_U0RBR_THR_DLL_reg)=data)
#define get_ISO_U0RBR_THR_DLL_reg   (*((volatile unsigned int*) ISO_U0RBR_THR_DLL_reg))
#define ISO_U0RBR_THR_DLL_inst_adr                                                   "0x0000"
#define ISO_U0RBR_THR_DLL_inst                                                       0x0000
#define ISO_U0RBR_THR_DLL_DLL_shift                                                  (0)
#define ISO_U0RBR_THR_DLL_DLL_mask                                                   (0x000000FF)
#define ISO_U0RBR_THR_DLL_DLL(data)                                                  (0x000000FF&((data)<<0))
#define ISO_U0RBR_THR_DLL_DLL_src(data)                                              ((0x000000FF&(data))>>0)
#define ISO_U0RBR_THR_DLL_get_DLL(data)                                              ((0x000000FF&(data))>>0)


#define ISO_U0IER_DLH                                                                0x18007804
#define ISO_U0IER_DLH_reg_addr                                                       "0xB8007804"
#define ISO_U0IER_DLH_reg                                                            0xB8007804
#define set_ISO_U0IER_DLH_reg(data)   (*((volatile unsigned int*) ISO_U0IER_DLH_reg)=data)
#define get_ISO_U0IER_DLH_reg   (*((volatile unsigned int*) ISO_U0IER_DLH_reg))
#define ISO_U0IER_DLH_inst_adr                                                       "0x0001"
#define ISO_U0IER_DLH_inst                                                           0x0001
#define ISO_U0IER_DLH_DLH_shift                                                      (0)
#define ISO_U0IER_DLH_DLH_mask                                                       (0x000000FF)
#define ISO_U0IER_DLH_DLH(data)                                                      (0x000000FF&((data)<<0))
#define ISO_U0IER_DLH_DLH_src(data)                                                  ((0x000000FF&(data))>>0)
#define ISO_U0IER_DLH_get_DLH(data)                                                  ((0x000000FF&(data))>>0)


#define ISO_U0IIR_FCR                                                                0x18007808
#define ISO_U0IIR_FCR_reg_addr                                                       "0xB8007808"
#define ISO_U0IIR_FCR_reg                                                            0xB8007808
#define set_ISO_U0IIR_FCR_reg(data)   (*((volatile unsigned int*) ISO_U0IIR_FCR_reg)=data)
#define get_ISO_U0IIR_FCR_reg   (*((volatile unsigned int*) ISO_U0IIR_FCR_reg))
#define ISO_U0IIR_FCR_inst_adr                                                       "0x0002"
#define ISO_U0IIR_FCR_inst                                                           0x0002
#define ISO_U0IIR_FCR_FIFO16_shift                                                   (6)
#define ISO_U0IIR_FCR_FIFO16_mask                                                    (0x000000C0)
#define ISO_U0IIR_FCR_FIFO16(data)                                                   (0x000000C0&((data)<<6))
#define ISO_U0IIR_FCR_FIFO16_src(data)                                               ((0x000000C0&(data))>>6)
#define ISO_U0IIR_FCR_get_FIFO16(data)                                               ((0x000000C0&(data))>>6)
#define ISO_U0IIR_FCR_IID_shift                                                      (0)
#define ISO_U0IIR_FCR_IID_mask                                                       (0x0000000F)
#define ISO_U0IIR_FCR_IID(data)                                                      (0x0000000F&((data)<<0))
#define ISO_U0IIR_FCR_IID_src(data)                                                  ((0x0000000F&(data))>>0)
#define ISO_U0IIR_FCR_get_IID(data)                                                  ((0x0000000F&(data))>>0)


#define ISO_U0LCR                                                                    0x1800780C
#define ISO_U0LCR_reg_addr                                                           "0xB800780C"
#define ISO_U0LCR_reg                                                                0xB800780C
#define set_ISO_U0LCR_reg(data)   (*((volatile unsigned int*) ISO_U0LCR_reg)=data)
#define get_ISO_U0LCR_reg   (*((volatile unsigned int*) ISO_U0LCR_reg))
#define ISO_U0LCR_inst_adr                                                           "0x0003"
#define ISO_U0LCR_inst                                                               0x0003
#define ISO_U0LCR_DLAB_shift                                                         (7)
#define ISO_U0LCR_DLAB_mask                                                          (0x00000080)
#define ISO_U0LCR_DLAB(data)                                                         (0x00000080&((data)<<7))
#define ISO_U0LCR_DLAB_src(data)                                                     ((0x00000080&(data))>>7)
#define ISO_U0LCR_get_DLAB(data)                                                     ((0x00000080&(data))>>7)
#define ISO_U0LCR_BRK_shift                                                          (6)
#define ISO_U0LCR_BRK_mask                                                           (0x00000040)
#define ISO_U0LCR_BRK(data)                                                          (0x00000040&((data)<<6))
#define ISO_U0LCR_BRK_src(data)                                                      ((0x00000040&(data))>>6)
#define ISO_U0LCR_get_BRK(data)                                                      ((0x00000040&(data))>>6)
#define ISO_U0LCR_EPS_shift                                                          (4)
#define ISO_U0LCR_EPS_mask                                                           (0x00000010)
#define ISO_U0LCR_EPS(data)                                                          (0x00000010&((data)<<4))
#define ISO_U0LCR_EPS_src(data)                                                      ((0x00000010&(data))>>4)
#define ISO_U0LCR_get_EPS(data)                                                      ((0x00000010&(data))>>4)
#define ISO_U0LCR_PEN_shift                                                          (3)
#define ISO_U0LCR_PEN_mask                                                           (0x00000008)
#define ISO_U0LCR_PEN(data)                                                          (0x00000008&((data)<<3))
#define ISO_U0LCR_PEN_src(data)                                                      ((0x00000008&(data))>>3)
#define ISO_U0LCR_get_PEN(data)                                                      ((0x00000008&(data))>>3)
#define ISO_U0LCR_STB_shift                                                          (2)
#define ISO_U0LCR_STB_mask                                                           (0x00000004)
#define ISO_U0LCR_STB(data)                                                          (0x00000004&((data)<<2))
#define ISO_U0LCR_STB_src(data)                                                      ((0x00000004&(data))>>2)
#define ISO_U0LCR_get_STB(data)                                                      ((0x00000004&(data))>>2)
#define ISO_U0LCR_WLS_shift                                                          (0)
#define ISO_U0LCR_WLS_mask                                                           (0x00000003)
#define ISO_U0LCR_WLS(data)                                                          (0x00000003&((data)<<0))
#define ISO_U0LCR_WLS_src(data)                                                      ((0x00000003&(data))>>0)
#define ISO_U0LCR_get_WLS(data)                                                      ((0x00000003&(data))>>0)


#define ISO_U0MCR                                                                    0x18007810
#define ISO_U0MCR_reg_addr                                                           "0xB8007810"
#define ISO_U0MCR_reg                                                                0xB8007810
#define set_ISO_U0MCR_reg(data)   (*((volatile unsigned int*) ISO_U0MCR_reg)=data)
#define get_ISO_U0MCR_reg   (*((volatile unsigned int*) ISO_U0MCR_reg))
#define ISO_U0MCR_inst_adr                                                           "0x0004"
#define ISO_U0MCR_inst                                                               0x0004
#define ISO_U0MCR_AFCE_shift                                                         (5)
#define ISO_U0MCR_AFCE_mask                                                          (0x00000020)
#define ISO_U0MCR_AFCE(data)                                                         (0x00000020&((data)<<5))
#define ISO_U0MCR_AFCE_src(data)                                                     ((0x00000020&(data))>>5)
#define ISO_U0MCR_get_AFCE(data)                                                     ((0x00000020&(data))>>5)
#define ISO_U0MCR_LOOP_shift                                                         (4)
#define ISO_U0MCR_LOOP_mask                                                          (0x00000010)
#define ISO_U0MCR_LOOP(data)                                                         (0x00000010&((data)<<4))
#define ISO_U0MCR_LOOP_src(data)                                                     ((0x00000010&(data))>>4)
#define ISO_U0MCR_get_LOOP(data)                                                     ((0x00000010&(data))>>4)
#define ISO_U0MCR_RTS_shift                                                          (1)
#define ISO_U0MCR_RTS_mask                                                           (0x00000002)
#define ISO_U0MCR_RTS(data)                                                          (0x00000002&((data)<<1))
#define ISO_U0MCR_RTS_src(data)                                                      ((0x00000002&(data))>>1)
#define ISO_U0MCR_get_RTS(data)                                                      ((0x00000002&(data))>>1)
#define ISO_U0MCR_DTR_shift                                                          (0)
#define ISO_U0MCR_DTR_mask                                                           (0x00000001)
#define ISO_U0MCR_DTR(data)                                                          (0x00000001&((data)<<0))
#define ISO_U0MCR_DTR_src(data)                                                      ((0x00000001&(data))>>0)
#define ISO_U0MCR_get_DTR(data)                                                      ((0x00000001&(data))>>0)


#define ISO_U0LSR                                                                    0x18007814
#define ISO_U0LSR_reg_addr                                                           "0xB8007814"
#define ISO_U0LSR_reg                                                                0xB8007814
#define set_ISO_U0LSR_reg(data)   (*((volatile unsigned int*) ISO_U0LSR_reg)=data)
#define get_ISO_U0LSR_reg   (*((volatile unsigned int*) ISO_U0LSR_reg))
#define ISO_U0LSR_inst_adr                                                           "0x0005"
#define ISO_U0LSR_inst                                                               0x0005
#define ISO_U0LSR_RFE_shift                                                          (7)
#define ISO_U0LSR_RFE_mask                                                           (0x00000080)
#define ISO_U0LSR_RFE(data)                                                          (0x00000080&((data)<<7))
#define ISO_U0LSR_RFE_src(data)                                                      ((0x00000080&(data))>>7)
#define ISO_U0LSR_get_RFE(data)                                                      ((0x00000080&(data))>>7)
#define ISO_U0LSR_TEMT_shift                                                         (6)
#define ISO_U0LSR_TEMT_mask                                                          (0x00000040)
#define ISO_U0LSR_TEMT(data)                                                         (0x00000040&((data)<<6))
#define ISO_U0LSR_TEMT_src(data)                                                     ((0x00000040&(data))>>6)
#define ISO_U0LSR_get_TEMT(data)                                                     ((0x00000040&(data))>>6)
#define ISO_U0LSR_THRE_shift                                                         (5)
#define ISO_U0LSR_THRE_mask                                                          (0x00000020)
#define ISO_U0LSR_THRE(data)                                                         (0x00000020&((data)<<5))
#define ISO_U0LSR_THRE_src(data)                                                     ((0x00000020&(data))>>5)
#define ISO_U0LSR_get_THRE(data)                                                     ((0x00000020&(data))>>5)
#define ISO_U0LSR_BI_shift                                                           (4)
#define ISO_U0LSR_BI_mask                                                            (0x00000010)
#define ISO_U0LSR_BI(data)                                                           (0x00000010&((data)<<4))
#define ISO_U0LSR_BI_src(data)                                                       ((0x00000010&(data))>>4)
#define ISO_U0LSR_get_BI(data)                                                       ((0x00000010&(data))>>4)
#define ISO_U0LSR_FE_shift                                                           (3)
#define ISO_U0LSR_FE_mask                                                            (0x00000008)
#define ISO_U0LSR_FE(data)                                                           (0x00000008&((data)<<3))
#define ISO_U0LSR_FE_src(data)                                                       ((0x00000008&(data))>>3)
#define ISO_U0LSR_get_FE(data)                                                       ((0x00000008&(data))>>3)
#define ISO_U0LSR_PE_shift                                                           (2)
#define ISO_U0LSR_PE_mask                                                            (0x00000004)
#define ISO_U0LSR_PE(data)                                                           (0x00000004&((data)<<2))
#define ISO_U0LSR_PE_src(data)                                                       ((0x00000004&(data))>>2)
#define ISO_U0LSR_get_PE(data)                                                       ((0x00000004&(data))>>2)
#define ISO_U0LSR_OE_shift                                                           (1)
#define ISO_U0LSR_OE_mask                                                            (0x00000002)
#define ISO_U0LSR_OE(data)                                                           (0x00000002&((data)<<1))
#define ISO_U0LSR_OE_src(data)                                                       ((0x00000002&(data))>>1)
#define ISO_U0LSR_get_OE(data)                                                       ((0x00000002&(data))>>1)
#define ISO_U0LSR_DR_shift                                                           (0)
#define ISO_U0LSR_DR_mask                                                            (0x00000001)
#define ISO_U0LSR_DR(data)                                                           (0x00000001&((data)<<0))
#define ISO_U0LSR_DR_src(data)                                                       ((0x00000001&(data))>>0)
#define ISO_U0LSR_get_DR(data)                                                       ((0x00000001&(data))>>0)


#define ISO_U0MSR                                                                    0x18007818
#define ISO_U0MSR_reg_addr                                                           "0xB8007818"
#define ISO_U0MSR_reg                                                                0xB8007818
#define set_ISO_U0MSR_reg(data)   (*((volatile unsigned int*) ISO_U0MSR_reg)=data)
#define get_ISO_U0MSR_reg   (*((volatile unsigned int*) ISO_U0MSR_reg))
#define ISO_U0MSR_inst_adr                                                           "0x0006"
#define ISO_U0MSR_inst                                                               0x0006
#define ISO_U0MSR_DCD_shift                                                          (7)
#define ISO_U0MSR_DCD_mask                                                           (0x00000080)
#define ISO_U0MSR_DCD(data)                                                          (0x00000080&((data)<<7))
#define ISO_U0MSR_DCD_src(data)                                                      ((0x00000080&(data))>>7)
#define ISO_U0MSR_get_DCD(data)                                                      ((0x00000080&(data))>>7)
#define ISO_U0MSR_RI_shift                                                           (6)
#define ISO_U0MSR_RI_mask                                                            (0x00000040)
#define ISO_U0MSR_RI(data)                                                           (0x00000040&((data)<<6))
#define ISO_U0MSR_RI_src(data)                                                       ((0x00000040&(data))>>6)
#define ISO_U0MSR_get_RI(data)                                                       ((0x00000040&(data))>>6)
#define ISO_U0MSR_DSR_shift                                                          (5)
#define ISO_U0MSR_DSR_mask                                                           (0x00000020)
#define ISO_U0MSR_DSR(data)                                                          (0x00000020&((data)<<5))
#define ISO_U0MSR_DSR_src(data)                                                      ((0x00000020&(data))>>5)
#define ISO_U0MSR_get_DSR(data)                                                      ((0x00000020&(data))>>5)
#define ISO_U0MSR_CTS_shift                                                          (4)
#define ISO_U0MSR_CTS_mask                                                           (0x00000010)
#define ISO_U0MSR_CTS(data)                                                          (0x00000010&((data)<<4))
#define ISO_U0MSR_CTS_src(data)                                                      ((0x00000010&(data))>>4)
#define ISO_U0MSR_get_CTS(data)                                                      ((0x00000010&(data))>>4)
#define ISO_U0MSR_DDCD_shift                                                         (3)
#define ISO_U0MSR_DDCD_mask                                                          (0x00000008)
#define ISO_U0MSR_DDCD(data)                                                         (0x00000008&((data)<<3))
#define ISO_U0MSR_DDCD_src(data)                                                     ((0x00000008&(data))>>3)
#define ISO_U0MSR_get_DDCD(data)                                                     ((0x00000008&(data))>>3)
#define ISO_U0MSR_TERI_shift                                                         (2)
#define ISO_U0MSR_TERI_mask                                                          (0x00000004)
#define ISO_U0MSR_TERI(data)                                                         (0x00000004&((data)<<2))
#define ISO_U0MSR_TERI_src(data)                                                     ((0x00000004&(data))>>2)
#define ISO_U0MSR_get_TERI(data)                                                     ((0x00000004&(data))>>2)
#define ISO_U0MSR_DDSR_shift                                                         (1)
#define ISO_U0MSR_DDSR_mask                                                          (0x00000002)
#define ISO_U0MSR_DDSR(data)                                                         (0x00000002&((data)<<1))
#define ISO_U0MSR_DDSR_src(data)                                                     ((0x00000002&(data))>>1)
#define ISO_U0MSR_get_DDSR(data)                                                     ((0x00000002&(data))>>1)
#define ISO_U0MSR_DCTS_shift                                                         (0)
#define ISO_U0MSR_DCTS_mask                                                          (0x00000001)
#define ISO_U0MSR_DCTS(data)                                                         (0x00000001&((data)<<0))
#define ISO_U0MSR_DCTS_src(data)                                                     ((0x00000001&(data))>>0)
#define ISO_U0MSR_get_DCTS(data)                                                     ((0x00000001&(data))>>0)


#define ISO_U0SCR                                                                    0x1800781C
#define ISO_U0SCR_reg_addr                                                           "0xB800781C"
#define ISO_U0SCR_reg                                                                0xB800781C
#define set_ISO_U0SCR_reg(data)   (*((volatile unsigned int*) ISO_U0SCR_reg)=data)
#define get_ISO_U0SCR_reg   (*((volatile unsigned int*) ISO_U0SCR_reg))
#define ISO_U0SCR_inst_adr                                                           "0x0007"
#define ISO_U0SCR_inst                                                               0x0007
#define ISO_U0SCR_SCR_shift                                                          (0)
#define ISO_U0SCR_SCR_mask                                                           (0x000000FF)
#define ISO_U0SCR_SCR(data)                                                          (0x000000FF&((data)<<0))
#define ISO_U0SCR_SCR_src(data)                                                      ((0x000000FF&(data))>>0)
#define ISO_U0SCR_get_SCR(data)                                                      ((0x000000FF&(data))>>0)


#define ISO_U0SRBR_0                                                                 0x18007830
#define ISO_U0SRBR_1                                                                 0x18007834
#define ISO_U0SRBR_2                                                                 0x18007838
#define ISO_U0SRBR_3                                                                 0x1800783C
#define ISO_U0SRBR_4                                                                 0x18007840
#define ISO_U0SRBR_5                                                                 0x18007844
#define ISO_U0SRBR_6                                                                 0x18007848
#define ISO_U0SRBR_7                                                                 0x1800784C
#define ISO_U0SRBR_8                                                                 0x18007850
#define ISO_U0SRBR_9                                                                 0x18007854
#define ISO_U0SRBR_10                                                                 0x18007858
#define ISO_U0SRBR_11                                                                 0x1800785C
#define ISO_U0SRBR_12                                                                 0x18007860
#define ISO_U0SRBR_13                                                                 0x18007864
#define ISO_U0SRBR_14                                                                 0x18007868
#define ISO_U0SRBR_15                                                                 0x1800786C
#define ISO_U0SRBR_0_reg_addr                                                        "0xB8007830"
#define ISO_U0SRBR_1_reg_addr                                                        "0xB8007834"
#define ISO_U0SRBR_2_reg_addr                                                        "0xB8007838"
#define ISO_U0SRBR_3_reg_addr                                                        "0xB800783C"
#define ISO_U0SRBR_4_reg_addr                                                        "0xB8007840"
#define ISO_U0SRBR_5_reg_addr                                                        "0xB8007844"
#define ISO_U0SRBR_6_reg_addr                                                        "0xB8007848"
#define ISO_U0SRBR_7_reg_addr                                                        "0xB800784C"
#define ISO_U0SRBR_8_reg_addr                                                        "0xB8007850"
#define ISO_U0SRBR_9_reg_addr                                                        "0xB8007854"
#define ISO_U0SRBR_10_reg_addr                                                        "0xB8007858"
#define ISO_U0SRBR_11_reg_addr                                                        "0xB800785C"
#define ISO_U0SRBR_12_reg_addr                                                        "0xB8007860"
#define ISO_U0SRBR_13_reg_addr                                                        "0xB8007864"
#define ISO_U0SRBR_14_reg_addr                                                        "0xB8007868"
#define ISO_U0SRBR_15_reg_addr                                                        "0xB800786C"
#define ISO_U0SRBR_0_reg                                                             0xB8007830
#define ISO_U0SRBR_1_reg                                                             0xB8007834
#define ISO_U0SRBR_2_reg                                                             0xB8007838
#define ISO_U0SRBR_3_reg                                                             0xB800783C
#define ISO_U0SRBR_4_reg                                                             0xB8007840
#define ISO_U0SRBR_5_reg                                                             0xB8007844
#define ISO_U0SRBR_6_reg                                                             0xB8007848
#define ISO_U0SRBR_7_reg                                                             0xB800784C
#define ISO_U0SRBR_8_reg                                                             0xB8007850
#define ISO_U0SRBR_9_reg                                                             0xB8007854
#define ISO_U0SRBR_10_reg                                                             0xB8007858
#define ISO_U0SRBR_11_reg                                                             0xB800785C
#define ISO_U0SRBR_12_reg                                                             0xB8007860
#define ISO_U0SRBR_13_reg                                                             0xB8007864
#define ISO_U0SRBR_14_reg                                                             0xB8007868
#define ISO_U0SRBR_15_reg                                                             0xB800786C
#define set_ISO_U0SRBR_0_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_0_reg)=data)
#define set_ISO_U0SRBR_1_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_1_reg)=data)
#define set_ISO_U0SRBR_2_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_2_reg)=data)
#define set_ISO_U0SRBR_3_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_3_reg)=data)
#define set_ISO_U0SRBR_4_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_4_reg)=data)
#define set_ISO_U0SRBR_5_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_5_reg)=data)
#define set_ISO_U0SRBR_6_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_6_reg)=data)
#define set_ISO_U0SRBR_7_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_7_reg)=data)
#define set_ISO_U0SRBR_8_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_8_reg)=data)
#define set_ISO_U0SRBR_9_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_9_reg)=data)
#define set_ISO_U0SRBR_10_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_10_reg)=data)
#define set_ISO_U0SRBR_11_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_11_reg)=data)
#define set_ISO_U0SRBR_12_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_12_reg)=data)
#define set_ISO_U0SRBR_13_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_13_reg)=data)
#define set_ISO_U0SRBR_14_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_14_reg)=data)
#define set_ISO_U0SRBR_15_reg(data)   (*((volatile unsigned int*) ISO_U0SRBR_15_reg)=data)
#define get_ISO_U0SRBR_0_reg   (*((volatile unsigned int*) ISO_U0SRBR_0_reg))
#define get_ISO_U0SRBR_1_reg   (*((volatile unsigned int*) ISO_U0SRBR_1_reg))
#define get_ISO_U0SRBR_2_reg   (*((volatile unsigned int*) ISO_U0SRBR_2_reg))
#define get_ISO_U0SRBR_3_reg   (*((volatile unsigned int*) ISO_U0SRBR_3_reg))
#define get_ISO_U0SRBR_4_reg   (*((volatile unsigned int*) ISO_U0SRBR_4_reg))
#define get_ISO_U0SRBR_5_reg   (*((volatile unsigned int*) ISO_U0SRBR_5_reg))
#define get_ISO_U0SRBR_6_reg   (*((volatile unsigned int*) ISO_U0SRBR_6_reg))
#define get_ISO_U0SRBR_7_reg   (*((volatile unsigned int*) ISO_U0SRBR_7_reg))
#define get_ISO_U0SRBR_8_reg   (*((volatile unsigned int*) ISO_U0SRBR_8_reg))
#define get_ISO_U0SRBR_9_reg   (*((volatile unsigned int*) ISO_U0SRBR_9_reg))
#define get_ISO_U0SRBR_10_reg   (*((volatile unsigned int*) ISO_U0SRBR_10_reg))
#define get_ISO_U0SRBR_11_reg   (*((volatile unsigned int*) ISO_U0SRBR_11_reg))
#define get_ISO_U0SRBR_12_reg   (*((volatile unsigned int*) ISO_U0SRBR_12_reg))
#define get_ISO_U0SRBR_13_reg   (*((volatile unsigned int*) ISO_U0SRBR_13_reg))
#define get_ISO_U0SRBR_14_reg   (*((volatile unsigned int*) ISO_U0SRBR_14_reg))
#define get_ISO_U0SRBR_15_reg   (*((volatile unsigned int*) ISO_U0SRBR_15_reg))
#define ISO_U0SRBR_0_inst_adr                                                        "0x000C"
#define ISO_U0SRBR_1_inst_adr                                                        "0x000D"
#define ISO_U0SRBR_2_inst_adr                                                        "0x000E"
#define ISO_U0SRBR_3_inst_adr                                                        "0x000F"
#define ISO_U0SRBR_4_inst_adr                                                        "0x0010"
#define ISO_U0SRBR_5_inst_adr                                                        "0x0011"
#define ISO_U0SRBR_6_inst_adr                                                        "0x0012"
#define ISO_U0SRBR_7_inst_adr                                                        "0x0013"
#define ISO_U0SRBR_8_inst_adr                                                        "0x0014"
#define ISO_U0SRBR_9_inst_adr                                                        "0x0015"
#define ISO_U0SRBR_10_inst_adr                                                        "0x0016"
#define ISO_U0SRBR_11_inst_adr                                                        "0x0017"
#define ISO_U0SRBR_12_inst_adr                                                        "0x0018"
#define ISO_U0SRBR_13_inst_adr                                                        "0x0019"
#define ISO_U0SRBR_14_inst_adr                                                        "0x001A"
#define ISO_U0SRBR_15_inst_adr                                                        "0x001B"
#define ISO_U0SRBR_0_inst                                                            0x000C
#define ISO_U0SRBR_1_inst                                                            0x000D
#define ISO_U0SRBR_2_inst                                                            0x000E
#define ISO_U0SRBR_3_inst                                                            0x000F
#define ISO_U0SRBR_4_inst                                                            0x0010
#define ISO_U0SRBR_5_inst                                                            0x0011
#define ISO_U0SRBR_6_inst                                                            0x0012
#define ISO_U0SRBR_7_inst                                                            0x0013
#define ISO_U0SRBR_8_inst                                                            0x0014
#define ISO_U0SRBR_9_inst                                                            0x0015
#define ISO_U0SRBR_10_inst                                                            0x0016
#define ISO_U0SRBR_11_inst                                                            0x0017
#define ISO_U0SRBR_12_inst                                                            0x0018
#define ISO_U0SRBR_13_inst                                                            0x0019
#define ISO_U0SRBR_14_inst                                                            0x001A
#define ISO_U0SRBR_15_inst                                                            0x001B
#define ISO_U0SRBR_RBD_shift                                                         (0)
#define ISO_U0SRBR_RBD_mask                                                          (0x000000FF)
#define ISO_U0SRBR_RBD(data)                                                         (0x000000FF&((data)<<0))
#define ISO_U0SRBR_RBD_src(data)                                                     ((0x000000FF&(data))>>0)
#define ISO_U0SRBR_get_RBD(data)                                                     ((0x000000FF&(data))>>0)


#define ISO_U0FAR                                                                    0x18007870
#define ISO_U0FAR_reg_addr                                                           "0xB8007870"
#define ISO_U0FAR_reg                                                                0xB8007870
#define set_ISO_U0FAR_reg(data)   (*((volatile unsigned int*) ISO_U0FAR_reg)=data)
#define get_ISO_U0FAR_reg   (*((volatile unsigned int*) ISO_U0FAR_reg))
#define ISO_U0FAR_inst_adr                                                           "0x001C"
#define ISO_U0FAR_inst                                                               0x001C
#define ISO_U0FAR_FAR_shift                                                          (0)
#define ISO_U0FAR_FAR_mask                                                           (0x000000FF)
#define ISO_U0FAR_FAR(data)                                                          (0x000000FF&((data)<<0))
#define ISO_U0FAR_FAR_src(data)                                                      ((0x000000FF&(data))>>0)
#define ISO_U0FAR_get_FAR(data)                                                      ((0x000000FF&(data))>>0)


#define ISO_U0TFR                                                                    0x18007874
#define ISO_U0TFR_reg_addr                                                           "0xB8007874"
#define ISO_U0TFR_reg                                                                0xB8007874
#define set_ISO_U0TFR_reg(data)   (*((volatile unsigned int*) ISO_U0TFR_reg)=data)
#define get_ISO_U0TFR_reg   (*((volatile unsigned int*) ISO_U0TFR_reg))
#define ISO_U0TFR_inst_adr                                                           "0x001D"
#define ISO_U0TFR_inst                                                               0x001D
#define ISO_U0TFR_FAR_shift                                                          (0)
#define ISO_U0TFR_FAR_mask                                                           (0x000000FF)
#define ISO_U0TFR_FAR(data)                                                          (0x000000FF&((data)<<0))
#define ISO_U0TFR_FAR_src(data)                                                      ((0x000000FF&(data))>>0)
#define ISO_U0TFR_get_FAR(data)                                                      ((0x000000FF&(data))>>0)


#define ISO_U0RFW                                                                    0x18007878
#define ISO_U0RFW_reg_addr                                                           "0xB8007878"
#define ISO_U0RFW_reg                                                                0xB8007878
#define set_ISO_U0RFW_reg(data)   (*((volatile unsigned int*) ISO_U0RFW_reg)=data)
#define get_ISO_U0RFW_reg   (*((volatile unsigned int*) ISO_U0RFW_reg))
#define ISO_U0RFW_inst_adr                                                           "0x001E"
#define ISO_U0RFW_inst                                                               0x001E
#define ISO_U0RFW_RFFE_shift                                                         (9)
#define ISO_U0RFW_RFFE_mask                                                          (0x00000200)
#define ISO_U0RFW_RFFE(data)                                                         (0x00000200&((data)<<9))
#define ISO_U0RFW_RFFE_src(data)                                                     ((0x00000200&(data))>>9)
#define ISO_U0RFW_get_RFFE(data)                                                     ((0x00000200&(data))>>9)
#define ISO_U0RFW_RFPF_shift                                                         (8)
#define ISO_U0RFW_RFPF_mask                                                          (0x00000100)
#define ISO_U0RFW_RFPF(data)                                                         (0x00000100&((data)<<8))
#define ISO_U0RFW_RFPF_src(data)                                                     ((0x00000100&(data))>>8)
#define ISO_U0RFW_get_RFPF(data)                                                     ((0x00000100&(data))>>8)
#define ISO_U0RFW_RFWD_shift                                                         (0)
#define ISO_U0RFW_RFWD_mask                                                          (0x000000FF)
#define ISO_U0RFW_RFWD(data)                                                         (0x000000FF&((data)<<0))
#define ISO_U0RFW_RFWD_src(data)                                                     ((0x000000FF&(data))>>0)
#define ISO_U0RFW_get_RFWD(data)                                                     ((0x000000FF&(data))>>0)


#define ISO_U0USR                                                                    0x1800787C
#define ISO_U0USR_reg_addr                                                           "0xB800787C"
#define ISO_U0USR_reg                                                                0xB800787C
#define set_ISO_U0USR_reg(data)   (*((volatile unsigned int*) ISO_U0USR_reg)=data)
#define get_ISO_U0USR_reg   (*((volatile unsigned int*) ISO_U0USR_reg))
#define ISO_U0USR_inst_adr                                                           "0x001F"
#define ISO_U0USR_inst                                                               0x001F
#define ISO_U0USR_RFF_shift                                                          (4)
#define ISO_U0USR_RFF_mask                                                           (0x00000010)
#define ISO_U0USR_RFF(data)                                                          (0x00000010&((data)<<4))
#define ISO_U0USR_RFF_src(data)                                                      ((0x00000010&(data))>>4)
#define ISO_U0USR_get_RFF(data)                                                      ((0x00000010&(data))>>4)
#define ISO_U0USR_RFNE_shift                                                         (3)
#define ISO_U0USR_RFNE_mask                                                          (0x00000008)
#define ISO_U0USR_RFNE(data)                                                         (0x00000008&((data)<<3))
#define ISO_U0USR_RFNE_src(data)                                                     ((0x00000008&(data))>>3)
#define ISO_U0USR_get_RFNE(data)                                                     ((0x00000008&(data))>>3)
#define ISO_U0USR_TFE_shift                                                          (2)
#define ISO_U0USR_TFE_mask                                                           (0x00000004)
#define ISO_U0USR_TFE(data)                                                          (0x00000004&((data)<<2))
#define ISO_U0USR_TFE_src(data)                                                      ((0x00000004&(data))>>2)
#define ISO_U0USR_get_TFE(data)                                                      ((0x00000004&(data))>>2)
#define ISO_U0USR_TFNF_shift                                                         (1)
#define ISO_U0USR_TFNF_mask                                                          (0x00000002)
#define ISO_U0USR_TFNF(data)                                                         (0x00000002&((data)<<1))
#define ISO_U0USR_TFNF_src(data)                                                     ((0x00000002&(data))>>1)
#define ISO_U0USR_get_TFNF(data)                                                     ((0x00000002&(data))>>1)
#define ISO_U0USR_BUSY_shift                                                         (0)
#define ISO_U0USR_BUSY_mask                                                          (0x00000001)
#define ISO_U0USR_BUSY(data)                                                         (0x00000001&((data)<<0))
#define ISO_U0USR_BUSY_src(data)                                                     ((0x00000001&(data))>>0)
#define ISO_U0USR_get_BUSY(data)                                                     ((0x00000001&(data))>>0)


#define ISO_U0TFL                                                                    0x18007880
#define ISO_U0TFL_reg_addr                                                           "0xB8007880"
#define ISO_U0TFL_reg                                                                0xB8007880
#define set_ISO_U0TFL_reg(data)   (*((volatile unsigned int*) ISO_U0TFL_reg)=data)
#define get_ISO_U0TFL_reg   (*((volatile unsigned int*) ISO_U0TFL_reg))
#define ISO_U0TFL_inst_adr                                                           "0x0020"
#define ISO_U0TFL_inst                                                               0x0020
#define ISO_U0TFL_TFL_shift                                                          (0)
#define ISO_U0TFL_TFL_mask                                                           (0x000000FF)
#define ISO_U0TFL_TFL(data)                                                          (0x000000FF&((data)<<0))
#define ISO_U0TFL_TFL_src(data)                                                      ((0x000000FF&(data))>>0)
#define ISO_U0TFL_get_TFL(data)                                                      ((0x000000FF&(data))>>0)


#define ISO_U0RFL                                                                    0x18007884
#define ISO_U0RFL_reg_addr                                                           "0xB8007884"
#define ISO_U0RFL_reg                                                                0xB8007884
#define set_ISO_U0RFL_reg(data)   (*((volatile unsigned int*) ISO_U0RFL_reg)=data)
#define get_ISO_U0RFL_reg   (*((volatile unsigned int*) ISO_U0RFL_reg))
#define ISO_U0RFL_inst_adr                                                           "0x0021"
#define ISO_U0RFL_inst                                                               0x0021
#define ISO_U0RFL_RFL_shift                                                          (0)
#define ISO_U0RFL_RFL_mask                                                           (0x000000FF)
#define ISO_U0RFL_RFL(data)                                                          (0x000000FF&((data)<<0))
#define ISO_U0RFL_RFL_src(data)                                                      ((0x000000FF&(data))>>0)
#define ISO_U0RFL_get_RFL(data)                                                      ((0x000000FF&(data))>>0)


#define ISO_U0SRR                                                                    0x18007888
#define ISO_U0SRR_reg_addr                                                           "0xB8007888"
#define ISO_U0SRR_reg                                                                0xB8007888
#define set_ISO_U0SRR_reg(data)   (*((volatile unsigned int*) ISO_U0SRR_reg)=data)
#define get_ISO_U0SRR_reg   (*((volatile unsigned int*) ISO_U0SRR_reg))
#define ISO_U0SRR_inst_adr                                                           "0x0022"
#define ISO_U0SRR_inst                                                               0x0022
#define ISO_U0SRR_XFR_shift                                                          (2)
#define ISO_U0SRR_XFR_mask                                                           (0x00000004)
#define ISO_U0SRR_XFR(data)                                                          (0x00000004&((data)<<2))
#define ISO_U0SRR_XFR_src(data)                                                      ((0x00000004&(data))>>2)
#define ISO_U0SRR_get_XFR(data)                                                      ((0x00000004&(data))>>2)
#define ISO_U0SRR_RFR_shift                                                          (1)
#define ISO_U0SRR_RFR_mask                                                           (0x00000002)
#define ISO_U0SRR_RFR(data)                                                          (0x00000002&((data)<<1))
#define ISO_U0SRR_RFR_src(data)                                                      ((0x00000002&(data))>>1)
#define ISO_U0SRR_get_RFR(data)                                                      ((0x00000002&(data))>>1)
#define ISO_U0SRR_UR_shift                                                           (0)
#define ISO_U0SRR_UR_mask                                                            (0x00000001)
#define ISO_U0SRR_UR(data)                                                           (0x00000001&((data)<<0))
#define ISO_U0SRR_UR_src(data)                                                       ((0x00000001&(data))>>0)
#define ISO_U0SRR_get_UR(data)                                                       ((0x00000001&(data))>>0)


#define ISO_U0SBCR                                                                   0x18007890
#define ISO_U0SBCR_reg_addr                                                          "0xB8007890"
#define ISO_U0SBCR_reg                                                               0xB8007890
#define set_ISO_U0SBCR_reg(data)   (*((volatile unsigned int*) ISO_U0SBCR_reg)=data)
#define get_ISO_U0SBCR_reg   (*((volatile unsigned int*) ISO_U0SBCR_reg))
#define ISO_U0SBCR_inst_adr                                                          "0x0024"
#define ISO_U0SBCR_inst                                                              0x0024
#define ISO_U0SBCR_SBCR_shift                                                        (0)
#define ISO_U0SBCR_SBCR_mask                                                         (0x00000001)
#define ISO_U0SBCR_SBCR(data)                                                        (0x00000001&((data)<<0))
#define ISO_U0SBCR_SBCR_src(data)                                                    ((0x00000001&(data))>>0)
#define ISO_U0SBCR_get_SBCR(data)                                                    ((0x00000001&(data))>>0)


#define ISO_U0SDMAM                                                                  0x18007894
#define ISO_U0SDMAM_reg_addr                                                         "0xB8007894"
#define ISO_U0SDMAM_reg                                                              0xB8007894
#define set_ISO_U0SDMAM_reg(data)   (*((volatile unsigned int*) ISO_U0SDMAM_reg)=data)
#define get_ISO_U0SDMAM_reg   (*((volatile unsigned int*) ISO_U0SDMAM_reg))
#define ISO_U0SDMAM_inst_adr                                                         "0x0025"
#define ISO_U0SDMAM_inst                                                             0x0025
#define ISO_U0SDMAM_SDMAM_shift                                                      (0)
#define ISO_U0SDMAM_SDMAM_mask                                                       (0x00000001)
#define ISO_U0SDMAM_SDMAM(data)                                                      (0x00000001&((data)<<0))
#define ISO_U0SDMAM_SDMAM_src(data)                                                  ((0x00000001&(data))>>0)
#define ISO_U0SDMAM_get_SDMAM(data)                                                  ((0x00000001&(data))>>0)


#define ISO_U0SFE                                                                    0x18007898
#define ISO_U0SFE_reg_addr                                                           "0xB8007898"
#define ISO_U0SFE_reg                                                                0xB8007898
#define set_ISO_U0SFE_reg(data)   (*((volatile unsigned int*) ISO_U0SFE_reg)=data)
#define get_ISO_U0SFE_reg   (*((volatile unsigned int*) ISO_U0SFE_reg))
#define ISO_U0SFE_inst_adr                                                           "0x0026"
#define ISO_U0SFE_inst                                                               0x0026
#define ISO_U0SFE_SFE_shift                                                          (0)
#define ISO_U0SFE_SFE_mask                                                           (0x00000001)
#define ISO_U0SFE_SFE(data)                                                          (0x00000001&((data)<<0))
#define ISO_U0SFE_SFE_src(data)                                                      ((0x00000001&(data))>>0)
#define ISO_U0SFE_get_SFE(data)                                                      ((0x00000001&(data))>>0)


#define ISO_U0SRT                                                                    0x1800789C
#define ISO_U0SRT_reg_addr                                                           "0xB800789C"
#define ISO_U0SRT_reg                                                                0xB800789C
#define set_ISO_U0SRT_reg(data)   (*((volatile unsigned int*) ISO_U0SRT_reg)=data)
#define get_ISO_U0SRT_reg   (*((volatile unsigned int*) ISO_U0SRT_reg))
#define ISO_U0SRT_inst_adr                                                           "0x0027"
#define ISO_U0SRT_inst                                                               0x0027
#define ISO_U0SRT_SRT_shift                                                          (0)
#define ISO_U0SRT_SRT_mask                                                           (0x00000003)
#define ISO_U0SRT_SRT(data)                                                          (0x00000003&((data)<<0))
#define ISO_U0SRT_SRT_src(data)                                                      ((0x00000003&(data))>>0)
#define ISO_U0SRT_get_SRT(data)                                                      ((0x00000003&(data))>>0)


#define ISO_U0STET                                                                   0x180078A0
#define ISO_U0STET_reg_addr                                                          "0xB80078A0"
#define ISO_U0STET_reg                                                               0xB80078A0
#define set_ISO_U0STET_reg(data)   (*((volatile unsigned int*) ISO_U0STET_reg)=data)
#define get_ISO_U0STET_reg   (*((volatile unsigned int*) ISO_U0STET_reg))
#define ISO_U0STET_inst_adr                                                          "0x0028"
#define ISO_U0STET_inst                                                              0x0028
#define ISO_U0STET_STET_shift                                                        (0)
#define ISO_U0STET_STET_mask                                                         (0x00000003)
#define ISO_U0STET_STET(data)                                                        (0x00000003&((data)<<0))
#define ISO_U0STET_STET_src(data)                                                    ((0x00000003&(data))>>0)
#define ISO_U0STET_get_STET(data)                                                    ((0x00000003&(data))>>0)


#define ISO_U0HTX                                                                    0x180078A4
#define ISO_U0HTX_reg_addr                                                           "0xB80078A4"
#define ISO_U0HTX_reg                                                                0xB80078A4
#define set_ISO_U0HTX_reg(data)   (*((volatile unsigned int*) ISO_U0HTX_reg)=data)
#define get_ISO_U0HTX_reg   (*((volatile unsigned int*) ISO_U0HTX_reg))
#define ISO_U0HTX_inst_adr                                                           "0x0029"
#define ISO_U0HTX_inst                                                               0x0029
#define ISO_U0HTX_HTX_shift                                                          (0)
#define ISO_U0HTX_HTX_mask                                                           (0x00000001)
#define ISO_U0HTX_HTX(data)                                                          (0x00000001&((data)<<0))
#define ISO_U0HTX_HTX_src(data)                                                      ((0x00000001&(data))>>0)
#define ISO_U0HTX_get_HTX(data)                                                      ((0x00000001&(data))>>0)


#define ISO_U0DMASA                                                                  0x180078A8
#define ISO_U0DMASA_reg_addr                                                         "0xB80078A8"
#define ISO_U0DMASA_reg                                                              0xB80078A8
#define set_ISO_U0DMASA_reg(data)   (*((volatile unsigned int*) ISO_U0DMASA_reg)=data)
#define get_ISO_U0DMASA_reg   (*((volatile unsigned int*) ISO_U0DMASA_reg))
#define ISO_U0DMASA_inst_adr                                                         "0x002A"
#define ISO_U0DMASA_inst                                                             0x002A
#define ISO_U0DMASA_DMASA_shift                                                      (0)
#define ISO_U0DMASA_DMASA_mask                                                       (0x00000001)
#define ISO_U0DMASA_DMASA(data)                                                      (0x00000001&((data)<<0))
#define ISO_U0DMASA_DMASA_src(data)                                                  ((0x00000001&(data))>>0)
#define ISO_U0DMASA_get_DMASA(data)                                                  ((0x00000001&(data))>>0)


#define ISO_U0CPR                                                                    0x180078F4
#define ISO_U0CPR_reg_addr                                                           "0xB80078F4"
#define ISO_U0CPR_reg                                                                0xB80078F4
#define set_ISO_U0CPR_reg(data)   (*((volatile unsigned int*) ISO_U0CPR_reg)=data)
#define get_ISO_U0CPR_reg   (*((volatile unsigned int*) ISO_U0CPR_reg))
#define ISO_U0CPR_inst_adr                                                           "0x003D"
#define ISO_U0CPR_inst                                                               0x003D
#define ISO_U0CPR_FIFO_MODE_shift                                                    (16)
#define ISO_U0CPR_FIFO_MODE_mask                                                     (0x00FF0000)
#define ISO_U0CPR_FIFO_MODE(data)                                                    (0x00FF0000&((data)<<16))
#define ISO_U0CPR_FIFO_MODE_src(data)                                                ((0x00FF0000&(data))>>16)
#define ISO_U0CPR_get_FIFO_MODE(data)                                                ((0x00FF0000&(data))>>16)
#define ISO_U0CPR_DMA_EXTRA_shift                                                    (13)
#define ISO_U0CPR_DMA_EXTRA_mask                                                     (0x00002000)
#define ISO_U0CPR_DMA_EXTRA(data)                                                    (0x00002000&((data)<<13))
#define ISO_U0CPR_DMA_EXTRA_src(data)                                                ((0x00002000&(data))>>13)
#define ISO_U0CPR_get_DMA_EXTRA(data)                                                ((0x00002000&(data))>>13)
#define ISO_U0CPR_UART_ADD_ENCODED_PARAMS_shift                                      (12)
#define ISO_U0CPR_UART_ADD_ENCODED_PARAMS_mask                                       (0x00001000)
#define ISO_U0CPR_UART_ADD_ENCODED_PARAMS(data)                                      (0x00001000&((data)<<12))
#define ISO_U0CPR_UART_ADD_ENCODED_PARAMS_src(data)                                  ((0x00001000&(data))>>12)
#define ISO_U0CPR_get_UART_ADD_ENCODED_PARAMS(data)                                  ((0x00001000&(data))>>12)
#define ISO_U0CPR_SHADOW_shift                                                       (11)
#define ISO_U0CPR_SHADOW_mask                                                        (0x00000800)
#define ISO_U0CPR_SHADOW(data)                                                       (0x00000800&((data)<<11))
#define ISO_U0CPR_SHADOW_src(data)                                                   ((0x00000800&(data))>>11)
#define ISO_U0CPR_get_SHADOW(data)                                                   ((0x00000800&(data))>>11)
#define ISO_U0CPR_FIFO_STAT_shift                                                    (10)
#define ISO_U0CPR_FIFO_STAT_mask                                                     (0x00000400)
#define ISO_U0CPR_FIFO_STAT(data)                                                    (0x00000400&((data)<<10))
#define ISO_U0CPR_FIFO_STAT_src(data)                                                ((0x00000400&(data))>>10)
#define ISO_U0CPR_get_FIFO_STAT(data)                                                ((0x00000400&(data))>>10)
#define ISO_U0CPR_FIFO_ACCESS_shift                                                  (9)
#define ISO_U0CPR_FIFO_ACCESS_mask                                                   (0x00000200)
#define ISO_U0CPR_FIFO_ACCESS(data)                                                  (0x00000200&((data)<<9))
#define ISO_U0CPR_FIFO_ACCESS_src(data)                                              ((0x00000200&(data))>>9)
#define ISO_U0CPR_get_FIFO_ACCESS(data)                                              ((0x00000200&(data))>>9)
#define ISO_U0CPR_ADDITIONAL_FEAT_shift                                              (8)
#define ISO_U0CPR_ADDITIONAL_FEAT_mask                                               (0x00000100)
#define ISO_U0CPR_ADDITIONAL_FEAT(data)                                              (0x00000100&((data)<<8))
#define ISO_U0CPR_ADDITIONAL_FEAT_src(data)                                          ((0x00000100&(data))>>8)
#define ISO_U0CPR_get_ADDITIONAL_FEAT(data)                                          ((0x00000100&(data))>>8)
#define ISO_U0CPR_SIR_LP_MODE_shift                                                  (7)
#define ISO_U0CPR_SIR_LP_MODE_mask                                                   (0x00000080)
#define ISO_U0CPR_SIR_LP_MODE(data)                                                  (0x00000080&((data)<<7))
#define ISO_U0CPR_SIR_LP_MODE_src(data)                                              ((0x00000080&(data))>>7)
#define ISO_U0CPR_get_SIR_LP_MODE(data)                                              ((0x00000080&(data))>>7)
#define ISO_U0CPR_SIR_MODE_shift                                                     (6)
#define ISO_U0CPR_SIR_MODE_mask                                                      (0x00000040)
#define ISO_U0CPR_SIR_MODE(data)                                                     (0x00000040&((data)<<6))
#define ISO_U0CPR_SIR_MODE_src(data)                                                 ((0x00000040&(data))>>6)
#define ISO_U0CPR_get_SIR_MODE(data)                                                 ((0x00000040&(data))>>6)
#define ISO_U0CPR_THRE_MODE_shift                                                    (5)
#define ISO_U0CPR_THRE_MODE_mask                                                     (0x00000020)
#define ISO_U0CPR_THRE_MODE(data)                                                    (0x00000020&((data)<<5))
#define ISO_U0CPR_THRE_MODE_src(data)                                                ((0x00000020&(data))>>5)
#define ISO_U0CPR_get_THRE_MODE(data)                                                ((0x00000020&(data))>>5)
#define ISO_U0CPR_AFCE_MODE_shift                                                    (4)
#define ISO_U0CPR_AFCE_MODE_mask                                                     (0x00000010)
#define ISO_U0CPR_AFCE_MODE(data)                                                    (0x00000010&((data)<<4))
#define ISO_U0CPR_AFCE_MODE_src(data)                                                ((0x00000010&(data))>>4)
#define ISO_U0CPR_get_AFCE_MODE(data)                                                ((0x00000010&(data))>>4)
#define ISO_U0CPR_APB_DATA_WIDTH_shift                                               (0)
#define ISO_U0CPR_APB_DATA_WIDTH_mask                                                (0x00000003)
#define ISO_U0CPR_APB_DATA_WIDTH(data)                                               (0x00000003&((data)<<0))
#define ISO_U0CPR_APB_DATA_WIDTH_src(data)                                           ((0x00000003&(data))>>0)
#define ISO_U0CPR_get_APB_DATA_WIDTH(data)                                           ((0x00000003&(data))>>0)


#define ISO_U0UCV                                                                    0x180078F8
#define ISO_U0UCV_reg_addr                                                           "0xB80078F8"
#define ISO_U0UCV_reg                                                                0xB80078F8
#define set_ISO_U0UCV_reg(data)   (*((volatile unsigned int*) ISO_U0UCV_reg)=data)
#define get_ISO_U0UCV_reg   (*((volatile unsigned int*) ISO_U0UCV_reg))
#define ISO_U0UCV_inst_adr                                                           "0x003E"
#define ISO_U0UCV_inst                                                               0x003E
#define ISO_U0UCV_UCV_shift                                                          (0)
#define ISO_U0UCV_UCV_mask                                                           (0xFFFFFFFF)
#define ISO_U0UCV_UCV(data)                                                          (0xFFFFFFFF&((data)<<0))
#define ISO_U0UCV_UCV_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define ISO_U0UCV_get_UCV(data)                                                      ((0xFFFFFFFF&(data))>>0)


#define ISO_U0CTR                                                                    0x180078FC
#define ISO_U0CTR_reg_addr                                                           "0xB80078FC"
#define ISO_U0CTR_reg                                                                0xB80078FC
#define set_ISO_U0CTR_reg(data)   (*((volatile unsigned int*) ISO_U0CTR_reg)=data)
#define get_ISO_U0CTR_reg   (*((volatile unsigned int*) ISO_U0CTR_reg))
#define ISO_U0CTR_inst_adr                                                           "0x003F"
#define ISO_U0CTR_inst                                                               0x003F
#define ISO_U0CTR_CTR_shift                                                          (0)
#define ISO_U0CTR_CTR_mask                                                           (0xFFFFFFFF)
#define ISO_U0CTR_CTR(data)                                                          (0xFFFFFFFF&((data)<<0))
#define ISO_U0CTR_CTR_src(data)                                                      ((0xFFFFFFFF&(data))>>0)
#define ISO_U0CTR_get_CTR(data)                                                      ((0xFFFFFFFF&(data))>>0)


#endif
