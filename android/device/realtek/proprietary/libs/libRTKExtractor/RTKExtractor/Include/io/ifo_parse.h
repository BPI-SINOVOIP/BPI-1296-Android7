#ifndef IFO_READ_H_INCLUDED
#define IFO_READ_H_INCLUDED


#include "ifo_types.h"
#include "dvd_parse.h"

#ifdef __cplusplus
extern "C" {
#endif


ifo_handle_t *RTOpenIFO(dvd_info_t *, int );
ifo_handle_t *RTParseVMGI(dvd_info_t *);
ifo_handle_t *RTParseVTSI(dvd_info_t *, int);
void RTCloseIFO(ifo_handle_t *);


int RTParseFPPGC(ifo_handle_t *);
int RTParseTTSRPT(ifo_handle_t *);
int RTParsePGCIUT(ifo_handle_t *);
int RTParseCADT(ifo_handle_t *);
int RTParseVOBUADMAP(ifo_handle_t *);
int RTParsePTLMAIT(ifo_handle_t *);
int RTParseVTSATRT(ifo_handle_t *);
int RTParseVTSPTTSRPT(ifo_handle_t *);
int RTParsePGCIT(ifo_handle_t *);
int RTParseVTSTMAPT(ifo_handle_t *);
int RTParseTITLECADT(ifo_handle_t *);
int RTParseTITLEVOBUADMAP(ifo_handle_t *);
int RTParseTXTDTMGI(ifo_handle_t *);


void RTClearupPGC(pgc_t *pgc);
void RTClearupPGCIT(pgcit_t *pgcit);
void RTClearupVOBUADMAP(ifo_handle_t *);
void RTClearupTITLEVOBUADMAP(ifo_handle_t *);
void RTClearupVTSTMAPT(ifo_handle_t *ifofile);
void RTClearupPGCCOMMAND(pgc_command_tbl_t *cmd_tbl);

#ifdef __cplusplus
};
#endif
#endif /* IFO_READ_H_INCLUDED */
