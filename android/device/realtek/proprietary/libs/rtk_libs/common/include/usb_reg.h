/**************************************************************
// Spec Version                  : 0.0.1
// Parser Version                : MAR_Parser_1.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Register_Name_reg
// Firmware Header Generate Date : 2010/4/23
***************************************************************/


#ifndef _USB_REG_H_INCLUDED_
#define _USB_REG_H_INCLUDED_
#ifdef  _USB_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     sram_debug_sel:1;
unsigned int     sram_debug_mode:1;
unsigned int     dev_mode:1;
unsigned int     dbus_multi_req_disable:1;
unsigned int     mdio_phy_sel:1;
}WRAP_CTR;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     device_int:1;
unsigned int     host_int:1;
}GNR_INT;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     reset_utmi_p1:1;
unsigned int     reset_utmi_p0:1;
}USB2_PHY_UTMI;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     reset_pipe3_p1:1;
unsigned int     reset_pipe3_p0:1;
}USB3_PHY_PIPE;

typedef struct 
{
unsigned int     data:16;
unsigned int     phy_addr:3;
unsigned int     phy_reg_addr:5;
unsigned int     mdio_busy:1;
unsigned int     mdio_st:2;
unsigned int     mdio_rdy:1;
unsigned int     mclk_rate:2;
unsigned int     mdio_srst:1;
unsigned int     mdio_rdwr:1;
}MDIO_CTR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     p0_vstatus_out:8;
}VSTATUS0_OUT;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     p1_vstatus_out:8;
}VSTATUS1_OUT;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     test_rst_p0:1;
unsigned int     test_en_p0:1;
}SLP_BACK0_EN;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     test_speed_p0:2;
unsigned int     test_seed_p0:8;
unsigned int     test_psl_p0:2;
}SLP_BACK0_CTR;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     test_fail_p0:1;
unsigned int     test_done_p0:1;
}SLP_BACK0_ST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     test_rst_p1:1;
unsigned int     test_en_p1:1;
}SLP_BACK1_EN;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     test_speed_p1:2;
unsigned int     test_seed_p1:8;
unsigned int     test_ps_p1:2;
}SLP_BACK1_CTR;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     test_fail_p1:1;
unsigned int     test_done_pl:1;
}SLP_BACK1_ST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     p0_usb2phy_slb_hs:1;
unsigned int     p0_usb2phy_force_slb:1;
}PHY_SLB0_EN;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     p0_usb2phy_slb_fail:1;
unsigned int     p0_usb2phy_slb_done:1;
}PHY_SLB0_ST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     p1_usb2phy_slb_hs:1;
unsigned int     p1_usb2phy_force_slb:1;
}PHY_SLB1_EN;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     p1_usb2phy_slb_fail:1;
unsigned int     p1_usb2phy_slb_done:1;
}PHY_SLB1_ST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     p1_suspend_r:1;
unsigned int     p0_suspend_r:1;
}USB2_SPD_CTR;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     dbg_sel1:6;
unsigned int     dbg_sel0:6;
unsigned int     dbg_en:1;
}USB_DBG;

typedef struct 
{
unsigned int     reg1:16;
unsigned int     reg0:16;
}USB_SCTCH;

typedef struct 
{
unsigned int     test_reg:32;
}USB_TMP;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     host_msi_enable:1;
unsigned int     hub_port_over_current:4;
unsigned int     hub_port_perm_attach:4;
unsigned int     host_num_u2_port:4;
unsigned int     host_num_u3_port:4;
}HMAC_CTR0;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     host_fladj_125mhz:12;
unsigned int     host_fladj_240mhz:9;
unsigned int     host_fladj_30mhz:6;
}HMAC_CTR1;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     utmiotg_vbusvalid:1;
unsigned int     dev_usb_outep_pkt_buff:16;
}DMAC_CTR0;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     dev_fladj_125mhz:12;
unsigned int     dev_fladj_240mhz:9;
unsigned int     dev_fladj_30mhz:6;
}DMAC_CTR1;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     p1_DmPulldown:1;
unsigned int     p1_DpPulldown:1;
unsigned int     p1_IDPULLUP:1;
unsigned int     reserved_1:5;
unsigned int     p1_DmPulldown_sel:1;
unsigned int     p1_DpPulldown_sel:1;
unsigned int     p1_IDPULLUP_sel:1;
}USB2_PHY;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     error_st:1;
unsigned int     done_st:1;
unsigned int     reserved_1:15;
unsigned int     go_ct:1;
}RAM_CTR;

typedef struct 
{
unsigned int     sram_addr:32;
}RAM_ADDR;

typedef struct 
{
unsigned int     ram_wdata:32;
}RAM_WDATA;

typedef struct 
{
unsigned int     ram_rdata:32;
}RAM_RDATA;

#endif

#define WRAP_CTR                                                                     0x18013000
#define WRAP_CTR_reg_addr                                                            "0xB8013000"
#define WRAP_CTR_reg                                                                 0xB8013000
#define set_WRAP_CTR_reg(data)   (*((volatile unsigned int*) WRAP_CTR_reg)=data)
#define get_WRAP_CTR_reg   (*((volatile unsigned int*) WRAP_CTR_reg))
#define WRAP_CTR_inst_adr                                                            "0x0000"
#define WRAP_CTR_inst                                                                0x0000
#define WRAP_CTR_sram_debug_sel_shift                                                (4)
#define WRAP_CTR_sram_debug_sel_mask                                                 (0x00000010)
#define WRAP_CTR_sram_debug_sel(data)                                                (0x00000010&((data)<<4))
#define WRAP_CTR_sram_debug_sel_src(data)                                            ((0x00000010&(data))>>4)
#define WRAP_CTR_get_sram_debug_sel(data)                                            ((0x00000010&(data))>>4)
#define WRAP_CTR_sram_debug_mode_shift                                               (3)
#define WRAP_CTR_sram_debug_mode_mask                                                (0x00000008)
#define WRAP_CTR_sram_debug_mode(data)                                               (0x00000008&((data)<<3))
#define WRAP_CTR_sram_debug_mode_src(data)                                           ((0x00000008&(data))>>3)
#define WRAP_CTR_get_sram_debug_mode(data)                                           ((0x00000008&(data))>>3)
#define WRAP_CTR_dev_mode_shift                                                      (2)
#define WRAP_CTR_dev_mode_mask                                                       (0x00000004)
#define WRAP_CTR_dev_mode(data)                                                      (0x00000004&((data)<<2))
#define WRAP_CTR_dev_mode_src(data)                                                  ((0x00000004&(data))>>2)
#define WRAP_CTR_get_dev_mode(data)                                                  ((0x00000004&(data))>>2)
#define WRAP_CTR_dbus_multi_req_disable_shift                                        (1)
#define WRAP_CTR_dbus_multi_req_disable_mask                                         (0x00000002)
#define WRAP_CTR_dbus_multi_req_disable(data)                                        (0x00000002&((data)<<1))
#define WRAP_CTR_dbus_multi_req_disable_src(data)                                    ((0x00000002&(data))>>1)
#define WRAP_CTR_get_dbus_multi_req_disable(data)                                    ((0x00000002&(data))>>1)
#define WRAP_CTR_mdio_phy_sel_shift                                                  (0)
#define WRAP_CTR_mdio_phy_sel_mask                                                   (0x00000001)
#define WRAP_CTR_mdio_phy_sel(data)                                                  (0x00000001&((data)<<0))
#define WRAP_CTR_mdio_phy_sel_src(data)                                              ((0x00000001&(data))>>0)
#define WRAP_CTR_get_mdio_phy_sel(data)                                              ((0x00000001&(data))>>0)


#define GNR_INT                                                                      0x18013004
#define GNR_INT_reg_addr                                                             "0xB8013004"
#define GNR_INT_reg                                                                  0xB8013004
#define set_GNR_INT_reg(data)   (*((volatile unsigned int*) GNR_INT_reg)=data)
#define get_GNR_INT_reg   (*((volatile unsigned int*) GNR_INT_reg))
#define GNR_INT_inst_adr                                                             "0x0001"
#define GNR_INT_inst                                                                 0x0001
#define GNR_INT_device_int_shift                                                     (1)
#define GNR_INT_device_int_mask                                                      (0x00000002)
#define GNR_INT_device_int(data)                                                     (0x00000002&((data)<<1))
#define GNR_INT_device_int_src(data)                                                 ((0x00000002&(data))>>1)
#define GNR_INT_get_device_int(data)                                                 ((0x00000002&(data))>>1)
#define GNR_INT_host_int_shift                                                       (0)
#define GNR_INT_host_int_mask                                                        (0x00000001)
#define GNR_INT_host_int(data)                                                       (0x00000001&((data)<<0))
#define GNR_INT_host_int_src(data)                                                   ((0x00000001&(data))>>0)
#define GNR_INT_get_host_int(data)                                                   ((0x00000001&(data))>>0)


#define USB2_PHY_UTMI                                                                0x18013008
#define USB2_PHY_UTMI_reg_addr                                                       "0xB8013008"
#define USB2_PHY_UTMI_reg                                                            0xB8013008
#define set_USB2_PHY_UTMI_reg(data)   (*((volatile unsigned int*) USB2_PHY_UTMI_reg)=data)
#define get_USB2_PHY_UTMI_reg   (*((volatile unsigned int*) USB2_PHY_UTMI_reg))
#define USB2_PHY_UTMI_inst_adr                                                       "0x0002"
#define USB2_PHY_UTMI_inst                                                           0x0002
#define USB2_PHY_UTMI_reset_utmi_p1_shift                                            (1)
#define USB2_PHY_UTMI_reset_utmi_p1_mask                                             (0x00000002)
#define USB2_PHY_UTMI_reset_utmi_p1(data)                                            (0x00000002&((data)<<1))
#define USB2_PHY_UTMI_reset_utmi_p1_src(data)                                        ((0x00000002&(data))>>1)
#define USB2_PHY_UTMI_get_reset_utmi_p1(data)                                        ((0x00000002&(data))>>1)
#define USB2_PHY_UTMI_reset_utmi_p0_shift                                            (0)
#define USB2_PHY_UTMI_reset_utmi_p0_mask                                             (0x00000001)
#define USB2_PHY_UTMI_reset_utmi_p0(data)                                            (0x00000001&((data)<<0))
#define USB2_PHY_UTMI_reset_utmi_p0_src(data)                                        ((0x00000001&(data))>>0)
#define USB2_PHY_UTMI_get_reset_utmi_p0(data)                                        ((0x00000001&(data))>>0)


#define USB3_PHY_PIPE                                                                0x1801300c
#define USB3_PHY_PIPE_reg_addr                                                       "0xB801300C"
#define USB3_PHY_PIPE_reg                                                            0xB801300C
#define set_USB3_PHY_PIPE_reg(data)   (*((volatile unsigned int*) USB3_PHY_PIPE_reg)=data)
#define get_USB3_PHY_PIPE_reg   (*((volatile unsigned int*) USB3_PHY_PIPE_reg))
#define USB3_PHY_PIPE_inst_adr                                                       "0x0003"
#define USB3_PHY_PIPE_inst                                                           0x0003
#define USB3_PHY_PIPE_reset_pipe3_p1_shift                                           (1)
#define USB3_PHY_PIPE_reset_pipe3_p1_mask                                            (0x00000002)
#define USB3_PHY_PIPE_reset_pipe3_p1(data)                                           (0x00000002&((data)<<1))
#define USB3_PHY_PIPE_reset_pipe3_p1_src(data)                                       ((0x00000002&(data))>>1)
#define USB3_PHY_PIPE_get_reset_pipe3_p1(data)                                       ((0x00000002&(data))>>1)
#define USB3_PHY_PIPE_reset_pipe3_p0_shift                                           (0)
#define USB3_PHY_PIPE_reset_pipe3_p0_mask                                            (0x00000001)
#define USB3_PHY_PIPE_reset_pipe3_p0(data)                                           (0x00000001&((data)<<0))
#define USB3_PHY_PIPE_reset_pipe3_p0_src(data)                                       ((0x00000001&(data))>>0)
#define USB3_PHY_PIPE_get_reset_pipe3_p0(data)                                       ((0x00000001&(data))>>0)


#define MDIO_CTR                                                                     0x18013010
#define MDIO_CTR_reg_addr                                                            "0xB8013010"
#define MDIO_CTR_reg                                                                 0xB8013010
#define set_MDIO_CTR_reg(data)   (*((volatile unsigned int*) MDIO_CTR_reg)=data)
#define get_MDIO_CTR_reg   (*((volatile unsigned int*) MDIO_CTR_reg))
#define MDIO_CTR_inst_adr                                                            "0x0004"
#define MDIO_CTR_inst                                                                0x0004
#define MDIO_CTR_data_shift                                                          (16)
#define MDIO_CTR_data_mask                                                           (0xFFFF0000)
#define MDIO_CTR_data(data)                                                          (0xFFFF0000&((data)<<16))
#define MDIO_CTR_data_src(data)                                                      ((0xFFFF0000&(data))>>16)
#define MDIO_CTR_get_data(data)                                                      ((0xFFFF0000&(data))>>16)
#define MDIO_CTR_phy_addr_shift                                                      (13)
#define MDIO_CTR_phy_addr_mask                                                       (0x0000E000)
#define MDIO_CTR_phy_addr(data)                                                      (0x0000E000&((data)<<13))
#define MDIO_CTR_phy_addr_src(data)                                                  ((0x0000E000&(data))>>13)
#define MDIO_CTR_get_phy_addr(data)                                                  ((0x0000E000&(data))>>13)
#define MDIO_CTR_phy_reg_addr_shift                                                  (8)
#define MDIO_CTR_phy_reg_addr_mask                                                   (0x00001F00)
#define MDIO_CTR_phy_reg_addr(data)                                                  (0x00001F00&((data)<<8))
#define MDIO_CTR_phy_reg_addr_src(data)                                              ((0x00001F00&(data))>>8)
#define MDIO_CTR_get_phy_reg_addr(data)                                              ((0x00001F00&(data))>>8)
#define MDIO_CTR_mdio_busy_shift                                                     (7)
#define MDIO_CTR_mdio_busy_mask                                                      (0x00000080)
#define MDIO_CTR_mdio_busy(data)                                                     (0x00000080&((data)<<7))
#define MDIO_CTR_mdio_busy_src(data)                                                 ((0x00000080&(data))>>7)
#define MDIO_CTR_get_mdio_busy(data)                                                 ((0x00000080&(data))>>7)
#define MDIO_CTR_mdio_st_shift                                                       (5)
#define MDIO_CTR_mdio_st_mask                                                        (0x00000060)
#define MDIO_CTR_mdio_st(data)                                                       (0x00000060&((data)<<5))
#define MDIO_CTR_mdio_st_src(data)                                                   ((0x00000060&(data))>>5)
#define MDIO_CTR_get_mdio_st(data)                                                   ((0x00000060&(data))>>5)
#define MDIO_CTR_mdio_rdy_shift                                                      (4)
#define MDIO_CTR_mdio_rdy_mask                                                       (0x00000010)
#define MDIO_CTR_mdio_rdy(data)                                                      (0x00000010&((data)<<4))
#define MDIO_CTR_mdio_rdy_src(data)                                                  ((0x00000010&(data))>>4)
#define MDIO_CTR_get_mdio_rdy(data)                                                  ((0x00000010&(data))>>4)
#define MDIO_CTR_mclk_rate_shift                                                     (2)
#define MDIO_CTR_mclk_rate_mask                                                      (0x0000000C)
#define MDIO_CTR_mclk_rate(data)                                                     (0x0000000C&((data)<<2))
#define MDIO_CTR_mclk_rate_src(data)                                                 ((0x0000000C&(data))>>2)
#define MDIO_CTR_get_mclk_rate(data)                                                 ((0x0000000C&(data))>>2)
#define MDIO_CTR_mdio_srst_shift                                                     (1)
#define MDIO_CTR_mdio_srst_mask                                                      (0x00000002)
#define MDIO_CTR_mdio_srst(data)                                                     (0x00000002&((data)<<1))
#define MDIO_CTR_mdio_srst_src(data)                                                 ((0x00000002&(data))>>1)
#define MDIO_CTR_get_mdio_srst(data)                                                 ((0x00000002&(data))>>1)
#define MDIO_CTR_mdio_rdwr_shift                                                     (0)
#define MDIO_CTR_mdio_rdwr_mask                                                      (0x00000001)
#define MDIO_CTR_mdio_rdwr(data)                                                     (0x00000001&((data)<<0))
#define MDIO_CTR_mdio_rdwr_src(data)                                                 ((0x00000001&(data))>>0)
#define MDIO_CTR_get_mdio_rdwr(data)                                                 ((0x00000001&(data))>>0)


#define VSTATUS0_OUT                                                                 0x18013014
#define VSTATUS0_OUT_reg_addr                                                        "0xB8013014"
#define VSTATUS0_OUT_reg                                                             0xB8013014
#define set_VSTATUS0_OUT_reg(data)   (*((volatile unsigned int*) VSTATUS0_OUT_reg)=data)
#define get_VSTATUS0_OUT_reg   (*((volatile unsigned int*) VSTATUS0_OUT_reg))
#define VSTATUS0_OUT_inst_adr                                                        "0x0005"
#define VSTATUS0_OUT_inst                                                            0x0005
#define VSTATUS0_OUT_p0_vstatus_out_shift                                            (0)
#define VSTATUS0_OUT_p0_vstatus_out_mask                                             (0x000000FF)
#define VSTATUS0_OUT_p0_vstatus_out(data)                                            (0x000000FF&((data)<<0))
#define VSTATUS0_OUT_p0_vstatus_out_src(data)                                        ((0x000000FF&(data))>>0)
#define VSTATUS0_OUT_get_p0_vstatus_out(data)                                        ((0x000000FF&(data))>>0)


#define VSTATUS1_OUT                                                                 0x18013018
#define VSTATUS1_OUT_reg_addr                                                        "0xB8013018"
#define VSTATUS1_OUT_reg                                                             0xB8013018
#define set_VSTATUS1_OUT_reg(data)   (*((volatile unsigned int*) VSTATUS1_OUT_reg)=data)
#define get_VSTATUS1_OUT_reg   (*((volatile unsigned int*) VSTATUS1_OUT_reg))
#define VSTATUS1_OUT_inst_adr                                                        "0x0006"
#define VSTATUS1_OUT_inst                                                            0x0006
#define VSTATUS1_OUT_p1_vstatus_out_shift                                            (0)
#define VSTATUS1_OUT_p1_vstatus_out_mask                                             (0x000000FF)
#define VSTATUS1_OUT_p1_vstatus_out(data)                                            (0x000000FF&((data)<<0))
#define VSTATUS1_OUT_p1_vstatus_out_src(data)                                        ((0x000000FF&(data))>>0)
#define VSTATUS1_OUT_get_p1_vstatus_out(data)                                        ((0x000000FF&(data))>>0)


#define SLP_BACK0_EN                                                                 0x1801301c
#define SLP_BACK0_EN_reg_addr                                                        "0xB801301C"
#define SLP_BACK0_EN_reg                                                             0xB801301C
#define set_SLP_BACK0_EN_reg(data)   (*((volatile unsigned int*) SLP_BACK0_EN_reg)=data)
#define get_SLP_BACK0_EN_reg   (*((volatile unsigned int*) SLP_BACK0_EN_reg))
#define SLP_BACK0_EN_inst_adr                                                        "0x0007"
#define SLP_BACK0_EN_inst                                                            0x0007
#define SLP_BACK0_EN_test_rst_p0_shift                                               (1)
#define SLP_BACK0_EN_test_rst_p0_mask                                                (0x00000002)
#define SLP_BACK0_EN_test_rst_p0(data)                                               (0x00000002&((data)<<1))
#define SLP_BACK0_EN_test_rst_p0_src(data)                                           ((0x00000002&(data))>>1)
#define SLP_BACK0_EN_get_test_rst_p0(data)                                           ((0x00000002&(data))>>1)
#define SLP_BACK0_EN_test_en_p0_shift                                                (0)
#define SLP_BACK0_EN_test_en_p0_mask                                                 (0x00000001)
#define SLP_BACK0_EN_test_en_p0(data)                                                (0x00000001&((data)<<0))
#define SLP_BACK0_EN_test_en_p0_src(data)                                            ((0x00000001&(data))>>0)
#define SLP_BACK0_EN_get_test_en_p0(data)                                            ((0x00000001&(data))>>0)


#define SLP_BACK0_CTR                                                                0x18013020
#define SLP_BACK0_CTR_reg_addr                                                       "0xB8013020"
#define SLP_BACK0_CTR_reg                                                            0xB8013020
#define set_SLP_BACK0_CTR_reg(data)   (*((volatile unsigned int*) SLP_BACK0_CTR_reg)=data)
#define get_SLP_BACK0_CTR_reg   (*((volatile unsigned int*) SLP_BACK0_CTR_reg))
#define SLP_BACK0_CTR_inst_adr                                                       "0x0008"
#define SLP_BACK0_CTR_inst                                                           0x0008
#define SLP_BACK0_CTR_test_speed_p0_shift                                            (10)
#define SLP_BACK0_CTR_test_speed_p0_mask                                             (0x00000C00)
#define SLP_BACK0_CTR_test_speed_p0(data)                                            (0x00000C00&((data)<<10))
#define SLP_BACK0_CTR_test_speed_p0_src(data)                                        ((0x00000C00&(data))>>10)
#define SLP_BACK0_CTR_get_test_speed_p0(data)                                        ((0x00000C00&(data))>>10)
#define SLP_BACK0_CTR_test_seed_p0_shift                                             (2)
#define SLP_BACK0_CTR_test_seed_p0_mask                                              (0x000003FC)
#define SLP_BACK0_CTR_test_seed_p0(data)                                             (0x000003FC&((data)<<2))
#define SLP_BACK0_CTR_test_seed_p0_src(data)                                         ((0x000003FC&(data))>>2)
#define SLP_BACK0_CTR_get_test_seed_p0(data)                                         ((0x000003FC&(data))>>2)
#define SLP_BACK0_CTR_test_psl_p0_shift                                              (0)
#define SLP_BACK0_CTR_test_psl_p0_mask                                               (0x00000003)
#define SLP_BACK0_CTR_test_psl_p0(data)                                              (0x00000003&((data)<<0))
#define SLP_BACK0_CTR_test_psl_p0_src(data)                                          ((0x00000003&(data))>>0)
#define SLP_BACK0_CTR_get_test_psl_p0(data)                                          ((0x00000003&(data))>>0)


#define SLP_BACK0_ST                                                                 0x18013024
#define SLP_BACK0_ST_reg_addr                                                        "0xB8013024"
#define SLP_BACK0_ST_reg                                                             0xB8013024
#define set_SLP_BACK0_ST_reg(data)   (*((volatile unsigned int*) SLP_BACK0_ST_reg)=data)
#define get_SLP_BACK0_ST_reg   (*((volatile unsigned int*) SLP_BACK0_ST_reg))
#define SLP_BACK0_ST_inst_adr                                                        "0x0009"
#define SLP_BACK0_ST_inst                                                            0x0009
#define SLP_BACK0_ST_test_fail_p0_shift                                              (1)
#define SLP_BACK0_ST_test_fail_p0_mask                                               (0x00000002)
#define SLP_BACK0_ST_test_fail_p0(data)                                              (0x00000002&((data)<<1))
#define SLP_BACK0_ST_test_fail_p0_src(data)                                          ((0x00000002&(data))>>1)
#define SLP_BACK0_ST_get_test_fail_p0(data)                                          ((0x00000002&(data))>>1)
#define SLP_BACK0_ST_test_done_p0_shift                                              (0)
#define SLP_BACK0_ST_test_done_p0_mask                                               (0x00000001)
#define SLP_BACK0_ST_test_done_p0(data)                                              (0x00000001&((data)<<0))
#define SLP_BACK0_ST_test_done_p0_src(data)                                          ((0x00000001&(data))>>0)
#define SLP_BACK0_ST_get_test_done_p0(data)                                          ((0x00000001&(data))>>0)


#define SLP_BACK1_EN                                                                 0x18013028
#define SLP_BACK1_EN_reg_addr                                                        "0xB8013028"
#define SLP_BACK1_EN_reg                                                             0xB8013028
#define set_SLP_BACK1_EN_reg(data)   (*((volatile unsigned int*) SLP_BACK1_EN_reg)=data)
#define get_SLP_BACK1_EN_reg   (*((volatile unsigned int*) SLP_BACK1_EN_reg))
#define SLP_BACK1_EN_inst_adr                                                        "0x000A"
#define SLP_BACK1_EN_inst                                                            0x000A
#define SLP_BACK1_EN_test_rst_p1_shift                                               (1)
#define SLP_BACK1_EN_test_rst_p1_mask                                                (0x00000002)
#define SLP_BACK1_EN_test_rst_p1(data)                                               (0x00000002&((data)<<1))
#define SLP_BACK1_EN_test_rst_p1_src(data)                                           ((0x00000002&(data))>>1)
#define SLP_BACK1_EN_get_test_rst_p1(data)                                           ((0x00000002&(data))>>1)
#define SLP_BACK1_EN_test_en_p1_shift                                                (0)
#define SLP_BACK1_EN_test_en_p1_mask                                                 (0x00000001)
#define SLP_BACK1_EN_test_en_p1(data)                                                (0x00000001&((data)<<0))
#define SLP_BACK1_EN_test_en_p1_src(data)                                            ((0x00000001&(data))>>0)
#define SLP_BACK1_EN_get_test_en_p1(data)                                            ((0x00000001&(data))>>0)


#define SLP_BACK1_CTR                                                                0x1801302c
#define SLP_BACK1_CTR_reg_addr                                                       "0xB801302C"
#define SLP_BACK1_CTR_reg                                                            0xB801302C
#define set_SLP_BACK1_CTR_reg(data)   (*((volatile unsigned int*) SLP_BACK1_CTR_reg)=data)
#define get_SLP_BACK1_CTR_reg   (*((volatile unsigned int*) SLP_BACK1_CTR_reg))
#define SLP_BACK1_CTR_inst_adr                                                       "0x000B"
#define SLP_BACK1_CTR_inst                                                           0x000B
#define SLP_BACK1_CTR_test_speed_p1_shift                                            (10)
#define SLP_BACK1_CTR_test_speed_p1_mask                                             (0x00000C00)
#define SLP_BACK1_CTR_test_speed_p1(data)                                            (0x00000C00&((data)<<10))
#define SLP_BACK1_CTR_test_speed_p1_src(data)                                        ((0x00000C00&(data))>>10)
#define SLP_BACK1_CTR_get_test_speed_p1(data)                                        ((0x00000C00&(data))>>10)
#define SLP_BACK1_CTR_test_seed_p1_shift                                             (2)
#define SLP_BACK1_CTR_test_seed_p1_mask                                              (0x000003FC)
#define SLP_BACK1_CTR_test_seed_p1(data)                                             (0x000003FC&((data)<<2))
#define SLP_BACK1_CTR_test_seed_p1_src(data)                                         ((0x000003FC&(data))>>2)
#define SLP_BACK1_CTR_get_test_seed_p1(data)                                         ((0x000003FC&(data))>>2)
#define SLP_BACK1_CTR_test_ps_p1_shift                                               (0)
#define SLP_BACK1_CTR_test_ps_p1_mask                                                (0x00000003)
#define SLP_BACK1_CTR_test_ps_p1(data)                                               (0x00000003&((data)<<0))
#define SLP_BACK1_CTR_test_ps_p1_src(data)                                           ((0x00000003&(data))>>0)
#define SLP_BACK1_CTR_get_test_ps_p1(data)                                           ((0x00000003&(data))>>0)


#define SLP_BACK1_ST                                                                 0x18013030
#define SLP_BACK1_ST_reg_addr                                                        "0xB8013030"
#define SLP_BACK1_ST_reg                                                             0xB8013030
#define set_SLP_BACK1_ST_reg(data)   (*((volatile unsigned int*) SLP_BACK1_ST_reg)=data)
#define get_SLP_BACK1_ST_reg   (*((volatile unsigned int*) SLP_BACK1_ST_reg))
#define SLP_BACK1_ST_inst_adr                                                        "0x000C"
#define SLP_BACK1_ST_inst                                                            0x000C
#define SLP_BACK1_ST_test_fail_p1_shift                                              (1)
#define SLP_BACK1_ST_test_fail_p1_mask                                               (0x00000002)
#define SLP_BACK1_ST_test_fail_p1(data)                                              (0x00000002&((data)<<1))
#define SLP_BACK1_ST_test_fail_p1_src(data)                                          ((0x00000002&(data))>>1)
#define SLP_BACK1_ST_get_test_fail_p1(data)                                          ((0x00000002&(data))>>1)
#define SLP_BACK1_ST_test_done_pl_shift                                              (0)
#define SLP_BACK1_ST_test_done_pl_mask                                               (0x00000001)
#define SLP_BACK1_ST_test_done_pl(data)                                              (0x00000001&((data)<<0))
#define SLP_BACK1_ST_test_done_pl_src(data)                                          ((0x00000001&(data))>>0)
#define SLP_BACK1_ST_get_test_done_pl(data)                                          ((0x00000001&(data))>>0)


#define PHY_SLB0_EN                                                                  0x18013034
#define PHY_SLB0_EN_reg_addr                                                         "0xB8013034"
#define PHY_SLB0_EN_reg                                                              0xB8013034
#define set_PHY_SLB0_EN_reg(data)   (*((volatile unsigned int*) PHY_SLB0_EN_reg)=data)
#define get_PHY_SLB0_EN_reg   (*((volatile unsigned int*) PHY_SLB0_EN_reg))
#define PHY_SLB0_EN_inst_adr                                                         "0x000D"
#define PHY_SLB0_EN_inst                                                             0x000D
#define PHY_SLB0_EN_p0_usb2phy_slb_hs_shift                                          (1)
#define PHY_SLB0_EN_p0_usb2phy_slb_hs_mask                                           (0x00000002)
#define PHY_SLB0_EN_p0_usb2phy_slb_hs(data)                                          (0x00000002&((data)<<1))
#define PHY_SLB0_EN_p0_usb2phy_slb_hs_src(data)                                      ((0x00000002&(data))>>1)
#define PHY_SLB0_EN_get_p0_usb2phy_slb_hs(data)                                      ((0x00000002&(data))>>1)
#define PHY_SLB0_EN_p0_usb2phy_force_slb_shift                                       (0)
#define PHY_SLB0_EN_p0_usb2phy_force_slb_mask                                        (0x00000001)
#define PHY_SLB0_EN_p0_usb2phy_force_slb(data)                                       (0x00000001&((data)<<0))
#define PHY_SLB0_EN_p0_usb2phy_force_slb_src(data)                                   ((0x00000001&(data))>>0)
#define PHY_SLB0_EN_get_p0_usb2phy_force_slb(data)                                   ((0x00000001&(data))>>0)


#define PHY_SLB0_ST                                                                  0x18013038
#define PHY_SLB0_ST_reg_addr                                                         "0xB8013038"
#define PHY_SLB0_ST_reg                                                              0xB8013038
#define set_PHY_SLB0_ST_reg(data)   (*((volatile unsigned int*) PHY_SLB0_ST_reg)=data)
#define get_PHY_SLB0_ST_reg   (*((volatile unsigned int*) PHY_SLB0_ST_reg))
#define PHY_SLB0_ST_inst_adr                                                         "0x000E"
#define PHY_SLB0_ST_inst                                                             0x000E
#define PHY_SLB0_ST_p0_usb2phy_slb_fail_shift                                        (1)
#define PHY_SLB0_ST_p0_usb2phy_slb_fail_mask                                         (0x00000002)
#define PHY_SLB0_ST_p0_usb2phy_slb_fail(data)                                        (0x00000002&((data)<<1))
#define PHY_SLB0_ST_p0_usb2phy_slb_fail_src(data)                                    ((0x00000002&(data))>>1)
#define PHY_SLB0_ST_get_p0_usb2phy_slb_fail(data)                                    ((0x00000002&(data))>>1)
#define PHY_SLB0_ST_p0_usb2phy_slb_done_shift                                        (0)
#define PHY_SLB0_ST_p0_usb2phy_slb_done_mask                                         (0x00000001)
#define PHY_SLB0_ST_p0_usb2phy_slb_done(data)                                        (0x00000001&((data)<<0))
#define PHY_SLB0_ST_p0_usb2phy_slb_done_src(data)                                    ((0x00000001&(data))>>0)
#define PHY_SLB0_ST_get_p0_usb2phy_slb_done(data)                                    ((0x00000001&(data))>>0)


#define PHY_SLB1_EN                                                                  0x1801303C
#define PHY_SLB1_EN_reg_addr                                                         "0xB801303C"
#define PHY_SLB1_EN_reg                                                              0xB801303C
#define set_PHY_SLB1_EN_reg(data)   (*((volatile unsigned int*) PHY_SLB1_EN_reg)=data)
#define get_PHY_SLB1_EN_reg   (*((volatile unsigned int*) PHY_SLB1_EN_reg))
#define PHY_SLB1_EN_inst_adr                                                         "0x000F"
#define PHY_SLB1_EN_inst                                                             0x000F
#define PHY_SLB1_EN_p1_usb2phy_slb_hs_shift                                          (1)
#define PHY_SLB1_EN_p1_usb2phy_slb_hs_mask                                           (0x00000002)
#define PHY_SLB1_EN_p1_usb2phy_slb_hs(data)                                          (0x00000002&((data)<<1))
#define PHY_SLB1_EN_p1_usb2phy_slb_hs_src(data)                                      ((0x00000002&(data))>>1)
#define PHY_SLB1_EN_get_p1_usb2phy_slb_hs(data)                                      ((0x00000002&(data))>>1)
#define PHY_SLB1_EN_p1_usb2phy_force_slb_shift                                       (0)
#define PHY_SLB1_EN_p1_usb2phy_force_slb_mask                                        (0x00000001)
#define PHY_SLB1_EN_p1_usb2phy_force_slb(data)                                       (0x00000001&((data)<<0))
#define PHY_SLB1_EN_p1_usb2phy_force_slb_src(data)                                   ((0x00000001&(data))>>0)
#define PHY_SLB1_EN_get_p1_usb2phy_force_slb(data)                                   ((0x00000001&(data))>>0)


#define PHY_SLB1_ST                                                                  0x18013040
#define PHY_SLB1_ST_reg_addr                                                         "0xB8013040"
#define PHY_SLB1_ST_reg                                                              0xB8013040
#define set_PHY_SLB1_ST_reg(data)   (*((volatile unsigned int*) PHY_SLB1_ST_reg)=data)
#define get_PHY_SLB1_ST_reg   (*((volatile unsigned int*) PHY_SLB1_ST_reg))
#define PHY_SLB1_ST_inst_adr                                                         "0x0010"
#define PHY_SLB1_ST_inst                                                             0x0010
#define PHY_SLB1_ST_p1_usb2phy_slb_fail_shift                                        (1)
#define PHY_SLB1_ST_p1_usb2phy_slb_fail_mask                                         (0x00000002)
#define PHY_SLB1_ST_p1_usb2phy_slb_fail(data)                                        (0x00000002&((data)<<1))
#define PHY_SLB1_ST_p1_usb2phy_slb_fail_src(data)                                    ((0x00000002&(data))>>1)
#define PHY_SLB1_ST_get_p1_usb2phy_slb_fail(data)                                    ((0x00000002&(data))>>1)
#define PHY_SLB1_ST_p1_usb2phy_slb_done_shift                                        (0)
#define PHY_SLB1_ST_p1_usb2phy_slb_done_mask                                         (0x00000001)
#define PHY_SLB1_ST_p1_usb2phy_slb_done(data)                                        (0x00000001&((data)<<0))
#define PHY_SLB1_ST_p1_usb2phy_slb_done_src(data)                                    ((0x00000001&(data))>>0)
#define PHY_SLB1_ST_get_p1_usb2phy_slb_done(data)                                    ((0x00000001&(data))>>0)


#define USB2_SPD_CTR                                                                 0x18013044
#define USB2_SPD_CTR_reg_addr                                                        "0xB8013044"
#define USB2_SPD_CTR_reg                                                             0xB8013044
#define set_USB2_SPD_CTR_reg(data)   (*((volatile unsigned int*) USB2_SPD_CTR_reg)=data)
#define get_USB2_SPD_CTR_reg   (*((volatile unsigned int*) USB2_SPD_CTR_reg))
#define USB2_SPD_CTR_inst_adr                                                        "0x0011"
#define USB2_SPD_CTR_inst                                                            0x0011
#define USB2_SPD_CTR_p1_suspend_r_shift                                              (1)
#define USB2_SPD_CTR_p1_suspend_r_mask                                               (0x00000002)
#define USB2_SPD_CTR_p1_suspend_r(data)                                              (0x00000002&((data)<<1))
#define USB2_SPD_CTR_p1_suspend_r_src(data)                                          ((0x00000002&(data))>>1)
#define USB2_SPD_CTR_get_p1_suspend_r(data)                                          ((0x00000002&(data))>>1)
#define USB2_SPD_CTR_p0_suspend_r_shift                                              (0)
#define USB2_SPD_CTR_p0_suspend_r_mask                                               (0x00000001)
#define USB2_SPD_CTR_p0_suspend_r(data)                                              (0x00000001&((data)<<0))
#define USB2_SPD_CTR_p0_suspend_r_src(data)                                          ((0x00000001&(data))>>0)
#define USB2_SPD_CTR_get_p0_suspend_r(data)                                          ((0x00000001&(data))>>0)


#define USB_DBG                                                                      0x18013048
#define USB_DBG_reg_addr                                                             "0xB8013048"
#define USB_DBG_reg                                                                  0xB8013048
#define set_USB_DBG_reg(data)   (*((volatile unsigned int*) USB_DBG_reg)=data)
#define get_USB_DBG_reg   (*((volatile unsigned int*) USB_DBG_reg))
#define USB_DBG_inst_adr                                                             "0x0012"
#define USB_DBG_inst                                                                 0x0012
#define USB_DBG_dbg_sel1_shift                                                       (7)
#define USB_DBG_dbg_sel1_mask                                                        (0x00001F80)
#define USB_DBG_dbg_sel1(data)                                                       (0x00001F80&((data)<<7))
#define USB_DBG_dbg_sel1_src(data)                                                   ((0x00001F80&(data))>>7)
#define USB_DBG_get_dbg_sel1(data)                                                   ((0x00001F80&(data))>>7)
#define USB_DBG_dbg_sel0_shift                                                       (1)
#define USB_DBG_dbg_sel0_mask                                                        (0x0000007E)
#define USB_DBG_dbg_sel0(data)                                                       (0x0000007E&((data)<<1))
#define USB_DBG_dbg_sel0_src(data)                                                   ((0x0000007E&(data))>>1)
#define USB_DBG_get_dbg_sel0(data)                                                   ((0x0000007E&(data))>>1)
#define USB_DBG_dbg_en_shift                                                         (0)
#define USB_DBG_dbg_en_mask                                                          (0x00000001)
#define USB_DBG_dbg_en(data)                                                         (0x00000001&((data)<<0))
#define USB_DBG_dbg_en_src(data)                                                     ((0x00000001&(data))>>0)
#define USB_DBG_get_dbg_en(data)                                                     ((0x00000001&(data))>>0)


#define USB_SCTCH                                                                    0x1801304c
#define USB_SCTCH_reg_addr                                                           "0xB801304C"
#define USB_SCTCH_reg                                                                0xB801304C
#define set_USB_SCTCH_reg(data)   (*((volatile unsigned int*) USB_SCTCH_reg)=data)
#define get_USB_SCTCH_reg   (*((volatile unsigned int*) USB_SCTCH_reg))
#define USB_SCTCH_inst_adr                                                           "0x0013"
#define USB_SCTCH_inst                                                               0x0013
#define USB_SCTCH_reg1_shift                                                         (16)
#define USB_SCTCH_reg1_mask                                                          (0xFFFF0000)
#define USB_SCTCH_reg1(data)                                                         (0xFFFF0000&((data)<<16))
#define USB_SCTCH_reg1_src(data)                                                     ((0xFFFF0000&(data))>>16)
#define USB_SCTCH_get_reg1(data)                                                     ((0xFFFF0000&(data))>>16)
#define USB_SCTCH_reg0_shift                                                         (0)
#define USB_SCTCH_reg0_mask                                                          (0x0000FFFF)
#define USB_SCTCH_reg0(data)                                                         (0x0000FFFF&((data)<<0))
#define USB_SCTCH_reg0_src(data)                                                     ((0x0000FFFF&(data))>>0)
#define USB_SCTCH_get_reg0(data)                                                     ((0x0000FFFF&(data))>>0)


#define USB_TMP_0                                                                    0x18013050
#define USB_TMP_1                                                                    0x18013054
#define USB_TMP_2                                                                    0x18013058
#define USB_TMP_3                                                                    0x1801305C
#define USB_TMP_0_reg_addr                                                           "0xB8013050"
#define USB_TMP_1_reg_addr                                                           "0xB8013054"
#define USB_TMP_2_reg_addr                                                           "0xB8013058"
#define USB_TMP_3_reg_addr                                                           "0xB801305C"
#define USB_TMP_0_reg                                                                0xB8013050
#define USB_TMP_1_reg                                                                0xB8013054
#define USB_TMP_2_reg                                                                0xB8013058
#define USB_TMP_3_reg                                                                0xB801305C
#define set_USB_TMP_0_reg(data)   (*((volatile unsigned int*) USB_TMP_0_reg)=data)
#define set_USB_TMP_1_reg(data)   (*((volatile unsigned int*) USB_TMP_1_reg)=data)
#define set_USB_TMP_2_reg(data)   (*((volatile unsigned int*) USB_TMP_2_reg)=data)
#define set_USB_TMP_3_reg(data)   (*((volatile unsigned int*) USB_TMP_3_reg)=data)
#define get_USB_TMP_0_reg   (*((volatile unsigned int*) USB_TMP_0_reg))
#define get_USB_TMP_1_reg   (*((volatile unsigned int*) USB_TMP_1_reg))
#define get_USB_TMP_2_reg   (*((volatile unsigned int*) USB_TMP_2_reg))
#define get_USB_TMP_3_reg   (*((volatile unsigned int*) USB_TMP_3_reg))
#define USB_TMP_0_inst_adr                                                           "0x0014"
#define USB_TMP_1_inst_adr                                                           "0x0015"
#define USB_TMP_2_inst_adr                                                           "0x0016"
#define USB_TMP_3_inst_adr                                                           "0x0017"
#define USB_TMP_0_inst                                                               0x0014
#define USB_TMP_1_inst                                                               0x0015
#define USB_TMP_2_inst                                                               0x0016
#define USB_TMP_3_inst                                                               0x0017
#define USB_TMP_test_reg_shift                                                       (0)
#define USB_TMP_test_reg_mask                                                        (0xFFFFFFFF)
#define USB_TMP_test_reg(data)                                                       (0xFFFFFFFF&((data)<<0))
#define USB_TMP_test_reg_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define USB_TMP_get_test_reg(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define HMAC_CTR0                                                                    0x18013060
#define HMAC_CTR0_reg_addr                                                           "0xB8013060"
#define HMAC_CTR0_reg                                                                0xB8013060
#define set_HMAC_CTR0_reg(data)   (*((volatile unsigned int*) HMAC_CTR0_reg)=data)
#define get_HMAC_CTR0_reg   (*((volatile unsigned int*) HMAC_CTR0_reg))
#define HMAC_CTR0_inst_adr                                                           "0x0018"
#define HMAC_CTR0_inst                                                               0x0018
#define HMAC_CTR0_host_msi_enable_shift                                              (16)
#define HMAC_CTR0_host_msi_enable_mask                                               (0x00010000)
#define HMAC_CTR0_host_msi_enable(data)                                              (0x00010000&((data)<<16))
#define HMAC_CTR0_host_msi_enable_src(data)                                          ((0x00010000&(data))>>16)
#define HMAC_CTR0_get_host_msi_enable(data)                                          ((0x00010000&(data))>>16)
#define HMAC_CTR0_hub_port_over_current_shift                                        (12)
#define HMAC_CTR0_hub_port_over_current_mask                                         (0x0000F000)
#define HMAC_CTR0_hub_port_over_current(data)                                        (0x0000F000&((data)<<12))
#define HMAC_CTR0_hub_port_over_current_src(data)                                    ((0x0000F000&(data))>>12)
#define HMAC_CTR0_get_hub_port_over_current(data)                                    ((0x0000F000&(data))>>12)
#define HMAC_CTR0_hub_port_perm_attach_shift                                         (8)
#define HMAC_CTR0_hub_port_perm_attach_mask                                          (0x00000F00)
#define HMAC_CTR0_hub_port_perm_attach(data)                                         (0x00000F00&((data)<<8))
#define HMAC_CTR0_hub_port_perm_attach_src(data)                                     ((0x00000F00&(data))>>8)
#define HMAC_CTR0_get_hub_port_perm_attach(data)                                     ((0x00000F00&(data))>>8)
#define HMAC_CTR0_host_num_u2_port_shift                                             (4)
#define HMAC_CTR0_host_num_u2_port_mask                                              (0x000000F0)
#define HMAC_CTR0_host_num_u2_port(data)                                             (0x000000F0&((data)<<4))
#define HMAC_CTR0_host_num_u2_port_src(data)                                         ((0x000000F0&(data))>>4)
#define HMAC_CTR0_get_host_num_u2_port(data)                                         ((0x000000F0&(data))>>4)
#define HMAC_CTR0_host_num_u3_port_shift                                             (0)
#define HMAC_CTR0_host_num_u3_port_mask                                              (0x0000000F)
#define HMAC_CTR0_host_num_u3_port(data)                                             (0x0000000F&((data)<<0))
#define HMAC_CTR0_host_num_u3_port_src(data)                                         ((0x0000000F&(data))>>0)
#define HMAC_CTR0_get_host_num_u3_port(data)                                         ((0x0000000F&(data))>>0)


#define HMAC_CTR1                                                                    0x18013064
#define HMAC_CTR1_reg_addr                                                           "0xB8013064"
#define HMAC_CTR1_reg                                                                0xB8013064
#define set_HMAC_CTR1_reg(data)   (*((volatile unsigned int*) HMAC_CTR1_reg)=data)
#define get_HMAC_CTR1_reg   (*((volatile unsigned int*) HMAC_CTR1_reg))
#define HMAC_CTR1_inst_adr                                                           "0x0019"
#define HMAC_CTR1_inst                                                               0x0019
#define HMAC_CTR1_host_fladj_125mhz_shift                                            (15)
#define HMAC_CTR1_host_fladj_125mhz_mask                                             (0x07FF8000)
#define HMAC_CTR1_host_fladj_125mhz(data)                                            (0x07FF8000&((data)<<15))
#define HMAC_CTR1_host_fladj_125mhz_src(data)                                        ((0x07FF8000&(data))>>15)
#define HMAC_CTR1_get_host_fladj_125mhz(data)                                        ((0x07FF8000&(data))>>15)
#define HMAC_CTR1_host_fladj_240mhz_shift                                            (6)
#define HMAC_CTR1_host_fladj_240mhz_mask                                             (0x00007FC0)
#define HMAC_CTR1_host_fladj_240mhz(data)                                            (0x00007FC0&((data)<<6))
#define HMAC_CTR1_host_fladj_240mhz_src(data)                                        ((0x00007FC0&(data))>>6)
#define HMAC_CTR1_get_host_fladj_240mhz(data)                                        ((0x00007FC0&(data))>>6)
#define HMAC_CTR1_host_fladj_30mhz_shift                                             (0)
#define HMAC_CTR1_host_fladj_30mhz_mask                                              (0x0000003F)
#define HMAC_CTR1_host_fladj_30mhz(data)                                             (0x0000003F&((data)<<0))
#define HMAC_CTR1_host_fladj_30mhz_src(data)                                         ((0x0000003F&(data))>>0)
#define HMAC_CTR1_get_host_fladj_30mhz(data)                                         ((0x0000003F&(data))>>0)


#define DMAC_CTR0                                                                    0x18013068
#define DMAC_CTR0_reg_addr                                                           "0xB8013068"
#define DMAC_CTR0_reg                                                                0xB8013068
#define set_DMAC_CTR0_reg(data)   (*((volatile unsigned int*) DMAC_CTR0_reg)=data)
#define get_DMAC_CTR0_reg   (*((volatile unsigned int*) DMAC_CTR0_reg))
#define DMAC_CTR0_inst_adr                                                           "0x001A"
#define DMAC_CTR0_inst                                                               0x001A
#define DMAC_CTR0_utmiotg_vbusvalid_shift                                            (16)
#define DMAC_CTR0_utmiotg_vbusvalid_mask                                             (0x00010000)
#define DMAC_CTR0_utmiotg_vbusvalid(data)                                            (0x00010000&((data)<<16))
#define DMAC_CTR0_utmiotg_vbusvalid_src(data)                                        ((0x00010000&(data))>>16)
#define DMAC_CTR0_get_utmiotg_vbusvalid(data)                                        ((0x00010000&(data))>>16)
#define DMAC_CTR0_dev_usb_outep_pkt_buff_shift                                       (0)
#define DMAC_CTR0_dev_usb_outep_pkt_buff_mask                                        (0x0000FFFF)
#define DMAC_CTR0_dev_usb_outep_pkt_buff(data)                                       (0x0000FFFF&((data)<<0))
#define DMAC_CTR0_dev_usb_outep_pkt_buff_src(data)                                   ((0x0000FFFF&(data))>>0)
#define DMAC_CTR0_get_dev_usb_outep_pkt_buff(data)                                   ((0x0000FFFF&(data))>>0)


#define DMAC_CTR1                                                                    0x1801306C
#define DMAC_CTR1_reg_addr                                                           "0xB801306C"
#define DMAC_CTR1_reg                                                                0xB801306C
#define set_DMAC_CTR1_reg(data)   (*((volatile unsigned int*) DMAC_CTR1_reg)=data)
#define get_DMAC_CTR1_reg   (*((volatile unsigned int*) DMAC_CTR1_reg))
#define DMAC_CTR1_inst_adr                                                           "0x001B"
#define DMAC_CTR1_inst                                                               0x001B
#define DMAC_CTR1_dev_fladj_125mhz_shift                                             (15)
#define DMAC_CTR1_dev_fladj_125mhz_mask                                              (0x07FF8000)
#define DMAC_CTR1_dev_fladj_125mhz(data)                                             (0x07FF8000&((data)<<15))
#define DMAC_CTR1_dev_fladj_125mhz_src(data)                                         ((0x07FF8000&(data))>>15)
#define DMAC_CTR1_get_dev_fladj_125mhz(data)                                         ((0x07FF8000&(data))>>15)
#define DMAC_CTR1_dev_fladj_240mhz_shift                                             (6)
#define DMAC_CTR1_dev_fladj_240mhz_mask                                              (0x00007FC0)
#define DMAC_CTR1_dev_fladj_240mhz(data)                                             (0x00007FC0&((data)<<6))
#define DMAC_CTR1_dev_fladj_240mhz_src(data)                                         ((0x00007FC0&(data))>>6)
#define DMAC_CTR1_get_dev_fladj_240mhz(data)                                         ((0x00007FC0&(data))>>6)
#define DMAC_CTR1_dev_fladj_30mhz_shift                                              (0)
#define DMAC_CTR1_dev_fladj_30mhz_mask                                               (0x0000003F)
#define DMAC_CTR1_dev_fladj_30mhz(data)                                              (0x0000003F&((data)<<0))
#define DMAC_CTR1_dev_fladj_30mhz_src(data)                                          ((0x0000003F&(data))>>0)
#define DMAC_CTR1_get_dev_fladj_30mhz(data)                                          ((0x0000003F&(data))>>0)


#define USB2_PHY                                                                     0x18013070
#define USB2_PHY_reg_addr                                                            "0xB8013070"
#define USB2_PHY_reg                                                                 0xB8013070
#define set_USB2_PHY_reg(data)   (*((volatile unsigned int*) USB2_PHY_reg)=data)
#define get_USB2_PHY_reg   (*((volatile unsigned int*) USB2_PHY_reg))
#define USB2_PHY_inst_adr                                                            "0x001C"
#define USB2_PHY_inst                                                                0x001C
#define USB2_PHY_p1_DmPulldown_shift                                                 (10)
#define USB2_PHY_p1_DmPulldown_mask                                                  (0x00000400)
#define USB2_PHY_p1_DmPulldown(data)                                                 (0x00000400&((data)<<10))
#define USB2_PHY_p1_DmPulldown_src(data)                                             ((0x00000400&(data))>>10)
#define USB2_PHY_get_p1_DmPulldown(data)                                             ((0x00000400&(data))>>10)
#define USB2_PHY_p1_DpPulldown_shift                                                 (9)
#define USB2_PHY_p1_DpPulldown_mask                                                  (0x00000200)
#define USB2_PHY_p1_DpPulldown(data)                                                 (0x00000200&((data)<<9))
#define USB2_PHY_p1_DpPulldown_src(data)                                             ((0x00000200&(data))>>9)
#define USB2_PHY_get_p1_DpPulldown(data)                                             ((0x00000200&(data))>>9)
#define USB2_PHY_p1_IDPULLUP_shift                                                   (8)
#define USB2_PHY_p1_IDPULLUP_mask                                                    (0x00000100)
#define USB2_PHY_p1_IDPULLUP(data)                                                   (0x00000100&((data)<<8))
#define USB2_PHY_p1_IDPULLUP_src(data)                                               ((0x00000100&(data))>>8)
#define USB2_PHY_get_p1_IDPULLUP(data)                                               ((0x00000100&(data))>>8)
#define USB2_PHY_p1_DmPulldown_sel_shift                                             (2)
#define USB2_PHY_p1_DmPulldown_sel_mask                                              (0x00000004)
#define USB2_PHY_p1_DmPulldown_sel(data)                                             (0x00000004&((data)<<2))
#define USB2_PHY_p1_DmPulldown_sel_src(data)                                         ((0x00000004&(data))>>2)
#define USB2_PHY_get_p1_DmPulldown_sel(data)                                         ((0x00000004&(data))>>2)
#define USB2_PHY_p1_DpPulldown_sel_shift                                             (1)
#define USB2_PHY_p1_DpPulldown_sel_mask                                              (0x00000002)
#define USB2_PHY_p1_DpPulldown_sel(data)                                             (0x00000002&((data)<<1))
#define USB2_PHY_p1_DpPulldown_sel_src(data)                                         ((0x00000002&(data))>>1)
#define USB2_PHY_get_p1_DpPulldown_sel(data)                                         ((0x00000002&(data))>>1)
#define USB2_PHY_p1_IDPULLUP_sel_shift                                               (0)
#define USB2_PHY_p1_IDPULLUP_sel_mask                                                (0x00000001)
#define USB2_PHY_p1_IDPULLUP_sel(data)                                               (0x00000001&((data)<<0))
#define USB2_PHY_p1_IDPULLUP_sel_src(data)                                           ((0x00000001&(data))>>0)
#define USB2_PHY_get_p1_IDPULLUP_sel(data)                                           ((0x00000001&(data))>>0)


#define RAM_CTR                                                                      0x18013074
#define RAM_CTR_reg_addr                                                             "0xB8013074"
#define RAM_CTR_reg                                                                  0xB8013074
#define set_RAM_CTR_reg(data)   (*((volatile unsigned int*) RAM_CTR_reg)=data)
#define get_RAM_CTR_reg   (*((volatile unsigned int*) RAM_CTR_reg))
#define RAM_CTR_inst_adr                                                             "0x001D"
#define RAM_CTR_inst                                                                 0x001D
#define RAM_CTR_error_st_shift                                                       (17)
#define RAM_CTR_error_st_mask                                                        (0x00020000)
#define RAM_CTR_error_st(data)                                                       (0x00020000&((data)<<17))
#define RAM_CTR_error_st_src(data)                                                   ((0x00020000&(data))>>17)
#define RAM_CTR_get_error_st(data)                                                   ((0x00020000&(data))>>17)
#define RAM_CTR_done_st_shift                                                        (16)
#define RAM_CTR_done_st_mask                                                         (0x00010000)
#define RAM_CTR_done_st(data)                                                        (0x00010000&((data)<<16))
#define RAM_CTR_done_st_src(data)                                                    ((0x00010000&(data))>>16)
#define RAM_CTR_get_done_st(data)                                                    ((0x00010000&(data))>>16)
#define RAM_CTR_go_ct_shift                                                          (0)
#define RAM_CTR_go_ct_mask                                                           (0x00000001)
#define RAM_CTR_go_ct(data)                                                          (0x00000001&((data)<<0))
#define RAM_CTR_go_ct_src(data)                                                      ((0x00000001&(data))>>0)
#define RAM_CTR_get_go_ct(data)                                                      ((0x00000001&(data))>>0)


#define RAM_ADDR                                                                     0x18013078
#define RAM_ADDR_reg_addr                                                            "0xB8013078"
#define RAM_ADDR_reg                                                                 0xB8013078
#define set_RAM_ADDR_reg(data)   (*((volatile unsigned int*) RAM_ADDR_reg)=data)
#define get_RAM_ADDR_reg   (*((volatile unsigned int*) RAM_ADDR_reg))
#define RAM_ADDR_inst_adr                                                            "0x001E"
#define RAM_ADDR_inst                                                                0x001E
#define RAM_ADDR_sram_addr_shift                                                     (0)
#define RAM_ADDR_sram_addr_mask                                                      (0xFFFFFFFF)
#define RAM_ADDR_sram_addr(data)                                                     (0xFFFFFFFF&((data)<<0))
#define RAM_ADDR_sram_addr_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define RAM_ADDR_get_sram_addr(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define RAM_WDATA                                                                    0x1801307C
#define RAM_WDATA_reg_addr                                                           "0xB801307C"
#define RAM_WDATA_reg                                                                0xB801307C
#define set_RAM_WDATA_reg(data)   (*((volatile unsigned int*) RAM_WDATA_reg)=data)
#define get_RAM_WDATA_reg   (*((volatile unsigned int*) RAM_WDATA_reg))
#define RAM_WDATA_inst_adr                                                           "0x001F"
#define RAM_WDATA_inst                                                               0x001F
#define RAM_WDATA_ram_wdata_shift                                                    (0)
#define RAM_WDATA_ram_wdata_mask                                                     (0xFFFFFFFF)
#define RAM_WDATA_ram_wdata(data)                                                    (0xFFFFFFFF&((data)<<0))
#define RAM_WDATA_ram_wdata_src(data)                                                ((0xFFFFFFFF&(data))>>0)
#define RAM_WDATA_get_ram_wdata(data)                                                ((0xFFFFFFFF&(data))>>0)


#define RAM_RDATA                                                                    0x18013080
#define RAM_RDATA_reg_addr                                                           "0xB8013080"
#define RAM_RDATA_reg                                                                0xB8013080
#define set_RAM_RDATA_reg(data)   (*((volatile unsigned int*) RAM_RDATA_reg)=data)
#define get_RAM_RDATA_reg   (*((volatile unsigned int*) RAM_RDATA_reg))
#define RAM_RDATA_inst_adr                                                           "0x0020"
#define RAM_RDATA_inst                                                               0x0020
#define RAM_RDATA_ram_rdata_shift                                                    (0)
#define RAM_RDATA_ram_rdata_mask                                                     (0xFFFFFFFF)
#define RAM_RDATA_ram_rdata(data)                                                    (0xFFFFFFFF&((data)<<0))
#define RAM_RDATA_ram_rdata_src(data)                                                ((0xFFFFFFFF&(data))>>0)
#define RAM_RDATA_get_ram_rdata(data)                                                ((0xFFFFFFFF&(data))>>0)


#endif
