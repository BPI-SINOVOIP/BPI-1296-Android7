#ifndef __C_FLOWEVENT_H__
#define __C_FLOWEVENT_H__

/* #include <flowevent/RecordEvent.h>  where the event structure will be defined later. */
typedef struct _tagEVRecordVobuPayload {
    int numOfFields;
    int size;
}EVRecordVobuPayload;

typedef struct _tagEventSpicFrame {
	unsigned int startTime;
	unsigned int duration;
	unsigned int type;
	unsigned int size;
	int charsetEncoding;
	char *payload;
}EventSpicFrame;


typedef enum{

    //General Event
    EV_FILE_EOF,

    //Record Flow Event
    FE_RECORD_STOP,
    FE_RECORD_VOBU,
    FE_RECORD_CELL,
    FE_RECORD_VOB,
    FE_RECORD_FULL,
    EV_DISK_FULL,
    FE_RECORD_NOSIGNAL,        // Vin no signal
    FE_RECORD_SIGNAL,          // Vin has signal
    FE_RECORD_COPYFREEDATA,    // Copy protection data
    FE_RECORD_COPYPROCTECTEDDATA, 
    FE_RECORD_NTSC,
    FE_RECORD_PAL,
    FE_RECORD_VIDEO_FORMAT_CHANGE,
    FE_RECORD_NO_OUTPUT_SPACE,
    FE_RECORD_OUTPUT_SPACE_AVAILABLE,
    FE_RECORD_DISC_ERROR,

    //Playback Flow Event
    FE_Playback_ResetSpeed,    // 0 parameter
    FE_Playback_VideoEOS,      // 1 parameter, unsigned int assigned on sending EOS, feedback from Video
    FE_Playback_AudioEOS,      // 1 parameter, unsigned int assigned on sending EOS, feedback from Audio
    FE_Playback_IndexGenEOS,   // 1 parameter, unsigned int assigned on sending EOS, feedback from IndexGen
    FE_Playback_Completed,     // 0 parameter
    FE_Playback_DomainChange,  // 1 parameter, new domain (NAV_DOMAIN)
    FE_Playback_TitleChange,   // 1 parameter, new title number (int)
    FE_Playback_ChapterChange, // 1 parameter, new chapter number (int)
    FE_Playback_TimeChange,    // 1 parameter, new elapsed time (NAVTIME)
    FE_Playback_RateChange,    // 1 parameter, new rate (long)
    FE_Playback_CommandUpdate, // 1 parameter, command execution status (NAVCMDSTATUS)
    FE_Playback_RequestTmpPml, // 1 parameter, requested temporary parental management level (int)
    FE_Playback_SetAutoStop,   // 1 parameter, information for handling auto stop (NAVAUTOSTOPINFO)
    FE_Playback_JPEGDecEOS,    // 1 parameter, new width and height values after firmware JPEG decoding(VIDEO_RPC_DEC_JPEG_INFO)
	FE_Playback_ResetRepeatMode,	// 0 parameter
	FE_Playback_FatalError,    // 1 parameter, error code (long)
    FE_Playback_ReadError,     // 0 parameter
    FE_Playback_BitstreamError,     // 0 parameter
	FE_Playback_StreamingInfoUpdate, // 1 parameter, new streaming info (NAVSTREAMINGINFO)
    FE_Playback_BufferingStart,   // 0 parameter
    FE_Playback_BufferingRestart, // 0 parameter
    FE_Playback_BufferingEnd,     // 0 parameter
    
    FE_Playback_SetAudioPreference,	// 1 parameter, set audio preferences decoding type (ENUM_MEDIA_TYPE)

	FE_Playback_NoSignal,			// 0 parameter
	
	//Playback Flow Event for DivX DRM
	FE_Playback_DRM_NOT_AUTH_USER,	// 0 parameter
	FE_Playback_DRM_RENTAL_EXPIRED,	// 0 parameter
	FE_Playback_DRM_REQUEST_RENTAL,	// 1 parameter, available views

	FE_Playback_AudioFatalError,	// 1 parameter, (AUDIO_ERROR)
   
    FE_Playback_BackwardToBeginning, // 0 parameter
    
    FE_Playback_UnsupportedFormat,	// 1 parameter, format (ENUM_MEDIA_TYPE)
    FE_Playback_InvalidFile,        // format is correct, but lack some major characteristics to play this file(ex : ts file)

	FE_Playback_SpicFrame,          // 1 parameter, format (EventSpicFrame)

    FE_Playback_ContentEncrypt,     // 0 parameter
    
    FE_Timeshift_PlaybackCatchRecord,
    FE_Timeshift_RecordCatchPlayback,
    FE_Timeshift_LivePauseWrapAround,

    FE_TRANSCODE_STOP,
    FE_TRANSCODE_VE_STOP,
    FE_TRANSCODE_AE_STOP,
    
    FE_Playback_APSStatus,		// 1 parameter (int): APS Status, zero means APS off and non-zero means APS on
    
    FE_Playback_VCDNavigationFail,

    FE_Playback_HDMV_UnsupportedFormat_Retry,
    FE_Playback_HDMV_NavigationFail_Retry,
    
    FE_Playback_UnderflowStatus,	// 1 parameter (long): 1 means underflow happened while 0 means underflow disappear 
    FE_Playback_DecoderMessage,		// 1 parameter (DECODER_MESSAGE_CODE)
	#if defined(USE_DTA_LIB) && defined(BESTV)
	FE_Playback_BESTV_UNEXPECTED_ERROR, // 0 parameter
	#endif

    FE_Playback_IO_UnderflowStatus, // 1 parameter (long): 1 means underflow happened while 0 means underflow disappear

	FE_UNKNOWN,
} EVCode;

#endif
