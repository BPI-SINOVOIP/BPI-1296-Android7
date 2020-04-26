/**
@file
@verbatim

$Id: DivxMenuControllerTypes.h 2807 2006-03-10 00:29:08Z sbramwell $

Copyright (c) 2002-2006 DivX, Inc. All rights reserved.

This software is the confidential and proprietary information of DivX,
Inc. and may be used only in accordance with the terms of your license from
DivX, Inc.

@endverbatim
*/

#ifndef LMC_DIVXMENUCONTROLTYPE_H
#define LMC_DIVXMENUCONTROLTYPE_H

#include "portab.h"
#include "MemoryAlloc.h"
#include "MenuChunkTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// API Return Codes
#define DIVXMENUCONTROL_SUCCESS						0  /**< DIVXMENUCONTROL_SUCCESS Returned if a method executed successfully. */
#define DIVXMENUCONTROL_UNKNOWN_ERROR				1  /**< DIVXMENUCONTROL_UNKNOWN_ERROR Returned if a method fails for an unknown reason. */
#define DIVXMENUCONTROL_NO_LANGUAGE_MENU_ERROR		2  /**< DIVXMENUCONTROL_NO_LANGUAGE_MENU_ERROR Returned if no language menu is present in the menu. */
#define DIVXMENUCONTROL_INVALID_MENU_ERROR			3  /**< DIVXMENUCONTROL_INVALID_MENU_ERROR Returned if the menu is not formatted correctly. */
#define DIVXMENUCONTROL_NO_ACTIVE_BUTTON_MENU_ERROR	4  /**< DIVXMENUCONTROL_NO_ACTIVE_BUTTON_MENU_ERROR Returned if the menu is in a state where no button is active */
#define DIVXMENUCONTROL_NO_ACTION_NECESSARY			5  /**< DIVXMENUCONTROL_NO_ACTION_NECESSARY Returned if method does not require the caller to update the tracks that are playing */
#define DIVXMENUCONTROL_INVALID_STATE_ERROR			6  /**< DIVXMENUCONTROL_INVALID_STATE_ERROR Returned if the menu was not initialized or is invalid state */
#define DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR		7  /**< DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR Returned if a menu or track is selected that does not exist */
#define DIVXMENUCONTROL_MENU_VERSION_NOT_UNDERSTOOD 8  /**< DIVXMENUCONTROL_MENU_VERSION_NOT_UNDERSTOOD Returned if a menu version is newer than the DivXMenuControllerAPI */
#define DIVXMENUCONTROL_OUT_OF_MEMORY_ERROR         9  /**< DIVXMENUCONTROL_OUT_OF_MEMORY The state machine does not have enough memory to load the menu chunk */
#define DIVXMENUCONTROL_LANGUAGE_NOT_AVAILABLE		10 /**< DIVXMENUCONTROL_LANGUAGE_NOT_AVAILABLE The requested language is not available in the file */
#define DIVXMENUCONTROL_TITLE_NOT_AVAILABLE			11 /**< DIVXMENUCONTROL_TITLE_NOT_AVAILABLE The requested title is not available in the file */
#define DIVXMENUCONTROL_CHAPTER_NOT_AVAILABLE		12 /**< DIVXMENUCONTROL_CHAPTER_NOT_AVAILABLE The requested chapter is not available in the file */
#define DIVXMENUCONTROL_INVALID_TRACK				13 /**< DIVXMENUCONTROL_INVALID_TRACK The requested track is not available in the file */


#define DIVXMENUCONTROL_TRACK_ID_SIZE				5  /**< DIVXMENUCONTROL_TRACK_ID_SIZE The maximum track id size including the NULL terminator */

#define DIVXMENU_CONTROLLER_VERSION                 1  /**< DIVXMENU_CONTROLLER_VERSION The Latest Menu supported */

#define DIVXMENUCONTROL_TRACKTYPELENGTH				20 /**< DIVXMENUCONTROL_TRACKTYPELENGTH Maximum Length of track type string */
#define DIVXMENUCONTROL_MENUTYPELENGTH				20 /**< DIVXMENUCONTROL_MENUTYPELENGTH Maximum Length of menu type string */

/**
Defines a media track. 
*/
typedef struct DMCMediaTrackType {

	char				  trackId[DIVXMENUCONTROL_TRACK_ID_SIZE]; /**< trackId The Null terminated track Id string. */  
	int					  startFrame; /**< startFrame The first frame in the track to play */  
	int					  endFrame;	/**< endFrame The last frame in the track to play */  
	double				  startTime; /**< startTime Not currently used */  
	double				  endTime; /**< endTime Not currently used */  
	uint64_t			  riffOffset; /**< riffOffset The location of the Riff chunk in the file */  
	char allocated;

} DMCMediaTrackRecord;

typedef struct DMCMediaTrackListType {

    DMCMediaTrackRecord * mediaTrack; 
    struct DMCMediaTrackListType   * next;
	char allocated;

} DMCMediaTrackList;

typedef struct DMCNodeType {

	wchar_t				  *name; /**< name The name of the chapter, title, subtitle or audio track */  
	char				  charId[DIVXMENUCONTROL_TRACK_ID_SIZE]; /**< charId The Null terminated track Id string. */  
	int					  id;		/**< id The unique DivX Menu object identifier. */ 
	char allocated;

} DMCNodeRecord;

typedef struct DMCNodeListType {

    DMCNodeRecord *node;
    struct DMCNodeListType   * next;
	char allocated;

}DMCNodeList;

typedef enum DMCPlaybackTypeEnum {
	DMCPlayMainFeature,  /**< DMCPlayMainFeature indicates a feature is playing and that trick play should be enabled. */  
	DMCPlayMenu,		/**< DMCPlayMenu indicates a menu is playing and trick play should be disabled. */ 
	DMCPlayForceMenu,	/**< DMCPlayForceMenu indicates a menu is playing and trick play should be disabled, and menu button is disabled. Player must play unless stop button pressed */ 
	DMCReplayMenu,		/**< DMCReplayMenu indicates that the tracks have not changed and no re-initialization is required for playback. */ 
	DMCUpdateMenu,		/**< DMCUpdateMenu indicates a new track position is required for playback. This can occur when a button is activated. */ 
	DMCPlayMainFeatureFromCurrentOffset  /**< DMCPlayMainFeatureFromCurrentOffset indicates that playback should continue from the last location. The audio or subtitle track may have changed*/ 

} DMCPlaybackType;

typedef struct _DMCTranslationMemory {
	unsigned char *base;
	unsigned int size;
	unsigned int currentPosition;
} DMCTranslationMemory;

typedef enum DMCStateEnum {
    DMCPlayingMainFeaturePlayState,     // playing a feature movie
    DMCPlayingMenuState,        // playing a menu
    DMCPUnknownState            // doing strange unknown type things (or maybe just doing nothing) 
} DMCState;

enum ACTIONTYPE { SELECT, UP, DOWN, LEFT, RIGHT, EXIT };

// A media title track record
typedef struct DMCTitleTrackType {

    int titleID;                // the ID of the title?
    char audioTrackID[DIVXMENUCONTROL_TRACK_ID_SIZE];   // IDs for the audio tracks?
    char subtitleTrackID[DIVXMENUCONTROL_TRACK_ID_SIZE];        // IDs for the subtitle tracks?
} DMCTitleTrackRecord;

// A list of the above track records
typedef struct DMCTitleTrackListType {

    DMCTitleTrackRecord *titleTrack;    // the track (see above)
    struct DMCTitleTrackListType *next; // the next track
} DMCTitleTrackList;

#define MENUTAGLENGTH 3

typedef struct _DMCData {
	unsigned int currentMediaMenuID;
	int activeTitleID;
	DMCNodeRecord *RecordList;
	DMCNodeList *NodeList;
	DMCTitleTrackRecord *titleTrack;
	DMCMediaTrackRecord *mediaTrack;
	DMCTitleTrackList *titleTrackList;
	MenuObject *currentButtonMenu;
	MenuObject currentButtonObject;
	MenuObject startingMenuObject;
	DMCMediaTrackList *mediaTrackList;
	MenuObject *currentLanguageMenu;
	MenuObject currentLanguageMenuObject;
	MenuObject *currentMediaMenu;
	MenuObject mediaMenu;           
	MenuObject *mediaManager;
	MenuObject mediaManagerObject;
	MenuObject *media;
	MenuObject topLevelMediaObject;
	wchar_t menuLanguageCode[MENUTAGLENGTH];
	DMCState state;
	DMCTranslationMemory *dmcTranslationMemory;
	MemBuffer memPool;
	int nodeListAlloc;
	int nodeRecordAlloc;
	int trackListAlloc;
	int trackRecordListAlloc;


} DMCData;

/** @} */
#ifdef __cplusplus
}
#endif

#endif  //LMC_DIVXMENUCONTROLTYPE_H
