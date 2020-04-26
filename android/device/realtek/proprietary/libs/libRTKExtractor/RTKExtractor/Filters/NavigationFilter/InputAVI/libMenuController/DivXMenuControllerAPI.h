/**
@file
@verbatim

$Id: DivXMenuControllerAPI.h 3259 2006-04-05 22:41:12Z bmacdonald $

Copyright (c) 2005-2006 DivX, Inc. All rights reserved.

This software is the confidential and proprietary information of DivX,
Inc. and may be used only in accordance with the terms of your license from
DivX, Inc.

@endverbatim
*/

/** @defgroup menu libMenuController API
 *  Description of menucontroller
 *  @{
 */

#ifndef LMC_DIVXMENUCONTROLAPI_H
#define LMC_DIVXMENUCONTROLAPI_H

#include "MemoryAlloc.h"
#include "MenuChunkTypes.h"
#include "DivxMenuControllerTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
Initializes the DivXMenuControlerAPI. 
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList(). 

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  menuChunk - A pointer to memory containing the menuChunk data.
@param[in]  menuChunkSize - The size of the menuChunk.
@param[out] initialPlayList - A list of tracks that define the start of the menu.
@param[out] playbackType - Indicates the type of playback.
@param[in]  menuLanguage - The controller will select a menu in the desired language if one exists.  If the desired language menu does not exist, the controller defaults to English.  If the English menu does not exist, the first language menu is selected.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_LANGUAGE_MENU_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR, DIVXMENUCONTROL_MENU_VERSION_NOT_UNDERSTOOD.
*/
int dmcInit(DMCData *dmcData, unsigned char *menuChunk, int menuChunkSize, DMCMediaTrackList ** initialPlayList, DMCPlaybackType * playbackType, char * menuLanguage);

/**
Returns the version of the DivXMenuControllerAPI.  

@return int The verstion of the DivXMenuControllerAPI
*/
int dmcGetControllerVersion();

/**
Determines the version of the menu.  A menu can be played by the controller if the menu version is less than or equal to the controller version.

@param[in]  menuChunk - A pointer to memory containing the first 28 bytes of the menu chunk.
@param[in]  menuChunkSize - The size of the menuChunk.
@param[out] outMenuVersion - A pointer to memory to store the menu version.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INAVLID_MENU_ERROR
*/
int dmcGetMenuVersion(unsigned char *menuChunk, int menuChunkSize,int *outMenuVersion);

/**
Sets a memory pool for the state machine

@param[out] dmcData - A pointer to be filled with the address of instance data for this movie
@param[in]  Pool - Pointer to the head of the memory pool - NULL is a magic value, which indicates that the OS heap should be used
@param[in]  Size - Size of the memory pool - ignored if Pool is NULL
@param[in]  Alignment - Byte alignment of memory allocation - ignored if Pool is NULL
*/

void dmcSetMemoryPool(DMCData **dmcData, unsigned char* Pool, unsigned int Size, unsigned int Alignment);

/**
Specifies the desired audio track language. If multiple audio tracks exist, the controller will return the track in the desired language if it exists.  If the desired language does not exist the English track is returned.  If the English track does not exist, the first audio track is selected.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  languageCode - The default null terminated ISO 639 two-letter language code for audio tracks.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSetDefaultAudioLanguage(DMCData *dmcData, char * languageCode);

/**
Specifies the desired subtitle track language. If multiple subtitle tracks exist, the controller will return the track in the desired language if it exists.  If the desired language does not exist the English track is returned.  If the English track does not exist, the first subtitle track is selected.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  languageCode - The default null terminated ISO 639 two-letter language code for subtitle tracks.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSetDefaultSubtitleLanguage(DMCData *dmcData, char * languageCode);

/**
Closes and deallocates memory associated with the DivXMenuController session.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@return void 
*/
void dmcClose(DMCData *dmcData);

/**
Deallocates the memory for the DMCMediaTrackList.  The calling application is responsible for deallocating returned DMCMediaTrackList pointers.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  trackList - The track list to deallocate.
@return void 
*/
void dmcDeallocTrackList(DMCData *dmcData, DMCMediaTrackList * trackList);

/**
Deallocates the memory for the DMCNodeList.  The calling application is responsible for deallocating returned DMCNodeList pointers.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  nodeList - The node list to deallocate.
@return void 
*/
void dmcDeallocNodeList(DMCData *dmcData, DMCNodeList * nodeList);

/**
Informs the DivXMenuController that the caller has finished playing the current menu or main feature.  In response, the controller informs the caller which tracks to play next.  
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the next menu or main feature.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_UNKNOWN_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcMenuFinished(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Selects the active menu button and returns a new playList.  If no button is active or the button does not have a select action, DIVXMENUCONTROL_NO_ACTION_NECESSARY is returned.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the next menu or main feature.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSelectAction(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Responds to a user initiated up action.  If no button is active or the button does not have an up action,DIVXMENUCONTROL_NO_ACTION_NECESSARY is returned.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the next menu or main feature.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcUpAction(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Responds to a user initiated down action.  If no button is active or the button does not have a down action, DIVXMENUCONTROL_NO_ACTION_NECESSARY is returned.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the next menu or main feature.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcDownAction(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Responds to a user initiated left action.  If no button is active or the button does not have a left action, DIVXMENUCONTROL_NO_ACTION_NECESSARY is returned.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the next menu or main feature.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcLeftAction(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Responds to a user initiated right action.  If no button is active or the button does not have a right action, DIVXMENUCONTROL_NO_ACTION_NECESSARY is returned.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the next menu or main feature.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcRightAction(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Returns the title that is currently active.  

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] titleRecord - The description of the active title.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetActiveTitle(DMCData *dmcData, DMCNodeRecord * titleRecord);

/**
Returns a list of  titles.
This method returns a pointer to a DMCNodeList. This list must be deallocated with dmcDeallocNodeList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] trackList - A list of title descriptions.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetTitles(DMCData *dmcData, DMCNodeList ** trackList);

/**
Returns a list of chapters contained in the title.
This method returns a pointer to a DMCNodeList. This list must be deallocated with dmcDeallocNodeList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] trackList - A list of chapter descriptions.
@param[in] titleID - The unique title identifier.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetChapters(DMCData *dmcData, DMCNodeList ** trackList, int titleID);

/**
Selects a title or chapter for playback.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  mediaObjectID - Id of media object
@param[out] playList - A list of tracks that define the title or chapter.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcPlayMediaTrack(DMCData *dmcData, int mediaObjectID, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
Returns a list of audio tracks contained in the title.
This method returns a pointer to a DMCNodeList. This list must be deallocated with dmcDeallocNodeList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] trackList - A list of audio track descriptions.
@param[in]  titleID - The unique title identifier.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INAVLID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetAudioTracks(DMCData *dmcData, DMCNodeList ** trackList, int titleID);

/**
Returns a list of subtitle tracks contained in the title.
This method returns a pointer to a DMCNodeList. This list must be deallocated with dmcDeallocNodeList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] trackList - A list of subtitle track descriptions.
@param[in]  titleID - The unique title identifier.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetSubtitleTracks(DMCData *dmcData, DMCNodeList ** trackList, int titleID);

/**
Informs the controller that a new audio track has been selected for playback.  The caller must inform the controller 
of any external audio track switching so it maintains the proper state.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in] titleID - The unique title identifier.
@param[in] objectID - The unique audio track identifier.. This is internal to the menu object model
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSelectAudioTrack(DMCData *dmcData, int titleID, int objectID);

/**
Informs the controller that a new audio track has been selected for playback.  The caller must inform the controller 
of any external audio track switching so it maintains the proper state.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  titleID - The unique title identifier.
@param[in]  trackID - The audio track identifier.Ex: For audio track '02wb', the trackID is 02
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSelectAudioTrackID(DMCData *dmcData, int titleID, int trackID);

/**
Informs the controller that a new subtitle track has been selected for playback.  The caller must inform the controller 
of any external subtitle track switching so it maintains the proper state.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  titleID - The unique title identifier.
@param[in]  objectID - The unique subtitle track identifier. This is internal to the menu object model.objectID= -1 for turning off subtitles.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSelectSubtitleTrack(DMCData *dmcData, int titleID, int objectID);

/**
Informs the controller that a new subtitle track has been selected for playback.  The caller must inform the controller 
of any external subtitle track switching so it maintains the proper state.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  titleID - The unique title identifier.
@param[in]  trackID - The unique subtitle track identifier.Ex: For subtitle track '03sb', the trackID is 03. trackID= -1 for turning off subtitles
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSelectSubtitleTrackID(DMCData *dmcData, int titleID, int trackID);


/**
Returns the ID for the latest active menu.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] menuID - The output menu id.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetCurrentMenu(DMCData *dmcData, int *menuID);

/**
Returns the ID for the title menu.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] menuID - The output menu id.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetTitleMenu(DMCData *dmcData, int *menuID);

/**
Returns the ID for the root menu.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] menuID - The output menu id.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcGetRootMenu(DMCData *dmcData, int *menuID);

/**
Returns the ID for the Audio menu of a title.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool()
@param[in]  menuID - title number (range: 0-?)
@return Id of audio menu for that title
*/
int dmcGetAudioMenu(DMCData* dmcData, int *menuID);

/**
Returns the ID for the Subtitle menu of a title.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool()
@param[in]  menuID - title number (range: 0-?)
@return Id of subtitle menu for that title
*/
int dmcGetSubtitleMenu(DMCData* dmcData, int *menuID);

/**
Returns the ID for the Chapter menu of a title.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  menuID - title number (range: 0-?)
@return Id of chapter menu for that title
*/
int dmcGetChapterMenu(DMCData* dmcData, int *menuID);

/**
Selects a menu for playback.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[out] playList - A list of tracks that define the menu.
@param[out] playbackType - Indicates the type of playback.
@param[in]  menuID - The menu to play.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_MENU_DOESNT_EXIST_ERROR, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcSelectMenu(DMCData *dmcData, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType, int menuID);

/**
As a mouse moves over a menu, it can cause buttons to be activated.  A mouse over event will either return a new playlist or DIVXMENUCONTROL_NO_ACTION_NECESSARY.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  xPos - The location of the mouse on the x axis.  The upper left corner is 0 and all x positions are positive.
@param[in]  yPos - The location of the mouse on the y axis.  The upper left corner is 0 and all y positions are positive.
@param[out] playList - A list of tracks that define the menu.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcMouseOver(DMCData *dmcData, int xPos, int yPos, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/**
If a mouse click occurs over a menu button, a new playlist is returned.  If the mouse is not over a button, DIVXMENUCONTROL_NO_ACTION_NECESSARY is returned.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool().
@param[in]  xPos - The location of the mouse on the x axis.  The upper left corner is 0 and all x positions are positive.
@param[in]  yPos - The location of the mouse on the y axis.  The upper left corner is 0 and all y positions are positive.
@param[out] playList - A list of tracks that define the menu.
@param[out] playbackType - Indicates the type of playback.
@return int The return code; DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_NO_ACTION_NECESSARY, DIVXMENUCONTROL_INVALID_MENU_ERROR, DIVXMENUCONTROL_INVALID_STATE_ERROR.
*/
int dmcMouseClick(DMCData *dmcData, int xPos, int yPos, DMCMediaTrackList ** playList, DMCPlaybackType * playbackType);

/** 
Get the title object ID for a title number.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool()
@param[in]  titleNo - title number 
@param[out] titleID - pointer to the title object ID corresponding to the the title number

@return int The return code: DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_TITLE_NOT_AVAILABLE
*/
int dmcGetTitleID(DMCData *dmcData, int titleNo,int *titleID);

/** 
Get the chapter object ID for a chapter number for the specified title ID.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool()
@param[in]  titleID - title ID for the chapter
@param[in]  chapterNo - chapter number
@param[out] chapterID - pointer to the chapter object ID corresponding to the the chapter and title

@return int The return code: DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_CHAPTER_NOT_AVAILABLE
*/
int dmcGetChapterID(DMCData *dmcData, int titleID,int chapterNo,int *chapterID); 

/** 
Get the chapter number for the specified title ID for the video frame specified.

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool()
@param[in]  titleID - title ID for the chapter
@param[in]  currentVideoFrame - video frame of the required chapter (generally the current video frame playing)
@param[out] chapter - pointer to the chapter number of the specified video frame

@return int The return code: DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INVALID_MENU_ERROR, and other valid API return code
*/
int dmcGetCurrentChapterNumber(DMCData *dmcData, int titleID, int currentVideoFrame,int *chapter);

/** 
Get the chapter object ID for a chapter number for the specified title ID.
This method returns a pointer to a DMCMediaTrackList. This list must be deallocated with dmcDeallocTrackList().

@param[in]  dmcData - A DMCData pointer returned by dmcSetMemoryPool()
@param[in]  titleID - title ID for the chapter
@param[in]  currentVideoFrame - video frame of the required chapter (generally the current video frame playing)
@param[in]  jump - Added to the chapter number corresponding to the video frame, to return playlist ahead or prior to the current chapter. +1 = next chapter, -1 = previous chapter
@param[out] chapterNo - pointer to the chapter number of the specified video frame
@param[out] playList - pointer to pointer of media track list
@param[out] playbackType - type of the returned playlist

@return int The return code: DIVXMENUCONTROL_SUCCESS, DIVXMENUCONTROL_INVALID_MENU_ERROR, and other valid API return code
*/
int dmcGetChapterPlayList(DMCData *dmcData, int titleID, int currentVideoFrame,int jump, int *chapterNo, DMCMediaTrackList **playList, DMCPlaybackType	*playbackType);


/*
Not external API functions
*/
DMCMediaTrackRecord *allocMediaTrackRecord(DMCData *dmcData);
DMCMediaTrackList *dmcAllocTrackList(DMCData *dmcData);
int dmcAddTrackListElement(DMCData *dmcData,DMCMediaTrackRecord * trackRecord, DMCMediaTrackList * trackList);


/** @} */
#ifdef __cplusplus
}
#endif

#endif  //LMC_DIVXMENUCONTROLAPI_H
