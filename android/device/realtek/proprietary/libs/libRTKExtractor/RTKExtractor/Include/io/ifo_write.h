#ifndef IFO_WRITE_H_INCLUDED
#define IFO_WRITE_H_INCLUDED

#include "ifo_types.h"


#define MAX_VTS_TTU_N		99
#define MAX_VTS_PTT_SRPT_N	999
#define MAX_VTS_CELL_N		254
#define MAX_PGC_CMD_N		128
#define MAX_PGC_PG_N            99
//For dual layer, will up to 16 hours recording, assume 1 VOBU will will be 24/25
//Second for SEP and SLP(save memory prupose)
#define MAX_VTS_VOBU_N          75000
//#define MAX_VTS_VOBU_N		40959

//#define VTSI_START_SECTOR   11264
#define MAX_TT_SRP          99  // Total Disc titles 
#define MAX_VMG_CELL_N      170
#define MAX_VMG_VOBU_N      511

#define VMGI_IFO_START_SECTOR     4096
#define VMGI_VOB_START_SECTOR     4352
#define VMGI_BUP_START_SECTOR    15104
#define VTSI_IFO_START_SECTOR    15360
#define IFO_GAP                    128   // the distance between all VTSI IFO and BUP files
#define VOB_START_SECTOR         16384

#ifdef __cplusplus
extern "C" {
#endif

/**
 * The ifo_handle_t should not be released here since it is not created 
 * inside dvdWrite.
 * Inside the ifoWrite, the original file will be deleted and a new ifo is 
 * re-opened with the new location(UDF File system will provide necessary API).
 * A new ifo->file->title_devs[0].fd is assigned. It should be protected
 * in the time-shift application.
 * It will be always fileread/write in this case even for the new files.
 */

/**
 * okey = ifoCreate(ifo, title);
 *
 * For an empty disc, an empty dummy ifo file should be created with only empty
 * PGC at the desired location on the disc. All the necessary fields in the ifo 
 * will be correctly iniriated.
 * If title 0 is given, the video manager IFO file is create.
 * Returns valid ifo_handle_t pointer if the data was successfully read and NULL on error.
 */
ifo_handle_t * RTCreateIFO(dvd_info_t *dvd, int );
ifo_handle_t * RTCreateVMGI(dvd_info_t *dvd);


/**
 * okey = ifoWrite(ifo, title);
 *
 * Creates an IFO based on the data (ifo_handle_t *)ifo for the IFO file 
 * corresponding to the given title.
 * If title 0 is given, the video manager IFO file is create.
 * Returns 1 if the data was successfully read and 0 on error.

 */
int RT_ifoWrite(ifo_handle_t *, int );

/**
 * okey = ifoCreateVMGI(vmgi);
 *
 * Creates an IFO based on the vmgi data.  
 * Returns 1 if the data was successfully read and 0 on error.
 */

int RT_ifoWriteVMGI(ifo_handle_t *);

/**
 * okey = ifoCreateVTSI(ifo, title);
 *
 * Creates an VTS IFO based on the vtsi data.  
 * Returns 1 if the data was successfully read and 0 on error.
 */
int RT_ifoWriteVTSI(dvd_info_t *, int);

int RT_ifoValidateVMGI(ifo_handle_t *ifofile);
int RT_ifoValidateVTSI(ifo_handle_t *ifofile, int vtsN, uint32_t VOBSize);

int RTCreateFirstPlayPGC(ifo_handle_t *ifofile);
int RTCreateVMGPTTSRPT(ifo_handle_t *ifofile, int InitTitleNumber);
int RTCreateVMGVTSATRT(ifo_handle_t *ifofile, int max_vts, int InitVTSNumber);

int RTCreateVTSPTTSRPT(ifo_handle_t *ifofile, int InitTitleNumber);
int RTCreatePGCIT(ifo_handle_t *ifofile, int VTS_number, int initPGCINumber);
int RTCreatePGCIUT(ifo_handle_t *ifofile);
int RTCreatePGCI(pgcit_t *pgcit, int PGCNumber, pgci_srp_t *pPGCI_Srp);
int RTCreatePGC(pgc_t **pgc);
int RTCreateVTSTMAPT(ifo_handle_t *ifofile, int InitTitleNumber);
int RTCreateCADT(ifo_handle_t *ifofile);
int RTCreateTITLECADT(ifo_handle_t *ifofile);
int RTCreateTITLEVOBUADMAP(ifo_handle_t *ifofile);
int RTCreateVOBUADMAP(ifo_handle_t *ifofile);
int RT_ifoDeleteVOBUs(vobu_admap_t *vobu_admap, int startIndex, int endIndex);
int RT_ifoAddOneCellInPGC(pgc_t *pgc, int cellIndex, cell_playback_t *pCellPlayback, cell_position_t *pCellPosition,
						int CellCmdNumber, unsigned char *pCellCmd);
int RT_ifoAddOnePG(pgc_t *pgc, int PGNumber, int entryCellNumber);

int reopen_fd(ifo_handle_t *ifofile, int VTS_number, int isBup, unsigned int startAddr);
#ifdef __cplusplus
}
#endif
#endif

