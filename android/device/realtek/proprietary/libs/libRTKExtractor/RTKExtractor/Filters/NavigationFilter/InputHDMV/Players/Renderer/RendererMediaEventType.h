#ifndef RENDERER_MEDIA_EVENT_TYPE_H
#define RENDERER_MEDIA_EVENT_TYPE_H

// Media Event Type.
typedef enum tagEMediaEventType
{
	eMediaEvent_Stop,				// Playback Stopped
	eMediaEvent_Play,				// Playback has started (either from Stop or from Pause)
	eMediaEvent_Pause,				// Playback Paused
	eMediaEvent_SourceInfo,			// Info about media source, like if it is encrypted or not...
	eMediaEvent_NewSegment,			// New Segment (discontinious) about to start
	eMediaEvent_PlaybackRate		// Change in playback rate.
} EMediaEventType;

#endif
