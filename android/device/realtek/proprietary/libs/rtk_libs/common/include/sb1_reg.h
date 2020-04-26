/**************************************************************
// Spec Version                  : 0.1.2
// Parser Version                : MAR_Parser_1.0
// CModelGen Version             : 1.0 2006.04.18
// Naming Rule                   : Module_Register_Name
// Firmware Header Generate Date : 2007/12/7
***************************************************************/


#ifndef _SB1_REG_H_INCLUDED_
#define _SB1_REG_H_INCLUDED_
#ifdef  _SB1_USE_STRUCT
typedef struct 
{
unsigned int     write_enable3:1;
unsigned int     dmy:22;
unsigned int     write_enable2:1;
unsigned int     wl:6;
unsigned int     write_enable1:1;
unsigned int     rr:1;
}SB1_CTRL;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     usb:9;
unsigned int     reserved_1:7;
unsigned int     mcp:9;
}SB1_PRIORITY1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sata0:9;
unsigned int     reserved_1:7;
unsigned int     nf:9;
}SB1_PRIORITY2;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     pci:9;
unsigned int     reserved_1:7;
unsigned int     sata1:9;
}SB1_PRIORITY3;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     tp:9;
unsigned int     reserved_1:7;
unsigned int     etn:9;
}SB1_PRIORITY4;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     aio:9;
}SB1_PRIORITY5;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     status_id:9;
unsigned int     reserved_1:7;
unsigned int     mask_id:9;
}SB1_MASK;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     write_enable2:1;
unsigned int     pc2_go:1;
unsigned int     pc2_sel:9;
unsigned int     reserved_1:5;
unsigned int     write_enable1:1;
unsigned int     pc1_go:1;
unsigned int     pc1_sel:9;
}SB1_PC_CTRL;

typedef struct 
{
unsigned int     tol:32;
}SB1_PC1_CYC;

typedef struct 
{
unsigned int     tol_cyc:32;
}SB1_PC1_ACK;

typedef struct 
{
unsigned int     req_lat:32;
}SB1_PC1_ACC;

typedef struct 
{
unsigned int     max_lat:16;
unsigned int     tol_num:16;
}SB1_PC1_REQ;

typedef struct 
{
unsigned int     tol:32;
}SB1_PC2_CYC;

typedef struct 
{
unsigned int     tol_cyc:32;
}SB1_PC2_ACK;

typedef struct 
{
unsigned int     req_lat:32;
}SB1_PC2_ACC;

typedef struct 
{
unsigned int     max_lat:16;
unsigned int     tol_num:16;
}SB1_PC2_REQ;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     write_enable3:1;
unsigned int     sel1:5;
unsigned int     write_enable2:1;
unsigned int     sel0:5;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}SB1_DBG;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     write_enable2:1;
unsigned int     en_wbuf_rule:1;
unsigned int     wbuf_rule:1;
unsigned int     write_enable1:1;
unsigned int     ip:9;
}SB1_HI_PRI;

#endif

#define SB1_CTRL                                                                     0x1801C200
#define SB1_CTRL_reg_addr                                                            "0xB801C200"
#define SB1_CTRL_reg                                                                 0xB801C200
#define SB1_CTRL_inst_adr                                                            "0x0080"
#define SB1_CTRL_inst                                                                0x0080
#define SB1_CTRL_write_enable3_shift                                                 (31)
#define SB1_CTRL_write_enable3_mask                                                  (0x80000000)
#define SB1_CTRL_write_enable3(data)                                                 (0x80000000&((data)<<31))
#define SB1_CTRL_write_enable3_src(data)                                             ((0x80000000&(data))>>31)
#define SB1_CTRL_dmy_shift                                                           (9)
#define SB1_CTRL_dmy_mask                                                            (0x7FFFFE00)
#define SB1_CTRL_dmy(data)                                                           (0x7FFFFE00&((data)<<9))
#define SB1_CTRL_dmy_src(data)                                                       ((0x7FFFFE00&(data))>>9)
#define SB1_CTRL_write_enable2_shift                                                 (8)
#define SB1_CTRL_write_enable2_mask                                                  (0x00000100)
#define SB1_CTRL_write_enable2(data)                                                 (0x00000100&((data)<<8))
#define SB1_CTRL_write_enable2_src(data)                                             ((0x00000100&(data))>>8)
#define SB1_CTRL_wl_shift                                                            (2)
#define SB1_CTRL_wl_mask                                                             (0x000000FC)
#define SB1_CTRL_wl(data)                                                            (0x000000FC&((data)<<2))
#define SB1_CTRL_wl_src(data)                                                        ((0x000000FC&(data))>>2)
#define SB1_CTRL_write_enable1_shift                                                 (1)
#define SB1_CTRL_write_enable1_mask                                                  (0x00000002)
#define SB1_CTRL_write_enable1(data)                                                 (0x00000002&((data)<<1))
#define SB1_CTRL_write_enable1_src(data)                                             ((0x00000002&(data))>>1)
#define SB1_CTRL_rr_shift                                                            (0)
#define SB1_CTRL_rr_mask                                                             (0x00000001)
#define SB1_CTRL_rr(data)                                                            (0x00000001&((data)<<0))
#define SB1_CTRL_rr_src(data)                                                        ((0x00000001&(data))>>0)


#define SB1_PRIORITY1                                                                0x1801C204
#define SB1_PRIORITY1_reg_addr                                                       "0xB801C204"
#define SB1_PRIORITY1_reg                                                            0xB801C204
#define SB1_PRIORITY1_inst_adr                                                       "0x0081"
#define SB1_PRIORITY1_inst                                                           0x0081
#define SB1_PRIORITY1_usb_shift                                                      (16)
#define SB1_PRIORITY1_usb_mask                                                       (0x01FF0000)
#define SB1_PRIORITY1_usb(data)                                                      (0x01FF0000&((data)<<16))
#define SB1_PRIORITY1_usb_src(data)                                                  ((0x01FF0000&(data))>>16)
#define SB1_PRIORITY1_mcp_shift                                                      (0)
#define SB1_PRIORITY1_mcp_mask                                                       (0x000001FF)
#define SB1_PRIORITY1_mcp(data)                                                      (0x000001FF&((data)<<0))
#define SB1_PRIORITY1_mcp_src(data)                                                  ((0x000001FF&(data))>>0)


#define SB1_PRIORITY2                                                                0x1801C208
#define SB1_PRIORITY2_reg_addr                                                       "0xB801C208"
#define SB1_PRIORITY2_reg                                                            0xB801C208
#define SB1_PRIORITY2_inst_adr                                                       "0x0082"
#define SB1_PRIORITY2_inst                                                           0x0082
#define SB1_PRIORITY2_sata0_shift                                                    (16)
#define SB1_PRIORITY2_sata0_mask                                                     (0x01FF0000)
#define SB1_PRIORITY2_sata0(data)                                                    (0x01FF0000&((data)<<16))
#define SB1_PRIORITY2_sata0_src(data)                                                ((0x01FF0000&(data))>>16)
#define SB1_PRIORITY2_nf_shift                                                       (0)
#define SB1_PRIORITY2_nf_mask                                                        (0x000001FF)
#define SB1_PRIORITY2_nf(data)                                                       (0x000001FF&((data)<<0))
#define SB1_PRIORITY2_nf_src(data)                                                   ((0x000001FF&(data))>>0)


#define SB1_PRIORITY3                                                                0x1801C20C
#define SB1_PRIORITY3_reg_addr                                                       "0xB801C20C"
#define SB1_PRIORITY3_reg                                                            0xB801C20C
#define SB1_PRIORITY3_inst_adr                                                       "0x0083"
#define SB1_PRIORITY3_inst                                                           0x0083
#define SB1_PRIORITY3_pci_shift                                                      (16)
#define SB1_PRIORITY3_pci_mask                                                       (0x01FF0000)
#define SB1_PRIORITY3_pci(data)                                                      (0x01FF0000&((data)<<16))
#define SB1_PRIORITY3_pci_src(data)                                                  ((0x01FF0000&(data))>>16)
#define SB1_PRIORITY3_sata1_shift                                                    (0)
#define SB1_PRIORITY3_sata1_mask                                                     (0x000001FF)
#define SB1_PRIORITY3_sata1(data)                                                    (0x000001FF&((data)<<0))
#define SB1_PRIORITY3_sata1_src(data)                                                ((0x000001FF&(data))>>0)


#define SB1_PRIORITY4                                                                0x1801C210
#define SB1_PRIORITY4_reg_addr                                                       "0xB801C210"
#define SB1_PRIORITY4_reg                                                            0xB801C210
#define SB1_PRIORITY4_inst_adr                                                       "0x0084"
#define SB1_PRIORITY4_inst                                                           0x0084
#define SB1_PRIORITY4_tp_shift                                                       (16)
#define SB1_PRIORITY4_tp_mask                                                        (0x01FF0000)
#define SB1_PRIORITY4_tp(data)                                                       (0x01FF0000&((data)<<16))
#define SB1_PRIORITY4_tp_src(data)                                                   ((0x01FF0000&(data))>>16)
#define SB1_PRIORITY4_etn_shift                                                      (0)
#define SB1_PRIORITY4_etn_mask                                                       (0x000001FF)
#define SB1_PRIORITY4_etn(data)                                                      (0x000001FF&((data)<<0))
#define SB1_PRIORITY4_etn_src(data)                                                  ((0x000001FF&(data))>>0)


#define SB1_PRIORITY5                                                                0x1801C214
#define SB1_PRIORITY5_reg_addr                                                       "0xB801C214"
#define SB1_PRIORITY5_reg                                                            0xB801C214
#define SB1_PRIORITY5_inst_adr                                                       "0x0085"
#define SB1_PRIORITY5_inst                                                           0x0085
#define SB1_PRIORITY5_aio_shift                                                      (0)
#define SB1_PRIORITY5_aio_mask                                                       (0x000001FF)
#define SB1_PRIORITY5_aio(data)                                                      (0x000001FF&((data)<<0))
#define SB1_PRIORITY5_aio_src(data)                                                  ((0x000001FF&(data))>>0)


#define SB1_MASK                                                                     0x1801C218
#define SB1_MASK_reg_addr                                                            "0xB801C218"
#define SB1_MASK_reg                                                                 0xB801C218
#define SB1_MASK_inst_adr                                                            "0x0086"
#define SB1_MASK_inst                                                                0x0086
#define SB1_MASK_status_id_shift                                                     (16)
#define SB1_MASK_status_id_mask                                                      (0x01FF0000)
#define SB1_MASK_status_id(data)                                                     (0x01FF0000&((data)<<16))
#define SB1_MASK_status_id_src(data)                                                 ((0x01FF0000&(data))>>16)
#define SB1_MASK_mask_id_shift                                                       (0)
#define SB1_MASK_mask_id_mask                                                        (0x000001FF)
#define SB1_MASK_mask_id(data)                                                       (0x000001FF&((data)<<0))
#define SB1_MASK_mask_id_src(data)                                                   ((0x000001FF&(data))>>0)


#define SB1_PC_CTRL                                                                  0x1801C21C
#define SB1_PC_CTRL_reg_addr                                                         "0xB801C21C"
#define SB1_PC_CTRL_reg                                                              0xB801C21C
#define SB1_PC_CTRL_inst_adr                                                         "0x0087"
#define SB1_PC_CTRL_inst                                                             0x0087
#define SB1_PC_CTRL_write_enable2_shift                                              (26)
#define SB1_PC_CTRL_write_enable2_mask                                               (0x04000000)
#define SB1_PC_CTRL_write_enable2(data)                                              (0x04000000&((data)<<26))
#define SB1_PC_CTRL_write_enable2_src(data)                                          ((0x04000000&(data))>>26)
#define SB1_PC_CTRL_pc2_go_shift                                                     (25)
#define SB1_PC_CTRL_pc2_go_mask                                                      (0x02000000)
#define SB1_PC_CTRL_pc2_go(data)                                                     (0x02000000&((data)<<25))
#define SB1_PC_CTRL_pc2_go_src(data)                                                 ((0x02000000&(data))>>25)
#define SB1_PC_CTRL_pc2_sel_shift                                                    (16)
#define SB1_PC_CTRL_pc2_sel_mask                                                     (0x01FF0000)
#define SB1_PC_CTRL_pc2_sel(data)                                                    (0x01FF0000&((data)<<16))
#define SB1_PC_CTRL_pc2_sel_src(data)                                                ((0x01FF0000&(data))>>16)
#define SB1_PC_CTRL_write_enable1_shift                                              (10)
#define SB1_PC_CTRL_write_enable1_mask                                               (0x00000400)
#define SB1_PC_CTRL_write_enable1(data)                                              (0x00000400&((data)<<10))
#define SB1_PC_CTRL_write_enable1_src(data)                                          ((0x00000400&(data))>>10)
#define SB1_PC_CTRL_pc1_go_shift                                                     (9)
#define SB1_PC_CTRL_pc1_go_mask                                                      (0x00000200)
#define SB1_PC_CTRL_pc1_go(data)                                                     (0x00000200&((data)<<9))
#define SB1_PC_CTRL_pc1_go_src(data)                                                 ((0x00000200&(data))>>9)
#define SB1_PC_CTRL_pc1_sel_shift                                                    (0)
#define SB1_PC_CTRL_pc1_sel_mask                                                     (0x000001FF)
#define SB1_PC_CTRL_pc1_sel(data)                                                    (0x000001FF&((data)<<0))
#define SB1_PC_CTRL_pc1_sel_src(data)                                                ((0x000001FF&(data))>>0)


#define SB1_PC1_CYC                                                                  0x1801C220
#define SB1_PC1_CYC_reg_addr                                                         "0xB801C220"
#define SB1_PC1_CYC_reg                                                              0xB801C220
#define SB1_PC1_CYC_inst_adr                                                         "0x0088"
#define SB1_PC1_CYC_inst                                                             0x0088
#define SB1_PC1_CYC_tol_shift                                                        (0)
#define SB1_PC1_CYC_tol_mask                                                         (0xFFFFFFFF)
#define SB1_PC1_CYC_tol(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB1_PC1_CYC_tol_src(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB1_PC1_ACK                                                                  0x1801C224
#define SB1_PC1_ACK_reg_addr                                                         "0xB801C224"
#define SB1_PC1_ACK_reg                                                              0xB801C224
#define SB1_PC1_ACK_inst_adr                                                         "0x0089"
#define SB1_PC1_ACK_inst                                                             0x0089
#define SB1_PC1_ACK_tol_cyc_shift                                                    (0)
#define SB1_PC1_ACK_tol_cyc_mask                                                     (0xFFFFFFFF)
#define SB1_PC1_ACK_tol_cyc(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB1_PC1_ACK_tol_cyc_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB1_PC1_ACC                                                                  0x1801C228
#define SB1_PC1_ACC_reg_addr                                                         "0xB801C228"
#define SB1_PC1_ACC_reg                                                              0xB801C228
#define SB1_PC1_ACC_inst_adr                                                         "0x008A"
#define SB1_PC1_ACC_inst                                                             0x008A
#define SB1_PC1_ACC_req_lat_shift                                                    (0)
#define SB1_PC1_ACC_req_lat_mask                                                     (0xFFFFFFFF)
#define SB1_PC1_ACC_req_lat(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB1_PC1_ACC_req_lat_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB1_PC1_REQ                                                                  0x1801C22C
#define SB1_PC1_REQ_reg_addr                                                         "0xB801C22C"
#define SB1_PC1_REQ_reg                                                              0xB801C22C
#define SB1_PC1_REQ_inst_adr                                                         "0x008B"
#define SB1_PC1_REQ_inst                                                             0x008B
#define SB1_PC1_REQ_max_lat_shift                                                    (16)
#define SB1_PC1_REQ_max_lat_mask                                                     (0xFFFF0000)
#define SB1_PC1_REQ_max_lat(data)                                                    (0xFFFF0000&((data)<<16))
#define SB1_PC1_REQ_max_lat_src(data)                                                ((0xFFFF0000&(data))>>16)
#define SB1_PC1_REQ_tol_num_shift                                                    (0)
#define SB1_PC1_REQ_tol_num_mask                                                     (0x0000FFFF)
#define SB1_PC1_REQ_tol_num(data)                                                    (0x0000FFFF&((data)<<0))
#define SB1_PC1_REQ_tol_num_src(data)                                                ((0x0000FFFF&(data))>>0)


#define SB1_PC2_CYC                                                                  0x1801C230
#define SB1_PC2_CYC_reg_addr                                                         "0xB801C230"
#define SB1_PC2_CYC_reg                                                              0xB801C230
#define SB1_PC2_CYC_inst_adr                                                         "0x008C"
#define SB1_PC2_CYC_inst                                                             0x008C
#define SB1_PC2_CYC_tol_shift                                                        (0)
#define SB1_PC2_CYC_tol_mask                                                         (0xFFFFFFFF)
#define SB1_PC2_CYC_tol(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB1_PC2_CYC_tol_src(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB1_PC2_ACK                                                                  0x1801C234
#define SB1_PC2_ACK_reg_addr                                                         "0xB801C234"
#define SB1_PC2_ACK_reg                                                              0xB801C234
#define SB1_PC2_ACK_inst_adr                                                         "0x008D"
#define SB1_PC2_ACK_inst                                                             0x008D
#define SB1_PC2_ACK_tol_cyc_shift                                                    (0)
#define SB1_PC2_ACK_tol_cyc_mask                                                     (0xFFFFFFFF)
#define SB1_PC2_ACK_tol_cyc(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB1_PC2_ACK_tol_cyc_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB1_PC2_ACC                                                                  0x1801C238
#define SB1_PC2_ACC_reg_addr                                                         "0xB801C238"
#define SB1_PC2_ACC_reg                                                              0xB801C238
#define SB1_PC2_ACC_inst_adr                                                         "0x008E"
#define SB1_PC2_ACC_inst                                                             0x008E
#define SB1_PC2_ACC_req_lat_shift                                                    (0)
#define SB1_PC2_ACC_req_lat_mask                                                     (0xFFFFFFFF)
#define SB1_PC2_ACC_req_lat(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB1_PC2_ACC_req_lat_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB1_PC2_REQ                                                                  0x1801C23C
#define SB1_PC2_REQ_reg_addr                                                         "0xB801C23C"
#define SB1_PC2_REQ_reg                                                              0xB801C23C
#define SB1_PC2_REQ_inst_adr                                                         "0x008F"
#define SB1_PC2_REQ_inst                                                             0x008F
#define SB1_PC2_REQ_max_lat_shift                                                    (16)
#define SB1_PC2_REQ_max_lat_mask                                                     (0xFFFF0000)
#define SB1_PC2_REQ_max_lat(data)                                                    (0xFFFF0000&((data)<<16))
#define SB1_PC2_REQ_max_lat_src(data)                                                ((0xFFFF0000&(data))>>16)
#define SB1_PC2_REQ_tol_num_shift                                                    (0)
#define SB1_PC2_REQ_tol_num_mask                                                     (0x0000FFFF)
#define SB1_PC2_REQ_tol_num(data)                                                    (0x0000FFFF&((data)<<0))
#define SB1_PC2_REQ_tol_num_src(data)                                                ((0x0000FFFF&(data))>>0)


#define SB1_DBG                                                                      0x1801C240
#define SB1_DBG_reg_addr                                                             "0xB801C240"
#define SB1_DBG_reg                                                                  0xB801C240
#define SB1_DBG_inst_adr                                                             "0x0090"
#define SB1_DBG_inst                                                                 0x0090
#define SB1_DBG_write_enable3_shift                                                  (13)
#define SB1_DBG_write_enable3_mask                                                   (0x00002000)
#define SB1_DBG_write_enable3(data)                                                  (0x00002000&((data)<<13))
#define SB1_DBG_write_enable3_src(data)                                              ((0x00002000&(data))>>13)
#define SB1_DBG_sel1_shift                                                           (8)
#define SB1_DBG_sel1_mask                                                            (0x00001F00)
#define SB1_DBG_sel1(data)                                                           (0x00001F00&((data)<<8))
#define SB1_DBG_sel1_src(data)                                                       ((0x00001F00&(data))>>8)
#define SB1_DBG_write_enable2_shift                                                  (7)
#define SB1_DBG_write_enable2_mask                                                   (0x00000080)
#define SB1_DBG_write_enable2(data)                                                  (0x00000080&((data)<<7))
#define SB1_DBG_write_enable2_src(data)                                              ((0x00000080&(data))>>7)
#define SB1_DBG_sel0_shift                                                           (2)
#define SB1_DBG_sel0_mask                                                            (0x0000007C)
#define SB1_DBG_sel0(data)                                                           (0x0000007C&((data)<<2))
#define SB1_DBG_sel0_src(data)                                                       ((0x0000007C&(data))>>2)
#define SB1_DBG_write_enable1_shift                                                  (1)
#define SB1_DBG_write_enable1_mask                                                   (0x00000002)
#define SB1_DBG_write_enable1(data)                                                  (0x00000002&((data)<<1))
#define SB1_DBG_write_enable1_src(data)                                              ((0x00000002&(data))>>1)
#define SB1_DBG_enable_shift                                                         (0)
#define SB1_DBG_enable_mask                                                          (0x00000001)
#define SB1_DBG_enable(data)                                                         (0x00000001&((data)<<0))
#define SB1_DBG_enable_src(data)                                                     ((0x00000001&(data))>>0)


#define SB1_HI_PRI                                                                   0x1801C244
#define SB1_HI_PRI_reg_addr                                                          "0xB801C244"
#define SB1_HI_PRI_reg                                                               0xB801C244
#define SB1_HI_PRI_inst_adr                                                          "0x0091"
#define SB1_HI_PRI_inst                                                              0x0091
#define SB1_HI_PRI_write_enable2_shift                                               (12)
#define SB1_HI_PRI_write_enable2_mask                                                (0x00001000)
#define SB1_HI_PRI_write_enable2(data)                                               (0x00001000&((data)<<12))
#define SB1_HI_PRI_write_enable2_src(data)                                           ((0x00001000&(data))>>12)
#define SB1_HI_PRI_en_wbuf_rule_shift                                                (11)
#define SB1_HI_PRI_en_wbuf_rule_mask                                                 (0x00000800)
#define SB1_HI_PRI_en_wbuf_rule(data)                                                (0x00000800&((data)<<11))
#define SB1_HI_PRI_en_wbuf_rule_src(data)                                            ((0x00000800&(data))>>11)
#define SB1_HI_PRI_wbuf_rule_shift                                                   (10)
#define SB1_HI_PRI_wbuf_rule_mask                                                    (0x00000400)
#define SB1_HI_PRI_wbuf_rule(data)                                                   (0x00000400&((data)<<10))
#define SB1_HI_PRI_wbuf_rule_src(data)                                               ((0x00000400&(data))>>10)
#define SB1_HI_PRI_write_enable1_shift                                               (9)
#define SB1_HI_PRI_write_enable1_mask                                                (0x00000200)
#define SB1_HI_PRI_write_enable1(data)                                               (0x00000200&((data)<<9))
#define SB1_HI_PRI_write_enable1_src(data)                                           ((0x00000200&(data))>>9)
#define SB1_HI_PRI_ip_shift                                                          (0)
#define SB1_HI_PRI_ip_mask                                                           (0x000001FF)
#define SB1_HI_PRI_ip(data)                                                          (0x000001FF&((data)<<0))
#define SB1_HI_PRI_ip_src(data)                                                      ((0x000001FF&(data))>>0)


#endif
