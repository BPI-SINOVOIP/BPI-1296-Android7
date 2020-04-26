#ifndef __C_DVD_VIDEO_NAV_H__
#define __C_DVD_VIDEO_NAV_H__

#include "common.h"
#include "CDVDVideoVM.h"
#include "NavRead.h"
#include "VOBUInfoTable.h"
#include "InputDVDVideo.h"
#include <Filters/PrivateInfo.h>

#define VOBU_INFO_TABLE_SIZE 16

class CDVDVideoNav
{
    friend class CDVDVideoVM;

public:

    CDVDVideoNav() : m_vm(this) {}
    virtual ~CDVDVideoNav()     { Unload(); }

    // input plugin initialization
    HRESULT Open(ENUM_MEDIA_TYPE majorType, ENUM_MEDIA_TYPE subType, osal_mutex_t* mutex, INPUTPLUGIN* pInputPlugin);

    // input plugin functions
    static HRESULT loadMedia(void* pInstance, char* path, bool* pbContinue, NAVLOADFAILURE* pFailure);
    static HRESULT unloadMedia(void* pInstance);
    static HRESULT getMediaInfo(void* pInstance, NAVMEDIAINFO* pMediaInfo);
    static HRESULT read(void* pInstance, NAVBUF* pBuffer, unsigned int prohibitedChannelMask, NAV_STREAM_TYPE* channelFullnessInfo);
    static HRESULT updatePlaybackPosition(void* pInstance, PRESENTATION_POSITIONS* pPosition, NAVDEMUXPTSINFO* pDemuxPTSInfo);
    static HRESULT privateDataFeedback(void* pInstance, unsigned int id, unsigned char* data, unsigned int size, int64_t lastPTS);
    static HRESULT execUserCmd(void* pInstance, NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);
    static HRESULT propertySetGet(void* pInstance, NAV_PROP_ID id, void* inData, unsigned int inDataSize,
                                  void* outData, unsigned int outDataSize, unsigned int* returnedSize);
    static HRESULT dispose(void* pInstance);

protected:

    //
    // methods exposed by the plugin
    //

    // media initialization, loading IFOs
    HRESULT Load(char* path);

    // release media, free IFOs
    HRESULT Unload();

    // reading more data packs or commands
    HRESULT Read(NAVBUF* pBuffer);

    // update current playback position
    HRESULT UpdatePosition(PRESENTATION_POSITIONS* pPosition);

    // sending private data pointer back for navigator's reference
    HRESULT PrivateDataFeedback(unsigned int id, unsigned char* data, unsigned int size);

    // to execute an user command
    HRESULT ExecUserCmd(NAV_CMD_ID id, void* cmdData, unsigned int cmdDataSize, unsigned int* pIsFlushRequired);

    // set/get properties
    HRESULT PropertySetGet(NAV_PROP_ID id, void* inData, unsigned int inDataSize, void* outData,
                           unsigned int outDataSize, unsigned int* returnedSize);

    //
    // methods for user commands
    //

    HRESULT PlayTitle                (int titleNum);
    HRESULT PlayChapter              (int titleNum, int chapterNum);
    HRESULT PlayAtTime               (int titleNum, unsigned int eltmSeconds, unsigned int frameIdx);
    HRESULT PlaySegment              (int titleNum, unsigned int startSeconds, unsigned int startFrameIdx,
                                      unsigned int endSeconds, unsigned int endFrameIdx, bool bRepeat);
    HRESULT ButtonSelectNumeric      (int buttonNum);
    HRESULT ButtonSelectPoint        (unsigned int x, unsigned int y);
    HRESULT ButtonActivateNumeric    (int buttonNum);
    HRESULT ButtonActivatePoint      (unsigned int x, unsigned int y);
    HRESULT ButtonActivate           ();
    HRESULT ButtonMoveSelection      (NAV_BTNDIR_ID direction);
    HRESULT StillEscape              ();
    HRESULT SetVideoPresentationMode (NAV_VIDEO_PRES_MODE mode);
    HRESULT SetAudioStream           (int streamNum);
    HRESULT SetSubpictureStream      (int streamNum, NAV_DISPLAY_STATE displayState, bool bDummyStream);
    HRESULT Run                      ();
    HRESULT Pause                    ();
    HRESULT Stop                     ();
    HRESULT SetSpeed                 (int speed, int skip);
    HRESULT SetSpeedPos              (int64_t pts);

    //
    // methods for property set/get
    //

    HRESULT GetPlaybackStatus   (DVDVideoVOBUInfo* info, NAVPLAYBACKSTATUS* status);
    HRESULT GetPlaybackStatusEx (DVDVideoVOBUInfo* info, NAVPLAYBACKSTATUS_DVDV* status);
    HRESULT GetVideoStatus      (NAVVIDEOSTATUS* status);
    HRESULT GetAudioStatus      (NAVAUDIOSTATUS* status);
    HRESULT GetSpicStatus       (NAVSPICSTATUS* status);
    HRESULT GetMenuStatus       (NAVMENUSTATUS* status);
    HRESULT GetDiscStatus       (NAVDISCSTATUS* status);
    HRESULT GetTitleStatus      (NAVTITLESTATUS* status, int titleNum);
    HRESULT GetNavState         (void* state, unsigned int size, unsigned int* returnedSize);
    HRESULT GetCmdBlockingFlags (int* blocking);
    HRESULT GetButtonJumpTarget (btni_t* btni, int* titleNum);
    HRESULT SetNavState         (void* state, unsigned int size, bool bForced);
    HRESULT SetRepeat           (NAV_REPEAT_MODE repeatMode);
    HRESULT SetParentalControl  (NAVPARENTALCONTROL* pParentalControl);
    HRESULT SetLangPreference   (NAVLANGPREFERENCE* pLangPreference);
    HRESULT ApproveTmpParental  (bool bApproved);

    //
    // methods exposed to VM
    //

    // prepare a new playback session
    HRESULT SetPlayback(uint32_t startAddress,      // start address (lower bound)
                        uint32_t endAddress,        // end address (upper bound)
                        uint32_t targetAddress,     // address to start playback from
                        uint32_t targetFrameNumber, // frame number to start playback from
                        bool     bSearchForward,    // true if to search forward for target frame
                        bool     bBranching,        // true if jumping to new location
                        DVDVideoState* pState);     // new state

    // prepare a STILL session
    void SetStill(int stillSeconds,       // number of seconds to hold still (-1: INFINITE)
                  DVDVideoState* pState); // new state

    // prepare WAITING for presentation to finish
    HRESULT SetWaitingForFinish(DVDVideoState* pState); // new state

    // prepare pending on TmpPML request
    HRESULT SetRequestTmpPML(int parentalLevel, int nextLineOnOK, int nextLineOnNG, bool bPreCmd, DVDVideoState* pState);

    // prepare for entering STOP state
    void SetStop(DVDVideoState* pState); // new state

    // simply change key parameters of state
    void SetParameters(DVDVideoState* pState); // reference state

    // request a seamless angle change
    void SetSeamlessAngleChange();

    // request/release IFOs (VMG/VTS), interlocked with authoring module if necessary
    inline vmg_t* LockVMG()         { return m_reader.LockVMG(); }
    inline vts_t* LockVTS(int vtsN) { return m_reader.LockVTS(vtsN); }
    void   UnlockALL();

    // get current UOP permissions
    int GetUOPFlags();

    // state related functions
    void GetReadingState (DVDVideoState* pState); // get state of reading position
    void GetPlayingState (DVDVideoState* pState); // get state of playing position

    // VOBUInfo related functions
    DVDVideoVOBUInfo* GetReadingVOBUInfo(); // get VOBUInfo of reading position
    DVDVideoVOBUInfo* GetPlayingVOBUInfo(); // get VOBUInfo of playing position
    DVDVideoVOBUInfo* GetOngoingVOBUInfo(); // get VOBUInfo that's under construction

    // get PTS of playing position
    inline int64_t GetPlayingPTS() { return m_lastPresentedPTS; }

    // functions to update various types of playback information
    void UpdateSendFlush         ();
    void UpdateVideoTarget       (bool bActive, int id, int subid);
    void UpdateVideoAttr         (video_attr_t* attr);
    void UpdateVideoPresMode     ();
    void UpdateAudioTarget       (bool bActive, int id, int subid);
    void UpdateSubpictureTarget  (bool bActive, int id, int subid, bool bDisplay);
    void UpdateSubpicturePalette (uint32_t* palette);

    //
    // internal procedures
    //

    // reset state to as initial access
    void Reset();

    // update pending changes (if any)
    bool HandlePendingChanges(NAVBUF* pBuffer);

    // prepare next VOBU for presentation
    void PrepareNextVOBU(DVDVideoVOBUInfo* pCurrVOBUInfo);

    // get jump destination address according to SRI and target frame number
    uint32_t GetJumpDestinationAddress(DVDVideoVOBUInfo* pCurrVOBUInfo);

    // VOBU initialization
    void InitVOBU(DVDVideoVOBUInfo* pVOBUInfo, NAVBUF* pBuffer);

    // read blocks from VOBU
    HRESULT ReadBlocksFromVOBU(uint32_t readEndAddr, NAVBUF* pBuffer, DVDVideoVOBUInfo* pVOBUInfo);

    // skip VOBUs (forward or backward) on encountering read errors
    HRESULT SkipVOBUsOnReadError(DVDVideoVOBUInfo* pVOBUInfo);

    // procedure on entering a VOBU
    void OnVOBUPlayingBegin(DVDVideoVOBUInfo* pVOBUInfo);

    // procedure on leaving a VOBU
    void OnVOBUPlayingEnd(DVDVideoVOBUInfo* pVOBUInfo);

    // check if presentation already finished
    bool IsPresentationFinished();

    // according to speed settings, update scanning mode and related parameters
    void UpdateScanMode();

    // set decode command (frame accurate decode-begin-point and presentation duration)
    void SetDecodeCommand(int beginFrameIndex, int durationInFrameCount); // duration -1 means INFINITE

    // jump to point A of the AB repeat region
    HRESULT JumpToPointA();

    // pack private-info NAVBUF
    void PackPrivateInfo(unsigned int infoId, void* infoData, unsigned int infoSize, NAVBUF* pBuffer);

    // pack event NAVBUF
    void PackEvent(unsigned int eventId, void* eventData, unsigned int eventSize, NAVBUF* pBuffer);

    //
    // inline function related to speed/scan settings
    //

    inline bool IsSpeedNormal      (DVDVideoState* pState) { return (pState->speed == 256);      }
    inline bool IsSpeedForward     (DVDVideoState* pState) { return (pState->speed > 0);         }
    inline bool IsScanning         ()                      { return (m_scanMode != 0);           }
    inline bool IsScanningIOnly    ()                      { return (m_scanMode == 1);           }

    inline void RestoreNormalSpeed(DVDVideoState* pState) {
        pState->speed = pState->skip = 256;
        AddFlag(pState->events, DVDV_RestoreNormalSpeed);
    }

    //
    // button helper functions
    //

    // get button information (set bForce to true to force selecting a button, updatedBtnN is to return selected button number)
    btni_t* GetBTNI(DVDVideoVOBUInfo* pVOBUInfo, int btnN, bool bForce = false, int* updatedBtnN = NULL);

    HRESULT SelectButton   (DVDVideoVOBUInfo* pVOBUInfo, int btnN, bool bForce = false, bool bNoAutoAct = false);
    HRESULT ActivateButton (DVDVideoVOBUInfo* pVOBUInfo, int btnN);

    int     GetBtnnByNum   (DVDVideoVOBUInfo* pVOBUInfo, int buttonNum);                  // get BTNN from user button number
    int     GetBtnnByPoint (DVDVideoVOBUInfo* pVOBUInfo, unsigned int x, unsigned int y); // get BTNN from (x,y) coordinate
    void    DrawButton     (DVDVideoVOBUInfo* pVOBUInfo, btni_t* btni, bool bAct);        // draw a button

    //
    // internal variables and definitions
    //

    CDVDVideoVM     m_vm;                 // navigator virtual machine
    DVDVideoState   m_readingState;       // current reading state
    CDVDVideoReader m_reader;             // dvd reader
    ENUM_MEDIA_TYPE m_majorType;          // input media type (major)
    ENUM_MEDIA_TYPE m_subType;            // input media type (sub)
    osal_mutex_t*   m_pMutex;             // mutex of outside filter
    int             m_stillSeconds;       // remaining number of seconds to hold still
    int             m_waitCount;          // counting how many WAIT periods had elapsed
    bool            m_bRunning;           // whether presentation is running or paused
    int64_t         m_lastReferenceClock; // last updated reference clock
    int64_t         m_lastPresentedPTS;   // last updated presented PTS
    int64_t         m_tickTimer;          // counting timer ticks (90khz)
    int64_t         m_tickSincePresented; // ticks since latest presented PTS (90khz)
    int             m_buttonActDelay;     // number of delay periods (use NAVBUF_WAIT_TIME) after button activation
    int             m_buttonActHLIs;      // number of HLI periods during which the button activation color remains
    bool            m_bHackUOP;           // if true, all UOPs are permitted
    unsigned long   m_pPhyAddrOffset;     // difference between virtual non-cache and physical address of read buffer

    struct { int64_t startPTS;
             int     startFrameN; } m_vobuInfoBeforeSpeedChange; // VOBU info before speed change

    struct { int          title;
             unsigned int frameN_A;
             unsigned int frameN_B;
             bool         bRepeat;  } m_repeatAB; // AB repeat region

    // VOBU information table
    CVOBUInfoTable<VOBU_INFO_TABLE_SIZE> m_VOBUInfoTable;
    int64_t                              m_VOBUInfoTableFullPTS;

    // read buffer
    unsigned char* m_pReadBuffer;

    // media signature (16 bytes MD5 signature from the first 32K bytes of VMG and VTS #1 IFOs)
    uint32_t m_mediaSignature[4];

    // current playback session
    uint32_t m_startAddress;         // start address of current session
    uint32_t m_endAddress;           // end address of current session
    uint32_t m_currAddress;          // address of current playback position
    uint32_t m_seekTargetFrameN;     // target frame number we're seeking to
    uint32_t m_dispTargetFrameN;     // target frame number we displayed to user via GetPlaybackStatus
    bool     m_bVOBUInited;          // true if VOBU is already initialized
    bool     m_bNewSession;          // true if the session is just started
    bool     m_bNewSegment;          // true if we should deliver NewSegment before feeding new data
    bool     m_bHLIActive;           // true if a valid HLI is active
    bool     m_bFlushOnPlay;         // true means need to FLUSH before next play session
    bool     m_bTimeShifting;        // true if playing/recording are on the same title
    bool     m_bSeamlessAngleChange; // true if seamless angle change is requested
    bool     m_bSearchForward;       // true if search forward for target, false if search backward
    long     m_playbackRate;
    long     m_readErrorLevel;

    // scanning parameters
    uint32_t m_scanMode;              // 0:linear-play, 1:I-scan N:VOBU-scan(N>1, also means number of frames played per stride)
    uint32_t m_scanStride;            // number of frames per stride (played + skipped frames)
    int      m_scanPlayedFrameError;  // error count of played frames during scanning
    int      m_scanSkippedFrameError; // error count of skipped frames during scanning

    // TmpPML request related parameters
    int  m_tmpPmlRequestedLevel;
    int  m_tmpPmlNextLineOnOK;
    int  m_tmpPmlNextLineOnNG;
    bool m_tmpPmlIsPreCmd;

    // type of pending playback information update
    enum { PENDING_FLUSH            = 0x00000001,
           PENDING_STC_OFFSET       = 0x00000002,
           PENDING_VIDEO_DMX_TARGET = 0x00000004,
           PENDING_VIDEO_PRES_MODE  = 0x00000008,
           PENDING_VIDEO_PUSH_FRAME = 0x00000010,
           PENDING_AUDIO_DMX_TARGET = 0x00000020,
           PENDING_SPIC_DMX_TARGET  = 0x00000040,
           PENDING_SPIC_PALETTE     = 0x00000080,
           PENDING_SPIC_BUTTON      = 0x00000100,
           PENDING_SPIC_ON_OFF      = 0x00000200,
           PENDING_BUTTON_ACT_DELAY = 0x00000400,
           PENDING_MIXER_BLEND      = 0x00000800,
           PENDING_NEW_SEGMENT      = 0x00001000,
           PENDING_VIDEO_DECODECMD  = 0x00002000,
           PENDING_END_SEGMENT      = 0x00004000,
           PENDING_RESET_SPEED      = 0x00008000,
           PENDING_NOTIFY_DOMAIN    = 0x00010000,
           PENDING_NOTIFY_TITLE     = 0x00020000,
           PENDING_NOTIFY_CHAPTER   = 0x00040000,
           PENDING_REQUEST_TMPPML   = 0x00080000,
           PENDING_VOBU_INFO        = 0x00100000,
           PENDING_V_ATR            = 0x00200000,
           PENDING_DCI_CCI          = 0x00400000,
           PENDING_SPIC_CONFIG      = 0x00800000,
           PENDING_AUDIO_FLUSH      = 0x01000000,
           PENDING_SPIC_FLUSH       = 0x02000000,
           PENDING_AUDIO_DECODECMD  = 0x04000000,
           PENDING_CSS              = 0x08000000,
           PENDING_FATAL_ERROR      = 0x10000000 };

    // pending changes
    int m_pendingChanges;

    // data for pending updates
    DEMUXTARGET         m_updatedVideoTarget; // updated video demux target
    DEMUXTARGET         m_updatedAudioTarget; // updated audio demux target
    DEMUXTARGET         m_updatedSpicTarget;  // updated subpicture demux target
    SPUDVDVPALETTE      m_updatedSpicPalette; // updated subpicture palette
    SPUDVDVBUTTON       m_updatedSpicButton;  // updated subpicture button information
    SPUCONFIG           m_updatedSpicConfig;  // updated subpicture configuration
    int                 m_updatedSpicOnOff;   // updated subpicture on/off status
    int64_t             m_updatedSTCOffset;   // updated STC offset
    VIDEODVDVOBUINFO    m_updatedVOBUInfo;    // updated VOBU info
    VIDEODVDDCICCI      m_updatedDCI_CCI;     // updated DCI CCI
    unsigned int        m_updatedV_ATR;       // updated V_ATR
    VIDEODECODECOMMAND  m_updatedVideoDecCmd; // updated video decode command
    VIDEOMIXERBLEND     m_updatedBlendCmd;    // updated blend command
    AUDIODECODECOMMAND  m_updatedAudioDecCmd; // updated audio decode command

    // remember current state to sense when to send state-change notifications
    DVDVideoDomain m_currDomain;
    int            m_currTTN;
    int            m_currPTTN;

    // remember current copy protection level to sense when to send updated info downstream
    int m_currAPS;
    int m_currCGMS;
    int m_currRWAspectRatio;
    bool m_bSendAPSEvent;

};

#endif //__C_DVD_VIDEO_NAV_H__
