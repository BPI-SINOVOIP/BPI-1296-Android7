#ifndef __DVD_VIDEO_IFO_HELPER_H__
#define __DVD_VIDEO_IFO_HELPER_H__

#include "common.h"

//------------------------------------------------------------------------------
// Queries from VMG

// get ttN that's corresponding to the vtsN/vts_ttN pair
int getTTN(vmg_t* vmg, int vtsN, int vts_ttN);

// get title information (includes vtsN, vts_ttN, ... etc) by ttN
title_info_t* getTitleInfo(vmg_t* vmg, int ttN);

// get parental ID field from PTL_MAIT, vtsN is 0 (for VMG) or 1~N (for VTS)
int getParentalIDField(vmg_t* vmg, uint16_t country, uint16_t level, int vtsN);

//------------------------------------------------------------------------------
// Queries from VTS

// get pttN that's corresponding to vts_ttN/pgcN/pgN
int getPTTN(vts_t* vts, int vts_ttN, int pgcN, int pgN);

// get PTT information (includes pgcN, pgN) by vts_ttN/pttN
ptt_info_t* getPTTInfo(vts_t* vts, int vts_ttN, int pttN);

// search time-map to return RLBN of target vobu, based on target elapsed time in seconds
uint32_t searchTimeMap(vts_t* vts, int pgcN, int seconds);

//------------------------------------------------------------------------------
// Queries from PGCIT

// find entry pgcN from pgc table
int getEntryPGCN(pgcit_t* pgcit, int id, int n);

// find entry pgcN for menu identified by menuID
inline int getMenuEntryPGCN(pgcit_t* pgcit, int menuID) { return getEntryPGCN(pgcit, menuID,  3); }

// find entry pgcN for title identified by vts_ttn
inline int getTitleEntryPGCN(pgcit_t* pgcit, int vts_ttn) { return getEntryPGCN(pgcit, vts_ttn, 6); }

// get pgc from pgc table
pgc_t* getPGC(pgcit_t* pgcit, int pgcN);

// get first pgcN in the current parental block (identified by pgcN)
int getParentalBlockFirstPGCN(pgcit_t* pgcit, int pgcN);

// select one pgc from the current parental block (identified by pgcN) by matching its parental ID against ptl_id_fld
// returns pgc/pgcN on success
int selectPGCByPTL(pgcit_t* pgcit, int pgcN, int ptl_id_fld, pgc_t** pgc);

//------------------------------------------------------------------------------
// Queries from PGC

// get first cellN of PG
int getPGFirstCellN(pgc_t* pgc, int pgN);

// get last cellN of PG
int getPGLastCellN(pgc_t* pgc, int pgN);

// get the PGN that a cell belongs to
int getCellPGN(pgc_t* pgc, int cellN);

// get first cellN in the current angle block (identified by cellN)
int getAngleBlockFirstCellN(pgc_t* pgc, int cellN, int PGFirstCellN);

// get last cellN in the current angle block (identified by cellN)
int getAngleBlockLastCellN(pgc_t* pgc, int cellN, int PGLastCellN);

// select one cell from the current angle block (identified by cellN) according to angleN
// firstCellN/lastCellN specified the search range
int selectCellByAngle(pgc_t* pgc, int cellN, int angleN, int firstCellN, int lastCellN);

// counting playback frames from start cellN to end cellN
int countFramesOfCells(pgc_t* pgc, int startCellN, int endCellN, int framesPerSecond);

// get the cellN that a frame belongs to (always return cell of angle 1)
int getFrameCellN(pgc_t* pgc, int frameN, int framesPerSecond);

//------------------------------------------------------------------------------
// Other Queries

// locate VOBU address within a cell (relative to a reference VOBU) by searching C_ADT/VOBU_ADMAP
uint32_t findVOBUAddrInCell_BySearchingADMAP(c_adt_t* c_adt, vobu_admap_t* vobu_admap, int VOB_IDN, int C_IDN,
                                             uint32_t referenceVOBUAddr, int VOBUNumberOffset);

#endif //__DVD_VIDEO_IFO_HELPER_H__
