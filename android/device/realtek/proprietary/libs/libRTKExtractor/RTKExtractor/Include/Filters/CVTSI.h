#ifndef CVTSI_H_INCLUDED
#define CVTSI_H_INCLUDED

#include <stdio.h>

#ifndef WIN32
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#endif

#include <hresult.h>
#include <OSAL.h>
#include <Util.h>
#include <Filters/CDVDVideoTS.h>

#include <io/ifo_types.h>
#include <io/ifo_parse.h>
#include <io/ifo_write.h>
#include <io/dvd_write.h>

#define IN
#define OUT

typedef struct {
  int lower_pbtm_frames;
  int higher_vrchpnum;
  int isCHPSplit;
}VTSI_SPIT_INFO;

typedef struct {
  int cell_id;
  int vobuSize;
  int cellStartAddr;
}VTSI_ADDPTT_INFO;

typedef struct {
  char cellID;
  char cellStart;
  char isHide;
  char dummy;
  unsigned int vobuAddr;
}VTSI_MK_VRINFO;

typedef struct {
  int totalCHP;
  VTSI_MK_VRINFO vrinfo[MAX_VTS_PTT_SRPT_N];
}VTSI_MK;

typedef struct {
  //New Cell start LBA
  unsigned int startAddr;
  unsigned int lastvobu_startaddr;
  //New Cell end LBA
  unsigned int endAddr;
  //New Cell Duration, in frame
  unsigned int duration;
  //New cell ID
  unsigned int cellId;  
}SLIPTCELLDETAIL;

typedef struct {
  unsigned char cellID;
  unsigned char devide;
  unsigned char dummy1;
  unsigned char dummy2;
  SLIPTCELLDETAIL *splitcell;
}SPLITCELL;


class CVTSI{

public:

	// The VTSI constructor takes :
	// 1. DVD device handle as input since it will use it to do raw data writing.
	// 2. pVTSI pointer. If it is NULL, the VTSI data structure is allocated and
	//   filled with dvdread library and DVDAuthor Filter should put it into the 
	//   shared database. Otherwise, it will be got from shared database and later 
	//   will be shared the same copy by dvd navigator filter even it is modified.
	// 3. VTS number which indicate the VTS set number
	// For +VR, max 3 VTS can be existed at the same time. The ifo structure is created
	// based on the worst case except VOBU table since it is shared by the three VTSs.
	// For DVD-Video, not necessary.
	
    CVTSI(IN dvd_info_t *dvd, IN OUT ifo_handle_t **pVTSI, IN int VTS_number);

	// Only free the spce that is allocated during construction 
    virtual ~CVTSI(void);

	// return the VTSI ifo_handle_t which is fully compatible with dvdread and 
	// can be used by DvdNav with ifoRead_PGCIT, ifoRead_C_ADT... 
	ifo_handle_t *GetVTSI();

	//This info is necessary to decide the VTS_EA in VTSI_MAT
	HRESULT SetVOBTotalSize(unsigned int totalSectorSize);

	//Get the total number of PGCs in the current VTS.
	virtual HRESULT GetTotalTitle(OUT int *Tnum) = 0;

	//Get the recording infomation
	HRESULT GetRecordInfo(IN int Tnum,
							OUT uint32_t *begin_lbn,
							OUT uint32_t *end_lbn);

	//Set the recording information
	virtual HRESULT SetRecordInfo(IN OUT int *Tnum) = 0;

	// All the contents of pVTSI is checked and is corrected if necessary(mostly for xx_EA).
	// Also some new structure pointers are inserted to the right place, if not done
	// during the recoring since playback may take the semiphor.
	// It is should be called before UpdateVTSIFile().	*/
	HRESULT ValidateVTSI(IN uint32_t VOBSize);

	// Update the VTS_0x_0.IFO/VTS_0x_0.BUP based on the structure of ifo_handle_t 
	// pVTSI to optical disc from the start address startLBA	
	HRESULT Realize(bool isIFO, IN OUT uint32_t *bupStartLBA);

	// Set the content of VTSI based on the IFO files on the disc.  
	// It should be called whenever a new disk is found. 
	
	HRESULT Open(IN OUT ifo_handle_t **pVTSI, IN int VTS_number);

	virtual HRESULT Close() = 0;
	virtual HRESULT Close(CDVDVideoTS *pDVDCommon) = 0;

	// If the disc is an empty one, the default value will be put into the VTSI.
	// It is different for DVD-Video and DVD+RW for the create new VTSI operation.
	virtual HRESULT Initialize(IN OUT ifo_handle_t **pVTSI, IN int VTS_number) = 0;

	// Delete title with the desired title number in the current VTS. Since there
	// are multiple titles for +VR but one title for DVD-Video, different 
	// implementation is needed. 
	// Basic operation should include change old PGC(s) to the empty PGC, clear PTT
	// search table, C_ADT and VOBU_ADT
	virtual HRESULT Delete(IN int title) = 0;

	HRESULT ReOpenFileHandle();
	HRESULT ReSetVTSI(IN ifo_handle_t *pVTSI, IN CDVDVideoTS *pDVDCommon);

	// For VTSI_MAT update
	// the define of video_attr_t in ifo_type.h seems incorrect 
	// Set/Get Video attribute. return internal variable directly
	video_attr_t &VideoAttribute(IN int isMenu);

	// By default is one 
	// Set/Get Audio Number
	uint8_t GetNumOfAudio(IN int isMenu);
	HRESULT SetNumOfAudio(IN int isMenu, uint8_t numOfAudio);

	// Set/Get Audio stream N attribute
	audio_attr_t &AudioAttribute(IN int isMenu, IN int AudioNumber);

	// By default is zero
	// Set/Get Sub Picture number
	uint8_t GetNumOfSubPicture(IN int isMenu);
	HRESULT SetNumOfSubPicture(IN int isMenu, uint8_t numOfSubPicture);

	// Set/Get sub picture attribute
	subp_attr_t &SubPictureAttribute(IN int isMenu, IN int SubNumber);

	// For VTS_PTT_SRPT update
	HRESULT CreateNewPTT(IN int titleNumber);

	// If the PTT is the first one of a new title, nr_of_srpts will increase
	// by one and ttu_offset[nr_of_srpts] will be set the correct value.
	// For VR, it is a insertion, should be protected by the Semaphore
	// nr_of_ptts in title will increase by one. PGCN and PGC will be set to the 
	// correct entry of title->ptt[nr_of_ptts] 
	// This one should be called once a new chapter is created.
	// For +VR, one PM is one PTT. 
	HRESULT AddOnePTT(IN int titleNumber, IN int PGCN, int PGN);
	HRESULT DeleteOnePTT(IN int titleNumber);

	// Delete the whole title's PTT search pointer
	// It should be done with Semaphore protection
	HRESULT DeletePTT(IN int titleNumber);

	//Clear the PTT srpt with 1 PG only
	HRESULT ClearPTT(IN int titleNumber);

	HRESULT MergePTT(IN int titleNumber);

	//Return the total free chapter number
	HRESULT GetTotalNumberOfFreePTT(int titleNumber);

	// For VTS_PGCIT update
	// PGC_GI update
	// Only support one to one mapping between PGCI_SRP and PGCI currently. 
	// The content of *pPGCI_Srp will be copied into the *pVTSI.
	// Leaving the pgc pointer NULL which will be assigned internally by the VTSI.
	// All the PGCs with PGC number equal or greater than PGCNumber will be moved 
	// forward in the pgci_srp_t->pgc. It should be protected by the Semaphore.
	
	HRESULT CreateNewPGCI(IN pgcit_t *pgcit, IN int PGCNumber, IN pgci_srp_t *pPGCI_Srp);

	// If PGC N is empty PGC, it will be replace with the desired PGC.
	HRESULT ReplacePGCI(IN pgcit_t *pgcit, IN int PGCNumber, IN pgci_srp_t *pPGCI_Srp);
	HRESULT MergePGCI(IN pgcit_t *pgcit, IN int PGCNumber);

	// Test if PGC N is empty PGC or not. 
	bool IsEmptyPGC(IN pgcit_t *pgcit, IN int PGCNum);

	//Set/Get PG Number
	uint8_t  GetPGC_PG_NUM(IN pgcit_t *pgcit, IN int PGCNumber);
	HRESULT  SetPGC_PG_NUM(IN pgcit_t *pgcit, IN int PGCNumber, uint8_t PGC_PG_Num);

	//Set/Get Cell Number
	uint8_t  GetPGC_CELL_NUM(IN pgcit_t *pgcit, IN int PGCNumber);
	HRESULT  SetPGC_CELL_NUM(IN pgcit_t *pgcit, IN int PGCNumbe, uint8_t PGC_CELL_NUM);

	//Set/Get PGC_CNT
	dvd_time_t &PGC_PB_TM(IN pgcit_t *pgcit, IN int PGCNumber);

	// Set/get PGC_UOP_CTL
	user_ops_t &PGC_UOP_CTL(IN pgcit_t *pgcit, IN int PGCNumber);

	// Set/get PGC_AST_CTLT
	uint16_t &PGC_AST_CTLT(IN pgcit_t *pgcit, IN int PGCNumber, IN int audioNumber);

	// Set/Get PGC_SPST_CTLT
	uint32_t &PGC_SPST_CTLT(IN pgcit_t *pgcit, IN int PGCNumber, IN int subNumber);

	HRESULT SetPGC_NV_CTL(IN pgcit_t *pgcit, IN int PGCNumber, IN int nextPGCN, IN int prevPGCN, 
							IN int goupPGCN, IN int PGPBMode, IN int stillTime);

	HRESULT SetPGC_SP_PLT(IN pgcit_t *pgcit, IN int PGCNumber, IN uint32_t *palette);

	// PGC_CMDT update
	HRESULT SetPGC_PRE_CMD(IN pgcit_t *pgcit, IN int PGCNumber, IN int numOfPreCmd, IN unsigned char *preCmd);

	HRESULT SetPGC_POST_CMD(IN pgcit_t *pgcit, IN int PGCNumber, IN int numOfPostCmd, IN unsigned char *postCmd);

	// Cell command is updated during AddOneCell

	// PGC_PGMAP update
	HRESULT AddOnePG(IN pgcit_t *pgcit, IN int PGCNumber, IN int entryCellNumber);
	HRESULT InsertOnePG(IN pgcit_t *pgcit, IN int PGCNumber, IN int PGNumber, IN int entryCellNumber, OUT VTSI_ADDPTT_INFO *pInfo);
	HRESULT DeleteOnePG(IN pgcit_t *pgcit, IN int PGCNumber, IN int PGNumber);

	// PGC_C_PBIT and C_POSIT update
	HRESULT AddOneCellInPGC(IN pgcit_t *pgcit, IN int PGCNumber, IN int cellIndex, IN cell_playback_t *pCellPlayback, IN cell_position_t *pCellPosition,
						IN int CellCmdNumber, IN unsigned char *pCellCmd);
	HRESULT DeleteOneCellInPGC(IN pgcit_t *pgcit, IN int PGCNumber);

	// VTSM_PGCI_UT
	// Can be set by the above PGCI ApIs with the correct pgcit pointer

	// VTS_TMAPPT
	// Empty TMAP currently by default
	HRESULT DeleteTMAP(IN int title);
	HRESULT CreateNewTMAP(IN int title);

	// VTSM_C_ADT
	// Not exist by default
	//If any, can be set with AddOneCellInVTS with the correct c_adt pointer

	// VTSM_VOBU_ADMAP
	// Not exist by default
	//If any, can be set with AddOneVOBU with the correct vobu_admap pointer

	// For VTS_C_ADT/VTSM_C_ADT update
	HRESULT AddOneCell(IN c_adt_t *c_adt, IN cell_adr_t *cell_adr);
	HRESULT ClearCell(IN c_adt_t *c_adt, IN int vobId, IN int cellId);

	// For VTS_VOBU_ADMAPI
	virtual HRESULT AddOneVOBU(IN vobu_admap_t *vobu_admap, IN unsigned int vobuStartAddr) = 0;

	// Cut the VOBU info[startIndex, endIndex] from the VOBU_ADMAPI
	// Used in the DeleteTilte()
	HRESULT DeleteVOBUs(IN vobu_admap_t *vobu_admap, IN int startIndex, IN int endIndex);

    int GetTotalNonEmptyVOBUs(IN vobu_admap_t *vobu_admap);
    // Used by AddVRChapter()
	int GetVOBUSize(IN unsigned int vobuStartAddr, IN unsigned int cellEndLBA);

	HRESULT SplitCell(IN int PGCNumber, IN int oldCellNumber, IN int newCellNumber, IN int oldCellELTM, 
                          IN int VOBUStartLBA, IN int prevVOBUStartAddr);
	HRESULT SplitTitle(IN int PGCNumber, IN int PGNumber, IN int CellNumber, IN int freeCelID,
			   IN int VOBUStartLBA, IN int prevVOBUStartAddr, IN int newCellELTM, OUT VTSI_SPIT_INFO *pSplitInfo);

	HRESULT GetCellLBAs(IN int PGCNumber, IN int CellNumber, OUT unsigned int *startLBA, OUT unsigned int *endLBA);
	int GetGlobalCellNumber(IN int PGCNumber, IN int CellNumberInPGC);
	int GetPGCCellNumber(IN int PGCNumber, IN int CellNumberGlobal);

	void frame2pbtime(IN int recordDuration, IN int frame_rate, OUT dvd_time_t *pPlayback_time);

    unsigned int* locateVOBU(unsigned int vobuAddr);
        
protected:
	ifo_handle_t *m_pVTSI;
	dvd_info_t *m_dvd;
	int m_FreeFlag;
	int m_dev;
	int m_vtsN;
	unsigned int m_VOBSize;
};
							
class CVTSI_PlusRW : public CVTSI
{
public:

	// The VTSI constructor takes :
	//1. The memory allocation is based on the worst case.
	//2. VOBU table is shared by three VTS, i.e., whenever VTS_number is not one,
	//   no VOBU table is created.
	
    CVTSI_PlusRW(IN dvd_info_t *dvd, IN OUT ifo_handle_t **pVTSI, IN int VTS_number);

	// Only free the spce that is allocated during construction 
    ~CVTSI_PlusRW(void);

	HRESULT GetTotalTitle(OUT int *Tnum);

	HRESULT SetRecordInfo(IN OUT int *Tnum);

	HRESULT Initialize(IN OUT ifo_handle_t **pVTSI, IN int VTS_number);

	HRESULT Close();
	HRESULT Close(CDVDVideoTS *pDVDCommon);

	HRESULT Delete(IN int title);

	HRESULT CreateNewTitle(IN int title);
        HRESULT CreateNewEmptyTitle(IN int title);

	HRESULT DestoryTitle(IN int title);

	//The title N and Title N+1 will be merged.
	HRESULT MergeTitle(IN int Title);

	// To set the VOBU table handler for VTS2 and VTS3 in order to create the correct ifo files 
	HRESULT SetVOBUTable(IN vobu_admap_t   *vts_vobu_admap);

	HRESULT GetCellIDStatus(IN c_adt_t *c_adt, IN OUT char *cellId);

	//Special Operation for VOBU admap by +RW
	HRESULT PrepareVOBUAdmap(IN vobu_admap_t *vobu_admap, IN unsigned int vobuStartAddr);
	HRESULT AddOneVOBU(IN vobu_admap_t *vobu_admap, IN unsigned int vobuStartAddr);
	HRESULT FormatVOBUAdmap(IN vobu_admap_t *vobu_admap);
	HRESULT ResetVOBUAdmapOwnership(int flag);
	HRESULT CanEditTitle(IN int PGCNumber);
	HRESULT SplitTitle(IN int PGCNumber, IN int PGNumber, IN int CellNumber, IN int freeCelID,
			   IN int VOBUStartLBA, IN int prevVOBUStartAddr, IN int newCellELTM, OUT VTSI_SPIT_INFO *pSplitInfo);
	HRESULT GeneralMergeTitle(IN int title);
        HRESULT MakeCompatible(IN int title, IN VTSI_MK *pMK, int totalFreeCellNumber, char *pFreeCellId,
                               OUT int* cellNum, OUT SPLITCELL **pResult);
        HRESULT ClearSplitCell( IN int cellNum, IN SPLITCELL *pResult);
private:

	int m_newTitleNumber;
	//Flag to share the VOBU table. 
	//Has problem for Playback. Need to fix later...
	int m_FreeVOBUAdmap;
	int m_vobu_admap_index;
	int m_vobu_admap_reserved_index;

};


class CVTSI_DVDVideo : public CVTSI
{
public:

	CVTSI_DVDVideo(IN dvd_info_t *dvd, IN OUT ifo_handle_t **pVTSI, IN int VTS_number);    

	// Only free the spce that is allocated during construction 
        ~CVTSI_DVDVideo(void);

	HRESULT GetTotalTitle(OUT int *Tnum);

	HRESULT SetRecordInfo(IN OUT int *Tnum);

	HRESULT Initialize(IN OUT ifo_handle_t **pVTSI, IN int VTS_number);

	HRESULT Close();
	HRESULT Close(CDVDVideoTS *pDVDCommon);

	HRESULT Delete(IN int title);

	HRESULT AddOneVOBU(IN vobu_admap_t *vobu_admap, IN unsigned int vobuStartAddr);

private:

	int m_newTitleNumber;
	int m_vobu_admap_index;
};

#endif
