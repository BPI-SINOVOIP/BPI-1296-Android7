#ifndef __DVR_IFO_TYPES_H_INCLUDED
#define __DVR_IFO_TYPES_H_INCLUDED

//#include <inttypes.h>
#include "dvd_parse.h"
#include "video_quality.h"
#include "dvr_ifo_livepause_limit.h"
//#undef ATTRIBUTE_PACKED
//#undef PRAGMA_PACK_BEGIN 
//#undef PRAGMA_PACK_END
//
//#if defined(__GNUC__)
//#if __GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95)
//#define ATTRIBUTE_PACKED __attribute__ ((packed))
//#define PRAGMA_PACK 0
//#endif
//#endif
//

#define USE_MMAP
//#define USE_DVD_FILE
//#define READ_ONLY

#if !defined(ATTRIBUTE_PACKED)
#define ATTRIBUTE_PACKED
#define PRAGMA_PACK 1
#endif

#if PRAGMA_PACK
#pragma pack(1)
#endif


#define DVR_MANGR_IFO		"VR_MANGR.IFO"
#define DVR_MANGR_BUP		"VR_MANGR.BUP"
#define DVR_VMG_ID			"DVD_RTR_VMG0"
#define	DVR_MANGR_IFO_MMAP	"vr_ifo_mmap.dat"
#define	DVR_MANGR_IFO_MMAP_BAK	"vr_ifo_mmap.dat.bak"
#define	DVR_TMP_MANGR_IFO_MMAP	"vr_tmp_ifo_mmap.dat"
#define DVR_VERN			0x11				// version 1.1
#define DVR_VT_MOVIE		0x00				// VRO type
#define DVR_VT_STILL		0x01
#define DVR_VT_AUDIO		0x02
#define DVR_MOVIE_VRO		"VR_MOVIE.VRO"
#define DVR_STILL_VRO		"VR_STILL.VRO"
#define DVR_AUDIO_VRO		"VR_AUDIO.VRO"

#define DVR_RESERVED_FREE_BLK	35840  // 70MB

#define DVR_MAX_PL_SRP		99
#define DVR_MAX_ORG_PG		99
#define DVR_MAX_UD_PGC		99
#define DVR_MAX_VOB_STI		64
#define DVR_MAX_M_C			999
#define DVR_MAX_M_C_STEP	1998	//99
#define DVR_MAX_S_C			999
#define DVR_MAX_S_C_STEP	99
#define DVR_MAX_C_EPI		999
//#define DVR_MAX_M_C_EPI		999
#define DVR_MAX_C			1998
#define DVR_STEP_M_C_EPI	99
//#define DVR_MAX_S_C_EPI		999
#define DVR_STEP_S_C_EPI	99
#define DVR_MAX_M_VOB		999
#define DVR_MAX_S_VOG		999
#define DVR_STEP_VOBU_ENT	360 * 20
#define DVR_STEP_TM_ENT		360
#define DVR_MAX_M_VOBU		65535	
#define DVR_MAX_NAME_FIELD  64

//===== for HDD =============================================
#define DVR_MAX_VOB_STI_EXT	255		// <-- 64
#define DVR_MAX_C_EPI_EXT	2000	//999		// <-- 999
#define DVR_MAX_C_EXT		2000	//999		// <-- 1998
#define DVR_MAX_M_C_EXT		DVR_MAX_C_EXT	//999		// <-- 999
#define DVR_MAX_S_C_EXT		DVR_MAX_C_EXT	//999		// <-- 999
#define DVR_MAX_M_VOB_EXT	DVR_MAX_C_EXT	//999		// <-- 999
#define DVR_MAX_S_VOG_EXT	DVR_MAX_C_EXT	//999		// <-- 999
#define DVR_MAX_ORG_PG_EXT	999		// <-- 99
#define DVR_MAX_UD_PGC_EXT	999		// <-- 99
#define DVR_MAX_PL_SRP_EXT	999		// <-- 99
#define DVR_MAX_MVOBU_EXT	4000	//14400	//	57600 // 59593
#define	DVR_MAX_TM_EXT		200		//720		// = (14400/20)	// 2880 = (57600/20) // 2980 = (59593/20)
#define	DVR_MAX_SVOBU_EXT	64
#define	DVR_MAX_C_EP_NS_EXT	100		

//===== for LivePause =============================================
#define DVR_MAX_VOB_STI_LP	DVR_LP_CELL_LIMIT //30		// <-- 64
#define DVR_MAX_C_EPI_LP	DVR_LP_CELL_LIMIT //30	//999		// <-- 999
#define DVR_MAX_C_LP		DVR_LP_CELL_LIMIT //30	//999		// <-- 1998
#define DVR_MAX_M_C_LP		DVR_LP_CELL_LIMIT //30	//999		// <-- 999
#define DVR_MAX_S_C_LP		1	//999		// <-- 999
#define DVR_MAX_M_VOB_LP	DVR_LP_CELL_LIMIT //30	//999		// <-- 999
#define DVR_MAX_S_VOG_LP	1	//999		// <-- 999
#define DVR_MAX_ORG_PG_LP	1		// <-- 99
#define DVR_MAX_UD_PGC_LP	1		// <-- 99
#define DVR_MAX_PL_SRP_LP	1		// <-- 99
#define DVR_MAX_MVOBU_LP	DVR_LP_VOBU_LIMIT //900	//14400	//	57600 // 59593
#define	DVR_MAX_TM_LP		DVR_LP_VOBU_LIMIT/20 //45		//720		// = (14400/20)	// 2880 = (57600/20) // 2980 = (59593/20)
#define	DVR_MAX_SVOBU_LP	1
#define	DVR_MAX_C_EP_NS_LP	1
#define DVR_MAX_C_BLK_SZ_LP 5120
//=======================================================

#define DVR_RECFLG_DEFAULT_EP 		0x01 << 31
#define DVR_RECFLG_DEFAULT_REP_PICT	0x01 << 30
#define DVR_RECFLG_CHANNEL 			0x01 << 29	// neco+
#define DVR_RECFLG_RECORDING_MODE	0x01 << 28	// neco+

#define DVR_CHANNEL_LINE	1	// neco+
#define DVR_CHANNEL_DV		2	// neco+
#define DVR_CHANNEL_CH 		3	// neco+

#define DVR_UNKNOW_PGC 255
#define DVR_ORG_PGC	0
#define DVR_UD_PGC  1
#define DVR_M_CELL	0				// Movie cell
#define DVR_S_CELL	1				// Still picture cell

#define DVR_CHRS_ISO_646    0x00
#define DVR_CHRS_ISO_8859   0x11
#define DVR_CHRS_SHIFT_JIS  0x12
#define DVR_CHRS_KS_C_5601  0x13
#define DVR_CHRS_KS_C_5700  0x14

//============== neco +==================================================
#define DVR_NTSC_FRAME_INTERVAL 3003 
#define DVR_PAL_FRAME_INTERVAL 3600  
#define DVR_ONE_SECOND_INTERVAL 90000
#define DVR_TVS_525_60 0x00 
#define DVR_TVS_625_50 0x01 
#define DVR_TVS_FPS_NTSC	30	
#define DVR_TVS_FPS_PAL		25	

#define MOVIE_PL 0x00 
#define STILL_PL 0x01 
#define HYBRID_PL 0x02 

#define PG_NOT_PROTECT_STATE 0 
#define PG_PROTECT_STATE 1 

#define S_VOB_ENT_TYPE_A 0x00 
#define S_VOB_ENT_TYPE_B 0x01 
#define S_VOB_ENT_TYPE_C 0x02 
#define S_VOB_ENT_TYPE_D 0x03
#define Type_Delete 0 
#define Type_A_B_Delete 1 

#define DVR_IFO_TYPE_NORMAL 0	
#define DVR_IFO_TYPE_EXTEND 1	
#define DVR_IFO_TYPE_READONLY 2	
#define DVR_IFO_TYPE_SUPERIFO 3	

#define DVR_MAX_VOBU_SZ		((HQ_PEAKBITRATE+EP_AUDIO_DATARATE)*11/10)/16 // 562500 // = 0.5(sec) * 9(MBits/sec) * 1000000 / 8(bits/byte)
//687600 = 0.5(sec) * 11(MBits/sec) * 1000000 / 8(bits/byte)
//630000 (10.08Mbps)


//=====================================================================

#define byte unsigned char	

#define DVR_TY_PG_OLD	0
#define DVR_TY_PG_NEW	1

//======neco+========================
enum VRDBKey{
	dbVrKeyDVD = 0,
	dbVrKeyHDD,
	KeyNum = 2
};

enum tyVRMapFile{
	tyVRMap,
	tyVRMapBak
};

typedef struct ch_data_t {
	uint16_t	i16Ch_num;
	char*		Ch_name;
} ch_data_t;

//===================================

typedef struct check_ifo_is_open {
	int			nIsIFOOpenChecked ;
	int			nfhIfoMap;			// for loading to MMAP at 1st Opening
	uint32_t	i32IfoMapData;
	uint32_t	i32IfoMapSize;
	byte		byIsMMAPFileOpened;

} check_ifo_is_open;

//===================================

typedef struct dtdf_t {
	uint16_t	i16Year;
	uint8_t		i8Month;
	uint8_t		i8Day;
	uint8_t		i8Hour;
	uint8_t		i8Minute;
	uint8_t		i8Second;
} dtdf_t;

typedef struct c_ty_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		C_TY1 : 3;
	uint8_t		zero : 5;
#else
	uint8_t		zero : 5;
	uint8_t		C_TY1 : 3;
#endif
} c_ty_t;

typedef struct ep_ty_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		EP_TY1 : 2;
	uint8_t		zero : 6;
#else
	uint8_t		zero : 6;
	uint8_t		EP_TY1 : 2;
#endif
} ep_ty_t;

typedef struct pg_ty_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		Protect : 1;
	uint8_t		NewPG : 1;
	uint8_t		CopyRight: 2;
	uint8_t		Child_Lock : 1;
	uint8_t		Edit_Status : 1;
	uint8_t		zero : 2;
#else
	uint8_t		zero : 2;
	uint8_t		Edit_Status : 1;
	uint8_t		Child_Lock : 1;
	uint8_t		CopyRight: 2;
	uint8_t		NewPG : 1;
	uint8_t		Protect : 1;
#endif
} pg_ty_t;

typedef struct aa_ty_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		zero_1 : 2;
	uint8_t		TE : 1;
	uint8_t		zero_2 : 5;
#else
	uint8_t		zero_2 : 5;
	uint8_t		TE : 1;
	uint8_t		zero_1 : 2;
#endif
} aa_ty_t;

typedef struct s_vob_ent_ty_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		MAP_TY : 2;
	uint8_t		TE : 1;
	uint8_t		zero : 4;
	uint8_t		SPST_Ns : 1;
#else
	uint8_t		SPST_Ns : 1;
	uint8_t		zero : 4;
	uint8_t		TE : 1;
	uint8_t		MAP_TY : 2;
#endif
} s_vob_ent_ty_t;

typedef struct oa_atr_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	ACM : 3;
	uint16_t	zero : 5;
	uint16_t	QuanDRC : 2;
	uint16_t	fs : 2;
	uint16_t	AudioChannelNs : 4;
#else
	uint16_t	AudioChannelNs : 4;
	uint16_t	fs : 2;
	uint16_t	QuanDRC : 2;
	uint16_t	zero : 5;
	uint16_t	ACM : 3;
#endif
} oa_atr_t;

typedef struct ptm_t {
	uint32_t	i32PTM_base;
	uint16_t	i16PTM_extension;
} ptm_t;

typedef struct rep_nm_t {
	char		nm1stfld[64];
	char		nm2ndfld[64];
} rep_nm_t;

typedef struct {
		uint8_t	i8S_VOB_ENTN;
		uint8_t zero[5];
} s_pict_pt_t;

//typedef struct {
//		uint32_t i32PTM_base;
//		uint16_t i16PTM_extension;
//} m_pict_pt_t;

union pict_pt_t {
	ptm_t		M_PICT_PT;
	s_pict_pt_t	S_PICT_PT;
};

typedef struct rep_picti_t {
	uint16_t	i16CN;
	union pict_pt_t	PICT_PT;
} rep_picti_t;

typedef struct disc_rep_picti_t {
	uint8_t		i8PGCN;
	uint16_t	i16PGN;
	uint16_t	i16CN;
	union pict_pt_t	PICT_PT;
	uint8_t		REP_PICT_CREATE_TM[5];
} disc_rep_picti_t;

typedef struct disc_rep_picti_disc_t {
	uint8_t		i8PGCN;
	uint8_t		i8PGN;
	uint16_t	i16CN;
	union pict_pt_t	PICT_PT;
	uint8_t		REP_PICT_CREATE_TM[5];
} disc_rep_picti_disc_t;

typedef struct tm_zone_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	TZ_TY : 4;
	uint16_t	TZ_OFFSET : 12;
#else
	uint16_t	TZ_OFFSET : 12;
	uint16_t	TZ_TY : 4;
#endif
} tm_zone_t;

typedef struct a_atr_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	ACM : 3;
	uint16_t	zero : 3;
	uint16_t	AF : 2;
	uint16_t	QuanDRC : 2;
	uint16_t	fs : 2;
	uint16_t	AudioChannelNs : 4;
#else
	uint16_t	AudioChannelNs : 4;
	uint16_t	fs : 2;
	uint16_t	QuanDRC : 2;
	uint16_t	AF : 2;


	uint16_t	zero : 3;
	uint16_t	ACM : 3;
#endif
	uint8_t		i8Bitrate;
} a_atr_t;

typedef struct v_atr_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	VCM : 2;
	uint16_t	TVS : 2;
	uint16_t	AR : 2;
	uint16_t	AF : 2;
	uint16_t	line21_1 : 1;
	uint16_t	line21_2 : 1;
	uint16_t	VR : 3;
	uint16_t	zero : 3;
#else
	uint16_t	zero : 3;
	uint16_t	VR : 3;
	uint16_t	line21_2 : 1;
	uint16_t	line21_1 : 1;
	uint16_t	AF : 2;
	uint16_t	AR : 2;
	uint16_t	TVS : 2;
	uint16_t	VCM : 2;
#endif
} v_atr_t;

typedef struct yuv_t {
	uint8_t		i8Y;
	uint8_t		i8Cb;
	uint8_t		i8Cr;
} yuv_t;

typedef struct vob_ty_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	TE : 1;
	uint16_t	A0_STATUS : 2;
	uint16_t	A1_STATUS : 2;
	uint16_t	zero_1 : 3;
	uint16_t	SML_FLG : 1;
	uint16_t	A0_GAP_LOC : 2;
	uint16_t	A1_GAP_LOC : 2;
	uint16_t	zero_2 : 3;
#else
	uint16_t	zero_1 : 3;
	uint16_t	A1_STATUS : 2;
	uint16_t	A0_STATUS : 2;
	uint16_t	TE : 1;
	uint16_t	zero_2 : 3;
	uint16_t	A1_GAP_LOC : 2;
	uint16_t	A0_GAP_LOC : 2;
	uint16_t	SML_FLG : 1;
#endif
} vob_ty_t;

typedef struct cpsi_t {
	uint8_t		CPS;
	uint8_t		VernInfo;
} cpsi_t;

typedef struct etki_t {
	uint8_t		zero_1[3];
#ifdef WORDS_BIGENDIAN
	uint8_t		zero_2 : 7;
	uint8_t		Kte_Stat : 1;
#else
	uint8_t		Kte_Stat : 1;
	uint8_t		zero_2 : 7;
#endif
	uint64_t	i64Kte;
} etki_t;


typedef struct s_c_epi_t {
	ep_ty_t		EP_TY;
	uint8_t		i8S_VOB_ENTN;
	rep_nm_t	*pPRM_TXTI;
} s_c_epi_t;
#define S_C_EPI_SIZE 2

typedef struct s_c_gi_t {
	uint8_t		zero;
	c_ty_t		C_TY;
	uint16_t	i16S_VOGI_SRPN;
	uint16_t	i16C_EPI_Ns;
	uint8_t		i8S_S_VOB_ENTN;
	uint8_t		i8E_S_VOB_ENTN;
} s_c_gi_t;

typedef struct s_ci_t {
	s_c_gi_t	S_C_GI;
	s_c_epi_t	*pS_C_EPI;
} s_ci_t;

typedef struct m_c_epi_t {
	ep_ty_t		EP_TY;
	ptm_t		EP_PTM;
	rep_nm_t	*pPRM_TXTI;		// only for type B
} m_c_epi_t;
#define M_C_EPI_SIZE 7

typedef struct m_c_gi_t {
	uint8_t		zero;
	c_ty_t		C_TY;
	uint16_t	i16M_VOBI_SRPN;
	uint16_t	i16C_EPI_Ns;
	ptm_t		C_V_S_PTM;
	ptm_t		C_V_E_PTM;
} m_c_gi_t;

typedef struct m_ci_t {
	m_c_gi_t	M_C_GI;
	m_c_epi_t	*pM_C_EPI;
} m_ci_t;

typedef struct ci_t {
	uint8_t		zero;
	c_ty_t		CellType;
	void		*pCI;
} ci_t;
#define CI_SIZE 2

typedef struct pgi_t {
	uint8_t		zero;
	pg_ty_t		PG_TY;
	uint16_t	i16C_Ns;
	rep_nm_t	PRM_TXTI;
	uint16_t	i16IT_TXT_SRPN;
	rep_picti_t	REP_PICTI;
} pgi_t;

typedef struct pgc_gi_t {
//	uint8_t		zero;
	uint16_t	i16PG_Ns;
	uint16_t	i16CI_SRP_Ns;
} pgc_gi_t;

typedef struct pgci_t {
	pgc_gi_t	PGC_GI;
	pgi_t		*pPGI;
	ci_t		*pCI;
} pgci_t;

typedef struct mnfi_t {
	char		MNF_ID[32];
	uint8_t		MNFI_REC_TM[5];
	char		*pMNFI_DT;
} mnfi_t;
#define MNFI_SIZE 37

typedef struct mnfi_srp_t {
	uint16_t	i16MNFI_SA;
	uint16_t	i16MNFI_SZ;
} mnfi_srp_t;
#define MNFI_SRP_SIZE 2

typedef struct mnfiti_t {
	uint16_t	i16MNFI_Ns;
	uint32_t	i32MNFIT_EA;
	char		LAST_MNF_ID[32];
} mnfiti_t;

typedef struct mnfit_t {
	mnfiti_t	MNFITI;
	mnfi_srp_t	*pMNFI_SRP;
	mnfi_t		*pMNFI;
} mnfit_t;

typedef struct it_txt_t {
	char		*pIT_TXT;
} it_txt_t;

typedef struct it_txt_srp_t {
	uint16_t	i16IT_TXT_SA;
	uint16_t	i16IT_TXT_SZ;
} it_txt_srp_t;

typedef struct txtdti_t {
	uint8_t		CHRS;
	uint8_t		zero;
	uint16_t	i16IT_TXT_SRP_Ns;
	uint32_t	i32TXTDT_MG_EA;
} txtdti_t;

typedef struct txtdt_mg_t {
	txtdti_t	TXTDTI;
	it_txt_srp_t *pIT_TXT_SRP;
	it_txt_t	*pIT_TXT;
} txtdt_mg_t;

typedef struct ud_pgciti_t {
	uint8_t		zero;
	uint8_t		i8UD_PGCI_SRP_Ns;
	uint32_t	i32UD_PGCIT_EA;
} ud_pgciti_t;

typedef struct ud_pgcit_t {
	ud_pgciti_t	UD_PGCITI;
	pgci_t		*pUD_PGCI;
} ud_pgcit_t;

typedef struct aa_ent_t {
	aa_ty_t		AA_TY;
	uint16_t	i16AA_PART_SZ;
	uint16_t	i16AA_PART_PB_TM;
} aa_ent_t;

typedef struct s_aag_gi_t {
	uint8_t		i8AA_ENT_Ns;
	uint8_t		i8S_AA_STIN;
	uint32_t	i32S_AAG_SA;
} s_aag_gi_t;

typedef struct s_aagi_t {
	s_aag_gi_t	S_AAG_GI;
	aa_ent_t	*pAA_ENT;
} s_aagi_t;

typedef struct s_aafi_t {
	uint16_t	i16S_AAGI_SRP_Ns;
	s_aagi_t	*pS_AAGI;
} s_aafi_t;
#define S_AAFI_SIZE 2

typedef struct s_aa_sti_t {
	oa_atr_t	AA_ATR;
} s_aa_sti_t;

typedef struct s_vob_ent_d_t {
	s_vob_ent_ty_t	S_VOB_ENT_TY;
	uint8_t		i8V_PART_SZ;
	uint16_t	i16OA_PART_SZ;
	uint16_t	i16OA_PART_PB_TM;
	uint16_t	i16S_AAGI_SRPN;
	uint8_t		AA_ENTN;
} s_vob_ent_d_t;

typedef struct s_vob_ent_c_t {
	s_vob_ent_ty_t	S_VOB_ENT_TY;
	uint8_t		i8V_PART_SZ;
	uint16_t	i16S_AAGI_SRPN;
	uint8_t		i8AA_ENTN;
} s_vob_ent_c_t;

typedef struct s_vob_ent_b_t {
	s_vob_ent_ty_t	S_VOB_ENT_TY;
	uint8_t		i8V_PART_SZ;
	uint16_t	i16OA_PART_SZ;
	uint16_t	i16OA_PART_PB_TM;
} s_vob_ent_b_t;

typedef struct s_vob_ent_a_t {
	s_vob_ent_ty_t	S_VOB_ENT_TY;
	uint8_t		i8V_PART_SZ;
} s_vob_ent_a_t;

typedef struct s_vob_ent_t {
	uint8_t		i8Type;
	void		*pS_VOB_ENT;
} s_vob_ent_t;

typedef struct s_vog_gi_t {
	uint8_t		i8S_VOB_Ns;
	uint8_t		i8S_VOB_STIN;
	uint8_t		FIRST_VOB_REC_TM[5];
	uint8_t		LAST_VOB_REC_TM[5];
	uint32_t	i32S_VOG_SA;
	tm_zone_t	LOCAL_TM_ZONE;
} s_vog_gi_t;

typedef struct s_vogi_t {
	s_vog_gi_t	S_VOG_GI;
	s_vob_ent_t	*pS_VOB_ENT;
} s_vogi_t;

typedef struct s_avfi_t {
	uint16_t	i16S_VOGI_SRP_Ns;
	s_vogi_t	*pS_VOGI;
} s_avfi_t;
#define S_AVFI_SIZE 2

typedef struct s_vob_sti_t {
	v_atr_t		V_ATR;
	oa_atr_t	OA_ATR;
	uint8_t		zero[2];
	yuv_t		SP_PLT[16];
} s_vob_sti_t;

typedef struct s_avfiti_t {
	uint8_t		i8S_AVFI_Ns;
	uint8_t		i8S_AAFI_Ns;
	uint8_t		i8S_VOB_STI_Ns;
	uint8_t		i8S_AA_STI_Ns;
	uint32_t	i32S_AVFIT_EA;
} s_avfiti_t;

typedef struct s_avfit_t {
	s_avfiti_t	S_AVFITI;
	s_vob_sti_t	*pS_VOB_STI;
	s_avfi_t	S_AVFI;
	s_aa_sti_t	*pS_AA_STI;
	s_aafi_t	S_AAFI;
} s_avfit_t;

typedef struct vobu_ent_t {
	uint8_t		i81STREF_SZ;
#ifdef WORDS_BIGENDIAN
	uint16_t	VOBU_PB_TM : 6;
	uint16_t	VOBU_SZ : 10;
#else
	uint16_t	VOBU_SZ : 10;
	uint16_t	VOBU_PB_TM : 6;
#endif
} vobu_ent_t;

typedef struct tm_ent_t {
	uint16_t	i16VOBU_ENTN;
	uint8_t		i8TM_DIFF;
	uint32_t	i32VOBU_ADR;
} tm_ent_t;

typedef struct tmap_gi_t {
	uint16_t	i16TM_ENT_Ns;
	uint16_t	i16VOBU_ENT_Ns;
	uint16_t	i16TM_OFS;
	uint32_t	i32ADR_OFS;
} tmap_gi_t;

typedef struct tmapi_t {
	tmap_gi_t	TMAP_GI;
	tm_ent_t	*pTM_ENT;
	vobu_ent_t	*pVOBU_ENT;
} tmapi_t;

typedef struct agapi_t {
	ptm_t		VOB_A_STP_PTM[2];
	uint16_t	i16VOB_A_GAP_LEN[2];
} agapi_t;

typedef struct smli_t {
	ptm_t		VOB_FIRST_SCR;
	ptm_t		PREV_VOB_LAST_SCR;
} smli_t;

typedef struct m_vob_gi_t {
	vob_ty_t	VOB_TY;
	uint8_t		VOB_REC_TM[5];
	uint8_t		i8VOB_REC_TM_SUB;
	uint8_t		i8M_VOB_STIN;
	ptm_t		VOB_V_S_PTM;
	ptm_t		VOB_V_E_PTM;
	tm_zone_t	LOCAL_TM_ZONE;
} m_vob_gi_t;

typedef struct m_vobi_t {
	m_vob_gi_t	M_VOB_GI;
	smli_t		*pSMLI;
	agapi_t		*pAGAPI;
	tmapi_t		TMAPI;
} m_vobi_t;
#define M_VOBI_SIZE 23

typedef struct m_avfi_t {
	uint16_t	i16M_VOBI_SRP_Ns;
	m_vobi_t	*pM_VOBI;
} m_avfi_t;
#define M_AVFI_SIZE 2

typedef struct m_vob_sti_t {
	v_atr_t		V_ATR;
	uint8_t		i8AST_Ns;
	uint8_t		i8SPST_Ns;
	a_atr_t		A_ATR0;
	a_atr_t		A_ATR1;
	uint8_t		zero[2];
	yuv_t		SP_PLT[16];
} m_vob_sti_t;

typedef struct m_avfiti_t {
	uint8_t		zero[2];
	uint8_t		i8M_AVFI_Ns;
	uint8_t		i8M_VOB_STI_Ns;
	uint32_t	i32M_AVFIT_EA;
} m_avfiti_t;

typedef struct m_avfit_t {
	m_avfiti_t	M_AVFITI;
	m_vob_sti_t *pM_VOB_STI;
	m_avfi_t	M_AVFI;
} m_avfit_t;

typedef struct pl_ty_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		PL_TY1 : 4;
	uint8_t		zero : 4;
#else
	uint8_t		zero : 4;
	uint8_t		PL_TY1 : 4;
#endif
} pl_ty_t;

typedef struct pl_srp_t {
	uint8_t		zero;
	pl_ty_t		PL_TY;
	uint8_t		i8PGCN;
	uint8_t		PL_CREATE_TM[5];
	rep_nm_t	PRM_TXTI;
	uint16_t	i16IT_TXT_SRPN;
	rep_picti_t REP_PICTI;
} pl_srp_t;

typedef struct pl_srpti_t {
	uint8_t		zero[3];
	uint8_t		i8PL_SRP_Ns;
	uint32_t	i32PL_SRPT_EA;
} pl_srpti_t;

typedef struct pl_srpt_t {
	pl_srpti_t	PL_SRPTI;
	pl_srp_t	*pPL_SRP;
} pl_srpt_t;

typedef struct rtr_vmgi_mat_t {
	char		VMG_ID[12];
	uint32_t	i32RTR_VMG_EA;
	uint8_t		zero_1[12];
	uint32_t	i32VMGI_EA;
	uint8_t		zero_2;
	uint8_t		VERN;
	uint8_t		zero_3[30];
	tm_zone_t	TM_ZONE;
	uint8_t		i8STILL_TM;
	uint8_t		CHRS;
	disc_rep_picti_t RSM_MRKI;
	disc_rep_picti_t DISC_REP_PICTI;
	rep_nm_t	DISC_REP_NM;
	uint8_t		zero_4[30];
	uint32_t	i32M_AVFIT_SA;
	uint32_t	i32S_AVFIT_SA;
	etki_t		ETKI;
	cpsi_t		CPSI;	//copy protection
	uint8_t		zero_5[26];
	uint32_t	i32ORG_PGCI_SA;
	uint32_t	i32UD_PGCIT_SA;
	uint8_t		zero_6[40];
	uint32_t	i32TXTDT_MG_SA;
	uint32_t	i32MNFIT_SA;
	uint8_t		zero_7[152];
} rtr_vmgi_mat_t;
#define RTR_VMGI_MAT_SIZE 514

typedef struct rtr_vmgi_mat_disc_t {
	char		VMG_ID[12];
	uint32_t	i32RTR_VMG_EA;
	uint8_t		zero_1[12];
	uint32_t	i32VMGI_EA;
	uint8_t		zero_2;
	uint8_t		VERN;
	uint8_t		zero_3[30];
	tm_zone_t	TM_ZONE;
	uint8_t		i8STILL_TM;
	uint8_t		CHRS;
	disc_rep_picti_disc_t RSM_MRKI;
	disc_rep_picti_disc_t DISC_REP_PICTI;
	rep_nm_t	DISC_REP_NM;
	uint8_t		zero_4[30];
	uint32_t	i32M_AVFIT_SA;
	uint32_t	i32S_AVFIT_SA;
	etki_t		ETKI;
	cpsi_t		CPSI;	//copy protection
	uint8_t		zero_5[26];
	uint32_t	i32ORG_PGCI_SA;
	uint32_t	i32UD_PGCIT_SA;
	uint8_t		zero_6[40];
	uint32_t	i32TXTDT_MG_SA;
	uint32_t	i32MNFIT_SA;
	uint8_t		zero_7[152];
} rtr_vmgi_mat_disc_t;


#ifdef USE_MMAP	
#define TY_VMGI_MAT			0
#define TY_PL_SRPT			TY_VMGI_MAT+1	// 1
#define TY_M_AVFIT			TY_PL_SRPT+1	// 2
#define TY_S_AVFIT			TY_M_AVFIT+1	// 3
#define TY_ORG_PGCI			TY_S_AVFIT+1	// 4
//#define TY_UD_PGCIT		5
//#define TY_PL_SRP			6
#define TY_M_VOB_STI		TY_ORG_PGCI+1	// 5
#define TY_M_VOBI			TY_M_VOB_STI+1	// 6
#define TY_SMLI				TY_M_VOBI+1		// 7
#define TY_AGAPI			TY_SMLI+1		// 8
#define TY_TM_ENT			TY_AGAPI+1		// 9
#define TY_VOBU_ENT			TY_TM_ENT+1		//10
#define TY_S_VOB_STI		TY_VOBU_ENT+1	//11
#define TY_S_AA_STI			TY_S_VOB_STI+1	//12
#define TY_S_VOGI			TY_S_AA_STI+1	//13
#define TY_S_VOB_ENT		TY_S_VOGI+1		//14
#define TY_S_VOB_ENT_ABCD	TY_S_VOB_ENT+1	//15
#define TY_S_AAGI			TY_S_VOB_ENT_ABCD+1 //16
#define TY_AA_ENT			TY_S_AAGI+1		//17
#define TY_PGI				TY_AA_ENT+1		//18
#define TY_CI				TY_PGI+1		//19
#define TY_MS_CI			TY_CI+1			//20
#define TY_MS_C_EPI			TY_MS_CI+1		//21
#define TY_SRP				TY_MS_C_EPI+1	//22
#define TY_MAP_M_INDEX		TY_SRP+1		//23
#define TY_MAP_S_INDEX		TY_MAP_M_INDEX+1		//24
#define TY_MAP_C_INDEX		TY_MAP_S_INDEX+1		//25


/*
#define TY_UD_PGCI		24
#define TY_UD_CI		25
#define TY_UD_MS_CI		26
#define TY_UD_MS_C_EPI		27
*/
#define TY_TOTAL_SZ			TY_MAP_C_INDEX+1	//26

//===== for HDD ===================================================

#define DVR_UNIT_MVOBU_SZ		DVR_MAX_MVOBU_EXT * sizeof(vobu_ent_t) //3
#define DVR_UNIT_TM_SZ			DVR_MAX_TM_EXT * sizeof(tm_ent_t)	//7
#define	DVR_UNIT_S_VOBU_SZ		DVR_MAX_SVOBU_EXT * sizeof(s_vob_ent_t)	//5
#define	DVR_UNIT_S_VOBU_TY_SZ	DVR_MAX_SVOBU_EXT * sizeof(s_vob_ent_d_t)	//9
#define	DVR_UNIT_MS_EPI_SZ		DVR_MAX_C_EP_NS_EXT * sizeof(m_c_epi_t)	//11

//===== for LivePause =============================================

#define DVR_UNIT_MVOBU_SZ_LP		DVR_MAX_MVOBU_LP * sizeof(vobu_ent_t) //3
#define DVR_UNIT_TM_SZ_LP			DVR_MAX_TM_LP * sizeof(tm_ent_t)	//7
#define	DVR_UNIT_S_VOBU_SZ_LP		DVR_MAX_SVOBU_LP * sizeof(s_vob_ent_t)	//5
#define	DVR_UNIT_S_VOBU_TY_SZ_LP	DVR_MAX_SVOBU_LP * sizeof(s_vob_ent_d_t)	//9
#define	DVR_UNIT_MS_EPI_SZ_LP		DVR_MAX_C_EP_NS_LP * sizeof(m_c_epi_t)	//11

//=================================================================

typedef struct table_map_sa{
	uint32_t	i32Table_map_sa[TY_TOTAL_SZ+1];
	byte		*pIfoMapData;
} table_map_sa;
#endif

typedef struct {
	
	int			file;
	int			Map_file;
	
	rtr_vmgi_mat_t	*pVMGI_MAT;
	pl_srpt_t		*pPL_SRPT;
	m_avfit_t		*pM_AVFIT;
	s_avfit_t		*pS_AVFIT;
	pgci_t			*pORG_PGCI;
	ud_pgcit_t		*pUD_PGCIT;
	txtdt_mg_t		*pTXTDT_MG;
	mnfit_t			*pMNFIT;
#ifdef USE_MMAP	
	table_map_sa	TABLE_MAP_DATA;
#endif
	byte	*byIsLoadCheck;
	byte		byIsLivePause;
	byte byExtIFOType;
	
} dvr_ifo_handle_t;

#if PRAGMA_PACK
#pragma pack()
#endif

#endif	//__DVR_IFO_TYPES_H_INCLUDED
