#ifndef __VRMI_IFO_TYPES_H__
#define __VRMI_IFO_TYPES_H__

#include <OSAL.h>



#undef ATTRIBUTE_PACKED
#undef PRAGMA_PACK_BEGIN 
#undef PRAGMA_PACK_END

#if defined(__GNUC__)
#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
#define ATTRIBUTE_PACKED __attribute__ ((packed))
#define PRAGMA_PACK 0
#endif
#endif

#if !defined(ATTRIBUTE_PACKED)
#define ATTRIBUTE_PACKED
#define PRAGMA_PACK 1
#endif

#if PRAGMA_PACK
#pragma pack(1)
#endif


//
//   For VRMI_GI's LAST_DATE
//
typedef struct DATE_st{
    uint16_t year;          //16
#ifdef WORDS_BIGENDIAN
    unsigned char week       :3;
    unsigned char month      :5;

    unsigned char reserved   :2;
    unsigned char day        :6;

    unsigned char ds         :1;
    unsigned char tm         :1;
    unsigned char tz_sign    :1;
    unsigned char time_zone  :5;
    
#else
    unsigned char month      :5;
    unsigned char week       :3;

    unsigned char day        :6;
    unsigned char reserved   :2;

    unsigned char time_zone  :5;
    unsigned char tz_sign    :1;
    unsigned char tm         :1;
    unsigned char ds         :1;
#endif
} ATTRIBUTE_PACKED DATE_st;


//
// For VRMI_GI's LAST_TIME
//
typedef struct TIME_st{
    uint8_t hour;                   // 8
    uint8_t min;                    // 8
    uint8_t sec;                    // 8
    uint8_t reserved;               // 8
} ATTRIBUTE_PACKED TIME_st;


//
// For VRMI_GI's DSC_KF_PNT
// 
typedef struct KF_PNT_st{
    uint8_t       zero_1;            // 8
    uint8_t       kf_video_frame_n;  // 8
    uint8_t       zero_2[2];         //16
    uint32_t      kf_vobu_a;         //32
} ATTRIBUTE_PACKED KF_PNT_st;


//
//  For VRMI_GI's DSC_NM_FMT
// 
typedef struct VRMI_DSC_NM_st{
    uint8_t       char_set_1;        //8
    uint8_t       length_1;          //8
    uint8_t       char_set_2;        //8
    uint8_t       length_2;          //8
} ATTRIBUTE_PACKED VRMI_DSC_NM_st;


//
//  For VRMI_GI (VRMI General Information)
//
typedef struct VRMI_GI_st{
    char           vrm_id[12];     // 12
    uint8_t        zero_1[16];     // 16
    uint32_t       vrmi_ea;        //  4
    uint16_t       vern;           //  2
    uint8_t        zero_2[30];     // 30
    uint8_t        dsc_st;         //  1
    uint8_t        zero_3[6];      //  6
    DATE_st        last_date;      //  5
    TIME_st        last_time;      //  4
    uint8_t        zero_4[36];     // 36
    KF_PNT_st      dsc_kf_pnt;     //  8
    VRMI_DSC_NM_st dsc_nm_fmt;     //  4
    char           dsc_nm[64];     // 64
    char           alt_dsc_nm[64]; // 64
    char           man_id[32];     // 32
    char           mdl_id[16];     // 16
    char           frmw_id[16];    // 16
    char           menu_lo_id[2];  //  2
    uint8_t        zero_5[190];    //190
    uint8_t        man_ud[512];    //512
    uint8_t        ftt_ns;         //  1
    uint8_t        dsc_pb_ns;      //  1
    uint8_t        dsc_pb_seqt[99];// 99
    uint8_t        zero_6[411];    //411
    uint8_t        vrmi_gi_ud[512];//512
} ATTRIBUTE_PACKED VRMI_GI_st;


typedef struct VRCHP_IT_st{
 // VRCHP_MKI 
    uint8_t        cell_id;         // 8
    uint8_t        video_frame_n;   // 8
#ifdef WORDS_BIGENDIAN
    unsigned char  rec_start         : 1;
    unsigned char  cell_start        : 1;
    unsigned char  vobu_start        : 1;
    unsigned char  vobu_size         : 5;
#else
    unsigned char  vobu_size         : 5;
    unsigned char  vobu_start        : 1;
    unsigned char  cell_start        : 1;
    unsigned char  rec_start         : 1;
#endif
    uint8_t        vobu_size_low;   // 8
    uint32_t       chp_vobu_a;      //32
 /////////////////////////////////////////////
 // VRCHP_KFI 
    uint8_t        zero_1;          // 8
    uint8_t        kf_video_frame_n;// 8
    uint8_t        zero_2[2];       //16
    uint32_t       kf_vobu_a;       //32
} ATTRIBUTE_PACKED VRCHP_IT_st;

//
//  For VRMI_CHPI (VRMI Chapter Information)



// 
typedef struct VRMI_CHPI_st{
    uint8_t        vrchp_ns;      //   1
    uint8_t        zero_1[31];    //  31
    VRCHP_IT_st vrchp_it[254]; //4064
} ATTRIBUTE_PACKED VRMI_CHPI_st;


typedef struct VRMI_REC_MODE_st{
#ifdef WORDS_BIGENDIAN
    unsigned char   zero_1     :5;
    unsigned char   protection :3;

    unsigned char   pl_match   :2;
    unsigned char   pg_match   :2;
    unsigned char   zero_2     :4;
#else
    unsigned char   protection :3;
    unsigned char   zero_1     :5;

    unsigned char   zero_2     :4;
    unsigned char   pg_match   :2;
    unsigned char   pl_match   :2;
#endif
} ATTRIBUTE_PACKED VRMI_REC_MODE_st;



typedef struct VRMI_PB_TM{
    uint8_t         hour;      //8
    uint8_t         min;       //8
    uint8_t         sec;       //8
#ifdef WORDS_BIGENDIAN
    unsigned char   tc_flag     :2;
    unsigned char   video_frame :6;
#else
    unsigned char   video_frame :6;
    unsigned char   tc_flag     :2;
#endif
} ATTRIBUTE_PACKED VRMI_PB_TM;


//
//  Form VRMI_RECI's VRPLI
//
typedef struct VRMI_VRPLI_st{
#ifdef WORDS_BIGENDIAN
    unsigned char    pl_mode     :1;
    unsigned char    zero_1      :7;
#else
    unsigned char    zero_1      :7;
    unsigned char    pl_mode     :1;
#endif
    uint8_t          vrpl_ns;   //8
} ATTRIBUTE_PACKED VRMI_VRPLI_st;



//
//  For VRMI_RECI (VRMI Recording Information)
//
typedef struct VRMI_RECI_st{
    uint8_t           rec_st;        //  1
    VRMI_REC_MODE_st  rec_mode;      //  2
    uint8_t           rec_brt;       //  1
    uint8_t           rec_src;       //  1
    uint8_t           rec_cnt;       //  1
    uint8_t           rec_vob_ifo;   //  1
    DATE_st           rec_date;      //  5
    TIME_st           rec_time;      //  4
    VRMI_PB_TM        rec_pb_tm;     //  4
    uint8_t           zero_1[8];     //  8
    uint32_t          rec_e_a;       //  4
    uint8_t           rec_vrchp_ns;  //  1
    uint8_t           zero_2[19];    // 19
    KF_PNT_st         rec_kf_pnt;    //  8
    VRMI_DSC_NM_st    rec_nm_fmt;    //  4
    char              rec_nm[64];    // 64
    char              alt_rec_nm[64];// 64
    VRMI_VRPLI_st     *vrpli;         //  2     (VRMI_Video unused)
    uint8_t           *vrpl;          // 99     (VRMI_Video unused)
    uint8_t           *zero_3;        //155     (VRMI_Video unused)
    uint8_t           *vrmi_reci_ud;  // 64     (default not allocate, until someone use it!)
} ATTRIBUTE_PACKED VRMI_RECI_st;
#define MIN_VRMI_RECI_SIZE 192     // VRMI_RECI's size excluding the last 4 pointers


//
//  VRMI main structure <Video Recording Manager Information>
//
typedef struct VRMI_st{
    VRMI_GI_st     vrmi_gi;        // VRMI_GI
    VRMI_CHPI_st   vrmi_chpi;      // VRMI_CHPI
    VRMI_RECI_st   *vrmi_reci;     // VRMI_RECI (PlusRW: #1~#49, DVD-Video: #1~#99)     
    uint8_t        *vrmi_res;      // VRMI_RES  (PlusRW: 1536, DVD-Video: 1280 bytes)
} ATTRIBUTE_PACKED VRMI_st;





#if PRAGMA_PACK
#pragma pack()
#endif

#endif  /* __VRMI_IFO_TYPES_H__*/


