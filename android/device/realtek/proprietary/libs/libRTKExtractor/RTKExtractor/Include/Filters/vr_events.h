
#ifndef VR_EVENTS_H_INCLUDED
#define VR_EVENTS_H_INCLUDED

#include <Filters/PrivateInfo.h>

#define VR_BLOCK_OK			 						0x00000000
//#define VR_RDI_PACKET		 9

#define VR_PRIVATEINFO_NEW_SEGMENT					0x00000001
#define VR_PRIVATEINFO_FLUSH						0x00000002
#define VR_PRIVATEINFO_VIDEO_MIXER_RPTS				0x00000004
#define VR_PRIVATEINFO_VIDEO_DECODE_COMMAND			0x00000008
#define VR_PRIVATEINFO_VIDEO_MIXER_BLEND			0x00000010
#define VR_PRIVATEINFO_VIDEO_END_OF_SEQUENCE		0x00000020
#define VR_PRIVATEINFO_AUDIO_CHANGE					0x00000040
#define VR_PRIVATEINFO_RESET_SPEED					0x00000080
#define VR_PRIVATEINFO_STC_OFFSET					0x00000100
#define VR_PRIVATEINFO_DEMUX_SPU_TARGET				0x00000200
#define VR_PRIVATEINFO_SPU_PALETTE					0x00000400
#define VR_PRIVATEINFO_SPU_ON_OFF					0x00000800
#define VR_PRIVATEINFO_SPU_CLEAR					0x00001000
#define VR_PRIVATEINFO_VIDEO_CONTEXT				0x00002000
#define VR_PRIVATEINFO_TITLE_CHANGE					0x00004000
#define VR_PRIVATEINFO_CHAPTER_CHANGE				0x00008000
#define VR_PRIVATEINFO_PLAYBACK_CATCH_RECORD		0x00010000
#define VR_PRIVATEINFO_VIDEO_DECODE_TO_BUFFER		0x00020000
#define VR_PRIVATEINFO_END_SEGMENT					0x00040000
#define VR_PRIVATEINFO_VOBU_INFO					0x00080000
#define VR_PRIVATEINFO_V_ATR						0x00100000
#define VR_PRIVATEINFO_DCI_CCI						0x00200000
#define VR_PRIVATEINFO_VIDEO_PRESENTATION_MODE		0x00400000
#define VR_PRIVATEINFO_AUDIO_DECODE_COMMAND			0x00800000
#define	VR_PRIVATEINFO_NAV_WAIT						0x01000000	// neco+
#define	VR_PRIVATEINFO_MEDIA_REMOVED				0x02000000	// neco+
#define	VR_PRIVATEINFO_READ_ERROR					0x04000000	// neco+
#define	VR_PRIVATEINFO_BACKWARD_TO_BEGINNING		0x08000000	// neco+
#define VR_STILL_FRAME	     						0x10000000
#define VR_IDLE              						0x20000000
#define VR_STOP				 						0x40000000
#define VR_PLAYBACK_COMPLETED						0x80000000	// fault tolerant, post completed event to force stopping playback

typedef struct {
  /* The length (in seconds) the still frame should be displayed for. */
  int length;
} vr_still_event_t;

typedef struct {
	int nPGNo;
	int nPLNo;
	int nOutputFormat;
	int nTargetWidth;
	int nTargetHeight;
	int nTargetX;
	int nTargetY;
	int nPitch;
	unsigned char *pTarget;
	unsigned char *pTargetChroma;
} vr_decode_keyframe_t;

typedef struct {
	int nKeyFrameNum;
	vr_decode_keyframe_t *pDecKeyFrame;
} vr_decode_multiple_keyframe_t;

#endif /* VR_EVENTS_H_INCLUDED */
