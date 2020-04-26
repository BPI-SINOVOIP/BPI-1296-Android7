#ifndef __C__DVD_DEMUX_H__
#define __C__DVD_DEMUX_H__

#include <OSAL.h>
#include <CStream.h>
#include <Filters/INavControl.h>
#include "CDVDSeek.h"

// #define DVD_DEMUX_PRIVATE_PTS                                   1
#define DVD_DEMUX_PRIVATE_PREVIEW                               1

//////////////////////////////////////////////////////////////////////////
// Export data structure                                                //
//////////////////////////////////////////////////////////////////////////

// for flow manager
#if 0
typedef enum{
    UOP_DVD_MENU_JUMP_E         = 0,
    UOP_DVD_PG_E                = 1,
    UOP_DVD_CHAPTER_E           = 2,
    UOP_DVD_TITLE_E             = 3,
    UOP_DVD_ANGLE_E             = 4,
    UOP_DVD_BUTTON_MOVE_E       = 5,
    UOP_DVD_BUTTON_PRESS_E      = 6,
    UOP_DVD_BUTTON_NUMBER_E     = 7
} UOP_DVD_E;

typedef struct{
    UOP_DVD_E                   UOP;
    BYTE*                       pData;
} UOP_Data_T;

typedef enum{
    UOP_PREV_E                  = 0,
    UOP_NEXT_E                  = 1
} UOP_PLAY_DIR_E;

typedef enum{
    UOP_BUTTON_UPPER_E          = 0,
    UOP_BUTTON_LOWER_E          = 1,
    UOP_BUTTON_RIGHT_E          = 2,
    UOP_BUTTON_LEFT_E           = 3
} UOP_BUTTON_DIR_E;

typedef enum{
    DEMUX_DVD_BUTTON_SELECT     = 0,
    DEMUX_DVD_BUTTON_ACTION     = 1
} DEMUX_DVD_BUTTON_OP_E;
#endif

#define UOP_BUTTON_DIR          unsigned char
#define UOP_BUTTON_UPPER        ((unsigned char)0)
#define UOP_BUTTON_LOWER        ((unsigned char)1)
#define UOP_BUTTON_RIGHT        ((unsigned char)2)
#define UOP_BUTTON_LEFT         ((unsigned char)3)

#define UOP_BUTTON_WRAP         unsigned char
#define UOP_BUTTON_WRAP_YES     ((unsigned char)0)
#define UOP_BUTTON_WRAP_NO      ((unsigned char)1)

typedef enum{
    UOP_PREV_E                  = 0,
    UOP_NEXT_E                  = 1
} UOP_PLAY_DIR_E;

typedef struct{
    UOP_BUTTON_DIR              ButtonDir;
    UOP_BUTTON_WRAP             ButtonWrap;
} UOP_BUTTON_MOVE_T;

typedef union{
    DVDSeekMenuID_E             MenuId;
    UOP_PLAY_DIR_E              PlayDir;
    UOP_BUTTON_MOVE_T           ButtonMove;
    int                         NumberPress;
} UOP_User_Data_T;

typedef enum{
    UOP_DVD_MENU_JUMP_E         = 0,
    UOP_DVD_PG_E                = 1,
    UOP_DVD_CHAPTER_E           = 2,
    UOP_DVD_TITLE_E             = 3,
    UOP_DVD_ANGLE_E             = 4,
    UOP_DVD_BUTTON_MOVE_E       = 5,
    UOP_DVD_BUTTON_PRESS_E      = 6,
    UOP_DVD_BUTTON_NUMBER_E     = 7
} UOP_DVD_E;

typedef struct{
    UOP_DVD_E                   UOP_Cmd;
    UOP_User_Data_T             UOP_Data;
} UOP_Data_T;

// for audio decoder
typedef enum{
    AUDIO_FMT_A52               = 0x0000,
    AUDIO_FMT_MPEG              = 0x0001,
    AUDIO_FMT_LPCM_BE           = 0x0002,
    AUDIO_FMT_LPCM_LE           = 0x0003,
    AUDIO_FMT_WMAV1             = 0x0004,
    AUDIO_FMT_DTS               = 0x0005,

    AUDIO_FMT_UNKNOWN           = 0x00ff
} AUDIO_FORMAT_E;

// for spu decoder data structure
typedef enum{
    SPU_PALLETE_E               = 0,
    SPU_HIGHLIGHT_E             = 1
} SPU_PrivateInfoCmd_E;

typedef enum{
    DEMUX_DVD_BUTTON_SELECT     = 0,
    DEMUX_DVD_BUTTON_ACTION     = 1
} DEMUX_DVD_BUTTON_OP_E;

typedef struct{
    DEMUX_DVD_BUTTON_OP_E       ButtonOP;
    int                         ButtonId;
    // hli_t*                      pHLI;
    void*                       pHLI;
} SPU_HLI_Info_T;

// forward declaration
class   CDVDParseNavPack;
class   CDVDDemuxVideoOutPin;
class   CDVDDemuxAudioOutPin;
class   CDVDDemuxSPUOutPin;

typedef ulCircularQueue<UOP_Data_T>             DVD_UOP_QUEUE;
// ulCircularQueue<UOP_T>
//
class CDVDDemux: public CBaseFilter,
                 public IFilterSeeking, // for seeking interface
                 public INavControl // navigator control interface
{
public:
                                CDVDDemux(
                                    char*                   pDVDPath= "/dev/dvd"
                                );
                                ~CDVDDemux(void);

    HRESULT                     Stop(void);
    HRESULT                     Pause(
                                    FILTER_STATE            State
                                );
    HRESULT                     Run(void);

    HRESULT                     CheckCapabilities(
                                    FILTER_SEEKING_CAP*     pSeekingCap
                                );
    HRESULT                     SetSeekingInfo(
                                    long                    Speed,
                                    long                    Skip
                                );
    HRESULT                     GetSeekingInfo(
                                    long*                   pSpeed,
                                    long*                   pSkip
                                );

    HRESULT                     PushUOP(
                                    UOP_Data_T&             UOPData,
                                    long                    MilliTimeOut
                                );
    HRESULT                     FilterType(
                                    FILTER_TYPE*            pFilterType
                                );
protected:

    // thread
    static void*                DemuxThreadEntry(
                                    void*                   pThis
                                );
    bool                        DemuxThread(void);

    // for control cmd for downstream filters
    bool                        HandleControlPack(
                                    void*                   pPack
                                );

    bool                        HandleSPUBlock(
                                    void*                   pPack,
                                    int64_t*                pPTS,
                                    CDVDDemuxSPUOutPin*     pSPUOutPin,
                                    IMemRingAllocator*      pSPUAllocator,
                                    BYTE*                   pBufferLower,
                                    BYTE*                   pBufferUpper
                                );
    bool                        HandleVideoBlock(
                                    void*                   pPack,
                                    int64_t*                pPTS,
                                    CDVDDemuxVideoOutPin*   pVideoOutPin,
                                    IMemRingAllocator*      pVideoAllocator,
                                    BYTE*                   pBufferLower,
                                    BYTE*                   pBufferUpper
                                );
    bool                        HandleAudioBlock(
                                    void*                   pPack,
                                    int64_t*                pPTS,
                                    CDVDDemuxAudioOutPin*   pAudioOutPin,
                                    IMemRingAllocator*      pAudioAllocator,
                                    BYTE*                   pBufferLower,
                                    BYTE*                   pBufferUpper
                                );

    // SPU decoder
    bool                        DeliverDefaultButton(void);
    // UOP
    bool                        HandleUOP(void);
    bool                        HandleUOP_PG(
                                    UOP_PLAY_DIR_E          PlayDir
                                );
    bool                        HandleUOP_Chapter(
                                    UOP_PLAY_DIR_E          PlayDir
                                );
    bool                        HandleUOP_Title(
                                    UOP_PLAY_DIR_E          PlayDir
                                );
    bool                        HandleUOP_Angle(
                                    UOP_PLAY_DIR_E          PlayDir
                                );
    bool                        HandleUOP_ButtonMove(
                                    UOP_BUTTON_DIR          ButtonDir,
                                    UOP_BUTTON_WRAP         ButtonWrap
                                );
    bool                        HandleUOP_ButtonPress(void);
    bool                        HandleUOP_ButtonNumber(
                                    int                     ButtonNumber
                                );
protected:
    CDVDParseNavPack*           m_pDVDParseNavPack;
    CThread*                    m_pThread;
    char*                       m_pDVDPath;

    CDVDDemuxVideoOutPin*       m_pVideoOutPin;
    CDVDDemuxAudioOutPin*       m_pAudioOutPin;
    CDVDDemuxSPUOutPin*         m_pSPUOutPin;

    AUDIO_FORMAT_E              m_AudioFmt;

    DVD_UOP_QUEUE               m_UOPQueue;
    long                        m_Speed;
    long                        m_Skip;

    CDVDSeek                    m_seek; //@FIXME: temporary solution.

public:
    //
    // INavControl interface
    //
    virtual HRESULT LoadMedia(unsigned char* path);
    virtual HRESULT UnloadMedia();
    virtual HRESULT SetNavState(void* state, unsigned int size);
    virtual HRESULT SetConfiguration(unsigned int configID, void* configData,
                                     unsigned int size);
    virtual HRESULT PlayTitle(unsigned int titleNum, unsigned int cmdID);
    virtual HRESULT PlayChapter(unsigned int titleNum, unsigned int chapterNum,
                                unsigned int cmdID);
    virtual HRESULT PlayNextChapter(unsigned int cmdID);
    virtual HRESULT PlayPrevChapter(unsigned int cmdID);
    virtual HRESULT PlayAtTime(unsigned int titleNum, unsigned int eltmSeconds,
                               unsigned int frameIdx, unsigned int cmdID);
    virtual HRESULT PlaySegment(unsigned int titleNum,
                                unsigned int startSeconds,
                                unsigned int startFrameIdx,
                                unsigned int endSeconds,
                                unsigned int endFrameIdx,
                                unsigned int cmdID);
    virtual HRESULT MenuShow(unsigned int menuID, unsigned int cmdID);
    virtual HRESULT MenuEscape(unsigned int cmdID);
    virtual HRESULT ButtonSelectNumeric(unsigned int buttonNum,
                                        unsigned int cmdID);
    virtual HRESULT ButtonSelectPoint(unsigned int x, unsigned int y,
                                      unsigned int cmdID);
    virtual HRESULT ButtonActivateNumeric(unsigned int buttonNum,
                                          unsigned int cmdID);
    virtual HRESULT ButtonActivatePoint(unsigned int x, unsigned int y,
                                        unsigned int cmdID);
    virtual HRESULT ButtonActivate(unsigned int cmdID);
    virtual HRESULT ButtonMoveSelection(NAV_BTNDIR_ID direction,unsigned int cmdID);
    virtual HRESULT GoUp(unsigned int cmdID);
    virtual HRESULT StillEscape(unsigned int cmdID);
    virtual HRESULT SetVideoPresentationMode(unsigned int mode,
                                             unsigned int cmdID);
    virtual HRESULT SetAudioStream(unsigned int streamNum, unsigned int cmdID);
    virtual HRESULT SetAudioDownmixMode(unsigned int mode, unsigned int cmdID);
    virtual HRESULT SetSubpictureStream(unsigned int streamNum,
                                        unsigned int dsplay,
                                        unsigned int cmdID);
    virtual HRESULT SetAngle(unsigned int angleNum, unsigned int cmdID);
};

#endif

