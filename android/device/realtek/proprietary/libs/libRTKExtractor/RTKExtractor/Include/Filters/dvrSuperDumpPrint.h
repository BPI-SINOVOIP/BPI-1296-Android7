#ifndef __DVR_SUPER_DUMP_PRINT_H_INCLUDED
#define __DVR_SUPER_DUMP_PRINT_H_INCLUDED


#ifndef __DVR_IFO_TYPES_H_INCLUDED
#    include <io/dvr_ifo_types.h>
#endif

#ifndef __DVR_SUPER_IFO_TYPES_H_INCLUDED
#    include <io/dvr_super_ifo_types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

//void dvrDump_RTR_VMGI(dvr_ifo_handle_t *pIfoHandle);
int dvrDump_Super_IFO(dvr_super_ifo_handle_t *pIfoHandle); //neco


#ifdef __cplusplus
};
#endif
#endif	//__DVR_DUMP_PRINT_H_INCLUDED

#define TYPE_GENRE_MOVIE			0x30
#define TYPE_GENRE_MUSIC			0x31
#define TYPE_GENRE_DRAMA			0x32
#define TYPE_GENRE_ANIMATION		0x33
#define TYPE_GENRE_SPORTS			0x34
#define TYPE_GENRE_DOCUMENTARY		0x35
#define TYPE_GENRE_NEWS				0x36
#define TYPE_GENRE_WEATHER			0x37
#define TYPE_GENRE_EDUCATION		0x38
#define TYPE_GENRE_HOBBY			0x39
#define TYPE_GENRE_ENTERTAINMENT	0x3a
#define TYPE_GENRE_ART				0x3b
#define TYPE_GENRE_SHOPPING			0x3c
#define TYPE_SOURCE_BROADCASTONG	0x60
#define TYPE_SOURCE_CAMERA			0x61
#define TYPE_SOURCE_STILLS			0x62
#define TYPE_OTHERS_MEMO			0x80
#define TYPE_OTHERS_OTHERS			0x90
#define TYPE_TRANSITION_EFFECT_FADE		0x40
#define TYPE_TRANSITION_EFFECT_WIPE		0x41
#define TYPE_TRANSITION_EFFECT_SLIDE	0x42
