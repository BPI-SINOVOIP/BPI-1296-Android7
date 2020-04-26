//----------------------------------------------------------------------------- 
// BDJModuleTypes.h 
// Copyright (c) 2005, Sonic Solutions.  All rights reserved. 
//----------------------------------------------------------------------------- 
 
#ifndef BDJMODULETYPES_H 
#define BDJMODULETYPES_H 

typedef int BDJ_Module_Handle;

// This file provided to outside
// customers who may not have
// Sonic's commonTypes.h file,
// so we'll cherry-pick a couple of defs

#ifndef __CommonTypes_h__
	typedef unsigned char	UInt8;
	typedef signed short	SInt16;
	typedef unsigned short	UInt16;
	typedef signed long		SInt32;
	typedef unsigned long UInt32;
	typedef signed long SInt32;
	typedef SInt32 SonicError;
#endif

typedef enum tagBDJ_Event{ 
	BDJ_DISC_INSERTED, 
	BDJ_DISC_EJECTED, 
	BDJ_UOPS_MASK_CHANGE, 
	BDJ_UOP_MASKED, 
	BDJ_PIP_AVAILABLE, 
	BDJ_PIP_UNAVAILABLE, 
	BDJ_PIP_STREAM_NUM_CHANGED,
	BDJ_PLAYLIST_MARK, 
	BDJ_PLAYITEM, 
	BDJ_END_OF_PLAYLIST, 
	BDJ_AUDIO_LANGUAGE_PREF_CHANGE, 
	BDJ_SUBTITLE_LANGUAGE_PREFCHANGE, 
	BDJ_MENU_LANGUAGE_PREF_CHANGE, 
	BDJ_TIME_SEARCH, 

	// BDJ_PG_TEXTST_CHANGE: PG/TextST or PiP PG/TextST Stream number or display state has changed.
	//      param 1: PG/TextST or PiP PG/TextST stream number. 0 if no stream selected.
	//      param 2: Stream type & Presentation state.
	//					bit 0: Stream type.          0= PG,  1= Text Subtitle
	//					bit 1: Stream Display State. 0= Off, 1= On
	//					bit 1: Primary/PiP Type.     0= Primary PG/TextST, 1= PiP PG/TextST
	//
	BDJ_PG_TEXTST_CHANGE, 

	BDJ_PRIMARY_AUDIO_CHANGE, 
	BDJ_SECONDARY_AUDIO_CHANGE, 
	BDJ_ANGLE_CHANGE, 
	BDJ_VIDEO_FORMAT_CHANGED,
	BDJ_MEDIA_TIME,
	BDJ_BDPLUS_STATUS,
	BDJ_GAIN_CHANGE,
	BDJ_PANNING_CHANGE,
	BDJ_END_OF_IAUDIO,
	BDJ_CLOSE_IAUDIO,
	BDJ_OUTPUT_VIDEO_FORMAT_CHANGED,
	BDJ_PARENTAL_RATING_CHANGED,
	BDJ_COUNTRY_PREF_CHANGED,
	BDJ_RATE_CHANGED,
	BDJ_SUBTITLE_DISPLAY_CHANGED,
	BDJ_SUBTITLE_AVAILABLE,
	BDJ_DATA_STARVED,
	BDJ_STOP_TIME,

	// BDJ_MEDIA_SELECT: event to trigger javax.tv.media.MediaSelectEvent
	//      param 1: MediaSelect stream numbers (part-1)
	//      			bit 00-06 : Primary Video stream number (if valid, will always be 1).
	//      			bit    07 : Primary Video stream number valid.
	//      			bit 08-14 : Primary Audio stream number (0: None, 1-32: stream num).
	//					bit    15 : Primary Audio stream number valid.
	//					bit 16-22 : Secondary Video stream number (0: None, 1-32: stream num).
	//					bit    23 : Secondary Video stream number valid
	//      			bit 24-30 : Secondary Audio stream number (0: none, 1-32: stream num).
	//      			bit    31 : Secondary Audio stream number valid.
	//      param 2: MediaSelect stream numbers (part-2)
	//      			bit 00-14 : Primary Subtitle (aka PG/TextST) stream number (0: None, 1-255 stream num)
	//      			bit    15 : Primary Subtitle (aka PG/TextST) stream number valid.
	//      			bit 16-30 : PiP Subtitle (aka PiP PG/TextST) stream number (0: None, 1-255 stream num)
	//      			bit    31 : PiP Subtitle (aka PiP PG/TextST) stream number valid.
	//				 Only one of the two Subtitle numbers will be valid.
	//
	BDJ_MEDIA_SELECT
} BDJ_Event; 
 
typedef enum tagBDJ_CallbackEvent{ 
	BDJ_TITLE_CHANGE, 
	BDJ_OBJECT_TERMINATED,
	BDJ_TITLE_STARTED,
	BDJ_KEEP_RESOLUTION
} BDJ_CallbackEvent; 
 
 
typedef SInt32(*BDJCallback)(UInt32 type, UInt32 param2); 
 
 
typedef enum tagBDROM_InputKeyCodes 
{ 
	BDJ_VK_ENTER = 10,  
	BDJ_VK_PAUSE = 19,  
	BDJ_VK_LEFT = 37,   
	BDJ_VK_UP = 38,  
	BDJ_VK_RIGHT = 39, 
	BDJ_VK_DOWN = 40,  
	BDJ_VK_0 = 48,  
	BDJ_VK_1 = 49,  
	BDJ_VK_2 = 50,  
	BDJ_VK_3 = 51,  
	BDJ_VK_4 = 52,  
	BDJ_VK_5 = 53,  
	BDJ_VK_6 = 54,  
	BDJ_VK_7 = 55,  
	BDJ_VK_8 = 56,  
	BDJ_VK_9 = 57,  
	BDJ_VK_COLORED_KEY_0 = 403,  
	BDJ_VK_COLORED_KEY_1 = 404,  
	BDJ_VK_COLORED_KEY_2 = 405,  
	BDJ_VK_COLORED_KEY_3 = 406,  
	BDJ_VK_REWIND =412, 
	BDJ_VK_STOP =413, 
	BDJ_VK_PLAY =415, 
	BDJ_VK_FAST_FWD =417,  
	BDJ_VK_TRACK_NEXT =425, 
	BDJ_VK_TRACK_PREV =424,  
	BDJ_VK_FADER_FRONT =453,  
	BDJ_VK_FADER_REAR =454,  
	BDJ_VK_SUBTITLE =460, 
	BDJ_VK_POPUP_MENU = 461,  
	BDJ_VK_STILL_OFF = 462,  
	BDJ_VK_SECONDARY_VIDEO_ENABLE_DISABLE = 464, 
	BDJ_VK_SECONDARY_AUDIO_ENABLE_DISABLE = 463, 
	BDJ_VK_PG_TEXTST_ENABLE_DISABLE = 465,
	BDJ_VK_CANCEL = 3,
	BDJ_VK_CLEAR = 12,
} BDROM_InputKeyCodes; 
 
typedef struct tagBDROMVersion
{	
	UInt8	profileMajor;
	UInt8	profileMinor;
	UInt8	profileMicro;
	UInt8	reserved;
	UInt8	bdromSpecMajor;
	UInt8	bdromSpecMinor;
	UInt16	buildNumber;
} BDROMVersion;

typedef enum tagBDROM_UOMaskIndexes 
{ 
	BDJ_UO_CHAPTER_SEARCH_MASK_INDEX = 2,
	BDJ_UO_TIME_SEARCH_MASK_INDEX = 3,
	BDJ_UO_SKIP_TO_NEXT_POINT_MASK_INDEX = 4,
	BDJ_UO_SKIP_BACK_TO_PREVIOUS_POINT_MASK_INDEX = 5,
	BDJ_UO_PLAY_MASK_INDEX = 6,
	BDJ_UO_STOP_MASK_INDEX = 7,
	BDJ_UO_PAUSE_ON_MASK_INDEX = 8,
	BDJ_UO_STILL_OFF_MASK_INDEX = 10,
	BDJ_UO_FORWARD_PLAY_MASK_INDEX = 11,
	BDJ_UO_BACKWARD_PLAY_MASK_INDEX = 12,
	BDJ_UO_RESUME_MASK_INDEX = 13,
	BDJ_UO_MOVE_UP_SELECTED_BUTTON_MASK_INDEX = 14,
	BDJ_UO_MOVE_DOWN_SELECTED_BUTTON_MASK_INDEX = 15,
	BDJ_UO_MOVE_LEFT_SELECTED_BUTTON_MASK_INDEX = 16,
	BDJ_UO_MOVE_RIGHT_SELECTED_BUTTON_MASK_INDEX = 17,
	BDJ_UO_SELECT_BUTTON_MASK_INDEX = 18,
	BDJ_UO_ACTIVATE_BUTTON_MASK_INDEX = 19,
	BDJ_UO_SELECT_AND_ACTIVATE_MASK_INDEX = 20,
	BDJ_UO_AUDIO_CHANGE_MASK_INDEX = 21,
	BDJ_UO_PRIMARY_AUDIO_CHANGE_MASK_INDEX = 21,
	BDJ_UO_ANGLE_CHANGE_MASK_INDEX = 23,
	BDJ_UO_POPUP_ON_MASK_INDEX = 24,
	BDJ_UO_POPUP_OFF_MASK_INDEX = 25,
	BDJ_UO_PG_TEXTST_ENABLE_DISABLE_MASK_INDEX = 26,
	BDJ_UO_PG_TEXTST_CHANGE_MASK_INDEX = 27,
	BDJ_UO_SECONDARY_VIDEO_ENABLE_DISABLE_MASK_INDEX = 28,
	BDJ_UO_SECONDARY_VIDEO_CHANGE_MASK_INDEX = 29,
	BDJ_UO_SECONDARY_AUDIO_ENABLE_DISABLE_MASK_INDEX = 30,
	BDJ_UO_SECONDARY_AUDIO_CHANGE_MASK_INDEX = 31,
	BDJ_UO_PIP_PG_TEXTST_CHANGE_MASK_INDEX = 33,
} BDROM_UOMaskIndexes; 
#endif
