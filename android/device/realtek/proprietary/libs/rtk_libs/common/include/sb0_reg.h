/**************************************************************
// Spec Version                  : 0.1.0
// Parser Version                : MAR_Parser_1.0
// CModelGen Version             : 1.0 2006.04.18
// Naming Rule                   : Module_Register_Name_reg
// Firmware Header Generate Date : 2007/7/16
***************************************************************/


#ifndef _SB0_REG_H_INCLUDED_
#define _SB0_REG_H_INCLUDED_
#ifdef  _SB0_USE_STRUCT
typedef struct 
{
unsigned int     write_enable3:1;
unsigned int     dmy:22;
unsigned int     write_enable2:1;
unsigned int     wl:6;
unsigned int     write_enable1:1;
unsigned int     rr:1;
}SB0_CTRL;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     aio:3;
unsigned int     vi:3;
unsigned int     vo:3;
}SB0_PRIORITY;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     status_id:3;
unsigned int     reserved_1:1;
unsigned int     mask_id:3;
}SB0_MASK;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     write_enable2:1;
unsigned int     pc2_go:1;
unsigned int     pc2_sel:3;
unsigned int     reserved_1:3;
unsigned int     write_enable1:1;
unsigned int     pc1_go:1;
unsigned int     pc1_sel:3;
}SB0_PC_CTRL;

typedef struct 
{
unsigned int     tol:32;
}SB0_PC1_CYC;

typedef struct 
{
unsigned int     tol_cyc:32;
}SB0_PC1_ACK;

typedef struct 
{
unsigned int     req_lat:32;
}SB0_PC1_ACC;

typedef struct 
{
unsigned int     max_lat:16;
unsigned int     tol_num:16;
}SB0_PC1_REQ;

typedef struct 
{
unsigned int     tol:32;
}SB0_PC2_CYC;

typedef struct 
{
unsigned int     tol_cyc:32;
}SB0_PC2_ACK;

typedef struct 
{
unsigned int     req_lat:32;
}SB0_PC2_ACC;

typedef struct 
{
unsigned int     max_lat:16;
unsigned int     tol_num:16;
}SB0_PC2_REQ;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     write_enable3:1;
unsigned int     sel1:4;
unsigned int     write_enable2:1;
unsigned int     sel0:4;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}SB0_DBG;

#endif

#define SB0_CTRL                                                                     0x1801C000
#define SB0_CTRL_reg_addr                                                            "0xB801C000"
#define SB0_CTRL_reg                                                                 0xB801C000
#define SB0_CTRL_inst_adr                                                            "0x0000"
#define SB0_CTRL_inst                                                                0x0000
#define SB0_CTRL_write_enable3_shift                                                 (31)
#define SB0_CTRL_write_enable3_mask                                                  (0x80000000)
#define SB0_CTRL_write_enable3(data)                                                 (0x80000000&((data)<<31))
#define SB0_CTRL_write_enable3_src(data)                                             ((0x80000000&(data))>>31)
#define SB0_CTRL_dmy_shift                                                           (9)
#define SB0_CTRL_dmy_mask                                                            (0x7FFFFE00)
#define SB0_CTRL_dmy(data)                                                           (0x7FFFFE00&((data)<<9))
#define SB0_CTRL_dmy_src(data)                                                       ((0x7FFFFE00&(data))>>9)
#define SB0_CTRL_write_enable2_shift                                                 (8)
#define SB0_CTRL_write_enable2_mask                                                  (0x00000100)
#define SB0_CTRL_write_enable2(data)                                                 (0x00000100&((data)<<8))
#define SB0_CTRL_write_enable2_src(data)                                             ((0x00000100&(data))>>8)
#define SB0_CTRL_wl_shift                                                            (2)
#define SB0_CTRL_wl_mask                                                             (0x000000FC)
#define SB0_CTRL_wl(data)                                                            (0x000000FC&((data)<<2))
#define SB0_CTRL_wl_src(data)                                                        ((0x000000FC&(data))>>2)
#define SB0_CTRL_write_enable1_shift                                                 (1)
#define SB0_CTRL_write_enable1_mask                                                  (0x00000002)
#define SB0_CTRL_write_enable1(data)                                                 (0x00000002&((data)<<1))
#define SB0_CTRL_write_enable1_src(data)                                             ((0x00000002&(data))>>1)
#define SB0_CTRL_rr_shift                                                            (0)
#define SB0_CTRL_rr_mask                                                             (0x00000001)
#define SB0_CTRL_rr(data)                                                            (0x00000001&((data)<<0))
#define SB0_CTRL_rr_src(data)                                                        ((0x00000001&(data))>>0)


#define SB0_PRIORITY                                                                 0x1801C004
#define SB0_PRIORITY_reg_addr                                                        "0xB801C004"
#define SB0_PRIORITY_reg                                                             0xB801C004
#define SB0_PRIORITY_inst_adr                                                        "0x0001"
#define SB0_PRIORITY_inst                                                            0x0001
#define SB0_PRIORITY_aio_shift                                                       (6)
#define SB0_PRIORITY_aio_mask                                                        (0x000001C0)
#define SB0_PRIORITY_aio(data)                                                       (0x000001C0&((data)<<6))
#define SB0_PRIORITY_aio_src(data)                                                   ((0x000001C0&(data))>>6)
#define SB0_PRIORITY_vi_shift                                                        (3)
#define SB0_PRIORITY_vi_mask                                                         (0x00000038)
#define SB0_PRIORITY_vi(data)                                                        (0x00000038&((data)<<3))
#define SB0_PRIORITY_vi_src(data)                                                    ((0x00000038&(data))>>3)
#define SB0_PRIORITY_vo_shift                                                        (0)
#define SB0_PRIORITY_vo_mask                                                         (0x00000007)
#define SB0_PRIORITY_vo(data)                                                        (0x00000007&((data)<<0))
#define SB0_PRIORITY_vo_src(data)                                                    ((0x00000007&(data))>>0)


#define SB0_MASK                                                                     0x1801C008
#define SB0_MASK_reg_addr                                                            "0xB801C008"
#define SB0_MASK_reg                                                                 0xB801C008
#define SB0_MASK_inst_adr                                                            "0x0002"
#define SB0_MASK_inst                                                                0x0002
#define SB0_MASK_status_id_shift                                                     (4)
#define SB0_MASK_status_id_mask                                                      (0x00000070)
#define SB0_MASK_status_id(data)                                                     (0x00000070&((data)<<4))
#define SB0_MASK_status_id_src(data)                                                 ((0x00000070&(data))>>4)
#define SB0_MASK_mask_id_shift                                                       (0)
#define SB0_MASK_mask_id_mask                                                        (0x00000007)
#define SB0_MASK_mask_id(data)                                                       (0x00000007&((data)<<0))
#define SB0_MASK_mask_id_src(data)                                                   ((0x00000007&(data))>>0)


#define SB0_PC_CTRL                                                                  0x1801C00C
#define SB0_PC_CTRL_reg_addr                                                         "0xB801C00C"
#define SB0_PC_CTRL_reg                                                              0xB801C00C
#define SB0_PC_CTRL_inst_adr                                                         "0x0003"
#define SB0_PC_CTRL_inst                                                             0x0003
#define SB0_PC_CTRL_write_enable2_shift                                              (12)
#define SB0_PC_CTRL_write_enable2_mask                                               (0x00001000)
#define SB0_PC_CTRL_write_enable2(data)                                              (0x00001000&((data)<<12))
#define SB0_PC_CTRL_write_enable2_src(data)                                          ((0x00001000&(data))>>12)
#define SB0_PC_CTRL_pc2_go_shift                                                     (11)
#define SB0_PC_CTRL_pc2_go_mask                                                      (0x00000800)
#define SB0_PC_CTRL_pc2_go(data)                                                     (0x00000800&((data)<<11))
#define SB0_PC_CTRL_pc2_go_src(data)                                                 ((0x00000800&(data))>>11)
#define SB0_PC_CTRL_pc2_sel_shift                                                    (8)
#define SB0_PC_CTRL_pc2_sel_mask                                                     (0x00000700)
#define SB0_PC_CTRL_pc2_sel(data)                                                    (0x00000700&((data)<<8))
#define SB0_PC_CTRL_pc2_sel_src(data)                                                ((0x00000700&(data))>>8)
#define SB0_PC_CTRL_write_enable1_shift                                              (4)
#define SB0_PC_CTRL_write_enable1_mask                                               (0x00000010)
#define SB0_PC_CTRL_write_enable1(data)                                              (0x00000010&((data)<<4))
#define SB0_PC_CTRL_write_enable1_src(data)                                          ((0x00000010&(data))>>4)
#define SB0_PC_CTRL_pc1_go_shift                                                     (3)
#define SB0_PC_CTRL_pc1_go_mask                                                      (0x00000008)
#define SB0_PC_CTRL_pc1_go(data)                                                     (0x00000008&((data)<<3))
#define SB0_PC_CTRL_pc1_go_src(data)                                                 ((0x00000008&(data))>>3)
#define SB0_PC_CTRL_pc1_sel_shift                                                    (0)
#define SB0_PC_CTRL_pc1_sel_mask                                                     (0x00000007)
#define SB0_PC_CTRL_pc1_sel(data)                                                    (0x00000007&((data)<<0))
#define SB0_PC_CTRL_pc1_sel_src(data)                                                ((0x00000007&(data))>>0)


#define SB0_PC1_CYC                                                                  0x1801C010
#define SB0_PC1_CYC_reg_addr                                                         "0xB801C010"
#define SB0_PC1_CYC_reg                                                              0xB801C010
#define SB0_PC1_CYC_inst_adr                                                         "0x0004"
#define SB0_PC1_CYC_inst                                                             0x0004
#define SB0_PC1_CYC_tol_shift                                                        (0)
#define SB0_PC1_CYC_tol_mask                                                         (0xFFFFFFFF)
#define SB0_PC1_CYC_tol(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB0_PC1_CYC_tol_src(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB0_PC1_ACK                                                                  0x1801C014
#define SB0_PC1_ACK_reg_addr                                                         "0xB801C014"
#define SB0_PC1_ACK_reg                                                              0xB801C014
#define SB0_PC1_ACK_inst_adr                                                         "0x0005"
#define SB0_PC1_ACK_inst                                                             0x0005
#define SB0_PC1_ACK_tol_cyc_shift                                                    (0)
#define SB0_PC1_ACK_tol_cyc_mask                                                     (0xFFFFFFFF)
#define SB0_PC1_ACK_tol_cyc(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB0_PC1_ACK_tol_cyc_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB0_PC1_ACC                                                                  0x1801C018
#define SB0_PC1_ACC_reg_addr                                                         "0xB801C018"
#define SB0_PC1_ACC_reg                                                              0xB801C018
#define SB0_PC1_ACC_inst_adr                                                         "0x0006"
#define SB0_PC1_ACC_inst                                                             0x0006
#define SB0_PC1_ACC_req_lat_shift                                                    (0)
#define SB0_PC1_ACC_req_lat_mask                                                     (0xFFFFFFFF)
#define SB0_PC1_ACC_req_lat(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB0_PC1_ACC_req_lat_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB0_PC1_REQ                                                                  0x1801C01C
#define SB0_PC1_REQ_reg_addr                                                         "0xB801C01C"
#define SB0_PC1_REQ_reg                                                              0xB801C01C
#define SB0_PC1_REQ_inst_adr                                                         "0x0007"
#define SB0_PC1_REQ_inst                                                             0x0007
#define SB0_PC1_REQ_max_lat_shift                                                    (16)
#define SB0_PC1_REQ_max_lat_mask                                                     (0xFFFF0000)
#define SB0_PC1_REQ_max_lat(data)                                                    (0xFFFF0000&((data)<<16))
#define SB0_PC1_REQ_max_lat_src(data)                                                ((0xFFFF0000&(data))>>16)
#define SB0_PC1_REQ_tol_num_shift                                                    (0)
#define SB0_PC1_REQ_tol_num_mask                                                     (0x0000FFFF)
#define SB0_PC1_REQ_tol_num(data)                                                    (0x0000FFFF&((data)<<0))
#define SB0_PC1_REQ_tol_num_src(data)                                                ((0x0000FFFF&(data))>>0)


#define SB0_PC2_CYC                                                                  0x1801C020
#define SB0_PC2_CYC_reg_addr                                                         "0xB801C020"
#define SB0_PC2_CYC_reg                                                              0xB801C020
#define SB0_PC2_CYC_inst_adr                                                         "0x0008"
#define SB0_PC2_CYC_inst                                                             0x0008
#define SB0_PC2_CYC_tol_shift                                                        (0)
#define SB0_PC2_CYC_tol_mask                                                         (0xFFFFFFFF)
#define SB0_PC2_CYC_tol(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB0_PC2_CYC_tol_src(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB0_PC2_ACK                                                                  0x1801C024
#define SB0_PC2_ACK_reg_addr                                                         "0xB801C024"
#define SB0_PC2_ACK_reg                                                              0xB801C024
#define SB0_PC2_ACK_inst_adr                                                         "0x0009"
#define SB0_PC2_ACK_inst                                                             0x0009
#define SB0_PC2_ACK_tol_cyc_shift                                                    (0)
#define SB0_PC2_ACK_tol_cyc_mask                                                     (0xFFFFFFFF)
#define SB0_PC2_ACK_tol_cyc(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB0_PC2_ACK_tol_cyc_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB0_PC2_ACC                                                                  0x1801C028
#define SB0_PC2_ACC_reg_addr                                                         "0xB801C028"
#define SB0_PC2_ACC_reg                                                              0xB801C028
#define SB0_PC2_ACC_inst_adr                                                         "0x000A"
#define SB0_PC2_ACC_inst                                                             0x000A
#define SB0_PC2_ACC_req_lat_shift                                                    (0)
#define SB0_PC2_ACC_req_lat_mask                                                     (0xFFFFFFFF)
#define SB0_PC2_ACC_req_lat(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB0_PC2_ACC_req_lat_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB0_PC2_REQ                                                                  0x1801C02C
#define SB0_PC2_REQ_reg_addr                                                         "0xB801C02C"
#define SB0_PC2_REQ_reg                                                              0xB801C02C
#define SB0_PC2_REQ_inst_adr                                                         "0x000B"
#define SB0_PC2_REQ_inst                                                             0x000B
#define SB0_PC2_REQ_max_lat_shift                                                    (16)
#define SB0_PC2_REQ_max_lat_mask                                                     (0xFFFF0000)
#define SB0_PC2_REQ_max_lat(data)                                                    (0xFFFF0000&((data)<<16))
#define SB0_PC2_REQ_max_lat_src(data)                                                ((0xFFFF0000&(data))>>16)
#define SB0_PC2_REQ_tol_num_shift                                                    (0)
#define SB0_PC2_REQ_tol_num_mask                                                     (0x0000FFFF)
#define SB0_PC2_REQ_tol_num(data)                                                    (0x0000FFFF&((data)<<0))
#define SB0_PC2_REQ_tol_num_src(data)                                                ((0x0000FFFF&(data))>>0)


#define SB0_DBG                                                                      0x1801C030
#define SB0_DBG_reg_addr                                                             "0xB801C030"
#define SB0_DBG_reg                                                                  0xB801C030
#define SB0_DBG_inst_adr                                                             "0x000C"
#define SB0_DBG_inst                                                                 0x000C
#define SB0_DBG_write_enable3_shift                                                  (11)
#define SB0_DBG_write_enable3_mask                                                   (0x00000800)
#define SB0_DBG_write_enable3(data)                                                  (0x00000800&((data)<<11))
#define SB0_DBG_write_enable3_src(data)                                              ((0x00000800&(data))>>11)
#define SB0_DBG_sel1_shift                                                           (7)
#define SB0_DBG_sel1_mask                                                            (0x00000780)
#define SB0_DBG_sel1(data)                                                           (0x00000780&((data)<<7))
#define SB0_DBG_sel1_src(data)                                                       ((0x00000780&(data))>>7)
#define SB0_DBG_write_enable2_shift                                                  (6)
#define SB0_DBG_write_enable2_mask                                                   (0x00000040)
#define SB0_DBG_write_enable2(data)                                                  (0x00000040&((data)<<6))
#define SB0_DBG_write_enable2_src(data)                                              ((0x00000040&(data))>>6)
#define SB0_DBG_sel0_shift                                                           (2)
#define SB0_DBG_sel0_mask                                                            (0x0000003C)
#define SB0_DBG_sel0(data)                                                           (0x0000003C&((data)<<2))
#define SB0_DBG_sel0_src(data)                                                       ((0x0000003C&(data))>>2)
#define SB0_DBG_write_enable1_shift                                                  (1)
#define SB0_DBG_write_enable1_mask                                                   (0x00000002)
#define SB0_DBG_write_enable1(data)                                                  (0x00000002&((data)<<1))
#define SB0_DBG_write_enable1_src(data)                                              ((0x00000002&(data))>>1)
#define SB0_DBG_enable_shift                                                         (0)
#define SB0_DBG_enable_mask                                                          (0x00000001)
#define SB0_DBG_enable(data)                                                         (0x00000001&((data)<<0))
#define SB0_DBG_enable_src(data)                                                     ((0x00000001&(data))>>0)


#endif
