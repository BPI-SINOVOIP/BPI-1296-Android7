#ifndef __DVR_IFO_ACCESS_H_INCLUDED
#define __DVR_IFO_ACCESS_H_INCLUDED

#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h> 
#include <assert.h>
#include "dvr_ifo_types.h"
#include "dvd_parse.h"

//#define USE_MMAP

#define DBGSTR(exp) 				\
	{								\
	if (0)							\
		assert(exp);				\
	else							\
		if (!(exp))					\
			printf("[%s:%d]%s() failed -------------------------------------> :(\n", __FILE__, __LINE__, __func__);	\
	}


#ifdef USE_MMAP
#define TABLE_MAP_PTR(tbType)	\
	(pIfoHandle->TABLE_MAP_DATA.pIfoMapData + pIfoHandle->TABLE_MAP_DATA.i32Table_map_sa[tbType])
#endif	


#ifdef __cplusplus
extern "C" {
#endif

dvr_ifo_handle_t *dvrIfoCreate(dvd_info_t *pDvdReader, byte byExtIFOType, byte byIsLivePause);
dvr_ifo_handle_t *dvrIfoOpen(dvd_info_t *pDvdReader, byte byExtIFOType, byte byOpentype, byte byIsLivePausePath);
int dvrIfoWrite(dvd_info_t *, dvr_ifo_handle_t *);
void dvrIfoClose(dvr_ifo_handle_t *);

void dvrGetDTDF(uint8_t i8DTDF[5], dtdf_t *pDTDF);
void dvrSetDTDF(dtdf_t *pDTDF, uint8_t i8DTDF[5]);
uint8_t dvrGetCellType(dvr_ifo_handle_t *pIfoHandle, 
					   uint8_t i8PGCN, uint16_t i16CN);
int dvrVroABDel(dvd_file_t *dvd_file, int nStartSector, int nEndSector);
int dvrLPInitVroFile(dvd_file_t *dvd_file, uint32_t nLPTotalMVOBNs,/* uint32_t nAutoMVOBUNs, */uint32_t i32MaxVOBsz,char *pszPath);

bool CheckIsLivePausePath(const char *lpctstrPath);
bool CheckIsNormalRecPath(const char *lpctstrPath);
bool CheckIsHDDPath(const char *lpctstrPath);


#ifdef USE_MMAP

void* dvrIfoGetMMapTableSA(dvr_ifo_handle_t *pIfoHandle, int tyTable, uint16_t i16Index);
int xDVDOpenFile(char *pszFile);
int xDVDCreateFile(char *pszFile);
int xDVDOpenFile_do_flush(char *pszFile);
int xDVDCreateFile_do_flush(char *pszFile);

void xDVDCloseFile(int file);
int xDVDReadBytes(int file, byte *pbyBuf, int nOffset, int nSize);
int xDVDWriteBytes(int file, byte *pbyBuf, int nOffset, int nSize);
int dvrMmapCreate(char *lpctstrDVDPath, byte FileType, byte byExtIFOType);
uint16_t ConvertAddrToIndex(dvr_ifo_handle_t *pIfoHandle, int nType, uint32_t i32Addr);
int dvrResetMmapLookupTable(dvr_ifo_handle_t *pIfoHandle, int nType, uint32_t i32Addr);

#endif

#ifdef __cplusplus
};
#endif
#endif	//__DVR_IFO_ACCESS_H_INCLUDED
