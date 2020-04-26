#ifndef IFO_VRMI_H_INCLUDED
#define IFO_VRMI_H_INCLUDED


#include "ifo_types_VRMI.h"
#include "dvd_parse.h"

#ifdef __cplusplus
extern "C" {
#endif

#define VRMI_IFO_START_SECTOR  1536
#define VRMI_BUP_START_SECTOR  2293664

VRMI_st *ifoOpenVRMI(dvd_info_t *m_dvd, dvd_file_t **pfile, bool isPlusRW);
int ifoReadVRMI(dvd_file_t *file, VRMI_st *pVRMI, bool isPlusRW);
void ifoCloseVRMI(VRMI_st *pVRMI, bool isPlusRW);
VRMI_st * ifoCreateVRMI(dvd_info_t *dvd, dvd_file_t **file, bool isPlusRW);
int ifoWriteVRMI(dvd_file_t *file, VRMI_st *pVRMI, bool isPlusRW);
int reopen_vrmifd(dvd_file_t *file, int isBup, unsigned int startAddr);
#ifdef __cplusplus
};
#endif


#endif  // IFO_VRMI_H_INCLUDED

