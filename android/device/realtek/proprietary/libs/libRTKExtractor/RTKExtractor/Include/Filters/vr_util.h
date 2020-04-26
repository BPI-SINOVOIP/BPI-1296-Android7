#ifndef __VR_UTIL_H__
#define __VR_UTIL_H__

#include <../io/dvr_ifo_types.h>

typedef enum {
	VR_TY_VOB_STI,
	VR_TY_C_EPI,
	VR_TY_C,
	VR_TY_M_C,
	VR_TY_S_C,
	VR_TY_M_VOB,
	VR_TY_S_VOG,
	VR_TY_ORG_PG,
	VR_TY_UD_PGC,
	VR_TY_PL_SRP,
	VR_TY_VOBU,
	VR_TY_TM,
	VR_TY_SVOBU,
	VR_TY_C_EP_NS,
	
	VR_TY_UNIT_MVOBU_SZ,
	VR_TY_UNIT_TM_SZ,
	VR_TY_UNIT_S_VOBU_SZ,
	VR_TY_UNIT_S_VOBU_TY_SZ,
	VR_TY_UNIT_MS_EPI_SZ
	
}VR_IFO_TABLE_TYPE;

#ifdef __cplusplus
	extern "C" uint32_t GetIFOMaxValue(byte byIFOTableType);
#else
    uint32_t GetIFOMaxValue(byte byIFOTableType);
#endif


#endif //__VR_UTIL_H__
