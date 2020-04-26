#ifndef __INBAND_API_H__
#define __INBAND_API_H__

/** Inband APIs */
/**
	  This spec defines a set of API to send command via cmd fifo.
		Inband command is passed through inband cmd fifo.
		System software should take the responsibility
		to convert and write cmds to the cmd fifo
		keep the cmd fifo from overflow.
*/

/** inband cmd type. I use prefix "VIDEO_DEC_" to label the cmd used in video decoder. */
typedef enum
{
  INBAND_CMD_TYPE_PTS = 0,
  INBAND_CMD_TYPE_PTS_SKIP,
  INBAND_CMD_TYPE_NEW_SEG,
  INBAND_CMD_TYPE_SEQ_END,
  INBAND_CMD_TYPE_EOS,
  INBAND_CMD_TYPE_CONTEXT,
  INBAND_CMD_TYPE_DECODE,

  /* Video Decoder In-band Command */
  VIDEO_DEC_INBAND_CMD_TYPE_VOBU,
  VIDEO_DEC_INBAND_CMD_TYPE_DVDVR_DCI_CCI,
  VIDEO_DEC_INBAND_CMD_TYPE_DVDV_VATR,

  /* MSG Type for parse mode */
  VIDEO_DEC_INBAND_CMD_TYPE_SEG_INFO,
  VIDEO_DEC_INBAND_CMD_TYPE_PIC_INFO,

  /* Sub-picture Decoder In-band Command */
  VIDEO_SUBP_INBAND_CMD_TYPE_SET_PALETTE,
  VIDEO_SUBP_INBAND_CMD_TYPE_SET_HIGHLIGHT,

  /* Video Mixer In-band Command */
  VIDEO_MIXER_INBAND_CMD_TYPE_SET_BG_COLOR,
  VIDEO_MIXER_INBAND_CMD_TYPE_SET_MIXER_RPTS,
  VIDEO_MIXER_INBAND_CMD_TYPE_BLEND,

  /* Video Scaler In-band Command */
  VIDEO_SCALER_INBAND_CMD_TYPE_OUTPUT_FMT,

  /*DivX3 resolution In-band Command*/
  VIDEO_DIVX3_INBAND_CMD_TYPE_RESOLUTION,

  /*MPEG4 DivX4 detected In-band command*/
  VIDEO_MPEG4_INBAND_CMD_TYPE_DIVX4,
  /* Audio In-band Commands Start Here */

  /* DV In-band Commands */
  VIDEO_DV_INBAND_CMD_TYPE_VAUX,
  VIDEO_DV_INBAND_CMD_TYPE_FF,                //fast forward

  /* Transport Demux In-band command */
  VIDEO_TRANSPORT_DEMUX_INBAND_CMD_TYPE_PID,
  VIDEO_TRANSPORT_DEMUX_INBAND_CMD_TYPE_PTS_OFFSET,
  VIDEO_TRANSPORT_DEMUX_INBAND_CMD_TYPE_PACKET_SIZE,

  /* Real Video In-band command */
  VIDEO_RV_INBAND_CMD_TYPE_FRAME_INFO,
  VIDEO_RV_INBAND_CMD_TYPE_FORMAT_INFO,
  VIDEO_RV_INBAND_CMD_TYPE_SEGMENT_INFO,

  /*VC1 video In-band command*/
  VIDEO_VC1_INBAND_CMD_TYPE_SEQ_INFO,

  /* general video properties */
  VIDEO_INBAND_CMD_TYPE_VIDEO_USABILITY_INFO,
  VIDEO_INBAND_CMD_TYPE_VIDEO_MPEG4_USABILITY_INFO,

  /*MJPEG resolution In-band Command*/
  VIDEO_MJPEG_INBAND_CMD_TYPE_RESOLUTION,

  /* picture object for graphic */
  VIDEO_GRAPHIC_INBAND_CMD_TYPE_PICTURE_OBJECT,
  VIDEO_GRAPHIC_INBAND_CMD_TYPE_DISPLAY_INFO,

  /* subtitle offset sequence id for 3D video */
  VIDEO_DEC_INBAND_CMD_TYPE_SUBP_OFFSET_SEQUENCE_ID,

  VIDEO_H264_INBAND_CMD_TYPE_DPBBYPASS,

  /* Clear back frame to black color and send it to VO */
  VIDEO_FJPEG_INBAND_CMD_TYPE_CLEAR_SCREEN,

  /* each picture info of MJPEG */
  VIDEO_FJPEG_INBAND_CMD_TYPE_PIC_INFO,

  /*FJPEG resolution In-band Command*/
  VIDEO_FJPEG_INBAND_CMD_TYPE_RESOLUTION,  
  
} INBAND_CMD_TYPE ;


/* Audio/VIdeo Decoder Inband APIs */
/**
==========================================================================================
	Note (Please read):

	At decoder filter init time, 2 ringbuffers are inited with type of STREAM and COMMAND
	If parsing will be used, 3rd ringbuffer needs to be inited with type of MESSAGE

	The inband commands are issued in the command segment, each segment begins with NEW_SEG,
	end with EOS:
	([] means optional):
		----------
		NEW_SEG
		----------
		[OUTPUT_FMT]
		[PTS_SKIP]
		DECODE
		----------
		[PTS]
		[SEQ_END]
		[CONTEXT]
		----------
		EOS
		----------
	System can issue one or more command segments. Decoder will finish decoding one segment
	before decoding the new one. Decoding may be stopped in 2 ways:
	1. decode is finished (PTSDuration is finished)
	2. EOS is reached.
	Otherwise, decoder will wait for more bistream and bistream commands (PTS, SEQ_END, CONTEXT).

	System must issue all bistream commands before updating ring buffer write point to avoid
	racing condition.

	Deocoder needs to mantain a SegNum. SegNum = 0 when decoder is created, increated by one after
	each EOS. Decoder needs to attach SegNum in the 1st ouput frame/field and EOS in the last ouput
	frame/field in this segment.

	Decoder also needs to attach both PTS and RPTS (RelativePTS) to the decoded picture for system
	to retrive from VOUT when they are displayed.

	Decoder needs to attach EOS with the integer passed by NEW_SEG by system to the last decoded field/frame.


	==========================================================================================
**/
/**
 * This header will be used in all following inband command struct.
 * While receiver(Video decoder) looks into ICQ and retrieve a cmd out,
 * it will use this struct to access the wPtr and size to decide
 * if this cmd should be processed and how many bytes it should consumes.
 * These three fields are included for easy implementing in ALL cmd even
 * some cmd doesn't need wPtr and size.
 *
 * I give a easy-version example:
 *
 * In system side:
 *
 * VIDEO_DEC_OOXX  ooxx ;
 * INBAND_CMD_PKT_HEADER *header ;
 * header = &ooxx.header ;
 * header->type = VIDEO_INBAND_CMD_TYPE_OOXX ;
 * header->wPtr = bitstream.write_ptr ;
 * header->size = sizeof( VIDEO_DEC_OOXX ) ;
 * ooxx.oo = oo ;
 * ooxx.xx = xx ;
 * ...
 * swap_endian(&ooxx) ;
 * push_cmd_into_icq(&ooxx) ;
 *
 *
 * In video side: (given the icq)
 *
 * INBAND_CMD_PKT_HEADER *header = icq.RdPtr ;
 * unsigned int *cmd ;
 * while( is_this_cmd_shoud_be_processed( header->wPtr))
 * {
 *   cmd = malloc(sizeof( header->size) );
 *   memcpy( cmd, icq.RdPtr + sizeof( INBAND_CMD_PKT_HEADER) ) ;
 *   switch( header->type)
 *   {
 *     case VIDEO_DEC_OOXX:
 *      ... ;
 *			... ;
 *			break ;
 *		 case VIDEO_DEC_XXOO:
 *		 ...
 * 		 ...
 *			break ;
 *   }
 *   ICQ_UpdateRdPtr(icq, header->size) ;
 * }
 *
 */
typedef struct
{
  INBAND_CMD_TYPE type ;
  unsigned int size ;
} INBAND_CMD_PKT_HEADER ;


/**
	\brief
		This function will send a new segment tag in cmd fifo.
		Video decoder will check cmd fifo and handle this new seg tag
		when decodeing to the location on address wPtr. The video
		decoder will expect a pts followed by new seg tag immediately.
		Then, the following decode command will be executed in pts offset
		to the pts of new segement. Decoder needs to attach the segID to
		the last output picture with EOS.

	\param wPtr
		An coresponding address to bit stream buffer.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
} NEW_SEG ;

/**
	\brief
	\param wPtr
		An coresponding address to bit stream buffer.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
} SEQ_END ;

/**
 *  \brief Mark a PTS on an address.
 *  \param:
 */
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int PTSH ;
  unsigned int PTSL ;
} PTS_INFO ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int PTSH ;
  unsigned int PTSL ;
  unsigned int DataSize;
} PTS_INFO_V2 ;

/**
 *  \brief Tell decoder to look at the system PTS (SPTS not RPTS) in shared reference clock.
 *  if (SPTS of current picture + OffsetPTS < current SPTS in ref clock) skip decoding this picture.
 *  This is used to catch the display quickly. OffsetPTS is the estimated latency between decoder and
 *  VOUT with a margin.
 *  \param:
 */
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int OffsetPTSH ;
  unsigned int OffsetPTSL ;
} PTS_SKIP ;

/**
	\brief Mark a EOS on an address.
		This is the last inband command of a segment and is mandatary.
	\param wPtr
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int eventID ;
} EOS ;

/**
	\brief Send a general purpose context which is associated a wPtr.
	\param: wPtr
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int context ;
} CTXT ;


/**
	\brief This command is mainly for decoding thumbnail.
*/
typedef enum
{
 IC_RGB332 = 0,
 IC_RGB565,
 IC_RGBA8888,
 IC_ARGB8888,
 IC_YUV
} INBAND_COLOR_FMT;

typedef enum
{
	INBAND_ROTATE_0 = 0,
	INBAND_ROTATE_90 = 1,
	INBAND_ROTATE_180 = 2,
	INBAND_ROTATE_270 = 3,
} INBAND_JPEG_ROTATION ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  INBAND_COLOR_FMT colorFormat ;    //0: RGB332, 1: RGB565, 2: RGBA8888, 3:ARGB8888, 4: YUV
  unsigned int pTargetLuma ;    //Linear address of Luman buffer.
  unsigned int pTargetChroma ;  //Linear address of Chroma buffer.
  unsigned int pitch ;
  unsigned int targetRectX ;
  unsigned int targetRectY ;
  unsigned int targetRectWidth ;
  unsigned int targetRectHeight ;
  INBAND_JPEG_ROTATION rotation ;
  unsigned int alpha ;        // if RGB8888 mode, alpha is for RGBA or ARGB
  unsigned int ID ;  // this is the same as eventID in EOS
} VIDEO_SCALER_OUTFORMAT ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int width ;
  unsigned int height ;
}VIDEO_DIVX3_RESOLUTION ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int DivX4_Detected ;
}VIDEO_MPEG4_DIVX4 ;

typedef enum
{
  NORMAL_DECODE = 0,
  I_ONLY_DECODE,
  FASTFR_DECODE,
  RESERVED1,
  TS_NORMAL_DECODE,
  TS_I_ONLY_DECODE,
  TS_FASTFR_DECODE,
  RESERVED2,
  BITSTREAM_PARSING,
  TRANSCODE_PARSING

} DECODE_MODE ;



/**
	\brief This struct indicates when to decode and how long to decode the bitstream.
	To determine the first picture to decode:
		1. find the first decodable GOP
		2. skip "skip_GOP" GOPs, should be ignored by audio.
		3. start counting PTS from 0, start decode the first picture with PTS >= PTSFramGOP
	|...BBPBBP...|IBBP...  |IBBP...  | ... |IBBP........... BPBBPBBPBBBPBB|....
	|<-NS-skip ->| 1st GOP | 2nd GOP | ... |<-RelativePTS ->|<-Duration ->|
	                                                        |<- decoded ->|

	mode: 	0: decode only
		1: parse only
		2: parse and decode

	Implementation note:
		1. If parse is enabled, an INFO message is generated during first pass for each sequence header and picture.
 */
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int RelativePTSH ;
  unsigned int RelativePTSL ;
  unsigned int PTSDurationH ;
  unsigned int PTSDurationL ;
  unsigned int skip_GOP ;
  DECODE_MODE  mode ;

} DECODE ;


/**
	\brief This struct is used to delimit the VOBU boundaries in the video
	 bitstream and specify other information related to a VOBU such as the
	 start PTM and end PTM. The video decoder needs this message to stamp
	 the PTS for the 'B' pictures before the first 'I' picture in the first
	 VOBU of a new decoder segment. VOBU boundary information is also used
	 in the DVD smooth reverse playback.
 */
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int startPTM_L ;
  unsigned int startPTM_H ;
  unsigned int endPTM_L ;
  unsigned int endPTM_H ;
  unsigned int RPTS_L ;
  unsigned int RPTS_H ;
  unsigned int property ;   /* bit 0 (LSB): 1 if still VOBU, 0 otherwise */
                            /* bits 1-31  : TBD */
} VIDEO_DEC_VOBU ;


/**
	\brief This struct is used to pass the DCI (Display Control Information)
	 and the CCI (Copy Control Information) from the DVD-VR RDI packets.
	 (see DVD-VR Speccification sec. 5.5.4.2 for details). The Video Decoder
	 will translate the DCI and CCI information into video firmware internal
	 format to represent the VBI data and stamp it into each decoded picture
	 as part of the picture informaion.
 */
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int DCI ;
  unsigned int CCI ;

} VIDEO_DEC_DVDVR_DCI_CCI ;


/**
	\brief This struct is used to pass the video attribute information
	 in DVD-Video extracted from VMGM_V_ATR, VTSM_V_ATR or VTS_V_ATR
	 (see DVD-Video Specification sec. 4.1.1 and 4.2.1 for details)
	 The Video Decoder will translate part of the video attribute information
	 into video firmware internal format to represent the VBI data and stamp
	 it into each decoded picture as part of the picture informaion. Other
	 part of the information will be used to verify the video attributes
	 the Video Decoder actually sees in the video elementary stream. If
	 any descrepancy arises, the Video Decoder shall report to the system
	 software or seek error concealment.
 */
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int V_ATR ;
} VIDEO_DEC_DVDV_VATR ;


/**
 * This is the picture info decoder write to the message ringbuffer when parse mode is turned on.
 */
typedef enum
{
	INBAND_D_TYPE = 0,
	INBAND_I_TYPE,
	INBAND_P_TYPE,
	INBAND_B_TYPE,

} INBAND_PIC_TYPE ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int PTSH ;       /* System   PTS High */
  unsigned int PTSL ;       /* System   PTS Low  */
  unsigned int RPTSH ;      /* Relative PTS High */
  unsigned int RPTSL ;      /* Relative PTS Low  */
  unsigned int DTSH ;       /*          DTS High */
  unsigned int DTSL ;       /*          DTS Low  */
  INBAND_PIC_TYPE	picType ;  /* Picture coding type, as defined in video/common/mpeg_def.h */
  unsigned int numOfField ;  /* 2 fields or 3 fields */
  unsigned int byteFromNS ;  /* Bytes from NEW_SEG */
  unsigned int picSize ;     /* Picture size in bytes, including high level headers (SEQ and/or GOP) before and stuffing after.
                              * from the current header to the next header. */
  unsigned int headSize ;    /* High level (SEG and/or GOP) header size, included in picSize, in bytes. 0 means no SEG or GOP header
                              * before current picture */
} VIDEO_DEC_PIC_INFO ;


typedef enum
{
	VIDEO_MPEG2 = 0,
	VIDEO_MPEG1
} INBAND_VIDEO_TYPE ;


typedef enum
{
  INBAND_CMD_GRAPHIC_FORMAT_RGB565            = 4,    /* 16-bit RGB    (565)  with constant alpha */
  INBAND_CMD_GRAPHIC_FORMAT_ARGB1555          = 5,    /* 16-bit ARGB   (1555) */
  INBAND_CMD_GRAPHIC_FORMAT_ARGB4444          = 6,    /* 16-bit ARGB   (4444) */
  INBAND_CMD_GRAPHIC_FORMAT_ARGB8888          = 7,    /* 32-bit ARGB   (8888) */
  INBAND_CMD_GRAPHIC_FORMAT_YCBCRA4444        = 11,   /* 16-bit YCBCRA (4444) */
  INBAND_CMD_GRAPHIC_FORMAT_YCBCRA8888        = 12,   /* 32-bit YCBCRA (8888) */
  INBAND_CMD_GRAPHIC_FORMAT_RGBA5551          = 13,   /* 16-bit RGBA   (5551) */
  INBAND_CMD_GRAPHIC_FORMAT_RGBA4444          = 14,   /* 16-bit RGBA   (4444) */
  INBAND_CMD_GRAPHIC_FORMAT_RGBA8888          = 15,   /* 32-bit RGBA   (8888) */
  INBAND_CMD_GRAPHIC_FORMAT_RGB556            = 20,   /* 16-bit RGB    (556)  with constant alpha */
  INBAND_CMD_GRAPHIC_FORMAT_RGB655            = 21,   /* 16-bit RGB    (655)  with constant alpha */
  INBAND_CMD_GRAPHIC_FORMAT_RGB888            = 22,   /* 24-bit RGB    (888)  with constant alpha */
  INBAND_CMD_GRAPHIC_FORMAT_RGB565_LITTLE     = 36,   /* litttle endian below */
  INBAND_CMD_GRAPHIC_FORMAT_ARGB1555_LITTLE   = 37,
  INBAND_CMD_GRAPHIC_FORMAT_ARGB4444_LITTLE   = 38,
  INBAND_CMD_GRAPHIC_FORMAT_ARGB8888_LITTLE   = 39,
  INBAND_CMD_GRAPHIC_FORMAT_YCBCRA4444_LITTLE = 43,
  INBAND_CMD_GRAPHIC_FORMAT_YCBCRA8888_LITTLE = 44,
  INBAND_CMD_GRAPHIC_FORMAT_RGBA5551_LITTLE   = 45,
  INBAND_CMD_GRAPHIC_FORMAT_RGBA4444_LITTLE   = 46,
  INBAND_CMD_GRAPHIC_FORMAT_RGBA8888_LITTLE   = 47,
  INBAND_CMD_GRAPHIC_FORMAT_RGB556_LITTLE     = 52,
  INBAND_CMD_GRAPHIC_FORMAT_RGB655_LITTLE     = 53,
  INBAND_CMD_GRAPHIC_FORMAT_RGB888_LITTLE     = 54

} INBAND_CMD_GRAPHIC_FORMAT ;



typedef enum
{
   INBAND_CMD_GRAPHIC_2D_MODE = 0
  ,INBAND_CMD_GRAPHIC_SIDE_BY_SIDE = 1
  ,INBAND_CMD_GRAPHIC_TOP_AND_BOTTOM = 2
  ,INBAND_CMD_GRAPHIC_FRAME_PACKING = 3

} INBAND_CMD_GRAPHIC_3D_MODE ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  INBAND_VIDEO_TYPE VideoEncoderType ;     // enum mpeg1 or mpeg2
  unsigned int FrameRate ;            // enum video frame rate
  unsigned int FrameWidth ;           // picture vertical size
  unsigned int FrameHeight ;          // picture horizontal size
  unsigned int VideoBitRate ;
  unsigned int AspectRatio ;          // enum aspect ratio information

} VIDEO_DEC_SEG_INFO ;


/**
 * \defgroup   Sub-picture Decoder in-band command definitions.
 * \@{
 */

/**
	\brief To specify the default 16-color sub-picture palette for
 *       sub-picture display
 */

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int color[16] ;
} SUBPIC_DEC_PALETTE ;


/**
	\brief To specify the PTS described in the SP_PKT parsed by system software.
  *      Sub-picture decoder will add this PTS to the relative PTM extracted
  *      from the SP_DCSQ_STM field in the SP_DCSQ header to determine the
  *      execution start time of the SP display commands included in the SP_DCSQ.
  *      Refer to DVD Video Specification section 5.4.3.3-(1) for details.
  *
  *      Note: use common PTS structure PTS_INFO
  */


/**
	\brief To specify highligh area and display control:
  *
  * startPTS_H:  High word of the PTS at which to start displaying the highlight area.
  * startPTS_L:  Low  word of the PTS at which to start displaying the highlight area.
  *
  * winPosition: The x and y coordinates of the top-left corner point of the highlight area.
  *              Coordinates are relative to the sub-picture window in the following format:
  *              winPosition = (y << 16) | x ;
  *
  * winSize:     The width and height of the highlight area in units of sub-picture pixels
  *              in following format:
  *              winSize = (width << 16) | height ;
  *
  * alpha:       Alpha of the 4 colors in the highlight color map in the following format:
  *              alpha = (alpha3 << 24) | (alpha2 << 16) | (alpha1 << 8) | alpha0 ;
  *
  * colorMap:    The 2bit-to-4bit highlight colorMap in the following format:
  *              colorMap = (color3 << 24) | (color2 << 16) | (color1 << 8) | color0 ;
  *
  * hlEnable:    set to 1 to activate the highlight area as configured above;
  *              set to 0 to de-activated the highlight area so that only the original
  *              sub-picture is shown.
  */

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int startPTS_H ;
  unsigned int startPTS_L ;
  unsigned int winPosition ;
  unsigned int winSize ;
  unsigned int alpha ;
  unsigned int colorMap ;
  unsigned int hlEnable ;
} SUBPIC_DEC_HIGHLIGHT ;


/** Video Mixer Inband APIs */
/**
	  This spec defines a set of API to send command via cmd fifo.
		Inband command is passed through inband cmd fifo. Each command
		is contained in INBAND_CMD_PKT. Once the system software
		push a INBAND_CMD_PKT in the cmd fifo, it should then update
		the write pointer. System software should take the responsibility
		to keep the cmd fifo from overflow.
	==========================================================================================
	Note (Please read):

	Video Mixer is filter with 2 input pins and one output pin. Its inputs are connected to 2 decoders
	and its output is connected to a VOUT or a Video Encoder.

	Video mixer operation is very simple, it does not have run, stop, or pause. It only have 3 inband commands:
	SET_BG_COLOR, SET_MIXER_RPTS and BLEND.

	SET_BG_COLOR sets background color but it does not cause an action.
	SET_MIXER_RPTS sets mixer relative PTS. All the future BLEND command refer to this RPTS.

	BLEND is the only command that causes actions: blend 1 or 2 stream(s) and/or background
	and send the output video to output pin. Mixer RPTS is counted with mixer output.

	If (alphaA + alphaB) =< 255, A and B will be blended and then blended with backgraoud with (1-alphaA-alphaB).
	If (alphaA + alphaB) > 255, this command will be discarded (illegal command)

	BlendMode selects A only, B only and AB blend.
	For A only mode, only stream A buffer is dequeued and blened with backgroud color if alphaA is not 255.
	For B only mode, only stream B buffer is dequeued and blened with backgroud color if alphaA is not 255.
	For AB only mode, both stream A and stream B buffers are dequeued and blened with backgroud color if (alphaA+alphaB)<255.

	Implementation notes:

	1. Mixte maintains 3 variables: MixerRPTS, OffsetA, OffsetB
		- RPTS is 0 when created
		- OffsetA is updated at the beginning of BlendA or BlendAB command, if the first field/frame of streamA has NEW_SEG mark:
			OffsetA = DecoderRPTSA - MixerRPTS
		- OffsetB is updated at the beginning of BlendB or BlendAB command, if the first field/frame of streamB has NEW_SEG mark:
			OffsetB = DecoderRPTSB - MixerRPTS

	2. MixerPTS is updated in the following cases:
		- When SET_MIXER_RPTS is processed, MixerRPTS is updated by this command.
		- When a field or frame is ouputed, MixerRPTS is updated to: (TargetDecoderRPTS - Offset of that stream)
		- When a blend command is finished (PTSDuration is reached), MixterRPTS is updated to the endingPTS.
			endingPTS is calculated before blending is excuted: endingPTS = MixerPTS + PTSDuration.

	3. A and B each has a receive queue of 2 frames or 4 fields: QA[2] and QB[2]

	3. Blending command may be implemented this way (use top field as example, bot field is the same):

		endingPTS = MixterPTS + PTSDuration;

		switch (blendMode)
		{
		    case A:
		    	if (QA[0].NEW_SEG) OffsetA = QA[0].RPTS - MixerPTS;
			while (1){
			    if ( QA[0].alphaA == 255) output QA[0];
			    else mix (QA[0], BG);
			    MixterPTS = QA[0].RPTS-OffsetA;
			    if (QA[0].EOS) break;
			    QA[0]=QA[1];QA[1]=NULL ( Move QA[1] to QA[0], decoder will fill QA[1] with a new field )
			    if (QA[0].NEW_SEG || QA[0].RPTS-offsetA > endingPTS) {
			    	MixterPTS = endingPTS;
			    	break;
			    }
			}
		    case B:
			if (QB[0].NEW_SEG) OffsetB = QB[0].RPTS - MixerPTS;
			while (1){
			    if ( QB[0].alphaA == 255) output QB[0];
			    else mix (QB[0], BG);
			    MixterPTS = QB[0].RPTS-OffsetA;
			    if (QB[0].EOS) break;
			    QB[0]=QB[1]; QB[1]=NULL; ( Move QB[1] to QB[0], decoder will fill QB[1] with a new field )
			    if (QB[0].NEW_SEG || QB[0].RPTS-offsetA > endingPTS) {
			    	MixterPTS = endingPTS;
			    	break;
			    }
			}
		    case AB:
			if (QA[0].NEW_SEG) OffsetA = QA[0].RPTS - MixterPTS;
			if (QB[0].NEW_SEG) OffsetB = QB[0].RPTS - MixterPTS;
			while (1){
			    if( !QA[0].EOS && QA[1]==NULL) wait for QA[1];
			    if( !QB[0].EOS && QB[1]==NULL) wait for QB[1];
			    convert RPTS (decoder) to MPTS (mixter) for all field by: MPTS = RPTS - offset
		            pick target following MPTS order;
			    if(target = QA[1]) QA[0] = QA[1]; QA[1]=NULL; (remove QA[0]);
			    if(target = QB[1]) QB[0] = QB[1]; QB[1]=NULL; (remove QB[0]);
			    ref = pick the top field in other stream with closest MPTS with target;
			    if(ref = QA[1]) QA[0] = QA[1]; QA[1]=NULL; (remove QA[0]);
			    if(ref = QB[1]) QB[0] = QB[1]; QB[1]=NULL; (remove QB[0]);
			    tmp = mix (target, ref);
			    if (alphA+alphaB<255) mix (tmp,BG);
			    MixterPTS = target.MPTS;
			    if (QA[0].EOS) break;
			    if (QB[0].EOS) break;
			    if (QA[0].MPTS>endingPTS && QB[1].MPTS>endingPTS){
			    	MixterPTS = endingPTS;
			    	break;
			    }
			}
		}

	==========================================================================================
*/


/**
	\brief Set mixer background color
	\param  a 'VIDEO_MIXER_BG_COLOR' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int Y ;
  unsigned int U ;
  unsigned int V ;
} VIDEO_MIXER_BG_COLOR ;


/**
	\brief Set mixer Relative PTS
	\param  a 'VIDEO_MIXER_RPTS' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int PTSH ;
  unsigned int PTSL ;
} VIDEO_MIXER_RPTS ;


/**
	\brief Alpha blend 1 or 2 streams and/or with background
	\param  a 'VIDEO_MIXER_BLEND' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int PTSDurationH ;	// Duration in PTS
  unsigned int PTSDurationL ;	// Duration in PTS
  unsigned int YalphaA ;	    // Ignored in BlendMode 0, 0 to 255
  unsigned int UValphaA ;	    // Ignored in BlendMode 0, 0 to 255
  unsigned int YalphaB ;	    // Ignored in BlendMode 1, 0 to 255
  unsigned int UValphaB ;	    // Ignored in BlendMode 1, 0 to 255
  unsigned int BlendMode ;	  // May change to enum.
                              // 0: reserved; 1: A only; 2: B only; 3: AB
} VIDEO_MIXER_BLEND ;


/** \@} */


/**
	\brief DV Inband VAUX command
	\param  a 'INBAND_DV_VAUX' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int VS ;
  unsigned int VSC ;
  unsigned int REC_DATE ;
  unsigned int REC_TIME ;
} VIDEO_DV_VAUX ;


/**
	\brief DV Inband VAUX command
	\param  a 'INBAND_DV_VAUX' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int Speed ; /* 0: Normal, 1: Fast-forward or back-word  */
} VIDEO_DV_FF ;

/**
	\brief Set the transport demux program ID
	\param  a 'VIDEO_TSDMX_PID' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int PID ;
} VIDEO_TRANSPORT_DEMUX_PID ;



/**
	\brief Set the transport demux PTS_Offset
	\param  a 'VIDEO_TSDMX_PTS_OFFSET' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int PTSOffsetH ;
  unsigned int PTSOffsetL ;
} VIDEO_TRANSPORT_DEMUX_PTS_OFFSET ;

/**
	\brief Set the transport demux PTS_Offset
	\param  a 'VIDEO_TSDMX_PTS_OFFSET' structure.
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int size ;
} VIDEO_TRANSPORT_DEMUX_PTS_PACKET ;


typedef struct
{
  INBAND_CMD_PKT_HEADER header;
  unsigned int          wPtr ;
  unsigned int          SequenceNum;
  unsigned int          Flags;
  unsigned int          NumSegments;
} VIDEO_RV_FRAME_INFO;

typedef struct
{
  INBAND_CMD_PKT_HEADER header;
  unsigned int          Length;
  unsigned int          MOFTag;
  unsigned int          SubMOFTag;
  unsigned int          Width;
  unsigned int          Height;
  unsigned int          BitCount;
  unsigned int          PadWidth;
  unsigned int          PadHeight;
  unsigned int          FramesPerSecond;
  unsigned int          SPOExtra;
  unsigned int          StreamVersion;
  unsigned int          MajorBitstreamVersion;
  unsigned int          MinorBitstreamVersion;
  unsigned int          NumResampledImageSizes;
  unsigned int          LargestPels;
  unsigned int          LargestLines;
  unsigned int          pDimensions[2*(8+1)];
} VIDEO_RV_FORMAT_INFO;

typedef struct
{
  INBAND_CMD_PKT_HEADER header;
  unsigned int          wPtr ;
  unsigned int          isValid;
  unsigned int          Offset;
} VIDEO_RV_SEGMENT_INFO;

typedef struct
{
	INBAND_CMD_PKT_HEADER header;
	unsigned int          profile;
	unsigned int          displayWidth;
	unsigned int          displayHeight;
	unsigned int          extra_data[32];
}VIDEO_VC1_INBAND_CMD_TYPE_SEQ;

typedef struct
{
       INBAND_CMD_PKT_HEADER header;
        unsigned int          profile;
        unsigned int          displayWidth;
        unsigned int          displayHeight;
        unsigned int          extra_data[128];
}VIDEO_VC1_INBAND_CMD_TYPE_SEQ2;


typedef struct
{
  INBAND_CMD_PKT_HEADER header ;

  /* when any of the following information is unavailable or invalid,
     the corresponding variable must be set to 0xffffffff.
  */
  unsigned int displayRatioHorizontal ; /* horizontal factor of the display aspect ratio */
  unsigned int displayRatioVertical ;   /* vertical   factor of the display aspect ratio */
  unsigned int frameDurationInPTS ;     /* frame rate information evaluated in PTS increment
                                           on consecutive frames, equal to 90000/FPS */

} VIDEO_USABILITY_INFO ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;

  unsigned int pixel_width ;
  unsigned int pixel_height  ;
  unsigned int display_width ;
  unsigned int display_height ;
  unsigned int interlaced ;
  unsigned int frame_rate ;

} VIDEO_MPEG4_USABILITY_INFO ;


typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int width ;
  unsigned int height ;
}VIDEO_FJPEG_RESOLUTION ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int width ;
  unsigned int height ;
}VIDEO_MJPEG_RESOLUTION ;


typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  INBAND_CMD_GRAPHIC_FORMAT format ;
  unsigned int PTSH ;
  unsigned int PTSL ;
  unsigned int context ;  /* system can check it to know which picture displaying on VO */
  int colorkey ;          /* -1: disable colorkey */
  int alpha ;             /*  0: disable constant alpha */
  unsigned int x ;
  unsigned int y ;
  unsigned int width ;
  unsigned int height ;
  unsigned int address ;
  unsigned int pitch ;
  unsigned int address_right ;
  unsigned int pitch_right ;
  INBAND_CMD_GRAPHIC_3D_MODE picLayout ;
  unsigned int dataByte1 ;
  unsigned int dataByte2 ;
  unsigned int dataByte3 ;

} VIDEO_GRAPHIC_PICTURE_OBJECT ;

/*
 (x,y,width,height): the coordinate of this picture on back frame
    lastPicOfFrame : 1: decode this picture then blend to back frame and send it to VO
                     0: ONLY decode this picture
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int bsSize ;
  unsigned int x ;
  unsigned int y ;
  unsigned int width ;
  unsigned int height ;
  unsigned int lastPicOfFrame ;
  unsigned int PTSH ;                /* PTS ONLY be valid while lastPicOfFrame  is 1 */
  unsigned int PTSL ;

} VIDEO_FJPEG_PIC_INFO ;


typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  int displayEachPic ;      /* 1: each picture sent by upstream must be displayed, 0: follow av-sync to drop/repeat picture */

} VIDEO_GRAPHIC_DISPLAY_INFO ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int sequenceID ;   /* 0xff: represent zero-offset */

} SUBP_OFFSET_SEQUENCE_ID ;

#endif
