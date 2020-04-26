#ifndef __CVRMI_H__
#define __CVRMI_H__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


//For mydvdread
#include <io/ifo_types.h>
#include <io/ifo_parse.h>
#include <io/ifo_write.h>
#include <io/ifo_VRMI.h>
#include <io/dvd_write.h>

#include <OSAL.h>
#include <Util.h>
#include <hresult.h>     

#include <Application/AppClass/setupdef.h>



#define IN
#define OUT

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//#define DISC_TOTAL_SPACE   0x22bf40  // disc's total space RLBN
// Since write disc is not stable beyond the address 0x224000, so we fix the capacity
#define DISC_TOTAL_SPACE   0x220000

// when function parameter is RECnum, used to indicate VRMI_GI field
#define VRMI_GI_DSC 99

#define VRMI_UNKNOWN_TYPE 0



class CVRMI {
	
public:
    enum NAME_TYPE    {NM=0, ALT_NM=1};    // indicate name field is NM or ALT_NM
    enum PLMATCH_TYPE {NO_MATCH=1, VOBU_ACC, V_FRAME_ACC}; // one of VRMI_RECI's REC_MODE filed
    enum RECBRT_TYPE  {CBR=1, CVBR, VBR, MIX_BR};          // VRMI_RECI's REC_BRT
    enum RECSRC_TYPE  {A_BROAD=1, A_CAMERA, A_KNOWN, D_BROAD=17, D_CAMERA, D_UNKNOW, MIX_SRC=255}; // VRMI_RECI's REC_SRC
	
protected:
    VRMI_st  *m_pVRMI; 
    FILE *in;
    dvd_info_t *m_dvd;
    dvd_file_t *m_file;
    uint32_t m_splitThreshold;
    int m_FreeFlag;
    unsigned int m_discTotalCapacity;
	
    HRESULT CheckVRCHP(IN VRCHP_IT_st *vrchp);
	
public:
	
    // The VRMI constructor takes:
    // pVRMI pointer,
    //  if it is NULL, (means Authoring filter call this constructor)
    //    constructor will try open VIDEO_RM.IFO or .BAK file into m_pVRMI.
    //    if files are not exist, call Init() to initial one new VRMI structure.
    //    then return to the caller.
    //  if it is NOT NULL, (means Navigator filter call this constructor)
    //    it will be got from shared database and later will be shared the same
    //    copy by dvd navigator filter even it is modified.
    CVRMI(IN dvd_info_t *dvd, IN OUT VRMI_st **pVRMI);
	
    // free the space allocated during construction
    virtual ~CVRMI(void);
	
    virtual HRESULT Open(IN OUT VRMI_st **pVRMI)=0;

    virtual HRESULT Close()=0;

    // Init the content of VRMI.
    // called by constructor, or when format a disc or something else.
    virtual HRESULT Initialize(IN OUT VRMI_st **pVRMI)=0;
	
    // It will check all title, 
    // if isForce is TRUE, means it will delete all titles including protected title
    // otherwise, if there are protected title, it will return false
    virtual HRESULT EraseAll(IN OUT VRMI_st **pVRMI, IN bool isForce)=0;
	
	
    // Update the VIDEO_RM.IFO/VIDEO_RM.BAK based on the
    // m_pVRMI structure and dvd_reader_t's fd
    // into the optical disc from the start address startLBA
    virtual HRESULT Realize(bool isIFO, IN int BupStartLBA)=0;
	
    // Get number of Title (including free space title)
    HRESULT getTotalTitle(OUT int *Tnum);
	
	
    HRESULT getRecordInfo(IN int Tnum, OUT uint32_t * begin_lbn,
                          OUT uint32_t * end_lbn, OUT bool * isProtect); 
	
    // Disc Total Space related functions
    unsigned int getDiscTotalCapacity();
    HRESULT setDiscTotalCapacity(unsigned int capacity);

    // Check if this title is free title
    HRESULT IsFreeTitle(IN int RECnum, OUT bool *isFree);

    // Check if Disc is full
    HRESULT IsDiscFull(OUT bool *isFull);

    // Check if this title is empty title
    // using for drawing menu SPU
    HRESULT IsEmptyTitle(IN int RECnum, OUT bool *isEmpty);
    
    // add one free title
    // only modify vrmi_gi->ftt_ns ++
    HRESULT CreateNewTitle();

    // Different with SplitRECI, 
    // This function is used to split one title to two titles which have content
    HRESULT SplitTitle(IN int RECnum, IN int lower_pbtm_frames, IN uint32_t lower_end_a, IN int higher_vrchpnum, IN bool isVRCHPSplit, IN VRCHP_IT_st *pVRCHP);

    // Merge two Titles which have contents
    HRESULT GeneralMergeTitle(IN int RECnum);
	
    ///////////////////////////////////////////////////////////////////////////////////////
    // Check if this disc recordable (Total Free Space, VRMI_RECI num, VRCHP num)
    // it will return a recording number can be recored now
    // priority:  remain free space > bigger deleted title > smaller deleted title 
    virtual HRESULT CanRecord(OUT int *RECnum, OUT unsigned int *space)=0;
    virtual HRESULT CanRecord(OUT int *RECnum, OUT unsigned int *space, IN unsigned int NWA)=0;
	
    // For Add recording...
    // It can help to fill VRMI_RECI's some fields
    // as the parameter following........
    // pl_match: in REC_MODE's "PL match" (VRMI_UNKNOWN_TYPE, NO_MATCH, VOBU_ACC, V_FRAME_ACC)
    // rec_brt:  REC bitrate mode (VRMI_UNKNOWN_TYPE, CBR, CVBR, VBR, MIX_BR)
    // brt_level: REC bitrate level (input: 4~34)
    // rec_src: REC source (VRMI_UNKNOWN_TYPE, A_BROAD=1, A_CAMERA, A_KNOWN, D_BROAD=17, D_CAMERA, D_UNKNOW, MIX_SRC=255)
    // rec_cnt: REC content level, used by Digital Video Broadcasting system
   virtual HRESULT PreFillRECI(IN int               RECnum, 
                               IN enum PLMATCH_TYPE pl_match, 
                               IN enum RECBRT_TYPE  rec_brt, 
                               IN ENUM_REC_QUALITY  brt_qty,
                               IN enum RECSRC_TYPE  rec_src,
                               IN uint8_t           rec_cnt,
                               IN bool              audio_iec)=0;
	
    // REC End!!   some values need to be set:
    // RecordingEnd(REC_PB_TM, REC_E_A, REC_KF_PNT)
    // it will fill in some fields, and decide split title or not(according to end_a)
    virtual HRESULT RecordingEnd(IN int RECnum, IN VRMI_PB_TM *pbtm, 
                                 IN uint32_t end_a, IN uint32_t f_space,
                                 OUT bool *isSplit, OUT bool *isFull)=0;     
	
    // Set threshold to decide when recording not full,
    // title will be split or not
    HRESULT SetSplitThreshold(IN uint32_t threshold);
	
    // Get the Split threshold
    HRESULT GetSplitThreshold(OUT uint32_t *threshold);

    // For +R disc, to add the end address (add Files, FS, RSAT area)
    HRESULT AddRecordingEndAddress(IN int RECnum, IN uint32_t addr);

    
    //////////////////////////////////////////////////////////////////////////
    //API about count Free Space:
			
    // Return this title's free space
    // if recorded title, return 0
    // if deleted title, return this title's free space (dont consider other neighbor titles)
    // if empty title, return this title's all free space (maybe DISC_TOTAL_SPACE - all nonempty recordings)
    virtual HRESULT GetRECFreeSpace(IN int RECnum, OUT unsigned int * space)=0;

    // Return this title's space
    // recorded title and deleted title and empty title are all included.
    virtual HRESULT GetRECSpace(IN int RECnum, OUT unsigned int * space)=0;
    virtual HRESULT GetRECSpace(IN int RECnum, OUT unsigned int * space, IN unsigned int NWA)=0;

	
    /////////////////////////////////////////////////////////////////////////
	
    // Set one recording shall not be overwritten or deleted
    HRESULT protectTitle(IN int RECnum, IN bool isProtect);
	
    // Delete one recording,
    // if it is not the last recording,
    //   it will set most VRMI_RECI[RECnum]'s fields zero except REC_E_A
    // else
    //   it will set all fields zero!
    // and modify corresponding VRCHP_IT
    // !!NOTICE: it doesn't modify FTT_ns
    virtual HRESULT DeleteTitle(IN int RECnum)=0;
	
    // After call DeleteRecording(), may call MergeRECIs() recursively
    // it will modify FTT_ns
    virtual HRESULT MergeRECIs(IN int RECnum1, IN int RECnum2)=0;

	
    //////////////////////////////////////////////////////////////////////////////////
    //API about VR Chapters:
	
    // Return how many VRMI Chapters in this disc
    HRESULT GetTotalVRCHPnum(OUT int *VRCHPnum);
	
	HRESULT GetTotalVRCHPnum(IN int RECnum, OUT int *VRCHPnum);
	
	// Locate VRCHP num in title according to lba
	HRESULT LocateVRCHPnum(IN int RECnum, IN unsigned int lba, OUT int *VRCHPnum);
	
	// Locate the index of VRCHP table
	int LocateVRCHPidx(IN int RECnum, IN int CHPnum);
	
	// Check if this address has already set to VR chapter
	int CheckVRCHPIsSet(IN unsigned int markVobuAddr);	
	
    // Add RECnum's one VRCHP_IT
    // always append it
    // If there are many VR Chapters, must call many times.
    // and this function also modify VRMI_RECI's VRPLI VRPL info
    HRESULT AddVRCHP(IN int RECnum, IN VRCHP_IT_st *vrchp);
	
    // Delete RECnum's "all" VR Chapter field
    // also delete VRMI_RECI[RECnum] all VRPLI and VRPL
    HRESULT DelAllVRCHP(IN int RECnum);
	
    // according to the index, 
    // can insert one VRCHP to anywhere
    HRESULT InsertVRCHP(IN int RECnum, IN int CHPnum, IN VRCHP_IT_st *vrchp);
	
    // according to the index, 
    // can delete one VRCHP at anywhere
    HRESULT DeleteVRCHP(IN int RECnum, IN int CHPnum);
	
    // according to index, return the pointer of VRCHP entry
    VRCHP_IT_st * getVRCHP(IN int RECnum, IN int CHPnum);
	
    /////////////////////////////////////////////////////////////////////////////////
    //API about RECI's VRPL, VRPLI entry
    // Set/get VRMI_RECI's VRPL[VRnum]
    // set it flexible
    // after set it, suggest to call CheckVRPLI for correcting VRPLI automatically
    unsigned char *ModifyVRPL(IN int RECnum, IN int VRnum);
	
    // Automatically correct VRMI_RECI's VRPLI value
    // according to the VRPL now 
    HRESULT CheckVRPLI(IN int RECnum);
    
    // Set/get VRMI_RECI's VRPLI,
    // and VRMI_RECI's VRPL,  authoring filter must take care by itself
    VRMI_VRPLI_st *VRPLI(IN int RECnum);
    
    // Set VRChp hide/unhide
    HRESULT SetVRPLHide(IN int RECnum, IN int VRChpnum, IN int setHide);
	
	
	
    ////////////////////////////////////////////////////////////////////////
    // functions below are for fast setting/getting
    // #define VRMI_GI_DSC 99
    // if RECnum = VRMI_GI_DSC, it will set VRMI_GI's corresponding field
	
    // Set NM_FMT,
    // enum NAME assign NM or ALT_NM
    HRESULT SetNameFMT(IN int RECnum, IN enum NAME_TYPE nType, IN int fmt);
	
    // Get NM_FMT
    // enum NAME assign NM or ALT_NM
    uint8_t * GetNMFMT(IN int RECnum, IN NAME_TYPE ntype);
	
    // Set NAME, also modify NM_FMT's length
    // enum NAME assign NM or ALT_NM
    HRESULT SetNAME(IN int RECnum, IN NAME_TYPE nType, IN char *name, IN int length);
	
    // Get NAME
    // enum NAME assign NM or ALT_NM
    char * GetNAME(IN int RECnum, IN NAME_TYPE nType);
	
    char * GetMANID();
    char * GetMANUD();
	
    HRESULT UpdateDateTime(IN int RECnum);
	
    // Set/get KF_PNT
    KF_PNT_st *KFPNT(IN int RECnum);

    //Set/Get Rec_PB_TM
    VRMI_PB_TM &REC_PBTM(IN int RECnum);

    //Set/Get TV system type
    HRESULT GetTVSystemType(OUT bool *isNTSC);
    HRESULT SetTVSystemType(IN bool isNTSC); 
   
    // Set/Get PL, PG match (REC mode)
    HRESULT SetPLMatch(IN int RECnum, IN char mode);
    HRESULT GetPLMatch(IN int RECnum, OUT char *mode);
    HRESULT SetPGMatch(IN int RECnum, IN char mode);
    HRESULT GetPGMatch(IN int RECnum, OUT char *mode);
    
    // Get how many chapters which "cell start" field != 1
    HRESULT GetTotalNewCell(OUT int *totalNewCell);

    void print_vrm_readable(FILE *); 
    uint8_t QualityToBitRateLevel(IN ENUM_REC_QUALITY brt_qty);
    HRESULT GetRECQuality(IN int RECnum, OUT ENUM_REC_QUALITY * quality);
    HRESULT BitRateLevelToQuality(IN int8_t brt_level, OUT ENUM_REC_QUALITY *qua);
	
private:
    HRESULT TranslateTime(TIME *pTime);
};




class CVRMI_PlusRW: public CVRMI {
public:
    CVRMI_PlusRW(IN dvd_info_t *dvd, IN OUT VRMI_st **pVRMI);
    ~CVRMI_PlusRW();
    
    virtual HRESULT Open(IN OUT VRMI_st **pVRMI);
    virtual HRESULT Close();
    virtual HRESULT Initialize(IN OUT VRMI_st **pVRMI);
    virtual HRESULT Realize(IN bool isIFO, IN int BupStartLBA);
    virtual HRESULT EraseAll(IN OUT VRMI_st **pVRMI, IN bool isForce);
    virtual HRESULT CanRecord(OUT int *RECnum, OUT unsigned int *space);
    virtual HRESULT CanRecord(OUT int *RECnum, OUT unsigned int *space, IN unsigned int NWA);
    virtual HRESULT PreFillRECI(IN int               RECnum,
                                IN enum PLMATCH_TYPE pl_match,
                                IN enum RECBRT_TYPE  rec_brt,
                                IN ENUM_REC_QUALITY  brt_qty,
                                IN enum RECSRC_TYPE  rec_src,
                                IN uint8_t           rec_cnt,
                                IN bool              audio_iec);
	
    HRESULT CanDubbing(IN int RECnum, IN int totalTitleNum,
                       IN int totalChapterNum, OUT bool *value);
    
    // when this recording is full, check if next title can be overwrite
    // if can be overwrite, then return title number and recordable space
    HRESULT CheckContinueRecording(IN int Titlenum, IN unsigned int RECspace, OUT int *Titlenum1, OUT unsigned int *RECspace1);

    // Delete this Title's information (including VRCHP)
    // and move titles below
    HRESULT OverwriteTitles(IN int Titlenum);
    
    // Used in PHILIPS requirement:
    //   +-R disc, combine deleted title into prev title
    //   +-RW disc, combine deleted title(<3min) into prev title
    HRESULT CombineTitle(IN int Titlenum);

    virtual HRESULT RecordingEnd(IN int RECnum, IN VRMI_PB_TM *pbtm,
                                 IN uint32_t end_a, IN uint32_t f_space,
                                 OUT bool *isSplit, OUT bool *isFull);

    // when end of recording, if needs spilit (depending on split threshold)
    // will call this function to split one recording to two
    HRESULT SplitRECI(IN int RECnum, IN uint32_t end_a,
                      IN uint32_t f_space, OUT bool *isSplit);

						
    /////////////////////////////////////////////////////////////////////////
    //API about count Free Space:

    // Return Total Disc free space (deleted title & remain space)
    HRESULT GetTotalFreeSpace(OUT unsigned int * space);

    // Return suggested recording record
    // If there are free space after all recordings, return a recording number can be used
    // If there are no free space after all recordings,
    // then return the recording number which has biggest free space
    HRESULT GetLargestSpace(OUT int * RECnum, OUT unsigned int * space);

    virtual HRESULT GetRECFreeSpace(IN int RECnum, OUT unsigned int * space);

    virtual HRESULT GetRECSpace(IN int RECnum, OUT unsigned int * space);
    virtual HRESULT GetRECSpace(IN int RECnum, OUT unsigned int * space, IN unsigned int NWA);

    ////////////////////////////////////////////////////////////////////////////
    // API about delete and merge
    virtual HRESULT DeleteTitle(IN int RECnum);
    virtual HRESULT MergeRECIs(IN int RECnum1, IN int RECnum2);

    char * GetRecordUD(IN int RECnum);
};
  
class CVRMI_Video :public CVRMI{
public:
    CVRMI_Video(IN dvd_info_t *dvd, IN OUT VRMI_st **pVRMI);
    ~CVRMI_Video();

    virtual HRESULT Open(IN OUT VRMI_st **pVRMI);
    virtual HRESULT Close();
    virtual HRESULT Initialize(IN OUT VRMI_st **pVRMI);
    virtual HRESULT Realize(IN bool isIFO, IN int startLBA);
    virtual HRESULT EraseAll(IN OUT VRMI_st **pVRMI, IN bool isForce);
    virtual HRESULT CanRecord(OUT int *RECnum, OUT unsigned int *space);
    virtual HRESULT CanRecord(OUT int *RECnum, OUT unsigned int *space, IN unsigned int NWA);
    virtual HRESULT PreFillRECI(IN int               RECnum,
                                IN enum PLMATCH_TYPE pl_match,
                                IN enum RECBRT_TYPE  rec_brt,
                                IN ENUM_REC_QUALITY  brt_qty,
                                IN enum RECSRC_TYPE  rec_src,
                                IN uint8_t           rec_cnt,
                                IN bool              audio_iec);
		                        
    virtual HRESULT RecordingEnd(IN int RECnum, IN VRMI_PB_TM *pbtm,
                                 IN uint32_t end_a, IN uint32_t f_space,
                                 OUT bool *isSplit, OUT bool *isFull);

    /////////////////////////////////////////////////////////////////////////
    //API about count Free Space:	
    virtual HRESULT GetRECFreeSpace(IN int RECnum, OUT unsigned int * space);	

    virtual HRESULT GetRECSpace(IN int RECnum, OUT unsigned int * space);
    virtual HRESULT GetRECSpace(IN int RECnum, OUT unsigned int * space, IN unsigned int NWA);

    ////////////////////////////////////////////////////////////////////////////
    // API about delete and merge
    virtual HRESULT DeleteTitle(IN int RECnum);
    virtual HRESULT MergeRECIs(IN int RECnum1, IN int RECnum2);

};
#endif  /* __CVRMI_H__*/
