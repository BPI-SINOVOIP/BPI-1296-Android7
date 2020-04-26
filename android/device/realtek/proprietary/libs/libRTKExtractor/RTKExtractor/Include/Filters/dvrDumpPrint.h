#ifndef __DVR_DUMP_PRINT_H_INCLUDED
#define __DVR_DUMP_PRINT_H_INCLUDED


#ifndef __DVR_IFO_TYPES_H_INCLUDED
#    include <io/dvr_ifo_types.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

//void dvrDump_RTR_VMGI(dvr_ifo_handle_t *pIfoHandle);
//int dvrDump_IFO(dvr_ifo_handle_t *pIfoHandle); //neco
int dvrDump_IFO(dvr_ifo_handle_t *pIfoHandle, char *command); //neco

#ifdef __cplusplus
};
#endif
#endif	//__DVR_DUMP_PRINT_H_INCLUDED

#define CMD_LINE_DIVIDER " \t"

#define TY_NORMAL_TYPE		9999
#define TY_MNFIT_MNFI_SRP	1
#define TY_MNFIT_MNFI	  	2
#define TY_TXTDT_TXT_SRP	3
#define TY_TXTDT_IT_TXT		4
#define TY_PGC_PG			5
#define	TY_PGC_CELL			6
#define TY_MVOB_STI			7
#define TY_MVOB				8
#define TY_MVOBI_TM_ENT		9
#define TY_MVOBI_VOBU_ENT	10

#define TY_SVOB_STI			11
#define TY_SVOG				12
#define TY_SAA_STI			13
#define TY_SAAGI			14


#define TYPE_GENRE_MOVIE		0x30
#define TYPE_GENRE_MUSIC		0x31
#define TYPE_GENRE_DRAMA		0x32
#define TYPE_GENRE_ANIMATION		0x33
#define TYPE_GENRE_SPORTS		0x34
#define TYPE_GENRE_DOCUMENTARY		0x35
#define TYPE_GENRE_NEWS			0x36
#define TYPE_GENRE_WEATHER		0x37
#define TYPE_GENRE_EDUCATION		0x38
#define TYPE_GENRE_HOBBY		0x39
#define TYPE_GENRE_ENTERTAINMENT	0x3a
#define TYPE_GENRE_ART			0x3b
#define TYPE_GENRE_SHOPPING		0x3c
#define TYPE_SOURCE_BROADCASTONG	0x60
#define TYPE_SOURCE_CAMERA		0x61
#define TYPE_SOURCE_STILLS		0x62
#define TYPE_OTHERS_MEMO		0x80
#define TYPE_OTHERS_OTHERS		0x90
