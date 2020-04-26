/**
@file
@verbatim

$Id: MenuChunkTypes.h 3092 2006-03-30 22:12:11Z medmond $

Copyright (c) 2005-2006 DivX, Inc. All rights reserved.

This software is the confidential and proprietary information of DivX,
Inc. and may be used only in accordance with the terms of your license from
DivX, Inc.

@endverbatim
*/

#ifndef LMC_MENUCHUNKTYPES_H
#define LMC_MENUCHUNKTYPES_H
#include "portab.h"
/** @file MenuChunkTypes.h */
/** @ingroup menu */

//wide string tag names for all the menu objects
extern wchar_t * DivXMediaManager;
extern wchar_t * LanguageMenus;
extern wchar_t * Media;
extern wchar_t * DivXMediaMenu;
extern wchar_t * MediaTrack;
extern wchar_t * AudioTrack;
extern wchar_t * ButtonMenu;
extern wchar_t * PlayAction;
extern wchar_t * PlayFromCurrentOffsetAction;
extern wchar_t * MenuTransitionAction;
extern wchar_t * ButtonTransitionAction;
extern wchar_t * MediaSource;
extern wchar_t * MenuRectangle;
extern wchar_t * Action;
extern wchar_t * SubtitleSelectAction;
extern wchar_t * AudioSelectAction;
extern wchar_t * Title;
extern wchar_t * Chapter;
extern wchar_t * TranslationTable;
extern wchar_t * TranslationLookup;
extern wchar_t * Translation;
extern wchar_t * MenuTracks;

//track type tag strings
extern wchar_t * VideoTrackType;
extern wchar_t * SubtitleTrackType;
extern wchar_t * AudioTrackType;

//menu type tag strings
extern wchar_t * ChapterMenuType;
extern wchar_t * TitleMenuType;
extern wchar_t * AudioMenuType;
extern wchar_t * SubtitleMenuType;
extern wchar_t * OtherMenuType;


/*!represents a menu object */
typedef struct MenuObject
{
	unsigned char *offset;
    // common data section
    /*!the name of this object type*/
    wchar_t*                m_name;
    /*!FourCC code of this object type*/
    char                    m_fourCC[5];
    /*!size of the object*/
    int                     m_size;
    /*!ID tag of this object*/
    int                     m_objectID;
    /*!address of parent object*/
    struct MenuObject*      m_parent;
    /*!head of children list*/
    struct MenuObjectList*  m_childList;
    /*!number of children*/
    unsigned int            m_childCount;

    /*!union representing object specific data sections*/
    union Types
    {
        /*!struct action - represents a generic user action*/
        struct ActionType
        {
            /*!reserved for future use*/
            int m_reserved;
        } Action;
        /*!represents a user select audio track*/
        struct AudioSelectActionType
        {
            /*!the ID of the media title*/
            int         m_titleID;
            /*!the ID of the titles' audio track*/
	        char*    m_trackID;
        } AudioSelectAction;
        /*!represents a button on a menu*/
        struct ButtonMenuType
        {
            /*!ID of the overlay for this button*/
            int         m_overlayID;
            /*!Action ID for user selecting this button*/
            int         m_selectAction;
            /*!the text associated with this button*/
            wchar_t*    m_text;
            /*!action ID for user pressing up on this button*/
            int         m_upAction;
            /*!action ID for user pressing down on this button*/
            int         m_downAction;
            /*!action ID for user pressing left on this button*/
            int         m_leftAction;
            /*!action ID for user pressing right on this button*/
            int         m_rightAction;
            /*!reserved for future use*/
	        int         m_reserved;
        } ButtonMenu;
        /*!Buttons changing, such as a graphic that changes when the user has selected*/
        struct ButtonTransitionActionType
        {
            /*!ID of a button to transition to*/
            int m_buttonMenuID;
        } ButtonTransitionAction;
        /*!represents a chapter in a movie title*/
        struct ChapterType
        {
            /*!language localization lookup ID*/
            int m_translationLookupID;
        } Chapter;
        /*!Top level object in menu*/
        struct DivxMediaManagerType
        {
            /*!offset of the start of MRIF data (menu media source data)*/
            uint64_t    m_offset;
            /*!ID of first menu to load*/
            int         m_startingMenuID;
            /*!the default language for playback*/
            wchar_t*    m_defaultLanguage;
            /*!Menu data version number*/
            wchar_t*    m_version;
            /*!Version of the menu object model*/
            int         m_MenuVersion;
        } DivxMediaManager;
        /*!Represents a specific menu screen*/
        struct DivxMediaMenuType
        {
            /*!the video playing in the background*/
            int m_backgroundVideoID;
            /*!the audio playing in the background*/
            int m_backgroundAudioID;
            /*!the button that should be initially selected*/
            int m_startingMenuItemID;
            /*!action to execute upon leaving this menu*/
            int m_exitAction;
            /*!action to execute upon entering this menu*/
	        int m_enterAction;
            /*!Name of this menu*/
	        wchar_t * m_menuName;
            /*!Type of menu(can be a root menu, title menu, or chapter menu)*/
	        wchar_t * m_menuType;
        } DivxMediaMenu;
        /*!Specifies a Language Local version of a menu*/
        struct LanguageMenusType
        {
            /*!the language of the menu*/
            wchar_t * m_languageCode;
            /*!the ID of the starting menu*/
            int m_startingMenuID;
            /*!the ID of the top level menu*/
            int m_rootMenuID;
        } LanguageMenus;
        /*!represents a single media in the file*/
        struct MediaType
        {
            /*!reserved for future use*/
            int m_reserved;
        } Media;
        /*!represents the location of data source for a media*/
        struct MediaSourceType
        {
            /*!location of source media*/
            wchar_t * m_location;
            /*!type of media that the source represents*/
            wchar_t * m_mediaType;
            /*!riff offset for the media*/
            uint64_t m_riffOffset;
            /*!size of the file*/
            uint64_t m_fileSize;
            /*!reserved for future use*/
            int m_reserved;
        } MediaSource;
        /*!represents a single track of media in the file*/
        struct MediaTrackType
        {
            /*!ID of the corresponding media source object*/
            int m_mediaSourceID;
            /*!the track ID code*/
            char * m_trackID;
            /*!source media track ID*/
            wchar_t * m_sourceTrackID;
            /*!starting frame of video or subtitle tracks*/
            int m_startFrame;
            /*!ending frame of video or subtitle tracks*/
            int m_endFrame;
            /*!starting time for audio tracks*/
            double m_startTime;
            /*!ending time for audio tracks*/
            double m_endTime;
            /*!the localization code for this track*/
	        wchar_t * m_languageCode;
            /*!the lookup for localization to other languages*/
	        int m_translationLookupID;
            /*!type tag- can be "audio", "video", or "subtitle"*/
	        wchar_t * m_type;
        } MediaTrack;
        /*!Bounding rectangle for action objects (mouse support only)*/
        struct MenuRectangleType
        {
            /*!top of rectangle*/
            int m_top;
            /*!left of rectangle*/
            int m_left;
            /*!width of rectangle*/
            int m_width;
            /*!height of rectangle*/
            int m_height;
        } MenuRectangle;
        /*!Tracks for menu data*/
        struct MenuTracksType
        {
            int m_unused;
        } MenuTracks;
        /*!Represents a transition between menus*/
        struct MenuTransitionActionType
        {
            /*!The menu to go to*/
            int m_menuID;
        } MenuTransitionAction;
        /*!Playing back a selected media clip*/
        struct PlayActionType
        {
            /*!the media ID of the media to playback*/
            int m_mediaObjectID;
            /*!the title to playback*/
	        int m_titleID;
        } PlayAction;
        /*!Continue a clip already in progress*/
        struct PlayFromCurrentOffsetActionType
        {
            /*!the media ID of the media to playback*/
            int m_mediaObjectID;
            /*!the title to playback*/
	        int m_titleID;
        } PlayFromCurrentOffsetAction;
        /*!User is selecting a subtitle track*/
        struct SubtitleSelectActionType
        {
            /*!ID of the playing title*/
            int m_titleID;
            /*!ID of the subtitle track*/
            char* m_trackID;
        } SubtitleSelectAction;
        /*!represents a title in the file (can have multiple titles)*/
        struct TitleType
        {
            /*!translation lookup for this title*/
            int m_translationLookupID;
            /*!the default audio track to play*/
	        wchar_t * m_defaultAudioTrack;
            /*!the default subtitle track*/
	        wchar_t * m_defaultSubtitleTrack;
        } Title;
        /*!a translation string*/
        struct TranslationType
        {
            /*!the language in use*/
            wchar_t * m_languageCode;
            /*!the localized string*/
	        wchar_t * m_value;
        } Translation;
        /*!Lookup for language Localization*/
        struct TranslationLookupType
        {
            int m_unused;
        } TranslationLookup;
        /*!Table for language lookups*/
        struct TranslationTableType
        {
            int m_unused;
        } TranslationTable;
    }Type;
} MenuObject; 

/*!Menu Object List Node structure*/
typedef struct MenuObjectList
{
    /*!pointer to the actual object*/
    MenuObject* node;
    /*!pointer to the next list object*/
    struct MenuObjectList* next;
}MenuObjectList;


#ifdef __cplusplus
extern "C" {
#endif


/*!
@brief Converts from Unicode to ASCII

@param[in] w -  address of unicode string
@param[in,out] c - address of empty buffer for ASCII string
*/
void wcharToChar(wchar_t * w, char* c);

/*!
@brief Converts from ASCII to Unicode

@param[in,out] w - address of unicode string
@param[in] c - address of empty buffer for ASCII string
*/
void charToWchar(char * c, wchar_t* w);


/*!
@brief Returns the version number of a DMNU chunk

@param[in] menuChunk - DMNU chunk pointer
@param[in] menuChunkSize - size of DMNU chunk
@param[in,out] MenuVersion - Address of integer write with version number

@return integer error code
*/
int getMenuVersion(char *menuChunk, int menuChunkSize, int *MenuVersion);

MenuObject* getMenu(unsigned char* chunk, int *chunksize, int* size, unsigned char* error,MenuObject* Menu);
void copyText(wchar_t *dest, unsigned char *data);
MenuObject* findChildById(unsigned char* chunk,int id, MenuObject* Menu);
MenuObject* findChildrenByName(unsigned char* chunk,unsigned int *search_offset,wchar_t* TagName, unsigned char *error, MenuObject* Menu);
MenuObject* findAllChildrenByName(unsigned char* chunk,unsigned int *search_offset,wchar_t* TagName, unsigned char *error, MenuObject* Menu);
unsigned char* findOffsetById(unsigned char *chunk,int id);
MenuObject* findChild(unsigned char *chunk, unsigned int *search_offset, unsigned char *error, MenuObject* menuObject);
size_t sizeText(unsigned char *chunk);
void copyMenuToChar(unsigned char *data,char *dest, int maxSize);
void copyMenuToWChar(unsigned char *data, wchar_t *dest, int maxSize);

#ifdef __cplusplus
}
#endif


#endif //LMC_MENUCHUNKYTPES_H
