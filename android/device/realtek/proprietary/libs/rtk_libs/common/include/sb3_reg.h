/**************************************************************
// Spec Version                  : 0.1.2
// Parser Version                : MAR_Parser_1.0
// CModelGen Version             : 1.0 2006.04.18
// Naming Rule                   : Module_Register_Name
// Firmware Header Generate Date : 2007/12/7
***************************************************************/


#ifndef _SB3_REG_H_INCLUDED_
#define _SB3_REG_H_INCLUDED_
#ifdef  _SB3_USE_STRUCT
typedef struct 
{
unsigned int     write_enable3:1;
unsigned int     dmy:22;
unsigned int     write_enable2:1;
unsigned int     wl:6;
unsigned int     write_enable1:1;
unsigned int     rr:1;
}SB3_CTRL;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     aee:5;
unsigned int     md:5;
unsigned int     vde:5;
unsigned int     sde:5;
unsigned int     ade:5;
}SB3_PRIORITY;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     status_id:5;
unsigned int     reserved_1:3;
unsigned int     mask_id:5;
}SB3_MASK;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     write_enable2:1;
unsigned int     pc2_go:1;
unsigned int     pc2_sel:5;
unsigned int     reserved_1:1;
unsigned int     write_enable1:1;
unsigned int     pc1_go:1;
unsigned int     pc1_sel:5;
}SB3_PC_CTRL;

typedef struct 
{
unsigned int     tol:32;
}SB3_PC1_CYC;

typedef struct 
{
unsigned int     tol_cyc:32;
}SB3_PC1_ACK;

typedef struct 
{
unsigned int     req_lat:32;
}SB3_PC1_ACC;

typedef struct 
{
unsigned int     max_lat:16;
unsigned int     tol_num:16;
}SB3_PC1_REQ;

typedef struct 
{
unsigned int     tol:32;
}SB3_PC2_CYC;

typedef struct 
{
unsigned int     tol_cyc:32;
}SB3_PC2_ACK;

typedef struct 
{
unsigned int     req_lat:32;
}SB3_PC2_ACC;

typedef struct 
{
unsigned int     max_lat:16;
unsigned int     tol_num:16;
}SB3_PC2_REQ;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     write_enable3:1;
unsigned int     sel1:5;
unsigned int     write_enable2:1;
unsigned int     sel0:5;
unsigned int     write_enable1:1;
unsigned int     enable:1;
}SB3_DBG;

#endif

#define SB3_CTRL                                                                     0x1801C400
#define SB3_CTRL_reg_addr                                                            "0xB801C400"
#define SB3_CTRL_reg                                                                 0xB801C400
#define SB3_CTRL_inst_adr                                                            "0x0000"
#define SB3_CTRL_inst                                                                0x0000
#define SB3_CTRL_write_enable3_shift                                                 (31)
#define SB3_CTRL_write_enable3_mask                                                  (0x80000000)
#define SB3_CTRL_write_enable3(data)                                                 (0x80000000&((data)<<31))
#define SB3_CTRL_write_enable3_src(data)                                             ((0x80000000&(data))>>31)
#define SB3_CTRL_dmy_shift                                                           (9)
#define SB3_CTRL_dmy_mask                                                            (0x7FFFFE00)
#define SB3_CTRL_dmy(data)                                                           (0x7FFFFE00&((data)<<9))
#define SB3_CTRL_dmy_src(data)                                                       ((0x7FFFFE00&(data))>>9)
#define SB3_CTRL_write_enable2_shift                                                 (8)
#define SB3_CTRL_write_enable2_mask                                                  (0x00000100)
#define SB3_CTRL_write_enable2(data)                                                 (0x00000100&((data)<<8))
#define SB3_CTRL_write_enable2_src(data)                                             ((0x00000100&(data))>>8)
#define SB3_CTRL_wl_shift                                                            (2)
#define SB3_CTRL_wl_mask                                                             (0x000000FC)
#define SB3_CTRL_wl(data)                                                            (0x000000FC&((data)<<2))
#define SB3_CTRL_wl_src(data)                                                        ((0x000000FC&(data))>>2)
#define SB3_CTRL_write_enable1_shift                                                 (1)
#define SB3_CTRL_write_enable1_mask                                                  (0x00000002)
#define SB3_CTRL_write_enable1(data)                                                 (0x00000002&((data)<<1))
#define SB3_CTRL_write_enable1_src(data)                                             ((0x00000002&(data))>>1)
#define SB3_CTRL_rr_shift                                                            (0)
#define SB3_CTRL_rr_mask                                                             (0x00000001)
#define SB3_CTRL_rr(data)                                                            (0x00000001&((data)<<0))
#define SB3_CTRL_rr_src(data)                                                        ((0x00000001&(data))>>0)


#define SB3_PRIORITY                                                                 0x1801C404
#define SB3_PRIORITY_reg_addr                                                        "0xB801C404"
#define SB3_PRIORITY_reg                                                             0xB801C404
#define SB3_PRIORITY_inst_adr                                                        "0x0001"
#define SB3_PRIORITY_inst                                                            0x0001
#define SB3_PRIORITY_aee_shift                                                       (20)
#define SB3_PRIORITY_aee_mask                                                        (0x01F00000)
#define SB3_PRIORITY_aee(data)                                                       (0x01F00000&((data)<<20))
#define SB3_PRIORITY_aee_src(data)                                                   ((0x01F00000&(data))>>20)
#define SB3_PRIORITY_md_shift                                                        (15)
#define SB3_PRIORITY_md_mask                                                         (0x000F8000)
#define SB3_PRIORITY_md(data)                                                        (0x000F8000&((data)<<15))
#define SB3_PRIORITY_md_src(data)                                                    ((0x000F8000&(data))>>15)
#define SB3_PRIORITY_vde_shift                                                       (10)
#define SB3_PRIORITY_vde_mask                                                        (0x00007C00)
#define SB3_PRIORITY_vde(data)                                                       (0x00007C00&((data)<<10))
#define SB3_PRIORITY_vde_src(data)                                                   ((0x00007C00&(data))>>10)
#define SB3_PRIORITY_sde_shift                                                       (5)
#define SB3_PRIORITY_sde_mask                                                        (0x000003E0)
#define SB3_PRIORITY_sde(data)                                                       (0x000003E0&((data)<<5))
#define SB3_PRIORITY_sde_src(data)                                                   ((0x000003E0&(data))>>5)
#define SB3_PRIORITY_ade_shift                                                       (0)
#define SB3_PRIORITY_ade_mask                                                        (0x0000001F)
#define SB3_PRIORITY_ade(data)                                                       (0x0000001F&((data)<<0))
#define SB3_PRIORITY_ade_src(data)                                                   ((0x0000001F&(data))>>0)


#define SB3_MASK                                                                     0x1801C408
#define SB3_MASK_reg_addr                                                            "0xB801C408"
#define SB3_MASK_reg                                                                 0xB801C408
#define SB3_MASK_inst_adr                                                            "0x0002"
#define SB3_MASK_inst                                                                0x0002
#define SB3_MASK_status_id_shift                                                     (8)
#define SB3_MASK_status_id_mask                                                      (0x00001F00)
#define SB3_MASK_status_id(data)                                                     (0x00001F00&((data)<<8))
#define SB3_MASK_status_id_src(data)                                                 ((0x00001F00&(data))>>8)
#define SB3_MASK_mask_id_shift                                                       (0)
#define SB3_MASK_mask_id_mask                                                        (0x0000001F)
#define SB3_MASK_mask_id(data)                                                       (0x0000001F&((data)<<0))
#define SB3_MASK_mask_id_src(data)                                                   ((0x0000001F&(data))>>0)


#define SB3_PC_CTRL                                                                  0x1801C40C
#define SB3_PC_CTRL_reg_addr                                                         "0xB801C40C"
#define SB3_PC_CTRL_reg                                                              0xB801C40C
#define SB3_PC_CTRL_inst_adr                                                         "0x0003"
#define SB3_PC_CTRL_inst                                                             0x0003
#define SB3_PC_CTRL_write_enable2_shift                                              (14)
#define SB3_PC_CTRL_write_enable2_mask                                               (0x00004000)
#define SB3_PC_CTRL_write_enable2(data)                                              (0x00004000&((data)<<14))
#define SB3_PC_CTRL_write_enable2_src(data)                                          ((0x00004000&(data))>>14)
#define SB3_PC_CTRL_pc2_go_shift                                                     (13)
#define SB3_PC_CTRL_pc2_go_mask                                                      (0x00002000)
#define SB3_PC_CTRL_pc2_go(data)                                                     (0x00002000&((data)<<13))
#define SB3_PC_CTRL_pc2_go_src(data)                                                 ((0x00002000&(data))>>13)
#define SB3_PC_CTRL_pc2_sel_shift                                                    (8)
#define SB3_PC_CTRL_pc2_sel_mask                                                     (0x00001F00)
#define SB3_PC_CTRL_pc2_sel(data)                                                    (0x00001F00&((data)<<8))
#define SB3_PC_CTRL_pc2_sel_src(data)                                                ((0x00001F00&(data))>>8)
#define SB3_PC_CTRL_write_enable1_shift                                              (6)
#define SB3_PC_CTRL_write_enable1_mask                                               (0x00000040)
#define SB3_PC_CTRL_write_enable1(data)                                              (0x00000040&((data)<<6))
#define SB3_PC_CTRL_write_enable1_src(data)                                          ((0x00000040&(data))>>6)
#define SB3_PC_CTRL_pc1_go_shift                                                     (5)
#define SB3_PC_CTRL_pc1_go_mask                                                      (0x00000020)
#define SB3_PC_CTRL_pc1_go(data)                                                     (0x00000020&((data)<<5))
#define SB3_PC_CTRL_pc1_go_src(data)                                                 ((0x00000020&(data))>>5)
#define SB3_PC_CTRL_pc1_sel_shift                                                    (0)
#define SB3_PC_CTRL_pc1_sel_mask                                                     (0x0000001F)
#define SB3_PC_CTRL_pc1_sel(data)                                                    (0x0000001F&((data)<<0))
#define SB3_PC_CTRL_pc1_sel_src(data)                                                ((0x0000001F&(data))>>0)


#define SB3_PC1_CYC                                                                  0x1801C410
#define SB3_PC1_CYC_reg_addr                                                         "0xB801C410"
#define SB3_PC1_CYC_reg                                                              0xB801C410
#define SB3_PC1_CYC_inst_adr                                                         "0x0004"
#define SB3_PC1_CYC_inst                                                             0x0004
#define SB3_PC1_CYC_tol_shift                                                        (0)
#define SB3_PC1_CYC_tol_mask                                                         (0xFFFFFFFF)
#define SB3_PC1_CYC_tol(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB3_PC1_CYC_tol_src(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB3_PC1_ACK                                                                  0x1801C414
#define SB3_PC1_ACK_reg_addr                                                         "0xB801C414"
#define SB3_PC1_ACK_reg                                                              0xB801C414
#define SB3_PC1_ACK_inst_adr                                                         "0x0005"
#define SB3_PC1_ACK_inst                                                             0x0005
#define SB3_PC1_ACK_tol_cyc_shift                                                    (0)
#define SB3_PC1_ACK_tol_cyc_mask                                                     (0xFFFFFFFF)
#define SB3_PC1_ACK_tol_cyc(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB3_PC1_ACK_tol_cyc_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB3_PC1_ACC                                                                  0x1801C418
#define SB3_PC1_ACC_reg_addr                                                         "0xB801C418"
#define SB3_PC1_ACC_reg                                                              0xB801C418
#define SB3_PC1_ACC_inst_adr                                                         "0x0006"
#define SB3_PC1_ACC_inst                                                             0x0006
#define SB3_PC1_ACC_req_lat_shift                                                    (0)
#define SB3_PC1_ACC_req_lat_mask                                                     (0xFFFFFFFF)
#define SB3_PC1_ACC_req_lat(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB3_PC1_ACC_req_lat_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB3_PC1_REQ                                                                  0x1801C41C
#define SB3_PC1_REQ_reg_addr                                                         "0xB801C41C"
#define SB3_PC1_REQ_reg                                                              0xB801C41C
#define SB3_PC1_REQ_inst_adr                                                         "0x0007"
#define SB3_PC1_REQ_inst                                                             0x0007
#define SB3_PC1_REQ_max_lat_shift                                                    (16)
#define SB3_PC1_REQ_max_lat_mask                                                     (0xFFFF0000)
#define SB3_PC1_REQ_max_lat(data)                                                    (0xFFFF0000&((data)<<16))
#define SB3_PC1_REQ_max_lat_src(data)                                                ((0xFFFF0000&(data))>>16)
#define SB3_PC1_REQ_tol_num_shift                                                    (0)
#define SB3_PC1_REQ_tol_num_mask                                                     (0x0000FFFF)
#define SB3_PC1_REQ_tol_num(data)                                                    (0x0000FFFF&((data)<<0))
#define SB3_PC1_REQ_tol_num_src(data)                                                ((0x0000FFFF&(data))>>0)


#define SB3_PC2_CYC                                                                  0x1801C420
#define SB3_PC2_CYC_reg_addr                                                         "0xB801C420"
#define SB3_PC2_CYC_reg                                                              0xB801C420
#define SB3_PC2_CYC_inst_adr                                                         "0x0008"
#define SB3_PC2_CYC_inst                                                             0x0008
#define SB3_PC2_CYC_tol_shift                                                        (0)
#define SB3_PC2_CYC_tol_mask                                                         (0xFFFFFFFF)
#define SB3_PC2_CYC_tol(data)                                                        (0xFFFFFFFF&((data)<<0))
#define SB3_PC2_CYC_tol_src(data)                                                    ((0xFFFFFFFF&(data))>>0)


#define SB3_PC2_ACK                                                                  0x1801C424
#define SB3_PC2_ACK_reg_addr                                                         "0xB801C424"
#define SB3_PC2_ACK_reg                                                              0xB801C424
#define SB3_PC2_ACK_inst_adr                                                         "0x0009"
#define SB3_PC2_ACK_inst                                                             0x0009
#define SB3_PC2_ACK_tol_cyc_shift                                                    (0)
#define SB3_PC2_ACK_tol_cyc_mask                                                     (0xFFFFFFFF)
#define SB3_PC2_ACK_tol_cyc(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB3_PC2_ACK_tol_cyc_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB3_PC2_ACC                                                                  0x1801C428
#define SB3_PC2_ACC_reg_addr                                                         "0xB801C428"
#define SB3_PC2_ACC_reg                                                              0xB801C428
#define SB3_PC2_ACC_inst_adr                                                         "0x000A"
#define SB3_PC2_ACC_inst                                                             0x000A
#define SB3_PC2_ACC_req_lat_shift                                                    (0)
#define SB3_PC2_ACC_req_lat_mask                                                     (0xFFFFFFFF)
#define SB3_PC2_ACC_req_lat(data)                                                    (0xFFFFFFFF&((data)<<0))
#define SB3_PC2_ACC_req_lat_src(data)                                                ((0xFFFFFFFF&(data))>>0)


#define SB3_PC2_REQ                                                                  0x1801C42C
#define SB3_PC2_REQ_reg_addr                                                         "0xB801C42C"
#define SB3_PC2_REQ_reg                                                              0xB801C42C
#define SB3_PC2_REQ_inst_adr                                                         "0x000B"
#define SB3_PC2_REQ_inst                                                             0x000B
#define SB3_PC2_REQ_max_lat_shift                                                    (16)
#define SB3_PC2_REQ_max_lat_mask                                                     (0xFFFF0000)
#define SB3_PC2_REQ_max_lat(data)                                                    (0xFFFF0000&((data)<<16))
#define SB3_PC2_REQ_max_lat_src(data)                                                ((0xFFFF0000&(data))>>16)
#define SB3_PC2_REQ_tol_num_shift                                                    (0)
#define SB3_PC2_REQ_tol_num_mask                                                     (0x0000FFFF)
#define SB3_PC2_REQ_tol_num(data)                                                    (0x0000FFFF&((data)<<0))
#define SB3_PC2_REQ_tol_num_src(data)                                                ((0x0000FFFF&(data))>>0)


#define SB3_DBG                                                                      0x1801C430
#define SB3_DBG_reg_addr                                                             "0xB801C430"
#define SB3_DBG_reg                                                                  0xB801C430
#define SB3_DBG_inst_adr                                                             "0x000C"
#define SB3_DBG_inst                                                                 0x000C
#define SB3_DBG_write_enable3_shift                                                  (13)
#define SB3_DBG_write_enable3_mask                                                   (0x00002000)
#define SB3_DBG_write_enable3(data)                                                  (0x00002000&((data)<<13))
#define SB3_DBG_write_enable3_src(data)                                              ((0x00002000&(data))>>13)
#define SB3_DBG_sel1_shift                                                           (8)
#define SB3_DBG_sel1_mask                                                            (0x00001F00)
#define SB3_DBG_sel1(data)                                                           (0x00001F00&((data)<<8))
#define SB3_DBG_sel1_src(data)                                                       ((0x00001F00&(data))>>8)
#define SB3_DBG_write_enable2_shift                                                  (7)
#define SB3_DBG_write_enable2_mask                                                   (0x00000080)
#define SB3_DBG_write_enable2(data)                                                  (0x00000080&((data)<<7))
#define SB3_DBG_write_enable2_src(data)                                              ((0x00000080&(data))>>7)
#define SB3_DBG_sel0_shift                                                           (2)
#define SB3_DBG_sel0_mask                                                            (0x0000007C)
#define SB3_DBG_sel0(data)                                                           (0x0000007C&((data)<<2))
#define SB3_DBG_sel0_src(data)                                                       ((0x0000007C&(data))>>2)
#define SB3_DBG_write_enable1_shift                                                  (1)
#define SB3_DBG_write_enable1_mask                                                   (0x00000002)
#define SB3_DBG_write_enable1(data)                                                  (0x00000002&((data)<<1))
#define SB3_DBG_write_enable1_src(data)                                              ((0x00000002&(data))>>1)
#define SB3_DBG_enable_shift                                                         (0)
#define SB3_DBG_enable_mask                                                          (0x00000001)
#define SB3_DBG_enable(data)                                                         (0x00000001&((data)<<0))
#define SB3_DBG_enable_src(data)                                                     ((0x00000001&(data))>>0)


#endif
