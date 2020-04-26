/**************************************************************
// Spec Version                  : 0.0.5
// Parser Version                : MAR_Parser_1.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name_reg
// Firmware Header Generate Date : 2009/5/12
***************************************************************/


#ifndef _SBX_REG_H_INCLUDED_
#define _SBX_REG_H_INCLUDED_
#ifdef  _SBX_USE_STRUCT
typedef struct 
{
unsigned int     tp:8;
unsigned int     etn:8;
unsigned int     pcie:8;
unsigned int     sata:8;
}SB1_PRIORITY_1;

typedef struct 
{
unsigned int     nf:8;
unsigned int     usb:8;
unsigned int     cp:8;
unsigned int     cr:8;
}SB1_PRIORITY_2;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     index:8;
}SB1_CHANNEL_HIGH_PRIORITY;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     index:8;
}SB1_CHANNEL_REQ_MASK;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     index:8;
}SB1_CHANNEL_REQ_BUSY;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     allow_cmd_cnt:3;
}SB1_MISC_CTRL;

typedef struct 
{
unsigned int     md:8;
unsigned int     vde:8;
unsigned int     ade:8;
unsigned int     aee:8;
}SB3_PRIORITY_1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     ve:8;
}SB3_PRIORITY_2;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     index:8;
}SB3_CHANNEL_HIGH_PRIORITY;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     index:8;
}SB3_CHANNEL_REQ_MASK;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     index:8;
}SB3_CHANNEL_REQ_BUSY;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     allow_cmd_cnt:3;
}SB3_MISC_CTRL;

#endif

#define SB1_PRIORITY_1                                                               0x1801C000
#define SB1_PRIORITY_1_reg_addr                                                      "0xB801C000"
#define SB1_PRIORITY_1_reg                                                           0xB801C000
#define set_SB1_PRIORITY_1_reg(data)   (*((volatile unsigned int*) SB1_PRIORITY_1_reg)=data)
#define get_SB1_PRIORITY_1_reg   (*((volatile unsigned int*) SB1_PRIORITY_1_reg))
#define SB1_PRIORITY_1_inst_adr                                                      "0x0000"
#define SB1_PRIORITY_1_inst                                                          0x0000
#define SB1_PRIORITY_1_tp_shift                                                      (24)
#define SB1_PRIORITY_1_tp_mask                                                       (0xFF000000)
#define SB1_PRIORITY_1_tp(data)                                                      (0xFF000000&((data)<<24))
#define SB1_PRIORITY_1_tp_src(data)                                                  ((0xFF000000&(data))>>24)
#define SB1_PRIORITY_1_get_tp(data)                                                  ((0xFF000000&(data))>>24)
#define SB1_PRIORITY_1_etn_shift                                                     (16)
#define SB1_PRIORITY_1_etn_mask                                                      (0x00FF0000)
#define SB1_PRIORITY_1_etn(data)                                                     (0x00FF0000&((data)<<16))
#define SB1_PRIORITY_1_etn_src(data)                                                 ((0x00FF0000&(data))>>16)
#define SB1_PRIORITY_1_get_etn(data)                                                 ((0x00FF0000&(data))>>16)
#define SB1_PRIORITY_1_pcie_shift                                                    (8)
#define SB1_PRIORITY_1_pcie_mask                                                     (0x0000FF00)
#define SB1_PRIORITY_1_pcie(data)                                                    (0x0000FF00&((data)<<8))
#define SB1_PRIORITY_1_pcie_src(data)                                                ((0x0000FF00&(data))>>8)
#define SB1_PRIORITY_1_get_pcie(data)                                                ((0x0000FF00&(data))>>8)
#define SB1_PRIORITY_1_sata_shift                                                    (0)
#define SB1_PRIORITY_1_sata_mask                                                     (0x000000FF)
#define SB1_PRIORITY_1_sata(data)                                                    (0x000000FF&((data)<<0))
#define SB1_PRIORITY_1_sata_src(data)                                                ((0x000000FF&(data))>>0)
#define SB1_PRIORITY_1_get_sata(data)                                                ((0x000000FF&(data))>>0)


#define SB1_PRIORITY_2                                                               0x1801C004
#define SB1_PRIORITY_2_reg_addr                                                      "0xB801C004"
#define SB1_PRIORITY_2_reg                                                           0xB801C004
#define set_SB1_PRIORITY_2_reg(data)   (*((volatile unsigned int*) SB1_PRIORITY_2_reg)=data)
#define get_SB1_PRIORITY_2_reg   (*((volatile unsigned int*) SB1_PRIORITY_2_reg))
#define SB1_PRIORITY_2_inst_adr                                                      "0x0001"
#define SB1_PRIORITY_2_inst                                                          0x0001
#define SB1_PRIORITY_2_nf_shift                                                      (24)
#define SB1_PRIORITY_2_nf_mask                                                       (0xFF000000)
#define SB1_PRIORITY_2_nf(data)                                                      (0xFF000000&((data)<<24))
#define SB1_PRIORITY_2_nf_src(data)                                                  ((0xFF000000&(data))>>24)
#define SB1_PRIORITY_2_get_nf(data)                                                  ((0xFF000000&(data))>>24)
#define SB1_PRIORITY_2_usb_shift                                                     (16)
#define SB1_PRIORITY_2_usb_mask                                                      (0x00FF0000)
#define SB1_PRIORITY_2_usb(data)                                                     (0x00FF0000&((data)<<16))
#define SB1_PRIORITY_2_usb_src(data)                                                 ((0x00FF0000&(data))>>16)
#define SB1_PRIORITY_2_get_usb(data)                                                 ((0x00FF0000&(data))>>16)
#define SB1_PRIORITY_2_cp_shift                                                      (8)
#define SB1_PRIORITY_2_cp_mask                                                       (0x0000FF00)
#define SB1_PRIORITY_2_cp(data)                                                      (0x0000FF00&((data)<<8))
#define SB1_PRIORITY_2_cp_src(data)                                                  ((0x0000FF00&(data))>>8)
#define SB1_PRIORITY_2_get_cp(data)                                                  ((0x0000FF00&(data))>>8)
#define SB1_PRIORITY_2_cr_shift                                                      (0)
#define SB1_PRIORITY_2_cr_mask                                                       (0x000000FF)
#define SB1_PRIORITY_2_cr(data)                                                      (0x000000FF&((data)<<0))
#define SB1_PRIORITY_2_cr_src(data)                                                  ((0x000000FF&(data))>>0)
#define SB1_PRIORITY_2_get_cr(data)                                                  ((0x000000FF&(data))>>0)


#define SB1_CHANNEL_HIGH_PRIORITY                                                    0x1801C008
#define SB1_CHANNEL_HIGH_PRIORITY_reg_addr                                           "0xB801C008"
#define SB1_CHANNEL_HIGH_PRIORITY_reg                                                0xB801C008
#define set_SB1_CHANNEL_HIGH_PRIORITY_reg(data)   (*((volatile unsigned int*) SB1_CHANNEL_HIGH_PRIORITY_reg)=data)
#define get_SB1_CHANNEL_HIGH_PRIORITY_reg   (*((volatile unsigned int*) SB1_CHANNEL_HIGH_PRIORITY_reg))
#define SB1_CHANNEL_HIGH_PRIORITY_inst_adr                                           "0x0002"
#define SB1_CHANNEL_HIGH_PRIORITY_inst                                               0x0002
#define SB1_CHANNEL_HIGH_PRIORITY_index_shift                                        (0)
#define SB1_CHANNEL_HIGH_PRIORITY_index_mask                                         (0x000000FF)
#define SB1_CHANNEL_HIGH_PRIORITY_index(data)                                        (0x000000FF&((data)<<0))
#define SB1_CHANNEL_HIGH_PRIORITY_index_src(data)                                    ((0x000000FF&(data))>>0)
#define SB1_CHANNEL_HIGH_PRIORITY_get_index(data)                                    ((0x000000FF&(data))>>0)


#define SB1_CHANNEL_REQ_MASK                                                         0x1801C00C
#define SB1_CHANNEL_REQ_MASK_reg_addr                                                "0xB801C00C"
#define SB1_CHANNEL_REQ_MASK_reg                                                     0xB801C00C
#define set_SB1_CHANNEL_REQ_MASK_reg(data)   (*((volatile unsigned int*) SB1_CHANNEL_REQ_MASK_reg)=data)
#define get_SB1_CHANNEL_REQ_MASK_reg   (*((volatile unsigned int*) SB1_CHANNEL_REQ_MASK_reg))
#define SB1_CHANNEL_REQ_MASK_inst_adr                                                "0x0003"
#define SB1_CHANNEL_REQ_MASK_inst                                                    0x0003
#define SB1_CHANNEL_REQ_MASK_index_shift                                             (0)
#define SB1_CHANNEL_REQ_MASK_index_mask                                              (0x000000FF)
#define SB1_CHANNEL_REQ_MASK_index(data)                                             (0x000000FF&((data)<<0))
#define SB1_CHANNEL_REQ_MASK_index_src(data)                                         ((0x000000FF&(data))>>0)
#define SB1_CHANNEL_REQ_MASK_get_index(data)                                         ((0x000000FF&(data))>>0)


#define SB1_CHANNEL_REQ_BUSY                                                         0x1801C010
#define SB1_CHANNEL_REQ_BUSY_reg_addr                                                "0xB801C010"
#define SB1_CHANNEL_REQ_BUSY_reg                                                     0xB801C010
#define set_SB1_CHANNEL_REQ_BUSY_reg(data)   (*((volatile unsigned int*) SB1_CHANNEL_REQ_BUSY_reg)=data)
#define get_SB1_CHANNEL_REQ_BUSY_reg   (*((volatile unsigned int*) SB1_CHANNEL_REQ_BUSY_reg))
#define SB1_CHANNEL_REQ_BUSY_inst_adr                                                "0x0004"
#define SB1_CHANNEL_REQ_BUSY_inst                                                    0x0004
#define SB1_CHANNEL_REQ_BUSY_index_shift                                             (0)
#define SB1_CHANNEL_REQ_BUSY_index_mask                                              (0x000000FF)
#define SB1_CHANNEL_REQ_BUSY_index(data)                                             (0x000000FF&((data)<<0))
#define SB1_CHANNEL_REQ_BUSY_index_src(data)                                         ((0x000000FF&(data))>>0)
#define SB1_CHANNEL_REQ_BUSY_get_index(data)                                         ((0x000000FF&(data))>>0)


#define SB1_MISC_CTRL                                                                0x1801C014
#define SB1_MISC_CTRL_reg_addr                                                       "0xB801C014"
#define SB1_MISC_CTRL_reg                                                            0xB801C014
#define set_SB1_MISC_CTRL_reg(data)   (*((volatile unsigned int*) SB1_MISC_CTRL_reg)=data)
#define get_SB1_MISC_CTRL_reg   (*((volatile unsigned int*) SB1_MISC_CTRL_reg))
#define SB1_MISC_CTRL_inst_adr                                                       "0x0005"
#define SB1_MISC_CTRL_inst                                                           0x0005
#define SB1_MISC_CTRL_allow_cmd_cnt_shift                                            (0)
#define SB1_MISC_CTRL_allow_cmd_cnt_mask                                             (0x00000007)
#define SB1_MISC_CTRL_allow_cmd_cnt(data)                                            (0x00000007&((data)<<0))
#define SB1_MISC_CTRL_allow_cmd_cnt_src(data)                                        ((0x00000007&(data))>>0)
#define SB1_MISC_CTRL_get_allow_cmd_cnt(data)                                        ((0x00000007&(data))>>0)


#define SB3_PRIORITY_1                                                               0x1801C200
#define SB3_PRIORITY_1_reg_addr                                                      "0xB801C200"
#define SB3_PRIORITY_1_reg                                                           0xB801C200
#define set_SB3_PRIORITY_1_reg(data)   (*((volatile unsigned int*) SB3_PRIORITY_1_reg)=data)
#define get_SB3_PRIORITY_1_reg   (*((volatile unsigned int*) SB3_PRIORITY_1_reg))
#define SB3_PRIORITY_1_inst_adr                                                      "0x0080"
#define SB3_PRIORITY_1_inst                                                          0x0080
#define SB3_PRIORITY_1_md_shift                                                      (24)
#define SB3_PRIORITY_1_md_mask                                                       (0xFF000000)
#define SB3_PRIORITY_1_md(data)                                                      (0xFF000000&((data)<<24))
#define SB3_PRIORITY_1_md_src(data)                                                  ((0xFF000000&(data))>>24)
#define SB3_PRIORITY_1_get_md(data)                                                  ((0xFF000000&(data))>>24)
#define SB3_PRIORITY_1_vde_shift                                                     (16)
#define SB3_PRIORITY_1_vde_mask                                                      (0x00FF0000)
#define SB3_PRIORITY_1_vde(data)                                                     (0x00FF0000&((data)<<16))
#define SB3_PRIORITY_1_vde_src(data)                                                 ((0x00FF0000&(data))>>16)
#define SB3_PRIORITY_1_get_vde(data)                                                 ((0x00FF0000&(data))>>16)
#define SB3_PRIORITY_1_ade_shift                                                     (8)
#define SB3_PRIORITY_1_ade_mask                                                      (0x0000FF00)
#define SB3_PRIORITY_1_ade(data)                                                     (0x0000FF00&((data)<<8))
#define SB3_PRIORITY_1_ade_src(data)                                                 ((0x0000FF00&(data))>>8)
#define SB3_PRIORITY_1_get_ade(data)                                                 ((0x0000FF00&(data))>>8)
#define SB3_PRIORITY_1_aee_shift                                                     (0)
#define SB3_PRIORITY_1_aee_mask                                                      (0x000000FF)
#define SB3_PRIORITY_1_aee(data)                                                     (0x000000FF&((data)<<0))
#define SB3_PRIORITY_1_aee_src(data)                                                 ((0x000000FF&(data))>>0)
#define SB3_PRIORITY_1_get_aee(data)                                                 ((0x000000FF&(data))>>0)


#define SB3_PRIORITY_2                                                               0x1801C214
#define SB3_PRIORITY_2_reg_addr                                                      "0xB801C214"
#define SB3_PRIORITY_2_reg                                                           0xB801C214
#define set_SB3_PRIORITY_2_reg(data)   (*((volatile unsigned int*) SB3_PRIORITY_2_reg)=data)
#define get_SB3_PRIORITY_2_reg   (*((volatile unsigned int*) SB3_PRIORITY_2_reg))
#define SB3_PRIORITY_2_inst_adr                                                      "0x0085"
#define SB3_PRIORITY_2_inst                                                          0x0085
#define SB3_PRIORITY_2_ve_shift                                                      (0)
#define SB3_PRIORITY_2_ve_mask                                                       (0x000000FF)
#define SB3_PRIORITY_2_ve(data)                                                      (0x000000FF&((data)<<0))
#define SB3_PRIORITY_2_ve_src(data)                                                  ((0x000000FF&(data))>>0)
#define SB3_PRIORITY_2_get_ve(data)                                                  ((0x000000FF&(data))>>0)


#define SB3_CHANNEL_HIGH_PRIORITY                                                    0x1801C204
#define SB3_CHANNEL_HIGH_PRIORITY_reg_addr                                           "0xB801C204"
#define SB3_CHANNEL_HIGH_PRIORITY_reg                                                0xB801C204
#define set_SB3_CHANNEL_HIGH_PRIORITY_reg(data)   (*((volatile unsigned int*) SB3_CHANNEL_HIGH_PRIORITY_reg)=data)
#define get_SB3_CHANNEL_HIGH_PRIORITY_reg   (*((volatile unsigned int*) SB3_CHANNEL_HIGH_PRIORITY_reg))
#define SB3_CHANNEL_HIGH_PRIORITY_inst_adr                                           "0x0081"
#define SB3_CHANNEL_HIGH_PRIORITY_inst                                               0x0081
#define SB3_CHANNEL_HIGH_PRIORITY_index_shift                                        (0)
#define SB3_CHANNEL_HIGH_PRIORITY_index_mask                                         (0x000000FF)
#define SB3_CHANNEL_HIGH_PRIORITY_index(data)                                        (0x000000FF&((data)<<0))
#define SB3_CHANNEL_HIGH_PRIORITY_index_src(data)                                    ((0x000000FF&(data))>>0)
#define SB3_CHANNEL_HIGH_PRIORITY_get_index(data)                                    ((0x000000FF&(data))>>0)


#define SB3_CHANNEL_REQ_MASK                                                         0x1801C208
#define SB3_CHANNEL_REQ_MASK_reg_addr                                                "0xB801C208"
#define SB3_CHANNEL_REQ_MASK_reg                                                     0xB801C208
#define set_SB3_CHANNEL_REQ_MASK_reg(data)   (*((volatile unsigned int*) SB3_CHANNEL_REQ_MASK_reg)=data)
#define get_SB3_CHANNEL_REQ_MASK_reg   (*((volatile unsigned int*) SB3_CHANNEL_REQ_MASK_reg))
#define SB3_CHANNEL_REQ_MASK_inst_adr                                                "0x0082"
#define SB3_CHANNEL_REQ_MASK_inst                                                    0x0082
#define SB3_CHANNEL_REQ_MASK_index_shift                                             (0)
#define SB3_CHANNEL_REQ_MASK_index_mask                                              (0x000000FF)
#define SB3_CHANNEL_REQ_MASK_index(data)                                             (0x000000FF&((data)<<0))
#define SB3_CHANNEL_REQ_MASK_index_src(data)                                         ((0x000000FF&(data))>>0)
#define SB3_CHANNEL_REQ_MASK_get_index(data)                                         ((0x000000FF&(data))>>0)


#define SB3_CHANNEL_REQ_BUSY                                                         0x1801C20C
#define SB3_CHANNEL_REQ_BUSY_reg_addr                                                "0xB801C20C"
#define SB3_CHANNEL_REQ_BUSY_reg                                                     0xB801C20C
#define set_SB3_CHANNEL_REQ_BUSY_reg(data)   (*((volatile unsigned int*) SB3_CHANNEL_REQ_BUSY_reg)=data)
#define get_SB3_CHANNEL_REQ_BUSY_reg   (*((volatile unsigned int*) SB3_CHANNEL_REQ_BUSY_reg))
#define SB3_CHANNEL_REQ_BUSY_inst_adr                                                "0x0083"
#define SB3_CHANNEL_REQ_BUSY_inst                                                    0x0083
#define SB3_CHANNEL_REQ_BUSY_index_shift                                             (0)
#define SB3_CHANNEL_REQ_BUSY_index_mask                                              (0x000000FF)
#define SB3_CHANNEL_REQ_BUSY_index(data)                                             (0x000000FF&((data)<<0))
#define SB3_CHANNEL_REQ_BUSY_index_src(data)                                         ((0x000000FF&(data))>>0)
#define SB3_CHANNEL_REQ_BUSY_get_index(data)                                         ((0x000000FF&(data))>>0)


#define SB3_MISC_CTRL                                                                0x1801C210
#define SB3_MISC_CTRL_reg_addr                                                       "0xB801C210"
#define SB3_MISC_CTRL_reg                                                            0xB801C210
#define set_SB3_MISC_CTRL_reg(data)   (*((volatile unsigned int*) SB3_MISC_CTRL_reg)=data)
#define get_SB3_MISC_CTRL_reg   (*((volatile unsigned int*) SB3_MISC_CTRL_reg))
#define SB3_MISC_CTRL_inst_adr                                                       "0x0084"
#define SB3_MISC_CTRL_inst                                                           0x0084
#define SB3_MISC_CTRL_allow_cmd_cnt_shift                                            (0)
#define SB3_MISC_CTRL_allow_cmd_cnt_mask                                             (0x00000007)
#define SB3_MISC_CTRL_allow_cmd_cnt(data)                                            (0x00000007&((data)<<0))
#define SB3_MISC_CTRL_allow_cmd_cnt_src(data)                                        ((0x00000007&(data))>>0)
#define SB3_MISC_CTRL_get_allow_cmd_cnt(data)                                        ((0x00000007&(data))>>0)


#endif
