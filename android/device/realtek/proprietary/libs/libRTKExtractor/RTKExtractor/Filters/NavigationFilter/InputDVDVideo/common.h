#ifndef __DVD_VIDEO_COMMON_H__
#define __DVD_VIDEO_COMMON_H__

#include <OSAL.h>
#include <Util.h>
#include <Filters/NavigationFilter/NavDef.h>
#include "NavTypes.h"

// configurable parameters

// PTS would start from this value on entering a new segment (90khz)
#define START_PTS_FOR_NEW_SEGMENT 1000000L
// PTS would start from this value (90khz) and decrement if played in reverse direction (it takes ~35 years to wrap around)
#define START_PTS_FOR_REVERSE 100000000000000LL
// wait time in milliseconds for each NAVBUF_WAIT issued
#define NAVBUF_WAIT_TIME 10
// number of delay periods (use NAVBUF_WAIT_TIME) after button activation
#define BUTTON_ACT_DELAY 30
// number of HLI periods during which the button activation color remains
#define BUTTON_ACT_DELAY_HLI 2
// when stuck in WAIT modes, allow no more than this number of loops before escaping
#define PRESENTATION_WAIT_LIMIT 100
// seek to this number of frames prior to actual destination, to be able to locate leading B-frames in open GOP
#define SEEK_FRAME_OFFSET 2
// wait time in milliseconds for each read-error encountered
#define READ_ERROR_WAIT_TIME 100
// error tolerance (in 90khz ticks) for stc offset adjustment
#define STC_OFFSET_ERROR_TOLERANCE 1000

// optional features
#define __BYPASS_UOP_CHECKING_FOR_AB_REPEAT__
#define __DO_NOT_RESET_SPEED_FOR_AB_REPEAT__
// #define __USE_ADAPTIVE_FRAME_COUNT_ACCURATE_I_SCAN__
// #define __SUPPORT_SDDS_AUDIO__
// #define __RETRY_READING_EXACT_AMOUNT_OF_SECTORS__
// #define __LOAD_C_ADT_AND_VOBU_ADMAP__
// #define __PLAY_NON_SEAMLESS_CELL_SEAMLESSLY__

// workarounds
#define __WORKAROUND_NEXT_CHAPTER__ // go to end of last chapter even if next PGC does not exist (violate VI3-42)
#define __WORKAROUND_PREV_CHAPTER__ // jump to previous title if it is 1st chapter in title (violate VI3-42)
#define __WORKAROUND_CELL_COMMAND__ // back to normal speed on encountering cell command (violate VI3-50)

// re-define names to make it more readable
#define vmg_t ifo_handle_t
#define vts_t ifo_handle_t

// macros for debug/trace
#define VERIFY(x)       {/* if(!(x)) printf("verify error, %s : %d\n", __FILE__, __LINE__); */}
#define DUMPCMD(x)      {/* printf x; */}
#define DUMPNAV(x)      {/* printf x; */}
#define DUMPHLI(x)      {/* printf x; */}
#define DUMPSCAN(x)     {/* printf x; */}
#define DUMPTS(x)       {/* printf x; */}
#define DUMPREADERR(x)  {   printf x;   }
#define DUMP_V_INFO(x)  {   printf x;   } // debug info for video
#define DUMP_A_INFO(x)  {   printf x;   } // debug info for audio
#define DUMP_G_INFO(x)  {   printf x;   } // debug info general
#define NAV_WARNING(x)  {   printf x;   }

// helper functions
inline bool HasFlag    (int  self, int flag)  { return (self & flag)  != 0; }
inline bool IsAmong    (int  self, int group) { return (self & group) != 0; }
inline void AddFlag    (int& self, int flag)  { self |= flag;               }
inline void RemoveFlag (int& self, int flag)  { self &= ~flag;              }

inline int GetBits(uint64_t cmd, int start, int end) {

    assert(start >= 0 && end >= start && end-start < 32 && end < 64);
    return (int)((cmd>>start) & (((uint64_t)1<<(end-start+1))-1));
}

inline int SetBits(int dest, int src, int start, int end) {

    assert(start >= 0 && end >= start && end < 32);
    int mask = (((uint64_t)1<<(end-start+1))-1) << start;
    return (dest & (~mask)) | ((src<<start) & mask);
}

inline int BCD44_TO_DEC(uint8_t bcd) { return GetBits(bcd,4,7) * 10 + GetBits(bcd,0,3); }
inline int BCD24_TO_DEC(uint8_t bcd) { return GetBits(bcd,4,5) * 10 + GetBits(bcd,0,3); }

// dvd-video domains (VI3-16)
enum DVDVideoDomain {

    FP_DOM     = 0x00000001,
    VMGM_DOM   = 0x00000002,
    VTSM_DOM   = 0x00000004,
    TT_DOM     = 0x00000008,
    STOP_STATE = 0x00010000
};

// dvd-video command area definitions
enum DVDVideoCmdArea {

    DVDV_AREA_PGC_PRE  = 0x00000001,
    DVDV_AREA_PGC_POST = 0x00000002,
    DVDV_AREA_CELL     = 0x00000004,
    DVDV_AREA_BUTTON   = 0x00000008,
    DVDV_AREA_USER     = 0x00010000
};

// dvd-video still modes
enum DVDVideoStillMode {

    DVDV_STILL_NONE      = 0x00000001,
    DVDV_STILL_PGC       = 0x00000002,
    DVDV_STILL_CELL      = 0x00000004,
    DVDV_STILL_VOBU      = 0x00000008,
    DVDV_WAIT_PGC_END    = 0x00010000,
    DVDV_WAIT_PG_END     = 0x00020000,
    DVDV_WAIT_CELL_END   = 0x00040000,
    DVDV_WAIT_CELL_BEGIN = 0x00080000,
    DVDV_WAIT_VOBU_END   = 0x00100000,
    DVDV_WAIT_POINT_B    = 0x00200000,
    DVDV_REQ_SetTmpPML   = 0x01000000
};

// dvd-video angle types
enum DVDVideoAngleType {

    DVDV_ANGLE_NONE        = 0x00000001,
    DVDV_ANGLE_SEAMLESS    = 0x00000002,
    DVDV_ANGLE_NONSEAMLESS = 0x00000004
};

// dvd-video aspect ratio types
enum DVDVideoRatioType {

    DVDV_RATIO_4_3  = 0x00000001,
    DVDV_RATIO_16_9 = 0x00000002
};

// dvd-video state event flags
enum DVDVideoStateEventFlags {

    DVDV_ReselectAudioSubpicture = 0x00000001,
    DVDV_SetVideoTarget          = 0x00000002,
    DVDV_SetVideoPresMode        = 0x00000004,
    DVDV_SetAudioTarget          = 0x00000008,
    DVDV_SetAudioMixMode         = 0x00000010,
    DVDV_SetSubpictureTarget     = 0x00000020,
    DVDV_SetSubpicturePalette    = 0x00000040,
    DVDV_SetAngle                = 0x00000080,
    DVDV_RestoreNormalSpeed      = 0x00000100,
    DVDV_UpdateCGMS              = 0x00000200
};

// dvd-video branch commands (VI4-195 ~ 220)
enum DVDVideoBranchCmdID {

    DVDV_NOP,
    DVDV_LinkPGCN,
    DVDV_LinkPTTN,
    DVDV_LinkPGN,
    DVDV_LinkCN,
    DVDV_LinkTopC,
    DVDV_LinkNextC,
    DVDV_LinkPrevC,
    DVDV_LinkTopPG,
    DVDV_LinkNextPG,
    DVDV_LinkPrevPG,
    DVDV_LinkTopPGC,
    DVDV_LinkNextPGC,
    DVDV_LinkPrevPGC,
    DVDV_LinkGoUpPGC,
    DVDV_LinkTailPGC,
    DVDV_RSM,
    DVDV_Exit,
    DVDV_JumpTT,
    DVDV_JumpVTS_TT,
    DVDV_JumpVTS_PTT,
    DVDV_JumpSS,
    DVDV_CallSS,
    DVDV_SetTmpPML,

    // proprietary commands
    DVDV_SkipDummyPGC,
    DVDV_BounceOffRestrictedCell
};

enum  DVDVideoUOPFlags {

    DVDV_UOP_TimePlayOrSearch    = 1 << 0,
    DVDV_UOP_PttPlayOrSearch     = 1 << 1,
    DVDV_UOP_TitlePlay           = 1 << 2,
    DVDV_UOP_Stop                = 1 << 3,
    DVDV_UOP_GoUp                = 1 << 4,
    DVDV_UOP_TimeOrPttSearch     = 1 << 5,
    DVDV_UOP_TopOrPrevPgSearch   = 1 << 6,
    DVDV_UOP_NextPgSearch        = 1 << 7,
    DVDV_UOP_ForwardScan         = 1 << 8,
    DVDV_UOP_BackwardScan        = 1 << 9,
    DVDV_UOP_TitleMenuCall       = 1 << 10,
    DVDV_UOP_RootMenuCall        = 1 << 11,
    DVDV_UOP_SpicMenuCall        = 1 << 12,
    DVDV_UOP_AudioMenuCall       = 1 << 13,
    DVDV_UOP_AngleMenuCall       = 1 << 14,
    DVDV_UOP_PttMenuCall         = 1 << 15,
    DVDV_UOP_Resume              = 1 << 16,
    DVDV_UOP_ButtonSelectOrAct   = 1 << 17,
    DVDV_UOP_StillOff            = 1 << 18,
    DVDV_UOP_PauseOn             = 1 << 19,
    DVDV_UOP_AudioStreamChange   = 1 << 20,
    DVDV_UOP_SpicStreamChange    = 1 << 21,
    DVDV_UOP_AngleChange         = 1 << 22,
    DVDV_UOP_AudioPresModeChange = 1 << 23,
    DVDV_UOP_VideoPresModeChange = 1 << 24
};

struct DVDVideoBranchCmd {

    DVDVideoBranchCmd() { id = DVDV_NOP; }

    DVDVideoBranchCmdID id;
    int operand;
    int line;
};

// dvd-video PG shuffler
typedef CShuffler<4> CDVDVideoPGShuffler;

// RSM information for resume-from-menu (VI3-19)
struct DVDVideoRSM {

    int                 pgcN;
    int                 pgN;
    int                 cellN;
    uint32_t            vobuStartAddress;
    uint16_t            ttN;
    uint16_t            btnN;
    int                 loopCount;
    int                 vobuStartFrameN; // used only when resuming into a different angle
    CDVDVideoPGShuffler shuffler;
};

// dvd-video state
struct DVDVideoState {

    uint16_t            SPRM[24];
    uint16_t            GPRM[16];
    int                 GPRMModes;
    DVDVideoDomain      domain;
    int                 vtsN;
    int                 pgcN;
    int                 pgN;
    int                 cellN;
    int                 loopCount;
    int                 parentalIDField;
    DVDVideoAngleType   angleType;
    bool                bDoVOBUStill;
    bool                bRestrictedCell;
    int                 pgcUOPFlags;
    NAV_VIDEO_PRES_MODE vpm;
    DVDVideoRatioType   ratio;
    int                 framesPerSecond;
    int                 ptsTickPerFrame;
    DVDVideoStillMode   stillMode;
    NAV_REPEAT_MODE     repeatMode;
    CDVDVideoPGShuffler shuffler;
    DVDVideoRSM         rsm;
    int                 effectiveASTN;
    int                 effectiveSPSTN;
    int                 events;
    int                 speed;
    int                 skip;

    // information used to calculate playback time and durations (seq: sequential playback area)
    int seqStartCellN;
    int seqEndCellN;
    int pttStartCellN;
    int pttEndCellN;
    int seqEndFrameN;
    int pttStartFrameN;
    int pttEndFrameN;
    int cellStartFrameN;

    inline uint16_t& M_LCD()            { return SPRM[0];  }
    inline uint16_t& ASTN()             { return SPRM[1];  }
    inline uint16_t& SPSTN()            { return SPRM[2];  }
    inline uint16_t& AGLN()             { return SPRM[3];  }
    inline uint16_t& TTN()              { return SPRM[4];  }
    inline uint16_t& VTS_TTN()          { return SPRM[5];  }
    inline uint16_t& TT_PGCN()          { return SPRM[6];  }
    inline uint16_t& PTTN()             { return SPRM[7];  }
    inline uint16_t& HL_BTNN()          { return SPRM[8];  }
    inline uint16_t& NV_TMR()           { return SPRM[9];  }
    inline uint16_t& NV_TMR_TT_PGCN()   { return SPRM[10]; }
    inline uint16_t& P_AMXMD()          { return SPRM[11]; }
    inline uint16_t& PTL_CTY_CD()       { return SPRM[12]; }
    inline uint16_t& PTL_LVL()          { return SPRM[13]; }
    inline uint16_t& P_CFG_VIDEO()      { return SPRM[14]; }
    inline uint16_t& P_CFG_AUDIO()      { return SPRM[15]; }
    inline uint16_t& AST_INI_LCD()      { return SPRM[16]; }
    inline uint16_t& AST_INI_LCD_EXT()  { return SPRM[17]; }
    inline uint16_t& SPST_INI_LCD()     { return SPRM[18]; }
    inline uint16_t& SPST_INI_LCD_EXT() { return SPRM[19]; }
    inline uint16_t& REGION()           { return SPRM[20]; }

    inline bool InStillModes() {
        return IsAmong(stillMode, DVDV_STILL_PGC|DVDV_STILL_CELL|DVDV_STILL_VOBU);
    }

    inline bool InWaitModes() {
        return IsAmong(stillMode, DVDV_WAIT_PGC_END|DVDV_WAIT_PG_END|DVDV_WAIT_CELL_END|DVDV_WAIT_CELL_BEGIN|DVDV_WAIT_VOBU_END|DVDV_WAIT_POINT_B);
    }
};

// dvd-video vobu info
struct DVDVideoVOBUInfo {

    uint16_t          pttN;
    int               pgN;
    int               cellN;
    uint32_t          vobuStartAddress;
    uint32_t          vobuEndAddress;
    int64_t           vobuStartPTS;
    int64_t           vobuEndPTS;
    int64_t           stcOffset;
    int               pttStartFrameN;
    int               pttEndFrameN;
    int               cellStartFrameN;
    int               vobuStartFrameN;
    DVDVideoAngleType angleType;
    bool              bDoVOBUStill;
    bool              bRestrictedCell;
    bool              bHasPCI;
    bool              bHasDSI;
    pci_t             pci;
    dsi_t             dsi;

    inline bool HasVideo() { // determine whether this VOBU has video
        // (note: endian conversion is skipped here since we only care about whether it is zero)
        return (dsi.dsi_gi.vobu_1stref_ea != 0);
    }

    inline bool HasStillVideo() { // determine whether this VOBU has still video
        return (dsi.dsi_gi.vobu_1stref_ea != 0 && dsi.dsi_gi.vobu_2ndref_ea == 0);
    }
};

#endif //__DVD_VIDEO_COMMON_H__
