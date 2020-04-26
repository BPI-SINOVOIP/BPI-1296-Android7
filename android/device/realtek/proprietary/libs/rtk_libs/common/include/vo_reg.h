/**************************************************************
// Spec Version                  : 0.8
// Parser Version                : DVR_Parser_5.0
// CModelGen Version             : 4.1 2009.03.10
// Naming Rule                   : Module_Register_Name
// Firmware Header Generate Date : 2010/6/9
***************************************************************/

#ifndef __ASSEMBLY__
#define _VO_USE_STRUCT
#endif

#ifndef _VO_REG_H_INCLUDED_
#define _VO_REG_H_INCLUDED_
#ifdef  _VO_USE_STRUCT
typedef struct 
{
unsigned int     reserved_0:27;
unsigned int     ch2i:1;
unsigned int     ch2:1;
unsigned int     ch1i:1;
unsigned int     ch1:1;
unsigned int     write_data:1;
}vo_MODE;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     write_en3:1;
unsigned int     h:2;
unsigned int     reserved_1:1;
unsigned int     write_en2:1;
unsigned int     i:2;
unsigned int     reserved_2:1;
unsigned int     write_en1:1;
unsigned int     p:2;
}vo_OUT;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     i_vbi:1;
unsigned int     vbi:1;
unsigned int     csr:1;
unsigned int     osd2:1;
unsigned int     osd1:1;
unsigned int     sub1:1;
unsigned int     v2:1;
unsigned int     V1:1;
unsigned int     reserved_1:1;
unsigned int     pic:1;
unsigned int     bg:1;
unsigned int     write_data:1;
}vo_MIX1;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     i_vbi:1;
unsigned int     vbi:1;
unsigned int     reserved_1:1;
unsigned int     reserved_2:1;
unsigned int     reserved_3:1;
unsigned int     reserved_4:1;
unsigned int     reserved_5:1;
unsigned int     reserved_6:1;
unsigned int     dd:1;
unsigned int     reserved_7:1;
unsigned int     bg:1;
unsigned int     write_data:1;
}vo_MIX2;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     osd2:1;
unsigned int     osd1:1;
unsigned int     sub1:1;
unsigned int     v2:1;
unsigned int     v1:1;
unsigned int     reserved_1:1;
unsigned int     mix1:1;
unsigned int     write_data:1;
}vo_WB;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     top_and_bottom:1;
unsigned int     side_by_side:1;
unsigned int     right:1;
unsigned int     write_data:1;
}vo_3D;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     wbgo:1;
unsigned int     m2go:1;
unsigned int     m1go:1;
unsigned int     write_data:1;
}vo_FC;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     ref:2;
unsigned int     reserved_1:1;
unsigned int     pos:11;
unsigned int     reserved_2:1;
unsigned int     th:11;
}vo_VBI_END;

typedef struct 
{
unsigned int     reserved_0:12;
unsigned int     d3_dbh:1;
unsigned int     d3_dbw:1;
unsigned int     d3_dup:1;
unsigned int     reserved_1:1;
unsigned int     st:1;
unsigned int     reserved_2:1;
unsigned int     cd:1;
unsigned int     gfx:1;
unsigned int     di:1;
unsigned int     f422:1;
unsigned int     vs:1;
unsigned int     vs_near:1;
unsigned int     vs_yodd:1;
unsigned int     vs_codd:1;
unsigned int     hs:1;
unsigned int     hs_yodd:1;
unsigned int     hs_codd:1;
unsigned int     topfield:1;
unsigned int     dmaweave:1;
unsigned int     write_data:1;
}vo_V1;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     w_c:7;
unsigned int     reserved_1:1;
unsigned int     w_y:7;
unsigned int     reserved_2:1;
unsigned int     c:7;
unsigned int     reserved_3:1;
unsigned int     y:7;
}vo_V1_INDEX;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     w_c:7;
unsigned int     reserved_1:1;
unsigned int     w_y:7;
unsigned int     reserved_2:1;
unsigned int     c:7;
unsigned int     reserved_3:1;
unsigned int     y:7;
}vo_V1_INDEX_3D;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     w_c_en:1;
unsigned int     w_c:3;
unsigned int     w_y_en:1;
unsigned int     w_y:3;
unsigned int     c_en:1;
unsigned int     c:3;
unsigned int     y_en:1;
unsigned int     y:3;
}vo_V1_INDEX_RR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     sa:31;
}vo_V1_SEQ;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     sa:31;
}vo_V1_SEQ_3D;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     endian:1;
unsigned int     format:2;
unsigned int     reserved_1:3;
unsigned int     extend:1;
unsigned int     reserved_2:8;
unsigned int     pitch:16;
}vo_V1_GFX;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_V1_GC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_V1_GC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_V1_GC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_V1_GC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_V1_GC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_V1_GC6;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     a:9;
}vo_V1_ALPHA;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     w:11;
unsigned int     reserved_1:1;
unsigned int     h:11;
}vo_V1_SIZE;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     th:9;
}vo_V1_BUF;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     mode:2;
}vo_V1_DI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     n_c:7;
unsigned int     reserved_1:1;
unsigned int     n_y:7;
unsigned int     reserved_2:1;
unsigned int     p_c:7;
unsigned int     reserved_3:1;
unsigned int     p_y:7;
}vo_V1_DI_3F;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     n_c_en:1;
unsigned int     n_c:3;
unsigned int     n_y_en:1;
unsigned int     n_y:3;
unsigned int     p_c_en:1;
unsigned int     p_c:3;
unsigned int     p_y_en:1;
unsigned int     p_y:3;
}vo_V1_DI_3F_RR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_V1_MA_FR;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_V1_MA_FW;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     index:7;
}vo_V1_MA_MV;

typedef struct 
{
unsigned int     reserved_0:25;
unsigned int     index:7;
}vo_V1_MA_SAD;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     c_range:1;
unsigned int     y_range:2;
unsigned int     reserved_1:3;
unsigned int     th_pos:5;
unsigned int     reserved_2:1;
unsigned int     th_prediff:7;
unsigned int     reserved_3:2;
unsigned int     th_refdiff:6;
}vo_V1_INTRA;

typedef struct 
{
unsigned int     split:1;
unsigned int     c_intra:1;
unsigned int     lp:1;
unsigned int     fm:1;
unsigned int     reserved_0:1;
unsigned int     sc:3;
unsigned int     th_noise:8;
unsigned int     th_c:8;
unsigned int     th_y:8;
}vo_V1_MA;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     correct:1;
unsigned int     fm:1;
unsigned int     limit:1;
unsigned int     weave:1;
unsigned int     blend:1;
unsigned int     noisy_high:1;
unsigned int     noisy:1;
unsigned int     ma:1;
unsigned int     write_data:1;
}vo_V1_MA_NEW;

typedef struct 
{
unsigned int     reserved_0:28;
unsigned int     mode:1;
unsigned int     correct:1;
unsigned int     en:1;
unsigned int     write_data:1;
}vo_V1_MA_REF;

typedef struct 
{
unsigned int     m1d1:8;
unsigned int     m1d0:8;
unsigned int     m0d1:8;
unsigned int     m0d0:8;
}vo_V1_MA_TH;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     limit:8;
unsigned int     big:8;
unsigned int     th:8;
}vo_V1_MA_FT;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     noisy:8;
unsigned int     th:8;
}vo_V1_MA_FM;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     cth_log2:3;
unsigned int     cth_low:8;
unsigned int     reserved_1:5;
unsigned int     yth_log2:3;
unsigned int     yth_low:8;
}vo_V1_DI_BLEND;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     mode:2;
}vo_V1_FILM;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     th:8;
unsigned int     reserved_1:4;
unsigned int     wad:12;
}vo_V1_WADTB;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     e:11;
unsigned int     reserved_1:5;
unsigned int     s:11;
}vo_V1_DI_YWIN;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     e:11;
unsigned int     reserved_1:5;
unsigned int     s:11;
}vo_V1_DI_CWIN;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     e:11;
unsigned int     reserved_1:5;
unsigned int     s:11;
}vo_V1_DI_WINX;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     swad:25;
}vo_V1_FILM_SWAD;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     th:25;
}vo_V1_FILM_TH;

typedef struct 
{
unsigned int     reserved_0:29;
unsigned int     mode:3;
}vo_V1_DI_DBG;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     cnt:21;
}vo_V1_STA_TOTAL;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     sum:29;
}vo_V1_STA_SUM;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     cnt:21;
}vo_V1_STA_BKT;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     sum:29;
}vo_V1_STA_AD;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     offset:12;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_V1_VSYI;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     offset:12;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_V1_VSCI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_V1_VSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V1_VSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V1_VSCC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_V1_HSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_V1_HSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V1_HSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V1_HSCC;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_V1_CC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_V1_CC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_V1_CC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_V1_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_V1_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_V1_CC6;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     reserved_1:1;
unsigned int     st:1;
unsigned int     reserved_2:1;
unsigned int     reserved_3:1;
unsigned int     reserved_4:1;
unsigned int     reserved_5:1;
unsigned int     f422:1;
unsigned int     vs:1;
unsigned int     vs_near:1;
unsigned int     vs_yodd:1;
unsigned int     vs_codd:1;
unsigned int     hs:1;
unsigned int     hs_yodd:1;
unsigned int     hs_codd:1;
unsigned int     topfield:1;
unsigned int     dmaweave:1;
unsigned int     write_data:1;
}vo_V2;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     w_c:7;
unsigned int     reserved_1:1;
unsigned int     w_y:7;
unsigned int     reserved_2:1;
unsigned int     c:7;
unsigned int     reserved_3:1;
unsigned int     y:7;
}vo_V2_INDEX;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     w_c_en:1;
unsigned int     w_c:3;
unsigned int     w_y_en:1;
unsigned int     w_y:3;
unsigned int     c_en:1;
unsigned int     c:3;
unsigned int     y_en:1;
unsigned int     y:3;
}vo_V2_INDEX_RR;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     check_last:1;
unsigned int     en:1;
unsigned int     r:8;
unsigned int     e:8;
unsigned int     s:8;
}vo_V2_LKEY;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     a:9;
}vo_V2_ALPHA;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     w:11;
unsigned int     reserved_1:5;
unsigned int     h:11;
}vo_V2_SIZE;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     th:9;
}vo_V2_BUF;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     offset:12;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_V2_VSYI;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     offset:12;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_V2_VSCI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_V2_VSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V2_VSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V2_VSCC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_V2_HSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_V2_HSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V2_HSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_V2_HSCC;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_V2_CC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_V2_CC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_V2_CC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_V2_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_V2_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_V2_CC6;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     en:1;           /*sub-picture enable */
unsigned int     d3_dbh:1;
unsigned int     d3_dbw:1;
unsigned int     d3_dup:1;
unsigned int     reserved_1:1;
unsigned int     reserved_2:1;
unsigned int     dvd:1;
unsigned int     hddvd:1;
unsigned int     reserved_3:1;
unsigned int     reserved_4:1;
unsigned int     hl:1;
unsigned int     reserved_5:1;
unsigned int     dvb:1;
unsigned int     osd:1;
unsigned int     fetch_1080i:1;
unsigned int     odd_1080i:1;
unsigned int     reserved_6:1;
unsigned int     reserved_7:1;
unsigned int     reserved_8:1;
unsigned int     vs:1;
unsigned int     vs_near:1;
unsigned int     vs_yodd:1;
unsigned int     vs_codd:1;
unsigned int     hs:1;
unsigned int     hs_yodd:1;
unsigned int     hs_codd:1;
unsigned int     chroma1:1;
unsigned int     chroma0:1;
unsigned int     reserved_9:1;
unsigned int     write_data:1;
}vo_SUB1;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     ref:2;
unsigned int     reserved_1:1;
unsigned int     pos:11;
unsigned int     reserved_2:1;
unsigned int     th:11;
}vo_SUB1_START;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     w:12;
}vo_SUB1_SUBP;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     w:11;
unsigned int     reserved_1:1;
unsigned int     h:11;
}vo_SUB1_SIZE;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     th:9;
}vo_SUB1_BUF;

typedef struct 
{
unsigned int     endian:1;
unsigned int     a:31;
}vo_SUB1_WI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_SUB1_WI_3D;

typedef struct 
{
unsigned int     keep:1;
unsigned int     a:31;
}vo_SUB1_CLUT;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_SUB1_PXDT;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_SUB1_PXDB;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     num:11;
}vo_SUB1_LC;

typedef struct 
{
unsigned int     a3:4;
unsigned int     a2:4;
unsigned int     a1:4;
unsigned int     a0:4;
unsigned int     i3:4;
unsigned int     i2:4;
unsigned int     i1:4;
unsigned int     i0:4;
}vo_SUB1_DFT;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     e:11;
unsigned int     reserved_1:1;
unsigned int     s:11;
}vo_SUB1_HLX;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     e:11;
unsigned int     reserved_1:1;
unsigned int     s:11;
}vo_SUB1_HLY;

typedef struct 
{
unsigned int     a3:4;
unsigned int     a2:4;
unsigned int     a1:4;
unsigned int     a0:4;
unsigned int     i3:4;
unsigned int     i2:4;
unsigned int     i1:4;
unsigned int     i0:4;
}vo_SUB1_HL;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_SUB1_CC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_SUB1_CC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_SUB1_CC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_SUB1_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_SUB1_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_SUB1_CC6;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_SUB1_VSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_SUB1_VSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_SUB1_VSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_SUB1_VSCC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_SUB1_HSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_SUB1_HSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_SUB1_HSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_SUB1_HSCC;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     d3_dbh:1;
unsigned int     d3_dbw:1;
unsigned int     d3_dup:1;
unsigned int     reserved_1:6;
unsigned int     reserved_2:1;
unsigned int     reserved_3:1;
unsigned int     dvb:1;
unsigned int     osd:1;
unsigned int     fetch_1080i:1;
unsigned int     odd_1080i:1;
unsigned int     reserved_4:1;
unsigned int     reserved_5:1;
unsigned int     reserved_6:1;
unsigned int     vs:1;
unsigned int     vs_near:1;
unsigned int     vs_yodd:1;
unsigned int     vs_codd:1;
unsigned int     hs:1;
unsigned int     hs_yodd:1;
unsigned int     hs_codd:1;
unsigned int     chroma1:1;
unsigned int     chroma0:1;
unsigned int     reserved_7:1;
unsigned int     write_data:1;
}vo_OSD1;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     ref:2;
unsigned int     reserved_1:1;
unsigned int     pos:11;
unsigned int     reserved_2:1;
unsigned int     th:11;
}vo_OSD1_START;

typedef struct 
{
unsigned int     endian:1;
unsigned int     a:31;
}vo_OSD1_WI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_OSD1_WI_3D;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     w:11;
unsigned int     reserved_1:1;
unsigned int     h:11;
}vo_OSD1_SIZE;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     th:9;
}vo_OSD1_BUF;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_OSD1_CC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_OSD1_CC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_OSD1_CC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_OSD1_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_OSD1_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_OSD1_CC6;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_OSD1_VSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_OSD1_VSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD1_VSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD1_VSCC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_OSD1_HSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_OSD1_HSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD1_HSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD1_HSCC;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     d3_dbh:1;
unsigned int     d3_dbw:1;
unsigned int     d3_dup:1;
unsigned int     reserved_1:4;
unsigned int     o422:1;
unsigned int     reserved_2:1;
unsigned int     reserved_3:1;
unsigned int     reserved_4:1;
unsigned int     dvb:1;
unsigned int     osd:1;
unsigned int     fetch_1080i:1;
unsigned int     odd_1080i:1;
unsigned int     reserved_5:1;
unsigned int     reserved_6:1;
unsigned int     reserved_7:1;
unsigned int     vs:1;
unsigned int     vs_near:1;
unsigned int     vs_yodd:1;
unsigned int     vs_codd:1;
unsigned int     hs:1;
unsigned int     hs_yodd:1;
unsigned int     hs_codd:1;
unsigned int     chroma1:1;
unsigned int     chroma0:1;
unsigned int     reserved_8:1;
unsigned int     write_data:1;
}vo_OSD2;

typedef struct 
{
unsigned int     reserved_0:6;
unsigned int     ref:2;
unsigned int     reserved_1:1;
unsigned int     pos:11;
unsigned int     reserved_2:1;
unsigned int     th:11;
}vo_OSD2_START;

typedef struct 
{
unsigned int     endian:1;
unsigned int     a:31;
}vo_OSD2_WI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_OSD2_WI_3D;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     w:11;
unsigned int     reserved_1:5;
unsigned int     h:11;
}vo_OSD2_SIZE;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     th:9;
}vo_OSD2_BUF;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_OSD2_CC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_OSD2_CC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_OSD2_CC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_OSD2_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_OSD2_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_OSD2_CC6;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_OSD2_VSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_OSD2_VSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD2_VSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD2_VSCC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_OSD2_HSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_OSD2_HSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD2_HSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_OSD2_HSCC;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     d3_dbh:1;
unsigned int     d3_dbw:1;
unsigned int     reserved_1:1;
unsigned int     fetch_1080i:1;
unsigned int     odd_1080i:1;
unsigned int     reserved_2:1;
unsigned int     m4bit:1;
unsigned int     m2bit:1;
unsigned int     reload:1;
unsigned int     update:1;
unsigned int     reserved_3:1;
unsigned int     reserved_4:1;
unsigned int     write_data:1;
}vo_CSR;

typedef struct 
{
unsigned int     reserved_0:11;
unsigned int     w:9;
unsigned int     reserved_1:3;
unsigned int     h:9;
}vo_CSR_SIZE;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_CSR_BS;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_CSR_BS_3D;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_CSR_CLUT;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     d3_dbh:1;
unsigned int     d3_dbw:1;
unsigned int     d3_dup:1;
unsigned int     gfx:1;
unsigned int     f420:1;
unsigned int     reserved_1:1;
unsigned int     skip_v1:1;
unsigned int     write_data:1;
}vo_PIC;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     en:1;
unsigned int     e:8;
unsigned int     s:8;
}vo_PIC_LKEY;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     c:7;
unsigned int     reserved_1:1;
unsigned int     y:7;
}vo_PIC_INDEX;

typedef struct 
{
unsigned int     reserved_0:17;
unsigned int     c:7;
unsigned int     reserved_1:1;
unsigned int     y:7;
}vo_PIC_INDEX_3D;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     sa:31;
}vo_PIC_SEQ;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     sa:31;
}vo_PIC_SEQ_3D;

typedef struct 
{
unsigned int     endian:1;
unsigned int     format:3;
unsigned int     reserved_0:2;
unsigned int     gfx_alpha:1;
unsigned int     extend:1;
unsigned int     reserved_1:8;
unsigned int     pitch:16;
}vo_PIC_GFX;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m01:12;
unsigned int     m00:12;
}vo_PIC_GC1;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m10:12;
unsigned int     m02:12;
}vo_PIC_GC2;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m12:12;
unsigned int     m11:12;
}vo_PIC_GC3;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     m21:12;
unsigned int     m20:12;
}vo_PIC_GC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_PIC_GC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_PIC_GC6;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     w:11;
unsigned int     reserved_1:1;
unsigned int     h:11;
}vo_PIC_SIZE;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     a:9;
}vo_PIC_ALPHA;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     x:11;
unsigned int     reserved_1:1;
unsigned int     y:11;
}vo_PIC_I;

typedef struct 
{
unsigned int     reserved_0:15;
unsigned int     h2v:1;
unsigned int     reserved_1:2;
unsigned int     af:1;
unsigned int     hd:1;
unsigned int     dd:1;
unsigned int     f422:1;
unsigned int     wb:1;
unsigned int     reint:1;
unsigned int     bot:1;
unsigned int     vs:1;
unsigned int     vs_yodd:1;
unsigned int     vs_codd:1;
unsigned int     hs:1;
unsigned int     hs_yodd:1;
unsigned int     hs_codd:1;
unsigned int     write_data:1;
}vo_DDWB;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     write_en5:1;
unsigned int     interlace:1;
unsigned int     write_en4:1;
unsigned int     bot:1;
unsigned int     reserved_1:8;
unsigned int     write_en2:1;
unsigned int     c:7;
unsigned int     write_en1:1;
unsigned int     y:7;
}vo_DD_INDEX;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     write_en5:1;
unsigned int     interlace:1;
unsigned int     write_en4:1;
unsigned int     bot:1;
unsigned int     reserved_1:8;
unsigned int     write_en2:1;
unsigned int     c:7;
unsigned int     write_en1:1;
unsigned int     y:7;
}vo_WB_INDEX;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     w:11;
unsigned int     reserved_1:1;
unsigned int     h:11;
}vo_DD_SIZE;

typedef struct 
{
unsigned int     reserved_0:9;
unsigned int     w:11;
unsigned int     reserved_1:1;
unsigned int     h:11;
}vo_WB_SIZE;

typedef struct 
{
unsigned int     reserved_0:24;
unsigned int     num:8;
}vo_WB_TPC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_DDWB_VSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_DDWB_VSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_DDWB_VSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_DDWB_VSCC;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     offset:11;
unsigned int     phase:14;
unsigned int     reserved_1:2;
}vo_DDWB_HSI;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     out:11;
unsigned int     reserved_1:1;
unsigned int     delta:19;
}vo_DDWB_HSD;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_DDWB_HSYC;

typedef struct 
{
unsigned int     reserved_0:2;
unsigned int     c1:14;
unsigned int     reserved_1:2;
unsigned int     c0:14;
}vo_DDWB_HSCC;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_VBI1;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     l:18;
}vo_VBI1_SIZE;

typedef struct 
{
unsigned int     reserved_0:1;
unsigned int     a:31;
}vo_VBI2;

typedef struct 
{
unsigned int     reserved_0:14;
unsigned int     l:18;
}vo_VBI2_SIZE;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     write_en4:1;
unsigned int     num_req_4:1;
unsigned int     write_en3:1;
unsigned int     dyn_id:1;
unsigned int     write_en2:1;
unsigned int     ref_mix:1;
unsigned int     reserved_1:2;
unsigned int     write_en1:1;
unsigned int     l:5;
}vo_DMA;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     wb:4;
unsigned int     vbi:4;
unsigned int     pic:4;
unsigned int     dd:4;
unsigned int     v2:4;
unsigned int     v1:4;
}vo_DMA_PV;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     csr:4;
unsigned int     osd2:4;
unsigned int     osd1:4;
unsigned int     sub1:4;
}vo_DMA_PG;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     write_en3:1;
unsigned int     init:8;
unsigned int     reserved_1:3;
unsigned int     write_en2:1;
unsigned int     interval:8;
unsigned int     reserved_2:2;
unsigned int     write_en1:1;
unsigned int     num:5;
}vo_DMA_BWC;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     update:1;
unsigned int     write_data:1;
}vo_DMA_UPDATE;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_Y0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_Y1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_Y2;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_Y3;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_C0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_C1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_C2;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1_C3;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V2_Y0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V2_Y1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V2_C0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V2_C1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_SUB1_WI;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_SUB1_D0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_SUB1_D1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_OSD1_WI;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_OSD1_D0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_OSD1_D1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_OSD2_WI;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_OSD2_D0;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_OSD2_D1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_CSR;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_PIC_Y;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_PIC_C;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_DD_Y;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_DD_C;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1DI;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1MV;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_V1SAD;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_VBI1;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     sram:1;
unsigned int     reserved_1:2;
unsigned int     depth:10;
unsigned int     reserved_2:2;
unsigned int     sa:10;
}vo_DMA_VBI2;

typedef struct 
{
unsigned int     reserved_0:13;
unsigned int     depth:7;
unsigned int     reserved_1:5;
unsigned int     sa:7;
}vo_DMA_WB_Y;

typedef struct 
{
unsigned int     reserved_0:13;
unsigned int     depth:7;
unsigned int     reserved_1:5;
unsigned int     sa:7;
}vo_DMA_WB_C;

typedef struct 
{
unsigned int     reserved_0:13;
unsigned int     depth:7;
unsigned int     reserved_1:5;
unsigned int     sa:7;
}vo_DMA_WB_V1;

typedef struct 
{
unsigned int     csr:4;
unsigned int     osd2:4;
unsigned int     osd1:4;
unsigned int     v2:4;
unsigned int     sub1:4;
unsigned int     v1:4;
unsigned int     reserved_0:4;
unsigned int     pic:4;
}vo_MIX_ORDER;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     cd:1;
unsigned int     cu:1;
unsigned int     v2b:1;
unsigned int     v2bg:1;
unsigned int     v1b:1;
unsigned int     write_data:1;
}vo_MIX;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     cr:8;
unsigned int     cb:8;
unsigned int     y:8;
}vo_M1BG;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     w:13;
unsigned int     reserved_1:3;
unsigned int     h:13;
}vo_M1_SIZE;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     cr:8;
unsigned int     cb:8;
unsigned int     y:8;
}vo_M2BG;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     w:13;
unsigned int     reserved_1:3;
unsigned int     h:13;
}vo_M2_SIZE;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_V1_XY;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     cr:8;
unsigned int     cb:8;
unsigned int     y:8;
}vo_V1B_C;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     e:13;
unsigned int     reserved_1:3;
unsigned int     s:13;
}vo_V1B_X;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     e:13;
unsigned int     reserved_1:3;
unsigned int     s:13;
}vo_V1B_Y;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     dir_r:1;
unsigned int     offset_r:11;
unsigned int     reserved_1:4;
unsigned int     dir_l:1;
unsigned int     offset_l:11;
}vo_V1_3D;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_V2_XY;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     cr:8;
unsigned int     cb:8;
unsigned int     y:8;
}vo_V2B_C;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     e:13;
unsigned int     reserved_1:3;
unsigned int     s:13;
}vo_V2B_X;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     e:13;
unsigned int     reserved_1:3;
unsigned int     s:13;
}vo_V2B_Y;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     cr:8;
unsigned int     cb:8;
unsigned int     y:8;
}vo_V2BG_C;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     e:13;
unsigned int     reserved_1:3;
unsigned int     s:13;
}vo_V2BG_X;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     e:13;
unsigned int     reserved_1:3;
unsigned int     s:13;
}vo_V2BG_Y;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_SUB1_XY;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     dir_r:1;
unsigned int     offset_r:11;
unsigned int     reserved_1:4;
unsigned int     dir_l:1;
unsigned int     offset_l:11;
}vo_SUB1_3D;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_OSD1_XY;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     dir_r:1;
unsigned int     offset_r:11;
unsigned int     reserved_1:4;
unsigned int     dir_l:1;
unsigned int     offset_l:11;
}vo_OSD1_3D;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_OSD2_XY;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     dir_r:1;
unsigned int     offset_r:11;
unsigned int     reserved_1:4;
unsigned int     dir_l:1;
unsigned int     offset_l:11;
}vo_OSD2_3D;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_CSR_XY;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     dir_r:1;
unsigned int     offset_r:11;
unsigned int     reserved_1:4;
unsigned int     dir_l:1;
unsigned int     offset_l:11;
}vo_CSR_3D;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_PIC_XY;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     dir_r:1;
unsigned int     offset_r:11;
unsigned int     reserved_1:4;
unsigned int     dir_l:1;
unsigned int     offset_l:11;
}vo_PIC_3D;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     x:13;
unsigned int     reserved_1:3;
unsigned int     y:13;
}vo_DD_XY;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     write_en2:1;
unsigned int     th:11;
unsigned int     reserved_1:1;
unsigned int     write_en1:1;
unsigned int     b:10;
}vo_CH1_BUF;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     write_en2:1;
unsigned int     th:11;
unsigned int     reserved_1:1;
unsigned int     write_en1:1;
unsigned int     b:10;
}vo_CHI_BUF;

typedef struct 
{
unsigned int     reserved_0:8;
unsigned int     write_en2:1;
unsigned int     th:11;
unsigned int     reserved_1:1;
unsigned int     write_en1:1;
unsigned int     b:10;
}vo_CH2_BUF;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     itop:1;
unsigned int     ireint:1;
unsigned int     reserved_1:1;
unsigned int     top:1;
unsigned int     reint:1;
unsigned int     write_data:1;
}vo_CH1;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     itop:1;
unsigned int     ireint:1;
unsigned int     reserved_1:1;
unsigned int     top:1;
unsigned int     reint:1;
unsigned int     write_data:1;
}vo_CH2;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m01:12;
unsigned int     reserved_1:4;
unsigned int     m00:12;
}vo_PORT_P_CC1;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m10:12;
unsigned int     reserved_1:4;
unsigned int     m02:12;
}vo_PORT_P_CC2;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m12:12;
unsigned int     reserved_1:4;
unsigned int     m11:12;
}vo_PORT_P_CC3;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m21:12;
unsigned int     reserved_1:4;
unsigned int     m20:12;
}vo_PORT_P_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_PORT_P_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_PORT_P_CC6;

typedef struct 
{
unsigned int     Cmax:8;
unsigned int     Cmin:8;
unsigned int     Ymax:8;
unsigned int     Ymin:8;
}vo_PORT_P_LMT;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m01:12;
unsigned int     reserved_1:4;
unsigned int     m00:12;
}vo_PORT_I_CC1;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m10:12;
unsigned int     reserved_1:4;
unsigned int     m02:12;
}vo_PORT_I_CC2;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m12:12;
unsigned int     reserved_1:4;
unsigned int     m11:12;
}vo_PORT_I_CC3;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     m21:12;
unsigned int     reserved_1:4;
unsigned int     m20:12;
}vo_PORT_I_CC4;

typedef struct 
{
unsigned int     reserved_0:20;
unsigned int     m22:12;
}vo_PORT_I_CC5;

typedef struct 
{
unsigned int     reserved_0:5;
unsigned int     a2:9;
unsigned int     a1:9;
unsigned int     a0:9;
}vo_PORT_I_CC6;

typedef struct 
{
unsigned int     Cmax:8;
unsigned int     Cmin:8;
unsigned int     Ymax:8;
unsigned int     Ymin:8;
}vo_PORT_I_LMT;

typedef struct 
{
unsigned int     m01:16;
unsigned int     m00:16;
}vo_PORT_H_CC1;

typedef struct 
{
unsigned int     m10:16;
unsigned int     m02:16;
}vo_PORT_H_CC2;

typedef struct 
{
unsigned int     m12:16;
unsigned int     m11:16;
}vo_PORT_H_CC3;

typedef struct 
{
unsigned int     m21:16;
unsigned int     m20:16;
}vo_PORT_H_CC4;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     m22:16;
}vo_PORT_H_CC5;

typedef struct 
{
unsigned int     reserved_0:3;
unsigned int     a1:13;
unsigned int     reserved_1:3;
unsigned int     a0:13;
}vo_PORT_H_CC6;

typedef struct 
{
unsigned int     reserved_0:19;
unsigned int     a2:13;
}vo_PORT_H_CC7;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     Ymax:12;
unsigned int     reserved_1:4;
unsigned int     Ymin:12;
}vo_PORT_H_LMT_Y;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     Cmax:12;
unsigned int     reserved_1:4;
unsigned int     Cmin:12;
}vo_PORT_H_LMT_C;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     reserved_1:1;
unsigned int     reserved_2:1;
unsigned int     reserved_3:1;
unsigned int     h_under:1;
unsigned int     i_under:1;
unsigned int     p_under:1;
unsigned int     sub1:1;
unsigned int     wbfin:1;
unsigned int     m2fin:1;
unsigned int     m1fin:1;
unsigned int     write_data:1;
}vo_INTEN;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     reserved_1:1;
unsigned int     reserved_2:1;
unsigned int     reserved_3:1;
unsigned int     h_under:1;
unsigned int     i_under:1;
unsigned int     p_under:1;
unsigned int     sub1:1;
unsigned int     wbfin:1;
unsigned int     m2fin:1;
unsigned int     m1fin:1;
unsigned int     write_data:1;
}vo_INTST;

typedef struct 
{
unsigned int     reserved_0:18;
unsigned int     tve_h_fetch:1;
unsigned int     tve_i_fetch:1;
unsigned int     tve_p_fetch:1;
unsigned int     pic:1;
unsigned int     dd:1;
unsigned int     csr:1;
unsigned int     osd2:1;
unsigned int     osd1:1;
unsigned int     sub1:1;
unsigned int     v2:1;
unsigned int     v1:1;
unsigned int     wb:1;
unsigned int     mix2:1;
unsigned int     mix1:1;
}vo_STATUS;

typedef struct 
{
unsigned int     reserved_0:4;
unsigned int     wb:1;
unsigned int     csr:1;
unsigned int     pic:1;
unsigned int     dd:1;
unsigned int     osd2:1;
unsigned int     osd1:1;
unsigned int     sub1:1;
unsigned int     v2:1;
unsigned int     v1:1;
unsigned int     ch2:1;
unsigned int     ch1_i:1;
unsigned int     ch1_p:1;
unsigned int     reserved_1:5;
unsigned int     th:11;
}vo_MON_SETUP;

typedef struct 
{
unsigned int     delay:16;
unsigned int     reserved_0:7;
unsigned int     wb:1;
unsigned int     csr:1;
unsigned int     pic:1;
unsigned int     dd:1;
unsigned int     osd2:1;
unsigned int     osd1:1;
unsigned int     sub1:1;
unsigned int     v2:1;
unsigned int     v1:1;
}vo_MON_BUF;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     not_full:11;
}vo_MON_UF;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     y:11;
}vo_MON_UF_P;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     y:11;
}vo_MON_UF_I;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     y:11;
}vo_MON_UF_H;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}vo_MON_UF_V1;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}vo_MON_UF_V2;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}vo_MON_UF_SUB1;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}vo_MON_UF_OSD1;

typedef struct 
{
unsigned int     reserved_0:23;
unsigned int     cnt:9;
}vo_MON_UF_OSD2;

typedef struct 
{
unsigned int     reserved_0:7;
unsigned int     write_en3:1;
unsigned int     sel1:8;
unsigned int     reserved_1:3;
unsigned int     write_en2:1;
unsigned int     sel0:8;
unsigned int     reserved_2:2;
unsigned int     write_en1:1;
unsigned int     en:1;
}vo_DBG;

typedef struct 
{
unsigned int     reserved_0:26;
unsigned int     ramp_4:1;
unsigned int     ramp_2:1;
unsigned int     ramp_1:1;
unsigned int     go:1;
unsigned int     en:1;
unsigned int     write_data:1;
}vo_DACT;

typedef struct 
{
unsigned int     reserved_0:21;
unsigned int     l:11;
}vo_DACT_L;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     a:10;
}vo_DACT_R;

typedef struct 
{
unsigned int     reserved_0:22;
unsigned int     a:10;
}vo_DACT_A;

typedef struct 
{
unsigned int     reserved_0:16;
unsigned int     d:16;
}vo_DACT_D;

typedef struct 
{
unsigned int     reserved_0:30;
unsigned int     en:1;
unsigned int     write_data:1;
}vo_RBRAM;

typedef struct 
{
unsigned int     mode:32;
}vo_VERF;

#endif

#define VO_MODE                                                                      0x18005000
#define VO_MODE_reg_addr                                                             "0xB8005000"
#define VO_MODE_reg                                                                  0xB8005000
#define set_VO_MODE_reg(data)   (*((volatile unsigned int*) VO_MODE_reg)=data)
#define get_VO_MODE_reg   (*((volatile unsigned int*) VO_MODE_reg))
#define VO_MODE_inst_adr                                                             "0x0000"
#define VO_MODE_inst                                                                 0x0000
#define VO_MODE_ch2i_shift                                                           (4)
#define VO_MODE_ch2i_mask                                                            (0x00000010)
#define VO_MODE_ch2i(data)                                                           (0x00000010&((data)<<4))
#define VO_MODE_ch2i_src(data)                                                       ((0x00000010&(data))>>4)
#define VO_MODE_get_ch2i(data)                                                       ((0x00000010&(data))>>4)
#define VO_MODE_ch2_shift                                                            (3)
#define VO_MODE_ch2_mask                                                             (0x00000008)
#define VO_MODE_ch2(data)                                                            (0x00000008&((data)<<3))
#define VO_MODE_ch2_src(data)                                                        ((0x00000008&(data))>>3)
#define VO_MODE_get_ch2(data)                                                        ((0x00000008&(data))>>3)
#define VO_MODE_ch1i_shift                                                           (2)
#define VO_MODE_ch1i_mask                                                            (0x00000004)
#define VO_MODE_ch1i(data)                                                           (0x00000004&((data)<<2))
#define VO_MODE_ch1i_src(data)                                                       ((0x00000004&(data))>>2)
#define VO_MODE_get_ch1i(data)                                                       ((0x00000004&(data))>>2)
#define VO_MODE_ch1_shift                                                            (1)
#define VO_MODE_ch1_mask                                                             (0x00000002)
#define VO_MODE_ch1(data)                                                            (0x00000002&((data)<<1))
#define VO_MODE_ch1_src(data)                                                        ((0x00000002&(data))>>1)
#define VO_MODE_get_ch1(data)                                                        ((0x00000002&(data))>>1)
#define VO_MODE_write_data_shift                                                     (0)
#define VO_MODE_write_data_mask                                                      (0x00000001)
#define VO_MODE_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_MODE_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_MODE_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_OUT                                                                       0x18005004
#define VO_OUT_reg_addr                                                              "0xB8005004"
#define VO_OUT_reg                                                                   0xB8005004
#define set_VO_OUT_reg(data)   (*((volatile unsigned int*) VO_OUT_reg)=data)
#define get_VO_OUT_reg   (*((volatile unsigned int*) VO_OUT_reg))
#define VO_OUT_inst_adr                                                              "0x0001"
#define VO_OUT_inst                                                                  0x0001
#define VO_OUT_write_en3_shift                                                       (10)
#define VO_OUT_write_en3_mask                                                        (0x00000400)
#define VO_OUT_write_en3(data)                                                       (0x00000400&((data)<<10))
#define VO_OUT_write_en3_src(data)                                                   ((0x00000400&(data))>>10)
#define VO_OUT_get_write_en3(data)                                                   ((0x00000400&(data))>>10)
#define VO_OUT_h_shift                                                               (8)
#define VO_OUT_h_mask                                                                (0x00000300)
#define VO_OUT_h(data)                                                               (0x00000300&((data)<<8))
#define VO_OUT_h_src(data)                                                           ((0x00000300&(data))>>8)
#define VO_OUT_get_h(data)                                                           ((0x00000300&(data))>>8)
#define VO_OUT_write_en2_shift                                                       (6)
#define VO_OUT_write_en2_mask                                                        (0x00000040)
#define VO_OUT_write_en2(data)                                                       (0x00000040&((data)<<6))
#define VO_OUT_write_en2_src(data)                                                   ((0x00000040&(data))>>6)
#define VO_OUT_get_write_en2(data)                                                   ((0x00000040&(data))>>6)
#define VO_OUT_i_shift                                                               (4)
#define VO_OUT_i_mask                                                                (0x00000030)
#define VO_OUT_i(data)                                                               (0x00000030&((data)<<4))
#define VO_OUT_i_src(data)                                                           ((0x00000030&(data))>>4)
#define VO_OUT_get_i(data)                                                           ((0x00000030&(data))>>4)
#define VO_OUT_write_en1_shift                                                       (2)
#define VO_OUT_write_en1_mask                                                        (0x00000004)
#define VO_OUT_write_en1(data)                                                       (0x00000004&((data)<<2))
#define VO_OUT_write_en1_src(data)                                                   ((0x00000004&(data))>>2)
#define VO_OUT_get_write_en1(data)                                                   ((0x00000004&(data))>>2)
#define VO_OUT_p_shift                                                               (0)
#define VO_OUT_p_mask                                                                (0x00000003)
#define VO_OUT_p(data)                                                               (0x00000003&((data)<<0))
#define VO_OUT_p_src(data)                                                           ((0x00000003&(data))>>0)
#define VO_OUT_get_p(data)                                                           ((0x00000003&(data))>>0)


#define VO_MIX1                                                                      0x18005008
#define VO_MIX1_reg_addr                                                             "0xB8005008"
#define VO_MIX1_reg                                                                  0xB8005008
#define set_VO_MIX1_reg(data)   (*((volatile unsigned int*) VO_MIX1_reg)=data)
#define get_VO_MIX1_reg   (*((volatile unsigned int*) VO_MIX1_reg))
#define VO_MIX1_inst_adr                                                             "0x0002"
#define VO_MIX1_inst                                                                 0x0002
#define VO_MIX1_i_vbi_shift                                                          (11)
#define VO_MIX1_i_vbi_mask                                                           (0x00000800)
#define VO_MIX1_i_vbi(data)                                                          (0x00000800&((data)<<11))
#define VO_MIX1_i_vbi_src(data)                                                      ((0x00000800&(data))>>11)
#define VO_MIX1_get_i_vbi(data)                                                      ((0x00000800&(data))>>11)
#define VO_MIX1_vbi_shift                                                            (10)
#define VO_MIX1_vbi_mask                                                             (0x00000400)
#define VO_MIX1_vbi(data)                                                            (0x00000400&((data)<<10))
#define VO_MIX1_vbi_src(data)                                                        ((0x00000400&(data))>>10)
#define VO_MIX1_get_vbi(data)                                                        ((0x00000400&(data))>>10)
#define VO_MIX1_csr_shift                                                            (9)
#define VO_MIX1_csr_mask                                                             (0x00000200)
#define VO_MIX1_csr(data)                                                            (0x00000200&((data)<<9))
#define VO_MIX1_csr_src(data)                                                        ((0x00000200&(data))>>9)
#define VO_MIX1_get_csr(data)                                                        ((0x00000200&(data))>>9)
#define VO_MIX1_osd2_shift                                                           (8)
#define VO_MIX1_osd2_mask                                                            (0x00000100)
#define VO_MIX1_osd2(data)                                                           (0x00000100&((data)<<8))
#define VO_MIX1_osd2_src(data)                                                       ((0x00000100&(data))>>8)
#define VO_MIX1_get_osd2(data)                                                       ((0x00000100&(data))>>8)
#define VO_MIX1_osd1_shift                                                           (7)
#define VO_MIX1_osd1_mask                                                            (0x00000080)
#define VO_MIX1_osd1(data)                                                           (0x00000080&((data)<<7))
#define VO_MIX1_osd1_src(data)                                                       ((0x00000080&(data))>>7)
#define VO_MIX1_get_osd1(data)                                                       ((0x00000080&(data))>>7)
#define VO_MIX1_sub1_shift                                                           (6)
#define VO_MIX1_sub1_mask                                                            (0x00000040)
#define VO_MIX1_sub1(data)                                                           (0x00000040&((data)<<6))
#define VO_MIX1_sub1_src(data)                                                       ((0x00000040&(data))>>6)
#define VO_MIX1_get_sub1(data)                                                       ((0x00000040&(data))>>6)
#define VO_MIX1_v2_shift                                                             (5)
#define VO_MIX1_v2_mask                                                              (0x00000020)
#define VO_MIX1_v2(data)                                                             (0x00000020&((data)<<5))
#define VO_MIX1_v2_src(data)                                                         ((0x00000020&(data))>>5)
#define VO_MIX1_get_v2(data)                                                         ((0x00000020&(data))>>5)
#define VO_MIX1_v1_shift                                                             (4)
#define VO_MIX1_v1_mask                                                              (0x00000010)
#define VO_MIX1_v1(data)                                                             (0x00000010&((data)<<4))
#define VO_MIX1_v1_src(data)                                                         ((0x00000010&(data))>>4)
#define VO_MIX1_get_v1(data)                                                         ((0x00000010&(data))>>4)
#define VO_MIX1_pic_shift                                                            (2)
#define VO_MIX1_pic_mask                                                             (0x00000004)
#define VO_MIX1_pic(data)                                                            (0x00000004&((data)<<2))
#define VO_MIX1_pic_src(data)                                                        ((0x00000004&(data))>>2)
#define VO_MIX1_get_pic(data)                                                        ((0x00000004&(data))>>2)
#define VO_MIX1_bg_shift                                                             (1)
#define VO_MIX1_bg_mask                                                              (0x00000002)
#define VO_MIX1_bg(data)                                                             (0x00000002&((data)<<1))
#define VO_MIX1_bg_src(data)                                                         ((0x00000002&(data))>>1)
#define VO_MIX1_get_bg(data)                                                         ((0x00000002&(data))>>1)
#define VO_MIX1_write_data_shift                                                     (0)
#define VO_MIX1_write_data_mask                                                      (0x00000001)
#define VO_MIX1_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_MIX1_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_MIX1_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_MIX2                                                                      0x1800500C
#define VO_MIX2_reg_addr                                                             "0xB800500C"
#define VO_MIX2_reg                                                                  0xB800500C
#define set_VO_MIX2_reg(data)   (*((volatile unsigned int*) VO_MIX2_reg)=data)
#define get_VO_MIX2_reg   (*((volatile unsigned int*) VO_MIX2_reg))
#define VO_MIX2_inst_adr                                                             "0x0003"
#define VO_MIX2_inst                                                                 0x0003
#define VO_MIX2_i_vbi_shift                                                          (11)
#define VO_MIX2_i_vbi_mask                                                           (0x00000800)
#define VO_MIX2_i_vbi(data)                                                          (0x00000800&((data)<<11))
#define VO_MIX2_i_vbi_src(data)                                                      ((0x00000800&(data))>>11)
#define VO_MIX2_get_i_vbi(data)                                                      ((0x00000800&(data))>>11)
#define VO_MIX2_vbi_shift                                                            (10)
#define VO_MIX2_vbi_mask                                                             (0x00000400)
#define VO_MIX2_vbi(data)                                                            (0x00000400&((data)<<10))
#define VO_MIX2_vbi_src(data)                                                        ((0x00000400&(data))>>10)
#define VO_MIX2_get_vbi(data)                                                        ((0x00000400&(data))>>10)
#define VO_MIX2_dd_shift                                                             (3)
#define VO_MIX2_dd_mask                                                              (0x00000008)
#define VO_MIX2_dd(data)                                                             (0x00000008&((data)<<3))
#define VO_MIX2_dd_src(data)                                                         ((0x00000008&(data))>>3)
#define VO_MIX2_get_dd(data)                                                         ((0x00000008&(data))>>3)
#define VO_MIX2_bg_shift                                                             (1)
#define VO_MIX2_bg_mask                                                              (0x00000002)
#define VO_MIX2_bg(data)                                                             (0x00000002&((data)<<1))
#define VO_MIX2_bg_src(data)                                                         ((0x00000002&(data))>>1)
#define VO_MIX2_get_bg(data)                                                         ((0x00000002&(data))>>1)
#define VO_MIX2_write_data_shift                                                     (0)
#define VO_MIX2_write_data_mask                                                      (0x00000001)
#define VO_MIX2_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_MIX2_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_MIX2_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_WB                                                                        0x18005010
#define VO_WB_reg_addr                                                               "0xB8005010"
#define VO_WB_reg                                                                    0xB8005010
#define set_VO_WB_reg(data)   (*((volatile unsigned int*) VO_WB_reg)=data)
#define get_VO_WB_reg   (*((volatile unsigned int*) VO_WB_reg))
#define VO_WB_inst_adr                                                               "0x0004"
#define VO_WB_inst                                                                   0x0004
#define VO_WB_osd2_shift                                                             (7)
#define VO_WB_osd2_mask                                                              (0x00000080)
#define VO_WB_osd2(data)                                                             (0x00000080&((data)<<7))
#define VO_WB_osd2_src(data)                                                         ((0x00000080&(data))>>7)
#define VO_WB_get_osd2(data)                                                         ((0x00000080&(data))>>7)
#define VO_WB_osd1_shift                                                             (6)
#define VO_WB_osd1_mask                                                              (0x00000040)
#define VO_WB_osd1(data)                                                             (0x00000040&((data)<<6))
#define VO_WB_osd1_src(data)                                                         ((0x00000040&(data))>>6)
#define VO_WB_get_osd1(data)                                                         ((0x00000040&(data))>>6)
#define VO_WB_sub1_shift                                                             (5)
#define VO_WB_sub1_mask                                                              (0x00000020)
#define VO_WB_sub1(data)                                                             (0x00000020&((data)<<5))
#define VO_WB_sub1_src(data)                                                         ((0x00000020&(data))>>5)
#define VO_WB_get_sub1(data)                                                         ((0x00000020&(data))>>5)
#define VO_WB_v2_shift                                                               (4)
#define VO_WB_v2_mask                                                                (0x00000010)
#define VO_WB_v2(data)                                                               (0x00000010&((data)<<4))
#define VO_WB_v2_src(data)                                                           ((0x00000010&(data))>>4)
#define VO_WB_get_v2(data)                                                           ((0x00000010&(data))>>4)
#define VO_WB_v1_shift                                                               (3)
#define VO_WB_v1_mask                                                                (0x00000008)
#define VO_WB_v1(data)                                                               (0x00000008&((data)<<3))
#define VO_WB_v1_src(data)                                                           ((0x00000008&(data))>>3)
#define VO_WB_get_v1(data)                                                           ((0x00000008&(data))>>3)
#define VO_WB_mix1_shift                                                             (1)
#define VO_WB_mix1_mask                                                              (0x00000002)
#define VO_WB_mix1(data)                                                             (0x00000002&((data)<<1))
#define VO_WB_mix1_src(data)                                                         ((0x00000002&(data))>>1)
#define VO_WB_get_mix1(data)                                                         ((0x00000002&(data))>>1)
#define VO_WB_write_data_shift                                                       (0)
#define VO_WB_write_data_mask                                                        (0x00000001)
#define VO_WB_write_data(data)                                                       (0x00000001&((data)<<0))
#define VO_WB_write_data_src(data)                                                   ((0x00000001&(data))>>0)
#define VO_WB_get_write_data(data)                                                   ((0x00000001&(data))>>0)


#define VO_3D                                                                        0x18005014
#define VO_3D_reg_addr                                                               "0xB8005014"
#define VO_3D_reg                                                                    0xB8005014
#define set_VO_3D_reg(data)   (*((volatile unsigned int*) VO_3D_reg)=data)
#define get_VO_3D_reg   (*((volatile unsigned int*) VO_3D_reg))
#define VO_3D_inst_adr                                                               "0x0005"
#define VO_3D_inst                                                                   0x0005
#define VO_3D_top_and_bottom_shift                                                   (3)
#define VO_3D_top_and_bottom_mask                                                    (0x00000008)
#define VO_3D_top_and_bottom(data)                                                   (0x00000008&((data)<<3))
#define VO_3D_top_and_bottom_src(data)                                               ((0x00000008&(data))>>3)
#define VO_3D_get_top_and_bottom(data)                                               ((0x00000008&(data))>>3)
#define VO_3D_side_by_side_shift                                                     (2)
#define VO_3D_side_by_side_mask                                                      (0x00000004)
#define VO_3D_side_by_side(data)                                                     (0x00000004&((data)<<2))
#define VO_3D_side_by_side_src(data)                                                 ((0x00000004&(data))>>2)
#define VO_3D_get_side_by_side(data)                                                 ((0x00000004&(data))>>2)
#define VO_3D_right_shift                                                            (1)
#define VO_3D_right_mask                                                             (0x00000002)
#define VO_3D_right(data)                                                            (0x00000002&((data)<<1))
#define VO_3D_right_src(data)                                                        ((0x00000002&(data))>>1)
#define VO_3D_get_right(data)                                                        ((0x00000002&(data))>>1)
#define VO_3D_write_data_shift                                                       (0)
#define VO_3D_write_data_mask                                                        (0x00000001)
#define VO_3D_write_data(data)                                                       (0x00000001&((data)<<0))
#define VO_3D_write_data_src(data)                                                   ((0x00000001&(data))>>0)
#define VO_3D_get_write_data(data)                                                   ((0x00000001&(data))>>0)


#define VO_FC                                                                        0x18005018
#define VO_FC_reg_addr                                                               "0xB8005018"
#define VO_FC_reg                                                                    0xB8005018
#define set_VO_FC_reg(data)   (*((volatile unsigned int*) VO_FC_reg)=data)
#define get_VO_FC_reg   (*((volatile unsigned int*) VO_FC_reg))
#define VO_FC_inst_adr                                                               "0x0006"
#define VO_FC_inst                                                                   0x0006
#define VO_FC_wbgo_shift                                                             (3)
#define VO_FC_wbgo_mask                                                              (0x00000008)
#define VO_FC_wbgo(data)                                                             (0x00000008&((data)<<3))
#define VO_FC_wbgo_src(data)                                                         ((0x00000008&(data))>>3)
#define VO_FC_get_wbgo(data)                                                         ((0x00000008&(data))>>3)
#define VO_FC_m2go_shift                                                             (2)
#define VO_FC_m2go_mask                                                              (0x00000004)
#define VO_FC_m2go(data)                                                             (0x00000004&((data)<<2))
#define VO_FC_m2go_src(data)                                                         ((0x00000004&(data))>>2)
#define VO_FC_get_m2go(data)                                                         ((0x00000004&(data))>>2)
#define VO_FC_m1go_shift                                                             (1)
#define VO_FC_m1go_mask                                                              (0x00000002)
#define VO_FC_m1go(data)                                                             (0x00000002&((data)<<1))
#define VO_FC_m1go_src(data)                                                         ((0x00000002&(data))>>1)
#define VO_FC_get_m1go(data)                                                         ((0x00000002&(data))>>1)
#define VO_FC_write_data_shift                                                       (0)
#define VO_FC_write_data_mask                                                        (0x00000001)
#define VO_FC_write_data(data)                                                       (0x00000001&((data)<<0))
#define VO_FC_write_data_src(data)                                                   ((0x00000001&(data))>>0)
#define VO_FC_get_write_data(data)                                                   ((0x00000001&(data))>>0)


#define VO_VBI_END                                                                   0x1800501C
#define VO_VBI_END_reg_addr                                                          "0xB800501C"
#define VO_VBI_END_reg                                                               0xB800501C
#define set_VO_VBI_END_reg(data)   (*((volatile unsigned int*) VO_VBI_END_reg)=data)
#define get_VO_VBI_END_reg   (*((volatile unsigned int*) VO_VBI_END_reg))
#define VO_VBI_END_inst_adr                                                          "0x0007"
#define VO_VBI_END_inst                                                              0x0007
#define VO_VBI_END_ref_shift                                                         (24)
#define VO_VBI_END_ref_mask                                                          (0x03000000)
#define VO_VBI_END_ref(data)                                                         (0x03000000&((data)<<24))
#define VO_VBI_END_ref_src(data)                                                     ((0x03000000&(data))>>24)
#define VO_VBI_END_get_ref(data)                                                     ((0x03000000&(data))>>24)
#define VO_VBI_END_pos_shift                                                         (12)
#define VO_VBI_END_pos_mask                                                          (0x007FF000)
#define VO_VBI_END_pos(data)                                                         (0x007FF000&((data)<<12))
#define VO_VBI_END_pos_src(data)                                                     ((0x007FF000&(data))>>12)
#define VO_VBI_END_get_pos(data)                                                     ((0x007FF000&(data))>>12)
#define VO_VBI_END_th_shift                                                          (0)
#define VO_VBI_END_th_mask                                                           (0x000007FF)
#define VO_VBI_END_th(data)                                                          (0x000007FF&((data)<<0))
#define VO_VBI_END_th_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_VBI_END_get_th(data)                                                      ((0x000007FF&(data))>>0)


#define VO_V1                                                                        0x18005020
#define VO_V1_reg_addr                                                               "0xB8005020"
#define VO_V1_reg                                                                    0xB8005020
#define set_VO_V1_reg(data)   (*((volatile unsigned int*) VO_V1_reg)=data)
#define get_VO_V1_reg   (*((volatile unsigned int*) VO_V1_reg))
#define VO_V1_inst_adr                                                               "0x0008"
#define VO_V1_inst                                                                   0x0008
#define VO_V1_d3_dbh_shift                                                           (19)
#define VO_V1_d3_dbh_mask                                                            (0x00080000)
#define VO_V1_d3_dbh(data)                                                           (0x00080000&((data)<<19))
#define VO_V1_d3_dbh_src(data)                                                       ((0x00080000&(data))>>19)
#define VO_V1_get_d3_dbh(data)                                                       ((0x00080000&(data))>>19)
#define VO_V1_d3_dbw_shift                                                           (18)
#define VO_V1_d3_dbw_mask                                                            (0x00040000)
#define VO_V1_d3_dbw(data)                                                           (0x00040000&((data)<<18))
#define VO_V1_d3_dbw_src(data)                                                       ((0x00040000&(data))>>18)
#define VO_V1_get_d3_dbw(data)                                                       ((0x00040000&(data))>>18)
#define VO_V1_d3_dup_shift                                                           (17)
#define VO_V1_d3_dup_mask                                                            (0x00020000)
#define VO_V1_d3_dup(data)                                                           (0x00020000&((data)<<17))
#define VO_V1_d3_dup_src(data)                                                       ((0x00020000&(data))>>17)
#define VO_V1_get_d3_dup(data)                                                       ((0x00020000&(data))>>17)
#define VO_V1_st_shift                                                               (15)
#define VO_V1_st_mask                                                                (0x00008000)
#define VO_V1_st(data)                                                               (0x00008000&((data)<<15))
#define VO_V1_st_src(data)                                                           ((0x00008000&(data))>>15)
#define VO_V1_get_st(data)                                                           ((0x00008000&(data))>>15)
#define VO_V1_cd_shift                                                               (13)
#define VO_V1_cd_mask                                                                (0x00002000)
#define VO_V1_cd(data)                                                               (0x00002000&((data)<<13))
#define VO_V1_cd_src(data)                                                           ((0x00002000&(data))>>13)
#define VO_V1_get_cd(data)                                                           ((0x00002000&(data))>>13)
#define VO_V1_gfx_shift                                                              (12)
#define VO_V1_gfx_mask                                                               (0x00001000)
#define VO_V1_gfx(data)                                                              (0x00001000&((data)<<12))
#define VO_V1_gfx_src(data)                                                          ((0x00001000&(data))>>12)
#define VO_V1_get_gfx(data)                                                          ((0x00001000&(data))>>12)
#define VO_V1_di_shift                                                               (11)
#define VO_V1_di_mask                                                                (0x00000800)
#define VO_V1_di(data)                                                               (0x00000800&((data)<<11))
#define VO_V1_di_src(data)                                                           ((0x00000800&(data))>>11)
#define VO_V1_get_di(data)                                                           ((0x00000800&(data))>>11)
#define VO_V1_f422_shift                                                             (10)
#define VO_V1_f422_mask                                                              (0x00000400)
#define VO_V1_f422(data)                                                             (0x00000400&((data)<<10))
#define VO_V1_f422_src(data)                                                         ((0x00000400&(data))>>10)
#define VO_V1_get_f422(data)                                                         ((0x00000400&(data))>>10)
#define VO_V1_vs_shift                                                               (9)
#define VO_V1_vs_mask                                                                (0x00000200)
#define VO_V1_vs(data)                                                               (0x00000200&((data)<<9))
#define VO_V1_vs_src(data)                                                           ((0x00000200&(data))>>9)
#define VO_V1_get_vs(data)                                                           ((0x00000200&(data))>>9)
#define VO_V1_vs_near_shift                                                          (8)
#define VO_V1_vs_near_mask                                                           (0x00000100)
#define VO_V1_vs_near(data)                                                          (0x00000100&((data)<<8))
#define VO_V1_vs_near_src(data)                                                      ((0x00000100&(data))>>8)
#define VO_V1_get_vs_near(data)                                                      ((0x00000100&(data))>>8)
#define VO_V1_vs_yodd_shift                                                          (7)
#define VO_V1_vs_yodd_mask                                                           (0x00000080)
#define VO_V1_vs_yodd(data)                                                          (0x00000080&((data)<<7))
#define VO_V1_vs_yodd_src(data)                                                      ((0x00000080&(data))>>7)
#define VO_V1_get_vs_yodd(data)                                                      ((0x00000080&(data))>>7)
#define VO_V1_vs_codd_shift                                                          (6)
#define VO_V1_vs_codd_mask                                                           (0x00000040)
#define VO_V1_vs_codd(data)                                                          (0x00000040&((data)<<6))
#define VO_V1_vs_codd_src(data)                                                      ((0x00000040&(data))>>6)
#define VO_V1_get_vs_codd(data)                                                      ((0x00000040&(data))>>6)
#define VO_V1_hs_shift                                                               (5)
#define VO_V1_hs_mask                                                                (0x00000020)
#define VO_V1_hs(data)                                                               (0x00000020&((data)<<5))
#define VO_V1_hs_src(data)                                                           ((0x00000020&(data))>>5)
#define VO_V1_get_hs(data)                                                           ((0x00000020&(data))>>5)
#define VO_V1_hs_yodd_shift                                                          (4)
#define VO_V1_hs_yodd_mask                                                           (0x00000010)
#define VO_V1_hs_yodd(data)                                                          (0x00000010&((data)<<4))
#define VO_V1_hs_yodd_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_V1_get_hs_yodd(data)                                                      ((0x00000010&(data))>>4)
#define VO_V1_hs_codd_shift                                                          (3)
#define VO_V1_hs_codd_mask                                                           (0x00000008)
#define VO_V1_hs_codd(data)                                                          (0x00000008&((data)<<3))
#define VO_V1_hs_codd_src(data)                                                      ((0x00000008&(data))>>3)
#define VO_V1_get_hs_codd(data)                                                      ((0x00000008&(data))>>3)
#define VO_V1_topfield_shift                                                         (2)
#define VO_V1_topfield_mask                                                          (0x00000004)
#define VO_V1_topfield(data)                                                         (0x00000004&((data)<<2))
#define VO_V1_topfield_src(data)                                                     ((0x00000004&(data))>>2)
#define VO_V1_get_topfield(data)                                                     ((0x00000004&(data))>>2)
#define VO_V1_dmaweave_shift                                                         (1)
#define VO_V1_dmaweave_mask                                                          (0x00000002)
#define VO_V1_dmaweave(data)                                                         (0x00000002&((data)<<1))
#define VO_V1_dmaweave_src(data)                                                     ((0x00000002&(data))>>1)
#define VO_V1_get_dmaweave(data)                                                     ((0x00000002&(data))>>1)
#define VO_V1_write_data_shift                                                       (0)
#define VO_V1_write_data_mask                                                        (0x00000001)
#define VO_V1_write_data(data)                                                       (0x00000001&((data)<<0))
#define VO_V1_write_data_src(data)                                                   ((0x00000001&(data))>>0)
#define VO_V1_get_write_data(data)                                                   ((0x00000001&(data))>>0)


#define VO_V1_INDEX                                                                  0x18005024
#define VO_V1_INDEX_reg_addr                                                         "0xB8005024"
#define VO_V1_INDEX_reg                                                              0xB8005024
#define set_VO_V1_INDEX_reg(data)   (*((volatile unsigned int*) VO_V1_INDEX_reg)=data)
#define get_VO_V1_INDEX_reg   (*((volatile unsigned int*) VO_V1_INDEX_reg))
#define VO_V1_INDEX_inst_adr                                                         "0x0009"
#define VO_V1_INDEX_inst                                                             0x0009
#define VO_V1_INDEX_w_c_shift                                                        (24)
#define VO_V1_INDEX_w_c_mask                                                         (0x7F000000)
#define VO_V1_INDEX_w_c(data)                                                        (0x7F000000&((data)<<24))
#define VO_V1_INDEX_w_c_src(data)                                                    ((0x7F000000&(data))>>24)
#define VO_V1_INDEX_get_w_c(data)                                                    ((0x7F000000&(data))>>24)
#define VO_V1_INDEX_w_y_shift                                                        (16)
#define VO_V1_INDEX_w_y_mask                                                         (0x007F0000)
#define VO_V1_INDEX_w_y(data)                                                        (0x007F0000&((data)<<16))
#define VO_V1_INDEX_w_y_src(data)                                                    ((0x007F0000&(data))>>16)
#define VO_V1_INDEX_get_w_y(data)                                                    ((0x007F0000&(data))>>16)
#define VO_V1_INDEX_c_shift                                                          (8)
#define VO_V1_INDEX_c_mask                                                           (0x00007F00)
#define VO_V1_INDEX_c(data)                                                          (0x00007F00&((data)<<8))
#define VO_V1_INDEX_c_src(data)                                                      ((0x00007F00&(data))>>8)
#define VO_V1_INDEX_get_c(data)                                                      ((0x00007F00&(data))>>8)
#define VO_V1_INDEX_y_shift                                                          (0)
#define VO_V1_INDEX_y_mask                                                           (0x0000007F)
#define VO_V1_INDEX_y(data)                                                          (0x0000007F&((data)<<0))
#define VO_V1_INDEX_y_src(data)                                                      ((0x0000007F&(data))>>0)
#define VO_V1_INDEX_get_y(data)                                                      ((0x0000007F&(data))>>0)


#define VO_V1_INDEX_3D                                                               0x18005028
#define VO_V1_INDEX_3D_reg_addr                                                      "0xB8005028"
#define VO_V1_INDEX_3D_reg                                                           0xB8005028
#define set_VO_V1_INDEX_3D_reg(data)   (*((volatile unsigned int*) VO_V1_INDEX_3D_reg)=data)
#define get_VO_V1_INDEX_3D_reg   (*((volatile unsigned int*) VO_V1_INDEX_3D_reg))
#define VO_V1_INDEX_3D_inst_adr                                                      "0x000A"
#define VO_V1_INDEX_3D_inst                                                          0x000A
#define VO_V1_INDEX_3D_w_c_shift                                                     (24)
#define VO_V1_INDEX_3D_w_c_mask                                                      (0x7F000000)
#define VO_V1_INDEX_3D_w_c(data)                                                     (0x7F000000&((data)<<24))
#define VO_V1_INDEX_3D_w_c_src(data)                                                 ((0x7F000000&(data))>>24)
#define VO_V1_INDEX_3D_get_w_c(data)                                                 ((0x7F000000&(data))>>24)
#define VO_V1_INDEX_3D_w_y_shift                                                     (16)
#define VO_V1_INDEX_3D_w_y_mask                                                      (0x007F0000)
#define VO_V1_INDEX_3D_w_y(data)                                                     (0x007F0000&((data)<<16))
#define VO_V1_INDEX_3D_w_y_src(data)                                                 ((0x007F0000&(data))>>16)
#define VO_V1_INDEX_3D_get_w_y(data)                                                 ((0x007F0000&(data))>>16)
#define VO_V1_INDEX_3D_c_shift                                                       (8)
#define VO_V1_INDEX_3D_c_mask                                                        (0x00007F00)
#define VO_V1_INDEX_3D_c(data)                                                       (0x00007F00&((data)<<8))
#define VO_V1_INDEX_3D_c_src(data)                                                   ((0x00007F00&(data))>>8)
#define VO_V1_INDEX_3D_get_c(data)                                                   ((0x00007F00&(data))>>8)
#define VO_V1_INDEX_3D_y_shift                                                       (0)
#define VO_V1_INDEX_3D_y_mask                                                        (0x0000007F)
#define VO_V1_INDEX_3D_y(data)                                                       (0x0000007F&((data)<<0))
#define VO_V1_INDEX_3D_y_src(data)                                                   ((0x0000007F&(data))>>0)
#define VO_V1_INDEX_3D_get_y(data)                                                   ((0x0000007F&(data))>>0)


#define VO_V1_INDEX_RR                                                               0x1800502C
#define VO_V1_INDEX_RR_reg_addr                                                      "0xB800502C"
#define VO_V1_INDEX_RR_reg                                                           0xB800502C
#define set_VO_V1_INDEX_RR_reg(data)   (*((volatile unsigned int*) VO_V1_INDEX_RR_reg)=data)
#define get_VO_V1_INDEX_RR_reg   (*((volatile unsigned int*) VO_V1_INDEX_RR_reg))
#define VO_V1_INDEX_RR_inst_adr                                                      "0x000B"
#define VO_V1_INDEX_RR_inst                                                          0x000B
#define VO_V1_INDEX_RR_w_c_en_shift                                                  (15)
#define VO_V1_INDEX_RR_w_c_en_mask                                                   (0x00008000)
#define VO_V1_INDEX_RR_w_c_en(data)                                                  (0x00008000&((data)<<15))
#define VO_V1_INDEX_RR_w_c_en_src(data)                                              ((0x00008000&(data))>>15)
#define VO_V1_INDEX_RR_get_w_c_en(data)                                              ((0x00008000&(data))>>15)
#define VO_V1_INDEX_RR_w_c_shift                                                     (12)
#define VO_V1_INDEX_RR_w_c_mask                                                      (0x00007000)
#define VO_V1_INDEX_RR_w_c(data)                                                     (0x00007000&((data)<<12))
#define VO_V1_INDEX_RR_w_c_src(data)                                                 ((0x00007000&(data))>>12)
#define VO_V1_INDEX_RR_get_w_c(data)                                                 ((0x00007000&(data))>>12)
#define VO_V1_INDEX_RR_w_y_en_shift                                                  (11)
#define VO_V1_INDEX_RR_w_y_en_mask                                                   (0x00000800)
#define VO_V1_INDEX_RR_w_y_en(data)                                                  (0x00000800&((data)<<11))
#define VO_V1_INDEX_RR_w_y_en_src(data)                                              ((0x00000800&(data))>>11)
#define VO_V1_INDEX_RR_get_w_y_en(data)                                              ((0x00000800&(data))>>11)
#define VO_V1_INDEX_RR_w_y_shift                                                     (8)
#define VO_V1_INDEX_RR_w_y_mask                                                      (0x00000700)
#define VO_V1_INDEX_RR_w_y(data)                                                     (0x00000700&((data)<<8))
#define VO_V1_INDEX_RR_w_y_src(data)                                                 ((0x00000700&(data))>>8)
#define VO_V1_INDEX_RR_get_w_y(data)                                                 ((0x00000700&(data))>>8)
#define VO_V1_INDEX_RR_c_en_shift                                                    (7)
#define VO_V1_INDEX_RR_c_en_mask                                                     (0x00000080)
#define VO_V1_INDEX_RR_c_en(data)                                                    (0x00000080&((data)<<7))
#define VO_V1_INDEX_RR_c_en_src(data)                                                ((0x00000080&(data))>>7)
#define VO_V1_INDEX_RR_get_c_en(data)                                                ((0x00000080&(data))>>7)
#define VO_V1_INDEX_RR_c_shift                                                       (4)
#define VO_V1_INDEX_RR_c_mask                                                        (0x00000070)
#define VO_V1_INDEX_RR_c(data)                                                       (0x00000070&((data)<<4))
#define VO_V1_INDEX_RR_c_src(data)                                                   ((0x00000070&(data))>>4)
#define VO_V1_INDEX_RR_get_c(data)                                                   ((0x00000070&(data))>>4)
#define VO_V1_INDEX_RR_y_en_shift                                                    (3)
#define VO_V1_INDEX_RR_y_en_mask                                                     (0x00000008)
#define VO_V1_INDEX_RR_y_en(data)                                                    (0x00000008&((data)<<3))
#define VO_V1_INDEX_RR_y_en_src(data)                                                ((0x00000008&(data))>>3)
#define VO_V1_INDEX_RR_get_y_en(data)                                                ((0x00000008&(data))>>3)
#define VO_V1_INDEX_RR_y_shift                                                       (0)
#define VO_V1_INDEX_RR_y_mask                                                        (0x00000007)
#define VO_V1_INDEX_RR_y(data)                                                       (0x00000007&((data)<<0))
#define VO_V1_INDEX_RR_y_src(data)                                                   ((0x00000007&(data))>>0)
#define VO_V1_INDEX_RR_get_y(data)                                                   ((0x00000007&(data))>>0)


#define VO_V1_SEQ                                                                    0x18005030
#define VO_V1_SEQ_reg_addr                                                           "0xB8005030"
#define VO_V1_SEQ_reg                                                                0xB8005030
#define set_VO_V1_SEQ_reg(data)   (*((volatile unsigned int*) VO_V1_SEQ_reg)=data)
#define get_VO_V1_SEQ_reg   (*((volatile unsigned int*) VO_V1_SEQ_reg))
#define VO_V1_SEQ_inst_adr                                                           "0x000C"
#define VO_V1_SEQ_inst                                                               0x000C
#define VO_V1_SEQ_sa_shift                                                           (0)
#define VO_V1_SEQ_sa_mask                                                            (0x7FFFFFFF)
#define VO_V1_SEQ_sa(data)                                                           (0x7FFFFFFF&((data)<<0))
#define VO_V1_SEQ_sa_src(data)                                                       ((0x7FFFFFFF&(data))>>0)
#define VO_V1_SEQ_get_sa(data)                                                       ((0x7FFFFFFF&(data))>>0)


#define VO_V1_SEQ_3D                                                                 0x18005034
#define VO_V1_SEQ_3D_reg_addr                                                        "0xB8005034"
#define VO_V1_SEQ_3D_reg                                                             0xB8005034
#define set_VO_V1_SEQ_3D_reg(data)   (*((volatile unsigned int*) VO_V1_SEQ_3D_reg)=data)
#define get_VO_V1_SEQ_3D_reg   (*((volatile unsigned int*) VO_V1_SEQ_3D_reg))
#define VO_V1_SEQ_3D_inst_adr                                                        "0x000D"
#define VO_V1_SEQ_3D_inst                                                            0x000D
#define VO_V1_SEQ_3D_sa_shift                                                        (0)
#define VO_V1_SEQ_3D_sa_mask                                                         (0x7FFFFFFF)
#define VO_V1_SEQ_3D_sa(data)                                                        (0x7FFFFFFF&((data)<<0))
#define VO_V1_SEQ_3D_sa_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define VO_V1_SEQ_3D_get_sa(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define VO_V1_GFX                                                                    0x18005038
#define VO_V1_GFX_reg_addr                                                           "0xB8005038"
#define VO_V1_GFX_reg                                                                0xB8005038
#define set_VO_V1_GFX_reg(data)   (*((volatile unsigned int*) VO_V1_GFX_reg)=data)
#define get_VO_V1_GFX_reg   (*((volatile unsigned int*) VO_V1_GFX_reg))
#define VO_V1_GFX_inst_adr                                                           "0x000E"
#define VO_V1_GFX_inst                                                               0x000E
#define VO_V1_GFX_endian_shift                                                       (30)
#define VO_V1_GFX_endian_mask                                                        (0x40000000)
#define VO_V1_GFX_endian(data)                                                       (0x40000000&((data)<<30))
#define VO_V1_GFX_endian_src(data)                                                   ((0x40000000&(data))>>30)
#define VO_V1_GFX_get_endian(data)                                                   ((0x40000000&(data))>>30)
#define VO_V1_GFX_format_shift                                                       (28)
#define VO_V1_GFX_format_mask                                                        (0x30000000)
#define VO_V1_GFX_format(data)                                                       (0x30000000&((data)<<28))
#define VO_V1_GFX_format_src(data)                                                   ((0x30000000&(data))>>28)
#define VO_V1_GFX_get_format(data)                                                   ((0x30000000&(data))>>28)
#define VO_V1_GFX_extend_shift                                                       (24)
#define VO_V1_GFX_extend_mask                                                        (0x01000000)
#define VO_V1_GFX_extend(data)                                                       (0x01000000&((data)<<24))
#define VO_V1_GFX_extend_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_V1_GFX_get_extend(data)                                                   ((0x01000000&(data))>>24)
#define VO_V1_GFX_pitch_shift                                                        (0)
#define VO_V1_GFX_pitch_mask                                                         (0x0000FFFF)
#define VO_V1_GFX_pitch(data)                                                        (0x0000FFFF&((data)<<0))
#define VO_V1_GFX_pitch_src(data)                                                    ((0x0000FFFF&(data))>>0)
#define VO_V1_GFX_get_pitch(data)                                                    ((0x0000FFFF&(data))>>0)


#define VO_V1_GC1                                                                    0x1800503C
#define VO_V1_GC1_reg_addr                                                           "0xB800503C"
#define VO_V1_GC1_reg                                                                0xB800503C
#define set_VO_V1_GC1_reg(data)   (*((volatile unsigned int*) VO_V1_GC1_reg)=data)
#define get_VO_V1_GC1_reg   (*((volatile unsigned int*) VO_V1_GC1_reg))
#define VO_V1_GC1_inst_adr                                                           "0x000F"
#define VO_V1_GC1_inst                                                               0x000F
#define VO_V1_GC1_m01_shift                                                          (12)
#define VO_V1_GC1_m01_mask                                                           (0x00FFF000)
#define VO_V1_GC1_m01(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_GC1_m01_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC1_get_m01(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC1_m00_shift                                                          (0)
#define VO_V1_GC1_m00_mask                                                           (0x00000FFF)
#define VO_V1_GC1_m00(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_GC1_m00_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_GC1_get_m00(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_GC2                                                                    0x18005040
#define VO_V1_GC2_reg_addr                                                           "0xB8005040"
#define VO_V1_GC2_reg                                                                0xB8005040
#define set_VO_V1_GC2_reg(data)   (*((volatile unsigned int*) VO_V1_GC2_reg)=data)
#define get_VO_V1_GC2_reg   (*((volatile unsigned int*) VO_V1_GC2_reg))
#define VO_V1_GC2_inst_adr                                                           "0x0010"
#define VO_V1_GC2_inst                                                               0x0010
#define VO_V1_GC2_m10_shift                                                          (12)
#define VO_V1_GC2_m10_mask                                                           (0x00FFF000)
#define VO_V1_GC2_m10(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_GC2_m10_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC2_get_m10(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC2_m02_shift                                                          (0)
#define VO_V1_GC2_m02_mask                                                           (0x00000FFF)
#define VO_V1_GC2_m02(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_GC2_m02_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_GC2_get_m02(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_GC3                                                                    0x18005044
#define VO_V1_GC3_reg_addr                                                           "0xB8005044"
#define VO_V1_GC3_reg                                                                0xB8005044
#define set_VO_V1_GC3_reg(data)   (*((volatile unsigned int*) VO_V1_GC3_reg)=data)
#define get_VO_V1_GC3_reg   (*((volatile unsigned int*) VO_V1_GC3_reg))
#define VO_V1_GC3_inst_adr                                                           "0x0011"
#define VO_V1_GC3_inst                                                               0x0011
#define VO_V1_GC3_m12_shift                                                          (12)
#define VO_V1_GC3_m12_mask                                                           (0x00FFF000)
#define VO_V1_GC3_m12(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_GC3_m12_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC3_get_m12(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC3_m11_shift                                                          (0)
#define VO_V1_GC3_m11_mask                                                           (0x00000FFF)
#define VO_V1_GC3_m11(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_GC3_m11_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_GC3_get_m11(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_GC4                                                                    0x18005048
#define VO_V1_GC4_reg_addr                                                           "0xB8005048"
#define VO_V1_GC4_reg                                                                0xB8005048
#define set_VO_V1_GC4_reg(data)   (*((volatile unsigned int*) VO_V1_GC4_reg)=data)
#define get_VO_V1_GC4_reg   (*((volatile unsigned int*) VO_V1_GC4_reg))
#define VO_V1_GC4_inst_adr                                                           "0x0012"
#define VO_V1_GC4_inst                                                               0x0012
#define VO_V1_GC4_m21_shift                                                          (12)
#define VO_V1_GC4_m21_mask                                                           (0x00FFF000)
#define VO_V1_GC4_m21(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_GC4_m21_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC4_get_m21(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_GC4_m20_shift                                                          (0)
#define VO_V1_GC4_m20_mask                                                           (0x00000FFF)
#define VO_V1_GC4_m20(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_GC4_m20_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_GC4_get_m20(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_GC5                                                                    0x1800504C
#define VO_V1_GC5_reg_addr                                                           "0xB800504C"
#define VO_V1_GC5_reg                                                                0xB800504C
#define set_VO_V1_GC5_reg(data)   (*((volatile unsigned int*) VO_V1_GC5_reg)=data)
#define get_VO_V1_GC5_reg   (*((volatile unsigned int*) VO_V1_GC5_reg))
#define VO_V1_GC5_inst_adr                                                           "0x0013"
#define VO_V1_GC5_inst                                                               0x0013
#define VO_V1_GC5_m22_shift                                                          (0)
#define VO_V1_GC5_m22_mask                                                           (0x00000FFF)
#define VO_V1_GC5_m22(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_GC5_m22_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_GC5_get_m22(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_GC6                                                                    0x18005050
#define VO_V1_GC6_reg_addr                                                           "0xB8005050"
#define VO_V1_GC6_reg                                                                0xB8005050
#define set_VO_V1_GC6_reg(data)   (*((volatile unsigned int*) VO_V1_GC6_reg)=data)
#define get_VO_V1_GC6_reg   (*((volatile unsigned int*) VO_V1_GC6_reg))
#define VO_V1_GC6_inst_adr                                                           "0x0014"
#define VO_V1_GC6_inst                                                               0x0014
#define VO_V1_GC6_a2_shift                                                           (18)
#define VO_V1_GC6_a2_mask                                                            (0x07FC0000)
#define VO_V1_GC6_a2(data)                                                           (0x07FC0000&((data)<<18))
#define VO_V1_GC6_a2_src(data)                                                       ((0x07FC0000&(data))>>18)
#define VO_V1_GC6_get_a2(data)                                                       ((0x07FC0000&(data))>>18)
#define VO_V1_GC6_a1_shift                                                           (9)
#define VO_V1_GC6_a1_mask                                                            (0x0003FE00)
#define VO_V1_GC6_a1(data)                                                           (0x0003FE00&((data)<<9))
#define VO_V1_GC6_a1_src(data)                                                       ((0x0003FE00&(data))>>9)
#define VO_V1_GC6_get_a1(data)                                                       ((0x0003FE00&(data))>>9)
#define VO_V1_GC6_a0_shift                                                           (0)
#define VO_V1_GC6_a0_mask                                                            (0x000001FF)
#define VO_V1_GC6_a0(data)                                                           (0x000001FF&((data)<<0))
#define VO_V1_GC6_a0_src(data)                                                       ((0x000001FF&(data))>>0)
#define VO_V1_GC6_get_a0(data)                                                       ((0x000001FF&(data))>>0)


#define VO_V1_ALPHA                                                                  0x18005054
#define VO_V1_ALPHA_reg_addr                                                         "0xB8005054"
#define VO_V1_ALPHA_reg                                                              0xB8005054
#define set_VO_V1_ALPHA_reg(data)   (*((volatile unsigned int*) VO_V1_ALPHA_reg)=data)
#define get_VO_V1_ALPHA_reg   (*((volatile unsigned int*) VO_V1_ALPHA_reg))
#define VO_V1_ALPHA_inst_adr                                                         "0x0015"
#define VO_V1_ALPHA_inst                                                             0x0015
#define VO_V1_ALPHA_a_shift                                                          (0)
#define VO_V1_ALPHA_a_mask                                                           (0x000001FF)
#define VO_V1_ALPHA_a(data)                                                          (0x000001FF&((data)<<0))
#define VO_V1_ALPHA_a_src(data)                                                      ((0x000001FF&(data))>>0)
#define VO_V1_ALPHA_get_a(data)                                                      ((0x000001FF&(data))>>0)


#define VO_V1_SIZE                                                                   0x18005058
#define VO_V1_SIZE_reg_addr                                                          "0xB8005058"
#define VO_V1_SIZE_reg                                                               0xB8005058
#define set_VO_V1_SIZE_reg(data)   (*((volatile unsigned int*) VO_V1_SIZE_reg)=data)
#define get_VO_V1_SIZE_reg   (*((volatile unsigned int*) VO_V1_SIZE_reg))
#define VO_V1_SIZE_inst_adr                                                          "0x0016"
#define VO_V1_SIZE_inst                                                              0x0016
#define VO_V1_SIZE_w_shift                                                           (12)
#define VO_V1_SIZE_w_mask                                                            (0x007FF000)
#define VO_V1_SIZE_w(data)                                                           (0x007FF000&((data)<<12))
#define VO_V1_SIZE_w_src(data)                                                       ((0x007FF000&(data))>>12)
#define VO_V1_SIZE_get_w(data)                                                       ((0x007FF000&(data))>>12)
#define VO_V1_SIZE_h_shift                                                           (0)
#define VO_V1_SIZE_h_mask                                                            (0x000007FF)
#define VO_V1_SIZE_h(data)                                                           (0x000007FF&((data)<<0))
#define VO_V1_SIZE_h_src(data)                                                       ((0x000007FF&(data))>>0)
#define VO_V1_SIZE_get_h(data)                                                       ((0x000007FF&(data))>>0)


#define VO_V1_BUF                                                                    0x1800505C
#define VO_V1_BUF_reg_addr                                                           "0xB800505C"
#define VO_V1_BUF_reg                                                                0xB800505C
#define set_VO_V1_BUF_reg(data)   (*((volatile unsigned int*) VO_V1_BUF_reg)=data)
#define get_VO_V1_BUF_reg   (*((volatile unsigned int*) VO_V1_BUF_reg))
#define VO_V1_BUF_inst_adr                                                           "0x0017"
#define VO_V1_BUF_inst                                                               0x0017
#define VO_V1_BUF_th_shift                                                           (0)
#define VO_V1_BUF_th_mask                                                            (0x000001FF)
#define VO_V1_BUF_th(data)                                                           (0x000001FF&((data)<<0))
#define VO_V1_BUF_th_src(data)                                                       ((0x000001FF&(data))>>0)
#define VO_V1_BUF_get_th(data)                                                       ((0x000001FF&(data))>>0)


#define VO_V1_DI                                                                     0x18005060
#define VO_V1_DI_reg_addr                                                            "0xB8005060"
#define VO_V1_DI_reg                                                                 0xB8005060
#define set_VO_V1_DI_reg(data)   (*((volatile unsigned int*) VO_V1_DI_reg)=data)
#define get_VO_V1_DI_reg   (*((volatile unsigned int*) VO_V1_DI_reg))
#define VO_V1_DI_inst_adr                                                            "0x0018"
#define VO_V1_DI_inst                                                                0x0018
#define VO_V1_DI_mode_shift                                                          (0)
#define VO_V1_DI_mode_mask                                                           (0x00000003)
#define VO_V1_DI_mode(data)                                                          (0x00000003&((data)<<0))
#define VO_V1_DI_mode_src(data)                                                      ((0x00000003&(data))>>0)
#define VO_V1_DI_get_mode(data)                                                      ((0x00000003&(data))>>0)


#define VO_V1_DI_3F                                                                  0x18005064
#define VO_V1_DI_3F_reg_addr                                                         "0xB8005064"
#define VO_V1_DI_3F_reg                                                              0xB8005064
#define set_VO_V1_DI_3F_reg(data)   (*((volatile unsigned int*) VO_V1_DI_3F_reg)=data)
#define get_VO_V1_DI_3F_reg   (*((volatile unsigned int*) VO_V1_DI_3F_reg))
#define VO_V1_DI_3F_inst_adr                                                         "0x0019"
#define VO_V1_DI_3F_inst                                                             0x0019
#define VO_V1_DI_3F_n_c_shift                                                        (24)
#define VO_V1_DI_3F_n_c_mask                                                         (0x7F000000)
#define VO_V1_DI_3F_n_c(data)                                                        (0x7F000000&((data)<<24))
#define VO_V1_DI_3F_n_c_src(data)                                                    ((0x7F000000&(data))>>24)
#define VO_V1_DI_3F_get_n_c(data)                                                    ((0x7F000000&(data))>>24)
#define VO_V1_DI_3F_n_y_shift                                                        (16)
#define VO_V1_DI_3F_n_y_mask                                                         (0x007F0000)
#define VO_V1_DI_3F_n_y(data)                                                        (0x007F0000&((data)<<16))
#define VO_V1_DI_3F_n_y_src(data)                                                    ((0x007F0000&(data))>>16)
#define VO_V1_DI_3F_get_n_y(data)                                                    ((0x007F0000&(data))>>16)
#define VO_V1_DI_3F_p_c_shift                                                        (8)
#define VO_V1_DI_3F_p_c_mask                                                         (0x00007F00)
#define VO_V1_DI_3F_p_c(data)                                                        (0x00007F00&((data)<<8))
#define VO_V1_DI_3F_p_c_src(data)                                                    ((0x00007F00&(data))>>8)
#define VO_V1_DI_3F_get_p_c(data)                                                    ((0x00007F00&(data))>>8)
#define VO_V1_DI_3F_p_y_shift                                                        (0)
#define VO_V1_DI_3F_p_y_mask                                                         (0x0000007F)
#define VO_V1_DI_3F_p_y(data)                                                        (0x0000007F&((data)<<0))
#define VO_V1_DI_3F_p_y_src(data)                                                    ((0x0000007F&(data))>>0)
#define VO_V1_DI_3F_get_p_y(data)                                                    ((0x0000007F&(data))>>0)


#define VO_V1_DI_3F_RR                                                               0x18005068
#define VO_V1_DI_3F_RR_reg_addr                                                      "0xB8005068"
#define VO_V1_DI_3F_RR_reg                                                           0xB8005068
#define set_VO_V1_DI_3F_RR_reg(data)   (*((volatile unsigned int*) VO_V1_DI_3F_RR_reg)=data)
#define get_VO_V1_DI_3F_RR_reg   (*((volatile unsigned int*) VO_V1_DI_3F_RR_reg))
#define VO_V1_DI_3F_RR_inst_adr                                                      "0x001A"
#define VO_V1_DI_3F_RR_inst                                                          0x001A
#define VO_V1_DI_3F_RR_n_c_en_shift                                                  (15)
#define VO_V1_DI_3F_RR_n_c_en_mask                                                   (0x00008000)
#define VO_V1_DI_3F_RR_n_c_en(data)                                                  (0x00008000&((data)<<15))
#define VO_V1_DI_3F_RR_n_c_en_src(data)                                              ((0x00008000&(data))>>15)
#define VO_V1_DI_3F_RR_get_n_c_en(data)                                              ((0x00008000&(data))>>15)
#define VO_V1_DI_3F_RR_n_c_shift                                                     (12)
#define VO_V1_DI_3F_RR_n_c_mask                                                      (0x00007000)
#define VO_V1_DI_3F_RR_n_c(data)                                                     (0x00007000&((data)<<12))
#define VO_V1_DI_3F_RR_n_c_src(data)                                                 ((0x00007000&(data))>>12)
#define VO_V1_DI_3F_RR_get_n_c(data)                                                 ((0x00007000&(data))>>12)
#define VO_V1_DI_3F_RR_n_y_en_shift                                                  (11)
#define VO_V1_DI_3F_RR_n_y_en_mask                                                   (0x00000800)
#define VO_V1_DI_3F_RR_n_y_en(data)                                                  (0x00000800&((data)<<11))
#define VO_V1_DI_3F_RR_n_y_en_src(data)                                              ((0x00000800&(data))>>11)
#define VO_V1_DI_3F_RR_get_n_y_en(data)                                              ((0x00000800&(data))>>11)
#define VO_V1_DI_3F_RR_n_y_shift                                                     (8)
#define VO_V1_DI_3F_RR_n_y_mask                                                      (0x00000700)
#define VO_V1_DI_3F_RR_n_y(data)                                                     (0x00000700&((data)<<8))
#define VO_V1_DI_3F_RR_n_y_src(data)                                                 ((0x00000700&(data))>>8)
#define VO_V1_DI_3F_RR_get_n_y(data)                                                 ((0x00000700&(data))>>8)
#define VO_V1_DI_3F_RR_p_c_en_shift                                                  (7)
#define VO_V1_DI_3F_RR_p_c_en_mask                                                   (0x00000080)
#define VO_V1_DI_3F_RR_p_c_en(data)                                                  (0x00000080&((data)<<7))
#define VO_V1_DI_3F_RR_p_c_en_src(data)                                              ((0x00000080&(data))>>7)
#define VO_V1_DI_3F_RR_get_p_c_en(data)                                              ((0x00000080&(data))>>7)
#define VO_V1_DI_3F_RR_p_c_shift                                                     (4)
#define VO_V1_DI_3F_RR_p_c_mask                                                      (0x00000070)
#define VO_V1_DI_3F_RR_p_c(data)                                                     (0x00000070&((data)<<4))
#define VO_V1_DI_3F_RR_p_c_src(data)                                                 ((0x00000070&(data))>>4)
#define VO_V1_DI_3F_RR_get_p_c(data)                                                 ((0x00000070&(data))>>4)
#define VO_V1_DI_3F_RR_p_y_en_shift                                                  (3)
#define VO_V1_DI_3F_RR_p_y_en_mask                                                   (0x00000008)
#define VO_V1_DI_3F_RR_p_y_en(data)                                                  (0x00000008&((data)<<3))
#define VO_V1_DI_3F_RR_p_y_en_src(data)                                              ((0x00000008&(data))>>3)
#define VO_V1_DI_3F_RR_get_p_y_en(data)                                              ((0x00000008&(data))>>3)
#define VO_V1_DI_3F_RR_p_y_shift                                                     (0)
#define VO_V1_DI_3F_RR_p_y_mask                                                      (0x00000007)
#define VO_V1_DI_3F_RR_p_y(data)                                                     (0x00000007&((data)<<0))
#define VO_V1_DI_3F_RR_p_y_src(data)                                                 ((0x00000007&(data))>>0)
#define VO_V1_DI_3F_RR_get_p_y(data)                                                 ((0x00000007&(data))>>0)


#define VO_V1_MA_FR                                                                  0x1800506C
#define VO_V1_MA_FR_reg_addr                                                         "0xB800506C"
#define VO_V1_MA_FR_reg                                                              0xB800506C
#define set_VO_V1_MA_FR_reg(data)   (*((volatile unsigned int*) VO_V1_MA_FR_reg)=data)
#define get_VO_V1_MA_FR_reg   (*((volatile unsigned int*) VO_V1_MA_FR_reg))
#define VO_V1_MA_FR_inst_adr                                                         "0x001B"
#define VO_V1_MA_FR_inst                                                             0x001B
#define VO_V1_MA_FR_a_shift                                                          (0)
#define VO_V1_MA_FR_a_mask                                                           (0x7FFFFFFF)
#define VO_V1_MA_FR_a(data)                                                          (0x7FFFFFFF&((data)<<0))
#define VO_V1_MA_FR_a_src(data)                                                      ((0x7FFFFFFF&(data))>>0)
#define VO_V1_MA_FR_get_a(data)                                                      ((0x7FFFFFFF&(data))>>0)


#define VO_V1_MA_FW                                                                  0x18005070
#define VO_V1_MA_FW_reg_addr                                                         "0xB8005070"
#define VO_V1_MA_FW_reg                                                              0xB8005070
#define set_VO_V1_MA_FW_reg(data)   (*((volatile unsigned int*) VO_V1_MA_FW_reg)=data)
#define get_VO_V1_MA_FW_reg   (*((volatile unsigned int*) VO_V1_MA_FW_reg))
#define VO_V1_MA_FW_inst_adr                                                         "0x001C"
#define VO_V1_MA_FW_inst                                                             0x001C
#define VO_V1_MA_FW_a_shift                                                          (0)
#define VO_V1_MA_FW_a_mask                                                           (0x7FFFFFFF)
#define VO_V1_MA_FW_a(data)                                                          (0x7FFFFFFF&((data)<<0))
#define VO_V1_MA_FW_a_src(data)                                                      ((0x7FFFFFFF&(data))>>0)
#define VO_V1_MA_FW_get_a(data)                                                      ((0x7FFFFFFF&(data))>>0)


#define VO_V1_MA_MV                                                                  0x18005074
#define VO_V1_MA_MV_reg_addr                                                         "0xB8005074"
#define VO_V1_MA_MV_reg                                                              0xB8005074
#define set_VO_V1_MA_MV_reg(data)   (*((volatile unsigned int*) VO_V1_MA_MV_reg)=data)
#define get_VO_V1_MA_MV_reg   (*((volatile unsigned int*) VO_V1_MA_MV_reg))
#define VO_V1_MA_MV_inst_adr                                                         "0x001D"
#define VO_V1_MA_MV_inst                                                             0x001D
#define VO_V1_MA_MV_index_shift                                                      (0)
#define VO_V1_MA_MV_index_mask                                                       (0x0000007F)
#define VO_V1_MA_MV_index(data)                                                      (0x0000007F&((data)<<0))
#define VO_V1_MA_MV_index_src(data)                                                  ((0x0000007F&(data))>>0)
#define VO_V1_MA_MV_get_index(data)                                                  ((0x0000007F&(data))>>0)


#define VO_V1_MA_SAD                                                                 0x18005078
#define VO_V1_MA_SAD_reg_addr                                                        "0xB8005078"
#define VO_V1_MA_SAD_reg                                                             0xB8005078
#define set_VO_V1_MA_SAD_reg(data)   (*((volatile unsigned int*) VO_V1_MA_SAD_reg)=data)
#define get_VO_V1_MA_SAD_reg   (*((volatile unsigned int*) VO_V1_MA_SAD_reg))
#define VO_V1_MA_SAD_inst_adr                                                        "0x001E"
#define VO_V1_MA_SAD_inst                                                            0x001E
#define VO_V1_MA_SAD_index_shift                                                     (0)
#define VO_V1_MA_SAD_index_mask                                                      (0x0000007F)
#define VO_V1_MA_SAD_index(data)                                                     (0x0000007F&((data)<<0))
#define VO_V1_MA_SAD_index_src(data)                                                 ((0x0000007F&(data))>>0)
#define VO_V1_MA_SAD_get_index(data)                                                 ((0x0000007F&(data))>>0)


#define VO_V1_INTRA                                                                  0x1800507C
#define VO_V1_INTRA_reg_addr                                                         "0xB800507C"
#define VO_V1_INTRA_reg                                                              0xB800507C
#define set_VO_V1_INTRA_reg(data)   (*((volatile unsigned int*) VO_V1_INTRA_reg)=data)
#define get_VO_V1_INTRA_reg   (*((volatile unsigned int*) VO_V1_INTRA_reg))
#define VO_V1_INTRA_inst_adr                                                         "0x001F"
#define VO_V1_INTRA_inst                                                             0x001F
#define VO_V1_INTRA_c_range_shift                                                    (26)
#define VO_V1_INTRA_c_range_mask                                                     (0x04000000)
#define VO_V1_INTRA_c_range(data)                                                    (0x04000000&((data)<<26))
#define VO_V1_INTRA_c_range_src(data)                                                ((0x04000000&(data))>>26)
#define VO_V1_INTRA_get_c_range(data)                                                ((0x04000000&(data))>>26)
#define VO_V1_INTRA_y_range_shift                                                    (24)
#define VO_V1_INTRA_y_range_mask                                                     (0x03000000)
#define VO_V1_INTRA_y_range(data)                                                    (0x03000000&((data)<<24))
#define VO_V1_INTRA_y_range_src(data)                                                ((0x03000000&(data))>>24)
#define VO_V1_INTRA_get_y_range(data)                                                ((0x03000000&(data))>>24)
#define VO_V1_INTRA_th_pos_shift                                                     (16)
#define VO_V1_INTRA_th_pos_mask                                                      (0x001F0000)
#define VO_V1_INTRA_th_pos(data)                                                     (0x001F0000&((data)<<16))
#define VO_V1_INTRA_th_pos_src(data)                                                 ((0x001F0000&(data))>>16)
#define VO_V1_INTRA_get_th_pos(data)                                                 ((0x001F0000&(data))>>16)
#define VO_V1_INTRA_th_prediff_shift                                                 (8)
#define VO_V1_INTRA_th_prediff_mask                                                  (0x00007F00)
#define VO_V1_INTRA_th_prediff(data)                                                 (0x00007F00&((data)<<8))
#define VO_V1_INTRA_th_prediff_src(data)                                             ((0x00007F00&(data))>>8)
#define VO_V1_INTRA_get_th_prediff(data)                                             ((0x00007F00&(data))>>8)
#define VO_V1_INTRA_th_refdiff_shift                                                 (0)
#define VO_V1_INTRA_th_refdiff_mask                                                  (0x0000003F)
#define VO_V1_INTRA_th_refdiff(data)                                                 (0x0000003F&((data)<<0))
#define VO_V1_INTRA_th_refdiff_src(data)                                             ((0x0000003F&(data))>>0)
#define VO_V1_INTRA_get_th_refdiff(data)                                             ((0x0000003F&(data))>>0)


#define VO_V1_MA                                                                     0x18005080
#define VO_V1_MA_reg_addr                                                            "0xB8005080"
#define VO_V1_MA_reg                                                                 0xB8005080
#define set_VO_V1_MA_reg(data)   (*((volatile unsigned int*) VO_V1_MA_reg)=data)
#define get_VO_V1_MA_reg   (*((volatile unsigned int*) VO_V1_MA_reg))
#define VO_V1_MA_inst_adr                                                            "0x0020"
#define VO_V1_MA_inst                                                                0x0020
#define VO_V1_MA_split_shift                                                         (31)
#define VO_V1_MA_split_mask                                                          (0x80000000)
#define VO_V1_MA_split(data)                                                         (0x80000000&((data)<<31))
#define VO_V1_MA_split_src(data)                                                     ((0x80000000&(data))>>31)
#define VO_V1_MA_get_split(data)                                                     ((0x80000000&(data))>>31)
#define VO_V1_MA_c_intra_shift                                                       (30)
#define VO_V1_MA_c_intra_mask                                                        (0x40000000)
#define VO_V1_MA_c_intra(data)                                                       (0x40000000&((data)<<30))
#define VO_V1_MA_c_intra_src(data)                                                   ((0x40000000&(data))>>30)
#define VO_V1_MA_get_c_intra(data)                                                   ((0x40000000&(data))>>30)
#define VO_V1_MA_lp_shift                                                            (29)
#define VO_V1_MA_lp_mask                                                             (0x20000000)
#define VO_V1_MA_lp(data)                                                            (0x20000000&((data)<<29))
#define VO_V1_MA_lp_src(data)                                                        ((0x20000000&(data))>>29)
#define VO_V1_MA_get_lp(data)                                                        ((0x20000000&(data))>>29)
#define VO_V1_MA_fm_shift                                                            (28)
#define VO_V1_MA_fm_mask                                                             (0x10000000)
#define VO_V1_MA_fm(data)                                                            (0x10000000&((data)<<28))
#define VO_V1_MA_fm_src(data)                                                        ((0x10000000&(data))>>28)
#define VO_V1_MA_get_fm(data)                                                        ((0x10000000&(data))>>28)
#define VO_V1_MA_sc_shift                                                            (24)
#define VO_V1_MA_sc_mask                                                             (0x07000000)
#define VO_V1_MA_sc(data)                                                            (0x07000000&((data)<<24))
#define VO_V1_MA_sc_src(data)                                                        ((0x07000000&(data))>>24)
#define VO_V1_MA_get_sc(data)                                                        ((0x07000000&(data))>>24)
#define VO_V1_MA_th_noise_shift                                                      (16)
#define VO_V1_MA_th_noise_mask                                                       (0x00FF0000)
#define VO_V1_MA_th_noise(data)                                                      (0x00FF0000&((data)<<16))
#define VO_V1_MA_th_noise_src(data)                                                  ((0x00FF0000&(data))>>16)
#define VO_V1_MA_get_th_noise(data)                                                  ((0x00FF0000&(data))>>16)
#define VO_V1_MA_th_c_shift                                                          (8)
#define VO_V1_MA_th_c_mask                                                           (0x0000FF00)
#define VO_V1_MA_th_c(data)                                                          (0x0000FF00&((data)<<8))
#define VO_V1_MA_th_c_src(data)                                                      ((0x0000FF00&(data))>>8)
#define VO_V1_MA_get_th_c(data)                                                      ((0x0000FF00&(data))>>8)
#define VO_V1_MA_th_y_shift                                                          (0)
#define VO_V1_MA_th_y_mask                                                           (0x000000FF)
#define VO_V1_MA_th_y(data)                                                          (0x000000FF&((data)<<0))
#define VO_V1_MA_th_y_src(data)                                                      ((0x000000FF&(data))>>0)
#define VO_V1_MA_get_th_y(data)                                                      ((0x000000FF&(data))>>0)


#define VO_V1_MA_NEW                                                                 0x18005084
#define VO_V1_MA_NEW_reg_addr                                                        "0xB8005084"
#define VO_V1_MA_NEW_reg                                                             0xB8005084
#define set_VO_V1_MA_NEW_reg(data)   (*((volatile unsigned int*) VO_V1_MA_NEW_reg)=data)
#define get_VO_V1_MA_NEW_reg   (*((volatile unsigned int*) VO_V1_MA_NEW_reg))
#define VO_V1_MA_NEW_inst_adr                                                        "0x0021"
#define VO_V1_MA_NEW_inst                                                            0x0021
#define VO_V1_MA_NEW_correct_shift                                                   (8)
#define VO_V1_MA_NEW_correct_mask                                                    (0x00000100)
#define VO_V1_MA_NEW_correct(data)                                                   (0x00000100&((data)<<8))
#define VO_V1_MA_NEW_correct_src(data)                                               ((0x00000100&(data))>>8)
#define VO_V1_MA_NEW_get_correct(data)                                               ((0x00000100&(data))>>8)
#define VO_V1_MA_NEW_fm_shift                                                        (7)
#define VO_V1_MA_NEW_fm_mask                                                         (0x00000080)
#define VO_V1_MA_NEW_fm(data)                                                        (0x00000080&((data)<<7))
#define VO_V1_MA_NEW_fm_src(data)                                                    ((0x00000080&(data))>>7)
#define VO_V1_MA_NEW_get_fm(data)                                                    ((0x00000080&(data))>>7)
#define VO_V1_MA_NEW_limit_shift                                                     (6)
#define VO_V1_MA_NEW_limit_mask                                                      (0x00000040)
#define VO_V1_MA_NEW_limit(data)                                                     (0x00000040&((data)<<6))
#define VO_V1_MA_NEW_limit_src(data)                                                 ((0x00000040&(data))>>6)
#define VO_V1_MA_NEW_get_limit(data)                                                 ((0x00000040&(data))>>6)
#define VO_V1_MA_NEW_weave_shift                                                     (5)
#define VO_V1_MA_NEW_weave_mask                                                      (0x00000020)
#define VO_V1_MA_NEW_weave(data)                                                     (0x00000020&((data)<<5))
#define VO_V1_MA_NEW_weave_src(data)                                                 ((0x00000020&(data))>>5)
#define VO_V1_MA_NEW_get_weave(data)                                                 ((0x00000020&(data))>>5)
#define VO_V1_MA_NEW_blend_shift                                                     (4)
#define VO_V1_MA_NEW_blend_mask                                                      (0x00000010)
#define VO_V1_MA_NEW_blend(data)                                                     (0x00000010&((data)<<4))
#define VO_V1_MA_NEW_blend_src(data)                                                 ((0x00000010&(data))>>4)
#define VO_V1_MA_NEW_get_blend(data)                                                 ((0x00000010&(data))>>4)
#define VO_V1_MA_NEW_noisy_high_shift                                                (3)
#define VO_V1_MA_NEW_noisy_high_mask                                                 (0x00000008)
#define VO_V1_MA_NEW_noisy_high(data)                                                (0x00000008&((data)<<3))
#define VO_V1_MA_NEW_noisy_high_src(data)                                            ((0x00000008&(data))>>3)
#define VO_V1_MA_NEW_get_noisy_high(data)                                            ((0x00000008&(data))>>3)
#define VO_V1_MA_NEW_noisy_shift                                                     (2)
#define VO_V1_MA_NEW_noisy_mask                                                      (0x00000004)
#define VO_V1_MA_NEW_noisy(data)                                                     (0x00000004&((data)<<2))
#define VO_V1_MA_NEW_noisy_src(data)                                                 ((0x00000004&(data))>>2)
#define VO_V1_MA_NEW_get_noisy(data)                                                 ((0x00000004&(data))>>2)
#define VO_V1_MA_NEW_ma_shift                                                        (1)
#define VO_V1_MA_NEW_ma_mask                                                         (0x00000002)
#define VO_V1_MA_NEW_ma(data)                                                        (0x00000002&((data)<<1))
#define VO_V1_MA_NEW_ma_src(data)                                                    ((0x00000002&(data))>>1)
#define VO_V1_MA_NEW_get_ma(data)                                                    ((0x00000002&(data))>>1)
#define VO_V1_MA_NEW_write_data_shift                                                (0)
#define VO_V1_MA_NEW_write_data_mask                                                 (0x00000001)
#define VO_V1_MA_NEW_write_data(data)                                                (0x00000001&((data)<<0))
#define VO_V1_MA_NEW_write_data_src(data)                                            ((0x00000001&(data))>>0)
#define VO_V1_MA_NEW_get_write_data(data)                                            ((0x00000001&(data))>>0)


#define VO_V1_MA_REF                                                                 0x18005088
#define VO_V1_MA_REF_reg_addr                                                        "0xB8005088"
#define VO_V1_MA_REF_reg                                                             0xB8005088
#define set_VO_V1_MA_REF_reg(data)   (*((volatile unsigned int*) VO_V1_MA_REF_reg)=data)
#define get_VO_V1_MA_REF_reg   (*((volatile unsigned int*) VO_V1_MA_REF_reg))
#define VO_V1_MA_REF_inst_adr                                                        "0x0022"
#define VO_V1_MA_REF_inst                                                            0x0022
#define VO_V1_MA_REF_mode_shift                                                      (3)
#define VO_V1_MA_REF_mode_mask                                                       (0x00000008)
#define VO_V1_MA_REF_mode(data)                                                      (0x00000008&((data)<<3))
#define VO_V1_MA_REF_mode_src(data)                                                  ((0x00000008&(data))>>3)
#define VO_V1_MA_REF_get_mode(data)                                                  ((0x00000008&(data))>>3)
#define VO_V1_MA_REF_correct_shift                                                   (2)
#define VO_V1_MA_REF_correct_mask                                                    (0x00000004)
#define VO_V1_MA_REF_correct(data)                                                   (0x00000004&((data)<<2))
#define VO_V1_MA_REF_correct_src(data)                                               ((0x00000004&(data))>>2)
#define VO_V1_MA_REF_get_correct(data)                                               ((0x00000004&(data))>>2)
#define VO_V1_MA_REF_en_shift                                                        (1)
#define VO_V1_MA_REF_en_mask                                                         (0x00000002)
#define VO_V1_MA_REF_en(data)                                                        (0x00000002&((data)<<1))
#define VO_V1_MA_REF_en_src(data)                                                    ((0x00000002&(data))>>1)
#define VO_V1_MA_REF_get_en(data)                                                    ((0x00000002&(data))>>1)
#define VO_V1_MA_REF_write_data_shift                                                (0)
#define VO_V1_MA_REF_write_data_mask                                                 (0x00000001)
#define VO_V1_MA_REF_write_data(data)                                                (0x00000001&((data)<<0))
#define VO_V1_MA_REF_write_data_src(data)                                            ((0x00000001&(data))>>0)
#define VO_V1_MA_REF_get_write_data(data)                                            ((0x00000001&(data))>>0)


#define VO_V1_MA_TH                                                                  0x1800508C
#define VO_V1_MA_TH_reg_addr                                                         "0xB800508C"
#define VO_V1_MA_TH_reg                                                              0xB800508C
#define set_VO_V1_MA_TH_reg(data)   (*((volatile unsigned int*) VO_V1_MA_TH_reg)=data)
#define get_VO_V1_MA_TH_reg   (*((volatile unsigned int*) VO_V1_MA_TH_reg))
#define VO_V1_MA_TH_inst_adr                                                         "0x0023"
#define VO_V1_MA_TH_inst                                                             0x0023
#define VO_V1_MA_TH_m1d1_shift                                                       (24)
#define VO_V1_MA_TH_m1d1_mask                                                        (0xFF000000)
#define VO_V1_MA_TH_m1d1(data)                                                       (0xFF000000&((data)<<24))
#define VO_V1_MA_TH_m1d1_src(data)                                                   ((0xFF000000&(data))>>24)
#define VO_V1_MA_TH_get_m1d1(data)                                                   ((0xFF000000&(data))>>24)
#define VO_V1_MA_TH_m1d0_shift                                                       (16)
#define VO_V1_MA_TH_m1d0_mask                                                        (0x00FF0000)
#define VO_V1_MA_TH_m1d0(data)                                                       (0x00FF0000&((data)<<16))
#define VO_V1_MA_TH_m1d0_src(data)                                                   ((0x00FF0000&(data))>>16)
#define VO_V1_MA_TH_get_m1d0(data)                                                   ((0x00FF0000&(data))>>16)
#define VO_V1_MA_TH_m0d1_shift                                                       (8)
#define VO_V1_MA_TH_m0d1_mask                                                        (0x0000FF00)
#define VO_V1_MA_TH_m0d1(data)                                                       (0x0000FF00&((data)<<8))
#define VO_V1_MA_TH_m0d1_src(data)                                                   ((0x0000FF00&(data))>>8)
#define VO_V1_MA_TH_get_m0d1(data)                                                   ((0x0000FF00&(data))>>8)
#define VO_V1_MA_TH_m0d0_shift                                                       (0)
#define VO_V1_MA_TH_m0d0_mask                                                        (0x000000FF)
#define VO_V1_MA_TH_m0d0(data)                                                       (0x000000FF&((data)<<0))
#define VO_V1_MA_TH_m0d0_src(data)                                                   ((0x000000FF&(data))>>0)
#define VO_V1_MA_TH_get_m0d0(data)                                                   ((0x000000FF&(data))>>0)


#define VO_V1_MA_FT                                                                  0x18005090
#define VO_V1_MA_FT_reg_addr                                                         "0xB8005090"
#define VO_V1_MA_FT_reg                                                              0xB8005090
#define set_VO_V1_MA_FT_reg(data)   (*((volatile unsigned int*) VO_V1_MA_FT_reg)=data)
#define get_VO_V1_MA_FT_reg   (*((volatile unsigned int*) VO_V1_MA_FT_reg))
#define VO_V1_MA_FT_inst_adr                                                         "0x0024"
#define VO_V1_MA_FT_inst                                                             0x0024
#define VO_V1_MA_FT_limit_shift                                                      (16)
#define VO_V1_MA_FT_limit_mask                                                       (0x00FF0000)
#define VO_V1_MA_FT_limit(data)                                                      (0x00FF0000&((data)<<16))
#define VO_V1_MA_FT_limit_src(data)                                                  ((0x00FF0000&(data))>>16)
#define VO_V1_MA_FT_get_limit(data)                                                  ((0x00FF0000&(data))>>16)
#define VO_V1_MA_FT_big_shift                                                        (8)
#define VO_V1_MA_FT_big_mask                                                         (0x0000FF00)
#define VO_V1_MA_FT_big(data)                                                        (0x0000FF00&((data)<<8))
#define VO_V1_MA_FT_big_src(data)                                                    ((0x0000FF00&(data))>>8)
#define VO_V1_MA_FT_get_big(data)                                                    ((0x0000FF00&(data))>>8)
#define VO_V1_MA_FT_th_shift                                                         (0)
#define VO_V1_MA_FT_th_mask                                                          (0x000000FF)
#define VO_V1_MA_FT_th(data)                                                         (0x000000FF&((data)<<0))
#define VO_V1_MA_FT_th_src(data)                                                     ((0x000000FF&(data))>>0)
#define VO_V1_MA_FT_get_th(data)                                                     ((0x000000FF&(data))>>0)


#define VO_V1_MA_FM                                                                  0x18005094
#define VO_V1_MA_FM_reg_addr                                                         "0xB8005094"
#define VO_V1_MA_FM_reg                                                              0xB8005094
#define set_VO_V1_MA_FM_reg(data)   (*((volatile unsigned int*) VO_V1_MA_FM_reg)=data)
#define get_VO_V1_MA_FM_reg   (*((volatile unsigned int*) VO_V1_MA_FM_reg))
#define VO_V1_MA_FM_inst_adr                                                         "0x0025"
#define VO_V1_MA_FM_inst                                                             0x0025
#define VO_V1_MA_FM_noisy_shift                                                      (8)
#define VO_V1_MA_FM_noisy_mask                                                       (0x0000FF00)
#define VO_V1_MA_FM_noisy(data)                                                      (0x0000FF00&((data)<<8))
#define VO_V1_MA_FM_noisy_src(data)                                                  ((0x0000FF00&(data))>>8)
#define VO_V1_MA_FM_get_noisy(data)                                                  ((0x0000FF00&(data))>>8)
#define VO_V1_MA_FM_th_shift                                                         (0)
#define VO_V1_MA_FM_th_mask                                                          (0x000000FF)
#define VO_V1_MA_FM_th(data)                                                         (0x000000FF&((data)<<0))
#define VO_V1_MA_FM_th_src(data)                                                     ((0x000000FF&(data))>>0)
#define VO_V1_MA_FM_get_th(data)                                                     ((0x000000FF&(data))>>0)


#define VO_V1_DI_BLEND                                                               0x18005098
#define VO_V1_DI_BLEND_reg_addr                                                      "0xB8005098"
#define VO_V1_DI_BLEND_reg                                                           0xB8005098
#define set_VO_V1_DI_BLEND_reg(data)   (*((volatile unsigned int*) VO_V1_DI_BLEND_reg)=data)
#define get_VO_V1_DI_BLEND_reg   (*((volatile unsigned int*) VO_V1_DI_BLEND_reg))
#define VO_V1_DI_BLEND_inst_adr                                                      "0x0026"
#define VO_V1_DI_BLEND_inst                                                          0x0026
#define VO_V1_DI_BLEND_cth_log2_shift                                                (24)
#define VO_V1_DI_BLEND_cth_log2_mask                                                 (0x07000000)
#define VO_V1_DI_BLEND_cth_log2(data)                                                (0x07000000&((data)<<24))
#define VO_V1_DI_BLEND_cth_log2_src(data)                                            ((0x07000000&(data))>>24)
#define VO_V1_DI_BLEND_get_cth_log2(data)                                            ((0x07000000&(data))>>24)
#define VO_V1_DI_BLEND_cth_low_shift                                                 (16)
#define VO_V1_DI_BLEND_cth_low_mask                                                  (0x00FF0000)
#define VO_V1_DI_BLEND_cth_low(data)                                                 (0x00FF0000&((data)<<16))
#define VO_V1_DI_BLEND_cth_low_src(data)                                             ((0x00FF0000&(data))>>16)
#define VO_V1_DI_BLEND_get_cth_low(data)                                             ((0x00FF0000&(data))>>16)
#define VO_V1_DI_BLEND_yth_log2_shift                                                (8)
#define VO_V1_DI_BLEND_yth_log2_mask                                                 (0x00000700)
#define VO_V1_DI_BLEND_yth_log2(data)                                                (0x00000700&((data)<<8))
#define VO_V1_DI_BLEND_yth_log2_src(data)                                            ((0x00000700&(data))>>8)
#define VO_V1_DI_BLEND_get_yth_log2(data)                                            ((0x00000700&(data))>>8)
#define VO_V1_DI_BLEND_yth_low_shift                                                 (0)
#define VO_V1_DI_BLEND_yth_low_mask                                                  (0x000000FF)
#define VO_V1_DI_BLEND_yth_low(data)                                                 (0x000000FF&((data)<<0))
#define VO_V1_DI_BLEND_yth_low_src(data)                                             ((0x000000FF&(data))>>0)
#define VO_V1_DI_BLEND_get_yth_low(data)                                             ((0x000000FF&(data))>>0)


#define VO_V1_FILM                                                                   0x1800509C
#define VO_V1_FILM_reg_addr                                                          "0xB800509C"
#define VO_V1_FILM_reg                                                               0xB800509C
#define set_VO_V1_FILM_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_reg)=data)
#define get_VO_V1_FILM_reg   (*((volatile unsigned int*) VO_V1_FILM_reg))
#define VO_V1_FILM_inst_adr                                                          "0x0027"
#define VO_V1_FILM_inst                                                              0x0027
#define VO_V1_FILM_mode_shift                                                        (0)
#define VO_V1_FILM_mode_mask                                                         (0x00000003)
#define VO_V1_FILM_mode(data)                                                        (0x00000003&((data)<<0))
#define VO_V1_FILM_mode_src(data)                                                    ((0x00000003&(data))>>0)
#define VO_V1_FILM_get_mode(data)                                                    ((0x00000003&(data))>>0)


#define VO_V1_WADTB_0                                                                0x180050A0
#define VO_V1_WADTB_1                                                                0x180050A4
#define VO_V1_WADTB_2                                                                0x180050A8
#define VO_V1_WADTB_3                                                                0x180050AC
#define VO_V1_WADTB_4                                                                0x180050B0
#define VO_V1_WADTB_5                                                                0x180050B4
#define VO_V1_WADTB_6                                                                0x180050B8
#define VO_V1_WADTB_7                                                                0x180050BC
#define VO_V1_WADTB_8                                                                0x180050C0
#define VO_V1_WADTB_0_reg_addr                                                       "0xB80050A0"
#define VO_V1_WADTB_1_reg_addr                                                       "0xB80050A4"
#define VO_V1_WADTB_2_reg_addr                                                       "0xB80050A8"
#define VO_V1_WADTB_3_reg_addr                                                       "0xB80050AC"
#define VO_V1_WADTB_4_reg_addr                                                       "0xB80050B0"
#define VO_V1_WADTB_5_reg_addr                                                       "0xB80050B4"
#define VO_V1_WADTB_6_reg_addr                                                       "0xB80050B8"
#define VO_V1_WADTB_7_reg_addr                                                       "0xB80050BC"
#define VO_V1_WADTB_8_reg_addr                                                       "0xB80050C0"
#define VO_V1_WADTB_0_reg                                                            0xB80050A0
#define VO_V1_WADTB_1_reg                                                            0xB80050A4
#define VO_V1_WADTB_2_reg                                                            0xB80050A8
#define VO_V1_WADTB_3_reg                                                            0xB80050AC
#define VO_V1_WADTB_4_reg                                                            0xB80050B0
#define VO_V1_WADTB_5_reg                                                            0xB80050B4
#define VO_V1_WADTB_6_reg                                                            0xB80050B8
#define VO_V1_WADTB_7_reg                                                            0xB80050BC
#define VO_V1_WADTB_8_reg                                                            0xB80050C0
#define set_VO_V1_WADTB_0_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_0_reg)=data)
#define set_VO_V1_WADTB_1_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_1_reg)=data)
#define set_VO_V1_WADTB_2_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_2_reg)=data)
#define set_VO_V1_WADTB_3_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_3_reg)=data)
#define set_VO_V1_WADTB_4_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_4_reg)=data)
#define set_VO_V1_WADTB_5_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_5_reg)=data)
#define set_VO_V1_WADTB_6_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_6_reg)=data)
#define set_VO_V1_WADTB_7_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_7_reg)=data)
#define set_VO_V1_WADTB_8_reg(data)   (*((volatile unsigned int*) VO_V1_WADTB_8_reg)=data)
#define get_VO_V1_WADTB_0_reg   (*((volatile unsigned int*) VO_V1_WADTB_0_reg))
#define get_VO_V1_WADTB_1_reg   (*((volatile unsigned int*) VO_V1_WADTB_1_reg))
#define get_VO_V1_WADTB_2_reg   (*((volatile unsigned int*) VO_V1_WADTB_2_reg))
#define get_VO_V1_WADTB_3_reg   (*((volatile unsigned int*) VO_V1_WADTB_3_reg))
#define get_VO_V1_WADTB_4_reg   (*((volatile unsigned int*) VO_V1_WADTB_4_reg))
#define get_VO_V1_WADTB_5_reg   (*((volatile unsigned int*) VO_V1_WADTB_5_reg))
#define get_VO_V1_WADTB_6_reg   (*((volatile unsigned int*) VO_V1_WADTB_6_reg))
#define get_VO_V1_WADTB_7_reg   (*((volatile unsigned int*) VO_V1_WADTB_7_reg))
#define get_VO_V1_WADTB_8_reg   (*((volatile unsigned int*) VO_V1_WADTB_8_reg))
#define VO_V1_WADTB_0_inst_adr                                                       "0x0028"
#define VO_V1_WADTB_1_inst_adr                                                       "0x0029"
#define VO_V1_WADTB_2_inst_adr                                                       "0x002A"
#define VO_V1_WADTB_3_inst_adr                                                       "0x002B"
#define VO_V1_WADTB_4_inst_adr                                                       "0x002C"
#define VO_V1_WADTB_5_inst_adr                                                       "0x002D"
#define VO_V1_WADTB_6_inst_adr                                                       "0x002E"
#define VO_V1_WADTB_7_inst_adr                                                       "0x002F"
#define VO_V1_WADTB_8_inst_adr                                                       "0x0030"
#define VO_V1_WADTB_0_inst                                                           0x0028
#define VO_V1_WADTB_1_inst                                                           0x0029
#define VO_V1_WADTB_2_inst                                                           0x002A
#define VO_V1_WADTB_3_inst                                                           0x002B
#define VO_V1_WADTB_4_inst                                                           0x002C
#define VO_V1_WADTB_5_inst                                                           0x002D
#define VO_V1_WADTB_6_inst                                                           0x002E
#define VO_V1_WADTB_7_inst                                                           0x002F
#define VO_V1_WADTB_8_inst                                                           0x0030
#define VO_V1_WADTB_th_shift                                                         (16)
#define VO_V1_WADTB_th_mask                                                          (0x00FF0000)
#define VO_V1_WADTB_th(data)                                                         (0x00FF0000&((data)<<16))
#define VO_V1_WADTB_th_src(data)                                                     ((0x00FF0000&(data))>>16)
#define VO_V1_WADTB_get_th(data)                                                     ((0x00FF0000&(data))>>16)
#define VO_V1_WADTB_wad_shift                                                        (0)
#define VO_V1_WADTB_wad_mask                                                         (0x00000FFF)
#define VO_V1_WADTB_wad(data)                                                        (0x00000FFF&((data)<<0))
#define VO_V1_WADTB_wad_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_V1_WADTB_get_wad(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_V1_DI_YWIN                                                                0x180050C4
#define VO_V1_DI_YWIN_reg_addr                                                       "0xB80050C4"
#define VO_V1_DI_YWIN_reg                                                            0xB80050C4
#define set_VO_V1_DI_YWIN_reg(data)   (*((volatile unsigned int*) VO_V1_DI_YWIN_reg)=data)
#define get_VO_V1_DI_YWIN_reg   (*((volatile unsigned int*) VO_V1_DI_YWIN_reg))
#define VO_V1_DI_YWIN_inst_adr                                                       "0x0031"
#define VO_V1_DI_YWIN_inst                                                           0x0031
#define VO_V1_DI_YWIN_e_shift                                                        (16)
#define VO_V1_DI_YWIN_e_mask                                                         (0x07FF0000)
#define VO_V1_DI_YWIN_e(data)                                                        (0x07FF0000&((data)<<16))
#define VO_V1_DI_YWIN_e_src(data)                                                    ((0x07FF0000&(data))>>16)
#define VO_V1_DI_YWIN_get_e(data)                                                    ((0x07FF0000&(data))>>16)
#define VO_V1_DI_YWIN_s_shift                                                        (0)
#define VO_V1_DI_YWIN_s_mask                                                         (0x000007FF)
#define VO_V1_DI_YWIN_s(data)                                                        (0x000007FF&((data)<<0))
#define VO_V1_DI_YWIN_s_src(data)                                                    ((0x000007FF&(data))>>0)
#define VO_V1_DI_YWIN_get_s(data)                                                    ((0x000007FF&(data))>>0)


#define VO_V1_DI_CWIN                                                                0x180050C8
#define VO_V1_DI_CWIN_reg_addr                                                       "0xB80050C8"
#define VO_V1_DI_CWIN_reg                                                            0xB80050C8
#define set_VO_V1_DI_CWIN_reg(data)   (*((volatile unsigned int*) VO_V1_DI_CWIN_reg)=data)
#define get_VO_V1_DI_CWIN_reg   (*((volatile unsigned int*) VO_V1_DI_CWIN_reg))
#define VO_V1_DI_CWIN_inst_adr                                                       "0x0032"
#define VO_V1_DI_CWIN_inst                                                           0x0032
#define VO_V1_DI_CWIN_e_shift                                                        (16)
#define VO_V1_DI_CWIN_e_mask                                                         (0x07FF0000)
#define VO_V1_DI_CWIN_e(data)                                                        (0x07FF0000&((data)<<16))
#define VO_V1_DI_CWIN_e_src(data)                                                    ((0x07FF0000&(data))>>16)
#define VO_V1_DI_CWIN_get_e(data)                                                    ((0x07FF0000&(data))>>16)
#define VO_V1_DI_CWIN_s_shift                                                        (0)
#define VO_V1_DI_CWIN_s_mask                                                         (0x000007FF)
#define VO_V1_DI_CWIN_s(data)                                                        (0x000007FF&((data)<<0))
#define VO_V1_DI_CWIN_s_src(data)                                                    ((0x000007FF&(data))>>0)
#define VO_V1_DI_CWIN_get_s(data)                                                    ((0x000007FF&(data))>>0)


#define VO_V1_DI_WINX                                                                0x180050CC
#define VO_V1_DI_WINX_reg_addr                                                       "0xB80050CC"
#define VO_V1_DI_WINX_reg                                                            0xB80050CC
#define set_VO_V1_DI_WINX_reg(data)   (*((volatile unsigned int*) VO_V1_DI_WINX_reg)=data)
#define get_VO_V1_DI_WINX_reg   (*((volatile unsigned int*) VO_V1_DI_WINX_reg))
#define VO_V1_DI_WINX_inst_adr                                                       "0x0033"
#define VO_V1_DI_WINX_inst                                                           0x0033
#define VO_V1_DI_WINX_e_shift                                                        (16)
#define VO_V1_DI_WINX_e_mask                                                         (0x07FF0000)
#define VO_V1_DI_WINX_e(data)                                                        (0x07FF0000&((data)<<16))
#define VO_V1_DI_WINX_e_src(data)                                                    ((0x07FF0000&(data))>>16)
#define VO_V1_DI_WINX_get_e(data)                                                    ((0x07FF0000&(data))>>16)
#define VO_V1_DI_WINX_s_shift                                                        (0)
#define VO_V1_DI_WINX_s_mask                                                         (0x000007FF)
#define VO_V1_DI_WINX_s(data)                                                        (0x000007FF&((data)<<0))
#define VO_V1_DI_WINX_s_src(data)                                                    ((0x000007FF&(data))>>0)
#define VO_V1_DI_WINX_get_s(data)                                                    ((0x000007FF&(data))>>0)


#define VO_V1_FILM_SWAD_0                                                            0x180050D0
#define VO_V1_FILM_SWAD_1                                                            0x180050D4
#define VO_V1_FILM_SWAD_2                                                            0x180050D8
#define VO_V1_FILM_SWAD_3                                                            0x180050DC
#define VO_V1_FILM_SWAD_4                                                            0x180050E0
#define VO_V1_FILM_SWAD_5                                                            0x180050E4
#define VO_V1_FILM_SWAD_0_reg_addr                                                   "0xB80050D0"
#define VO_V1_FILM_SWAD_1_reg_addr                                                   "0xB80050D4"
#define VO_V1_FILM_SWAD_2_reg_addr                                                   "0xB80050D8"
#define VO_V1_FILM_SWAD_3_reg_addr                                                   "0xB80050DC"
#define VO_V1_FILM_SWAD_4_reg_addr                                                   "0xB80050E0"
#define VO_V1_FILM_SWAD_5_reg_addr                                                   "0xB80050E4"
#define VO_V1_FILM_SWAD_0_reg                                                        0xB80050D0
#define VO_V1_FILM_SWAD_1_reg                                                        0xB80050D4
#define VO_V1_FILM_SWAD_2_reg                                                        0xB80050D8
#define VO_V1_FILM_SWAD_3_reg                                                        0xB80050DC
#define VO_V1_FILM_SWAD_4_reg                                                        0xB80050E0
#define VO_V1_FILM_SWAD_5_reg                                                        0xB80050E4
#define set_VO_V1_FILM_SWAD_0_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_SWAD_0_reg)=data)
#define set_VO_V1_FILM_SWAD_1_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_SWAD_1_reg)=data)
#define set_VO_V1_FILM_SWAD_2_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_SWAD_2_reg)=data)
#define set_VO_V1_FILM_SWAD_3_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_SWAD_3_reg)=data)
#define set_VO_V1_FILM_SWAD_4_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_SWAD_4_reg)=data)
#define set_VO_V1_FILM_SWAD_5_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_SWAD_5_reg)=data)
#define get_VO_V1_FILM_SWAD_0_reg   (*((volatile unsigned int*) VO_V1_FILM_SWAD_0_reg))
#define get_VO_V1_FILM_SWAD_1_reg   (*((volatile unsigned int*) VO_V1_FILM_SWAD_1_reg))
#define get_VO_V1_FILM_SWAD_2_reg   (*((volatile unsigned int*) VO_V1_FILM_SWAD_2_reg))
#define get_VO_V1_FILM_SWAD_3_reg   (*((volatile unsigned int*) VO_V1_FILM_SWAD_3_reg))
#define get_VO_V1_FILM_SWAD_4_reg   (*((volatile unsigned int*) VO_V1_FILM_SWAD_4_reg))
#define get_VO_V1_FILM_SWAD_5_reg   (*((volatile unsigned int*) VO_V1_FILM_SWAD_5_reg))
#define VO_V1_FILM_SWAD_0_inst_adr                                                   "0x0034"
#define VO_V1_FILM_SWAD_1_inst_adr                                                   "0x0035"
#define VO_V1_FILM_SWAD_2_inst_adr                                                   "0x0036"
#define VO_V1_FILM_SWAD_3_inst_adr                                                   "0x0037"
#define VO_V1_FILM_SWAD_4_inst_adr                                                   "0x0038"
#define VO_V1_FILM_SWAD_5_inst_adr                                                   "0x0039"
#define VO_V1_FILM_SWAD_0_inst                                                       0x0034
#define VO_V1_FILM_SWAD_1_inst                                                       0x0035
#define VO_V1_FILM_SWAD_2_inst                                                       0x0036
#define VO_V1_FILM_SWAD_3_inst                                                       0x0037
#define VO_V1_FILM_SWAD_4_inst                                                       0x0038
#define VO_V1_FILM_SWAD_5_inst                                                       0x0039
#define VO_V1_FILM_SWAD_swad_shift                                                   (0)
#define VO_V1_FILM_SWAD_swad_mask                                                    (0x01FFFFFF)
#define VO_V1_FILM_SWAD_swad(data)                                                   (0x01FFFFFF&((data)<<0))
#define VO_V1_FILM_SWAD_swad_src(data)                                               ((0x01FFFFFF&(data))>>0)
#define VO_V1_FILM_SWAD_get_swad(data)                                               ((0x01FFFFFF&(data))>>0)


#define VO_V1_FILM_TH_0                                                              0x180050E8
#define VO_V1_FILM_TH_1                                                              0x180050EC
#define VO_V1_FILM_TH_2                                                              0x180050F0
#define VO_V1_FILM_TH_3                                                              0x180050F4
#define VO_V1_FILM_TH_4                                                              0x180050F8
#define VO_V1_FILM_TH_5                                                              0x180050FC
#define VO_V1_FILM_TH_0_reg_addr                                                     "0xB80050E8"
#define VO_V1_FILM_TH_1_reg_addr                                                     "0xB80050EC"
#define VO_V1_FILM_TH_2_reg_addr                                                     "0xB80050F0"
#define VO_V1_FILM_TH_3_reg_addr                                                     "0xB80050F4"
#define VO_V1_FILM_TH_4_reg_addr                                                     "0xB80050F8"
#define VO_V1_FILM_TH_5_reg_addr                                                     "0xB80050FC"
#define VO_V1_FILM_TH_0_reg                                                          0xB80050E8
#define VO_V1_FILM_TH_1_reg                                                          0xB80050EC
#define VO_V1_FILM_TH_2_reg                                                          0xB80050F0
#define VO_V1_FILM_TH_3_reg                                                          0xB80050F4
#define VO_V1_FILM_TH_4_reg                                                          0xB80050F8
#define VO_V1_FILM_TH_5_reg                                                          0xB80050FC
#define set_VO_V1_FILM_TH_0_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_TH_0_reg)=data)
#define set_VO_V1_FILM_TH_1_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_TH_1_reg)=data)
#define set_VO_V1_FILM_TH_2_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_TH_2_reg)=data)
#define set_VO_V1_FILM_TH_3_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_TH_3_reg)=data)
#define set_VO_V1_FILM_TH_4_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_TH_4_reg)=data)
#define set_VO_V1_FILM_TH_5_reg(data)   (*((volatile unsigned int*) VO_V1_FILM_TH_5_reg)=data)
#define get_VO_V1_FILM_TH_0_reg   (*((volatile unsigned int*) VO_V1_FILM_TH_0_reg))
#define get_VO_V1_FILM_TH_1_reg   (*((volatile unsigned int*) VO_V1_FILM_TH_1_reg))
#define get_VO_V1_FILM_TH_2_reg   (*((volatile unsigned int*) VO_V1_FILM_TH_2_reg))
#define get_VO_V1_FILM_TH_3_reg   (*((volatile unsigned int*) VO_V1_FILM_TH_3_reg))
#define get_VO_V1_FILM_TH_4_reg   (*((volatile unsigned int*) VO_V1_FILM_TH_4_reg))
#define get_VO_V1_FILM_TH_5_reg   (*((volatile unsigned int*) VO_V1_FILM_TH_5_reg))
#define VO_V1_FILM_TH_0_inst_adr                                                     "0x003A"
#define VO_V1_FILM_TH_1_inst_adr                                                     "0x003B"
#define VO_V1_FILM_TH_2_inst_adr                                                     "0x003C"
#define VO_V1_FILM_TH_3_inst_adr                                                     "0x003D"
#define VO_V1_FILM_TH_4_inst_adr                                                     "0x003E"
#define VO_V1_FILM_TH_5_inst_adr                                                     "0x003F"
#define VO_V1_FILM_TH_0_inst                                                         0x003A
#define VO_V1_FILM_TH_1_inst                                                         0x003B
#define VO_V1_FILM_TH_2_inst                                                         0x003C
#define VO_V1_FILM_TH_3_inst                                                         0x003D
#define VO_V1_FILM_TH_4_inst                                                         0x003E
#define VO_V1_FILM_TH_5_inst                                                         0x003F
#define VO_V1_FILM_TH_th_shift                                                       (0)
#define VO_V1_FILM_TH_th_mask                                                        (0x01FFFFFF)
#define VO_V1_FILM_TH_th(data)                                                       (0x01FFFFFF&((data)<<0))
#define VO_V1_FILM_TH_th_src(data)                                                   ((0x01FFFFFF&(data))>>0)
#define VO_V1_FILM_TH_get_th(data)                                                   ((0x01FFFFFF&(data))>>0)


#define VO_V1_DI_DBG                                                                 0x18005100
#define VO_V1_DI_DBG_reg_addr                                                        "0xB8005100"
#define VO_V1_DI_DBG_reg                                                             0xB8005100
#define set_VO_V1_DI_DBG_reg(data)   (*((volatile unsigned int*) VO_V1_DI_DBG_reg)=data)
#define get_VO_V1_DI_DBG_reg   (*((volatile unsigned int*) VO_V1_DI_DBG_reg))
#define VO_V1_DI_DBG_inst_adr                                                        "0x0040"
#define VO_V1_DI_DBG_inst                                                            0x0040
#define VO_V1_DI_DBG_mode_shift                                                      (0)
#define VO_V1_DI_DBG_mode_mask                                                       (0x00000007)
#define VO_V1_DI_DBG_mode(data)                                                      (0x00000007&((data)<<0))
#define VO_V1_DI_DBG_mode_src(data)                                                  ((0x00000007&(data))>>0)
#define VO_V1_DI_DBG_get_mode(data)                                                  ((0x00000007&(data))>>0)


#define VO_V1_STA_TOTAL                                                              0x18005104
#define VO_V1_STA_TOTAL_reg_addr                                                     "0xB8005104"
#define VO_V1_STA_TOTAL_reg                                                          0xB8005104
#define set_VO_V1_STA_TOTAL_reg(data)   (*((volatile unsigned int*) VO_V1_STA_TOTAL_reg)=data)
#define get_VO_V1_STA_TOTAL_reg   (*((volatile unsigned int*) VO_V1_STA_TOTAL_reg))
#define VO_V1_STA_TOTAL_inst_adr                                                     "0x0041"
#define VO_V1_STA_TOTAL_inst                                                         0x0041
#define VO_V1_STA_TOTAL_cnt_shift                                                    (0)
#define VO_V1_STA_TOTAL_cnt_mask                                                     (0x001FFFFF)
#define VO_V1_STA_TOTAL_cnt(data)                                                    (0x001FFFFF&((data)<<0))
#define VO_V1_STA_TOTAL_cnt_src(data)                                                ((0x001FFFFF&(data))>>0)
#define VO_V1_STA_TOTAL_get_cnt(data)                                                ((0x001FFFFF&(data))>>0)


#define VO_V1_STA_SUM                                                                0x18005108
#define VO_V1_STA_SUM_reg_addr                                                       "0xB8005108"
#define VO_V1_STA_SUM_reg                                                            0xB8005108
#define set_VO_V1_STA_SUM_reg(data)   (*((volatile unsigned int*) VO_V1_STA_SUM_reg)=data)
#define get_VO_V1_STA_SUM_reg   (*((volatile unsigned int*) VO_V1_STA_SUM_reg))
#define VO_V1_STA_SUM_inst_adr                                                       "0x0042"
#define VO_V1_STA_SUM_inst                                                           0x0042
#define VO_V1_STA_SUM_sum_shift                                                      (0)
#define VO_V1_STA_SUM_sum_mask                                                       (0x1FFFFFFF)
#define VO_V1_STA_SUM_sum(data)                                                      (0x1FFFFFFF&((data)<<0))
#define VO_V1_STA_SUM_sum_src(data)                                                  ((0x1FFFFFFF&(data))>>0)
#define VO_V1_STA_SUM_get_sum(data)                                                  ((0x1FFFFFFF&(data))>>0)


#define VO_V1_STA_BKT_0                                                              0x1800510C
#define VO_V1_STA_BKT_1                                                              0x18005110
#define VO_V1_STA_BKT_2                                                              0x18005114
#define VO_V1_STA_BKT_3                                                              0x18005118
#define VO_V1_STA_BKT_4                                                              0x1800511C
#define VO_V1_STA_BKT_5                                                              0x18005120
#define VO_V1_STA_BKT_6                                                              0x18005124
#define VO_V1_STA_BKT_7                                                              0x18005128
#define VO_V1_STA_BKT_8                                                              0x1800512C
#define VO_V1_STA_BKT_9                                                              0x18005130
#define VO_V1_STA_BKT_0_reg_addr                                                     "0xB800510C"
#define VO_V1_STA_BKT_1_reg_addr                                                     "0xB8005110"
#define VO_V1_STA_BKT_2_reg_addr                                                     "0xB8005114"
#define VO_V1_STA_BKT_3_reg_addr                                                     "0xB8005118"
#define VO_V1_STA_BKT_4_reg_addr                                                     "0xB800511C"
#define VO_V1_STA_BKT_5_reg_addr                                                     "0xB8005120"
#define VO_V1_STA_BKT_6_reg_addr                                                     "0xB8005124"
#define VO_V1_STA_BKT_7_reg_addr                                                     "0xB8005128"
#define VO_V1_STA_BKT_8_reg_addr                                                     "0xB800512C"
#define VO_V1_STA_BKT_9_reg_addr                                                     "0xB8005130"
#define VO_V1_STA_BKT_0_reg                                                          0xB800510C
#define VO_V1_STA_BKT_1_reg                                                          0xB8005110
#define VO_V1_STA_BKT_2_reg                                                          0xB8005114
#define VO_V1_STA_BKT_3_reg                                                          0xB8005118
#define VO_V1_STA_BKT_4_reg                                                          0xB800511C
#define VO_V1_STA_BKT_5_reg                                                          0xB8005120
#define VO_V1_STA_BKT_6_reg                                                          0xB8005124
#define VO_V1_STA_BKT_7_reg                                                          0xB8005128
#define VO_V1_STA_BKT_8_reg                                                          0xB800512C
#define VO_V1_STA_BKT_9_reg                                                          0xB8005130
#define set_VO_V1_STA_BKT_0_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_0_reg)=data)
#define set_VO_V1_STA_BKT_1_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_1_reg)=data)
#define set_VO_V1_STA_BKT_2_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_2_reg)=data)
#define set_VO_V1_STA_BKT_3_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_3_reg)=data)
#define set_VO_V1_STA_BKT_4_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_4_reg)=data)
#define set_VO_V1_STA_BKT_5_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_5_reg)=data)
#define set_VO_V1_STA_BKT_6_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_6_reg)=data)
#define set_VO_V1_STA_BKT_7_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_7_reg)=data)
#define set_VO_V1_STA_BKT_8_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_8_reg)=data)
#define set_VO_V1_STA_BKT_9_reg(data)   (*((volatile unsigned int*) VO_V1_STA_BKT_9_reg)=data)
#define get_VO_V1_STA_BKT_0_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_0_reg))
#define get_VO_V1_STA_BKT_1_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_1_reg))
#define get_VO_V1_STA_BKT_2_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_2_reg))
#define get_VO_V1_STA_BKT_3_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_3_reg))
#define get_VO_V1_STA_BKT_4_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_4_reg))
#define get_VO_V1_STA_BKT_5_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_5_reg))
#define get_VO_V1_STA_BKT_6_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_6_reg))
#define get_VO_V1_STA_BKT_7_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_7_reg))
#define get_VO_V1_STA_BKT_8_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_8_reg))
#define get_VO_V1_STA_BKT_9_reg   (*((volatile unsigned int*) VO_V1_STA_BKT_9_reg))
#define VO_V1_STA_BKT_0_inst_adr                                                     "0x0043"
#define VO_V1_STA_BKT_1_inst_adr                                                     "0x0044"
#define VO_V1_STA_BKT_2_inst_adr                                                     "0x0045"
#define VO_V1_STA_BKT_3_inst_adr                                                     "0x0046"
#define VO_V1_STA_BKT_4_inst_adr                                                     "0x0047"
#define VO_V1_STA_BKT_5_inst_adr                                                     "0x0048"
#define VO_V1_STA_BKT_6_inst_adr                                                     "0x0049"
#define VO_V1_STA_BKT_7_inst_adr                                                     "0x004A"
#define VO_V1_STA_BKT_8_inst_adr                                                     "0x004B"
#define VO_V1_STA_BKT_9_inst_adr                                                     "0x004C"
#define VO_V1_STA_BKT_0_inst                                                         0x0043
#define VO_V1_STA_BKT_1_inst                                                         0x0044
#define VO_V1_STA_BKT_2_inst                                                         0x0045
#define VO_V1_STA_BKT_3_inst                                                         0x0046
#define VO_V1_STA_BKT_4_inst                                                         0x0047
#define VO_V1_STA_BKT_5_inst                                                         0x0048
#define VO_V1_STA_BKT_6_inst                                                         0x0049
#define VO_V1_STA_BKT_7_inst                                                         0x004A
#define VO_V1_STA_BKT_8_inst                                                         0x004B
#define VO_V1_STA_BKT_9_inst                                                         0x004C
#define VO_V1_STA_BKT_cnt_shift                                                      (0)
#define VO_V1_STA_BKT_cnt_mask                                                       (0x001FFFFF)
#define VO_V1_STA_BKT_cnt(data)                                                      (0x001FFFFF&((data)<<0))
#define VO_V1_STA_BKT_cnt_src(data)                                                  ((0x001FFFFF&(data))>>0)
#define VO_V1_STA_BKT_get_cnt(data)                                                  ((0x001FFFFF&(data))>>0)


#define VO_V1_STA_AD_0                                                               0x18005134
#define VO_V1_STA_AD_1                                                               0x18005138
#define VO_V1_STA_AD_2                                                               0x1800513C
#define VO_V1_STA_AD_3                                                               0x18005140
#define VO_V1_STA_AD_4                                                               0x18005144
#define VO_V1_STA_AD_5                                                               0x18005148
#define VO_V1_STA_AD_6                                                               0x1800514C
#define VO_V1_STA_AD_7                                                               0x18005150
#define VO_V1_STA_AD_8                                                               0x18005154
#define VO_V1_STA_AD_9                                                               0x18005158
#define VO_V1_STA_AD_0_reg_addr                                                      "0xB8005134"
#define VO_V1_STA_AD_1_reg_addr                                                      "0xB8005138"
#define VO_V1_STA_AD_2_reg_addr                                                      "0xB800513C"
#define VO_V1_STA_AD_3_reg_addr                                                      "0xB8005140"
#define VO_V1_STA_AD_4_reg_addr                                                      "0xB8005144"
#define VO_V1_STA_AD_5_reg_addr                                                      "0xB8005148"
#define VO_V1_STA_AD_6_reg_addr                                                      "0xB800514C"
#define VO_V1_STA_AD_7_reg_addr                                                      "0xB8005150"
#define VO_V1_STA_AD_8_reg_addr                                                      "0xB8005154"
#define VO_V1_STA_AD_9_reg_addr                                                      "0xB8005158"
#define VO_V1_STA_AD_0_reg                                                           0xB8005134
#define VO_V1_STA_AD_1_reg                                                           0xB8005138
#define VO_V1_STA_AD_2_reg                                                           0xB800513C
#define VO_V1_STA_AD_3_reg                                                           0xB8005140
#define VO_V1_STA_AD_4_reg                                                           0xB8005144
#define VO_V1_STA_AD_5_reg                                                           0xB8005148
#define VO_V1_STA_AD_6_reg                                                           0xB800514C
#define VO_V1_STA_AD_7_reg                                                           0xB8005150
#define VO_V1_STA_AD_8_reg                                                           0xB8005154
#define VO_V1_STA_AD_9_reg                                                           0xB8005158
#define set_VO_V1_STA_AD_0_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_0_reg)=data)
#define set_VO_V1_STA_AD_1_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_1_reg)=data)
#define set_VO_V1_STA_AD_2_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_2_reg)=data)
#define set_VO_V1_STA_AD_3_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_3_reg)=data)
#define set_VO_V1_STA_AD_4_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_4_reg)=data)
#define set_VO_V1_STA_AD_5_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_5_reg)=data)
#define set_VO_V1_STA_AD_6_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_6_reg)=data)
#define set_VO_V1_STA_AD_7_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_7_reg)=data)
#define set_VO_V1_STA_AD_8_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_8_reg)=data)
#define set_VO_V1_STA_AD_9_reg(data)   (*((volatile unsigned int*) VO_V1_STA_AD_9_reg)=data)
#define get_VO_V1_STA_AD_0_reg   (*((volatile unsigned int*) VO_V1_STA_AD_0_reg))
#define get_VO_V1_STA_AD_1_reg   (*((volatile unsigned int*) VO_V1_STA_AD_1_reg))
#define get_VO_V1_STA_AD_2_reg   (*((volatile unsigned int*) VO_V1_STA_AD_2_reg))
#define get_VO_V1_STA_AD_3_reg   (*((volatile unsigned int*) VO_V1_STA_AD_3_reg))
#define get_VO_V1_STA_AD_4_reg   (*((volatile unsigned int*) VO_V1_STA_AD_4_reg))
#define get_VO_V1_STA_AD_5_reg   (*((volatile unsigned int*) VO_V1_STA_AD_5_reg))
#define get_VO_V1_STA_AD_6_reg   (*((volatile unsigned int*) VO_V1_STA_AD_6_reg))
#define get_VO_V1_STA_AD_7_reg   (*((volatile unsigned int*) VO_V1_STA_AD_7_reg))
#define get_VO_V1_STA_AD_8_reg   (*((volatile unsigned int*) VO_V1_STA_AD_8_reg))
#define get_VO_V1_STA_AD_9_reg   (*((volatile unsigned int*) VO_V1_STA_AD_9_reg))
#define VO_V1_STA_AD_0_inst_adr                                                      "0x004D"
#define VO_V1_STA_AD_1_inst_adr                                                      "0x004E"
#define VO_V1_STA_AD_2_inst_adr                                                      "0x004F"
#define VO_V1_STA_AD_3_inst_adr                                                      "0x0050"
#define VO_V1_STA_AD_4_inst_adr                                                      "0x0051"
#define VO_V1_STA_AD_5_inst_adr                                                      "0x0052"
#define VO_V1_STA_AD_6_inst_adr                                                      "0x0053"
#define VO_V1_STA_AD_7_inst_adr                                                      "0x0054"
#define VO_V1_STA_AD_8_inst_adr                                                      "0x0055"
#define VO_V1_STA_AD_9_inst_adr                                                      "0x0056"
#define VO_V1_STA_AD_0_inst                                                          0x004D
#define VO_V1_STA_AD_1_inst                                                          0x004E
#define VO_V1_STA_AD_2_inst                                                          0x004F
#define VO_V1_STA_AD_3_inst                                                          0x0050
#define VO_V1_STA_AD_4_inst                                                          0x0051
#define VO_V1_STA_AD_5_inst                                                          0x0052
#define VO_V1_STA_AD_6_inst                                                          0x0053
#define VO_V1_STA_AD_7_inst                                                          0x0054
#define VO_V1_STA_AD_8_inst                                                          0x0055
#define VO_V1_STA_AD_9_inst                                                          0x0056
#define VO_V1_STA_AD_sum_shift                                                       (0)
#define VO_V1_STA_AD_sum_mask                                                        (0x1FFFFFFF)
#define VO_V1_STA_AD_sum(data)                                                       (0x1FFFFFFF&((data)<<0))
#define VO_V1_STA_AD_sum_src(data)                                                   ((0x1FFFFFFF&(data))>>0)
#define VO_V1_STA_AD_get_sum(data)                                                   ((0x1FFFFFFF&(data))>>0)


#define VO_V1_VSYI                                                                   0x1800515C
#define VO_V1_VSYI_reg_addr                                                          "0xB800515C"
#define VO_V1_VSYI_reg                                                               0xB800515C
#define set_VO_V1_VSYI_reg(data)   (*((volatile unsigned int*) VO_V1_VSYI_reg)=data)
#define get_VO_V1_VSYI_reg   (*((volatile unsigned int*) VO_V1_VSYI_reg))
#define VO_V1_VSYI_inst_adr                                                          "0x0057"
#define VO_V1_VSYI_inst                                                              0x0057
#define VO_V1_VSYI_offset_shift                                                      (16)
#define VO_V1_VSYI_offset_mask                                                       (0x0FFF0000)
#define VO_V1_VSYI_offset(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_V1_VSYI_offset_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V1_VSYI_get_offset(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V1_VSYI_phase_shift                                                       (2)
#define VO_V1_VSYI_phase_mask                                                        (0x0000FFFC)
#define VO_V1_VSYI_phase(data)                                                       (0x0000FFFC&((data)<<2))
#define VO_V1_VSYI_phase_src(data)                                                   ((0x0000FFFC&(data))>>2)
#define VO_V1_VSYI_get_phase(data)                                                   ((0x0000FFFC&(data))>>2)


#define VO_V1_VSCI                                                                   0x18005160
#define VO_V1_VSCI_reg_addr                                                          "0xB8005160"
#define VO_V1_VSCI_reg                                                               0xB8005160
#define set_VO_V1_VSCI_reg(data)   (*((volatile unsigned int*) VO_V1_VSCI_reg)=data)
#define get_VO_V1_VSCI_reg   (*((volatile unsigned int*) VO_V1_VSCI_reg))
#define VO_V1_VSCI_inst_adr                                                          "0x0058"
#define VO_V1_VSCI_inst                                                              0x0058
#define VO_V1_VSCI_offset_shift                                                      (16)
#define VO_V1_VSCI_offset_mask                                                       (0x0FFF0000)
#define VO_V1_VSCI_offset(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_V1_VSCI_offset_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V1_VSCI_get_offset(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V1_VSCI_phase_shift                                                       (2)
#define VO_V1_VSCI_phase_mask                                                        (0x0000FFFC)
#define VO_V1_VSCI_phase(data)                                                       (0x0000FFFC&((data)<<2))
#define VO_V1_VSCI_phase_src(data)                                                   ((0x0000FFFC&(data))>>2)
#define VO_V1_VSCI_get_phase(data)                                                   ((0x0000FFFC&(data))>>2)


#define VO_V1_VSD                                                                    0x18005164
#define VO_V1_VSD_reg_addr                                                           "0xB8005164"
#define VO_V1_VSD_reg                                                                0xB8005164
#define set_VO_V1_VSD_reg(data)   (*((volatile unsigned int*) VO_V1_VSD_reg)=data)
#define get_VO_V1_VSD_reg   (*((volatile unsigned int*) VO_V1_VSD_reg))
#define VO_V1_VSD_inst_adr                                                           "0x0059"
#define VO_V1_VSD_inst                                                               0x0059
#define VO_V1_VSD_out_shift                                                          (20)
#define VO_V1_VSD_out_mask                                                           (0x7FF00000)
#define VO_V1_VSD_out(data)                                                          (0x7FF00000&((data)<<20))
#define VO_V1_VSD_out_src(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V1_VSD_get_out(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V1_VSD_delta_shift                                                        (0)
#define VO_V1_VSD_delta_mask                                                         (0x0007FFFF)
#define VO_V1_VSD_delta(data)                                                        (0x0007FFFF&((data)<<0))
#define VO_V1_VSD_delta_src(data)                                                    ((0x0007FFFF&(data))>>0)
#define VO_V1_VSD_get_delta(data)                                                    ((0x0007FFFF&(data))>>0)


#define VO_V1_VSYC_0                                                                 0x18005168
#define VO_V1_VSYC_1                                                                 0x1800516C
#define VO_V1_VSYC_2                                                                 0x18005170
#define VO_V1_VSYC_3                                                                 0x18005174
#define VO_V1_VSYC_4                                                                 0x18005178
#define VO_V1_VSYC_5                                                                 0x1800517C
#define VO_V1_VSYC_6                                                                 0x18005180
#define VO_V1_VSYC_7                                                                 0x18005184
#define VO_V1_VSYC_8                                                                 0x18005188
#define VO_V1_VSYC_9                                                                 0x1800518C
#define VO_V1_VSYC_10                                                                 0x18005190
#define VO_V1_VSYC_11                                                                 0x18005194
#define VO_V1_VSYC_12                                                                 0x18005198
#define VO_V1_VSYC_13                                                                 0x1800519C
#define VO_V1_VSYC_14                                                                 0x180051A0
#define VO_V1_VSYC_15                                                                 0x180051A4
#define VO_V1_VSYC_0_reg_addr                                                        "0xB8005168"
#define VO_V1_VSYC_1_reg_addr                                                        "0xB800516C"
#define VO_V1_VSYC_2_reg_addr                                                        "0xB8005170"
#define VO_V1_VSYC_3_reg_addr                                                        "0xB8005174"
#define VO_V1_VSYC_4_reg_addr                                                        "0xB8005178"
#define VO_V1_VSYC_5_reg_addr                                                        "0xB800517C"
#define VO_V1_VSYC_6_reg_addr                                                        "0xB8005180"
#define VO_V1_VSYC_7_reg_addr                                                        "0xB8005184"
#define VO_V1_VSYC_8_reg_addr                                                        "0xB8005188"
#define VO_V1_VSYC_9_reg_addr                                                        "0xB800518C"
#define VO_V1_VSYC_10_reg_addr                                                        "0xB8005190"
#define VO_V1_VSYC_11_reg_addr                                                        "0xB8005194"
#define VO_V1_VSYC_12_reg_addr                                                        "0xB8005198"
#define VO_V1_VSYC_13_reg_addr                                                        "0xB800519C"
#define VO_V1_VSYC_14_reg_addr                                                        "0xB80051A0"
#define VO_V1_VSYC_15_reg_addr                                                        "0xB80051A4"
#define VO_V1_VSYC_0_reg                                                             0xB8005168
#define VO_V1_VSYC_1_reg                                                             0xB800516C
#define VO_V1_VSYC_2_reg                                                             0xB8005170
#define VO_V1_VSYC_3_reg                                                             0xB8005174
#define VO_V1_VSYC_4_reg                                                             0xB8005178
#define VO_V1_VSYC_5_reg                                                             0xB800517C
#define VO_V1_VSYC_6_reg                                                             0xB8005180
#define VO_V1_VSYC_7_reg                                                             0xB8005184
#define VO_V1_VSYC_8_reg                                                             0xB8005188
#define VO_V1_VSYC_9_reg                                                             0xB800518C
#define VO_V1_VSYC_10_reg                                                             0xB8005190
#define VO_V1_VSYC_11_reg                                                             0xB8005194
#define VO_V1_VSYC_12_reg                                                             0xB8005198
#define VO_V1_VSYC_13_reg                                                             0xB800519C
#define VO_V1_VSYC_14_reg                                                             0xB80051A0
#define VO_V1_VSYC_15_reg                                                             0xB80051A4
#define set_VO_V1_VSYC_0_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_0_reg)=data)
#define set_VO_V1_VSYC_1_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_1_reg)=data)
#define set_VO_V1_VSYC_2_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_2_reg)=data)
#define set_VO_V1_VSYC_3_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_3_reg)=data)
#define set_VO_V1_VSYC_4_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_4_reg)=data)
#define set_VO_V1_VSYC_5_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_5_reg)=data)
#define set_VO_V1_VSYC_6_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_6_reg)=data)
#define set_VO_V1_VSYC_7_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_7_reg)=data)
#define set_VO_V1_VSYC_8_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_8_reg)=data)
#define set_VO_V1_VSYC_9_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_9_reg)=data)
#define set_VO_V1_VSYC_10_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_10_reg)=data)
#define set_VO_V1_VSYC_11_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_11_reg)=data)
#define set_VO_V1_VSYC_12_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_12_reg)=data)
#define set_VO_V1_VSYC_13_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_13_reg)=data)
#define set_VO_V1_VSYC_14_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_14_reg)=data)
#define set_VO_V1_VSYC_15_reg(data)   (*((volatile unsigned int*) VO_V1_VSYC_15_reg)=data)
#define get_VO_V1_VSYC_0_reg   (*((volatile unsigned int*) VO_V1_VSYC_0_reg))
#define get_VO_V1_VSYC_1_reg   (*((volatile unsigned int*) VO_V1_VSYC_1_reg))
#define get_VO_V1_VSYC_2_reg   (*((volatile unsigned int*) VO_V1_VSYC_2_reg))
#define get_VO_V1_VSYC_3_reg   (*((volatile unsigned int*) VO_V1_VSYC_3_reg))
#define get_VO_V1_VSYC_4_reg   (*((volatile unsigned int*) VO_V1_VSYC_4_reg))
#define get_VO_V1_VSYC_5_reg   (*((volatile unsigned int*) VO_V1_VSYC_5_reg))
#define get_VO_V1_VSYC_6_reg   (*((volatile unsigned int*) VO_V1_VSYC_6_reg))
#define get_VO_V1_VSYC_7_reg   (*((volatile unsigned int*) VO_V1_VSYC_7_reg))
#define get_VO_V1_VSYC_8_reg   (*((volatile unsigned int*) VO_V1_VSYC_8_reg))
#define get_VO_V1_VSYC_9_reg   (*((volatile unsigned int*) VO_V1_VSYC_9_reg))
#define get_VO_V1_VSYC_10_reg   (*((volatile unsigned int*) VO_V1_VSYC_10_reg))
#define get_VO_V1_VSYC_11_reg   (*((volatile unsigned int*) VO_V1_VSYC_11_reg))
#define get_VO_V1_VSYC_12_reg   (*((volatile unsigned int*) VO_V1_VSYC_12_reg))
#define get_VO_V1_VSYC_13_reg   (*((volatile unsigned int*) VO_V1_VSYC_13_reg))
#define get_VO_V1_VSYC_14_reg   (*((volatile unsigned int*) VO_V1_VSYC_14_reg))
#define get_VO_V1_VSYC_15_reg   (*((volatile unsigned int*) VO_V1_VSYC_15_reg))
#define VO_V1_VSYC_0_inst_adr                                                        "0x005A"
#define VO_V1_VSYC_1_inst_adr                                                        "0x005B"
#define VO_V1_VSYC_2_inst_adr                                                        "0x005C"
#define VO_V1_VSYC_3_inst_adr                                                        "0x005D"
#define VO_V1_VSYC_4_inst_adr                                                        "0x005E"
#define VO_V1_VSYC_5_inst_adr                                                        "0x005F"
#define VO_V1_VSYC_6_inst_adr                                                        "0x0060"
#define VO_V1_VSYC_7_inst_adr                                                        "0x0061"
#define VO_V1_VSYC_8_inst_adr                                                        "0x0062"
#define VO_V1_VSYC_9_inst_adr                                                        "0x0063"
#define VO_V1_VSYC_10_inst_adr                                                        "0x0064"
#define VO_V1_VSYC_11_inst_adr                                                        "0x0065"
#define VO_V1_VSYC_12_inst_adr                                                        "0x0066"
#define VO_V1_VSYC_13_inst_adr                                                        "0x0067"
#define VO_V1_VSYC_14_inst_adr                                                        "0x0068"
#define VO_V1_VSYC_15_inst_adr                                                        "0x0069"
#define VO_V1_VSYC_0_inst                                                            0x005A
#define VO_V1_VSYC_1_inst                                                            0x005B
#define VO_V1_VSYC_2_inst                                                            0x005C
#define VO_V1_VSYC_3_inst                                                            0x005D
#define VO_V1_VSYC_4_inst                                                            0x005E
#define VO_V1_VSYC_5_inst                                                            0x005F
#define VO_V1_VSYC_6_inst                                                            0x0060
#define VO_V1_VSYC_7_inst                                                            0x0061
#define VO_V1_VSYC_8_inst                                                            0x0062
#define VO_V1_VSYC_9_inst                                                            0x0063
#define VO_V1_VSYC_10_inst                                                            0x0064
#define VO_V1_VSYC_11_inst                                                            0x0065
#define VO_V1_VSYC_12_inst                                                            0x0066
#define VO_V1_VSYC_13_inst                                                            0x0067
#define VO_V1_VSYC_14_inst                                                            0x0068
#define VO_V1_VSYC_15_inst                                                            0x0069
#define VO_V1_VSYC_c1_shift                                                          (16)
#define VO_V1_VSYC_c1_mask                                                           (0x3FFF0000)
#define VO_V1_VSYC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V1_VSYC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_VSYC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_VSYC_c0_shift                                                          (0)
#define VO_V1_VSYC_c0_mask                                                           (0x00003FFF)
#define VO_V1_VSYC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V1_VSYC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V1_VSYC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V1_VSCC_0                                                                 0x180051A8
#define VO_V1_VSCC_1                                                                 0x180051AC
#define VO_V1_VSCC_2                                                                 0x180051B0
#define VO_V1_VSCC_3                                                                 0x180051B4
#define VO_V1_VSCC_4                                                                 0x180051B8
#define VO_V1_VSCC_5                                                                 0x180051BC
#define VO_V1_VSCC_6                                                                 0x180051C0
#define VO_V1_VSCC_7                                                                 0x180051C4
#define VO_V1_VSCC_8                                                                 0x180051C8
#define VO_V1_VSCC_9                                                                 0x180051CC
#define VO_V1_VSCC_10                                                                 0x180051D0
#define VO_V1_VSCC_11                                                                 0x180051D4
#define VO_V1_VSCC_12                                                                 0x180051D8
#define VO_V1_VSCC_13                                                                 0x180051DC
#define VO_V1_VSCC_14                                                                 0x180051E0
#define VO_V1_VSCC_15                                                                 0x180051E4
#define VO_V1_VSCC_0_reg_addr                                                        "0xB80051A8"
#define VO_V1_VSCC_1_reg_addr                                                        "0xB80051AC"
#define VO_V1_VSCC_2_reg_addr                                                        "0xB80051B0"
#define VO_V1_VSCC_3_reg_addr                                                        "0xB80051B4"
#define VO_V1_VSCC_4_reg_addr                                                        "0xB80051B8"
#define VO_V1_VSCC_5_reg_addr                                                        "0xB80051BC"
#define VO_V1_VSCC_6_reg_addr                                                        "0xB80051C0"
#define VO_V1_VSCC_7_reg_addr                                                        "0xB80051C4"
#define VO_V1_VSCC_8_reg_addr                                                        "0xB80051C8"
#define VO_V1_VSCC_9_reg_addr                                                        "0xB80051CC"
#define VO_V1_VSCC_10_reg_addr                                                        "0xB80051D0"
#define VO_V1_VSCC_11_reg_addr                                                        "0xB80051D4"
#define VO_V1_VSCC_12_reg_addr                                                        "0xB80051D8"
#define VO_V1_VSCC_13_reg_addr                                                        "0xB80051DC"
#define VO_V1_VSCC_14_reg_addr                                                        "0xB80051E0"
#define VO_V1_VSCC_15_reg_addr                                                        "0xB80051E4"
#define VO_V1_VSCC_0_reg                                                             0xB80051A8
#define VO_V1_VSCC_1_reg                                                             0xB80051AC
#define VO_V1_VSCC_2_reg                                                             0xB80051B0
#define VO_V1_VSCC_3_reg                                                             0xB80051B4
#define VO_V1_VSCC_4_reg                                                             0xB80051B8
#define VO_V1_VSCC_5_reg                                                             0xB80051BC
#define VO_V1_VSCC_6_reg                                                             0xB80051C0
#define VO_V1_VSCC_7_reg                                                             0xB80051C4
#define VO_V1_VSCC_8_reg                                                             0xB80051C8
#define VO_V1_VSCC_9_reg                                                             0xB80051CC
#define VO_V1_VSCC_10_reg                                                             0xB80051D0
#define VO_V1_VSCC_11_reg                                                             0xB80051D4
#define VO_V1_VSCC_12_reg                                                             0xB80051D8
#define VO_V1_VSCC_13_reg                                                             0xB80051DC
#define VO_V1_VSCC_14_reg                                                             0xB80051E0
#define VO_V1_VSCC_15_reg                                                             0xB80051E4
#define set_VO_V1_VSCC_0_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_0_reg)=data)
#define set_VO_V1_VSCC_1_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_1_reg)=data)
#define set_VO_V1_VSCC_2_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_2_reg)=data)
#define set_VO_V1_VSCC_3_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_3_reg)=data)
#define set_VO_V1_VSCC_4_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_4_reg)=data)
#define set_VO_V1_VSCC_5_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_5_reg)=data)
#define set_VO_V1_VSCC_6_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_6_reg)=data)
#define set_VO_V1_VSCC_7_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_7_reg)=data)
#define set_VO_V1_VSCC_8_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_8_reg)=data)
#define set_VO_V1_VSCC_9_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_9_reg)=data)
#define set_VO_V1_VSCC_10_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_10_reg)=data)
#define set_VO_V1_VSCC_11_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_11_reg)=data)
#define set_VO_V1_VSCC_12_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_12_reg)=data)
#define set_VO_V1_VSCC_13_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_13_reg)=data)
#define set_VO_V1_VSCC_14_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_14_reg)=data)
#define set_VO_V1_VSCC_15_reg(data)   (*((volatile unsigned int*) VO_V1_VSCC_15_reg)=data)
#define get_VO_V1_VSCC_0_reg   (*((volatile unsigned int*) VO_V1_VSCC_0_reg))
#define get_VO_V1_VSCC_1_reg   (*((volatile unsigned int*) VO_V1_VSCC_1_reg))
#define get_VO_V1_VSCC_2_reg   (*((volatile unsigned int*) VO_V1_VSCC_2_reg))
#define get_VO_V1_VSCC_3_reg   (*((volatile unsigned int*) VO_V1_VSCC_3_reg))
#define get_VO_V1_VSCC_4_reg   (*((volatile unsigned int*) VO_V1_VSCC_4_reg))
#define get_VO_V1_VSCC_5_reg   (*((volatile unsigned int*) VO_V1_VSCC_5_reg))
#define get_VO_V1_VSCC_6_reg   (*((volatile unsigned int*) VO_V1_VSCC_6_reg))
#define get_VO_V1_VSCC_7_reg   (*((volatile unsigned int*) VO_V1_VSCC_7_reg))
#define get_VO_V1_VSCC_8_reg   (*((volatile unsigned int*) VO_V1_VSCC_8_reg))
#define get_VO_V1_VSCC_9_reg   (*((volatile unsigned int*) VO_V1_VSCC_9_reg))
#define get_VO_V1_VSCC_10_reg   (*((volatile unsigned int*) VO_V1_VSCC_10_reg))
#define get_VO_V1_VSCC_11_reg   (*((volatile unsigned int*) VO_V1_VSCC_11_reg))
#define get_VO_V1_VSCC_12_reg   (*((volatile unsigned int*) VO_V1_VSCC_12_reg))
#define get_VO_V1_VSCC_13_reg   (*((volatile unsigned int*) VO_V1_VSCC_13_reg))
#define get_VO_V1_VSCC_14_reg   (*((volatile unsigned int*) VO_V1_VSCC_14_reg))
#define get_VO_V1_VSCC_15_reg   (*((volatile unsigned int*) VO_V1_VSCC_15_reg))
#define VO_V1_VSCC_0_inst_adr                                                        "0x006A"
#define VO_V1_VSCC_1_inst_adr                                                        "0x006B"
#define VO_V1_VSCC_2_inst_adr                                                        "0x006C"
#define VO_V1_VSCC_3_inst_adr                                                        "0x006D"
#define VO_V1_VSCC_4_inst_adr                                                        "0x006E"
#define VO_V1_VSCC_5_inst_adr                                                        "0x006F"
#define VO_V1_VSCC_6_inst_adr                                                        "0x0070"
#define VO_V1_VSCC_7_inst_adr                                                        "0x0071"
#define VO_V1_VSCC_8_inst_adr                                                        "0x0072"
#define VO_V1_VSCC_9_inst_adr                                                        "0x0073"
#define VO_V1_VSCC_10_inst_adr                                                        "0x0074"
#define VO_V1_VSCC_11_inst_adr                                                        "0x0075"
#define VO_V1_VSCC_12_inst_adr                                                        "0x0076"
#define VO_V1_VSCC_13_inst_adr                                                        "0x0077"
#define VO_V1_VSCC_14_inst_adr                                                        "0x0078"
#define VO_V1_VSCC_15_inst_adr                                                        "0x0079"
#define VO_V1_VSCC_0_inst                                                            0x006A
#define VO_V1_VSCC_1_inst                                                            0x006B
#define VO_V1_VSCC_2_inst                                                            0x006C
#define VO_V1_VSCC_3_inst                                                            0x006D
#define VO_V1_VSCC_4_inst                                                            0x006E
#define VO_V1_VSCC_5_inst                                                            0x006F
#define VO_V1_VSCC_6_inst                                                            0x0070
#define VO_V1_VSCC_7_inst                                                            0x0071
#define VO_V1_VSCC_8_inst                                                            0x0072
#define VO_V1_VSCC_9_inst                                                            0x0073
#define VO_V1_VSCC_10_inst                                                            0x0074
#define VO_V1_VSCC_11_inst                                                            0x0075
#define VO_V1_VSCC_12_inst                                                            0x0076
#define VO_V1_VSCC_13_inst                                                            0x0077
#define VO_V1_VSCC_14_inst                                                            0x0078
#define VO_V1_VSCC_15_inst                                                            0x0079
#define VO_V1_VSCC_c1_shift                                                          (16)
#define VO_V1_VSCC_c1_mask                                                           (0x3FFF0000)
#define VO_V1_VSCC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V1_VSCC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_VSCC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_VSCC_c0_shift                                                          (0)
#define VO_V1_VSCC_c0_mask                                                           (0x00003FFF)
#define VO_V1_VSCC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V1_VSCC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V1_VSCC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V1_HSI                                                                    0x180051E8
#define VO_V1_HSI_reg_addr                                                           "0xB80051E8"
#define VO_V1_HSI_reg                                                                0xB80051E8
#define set_VO_V1_HSI_reg(data)   (*((volatile unsigned int*) VO_V1_HSI_reg)=data)
#define get_VO_V1_HSI_reg   (*((volatile unsigned int*) VO_V1_HSI_reg))
#define VO_V1_HSI_inst_adr                                                           "0x007A"
#define VO_V1_HSI_inst                                                               0x007A
#define VO_V1_HSI_offset_shift                                                       (16)
#define VO_V1_HSI_offset_mask                                                        (0x07FF0000)
#define VO_V1_HSI_offset(data)                                                       (0x07FF0000&((data)<<16))
#define VO_V1_HSI_offset_src(data)                                                   ((0x07FF0000&(data))>>16)
#define VO_V1_HSI_get_offset(data)                                                   ((0x07FF0000&(data))>>16)
#define VO_V1_HSI_phase_shift                                                        (2)
#define VO_V1_HSI_phase_mask                                                         (0x0000FFFC)
#define VO_V1_HSI_phase(data)                                                        (0x0000FFFC&((data)<<2))
#define VO_V1_HSI_phase_src(data)                                                    ((0x0000FFFC&(data))>>2)
#define VO_V1_HSI_get_phase(data)                                                    ((0x0000FFFC&(data))>>2)


#define VO_V1_HSD                                                                    0x180051EC
#define VO_V1_HSD_reg_addr                                                           "0xB80051EC"
#define VO_V1_HSD_reg                                                                0xB80051EC
#define set_VO_V1_HSD_reg(data)   (*((volatile unsigned int*) VO_V1_HSD_reg)=data)
#define get_VO_V1_HSD_reg   (*((volatile unsigned int*) VO_V1_HSD_reg))
#define VO_V1_HSD_inst_adr                                                           "0x007B"
#define VO_V1_HSD_inst                                                               0x007B
#define VO_V1_HSD_out_shift                                                          (20)
#define VO_V1_HSD_out_mask                                                           (0x7FF00000)
#define VO_V1_HSD_out(data)                                                          (0x7FF00000&((data)<<20))
#define VO_V1_HSD_out_src(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V1_HSD_get_out(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V1_HSD_delta_shift                                                        (0)
#define VO_V1_HSD_delta_mask                                                         (0x0007FFFF)
#define VO_V1_HSD_delta(data)                                                        (0x0007FFFF&((data)<<0))
#define VO_V1_HSD_delta_src(data)                                                    ((0x0007FFFF&(data))>>0)
#define VO_V1_HSD_get_delta(data)                                                    ((0x0007FFFF&(data))>>0)


#define VO_V1_HSYC_0                                                                 0x180051F0
#define VO_V1_HSYC_1                                                                 0x180051F4
#define VO_V1_HSYC_2                                                                 0x180051F8
#define VO_V1_HSYC_3                                                                 0x180051FC
#define VO_V1_HSYC_4                                                                 0x18005200
#define VO_V1_HSYC_5                                                                 0x18005204
#define VO_V1_HSYC_6                                                                 0x18005208
#define VO_V1_HSYC_7                                                                 0x1800520C
#define VO_V1_HSYC_8                                                                 0x18005210
#define VO_V1_HSYC_9                                                                 0x18005214
#define VO_V1_HSYC_10                                                                 0x18005218
#define VO_V1_HSYC_11                                                                 0x1800521C
#define VO_V1_HSYC_12                                                                 0x18005220
#define VO_V1_HSYC_13                                                                 0x18005224
#define VO_V1_HSYC_14                                                                 0x18005228
#define VO_V1_HSYC_15                                                                 0x1800522C
#define VO_V1_HSYC_16                                                                 0x18005230
#define VO_V1_HSYC_17                                                                 0x18005234
#define VO_V1_HSYC_18                                                                 0x18005238
#define VO_V1_HSYC_19                                                                 0x1800523C
#define VO_V1_HSYC_20                                                                 0x18005240
#define VO_V1_HSYC_21                                                                 0x18005244
#define VO_V1_HSYC_22                                                                 0x18005248
#define VO_V1_HSYC_23                                                                 0x1800524C
#define VO_V1_HSYC_24                                                                 0x18005250
#define VO_V1_HSYC_25                                                                 0x18005254
#define VO_V1_HSYC_26                                                                 0x18005258
#define VO_V1_HSYC_27                                                                 0x1800525C
#define VO_V1_HSYC_28                                                                 0x18005260
#define VO_V1_HSYC_29                                                                 0x18005264
#define VO_V1_HSYC_30                                                                 0x18005268
#define VO_V1_HSYC_31                                                                 0x1800526C
#define VO_V1_HSYC_0_reg_addr                                                        "0xB80051F0"
#define VO_V1_HSYC_1_reg_addr                                                        "0xB80051F4"
#define VO_V1_HSYC_2_reg_addr                                                        "0xB80051F8"
#define VO_V1_HSYC_3_reg_addr                                                        "0xB80051FC"
#define VO_V1_HSYC_4_reg_addr                                                        "0xB8005200"
#define VO_V1_HSYC_5_reg_addr                                                        "0xB8005204"
#define VO_V1_HSYC_6_reg_addr                                                        "0xB8005208"
#define VO_V1_HSYC_7_reg_addr                                                        "0xB800520C"
#define VO_V1_HSYC_8_reg_addr                                                        "0xB8005210"
#define VO_V1_HSYC_9_reg_addr                                                        "0xB8005214"
#define VO_V1_HSYC_10_reg_addr                                                        "0xB8005218"
#define VO_V1_HSYC_11_reg_addr                                                        "0xB800521C"
#define VO_V1_HSYC_12_reg_addr                                                        "0xB8005220"
#define VO_V1_HSYC_13_reg_addr                                                        "0xB8005224"
#define VO_V1_HSYC_14_reg_addr                                                        "0xB8005228"
#define VO_V1_HSYC_15_reg_addr                                                        "0xB800522C"
#define VO_V1_HSYC_16_reg_addr                                                        "0xB8005230"
#define VO_V1_HSYC_17_reg_addr                                                        "0xB8005234"
#define VO_V1_HSYC_18_reg_addr                                                        "0xB8005238"
#define VO_V1_HSYC_19_reg_addr                                                        "0xB800523C"
#define VO_V1_HSYC_20_reg_addr                                                        "0xB8005240"
#define VO_V1_HSYC_21_reg_addr                                                        "0xB8005244"
#define VO_V1_HSYC_22_reg_addr                                                        "0xB8005248"
#define VO_V1_HSYC_23_reg_addr                                                        "0xB800524C"
#define VO_V1_HSYC_24_reg_addr                                                        "0xB8005250"
#define VO_V1_HSYC_25_reg_addr                                                        "0xB8005254"
#define VO_V1_HSYC_26_reg_addr                                                        "0xB8005258"
#define VO_V1_HSYC_27_reg_addr                                                        "0xB800525C"
#define VO_V1_HSYC_28_reg_addr                                                        "0xB8005260"
#define VO_V1_HSYC_29_reg_addr                                                        "0xB8005264"
#define VO_V1_HSYC_30_reg_addr                                                        "0xB8005268"
#define VO_V1_HSYC_31_reg_addr                                                        "0xB800526C"
#define VO_V1_HSYC_0_reg                                                             0xB80051F0
#define VO_V1_HSYC_1_reg                                                             0xB80051F4
#define VO_V1_HSYC_2_reg                                                             0xB80051F8
#define VO_V1_HSYC_3_reg                                                             0xB80051FC
#define VO_V1_HSYC_4_reg                                                             0xB8005200
#define VO_V1_HSYC_5_reg                                                             0xB8005204
#define VO_V1_HSYC_6_reg                                                             0xB8005208
#define VO_V1_HSYC_7_reg                                                             0xB800520C
#define VO_V1_HSYC_8_reg                                                             0xB8005210
#define VO_V1_HSYC_9_reg                                                             0xB8005214
#define VO_V1_HSYC_10_reg                                                             0xB8005218
#define VO_V1_HSYC_11_reg                                                             0xB800521C
#define VO_V1_HSYC_12_reg                                                             0xB8005220
#define VO_V1_HSYC_13_reg                                                             0xB8005224
#define VO_V1_HSYC_14_reg                                                             0xB8005228
#define VO_V1_HSYC_15_reg                                                             0xB800522C
#define VO_V1_HSYC_16_reg                                                             0xB8005230
#define VO_V1_HSYC_17_reg                                                             0xB8005234
#define VO_V1_HSYC_18_reg                                                             0xB8005238
#define VO_V1_HSYC_19_reg                                                             0xB800523C
#define VO_V1_HSYC_20_reg                                                             0xB8005240
#define VO_V1_HSYC_21_reg                                                             0xB8005244
#define VO_V1_HSYC_22_reg                                                             0xB8005248
#define VO_V1_HSYC_23_reg                                                             0xB800524C
#define VO_V1_HSYC_24_reg                                                             0xB8005250
#define VO_V1_HSYC_25_reg                                                             0xB8005254
#define VO_V1_HSYC_26_reg                                                             0xB8005258
#define VO_V1_HSYC_27_reg                                                             0xB800525C
#define VO_V1_HSYC_28_reg                                                             0xB8005260
#define VO_V1_HSYC_29_reg                                                             0xB8005264
#define VO_V1_HSYC_30_reg                                                             0xB8005268
#define VO_V1_HSYC_31_reg                                                             0xB800526C
#define set_VO_V1_HSYC_0_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_0_reg)=data)
#define set_VO_V1_HSYC_1_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_1_reg)=data)
#define set_VO_V1_HSYC_2_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_2_reg)=data)
#define set_VO_V1_HSYC_3_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_3_reg)=data)
#define set_VO_V1_HSYC_4_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_4_reg)=data)
#define set_VO_V1_HSYC_5_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_5_reg)=data)
#define set_VO_V1_HSYC_6_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_6_reg)=data)
#define set_VO_V1_HSYC_7_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_7_reg)=data)
#define set_VO_V1_HSYC_8_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_8_reg)=data)
#define set_VO_V1_HSYC_9_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_9_reg)=data)
#define set_VO_V1_HSYC_10_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_10_reg)=data)
#define set_VO_V1_HSYC_11_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_11_reg)=data)
#define set_VO_V1_HSYC_12_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_12_reg)=data)
#define set_VO_V1_HSYC_13_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_13_reg)=data)
#define set_VO_V1_HSYC_14_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_14_reg)=data)
#define set_VO_V1_HSYC_15_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_15_reg)=data)
#define set_VO_V1_HSYC_16_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_16_reg)=data)
#define set_VO_V1_HSYC_17_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_17_reg)=data)
#define set_VO_V1_HSYC_18_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_18_reg)=data)
#define set_VO_V1_HSYC_19_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_19_reg)=data)
#define set_VO_V1_HSYC_20_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_20_reg)=data)
#define set_VO_V1_HSYC_21_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_21_reg)=data)
#define set_VO_V1_HSYC_22_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_22_reg)=data)
#define set_VO_V1_HSYC_23_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_23_reg)=data)
#define set_VO_V1_HSYC_24_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_24_reg)=data)
#define set_VO_V1_HSYC_25_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_25_reg)=data)
#define set_VO_V1_HSYC_26_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_26_reg)=data)
#define set_VO_V1_HSYC_27_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_27_reg)=data)
#define set_VO_V1_HSYC_28_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_28_reg)=data)
#define set_VO_V1_HSYC_29_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_29_reg)=data)
#define set_VO_V1_HSYC_30_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_30_reg)=data)
#define set_VO_V1_HSYC_31_reg(data)   (*((volatile unsigned int*) VO_V1_HSYC_31_reg)=data)
#define get_VO_V1_HSYC_0_reg   (*((volatile unsigned int*) VO_V1_HSYC_0_reg))
#define get_VO_V1_HSYC_1_reg   (*((volatile unsigned int*) VO_V1_HSYC_1_reg))
#define get_VO_V1_HSYC_2_reg   (*((volatile unsigned int*) VO_V1_HSYC_2_reg))
#define get_VO_V1_HSYC_3_reg   (*((volatile unsigned int*) VO_V1_HSYC_3_reg))
#define get_VO_V1_HSYC_4_reg   (*((volatile unsigned int*) VO_V1_HSYC_4_reg))
#define get_VO_V1_HSYC_5_reg   (*((volatile unsigned int*) VO_V1_HSYC_5_reg))
#define get_VO_V1_HSYC_6_reg   (*((volatile unsigned int*) VO_V1_HSYC_6_reg))
#define get_VO_V1_HSYC_7_reg   (*((volatile unsigned int*) VO_V1_HSYC_7_reg))
#define get_VO_V1_HSYC_8_reg   (*((volatile unsigned int*) VO_V1_HSYC_8_reg))
#define get_VO_V1_HSYC_9_reg   (*((volatile unsigned int*) VO_V1_HSYC_9_reg))
#define get_VO_V1_HSYC_10_reg   (*((volatile unsigned int*) VO_V1_HSYC_10_reg))
#define get_VO_V1_HSYC_11_reg   (*((volatile unsigned int*) VO_V1_HSYC_11_reg))
#define get_VO_V1_HSYC_12_reg   (*((volatile unsigned int*) VO_V1_HSYC_12_reg))
#define get_VO_V1_HSYC_13_reg   (*((volatile unsigned int*) VO_V1_HSYC_13_reg))
#define get_VO_V1_HSYC_14_reg   (*((volatile unsigned int*) VO_V1_HSYC_14_reg))
#define get_VO_V1_HSYC_15_reg   (*((volatile unsigned int*) VO_V1_HSYC_15_reg))
#define get_VO_V1_HSYC_16_reg   (*((volatile unsigned int*) VO_V1_HSYC_16_reg))
#define get_VO_V1_HSYC_17_reg   (*((volatile unsigned int*) VO_V1_HSYC_17_reg))
#define get_VO_V1_HSYC_18_reg   (*((volatile unsigned int*) VO_V1_HSYC_18_reg))
#define get_VO_V1_HSYC_19_reg   (*((volatile unsigned int*) VO_V1_HSYC_19_reg))
#define get_VO_V1_HSYC_20_reg   (*((volatile unsigned int*) VO_V1_HSYC_20_reg))
#define get_VO_V1_HSYC_21_reg   (*((volatile unsigned int*) VO_V1_HSYC_21_reg))
#define get_VO_V1_HSYC_22_reg   (*((volatile unsigned int*) VO_V1_HSYC_22_reg))
#define get_VO_V1_HSYC_23_reg   (*((volatile unsigned int*) VO_V1_HSYC_23_reg))
#define get_VO_V1_HSYC_24_reg   (*((volatile unsigned int*) VO_V1_HSYC_24_reg))
#define get_VO_V1_HSYC_25_reg   (*((volatile unsigned int*) VO_V1_HSYC_25_reg))
#define get_VO_V1_HSYC_26_reg   (*((volatile unsigned int*) VO_V1_HSYC_26_reg))
#define get_VO_V1_HSYC_27_reg   (*((volatile unsigned int*) VO_V1_HSYC_27_reg))
#define get_VO_V1_HSYC_28_reg   (*((volatile unsigned int*) VO_V1_HSYC_28_reg))
#define get_VO_V1_HSYC_29_reg   (*((volatile unsigned int*) VO_V1_HSYC_29_reg))
#define get_VO_V1_HSYC_30_reg   (*((volatile unsigned int*) VO_V1_HSYC_30_reg))
#define get_VO_V1_HSYC_31_reg   (*((volatile unsigned int*) VO_V1_HSYC_31_reg))
#define VO_V1_HSYC_0_inst_adr                                                        "0x007C"
#define VO_V1_HSYC_1_inst_adr                                                        "0x007D"
#define VO_V1_HSYC_2_inst_adr                                                        "0x007E"
#define VO_V1_HSYC_3_inst_adr                                                        "0x007F"
#define VO_V1_HSYC_4_inst_adr                                                        "0x0080"
#define VO_V1_HSYC_5_inst_adr                                                        "0x0081"
#define VO_V1_HSYC_6_inst_adr                                                        "0x0082"
#define VO_V1_HSYC_7_inst_adr                                                        "0x0083"
#define VO_V1_HSYC_8_inst_adr                                                        "0x0084"
#define VO_V1_HSYC_9_inst_adr                                                        "0x0085"
#define VO_V1_HSYC_10_inst_adr                                                        "0x0086"
#define VO_V1_HSYC_11_inst_adr                                                        "0x0087"
#define VO_V1_HSYC_12_inst_adr                                                        "0x0088"
#define VO_V1_HSYC_13_inst_adr                                                        "0x0089"
#define VO_V1_HSYC_14_inst_adr                                                        "0x008A"
#define VO_V1_HSYC_15_inst_adr                                                        "0x008B"
#define VO_V1_HSYC_16_inst_adr                                                        "0x008C"
#define VO_V1_HSYC_17_inst_adr                                                        "0x008D"
#define VO_V1_HSYC_18_inst_adr                                                        "0x008E"
#define VO_V1_HSYC_19_inst_adr                                                        "0x008F"
#define VO_V1_HSYC_20_inst_adr                                                        "0x0090"
#define VO_V1_HSYC_21_inst_adr                                                        "0x0091"
#define VO_V1_HSYC_22_inst_adr                                                        "0x0092"
#define VO_V1_HSYC_23_inst_adr                                                        "0x0093"
#define VO_V1_HSYC_24_inst_adr                                                        "0x0094"
#define VO_V1_HSYC_25_inst_adr                                                        "0x0095"
#define VO_V1_HSYC_26_inst_adr                                                        "0x0096"
#define VO_V1_HSYC_27_inst_adr                                                        "0x0097"
#define VO_V1_HSYC_28_inst_adr                                                        "0x0098"
#define VO_V1_HSYC_29_inst_adr                                                        "0x0099"
#define VO_V1_HSYC_30_inst_adr                                                        "0x009A"
#define VO_V1_HSYC_31_inst_adr                                                        "0x009B"
#define VO_V1_HSYC_0_inst                                                            0x007C
#define VO_V1_HSYC_1_inst                                                            0x007D
#define VO_V1_HSYC_2_inst                                                            0x007E
#define VO_V1_HSYC_3_inst                                                            0x007F
#define VO_V1_HSYC_4_inst                                                            0x0080
#define VO_V1_HSYC_5_inst                                                            0x0081
#define VO_V1_HSYC_6_inst                                                            0x0082
#define VO_V1_HSYC_7_inst                                                            0x0083
#define VO_V1_HSYC_8_inst                                                            0x0084
#define VO_V1_HSYC_9_inst                                                            0x0085
#define VO_V1_HSYC_10_inst                                                            0x0086
#define VO_V1_HSYC_11_inst                                                            0x0087
#define VO_V1_HSYC_12_inst                                                            0x0088
#define VO_V1_HSYC_13_inst                                                            0x0089
#define VO_V1_HSYC_14_inst                                                            0x008A
#define VO_V1_HSYC_15_inst                                                            0x008B
#define VO_V1_HSYC_16_inst                                                            0x008C
#define VO_V1_HSYC_17_inst                                                            0x008D
#define VO_V1_HSYC_18_inst                                                            0x008E
#define VO_V1_HSYC_19_inst                                                            0x008F
#define VO_V1_HSYC_20_inst                                                            0x0090
#define VO_V1_HSYC_21_inst                                                            0x0091
#define VO_V1_HSYC_22_inst                                                            0x0092
#define VO_V1_HSYC_23_inst                                                            0x0093
#define VO_V1_HSYC_24_inst                                                            0x0094
#define VO_V1_HSYC_25_inst                                                            0x0095
#define VO_V1_HSYC_26_inst                                                            0x0096
#define VO_V1_HSYC_27_inst                                                            0x0097
#define VO_V1_HSYC_28_inst                                                            0x0098
#define VO_V1_HSYC_29_inst                                                            0x0099
#define VO_V1_HSYC_30_inst                                                            0x009A
#define VO_V1_HSYC_31_inst                                                            0x009B
#define VO_V1_HSYC_c1_shift                                                          (16)
#define VO_V1_HSYC_c1_mask                                                           (0x3FFF0000)
#define VO_V1_HSYC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V1_HSYC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_HSYC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_HSYC_c0_shift                                                          (0)
#define VO_V1_HSYC_c0_mask                                                           (0x00003FFF)
#define VO_V1_HSYC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V1_HSYC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V1_HSYC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V1_HSCC_0                                                                 0x18005270
#define VO_V1_HSCC_1                                                                 0x18005274
#define VO_V1_HSCC_2                                                                 0x18005278
#define VO_V1_HSCC_3                                                                 0x1800527C
#define VO_V1_HSCC_4                                                                 0x18005280
#define VO_V1_HSCC_5                                                                 0x18005284
#define VO_V1_HSCC_6                                                                 0x18005288
#define VO_V1_HSCC_7                                                                 0x1800528C
#define VO_V1_HSCC_8                                                                 0x18005290
#define VO_V1_HSCC_9                                                                 0x18005294
#define VO_V1_HSCC_10                                                                 0x18005298
#define VO_V1_HSCC_11                                                                 0x1800529C
#define VO_V1_HSCC_12                                                                 0x180052A0
#define VO_V1_HSCC_13                                                                 0x180052A4
#define VO_V1_HSCC_14                                                                 0x180052A8
#define VO_V1_HSCC_15                                                                 0x180052AC
#define VO_V1_HSCC_0_reg_addr                                                        "0xB8005270"
#define VO_V1_HSCC_1_reg_addr                                                        "0xB8005274"
#define VO_V1_HSCC_2_reg_addr                                                        "0xB8005278"
#define VO_V1_HSCC_3_reg_addr                                                        "0xB800527C"
#define VO_V1_HSCC_4_reg_addr                                                        "0xB8005280"
#define VO_V1_HSCC_5_reg_addr                                                        "0xB8005284"
#define VO_V1_HSCC_6_reg_addr                                                        "0xB8005288"
#define VO_V1_HSCC_7_reg_addr                                                        "0xB800528C"
#define VO_V1_HSCC_8_reg_addr                                                        "0xB8005290"
#define VO_V1_HSCC_9_reg_addr                                                        "0xB8005294"
#define VO_V1_HSCC_10_reg_addr                                                        "0xB8005298"
#define VO_V1_HSCC_11_reg_addr                                                        "0xB800529C"
#define VO_V1_HSCC_12_reg_addr                                                        "0xB80052A0"
#define VO_V1_HSCC_13_reg_addr                                                        "0xB80052A4"
#define VO_V1_HSCC_14_reg_addr                                                        "0xB80052A8"
#define VO_V1_HSCC_15_reg_addr                                                        "0xB80052AC"
#define VO_V1_HSCC_0_reg                                                             0xB8005270
#define VO_V1_HSCC_1_reg                                                             0xB8005274
#define VO_V1_HSCC_2_reg                                                             0xB8005278
#define VO_V1_HSCC_3_reg                                                             0xB800527C
#define VO_V1_HSCC_4_reg                                                             0xB8005280
#define VO_V1_HSCC_5_reg                                                             0xB8005284
#define VO_V1_HSCC_6_reg                                                             0xB8005288
#define VO_V1_HSCC_7_reg                                                             0xB800528C
#define VO_V1_HSCC_8_reg                                                             0xB8005290
#define VO_V1_HSCC_9_reg                                                             0xB8005294
#define VO_V1_HSCC_10_reg                                                             0xB8005298
#define VO_V1_HSCC_11_reg                                                             0xB800529C
#define VO_V1_HSCC_12_reg                                                             0xB80052A0
#define VO_V1_HSCC_13_reg                                                             0xB80052A4
#define VO_V1_HSCC_14_reg                                                             0xB80052A8
#define VO_V1_HSCC_15_reg                                                             0xB80052AC
#define set_VO_V1_HSCC_0_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_0_reg)=data)
#define set_VO_V1_HSCC_1_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_1_reg)=data)
#define set_VO_V1_HSCC_2_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_2_reg)=data)
#define set_VO_V1_HSCC_3_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_3_reg)=data)
#define set_VO_V1_HSCC_4_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_4_reg)=data)
#define set_VO_V1_HSCC_5_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_5_reg)=data)
#define set_VO_V1_HSCC_6_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_6_reg)=data)
#define set_VO_V1_HSCC_7_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_7_reg)=data)
#define set_VO_V1_HSCC_8_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_8_reg)=data)
#define set_VO_V1_HSCC_9_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_9_reg)=data)
#define set_VO_V1_HSCC_10_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_10_reg)=data)
#define set_VO_V1_HSCC_11_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_11_reg)=data)
#define set_VO_V1_HSCC_12_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_12_reg)=data)
#define set_VO_V1_HSCC_13_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_13_reg)=data)
#define set_VO_V1_HSCC_14_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_14_reg)=data)
#define set_VO_V1_HSCC_15_reg(data)   (*((volatile unsigned int*) VO_V1_HSCC_15_reg)=data)
#define get_VO_V1_HSCC_0_reg   (*((volatile unsigned int*) VO_V1_HSCC_0_reg))
#define get_VO_V1_HSCC_1_reg   (*((volatile unsigned int*) VO_V1_HSCC_1_reg))
#define get_VO_V1_HSCC_2_reg   (*((volatile unsigned int*) VO_V1_HSCC_2_reg))
#define get_VO_V1_HSCC_3_reg   (*((volatile unsigned int*) VO_V1_HSCC_3_reg))
#define get_VO_V1_HSCC_4_reg   (*((volatile unsigned int*) VO_V1_HSCC_4_reg))
#define get_VO_V1_HSCC_5_reg   (*((volatile unsigned int*) VO_V1_HSCC_5_reg))
#define get_VO_V1_HSCC_6_reg   (*((volatile unsigned int*) VO_V1_HSCC_6_reg))
#define get_VO_V1_HSCC_7_reg   (*((volatile unsigned int*) VO_V1_HSCC_7_reg))
#define get_VO_V1_HSCC_8_reg   (*((volatile unsigned int*) VO_V1_HSCC_8_reg))
#define get_VO_V1_HSCC_9_reg   (*((volatile unsigned int*) VO_V1_HSCC_9_reg))
#define get_VO_V1_HSCC_10_reg   (*((volatile unsigned int*) VO_V1_HSCC_10_reg))
#define get_VO_V1_HSCC_11_reg   (*((volatile unsigned int*) VO_V1_HSCC_11_reg))
#define get_VO_V1_HSCC_12_reg   (*((volatile unsigned int*) VO_V1_HSCC_12_reg))
#define get_VO_V1_HSCC_13_reg   (*((volatile unsigned int*) VO_V1_HSCC_13_reg))
#define get_VO_V1_HSCC_14_reg   (*((volatile unsigned int*) VO_V1_HSCC_14_reg))
#define get_VO_V1_HSCC_15_reg   (*((volatile unsigned int*) VO_V1_HSCC_15_reg))
#define VO_V1_HSCC_0_inst_adr                                                        "0x009C"
#define VO_V1_HSCC_1_inst_adr                                                        "0x009D"
#define VO_V1_HSCC_2_inst_adr                                                        "0x009E"
#define VO_V1_HSCC_3_inst_adr                                                        "0x009F"
#define VO_V1_HSCC_4_inst_adr                                                        "0x00A0"
#define VO_V1_HSCC_5_inst_adr                                                        "0x00A1"
#define VO_V1_HSCC_6_inst_adr                                                        "0x00A2"
#define VO_V1_HSCC_7_inst_adr                                                        "0x00A3"
#define VO_V1_HSCC_8_inst_adr                                                        "0x00A4"
#define VO_V1_HSCC_9_inst_adr                                                        "0x00A5"
#define VO_V1_HSCC_10_inst_adr                                                        "0x00A6"
#define VO_V1_HSCC_11_inst_adr                                                        "0x00A7"
#define VO_V1_HSCC_12_inst_adr                                                        "0x00A8"
#define VO_V1_HSCC_13_inst_adr                                                        "0x00A9"
#define VO_V1_HSCC_14_inst_adr                                                        "0x00AA"
#define VO_V1_HSCC_15_inst_adr                                                        "0x00AB"
#define VO_V1_HSCC_0_inst                                                            0x009C
#define VO_V1_HSCC_1_inst                                                            0x009D
#define VO_V1_HSCC_2_inst                                                            0x009E
#define VO_V1_HSCC_3_inst                                                            0x009F
#define VO_V1_HSCC_4_inst                                                            0x00A0
#define VO_V1_HSCC_5_inst                                                            0x00A1
#define VO_V1_HSCC_6_inst                                                            0x00A2
#define VO_V1_HSCC_7_inst                                                            0x00A3
#define VO_V1_HSCC_8_inst                                                            0x00A4
#define VO_V1_HSCC_9_inst                                                            0x00A5
#define VO_V1_HSCC_10_inst                                                            0x00A6
#define VO_V1_HSCC_11_inst                                                            0x00A7
#define VO_V1_HSCC_12_inst                                                            0x00A8
#define VO_V1_HSCC_13_inst                                                            0x00A9
#define VO_V1_HSCC_14_inst                                                            0x00AA
#define VO_V1_HSCC_15_inst                                                            0x00AB
#define VO_V1_HSCC_c1_shift                                                          (16)
#define VO_V1_HSCC_c1_mask                                                           (0x3FFF0000)
#define VO_V1_HSCC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V1_HSCC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_HSCC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V1_HSCC_c0_shift                                                          (0)
#define VO_V1_HSCC_c0_mask                                                           (0x00003FFF)
#define VO_V1_HSCC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V1_HSCC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V1_HSCC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V1_CC1                                                                    0x180052B0
#define VO_V1_CC1_reg_addr                                                           "0xB80052B0"
#define VO_V1_CC1_reg                                                                0xB80052B0
#define set_VO_V1_CC1_reg(data)   (*((volatile unsigned int*) VO_V1_CC1_reg)=data)
#define get_VO_V1_CC1_reg   (*((volatile unsigned int*) VO_V1_CC1_reg))
#define VO_V1_CC1_inst_adr                                                           "0x00AC"
#define VO_V1_CC1_inst                                                               0x00AC
#define VO_V1_CC1_m01_shift                                                          (12)
#define VO_V1_CC1_m01_mask                                                           (0x00FFF000)
#define VO_V1_CC1_m01(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_CC1_m01_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC1_get_m01(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC1_m00_shift                                                          (0)
#define VO_V1_CC1_m00_mask                                                           (0x00000FFF)
#define VO_V1_CC1_m00(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_CC1_m00_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_CC1_get_m00(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_CC2                                                                    0x180052B4
#define VO_V1_CC2_reg_addr                                                           "0xB80052B4"
#define VO_V1_CC2_reg                                                                0xB80052B4
#define set_VO_V1_CC2_reg(data)   (*((volatile unsigned int*) VO_V1_CC2_reg)=data)
#define get_VO_V1_CC2_reg   (*((volatile unsigned int*) VO_V1_CC2_reg))
#define VO_V1_CC2_inst_adr                                                           "0x00AD"
#define VO_V1_CC2_inst                                                               0x00AD
#define VO_V1_CC2_m10_shift                                                          (12)
#define VO_V1_CC2_m10_mask                                                           (0x00FFF000)
#define VO_V1_CC2_m10(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_CC2_m10_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC2_get_m10(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC2_m02_shift                                                          (0)
#define VO_V1_CC2_m02_mask                                                           (0x00000FFF)
#define VO_V1_CC2_m02(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_CC2_m02_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_CC2_get_m02(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_CC3                                                                    0x180052B8
#define VO_V1_CC3_reg_addr                                                           "0xB80052B8"
#define VO_V1_CC3_reg                                                                0xB80052B8
#define set_VO_V1_CC3_reg(data)   (*((volatile unsigned int*) VO_V1_CC3_reg)=data)
#define get_VO_V1_CC3_reg   (*((volatile unsigned int*) VO_V1_CC3_reg))
#define VO_V1_CC3_inst_adr                                                           "0x00AE"
#define VO_V1_CC3_inst                                                               0x00AE
#define VO_V1_CC3_m12_shift                                                          (12)
#define VO_V1_CC3_m12_mask                                                           (0x00FFF000)
#define VO_V1_CC3_m12(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_CC3_m12_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC3_get_m12(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC3_m11_shift                                                          (0)
#define VO_V1_CC3_m11_mask                                                           (0x00000FFF)
#define VO_V1_CC3_m11(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_CC3_m11_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_CC3_get_m11(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_CC4                                                                    0x180052BC
#define VO_V1_CC4_reg_addr                                                           "0xB80052BC"
#define VO_V1_CC4_reg                                                                0xB80052BC
#define set_VO_V1_CC4_reg(data)   (*((volatile unsigned int*) VO_V1_CC4_reg)=data)
#define get_VO_V1_CC4_reg   (*((volatile unsigned int*) VO_V1_CC4_reg))
#define VO_V1_CC4_inst_adr                                                           "0x00AF"
#define VO_V1_CC4_inst                                                               0x00AF
#define VO_V1_CC4_m21_shift                                                          (12)
#define VO_V1_CC4_m21_mask                                                           (0x00FFF000)
#define VO_V1_CC4_m21(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V1_CC4_m21_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC4_get_m21(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V1_CC4_m20_shift                                                          (0)
#define VO_V1_CC4_m20_mask                                                           (0x00000FFF)
#define VO_V1_CC4_m20(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_CC4_m20_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_CC4_get_m20(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_CC5                                                                    0x180052C0
#define VO_V1_CC5_reg_addr                                                           "0xB80052C0"
#define VO_V1_CC5_reg                                                                0xB80052C0
#define set_VO_V1_CC5_reg(data)   (*((volatile unsigned int*) VO_V1_CC5_reg)=data)
#define get_VO_V1_CC5_reg   (*((volatile unsigned int*) VO_V1_CC5_reg))
#define VO_V1_CC5_inst_adr                                                           "0x00B0"
#define VO_V1_CC5_inst                                                               0x00B0
#define VO_V1_CC5_m22_shift                                                          (0)
#define VO_V1_CC5_m22_mask                                                           (0x00000FFF)
#define VO_V1_CC5_m22(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V1_CC5_m22_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V1_CC5_get_m22(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V1_CC6                                                                    0x180052C4
#define VO_V1_CC6_reg_addr                                                           "0xB80052C4"
#define VO_V1_CC6_reg                                                                0xB80052C4
#define set_VO_V1_CC6_reg(data)   (*((volatile unsigned int*) VO_V1_CC6_reg)=data)
#define get_VO_V1_CC6_reg   (*((volatile unsigned int*) VO_V1_CC6_reg))
#define VO_V1_CC6_inst_adr                                                           "0x00B1"
#define VO_V1_CC6_inst                                                               0x00B1
#define VO_V1_CC6_a2_shift                                                           (18)
#define VO_V1_CC6_a2_mask                                                            (0x07FC0000)
#define VO_V1_CC6_a2(data)                                                           (0x07FC0000&((data)<<18))
#define VO_V1_CC6_a2_src(data)                                                       ((0x07FC0000&(data))>>18)
#define VO_V1_CC6_get_a2(data)                                                       ((0x07FC0000&(data))>>18)
#define VO_V1_CC6_a1_shift                                                           (9)
#define VO_V1_CC6_a1_mask                                                            (0x0003FE00)
#define VO_V1_CC6_a1(data)                                                           (0x0003FE00&((data)<<9))
#define VO_V1_CC6_a1_src(data)                                                       ((0x0003FE00&(data))>>9)
#define VO_V1_CC6_get_a1(data)                                                       ((0x0003FE00&(data))>>9)
#define VO_V1_CC6_a0_shift                                                           (0)
#define VO_V1_CC6_a0_mask                                                            (0x000001FF)
#define VO_V1_CC6_a0(data)                                                           (0x000001FF&((data)<<0))
#define VO_V1_CC6_a0_src(data)                                                       ((0x000001FF&(data))>>0)
#define VO_V1_CC6_get_a0(data)                                                       ((0x000001FF&(data))>>0)


#define VO_V2                                                                        0x180052C8
#define VO_V2_reg_addr                                                               "0xB80052C8"
#define VO_V2_reg                                                                    0xB80052C8
#define set_VO_V2_reg(data)   (*((volatile unsigned int*) VO_V2_reg)=data)
#define get_VO_V2_reg   (*((volatile unsigned int*) VO_V2_reg))
#define VO_V2_inst_adr                                                               "0x00B2"
#define VO_V2_inst                                                                   0x00B2
#define VO_V2_st_shift                                                               (15)
#define VO_V2_st_mask                                                                (0x00008000)
#define VO_V2_st(data)                                                               (0x00008000&((data)<<15))
#define VO_V2_st_src(data)                                                           ((0x00008000&(data))>>15)
#define VO_V2_get_st(data)                                                           ((0x00008000&(data))>>15)
#define VO_V2_f422_shift                                                             (10)
#define VO_V2_f422_mask                                                              (0x00000400)
#define VO_V2_f422(data)                                                             (0x00000400&((data)<<10))
#define VO_V2_f422_src(data)                                                         ((0x00000400&(data))>>10)
#define VO_V2_get_f422(data)                                                         ((0x00000400&(data))>>10)
#define VO_V2_vs_shift                                                               (9)
#define VO_V2_vs_mask                                                                (0x00000200)
#define VO_V2_vs(data)                                                               (0x00000200&((data)<<9))
#define VO_V2_vs_src(data)                                                           ((0x00000200&(data))>>9)
#define VO_V2_get_vs(data)                                                           ((0x00000200&(data))>>9)
#define VO_V2_vs_near_shift                                                          (8)
#define VO_V2_vs_near_mask                                                           (0x00000100)
#define VO_V2_vs_near(data)                                                          (0x00000100&((data)<<8))
#define VO_V2_vs_near_src(data)                                                      ((0x00000100&(data))>>8)
#define VO_V2_get_vs_near(data)                                                      ((0x00000100&(data))>>8)
#define VO_V2_vs_yodd_shift                                                          (7)
#define VO_V2_vs_yodd_mask                                                           (0x00000080)
#define VO_V2_vs_yodd(data)                                                          (0x00000080&((data)<<7))
#define VO_V2_vs_yodd_src(data)                                                      ((0x00000080&(data))>>7)
#define VO_V2_get_vs_yodd(data)                                                      ((0x00000080&(data))>>7)
#define VO_V2_vs_codd_shift                                                          (6)
#define VO_V2_vs_codd_mask                                                           (0x00000040)
#define VO_V2_vs_codd(data)                                                          (0x00000040&((data)<<6))
#define VO_V2_vs_codd_src(data)                                                      ((0x00000040&(data))>>6)
#define VO_V2_get_vs_codd(data)                                                      ((0x00000040&(data))>>6)
#define VO_V2_hs_shift                                                               (5)
#define VO_V2_hs_mask                                                                (0x00000020)
#define VO_V2_hs(data)                                                               (0x00000020&((data)<<5))
#define VO_V2_hs_src(data)                                                           ((0x00000020&(data))>>5)
#define VO_V2_get_hs(data)                                                           ((0x00000020&(data))>>5)
#define VO_V2_hs_yodd_shift                                                          (4)
#define VO_V2_hs_yodd_mask                                                           (0x00000010)
#define VO_V2_hs_yodd(data)                                                          (0x00000010&((data)<<4))
#define VO_V2_hs_yodd_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_V2_get_hs_yodd(data)                                                      ((0x00000010&(data))>>4)
#define VO_V2_hs_codd_shift                                                          (3)
#define VO_V2_hs_codd_mask                                                           (0x00000008)
#define VO_V2_hs_codd(data)                                                          (0x00000008&((data)<<3))
#define VO_V2_hs_codd_src(data)                                                      ((0x00000008&(data))>>3)
#define VO_V2_get_hs_codd(data)                                                      ((0x00000008&(data))>>3)
#define VO_V2_topfield_shift                                                         (2)
#define VO_V2_topfield_mask                                                          (0x00000004)
#define VO_V2_topfield(data)                                                         (0x00000004&((data)<<2))
#define VO_V2_topfield_src(data)                                                     ((0x00000004&(data))>>2)
#define VO_V2_get_topfield(data)                                                     ((0x00000004&(data))>>2)
#define VO_V2_dmaweave_shift                                                         (1)
#define VO_V2_dmaweave_mask                                                          (0x00000002)
#define VO_V2_dmaweave(data)                                                         (0x00000002&((data)<<1))
#define VO_V2_dmaweave_src(data)                                                     ((0x00000002&(data))>>1)
#define VO_V2_get_dmaweave(data)                                                     ((0x00000002&(data))>>1)
#define VO_V2_write_data_shift                                                       (0)
#define VO_V2_write_data_mask                                                        (0x00000001)
#define VO_V2_write_data(data)                                                       (0x00000001&((data)<<0))
#define VO_V2_write_data_src(data)                                                   ((0x00000001&(data))>>0)
#define VO_V2_get_write_data(data)                                                   ((0x00000001&(data))>>0)


#define VO_V2_INDEX                                                                  0x180052CC
#define VO_V2_INDEX_reg_addr                                                         "0xB80052CC"
#define VO_V2_INDEX_reg                                                              0xB80052CC
#define set_VO_V2_INDEX_reg(data)   (*((volatile unsigned int*) VO_V2_INDEX_reg)=data)
#define get_VO_V2_INDEX_reg   (*((volatile unsigned int*) VO_V2_INDEX_reg))
#define VO_V2_INDEX_inst_adr                                                         "0x00B3"
#define VO_V2_INDEX_inst                                                             0x00B3
#define VO_V2_INDEX_w_c_shift                                                        (24)
#define VO_V2_INDEX_w_c_mask                                                         (0x7F000000)
#define VO_V2_INDEX_w_c(data)                                                        (0x7F000000&((data)<<24))
#define VO_V2_INDEX_w_c_src(data)                                                    ((0x7F000000&(data))>>24)
#define VO_V2_INDEX_get_w_c(data)                                                    ((0x7F000000&(data))>>24)
#define VO_V2_INDEX_w_y_shift                                                        (16)
#define VO_V2_INDEX_w_y_mask                                                         (0x007F0000)
#define VO_V2_INDEX_w_y(data)                                                        (0x007F0000&((data)<<16))
#define VO_V2_INDEX_w_y_src(data)                                                    ((0x007F0000&(data))>>16)
#define VO_V2_INDEX_get_w_y(data)                                                    ((0x007F0000&(data))>>16)
#define VO_V2_INDEX_c_shift                                                          (8)
#define VO_V2_INDEX_c_mask                                                           (0x00007F00)
#define VO_V2_INDEX_c(data)                                                          (0x00007F00&((data)<<8))
#define VO_V2_INDEX_c_src(data)                                                      ((0x00007F00&(data))>>8)
#define VO_V2_INDEX_get_c(data)                                                      ((0x00007F00&(data))>>8)
#define VO_V2_INDEX_y_shift                                                          (0)
#define VO_V2_INDEX_y_mask                                                           (0x0000007F)
#define VO_V2_INDEX_y(data)                                                          (0x0000007F&((data)<<0))
#define VO_V2_INDEX_y_src(data)                                                      ((0x0000007F&(data))>>0)
#define VO_V2_INDEX_get_y(data)                                                      ((0x0000007F&(data))>>0)


#define VO_V2_INDEX_RR                                                               0x180052D0
#define VO_V2_INDEX_RR_reg_addr                                                      "0xB80052D0"
#define VO_V2_INDEX_RR_reg                                                           0xB80052D0
#define set_VO_V2_INDEX_RR_reg(data)   (*((volatile unsigned int*) VO_V2_INDEX_RR_reg)=data)
#define get_VO_V2_INDEX_RR_reg   (*((volatile unsigned int*) VO_V2_INDEX_RR_reg))
#define VO_V2_INDEX_RR_inst_adr                                                      "0x00B4"
#define VO_V2_INDEX_RR_inst                                                          0x00B4
#define VO_V2_INDEX_RR_w_c_en_shift                                                  (15)
#define VO_V2_INDEX_RR_w_c_en_mask                                                   (0x00008000)
#define VO_V2_INDEX_RR_w_c_en(data)                                                  (0x00008000&((data)<<15))
#define VO_V2_INDEX_RR_w_c_en_src(data)                                              ((0x00008000&(data))>>15)
#define VO_V2_INDEX_RR_get_w_c_en(data)                                              ((0x00008000&(data))>>15)
#define VO_V2_INDEX_RR_w_c_shift                                                     (12)
#define VO_V2_INDEX_RR_w_c_mask                                                      (0x00007000)
#define VO_V2_INDEX_RR_w_c(data)                                                     (0x00007000&((data)<<12))
#define VO_V2_INDEX_RR_w_c_src(data)                                                 ((0x00007000&(data))>>12)
#define VO_V2_INDEX_RR_get_w_c(data)                                                 ((0x00007000&(data))>>12)
#define VO_V2_INDEX_RR_w_y_en_shift                                                  (11)
#define VO_V2_INDEX_RR_w_y_en_mask                                                   (0x00000800)
#define VO_V2_INDEX_RR_w_y_en(data)                                                  (0x00000800&((data)<<11))
#define VO_V2_INDEX_RR_w_y_en_src(data)                                              ((0x00000800&(data))>>11)
#define VO_V2_INDEX_RR_get_w_y_en(data)                                              ((0x00000800&(data))>>11)
#define VO_V2_INDEX_RR_w_y_shift                                                     (8)
#define VO_V2_INDEX_RR_w_y_mask                                                      (0x00000700)
#define VO_V2_INDEX_RR_w_y(data)                                                     (0x00000700&((data)<<8))
#define VO_V2_INDEX_RR_w_y_src(data)                                                 ((0x00000700&(data))>>8)
#define VO_V2_INDEX_RR_get_w_y(data)                                                 ((0x00000700&(data))>>8)
#define VO_V2_INDEX_RR_c_en_shift                                                    (7)
#define VO_V2_INDEX_RR_c_en_mask                                                     (0x00000080)
#define VO_V2_INDEX_RR_c_en(data)                                                    (0x00000080&((data)<<7))
#define VO_V2_INDEX_RR_c_en_src(data)                                                ((0x00000080&(data))>>7)
#define VO_V2_INDEX_RR_get_c_en(data)                                                ((0x00000080&(data))>>7)
#define VO_V2_INDEX_RR_c_shift                                                       (4)
#define VO_V2_INDEX_RR_c_mask                                                        (0x00000070)
#define VO_V2_INDEX_RR_c(data)                                                       (0x00000070&((data)<<4))
#define VO_V2_INDEX_RR_c_src(data)                                                   ((0x00000070&(data))>>4)
#define VO_V2_INDEX_RR_get_c(data)                                                   ((0x00000070&(data))>>4)
#define VO_V2_INDEX_RR_y_en_shift                                                    (3)
#define VO_V2_INDEX_RR_y_en_mask                                                     (0x00000008)
#define VO_V2_INDEX_RR_y_en(data)                                                    (0x00000008&((data)<<3))
#define VO_V2_INDEX_RR_y_en_src(data)                                                ((0x00000008&(data))>>3)
#define VO_V2_INDEX_RR_get_y_en(data)                                                ((0x00000008&(data))>>3)
#define VO_V2_INDEX_RR_y_shift                                                       (0)
#define VO_V2_INDEX_RR_y_mask                                                        (0x00000007)
#define VO_V2_INDEX_RR_y(data)                                                       (0x00000007&((data)<<0))
#define VO_V2_INDEX_RR_y_src(data)                                                   ((0x00000007&(data))>>0)
#define VO_V2_INDEX_RR_get_y(data)                                                   ((0x00000007&(data))>>0)


#define VO_V2_LKEY                                                                   0x180052D4
#define VO_V2_LKEY_reg_addr                                                          "0xB80052D4"
#define VO_V2_LKEY_reg                                                               0xB80052D4
#define set_VO_V2_LKEY_reg(data)   (*((volatile unsigned int*) VO_V2_LKEY_reg)=data)
#define get_VO_V2_LKEY_reg   (*((volatile unsigned int*) VO_V2_LKEY_reg))
#define VO_V2_LKEY_inst_adr                                                          "0x00B5"
#define VO_V2_LKEY_inst                                                              0x00B5
#define VO_V2_LKEY_check_last_shift                                                  (25)
#define VO_V2_LKEY_check_last_mask                                                   (0x02000000)
#define VO_V2_LKEY_check_last(data)                                                  (0x02000000&((data)<<25))
#define VO_V2_LKEY_check_last_src(data)                                              ((0x02000000&(data))>>25)
#define VO_V2_LKEY_get_check_last(data)                                              ((0x02000000&(data))>>25)
#define VO_V2_LKEY_en_shift                                                          (24)
#define VO_V2_LKEY_en_mask                                                           (0x01000000)
#define VO_V2_LKEY_en(data)                                                          (0x01000000&((data)<<24))
#define VO_V2_LKEY_en_src(data)                                                      ((0x01000000&(data))>>24)
#define VO_V2_LKEY_get_en(data)                                                      ((0x01000000&(data))>>24)
#define VO_V2_LKEY_r_shift                                                           (16)
#define VO_V2_LKEY_r_mask                                                            (0x00FF0000)
#define VO_V2_LKEY_r(data)                                                           (0x00FF0000&((data)<<16))
#define VO_V2_LKEY_r_src(data)                                                       ((0x00FF0000&(data))>>16)
#define VO_V2_LKEY_get_r(data)                                                       ((0x00FF0000&(data))>>16)
#define VO_V2_LKEY_e_shift                                                           (8)
#define VO_V2_LKEY_e_mask                                                            (0x0000FF00)
#define VO_V2_LKEY_e(data)                                                           (0x0000FF00&((data)<<8))
#define VO_V2_LKEY_e_src(data)                                                       ((0x0000FF00&(data))>>8)
#define VO_V2_LKEY_get_e(data)                                                       ((0x0000FF00&(data))>>8)
#define VO_V2_LKEY_s_shift                                                           (0)
#define VO_V2_LKEY_s_mask                                                            (0x000000FF)
#define VO_V2_LKEY_s(data)                                                           (0x000000FF&((data)<<0))
#define VO_V2_LKEY_s_src(data)                                                       ((0x000000FF&(data))>>0)
#define VO_V2_LKEY_get_s(data)                                                       ((0x000000FF&(data))>>0)


#define VO_V2_ALPHA                                                                  0x180052D8
#define VO_V2_ALPHA_reg_addr                                                         "0xB80052D8"
#define VO_V2_ALPHA_reg                                                              0xB80052D8
#define set_VO_V2_ALPHA_reg(data)   (*((volatile unsigned int*) VO_V2_ALPHA_reg)=data)
#define get_VO_V2_ALPHA_reg   (*((volatile unsigned int*) VO_V2_ALPHA_reg))
#define VO_V2_ALPHA_inst_adr                                                         "0x00B6"
#define VO_V2_ALPHA_inst                                                             0x00B6
#define VO_V2_ALPHA_a_shift                                                          (0)
#define VO_V2_ALPHA_a_mask                                                           (0x000001FF)
#define VO_V2_ALPHA_a(data)                                                          (0x000001FF&((data)<<0))
#define VO_V2_ALPHA_a_src(data)                                                      ((0x000001FF&(data))>>0)
#define VO_V2_ALPHA_get_a(data)                                                      ((0x000001FF&(data))>>0)


#define VO_V2_SIZE                                                                   0x180052DC
#define VO_V2_SIZE_reg_addr                                                          "0xB80052DC"
#define VO_V2_SIZE_reg                                                               0xB80052DC
#define set_VO_V2_SIZE_reg(data)   (*((volatile unsigned int*) VO_V2_SIZE_reg)=data)
#define get_VO_V2_SIZE_reg   (*((volatile unsigned int*) VO_V2_SIZE_reg))
#define VO_V2_SIZE_inst_adr                                                          "0x00B7"
#define VO_V2_SIZE_inst                                                              0x00B7
#define VO_V2_SIZE_w_shift                                                           (16)
#define VO_V2_SIZE_w_mask                                                            (0x07FF0000)
#define VO_V2_SIZE_w(data)                                                           (0x07FF0000&((data)<<16))
#define VO_V2_SIZE_w_src(data)                                                       ((0x07FF0000&(data))>>16)
#define VO_V2_SIZE_get_w(data)                                                       ((0x07FF0000&(data))>>16)
#define VO_V2_SIZE_h_shift                                                           (0)
#define VO_V2_SIZE_h_mask                                                            (0x000007FF)
#define VO_V2_SIZE_h(data)                                                           (0x000007FF&((data)<<0))
#define VO_V2_SIZE_h_src(data)                                                       ((0x000007FF&(data))>>0)
#define VO_V2_SIZE_get_h(data)                                                       ((0x000007FF&(data))>>0)


#define VO_V2_BUF                                                                    0x180052E0
#define VO_V2_BUF_reg_addr                                                           "0xB80052E0"
#define VO_V2_BUF_reg                                                                0xB80052E0
#define set_VO_V2_BUF_reg(data)   (*((volatile unsigned int*) VO_V2_BUF_reg)=data)
#define get_VO_V2_BUF_reg   (*((volatile unsigned int*) VO_V2_BUF_reg))
#define VO_V2_BUF_inst_adr                                                           "0x00B8"
#define VO_V2_BUF_inst                                                               0x00B8
#define VO_V2_BUF_th_shift                                                           (0)
#define VO_V2_BUF_th_mask                                                            (0x000001FF)
#define VO_V2_BUF_th(data)                                                           (0x000001FF&((data)<<0))
#define VO_V2_BUF_th_src(data)                                                       ((0x000001FF&(data))>>0)
#define VO_V2_BUF_get_th(data)                                                       ((0x000001FF&(data))>>0)


#define VO_V2_VSYI                                                                   0x180052E4
#define VO_V2_VSYI_reg_addr                                                          "0xB80052E4"
#define VO_V2_VSYI_reg                                                               0xB80052E4
#define set_VO_V2_VSYI_reg(data)   (*((volatile unsigned int*) VO_V2_VSYI_reg)=data)
#define get_VO_V2_VSYI_reg   (*((volatile unsigned int*) VO_V2_VSYI_reg))
#define VO_V2_VSYI_inst_adr                                                          "0x00B9"
#define VO_V2_VSYI_inst                                                              0x00B9
#define VO_V2_VSYI_offset_shift                                                      (16)
#define VO_V2_VSYI_offset_mask                                                       (0x0FFF0000)
#define VO_V2_VSYI_offset(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_V2_VSYI_offset_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V2_VSYI_get_offset(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V2_VSYI_phase_shift                                                       (2)
#define VO_V2_VSYI_phase_mask                                                        (0x0000FFFC)
#define VO_V2_VSYI_phase(data)                                                       (0x0000FFFC&((data)<<2))
#define VO_V2_VSYI_phase_src(data)                                                   ((0x0000FFFC&(data))>>2)
#define VO_V2_VSYI_get_phase(data)                                                   ((0x0000FFFC&(data))>>2)


#define VO_V2_VSCI                                                                   0x180052E8
#define VO_V2_VSCI_reg_addr                                                          "0xB80052E8"
#define VO_V2_VSCI_reg                                                               0xB80052E8
#define set_VO_V2_VSCI_reg(data)   (*((volatile unsigned int*) VO_V2_VSCI_reg)=data)
#define get_VO_V2_VSCI_reg   (*((volatile unsigned int*) VO_V2_VSCI_reg))
#define VO_V2_VSCI_inst_adr                                                          "0x00BA"
#define VO_V2_VSCI_inst                                                              0x00BA
#define VO_V2_VSCI_offset_shift                                                      (16)
#define VO_V2_VSCI_offset_mask                                                       (0x0FFF0000)
#define VO_V2_VSCI_offset(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_V2_VSCI_offset_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V2_VSCI_get_offset(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_V2_VSCI_phase_shift                                                       (2)
#define VO_V2_VSCI_phase_mask                                                        (0x0000FFFC)
#define VO_V2_VSCI_phase(data)                                                       (0x0000FFFC&((data)<<2))
#define VO_V2_VSCI_phase_src(data)                                                   ((0x0000FFFC&(data))>>2)
#define VO_V2_VSCI_get_phase(data)                                                   ((0x0000FFFC&(data))>>2)


#define VO_V2_VSD                                                                    0x180052EC
#define VO_V2_VSD_reg_addr                                                           "0xB80052EC"
#define VO_V2_VSD_reg                                                                0xB80052EC
#define set_VO_V2_VSD_reg(data)   (*((volatile unsigned int*) VO_V2_VSD_reg)=data)
#define get_VO_V2_VSD_reg   (*((volatile unsigned int*) VO_V2_VSD_reg))
#define VO_V2_VSD_inst_adr                                                           "0x00BB"
#define VO_V2_VSD_inst                                                               0x00BB
#define VO_V2_VSD_out_shift                                                          (20)
#define VO_V2_VSD_out_mask                                                           (0x7FF00000)
#define VO_V2_VSD_out(data)                                                          (0x7FF00000&((data)<<20))
#define VO_V2_VSD_out_src(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V2_VSD_get_out(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V2_VSD_delta_shift                                                        (0)
#define VO_V2_VSD_delta_mask                                                         (0x0007FFFF)
#define VO_V2_VSD_delta(data)                                                        (0x0007FFFF&((data)<<0))
#define VO_V2_VSD_delta_src(data)                                                    ((0x0007FFFF&(data))>>0)
#define VO_V2_VSD_get_delta(data)                                                    ((0x0007FFFF&(data))>>0)


#define VO_V2_VSYC_0                                                                 0x180052F0
#define VO_V2_VSYC_1                                                                 0x180052F4
#define VO_V2_VSYC_2                                                                 0x180052F8
#define VO_V2_VSYC_3                                                                 0x180052FC
#define VO_V2_VSYC_4                                                                 0x18005300
#define VO_V2_VSYC_5                                                                 0x18005304
#define VO_V2_VSYC_6                                                                 0x18005308
#define VO_V2_VSYC_7                                                                 0x1800530C
#define VO_V2_VSYC_0_reg_addr                                                        "0xB80052F0"
#define VO_V2_VSYC_1_reg_addr                                                        "0xB80052F4"
#define VO_V2_VSYC_2_reg_addr                                                        "0xB80052F8"
#define VO_V2_VSYC_3_reg_addr                                                        "0xB80052FC"
#define VO_V2_VSYC_4_reg_addr                                                        "0xB8005300"
#define VO_V2_VSYC_5_reg_addr                                                        "0xB8005304"
#define VO_V2_VSYC_6_reg_addr                                                        "0xB8005308"
#define VO_V2_VSYC_7_reg_addr                                                        "0xB800530C"
#define VO_V2_VSYC_0_reg                                                             0xB80052F0
#define VO_V2_VSYC_1_reg                                                             0xB80052F4
#define VO_V2_VSYC_2_reg                                                             0xB80052F8
#define VO_V2_VSYC_3_reg                                                             0xB80052FC
#define VO_V2_VSYC_4_reg                                                             0xB8005300
#define VO_V2_VSYC_5_reg                                                             0xB8005304
#define VO_V2_VSYC_6_reg                                                             0xB8005308
#define VO_V2_VSYC_7_reg                                                             0xB800530C
#define set_VO_V2_VSYC_0_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_0_reg)=data)
#define set_VO_V2_VSYC_1_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_1_reg)=data)
#define set_VO_V2_VSYC_2_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_2_reg)=data)
#define set_VO_V2_VSYC_3_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_3_reg)=data)
#define set_VO_V2_VSYC_4_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_4_reg)=data)
#define set_VO_V2_VSYC_5_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_5_reg)=data)
#define set_VO_V2_VSYC_6_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_6_reg)=data)
#define set_VO_V2_VSYC_7_reg(data)   (*((volatile unsigned int*) VO_V2_VSYC_7_reg)=data)
#define get_VO_V2_VSYC_0_reg   (*((volatile unsigned int*) VO_V2_VSYC_0_reg))
#define get_VO_V2_VSYC_1_reg   (*((volatile unsigned int*) VO_V2_VSYC_1_reg))
#define get_VO_V2_VSYC_2_reg   (*((volatile unsigned int*) VO_V2_VSYC_2_reg))
#define get_VO_V2_VSYC_3_reg   (*((volatile unsigned int*) VO_V2_VSYC_3_reg))
#define get_VO_V2_VSYC_4_reg   (*((volatile unsigned int*) VO_V2_VSYC_4_reg))
#define get_VO_V2_VSYC_5_reg   (*((volatile unsigned int*) VO_V2_VSYC_5_reg))
#define get_VO_V2_VSYC_6_reg   (*((volatile unsigned int*) VO_V2_VSYC_6_reg))
#define get_VO_V2_VSYC_7_reg   (*((volatile unsigned int*) VO_V2_VSYC_7_reg))
#define VO_V2_VSYC_0_inst_adr                                                        "0x00BC"
#define VO_V2_VSYC_1_inst_adr                                                        "0x00BD"
#define VO_V2_VSYC_2_inst_adr                                                        "0x00BE"
#define VO_V2_VSYC_3_inst_adr                                                        "0x00BF"
#define VO_V2_VSYC_4_inst_adr                                                        "0x00C0"
#define VO_V2_VSYC_5_inst_adr                                                        "0x00C1"
#define VO_V2_VSYC_6_inst_adr                                                        "0x00C2"
#define VO_V2_VSYC_7_inst_adr                                                        "0x00C3"
#define VO_V2_VSYC_0_inst                                                            0x00BC
#define VO_V2_VSYC_1_inst                                                            0x00BD
#define VO_V2_VSYC_2_inst                                                            0x00BE
#define VO_V2_VSYC_3_inst                                                            0x00BF
#define VO_V2_VSYC_4_inst                                                            0x00C0
#define VO_V2_VSYC_5_inst                                                            0x00C1
#define VO_V2_VSYC_6_inst                                                            0x00C2
#define VO_V2_VSYC_7_inst                                                            0x00C3
#define VO_V2_VSYC_c1_shift                                                          (16)
#define VO_V2_VSYC_c1_mask                                                           (0x3FFF0000)
#define VO_V2_VSYC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V2_VSYC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_VSYC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_VSYC_c0_shift                                                          (0)
#define VO_V2_VSYC_c0_mask                                                           (0x00003FFF)
#define VO_V2_VSYC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V2_VSYC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V2_VSYC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V2_VSCC_0                                                                 0x18005310
#define VO_V2_VSCC_1                                                                 0x18005314
#define VO_V2_VSCC_2                                                                 0x18005318
#define VO_V2_VSCC_3                                                                 0x1800531C
#define VO_V2_VSCC_4                                                                 0x18005320
#define VO_V2_VSCC_5                                                                 0x18005324
#define VO_V2_VSCC_6                                                                 0x18005328
#define VO_V2_VSCC_7                                                                 0x1800532C
#define VO_V2_VSCC_0_reg_addr                                                        "0xB8005310"
#define VO_V2_VSCC_1_reg_addr                                                        "0xB8005314"
#define VO_V2_VSCC_2_reg_addr                                                        "0xB8005318"
#define VO_V2_VSCC_3_reg_addr                                                        "0xB800531C"
#define VO_V2_VSCC_4_reg_addr                                                        "0xB8005320"
#define VO_V2_VSCC_5_reg_addr                                                        "0xB8005324"
#define VO_V2_VSCC_6_reg_addr                                                        "0xB8005328"
#define VO_V2_VSCC_7_reg_addr                                                        "0xB800532C"
#define VO_V2_VSCC_0_reg                                                             0xB8005310
#define VO_V2_VSCC_1_reg                                                             0xB8005314
#define VO_V2_VSCC_2_reg                                                             0xB8005318
#define VO_V2_VSCC_3_reg                                                             0xB800531C
#define VO_V2_VSCC_4_reg                                                             0xB8005320
#define VO_V2_VSCC_5_reg                                                             0xB8005324
#define VO_V2_VSCC_6_reg                                                             0xB8005328
#define VO_V2_VSCC_7_reg                                                             0xB800532C
#define set_VO_V2_VSCC_0_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_0_reg)=data)
#define set_VO_V2_VSCC_1_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_1_reg)=data)
#define set_VO_V2_VSCC_2_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_2_reg)=data)
#define set_VO_V2_VSCC_3_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_3_reg)=data)
#define set_VO_V2_VSCC_4_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_4_reg)=data)
#define set_VO_V2_VSCC_5_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_5_reg)=data)
#define set_VO_V2_VSCC_6_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_6_reg)=data)
#define set_VO_V2_VSCC_7_reg(data)   (*((volatile unsigned int*) VO_V2_VSCC_7_reg)=data)
#define get_VO_V2_VSCC_0_reg   (*((volatile unsigned int*) VO_V2_VSCC_0_reg))
#define get_VO_V2_VSCC_1_reg   (*((volatile unsigned int*) VO_V2_VSCC_1_reg))
#define get_VO_V2_VSCC_2_reg   (*((volatile unsigned int*) VO_V2_VSCC_2_reg))
#define get_VO_V2_VSCC_3_reg   (*((volatile unsigned int*) VO_V2_VSCC_3_reg))
#define get_VO_V2_VSCC_4_reg   (*((volatile unsigned int*) VO_V2_VSCC_4_reg))
#define get_VO_V2_VSCC_5_reg   (*((volatile unsigned int*) VO_V2_VSCC_5_reg))
#define get_VO_V2_VSCC_6_reg   (*((volatile unsigned int*) VO_V2_VSCC_6_reg))
#define get_VO_V2_VSCC_7_reg   (*((volatile unsigned int*) VO_V2_VSCC_7_reg))
#define VO_V2_VSCC_0_inst_adr                                                        "0x00C4"
#define VO_V2_VSCC_1_inst_adr                                                        "0x00C5"
#define VO_V2_VSCC_2_inst_adr                                                        "0x00C6"
#define VO_V2_VSCC_3_inst_adr                                                        "0x00C7"
#define VO_V2_VSCC_4_inst_adr                                                        "0x00C8"
#define VO_V2_VSCC_5_inst_adr                                                        "0x00C9"
#define VO_V2_VSCC_6_inst_adr                                                        "0x00CA"
#define VO_V2_VSCC_7_inst_adr                                                        "0x00CB"
#define VO_V2_VSCC_0_inst                                                            0x00C4
#define VO_V2_VSCC_1_inst                                                            0x00C5
#define VO_V2_VSCC_2_inst                                                            0x00C6
#define VO_V2_VSCC_3_inst                                                            0x00C7
#define VO_V2_VSCC_4_inst                                                            0x00C8
#define VO_V2_VSCC_5_inst                                                            0x00C9
#define VO_V2_VSCC_6_inst                                                            0x00CA
#define VO_V2_VSCC_7_inst                                                            0x00CB
#define VO_V2_VSCC_c1_shift                                                          (16)
#define VO_V2_VSCC_c1_mask                                                           (0x3FFF0000)
#define VO_V2_VSCC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V2_VSCC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_VSCC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_VSCC_c0_shift                                                          (0)
#define VO_V2_VSCC_c0_mask                                                           (0x00003FFF)
#define VO_V2_VSCC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V2_VSCC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V2_VSCC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V2_HSI                                                                    0x18005330
#define VO_V2_HSI_reg_addr                                                           "0xB8005330"
#define VO_V2_HSI_reg                                                                0xB8005330
#define set_VO_V2_HSI_reg(data)   (*((volatile unsigned int*) VO_V2_HSI_reg)=data)
#define get_VO_V2_HSI_reg   (*((volatile unsigned int*) VO_V2_HSI_reg))
#define VO_V2_HSI_inst_adr                                                           "0x00CC"
#define VO_V2_HSI_inst                                                               0x00CC
#define VO_V2_HSI_offset_shift                                                       (16)
#define VO_V2_HSI_offset_mask                                                        (0x07FF0000)
#define VO_V2_HSI_offset(data)                                                       (0x07FF0000&((data)<<16))
#define VO_V2_HSI_offset_src(data)                                                   ((0x07FF0000&(data))>>16)
#define VO_V2_HSI_get_offset(data)                                                   ((0x07FF0000&(data))>>16)
#define VO_V2_HSI_phase_shift                                                        (2)
#define VO_V2_HSI_phase_mask                                                         (0x0000FFFC)
#define VO_V2_HSI_phase(data)                                                        (0x0000FFFC&((data)<<2))
#define VO_V2_HSI_phase_src(data)                                                    ((0x0000FFFC&(data))>>2)
#define VO_V2_HSI_get_phase(data)                                                    ((0x0000FFFC&(data))>>2)


#define VO_V2_HSD                                                                    0x18005334
#define VO_V2_HSD_reg_addr                                                           "0xB8005334"
#define VO_V2_HSD_reg                                                                0xB8005334
#define set_VO_V2_HSD_reg(data)   (*((volatile unsigned int*) VO_V2_HSD_reg)=data)
#define get_VO_V2_HSD_reg   (*((volatile unsigned int*) VO_V2_HSD_reg))
#define VO_V2_HSD_inst_adr                                                           "0x00CD"
#define VO_V2_HSD_inst                                                               0x00CD
#define VO_V2_HSD_out_shift                                                          (20)
#define VO_V2_HSD_out_mask                                                           (0x7FF00000)
#define VO_V2_HSD_out(data)                                                          (0x7FF00000&((data)<<20))
#define VO_V2_HSD_out_src(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V2_HSD_get_out(data)                                                      ((0x7FF00000&(data))>>20)
#define VO_V2_HSD_delta_shift                                                        (0)
#define VO_V2_HSD_delta_mask                                                         (0x0007FFFF)
#define VO_V2_HSD_delta(data)                                                        (0x0007FFFF&((data)<<0))
#define VO_V2_HSD_delta_src(data)                                                    ((0x0007FFFF&(data))>>0)
#define VO_V2_HSD_get_delta(data)                                                    ((0x0007FFFF&(data))>>0)


#define VO_V2_HSYC_0                                                                 0x18005338
#define VO_V2_HSYC_1                                                                 0x1800533C
#define VO_V2_HSYC_2                                                                 0x18005340
#define VO_V2_HSYC_3                                                                 0x18005344
#define VO_V2_HSYC_4                                                                 0x18005348
#define VO_V2_HSYC_5                                                                 0x1800534C
#define VO_V2_HSYC_6                                                                 0x18005350
#define VO_V2_HSYC_7                                                                 0x18005354
#define VO_V2_HSYC_8                                                                 0x18005358
#define VO_V2_HSYC_9                                                                 0x1800535C
#define VO_V2_HSYC_10                                                                 0x18005360
#define VO_V2_HSYC_11                                                                 0x18005364
#define VO_V2_HSYC_12                                                                 0x18005368
#define VO_V2_HSYC_13                                                                 0x1800536C
#define VO_V2_HSYC_14                                                                 0x18005370
#define VO_V2_HSYC_15                                                                 0x18005374
#define VO_V2_HSYC_0_reg_addr                                                        "0xB8005338"
#define VO_V2_HSYC_1_reg_addr                                                        "0xB800533C"
#define VO_V2_HSYC_2_reg_addr                                                        "0xB8005340"
#define VO_V2_HSYC_3_reg_addr                                                        "0xB8005344"
#define VO_V2_HSYC_4_reg_addr                                                        "0xB8005348"
#define VO_V2_HSYC_5_reg_addr                                                        "0xB800534C"
#define VO_V2_HSYC_6_reg_addr                                                        "0xB8005350"
#define VO_V2_HSYC_7_reg_addr                                                        "0xB8005354"
#define VO_V2_HSYC_8_reg_addr                                                        "0xB8005358"
#define VO_V2_HSYC_9_reg_addr                                                        "0xB800535C"
#define VO_V2_HSYC_10_reg_addr                                                        "0xB8005360"
#define VO_V2_HSYC_11_reg_addr                                                        "0xB8005364"
#define VO_V2_HSYC_12_reg_addr                                                        "0xB8005368"
#define VO_V2_HSYC_13_reg_addr                                                        "0xB800536C"
#define VO_V2_HSYC_14_reg_addr                                                        "0xB8005370"
#define VO_V2_HSYC_15_reg_addr                                                        "0xB8005374"
#define VO_V2_HSYC_0_reg                                                             0xB8005338
#define VO_V2_HSYC_1_reg                                                             0xB800533C
#define VO_V2_HSYC_2_reg                                                             0xB8005340
#define VO_V2_HSYC_3_reg                                                             0xB8005344
#define VO_V2_HSYC_4_reg                                                             0xB8005348
#define VO_V2_HSYC_5_reg                                                             0xB800534C
#define VO_V2_HSYC_6_reg                                                             0xB8005350
#define VO_V2_HSYC_7_reg                                                             0xB8005354
#define VO_V2_HSYC_8_reg                                                             0xB8005358
#define VO_V2_HSYC_9_reg                                                             0xB800535C
#define VO_V2_HSYC_10_reg                                                             0xB8005360
#define VO_V2_HSYC_11_reg                                                             0xB8005364
#define VO_V2_HSYC_12_reg                                                             0xB8005368
#define VO_V2_HSYC_13_reg                                                             0xB800536C
#define VO_V2_HSYC_14_reg                                                             0xB8005370
#define VO_V2_HSYC_15_reg                                                             0xB8005374
#define set_VO_V2_HSYC_0_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_0_reg)=data)
#define set_VO_V2_HSYC_1_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_1_reg)=data)
#define set_VO_V2_HSYC_2_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_2_reg)=data)
#define set_VO_V2_HSYC_3_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_3_reg)=data)
#define set_VO_V2_HSYC_4_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_4_reg)=data)
#define set_VO_V2_HSYC_5_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_5_reg)=data)
#define set_VO_V2_HSYC_6_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_6_reg)=data)
#define set_VO_V2_HSYC_7_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_7_reg)=data)
#define set_VO_V2_HSYC_8_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_8_reg)=data)
#define set_VO_V2_HSYC_9_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_9_reg)=data)
#define set_VO_V2_HSYC_10_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_10_reg)=data)
#define set_VO_V2_HSYC_11_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_11_reg)=data)
#define set_VO_V2_HSYC_12_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_12_reg)=data)
#define set_VO_V2_HSYC_13_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_13_reg)=data)
#define set_VO_V2_HSYC_14_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_14_reg)=data)
#define set_VO_V2_HSYC_15_reg(data)   (*((volatile unsigned int*) VO_V2_HSYC_15_reg)=data)
#define get_VO_V2_HSYC_0_reg   (*((volatile unsigned int*) VO_V2_HSYC_0_reg))
#define get_VO_V2_HSYC_1_reg   (*((volatile unsigned int*) VO_V2_HSYC_1_reg))
#define get_VO_V2_HSYC_2_reg   (*((volatile unsigned int*) VO_V2_HSYC_2_reg))
#define get_VO_V2_HSYC_3_reg   (*((volatile unsigned int*) VO_V2_HSYC_3_reg))
#define get_VO_V2_HSYC_4_reg   (*((volatile unsigned int*) VO_V2_HSYC_4_reg))
#define get_VO_V2_HSYC_5_reg   (*((volatile unsigned int*) VO_V2_HSYC_5_reg))
#define get_VO_V2_HSYC_6_reg   (*((volatile unsigned int*) VO_V2_HSYC_6_reg))
#define get_VO_V2_HSYC_7_reg   (*((volatile unsigned int*) VO_V2_HSYC_7_reg))
#define get_VO_V2_HSYC_8_reg   (*((volatile unsigned int*) VO_V2_HSYC_8_reg))
#define get_VO_V2_HSYC_9_reg   (*((volatile unsigned int*) VO_V2_HSYC_9_reg))
#define get_VO_V2_HSYC_10_reg   (*((volatile unsigned int*) VO_V2_HSYC_10_reg))
#define get_VO_V2_HSYC_11_reg   (*((volatile unsigned int*) VO_V2_HSYC_11_reg))
#define get_VO_V2_HSYC_12_reg   (*((volatile unsigned int*) VO_V2_HSYC_12_reg))
#define get_VO_V2_HSYC_13_reg   (*((volatile unsigned int*) VO_V2_HSYC_13_reg))
#define get_VO_V2_HSYC_14_reg   (*((volatile unsigned int*) VO_V2_HSYC_14_reg))
#define get_VO_V2_HSYC_15_reg   (*((volatile unsigned int*) VO_V2_HSYC_15_reg))
#define VO_V2_HSYC_0_inst_adr                                                        "0x00CE"
#define VO_V2_HSYC_1_inst_adr                                                        "0x00CF"
#define VO_V2_HSYC_2_inst_adr                                                        "0x00D0"
#define VO_V2_HSYC_3_inst_adr                                                        "0x00D1"
#define VO_V2_HSYC_4_inst_adr                                                        "0x00D2"
#define VO_V2_HSYC_5_inst_adr                                                        "0x00D3"
#define VO_V2_HSYC_6_inst_adr                                                        "0x00D4"
#define VO_V2_HSYC_7_inst_adr                                                        "0x00D5"
#define VO_V2_HSYC_8_inst_adr                                                        "0x00D6"
#define VO_V2_HSYC_9_inst_adr                                                        "0x00D7"
#define VO_V2_HSYC_10_inst_adr                                                        "0x00D8"
#define VO_V2_HSYC_11_inst_adr                                                        "0x00D9"
#define VO_V2_HSYC_12_inst_adr                                                        "0x00DA"
#define VO_V2_HSYC_13_inst_adr                                                        "0x00DB"
#define VO_V2_HSYC_14_inst_adr                                                        "0x00DC"
#define VO_V2_HSYC_15_inst_adr                                                        "0x00DD"
#define VO_V2_HSYC_0_inst                                                            0x00CE
#define VO_V2_HSYC_1_inst                                                            0x00CF
#define VO_V2_HSYC_2_inst                                                            0x00D0
#define VO_V2_HSYC_3_inst                                                            0x00D1
#define VO_V2_HSYC_4_inst                                                            0x00D2
#define VO_V2_HSYC_5_inst                                                            0x00D3
#define VO_V2_HSYC_6_inst                                                            0x00D4
#define VO_V2_HSYC_7_inst                                                            0x00D5
#define VO_V2_HSYC_8_inst                                                            0x00D6
#define VO_V2_HSYC_9_inst                                                            0x00D7
#define VO_V2_HSYC_10_inst                                                            0x00D8
#define VO_V2_HSYC_11_inst                                                            0x00D9
#define VO_V2_HSYC_12_inst                                                            0x00DA
#define VO_V2_HSYC_13_inst                                                            0x00DB
#define VO_V2_HSYC_14_inst                                                            0x00DC
#define VO_V2_HSYC_15_inst                                                            0x00DD
#define VO_V2_HSYC_c1_shift                                                          (16)
#define VO_V2_HSYC_c1_mask                                                           (0x3FFF0000)
#define VO_V2_HSYC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V2_HSYC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_HSYC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_HSYC_c0_shift                                                          (0)
#define VO_V2_HSYC_c0_mask                                                           (0x00003FFF)
#define VO_V2_HSYC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V2_HSYC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V2_HSYC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V2_HSCC_0                                                                 0x18005378
#define VO_V2_HSCC_1                                                                 0x1800537C
#define VO_V2_HSCC_2                                                                 0x18005380
#define VO_V2_HSCC_3                                                                 0x18005384
#define VO_V2_HSCC_4                                                                 0x18005388
#define VO_V2_HSCC_5                                                                 0x1800538C
#define VO_V2_HSCC_6                                                                 0x18005390
#define VO_V2_HSCC_7                                                                 0x18005394
#define VO_V2_HSCC_0_reg_addr                                                        "0xB8005378"
#define VO_V2_HSCC_1_reg_addr                                                        "0xB800537C"
#define VO_V2_HSCC_2_reg_addr                                                        "0xB8005380"
#define VO_V2_HSCC_3_reg_addr                                                        "0xB8005384"
#define VO_V2_HSCC_4_reg_addr                                                        "0xB8005388"
#define VO_V2_HSCC_5_reg_addr                                                        "0xB800538C"
#define VO_V2_HSCC_6_reg_addr                                                        "0xB8005390"
#define VO_V2_HSCC_7_reg_addr                                                        "0xB8005394"
#define VO_V2_HSCC_0_reg                                                             0xB8005378
#define VO_V2_HSCC_1_reg                                                             0xB800537C
#define VO_V2_HSCC_2_reg                                                             0xB8005380
#define VO_V2_HSCC_3_reg                                                             0xB8005384
#define VO_V2_HSCC_4_reg                                                             0xB8005388
#define VO_V2_HSCC_5_reg                                                             0xB800538C
#define VO_V2_HSCC_6_reg                                                             0xB8005390
#define VO_V2_HSCC_7_reg                                                             0xB8005394
#define set_VO_V2_HSCC_0_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_0_reg)=data)
#define set_VO_V2_HSCC_1_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_1_reg)=data)
#define set_VO_V2_HSCC_2_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_2_reg)=data)
#define set_VO_V2_HSCC_3_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_3_reg)=data)
#define set_VO_V2_HSCC_4_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_4_reg)=data)
#define set_VO_V2_HSCC_5_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_5_reg)=data)
#define set_VO_V2_HSCC_6_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_6_reg)=data)
#define set_VO_V2_HSCC_7_reg(data)   (*((volatile unsigned int*) VO_V2_HSCC_7_reg)=data)
#define get_VO_V2_HSCC_0_reg   (*((volatile unsigned int*) VO_V2_HSCC_0_reg))
#define get_VO_V2_HSCC_1_reg   (*((volatile unsigned int*) VO_V2_HSCC_1_reg))
#define get_VO_V2_HSCC_2_reg   (*((volatile unsigned int*) VO_V2_HSCC_2_reg))
#define get_VO_V2_HSCC_3_reg   (*((volatile unsigned int*) VO_V2_HSCC_3_reg))
#define get_VO_V2_HSCC_4_reg   (*((volatile unsigned int*) VO_V2_HSCC_4_reg))
#define get_VO_V2_HSCC_5_reg   (*((volatile unsigned int*) VO_V2_HSCC_5_reg))
#define get_VO_V2_HSCC_6_reg   (*((volatile unsigned int*) VO_V2_HSCC_6_reg))
#define get_VO_V2_HSCC_7_reg   (*((volatile unsigned int*) VO_V2_HSCC_7_reg))
#define VO_V2_HSCC_0_inst_adr                                                        "0x00DE"
#define VO_V2_HSCC_1_inst_adr                                                        "0x00DF"
#define VO_V2_HSCC_2_inst_adr                                                        "0x00E0"
#define VO_V2_HSCC_3_inst_adr                                                        "0x00E1"
#define VO_V2_HSCC_4_inst_adr                                                        "0x00E2"
#define VO_V2_HSCC_5_inst_adr                                                        "0x00E3"
#define VO_V2_HSCC_6_inst_adr                                                        "0x00E4"
#define VO_V2_HSCC_7_inst_adr                                                        "0x00E5"
#define VO_V2_HSCC_0_inst                                                            0x00DE
#define VO_V2_HSCC_1_inst                                                            0x00DF
#define VO_V2_HSCC_2_inst                                                            0x00E0
#define VO_V2_HSCC_3_inst                                                            0x00E1
#define VO_V2_HSCC_4_inst                                                            0x00E2
#define VO_V2_HSCC_5_inst                                                            0x00E3
#define VO_V2_HSCC_6_inst                                                            0x00E4
#define VO_V2_HSCC_7_inst                                                            0x00E5
#define VO_V2_HSCC_c1_shift                                                          (16)
#define VO_V2_HSCC_c1_mask                                                           (0x3FFF0000)
#define VO_V2_HSCC_c1(data)                                                          (0x3FFF0000&((data)<<16))
#define VO_V2_HSCC_c1_src(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_HSCC_get_c1(data)                                                      ((0x3FFF0000&(data))>>16)
#define VO_V2_HSCC_c0_shift                                                          (0)
#define VO_V2_HSCC_c0_mask                                                           (0x00003FFF)
#define VO_V2_HSCC_c0(data)                                                          (0x00003FFF&((data)<<0))
#define VO_V2_HSCC_c0_src(data)                                                      ((0x00003FFF&(data))>>0)
#define VO_V2_HSCC_get_c0(data)                                                      ((0x00003FFF&(data))>>0)


#define VO_V2_CC1                                                                    0x18005398
#define VO_V2_CC1_reg_addr                                                           "0xB8005398"
#define VO_V2_CC1_reg                                                                0xB8005398
#define set_VO_V2_CC1_reg(data)   (*((volatile unsigned int*) VO_V2_CC1_reg)=data)
#define get_VO_V2_CC1_reg   (*((volatile unsigned int*) VO_V2_CC1_reg))
#define VO_V2_CC1_inst_adr                                                           "0x00E6"
#define VO_V2_CC1_inst                                                               0x00E6
#define VO_V2_CC1_m01_shift                                                          (12)
#define VO_V2_CC1_m01_mask                                                           (0x00FFF000)
#define VO_V2_CC1_m01(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V2_CC1_m01_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC1_get_m01(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC1_m00_shift                                                          (0)
#define VO_V2_CC1_m00_mask                                                           (0x00000FFF)
#define VO_V2_CC1_m00(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V2_CC1_m00_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V2_CC1_get_m00(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V2_CC2                                                                    0x1800539C
#define VO_V2_CC2_reg_addr                                                           "0xB800539C"
#define VO_V2_CC2_reg                                                                0xB800539C
#define set_VO_V2_CC2_reg(data)   (*((volatile unsigned int*) VO_V2_CC2_reg)=data)
#define get_VO_V2_CC2_reg   (*((volatile unsigned int*) VO_V2_CC2_reg))
#define VO_V2_CC2_inst_adr                                                           "0x00E7"
#define VO_V2_CC2_inst                                                               0x00E7
#define VO_V2_CC2_m10_shift                                                          (12)
#define VO_V2_CC2_m10_mask                                                           (0x00FFF000)
#define VO_V2_CC2_m10(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V2_CC2_m10_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC2_get_m10(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC2_m02_shift                                                          (0)
#define VO_V2_CC2_m02_mask                                                           (0x00000FFF)
#define VO_V2_CC2_m02(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V2_CC2_m02_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V2_CC2_get_m02(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V2_CC3                                                                    0x180053A0
#define VO_V2_CC3_reg_addr                                                           "0xB80053A0"
#define VO_V2_CC3_reg                                                                0xB80053A0
#define set_VO_V2_CC3_reg(data)   (*((volatile unsigned int*) VO_V2_CC3_reg)=data)
#define get_VO_V2_CC3_reg   (*((volatile unsigned int*) VO_V2_CC3_reg))
#define VO_V2_CC3_inst_adr                                                           "0x00E8"
#define VO_V2_CC3_inst                                                               0x00E8
#define VO_V2_CC3_m12_shift                                                          (12)
#define VO_V2_CC3_m12_mask                                                           (0x00FFF000)
#define VO_V2_CC3_m12(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V2_CC3_m12_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC3_get_m12(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC3_m11_shift                                                          (0)
#define VO_V2_CC3_m11_mask                                                           (0x00000FFF)
#define VO_V2_CC3_m11(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V2_CC3_m11_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V2_CC3_get_m11(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V2_CC4                                                                    0x180053A4
#define VO_V2_CC4_reg_addr                                                           "0xB80053A4"
#define VO_V2_CC4_reg                                                                0xB80053A4
#define set_VO_V2_CC4_reg(data)   (*((volatile unsigned int*) VO_V2_CC4_reg)=data)
#define get_VO_V2_CC4_reg   (*((volatile unsigned int*) VO_V2_CC4_reg))
#define VO_V2_CC4_inst_adr                                                           "0x00E9"
#define VO_V2_CC4_inst                                                               0x00E9
#define VO_V2_CC4_m21_shift                                                          (12)
#define VO_V2_CC4_m21_mask                                                           (0x00FFF000)
#define VO_V2_CC4_m21(data)                                                          (0x00FFF000&((data)<<12))
#define VO_V2_CC4_m21_src(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC4_get_m21(data)                                                      ((0x00FFF000&(data))>>12)
#define VO_V2_CC4_m20_shift                                                          (0)
#define VO_V2_CC4_m20_mask                                                           (0x00000FFF)
#define VO_V2_CC4_m20(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V2_CC4_m20_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V2_CC4_get_m20(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V2_CC5                                                                    0x180053A8
#define VO_V2_CC5_reg_addr                                                           "0xB80053A8"
#define VO_V2_CC5_reg                                                                0xB80053A8
#define set_VO_V2_CC5_reg(data)   (*((volatile unsigned int*) VO_V2_CC5_reg)=data)
#define get_VO_V2_CC5_reg   (*((volatile unsigned int*) VO_V2_CC5_reg))
#define VO_V2_CC5_inst_adr                                                           "0x00EA"
#define VO_V2_CC5_inst                                                               0x00EA
#define VO_V2_CC5_m22_shift                                                          (0)
#define VO_V2_CC5_m22_mask                                                           (0x00000FFF)
#define VO_V2_CC5_m22(data)                                                          (0x00000FFF&((data)<<0))
#define VO_V2_CC5_m22_src(data)                                                      ((0x00000FFF&(data))>>0)
#define VO_V2_CC5_get_m22(data)                                                      ((0x00000FFF&(data))>>0)


#define VO_V2_CC6                                                                    0x180053AC
#define VO_V2_CC6_reg_addr                                                           "0xB80053AC"
#define VO_V2_CC6_reg                                                                0xB80053AC
#define set_VO_V2_CC6_reg(data)   (*((volatile unsigned int*) VO_V2_CC6_reg)=data)
#define get_VO_V2_CC6_reg   (*((volatile unsigned int*) VO_V2_CC6_reg))
#define VO_V2_CC6_inst_adr                                                           "0x00EB"
#define VO_V2_CC6_inst                                                               0x00EB
#define VO_V2_CC6_a2_shift                                                           (18)
#define VO_V2_CC6_a2_mask                                                            (0x07FC0000)
#define VO_V2_CC6_a2(data)                                                           (0x07FC0000&((data)<<18))
#define VO_V2_CC6_a2_src(data)                                                       ((0x07FC0000&(data))>>18)
#define VO_V2_CC6_get_a2(data)                                                       ((0x07FC0000&(data))>>18)
#define VO_V2_CC6_a1_shift                                                           (9)
#define VO_V2_CC6_a1_mask                                                            (0x0003FE00)
#define VO_V2_CC6_a1(data)                                                           (0x0003FE00&((data)<<9))
#define VO_V2_CC6_a1_src(data)                                                       ((0x0003FE00&(data))>>9)
#define VO_V2_CC6_get_a1(data)                                                       ((0x0003FE00&(data))>>9)
#define VO_V2_CC6_a0_shift                                                           (0)
#define VO_V2_CC6_a0_mask                                                            (0x000001FF)
#define VO_V2_CC6_a0(data)                                                           (0x000001FF&((data)<<0))
#define VO_V2_CC6_a0_src(data)                                                       ((0x000001FF&(data))>>0)
#define VO_V2_CC6_get_a0(data)                                                       ((0x000001FF&(data))>>0)


#define VO_SUB1                                                                      0x180053B0
#define VO_SUB1_reg_addr                                                             "0xB80053B0"
#define VO_SUB1_reg                                                                  0xB80053B0
#define set_VO_SUB1_reg(data)   (*((volatile unsigned int*) VO_SUB1_reg)=data)
#define get_VO_SUB1_reg   (*((volatile unsigned int*) VO_SUB1_reg))
#define VO_SUB1_inst_adr                                                             "0x00EC"
#define VO_SUB1_inst                                                                 0x00EC
#define VO_SUB1_d3_dbh_shift                                                         (28)
#define VO_SUB1_d3_dbh_mask                                                          (0x10000000)
#define VO_SUB1_d3_dbh(data)                                                         (0x10000000&((data)<<28))
#define VO_SUB1_d3_dbh_src(data)                                                     ((0x10000000&(data))>>28)
#define VO_SUB1_get_d3_dbh(data)                                                     ((0x10000000&(data))>>28)
#define VO_SUB1_d3_dbw_shift                                                         (27)
#define VO_SUB1_d3_dbw_mask                                                          (0x08000000)
#define VO_SUB1_d3_dbw(data)                                                         (0x08000000&((data)<<27))
#define VO_SUB1_d3_dbw_src(data)                                                     ((0x08000000&(data))>>27)
#define VO_SUB1_get_d3_dbw(data)                                                     ((0x08000000&(data))>>27)
#define VO_SUB1_d3_dup_shift                                                         (26)
#define VO_SUB1_d3_dup_mask                                                          (0x04000000)
#define VO_SUB1_d3_dup(data)                                                         (0x04000000&((data)<<26))
#define VO_SUB1_d3_dup_src(data)                                                     ((0x04000000&(data))>>26)
#define VO_SUB1_get_d3_dup(data)                                                     ((0x04000000&(data))>>26)
#define VO_SUB1_dvd_shift                                                            (23)
#define VO_SUB1_dvd_mask                                                             (0x00800000)
#define VO_SUB1_dvd(data)                                                            (0x00800000&((data)<<23))
#define VO_SUB1_dvd_src(data)                                                        ((0x00800000&(data))>>23)
#define VO_SUB1_get_dvd(data)                                                        ((0x00800000&(data))>>23)
#define VO_SUB1_hddvd_shift                                                          (22)
#define VO_SUB1_hddvd_mask                                                           (0x00400000)
#define VO_SUB1_hddvd(data)                                                          (0x00400000&((data)<<22))
#define VO_SUB1_hddvd_src(data)                                                      ((0x00400000&(data))>>22)
#define VO_SUB1_get_hddvd(data)                                                      ((0x00400000&(data))>>22)
#define VO_SUB1_hl_shift                                                             (19)
#define VO_SUB1_hl_mask                                                              (0x00080000)
#define VO_SUB1_hl(data)                                                             (0x00080000&((data)<<19))
#define VO_SUB1_hl_src(data)                                                         ((0x00080000&(data))>>19)
#define VO_SUB1_get_hl(data)                                                         ((0x00080000&(data))>>19)
#define VO_SUB1_dvb_shift                                                            (17)
#define VO_SUB1_dvb_mask                                                             (0x00020000)
#define VO_SUB1_dvb(data)                                                            (0x00020000&((data)<<17))
#define VO_SUB1_dvb_src(data)                                                        ((0x00020000&(data))>>17)
#define VO_SUB1_get_dvb(data)                                                        ((0x00020000&(data))>>17)
#define VO_SUB1_osd_shift                                                            (16)
#define VO_SUB1_osd_mask                                                             (0x00010000)
#define VO_SUB1_osd(data)                                                            (0x00010000&((data)<<16))
#define VO_SUB1_osd_src(data)                                                        ((0x00010000&(data))>>16)
#define VO_SUB1_get_osd(data)                                                        ((0x00010000&(data))>>16)
#define VO_SUB1_fetch_1080i_shift                                                    (15)
#define VO_SUB1_fetch_1080i_mask                                                     (0x00008000)
#define VO_SUB1_fetch_1080i(data)                                                    (0x00008000&((data)<<15))
#define VO_SUB1_fetch_1080i_src(data)                                                ((0x00008000&(data))>>15)
#define VO_SUB1_get_fetch_1080i(data)                                                ((0x00008000&(data))>>15)
#define VO_SUB1_odd_1080i_shift                                                      (14)
#define VO_SUB1_odd_1080i_mask                                                       (0x00004000)
#define VO_SUB1_odd_1080i(data)                                                      (0x00004000&((data)<<14))
#define VO_SUB1_odd_1080i_src(data)                                                  ((0x00004000&(data))>>14)
#define VO_SUB1_get_odd_1080i(data)                                                  ((0x00004000&(data))>>14)
#define VO_SUB1_vs_shift                                                             (10)
#define VO_SUB1_vs_mask                                                              (0x00000400)
#define VO_SUB1_vs(data)                                                             (0x00000400&((data)<<10))
#define VO_SUB1_vs_src(data)                                                         ((0x00000400&(data))>>10)
#define VO_SUB1_get_vs(data)                                                         ((0x00000400&(data))>>10)
#define VO_SUB1_vs_near_shift                                                        (9)
#define VO_SUB1_vs_near_mask                                                         (0x00000200)
#define VO_SUB1_vs_near(data)                                                        (0x00000200&((data)<<9))
#define VO_SUB1_vs_near_src(data)                                                    ((0x00000200&(data))>>9)
#define VO_SUB1_get_vs_near(data)                                                    ((0x00000200&(data))>>9)
#define VO_SUB1_vs_yodd_shift                                                        (8)
#define VO_SUB1_vs_yodd_mask                                                         (0x00000100)
#define VO_SUB1_vs_yodd(data)                                                        (0x00000100&((data)<<8))
#define VO_SUB1_vs_yodd_src(data)                                                    ((0x00000100&(data))>>8)
#define VO_SUB1_get_vs_yodd(data)                                                    ((0x00000100&(data))>>8)
#define VO_SUB1_vs_codd_shift                                                        (7)
#define VO_SUB1_vs_codd_mask                                                         (0x00000080)
#define VO_SUB1_vs_codd(data)                                                        (0x00000080&((data)<<7))
#define VO_SUB1_vs_codd_src(data)                                                    ((0x00000080&(data))>>7)
#define VO_SUB1_get_vs_codd(data)                                                    ((0x00000080&(data))>>7)
#define VO_SUB1_hs_shift                                                             (6)
#define VO_SUB1_hs_mask                                                              (0x00000040)
#define VO_SUB1_hs(data)                                                             (0x00000040&((data)<<6))
#define VO_SUB1_hs_src(data)                                                         ((0x00000040&(data))>>6)
#define VO_SUB1_get_hs(data)                                                         ((0x00000040&(data))>>6)
#define VO_SUB1_hs_yodd_shift                                                        (5)
#define VO_SUB1_hs_yodd_mask                                                         (0x00000020)
#define VO_SUB1_hs_yodd(data)                                                        (0x00000020&((data)<<5))
#define VO_SUB1_hs_yodd_src(data)                                                    ((0x00000020&(data))>>5)
#define VO_SUB1_get_hs_yodd(data)                                                    ((0x00000020&(data))>>5)
#define VO_SUB1_hs_codd_shift                                                        (4)
#define VO_SUB1_hs_codd_mask                                                         (0x00000010)
#define VO_SUB1_hs_codd(data)                                                        (0x00000010&((data)<<4))
#define VO_SUB1_hs_codd_src(data)                                                    ((0x00000010&(data))>>4)
#define VO_SUB1_get_hs_codd(data)                                                    ((0x00000010&(data))>>4)
#define VO_SUB1_chroma1_shift                                                        (3)
#define VO_SUB1_chroma1_mask                                                         (0x00000008)
#define VO_SUB1_chroma1(data)                                                        (0x00000008&((data)<<3))
#define VO_SUB1_chroma1_src(data)                                                    ((0x00000008&(data))>>3)
#define VO_SUB1_get_chroma1(data)                                                    ((0x00000008&(data))>>3)
#define VO_SUB1_chroma0_shift                                                        (2)
#define VO_SUB1_chroma0_mask                                                         (0x00000004)
#define VO_SUB1_chroma0(data)                                                        (0x00000004&((data)<<2))
#define VO_SUB1_chroma0_src(data)                                                    ((0x00000004&(data))>>2)
#define VO_SUB1_get_chroma0(data)                                                    ((0x00000004&(data))>>2)
#define VO_SUB1_write_data_shift                                                     (0)
#define VO_SUB1_write_data_mask                                                      (0x00000001)
#define VO_SUB1_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_SUB1_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_SUB1_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_SUB1_START                                                                0x180053B4
#define VO_SUB1_START_reg_addr                                                       "0xB80053B4"
#define VO_SUB1_START_reg                                                            0xB80053B4
#define set_VO_SUB1_START_reg(data)   (*((volatile unsigned int*) VO_SUB1_START_reg)=data)
#define get_VO_SUB1_START_reg   (*((volatile unsigned int*) VO_SUB1_START_reg))
#define VO_SUB1_START_inst_adr                                                       "0x00ED"
#define VO_SUB1_START_inst                                                           0x00ED
#define VO_SUB1_START_ref_shift                                                      (24)
#define VO_SUB1_START_ref_mask                                                       (0x03000000)
#define VO_SUB1_START_ref(data)                                                      (0x03000000&((data)<<24))
#define VO_SUB1_START_ref_src(data)                                                  ((0x03000000&(data))>>24)
#define VO_SUB1_START_get_ref(data)                                                  ((0x03000000&(data))>>24)
#define VO_SUB1_START_pos_shift                                                      (12)
#define VO_SUB1_START_pos_mask                                                       (0x007FF000)
#define VO_SUB1_START_pos(data)                                                      (0x007FF000&((data)<<12))
#define VO_SUB1_START_pos_src(data)                                                  ((0x007FF000&(data))>>12)
#define VO_SUB1_START_get_pos(data)                                                  ((0x007FF000&(data))>>12)
#define VO_SUB1_START_th_shift                                                       (0)
#define VO_SUB1_START_th_mask                                                        (0x000007FF)
#define VO_SUB1_START_th(data)                                                       (0x000007FF&((data)<<0))
#define VO_SUB1_START_th_src(data)                                                   ((0x000007FF&(data))>>0)
#define VO_SUB1_START_get_th(data)                                                   ((0x000007FF&(data))>>0)


#define VO_SUB1_SUBP                                                                 0x180053B8
#define VO_SUB1_SUBP_reg_addr                                                        "0xB80053B8"
#define VO_SUB1_SUBP_reg                                                             0xB80053B8
#define set_VO_SUB1_SUBP_reg(data)   (*((volatile unsigned int*) VO_SUB1_SUBP_reg)=data)
#define get_VO_SUB1_SUBP_reg   (*((volatile unsigned int*) VO_SUB1_SUBP_reg))
#define VO_SUB1_SUBP_inst_adr                                                        "0x00EE"
#define VO_SUB1_SUBP_inst                                                            0x00EE
#define VO_SUB1_SUBP_w_shift                                                         (0)
#define VO_SUB1_SUBP_w_mask                                                          (0x00000FFF)
#define VO_SUB1_SUBP_w(data)                                                         (0x00000FFF&((data)<<0))
#define VO_SUB1_SUBP_w_src(data)                                                     ((0x00000FFF&(data))>>0)
#define VO_SUB1_SUBP_get_w(data)                                                     ((0x00000FFF&(data))>>0)


#define VO_SUB1_SIZE                                                                 0x180053BC
#define VO_SUB1_SIZE_reg_addr                                                        "0xB80053BC"
#define VO_SUB1_SIZE_reg                                                             0xB80053BC
#define set_VO_SUB1_SIZE_reg(data)   (*((volatile unsigned int*) VO_SUB1_SIZE_reg)=data)
#define get_VO_SUB1_SIZE_reg   (*((volatile unsigned int*) VO_SUB1_SIZE_reg))
#define VO_SUB1_SIZE_inst_adr                                                        "0x00EF"
#define VO_SUB1_SIZE_inst                                                            0x00EF
#define VO_SUB1_SIZE_w_shift                                                         (12)
#define VO_SUB1_SIZE_w_mask                                                          (0x007FF000)
#define VO_SUB1_SIZE_w(data)                                                         (0x007FF000&((data)<<12))
#define VO_SUB1_SIZE_w_src(data)                                                     ((0x007FF000&(data))>>12)
#define VO_SUB1_SIZE_get_w(data)                                                     ((0x007FF000&(data))>>12)
#define VO_SUB1_SIZE_h_shift                                                         (0)
#define VO_SUB1_SIZE_h_mask                                                          (0x000007FF)
#define VO_SUB1_SIZE_h(data)                                                         (0x000007FF&((data)<<0))
#define VO_SUB1_SIZE_h_src(data)                                                     ((0x000007FF&(data))>>0)
#define VO_SUB1_SIZE_get_h(data)                                                     ((0x000007FF&(data))>>0)


#define VO_SUB1_BUF                                                                  0x180053C0
#define VO_SUB1_BUF_reg_addr                                                         "0xB80053C0"
#define VO_SUB1_BUF_reg                                                              0xB80053C0
#define set_VO_SUB1_BUF_reg(data)   (*((volatile unsigned int*) VO_SUB1_BUF_reg)=data)
#define get_VO_SUB1_BUF_reg   (*((volatile unsigned int*) VO_SUB1_BUF_reg))
#define VO_SUB1_BUF_inst_adr                                                         "0x00F0"
#define VO_SUB1_BUF_inst                                                             0x00F0
#define VO_SUB1_BUF_th_shift                                                         (0)
#define VO_SUB1_BUF_th_mask                                                          (0x000001FF)
#define VO_SUB1_BUF_th(data)                                                         (0x000001FF&((data)<<0))
#define VO_SUB1_BUF_th_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_SUB1_BUF_get_th(data)                                                     ((0x000001FF&(data))>>0)


#define VO_SUB1_WI                                                                   0x180053C4
#define VO_SUB1_WI_reg_addr                                                          "0xB80053C4"
#define VO_SUB1_WI_reg                                                               0xB80053C4
#define set_VO_SUB1_WI_reg(data)   (*((volatile unsigned int*) VO_SUB1_WI_reg)=data)
#define get_VO_SUB1_WI_reg   (*((volatile unsigned int*) VO_SUB1_WI_reg))
#define VO_SUB1_WI_inst_adr                                                          "0x00F1"
#define VO_SUB1_WI_inst                                                              0x00F1
#define VO_SUB1_WI_endian_shift                                                      (31)
#define VO_SUB1_WI_endian_mask                                                       (0x80000000)
#define VO_SUB1_WI_endian(data)                                                      (0x80000000&((data)<<31))
#define VO_SUB1_WI_endian_src(data)                                                  ((0x80000000&(data))>>31)
#define VO_SUB1_WI_get_endian(data)                                                  ((0x80000000&(data))>>31)
#define VO_SUB1_WI_a_shift                                                           (0)
#define VO_SUB1_WI_a_mask                                                            (0x7FFFFFFF)
#define VO_SUB1_WI_a(data)                                                           (0x7FFFFFFF&((data)<<0))
#define VO_SUB1_WI_a_src(data)                                                       ((0x7FFFFFFF&(data))>>0)
#define VO_SUB1_WI_get_a(data)                                                       ((0x7FFFFFFF&(data))>>0)


#define VO_SUB1_WI_3D                                                                0x180053C8
#define VO_SUB1_WI_3D_reg_addr                                                       "0xB80053C8"
#define VO_SUB1_WI_3D_reg                                                            0xB80053C8
#define set_VO_SUB1_WI_3D_reg(data)   (*((volatile unsigned int*) VO_SUB1_WI_3D_reg)=data)
#define get_VO_SUB1_WI_3D_reg   (*((volatile unsigned int*) VO_SUB1_WI_3D_reg))
#define VO_SUB1_WI_3D_inst_adr                                                       "0x00F2"
#define VO_SUB1_WI_3D_inst                                                           0x00F2
#define VO_SUB1_WI_3D_a_shift                                                        (0)
#define VO_SUB1_WI_3D_a_mask                                                         (0x7FFFFFFF)
#define VO_SUB1_WI_3D_a(data)                                                        (0x7FFFFFFF&((data)<<0))
#define VO_SUB1_WI_3D_a_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define VO_SUB1_WI_3D_get_a(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define VO_SUB1_CLUT                                                                 0x180053CC
#define VO_SUB1_CLUT_reg_addr                                                        "0xB80053CC"
#define VO_SUB1_CLUT_reg                                                             0xB80053CC
#define set_VO_SUB1_CLUT_reg(data)   (*((volatile unsigned int*) VO_SUB1_CLUT_reg)=data)
#define get_VO_SUB1_CLUT_reg   (*((volatile unsigned int*) VO_SUB1_CLUT_reg))
#define VO_SUB1_CLUT_inst_adr                                                        "0x00F3"
#define VO_SUB1_CLUT_inst                                                            0x00F3
#define VO_SUB1_CLUT_keep_shift                                                      (31)
#define VO_SUB1_CLUT_keep_mask                                                       (0x80000000)
#define VO_SUB1_CLUT_keep(data)                                                      (0x80000000&((data)<<31))
#define VO_SUB1_CLUT_keep_src(data)                                                  ((0x80000000&(data))>>31)
#define VO_SUB1_CLUT_get_keep(data)                                                  ((0x80000000&(data))>>31)
#define VO_SUB1_CLUT_a_shift                                                         (0)
#define VO_SUB1_CLUT_a_mask                                                          (0x7FFFFFFF)
#define VO_SUB1_CLUT_a(data)                                                         (0x7FFFFFFF&((data)<<0))
#define VO_SUB1_CLUT_a_src(data)                                                     ((0x7FFFFFFF&(data))>>0)
#define VO_SUB1_CLUT_get_a(data)                                                     ((0x7FFFFFFF&(data))>>0)


#define VO_SUB1_PXDT                                                                 0x180053D0
#define VO_SUB1_PXDT_reg_addr                                                        "0xB80053D0"
#define VO_SUB1_PXDT_reg                                                             0xB80053D0
#define set_VO_SUB1_PXDT_reg(data)   (*((volatile unsigned int*) VO_SUB1_PXDT_reg)=data)
#define get_VO_SUB1_PXDT_reg   (*((volatile unsigned int*) VO_SUB1_PXDT_reg))
#define VO_SUB1_PXDT_inst_adr                                                        "0x00F4"
#define VO_SUB1_PXDT_inst                                                            0x00F4
#define VO_SUB1_PXDT_a_shift                                                         (0)
#define VO_SUB1_PXDT_a_mask                                                          (0x7FFFFFFF)
#define VO_SUB1_PXDT_a(data)                                                         (0x7FFFFFFF&((data)<<0))
#define VO_SUB1_PXDT_a_src(data)                                                     ((0x7FFFFFFF&(data))>>0)
#define VO_SUB1_PXDT_get_a(data)                                                     ((0x7FFFFFFF&(data))>>0)


#define VO_SUB1_PXDB                                                                 0x180053D4
#define VO_SUB1_PXDB_reg_addr                                                        "0xB80053D4"
#define VO_SUB1_PXDB_reg                                                             0xB80053D4
#define set_VO_SUB1_PXDB_reg(data)   (*((volatile unsigned int*) VO_SUB1_PXDB_reg)=data)
#define get_VO_SUB1_PXDB_reg   (*((volatile unsigned int*) VO_SUB1_PXDB_reg))
#define VO_SUB1_PXDB_inst_adr                                                        "0x00F5"
#define VO_SUB1_PXDB_inst                                                            0x00F5
#define VO_SUB1_PXDB_a_shift                                                         (0)
#define VO_SUB1_PXDB_a_mask                                                          (0x7FFFFFFF)
#define VO_SUB1_PXDB_a(data)                                                         (0x7FFFFFFF&((data)<<0))
#define VO_SUB1_PXDB_a_src(data)                                                     ((0x7FFFFFFF&(data))>>0)
#define VO_SUB1_PXDB_get_a(data)                                                     ((0x7FFFFFFF&(data))>>0)


#define VO_SUB1_LC                                                                   0x180053D8
#define VO_SUB1_LC_reg_addr                                                          "0xB80053D8"
#define VO_SUB1_LC_reg                                                               0xB80053D8
#define set_VO_SUB1_LC_reg(data)   (*((volatile unsigned int*) VO_SUB1_LC_reg)=data)
#define get_VO_SUB1_LC_reg   (*((volatile unsigned int*) VO_SUB1_LC_reg))
#define VO_SUB1_LC_inst_adr                                                          "0x00F6"
#define VO_SUB1_LC_inst                                                              0x00F6
#define VO_SUB1_LC_num_shift                                                         (0)
#define VO_SUB1_LC_num_mask                                                          (0x000007FF)
#define VO_SUB1_LC_num(data)                                                         (0x000007FF&((data)<<0))
#define VO_SUB1_LC_num_src(data)                                                     ((0x000007FF&(data))>>0)
#define VO_SUB1_LC_get_num(data)                                                     ((0x000007FF&(data))>>0)


#define VO_SUB1_DFT                                                                  0x180053DC
#define VO_SUB1_DFT_reg_addr                                                         "0xB80053DC"
#define VO_SUB1_DFT_reg                                                              0xB80053DC
#define set_VO_SUB1_DFT_reg(data)   (*((volatile unsigned int*) VO_SUB1_DFT_reg)=data)
#define get_VO_SUB1_DFT_reg   (*((volatile unsigned int*) VO_SUB1_DFT_reg))
#define VO_SUB1_DFT_inst_adr                                                         "0x00F7"
#define VO_SUB1_DFT_inst                                                             0x00F7
#define VO_SUB1_DFT_a3_shift                                                         (28)
#define VO_SUB1_DFT_a3_mask                                                          (0xF0000000)
#define VO_SUB1_DFT_a3(data)                                                         (0xF0000000&((data)<<28))
#define VO_SUB1_DFT_a3_src(data)                                                     ((0xF0000000&(data))>>28)
#define VO_SUB1_DFT_get_a3(data)                                                     ((0xF0000000&(data))>>28)
#define VO_SUB1_DFT_a2_shift                                                         (24)
#define VO_SUB1_DFT_a2_mask                                                          (0x0F000000)
#define VO_SUB1_DFT_a2(data)                                                         (0x0F000000&((data)<<24))
#define VO_SUB1_DFT_a2_src(data)                                                     ((0x0F000000&(data))>>24)
#define VO_SUB1_DFT_get_a2(data)                                                     ((0x0F000000&(data))>>24)
#define VO_SUB1_DFT_a1_shift                                                         (20)
#define VO_SUB1_DFT_a1_mask                                                          (0x00F00000)
#define VO_SUB1_DFT_a1(data)                                                         (0x00F00000&((data)<<20))
#define VO_SUB1_DFT_a1_src(data)                                                     ((0x00F00000&(data))>>20)
#define VO_SUB1_DFT_get_a1(data)                                                     ((0x00F00000&(data))>>20)
#define VO_SUB1_DFT_a0_shift                                                         (16)
#define VO_SUB1_DFT_a0_mask                                                          (0x000F0000)
#define VO_SUB1_DFT_a0(data)                                                         (0x000F0000&((data)<<16))
#define VO_SUB1_DFT_a0_src(data)                                                     ((0x000F0000&(data))>>16)
#define VO_SUB1_DFT_get_a0(data)                                                     ((0x000F0000&(data))>>16)
#define VO_SUB1_DFT_i3_shift                                                         (12)
#define VO_SUB1_DFT_i3_mask                                                          (0x0000F000)
#define VO_SUB1_DFT_i3(data)                                                         (0x0000F000&((data)<<12))
#define VO_SUB1_DFT_i3_src(data)                                                     ((0x0000F000&(data))>>12)
#define VO_SUB1_DFT_get_i3(data)                                                     ((0x0000F000&(data))>>12)
#define VO_SUB1_DFT_i2_shift                                                         (8)
#define VO_SUB1_DFT_i2_mask                                                          (0x00000F00)
#define VO_SUB1_DFT_i2(data)                                                         (0x00000F00&((data)<<8))
#define VO_SUB1_DFT_i2_src(data)                                                     ((0x00000F00&(data))>>8)
#define VO_SUB1_DFT_get_i2(data)                                                     ((0x00000F00&(data))>>8)
#define VO_SUB1_DFT_i1_shift                                                         (4)
#define VO_SUB1_DFT_i1_mask                                                          (0x000000F0)
#define VO_SUB1_DFT_i1(data)                                                         (0x000000F0&((data)<<4))
#define VO_SUB1_DFT_i1_src(data)                                                     ((0x000000F0&(data))>>4)
#define VO_SUB1_DFT_get_i1(data)                                                     ((0x000000F0&(data))>>4)
#define VO_SUB1_DFT_i0_shift                                                         (0)
#define VO_SUB1_DFT_i0_mask                                                          (0x0000000F)
#define VO_SUB1_DFT_i0(data)                                                         (0x0000000F&((data)<<0))
#define VO_SUB1_DFT_i0_src(data)                                                     ((0x0000000F&(data))>>0)
#define VO_SUB1_DFT_get_i0(data)                                                     ((0x0000000F&(data))>>0)


#define VO_SUB1_HLX                                                                  0x180053E0
#define VO_SUB1_HLX_reg_addr                                                         "0xB80053E0"
#define VO_SUB1_HLX_reg                                                              0xB80053E0
#define set_VO_SUB1_HLX_reg(data)   (*((volatile unsigned int*) VO_SUB1_HLX_reg)=data)
#define get_VO_SUB1_HLX_reg   (*((volatile unsigned int*) VO_SUB1_HLX_reg))
#define VO_SUB1_HLX_inst_adr                                                         "0x00F8"
#define VO_SUB1_HLX_inst                                                             0x00F8
#define VO_SUB1_HLX_e_shift                                                          (12)
#define VO_SUB1_HLX_e_mask                                                           (0x007FF000)
#define VO_SUB1_HLX_e(data)                                                          (0x007FF000&((data)<<12))
#define VO_SUB1_HLX_e_src(data)                                                      ((0x007FF000&(data))>>12)
#define VO_SUB1_HLX_get_e(data)                                                      ((0x007FF000&(data))>>12)
#define VO_SUB1_HLX_s_shift                                                          (0)
#define VO_SUB1_HLX_s_mask                                                           (0x000007FF)
#define VO_SUB1_HLX_s(data)                                                          (0x000007FF&((data)<<0))
#define VO_SUB1_HLX_s_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_SUB1_HLX_get_s(data)                                                      ((0x000007FF&(data))>>0)


#define VO_SUB1_HLY                                                                  0x180053E4
#define VO_SUB1_HLY_reg_addr                                                         "0xB80053E4"
#define VO_SUB1_HLY_reg                                                              0xB80053E4
#define set_VO_SUB1_HLY_reg(data)   (*((volatile unsigned int*) VO_SUB1_HLY_reg)=data)
#define get_VO_SUB1_HLY_reg   (*((volatile unsigned int*) VO_SUB1_HLY_reg))
#define VO_SUB1_HLY_inst_adr                                                         "0x00F9"
#define VO_SUB1_HLY_inst                                                             0x00F9
#define VO_SUB1_HLY_e_shift                                                          (12)
#define VO_SUB1_HLY_e_mask                                                           (0x007FF000)
#define VO_SUB1_HLY_e(data)                                                          (0x007FF000&((data)<<12))
#define VO_SUB1_HLY_e_src(data)                                                      ((0x007FF000&(data))>>12)
#define VO_SUB1_HLY_get_e(data)                                                      ((0x007FF000&(data))>>12)
#define VO_SUB1_HLY_s_shift                                                          (0)
#define VO_SUB1_HLY_s_mask                                                           (0x000007FF)
#define VO_SUB1_HLY_s(data)                                                          (0x000007FF&((data)<<0))
#define VO_SUB1_HLY_s_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_SUB1_HLY_get_s(data)                                                      ((0x000007FF&(data))>>0)


#define VO_SUB1_HL                                                                   0x180053E8
#define VO_SUB1_HL_reg_addr                                                          "0xB80053E8"
#define VO_SUB1_HL_reg                                                               0xB80053E8
#define set_VO_SUB1_HL_reg(data)   (*((volatile unsigned int*) VO_SUB1_HL_reg)=data)
#define get_VO_SUB1_HL_reg   (*((volatile unsigned int*) VO_SUB1_HL_reg))
#define VO_SUB1_HL_inst_adr                                                          "0x00FA"
#define VO_SUB1_HL_inst                                                              0x00FA
#define VO_SUB1_HL_a3_shift                                                          (28)
#define VO_SUB1_HL_a3_mask                                                           (0xF0000000)
#define VO_SUB1_HL_a3(data)                                                          (0xF0000000&((data)<<28))
#define VO_SUB1_HL_a3_src(data)                                                      ((0xF0000000&(data))>>28)
#define VO_SUB1_HL_get_a3(data)                                                      ((0xF0000000&(data))>>28)
#define VO_SUB1_HL_a2_shift                                                          (24)
#define VO_SUB1_HL_a2_mask                                                           (0x0F000000)
#define VO_SUB1_HL_a2(data)                                                          (0x0F000000&((data)<<24))
#define VO_SUB1_HL_a2_src(data)                                                      ((0x0F000000&(data))>>24)
#define VO_SUB1_HL_get_a2(data)                                                      ((0x0F000000&(data))>>24)
#define VO_SUB1_HL_a1_shift                                                          (20)
#define VO_SUB1_HL_a1_mask                                                           (0x00F00000)
#define VO_SUB1_HL_a1(data)                                                          (0x00F00000&((data)<<20))
#define VO_SUB1_HL_a1_src(data)                                                      ((0x00F00000&(data))>>20)
#define VO_SUB1_HL_get_a1(data)                                                      ((0x00F00000&(data))>>20)
#define VO_SUB1_HL_a0_shift                                                          (16)
#define VO_SUB1_HL_a0_mask                                                           (0x000F0000)
#define VO_SUB1_HL_a0(data)                                                          (0x000F0000&((data)<<16))
#define VO_SUB1_HL_a0_src(data)                                                      ((0x000F0000&(data))>>16)
#define VO_SUB1_HL_get_a0(data)                                                      ((0x000F0000&(data))>>16)
#define VO_SUB1_HL_i3_shift                                                          (12)
#define VO_SUB1_HL_i3_mask                                                           (0x0000F000)
#define VO_SUB1_HL_i3(data)                                                          (0x0000F000&((data)<<12))
#define VO_SUB1_HL_i3_src(data)                                                      ((0x0000F000&(data))>>12)
#define VO_SUB1_HL_get_i3(data)                                                      ((0x0000F000&(data))>>12)
#define VO_SUB1_HL_i2_shift                                                          (8)
#define VO_SUB1_HL_i2_mask                                                           (0x00000F00)
#define VO_SUB1_HL_i2(data)                                                          (0x00000F00&((data)<<8))
#define VO_SUB1_HL_i2_src(data)                                                      ((0x00000F00&(data))>>8)
#define VO_SUB1_HL_get_i2(data)                                                      ((0x00000F00&(data))>>8)
#define VO_SUB1_HL_i1_shift                                                          (4)
#define VO_SUB1_HL_i1_mask                                                           (0x000000F0)
#define VO_SUB1_HL_i1(data)                                                          (0x000000F0&((data)<<4))
#define VO_SUB1_HL_i1_src(data)                                                      ((0x000000F0&(data))>>4)
#define VO_SUB1_HL_get_i1(data)                                                      ((0x000000F0&(data))>>4)
#define VO_SUB1_HL_i0_shift                                                          (0)
#define VO_SUB1_HL_i0_mask                                                           (0x0000000F)
#define VO_SUB1_HL_i0(data)                                                          (0x0000000F&((data)<<0))
#define VO_SUB1_HL_i0_src(data)                                                      ((0x0000000F&(data))>>0)
#define VO_SUB1_HL_get_i0(data)                                                      ((0x0000000F&(data))>>0)


#define VO_SUB1_CC1                                                                  0x180053EC
#define VO_SUB1_CC1_reg_addr                                                         "0xB80053EC"
#define VO_SUB1_CC1_reg                                                              0xB80053EC
#define set_VO_SUB1_CC1_reg(data)   (*((volatile unsigned int*) VO_SUB1_CC1_reg)=data)
#define get_VO_SUB1_CC1_reg   (*((volatile unsigned int*) VO_SUB1_CC1_reg))
#define VO_SUB1_CC1_inst_adr                                                         "0x00FB"
#define VO_SUB1_CC1_inst                                                             0x00FB
#define VO_SUB1_CC1_m01_shift                                                        (12)
#define VO_SUB1_CC1_m01_mask                                                         (0x00FFF000)
#define VO_SUB1_CC1_m01(data)                                                        (0x00FFF000&((data)<<12))
#define VO_SUB1_CC1_m01_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC1_get_m01(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC1_m00_shift                                                        (0)
#define VO_SUB1_CC1_m00_mask                                                         (0x00000FFF)
#define VO_SUB1_CC1_m00(data)                                                        (0x00000FFF&((data)<<0))
#define VO_SUB1_CC1_m00_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_SUB1_CC1_get_m00(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_SUB1_CC2                                                                  0x180053F0
#define VO_SUB1_CC2_reg_addr                                                         "0xB80053F0"
#define VO_SUB1_CC2_reg                                                              0xB80053F0
#define set_VO_SUB1_CC2_reg(data)   (*((volatile unsigned int*) VO_SUB1_CC2_reg)=data)
#define get_VO_SUB1_CC2_reg   (*((volatile unsigned int*) VO_SUB1_CC2_reg))
#define VO_SUB1_CC2_inst_adr                                                         "0x00FC"
#define VO_SUB1_CC2_inst                                                             0x00FC
#define VO_SUB1_CC2_m10_shift                                                        (12)
#define VO_SUB1_CC2_m10_mask                                                         (0x00FFF000)
#define VO_SUB1_CC2_m10(data)                                                        (0x00FFF000&((data)<<12))
#define VO_SUB1_CC2_m10_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC2_get_m10(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC2_m02_shift                                                        (0)
#define VO_SUB1_CC2_m02_mask                                                         (0x00000FFF)
#define VO_SUB1_CC2_m02(data)                                                        (0x00000FFF&((data)<<0))
#define VO_SUB1_CC2_m02_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_SUB1_CC2_get_m02(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_SUB1_CC3                                                                  0x180053F4
#define VO_SUB1_CC3_reg_addr                                                         "0xB80053F4"
#define VO_SUB1_CC3_reg                                                              0xB80053F4
#define set_VO_SUB1_CC3_reg(data)   (*((volatile unsigned int*) VO_SUB1_CC3_reg)=data)
#define get_VO_SUB1_CC3_reg   (*((volatile unsigned int*) VO_SUB1_CC3_reg))
#define VO_SUB1_CC3_inst_adr                                                         "0x00FD"
#define VO_SUB1_CC3_inst                                                             0x00FD
#define VO_SUB1_CC3_m12_shift                                                        (12)
#define VO_SUB1_CC3_m12_mask                                                         (0x00FFF000)
#define VO_SUB1_CC3_m12(data)                                                        (0x00FFF000&((data)<<12))
#define VO_SUB1_CC3_m12_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC3_get_m12(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC3_m11_shift                                                        (0)
#define VO_SUB1_CC3_m11_mask                                                         (0x00000FFF)
#define VO_SUB1_CC3_m11(data)                                                        (0x00000FFF&((data)<<0))
#define VO_SUB1_CC3_m11_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_SUB1_CC3_get_m11(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_SUB1_CC4                                                                  0x180053F8
#define VO_SUB1_CC4_reg_addr                                                         "0xB80053F8"
#define VO_SUB1_CC4_reg                                                              0xB80053F8
#define set_VO_SUB1_CC4_reg(data)   (*((volatile unsigned int*) VO_SUB1_CC4_reg)=data)
#define get_VO_SUB1_CC4_reg   (*((volatile unsigned int*) VO_SUB1_CC4_reg))
#define VO_SUB1_CC4_inst_adr                                                         "0x00FE"
#define VO_SUB1_CC4_inst                                                             0x00FE
#define VO_SUB1_CC4_m21_shift                                                        (12)
#define VO_SUB1_CC4_m21_mask                                                         (0x00FFF000)
#define VO_SUB1_CC4_m21(data)                                                        (0x00FFF000&((data)<<12))
#define VO_SUB1_CC4_m21_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC4_get_m21(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_SUB1_CC4_m20_shift                                                        (0)
#define VO_SUB1_CC4_m20_mask                                                         (0x00000FFF)
#define VO_SUB1_CC4_m20(data)                                                        (0x00000FFF&((data)<<0))
#define VO_SUB1_CC4_m20_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_SUB1_CC4_get_m20(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_SUB1_CC5                                                                  0x180053FC
#define VO_SUB1_CC5_reg_addr                                                         "0xB80053FC"
#define VO_SUB1_CC5_reg                                                              0xB80053FC
#define set_VO_SUB1_CC5_reg(data)   (*((volatile unsigned int*) VO_SUB1_CC5_reg)=data)
#define get_VO_SUB1_CC5_reg   (*((volatile unsigned int*) VO_SUB1_CC5_reg))
#define VO_SUB1_CC5_inst_adr                                                         "0x00FF"
#define VO_SUB1_CC5_inst                                                             0x00FF
#define VO_SUB1_CC5_m22_shift                                                        (0)
#define VO_SUB1_CC5_m22_mask                                                         (0x00000FFF)
#define VO_SUB1_CC5_m22(data)                                                        (0x00000FFF&((data)<<0))
#define VO_SUB1_CC5_m22_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_SUB1_CC5_get_m22(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_SUB1_CC6                                                                  0x18005400
#define VO_SUB1_CC6_reg_addr                                                         "0xB8005400"
#define VO_SUB1_CC6_reg                                                              0xB8005400
#define set_VO_SUB1_CC6_reg(data)   (*((volatile unsigned int*) VO_SUB1_CC6_reg)=data)
#define get_VO_SUB1_CC6_reg   (*((volatile unsigned int*) VO_SUB1_CC6_reg))
#define VO_SUB1_CC6_inst_adr                                                         "0x0000"
#define VO_SUB1_CC6_inst                                                             0x0000
#define VO_SUB1_CC6_a2_shift                                                         (18)
#define VO_SUB1_CC6_a2_mask                                                          (0x07FC0000)
#define VO_SUB1_CC6_a2(data)                                                         (0x07FC0000&((data)<<18))
#define VO_SUB1_CC6_a2_src(data)                                                     ((0x07FC0000&(data))>>18)
#define VO_SUB1_CC6_get_a2(data)                                                     ((0x07FC0000&(data))>>18)
#define VO_SUB1_CC6_a1_shift                                                         (9)
#define VO_SUB1_CC6_a1_mask                                                          (0x0003FE00)
#define VO_SUB1_CC6_a1(data)                                                         (0x0003FE00&((data)<<9))
#define VO_SUB1_CC6_a1_src(data)                                                     ((0x0003FE00&(data))>>9)
#define VO_SUB1_CC6_get_a1(data)                                                     ((0x0003FE00&(data))>>9)
#define VO_SUB1_CC6_a0_shift                                                         (0)
#define VO_SUB1_CC6_a0_mask                                                          (0x000001FF)
#define VO_SUB1_CC6_a0(data)                                                         (0x000001FF&((data)<<0))
#define VO_SUB1_CC6_a0_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_SUB1_CC6_get_a0(data)                                                     ((0x000001FF&(data))>>0)


#define VO_SUB1_VSI                                                                  0x18005404
#define VO_SUB1_VSI_reg_addr                                                         "0xB8005404"
#define VO_SUB1_VSI_reg                                                              0xB8005404
#define set_VO_SUB1_VSI_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSI_reg)=data)
#define get_VO_SUB1_VSI_reg   (*((volatile unsigned int*) VO_SUB1_VSI_reg))
#define VO_SUB1_VSI_inst_adr                                                         "0x0001"
#define VO_SUB1_VSI_inst                                                             0x0001
#define VO_SUB1_VSI_offset_shift                                                     (16)
#define VO_SUB1_VSI_offset_mask                                                      (0x07FF0000)
#define VO_SUB1_VSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_SUB1_VSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_SUB1_VSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_SUB1_VSI_phase_shift                                                      (2)
#define VO_SUB1_VSI_phase_mask                                                       (0x0000FFFC)
#define VO_SUB1_VSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_SUB1_VSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_SUB1_VSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_SUB1_VSD                                                                  0x18005408
#define VO_SUB1_VSD_reg_addr                                                         "0xB8005408"
#define VO_SUB1_VSD_reg                                                              0xB8005408
#define set_VO_SUB1_VSD_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSD_reg)=data)
#define get_VO_SUB1_VSD_reg   (*((volatile unsigned int*) VO_SUB1_VSD_reg))
#define VO_SUB1_VSD_inst_adr                                                         "0x0002"
#define VO_SUB1_VSD_inst                                                             0x0002
#define VO_SUB1_VSD_out_shift                                                        (20)
#define VO_SUB1_VSD_out_mask                                                         (0x7FF00000)
#define VO_SUB1_VSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_SUB1_VSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_SUB1_VSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_SUB1_VSD_delta_shift                                                      (0)
#define VO_SUB1_VSD_delta_mask                                                       (0x0007FFFF)
#define VO_SUB1_VSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_SUB1_VSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_SUB1_VSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_SUB1_VSYC_0                                                               0x1800540C
#define VO_SUB1_VSYC_1                                                               0x18005410
#define VO_SUB1_VSYC_2                                                               0x18005414
#define VO_SUB1_VSYC_3                                                               0x18005418
#define VO_SUB1_VSYC_0_reg_addr                                                      "0xB800540C"
#define VO_SUB1_VSYC_1_reg_addr                                                      "0xB8005410"
#define VO_SUB1_VSYC_2_reg_addr                                                      "0xB8005414"
#define VO_SUB1_VSYC_3_reg_addr                                                      "0xB8005418"
#define VO_SUB1_VSYC_0_reg                                                           0xB800540C
#define VO_SUB1_VSYC_1_reg                                                           0xB8005410
#define VO_SUB1_VSYC_2_reg                                                           0xB8005414
#define VO_SUB1_VSYC_3_reg                                                           0xB8005418
#define set_VO_SUB1_VSYC_0_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSYC_0_reg)=data)
#define set_VO_SUB1_VSYC_1_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSYC_1_reg)=data)
#define set_VO_SUB1_VSYC_2_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSYC_2_reg)=data)
#define set_VO_SUB1_VSYC_3_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSYC_3_reg)=data)
#define get_VO_SUB1_VSYC_0_reg   (*((volatile unsigned int*) VO_SUB1_VSYC_0_reg))
#define get_VO_SUB1_VSYC_1_reg   (*((volatile unsigned int*) VO_SUB1_VSYC_1_reg))
#define get_VO_SUB1_VSYC_2_reg   (*((volatile unsigned int*) VO_SUB1_VSYC_2_reg))
#define get_VO_SUB1_VSYC_3_reg   (*((volatile unsigned int*) VO_SUB1_VSYC_3_reg))
#define VO_SUB1_VSYC_0_inst_adr                                                      "0x0003"
#define VO_SUB1_VSYC_1_inst_adr                                                      "0x0004"
#define VO_SUB1_VSYC_2_inst_adr                                                      "0x0005"
#define VO_SUB1_VSYC_3_inst_adr                                                      "0x0006"
#define VO_SUB1_VSYC_0_inst                                                          0x0003
#define VO_SUB1_VSYC_1_inst                                                          0x0004
#define VO_SUB1_VSYC_2_inst                                                          0x0005
#define VO_SUB1_VSYC_3_inst                                                          0x0006
#define VO_SUB1_VSYC_c1_shift                                                        (16)
#define VO_SUB1_VSYC_c1_mask                                                         (0x3FFF0000)
#define VO_SUB1_VSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_SUB1_VSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_VSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_VSYC_c0_shift                                                        (0)
#define VO_SUB1_VSYC_c0_mask                                                         (0x00003FFF)
#define VO_SUB1_VSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_SUB1_VSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_SUB1_VSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_SUB1_VSCC_0                                                               0x1800541C
#define VO_SUB1_VSCC_1                                                               0x18005420
#define VO_SUB1_VSCC_2                                                               0x18005424
#define VO_SUB1_VSCC_3                                                               0x18005428
#define VO_SUB1_VSCC_0_reg_addr                                                      "0xB800541C"
#define VO_SUB1_VSCC_1_reg_addr                                                      "0xB8005420"
#define VO_SUB1_VSCC_2_reg_addr                                                      "0xB8005424"
#define VO_SUB1_VSCC_3_reg_addr                                                      "0xB8005428"
#define VO_SUB1_VSCC_0_reg                                                           0xB800541C
#define VO_SUB1_VSCC_1_reg                                                           0xB8005420
#define VO_SUB1_VSCC_2_reg                                                           0xB8005424
#define VO_SUB1_VSCC_3_reg                                                           0xB8005428
#define set_VO_SUB1_VSCC_0_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSCC_0_reg)=data)
#define set_VO_SUB1_VSCC_1_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSCC_1_reg)=data)
#define set_VO_SUB1_VSCC_2_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSCC_2_reg)=data)
#define set_VO_SUB1_VSCC_3_reg(data)   (*((volatile unsigned int*) VO_SUB1_VSCC_3_reg)=data)
#define get_VO_SUB1_VSCC_0_reg   (*((volatile unsigned int*) VO_SUB1_VSCC_0_reg))
#define get_VO_SUB1_VSCC_1_reg   (*((volatile unsigned int*) VO_SUB1_VSCC_1_reg))
#define get_VO_SUB1_VSCC_2_reg   (*((volatile unsigned int*) VO_SUB1_VSCC_2_reg))
#define get_VO_SUB1_VSCC_3_reg   (*((volatile unsigned int*) VO_SUB1_VSCC_3_reg))
#define VO_SUB1_VSCC_0_inst_adr                                                      "0x0007"
#define VO_SUB1_VSCC_1_inst_adr                                                      "0x0008"
#define VO_SUB1_VSCC_2_inst_adr                                                      "0x0009"
#define VO_SUB1_VSCC_3_inst_adr                                                      "0x000A"
#define VO_SUB1_VSCC_0_inst                                                          0x0007
#define VO_SUB1_VSCC_1_inst                                                          0x0008
#define VO_SUB1_VSCC_2_inst                                                          0x0009
#define VO_SUB1_VSCC_3_inst                                                          0x000A
#define VO_SUB1_VSCC_c1_shift                                                        (16)
#define VO_SUB1_VSCC_c1_mask                                                         (0x3FFF0000)
#define VO_SUB1_VSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_SUB1_VSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_VSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_VSCC_c0_shift                                                        (0)
#define VO_SUB1_VSCC_c0_mask                                                         (0x00003FFF)
#define VO_SUB1_VSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_SUB1_VSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_SUB1_VSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_SUB1_HSI                                                                  0x1800542C
#define VO_SUB1_HSI_reg_addr                                                         "0xB800542C"
#define VO_SUB1_HSI_reg                                                              0xB800542C
#define set_VO_SUB1_HSI_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSI_reg)=data)
#define get_VO_SUB1_HSI_reg   (*((volatile unsigned int*) VO_SUB1_HSI_reg))
#define VO_SUB1_HSI_inst_adr                                                         "0x000B"
#define VO_SUB1_HSI_inst                                                             0x000B
#define VO_SUB1_HSI_offset_shift                                                     (16)
#define VO_SUB1_HSI_offset_mask                                                      (0x07FF0000)
#define VO_SUB1_HSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_SUB1_HSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_SUB1_HSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_SUB1_HSI_phase_shift                                                      (2)
#define VO_SUB1_HSI_phase_mask                                                       (0x0000FFFC)
#define VO_SUB1_HSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_SUB1_HSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_SUB1_HSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_SUB1_HSD                                                                  0x18005430
#define VO_SUB1_HSD_reg_addr                                                         "0xB8005430"
#define VO_SUB1_HSD_reg                                                              0xB8005430
#define set_VO_SUB1_HSD_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSD_reg)=data)
#define get_VO_SUB1_HSD_reg   (*((volatile unsigned int*) VO_SUB1_HSD_reg))
#define VO_SUB1_HSD_inst_adr                                                         "0x000C"
#define VO_SUB1_HSD_inst                                                             0x000C
#define VO_SUB1_HSD_out_shift                                                        (20)
#define VO_SUB1_HSD_out_mask                                                         (0x7FF00000)
#define VO_SUB1_HSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_SUB1_HSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_SUB1_HSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_SUB1_HSD_delta_shift                                                      (0)
#define VO_SUB1_HSD_delta_mask                                                       (0x0007FFFF)
#define VO_SUB1_HSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_SUB1_HSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_SUB1_HSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_SUB1_HSYC_0                                                               0x18005434
#define VO_SUB1_HSYC_1                                                               0x18005438
#define VO_SUB1_HSYC_2                                                               0x1800543C
#define VO_SUB1_HSYC_3                                                               0x18005440
#define VO_SUB1_HSYC_4                                                               0x18005444
#define VO_SUB1_HSYC_5                                                               0x18005448
#define VO_SUB1_HSYC_6                                                               0x1800544C
#define VO_SUB1_HSYC_7                                                               0x18005450
#define VO_SUB1_HSYC_0_reg_addr                                                      "0xB8005434"
#define VO_SUB1_HSYC_1_reg_addr                                                      "0xB8005438"
#define VO_SUB1_HSYC_2_reg_addr                                                      "0xB800543C"
#define VO_SUB1_HSYC_3_reg_addr                                                      "0xB8005440"
#define VO_SUB1_HSYC_4_reg_addr                                                      "0xB8005444"
#define VO_SUB1_HSYC_5_reg_addr                                                      "0xB8005448"
#define VO_SUB1_HSYC_6_reg_addr                                                      "0xB800544C"
#define VO_SUB1_HSYC_7_reg_addr                                                      "0xB8005450"
#define VO_SUB1_HSYC_0_reg                                                           0xB8005434
#define VO_SUB1_HSYC_1_reg                                                           0xB8005438
#define VO_SUB1_HSYC_2_reg                                                           0xB800543C
#define VO_SUB1_HSYC_3_reg                                                           0xB8005440
#define VO_SUB1_HSYC_4_reg                                                           0xB8005444
#define VO_SUB1_HSYC_5_reg                                                           0xB8005448
#define VO_SUB1_HSYC_6_reg                                                           0xB800544C
#define VO_SUB1_HSYC_7_reg                                                           0xB8005450
#define set_VO_SUB1_HSYC_0_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_0_reg)=data)
#define set_VO_SUB1_HSYC_1_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_1_reg)=data)
#define set_VO_SUB1_HSYC_2_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_2_reg)=data)
#define set_VO_SUB1_HSYC_3_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_3_reg)=data)
#define set_VO_SUB1_HSYC_4_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_4_reg)=data)
#define set_VO_SUB1_HSYC_5_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_5_reg)=data)
#define set_VO_SUB1_HSYC_6_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_6_reg)=data)
#define set_VO_SUB1_HSYC_7_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSYC_7_reg)=data)
#define get_VO_SUB1_HSYC_0_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_0_reg))
#define get_VO_SUB1_HSYC_1_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_1_reg))
#define get_VO_SUB1_HSYC_2_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_2_reg))
#define get_VO_SUB1_HSYC_3_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_3_reg))
#define get_VO_SUB1_HSYC_4_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_4_reg))
#define get_VO_SUB1_HSYC_5_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_5_reg))
#define get_VO_SUB1_HSYC_6_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_6_reg))
#define get_VO_SUB1_HSYC_7_reg   (*((volatile unsigned int*) VO_SUB1_HSYC_7_reg))
#define VO_SUB1_HSYC_0_inst_adr                                                      "0x000D"
#define VO_SUB1_HSYC_1_inst_adr                                                      "0x000E"
#define VO_SUB1_HSYC_2_inst_adr                                                      "0x000F"
#define VO_SUB1_HSYC_3_inst_adr                                                      "0x0010"
#define VO_SUB1_HSYC_4_inst_adr                                                      "0x0011"
#define VO_SUB1_HSYC_5_inst_adr                                                      "0x0012"
#define VO_SUB1_HSYC_6_inst_adr                                                      "0x0013"
#define VO_SUB1_HSYC_7_inst_adr                                                      "0x0014"
#define VO_SUB1_HSYC_0_inst                                                          0x000D
#define VO_SUB1_HSYC_1_inst                                                          0x000E
#define VO_SUB1_HSYC_2_inst                                                          0x000F
#define VO_SUB1_HSYC_3_inst                                                          0x0010
#define VO_SUB1_HSYC_4_inst                                                          0x0011
#define VO_SUB1_HSYC_5_inst                                                          0x0012
#define VO_SUB1_HSYC_6_inst                                                          0x0013
#define VO_SUB1_HSYC_7_inst                                                          0x0014
#define VO_SUB1_HSYC_c1_shift                                                        (16)
#define VO_SUB1_HSYC_c1_mask                                                         (0x3FFF0000)
#define VO_SUB1_HSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_SUB1_HSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_HSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_HSYC_c0_shift                                                        (0)
#define VO_SUB1_HSYC_c0_mask                                                         (0x00003FFF)
#define VO_SUB1_HSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_SUB1_HSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_SUB1_HSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_SUB1_HSCC_0                                                               0x18005454
#define VO_SUB1_HSCC_1                                                               0x18005458
#define VO_SUB1_HSCC_2                                                               0x1800545C
#define VO_SUB1_HSCC_3                                                               0x18005460
#define VO_SUB1_HSCC_4                                                               0x18005464
#define VO_SUB1_HSCC_5                                                               0x18005468
#define VO_SUB1_HSCC_6                                                               0x1800546C
#define VO_SUB1_HSCC_7                                                               0x18005470
#define VO_SUB1_HSCC_0_reg_addr                                                      "0xB8005454"
#define VO_SUB1_HSCC_1_reg_addr                                                      "0xB8005458"
#define VO_SUB1_HSCC_2_reg_addr                                                      "0xB800545C"
#define VO_SUB1_HSCC_3_reg_addr                                                      "0xB8005460"
#define VO_SUB1_HSCC_4_reg_addr                                                      "0xB8005464"
#define VO_SUB1_HSCC_5_reg_addr                                                      "0xB8005468"
#define VO_SUB1_HSCC_6_reg_addr                                                      "0xB800546C"
#define VO_SUB1_HSCC_7_reg_addr                                                      "0xB8005470"
#define VO_SUB1_HSCC_0_reg                                                           0xB8005454
#define VO_SUB1_HSCC_1_reg                                                           0xB8005458
#define VO_SUB1_HSCC_2_reg                                                           0xB800545C
#define VO_SUB1_HSCC_3_reg                                                           0xB8005460
#define VO_SUB1_HSCC_4_reg                                                           0xB8005464
#define VO_SUB1_HSCC_5_reg                                                           0xB8005468
#define VO_SUB1_HSCC_6_reg                                                           0xB800546C
#define VO_SUB1_HSCC_7_reg                                                           0xB8005470
#define set_VO_SUB1_HSCC_0_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_0_reg)=data)
#define set_VO_SUB1_HSCC_1_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_1_reg)=data)
#define set_VO_SUB1_HSCC_2_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_2_reg)=data)
#define set_VO_SUB1_HSCC_3_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_3_reg)=data)
#define set_VO_SUB1_HSCC_4_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_4_reg)=data)
#define set_VO_SUB1_HSCC_5_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_5_reg)=data)
#define set_VO_SUB1_HSCC_6_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_6_reg)=data)
#define set_VO_SUB1_HSCC_7_reg(data)   (*((volatile unsigned int*) VO_SUB1_HSCC_7_reg)=data)
#define get_VO_SUB1_HSCC_0_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_0_reg))
#define get_VO_SUB1_HSCC_1_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_1_reg))
#define get_VO_SUB1_HSCC_2_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_2_reg))
#define get_VO_SUB1_HSCC_3_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_3_reg))
#define get_VO_SUB1_HSCC_4_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_4_reg))
#define get_VO_SUB1_HSCC_5_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_5_reg))
#define get_VO_SUB1_HSCC_6_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_6_reg))
#define get_VO_SUB1_HSCC_7_reg   (*((volatile unsigned int*) VO_SUB1_HSCC_7_reg))
#define VO_SUB1_HSCC_0_inst_adr                                                      "0x0015"
#define VO_SUB1_HSCC_1_inst_adr                                                      "0x0016"
#define VO_SUB1_HSCC_2_inst_adr                                                      "0x0017"
#define VO_SUB1_HSCC_3_inst_adr                                                      "0x0018"
#define VO_SUB1_HSCC_4_inst_adr                                                      "0x0019"
#define VO_SUB1_HSCC_5_inst_adr                                                      "0x001A"
#define VO_SUB1_HSCC_6_inst_adr                                                      "0x001B"
#define VO_SUB1_HSCC_7_inst_adr                                                      "0x001C"
#define VO_SUB1_HSCC_0_inst                                                          0x0015
#define VO_SUB1_HSCC_1_inst                                                          0x0016
#define VO_SUB1_HSCC_2_inst                                                          0x0017
#define VO_SUB1_HSCC_3_inst                                                          0x0018
#define VO_SUB1_HSCC_4_inst                                                          0x0019
#define VO_SUB1_HSCC_5_inst                                                          0x001A
#define VO_SUB1_HSCC_6_inst                                                          0x001B
#define VO_SUB1_HSCC_7_inst                                                          0x001C
#define VO_SUB1_HSCC_c1_shift                                                        (16)
#define VO_SUB1_HSCC_c1_mask                                                         (0x3FFF0000)
#define VO_SUB1_HSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_SUB1_HSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_HSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_SUB1_HSCC_c0_shift                                                        (0)
#define VO_SUB1_HSCC_c0_mask                                                         (0x00003FFF)
#define VO_SUB1_HSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_SUB1_HSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_SUB1_HSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD1                                                                      0x18005474
#define VO_OSD1_reg_addr                                                             "0xB8005474"
#define VO_OSD1_reg                                                                  0xB8005474
#define set_VO_OSD1_reg(data)   (*((volatile unsigned int*) VO_OSD1_reg)=data)
#define get_VO_OSD1_reg   (*((volatile unsigned int*) VO_OSD1_reg))
#define VO_OSD1_inst_adr                                                             "0x001D"
#define VO_OSD1_inst                                                                 0x001D
#define VO_OSD1_d3_dbh_shift                                                         (28)
#define VO_OSD1_d3_dbh_mask                                                          (0x10000000)
#define VO_OSD1_d3_dbh(data)                                                         (0x10000000&((data)<<28))
#define VO_OSD1_d3_dbh_src(data)                                                     ((0x10000000&(data))>>28)
#define VO_OSD1_get_d3_dbh(data)                                                     ((0x10000000&(data))>>28)
#define VO_OSD1_d3_dbw_shift                                                         (27)
#define VO_OSD1_d3_dbw_mask                                                          (0x08000000)
#define VO_OSD1_d3_dbw(data)                                                         (0x08000000&((data)<<27))
#define VO_OSD1_d3_dbw_src(data)                                                     ((0x08000000&(data))>>27)
#define VO_OSD1_get_d3_dbw(data)                                                     ((0x08000000&(data))>>27)
#define VO_OSD1_d3_dup_shift                                                         (26)
#define VO_OSD1_d3_dup_mask                                                          (0x04000000)
#define VO_OSD1_d3_dup(data)                                                         (0x04000000&((data)<<26))
#define VO_OSD1_d3_dup_src(data)                                                     ((0x04000000&(data))>>26)
#define VO_OSD1_get_d3_dup(data)                                                     ((0x04000000&(data))>>26)
#define VO_OSD1_dvb_shift                                                            (17)
#define VO_OSD1_dvb_mask                                                             (0x00020000)
#define VO_OSD1_dvb(data)                                                            (0x00020000&((data)<<17))
#define VO_OSD1_dvb_src(data)                                                        ((0x00020000&(data))>>17)
#define VO_OSD1_get_dvb(data)                                                        ((0x00020000&(data))>>17)
#define VO_OSD1_osd_shift                                                            (16)
#define VO_OSD1_osd_mask                                                             (0x00010000)
#define VO_OSD1_osd(data)                                                            (0x00010000&((data)<<16))
#define VO_OSD1_osd_src(data)                                                        ((0x00010000&(data))>>16)
#define VO_OSD1_get_osd(data)                                                        ((0x00010000&(data))>>16)
#define VO_OSD1_fetch_1080i_shift                                                    (15)
#define VO_OSD1_fetch_1080i_mask                                                     (0x00008000)
#define VO_OSD1_fetch_1080i(data)                                                    (0x00008000&((data)<<15))
#define VO_OSD1_fetch_1080i_src(data)                                                ((0x00008000&(data))>>15)
#define VO_OSD1_get_fetch_1080i(data)                                                ((0x00008000&(data))>>15)
#define VO_OSD1_odd_1080i_shift                                                      (14)
#define VO_OSD1_odd_1080i_mask                                                       (0x00004000)
#define VO_OSD1_odd_1080i(data)                                                      (0x00004000&((data)<<14))
#define VO_OSD1_odd_1080i_src(data)                                                  ((0x00004000&(data))>>14)
#define VO_OSD1_get_odd_1080i(data)                                                  ((0x00004000&(data))>>14)
#define VO_OSD1_vs_shift                                                             (10)
#define VO_OSD1_vs_mask                                                              (0x00000400)
#define VO_OSD1_vs(data)                                                             (0x00000400&((data)<<10))
#define VO_OSD1_vs_src(data)                                                         ((0x00000400&(data))>>10)
#define VO_OSD1_get_vs(data)                                                         ((0x00000400&(data))>>10)
#define VO_OSD1_vs_near_shift                                                        (9)
#define VO_OSD1_vs_near_mask                                                         (0x00000200)
#define VO_OSD1_vs_near(data)                                                        (0x00000200&((data)<<9))
#define VO_OSD1_vs_near_src(data)                                                    ((0x00000200&(data))>>9)
#define VO_OSD1_get_vs_near(data)                                                    ((0x00000200&(data))>>9)
#define VO_OSD1_vs_yodd_shift                                                        (8)
#define VO_OSD1_vs_yodd_mask                                                         (0x00000100)
#define VO_OSD1_vs_yodd(data)                                                        (0x00000100&((data)<<8))
#define VO_OSD1_vs_yodd_src(data)                                                    ((0x00000100&(data))>>8)
#define VO_OSD1_get_vs_yodd(data)                                                    ((0x00000100&(data))>>8)
#define VO_OSD1_vs_codd_shift                                                        (7)
#define VO_OSD1_vs_codd_mask                                                         (0x00000080)
#define VO_OSD1_vs_codd(data)                                                        (0x00000080&((data)<<7))
#define VO_OSD1_vs_codd_src(data)                                                    ((0x00000080&(data))>>7)
#define VO_OSD1_get_vs_codd(data)                                                    ((0x00000080&(data))>>7)
#define VO_OSD1_hs_shift                                                             (6)
#define VO_OSD1_hs_mask                                                              (0x00000040)
#define VO_OSD1_hs(data)                                                             (0x00000040&((data)<<6))
#define VO_OSD1_hs_src(data)                                                         ((0x00000040&(data))>>6)
#define VO_OSD1_get_hs(data)                                                         ((0x00000040&(data))>>6)
#define VO_OSD1_hs_yodd_shift                                                        (5)
#define VO_OSD1_hs_yodd_mask                                                         (0x00000020)
#define VO_OSD1_hs_yodd(data)                                                        (0x00000020&((data)<<5))
#define VO_OSD1_hs_yodd_src(data)                                                    ((0x00000020&(data))>>5)
#define VO_OSD1_get_hs_yodd(data)                                                    ((0x00000020&(data))>>5)
#define VO_OSD1_hs_codd_shift                                                        (4)
#define VO_OSD1_hs_codd_mask                                                         (0x00000010)
#define VO_OSD1_hs_codd(data)                                                        (0x00000010&((data)<<4))
#define VO_OSD1_hs_codd_src(data)                                                    ((0x00000010&(data))>>4)
#define VO_OSD1_get_hs_codd(data)                                                    ((0x00000010&(data))>>4)
#define VO_OSD1_chroma1_shift                                                        (3)
#define VO_OSD1_chroma1_mask                                                         (0x00000008)
#define VO_OSD1_chroma1(data)                                                        (0x00000008&((data)<<3))
#define VO_OSD1_chroma1_src(data)                                                    ((0x00000008&(data))>>3)
#define VO_OSD1_get_chroma1(data)                                                    ((0x00000008&(data))>>3)
#define VO_OSD1_chroma0_shift                                                        (2)
#define VO_OSD1_chroma0_mask                                                         (0x00000004)
#define VO_OSD1_chroma0(data)                                                        (0x00000004&((data)<<2))
#define VO_OSD1_chroma0_src(data)                                                    ((0x00000004&(data))>>2)
#define VO_OSD1_get_chroma0(data)                                                    ((0x00000004&(data))>>2)
#define VO_OSD1_write_data_shift                                                     (0)
#define VO_OSD1_write_data_mask                                                      (0x00000001)
#define VO_OSD1_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_OSD1_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_OSD1_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_OSD1_START                                                                0x18005478
#define VO_OSD1_START_reg_addr                                                       "0xB8005478"
#define VO_OSD1_START_reg                                                            0xB8005478
#define set_VO_OSD1_START_reg(data)   (*((volatile unsigned int*) VO_OSD1_START_reg)=data)
#define get_VO_OSD1_START_reg   (*((volatile unsigned int*) VO_OSD1_START_reg))
#define VO_OSD1_START_inst_adr                                                       "0x001E"
#define VO_OSD1_START_inst                                                           0x001E
#define VO_OSD1_START_ref_shift                                                      (24)
#define VO_OSD1_START_ref_mask                                                       (0x03000000)
#define VO_OSD1_START_ref(data)                                                      (0x03000000&((data)<<24))
#define VO_OSD1_START_ref_src(data)                                                  ((0x03000000&(data))>>24)
#define VO_OSD1_START_get_ref(data)                                                  ((0x03000000&(data))>>24)
#define VO_OSD1_START_pos_shift                                                      (12)
#define VO_OSD1_START_pos_mask                                                       (0x007FF000)
#define VO_OSD1_START_pos(data)                                                      (0x007FF000&((data)<<12))
#define VO_OSD1_START_pos_src(data)                                                  ((0x007FF000&(data))>>12)
#define VO_OSD1_START_get_pos(data)                                                  ((0x007FF000&(data))>>12)
#define VO_OSD1_START_th_shift                                                       (0)
#define VO_OSD1_START_th_mask                                                        (0x000007FF)
#define VO_OSD1_START_th(data)                                                       (0x000007FF&((data)<<0))
#define VO_OSD1_START_th_src(data)                                                   ((0x000007FF&(data))>>0)
#define VO_OSD1_START_get_th(data)                                                   ((0x000007FF&(data))>>0)


#define VO_OSD1_WI                                                                   0x1800547C
#define VO_OSD1_WI_reg_addr                                                          "0xB800547C"
#define VO_OSD1_WI_reg                                                               0xB800547C
#define set_VO_OSD1_WI_reg(data)   (*((volatile unsigned int*) VO_OSD1_WI_reg)=data)
#define get_VO_OSD1_WI_reg   (*((volatile unsigned int*) VO_OSD1_WI_reg))
#define VO_OSD1_WI_inst_adr                                                          "0x001F"
#define VO_OSD1_WI_inst                                                              0x001F
#define VO_OSD1_WI_endian_shift                                                      (31)
#define VO_OSD1_WI_endian_mask                                                       (0x80000000)
#define VO_OSD1_WI_endian(data)                                                      (0x80000000&((data)<<31))
#define VO_OSD1_WI_endian_src(data)                                                  ((0x80000000&(data))>>31)
#define VO_OSD1_WI_get_endian(data)                                                  ((0x80000000&(data))>>31)
#define VO_OSD1_WI_a_shift                                                           (0)
#define VO_OSD1_WI_a_mask                                                            (0x7FFFFFFF)
#define VO_OSD1_WI_a(data)                                                           (0x7FFFFFFF&((data)<<0))
#define VO_OSD1_WI_a_src(data)                                                       ((0x7FFFFFFF&(data))>>0)
#define VO_OSD1_WI_get_a(data)                                                       ((0x7FFFFFFF&(data))>>0)


#define VO_OSD1_WI_3D                                                                0x18005480
#define VO_OSD1_WI_3D_reg_addr                                                       "0xB8005480"
#define VO_OSD1_WI_3D_reg                                                            0xB8005480
#define set_VO_OSD1_WI_3D_reg(data)   (*((volatile unsigned int*) VO_OSD1_WI_3D_reg)=data)
#define get_VO_OSD1_WI_3D_reg   (*((volatile unsigned int*) VO_OSD1_WI_3D_reg))
#define VO_OSD1_WI_3D_inst_adr                                                       "0x0020"
#define VO_OSD1_WI_3D_inst                                                           0x0020
#define VO_OSD1_WI_3D_a_shift                                                        (0)
#define VO_OSD1_WI_3D_a_mask                                                         (0x7FFFFFFF)
#define VO_OSD1_WI_3D_a(data)                                                        (0x7FFFFFFF&((data)<<0))
#define VO_OSD1_WI_3D_a_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define VO_OSD1_WI_3D_get_a(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define VO_OSD1_SIZE                                                                 0x18005484
#define VO_OSD1_SIZE_reg_addr                                                        "0xB8005484"
#define VO_OSD1_SIZE_reg                                                             0xB8005484
#define set_VO_OSD1_SIZE_reg(data)   (*((volatile unsigned int*) VO_OSD1_SIZE_reg)=data)
#define get_VO_OSD1_SIZE_reg   (*((volatile unsigned int*) VO_OSD1_SIZE_reg))
#define VO_OSD1_SIZE_inst_adr                                                        "0x0021"
#define VO_OSD1_SIZE_inst                                                            0x0021
#define VO_OSD1_SIZE_w_shift                                                         (12)
#define VO_OSD1_SIZE_w_mask                                                          (0x007FF000)
#define VO_OSD1_SIZE_w(data)                                                         (0x007FF000&((data)<<12))
#define VO_OSD1_SIZE_w_src(data)                                                     ((0x007FF000&(data))>>12)
#define VO_OSD1_SIZE_get_w(data)                                                     ((0x007FF000&(data))>>12)
#define VO_OSD1_SIZE_h_shift                                                         (0)
#define VO_OSD1_SIZE_h_mask                                                          (0x000007FF)
#define VO_OSD1_SIZE_h(data)                                                         (0x000007FF&((data)<<0))
#define VO_OSD1_SIZE_h_src(data)                                                     ((0x000007FF&(data))>>0)
#define VO_OSD1_SIZE_get_h(data)                                                     ((0x000007FF&(data))>>0)


#define VO_OSD1_BUF                                                                  0x18005488
#define VO_OSD1_BUF_reg_addr                                                         "0xB8005488"
#define VO_OSD1_BUF_reg                                                              0xB8005488
#define set_VO_OSD1_BUF_reg(data)   (*((volatile unsigned int*) VO_OSD1_BUF_reg)=data)
#define get_VO_OSD1_BUF_reg   (*((volatile unsigned int*) VO_OSD1_BUF_reg))
#define VO_OSD1_BUF_inst_adr                                                         "0x0022"
#define VO_OSD1_BUF_inst                                                             0x0022
#define VO_OSD1_BUF_th_shift                                                         (0)
#define VO_OSD1_BUF_th_mask                                                          (0x000001FF)
#define VO_OSD1_BUF_th(data)                                                         (0x000001FF&((data)<<0))
#define VO_OSD1_BUF_th_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_OSD1_BUF_get_th(data)                                                     ((0x000001FF&(data))>>0)


#define VO_OSD1_CC1                                                                  0x1800548C
#define VO_OSD1_CC1_reg_addr                                                         "0xB800548C"
#define VO_OSD1_CC1_reg                                                              0xB800548C
#define set_VO_OSD1_CC1_reg(data)   (*((volatile unsigned int*) VO_OSD1_CC1_reg)=data)
#define get_VO_OSD1_CC1_reg   (*((volatile unsigned int*) VO_OSD1_CC1_reg))
#define VO_OSD1_CC1_inst_adr                                                         "0x0023"
#define VO_OSD1_CC1_inst                                                             0x0023
#define VO_OSD1_CC1_m01_shift                                                        (12)
#define VO_OSD1_CC1_m01_mask                                                         (0x00FFF000)
#define VO_OSD1_CC1_m01(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD1_CC1_m01_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC1_get_m01(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC1_m00_shift                                                        (0)
#define VO_OSD1_CC1_m00_mask                                                         (0x00000FFF)
#define VO_OSD1_CC1_m00(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD1_CC1_m00_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD1_CC1_get_m00(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD1_CC2                                                                  0x18005490
#define VO_OSD1_CC2_reg_addr                                                         "0xB8005490"
#define VO_OSD1_CC2_reg                                                              0xB8005490
#define set_VO_OSD1_CC2_reg(data)   (*((volatile unsigned int*) VO_OSD1_CC2_reg)=data)
#define get_VO_OSD1_CC2_reg   (*((volatile unsigned int*) VO_OSD1_CC2_reg))
#define VO_OSD1_CC2_inst_adr                                                         "0x0024"
#define VO_OSD1_CC2_inst                                                             0x0024
#define VO_OSD1_CC2_m10_shift                                                        (12)
#define VO_OSD1_CC2_m10_mask                                                         (0x00FFF000)
#define VO_OSD1_CC2_m10(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD1_CC2_m10_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC2_get_m10(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC2_m02_shift                                                        (0)
#define VO_OSD1_CC2_m02_mask                                                         (0x00000FFF)
#define VO_OSD1_CC2_m02(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD1_CC2_m02_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD1_CC2_get_m02(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD1_CC3                                                                  0x18005494
#define VO_OSD1_CC3_reg_addr                                                         "0xB8005494"
#define VO_OSD1_CC3_reg                                                              0xB8005494
#define set_VO_OSD1_CC3_reg(data)   (*((volatile unsigned int*) VO_OSD1_CC3_reg)=data)
#define get_VO_OSD1_CC3_reg   (*((volatile unsigned int*) VO_OSD1_CC3_reg))
#define VO_OSD1_CC3_inst_adr                                                         "0x0025"
#define VO_OSD1_CC3_inst                                                             0x0025
#define VO_OSD1_CC3_m12_shift                                                        (12)
#define VO_OSD1_CC3_m12_mask                                                         (0x00FFF000)
#define VO_OSD1_CC3_m12(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD1_CC3_m12_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC3_get_m12(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC3_m11_shift                                                        (0)
#define VO_OSD1_CC3_m11_mask                                                         (0x00000FFF)
#define VO_OSD1_CC3_m11(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD1_CC3_m11_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD1_CC3_get_m11(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD1_CC4                                                                  0x18005498
#define VO_OSD1_CC4_reg_addr                                                         "0xB8005498"
#define VO_OSD1_CC4_reg                                                              0xB8005498
#define set_VO_OSD1_CC4_reg(data)   (*((volatile unsigned int*) VO_OSD1_CC4_reg)=data)
#define get_VO_OSD1_CC4_reg   (*((volatile unsigned int*) VO_OSD1_CC4_reg))
#define VO_OSD1_CC4_inst_adr                                                         "0x0026"
#define VO_OSD1_CC4_inst                                                             0x0026
#define VO_OSD1_CC4_m21_shift                                                        (12)
#define VO_OSD1_CC4_m21_mask                                                         (0x00FFF000)
#define VO_OSD1_CC4_m21(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD1_CC4_m21_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC4_get_m21(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD1_CC4_m20_shift                                                        (0)
#define VO_OSD1_CC4_m20_mask                                                         (0x00000FFF)
#define VO_OSD1_CC4_m20(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD1_CC4_m20_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD1_CC4_get_m20(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD1_CC5                                                                  0x1800549C
#define VO_OSD1_CC5_reg_addr                                                         "0xB800549C"
#define VO_OSD1_CC5_reg                                                              0xB800549C
#define set_VO_OSD1_CC5_reg(data)   (*((volatile unsigned int*) VO_OSD1_CC5_reg)=data)
#define get_VO_OSD1_CC5_reg   (*((volatile unsigned int*) VO_OSD1_CC5_reg))
#define VO_OSD1_CC5_inst_adr                                                         "0x0027"
#define VO_OSD1_CC5_inst                                                             0x0027
#define VO_OSD1_CC5_m22_shift                                                        (0)
#define VO_OSD1_CC5_m22_mask                                                         (0x00000FFF)
#define VO_OSD1_CC5_m22(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD1_CC5_m22_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD1_CC5_get_m22(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD1_CC6                                                                  0x180054A0
#define VO_OSD1_CC6_reg_addr                                                         "0xB80054A0"
#define VO_OSD1_CC6_reg                                                              0xB80054A0
#define set_VO_OSD1_CC6_reg(data)   (*((volatile unsigned int*) VO_OSD1_CC6_reg)=data)
#define get_VO_OSD1_CC6_reg   (*((volatile unsigned int*) VO_OSD1_CC6_reg))
#define VO_OSD1_CC6_inst_adr                                                         "0x0028"
#define VO_OSD1_CC6_inst                                                             0x0028
#define VO_OSD1_CC6_a2_shift                                                         (18)
#define VO_OSD1_CC6_a2_mask                                                          (0x07FC0000)
#define VO_OSD1_CC6_a2(data)                                                         (0x07FC0000&((data)<<18))
#define VO_OSD1_CC6_a2_src(data)                                                     ((0x07FC0000&(data))>>18)
#define VO_OSD1_CC6_get_a2(data)                                                     ((0x07FC0000&(data))>>18)
#define VO_OSD1_CC6_a1_shift                                                         (9)
#define VO_OSD1_CC6_a1_mask                                                          (0x0003FE00)
#define VO_OSD1_CC6_a1(data)                                                         (0x0003FE00&((data)<<9))
#define VO_OSD1_CC6_a1_src(data)                                                     ((0x0003FE00&(data))>>9)
#define VO_OSD1_CC6_get_a1(data)                                                     ((0x0003FE00&(data))>>9)
#define VO_OSD1_CC6_a0_shift                                                         (0)
#define VO_OSD1_CC6_a0_mask                                                          (0x000001FF)
#define VO_OSD1_CC6_a0(data)                                                         (0x000001FF&((data)<<0))
#define VO_OSD1_CC6_a0_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_OSD1_CC6_get_a0(data)                                                     ((0x000001FF&(data))>>0)


#define VO_OSD1_VSI                                                                  0x180054A4
#define VO_OSD1_VSI_reg_addr                                                         "0xB80054A4"
#define VO_OSD1_VSI_reg                                                              0xB80054A4
#define set_VO_OSD1_VSI_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSI_reg)=data)
#define get_VO_OSD1_VSI_reg   (*((volatile unsigned int*) VO_OSD1_VSI_reg))
#define VO_OSD1_VSI_inst_adr                                                         "0x0029"
#define VO_OSD1_VSI_inst                                                             0x0029
#define VO_OSD1_VSI_offset_shift                                                     (16)
#define VO_OSD1_VSI_offset_mask                                                      (0x07FF0000)
#define VO_OSD1_VSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_OSD1_VSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD1_VSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD1_VSI_phase_shift                                                      (2)
#define VO_OSD1_VSI_phase_mask                                                       (0x0000FFFC)
#define VO_OSD1_VSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_OSD1_VSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_OSD1_VSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_OSD1_VSD                                                                  0x180054A8
#define VO_OSD1_VSD_reg_addr                                                         "0xB80054A8"
#define VO_OSD1_VSD_reg                                                              0xB80054A8
#define set_VO_OSD1_VSD_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSD_reg)=data)
#define get_VO_OSD1_VSD_reg   (*((volatile unsigned int*) VO_OSD1_VSD_reg))
#define VO_OSD1_VSD_inst_adr                                                         "0x002A"
#define VO_OSD1_VSD_inst                                                             0x002A
#define VO_OSD1_VSD_out_shift                                                        (20)
#define VO_OSD1_VSD_out_mask                                                         (0x7FF00000)
#define VO_OSD1_VSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_OSD1_VSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD1_VSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD1_VSD_delta_shift                                                      (0)
#define VO_OSD1_VSD_delta_mask                                                       (0x0007FFFF)
#define VO_OSD1_VSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_OSD1_VSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_OSD1_VSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_OSD1_VSYC_0                                                               0x180054AC
#define VO_OSD1_VSYC_1                                                               0x180054B0
#define VO_OSD1_VSYC_2                                                               0x180054B4
#define VO_OSD1_VSYC_3                                                               0x180054B8
#define VO_OSD1_VSYC_0_reg_addr                                                      "0xB80054AC"
#define VO_OSD1_VSYC_1_reg_addr                                                      "0xB80054B0"
#define VO_OSD1_VSYC_2_reg_addr                                                      "0xB80054B4"
#define VO_OSD1_VSYC_3_reg_addr                                                      "0xB80054B8"
#define VO_OSD1_VSYC_0_reg                                                           0xB80054AC
#define VO_OSD1_VSYC_1_reg                                                           0xB80054B0
#define VO_OSD1_VSYC_2_reg                                                           0xB80054B4
#define VO_OSD1_VSYC_3_reg                                                           0xB80054B8
#define set_VO_OSD1_VSYC_0_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSYC_0_reg)=data)
#define set_VO_OSD1_VSYC_1_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSYC_1_reg)=data)
#define set_VO_OSD1_VSYC_2_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSYC_2_reg)=data)
#define set_VO_OSD1_VSYC_3_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSYC_3_reg)=data)
#define get_VO_OSD1_VSYC_0_reg   (*((volatile unsigned int*) VO_OSD1_VSYC_0_reg))
#define get_VO_OSD1_VSYC_1_reg   (*((volatile unsigned int*) VO_OSD1_VSYC_1_reg))
#define get_VO_OSD1_VSYC_2_reg   (*((volatile unsigned int*) VO_OSD1_VSYC_2_reg))
#define get_VO_OSD1_VSYC_3_reg   (*((volatile unsigned int*) VO_OSD1_VSYC_3_reg))
#define VO_OSD1_VSYC_0_inst_adr                                                      "0x002B"
#define VO_OSD1_VSYC_1_inst_adr                                                      "0x002C"
#define VO_OSD1_VSYC_2_inst_adr                                                      "0x002D"
#define VO_OSD1_VSYC_3_inst_adr                                                      "0x002E"
#define VO_OSD1_VSYC_0_inst                                                          0x002B
#define VO_OSD1_VSYC_1_inst                                                          0x002C
#define VO_OSD1_VSYC_2_inst                                                          0x002D
#define VO_OSD1_VSYC_3_inst                                                          0x002E
#define VO_OSD1_VSYC_c1_shift                                                        (16)
#define VO_OSD1_VSYC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD1_VSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD1_VSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_VSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_VSYC_c0_shift                                                        (0)
#define VO_OSD1_VSYC_c0_mask                                                         (0x00003FFF)
#define VO_OSD1_VSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD1_VSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD1_VSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD1_VSCC_0                                                               0x180054BC
#define VO_OSD1_VSCC_1                                                               0x180054C0
#define VO_OSD1_VSCC_2                                                               0x180054C4
#define VO_OSD1_VSCC_3                                                               0x180054C8
#define VO_OSD1_VSCC_0_reg_addr                                                      "0xB80054BC"
#define VO_OSD1_VSCC_1_reg_addr                                                      "0xB80054C0"
#define VO_OSD1_VSCC_2_reg_addr                                                      "0xB80054C4"
#define VO_OSD1_VSCC_3_reg_addr                                                      "0xB80054C8"
#define VO_OSD1_VSCC_0_reg                                                           0xB80054BC
#define VO_OSD1_VSCC_1_reg                                                           0xB80054C0
#define VO_OSD1_VSCC_2_reg                                                           0xB80054C4
#define VO_OSD1_VSCC_3_reg                                                           0xB80054C8
#define set_VO_OSD1_VSCC_0_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSCC_0_reg)=data)
#define set_VO_OSD1_VSCC_1_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSCC_1_reg)=data)
#define set_VO_OSD1_VSCC_2_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSCC_2_reg)=data)
#define set_VO_OSD1_VSCC_3_reg(data)   (*((volatile unsigned int*) VO_OSD1_VSCC_3_reg)=data)
#define get_VO_OSD1_VSCC_0_reg   (*((volatile unsigned int*) VO_OSD1_VSCC_0_reg))
#define get_VO_OSD1_VSCC_1_reg   (*((volatile unsigned int*) VO_OSD1_VSCC_1_reg))
#define get_VO_OSD1_VSCC_2_reg   (*((volatile unsigned int*) VO_OSD1_VSCC_2_reg))
#define get_VO_OSD1_VSCC_3_reg   (*((volatile unsigned int*) VO_OSD1_VSCC_3_reg))
#define VO_OSD1_VSCC_0_inst_adr                                                      "0x002F"
#define VO_OSD1_VSCC_1_inst_adr                                                      "0x0030"
#define VO_OSD1_VSCC_2_inst_adr                                                      "0x0031"
#define VO_OSD1_VSCC_3_inst_adr                                                      "0x0032"
#define VO_OSD1_VSCC_0_inst                                                          0x002F
#define VO_OSD1_VSCC_1_inst                                                          0x0030
#define VO_OSD1_VSCC_2_inst                                                          0x0031
#define VO_OSD1_VSCC_3_inst                                                          0x0032
#define VO_OSD1_VSCC_c1_shift                                                        (16)
#define VO_OSD1_VSCC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD1_VSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD1_VSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_VSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_VSCC_c0_shift                                                        (0)
#define VO_OSD1_VSCC_c0_mask                                                         (0x00003FFF)
#define VO_OSD1_VSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD1_VSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD1_VSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD1_HSI                                                                  0x180054CC
#define VO_OSD1_HSI_reg_addr                                                         "0xB80054CC"
#define VO_OSD1_HSI_reg                                                              0xB80054CC
#define set_VO_OSD1_HSI_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSI_reg)=data)
#define get_VO_OSD1_HSI_reg   (*((volatile unsigned int*) VO_OSD1_HSI_reg))
#define VO_OSD1_HSI_inst_adr                                                         "0x0033"
#define VO_OSD1_HSI_inst                                                             0x0033
#define VO_OSD1_HSI_offset_shift                                                     (16)
#define VO_OSD1_HSI_offset_mask                                                      (0x07FF0000)
#define VO_OSD1_HSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_OSD1_HSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD1_HSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD1_HSI_phase_shift                                                      (2)
#define VO_OSD1_HSI_phase_mask                                                       (0x0000FFFC)
#define VO_OSD1_HSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_OSD1_HSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_OSD1_HSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_OSD1_HSD                                                                  0x180054D0
#define VO_OSD1_HSD_reg_addr                                                         "0xB80054D0"
#define VO_OSD1_HSD_reg                                                              0xB80054D0
#define set_VO_OSD1_HSD_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSD_reg)=data)
#define get_VO_OSD1_HSD_reg   (*((volatile unsigned int*) VO_OSD1_HSD_reg))
#define VO_OSD1_HSD_inst_adr                                                         "0x0034"
#define VO_OSD1_HSD_inst                                                             0x0034
#define VO_OSD1_HSD_out_shift                                                        (20)
#define VO_OSD1_HSD_out_mask                                                         (0x7FF00000)
#define VO_OSD1_HSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_OSD1_HSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD1_HSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD1_HSD_delta_shift                                                      (0)
#define VO_OSD1_HSD_delta_mask                                                       (0x0007FFFF)
#define VO_OSD1_HSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_OSD1_HSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_OSD1_HSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_OSD1_HSYC_0                                                               0x180054D4
#define VO_OSD1_HSYC_1                                                               0x180054D8
#define VO_OSD1_HSYC_2                                                               0x180054DC
#define VO_OSD1_HSYC_3                                                               0x180054E0
#define VO_OSD1_HSYC_4                                                               0x180054E4
#define VO_OSD1_HSYC_5                                                               0x180054E8
#define VO_OSD1_HSYC_6                                                               0x180054EC
#define VO_OSD1_HSYC_7                                                               0x180054F0
#define VO_OSD1_HSYC_0_reg_addr                                                      "0xB80054D4"
#define VO_OSD1_HSYC_1_reg_addr                                                      "0xB80054D8"
#define VO_OSD1_HSYC_2_reg_addr                                                      "0xB80054DC"
#define VO_OSD1_HSYC_3_reg_addr                                                      "0xB80054E0"
#define VO_OSD1_HSYC_4_reg_addr                                                      "0xB80054E4"
#define VO_OSD1_HSYC_5_reg_addr                                                      "0xB80054E8"
#define VO_OSD1_HSYC_6_reg_addr                                                      "0xB80054EC"
#define VO_OSD1_HSYC_7_reg_addr                                                      "0xB80054F0"
#define VO_OSD1_HSYC_0_reg                                                           0xB80054D4
#define VO_OSD1_HSYC_1_reg                                                           0xB80054D8
#define VO_OSD1_HSYC_2_reg                                                           0xB80054DC
#define VO_OSD1_HSYC_3_reg                                                           0xB80054E0
#define VO_OSD1_HSYC_4_reg                                                           0xB80054E4
#define VO_OSD1_HSYC_5_reg                                                           0xB80054E8
#define VO_OSD1_HSYC_6_reg                                                           0xB80054EC
#define VO_OSD1_HSYC_7_reg                                                           0xB80054F0
#define set_VO_OSD1_HSYC_0_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_0_reg)=data)
#define set_VO_OSD1_HSYC_1_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_1_reg)=data)
#define set_VO_OSD1_HSYC_2_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_2_reg)=data)
#define set_VO_OSD1_HSYC_3_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_3_reg)=data)
#define set_VO_OSD1_HSYC_4_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_4_reg)=data)
#define set_VO_OSD1_HSYC_5_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_5_reg)=data)
#define set_VO_OSD1_HSYC_6_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_6_reg)=data)
#define set_VO_OSD1_HSYC_7_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSYC_7_reg)=data)
#define get_VO_OSD1_HSYC_0_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_0_reg))
#define get_VO_OSD1_HSYC_1_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_1_reg))
#define get_VO_OSD1_HSYC_2_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_2_reg))
#define get_VO_OSD1_HSYC_3_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_3_reg))
#define get_VO_OSD1_HSYC_4_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_4_reg))
#define get_VO_OSD1_HSYC_5_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_5_reg))
#define get_VO_OSD1_HSYC_6_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_6_reg))
#define get_VO_OSD1_HSYC_7_reg   (*((volatile unsigned int*) VO_OSD1_HSYC_7_reg))
#define VO_OSD1_HSYC_0_inst_adr                                                      "0x0035"
#define VO_OSD1_HSYC_1_inst_adr                                                      "0x0036"
#define VO_OSD1_HSYC_2_inst_adr                                                      "0x0037"
#define VO_OSD1_HSYC_3_inst_adr                                                      "0x0038"
#define VO_OSD1_HSYC_4_inst_adr                                                      "0x0039"
#define VO_OSD1_HSYC_5_inst_adr                                                      "0x003A"
#define VO_OSD1_HSYC_6_inst_adr                                                      "0x003B"
#define VO_OSD1_HSYC_7_inst_adr                                                      "0x003C"
#define VO_OSD1_HSYC_0_inst                                                          0x0035
#define VO_OSD1_HSYC_1_inst                                                          0x0036
#define VO_OSD1_HSYC_2_inst                                                          0x0037
#define VO_OSD1_HSYC_3_inst                                                          0x0038
#define VO_OSD1_HSYC_4_inst                                                          0x0039
#define VO_OSD1_HSYC_5_inst                                                          0x003A
#define VO_OSD1_HSYC_6_inst                                                          0x003B
#define VO_OSD1_HSYC_7_inst                                                          0x003C
#define VO_OSD1_HSYC_c1_shift                                                        (16)
#define VO_OSD1_HSYC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD1_HSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD1_HSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_HSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_HSYC_c0_shift                                                        (0)
#define VO_OSD1_HSYC_c0_mask                                                         (0x00003FFF)
#define VO_OSD1_HSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD1_HSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD1_HSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD1_HSCC_0                                                               0x180054F4
#define VO_OSD1_HSCC_1                                                               0x180054F8
#define VO_OSD1_HSCC_2                                                               0x180054FC
#define VO_OSD1_HSCC_3                                                               0x18005500
#define VO_OSD1_HSCC_4                                                               0x18005504
#define VO_OSD1_HSCC_5                                                               0x18005508
#define VO_OSD1_HSCC_6                                                               0x1800550C
#define VO_OSD1_HSCC_7                                                               0x18005510
#define VO_OSD1_HSCC_0_reg_addr                                                      "0xB80054F4"
#define VO_OSD1_HSCC_1_reg_addr                                                      "0xB80054F8"
#define VO_OSD1_HSCC_2_reg_addr                                                      "0xB80054FC"
#define VO_OSD1_HSCC_3_reg_addr                                                      "0xB8005500"
#define VO_OSD1_HSCC_4_reg_addr                                                      "0xB8005504"
#define VO_OSD1_HSCC_5_reg_addr                                                      "0xB8005508"
#define VO_OSD1_HSCC_6_reg_addr                                                      "0xB800550C"
#define VO_OSD1_HSCC_7_reg_addr                                                      "0xB8005510"
#define VO_OSD1_HSCC_0_reg                                                           0xB80054F4
#define VO_OSD1_HSCC_1_reg                                                           0xB80054F8
#define VO_OSD1_HSCC_2_reg                                                           0xB80054FC
#define VO_OSD1_HSCC_3_reg                                                           0xB8005500
#define VO_OSD1_HSCC_4_reg                                                           0xB8005504
#define VO_OSD1_HSCC_5_reg                                                           0xB8005508
#define VO_OSD1_HSCC_6_reg                                                           0xB800550C
#define VO_OSD1_HSCC_7_reg                                                           0xB8005510
#define set_VO_OSD1_HSCC_0_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_0_reg)=data)
#define set_VO_OSD1_HSCC_1_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_1_reg)=data)
#define set_VO_OSD1_HSCC_2_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_2_reg)=data)
#define set_VO_OSD1_HSCC_3_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_3_reg)=data)
#define set_VO_OSD1_HSCC_4_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_4_reg)=data)
#define set_VO_OSD1_HSCC_5_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_5_reg)=data)
#define set_VO_OSD1_HSCC_6_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_6_reg)=data)
#define set_VO_OSD1_HSCC_7_reg(data)   (*((volatile unsigned int*) VO_OSD1_HSCC_7_reg)=data)
#define get_VO_OSD1_HSCC_0_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_0_reg))
#define get_VO_OSD1_HSCC_1_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_1_reg))
#define get_VO_OSD1_HSCC_2_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_2_reg))
#define get_VO_OSD1_HSCC_3_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_3_reg))
#define get_VO_OSD1_HSCC_4_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_4_reg))
#define get_VO_OSD1_HSCC_5_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_5_reg))
#define get_VO_OSD1_HSCC_6_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_6_reg))
#define get_VO_OSD1_HSCC_7_reg   (*((volatile unsigned int*) VO_OSD1_HSCC_7_reg))
#define VO_OSD1_HSCC_0_inst_adr                                                      "0x003D"
#define VO_OSD1_HSCC_1_inst_adr                                                      "0x003E"
#define VO_OSD1_HSCC_2_inst_adr                                                      "0x003F"
#define VO_OSD1_HSCC_3_inst_adr                                                      "0x0040"
#define VO_OSD1_HSCC_4_inst_adr                                                      "0x0041"
#define VO_OSD1_HSCC_5_inst_adr                                                      "0x0042"
#define VO_OSD1_HSCC_6_inst_adr                                                      "0x0043"
#define VO_OSD1_HSCC_7_inst_adr                                                      "0x0044"
#define VO_OSD1_HSCC_0_inst                                                          0x003D
#define VO_OSD1_HSCC_1_inst                                                          0x003E
#define VO_OSD1_HSCC_2_inst                                                          0x003F
#define VO_OSD1_HSCC_3_inst                                                          0x0040
#define VO_OSD1_HSCC_4_inst                                                          0x0041
#define VO_OSD1_HSCC_5_inst                                                          0x0042
#define VO_OSD1_HSCC_6_inst                                                          0x0043
#define VO_OSD1_HSCC_7_inst                                                          0x0044
#define VO_OSD1_HSCC_c1_shift                                                        (16)
#define VO_OSD1_HSCC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD1_HSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD1_HSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_HSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD1_HSCC_c0_shift                                                        (0)
#define VO_OSD1_HSCC_c0_mask                                                         (0x00003FFF)
#define VO_OSD1_HSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD1_HSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD1_HSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD2                                                                      0x18005514
#define VO_OSD2_reg_addr                                                             "0xB8005514"
#define VO_OSD2_reg                                                                  0xB8005514
#define set_VO_OSD2_reg(data)   (*((volatile unsigned int*) VO_OSD2_reg)=data)
#define get_VO_OSD2_reg   (*((volatile unsigned int*) VO_OSD2_reg))
#define VO_OSD2_inst_adr                                                             "0x0045"
#define VO_OSD2_inst                                                                 0x0045
#define VO_OSD2_d3_dbh_shift                                                         (28)
#define VO_OSD2_d3_dbh_mask                                                          (0x10000000)
#define VO_OSD2_d3_dbh(data)                                                         (0x10000000&((data)<<28))
#define VO_OSD2_d3_dbh_src(data)                                                     ((0x10000000&(data))>>28)
#define VO_OSD2_get_d3_dbh(data)                                                     ((0x10000000&(data))>>28)
#define VO_OSD2_d3_dbw_shift                                                         (27)
#define VO_OSD2_d3_dbw_mask                                                          (0x08000000)
#define VO_OSD2_d3_dbw(data)                                                         (0x08000000&((data)<<27))
#define VO_OSD2_d3_dbw_src(data)                                                     ((0x08000000&(data))>>27)
#define VO_OSD2_get_d3_dbw(data)                                                     ((0x08000000&(data))>>27)
#define VO_OSD2_d3_dup_shift                                                         (26)
#define VO_OSD2_d3_dup_mask                                                          (0x04000000)
#define VO_OSD2_d3_dup(data)                                                         (0x04000000&((data)<<26))
#define VO_OSD2_d3_dup_src(data)                                                     ((0x04000000&(data))>>26)
#define VO_OSD2_get_d3_dup(data)                                                     ((0x04000000&(data))>>26)
#define VO_OSD2_o422_shift                                                           (21)
#define VO_OSD2_o422_mask                                                            (0x00200000)
#define VO_OSD2_o422(data)                                                           (0x00200000&((data)<<21))
#define VO_OSD2_o422_src(data)                                                       ((0x00200000&(data))>>21)
#define VO_OSD2_get_o422(data)                                                       ((0x00200000&(data))>>21)
#define VO_OSD2_dvb_shift                                                            (17)
#define VO_OSD2_dvb_mask                                                             (0x00020000)
#define VO_OSD2_dvb(data)                                                            (0x00020000&((data)<<17))
#define VO_OSD2_dvb_src(data)                                                        ((0x00020000&(data))>>17)
#define VO_OSD2_get_dvb(data)                                                        ((0x00020000&(data))>>17)
#define VO_OSD2_osd_shift                                                            (16)
#define VO_OSD2_osd_mask                                                             (0x00010000)
#define VO_OSD2_osd(data)                                                            (0x00010000&((data)<<16))
#define VO_OSD2_osd_src(data)                                                        ((0x00010000&(data))>>16)
#define VO_OSD2_get_osd(data)                                                        ((0x00010000&(data))>>16)
#define VO_OSD2_fetch_1080i_shift                                                    (15)
#define VO_OSD2_fetch_1080i_mask                                                     (0x00008000)
#define VO_OSD2_fetch_1080i(data)                                                    (0x00008000&((data)<<15))
#define VO_OSD2_fetch_1080i_src(data)                                                ((0x00008000&(data))>>15)
#define VO_OSD2_get_fetch_1080i(data)                                                ((0x00008000&(data))>>15)
#define VO_OSD2_odd_1080i_shift                                                      (14)
#define VO_OSD2_odd_1080i_mask                                                       (0x00004000)
#define VO_OSD2_odd_1080i(data)                                                      (0x00004000&((data)<<14))
#define VO_OSD2_odd_1080i_src(data)                                                  ((0x00004000&(data))>>14)
#define VO_OSD2_get_odd_1080i(data)                                                  ((0x00004000&(data))>>14)
#define VO_OSD2_vs_shift                                                             (10)
#define VO_OSD2_vs_mask                                                              (0x00000400)
#define VO_OSD2_vs(data)                                                             (0x00000400&((data)<<10))
#define VO_OSD2_vs_src(data)                                                         ((0x00000400&(data))>>10)
#define VO_OSD2_get_vs(data)                                                         ((0x00000400&(data))>>10)
#define VO_OSD2_vs_near_shift                                                        (9)
#define VO_OSD2_vs_near_mask                                                         (0x00000200)
#define VO_OSD2_vs_near(data)                                                        (0x00000200&((data)<<9))
#define VO_OSD2_vs_near_src(data)                                                    ((0x00000200&(data))>>9)
#define VO_OSD2_get_vs_near(data)                                                    ((0x00000200&(data))>>9)
#define VO_OSD2_vs_yodd_shift                                                        (8)
#define VO_OSD2_vs_yodd_mask                                                         (0x00000100)
#define VO_OSD2_vs_yodd(data)                                                        (0x00000100&((data)<<8))
#define VO_OSD2_vs_yodd_src(data)                                                    ((0x00000100&(data))>>8)
#define VO_OSD2_get_vs_yodd(data)                                                    ((0x00000100&(data))>>8)
#define VO_OSD2_vs_codd_shift                                                        (7)
#define VO_OSD2_vs_codd_mask                                                         (0x00000080)
#define VO_OSD2_vs_codd(data)                                                        (0x00000080&((data)<<7))
#define VO_OSD2_vs_codd_src(data)                                                    ((0x00000080&(data))>>7)
#define VO_OSD2_get_vs_codd(data)                                                    ((0x00000080&(data))>>7)
#define VO_OSD2_hs_shift                                                             (6)
#define VO_OSD2_hs_mask                                                              (0x00000040)
#define VO_OSD2_hs(data)                                                             (0x00000040&((data)<<6))
#define VO_OSD2_hs_src(data)                                                         ((0x00000040&(data))>>6)
#define VO_OSD2_get_hs(data)                                                         ((0x00000040&(data))>>6)
#define VO_OSD2_hs_yodd_shift                                                        (5)
#define VO_OSD2_hs_yodd_mask                                                         (0x00000020)
#define VO_OSD2_hs_yodd(data)                                                        (0x00000020&((data)<<5))
#define VO_OSD2_hs_yodd_src(data)                                                    ((0x00000020&(data))>>5)
#define VO_OSD2_get_hs_yodd(data)                                                    ((0x00000020&(data))>>5)
#define VO_OSD2_hs_codd_shift                                                        (4)
#define VO_OSD2_hs_codd_mask                                                         (0x00000010)
#define VO_OSD2_hs_codd(data)                                                        (0x00000010&((data)<<4))
#define VO_OSD2_hs_codd_src(data)                                                    ((0x00000010&(data))>>4)
#define VO_OSD2_get_hs_codd(data)                                                    ((0x00000010&(data))>>4)
#define VO_OSD2_chroma1_shift                                                        (3)
#define VO_OSD2_chroma1_mask                                                         (0x00000008)
#define VO_OSD2_chroma1(data)                                                        (0x00000008&((data)<<3))
#define VO_OSD2_chroma1_src(data)                                                    ((0x00000008&(data))>>3)
#define VO_OSD2_get_chroma1(data)                                                    ((0x00000008&(data))>>3)
#define VO_OSD2_chroma0_shift                                                        (2)
#define VO_OSD2_chroma0_mask                                                         (0x00000004)
#define VO_OSD2_chroma0(data)                                                        (0x00000004&((data)<<2))
#define VO_OSD2_chroma0_src(data)                                                    ((0x00000004&(data))>>2)
#define VO_OSD2_get_chroma0(data)                                                    ((0x00000004&(data))>>2)
#define VO_OSD2_write_data_shift                                                     (0)
#define VO_OSD2_write_data_mask                                                      (0x00000001)
#define VO_OSD2_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_OSD2_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_OSD2_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_OSD2_START                                                                0x18005518
#define VO_OSD2_START_reg_addr                                                       "0xB8005518"
#define VO_OSD2_START_reg                                                            0xB8005518
#define set_VO_OSD2_START_reg(data)   (*((volatile unsigned int*) VO_OSD2_START_reg)=data)
#define get_VO_OSD2_START_reg   (*((volatile unsigned int*) VO_OSD2_START_reg))
#define VO_OSD2_START_inst_adr                                                       "0x0046"
#define VO_OSD2_START_inst                                                           0x0046
#define VO_OSD2_START_ref_shift                                                      (24)
#define VO_OSD2_START_ref_mask                                                       (0x03000000)
#define VO_OSD2_START_ref(data)                                                      (0x03000000&((data)<<24))
#define VO_OSD2_START_ref_src(data)                                                  ((0x03000000&(data))>>24)
#define VO_OSD2_START_get_ref(data)                                                  ((0x03000000&(data))>>24)
#define VO_OSD2_START_pos_shift                                                      (12)
#define VO_OSD2_START_pos_mask                                                       (0x007FF000)
#define VO_OSD2_START_pos(data)                                                      (0x007FF000&((data)<<12))
#define VO_OSD2_START_pos_src(data)                                                  ((0x007FF000&(data))>>12)
#define VO_OSD2_START_get_pos(data)                                                  ((0x007FF000&(data))>>12)
#define VO_OSD2_START_th_shift                                                       (0)
#define VO_OSD2_START_th_mask                                                        (0x000007FF)
#define VO_OSD2_START_th(data)                                                       (0x000007FF&((data)<<0))
#define VO_OSD2_START_th_src(data)                                                   ((0x000007FF&(data))>>0)
#define VO_OSD2_START_get_th(data)                                                   ((0x000007FF&(data))>>0)


#define VO_OSD2_WI                                                                   0x1800551C
#define VO_OSD2_WI_reg_addr                                                          "0xB800551C"
#define VO_OSD2_WI_reg                                                               0xB800551C
#define set_VO_OSD2_WI_reg(data)   (*((volatile unsigned int*) VO_OSD2_WI_reg)=data)
#define get_VO_OSD2_WI_reg   (*((volatile unsigned int*) VO_OSD2_WI_reg))
#define VO_OSD2_WI_inst_adr                                                          "0x0047"
#define VO_OSD2_WI_inst                                                              0x0047
#define VO_OSD2_WI_endian_shift                                                      (31)
#define VO_OSD2_WI_endian_mask                                                       (0x80000000)
#define VO_OSD2_WI_endian(data)                                                      (0x80000000&((data)<<31))
#define VO_OSD2_WI_endian_src(data)                                                  ((0x80000000&(data))>>31)
#define VO_OSD2_WI_get_endian(data)                                                  ((0x80000000&(data))>>31)
#define VO_OSD2_WI_a_shift                                                           (0)
#define VO_OSD2_WI_a_mask                                                            (0x7FFFFFFF)
#define VO_OSD2_WI_a(data)                                                           (0x7FFFFFFF&((data)<<0))
#define VO_OSD2_WI_a_src(data)                                                       ((0x7FFFFFFF&(data))>>0)
#define VO_OSD2_WI_get_a(data)                                                       ((0x7FFFFFFF&(data))>>0)


#define VO_OSD2_WI_3D                                                                0x18005520
#define VO_OSD2_WI_3D_reg_addr                                                       "0xB8005520"
#define VO_OSD2_WI_3D_reg                                                            0xB8005520
#define set_VO_OSD2_WI_3D_reg(data)   (*((volatile unsigned int*) VO_OSD2_WI_3D_reg)=data)
#define get_VO_OSD2_WI_3D_reg   (*((volatile unsigned int*) VO_OSD2_WI_3D_reg))
#define VO_OSD2_WI_3D_inst_adr                                                       "0x0048"
#define VO_OSD2_WI_3D_inst                                                           0x0048
#define VO_OSD2_WI_3D_a_shift                                                        (0)
#define VO_OSD2_WI_3D_a_mask                                                         (0x7FFFFFFF)
#define VO_OSD2_WI_3D_a(data)                                                        (0x7FFFFFFF&((data)<<0))
#define VO_OSD2_WI_3D_a_src(data)                                                    ((0x7FFFFFFF&(data))>>0)
#define VO_OSD2_WI_3D_get_a(data)                                                    ((0x7FFFFFFF&(data))>>0)


#define VO_OSD2_SIZE                                                                 0x18005524
#define VO_OSD2_SIZE_reg_addr                                                        "0xB8005524"
#define VO_OSD2_SIZE_reg                                                             0xB8005524
#define set_VO_OSD2_SIZE_reg(data)   (*((volatile unsigned int*) VO_OSD2_SIZE_reg)=data)
#define get_VO_OSD2_SIZE_reg   (*((volatile unsigned int*) VO_OSD2_SIZE_reg))
#define VO_OSD2_SIZE_inst_adr                                                        "0x0049"
#define VO_OSD2_SIZE_inst                                                            0x0049
#define VO_OSD2_SIZE_w_shift                                                         (16)
#define VO_OSD2_SIZE_w_mask                                                          (0x07FF0000)
#define VO_OSD2_SIZE_w(data)                                                         (0x07FF0000&((data)<<16))
#define VO_OSD2_SIZE_w_src(data)                                                     ((0x07FF0000&(data))>>16)
#define VO_OSD2_SIZE_get_w(data)                                                     ((0x07FF0000&(data))>>16)
#define VO_OSD2_SIZE_h_shift                                                         (0)
#define VO_OSD2_SIZE_h_mask                                                          (0x000007FF)
#define VO_OSD2_SIZE_h(data)                                                         (0x000007FF&((data)<<0))
#define VO_OSD2_SIZE_h_src(data)                                                     ((0x000007FF&(data))>>0)
#define VO_OSD2_SIZE_get_h(data)                                                     ((0x000007FF&(data))>>0)


#define VO_OSD2_BUF                                                                  0x18005528
#define VO_OSD2_BUF_reg_addr                                                         "0xB8005528"
#define VO_OSD2_BUF_reg                                                              0xB8005528
#define set_VO_OSD2_BUF_reg(data)   (*((volatile unsigned int*) VO_OSD2_BUF_reg)=data)
#define get_VO_OSD2_BUF_reg   (*((volatile unsigned int*) VO_OSD2_BUF_reg))
#define VO_OSD2_BUF_inst_adr                                                         "0x004A"
#define VO_OSD2_BUF_inst                                                             0x004A
#define VO_OSD2_BUF_th_shift                                                         (0)
#define VO_OSD2_BUF_th_mask                                                          (0x000001FF)
#define VO_OSD2_BUF_th(data)                                                         (0x000001FF&((data)<<0))
#define VO_OSD2_BUF_th_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_OSD2_BUF_get_th(data)                                                     ((0x000001FF&(data))>>0)


#define VO_OSD2_CC1                                                                  0x1800552C
#define VO_OSD2_CC1_reg_addr                                                         "0xB800552C"
#define VO_OSD2_CC1_reg                                                              0xB800552C
#define set_VO_OSD2_CC1_reg(data)   (*((volatile unsigned int*) VO_OSD2_CC1_reg)=data)
#define get_VO_OSD2_CC1_reg   (*((volatile unsigned int*) VO_OSD2_CC1_reg))
#define VO_OSD2_CC1_inst_adr                                                         "0x004B"
#define VO_OSD2_CC1_inst                                                             0x004B
#define VO_OSD2_CC1_m01_shift                                                        (12)
#define VO_OSD2_CC1_m01_mask                                                         (0x00FFF000)
#define VO_OSD2_CC1_m01(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD2_CC1_m01_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC1_get_m01(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC1_m00_shift                                                        (0)
#define VO_OSD2_CC1_m00_mask                                                         (0x00000FFF)
#define VO_OSD2_CC1_m00(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD2_CC1_m00_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD2_CC1_get_m00(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD2_CC2                                                                  0x18005530
#define VO_OSD2_CC2_reg_addr                                                         "0xB8005530"
#define VO_OSD2_CC2_reg                                                              0xB8005530
#define set_VO_OSD2_CC2_reg(data)   (*((volatile unsigned int*) VO_OSD2_CC2_reg)=data)
#define get_VO_OSD2_CC2_reg   (*((volatile unsigned int*) VO_OSD2_CC2_reg))
#define VO_OSD2_CC2_inst_adr                                                         "0x004C"
#define VO_OSD2_CC2_inst                                                             0x004C
#define VO_OSD2_CC2_m10_shift                                                        (12)
#define VO_OSD2_CC2_m10_mask                                                         (0x00FFF000)
#define VO_OSD2_CC2_m10(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD2_CC2_m10_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC2_get_m10(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC2_m02_shift                                                        (0)
#define VO_OSD2_CC2_m02_mask                                                         (0x00000FFF)
#define VO_OSD2_CC2_m02(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD2_CC2_m02_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD2_CC2_get_m02(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD2_CC3                                                                  0x18005534
#define VO_OSD2_CC3_reg_addr                                                         "0xB8005534"
#define VO_OSD2_CC3_reg                                                              0xB8005534
#define set_VO_OSD2_CC3_reg(data)   (*((volatile unsigned int*) VO_OSD2_CC3_reg)=data)
#define get_VO_OSD2_CC3_reg   (*((volatile unsigned int*) VO_OSD2_CC3_reg))
#define VO_OSD2_CC3_inst_adr                                                         "0x004D"
#define VO_OSD2_CC3_inst                                                             0x004D
#define VO_OSD2_CC3_m12_shift                                                        (12)
#define VO_OSD2_CC3_m12_mask                                                         (0x00FFF000)
#define VO_OSD2_CC3_m12(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD2_CC3_m12_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC3_get_m12(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC3_m11_shift                                                        (0)
#define VO_OSD2_CC3_m11_mask                                                         (0x00000FFF)
#define VO_OSD2_CC3_m11(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD2_CC3_m11_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD2_CC3_get_m11(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD2_CC4                                                                  0x18005538
#define VO_OSD2_CC4_reg_addr                                                         "0xB8005538"
#define VO_OSD2_CC4_reg                                                              0xB8005538
#define set_VO_OSD2_CC4_reg(data)   (*((volatile unsigned int*) VO_OSD2_CC4_reg)=data)
#define get_VO_OSD2_CC4_reg   (*((volatile unsigned int*) VO_OSD2_CC4_reg))
#define VO_OSD2_CC4_inst_adr                                                         "0x004E"
#define VO_OSD2_CC4_inst                                                             0x004E
#define VO_OSD2_CC4_m21_shift                                                        (12)
#define VO_OSD2_CC4_m21_mask                                                         (0x00FFF000)
#define VO_OSD2_CC4_m21(data)                                                        (0x00FFF000&((data)<<12))
#define VO_OSD2_CC4_m21_src(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC4_get_m21(data)                                                    ((0x00FFF000&(data))>>12)
#define VO_OSD2_CC4_m20_shift                                                        (0)
#define VO_OSD2_CC4_m20_mask                                                         (0x00000FFF)
#define VO_OSD2_CC4_m20(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD2_CC4_m20_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD2_CC4_get_m20(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD2_CC5                                                                  0x1800553C
#define VO_OSD2_CC5_reg_addr                                                         "0xB800553C"
#define VO_OSD2_CC5_reg                                                              0xB800553C
#define set_VO_OSD2_CC5_reg(data)   (*((volatile unsigned int*) VO_OSD2_CC5_reg)=data)
#define get_VO_OSD2_CC5_reg   (*((volatile unsigned int*) VO_OSD2_CC5_reg))
#define VO_OSD2_CC5_inst_adr                                                         "0x004F"
#define VO_OSD2_CC5_inst                                                             0x004F
#define VO_OSD2_CC5_m22_shift                                                        (0)
#define VO_OSD2_CC5_m22_mask                                                         (0x00000FFF)
#define VO_OSD2_CC5_m22(data)                                                        (0x00000FFF&((data)<<0))
#define VO_OSD2_CC5_m22_src(data)                                                    ((0x00000FFF&(data))>>0)
#define VO_OSD2_CC5_get_m22(data)                                                    ((0x00000FFF&(data))>>0)


#define VO_OSD2_CC6                                                                  0x18005540
#define VO_OSD2_CC6_reg_addr                                                         "0xB8005540"
#define VO_OSD2_CC6_reg                                                              0xB8005540
#define set_VO_OSD2_CC6_reg(data)   (*((volatile unsigned int*) VO_OSD2_CC6_reg)=data)
#define get_VO_OSD2_CC6_reg   (*((volatile unsigned int*) VO_OSD2_CC6_reg))
#define VO_OSD2_CC6_inst_adr                                                         "0x0050"
#define VO_OSD2_CC6_inst                                                             0x0050
#define VO_OSD2_CC6_a2_shift                                                         (18)
#define VO_OSD2_CC6_a2_mask                                                          (0x07FC0000)
#define VO_OSD2_CC6_a2(data)                                                         (0x07FC0000&((data)<<18))
#define VO_OSD2_CC6_a2_src(data)                                                     ((0x07FC0000&(data))>>18)
#define VO_OSD2_CC6_get_a2(data)                                                     ((0x07FC0000&(data))>>18)
#define VO_OSD2_CC6_a1_shift                                                         (9)
#define VO_OSD2_CC6_a1_mask                                                          (0x0003FE00)
#define VO_OSD2_CC6_a1(data)                                                         (0x0003FE00&((data)<<9))
#define VO_OSD2_CC6_a1_src(data)                                                     ((0x0003FE00&(data))>>9)
#define VO_OSD2_CC6_get_a1(data)                                                     ((0x0003FE00&(data))>>9)
#define VO_OSD2_CC6_a0_shift                                                         (0)
#define VO_OSD2_CC6_a0_mask                                                          (0x000001FF)
#define VO_OSD2_CC6_a0(data)                                                         (0x000001FF&((data)<<0))
#define VO_OSD2_CC6_a0_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_OSD2_CC6_get_a0(data)                                                     ((0x000001FF&(data))>>0)


#define VO_OSD2_VSI                                                                  0x18005544
#define VO_OSD2_VSI_reg_addr                                                         "0xB8005544"
#define VO_OSD2_VSI_reg                                                              0xB8005544
#define set_VO_OSD2_VSI_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSI_reg)=data)
#define get_VO_OSD2_VSI_reg   (*((volatile unsigned int*) VO_OSD2_VSI_reg))
#define VO_OSD2_VSI_inst_adr                                                         "0x0051"
#define VO_OSD2_VSI_inst                                                             0x0051
#define VO_OSD2_VSI_offset_shift                                                     (16)
#define VO_OSD2_VSI_offset_mask                                                      (0x07FF0000)
#define VO_OSD2_VSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_OSD2_VSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD2_VSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD2_VSI_phase_shift                                                      (2)
#define VO_OSD2_VSI_phase_mask                                                       (0x0000FFFC)
#define VO_OSD2_VSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_OSD2_VSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_OSD2_VSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_OSD2_VSD                                                                  0x18005548
#define VO_OSD2_VSD_reg_addr                                                         "0xB8005548"
#define VO_OSD2_VSD_reg                                                              0xB8005548
#define set_VO_OSD2_VSD_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSD_reg)=data)
#define get_VO_OSD2_VSD_reg   (*((volatile unsigned int*) VO_OSD2_VSD_reg))
#define VO_OSD2_VSD_inst_adr                                                         "0x0052"
#define VO_OSD2_VSD_inst                                                             0x0052
#define VO_OSD2_VSD_out_shift                                                        (20)
#define VO_OSD2_VSD_out_mask                                                         (0x7FF00000)
#define VO_OSD2_VSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_OSD2_VSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD2_VSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD2_VSD_delta_shift                                                      (0)
#define VO_OSD2_VSD_delta_mask                                                       (0x0007FFFF)
#define VO_OSD2_VSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_OSD2_VSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_OSD2_VSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_OSD2_VSYC_0                                                               0x1800554C
#define VO_OSD2_VSYC_1                                                               0x18005550
#define VO_OSD2_VSYC_2                                                               0x18005554
#define VO_OSD2_VSYC_3                                                               0x18005558
#define VO_OSD2_VSYC_0_reg_addr                                                      "0xB800554C"
#define VO_OSD2_VSYC_1_reg_addr                                                      "0xB8005550"
#define VO_OSD2_VSYC_2_reg_addr                                                      "0xB8005554"
#define VO_OSD2_VSYC_3_reg_addr                                                      "0xB8005558"
#define VO_OSD2_VSYC_0_reg                                                           0xB800554C
#define VO_OSD2_VSYC_1_reg                                                           0xB8005550
#define VO_OSD2_VSYC_2_reg                                                           0xB8005554
#define VO_OSD2_VSYC_3_reg                                                           0xB8005558
#define set_VO_OSD2_VSYC_0_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSYC_0_reg)=data)
#define set_VO_OSD2_VSYC_1_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSYC_1_reg)=data)
#define set_VO_OSD2_VSYC_2_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSYC_2_reg)=data)
#define set_VO_OSD2_VSYC_3_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSYC_3_reg)=data)
#define get_VO_OSD2_VSYC_0_reg   (*((volatile unsigned int*) VO_OSD2_VSYC_0_reg))
#define get_VO_OSD2_VSYC_1_reg   (*((volatile unsigned int*) VO_OSD2_VSYC_1_reg))
#define get_VO_OSD2_VSYC_2_reg   (*((volatile unsigned int*) VO_OSD2_VSYC_2_reg))
#define get_VO_OSD2_VSYC_3_reg   (*((volatile unsigned int*) VO_OSD2_VSYC_3_reg))
#define VO_OSD2_VSYC_0_inst_adr                                                      "0x0053"
#define VO_OSD2_VSYC_1_inst_adr                                                      "0x0054"
#define VO_OSD2_VSYC_2_inst_adr                                                      "0x0055"
#define VO_OSD2_VSYC_3_inst_adr                                                      "0x0056"
#define VO_OSD2_VSYC_0_inst                                                          0x0053
#define VO_OSD2_VSYC_1_inst                                                          0x0054
#define VO_OSD2_VSYC_2_inst                                                          0x0055
#define VO_OSD2_VSYC_3_inst                                                          0x0056
#define VO_OSD2_VSYC_c1_shift                                                        (16)
#define VO_OSD2_VSYC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD2_VSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD2_VSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_VSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_VSYC_c0_shift                                                        (0)
#define VO_OSD2_VSYC_c0_mask                                                         (0x00003FFF)
#define VO_OSD2_VSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD2_VSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD2_VSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD2_VSCC_0                                                               0x1800555C
#define VO_OSD2_VSCC_1                                                               0x18005560
#define VO_OSD2_VSCC_2                                                               0x18005564
#define VO_OSD2_VSCC_3                                                               0x18005568
#define VO_OSD2_VSCC_0_reg_addr                                                      "0xB800555C"
#define VO_OSD2_VSCC_1_reg_addr                                                      "0xB8005560"
#define VO_OSD2_VSCC_2_reg_addr                                                      "0xB8005564"
#define VO_OSD2_VSCC_3_reg_addr                                                      "0xB8005568"
#define VO_OSD2_VSCC_0_reg                                                           0xB800555C
#define VO_OSD2_VSCC_1_reg                                                           0xB8005560
#define VO_OSD2_VSCC_2_reg                                                           0xB8005564
#define VO_OSD2_VSCC_3_reg                                                           0xB8005568
#define set_VO_OSD2_VSCC_0_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSCC_0_reg)=data)
#define set_VO_OSD2_VSCC_1_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSCC_1_reg)=data)
#define set_VO_OSD2_VSCC_2_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSCC_2_reg)=data)
#define set_VO_OSD2_VSCC_3_reg(data)   (*((volatile unsigned int*) VO_OSD2_VSCC_3_reg)=data)
#define get_VO_OSD2_VSCC_0_reg   (*((volatile unsigned int*) VO_OSD2_VSCC_0_reg))
#define get_VO_OSD2_VSCC_1_reg   (*((volatile unsigned int*) VO_OSD2_VSCC_1_reg))
#define get_VO_OSD2_VSCC_2_reg   (*((volatile unsigned int*) VO_OSD2_VSCC_2_reg))
#define get_VO_OSD2_VSCC_3_reg   (*((volatile unsigned int*) VO_OSD2_VSCC_3_reg))
#define VO_OSD2_VSCC_0_inst_adr                                                      "0x0057"
#define VO_OSD2_VSCC_1_inst_adr                                                      "0x0058"
#define VO_OSD2_VSCC_2_inst_adr                                                      "0x0059"
#define VO_OSD2_VSCC_3_inst_adr                                                      "0x005A"
#define VO_OSD2_VSCC_0_inst                                                          0x0057
#define VO_OSD2_VSCC_1_inst                                                          0x0058
#define VO_OSD2_VSCC_2_inst                                                          0x0059
#define VO_OSD2_VSCC_3_inst                                                          0x005A
#define VO_OSD2_VSCC_c1_shift                                                        (16)
#define VO_OSD2_VSCC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD2_VSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD2_VSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_VSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_VSCC_c0_shift                                                        (0)
#define VO_OSD2_VSCC_c0_mask                                                         (0x00003FFF)
#define VO_OSD2_VSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD2_VSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD2_VSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD2_HSI                                                                  0x1800556C
#define VO_OSD2_HSI_reg_addr                                                         "0xB800556C"
#define VO_OSD2_HSI_reg                                                              0xB800556C
#define set_VO_OSD2_HSI_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSI_reg)=data)
#define get_VO_OSD2_HSI_reg   (*((volatile unsigned int*) VO_OSD2_HSI_reg))
#define VO_OSD2_HSI_inst_adr                                                         "0x005B"
#define VO_OSD2_HSI_inst                                                             0x005B
#define VO_OSD2_HSI_offset_shift                                                     (16)
#define VO_OSD2_HSI_offset_mask                                                      (0x07FF0000)
#define VO_OSD2_HSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_OSD2_HSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD2_HSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_OSD2_HSI_phase_shift                                                      (2)
#define VO_OSD2_HSI_phase_mask                                                       (0x0000FFFC)
#define VO_OSD2_HSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_OSD2_HSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_OSD2_HSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_OSD2_HSD                                                                  0x18005570
#define VO_OSD2_HSD_reg_addr                                                         "0xB8005570"
#define VO_OSD2_HSD_reg                                                              0xB8005570
#define set_VO_OSD2_HSD_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSD_reg)=data)
#define get_VO_OSD2_HSD_reg   (*((volatile unsigned int*) VO_OSD2_HSD_reg))
#define VO_OSD2_HSD_inst_adr                                                         "0x005C"
#define VO_OSD2_HSD_inst                                                             0x005C
#define VO_OSD2_HSD_out_shift                                                        (20)
#define VO_OSD2_HSD_out_mask                                                         (0x7FF00000)
#define VO_OSD2_HSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_OSD2_HSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD2_HSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_OSD2_HSD_delta_shift                                                      (0)
#define VO_OSD2_HSD_delta_mask                                                       (0x0007FFFF)
#define VO_OSD2_HSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_OSD2_HSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_OSD2_HSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_OSD2_HSYC_0                                                               0x18005574
#define VO_OSD2_HSYC_1                                                               0x18005578
#define VO_OSD2_HSYC_2                                                               0x1800557C
#define VO_OSD2_HSYC_3                                                               0x18005580
#define VO_OSD2_HSYC_4                                                               0x18005584
#define VO_OSD2_HSYC_5                                                               0x18005588
#define VO_OSD2_HSYC_6                                                               0x1800558C
#define VO_OSD2_HSYC_7                                                               0x18005590
#define VO_OSD2_HSYC_0_reg_addr                                                      "0xB8005574"
#define VO_OSD2_HSYC_1_reg_addr                                                      "0xB8005578"
#define VO_OSD2_HSYC_2_reg_addr                                                      "0xB800557C"
#define VO_OSD2_HSYC_3_reg_addr                                                      "0xB8005580"
#define VO_OSD2_HSYC_4_reg_addr                                                      "0xB8005584"
#define VO_OSD2_HSYC_5_reg_addr                                                      "0xB8005588"
#define VO_OSD2_HSYC_6_reg_addr                                                      "0xB800558C"
#define VO_OSD2_HSYC_7_reg_addr                                                      "0xB8005590"
#define VO_OSD2_HSYC_0_reg                                                           0xB8005574
#define VO_OSD2_HSYC_1_reg                                                           0xB8005578
#define VO_OSD2_HSYC_2_reg                                                           0xB800557C
#define VO_OSD2_HSYC_3_reg                                                           0xB8005580
#define VO_OSD2_HSYC_4_reg                                                           0xB8005584
#define VO_OSD2_HSYC_5_reg                                                           0xB8005588
#define VO_OSD2_HSYC_6_reg                                                           0xB800558C
#define VO_OSD2_HSYC_7_reg                                                           0xB8005590
#define set_VO_OSD2_HSYC_0_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_0_reg)=data)
#define set_VO_OSD2_HSYC_1_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_1_reg)=data)
#define set_VO_OSD2_HSYC_2_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_2_reg)=data)
#define set_VO_OSD2_HSYC_3_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_3_reg)=data)
#define set_VO_OSD2_HSYC_4_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_4_reg)=data)
#define set_VO_OSD2_HSYC_5_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_5_reg)=data)
#define set_VO_OSD2_HSYC_6_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_6_reg)=data)
#define set_VO_OSD2_HSYC_7_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSYC_7_reg)=data)
#define get_VO_OSD2_HSYC_0_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_0_reg))
#define get_VO_OSD2_HSYC_1_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_1_reg))
#define get_VO_OSD2_HSYC_2_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_2_reg))
#define get_VO_OSD2_HSYC_3_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_3_reg))
#define get_VO_OSD2_HSYC_4_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_4_reg))
#define get_VO_OSD2_HSYC_5_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_5_reg))
#define get_VO_OSD2_HSYC_6_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_6_reg))
#define get_VO_OSD2_HSYC_7_reg   (*((volatile unsigned int*) VO_OSD2_HSYC_7_reg))
#define VO_OSD2_HSYC_0_inst_adr                                                      "0x005D"
#define VO_OSD2_HSYC_1_inst_adr                                                      "0x005E"
#define VO_OSD2_HSYC_2_inst_adr                                                      "0x005F"
#define VO_OSD2_HSYC_3_inst_adr                                                      "0x0060"
#define VO_OSD2_HSYC_4_inst_adr                                                      "0x0061"
#define VO_OSD2_HSYC_5_inst_adr                                                      "0x0062"
#define VO_OSD2_HSYC_6_inst_adr                                                      "0x0063"
#define VO_OSD2_HSYC_7_inst_adr                                                      "0x0064"
#define VO_OSD2_HSYC_0_inst                                                          0x005D
#define VO_OSD2_HSYC_1_inst                                                          0x005E
#define VO_OSD2_HSYC_2_inst                                                          0x005F
#define VO_OSD2_HSYC_3_inst                                                          0x0060
#define VO_OSD2_HSYC_4_inst                                                          0x0061
#define VO_OSD2_HSYC_5_inst                                                          0x0062
#define VO_OSD2_HSYC_6_inst                                                          0x0063
#define VO_OSD2_HSYC_7_inst                                                          0x0064
#define VO_OSD2_HSYC_c1_shift                                                        (16)
#define VO_OSD2_HSYC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD2_HSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD2_HSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_HSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_HSYC_c0_shift                                                        (0)
#define VO_OSD2_HSYC_c0_mask                                                         (0x00003FFF)
#define VO_OSD2_HSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD2_HSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD2_HSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_OSD2_HSCC_0                                                               0x18005594
#define VO_OSD2_HSCC_1                                                               0x18005598
#define VO_OSD2_HSCC_2                                                               0x1800559C
#define VO_OSD2_HSCC_3                                                               0x180055A0
#define VO_OSD2_HSCC_4                                                               0x180055A4
#define VO_OSD2_HSCC_5                                                               0x180055A8
#define VO_OSD2_HSCC_6                                                               0x180055AC
#define VO_OSD2_HSCC_7                                                               0x180055B0
#define VO_OSD2_HSCC_0_reg_addr                                                      "0xB8005594"
#define VO_OSD2_HSCC_1_reg_addr                                                      "0xB8005598"
#define VO_OSD2_HSCC_2_reg_addr                                                      "0xB800559C"
#define VO_OSD2_HSCC_3_reg_addr                                                      "0xB80055A0"
#define VO_OSD2_HSCC_4_reg_addr                                                      "0xB80055A4"
#define VO_OSD2_HSCC_5_reg_addr                                                      "0xB80055A8"
#define VO_OSD2_HSCC_6_reg_addr                                                      "0xB80055AC"
#define VO_OSD2_HSCC_7_reg_addr                                                      "0xB80055B0"
#define VO_OSD2_HSCC_0_reg                                                           0xB8005594
#define VO_OSD2_HSCC_1_reg                                                           0xB8005598
#define VO_OSD2_HSCC_2_reg                                                           0xB800559C
#define VO_OSD2_HSCC_3_reg                                                           0xB80055A0
#define VO_OSD2_HSCC_4_reg                                                           0xB80055A4
#define VO_OSD2_HSCC_5_reg                                                           0xB80055A8
#define VO_OSD2_HSCC_6_reg                                                           0xB80055AC
#define VO_OSD2_HSCC_7_reg                                                           0xB80055B0
#define set_VO_OSD2_HSCC_0_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_0_reg)=data)
#define set_VO_OSD2_HSCC_1_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_1_reg)=data)
#define set_VO_OSD2_HSCC_2_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_2_reg)=data)
#define set_VO_OSD2_HSCC_3_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_3_reg)=data)
#define set_VO_OSD2_HSCC_4_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_4_reg)=data)
#define set_VO_OSD2_HSCC_5_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_5_reg)=data)
#define set_VO_OSD2_HSCC_6_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_6_reg)=data)
#define set_VO_OSD2_HSCC_7_reg(data)   (*((volatile unsigned int*) VO_OSD2_HSCC_7_reg)=data)
#define get_VO_OSD2_HSCC_0_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_0_reg))
#define get_VO_OSD2_HSCC_1_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_1_reg))
#define get_VO_OSD2_HSCC_2_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_2_reg))
#define get_VO_OSD2_HSCC_3_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_3_reg))
#define get_VO_OSD2_HSCC_4_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_4_reg))
#define get_VO_OSD2_HSCC_5_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_5_reg))
#define get_VO_OSD2_HSCC_6_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_6_reg))
#define get_VO_OSD2_HSCC_7_reg   (*((volatile unsigned int*) VO_OSD2_HSCC_7_reg))
#define VO_OSD2_HSCC_0_inst_adr                                                      "0x0065"
#define VO_OSD2_HSCC_1_inst_adr                                                      "0x0066"
#define VO_OSD2_HSCC_2_inst_adr                                                      "0x0067"
#define VO_OSD2_HSCC_3_inst_adr                                                      "0x0068"
#define VO_OSD2_HSCC_4_inst_adr                                                      "0x0069"
#define VO_OSD2_HSCC_5_inst_adr                                                      "0x006A"
#define VO_OSD2_HSCC_6_inst_adr                                                      "0x006B"
#define VO_OSD2_HSCC_7_inst_adr                                                      "0x006C"
#define VO_OSD2_HSCC_0_inst                                                          0x0065
#define VO_OSD2_HSCC_1_inst                                                          0x0066
#define VO_OSD2_HSCC_2_inst                                                          0x0067
#define VO_OSD2_HSCC_3_inst                                                          0x0068
#define VO_OSD2_HSCC_4_inst                                                          0x0069
#define VO_OSD2_HSCC_5_inst                                                          0x006A
#define VO_OSD2_HSCC_6_inst                                                          0x006B
#define VO_OSD2_HSCC_7_inst                                                          0x006C
#define VO_OSD2_HSCC_c1_shift                                                        (16)
#define VO_OSD2_HSCC_c1_mask                                                         (0x3FFF0000)
#define VO_OSD2_HSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_OSD2_HSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_HSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_OSD2_HSCC_c0_shift                                                        (0)
#define VO_OSD2_HSCC_c0_mask                                                         (0x00003FFF)
#define VO_OSD2_HSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_OSD2_HSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_OSD2_HSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_CSR                                                                       0x180055B4
#define VO_CSR_reg_addr                                                              "0xB80055B4"
#define VO_CSR_reg                                                                   0xB80055B4
#define set_VO_CSR_reg(data)   (*((volatile unsigned int*) VO_CSR_reg)=data)
#define get_VO_CSR_reg   (*((volatile unsigned int*) VO_CSR_reg))
#define VO_CSR_inst_adr                                                              "0x006D"
#define VO_CSR_inst                                                                  0x006D
#define VO_CSR_d3_dbh_shift                                                          (12)
#define VO_CSR_d3_dbh_mask                                                           (0x00001000)
#define VO_CSR_d3_dbh(data)                                                          (0x00001000&((data)<<12))
#define VO_CSR_d3_dbh_src(data)                                                      ((0x00001000&(data))>>12)
#define VO_CSR_get_d3_dbh(data)                                                      ((0x00001000&(data))>>12)
#define VO_CSR_d3_dbw_shift                                                          (11)
#define VO_CSR_d3_dbw_mask                                                           (0x00000800)
#define VO_CSR_d3_dbw(data)                                                          (0x00000800&((data)<<11))
#define VO_CSR_d3_dbw_src(data)                                                      ((0x00000800&(data))>>11)
#define VO_CSR_get_d3_dbw(data)                                                      ((0x00000800&(data))>>11)
#define VO_CSR_fetch_1080i_shift                                                     (9)
#define VO_CSR_fetch_1080i_mask                                                      (0x00000200)
#define VO_CSR_fetch_1080i(data)                                                     (0x00000200&((data)<<9))
#define VO_CSR_fetch_1080i_src(data)                                                 ((0x00000200&(data))>>9)
#define VO_CSR_get_fetch_1080i(data)                                                 ((0x00000200&(data))>>9)
#define VO_CSR_odd_1080i_shift                                                       (8)
#define VO_CSR_odd_1080i_mask                                                        (0x00000100)
#define VO_CSR_odd_1080i(data)                                                       (0x00000100&((data)<<8))
#define VO_CSR_odd_1080i_src(data)                                                   ((0x00000100&(data))>>8)
#define VO_CSR_get_odd_1080i(data)                                                   ((0x00000100&(data))>>8)
#define VO_CSR_m4bit_shift                                                           (6)
#define VO_CSR_m4bit_mask                                                            (0x00000040)
#define VO_CSR_m4bit(data)                                                           (0x00000040&((data)<<6))
#define VO_CSR_m4bit_src(data)                                                       ((0x00000040&(data))>>6)
#define VO_CSR_get_m4bit(data)                                                       ((0x00000040&(data))>>6)
#define VO_CSR_m2bit_shift                                                           (5)
#define VO_CSR_m2bit_mask                                                            (0x00000020)
#define VO_CSR_m2bit(data)                                                           (0x00000020&((data)<<5))
#define VO_CSR_m2bit_src(data)                                                       ((0x00000020&(data))>>5)
#define VO_CSR_get_m2bit(data)                                                       ((0x00000020&(data))>>5)
#define VO_CSR_reload_shift                                                          (4)
#define VO_CSR_reload_mask                                                           (0x00000010)
#define VO_CSR_reload(data)                                                          (0x00000010&((data)<<4))
#define VO_CSR_reload_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_CSR_get_reload(data)                                                      ((0x00000010&(data))>>4)
#define VO_CSR_update_shift                                                          (3)
#define VO_CSR_update_mask                                                           (0x00000008)
#define VO_CSR_update(data)                                                          (0x00000008&((data)<<3))
#define VO_CSR_update_src(data)                                                      ((0x00000008&(data))>>3)
#define VO_CSR_get_update(data)                                                      ((0x00000008&(data))>>3)
#define VO_CSR_write_data_shift                                                      (0)
#define VO_CSR_write_data_mask                                                       (0x00000001)
#define VO_CSR_write_data(data)                                                      (0x00000001&((data)<<0))
#define VO_CSR_write_data_src(data)                                                  ((0x00000001&(data))>>0)
#define VO_CSR_get_write_data(data)                                                  ((0x00000001&(data))>>0)


#define VO_CSR_SIZE                                                                  0x180055B8
#define VO_CSR_SIZE_reg_addr                                                         "0xB80055B8"
#define VO_CSR_SIZE_reg                                                              0xB80055B8
#define set_VO_CSR_SIZE_reg(data)   (*((volatile unsigned int*) VO_CSR_SIZE_reg)=data)
#define get_VO_CSR_SIZE_reg   (*((volatile unsigned int*) VO_CSR_SIZE_reg))
#define VO_CSR_SIZE_inst_adr                                                         "0x006E"
#define VO_CSR_SIZE_inst                                                             0x006E
#define VO_CSR_SIZE_w_shift                                                          (12)
#define VO_CSR_SIZE_w_mask                                                           (0x001FF000)
#define VO_CSR_SIZE_w(data)                                                          (0x001FF000&((data)<<12))
#define VO_CSR_SIZE_w_src(data)                                                      ((0x001FF000&(data))>>12)
#define VO_CSR_SIZE_get_w(data)                                                      ((0x001FF000&(data))>>12)
#define VO_CSR_SIZE_h_shift                                                          (0)
#define VO_CSR_SIZE_h_mask                                                           (0x000001FF)
#define VO_CSR_SIZE_h(data)                                                          (0x000001FF&((data)<<0))
#define VO_CSR_SIZE_h_src(data)                                                      ((0x000001FF&(data))>>0)
#define VO_CSR_SIZE_get_h(data)                                                      ((0x000001FF&(data))>>0)


#define VO_CSR_BS                                                                    0x180055BC
#define VO_CSR_BS_reg_addr                                                           "0xB80055BC"
#define VO_CSR_BS_reg                                                                0xB80055BC
#define set_VO_CSR_BS_reg(data)   (*((volatile unsigned int*) VO_CSR_BS_reg)=data)
#define get_VO_CSR_BS_reg   (*((volatile unsigned int*) VO_CSR_BS_reg))
#define VO_CSR_BS_inst_adr                                                           "0x006F"
#define VO_CSR_BS_inst                                                               0x006F
#define VO_CSR_BS_a_shift                                                            (0)
#define VO_CSR_BS_a_mask                                                             (0x7FFFFFFF)
#define VO_CSR_BS_a(data)                                                            (0x7FFFFFFF&((data)<<0))
#define VO_CSR_BS_a_src(data)                                                        ((0x7FFFFFFF&(data))>>0)
#define VO_CSR_BS_get_a(data)                                                        ((0x7FFFFFFF&(data))>>0)


#define VO_CSR_BS_3D                                                                 0x180055C0
#define VO_CSR_BS_3D_reg_addr                                                        "0xB80055C0"
#define VO_CSR_BS_3D_reg                                                             0xB80055C0
#define set_VO_CSR_BS_3D_reg(data)   (*((volatile unsigned int*) VO_CSR_BS_3D_reg)=data)
#define get_VO_CSR_BS_3D_reg   (*((volatile unsigned int*) VO_CSR_BS_3D_reg))
#define VO_CSR_BS_3D_inst_adr                                                        "0x0070"
#define VO_CSR_BS_3D_inst                                                            0x0070
#define VO_CSR_BS_3D_a_shift                                                         (0)
#define VO_CSR_BS_3D_a_mask                                                          (0x7FFFFFFF)
#define VO_CSR_BS_3D_a(data)                                                         (0x7FFFFFFF&((data)<<0))
#define VO_CSR_BS_3D_a_src(data)                                                     ((0x7FFFFFFF&(data))>>0)
#define VO_CSR_BS_3D_get_a(data)                                                     ((0x7FFFFFFF&(data))>>0)


#define VO_CSR_CLUT                                                                  0x180055C4
#define VO_CSR_CLUT_reg_addr                                                         "0xB80055C4"
#define VO_CSR_CLUT_reg                                                              0xB80055C4
#define set_VO_CSR_CLUT_reg(data)   (*((volatile unsigned int*) VO_CSR_CLUT_reg)=data)
#define get_VO_CSR_CLUT_reg   (*((volatile unsigned int*) VO_CSR_CLUT_reg))
#define VO_CSR_CLUT_inst_adr                                                         "0x0071"
#define VO_CSR_CLUT_inst                                                             0x0071
#define VO_CSR_CLUT_a_shift                                                          (0)
#define VO_CSR_CLUT_a_mask                                                           (0x7FFFFFFF)
#define VO_CSR_CLUT_a(data)                                                          (0x7FFFFFFF&((data)<<0))
#define VO_CSR_CLUT_a_src(data)                                                      ((0x7FFFFFFF&(data))>>0)
#define VO_CSR_CLUT_get_a(data)                                                      ((0x7FFFFFFF&(data))>>0)


#define VO_PIC                                                                       0x180055C8
#define VO_PIC_reg_addr                                                              "0xB80055C8"
#define VO_PIC_reg                                                                   0xB80055C8
#define set_VO_PIC_reg(data)   (*((volatile unsigned int*) VO_PIC_reg)=data)
#define get_VO_PIC_reg   (*((volatile unsigned int*) VO_PIC_reg))
#define VO_PIC_inst_adr                                                              "0x0072"
#define VO_PIC_inst                                                                  0x0072
#define VO_PIC_d3_dbh_shift                                                          (7)
#define VO_PIC_d3_dbh_mask                                                           (0x00000080)
#define VO_PIC_d3_dbh(data)                                                          (0x00000080&((data)<<7))
#define VO_PIC_d3_dbh_src(data)                                                      ((0x00000080&(data))>>7)
#define VO_PIC_get_d3_dbh(data)                                                      ((0x00000080&(data))>>7)
#define VO_PIC_d3_dbw_shift                                                          (6)
#define VO_PIC_d3_dbw_mask                                                           (0x00000040)
#define VO_PIC_d3_dbw(data)                                                          (0x00000040&((data)<<6))
#define VO_PIC_d3_dbw_src(data)                                                      ((0x00000040&(data))>>6)
#define VO_PIC_get_d3_dbw(data)                                                      ((0x00000040&(data))>>6)
#define VO_PIC_d3_dup_shift                                                          (5)
#define VO_PIC_d3_dup_mask                                                           (0x00000020)
#define VO_PIC_d3_dup(data)                                                          (0x00000020&((data)<<5))
#define VO_PIC_d3_dup_src(data)                                                      ((0x00000020&(data))>>5)
#define VO_PIC_get_d3_dup(data)                                                      ((0x00000020&(data))>>5)
#define VO_PIC_gfx_shift                                                             (4)
#define VO_PIC_gfx_mask                                                              (0x00000010)
#define VO_PIC_gfx(data)                                                             (0x00000010&((data)<<4))
#define VO_PIC_gfx_src(data)                                                         ((0x00000010&(data))>>4)
#define VO_PIC_get_gfx(data)                                                         ((0x00000010&(data))>>4)
#define VO_PIC_f420_shift                                                            (3)
#define VO_PIC_f420_mask                                                             (0x00000008)
#define VO_PIC_f420(data)                                                            (0x00000008&((data)<<3))
#define VO_PIC_f420_src(data)                                                        ((0x00000008&(data))>>3)
#define VO_PIC_get_f420(data)                                                        ((0x00000008&(data))>>3)
#define VO_PIC_skip_v1_shift                                                         (1)
#define VO_PIC_skip_v1_mask                                                          (0x00000002)
#define VO_PIC_skip_v1(data)                                                         (0x00000002&((data)<<1))
#define VO_PIC_skip_v1_src(data)                                                     ((0x00000002&(data))>>1)
#define VO_PIC_get_skip_v1(data)                                                     ((0x00000002&(data))>>1)
#define VO_PIC_write_data_shift                                                      (0)
#define VO_PIC_write_data_mask                                                       (0x00000001)
#define VO_PIC_write_data(data)                                                      (0x00000001&((data)<<0))
#define VO_PIC_write_data_src(data)                                                  ((0x00000001&(data))>>0)
#define VO_PIC_get_write_data(data)                                                  ((0x00000001&(data))>>0)


#define VO_PIC_LKEY                                                                  0x180055CC
#define VO_PIC_LKEY_reg_addr                                                         "0xB80055CC"
#define VO_PIC_LKEY_reg                                                              0xB80055CC
#define set_VO_PIC_LKEY_reg(data)   (*((volatile unsigned int*) VO_PIC_LKEY_reg)=data)
#define get_VO_PIC_LKEY_reg   (*((volatile unsigned int*) VO_PIC_LKEY_reg))
#define VO_PIC_LKEY_inst_adr                                                         "0x0073"
#define VO_PIC_LKEY_inst                                                             0x0073
#define VO_PIC_LKEY_en_shift                                                         (16)
#define VO_PIC_LKEY_en_mask                                                          (0x00010000)
#define VO_PIC_LKEY_en(data)                                                         (0x00010000&((data)<<16))
#define VO_PIC_LKEY_en_src(data)                                                     ((0x00010000&(data))>>16)
#define VO_PIC_LKEY_get_en(data)                                                     ((0x00010000&(data))>>16)
#define VO_PIC_LKEY_e_shift                                                          (8)
#define VO_PIC_LKEY_e_mask                                                           (0x0000FF00)
#define VO_PIC_LKEY_e(data)                                                          (0x0000FF00&((data)<<8))
#define VO_PIC_LKEY_e_src(data)                                                      ((0x0000FF00&(data))>>8)
#define VO_PIC_LKEY_get_e(data)                                                      ((0x0000FF00&(data))>>8)
#define VO_PIC_LKEY_s_shift                                                          (0)
#define VO_PIC_LKEY_s_mask                                                           (0x000000FF)
#define VO_PIC_LKEY_s(data)                                                          (0x000000FF&((data)<<0))
#define VO_PIC_LKEY_s_src(data)                                                      ((0x000000FF&(data))>>0)
#define VO_PIC_LKEY_get_s(data)                                                      ((0x000000FF&(data))>>0)


#define VO_PIC_INDEX                                                                 0x180055D0
#define VO_PIC_INDEX_reg_addr                                                        "0xB80055D0"
#define VO_PIC_INDEX_reg                                                             0xB80055D0
#define set_VO_PIC_INDEX_reg(data)   (*((volatile unsigned int*) VO_PIC_INDEX_reg)=data)
#define get_VO_PIC_INDEX_reg   (*((volatile unsigned int*) VO_PIC_INDEX_reg))
#define VO_PIC_INDEX_inst_adr                                                        "0x0074"
#define VO_PIC_INDEX_inst                                                            0x0074
#define VO_PIC_INDEX_c_shift                                                         (8)
#define VO_PIC_INDEX_c_mask                                                          (0x00007F00)
#define VO_PIC_INDEX_c(data)                                                         (0x00007F00&((data)<<8))
#define VO_PIC_INDEX_c_src(data)                                                     ((0x00007F00&(data))>>8)
#define VO_PIC_INDEX_get_c(data)                                                     ((0x00007F00&(data))>>8)
#define VO_PIC_INDEX_y_shift                                                         (0)
#define VO_PIC_INDEX_y_mask                                                          (0x0000007F)
#define VO_PIC_INDEX_y(data)                                                         (0x0000007F&((data)<<0))
#define VO_PIC_INDEX_y_src(data)                                                     ((0x0000007F&(data))>>0)
#define VO_PIC_INDEX_get_y(data)                                                     ((0x0000007F&(data))>>0)


#define VO_PIC_INDEX_3D                                                              0x180055D4
#define VO_PIC_INDEX_3D_reg_addr                                                     "0xB80055D4"
#define VO_PIC_INDEX_3D_reg                                                          0xB80055D4
#define set_VO_PIC_INDEX_3D_reg(data)   (*((volatile unsigned int*) VO_PIC_INDEX_3D_reg)=data)
#define get_VO_PIC_INDEX_3D_reg   (*((volatile unsigned int*) VO_PIC_INDEX_3D_reg))
#define VO_PIC_INDEX_3D_inst_adr                                                     "0x0075"
#define VO_PIC_INDEX_3D_inst                                                         0x0075
#define VO_PIC_INDEX_3D_c_shift                                                      (8)
#define VO_PIC_INDEX_3D_c_mask                                                       (0x00007F00)
#define VO_PIC_INDEX_3D_c(data)                                                      (0x00007F00&((data)<<8))
#define VO_PIC_INDEX_3D_c_src(data)                                                  ((0x00007F00&(data))>>8)
#define VO_PIC_INDEX_3D_get_c(data)                                                  ((0x00007F00&(data))>>8)
#define VO_PIC_INDEX_3D_y_shift                                                      (0)
#define VO_PIC_INDEX_3D_y_mask                                                       (0x0000007F)
#define VO_PIC_INDEX_3D_y(data)                                                      (0x0000007F&((data)<<0))
#define VO_PIC_INDEX_3D_y_src(data)                                                  ((0x0000007F&(data))>>0)
#define VO_PIC_INDEX_3D_get_y(data)                                                  ((0x0000007F&(data))>>0)


#define VO_PIC_SEQ                                                                   0x180055D8
#define VO_PIC_SEQ_reg_addr                                                          "0xB80055D8"
#define VO_PIC_SEQ_reg                                                               0xB80055D8
#define set_VO_PIC_SEQ_reg(data)   (*((volatile unsigned int*) VO_PIC_SEQ_reg)=data)
#define get_VO_PIC_SEQ_reg   (*((volatile unsigned int*) VO_PIC_SEQ_reg))
#define VO_PIC_SEQ_inst_adr                                                          "0x0076"
#define VO_PIC_SEQ_inst                                                              0x0076
#define VO_PIC_SEQ_sa_shift                                                          (0)
#define VO_PIC_SEQ_sa_mask                                                           (0x7FFFFFFF)
#define VO_PIC_SEQ_sa(data)                                                          (0x7FFFFFFF&((data)<<0))
#define VO_PIC_SEQ_sa_src(data)                                                      ((0x7FFFFFFF&(data))>>0)
#define VO_PIC_SEQ_get_sa(data)                                                      ((0x7FFFFFFF&(data))>>0)


#define VO_PIC_SEQ_3D                                                                0x180055DC
#define VO_PIC_SEQ_3D_reg_addr                                                       "0xB80055DC"
#define VO_PIC_SEQ_3D_reg                                                            0xB80055DC
#define set_VO_PIC_SEQ_3D_reg(data)   (*((volatile unsigned int*) VO_PIC_SEQ_3D_reg)=data)
#define get_VO_PIC_SEQ_3D_reg   (*((volatile unsigned int*) VO_PIC_SEQ_3D_reg))
#define VO_PIC_SEQ_3D_inst_adr                                                       "0x0077"
#define VO_PIC_SEQ_3D_inst                                                           0x0077
#define VO_PIC_SEQ_3D_sa_shift                                                       (0)
#define VO_PIC_SEQ_3D_sa_mask                                                        (0x7FFFFFFF)
#define VO_PIC_SEQ_3D_sa(data)                                                       (0x7FFFFFFF&((data)<<0))
#define VO_PIC_SEQ_3D_sa_src(data)                                                   ((0x7FFFFFFF&(data))>>0)
#define VO_PIC_SEQ_3D_get_sa(data)                                                   ((0x7FFFFFFF&(data))>>0)


#define VO_PIC_GFX                                                                   0x180055E0
#define VO_PIC_GFX_reg_addr                                                          "0xB80055E0"
#define VO_PIC_GFX_reg                                                               0xB80055E0
#define set_VO_PIC_GFX_reg(data)   (*((volatile unsigned int*) VO_PIC_GFX_reg)=data)
#define get_VO_PIC_GFX_reg   (*((volatile unsigned int*) VO_PIC_GFX_reg))
#define VO_PIC_GFX_inst_adr                                                          "0x0078"
#define VO_PIC_GFX_inst                                                              0x0078
#define VO_PIC_GFX_endian_shift                                                      (31)
#define VO_PIC_GFX_endian_mask                                                       (0x80000000)
#define VO_PIC_GFX_endian(data)                                                      (0x80000000&((data)<<31))
#define VO_PIC_GFX_endian_src(data)                                                  ((0x80000000&(data))>>31)
#define VO_PIC_GFX_get_endian(data)                                                  ((0x80000000&(data))>>31)
#define VO_PIC_GFX_format_shift                                                      (28)
#define VO_PIC_GFX_format_mask                                                       (0x70000000)
#define VO_PIC_GFX_format(data)                                                      (0x70000000&((data)<<28))
#define VO_PIC_GFX_format_src(data)                                                  ((0x70000000&(data))>>28)
#define VO_PIC_GFX_get_format(data)                                                  ((0x70000000&(data))>>28)
#define VO_PIC_GFX_gfx_alpha_shift                                                   (25)
#define VO_PIC_GFX_gfx_alpha_mask                                                    (0x02000000)
#define VO_PIC_GFX_gfx_alpha(data)                                                   (0x02000000&((data)<<25))
#define VO_PIC_GFX_gfx_alpha_src(data)                                               ((0x02000000&(data))>>25)
#define VO_PIC_GFX_get_gfx_alpha(data)                                               ((0x02000000&(data))>>25)
#define VO_PIC_GFX_extend_shift                                                      (24)
#define VO_PIC_GFX_extend_mask                                                       (0x01000000)
#define VO_PIC_GFX_extend(data)                                                      (0x01000000&((data)<<24))
#define VO_PIC_GFX_extend_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_PIC_GFX_get_extend(data)                                                  ((0x01000000&(data))>>24)
#define VO_PIC_GFX_pitch_shift                                                       (0)
#define VO_PIC_GFX_pitch_mask                                                        (0x0000FFFF)
#define VO_PIC_GFX_pitch(data)                                                       (0x0000FFFF&((data)<<0))
#define VO_PIC_GFX_pitch_src(data)                                                   ((0x0000FFFF&(data))>>0)
#define VO_PIC_GFX_get_pitch(data)                                                   ((0x0000FFFF&(data))>>0)


#define VO_PIC_GC1                                                                   0x180055E4
#define VO_PIC_GC1_reg_addr                                                          "0xB80055E4"
#define VO_PIC_GC1_reg                                                               0xB80055E4
#define set_VO_PIC_GC1_reg(data)   (*((volatile unsigned int*) VO_PIC_GC1_reg)=data)
#define get_VO_PIC_GC1_reg   (*((volatile unsigned int*) VO_PIC_GC1_reg))
#define VO_PIC_GC1_inst_adr                                                          "0x0079"
#define VO_PIC_GC1_inst                                                              0x0079
#define VO_PIC_GC1_m01_shift                                                         (12)
#define VO_PIC_GC1_m01_mask                                                          (0x00FFF000)
#define VO_PIC_GC1_m01(data)                                                         (0x00FFF000&((data)<<12))
#define VO_PIC_GC1_m01_src(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC1_get_m01(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC1_m00_shift                                                         (0)
#define VO_PIC_GC1_m00_mask                                                          (0x00000FFF)
#define VO_PIC_GC1_m00(data)                                                         (0x00000FFF&((data)<<0))
#define VO_PIC_GC1_m00_src(data)                                                     ((0x00000FFF&(data))>>0)
#define VO_PIC_GC1_get_m00(data)                                                     ((0x00000FFF&(data))>>0)


#define VO_PIC_GC2                                                                   0x180055E8
#define VO_PIC_GC2_reg_addr                                                          "0xB80055E8"
#define VO_PIC_GC2_reg                                                               0xB80055E8
#define set_VO_PIC_GC2_reg(data)   (*((volatile unsigned int*) VO_PIC_GC2_reg)=data)
#define get_VO_PIC_GC2_reg   (*((volatile unsigned int*) VO_PIC_GC2_reg))
#define VO_PIC_GC2_inst_adr                                                          "0x007A"
#define VO_PIC_GC2_inst                                                              0x007A
#define VO_PIC_GC2_m10_shift                                                         (12)
#define VO_PIC_GC2_m10_mask                                                          (0x00FFF000)
#define VO_PIC_GC2_m10(data)                                                         (0x00FFF000&((data)<<12))
#define VO_PIC_GC2_m10_src(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC2_get_m10(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC2_m02_shift                                                         (0)
#define VO_PIC_GC2_m02_mask                                                          (0x00000FFF)
#define VO_PIC_GC2_m02(data)                                                         (0x00000FFF&((data)<<0))
#define VO_PIC_GC2_m02_src(data)                                                     ((0x00000FFF&(data))>>0)
#define VO_PIC_GC2_get_m02(data)                                                     ((0x00000FFF&(data))>>0)


#define VO_PIC_GC3                                                                   0x180055EC
#define VO_PIC_GC3_reg_addr                                                          "0xB80055EC"
#define VO_PIC_GC3_reg                                                               0xB80055EC
#define set_VO_PIC_GC3_reg(data)   (*((volatile unsigned int*) VO_PIC_GC3_reg)=data)
#define get_VO_PIC_GC3_reg   (*((volatile unsigned int*) VO_PIC_GC3_reg))
#define VO_PIC_GC3_inst_adr                                                          "0x007B"
#define VO_PIC_GC3_inst                                                              0x007B
#define VO_PIC_GC3_m12_shift                                                         (12)
#define VO_PIC_GC3_m12_mask                                                          (0x00FFF000)
#define VO_PIC_GC3_m12(data)                                                         (0x00FFF000&((data)<<12))
#define VO_PIC_GC3_m12_src(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC3_get_m12(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC3_m11_shift                                                         (0)
#define VO_PIC_GC3_m11_mask                                                          (0x00000FFF)
#define VO_PIC_GC3_m11(data)                                                         (0x00000FFF&((data)<<0))
#define VO_PIC_GC3_m11_src(data)                                                     ((0x00000FFF&(data))>>0)
#define VO_PIC_GC3_get_m11(data)                                                     ((0x00000FFF&(data))>>0)


#define VO_PIC_GC4                                                                   0x180055F0
#define VO_PIC_GC4_reg_addr                                                          "0xB80055F0"
#define VO_PIC_GC4_reg                                                               0xB80055F0
#define set_VO_PIC_GC4_reg(data)   (*((volatile unsigned int*) VO_PIC_GC4_reg)=data)
#define get_VO_PIC_GC4_reg   (*((volatile unsigned int*) VO_PIC_GC4_reg))
#define VO_PIC_GC4_inst_adr                                                          "0x007C"
#define VO_PIC_GC4_inst                                                              0x007C
#define VO_PIC_GC4_m21_shift                                                         (12)
#define VO_PIC_GC4_m21_mask                                                          (0x00FFF000)
#define VO_PIC_GC4_m21(data)                                                         (0x00FFF000&((data)<<12))
#define VO_PIC_GC4_m21_src(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC4_get_m21(data)                                                     ((0x00FFF000&(data))>>12)
#define VO_PIC_GC4_m20_shift                                                         (0)
#define VO_PIC_GC4_m20_mask                                                          (0x00000FFF)
#define VO_PIC_GC4_m20(data)                                                         (0x00000FFF&((data)<<0))
#define VO_PIC_GC4_m20_src(data)                                                     ((0x00000FFF&(data))>>0)
#define VO_PIC_GC4_get_m20(data)                                                     ((0x00000FFF&(data))>>0)


#define VO_PIC_GC5                                                                   0x180055F4
#define VO_PIC_GC5_reg_addr                                                          "0xB80055F4"
#define VO_PIC_GC5_reg                                                               0xB80055F4
#define set_VO_PIC_GC5_reg(data)   (*((volatile unsigned int*) VO_PIC_GC5_reg)=data)
#define get_VO_PIC_GC5_reg   (*((volatile unsigned int*) VO_PIC_GC5_reg))
#define VO_PIC_GC5_inst_adr                                                          "0x007D"
#define VO_PIC_GC5_inst                                                              0x007D
#define VO_PIC_GC5_m22_shift                                                         (0)
#define VO_PIC_GC5_m22_mask                                                          (0x00000FFF)
#define VO_PIC_GC5_m22(data)                                                         (0x00000FFF&((data)<<0))
#define VO_PIC_GC5_m22_src(data)                                                     ((0x00000FFF&(data))>>0)
#define VO_PIC_GC5_get_m22(data)                                                     ((0x00000FFF&(data))>>0)


#define VO_PIC_GC6                                                                   0x180055F8
#define VO_PIC_GC6_reg_addr                                                          "0xB80055F8"
#define VO_PIC_GC6_reg                                                               0xB80055F8
#define set_VO_PIC_GC6_reg(data)   (*((volatile unsigned int*) VO_PIC_GC6_reg)=data)
#define get_VO_PIC_GC6_reg   (*((volatile unsigned int*) VO_PIC_GC6_reg))
#define VO_PIC_GC6_inst_adr                                                          "0x007E"
#define VO_PIC_GC6_inst                                                              0x007E
#define VO_PIC_GC6_a2_shift                                                          (18)
#define VO_PIC_GC6_a2_mask                                                           (0x07FC0000)
#define VO_PIC_GC6_a2(data)                                                          (0x07FC0000&((data)<<18))
#define VO_PIC_GC6_a2_src(data)                                                      ((0x07FC0000&(data))>>18)
#define VO_PIC_GC6_get_a2(data)                                                      ((0x07FC0000&(data))>>18)
#define VO_PIC_GC6_a1_shift                                                          (9)
#define VO_PIC_GC6_a1_mask                                                           (0x0003FE00)
#define VO_PIC_GC6_a1(data)                                                          (0x0003FE00&((data)<<9))
#define VO_PIC_GC6_a1_src(data)                                                      ((0x0003FE00&(data))>>9)
#define VO_PIC_GC6_get_a1(data)                                                      ((0x0003FE00&(data))>>9)
#define VO_PIC_GC6_a0_shift                                                          (0)
#define VO_PIC_GC6_a0_mask                                                           (0x000001FF)
#define VO_PIC_GC6_a0(data)                                                          (0x000001FF&((data)<<0))
#define VO_PIC_GC6_a0_src(data)                                                      ((0x000001FF&(data))>>0)
#define VO_PIC_GC6_get_a0(data)                                                      ((0x000001FF&(data))>>0)


#define VO_PIC_SIZE                                                                  0x180055FC
#define VO_PIC_SIZE_reg_addr                                                         "0xB80055FC"
#define VO_PIC_SIZE_reg                                                              0xB80055FC
#define set_VO_PIC_SIZE_reg(data)   (*((volatile unsigned int*) VO_PIC_SIZE_reg)=data)
#define get_VO_PIC_SIZE_reg   (*((volatile unsigned int*) VO_PIC_SIZE_reg))
#define VO_PIC_SIZE_inst_adr                                                         "0x007F"
#define VO_PIC_SIZE_inst                                                             0x007F
#define VO_PIC_SIZE_w_shift                                                          (12)
#define VO_PIC_SIZE_w_mask                                                           (0x007FF000)
#define VO_PIC_SIZE_w(data)                                                          (0x007FF000&((data)<<12))
#define VO_PIC_SIZE_w_src(data)                                                      ((0x007FF000&(data))>>12)
#define VO_PIC_SIZE_get_w(data)                                                      ((0x007FF000&(data))>>12)
#define VO_PIC_SIZE_h_shift                                                          (0)
#define VO_PIC_SIZE_h_mask                                                           (0x000007FF)
#define VO_PIC_SIZE_h(data)                                                          (0x000007FF&((data)<<0))
#define VO_PIC_SIZE_h_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_PIC_SIZE_get_h(data)                                                      ((0x000007FF&(data))>>0)


#define VO_PIC_ALPHA                                                                 0x18005600
#define VO_PIC_ALPHA_reg_addr                                                        "0xB8005600"
#define VO_PIC_ALPHA_reg                                                             0xB8005600
#define set_VO_PIC_ALPHA_reg(data)   (*((volatile unsigned int*) VO_PIC_ALPHA_reg)=data)
#define get_VO_PIC_ALPHA_reg   (*((volatile unsigned int*) VO_PIC_ALPHA_reg))
#define VO_PIC_ALPHA_inst_adr                                                        "0x0080"
#define VO_PIC_ALPHA_inst                                                            0x0080
#define VO_PIC_ALPHA_a_shift                                                         (0)
#define VO_PIC_ALPHA_a_mask                                                          (0x000001FF)
#define VO_PIC_ALPHA_a(data)                                                         (0x000001FF&((data)<<0))
#define VO_PIC_ALPHA_a_src(data)                                                     ((0x000001FF&(data))>>0)
#define VO_PIC_ALPHA_get_a(data)                                                     ((0x000001FF&(data))>>0)


#define VO_PIC_I                                                                     0x18005604
#define VO_PIC_I_reg_addr                                                            "0xB8005604"
#define VO_PIC_I_reg                                                                 0xB8005604
#define set_VO_PIC_I_reg(data)   (*((volatile unsigned int*) VO_PIC_I_reg)=data)
#define get_VO_PIC_I_reg   (*((volatile unsigned int*) VO_PIC_I_reg))
#define VO_PIC_I_inst_adr                                                            "0x0081"
#define VO_PIC_I_inst                                                                0x0081
#define VO_PIC_I_x_shift                                                             (12)
#define VO_PIC_I_x_mask                                                              (0x007FF000)
#define VO_PIC_I_x(data)                                                             (0x007FF000&((data)<<12))
#define VO_PIC_I_x_src(data)                                                         ((0x007FF000&(data))>>12)
#define VO_PIC_I_get_x(data)                                                         ((0x007FF000&(data))>>12)
#define VO_PIC_I_y_shift                                                             (0)
#define VO_PIC_I_y_mask                                                              (0x000007FF)
#define VO_PIC_I_y(data)                                                             (0x000007FF&((data)<<0))
#define VO_PIC_I_y_src(data)                                                         ((0x000007FF&(data))>>0)
#define VO_PIC_I_get_y(data)                                                         ((0x000007FF&(data))>>0)


#define VO_DDWB                                                                      0x18005608
#define VO_DDWB_reg_addr                                                             "0xB8005608"
#define VO_DDWB_reg                                                                  0xB8005608
#define set_VO_DDWB_reg(data)   (*((volatile unsigned int*) VO_DDWB_reg)=data)
#define get_VO_DDWB_reg   (*((volatile unsigned int*) VO_DDWB_reg))
#define VO_DDWB_inst_adr                                                             "0x0082"
#define VO_DDWB_inst                                                                 0x0082
#define VO_DDWB_h2v_shift                                                            (16)
#define VO_DDWB_h2v_mask                                                             (0x00010000)
#define VO_DDWB_h2v(data)                                                            (0x00010000&((data)<<16))
#define VO_DDWB_h2v_src(data)                                                        ((0x00010000&(data))>>16)
#define VO_DDWB_get_h2v(data)                                                        ((0x00010000&(data))>>16)
#define VO_DDWB_af_shift                                                             (13)
#define VO_DDWB_af_mask                                                              (0x00002000)
#define VO_DDWB_af(data)                                                             (0x00002000&((data)<<13))
#define VO_DDWB_af_src(data)                                                         ((0x00002000&(data))>>13)
#define VO_DDWB_get_af(data)                                                         ((0x00002000&(data))>>13)
#define VO_DDWB_hd_shift                                                             (12)
#define VO_DDWB_hd_mask                                                              (0x00001000)
#define VO_DDWB_hd(data)                                                             (0x00001000&((data)<<12))
#define VO_DDWB_hd_src(data)                                                         ((0x00001000&(data))>>12)
#define VO_DDWB_get_hd(data)                                                         ((0x00001000&(data))>>12)
#define VO_DDWB_dd_shift                                                             (11)
#define VO_DDWB_dd_mask                                                              (0x00000800)
#define VO_DDWB_dd(data)                                                             (0x00000800&((data)<<11))
#define VO_DDWB_dd_src(data)                                                         ((0x00000800&(data))>>11)
#define VO_DDWB_get_dd(data)                                                         ((0x00000800&(data))>>11)
#define VO_DDWB_f422_shift                                                           (10)
#define VO_DDWB_f422_mask                                                            (0x00000400)
#define VO_DDWB_f422(data)                                                           (0x00000400&((data)<<10))
#define VO_DDWB_f422_src(data)                                                       ((0x00000400&(data))>>10)
#define VO_DDWB_get_f422(data)                                                       ((0x00000400&(data))>>10)
#define VO_DDWB_wb_shift                                                             (9)
#define VO_DDWB_wb_mask                                                              (0x00000200)
#define VO_DDWB_wb(data)                                                             (0x00000200&((data)<<9))
#define VO_DDWB_wb_src(data)                                                         ((0x00000200&(data))>>9)
#define VO_DDWB_get_wb(data)                                                         ((0x00000200&(data))>>9)
#define VO_DDWB_reint_shift                                                          (8)
#define VO_DDWB_reint_mask                                                           (0x00000100)
#define VO_DDWB_reint(data)                                                          (0x00000100&((data)<<8))
#define VO_DDWB_reint_src(data)                                                      ((0x00000100&(data))>>8)
#define VO_DDWB_get_reint(data)                                                      ((0x00000100&(data))>>8)
#define VO_DDWB_bot_shift                                                            (7)
#define VO_DDWB_bot_mask                                                             (0x00000080)
#define VO_DDWB_bot(data)                                                            (0x00000080&((data)<<7))
#define VO_DDWB_bot_src(data)                                                        ((0x00000080&(data))>>7)
#define VO_DDWB_get_bot(data)                                                        ((0x00000080&(data))>>7)
#define VO_DDWB_vs_shift                                                             (6)
#define VO_DDWB_vs_mask                                                              (0x00000040)
#define VO_DDWB_vs(data)                                                             (0x00000040&((data)<<6))
#define VO_DDWB_vs_src(data)                                                         ((0x00000040&(data))>>6)
#define VO_DDWB_get_vs(data)                                                         ((0x00000040&(data))>>6)
#define VO_DDWB_vs_yodd_shift                                                        (5)
#define VO_DDWB_vs_yodd_mask                                                         (0x00000020)
#define VO_DDWB_vs_yodd(data)                                                        (0x00000020&((data)<<5))
#define VO_DDWB_vs_yodd_src(data)                                                    ((0x00000020&(data))>>5)
#define VO_DDWB_get_vs_yodd(data)                                                    ((0x00000020&(data))>>5)
#define VO_DDWB_vs_codd_shift                                                        (4)
#define VO_DDWB_vs_codd_mask                                                         (0x00000010)
#define VO_DDWB_vs_codd(data)                                                        (0x00000010&((data)<<4))
#define VO_DDWB_vs_codd_src(data)                                                    ((0x00000010&(data))>>4)
#define VO_DDWB_get_vs_codd(data)                                                    ((0x00000010&(data))>>4)
#define VO_DDWB_hs_shift                                                             (3)
#define VO_DDWB_hs_mask                                                              (0x00000008)
#define VO_DDWB_hs(data)                                                             (0x00000008&((data)<<3))
#define VO_DDWB_hs_src(data)                                                         ((0x00000008&(data))>>3)
#define VO_DDWB_get_hs(data)                                                         ((0x00000008&(data))>>3)
#define VO_DDWB_hs_yodd_shift                                                        (2)
#define VO_DDWB_hs_yodd_mask                                                         (0x00000004)
#define VO_DDWB_hs_yodd(data)                                                        (0x00000004&((data)<<2))
#define VO_DDWB_hs_yodd_src(data)                                                    ((0x00000004&(data))>>2)
#define VO_DDWB_get_hs_yodd(data)                                                    ((0x00000004&(data))>>2)
#define VO_DDWB_hs_codd_shift                                                        (1)
#define VO_DDWB_hs_codd_mask                                                         (0x00000002)
#define VO_DDWB_hs_codd(data)                                                        (0x00000002&((data)<<1))
#define VO_DDWB_hs_codd_src(data)                                                    ((0x00000002&(data))>>1)
#define VO_DDWB_get_hs_codd(data)                                                    ((0x00000002&(data))>>1)
#define VO_DDWB_write_data_shift                                                     (0)
#define VO_DDWB_write_data_mask                                                      (0x00000001)
#define VO_DDWB_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_DDWB_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_DDWB_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_DD_INDEX                                                                  0x1800560C
#define VO_DD_INDEX_reg_addr                                                         "0xB800560C"
#define VO_DD_INDEX_reg                                                              0xB800560C
#define set_VO_DD_INDEX_reg(data)   (*((volatile unsigned int*) VO_DD_INDEX_reg)=data)
#define get_VO_DD_INDEX_reg   (*((volatile unsigned int*) VO_DD_INDEX_reg))
#define VO_DD_INDEX_inst_adr                                                         "0x0083"
#define VO_DD_INDEX_inst                                                             0x0083
#define VO_DD_INDEX_write_en5_shift                                                  (27)
#define VO_DD_INDEX_write_en5_mask                                                   (0x08000000)
#define VO_DD_INDEX_write_en5(data)                                                  (0x08000000&((data)<<27))
#define VO_DD_INDEX_write_en5_src(data)                                              ((0x08000000&(data))>>27)
#define VO_DD_INDEX_get_write_en5(data)                                              ((0x08000000&(data))>>27)
#define VO_DD_INDEX_interlace_shift                                                  (26)
#define VO_DD_INDEX_interlace_mask                                                   (0x04000000)
#define VO_DD_INDEX_interlace(data)                                                  (0x04000000&((data)<<26))
#define VO_DD_INDEX_interlace_src(data)                                              ((0x04000000&(data))>>26)
#define VO_DD_INDEX_get_interlace(data)                                              ((0x04000000&(data))>>26)
#define VO_DD_INDEX_write_en4_shift                                                  (25)
#define VO_DD_INDEX_write_en4_mask                                                   (0x02000000)
#define VO_DD_INDEX_write_en4(data)                                                  (0x02000000&((data)<<25))
#define VO_DD_INDEX_write_en4_src(data)                                              ((0x02000000&(data))>>25)
#define VO_DD_INDEX_get_write_en4(data)                                              ((0x02000000&(data))>>25)
#define VO_DD_INDEX_bot_shift                                                        (24)
#define VO_DD_INDEX_bot_mask                                                         (0x01000000)
#define VO_DD_INDEX_bot(data)                                                        (0x01000000&((data)<<24))
#define VO_DD_INDEX_bot_src(data)                                                    ((0x01000000&(data))>>24)
#define VO_DD_INDEX_get_bot(data)                                                    ((0x01000000&(data))>>24)
#define VO_DD_INDEX_write_en2_shift                                                  (15)
#define VO_DD_INDEX_write_en2_mask                                                   (0x00008000)
#define VO_DD_INDEX_write_en2(data)                                                  (0x00008000&((data)<<15))
#define VO_DD_INDEX_write_en2_src(data)                                              ((0x00008000&(data))>>15)
#define VO_DD_INDEX_get_write_en2(data)                                              ((0x00008000&(data))>>15)
#define VO_DD_INDEX_c_shift                                                          (8)
#define VO_DD_INDEX_c_mask                                                           (0x00007F00)
#define VO_DD_INDEX_c(data)                                                          (0x00007F00&((data)<<8))
#define VO_DD_INDEX_c_src(data)                                                      ((0x00007F00&(data))>>8)
#define VO_DD_INDEX_get_c(data)                                                      ((0x00007F00&(data))>>8)
#define VO_DD_INDEX_write_en1_shift                                                  (7)
#define VO_DD_INDEX_write_en1_mask                                                   (0x00000080)
#define VO_DD_INDEX_write_en1(data)                                                  (0x00000080&((data)<<7))
#define VO_DD_INDEX_write_en1_src(data)                                              ((0x00000080&(data))>>7)
#define VO_DD_INDEX_get_write_en1(data)                                              ((0x00000080&(data))>>7)
#define VO_DD_INDEX_y_shift                                                          (0)
#define VO_DD_INDEX_y_mask                                                           (0x0000007F)
#define VO_DD_INDEX_y(data)                                                          (0x0000007F&((data)<<0))
#define VO_DD_INDEX_y_src(data)                                                      ((0x0000007F&(data))>>0)
#define VO_DD_INDEX_get_y(data)                                                      ((0x0000007F&(data))>>0)


#define VO_WB_INDEX                                                                  0x18005610
#define VO_WB_INDEX_reg_addr                                                         "0xB8005610"
#define VO_WB_INDEX_reg                                                              0xB8005610
#define set_VO_WB_INDEX_reg(data)   (*((volatile unsigned int*) VO_WB_INDEX_reg)=data)
#define get_VO_WB_INDEX_reg   (*((volatile unsigned int*) VO_WB_INDEX_reg))
#define VO_WB_INDEX_inst_adr                                                         "0x0084"
#define VO_WB_INDEX_inst                                                             0x0084
#define VO_WB_INDEX_write_en5_shift                                                  (27)
#define VO_WB_INDEX_write_en5_mask                                                   (0x08000000)
#define VO_WB_INDEX_write_en5(data)                                                  (0x08000000&((data)<<27))
#define VO_WB_INDEX_write_en5_src(data)                                              ((0x08000000&(data))>>27)
#define VO_WB_INDEX_get_write_en5(data)                                              ((0x08000000&(data))>>27)
#define VO_WB_INDEX_interlace_shift                                                  (26)
#define VO_WB_INDEX_interlace_mask                                                   (0x04000000)
#define VO_WB_INDEX_interlace(data)                                                  (0x04000000&((data)<<26))
#define VO_WB_INDEX_interlace_src(data)                                              ((0x04000000&(data))>>26)
#define VO_WB_INDEX_get_interlace(data)                                              ((0x04000000&(data))>>26)
#define VO_WB_INDEX_write_en4_shift                                                  (25)
#define VO_WB_INDEX_write_en4_mask                                                   (0x02000000)
#define VO_WB_INDEX_write_en4(data)                                                  (0x02000000&((data)<<25))
#define VO_WB_INDEX_write_en4_src(data)                                              ((0x02000000&(data))>>25)
#define VO_WB_INDEX_get_write_en4(data)                                              ((0x02000000&(data))>>25)
#define VO_WB_INDEX_bot_shift                                                        (24)
#define VO_WB_INDEX_bot_mask                                                         (0x01000000)
#define VO_WB_INDEX_bot(data)                                                        (0x01000000&((data)<<24))
#define VO_WB_INDEX_bot_src(data)                                                    ((0x01000000&(data))>>24)
#define VO_WB_INDEX_get_bot(data)                                                    ((0x01000000&(data))>>24)
#define VO_WB_INDEX_write_en2_shift                                                  (15)
#define VO_WB_INDEX_write_en2_mask                                                   (0x00008000)
#define VO_WB_INDEX_write_en2(data)                                                  (0x00008000&((data)<<15))
#define VO_WB_INDEX_write_en2_src(data)                                              ((0x00008000&(data))>>15)
#define VO_WB_INDEX_get_write_en2(data)                                              ((0x00008000&(data))>>15)
#define VO_WB_INDEX_c_shift                                                          (8)
#define VO_WB_INDEX_c_mask                                                           (0x00007F00)
#define VO_WB_INDEX_c(data)                                                          (0x00007F00&((data)<<8))
#define VO_WB_INDEX_c_src(data)                                                      ((0x00007F00&(data))>>8)
#define VO_WB_INDEX_get_c(data)                                                      ((0x00007F00&(data))>>8)
#define VO_WB_INDEX_write_en1_shift                                                  (7)
#define VO_WB_INDEX_write_en1_mask                                                   (0x00000080)
#define VO_WB_INDEX_write_en1(data)                                                  (0x00000080&((data)<<7))
#define VO_WB_INDEX_write_en1_src(data)                                              ((0x00000080&(data))>>7)
#define VO_WB_INDEX_get_write_en1(data)                                              ((0x00000080&(data))>>7)
#define VO_WB_INDEX_y_shift                                                          (0)
#define VO_WB_INDEX_y_mask                                                           (0x0000007F)
#define VO_WB_INDEX_y(data)                                                          (0x0000007F&((data)<<0))
#define VO_WB_INDEX_y_src(data)                                                      ((0x0000007F&(data))>>0)
#define VO_WB_INDEX_get_y(data)                                                      ((0x0000007F&(data))>>0)


#define VO_DD_SIZE                                                                   0x18005614
#define VO_DD_SIZE_reg_addr                                                          "0xB8005614"
#define VO_DD_SIZE_reg                                                               0xB8005614
#define set_VO_DD_SIZE_reg(data)   (*((volatile unsigned int*) VO_DD_SIZE_reg)=data)
#define get_VO_DD_SIZE_reg   (*((volatile unsigned int*) VO_DD_SIZE_reg))
#define VO_DD_SIZE_inst_adr                                                          "0x0085"
#define VO_DD_SIZE_inst                                                              0x0085
#define VO_DD_SIZE_w_shift                                                           (12)
#define VO_DD_SIZE_w_mask                                                            (0x007FF000)
#define VO_DD_SIZE_w(data)                                                           (0x007FF000&((data)<<12))
#define VO_DD_SIZE_w_src(data)                                                       ((0x007FF000&(data))>>12)
#define VO_DD_SIZE_get_w(data)                                                       ((0x007FF000&(data))>>12)
#define VO_DD_SIZE_h_shift                                                           (0)
#define VO_DD_SIZE_h_mask                                                            (0x000007FF)
#define VO_DD_SIZE_h(data)                                                           (0x000007FF&((data)<<0))
#define VO_DD_SIZE_h_src(data)                                                       ((0x000007FF&(data))>>0)
#define VO_DD_SIZE_get_h(data)                                                       ((0x000007FF&(data))>>0)


#define VO_WB_SIZE                                                                   0x18005618
#define VO_WB_SIZE_reg_addr                                                          "0xB8005618"
#define VO_WB_SIZE_reg                                                               0xB8005618
#define set_VO_WB_SIZE_reg(data)   (*((volatile unsigned int*) VO_WB_SIZE_reg)=data)
#define get_VO_WB_SIZE_reg   (*((volatile unsigned int*) VO_WB_SIZE_reg))
#define VO_WB_SIZE_inst_adr                                                          "0x0086"
#define VO_WB_SIZE_inst                                                              0x0086
#define VO_WB_SIZE_w_shift                                                           (12)
#define VO_WB_SIZE_w_mask                                                            (0x007FF000)
#define VO_WB_SIZE_w(data)                                                           (0x007FF000&((data)<<12))
#define VO_WB_SIZE_w_src(data)                                                       ((0x007FF000&(data))>>12)
#define VO_WB_SIZE_get_w(data)                                                       ((0x007FF000&(data))>>12)
#define VO_WB_SIZE_h_shift                                                           (0)
#define VO_WB_SIZE_h_mask                                                            (0x000007FF)
#define VO_WB_SIZE_h(data)                                                           (0x000007FF&((data)<<0))
#define VO_WB_SIZE_h_src(data)                                                       ((0x000007FF&(data))>>0)
#define VO_WB_SIZE_get_h(data)                                                       ((0x000007FF&(data))>>0)


#define VO_WB_TPC                                                                    0x1800561C
#define VO_WB_TPC_reg_addr                                                           "0xB800561C"
#define VO_WB_TPC_reg                                                                0xB800561C
#define set_VO_WB_TPC_reg(data)   (*((volatile unsigned int*) VO_WB_TPC_reg)=data)
#define get_VO_WB_TPC_reg   (*((volatile unsigned int*) VO_WB_TPC_reg))
#define VO_WB_TPC_inst_adr                                                           "0x0087"
#define VO_WB_TPC_inst                                                               0x0087
#define VO_WB_TPC_num_shift                                                          (0)
#define VO_WB_TPC_num_mask                                                           (0x000000FF)
#define VO_WB_TPC_num(data)                                                          (0x000000FF&((data)<<0))
#define VO_WB_TPC_num_src(data)                                                      ((0x000000FF&(data))>>0)
#define VO_WB_TPC_get_num(data)                                                      ((0x000000FF&(data))>>0)


#define VO_DDWB_VSI                                                                  0x18005620
#define VO_DDWB_VSI_reg_addr                                                         "0xB8005620"
#define VO_DDWB_VSI_reg                                                              0xB8005620
#define set_VO_DDWB_VSI_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSI_reg)=data)
#define get_VO_DDWB_VSI_reg   (*((volatile unsigned int*) VO_DDWB_VSI_reg))
#define VO_DDWB_VSI_inst_adr                                                         "0x0088"
#define VO_DDWB_VSI_inst                                                             0x0088
#define VO_DDWB_VSI_offset_shift                                                     (16)
#define VO_DDWB_VSI_offset_mask                                                      (0x07FF0000)
#define VO_DDWB_VSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_DDWB_VSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_DDWB_VSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_DDWB_VSI_phase_shift                                                      (2)
#define VO_DDWB_VSI_phase_mask                                                       (0x0000FFFC)
#define VO_DDWB_VSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_DDWB_VSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_DDWB_VSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_DDWB_VSD                                                                  0x18005624
#define VO_DDWB_VSD_reg_addr                                                         "0xB8005624"
#define VO_DDWB_VSD_reg                                                              0xB8005624
#define set_VO_DDWB_VSD_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSD_reg)=data)
#define get_VO_DDWB_VSD_reg   (*((volatile unsigned int*) VO_DDWB_VSD_reg))
#define VO_DDWB_VSD_inst_adr                                                         "0x0089"
#define VO_DDWB_VSD_inst                                                             0x0089
#define VO_DDWB_VSD_out_shift                                                        (20)
#define VO_DDWB_VSD_out_mask                                                         (0x7FF00000)
#define VO_DDWB_VSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_DDWB_VSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_DDWB_VSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_DDWB_VSD_delta_shift                                                      (0)
#define VO_DDWB_VSD_delta_mask                                                       (0x0007FFFF)
#define VO_DDWB_VSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_DDWB_VSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_DDWB_VSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_DDWB_VSYC_0                                                               0x18005628
#define VO_DDWB_VSYC_1                                                               0x1800562C
#define VO_DDWB_VSYC_2                                                               0x18005630
#define VO_DDWB_VSYC_3                                                               0x18005634
#define VO_DDWB_VSYC_4                                                               0x18005638
#define VO_DDWB_VSYC_5                                                               0x1800563C
#define VO_DDWB_VSYC_6                                                               0x18005640
#define VO_DDWB_VSYC_7                                                               0x18005644
#define VO_DDWB_VSYC_0_reg_addr                                                      "0xB8005628"
#define VO_DDWB_VSYC_1_reg_addr                                                      "0xB800562C"
#define VO_DDWB_VSYC_2_reg_addr                                                      "0xB8005630"
#define VO_DDWB_VSYC_3_reg_addr                                                      "0xB8005634"
#define VO_DDWB_VSYC_4_reg_addr                                                      "0xB8005638"
#define VO_DDWB_VSYC_5_reg_addr                                                      "0xB800563C"
#define VO_DDWB_VSYC_6_reg_addr                                                      "0xB8005640"
#define VO_DDWB_VSYC_7_reg_addr                                                      "0xB8005644"
#define VO_DDWB_VSYC_0_reg                                                           0xB8005628
#define VO_DDWB_VSYC_1_reg                                                           0xB800562C
#define VO_DDWB_VSYC_2_reg                                                           0xB8005630
#define VO_DDWB_VSYC_3_reg                                                           0xB8005634
#define VO_DDWB_VSYC_4_reg                                                           0xB8005638
#define VO_DDWB_VSYC_5_reg                                                           0xB800563C
#define VO_DDWB_VSYC_6_reg                                                           0xB8005640
#define VO_DDWB_VSYC_7_reg                                                           0xB8005644
#define set_VO_DDWB_VSYC_0_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_0_reg)=data)
#define set_VO_DDWB_VSYC_1_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_1_reg)=data)
#define set_VO_DDWB_VSYC_2_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_2_reg)=data)
#define set_VO_DDWB_VSYC_3_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_3_reg)=data)
#define set_VO_DDWB_VSYC_4_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_4_reg)=data)
#define set_VO_DDWB_VSYC_5_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_5_reg)=data)
#define set_VO_DDWB_VSYC_6_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_6_reg)=data)
#define set_VO_DDWB_VSYC_7_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSYC_7_reg)=data)
#define get_VO_DDWB_VSYC_0_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_0_reg))
#define get_VO_DDWB_VSYC_1_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_1_reg))
#define get_VO_DDWB_VSYC_2_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_2_reg))
#define get_VO_DDWB_VSYC_3_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_3_reg))
#define get_VO_DDWB_VSYC_4_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_4_reg))
#define get_VO_DDWB_VSYC_5_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_5_reg))
#define get_VO_DDWB_VSYC_6_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_6_reg))
#define get_VO_DDWB_VSYC_7_reg   (*((volatile unsigned int*) VO_DDWB_VSYC_7_reg))
#define VO_DDWB_VSYC_0_inst_adr                                                      "0x008A"
#define VO_DDWB_VSYC_1_inst_adr                                                      "0x008B"
#define VO_DDWB_VSYC_2_inst_adr                                                      "0x008C"
#define VO_DDWB_VSYC_3_inst_adr                                                      "0x008D"
#define VO_DDWB_VSYC_4_inst_adr                                                      "0x008E"
#define VO_DDWB_VSYC_5_inst_adr                                                      "0x008F"
#define VO_DDWB_VSYC_6_inst_adr                                                      "0x0090"
#define VO_DDWB_VSYC_7_inst_adr                                                      "0x0091"
#define VO_DDWB_VSYC_0_inst                                                          0x008A
#define VO_DDWB_VSYC_1_inst                                                          0x008B
#define VO_DDWB_VSYC_2_inst                                                          0x008C
#define VO_DDWB_VSYC_3_inst                                                          0x008D
#define VO_DDWB_VSYC_4_inst                                                          0x008E
#define VO_DDWB_VSYC_5_inst                                                          0x008F
#define VO_DDWB_VSYC_6_inst                                                          0x0090
#define VO_DDWB_VSYC_7_inst                                                          0x0091
#define VO_DDWB_VSYC_c1_shift                                                        (16)
#define VO_DDWB_VSYC_c1_mask                                                         (0x3FFF0000)
#define VO_DDWB_VSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_DDWB_VSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_VSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_VSYC_c0_shift                                                        (0)
#define VO_DDWB_VSYC_c0_mask                                                         (0x00003FFF)
#define VO_DDWB_VSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_DDWB_VSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_DDWB_VSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_DDWB_VSCC_0                                                               0x18005648
#define VO_DDWB_VSCC_1                                                               0x1800564C
#define VO_DDWB_VSCC_2                                                               0x18005650
#define VO_DDWB_VSCC_3                                                               0x18005654
#define VO_DDWB_VSCC_4                                                               0x18005658
#define VO_DDWB_VSCC_5                                                               0x1800565C
#define VO_DDWB_VSCC_6                                                               0x18005660
#define VO_DDWB_VSCC_7                                                               0x18005664
#define VO_DDWB_VSCC_0_reg_addr                                                      "0xB8005648"
#define VO_DDWB_VSCC_1_reg_addr                                                      "0xB800564C"
#define VO_DDWB_VSCC_2_reg_addr                                                      "0xB8005650"
#define VO_DDWB_VSCC_3_reg_addr                                                      "0xB8005654"
#define VO_DDWB_VSCC_4_reg_addr                                                      "0xB8005658"
#define VO_DDWB_VSCC_5_reg_addr                                                      "0xB800565C"
#define VO_DDWB_VSCC_6_reg_addr                                                      "0xB8005660"
#define VO_DDWB_VSCC_7_reg_addr                                                      "0xB8005664"
#define VO_DDWB_VSCC_0_reg                                                           0xB8005648
#define VO_DDWB_VSCC_1_reg                                                           0xB800564C
#define VO_DDWB_VSCC_2_reg                                                           0xB8005650
#define VO_DDWB_VSCC_3_reg                                                           0xB8005654
#define VO_DDWB_VSCC_4_reg                                                           0xB8005658
#define VO_DDWB_VSCC_5_reg                                                           0xB800565C
#define VO_DDWB_VSCC_6_reg                                                           0xB8005660
#define VO_DDWB_VSCC_7_reg                                                           0xB8005664
#define set_VO_DDWB_VSCC_0_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_0_reg)=data)
#define set_VO_DDWB_VSCC_1_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_1_reg)=data)
#define set_VO_DDWB_VSCC_2_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_2_reg)=data)
#define set_VO_DDWB_VSCC_3_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_3_reg)=data)
#define set_VO_DDWB_VSCC_4_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_4_reg)=data)
#define set_VO_DDWB_VSCC_5_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_5_reg)=data)
#define set_VO_DDWB_VSCC_6_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_6_reg)=data)
#define set_VO_DDWB_VSCC_7_reg(data)   (*((volatile unsigned int*) VO_DDWB_VSCC_7_reg)=data)
#define get_VO_DDWB_VSCC_0_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_0_reg))
#define get_VO_DDWB_VSCC_1_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_1_reg))
#define get_VO_DDWB_VSCC_2_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_2_reg))
#define get_VO_DDWB_VSCC_3_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_3_reg))
#define get_VO_DDWB_VSCC_4_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_4_reg))
#define get_VO_DDWB_VSCC_5_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_5_reg))
#define get_VO_DDWB_VSCC_6_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_6_reg))
#define get_VO_DDWB_VSCC_7_reg   (*((volatile unsigned int*) VO_DDWB_VSCC_7_reg))
#define VO_DDWB_VSCC_0_inst_adr                                                      "0x0092"
#define VO_DDWB_VSCC_1_inst_adr                                                      "0x0093"
#define VO_DDWB_VSCC_2_inst_adr                                                      "0x0094"
#define VO_DDWB_VSCC_3_inst_adr                                                      "0x0095"
#define VO_DDWB_VSCC_4_inst_adr                                                      "0x0096"
#define VO_DDWB_VSCC_5_inst_adr                                                      "0x0097"
#define VO_DDWB_VSCC_6_inst_adr                                                      "0x0098"
#define VO_DDWB_VSCC_7_inst_adr                                                      "0x0099"
#define VO_DDWB_VSCC_0_inst                                                          0x0092
#define VO_DDWB_VSCC_1_inst                                                          0x0093
#define VO_DDWB_VSCC_2_inst                                                          0x0094
#define VO_DDWB_VSCC_3_inst                                                          0x0095
#define VO_DDWB_VSCC_4_inst                                                          0x0096
#define VO_DDWB_VSCC_5_inst                                                          0x0097
#define VO_DDWB_VSCC_6_inst                                                          0x0098
#define VO_DDWB_VSCC_7_inst                                                          0x0099
#define VO_DDWB_VSCC_c1_shift                                                        (16)
#define VO_DDWB_VSCC_c1_mask                                                         (0x3FFF0000)
#define VO_DDWB_VSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_DDWB_VSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_VSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_VSCC_c0_shift                                                        (0)
#define VO_DDWB_VSCC_c0_mask                                                         (0x00003FFF)
#define VO_DDWB_VSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_DDWB_VSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_DDWB_VSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_DDWB_HSI                                                                  0x18005668
#define VO_DDWB_HSI_reg_addr                                                         "0xB8005668"
#define VO_DDWB_HSI_reg                                                              0xB8005668
#define set_VO_DDWB_HSI_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSI_reg)=data)
#define get_VO_DDWB_HSI_reg   (*((volatile unsigned int*) VO_DDWB_HSI_reg))
#define VO_DDWB_HSI_inst_adr                                                         "0x009A"
#define VO_DDWB_HSI_inst                                                             0x009A
#define VO_DDWB_HSI_offset_shift                                                     (16)
#define VO_DDWB_HSI_offset_mask                                                      (0x07FF0000)
#define VO_DDWB_HSI_offset(data)                                                     (0x07FF0000&((data)<<16))
#define VO_DDWB_HSI_offset_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_DDWB_HSI_get_offset(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_DDWB_HSI_phase_shift                                                      (2)
#define VO_DDWB_HSI_phase_mask                                                       (0x0000FFFC)
#define VO_DDWB_HSI_phase(data)                                                      (0x0000FFFC&((data)<<2))
#define VO_DDWB_HSI_phase_src(data)                                                  ((0x0000FFFC&(data))>>2)
#define VO_DDWB_HSI_get_phase(data)                                                  ((0x0000FFFC&(data))>>2)


#define VO_DDWB_HSD                                                                  0x1800566C
#define VO_DDWB_HSD_reg_addr                                                         "0xB800566C"
#define VO_DDWB_HSD_reg                                                              0xB800566C
#define set_VO_DDWB_HSD_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSD_reg)=data)
#define get_VO_DDWB_HSD_reg   (*((volatile unsigned int*) VO_DDWB_HSD_reg))
#define VO_DDWB_HSD_inst_adr                                                         "0x009B"
#define VO_DDWB_HSD_inst                                                             0x009B
#define VO_DDWB_HSD_out_shift                                                        (20)
#define VO_DDWB_HSD_out_mask                                                         (0x7FF00000)
#define VO_DDWB_HSD_out(data)                                                        (0x7FF00000&((data)<<20))
#define VO_DDWB_HSD_out_src(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_DDWB_HSD_get_out(data)                                                    ((0x7FF00000&(data))>>20)
#define VO_DDWB_HSD_delta_shift                                                      (0)
#define VO_DDWB_HSD_delta_mask                                                       (0x0007FFFF)
#define VO_DDWB_HSD_delta(data)                                                      (0x0007FFFF&((data)<<0))
#define VO_DDWB_HSD_delta_src(data)                                                  ((0x0007FFFF&(data))>>0)
#define VO_DDWB_HSD_get_delta(data)                                                  ((0x0007FFFF&(data))>>0)


#define VO_DDWB_HSYC_0                                                               0x18005670
#define VO_DDWB_HSYC_1                                                               0x18005674
#define VO_DDWB_HSYC_2                                                               0x18005678
#define VO_DDWB_HSYC_3                                                               0x1800567C
#define VO_DDWB_HSYC_4                                                               0x18005680
#define VO_DDWB_HSYC_5                                                               0x18005684
#define VO_DDWB_HSYC_6                                                               0x18005688
#define VO_DDWB_HSYC_7                                                               0x1800568C
#define VO_DDWB_HSYC_8                                                               0x18005690
#define VO_DDWB_HSYC_9                                                               0x18005694
#define VO_DDWB_HSYC_10                                                               0x18005698
#define VO_DDWB_HSYC_11                                                               0x1800569C
#define VO_DDWB_HSYC_12                                                               0x180056A0
#define VO_DDWB_HSYC_13                                                               0x180056A4
#define VO_DDWB_HSYC_14                                                               0x180056A8
#define VO_DDWB_HSYC_15                                                               0x180056AC
#define VO_DDWB_HSYC_0_reg_addr                                                      "0xB8005670"
#define VO_DDWB_HSYC_1_reg_addr                                                      "0xB8005674"
#define VO_DDWB_HSYC_2_reg_addr                                                      "0xB8005678"
#define VO_DDWB_HSYC_3_reg_addr                                                      "0xB800567C"
#define VO_DDWB_HSYC_4_reg_addr                                                      "0xB8005680"
#define VO_DDWB_HSYC_5_reg_addr                                                      "0xB8005684"
#define VO_DDWB_HSYC_6_reg_addr                                                      "0xB8005688"
#define VO_DDWB_HSYC_7_reg_addr                                                      "0xB800568C"
#define VO_DDWB_HSYC_8_reg_addr                                                      "0xB8005690"
#define VO_DDWB_HSYC_9_reg_addr                                                      "0xB8005694"
#define VO_DDWB_HSYC_10_reg_addr                                                      "0xB8005698"
#define VO_DDWB_HSYC_11_reg_addr                                                      "0xB800569C"
#define VO_DDWB_HSYC_12_reg_addr                                                      "0xB80056A0"
#define VO_DDWB_HSYC_13_reg_addr                                                      "0xB80056A4"
#define VO_DDWB_HSYC_14_reg_addr                                                      "0xB80056A8"
#define VO_DDWB_HSYC_15_reg_addr                                                      "0xB80056AC"
#define VO_DDWB_HSYC_0_reg                                                           0xB8005670
#define VO_DDWB_HSYC_1_reg                                                           0xB8005674
#define VO_DDWB_HSYC_2_reg                                                           0xB8005678
#define VO_DDWB_HSYC_3_reg                                                           0xB800567C
#define VO_DDWB_HSYC_4_reg                                                           0xB8005680
#define VO_DDWB_HSYC_5_reg                                                           0xB8005684
#define VO_DDWB_HSYC_6_reg                                                           0xB8005688
#define VO_DDWB_HSYC_7_reg                                                           0xB800568C
#define VO_DDWB_HSYC_8_reg                                                           0xB8005690
#define VO_DDWB_HSYC_9_reg                                                           0xB8005694
#define VO_DDWB_HSYC_10_reg                                                           0xB8005698
#define VO_DDWB_HSYC_11_reg                                                           0xB800569C
#define VO_DDWB_HSYC_12_reg                                                           0xB80056A0
#define VO_DDWB_HSYC_13_reg                                                           0xB80056A4
#define VO_DDWB_HSYC_14_reg                                                           0xB80056A8
#define VO_DDWB_HSYC_15_reg                                                           0xB80056AC
#define set_VO_DDWB_HSYC_0_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_0_reg)=data)
#define set_VO_DDWB_HSYC_1_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_1_reg)=data)
#define set_VO_DDWB_HSYC_2_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_2_reg)=data)
#define set_VO_DDWB_HSYC_3_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_3_reg)=data)
#define set_VO_DDWB_HSYC_4_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_4_reg)=data)
#define set_VO_DDWB_HSYC_5_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_5_reg)=data)
#define set_VO_DDWB_HSYC_6_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_6_reg)=data)
#define set_VO_DDWB_HSYC_7_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_7_reg)=data)
#define set_VO_DDWB_HSYC_8_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_8_reg)=data)
#define set_VO_DDWB_HSYC_9_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_9_reg)=data)
#define set_VO_DDWB_HSYC_10_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_10_reg)=data)
#define set_VO_DDWB_HSYC_11_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_11_reg)=data)
#define set_VO_DDWB_HSYC_12_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_12_reg)=data)
#define set_VO_DDWB_HSYC_13_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_13_reg)=data)
#define set_VO_DDWB_HSYC_14_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_14_reg)=data)
#define set_VO_DDWB_HSYC_15_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSYC_15_reg)=data)
#define get_VO_DDWB_HSYC_0_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_0_reg))
#define get_VO_DDWB_HSYC_1_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_1_reg))
#define get_VO_DDWB_HSYC_2_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_2_reg))
#define get_VO_DDWB_HSYC_3_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_3_reg))
#define get_VO_DDWB_HSYC_4_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_4_reg))
#define get_VO_DDWB_HSYC_5_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_5_reg))
#define get_VO_DDWB_HSYC_6_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_6_reg))
#define get_VO_DDWB_HSYC_7_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_7_reg))
#define get_VO_DDWB_HSYC_8_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_8_reg))
#define get_VO_DDWB_HSYC_9_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_9_reg))
#define get_VO_DDWB_HSYC_10_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_10_reg))
#define get_VO_DDWB_HSYC_11_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_11_reg))
#define get_VO_DDWB_HSYC_12_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_12_reg))
#define get_VO_DDWB_HSYC_13_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_13_reg))
#define get_VO_DDWB_HSYC_14_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_14_reg))
#define get_VO_DDWB_HSYC_15_reg   (*((volatile unsigned int*) VO_DDWB_HSYC_15_reg))
#define VO_DDWB_HSYC_0_inst_adr                                                      "0x009C"
#define VO_DDWB_HSYC_1_inst_adr                                                      "0x009D"
#define VO_DDWB_HSYC_2_inst_adr                                                      "0x009E"
#define VO_DDWB_HSYC_3_inst_adr                                                      "0x009F"
#define VO_DDWB_HSYC_4_inst_adr                                                      "0x00A0"
#define VO_DDWB_HSYC_5_inst_adr                                                      "0x00A1"
#define VO_DDWB_HSYC_6_inst_adr                                                      "0x00A2"
#define VO_DDWB_HSYC_7_inst_adr                                                      "0x00A3"
#define VO_DDWB_HSYC_8_inst_adr                                                      "0x00A4"
#define VO_DDWB_HSYC_9_inst_adr                                                      "0x00A5"
#define VO_DDWB_HSYC_10_inst_adr                                                      "0x00A6"
#define VO_DDWB_HSYC_11_inst_adr                                                      "0x00A7"
#define VO_DDWB_HSYC_12_inst_adr                                                      "0x00A8"
#define VO_DDWB_HSYC_13_inst_adr                                                      "0x00A9"
#define VO_DDWB_HSYC_14_inst_adr                                                      "0x00AA"
#define VO_DDWB_HSYC_15_inst_adr                                                      "0x00AB"
#define VO_DDWB_HSYC_0_inst                                                          0x009C
#define VO_DDWB_HSYC_1_inst                                                          0x009D
#define VO_DDWB_HSYC_2_inst                                                          0x009E
#define VO_DDWB_HSYC_3_inst                                                          0x009F
#define VO_DDWB_HSYC_4_inst                                                          0x00A0
#define VO_DDWB_HSYC_5_inst                                                          0x00A1
#define VO_DDWB_HSYC_6_inst                                                          0x00A2
#define VO_DDWB_HSYC_7_inst                                                          0x00A3
#define VO_DDWB_HSYC_8_inst                                                          0x00A4
#define VO_DDWB_HSYC_9_inst                                                          0x00A5
#define VO_DDWB_HSYC_10_inst                                                          0x00A6
#define VO_DDWB_HSYC_11_inst                                                          0x00A7
#define VO_DDWB_HSYC_12_inst                                                          0x00A8
#define VO_DDWB_HSYC_13_inst                                                          0x00A9
#define VO_DDWB_HSYC_14_inst                                                          0x00AA
#define VO_DDWB_HSYC_15_inst                                                          0x00AB
#define VO_DDWB_HSYC_c1_shift                                                        (16)
#define VO_DDWB_HSYC_c1_mask                                                         (0x3FFF0000)
#define VO_DDWB_HSYC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_DDWB_HSYC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_HSYC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_HSYC_c0_shift                                                        (0)
#define VO_DDWB_HSYC_c0_mask                                                         (0x00003FFF)
#define VO_DDWB_HSYC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_DDWB_HSYC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_DDWB_HSYC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_DDWB_HSCC_0                                                               0x180056B0
#define VO_DDWB_HSCC_1                                                               0x180056B4
#define VO_DDWB_HSCC_2                                                               0x180056B8
#define VO_DDWB_HSCC_3                                                               0x180056BC
#define VO_DDWB_HSCC_4                                                               0x180056C0
#define VO_DDWB_HSCC_5                                                               0x180056C4
#define VO_DDWB_HSCC_6                                                               0x180056C8
#define VO_DDWB_HSCC_7                                                               0x180056CC
#define VO_DDWB_HSCC_0_reg_addr                                                      "0xB80056B0"
#define VO_DDWB_HSCC_1_reg_addr                                                      "0xB80056B4"
#define VO_DDWB_HSCC_2_reg_addr                                                      "0xB80056B8"
#define VO_DDWB_HSCC_3_reg_addr                                                      "0xB80056BC"
#define VO_DDWB_HSCC_4_reg_addr                                                      "0xB80056C0"
#define VO_DDWB_HSCC_5_reg_addr                                                      "0xB80056C4"
#define VO_DDWB_HSCC_6_reg_addr                                                      "0xB80056C8"
#define VO_DDWB_HSCC_7_reg_addr                                                      "0xB80056CC"
#define VO_DDWB_HSCC_0_reg                                                           0xB80056B0
#define VO_DDWB_HSCC_1_reg                                                           0xB80056B4
#define VO_DDWB_HSCC_2_reg                                                           0xB80056B8
#define VO_DDWB_HSCC_3_reg                                                           0xB80056BC
#define VO_DDWB_HSCC_4_reg                                                           0xB80056C0
#define VO_DDWB_HSCC_5_reg                                                           0xB80056C4
#define VO_DDWB_HSCC_6_reg                                                           0xB80056C8
#define VO_DDWB_HSCC_7_reg                                                           0xB80056CC
#define set_VO_DDWB_HSCC_0_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_0_reg)=data)
#define set_VO_DDWB_HSCC_1_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_1_reg)=data)
#define set_VO_DDWB_HSCC_2_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_2_reg)=data)
#define set_VO_DDWB_HSCC_3_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_3_reg)=data)
#define set_VO_DDWB_HSCC_4_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_4_reg)=data)
#define set_VO_DDWB_HSCC_5_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_5_reg)=data)
#define set_VO_DDWB_HSCC_6_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_6_reg)=data)
#define set_VO_DDWB_HSCC_7_reg(data)   (*((volatile unsigned int*) VO_DDWB_HSCC_7_reg)=data)
#define get_VO_DDWB_HSCC_0_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_0_reg))
#define get_VO_DDWB_HSCC_1_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_1_reg))
#define get_VO_DDWB_HSCC_2_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_2_reg))
#define get_VO_DDWB_HSCC_3_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_3_reg))
#define get_VO_DDWB_HSCC_4_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_4_reg))
#define get_VO_DDWB_HSCC_5_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_5_reg))
#define get_VO_DDWB_HSCC_6_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_6_reg))
#define get_VO_DDWB_HSCC_7_reg   (*((volatile unsigned int*) VO_DDWB_HSCC_7_reg))
#define VO_DDWB_HSCC_0_inst_adr                                                      "0x00AC"
#define VO_DDWB_HSCC_1_inst_adr                                                      "0x00AD"
#define VO_DDWB_HSCC_2_inst_adr                                                      "0x00AE"
#define VO_DDWB_HSCC_3_inst_adr                                                      "0x00AF"
#define VO_DDWB_HSCC_4_inst_adr                                                      "0x00B0"
#define VO_DDWB_HSCC_5_inst_adr                                                      "0x00B1"
#define VO_DDWB_HSCC_6_inst_adr                                                      "0x00B2"
#define VO_DDWB_HSCC_7_inst_adr                                                      "0x00B3"
#define VO_DDWB_HSCC_0_inst                                                          0x00AC
#define VO_DDWB_HSCC_1_inst                                                          0x00AD
#define VO_DDWB_HSCC_2_inst                                                          0x00AE
#define VO_DDWB_HSCC_3_inst                                                          0x00AF
#define VO_DDWB_HSCC_4_inst                                                          0x00B0
#define VO_DDWB_HSCC_5_inst                                                          0x00B1
#define VO_DDWB_HSCC_6_inst                                                          0x00B2
#define VO_DDWB_HSCC_7_inst                                                          0x00B3
#define VO_DDWB_HSCC_c1_shift                                                        (16)
#define VO_DDWB_HSCC_c1_mask                                                         (0x3FFF0000)
#define VO_DDWB_HSCC_c1(data)                                                        (0x3FFF0000&((data)<<16))
#define VO_DDWB_HSCC_c1_src(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_HSCC_get_c1(data)                                                    ((0x3FFF0000&(data))>>16)
#define VO_DDWB_HSCC_c0_shift                                                        (0)
#define VO_DDWB_HSCC_c0_mask                                                         (0x00003FFF)
#define VO_DDWB_HSCC_c0(data)                                                        (0x00003FFF&((data)<<0))
#define VO_DDWB_HSCC_c0_src(data)                                                    ((0x00003FFF&(data))>>0)
#define VO_DDWB_HSCC_get_c0(data)                                                    ((0x00003FFF&(data))>>0)


#define VO_VBI1                                                                      0x180056D0
#define VO_VBI1_reg_addr                                                             "0xB80056D0"
#define VO_VBI1_reg                                                                  0xB80056D0
#define set_VO_VBI1_reg(data)   (*((volatile unsigned int*) VO_VBI1_reg)=data)
#define get_VO_VBI1_reg   (*((volatile unsigned int*) VO_VBI1_reg))
#define VO_VBI1_inst_adr                                                             "0x00B4"
#define VO_VBI1_inst                                                                 0x00B4
#define VO_VBI1_a_shift                                                              (0)
#define VO_VBI1_a_mask                                                               (0x7FFFFFFF)
#define VO_VBI1_a(data)                                                              (0x7FFFFFFF&((data)<<0))
#define VO_VBI1_a_src(data)                                                          ((0x7FFFFFFF&(data))>>0)
#define VO_VBI1_get_a(data)                                                          ((0x7FFFFFFF&(data))>>0)


#define VO_VBI1_SIZE                                                                 0x180056D4
#define VO_VBI1_SIZE_reg_addr                                                        "0xB80056D4"
#define VO_VBI1_SIZE_reg                                                             0xB80056D4
#define set_VO_VBI1_SIZE_reg(data)   (*((volatile unsigned int*) VO_VBI1_SIZE_reg)=data)
#define get_VO_VBI1_SIZE_reg   (*((volatile unsigned int*) VO_VBI1_SIZE_reg))
#define VO_VBI1_SIZE_inst_adr                                                        "0x00B5"
#define VO_VBI1_SIZE_inst                                                            0x00B5
#define VO_VBI1_SIZE_l_shift                                                         (0)
#define VO_VBI1_SIZE_l_mask                                                          (0x0003FFFF)
#define VO_VBI1_SIZE_l(data)                                                         (0x0003FFFF&((data)<<0))
#define VO_VBI1_SIZE_l_src(data)                                                     ((0x0003FFFF&(data))>>0)
#define VO_VBI1_SIZE_get_l(data)                                                     ((0x0003FFFF&(data))>>0)


#define VO_VBI2                                                                      0x180056D8
#define VO_VBI2_reg_addr                                                             "0xB80056D8"
#define VO_VBI2_reg                                                                  0xB80056D8
#define set_VO_VBI2_reg(data)   (*((volatile unsigned int*) VO_VBI2_reg)=data)
#define get_VO_VBI2_reg   (*((volatile unsigned int*) VO_VBI2_reg))
#define VO_VBI2_inst_adr                                                             "0x00B6"
#define VO_VBI2_inst                                                                 0x00B6
#define VO_VBI2_a_shift                                                              (0)
#define VO_VBI2_a_mask                                                               (0x7FFFFFFF)
#define VO_VBI2_a(data)                                                              (0x7FFFFFFF&((data)<<0))
#define VO_VBI2_a_src(data)                                                          ((0x7FFFFFFF&(data))>>0)
#define VO_VBI2_get_a(data)                                                          ((0x7FFFFFFF&(data))>>0)


#define VO_VBI2_SIZE                                                                 0x180056DC
#define VO_VBI2_SIZE_reg_addr                                                        "0xB80056DC"
#define VO_VBI2_SIZE_reg                                                             0xB80056DC
#define set_VO_VBI2_SIZE_reg(data)   (*((volatile unsigned int*) VO_VBI2_SIZE_reg)=data)
#define get_VO_VBI2_SIZE_reg   (*((volatile unsigned int*) VO_VBI2_SIZE_reg))
#define VO_VBI2_SIZE_inst_adr                                                        "0x00B7"
#define VO_VBI2_SIZE_inst                                                            0x00B7
#define VO_VBI2_SIZE_l_shift                                                         (0)
#define VO_VBI2_SIZE_l_mask                                                          (0x0003FFFF)
#define VO_VBI2_SIZE_l(data)                                                         (0x0003FFFF&((data)<<0))
#define VO_VBI2_SIZE_l_src(data)                                                     ((0x0003FFFF&(data))>>0)
#define VO_VBI2_SIZE_get_l(data)                                                     ((0x0003FFFF&(data))>>0)


#define VO_DMA                                                                       0x180056E0
#define VO_DMA_reg_addr                                                              "0xB80056E0"
#define VO_DMA_reg                                                                   0xB80056E0
#define set_VO_DMA_reg(data)   (*((volatile unsigned int*) VO_DMA_reg)=data)
#define get_VO_DMA_reg   (*((volatile unsigned int*) VO_DMA_reg))
#define VO_DMA_inst_adr                                                              "0x00B8"
#define VO_DMA_inst                                                                  0x00B8
#define VO_DMA_write_en4_shift                                                       (13)
#define VO_DMA_write_en4_mask                                                        (0x00002000)
#define VO_DMA_write_en4(data)                                                       (0x00002000&((data)<<13))
#define VO_DMA_write_en4_src(data)                                                   ((0x00002000&(data))>>13)
#define VO_DMA_get_write_en4(data)                                                   ((0x00002000&(data))>>13)
#define VO_DMA_num_req_4_shift                                                       (12)
#define VO_DMA_num_req_4_mask                                                        (0x00001000)
#define VO_DMA_num_req_4(data)                                                       (0x00001000&((data)<<12))
#define VO_DMA_num_req_4_src(data)                                                   ((0x00001000&(data))>>12)
#define VO_DMA_get_num_req_4(data)                                                   ((0x00001000&(data))>>12)
#define VO_DMA_write_en3_shift                                                       (11)
#define VO_DMA_write_en3_mask                                                        (0x00000800)
#define VO_DMA_write_en3(data)                                                       (0x00000800&((data)<<11))
#define VO_DMA_write_en3_src(data)                                                   ((0x00000800&(data))>>11)
#define VO_DMA_get_write_en3(data)                                                   ((0x00000800&(data))>>11)
#define VO_DMA_dyn_id_shift                                                          (10)
#define VO_DMA_dyn_id_mask                                                           (0x00000400)
#define VO_DMA_dyn_id(data)                                                          (0x00000400&((data)<<10))
#define VO_DMA_dyn_id_src(data)                                                      ((0x00000400&(data))>>10)
#define VO_DMA_get_dyn_id(data)                                                      ((0x00000400&(data))>>10)
#define VO_DMA_write_en2_shift                                                       (9)
#define VO_DMA_write_en2_mask                                                        (0x00000200)
#define VO_DMA_write_en2(data)                                                       (0x00000200&((data)<<9))
#define VO_DMA_write_en2_src(data)                                                   ((0x00000200&(data))>>9)
#define VO_DMA_get_write_en2(data)                                                   ((0x00000200&(data))>>9)
#define VO_DMA_ref_mix_shift                                                         (8)
#define VO_DMA_ref_mix_mask                                                          (0x00000100)
#define VO_DMA_ref_mix(data)                                                         (0x00000100&((data)<<8))
#define VO_DMA_ref_mix_src(data)                                                     ((0x00000100&(data))>>8)
#define VO_DMA_get_ref_mix(data)                                                     ((0x00000100&(data))>>8)
#define VO_DMA_write_en1_shift                                                       (5)
#define VO_DMA_write_en1_mask                                                        (0x00000020)
#define VO_DMA_write_en1(data)                                                       (0x00000020&((data)<<5))
#define VO_DMA_write_en1_src(data)                                                   ((0x00000020&(data))>>5)
#define VO_DMA_get_write_en1(data)                                                   ((0x00000020&(data))>>5)
#define VO_DMA_l_shift                                                               (0)
#define VO_DMA_l_mask                                                                (0x0000001F)
#define VO_DMA_l(data)                                                               (0x0000001F&((data)<<0))
#define VO_DMA_l_src(data)                                                           ((0x0000001F&(data))>>0)
#define VO_DMA_get_l(data)                                                           ((0x0000001F&(data))>>0)


#define VO_DMA_PV                                                                    0x180056E4
#define VO_DMA_PV_reg_addr                                                           "0xB80056E4"
#define VO_DMA_PV_reg                                                                0xB80056E4
#define set_VO_DMA_PV_reg(data)   (*((volatile unsigned int*) VO_DMA_PV_reg)=data)
#define get_VO_DMA_PV_reg   (*((volatile unsigned int*) VO_DMA_PV_reg))
#define VO_DMA_PV_inst_adr                                                           "0x00B9"
#define VO_DMA_PV_inst                                                               0x00B9
#define VO_DMA_PV_wb_shift                                                           (20)
#define VO_DMA_PV_wb_mask                                                            (0x00F00000)
#define VO_DMA_PV_wb(data)                                                           (0x00F00000&((data)<<20))
#define VO_DMA_PV_wb_src(data)                                                       ((0x00F00000&(data))>>20)
#define VO_DMA_PV_get_wb(data)                                                       ((0x00F00000&(data))>>20)
#define VO_DMA_PV_vbi_shift                                                          (16)
#define VO_DMA_PV_vbi_mask                                                           (0x000F0000)
#define VO_DMA_PV_vbi(data)                                                          (0x000F0000&((data)<<16))
#define VO_DMA_PV_vbi_src(data)                                                      ((0x000F0000&(data))>>16)
#define VO_DMA_PV_get_vbi(data)                                                      ((0x000F0000&(data))>>16)
#define VO_DMA_PV_pic_shift                                                          (12)
#define VO_DMA_PV_pic_mask                                                           (0x0000F000)
#define VO_DMA_PV_pic(data)                                                          (0x0000F000&((data)<<12))
#define VO_DMA_PV_pic_src(data)                                                      ((0x0000F000&(data))>>12)
#define VO_DMA_PV_get_pic(data)                                                      ((0x0000F000&(data))>>12)
#define VO_DMA_PV_dd_shift                                                           (8)
#define VO_DMA_PV_dd_mask                                                            (0x00000F00)
#define VO_DMA_PV_dd(data)                                                           (0x00000F00&((data)<<8))
#define VO_DMA_PV_dd_src(data)                                                       ((0x00000F00&(data))>>8)
#define VO_DMA_PV_get_dd(data)                                                       ((0x00000F00&(data))>>8)
#define VO_DMA_PV_v2_shift                                                           (4)
#define VO_DMA_PV_v2_mask                                                            (0x000000F0)
#define VO_DMA_PV_v2(data)                                                           (0x000000F0&((data)<<4))
#define VO_DMA_PV_v2_src(data)                                                       ((0x000000F0&(data))>>4)
#define VO_DMA_PV_get_v2(data)                                                       ((0x000000F0&(data))>>4)
#define VO_DMA_PV_v1_shift                                                           (0)
#define VO_DMA_PV_v1_mask                                                            (0x0000000F)
#define VO_DMA_PV_v1(data)                                                           (0x0000000F&((data)<<0))
#define VO_DMA_PV_v1_src(data)                                                       ((0x0000000F&(data))>>0)
#define VO_DMA_PV_get_v1(data)                                                       ((0x0000000F&(data))>>0)


#define VO_DMA_PG                                                                    0x180056E8
#define VO_DMA_PG_reg_addr                                                           "0xB80056E8"
#define VO_DMA_PG_reg                                                                0xB80056E8
#define set_VO_DMA_PG_reg(data)   (*((volatile unsigned int*) VO_DMA_PG_reg)=data)
#define get_VO_DMA_PG_reg   (*((volatile unsigned int*) VO_DMA_PG_reg))
#define VO_DMA_PG_inst_adr                                                           "0x00BA"
#define VO_DMA_PG_inst                                                               0x00BA
#define VO_DMA_PG_csr_shift                                                          (12)
#define VO_DMA_PG_csr_mask                                                           (0x0000F000)
#define VO_DMA_PG_csr(data)                                                          (0x0000F000&((data)<<12))
#define VO_DMA_PG_csr_src(data)                                                      ((0x0000F000&(data))>>12)
#define VO_DMA_PG_get_csr(data)                                                      ((0x0000F000&(data))>>12)
#define VO_DMA_PG_osd2_shift                                                         (8)
#define VO_DMA_PG_osd2_mask                                                          (0x00000F00)
#define VO_DMA_PG_osd2(data)                                                         (0x00000F00&((data)<<8))
#define VO_DMA_PG_osd2_src(data)                                                     ((0x00000F00&(data))>>8)
#define VO_DMA_PG_get_osd2(data)                                                     ((0x00000F00&(data))>>8)
#define VO_DMA_PG_osd1_shift                                                         (4)
#define VO_DMA_PG_osd1_mask                                                          (0x000000F0)
#define VO_DMA_PG_osd1(data)                                                         (0x000000F0&((data)<<4))
#define VO_DMA_PG_osd1_src(data)                                                     ((0x000000F0&(data))>>4)
#define VO_DMA_PG_get_osd1(data)                                                     ((0x000000F0&(data))>>4)
#define VO_DMA_PG_sub1_shift                                                         (0)
#define VO_DMA_PG_sub1_mask                                                          (0x0000000F)
#define VO_DMA_PG_sub1(data)                                                         (0x0000000F&((data)<<0))
#define VO_DMA_PG_sub1_src(data)                                                     ((0x0000000F&(data))>>0)
#define VO_DMA_PG_get_sub1(data)                                                     ((0x0000000F&(data))>>0)


#define VO_DMA_BWC                                                                   0x180056EC
#define VO_DMA_BWC_reg_addr                                                          "0xB80056EC"
#define VO_DMA_BWC_reg                                                               0xB80056EC
#define set_VO_DMA_BWC_reg(data)   (*((volatile unsigned int*) VO_DMA_BWC_reg)=data)
#define get_VO_DMA_BWC_reg   (*((volatile unsigned int*) VO_DMA_BWC_reg))
#define VO_DMA_BWC_inst_adr                                                          "0x00BB"
#define VO_DMA_BWC_inst                                                              0x00BB
#define VO_DMA_BWC_write_en3_shift                                                   (28)
#define VO_DMA_BWC_write_en3_mask                                                    (0x10000000)
#define VO_DMA_BWC_write_en3(data)                                                   (0x10000000&((data)<<28))
#define VO_DMA_BWC_write_en3_src(data)                                               ((0x10000000&(data))>>28)
#define VO_DMA_BWC_get_write_en3(data)                                               ((0x10000000&(data))>>28)
#define VO_DMA_BWC_init_shift                                                        (20)
#define VO_DMA_BWC_init_mask                                                         (0x0FF00000)
#define VO_DMA_BWC_init(data)                                                        (0x0FF00000&((data)<<20))
#define VO_DMA_BWC_init_src(data)                                                    ((0x0FF00000&(data))>>20)
#define VO_DMA_BWC_get_init(data)                                                    ((0x0FF00000&(data))>>20)
#define VO_DMA_BWC_write_en2_shift                                                   (16)
#define VO_DMA_BWC_write_en2_mask                                                    (0x00010000)
#define VO_DMA_BWC_write_en2(data)                                                   (0x00010000&((data)<<16))
#define VO_DMA_BWC_write_en2_src(data)                                               ((0x00010000&(data))>>16)
#define VO_DMA_BWC_get_write_en2(data)                                               ((0x00010000&(data))>>16)
#define VO_DMA_BWC_interval_shift                                                    (8)
#define VO_DMA_BWC_interval_mask                                                     (0x0000FF00)
#define VO_DMA_BWC_interval(data)                                                    (0x0000FF00&((data)<<8))
#define VO_DMA_BWC_interval_src(data)                                                ((0x0000FF00&(data))>>8)
#define VO_DMA_BWC_get_interval(data)                                                ((0x0000FF00&(data))>>8)
#define VO_DMA_BWC_write_en1_shift                                                   (5)
#define VO_DMA_BWC_write_en1_mask                                                    (0x00000020)
#define VO_DMA_BWC_write_en1(data)                                                   (0x00000020&((data)<<5))
#define VO_DMA_BWC_write_en1_src(data)                                               ((0x00000020&(data))>>5)
#define VO_DMA_BWC_get_write_en1(data)                                               ((0x00000020&(data))>>5)
#define VO_DMA_BWC_num_shift                                                         (0)
#define VO_DMA_BWC_num_mask                                                          (0x0000001F)
#define VO_DMA_BWC_num(data)                                                         (0x0000001F&((data)<<0))
#define VO_DMA_BWC_num_src(data)                                                     ((0x0000001F&(data))>>0)
#define VO_DMA_BWC_get_num(data)                                                     ((0x0000001F&(data))>>0)


#define VO_DMA_UPDATE                                                                0x180056F0
#define VO_DMA_UPDATE_reg_addr                                                       "0xB80056F0"
#define VO_DMA_UPDATE_reg                                                            0xB80056F0
#define set_VO_DMA_UPDATE_reg(data)   (*((volatile unsigned int*) VO_DMA_UPDATE_reg)=data)
#define get_VO_DMA_UPDATE_reg   (*((volatile unsigned int*) VO_DMA_UPDATE_reg))
#define VO_DMA_UPDATE_inst_adr                                                       "0x00BC"
#define VO_DMA_UPDATE_inst                                                           0x00BC
#define VO_DMA_UPDATE_update_shift                                                   (1)
#define VO_DMA_UPDATE_update_mask                                                    (0x00000002)
#define VO_DMA_UPDATE_update(data)                                                   (0x00000002&((data)<<1))
#define VO_DMA_UPDATE_update_src(data)                                               ((0x00000002&(data))>>1)
#define VO_DMA_UPDATE_get_update(data)                                               ((0x00000002&(data))>>1)
#define VO_DMA_UPDATE_write_data_shift                                               (0)
#define VO_DMA_UPDATE_write_data_mask                                                (0x00000001)
#define VO_DMA_UPDATE_write_data(data)                                               (0x00000001&((data)<<0))
#define VO_DMA_UPDATE_write_data_src(data)                                           ((0x00000001&(data))>>0)
#define VO_DMA_UPDATE_get_write_data(data)                                           ((0x00000001&(data))>>0)


#define VO_DMA_V1_Y0                                                                 0x180056F4
#define VO_DMA_V1_Y0_reg_addr                                                        "0xB80056F4"
#define VO_DMA_V1_Y0_reg                                                             0xB80056F4
#define set_VO_DMA_V1_Y0_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_Y0_reg)=data)
#define get_VO_DMA_V1_Y0_reg   (*((volatile unsigned int*) VO_DMA_V1_Y0_reg))
#define VO_DMA_V1_Y0_inst_adr                                                        "0x00BD"
#define VO_DMA_V1_Y0_inst                                                            0x00BD
#define VO_DMA_V1_Y0_sram_shift                                                      (24)
#define VO_DMA_V1_Y0_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_Y0_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_Y0_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y0_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y0_depth_shift                                                     (12)
#define VO_DMA_V1_Y0_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_Y0_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_Y0_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y0_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y0_sa_shift                                                        (0)
#define VO_DMA_V1_Y0_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_Y0_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_Y0_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_Y0_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_Y1                                                                 0x180056F8
#define VO_DMA_V1_Y1_reg_addr                                                        "0xB80056F8"
#define VO_DMA_V1_Y1_reg                                                             0xB80056F8
#define set_VO_DMA_V1_Y1_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_Y1_reg)=data)
#define get_VO_DMA_V1_Y1_reg   (*((volatile unsigned int*) VO_DMA_V1_Y1_reg))
#define VO_DMA_V1_Y1_inst_adr                                                        "0x00BE"
#define VO_DMA_V1_Y1_inst                                                            0x00BE
#define VO_DMA_V1_Y1_sram_shift                                                      (24)
#define VO_DMA_V1_Y1_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_Y1_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_Y1_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y1_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y1_depth_shift                                                     (12)
#define VO_DMA_V1_Y1_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_Y1_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_Y1_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y1_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y1_sa_shift                                                        (0)
#define VO_DMA_V1_Y1_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_Y1_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_Y1_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_Y1_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_Y2                                                                 0x180056FC
#define VO_DMA_V1_Y2_reg_addr                                                        "0xB80056FC"
#define VO_DMA_V1_Y2_reg                                                             0xB80056FC
#define set_VO_DMA_V1_Y2_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_Y2_reg)=data)
#define get_VO_DMA_V1_Y2_reg   (*((volatile unsigned int*) VO_DMA_V1_Y2_reg))
#define VO_DMA_V1_Y2_inst_adr                                                        "0x00BF"
#define VO_DMA_V1_Y2_inst                                                            0x00BF
#define VO_DMA_V1_Y2_sram_shift                                                      (24)
#define VO_DMA_V1_Y2_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_Y2_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_Y2_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y2_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y2_depth_shift                                                     (12)
#define VO_DMA_V1_Y2_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_Y2_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_Y2_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y2_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y2_sa_shift                                                        (0)
#define VO_DMA_V1_Y2_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_Y2_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_Y2_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_Y2_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_Y3                                                                 0x18005700
#define VO_DMA_V1_Y3_reg_addr                                                        "0xB8005700"
#define VO_DMA_V1_Y3_reg                                                             0xB8005700
#define set_VO_DMA_V1_Y3_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_Y3_reg)=data)
#define get_VO_DMA_V1_Y3_reg   (*((volatile unsigned int*) VO_DMA_V1_Y3_reg))
#define VO_DMA_V1_Y3_inst_adr                                                        "0x00C0"
#define VO_DMA_V1_Y3_inst                                                            0x00C0
#define VO_DMA_V1_Y3_sram_shift                                                      (24)
#define VO_DMA_V1_Y3_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_Y3_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_Y3_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y3_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_Y3_depth_shift                                                     (12)
#define VO_DMA_V1_Y3_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_Y3_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_Y3_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y3_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_Y3_sa_shift                                                        (0)
#define VO_DMA_V1_Y3_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_Y3_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_Y3_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_Y3_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_C0                                                                 0x18005704
#define VO_DMA_V1_C0_reg_addr                                                        "0xB8005704"
#define VO_DMA_V1_C0_reg                                                             0xB8005704
#define set_VO_DMA_V1_C0_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_C0_reg)=data)
#define get_VO_DMA_V1_C0_reg   (*((volatile unsigned int*) VO_DMA_V1_C0_reg))
#define VO_DMA_V1_C0_inst_adr                                                        "0x00C1"
#define VO_DMA_V1_C0_inst                                                            0x00C1
#define VO_DMA_V1_C0_sram_shift                                                      (24)
#define VO_DMA_V1_C0_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_C0_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_C0_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C0_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C0_depth_shift                                                     (12)
#define VO_DMA_V1_C0_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_C0_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_C0_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C0_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C0_sa_shift                                                        (0)
#define VO_DMA_V1_C0_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_C0_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_C0_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_C0_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_C1                                                                 0x18005708
#define VO_DMA_V1_C1_reg_addr                                                        "0xB8005708"
#define VO_DMA_V1_C1_reg                                                             0xB8005708
#define set_VO_DMA_V1_C1_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_C1_reg)=data)
#define get_VO_DMA_V1_C1_reg   (*((volatile unsigned int*) VO_DMA_V1_C1_reg))
#define VO_DMA_V1_C1_inst_adr                                                        "0x00C2"
#define VO_DMA_V1_C1_inst                                                            0x00C2
#define VO_DMA_V1_C1_sram_shift                                                      (24)
#define VO_DMA_V1_C1_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_C1_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_C1_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C1_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C1_depth_shift                                                     (12)
#define VO_DMA_V1_C1_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_C1_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_C1_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C1_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C1_sa_shift                                                        (0)
#define VO_DMA_V1_C1_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_C1_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_C1_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_C1_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_C2                                                                 0x1800570C
#define VO_DMA_V1_C2_reg_addr                                                        "0xB800570C"
#define VO_DMA_V1_C2_reg                                                             0xB800570C
#define set_VO_DMA_V1_C2_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_C2_reg)=data)
#define get_VO_DMA_V1_C2_reg   (*((volatile unsigned int*) VO_DMA_V1_C2_reg))
#define VO_DMA_V1_C2_inst_adr                                                        "0x00C3"
#define VO_DMA_V1_C2_inst                                                            0x00C3
#define VO_DMA_V1_C2_sram_shift                                                      (24)
#define VO_DMA_V1_C2_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_C2_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_C2_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C2_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C2_depth_shift                                                     (12)
#define VO_DMA_V1_C2_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_C2_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_C2_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C2_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C2_sa_shift                                                        (0)
#define VO_DMA_V1_C2_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_C2_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_C2_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_C2_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V1_C3                                                                 0x18005710
#define VO_DMA_V1_C3_reg_addr                                                        "0xB8005710"
#define VO_DMA_V1_C3_reg                                                             0xB8005710
#define set_VO_DMA_V1_C3_reg(data)   (*((volatile unsigned int*) VO_DMA_V1_C3_reg)=data)
#define get_VO_DMA_V1_C3_reg   (*((volatile unsigned int*) VO_DMA_V1_C3_reg))
#define VO_DMA_V1_C3_inst_adr                                                        "0x00C4"
#define VO_DMA_V1_C3_inst                                                            0x00C4
#define VO_DMA_V1_C3_sram_shift                                                      (24)
#define VO_DMA_V1_C3_sram_mask                                                       (0x01000000)
#define VO_DMA_V1_C3_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1_C3_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C3_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1_C3_depth_shift                                                     (12)
#define VO_DMA_V1_C3_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1_C3_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1_C3_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C3_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1_C3_sa_shift                                                        (0)
#define VO_DMA_V1_C3_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1_C3_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1_C3_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1_C3_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V2_Y0                                                                 0x18005714
#define VO_DMA_V2_Y0_reg_addr                                                        "0xB8005714"
#define VO_DMA_V2_Y0_reg                                                             0xB8005714
#define set_VO_DMA_V2_Y0_reg(data)   (*((volatile unsigned int*) VO_DMA_V2_Y0_reg)=data)
#define get_VO_DMA_V2_Y0_reg   (*((volatile unsigned int*) VO_DMA_V2_Y0_reg))
#define VO_DMA_V2_Y0_inst_adr                                                        "0x00C5"
#define VO_DMA_V2_Y0_inst                                                            0x00C5
#define VO_DMA_V2_Y0_sram_shift                                                      (24)
#define VO_DMA_V2_Y0_sram_mask                                                       (0x01000000)
#define VO_DMA_V2_Y0_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V2_Y0_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_Y0_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_Y0_depth_shift                                                     (12)
#define VO_DMA_V2_Y0_depth_mask                                                      (0x003FF000)
#define VO_DMA_V2_Y0_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V2_Y0_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_Y0_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_Y0_sa_shift                                                        (0)
#define VO_DMA_V2_Y0_sa_mask                                                         (0x000003FF)
#define VO_DMA_V2_Y0_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V2_Y0_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V2_Y0_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V2_Y1                                                                 0x18005718
#define VO_DMA_V2_Y1_reg_addr                                                        "0xB8005718"
#define VO_DMA_V2_Y1_reg                                                             0xB8005718
#define set_VO_DMA_V2_Y1_reg(data)   (*((volatile unsigned int*) VO_DMA_V2_Y1_reg)=data)
#define get_VO_DMA_V2_Y1_reg   (*((volatile unsigned int*) VO_DMA_V2_Y1_reg))
#define VO_DMA_V2_Y1_inst_adr                                                        "0x00C6"
#define VO_DMA_V2_Y1_inst                                                            0x00C6
#define VO_DMA_V2_Y1_sram_shift                                                      (24)
#define VO_DMA_V2_Y1_sram_mask                                                       (0x01000000)
#define VO_DMA_V2_Y1_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V2_Y1_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_Y1_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_Y1_depth_shift                                                     (12)
#define VO_DMA_V2_Y1_depth_mask                                                      (0x003FF000)
#define VO_DMA_V2_Y1_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V2_Y1_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_Y1_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_Y1_sa_shift                                                        (0)
#define VO_DMA_V2_Y1_sa_mask                                                         (0x000003FF)
#define VO_DMA_V2_Y1_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V2_Y1_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V2_Y1_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V2_C0                                                                 0x1800571C
#define VO_DMA_V2_C0_reg_addr                                                        "0xB800571C"
#define VO_DMA_V2_C0_reg                                                             0xB800571C
#define set_VO_DMA_V2_C0_reg(data)   (*((volatile unsigned int*) VO_DMA_V2_C0_reg)=data)
#define get_VO_DMA_V2_C0_reg   (*((volatile unsigned int*) VO_DMA_V2_C0_reg))
#define VO_DMA_V2_C0_inst_adr                                                        "0x00C7"
#define VO_DMA_V2_C0_inst                                                            0x00C7
#define VO_DMA_V2_C0_sram_shift                                                      (24)
#define VO_DMA_V2_C0_sram_mask                                                       (0x01000000)
#define VO_DMA_V2_C0_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V2_C0_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_C0_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_C0_depth_shift                                                     (12)
#define VO_DMA_V2_C0_depth_mask                                                      (0x003FF000)
#define VO_DMA_V2_C0_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V2_C0_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_C0_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_C0_sa_shift                                                        (0)
#define VO_DMA_V2_C0_sa_mask                                                         (0x000003FF)
#define VO_DMA_V2_C0_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V2_C0_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V2_C0_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_V2_C1                                                                 0x18005720
#define VO_DMA_V2_C1_reg_addr                                                        "0xB8005720"
#define VO_DMA_V2_C1_reg                                                             0xB8005720
#define set_VO_DMA_V2_C1_reg(data)   (*((volatile unsigned int*) VO_DMA_V2_C1_reg)=data)
#define get_VO_DMA_V2_C1_reg   (*((volatile unsigned int*) VO_DMA_V2_C1_reg))
#define VO_DMA_V2_C1_inst_adr                                                        "0x00C8"
#define VO_DMA_V2_C1_inst                                                            0x00C8
#define VO_DMA_V2_C1_sram_shift                                                      (24)
#define VO_DMA_V2_C1_sram_mask                                                       (0x01000000)
#define VO_DMA_V2_C1_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V2_C1_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_C1_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V2_C1_depth_shift                                                     (12)
#define VO_DMA_V2_C1_depth_mask                                                      (0x003FF000)
#define VO_DMA_V2_C1_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V2_C1_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_C1_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V2_C1_sa_shift                                                        (0)
#define VO_DMA_V2_C1_sa_mask                                                         (0x000003FF)
#define VO_DMA_V2_C1_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V2_C1_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V2_C1_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_SUB1_WI                                                               0x18005724
#define VO_DMA_SUB1_WI_reg_addr                                                      "0xB8005724"
#define VO_DMA_SUB1_WI_reg                                                           0xB8005724
#define set_VO_DMA_SUB1_WI_reg(data)   (*((volatile unsigned int*) VO_DMA_SUB1_WI_reg)=data)
#define get_VO_DMA_SUB1_WI_reg   (*((volatile unsigned int*) VO_DMA_SUB1_WI_reg))
#define VO_DMA_SUB1_WI_inst_adr                                                      "0x00C9"
#define VO_DMA_SUB1_WI_inst                                                          0x00C9
#define VO_DMA_SUB1_WI_sram_shift                                                    (24)
#define VO_DMA_SUB1_WI_sram_mask                                                     (0x01000000)
#define VO_DMA_SUB1_WI_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_SUB1_WI_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_SUB1_WI_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_SUB1_WI_depth_shift                                                   (12)
#define VO_DMA_SUB1_WI_depth_mask                                                    (0x003FF000)
#define VO_DMA_SUB1_WI_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_SUB1_WI_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_SUB1_WI_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_SUB1_WI_sa_shift                                                      (0)
#define VO_DMA_SUB1_WI_sa_mask                                                       (0x000003FF)
#define VO_DMA_SUB1_WI_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_SUB1_WI_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_SUB1_WI_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_SUB1_D0                                                               0x18005728
#define VO_DMA_SUB1_D0_reg_addr                                                      "0xB8005728"
#define VO_DMA_SUB1_D0_reg                                                           0xB8005728
#define set_VO_DMA_SUB1_D0_reg(data)   (*((volatile unsigned int*) VO_DMA_SUB1_D0_reg)=data)
#define get_VO_DMA_SUB1_D0_reg   (*((volatile unsigned int*) VO_DMA_SUB1_D0_reg))
#define VO_DMA_SUB1_D0_inst_adr                                                      "0x00CA"
#define VO_DMA_SUB1_D0_inst                                                          0x00CA
#define VO_DMA_SUB1_D0_sram_shift                                                    (24)
#define VO_DMA_SUB1_D0_sram_mask                                                     (0x01000000)
#define VO_DMA_SUB1_D0_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_SUB1_D0_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_SUB1_D0_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_SUB1_D0_depth_shift                                                   (12)
#define VO_DMA_SUB1_D0_depth_mask                                                    (0x003FF000)
#define VO_DMA_SUB1_D0_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_SUB1_D0_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_SUB1_D0_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_SUB1_D0_sa_shift                                                      (0)
#define VO_DMA_SUB1_D0_sa_mask                                                       (0x000003FF)
#define VO_DMA_SUB1_D0_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_SUB1_D0_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_SUB1_D0_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_SUB1_D1                                                               0x1800572C
#define VO_DMA_SUB1_D1_reg_addr                                                      "0xB800572C"
#define VO_DMA_SUB1_D1_reg                                                           0xB800572C
#define set_VO_DMA_SUB1_D1_reg(data)   (*((volatile unsigned int*) VO_DMA_SUB1_D1_reg)=data)
#define get_VO_DMA_SUB1_D1_reg   (*((volatile unsigned int*) VO_DMA_SUB1_D1_reg))
#define VO_DMA_SUB1_D1_inst_adr                                                      "0x00CB"
#define VO_DMA_SUB1_D1_inst                                                          0x00CB
#define VO_DMA_SUB1_D1_sram_shift                                                    (24)
#define VO_DMA_SUB1_D1_sram_mask                                                     (0x01000000)
#define VO_DMA_SUB1_D1_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_SUB1_D1_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_SUB1_D1_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_SUB1_D1_depth_shift                                                   (12)
#define VO_DMA_SUB1_D1_depth_mask                                                    (0x003FF000)
#define VO_DMA_SUB1_D1_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_SUB1_D1_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_SUB1_D1_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_SUB1_D1_sa_shift                                                      (0)
#define VO_DMA_SUB1_D1_sa_mask                                                       (0x000003FF)
#define VO_DMA_SUB1_D1_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_SUB1_D1_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_SUB1_D1_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_OSD1_WI                                                               0x18005730
#define VO_DMA_OSD1_WI_reg_addr                                                      "0xB8005730"
#define VO_DMA_OSD1_WI_reg                                                           0xB8005730
#define set_VO_DMA_OSD1_WI_reg(data)   (*((volatile unsigned int*) VO_DMA_OSD1_WI_reg)=data)
#define get_VO_DMA_OSD1_WI_reg   (*((volatile unsigned int*) VO_DMA_OSD1_WI_reg))
#define VO_DMA_OSD1_WI_inst_adr                                                      "0x00CC"
#define VO_DMA_OSD1_WI_inst                                                          0x00CC
#define VO_DMA_OSD1_WI_sram_shift                                                    (24)
#define VO_DMA_OSD1_WI_sram_mask                                                     (0x01000000)
#define VO_DMA_OSD1_WI_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_OSD1_WI_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD1_WI_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD1_WI_depth_shift                                                   (12)
#define VO_DMA_OSD1_WI_depth_mask                                                    (0x003FF000)
#define VO_DMA_OSD1_WI_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_OSD1_WI_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD1_WI_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD1_WI_sa_shift                                                      (0)
#define VO_DMA_OSD1_WI_sa_mask                                                       (0x000003FF)
#define VO_DMA_OSD1_WI_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_OSD1_WI_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_OSD1_WI_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_OSD1_D0                                                               0x18005734
#define VO_DMA_OSD1_D0_reg_addr                                                      "0xB8005734"
#define VO_DMA_OSD1_D0_reg                                                           0xB8005734
#define set_VO_DMA_OSD1_D0_reg(data)   (*((volatile unsigned int*) VO_DMA_OSD1_D0_reg)=data)
#define get_VO_DMA_OSD1_D0_reg   (*((volatile unsigned int*) VO_DMA_OSD1_D0_reg))
#define VO_DMA_OSD1_D0_inst_adr                                                      "0x00CD"
#define VO_DMA_OSD1_D0_inst                                                          0x00CD
#define VO_DMA_OSD1_D0_sram_shift                                                    (24)
#define VO_DMA_OSD1_D0_sram_mask                                                     (0x01000000)
#define VO_DMA_OSD1_D0_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_OSD1_D0_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD1_D0_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD1_D0_depth_shift                                                   (12)
#define VO_DMA_OSD1_D0_depth_mask                                                    (0x003FF000)
#define VO_DMA_OSD1_D0_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_OSD1_D0_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD1_D0_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD1_D0_sa_shift                                                      (0)
#define VO_DMA_OSD1_D0_sa_mask                                                       (0x000003FF)
#define VO_DMA_OSD1_D0_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_OSD1_D0_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_OSD1_D0_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_OSD1_D1                                                               0x18005738
#define VO_DMA_OSD1_D1_reg_addr                                                      "0xB8005738"
#define VO_DMA_OSD1_D1_reg                                                           0xB8005738
#define set_VO_DMA_OSD1_D1_reg(data)   (*((volatile unsigned int*) VO_DMA_OSD1_D1_reg)=data)
#define get_VO_DMA_OSD1_D1_reg   (*((volatile unsigned int*) VO_DMA_OSD1_D1_reg))
#define VO_DMA_OSD1_D1_inst_adr                                                      "0x00CE"
#define VO_DMA_OSD1_D1_inst                                                          0x00CE
#define VO_DMA_OSD1_D1_sram_shift                                                    (24)
#define VO_DMA_OSD1_D1_sram_mask                                                     (0x01000000)
#define VO_DMA_OSD1_D1_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_OSD1_D1_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD1_D1_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD1_D1_depth_shift                                                   (12)
#define VO_DMA_OSD1_D1_depth_mask                                                    (0x003FF000)
#define VO_DMA_OSD1_D1_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_OSD1_D1_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD1_D1_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD1_D1_sa_shift                                                      (0)
#define VO_DMA_OSD1_D1_sa_mask                                                       (0x000003FF)
#define VO_DMA_OSD1_D1_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_OSD1_D1_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_OSD1_D1_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_OSD2_WI                                                               0x1800573C
#define VO_DMA_OSD2_WI_reg_addr                                                      "0xB800573C"
#define VO_DMA_OSD2_WI_reg                                                           0xB800573C
#define set_VO_DMA_OSD2_WI_reg(data)   (*((volatile unsigned int*) VO_DMA_OSD2_WI_reg)=data)
#define get_VO_DMA_OSD2_WI_reg   (*((volatile unsigned int*) VO_DMA_OSD2_WI_reg))
#define VO_DMA_OSD2_WI_inst_adr                                                      "0x00CF"
#define VO_DMA_OSD2_WI_inst                                                          0x00CF
#define VO_DMA_OSD2_WI_sram_shift                                                    (24)
#define VO_DMA_OSD2_WI_sram_mask                                                     (0x01000000)
#define VO_DMA_OSD2_WI_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_OSD2_WI_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD2_WI_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD2_WI_depth_shift                                                   (12)
#define VO_DMA_OSD2_WI_depth_mask                                                    (0x003FF000)
#define VO_DMA_OSD2_WI_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_OSD2_WI_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD2_WI_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD2_WI_sa_shift                                                      (0)
#define VO_DMA_OSD2_WI_sa_mask                                                       (0x000003FF)
#define VO_DMA_OSD2_WI_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_OSD2_WI_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_OSD2_WI_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_OSD2_D0                                                               0x18005740
#define VO_DMA_OSD2_D0_reg_addr                                                      "0xB8005740"
#define VO_DMA_OSD2_D0_reg                                                           0xB8005740
#define set_VO_DMA_OSD2_D0_reg(data)   (*((volatile unsigned int*) VO_DMA_OSD2_D0_reg)=data)
#define get_VO_DMA_OSD2_D0_reg   (*((volatile unsigned int*) VO_DMA_OSD2_D0_reg))
#define VO_DMA_OSD2_D0_inst_adr                                                      "0x00D0"
#define VO_DMA_OSD2_D0_inst                                                          0x00D0
#define VO_DMA_OSD2_D0_sram_shift                                                    (24)
#define VO_DMA_OSD2_D0_sram_mask                                                     (0x01000000)
#define VO_DMA_OSD2_D0_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_OSD2_D0_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD2_D0_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD2_D0_depth_shift                                                   (12)
#define VO_DMA_OSD2_D0_depth_mask                                                    (0x003FF000)
#define VO_DMA_OSD2_D0_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_OSD2_D0_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD2_D0_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD2_D0_sa_shift                                                      (0)
#define VO_DMA_OSD2_D0_sa_mask                                                       (0x000003FF)
#define VO_DMA_OSD2_D0_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_OSD2_D0_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_OSD2_D0_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_OSD2_D1                                                               0x18005744
#define VO_DMA_OSD2_D1_reg_addr                                                      "0xB8005744"
#define VO_DMA_OSD2_D1_reg                                                           0xB8005744
#define set_VO_DMA_OSD2_D1_reg(data)   (*((volatile unsigned int*) VO_DMA_OSD2_D1_reg)=data)
#define get_VO_DMA_OSD2_D1_reg   (*((volatile unsigned int*) VO_DMA_OSD2_D1_reg))
#define VO_DMA_OSD2_D1_inst_adr                                                      "0x00D1"
#define VO_DMA_OSD2_D1_inst                                                          0x00D1
#define VO_DMA_OSD2_D1_sram_shift                                                    (24)
#define VO_DMA_OSD2_D1_sram_mask                                                     (0x01000000)
#define VO_DMA_OSD2_D1_sram(data)                                                    (0x01000000&((data)<<24))
#define VO_DMA_OSD2_D1_sram_src(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD2_D1_get_sram(data)                                                ((0x01000000&(data))>>24)
#define VO_DMA_OSD2_D1_depth_shift                                                   (12)
#define VO_DMA_OSD2_D1_depth_mask                                                    (0x003FF000)
#define VO_DMA_OSD2_D1_depth(data)                                                   (0x003FF000&((data)<<12))
#define VO_DMA_OSD2_D1_depth_src(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD2_D1_get_depth(data)                                               ((0x003FF000&(data))>>12)
#define VO_DMA_OSD2_D1_sa_shift                                                      (0)
#define VO_DMA_OSD2_D1_sa_mask                                                       (0x000003FF)
#define VO_DMA_OSD2_D1_sa(data)                                                      (0x000003FF&((data)<<0))
#define VO_DMA_OSD2_D1_sa_src(data)                                                  ((0x000003FF&(data))>>0)
#define VO_DMA_OSD2_D1_get_sa(data)                                                  ((0x000003FF&(data))>>0)


#define VO_DMA_CSR                                                                   0x18005748
#define VO_DMA_CSR_reg_addr                                                          "0xB8005748"
#define VO_DMA_CSR_reg                                                               0xB8005748
#define set_VO_DMA_CSR_reg(data)   (*((volatile unsigned int*) VO_DMA_CSR_reg)=data)
#define get_VO_DMA_CSR_reg   (*((volatile unsigned int*) VO_DMA_CSR_reg))
#define VO_DMA_CSR_inst_adr                                                          "0x00D2"
#define VO_DMA_CSR_inst                                                              0x00D2
#define VO_DMA_CSR_sram_shift                                                        (24)
#define VO_DMA_CSR_sram_mask                                                         (0x01000000)
#define VO_DMA_CSR_sram(data)                                                        (0x01000000&((data)<<24))
#define VO_DMA_CSR_sram_src(data)                                                    ((0x01000000&(data))>>24)
#define VO_DMA_CSR_get_sram(data)                                                    ((0x01000000&(data))>>24)
#define VO_DMA_CSR_depth_shift                                                       (12)
#define VO_DMA_CSR_depth_mask                                                        (0x003FF000)
#define VO_DMA_CSR_depth(data)                                                       (0x003FF000&((data)<<12))
#define VO_DMA_CSR_depth_src(data)                                                   ((0x003FF000&(data))>>12)
#define VO_DMA_CSR_get_depth(data)                                                   ((0x003FF000&(data))>>12)
#define VO_DMA_CSR_sa_shift                                                          (0)
#define VO_DMA_CSR_sa_mask                                                           (0x000003FF)
#define VO_DMA_CSR_sa(data)                                                          (0x000003FF&((data)<<0))
#define VO_DMA_CSR_sa_src(data)                                                      ((0x000003FF&(data))>>0)
#define VO_DMA_CSR_get_sa(data)                                                      ((0x000003FF&(data))>>0)


#define VO_DMA_PIC_Y                                                                 0x1800574C
#define VO_DMA_PIC_Y_reg_addr                                                        "0xB800574C"
#define VO_DMA_PIC_Y_reg                                                             0xB800574C
#define set_VO_DMA_PIC_Y_reg(data)   (*((volatile unsigned int*) VO_DMA_PIC_Y_reg)=data)
#define get_VO_DMA_PIC_Y_reg   (*((volatile unsigned int*) VO_DMA_PIC_Y_reg))
#define VO_DMA_PIC_Y_inst_adr                                                        "0x00D3"
#define VO_DMA_PIC_Y_inst                                                            0x00D3
#define VO_DMA_PIC_Y_sram_shift                                                      (24)
#define VO_DMA_PIC_Y_sram_mask                                                       (0x01000000)
#define VO_DMA_PIC_Y_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_PIC_Y_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_PIC_Y_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_PIC_Y_depth_shift                                                     (12)
#define VO_DMA_PIC_Y_depth_mask                                                      (0x003FF000)
#define VO_DMA_PIC_Y_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_PIC_Y_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_PIC_Y_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_PIC_Y_sa_shift                                                        (0)
#define VO_DMA_PIC_Y_sa_mask                                                         (0x000003FF)
#define VO_DMA_PIC_Y_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_PIC_Y_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_PIC_Y_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_PIC_C                                                                 0x18005750
#define VO_DMA_PIC_C_reg_addr                                                        "0xB8005750"
#define VO_DMA_PIC_C_reg                                                             0xB8005750
#define set_VO_DMA_PIC_C_reg(data)   (*((volatile unsigned int*) VO_DMA_PIC_C_reg)=data)
#define get_VO_DMA_PIC_C_reg   (*((volatile unsigned int*) VO_DMA_PIC_C_reg))
#define VO_DMA_PIC_C_inst_adr                                                        "0x00D4"
#define VO_DMA_PIC_C_inst                                                            0x00D4
#define VO_DMA_PIC_C_sram_shift                                                      (24)
#define VO_DMA_PIC_C_sram_mask                                                       (0x01000000)
#define VO_DMA_PIC_C_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_PIC_C_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_PIC_C_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_PIC_C_depth_shift                                                     (12)
#define VO_DMA_PIC_C_depth_mask                                                      (0x003FF000)
#define VO_DMA_PIC_C_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_PIC_C_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_PIC_C_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_PIC_C_sa_shift                                                        (0)
#define VO_DMA_PIC_C_sa_mask                                                         (0x000003FF)
#define VO_DMA_PIC_C_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_PIC_C_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_PIC_C_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_DD_Y                                                                  0x18005754
#define VO_DMA_DD_Y_reg_addr                                                         "0xB8005754"
#define VO_DMA_DD_Y_reg                                                              0xB8005754
#define set_VO_DMA_DD_Y_reg(data)   (*((volatile unsigned int*) VO_DMA_DD_Y_reg)=data)
#define get_VO_DMA_DD_Y_reg   (*((volatile unsigned int*) VO_DMA_DD_Y_reg))
#define VO_DMA_DD_Y_inst_adr                                                         "0x00D5"
#define VO_DMA_DD_Y_inst                                                             0x00D5
#define VO_DMA_DD_Y_sram_shift                                                       (24)
#define VO_DMA_DD_Y_sram_mask                                                        (0x01000000)
#define VO_DMA_DD_Y_sram(data)                                                       (0x01000000&((data)<<24))
#define VO_DMA_DD_Y_sram_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_DD_Y_get_sram(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_DD_Y_depth_shift                                                      (12)
#define VO_DMA_DD_Y_depth_mask                                                       (0x003FF000)
#define VO_DMA_DD_Y_depth(data)                                                      (0x003FF000&((data)<<12))
#define VO_DMA_DD_Y_depth_src(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_DD_Y_get_depth(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_DD_Y_sa_shift                                                         (0)
#define VO_DMA_DD_Y_sa_mask                                                          (0x000003FF)
#define VO_DMA_DD_Y_sa(data)                                                         (0x000003FF&((data)<<0))
#define VO_DMA_DD_Y_sa_src(data)                                                     ((0x000003FF&(data))>>0)
#define VO_DMA_DD_Y_get_sa(data)                                                     ((0x000003FF&(data))>>0)


#define VO_DMA_DD_C                                                                  0x18005758
#define VO_DMA_DD_C_reg_addr                                                         "0xB8005758"
#define VO_DMA_DD_C_reg                                                              0xB8005758
#define set_VO_DMA_DD_C_reg(data)   (*((volatile unsigned int*) VO_DMA_DD_C_reg)=data)
#define get_VO_DMA_DD_C_reg   (*((volatile unsigned int*) VO_DMA_DD_C_reg))
#define VO_DMA_DD_C_inst_adr                                                         "0x00D6"
#define VO_DMA_DD_C_inst                                                             0x00D6
#define VO_DMA_DD_C_sram_shift                                                       (24)
#define VO_DMA_DD_C_sram_mask                                                        (0x01000000)
#define VO_DMA_DD_C_sram(data)                                                       (0x01000000&((data)<<24))
#define VO_DMA_DD_C_sram_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_DD_C_get_sram(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_DD_C_depth_shift                                                      (12)
#define VO_DMA_DD_C_depth_mask                                                       (0x003FF000)
#define VO_DMA_DD_C_depth(data)                                                      (0x003FF000&((data)<<12))
#define VO_DMA_DD_C_depth_src(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_DD_C_get_depth(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_DD_C_sa_shift                                                         (0)
#define VO_DMA_DD_C_sa_mask                                                          (0x000003FF)
#define VO_DMA_DD_C_sa(data)                                                         (0x000003FF&((data)<<0))
#define VO_DMA_DD_C_sa_src(data)                                                     ((0x000003FF&(data))>>0)
#define VO_DMA_DD_C_get_sa(data)                                                     ((0x000003FF&(data))>>0)


#define VO_DMA_V1DI                                                                  0x1800575C
#define VO_DMA_V1DI_reg_addr                                                         "0xB800575C"
#define VO_DMA_V1DI_reg                                                              0xB800575C
#define set_VO_DMA_V1DI_reg(data)   (*((volatile unsigned int*) VO_DMA_V1DI_reg)=data)
#define get_VO_DMA_V1DI_reg   (*((volatile unsigned int*) VO_DMA_V1DI_reg))
#define VO_DMA_V1DI_inst_adr                                                         "0x00D7"
#define VO_DMA_V1DI_inst                                                             0x00D7
#define VO_DMA_V1DI_sram_shift                                                       (24)
#define VO_DMA_V1DI_sram_mask                                                        (0x01000000)
#define VO_DMA_V1DI_sram(data)                                                       (0x01000000&((data)<<24))
#define VO_DMA_V1DI_sram_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_V1DI_get_sram(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_V1DI_depth_shift                                                      (12)
#define VO_DMA_V1DI_depth_mask                                                       (0x003FF000)
#define VO_DMA_V1DI_depth(data)                                                      (0x003FF000&((data)<<12))
#define VO_DMA_V1DI_depth_src(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_V1DI_get_depth(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_V1DI_sa_shift                                                         (0)
#define VO_DMA_V1DI_sa_mask                                                          (0x000003FF)
#define VO_DMA_V1DI_sa(data)                                                         (0x000003FF&((data)<<0))
#define VO_DMA_V1DI_sa_src(data)                                                     ((0x000003FF&(data))>>0)
#define VO_DMA_V1DI_get_sa(data)                                                     ((0x000003FF&(data))>>0)


#define VO_DMA_V1MV                                                                  0x18005760
#define VO_DMA_V1MV_reg_addr                                                         "0xB8005760"
#define VO_DMA_V1MV_reg                                                              0xB8005760
#define set_VO_DMA_V1MV_reg(data)   (*((volatile unsigned int*) VO_DMA_V1MV_reg)=data)
#define get_VO_DMA_V1MV_reg   (*((volatile unsigned int*) VO_DMA_V1MV_reg))
#define VO_DMA_V1MV_inst_adr                                                         "0x00D8"
#define VO_DMA_V1MV_inst                                                             0x00D8
#define VO_DMA_V1MV_sram_shift                                                       (24)
#define VO_DMA_V1MV_sram_mask                                                        (0x01000000)
#define VO_DMA_V1MV_sram(data)                                                       (0x01000000&((data)<<24))
#define VO_DMA_V1MV_sram_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_V1MV_get_sram(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_V1MV_depth_shift                                                      (12)
#define VO_DMA_V1MV_depth_mask                                                       (0x003FF000)
#define VO_DMA_V1MV_depth(data)                                                      (0x003FF000&((data)<<12))
#define VO_DMA_V1MV_depth_src(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_V1MV_get_depth(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_V1MV_sa_shift                                                         (0)
#define VO_DMA_V1MV_sa_mask                                                          (0x000003FF)
#define VO_DMA_V1MV_sa(data)                                                         (0x000003FF&((data)<<0))
#define VO_DMA_V1MV_sa_src(data)                                                     ((0x000003FF&(data))>>0)
#define VO_DMA_V1MV_get_sa(data)                                                     ((0x000003FF&(data))>>0)


#define VO_DMA_V1SAD                                                                 0x18005764
#define VO_DMA_V1SAD_reg_addr                                                        "0xB8005764"
#define VO_DMA_V1SAD_reg                                                             0xB8005764
#define set_VO_DMA_V1SAD_reg(data)   (*((volatile unsigned int*) VO_DMA_V1SAD_reg)=data)
#define get_VO_DMA_V1SAD_reg   (*((volatile unsigned int*) VO_DMA_V1SAD_reg))
#define VO_DMA_V1SAD_inst_adr                                                        "0x00D9"
#define VO_DMA_V1SAD_inst                                                            0x00D9
#define VO_DMA_V1SAD_sram_shift                                                      (24)
#define VO_DMA_V1SAD_sram_mask                                                       (0x01000000)
#define VO_DMA_V1SAD_sram(data)                                                      (0x01000000&((data)<<24))
#define VO_DMA_V1SAD_sram_src(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1SAD_get_sram(data)                                                  ((0x01000000&(data))>>24)
#define VO_DMA_V1SAD_depth_shift                                                     (12)
#define VO_DMA_V1SAD_depth_mask                                                      (0x003FF000)
#define VO_DMA_V1SAD_depth(data)                                                     (0x003FF000&((data)<<12))
#define VO_DMA_V1SAD_depth_src(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1SAD_get_depth(data)                                                 ((0x003FF000&(data))>>12)
#define VO_DMA_V1SAD_sa_shift                                                        (0)
#define VO_DMA_V1SAD_sa_mask                                                         (0x000003FF)
#define VO_DMA_V1SAD_sa(data)                                                        (0x000003FF&((data)<<0))
#define VO_DMA_V1SAD_sa_src(data)                                                    ((0x000003FF&(data))>>0)
#define VO_DMA_V1SAD_get_sa(data)                                                    ((0x000003FF&(data))>>0)


#define VO_DMA_VBI1                                                                  0x18005768
#define VO_DMA_VBI1_reg_addr                                                         "0xB8005768"
#define VO_DMA_VBI1_reg                                                              0xB8005768
#define set_VO_DMA_VBI1_reg(data)   (*((volatile unsigned int*) VO_DMA_VBI1_reg)=data)
#define get_VO_DMA_VBI1_reg   (*((volatile unsigned int*) VO_DMA_VBI1_reg))
#define VO_DMA_VBI1_inst_adr                                                         "0x00DA"
#define VO_DMA_VBI1_inst                                                             0x00DA
#define VO_DMA_VBI1_sram_shift                                                       (24)
#define VO_DMA_VBI1_sram_mask                                                        (0x01000000)
#define VO_DMA_VBI1_sram(data)                                                       (0x01000000&((data)<<24))
#define VO_DMA_VBI1_sram_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_VBI1_get_sram(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_VBI1_depth_shift                                                      (12)
#define VO_DMA_VBI1_depth_mask                                                       (0x003FF000)
#define VO_DMA_VBI1_depth(data)                                                      (0x003FF000&((data)<<12))
#define VO_DMA_VBI1_depth_src(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_VBI1_get_depth(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_VBI1_sa_shift                                                         (0)
#define VO_DMA_VBI1_sa_mask                                                          (0x000003FF)
#define VO_DMA_VBI1_sa(data)                                                         (0x000003FF&((data)<<0))
#define VO_DMA_VBI1_sa_src(data)                                                     ((0x000003FF&(data))>>0)
#define VO_DMA_VBI1_get_sa(data)                                                     ((0x000003FF&(data))>>0)


#define VO_DMA_VBI2                                                                  0x1800576C
#define VO_DMA_VBI2_reg_addr                                                         "0xB800576C"
#define VO_DMA_VBI2_reg                                                              0xB800576C
#define set_VO_DMA_VBI2_reg(data)   (*((volatile unsigned int*) VO_DMA_VBI2_reg)=data)
#define get_VO_DMA_VBI2_reg   (*((volatile unsigned int*) VO_DMA_VBI2_reg))
#define VO_DMA_VBI2_inst_adr                                                         "0x00DB"
#define VO_DMA_VBI2_inst                                                             0x00DB
#define VO_DMA_VBI2_sram_shift                                                       (24)
#define VO_DMA_VBI2_sram_mask                                                        (0x01000000)
#define VO_DMA_VBI2_sram(data)                                                       (0x01000000&((data)<<24))
#define VO_DMA_VBI2_sram_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_VBI2_get_sram(data)                                                   ((0x01000000&(data))>>24)
#define VO_DMA_VBI2_depth_shift                                                      (12)
#define VO_DMA_VBI2_depth_mask                                                       (0x003FF000)
#define VO_DMA_VBI2_depth(data)                                                      (0x003FF000&((data)<<12))
#define VO_DMA_VBI2_depth_src(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_VBI2_get_depth(data)                                                  ((0x003FF000&(data))>>12)
#define VO_DMA_VBI2_sa_shift                                                         (0)
#define VO_DMA_VBI2_sa_mask                                                          (0x000003FF)
#define VO_DMA_VBI2_sa(data)                                                         (0x000003FF&((data)<<0))
#define VO_DMA_VBI2_sa_src(data)                                                     ((0x000003FF&(data))>>0)
#define VO_DMA_VBI2_get_sa(data)                                                     ((0x000003FF&(data))>>0)


#define VO_DMA_WB_Y                                                                  0x18005770
#define VO_DMA_WB_Y_reg_addr                                                         "0xB8005770"
#define VO_DMA_WB_Y_reg                                                              0xB8005770
#define set_VO_DMA_WB_Y_reg(data)   (*((volatile unsigned int*) VO_DMA_WB_Y_reg)=data)
#define get_VO_DMA_WB_Y_reg   (*((volatile unsigned int*) VO_DMA_WB_Y_reg))
#define VO_DMA_WB_Y_inst_adr                                                         "0x00DC"
#define VO_DMA_WB_Y_inst                                                             0x00DC
#define VO_DMA_WB_Y_depth_shift                                                      (12)
#define VO_DMA_WB_Y_depth_mask                                                       (0x0007F000)
#define VO_DMA_WB_Y_depth(data)                                                      (0x0007F000&((data)<<12))
#define VO_DMA_WB_Y_depth_src(data)                                                  ((0x0007F000&(data))>>12)
#define VO_DMA_WB_Y_get_depth(data)                                                  ((0x0007F000&(data))>>12)
#define VO_DMA_WB_Y_sa_shift                                                         (0)
#define VO_DMA_WB_Y_sa_mask                                                          (0x0000007F)
#define VO_DMA_WB_Y_sa(data)                                                         (0x0000007F&((data)<<0))
#define VO_DMA_WB_Y_sa_src(data)                                                     ((0x0000007F&(data))>>0)
#define VO_DMA_WB_Y_get_sa(data)                                                     ((0x0000007F&(data))>>0)


#define VO_DMA_WB_C                                                                  0x18005774
#define VO_DMA_WB_C_reg_addr                                                         "0xB8005774"
#define VO_DMA_WB_C_reg                                                              0xB8005774
#define set_VO_DMA_WB_C_reg(data)   (*((volatile unsigned int*) VO_DMA_WB_C_reg)=data)
#define get_VO_DMA_WB_C_reg   (*((volatile unsigned int*) VO_DMA_WB_C_reg))
#define VO_DMA_WB_C_inst_adr                                                         "0x00DD"
#define VO_DMA_WB_C_inst                                                             0x00DD
#define VO_DMA_WB_C_depth_shift                                                      (12)
#define VO_DMA_WB_C_depth_mask                                                       (0x0007F000)
#define VO_DMA_WB_C_depth(data)                                                      (0x0007F000&((data)<<12))
#define VO_DMA_WB_C_depth_src(data)                                                  ((0x0007F000&(data))>>12)
#define VO_DMA_WB_C_get_depth(data)                                                  ((0x0007F000&(data))>>12)
#define VO_DMA_WB_C_sa_shift                                                         (0)
#define VO_DMA_WB_C_sa_mask                                                          (0x0000007F)
#define VO_DMA_WB_C_sa(data)                                                         (0x0000007F&((data)<<0))
#define VO_DMA_WB_C_sa_src(data)                                                     ((0x0000007F&(data))>>0)
#define VO_DMA_WB_C_get_sa(data)                                                     ((0x0000007F&(data))>>0)


#define VO_DMA_WB_V1                                                                 0x18005778
#define VO_DMA_WB_V1_reg_addr                                                        "0xB8005778"
#define VO_DMA_WB_V1_reg                                                             0xB8005778
#define set_VO_DMA_WB_V1_reg(data)   (*((volatile unsigned int*) VO_DMA_WB_V1_reg)=data)
#define get_VO_DMA_WB_V1_reg   (*((volatile unsigned int*) VO_DMA_WB_V1_reg))
#define VO_DMA_WB_V1_inst_adr                                                        "0x00DE"
#define VO_DMA_WB_V1_inst                                                            0x00DE
#define VO_DMA_WB_V1_depth_shift                                                     (12)
#define VO_DMA_WB_V1_depth_mask                                                      (0x0007F000)
#define VO_DMA_WB_V1_depth(data)                                                     (0x0007F000&((data)<<12))
#define VO_DMA_WB_V1_depth_src(data)                                                 ((0x0007F000&(data))>>12)
#define VO_DMA_WB_V1_get_depth(data)                                                 ((0x0007F000&(data))>>12)
#define VO_DMA_WB_V1_sa_shift                                                        (0)
#define VO_DMA_WB_V1_sa_mask                                                         (0x0000007F)
#define VO_DMA_WB_V1_sa(data)                                                        (0x0000007F&((data)<<0))
#define VO_DMA_WB_V1_sa_src(data)                                                    ((0x0000007F&(data))>>0)
#define VO_DMA_WB_V1_get_sa(data)                                                    ((0x0000007F&(data))>>0)


#define VO_MIX_ORDER                                                                 0x1800577C
#define VO_MIX_ORDER_reg_addr                                                        "0xB800577C"
#define VO_MIX_ORDER_reg                                                             0xB800577C
#define set_VO_MIX_ORDER_reg(data)   (*((volatile unsigned int*) VO_MIX_ORDER_reg)=data)
#define get_VO_MIX_ORDER_reg   (*((volatile unsigned int*) VO_MIX_ORDER_reg))
#define VO_MIX_ORDER_inst_adr                                                        "0x00DF"
#define VO_MIX_ORDER_inst                                                            0x00DF
#define VO_MIX_ORDER_csr_shift                                                       (28)
#define VO_MIX_ORDER_csr_mask                                                        (0xF0000000)
#define VO_MIX_ORDER_csr(data)                                                       (0xF0000000&((data)<<28))
#define VO_MIX_ORDER_csr_src(data)                                                   ((0xF0000000&(data))>>28)
#define VO_MIX_ORDER_get_csr(data)                                                   ((0xF0000000&(data))>>28)
#define VO_MIX_ORDER_osd2_shift                                                      (24)
#define VO_MIX_ORDER_osd2_mask                                                       (0x0F000000)
#define VO_MIX_ORDER_osd2(data)                                                      (0x0F000000&((data)<<24))
#define VO_MIX_ORDER_osd2_src(data)                                                  ((0x0F000000&(data))>>24)
#define VO_MIX_ORDER_get_osd2(data)                                                  ((0x0F000000&(data))>>24)
#define VO_MIX_ORDER_osd1_shift                                                      (20)
#define VO_MIX_ORDER_osd1_mask                                                       (0x00F00000)
#define VO_MIX_ORDER_osd1(data)                                                      (0x00F00000&((data)<<20))
#define VO_MIX_ORDER_osd1_src(data)                                                  ((0x00F00000&(data))>>20)
#define VO_MIX_ORDER_get_osd1(data)                                                  ((0x00F00000&(data))>>20)
#define VO_MIX_ORDER_v2_shift                                                        (16)
#define VO_MIX_ORDER_v2_mask                                                         (0x000F0000)
#define VO_MIX_ORDER_v2(data)                                                        (0x000F0000&((data)<<16))
#define VO_MIX_ORDER_v2_src(data)                                                    ((0x000F0000&(data))>>16)
#define VO_MIX_ORDER_get_v2(data)                                                    ((0x000F0000&(data))>>16)
#define VO_MIX_ORDER_sub1_shift                                                      (12)
#define VO_MIX_ORDER_sub1_mask                                                       (0x0000F000)
#define VO_MIX_ORDER_sub1(data)                                                      (0x0000F000&((data)<<12))
#define VO_MIX_ORDER_sub1_src(data)                                                  ((0x0000F000&(data))>>12)
#define VO_MIX_ORDER_get_sub1(data)                                                  ((0x0000F000&(data))>>12)
#define VO_MIX_ORDER_v1_shift                                                        (8)
#define VO_MIX_ORDER_v1_mask                                                         (0x00000F00)
#define VO_MIX_ORDER_v1(data)                                                        (0x00000F00&((data)<<8))
#define VO_MIX_ORDER_v1_src(data)                                                    ((0x00000F00&(data))>>8)
#define VO_MIX_ORDER_get_v1(data)                                                    ((0x00000F00&(data))>>8)
#define VO_MIX_ORDER_pic_shift                                                       (0)
#define VO_MIX_ORDER_pic_mask                                                        (0x0000000F)
#define VO_MIX_ORDER_pic(data)                                                       (0x0000000F&((data)<<0))
#define VO_MIX_ORDER_pic_src(data)                                                   ((0x0000000F&(data))>>0)
#define VO_MIX_ORDER_get_pic(data)                                                   ((0x0000000F&(data))>>0)


#define VO_MIX                                                                       0x18005780
#define VO_MIX_reg_addr                                                              "0xB8005780"
#define VO_MIX_reg                                                                   0xB8005780
#define set_VO_MIX_reg(data)   (*((volatile unsigned int*) VO_MIX_reg)=data)
#define get_VO_MIX_reg   (*((volatile unsigned int*) VO_MIX_reg))
#define VO_MIX_inst_adr                                                              "0x00E0"
#define VO_MIX_inst                                                                  0x00E0
#define VO_MIX_cd_shift                                                              (5)
#define VO_MIX_cd_mask                                                               (0x00000020)
#define VO_MIX_cd(data)                                                              (0x00000020&((data)<<5))
#define VO_MIX_cd_src(data)                                                          ((0x00000020&(data))>>5)
#define VO_MIX_get_cd(data)                                                          ((0x00000020&(data))>>5)
#define VO_MIX_cu_shift                                                              (4)
#define VO_MIX_cu_mask                                                               (0x00000010)
#define VO_MIX_cu(data)                                                              (0x00000010&((data)<<4))
#define VO_MIX_cu_src(data)                                                          ((0x00000010&(data))>>4)
#define VO_MIX_get_cu(data)                                                          ((0x00000010&(data))>>4)
#define VO_MIX_v2b_shift                                                             (3)
#define VO_MIX_v2b_mask                                                              (0x00000008)
#define VO_MIX_v2b(data)                                                             (0x00000008&((data)<<3))
#define VO_MIX_v2b_src(data)                                                         ((0x00000008&(data))>>3)
#define VO_MIX_get_v2b(data)                                                         ((0x00000008&(data))>>3)
#define VO_MIX_v2bg_shift                                                            (2)
#define VO_MIX_v2bg_mask                                                             (0x00000004)
#define VO_MIX_v2bg(data)                                                            (0x00000004&((data)<<2))
#define VO_MIX_v2bg_src(data)                                                        ((0x00000004&(data))>>2)
#define VO_MIX_get_v2bg(data)                                                        ((0x00000004&(data))>>2)
#define VO_MIX_v1b_shift                                                             (1)
#define VO_MIX_v1b_mask                                                              (0x00000002)
#define VO_MIX_v1b(data)                                                             (0x00000002&((data)<<1))
#define VO_MIX_v1b_src(data)                                                         ((0x00000002&(data))>>1)
#define VO_MIX_get_v1b(data)                                                         ((0x00000002&(data))>>1)
#define VO_MIX_write_data_shift                                                      (0)
#define VO_MIX_write_data_mask                                                       (0x00000001)
#define VO_MIX_write_data(data)                                                      (0x00000001&((data)<<0))
#define VO_MIX_write_data_src(data)                                                  ((0x00000001&(data))>>0)
#define VO_MIX_get_write_data(data)                                                  ((0x00000001&(data))>>0)


#define VO_M1BG                                                                      0x18005784
#define VO_M1BG_reg_addr                                                             "0xB8005784"
#define VO_M1BG_reg                                                                  0xB8005784
#define set_VO_M1BG_reg(data)   (*((volatile unsigned int*) VO_M1BG_reg)=data)
#define get_VO_M1BG_reg   (*((volatile unsigned int*) VO_M1BG_reg))
#define VO_M1BG_inst_adr                                                             "0x00E1"
#define VO_M1BG_inst                                                                 0x00E1
#define VO_M1BG_cr_shift                                                             (16)
#define VO_M1BG_cr_mask                                                              (0x00FF0000)
#define VO_M1BG_cr(data)                                                             (0x00FF0000&((data)<<16))
#define VO_M1BG_cr_src(data)                                                         ((0x00FF0000&(data))>>16)
#define VO_M1BG_get_cr(data)                                                         ((0x00FF0000&(data))>>16)
#define VO_M1BG_cb_shift                                                             (8)
#define VO_M1BG_cb_mask                                                              (0x0000FF00)
#define VO_M1BG_cb(data)                                                             (0x0000FF00&((data)<<8))
#define VO_M1BG_cb_src(data)                                                         ((0x0000FF00&(data))>>8)
#define VO_M1BG_get_cb(data)                                                         ((0x0000FF00&(data))>>8)
#define VO_M1BG_y_shift                                                              (0)
#define VO_M1BG_y_mask                                                               (0x000000FF)
#define VO_M1BG_y(data)                                                              (0x000000FF&((data)<<0))
#define VO_M1BG_y_src(data)                                                          ((0x000000FF&(data))>>0)
#define VO_M1BG_get_y(data)                                                          ((0x000000FF&(data))>>0)


#define VO_M1_SIZE                                                                   0x18005788
#define VO_M1_SIZE_reg_addr                                                          "0xB8005788"
#define VO_M1_SIZE_reg                                                               0xB8005788
#define set_VO_M1_SIZE_reg(data)   (*((volatile unsigned int*) VO_M1_SIZE_reg)=data)
#define get_VO_M1_SIZE_reg   (*((volatile unsigned int*) VO_M1_SIZE_reg))
#define VO_M1_SIZE_inst_adr                                                          "0x00E2"
#define VO_M1_SIZE_inst                                                              0x00E2
#define VO_M1_SIZE_w_shift                                                           (16)
#define VO_M1_SIZE_w_mask                                                            (0x1FFF0000)
#define VO_M1_SIZE_w(data)                                                           (0x1FFF0000&((data)<<16))
#define VO_M1_SIZE_w_src(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_M1_SIZE_get_w(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_M1_SIZE_h_shift                                                           (0)
#define VO_M1_SIZE_h_mask                                                            (0x00001FFF)
#define VO_M1_SIZE_h(data)                                                           (0x00001FFF&((data)<<0))
#define VO_M1_SIZE_h_src(data)                                                       ((0x00001FFF&(data))>>0)
#define VO_M1_SIZE_get_h(data)                                                       ((0x00001FFF&(data))>>0)


#define VO_M2BG                                                                      0x1800578C
#define VO_M2BG_reg_addr                                                             "0xB800578C"
#define VO_M2BG_reg                                                                  0xB800578C
#define set_VO_M2BG_reg(data)   (*((volatile unsigned int*) VO_M2BG_reg)=data)
#define get_VO_M2BG_reg   (*((volatile unsigned int*) VO_M2BG_reg))
#define VO_M2BG_inst_adr                                                             "0x00E3"
#define VO_M2BG_inst                                                                 0x00E3
#define VO_M2BG_cr_shift                                                             (16)
#define VO_M2BG_cr_mask                                                              (0x00FF0000)
#define VO_M2BG_cr(data)                                                             (0x00FF0000&((data)<<16))
#define VO_M2BG_cr_src(data)                                                         ((0x00FF0000&(data))>>16)
#define VO_M2BG_get_cr(data)                                                         ((0x00FF0000&(data))>>16)
#define VO_M2BG_cb_shift                                                             (8)
#define VO_M2BG_cb_mask                                                              (0x0000FF00)
#define VO_M2BG_cb(data)                                                             (0x0000FF00&((data)<<8))
#define VO_M2BG_cb_src(data)                                                         ((0x0000FF00&(data))>>8)
#define VO_M2BG_get_cb(data)                                                         ((0x0000FF00&(data))>>8)
#define VO_M2BG_y_shift                                                              (0)
#define VO_M2BG_y_mask                                                               (0x000000FF)
#define VO_M2BG_y(data)                                                              (0x000000FF&((data)<<0))
#define VO_M2BG_y_src(data)                                                          ((0x000000FF&(data))>>0)
#define VO_M2BG_get_y(data)                                                          ((0x000000FF&(data))>>0)


#define VO_M2_SIZE                                                                   0x18005790
#define VO_M2_SIZE_reg_addr                                                          "0xB8005790"
#define VO_M2_SIZE_reg                                                               0xB8005790
#define set_VO_M2_SIZE_reg(data)   (*((volatile unsigned int*) VO_M2_SIZE_reg)=data)
#define get_VO_M2_SIZE_reg   (*((volatile unsigned int*) VO_M2_SIZE_reg))
#define VO_M2_SIZE_inst_adr                                                          "0x00E4"
#define VO_M2_SIZE_inst                                                              0x00E4
#define VO_M2_SIZE_w_shift                                                           (16)
#define VO_M2_SIZE_w_mask                                                            (0x1FFF0000)
#define VO_M2_SIZE_w(data)                                                           (0x1FFF0000&((data)<<16))
#define VO_M2_SIZE_w_src(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_M2_SIZE_get_w(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_M2_SIZE_h_shift                                                           (0)
#define VO_M2_SIZE_h_mask                                                            (0x00001FFF)
#define VO_M2_SIZE_h(data)                                                           (0x00001FFF&((data)<<0))
#define VO_M2_SIZE_h_src(data)                                                       ((0x00001FFF&(data))>>0)
#define VO_M2_SIZE_get_h(data)                                                       ((0x00001FFF&(data))>>0)


#define VO_V1_XY                                                                     0x18005794
#define VO_V1_XY_reg_addr                                                            "0xB8005794"
#define VO_V1_XY_reg                                                                 0xB8005794
#define set_VO_V1_XY_reg(data)   (*((volatile unsigned int*) VO_V1_XY_reg)=data)
#define get_VO_V1_XY_reg   (*((volatile unsigned int*) VO_V1_XY_reg))
#define VO_V1_XY_inst_adr                                                            "0x00E5"
#define VO_V1_XY_inst                                                                0x00E5
#define VO_V1_XY_x_shift                                                             (16)
#define VO_V1_XY_x_mask                                                              (0x1FFF0000)
#define VO_V1_XY_x(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_V1_XY_x_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V1_XY_get_x(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V1_XY_y_shift                                                             (0)
#define VO_V1_XY_y_mask                                                              (0x00001FFF)
#define VO_V1_XY_y(data)                                                             (0x00001FFF&((data)<<0))
#define VO_V1_XY_y_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_V1_XY_get_y(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_V1B_C                                                                     0x18005798
#define VO_V1B_C_reg_addr                                                            "0xB8005798"
#define VO_V1B_C_reg                                                                 0xB8005798
#define set_VO_V1B_C_reg(data)   (*((volatile unsigned int*) VO_V1B_C_reg)=data)
#define get_VO_V1B_C_reg   (*((volatile unsigned int*) VO_V1B_C_reg))
#define VO_V1B_C_inst_adr                                                            "0x00E6"
#define VO_V1B_C_inst                                                                0x00E6
#define VO_V1B_C_cr_shift                                                            (16)
#define VO_V1B_C_cr_mask                                                             (0x00FF0000)
#define VO_V1B_C_cr(data)                                                            (0x00FF0000&((data)<<16))
#define VO_V1B_C_cr_src(data)                                                        ((0x00FF0000&(data))>>16)
#define VO_V1B_C_get_cr(data)                                                        ((0x00FF0000&(data))>>16)
#define VO_V1B_C_cb_shift                                                            (8)
#define VO_V1B_C_cb_mask                                                             (0x0000FF00)
#define VO_V1B_C_cb(data)                                                            (0x0000FF00&((data)<<8))
#define VO_V1B_C_cb_src(data)                                                        ((0x0000FF00&(data))>>8)
#define VO_V1B_C_get_cb(data)                                                        ((0x0000FF00&(data))>>8)
#define VO_V1B_C_y_shift                                                             (0)
#define VO_V1B_C_y_mask                                                              (0x000000FF)
#define VO_V1B_C_y(data)                                                             (0x000000FF&((data)<<0))
#define VO_V1B_C_y_src(data)                                                         ((0x000000FF&(data))>>0)
#define VO_V1B_C_get_y(data)                                                         ((0x000000FF&(data))>>0)


#define VO_V1B_X                                                                     0x1800579C
#define VO_V1B_X_reg_addr                                                            "0xB800579C"
#define VO_V1B_X_reg                                                                 0xB800579C
#define set_VO_V1B_X_reg(data)   (*((volatile unsigned int*) VO_V1B_X_reg)=data)
#define get_VO_V1B_X_reg   (*((volatile unsigned int*) VO_V1B_X_reg))
#define VO_V1B_X_inst_adr                                                            "0x00E7"
#define VO_V1B_X_inst                                                                0x00E7
#define VO_V1B_X_e_shift                                                             (16)
#define VO_V1B_X_e_mask                                                              (0x1FFF0000)
#define VO_V1B_X_e(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_V1B_X_e_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V1B_X_get_e(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V1B_X_s_shift                                                             (0)
#define VO_V1B_X_s_mask                                                              (0x00001FFF)
#define VO_V1B_X_s(data)                                                             (0x00001FFF&((data)<<0))
#define VO_V1B_X_s_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_V1B_X_get_s(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_V1B_Y                                                                     0x180057A0
#define VO_V1B_Y_reg_addr                                                            "0xB80057A0"
#define VO_V1B_Y_reg                                                                 0xB80057A0
#define set_VO_V1B_Y_reg(data)   (*((volatile unsigned int*) VO_V1B_Y_reg)=data)
#define get_VO_V1B_Y_reg   (*((volatile unsigned int*) VO_V1B_Y_reg))
#define VO_V1B_Y_inst_adr                                                            "0x00E8"
#define VO_V1B_Y_inst                                                                0x00E8
#define VO_V1B_Y_e_shift                                                             (16)
#define VO_V1B_Y_e_mask                                                              (0x1FFF0000)
#define VO_V1B_Y_e(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_V1B_Y_e_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V1B_Y_get_e(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V1B_Y_s_shift                                                             (0)
#define VO_V1B_Y_s_mask                                                              (0x00001FFF)
#define VO_V1B_Y_s(data)                                                             (0x00001FFF&((data)<<0))
#define VO_V1B_Y_s_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_V1B_Y_get_s(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_V1_3D                                                                     0x180057A4
#define VO_V1_3D_reg_addr                                                            "0xB80057A4"
#define VO_V1_3D_reg                                                                 0xB80057A4
#define set_VO_V1_3D_reg(data)   (*((volatile unsigned int*) VO_V1_3D_reg)=data)
#define get_VO_V1_3D_reg   (*((volatile unsigned int*) VO_V1_3D_reg))
#define VO_V1_3D_inst_adr                                                            "0x00E9"
#define VO_V1_3D_inst                                                                0x00E9
#define VO_V1_3D_dir_r_shift                                                         (27)
#define VO_V1_3D_dir_r_mask                                                          (0x08000000)
#define VO_V1_3D_dir_r(data)                                                         (0x08000000&((data)<<27))
#define VO_V1_3D_dir_r_src(data)                                                     ((0x08000000&(data))>>27)
#define VO_V1_3D_get_dir_r(data)                                                     ((0x08000000&(data))>>27)
#define VO_V1_3D_offset_r_shift                                                      (16)
#define VO_V1_3D_offset_r_mask                                                       (0x07FF0000)
#define VO_V1_3D_offset_r(data)                                                      (0x07FF0000&((data)<<16))
#define VO_V1_3D_offset_r_src(data)                                                  ((0x07FF0000&(data))>>16)
#define VO_V1_3D_get_offset_r(data)                                                  ((0x07FF0000&(data))>>16)
#define VO_V1_3D_dir_l_shift                                                         (11)
#define VO_V1_3D_dir_l_mask                                                          (0x00000800)
#define VO_V1_3D_dir_l(data)                                                         (0x00000800&((data)<<11))
#define VO_V1_3D_dir_l_src(data)                                                     ((0x00000800&(data))>>11)
#define VO_V1_3D_get_dir_l(data)                                                     ((0x00000800&(data))>>11)
#define VO_V1_3D_offset_l_shift                                                      (0)
#define VO_V1_3D_offset_l_mask                                                       (0x000007FF)
#define VO_V1_3D_offset_l(data)                                                      (0x000007FF&((data)<<0))
#define VO_V1_3D_offset_l_src(data)                                                  ((0x000007FF&(data))>>0)
#define VO_V1_3D_get_offset_l(data)                                                  ((0x000007FF&(data))>>0)


#define VO_V2_XY                                                                     0x180057A8
#define VO_V2_XY_reg_addr                                                            "0xB80057A8"
#define VO_V2_XY_reg                                                                 0xB80057A8
#define set_VO_V2_XY_reg(data)   (*((volatile unsigned int*) VO_V2_XY_reg)=data)
#define get_VO_V2_XY_reg   (*((volatile unsigned int*) VO_V2_XY_reg))
#define VO_V2_XY_inst_adr                                                            "0x00EA"
#define VO_V2_XY_inst                                                                0x00EA
#define VO_V2_XY_x_shift                                                             (16)
#define VO_V2_XY_x_mask                                                              (0x1FFF0000)
#define VO_V2_XY_x(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_V2_XY_x_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V2_XY_get_x(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V2_XY_y_shift                                                             (0)
#define VO_V2_XY_y_mask                                                              (0x00001FFF)
#define VO_V2_XY_y(data)                                                             (0x00001FFF&((data)<<0))
#define VO_V2_XY_y_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_V2_XY_get_y(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_V2B_C                                                                     0x180057AC
#define VO_V2B_C_reg_addr                                                            "0xB80057AC"
#define VO_V2B_C_reg                                                                 0xB80057AC
#define set_VO_V2B_C_reg(data)   (*((volatile unsigned int*) VO_V2B_C_reg)=data)
#define get_VO_V2B_C_reg   (*((volatile unsigned int*) VO_V2B_C_reg))
#define VO_V2B_C_inst_adr                                                            "0x00EB"
#define VO_V2B_C_inst                                                                0x00EB
#define VO_V2B_C_cr_shift                                                            (16)
#define VO_V2B_C_cr_mask                                                             (0x00FF0000)
#define VO_V2B_C_cr(data)                                                            (0x00FF0000&((data)<<16))
#define VO_V2B_C_cr_src(data)                                                        ((0x00FF0000&(data))>>16)
#define VO_V2B_C_get_cr(data)                                                        ((0x00FF0000&(data))>>16)
#define VO_V2B_C_cb_shift                                                            (8)
#define VO_V2B_C_cb_mask                                                             (0x0000FF00)
#define VO_V2B_C_cb(data)                                                            (0x0000FF00&((data)<<8))
#define VO_V2B_C_cb_src(data)                                                        ((0x0000FF00&(data))>>8)
#define VO_V2B_C_get_cb(data)                                                        ((0x0000FF00&(data))>>8)
#define VO_V2B_C_y_shift                                                             (0)
#define VO_V2B_C_y_mask                                                              (0x000000FF)
#define VO_V2B_C_y(data)                                                             (0x000000FF&((data)<<0))
#define VO_V2B_C_y_src(data)                                                         ((0x000000FF&(data))>>0)
#define VO_V2B_C_get_y(data)                                                         ((0x000000FF&(data))>>0)


#define VO_V2B_X                                                                     0x180057B0
#define VO_V2B_X_reg_addr                                                            "0xB80057B0"
#define VO_V2B_X_reg                                                                 0xB80057B0
#define set_VO_V2B_X_reg(data)   (*((volatile unsigned int*) VO_V2B_X_reg)=data)
#define get_VO_V2B_X_reg   (*((volatile unsigned int*) VO_V2B_X_reg))
#define VO_V2B_X_inst_adr                                                            "0x00EC"
#define VO_V2B_X_inst                                                                0x00EC
#define VO_V2B_X_e_shift                                                             (16)
#define VO_V2B_X_e_mask                                                              (0x1FFF0000)
#define VO_V2B_X_e(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_V2B_X_e_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V2B_X_get_e(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V2B_X_s_shift                                                             (0)
#define VO_V2B_X_s_mask                                                              (0x00001FFF)
#define VO_V2B_X_s(data)                                                             (0x00001FFF&((data)<<0))
#define VO_V2B_X_s_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_V2B_X_get_s(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_V2B_Y                                                                     0x180057B4
#define VO_V2B_Y_reg_addr                                                            "0xB80057B4"
#define VO_V2B_Y_reg                                                                 0xB80057B4
#define set_VO_V2B_Y_reg(data)   (*((volatile unsigned int*) VO_V2B_Y_reg)=data)
#define get_VO_V2B_Y_reg   (*((volatile unsigned int*) VO_V2B_Y_reg))
#define VO_V2B_Y_inst_adr                                                            "0x00ED"
#define VO_V2B_Y_inst                                                                0x00ED
#define VO_V2B_Y_e_shift                                                             (16)
#define VO_V2B_Y_e_mask                                                              (0x1FFF0000)
#define VO_V2B_Y_e(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_V2B_Y_e_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V2B_Y_get_e(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_V2B_Y_s_shift                                                             (0)
#define VO_V2B_Y_s_mask                                                              (0x00001FFF)
#define VO_V2B_Y_s(data)                                                             (0x00001FFF&((data)<<0))
#define VO_V2B_Y_s_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_V2B_Y_get_s(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_V2BG_C                                                                    0x180057B8
#define VO_V2BG_C_reg_addr                                                           "0xB80057B8"
#define VO_V2BG_C_reg                                                                0xB80057B8
#define set_VO_V2BG_C_reg(data)   (*((volatile unsigned int*) VO_V2BG_C_reg)=data)
#define get_VO_V2BG_C_reg   (*((volatile unsigned int*) VO_V2BG_C_reg))
#define VO_V2BG_C_inst_adr                                                           "0x00EE"
#define VO_V2BG_C_inst                                                               0x00EE
#define VO_V2BG_C_cr_shift                                                           (16)
#define VO_V2BG_C_cr_mask                                                            (0x00FF0000)
#define VO_V2BG_C_cr(data)                                                           (0x00FF0000&((data)<<16))
#define VO_V2BG_C_cr_src(data)                                                       ((0x00FF0000&(data))>>16)
#define VO_V2BG_C_get_cr(data)                                                       ((0x00FF0000&(data))>>16)
#define VO_V2BG_C_cb_shift                                                           (8)
#define VO_V2BG_C_cb_mask                                                            (0x0000FF00)
#define VO_V2BG_C_cb(data)                                                           (0x0000FF00&((data)<<8))
#define VO_V2BG_C_cb_src(data)                                                       ((0x0000FF00&(data))>>8)
#define VO_V2BG_C_get_cb(data)                                                       ((0x0000FF00&(data))>>8)
#define VO_V2BG_C_y_shift                                                            (0)
#define VO_V2BG_C_y_mask                                                             (0x000000FF)
#define VO_V2BG_C_y(data)                                                            (0x000000FF&((data)<<0))
#define VO_V2BG_C_y_src(data)                                                        ((0x000000FF&(data))>>0)
#define VO_V2BG_C_get_y(data)                                                        ((0x000000FF&(data))>>0)


#define VO_V2BG_X                                                                    0x180057BC
#define VO_V2BG_X_reg_addr                                                           "0xB80057BC"
#define VO_V2BG_X_reg                                                                0xB80057BC
#define set_VO_V2BG_X_reg(data)   (*((volatile unsigned int*) VO_V2BG_X_reg)=data)
#define get_VO_V2BG_X_reg   (*((volatile unsigned int*) VO_V2BG_X_reg))
#define VO_V2BG_X_inst_adr                                                           "0x00EF"
#define VO_V2BG_X_inst                                                               0x00EF
#define VO_V2BG_X_e_shift                                                            (16)
#define VO_V2BG_X_e_mask                                                             (0x1FFF0000)
#define VO_V2BG_X_e(data)                                                            (0x1FFF0000&((data)<<16))
#define VO_V2BG_X_e_src(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_V2BG_X_get_e(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_V2BG_X_s_shift                                                            (0)
#define VO_V2BG_X_s_mask                                                             (0x00001FFF)
#define VO_V2BG_X_s(data)                                                            (0x00001FFF&((data)<<0))
#define VO_V2BG_X_s_src(data)                                                        ((0x00001FFF&(data))>>0)
#define VO_V2BG_X_get_s(data)                                                        ((0x00001FFF&(data))>>0)


#define VO_V2BG_Y                                                                    0x180057C0
#define VO_V2BG_Y_reg_addr                                                           "0xB80057C0"
#define VO_V2BG_Y_reg                                                                0xB80057C0
#define set_VO_V2BG_Y_reg(data)   (*((volatile unsigned int*) VO_V2BG_Y_reg)=data)
#define get_VO_V2BG_Y_reg   (*((volatile unsigned int*) VO_V2BG_Y_reg))
#define VO_V2BG_Y_inst_adr                                                           "0x00F0"
#define VO_V2BG_Y_inst                                                               0x00F0
#define VO_V2BG_Y_e_shift                                                            (16)
#define VO_V2BG_Y_e_mask                                                             (0x1FFF0000)
#define VO_V2BG_Y_e(data)                                                            (0x1FFF0000&((data)<<16))
#define VO_V2BG_Y_e_src(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_V2BG_Y_get_e(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_V2BG_Y_s_shift                                                            (0)
#define VO_V2BG_Y_s_mask                                                             (0x00001FFF)
#define VO_V2BG_Y_s(data)                                                            (0x00001FFF&((data)<<0))
#define VO_V2BG_Y_s_src(data)                                                        ((0x00001FFF&(data))>>0)
#define VO_V2BG_Y_get_s(data)                                                        ((0x00001FFF&(data))>>0)


#define VO_SUB1_XY                                                                   0x180057C4
#define VO_SUB1_XY_reg_addr                                                          "0xB80057C4"
#define VO_SUB1_XY_reg                                                               0xB80057C4
#define set_VO_SUB1_XY_reg(data)   (*((volatile unsigned int*) VO_SUB1_XY_reg)=data)
#define get_VO_SUB1_XY_reg   (*((volatile unsigned int*) VO_SUB1_XY_reg))
#define VO_SUB1_XY_inst_adr                                                          "0x00F1"
#define VO_SUB1_XY_inst                                                              0x00F1
#define VO_SUB1_XY_x_shift                                                           (16)
#define VO_SUB1_XY_x_mask                                                            (0x1FFF0000)
#define VO_SUB1_XY_x(data)                                                           (0x1FFF0000&((data)<<16))
#define VO_SUB1_XY_x_src(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_SUB1_XY_get_x(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_SUB1_XY_y_shift                                                           (0)
#define VO_SUB1_XY_y_mask                                                            (0x00001FFF)
#define VO_SUB1_XY_y(data)                                                           (0x00001FFF&((data)<<0))
#define VO_SUB1_XY_y_src(data)                                                       ((0x00001FFF&(data))>>0)
#define VO_SUB1_XY_get_y(data)                                                       ((0x00001FFF&(data))>>0)


#define VO_SUB1_3D                                                                   0x180057C8
#define VO_SUB1_3D_reg_addr                                                          "0xB80057C8"
#define VO_SUB1_3D_reg                                                               0xB80057C8
#define set_VO_SUB1_3D_reg(data)   (*((volatile unsigned int*) VO_SUB1_3D_reg)=data)
#define get_VO_SUB1_3D_reg   (*((volatile unsigned int*) VO_SUB1_3D_reg))
#define VO_SUB1_3D_inst_adr                                                          "0x00F2"
#define VO_SUB1_3D_inst                                                              0x00F2
#define VO_SUB1_3D_dir_r_shift                                                       (27)
#define VO_SUB1_3D_dir_r_mask                                                        (0x08000000)
#define VO_SUB1_3D_dir_r(data)                                                       (0x08000000&((data)<<27))
#define VO_SUB1_3D_dir_r_src(data)                                                   ((0x08000000&(data))>>27)
#define VO_SUB1_3D_get_dir_r(data)                                                   ((0x08000000&(data))>>27)
#define VO_SUB1_3D_offset_r_shift                                                    (16)
#define VO_SUB1_3D_offset_r_mask                                                     (0x07FF0000)
#define VO_SUB1_3D_offset_r(data)                                                    (0x07FF0000&((data)<<16))
#define VO_SUB1_3D_offset_r_src(data)                                                ((0x07FF0000&(data))>>16)
#define VO_SUB1_3D_get_offset_r(data)                                                ((0x07FF0000&(data))>>16)
#define VO_SUB1_3D_dir_l_shift                                                       (11)
#define VO_SUB1_3D_dir_l_mask                                                        (0x00000800)
#define VO_SUB1_3D_dir_l(data)                                                       (0x00000800&((data)<<11))
#define VO_SUB1_3D_dir_l_src(data)                                                   ((0x00000800&(data))>>11)
#define VO_SUB1_3D_get_dir_l(data)                                                   ((0x00000800&(data))>>11)
#define VO_SUB1_3D_offset_l_shift                                                    (0)
#define VO_SUB1_3D_offset_l_mask                                                     (0x000007FF)
#define VO_SUB1_3D_offset_l(data)                                                    (0x000007FF&((data)<<0))
#define VO_SUB1_3D_offset_l_src(data)                                                ((0x000007FF&(data))>>0)
#define VO_SUB1_3D_get_offset_l(data)                                                ((0x000007FF&(data))>>0)


#define VO_OSD1_XY                                                                   0x180057CC
#define VO_OSD1_XY_reg_addr                                                          "0xB80057CC"
#define VO_OSD1_XY_reg                                                               0xB80057CC
#define set_VO_OSD1_XY_reg(data)   (*((volatile unsigned int*) VO_OSD1_XY_reg)=data)
#define get_VO_OSD1_XY_reg   (*((volatile unsigned int*) VO_OSD1_XY_reg))
#define VO_OSD1_XY_inst_adr                                                          "0x00F3"
#define VO_OSD1_XY_inst                                                              0x00F3
#define VO_OSD1_XY_x_shift                                                           (16)
#define VO_OSD1_XY_x_mask                                                            (0x1FFF0000)
#define VO_OSD1_XY_x(data)                                                           (0x1FFF0000&((data)<<16))
#define VO_OSD1_XY_x_src(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_OSD1_XY_get_x(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_OSD1_XY_y_shift                                                           (0)
#define VO_OSD1_XY_y_mask                                                            (0x00001FFF)
#define VO_OSD1_XY_y(data)                                                           (0x00001FFF&((data)<<0))
#define VO_OSD1_XY_y_src(data)                                                       ((0x00001FFF&(data))>>0)
#define VO_OSD1_XY_get_y(data)                                                       ((0x00001FFF&(data))>>0)


#define VO_OSD1_3D                                                                   0x180057D0
#define VO_OSD1_3D_reg_addr                                                          "0xB80057D0"
#define VO_OSD1_3D_reg                                                               0xB80057D0
#define set_VO_OSD1_3D_reg(data)   (*((volatile unsigned int*) VO_OSD1_3D_reg)=data)
#define get_VO_OSD1_3D_reg   (*((volatile unsigned int*) VO_OSD1_3D_reg))
#define VO_OSD1_3D_inst_adr                                                          "0x00F4"
#define VO_OSD1_3D_inst                                                              0x00F4
#define VO_OSD1_3D_dir_r_shift                                                       (27)
#define VO_OSD1_3D_dir_r_mask                                                        (0x08000000)
#define VO_OSD1_3D_dir_r(data)                                                       (0x08000000&((data)<<27))
#define VO_OSD1_3D_dir_r_src(data)                                                   ((0x08000000&(data))>>27)
#define VO_OSD1_3D_get_dir_r(data)                                                   ((0x08000000&(data))>>27)
#define VO_OSD1_3D_offset_r_shift                                                    (16)
#define VO_OSD1_3D_offset_r_mask                                                     (0x07FF0000)
#define VO_OSD1_3D_offset_r(data)                                                    (0x07FF0000&((data)<<16))
#define VO_OSD1_3D_offset_r_src(data)                                                ((0x07FF0000&(data))>>16)
#define VO_OSD1_3D_get_offset_r(data)                                                ((0x07FF0000&(data))>>16)
#define VO_OSD1_3D_dir_l_shift                                                       (11)
#define VO_OSD1_3D_dir_l_mask                                                        (0x00000800)
#define VO_OSD1_3D_dir_l(data)                                                       (0x00000800&((data)<<11))
#define VO_OSD1_3D_dir_l_src(data)                                                   ((0x00000800&(data))>>11)
#define VO_OSD1_3D_get_dir_l(data)                                                   ((0x00000800&(data))>>11)
#define VO_OSD1_3D_offset_l_shift                                                    (0)
#define VO_OSD1_3D_offset_l_mask                                                     (0x000007FF)
#define VO_OSD1_3D_offset_l(data)                                                    (0x000007FF&((data)<<0))
#define VO_OSD1_3D_offset_l_src(data)                                                ((0x000007FF&(data))>>0)
#define VO_OSD1_3D_get_offset_l(data)                                                ((0x000007FF&(data))>>0)


#define VO_OSD2_XY                                                                   0x180057D4
#define VO_OSD2_XY_reg_addr                                                          "0xB80057D4"
#define VO_OSD2_XY_reg                                                               0xB80057D4
#define set_VO_OSD2_XY_reg(data)   (*((volatile unsigned int*) VO_OSD2_XY_reg)=data)
#define get_VO_OSD2_XY_reg   (*((volatile unsigned int*) VO_OSD2_XY_reg))
#define VO_OSD2_XY_inst_adr                                                          "0x00F5"
#define VO_OSD2_XY_inst                                                              0x00F5
#define VO_OSD2_XY_x_shift                                                           (16)
#define VO_OSD2_XY_x_mask                                                            (0x1FFF0000)
#define VO_OSD2_XY_x(data)                                                           (0x1FFF0000&((data)<<16))
#define VO_OSD2_XY_x_src(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_OSD2_XY_get_x(data)                                                       ((0x1FFF0000&(data))>>16)
#define VO_OSD2_XY_y_shift                                                           (0)
#define VO_OSD2_XY_y_mask                                                            (0x00001FFF)
#define VO_OSD2_XY_y(data)                                                           (0x00001FFF&((data)<<0))
#define VO_OSD2_XY_y_src(data)                                                       ((0x00001FFF&(data))>>0)
#define VO_OSD2_XY_get_y(data)                                                       ((0x00001FFF&(data))>>0)


#define VO_OSD2_3D                                                                   0x180057D8
#define VO_OSD2_3D_reg_addr                                                          "0xB80057D8"
#define VO_OSD2_3D_reg                                                               0xB80057D8
#define set_VO_OSD2_3D_reg(data)   (*((volatile unsigned int*) VO_OSD2_3D_reg)=data)
#define get_VO_OSD2_3D_reg   (*((volatile unsigned int*) VO_OSD2_3D_reg))
#define VO_OSD2_3D_inst_adr                                                          "0x00F6"
#define VO_OSD2_3D_inst                                                              0x00F6
#define VO_OSD2_3D_dir_r_shift                                                       (27)
#define VO_OSD2_3D_dir_r_mask                                                        (0x08000000)
#define VO_OSD2_3D_dir_r(data)                                                       (0x08000000&((data)<<27))
#define VO_OSD2_3D_dir_r_src(data)                                                   ((0x08000000&(data))>>27)
#define VO_OSD2_3D_get_dir_r(data)                                                   ((0x08000000&(data))>>27)
#define VO_OSD2_3D_offset_r_shift                                                    (16)
#define VO_OSD2_3D_offset_r_mask                                                     (0x07FF0000)
#define VO_OSD2_3D_offset_r(data)                                                    (0x07FF0000&((data)<<16))
#define VO_OSD2_3D_offset_r_src(data)                                                ((0x07FF0000&(data))>>16)
#define VO_OSD2_3D_get_offset_r(data)                                                ((0x07FF0000&(data))>>16)
#define VO_OSD2_3D_dir_l_shift                                                       (11)
#define VO_OSD2_3D_dir_l_mask                                                        (0x00000800)
#define VO_OSD2_3D_dir_l(data)                                                       (0x00000800&((data)<<11))
#define VO_OSD2_3D_dir_l_src(data)                                                   ((0x00000800&(data))>>11)
#define VO_OSD2_3D_get_dir_l(data)                                                   ((0x00000800&(data))>>11)
#define VO_OSD2_3D_offset_l_shift                                                    (0)
#define VO_OSD2_3D_offset_l_mask                                                     (0x000007FF)
#define VO_OSD2_3D_offset_l(data)                                                    (0x000007FF&((data)<<0))
#define VO_OSD2_3D_offset_l_src(data)                                                ((0x000007FF&(data))>>0)
#define VO_OSD2_3D_get_offset_l(data)                                                ((0x000007FF&(data))>>0)


#define VO_CSR_XY                                                                    0x180057DC
#define VO_CSR_XY_reg_addr                                                           "0xB80057DC"
#define VO_CSR_XY_reg                                                                0xB80057DC
#define set_VO_CSR_XY_reg(data)   (*((volatile unsigned int*) VO_CSR_XY_reg)=data)
#define get_VO_CSR_XY_reg   (*((volatile unsigned int*) VO_CSR_XY_reg))
#define VO_CSR_XY_inst_adr                                                           "0x00F7"
#define VO_CSR_XY_inst                                                               0x00F7
#define VO_CSR_XY_x_shift                                                            (16)
#define VO_CSR_XY_x_mask                                                             (0x1FFF0000)
#define VO_CSR_XY_x(data)                                                            (0x1FFF0000&((data)<<16))
#define VO_CSR_XY_x_src(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_CSR_XY_get_x(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_CSR_XY_y_shift                                                            (0)
#define VO_CSR_XY_y_mask                                                             (0x00001FFF)
#define VO_CSR_XY_y(data)                                                            (0x00001FFF&((data)<<0))
#define VO_CSR_XY_y_src(data)                                                        ((0x00001FFF&(data))>>0)
#define VO_CSR_XY_get_y(data)                                                        ((0x00001FFF&(data))>>0)


#define VO_CSR_3D                                                                    0x180057E0
#define VO_CSR_3D_reg_addr                                                           "0xB80057E0"
#define VO_CSR_3D_reg                                                                0xB80057E0
#define set_VO_CSR_3D_reg(data)   (*((volatile unsigned int*) VO_CSR_3D_reg)=data)
#define get_VO_CSR_3D_reg   (*((volatile unsigned int*) VO_CSR_3D_reg))
#define VO_CSR_3D_inst_adr                                                           "0x00F8"
#define VO_CSR_3D_inst                                                               0x00F8
#define VO_CSR_3D_dir_r_shift                                                        (27)
#define VO_CSR_3D_dir_r_mask                                                         (0x08000000)
#define VO_CSR_3D_dir_r(data)                                                        (0x08000000&((data)<<27))
#define VO_CSR_3D_dir_r_src(data)                                                    ((0x08000000&(data))>>27)
#define VO_CSR_3D_get_dir_r(data)                                                    ((0x08000000&(data))>>27)
#define VO_CSR_3D_offset_r_shift                                                     (16)
#define VO_CSR_3D_offset_r_mask                                                      (0x07FF0000)
#define VO_CSR_3D_offset_r(data)                                                     (0x07FF0000&((data)<<16))
#define VO_CSR_3D_offset_r_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_CSR_3D_get_offset_r(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_CSR_3D_dir_l_shift                                                        (11)
#define VO_CSR_3D_dir_l_mask                                                         (0x00000800)
#define VO_CSR_3D_dir_l(data)                                                        (0x00000800&((data)<<11))
#define VO_CSR_3D_dir_l_src(data)                                                    ((0x00000800&(data))>>11)
#define VO_CSR_3D_get_dir_l(data)                                                    ((0x00000800&(data))>>11)
#define VO_CSR_3D_offset_l_shift                                                     (0)
#define VO_CSR_3D_offset_l_mask                                                      (0x000007FF)
#define VO_CSR_3D_offset_l(data)                                                     (0x000007FF&((data)<<0))
#define VO_CSR_3D_offset_l_src(data)                                                 ((0x000007FF&(data))>>0)
#define VO_CSR_3D_get_offset_l(data)                                                 ((0x000007FF&(data))>>0)


#define VO_PIC_XY                                                                    0x180057E4
#define VO_PIC_XY_reg_addr                                                           "0xB80057E4"
#define VO_PIC_XY_reg                                                                0xB80057E4
#define set_VO_PIC_XY_reg(data)   (*((volatile unsigned int*) VO_PIC_XY_reg)=data)
#define get_VO_PIC_XY_reg   (*((volatile unsigned int*) VO_PIC_XY_reg))
#define VO_PIC_XY_inst_adr                                                           "0x00F9"
#define VO_PIC_XY_inst                                                               0x00F9
#define VO_PIC_XY_x_shift                                                            (16)
#define VO_PIC_XY_x_mask                                                             (0x1FFF0000)
#define VO_PIC_XY_x(data)                                                            (0x1FFF0000&((data)<<16))
#define VO_PIC_XY_x_src(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_PIC_XY_get_x(data)                                                        ((0x1FFF0000&(data))>>16)
#define VO_PIC_XY_y_shift                                                            (0)
#define VO_PIC_XY_y_mask                                                             (0x00001FFF)
#define VO_PIC_XY_y(data)                                                            (0x00001FFF&((data)<<0))
#define VO_PIC_XY_y_src(data)                                                        ((0x00001FFF&(data))>>0)
#define VO_PIC_XY_get_y(data)                                                        ((0x00001FFF&(data))>>0)


#define VO_PIC_3D                                                                    0x180057E8
#define VO_PIC_3D_reg_addr                                                           "0xB80057E8"
#define VO_PIC_3D_reg                                                                0xB80057E8
#define set_VO_PIC_3D_reg(data)   (*((volatile unsigned int*) VO_PIC_3D_reg)=data)
#define get_VO_PIC_3D_reg   (*((volatile unsigned int*) VO_PIC_3D_reg))
#define VO_PIC_3D_inst_adr                                                           "0x00FA"
#define VO_PIC_3D_inst                                                               0x00FA
#define VO_PIC_3D_dir_r_shift                                                        (27)
#define VO_PIC_3D_dir_r_mask                                                         (0x08000000)
#define VO_PIC_3D_dir_r(data)                                                        (0x08000000&((data)<<27))
#define VO_PIC_3D_dir_r_src(data)                                                    ((0x08000000&(data))>>27)
#define VO_PIC_3D_get_dir_r(data)                                                    ((0x08000000&(data))>>27)
#define VO_PIC_3D_offset_r_shift                                                     (16)
#define VO_PIC_3D_offset_r_mask                                                      (0x07FF0000)
#define VO_PIC_3D_offset_r(data)                                                     (0x07FF0000&((data)<<16))
#define VO_PIC_3D_offset_r_src(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_PIC_3D_get_offset_r(data)                                                 ((0x07FF0000&(data))>>16)
#define VO_PIC_3D_dir_l_shift                                                        (11)
#define VO_PIC_3D_dir_l_mask                                                         (0x00000800)
#define VO_PIC_3D_dir_l(data)                                                        (0x00000800&((data)<<11))
#define VO_PIC_3D_dir_l_src(data)                                                    ((0x00000800&(data))>>11)
#define VO_PIC_3D_get_dir_l(data)                                                    ((0x00000800&(data))>>11)
#define VO_PIC_3D_offset_l_shift                                                     (0)
#define VO_PIC_3D_offset_l_mask                                                      (0x000007FF)
#define VO_PIC_3D_offset_l(data)                                                     (0x000007FF&((data)<<0))
#define VO_PIC_3D_offset_l_src(data)                                                 ((0x000007FF&(data))>>0)
#define VO_PIC_3D_get_offset_l(data)                                                 ((0x000007FF&(data))>>0)


#define VO_DD_XY                                                                     0x180057EC
#define VO_DD_XY_reg_addr                                                            "0xB80057EC"
#define VO_DD_XY_reg                                                                 0xB80057EC
#define set_VO_DD_XY_reg(data)   (*((volatile unsigned int*) VO_DD_XY_reg)=data)
#define get_VO_DD_XY_reg   (*((volatile unsigned int*) VO_DD_XY_reg))
#define VO_DD_XY_inst_adr                                                            "0x00FB"
#define VO_DD_XY_inst                                                                0x00FB
#define VO_DD_XY_x_shift                                                             (16)
#define VO_DD_XY_x_mask                                                              (0x1FFF0000)
#define VO_DD_XY_x(data)                                                             (0x1FFF0000&((data)<<16))
#define VO_DD_XY_x_src(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_DD_XY_get_x(data)                                                         ((0x1FFF0000&(data))>>16)
#define VO_DD_XY_y_shift                                                             (0)
#define VO_DD_XY_y_mask                                                              (0x00001FFF)
#define VO_DD_XY_y(data)                                                             (0x00001FFF&((data)<<0))
#define VO_DD_XY_y_src(data)                                                         ((0x00001FFF&(data))>>0)
#define VO_DD_XY_get_y(data)                                                         ((0x00001FFF&(data))>>0)


#define VO_CH1_BUF                                                                   0x180057F0
#define VO_CH1_BUF_reg_addr                                                          "0xB80057F0"
#define VO_CH1_BUF_reg                                                               0xB80057F0
#define set_VO_CH1_BUF_reg(data)   (*((volatile unsigned int*) VO_CH1_BUF_reg)=data)
#define get_VO_CH1_BUF_reg   (*((volatile unsigned int*) VO_CH1_BUF_reg))
#define VO_CH1_BUF_inst_adr                                                          "0x00FC"
#define VO_CH1_BUF_inst                                                              0x00FC
#define VO_CH1_BUF_write_en2_shift                                                   (23)
#define VO_CH1_BUF_write_en2_mask                                                    (0x00800000)
#define VO_CH1_BUF_write_en2(data)                                                   (0x00800000&((data)<<23))
#define VO_CH1_BUF_write_en2_src(data)                                               ((0x00800000&(data))>>23)
#define VO_CH1_BUF_get_write_en2(data)                                               ((0x00800000&(data))>>23)
#define VO_CH1_BUF_th_shift                                                          (12)
#define VO_CH1_BUF_th_mask                                                           (0x007FF000)
#define VO_CH1_BUF_th(data)                                                          (0x007FF000&((data)<<12))
#define VO_CH1_BUF_th_src(data)                                                      ((0x007FF000&(data))>>12)
#define VO_CH1_BUF_get_th(data)                                                      ((0x007FF000&(data))>>12)
#define VO_CH1_BUF_write_en1_shift                                                   (10)
#define VO_CH1_BUF_write_en1_mask                                                    (0x00000400)
#define VO_CH1_BUF_write_en1(data)                                                   (0x00000400&((data)<<10))
#define VO_CH1_BUF_write_en1_src(data)                                               ((0x00000400&(data))>>10)
#define VO_CH1_BUF_get_write_en1(data)                                               ((0x00000400&(data))>>10)
#define VO_CH1_BUF_b_shift                                                           (0)
#define VO_CH1_BUF_b_mask                                                            (0x000003FF)
#define VO_CH1_BUF_b(data)                                                           (0x000003FF&((data)<<0))
#define VO_CH1_BUF_b_src(data)                                                       ((0x000003FF&(data))>>0)
#define VO_CH1_BUF_get_b(data)                                                       ((0x000003FF&(data))>>0)


#define VO_CHI_BUF                                                                   0x180057F4
#define VO_CHI_BUF_reg_addr                                                          "0xB80057F4"
#define VO_CHI_BUF_reg                                                               0xB80057F4
#define set_VO_CHI_BUF_reg(data)   (*((volatile unsigned int*) VO_CHI_BUF_reg)=data)
#define get_VO_CHI_BUF_reg   (*((volatile unsigned int*) VO_CHI_BUF_reg))
#define VO_CHI_BUF_inst_adr                                                          "0x00FD"
#define VO_CHI_BUF_inst                                                              0x00FD
#define VO_CHI_BUF_write_en2_shift                                                   (23)
#define VO_CHI_BUF_write_en2_mask                                                    (0x00800000)
#define VO_CHI_BUF_write_en2(data)                                                   (0x00800000&((data)<<23))
#define VO_CHI_BUF_write_en2_src(data)                                               ((0x00800000&(data))>>23)
#define VO_CHI_BUF_get_write_en2(data)                                               ((0x00800000&(data))>>23)
#define VO_CHI_BUF_th_shift                                                          (12)
#define VO_CHI_BUF_th_mask                                                           (0x007FF000)
#define VO_CHI_BUF_th(data)                                                          (0x007FF000&((data)<<12))
#define VO_CHI_BUF_th_src(data)                                                      ((0x007FF000&(data))>>12)
#define VO_CHI_BUF_get_th(data)                                                      ((0x007FF000&(data))>>12)
#define VO_CHI_BUF_write_en1_shift                                                   (10)
#define VO_CHI_BUF_write_en1_mask                                                    (0x00000400)
#define VO_CHI_BUF_write_en1(data)                                                   (0x00000400&((data)<<10))
#define VO_CHI_BUF_write_en1_src(data)                                               ((0x00000400&(data))>>10)
#define VO_CHI_BUF_get_write_en1(data)                                               ((0x00000400&(data))>>10)
#define VO_CHI_BUF_b_shift                                                           (0)
#define VO_CHI_BUF_b_mask                                                            (0x000003FF)
#define VO_CHI_BUF_b(data)                                                           (0x000003FF&((data)<<0))
#define VO_CHI_BUF_b_src(data)                                                       ((0x000003FF&(data))>>0)
#define VO_CHI_BUF_get_b(data)                                                       ((0x000003FF&(data))>>0)


#define VO_CH2_BUF                                                                   0x180057F8
#define VO_CH2_BUF_reg_addr                                                          "0xB80057F8"
#define VO_CH2_BUF_reg                                                               0xB80057F8
#define set_VO_CH2_BUF_reg(data)   (*((volatile unsigned int*) VO_CH2_BUF_reg)=data)
#define get_VO_CH2_BUF_reg   (*((volatile unsigned int*) VO_CH2_BUF_reg))
#define VO_CH2_BUF_inst_adr                                                          "0x00FE"
#define VO_CH2_BUF_inst                                                              0x00FE
#define VO_CH2_BUF_write_en2_shift                                                   (23)
#define VO_CH2_BUF_write_en2_mask                                                    (0x00800000)
#define VO_CH2_BUF_write_en2(data)                                                   (0x00800000&((data)<<23))
#define VO_CH2_BUF_write_en2_src(data)                                               ((0x00800000&(data))>>23)
#define VO_CH2_BUF_get_write_en2(data)                                               ((0x00800000&(data))>>23)
#define VO_CH2_BUF_th_shift                                                          (12)
#define VO_CH2_BUF_th_mask                                                           (0x007FF000)
#define VO_CH2_BUF_th(data)                                                          (0x007FF000&((data)<<12))
#define VO_CH2_BUF_th_src(data)                                                      ((0x007FF000&(data))>>12)
#define VO_CH2_BUF_get_th(data)                                                      ((0x007FF000&(data))>>12)
#define VO_CH2_BUF_write_en1_shift                                                   (10)
#define VO_CH2_BUF_write_en1_mask                                                    (0x00000400)
#define VO_CH2_BUF_write_en1(data)                                                   (0x00000400&((data)<<10))
#define VO_CH2_BUF_write_en1_src(data)                                               ((0x00000400&(data))>>10)
#define VO_CH2_BUF_get_write_en1(data)                                               ((0x00000400&(data))>>10)
#define VO_CH2_BUF_b_shift                                                           (0)
#define VO_CH2_BUF_b_mask                                                            (0x000003FF)
#define VO_CH2_BUF_b(data)                                                           (0x000003FF&((data)<<0))
#define VO_CH2_BUF_b_src(data)                                                       ((0x000003FF&(data))>>0)
#define VO_CH2_BUF_get_b(data)                                                       ((0x000003FF&(data))>>0)


#define VO_CH1                                                                       0x180057FC
#define VO_CH1_reg_addr                                                              "0xB80057FC"
#define VO_CH1_reg                                                                   0xB80057FC
#define set_VO_CH1_reg(data)   (*((volatile unsigned int*) VO_CH1_reg)=data)
#define get_VO_CH1_reg   (*((volatile unsigned int*) VO_CH1_reg))
#define VO_CH1_inst_adr                                                              "0x00FF"
#define VO_CH1_inst                                                                  0x00FF
#define VO_CH1_itop_shift                                                            (5)
#define VO_CH1_itop_mask                                                             (0x00000020)
#define VO_CH1_itop(data)                                                            (0x00000020&((data)<<5))
#define VO_CH1_itop_src(data)                                                        ((0x00000020&(data))>>5)
#define VO_CH1_get_itop(data)                                                        ((0x00000020&(data))>>5)
#define VO_CH1_ireint_shift                                                          (4)
#define VO_CH1_ireint_mask                                                           (0x00000010)
#define VO_CH1_ireint(data)                                                          (0x00000010&((data)<<4))
#define VO_CH1_ireint_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_CH1_get_ireint(data)                                                      ((0x00000010&(data))>>4)
#define VO_CH1_top_shift                                                             (2)
#define VO_CH1_top_mask                                                              (0x00000004)
#define VO_CH1_top(data)                                                             (0x00000004&((data)<<2))
#define VO_CH1_top_src(data)                                                         ((0x00000004&(data))>>2)
#define VO_CH1_get_top(data)                                                         ((0x00000004&(data))>>2)
#define VO_CH1_reint_shift                                                           (1)
#define VO_CH1_reint_mask                                                            (0x00000002)
#define VO_CH1_reint(data)                                                           (0x00000002&((data)<<1))
#define VO_CH1_reint_src(data)                                                       ((0x00000002&(data))>>1)
#define VO_CH1_get_reint(data)                                                       ((0x00000002&(data))>>1)
#define VO_CH1_write_data_shift                                                      (0)
#define VO_CH1_write_data_mask                                                       (0x00000001)
#define VO_CH1_write_data(data)                                                      (0x00000001&((data)<<0))
#define VO_CH1_write_data_src(data)                                                  ((0x00000001&(data))>>0)
#define VO_CH1_get_write_data(data)                                                  ((0x00000001&(data))>>0)


#define VO_CH2                                                                       0x18005800
#define VO_CH2_reg_addr                                                              "0xB8005800"
#define VO_CH2_reg                                                                   0xB8005800
#define set_VO_CH2_reg(data)   (*((volatile unsigned int*) VO_CH2_reg)=data)
#define get_VO_CH2_reg   (*((volatile unsigned int*) VO_CH2_reg))
#define VO_CH2_inst_adr                                                              "0x0000"
#define VO_CH2_inst                                                                  0x0000
#define VO_CH2_itop_shift                                                            (5)
#define VO_CH2_itop_mask                                                             (0x00000020)
#define VO_CH2_itop(data)                                                            (0x00000020&((data)<<5))
#define VO_CH2_itop_src(data)                                                        ((0x00000020&(data))>>5)
#define VO_CH2_get_itop(data)                                                        ((0x00000020&(data))>>5)
#define VO_CH2_ireint_shift                                                          (4)
#define VO_CH2_ireint_mask                                                           (0x00000010)
#define VO_CH2_ireint(data)                                                          (0x00000010&((data)<<4))
#define VO_CH2_ireint_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_CH2_get_ireint(data)                                                      ((0x00000010&(data))>>4)
#define VO_CH2_top_shift                                                             (2)
#define VO_CH2_top_mask                                                              (0x00000004)
#define VO_CH2_top(data)                                                             (0x00000004&((data)<<2))
#define VO_CH2_top_src(data)                                                         ((0x00000004&(data))>>2)
#define VO_CH2_get_top(data)                                                         ((0x00000004&(data))>>2)
#define VO_CH2_reint_shift                                                           (1)
#define VO_CH2_reint_mask                                                            (0x00000002)
#define VO_CH2_reint(data)                                                           (0x00000002&((data)<<1))
#define VO_CH2_reint_src(data)                                                       ((0x00000002&(data))>>1)
#define VO_CH2_get_reint(data)                                                       ((0x00000002&(data))>>1)
#define VO_CH2_write_data_shift                                                      (0)
#define VO_CH2_write_data_mask                                                       (0x00000001)
#define VO_CH2_write_data(data)                                                      (0x00000001&((data)<<0))
#define VO_CH2_write_data_src(data)                                                  ((0x00000001&(data))>>0)
#define VO_CH2_get_write_data(data)                                                  ((0x00000001&(data))>>0)


#define VO_PORT_P_CC1                                                                0x18005804
#define VO_PORT_P_CC1_reg_addr                                                       "0xB8005804"
#define VO_PORT_P_CC1_reg                                                            0xB8005804
#define set_VO_PORT_P_CC1_reg(data)   (*((volatile unsigned int*) VO_PORT_P_CC1_reg)=data)
#define get_VO_PORT_P_CC1_reg   (*((volatile unsigned int*) VO_PORT_P_CC1_reg))
#define VO_PORT_P_CC1_inst_adr                                                       "0x0001"
#define VO_PORT_P_CC1_inst                                                           0x0001
#define VO_PORT_P_CC1_m01_shift                                                      (16)
#define VO_PORT_P_CC1_m01_mask                                                       (0x0FFF0000)
#define VO_PORT_P_CC1_m01(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_P_CC1_m01_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC1_get_m01(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC1_m00_shift                                                      (0)
#define VO_PORT_P_CC1_m00_mask                                                       (0x00000FFF)
#define VO_PORT_P_CC1_m00(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_P_CC1_m00_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_P_CC1_get_m00(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_P_CC2                                                                0x18005808
#define VO_PORT_P_CC2_reg_addr                                                       "0xB8005808"
#define VO_PORT_P_CC2_reg                                                            0xB8005808
#define set_VO_PORT_P_CC2_reg(data)   (*((volatile unsigned int*) VO_PORT_P_CC2_reg)=data)
#define get_VO_PORT_P_CC2_reg   (*((volatile unsigned int*) VO_PORT_P_CC2_reg))
#define VO_PORT_P_CC2_inst_adr                                                       "0x0002"
#define VO_PORT_P_CC2_inst                                                           0x0002
#define VO_PORT_P_CC2_m10_shift                                                      (16)
#define VO_PORT_P_CC2_m10_mask                                                       (0x0FFF0000)
#define VO_PORT_P_CC2_m10(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_P_CC2_m10_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC2_get_m10(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC2_m02_shift                                                      (0)
#define VO_PORT_P_CC2_m02_mask                                                       (0x00000FFF)
#define VO_PORT_P_CC2_m02(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_P_CC2_m02_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_P_CC2_get_m02(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_P_CC3                                                                0x1800580C
#define VO_PORT_P_CC3_reg_addr                                                       "0xB800580C"
#define VO_PORT_P_CC3_reg                                                            0xB800580C
#define set_VO_PORT_P_CC3_reg(data)   (*((volatile unsigned int*) VO_PORT_P_CC3_reg)=data)
#define get_VO_PORT_P_CC3_reg   (*((volatile unsigned int*) VO_PORT_P_CC3_reg))
#define VO_PORT_P_CC3_inst_adr                                                       "0x0003"
#define VO_PORT_P_CC3_inst                                                           0x0003
#define VO_PORT_P_CC3_m12_shift                                                      (16)
#define VO_PORT_P_CC3_m12_mask                                                       (0x0FFF0000)
#define VO_PORT_P_CC3_m12(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_P_CC3_m12_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC3_get_m12(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC3_m11_shift                                                      (0)
#define VO_PORT_P_CC3_m11_mask                                                       (0x00000FFF)
#define VO_PORT_P_CC3_m11(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_P_CC3_m11_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_P_CC3_get_m11(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_P_CC4                                                                0x18005810
#define VO_PORT_P_CC4_reg_addr                                                       "0xB8005810"
#define VO_PORT_P_CC4_reg                                                            0xB8005810
#define set_VO_PORT_P_CC4_reg(data)   (*((volatile unsigned int*) VO_PORT_P_CC4_reg)=data)
#define get_VO_PORT_P_CC4_reg   (*((volatile unsigned int*) VO_PORT_P_CC4_reg))
#define VO_PORT_P_CC4_inst_adr                                                       "0x0004"
#define VO_PORT_P_CC4_inst                                                           0x0004
#define VO_PORT_P_CC4_m21_shift                                                      (16)
#define VO_PORT_P_CC4_m21_mask                                                       (0x0FFF0000)
#define VO_PORT_P_CC4_m21(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_P_CC4_m21_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC4_get_m21(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_P_CC4_m20_shift                                                      (0)
#define VO_PORT_P_CC4_m20_mask                                                       (0x00000FFF)
#define VO_PORT_P_CC4_m20(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_P_CC4_m20_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_P_CC4_get_m20(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_P_CC5                                                                0x18005814
#define VO_PORT_P_CC5_reg_addr                                                       "0xB8005814"
#define VO_PORT_P_CC5_reg                                                            0xB8005814
#define set_VO_PORT_P_CC5_reg(data)   (*((volatile unsigned int*) VO_PORT_P_CC5_reg)=data)
#define get_VO_PORT_P_CC5_reg   (*((volatile unsigned int*) VO_PORT_P_CC5_reg))
#define VO_PORT_P_CC5_inst_adr                                                       "0x0005"
#define VO_PORT_P_CC5_inst                                                           0x0005
#define VO_PORT_P_CC5_m22_shift                                                      (0)
#define VO_PORT_P_CC5_m22_mask                                                       (0x00000FFF)
#define VO_PORT_P_CC5_m22(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_P_CC5_m22_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_P_CC5_get_m22(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_P_CC6                                                                0x18005818
#define VO_PORT_P_CC6_reg_addr                                                       "0xB8005818"
#define VO_PORT_P_CC6_reg                                                            0xB8005818
#define set_VO_PORT_P_CC6_reg(data)   (*((volatile unsigned int*) VO_PORT_P_CC6_reg)=data)
#define get_VO_PORT_P_CC6_reg   (*((volatile unsigned int*) VO_PORT_P_CC6_reg))
#define VO_PORT_P_CC6_inst_adr                                                       "0x0006"
#define VO_PORT_P_CC6_inst                                                           0x0006
#define VO_PORT_P_CC6_a2_shift                                                       (18)
#define VO_PORT_P_CC6_a2_mask                                                        (0x07FC0000)
#define VO_PORT_P_CC6_a2(data)                                                       (0x07FC0000&((data)<<18))
#define VO_PORT_P_CC6_a2_src(data)                                                   ((0x07FC0000&(data))>>18)
#define VO_PORT_P_CC6_get_a2(data)                                                   ((0x07FC0000&(data))>>18)
#define VO_PORT_P_CC6_a1_shift                                                       (9)
#define VO_PORT_P_CC6_a1_mask                                                        (0x0003FE00)
#define VO_PORT_P_CC6_a1(data)                                                       (0x0003FE00&((data)<<9))
#define VO_PORT_P_CC6_a1_src(data)                                                   ((0x0003FE00&(data))>>9)
#define VO_PORT_P_CC6_get_a1(data)                                                   ((0x0003FE00&(data))>>9)
#define VO_PORT_P_CC6_a0_shift                                                       (0)
#define VO_PORT_P_CC6_a0_mask                                                        (0x000001FF)
#define VO_PORT_P_CC6_a0(data)                                                       (0x000001FF&((data)<<0))
#define VO_PORT_P_CC6_a0_src(data)                                                   ((0x000001FF&(data))>>0)
#define VO_PORT_P_CC6_get_a0(data)                                                   ((0x000001FF&(data))>>0)


#define VO_PORT_P_LMT                                                                0x1800581C
#define VO_PORT_P_LMT_reg_addr                                                       "0xB800581C"
#define VO_PORT_P_LMT_reg                                                            0xB800581C
#define set_VO_PORT_P_LMT_reg(data)   (*((volatile unsigned int*) VO_PORT_P_LMT_reg)=data)
#define get_VO_PORT_P_LMT_reg   (*((volatile unsigned int*) VO_PORT_P_LMT_reg))
#define VO_PORT_P_LMT_inst_adr                                                       "0x0007"
#define VO_PORT_P_LMT_inst                                                           0x0007
#define VO_PORT_P_LMT_Cmax_shift                                                     (24)
#define VO_PORT_P_LMT_Cmax_mask                                                      (0xFF000000)
#define VO_PORT_P_LMT_Cmax(data)                                                     (0xFF000000&((data)<<24))
#define VO_PORT_P_LMT_Cmax_src(data)                                                 ((0xFF000000&(data))>>24)
#define VO_PORT_P_LMT_get_Cmax(data)                                                 ((0xFF000000&(data))>>24)
#define VO_PORT_P_LMT_Cmin_shift                                                     (16)
#define VO_PORT_P_LMT_Cmin_mask                                                      (0x00FF0000)
#define VO_PORT_P_LMT_Cmin(data)                                                     (0x00FF0000&((data)<<16))
#define VO_PORT_P_LMT_Cmin_src(data)                                                 ((0x00FF0000&(data))>>16)
#define VO_PORT_P_LMT_get_Cmin(data)                                                 ((0x00FF0000&(data))>>16)
#define VO_PORT_P_LMT_Ymax_shift                                                     (8)
#define VO_PORT_P_LMT_Ymax_mask                                                      (0x0000FF00)
#define VO_PORT_P_LMT_Ymax(data)                                                     (0x0000FF00&((data)<<8))
#define VO_PORT_P_LMT_Ymax_src(data)                                                 ((0x0000FF00&(data))>>8)
#define VO_PORT_P_LMT_get_Ymax(data)                                                 ((0x0000FF00&(data))>>8)
#define VO_PORT_P_LMT_Ymin_shift                                                     (0)
#define VO_PORT_P_LMT_Ymin_mask                                                      (0x000000FF)
#define VO_PORT_P_LMT_Ymin(data)                                                     (0x000000FF&((data)<<0))
#define VO_PORT_P_LMT_Ymin_src(data)                                                 ((0x000000FF&(data))>>0)
#define VO_PORT_P_LMT_get_Ymin(data)                                                 ((0x000000FF&(data))>>0)


#define VO_PORT_I_CC1                                                                0x18005820
#define VO_PORT_I_CC1_reg_addr                                                       "0xB8005820"
#define VO_PORT_I_CC1_reg                                                            0xB8005820
#define set_VO_PORT_I_CC1_reg(data)   (*((volatile unsigned int*) VO_PORT_I_CC1_reg)=data)
#define get_VO_PORT_I_CC1_reg   (*((volatile unsigned int*) VO_PORT_I_CC1_reg))
#define VO_PORT_I_CC1_inst_adr                                                       "0x0008"
#define VO_PORT_I_CC1_inst                                                           0x0008
#define VO_PORT_I_CC1_m01_shift                                                      (16)
#define VO_PORT_I_CC1_m01_mask                                                       (0x0FFF0000)
#define VO_PORT_I_CC1_m01(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_I_CC1_m01_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC1_get_m01(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC1_m00_shift                                                      (0)
#define VO_PORT_I_CC1_m00_mask                                                       (0x00000FFF)
#define VO_PORT_I_CC1_m00(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_I_CC1_m00_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_I_CC1_get_m00(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_I_CC2                                                                0x18005824
#define VO_PORT_I_CC2_reg_addr                                                       "0xB8005824"
#define VO_PORT_I_CC2_reg                                                            0xB8005824
#define set_VO_PORT_I_CC2_reg(data)   (*((volatile unsigned int*) VO_PORT_I_CC2_reg)=data)
#define get_VO_PORT_I_CC2_reg   (*((volatile unsigned int*) VO_PORT_I_CC2_reg))
#define VO_PORT_I_CC2_inst_adr                                                       "0x0009"
#define VO_PORT_I_CC2_inst                                                           0x0009
#define VO_PORT_I_CC2_m10_shift                                                      (16)
#define VO_PORT_I_CC2_m10_mask                                                       (0x0FFF0000)
#define VO_PORT_I_CC2_m10(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_I_CC2_m10_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC2_get_m10(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC2_m02_shift                                                      (0)
#define VO_PORT_I_CC2_m02_mask                                                       (0x00000FFF)
#define VO_PORT_I_CC2_m02(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_I_CC2_m02_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_I_CC2_get_m02(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_I_CC3                                                                0x18005828
#define VO_PORT_I_CC3_reg_addr                                                       "0xB8005828"
#define VO_PORT_I_CC3_reg                                                            0xB8005828
#define set_VO_PORT_I_CC3_reg(data)   (*((volatile unsigned int*) VO_PORT_I_CC3_reg)=data)
#define get_VO_PORT_I_CC3_reg   (*((volatile unsigned int*) VO_PORT_I_CC3_reg))
#define VO_PORT_I_CC3_inst_adr                                                       "0x000A"
#define VO_PORT_I_CC3_inst                                                           0x000A
#define VO_PORT_I_CC3_m12_shift                                                      (16)
#define VO_PORT_I_CC3_m12_mask                                                       (0x0FFF0000)
#define VO_PORT_I_CC3_m12(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_I_CC3_m12_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC3_get_m12(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC3_m11_shift                                                      (0)
#define VO_PORT_I_CC3_m11_mask                                                       (0x00000FFF)
#define VO_PORT_I_CC3_m11(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_I_CC3_m11_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_I_CC3_get_m11(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_I_CC4                                                                0x1800582C
#define VO_PORT_I_CC4_reg_addr                                                       "0xB800582C"
#define VO_PORT_I_CC4_reg                                                            0xB800582C
#define set_VO_PORT_I_CC4_reg(data)   (*((volatile unsigned int*) VO_PORT_I_CC4_reg)=data)
#define get_VO_PORT_I_CC4_reg   (*((volatile unsigned int*) VO_PORT_I_CC4_reg))
#define VO_PORT_I_CC4_inst_adr                                                       "0x000B"
#define VO_PORT_I_CC4_inst                                                           0x000B
#define VO_PORT_I_CC4_m21_shift                                                      (16)
#define VO_PORT_I_CC4_m21_mask                                                       (0x0FFF0000)
#define VO_PORT_I_CC4_m21(data)                                                      (0x0FFF0000&((data)<<16))
#define VO_PORT_I_CC4_m21_src(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC4_get_m21(data)                                                  ((0x0FFF0000&(data))>>16)
#define VO_PORT_I_CC4_m20_shift                                                      (0)
#define VO_PORT_I_CC4_m20_mask                                                       (0x00000FFF)
#define VO_PORT_I_CC4_m20(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_I_CC4_m20_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_I_CC4_get_m20(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_I_CC5                                                                0x18005830
#define VO_PORT_I_CC5_reg_addr                                                       "0xB8005830"
#define VO_PORT_I_CC5_reg                                                            0xB8005830
#define set_VO_PORT_I_CC5_reg(data)   (*((volatile unsigned int*) VO_PORT_I_CC5_reg)=data)
#define get_VO_PORT_I_CC5_reg   (*((volatile unsigned int*) VO_PORT_I_CC5_reg))
#define VO_PORT_I_CC5_inst_adr                                                       "0x000C"
#define VO_PORT_I_CC5_inst                                                           0x000C
#define VO_PORT_I_CC5_m22_shift                                                      (0)
#define VO_PORT_I_CC5_m22_mask                                                       (0x00000FFF)
#define VO_PORT_I_CC5_m22(data)                                                      (0x00000FFF&((data)<<0))
#define VO_PORT_I_CC5_m22_src(data)                                                  ((0x00000FFF&(data))>>0)
#define VO_PORT_I_CC5_get_m22(data)                                                  ((0x00000FFF&(data))>>0)


#define VO_PORT_I_CC6                                                                0x18005834
#define VO_PORT_I_CC6_reg_addr                                                       "0xB8005834"
#define VO_PORT_I_CC6_reg                                                            0xB8005834
#define set_VO_PORT_I_CC6_reg(data)   (*((volatile unsigned int*) VO_PORT_I_CC6_reg)=data)
#define get_VO_PORT_I_CC6_reg   (*((volatile unsigned int*) VO_PORT_I_CC6_reg))
#define VO_PORT_I_CC6_inst_adr                                                       "0x000D"
#define VO_PORT_I_CC6_inst                                                           0x000D
#define VO_PORT_I_CC6_a2_shift                                                       (18)
#define VO_PORT_I_CC6_a2_mask                                                        (0x07FC0000)
#define VO_PORT_I_CC6_a2(data)                                                       (0x07FC0000&((data)<<18))
#define VO_PORT_I_CC6_a2_src(data)                                                   ((0x07FC0000&(data))>>18)
#define VO_PORT_I_CC6_get_a2(data)                                                   ((0x07FC0000&(data))>>18)
#define VO_PORT_I_CC6_a1_shift                                                       (9)
#define VO_PORT_I_CC6_a1_mask                                                        (0x0003FE00)
#define VO_PORT_I_CC6_a1(data)                                                       (0x0003FE00&((data)<<9))
#define VO_PORT_I_CC6_a1_src(data)                                                   ((0x0003FE00&(data))>>9)
#define VO_PORT_I_CC6_get_a1(data)                                                   ((0x0003FE00&(data))>>9)
#define VO_PORT_I_CC6_a0_shift                                                       (0)
#define VO_PORT_I_CC6_a0_mask                                                        (0x000001FF)
#define VO_PORT_I_CC6_a0(data)                                                       (0x000001FF&((data)<<0))
#define VO_PORT_I_CC6_a0_src(data)                                                   ((0x000001FF&(data))>>0)
#define VO_PORT_I_CC6_get_a0(data)                                                   ((0x000001FF&(data))>>0)


#define VO_PORT_I_LMT                                                                0x18005838
#define VO_PORT_I_LMT_reg_addr                                                       "0xB8005838"
#define VO_PORT_I_LMT_reg                                                            0xB8005838
#define set_VO_PORT_I_LMT_reg(data)   (*((volatile unsigned int*) VO_PORT_I_LMT_reg)=data)
#define get_VO_PORT_I_LMT_reg   (*((volatile unsigned int*) VO_PORT_I_LMT_reg))
#define VO_PORT_I_LMT_inst_adr                                                       "0x000E"
#define VO_PORT_I_LMT_inst                                                           0x000E
#define VO_PORT_I_LMT_Cmax_shift                                                     (24)
#define VO_PORT_I_LMT_Cmax_mask                                                      (0xFF000000)
#define VO_PORT_I_LMT_Cmax(data)                                                     (0xFF000000&((data)<<24))
#define VO_PORT_I_LMT_Cmax_src(data)                                                 ((0xFF000000&(data))>>24)
#define VO_PORT_I_LMT_get_Cmax(data)                                                 ((0xFF000000&(data))>>24)
#define VO_PORT_I_LMT_Cmin_shift                                                     (16)
#define VO_PORT_I_LMT_Cmin_mask                                                      (0x00FF0000)
#define VO_PORT_I_LMT_Cmin(data)                                                     (0x00FF0000&((data)<<16))
#define VO_PORT_I_LMT_Cmin_src(data)                                                 ((0x00FF0000&(data))>>16)
#define VO_PORT_I_LMT_get_Cmin(data)                                                 ((0x00FF0000&(data))>>16)
#define VO_PORT_I_LMT_Ymax_shift                                                     (8)
#define VO_PORT_I_LMT_Ymax_mask                                                      (0x0000FF00)
#define VO_PORT_I_LMT_Ymax(data)                                                     (0x0000FF00&((data)<<8))
#define VO_PORT_I_LMT_Ymax_src(data)                                                 ((0x0000FF00&(data))>>8)
#define VO_PORT_I_LMT_get_Ymax(data)                                                 ((0x0000FF00&(data))>>8)
#define VO_PORT_I_LMT_Ymin_shift                                                     (0)
#define VO_PORT_I_LMT_Ymin_mask                                                      (0x000000FF)
#define VO_PORT_I_LMT_Ymin(data)                                                     (0x000000FF&((data)<<0))
#define VO_PORT_I_LMT_Ymin_src(data)                                                 ((0x000000FF&(data))>>0)
#define VO_PORT_I_LMT_get_Ymin(data)                                                 ((0x000000FF&(data))>>0)


#define VO_PORT_H_CC1                                                                0x1800583C
#define VO_PORT_H_CC1_reg_addr                                                       "0xB800583C"
#define VO_PORT_H_CC1_reg                                                            0xB800583C
#define set_VO_PORT_H_CC1_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC1_reg)=data)
#define get_VO_PORT_H_CC1_reg   (*((volatile unsigned int*) VO_PORT_H_CC1_reg))
#define VO_PORT_H_CC1_inst_adr                                                       "0x000F"
#define VO_PORT_H_CC1_inst                                                           0x000F
#define VO_PORT_H_CC1_m01_shift                                                      (16)
#define VO_PORT_H_CC1_m01_mask                                                       (0xFFFF0000)
#define VO_PORT_H_CC1_m01(data)                                                      (0xFFFF0000&((data)<<16))
#define VO_PORT_H_CC1_m01_src(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC1_get_m01(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC1_m00_shift                                                      (0)
#define VO_PORT_H_CC1_m00_mask                                                       (0x0000FFFF)
#define VO_PORT_H_CC1_m00(data)                                                      (0x0000FFFF&((data)<<0))
#define VO_PORT_H_CC1_m00_src(data)                                                  ((0x0000FFFF&(data))>>0)
#define VO_PORT_H_CC1_get_m00(data)                                                  ((0x0000FFFF&(data))>>0)


#define VO_PORT_H_CC2                                                                0x18005840
#define VO_PORT_H_CC2_reg_addr                                                       "0xB8005840"
#define VO_PORT_H_CC2_reg                                                            0xB8005840
#define set_VO_PORT_H_CC2_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC2_reg)=data)
#define get_VO_PORT_H_CC2_reg   (*((volatile unsigned int*) VO_PORT_H_CC2_reg))
#define VO_PORT_H_CC2_inst_adr                                                       "0x0010"
#define VO_PORT_H_CC2_inst                                                           0x0010
#define VO_PORT_H_CC2_m10_shift                                                      (16)
#define VO_PORT_H_CC2_m10_mask                                                       (0xFFFF0000)
#define VO_PORT_H_CC2_m10(data)                                                      (0xFFFF0000&((data)<<16))
#define VO_PORT_H_CC2_m10_src(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC2_get_m10(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC2_m02_shift                                                      (0)
#define VO_PORT_H_CC2_m02_mask                                                       (0x0000FFFF)
#define VO_PORT_H_CC2_m02(data)                                                      (0x0000FFFF&((data)<<0))
#define VO_PORT_H_CC2_m02_src(data)                                                  ((0x0000FFFF&(data))>>0)
#define VO_PORT_H_CC2_get_m02(data)                                                  ((0x0000FFFF&(data))>>0)


#define VO_PORT_H_CC3                                                                0x18005844
#define VO_PORT_H_CC3_reg_addr                                                       "0xB8005844"
#define VO_PORT_H_CC3_reg                                                            0xB8005844
#define set_VO_PORT_H_CC3_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC3_reg)=data)
#define get_VO_PORT_H_CC3_reg   (*((volatile unsigned int*) VO_PORT_H_CC3_reg))
#define VO_PORT_H_CC3_inst_adr                                                       "0x0011"
#define VO_PORT_H_CC3_inst                                                           0x0011
#define VO_PORT_H_CC3_m12_shift                                                      (16)
#define VO_PORT_H_CC3_m12_mask                                                       (0xFFFF0000)
#define VO_PORT_H_CC3_m12(data)                                                      (0xFFFF0000&((data)<<16))
#define VO_PORT_H_CC3_m12_src(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC3_get_m12(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC3_m11_shift                                                      (0)
#define VO_PORT_H_CC3_m11_mask                                                       (0x0000FFFF)
#define VO_PORT_H_CC3_m11(data)                                                      (0x0000FFFF&((data)<<0))
#define VO_PORT_H_CC3_m11_src(data)                                                  ((0x0000FFFF&(data))>>0)
#define VO_PORT_H_CC3_get_m11(data)                                                  ((0x0000FFFF&(data))>>0)


#define VO_PORT_H_CC4                                                                0x18005848
#define VO_PORT_H_CC4_reg_addr                                                       "0xB8005848"
#define VO_PORT_H_CC4_reg                                                            0xB8005848
#define set_VO_PORT_H_CC4_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC4_reg)=data)
#define get_VO_PORT_H_CC4_reg   (*((volatile unsigned int*) VO_PORT_H_CC4_reg))
#define VO_PORT_H_CC4_inst_adr                                                       "0x0012"
#define VO_PORT_H_CC4_inst                                                           0x0012
#define VO_PORT_H_CC4_m21_shift                                                      (16)
#define VO_PORT_H_CC4_m21_mask                                                       (0xFFFF0000)
#define VO_PORT_H_CC4_m21(data)                                                      (0xFFFF0000&((data)<<16))
#define VO_PORT_H_CC4_m21_src(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC4_get_m21(data)                                                  ((0xFFFF0000&(data))>>16)
#define VO_PORT_H_CC4_m20_shift                                                      (0)
#define VO_PORT_H_CC4_m20_mask                                                       (0x0000FFFF)
#define VO_PORT_H_CC4_m20(data)                                                      (0x0000FFFF&((data)<<0))
#define VO_PORT_H_CC4_m20_src(data)                                                  ((0x0000FFFF&(data))>>0)
#define VO_PORT_H_CC4_get_m20(data)                                                  ((0x0000FFFF&(data))>>0)


#define VO_PORT_H_CC5                                                                0x1800584C
#define VO_PORT_H_CC5_reg_addr                                                       "0xB800584C"
#define VO_PORT_H_CC5_reg                                                            0xB800584C
#define set_VO_PORT_H_CC5_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC5_reg)=data)
#define get_VO_PORT_H_CC5_reg   (*((volatile unsigned int*) VO_PORT_H_CC5_reg))
#define VO_PORT_H_CC5_inst_adr                                                       "0x0013"
#define VO_PORT_H_CC5_inst                                                           0x0013
#define VO_PORT_H_CC5_m22_shift                                                      (0)
#define VO_PORT_H_CC5_m22_mask                                                       (0x0000FFFF)
#define VO_PORT_H_CC5_m22(data)                                                      (0x0000FFFF&((data)<<0))
#define VO_PORT_H_CC5_m22_src(data)                                                  ((0x0000FFFF&(data))>>0)
#define VO_PORT_H_CC5_get_m22(data)                                                  ((0x0000FFFF&(data))>>0)


#define VO_PORT_H_CC6                                                                0x18005850
#define VO_PORT_H_CC6_reg_addr                                                       "0xB8005850"
#define VO_PORT_H_CC6_reg                                                            0xB8005850
#define set_VO_PORT_H_CC6_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC6_reg)=data)
#define get_VO_PORT_H_CC6_reg   (*((volatile unsigned int*) VO_PORT_H_CC6_reg))
#define VO_PORT_H_CC6_inst_adr                                                       "0x0014"
#define VO_PORT_H_CC6_inst                                                           0x0014
#define VO_PORT_H_CC6_a1_shift                                                       (16)
#define VO_PORT_H_CC6_a1_mask                                                        (0x1FFF0000)
#define VO_PORT_H_CC6_a1(data)                                                       (0x1FFF0000&((data)<<16))
#define VO_PORT_H_CC6_a1_src(data)                                                   ((0x1FFF0000&(data))>>16)
#define VO_PORT_H_CC6_get_a1(data)                                                   ((0x1FFF0000&(data))>>16)
#define VO_PORT_H_CC6_a0_shift                                                       (0)
#define VO_PORT_H_CC6_a0_mask                                                        (0x00001FFF)
#define VO_PORT_H_CC6_a0(data)                                                       (0x00001FFF&((data)<<0))
#define VO_PORT_H_CC6_a0_src(data)                                                   ((0x00001FFF&(data))>>0)
#define VO_PORT_H_CC6_get_a0(data)                                                   ((0x00001FFF&(data))>>0)


#define VO_PORT_H_CC7                                                                0x18005854
#define VO_PORT_H_CC7_reg_addr                                                       "0xB8005854"
#define VO_PORT_H_CC7_reg                                                            0xB8005854
#define set_VO_PORT_H_CC7_reg(data)   (*((volatile unsigned int*) VO_PORT_H_CC7_reg)=data)
#define get_VO_PORT_H_CC7_reg   (*((volatile unsigned int*) VO_PORT_H_CC7_reg))
#define VO_PORT_H_CC7_inst_adr                                                       "0x0015"
#define VO_PORT_H_CC7_inst                                                           0x0015
#define VO_PORT_H_CC7_a2_shift                                                       (0)
#define VO_PORT_H_CC7_a2_mask                                                        (0x00001FFF)
#define VO_PORT_H_CC7_a2(data)                                                       (0x00001FFF&((data)<<0))
#define VO_PORT_H_CC7_a2_src(data)                                                   ((0x00001FFF&(data))>>0)
#define VO_PORT_H_CC7_get_a2(data)                                                   ((0x00001FFF&(data))>>0)


#define VO_PORT_H_LMT_Y                                                              0x18005858
#define VO_PORT_H_LMT_Y_reg_addr                                                     "0xB8005858"
#define VO_PORT_H_LMT_Y_reg                                                          0xB8005858
#define set_VO_PORT_H_LMT_Y_reg(data)   (*((volatile unsigned int*) VO_PORT_H_LMT_Y_reg)=data)
#define get_VO_PORT_H_LMT_Y_reg   (*((volatile unsigned int*) VO_PORT_H_LMT_Y_reg))
#define VO_PORT_H_LMT_Y_inst_adr                                                     "0x0016"
#define VO_PORT_H_LMT_Y_inst                                                         0x0016
#define VO_PORT_H_LMT_Y_Ymax_shift                                                   (16)
#define VO_PORT_H_LMT_Y_Ymax_mask                                                    (0x0FFF0000)
#define VO_PORT_H_LMT_Y_Ymax(data)                                                   (0x0FFF0000&((data)<<16))
#define VO_PORT_H_LMT_Y_Ymax_src(data)                                               ((0x0FFF0000&(data))>>16)
#define VO_PORT_H_LMT_Y_get_Ymax(data)                                               ((0x0FFF0000&(data))>>16)
#define VO_PORT_H_LMT_Y_Ymin_shift                                                   (0)
#define VO_PORT_H_LMT_Y_Ymin_mask                                                    (0x00000FFF)
#define VO_PORT_H_LMT_Y_Ymin(data)                                                   (0x00000FFF&((data)<<0))
#define VO_PORT_H_LMT_Y_Ymin_src(data)                                               ((0x00000FFF&(data))>>0)
#define VO_PORT_H_LMT_Y_get_Ymin(data)                                               ((0x00000FFF&(data))>>0)


#define VO_PORT_H_LMT_C                                                              0x1800585C
#define VO_PORT_H_LMT_C_reg_addr                                                     "0xB800585C"
#define VO_PORT_H_LMT_C_reg                                                          0xB800585C
#define set_VO_PORT_H_LMT_C_reg(data)   (*((volatile unsigned int*) VO_PORT_H_LMT_C_reg)=data)
#define get_VO_PORT_H_LMT_C_reg   (*((volatile unsigned int*) VO_PORT_H_LMT_C_reg))
#define VO_PORT_H_LMT_C_inst_adr                                                     "0x0017"
#define VO_PORT_H_LMT_C_inst                                                         0x0017
#define VO_PORT_H_LMT_C_Cmax_shift                                                   (16)
#define VO_PORT_H_LMT_C_Cmax_mask                                                    (0x0FFF0000)
#define VO_PORT_H_LMT_C_Cmax(data)                                                   (0x0FFF0000&((data)<<16))
#define VO_PORT_H_LMT_C_Cmax_src(data)                                               ((0x0FFF0000&(data))>>16)
#define VO_PORT_H_LMT_C_get_Cmax(data)                                               ((0x0FFF0000&(data))>>16)
#define VO_PORT_H_LMT_C_Cmin_shift                                                   (0)
#define VO_PORT_H_LMT_C_Cmin_mask                                                    (0x00000FFF)
#define VO_PORT_H_LMT_C_Cmin(data)                                                   (0x00000FFF&((data)<<0))
#define VO_PORT_H_LMT_C_Cmin_src(data)                                               ((0x00000FFF&(data))>>0)
#define VO_PORT_H_LMT_C_get_Cmin(data)                                               ((0x00000FFF&(data))>>0)


#define VO_INTEN                                                                     0x18005860
#define VO_INTEN_reg_addr                                                            "0xB8005860"
#define VO_INTEN_reg                                                                 0xB8005860
#define set_VO_INTEN_reg(data)   (*((volatile unsigned int*) VO_INTEN_reg)=data)
#define get_VO_INTEN_reg   (*((volatile unsigned int*) VO_INTEN_reg))
#define VO_INTEN_inst_adr                                                            "0x0018"
#define VO_INTEN_inst                                                                0x0018
#define VO_INTEN_h_under_shift                                                       (7)
#define VO_INTEN_h_under_mask                                                        (0x00000080)
#define VO_INTEN_h_under(data)                                                       (0x00000080&((data)<<7))
#define VO_INTEN_h_under_src(data)                                                   ((0x00000080&(data))>>7)
#define VO_INTEN_get_h_under(data)                                                   ((0x00000080&(data))>>7)
#define VO_INTEN_i_under_shift                                                       (6)
#define VO_INTEN_i_under_mask                                                        (0x00000040)
#define VO_INTEN_i_under(data)                                                       (0x00000040&((data)<<6))
#define VO_INTEN_i_under_src(data)                                                   ((0x00000040&(data))>>6)
#define VO_INTEN_get_i_under(data)                                                   ((0x00000040&(data))>>6)
#define VO_INTEN_p_under_shift                                                       (5)
#define VO_INTEN_p_under_mask                                                        (0x00000020)
#define VO_INTEN_p_under(data)                                                       (0x00000020&((data)<<5))
#define VO_INTEN_p_under_src(data)                                                   ((0x00000020&(data))>>5)
#define VO_INTEN_get_p_under(data)                                                   ((0x00000020&(data))>>5)
#define VO_INTEN_sub1_shift                                                          (4)
#define VO_INTEN_sub1_mask                                                           (0x00000010)
#define VO_INTEN_sub1(data)                                                          (0x00000010&((data)<<4))
#define VO_INTEN_sub1_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_INTEN_get_sub1(data)                                                      ((0x00000010&(data))>>4)
#define VO_INTEN_wbfin_shift                                                         (3)
#define VO_INTEN_wbfin_mask                                                          (0x00000008)
#define VO_INTEN_wbfin(data)                                                         (0x00000008&((data)<<3))
#define VO_INTEN_wbfin_src(data)                                                     ((0x00000008&(data))>>3)
#define VO_INTEN_get_wbfin(data)                                                     ((0x00000008&(data))>>3)
#define VO_INTEN_m2fin_shift                                                         (2)
#define VO_INTEN_m2fin_mask                                                          (0x00000004)
#define VO_INTEN_m2fin(data)                                                         (0x00000004&((data)<<2))
#define VO_INTEN_m2fin_src(data)                                                     ((0x00000004&(data))>>2)
#define VO_INTEN_get_m2fin(data)                                                     ((0x00000004&(data))>>2)
#define VO_INTEN_m1fin_shift                                                         (1)
#define VO_INTEN_m1fin_mask                                                          (0x00000002)
#define VO_INTEN_m1fin(data)                                                         (0x00000002&((data)<<1))
#define VO_INTEN_m1fin_src(data)                                                     ((0x00000002&(data))>>1)
#define VO_INTEN_get_m1fin(data)                                                     ((0x00000002&(data))>>1)
#define VO_INTEN_write_data_shift                                                    (0)
#define VO_INTEN_write_data_mask                                                     (0x00000001)
#define VO_INTEN_write_data(data)                                                    (0x00000001&((data)<<0))
#define VO_INTEN_write_data_src(data)                                                ((0x00000001&(data))>>0)
#define VO_INTEN_get_write_data(data)                                                ((0x00000001&(data))>>0)


#define VO_INTST                                                                     0x18005864
#define VO_INTST_reg_addr                                                            "0xB8005864"
#define VO_INTST_reg                                                                 0xB8005864
#define set_VO_INTST_reg(data)   (*((volatile unsigned int*) VO_INTST_reg)=data)
#define get_VO_INTST_reg   (*((volatile unsigned int*) VO_INTST_reg))
#define VO_INTST_inst_adr                                                            "0x0019"
#define VO_INTST_inst                                                                0x0019
#define VO_INTST_h_under_shift                                                       (7)
#define VO_INTST_h_under_mask                                                        (0x00000080)
#define VO_INTST_h_under(data)                                                       (0x00000080&((data)<<7))
#define VO_INTST_h_under_src(data)                                                   ((0x00000080&(data))>>7)
#define VO_INTST_get_h_under(data)                                                   ((0x00000080&(data))>>7)
#define VO_INTST_i_under_shift                                                       (6)
#define VO_INTST_i_under_mask                                                        (0x00000040)
#define VO_INTST_i_under(data)                                                       (0x00000040&((data)<<6))
#define VO_INTST_i_under_src(data)                                                   ((0x00000040&(data))>>6)
#define VO_INTST_get_i_under(data)                                                   ((0x00000040&(data))>>6)
#define VO_INTST_p_under_shift                                                       (5)
#define VO_INTST_p_under_mask                                                        (0x00000020)
#define VO_INTST_p_under(data)                                                       (0x00000020&((data)<<5))
#define VO_INTST_p_under_src(data)                                                   ((0x00000020&(data))>>5)
#define VO_INTST_get_p_under(data)                                                   ((0x00000020&(data))>>5)
#define VO_INTST_sub1_shift                                                          (4)
#define VO_INTST_sub1_mask                                                           (0x00000010)
#define VO_INTST_sub1(data)                                                          (0x00000010&((data)<<4))
#define VO_INTST_sub1_src(data)                                                      ((0x00000010&(data))>>4)
#define VO_INTST_get_sub1(data)                                                      ((0x00000010&(data))>>4)
#define VO_INTST_wbfin_shift                                                         (3)
#define VO_INTST_wbfin_mask                                                          (0x00000008)
#define VO_INTST_wbfin(data)                                                         (0x00000008&((data)<<3))
#define VO_INTST_wbfin_src(data)                                                     ((0x00000008&(data))>>3)
#define VO_INTST_get_wbfin(data)                                                     ((0x00000008&(data))>>3)
#define VO_INTST_m2fin_shift                                                         (2)
#define VO_INTST_m2fin_mask                                                          (0x00000004)
#define VO_INTST_m2fin(data)                                                         (0x00000004&((data)<<2))
#define VO_INTST_m2fin_src(data)                                                     ((0x00000004&(data))>>2)
#define VO_INTST_get_m2fin(data)                                                     ((0x00000004&(data))>>2)
#define VO_INTST_m1fin_shift                                                         (1)
#define VO_INTST_m1fin_mask                                                          (0x00000002)
#define VO_INTST_m1fin(data)                                                         (0x00000002&((data)<<1))
#define VO_INTST_m1fin_src(data)                                                     ((0x00000002&(data))>>1)
#define VO_INTST_get_m1fin(data)                                                     ((0x00000002&(data))>>1)
#define VO_INTST_write_data_shift                                                    (0)
#define VO_INTST_write_data_mask                                                     (0x00000001)
#define VO_INTST_write_data(data)                                                    (0x00000001&((data)<<0))
#define VO_INTST_write_data_src(data)                                                ((0x00000001&(data))>>0)
#define VO_INTST_get_write_data(data)                                                ((0x00000001&(data))>>0)


#define VO_STATUS                                                                    0x18005868
#define VO_STATUS_reg_addr                                                           "0xB8005868"
#define VO_STATUS_reg                                                                0xB8005868
#define set_VO_STATUS_reg(data)   (*((volatile unsigned int*) VO_STATUS_reg)=data)
#define get_VO_STATUS_reg   (*((volatile unsigned int*) VO_STATUS_reg))
#define VO_STATUS_inst_adr                                                           "0x001A"
#define VO_STATUS_inst                                                               0x001A
#define VO_STATUS_tve_h_fetch_shift                                                  (13)
#define VO_STATUS_tve_h_fetch_mask                                                   (0x00002000)
#define VO_STATUS_tve_h_fetch(data)                                                  (0x00002000&((data)<<13))
#define VO_STATUS_tve_h_fetch_src(data)                                              ((0x00002000&(data))>>13)
#define VO_STATUS_get_tve_h_fetch(data)                                              ((0x00002000&(data))>>13)
#define VO_STATUS_tve_i_fetch_shift                                                  (12)
#define VO_STATUS_tve_i_fetch_mask                                                   (0x00001000)
#define VO_STATUS_tve_i_fetch(data)                                                  (0x00001000&((data)<<12))
#define VO_STATUS_tve_i_fetch_src(data)                                              ((0x00001000&(data))>>12)
#define VO_STATUS_get_tve_i_fetch(data)                                              ((0x00001000&(data))>>12)
#define VO_STATUS_tve_p_fetch_shift                                                  (11)
#define VO_STATUS_tve_p_fetch_mask                                                   (0x00000800)
#define VO_STATUS_tve_p_fetch(data)                                                  (0x00000800&((data)<<11))
#define VO_STATUS_tve_p_fetch_src(data)                                              ((0x00000800&(data))>>11)
#define VO_STATUS_get_tve_p_fetch(data)                                              ((0x00000800&(data))>>11)
#define VO_STATUS_pic_shift                                                          (10)
#define VO_STATUS_pic_mask                                                           (0x00000400)
#define VO_STATUS_pic(data)                                                          (0x00000400&((data)<<10))
#define VO_STATUS_pic_src(data)                                                      ((0x00000400&(data))>>10)
#define VO_STATUS_get_pic(data)                                                      ((0x00000400&(data))>>10)
#define VO_STATUS_dd_shift                                                           (9)
#define VO_STATUS_dd_mask                                                            (0x00000200)
#define VO_STATUS_dd(data)                                                           (0x00000200&((data)<<9))
#define VO_STATUS_dd_src(data)                                                       ((0x00000200&(data))>>9)
#define VO_STATUS_get_dd(data)                                                       ((0x00000200&(data))>>9)
#define VO_STATUS_csr_shift                                                          (8)
#define VO_STATUS_csr_mask                                                           (0x00000100)
#define VO_STATUS_csr(data)                                                          (0x00000100&((data)<<8))
#define VO_STATUS_csr_src(data)                                                      ((0x00000100&(data))>>8)
#define VO_STATUS_get_csr(data)                                                      ((0x00000100&(data))>>8)
#define VO_STATUS_osd2_shift                                                         (7)
#define VO_STATUS_osd2_mask                                                          (0x00000080)
#define VO_STATUS_osd2(data)                                                         (0x00000080&((data)<<7))
#define VO_STATUS_osd2_src(data)                                                     ((0x00000080&(data))>>7)
#define VO_STATUS_get_osd2(data)                                                     ((0x00000080&(data))>>7)
#define VO_STATUS_osd1_shift                                                         (6)
#define VO_STATUS_osd1_mask                                                          (0x00000040)
#define VO_STATUS_osd1(data)                                                         (0x00000040&((data)<<6))
#define VO_STATUS_osd1_src(data)                                                     ((0x00000040&(data))>>6)
#define VO_STATUS_get_osd1(data)                                                     ((0x00000040&(data))>>6)
#define VO_STATUS_sub1_shift                                                         (5)
#define VO_STATUS_sub1_mask                                                          (0x00000020)
#define VO_STATUS_sub1(data)                                                         (0x00000020&((data)<<5))
#define VO_STATUS_sub1_src(data)                                                     ((0x00000020&(data))>>5)
#define VO_STATUS_get_sub1(data)                                                     ((0x00000020&(data))>>5)
#define VO_STATUS_v2_shift                                                           (4)
#define VO_STATUS_v2_mask                                                            (0x00000010)
#define VO_STATUS_v2(data)                                                           (0x00000010&((data)<<4))
#define VO_STATUS_v2_src(data)                                                       ((0x00000010&(data))>>4)
#define VO_STATUS_get_v2(data)                                                       ((0x00000010&(data))>>4)
#define VO_STATUS_v1_shift                                                           (3)
#define VO_STATUS_v1_mask                                                            (0x00000008)
#define VO_STATUS_v1(data)                                                           (0x00000008&((data)<<3))
#define VO_STATUS_v1_src(data)                                                       ((0x00000008&(data))>>3)
#define VO_STATUS_get_v1(data)                                                       ((0x00000008&(data))>>3)
#define VO_STATUS_wb_shift                                                           (2)
#define VO_STATUS_wb_mask                                                            (0x00000004)
#define VO_STATUS_wb(data)                                                           (0x00000004&((data)<<2))
#define VO_STATUS_wb_src(data)                                                       ((0x00000004&(data))>>2)
#define VO_STATUS_get_wb(data)                                                       ((0x00000004&(data))>>2)
#define VO_STATUS_mix2_shift                                                         (1)
#define VO_STATUS_mix2_mask                                                          (0x00000002)
#define VO_STATUS_mix2(data)                                                         (0x00000002&((data)<<1))
#define VO_STATUS_mix2_src(data)                                                     ((0x00000002&(data))>>1)
#define VO_STATUS_get_mix2(data)                                                     ((0x00000002&(data))>>1)
#define VO_STATUS_mix1_shift                                                         (0)
#define VO_STATUS_mix1_mask                                                          (0x00000001)
#define VO_STATUS_mix1(data)                                                         (0x00000001&((data)<<0))
#define VO_STATUS_mix1_src(data)                                                     ((0x00000001&(data))>>0)
#define VO_STATUS_get_mix1(data)                                                     ((0x00000001&(data))>>0)


#define VO_MON_SETUP                                                                 0x1800586C
#define VO_MON_SETUP_reg_addr                                                        "0xB800586C"
#define VO_MON_SETUP_reg                                                             0xB800586C
#define set_VO_MON_SETUP_reg(data)   (*((volatile unsigned int*) VO_MON_SETUP_reg)=data)
#define get_VO_MON_SETUP_reg   (*((volatile unsigned int*) VO_MON_SETUP_reg))
#define VO_MON_SETUP_inst_adr                                                        "0x001B"
#define VO_MON_SETUP_inst                                                            0x001B
#define VO_MON_SETUP_wb_shift                                                        (27)
#define VO_MON_SETUP_wb_mask                                                         (0x08000000)
#define VO_MON_SETUP_wb(data)                                                        (0x08000000&((data)<<27))
#define VO_MON_SETUP_wb_src(data)                                                    ((0x08000000&(data))>>27)
#define VO_MON_SETUP_get_wb(data)                                                    ((0x08000000&(data))>>27)
#define VO_MON_SETUP_csr_shift                                                       (26)
#define VO_MON_SETUP_csr_mask                                                        (0x04000000)
#define VO_MON_SETUP_csr(data)                                                       (0x04000000&((data)<<26))
#define VO_MON_SETUP_csr_src(data)                                                   ((0x04000000&(data))>>26)
#define VO_MON_SETUP_get_csr(data)                                                   ((0x04000000&(data))>>26)
#define VO_MON_SETUP_pic_shift                                                       (25)
#define VO_MON_SETUP_pic_mask                                                        (0x02000000)
#define VO_MON_SETUP_pic(data)                                                       (0x02000000&((data)<<25))
#define VO_MON_SETUP_pic_src(data)                                                   ((0x02000000&(data))>>25)
#define VO_MON_SETUP_get_pic(data)                                                   ((0x02000000&(data))>>25)
#define VO_MON_SETUP_dd_shift                                                        (24)
#define VO_MON_SETUP_dd_mask                                                         (0x01000000)
#define VO_MON_SETUP_dd(data)                                                        (0x01000000&((data)<<24))
#define VO_MON_SETUP_dd_src(data)                                                    ((0x01000000&(data))>>24)
#define VO_MON_SETUP_get_dd(data)                                                    ((0x01000000&(data))>>24)
#define VO_MON_SETUP_osd2_shift                                                      (23)
#define VO_MON_SETUP_osd2_mask                                                       (0x00800000)
#define VO_MON_SETUP_osd2(data)                                                      (0x00800000&((data)<<23))
#define VO_MON_SETUP_osd2_src(data)                                                  ((0x00800000&(data))>>23)
#define VO_MON_SETUP_get_osd2(data)                                                  ((0x00800000&(data))>>23)
#define VO_MON_SETUP_osd1_shift                                                      (22)
#define VO_MON_SETUP_osd1_mask                                                       (0x00400000)
#define VO_MON_SETUP_osd1(data)                                                      (0x00400000&((data)<<22))
#define VO_MON_SETUP_osd1_src(data)                                                  ((0x00400000&(data))>>22)
#define VO_MON_SETUP_get_osd1(data)                                                  ((0x00400000&(data))>>22)
#define VO_MON_SETUP_sub1_shift                                                      (21)
#define VO_MON_SETUP_sub1_mask                                                       (0x00200000)
#define VO_MON_SETUP_sub1(data)                                                      (0x00200000&((data)<<21))
#define VO_MON_SETUP_sub1_src(data)                                                  ((0x00200000&(data))>>21)
#define VO_MON_SETUP_get_sub1(data)                                                  ((0x00200000&(data))>>21)
#define VO_MON_SETUP_v2_shift                                                        (20)
#define VO_MON_SETUP_v2_mask                                                         (0x00100000)
#define VO_MON_SETUP_v2(data)                                                        (0x00100000&((data)<<20))
#define VO_MON_SETUP_v2_src(data)                                                    ((0x00100000&(data))>>20)
#define VO_MON_SETUP_get_v2(data)                                                    ((0x00100000&(data))>>20)
#define VO_MON_SETUP_v1_shift                                                        (19)
#define VO_MON_SETUP_v1_mask                                                         (0x00080000)
#define VO_MON_SETUP_v1(data)                                                        (0x00080000&((data)<<19))
#define VO_MON_SETUP_v1_src(data)                                                    ((0x00080000&(data))>>19)
#define VO_MON_SETUP_get_v1(data)                                                    ((0x00080000&(data))>>19)
#define VO_MON_SETUP_ch2_shift                                                       (18)
#define VO_MON_SETUP_ch2_mask                                                        (0x00040000)
#define VO_MON_SETUP_ch2(data)                                                       (0x00040000&((data)<<18))
#define VO_MON_SETUP_ch2_src(data)                                                   ((0x00040000&(data))>>18)
#define VO_MON_SETUP_get_ch2(data)                                                   ((0x00040000&(data))>>18)
#define VO_MON_SETUP_ch1_i_shift                                                     (17)
#define VO_MON_SETUP_ch1_i_mask                                                      (0x00020000)
#define VO_MON_SETUP_ch1_i(data)                                                     (0x00020000&((data)<<17))
#define VO_MON_SETUP_ch1_i_src(data)                                                 ((0x00020000&(data))>>17)
#define VO_MON_SETUP_get_ch1_i(data)                                                 ((0x00020000&(data))>>17)
#define VO_MON_SETUP_ch1_p_shift                                                     (16)
#define VO_MON_SETUP_ch1_p_mask                                                      (0x00010000)
#define VO_MON_SETUP_ch1_p(data)                                                     (0x00010000&((data)<<16))
#define VO_MON_SETUP_ch1_p_src(data)                                                 ((0x00010000&(data))>>16)
#define VO_MON_SETUP_get_ch1_p(data)                                                 ((0x00010000&(data))>>16)
#define VO_MON_SETUP_th_shift                                                        (0)
#define VO_MON_SETUP_th_mask                                                         (0x000007FF)
#define VO_MON_SETUP_th(data)                                                        (0x000007FF&((data)<<0))
#define VO_MON_SETUP_th_src(data)                                                    ((0x000007FF&(data))>>0)
#define VO_MON_SETUP_get_th(data)                                                    ((0x000007FF&(data))>>0)


#define VO_MON_BUF                                                                   0x18005870
#define VO_MON_BUF_reg_addr                                                          "0xB8005870"
#define VO_MON_BUF_reg                                                               0xB8005870
#define set_VO_MON_BUF_reg(data)   (*((volatile unsigned int*) VO_MON_BUF_reg)=data)
#define get_VO_MON_BUF_reg   (*((volatile unsigned int*) VO_MON_BUF_reg))
#define VO_MON_BUF_inst_adr                                                          "0x001C"
#define VO_MON_BUF_inst                                                              0x001C
#define VO_MON_BUF_delay_shift                                                       (16)
#define VO_MON_BUF_delay_mask                                                        (0xFFFF0000)
#define VO_MON_BUF_delay(data)                                                       (0xFFFF0000&((data)<<16))
#define VO_MON_BUF_delay_src(data)                                                   ((0xFFFF0000&(data))>>16)
#define VO_MON_BUF_get_delay(data)                                                   ((0xFFFF0000&(data))>>16)
#define VO_MON_BUF_wb_shift                                                          (8)
#define VO_MON_BUF_wb_mask                                                           (0x00000100)
#define VO_MON_BUF_wb(data)                                                          (0x00000100&((data)<<8))
#define VO_MON_BUF_wb_src(data)                                                      ((0x00000100&(data))>>8)
#define VO_MON_BUF_get_wb(data)                                                      ((0x00000100&(data))>>8)
#define VO_MON_BUF_csr_shift                                                         (7)
#define VO_MON_BUF_csr_mask                                                          (0x00000080)
#define VO_MON_BUF_csr(data)                                                         (0x00000080&((data)<<7))
#define VO_MON_BUF_csr_src(data)                                                     ((0x00000080&(data))>>7)
#define VO_MON_BUF_get_csr(data)                                                     ((0x00000080&(data))>>7)
#define VO_MON_BUF_pic_shift                                                         (6)
#define VO_MON_BUF_pic_mask                                                          (0x00000040)
#define VO_MON_BUF_pic(data)                                                         (0x00000040&((data)<<6))
#define VO_MON_BUF_pic_src(data)                                                     ((0x00000040&(data))>>6)
#define VO_MON_BUF_get_pic(data)                                                     ((0x00000040&(data))>>6)
#define VO_MON_BUF_dd_shift                                                          (5)
#define VO_MON_BUF_dd_mask                                                           (0x00000020)
#define VO_MON_BUF_dd(data)                                                          (0x00000020&((data)<<5))
#define VO_MON_BUF_dd_src(data)                                                      ((0x00000020&(data))>>5)
#define VO_MON_BUF_get_dd(data)                                                      ((0x00000020&(data))>>5)
#define VO_MON_BUF_osd2_shift                                                        (4)
#define VO_MON_BUF_osd2_mask                                                         (0x00000010)
#define VO_MON_BUF_osd2(data)                                                        (0x00000010&((data)<<4))
#define VO_MON_BUF_osd2_src(data)                                                    ((0x00000010&(data))>>4)
#define VO_MON_BUF_get_osd2(data)                                                    ((0x00000010&(data))>>4)
#define VO_MON_BUF_osd1_shift                                                        (3)
#define VO_MON_BUF_osd1_mask                                                         (0x00000008)
#define VO_MON_BUF_osd1(data)                                                        (0x00000008&((data)<<3))
#define VO_MON_BUF_osd1_src(data)                                                    ((0x00000008&(data))>>3)
#define VO_MON_BUF_get_osd1(data)                                                    ((0x00000008&(data))>>3)
#define VO_MON_BUF_sub1_shift                                                        (2)
#define VO_MON_BUF_sub1_mask                                                         (0x00000004)
#define VO_MON_BUF_sub1(data)                                                        (0x00000004&((data)<<2))
#define VO_MON_BUF_sub1_src(data)                                                    ((0x00000004&(data))>>2)
#define VO_MON_BUF_get_sub1(data)                                                    ((0x00000004&(data))>>2)
#define VO_MON_BUF_v2_shift                                                          (1)
#define VO_MON_BUF_v2_mask                                                           (0x00000002)
#define VO_MON_BUF_v2(data)                                                          (0x00000002&((data)<<1))
#define VO_MON_BUF_v2_src(data)                                                      ((0x00000002&(data))>>1)
#define VO_MON_BUF_get_v2(data)                                                      ((0x00000002&(data))>>1)
#define VO_MON_BUF_v1_shift                                                          (0)
#define VO_MON_BUF_v1_mask                                                           (0x00000001)
#define VO_MON_BUF_v1(data)                                                          (0x00000001&((data)<<0))
#define VO_MON_BUF_v1_src(data)                                                      ((0x00000001&(data))>>0)
#define VO_MON_BUF_get_v1(data)                                                      ((0x00000001&(data))>>0)


#define VO_MON_UF                                                                    0x18005874
#define VO_MON_UF_reg_addr                                                           "0xB8005874"
#define VO_MON_UF_reg                                                                0xB8005874
#define set_VO_MON_UF_reg(data)   (*((volatile unsigned int*) VO_MON_UF_reg)=data)
#define get_VO_MON_UF_reg   (*((volatile unsigned int*) VO_MON_UF_reg))
#define VO_MON_UF_inst_adr                                                           "0x001D"
#define VO_MON_UF_inst                                                               0x001D
#define VO_MON_UF_not_full_shift                                                     (0)
#define VO_MON_UF_not_full_mask                                                      (0x000007FF)
#define VO_MON_UF_not_full(data)                                                     (0x000007FF&((data)<<0))
#define VO_MON_UF_not_full_src(data)                                                 ((0x000007FF&(data))>>0)
#define VO_MON_UF_get_not_full(data)                                                 ((0x000007FF&(data))>>0)


#define VO_MON_UF_P                                                                  0x18005878
#define VO_MON_UF_P_reg_addr                                                         "0xB8005878"
#define VO_MON_UF_P_reg                                                              0xB8005878
#define set_VO_MON_UF_P_reg(data)   (*((volatile unsigned int*) VO_MON_UF_P_reg)=data)
#define get_VO_MON_UF_P_reg   (*((volatile unsigned int*) VO_MON_UF_P_reg))
#define VO_MON_UF_P_inst_adr                                                         "0x001E"
#define VO_MON_UF_P_inst                                                             0x001E
#define VO_MON_UF_P_y_shift                                                          (0)
#define VO_MON_UF_P_y_mask                                                           (0x000007FF)
#define VO_MON_UF_P_y(data)                                                          (0x000007FF&((data)<<0))
#define VO_MON_UF_P_y_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_MON_UF_P_get_y(data)                                                      ((0x000007FF&(data))>>0)


#define VO_MON_UF_I                                                                  0x1800587C
#define VO_MON_UF_I_reg_addr                                                         "0xB800587C"
#define VO_MON_UF_I_reg                                                              0xB800587C
#define set_VO_MON_UF_I_reg(data)   (*((volatile unsigned int*) VO_MON_UF_I_reg)=data)
#define get_VO_MON_UF_I_reg   (*((volatile unsigned int*) VO_MON_UF_I_reg))
#define VO_MON_UF_I_inst_adr                                                         "0x001F"
#define VO_MON_UF_I_inst                                                             0x001F
#define VO_MON_UF_I_y_shift                                                          (0)
#define VO_MON_UF_I_y_mask                                                           (0x000007FF)
#define VO_MON_UF_I_y(data)                                                          (0x000007FF&((data)<<0))
#define VO_MON_UF_I_y_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_MON_UF_I_get_y(data)                                                      ((0x000007FF&(data))>>0)


#define VO_MON_UF_H                                                                  0x18005880
#define VO_MON_UF_H_reg_addr                                                         "0xB8005880"
#define VO_MON_UF_H_reg                                                              0xB8005880
#define set_VO_MON_UF_H_reg(data)   (*((volatile unsigned int*) VO_MON_UF_H_reg)=data)
#define get_VO_MON_UF_H_reg   (*((volatile unsigned int*) VO_MON_UF_H_reg))
#define VO_MON_UF_H_inst_adr                                                         "0x0020"
#define VO_MON_UF_H_inst                                                             0x0020
#define VO_MON_UF_H_y_shift                                                          (0)
#define VO_MON_UF_H_y_mask                                                           (0x000007FF)
#define VO_MON_UF_H_y(data)                                                          (0x000007FF&((data)<<0))
#define VO_MON_UF_H_y_src(data)                                                      ((0x000007FF&(data))>>0)
#define VO_MON_UF_H_get_y(data)                                                      ((0x000007FF&(data))>>0)


#define VO_MON_UF_V1                                                                 0x18005884
#define VO_MON_UF_V1_reg_addr                                                        "0xB8005884"
#define VO_MON_UF_V1_reg                                                             0xB8005884
#define set_VO_MON_UF_V1_reg(data)   (*((volatile unsigned int*) VO_MON_UF_V1_reg)=data)
#define get_VO_MON_UF_V1_reg   (*((volatile unsigned int*) VO_MON_UF_V1_reg))
#define VO_MON_UF_V1_inst_adr                                                        "0x0021"
#define VO_MON_UF_V1_inst                                                            0x0021
#define VO_MON_UF_V1_cnt_shift                                                       (0)
#define VO_MON_UF_V1_cnt_mask                                                        (0x000001FF)
#define VO_MON_UF_V1_cnt(data)                                                       (0x000001FF&((data)<<0))
#define VO_MON_UF_V1_cnt_src(data)                                                   ((0x000001FF&(data))>>0)
#define VO_MON_UF_V1_get_cnt(data)                                                   ((0x000001FF&(data))>>0)


#define VO_MON_UF_V2                                                                 0x18005888
#define VO_MON_UF_V2_reg_addr                                                        "0xB8005888"
#define VO_MON_UF_V2_reg                                                             0xB8005888
#define set_VO_MON_UF_V2_reg(data)   (*((volatile unsigned int*) VO_MON_UF_V2_reg)=data)
#define get_VO_MON_UF_V2_reg   (*((volatile unsigned int*) VO_MON_UF_V2_reg))
#define VO_MON_UF_V2_inst_adr                                                        "0x0022"
#define VO_MON_UF_V2_inst                                                            0x0022
#define VO_MON_UF_V2_cnt_shift                                                       (0)
#define VO_MON_UF_V2_cnt_mask                                                        (0x000001FF)
#define VO_MON_UF_V2_cnt(data)                                                       (0x000001FF&((data)<<0))
#define VO_MON_UF_V2_cnt_src(data)                                                   ((0x000001FF&(data))>>0)
#define VO_MON_UF_V2_get_cnt(data)                                                   ((0x000001FF&(data))>>0)


#define VO_MON_UF_SUB1                                                               0x1800588C
#define VO_MON_UF_SUB1_reg_addr                                                      "0xB800588C"
#define VO_MON_UF_SUB1_reg                                                           0xB800588C
#define set_VO_MON_UF_SUB1_reg(data)   (*((volatile unsigned int*) VO_MON_UF_SUB1_reg)=data)
#define get_VO_MON_UF_SUB1_reg   (*((volatile unsigned int*) VO_MON_UF_SUB1_reg))
#define VO_MON_UF_SUB1_inst_adr                                                      "0x0023"
#define VO_MON_UF_SUB1_inst                                                          0x0023
#define VO_MON_UF_SUB1_cnt_shift                                                     (0)
#define VO_MON_UF_SUB1_cnt_mask                                                      (0x000001FF)
#define VO_MON_UF_SUB1_cnt(data)                                                     (0x000001FF&((data)<<0))
#define VO_MON_UF_SUB1_cnt_src(data)                                                 ((0x000001FF&(data))>>0)
#define VO_MON_UF_SUB1_get_cnt(data)                                                 ((0x000001FF&(data))>>0)


#define VO_MON_UF_OSD1                                                               0x18005890
#define VO_MON_UF_OSD1_reg_addr                                                      "0xB8005890"
#define VO_MON_UF_OSD1_reg                                                           0xB8005890
#define set_VO_MON_UF_OSD1_reg(data)   (*((volatile unsigned int*) VO_MON_UF_OSD1_reg)=data)
#define get_VO_MON_UF_OSD1_reg   (*((volatile unsigned int*) VO_MON_UF_OSD1_reg))
#define VO_MON_UF_OSD1_inst_adr                                                      "0x0024"
#define VO_MON_UF_OSD1_inst                                                          0x0024
#define VO_MON_UF_OSD1_cnt_shift                                                     (0)
#define VO_MON_UF_OSD1_cnt_mask                                                      (0x000001FF)
#define VO_MON_UF_OSD1_cnt(data)                                                     (0x000001FF&((data)<<0))
#define VO_MON_UF_OSD1_cnt_src(data)                                                 ((0x000001FF&(data))>>0)
#define VO_MON_UF_OSD1_get_cnt(data)                                                 ((0x000001FF&(data))>>0)


#define VO_MON_UF_OSD2                                                               0x18005894
#define VO_MON_UF_OSD2_reg_addr                                                      "0xB8005894"
#define VO_MON_UF_OSD2_reg                                                           0xB8005894
#define set_VO_MON_UF_OSD2_reg(data)   (*((volatile unsigned int*) VO_MON_UF_OSD2_reg)=data)
#define get_VO_MON_UF_OSD2_reg   (*((volatile unsigned int*) VO_MON_UF_OSD2_reg))
#define VO_MON_UF_OSD2_inst_adr                                                      "0x0025"
#define VO_MON_UF_OSD2_inst                                                          0x0025
#define VO_MON_UF_OSD2_cnt_shift                                                     (0)
#define VO_MON_UF_OSD2_cnt_mask                                                      (0x000001FF)
#define VO_MON_UF_OSD2_cnt(data)                                                     (0x000001FF&((data)<<0))
#define VO_MON_UF_OSD2_cnt_src(data)                                                 ((0x000001FF&(data))>>0)
#define VO_MON_UF_OSD2_get_cnt(data)                                                 ((0x000001FF&(data))>>0)


#define VO_DBG                                                                       0x18005898
#define VO_DBG_reg_addr                                                              "0xB8005898"
#define VO_DBG_reg                                                                   0xB8005898
#define set_VO_DBG_reg(data)   (*((volatile unsigned int*) VO_DBG_reg)=data)
#define get_VO_DBG_reg   (*((volatile unsigned int*) VO_DBG_reg))
#define VO_DBG_inst_adr                                                              "0x0026"
#define VO_DBG_inst                                                                  0x0026
#define VO_DBG_write_en3_shift                                                       (24)
#define VO_DBG_write_en3_mask                                                        (0x01000000)
#define VO_DBG_write_en3(data)                                                       (0x01000000&((data)<<24))
#define VO_DBG_write_en3_src(data)                                                   ((0x01000000&(data))>>24)
#define VO_DBG_get_write_en3(data)                                                   ((0x01000000&(data))>>24)
#define VO_DBG_sel1_shift                                                            (16)
#define VO_DBG_sel1_mask                                                             (0x00FF0000)
#define VO_DBG_sel1(data)                                                            (0x00FF0000&((data)<<16))
#define VO_DBG_sel1_src(data)                                                        ((0x00FF0000&(data))>>16)
#define VO_DBG_get_sel1(data)                                                        ((0x00FF0000&(data))>>16)
#define VO_DBG_write_en2_shift                                                       (12)
#define VO_DBG_write_en2_mask                                                        (0x00001000)
#define VO_DBG_write_en2(data)                                                       (0x00001000&((data)<<12))
#define VO_DBG_write_en2_src(data)                                                   ((0x00001000&(data))>>12)
#define VO_DBG_get_write_en2(data)                                                   ((0x00001000&(data))>>12)
#define VO_DBG_sel0_shift                                                            (4)
#define VO_DBG_sel0_mask                                                             (0x00000FF0)
#define VO_DBG_sel0(data)                                                            (0x00000FF0&((data)<<4))
#define VO_DBG_sel0_src(data)                                                        ((0x00000FF0&(data))>>4)
#define VO_DBG_get_sel0(data)                                                        ((0x00000FF0&(data))>>4)
#define VO_DBG_write_en1_shift                                                       (1)
#define VO_DBG_write_en1_mask                                                        (0x00000002)
#define VO_DBG_write_en1(data)                                                       (0x00000002&((data)<<1))
#define VO_DBG_write_en1_src(data)                                                   ((0x00000002&(data))>>1)
#define VO_DBG_get_write_en1(data)                                                   ((0x00000002&(data))>>1)
#define VO_DBG_en_shift                                                              (0)
#define VO_DBG_en_mask                                                               (0x00000001)
#define VO_DBG_en(data)                                                              (0x00000001&((data)<<0))
#define VO_DBG_en_src(data)                                                          ((0x00000001&(data))>>0)
#define VO_DBG_get_en(data)                                                          ((0x00000001&(data))>>0)


#define VO_DACT                                                                      0x1800589C
#define VO_DACT_reg_addr                                                             "0xB800589C"
#define VO_DACT_reg                                                                  0xB800589C
#define set_VO_DACT_reg(data)   (*((volatile unsigned int*) VO_DACT_reg)=data)
#define get_VO_DACT_reg   (*((volatile unsigned int*) VO_DACT_reg))
#define VO_DACT_inst_adr                                                             "0x0027"
#define VO_DACT_inst                                                                 0x0027
#define VO_DACT_ramp_4_shift                                                         (5)
#define VO_DACT_ramp_4_mask                                                          (0x00000020)
#define VO_DACT_ramp_4(data)                                                         (0x00000020&((data)<<5))
#define VO_DACT_ramp_4_src(data)                                                     ((0x00000020&(data))>>5)
#define VO_DACT_get_ramp_4(data)                                                     ((0x00000020&(data))>>5)
#define VO_DACT_ramp_2_shift                                                         (4)
#define VO_DACT_ramp_2_mask                                                          (0x00000010)
#define VO_DACT_ramp_2(data)                                                         (0x00000010&((data)<<4))
#define VO_DACT_ramp_2_src(data)                                                     ((0x00000010&(data))>>4)
#define VO_DACT_get_ramp_2(data)                                                     ((0x00000010&(data))>>4)
#define VO_DACT_ramp_1_shift                                                         (3)
#define VO_DACT_ramp_1_mask                                                          (0x00000008)
#define VO_DACT_ramp_1(data)                                                         (0x00000008&((data)<<3))
#define VO_DACT_ramp_1_src(data)                                                     ((0x00000008&(data))>>3)
#define VO_DACT_get_ramp_1(data)                                                     ((0x00000008&(data))>>3)
#define VO_DACT_go_shift                                                             (2)
#define VO_DACT_go_mask                                                              (0x00000004)
#define VO_DACT_go(data)                                                             (0x00000004&((data)<<2))
#define VO_DACT_go_src(data)                                                         ((0x00000004&(data))>>2)
#define VO_DACT_get_go(data)                                                         ((0x00000004&(data))>>2)
#define VO_DACT_en_shift                                                             (1)
#define VO_DACT_en_mask                                                              (0x00000002)
#define VO_DACT_en(data)                                                             (0x00000002&((data)<<1))
#define VO_DACT_en_src(data)                                                         ((0x00000002&(data))>>1)
#define VO_DACT_get_en(data)                                                         ((0x00000002&(data))>>1)
#define VO_DACT_write_data_shift                                                     (0)
#define VO_DACT_write_data_mask                                                      (0x00000001)
#define VO_DACT_write_data(data)                                                     (0x00000001&((data)<<0))
#define VO_DACT_write_data_src(data)                                                 ((0x00000001&(data))>>0)
#define VO_DACT_get_write_data(data)                                                 ((0x00000001&(data))>>0)


#define VO_DACT_L                                                                    0x180058A0
#define VO_DACT_L_reg_addr                                                           "0xB80058A0"
#define VO_DACT_L_reg                                                                0xB80058A0
#define set_VO_DACT_L_reg(data)   (*((volatile unsigned int*) VO_DACT_L_reg)=data)
#define get_VO_DACT_L_reg   (*((volatile unsigned int*) VO_DACT_L_reg))
#define VO_DACT_L_inst_adr                                                           "0x0028"
#define VO_DACT_L_inst                                                               0x0028
#define VO_DACT_L_l_shift                                                            (0)
#define VO_DACT_L_l_mask                                                             (0x000007FF)
#define VO_DACT_L_l(data)                                                            (0x000007FF&((data)<<0))
#define VO_DACT_L_l_src(data)                                                        ((0x000007FF&(data))>>0)
#define VO_DACT_L_get_l(data)                                                        ((0x000007FF&(data))>>0)


#define VO_DACT_R                                                                    0x180058A4
#define VO_DACT_R_reg_addr                                                           "0xB80058A4"
#define VO_DACT_R_reg                                                                0xB80058A4
#define set_VO_DACT_R_reg(data)   (*((volatile unsigned int*) VO_DACT_R_reg)=data)
#define get_VO_DACT_R_reg   (*((volatile unsigned int*) VO_DACT_R_reg))
#define VO_DACT_R_inst_adr                                                           "0x0029"
#define VO_DACT_R_inst                                                               0x0029
#define VO_DACT_R_a_shift                                                            (0)
#define VO_DACT_R_a_mask                                                             (0x000003FF)
#define VO_DACT_R_a(data)                                                            (0x000003FF&((data)<<0))
#define VO_DACT_R_a_src(data)                                                        ((0x000003FF&(data))>>0)
#define VO_DACT_R_get_a(data)                                                        ((0x000003FF&(data))>>0)


#define VO_DACT_A                                                                    0x180058A8
#define VO_DACT_A_reg_addr                                                           "0xB80058A8"
#define VO_DACT_A_reg                                                                0xB80058A8
#define set_VO_DACT_A_reg(data)   (*((volatile unsigned int*) VO_DACT_A_reg)=data)
#define get_VO_DACT_A_reg   (*((volatile unsigned int*) VO_DACT_A_reg))
#define VO_DACT_A_inst_adr                                                           "0x002A"
#define VO_DACT_A_inst                                                               0x002A
#define VO_DACT_A_a_shift                                                            (0)
#define VO_DACT_A_a_mask                                                             (0x000003FF)
#define VO_DACT_A_a(data)                                                            (0x000003FF&((data)<<0))
#define VO_DACT_A_a_src(data)                                                        ((0x000003FF&(data))>>0)
#define VO_DACT_A_get_a(data)                                                        ((0x000003FF&(data))>>0)


#define VO_DACT_D                                                                    0x180058AC
#define VO_DACT_D_reg_addr                                                           "0xB80058AC"
#define VO_DACT_D_reg                                                                0xB80058AC
#define set_VO_DACT_D_reg(data)   (*((volatile unsigned int*) VO_DACT_D_reg)=data)
#define get_VO_DACT_D_reg   (*((volatile unsigned int*) VO_DACT_D_reg))
#define VO_DACT_D_inst_adr                                                           "0x002B"
#define VO_DACT_D_inst                                                               0x002B
#define VO_DACT_D_d_shift                                                            (0)
#define VO_DACT_D_d_mask                                                             (0x0000FFFF)
#define VO_DACT_D_d(data)                                                            (0x0000FFFF&((data)<<0))
#define VO_DACT_D_d_src(data)                                                        ((0x0000FFFF&(data))>>0)
#define VO_DACT_D_get_d(data)                                                        ((0x0000FFFF&(data))>>0)


#define VO_RBRAM                                                                     0x180058B0
#define VO_RBRAM_reg_addr                                                            "0xB80058B0"
#define VO_RBRAM_reg                                                                 0xB80058B0
#define set_VO_RBRAM_reg(data)   (*((volatile unsigned int*) VO_RBRAM_reg)=data)
#define get_VO_RBRAM_reg   (*((volatile unsigned int*) VO_RBRAM_reg))
#define VO_RBRAM_inst_adr                                                            "0x002C"
#define VO_RBRAM_inst                                                                0x002C
#define VO_RBRAM_en_shift                                                            (1)
#define VO_RBRAM_en_mask                                                             (0x00000002)
#define VO_RBRAM_en(data)                                                            (0x00000002&((data)<<1))
#define VO_RBRAM_en_src(data)                                                        ((0x00000002&(data))>>1)
#define VO_RBRAM_get_en(data)                                                        ((0x00000002&(data))>>1)
#define VO_RBRAM_write_data_shift                                                    (0)
#define VO_RBRAM_write_data_mask                                                     (0x00000001)
#define VO_RBRAM_write_data(data)                                                    (0x00000001&((data)<<0))
#define VO_RBRAM_write_data_src(data)                                                ((0x00000001&(data))>>0)
#define VO_RBRAM_get_write_data(data)                                                ((0x00000001&(data))>>0)


#define VO_VERF_0                                                                    0x180058B4
#define VO_VERF_1                                                                    0x180058B8
#define VO_VERF_2                                                                    0x180058BC
#define VO_VERF_0_reg_addr                                                           "0xB80058B4"
#define VO_VERF_1_reg_addr                                                           "0xB80058B8"
#define VO_VERF_2_reg_addr                                                           "0xB80058BC"
#define VO_VERF_0_reg                                                                0xB80058B4
#define VO_VERF_1_reg                                                                0xB80058B8
#define VO_VERF_2_reg                                                                0xB80058BC
#define set_VO_VERF_0_reg(data)   (*((volatile unsigned int*) VO_VERF_0_reg)=data)
#define set_VO_VERF_1_reg(data)   (*((volatile unsigned int*) VO_VERF_1_reg)=data)
#define set_VO_VERF_2_reg(data)   (*((volatile unsigned int*) VO_VERF_2_reg)=data)
#define get_VO_VERF_0_reg   (*((volatile unsigned int*) VO_VERF_0_reg))
#define get_VO_VERF_1_reg   (*((volatile unsigned int*) VO_VERF_1_reg))
#define get_VO_VERF_2_reg   (*((volatile unsigned int*) VO_VERF_2_reg))
#define VO_VERF_0_inst_adr                                                           "0x002D"
#define VO_VERF_1_inst_adr                                                           "0x002E"
#define VO_VERF_2_inst_adr                                                           "0x002F"
#define VO_VERF_0_inst                                                               0x002D
#define VO_VERF_1_inst                                                               0x002E
#define VO_VERF_2_inst                                                               0x002F
#define VO_VERF_mode_shift                                                           (0)
#define VO_VERF_mode_mask                                                            (0xFFFFFFFF)
#define VO_VERF_mode(data)                                                           (0xFFFFFFFF&((data)<<0))
#define VO_VERF_mode_src(data)                                                       ((0xFFFFFFFF&(data))>>0)
#define VO_VERF_get_mode(data)                                                       ((0xFFFFFFFF&(data))>>0)


#endif
