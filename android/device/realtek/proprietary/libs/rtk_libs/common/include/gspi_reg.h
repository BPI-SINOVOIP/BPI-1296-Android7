/**************************************************************
// Spec Version                  : 0.3
// Parser Version                : DVR_Parser_5.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name_reg
// Firmware Header Generate Date : 2010/5/5
***************************************************************/


#ifndef _GSPI_REG_H_INCLUDED_
#define _GSPI_REG_H_INCLUDED_
#ifdef  _GSPI_USE_STRUCT
typedef struct 
{
unsigned int     Cmd_len:7;
unsigned int     div_3_en:1;
unsigned int     sck_h:8;
unsigned int     sck_l:8;
unsigned int     reserved_0:6;
unsigned int     phase:1;
unsigned int     polarity:1;
}SCK_CTRL;

typedef struct 
{
unsigned int     RWCMD:1;
unsigned int     reserved_0:1;
unsigned int     DMY_LEN:6;
unsigned int     ADDR_LEN:8;
unsigned int     reserved_1:1;
unsigned int     SOdualmodeselection:2;
unsigned int     First_bit_sel:1;
unsigned int     LSBfirst:1;
unsigned int     reserved_2:1;
unsigned int     WR_LEN:10;
}SO_CTRL;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     reserved_1:6;
unsigned int     reserved_2:8;
unsigned int     reserved_3:4;
unsigned int     First_bit_sel:1;
unsigned int     LSBfirst:1;
unsigned int     RD_LEN:10;
}SI_CTRL;

typedef struct 
{
unsigned int     W_FIFO:32;
}SPI_W_FIFO;

typedef struct 
{
unsigned int     R_FIFO:32;
}SPI_R_FIFO;

typedef struct 
{
unsigned int     SPI_INS:32;
}SPI_INSTRUCTION;

typedef struct 
{
unsigned int     SPI_ADDR:32;
}SPI_ADDR;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     Tcs_high:8;
unsigned int     Tcs_end:8;
unsigned int     Tcs_start:8;
}CE_TIMING_CTRL;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     reserved_1:8;
unsigned int     reserved_2:7;
unsigned int     CMD_D_IE:1;
unsigned int     reserved_3:6;
unsigned int     FIFO_PTR_RST:1;
unsigned int     CMD_ACT:1;
}SPI_CTRL;

typedef struct 
{
unsigned int     R_FIFO_PTR:4;
unsigned int     reserved_0:2;
unsigned int     R_FIFO_EMPTY:1;
unsigned int     R_FIFO_FULL:1;
unsigned int     W_FIFO_PTR:4;
unsigned int     reserved_1:2;
unsigned int     W_FIFO_EMPTY:1;
unsigned int     W_FIFO_FULL:1;
unsigned int     reserved_2:8;
unsigned int     reserved_3:6;
unsigned int     SFC_BUSY:1;
unsigned int     CMD_DONE:1;
}SPI_STATUS;

#endif

#define SCK_CTRL                                                                     0x1801BD00
#define SCK_CTRL_reg_addr                                                            "0xB801BD00"
#define SCK_CTRL_reg                                                                 0xB801BD00
#define set_SCK_CTRL_reg(data)   (*((volatile unsigned int*) SCK_CTRL_reg)=data)
#define get_SCK_CTRL_reg   (*((volatile unsigned int*) SCK_CTRL_reg))
#define SCK_CTRL_inst_adr                                                            "0x0040"
#define SCK_CTRL_inst                                                                0x0040
#define SCK_CTRL_Cmd_len_shift                                                       (25)
#define SCK_CTRL_Cmd_len_mask                                                        (0xFE000000)
#define SCK_CTRL_Cmd_len(data)                                                       (0xFE000000&((data)<<25))
#define SCK_CTRL_Cmd_len_src(data)                                                   ((0xFE000000&(data))>>25)
#define SCK_CTRL_get_Cmd_len(data)                                                   ((0xFE000000&(data))>>25)
#define SCK_CTRL_div_3_en_shift                                                      (24)
#define SCK_CTRL_div_3_en_mask                                                       (0x01000000)
#define SCK_CTRL_div_3_en(data)                                                      (0x01000000&((data)<<24))
#define SCK_CTRL_div_3_en_src(data)                                                  ((0x01000000&(data))>>24)
#define SCK_CTRL_get_div_3_en(data)                                                  ((0x01000000&(data))>>24)
#define SCK_CTRL_sck_h_shift                                                         (16)
#define SCK_CTRL_sck_h_mask                                                          (0x00FF0000)
#define SCK_CTRL_sck_h(data)                                                         (0x00FF0000&((data)<<16))
#define SCK_CTRL_sck_h_src(data)                                                     ((0x00FF0000&(data))>>16)
#define SCK_CTRL_get_sck_h(data)                                                     ((0x00FF0000&(data))>>16)
#define SCK_CTRL_sck_l_shift                                                         (8)
#define SCK_CTRL_sck_l_mask                                                          (0x0000FF00)
#define SCK_CTRL_sck_l(data)                                                         (0x0000FF00&((data)<<8))
#define SCK_CTRL_sck_l_src(data)                                                     ((0x0000FF00&(data))>>8)
#define SCK_CTRL_get_sck_l(data)                                                     ((0x0000FF00&(data))>>8)
#define SCK_CTRL_phase_shift                                                         (1)
#define SCK_CTRL_phase_mask                                                          (0x00000002)
#define SCK_CTRL_phase(data)                                                         (0x00000002&((data)<<1))
#define SCK_CTRL_phase_src(data)                                                     ((0x00000002&(data))>>1)
#define SCK_CTRL_get_phase(data)                                                     ((0x00000002&(data))>>1)
#define SCK_CTRL_polarity_shift                                                      (0)
#define SCK_CTRL_polarity_mask                                                       (0x00000001)
#define SCK_CTRL_polarity(data)                                                      (0x00000001&((data)<<0))
#define SCK_CTRL_polarity_src(data)                                                  ((0x00000001&(data))>>0)
#define SCK_CTRL_get_polarity(data)                                                  ((0x00000001&(data))>>0)


#define SO_CTRL                                                                      0x1801BD04
#define SO_CTRL_reg_addr                                                             "0xB801BD04"
#define SO_CTRL_reg                                                                  0xB801BD04
#define set_SO_CTRL_reg(data)   (*((volatile unsigned int*) SO_CTRL_reg)=data)
#define get_SO_CTRL_reg   (*((volatile unsigned int*) SO_CTRL_reg))
#define SO_CTRL_inst_adr                                                             "0x0041"
#define SO_CTRL_inst                                                                 0x0041
#define SO_CTRL_RWCMD_shift                                                          (31)
#define SO_CTRL_RWCMD_mask                                                           (0x80000000)
#define SO_CTRL_RWCMD(data)                                                          (0x80000000&((data)<<31))
#define SO_CTRL_RWCMD_src(data)                                                      ((0x80000000&(data))>>31)
#define SO_CTRL_get_RWCMD(data)                                                      ((0x80000000&(data))>>31)
#define SO_CTRL_DMY_LEN_shift                                                        (24)
#define SO_CTRL_DMY_LEN_mask                                                         (0x3F000000)
#define SO_CTRL_DMY_LEN(data)                                                        (0x3F000000&((data)<<24))
#define SO_CTRL_DMY_LEN_src(data)                                                    ((0x3F000000&(data))>>24)
#define SO_CTRL_get_DMY_LEN(data)                                                    ((0x3F000000&(data))>>24)
#define SO_CTRL_ADDR_LEN_shift                                                       (16)
#define SO_CTRL_ADDR_LEN_mask                                                        (0x00FF0000)
#define SO_CTRL_ADDR_LEN(data)                                                       (0x00FF0000&((data)<<16))
#define SO_CTRL_ADDR_LEN_src(data)                                                   ((0x00FF0000&(data))>>16)
#define SO_CTRL_get_ADDR_LEN(data)                                                   ((0x00FF0000&(data))>>16)
#define SO_CTRL_SOdualmodeselection_shift                                            (13)
#define SO_CTRL_SOdualmodeselection_mask                                             (0x00006000)
#define SO_CTRL_SOdualmodeselection(data)                                            (0x00006000&((data)<<13))
#define SO_CTRL_SOdualmodeselection_src(data)                                        ((0x00006000&(data))>>13)
#define SO_CTRL_get_SOdualmodeselection(data)                                        ((0x00006000&(data))>>13)
#define SO_CTRL_First_bit_sel_shift                                                  (12)
#define SO_CTRL_First_bit_sel_mask                                                   (0x00001000)
#define SO_CTRL_First_bit_sel(data)                                                  (0x00001000&((data)<<12))
#define SO_CTRL_First_bit_sel_src(data)                                              ((0x00001000&(data))>>12)
#define SO_CTRL_get_First_bit_sel(data)                                              ((0x00001000&(data))>>12)
#define SO_CTRL_LSBfirst_shift                                                       (11)
#define SO_CTRL_LSBfirst_mask                                                        (0x00000800)
#define SO_CTRL_LSBfirst(data)                                                       (0x00000800&((data)<<11))
#define SO_CTRL_LSBfirst_src(data)                                                   ((0x00000800&(data))>>11)
#define SO_CTRL_get_LSBfirst(data)                                                   ((0x00000800&(data))>>11)
#define SO_CTRL_WR_LEN_shift                                                         (0)
#define SO_CTRL_WR_LEN_mask                                                          (0x000003FF)
#define SO_CTRL_WR_LEN(data)                                                         (0x000003FF&((data)<<0))
#define SO_CTRL_WR_LEN_src(data)                                                     ((0x000003FF&(data))>>0)
#define SO_CTRL_get_WR_LEN(data)                                                     ((0x000003FF&(data))>>0)


#define SI_CTRL                                                                      0x1801BD08
#define SI_CTRL_reg_addr                                                             "0xB801BD08"
#define SI_CTRL_reg                                                                  0xB801BD08
#define set_SI_CTRL_reg(data)   (*((volatile unsigned int*) SI_CTRL_reg)=data)
#define get_SI_CTRL_reg   (*((volatile unsigned int*) SI_CTRL_reg))
#define SI_CTRL_inst_adr                                                             "0x0042"
#define SI_CTRL_inst                                                                 0x0042
#define SI_CTRL_First_bit_sel_shift                                                  (11)
#define SI_CTRL_First_bit_sel_mask                                                   (0x00000800)
#define SI_CTRL_First_bit_sel(data)                                                  (0x00000800&((data)<<11))
#define SI_CTRL_First_bit_sel_src(data)                                              ((0x00000800&(data))>>11)
#define SI_CTRL_get_First_bit_sel(data)                                              ((0x00000800&(data))>>11)
#define SI_CTRL_LSBfirst_shift                                                       (10)
#define SI_CTRL_LSBfirst_mask                                                        (0x00000400)
#define SI_CTRL_LSBfirst(data)                                                       (0x00000400&((data)<<10))
#define SI_CTRL_LSBfirst_src(data)                                                   ((0x00000400&(data))>>10)
#define SI_CTRL_get_LSBfirst(data)                                                   ((0x00000400&(data))>>10)
#define SI_CTRL_RD_LEN_shift                                                         (0)
#define SI_CTRL_RD_LEN_mask                                                          (0x000003FF)
#define SI_CTRL_RD_LEN(data)                                                         (0x000003FF&((data)<<0))
#define SI_CTRL_RD_LEN_src(data)                                                     ((0x000003FF&(data))>>0)
#define SI_CTRL_get_RD_LEN(data)                                                     ((0x000003FF&(data))>>0)


#define SPI_W_FIFO                                                                   0x1801BD0C
#define SPI_W_FIFO_reg_addr                                                          "0xB801BD0C"
#define SPI_W_FIFO_reg                                                               0xB801BD0C
#define set_SPI_W_FIFO_reg(data)   (*((volatile unsigned int*) SPI_W_FIFO_reg)=data)
#define get_SPI_W_FIFO_reg   (*((volatile unsigned int*) SPI_W_FIFO_reg))
#define SPI_W_FIFO_inst_adr                                                          "0x0043"
#define SPI_W_FIFO_inst                                                              0x0043
#define SPI_W_FIFO_W_FIFO_shift                                                      (0)
#define SPI_W_FIFO_W_FIFO_mask                                                       (0xFFFFFFFF)
#define SPI_W_FIFO_W_FIFO(data)                                                      (0xFFFFFFFF&((data)<<0))
#define SPI_W_FIFO_W_FIFO_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define SPI_W_FIFO_get_W_FIFO(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define SPI_R_FIFO                                                                   0x1801BD10
#define SPI_R_FIFO_reg_addr                                                          "0xB801BD10"
#define SPI_R_FIFO_reg                                                               0xB801BD10
#define set_SPI_R_FIFO_reg(data)   (*((volatile unsigned int*) SPI_R_FIFO_reg)=data)
#define get_SPI_R_FIFO_reg   (*((volatile unsigned int*) SPI_R_FIFO_reg))
#define SPI_R_FIFO_inst_adr                                                          "0x0044"
#define SPI_R_FIFO_inst                                                              0x0044
#define SPI_R_FIFO_R_FIFO_shift                                                      (0)
#define SPI_R_FIFO_R_FIFO_mask                                                       (0xFFFFFFFF)
#define SPI_R_FIFO_R_FIFO(data)                                                      (0xFFFFFFFF&((data)<<0))
#define SPI_R_FIFO_R_FIFO_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define SPI_R_FIFO_get_R_FIFO(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define SPI_INSTRUCTION                                                              0x1801BD14
#define SPI_INSTRUCTION_reg_addr                                                     "0xB801BD14"
#define SPI_INSTRUCTION_reg                                                          0xB801BD14
#define set_SPI_INSTRUCTION_reg(data)   (*((volatile unsigned int*) SPI_INSTRUCTION_reg)=data)
#define get_SPI_INSTRUCTION_reg   (*((volatile unsigned int*) SPI_INSTRUCTION_reg))
#define SPI_INSTRUCTION_inst_adr                                                     "0x0045"
#define SPI_INSTRUCTION_inst                                                         0x0045
#define SPI_INSTRUCTION_SPI_INS_shift                                                (0)
#define SPI_INSTRUCTION_SPI_INS_mask                                                 (0xFFFFFFFF)
#define SPI_INSTRUCTION_SPI_INS(data)                                                (0xFFFFFFFF&((data)<<0))
#define SPI_INSTRUCTION_SPI_INS_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define SPI_INSTRUCTION_get_SPI_INS(data)                                            ((0xFFFFFFFF&(data))>>0)


#define SPI_ADDR                                                                     0x1801BD18
#define SPI_ADDR_reg_addr                                                            "0xB801BD18"
#define SPI_ADDR_reg                                                                 0xB801BD18
#define set_SPI_ADDR_reg(data)   (*((volatile unsigned int*) SPI_ADDR_reg)=data)
#define get_SPI_ADDR_reg   (*((volatile unsigned int*) SPI_ADDR_reg))
#define SPI_ADDR_inst_adr                                                            "0x0046"
#define SPI_ADDR_inst                                                                0x0046
#define SPI_ADDR_SPI_ADDR_shift                                                      (0)
#define SPI_ADDR_SPI_ADDR_mask                                                       (0xFFFFFFFF)
#define SPI_ADDR_SPI_ADDR(data)                                                      (0xFFFFFFFF&((data)<<0))
#define SPI_ADDR_SPI_ADDR_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define SPI_ADDR_get_SPI_ADDR(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define CE_TIMING_CTRL                                                               0x1801BD1C
#define CE_TIMING_CTRL_reg_addr                                                      "0xB801BD1C"
#define CE_TIMING_CTRL_reg                                                           0xB801BD1C
#define set_CE_TIMING_CTRL_reg(data)   (*((volatile unsigned int*) CE_TIMING_CTRL_reg)=data)
#define get_CE_TIMING_CTRL_reg   (*((volatile unsigned int*) CE_TIMING_CTRL_reg))
#define CE_TIMING_CTRL_inst_adr                                                      "0x0047"
#define CE_TIMING_CTRL_inst                                                          0x0047
#define CE_TIMING_CTRL_Tcs_high_shift                                                (16)
#define CE_TIMING_CTRL_Tcs_high_mask                                                 (0x00FF0000)
#define CE_TIMING_CTRL_Tcs_high(data)                                                (0x00FF0000&((data)<<16))
#define CE_TIMING_CTRL_Tcs_high_src(data)                                            ((0x00FF0000&(data))>>16)
#define CE_TIMING_CTRL_get_Tcs_high(data)                                            ((0x00FF0000&(data))>>16)
#define CE_TIMING_CTRL_Tcs_end_shift                                                 (8)
#define CE_TIMING_CTRL_Tcs_end_mask                                                  (0x0000FF00)
#define CE_TIMING_CTRL_Tcs_end(data)                                                 (0x0000FF00&((data)<<8))
#define CE_TIMING_CTRL_Tcs_end_src(data)                                             ((0x0000FF00&(data))>>8)
#define CE_TIMING_CTRL_get_Tcs_end(data)                                             ((0x0000FF00&(data))>>8)
#define CE_TIMING_CTRL_Tcs_start_shift                                               (0)
#define CE_TIMING_CTRL_Tcs_start_mask                                                (0x000000FF)
#define CE_TIMING_CTRL_Tcs_start(data)                                               (0x000000FF&((data)<<0))
#define CE_TIMING_CTRL_Tcs_start_src(data)                                           ((0x000000FF&(data))>>0)
#define CE_TIMING_CTRL_get_Tcs_start(data)                                           ((0x000000FF&(data))>>0)


#define SPI_CTRL                                                                     0x1801BD20
#define SPI_CTRL_reg_addr                                                            "0xB801BD20"
#define SPI_CTRL_reg                                                                 0xB801BD20
#define set_SPI_CTRL_reg(data)   (*((volatile unsigned int*) SPI_CTRL_reg)=data)
#define get_SPI_CTRL_reg   (*((volatile unsigned int*) SPI_CTRL_reg))
#define SPI_CTRL_inst_adr                                                            "0x0048"
#define SPI_CTRL_inst                                                                0x0048
#define SPI_CTRL_CMD_D_IE_shift                                                      (8)
#define SPI_CTRL_CMD_D_IE_mask                                                       (0x00000100)
#define SPI_CTRL_CMD_D_IE(data)                                                      (0x00000100&((data)<<8))
#define SPI_CTRL_CMD_D_IE_src(data)                                                  ((0x00000100&(data))>>8)
#define SPI_CTRL_get_CMD_D_IE(data)                                                  ((0x00000100&(data))>>8)
#define SPI_CTRL_FIFO_PTR_RST_shift                                                  (1)
#define SPI_CTRL_FIFO_PTR_RST_mask                                                   (0x00000002)
#define SPI_CTRL_FIFO_PTR_RST(data)                                                  (0x00000002&((data)<<1))
#define SPI_CTRL_FIFO_PTR_RST_src(data)                                              ((0x00000002&(data))>>1)
#define SPI_CTRL_get_FIFO_PTR_RST(data)                                              ((0x00000002&(data))>>1)
#define SPI_CTRL_CMD_ACT_shift                                                       (0)
#define SPI_CTRL_CMD_ACT_mask                                                        (0x00000001)
#define SPI_CTRL_CMD_ACT(data)                                                       (0x00000001&((data)<<0))
#define SPI_CTRL_CMD_ACT_src(data)                                                   ((0x00000001&(data))>>0)
#define SPI_CTRL_get_CMD_ACT(data)                                                   ((0x00000001&(data))>>0)


#define SPI_STATUS                                                                   0x1801BD24
#define SPI_STATUS_reg_addr                                                          "0xB801BD24"
#define SPI_STATUS_reg                                                               0xB801BD24
#define set_SPI_STATUS_reg(data)   (*((volatile unsigned int*) SPI_STATUS_reg)=data)
#define get_SPI_STATUS_reg   (*((volatile unsigned int*) SPI_STATUS_reg))
#define SPI_STATUS_inst_adr                                                          "0x0049"
#define SPI_STATUS_inst                                                              0x0049
#define SPI_STATUS_R_FIFO_PTR_shift                                                  (28)
#define SPI_STATUS_R_FIFO_PTR_mask                                                   (0xF0000000)
#define SPI_STATUS_R_FIFO_PTR(data)                                                  (0xF0000000&((data)<<28))
#define SPI_STATUS_R_FIFO_PTR_src(data)                                              ((0xF0000000&(data))>>28)
#define SPI_STATUS_get_R_FIFO_PTR(data)                                              ((0xF0000000&(data))>>28)
#define SPI_STATUS_R_FIFO_EMPTY_shift                                                (25)
#define SPI_STATUS_R_FIFO_EMPTY_mask                                                 (0x02000000)
#define SPI_STATUS_R_FIFO_EMPTY(data)                                                (0x02000000&((data)<<25))
#define SPI_STATUS_R_FIFO_EMPTY_src(data)                                            ((0x02000000&(data))>>25)
#define SPI_STATUS_get_R_FIFO_EMPTY(data)                                            ((0x02000000&(data))>>25)
#define SPI_STATUS_R_FIFO_FULL_shift                                                 (24)
#define SPI_STATUS_R_FIFO_FULL_mask                                                  (0x01000000)
#define SPI_STATUS_R_FIFO_FULL(data)                                                 (0x01000000&((data)<<24))
#define SPI_STATUS_R_FIFO_FULL_src(data)                                             ((0x01000000&(data))>>24)
#define SPI_STATUS_get_R_FIFO_FULL(data)                                             ((0x01000000&(data))>>24)
#define SPI_STATUS_W_FIFO_PTR_shift                                                  (20)
#define SPI_STATUS_W_FIFO_PTR_mask                                                   (0x00F00000)
#define SPI_STATUS_W_FIFO_PTR(data)                                                  (0x00F00000&((data)<<20))
#define SPI_STATUS_W_FIFO_PTR_src(data)                                              ((0x00F00000&(data))>>20)
#define SPI_STATUS_get_W_FIFO_PTR(data)                                              ((0x00F00000&(data))>>20)
#define SPI_STATUS_W_FIFO_EMPTY_shift                                                (17)
#define SPI_STATUS_W_FIFO_EMPTY_mask                                                 (0x00020000)
#define SPI_STATUS_W_FIFO_EMPTY(data)                                                (0x00020000&((data)<<17))
#define SPI_STATUS_W_FIFO_EMPTY_src(data)                                            ((0x00020000&(data))>>17)
#define SPI_STATUS_get_W_FIFO_EMPTY(data)                                            ((0x00020000&(data))>>17)
#define SPI_STATUS_W_FIFO_FULL_shift                                                 (16)
#define SPI_STATUS_W_FIFO_FULL_mask                                                  (0x00010000)
#define SPI_STATUS_W_FIFO_FULL(data)                                                 (0x00010000&((data)<<16))
#define SPI_STATUS_W_FIFO_FULL_src(data)                                             ((0x00010000&(data))>>16)
#define SPI_STATUS_get_W_FIFO_FULL(data)                                             ((0x00010000&(data))>>16)
#define SPI_STATUS_SFC_BUSY_shift                                                    (1)
#define SPI_STATUS_SFC_BUSY_mask                                                     (0x00000002)
#define SPI_STATUS_SFC_BUSY(data)                                                    (0x00000002&((data)<<1))
#define SPI_STATUS_SFC_BUSY_src(data)                                                ((0x00000002&(data))>>1)
#define SPI_STATUS_get_SFC_BUSY(data)                                                ((0x00000002&(data))>>1)
#define SPI_STATUS_CMD_DONE_shift                                                    (0)
#define SPI_STATUS_CMD_DONE_mask                                                     (0x00000001)
#define SPI_STATUS_CMD_DONE(data)                                                    (0x00000001&((data)<<0))
#define SPI_STATUS_CMD_DONE_src(data)                                                ((0x00000001&(data))>>0)
#define SPI_STATUS_get_CMD_DONE(data)                                                ((0x00000001&(data))>>0)


#endif
