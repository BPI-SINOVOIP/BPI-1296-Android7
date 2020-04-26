#ifndef __DVR_SUPER_IFO_ACCESS_H_INCLUDED
#define __DVR_SUPER_IFO_ACCESS_H_INCLUDED

#include "dvr_super_ifo_types.h"
//#include "dvd_reader.h"
#include "dvr_ifo_access.h"
#include <sys/ioctl.h>
/*
#ifdef USE_MMAP
#define TABLE_MAP_PTR_SU(tbType)	\
	(pIfoHandle->TABLE_MAP_DATA.pIfoMapData + pIfoHandle->TABLE_MAP_DATA.i32Table_map_sa[tbType])
#endif
*/
#ifdef __cplusplus
extern "C" {
#endif

dvr_super_ifo_handle_t *dvrSuper_IfoCreate(const char* );
dvr_super_ifo_handle_t *dvrSuper_IfoOpen(const char* );
int dvrSuper_IfoWrite(dvr_super_ifo_handle_t *);
void dvrSuper_IfoClose(dvr_super_ifo_handle_t *);

int dvrSuper_FileOpen(const char *);
int dvrSuper_FileCreate(const char *);
size_t IfoReadBytes(int , void *, size_t );
size_t IfoWriteBytes(int , void *, size_t );
void dvrSuper_FileClose(int);

/*
void dvrGetDTDF(uint8_t i8DTDF[5], dtdf_t *pDTDF);
void dvrSetDTDF(dtdf_t *pDTDF, uint8_t i8DTDF[5]);
*/
uint8_t dvrSuperGetCellType(dvr_super_ifo_handle_t *pIfoHandle, 
					   uint8_t i8PGCN, uint16_t i16CN);

#ifdef USE_MMAP
void* dvrIfoGetMMapTableSA_Super(dvr_super_ifo_handle_t *pIfoHandle, int tyTable);

#endif
#ifdef __cplusplus
};
#endif
#endif	//__DVR_IFO_ACCESS_H_INCLUDED
