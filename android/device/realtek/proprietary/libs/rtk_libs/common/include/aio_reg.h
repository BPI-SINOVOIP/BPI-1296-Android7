/**************************************************************
// Spec Version                  : 0.3
// Parser Version                : DVR_Parser_5.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name_reg
// Firmware Header Generate Date : 2010/4/23
***************************************************************/


#ifndef _AIO_REG_H_INCLUDED_
#define _AIO_REG_H_INCLUDED_
#ifdef  _AIO_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     edgeneg:1;
unsigned int     wsdelay:1;
unsigned int     slave:1;
unsigned int     bit:3;
unsigned int     reserved_1:9;
unsigned int     rmute0:1;
unsigned int     lmute0:1;
unsigned int     en0:1;
}AIO_I_I2S_GCTL;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     wsperiod:6;
unsigned int     protocol:2;
unsigned int     wsdelay:1;
unsigned int     slave:1;
unsigned int     bit:3;
unsigned int     reserved_1:12;
unsigned int     rmute0:1;
unsigned int     lmute0:1;
unsigned int     en0:1;
}AIO_O_I2S_GCTL;

typedef struct 
{
unsigned int     cs1:32;
}AIO_O_PCM_CS1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     cs2:8;
}AIO_O_PCM_CS2;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     bit:3;
unsigned int     wait_en:1;
unsigned int     valid:1;
unsigned int     format:1;
unsigned int     rmute:1;
unsigned int     lmute:1;
unsigned int     en:1;
}AIO_O_SPF_GCTL;

typedef struct 
{
unsigned int     cs1:32;
}AIO_O_RAW_CS1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     cs2:8;
}AIO_O_RAW_CS2;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     realgo:1;
unsigned int     spcnt:7;
unsigned int     fcnt:8;
}AIO_O_SPF_FCNT;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     clk_sel:3;
unsigned int     pcmsync:1;
unsigned int     pkt:1;
unsigned int     sample_flat:4;
unsigned int     rmute3:1;
unsigned int     lmute3:1;
unsigned int     rmute2:1;
unsigned int     lmute2:1;
unsigned int     rmute1:1;
unsigned int     lmute1:1;
unsigned int     8ch:1;
unsigned int     bit:3;
unsigned int     wait_en:1;
unsigned int     reserved_1:1;
unsigned int     rmute:1;
unsigned int     lmute:1;
unsigned int     switch_wait_en:1;
unsigned int     sel:2;
unsigned int     en:1;
}AIO_O_HDMI_GCTL;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     realgo:1;
unsigned int     fcnt:8;
}AIO_O_HDMI_FCNT;

typedef struct 
{
unsigned int     cs1:32;
}AIO_O_HDMI_CS1;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     cs2:8;
}AIO_O_HDMI_CS2;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     rgain:7;
unsigned int     lgain:7;
unsigned int     bit:3;
unsigned int     hpfen:1;
unsigned int     rmute:1;
unsigned int     lmute:1;
unsigned int     en:1;
}AIO_O_ACDAC_GCTL;

typedef struct 
{
unsigned int     write_en1:1;
unsigned int     pow_bias:1;
unsigned int     reserved_0:2;
unsigned int     write_en3:1;
unsigned int     pow_dac:1;
unsigned int     write_en4:1;
unsigned int     rstb:1;
unsigned int     reserved_1:3;
unsigned int     write_en6:1;
unsigned int     unmute_l:1;
unsigned int     write_en7:1;
unsigned int     unmute_r:1;
unsigned int     write_en8:1;
unsigned int     ckxen:1;
unsigned int     write_en9:1;
unsigned int     ckxsel:1;
unsigned int     write_en10:1;
unsigned int     cks_sel:1;
unsigned int     write_en11:1;
unsigned int     v2i_sel:1;
unsigned int     write_en12:1;
unsigned int     imp:1;
unsigned int     write_en13:1;
unsigned int     disc:1;
unsigned int     write_en14:1;
unsigned int     en_amp:1;
unsigned int     write_en15:1;
unsigned int     vrp_sel:2;
}AIO_O_ACANA_GCTL1;

typedef struct 
{
unsigned int     reserved_0:13;
unsigned int     write_en1:1;
unsigned int     vcmh_sel:1;
unsigned int     write_en2:1;
unsigned int     ct_bias:16;
}AIO_O_ACANA_GCTL2;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     thd:5;
unsigned int     en:1;
}AIO_I_FIFOCTL0;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     en10:1;
unsigned int     en9:1;
unsigned int     en8:1;
unsigned int     en7:1;
unsigned int     en6:1;
unsigned int     emptystopen:1;
unsigned int     thd:5;
unsigned int     en5:1;
unsigned int     en4:1;
unsigned int     en3:1;
unsigned int     en2:1;
unsigned int     en1:1;
unsigned int     en0:1;
}AIO_O_FIFOCTL;

typedef struct 
{
unsigned int     data:32;
}AIO_O_I2S_LDATA0;

typedef struct 
{
unsigned int     data:32;
}AIO_O_I2S_RDATA0;

typedef struct 
{
unsigned int     data:32;
}AIO_O_SPF_LDATA;

typedef struct 
{
unsigned int     data:32;
}AIO_O_SPF_RDATA;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     empty11:1;
unsigned int     empty10:1;
unsigned int     empty9:1;
unsigned int     empty8:1;
unsigned int     empty7:1;
unsigned int     empty6:1;
unsigned int     empty5:1;
unsigned int     empty4:1;
unsigned int     empty3:1;
unsigned int     empty2:1;
unsigned int     empty1:1;
unsigned int     empty0:1;
}AIO_O_FIFO_STATUS;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_I_SA;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_I_EA;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_I_RP;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_I_WP;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_O_SA;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_O_EA;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_O_RP;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     addr:28;
unsigned int     reserved_1:3;
}AIO_O_WP;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     dmago:1;
}AIO_O_DMA_STATUS;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     go0:1;
unsigned int     reserved_1:1;
}AIO_I_GO;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     goacdac:1;
unsigned int     gohdmi:1;
unsigned int     goi2s:1;
unsigned int     gospf:1;
}AIO_O_GO;

typedef struct 
{
unsigned int     reserved_0:31;
unsigned int     pause0:1;
}AIO_I_PAUSE;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     pause2:1;
unsigned int     pause1:1;
unsigned int     pause0:1;
}AIO_O_PAUSE;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     thd:28;
unsigned int     reserved_1:3;
}AIO_I_I2S_DDRTHD;

typedef struct 
{
unsigned int     count:32;
}AIO_I_I2S_TIMCNT;

typedef struct 
{
unsigned int     thd:32;
}AIO_I_I2S_TIMTHD;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     timi2sinte:1;
unsigned int     reserved_1:3;
unsigned int     thdinte0:1;
unsigned int     reserved_2:10;
unsigned int     fifointe1:1;
unsigned int     fifointe0:1;
unsigned int     reserved_3:6;
unsigned int     draminte1:1;
unsigned int     draminte0:1;
unsigned int     write_data:1;
}AIO_I_INTE;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     timi2sints:1;
unsigned int     reserved_1:3;
unsigned int     thdints0:1;
unsigned int     reserved_2:10;
unsigned int     fifoints1:1;
unsigned int     fifoints0:1;
unsigned int     reserved_3:6;
unsigned int     dramints1:1;
unsigned int     dramints0:1;
unsigned int     write_data:1;
}AIO_I_INTS;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     thd:28;
unsigned int     reserved_1:3;
}AIO_O_PCM_DDRTHD;

typedef struct 
{
unsigned int     count:32;
}AIO_O_PCM_TIMCNT;

typedef struct 
{
unsigned int     thd:32;
}AIO_O_PCM_TIMTHD;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     thd:28;
unsigned int     reserved_1:3;
}AIO_O_RAW_DDRTHD;

typedef struct 
{
unsigned int     count:32;
}AIO_O_RAW_TIMCNT;

typedef struct 
{
unsigned int     thd:32;
}AIO_O_RAW_TIMTHD;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     thd:28;
unsigned int     reserved_1:3;
}AIO_O_HDMI_DDRTHD;

typedef struct 
{
unsigned int     count:32;
}AIO_O_HDMI_TIMCNT;

typedef struct 
{
unsigned int     thd:32;
}AIO_O_HDMI_TIMTHD;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     draminte11:1;
unsigned int     draminte10:1;
unsigned int     draminte9:1;
unsigned int     draminte8:1;
unsigned int     draminte7:1;
unsigned int     draminte6:1;
unsigned int     draminte5:1;
unsigned int     draminte4:1;
unsigned int     fifointe11:1;
unsigned int     fifointe10:1;
unsigned int     fifointe9:1;
unsigned int     fifointe8:1;
unsigned int     fifointe7:1;
unsigned int     fifointe6:1;
unsigned int     fifointe5:1;
unsigned int     fifointe4:1;
unsigned int     pcmtiminte:1;
unsigned int     rawtiminte:1;
unsigned int     hdmitiminte:1;
unsigned int     pcmthdinte:1;
unsigned int     rawthdinte:1;
unsigned int     hdmithdinte:1;
unsigned int     draminte3:1;
unsigned int     draminte2:1;
unsigned int     draminte1:1;
unsigned int     draminte0:1;
unsigned int     fifointe3:1;
unsigned int     fifointe2:1;
unsigned int     fifointe1:1;
unsigned int     fifointe0:1;
unsigned int     write_data:1;
}AIO_O_INTE;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     dramints11:1;
unsigned int     dramints10:1;
unsigned int     dramints9:1;
unsigned int     dramints8:1;
unsigned int     dramints7:1;
unsigned int     dramints6:1;
unsigned int     dramints5:1;
unsigned int     dramints4:1;
unsigned int     fifoints11:1;
unsigned int     fifoints10:1;
unsigned int     fifoints9:1;
unsigned int     fifoints8:1;
unsigned int     fifoints7:1;
unsigned int     fifoints6:1;
unsigned int     fifoints5:1;
unsigned int     fifoints4:1;
unsigned int     pcmtimints:1;
unsigned int     rawtimints:1;
unsigned int     hdmitimints:1;
unsigned int     pcmthdints:1;
unsigned int     rawthdints:1;
unsigned int     hdmithdints:1;
unsigned int     dramints3:1;
unsigned int     dramints2:1;
unsigned int     dramints1:1;
unsigned int     dramints0:1;
unsigned int     fifoints3:1;
unsigned int     fifoints2:1;
unsigned int     fifoints1:1;
unsigned int     fifoints0:1;
unsigned int     write_data:1;
}AIO_O_INTS;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     loopbacken:1;
unsigned int     codec_tmode:1;
unsigned int     dbgen:1;
unsigned int     sel0:3;
unsigned int     sel1:3;
}AIO_DBG;

typedef struct 
{
unsigned int     spare:32;
}AIO_SPARE;

#endif

#define AIO_I_I2S_GCTL                                                               0x18006000
#define AIO_I_I2S_GCTL_reg_addr                                                      "0xB8006000"
#define AIO_I_I2S_GCTL_reg                                                           0xB8006000
#define set_AIO_I_I2S_GCTL_reg(data)   (*((volatile unsigned int*) AIO_I_I2S_GCTL_reg)=data)
#define get_AIO_I_I2S_GCTL_reg   (*((volatile unsigned int*) AIO_I_I2S_GCTL_reg))
#define AIO_I_I2S_GCTL_inst_adr                                                      "0x0000"
#define AIO_I_I2S_GCTL_inst                                                          0x0000
#define AIO_I_I2S_GCTL_edgeneg_shift                                                 (17)
#define AIO_I_I2S_GCTL_edgeneg_mask                                                  (0x00020000)
#define AIO_I_I2S_GCTL_edgeneg(data)                                                 (0x00020000&((data)<<17))
#define AIO_I_I2S_GCTL_edgeneg_src(data)                                             ((0x00020000&(data))>>17)
#define AIO_I_I2S_GCTL_get_edgeneg(data)                                             ((0x00020000&(data))>>17)
#define AIO_I_I2S_GCTL_wsdelay_shift                                                 (16)
#define AIO_I_I2S_GCTL_wsdelay_mask                                                  (0x00010000)
#define AIO_I_I2S_GCTL_wsdelay(data)                                                 (0x00010000&((data)<<16))
#define AIO_I_I2S_GCTL_wsdelay_src(data)                                             ((0x00010000&(data))>>16)
#define AIO_I_I2S_GCTL_get_wsdelay(data)                                             ((0x00010000&(data))>>16)
#define AIO_I_I2S_GCTL_slave_shift                                                   (15)
#define AIO_I_I2S_GCTL_slave_mask                                                    (0x00008000)
#define AIO_I_I2S_GCTL_slave(data)                                                   (0x00008000&((data)<<15))
#define AIO_I_I2S_GCTL_slave_src(data)                                               ((0x00008000&(data))>>15)
#define AIO_I_I2S_GCTL_get_slave(data)                                               ((0x00008000&(data))>>15)
#define AIO_I_I2S_GCTL_bit_shift                                                     (12)
#define AIO_I_I2S_GCTL_bit_mask                                                      (0x00007000)
#define AIO_I_I2S_GCTL_bit(data)                                                     (0x00007000&((data)<<12))
#define AIO_I_I2S_GCTL_bit_src(data)                                                 ((0x00007000&(data))>>12)
#define AIO_I_I2S_GCTL_get_bit(data)                                                 ((0x00007000&(data))>>12)
#define AIO_I_I2S_GCTL_rmute0_shift                                                  (2)
#define AIO_I_I2S_GCTL_rmute0_mask                                                   (0x00000004)
#define AIO_I_I2S_GCTL_rmute0(data)                                                  (0x00000004&((data)<<2))
#define AIO_I_I2S_GCTL_rmute0_src(data)                                              ((0x00000004&(data))>>2)
#define AIO_I_I2S_GCTL_get_rmute0(data)                                              ((0x00000004&(data))>>2)
#define AIO_I_I2S_GCTL_lmute0_shift                                                  (1)
#define AIO_I_I2S_GCTL_lmute0_mask                                                   (0x00000002)
#define AIO_I_I2S_GCTL_lmute0(data)                                                  (0x00000002&((data)<<1))
#define AIO_I_I2S_GCTL_lmute0_src(data)                                              ((0x00000002&(data))>>1)
#define AIO_I_I2S_GCTL_get_lmute0(data)                                              ((0x00000002&(data))>>1)
#define AIO_I_I2S_GCTL_en0_shift                                                     (0)
#define AIO_I_I2S_GCTL_en0_mask                                                      (0x00000001)
#define AIO_I_I2S_GCTL_en0(data)                                                     (0x00000001&((data)<<0))
#define AIO_I_I2S_GCTL_en0_src(data)                                                 ((0x00000001&(data))>>0)
#define AIO_I_I2S_GCTL_get_en0(data)                                                 ((0x00000001&(data))>>0)


#define AIO_O_I2S_GCTL                                                               0x18006004
#define AIO_O_I2S_GCTL_reg_addr                                                      "0xB8006004"
#define AIO_O_I2S_GCTL_reg                                                           0xB8006004
#define set_AIO_O_I2S_GCTL_reg(data)   (*((volatile unsigned int*) AIO_O_I2S_GCTL_reg)=data)
#define get_AIO_O_I2S_GCTL_reg   (*((volatile unsigned int*) AIO_O_I2S_GCTL_reg))
#define AIO_O_I2S_GCTL_inst_adr                                                      "0x0001"
#define AIO_O_I2S_GCTL_inst                                                          0x0001
#define AIO_O_I2S_GCTL_wsperiod_shift                                                (22)
#define AIO_O_I2S_GCTL_wsperiod_mask                                                 (0x0FC00000)
#define AIO_O_I2S_GCTL_wsperiod(data)                                                (0x0FC00000&((data)<<22))
#define AIO_O_I2S_GCTL_wsperiod_src(data)                                            ((0x0FC00000&(data))>>22)
#define AIO_O_I2S_GCTL_get_wsperiod(data)                                            ((0x0FC00000&(data))>>22)
#define AIO_O_I2S_GCTL_protocol_shift                                                (20)
#define AIO_O_I2S_GCTL_protocol_mask                                                 (0x00300000)
#define AIO_O_I2S_GCTL_protocol(data)                                                (0x00300000&((data)<<20))
#define AIO_O_I2S_GCTL_protocol_src(data)                                            ((0x00300000&(data))>>20)
#define AIO_O_I2S_GCTL_get_protocol(data)                                            ((0x00300000&(data))>>20)
#define AIO_O_I2S_GCTL_wsdelay_shift                                                 (19)
#define AIO_O_I2S_GCTL_wsdelay_mask                                                  (0x00080000)
#define AIO_O_I2S_GCTL_wsdelay(data)                                                 (0x00080000&((data)<<19))
#define AIO_O_I2S_GCTL_wsdelay_src(data)                                             ((0x00080000&(data))>>19)
#define AIO_O_I2S_GCTL_get_wsdelay(data)                                             ((0x00080000&(data))>>19)
#define AIO_O_I2S_GCTL_slave_shift                                                   (18)
#define AIO_O_I2S_GCTL_slave_mask                                                    (0x00040000)
#define AIO_O_I2S_GCTL_slave(data)                                                   (0x00040000&((data)<<18))
#define AIO_O_I2S_GCTL_slave_src(data)                                               ((0x00040000&(data))>>18)
#define AIO_O_I2S_GCTL_get_slave(data)                                               ((0x00040000&(data))>>18)
#define AIO_O_I2S_GCTL_bit_shift                                                     (15)
#define AIO_O_I2S_GCTL_bit_mask                                                      (0x00038000)
#define AIO_O_I2S_GCTL_bit(data)                                                     (0x00038000&((data)<<15))
#define AIO_O_I2S_GCTL_bit_src(data)                                                 ((0x00038000&(data))>>15)
#define AIO_O_I2S_GCTL_get_bit(data)                                                 ((0x00038000&(data))>>15)
#define AIO_O_I2S_GCTL_rmute0_shift                                                  (2)
#define AIO_O_I2S_GCTL_rmute0_mask                                                   (0x00000004)
#define AIO_O_I2S_GCTL_rmute0(data)                                                  (0x00000004&((data)<<2))
#define AIO_O_I2S_GCTL_rmute0_src(data)                                              ((0x00000004&(data))>>2)
#define AIO_O_I2S_GCTL_get_rmute0(data)                                              ((0x00000004&(data))>>2)
#define AIO_O_I2S_GCTL_lmute0_shift                                                  (1)
#define AIO_O_I2S_GCTL_lmute0_mask                                                   (0x00000002)
#define AIO_O_I2S_GCTL_lmute0(data)                                                  (0x00000002&((data)<<1))
#define AIO_O_I2S_GCTL_lmute0_src(data)                                              ((0x00000002&(data))>>1)
#define AIO_O_I2S_GCTL_get_lmute0(data)                                              ((0x00000002&(data))>>1)
#define AIO_O_I2S_GCTL_en0_shift                                                     (0)
#define AIO_O_I2S_GCTL_en0_mask                                                      (0x00000001)
#define AIO_O_I2S_GCTL_en0(data)                                                     (0x00000001&((data)<<0))
#define AIO_O_I2S_GCTL_en0_src(data)                                                 ((0x00000001&(data))>>0)
#define AIO_O_I2S_GCTL_get_en0(data)                                                 ((0x00000001&(data))>>0)


#define AIO_O_PCM_CS1                                                                0x1800600c
#define AIO_O_PCM_CS1_reg_addr                                                       "0xB800600C"
#define AIO_O_PCM_CS1_reg                                                            0xB800600C
#define set_AIO_O_PCM_CS1_reg(data)   (*((volatile unsigned int*) AIO_O_PCM_CS1_reg)=data)
#define get_AIO_O_PCM_CS1_reg   (*((volatile unsigned int*) AIO_O_PCM_CS1_reg))
#define AIO_O_PCM_CS1_inst_adr                                                       "0x0003"
#define AIO_O_PCM_CS1_inst                                                           0x0003
#define AIO_O_PCM_CS1_cs1_shift                                                      (0)
#define AIO_O_PCM_CS1_cs1_mask                                                       (0xFFFFFFFF)
#define AIO_O_PCM_CS1_cs1(data)                                                      (0xFFFFFFFF&((data)<<0))
#define AIO_O_PCM_CS1_cs1_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define AIO_O_PCM_CS1_get_cs1(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define AIO_O_PCM_CS2                                                                0x18006010
#define AIO_O_PCM_CS2_reg_addr                                                       "0xB8006010"
#define AIO_O_PCM_CS2_reg                                                            0xB8006010
#define set_AIO_O_PCM_CS2_reg(data)   (*((volatile unsigned int*) AIO_O_PCM_CS2_reg)=data)
#define get_AIO_O_PCM_CS2_reg   (*((volatile unsigned int*) AIO_O_PCM_CS2_reg))
#define AIO_O_PCM_CS2_inst_adr                                                       "0x0004"
#define AIO_O_PCM_CS2_inst                                                           0x0004
#define AIO_O_PCM_CS2_cs2_shift                                                      (0)
#define AIO_O_PCM_CS2_cs2_mask                                                       (0x000000FF)
#define AIO_O_PCM_CS2_cs2(data)                                                      (0x000000FF&((data)<<0))
#define AIO_O_PCM_CS2_cs2_src(data)                                                  ((0x000000FF&(data))>>0)
#define AIO_O_PCM_CS2_get_cs2(data)                                                  ((0x000000FF&(data))>>0)


#define AIO_O_SPF_GCTL                                                               0x18006050
#define AIO_O_SPF_GCTL_reg_addr                                                      "0xB8006050"
#define AIO_O_SPF_GCTL_reg                                                           0xB8006050
#define set_AIO_O_SPF_GCTL_reg(data)   (*((volatile unsigned int*) AIO_O_SPF_GCTL_reg)=data)
#define get_AIO_O_SPF_GCTL_reg   (*((volatile unsigned int*) AIO_O_SPF_GCTL_reg))
#define AIO_O_SPF_GCTL_inst_adr                                                      "0x0014"
#define AIO_O_SPF_GCTL_inst                                                          0x0014
#define AIO_O_SPF_GCTL_bit_shift                                                     (6)
#define AIO_O_SPF_GCTL_bit_mask                                                      (0x000001C0)
#define AIO_O_SPF_GCTL_bit(data)                                                     (0x000001C0&((data)<<6))
#define AIO_O_SPF_GCTL_bit_src(data)                                                 ((0x000001C0&(data))>>6)
#define AIO_O_SPF_GCTL_get_bit(data)                                                 ((0x000001C0&(data))>>6)
#define AIO_O_SPF_GCTL_wait_en_shift                                                 (5)
#define AIO_O_SPF_GCTL_wait_en_mask                                                  (0x00000020)
#define AIO_O_SPF_GCTL_wait_en(data)                                                 (0x00000020&((data)<<5))
#define AIO_O_SPF_GCTL_wait_en_src(data)                                             ((0x00000020&(data))>>5)
#define AIO_O_SPF_GCTL_get_wait_en(data)                                             ((0x00000020&(data))>>5)
#define AIO_O_SPF_GCTL_valid_shift                                                   (4)
#define AIO_O_SPF_GCTL_valid_mask                                                    (0x00000010)
#define AIO_O_SPF_GCTL_valid(data)                                                   (0x00000010&((data)<<4))
#define AIO_O_SPF_GCTL_valid_src(data)                                               ((0x00000010&(data))>>4)
#define AIO_O_SPF_GCTL_get_valid(data)                                               ((0x00000010&(data))>>4)
#define AIO_O_SPF_GCTL_format_shift                                                  (3)
#define AIO_O_SPF_GCTL_format_mask                                                   (0x00000008)
#define AIO_O_SPF_GCTL_format(data)                                                  (0x00000008&((data)<<3))
#define AIO_O_SPF_GCTL_format_src(data)                                              ((0x00000008&(data))>>3)
#define AIO_O_SPF_GCTL_get_format(data)                                              ((0x00000008&(data))>>3)
#define AIO_O_SPF_GCTL_rmute_shift                                                   (2)
#define AIO_O_SPF_GCTL_rmute_mask                                                    (0x00000004)
#define AIO_O_SPF_GCTL_rmute(data)                                                   (0x00000004&((data)<<2))
#define AIO_O_SPF_GCTL_rmute_src(data)                                               ((0x00000004&(data))>>2)
#define AIO_O_SPF_GCTL_get_rmute(data)                                               ((0x00000004&(data))>>2)
#define AIO_O_SPF_GCTL_lmute_shift                                                   (1)
#define AIO_O_SPF_GCTL_lmute_mask                                                    (0x00000002)
#define AIO_O_SPF_GCTL_lmute(data)                                                   (0x00000002&((data)<<1))
#define AIO_O_SPF_GCTL_lmute_src(data)                                               ((0x00000002&(data))>>1)
#define AIO_O_SPF_GCTL_get_lmute(data)                                               ((0x00000002&(data))>>1)
#define AIO_O_SPF_GCTL_en_shift                                                      (0)
#define AIO_O_SPF_GCTL_en_mask                                                       (0x00000001)
#define AIO_O_SPF_GCTL_en(data)                                                      (0x00000001&((data)<<0))
#define AIO_O_SPF_GCTL_en_src(data)                                                  ((0x00000001&(data))>>0)
#define AIO_O_SPF_GCTL_get_en(data)                                                  ((0x00000001&(data))>>0)


#define AIO_O_RAW_CS1                                                                0x18006058
#define AIO_O_RAW_CS1_reg_addr                                                       "0xB8006058"
#define AIO_O_RAW_CS1_reg                                                            0xB8006058
#define set_AIO_O_RAW_CS1_reg(data)   (*((volatile unsigned int*) AIO_O_RAW_CS1_reg)=data)
#define get_AIO_O_RAW_CS1_reg   (*((volatile unsigned int*) AIO_O_RAW_CS1_reg))
#define AIO_O_RAW_CS1_inst_adr                                                       "0x0016"
#define AIO_O_RAW_CS1_inst                                                           0x0016
#define AIO_O_RAW_CS1_cs1_shift                                                      (0)
#define AIO_O_RAW_CS1_cs1_mask                                                       (0xFFFFFFFF)
#define AIO_O_RAW_CS1_cs1(data)                                                      (0xFFFFFFFF&((data)<<0))
#define AIO_O_RAW_CS1_cs1_src(data)                                                  ((0xFFFFFFFF&(data))>>0)
#define AIO_O_RAW_CS1_get_cs1(data)                                                  ((0xFFFFFFFF&(data))>>0)


#define AIO_O_RAW_CS2                                                                0x1800605c
#define AIO_O_RAW_CS2_reg_addr                                                       "0xB800605C"
#define AIO_O_RAW_CS2_reg                                                            0xB800605C
#define set_AIO_O_RAW_CS2_reg(data)   (*((volatile unsigned int*) AIO_O_RAW_CS2_reg)=data)
#define get_AIO_O_RAW_CS2_reg   (*((volatile unsigned int*) AIO_O_RAW_CS2_reg))
#define AIO_O_RAW_CS2_inst_adr                                                       "0x0017"
#define AIO_O_RAW_CS2_inst                                                           0x0017
#define AIO_O_RAW_CS2_cs2_shift                                                      (0)
#define AIO_O_RAW_CS2_cs2_mask                                                       (0x000000FF)
#define AIO_O_RAW_CS2_cs2(data)                                                      (0x000000FF&((data)<<0))
#define AIO_O_RAW_CS2_cs2_src(data)                                                  ((0x000000FF&(data))>>0)
#define AIO_O_RAW_CS2_get_cs2(data)                                                  ((0x000000FF&(data))>>0)


#define AIO_O_SPF_FCNT                                                               0x18006060
#define AIO_O_SPF_FCNT_reg_addr                                                      "0xB8006060"
#define AIO_O_SPF_FCNT_reg                                                           0xB8006060
#define set_AIO_O_SPF_FCNT_reg(data)   (*((volatile unsigned int*) AIO_O_SPF_FCNT_reg)=data)
#define get_AIO_O_SPF_FCNT_reg   (*((volatile unsigned int*) AIO_O_SPF_FCNT_reg))
#define AIO_O_SPF_FCNT_inst_adr                                                      "0x0018"
#define AIO_O_SPF_FCNT_inst                                                          0x0018
#define AIO_O_SPF_FCNT_realgo_shift                                                  (15)
#define AIO_O_SPF_FCNT_realgo_mask                                                   (0x00008000)
#define AIO_O_SPF_FCNT_realgo(data)                                                  (0x00008000&((data)<<15))
#define AIO_O_SPF_FCNT_realgo_src(data)                                              ((0x00008000&(data))>>15)
#define AIO_O_SPF_FCNT_get_realgo(data)                                              ((0x00008000&(data))>>15)
#define AIO_O_SPF_FCNT_spcnt_shift                                                   (8)
#define AIO_O_SPF_FCNT_spcnt_mask                                                    (0x00007F00)
#define AIO_O_SPF_FCNT_spcnt(data)                                                   (0x00007F00&((data)<<8))
#define AIO_O_SPF_FCNT_spcnt_src(data)                                               ((0x00007F00&(data))>>8)
#define AIO_O_SPF_FCNT_get_spcnt(data)                                               ((0x00007F00&(data))>>8)
#define AIO_O_SPF_FCNT_fcnt_shift                                                    (0)
#define AIO_O_SPF_FCNT_fcnt_mask                                                     (0x000000FF)
#define AIO_O_SPF_FCNT_fcnt(data)                                                    (0x000000FF&((data)<<0))
#define AIO_O_SPF_FCNT_fcnt_src(data)                                                ((0x000000FF&(data))>>0)
#define AIO_O_SPF_FCNT_get_fcnt(data)                                                ((0x000000FF&(data))>>0)


#define AIO_O_HDMI_GCTL                                                              0x18006070
#define AIO_O_HDMI_GCTL_reg_addr                                                     "0xB8006070"
#define AIO_O_HDMI_GCTL_reg                                                          0xB8006070
#define set_AIO_O_HDMI_GCTL_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_GCTL_reg)=data)
#define get_AIO_O_HDMI_GCTL_reg   (*((volatile unsigned int*) AIO_O_HDMI_GCTL_reg))
#define AIO_O_HDMI_GCTL_inst_adr                                                     "0x001C"
#define AIO_O_HDMI_GCTL_inst                                                         0x001C
#define AIO_O_HDMI_GCTL_clk_sel_shift                                                (24)
#define AIO_O_HDMI_GCTL_clk_sel_mask                                                 (0x07000000)
#define AIO_O_HDMI_GCTL_clk_sel(data)                                                (0x07000000&((data)<<24))
#define AIO_O_HDMI_GCTL_clk_sel_src(data)                                            ((0x07000000&(data))>>24)
#define AIO_O_HDMI_GCTL_get_clk_sel(data)                                            ((0x07000000&(data))>>24)
#define AIO_O_HDMI_GCTL_pcmsync_shift                                                (23)
#define AIO_O_HDMI_GCTL_pcmsync_mask                                                 (0x00800000)
#define AIO_O_HDMI_GCTL_pcmsync(data)                                                (0x00800000&((data)<<23))
#define AIO_O_HDMI_GCTL_pcmsync_src(data)                                            ((0x00800000&(data))>>23)
#define AIO_O_HDMI_GCTL_get_pcmsync(data)                                            ((0x00800000&(data))>>23)
#define AIO_O_HDMI_GCTL_pkt_shift                                                    (22)
#define AIO_O_HDMI_GCTL_pkt_mask                                                     (0x00400000)
#define AIO_O_HDMI_GCTL_pkt(data)                                                    (0x00400000&((data)<<22))
#define AIO_O_HDMI_GCTL_pkt_src(data)                                                ((0x00400000&(data))>>22)
#define AIO_O_HDMI_GCTL_get_pkt(data)                                                ((0x00400000&(data))>>22)
#define AIO_O_HDMI_GCTL_sample_flat_shift                                            (18)
#define AIO_O_HDMI_GCTL_sample_flat_mask                                             (0x003C0000)
#define AIO_O_HDMI_GCTL_sample_flat(data)                                            (0x003C0000&((data)<<18))
#define AIO_O_HDMI_GCTL_sample_flat_src(data)                                        ((0x003C0000&(data))>>18)
#define AIO_O_HDMI_GCTL_get_sample_flat(data)                                        ((0x003C0000&(data))>>18)
#define AIO_O_HDMI_GCTL_rmute3_shift                                                 (17)
#define AIO_O_HDMI_GCTL_rmute3_mask                                                  (0x00020000)
#define AIO_O_HDMI_GCTL_rmute3(data)                                                 (0x00020000&((data)<<17))
#define AIO_O_HDMI_GCTL_rmute3_src(data)                                             ((0x00020000&(data))>>17)
#define AIO_O_HDMI_GCTL_get_rmute3(data)                                             ((0x00020000&(data))>>17)
#define AIO_O_HDMI_GCTL_lmute3_shift                                                 (16)
#define AIO_O_HDMI_GCTL_lmute3_mask                                                  (0x00010000)
#define AIO_O_HDMI_GCTL_lmute3(data)                                                 (0x00010000&((data)<<16))
#define AIO_O_HDMI_GCTL_lmute3_src(data)                                             ((0x00010000&(data))>>16)
#define AIO_O_HDMI_GCTL_get_lmute3(data)                                             ((0x00010000&(data))>>16)
#define AIO_O_HDMI_GCTL_rmute2_shift                                                 (15)
#define AIO_O_HDMI_GCTL_rmute2_mask                                                  (0x00008000)
#define AIO_O_HDMI_GCTL_rmute2(data)                                                 (0x00008000&((data)<<15))
#define AIO_O_HDMI_GCTL_rmute2_src(data)                                             ((0x00008000&(data))>>15)
#define AIO_O_HDMI_GCTL_get_rmute2(data)                                             ((0x00008000&(data))>>15)
#define AIO_O_HDMI_GCTL_lmute2_shift                                                 (14)
#define AIO_O_HDMI_GCTL_lmute2_mask                                                  (0x00004000)
#define AIO_O_HDMI_GCTL_lmute2(data)                                                 (0x00004000&((data)<<14))
#define AIO_O_HDMI_GCTL_lmute2_src(data)                                             ((0x00004000&(data))>>14)
#define AIO_O_HDMI_GCTL_get_lmute2(data)                                             ((0x00004000&(data))>>14)
#define AIO_O_HDMI_GCTL_rmute1_shift                                                 (13)
#define AIO_O_HDMI_GCTL_rmute1_mask                                                  (0x00002000)
#define AIO_O_HDMI_GCTL_rmute1(data)                                                 (0x00002000&((data)<<13))
#define AIO_O_HDMI_GCTL_rmute1_src(data)                                             ((0x00002000&(data))>>13)
#define AIO_O_HDMI_GCTL_get_rmute1(data)                                             ((0x00002000&(data))>>13)
#define AIO_O_HDMI_GCTL_lmute1_shift                                                 (12)
#define AIO_O_HDMI_GCTL_lmute1_mask                                                  (0x00001000)
#define AIO_O_HDMI_GCTL_lmute1(data)                                                 (0x00001000&((data)<<12))
#define AIO_O_HDMI_GCTL_lmute1_src(data)                                             ((0x00001000&(data))>>12)
#define AIO_O_HDMI_GCTL_get_lmute1(data)                                             ((0x00001000&(data))>>12)
#define AIO_O_HDMI_GCTL_8ch_shift                                                    (11)
#define AIO_O_HDMI_GCTL_8ch_mask                                                     (0x00000800)
#define AIO_O_HDMI_GCTL_8ch(data)                                                    (0x00000800&((data)<<11))
#define AIO_O_HDMI_GCTL_8ch_src(data)                                                ((0x00000800&(data))>>11)
#define AIO_O_HDMI_GCTL_get_8ch(data)                                                ((0x00000800&(data))>>11)
#define AIO_O_HDMI_GCTL_bit_shift                                                    (8)
#define AIO_O_HDMI_GCTL_bit_mask                                                     (0x00000700)
#define AIO_O_HDMI_GCTL_bit(data)                                                    (0x00000700&((data)<<8))
#define AIO_O_HDMI_GCTL_bit_src(data)                                                ((0x00000700&(data))>>8)
#define AIO_O_HDMI_GCTL_get_bit(data)                                                ((0x00000700&(data))>>8)
#define AIO_O_HDMI_GCTL_wait_en_shift                                                (7)
#define AIO_O_HDMI_GCTL_wait_en_mask                                                 (0x00000080)
#define AIO_O_HDMI_GCTL_wait_en(data)                                                (0x00000080&((data)<<7))
#define AIO_O_HDMI_GCTL_wait_en_src(data)                                            ((0x00000080&(data))>>7)
#define AIO_O_HDMI_GCTL_get_wait_en(data)                                            ((0x00000080&(data))>>7)
#define AIO_O_HDMI_GCTL_rmute_shift                                                  (5)
#define AIO_O_HDMI_GCTL_rmute_mask                                                   (0x00000020)
#define AIO_O_HDMI_GCTL_rmute(data)                                                  (0x00000020&((data)<<5))
#define AIO_O_HDMI_GCTL_rmute_src(data)                                              ((0x00000020&(data))>>5)
#define AIO_O_HDMI_GCTL_get_rmute(data)                                              ((0x00000020&(data))>>5)
#define AIO_O_HDMI_GCTL_lmute_shift                                                  (4)
#define AIO_O_HDMI_GCTL_lmute_mask                                                   (0x00000010)
#define AIO_O_HDMI_GCTL_lmute(data)                                                  (0x00000010&((data)<<4))
#define AIO_O_HDMI_GCTL_lmute_src(data)                                              ((0x00000010&(data))>>4)
#define AIO_O_HDMI_GCTL_get_lmute(data)                                              ((0x00000010&(data))>>4)
#define AIO_O_HDMI_GCTL_switch_wait_en_shift                                         (3)
#define AIO_O_HDMI_GCTL_switch_wait_en_mask                                          (0x00000008)
#define AIO_O_HDMI_GCTL_switch_wait_en(data)                                         (0x00000008&((data)<<3))
#define AIO_O_HDMI_GCTL_switch_wait_en_src(data)                                     ((0x00000008&(data))>>3)
#define AIO_O_HDMI_GCTL_get_switch_wait_en(data)                                     ((0x00000008&(data))>>3)
#define AIO_O_HDMI_GCTL_sel_shift                                                    (1)
#define AIO_O_HDMI_GCTL_sel_mask                                                     (0x00000006)
#define AIO_O_HDMI_GCTL_sel(data)                                                    (0x00000006&((data)<<1))
#define AIO_O_HDMI_GCTL_sel_src(data)                                                ((0x00000006&(data))>>1)
#define AIO_O_HDMI_GCTL_get_sel(data)                                                ((0x00000006&(data))>>1)
#define AIO_O_HDMI_GCTL_en_shift                                                     (0)
#define AIO_O_HDMI_GCTL_en_mask                                                      (0x00000001)
#define AIO_O_HDMI_GCTL_en(data)                                                     (0x00000001&((data)<<0))
#define AIO_O_HDMI_GCTL_en_src(data)                                                 ((0x00000001&(data))>>0)
#define AIO_O_HDMI_GCTL_get_en(data)                                                 ((0x00000001&(data))>>0)


#define AIO_O_HDMI_FCNT                                                              0x18006074
#define AIO_O_HDMI_FCNT_reg_addr                                                     "0xB8006074"
#define AIO_O_HDMI_FCNT_reg                                                          0xB8006074
#define set_AIO_O_HDMI_FCNT_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_FCNT_reg)=data)
#define get_AIO_O_HDMI_FCNT_reg   (*((volatile unsigned int*) AIO_O_HDMI_FCNT_reg))
#define AIO_O_HDMI_FCNT_inst_adr                                                     "0x001D"
#define AIO_O_HDMI_FCNT_inst                                                         0x001D
#define AIO_O_HDMI_FCNT_realgo_shift                                                 (8)
#define AIO_O_HDMI_FCNT_realgo_mask                                                  (0x00000100)
#define AIO_O_HDMI_FCNT_realgo(data)                                                 (0x00000100&((data)<<8))
#define AIO_O_HDMI_FCNT_realgo_src(data)                                             ((0x00000100&(data))>>8)
#define AIO_O_HDMI_FCNT_get_realgo(data)                                             ((0x00000100&(data))>>8)
#define AIO_O_HDMI_FCNT_fcnt_shift                                                   (0)
#define AIO_O_HDMI_FCNT_fcnt_mask                                                    (0x000000FF)
#define AIO_O_HDMI_FCNT_fcnt(data)                                                   (0x000000FF&((data)<<0))
#define AIO_O_HDMI_FCNT_fcnt_src(data)                                               ((0x000000FF&(data))>>0)
#define AIO_O_HDMI_FCNT_get_fcnt(data)                                               ((0x000000FF&(data))>>0)


#define AIO_O_HDMI_CS1                                                               0x18006078
#define AIO_O_HDMI_CS1_reg_addr                                                      "0xB8006078"
#define AIO_O_HDMI_CS1_reg                                                           0xB8006078
#define set_AIO_O_HDMI_CS1_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_CS1_reg)=data)
#define get_AIO_O_HDMI_CS1_reg   (*((volatile unsigned int*) AIO_O_HDMI_CS1_reg))
#define AIO_O_HDMI_CS1_inst_adr                                                      "0x001E"
#define AIO_O_HDMI_CS1_inst                                                          0x001E
#define AIO_O_HDMI_CS1_cs1_shift                                                     (0)
#define AIO_O_HDMI_CS1_cs1_mask                                                      (0xFFFFFFFF)
#define AIO_O_HDMI_CS1_cs1(data)                                                     (0xFFFFFFFF&((data)<<0))
#define AIO_O_HDMI_CS1_cs1_src(data)                                                 ((0xFFFFFFFF&(data))>>0)
#define AIO_O_HDMI_CS1_get_cs1(data)                                                 ((0xFFFFFFFF&(data))>>0)


#define AIO_O_HDMI_CS2                                                               0x1800607c
#define AIO_O_HDMI_CS2_reg_addr                                                      "0xB800607C"
#define AIO_O_HDMI_CS2_reg                                                           0xB800607C
#define set_AIO_O_HDMI_CS2_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_CS2_reg)=data)
#define get_AIO_O_HDMI_CS2_reg   (*((volatile unsigned int*) AIO_O_HDMI_CS2_reg))
#define AIO_O_HDMI_CS2_inst_adr                                                      "0x001F"
#define AIO_O_HDMI_CS2_inst                                                          0x001F
#define AIO_O_HDMI_CS2_cs2_shift                                                     (0)
#define AIO_O_HDMI_CS2_cs2_mask                                                      (0x000000FF)
#define AIO_O_HDMI_CS2_cs2(data)                                                     (0x000000FF&((data)<<0))
#define AIO_O_HDMI_CS2_cs2_src(data)                                                 ((0x000000FF&(data))>>0)
#define AIO_O_HDMI_CS2_get_cs2(data)                                                 ((0x000000FF&(data))>>0)


#define AIO_O_ACDAC_GCTL                                                             0x18006600
#define AIO_O_ACDAC_GCTL_reg_addr                                                    "0xB8006600"
#define AIO_O_ACDAC_GCTL_reg                                                         0xB8006600
#define set_AIO_O_ACDAC_GCTL_reg(data)   (*((volatile unsigned int*) AIO_O_ACDAC_GCTL_reg)=data)
#define get_AIO_O_ACDAC_GCTL_reg   (*((volatile unsigned int*) AIO_O_ACDAC_GCTL_reg))
#define AIO_O_ACDAC_GCTL_inst_adr                                                    "0x0080"
#define AIO_O_ACDAC_GCTL_inst                                                        0x0080
#define AIO_O_ACDAC_GCTL_rgain_shift                                                 (14)
#define AIO_O_ACDAC_GCTL_rgain_mask                                                  (0x001FC000)
#define AIO_O_ACDAC_GCTL_rgain(data)                                                 (0x001FC000&((data)<<14))
#define AIO_O_ACDAC_GCTL_rgain_src(data)                                             ((0x001FC000&(data))>>14)
#define AIO_O_ACDAC_GCTL_get_rgain(data)                                             ((0x001FC000&(data))>>14)
#define AIO_O_ACDAC_GCTL_lgain_shift                                                 (7)
#define AIO_O_ACDAC_GCTL_lgain_mask                                                  (0x00003F80)
#define AIO_O_ACDAC_GCTL_lgain(data)                                                 (0x00003F80&((data)<<7))
#define AIO_O_ACDAC_GCTL_lgain_src(data)                                             ((0x00003F80&(data))>>7)
#define AIO_O_ACDAC_GCTL_get_lgain(data)                                             ((0x00003F80&(data))>>7)
#define AIO_O_ACDAC_GCTL_bit_shift                                                   (4)
#define AIO_O_ACDAC_GCTL_bit_mask                                                    (0x00000070)
#define AIO_O_ACDAC_GCTL_bit(data)                                                   (0x00000070&((data)<<4))
#define AIO_O_ACDAC_GCTL_bit_src(data)                                               ((0x00000070&(data))>>4)
#define AIO_O_ACDAC_GCTL_get_bit(data)                                               ((0x00000070&(data))>>4)
#define AIO_O_ACDAC_GCTL_hpfen_shift                                                 (3)
#define AIO_O_ACDAC_GCTL_hpfen_mask                                                  (0x00000008)
#define AIO_O_ACDAC_GCTL_hpfen(data)                                                 (0x00000008&((data)<<3))
#define AIO_O_ACDAC_GCTL_hpfen_src(data)                                             ((0x00000008&(data))>>3)
#define AIO_O_ACDAC_GCTL_get_hpfen(data)                                             ((0x00000008&(data))>>3)
#define AIO_O_ACDAC_GCTL_rmute_shift                                                 (2)
#define AIO_O_ACDAC_GCTL_rmute_mask                                                  (0x00000004)
#define AIO_O_ACDAC_GCTL_rmute(data)                                                 (0x00000004&((data)<<2))
#define AIO_O_ACDAC_GCTL_rmute_src(data)                                             ((0x00000004&(data))>>2)
#define AIO_O_ACDAC_GCTL_get_rmute(data)                                             ((0x00000004&(data))>>2)
#define AIO_O_ACDAC_GCTL_lmute_shift                                                 (1)
#define AIO_O_ACDAC_GCTL_lmute_mask                                                  (0x00000002)
#define AIO_O_ACDAC_GCTL_lmute(data)                                                 (0x00000002&((data)<<1))
#define AIO_O_ACDAC_GCTL_lmute_src(data)                                             ((0x00000002&(data))>>1)
#define AIO_O_ACDAC_GCTL_get_lmute(data)                                             ((0x00000002&(data))>>1)
#define AIO_O_ACDAC_GCTL_en_shift                                                    (0)
#define AIO_O_ACDAC_GCTL_en_mask                                                     (0x00000001)
#define AIO_O_ACDAC_GCTL_en(data)                                                    (0x00000001&((data)<<0))
#define AIO_O_ACDAC_GCTL_en_src(data)                                                ((0x00000001&(data))>>0)
#define AIO_O_ACDAC_GCTL_get_en(data)                                                ((0x00000001&(data))>>0)


#define AIO_O_ACANA_GCTL1                                                            0x18006604
#define AIO_O_ACANA_GCTL1_reg_addr                                                   "0xB8006604"
#define AIO_O_ACANA_GCTL1_reg                                                        0xB8006604
#define set_AIO_O_ACANA_GCTL1_reg(data)   (*((volatile unsigned int*) AIO_O_ACANA_GCTL1_reg)=data)
#define get_AIO_O_ACANA_GCTL1_reg   (*((volatile unsigned int*) AIO_O_ACANA_GCTL1_reg))
#define AIO_O_ACANA_GCTL1_inst_adr                                                   "0x0081"
#define AIO_O_ACANA_GCTL1_inst                                                       0x0081
#define AIO_O_ACANA_GCTL1_write_en1_shift                                            (31)
#define AIO_O_ACANA_GCTL1_write_en1_mask                                             (0x80000000)
#define AIO_O_ACANA_GCTL1_write_en1(data)                                            (0x80000000&((data)<<31))
#define AIO_O_ACANA_GCTL1_write_en1_src(data)                                        ((0x80000000&(data))>>31)
#define AIO_O_ACANA_GCTL1_get_write_en1(data)                                        ((0x80000000&(data))>>31)
#define AIO_O_ACANA_GCTL1_pow_bias_shift                                             (30)
#define AIO_O_ACANA_GCTL1_pow_bias_mask                                              (0x40000000)
#define AIO_O_ACANA_GCTL1_pow_bias(data)                                             (0x40000000&((data)<<30))
#define AIO_O_ACANA_GCTL1_pow_bias_src(data)                                         ((0x40000000&(data))>>30)
#define AIO_O_ACANA_GCTL1_get_pow_bias(data)                                         ((0x40000000&(data))>>30)
#define AIO_O_ACANA_GCTL1_write_en3_shift                                            (27)
#define AIO_O_ACANA_GCTL1_write_en3_mask                                             (0x08000000)
#define AIO_O_ACANA_GCTL1_write_en3(data)                                            (0x08000000&((data)<<27))
#define AIO_O_ACANA_GCTL1_write_en3_src(data)                                        ((0x08000000&(data))>>27)
#define AIO_O_ACANA_GCTL1_get_write_en3(data)                                        ((0x08000000&(data))>>27)
#define AIO_O_ACANA_GCTL1_pow_dac_shift                                              (26)
#define AIO_O_ACANA_GCTL1_pow_dac_mask                                               (0x04000000)
#define AIO_O_ACANA_GCTL1_pow_dac(data)                                              (0x04000000&((data)<<26))
#define AIO_O_ACANA_GCTL1_pow_dac_src(data)                                          ((0x04000000&(data))>>26)
#define AIO_O_ACANA_GCTL1_get_pow_dac(data)                                          ((0x04000000&(data))>>26)
#define AIO_O_ACANA_GCTL1_write_en4_shift                                            (25)
#define AIO_O_ACANA_GCTL1_write_en4_mask                                             (0x02000000)
#define AIO_O_ACANA_GCTL1_write_en4(data)                                            (0x02000000&((data)<<25))
#define AIO_O_ACANA_GCTL1_write_en4_src(data)                                        ((0x02000000&(data))>>25)
#define AIO_O_ACANA_GCTL1_get_write_en4(data)                                        ((0x02000000&(data))>>25)
#define AIO_O_ACANA_GCTL1_rstb_shift                                                 (24)
#define AIO_O_ACANA_GCTL1_rstb_mask                                                  (0x01000000)
#define AIO_O_ACANA_GCTL1_rstb(data)                                                 (0x01000000&((data)<<24))
#define AIO_O_ACANA_GCTL1_rstb_src(data)                                             ((0x01000000&(data))>>24)
#define AIO_O_ACANA_GCTL1_get_rstb(data)                                             ((0x01000000&(data))>>24)
#define AIO_O_ACANA_GCTL1_write_en6_shift                                            (20)
#define AIO_O_ACANA_GCTL1_write_en6_mask                                             (0x00100000)
#define AIO_O_ACANA_GCTL1_write_en6(data)                                            (0x00100000&((data)<<20))
#define AIO_O_ACANA_GCTL1_write_en6_src(data)                                        ((0x00100000&(data))>>20)
#define AIO_O_ACANA_GCTL1_get_write_en6(data)                                        ((0x00100000&(data))>>20)
#define AIO_O_ACANA_GCTL1_unmute_l_shift                                             (19)
#define AIO_O_ACANA_GCTL1_unmute_l_mask                                              (0x00080000)
#define AIO_O_ACANA_GCTL1_unmute_l(data)                                             (0x00080000&((data)<<19))
#define AIO_O_ACANA_GCTL1_unmute_l_src(data)                                         ((0x00080000&(data))>>19)
#define AIO_O_ACANA_GCTL1_get_unmute_l(data)                                         ((0x00080000&(data))>>19)
#define AIO_O_ACANA_GCTL1_write_en7_shift                                            (18)
#define AIO_O_ACANA_GCTL1_write_en7_mask                                             (0x00040000)
#define AIO_O_ACANA_GCTL1_write_en7(data)                                            (0x00040000&((data)<<18))
#define AIO_O_ACANA_GCTL1_write_en7_src(data)                                        ((0x00040000&(data))>>18)
#define AIO_O_ACANA_GCTL1_get_write_en7(data)                                        ((0x00040000&(data))>>18)
#define AIO_O_ACANA_GCTL1_unmute_r_shift                                             (17)
#define AIO_O_ACANA_GCTL1_unmute_r_mask                                              (0x00020000)
#define AIO_O_ACANA_GCTL1_unmute_r(data)                                             (0x00020000&((data)<<17))
#define AIO_O_ACANA_GCTL1_unmute_r_src(data)                                         ((0x00020000&(data))>>17)
#define AIO_O_ACANA_GCTL1_get_unmute_r(data)                                         ((0x00020000&(data))>>17)
#define AIO_O_ACANA_GCTL1_write_en8_shift                                            (16)
#define AIO_O_ACANA_GCTL1_write_en8_mask                                             (0x00010000)
#define AIO_O_ACANA_GCTL1_write_en8(data)                                            (0x00010000&((data)<<16))
#define AIO_O_ACANA_GCTL1_write_en8_src(data)                                        ((0x00010000&(data))>>16)
#define AIO_O_ACANA_GCTL1_get_write_en8(data)                                        ((0x00010000&(data))>>16)
#define AIO_O_ACANA_GCTL1_ckxen_shift                                                (15)
#define AIO_O_ACANA_GCTL1_ckxen_mask                                                 (0x00008000)
#define AIO_O_ACANA_GCTL1_ckxen(data)                                                (0x00008000&((data)<<15))
#define AIO_O_ACANA_GCTL1_ckxen_src(data)                                            ((0x00008000&(data))>>15)
#define AIO_O_ACANA_GCTL1_get_ckxen(data)                                            ((0x00008000&(data))>>15)
#define AIO_O_ACANA_GCTL1_write_en9_shift                                            (14)
#define AIO_O_ACANA_GCTL1_write_en9_mask                                             (0x00004000)
#define AIO_O_ACANA_GCTL1_write_en9(data)                                            (0x00004000&((data)<<14))
#define AIO_O_ACANA_GCTL1_write_en9_src(data)                                        ((0x00004000&(data))>>14)
#define AIO_O_ACANA_GCTL1_get_write_en9(data)                                        ((0x00004000&(data))>>14)
#define AIO_O_ACANA_GCTL1_ckxsel_shift                                               (13)
#define AIO_O_ACANA_GCTL1_ckxsel_mask                                                (0x00002000)
#define AIO_O_ACANA_GCTL1_ckxsel(data)                                               (0x00002000&((data)<<13))
#define AIO_O_ACANA_GCTL1_ckxsel_src(data)                                           ((0x00002000&(data))>>13)
#define AIO_O_ACANA_GCTL1_get_ckxsel(data)                                           ((0x00002000&(data))>>13)
#define AIO_O_ACANA_GCTL1_write_en10_shift                                           (12)
#define AIO_O_ACANA_GCTL1_write_en10_mask                                            (0x00001000)
#define AIO_O_ACANA_GCTL1_write_en10(data)                                           (0x00001000&((data)<<12))
#define AIO_O_ACANA_GCTL1_write_en10_src(data)                                       ((0x00001000&(data))>>12)
#define AIO_O_ACANA_GCTL1_get_write_en10(data)                                       ((0x00001000&(data))>>12)
#define AIO_O_ACANA_GCTL1_cks_sel_shift                                              (11)
#define AIO_O_ACANA_GCTL1_cks_sel_mask                                               (0x00000800)
#define AIO_O_ACANA_GCTL1_cks_sel(data)                                              (0x00000800&((data)<<11))
#define AIO_O_ACANA_GCTL1_cks_sel_src(data)                                          ((0x00000800&(data))>>11)
#define AIO_O_ACANA_GCTL1_get_cks_sel(data)                                          ((0x00000800&(data))>>11)
#define AIO_O_ACANA_GCTL1_write_en11_shift                                           (10)
#define AIO_O_ACANA_GCTL1_write_en11_mask                                            (0x00000400)
#define AIO_O_ACANA_GCTL1_write_en11(data)                                           (0x00000400&((data)<<10))
#define AIO_O_ACANA_GCTL1_write_en11_src(data)                                       ((0x00000400&(data))>>10)
#define AIO_O_ACANA_GCTL1_get_write_en11(data)                                       ((0x00000400&(data))>>10)
#define AIO_O_ACANA_GCTL1_v2i_sel_shift                                              (9)
#define AIO_O_ACANA_GCTL1_v2i_sel_mask                                               (0x00000200)
#define AIO_O_ACANA_GCTL1_v2i_sel(data)                                              (0x00000200&((data)<<9))
#define AIO_O_ACANA_GCTL1_v2i_sel_src(data)                                          ((0x00000200&(data))>>9)
#define AIO_O_ACANA_GCTL1_get_v2i_sel(data)                                          ((0x00000200&(data))>>9)
#define AIO_O_ACANA_GCTL1_write_en12_shift                                           (8)
#define AIO_O_ACANA_GCTL1_write_en12_mask                                            (0x00000100)
#define AIO_O_ACANA_GCTL1_write_en12(data)                                           (0x00000100&((data)<<8))
#define AIO_O_ACANA_GCTL1_write_en12_src(data)                                       ((0x00000100&(data))>>8)
#define AIO_O_ACANA_GCTL1_get_write_en12(data)                                       ((0x00000100&(data))>>8)
#define AIO_O_ACANA_GCTL1_imp_shift                                                  (7)
#define AIO_O_ACANA_GCTL1_imp_mask                                                   (0x00000080)
#define AIO_O_ACANA_GCTL1_imp(data)                                                  (0x00000080&((data)<<7))
#define AIO_O_ACANA_GCTL1_imp_src(data)                                              ((0x00000080&(data))>>7)
#define AIO_O_ACANA_GCTL1_get_imp(data)                                              ((0x00000080&(data))>>7)
#define AIO_O_ACANA_GCTL1_write_en13_shift                                           (6)
#define AIO_O_ACANA_GCTL1_write_en13_mask                                            (0x00000040)
#define AIO_O_ACANA_GCTL1_write_en13(data)                                           (0x00000040&((data)<<6))
#define AIO_O_ACANA_GCTL1_write_en13_src(data)                                       ((0x00000040&(data))>>6)
#define AIO_O_ACANA_GCTL1_get_write_en13(data)                                       ((0x00000040&(data))>>6)
#define AIO_O_ACANA_GCTL1_disc_shift                                                 (5)
#define AIO_O_ACANA_GCTL1_disc_mask                                                  (0x00000020)
#define AIO_O_ACANA_GCTL1_disc(data)                                                 (0x00000020&((data)<<5))
#define AIO_O_ACANA_GCTL1_disc_src(data)                                             ((0x00000020&(data))>>5)
#define AIO_O_ACANA_GCTL1_get_disc(data)                                             ((0x00000020&(data))>>5)
#define AIO_O_ACANA_GCTL1_write_en14_shift                                           (4)
#define AIO_O_ACANA_GCTL1_write_en14_mask                                            (0x00000010)
#define AIO_O_ACANA_GCTL1_write_en14(data)                                           (0x00000010&((data)<<4))
#define AIO_O_ACANA_GCTL1_write_en14_src(data)                                       ((0x00000010&(data))>>4)
#define AIO_O_ACANA_GCTL1_get_write_en14(data)                                       ((0x00000010&(data))>>4)
#define AIO_O_ACANA_GCTL1_en_amp_shift                                               (3)
#define AIO_O_ACANA_GCTL1_en_amp_mask                                                (0x00000008)
#define AIO_O_ACANA_GCTL1_en_amp(data)                                               (0x00000008&((data)<<3))
#define AIO_O_ACANA_GCTL1_en_amp_src(data)                                           ((0x00000008&(data))>>3)
#define AIO_O_ACANA_GCTL1_get_en_amp(data)                                           ((0x00000008&(data))>>3)
#define AIO_O_ACANA_GCTL1_write_en15_shift                                           (2)
#define AIO_O_ACANA_GCTL1_write_en15_mask                                            (0x00000004)
#define AIO_O_ACANA_GCTL1_write_en15(data)                                           (0x00000004&((data)<<2))
#define AIO_O_ACANA_GCTL1_write_en15_src(data)                                       ((0x00000004&(data))>>2)
#define AIO_O_ACANA_GCTL1_get_write_en15(data)                                       ((0x00000004&(data))>>2)
#define AIO_O_ACANA_GCTL1_vrp_sel_shift                                              (0)
#define AIO_O_ACANA_GCTL1_vrp_sel_mask                                               (0x00000003)
#define AIO_O_ACANA_GCTL1_vrp_sel(data)                                              (0x00000003&((data)<<0))
#define AIO_O_ACANA_GCTL1_vrp_sel_src(data)                                          ((0x00000003&(data))>>0)
#define AIO_O_ACANA_GCTL1_get_vrp_sel(data)                                          ((0x00000003&(data))>>0)


#define AIO_O_ACANA_GCTL2                                                            0x18006608
#define AIO_O_ACANA_GCTL2_reg_addr                                                   "0xB8006608"
#define AIO_O_ACANA_GCTL2_reg                                                        0xB8006608
#define set_AIO_O_ACANA_GCTL2_reg(data)   (*((volatile unsigned int*) AIO_O_ACANA_GCTL2_reg)=data)
#define get_AIO_O_ACANA_GCTL2_reg   (*((volatile unsigned int*) AIO_O_ACANA_GCTL2_reg))
#define AIO_O_ACANA_GCTL2_inst_adr                                                   "0x0082"
#define AIO_O_ACANA_GCTL2_inst                                                       0x0082
#define AIO_O_ACANA_GCTL2_write_en1_shift                                            (18)
#define AIO_O_ACANA_GCTL2_write_en1_mask                                             (0x00040000)
#define AIO_O_ACANA_GCTL2_write_en1(data)                                            (0x00040000&((data)<<18))
#define AIO_O_ACANA_GCTL2_write_en1_src(data)                                        ((0x00040000&(data))>>18)
#define AIO_O_ACANA_GCTL2_get_write_en1(data)                                        ((0x00040000&(data))>>18)
#define AIO_O_ACANA_GCTL2_vcmh_sel_shift                                             (17)
#define AIO_O_ACANA_GCTL2_vcmh_sel_mask                                              (0x00020000)
#define AIO_O_ACANA_GCTL2_vcmh_sel(data)                                             (0x00020000&((data)<<17))
#define AIO_O_ACANA_GCTL2_vcmh_sel_src(data)                                         ((0x00020000&(data))>>17)
#define AIO_O_ACANA_GCTL2_get_vcmh_sel(data)                                         ((0x00020000&(data))>>17)
#define AIO_O_ACANA_GCTL2_write_en2_shift                                            (16)
#define AIO_O_ACANA_GCTL2_write_en2_mask                                             (0x00010000)
#define AIO_O_ACANA_GCTL2_write_en2(data)                                            (0x00010000&((data)<<16))
#define AIO_O_ACANA_GCTL2_write_en2_src(data)                                        ((0x00010000&(data))>>16)
#define AIO_O_ACANA_GCTL2_get_write_en2(data)                                        ((0x00010000&(data))>>16)
#define AIO_O_ACANA_GCTL2_ct_bias_shift                                              (0)
#define AIO_O_ACANA_GCTL2_ct_bias_mask                                               (0x0000FFFF)
#define AIO_O_ACANA_GCTL2_ct_bias(data)                                              (0x0000FFFF&((data)<<0))
#define AIO_O_ACANA_GCTL2_ct_bias_src(data)                                          ((0x0000FFFF&(data))>>0)
#define AIO_O_ACANA_GCTL2_get_ct_bias(data)                                          ((0x0000FFFF&(data))>>0)


#define AIO_I_FIFOCTL0                                                               0x18006080
#define AIO_I_FIFOCTL0_reg_addr                                                      "0xB8006080"
#define AIO_I_FIFOCTL0_reg                                                           0xB8006080
#define set_AIO_I_FIFOCTL0_reg(data)   (*((volatile unsigned int*) AIO_I_FIFOCTL0_reg)=data)
#define get_AIO_I_FIFOCTL0_reg   (*((volatile unsigned int*) AIO_I_FIFOCTL0_reg))
#define AIO_I_FIFOCTL0_inst_adr                                                      "0x0020"
#define AIO_I_FIFOCTL0_inst                                                          0x0020
#define AIO_I_FIFOCTL0_thd_shift                                                     (1)
#define AIO_I_FIFOCTL0_thd_mask                                                      (0x0000003E)
#define AIO_I_FIFOCTL0_thd(data)                                                     (0x0000003E&((data)<<1))
#define AIO_I_FIFOCTL0_thd_src(data)                                                 ((0x0000003E&(data))>>1)
#define AIO_I_FIFOCTL0_get_thd(data)                                                 ((0x0000003E&(data))>>1)
#define AIO_I_FIFOCTL0_en_shift                                                      (0)
#define AIO_I_FIFOCTL0_en_mask                                                       (0x00000001)
#define AIO_I_FIFOCTL0_en(data)                                                      (0x00000001&((data)<<0))
#define AIO_I_FIFOCTL0_en_src(data)                                                  ((0x00000001&(data))>>0)
#define AIO_I_FIFOCTL0_get_en(data)                                                  ((0x00000001&(data))>>0)


#define AIO_O_FIFOCTL                                                                0x18006090
#define AIO_O_FIFOCTL_reg_addr                                                       "0xB8006090"
#define AIO_O_FIFOCTL_reg                                                            0xB8006090
#define set_AIO_O_FIFOCTL_reg(data)   (*((volatile unsigned int*) AIO_O_FIFOCTL_reg)=data)
#define get_AIO_O_FIFOCTL_reg   (*((volatile unsigned int*) AIO_O_FIFOCTL_reg))
#define AIO_O_FIFOCTL_inst_adr                                                       "0x0024"
#define AIO_O_FIFOCTL_inst                                                           0x0024
#define AIO_O_FIFOCTL_en10_shift                                                     (16)
#define AIO_O_FIFOCTL_en10_mask                                                      (0x00010000)
#define AIO_O_FIFOCTL_en10(data)                                                     (0x00010000&((data)<<16))
#define AIO_O_FIFOCTL_en10_src(data)                                                 ((0x00010000&(data))>>16)
#define AIO_O_FIFOCTL_get_en10(data)                                                 ((0x00010000&(data))>>16)
#define AIO_O_FIFOCTL_en9_shift                                                      (15)
#define AIO_O_FIFOCTL_en9_mask                                                       (0x00008000)
#define AIO_O_FIFOCTL_en9(data)                                                      (0x00008000&((data)<<15))
#define AIO_O_FIFOCTL_en9_src(data)                                                  ((0x00008000&(data))>>15)
#define AIO_O_FIFOCTL_get_en9(data)                                                  ((0x00008000&(data))>>15)
#define AIO_O_FIFOCTL_en8_shift                                                      (14)
#define AIO_O_FIFOCTL_en8_mask                                                       (0x00004000)
#define AIO_O_FIFOCTL_en8(data)                                                      (0x00004000&((data)<<14))
#define AIO_O_FIFOCTL_en8_src(data)                                                  ((0x00004000&(data))>>14)
#define AIO_O_FIFOCTL_get_en8(data)                                                  ((0x00004000&(data))>>14)
#define AIO_O_FIFOCTL_en7_shift                                                      (13)
#define AIO_O_FIFOCTL_en7_mask                                                       (0x00002000)
#define AIO_O_FIFOCTL_en7(data)                                                      (0x00002000&((data)<<13))
#define AIO_O_FIFOCTL_en7_src(data)                                                  ((0x00002000&(data))>>13)
#define AIO_O_FIFOCTL_get_en7(data)                                                  ((0x00002000&(data))>>13)
#define AIO_O_FIFOCTL_en6_shift                                                      (12)
#define AIO_O_FIFOCTL_en6_mask                                                       (0x00001000)
#define AIO_O_FIFOCTL_en6(data)                                                      (0x00001000&((data)<<12))
#define AIO_O_FIFOCTL_en6_src(data)                                                  ((0x00001000&(data))>>12)
#define AIO_O_FIFOCTL_get_en6(data)                                                  ((0x00001000&(data))>>12)
#define AIO_O_FIFOCTL_emptystopen_shift                                              (11)
#define AIO_O_FIFOCTL_emptystopen_mask                                               (0x00000800)
#define AIO_O_FIFOCTL_emptystopen(data)                                              (0x00000800&((data)<<11))
#define AIO_O_FIFOCTL_emptystopen_src(data)                                          ((0x00000800&(data))>>11)
#define AIO_O_FIFOCTL_get_emptystopen(data)                                          ((0x00000800&(data))>>11)
#define AIO_O_FIFOCTL_thd_shift                                                      (6)
#define AIO_O_FIFOCTL_thd_mask                                                       (0x000007C0)
#define AIO_O_FIFOCTL_thd(data)                                                      (0x000007C0&((data)<<6))
#define AIO_O_FIFOCTL_thd_src(data)                                                  ((0x000007C0&(data))>>6)
#define AIO_O_FIFOCTL_get_thd(data)                                                  ((0x000007C0&(data))>>6)
#define AIO_O_FIFOCTL_en5_shift                                                      (5)
#define AIO_O_FIFOCTL_en5_mask                                                       (0x00000020)
#define AIO_O_FIFOCTL_en5(data)                                                      (0x00000020&((data)<<5))
#define AIO_O_FIFOCTL_en5_src(data)                                                  ((0x00000020&(data))>>5)
#define AIO_O_FIFOCTL_get_en5(data)                                                  ((0x00000020&(data))>>5)
#define AIO_O_FIFOCTL_en4_shift                                                      (4)
#define AIO_O_FIFOCTL_en4_mask                                                       (0x00000010)
#define AIO_O_FIFOCTL_en4(data)                                                      (0x00000010&((data)<<4))
#define AIO_O_FIFOCTL_en4_src(data)                                                  ((0x00000010&(data))>>4)
#define AIO_O_FIFOCTL_get_en4(data)                                                  ((0x00000010&(data))>>4)
#define AIO_O_FIFOCTL_en3_shift                                                      (3)
#define AIO_O_FIFOCTL_en3_mask                                                       (0x00000008)
#define AIO_O_FIFOCTL_en3(data)                                                      (0x00000008&((data)<<3))
#define AIO_O_FIFOCTL_en3_src(data)                                                  ((0x00000008&(data))>>3)
#define AIO_O_FIFOCTL_get_en3(data)                                                  ((0x00000008&(data))>>3)
#define AIO_O_FIFOCTL_en2_shift                                                      (2)
#define AIO_O_FIFOCTL_en2_mask                                                       (0x00000004)
#define AIO_O_FIFOCTL_en2(data)                                                      (0x00000004&((data)<<2))
#define AIO_O_FIFOCTL_en2_src(data)                                                  ((0x00000004&(data))>>2)
#define AIO_O_FIFOCTL_get_en2(data)                                                  ((0x00000004&(data))>>2)
#define AIO_O_FIFOCTL_en1_shift                                                      (1)
#define AIO_O_FIFOCTL_en1_mask                                                       (0x00000002)
#define AIO_O_FIFOCTL_en1(data)                                                      (0x00000002&((data)<<1))
#define AIO_O_FIFOCTL_en1_src(data)                                                  ((0x00000002&(data))>>1)
#define AIO_O_FIFOCTL_get_en1(data)                                                  ((0x00000002&(data))>>1)
#define AIO_O_FIFOCTL_en0_shift                                                      (0)
#define AIO_O_FIFOCTL_en0_mask                                                       (0x00000001)
#define AIO_O_FIFOCTL_en0(data)                                                      (0x00000001&((data)<<0))
#define AIO_O_FIFOCTL_en0_src(data)                                                  ((0x00000001&(data))>>0)
#define AIO_O_FIFOCTL_get_en0(data)                                                  ((0x00000001&(data))>>0)


#define AIO_O_I2S_LDATA0                                                             0x18006094
#define AIO_O_I2S_LDATA0_reg_addr                                                    "0xB8006094"
#define AIO_O_I2S_LDATA0_reg                                                         0xB8006094
#define set_AIO_O_I2S_LDATA0_reg(data)   (*((volatile unsigned int*) AIO_O_I2S_LDATA0_reg)=data)
#define get_AIO_O_I2S_LDATA0_reg   (*((volatile unsigned int*) AIO_O_I2S_LDATA0_reg))
#define AIO_O_I2S_LDATA0_inst_adr                                                    "0x0025"
#define AIO_O_I2S_LDATA0_inst                                                        0x0025
#define AIO_O_I2S_LDATA0_data_shift                                                  (0)
#define AIO_O_I2S_LDATA0_data_mask                                                   (0xFFFFFFFF)
#define AIO_O_I2S_LDATA0_data(data)                                                  (0xFFFFFFFF&((data)<<0))
#define AIO_O_I2S_LDATA0_data_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define AIO_O_I2S_LDATA0_get_data(data)                                              ((0xFFFFFFFF&(data))>>0)


#define AIO_O_I2S_RDATA0                                                             0x18006098
#define AIO_O_I2S_RDATA0_reg_addr                                                    "0xB8006098"
#define AIO_O_I2S_RDATA0_reg                                                         0xB8006098
#define set_AIO_O_I2S_RDATA0_reg(data)   (*((volatile unsigned int*) AIO_O_I2S_RDATA0_reg)=data)
#define get_AIO_O_I2S_RDATA0_reg   (*((volatile unsigned int*) AIO_O_I2S_RDATA0_reg))
#define AIO_O_I2S_RDATA0_inst_adr                                                    "0x0026"
#define AIO_O_I2S_RDATA0_inst                                                        0x0026
#define AIO_O_I2S_RDATA0_data_shift                                                  (0)
#define AIO_O_I2S_RDATA0_data_mask                                                   (0xFFFFFFFF)
#define AIO_O_I2S_RDATA0_data(data)                                                  (0xFFFFFFFF&((data)<<0))
#define AIO_O_I2S_RDATA0_data_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define AIO_O_I2S_RDATA0_get_data(data)                                              ((0xFFFFFFFF&(data))>>0)


#define AIO_O_SPF_LDATA                                                              0x180060bc
#define AIO_O_SPF_LDATA_reg_addr                                                     "0xB80060BC"
#define AIO_O_SPF_LDATA_reg                                                          0xB80060BC
#define set_AIO_O_SPF_LDATA_reg(data)   (*((volatile unsigned int*) AIO_O_SPF_LDATA_reg)=data)
#define get_AIO_O_SPF_LDATA_reg   (*((volatile unsigned int*) AIO_O_SPF_LDATA_reg))
#define AIO_O_SPF_LDATA_inst_adr                                                     "0x002F"
#define AIO_O_SPF_LDATA_inst                                                         0x002F
#define AIO_O_SPF_LDATA_data_shift                                                   (0)
#define AIO_O_SPF_LDATA_data_mask                                                    (0xFFFFFFFF)
#define AIO_O_SPF_LDATA_data(data)                                                   (0xFFFFFFFF&((data)<<0))
#define AIO_O_SPF_LDATA_data_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define AIO_O_SPF_LDATA_get_data(data)                                               ((0xFFFFFFFF&(data))>>0)


#define AIO_O_SPF_RDATA                                                              0x180060c0
#define AIO_O_SPF_RDATA_reg_addr                                                     "0xB80060C0"
#define AIO_O_SPF_RDATA_reg                                                          0xB80060C0
#define set_AIO_O_SPF_RDATA_reg(data)   (*((volatile unsigned int*) AIO_O_SPF_RDATA_reg)=data)
#define get_AIO_O_SPF_RDATA_reg   (*((volatile unsigned int*) AIO_O_SPF_RDATA_reg))
#define AIO_O_SPF_RDATA_inst_adr                                                     "0x0030"
#define AIO_O_SPF_RDATA_inst                                                         0x0030
#define AIO_O_SPF_RDATA_data_shift                                                   (0)
#define AIO_O_SPF_RDATA_data_mask                                                    (0xFFFFFFFF)
#define AIO_O_SPF_RDATA_data(data)                                                   (0xFFFFFFFF&((data)<<0))
#define AIO_O_SPF_RDATA_data_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define AIO_O_SPF_RDATA_get_data(data)                                               ((0xFFFFFFFF&(data))>>0)


#define AIO_O_FIFO_STATUS                                                            0x180060c4
#define AIO_O_FIFO_STATUS_reg_addr                                                   "0xB80060C4"
#define AIO_O_FIFO_STATUS_reg                                                        0xB80060C4
#define set_AIO_O_FIFO_STATUS_reg(data)   (*((volatile unsigned int*) AIO_O_FIFO_STATUS_reg)=data)
#define get_AIO_O_FIFO_STATUS_reg   (*((volatile unsigned int*) AIO_O_FIFO_STATUS_reg))
#define AIO_O_FIFO_STATUS_inst_adr                                                   "0x0031"
#define AIO_O_FIFO_STATUS_inst                                                       0x0031
#define AIO_O_FIFO_STATUS_empty11_shift                                              (11)
#define AIO_O_FIFO_STATUS_empty11_mask                                               (0x00000800)
#define AIO_O_FIFO_STATUS_empty11(data)                                              (0x00000800&((data)<<11))
#define AIO_O_FIFO_STATUS_empty11_src(data)                                          ((0x00000800&(data))>>11)
#define AIO_O_FIFO_STATUS_get_empty11(data)                                          ((0x00000800&(data))>>11)
#define AIO_O_FIFO_STATUS_empty10_shift                                              (10)
#define AIO_O_FIFO_STATUS_empty10_mask                                               (0x00000400)
#define AIO_O_FIFO_STATUS_empty10(data)                                              (0x00000400&((data)<<10))
#define AIO_O_FIFO_STATUS_empty10_src(data)                                          ((0x00000400&(data))>>10)
#define AIO_O_FIFO_STATUS_get_empty10(data)                                          ((0x00000400&(data))>>10)
#define AIO_O_FIFO_STATUS_empty9_shift                                               (9)
#define AIO_O_FIFO_STATUS_empty9_mask                                                (0x00000200)
#define AIO_O_FIFO_STATUS_empty9(data)                                               (0x00000200&((data)<<9))
#define AIO_O_FIFO_STATUS_empty9_src(data)                                           ((0x00000200&(data))>>9)
#define AIO_O_FIFO_STATUS_get_empty9(data)                                           ((0x00000200&(data))>>9)
#define AIO_O_FIFO_STATUS_empty8_shift                                               (8)
#define AIO_O_FIFO_STATUS_empty8_mask                                                (0x00000100)
#define AIO_O_FIFO_STATUS_empty8(data)                                               (0x00000100&((data)<<8))
#define AIO_O_FIFO_STATUS_empty8_src(data)                                           ((0x00000100&(data))>>8)
#define AIO_O_FIFO_STATUS_get_empty8(data)                                           ((0x00000100&(data))>>8)
#define AIO_O_FIFO_STATUS_empty7_shift                                               (7)
#define AIO_O_FIFO_STATUS_empty7_mask                                                (0x00000080)
#define AIO_O_FIFO_STATUS_empty7(data)                                               (0x00000080&((data)<<7))
#define AIO_O_FIFO_STATUS_empty7_src(data)                                           ((0x00000080&(data))>>7)
#define AIO_O_FIFO_STATUS_get_empty7(data)                                           ((0x00000080&(data))>>7)
#define AIO_O_FIFO_STATUS_empty6_shift                                               (6)
#define AIO_O_FIFO_STATUS_empty6_mask                                                (0x00000040)
#define AIO_O_FIFO_STATUS_empty6(data)                                               (0x00000040&((data)<<6))
#define AIO_O_FIFO_STATUS_empty6_src(data)                                           ((0x00000040&(data))>>6)
#define AIO_O_FIFO_STATUS_get_empty6(data)                                           ((0x00000040&(data))>>6)
#define AIO_O_FIFO_STATUS_empty5_shift                                               (5)
#define AIO_O_FIFO_STATUS_empty5_mask                                                (0x00000020)
#define AIO_O_FIFO_STATUS_empty5(data)                                               (0x00000020&((data)<<5))
#define AIO_O_FIFO_STATUS_empty5_src(data)                                           ((0x00000020&(data))>>5)
#define AIO_O_FIFO_STATUS_get_empty5(data)                                           ((0x00000020&(data))>>5)
#define AIO_O_FIFO_STATUS_empty4_shift                                               (4)
#define AIO_O_FIFO_STATUS_empty4_mask                                                (0x00000010)
#define AIO_O_FIFO_STATUS_empty4(data)                                               (0x00000010&((data)<<4))
#define AIO_O_FIFO_STATUS_empty4_src(data)                                           ((0x00000010&(data))>>4)
#define AIO_O_FIFO_STATUS_get_empty4(data)                                           ((0x00000010&(data))>>4)
#define AIO_O_FIFO_STATUS_empty3_shift                                               (3)
#define AIO_O_FIFO_STATUS_empty3_mask                                                (0x00000008)
#define AIO_O_FIFO_STATUS_empty3(data)                                               (0x00000008&((data)<<3))
#define AIO_O_FIFO_STATUS_empty3_src(data)                                           ((0x00000008&(data))>>3)
#define AIO_O_FIFO_STATUS_get_empty3(data)                                           ((0x00000008&(data))>>3)
#define AIO_O_FIFO_STATUS_empty2_shift                                               (2)
#define AIO_O_FIFO_STATUS_empty2_mask                                                (0x00000004)
#define AIO_O_FIFO_STATUS_empty2(data)                                               (0x00000004&((data)<<2))
#define AIO_O_FIFO_STATUS_empty2_src(data)                                           ((0x00000004&(data))>>2)
#define AIO_O_FIFO_STATUS_get_empty2(data)                                           ((0x00000004&(data))>>2)
#define AIO_O_FIFO_STATUS_empty1_shift                                               (1)
#define AIO_O_FIFO_STATUS_empty1_mask                                                (0x00000002)
#define AIO_O_FIFO_STATUS_empty1(data)                                               (0x00000002&((data)<<1))
#define AIO_O_FIFO_STATUS_empty1_src(data)                                           ((0x00000002&(data))>>1)
#define AIO_O_FIFO_STATUS_get_empty1(data)                                           ((0x00000002&(data))>>1)
#define AIO_O_FIFO_STATUS_empty0_shift                                               (0)
#define AIO_O_FIFO_STATUS_empty0_mask                                                (0x00000001)
#define AIO_O_FIFO_STATUS_empty0(data)                                               (0x00000001&((data)<<0))
#define AIO_O_FIFO_STATUS_empty0_src(data)                                           ((0x00000001&(data))>>0)
#define AIO_O_FIFO_STATUS_get_empty0(data)                                           ((0x00000001&(data))>>0)


#define AIO_I_SA_0                                                                   0x18006100
#define AIO_I_SA_1                                                                   0x18006104
#define AIO_I_SA_0_reg_addr                                                          "0xB8006100"
#define AIO_I_SA_1_reg_addr                                                          "0xB8006104"
#define AIO_I_SA_0_reg                                                               0xB8006100
#define AIO_I_SA_1_reg                                                               0xB8006104
#define set_AIO_I_SA_0_reg(data)   (*((volatile unsigned int*) AIO_I_SA_0_reg)=data)
#define set_AIO_I_SA_1_reg(data)   (*((volatile unsigned int*) AIO_I_SA_1_reg)=data)
#define get_AIO_I_SA_0_reg   (*((volatile unsigned int*) AIO_I_SA_0_reg))
#define get_AIO_I_SA_1_reg   (*((volatile unsigned int*) AIO_I_SA_1_reg))
#define AIO_I_SA_0_inst_adr                                                          "0x0040"
#define AIO_I_SA_1_inst_adr                                                          "0x0041"
#define AIO_I_SA_0_inst                                                              0x0040
#define AIO_I_SA_1_inst                                                              0x0041
#define AIO_I_SA_addr_shift                                                          (3)
#define AIO_I_SA_addr_mask                                                           (0x7FFFFFF8)
#define AIO_I_SA_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_I_SA_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_I_SA_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_I_EA_0                                                                   0x18006120
#define AIO_I_EA_1                                                                   0x18006124
#define AIO_I_EA_0_reg_addr                                                          "0xB8006120"
#define AIO_I_EA_1_reg_addr                                                          "0xB8006124"
#define AIO_I_EA_0_reg                                                               0xB8006120
#define AIO_I_EA_1_reg                                                               0xB8006124
#define set_AIO_I_EA_0_reg(data)   (*((volatile unsigned int*) AIO_I_EA_0_reg)=data)
#define set_AIO_I_EA_1_reg(data)   (*((volatile unsigned int*) AIO_I_EA_1_reg)=data)
#define get_AIO_I_EA_0_reg   (*((volatile unsigned int*) AIO_I_EA_0_reg))
#define get_AIO_I_EA_1_reg   (*((volatile unsigned int*) AIO_I_EA_1_reg))
#define AIO_I_EA_0_inst_adr                                                          "0x0048"
#define AIO_I_EA_1_inst_adr                                                          "0x0049"
#define AIO_I_EA_0_inst                                                              0x0048
#define AIO_I_EA_1_inst                                                              0x0049
#define AIO_I_EA_addr_shift                                                          (3)
#define AIO_I_EA_addr_mask                                                           (0x7FFFFFF8)
#define AIO_I_EA_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_I_EA_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_I_EA_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_I_RP_0                                                                   0x18006140
#define AIO_I_RP_1                                                                   0x18006144
#define AIO_I_RP_0_reg_addr                                                          "0xB8006140"
#define AIO_I_RP_1_reg_addr                                                          "0xB8006144"
#define AIO_I_RP_0_reg                                                               0xB8006140
#define AIO_I_RP_1_reg                                                               0xB8006144
#define set_AIO_I_RP_0_reg(data)   (*((volatile unsigned int*) AIO_I_RP_0_reg)=data)
#define set_AIO_I_RP_1_reg(data)   (*((volatile unsigned int*) AIO_I_RP_1_reg)=data)
#define get_AIO_I_RP_0_reg   (*((volatile unsigned int*) AIO_I_RP_0_reg))
#define get_AIO_I_RP_1_reg   (*((volatile unsigned int*) AIO_I_RP_1_reg))
#define AIO_I_RP_0_inst_adr                                                          "0x0050"
#define AIO_I_RP_1_inst_adr                                                          "0x0051"
#define AIO_I_RP_0_inst                                                              0x0050
#define AIO_I_RP_1_inst                                                              0x0051
#define AIO_I_RP_addr_shift                                                          (3)
#define AIO_I_RP_addr_mask                                                           (0x7FFFFFF8)
#define AIO_I_RP_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_I_RP_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_I_RP_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_I_WP_0                                                                   0x18006160
#define AIO_I_WP_1                                                                   0x18006164
#define AIO_I_WP_0_reg_addr                                                          "0xB8006160"
#define AIO_I_WP_1_reg_addr                                                          "0xB8006164"
#define AIO_I_WP_0_reg                                                               0xB8006160
#define AIO_I_WP_1_reg                                                               0xB8006164
#define set_AIO_I_WP_0_reg(data)   (*((volatile unsigned int*) AIO_I_WP_0_reg)=data)
#define set_AIO_I_WP_1_reg(data)   (*((volatile unsigned int*) AIO_I_WP_1_reg)=data)
#define get_AIO_I_WP_0_reg   (*((volatile unsigned int*) AIO_I_WP_0_reg))
#define get_AIO_I_WP_1_reg   (*((volatile unsigned int*) AIO_I_WP_1_reg))
#define AIO_I_WP_0_inst_adr                                                          "0x0058"
#define AIO_I_WP_1_inst_adr                                                          "0x0059"
#define AIO_I_WP_0_inst                                                              0x0058
#define AIO_I_WP_1_inst                                                              0x0059
#define AIO_I_WP_addr_shift                                                          (3)
#define AIO_I_WP_addr_mask                                                           (0x7FFFFFF8)
#define AIO_I_WP_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_I_WP_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_I_WP_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_O_SA_0                                                                   0x18006180
#define AIO_O_SA_1                                                                   0x18006184
#define AIO_O_SA_2                                                                   0x18006188
#define AIO_O_SA_3                                                                   0x1800618C
#define AIO_O_SA_4                                                                   0x18006190
#define AIO_O_SA_5                                                                   0x18006194
#define AIO_O_SA_6                                                                   0x18006198
#define AIO_O_SA_7                                                                   0x1800619C
#define AIO_O_SA_8                                                                   0x180061A0
#define AIO_O_SA_9                                                                   0x180061A4
#define AIO_O_SA_10                                                                   0x180061A8
#define AIO_O_SA_11                                                                   0x180061AC
#define AIO_O_SA_0_reg_addr                                                          "0xB8006180"
#define AIO_O_SA_1_reg_addr                                                          "0xB8006184"
#define AIO_O_SA_2_reg_addr                                                          "0xB8006188"
#define AIO_O_SA_3_reg_addr                                                          "0xB800618C"
#define AIO_O_SA_4_reg_addr                                                          "0xB8006190"
#define AIO_O_SA_5_reg_addr                                                          "0xB8006194"
#define AIO_O_SA_6_reg_addr                                                          "0xB8006198"
#define AIO_O_SA_7_reg_addr                                                          "0xB800619C"
#define AIO_O_SA_8_reg_addr                                                          "0xB80061A0"
#define AIO_O_SA_9_reg_addr                                                          "0xB80061A4"
#define AIO_O_SA_10_reg_addr                                                          "0xB80061A8"
#define AIO_O_SA_11_reg_addr                                                          "0xB80061AC"
#define AIO_O_SA_0_reg                                                               0xB8006180
#define AIO_O_SA_1_reg                                                               0xB8006184
#define AIO_O_SA_2_reg                                                               0xB8006188
#define AIO_O_SA_3_reg                                                               0xB800618C
#define AIO_O_SA_4_reg                                                               0xB8006190
#define AIO_O_SA_5_reg                                                               0xB8006194
#define AIO_O_SA_6_reg                                                               0xB8006198
#define AIO_O_SA_7_reg                                                               0xB800619C
#define AIO_O_SA_8_reg                                                               0xB80061A0
#define AIO_O_SA_9_reg                                                               0xB80061A4
#define AIO_O_SA_10_reg                                                               0xB80061A8
#define AIO_O_SA_11_reg                                                               0xB80061AC
#define set_AIO_O_SA_0_reg(data)   (*((volatile unsigned int*) AIO_O_SA_0_reg)=data)
#define set_AIO_O_SA_1_reg(data)   (*((volatile unsigned int*) AIO_O_SA_1_reg)=data)
#define set_AIO_O_SA_2_reg(data)   (*((volatile unsigned int*) AIO_O_SA_2_reg)=data)
#define set_AIO_O_SA_3_reg(data)   (*((volatile unsigned int*) AIO_O_SA_3_reg)=data)
#define set_AIO_O_SA_4_reg(data)   (*((volatile unsigned int*) AIO_O_SA_4_reg)=data)
#define set_AIO_O_SA_5_reg(data)   (*((volatile unsigned int*) AIO_O_SA_5_reg)=data)
#define set_AIO_O_SA_6_reg(data)   (*((volatile unsigned int*) AIO_O_SA_6_reg)=data)
#define set_AIO_O_SA_7_reg(data)   (*((volatile unsigned int*) AIO_O_SA_7_reg)=data)
#define set_AIO_O_SA_8_reg(data)   (*((volatile unsigned int*) AIO_O_SA_8_reg)=data)
#define set_AIO_O_SA_9_reg(data)   (*((volatile unsigned int*) AIO_O_SA_9_reg)=data)
#define set_AIO_O_SA_10_reg(data)   (*((volatile unsigned int*) AIO_O_SA_10_reg)=data)
#define set_AIO_O_SA_11_reg(data)   (*((volatile unsigned int*) AIO_O_SA_11_reg)=data)
#define get_AIO_O_SA_0_reg   (*((volatile unsigned int*) AIO_O_SA_0_reg))
#define get_AIO_O_SA_1_reg   (*((volatile unsigned int*) AIO_O_SA_1_reg))
#define get_AIO_O_SA_2_reg   (*((volatile unsigned int*) AIO_O_SA_2_reg))
#define get_AIO_O_SA_3_reg   (*((volatile unsigned int*) AIO_O_SA_3_reg))
#define get_AIO_O_SA_4_reg   (*((volatile unsigned int*) AIO_O_SA_4_reg))
#define get_AIO_O_SA_5_reg   (*((volatile unsigned int*) AIO_O_SA_5_reg))
#define get_AIO_O_SA_6_reg   (*((volatile unsigned int*) AIO_O_SA_6_reg))
#define get_AIO_O_SA_7_reg   (*((volatile unsigned int*) AIO_O_SA_7_reg))
#define get_AIO_O_SA_8_reg   (*((volatile unsigned int*) AIO_O_SA_8_reg))
#define get_AIO_O_SA_9_reg   (*((volatile unsigned int*) AIO_O_SA_9_reg))
#define get_AIO_O_SA_10_reg   (*((volatile unsigned int*) AIO_O_SA_10_reg))
#define get_AIO_O_SA_11_reg   (*((volatile unsigned int*) AIO_O_SA_11_reg))
#define AIO_O_SA_0_inst_adr                                                          "0x0060"
#define AIO_O_SA_1_inst_adr                                                          "0x0061"
#define AIO_O_SA_2_inst_adr                                                          "0x0062"
#define AIO_O_SA_3_inst_adr                                                          "0x0063"
#define AIO_O_SA_4_inst_adr                                                          "0x0064"
#define AIO_O_SA_5_inst_adr                                                          "0x0065"
#define AIO_O_SA_6_inst_adr                                                          "0x0066"
#define AIO_O_SA_7_inst_adr                                                          "0x0067"
#define AIO_O_SA_8_inst_adr                                                          "0x0068"
#define AIO_O_SA_9_inst_adr                                                          "0x0069"
#define AIO_O_SA_10_inst_adr                                                          "0x006A"
#define AIO_O_SA_11_inst_adr                                                          "0x006B"
#define AIO_O_SA_0_inst                                                              0x0060
#define AIO_O_SA_1_inst                                                              0x0061
#define AIO_O_SA_2_inst                                                              0x0062
#define AIO_O_SA_3_inst                                                              0x0063
#define AIO_O_SA_4_inst                                                              0x0064
#define AIO_O_SA_5_inst                                                              0x0065
#define AIO_O_SA_6_inst                                                              0x0066
#define AIO_O_SA_7_inst                                                              0x0067
#define AIO_O_SA_8_inst                                                              0x0068
#define AIO_O_SA_9_inst                                                              0x0069
#define AIO_O_SA_10_inst                                                              0x006A
#define AIO_O_SA_11_inst                                                              0x006B
#define AIO_O_SA_addr_shift                                                          (3)
#define AIO_O_SA_addr_mask                                                           (0x7FFFFFF8)
#define AIO_O_SA_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_O_SA_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_O_SA_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_O_EA_0                                                                   0x180061B0
#define AIO_O_EA_1                                                                   0x180061B4
#define AIO_O_EA_2                                                                   0x180061B8
#define AIO_O_EA_3                                                                   0x180061BC
#define AIO_O_EA_4                                                                   0x180061C0
#define AIO_O_EA_5                                                                   0x180061C4
#define AIO_O_EA_6                                                                   0x180061C8
#define AIO_O_EA_7                                                                   0x180061CC
#define AIO_O_EA_8                                                                   0x180061D0
#define AIO_O_EA_9                                                                   0x180061D4
#define AIO_O_EA_10                                                                   0x180061D8
#define AIO_O_EA_11                                                                   0x180061DC
#define AIO_O_EA_0_reg_addr                                                          "0xB80061B0"
#define AIO_O_EA_1_reg_addr                                                          "0xB80061B4"
#define AIO_O_EA_2_reg_addr                                                          "0xB80061B8"
#define AIO_O_EA_3_reg_addr                                                          "0xB80061BC"
#define AIO_O_EA_4_reg_addr                                                          "0xB80061C0"
#define AIO_O_EA_5_reg_addr                                                          "0xB80061C4"
#define AIO_O_EA_6_reg_addr                                                          "0xB80061C8"
#define AIO_O_EA_7_reg_addr                                                          "0xB80061CC"
#define AIO_O_EA_8_reg_addr                                                          "0xB80061D0"
#define AIO_O_EA_9_reg_addr                                                          "0xB80061D4"
#define AIO_O_EA_10_reg_addr                                                          "0xB80061D8"
#define AIO_O_EA_11_reg_addr                                                          "0xB80061DC"
#define AIO_O_EA_0_reg                                                               0xB80061B0
#define AIO_O_EA_1_reg                                                               0xB80061B4
#define AIO_O_EA_2_reg                                                               0xB80061B8
#define AIO_O_EA_3_reg                                                               0xB80061BC
#define AIO_O_EA_4_reg                                                               0xB80061C0
#define AIO_O_EA_5_reg                                                               0xB80061C4
#define AIO_O_EA_6_reg                                                               0xB80061C8
#define AIO_O_EA_7_reg                                                               0xB80061CC
#define AIO_O_EA_8_reg                                                               0xB80061D0
#define AIO_O_EA_9_reg                                                               0xB80061D4
#define AIO_O_EA_10_reg                                                               0xB80061D8
#define AIO_O_EA_11_reg                                                               0xB80061DC
#define set_AIO_O_EA_0_reg(data)   (*((volatile unsigned int*) AIO_O_EA_0_reg)=data)
#define set_AIO_O_EA_1_reg(data)   (*((volatile unsigned int*) AIO_O_EA_1_reg)=data)
#define set_AIO_O_EA_2_reg(data)   (*((volatile unsigned int*) AIO_O_EA_2_reg)=data)
#define set_AIO_O_EA_3_reg(data)   (*((volatile unsigned int*) AIO_O_EA_3_reg)=data)
#define set_AIO_O_EA_4_reg(data)   (*((volatile unsigned int*) AIO_O_EA_4_reg)=data)
#define set_AIO_O_EA_5_reg(data)   (*((volatile unsigned int*) AIO_O_EA_5_reg)=data)
#define set_AIO_O_EA_6_reg(data)   (*((volatile unsigned int*) AIO_O_EA_6_reg)=data)
#define set_AIO_O_EA_7_reg(data)   (*((volatile unsigned int*) AIO_O_EA_7_reg)=data)
#define set_AIO_O_EA_8_reg(data)   (*((volatile unsigned int*) AIO_O_EA_8_reg)=data)
#define set_AIO_O_EA_9_reg(data)   (*((volatile unsigned int*) AIO_O_EA_9_reg)=data)
#define set_AIO_O_EA_10_reg(data)   (*((volatile unsigned int*) AIO_O_EA_10_reg)=data)
#define set_AIO_O_EA_11_reg(data)   (*((volatile unsigned int*) AIO_O_EA_11_reg)=data)
#define get_AIO_O_EA_0_reg   (*((volatile unsigned int*) AIO_O_EA_0_reg))
#define get_AIO_O_EA_1_reg   (*((volatile unsigned int*) AIO_O_EA_1_reg))
#define get_AIO_O_EA_2_reg   (*((volatile unsigned int*) AIO_O_EA_2_reg))
#define get_AIO_O_EA_3_reg   (*((volatile unsigned int*) AIO_O_EA_3_reg))
#define get_AIO_O_EA_4_reg   (*((volatile unsigned int*) AIO_O_EA_4_reg))
#define get_AIO_O_EA_5_reg   (*((volatile unsigned int*) AIO_O_EA_5_reg))
#define get_AIO_O_EA_6_reg   (*((volatile unsigned int*) AIO_O_EA_6_reg))
#define get_AIO_O_EA_7_reg   (*((volatile unsigned int*) AIO_O_EA_7_reg))
#define get_AIO_O_EA_8_reg   (*((volatile unsigned int*) AIO_O_EA_8_reg))
#define get_AIO_O_EA_9_reg   (*((volatile unsigned int*) AIO_O_EA_9_reg))
#define get_AIO_O_EA_10_reg   (*((volatile unsigned int*) AIO_O_EA_10_reg))
#define get_AIO_O_EA_11_reg   (*((volatile unsigned int*) AIO_O_EA_11_reg))
#define AIO_O_EA_0_inst_adr                                                          "0x006C"
#define AIO_O_EA_1_inst_adr                                                          "0x006D"
#define AIO_O_EA_2_inst_adr                                                          "0x006E"
#define AIO_O_EA_3_inst_adr                                                          "0x006F"
#define AIO_O_EA_4_inst_adr                                                          "0x0070"
#define AIO_O_EA_5_inst_adr                                                          "0x0071"
#define AIO_O_EA_6_inst_adr                                                          "0x0072"
#define AIO_O_EA_7_inst_adr                                                          "0x0073"
#define AIO_O_EA_8_inst_adr                                                          "0x0074"
#define AIO_O_EA_9_inst_adr                                                          "0x0075"
#define AIO_O_EA_10_inst_adr                                                          "0x0076"
#define AIO_O_EA_11_inst_adr                                                          "0x0077"
#define AIO_O_EA_0_inst                                                              0x006C
#define AIO_O_EA_1_inst                                                              0x006D
#define AIO_O_EA_2_inst                                                              0x006E
#define AIO_O_EA_3_inst                                                              0x006F
#define AIO_O_EA_4_inst                                                              0x0070
#define AIO_O_EA_5_inst                                                              0x0071
#define AIO_O_EA_6_inst                                                              0x0072
#define AIO_O_EA_7_inst                                                              0x0073
#define AIO_O_EA_8_inst                                                              0x0074
#define AIO_O_EA_9_inst                                                              0x0075
#define AIO_O_EA_10_inst                                                              0x0076
#define AIO_O_EA_11_inst                                                              0x0077
#define AIO_O_EA_addr_shift                                                          (3)
#define AIO_O_EA_addr_mask                                                           (0x7FFFFFF8)
#define AIO_O_EA_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_O_EA_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_O_EA_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_O_RP_0                                                                   0x180061E0
#define AIO_O_RP_1                                                                   0x180061E4
#define AIO_O_RP_2                                                                   0x180061E8
#define AIO_O_RP_3                                                                   0x180061EC
#define AIO_O_RP_4                                                                   0x180061F0
#define AIO_O_RP_5                                                                   0x180061F4
#define AIO_O_RP_6                                                                   0x180061F8
#define AIO_O_RP_7                                                                   0x180061FC
#define AIO_O_RP_8                                                                   0x18006200
#define AIO_O_RP_9                                                                   0x18006204
#define AIO_O_RP_10                                                                   0x18006208
#define AIO_O_RP_11                                                                   0x1800620C
#define AIO_O_RP_0_reg_addr                                                          "0xB80061E0"
#define AIO_O_RP_1_reg_addr                                                          "0xB80061E4"
#define AIO_O_RP_2_reg_addr                                                          "0xB80061E8"
#define AIO_O_RP_3_reg_addr                                                          "0xB80061EC"
#define AIO_O_RP_4_reg_addr                                                          "0xB80061F0"
#define AIO_O_RP_5_reg_addr                                                          "0xB80061F4"
#define AIO_O_RP_6_reg_addr                                                          "0xB80061F8"
#define AIO_O_RP_7_reg_addr                                                          "0xB80061FC"
#define AIO_O_RP_8_reg_addr                                                          "0xB8006200"
#define AIO_O_RP_9_reg_addr                                                          "0xB8006204"
#define AIO_O_RP_10_reg_addr                                                          "0xB8006208"
#define AIO_O_RP_11_reg_addr                                                          "0xB800620C"
#define AIO_O_RP_0_reg                                                               0xB80061E0
#define AIO_O_RP_1_reg                                                               0xB80061E4
#define AIO_O_RP_2_reg                                                               0xB80061E8
#define AIO_O_RP_3_reg                                                               0xB80061EC
#define AIO_O_RP_4_reg                                                               0xB80061F0
#define AIO_O_RP_5_reg                                                               0xB80061F4
#define AIO_O_RP_6_reg                                                               0xB80061F8
#define AIO_O_RP_7_reg                                                               0xB80061FC
#define AIO_O_RP_8_reg                                                               0xB8006200
#define AIO_O_RP_9_reg                                                               0xB8006204
#define AIO_O_RP_10_reg                                                               0xB8006208
#define AIO_O_RP_11_reg                                                               0xB800620C
#define set_AIO_O_RP_0_reg(data)   (*((volatile unsigned int*) AIO_O_RP_0_reg)=data)
#define set_AIO_O_RP_1_reg(data)   (*((volatile unsigned int*) AIO_O_RP_1_reg)=data)
#define set_AIO_O_RP_2_reg(data)   (*((volatile unsigned int*) AIO_O_RP_2_reg)=data)
#define set_AIO_O_RP_3_reg(data)   (*((volatile unsigned int*) AIO_O_RP_3_reg)=data)
#define set_AIO_O_RP_4_reg(data)   (*((volatile unsigned int*) AIO_O_RP_4_reg)=data)
#define set_AIO_O_RP_5_reg(data)   (*((volatile unsigned int*) AIO_O_RP_5_reg)=data)
#define set_AIO_O_RP_6_reg(data)   (*((volatile unsigned int*) AIO_O_RP_6_reg)=data)
#define set_AIO_O_RP_7_reg(data)   (*((volatile unsigned int*) AIO_O_RP_7_reg)=data)
#define set_AIO_O_RP_8_reg(data)   (*((volatile unsigned int*) AIO_O_RP_8_reg)=data)
#define set_AIO_O_RP_9_reg(data)   (*((volatile unsigned int*) AIO_O_RP_9_reg)=data)
#define set_AIO_O_RP_10_reg(data)   (*((volatile unsigned int*) AIO_O_RP_10_reg)=data)
#define set_AIO_O_RP_11_reg(data)   (*((volatile unsigned int*) AIO_O_RP_11_reg)=data)
#define get_AIO_O_RP_0_reg   (*((volatile unsigned int*) AIO_O_RP_0_reg))
#define get_AIO_O_RP_1_reg   (*((volatile unsigned int*) AIO_O_RP_1_reg))
#define get_AIO_O_RP_2_reg   (*((volatile unsigned int*) AIO_O_RP_2_reg))
#define get_AIO_O_RP_3_reg   (*((volatile unsigned int*) AIO_O_RP_3_reg))
#define get_AIO_O_RP_4_reg   (*((volatile unsigned int*) AIO_O_RP_4_reg))
#define get_AIO_O_RP_5_reg   (*((volatile unsigned int*) AIO_O_RP_5_reg))
#define get_AIO_O_RP_6_reg   (*((volatile unsigned int*) AIO_O_RP_6_reg))
#define get_AIO_O_RP_7_reg   (*((volatile unsigned int*) AIO_O_RP_7_reg))
#define get_AIO_O_RP_8_reg   (*((volatile unsigned int*) AIO_O_RP_8_reg))
#define get_AIO_O_RP_9_reg   (*((volatile unsigned int*) AIO_O_RP_9_reg))
#define get_AIO_O_RP_10_reg   (*((volatile unsigned int*) AIO_O_RP_10_reg))
#define get_AIO_O_RP_11_reg   (*((volatile unsigned int*) AIO_O_RP_11_reg))
#define AIO_O_RP_0_inst_adr                                                          "0x0078"
#define AIO_O_RP_1_inst_adr                                                          "0x0079"
#define AIO_O_RP_2_inst_adr                                                          "0x007A"
#define AIO_O_RP_3_inst_adr                                                          "0x007B"
#define AIO_O_RP_4_inst_adr                                                          "0x007C"
#define AIO_O_RP_5_inst_adr                                                          "0x007D"
#define AIO_O_RP_6_inst_adr                                                          "0x007E"
#define AIO_O_RP_7_inst_adr                                                          "0x007F"
#define AIO_O_RP_8_inst_adr                                                          "0x0080"
#define AIO_O_RP_9_inst_adr                                                          "0x0081"
#define AIO_O_RP_10_inst_adr                                                          "0x0082"
#define AIO_O_RP_11_inst_adr                                                          "0x0083"
#define AIO_O_RP_0_inst                                                              0x0078
#define AIO_O_RP_1_inst                                                              0x0079
#define AIO_O_RP_2_inst                                                              0x007A
#define AIO_O_RP_3_inst                                                              0x007B
#define AIO_O_RP_4_inst                                                              0x007C
#define AIO_O_RP_5_inst                                                              0x007D
#define AIO_O_RP_6_inst                                                              0x007E
#define AIO_O_RP_7_inst                                                              0x007F
#define AIO_O_RP_8_inst                                                              0x0080
#define AIO_O_RP_9_inst                                                              0x0081
#define AIO_O_RP_10_inst                                                              0x0082
#define AIO_O_RP_11_inst                                                              0x0083
#define AIO_O_RP_addr_shift                                                          (3)
#define AIO_O_RP_addr_mask                                                           (0x7FFFFFF8)
#define AIO_O_RP_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_O_RP_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_O_RP_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_O_WP_0                                                                   0x18006220
#define AIO_O_WP_1                                                                   0x18006224
#define AIO_O_WP_2                                                                   0x18006228
#define AIO_O_WP_3                                                                   0x1800622C
#define AIO_O_WP_4                                                                   0x18006230
#define AIO_O_WP_5                                                                   0x18006234
#define AIO_O_WP_6                                                                   0x18006238
#define AIO_O_WP_7                                                                   0x1800623C
#define AIO_O_WP_8                                                                   0x18006240
#define AIO_O_WP_9                                                                   0x18006244
#define AIO_O_WP_10                                                                   0x18006248
#define AIO_O_WP_11                                                                   0x1800624C
#define AIO_O_WP_0_reg_addr                                                          "0xB8006220"
#define AIO_O_WP_1_reg_addr                                                          "0xB8006224"
#define AIO_O_WP_2_reg_addr                                                          "0xB8006228"
#define AIO_O_WP_3_reg_addr                                                          "0xB800622C"
#define AIO_O_WP_4_reg_addr                                                          "0xB8006230"
#define AIO_O_WP_5_reg_addr                                                          "0xB8006234"
#define AIO_O_WP_6_reg_addr                                                          "0xB8006238"
#define AIO_O_WP_7_reg_addr                                                          "0xB800623C"
#define AIO_O_WP_8_reg_addr                                                          "0xB8006240"
#define AIO_O_WP_9_reg_addr                                                          "0xB8006244"
#define AIO_O_WP_10_reg_addr                                                          "0xB8006248"
#define AIO_O_WP_11_reg_addr                                                          "0xB800624C"
#define AIO_O_WP_0_reg                                                               0xB8006220
#define AIO_O_WP_1_reg                                                               0xB8006224
#define AIO_O_WP_2_reg                                                               0xB8006228
#define AIO_O_WP_3_reg                                                               0xB800622C
#define AIO_O_WP_4_reg                                                               0xB8006230
#define AIO_O_WP_5_reg                                                               0xB8006234
#define AIO_O_WP_6_reg                                                               0xB8006238
#define AIO_O_WP_7_reg                                                               0xB800623C
#define AIO_O_WP_8_reg                                                               0xB8006240
#define AIO_O_WP_9_reg                                                               0xB8006244
#define AIO_O_WP_10_reg                                                               0xB8006248
#define AIO_O_WP_11_reg                                                               0xB800624C
#define set_AIO_O_WP_0_reg(data)   (*((volatile unsigned int*) AIO_O_WP_0_reg)=data)
#define set_AIO_O_WP_1_reg(data)   (*((volatile unsigned int*) AIO_O_WP_1_reg)=data)
#define set_AIO_O_WP_2_reg(data)   (*((volatile unsigned int*) AIO_O_WP_2_reg)=data)
#define set_AIO_O_WP_3_reg(data)   (*((volatile unsigned int*) AIO_O_WP_3_reg)=data)
#define set_AIO_O_WP_4_reg(data)   (*((volatile unsigned int*) AIO_O_WP_4_reg)=data)
#define set_AIO_O_WP_5_reg(data)   (*((volatile unsigned int*) AIO_O_WP_5_reg)=data)
#define set_AIO_O_WP_6_reg(data)   (*((volatile unsigned int*) AIO_O_WP_6_reg)=data)
#define set_AIO_O_WP_7_reg(data)   (*((volatile unsigned int*) AIO_O_WP_7_reg)=data)
#define set_AIO_O_WP_8_reg(data)   (*((volatile unsigned int*) AIO_O_WP_8_reg)=data)
#define set_AIO_O_WP_9_reg(data)   (*((volatile unsigned int*) AIO_O_WP_9_reg)=data)
#define set_AIO_O_WP_10_reg(data)   (*((volatile unsigned int*) AIO_O_WP_10_reg)=data)
#define set_AIO_O_WP_11_reg(data)   (*((volatile unsigned int*) AIO_O_WP_11_reg)=data)
#define get_AIO_O_WP_0_reg   (*((volatile unsigned int*) AIO_O_WP_0_reg))
#define get_AIO_O_WP_1_reg   (*((volatile unsigned int*) AIO_O_WP_1_reg))
#define get_AIO_O_WP_2_reg   (*((volatile unsigned int*) AIO_O_WP_2_reg))
#define get_AIO_O_WP_3_reg   (*((volatile unsigned int*) AIO_O_WP_3_reg))
#define get_AIO_O_WP_4_reg   (*((volatile unsigned int*) AIO_O_WP_4_reg))
#define get_AIO_O_WP_5_reg   (*((volatile unsigned int*) AIO_O_WP_5_reg))
#define get_AIO_O_WP_6_reg   (*((volatile unsigned int*) AIO_O_WP_6_reg))
#define get_AIO_O_WP_7_reg   (*((volatile unsigned int*) AIO_O_WP_7_reg))
#define get_AIO_O_WP_8_reg   (*((volatile unsigned int*) AIO_O_WP_8_reg))
#define get_AIO_O_WP_9_reg   (*((volatile unsigned int*) AIO_O_WP_9_reg))
#define get_AIO_O_WP_10_reg   (*((volatile unsigned int*) AIO_O_WP_10_reg))
#define get_AIO_O_WP_11_reg   (*((volatile unsigned int*) AIO_O_WP_11_reg))
#define AIO_O_WP_0_inst_adr                                                          "0x0088"
#define AIO_O_WP_1_inst_adr                                                          "0x0089"
#define AIO_O_WP_2_inst_adr                                                          "0x008A"
#define AIO_O_WP_3_inst_adr                                                          "0x008B"
#define AIO_O_WP_4_inst_adr                                                          "0x008C"
#define AIO_O_WP_5_inst_adr                                                          "0x008D"
#define AIO_O_WP_6_inst_adr                                                          "0x008E"
#define AIO_O_WP_7_inst_adr                                                          "0x008F"
#define AIO_O_WP_8_inst_adr                                                          "0x0090"
#define AIO_O_WP_9_inst_adr                                                          "0x0091"
#define AIO_O_WP_10_inst_adr                                                          "0x0092"
#define AIO_O_WP_11_inst_adr                                                          "0x0093"
#define AIO_O_WP_0_inst                                                              0x0088
#define AIO_O_WP_1_inst                                                              0x0089
#define AIO_O_WP_2_inst                                                              0x008A
#define AIO_O_WP_3_inst                                                              0x008B
#define AIO_O_WP_4_inst                                                              0x008C
#define AIO_O_WP_5_inst                                                              0x008D
#define AIO_O_WP_6_inst                                                              0x008E
#define AIO_O_WP_7_inst                                                              0x008F
#define AIO_O_WP_8_inst                                                              0x0090
#define AIO_O_WP_9_inst                                                              0x0091
#define AIO_O_WP_10_inst                                                              0x0092
#define AIO_O_WP_11_inst                                                              0x0093
#define AIO_O_WP_addr_shift                                                          (3)
#define AIO_O_WP_addr_mask                                                           (0x7FFFFFF8)
#define AIO_O_WP_addr(data)                                                          (0x7FFFFFF8&((data)<<3))
#define AIO_O_WP_addr_src(data)                                                      ((0x7FFFFFF8&(data))>>3)
#define AIO_O_WP_get_addr(data)                                                      ((0x7FFFFFF8&(data))>>3)


#define AIO_O_DMA_STATUS                                                             0x18006250
#define AIO_O_DMA_STATUS_reg_addr                                                    "0xB8006250"
#define AIO_O_DMA_STATUS_reg                                                         0xB8006250
#define set_AIO_O_DMA_STATUS_reg(data)   (*((volatile unsigned int*) AIO_O_DMA_STATUS_reg)=data)
#define get_AIO_O_DMA_STATUS_reg   (*((volatile unsigned int*) AIO_O_DMA_STATUS_reg))
#define AIO_O_DMA_STATUS_inst_adr                                                    "0x0094"
#define AIO_O_DMA_STATUS_inst                                                        0x0094
#define AIO_O_DMA_STATUS_dmago_shift                                                 (0)
#define AIO_O_DMA_STATUS_dmago_mask                                                  (0x00000001)
#define AIO_O_DMA_STATUS_dmago(data)                                                 (0x00000001&((data)<<0))
#define AIO_O_DMA_STATUS_dmago_src(data)                                             ((0x00000001&(data))>>0)
#define AIO_O_DMA_STATUS_get_dmago(data)                                             ((0x00000001&(data))>>0)


#define AIO_I_GO                                                                     0x18006300
#define AIO_I_GO_reg_addr                                                            "0xB8006300"
#define AIO_I_GO_reg                                                                 0xB8006300
#define set_AIO_I_GO_reg(data)   (*((volatile unsigned int*) AIO_I_GO_reg)=data)
#define get_AIO_I_GO_reg   (*((volatile unsigned int*) AIO_I_GO_reg))
#define AIO_I_GO_inst_adr                                                            "0x00C0"
#define AIO_I_GO_inst                                                                0x00C0
#define AIO_I_GO_go0_shift                                                           (1)
#define AIO_I_GO_go0_mask                                                            (0x00000002)
#define AIO_I_GO_go0(data)                                                           (0x00000002&((data)<<1))
#define AIO_I_GO_go0_src(data)                                                       ((0x00000002&(data))>>1)
#define AIO_I_GO_get_go0(data)                                                       ((0x00000002&(data))>>1)


#define AIO_O_GO                                                                     0x18006304
#define AIO_O_GO_reg_addr                                                            "0xB8006304"
#define AIO_O_GO_reg                                                                 0xB8006304
#define set_AIO_O_GO_reg(data)   (*((volatile unsigned int*) AIO_O_GO_reg)=data)
#define get_AIO_O_GO_reg   (*((volatile unsigned int*) AIO_O_GO_reg))
#define AIO_O_GO_inst_adr                                                            "0x00C1"
#define AIO_O_GO_inst                                                                0x00C1
#define AIO_O_GO_goacdac_shift                                                       (3)
#define AIO_O_GO_goacdac_mask                                                        (0x00000008)
#define AIO_O_GO_goacdac(data)                                                       (0x00000008&((data)<<3))
#define AIO_O_GO_goacdac_src(data)                                                   ((0x00000008&(data))>>3)
#define AIO_O_GO_get_goacdac(data)                                                   ((0x00000008&(data))>>3)
#define AIO_O_GO_gohdmi_shift                                                        (2)
#define AIO_O_GO_gohdmi_mask                                                         (0x00000004)
#define AIO_O_GO_gohdmi(data)                                                        (0x00000004&((data)<<2))
#define AIO_O_GO_gohdmi_src(data)                                                    ((0x00000004&(data))>>2)
#define AIO_O_GO_get_gohdmi(data)                                                    ((0x00000004&(data))>>2)
#define AIO_O_GO_goi2s_shift                                                         (1)
#define AIO_O_GO_goi2s_mask                                                          (0x00000002)
#define AIO_O_GO_goi2s(data)                                                         (0x00000002&((data)<<1))
#define AIO_O_GO_goi2s_src(data)                                                     ((0x00000002&(data))>>1)
#define AIO_O_GO_get_goi2s(data)                                                     ((0x00000002&(data))>>1)
#define AIO_O_GO_gospf_shift                                                         (0)
#define AIO_O_GO_gospf_mask                                                          (0x00000001)
#define AIO_O_GO_gospf(data)                                                         (0x00000001&((data)<<0))
#define AIO_O_GO_gospf_src(data)                                                     ((0x00000001&(data))>>0)
#define AIO_O_GO_get_gospf(data)                                                     ((0x00000001&(data))>>0)


#define AIO_I_PAUSE                                                                  0x18006308
#define AIO_I_PAUSE_reg_addr                                                         "0xB8006308"
#define AIO_I_PAUSE_reg                                                              0xB8006308
#define set_AIO_I_PAUSE_reg(data)   (*((volatile unsigned int*) AIO_I_PAUSE_reg)=data)
#define get_AIO_I_PAUSE_reg   (*((volatile unsigned int*) AIO_I_PAUSE_reg))
#define AIO_I_PAUSE_inst_adr                                                         "0x00C2"
#define AIO_I_PAUSE_inst                                                             0x00C2
#define AIO_I_PAUSE_pause0_shift                                                     (0)
#define AIO_I_PAUSE_pause0_mask                                                      (0x00000001)
#define AIO_I_PAUSE_pause0(data)                                                     (0x00000001&((data)<<0))
#define AIO_I_PAUSE_pause0_src(data)                                                 ((0x00000001&(data))>>0)
#define AIO_I_PAUSE_get_pause0(data)                                                 ((0x00000001&(data))>>0)


#define AIO_O_PAUSE                                                                  0x1800630c
#define AIO_O_PAUSE_reg_addr                                                         "0xB800630C"
#define AIO_O_PAUSE_reg                                                              0xB800630C
#define set_AIO_O_PAUSE_reg(data)   (*((volatile unsigned int*) AIO_O_PAUSE_reg)=data)
#define get_AIO_O_PAUSE_reg   (*((volatile unsigned int*) AIO_O_PAUSE_reg))
#define AIO_O_PAUSE_inst_adr                                                         "0x00C3"
#define AIO_O_PAUSE_inst                                                             0x00C3
#define AIO_O_PAUSE_pause2_shift                                                     (2)
#define AIO_O_PAUSE_pause2_mask                                                      (0x00000004)
#define AIO_O_PAUSE_pause2(data)                                                     (0x00000004&((data)<<2))
#define AIO_O_PAUSE_pause2_src(data)                                                 ((0x00000004&(data))>>2)
#define AIO_O_PAUSE_get_pause2(data)                                                 ((0x00000004&(data))>>2)
#define AIO_O_PAUSE_pause1_shift                                                     (1)
#define AIO_O_PAUSE_pause1_mask                                                      (0x00000002)
#define AIO_O_PAUSE_pause1(data)                                                     (0x00000002&((data)<<1))
#define AIO_O_PAUSE_pause1_src(data)                                                 ((0x00000002&(data))>>1)
#define AIO_O_PAUSE_get_pause1(data)                                                 ((0x00000002&(data))>>1)
#define AIO_O_PAUSE_pause0_shift                                                     (0)
#define AIO_O_PAUSE_pause0_mask                                                      (0x00000001)
#define AIO_O_PAUSE_pause0(data)                                                     (0x00000001&((data)<<0))
#define AIO_O_PAUSE_pause0_src(data)                                                 ((0x00000001&(data))>>0)
#define AIO_O_PAUSE_get_pause0(data)                                                 ((0x00000001&(data))>>0)


#define AIO_I_I2S_DDRTHD                                                             0x18006310
#define AIO_I_I2S_DDRTHD_reg_addr                                                    "0xB8006310"
#define AIO_I_I2S_DDRTHD_reg                                                         0xB8006310
#define set_AIO_I_I2S_DDRTHD_reg(data)   (*((volatile unsigned int*) AIO_I_I2S_DDRTHD_reg)=data)
#define get_AIO_I_I2S_DDRTHD_reg   (*((volatile unsigned int*) AIO_I_I2S_DDRTHD_reg))
#define AIO_I_I2S_DDRTHD_inst_adr                                                    "0x00C4"
#define AIO_I_I2S_DDRTHD_inst                                                        0x00C4
#define AIO_I_I2S_DDRTHD_thd_shift                                                   (3)
#define AIO_I_I2S_DDRTHD_thd_mask                                                    (0x7FFFFFF8)
#define AIO_I_I2S_DDRTHD_thd(data)                                                   (0x7FFFFFF8&((data)<<3))
#define AIO_I_I2S_DDRTHD_thd_src(data)                                               ((0x7FFFFFF8&(data))>>3)
#define AIO_I_I2S_DDRTHD_get_thd(data)                                               ((0x7FFFFFF8&(data))>>3)


#define AIO_I_I2S_TIMCNT                                                             0x18006318
#define AIO_I_I2S_TIMCNT_reg_addr                                                    "0xB8006318"
#define AIO_I_I2S_TIMCNT_reg                                                         0xB8006318
#define set_AIO_I_I2S_TIMCNT_reg(data)   (*((volatile unsigned int*) AIO_I_I2S_TIMCNT_reg)=data)
#define get_AIO_I_I2S_TIMCNT_reg   (*((volatile unsigned int*) AIO_I_I2S_TIMCNT_reg))
#define AIO_I_I2S_TIMCNT_inst_adr                                                    "0x00C6"
#define AIO_I_I2S_TIMCNT_inst                                                        0x00C6
#define AIO_I_I2S_TIMCNT_count_shift                                                 (0)
#define AIO_I_I2S_TIMCNT_count_mask                                                  (0xFFFFFFFF)
#define AIO_I_I2S_TIMCNT_count(data)                                                 (0xFFFFFFFF&((data)<<0))
#define AIO_I_I2S_TIMCNT_count_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define AIO_I_I2S_TIMCNT_get_count(data)                                             ((0xFFFFFFFF&(data))>>0)


#define AIO_I_I2S_TIMTHD                                                             0x18006320
#define AIO_I_I2S_TIMTHD_reg_addr                                                    "0xB8006320"
#define AIO_I_I2S_TIMTHD_reg                                                         0xB8006320
#define set_AIO_I_I2S_TIMTHD_reg(data)   (*((volatile unsigned int*) AIO_I_I2S_TIMTHD_reg)=data)
#define get_AIO_I_I2S_TIMTHD_reg   (*((volatile unsigned int*) AIO_I_I2S_TIMTHD_reg))
#define AIO_I_I2S_TIMTHD_inst_adr                                                    "0x00C8"
#define AIO_I_I2S_TIMTHD_inst                                                        0x00C8
#define AIO_I_I2S_TIMTHD_thd_shift                                                   (0)
#define AIO_I_I2S_TIMTHD_thd_mask                                                    (0xFFFFFFFF)
#define AIO_I_I2S_TIMTHD_thd(data)                                                   (0xFFFFFFFF&((data)<<0))
#define AIO_I_I2S_TIMTHD_thd_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define AIO_I_I2S_TIMTHD_get_thd(data)                                               ((0xFFFFFFFF&(data))>>0)


#define AIO_I_INTE                                                                   0x18006330
#define AIO_I_INTE_reg_addr                                                          "0xB8006330"
#define AIO_I_INTE_reg                                                               0xB8006330
#define set_AIO_I_INTE_reg(data)   (*((volatile unsigned int*) AIO_I_INTE_reg)=data)
#define get_AIO_I_INTE_reg   (*((volatile unsigned int*) AIO_I_INTE_reg))
#define AIO_I_INTE_inst_adr                                                          "0x00CC"
#define AIO_I_INTE_inst                                                              0x00CC
#define AIO_I_INTE_timi2sinte_shift                                                  (25)
#define AIO_I_INTE_timi2sinte_mask                                                   (0x02000000)
#define AIO_I_INTE_timi2sinte(data)                                                  (0x02000000&((data)<<25))
#define AIO_I_INTE_timi2sinte_src(data)                                              ((0x02000000&(data))>>25)
#define AIO_I_INTE_get_timi2sinte(data)                                              ((0x02000000&(data))>>25)
#define AIO_I_INTE_thdinte0_shift                                                    (21)
#define AIO_I_INTE_thdinte0_mask                                                     (0x00200000)
#define AIO_I_INTE_thdinte0(data)                                                    (0x00200000&((data)<<21))
#define AIO_I_INTE_thdinte0_src(data)                                                ((0x00200000&(data))>>21)
#define AIO_I_INTE_get_thdinte0(data)                                                ((0x00200000&(data))>>21)
#define AIO_I_INTE_fifointe1_shift                                                   (10)
#define AIO_I_INTE_fifointe1_mask                                                    (0x00000400)
#define AIO_I_INTE_fifointe1(data)                                                   (0x00000400&((data)<<10))
#define AIO_I_INTE_fifointe1_src(data)                                               ((0x00000400&(data))>>10)
#define AIO_I_INTE_get_fifointe1(data)                                               ((0x00000400&(data))>>10)
#define AIO_I_INTE_fifointe0_shift                                                   (9)
#define AIO_I_INTE_fifointe0_mask                                                    (0x00000200)
#define AIO_I_INTE_fifointe0(data)                                                   (0x00000200&((data)<<9))
#define AIO_I_INTE_fifointe0_src(data)                                               ((0x00000200&(data))>>9)
#define AIO_I_INTE_get_fifointe0(data)                                               ((0x00000200&(data))>>9)
#define AIO_I_INTE_draminte1_shift                                                   (2)
#define AIO_I_INTE_draminte1_mask                                                    (0x00000004)
#define AIO_I_INTE_draminte1(data)                                                   (0x00000004&((data)<<2))
#define AIO_I_INTE_draminte1_src(data)                                               ((0x00000004&(data))>>2)
#define AIO_I_INTE_get_draminte1(data)                                               ((0x00000004&(data))>>2)
#define AIO_I_INTE_draminte0_shift                                                   (1)
#define AIO_I_INTE_draminte0_mask                                                    (0x00000002)
#define AIO_I_INTE_draminte0(data)                                                   (0x00000002&((data)<<1))
#define AIO_I_INTE_draminte0_src(data)                                               ((0x00000002&(data))>>1)
#define AIO_I_INTE_get_draminte0(data)                                               ((0x00000002&(data))>>1)
#define AIO_I_INTE_write_data_shift                                                  (0)
#define AIO_I_INTE_write_data_mask                                                   (0x00000001)
#define AIO_I_INTE_write_data(data)                                                  (0x00000001&((data)<<0))
#define AIO_I_INTE_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define AIO_I_INTE_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define AIO_I_INTS                                                                   0x18006334
#define AIO_I_INTS_reg_addr                                                          "0xB8006334"
#define AIO_I_INTS_reg                                                               0xB8006334
#define set_AIO_I_INTS_reg(data)   (*((volatile unsigned int*) AIO_I_INTS_reg)=data)
#define get_AIO_I_INTS_reg   (*((volatile unsigned int*) AIO_I_INTS_reg))
#define AIO_I_INTS_inst_adr                                                          "0x00CD"
#define AIO_I_INTS_inst                                                              0x00CD
#define AIO_I_INTS_timi2sints_shift                                                  (25)
#define AIO_I_INTS_timi2sints_mask                                                   (0x02000000)
#define AIO_I_INTS_timi2sints(data)                                                  (0x02000000&((data)<<25))
#define AIO_I_INTS_timi2sints_src(data)                                              ((0x02000000&(data))>>25)
#define AIO_I_INTS_get_timi2sints(data)                                              ((0x02000000&(data))>>25)
#define AIO_I_INTS_thdints0_shift                                                    (21)
#define AIO_I_INTS_thdints0_mask                                                     (0x00200000)
#define AIO_I_INTS_thdints0(data)                                                    (0x00200000&((data)<<21))
#define AIO_I_INTS_thdints0_src(data)                                                ((0x00200000&(data))>>21)
#define AIO_I_INTS_get_thdints0(data)                                                ((0x00200000&(data))>>21)
#define AIO_I_INTS_fifoints1_shift                                                   (10)
#define AIO_I_INTS_fifoints1_mask                                                    (0x00000400)
#define AIO_I_INTS_fifoints1(data)                                                   (0x00000400&((data)<<10))
#define AIO_I_INTS_fifoints1_src(data)                                               ((0x00000400&(data))>>10)
#define AIO_I_INTS_get_fifoints1(data)                                               ((0x00000400&(data))>>10)
#define AIO_I_INTS_fifoints0_shift                                                   (9)
#define AIO_I_INTS_fifoints0_mask                                                    (0x00000200)
#define AIO_I_INTS_fifoints0(data)                                                   (0x00000200&((data)<<9))
#define AIO_I_INTS_fifoints0_src(data)                                               ((0x00000200&(data))>>9)
#define AIO_I_INTS_get_fifoints0(data)                                               ((0x00000200&(data))>>9)
#define AIO_I_INTS_dramints1_shift                                                   (2)
#define AIO_I_INTS_dramints1_mask                                                    (0x00000004)
#define AIO_I_INTS_dramints1(data)                                                   (0x00000004&((data)<<2))
#define AIO_I_INTS_dramints1_src(data)                                               ((0x00000004&(data))>>2)
#define AIO_I_INTS_get_dramints1(data)                                               ((0x00000004&(data))>>2)
#define AIO_I_INTS_dramints0_shift                                                   (1)
#define AIO_I_INTS_dramints0_mask                                                    (0x00000002)
#define AIO_I_INTS_dramints0(data)                                                   (0x00000002&((data)<<1))
#define AIO_I_INTS_dramints0_src(data)                                               ((0x00000002&(data))>>1)
#define AIO_I_INTS_get_dramints0(data)                                               ((0x00000002&(data))>>1)
#define AIO_I_INTS_write_data_shift                                                  (0)
#define AIO_I_INTS_write_data_mask                                                   (0x00000001)
#define AIO_I_INTS_write_data(data)                                                  (0x00000001&((data)<<0))
#define AIO_I_INTS_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define AIO_I_INTS_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define AIO_O_PCM_DDRTHD                                                             0x18006400
#define AIO_O_PCM_DDRTHD_reg_addr                                                    "0xB8006400"
#define AIO_O_PCM_DDRTHD_reg                                                         0xB8006400
#define set_AIO_O_PCM_DDRTHD_reg(data)   (*((volatile unsigned int*) AIO_O_PCM_DDRTHD_reg)=data)
#define get_AIO_O_PCM_DDRTHD_reg   (*((volatile unsigned int*) AIO_O_PCM_DDRTHD_reg))
#define AIO_O_PCM_DDRTHD_inst_adr                                                    "0x0000"
#define AIO_O_PCM_DDRTHD_inst                                                        0x0000
#define AIO_O_PCM_DDRTHD_thd_shift                                                   (3)
#define AIO_O_PCM_DDRTHD_thd_mask                                                    (0x7FFFFFF8)
#define AIO_O_PCM_DDRTHD_thd(data)                                                   (0x7FFFFFF8&((data)<<3))
#define AIO_O_PCM_DDRTHD_thd_src(data)                                               ((0x7FFFFFF8&(data))>>3)
#define AIO_O_PCM_DDRTHD_get_thd(data)                                               ((0x7FFFFFF8&(data))>>3)


#define AIO_O_PCM_TIMCNT                                                             0x18006404
#define AIO_O_PCM_TIMCNT_reg_addr                                                    "0xB8006404"
#define AIO_O_PCM_TIMCNT_reg                                                         0xB8006404
#define set_AIO_O_PCM_TIMCNT_reg(data)   (*((volatile unsigned int*) AIO_O_PCM_TIMCNT_reg)=data)
#define get_AIO_O_PCM_TIMCNT_reg   (*((volatile unsigned int*) AIO_O_PCM_TIMCNT_reg))
#define AIO_O_PCM_TIMCNT_inst_adr                                                    "0x0001"
#define AIO_O_PCM_TIMCNT_inst                                                        0x0001
#define AIO_O_PCM_TIMCNT_count_shift                                                 (0)
#define AIO_O_PCM_TIMCNT_count_mask                                                  (0xFFFFFFFF)
#define AIO_O_PCM_TIMCNT_count(data)                                                 (0xFFFFFFFF&((data)<<0))
#define AIO_O_PCM_TIMCNT_count_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define AIO_O_PCM_TIMCNT_get_count(data)                                             ((0xFFFFFFFF&(data))>>0)


#define AIO_O_PCM_TIMTHD                                                             0x18006408
#define AIO_O_PCM_TIMTHD_reg_addr                                                    "0xB8006408"
#define AIO_O_PCM_TIMTHD_reg                                                         0xB8006408
#define set_AIO_O_PCM_TIMTHD_reg(data)   (*((volatile unsigned int*) AIO_O_PCM_TIMTHD_reg)=data)
#define get_AIO_O_PCM_TIMTHD_reg   (*((volatile unsigned int*) AIO_O_PCM_TIMTHD_reg))
#define AIO_O_PCM_TIMTHD_inst_adr                                                    "0x0002"
#define AIO_O_PCM_TIMTHD_inst                                                        0x0002
#define AIO_O_PCM_TIMTHD_thd_shift                                                   (0)
#define AIO_O_PCM_TIMTHD_thd_mask                                                    (0xFFFFFFFF)
#define AIO_O_PCM_TIMTHD_thd(data)                                                   (0xFFFFFFFF&((data)<<0))
#define AIO_O_PCM_TIMTHD_thd_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define AIO_O_PCM_TIMTHD_get_thd(data)                                               ((0xFFFFFFFF&(data))>>0)


#define AIO_O_RAW_DDRTHD                                                             0x18006410
#define AIO_O_RAW_DDRTHD_reg_addr                                                    "0xB8006410"
#define AIO_O_RAW_DDRTHD_reg                                                         0xB8006410
#define set_AIO_O_RAW_DDRTHD_reg(data)   (*((volatile unsigned int*) AIO_O_RAW_DDRTHD_reg)=data)
#define get_AIO_O_RAW_DDRTHD_reg   (*((volatile unsigned int*) AIO_O_RAW_DDRTHD_reg))
#define AIO_O_RAW_DDRTHD_inst_adr                                                    "0x0004"
#define AIO_O_RAW_DDRTHD_inst                                                        0x0004
#define AIO_O_RAW_DDRTHD_thd_shift                                                   (3)
#define AIO_O_RAW_DDRTHD_thd_mask                                                    (0x7FFFFFF8)
#define AIO_O_RAW_DDRTHD_thd(data)                                                   (0x7FFFFFF8&((data)<<3))
#define AIO_O_RAW_DDRTHD_thd_src(data)                                               ((0x7FFFFFF8&(data))>>3)
#define AIO_O_RAW_DDRTHD_get_thd(data)                                               ((0x7FFFFFF8&(data))>>3)


#define AIO_O_RAW_TIMCNT                                                             0x18006414
#define AIO_O_RAW_TIMCNT_reg_addr                                                    "0xB8006414"
#define AIO_O_RAW_TIMCNT_reg                                                         0xB8006414
#define set_AIO_O_RAW_TIMCNT_reg(data)   (*((volatile unsigned int*) AIO_O_RAW_TIMCNT_reg)=data)
#define get_AIO_O_RAW_TIMCNT_reg   (*((volatile unsigned int*) AIO_O_RAW_TIMCNT_reg))
#define AIO_O_RAW_TIMCNT_inst_adr                                                    "0x0005"
#define AIO_O_RAW_TIMCNT_inst                                                        0x0005
#define AIO_O_RAW_TIMCNT_count_shift                                                 (0)
#define AIO_O_RAW_TIMCNT_count_mask                                                  (0xFFFFFFFF)
#define AIO_O_RAW_TIMCNT_count(data)                                                 (0xFFFFFFFF&((data)<<0))
#define AIO_O_RAW_TIMCNT_count_src(data)                                             ((0xFFFFFFFF&(data))>>0)
#define AIO_O_RAW_TIMCNT_get_count(data)                                             ((0xFFFFFFFF&(data))>>0)


#define AIO_O_RAW_TIMTHD                                                             0x18006418
#define AIO_O_RAW_TIMTHD_reg_addr                                                    "0xB8006418"
#define AIO_O_RAW_TIMTHD_reg                                                         0xB8006418
#define set_AIO_O_RAW_TIMTHD_reg(data)   (*((volatile unsigned int*) AIO_O_RAW_TIMTHD_reg)=data)
#define get_AIO_O_RAW_TIMTHD_reg   (*((volatile unsigned int*) AIO_O_RAW_TIMTHD_reg))
#define AIO_O_RAW_TIMTHD_inst_adr                                                    "0x0006"
#define AIO_O_RAW_TIMTHD_inst                                                        0x0006
#define AIO_O_RAW_TIMTHD_thd_shift                                                   (0)
#define AIO_O_RAW_TIMTHD_thd_mask                                                    (0xFFFFFFFF)
#define AIO_O_RAW_TIMTHD_thd(data)                                                   (0xFFFFFFFF&((data)<<0))
#define AIO_O_RAW_TIMTHD_thd_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define AIO_O_RAW_TIMTHD_get_thd(data)                                               ((0xFFFFFFFF&(data))>>0)


#define AIO_O_HDMI_DDRTHD                                                            0x18006420
#define AIO_O_HDMI_DDRTHD_reg_addr                                                   "0xB8006420"
#define AIO_O_HDMI_DDRTHD_reg                                                        0xB8006420
#define set_AIO_O_HDMI_DDRTHD_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_DDRTHD_reg)=data)
#define get_AIO_O_HDMI_DDRTHD_reg   (*((volatile unsigned int*) AIO_O_HDMI_DDRTHD_reg))
#define AIO_O_HDMI_DDRTHD_inst_adr                                                   "0x0008"
#define AIO_O_HDMI_DDRTHD_inst                                                       0x0008
#define AIO_O_HDMI_DDRTHD_thd_shift                                                  (3)
#define AIO_O_HDMI_DDRTHD_thd_mask                                                   (0x7FFFFFF8)
#define AIO_O_HDMI_DDRTHD_thd(data)                                                  (0x7FFFFFF8&((data)<<3))
#define AIO_O_HDMI_DDRTHD_thd_src(data)                                              ((0x7FFFFFF8&(data))>>3)
#define AIO_O_HDMI_DDRTHD_get_thd(data)                                              ((0x7FFFFFF8&(data))>>3)


#define AIO_O_HDMI_TIMCNT                                                            0x18006424
#define AIO_O_HDMI_TIMCNT_reg_addr                                                   "0xB8006424"
#define AIO_O_HDMI_TIMCNT_reg                                                        0xB8006424
#define set_AIO_O_HDMI_TIMCNT_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_TIMCNT_reg)=data)
#define get_AIO_O_HDMI_TIMCNT_reg   (*((volatile unsigned int*) AIO_O_HDMI_TIMCNT_reg))
#define AIO_O_HDMI_TIMCNT_inst_adr                                                   "0x0009"
#define AIO_O_HDMI_TIMCNT_inst                                                       0x0009
#define AIO_O_HDMI_TIMCNT_count_shift                                                (0)
#define AIO_O_HDMI_TIMCNT_count_mask                                                 (0xFFFFFFFF)
#define AIO_O_HDMI_TIMCNT_count(data)                                                (0xFFFFFFFF&((data)<<0))
#define AIO_O_HDMI_TIMCNT_count_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define AIO_O_HDMI_TIMCNT_get_count(data)                                            ((0xFFFFFFFF&(data))>>0)


#define AIO_O_HDMI_TIMTHD                                                            0x18006428
#define AIO_O_HDMI_TIMTHD_reg_addr                                                   "0xB8006428"
#define AIO_O_HDMI_TIMTHD_reg                                                        0xB8006428
#define set_AIO_O_HDMI_TIMTHD_reg(data)   (*((volatile unsigned int*) AIO_O_HDMI_TIMTHD_reg)=data)
#define get_AIO_O_HDMI_TIMTHD_reg   (*((volatile unsigned int*) AIO_O_HDMI_TIMTHD_reg))
#define AIO_O_HDMI_TIMTHD_inst_adr                                                   "0x000A"
#define AIO_O_HDMI_TIMTHD_inst                                                       0x000A
#define AIO_O_HDMI_TIMTHD_thd_shift                                                  (0)
#define AIO_O_HDMI_TIMTHD_thd_mask                                                   (0xFFFFFFFF)
#define AIO_O_HDMI_TIMTHD_thd(data)                                                  (0xFFFFFFFF&((data)<<0))
#define AIO_O_HDMI_TIMTHD_thd_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define AIO_O_HDMI_TIMTHD_get_thd(data)                                              ((0xFFFFFFFF&(data))>>0)


#define AIO_O_INTE                                                                   0x18006440
#define AIO_O_INTE_reg_addr                                                          "0xB8006440"
#define AIO_O_INTE_reg                                                               0xB8006440
#define set_AIO_O_INTE_reg(data)   (*((volatile unsigned int*) AIO_O_INTE_reg)=data)
#define get_AIO_O_INTE_reg   (*((volatile unsigned int*) AIO_O_INTE_reg))
#define AIO_O_INTE_inst_adr                                                          "0x0010"
#define AIO_O_INTE_inst                                                              0x0010
#define AIO_O_INTE_draminte11_shift                                                  (30)
#define AIO_O_INTE_draminte11_mask                                                   (0x40000000)
#define AIO_O_INTE_draminte11(data)                                                  (0x40000000&((data)<<30))
#define AIO_O_INTE_draminte11_src(data)                                              ((0x40000000&(data))>>30)
#define AIO_O_INTE_get_draminte11(data)                                              ((0x40000000&(data))>>30)
#define AIO_O_INTE_draminte10_shift                                                  (29)
#define AIO_O_INTE_draminte10_mask                                                   (0x20000000)
#define AIO_O_INTE_draminte10(data)                                                  (0x20000000&((data)<<29))
#define AIO_O_INTE_draminte10_src(data)                                              ((0x20000000&(data))>>29)
#define AIO_O_INTE_get_draminte10(data)                                              ((0x20000000&(data))>>29)
#define AIO_O_INTE_draminte9_shift                                                   (28)
#define AIO_O_INTE_draminte9_mask                                                    (0x10000000)
#define AIO_O_INTE_draminte9(data)                                                   (0x10000000&((data)<<28))
#define AIO_O_INTE_draminte9_src(data)                                               ((0x10000000&(data))>>28)
#define AIO_O_INTE_get_draminte9(data)                                               ((0x10000000&(data))>>28)
#define AIO_O_INTE_draminte8_shift                                                   (27)
#define AIO_O_INTE_draminte8_mask                                                    (0x08000000)
#define AIO_O_INTE_draminte8(data)                                                   (0x08000000&((data)<<27))
#define AIO_O_INTE_draminte8_src(data)                                               ((0x08000000&(data))>>27)
#define AIO_O_INTE_get_draminte8(data)                                               ((0x08000000&(data))>>27)
#define AIO_O_INTE_draminte7_shift                                                   (26)
#define AIO_O_INTE_draminte7_mask                                                    (0x04000000)
#define AIO_O_INTE_draminte7(data)                                                   (0x04000000&((data)<<26))
#define AIO_O_INTE_draminte7_src(data)                                               ((0x04000000&(data))>>26)
#define AIO_O_INTE_get_draminte7(data)                                               ((0x04000000&(data))>>26)
#define AIO_O_INTE_draminte6_shift                                                   (25)
#define AIO_O_INTE_draminte6_mask                                                    (0x02000000)
#define AIO_O_INTE_draminte6(data)                                                   (0x02000000&((data)<<25))
#define AIO_O_INTE_draminte6_src(data)                                               ((0x02000000&(data))>>25)
#define AIO_O_INTE_get_draminte6(data)                                               ((0x02000000&(data))>>25)
#define AIO_O_INTE_draminte5_shift                                                   (24)
#define AIO_O_INTE_draminte5_mask                                                    (0x01000000)
#define AIO_O_INTE_draminte5(data)                                                   (0x01000000&((data)<<24))
#define AIO_O_INTE_draminte5_src(data)                                               ((0x01000000&(data))>>24)
#define AIO_O_INTE_get_draminte5(data)                                               ((0x01000000&(data))>>24)
#define AIO_O_INTE_draminte4_shift                                                   (23)
#define AIO_O_INTE_draminte4_mask                                                    (0x00800000)
#define AIO_O_INTE_draminte4(data)                                                   (0x00800000&((data)<<23))
#define AIO_O_INTE_draminte4_src(data)                                               ((0x00800000&(data))>>23)
#define AIO_O_INTE_get_draminte4(data)                                               ((0x00800000&(data))>>23)
#define AIO_O_INTE_fifointe11_shift                                                  (22)
#define AIO_O_INTE_fifointe11_mask                                                   (0x00400000)
#define AIO_O_INTE_fifointe11(data)                                                  (0x00400000&((data)<<22))
#define AIO_O_INTE_fifointe11_src(data)                                              ((0x00400000&(data))>>22)
#define AIO_O_INTE_get_fifointe11(data)                                              ((0x00400000&(data))>>22)
#define AIO_O_INTE_fifointe10_shift                                                  (21)
#define AIO_O_INTE_fifointe10_mask                                                   (0x00200000)
#define AIO_O_INTE_fifointe10(data)                                                  (0x00200000&((data)<<21))
#define AIO_O_INTE_fifointe10_src(data)                                              ((0x00200000&(data))>>21)
#define AIO_O_INTE_get_fifointe10(data)                                              ((0x00200000&(data))>>21)
#define AIO_O_INTE_fifointe9_shift                                                   (20)
#define AIO_O_INTE_fifointe9_mask                                                    (0x00100000)
#define AIO_O_INTE_fifointe9(data)                                                   (0x00100000&((data)<<20))
#define AIO_O_INTE_fifointe9_src(data)                                               ((0x00100000&(data))>>20)
#define AIO_O_INTE_get_fifointe9(data)                                               ((0x00100000&(data))>>20)
#define AIO_O_INTE_fifointe8_shift                                                   (19)
#define AIO_O_INTE_fifointe8_mask                                                    (0x00080000)
#define AIO_O_INTE_fifointe8(data)                                                   (0x00080000&((data)<<19))
#define AIO_O_INTE_fifointe8_src(data)                                               ((0x00080000&(data))>>19)
#define AIO_O_INTE_get_fifointe8(data)                                               ((0x00080000&(data))>>19)
#define AIO_O_INTE_fifointe7_shift                                                   (18)
#define AIO_O_INTE_fifointe7_mask                                                    (0x00040000)
#define AIO_O_INTE_fifointe7(data)                                                   (0x00040000&((data)<<18))
#define AIO_O_INTE_fifointe7_src(data)                                               ((0x00040000&(data))>>18)
#define AIO_O_INTE_get_fifointe7(data)                                               ((0x00040000&(data))>>18)
#define AIO_O_INTE_fifointe6_shift                                                   (17)
#define AIO_O_INTE_fifointe6_mask                                                    (0x00020000)
#define AIO_O_INTE_fifointe6(data)                                                   (0x00020000&((data)<<17))
#define AIO_O_INTE_fifointe6_src(data)                                               ((0x00020000&(data))>>17)
#define AIO_O_INTE_get_fifointe6(data)                                               ((0x00020000&(data))>>17)
#define AIO_O_INTE_fifointe5_shift                                                   (16)
#define AIO_O_INTE_fifointe5_mask                                                    (0x00010000)
#define AIO_O_INTE_fifointe5(data)                                                   (0x00010000&((data)<<16))
#define AIO_O_INTE_fifointe5_src(data)                                               ((0x00010000&(data))>>16)
#define AIO_O_INTE_get_fifointe5(data)                                               ((0x00010000&(data))>>16)
#define AIO_O_INTE_fifointe4_shift                                                   (15)
#define AIO_O_INTE_fifointe4_mask                                                    (0x00008000)
#define AIO_O_INTE_fifointe4(data)                                                   (0x00008000&((data)<<15))
#define AIO_O_INTE_fifointe4_src(data)                                               ((0x00008000&(data))>>15)
#define AIO_O_INTE_get_fifointe4(data)                                               ((0x00008000&(data))>>15)
#define AIO_O_INTE_pcmtiminte_shift                                                  (14)
#define AIO_O_INTE_pcmtiminte_mask                                                   (0x00004000)
#define AIO_O_INTE_pcmtiminte(data)                                                  (0x00004000&((data)<<14))
#define AIO_O_INTE_pcmtiminte_src(data)                                              ((0x00004000&(data))>>14)
#define AIO_O_INTE_get_pcmtiminte(data)                                              ((0x00004000&(data))>>14)
#define AIO_O_INTE_rawtiminte_shift                                                  (13)
#define AIO_O_INTE_rawtiminte_mask                                                   (0x00002000)
#define AIO_O_INTE_rawtiminte(data)                                                  (0x00002000&((data)<<13))
#define AIO_O_INTE_rawtiminte_src(data)                                              ((0x00002000&(data))>>13)
#define AIO_O_INTE_get_rawtiminte(data)                                              ((0x00002000&(data))>>13)
#define AIO_O_INTE_hdmitiminte_shift                                                 (12)
#define AIO_O_INTE_hdmitiminte_mask                                                  (0x00001000)
#define AIO_O_INTE_hdmitiminte(data)                                                 (0x00001000&((data)<<12))
#define AIO_O_INTE_hdmitiminte_src(data)                                             ((0x00001000&(data))>>12)
#define AIO_O_INTE_get_hdmitiminte(data)                                             ((0x00001000&(data))>>12)
#define AIO_O_INTE_pcmthdinte_shift                                                  (11)
#define AIO_O_INTE_pcmthdinte_mask                                                   (0x00000800)
#define AIO_O_INTE_pcmthdinte(data)                                                  (0x00000800&((data)<<11))
#define AIO_O_INTE_pcmthdinte_src(data)                                              ((0x00000800&(data))>>11)
#define AIO_O_INTE_get_pcmthdinte(data)                                              ((0x00000800&(data))>>11)
#define AIO_O_INTE_rawthdinte_shift                                                  (10)
#define AIO_O_INTE_rawthdinte_mask                                                   (0x00000400)
#define AIO_O_INTE_rawthdinte(data)                                                  (0x00000400&((data)<<10))
#define AIO_O_INTE_rawthdinte_src(data)                                              ((0x00000400&(data))>>10)
#define AIO_O_INTE_get_rawthdinte(data)                                              ((0x00000400&(data))>>10)
#define AIO_O_INTE_hdmithdinte_shift                                                 (9)
#define AIO_O_INTE_hdmithdinte_mask                                                  (0x00000200)
#define AIO_O_INTE_hdmithdinte(data)                                                 (0x00000200&((data)<<9))
#define AIO_O_INTE_hdmithdinte_src(data)                                             ((0x00000200&(data))>>9)
#define AIO_O_INTE_get_hdmithdinte(data)                                             ((0x00000200&(data))>>9)
#define AIO_O_INTE_draminte3_shift                                                   (8)
#define AIO_O_INTE_draminte3_mask                                                    (0x00000100)
#define AIO_O_INTE_draminte3(data)                                                   (0x00000100&((data)<<8))
#define AIO_O_INTE_draminte3_src(data)                                               ((0x00000100&(data))>>8)
#define AIO_O_INTE_get_draminte3(data)                                               ((0x00000100&(data))>>8)
#define AIO_O_INTE_draminte2_shift                                                   (7)
#define AIO_O_INTE_draminte2_mask                                                    (0x00000080)
#define AIO_O_INTE_draminte2(data)                                                   (0x00000080&((data)<<7))
#define AIO_O_INTE_draminte2_src(data)                                               ((0x00000080&(data))>>7)
#define AIO_O_INTE_get_draminte2(data)                                               ((0x00000080&(data))>>7)
#define AIO_O_INTE_draminte1_shift                                                   (6)
#define AIO_O_INTE_draminte1_mask                                                    (0x00000040)
#define AIO_O_INTE_draminte1(data)                                                   (0x00000040&((data)<<6))
#define AIO_O_INTE_draminte1_src(data)                                               ((0x00000040&(data))>>6)
#define AIO_O_INTE_get_draminte1(data)                                               ((0x00000040&(data))>>6)
#define AIO_O_INTE_draminte0_shift                                                   (5)
#define AIO_O_INTE_draminte0_mask                                                    (0x00000020)
#define AIO_O_INTE_draminte0(data)                                                   (0x00000020&((data)<<5))
#define AIO_O_INTE_draminte0_src(data)                                               ((0x00000020&(data))>>5)
#define AIO_O_INTE_get_draminte0(data)                                               ((0x00000020&(data))>>5)
#define AIO_O_INTE_fifointe3_shift                                                   (4)
#define AIO_O_INTE_fifointe3_mask                                                    (0x00000010)
#define AIO_O_INTE_fifointe3(data)                                                   (0x00000010&((data)<<4))
#define AIO_O_INTE_fifointe3_src(data)                                               ((0x00000010&(data))>>4)
#define AIO_O_INTE_get_fifointe3(data)                                               ((0x00000010&(data))>>4)
#define AIO_O_INTE_fifointe2_shift                                                   (3)
#define AIO_O_INTE_fifointe2_mask                                                    (0x00000008)
#define AIO_O_INTE_fifointe2(data)                                                   (0x00000008&((data)<<3))
#define AIO_O_INTE_fifointe2_src(data)                                               ((0x00000008&(data))>>3)
#define AIO_O_INTE_get_fifointe2(data)                                               ((0x00000008&(data))>>3)
#define AIO_O_INTE_fifointe1_shift                                                   (2)
#define AIO_O_INTE_fifointe1_mask                                                    (0x00000004)
#define AIO_O_INTE_fifointe1(data)                                                   (0x00000004&((data)<<2))
#define AIO_O_INTE_fifointe1_src(data)                                               ((0x00000004&(data))>>2)
#define AIO_O_INTE_get_fifointe1(data)                                               ((0x00000004&(data))>>2)
#define AIO_O_INTE_fifointe0_shift                                                   (1)
#define AIO_O_INTE_fifointe0_mask                                                    (0x00000002)
#define AIO_O_INTE_fifointe0(data)                                                   (0x00000002&((data)<<1))
#define AIO_O_INTE_fifointe0_src(data)                                               ((0x00000002&(data))>>1)
#define AIO_O_INTE_get_fifointe0(data)                                               ((0x00000002&(data))>>1)
#define AIO_O_INTE_write_data_shift                                                  (0)
#define AIO_O_INTE_write_data_mask                                                   (0x00000001)
#define AIO_O_INTE_write_data(data)                                                  (0x00000001&((data)<<0))
#define AIO_O_INTE_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define AIO_O_INTE_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define AIO_O_INTS                                                                   0x18006444
#define AIO_O_INTS_reg_addr                                                          "0xB8006444"
#define AIO_O_INTS_reg                                                               0xB8006444
#define set_AIO_O_INTS_reg(data)   (*((volatile unsigned int*) AIO_O_INTS_reg)=data)
#define get_AIO_O_INTS_reg   (*((volatile unsigned int*) AIO_O_INTS_reg))
#define AIO_O_INTS_inst_adr                                                          "0x0011"
#define AIO_O_INTS_inst                                                              0x0011
#define AIO_O_INTS_dramints11_shift                                                  (30)
#define AIO_O_INTS_dramints11_mask                                                   (0x40000000)
#define AIO_O_INTS_dramints11(data)                                                  (0x40000000&((data)<<30))
#define AIO_O_INTS_dramints11_src(data)                                              ((0x40000000&(data))>>30)
#define AIO_O_INTS_get_dramints11(data)                                              ((0x40000000&(data))>>30)
#define AIO_O_INTS_dramints10_shift                                                  (29)
#define AIO_O_INTS_dramints10_mask                                                   (0x20000000)
#define AIO_O_INTS_dramints10(data)                                                  (0x20000000&((data)<<29))
#define AIO_O_INTS_dramints10_src(data)                                              ((0x20000000&(data))>>29)
#define AIO_O_INTS_get_dramints10(data)                                              ((0x20000000&(data))>>29)
#define AIO_O_INTS_dramints9_shift                                                   (28)
#define AIO_O_INTS_dramints9_mask                                                    (0x10000000)
#define AIO_O_INTS_dramints9(data)                                                   (0x10000000&((data)<<28))
#define AIO_O_INTS_dramints9_src(data)                                               ((0x10000000&(data))>>28)
#define AIO_O_INTS_get_dramints9(data)                                               ((0x10000000&(data))>>28)
#define AIO_O_INTS_dramints8_shift                                                   (27)
#define AIO_O_INTS_dramints8_mask                                                    (0x08000000)
#define AIO_O_INTS_dramints8(data)                                                   (0x08000000&((data)<<27))
#define AIO_O_INTS_dramints8_src(data)                                               ((0x08000000&(data))>>27)
#define AIO_O_INTS_get_dramints8(data)                                               ((0x08000000&(data))>>27)
#define AIO_O_INTS_dramints7_shift                                                   (26)
#define AIO_O_INTS_dramints7_mask                                                    (0x04000000)
#define AIO_O_INTS_dramints7(data)                                                   (0x04000000&((data)<<26))
#define AIO_O_INTS_dramints7_src(data)                                               ((0x04000000&(data))>>26)
#define AIO_O_INTS_get_dramints7(data)                                               ((0x04000000&(data))>>26)
#define AIO_O_INTS_dramints6_shift                                                   (25)
#define AIO_O_INTS_dramints6_mask                                                    (0x02000000)
#define AIO_O_INTS_dramints6(data)                                                   (0x02000000&((data)<<25))
#define AIO_O_INTS_dramints6_src(data)                                               ((0x02000000&(data))>>25)
#define AIO_O_INTS_get_dramints6(data)                                               ((0x02000000&(data))>>25)
#define AIO_O_INTS_dramints5_shift                                                   (24)
#define AIO_O_INTS_dramints5_mask                                                    (0x01000000)
#define AIO_O_INTS_dramints5(data)                                                   (0x01000000&((data)<<24))
#define AIO_O_INTS_dramints5_src(data)                                               ((0x01000000&(data))>>24)
#define AIO_O_INTS_get_dramints5(data)                                               ((0x01000000&(data))>>24)
#define AIO_O_INTS_dramints4_shift                                                   (23)
#define AIO_O_INTS_dramints4_mask                                                    (0x00800000)
#define AIO_O_INTS_dramints4(data)                                                   (0x00800000&((data)<<23))
#define AIO_O_INTS_dramints4_src(data)                                               ((0x00800000&(data))>>23)
#define AIO_O_INTS_get_dramints4(data)                                               ((0x00800000&(data))>>23)
#define AIO_O_INTS_fifoints11_shift                                                  (22)
#define AIO_O_INTS_fifoints11_mask                                                   (0x00400000)
#define AIO_O_INTS_fifoints11(data)                                                  (0x00400000&((data)<<22))
#define AIO_O_INTS_fifoints11_src(data)                                              ((0x00400000&(data))>>22)
#define AIO_O_INTS_get_fifoints11(data)                                              ((0x00400000&(data))>>22)
#define AIO_O_INTS_fifoints10_shift                                                  (21)
#define AIO_O_INTS_fifoints10_mask                                                   (0x00200000)
#define AIO_O_INTS_fifoints10(data)                                                  (0x00200000&((data)<<21))
#define AIO_O_INTS_fifoints10_src(data)                                              ((0x00200000&(data))>>21)
#define AIO_O_INTS_get_fifoints10(data)                                              ((0x00200000&(data))>>21)
#define AIO_O_INTS_fifoints9_shift                                                   (20)
#define AIO_O_INTS_fifoints9_mask                                                    (0x00100000)
#define AIO_O_INTS_fifoints9(data)                                                   (0x00100000&((data)<<20))
#define AIO_O_INTS_fifoints9_src(data)                                               ((0x00100000&(data))>>20)
#define AIO_O_INTS_get_fifoints9(data)                                               ((0x00100000&(data))>>20)
#define AIO_O_INTS_fifoints8_shift                                                   (19)
#define AIO_O_INTS_fifoints8_mask                                                    (0x00080000)
#define AIO_O_INTS_fifoints8(data)                                                   (0x00080000&((data)<<19))
#define AIO_O_INTS_fifoints8_src(data)                                               ((0x00080000&(data))>>19)
#define AIO_O_INTS_get_fifoints8(data)                                               ((0x00080000&(data))>>19)
#define AIO_O_INTS_fifoints7_shift                                                   (18)
#define AIO_O_INTS_fifoints7_mask                                                    (0x00040000)
#define AIO_O_INTS_fifoints7(data)                                                   (0x00040000&((data)<<18))
#define AIO_O_INTS_fifoints7_src(data)                                               ((0x00040000&(data))>>18)
#define AIO_O_INTS_get_fifoints7(data)                                               ((0x00040000&(data))>>18)
#define AIO_O_INTS_fifoints6_shift                                                   (17)
#define AIO_O_INTS_fifoints6_mask                                                    (0x00020000)
#define AIO_O_INTS_fifoints6(data)                                                   (0x00020000&((data)<<17))
#define AIO_O_INTS_fifoints6_src(data)                                               ((0x00020000&(data))>>17)
#define AIO_O_INTS_get_fifoints6(data)                                               ((0x00020000&(data))>>17)
#define AIO_O_INTS_fifoints5_shift                                                   (16)
#define AIO_O_INTS_fifoints5_mask                                                    (0x00010000)
#define AIO_O_INTS_fifoints5(data)                                                   (0x00010000&((data)<<16))
#define AIO_O_INTS_fifoints5_src(data)                                               ((0x00010000&(data))>>16)
#define AIO_O_INTS_get_fifoints5(data)                                               ((0x00010000&(data))>>16)
#define AIO_O_INTS_fifoints4_shift                                                   (15)
#define AIO_O_INTS_fifoints4_mask                                                    (0x00008000)
#define AIO_O_INTS_fifoints4(data)                                                   (0x00008000&((data)<<15))
#define AIO_O_INTS_fifoints4_src(data)                                               ((0x00008000&(data))>>15)
#define AIO_O_INTS_get_fifoints4(data)                                               ((0x00008000&(data))>>15)
#define AIO_O_INTS_pcmtimints_shift                                                  (14)
#define AIO_O_INTS_pcmtimints_mask                                                   (0x00004000)
#define AIO_O_INTS_pcmtimints(data)                                                  (0x00004000&((data)<<14))
#define AIO_O_INTS_pcmtimints_src(data)                                              ((0x00004000&(data))>>14)
#define AIO_O_INTS_get_pcmtimints(data)                                              ((0x00004000&(data))>>14)
#define AIO_O_INTS_rawtimints_shift                                                  (13)
#define AIO_O_INTS_rawtimints_mask                                                   (0x00002000)
#define AIO_O_INTS_rawtimints(data)                                                  (0x00002000&((data)<<13))
#define AIO_O_INTS_rawtimints_src(data)                                              ((0x00002000&(data))>>13)
#define AIO_O_INTS_get_rawtimints(data)                                              ((0x00002000&(data))>>13)
#define AIO_O_INTS_hdmitimints_shift                                                 (12)
#define AIO_O_INTS_hdmitimints_mask                                                  (0x00001000)
#define AIO_O_INTS_hdmitimints(data)                                                 (0x00001000&((data)<<12))
#define AIO_O_INTS_hdmitimints_src(data)                                             ((0x00001000&(data))>>12)
#define AIO_O_INTS_get_hdmitimints(data)                                             ((0x00001000&(data))>>12)
#define AIO_O_INTS_pcmthdints_shift                                                  (11)
#define AIO_O_INTS_pcmthdints_mask                                                   (0x00000800)
#define AIO_O_INTS_pcmthdints(data)                                                  (0x00000800&((data)<<11))
#define AIO_O_INTS_pcmthdints_src(data)                                              ((0x00000800&(data))>>11)
#define AIO_O_INTS_get_pcmthdints(data)                                              ((0x00000800&(data))>>11)
#define AIO_O_INTS_rawthdints_shift                                                  (10)
#define AIO_O_INTS_rawthdints_mask                                                   (0x00000400)
#define AIO_O_INTS_rawthdints(data)                                                  (0x00000400&((data)<<10))
#define AIO_O_INTS_rawthdints_src(data)                                              ((0x00000400&(data))>>10)
#define AIO_O_INTS_get_rawthdints(data)                                              ((0x00000400&(data))>>10)
#define AIO_O_INTS_hdmithdints_shift                                                 (9)
#define AIO_O_INTS_hdmithdints_mask                                                  (0x00000200)
#define AIO_O_INTS_hdmithdints(data)                                                 (0x00000200&((data)<<9))
#define AIO_O_INTS_hdmithdints_src(data)                                             ((0x00000200&(data))>>9)
#define AIO_O_INTS_get_hdmithdints(data)                                             ((0x00000200&(data))>>9)
#define AIO_O_INTS_dramints3_shift                                                   (8)
#define AIO_O_INTS_dramints3_mask                                                    (0x00000100)
#define AIO_O_INTS_dramints3(data)                                                   (0x00000100&((data)<<8))
#define AIO_O_INTS_dramints3_src(data)                                               ((0x00000100&(data))>>8)
#define AIO_O_INTS_get_dramints3(data)                                               ((0x00000100&(data))>>8)
#define AIO_O_INTS_dramints2_shift                                                   (7)
#define AIO_O_INTS_dramints2_mask                                                    (0x00000080)
#define AIO_O_INTS_dramints2(data)                                                   (0x00000080&((data)<<7))
#define AIO_O_INTS_dramints2_src(data)                                               ((0x00000080&(data))>>7)
#define AIO_O_INTS_get_dramints2(data)                                               ((0x00000080&(data))>>7)
#define AIO_O_INTS_dramints1_shift                                                   (6)
#define AIO_O_INTS_dramints1_mask                                                    (0x00000040)
#define AIO_O_INTS_dramints1(data)                                                   (0x00000040&((data)<<6))
#define AIO_O_INTS_dramints1_src(data)                                               ((0x00000040&(data))>>6)
#define AIO_O_INTS_get_dramints1(data)                                               ((0x00000040&(data))>>6)
#define AIO_O_INTS_dramints0_shift                                                   (5)
#define AIO_O_INTS_dramints0_mask                                                    (0x00000020)
#define AIO_O_INTS_dramints0(data)                                                   (0x00000020&((data)<<5))
#define AIO_O_INTS_dramints0_src(data)                                               ((0x00000020&(data))>>5)
#define AIO_O_INTS_get_dramints0(data)                                               ((0x00000020&(data))>>5)
#define AIO_O_INTS_fifoints3_shift                                                   (4)
#define AIO_O_INTS_fifoints3_mask                                                    (0x00000010)
#define AIO_O_INTS_fifoints3(data)                                                   (0x00000010&((data)<<4))
#define AIO_O_INTS_fifoints3_src(data)                                               ((0x00000010&(data))>>4)
#define AIO_O_INTS_get_fifoints3(data)                                               ((0x00000010&(data))>>4)
#define AIO_O_INTS_fifoints2_shift                                                   (3)
#define AIO_O_INTS_fifoints2_mask                                                    (0x00000008)
#define AIO_O_INTS_fifoints2(data)                                                   (0x00000008&((data)<<3))
#define AIO_O_INTS_fifoints2_src(data)                                               ((0x00000008&(data))>>3)
#define AIO_O_INTS_get_fifoints2(data)                                               ((0x00000008&(data))>>3)
#define AIO_O_INTS_fifoints1_shift                                                   (2)
#define AIO_O_INTS_fifoints1_mask                                                    (0x00000004)
#define AIO_O_INTS_fifoints1(data)                                                   (0x00000004&((data)<<2))
#define AIO_O_INTS_fifoints1_src(data)                                               ((0x00000004&(data))>>2)
#define AIO_O_INTS_get_fifoints1(data)                                               ((0x00000004&(data))>>2)
#define AIO_O_INTS_fifoints0_shift                                                   (1)
#define AIO_O_INTS_fifoints0_mask                                                    (0x00000002)
#define AIO_O_INTS_fifoints0(data)                                                   (0x00000002&((data)<<1))
#define AIO_O_INTS_fifoints0_src(data)                                               ((0x00000002&(data))>>1)
#define AIO_O_INTS_get_fifoints0(data)                                               ((0x00000002&(data))>>1)
#define AIO_O_INTS_write_data_shift                                                  (0)
#define AIO_O_INTS_write_data_mask                                                   (0x00000001)
#define AIO_O_INTS_write_data(data)                                                  (0x00000001&((data)<<0))
#define AIO_O_INTS_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define AIO_O_INTS_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define AIO_DBG                                                                      0x18006500
#define AIO_DBG_reg_addr                                                             "0xB8006500"
#define AIO_DBG_reg                                                                  0xB8006500
#define set_AIO_DBG_reg(data)   (*((volatile unsigned int*) AIO_DBG_reg)=data)
#define get_AIO_DBG_reg   (*((volatile unsigned int*) AIO_DBG_reg))
#define AIO_DBG_inst_adr                                                             "0x0040"
#define AIO_DBG_inst                                                                 0x0040
#define AIO_DBG_loopbacken_shift                                                     (8)
#define AIO_DBG_loopbacken_mask                                                      (0x00000100)
#define AIO_DBG_loopbacken(data)                                                     (0x00000100&((data)<<8))
#define AIO_DBG_loopbacken_src(data)                                                 ((0x00000100&(data))>>8)
#define AIO_DBG_get_loopbacken(data)                                                 ((0x00000100&(data))>>8)
#define AIO_DBG_codec_tmode_shift                                                    (7)
#define AIO_DBG_codec_tmode_mask                                                     (0x00000080)
#define AIO_DBG_codec_tmode(data)                                                    (0x00000080&((data)<<7))
#define AIO_DBG_codec_tmode_src(data)                                                ((0x00000080&(data))>>7)
#define AIO_DBG_get_codec_tmode(data)                                                ((0x00000080&(data))>>7)
#define AIO_DBG_dbgen_shift                                                          (6)
#define AIO_DBG_dbgen_mask                                                           (0x00000040)
#define AIO_DBG_dbgen(data)                                                          (0x00000040&((data)<<6))
#define AIO_DBG_dbgen_src(data)                                                      ((0x00000040&(data))>>6)
#define AIO_DBG_get_dbgen(data)                                                      ((0x00000040&(data))>>6)
#define AIO_DBG_sel0_shift                                                           (3)
#define AIO_DBG_sel0_mask                                                            (0x00000038)
#define AIO_DBG_sel0(data)                                                           (0x00000038&((data)<<3))
#define AIO_DBG_sel0_src(data)                                                       ((0x00000038&(data))>>3)
#define AIO_DBG_get_sel0(data)                                                       ((0x00000038&(data))>>3)
#define AIO_DBG_sel1_shift                                                           (0)
#define AIO_DBG_sel1_mask                                                            (0x00000007)
#define AIO_DBG_sel1(data)                                                           (0x00000007&((data)<<0))
#define AIO_DBG_sel1_src(data)                                                       ((0x00000007&(data))>>0)
#define AIO_DBG_get_sel1(data)                                                       ((0x00000007&(data))>>0)


#define AIO_SPARE                                                                    0x18006504
#define AIO_SPARE_reg_addr                                                           "0xB8006504"
#define AIO_SPARE_reg                                                                0xB8006504
#define set_AIO_SPARE_reg(data)   (*((volatile unsigned int*) AIO_SPARE_reg)=data)
#define get_AIO_SPARE_reg   (*((volatile unsigned int*) AIO_SPARE_reg))
#define AIO_SPARE_inst_adr                                                           "0x0041"
#define AIO_SPARE_inst                                                               0x0041
#define AIO_SPARE_spare_shift                                                        (0)
#define AIO_SPARE_spare_mask                                                         (0xFFFFFFFF)
#define AIO_SPARE_spare(data)                                                        (0xFFFFFFFF&((data)<<0))
#define AIO_SPARE_spare_src(data)                                                    ((0xFFFFFFFF&(data))>>0)
#define AIO_SPARE_get_spare(data)                                                    ((0xFFFFFFFF&(data))>>0)


#endif
