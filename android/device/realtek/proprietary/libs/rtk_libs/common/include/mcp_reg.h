/**************************************************************
// Spec Version                  : 0.4
// Parser Version                : DVR_Parser_6.0
// CModelGen Version             : 5.0 2009.04.09
// Naming Rule                   : Module_Register_Name_reg
// Parse Option                  : Only Parse _op1
// Parse Address Region          : All Address Region 
// Firmware Header Generate Date : 2010/5/11 9:13:4
***************************************************************/


#ifndef _MCP_REG_H_INCLUDED_
#define _MCP_REG_H_INCLUDED_
#ifdef  _MCP_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     ofb_2:1;
unsigned int     ofb_1:1;
unsigned int     arb_mode:1;
unsigned int     reserved_1:1;
unsigned int     clear:1;
unsigned int     swap:1;
unsigned int     idle:1;
unsigned int     go:1;
unsigned int     write_data:1;
}MCP_CTRL;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     k_mcp_error:1;
unsigned int     k_error:1;
unsigned int     k_compare:1;
unsigned int     k_ring_empty:1;
unsigned int     mcp_error:1;
unsigned int     reserved_1:2;
unsigned int     compare:1;
unsigned int     error:1;
unsigned int     ring_empty:1;
unsigned int     write_data:1;
}MCP_STATUS;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     mcp_error_en:1;
unsigned int     reserved_1:2;
unsigned int     compare_en:1;
unsigned int     error_en:1;
unsigned int     ring_empty_en:1;
unsigned int     write_data:1;
}MCP_EN;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     aesh_pad_off:1;
unsigned int     csa_entropy:2;
unsigned int     mcp_round_no:8;
}MCP_CTRL1;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     base:31;
}MCP_BASE;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     limit:31;
}MCP_LIMIT;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     rdptr:31;
}MCP_RDPTR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     wrptr:31;
}MCP_WRPTR;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     count:16;
}MCP_DES_COUNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     compare:16;
}MCP_DES_COMPARE;

typedef struct 
{
unsigned int     data:32;
}MCP_DES_INI_KEY;

typedef struct 
{
unsigned int     data:32;
}MCP_AES_INI_KEY;

typedef struct 
{
unsigned int     data:32;
}MCP_MULTI2_SYS_KEY;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     otp_load:1;
}CP_OTP_LOAD;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     ofb_2:1;
unsigned int     ofb_1:1;
unsigned int     reserved_1:1;
unsigned int     reserved_2:1;
unsigned int     clear:1;
unsigned int     swap:1;
unsigned int     idle:1;
unsigned int     go:1;
unsigned int     write_data:1;
}K_MCP_CTRL;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     s_mcp_error:1;
unsigned int     s_error:1;
unsigned int     s_compare:1;
unsigned int     s_ring_empty:1;
unsigned int     mcp_error:1;
unsigned int     reserved_1:2;
unsigned int     compare:1;
unsigned int     error:1;
unsigned int     ring_empty:1;
unsigned int     write_data:1;
}K_MCP_STATUS;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     mcp_error_en:1;
unsigned int     reserved_1:2;
unsigned int     compare_en:1;
unsigned int     error_en:1;
unsigned int     ring_empty_en:1;
unsigned int     write_data:1;
}K_MCP_EN;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     aesh_pad_off:1;
unsigned int     csa_entropy:2;
unsigned int     mcp_round_no:8;
}K_MCP_CTRL1;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     base:31;
}K_MCP_BASE;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     limit:31;
}K_MCP_LIMIT;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     rdptr:31;
}K_MCP_RDPTR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     wrptr:31;
}K_MCP_WRPTR;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     count:16;
}K_MCP_DES_COUNT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     compare:16;
}K_MCP_DES_COMPARE;

typedef struct 
{
unsigned int     data:32;
}K_MCP_DES_INI_KEY;

typedef struct 
{
unsigned int     data:32;
}K_MCP_AES_INI_KEY;

typedef struct 
{
unsigned int     data:32;
}K_MCP_MULTI2_SYS_KEY;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     rd_sm:4;
unsigned int     wr_sm:3;
unsigned int     mcp_top_sm:10;
unsigned int     mcp_aes_like_sm:11;
}MCP_ST;

#endif

#define MCP_CTRL                                                                     0x18015100
#define MCP_CTRL_reg_addr                                                            "0xB8015100"
#define MCP_CTRL_reg                                                                 0xB8015100
#define set_MCP_CTRL_reg(data)   (*((volatile unsigned int*) MCP_CTRL_reg)=data)
#define get_MCP_CTRL_reg   (*((volatile unsigned int*) MCP_CTRL_reg))
#define MCP_CTRL_inst_adr                                                            "0x0040"
#define MCP_CTRL_inst                                                                0x0040
#define MCP_CTRL_ofb_2_shift                                                         (8)
#define MCP_CTRL_ofb_2_mask                                                          (0x00000100)
#define MCP_CTRL_ofb_2(data)                                                         (0x00000100&((data)<<8))
#define MCP_CTRL_ofb_2_src(data)                                                     ((0x00000100&(data))>>8)
#define MCP_CTRL_get_ofb_2(data)                                                     ((0x00000100&(data))>>8)
#define MCP_CTRL_ofb_1_shift                                                         (7)
#define MCP_CTRL_ofb_1_mask                                                          (0x00000080)
#define MCP_CTRL_ofb_1(data)                                                         (0x00000080&((data)<<7))
#define MCP_CTRL_ofb_1_src(data)                                                     ((0x00000080&(data))>>7)
#define MCP_CTRL_get_ofb_1(data)                                                     ((0x00000080&(data))>>7)
#define MCP_CTRL_arb_mode_shift                                                      (6)
#define MCP_CTRL_arb_mode_mask                                                       (0x00000040)
#define MCP_CTRL_arb_mode(data)                                                      (0x00000040&((data)<<6))
#define MCP_CTRL_arb_mode_src(data)                                                  ((0x00000040&(data))>>6)
#define MCP_CTRL_get_arb_mode(data)                                                  ((0x00000040&(data))>>6)
#define MCP_CTRL_clear_shift                                                         (4)
#define MCP_CTRL_clear_mask                                                          (0x00000010)
#define MCP_CTRL_clear(data)                                                         (0x00000010&((data)<<4))
#define MCP_CTRL_clear_src(data)                                                     ((0x00000010&(data))>>4)
#define MCP_CTRL_get_clear(data)                                                     ((0x00000010&(data))>>4)
#define MCP_CTRL_swap_shift                                                          (3)
#define MCP_CTRL_swap_mask                                                           (0x00000008)
#define MCP_CTRL_swap(data)                                                          (0x00000008&((data)<<3))
#define MCP_CTRL_swap_src(data)                                                      ((0x00000008&(data))>>3)
#define MCP_CTRL_get_swap(data)                                                      ((0x00000008&(data))>>3)
#define MCP_CTRL_idle_shift                                                          (2)
#define MCP_CTRL_idle_mask                                                           (0x00000004)
#define MCP_CTRL_idle(data)                                                          (0x00000004&((data)<<2))
#define MCP_CTRL_idle_src(data)                                                      ((0x00000004&(data))>>2)
#define MCP_CTRL_get_idle(data)                                                      ((0x00000004&(data))>>2)
#define MCP_CTRL_go_shift                                                            (1)
#define MCP_CTRL_go_mask                                                             (0x00000002)
#define MCP_CTRL_go(data)                                                            (0x00000002&((data)<<1))
#define MCP_CTRL_go_src(data)                                                        ((0x00000002&(data))>>1)
#define MCP_CTRL_get_go(data)                                                        ((0x00000002&(data))>>1)
#define MCP_CTRL_write_data_shift                                                    (0)
#define MCP_CTRL_write_data_mask                                                     (0x00000001)
#define MCP_CTRL_write_data(data)                                                    (0x00000001&((data)<<0))
#define MCP_CTRL_write_data_src(data)                                                ((0x00000001&(data))>>0)
#define MCP_CTRL_get_write_data(data)                                                ((0x00000001&(data))>>0)


#define MCP_STATUS                                                                   0x18015104
#define MCP_STATUS_reg_addr                                                          "0xB8015104"
#define MCP_STATUS_reg                                                               0xB8015104
#define set_MCP_STATUS_reg(data)   (*((volatile unsigned int*) MCP_STATUS_reg)=data)
#define get_MCP_STATUS_reg   (*((volatile unsigned int*) MCP_STATUS_reg))
#define MCP_STATUS_inst_adr                                                          "0x0041"
#define MCP_STATUS_inst                                                              0x0041
#define MCP_STATUS_k_mcp_error_shift                                                 (10)
#define MCP_STATUS_k_mcp_error_mask                                                  (0x00000400)
#define MCP_STATUS_k_mcp_error(data)                                                 (0x00000400&((data)<<10))
#define MCP_STATUS_k_mcp_error_src(data)                                             ((0x00000400&(data))>>10)
#define MCP_STATUS_get_k_mcp_error(data)                                             ((0x00000400&(data))>>10)
#define MCP_STATUS_k_error_shift                                                     (9)
#define MCP_STATUS_k_error_mask                                                      (0x00000200)
#define MCP_STATUS_k_error(data)                                                     (0x00000200&((data)<<9))
#define MCP_STATUS_k_error_src(data)                                                 ((0x00000200&(data))>>9)
#define MCP_STATUS_get_k_error(data)                                                 ((0x00000200&(data))>>9)
#define MCP_STATUS_k_compare_shift                                                   (8)
#define MCP_STATUS_k_compare_mask                                                    (0x00000100)
#define MCP_STATUS_k_compare(data)                                                   (0x00000100&((data)<<8))
#define MCP_STATUS_k_compare_src(data)                                               ((0x00000100&(data))>>8)
#define MCP_STATUS_get_k_compare(data)                                               ((0x00000100&(data))>>8)
#define MCP_STATUS_k_ring_empty_shift                                                (7)
#define MCP_STATUS_k_ring_empty_mask                                                 (0x00000080)
#define MCP_STATUS_k_ring_empty(data)                                                (0x00000080&((data)<<7))
#define MCP_STATUS_k_ring_empty_src(data)                                            ((0x00000080&(data))>>7)
#define MCP_STATUS_get_k_ring_empty(data)                                            ((0x00000080&(data))>>7)
#define MCP_STATUS_mcp_error_shift                                                   (6)
#define MCP_STATUS_mcp_error_mask                                                    (0x00000040)
#define MCP_STATUS_mcp_error(data)                                                   (0x00000040&((data)<<6))
#define MCP_STATUS_mcp_error_src(data)                                               ((0x00000040&(data))>>6)
#define MCP_STATUS_get_mcp_error(data)                                               ((0x00000040&(data))>>6)
#define MCP_STATUS_compare_shift                                                     (3)
#define MCP_STATUS_compare_mask                                                      (0x00000008)
#define MCP_STATUS_compare(data)                                                     (0x00000008&((data)<<3))
#define MCP_STATUS_compare_src(data)                                                 ((0x00000008&(data))>>3)
#define MCP_STATUS_get_compare(data)                                                 ((0x00000008&(data))>>3)
#define MCP_STATUS_error_shift                                                       (2)
#define MCP_STATUS_error_mask                                                        (0x00000004)
#define MCP_STATUS_error(data)                                                       (0x00000004&((data)<<2))
#define MCP_STATUS_error_src(data)                                                   ((0x00000004&(data))>>2)
#define MCP_STATUS_get_error(data)                                                   ((0x00000004&(data))>>2)
#define MCP_STATUS_ring_empty_shift                                                  (1)
#define MCP_STATUS_ring_empty_mask                                                   (0x00000002)
#define MCP_STATUS_ring_empty(data)                                                  (0x00000002&((data)<<1))
#define MCP_STATUS_ring_empty_src(data)                                              ((0x00000002&(data))>>1)
#define MCP_STATUS_get_ring_empty(data)                                              ((0x00000002&(data))>>1)
#define MCP_STATUS_write_data_shift                                                  (0)
#define MCP_STATUS_write_data_mask                                                   (0x00000001)
#define MCP_STATUS_write_data(data)                                                  (0x00000001&((data)<<0))
#define MCP_STATUS_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define MCP_STATUS_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define MCP_EN                                                                       0x18015108
#define MCP_EN_reg_addr                                                              "0xB8015108"
#define MCP_EN_reg                                                                   0xB8015108
#define set_MCP_EN_reg(data)   (*((volatile unsigned int*) MCP_EN_reg)=data)
#define get_MCP_EN_reg   (*((volatile unsigned int*) MCP_EN_reg))
#define MCP_EN_inst_adr                                                              "0x0042"
#define MCP_EN_inst                                                                  0x0042
#define MCP_EN_mcp_error_en_shift                                                    (6)
#define MCP_EN_mcp_error_en_mask                                                     (0x00000040)
#define MCP_EN_mcp_error_en(data)                                                    (0x00000040&((data)<<6))
#define MCP_EN_mcp_error_en_src(data)                                                ((0x00000040&(data))>>6)
#define MCP_EN_get_mcp_error_en(data)                                                ((0x00000040&(data))>>6)
#define MCP_EN_compare_en_shift                                                      (3)
#define MCP_EN_compare_en_mask                                                       (0x00000008)
#define MCP_EN_compare_en(data)                                                      (0x00000008&((data)<<3))
#define MCP_EN_compare_en_src(data)                                                  ((0x00000008&(data))>>3)
#define MCP_EN_get_compare_en(data)                                                  ((0x00000008&(data))>>3)
#define MCP_EN_error_en_shift                                                        (2)
#define MCP_EN_error_en_mask                                                         (0x00000004)
#define MCP_EN_error_en(data)                                                        (0x00000004&((data)<<2))
#define MCP_EN_error_en_src(data)                                                    ((0x00000004&(data))>>2)
#define MCP_EN_get_error_en(data)                                                    ((0x00000004&(data))>>2)
#define MCP_EN_ring_empty_en_shift                                                   (1)
#define MCP_EN_ring_empty_en_mask                                                    (0x00000002)
#define MCP_EN_ring_empty_en(data)                                                   (0x00000002&((data)<<1))
#define MCP_EN_ring_empty_en_src(data)                                               ((0x00000002&(data))>>1)
#define MCP_EN_get_ring_empty_en(data)                                               ((0x00000002&(data))>>1)
#define MCP_EN_write_data_shift                                                      (0)
#define MCP_EN_write_data_mask                                                       (0x00000001)
#define MCP_EN_write_data(data)                                                      (0x00000001&((data)<<0))
#define MCP_EN_write_data_src(data)                                                  ((0x00000001&(data))>>0)
#define MCP_EN_get_write_data(data)                                                  ((0x00000001&(data))>>0)


#define MCP_CTRL1                                                                    0x18015198
#define MCP_CTRL1_reg_addr                                                           "0xB8015198"
#define MCP_CTRL1_reg                                                                0xB8015198
#define set_MCP_CTRL1_reg(data)   (*((volatile unsigned int*) MCP_CTRL1_reg)=data)
#define get_MCP_CTRL1_reg   (*((volatile unsigned int*) MCP_CTRL1_reg))
#define MCP_CTRL1_inst_adr                                                           "0x0066"
#define MCP_CTRL1_inst                                                               0x0066
#define MCP_CTRL1_aesh_pad_off_shift                                                 (10)
#define MCP_CTRL1_aesh_pad_off_mask                                                  (0x00000400)
#define MCP_CTRL1_aesh_pad_off(data)                                                 (0x00000400&((data)<<10))
#define MCP_CTRL1_aesh_pad_off_src(data)                                             ((0x00000400&(data))>>10)
#define MCP_CTRL1_get_aesh_pad_off(data)                                             ((0x00000400&(data))>>10)
#define MCP_CTRL1_csa_entropy_shift                                                  (8)
#define MCP_CTRL1_csa_entropy_mask                                                   (0x00000300)
#define MCP_CTRL1_csa_entropy(data)                                                  (0x00000300&((data)<<8))
#define MCP_CTRL1_csa_entropy_src(data)                                              ((0x00000300&(data))>>8)
#define MCP_CTRL1_get_csa_entropy(data)                                              ((0x00000300&(data))>>8)
#define MCP_CTRL1_mcp_round_no_shift                                                 (0)
#define MCP_CTRL1_mcp_round_no_mask                                                  (0x000000FF)
#define MCP_CTRL1_mcp_round_no(data)                                                 (0x000000FF&((data)<<0))
#define MCP_CTRL1_mcp_round_no_src(data)                                             ((0x000000FF&(data))>>0)
#define MCP_CTRL1_get_mcp_round_no(data)                                             ((0x000000FF&(data))>>0)


#define MCP_BASE                                                                     0x1801510C
#define MCP_BASE_reg_addr                                                            "0xB801510C"
#define MCP_BASE_reg                                                                 0xB801510C
#define set_MCP_BASE_reg(data)   (*((volatile unsigned int*) MCP_BASE_reg)=data)
#define get_MCP_BASE_reg   (*((volatile unsigned int*) MCP_BASE_reg))
#define MCP_BASE_inst_adr                                                            "0x0043"
#define MCP_BASE_inst                                                                0x0043
#define MCP_BASE_base_shift                                                          (0)
#define MCP_BASE_base_mask                                                           (0x7FFFFFFF)
#define MCP_BASE_base(data)                                                          (0x7FFFFFFF&((data)<<0))
#define MCP_BASE_base_src(data)                                                      ((0x7FFFFFFF&(data))>>0)
#define MCP_BASE_get_base(data)                                                      ((0x7FFFFFFF&(data))>>0)


#define MCP_LIMIT                                                                    0x18015110
#define MCP_LIMIT_reg_addr                                                           "0xB8015110"
#define MCP_LIMIT_reg                                                                0xB8015110
#define set_MCP_LIMIT_reg(data)   (*((volatile unsigned int*) MCP_LIMIT_reg)=data)
#define get_MCP_LIMIT_reg   (*((volatile unsigned int*) MCP_LIMIT_reg))
#define MCP_LIMIT_inst_adr                                                           "0x0044"
#define MCP_LIMIT_inst                                                               0x0044
#define MCP_LIMIT_limit_shift                                                        (0)
#define MCP_LIMIT_limit_mask                                                         (0x7FFFFFFF)
#define MCP_LIMIT_limit(data)                                                        (0x7FFFFFFF&((data)<<0))
#define MCP_LIMIT_limit_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define MCP_LIMIT_get_limit(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define MCP_RDPTR                                                                    0x18015114
#define MCP_RDPTR_reg_addr                                                           "0xB8015114"
#define MCP_RDPTR_reg                                                                0xB8015114
#define set_MCP_RDPTR_reg(data)   (*((volatile unsigned int*) MCP_RDPTR_reg)=data)
#define get_MCP_RDPTR_reg   (*((volatile unsigned int*) MCP_RDPTR_reg))
#define MCP_RDPTR_inst_adr                                                           "0x0045"
#define MCP_RDPTR_inst                                                               0x0045
#define MCP_RDPTR_rdptr_shift                                                        (0)
#define MCP_RDPTR_rdptr_mask                                                         (0x7FFFFFFF)
#define MCP_RDPTR_rdptr(data)                                                        (0x7FFFFFFF&((data)<<0))
#define MCP_RDPTR_rdptr_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define MCP_RDPTR_get_rdptr(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define MCP_WRPTR                                                                    0x18015118
#define MCP_WRPTR_reg_addr                                                           "0xB8015118"
#define MCP_WRPTR_reg                                                                0xB8015118
#define set_MCP_WRPTR_reg(data)   (*((volatile unsigned int*) MCP_WRPTR_reg)=data)
#define get_MCP_WRPTR_reg   (*((volatile unsigned int*) MCP_WRPTR_reg))
#define MCP_WRPTR_inst_adr                                                           "0x0046"
#define MCP_WRPTR_inst                                                               0x0046
#define MCP_WRPTR_wrptr_shift                                                        (0)
#define MCP_WRPTR_wrptr_mask                                                         (0x7FFFFFFF)
#define MCP_WRPTR_wrptr(data)                                                        (0x7FFFFFFF&((data)<<0))
#define MCP_WRPTR_wrptr_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define MCP_WRPTR_get_wrptr(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define MCP_DES_COUNT                                                                0x18015134
#define MCP_DES_COUNT_reg_addr                                                       "0xB8015134"
#define MCP_DES_COUNT_reg                                                            0xB8015134
#define set_MCP_DES_COUNT_reg(data)   (*((volatile unsigned int*) MCP_DES_COUNT_reg)=data)
#define get_MCP_DES_COUNT_reg   (*((volatile unsigned int*) MCP_DES_COUNT_reg))
#define MCP_DES_COUNT_inst_adr                                                       "0x004D"
#define MCP_DES_COUNT_inst                                                           0x004D
#define MCP_DES_COUNT_count_shift                                                    (0)
#define MCP_DES_COUNT_count_mask                                                     (0x0000FFFF)
#define MCP_DES_COUNT_count(data)                                                    (0x0000FFFF&((data)<<0))
#define MCP_DES_COUNT_count_src(data)                                                ((0x0000FFFF&(data))>>0)
#define MCP_DES_COUNT_get_count(data)                                                ((0x0000FFFF&(data))>>0)


#define MCP_DES_COMPARE                                                              0x18015138
#define MCP_DES_COMPARE_reg_addr                                                     "0xB8015138"
#define MCP_DES_COMPARE_reg                                                          0xB8015138
#define set_MCP_DES_COMPARE_reg(data)   (*((volatile unsigned int*) MCP_DES_COMPARE_reg)=data)
#define get_MCP_DES_COMPARE_reg   (*((volatile unsigned int*) MCP_DES_COMPARE_reg))
#define MCP_DES_COMPARE_inst_adr                                                     "0x004E"
#define MCP_DES_COMPARE_inst                                                         0x004E
#define MCP_DES_COMPARE_compare_shift                                                (0)
#define MCP_DES_COMPARE_compare_mask                                                 (0x0000FFFF)
#define MCP_DES_COMPARE_compare(data)                                                (0x0000FFFF&((data)<<0))
#define MCP_DES_COMPARE_compare_src(data)                                            ((0x0000FFFF&(data))>>0)
#define MCP_DES_COMPARE_get_compare(data)                                            ((0x0000FFFF&(data))>>0)


#define MCP_DES_INI_KEY_0                                                            0x1801511C
#define MCP_DES_INI_KEY_1                                                            0x18015120
#define MCP_DES_INI_KEY_0_reg_addr                                                   "0xB801511C"
#define MCP_DES_INI_KEY_1_reg_addr                                                   "0xB8015120"
#define MCP_DES_INI_KEY_0_reg                                                        0xB801511C
#define MCP_DES_INI_KEY_1_reg                                                        0xB8015120
#define set_MCP_DES_INI_KEY_0_reg(data)   (*((volatile unsigned int*) MCP_DES_INI_KEY_0_reg)=data)
#define set_MCP_DES_INI_KEY_1_reg(data)   (*((volatile unsigned int*) MCP_DES_INI_KEY_1_reg)=data)
#define get_MCP_DES_INI_KEY_0_reg   (*((volatile unsigned int*) MCP_DES_INI_KEY_0_reg))
#define get_MCP_DES_INI_KEY_1_reg   (*((volatile unsigned int*) MCP_DES_INI_KEY_1_reg))
#define MCP_DES_INI_KEY_0_inst_adr                                                   "0x0047"
#define MCP_DES_INI_KEY_1_inst_adr                                                   "0x0048"
#define MCP_DES_INI_KEY_0_inst                                                       0x0047
#define MCP_DES_INI_KEY_1_inst                                                       0x0048
#define MCP_DES_INI_KEY_data_shift                                                   (0)
#define MCP_DES_INI_KEY_data_mask                                                    (0xFFFFFFFF)
#define MCP_DES_INI_KEY_data(data)                                                   (0xFFFFFFFF&((data)<<0))
#define MCP_DES_INI_KEY_data_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define MCP_DES_INI_KEY_get_data(data)                                               ((0xFFFFFFFF&(data))>>0)


#define MCP_AES_INI_KEY_0                                                            0x18015124
#define MCP_AES_INI_KEY_1                                                            0x18015128
#define MCP_AES_INI_KEY_2                                                            0x1801512C
#define MCP_AES_INI_KEY_3                                                            0x18015130
#define MCP_AES_INI_KEY_0_reg_addr                                                   "0xB8015124"
#define MCP_AES_INI_KEY_1_reg_addr                                                   "0xB8015128"
#define MCP_AES_INI_KEY_2_reg_addr                                                   "0xB801512C"
#define MCP_AES_INI_KEY_3_reg_addr                                                   "0xB8015130"
#define MCP_AES_INI_KEY_0_reg                                                        0xB8015124
#define MCP_AES_INI_KEY_1_reg                                                        0xB8015128
#define MCP_AES_INI_KEY_2_reg                                                        0xB801512C
#define MCP_AES_INI_KEY_3_reg                                                        0xB8015130
#define set_MCP_AES_INI_KEY_0_reg(data)   (*((volatile unsigned int*) MCP_AES_INI_KEY_0_reg)=data)
#define set_MCP_AES_INI_KEY_1_reg(data)   (*((volatile unsigned int*) MCP_AES_INI_KEY_1_reg)=data)
#define set_MCP_AES_INI_KEY_2_reg(data)   (*((volatile unsigned int*) MCP_AES_INI_KEY_2_reg)=data)
#define set_MCP_AES_INI_KEY_3_reg(data)   (*((volatile unsigned int*) MCP_AES_INI_KEY_3_reg)=data)
#define get_MCP_AES_INI_KEY_0_reg   (*((volatile unsigned int*) MCP_AES_INI_KEY_0_reg))
#define get_MCP_AES_INI_KEY_1_reg   (*((volatile unsigned int*) MCP_AES_INI_KEY_1_reg))
#define get_MCP_AES_INI_KEY_2_reg   (*((volatile unsigned int*) MCP_AES_INI_KEY_2_reg))
#define get_MCP_AES_INI_KEY_3_reg   (*((volatile unsigned int*) MCP_AES_INI_KEY_3_reg))
#define MCP_AES_INI_KEY_0_inst_adr                                                   "0x0049"
#define MCP_AES_INI_KEY_1_inst_adr                                                   "0x004A"
#define MCP_AES_INI_KEY_2_inst_adr                                                   "0x004B"
#define MCP_AES_INI_KEY_3_inst_adr                                                   "0x004C"
#define MCP_AES_INI_KEY_0_inst                                                       0x0049
#define MCP_AES_INI_KEY_1_inst                                                       0x004A
#define MCP_AES_INI_KEY_2_inst                                                       0x004B
#define MCP_AES_INI_KEY_3_inst                                                       0x004C
#define MCP_AES_INI_KEY_data_shift                                                   (0)
#define MCP_AES_INI_KEY_data_mask                                                    (0xFFFFFFFF)
#define MCP_AES_INI_KEY_data(data)                                                   (0xFFFFFFFF&((data)<<0))
#define MCP_AES_INI_KEY_data_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define MCP_AES_INI_KEY_get_data(data)                                               ((0xFFFFFFFF&(data))>>0)


#define MCP_MULTI2_SYS_KEY_0                                                         0x18015178
#define MCP_MULTI2_SYS_KEY_1                                                         0x1801517C
#define MCP_MULTI2_SYS_KEY_2                                                         0x18015180
#define MCP_MULTI2_SYS_KEY_3                                                         0x18015184
#define MCP_MULTI2_SYS_KEY_4                                                         0x18015188
#define MCP_MULTI2_SYS_KEY_5                                                         0x1801518C
#define MCP_MULTI2_SYS_KEY_6                                                         0x18015190
#define MCP_MULTI2_SYS_KEY_7                                                         0x18015194
#define MCP_MULTI2_SYS_KEY_0_reg_addr                                                "0xB8015178"
#define MCP_MULTI2_SYS_KEY_1_reg_addr                                                "0xB801517C"
#define MCP_MULTI2_SYS_KEY_2_reg_addr                                                "0xB8015180"
#define MCP_MULTI2_SYS_KEY_3_reg_addr                                                "0xB8015184"
#define MCP_MULTI2_SYS_KEY_4_reg_addr                                                "0xB8015188"
#define MCP_MULTI2_SYS_KEY_5_reg_addr                                                "0xB801518C"
#define MCP_MULTI2_SYS_KEY_6_reg_addr                                                "0xB8015190"
#define MCP_MULTI2_SYS_KEY_7_reg_addr                                                "0xB8015194"
#define MCP_MULTI2_SYS_KEY_0_reg                                                     0xB8015178
#define MCP_MULTI2_SYS_KEY_1_reg                                                     0xB801517C
#define MCP_MULTI2_SYS_KEY_2_reg                                                     0xB8015180
#define MCP_MULTI2_SYS_KEY_3_reg                                                     0xB8015184
#define MCP_MULTI2_SYS_KEY_4_reg                                                     0xB8015188
#define MCP_MULTI2_SYS_KEY_5_reg                                                     0xB801518C
#define MCP_MULTI2_SYS_KEY_6_reg                                                     0xB8015190
#define MCP_MULTI2_SYS_KEY_7_reg                                                     0xB8015194
#define set_MCP_MULTI2_SYS_KEY_0_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_0_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_1_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_1_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_2_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_2_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_3_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_3_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_4_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_4_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_5_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_5_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_6_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_6_reg)=data)
#define set_MCP_MULTI2_SYS_KEY_7_reg(data)   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_7_reg)=data)
#define get_MCP_MULTI2_SYS_KEY_0_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_0_reg))
#define get_MCP_MULTI2_SYS_KEY_1_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_1_reg))
#define get_MCP_MULTI2_SYS_KEY_2_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_2_reg))
#define get_MCP_MULTI2_SYS_KEY_3_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_3_reg))
#define get_MCP_MULTI2_SYS_KEY_4_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_4_reg))
#define get_MCP_MULTI2_SYS_KEY_5_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_5_reg))
#define get_MCP_MULTI2_SYS_KEY_6_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_6_reg))
#define get_MCP_MULTI2_SYS_KEY_7_reg   (*((volatile unsigned int*) MCP_MULTI2_SYS_KEY_7_reg))
#define MCP_MULTI2_SYS_KEY_0_inst_adr                                                "0x005E"
#define MCP_MULTI2_SYS_KEY_1_inst_adr                                                "0x005F"
#define MCP_MULTI2_SYS_KEY_2_inst_adr                                                "0x0060"
#define MCP_MULTI2_SYS_KEY_3_inst_adr                                                "0x0061"
#define MCP_MULTI2_SYS_KEY_4_inst_adr                                                "0x0062"
#define MCP_MULTI2_SYS_KEY_5_inst_adr                                                "0x0063"
#define MCP_MULTI2_SYS_KEY_6_inst_adr                                                "0x0064"
#define MCP_MULTI2_SYS_KEY_7_inst_adr                                                "0x0065"
#define MCP_MULTI2_SYS_KEY_0_inst                                                    0x005E
#define MCP_MULTI2_SYS_KEY_1_inst                                                    0x005F
#define MCP_MULTI2_SYS_KEY_2_inst                                                    0x0060
#define MCP_MULTI2_SYS_KEY_3_inst                                                    0x0061
#define MCP_MULTI2_SYS_KEY_4_inst                                                    0x0062
#define MCP_MULTI2_SYS_KEY_5_inst                                                    0x0063
#define MCP_MULTI2_SYS_KEY_6_inst                                                    0x0064
#define MCP_MULTI2_SYS_KEY_7_inst                                                    0x0065
#define MCP_MULTI2_SYS_KEY_data_shift                                                (0)
#define MCP_MULTI2_SYS_KEY_data_mask                                                 (0xFFFFFFFF)
#define MCP_MULTI2_SYS_KEY_data(data)                                                (0xFFFFFFFF&((data)<<0))
#define MCP_MULTI2_SYS_KEY_data_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define MCP_MULTI2_SYS_KEY_get_data(data)                                            ((0xFFFFFFFF&(data))>>0)


#define CP_OTP_LOAD                                                                  0x1801519C
#define CP_OTP_LOAD_reg_addr                                                         "0xB801519C"
#define CP_OTP_LOAD_reg                                                              0xB801519C
#define set_CP_OTP_LOAD_reg(data)   (*((volatile unsigned int*) CP_OTP_LOAD_reg)=data)
#define get_CP_OTP_LOAD_reg   (*((volatile unsigned int*) CP_OTP_LOAD_reg))
#define CP_OTP_LOAD_inst_adr                                                         "0x0067"
#define CP_OTP_LOAD_inst                                                             0x0067
#define CP_OTP_LOAD_otp_load_shift                                                   (0)
#define CP_OTP_LOAD_otp_load_mask                                                    (0x00000001)
#define CP_OTP_LOAD_otp_load(data)                                                   (0x00000001&((data)<<0))
#define CP_OTP_LOAD_otp_load_src(data)                                               ((0x00000001&(data))>>0)
#define CP_OTP_LOAD_get_otp_load(data)                                               ((0x00000001&(data))>>0)


#define K_MCP_CTRL                                                                   0x18015900
#define K_MCP_CTRL_reg_addr                                                          "0xB8015900"
#define K_MCP_CTRL_reg                                                               0xB8015900
#define set_K_MCP_CTRL_reg(data)   (*((volatile unsigned int*) K_MCP_CTRL_reg)=data)
#define get_K_MCP_CTRL_reg   (*((volatile unsigned int*) K_MCP_CTRL_reg))
#define K_MCP_CTRL_inst_adr                                                          "0x0040"
#define K_MCP_CTRL_inst                                                              0x0040
#define K_MCP_CTRL_ofb_2_shift                                                       (8)
#define K_MCP_CTRL_ofb_2_mask                                                        (0x00000100)
#define K_MCP_CTRL_ofb_2(data)                                                       (0x00000100&((data)<<8))
#define K_MCP_CTRL_ofb_2_src(data)                                                   ((0x00000100&(data))>>8)
#define K_MCP_CTRL_get_ofb_2(data)                                                   ((0x00000100&(data))>>8)
#define K_MCP_CTRL_ofb_1_shift                                                       (7)
#define K_MCP_CTRL_ofb_1_mask                                                        (0x00000080)
#define K_MCP_CTRL_ofb_1(data)                                                       (0x00000080&((data)<<7))
#define K_MCP_CTRL_ofb_1_src(data)                                                   ((0x00000080&(data))>>7)
#define K_MCP_CTRL_get_ofb_1(data)                                                   ((0x00000080&(data))>>7)
#define K_MCP_CTRL_clear_shift                                                       (4)
#define K_MCP_CTRL_clear_mask                                                        (0x00000010)
#define K_MCP_CTRL_clear(data)                                                       (0x00000010&((data)<<4))
#define K_MCP_CTRL_clear_src(data)                                                   ((0x00000010&(data))>>4)
#define K_MCP_CTRL_get_clear(data)                                                   ((0x00000010&(data))>>4)
#define K_MCP_CTRL_swap_shift                                                        (3)
#define K_MCP_CTRL_swap_mask                                                         (0x00000008)
#define K_MCP_CTRL_swap(data)                                                        (0x00000008&((data)<<3))
#define K_MCP_CTRL_swap_src(data)                                                    ((0x00000008&(data))>>3)
#define K_MCP_CTRL_get_swap(data)                                                    ((0x00000008&(data))>>3)
#define K_MCP_CTRL_idle_shift                                                        (2)
#define K_MCP_CTRL_idle_mask                                                         (0x00000004)
#define K_MCP_CTRL_idle(data)                                                        (0x00000004&((data)<<2))
#define K_MCP_CTRL_idle_src(data)                                                    ((0x00000004&(data))>>2)
#define K_MCP_CTRL_get_idle(data)                                                    ((0x00000004&(data))>>2)
#define K_MCP_CTRL_go_shift                                                          (1)
#define K_MCP_CTRL_go_mask                                                           (0x00000002)
#define K_MCP_CTRL_go(data)                                                          (0x00000002&((data)<<1))
#define K_MCP_CTRL_go_src(data)                                                      ((0x00000002&(data))>>1)
#define K_MCP_CTRL_get_go(data)                                                      ((0x00000002&(data))>>1)
#define K_MCP_CTRL_write_data_shift                                                  (0)
#define K_MCP_CTRL_write_data_mask                                                   (0x00000001)
#define K_MCP_CTRL_write_data(data)                                                  (0x00000001&((data)<<0))
#define K_MCP_CTRL_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define K_MCP_CTRL_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define K_MCP_STATUS                                                                 0x18015904
#define K_MCP_STATUS_reg_addr                                                        "0xB8015904"
#define K_MCP_STATUS_reg                                                             0xB8015904
#define set_K_MCP_STATUS_reg(data)   (*((volatile unsigned int*) K_MCP_STATUS_reg)=data)
#define get_K_MCP_STATUS_reg   (*((volatile unsigned int*) K_MCP_STATUS_reg))
#define K_MCP_STATUS_inst_adr                                                        "0x0041"
#define K_MCP_STATUS_inst                                                            0x0041
#define K_MCP_STATUS_s_mcp_error_shift                                               (10)
#define K_MCP_STATUS_s_mcp_error_mask                                                (0x00000400)
#define K_MCP_STATUS_s_mcp_error(data)                                               (0x00000400&((data)<<10))
#define K_MCP_STATUS_s_mcp_error_src(data)                                           ((0x00000400&(data))>>10)
#define K_MCP_STATUS_get_s_mcp_error(data)                                           ((0x00000400&(data))>>10)
#define K_MCP_STATUS_s_error_shift                                                   (9)
#define K_MCP_STATUS_s_error_mask                                                    (0x00000200)
#define K_MCP_STATUS_s_error(data)                                                   (0x00000200&((data)<<9))
#define K_MCP_STATUS_s_error_src(data)                                               ((0x00000200&(data))>>9)
#define K_MCP_STATUS_get_s_error(data)                                               ((0x00000200&(data))>>9)
#define K_MCP_STATUS_s_compare_shift                                                 (8)
#define K_MCP_STATUS_s_compare_mask                                                  (0x00000100)
#define K_MCP_STATUS_s_compare(data)                                                 (0x00000100&((data)<<8))
#define K_MCP_STATUS_s_compare_src(data)                                             ((0x00000100&(data))>>8)
#define K_MCP_STATUS_get_s_compare(data)                                             ((0x00000100&(data))>>8)
#define K_MCP_STATUS_s_ring_empty_shift                                              (7)
#define K_MCP_STATUS_s_ring_empty_mask                                               (0x00000080)
#define K_MCP_STATUS_s_ring_empty(data)                                              (0x00000080&((data)<<7))
#define K_MCP_STATUS_s_ring_empty_src(data)                                          ((0x00000080&(data))>>7)
#define K_MCP_STATUS_get_s_ring_empty(data)                                          ((0x00000080&(data))>>7)
#define K_MCP_STATUS_mcp_error_shift                                                 (6)
#define K_MCP_STATUS_mcp_error_mask                                                  (0x00000040)
#define K_MCP_STATUS_mcp_error(data)                                                 (0x00000040&((data)<<6))
#define K_MCP_STATUS_mcp_error_src(data)                                             ((0x00000040&(data))>>6)
#define K_MCP_STATUS_get_mcp_error(data)                                             ((0x00000040&(data))>>6)
#define K_MCP_STATUS_compare_shift                                                   (3)
#define K_MCP_STATUS_compare_mask                                                    (0x00000008)
#define K_MCP_STATUS_compare(data)                                                   (0x00000008&((data)<<3))
#define K_MCP_STATUS_compare_src(data)                                               ((0x00000008&(data))>>3)
#define K_MCP_STATUS_get_compare(data)                                               ((0x00000008&(data))>>3)
#define K_MCP_STATUS_error_shift                                                     (2)
#define K_MCP_STATUS_error_mask                                                      (0x00000004)
#define K_MCP_STATUS_error(data)                                                     (0x00000004&((data)<<2))
#define K_MCP_STATUS_error_src(data)                                                 ((0x00000004&(data))>>2)
#define K_MCP_STATUS_get_error(data)                                                 ((0x00000004&(data))>>2)
#define K_MCP_STATUS_ring_empty_shift                                                (1)
#define K_MCP_STATUS_ring_empty_mask                                                 (0x00000002)
#define K_MCP_STATUS_ring_empty(data)                                                (0x00000002&((data)<<1))
#define K_MCP_STATUS_ring_empty_src(data)                                            ((0x00000002&(data))>>1)
#define K_MCP_STATUS_get_ring_empty(data)                                            ((0x00000002&(data))>>1)
#define K_MCP_STATUS_write_data_shift                                                (0)
#define K_MCP_STATUS_write_data_mask                                                 (0x00000001)
#define K_MCP_STATUS_write_data(data)                                                (0x00000001&((data)<<0))
#define K_MCP_STATUS_write_data_src(data)                                            ((0x00000001&(data))>>0)
#define K_MCP_STATUS_get_write_data(data)                                            ((0x00000001&(data))>>0)


#define K_MCP_EN                                                                     0x18015908
#define K_MCP_EN_reg_addr                                                            "0xB8015908"
#define K_MCP_EN_reg                                                                 0xB8015908
#define set_K_MCP_EN_reg(data)   (*((volatile unsigned int*) K_MCP_EN_reg)=data)
#define get_K_MCP_EN_reg   (*((volatile unsigned int*) K_MCP_EN_reg))
#define K_MCP_EN_inst_adr                                                            "0x0042"
#define K_MCP_EN_inst                                                                0x0042
#define K_MCP_EN_mcp_error_en_shift                                                  (6)
#define K_MCP_EN_mcp_error_en_mask                                                   (0x00000040)
#define K_MCP_EN_mcp_error_en(data)                                                  (0x00000040&((data)<<6))
#define K_MCP_EN_mcp_error_en_src(data)                                              ((0x00000040&(data))>>6)
#define K_MCP_EN_get_mcp_error_en(data)                                              ((0x00000040&(data))>>6)
#define K_MCP_EN_compare_en_shift                                                    (3)
#define K_MCP_EN_compare_en_mask                                                     (0x00000008)
#define K_MCP_EN_compare_en(data)                                                    (0x00000008&((data)<<3))
#define K_MCP_EN_compare_en_src(data)                                                ((0x00000008&(data))>>3)
#define K_MCP_EN_get_compare_en(data)                                                ((0x00000008&(data))>>3)
#define K_MCP_EN_error_en_shift                                                      (2)
#define K_MCP_EN_error_en_mask                                                       (0x00000004)
#define K_MCP_EN_error_en(data)                                                      (0x00000004&((data)<<2))
#define K_MCP_EN_error_en_src(data)                                                  ((0x00000004&(data))>>2)
#define K_MCP_EN_get_error_en(data)                                                  ((0x00000004&(data))>>2)
#define K_MCP_EN_ring_empty_en_shift                                                 (1)
#define K_MCP_EN_ring_empty_en_mask                                                  (0x00000002)
#define K_MCP_EN_ring_empty_en(data)                                                 (0x00000002&((data)<<1))
#define K_MCP_EN_ring_empty_en_src(data)                                             ((0x00000002&(data))>>1)
#define K_MCP_EN_get_ring_empty_en(data)                                             ((0x00000002&(data))>>1)
#define K_MCP_EN_write_data_shift                                                    (0)
#define K_MCP_EN_write_data_mask                                                     (0x00000001)
#define K_MCP_EN_write_data(data)                                                    (0x00000001&((data)<<0))
#define K_MCP_EN_write_data_src(data)                                                ((0x00000001&(data))>>0)
#define K_MCP_EN_get_write_data(data)                                                ((0x00000001&(data))>>0)


#define K_MCP_CTRL1                                                                  0x18015998
#define K_MCP_CTRL1_reg_addr                                                         "0xB8015998"
#define K_MCP_CTRL1_reg                                                              0xB8015998
#define set_K_MCP_CTRL1_reg(data)   (*((volatile unsigned int*) K_MCP_CTRL1_reg)=data)
#define get_K_MCP_CTRL1_reg   (*((volatile unsigned int*) K_MCP_CTRL1_reg))
#define K_MCP_CTRL1_inst_adr                                                         "0x0066"
#define K_MCP_CTRL1_inst                                                             0x0066
#define K_MCP_CTRL1_aesh_pad_off_shift                                               (10)
#define K_MCP_CTRL1_aesh_pad_off_mask                                                (0x00000400)
#define K_MCP_CTRL1_aesh_pad_off(data)                                               (0x00000400&((data)<<10))
#define K_MCP_CTRL1_aesh_pad_off_src(data)                                           ((0x00000400&(data))>>10)
#define K_MCP_CTRL1_get_aesh_pad_off(data)                                           ((0x00000400&(data))>>10)
#define K_MCP_CTRL1_csa_entropy_shift                                                (8)
#define K_MCP_CTRL1_csa_entropy_mask                                                 (0x00000300)
#define K_MCP_CTRL1_csa_entropy(data)                                                (0x00000300&((data)<<8))
#define K_MCP_CTRL1_csa_entropy_src(data)                                            ((0x00000300&(data))>>8)
#define K_MCP_CTRL1_get_csa_entropy(data)                                            ((0x00000300&(data))>>8)
#define K_MCP_CTRL1_mcp_round_no_shift                                               (0)
#define K_MCP_CTRL1_mcp_round_no_mask                                                (0x000000FF)
#define K_MCP_CTRL1_mcp_round_no(data)                                               (0x000000FF&((data)<<0))
#define K_MCP_CTRL1_mcp_round_no_src(data)                                           ((0x000000FF&(data))>>0)
#define K_MCP_CTRL1_get_mcp_round_no(data)                                           ((0x000000FF&(data))>>0)


#define K_MCP_BASE                                                                   0x1801590C
#define K_MCP_BASE_reg_addr                                                          "0xB801590C"
#define K_MCP_BASE_reg                                                               0xB801590C
#define set_K_MCP_BASE_reg(data)   (*((volatile unsigned int*) K_MCP_BASE_reg)=data)
#define get_K_MCP_BASE_reg   (*((volatile unsigned int*) K_MCP_BASE_reg))
#define K_MCP_BASE_inst_adr                                                          "0x0043"
#define K_MCP_BASE_inst                                                              0x0043
#define K_MCP_BASE_base_shift                                                        (0)
#define K_MCP_BASE_base_mask                                                         (0x7FFFFFFF)
#define K_MCP_BASE_base(data)                                                        (0x7FFFFFFF&((data)<<0))
#define K_MCP_BASE_base_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define K_MCP_BASE_get_base(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define K_MCP_LIMIT                                                                  0x18015910
#define K_MCP_LIMIT_reg_addr                                                         "0xB8015910"
#define K_MCP_LIMIT_reg                                                              0xB8015910
#define set_K_MCP_LIMIT_reg(data)   (*((volatile unsigned int*) K_MCP_LIMIT_reg)=data)
#define get_K_MCP_LIMIT_reg   (*((volatile unsigned int*) K_MCP_LIMIT_reg))
#define K_MCP_LIMIT_inst_adr                                                         "0x0044"
#define K_MCP_LIMIT_inst                                                             0x0044
#define K_MCP_LIMIT_limit_shift                                                      (0)
#define K_MCP_LIMIT_limit_mask                                                       (0x7FFFFFFF)
#define K_MCP_LIMIT_limit(data)                                                      (0x7FFFFFFF&((data)<<0))
#define K_MCP_LIMIT_limit_src(data)                                                  ((0x7FFFFFFF&(data))>>0)
#define K_MCP_LIMIT_get_limit(data)                                                  ((0x7FFFFFFF&(data))>>0)


#define K_MCP_RDPTR                                                                  0x18015914
#define K_MCP_RDPTR_reg_addr                                                         "0xB8015914"
#define K_MCP_RDPTR_reg                                                              0xB8015914
#define set_K_MCP_RDPTR_reg(data)   (*((volatile unsigned int*) K_MCP_RDPTR_reg)=data)
#define get_K_MCP_RDPTR_reg   (*((volatile unsigned int*) K_MCP_RDPTR_reg))
#define K_MCP_RDPTR_inst_adr                                                         "0x0045"
#define K_MCP_RDPTR_inst                                                             0x0045
#define K_MCP_RDPTR_rdptr_shift                                                      (0)
#define K_MCP_RDPTR_rdptr_mask                                                       (0x7FFFFFFF)
#define K_MCP_RDPTR_rdptr(data)                                                      (0x7FFFFFFF&((data)<<0))
#define K_MCP_RDPTR_rdptr_src(data)                                                  ((0x7FFFFFFF&(data))>>0)
#define K_MCP_RDPTR_get_rdptr(data)                                                  ((0x7FFFFFFF&(data))>>0)


#define K_MCP_WRPTR                                                                  0x18015918
#define K_MCP_WRPTR_reg_addr                                                         "0xB8015918"
#define K_MCP_WRPTR_reg                                                              0xB8015918
#define set_K_MCP_WRPTR_reg(data)   (*((volatile unsigned int*) K_MCP_WRPTR_reg)=data)
#define get_K_MCP_WRPTR_reg   (*((volatile unsigned int*) K_MCP_WRPTR_reg))
#define K_MCP_WRPTR_inst_adr                                                         "0x0046"
#define K_MCP_WRPTR_inst                                                             0x0046
#define K_MCP_WRPTR_wrptr_shift                                                      (0)
#define K_MCP_WRPTR_wrptr_mask                                                       (0x7FFFFFFF)
#define K_MCP_WRPTR_wrptr(data)                                                      (0x7FFFFFFF&((data)<<0))
#define K_MCP_WRPTR_wrptr_src(data)                                                  ((0x7FFFFFFF&(data))>>0)
#define K_MCP_WRPTR_get_wrptr(data)                                                  ((0x7FFFFFFF&(data))>>0)


#define K_MCP_DES_COUNT                                                              0x18015934
#define K_MCP_DES_COUNT_reg_addr                                                     "0xB8015934"
#define K_MCP_DES_COUNT_reg                                                          0xB8015934
#define set_K_MCP_DES_COUNT_reg(data)   (*((volatile unsigned int*) K_MCP_DES_COUNT_reg)=data)
#define get_K_MCP_DES_COUNT_reg   (*((volatile unsigned int*) K_MCP_DES_COUNT_reg))
#define K_MCP_DES_COUNT_inst_adr                                                     "0x004D"
#define K_MCP_DES_COUNT_inst                                                         0x004D
#define K_MCP_DES_COUNT_count_shift                                                  (0)
#define K_MCP_DES_COUNT_count_mask                                                   (0x0000FFFF)
#define K_MCP_DES_COUNT_count(data)                                                  (0x0000FFFF&((data)<<0))
#define K_MCP_DES_COUNT_count_src(data)                                              ((0x0000FFFF&(data))>>0)
#define K_MCP_DES_COUNT_get_count(data)                                              ((0x0000FFFF&(data))>>0)


#define K_MCP_DES_COMPARE                                                            0x18015938
#define K_MCP_DES_COMPARE_reg_addr                                                   "0xB8015938"
#define K_MCP_DES_COMPARE_reg                                                        0xB8015938
#define set_K_MCP_DES_COMPARE_reg(data)   (*((volatile unsigned int*) K_MCP_DES_COMPARE_reg)=data)
#define get_K_MCP_DES_COMPARE_reg   (*((volatile unsigned int*) K_MCP_DES_COMPARE_reg))
#define K_MCP_DES_COMPARE_inst_adr                                                   "0x004E"
#define K_MCP_DES_COMPARE_inst                                                       0x004E
#define K_MCP_DES_COMPARE_compare_shift                                              (0)
#define K_MCP_DES_COMPARE_compare_mask                                               (0x0000FFFF)
#define K_MCP_DES_COMPARE_compare(data)                                              (0x0000FFFF&((data)<<0))
#define K_MCP_DES_COMPARE_compare_src(data)                                          ((0x0000FFFF&(data))>>0)
#define K_MCP_DES_COMPARE_get_compare(data)                                          ((0x0000FFFF&(data))>>0)


#define K_MCP_DES_INI_KEY_0                                                          0x1801591C
#define K_MCP_DES_INI_KEY_1                                                          0x18015920
#define K_MCP_DES_INI_KEY_0_reg_addr                                                 "0xB801591C"
#define K_MCP_DES_INI_KEY_1_reg_addr                                                 "0xB8015920"
#define K_MCP_DES_INI_KEY_0_reg                                                      0xB801591C
#define K_MCP_DES_INI_KEY_1_reg                                                      0xB8015920
#define set_K_MCP_DES_INI_KEY_0_reg(data)   (*((volatile unsigned int*) K_MCP_DES_INI_KEY_0_reg)=data)
#define set_K_MCP_DES_INI_KEY_1_reg(data)   (*((volatile unsigned int*) K_MCP_DES_INI_KEY_1_reg)=data)
#define get_K_MCP_DES_INI_KEY_0_reg   (*((volatile unsigned int*) K_MCP_DES_INI_KEY_0_reg))
#define get_K_MCP_DES_INI_KEY_1_reg   (*((volatile unsigned int*) K_MCP_DES_INI_KEY_1_reg))
#define K_MCP_DES_INI_KEY_0_inst_adr                                                 "0x0047"
#define K_MCP_DES_INI_KEY_1_inst_adr                                                 "0x0048"
#define K_MCP_DES_INI_KEY_0_inst                                                     0x0047
#define K_MCP_DES_INI_KEY_1_inst                                                     0x0048
#define K_MCP_DES_INI_KEY_data_shift                                                 (0)
#define K_MCP_DES_INI_KEY_data_mask                                                  (0xFFFFFFFF)
#define K_MCP_DES_INI_KEY_data(data)                                                 (0xFFFFFFFF&((data)<<0))
#define K_MCP_DES_INI_KEY_data_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define K_MCP_DES_INI_KEY_get_data(data)                                             ((0xFFFFFFFF&(data))>>0)


#define K_MCP_AES_INI_KEY_0                                                          0x18015924
#define K_MCP_AES_INI_KEY_1                                                          0x18015928
#define K_MCP_AES_INI_KEY_2                                                          0x1801592C
#define K_MCP_AES_INI_KEY_3                                                          0x18015930
#define K_MCP_AES_INI_KEY_0_reg_addr                                                 "0xB8015924"
#define K_MCP_AES_INI_KEY_1_reg_addr                                                 "0xB8015928"
#define K_MCP_AES_INI_KEY_2_reg_addr                                                 "0xB801592C"
#define K_MCP_AES_INI_KEY_3_reg_addr                                                 "0xB8015930"
#define K_MCP_AES_INI_KEY_0_reg                                                      0xB8015924
#define K_MCP_AES_INI_KEY_1_reg                                                      0xB8015928
#define K_MCP_AES_INI_KEY_2_reg                                                      0xB801592C
#define K_MCP_AES_INI_KEY_3_reg                                                      0xB8015930
#define set_K_MCP_AES_INI_KEY_0_reg(data)   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_0_reg)=data)
#define set_K_MCP_AES_INI_KEY_1_reg(data)   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_1_reg)=data)
#define set_K_MCP_AES_INI_KEY_2_reg(data)   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_2_reg)=data)
#define set_K_MCP_AES_INI_KEY_3_reg(data)   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_3_reg)=data)
#define get_K_MCP_AES_INI_KEY_0_reg   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_0_reg))
#define get_K_MCP_AES_INI_KEY_1_reg   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_1_reg))
#define get_K_MCP_AES_INI_KEY_2_reg   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_2_reg))
#define get_K_MCP_AES_INI_KEY_3_reg   (*((volatile unsigned int*) K_MCP_AES_INI_KEY_3_reg))
#define K_MCP_AES_INI_KEY_0_inst_adr                                                 "0x0049"
#define K_MCP_AES_INI_KEY_1_inst_adr                                                 "0x004A"
#define K_MCP_AES_INI_KEY_2_inst_adr                                                 "0x004B"
#define K_MCP_AES_INI_KEY_3_inst_adr                                                 "0x004C"
#define K_MCP_AES_INI_KEY_0_inst                                                     0x0049
#define K_MCP_AES_INI_KEY_1_inst                                                     0x004A
#define K_MCP_AES_INI_KEY_2_inst                                                     0x004B
#define K_MCP_AES_INI_KEY_3_inst                                                     0x004C
#define K_MCP_AES_INI_KEY_data_shift                                                 (0)
#define K_MCP_AES_INI_KEY_data_mask                                                  (0xFFFFFFFF)
#define K_MCP_AES_INI_KEY_data(data)                                                 (0xFFFFFFFF&((data)<<0))
#define K_MCP_AES_INI_KEY_data_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define K_MCP_AES_INI_KEY_get_data(data)                                             ((0xFFFFFFFF&(data))>>0)


#define K_MCP_MULTI2_SYS_KEY_0                                                       0x18015978
#define K_MCP_MULTI2_SYS_KEY_1                                                       0x1801597C
#define K_MCP_MULTI2_SYS_KEY_2                                                       0x18015980
#define K_MCP_MULTI2_SYS_KEY_3                                                       0x18015984
#define K_MCP_MULTI2_SYS_KEY_4                                                       0x18015988
#define K_MCP_MULTI2_SYS_KEY_5                                                       0x1801598C
#define K_MCP_MULTI2_SYS_KEY_6                                                       0x18015990
#define K_MCP_MULTI2_SYS_KEY_7                                                       0x18015994
#define K_MCP_MULTI2_SYS_KEY_0_reg_addr                                              "0xB8015978"
#define K_MCP_MULTI2_SYS_KEY_1_reg_addr                                              "0xB801597C"
#define K_MCP_MULTI2_SYS_KEY_2_reg_addr                                              "0xB8015980"
#define K_MCP_MULTI2_SYS_KEY_3_reg_addr                                              "0xB8015984"
#define K_MCP_MULTI2_SYS_KEY_4_reg_addr                                              "0xB8015988"
#define K_MCP_MULTI2_SYS_KEY_5_reg_addr                                              "0xB801598C"
#define K_MCP_MULTI2_SYS_KEY_6_reg_addr                                              "0xB8015990"
#define K_MCP_MULTI2_SYS_KEY_7_reg_addr                                              "0xB8015994"
#define K_MCP_MULTI2_SYS_KEY_0_reg                                                   0xB8015978
#define K_MCP_MULTI2_SYS_KEY_1_reg                                                   0xB801597C
#define K_MCP_MULTI2_SYS_KEY_2_reg                                                   0xB8015980
#define K_MCP_MULTI2_SYS_KEY_3_reg                                                   0xB8015984
#define K_MCP_MULTI2_SYS_KEY_4_reg                                                   0xB8015988
#define K_MCP_MULTI2_SYS_KEY_5_reg                                                   0xB801598C
#define K_MCP_MULTI2_SYS_KEY_6_reg                                                   0xB8015990
#define K_MCP_MULTI2_SYS_KEY_7_reg                                                   0xB8015994
#define set_K_MCP_MULTI2_SYS_KEY_0_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_0_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_1_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_1_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_2_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_2_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_3_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_3_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_4_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_4_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_5_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_5_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_6_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_6_reg)=data)
#define set_K_MCP_MULTI2_SYS_KEY_7_reg(data)   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_7_reg)=data)
#define get_K_MCP_MULTI2_SYS_KEY_0_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_0_reg))
#define get_K_MCP_MULTI2_SYS_KEY_1_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_1_reg))
#define get_K_MCP_MULTI2_SYS_KEY_2_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_2_reg))
#define get_K_MCP_MULTI2_SYS_KEY_3_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_3_reg))
#define get_K_MCP_MULTI2_SYS_KEY_4_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_4_reg))
#define get_K_MCP_MULTI2_SYS_KEY_5_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_5_reg))
#define get_K_MCP_MULTI2_SYS_KEY_6_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_6_reg))
#define get_K_MCP_MULTI2_SYS_KEY_7_reg   (*((volatile unsigned int*) K_MCP_MULTI2_SYS_KEY_7_reg))
#define K_MCP_MULTI2_SYS_KEY_0_inst_adr                                              "0x005E"
#define K_MCP_MULTI2_SYS_KEY_1_inst_adr                                              "0x005F"
#define K_MCP_MULTI2_SYS_KEY_2_inst_adr                                              "0x0060"
#define K_MCP_MULTI2_SYS_KEY_3_inst_adr                                              "0x0061"
#define K_MCP_MULTI2_SYS_KEY_4_inst_adr                                              "0x0062"
#define K_MCP_MULTI2_SYS_KEY_5_inst_adr                                              "0x0063"
#define K_MCP_MULTI2_SYS_KEY_6_inst_adr                                              "0x0064"
#define K_MCP_MULTI2_SYS_KEY_7_inst_adr                                              "0x0065"
#define K_MCP_MULTI2_SYS_KEY_0_inst                                                  0x005E
#define K_MCP_MULTI2_SYS_KEY_1_inst                                                  0x005F
#define K_MCP_MULTI2_SYS_KEY_2_inst                                                  0x0060
#define K_MCP_MULTI2_SYS_KEY_3_inst                                                  0x0061
#define K_MCP_MULTI2_SYS_KEY_4_inst                                                  0x0062
#define K_MCP_MULTI2_SYS_KEY_5_inst                                                  0x0063
#define K_MCP_MULTI2_SYS_KEY_6_inst                                                  0x0064
#define K_MCP_MULTI2_SYS_KEY_7_inst                                                  0x0065
#define K_MCP_MULTI2_SYS_KEY_data_shift                                              (0)
#define K_MCP_MULTI2_SYS_KEY_data_mask                                               (0xFFFFFFFF)
#define K_MCP_MULTI2_SYS_KEY_data(data)                                              (0xFFFFFFFF&((data)<<0))
#define K_MCP_MULTI2_SYS_KEY_data_src(data)                                          ((0xFFFFFFFF&(data))>>0)
#define K_MCP_MULTI2_SYS_KEY_get_data(data)                                          ((0xFFFFFFFF&(data))>>0)


#define MCP_ST                                                                       0x180151a0
#define MCP_ST_reg_addr                                                              "0xB80151A0"
#define MCP_ST_reg                                                                   0xB80151A0
#define set_MCP_ST_reg(data)   (*((volatile unsigned int*) MCP_ST_reg)=data)
#define get_MCP_ST_reg   (*((volatile unsigned int*) MCP_ST_reg))
#define MCP_ST_inst_adr                                                              "0x0068"
#define MCP_ST_inst                                                                  0x0068
#define MCP_ST_rd_sm_shift                                                           (24)
#define MCP_ST_rd_sm_mask                                                            (0x0F000000)
#define MCP_ST_rd_sm(data)                                                           (0x0F000000&((data)<<24))
#define MCP_ST_rd_sm_src(data)                                                       ((0x0F000000&(data))>>24)
#define MCP_ST_get_rd_sm(data)                                                       ((0x0F000000&(data))>>24)
#define MCP_ST_wr_sm_shift                                                           (21)
#define MCP_ST_wr_sm_mask                                                            (0x00E00000)
#define MCP_ST_wr_sm(data)                                                           (0x00E00000&((data)<<21))
#define MCP_ST_wr_sm_src(data)                                                       ((0x00E00000&(data))>>21)
#define MCP_ST_get_wr_sm(data)                                                       ((0x00E00000&(data))>>21)
#define MCP_ST_mcp_top_sm_shift                                                      (11)
#define MCP_ST_mcp_top_sm_mask                                                       (0x001FF800)
#define MCP_ST_mcp_top_sm(data)                                                      (0x001FF800&((data)<<11))
#define MCP_ST_mcp_top_sm_src(data)                                                  ((0x001FF800&(data))>>11)
#define MCP_ST_get_mcp_top_sm(data)                                                  ((0x001FF800&(data))>>11)
#define MCP_ST_mcp_aes_like_sm_shift                                                 (0)
#define MCP_ST_mcp_aes_like_sm_mask                                                  (0x000007FF)
#define MCP_ST_mcp_aes_like_sm(data)                                                 (0x000007FF&((data)<<0))
#define MCP_ST_mcp_aes_like_sm_src(data)                                             ((0x000007FF&(data))>>0)
#define MCP_ST_get_mcp_aes_like_sm(data)                                             ((0x000007FF&(data))>>0)


#endif
