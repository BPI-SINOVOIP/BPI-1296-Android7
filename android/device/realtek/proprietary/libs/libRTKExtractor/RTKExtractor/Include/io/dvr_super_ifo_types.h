#ifndef __DVR_SUPER_IFO_TYPES_H_INCLUDED
#define __DVR_SUPER_IFO_TYPES_H_INCLUDED

//#include "dvd_reader.h"
#ifndef __DVR_IFO_TYPES_H_INCLUDED
#include "io/dvr_ifo_types.h"
#endif


#if !defined(ATTRIBUTE_PACKED)
#define ATTRIBUTE_PACKED
#define PRAGMA_PACK 1
#endif

#if PRAGMA_PACK
#pragma pack(1)
#endif

#define DVR_VMG_ID_SUPER	"SDVD_RTR_VMG"
/*
#define DVR_VERN			0x11				// version 1.1
#define DVR_VT_MOVIE		0x00				// VRO type
#define DVR_VT_STILL		0x01
#define DVR_VT_AUDIO		0x02
#define DVR_MOVIE_VRO		"VR_MOVIE.VRO"
#define DVR_STILL_VRO		"VR_STILL.VRO"
#define DVR_AUDIO_VRO		"VR_AUDIO.VRO"

#define DVR_RESERVED_FREE_BLK	2560
*/
/*
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
#define DVR_STEP_VOBU_ENT	180 * 20
#define DVR_STEP_TM_ENT		180
#define DVR_MAX_NAME_FIELD  64

//=====neco+=============================================
#define DVR_MAX_VOB_STI_EXT	255		// <-- 64
#define DVR_MAX_C_EPI_EXT	65535	// <-- 999
#define DVR_MAX_C_EXT		65535	// <-- 1998
#define DVR_MAX_M_C_EXT		32768	// <-- 999
#define DVR_MAX_S_C_EXT		32767	// <-- 999
#define DVR_MAX_M_VOB_EXT	32768	// <-- 999
#define DVR_MAX_S_VOG_EXT	32767	// <-- 999
#define DVR_MAX_ORG_PG_EXT	255		// <-- 99
#define DVR_MAX_UD_PGC_EXT	255		// <-- 99
#define DVR_MAX_PL_SRP_EXT	255		// <-- 99
//=======================================================
*/
#ifndef USE_MMAP	
#define DVR_STEP_ORG_PG_SUPER	100
#define DVR_STEP_ORG_C_SUPER	100
#define DVR_STEP_VOBU_ENT_SUPER	60 * 20
#define DVR_STEP_TM_ENT_SUPER	60
#endif

#define DVR_MAX_VOB_STI_SUPER	255		// <-- 64
#define DVR_MAX_C_EPI_SUPER		999	// <-- 999
#define DVR_MAX_C_SUPER			999	// <-- 1998
#define DVR_MAX_M_C_SUPER		999	// <-- 999
//#define DVR_MAX_S_C_SUPER	32767	// <-- 999
#define DVR_MAX_M_VOB_SUPER		999	// <-- 999
//#define DVR_MAX_S_VOG_SUPER	32767	// <-- 999
#define DVR_MAX_ORG_PG_SUPER	999	// <-- 99
#define DVR_MAX_UD_PGC_SUPER	255		// <-- 99
#define DVR_MAX_PL_SRP_SUPER	255		// <-- 99
#define DVR_MAX_MVOBU_SUPER		65530
#define	DVR_MAX_TM_SUPER		3276	// (65530 /20)
//#define DVR_MAX_FILE_HOUR_SUPER	8

#ifdef USE_MMAP	

//#define DVR_UNIT_MVOBU_SZ_SUPER		DVR_MAX_MVOBU_SUPER * 13
//#define DVR_UNIT_TM_SZ_SUPER		DVR_MAX_TM_SUPER * 3
//#define	DVR_UNIT_MS_EPI_SZ_SUPER		DVR_MAX_C_EPI_EXT * 11

#define TY_SUPER_VMGI_MAT			0
//#define TY_SUPER_PL_SRPT		TY_SUPER_VMGI_MAT+1		// 1
#define TY_SUPER_M_AVFIT		TY_SUPER_VMGI_MAT+1		// 1
//#define TY_SUPER_S_AVFIT		TY_SUPER_M_AVFIT+1		// 2
#define TY_SUPER_ORG_PGCI		TY_SUPER_M_AVFIT+1		// 2
//#define TY_SUPER_UD_PGCIT		5
//#define TY_SUPER_PL_SRP		6
#define TY_SUPER_M_VOB_STI		TY_SUPER_ORG_PGCI+1		// 3
#define TY_SUPER_M_VOBI			TY_SUPER_M_VOB_STI+1	// 4
//#define TY_SUPER_SMLI			TY_SUPER_M_VOBI+1		// 7
//#define TY_SUPER_AGAPI		TY_SUPER_SMLI+1			// 8
#define TY_SUPER_TM_ENT			TY_SUPER_M_VOBI+1		// 5
#define TY_SUPER_VOBU_ENT		TY_SUPER_TM_ENT+1		// 6
/*
#define TY_SUPER_S_VOB_STI		TY_SUPER_VOBU_ENT+1		// 9
#define TY_SUPER_S_AA_STI		TY_SUPER_S_VOB_STI+1	//10
#define TY_SUPER_S_VOGI			TY_SUPER_S_AA_STI+1		//11
#define TY_SUPER_S_VOB_ENT		TY_SUPER_S_VOGI+1		//12
#define TY_SUPER_S_VOB_ENT_ABCD	TY_SUPER_S_VOB_ENT+1	//13
#define TY_SUPER_S_AAGI			TY_SUPER_S_VOB_ENT_ABCD+1 //14
#define TY_SUPER_AA_ENT			TY_SUPER_S_AAGI+1		//15
*/
#define TY_SUPER_PGI			TY_SUPER_VOBU_ENT+1		// 7
#define TY_SUPER_CI				TY_SUPER_PGI+1			// 8
#define TY_SUPER_MS_CI			TY_SUPER_CI+1			// 9
#define TY_SUPER_MS_C_EPI		TY_SUPER_MS_CI+1		//10
#define TY_SUPER_SRP			TY_SUPER_MS_C_EPI+1		//11
/*
#define TY_SUPER_UD_PGCI		24
#define TY_SUPER_UD_CI		25
#define TY_SUPER_UD_MS_CI		26
#define TY_SUPER_UD_MS_C_EPI		27
*/
#define TY_SUPER_TOTAL_SZ		TY_SUPER_SRP+1			//12

typedef struct super_table_map_sa{
	uint32_t	i32Table_map_sa[TY_SUPER_TOTAL_SZ+1];
	byte		*pIfoMapData;
} super_table_map_sa;
#endif

/*
#define DVR_RECFLG_DEFAULT_EP 		0x01 << 31
#define DVR_RECFLG_DEFAULT_REP_PICT	0x01 << 30
#define DVR_RECFLG_CHANNEL 			0x01 << 29	// neco+
#define DVR_RECFLG_RECORDING_MODE	0x01 << 28	// neco+

#define DVR_CHANNEL_LINE	1	// neco+
#define DVR_CHANNEL_DV		2	// neco+
#define DVR_CHANNEL_CH 		3	// neco+


#define DVR_ORG_PGC	0
#define DVR_UD_PGC  1
#define DVR_M_CELL	0				// Movie cell
#define DVR_S_CELL	1				// Still picture cell

#define DVR_CHRS_ISO_646    0x00
#define DVR_CHRS_ISO_8859   0x11
#define DVR_CHRS_SHIFT_JIS  0x12
#define DVR_CHRS_KS_C_5601  0x13
#define DVR_CHRS_KS_C_5700  0x14

#define DVR_NTSC_FRAME_INTERVAL 3003 //neco+
#define DVR_PAL_FRAME_INTERVAL 3600  //neco+
#define DVR_TVS_525_60 0x00 //neco+
#define DVR_TVS_625_50 0x01 //neco+

#define MOVIE_PL 0x00 //neco+
#define STILL_PL 0x01 //neco+
#define HYBRID_PL 0x02 //neco+

#define PG_NOT_PROTECT_STATE 0 //neco+
#define PG_PROTECT_STATE 1 //neco+

#define S_VOB_ENT_TYPE_A 0x00 //neco+
#define S_VOB_ENT_TYPE_B 0x01 //neco+
#define S_VOB_ENT_TYPE_C 0x02 //neco+
#define S_VOB_ENT_TYPE_D 0x03 //neco+
#define Type_Delete 0 //neco+
#define Type_A_B_Delete 1 //neco+

#define DVR_IFO_TYPE_NORMAL 0	//neco+
#define DVR_IFO_TYPE_EXTEND 1	//neco+
#define DVR_IFO_TYPE_SUPERIFO 2	//neco+
//======neco+========================
enum VRDBKey{
	dbVrKeyDVD = 0,
	dbVrKeyHDD,
	KeyNum = 2
};

typedef struct ch_data_t {
	uint16_t	i16Ch_num;
	char*		Ch_name;
} ch_data_t;

//===================================

typedef struct dtdf_t {
	uint16_t	i16Year;
	uint8_t		i8Month;
	uint8_t		i8Day;
	uint8_t		i8Hour;
	uint8_t		i8Minute;
	uint8_t		i8Second;
} dtdf_t;
*/
/*
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
	uint8_t		zero : 7;
#else
	uint8_t		zero : 7;
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
*/
typedef struct super_ptm_t {
	uint64_t		i64PTM_base;
	uint16_t	i16PTM_extension;
} super_ptm_t;
/*
typedef struct rep_nm_t {
	char		nm1stfld[64];
	char		nm2ndfld[64];
} rep_nm_t;
*/
typedef struct {
		uint8_t	i8S_VOB_ENTN;
		uint8_t zero[9];
} super_s_pict_pt_t;

/*
//typedef struct {
//		uint32_t i32PTM_base;
//		uint16_t i16PTM_extension;
//} m_pict_pt_t;
*/
union super_pict_pt_t {
	super_ptm_t	M_PICT_PT;
	super_s_pict_pt_t	S_PICT_PT;
};

typedef struct super_rep_picti_t {
	uint16_t	i16CN;
	union super_pict_pt_t	PICT_PT;
} super_rep_picti_t;

typedef struct super_disc_rep_picti_t {
	uint8_t		i8PGCN;
	uint8_t		i8PGN;
	uint16_t	i16CN;
	union super_pict_pt_t	PICT_PT;
	uint8_t		REP_PICT_CREATE_TM[5];
} super_disc_rep_picti_t;

typedef struct super_a_atr_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	ACM : 4;
	uint16_t	zero : 4;
	uint16_t	fs : 4;
	uint16_t	AudioChannelNs : 4;
#else
	uint16_t	AudioChannelNs : 4;
	uint16_t	fs : 4;
	uint16_t	zero : 4;
	uint16_t	ACM : 4;
#endif
	uint8_t		i8Bitrate;
} super_a_atr_t;

typedef struct super_v_atr_t {
#ifdef WORDS_BIGENDIAN
	uint16_t	VCM    : 4;
	uint16_t	FR     : 3;
	uint16_t	zero_1 : 1;
	uint16_t	AR 	   : 2;
	uint16_t	zero_2 : 6;
#else
	uint16_t	zero_2 : 6;
	uint16_t	AR     : 2;
	uint16_t	zero_1 : 1;
	uint16_t	FR 	   : 3;
	uint16_t	VCM    : 4;
#endif
	uint16_t	i16Height;
	uint16_t	i16Width;
} super_v_atr_t;

/*
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
*/
typedef struct super_m_c_epi_t {
	ep_ty_t		EP_TY;
	super_ptm_t	EP_PTM;
	rep_nm_t	*pPRM_TXTI;		// only for type B
} super_m_c_epi_t;

#define SUPER_M_C_EPI_SIZE 11

typedef struct super_m_c_gi_t {
	uint8_t		zero;
	c_ty_t		C_TY;
	uint16_t	i16M_VOBI_SRPN;
	uint16_t	i16C_EPI_Ns;
	super_ptm_t		C_V_S_PTM;
	super_ptm_t		C_V_E_PTM;
} super_m_c_gi_t;

typedef struct super_m_ci_t {
	super_m_c_gi_t	M_C_GI;
	super_m_c_epi_t	*pM_C_EPI;
} super_m_ci_t;
/*
typedef struct ci_t {
	uint8_t		zero;
	c_ty_t		CellType;
	void		*pCI;
} ci_t;
#define CI_SIZE 2
*/
typedef struct super_pgi_t {
	uint8_t		zero;
	pg_ty_t		PG_TY;
	uint16_t	i16C_Ns;		// 1~65535
	rep_nm_t	PRM_TXTI;
	uint16_t	i16IT_TXT_SRPN;
	super_rep_picti_t	REP_PICTI;		// 0xFF
} super_pgi_t;

typedef struct super_pgc_gi_t {
	uint16_t	i16PG_Ns;		// 0~65535
	uint16_t	i16CI_SRP_Ns;	// 0~65535
} super_pgc_gi_t;

typedef struct super_pgci_t {
	super_pgc_gi_t	PGC_GI;
	super_pgi_t		*pPGI;
	ci_t			*pCI;
} super_pgci_t;
/*
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
	uint16_t	i16IT_TXT_SRP_Ns;	// 1~65535
	uint32_t	i32TXTDT_MG_EA;
} txtdti_t;

typedef struct txtdt_mg_t {
	txtdti_t	TXTDTI;
	it_txt_srp_t *pIT_TXT_SRP;
	it_txt_t	*pIT_TXT;
} txtdt_mg_t;

typedef struct ud_pgciti_t {
	uint8_t		zero;
	uint8_t		i8UD_PGCI_SRP_Ns;	// 1~255
	uint32_t	i32UD_PGCIT_EA;
} ud_pgciti_t;
*/
typedef struct super_ud_pgcit_t {
	ud_pgciti_t			UD_PGCITI;
	super_pgci_t		*pUD_PGCI;
} super_ud_pgcit_t;

/*
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
*/

typedef struct super_vobu_ent_t {
	uint32_t		i321STREF_SZ;
	uint8_t			i8VOBU_PB_TM;
	uint32_t		i32VOBU_SZ;
	uint32_t		i32VOBU_SA;
//	uint8_t				i8PICT_ENTNs;
//	super_poct_ent_t	*pPICT_ENT;
} super_vobu_ent_t;
/*
typedef struct super_poct_ent_t {
	super_pict_atr_t	PICT_ATTR;
	uint32_t			i32PICT_SZ;
} super_poct_ent_t;


typedef struct super_pict_atr_t {
#ifdef WORDS_BIGENDIAN
	uint8_t		ft : 2;	
	uint8_t		FieldNs : 2;
	uint8_t		zero : 4;
#else
	uint8_t		zero : 4;	
	uint8_t		FieldNs : 2;
	uint8_t		ft : 2;
#endif
} super_pict_atr_t;
*/

typedef struct super_tm_ent_t {
	uint16_t	i16VOBU_ENTN;
	uint8_t		i8TM_DIFF;
//	uint32_t	i32VOBU_ADR;
} super_tm_ent_t;

#ifdef USE_MMAP	
#define DVR_UNIT_MVOBU_SZ_SUPER		DVR_MAX_MVOBU_SUPER * sizeof(super_vobu_ent_t)
#define DVR_UNIT_TM_SZ_SUPER		DVR_MAX_TM_SUPER * sizeof(super_tm_ent_t)
#endif

typedef struct super_tmap_gi_t {
	uint16_t	i16TM_ENT_Ns;
	uint16_t	i16VOBU_ENT_Ns;
	uint8_t		zero[6];
} super_tmap_gi_t;

typedef struct super_tmapi_t {
	super_tmap_gi_t	TMAP_GI;
	super_tm_ent_t		*pTM_ENT;
	super_vobu_ent_t	*pVOBU_ENT;
} super_tmapi_t;

typedef struct super_m_vob_gi_t {
	uint8_t		zero_1[8];
	uint8_t		i8M_VOB_STIN;	// 1~255
	super_ptm_t	VOB_V_S_PTM;
	super_ptm_t	VOB_V_E_PTM;
	uint8_t		zero_2[2];
} super_m_vob_gi_t;

typedef struct super_m_vobi_t {
	super_m_vob_gi_t	M_VOB_GI;
	super_tmapi_t		TMAPI;
} super_m_vobi_t;
#define SUPER_M_VOBI_SIZE 31	//23

typedef struct super_m_avfi_t {
	uint16_t	i16M_VOBI_SRP_Ns;
	super_m_vobi_t	*pM_VOBI;
} super_m_avfi_t;
//#define M_AVFI_SIZE 2

typedef struct super_m_vob_sti_t {
	super_v_atr_t		V_ATR;
	uint8_t				i8AST_Ns;
	uint8_t				zero_1;
	super_a_atr_t		A_ATR0;
	uint8_t				zero_2[53];
} super_m_vob_sti_t;

typedef struct super_m_avfiti_t {
	uint8_t		zero[3];
	uint8_t		i8M_VOB_STI_Ns;			// 0~255
	uint32_t	i32M_AVFIT_EA;
} super_m_avfiti_t;

typedef struct super_m_avfit_t {
	super_m_avfiti_t	M_AVFITI;
	super_m_vob_sti_t	*pM_VOB_STI;
	super_m_avfi_t		M_AVFI;
} super_m_avfit_t;

typedef struct super_pl_srp_t {
	uint8_t		zero[2];
	uint8_t		i8PGCN;					// 1~255
	uint8_t		PL_CREATE_TM[5];
	rep_nm_t	PRM_TXTI;
	uint16_t	i16IT_TXT_SRPN;
	super_rep_picti_t REP_PICTI;				// 0xFF
} super_pl_srp_t;
/*
typedef struct pl_srpti_t {
	uint8_t		zero[3];
	uint8_t		i8PL_SRP_Ns;			// 0~255
	uint32_t	i32PL_SRPT_EA;
} pl_srpti_t;
*/
typedef struct super_pl_srpt_t {
	pl_srpti_t	PL_SRPTI;
	super_pl_srp_t	*pPL_SRP;
} super_pl_srpt_t;

typedef struct super_rtr_vmgi_mat_t {
	char		VMG_ID[12];				// "SDVD_RTR_VMG"
	uint32_t	i32RTR_VMG_EA;
	uint8_t		zero_1[12];
	uint32_t	i32VMGI_EA;
	uint8_t		VERN;
	uint8_t		zero_2[35];
	//super_disc_rep_picti_t RSM_MRKI;			// 0xFF
	//super_disc_rep_picti_t DISC_REP_PICTI;	// 0xFF
	uint8_t		zero_3[188];
	uint32_t	i32M_AVFIT_SA;
	uint32_t	i32S_AVFIT_SA;
	uint8_t		zero_4[40];	
	uint32_t	i32ORG_PGCI_SA;
	uint32_t	i32UD_PGCIT_SA;
	uint8_t		zero_5[40];
	uint32_t	i32TXTDT_MG_SA;
	uint32_t	i32MNFIT_SA;
	uint8_t		zero_6[152];
} super_rtr_vmgi_mat_t;
#define SUPER_RTR_VMGI_MAT_SIZE 512

typedef struct {
	//dvd_file_t *file;
	int			file;
	int			Map_file;
	
	super_rtr_vmgi_mat_t	*pVMGI_MAT;
	super_pl_srpt_t			*pPL_SRPT;
	super_m_avfit_t			*pM_AVFIT;
	s_avfit_t				*pS_AVFIT;
	super_pgci_t			*pORG_PGCI;
	super_ud_pgcit_t		*pUD_PGCIT;
	txtdt_mg_t				*pTXTDT_MG;
	mnfit_t					*pMNFIT;
#ifdef USE_MMAP	
	super_table_map_sa	TABLE_MAP_DATA;
#endif	
} dvr_super_ifo_handle_t;

#if PRAGMA_PACK
#pragma pack()
#endif

#endif	//__DVR_IFO_TYPES_H_INCLUDED
