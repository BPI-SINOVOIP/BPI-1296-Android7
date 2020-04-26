#ifndef __C_DVD_SEEK_H__
#define __C_DVD_SEEK_H__

#include <OSAL.h>

// Un-including

// a. un-implement yet
// (1) dvdnav_part_play_auto_stop
// (2) dvdnav_time_play
// (3) dvdnav_get_position_in_title
// (4) dvdnav_time_search

// b. don't understand: skip it first
// (1) dvdnav_sector_search
// (2) dvdnav_get_video_scale_permission

// c. not a seeking interface. hide it internally
// (1) dvdnav_get_position
// (2) dvdnav_get_current_nav_pci
// (3) dvdnav_get_highlight_area
// (4) dvdnav_button_activate_cmd
// (5) dvdnav_mouse_select
// (6) dvdnav_mouse_activate
// (7) dvdnav_audio_stream_to_lang
// (8) dvdnav_spu_stream_to_lang
// dvdnav_get_active_audio_stream
// dvdnav_get_active_spu_stream

// problem
// (1) dvdnav_get_video_aspect return type is uint8_t while -1 denotes error
// (2) dvdnav_audio_stream_to_lang return type is uint8_t while -1 denotes error
// (3) dvdnav_is_domain_fp return type is uint8_t while -1 denotes error
// (4) dvdnav_is_domain_vmgm return type is uint8_t while -1 denotes error
// (5) dvdnav_is_domain_vtsm return type is uint8_t while -1 denotes error
// (6) dvdnav_is_domain_vts return type is uint8_t while -1 denotes error

typedef enum {
  /* When used in VTS domain, DVD_MENU_Escape behaves like DVD_MENU_Root,
   * but from within a menu domain, DVD_MENU_Escape resumes playback. */
    DVD_SEEK_MENU_Escape_E      = 0,
    DVD_SEEK_MENU_Title_E       = 2,
    DVD_SEEK_MENU_Root_E        = 3,
    DVD_SEEK_MENU_Subpicture_E  = 4,
    DVD_SEEK_MENU_Audio_E       = 5,
    DVD_SEEK_MENU_Angle_E       = 6,
    DVD_SEEK_MENU_Part_E        = 7
} DVDSeekMenuID_E;

typedef enum{
    DVD_ASPECT_4_3_E            = 0,
    DVD_ASPECT_16_9_E           = 1
} DVDAspect_E;

typedef struct dvdnav_s                 dvdnav_t;
// #include "nav_types.h"
// typedef struct hli_t  __attribute__ ((packed)) hli_t;


class CDVDSeek{
public:

    CDVDSeek(void);
    ~CDVDSeek(void);

    bool                        Stop(void);
    // bool                        Pause(void);


    // play function
    ///////////////////////////////////////////////////////////////////////
    bool                        PlayTitle(
                                    int                         TitleId
                                );
    bool                        PlayChapter(
                                    int                         TitleId,
                                    int                         ChapterId
                                );
    bool                        PlayChapter(
                                    int                         ChapterId
                                );

    bool                        PlayGoUpPGC(void);


    bool                        PlayPrevPG(void);
    bool                        PlayFirstPG(void);
    bool                        PlayNextPG(void);


    bool                        AngleChange(
                                    int32_t                     Angle
                                );

    // Jump ????
    //////////////////////////////////////////////////////////////////////
    bool                        MenuJump(
                                    DVDSeekMenuID_E             MenuId
                                );

    // Select & Activate
    bool                        ButtonSelectUpper(void);
    bool                        ButtonSelectLower(void);
    bool                        ButtonSelectRight(void);
    bool                        ButtonSelectLeft(void);

    bool                        ButtonActivate(
                                    int                         ButtonId
                                );

    bool                        ButtonActivateHighLight(void);
    bool                        ButtonHighLight(
                                    int&                        ButtonId
                                );
    // information
    //////////////////////////////////////////////////////////////////////
    bool                        DVDName(
                                    char*&                      pDVDName
                                );

    bool                        nTitles(
                                    int&                        numTitle
                                );

    bool                        nChapter(
                                    int&                        numChapter,
                                    int                         TitleId
                                );

    bool                        Chapter(
                                    int&                        TitleId,
                                    int&                        ChapterId
                                );

    bool                        AspectRatio(
                                    DVDAspect_E&                Aspect
                                );

    bool                        AngleInfo(
                                    int32_t&                    CurrentAngle,
                                    int32_t&                    nAngle
                                );

    /////////////////////////////////////////////////////////////////////

    bool                        SetLanguageMenu(
                                    char*                       LanguageCode
                                );
    bool                        SetLanguageAudio(
                                    char*                       LanguageCode
                                );
    bool                        SetLanguageSPU(
                                    char*                       LanguageCode
                                );
    

    bool                        GetLanguageAudio(
                                    int8_t&                     AudioId
                                );

    bool                        GetLanguageSPU(
                                    int8_t&                     SPUId
                                );

/*
    bool                        GetLanguageAudio(
                                    uint16_t&               LanguageCode,
				    uint8_t                 AudioStream
                                );
*/

    /////////////////////////////////////////////////////////////////////
    bool                        IsDomainFP(
                                    bool&                       bFP
                                );
    bool                        IsDomainVMGM(
                                    bool&                       bVMGM
                                );
    bool                        IsDomainVTSM(
                                    bool&                       bVTSM
                                );
    bool                        IsDomainVTS(
                                    bool&                       bVTS
                                );

    // hli_t*                      CurrentHLI(void);
    void*                       CurrentHLI(void);

    bool                        SetDVDNav(
                                    dvdnav_t*                   pDVDNav
                                );
protected:
    dvdnav_t*                   m_pDVDNav;
};

#endif
