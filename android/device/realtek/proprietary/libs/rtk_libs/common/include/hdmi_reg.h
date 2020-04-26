/**************************************************************
// Spec Version                  : 1.17
// Parser Version                : MAR_Parser_1.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name
// Firmware Header Generate Date : 2010/5/5
***************************************************************/


#ifndef _HDMI_REG_H_INCLUDED_
#define _HDMI_REG_H_INCLUDED_
#ifdef  _HDMI_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     rxsenseint:1;
unsigned int     enriupdint:1;
unsigned int     enpjupdint:1;
unsigned int     write_data:1;
}HDMI_INTEN;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     vendint:1;
unsigned int     vsyncint:1;
unsigned int     write_data:1;
}HDMI_INTENV;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     asfifo_empty:1;
unsigned int     asfifo_full:1;
unsigned int     owctshw:1;
unsigned int     asfifoow:1;
unsigned int     write_data:1;
}HDMI_AUDIO_ST;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     rxupdated:1;
unsigned int     riupdated:1;
unsigned int     pjupdated:1;
}HDMI_INTST;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     vendupdated:1;
unsigned int     vsyncupdated:1;
}HDMI_INTSTV;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     wdout:1;
unsigned int     Rxstatus:1;
}HDMI_PHY_STATUS;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     write_en2:1;
unsigned int     rirate:8;
unsigned int     write_en1:1;
unsigned int     pjrate:8;
}HDMI_UPDATE_ST;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     video_sd:1;
unsigned int     Interlaced_vfmt:1;
unsigned int     3D_video_format:4;
unsigned int     En_3D:1;
unsigned int     timing_gen_en:1;
unsigned int     chl_sel:2;
}HDMI_CHNL_SEL;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c1:17;
}HDMI_CS_TRANS0;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c2:17;
}HDMI_CS_TRANS1;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c3:17;
}HDMI_CS_TRANS2;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c4:17;
}HDMI_CS_TRANS3;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c5:17;
}HDMI_CS_TRANS4;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c6:17;
}HDMI_CS_TRANS5;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c7:17;
}HDMI_CS_TRANS6;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c8:17;
}HDMI_CS_TRANS7;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     c9:17;
}HDMI_CS_TRANS8;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     k1:16;
}HDMI_CS_TRANS9;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     k2:16;
}HDMI_CS_TRANS10;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     k3:16;
}HDMI_CS_TRANS11;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     write_en2:1;
unsigned int     ygmax:12;
unsigned int     reserved_1:3;
unsigned int     write_en1:1;
unsigned int     ygmin:12;
}HDMI_CS_TRANS12;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     write_en2:1;
unsigned int     cbbmax:12;
unsigned int     reserved_1:3;
unsigned int     write_en1:1;
unsigned int     cbbmin:12;
}HDMI_CS_TRANS13;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     write_en2:1;
unsigned int     crrmax:12;
unsigned int     reserved_1:3;
unsigned int     write_en1:1;
unsigned int     crrmin:12;
}HDMI_CS_TRANS14;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     write_en4:1;
unsigned int     nor_v:2;
unsigned int     write_en3:1;
unsigned int     nor_h:2;
unsigned int     write_en2:1;
unsigned int     disp_v:2;
unsigned int     write_en1:1;
unsigned int     disp_h:2;
}HDMI_SYNC_DLY;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     write_en3:1;
unsigned int     tmds_encen:1;
unsigned int     write_en2:1;
unsigned int     enablehdcp:1;
unsigned int     write_en1:1;
unsigned int     enablehdmi:1;
}HDMI_CR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     write_en9:1;
unsigned int     dp_vfch_num:2;
unsigned int     reserved_1:2;
unsigned int     write_en8:1;
unsigned int     fp_swen:1;
unsigned int     write_en7:1;
unsigned int     fp:3;
unsigned int     write_en6:1;
unsigned int     pp_swen:1;
unsigned int     reserved_2:2;
unsigned int     write_en4:1;
unsigned int     dp_swen:1;
unsigned int     write_en3:1;
unsigned int     default_phase:1;
unsigned int     reserved_3:3;
unsigned int     write_en2:1;
unsigned int     color_depth:4;
unsigned int     reserved_4:2;
unsigned int     write_en1:1;
unsigned int     dpc_enable:1;
}HDMI_DPC;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     write_en2:1;
unsigned int     dp_vfch:1;
unsigned int     reserved_1:3;
unsigned int     write_en1:1;
unsigned int     pp:4;
}HDMI_DPC1;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     tmds_ch2_out:10;
unsigned int     tmds_ch1_out:10;
unsigned int     tmds_ch0_out:10;
}HDMI_TMDS_EODR0;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     default_phase_st:1;
unsigned int     reserved_1:2;
unsigned int     tmds_clk_out:10;
}HDMI_TMDS_EODR1;

typedef struct 
{
unsigned int     ip:4;
unsigned int     rs:3;
unsigned int     cs:2;
unsigned int     cp:1;
unsigned int     m:2;
unsigned int     wdrst:1;
unsigned int     wdset:1;
unsigned int     rstb:1;
unsigned int     powbias:1;
unsigned int     powck:1;
unsigned int     powa:1;
unsigned int     powb:1;
unsigned int     powc:1;
unsigned int     emck:1;
unsigned int     ema:1;
unsigned int     emb:1;
unsigned int     emc:1;
unsigned int     reserved_0:6;
unsigned int     powpll:1;
unsigned int     cksel:1;
}HDMI_TMDS_SCR1;

typedef struct 
{
unsigned int     sel:2;
unsigned int     bias:1;
unsigned int     vsel:1;
unsigned int     powlv:1;
unsigned int     lv:3;
unsigned int     iemck:4;
unsigned int     iema:4;
unsigned int     iemb:4;
unsigned int     iemc:4;
unsigned int     rtonck:2;
unsigned int     rtona:2;
unsigned int     rtonb:2;
unsigned int     rtonc:2;
}HDMI_TMDS_SCR2;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     idrvck:3;
unsigned int     reserved_1:1;
unsigned int     idrva:3;
unsigned int     reserved_2:1;
unsigned int     idrvb:3;
unsigned int     reserved_3:1;
unsigned int     idrvc:3;
}HDMI_TMDS_SCR3;

typedef struct 
{
unsigned int     tmds1ten:1;
unsigned int     td1src:1;
unsigned int     reserved_0:4;
unsigned int     tmds1td:10;
unsigned int     tmds0ten:1;
unsigned int     td0src:1;
unsigned int     reserved_1:4;
unsigned int     tmds0td:10;
}HDMI_TMDS_STR0;

typedef struct 
{
unsigned int     tmdscten:1;
unsigned int     tdcsrc:1;
unsigned int     reserved_0:4;
unsigned int     tmdsctd:10;
unsigned int     tmds2ten:1;
unsigned int     td2src:1;
unsigned int     reserved_1:4;
unsigned int     tmds2td:10;
}HDMI_TMDS_STR1;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     write_en8:1;
unsigned int     ycbcr422_8bits:1;
unsigned int     write_en7:1;
unsigned int     cts_pkt_hp:1;
unsigned int     write_en6:1;
unsigned int     Audioheader0:8;
unsigned int     write_en5:1;
unsigned int     samplepresent:4;
unsigned int     write_en4:1;
unsigned int     forcesamplepresent:1;
unsigned int     write_en3:1;
unsigned int     b_frame:4;
unsigned int     write_en2:1;
unsigned int     forceb_frame:1;
unsigned int     write_en1:1;
unsigned int     audiolayout:1;
}HDMI_ASPCR1;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     write_en4:1;
unsigned int     hdmi_pk3map:3;
unsigned int     write_en3:1;
unsigned int     hdmi_pk2map:3;
unsigned int     write_en2:1;
unsigned int     hdmi_pk1map:3;
unsigned int     write_en1:1;
unsigned int     hdmi_pk0map:3;
}HDMI_ACACR;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     write_en4:1;
unsigned int     fs_div4:1;
unsigned int     write_en3:1;
unsigned int     disablecrp:1;
unsigned int     write_en2:1;
unsigned int     cts_source:1;
unsigned int     write_en1:1;
unsigned int     n_value:20;
}HDMI_ACRPCR;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     ctssw:20;
}HDMI_ACRPCTSR;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     ctshw:20;
}HDMI_ACRPPR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     enablegcp:1;
unsigned int     gcp_clearavmute:1;
unsigned int     gcp_setavmute:1;
unsigned int     write_data:1;
}HDMI_GCPCR;

typedef struct 
{
unsigned int     once_pkt:32;
}HDMI_ORP6PH;

typedef struct 
{
unsigned int     once_pkt:32;
}HDMI_ORP6W;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     write_en6:1;
unsigned int     prp5period:4;
unsigned int     write_en5:1;
unsigned int     prp4period:4;
unsigned int     write_en4:1;
unsigned int     prp3period:4;
unsigned int     write_en3:1;
unsigned int     prp2period:4;
unsigned int     write_en2:1;
unsigned int     prp1period:4;
unsigned int     write_en1:1;
unsigned int     prp0period:4;
}HDMI_RPCR;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     enprpkt5:1;
unsigned int     enprpkt4:1;
unsigned int     enprpkt3:1;
unsigned int     enprpkt2:1;
unsigned int     enprpkt1:1;
unsigned int     enprpkt0:1;
unsigned int     write_data:1;
}HDMI_RPEN;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     isramrbd:18;
}HDMI_RPRBDR;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     write_en4:1;
unsigned int     enrbpktsram:1;
unsigned int     write_en3:1;
unsigned int     wrdes:1;
unsigned int     write_en2:1;
unsigned int     rprwcmd:1;
unsigned int     write_en1:1;
unsigned int     rpadd:8;
}HDMI_OPCR;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     write_en2:1;
unsigned int     vbidipcnt:5;
unsigned int     write_en1:1;
unsigned int     hbidipcnt:5;
}HDMI_DIPCCR;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     write_en11:1;
unsigned int     ycbcr422_algo:1;
unsigned int     write_en10:1;
unsigned int     422_pixel_repeat:1;
unsigned int     write_en9:1;
unsigned int     vsyncpolin:1;
unsigned int     write_en8:1;
unsigned int     hsyncpolin:1;
unsigned int     write_en7:1;
unsigned int     vsyncpolinv:1;
unsigned int     write_en6:1;
unsigned int     hsyncpolinv:1;
unsigned int     write_en5:1;
unsigned int     pixelencycbcr422:1;
unsigned int     write_en4:1;
unsigned int     hdmi_videoxs:5;
unsigned int     write_en3:1;
unsigned int     pixelencfmt:2;
unsigned int     write_en2:1;
unsigned int     pixelrepeat:1;
unsigned int     write_en1:1;
unsigned int     hdmi_modesel:1;
}HDMI_SCHCR;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     write_en3:1;
unsigned int     enaudio:1;
unsigned int     write_en2:1;
unsigned int     envitd:1;
unsigned int     write_en1:1;
unsigned int     vitd:24;
}HDMI_ICR;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     write_en1:1;
unsigned int     vitd:12;
}HDMI_ICR1;

typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     write_en2:1;
unsigned int     crcmode:1;
unsigned int     write_en1:1;
unsigned int     crcchsel:2;
}HDMI_CRCCR;

typedef struct 
{
unsigned int     crcvalue:32;
}HDMI_CRCDOR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     hdcprekeykeepoutwin:8;
}HDMI_HDCP_KOWR;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     write_en2:1;
unsigned int     hdcpverkeepoutwinend:12;
unsigned int     write_en1:1;
unsigned int     hdcpvertkeepoutwinstart:12;
}HDMI_HDCP_KOWRSE;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     write_en2:1;
unsigned int     hdcpoppwinend:12;
unsigned int     write_en1:1;
unsigned int     hdcpoppwinstart:12;
}HDMI_HDCP_OWR;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     write_en4:1;
unsigned int     en1_1_feature:1;
unsigned int     write_en3:1;
unsigned int     downstrisrepeater:1;
unsigned int     write_en2:1;
unsigned int     aninfluencemode:1;
unsigned int     write_en1:1;
unsigned int     hdcp_encryptenable:1;
}HDMI_HDCP_CR;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     write_en9:1;
unsigned int     ddpken:1;
unsigned int     write_en8:1;
unsigned int     resetkmacc:1;
unsigned int     write_en7:1;
unsigned int     update_an:1;
unsigned int     write_en6:1;
unsigned int     aninfreq:1;
unsigned int     write_en5:1;
unsigned int     seedload:1;
unsigned int     write_en4:1;
unsigned int     deviceauthenticated:1;
unsigned int     write_en3:1;
unsigned int     forcetounauthenticated:1;
unsigned int     write_en2:1;
unsigned int     authcompute:1;
unsigned int     write_en1:1;
unsigned int     authrequest:1;
}HDMI_HDCP_AUTH;

typedef struct 
{
unsigned int     hdcpcipherstate:8;
unsigned int     reserved_0:20;
unsigned int     curdpkaccdone:1;
unsigned int     hdcp_encrypt_status:1;
unsigned int     authenticatedok:1;
unsigned int     anready:1;
}HDMI_HDCP_SR;

typedef struct 
{
unsigned int     anlsw:32;
}HDMI_HDCP_ANLR;

typedef struct 
{
unsigned int     anmsw:32;
}HDMI_HDCP_ANMR;

typedef struct 
{
unsigned int     aninfluencelsw:32;
}HDMI_HDCP_ANILR;

typedef struct 
{
unsigned int     aninfluencemsw:32;
}HDMI_HDCP_ANIMR;

typedef struct 
{
unsigned int     ddpklsb24:24;
unsigned int     dpkencpnt:8;
}HDMI_HDCP_DPKLR;

typedef struct 
{
unsigned int     ddpkmsw:32;
}HDMI_HDCP_DPKMR;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     ri:16;
unsigned int     pj:8;
}HDMI_HDCP_LIR;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     shastart:1;
unsigned int     shafirst:1;
unsigned int     rstshaptr:1;
unsigned int     write_data:1;
}HDMI_HDCP_SHACR;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     vmatch:1;
unsigned int     shaready:1;
}HDMI_HDCP_SHARR;

typedef struct 
{
unsigned int     sha_data:32;
}HDMI_HDCP_SHADR;

typedef struct 
{
unsigned int     milsw:32;
}HDMI_HDCP_MILSW;

typedef struct 
{
unsigned int     mimsw:32;
}HDMI_HDCP_MIMSW;

typedef struct 
{
unsigned int     kmlsw:32;
}HDMI_HDCP_KMLSW;

typedef struct 
{
unsigned int     kmmsw:32;
}HDMI_HDCP_KMMSW;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     chsel0:4;
unsigned int     chsel1:4;
}HDMI_DBG;

typedef struct 
{
unsigned int     dummy:32;
}HDMI_DUMMY;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     test_mode:1;
unsigned int     data_realign:1;
}HDMI_PHY;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     hblank:14;
unsigned int     reserved_1:2;
unsigned int     hactive:14;
}HDMI_H_PARA1;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     hsync:14;
unsigned int     reserved_1:2;
unsigned int     hfront:14;
}HDMI_H_PARA2;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     hback:14;
}HDMI_H_PARA3;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vact_video:14;
unsigned int     reserved_1:2;
unsigned int     vactive:14;
}HDMI_V_PARA1;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vact_space1:14;
unsigned int     reserved_1:2;
unsigned int     Vact_space:14;
}HDMI_V_PARA2;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vblank3:14;
unsigned int     reserved_1:2;
unsigned int     Vact_space2:14;
}HDMI_V_PARA3;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     vsync:14;
unsigned int     reserved_1:2;
unsigned int     vblank:14;
}HDMI_V_PARA4;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     vback:14;
unsigned int     reserved_1:2;
unsigned int     vfront:14;
}HDMI_V_PARA5;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vsync1:14;
unsigned int     reserved_1:2;
unsigned int     Vblank1:14;
}HDMI_V_PARA6;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vback1:14;
unsigned int     reserved_1:2;
unsigned int     Vfront1:14;
}HDMI_V_PARA7;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vsync2:14;
unsigned int     reserved_1:2;
unsigned int     Vblank2:14;
}HDMI_V_PARA8;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     Vback2:14;
unsigned int     reserved_1:2;
unsigned int     Vfront2:14;
}HDMI_V_PARA9;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     vsynci:14;
unsigned int     reserved_1:2;
unsigned int     vblanki:14;
}HDMI_V_PARA12;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     vbacki:14;
unsigned int     reserved_1:2;
unsigned int     vfronti:14;
}HDMI_V_PARA13;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     Y:12;
}HDMI_V_PARA10;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     Cb:12;
unsigned int     reserved_1:4;
unsigned int     Cr:12;
}HDMI_V_PARA11;

#endif

#define HDMI_INTEN                                                                   0x1800D000
#define HDMI_INTEN_reg_addr                                                          "0xB800D000"
#define HDMI_INTEN_reg                                                               0xB800D000
#define set_HDMI_INTEN_reg(data)   (*((volatile unsigned int*) HDMI_INTEN_reg)=data)
#define get_HDMI_INTEN_reg   (*((volatile unsigned int*) HDMI_INTEN_reg))
#define HDMI_INTEN_inst_adr                                                          "0x0000"
#define HDMI_INTEN_inst                                                              0x0000
#define HDMI_INTEN_rxsenseint_shift                                                  (3)
#define HDMI_INTEN_rxsenseint_mask                                                   (0x00000008)
#define HDMI_INTEN_rxsenseint(data)                                                  (0x00000008&((data)<<3))
#define HDMI_INTEN_rxsenseint_src(data)                                              ((0x00000008&(data))>>3)
#define HDMI_INTEN_get_rxsenseint(data)                                              ((0x00000008&(data))>>3)
#define HDMI_INTEN_enriupdint_shift                                                  (2)
#define HDMI_INTEN_enriupdint_mask                                                   (0x00000004)
#define HDMI_INTEN_enriupdint(data)                                                  (0x00000004&((data)<<2))
#define HDMI_INTEN_enriupdint_src(data)                                              ((0x00000004&(data))>>2)
#define HDMI_INTEN_get_enriupdint(data)                                              ((0x00000004&(data))>>2)
#define HDMI_INTEN_enpjupdint_shift                                                  (1)
#define HDMI_INTEN_enpjupdint_mask                                                   (0x00000002)
#define HDMI_INTEN_enpjupdint(data)                                                  (0x00000002&((data)<<1))
#define HDMI_INTEN_enpjupdint_src(data)                                              ((0x00000002&(data))>>1)
#define HDMI_INTEN_get_enpjupdint(data)                                              ((0x00000002&(data))>>1)
#define HDMI_INTEN_write_data_shift                                                  (0)
#define HDMI_INTEN_write_data_mask                                                   (0x00000001)
#define HDMI_INTEN_write_data(data)                                                  (0x00000001&((data)<<0))
#define HDMI_INTEN_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define HDMI_INTEN_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define HDMI_INTENV                                                                  0x1800D274
#define HDMI_INTENV_reg_addr                                                         "0xB800D274"
#define HDMI_INTENV_reg                                                              0xB800D274
#define set_HDMI_INTENV_reg(data)   (*((volatile unsigned int*) HDMI_INTENV_reg)=data)
#define get_HDMI_INTENV_reg   (*((volatile unsigned int*) HDMI_INTENV_reg))
#define HDMI_INTENV_inst_adr                                                         "0x009D"
#define HDMI_INTENV_inst                                                             0x009D
#define HDMI_INTENV_vendint_shift                                                    (2)
#define HDMI_INTENV_vendint_mask                                                     (0x00000004)
#define HDMI_INTENV_vendint(data)                                                    (0x00000004&((data)<<2))
#define HDMI_INTENV_vendint_src(data)                                                ((0x00000004&(data))>>2)
#define HDMI_INTENV_get_vendint(data)                                                ((0x00000004&(data))>>2)
#define HDMI_INTENV_vsyncint_shift                                                   (1)
#define HDMI_INTENV_vsyncint_mask                                                    (0x00000002)
#define HDMI_INTENV_vsyncint(data)                                                   (0x00000002&((data)<<1))
#define HDMI_INTENV_vsyncint_src(data)                                               ((0x00000002&(data))>>1)
#define HDMI_INTENV_get_vsyncint(data)                                               ((0x00000002&(data))>>1)
#define HDMI_INTENV_write_data_shift                                                 (0)
#define HDMI_INTENV_write_data_mask                                                  (0x00000001)
#define HDMI_INTENV_write_data(data)                                                 (0x00000001&((data)<<0))
#define HDMI_INTENV_write_data_src(data)                                             ((0x00000001&(data))>>0)
#define HDMI_INTENV_get_write_data(data)                                             ((0x00000001&(data))>>0)


#define HDMI_AUDIO_ST                                                                0x1800D004
#define HDMI_AUDIO_ST_reg_addr                                                       "0xB800D004"
#define HDMI_AUDIO_ST_reg                                                            0xB800D004
#define set_HDMI_AUDIO_ST_reg(data)   (*((volatile unsigned int*) HDMI_AUDIO_ST_reg)=data)
#define get_HDMI_AUDIO_ST_reg   (*((volatile unsigned int*) HDMI_AUDIO_ST_reg))
#define HDMI_AUDIO_ST_inst_adr                                                       "0x0001"
#define HDMI_AUDIO_ST_inst                                                           0x0001
#define HDMI_AUDIO_ST_asfifo_empty_shift                                             (4)
#define HDMI_AUDIO_ST_asfifo_empty_mask                                              (0x00000010)
#define HDMI_AUDIO_ST_asfifo_empty(data)                                             (0x00000010&((data)<<4))
#define HDMI_AUDIO_ST_asfifo_empty_src(data)                                         ((0x00000010&(data))>>4)
#define HDMI_AUDIO_ST_get_asfifo_empty(data)                                         ((0x00000010&(data))>>4)
#define HDMI_AUDIO_ST_asfifo_full_shift                                              (3)
#define HDMI_AUDIO_ST_asfifo_full_mask                                               (0x00000008)
#define HDMI_AUDIO_ST_asfifo_full(data)                                              (0x00000008&((data)<<3))
#define HDMI_AUDIO_ST_asfifo_full_src(data)                                          ((0x00000008&(data))>>3)
#define HDMI_AUDIO_ST_get_asfifo_full(data)                                          ((0x00000008&(data))>>3)
#define HDMI_AUDIO_ST_owctshw_shift                                                  (2)
#define HDMI_AUDIO_ST_owctshw_mask                                                   (0x00000004)
#define HDMI_AUDIO_ST_owctshw(data)                                                  (0x00000004&((data)<<2))
#define HDMI_AUDIO_ST_owctshw_src(data)                                              ((0x00000004&(data))>>2)
#define HDMI_AUDIO_ST_get_owctshw(data)                                              ((0x00000004&(data))>>2)
#define HDMI_AUDIO_ST_asfifoow_shift                                                 (1)
#define HDMI_AUDIO_ST_asfifoow_mask                                                  (0x00000002)
#define HDMI_AUDIO_ST_asfifoow(data)                                                 (0x00000002&((data)<<1))
#define HDMI_AUDIO_ST_asfifoow_src(data)                                             ((0x00000002&(data))>>1)
#define HDMI_AUDIO_ST_get_asfifoow(data)                                             ((0x00000002&(data))>>1)
#define HDMI_AUDIO_ST_write_data_shift                                               (0)
#define HDMI_AUDIO_ST_write_data_mask                                                (0x00000001)
#define HDMI_AUDIO_ST_write_data(data)                                               (0x00000001&((data)<<0))
#define HDMI_AUDIO_ST_write_data_src(data)                                           ((0x00000001&(data))>>0)
#define HDMI_AUDIO_ST_get_write_data(data)                                           ((0x00000001&(data))>>0)


#define HDMI_INTST                                                                   0x1800D008
#define HDMI_INTST_reg_addr                                                          "0xB800D008"
#define HDMI_INTST_reg                                                               0xB800D008
#define set_HDMI_INTST_reg(data)   (*((volatile unsigned int*) HDMI_INTST_reg)=data)
#define get_HDMI_INTST_reg   (*((volatile unsigned int*) HDMI_INTST_reg))
#define HDMI_INTST_inst_adr                                                          "0x0002"
#define HDMI_INTST_inst                                                              0x0002
#define HDMI_INTST_rxupdated_shift                                                   (2)
#define HDMI_INTST_rxupdated_mask                                                    (0x00000004)
#define HDMI_INTST_rxupdated(data)                                                   (0x00000004&((data)<<2))
#define HDMI_INTST_rxupdated_src(data)                                               ((0x00000004&(data))>>2)
#define HDMI_INTST_get_rxupdated(data)                                               ((0x00000004&(data))>>2)
#define HDMI_INTST_riupdated_shift                                                   (1)
#define HDMI_INTST_riupdated_mask                                                    (0x00000002)
#define HDMI_INTST_riupdated(data)                                                   (0x00000002&((data)<<1))
#define HDMI_INTST_riupdated_src(data)                                               ((0x00000002&(data))>>1)
#define HDMI_INTST_get_riupdated(data)                                               ((0x00000002&(data))>>1)
#define HDMI_INTST_pjupdated_shift                                                   (0)
#define HDMI_INTST_pjupdated_mask                                                    (0x00000001)
#define HDMI_INTST_pjupdated(data)                                                   (0x00000001&((data)<<0))
#define HDMI_INTST_pjupdated_src(data)                                               ((0x00000001&(data))>>0)
#define HDMI_INTST_get_pjupdated(data)                                               ((0x00000001&(data))>>0)


#define HDMI_INTSTV                                                                  0x1800D278
#define HDMI_INTSTV_reg_addr                                                         "0xB800D278"
#define HDMI_INTSTV_reg                                                              0xB800D278
#define set_HDMI_INTSTV_reg(data)   (*((volatile unsigned int*) HDMI_INTSTV_reg)=data)
#define get_HDMI_INTSTV_reg   (*((volatile unsigned int*) HDMI_INTSTV_reg))
#define HDMI_INTSTV_inst_adr                                                         "0x009E"
#define HDMI_INTSTV_inst                                                             0x009E
#define HDMI_INTSTV_vendupdated_shift                                                (1)
#define HDMI_INTSTV_vendupdated_mask                                                 (0x00000002)
#define HDMI_INTSTV_vendupdated(data)                                                (0x00000002&((data)<<1))
#define HDMI_INTSTV_vendupdated_src(data)                                            ((0x00000002&(data))>>1)
#define HDMI_INTSTV_get_vendupdated(data)                                            ((0x00000002&(data))>>1)
#define HDMI_INTSTV_vsyncupdated_shift                                               (0)
#define HDMI_INTSTV_vsyncupdated_mask                                                (0x00000001)
#define HDMI_INTSTV_vsyncupdated(data)                                               (0x00000001&((data)<<0))
#define HDMI_INTSTV_vsyncupdated_src(data)                                           ((0x00000001&(data))>>0)
#define HDMI_INTSTV_get_vsyncupdated(data)                                           ((0x00000001&(data))>>0)


#define HDMI_PHY_STATUS                                                              0x1800D15c
#define HDMI_PHY_STATUS_reg_addr                                                     "0xB800D15C"
#define HDMI_PHY_STATUS_reg                                                          0xB800D15C
#define set_HDMI_PHY_STATUS_reg(data)   (*((volatile unsigned int*) HDMI_PHY_STATUS_reg)=data)
#define get_HDMI_PHY_STATUS_reg   (*((volatile unsigned int*) HDMI_PHY_STATUS_reg))
#define HDMI_PHY_STATUS_inst_adr                                                     "0x0057"
#define HDMI_PHY_STATUS_inst                                                         0x0057
#define HDMI_PHY_STATUS_wdout_shift                                                  (1)
#define HDMI_PHY_STATUS_wdout_mask                                                   (0x00000002)
#define HDMI_PHY_STATUS_wdout(data)                                                  (0x00000002&((data)<<1))
#define HDMI_PHY_STATUS_wdout_src(data)                                              ((0x00000002&(data))>>1)
#define HDMI_PHY_STATUS_get_wdout(data)                                              ((0x00000002&(data))>>1)
#define HDMI_PHY_STATUS_Rxstatus_shift                                               (0)
#define HDMI_PHY_STATUS_Rxstatus_mask                                                (0x00000001)
#define HDMI_PHY_STATUS_Rxstatus(data)                                               (0x00000001&((data)<<0))
#define HDMI_PHY_STATUS_Rxstatus_src(data)                                           ((0x00000001&(data))>>0)
#define HDMI_PHY_STATUS_get_Rxstatus(data)                                           ((0x00000001&(data))>>0)


#define HDMI_UPDATE_ST                                                               0x1800D00c
#define HDMI_UPDATE_ST_reg_addr                                                      "0xB800D00C"
#define HDMI_UPDATE_ST_reg                                                           0xB800D00C
#define set_HDMI_UPDATE_ST_reg(data)   (*((volatile unsigned int*) HDMI_UPDATE_ST_reg)=data)
#define get_HDMI_UPDATE_ST_reg   (*((volatile unsigned int*) HDMI_UPDATE_ST_reg))
#define HDMI_UPDATE_ST_inst_adr                                                      "0x0003"
#define HDMI_UPDATE_ST_inst                                                          0x0003
#define HDMI_UPDATE_ST_write_en2_shift                                               (17)
#define HDMI_UPDATE_ST_write_en2_mask                                                (0x00020000)
#define HDMI_UPDATE_ST_write_en2(data)                                               (0x00020000&((data)<<17))
#define HDMI_UPDATE_ST_write_en2_src(data)                                           ((0x00020000&(data))>>17)
#define HDMI_UPDATE_ST_get_write_en2(data)                                           ((0x00020000&(data))>>17)
#define HDMI_UPDATE_ST_rirate_shift                                                  (9)
#define HDMI_UPDATE_ST_rirate_mask                                                   (0x0001FE00)
#define HDMI_UPDATE_ST_rirate(data)                                                  (0x0001FE00&((data)<<9))
#define HDMI_UPDATE_ST_rirate_src(data)                                              ((0x0001FE00&(data))>>9)
#define HDMI_UPDATE_ST_get_rirate(data)                                              ((0x0001FE00&(data))>>9)
#define HDMI_UPDATE_ST_write_en1_shift                                               (8)
#define HDMI_UPDATE_ST_write_en1_mask                                                (0x00000100)
#define HDMI_UPDATE_ST_write_en1(data)                                               (0x00000100&((data)<<8))
#define HDMI_UPDATE_ST_write_en1_src(data)                                           ((0x00000100&(data))>>8)
#define HDMI_UPDATE_ST_get_write_en1(data)                                           ((0x00000100&(data))>>8)
#define HDMI_UPDATE_ST_pjrate_shift                                                  (0)
#define HDMI_UPDATE_ST_pjrate_mask                                                   (0x000000FF)
#define HDMI_UPDATE_ST_pjrate(data)                                                  (0x000000FF&((data)<<0))
#define HDMI_UPDATE_ST_pjrate_src(data)                                              ((0x000000FF&(data))>>0)
#define HDMI_UPDATE_ST_get_pjrate(data)                                              ((0x000000FF&(data))>>0)


#define HDMI_CHNL_SEL                                                                0x1800D020
#define HDMI_CHNL_SEL_reg_addr                                                       "0xB800D020"
#define HDMI_CHNL_SEL_reg                                                            0xB800D020
#define set_HDMI_CHNL_SEL_reg(data)   (*((volatile unsigned int*) HDMI_CHNL_SEL_reg)=data)
#define get_HDMI_CHNL_SEL_reg   (*((volatile unsigned int*) HDMI_CHNL_SEL_reg))
#define HDMI_CHNL_SEL_inst_adr                                                       "0x0008"
#define HDMI_CHNL_SEL_inst                                                           0x0008
#define HDMI_CHNL_SEL_video_sd_shift                                                 (9)
#define HDMI_CHNL_SEL_video_sd_mask                                                  (0x00000200)
#define HDMI_CHNL_SEL_video_sd(data)                                                 (0x00000200&((data)<<9))
#define HDMI_CHNL_SEL_video_sd_src(data)                                             ((0x00000200&(data))>>9)
#define HDMI_CHNL_SEL_get_video_sd(data)                                             ((0x00000200&(data))>>9)
#define HDMI_CHNL_SEL_Interlaced_vfmt_shift                                          (8)
#define HDMI_CHNL_SEL_Interlaced_vfmt_mask                                           (0x00000100)
#define HDMI_CHNL_SEL_Interlaced_vfmt(data)                                          (0x00000100&((data)<<8))
#define HDMI_CHNL_SEL_Interlaced_vfmt_src(data)                                      ((0x00000100&(data))>>8)
#define HDMI_CHNL_SEL_get_Interlaced_vfmt(data)                                      ((0x00000100&(data))>>8)
#define HDMI_CHNL_SEL_3D_video_format_shift                                          (4)
#define HDMI_CHNL_SEL_3D_video_format_mask                                           (0x000000F0)
#define HDMI_CHNL_SEL_3D_video_format(data)                                          (0x000000F0&((data)<<4))
#define HDMI_CHNL_SEL_3D_video_format_src(data)                                      ((0x000000F0&(data))>>4)
#define HDMI_CHNL_SEL_get_3D_video_format(data)                                      ((0x000000F0&(data))>>4)
#define HDMI_CHNL_SEL_En_3D_shift                                                    (3)
#define HDMI_CHNL_SEL_En_3D_mask                                                     (0x00000008)
#define HDMI_CHNL_SEL_En_3D(data)                                                    (0x00000008&((data)<<3))
#define HDMI_CHNL_SEL_En_3D_src(data)                                                ((0x00000008&(data))>>3)
#define HDMI_CHNL_SEL_get_En_3D(data)                                                ((0x00000008&(data))>>3)
#define HDMI_CHNL_SEL_timing_gen_en_shift                                            (2)
#define HDMI_CHNL_SEL_timing_gen_en_mask                                             (0x00000004)
#define HDMI_CHNL_SEL_timing_gen_en(data)                                            (0x00000004&((data)<<2))
#define HDMI_CHNL_SEL_timing_gen_en_src(data)                                        ((0x00000004&(data))>>2)
#define HDMI_CHNL_SEL_get_timing_gen_en(data)                                        ((0x00000004&(data))>>2)
#define HDMI_CHNL_SEL_chl_sel_shift                                                  (0)
#define HDMI_CHNL_SEL_chl_sel_mask                                                   (0x00000003)
#define HDMI_CHNL_SEL_chl_sel(data)                                                  (0x00000003&((data)<<0))
#define HDMI_CHNL_SEL_chl_sel_src(data)                                              ((0x00000003&(data))>>0)
#define HDMI_CHNL_SEL_get_chl_sel(data)                                              ((0x00000003&(data))>>0)


#define HDMI_CS_TRANS0                                                               0x1800D024
#define HDMI_CS_TRANS0_reg_addr                                                      "0xB800D024"
#define HDMI_CS_TRANS0_reg                                                           0xB800D024
#define set_HDMI_CS_TRANS0_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS0_reg)=data)
#define get_HDMI_CS_TRANS0_reg   (*((volatile unsigned int*) HDMI_CS_TRANS0_reg))
#define HDMI_CS_TRANS0_inst_adr                                                      "0x0009"
#define HDMI_CS_TRANS0_inst                                                          0x0009
#define HDMI_CS_TRANS0_c1_shift                                                      (0)
#define HDMI_CS_TRANS0_c1_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS0_c1(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS0_c1_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS0_get_c1(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS1                                                               0x1800D028
#define HDMI_CS_TRANS1_reg_addr                                                      "0xB800D028"
#define HDMI_CS_TRANS1_reg                                                           0xB800D028
#define set_HDMI_CS_TRANS1_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS1_reg)=data)
#define get_HDMI_CS_TRANS1_reg   (*((volatile unsigned int*) HDMI_CS_TRANS1_reg))
#define HDMI_CS_TRANS1_inst_adr                                                      "0x000A"
#define HDMI_CS_TRANS1_inst                                                          0x000A
#define HDMI_CS_TRANS1_c2_shift                                                      (0)
#define HDMI_CS_TRANS1_c2_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS1_c2(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS1_c2_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS1_get_c2(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS2                                                               0x1800D02c
#define HDMI_CS_TRANS2_reg_addr                                                      "0xB800D02C"
#define HDMI_CS_TRANS2_reg                                                           0xB800D02C
#define set_HDMI_CS_TRANS2_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS2_reg)=data)
#define get_HDMI_CS_TRANS2_reg   (*((volatile unsigned int*) HDMI_CS_TRANS2_reg))
#define HDMI_CS_TRANS2_inst_adr                                                      "0x000B"
#define HDMI_CS_TRANS2_inst                                                          0x000B
#define HDMI_CS_TRANS2_c3_shift                                                      (0)
#define HDMI_CS_TRANS2_c3_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS2_c3(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS2_c3_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS2_get_c3(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS3                                                               0x1800D200
#define HDMI_CS_TRANS3_reg_addr                                                      "0xB800D200"
#define HDMI_CS_TRANS3_reg                                                           0xB800D200
#define set_HDMI_CS_TRANS3_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS3_reg)=data)
#define get_HDMI_CS_TRANS3_reg   (*((volatile unsigned int*) HDMI_CS_TRANS3_reg))
#define HDMI_CS_TRANS3_inst_adr                                                      "0x0080"
#define HDMI_CS_TRANS3_inst                                                          0x0080
#define HDMI_CS_TRANS3_c4_shift                                                      (0)
#define HDMI_CS_TRANS3_c4_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS3_c4(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS3_c4_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS3_get_c4(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS4                                                               0x1800D204
#define HDMI_CS_TRANS4_reg_addr                                                      "0xB800D204"
#define HDMI_CS_TRANS4_reg                                                           0xB800D204
#define set_HDMI_CS_TRANS4_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS4_reg)=data)
#define get_HDMI_CS_TRANS4_reg   (*((volatile unsigned int*) HDMI_CS_TRANS4_reg))
#define HDMI_CS_TRANS4_inst_adr                                                      "0x0081"
#define HDMI_CS_TRANS4_inst                                                          0x0081
#define HDMI_CS_TRANS4_c5_shift                                                      (0)
#define HDMI_CS_TRANS4_c5_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS4_c5(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS4_c5_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS4_get_c5(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS5                                                               0x1800D208
#define HDMI_CS_TRANS5_reg_addr                                                      "0xB800D208"
#define HDMI_CS_TRANS5_reg                                                           0xB800D208
#define set_HDMI_CS_TRANS5_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS5_reg)=data)
#define get_HDMI_CS_TRANS5_reg   (*((volatile unsigned int*) HDMI_CS_TRANS5_reg))
#define HDMI_CS_TRANS5_inst_adr                                                      "0x0082"
#define HDMI_CS_TRANS5_inst                                                          0x0082
#define HDMI_CS_TRANS5_c6_shift                                                      (0)
#define HDMI_CS_TRANS5_c6_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS5_c6(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS5_c6_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS5_get_c6(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS6                                                               0x1800D20c
#define HDMI_CS_TRANS6_reg_addr                                                      "0xB800D20C"
#define HDMI_CS_TRANS6_reg                                                           0xB800D20C
#define set_HDMI_CS_TRANS6_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS6_reg)=data)
#define get_HDMI_CS_TRANS6_reg   (*((volatile unsigned int*) HDMI_CS_TRANS6_reg))
#define HDMI_CS_TRANS6_inst_adr                                                      "0x0083"
#define HDMI_CS_TRANS6_inst                                                          0x0083
#define HDMI_CS_TRANS6_c7_shift                                                      (0)
#define HDMI_CS_TRANS6_c7_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS6_c7(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS6_c7_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS6_get_c7(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS7                                                               0x1800D210
#define HDMI_CS_TRANS7_reg_addr                                                      "0xB800D210"
#define HDMI_CS_TRANS7_reg                                                           0xB800D210
#define set_HDMI_CS_TRANS7_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS7_reg)=data)
#define get_HDMI_CS_TRANS7_reg   (*((volatile unsigned int*) HDMI_CS_TRANS7_reg))
#define HDMI_CS_TRANS7_inst_adr                                                      "0x0084"
#define HDMI_CS_TRANS7_inst                                                          0x0084
#define HDMI_CS_TRANS7_c8_shift                                                      (0)
#define HDMI_CS_TRANS7_c8_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS7_c8(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS7_c8_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS7_get_c8(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS8                                                               0x1800D214
#define HDMI_CS_TRANS8_reg_addr                                                      "0xB800D214"
#define HDMI_CS_TRANS8_reg                                                           0xB800D214
#define set_HDMI_CS_TRANS8_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS8_reg)=data)
#define get_HDMI_CS_TRANS8_reg   (*((volatile unsigned int*) HDMI_CS_TRANS8_reg))
#define HDMI_CS_TRANS8_inst_adr                                                      "0x0085"
#define HDMI_CS_TRANS8_inst                                                          0x0085
#define HDMI_CS_TRANS8_c9_shift                                                      (0)
#define HDMI_CS_TRANS8_c9_mask                                                       (0x0001FFFF)
#define HDMI_CS_TRANS8_c9(data)                                                      (0x0001FFFF&((data)<<0))
#define HDMI_CS_TRANS8_c9_src(data)                                                  ((0x0001FFFF&(data))>>0)
#define HDMI_CS_TRANS8_get_c9(data)                                                  ((0x0001FFFF&(data))>>0)


#define HDMI_CS_TRANS9                                                               0x1800D218
#define HDMI_CS_TRANS9_reg_addr                                                      "0xB800D218"
#define HDMI_CS_TRANS9_reg                                                           0xB800D218
#define set_HDMI_CS_TRANS9_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS9_reg)=data)
#define get_HDMI_CS_TRANS9_reg   (*((volatile unsigned int*) HDMI_CS_TRANS9_reg))
#define HDMI_CS_TRANS9_inst_adr                                                      "0x0086"
#define HDMI_CS_TRANS9_inst                                                          0x0086
#define HDMI_CS_TRANS9_k1_shift                                                      (0)
#define HDMI_CS_TRANS9_k1_mask                                                       (0x0000FFFF)
#define HDMI_CS_TRANS9_k1(data)                                                      (0x0000FFFF&((data)<<0))
#define HDMI_CS_TRANS9_k1_src(data)                                                  ((0x0000FFFF&(data))>>0)
#define HDMI_CS_TRANS9_get_k1(data)                                                  ((0x0000FFFF&(data))>>0)


#define HDMI_CS_TRANS10                                                              0x1800D21c
#define HDMI_CS_TRANS10_reg_addr                                                     "0xB800D21C"
#define HDMI_CS_TRANS10_reg                                                          0xB800D21C
#define set_HDMI_CS_TRANS10_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS10_reg)=data)
#define get_HDMI_CS_TRANS10_reg   (*((volatile unsigned int*) HDMI_CS_TRANS10_reg))
#define HDMI_CS_TRANS10_inst_adr                                                     "0x0087"
#define HDMI_CS_TRANS10_inst                                                         0x0087
#define HDMI_CS_TRANS10_k2_shift                                                     (0)
#define HDMI_CS_TRANS10_k2_mask                                                      (0x0000FFFF)
#define HDMI_CS_TRANS10_k2(data)                                                     (0x0000FFFF&((data)<<0))
#define HDMI_CS_TRANS10_k2_src(data)                                                 ((0x0000FFFF&(data))>>0)
#define HDMI_CS_TRANS10_get_k2(data)                                                 ((0x0000FFFF&(data))>>0)


#define HDMI_CS_TRANS11                                                              0x1800D220
#define HDMI_CS_TRANS11_reg_addr                                                     "0xB800D220"
#define HDMI_CS_TRANS11_reg                                                          0xB800D220
#define set_HDMI_CS_TRANS11_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS11_reg)=data)
#define get_HDMI_CS_TRANS11_reg   (*((volatile unsigned int*) HDMI_CS_TRANS11_reg))
#define HDMI_CS_TRANS11_inst_adr                                                     "0x0088"
#define HDMI_CS_TRANS11_inst                                                         0x0088
#define HDMI_CS_TRANS11_k3_shift                                                     (0)
#define HDMI_CS_TRANS11_k3_mask                                                      (0x0000FFFF)
#define HDMI_CS_TRANS11_k3(data)                                                     (0x0000FFFF&((data)<<0))
#define HDMI_CS_TRANS11_k3_src(data)                                                 ((0x0000FFFF&(data))>>0)
#define HDMI_CS_TRANS11_get_k3(data)                                                 ((0x0000FFFF&(data))>>0)


#define HDMI_CS_TRANS12                                                              0x1800D224
#define HDMI_CS_TRANS12_reg_addr                                                     "0xB800D224"
#define HDMI_CS_TRANS12_reg                                                          0xB800D224
#define set_HDMI_CS_TRANS12_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS12_reg)=data)
#define get_HDMI_CS_TRANS12_reg   (*((volatile unsigned int*) HDMI_CS_TRANS12_reg))
#define HDMI_CS_TRANS12_inst_adr                                                     "0x0089"
#define HDMI_CS_TRANS12_inst                                                         0x0089
#define HDMI_CS_TRANS12_write_en2_shift                                              (28)
#define HDMI_CS_TRANS12_write_en2_mask                                               (0x10000000)
#define HDMI_CS_TRANS12_write_en2(data)                                              (0x10000000&((data)<<28))
#define HDMI_CS_TRANS12_write_en2_src(data)                                          ((0x10000000&(data))>>28)
#define HDMI_CS_TRANS12_get_write_en2(data)                                          ((0x10000000&(data))>>28)
#define HDMI_CS_TRANS12_ygmax_shift                                                  (16)
#define HDMI_CS_TRANS12_ygmax_mask                                                   (0x0FFF0000)
#define HDMI_CS_TRANS12_ygmax(data)                                                  (0x0FFF0000&((data)<<16))
#define HDMI_CS_TRANS12_ygmax_src(data)                                              ((0x0FFF0000&(data))>>16)
#define HDMI_CS_TRANS12_get_ygmax(data)                                              ((0x0FFF0000&(data))>>16)
#define HDMI_CS_TRANS12_write_en1_shift                                              (12)
#define HDMI_CS_TRANS12_write_en1_mask                                               (0x00001000)
#define HDMI_CS_TRANS12_write_en1(data)                                              (0x00001000&((data)<<12))
#define HDMI_CS_TRANS12_write_en1_src(data)                                          ((0x00001000&(data))>>12)
#define HDMI_CS_TRANS12_get_write_en1(data)                                          ((0x00001000&(data))>>12)
#define HDMI_CS_TRANS12_ygmin_shift                                                  (0)
#define HDMI_CS_TRANS12_ygmin_mask                                                   (0x00000FFF)
#define HDMI_CS_TRANS12_ygmin(data)                                                  (0x00000FFF&((data)<<0))
#define HDMI_CS_TRANS12_ygmin_src(data)                                              ((0x00000FFF&(data))>>0)
#define HDMI_CS_TRANS12_get_ygmin(data)                                              ((0x00000FFF&(data))>>0)


#define HDMI_CS_TRANS13                                                              0x1800D228
#define HDMI_CS_TRANS13_reg_addr                                                     "0xB800D228"
#define HDMI_CS_TRANS13_reg                                                          0xB800D228
#define set_HDMI_CS_TRANS13_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS13_reg)=data)
#define get_HDMI_CS_TRANS13_reg   (*((volatile unsigned int*) HDMI_CS_TRANS13_reg))
#define HDMI_CS_TRANS13_inst_adr                                                     "0x008A"
#define HDMI_CS_TRANS13_inst                                                         0x008A
#define HDMI_CS_TRANS13_write_en2_shift                                              (28)
#define HDMI_CS_TRANS13_write_en2_mask                                               (0x10000000)
#define HDMI_CS_TRANS13_write_en2(data)                                              (0x10000000&((data)<<28))
#define HDMI_CS_TRANS13_write_en2_src(data)                                          ((0x10000000&(data))>>28)
#define HDMI_CS_TRANS13_get_write_en2(data)                                          ((0x10000000&(data))>>28)
#define HDMI_CS_TRANS13_cbbmax_shift                                                 (16)
#define HDMI_CS_TRANS13_cbbmax_mask                                                  (0x0FFF0000)
#define HDMI_CS_TRANS13_cbbmax(data)                                                 (0x0FFF0000&((data)<<16))
#define HDMI_CS_TRANS13_cbbmax_src(data)                                             ((0x0FFF0000&(data))>>16)
#define HDMI_CS_TRANS13_get_cbbmax(data)                                             ((0x0FFF0000&(data))>>16)
#define HDMI_CS_TRANS13_write_en1_shift                                              (12)
#define HDMI_CS_TRANS13_write_en1_mask                                               (0x00001000)
#define HDMI_CS_TRANS13_write_en1(data)                                              (0x00001000&((data)<<12))
#define HDMI_CS_TRANS13_write_en1_src(data)                                          ((0x00001000&(data))>>12)
#define HDMI_CS_TRANS13_get_write_en1(data)                                          ((0x00001000&(data))>>12)
#define HDMI_CS_TRANS13_cbbmin_shift                                                 (0)
#define HDMI_CS_TRANS13_cbbmin_mask                                                  (0x00000FFF)
#define HDMI_CS_TRANS13_cbbmin(data)                                                 (0x00000FFF&((data)<<0))
#define HDMI_CS_TRANS13_cbbmin_src(data)                                             ((0x00000FFF&(data))>>0)
#define HDMI_CS_TRANS13_get_cbbmin(data)                                             ((0x00000FFF&(data))>>0)


#define HDMI_CS_TRANS14                                                              0x1800D22c
#define HDMI_CS_TRANS14_reg_addr                                                     "0xB800D22C"
#define HDMI_CS_TRANS14_reg                                                          0xB800D22C
#define set_HDMI_CS_TRANS14_reg(data)   (*((volatile unsigned int*) HDMI_CS_TRANS14_reg)=data)
#define get_HDMI_CS_TRANS14_reg   (*((volatile unsigned int*) HDMI_CS_TRANS14_reg))
#define HDMI_CS_TRANS14_inst_adr                                                     "0x008B"
#define HDMI_CS_TRANS14_inst                                                         0x008B
#define HDMI_CS_TRANS14_write_en2_shift                                              (28)
#define HDMI_CS_TRANS14_write_en2_mask                                               (0x10000000)
#define HDMI_CS_TRANS14_write_en2(data)                                              (0x10000000&((data)<<28))
#define HDMI_CS_TRANS14_write_en2_src(data)                                          ((0x10000000&(data))>>28)
#define HDMI_CS_TRANS14_get_write_en2(data)                                          ((0x10000000&(data))>>28)
#define HDMI_CS_TRANS14_crrmax_shift                                                 (16)
#define HDMI_CS_TRANS14_crrmax_mask                                                  (0x0FFF0000)
#define HDMI_CS_TRANS14_crrmax(data)                                                 (0x0FFF0000&((data)<<16))
#define HDMI_CS_TRANS14_crrmax_src(data)                                             ((0x0FFF0000&(data))>>16)
#define HDMI_CS_TRANS14_get_crrmax(data)                                             ((0x0FFF0000&(data))>>16)
#define HDMI_CS_TRANS14_write_en1_shift                                              (12)
#define HDMI_CS_TRANS14_write_en1_mask                                               (0x00001000)
#define HDMI_CS_TRANS14_write_en1(data)                                              (0x00001000&((data)<<12))
#define HDMI_CS_TRANS14_write_en1_src(data)                                          ((0x00001000&(data))>>12)
#define HDMI_CS_TRANS14_get_write_en1(data)                                          ((0x00001000&(data))>>12)
#define HDMI_CS_TRANS14_crrmin_shift                                                 (0)
#define HDMI_CS_TRANS14_crrmin_mask                                                  (0x00000FFF)
#define HDMI_CS_TRANS14_crrmin(data)                                                 (0x00000FFF&((data)<<0))
#define HDMI_CS_TRANS14_crrmin_src(data)                                             ((0x00000FFF&(data))>>0)
#define HDMI_CS_TRANS14_get_crrmin(data)                                             ((0x00000FFF&(data))>>0)


#define HDMI_SYNC_DLY                                                                0x1800D030
#define HDMI_SYNC_DLY_reg_addr                                                       "0xB800D030"
#define HDMI_SYNC_DLY_reg                                                            0xB800D030
#define set_HDMI_SYNC_DLY_reg(data)   (*((volatile unsigned int*) HDMI_SYNC_DLY_reg)=data)
#define get_HDMI_SYNC_DLY_reg   (*((volatile unsigned int*) HDMI_SYNC_DLY_reg))
#define HDMI_SYNC_DLY_inst_adr                                                       "0x000C"
#define HDMI_SYNC_DLY_inst                                                           0x000C
#define HDMI_SYNC_DLY_write_en4_shift                                                (11)
#define HDMI_SYNC_DLY_write_en4_mask                                                 (0x00000800)
#define HDMI_SYNC_DLY_write_en4(data)                                                (0x00000800&((data)<<11))
#define HDMI_SYNC_DLY_write_en4_src(data)                                            ((0x00000800&(data))>>11)
#define HDMI_SYNC_DLY_get_write_en4(data)                                            ((0x00000800&(data))>>11)
#define HDMI_SYNC_DLY_nor_v_shift                                                    (9)
#define HDMI_SYNC_DLY_nor_v_mask                                                     (0x00000600)
#define HDMI_SYNC_DLY_nor_v(data)                                                    (0x00000600&((data)<<9))
#define HDMI_SYNC_DLY_nor_v_src(data)                                                ((0x00000600&(data))>>9)
#define HDMI_SYNC_DLY_get_nor_v(data)                                                ((0x00000600&(data))>>9)
#define HDMI_SYNC_DLY_write_en3_shift                                                (8)
#define HDMI_SYNC_DLY_write_en3_mask                                                 (0x00000100)
#define HDMI_SYNC_DLY_write_en3(data)                                                (0x00000100&((data)<<8))
#define HDMI_SYNC_DLY_write_en3_src(data)                                            ((0x00000100&(data))>>8)
#define HDMI_SYNC_DLY_get_write_en3(data)                                            ((0x00000100&(data))>>8)
#define HDMI_SYNC_DLY_nor_h_shift                                                    (6)
#define HDMI_SYNC_DLY_nor_h_mask                                                     (0x000000C0)
#define HDMI_SYNC_DLY_nor_h(data)                                                    (0x000000C0&((data)<<6))
#define HDMI_SYNC_DLY_nor_h_src(data)                                                ((0x000000C0&(data))>>6)
#define HDMI_SYNC_DLY_get_nor_h(data)                                                ((0x000000C0&(data))>>6)
#define HDMI_SYNC_DLY_write_en2_shift                                                (5)
#define HDMI_SYNC_DLY_write_en2_mask                                                 (0x00000020)
#define HDMI_SYNC_DLY_write_en2(data)                                                (0x00000020&((data)<<5))
#define HDMI_SYNC_DLY_write_en2_src(data)                                            ((0x00000020&(data))>>5)
#define HDMI_SYNC_DLY_get_write_en2(data)                                            ((0x00000020&(data))>>5)
#define HDMI_SYNC_DLY_disp_v_shift                                                   (3)
#define HDMI_SYNC_DLY_disp_v_mask                                                    (0x00000018)
#define HDMI_SYNC_DLY_disp_v(data)                                                   (0x00000018&((data)<<3))
#define HDMI_SYNC_DLY_disp_v_src(data)                                               ((0x00000018&(data))>>3)
#define HDMI_SYNC_DLY_get_disp_v(data)                                               ((0x00000018&(data))>>3)
#define HDMI_SYNC_DLY_write_en1_shift                                                (2)
#define HDMI_SYNC_DLY_write_en1_mask                                                 (0x00000004)
#define HDMI_SYNC_DLY_write_en1(data)                                                (0x00000004&((data)<<2))
#define HDMI_SYNC_DLY_write_en1_src(data)                                            ((0x00000004&(data))>>2)
#define HDMI_SYNC_DLY_get_write_en1(data)                                            ((0x00000004&(data))>>2)
#define HDMI_SYNC_DLY_disp_h_shift                                                   (0)
#define HDMI_SYNC_DLY_disp_h_mask                                                    (0x00000003)
#define HDMI_SYNC_DLY_disp_h(data)                                                   (0x00000003&((data)<<0))
#define HDMI_SYNC_DLY_disp_h_src(data)                                               ((0x00000003&(data))>>0)
#define HDMI_SYNC_DLY_get_disp_h(data)                                               ((0x00000003&(data))>>0)


#define HDMI_CR                                                                      0x1800D034
#define HDMI_CR_reg_addr                                                             "0xB800D034"
#define HDMI_CR_reg                                                                  0xB800D034
#define set_HDMI_CR_reg(data)   (*((volatile unsigned int*) HDMI_CR_reg)=data)
#define get_HDMI_CR_reg   (*((volatile unsigned int*) HDMI_CR_reg))
#define HDMI_CR_inst_adr                                                             "0x000D"
#define HDMI_CR_inst                                                                 0x000D
#define HDMI_CR_write_en3_shift                                                      (5)
#define HDMI_CR_write_en3_mask                                                       (0x00000020)
#define HDMI_CR_write_en3(data)                                                      (0x00000020&((data)<<5))
#define HDMI_CR_write_en3_src(data)                                                  ((0x00000020&(data))>>5)
#define HDMI_CR_get_write_en3(data)                                                  ((0x00000020&(data))>>5)
#define HDMI_CR_tmds_encen_shift                                                     (4)
#define HDMI_CR_tmds_encen_mask                                                      (0x00000010)
#define HDMI_CR_tmds_encen(data)                                                     (0x00000010&((data)<<4))
#define HDMI_CR_tmds_encen_src(data)                                                 ((0x00000010&(data))>>4)
#define HDMI_CR_get_tmds_encen(data)                                                 ((0x00000010&(data))>>4)
#define HDMI_CR_write_en2_shift                                                      (3)
#define HDMI_CR_write_en2_mask                                                       (0x00000008)
#define HDMI_CR_write_en2(data)                                                      (0x00000008&((data)<<3))
#define HDMI_CR_write_en2_src(data)                                                  ((0x00000008&(data))>>3)
#define HDMI_CR_get_write_en2(data)                                                  ((0x00000008&(data))>>3)
#define HDMI_CR_enablehdcp_shift                                                     (2)
#define HDMI_CR_enablehdcp_mask                                                      (0x00000004)
#define HDMI_CR_enablehdcp(data)                                                     (0x00000004&((data)<<2))
#define HDMI_CR_enablehdcp_src(data)                                                 ((0x00000004&(data))>>2)
#define HDMI_CR_get_enablehdcp(data)                                                 ((0x00000004&(data))>>2)
#define HDMI_CR_write_en1_shift                                                      (1)
#define HDMI_CR_write_en1_mask                                                       (0x00000002)
#define HDMI_CR_write_en1(data)                                                      (0x00000002&((data)<<1))
#define HDMI_CR_write_en1_src(data)                                                  ((0x00000002&(data))>>1)
#define HDMI_CR_get_write_en1(data)                                                  ((0x00000002&(data))>>1)
#define HDMI_CR_enablehdmi_shift                                                     (0)
#define HDMI_CR_enablehdmi_mask                                                      (0x00000001)
#define HDMI_CR_enablehdmi(data)                                                     (0x00000001&((data)<<0))
#define HDMI_CR_enablehdmi_src(data)                                                 ((0x00000001&(data))>>0)
#define HDMI_CR_get_enablehdmi(data)                                                 ((0x00000001&(data))>>0)


#define HDMI_DPC                                                                     0x1800D154
#define HDMI_DPC_reg_addr                                                            "0xB800D154"
#define HDMI_DPC_reg                                                                 0xB800D154
#define set_HDMI_DPC_reg(data)   (*((volatile unsigned int*) HDMI_DPC_reg)=data)
#define get_HDMI_DPC_reg   (*((volatile unsigned int*) HDMI_DPC_reg))
#define HDMI_DPC_inst_adr                                                            "0x0055"
#define HDMI_DPC_inst                                                                0x0055
#define HDMI_DPC_write_en9_shift                                                     (30)
#define HDMI_DPC_write_en9_mask                                                      (0x40000000)
#define HDMI_DPC_write_en9(data)                                                     (0x40000000&((data)<<30))
#define HDMI_DPC_write_en9_src(data)                                                 ((0x40000000&(data))>>30)
#define HDMI_DPC_get_write_en9(data)                                                 ((0x40000000&(data))>>30)
#define HDMI_DPC_dp_vfch_num_shift                                                   (28)
#define HDMI_DPC_dp_vfch_num_mask                                                    (0x30000000)
#define HDMI_DPC_dp_vfch_num(data)                                                   (0x30000000&((data)<<28))
#define HDMI_DPC_dp_vfch_num_src(data)                                               ((0x30000000&(data))>>28)
#define HDMI_DPC_get_dp_vfch_num(data)                                               ((0x30000000&(data))>>28)
#define HDMI_DPC_write_en8_shift                                                     (25)
#define HDMI_DPC_write_en8_mask                                                      (0x02000000)
#define HDMI_DPC_write_en8(data)                                                     (0x02000000&((data)<<25))
#define HDMI_DPC_write_en8_src(data)                                                 ((0x02000000&(data))>>25)
#define HDMI_DPC_get_write_en8(data)                                                 ((0x02000000&(data))>>25)
#define HDMI_DPC_fp_swen_shift                                                       (24)
#define HDMI_DPC_fp_swen_mask                                                        (0x01000000)
#define HDMI_DPC_fp_swen(data)                                                       (0x01000000&((data)<<24))
#define HDMI_DPC_fp_swen_src(data)                                                   ((0x01000000&(data))>>24)
#define HDMI_DPC_get_fp_swen(data)                                                   ((0x01000000&(data))>>24)
#define HDMI_DPC_write_en7_shift                                                     (23)
#define HDMI_DPC_write_en7_mask                                                      (0x00800000)
#define HDMI_DPC_write_en7(data)                                                     (0x00800000&((data)<<23))
#define HDMI_DPC_write_en7_src(data)                                                 ((0x00800000&(data))>>23)
#define HDMI_DPC_get_write_en7(data)                                                 ((0x00800000&(data))>>23)
#define HDMI_DPC_fp_shift                                                            (20)
#define HDMI_DPC_fp_mask                                                             (0x00700000)
#define HDMI_DPC_fp(data)                                                            (0x00700000&((data)<<20))
#define HDMI_DPC_fp_src(data)                                                        ((0x00700000&(data))>>20)
#define HDMI_DPC_get_fp(data)                                                        ((0x00700000&(data))>>20)
#define HDMI_DPC_write_en6_shift                                                     (19)
#define HDMI_DPC_write_en6_mask                                                      (0x00080000)
#define HDMI_DPC_write_en6(data)                                                     (0x00080000&((data)<<19))
#define HDMI_DPC_write_en6_src(data)                                                 ((0x00080000&(data))>>19)
#define HDMI_DPC_get_write_en6(data)                                                 ((0x00080000&(data))>>19)
#define HDMI_DPC_pp_swen_shift                                                       (18)
#define HDMI_DPC_pp_swen_mask                                                        (0x00040000)
#define HDMI_DPC_pp_swen(data)                                                       (0x00040000&((data)<<18))
#define HDMI_DPC_pp_swen_src(data)                                                   ((0x00040000&(data))>>18)
#define HDMI_DPC_get_pp_swen(data)                                                   ((0x00040000&(data))>>18)
#define HDMI_DPC_write_en4_shift                                                     (15)
#define HDMI_DPC_write_en4_mask                                                      (0x00008000)
#define HDMI_DPC_write_en4(data)                                                     (0x00008000&((data)<<15))
#define HDMI_DPC_write_en4_src(data)                                                 ((0x00008000&(data))>>15)
#define HDMI_DPC_get_write_en4(data)                                                 ((0x00008000&(data))>>15)
#define HDMI_DPC_dp_swen_shift                                                       (14)
#define HDMI_DPC_dp_swen_mask                                                        (0x00004000)
#define HDMI_DPC_dp_swen(data)                                                       (0x00004000&((data)<<14))
#define HDMI_DPC_dp_swen_src(data)                                                   ((0x00004000&(data))>>14)
#define HDMI_DPC_get_dp_swen(data)                                                   ((0x00004000&(data))>>14)
#define HDMI_DPC_write_en3_shift                                                     (13)
#define HDMI_DPC_write_en3_mask                                                      (0x00002000)
#define HDMI_DPC_write_en3(data)                                                     (0x00002000&((data)<<13))
#define HDMI_DPC_write_en3_src(data)                                                 ((0x00002000&(data))>>13)
#define HDMI_DPC_get_write_en3(data)                                                 ((0x00002000&(data))>>13)
#define HDMI_DPC_default_phase_shift                                                 (12)
#define HDMI_DPC_default_phase_mask                                                  (0x00001000)
#define HDMI_DPC_default_phase(data)                                                 (0x00001000&((data)<<12))
#define HDMI_DPC_default_phase_src(data)                                             ((0x00001000&(data))>>12)
#define HDMI_DPC_get_default_phase(data)                                             ((0x00001000&(data))>>12)
#define HDMI_DPC_write_en2_shift                                                     (8)
#define HDMI_DPC_write_en2_mask                                                      (0x00000100)
#define HDMI_DPC_write_en2(data)                                                     (0x00000100&((data)<<8))
#define HDMI_DPC_write_en2_src(data)                                                 ((0x00000100&(data))>>8)
#define HDMI_DPC_get_write_en2(data)                                                 ((0x00000100&(data))>>8)
#define HDMI_DPC_color_depth_shift                                                   (4)
#define HDMI_DPC_color_depth_mask                                                    (0x000000F0)
#define HDMI_DPC_color_depth(data)                                                   (0x000000F0&((data)<<4))
#define HDMI_DPC_color_depth_src(data)                                               ((0x000000F0&(data))>>4)
#define HDMI_DPC_get_color_depth(data)                                               ((0x000000F0&(data))>>4)
#define HDMI_DPC_write_en1_shift                                                     (1)
#define HDMI_DPC_write_en1_mask                                                      (0x00000002)
#define HDMI_DPC_write_en1(data)                                                     (0x00000002&((data)<<1))
#define HDMI_DPC_write_en1_src(data)                                                 ((0x00000002&(data))>>1)
#define HDMI_DPC_get_write_en1(data)                                                 ((0x00000002&(data))>>1)
#define HDMI_DPC_dpc_enable_shift                                                    (0)
#define HDMI_DPC_dpc_enable_mask                                                     (0x00000001)
#define HDMI_DPC_dpc_enable(data)                                                    (0x00000001&((data)<<0))
#define HDMI_DPC_dpc_enable_src(data)                                                ((0x00000001&(data))>>0)
#define HDMI_DPC_get_dpc_enable(data)                                                ((0x00000001&(data))>>0)


#define HDMI_DPC1                                                                    0x1800D158
#define HDMI_DPC1_reg_addr                                                           "0xB800D158"
#define HDMI_DPC1_reg                                                                0xB800D158
#define set_HDMI_DPC1_reg(data)   (*((volatile unsigned int*) HDMI_DPC1_reg)=data)
#define get_HDMI_DPC1_reg   (*((volatile unsigned int*) HDMI_DPC1_reg))
#define HDMI_DPC1_inst_adr                                                           "0x0056"
#define HDMI_DPC1_inst                                                               0x0056
#define HDMI_DPC1_write_en2_shift                                                    (9)
#define HDMI_DPC1_write_en2_mask                                                     (0x00000200)
#define HDMI_DPC1_write_en2(data)                                                    (0x00000200&((data)<<9))
#define HDMI_DPC1_write_en2_src(data)                                                ((0x00000200&(data))>>9)
#define HDMI_DPC1_get_write_en2(data)                                                ((0x00000200&(data))>>9)
#define HDMI_DPC1_dp_vfch_shift                                                      (8)
#define HDMI_DPC1_dp_vfch_mask                                                       (0x00000100)
#define HDMI_DPC1_dp_vfch(data)                                                      (0x00000100&((data)<<8))
#define HDMI_DPC1_dp_vfch_src(data)                                                  ((0x00000100&(data))>>8)
#define HDMI_DPC1_get_dp_vfch(data)                                                  ((0x00000100&(data))>>8)
#define HDMI_DPC1_write_en1_shift                                                    (4)
#define HDMI_DPC1_write_en1_mask                                                     (0x00000010)
#define HDMI_DPC1_write_en1(data)                                                    (0x00000010&((data)<<4))
#define HDMI_DPC1_write_en1_src(data)                                                ((0x00000010&(data))>>4)
#define HDMI_DPC1_get_write_en1(data)                                                ((0x00000010&(data))>>4)
#define HDMI_DPC1_pp_shift                                                           (0)
#define HDMI_DPC1_pp_mask                                                            (0x0000000F)
#define HDMI_DPC1_pp(data)                                                           (0x0000000F&((data)<<0))
#define HDMI_DPC1_pp_src(data)                                                       ((0x0000000F&(data))>>0)
#define HDMI_DPC1_get_pp(data)                                                       ((0x0000000F&(data))>>0)


#define HDMI_TMDS_EODR0                                                              0x1800D038
#define HDMI_TMDS_EODR0_reg_addr                                                     "0xB800D038"
#define HDMI_TMDS_EODR0_reg                                                          0xB800D038
#define set_HDMI_TMDS_EODR0_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_EODR0_reg)=data)
#define get_HDMI_TMDS_EODR0_reg   (*((volatile unsigned int*) HDMI_TMDS_EODR0_reg))
#define HDMI_TMDS_EODR0_inst_adr                                                     "0x000E"
#define HDMI_TMDS_EODR0_inst                                                         0x000E
#define HDMI_TMDS_EODR0_tmds_ch2_out_shift                                           (20)
#define HDMI_TMDS_EODR0_tmds_ch2_out_mask                                            (0x3FF00000)
#define HDMI_TMDS_EODR0_tmds_ch2_out(data)                                           (0x3FF00000&((data)<<20))
#define HDMI_TMDS_EODR0_tmds_ch2_out_src(data)                                       ((0x3FF00000&(data))>>20)
#define HDMI_TMDS_EODR0_get_tmds_ch2_out(data)                                       ((0x3FF00000&(data))>>20)
#define HDMI_TMDS_EODR0_tmds_ch1_out_shift                                           (10)
#define HDMI_TMDS_EODR0_tmds_ch1_out_mask                                            (0x000FFC00)
#define HDMI_TMDS_EODR0_tmds_ch1_out(data)                                           (0x000FFC00&((data)<<10))
#define HDMI_TMDS_EODR0_tmds_ch1_out_src(data)                                       ((0x000FFC00&(data))>>10)
#define HDMI_TMDS_EODR0_get_tmds_ch1_out(data)                                       ((0x000FFC00&(data))>>10)
#define HDMI_TMDS_EODR0_tmds_ch0_out_shift                                           (0)
#define HDMI_TMDS_EODR0_tmds_ch0_out_mask                                            (0x000003FF)
#define HDMI_TMDS_EODR0_tmds_ch0_out(data)                                           (0x000003FF&((data)<<0))
#define HDMI_TMDS_EODR0_tmds_ch0_out_src(data)                                       ((0x000003FF&(data))>>0)
#define HDMI_TMDS_EODR0_get_tmds_ch0_out(data)                                       ((0x000003FF&(data))>>0)


#define HDMI_TMDS_EODR1                                                              0x1800D03c
#define HDMI_TMDS_EODR1_reg_addr                                                     "0xB800D03C"
#define HDMI_TMDS_EODR1_reg                                                          0xB800D03C
#define set_HDMI_TMDS_EODR1_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_EODR1_reg)=data)
#define get_HDMI_TMDS_EODR1_reg   (*((volatile unsigned int*) HDMI_TMDS_EODR1_reg))
#define HDMI_TMDS_EODR1_inst_adr                                                     "0x000F"
#define HDMI_TMDS_EODR1_inst                                                         0x000F
#define HDMI_TMDS_EODR1_default_phase_st_shift                                       (12)
#define HDMI_TMDS_EODR1_default_phase_st_mask                                        (0x00001000)
#define HDMI_TMDS_EODR1_default_phase_st(data)                                       (0x00001000&((data)<<12))
#define HDMI_TMDS_EODR1_default_phase_st_src(data)                                   ((0x00001000&(data))>>12)
#define HDMI_TMDS_EODR1_get_default_phase_st(data)                                   ((0x00001000&(data))>>12)
#define HDMI_TMDS_EODR1_tmds_clk_out_shift                                           (0)
#define HDMI_TMDS_EODR1_tmds_clk_out_mask                                            (0x000003FF)
#define HDMI_TMDS_EODR1_tmds_clk_out(data)                                           (0x000003FF&((data)<<0))
#define HDMI_TMDS_EODR1_tmds_clk_out_src(data)                                       ((0x000003FF&(data))>>0)
#define HDMI_TMDS_EODR1_get_tmds_clk_out(data)                                       ((0x000003FF&(data))>>0)


#define HDMI_TMDS_SCR1                                                               0x1800D040
#define HDMI_TMDS_SCR1_reg_addr                                                      "0xB800D040"
#define HDMI_TMDS_SCR1_reg                                                           0xB800D040
#define set_HDMI_TMDS_SCR1_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_SCR1_reg)=data)
#define get_HDMI_TMDS_SCR1_reg   (*((volatile unsigned int*) HDMI_TMDS_SCR1_reg))
#define HDMI_TMDS_SCR1_inst_adr                                                      "0x0010"
#define HDMI_TMDS_SCR1_inst                                                          0x0010
#define HDMI_TMDS_SCR1_ip_shift                                                      (28)
#define HDMI_TMDS_SCR1_ip_mask                                                       (0xF0000000)
#define HDMI_TMDS_SCR1_ip(data)                                                      (0xF0000000&((data)<<28))
#define HDMI_TMDS_SCR1_ip_src(data)                                                  ((0xF0000000&(data))>>28)
#define HDMI_TMDS_SCR1_get_ip(data)                                                  ((0xF0000000&(data))>>28)
#define HDMI_TMDS_SCR1_rs_shift                                                      (25)
#define HDMI_TMDS_SCR1_rs_mask                                                       (0x0E000000)
#define HDMI_TMDS_SCR1_rs(data)                                                      (0x0E000000&((data)<<25))
#define HDMI_TMDS_SCR1_rs_src(data)                                                  ((0x0E000000&(data))>>25)
#define HDMI_TMDS_SCR1_get_rs(data)                                                  ((0x0E000000&(data))>>25)
#define HDMI_TMDS_SCR1_cs_shift                                                      (23)
#define HDMI_TMDS_SCR1_cs_mask                                                       (0x01800000)
#define HDMI_TMDS_SCR1_cs(data)                                                      (0x01800000&((data)<<23))
#define HDMI_TMDS_SCR1_cs_src(data)                                                  ((0x01800000&(data))>>23)
#define HDMI_TMDS_SCR1_get_cs(data)                                                  ((0x01800000&(data))>>23)
#define HDMI_TMDS_SCR1_cp_shift                                                      (22)
#define HDMI_TMDS_SCR1_cp_mask                                                       (0x00400000)
#define HDMI_TMDS_SCR1_cp(data)                                                      (0x00400000&((data)<<22))
#define HDMI_TMDS_SCR1_cp_src(data)                                                  ((0x00400000&(data))>>22)
#define HDMI_TMDS_SCR1_get_cp(data)                                                  ((0x00400000&(data))>>22)
#define HDMI_TMDS_SCR1_m_shift                                                       (20)
#define HDMI_TMDS_SCR1_m_mask                                                        (0x00300000)
#define HDMI_TMDS_SCR1_m(data)                                                       (0x00300000&((data)<<20))
#define HDMI_TMDS_SCR1_m_src(data)                                                   ((0x00300000&(data))>>20)
#define HDMI_TMDS_SCR1_get_m(data)                                                   ((0x00300000&(data))>>20)
#define HDMI_TMDS_SCR1_wdrst_shift                                                   (19)
#define HDMI_TMDS_SCR1_wdrst_mask                                                    (0x00080000)
#define HDMI_TMDS_SCR1_wdrst(data)                                                   (0x00080000&((data)<<19))
#define HDMI_TMDS_SCR1_wdrst_src(data)                                               ((0x00080000&(data))>>19)
#define HDMI_TMDS_SCR1_get_wdrst(data)                                               ((0x00080000&(data))>>19)
#define HDMI_TMDS_SCR1_wdset_shift                                                   (18)
#define HDMI_TMDS_SCR1_wdset_mask                                                    (0x00040000)
#define HDMI_TMDS_SCR1_wdset(data)                                                   (0x00040000&((data)<<18))
#define HDMI_TMDS_SCR1_wdset_src(data)                                               ((0x00040000&(data))>>18)
#define HDMI_TMDS_SCR1_get_wdset(data)                                               ((0x00040000&(data))>>18)
#define HDMI_TMDS_SCR1_rstb_shift                                                    (17)
#define HDMI_TMDS_SCR1_rstb_mask                                                     (0x00020000)
#define HDMI_TMDS_SCR1_rstb(data)                                                    (0x00020000&((data)<<17))
#define HDMI_TMDS_SCR1_rstb_src(data)                                                ((0x00020000&(data))>>17)
#define HDMI_TMDS_SCR1_get_rstb(data)                                                ((0x00020000&(data))>>17)
#define HDMI_TMDS_SCR1_powbias_shift                                                 (16)
#define HDMI_TMDS_SCR1_powbias_mask                                                  (0x00010000)
#define HDMI_TMDS_SCR1_powbias(data)                                                 (0x00010000&((data)<<16))
#define HDMI_TMDS_SCR1_powbias_src(data)                                             ((0x00010000&(data))>>16)
#define HDMI_TMDS_SCR1_get_powbias(data)                                             ((0x00010000&(data))>>16)
#define HDMI_TMDS_SCR1_powck_shift                                                   (15)
#define HDMI_TMDS_SCR1_powck_mask                                                    (0x00008000)
#define HDMI_TMDS_SCR1_powck(data)                                                   (0x00008000&((data)<<15))
#define HDMI_TMDS_SCR1_powck_src(data)                                               ((0x00008000&(data))>>15)
#define HDMI_TMDS_SCR1_get_powck(data)                                               ((0x00008000&(data))>>15)
#define HDMI_TMDS_SCR1_powa_shift                                                    (14)
#define HDMI_TMDS_SCR1_powa_mask                                                     (0x00004000)
#define HDMI_TMDS_SCR1_powa(data)                                                    (0x00004000&((data)<<14))
#define HDMI_TMDS_SCR1_powa_src(data)                                                ((0x00004000&(data))>>14)
#define HDMI_TMDS_SCR1_get_powa(data)                                                ((0x00004000&(data))>>14)
#define HDMI_TMDS_SCR1_powb_shift                                                    (13)
#define HDMI_TMDS_SCR1_powb_mask                                                     (0x00002000)
#define HDMI_TMDS_SCR1_powb(data)                                                    (0x00002000&((data)<<13))
#define HDMI_TMDS_SCR1_powb_src(data)                                                ((0x00002000&(data))>>13)
#define HDMI_TMDS_SCR1_get_powb(data)                                                ((0x00002000&(data))>>13)
#define HDMI_TMDS_SCR1_powc_shift                                                    (12)
#define HDMI_TMDS_SCR1_powc_mask                                                     (0x00001000)
#define HDMI_TMDS_SCR1_powc(data)                                                    (0x00001000&((data)<<12))
#define HDMI_TMDS_SCR1_powc_src(data)                                                ((0x00001000&(data))>>12)
#define HDMI_TMDS_SCR1_get_powc(data)                                                ((0x00001000&(data))>>12)
#define HDMI_TMDS_SCR1_emck_shift                                                    (11)
#define HDMI_TMDS_SCR1_emck_mask                                                     (0x00000800)
#define HDMI_TMDS_SCR1_emck(data)                                                    (0x00000800&((data)<<11))
#define HDMI_TMDS_SCR1_emck_src(data)                                                ((0x00000800&(data))>>11)
#define HDMI_TMDS_SCR1_get_emck(data)                                                ((0x00000800&(data))>>11)
#define HDMI_TMDS_SCR1_ema_shift                                                     (10)
#define HDMI_TMDS_SCR1_ema_mask                                                      (0x00000400)
#define HDMI_TMDS_SCR1_ema(data)                                                     (0x00000400&((data)<<10))
#define HDMI_TMDS_SCR1_ema_src(data)                                                 ((0x00000400&(data))>>10)
#define HDMI_TMDS_SCR1_get_ema(data)                                                 ((0x00000400&(data))>>10)
#define HDMI_TMDS_SCR1_emb_shift                                                     (9)
#define HDMI_TMDS_SCR1_emb_mask                                                      (0x00000200)
#define HDMI_TMDS_SCR1_emb(data)                                                     (0x00000200&((data)<<9))
#define HDMI_TMDS_SCR1_emb_src(data)                                                 ((0x00000200&(data))>>9)
#define HDMI_TMDS_SCR1_get_emb(data)                                                 ((0x00000200&(data))>>9)
#define HDMI_TMDS_SCR1_emc_shift                                                     (8)
#define HDMI_TMDS_SCR1_emc_mask                                                      (0x00000100)
#define HDMI_TMDS_SCR1_emc(data)                                                     (0x00000100&((data)<<8))
#define HDMI_TMDS_SCR1_emc_src(data)                                                 ((0x00000100&(data))>>8)
#define HDMI_TMDS_SCR1_get_emc(data)                                                 ((0x00000100&(data))>>8)
#define HDMI_TMDS_SCR1_powpll_shift                                                  (1)
#define HDMI_TMDS_SCR1_powpll_mask                                                   (0x00000002)
#define HDMI_TMDS_SCR1_powpll(data)                                                  (0x00000002&((data)<<1))
#define HDMI_TMDS_SCR1_powpll_src(data)                                              ((0x00000002&(data))>>1)
#define HDMI_TMDS_SCR1_get_powpll(data)                                              ((0x00000002&(data))>>1)
#define HDMI_TMDS_SCR1_cksel_shift                                                   (0)
#define HDMI_TMDS_SCR1_cksel_mask                                                    (0x00000001)
#define HDMI_TMDS_SCR1_cksel(data)                                                   (0x00000001&((data)<<0))
#define HDMI_TMDS_SCR1_cksel_src(data)                                               ((0x00000001&(data))>>0)
#define HDMI_TMDS_SCR1_get_cksel(data)                                               ((0x00000001&(data))>>0)


#define HDMI_TMDS_SCR2                                                               0x1800D044
#define HDMI_TMDS_SCR2_reg_addr                                                      "0xB800D044"
#define HDMI_TMDS_SCR2_reg                                                           0xB800D044
#define set_HDMI_TMDS_SCR2_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_SCR2_reg)=data)
#define get_HDMI_TMDS_SCR2_reg   (*((volatile unsigned int*) HDMI_TMDS_SCR2_reg))
#define HDMI_TMDS_SCR2_inst_adr                                                      "0x0011"
#define HDMI_TMDS_SCR2_inst                                                          0x0011
#define HDMI_TMDS_SCR2_sel_shift                                                     (30)
#define HDMI_TMDS_SCR2_sel_mask                                                      (0xC0000000)
#define HDMI_TMDS_SCR2_sel(data)                                                     (0xC0000000&((data)<<30))
#define HDMI_TMDS_SCR2_sel_src(data)                                                 ((0xC0000000&(data))>>30)
#define HDMI_TMDS_SCR2_get_sel(data)                                                 ((0xC0000000&(data))>>30)
#define HDMI_TMDS_SCR2_bias_shift                                                    (29)
#define HDMI_TMDS_SCR2_bias_mask                                                     (0x20000000)
#define HDMI_TMDS_SCR2_bias(data)                                                    (0x20000000&((data)<<29))
#define HDMI_TMDS_SCR2_bias_src(data)                                                ((0x20000000&(data))>>29)
#define HDMI_TMDS_SCR2_get_bias(data)                                                ((0x20000000&(data))>>29)
#define HDMI_TMDS_SCR2_vsel_shift                                                    (28)
#define HDMI_TMDS_SCR2_vsel_mask                                                     (0x10000000)
#define HDMI_TMDS_SCR2_vsel(data)                                                    (0x10000000&((data)<<28))
#define HDMI_TMDS_SCR2_vsel_src(data)                                                ((0x10000000&(data))>>28)
#define HDMI_TMDS_SCR2_get_vsel(data)                                                ((0x10000000&(data))>>28)
#define HDMI_TMDS_SCR2_powlv_shift                                                   (27)
#define HDMI_TMDS_SCR2_powlv_mask                                                    (0x08000000)
#define HDMI_TMDS_SCR2_powlv(data)                                                   (0x08000000&((data)<<27))
#define HDMI_TMDS_SCR2_powlv_src(data)                                               ((0x08000000&(data))>>27)
#define HDMI_TMDS_SCR2_get_powlv(data)                                               ((0x08000000&(data))>>27)
#define HDMI_TMDS_SCR2_lv_shift                                                      (24)
#define HDMI_TMDS_SCR2_lv_mask                                                       (0x07000000)
#define HDMI_TMDS_SCR2_lv(data)                                                      (0x07000000&((data)<<24))
#define HDMI_TMDS_SCR2_lv_src(data)                                                  ((0x07000000&(data))>>24)
#define HDMI_TMDS_SCR2_get_lv(data)                                                  ((0x07000000&(data))>>24)
#define HDMI_TMDS_SCR2_iemck_shift                                                   (20)
#define HDMI_TMDS_SCR2_iemck_mask                                                    (0x00F00000)
#define HDMI_TMDS_SCR2_iemck(data)                                                   (0x00F00000&((data)<<20))
#define HDMI_TMDS_SCR2_iemck_src(data)                                               ((0x00F00000&(data))>>20)
#define HDMI_TMDS_SCR2_get_iemck(data)                                               ((0x00F00000&(data))>>20)
#define HDMI_TMDS_SCR2_iema_shift                                                    (16)
#define HDMI_TMDS_SCR2_iema_mask                                                     (0x000F0000)
#define HDMI_TMDS_SCR2_iema(data)                                                    (0x000F0000&((data)<<16))
#define HDMI_TMDS_SCR2_iema_src(data)                                                ((0x000F0000&(data))>>16)
#define HDMI_TMDS_SCR2_get_iema(data)                                                ((0x000F0000&(data))>>16)
#define HDMI_TMDS_SCR2_iemb_shift                                                    (12)
#define HDMI_TMDS_SCR2_iemb_mask                                                     (0x0000F000)
#define HDMI_TMDS_SCR2_iemb(data)                                                    (0x0000F000&((data)<<12))
#define HDMI_TMDS_SCR2_iemb_src(data)                                                ((0x0000F000&(data))>>12)
#define HDMI_TMDS_SCR2_get_iemb(data)                                                ((0x0000F000&(data))>>12)
#define HDMI_TMDS_SCR2_iemc_shift                                                    (8)
#define HDMI_TMDS_SCR2_iemc_mask                                                     (0x00000F00)
#define HDMI_TMDS_SCR2_iemc(data)                                                    (0x00000F00&((data)<<8))
#define HDMI_TMDS_SCR2_iemc_src(data)                                                ((0x00000F00&(data))>>8)
#define HDMI_TMDS_SCR2_get_iemc(data)                                                ((0x00000F00&(data))>>8)
#define HDMI_TMDS_SCR2_rtonck_shift                                                  (6)
#define HDMI_TMDS_SCR2_rtonck_mask                                                   (0x000000C0)
#define HDMI_TMDS_SCR2_rtonck(data)                                                  (0x000000C0&((data)<<6))
#define HDMI_TMDS_SCR2_rtonck_src(data)                                              ((0x000000C0&(data))>>6)
#define HDMI_TMDS_SCR2_get_rtonck(data)                                              ((0x000000C0&(data))>>6)
#define HDMI_TMDS_SCR2_rtona_shift                                                   (4)
#define HDMI_TMDS_SCR2_rtona_mask                                                    (0x00000030)
#define HDMI_TMDS_SCR2_rtona(data)                                                   (0x00000030&((data)<<4))
#define HDMI_TMDS_SCR2_rtona_src(data)                                               ((0x00000030&(data))>>4)
#define HDMI_TMDS_SCR2_get_rtona(data)                                               ((0x00000030&(data))>>4)
#define HDMI_TMDS_SCR2_rtonb_shift                                                   (2)
#define HDMI_TMDS_SCR2_rtonb_mask                                                    (0x0000000C)
#define HDMI_TMDS_SCR2_rtonb(data)                                                   (0x0000000C&((data)<<2))
#define HDMI_TMDS_SCR2_rtonb_src(data)                                               ((0x0000000C&(data))>>2)
#define HDMI_TMDS_SCR2_get_rtonb(data)                                               ((0x0000000C&(data))>>2)
#define HDMI_TMDS_SCR2_rtonc_shift                                                   (0)
#define HDMI_TMDS_SCR2_rtonc_mask                                                    (0x00000003)
#define HDMI_TMDS_SCR2_rtonc(data)                                                   (0x00000003&((data)<<0))
#define HDMI_TMDS_SCR2_rtonc_src(data)                                               ((0x00000003&(data))>>0)
#define HDMI_TMDS_SCR2_get_rtonc(data)                                               ((0x00000003&(data))>>0)


#define HDMI_TMDS_SCR3                                                               0x1800D230
#define HDMI_TMDS_SCR3_reg_addr                                                      "0xB800D230"
#define HDMI_TMDS_SCR3_reg                                                           0xB800D230
#define set_HDMI_TMDS_SCR3_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_SCR3_reg)=data)
#define get_HDMI_TMDS_SCR3_reg   (*((volatile unsigned int*) HDMI_TMDS_SCR3_reg))
#define HDMI_TMDS_SCR3_inst_adr                                                      "0x008C"
#define HDMI_TMDS_SCR3_inst                                                          0x008C
#define HDMI_TMDS_SCR3_idrvck_shift                                                  (12)
#define HDMI_TMDS_SCR3_idrvck_mask                                                   (0x00007000)
#define HDMI_TMDS_SCR3_idrvck(data)                                                  (0x00007000&((data)<<12))
#define HDMI_TMDS_SCR3_idrvck_src(data)                                              ((0x00007000&(data))>>12)
#define HDMI_TMDS_SCR3_get_idrvck(data)                                              ((0x00007000&(data))>>12)
#define HDMI_TMDS_SCR3_idrva_shift                                                   (8)
#define HDMI_TMDS_SCR3_idrva_mask                                                    (0x00000700)
#define HDMI_TMDS_SCR3_idrva(data)                                                   (0x00000700&((data)<<8))
#define HDMI_TMDS_SCR3_idrva_src(data)                                               ((0x00000700&(data))>>8)
#define HDMI_TMDS_SCR3_get_idrva(data)                                               ((0x00000700&(data))>>8)
#define HDMI_TMDS_SCR3_idrvb_shift                                                   (4)
#define HDMI_TMDS_SCR3_idrvb_mask                                                    (0x00000070)
#define HDMI_TMDS_SCR3_idrvb(data)                                                   (0x00000070&((data)<<4))
#define HDMI_TMDS_SCR3_idrvb_src(data)                                               ((0x00000070&(data))>>4)
#define HDMI_TMDS_SCR3_get_idrvb(data)                                               ((0x00000070&(data))>>4)
#define HDMI_TMDS_SCR3_idrvc_shift                                                   (0)
#define HDMI_TMDS_SCR3_idrvc_mask                                                    (0x00000007)
#define HDMI_TMDS_SCR3_idrvc(data)                                                   (0x00000007&((data)<<0))
#define HDMI_TMDS_SCR3_idrvc_src(data)                                               ((0x00000007&(data))>>0)
#define HDMI_TMDS_SCR3_get_idrvc(data)                                               ((0x00000007&(data))>>0)


#define HDMI_TMDS_STR0                                                               0x1800D048
#define HDMI_TMDS_STR0_reg_addr                                                      "0xB800D048"
#define HDMI_TMDS_STR0_reg                                                           0xB800D048
#define set_HDMI_TMDS_STR0_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_STR0_reg)=data)
#define get_HDMI_TMDS_STR0_reg   (*((volatile unsigned int*) HDMI_TMDS_STR0_reg))
#define HDMI_TMDS_STR0_inst_adr                                                      "0x0012"
#define HDMI_TMDS_STR0_inst                                                          0x0012
#define HDMI_TMDS_STR0_tmds1ten_shift                                                (31)
#define HDMI_TMDS_STR0_tmds1ten_mask                                                 (0x80000000)
#define HDMI_TMDS_STR0_tmds1ten(data)                                                (0x80000000&((data)<<31))
#define HDMI_TMDS_STR0_tmds1ten_src(data)                                            ((0x80000000&(data))>>31)
#define HDMI_TMDS_STR0_get_tmds1ten(data)                                            ((0x80000000&(data))>>31)
#define HDMI_TMDS_STR0_td1src_shift                                                  (30)
#define HDMI_TMDS_STR0_td1src_mask                                                   (0x40000000)
#define HDMI_TMDS_STR0_td1src(data)                                                  (0x40000000&((data)<<30))
#define HDMI_TMDS_STR0_td1src_src(data)                                              ((0x40000000&(data))>>30)
#define HDMI_TMDS_STR0_get_td1src(data)                                              ((0x40000000&(data))>>30)
#define HDMI_TMDS_STR0_tmds1td_shift                                                 (16)
#define HDMI_TMDS_STR0_tmds1td_mask                                                  (0x03FF0000)
#define HDMI_TMDS_STR0_tmds1td(data)                                                 (0x03FF0000&((data)<<16))
#define HDMI_TMDS_STR0_tmds1td_src(data)                                             ((0x03FF0000&(data))>>16)
#define HDMI_TMDS_STR0_get_tmds1td(data)                                             ((0x03FF0000&(data))>>16)
#define HDMI_TMDS_STR0_tmds0ten_shift                                                (15)
#define HDMI_TMDS_STR0_tmds0ten_mask                                                 (0x00008000)
#define HDMI_TMDS_STR0_tmds0ten(data)                                                (0x00008000&((data)<<15))
#define HDMI_TMDS_STR0_tmds0ten_src(data)                                            ((0x00008000&(data))>>15)
#define HDMI_TMDS_STR0_get_tmds0ten(data)                                            ((0x00008000&(data))>>15)
#define HDMI_TMDS_STR0_td0src_shift                                                  (14)
#define HDMI_TMDS_STR0_td0src_mask                                                   (0x00004000)
#define HDMI_TMDS_STR0_td0src(data)                                                  (0x00004000&((data)<<14))
#define HDMI_TMDS_STR0_td0src_src(data)                                              ((0x00004000&(data))>>14)
#define HDMI_TMDS_STR0_get_td0src(data)                                              ((0x00004000&(data))>>14)
#define HDMI_TMDS_STR0_tmds0td_shift                                                 (0)
#define HDMI_TMDS_STR0_tmds0td_mask                                                  (0x000003FF)
#define HDMI_TMDS_STR0_tmds0td(data)                                                 (0x000003FF&((data)<<0))
#define HDMI_TMDS_STR0_tmds0td_src(data)                                             ((0x000003FF&(data))>>0)
#define HDMI_TMDS_STR0_get_tmds0td(data)                                             ((0x000003FF&(data))>>0)


#define HDMI_TMDS_STR1                                                               0x1800D04c
#define HDMI_TMDS_STR1_reg_addr                                                      "0xB800D04C"
#define HDMI_TMDS_STR1_reg                                                           0xB800D04C
#define set_HDMI_TMDS_STR1_reg(data)   (*((volatile unsigned int*) HDMI_TMDS_STR1_reg)=data)
#define get_HDMI_TMDS_STR1_reg   (*((volatile unsigned int*) HDMI_TMDS_STR1_reg))
#define HDMI_TMDS_STR1_inst_adr                                                      "0x0013"
#define HDMI_TMDS_STR1_inst                                                          0x0013
#define HDMI_TMDS_STR1_tmdscten_shift                                                (31)
#define HDMI_TMDS_STR1_tmdscten_mask                                                 (0x80000000)
#define HDMI_TMDS_STR1_tmdscten(data)                                                (0x80000000&((data)<<31))
#define HDMI_TMDS_STR1_tmdscten_src(data)                                            ((0x80000000&(data))>>31)
#define HDMI_TMDS_STR1_get_tmdscten(data)                                            ((0x80000000&(data))>>31)
#define HDMI_TMDS_STR1_tdcsrc_shift                                                  (30)
#define HDMI_TMDS_STR1_tdcsrc_mask                                                   (0x40000000)
#define HDMI_TMDS_STR1_tdcsrc(data)                                                  (0x40000000&((data)<<30))
#define HDMI_TMDS_STR1_tdcsrc_src(data)                                              ((0x40000000&(data))>>30)
#define HDMI_TMDS_STR1_get_tdcsrc(data)                                              ((0x40000000&(data))>>30)
#define HDMI_TMDS_STR1_tmdsctd_shift                                                 (16)
#define HDMI_TMDS_STR1_tmdsctd_mask                                                  (0x03FF0000)
#define HDMI_TMDS_STR1_tmdsctd(data)                                                 (0x03FF0000&((data)<<16))
#define HDMI_TMDS_STR1_tmdsctd_src(data)                                             ((0x03FF0000&(data))>>16)
#define HDMI_TMDS_STR1_get_tmdsctd(data)                                             ((0x03FF0000&(data))>>16)
#define HDMI_TMDS_STR1_tmds2ten_shift                                                (15)
#define HDMI_TMDS_STR1_tmds2ten_mask                                                 (0x00008000)
#define HDMI_TMDS_STR1_tmds2ten(data)                                                (0x00008000&((data)<<15))
#define HDMI_TMDS_STR1_tmds2ten_src(data)                                            ((0x00008000&(data))>>15)
#define HDMI_TMDS_STR1_get_tmds2ten(data)                                            ((0x00008000&(data))>>15)
#define HDMI_TMDS_STR1_td2src_shift                                                  (14)
#define HDMI_TMDS_STR1_td2src_mask                                                   (0x00004000)
#define HDMI_TMDS_STR1_td2src(data)                                                  (0x00004000&((data)<<14))
#define HDMI_TMDS_STR1_td2src_src(data)                                              ((0x00004000&(data))>>14)
#define HDMI_TMDS_STR1_get_td2src(data)                                              ((0x00004000&(data))>>14)
#define HDMI_TMDS_STR1_tmds2td_shift                                                 (0)
#define HDMI_TMDS_STR1_tmds2td_mask                                                  (0x000003FF)
#define HDMI_TMDS_STR1_tmds2td(data)                                                 (0x000003FF&((data)<<0))
#define HDMI_TMDS_STR1_tmds2td_src(data)                                             ((0x000003FF&(data))>>0)
#define HDMI_TMDS_STR1_get_tmds2td(data)                                             ((0x000003FF&(data))>>0)


#define HDMI_ASPCR1                                                                  0x1800D054
#define HDMI_ASPCR1_reg_addr                                                         "0xB800D054"
#define HDMI_ASPCR1_reg                                                              0xB800D054
#define set_HDMI_ASPCR1_reg(data)   (*((volatile unsigned int*) HDMI_ASPCR1_reg)=data)
#define get_HDMI_ASPCR1_reg   (*((volatile unsigned int*) HDMI_ASPCR1_reg))
#define HDMI_ASPCR1_inst_adr                                                         "0x0015"
#define HDMI_ASPCR1_inst                                                             0x0015
#define HDMI_ASPCR1_write_en8_shift                                                  (28)
#define HDMI_ASPCR1_write_en8_mask                                                   (0x10000000)
#define HDMI_ASPCR1_write_en8(data)                                                  (0x10000000&((data)<<28))
#define HDMI_ASPCR1_write_en8_src(data)                                              ((0x10000000&(data))>>28)
#define HDMI_ASPCR1_get_write_en8(data)                                              ((0x10000000&(data))>>28)
#define HDMI_ASPCR1_ycbcr422_8bits_shift                                             (27)
#define HDMI_ASPCR1_ycbcr422_8bits_mask                                              (0x08000000)
#define HDMI_ASPCR1_ycbcr422_8bits(data)                                             (0x08000000&((data)<<27))
#define HDMI_ASPCR1_ycbcr422_8bits_src(data)                                         ((0x08000000&(data))>>27)
#define HDMI_ASPCR1_get_ycbcr422_8bits(data)                                         ((0x08000000&(data))>>27)
#define HDMI_ASPCR1_write_en7_shift                                                  (26)
#define HDMI_ASPCR1_write_en7_mask                                                   (0x04000000)
#define HDMI_ASPCR1_write_en7(data)                                                  (0x04000000&((data)<<26))
#define HDMI_ASPCR1_write_en7_src(data)                                              ((0x04000000&(data))>>26)
#define HDMI_ASPCR1_get_write_en7(data)                                              ((0x04000000&(data))>>26)
#define HDMI_ASPCR1_cts_pkt_hp_shift                                                 (25)
#define HDMI_ASPCR1_cts_pkt_hp_mask                                                  (0x02000000)
#define HDMI_ASPCR1_cts_pkt_hp(data)                                                 (0x02000000&((data)<<25))
#define HDMI_ASPCR1_cts_pkt_hp_src(data)                                             ((0x02000000&(data))>>25)
#define HDMI_ASPCR1_get_cts_pkt_hp(data)                                             ((0x02000000&(data))>>25)
#define HDMI_ASPCR1_write_en6_shift                                                  (24)
#define HDMI_ASPCR1_write_en6_mask                                                   (0x01000000)
#define HDMI_ASPCR1_write_en6(data)                                                  (0x01000000&((data)<<24))
#define HDMI_ASPCR1_write_en6_src(data)                                              ((0x01000000&(data))>>24)
#define HDMI_ASPCR1_get_write_en6(data)                                              ((0x01000000&(data))>>24)
#define HDMI_ASPCR1_Audioheader0_shift                                               (16)
#define HDMI_ASPCR1_Audioheader0_mask                                                (0x00FF0000)
#define HDMI_ASPCR1_Audioheader0(data)                                               (0x00FF0000&((data)<<16))
#define HDMI_ASPCR1_Audioheader0_src(data)                                           ((0x00FF0000&(data))>>16)
#define HDMI_ASPCR1_get_Audioheader0(data)                                           ((0x00FF0000&(data))>>16)
#define HDMI_ASPCR1_write_en5_shift                                                  (15)
#define HDMI_ASPCR1_write_en5_mask                                                   (0x00008000)
#define HDMI_ASPCR1_write_en5(data)                                                  (0x00008000&((data)<<15))
#define HDMI_ASPCR1_write_en5_src(data)                                              ((0x00008000&(data))>>15)
#define HDMI_ASPCR1_get_write_en5(data)                                              ((0x00008000&(data))>>15)
#define HDMI_ASPCR1_samplepresent_shift                                              (11)
#define HDMI_ASPCR1_samplepresent_mask                                               (0x00007800)
#define HDMI_ASPCR1_samplepresent(data)                                              (0x00007800&((data)<<11))
#define HDMI_ASPCR1_samplepresent_src(data)                                          ((0x00007800&(data))>>11)
#define HDMI_ASPCR1_get_samplepresent(data)                                          ((0x00007800&(data))>>11)
#define HDMI_ASPCR1_write_en4_shift                                                  (10)
#define HDMI_ASPCR1_write_en4_mask                                                   (0x00000400)
#define HDMI_ASPCR1_write_en4(data)                                                  (0x00000400&((data)<<10))
#define HDMI_ASPCR1_write_en4_src(data)                                              ((0x00000400&(data))>>10)
#define HDMI_ASPCR1_get_write_en4(data)                                              ((0x00000400&(data))>>10)
#define HDMI_ASPCR1_forcesamplepresent_shift                                         (9)
#define HDMI_ASPCR1_forcesamplepresent_mask                                          (0x00000200)
#define HDMI_ASPCR1_forcesamplepresent(data)                                         (0x00000200&((data)<<9))
#define HDMI_ASPCR1_forcesamplepresent_src(data)                                     ((0x00000200&(data))>>9)
#define HDMI_ASPCR1_get_forcesamplepresent(data)                                     ((0x00000200&(data))>>9)
#define HDMI_ASPCR1_write_en3_shift                                                  (8)
#define HDMI_ASPCR1_write_en3_mask                                                   (0x00000100)
#define HDMI_ASPCR1_write_en3(data)                                                  (0x00000100&((data)<<8))
#define HDMI_ASPCR1_write_en3_src(data)                                              ((0x00000100&(data))>>8)
#define HDMI_ASPCR1_get_write_en3(data)                                              ((0x00000100&(data))>>8)
#define HDMI_ASPCR1_b_frame_shift                                                    (4)
#define HDMI_ASPCR1_b_frame_mask                                                     (0x000000F0)
#define HDMI_ASPCR1_b_frame(data)                                                    (0x000000F0&((data)<<4))
#define HDMI_ASPCR1_b_frame_src(data)                                                ((0x000000F0&(data))>>4)
#define HDMI_ASPCR1_get_b_frame(data)                                                ((0x000000F0&(data))>>4)
#define HDMI_ASPCR1_write_en2_shift                                                  (3)
#define HDMI_ASPCR1_write_en2_mask                                                   (0x00000008)
#define HDMI_ASPCR1_write_en2(data)                                                  (0x00000008&((data)<<3))
#define HDMI_ASPCR1_write_en2_src(data)                                              ((0x00000008&(data))>>3)
#define HDMI_ASPCR1_get_write_en2(data)                                              ((0x00000008&(data))>>3)
#define HDMI_ASPCR1_forceb_frame_shift                                               (2)
#define HDMI_ASPCR1_forceb_frame_mask                                                (0x00000004)
#define HDMI_ASPCR1_forceb_frame(data)                                               (0x00000004&((data)<<2))
#define HDMI_ASPCR1_forceb_frame_src(data)                                           ((0x00000004&(data))>>2)
#define HDMI_ASPCR1_get_forceb_frame(data)                                           ((0x00000004&(data))>>2)
#define HDMI_ASPCR1_write_en1_shift                                                  (1)
#define HDMI_ASPCR1_write_en1_mask                                                   (0x00000002)
#define HDMI_ASPCR1_write_en1(data)                                                  (0x00000002&((data)<<1))
#define HDMI_ASPCR1_write_en1_src(data)                                              ((0x00000002&(data))>>1)
#define HDMI_ASPCR1_get_write_en1(data)                                              ((0x00000002&(data))>>1)
#define HDMI_ASPCR1_audiolayout_shift                                                (0)
#define HDMI_ASPCR1_audiolayout_mask                                                 (0x00000001)
#define HDMI_ASPCR1_audiolayout(data)                                                (0x00000001&((data)<<0))
#define HDMI_ASPCR1_audiolayout_src(data)                                            ((0x00000001&(data))>>0)
#define HDMI_ASPCR1_get_audiolayout(data)                                            ((0x00000001&(data))>>0)


#define HDMI_ACACR                                                                   0x1800D058
#define HDMI_ACACR_reg_addr                                                          "0xB800D058"
#define HDMI_ACACR_reg                                                               0xB800D058
#define set_HDMI_ACACR_reg(data)   (*((volatile unsigned int*) HDMI_ACACR_reg)=data)
#define get_HDMI_ACACR_reg   (*((volatile unsigned int*) HDMI_ACACR_reg))
#define HDMI_ACACR_inst_adr                                                          "0x0016"
#define HDMI_ACACR_inst                                                              0x0016
#define HDMI_ACACR_write_en4_shift                                                   (15)
#define HDMI_ACACR_write_en4_mask                                                    (0x00008000)
#define HDMI_ACACR_write_en4(data)                                                   (0x00008000&((data)<<15))
#define HDMI_ACACR_write_en4_src(data)                                               ((0x00008000&(data))>>15)
#define HDMI_ACACR_get_write_en4(data)                                               ((0x00008000&(data))>>15)
#define HDMI_ACACR_hdmi_pk3map_shift                                                 (12)
#define HDMI_ACACR_hdmi_pk3map_mask                                                  (0x00007000)
#define HDMI_ACACR_hdmi_pk3map(data)                                                 (0x00007000&((data)<<12))
#define HDMI_ACACR_hdmi_pk3map_src(data)                                             ((0x00007000&(data))>>12)
#define HDMI_ACACR_get_hdmi_pk3map(data)                                             ((0x00007000&(data))>>12)
#define HDMI_ACACR_write_en3_shift                                                   (11)
#define HDMI_ACACR_write_en3_mask                                                    (0x00000800)
#define HDMI_ACACR_write_en3(data)                                                   (0x00000800&((data)<<11))
#define HDMI_ACACR_write_en3_src(data)                                               ((0x00000800&(data))>>11)
#define HDMI_ACACR_get_write_en3(data)                                               ((0x00000800&(data))>>11)
#define HDMI_ACACR_hdmi_pk2map_shift                                                 (8)
#define HDMI_ACACR_hdmi_pk2map_mask                                                  (0x00000700)
#define HDMI_ACACR_hdmi_pk2map(data)                                                 (0x00000700&((data)<<8))
#define HDMI_ACACR_hdmi_pk2map_src(data)                                             ((0x00000700&(data))>>8)
#define HDMI_ACACR_get_hdmi_pk2map(data)                                             ((0x00000700&(data))>>8)
#define HDMI_ACACR_write_en2_shift                                                   (7)
#define HDMI_ACACR_write_en2_mask                                                    (0x00000080)
#define HDMI_ACACR_write_en2(data)                                                   (0x00000080&((data)<<7))
#define HDMI_ACACR_write_en2_src(data)                                               ((0x00000080&(data))>>7)
#define HDMI_ACACR_get_write_en2(data)                                               ((0x00000080&(data))>>7)
#define HDMI_ACACR_hdmi_pk1map_shift                                                 (4)
#define HDMI_ACACR_hdmi_pk1map_mask                                                  (0x00000070)
#define HDMI_ACACR_hdmi_pk1map(data)                                                 (0x00000070&((data)<<4))
#define HDMI_ACACR_hdmi_pk1map_src(data)                                             ((0x00000070&(data))>>4)
#define HDMI_ACACR_get_hdmi_pk1map(data)                                             ((0x00000070&(data))>>4)
#define HDMI_ACACR_write_en1_shift                                                   (3)
#define HDMI_ACACR_write_en1_mask                                                    (0x00000008)
#define HDMI_ACACR_write_en1(data)                                                   (0x00000008&((data)<<3))
#define HDMI_ACACR_write_en1_src(data)                                               ((0x00000008&(data))>>3)
#define HDMI_ACACR_get_write_en1(data)                                               ((0x00000008&(data))>>3)
#define HDMI_ACACR_hdmi_pk0map_shift                                                 (0)
#define HDMI_ACACR_hdmi_pk0map_mask                                                  (0x00000007)
#define HDMI_ACACR_hdmi_pk0map(data)                                                 (0x00000007&((data)<<0))
#define HDMI_ACACR_hdmi_pk0map_src(data)                                             ((0x00000007&(data))>>0)
#define HDMI_ACACR_get_hdmi_pk0map(data)                                             ((0x00000007&(data))>>0)


#define HDMI_ACRPCR                                                                  0x1800D06c
#define HDMI_ACRPCR_reg_addr                                                         "0xB800D06C"
#define HDMI_ACRPCR_reg                                                              0xB800D06C
#define set_HDMI_ACRPCR_reg(data)   (*((volatile unsigned int*) HDMI_ACRPCR_reg)=data)
#define get_HDMI_ACRPCR_reg   (*((volatile unsigned int*) HDMI_ACRPCR_reg))
#define HDMI_ACRPCR_inst_adr                                                         "0x001B"
#define HDMI_ACRPCR_inst                                                             0x001B
#define HDMI_ACRPCR_write_en4_shift                                                  (26)
#define HDMI_ACRPCR_write_en4_mask                                                   (0x04000000)
#define HDMI_ACRPCR_write_en4(data)                                                  (0x04000000&((data)<<26))
#define HDMI_ACRPCR_write_en4_src(data)                                              ((0x04000000&(data))>>26)
#define HDMI_ACRPCR_get_write_en4(data)                                              ((0x04000000&(data))>>26)
#define HDMI_ACRPCR_fs_div4_shift                                                    (25)
#define HDMI_ACRPCR_fs_div4_mask                                                     (0x02000000)
#define HDMI_ACRPCR_fs_div4(data)                                                    (0x02000000&((data)<<25))
#define HDMI_ACRPCR_fs_div4_src(data)                                                ((0x02000000&(data))>>25)
#define HDMI_ACRPCR_get_fs_div4(data)                                                ((0x02000000&(data))>>25)
#define HDMI_ACRPCR_write_en3_shift                                                  (24)
#define HDMI_ACRPCR_write_en3_mask                                                   (0x01000000)
#define HDMI_ACRPCR_write_en3(data)                                                  (0x01000000&((data)<<24))
#define HDMI_ACRPCR_write_en3_src(data)                                              ((0x01000000&(data))>>24)
#define HDMI_ACRPCR_get_write_en3(data)                                              ((0x01000000&(data))>>24)
#define HDMI_ACRPCR_disablecrp_shift                                                 (23)
#define HDMI_ACRPCR_disablecrp_mask                                                  (0x00800000)
#define HDMI_ACRPCR_disablecrp(data)                                                 (0x00800000&((data)<<23))
#define HDMI_ACRPCR_disablecrp_src(data)                                             ((0x00800000&(data))>>23)
#define HDMI_ACRPCR_get_disablecrp(data)                                             ((0x00800000&(data))>>23)
#define HDMI_ACRPCR_write_en2_shift                                                  (22)
#define HDMI_ACRPCR_write_en2_mask                                                   (0x00400000)
#define HDMI_ACRPCR_write_en2(data)                                                  (0x00400000&((data)<<22))
#define HDMI_ACRPCR_write_en2_src(data)                                              ((0x00400000&(data))>>22)
#define HDMI_ACRPCR_get_write_en2(data)                                              ((0x00400000&(data))>>22)
#define HDMI_ACRPCR_cts_source_shift                                                 (21)
#define HDMI_ACRPCR_cts_source_mask                                                  (0x00200000)
#define HDMI_ACRPCR_cts_source(data)                                                 (0x00200000&((data)<<21))
#define HDMI_ACRPCR_cts_source_src(data)                                             ((0x00200000&(data))>>21)
#define HDMI_ACRPCR_get_cts_source(data)                                             ((0x00200000&(data))>>21)
#define HDMI_ACRPCR_write_en1_shift                                                  (20)
#define HDMI_ACRPCR_write_en1_mask                                                   (0x00100000)
#define HDMI_ACRPCR_write_en1(data)                                                  (0x00100000&((data)<<20))
#define HDMI_ACRPCR_write_en1_src(data)                                              ((0x00100000&(data))>>20)
#define HDMI_ACRPCR_get_write_en1(data)                                              ((0x00100000&(data))>>20)
#define HDMI_ACRPCR_n_value_shift                                                    (0)
#define HDMI_ACRPCR_n_value_mask                                                     (0x000FFFFF)
#define HDMI_ACRPCR_n_value(data)                                                    (0x000FFFFF&((data)<<0))
#define HDMI_ACRPCR_n_value_src(data)                                                ((0x000FFFFF&(data))>>0)
#define HDMI_ACRPCR_get_n_value(data)                                                ((0x000FFFFF&(data))>>0)


#define HDMI_ACRPCTSR                                                                0x1800D070
#define HDMI_ACRPCTSR_reg_addr                                                       "0xB800D070"
#define HDMI_ACRPCTSR_reg                                                            0xB800D070
#define set_HDMI_ACRPCTSR_reg(data)   (*((volatile unsigned int*) HDMI_ACRPCTSR_reg)=data)
#define get_HDMI_ACRPCTSR_reg   (*((volatile unsigned int*) HDMI_ACRPCTSR_reg))
#define HDMI_ACRPCTSR_inst_adr                                                       "0x001C"
#define HDMI_ACRPCTSR_inst                                                           0x001C
#define HDMI_ACRPCTSR_ctssw_shift                                                    (0)
#define HDMI_ACRPCTSR_ctssw_mask                                                     (0x000FFFFF)
#define HDMI_ACRPCTSR_ctssw(data)                                                    (0x000FFFFF&((data)<<0))
#define HDMI_ACRPCTSR_ctssw_src(data)                                                ((0x000FFFFF&(data))>>0)
#define HDMI_ACRPCTSR_get_ctssw(data)                                                ((0x000FFFFF&(data))>>0)


#define HDMI_ACRPPR                                                                  0x1800D074
#define HDMI_ACRPPR_reg_addr                                                         "0xB800D074"
#define HDMI_ACRPPR_reg                                                              0xB800D074
#define set_HDMI_ACRPPR_reg(data)   (*((volatile unsigned int*) HDMI_ACRPPR_reg)=data)
#define get_HDMI_ACRPPR_reg   (*((volatile unsigned int*) HDMI_ACRPPR_reg))
#define HDMI_ACRPPR_inst_adr                                                         "0x001D"
#define HDMI_ACRPPR_inst                                                             0x001D
#define HDMI_ACRPPR_ctshw_shift                                                      (0)
#define HDMI_ACRPPR_ctshw_mask                                                       (0x000FFFFF)
#define HDMI_ACRPPR_ctshw(data)                                                      (0x000FFFFF&((data)<<0))
#define HDMI_ACRPPR_ctshw_src(data)                                                  ((0x000FFFFF&(data))>>0)
#define HDMI_ACRPPR_get_ctshw(data)                                                  ((0x000FFFFF&(data))>>0)


#define HDMI_GCPCR                                                                   0x1800D078
#define HDMI_GCPCR_reg_addr                                                          "0xB800D078"
#define HDMI_GCPCR_reg                                                               0xB800D078
#define set_HDMI_GCPCR_reg(data)   (*((volatile unsigned int*) HDMI_GCPCR_reg)=data)
#define get_HDMI_GCPCR_reg   (*((volatile unsigned int*) HDMI_GCPCR_reg))
#define HDMI_GCPCR_inst_adr                                                          "0x001E"
#define HDMI_GCPCR_inst                                                              0x001E
#define HDMI_GCPCR_enablegcp_shift                                                   (3)
#define HDMI_GCPCR_enablegcp_mask                                                    (0x00000008)
#define HDMI_GCPCR_enablegcp(data)                                                   (0x00000008&((data)<<3))
#define HDMI_GCPCR_enablegcp_src(data)                                               ((0x00000008&(data))>>3)
#define HDMI_GCPCR_get_enablegcp(data)                                               ((0x00000008&(data))>>3)
#define HDMI_GCPCR_gcp_clearavmute_shift                                             (2)
#define HDMI_GCPCR_gcp_clearavmute_mask                                              (0x00000004)
#define HDMI_GCPCR_gcp_clearavmute(data)                                             (0x00000004&((data)<<2))
#define HDMI_GCPCR_gcp_clearavmute_src(data)                                         ((0x00000004&(data))>>2)
#define HDMI_GCPCR_get_gcp_clearavmute(data)                                         ((0x00000004&(data))>>2)
#define HDMI_GCPCR_gcp_setavmute_shift                                               (1)
#define HDMI_GCPCR_gcp_setavmute_mask                                                (0x00000002)
#define HDMI_GCPCR_gcp_setavmute(data)                                               (0x00000002&((data)<<1))
#define HDMI_GCPCR_gcp_setavmute_src(data)                                           ((0x00000002&(data))>>1)
#define HDMI_GCPCR_get_gcp_setavmute(data)                                           ((0x00000002&(data))>>1)
#define HDMI_GCPCR_write_data_shift                                                  (0)
#define HDMI_GCPCR_write_data_mask                                                   (0x00000001)
#define HDMI_GCPCR_write_data(data)                                                  (0x00000001&((data)<<0))
#define HDMI_GCPCR_write_data_src(data)                                              ((0x00000001&(data))>>0)
#define HDMI_GCPCR_get_write_data(data)                                              ((0x00000001&(data))>>0)


#define HDMI_ORP6PH                                                                  0x1800D07c
#define HDMI_ORP6PH_reg_addr                                                         "0xB800D07C"
#define HDMI_ORP6PH_reg                                                              0xB800D07C
#define set_HDMI_ORP6PH_reg(data)   (*((volatile unsigned int*) HDMI_ORP6PH_reg)=data)
#define get_HDMI_ORP6PH_reg   (*((volatile unsigned int*) HDMI_ORP6PH_reg))
#define HDMI_ORP6PH_inst_adr                                                         "0x001F"
#define HDMI_ORP6PH_inst                                                             0x001F
#define HDMI_ORP6PH_once_pkt_shift                                                   (0)
#define HDMI_ORP6PH_once_pkt_mask                                                    (0xFFFFFFFF)
#define HDMI_ORP6PH_once_pkt(data)                                                   (0xFFFFFFFF&((data)<<0))
#define HDMI_ORP6PH_once_pkt_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define HDMI_ORP6PH_get_once_pkt(data)                                               ((0xFFFFFFFF&(data))>>0)


#define HDMI_ORP6W_0                                                                 0x1800D080
#define HDMI_ORP6W_1                                                                 0x1800D084
#define HDMI_ORP6W_2                                                                 0x1800D088
#define HDMI_ORP6W_3                                                                 0x1800D08C
#define HDMI_ORP6W_4                                                                 0x1800D090
#define HDMI_ORP6W_5                                                                 0x1800D094
#define HDMI_ORP6W_6                                                                 0x1800D098
#define HDMI_ORP6W_7                                                                 0x1800D09C
#define HDMI_ORP6W_0_reg_addr                                                        "0xB800D080"
#define HDMI_ORP6W_1_reg_addr                                                        "0xB800D084"
#define HDMI_ORP6W_2_reg_addr                                                        "0xB800D088"
#define HDMI_ORP6W_3_reg_addr                                                        "0xB800D08C"
#define HDMI_ORP6W_4_reg_addr                                                        "0xB800D090"
#define HDMI_ORP6W_5_reg_addr                                                        "0xB800D094"
#define HDMI_ORP6W_6_reg_addr                                                        "0xB800D098"
#define HDMI_ORP6W_7_reg_addr                                                        "0xB800D09C"
#define HDMI_ORP6W_0_reg                                                             0xB800D080
#define HDMI_ORP6W_1_reg                                                             0xB800D084
#define HDMI_ORP6W_2_reg                                                             0xB800D088
#define HDMI_ORP6W_3_reg                                                             0xB800D08C
#define HDMI_ORP6W_4_reg                                                             0xB800D090
#define HDMI_ORP6W_5_reg                                                             0xB800D094
#define HDMI_ORP6W_6_reg                                                             0xB800D098
#define HDMI_ORP6W_7_reg                                                             0xB800D09C
#define set_HDMI_ORP6W_0_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_0_reg)=data)
#define set_HDMI_ORP6W_1_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_1_reg)=data)
#define set_HDMI_ORP6W_2_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_2_reg)=data)
#define set_HDMI_ORP6W_3_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_3_reg)=data)
#define set_HDMI_ORP6W_4_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_4_reg)=data)
#define set_HDMI_ORP6W_5_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_5_reg)=data)
#define set_HDMI_ORP6W_6_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_6_reg)=data)
#define set_HDMI_ORP6W_7_reg(data)   (*((volatile unsigned int*) HDMI_ORP6W_7_reg)=data)
#define get_HDMI_ORP6W_0_reg   (*((volatile unsigned int*) HDMI_ORP6W_0_reg))
#define get_HDMI_ORP6W_1_reg   (*((volatile unsigned int*) HDMI_ORP6W_1_reg))
#define get_HDMI_ORP6W_2_reg   (*((volatile unsigned int*) HDMI_ORP6W_2_reg))
#define get_HDMI_ORP6W_3_reg   (*((volatile unsigned int*) HDMI_ORP6W_3_reg))
#define get_HDMI_ORP6W_4_reg   (*((volatile unsigned int*) HDMI_ORP6W_4_reg))
#define get_HDMI_ORP6W_5_reg   (*((volatile unsigned int*) HDMI_ORP6W_5_reg))
#define get_HDMI_ORP6W_6_reg   (*((volatile unsigned int*) HDMI_ORP6W_6_reg))
#define get_HDMI_ORP6W_7_reg   (*((volatile unsigned int*) HDMI_ORP6W_7_reg))
#define HDMI_ORP6W_0_inst_adr                                                        "0x0020"
#define HDMI_ORP6W_1_inst_adr                                                        "0x0021"
#define HDMI_ORP6W_2_inst_adr                                                        "0x0022"
#define HDMI_ORP6W_3_inst_adr                                                        "0x0023"
#define HDMI_ORP6W_4_inst_adr                                                        "0x0024"
#define HDMI_ORP6W_5_inst_adr                                                        "0x0025"
#define HDMI_ORP6W_6_inst_adr                                                        "0x0026"
#define HDMI_ORP6W_7_inst_adr                                                        "0x0027"
#define HDMI_ORP6W_0_inst                                                            0x0020
#define HDMI_ORP6W_1_inst                                                            0x0021
#define HDMI_ORP6W_2_inst                                                            0x0022
#define HDMI_ORP6W_3_inst                                                            0x0023
#define HDMI_ORP6W_4_inst                                                            0x0024
#define HDMI_ORP6W_5_inst                                                            0x0025
#define HDMI_ORP6W_6_inst                                                            0x0026
#define HDMI_ORP6W_7_inst                                                            0x0027
#define HDMI_ORP6W_once_pkt_shift                                                    (0)
#define HDMI_ORP6W_once_pkt_mask                                                     (0xFFFFFFFF)
#define HDMI_ORP6W_once_pkt(data)                                                    (0xFFFFFFFF&((data)<<0))
#define HDMI_ORP6W_once_pkt_src(data)                                                ((0xFFFFFFFF&(data))>>0)
#define HDMI_ORP6W_get_once_pkt(data)                                                ((0xFFFFFFFF&(data))>>0)


#define HDMI_RPCR                                                                    0x1800D0a0
#define HDMI_RPCR_reg_addr                                                           "0xB800D0A0"
#define HDMI_RPCR_reg                                                                0xB800D0A0
#define set_HDMI_RPCR_reg(data)   (*((volatile unsigned int*) HDMI_RPCR_reg)=data)
#define get_HDMI_RPCR_reg   (*((volatile unsigned int*) HDMI_RPCR_reg))
#define HDMI_RPCR_inst_adr                                                           "0x0028"
#define HDMI_RPCR_inst                                                               0x0028
#define HDMI_RPCR_write_en6_shift                                                    (29)
#define HDMI_RPCR_write_en6_mask                                                     (0x20000000)
#define HDMI_RPCR_write_en6(data)                                                    (0x20000000&((data)<<29))
#define HDMI_RPCR_write_en6_src(data)                                                ((0x20000000&(data))>>29)
#define HDMI_RPCR_get_write_en6(data)                                                ((0x20000000&(data))>>29)
#define HDMI_RPCR_prp5period_shift                                                   (25)
#define HDMI_RPCR_prp5period_mask                                                    (0x1E000000)
#define HDMI_RPCR_prp5period(data)                                                   (0x1E000000&((data)<<25))
#define HDMI_RPCR_prp5period_src(data)                                               ((0x1E000000&(data))>>25)
#define HDMI_RPCR_get_prp5period(data)                                               ((0x1E000000&(data))>>25)
#define HDMI_RPCR_write_en5_shift                                                    (24)
#define HDMI_RPCR_write_en5_mask                                                     (0x01000000)
#define HDMI_RPCR_write_en5(data)                                                    (0x01000000&((data)<<24))
#define HDMI_RPCR_write_en5_src(data)                                                ((0x01000000&(data))>>24)
#define HDMI_RPCR_get_write_en5(data)                                                ((0x01000000&(data))>>24)
#define HDMI_RPCR_prp4period_shift                                                   (20)
#define HDMI_RPCR_prp4period_mask                                                    (0x00F00000)
#define HDMI_RPCR_prp4period(data)                                                   (0x00F00000&((data)<<20))
#define HDMI_RPCR_prp4period_src(data)                                               ((0x00F00000&(data))>>20)
#define HDMI_RPCR_get_prp4period(data)                                               ((0x00F00000&(data))>>20)
#define HDMI_RPCR_write_en4_shift                                                    (19)
#define HDMI_RPCR_write_en4_mask                                                     (0x00080000)
#define HDMI_RPCR_write_en4(data)                                                    (0x00080000&((data)<<19))
#define HDMI_RPCR_write_en4_src(data)                                                ((0x00080000&(data))>>19)
#define HDMI_RPCR_get_write_en4(data)                                                ((0x00080000&(data))>>19)
#define HDMI_RPCR_prp3period_shift                                                   (15)
#define HDMI_RPCR_prp3period_mask                                                    (0x00078000)
#define HDMI_RPCR_prp3period(data)                                                   (0x00078000&((data)<<15))
#define HDMI_RPCR_prp3period_src(data)                                               ((0x00078000&(data))>>15)
#define HDMI_RPCR_get_prp3period(data)                                               ((0x00078000&(data))>>15)
#define HDMI_RPCR_write_en3_shift                                                    (14)
#define HDMI_RPCR_write_en3_mask                                                     (0x00004000)
#define HDMI_RPCR_write_en3(data)                                                    (0x00004000&((data)<<14))
#define HDMI_RPCR_write_en3_src(data)                                                ((0x00004000&(data))>>14)
#define HDMI_RPCR_get_write_en3(data)                                                ((0x00004000&(data))>>14)
#define HDMI_RPCR_prp2period_shift                                                   (10)
#define HDMI_RPCR_prp2period_mask                                                    (0x00003C00)
#define HDMI_RPCR_prp2period(data)                                                   (0x00003C00&((data)<<10))
#define HDMI_RPCR_prp2period_src(data)                                               ((0x00003C00&(data))>>10)
#define HDMI_RPCR_get_prp2period(data)                                               ((0x00003C00&(data))>>10)
#define HDMI_RPCR_write_en2_shift                                                    (9)
#define HDMI_RPCR_write_en2_mask                                                     (0x00000200)
#define HDMI_RPCR_write_en2(data)                                                    (0x00000200&((data)<<9))
#define HDMI_RPCR_write_en2_src(data)                                                ((0x00000200&(data))>>9)
#define HDMI_RPCR_get_write_en2(data)                                                ((0x00000200&(data))>>9)
#define HDMI_RPCR_prp1period_shift                                                   (5)
#define HDMI_RPCR_prp1period_mask                                                    (0x000001E0)
#define HDMI_RPCR_prp1period(data)                                                   (0x000001E0&((data)<<5))
#define HDMI_RPCR_prp1period_src(data)                                               ((0x000001E0&(data))>>5)
#define HDMI_RPCR_get_prp1period(data)                                               ((0x000001E0&(data))>>5)
#define HDMI_RPCR_write_en1_shift                                                    (4)
#define HDMI_RPCR_write_en1_mask                                                     (0x00000010)
#define HDMI_RPCR_write_en1(data)                                                    (0x00000010&((data)<<4))
#define HDMI_RPCR_write_en1_src(data)                                                ((0x00000010&(data))>>4)
#define HDMI_RPCR_get_write_en1(data)                                                ((0x00000010&(data))>>4)
#define HDMI_RPCR_prp0period_shift                                                   (0)
#define HDMI_RPCR_prp0period_mask                                                    (0x0000000F)
#define HDMI_RPCR_prp0period(data)                                                   (0x0000000F&((data)<<0))
#define HDMI_RPCR_prp0period_src(data)                                               ((0x0000000F&(data))>>0)
#define HDMI_RPCR_get_prp0period(data)                                               ((0x0000000F&(data))>>0)


#define HDMI_RPEN                                                                    0x1800D0a4
#define HDMI_RPEN_reg_addr                                                           "0xB800D0A4"
#define HDMI_RPEN_reg                                                                0xB800D0A4
#define set_HDMI_RPEN_reg(data)   (*((volatile unsigned int*) HDMI_RPEN_reg)=data)
#define get_HDMI_RPEN_reg   (*((volatile unsigned int*) HDMI_RPEN_reg))
#define HDMI_RPEN_inst_adr                                                           "0x0029"
#define HDMI_RPEN_inst                                                               0x0029
#define HDMI_RPEN_enprpkt5_shift                                                     (6)
#define HDMI_RPEN_enprpkt5_mask                                                      (0x00000040)
#define HDMI_RPEN_enprpkt5(data)                                                     (0x00000040&((data)<<6))
#define HDMI_RPEN_enprpkt5_src(data)                                                 ((0x00000040&(data))>>6)
#define HDMI_RPEN_get_enprpkt5(data)                                                 ((0x00000040&(data))>>6)
#define HDMI_RPEN_enprpkt4_shift                                                     (5)
#define HDMI_RPEN_enprpkt4_mask                                                      (0x00000020)
#define HDMI_RPEN_enprpkt4(data)                                                     (0x00000020&((data)<<5))
#define HDMI_RPEN_enprpkt4_src(data)                                                 ((0x00000020&(data))>>5)
#define HDMI_RPEN_get_enprpkt4(data)                                                 ((0x00000020&(data))>>5)
#define HDMI_RPEN_enprpkt3_shift                                                     (4)
#define HDMI_RPEN_enprpkt3_mask                                                      (0x00000010)
#define HDMI_RPEN_enprpkt3(data)                                                     (0x00000010&((data)<<4))
#define HDMI_RPEN_enprpkt3_src(data)                                                 ((0x00000010&(data))>>4)
#define HDMI_RPEN_get_enprpkt3(data)                                                 ((0x00000010&(data))>>4)
#define HDMI_RPEN_enprpkt2_shift                                                     (3)
#define HDMI_RPEN_enprpkt2_mask                                                      (0x00000008)
#define HDMI_RPEN_enprpkt2(data)                                                     (0x00000008&((data)<<3))
#define HDMI_RPEN_enprpkt2_src(data)                                                 ((0x00000008&(data))>>3)
#define HDMI_RPEN_get_enprpkt2(data)                                                 ((0x00000008&(data))>>3)
#define HDMI_RPEN_enprpkt1_shift                                                     (2)
#define HDMI_RPEN_enprpkt1_mask                                                      (0x00000004)
#define HDMI_RPEN_enprpkt1(data)                                                     (0x00000004&((data)<<2))
#define HDMI_RPEN_enprpkt1_src(data)                                                 ((0x00000004&(data))>>2)
#define HDMI_RPEN_get_enprpkt1(data)                                                 ((0x00000004&(data))>>2)
#define HDMI_RPEN_enprpkt0_shift                                                     (1)
#define HDMI_RPEN_enprpkt0_mask                                                      (0x00000002)
#define HDMI_RPEN_enprpkt0(data)                                                     (0x00000002&((data)<<1))
#define HDMI_RPEN_enprpkt0_src(data)                                                 ((0x00000002&(data))>>1)
#define HDMI_RPEN_get_enprpkt0(data)                                                 ((0x00000002&(data))>>1)
#define HDMI_RPEN_write_data_shift                                                   (0)
#define HDMI_RPEN_write_data_mask                                                    (0x00000001)
#define HDMI_RPEN_write_data(data)                                                   (0x00000001&((data)<<0))
#define HDMI_RPEN_write_data_src(data)                                               ((0x00000001&(data))>>0)
#define HDMI_RPEN_get_write_data(data)                                               ((0x00000001&(data))>>0)


#define HDMI_RPRBDR                                                                  0x1800D0a8
#define HDMI_RPRBDR_reg_addr                                                         "0xB800D0A8"
#define HDMI_RPRBDR_reg                                                              0xB800D0A8
#define set_HDMI_RPRBDR_reg(data)   (*((volatile unsigned int*) HDMI_RPRBDR_reg)=data)
#define get_HDMI_RPRBDR_reg   (*((volatile unsigned int*) HDMI_RPRBDR_reg))
#define HDMI_RPRBDR_inst_adr                                                         "0x002A"
#define HDMI_RPRBDR_inst                                                             0x002A
#define HDMI_RPRBDR_isramrbd_shift                                                   (0)
#define HDMI_RPRBDR_isramrbd_mask                                                    (0x0003FFFF)
#define HDMI_RPRBDR_isramrbd(data)                                                   (0x0003FFFF&((data)<<0))
#define HDMI_RPRBDR_isramrbd_src(data)                                               ((0x0003FFFF&(data))>>0)
#define HDMI_RPRBDR_get_isramrbd(data)                                               ((0x0003FFFF&(data))>>0)


#define HDMI_OPCR                                                                    0x1800D0b0
#define HDMI_OPCR_reg_addr                                                           "0xB800D0B0"
#define HDMI_OPCR_reg                                                                0xB800D0B0
#define set_HDMI_OPCR_reg(data)   (*((volatile unsigned int*) HDMI_OPCR_reg)=data)
#define get_HDMI_OPCR_reg   (*((volatile unsigned int*) HDMI_OPCR_reg))
#define HDMI_OPCR_inst_adr                                                           "0x002C"
#define HDMI_OPCR_inst                                                               0x002C
#define HDMI_OPCR_write_en4_shift                                                    (14)
#define HDMI_OPCR_write_en4_mask                                                     (0x00004000)
#define HDMI_OPCR_write_en4(data)                                                    (0x00004000&((data)<<14))
#define HDMI_OPCR_write_en4_src(data)                                                ((0x00004000&(data))>>14)
#define HDMI_OPCR_get_write_en4(data)                                                ((0x00004000&(data))>>14)
#define HDMI_OPCR_enrbpktsram_shift                                                  (13)
#define HDMI_OPCR_enrbpktsram_mask                                                   (0x00002000)
#define HDMI_OPCR_enrbpktsram(data)                                                  (0x00002000&((data)<<13))
#define HDMI_OPCR_enrbpktsram_src(data)                                              ((0x00002000&(data))>>13)
#define HDMI_OPCR_get_enrbpktsram(data)                                              ((0x00002000&(data))>>13)
#define HDMI_OPCR_write_en3_shift                                                    (12)
#define HDMI_OPCR_write_en3_mask                                                     (0x00001000)
#define HDMI_OPCR_write_en3(data)                                                    (0x00001000&((data)<<12))
#define HDMI_OPCR_write_en3_src(data)                                                ((0x00001000&(data))>>12)
#define HDMI_OPCR_get_write_en3(data)                                                ((0x00001000&(data))>>12)
#define HDMI_OPCR_wrdes_shift                                                        (11)
#define HDMI_OPCR_wrdes_mask                                                         (0x00000800)
#define HDMI_OPCR_wrdes(data)                                                        (0x00000800&((data)<<11))
#define HDMI_OPCR_wrdes_src(data)                                                    ((0x00000800&(data))>>11)
#define HDMI_OPCR_get_wrdes(data)                                                    ((0x00000800&(data))>>11)
#define HDMI_OPCR_write_en2_shift                                                    (10)
#define HDMI_OPCR_write_en2_mask                                                     (0x00000400)
#define HDMI_OPCR_write_en2(data)                                                    (0x00000400&((data)<<10))
#define HDMI_OPCR_write_en2_src(data)                                                ((0x00000400&(data))>>10)
#define HDMI_OPCR_get_write_en2(data)                                                ((0x00000400&(data))>>10)
#define HDMI_OPCR_rprwcmd_shift                                                      (9)
#define HDMI_OPCR_rprwcmd_mask                                                       (0x00000200)
#define HDMI_OPCR_rprwcmd(data)                                                      (0x00000200&((data)<<9))
#define HDMI_OPCR_rprwcmd_src(data)                                                  ((0x00000200&(data))>>9)
#define HDMI_OPCR_get_rprwcmd(data)                                                  ((0x00000200&(data))>>9)
#define HDMI_OPCR_write_en1_shift                                                    (8)
#define HDMI_OPCR_write_en1_mask                                                     (0x00000100)
#define HDMI_OPCR_write_en1(data)                                                    (0x00000100&((data)<<8))
#define HDMI_OPCR_write_en1_src(data)                                                ((0x00000100&(data))>>8)
#define HDMI_OPCR_get_write_en1(data)                                                ((0x00000100&(data))>>8)
#define HDMI_OPCR_rpadd_shift                                                        (0)
#define HDMI_OPCR_rpadd_mask                                                         (0x000000FF)
#define HDMI_OPCR_rpadd(data)                                                        (0x000000FF&((data)<<0))
#define HDMI_OPCR_rpadd_src(data)                                                    ((0x000000FF&(data))>>0)
#define HDMI_OPCR_get_rpadd(data)                                                    ((0x000000FF&(data))>>0)


#define HDMI_DIPCCR                                                                  0x1800D0b4
#define HDMI_DIPCCR_reg_addr                                                         "0xB800D0B4"
#define HDMI_DIPCCR_reg                                                              0xB800D0B4
#define set_HDMI_DIPCCR_reg(data)   (*((volatile unsigned int*) HDMI_DIPCCR_reg)=data)
#define get_HDMI_DIPCCR_reg   (*((volatile unsigned int*) HDMI_DIPCCR_reg))
#define HDMI_DIPCCR_inst_adr                                                         "0x002D"
#define HDMI_DIPCCR_inst                                                             0x002D
#define HDMI_DIPCCR_write_en2_shift                                                  (11)
#define HDMI_DIPCCR_write_en2_mask                                                   (0x00000800)
#define HDMI_DIPCCR_write_en2(data)                                                  (0x00000800&((data)<<11))
#define HDMI_DIPCCR_write_en2_src(data)                                              ((0x00000800&(data))>>11)
#define HDMI_DIPCCR_get_write_en2(data)                                              ((0x00000800&(data))>>11)
#define HDMI_DIPCCR_vbidipcnt_shift                                                  (6)
#define HDMI_DIPCCR_vbidipcnt_mask                                                   (0x000007C0)
#define HDMI_DIPCCR_vbidipcnt(data)                                                  (0x000007C0&((data)<<6))
#define HDMI_DIPCCR_vbidipcnt_src(data)                                              ((0x000007C0&(data))>>6)
#define HDMI_DIPCCR_get_vbidipcnt(data)                                              ((0x000007C0&(data))>>6)
#define HDMI_DIPCCR_write_en1_shift                                                  (5)
#define HDMI_DIPCCR_write_en1_mask                                                   (0x00000020)
#define HDMI_DIPCCR_write_en1(data)                                                  (0x00000020&((data)<<5))
#define HDMI_DIPCCR_write_en1_src(data)                                              ((0x00000020&(data))>>5)
#define HDMI_DIPCCR_get_write_en1(data)                                              ((0x00000020&(data))>>5)
#define HDMI_DIPCCR_hbidipcnt_shift                                                  (0)
#define HDMI_DIPCCR_hbidipcnt_mask                                                   (0x0000001F)
#define HDMI_DIPCCR_hbidipcnt(data)                                                  (0x0000001F&((data)<<0))
#define HDMI_DIPCCR_hbidipcnt_src(data)                                              ((0x0000001F&(data))>>0)
#define HDMI_DIPCCR_get_hbidipcnt(data)                                              ((0x0000001F&(data))>>0)


#define HDMI_SCHCR                                                                   0x1800D0b8
#define HDMI_SCHCR_reg_addr                                                          "0xB800D0B8"
#define HDMI_SCHCR_reg                                                               0xB800D0B8
#define set_HDMI_SCHCR_reg(data)   (*((volatile unsigned int*) HDMI_SCHCR_reg)=data)
#define get_HDMI_SCHCR_reg   (*((volatile unsigned int*) HDMI_SCHCR_reg))
#define HDMI_SCHCR_inst_adr                                                          "0x002E"
#define HDMI_SCHCR_inst                                                              0x002E
#define HDMI_SCHCR_write_en11_shift                                                  (26)
#define HDMI_SCHCR_write_en11_mask                                                   (0x04000000)
#define HDMI_SCHCR_write_en11(data)                                                  (0x04000000&((data)<<26))
#define HDMI_SCHCR_write_en11_src(data)                                              ((0x04000000&(data))>>26)
#define HDMI_SCHCR_get_write_en11(data)                                              ((0x04000000&(data))>>26)
#define HDMI_SCHCR_ycbcr422_algo_shift                                               (25)
#define HDMI_SCHCR_ycbcr422_algo_mask                                                (0x02000000)
#define HDMI_SCHCR_ycbcr422_algo(data)                                               (0x02000000&((data)<<25))
#define HDMI_SCHCR_ycbcr422_algo_src(data)                                           ((0x02000000&(data))>>25)
#define HDMI_SCHCR_get_ycbcr422_algo(data)                                           ((0x02000000&(data))>>25)
#define HDMI_SCHCR_write_en10_shift                                                  (24)
#define HDMI_SCHCR_write_en10_mask                                                   (0x01000000)
#define HDMI_SCHCR_write_en10(data)                                                  (0x01000000&((data)<<24))
#define HDMI_SCHCR_write_en10_src(data)                                              ((0x01000000&(data))>>24)
#define HDMI_SCHCR_get_write_en10(data)                                              ((0x01000000&(data))>>24)
#define HDMI_SCHCR_422_pixel_repeat_shift                                            (23)
#define HDMI_SCHCR_422_pixel_repeat_mask                                             (0x00800000)
#define HDMI_SCHCR_422_pixel_repeat(data)                                            (0x00800000&((data)<<23))
#define HDMI_SCHCR_422_pixel_repeat_src(data)                                        ((0x00800000&(data))>>23)
#define HDMI_SCHCR_get_422_pixel_repeat(data)                                        ((0x00800000&(data))>>23)
#define HDMI_SCHCR_write_en9_shift                                                   (22)
#define HDMI_SCHCR_write_en9_mask                                                    (0x00400000)
#define HDMI_SCHCR_write_en9(data)                                                   (0x00400000&((data)<<22))
#define HDMI_SCHCR_write_en9_src(data)                                               ((0x00400000&(data))>>22)
#define HDMI_SCHCR_get_write_en9(data)                                               ((0x00400000&(data))>>22)
#define HDMI_SCHCR_vsyncpolin_shift                                                  (21)
#define HDMI_SCHCR_vsyncpolin_mask                                                   (0x00200000)
#define HDMI_SCHCR_vsyncpolin(data)                                                  (0x00200000&((data)<<21))
#define HDMI_SCHCR_vsyncpolin_src(data)                                              ((0x00200000&(data))>>21)
#define HDMI_SCHCR_get_vsyncpolin(data)                                              ((0x00200000&(data))>>21)
#define HDMI_SCHCR_write_en8_shift                                                   (20)
#define HDMI_SCHCR_write_en8_mask                                                    (0x00100000)
#define HDMI_SCHCR_write_en8(data)                                                   (0x00100000&((data)<<20))
#define HDMI_SCHCR_write_en8_src(data)                                               ((0x00100000&(data))>>20)
#define HDMI_SCHCR_get_write_en8(data)                                               ((0x00100000&(data))>>20)
#define HDMI_SCHCR_hsyncpolin_shift                                                  (19)
#define HDMI_SCHCR_hsyncpolin_mask                                                   (0x00080000)
#define HDMI_SCHCR_hsyncpolin(data)                                                  (0x00080000&((data)<<19))
#define HDMI_SCHCR_hsyncpolin_src(data)                                              ((0x00080000&(data))>>19)
#define HDMI_SCHCR_get_hsyncpolin(data)                                              ((0x00080000&(data))>>19)
#define HDMI_SCHCR_write_en7_shift                                                   (18)
#define HDMI_SCHCR_write_en7_mask                                                    (0x00040000)
#define HDMI_SCHCR_write_en7(data)                                                   (0x00040000&((data)<<18))
#define HDMI_SCHCR_write_en7_src(data)                                               ((0x00040000&(data))>>18)
#define HDMI_SCHCR_get_write_en7(data)                                               ((0x00040000&(data))>>18)
#define HDMI_SCHCR_vsyncpolinv_shift                                                 (17)
#define HDMI_SCHCR_vsyncpolinv_mask                                                  (0x00020000)
#define HDMI_SCHCR_vsyncpolinv(data)                                                 (0x00020000&((data)<<17))
#define HDMI_SCHCR_vsyncpolinv_src(data)                                             ((0x00020000&(data))>>17)
#define HDMI_SCHCR_get_vsyncpolinv(data)                                             ((0x00020000&(data))>>17)
#define HDMI_SCHCR_write_en6_shift                                                   (16)
#define HDMI_SCHCR_write_en6_mask                                                    (0x00010000)
#define HDMI_SCHCR_write_en6(data)                                                   (0x00010000&((data)<<16))
#define HDMI_SCHCR_write_en6_src(data)                                               ((0x00010000&(data))>>16)
#define HDMI_SCHCR_get_write_en6(data)                                               ((0x00010000&(data))>>16)
#define HDMI_SCHCR_hsyncpolinv_shift                                                 (15)
#define HDMI_SCHCR_hsyncpolinv_mask                                                  (0x00008000)
#define HDMI_SCHCR_hsyncpolinv(data)                                                 (0x00008000&((data)<<15))
#define HDMI_SCHCR_hsyncpolinv_src(data)                                             ((0x00008000&(data))>>15)
#define HDMI_SCHCR_get_hsyncpolinv(data)                                             ((0x00008000&(data))>>15)
#define HDMI_SCHCR_write_en5_shift                                                   (14)
#define HDMI_SCHCR_write_en5_mask                                                    (0x00004000)
#define HDMI_SCHCR_write_en5(data)                                                   (0x00004000&((data)<<14))
#define HDMI_SCHCR_write_en5_src(data)                                               ((0x00004000&(data))>>14)
#define HDMI_SCHCR_get_write_en5(data)                                               ((0x00004000&(data))>>14)
#define HDMI_SCHCR_pixelencycbcr422_shift                                            (13)
#define HDMI_SCHCR_pixelencycbcr422_mask                                             (0x00002000)
#define HDMI_SCHCR_pixelencycbcr422(data)                                            (0x00002000&((data)<<13))
#define HDMI_SCHCR_pixelencycbcr422_src(data)                                        ((0x00002000&(data))>>13)
#define HDMI_SCHCR_get_pixelencycbcr422(data)                                        ((0x00002000&(data))>>13)
#define HDMI_SCHCR_write_en4_shift                                                   (12)
#define HDMI_SCHCR_write_en4_mask                                                    (0x00001000)
#define HDMI_SCHCR_write_en4(data)                                                   (0x00001000&((data)<<12))
#define HDMI_SCHCR_write_en4_src(data)                                               ((0x00001000&(data))>>12)
#define HDMI_SCHCR_get_write_en4(data)                                               ((0x00001000&(data))>>12)
#define HDMI_SCHCR_hdmi_videoxs_shift                                                (7)
#define HDMI_SCHCR_hdmi_videoxs_mask                                                 (0x00000F80)
#define HDMI_SCHCR_hdmi_videoxs(data)                                                (0x00000F80&((data)<<7))
#define HDMI_SCHCR_hdmi_videoxs_src(data)                                            ((0x00000F80&(data))>>7)
#define HDMI_SCHCR_get_hdmi_videoxs(data)                                            ((0x00000F80&(data))>>7)
#define HDMI_SCHCR_write_en3_shift                                                   (6)
#define HDMI_SCHCR_write_en3_mask                                                    (0x00000040)
#define HDMI_SCHCR_write_en3(data)                                                   (0x00000040&((data)<<6))
#define HDMI_SCHCR_write_en3_src(data)                                               ((0x00000040&(data))>>6)
#define HDMI_SCHCR_get_write_en3(data)                                               ((0x00000040&(data))>>6)
#define HDMI_SCHCR_pixelencfmt_shift                                                 (4)
#define HDMI_SCHCR_pixelencfmt_mask                                                  (0x00000030)
#define HDMI_SCHCR_pixelencfmt(data)                                                 (0x00000030&((data)<<4))
#define HDMI_SCHCR_pixelencfmt_src(data)                                             ((0x00000030&(data))>>4)
#define HDMI_SCHCR_get_pixelencfmt(data)                                             ((0x00000030&(data))>>4)
#define HDMI_SCHCR_write_en2_shift                                                   (3)
#define HDMI_SCHCR_write_en2_mask                                                    (0x00000008)
#define HDMI_SCHCR_write_en2(data)                                                   (0x00000008&((data)<<3))
#define HDMI_SCHCR_write_en2_src(data)                                               ((0x00000008&(data))>>3)
#define HDMI_SCHCR_get_write_en2(data)                                               ((0x00000008&(data))>>3)
#define HDMI_SCHCR_pixelrepeat_shift                                                 (2)
#define HDMI_SCHCR_pixelrepeat_mask                                                  (0x00000004)
#define HDMI_SCHCR_pixelrepeat(data)                                                 (0x00000004&((data)<<2))
#define HDMI_SCHCR_pixelrepeat_src(data)                                             ((0x00000004&(data))>>2)
#define HDMI_SCHCR_get_pixelrepeat(data)                                             ((0x00000004&(data))>>2)
#define HDMI_SCHCR_write_en1_shift                                                   (1)
#define HDMI_SCHCR_write_en1_mask                                                    (0x00000002)
#define HDMI_SCHCR_write_en1(data)                                                   (0x00000002&((data)<<1))
#define HDMI_SCHCR_write_en1_src(data)                                               ((0x00000002&(data))>>1)
#define HDMI_SCHCR_get_write_en1(data)                                               ((0x00000002&(data))>>1)
#define HDMI_SCHCR_hdmi_modesel_shift                                                (0)
#define HDMI_SCHCR_hdmi_modesel_mask                                                 (0x00000001)
#define HDMI_SCHCR_hdmi_modesel(data)                                                (0x00000001&((data)<<0))
#define HDMI_SCHCR_hdmi_modesel_src(data)                                            ((0x00000001&(data))>>0)
#define HDMI_SCHCR_get_hdmi_modesel(data)                                            ((0x00000001&(data))>>0)


#define HDMI_ICR                                                                     0x1800D0bc
#define HDMI_ICR_reg_addr                                                            "0xB800D0BC"
#define HDMI_ICR_reg                                                                 0xB800D0BC
#define set_HDMI_ICR_reg(data)   (*((volatile unsigned int*) HDMI_ICR_reg)=data)
#define get_HDMI_ICR_reg   (*((volatile unsigned int*) HDMI_ICR_reg))
#define HDMI_ICR_inst_adr                                                            "0x002F"
#define HDMI_ICR_inst                                                                0x002F
#define HDMI_ICR_write_en3_shift                                                     (28)
#define HDMI_ICR_write_en3_mask                                                      (0x10000000)
#define HDMI_ICR_write_en3(data)                                                     (0x10000000&((data)<<28))
#define HDMI_ICR_write_en3_src(data)                                                 ((0x10000000&(data))>>28)
#define HDMI_ICR_get_write_en3(data)                                                 ((0x10000000&(data))>>28)
#define HDMI_ICR_enaudio_shift                                                       (27)
#define HDMI_ICR_enaudio_mask                                                        (0x08000000)
#define HDMI_ICR_enaudio(data)                                                       (0x08000000&((data)<<27))
#define HDMI_ICR_enaudio_src(data)                                                   ((0x08000000&(data))>>27)
#define HDMI_ICR_get_enaudio(data)                                                   ((0x08000000&(data))>>27)
#define HDMI_ICR_write_en2_shift                                                     (26)
#define HDMI_ICR_write_en2_mask                                                      (0x04000000)
#define HDMI_ICR_write_en2(data)                                                     (0x04000000&((data)<<26))
#define HDMI_ICR_write_en2_src(data)                                                 ((0x04000000&(data))>>26)
#define HDMI_ICR_get_write_en2(data)                                                 ((0x04000000&(data))>>26)
#define HDMI_ICR_envitd_shift                                                        (25)
#define HDMI_ICR_envitd_mask                                                         (0x02000000)
#define HDMI_ICR_envitd(data)                                                        (0x02000000&((data)<<25))
#define HDMI_ICR_envitd_src(data)                                                    ((0x02000000&(data))>>25)
#define HDMI_ICR_get_envitd(data)                                                    ((0x02000000&(data))>>25)
#define HDMI_ICR_write_en1_shift                                                     (24)
#define HDMI_ICR_write_en1_mask                                                      (0x01000000)
#define HDMI_ICR_write_en1(data)                                                     (0x01000000&((data)<<24))
#define HDMI_ICR_write_en1_src(data)                                                 ((0x01000000&(data))>>24)
#define HDMI_ICR_get_write_en1(data)                                                 ((0x01000000&(data))>>24)
#define HDMI_ICR_vitd_shift                                                          (0)
#define HDMI_ICR_vitd_mask                                                           (0x00FFFFFF)
#define HDMI_ICR_vitd(data)                                                          (0x00FFFFFF&((data)<<0))
#define HDMI_ICR_vitd_src(data)                                                      ((0x00FFFFFF&(data))>>0)
#define HDMI_ICR_get_vitd(data)                                                      ((0x00FFFFFF&(data))>>0)


#define HDMI_ICR1                                                                    0x1800D150
#define HDMI_ICR1_reg_addr                                                           "0xB800D150"
#define HDMI_ICR1_reg                                                                0xB800D150
#define set_HDMI_ICR1_reg(data)   (*((volatile unsigned int*) HDMI_ICR1_reg)=data)
#define get_HDMI_ICR1_reg   (*((volatile unsigned int*) HDMI_ICR1_reg))
#define HDMI_ICR1_inst_adr                                                           "0x0054"
#define HDMI_ICR1_inst                                                               0x0054
#define HDMI_ICR1_write_en1_shift                                                    (12)
#define HDMI_ICR1_write_en1_mask                                                     (0x00001000)
#define HDMI_ICR1_write_en1(data)                                                    (0x00001000&((data)<<12))
#define HDMI_ICR1_write_en1_src(data)                                                ((0x00001000&(data))>>12)
#define HDMI_ICR1_get_write_en1(data)                                                ((0x00001000&(data))>>12)
#define HDMI_ICR1_vitd_shift                                                         (0)
#define HDMI_ICR1_vitd_mask                                                          (0x00000FFF)
#define HDMI_ICR1_vitd(data)                                                         (0x00000FFF&((data)<<0))
#define HDMI_ICR1_vitd_src(data)                                                     ((0x00000FFF&(data))>>0)
#define HDMI_ICR1_get_vitd(data)                                                     ((0x00000FFF&(data))>>0)


#define HDMI_CRCCR                                                                   0x1800D0c0
#define HDMI_CRCCR_reg_addr                                                          "0xB800D0C0"
#define HDMI_CRCCR_reg                                                               0xB800D0C0
#define set_HDMI_CRCCR_reg(data)   (*((volatile unsigned int*) HDMI_CRCCR_reg)=data)
#define get_HDMI_CRCCR_reg   (*((volatile unsigned int*) HDMI_CRCCR_reg))
#define HDMI_CRCCR_inst_adr                                                          "0x0030"
#define HDMI_CRCCR_inst                                                              0x0030
#define HDMI_CRCCR_write_en2_shift                                                   (4)
#define HDMI_CRCCR_write_en2_mask                                                    (0x00000010)
#define HDMI_CRCCR_write_en2(data)                                                   (0x00000010&((data)<<4))
#define HDMI_CRCCR_write_en2_src(data)                                               ((0x00000010&(data))>>4)
#define HDMI_CRCCR_get_write_en2(data)                                               ((0x00000010&(data))>>4)
#define HDMI_CRCCR_crcmode_shift                                                     (3)
#define HDMI_CRCCR_crcmode_mask                                                      (0x00000008)
#define HDMI_CRCCR_crcmode(data)                                                     (0x00000008&((data)<<3))
#define HDMI_CRCCR_crcmode_src(data)                                                 ((0x00000008&(data))>>3)
#define HDMI_CRCCR_get_crcmode(data)                                                 ((0x00000008&(data))>>3)
#define HDMI_CRCCR_write_en1_shift                                                   (2)
#define HDMI_CRCCR_write_en1_mask                                                    (0x00000004)
#define HDMI_CRCCR_write_en1(data)                                                   (0x00000004&((data)<<2))
#define HDMI_CRCCR_write_en1_src(data)                                               ((0x00000004&(data))>>2)
#define HDMI_CRCCR_get_write_en1(data)                                               ((0x00000004&(data))>>2)
#define HDMI_CRCCR_crcchsel_shift                                                    (0)
#define HDMI_CRCCR_crcchsel_mask                                                     (0x00000003)
#define HDMI_CRCCR_crcchsel(data)                                                    (0x00000003&((data)<<0))
#define HDMI_CRCCR_crcchsel_src(data)                                                ((0x00000003&(data))>>0)
#define HDMI_CRCCR_get_crcchsel(data)                                                ((0x00000003&(data))>>0)


#define HDMI_CRCDOR                                                                  0x1800D0c4
#define HDMI_CRCDOR_reg_addr                                                         "0xB800D0C4"
#define HDMI_CRCDOR_reg                                                              0xB800D0C4
#define set_HDMI_CRCDOR_reg(data)   (*((volatile unsigned int*) HDMI_CRCDOR_reg)=data)
#define get_HDMI_CRCDOR_reg   (*((volatile unsigned int*) HDMI_CRCDOR_reg))
#define HDMI_CRCDOR_inst_adr                                                         "0x0031"
#define HDMI_CRCDOR_inst                                                             0x0031
#define HDMI_CRCDOR_crcvalue_shift                                                   (0)
#define HDMI_CRCDOR_crcvalue_mask                                                    (0xFFFFFFFF)
#define HDMI_CRCDOR_crcvalue(data)                                                   (0xFFFFFFFF&((data)<<0))
#define HDMI_CRCDOR_crcvalue_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define HDMI_CRCDOR_get_crcvalue(data)                                               ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_KOWR                                                               0x1800D0f0
#define HDMI_HDCP_KOWR_reg_addr                                                      "0xB800D0F0"
#define HDMI_HDCP_KOWR_reg                                                           0xB800D0F0
#define set_HDMI_HDCP_KOWR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_KOWR_reg)=data)
#define get_HDMI_HDCP_KOWR_reg   (*((volatile unsigned int*) HDMI_HDCP_KOWR_reg))
#define HDMI_HDCP_KOWR_inst_adr                                                      "0x003C"
#define HDMI_HDCP_KOWR_inst                                                          0x003C
#define HDMI_HDCP_KOWR_hdcprekeykeepoutwin_shift                                     (0)
#define HDMI_HDCP_KOWR_hdcprekeykeepoutwin_mask                                      (0x000000FF)
#define HDMI_HDCP_KOWR_hdcprekeykeepoutwin(data)                                     (0x000000FF&((data)<<0))
#define HDMI_HDCP_KOWR_hdcprekeykeepoutwin_src(data)                                 ((0x000000FF&(data))>>0)
#define HDMI_HDCP_KOWR_get_hdcprekeykeepoutwin(data)                                 ((0x000000FF&(data))>>0)


#define HDMI_HDCP_KOWRSE                                                             0x1800D0f4
#define HDMI_HDCP_KOWRSE_reg_addr                                                    "0xB800D0F4"
#define HDMI_HDCP_KOWRSE_reg                                                         0xB800D0F4
#define set_HDMI_HDCP_KOWRSE_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_KOWRSE_reg)=data)
#define get_HDMI_HDCP_KOWRSE_reg   (*((volatile unsigned int*) HDMI_HDCP_KOWRSE_reg))
#define HDMI_HDCP_KOWRSE_inst_adr                                                    "0x003D"
#define HDMI_HDCP_KOWRSE_inst                                                        0x003D
#define HDMI_HDCP_KOWRSE_write_en2_shift                                             (25)
#define HDMI_HDCP_KOWRSE_write_en2_mask                                              (0x02000000)
#define HDMI_HDCP_KOWRSE_write_en2(data)                                             (0x02000000&((data)<<25))
#define HDMI_HDCP_KOWRSE_write_en2_src(data)                                         ((0x02000000&(data))>>25)
#define HDMI_HDCP_KOWRSE_get_write_en2(data)                                         ((0x02000000&(data))>>25)
#define HDMI_HDCP_KOWRSE_hdcpverkeepoutwinend_shift                                  (13)
#define HDMI_HDCP_KOWRSE_hdcpverkeepoutwinend_mask                                   (0x01FFE000)
#define HDMI_HDCP_KOWRSE_hdcpverkeepoutwinend(data)                                  (0x01FFE000&((data)<<13))
#define HDMI_HDCP_KOWRSE_hdcpverkeepoutwinend_src(data)                              ((0x01FFE000&(data))>>13)
#define HDMI_HDCP_KOWRSE_get_hdcpverkeepoutwinend(data)                              ((0x01FFE000&(data))>>13)
#define HDMI_HDCP_KOWRSE_write_en1_shift                                             (12)
#define HDMI_HDCP_KOWRSE_write_en1_mask                                              (0x00001000)
#define HDMI_HDCP_KOWRSE_write_en1(data)                                             (0x00001000&((data)<<12))
#define HDMI_HDCP_KOWRSE_write_en1_src(data)                                         ((0x00001000&(data))>>12)
#define HDMI_HDCP_KOWRSE_get_write_en1(data)                                         ((0x00001000&(data))>>12)
#define HDMI_HDCP_KOWRSE_hdcpvertkeepoutwinstart_shift                               (0)
#define HDMI_HDCP_KOWRSE_hdcpvertkeepoutwinstart_mask                                (0x00000FFF)
#define HDMI_HDCP_KOWRSE_hdcpvertkeepoutwinstart(data)                               (0x00000FFF&((data)<<0))
#define HDMI_HDCP_KOWRSE_hdcpvertkeepoutwinstart_src(data)                           ((0x00000FFF&(data))>>0)
#define HDMI_HDCP_KOWRSE_get_hdcpvertkeepoutwinstart(data)                           ((0x00000FFF&(data))>>0)


#define HDMI_HDCP_OWR                                                                0x1800D0f8
#define HDMI_HDCP_OWR_reg_addr                                                       "0xB800D0F8"
#define HDMI_HDCP_OWR_reg                                                            0xB800D0F8
#define set_HDMI_HDCP_OWR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_OWR_reg)=data)
#define get_HDMI_HDCP_OWR_reg   (*((volatile unsigned int*) HDMI_HDCP_OWR_reg))
#define HDMI_HDCP_OWR_inst_adr                                                       "0x003E"
#define HDMI_HDCP_OWR_inst                                                           0x003E
#define HDMI_HDCP_OWR_write_en2_shift                                                (25)
#define HDMI_HDCP_OWR_write_en2_mask                                                 (0x02000000)
#define HDMI_HDCP_OWR_write_en2(data)                                                (0x02000000&((data)<<25))
#define HDMI_HDCP_OWR_write_en2_src(data)                                            ((0x02000000&(data))>>25)
#define HDMI_HDCP_OWR_get_write_en2(data)                                            ((0x02000000&(data))>>25)
#define HDMI_HDCP_OWR_hdcpoppwinend_shift                                            (13)
#define HDMI_HDCP_OWR_hdcpoppwinend_mask                                             (0x01FFE000)
#define HDMI_HDCP_OWR_hdcpoppwinend(data)                                            (0x01FFE000&((data)<<13))
#define HDMI_HDCP_OWR_hdcpoppwinend_src(data)                                        ((0x01FFE000&(data))>>13)
#define HDMI_HDCP_OWR_get_hdcpoppwinend(data)                                        ((0x01FFE000&(data))>>13)
#define HDMI_HDCP_OWR_write_en1_shift                                                (12)
#define HDMI_HDCP_OWR_write_en1_mask                                                 (0x00001000)
#define HDMI_HDCP_OWR_write_en1(data)                                                (0x00001000&((data)<<12))
#define HDMI_HDCP_OWR_write_en1_src(data)                                            ((0x00001000&(data))>>12)
#define HDMI_HDCP_OWR_get_write_en1(data)                                            ((0x00001000&(data))>>12)
#define HDMI_HDCP_OWR_hdcpoppwinstart_shift                                          (0)
#define HDMI_HDCP_OWR_hdcpoppwinstart_mask                                           (0x00000FFF)
#define HDMI_HDCP_OWR_hdcpoppwinstart(data)                                          (0x00000FFF&((data)<<0))
#define HDMI_HDCP_OWR_hdcpoppwinstart_src(data)                                      ((0x00000FFF&(data))>>0)
#define HDMI_HDCP_OWR_get_hdcpoppwinstart(data)                                      ((0x00000FFF&(data))>>0)


#define HDMI_HDCP_CR                                                                 0x1800D0fc
#define HDMI_HDCP_CR_reg_addr                                                        "0xB800D0FC"
#define HDMI_HDCP_CR_reg                                                             0xB800D0FC
#define set_HDMI_HDCP_CR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_CR_reg)=data)
#define get_HDMI_HDCP_CR_reg   (*((volatile unsigned int*) HDMI_HDCP_CR_reg))
#define HDMI_HDCP_CR_inst_adr                                                        "0x003F"
#define HDMI_HDCP_CR_inst                                                            0x003F
#define HDMI_HDCP_CR_write_en4_shift                                                 (7)
#define HDMI_HDCP_CR_write_en4_mask                                                  (0x00000080)
#define HDMI_HDCP_CR_write_en4(data)                                                 (0x00000080&((data)<<7))
#define HDMI_HDCP_CR_write_en4_src(data)                                             ((0x00000080&(data))>>7)
#define HDMI_HDCP_CR_get_write_en4(data)                                             ((0x00000080&(data))>>7)
#define HDMI_HDCP_CR_en1_1_feature_shift                                             (6)
#define HDMI_HDCP_CR_en1_1_feature_mask                                              (0x00000040)
#define HDMI_HDCP_CR_en1_1_feature(data)                                             (0x00000040&((data)<<6))
#define HDMI_HDCP_CR_en1_1_feature_src(data)                                         ((0x00000040&(data))>>6)
#define HDMI_HDCP_CR_get_en1_1_feature(data)                                         ((0x00000040&(data))>>6)
#define HDMI_HDCP_CR_write_en3_shift                                                 (5)
#define HDMI_HDCP_CR_write_en3_mask                                                  (0x00000020)
#define HDMI_HDCP_CR_write_en3(data)                                                 (0x00000020&((data)<<5))
#define HDMI_HDCP_CR_write_en3_src(data)                                             ((0x00000020&(data))>>5)
#define HDMI_HDCP_CR_get_write_en3(data)                                             ((0x00000020&(data))>>5)
#define HDMI_HDCP_CR_downstrisrepeater_shift                                         (4)
#define HDMI_HDCP_CR_downstrisrepeater_mask                                          (0x00000010)
#define HDMI_HDCP_CR_downstrisrepeater(data)                                         (0x00000010&((data)<<4))
#define HDMI_HDCP_CR_downstrisrepeater_src(data)                                     ((0x00000010&(data))>>4)
#define HDMI_HDCP_CR_get_downstrisrepeater(data)                                     ((0x00000010&(data))>>4)
#define HDMI_HDCP_CR_write_en2_shift                                                 (3)
#define HDMI_HDCP_CR_write_en2_mask                                                  (0x00000008)
#define HDMI_HDCP_CR_write_en2(data)                                                 (0x00000008&((data)<<3))
#define HDMI_HDCP_CR_write_en2_src(data)                                             ((0x00000008&(data))>>3)
#define HDMI_HDCP_CR_get_write_en2(data)                                             ((0x00000008&(data))>>3)
#define HDMI_HDCP_CR_aninfluencemode_shift                                           (2)
#define HDMI_HDCP_CR_aninfluencemode_mask                                            (0x00000004)
#define HDMI_HDCP_CR_aninfluencemode(data)                                           (0x00000004&((data)<<2))
#define HDMI_HDCP_CR_aninfluencemode_src(data)                                       ((0x00000004&(data))>>2)
#define HDMI_HDCP_CR_get_aninfluencemode(data)                                       ((0x00000004&(data))>>2)
#define HDMI_HDCP_CR_write_en1_shift                                                 (1)
#define HDMI_HDCP_CR_write_en1_mask                                                  (0x00000002)
#define HDMI_HDCP_CR_write_en1(data)                                                 (0x00000002&((data)<<1))
#define HDMI_HDCP_CR_write_en1_src(data)                                             ((0x00000002&(data))>>1)
#define HDMI_HDCP_CR_get_write_en1(data)                                             ((0x00000002&(data))>>1)
#define HDMI_HDCP_CR_hdcp_encryptenable_shift                                        (0)
#define HDMI_HDCP_CR_hdcp_encryptenable_mask                                         (0x00000001)
#define HDMI_HDCP_CR_hdcp_encryptenable(data)                                        (0x00000001&((data)<<0))
#define HDMI_HDCP_CR_hdcp_encryptenable_src(data)                                    ((0x00000001&(data))>>0)
#define HDMI_HDCP_CR_get_hdcp_encryptenable(data)                                    ((0x00000001&(data))>>0)


#define HDMI_HDCP_AUTH                                                               0x1800D100
#define HDMI_HDCP_AUTH_reg_addr                                                      "0xB800D100"
#define HDMI_HDCP_AUTH_reg                                                           0xB800D100
#define set_HDMI_HDCP_AUTH_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_AUTH_reg)=data)
#define get_HDMI_HDCP_AUTH_reg   (*((volatile unsigned int*) HDMI_HDCP_AUTH_reg))
#define HDMI_HDCP_AUTH_inst_adr                                                      "0x0040"
#define HDMI_HDCP_AUTH_inst                                                          0x0040
#define HDMI_HDCP_AUTH_write_en9_shift                                               (17)
#define HDMI_HDCP_AUTH_write_en9_mask                                                (0x00020000)
#define HDMI_HDCP_AUTH_write_en9(data)                                               (0x00020000&((data)<<17))
#define HDMI_HDCP_AUTH_write_en9_src(data)                                           ((0x00020000&(data))>>17)
#define HDMI_HDCP_AUTH_get_write_en9(data)                                           ((0x00020000&(data))>>17)
#define HDMI_HDCP_AUTH_ddpken_shift                                                  (16)
#define HDMI_HDCP_AUTH_ddpken_mask                                                   (0x00010000)
#define HDMI_HDCP_AUTH_ddpken(data)                                                  (0x00010000&((data)<<16))
#define HDMI_HDCP_AUTH_ddpken_src(data)                                              ((0x00010000&(data))>>16)
#define HDMI_HDCP_AUTH_get_ddpken(data)                                              ((0x00010000&(data))>>16)
#define HDMI_HDCP_AUTH_write_en8_shift                                               (15)
#define HDMI_HDCP_AUTH_write_en8_mask                                                (0x00008000)
#define HDMI_HDCP_AUTH_write_en8(data)                                               (0x00008000&((data)<<15))
#define HDMI_HDCP_AUTH_write_en8_src(data)                                           ((0x00008000&(data))>>15)
#define HDMI_HDCP_AUTH_get_write_en8(data)                                           ((0x00008000&(data))>>15)
#define HDMI_HDCP_AUTH_resetkmacc_shift                                              (14)
#define HDMI_HDCP_AUTH_resetkmacc_mask                                               (0x00004000)
#define HDMI_HDCP_AUTH_resetkmacc(data)                                              (0x00004000&((data)<<14))
#define HDMI_HDCP_AUTH_resetkmacc_src(data)                                          ((0x00004000&(data))>>14)
#define HDMI_HDCP_AUTH_get_resetkmacc(data)                                          ((0x00004000&(data))>>14)
#define HDMI_HDCP_AUTH_write_en7_shift                                               (13)
#define HDMI_HDCP_AUTH_write_en7_mask                                                (0x00002000)
#define HDMI_HDCP_AUTH_write_en7(data)                                               (0x00002000&((data)<<13))
#define HDMI_HDCP_AUTH_write_en7_src(data)                                           ((0x00002000&(data))>>13)
#define HDMI_HDCP_AUTH_get_write_en7(data)                                           ((0x00002000&(data))>>13)
#define HDMI_HDCP_AUTH_update_an_shift                                               (12)
#define HDMI_HDCP_AUTH_update_an_mask                                                (0x00001000)
#define HDMI_HDCP_AUTH_update_an(data)                                               (0x00001000&((data)<<12))
#define HDMI_HDCP_AUTH_update_an_src(data)                                           ((0x00001000&(data))>>12)
#define HDMI_HDCP_AUTH_get_update_an(data)                                           ((0x00001000&(data))>>12)
#define HDMI_HDCP_AUTH_write_en6_shift                                               (11)
#define HDMI_HDCP_AUTH_write_en6_mask                                                (0x00000800)
#define HDMI_HDCP_AUTH_write_en6(data)                                               (0x00000800&((data)<<11))
#define HDMI_HDCP_AUTH_write_en6_src(data)                                           ((0x00000800&(data))>>11)
#define HDMI_HDCP_AUTH_get_write_en6(data)                                           ((0x00000800&(data))>>11)
#define HDMI_HDCP_AUTH_aninfreq_shift                                                (10)
#define HDMI_HDCP_AUTH_aninfreq_mask                                                 (0x00000400)
#define HDMI_HDCP_AUTH_aninfreq(data)                                                (0x00000400&((data)<<10))
#define HDMI_HDCP_AUTH_aninfreq_src(data)                                            ((0x00000400&(data))>>10)
#define HDMI_HDCP_AUTH_get_aninfreq(data)                                            ((0x00000400&(data))>>10)
#define HDMI_HDCP_AUTH_write_en5_shift                                               (9)
#define HDMI_HDCP_AUTH_write_en5_mask                                                (0x00000200)
#define HDMI_HDCP_AUTH_write_en5(data)                                               (0x00000200&((data)<<9))
#define HDMI_HDCP_AUTH_write_en5_src(data)                                           ((0x00000200&(data))>>9)
#define HDMI_HDCP_AUTH_get_write_en5(data)                                           ((0x00000200&(data))>>9)
#define HDMI_HDCP_AUTH_seedload_shift                                                (8)
#define HDMI_HDCP_AUTH_seedload_mask                                                 (0x00000100)
#define HDMI_HDCP_AUTH_seedload(data)                                                (0x00000100&((data)<<8))
#define HDMI_HDCP_AUTH_seedload_src(data)                                            ((0x00000100&(data))>>8)
#define HDMI_HDCP_AUTH_get_seedload(data)                                            ((0x00000100&(data))>>8)
#define HDMI_HDCP_AUTH_write_en4_shift                                               (7)
#define HDMI_HDCP_AUTH_write_en4_mask                                                (0x00000080)
#define HDMI_HDCP_AUTH_write_en4(data)                                               (0x00000080&((data)<<7))
#define HDMI_HDCP_AUTH_write_en4_src(data)                                           ((0x00000080&(data))>>7)
#define HDMI_HDCP_AUTH_get_write_en4(data)                                           ((0x00000080&(data))>>7)
#define HDMI_HDCP_AUTH_deviceauthenticated_shift                                     (6)
#define HDMI_HDCP_AUTH_deviceauthenticated_mask                                      (0x00000040)
#define HDMI_HDCP_AUTH_deviceauthenticated(data)                                     (0x00000040&((data)<<6))
#define HDMI_HDCP_AUTH_deviceauthenticated_src(data)                                 ((0x00000040&(data))>>6)
#define HDMI_HDCP_AUTH_get_deviceauthenticated(data)                                 ((0x00000040&(data))>>6)
#define HDMI_HDCP_AUTH_write_en3_shift                                               (5)
#define HDMI_HDCP_AUTH_write_en3_mask                                                (0x00000020)
#define HDMI_HDCP_AUTH_write_en3(data)                                               (0x00000020&((data)<<5))
#define HDMI_HDCP_AUTH_write_en3_src(data)                                           ((0x00000020&(data))>>5)
#define HDMI_HDCP_AUTH_get_write_en3(data)                                           ((0x00000020&(data))>>5)
#define HDMI_HDCP_AUTH_forcetounauthenticated_shift                                  (4)
#define HDMI_HDCP_AUTH_forcetounauthenticated_mask                                   (0x00000010)
#define HDMI_HDCP_AUTH_forcetounauthenticated(data)                                  (0x00000010&((data)<<4))
#define HDMI_HDCP_AUTH_forcetounauthenticated_src(data)                              ((0x00000010&(data))>>4)
#define HDMI_HDCP_AUTH_get_forcetounauthenticated(data)                              ((0x00000010&(data))>>4)
#define HDMI_HDCP_AUTH_write_en2_shift                                               (3)
#define HDMI_HDCP_AUTH_write_en2_mask                                                (0x00000008)
#define HDMI_HDCP_AUTH_write_en2(data)                                               (0x00000008&((data)<<3))
#define HDMI_HDCP_AUTH_write_en2_src(data)                                           ((0x00000008&(data))>>3)
#define HDMI_HDCP_AUTH_get_write_en2(data)                                           ((0x00000008&(data))>>3)
#define HDMI_HDCP_AUTH_authcompute_shift                                             (2)
#define HDMI_HDCP_AUTH_authcompute_mask                                              (0x00000004)
#define HDMI_HDCP_AUTH_authcompute(data)                                             (0x00000004&((data)<<2))
#define HDMI_HDCP_AUTH_authcompute_src(data)                                         ((0x00000004&(data))>>2)
#define HDMI_HDCP_AUTH_get_authcompute(data)                                         ((0x00000004&(data))>>2)
#define HDMI_HDCP_AUTH_write_en1_shift                                               (1)
#define HDMI_HDCP_AUTH_write_en1_mask                                                (0x00000002)
#define HDMI_HDCP_AUTH_write_en1(data)                                               (0x00000002&((data)<<1))
#define HDMI_HDCP_AUTH_write_en1_src(data)                                           ((0x00000002&(data))>>1)
#define HDMI_HDCP_AUTH_get_write_en1(data)                                           ((0x00000002&(data))>>1)
#define HDMI_HDCP_AUTH_authrequest_shift                                             (0)
#define HDMI_HDCP_AUTH_authrequest_mask                                              (0x00000001)
#define HDMI_HDCP_AUTH_authrequest(data)                                             (0x00000001&((data)<<0))
#define HDMI_HDCP_AUTH_authrequest_src(data)                                         ((0x00000001&(data))>>0)
#define HDMI_HDCP_AUTH_get_authrequest(data)                                         ((0x00000001&(data))>>0)


#define HDMI_HDCP_SR                                                                 0x1800D104
#define HDMI_HDCP_SR_reg_addr                                                        "0xB800D104"
#define HDMI_HDCP_SR_reg                                                             0xB800D104
#define set_HDMI_HDCP_SR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_SR_reg)=data)
#define get_HDMI_HDCP_SR_reg   (*((volatile unsigned int*) HDMI_HDCP_SR_reg))
#define HDMI_HDCP_SR_inst_adr                                                        "0x0041"
#define HDMI_HDCP_SR_inst                                                            0x0041
#define HDMI_HDCP_SR_hdcpcipherstate_shift                                           (24)
#define HDMI_HDCP_SR_hdcpcipherstate_mask                                            (0xFF000000)
#define HDMI_HDCP_SR_hdcpcipherstate(data)                                           (0xFF000000&((data)<<24))
#define HDMI_HDCP_SR_hdcpcipherstate_src(data)                                       ((0xFF000000&(data))>>24)
#define HDMI_HDCP_SR_get_hdcpcipherstate(data)                                       ((0xFF000000&(data))>>24)
#define HDMI_HDCP_SR_curdpkaccdone_shift                                             (3)
#define HDMI_HDCP_SR_curdpkaccdone_mask                                              (0x00000008)
#define HDMI_HDCP_SR_curdpkaccdone(data)                                             (0x00000008&((data)<<3))
#define HDMI_HDCP_SR_curdpkaccdone_src(data)                                         ((0x00000008&(data))>>3)
#define HDMI_HDCP_SR_get_curdpkaccdone(data)                                         ((0x00000008&(data))>>3)
#define HDMI_HDCP_SR_hdcp_encrypt_status_shift                                       (2)
#define HDMI_HDCP_SR_hdcp_encrypt_status_mask                                        (0x00000004)
#define HDMI_HDCP_SR_hdcp_encrypt_status(data)                                       (0x00000004&((data)<<2))
#define HDMI_HDCP_SR_hdcp_encrypt_status_src(data)                                   ((0x00000004&(data))>>2)
#define HDMI_HDCP_SR_get_hdcp_encrypt_status(data)                                   ((0x00000004&(data))>>2)
#define HDMI_HDCP_SR_authenticatedok_shift                                           (1)
#define HDMI_HDCP_SR_authenticatedok_mask                                            (0x00000002)
#define HDMI_HDCP_SR_authenticatedok(data)                                           (0x00000002&((data)<<1))
#define HDMI_HDCP_SR_authenticatedok_src(data)                                       ((0x00000002&(data))>>1)
#define HDMI_HDCP_SR_get_authenticatedok(data)                                       ((0x00000002&(data))>>1)
#define HDMI_HDCP_SR_anready_shift                                                   (0)
#define HDMI_HDCP_SR_anready_mask                                                    (0x00000001)
#define HDMI_HDCP_SR_anready(data)                                                   (0x00000001&((data)<<0))
#define HDMI_HDCP_SR_anready_src(data)                                               ((0x00000001&(data))>>0)
#define HDMI_HDCP_SR_get_anready(data)                                               ((0x00000001&(data))>>0)


#define HDMI_HDCP_ANLR                                                               0x1800D108
#define HDMI_HDCP_ANLR_reg_addr                                                      "0xB800D108"
#define HDMI_HDCP_ANLR_reg                                                           0xB800D108
#define set_HDMI_HDCP_ANLR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_ANLR_reg)=data)
#define get_HDMI_HDCP_ANLR_reg   (*((volatile unsigned int*) HDMI_HDCP_ANLR_reg))
#define HDMI_HDCP_ANLR_inst_adr                                                      "0x0042"
#define HDMI_HDCP_ANLR_inst                                                          0x0042
#define HDMI_HDCP_ANLR_anlsw_shift                                                   (0)
#define HDMI_HDCP_ANLR_anlsw_mask                                                    (0xFFFFFFFF)
#define HDMI_HDCP_ANLR_anlsw(data)                                                   (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_ANLR_anlsw_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_ANLR_get_anlsw(data)                                               ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_ANMR                                                               0x1800D10c
#define HDMI_HDCP_ANMR_reg_addr                                                      "0xB800D10C"
#define HDMI_HDCP_ANMR_reg                                                           0xB800D10C
#define set_HDMI_HDCP_ANMR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_ANMR_reg)=data)
#define get_HDMI_HDCP_ANMR_reg   (*((volatile unsigned int*) HDMI_HDCP_ANMR_reg))
#define HDMI_HDCP_ANMR_inst_adr                                                      "0x0043"
#define HDMI_HDCP_ANMR_inst                                                          0x0043
#define HDMI_HDCP_ANMR_anmsw_shift                                                   (0)
#define HDMI_HDCP_ANMR_anmsw_mask                                                    (0xFFFFFFFF)
#define HDMI_HDCP_ANMR_anmsw(data)                                                   (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_ANMR_anmsw_src(data)                                               ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_ANMR_get_anmsw(data)                                               ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_ANILR                                                              0x1800D110
#define HDMI_HDCP_ANILR_reg_addr                                                     "0xB800D110"
#define HDMI_HDCP_ANILR_reg                                                          0xB800D110
#define set_HDMI_HDCP_ANILR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_ANILR_reg)=data)
#define get_HDMI_HDCP_ANILR_reg   (*((volatile unsigned int*) HDMI_HDCP_ANILR_reg))
#define HDMI_HDCP_ANILR_inst_adr                                                     "0x0044"
#define HDMI_HDCP_ANILR_inst                                                         0x0044
#define HDMI_HDCP_ANILR_aninfluencelsw_shift                                         (0)
#define HDMI_HDCP_ANILR_aninfluencelsw_mask                                          (0xFFFFFFFF)
#define HDMI_HDCP_ANILR_aninfluencelsw(data)                                         (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_ANILR_aninfluencelsw_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_ANILR_get_aninfluencelsw(data)                                     ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_ANIMR                                                              0x1800D114
#define HDMI_HDCP_ANIMR_reg_addr                                                     "0xB800D114"
#define HDMI_HDCP_ANIMR_reg                                                          0xB800D114
#define set_HDMI_HDCP_ANIMR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_ANIMR_reg)=data)
#define get_HDMI_HDCP_ANIMR_reg   (*((volatile unsigned int*) HDMI_HDCP_ANIMR_reg))
#define HDMI_HDCP_ANIMR_inst_adr                                                     "0x0045"
#define HDMI_HDCP_ANIMR_inst                                                         0x0045
#define HDMI_HDCP_ANIMR_aninfluencemsw_shift                                         (0)
#define HDMI_HDCP_ANIMR_aninfluencemsw_mask                                          (0xFFFFFFFF)
#define HDMI_HDCP_ANIMR_aninfluencemsw(data)                                         (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_ANIMR_aninfluencemsw_src(data)                                     ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_ANIMR_get_aninfluencemsw(data)                                     ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_DPKLR                                                              0x1800D118
#define HDMI_HDCP_DPKLR_reg_addr                                                     "0xB800D118"
#define HDMI_HDCP_DPKLR_reg                                                          0xB800D118
#define set_HDMI_HDCP_DPKLR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_DPKLR_reg)=data)
#define get_HDMI_HDCP_DPKLR_reg   (*((volatile unsigned int*) HDMI_HDCP_DPKLR_reg))
#define HDMI_HDCP_DPKLR_inst_adr                                                     "0x0046"
#define HDMI_HDCP_DPKLR_inst                                                         0x0046
#define HDMI_HDCP_DPKLR_ddpklsb24_shift                                              (8)
#define HDMI_HDCP_DPKLR_ddpklsb24_mask                                               (0xFFFFFF00)
#define HDMI_HDCP_DPKLR_ddpklsb24(data)                                              (0xFFFFFF00&((data)<<8))
#define HDMI_HDCP_DPKLR_ddpklsb24_src(data)                                          ((0xFFFFFF00&(data))>>8)
#define HDMI_HDCP_DPKLR_get_ddpklsb24(data)                                          ((0xFFFFFF00&(data))>>8)
#define HDMI_HDCP_DPKLR_dpkencpnt_shift                                              (0)
#define HDMI_HDCP_DPKLR_dpkencpnt_mask                                               (0x000000FF)
#define HDMI_HDCP_DPKLR_dpkencpnt(data)                                              (0x000000FF&((data)<<0))
#define HDMI_HDCP_DPKLR_dpkencpnt_src(data)                                          ((0x000000FF&(data))>>0)
#define HDMI_HDCP_DPKLR_get_dpkencpnt(data)                                          ((0x000000FF&(data))>>0)


#define HDMI_HDCP_DPKMR                                                              0x1800D11c
#define HDMI_HDCP_DPKMR_reg_addr                                                     "0xB800D11C"
#define HDMI_HDCP_DPKMR_reg                                                          0xB800D11C
#define set_HDMI_HDCP_DPKMR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_DPKMR_reg)=data)
#define get_HDMI_HDCP_DPKMR_reg   (*((volatile unsigned int*) HDMI_HDCP_DPKMR_reg))
#define HDMI_HDCP_DPKMR_inst_adr                                                     "0x0047"
#define HDMI_HDCP_DPKMR_inst                                                         0x0047
#define HDMI_HDCP_DPKMR_ddpkmsw_shift                                                (0)
#define HDMI_HDCP_DPKMR_ddpkmsw_mask                                                 (0xFFFFFFFF)
#define HDMI_HDCP_DPKMR_ddpkmsw(data)                                                (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_DPKMR_ddpkmsw_src(data)                                            ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_DPKMR_get_ddpkmsw(data)                                            ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_LIR                                                                0x1800D120
#define HDMI_HDCP_LIR_reg_addr                                                       "0xB800D120"
#define HDMI_HDCP_LIR_reg                                                            0xB800D120
#define set_HDMI_HDCP_LIR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_LIR_reg)=data)
#define get_HDMI_HDCP_LIR_reg   (*((volatile unsigned int*) HDMI_HDCP_LIR_reg))
#define HDMI_HDCP_LIR_inst_adr                                                       "0x0048"
#define HDMI_HDCP_LIR_inst                                                           0x0048
#define HDMI_HDCP_LIR_ri_shift                                                       (8)
#define HDMI_HDCP_LIR_ri_mask                                                        (0x00FFFF00)
#define HDMI_HDCP_LIR_ri(data)                                                       (0x00FFFF00&((data)<<8))
#define HDMI_HDCP_LIR_ri_src(data)                                                   ((0x00FFFF00&(data))>>8)
#define HDMI_HDCP_LIR_get_ri(data)                                                   ((0x00FFFF00&(data))>>8)
#define HDMI_HDCP_LIR_pj_shift                                                       (0)
#define HDMI_HDCP_LIR_pj_mask                                                        (0x000000FF)
#define HDMI_HDCP_LIR_pj(data)                                                       (0x000000FF&((data)<<0))
#define HDMI_HDCP_LIR_pj_src(data)                                                   ((0x000000FF&(data))>>0)
#define HDMI_HDCP_LIR_get_pj(data)                                                   ((0x000000FF&(data))>>0)


#define HDMI_HDCP_SHACR                                                              0x1800D124
#define HDMI_HDCP_SHACR_reg_addr                                                     "0xB800D124"
#define HDMI_HDCP_SHACR_reg                                                          0xB800D124
#define set_HDMI_HDCP_SHACR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_SHACR_reg)=data)
#define get_HDMI_HDCP_SHACR_reg   (*((volatile unsigned int*) HDMI_HDCP_SHACR_reg))
#define HDMI_HDCP_SHACR_inst_adr                                                     "0x0049"
#define HDMI_HDCP_SHACR_inst                                                         0x0049
#define HDMI_HDCP_SHACR_shastart_shift                                               (3)
#define HDMI_HDCP_SHACR_shastart_mask                                                (0x00000008)
#define HDMI_HDCP_SHACR_shastart(data)                                               (0x00000008&((data)<<3))
#define HDMI_HDCP_SHACR_shastart_src(data)                                           ((0x00000008&(data))>>3)
#define HDMI_HDCP_SHACR_get_shastart(data)                                           ((0x00000008&(data))>>3)
#define HDMI_HDCP_SHACR_shafirst_shift                                               (2)
#define HDMI_HDCP_SHACR_shafirst_mask                                                (0x00000004)
#define HDMI_HDCP_SHACR_shafirst(data)                                               (0x00000004&((data)<<2))
#define HDMI_HDCP_SHACR_shafirst_src(data)                                           ((0x00000004&(data))>>2)
#define HDMI_HDCP_SHACR_get_shafirst(data)                                           ((0x00000004&(data))>>2)
#define HDMI_HDCP_SHACR_rstshaptr_shift                                              (1)
#define HDMI_HDCP_SHACR_rstshaptr_mask                                               (0x00000002)
#define HDMI_HDCP_SHACR_rstshaptr(data)                                              (0x00000002&((data)<<1))
#define HDMI_HDCP_SHACR_rstshaptr_src(data)                                          ((0x00000002&(data))>>1)
#define HDMI_HDCP_SHACR_get_rstshaptr(data)                                          ((0x00000002&(data))>>1)
#define HDMI_HDCP_SHACR_write_data_shift                                             (0)
#define HDMI_HDCP_SHACR_write_data_mask                                              (0x00000001)
#define HDMI_HDCP_SHACR_write_data(data)                                             (0x00000001&((data)<<0))
#define HDMI_HDCP_SHACR_write_data_src(data)                                         ((0x00000001&(data))>>0)
#define HDMI_HDCP_SHACR_get_write_data(data)                                         ((0x00000001&(data))>>0)


#define HDMI_HDCP_SHARR                                                              0x1800D128
#define HDMI_HDCP_SHARR_reg_addr                                                     "0xB800D128"
#define HDMI_HDCP_SHARR_reg                                                          0xB800D128
#define set_HDMI_HDCP_SHARR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_SHARR_reg)=data)
#define get_HDMI_HDCP_SHARR_reg   (*((volatile unsigned int*) HDMI_HDCP_SHARR_reg))
#define HDMI_HDCP_SHARR_inst_adr                                                     "0x004A"
#define HDMI_HDCP_SHARR_inst                                                         0x004A
#define HDMI_HDCP_SHARR_vmatch_shift                                                 (1)
#define HDMI_HDCP_SHARR_vmatch_mask                                                  (0x00000002)
#define HDMI_HDCP_SHARR_vmatch(data)                                                 (0x00000002&((data)<<1))
#define HDMI_HDCP_SHARR_vmatch_src(data)                                             ((0x00000002&(data))>>1)
#define HDMI_HDCP_SHARR_get_vmatch(data)                                             ((0x00000002&(data))>>1)
#define HDMI_HDCP_SHARR_shaready_shift                                               (0)
#define HDMI_HDCP_SHARR_shaready_mask                                                (0x00000001)
#define HDMI_HDCP_SHARR_shaready(data)                                               (0x00000001&((data)<<0))
#define HDMI_HDCP_SHARR_shaready_src(data)                                           ((0x00000001&(data))>>0)
#define HDMI_HDCP_SHARR_get_shaready(data)                                           ((0x00000001&(data))>>0)


#define HDMI_HDCP_SHADR                                                              0x1800D12c
#define HDMI_HDCP_SHADR_reg_addr                                                     "0xB800D12C"
#define HDMI_HDCP_SHADR_reg                                                          0xB800D12C
#define set_HDMI_HDCP_SHADR_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_SHADR_reg)=data)
#define get_HDMI_HDCP_SHADR_reg   (*((volatile unsigned int*) HDMI_HDCP_SHADR_reg))
#define HDMI_HDCP_SHADR_inst_adr                                                     "0x004B"
#define HDMI_HDCP_SHADR_inst                                                         0x004B
#define HDMI_HDCP_SHADR_sha_data_shift                                               (0)
#define HDMI_HDCP_SHADR_sha_data_mask                                                (0xFFFFFFFF)
#define HDMI_HDCP_SHADR_sha_data(data)                                               (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_SHADR_sha_data_src(data)                                           ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_SHADR_get_sha_data(data)                                           ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_MILSW                                                              0x1800D130
#define HDMI_HDCP_MILSW_reg_addr                                                     "0xB800D130"
#define HDMI_HDCP_MILSW_reg                                                          0xB800D130
#define set_HDMI_HDCP_MILSW_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_MILSW_reg)=data)
#define get_HDMI_HDCP_MILSW_reg   (*((volatile unsigned int*) HDMI_HDCP_MILSW_reg))
#define HDMI_HDCP_MILSW_inst_adr                                                     "0x004C"
#define HDMI_HDCP_MILSW_inst                                                         0x004C
#define HDMI_HDCP_MILSW_milsw_shift                                                  (0)
#define HDMI_HDCP_MILSW_milsw_mask                                                   (0xFFFFFFFF)
#define HDMI_HDCP_MILSW_milsw(data)                                                  (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_MILSW_milsw_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_MILSW_get_milsw(data)                                              ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_MIMSW                                                              0x1800D134
#define HDMI_HDCP_MIMSW_reg_addr                                                     "0xB800D134"
#define HDMI_HDCP_MIMSW_reg                                                          0xB800D134
#define set_HDMI_HDCP_MIMSW_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_MIMSW_reg)=data)
#define get_HDMI_HDCP_MIMSW_reg   (*((volatile unsigned int*) HDMI_HDCP_MIMSW_reg))
#define HDMI_HDCP_MIMSW_inst_adr                                                     "0x004D"
#define HDMI_HDCP_MIMSW_inst                                                         0x004D
#define HDMI_HDCP_MIMSW_mimsw_shift                                                  (0)
#define HDMI_HDCP_MIMSW_mimsw_mask                                                   (0xFFFFFFFF)
#define HDMI_HDCP_MIMSW_mimsw(data)                                                  (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_MIMSW_mimsw_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_MIMSW_get_mimsw(data)                                              ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_KMLSW                                                              0x1800D138
#define HDMI_HDCP_KMLSW_reg_addr                                                     "0xB800D138"
#define HDMI_HDCP_KMLSW_reg                                                          0xB800D138
#define set_HDMI_HDCP_KMLSW_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_KMLSW_reg)=data)
#define get_HDMI_HDCP_KMLSW_reg   (*((volatile unsigned int*) HDMI_HDCP_KMLSW_reg))
#define HDMI_HDCP_KMLSW_inst_adr                                                     "0x004E"
#define HDMI_HDCP_KMLSW_inst                                                         0x004E
#define HDMI_HDCP_KMLSW_kmlsw_shift                                                  (0)
#define HDMI_HDCP_KMLSW_kmlsw_mask                                                   (0xFFFFFFFF)
#define HDMI_HDCP_KMLSW_kmlsw(data)                                                  (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_KMLSW_kmlsw_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_KMLSW_get_kmlsw(data)                                              ((0xFFFFFFFF&(data))>>0)


#define HDMI_HDCP_KMMSW                                                              0x1800D13c
#define HDMI_HDCP_KMMSW_reg_addr                                                     "0xB800D13C"
#define HDMI_HDCP_KMMSW_reg                                                          0xB800D13C
#define set_HDMI_HDCP_KMMSW_reg(data)   (*((volatile unsigned int*) HDMI_HDCP_KMMSW_reg)=data)
#define get_HDMI_HDCP_KMMSW_reg   (*((volatile unsigned int*) HDMI_HDCP_KMMSW_reg))
#define HDMI_HDCP_KMMSW_inst_adr                                                     "0x004F"
#define HDMI_HDCP_KMMSW_inst                                                         0x004F
#define HDMI_HDCP_KMMSW_kmmsw_shift                                                  (0)
#define HDMI_HDCP_KMMSW_kmmsw_mask                                                   (0xFFFFFFFF)
#define HDMI_HDCP_KMMSW_kmmsw(data)                                                  (0xFFFFFFFF&((data)<<0))
#define HDMI_HDCP_KMMSW_kmmsw_src(data)                                              ((0xFFFFFFFF&(data))>>0)
#define HDMI_HDCP_KMMSW_get_kmmsw(data)                                              ((0xFFFFFFFF&(data))>>0)


#define HDMI_DBG                                                                     0x1800D140
#define HDMI_DBG_reg_addr                                                            "0xB800D140"
#define HDMI_DBG_reg                                                                 0xB800D140
#define set_HDMI_DBG_reg(data)   (*((volatile unsigned int*) HDMI_DBG_reg)=data)
#define get_HDMI_DBG_reg   (*((volatile unsigned int*) HDMI_DBG_reg))
#define HDMI_DBG_inst_adr                                                            "0x0050"
#define HDMI_DBG_inst                                                                0x0050
#define HDMI_DBG_chsel0_shift                                                        (4)
#define HDMI_DBG_chsel0_mask                                                         (0x000000F0)
#define HDMI_DBG_chsel0(data)                                                        (0x000000F0&((data)<<4))
#define HDMI_DBG_chsel0_src(data)                                                    ((0x000000F0&(data))>>4)
#define HDMI_DBG_get_chsel0(data)                                                    ((0x000000F0&(data))>>4)
#define HDMI_DBG_chsel1_shift                                                        (0)
#define HDMI_DBG_chsel1_mask                                                         (0x0000000F)
#define HDMI_DBG_chsel1(data)                                                        (0x0000000F&((data)<<0))
#define HDMI_DBG_chsel1_src(data)                                                    ((0x0000000F&(data))>>0)
#define HDMI_DBG_get_chsel1(data)                                                    ((0x0000000F&(data))>>0)


#define HDMI_DUMMY_0                                                                 0x1800D144
#define HDMI_DUMMY_1                                                                 0x1800D148
#define HDMI_DUMMY_0_reg_addr                                                        "0xB800D144"
#define HDMI_DUMMY_1_reg_addr                                                        "0xB800D148"
#define HDMI_DUMMY_0_reg                                                             0xB800D144
#define HDMI_DUMMY_1_reg                                                             0xB800D148
#define set_HDMI_DUMMY_0_reg(data)   (*((volatile unsigned int*) HDMI_DUMMY_0_reg)=data)
#define set_HDMI_DUMMY_1_reg(data)   (*((volatile unsigned int*) HDMI_DUMMY_1_reg)=data)
#define get_HDMI_DUMMY_0_reg   (*((volatile unsigned int*) HDMI_DUMMY_0_reg))
#define get_HDMI_DUMMY_1_reg   (*((volatile unsigned int*) HDMI_DUMMY_1_reg))
#define HDMI_DUMMY_0_inst_adr                                                        "0x0051"
#define HDMI_DUMMY_1_inst_adr                                                        "0x0052"
#define HDMI_DUMMY_0_inst                                                            0x0051
#define HDMI_DUMMY_1_inst                                                            0x0052
#define HDMI_DUMMY_dummy_shift                                                       (0)
#define HDMI_DUMMY_dummy_mask                                                        (0xFFFFFFFF)
#define HDMI_DUMMY_dummy(data)                                                       (0xFFFFFFFF&((data)<<0))
#define HDMI_DUMMY_dummy_src(data)                                                   ((0xFFFFFFFF&(data))>>0)
#define HDMI_DUMMY_get_dummy(data)                                                   ((0xFFFFFFFF&(data))>>0)


#define HDMI_PHY                                                                     0x1800D14C
#define HDMI_PHY_reg_addr                                                            "0xB800D14C"
#define HDMI_PHY_reg                                                                 0xB800D14C
#define set_HDMI_PHY_reg(data)   (*((volatile unsigned int*) HDMI_PHY_reg)=data)
#define get_HDMI_PHY_reg   (*((volatile unsigned int*) HDMI_PHY_reg))
#define HDMI_PHY_inst_adr                                                            "0x0053"
#define HDMI_PHY_inst                                                                0x0053
#define HDMI_PHY_test_mode_shift                                                     (1)
#define HDMI_PHY_test_mode_mask                                                      (0x00000002)
#define HDMI_PHY_test_mode(data)                                                     (0x00000002&((data)<<1))
#define HDMI_PHY_test_mode_src(data)                                                 ((0x00000002&(data))>>1)
#define HDMI_PHY_get_test_mode(data)                                                 ((0x00000002&(data))>>1)
#define HDMI_PHY_data_realign_shift                                                  (0)
#define HDMI_PHY_data_realign_mask                                                   (0x00000001)
#define HDMI_PHY_data_realign(data)                                                  (0x00000001&((data)<<0))
#define HDMI_PHY_data_realign_src(data)                                              ((0x00000001&(data))>>0)
#define HDMI_PHY_get_data_realign(data)                                              ((0x00000001&(data))>>0)


#define HDMI_H_PARA1                                                                 0x1800D234
#define HDMI_H_PARA1_reg_addr                                                        "0xB800D234"
#define HDMI_H_PARA1_reg                                                             0xB800D234
#define set_HDMI_H_PARA1_reg(data)   (*((volatile unsigned int*) HDMI_H_PARA1_reg)=data)
#define get_HDMI_H_PARA1_reg   (*((volatile unsigned int*) HDMI_H_PARA1_reg))
#define HDMI_H_PARA1_inst_adr                                                        "0x008D"
#define HDMI_H_PARA1_inst                                                            0x008D
#define HDMI_H_PARA1_hblank_shift                                                    (16)
#define HDMI_H_PARA1_hblank_mask                                                     (0x3FFF0000)
#define HDMI_H_PARA1_hblank(data)                                                    (0x3FFF0000&((data)<<16))
#define HDMI_H_PARA1_hblank_src(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_H_PARA1_get_hblank(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_H_PARA1_hactive_shift                                                   (0)
#define HDMI_H_PARA1_hactive_mask                                                    (0x00003FFF)
#define HDMI_H_PARA1_hactive(data)                                                   (0x00003FFF&((data)<<0))
#define HDMI_H_PARA1_hactive_src(data)                                               ((0x00003FFF&(data))>>0)
#define HDMI_H_PARA1_get_hactive(data)                                               ((0x00003FFF&(data))>>0)


#define HDMI_H_PARA2                                                                 0x1800D238
#define HDMI_H_PARA2_reg_addr                                                        "0xB800D238"
#define HDMI_H_PARA2_reg                                                             0xB800D238
#define set_HDMI_H_PARA2_reg(data)   (*((volatile unsigned int*) HDMI_H_PARA2_reg)=data)
#define get_HDMI_H_PARA2_reg   (*((volatile unsigned int*) HDMI_H_PARA2_reg))
#define HDMI_H_PARA2_inst_adr                                                        "0x008E"
#define HDMI_H_PARA2_inst                                                            0x008E
#define HDMI_H_PARA2_hsync_shift                                                     (16)
#define HDMI_H_PARA2_hsync_mask                                                      (0x3FFF0000)
#define HDMI_H_PARA2_hsync(data)                                                     (0x3FFF0000&((data)<<16))
#define HDMI_H_PARA2_hsync_src(data)                                                 ((0x3FFF0000&(data))>>16)
#define HDMI_H_PARA2_get_hsync(data)                                                 ((0x3FFF0000&(data))>>16)
#define HDMI_H_PARA2_hfront_shift                                                    (0)
#define HDMI_H_PARA2_hfront_mask                                                     (0x00003FFF)
#define HDMI_H_PARA2_hfront(data)                                                    (0x00003FFF&((data)<<0))
#define HDMI_H_PARA2_hfront_src(data)                                                ((0x00003FFF&(data))>>0)
#define HDMI_H_PARA2_get_hfront(data)                                                ((0x00003FFF&(data))>>0)


#define HDMI_H_PARA3                                                                 0x1800D23c
#define HDMI_H_PARA3_reg_addr                                                        "0xB800D23C"
#define HDMI_H_PARA3_reg                                                             0xB800D23C
#define set_HDMI_H_PARA3_reg(data)   (*((volatile unsigned int*) HDMI_H_PARA3_reg)=data)
#define get_HDMI_H_PARA3_reg   (*((volatile unsigned int*) HDMI_H_PARA3_reg))
#define HDMI_H_PARA3_inst_adr                                                        "0x008F"
#define HDMI_H_PARA3_inst                                                            0x008F
#define HDMI_H_PARA3_hback_shift                                                     (0)
#define HDMI_H_PARA3_hback_mask                                                      (0x00003FFF)
#define HDMI_H_PARA3_hback(data)                                                     (0x00003FFF&((data)<<0))
#define HDMI_H_PARA3_hback_src(data)                                                 ((0x00003FFF&(data))>>0)
#define HDMI_H_PARA3_get_hback(data)                                                 ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA1                                                                 0x1800D240
#define HDMI_V_PARA1_reg_addr                                                        "0xB800D240"
#define HDMI_V_PARA1_reg                                                             0xB800D240
#define set_HDMI_V_PARA1_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA1_reg)=data)
#define get_HDMI_V_PARA1_reg   (*((volatile unsigned int*) HDMI_V_PARA1_reg))
#define HDMI_V_PARA1_inst_adr                                                        "0x0090"
#define HDMI_V_PARA1_inst                                                            0x0090
#define HDMI_V_PARA1_Vact_video_shift                                                (16)
#define HDMI_V_PARA1_Vact_video_mask                                                 (0x3FFF0000)
#define HDMI_V_PARA1_Vact_video(data)                                                (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA1_Vact_video_src(data)                                            ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA1_get_Vact_video(data)                                            ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA1_vactive_shift                                                   (0)
#define HDMI_V_PARA1_vactive_mask                                                    (0x00003FFF)
#define HDMI_V_PARA1_vactive(data)                                                   (0x00003FFF&((data)<<0))
#define HDMI_V_PARA1_vactive_src(data)                                               ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA1_get_vactive(data)                                               ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA2                                                                 0x1800D244
#define HDMI_V_PARA2_reg_addr                                                        "0xB800D244"
#define HDMI_V_PARA2_reg                                                             0xB800D244
#define set_HDMI_V_PARA2_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA2_reg)=data)
#define get_HDMI_V_PARA2_reg   (*((volatile unsigned int*) HDMI_V_PARA2_reg))
#define HDMI_V_PARA2_inst_adr                                                        "0x0091"
#define HDMI_V_PARA2_inst                                                            0x0091
#define HDMI_V_PARA2_Vact_space1_shift                                               (16)
#define HDMI_V_PARA2_Vact_space1_mask                                                (0x3FFF0000)
#define HDMI_V_PARA2_Vact_space1(data)                                               (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA2_Vact_space1_src(data)                                           ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA2_get_Vact_space1(data)                                           ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA2_Vact_space_shift                                                (0)
#define HDMI_V_PARA2_Vact_space_mask                                                 (0x00003FFF)
#define HDMI_V_PARA2_Vact_space(data)                                                (0x00003FFF&((data)<<0))
#define HDMI_V_PARA2_Vact_space_src(data)                                            ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA2_get_Vact_space(data)                                            ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA3                                                                 0x1800D248
#define HDMI_V_PARA3_reg_addr                                                        "0xB800D248"
#define HDMI_V_PARA3_reg                                                             0xB800D248
#define set_HDMI_V_PARA3_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA3_reg)=data)
#define get_HDMI_V_PARA3_reg   (*((volatile unsigned int*) HDMI_V_PARA3_reg))
#define HDMI_V_PARA3_inst_adr                                                        "0x0092"
#define HDMI_V_PARA3_inst                                                            0x0092
#define HDMI_V_PARA3_Vblank3_shift                                                   (16)
#define HDMI_V_PARA3_Vblank3_mask                                                    (0x3FFF0000)
#define HDMI_V_PARA3_Vblank3(data)                                                   (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA3_Vblank3_src(data)                                               ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA3_get_Vblank3(data)                                               ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA3_Vact_space2_shift                                               (0)
#define HDMI_V_PARA3_Vact_space2_mask                                                (0x00003FFF)
#define HDMI_V_PARA3_Vact_space2(data)                                               (0x00003FFF&((data)<<0))
#define HDMI_V_PARA3_Vact_space2_src(data)                                           ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA3_get_Vact_space2(data)                                           ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA4                                                                 0x1800D24C
#define HDMI_V_PARA4_reg_addr                                                        "0xB800D24C"
#define HDMI_V_PARA4_reg                                                             0xB800D24C
#define set_HDMI_V_PARA4_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA4_reg)=data)
#define get_HDMI_V_PARA4_reg   (*((volatile unsigned int*) HDMI_V_PARA4_reg))
#define HDMI_V_PARA4_inst_adr                                                        "0x0093"
#define HDMI_V_PARA4_inst                                                            0x0093
#define HDMI_V_PARA4_vsync_shift                                                     (16)
#define HDMI_V_PARA4_vsync_mask                                                      (0x3FFF0000)
#define HDMI_V_PARA4_vsync(data)                                                     (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA4_vsync_src(data)                                                 ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA4_get_vsync(data)                                                 ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA4_vblank_shift                                                    (0)
#define HDMI_V_PARA4_vblank_mask                                                     (0x00003FFF)
#define HDMI_V_PARA4_vblank(data)                                                    (0x00003FFF&((data)<<0))
#define HDMI_V_PARA4_vblank_src(data)                                                ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA4_get_vblank(data)                                                ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA5                                                                 0x1800D250
#define HDMI_V_PARA5_reg_addr                                                        "0xB800D250"
#define HDMI_V_PARA5_reg                                                             0xB800D250
#define set_HDMI_V_PARA5_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA5_reg)=data)
#define get_HDMI_V_PARA5_reg   (*((volatile unsigned int*) HDMI_V_PARA5_reg))
#define HDMI_V_PARA5_inst_adr                                                        "0x0094"
#define HDMI_V_PARA5_inst                                                            0x0094
#define HDMI_V_PARA5_vback_shift                                                     (16)
#define HDMI_V_PARA5_vback_mask                                                      (0x3FFF0000)
#define HDMI_V_PARA5_vback(data)                                                     (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA5_vback_src(data)                                                 ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA5_get_vback(data)                                                 ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA5_vfront_shift                                                    (0)
#define HDMI_V_PARA5_vfront_mask                                                     (0x00003FFF)
#define HDMI_V_PARA5_vfront(data)                                                    (0x00003FFF&((data)<<0))
#define HDMI_V_PARA5_vfront_src(data)                                                ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA5_get_vfront(data)                                                ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA6                                                                 0x1800D254
#define HDMI_V_PARA6_reg_addr                                                        "0xB800D254"
#define HDMI_V_PARA6_reg                                                             0xB800D254
#define set_HDMI_V_PARA6_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA6_reg)=data)
#define get_HDMI_V_PARA6_reg   (*((volatile unsigned int*) HDMI_V_PARA6_reg))
#define HDMI_V_PARA6_inst_adr                                                        "0x0095"
#define HDMI_V_PARA6_inst                                                            0x0095
#define HDMI_V_PARA6_Vsync1_shift                                                    (16)
#define HDMI_V_PARA6_Vsync1_mask                                                     (0x3FFF0000)
#define HDMI_V_PARA6_Vsync1(data)                                                    (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA6_Vsync1_src(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA6_get_Vsync1(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA6_Vblank1_shift                                                   (0)
#define HDMI_V_PARA6_Vblank1_mask                                                    (0x00003FFF)
#define HDMI_V_PARA6_Vblank1(data)                                                   (0x00003FFF&((data)<<0))
#define HDMI_V_PARA6_Vblank1_src(data)                                               ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA6_get_Vblank1(data)                                               ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA7                                                                 0x1800D258
#define HDMI_V_PARA7_reg_addr                                                        "0xB800D258"
#define HDMI_V_PARA7_reg                                                             0xB800D258
#define set_HDMI_V_PARA7_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA7_reg)=data)
#define get_HDMI_V_PARA7_reg   (*((volatile unsigned int*) HDMI_V_PARA7_reg))
#define HDMI_V_PARA7_inst_adr                                                        "0x0096"
#define HDMI_V_PARA7_inst                                                            0x0096
#define HDMI_V_PARA7_Vback1_shift                                                    (16)
#define HDMI_V_PARA7_Vback1_mask                                                     (0x3FFF0000)
#define HDMI_V_PARA7_Vback1(data)                                                    (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA7_Vback1_src(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA7_get_Vback1(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA7_Vfront1_shift                                                   (0)
#define HDMI_V_PARA7_Vfront1_mask                                                    (0x00003FFF)
#define HDMI_V_PARA7_Vfront1(data)                                                   (0x00003FFF&((data)<<0))
#define HDMI_V_PARA7_Vfront1_src(data)                                               ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA7_get_Vfront1(data)                                               ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA8                                                                 0x1800D25C
#define HDMI_V_PARA8_reg_addr                                                        "0xB800D25C"
#define HDMI_V_PARA8_reg                                                             0xB800D25C
#define set_HDMI_V_PARA8_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA8_reg)=data)
#define get_HDMI_V_PARA8_reg   (*((volatile unsigned int*) HDMI_V_PARA8_reg))
#define HDMI_V_PARA8_inst_adr                                                        "0x0097"
#define HDMI_V_PARA8_inst                                                            0x0097
#define HDMI_V_PARA8_Vsync2_shift                                                    (16)
#define HDMI_V_PARA8_Vsync2_mask                                                     (0x3FFF0000)
#define HDMI_V_PARA8_Vsync2(data)                                                    (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA8_Vsync2_src(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA8_get_Vsync2(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA8_Vblank2_shift                                                   (0)
#define HDMI_V_PARA8_Vblank2_mask                                                    (0x00003FFF)
#define HDMI_V_PARA8_Vblank2(data)                                                   (0x00003FFF&((data)<<0))
#define HDMI_V_PARA8_Vblank2_src(data)                                               ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA8_get_Vblank2(data)                                               ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA9                                                                 0x1800D260
#define HDMI_V_PARA9_reg_addr                                                        "0xB800D260"
#define HDMI_V_PARA9_reg                                                             0xB800D260
#define set_HDMI_V_PARA9_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA9_reg)=data)
#define get_HDMI_V_PARA9_reg   (*((volatile unsigned int*) HDMI_V_PARA9_reg))
#define HDMI_V_PARA9_inst_adr                                                        "0x0098"
#define HDMI_V_PARA9_inst                                                            0x0098
#define HDMI_V_PARA9_Vback2_shift                                                    (16)
#define HDMI_V_PARA9_Vback2_mask                                                     (0x3FFF0000)
#define HDMI_V_PARA9_Vback2(data)                                                    (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA9_Vback2_src(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA9_get_Vback2(data)                                                ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA9_Vfront2_shift                                                   (0)
#define HDMI_V_PARA9_Vfront2_mask                                                    (0x00003FFF)
#define HDMI_V_PARA9_Vfront2(data)                                                   (0x00003FFF&((data)<<0))
#define HDMI_V_PARA9_Vfront2_src(data)                                               ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA9_get_Vfront2(data)                                               ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA12                                                                0x1800D264
#define HDMI_V_PARA12_reg_addr                                                       "0xB800D264"
#define HDMI_V_PARA12_reg                                                            0xB800D264
#define set_HDMI_V_PARA12_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA12_reg)=data)
#define get_HDMI_V_PARA12_reg   (*((volatile unsigned int*) HDMI_V_PARA12_reg))
#define HDMI_V_PARA12_inst_adr                                                       "0x0099"
#define HDMI_V_PARA12_inst                                                           0x0099
#define HDMI_V_PARA12_vsynci_shift                                                   (16)
#define HDMI_V_PARA12_vsynci_mask                                                    (0x3FFF0000)
#define HDMI_V_PARA12_vsynci(data)                                                   (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA12_vsynci_src(data)                                               ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA12_get_vsynci(data)                                               ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA12_vblanki_shift                                                  (0)
#define HDMI_V_PARA12_vblanki_mask                                                   (0x00003FFF)
#define HDMI_V_PARA12_vblanki(data)                                                  (0x00003FFF&((data)<<0))
#define HDMI_V_PARA12_vblanki_src(data)                                              ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA12_get_vblanki(data)                                              ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA13                                                                0x1800D268
#define HDMI_V_PARA13_reg_addr                                                       "0xB800D268"
#define HDMI_V_PARA13_reg                                                            0xB800D268
#define set_HDMI_V_PARA13_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA13_reg)=data)
#define get_HDMI_V_PARA13_reg   (*((volatile unsigned int*) HDMI_V_PARA13_reg))
#define HDMI_V_PARA13_inst_adr                                                       "0x009A"
#define HDMI_V_PARA13_inst                                                           0x009A
#define HDMI_V_PARA13_vbacki_shift                                                   (16)
#define HDMI_V_PARA13_vbacki_mask                                                    (0x3FFF0000)
#define HDMI_V_PARA13_vbacki(data)                                                   (0x3FFF0000&((data)<<16))
#define HDMI_V_PARA13_vbacki_src(data)                                               ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA13_get_vbacki(data)                                               ((0x3FFF0000&(data))>>16)
#define HDMI_V_PARA13_vfronti_shift                                                  (0)
#define HDMI_V_PARA13_vfronti_mask                                                   (0x00003FFF)
#define HDMI_V_PARA13_vfronti(data)                                                  (0x00003FFF&((data)<<0))
#define HDMI_V_PARA13_vfronti_src(data)                                              ((0x00003FFF&(data))>>0)
#define HDMI_V_PARA13_get_vfronti(data)                                              ((0x00003FFF&(data))>>0)


#define HDMI_V_PARA10                                                                0x1800D26C
#define HDMI_V_PARA10_reg_addr                                                       "0xB800D26C"
#define HDMI_V_PARA10_reg                                                            0xB800D26C
#define set_HDMI_V_PARA10_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA10_reg)=data)
#define get_HDMI_V_PARA10_reg   (*((volatile unsigned int*) HDMI_V_PARA10_reg))
#define HDMI_V_PARA10_inst_adr                                                       "0x009B"
#define HDMI_V_PARA10_inst                                                           0x009B
#define HDMI_V_PARA10_Y_shift                                                        (0)
#define HDMI_V_PARA10_Y_mask                                                         (0x00000FFF)
#define HDMI_V_PARA10_Y(data)                                                        (0x00000FFF&((data)<<0))
#define HDMI_V_PARA10_Y_src(data)                                                    ((0x00000FFF&(data))>>0)
#define HDMI_V_PARA10_get_Y(data)                                                    ((0x00000FFF&(data))>>0)


#define HDMI_V_PARA11                                                                0x1800D270
#define HDMI_V_PARA11_reg_addr                                                       "0xB800D270"
#define HDMI_V_PARA11_reg                                                            0xB800D270
#define set_HDMI_V_PARA11_reg(data)   (*((volatile unsigned int*) HDMI_V_PARA11_reg)=data)
#define get_HDMI_V_PARA11_reg   (*((volatile unsigned int*) HDMI_V_PARA11_reg))
#define HDMI_V_PARA11_inst_adr                                                       "0x009C"
#define HDMI_V_PARA11_inst                                                           0x009C
#define HDMI_V_PARA11_Cb_shift                                                       (16)
#define HDMI_V_PARA11_Cb_mask                                                        (0x0FFF0000)
#define HDMI_V_PARA11_Cb(data)                                                       (0x0FFF0000&((data)<<16))
#define HDMI_V_PARA11_Cb_src(data)                                                   ((0x0FFF0000&(data))>>16)
#define HDMI_V_PARA11_get_Cb(data)                                                   ((0x0FFF0000&(data))>>16)
#define HDMI_V_PARA11_Cr_shift                                                       (0)
#define HDMI_V_PARA11_Cr_mask                                                        (0x00000FFF)
#define HDMI_V_PARA11_Cr(data)                                                       (0x00000FFF&((data)<<0))
#define HDMI_V_PARA11_Cr_src(data)                                                   ((0x00000FFF&(data))>>0)
#define HDMI_V_PARA11_get_Cr(data)                                                   ((0x00000FFF&(data))>>0)


#endif
