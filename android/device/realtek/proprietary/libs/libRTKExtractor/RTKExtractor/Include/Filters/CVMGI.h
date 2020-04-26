#ifndef __CVMGI_H_
#define __CVMGI_H_

#include <stdio.h>
#ifndef WIN32
#include <string.h>
#include <stdlib.h>
#endif


//For mydvdread
#include <io/ifo_types.h>
#include <io/ifo_parse.h>
#include <io/ifo_write.h>
#include <io/dvd_write.h>

#include <OSAL.h>
#include <Util.h>
#include <CStream.h>
#include <hresult.h>
#include <Filters/CDVDVideoTS.h>

//#include <Filters/vmgi_list.h>

#define IN
#define OUT

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

class CVMGI{

protected:
	ifo_handle_t *m_pVMGI;
	dvd_info_t *m_dvd;
	int m_FreeFlag;        // if (1), means I create this, so I must delete this 
    int m_titlesPerPage;
    int m_titleSeqList[99];
    
public:

	// The VMGI constructor takes :
	// 1. DVD device handle as input since it will use it to do raw data writing.
	// 2. pVMGI pointer. If it is NULL, the VMGI data structure is allocated and
	//   filled with dvdread library and DVDAuthor Filter should put it into the 
	//   shared database. Otherwise, it will be got from shared database and later 
	//   will be shared the same copy by dvd navigator filter even it is modified.
	CVMGI(IN dvd_info_t *dvd, IN OUT ifo_handle_t **pVMGI);

	// Only free the spce that is allocated during construction 
	virtual ~CVMGI();

	// Set the content of VMGI based on the IFO files on the disc.  
	// It should be called whenever a new disk is found. 
	HRESULT Open(IN OUT ifo_handle_t **pVMGI);

	// free the allocated memory
	HRESULT Close();
	HRESULT Close(CDVDVideoTS *pDVDCommon);

	// Update the VIDEO_TS.IFO/VIDEO_TS.BUP based on the structure of ifo_handle_t 
	// pVMGI to optical disc from the start address startLBA	
	HRESULT Realize(IN bool isIFO, IN uint32_t bupStarAddr);

	// If the disc is an empty one, the default value will be put into the VTSI.
	// It is different for DVD-Video and DVD+RW for the create new VTSI operation.
	virtual HRESULT Initialize(IN OUT ifo_handle_t **pVMGI)=0;

	HRESULT ValidateVMGI(); 

	// return the VMGI ifo_handle_t which is fully compatible with dvdread and 
	// can be used by DvdNav with ifoRead_PGCIT, ifoRead_C_ADT... 
	ifo_handle_t * GetVMGI();

	virtual HRESULT GetTotalTitle(OUT int *Tnum)=0;
	
	virtual HRESULT GetTotalNonEmptyTitle(OUT int *Tnum)=0;

	virtual HRESULT CanRecord(OUT bool *value)=0;

	// according to the title num and resolution
	// need to decide which VTS and VTS title should be record
	// Authoring filter will use these two value to init/set VTSI pointer
	virtual HRESULT SetRecordInfo(IN OUT int *Tnum, IN ENUM_VIDEO_HORIZONTAL_RESOLUTION Resolution, 
								  OUT int *VTSnum, OUT int *VTSTTnum)=0;

	// Authoring filter can use this function to get title information
    HRESULT getRecordInfo(IN int Tnum, OUT int *VTSnum, OUT int *VTSTTnum);
    	
//////////////////////////////////////////////////////////////////////////////////
// Other usage functions:
	HRESULT getPageNumber(OUT int *pageNum);
	HRESULT getVTSNumber(OUT int *VTSNum);
    HRESULT IsEmptyTitle(IN int title, OUT bool *isEmpty);

//////////////////////////////////////////////////////////////////////////////////
// API for VMG_PTT_SRPT

	// when REC STOP, can call this function
	virtual HRESULT AddTitle(IN int title, IN int VTSn, IN int VTSTTn)=0;

	// Delete will not handle merge
    // Authoring filter need to call merge by itself
	virtual HRESULT DeleteTitle(IN int title) = 0;

	// after AddTitle, the title's Number of PTT = 0
	// so during the recording, when VTSI complete one PTT, 
	// Must use this function to Set Title's NumberOfPTT information
	// NOTE: authoring filter must handle Full Title's PTT by itself
	uint16_t GetNumberOfPTT(int Tnum);
	HRESULT  SetNumberOfPTT(int Tnum, uint16_t numberOfPTT);


/////////////////////////////////////////////////////////////////////////////////
// API for VMGM_PGCI_UT
// Only has 1 Language Unit by default, set at initial time

	HRESULT CreateNewPGCI(IN pgcit_t *pgcit, IN int PGCNumber, IN pgci_srp_t *pPGCI_Srp);
	HRESULT ReplacePGCI(IN pgcit_t *pgcit, IN int PGCNumber, IN pgci_srp_t *pPGCI_Srp);
	bool IsEmptyPGC(IN pgcit_t *pgcit, IN int PGCNum);
	HRESULT DeletePGCI(IN pgcit_t *pgcit, IN int PGCNumber);



/////////////////////////////////////////////////////////////////////////////////
// API for VMG_VTS_ATRT
	HRESULT AddOneVTS(IN int num, IN vts_attributes_t *pVTSattr);
	HRESULT DeleteVTS(IN int num);
    

	// VMG_PTL_MAIT
	// VMG_TXTDT_MG
	  // empty currently by default

	
/////////////////////////////////////////////////////////////////////////////////
// API for VMGM_C_ADT 
    // if find cell_addr, then replace it, otherwise add the cell
	HRESULT AddOneCell(IN c_adt_t *c_adt, IN cell_adr_t *cell_adr);
	HRESULT DeleteOneCell(IN c_adt_t *c_adt);
	// after add or delete cell, we should count this PGC's total playback time
	HRESULT SyncPGC_PB_TM(IN pgcit_t *pgcit, IN int PGCNumber);


/////////////////////////////////////////////////////////////////////////////////
// API for VMGM_VOBU_ADMAP
	// if find addr bigger than vobuStartAddr, then insert this VOBU
	HRESULT AddOneVOBU(IN vobu_admap_t *vobu_admap, IN int vobuStartAddr);
	HRESULT DeleteOneVOBU(IN vobu_admap_t *vobu_admap);


////////////////////////////////////////////////////////////////////////////////

// API for Video, Audio, SubPicture Attribute  
// when VTSn = 0,  it means to set/get VMGM
	video_attr_t &VideoAttribute(IN int VTSn, IN int IsMenu);

	uint8_t GetNumOfAudio(IN int VTSn, IN int IsMenu);
	HRESULT SetNumOfAudio(IN int VTSn, IN int IsMenu, IN uint8_t numOfAudio);
	audio_attr_t &AudioAttribute(IN int VTSn, IN int ISMenu, IN int AudioNumber);
	
	uint8_t GetNumOfSubPicture(IN int VTSn, IN int IsMenu);
	HRESULT SetNumOfSubPicture(IN int VTSn, IN int IsMenu, IN uint8_t numOfSubPicture);
	subp_attr_t &SubPictureAttribute(IN int VTSn, IN int IsMenu, IN int SubNumber);


/////////////////////////////////////////////////////////////////////////////////
// API for PGCI  

	// PGC_PGMAP update
	HRESULT AddOnePG(IN pgcit_t *pgcit, IN int PGCNumber, IN int entryCellNumber);
	HRESULT DeleteOnePG(IN pgcit_t *pgcit, IN int PGCNumber);

	// PGC_C_PBIT and C_POSIT update
	// when CellNumber == -1, means to append the cell in PGC
	HRESULT AddOneCellInPGC(IN pgcit_t *pgcit, IN int PGCNumber, IN int cellIndex,
							IN cell_playback_t *pCellPlayback, 
							IN cell_position_t *pCellPosition,
							IN int CellCmdNumber, IN unsigned char *pCellCmd);

	HRESULT DeleteOneCellInPGC(IN pgcit_t *pgcit, IN int PGCNumber);


	//Set/Get PG Number
	uint8_t  GetPGC_PG_NUM(IN pgcit_t *pgcit, IN int PGCNumber);
	HRESULT  SetPGC_PG_NUM(IN pgcit_t *pgcit, IN int PGCNumber, IN uint8_t PGC_PG_NUM);
	//Set/Get Cell Number
	uint8_t  GetPGC_CELL_NUM(IN pgcit_t *pgcit, IN int PGCNumber);
	HRESULT  SetPGC_CELL_NUM(IN pgcit_t *pgcit, IN int PGCNumber, IN uint8_t PGC_CELL_NUM);
	//Set/Get PGC_CNT
	dvd_time_t &PGC_PB_TM(IN pgcit_t *pgcit, IN int PGCNumber);
	// Set/get PGC_UOP_CTL
	user_ops_t &PGC_UOP_CTL(IN pgcit_t *pgcit, IN int PGCNumber);
	// Set/get PGC_AST_CTLT
	uint16_t &PGC_AST_CTLT(IN pgcit_t *pgcit, IN int PGCNumber, IN int audioNumber);
	// Set/Get PGC_SPST_CTLT
	uint32_t &PGC_SPST_CTLT(IN pgcit_t *pgcit, IN int PGCNumber, IN int subNumber);

	HRESULT SetPGC_NV_CTL(IN pgcit_t *pgcit, IN int PGCNumber, 
		                  IN int nextPGCN, IN int prevPGCN, 
						  IN int goupPGCN, IN int PGPBMode, IN int stillTime);

	HRESULT SetPGC_SP_PLT(IN pgcit_t *pgcit, IN int PGCNumber, IN uint32_t *palette);

	// PGC_CMDT update
	// if numOfPreCmd =0, means to delete the cmd
	HRESULT SetPGC_PRE_CMD(IN pgcit_t *pgcit, IN int PGCNumber, 
		                   IN int numOfPreCmd, IN unsigned char *preCmd);
	HRESULT SetPGC_POST_CMD(IN pgcit_t *pgcit, IN int PGCNumber, 
		                    IN int numOfPostCmd, IN unsigned char *postCmd);

/////////////////////////////////////////////////////////////////////////////
// API for first-play PGC

	//Set/Get PG Number
	uint8_t  GetFP_PGC_PG_NUM(IN pgc_t *pgct);
	HRESULT  SetFP_PGC_PG_NUM(IN pgc_t *pgct, IN uint8_t PGC_PG_NUM);
	//Set/Get Cell Number
	uint8_t  GetFP_PGC_CELL_NUM(IN pgc_t *pgct);
	HRESULT  SetFP_PGC_CELL_NUM(IN pgc_t *pgct, IN uint8_t PGC_CELL_NUM);
	//Set/Get PGC_CNT
	dvd_time_t &FP_PGC_PB_TM(IN pgc_t *pgct);
	// Set/get PGC_UOP_CTL
	user_ops_t &FP_PGC_UOP_CTL(IN pgc_t *pgct);
	// Set/get PGC_AST_CTLT
	uint16_t &FP_PGC_AST_CTLT(IN pgc_t *pgct, IN int audioNumber);
	// Set/Get PGC_SPST_CTLT
	uint32_t &FP_PGC_SPST_CTLT(IN pgc_t *pgct, IN int subNumber);

	HRESULT SetFP_PGC_NV_CTL(IN pgc_t *pgct, 
		                  IN int nextPGCN, IN int prevPGCN, 
						  IN int goupPGCN, IN int PGPBMode, IN int stillTime);

	HRESULT SetFP_PGC_SP_PLT(IN pgc_t *pgct, IN uint32_t *palette);

	// PGC_CMDT update
	// if numOfPreCmd =0, means to delete the cmd
	HRESULT SetFP_PGC_PRE_CMD(IN pgc_t *pgct, 
		                   IN int numOfPreCmd, IN unsigned char *preCmd);
	HRESULT SetFP_PGC_POST_CMD(IN pgc_t *pgct,
		                    IN int numOfPostCmd, IN unsigned char *postCmd);

	HRESULT linkTitle(IN int title, IN int nextTitle);
		                    
	virtual HRESULT parseTitleSeqTable() = 0;                
};



class CVMGI_PlusRW : public CVMGI
{
    
public:
	CVMGI_PlusRW(IN dvd_info_t *dvd, IN OUT ifo_handle_t **pVMGI);
	~CVMGI_PlusRW(void);

	virtual HRESULT Initialize(IN OUT ifo_handle_t **pVMGI);
	virtual HRESULT GetTotalTitle(OUT int *Tnum);
	virtual HRESULT GetTotalNonEmptyTitle(OUT int *Tnum);

	virtual HRESULT CanRecord(OUT bool *value);
	virtual HRESULT SetRecordInfo(IN OUT int *Tnum, IN ENUM_VIDEO_HORIZONTAL_RESOLUTION Resolution, 
								  OUT int *VTSnum, OUT int *VTSTTnum);
	virtual HRESULT DeleteTitle(IN int title);

	// called while start recording
	// replace title information
	// automatically add free title and handle related Full Title
	// if the disc is full, authoring must destroy free title by calling DestroyFreeTitle()
	virtual HRESULT AddTitle(IN int title, IN int VTSn, IN int VTSTTn);
	
	// after AddTitle(), maybe this recording not use so many space
	// so authoring filter call CreateNewTitle() to create a new deleted title
	// prevent waste disc space
	HRESULT CreateNewTitle(IN int title, IN int VTSn, IN int VTSTTn);

	// after Delete Title, 
	// authoring filter must call MergeTitle()
	// Title1 and Title2 must be Play List title number.
	// It will automatically merge related two Full titles.
	// authoring filter dont need to call it twice 
	HRESULT MergeTitle(IN int title1, IN int title2);

	// Merge two titles which have contents
	HRESULT GeneralMergeTitle(IN int title);
	
	// after AddTitle, If authoring filter detect that the disc is full
	// it must call DestroyFreeTitle();
	HRESULT DestroyFreeTitle();
	
	// This function is used to split one title to two titles which have content
	// split playlist title and full title at the same time
	HRESULT SplitTitle(IN int title, IN int higher_chpnum, IN bool isCHPSplit);
	
	HRESULT SetTitlesPerPage(IN int num);

	HRESULT parseTitleSeqTable();

        HRESULT CheckDeleteVTS(OUT int *deleteVTS, OUT int *insertVTSTTn);

        HRESULT DestroyTitle(IN int title);
private:
	// according to Resolution to decide the VTS number
	bool ChooseVTS(IN ENUM_VIDEO_HORIZONTAL_RESOLUTION Resolution, OUT int *VTSn);

	// according to Title number and VTS number
	// decide the VTSTT number
	bool ChooseVTSTT(IN OUT int *Tnum, IN int VTSn, OUT int *VTSTTn);

	// After AddTitle and MergeTitle, it will call this function to correct all
	// VTS number and VTSTT number,
	// including VMG_PTT_SRPT and VMG_VTS_ATRT table
	HRESULT CorrectDiscTitle();

	// According to Total title number
	// update 3 PGC's pre and post cmds
	HRESULT UpdatePGCcmds();
};



class CVMGI_Video : public CVMGI
{
public:
	CVMGI_Video(IN dvd_info_t *dvd, IN OUT ifo_handle_t **pVMGI);
	~CVMGI_Video(void);

	virtual HRESULT Initialize(IN OUT ifo_handle_t **pVMGI);
	virtual HRESULT GetTotalTitle(OUT int *Tnum);
	virtual HRESULT GetTotalNonEmptyTitle(OUT int *Tnum);
	virtual HRESULT CanRecord(OUT bool *value);
	virtual HRESULT SetRecordInfo(IN OUT int *Tnum, IN ENUM_VIDEO_HORIZONTAL_RESOLUTION Resolution,
								  OUT int *VTSnum, OUT int *VTSTTnum);
	virtual HRESULT AddTitle(IN int title, IN int VTSn, IN int VTSTTn);

	HRESULT DeleteTitle(IN int title);
	HRESULT parseTitleSeqTable();

private:	
	HRESULT UpdatePGCcmds();
};

#endif // __CVMGI_H__
