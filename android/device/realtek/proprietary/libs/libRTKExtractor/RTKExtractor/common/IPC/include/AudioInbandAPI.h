
#ifndef __AUDIO_INBAND_API_H__
#define __AUDIO_INBAND_API_H__

#include "AudioRPCBaseDS_data.h"

/** Audio Inband APIs */
/**
	  This spec defines a set of API to send command via cmd fifo.
		Inband command is passed through inband cmd fifo. 
		System software should take the responsibility to convert and write cmds to the cmd fifo, and 
		keep the cmd fifo from overflow.
*/

/** Audio inband cmd type. I use prefix "AUDIO_DEC_" to label the cmd used in Audio decoder. 
	Prefix AUDIO_MIXER is used to lable the Mixer commands.
*/

//	downmix could be placed on in-band queue
//	set media type or private information to in-band queue
typedef enum
{
  	/* Audio Decoder In-band Command */
  	AUDIO_DEC_INBAND_CMD_TYPE_PTS = 0,
  	AUDIO_DEC_INBAND_CMD_TYPE_NEW_SEG = 1,		/* let begin_AGC_gain = 1 */
  	AUDIO_DEC_INBAMD_CMD_TYPE_NEW_FORMAT = 2,
  	AUDIO_DEC_INBAND_CMD_TYPE_EOS = 3,			/* Can not be passed, sent back to system */	
  	AUDIO_DEC_INBAND_CMD_TYPE_CONTEXT = 4,		/* write debug information, need a RPC_ToSystem command *//* Can not be passed, sent back to system */
  	AUDIO_DEC_INBAND_CMD_TYPE_DECODE = 5,		/* Information for each decoder is inclueded */
  	AUDIO_DEC_INABND_CMD_TYPE_PTS_SKIP = 6,
  	AUDIO_DEC_INBAND_CMD_TYPE_PRIVATE_INFO = 7,
	AUDIO_DEC_INBAND_CMD_TYPE_VORBIS_PACKETINFO = 8,    /* Information about vorbis packets */

	AUDIO_DEC_INBAND_CMD_TYPE_BS_DISCONTINUITY = 9,
	AUDIO_DEC_INBAND_CMD_TYPE_RESYNC = 10,		
	AUDIO_DEC_INBAND_CMD_TYPE_CGMS_INFO = 11,		
	AUDIO_DEC_INBAND_CMD_TYPE_RALSD_INFO = 12,
	AUDIO_DEC_INBAND_CMD_TYPE_WMAPRO_DRC = 13,		
	AUDIO_DEC_INBAND_CMD_TYPE_WMAPRO_DWNMIX_MTRX = 14,	

	AUDIO_DEC_INBAND_CMD_TYPE_EMPHASIS = 15,		// for lpcm emphasis info	

#ifndef AUDIO_TV_PLATFORM_TEMP          
         AUDIO_DEC_INBAND_CMD_TYPE_APE_SKIP_BIT = 17,
#endif         
  	/* Audio AO Mixer In-band Command */
  	AUDIO_AO_MIXER_INBAND_CMD_TYPE_SET_MIXER_RPTS = 64,
  	AUDIO_AO_MIXER_INBAND_CMD_TYPE_SET_BLEND,

  	/* Audio DV Mixer In-band Command */
 	AUDIO_DV_MIXER_INBAND_CMD_TYPE_SET_BLEND

} AUDIO_INBAND_CMD_TYPE ;

typedef struct
{
  	AUDIO_INBAND_CMD_TYPE 	type ;
  	long  					size ;
} AUDIO_INBAND_CMD_PKT_HEADER;

/* Audio Decoder Inband APIs */
/**
==========================================================================================
	Note (Please read):
	
	At decoder filter init time, 2 ringbuffers are inited with type of STREAM and COMMAND
	Since parsing mode is necessaey for audio firmware, the decoder will peek the bit stream buffer instead of
	reading it out.

	The inband commands are issued in the command segment, each segment looks this way
	([] means optional):
		----------
		NEW_SEG	
		----------
		DECODE
		FORMAT
		----------
		[PTS]
		[CONTEXT]
		----------
		[EOS]
		----------
	System can issue one or more command segments. Decoder will finish decoding one segment
	before decoding the new one. Decoding may be stopped in 2 ways: 
	1. decode is finished (PTSDuration is finished)
	2. EOS or another NEW_SEG is reached.
	Otherwise, decoder will wait for more bistream and bistream commands (PTS, SEQ_END, CONTEXT).

	System must issue all bistream commands before updating ring buffer write pointer to avoid
	racing condition.
	

	==========================================================================================
**/

/**
	\brief
		This function will send a new segment tag in cmd fifo.
		Audio decoder will check cmd fifo and handle this new seg tag
		when decodeing to the location on address wPtr. The Audio
		decoder will expect a pts followed by the new seg tag immediately.
		Then, the following decode command will be executed in pts offset
		to the pts of new segement.
		
	\param wPtr
		A coresponding address to the bit stream buffer. 
*/


typedef enum 
{
	AUDIO_BIG_ENDIAN        = 0,       // CD,DVD type
	AUDIO_LITTLE_ENDIAN     = 1,       
    AUDIO_LPCM_DHMV_TYPE    = 2,       // typo, stay here for backware compatilbe, HDMV type
    AUDIO_LPCM_HDMV_TYPE    = 2,       // HDMV type
} AUDIO_ENDIANTYPE;


typedef enum 
{
	AUDIO_CGMS_COPY_FREE,
	AUDIO_CGMS_COPY_NEVER,
	AUDIO_CGMS_COPY_ONCE,
	AUDIO_CGMS_INVALID,
} AUDIO_CGMS_TYPE;

typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long wPtr ;
} 	AUDIO_DEC_NEW_SEG ; 

typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long	wPtr ;
  	unsigned long	CGMSInfo;	
} 	AUDIO_DEC_CGMS_INFO ; 
#ifndef AUDIO_TV_PLATFORM_TEMP
typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long   SeekByteNum ;
  	unsigned long	SeekBitNum ;
} 	AUDIO_APEDEC_SEEK_INFO ; 
#endif
typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long	wPtr ;
  	unsigned long	emphasis;	
} 	AUDIO_DEC_EMPHASIS_INFO ; 

typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long wPtr ;
	long		  pktSize;
} 	AUDIO_DEC_RALSD_INFO ; 

/*	LPCM

	privateInfo[0] = sampling rate;
	privateInfo[1] = bitWidth;
	privateInfo[2] = downMixEnable, -1 = disable, >= 1 means the target channel number.
	
	MPEG
	
	privateInfo[0] = deemphasis, 1 = emphasis enable;
	privateInfo[1] = extbsExit, 1 = exist, 0 = not exist
	privateInfo[2] = downMixEnable, -1 = disable, >= 1 means the target channel number.

	AC3
	
	privateInfo[0] = bitStreamMode;	
					000 = complete main (CM) 
					001	= music and effect (ME)
					010 = visually impaired (VI)
					011 = hearing impaired (HI)
					100 = dialogou (D)
					101 = commentory (C)
					110	= emmergency (E)
					111	= voice over (VO) 
	
	privateInfo[1] = ACmode;		
					001 & bitStramMode = 111, VO
					010~111 & bitStramMode = 111, Karaoke
					000		L1+L2
					001		1/0, C
					010		2/0, L+R
					011		L,C,R
					100		L,R,S
					101		L,C,R,S
					110		L,R,SL,SR
					111		L,C,R,SL,SR

	privateInfo[2] = downMixEnable, -1 = disable, >= 1 means the target channel number.
					
*/	 

typedef	struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long 				wPtr;
  	AUDIO_DEC_TYPE				audioType;
  	long						privateInfo[8];  // note: privateinfo[6] is used for choosing decoder sync pts method
}	AUDIO_DEC_NEW_FORMAT ;


typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long 				wPtr ;
	unsigned long				PTSH;
	unsigned long				PTSL;
} AUDIO_DEC_PTS_INFO ;

typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long  				wPtr;
  	long						EOSID;	/* ID associated with command */
} AUDIO_DEC_EOS;

/**
	\brief Send a general purpose context which is associated a wPtr. It might be "here!", "run to here!" or something like that.
	\param: wPtr
*/
typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long 				wPtr;
	long 						context;
} AUDIO_DEC_CONTEXT;


enum DEC_MODE {
	AUDIO_DECODE,
	AUDIO_PARSING		// purpose: (1) check bit stream is valid or not. (2) for SPDIF pass through.
};

/**
	\brief Decode and/or parse a bitstream by the instruction of parameter AUDIO_DEC_DECODE.
	\param AUDIO_DEC_DECODE.
	See more detailed comments in AUDIO_DEC_DECODE
*/

typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long 				wPtr;
	unsigned long				peakReference;
	unsigned long				peakTarget;
	unsigned long				avgReference;
	unsigned long				avgTarget;
} 	AUDIO_DEC_WMAPRO_DRC ;


typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long 				wPtr;
	unsigned long				nameLen;
	unsigned long				dataLen;
} 	AUDIO_DEC_WMAPRO_DWNMIX_MTRX ;

typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header;
	unsigned long 				wPtr;
	unsigned long				RelativePTSH;
	unsigned long				RelativePTSL;
	unsigned long				PTSDurationH;
	unsigned long				PTSDurationL;
	enum						DEC_MODE	decodeMode; 

} 	AUDIO_DEC_DECODE ;


/**
 *  \brief Tell decoder to look at the system PTS (SPTS not RPTS) in shared reference clock.
 *  if (SPTS of current picture + deltaPTS < current SPTS in ref clock) skip decoding this picture.
 *  This is used to catch the display quickly. deltaPTS is the estimated latency between decoder and
 *  AOUT with a margin. In other words, it is the DTS.
 *  \param:
 */
 
typedef struct
{
 	AUDIO_INBAND_CMD_PKT_HEADER header;
  	unsigned long 				wPtr ;
  	unsigned long				OffsetPTSH;
  	unsigned long				OffsetPTSL;
}	AUDIO_DEC_PTS_SKIP;



typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long 				wPtr ;
} 	AUDIO_DEC_BS_DISCONTINUITY ; 


typedef struct
{
  	AUDIO_INBAND_CMD_PKT_HEADER header ;
  	unsigned long wPtr ;
} 	AUDIO_DEC_RESYNC ; 

//	We should discuss about the message ringbuffer, we could use it to prepare audio header information.

/**
 * \defgroup   Sub-picture Decoder in-band command definitions.
 * \@{
 */

/** Audio Mixer Inband APIs */
/**
	  This spec defines a set of API to send command via cmd fifo.
		Inband command is passed through inband cmd fifo. Each command
		is contained in AUDIO_INBAND_CMD_PKT. Once the system software
		push a AUDIO_INBAND_CMD_PKT in the cmd fifo, it should then update
		the write pointer. System software should take the responsibility
		to keep the cmd fifo from overflow.
	==========================================================================================
	Note (Please read):

	Audio Mixer is filter with 2 input pins and one output pin. Its inputs are connected to 2 decoders
	and its output is connected to a VOUT or a Audio Encoder.

	Audio mixer operation is very simple, it does not have run, stop, or pause. It only have 2 APIs:
	SET_MIXER_RPTS and BLEND. 
	
	SET_MIXER_RPTS sets mixer relative PTS. All the future BLEND command refer to this RPTS.

	BLEND is the only command that causes actions: blend 1 or 2 stream(s) and/or background 
	and send the output Audio to output pin. Mixer RPTS is counted with mixer output. 

	The output of Blending will be AGC controlled.

	BlendMode selects A only, B only and AB blend. 
	For A only mode, only stream A buffer is sent out.
	For B only mode, only stream B  buffer is sent out.
	For AB only mode, both stream A and stream B buffers are sent out and AGC conttrolled.
	
	Implementation notes for AB mode:

	==========================================================================================
*/

typedef struct
{
 	AUDIO_INBAND_CMD_PKT_HEADER header;
  	unsigned long				PTSH;
  	unsigned long				PTSL;
} AUDIO_AO_MIXER_RPTS ;

typedef struct
{
 	AUDIO_INBAND_CMD_PKT_HEADER header;
  	unsigned long				PTSH;
  	unsigned long				PTSL;
} AUDIO_DV_MIXER_RPTS ;

//	Since it will be fade out to 0 and fade in to 255, so the alpha is not assigned.
//	If SetFocus is considered, only 1 input pin is selected.

typedef	struct
{
	long	bitWidth;
	long	samplingRate;
}	AUDIO_MIXER_PCM_FORMAT;

/*	Since DV mixer is always an off-line application, we do not need to set relative PTS first.

	For example, we have 4 audio sources to be mixed.
	
	1.	AVI or MPEG demuxed audio PCM sample, demuxer will stamp the PTS according to GPTS.
	2.	Another AVI or MPEG demuxed audio PCM sample, demuxer will stamp the PTS according to GPTS.
	3.	Microphone in from AI, the PTS stamp is given by AI according to GPTS.
	4.	A WAVE file which is decoded by the LPCM decoder, demuxer will stamp the PTS according to GPTS.
	
	However, the time stamps are meaningless, because user will replace them here and there again and again.
	After some trials, user will decide the location of the three sources. Microphone is a real-time source
	which be inputed at the run time. 
	
	Since it is very lousy to re-stamp all the PTS for the 3 non-real-time resources, the muxer should only
	put the correct data in the ring buffer with a correct wptr. Since the whole process may be more than 1 
	hour, garbage should not be put into ring buffer at all.
	
	Therefore, PTS is useless for begin / stop marking. 
*/
	
typedef	struct

{
	
 	AUDIO_INBAND_CMD_PKT_HEADER header;
   	unsigned long	PTSDurationH;			// Duration in PTS 
  	unsigned long	PTSDuration;			// Duration in PTS
	long	pinID[4];				// for at most 4 inputs, 1 = valid, 0 = invalid
	
	long	alphaA[4];				// alpha for 4 inputs to begin with
	long	alphaB[4];				// alpha for 4 inputs to stop with
	long	channelValid[4];		// LSB 8 bits stand for 0~7 channels	
	long	frameSize;				// internal frame size, for DVMixer	editing, default = 256.
	AUDIO_MIXER_PCM_FORMAT	inputFormat[4];	// in case of needing converting
}	AUDIO_DV_MIXER_BLEND;



/*
	If the mixer is configure to select mode, there will be 2 cases:
	
	If SetFocus is considered, only 1 input pin is selected.
	If SwitchAgent is considered, mixer will not find the decoder is changed at all.
	
	If the mixer is configure to mixer mode:
	
	Two sources may have different PTS system, therefore, we need relative PTS.
	
	*/
	
typedef	struct
{

 	AUDIO_INBAND_CMD_PKT_HEADER header;
   	unsigned long	PTSDurationH;			// Duration in PTS
  	unsigned long	PTSDuration;			// Duration in PTS
	long	pinID[4];				// for at most 4 inputs, 1 = valid, 0 = invalid
	
	long	alphaA[4];				// alpha for 4 inputs to begin with
	long	alphaB[4];				// alpha for 4 inputs to stop with
	long	channelValid[4];		// LSB 8 bits stand for 0~7 channels	
	long	frameSize;				// internal frame size, for DVMixer	editing, default = 256.
	AUDIO_MIXER_PCM_FORMAT	inputFormat[4];	// in case of needing converting
}	AUDIO_AO_MIXER_BLAND;

typedef struct{
      unsigned int vorbisPacketSize;
//    unsigned char vorbisPacketState;
      unsigned int vorbisPacketState;
}     AUDIO_VORBIS_PACKET_INFO;

typedef struct{
      AUDIO_INBAND_CMD_PKT_HEADER header;
      unsigned int listSize;
}     AUDIO_VORBIS_PACKETLIST_SIZE;

typedef struct{
//    AUDIO_VORBIS_PACKETLIST_SIZE header;
      AUDIO_INBAND_CMD_PKT_HEADER header;
      AUDIO_VORBIS_PACKET_INFO  info[255];
}     AUDIO_VORBIS_PACKET_LIST_INFO;



#endif

/** \@} */
