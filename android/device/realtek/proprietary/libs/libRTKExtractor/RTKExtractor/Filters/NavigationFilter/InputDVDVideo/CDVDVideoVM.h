#ifndef __C_DVD_VIDEO_VM_H__
#define __C_DVD_VIDEO_VM_H__

#include "common.h"
#include "CDVDVideoCmdHandler.h"
#include "ifoHelpers.h"

class CDVDVideoNav;

// dvd-video navigation virtual machine

class CDVDVideoVM
{
    friend class CDVDVideoCmdHandler;

public:

    CDVDVideoVM(CDVDVideoNav* pNav) : m_commandHandler(this), m_pNav(pNav) {}
    virtual ~CDVDVideoVM() {}

    // linearly play into the next unit
    HRESULT PlayNext();

    // DVD user operations (Annex J, VIX-61)
    HRESULT Menu_Call           (int menuID);
    HRESULT Title_Play          (int ttN, bool bResetParam = true);
    HRESULT PTT_Play            (int ttN, int pttN);
    HRESULT PTT_Search          (int pttN);
    HRESULT PrevPG_Search       ();
    HRESULT TopPG_Search        ();
    HRESULT NextPG_Search       ();
    HRESULT Time_Play           (int ttN, int seconds, int frames);
    HRESULT Time_Search         (int seconds, int frames);
    HRESULT GoUp                ();
    HRESULT Resume              ();
    HRESULT Stop                ();
    HRESULT Audio_Stream_Change (int streamNum);
    HRESULT SP_Stream_Change    (int streamNum, bool bDisplay);
    HRESULT Angle_Change        (int angleNum);

    // our proprietary operations
    HRESULT FirstPlay            ();
    HRESULT ExecuteButtonCmd     (vm_cmd_t* pCmd);
    HRESULT TimerUpdateOneSecond ();
    HRESULT RestartFromVOBU      (uint32_t targetAddress);
    HRESULT RestartFromState     (DVDVideoState* pState, DVDVideoVOBUInfo* info, bool bForced);
    HRESULT ResumeAfterSetTmpPML (int nextLine, bool bPreCmd);

    // enquiry services
    HRESULT GetCmdPreview(vm_cmd_t* pCmds, int totalLines, int startLine, DVDVideoState* pState,
                          DVDVideoBranchCmd* pBranchCmd, DVDVideoCmdArea area);

    uint32_t FindVOBUAddrInCell_BySearchingADMAP(DVDVideoState* pState, int VOB_IDN, int C_IDN,
                                                 uint32_t referenceVOBUAddr, int VOBUNumberOffset);

    video_attr_t* GetCurrVideoAttr      (DVDVideoState* pState);
    audio_attr_t* GetCurrAudioAttr      (DVDVideoState* pState, int* pNumStream);
    subp_attr_t*  GetCurrSpicAttr       (DVDVideoState* pState, int* pNumStream);
    uint16_t*     GetCurrAudioControl   (DVDVideoState* pState);
    uint32_t*     GetCurrSpicControl    (DVDVideoState* pState);
    HRESULT       GetCurrCellPosInfo    (DVDVideoState* pState, int* VOB_IDN, int* C_IDN);
    title_info_t* GetTitleInfo          (DVDVideoState* pState, int* pNumTitles);
    void          GetMenuInfo           (DVDVideoState* pState, int* vmgmLCD, int* vmgmEXST, int* vtsmLCD, int* vtsmEXST);
    bool          GetDiscInfo           (DVDVideoState* pState, unsigned int* regionMask);
    int           GetTitleFrameCount    (DVDVideoState* pState, int titleNum, int* pFramesPerSecond);
    uint32_t      GetCurrCellEndAddress (DVDVideoState* pState); // for time-shifting
    int           GetCurrSeqEndFrameN   (DVDVideoState* pState); // for time-shifting
    int           GetCurrAngleCellN     (DVDVideoState* pState); // for cell consistency after seamless-angle-change

    void ForceToUpdateAudioTarget();
    
protected:

    //
    // DVD navigation commands (internal)
    //

    // link commands
    HRESULT LinkPGCN    (DVDVideoBranchCmd* pNewBranchCmd, int pgcN);
    HRESULT LinkPTTN    (DVDVideoBranchCmd* pNewBranchCmd, int pttN,  int btnN, bool bSkipPreCmd = true);
    HRESULT LinkPGN     (DVDVideoBranchCmd* pNewBranchCmd, int pgN,   int btnN);
    HRESULT LinkCN      (DVDVideoBranchCmd* pNewBranchCmd, int cellN, int btnN);

    // link sub commands
    HRESULT LinkTopC    (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkNextC   (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkPrevC   (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkTopPG   (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkNextPG  (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkPrevPG  (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkTopPGC  (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkNextPGC (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkPrevPGC (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkGoUpPGC (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT LinkTailPGC (DVDVideoBranchCmd* pNewBranchCmd, int btnN);
    HRESULT RSM         (DVDVideoBranchCmd* pNewBranchCmd);

    // jump commands
    HRESULT Exit        ();
    HRESULT JumpTT      (DVDVideoBranchCmd* pNewBranchCmd, int ttN);
    HRESULT JumpVTS_TT  (DVDVideoBranchCmd* pNewBranchCmd, int vts_ttN);
    HRESULT JumpVTS_PTT (DVDVideoBranchCmd* pNewBranchCmd, int vts_ttN, int pttN);
    HRESULT JumpSS      (DVDVideoBranchCmd* pNewBranchCmd, int operand);
    HRESULT JumpSS_FP   (DVDVideoBranchCmd* pNewBranchCmd);
    HRESULT JumpSS_VMGM (DVDVideoBranchCmd* pNewBranchCmd, int menuID, int vmgm_pgcN);
    HRESULT JumpSS_VTSM (DVDVideoBranchCmd* pNewBranchCmd, int menuID, int vtsN, int vts_ttN);
    HRESULT CallSS      (DVDVideoBranchCmd* pNewBranchCmd, int operand);
    HRESULT CallSS_FP   (DVDVideoBranchCmd* pNewBranchCmd, int rsmCN);
    HRESULT CallSS_VMGM (DVDVideoBranchCmd* pNewBranchCmd, int rsmCN, int menuID, int vmgm_pgcN);
    HRESULT CallSS_VTSM (DVDVideoBranchCmd* pNewBranchCmd, int rsmCN, int menuID);

    // set-temporary-parental-management-level (a special goto command)
    HRESULT SetTmpPML   (int parentalLevel, int nextLineOnOK, int currentLine);

    // proprietary commands
    HRESULT SkipDummyPGC            (DVDVideoBranchCmd* pNewBranchCmd);
    HRESULT BounceOffRestrictedCell (DVDVideoBranchCmd* pNewBranchCmd);

    //
    // others (helpers, internal procedures, internal definitions)
    //

    // flags for internal play procedures
    enum { ASSIGN_PG            = 0x00000001,
           ASSIGN_CELL          = 0x00000002,
           ASSIGN_VOBU          = 0x00000004,
           ASSIGN_FRAME         = 0x00000008,
           ASSIGN_CMD_LINE_NUM  = 0x00000010,
           SKIP_PGC_PRE_CMD     = 0x00010000,
           RES_CELL_BOUNCE      = 0x00020000  };

    // conditions for ResetParameters procedure
    enum { ON_Play                  = 0x00000001,
           ON_MenuCall_Stop_To_VMGM = 0x00000002,
           ON_MenuCall_Stop_To_VTSM = 0x00000004,
           ON_MenuCall_VMGM_To_VTSM = 0x00000008  };

    // preparation at the beginning of DVD user operations
    void CmdPrologue();

    // to invoke DVD navigation commands based on command descriptor
    HRESULT ProcessBranch(DVDVideoBranchCmd* pBranchCmd);
    HRESULT DispatchBranchCmd(DVDVideoBranchCmd* pBranchCmd);

    // internal play procedures
    HRESULT PlayPGC       (int flag, pgc_t* pgc, DVDVideoBranchCmd* pNewBranchCmd); // enter a new PGC
    HRESULT PlayPGC_Post  (int flag, pgc_t* pgc, DVDVideoBranchCmd* pNewBranchCmd); // leaving current PGC
    HRESULT PlayPG        (int flag, pgc_t* pgc, DVDVideoBranchCmd* pNewBranchCmd); // enter a new PG
    HRESULT PlayPG_Post   (int flag, pgc_t* pgc, DVDVideoBranchCmd* pNewBranchCmd); // leaving current PG
    HRESULT PlayCell      (int flag, pgc_t* pgc, DVDVideoBranchCmd* pNewBranchCmd); // enter a new Cell
    HRESULT PlayCell_Post (int flag, pgc_t* pgc, DVDVideoBranchCmd* pNewBranchCmd); // leaving current Cell

    // navigation sub procedures
    void     ResetParameters         (int condition);          // reset GPRM/SPRM according to condition (VI3-17, VI3-18)
    bool     SetTitle                (int ttN);                // preparation before title entrance, return false if rejected
    void     ReselectAudioSubpicture (pgc_t* pgc);             // apply audio/subpicture selection algorithm 1 (VI3-55)
    int      SelectForcedSubpicture  (pgc_t* pgc, int spstn);  // handle subpicture forcedly display (VI3-57)
    void     UpdateSettings          (pgc_t* pgc);             // update settings if necessary
    void     SetVideoTarget          (pgc_t* pgc);             // setup video demux target
    void     SetVideoPresMode        ();                       // setup video presentation mode
    void     SetAudioTarget          (pgc_t* pgc);             // setup audio demux target
    void     SetAudioMixMode         ();                       // setup audio decoder karaoke mixing mode
    void     SetSubpictureTarget     (pgc_t* pgc);             // setup subpicture demux target
    void     SaveRSM                 (int rsmCN);              // save information for resumption
    uint32_t RewindToSpicOrVideo     (DVDVideoVOBUInfo* info); // rewind and find the location of closest spic or video
    void     SwitchAngle             (pgc_t* pgc);             // seamless/non-seamless angle switch procedure
    bool     TimeSeek                (pgc_t* pgc, int seconds, int frames, int* pFlag); // info look-up for Time_Search/Time_Play

    // navigation helper functions
    vmg_t*        GetVMG              ();
    vts_t*        GetVTS              (int vtsN);
    pgcit_t*      GetPGCIT            (DVDVideoDomain domain, int vtsN, int M_LCD);
    pgcit_t*      GetMenuPGCIT        (pgci_ut_t* pgci_ut, int M_LCD);
    pgci_lu_t*    GetMenuPGCI_LU      (pgci_ut_t* pgci_ut, int M_LCD);
    void          CheckAngle          (pgc_t* pgc, int PGFirstCellN, int PGLastCellN);
    HRESULT       SetNextCell         (pgc_t* pgc, int PGLastCellN);
    HRESULT       SetPrevCell         (pgc_t* pgc, int PGFirstCellN);
    void          SetBTNN             (int btnN);
    void          SetChapter          (pgc_t* pgc, int PGFirstCellN, int PGLastCellN);
    void          CalculateFrameCount (pgc_t* pgc, int PGFirstCellN);
    int           GetFramesPerSecond  (video_attr_t* attr);
    int           GetPtsTickPerFrame  (video_attr_t* attr);
    int           GetParentalIDField  ();

    inline pgcit_t* GetCurrentPGCIT() { return GetPGCIT(m_state.domain, m_state.vtsN, m_state.M_LCD()); }
    inline pgc_t*   GetCurrentPGC()   { return getPGC(GetCurrentPGCIT(), m_state.pgcN); }
    inline void     DiscardRSM()      { m_state.rsm.ttN = 0; }
    inline bool     HasRSM()          { return (m_state.rsm.ttN > 0); }

    //
    // data members
    //

    DVDVideoState       m_state;             // the state we're working on
    DVDVideoCmdArea     m_cmdArea;           // current command area
    uint32_t            m_targetVOBUAddress; // used to assign VOBU address of target position
    uint32_t            m_targetFrameNumber; // used to assign frame number of target position
    int                 m_targetCmdLineNum;  // used to assign target command line number (pgc pre or post cmd)
    bool                m_bBranching;        // true if we're branching to a new location (rather than natural advancing)
    CDVDVideoCmdHandler m_commandHandler;    // command handler
    CDVDVideoNav*       m_pNav;              // reference to outside navigator
};

#endif //__C_DVD_VIDEO_VM_H__
