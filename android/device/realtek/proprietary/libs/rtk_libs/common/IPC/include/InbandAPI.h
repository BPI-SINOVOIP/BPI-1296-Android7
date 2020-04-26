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

#define PALETTE_SIZE 16

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

  /*VO receive VP_OBJ_PICTURE_TYPE In-band Command*/
  VIDEO_VO_INBAND_CMD_TYPE_OBJ_PIC,
  VIDEO_VO_INBAND_CMD_TYPE_OBJ_DVD_SP,
  VIDEO_VO_INBAND_CMD_TYPE_OBJ_DVB_SP,
  VIDEO_VO_INBAND_CMD_TYPE_OBJ_BD_SP,
  VIDEO_VO_INBAND_CMD_TYPE_OBJ_SP_FLUSH,
  VIDEO_VO_INBAND_CMD_TYPE_OBJ_SP_RESOLUTION,

  /* VO receive writeback buffers In-band Command */
  VIDEO_VO_INBAND_CMD_TYPE_WRITEBACK_BUFFER,

  /* for VO debug, VO can dump picture */
  VIDEO_VO_INBAND_CMD_TYPE_DUMP_PIC,
  VIDEO_CURSOR_INBAND_CMD_TYPE_PICTURE_OBJECT,
  VIDEO_CURSOR_INBAND_CMD_TYPE_COORDINATE_OBJECT,
  VIDEO_TRANSCODE_INBAND_CMD_TYPE_PICTURE_OBJECT,
  VIDEO_WRITEBACK_INBAND_CMD_TYPE_PICTURE_OBJECT,

  VIDEO_VO_INBAND_CMD_TYPE_OBJ_BD_SCALE_RGB_SP,

// TV code
  VIDEO_INBAND_CMD_TYPE_DIVX_CERTIFY,

  /*M_DOMAIN resolution In-band Command*/
  VIDEO_INBAND_CMD_TYPE_M_DOMAIN_RESOLUTION,

  /* DTV source In-band Command */
  VIDEO_INBAND_CMD_TYPE_SOURCE_DTV,

  /* Din source copy mode In-band Command */
  VIDEO_DIN_INBAND_CMD_TYPE_COPY_MODE,

  /* Video Decoder AU In-band command */
  VIDEO_DEC_INBAND_CMD_TYPE_AU,

  /* Video Decoder parse frame In-band command */
  VIDEO_DEC_INBAND_CMD_TYPE_PARSE_FRAME_IN,
  VIDEO_DEC_INBAND_CMD_TYPE_PARSE_FRAME_OUT,

  /* Set video decode mode In-band command */
  VIDEO_DEC_INBAND_CMD_TYPE_NEW_DECODE_MODE,

  /* Secure buffer protection */
  VIDEO_INBAND_CMD_TYPE_SECURE_PROTECTION,

  /* Dolby HDR inband command */
  VIDEO_DEC_INBAND_CMD_TYPE_DV_PROFILE,

  /* VP9 HDR10 In-band command */
  VIDEO_VP9_INBAND_CMD_TYPE_HDR10_METADATA


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

#define HAVE_PTS_INFO2

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int PTSH ;
  unsigned int PTSL ;
  unsigned int PTSH2 ;
  unsigned int PTSL2 ;
  unsigned int length ;
  unsigned int flag ;
} PTS_INFO2 ;


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

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  unsigned int info ;
} FRM_INFO ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int errNoticeTH ;
  unsigned int errNoticeSpan ;
} ERR_MSG ;

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
  TRANSCODE_PARSING,
  NORMAL_DECODE_MVC,
//TV code
  NORMAL_I_ONLY_DECODE,
  IP_ONLY_DECODE,
  VDEC_DIRECT_DECODE = 16,
  DRIP_I_ONLY_DECODE = 17,
  NORMAL_DECODE_LOWDELAY = 18,
  NO_REF_SYNC_DECODE = 32

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

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int RelativePTSH ;
  unsigned int RelativePTSL ;
  unsigned int PTSDurationH ;
  unsigned int PTSDurationL ;
  unsigned int skip_GOP ;
  DECODE_MODE  mode ;
  unsigned int isHM91 ; /* for HEVC codec version 1: HM91 0: HM10+*/
  unsigned int useAbsolutePTS ; /* 0: relative PTS, 1: absolute PTS */
} DECODE_NEW ;

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
// TV code
	INBAND_SI_TYPE,  /* DTV PVR : If both I & S are found together in one TP, it is marked to 'SI' */
	INBAND_SEQ_TYPE,

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
  INBAND_CMD_GRAPHIC_FORMAT_2BIT              = 0,
  INBAND_CMD_GRAPHIC_FORMAT_4BIT              = 1,
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
  unsigned int afbc ;
  unsigned int afbc_block_split ;
  unsigned int afbc_yuv_transform ;

} VIDEO_GRAPHIC_PICTURE_OBJECT ;

/*
 (x,y,width,height): the coordinate of this picture on back frame
    lastPicOfFrame : 1: decode this picture then blend to back frame and send it to VO
                     0: ONLY decode this picture
*/
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int version;
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
  unsigned int afbc ;
  unsigned int afbc_block_split ;
  unsigned int afbc_yuv_transform ;
  unsigned int partialSrcWin_x;
  unsigned int partialSrcWin_y;
  unsigned int partialSrcWin_w;
  unsigned int partialSrcWin_h;

  unsigned int pFrameBufferDbg;  //rtk1
  unsigned int pFrameBufferDbg_Right;
  
} VIDEO_GRAPHIC_PICTURE_OBJECT_VERSION ;
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

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  //unsigned long long packet[24];
  unsigned int RPTSH;
  unsigned int RPTSL;
  unsigned int Y_addr;
  unsigned int U_addr;
  unsigned int RPTSH2;
  unsigned int width;
  unsigned int height;
  unsigned int Y_pitch;
  unsigned int C_pitch;
  unsigned int PTSH2;
  unsigned int PTSL2;
  unsigned int PTSH;
  unsigned int PTSL;
  unsigned int pRefClock;
  unsigned int pLock;
  unsigned int RPTSL2;
  unsigned int context;
  unsigned int mode;
  unsigned int pixelAR_hor; /* pixel aspect ratio hor*/
  unsigned int pixelAR_ver; /* pixel aspect ratio ver*/
  unsigned int Y_addr_Right;
  unsigned int U_addr_Right;
  unsigned int pLock_Right;
  unsigned int mvc;         /* 1: mvc */
  unsigned int subPicOffset;/* 3D Blu-ray dependent-view sub-picture plane offset metadata as defined in BD spec sec. 9.3.3.6.
                                     Valid only when Y_BufId_Right and C_BufId_Right are both valid */

} VIDEO_VO_PICTURE_OBJECT ;


/****Tch VERSION 1.3(not used anymore)****/
struct tch_metadata_variables {
  int tmInputSignalBlackLevelOffset;
  int tmInputSignalWhiteLevelOffset;
  int shadowGain;
  int highlightGain;
  int midToneWidthAdjFactor;
  int tmOutputFineTuningNumVal;
  int tmOutputFineTuningX[15];
  int tmOutputFineTuningY[15];
  int saturationGainNumVal;
  int saturationGainX[15];
  int saturationGainY[15];
};

struct tch_metadata_tables {
  int luminanceMappingNumVal;
  int luminanceMappingX[33];
  int luminanceMappingY[33];
  int colourCorrectionNumVal;
  int colourCorrectionX[33];
  int colourCorrectionY[33];
  int chromaToLumaInjectionMuA;
  int chromaToLumaInjectionMuB;
};

struct tch_metadata {
  int specVersion;
  int payloadMode;
  int hdrPicColourSpace;
  int hdrMasterDisplayColourSpace;
  int hdrMasterDisplayMaxLuminance;
  int hdrMasterDisplayMinLuminance;
  int sdrPicColourSpace;
  int sdrMasterDisplayColourSpace;
  union
  {
    struct tch_metadata_variables variables;
    struct tch_metadata_tables tables;
  } u;
};
/****Tch VERSION 1.3(not used anymore)****/




/****Tch VERSION 1.4(now used)****/
typedef struct {
  int tmInputSignalBlackLevelOffset;
  int tmInputSignalWhiteLevelOffset;
  int shadowGain;
  int highlightGain;
  int midToneWidthAdjFactor;
  int tmOutputFineTuningNumVal;
  int tmOutputFineTuningX[10];
  int tmOutputFineTuningY[10];
  int saturationGainNumVal;
  int saturationGainX[6];
  int saturationGainY[6];
}tch_sl_hdr_metadata_variables;

typedef struct {
  int luminanceMappingNumVal;
  int luminanceMappingX[65];
  int luminanceMappingY[65];
  int colourCorrectionNumVal;
  int colourCorrectionX[65];
  int colourCorrectionY[65];
}tch_sl_hdr_metadata_tables;

typedef struct  {
  int partID;
  int majorSpecVersionID;
  int minorSpecVersionID;
  int payloadMode;
  int hdrPicColourSpace;
  int hdrDisplayColourSpace;
  int hdrDisplayMaxLuminance;
  int hdrDisplayMinLuminance;
  int sdrPicColourSpace;
  int sdrDisplayColourSpace;
  int sdrDisplayMaxLuminance;
  int sdrDisplayMinLuminance;
  int matrixCoefficient[4];
  int chromaToLumaInjection[2];
  int kCoefficient[3];
  union
  {
     tch_sl_hdr_metadata_variables variables;
     tch_sl_hdr_metadata_tables tables;
  } u;
}tch_sl_hdr_metadata;



typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int version;
  
  unsigned int mode;
  unsigned int Y_addr;
  unsigned int U_addr;
  unsigned int pLock;  
  unsigned int width;
  unsigned int height;
  unsigned int Y_pitch;
  unsigned int C_pitch;
    
  unsigned int RPTSH;
  unsigned int RPTSL;  
  unsigned int PTSH;
  unsigned int PTSL;
  
  /* for send two interlaced fields in the same packet, 
  valid only when mode is INTERLEAVED_TOP_BOT_FIELD or INTERLEAVED_BOT_TOP_FIELD*/
  unsigned int RPTSH2; 
  unsigned int RPTSL2;    
  unsigned int PTSH2;
  unsigned int PTSL2;
  
  unsigned int context;  
  unsigned int pRefClock;  /* not used now */

  unsigned int pixelAR_hor; /* pixel aspect ratio hor, not used now */
  unsigned int pixelAR_ver; /* pixel aspect ratio ver, not used now */
  
  unsigned int Y_addr_Right; /* for mvc */
  unsigned int U_addr_Right; /* for mvc */
  unsigned int pLock_Right; /* for mvc */
  unsigned int mvc;         /* 1: mvc */
  unsigned int subPicOffset;/* 3D Blu-ray dependent-view sub-picture plane offset metadata as defined in BD spec sec. 9.3.3.6.
                                     Valid only when Y_BufId_Right and C_BufId_Right are both valid */
  unsigned int pReceived;         // fix bug 44329 by version 0x72746B30 'rtk0'
  unsigned int pReceived_Right;   // fix bug 44329 by version 0x72746B30 'rtk0'
  unsigned int fps;  // add  framrate by version 0x72746B31
  unsigned int IsForceDIBobMode; //force vo use bob mode to do deinterlace.  0x72746B32  'rtk2'
  unsigned int lumaOffTblAddr;    // 'rtk3'
  unsigned int chromaOffTblAddr;  // 'rtk3'
  unsigned int lumaOffTblAddrR; /* for mvc, 'rtk3' */
  unsigned int chromaOffTblAddrR; /* for mvc, 'rtk3' */
  unsigned int bufBitDepth;   // 'rtk3'
  unsigned int bufFormat;     // 'rtk3', 0:FORMAT_420, 19:FORMAT_YVYU_P10_16BIT_MSB, 22:FORMAT_YVYU_P10_32BIT_LSB
	// VUI (Video Usability Information)
  unsigned int transferCharacteristics;   // 0:SDR, 1:HDR, 2:ST2084, 'rtk3'
  // Mastering display colour volume SEI, 'rtk3'
  unsigned int display_primaries_x0;
  unsigned int display_primaries_y0;
  unsigned int display_primaries_x1;
  unsigned int display_primaries_y1;
  unsigned int display_primaries_x2;
  unsigned int display_primaries_y2;
  unsigned int white_point_x;
  unsigned int white_point_y;
  unsigned int max_display_mastering_luminance;
  unsigned int min_display_mastering_luminance;

  /*for transcode interlaced feild use.*/  //'rtk4'
  unsigned int Y_addr_prev;
  unsigned int U_addr_prev;
  unsigned int Y_addr_next;
  unsigned int U_addr_next;
  unsigned int video_full_range_flag; //'rtk4'
  unsigned int matrix_coefficients; //'rtk4

  /*for transcode interlaced feild use.*/  //'rtk5'
  unsigned int pLock_prev;
  unsigned int pReceived_prev;
  unsigned int pLock_next;
  unsigned int pReceived_next;
  /*for technicolor prime used*/
  unsigned int is_tch_video;//'rtk6'
  struct tch_metadata tch_hdr_metadata;  //'rtk6'
  unsigned int pFrameBufferDbg;  //rtk7
  unsigned int pFrameBufferDbg_Right;
  unsigned int Y_addr_EL; //'rtk8' for dolby vision
  unsigned int U_addr_EL;
  unsigned int width_EL;
  unsigned int height_EL;
  unsigned int Y_pitch_EL;
  unsigned int C_pitch_EL;
  unsigned int lumaOffTblAddr_EL;
  unsigned int chromaOffTblAddr_EL;

  unsigned int dm_reg1_addr;
  unsigned int dm_reg1_size;
  unsigned int dm_reg2_addr;
  unsigned int dm_reg2_size;
  unsigned int dm_reg3_addr;
  unsigned int dm_reg3_size;
  unsigned int dv_lut1_addr;
  unsigned int dv_lut1_size;
  unsigned int dv_lut2_addr;
  unsigned int dv_lut2_size;

  unsigned int slice_height; //for compressed mode alignment

  unsigned int hdr_metadata_addr;//'rtk9'
  unsigned int hdr_metadata_size;//'rtk9'
  unsigned int tch_metadata_addr;//'rtk9'
  unsigned int tch_metadata_size;//'rtk9'
  
  unsigned int is_dolby_video;//'rtk10'
   
  unsigned int lumaOffTblSize;    // 'rtk11'
  unsigned int chromaOffTblSize;  // 'rtk11'
  
  
  unsigned int combine_mode_Y_address;  // 'rtk12'  for compressed mode scale workaroun
  unsigned int combine_mode_C_address;
  unsigned int combine_mode_width;
  unsigned int combine_mode_height;
  unsigned int combine_mode_pitch;
  unsigned int secure_flag;            //for secure usage.
  
   unsigned int tvve_picture_width;//rtk 13 TVVE   for vo calculate header pitch
  unsigned int tvve_lossy_en; 
  unsigned int tvve_bypass_en;
  unsigned int tvve_qlevel_sel_y;
  unsigned int tvve_qlevel_sel_c;
  
  unsigned int is_ve_tile_mode;  //cnm tile mode
  unsigned int film_grain_metadat_addr;
  unsigned int film_grain_metadat_size;
} VIDEO_VO_PICTURE_OBJECT_NEW ;



typedef struct {
  unsigned int transferCharacteristics; // 0:SDR, 1:HDR, 2:ST2084, 'rtk3'
  unsigned int video_full_range_flag;
  unsigned int matrix_coefficients;
  unsigned int display_primaries_x0;
  unsigned int display_primaries_y0;
  unsigned int display_primaries_x1;
  unsigned int display_primaries_y1;
  unsigned int display_primaries_x2;
  unsigned int display_primaries_y2;
  unsigned int white_point_x;
  unsigned int white_point_y;
  unsigned int max_display_mastering_luminance;
  unsigned int min_display_mastering_luminance;
  unsigned int max_content_light_level;
  unsigned int max_frame_avg_light_level;

}VIDEO_VO_SVP_HDR_METADATA;


typedef struct {
  unsigned int transferCharacteristics; // 0:SDR, 1:HDR, 2:ST2084, 'rtk3'
  unsigned int video_full_range_flag;
  unsigned int matrix_coefficients;
  unsigned int display_primaries_x0;
  unsigned int display_primaries_y0;
  unsigned int display_primaries_x1;
  unsigned int display_primaries_y1;
  unsigned int display_primaries_x2;
  unsigned int display_primaries_y2;
  unsigned int white_point_x;
  unsigned int white_point_y;
  unsigned int max_display_mastering_luminance;
  unsigned int min_display_mastering_luminance;
  unsigned int max_content_light_level;
  unsigned int max_frame_avg_light_level;
  unsigned int application_version;
  unsigned int average_maxrgb;
  unsigned int distribution_values[9];
  unsigned int num_bezier_curve_anchors;
  unsigned int knee_point_x ;
  unsigned int knee_point_y;
  unsigned int bezier_curve_anchors[9];

}VIDEO_VO_SVP_HDR_METADATA_HDR10PLUS;


typedef struct  {
    unsigned int context;
    unsigned int receive_time_H; // 90K
    unsigned int receive_time_L; // 90K
    unsigned int display_time_H; // 90K
    unsigned int display_time_L; // 90K
    unsigned int release_time_H; // 90K
    unsigned int release_time_L; // 90K
    unsigned int drop_cnt;
    unsigned int display_cnt;
    unsigned int reserve[64 - 9];
}VO_FRAMEBUFFER_DBG_INFO;

enum wb_targetFormat
{
  WB_FORMAT_NV21_MASK = 0x00000001,
  WB_FORMAT_422_MASK  = 0x00000002,
  WB_FORMAT_10BITS_MASK = 0x00000004,
  WB_FORMAT_MODE10B_MASK= 0x00000008,
  WB_USE_V1_MASK = 0x00000010,
  WB_MIX1_MASK = 0x00000020
};

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int version;//from 'TRA1'=0x54524131
 
  /* ID info */
  unsigned int agentID_H;
  unsigned int agentID_L;
  unsigned int bufferID_H;
  unsigned int bufferID_L;
  
  /* source pic info */
  unsigned int mode;
  unsigned int Y_addr;
  unsigned int U_addr;  
  unsigned int width;
  unsigned int height;
  unsigned int Y_pitch;
  unsigned int C_pitch;
 
  unsigned int lumaOffTblAddr;
  unsigned int chromaOffTblAddr;
  unsigned int bufBitDepth;
  unsigned int bufFormat; 
 
  unsigned int Y_addr_prev;
  unsigned int U_addr_prev;
  unsigned int Y_addr_next;
  unsigned int U_addr_next;
  
  /* target pic info */
  unsigned int wb_y_addr;
  unsigned int wb_c_addr;  
  unsigned int wb_w;
  unsigned int wb_h;
  unsigned int wb_pitch;
  unsigned int targetFormat;
  //see enum wb_targetFormat...
  //bit 0=>NV21, 1:NV21, 0:NV12;
  //bit 1=>422, 1:422, 0:420
  //bit 2=>bit depth, 1:10 bits, 0: 8 bits;
  //bit 3=>mode_10b, 0: use 2 bytes to store 1 components. MSB justified. 1: use 4 bytes to store 3 components, LSB justified.
  //bit 4=>wb_use_v1: config vo use which plane to do transcode, 1:V1, 0:V2
  //bit 5=>wb_mix1: transcode content after mixer 1(OSD+V1+...), 1:mixer 1, 0: V1/V2 only

  /* from 'TRA2'=0x54524132
  modify V2 color for every pic:
  valid in [0, 64] with 0 being the weakest and 64 being the strongest. Default value is 32. */
  unsigned int    contrast ;
  unsigned int    brightness ;
  unsigned int    hue ;
  unsigned int    saturation ;
 
 /*from 'TRA3'=0x54524133 for sharpness setting*/
  unsigned int    sharp_en;
  unsigned int    sharp_value;
  
    /*from 'TRA4'=0x54524134 for crop, default x=y=w=h =0*/
 unsigned int    crop_x;
  unsigned int    crop_y;
 unsigned int    crop_width;
 unsigned int    crop_height;
  
} VIDEO_TRANSCODE_PICTURE_OBJECT;
 
typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int version;//from 'WBK1'=0x57424B31
 
  /* ID info */
  unsigned int agentID_H;
  unsigned int agentID_L;
  unsigned int bufferID_H;
  unsigned int bufferID_L;
 
  /* return status */
  unsigned int success;
 
  /* wb pic info */
  unsigned int mode;
  unsigned int Y_addr;
  unsigned int U_addr;
  unsigned int width;
  unsigned int height;
  unsigned int Y_pitch;
  unsigned int C_pitch;
  unsigned int bufBitDepth;
  unsigned int bufFormat;
 
} VIDEO_WRITEBACK_PICTURE_OBJECT ;


typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int context ;  /* system can check it to know which picture displaying on VO */
  unsigned int width ;
  unsigned int height ;
  INBAND_CMD_GRAPHIC_FORMAT colorFmt ;//2-bit or 4-bit
  unsigned int address ;
  unsigned int address_right ;
  unsigned int clut_address ;
  unsigned int PTSH ;
  unsigned int PTSL ;
  unsigned int reserved[4];
} VIDEO_CSR_PICTURE_OBJECT ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int hide;
  unsigned int context ;  /* system can check it to know which picture displaying on VO */
  unsigned int x ;
  unsigned int y ;
  unsigned int PTSH ;
  unsigned int PTSL ;
  unsigned int reserved[4];
} VIDEO_CSR_COORDINATE_OBJECT ;

enum
{
    RVSD_FRAME_RATE_UNKNOWN = 0,
    RVSD_FRAME_RATE_23_98_FPS,
    RVSD_FRAME_RATE_24_FPS,
    RVSD_FRAME_RATE_25_FPS,
    RVSD_FRAME_RATE_29_97_FPS,
    RVSD_FRAME_RATE_30_FPS,
    RVSD_FRAME_RATE_50_FPS,
    RVSD_FRAME_RATE_59_94_FPS,
    RVSD_FRAME_RATE_60_FPS,
    RVSD_FRAME_RATE_ERROR

};

typedef struct {
  INBAND_CMD_PKT_HEADER header ;
  unsigned int bufferNum;
  unsigned int bufferId;
  unsigned int bufferSize;
  unsigned int addrY;
  unsigned int addrC;
  unsigned int pitchY;
  unsigned int pitchC;  //version >='rtk3': use height = pitchC ; if version <'rtk3', height =bufferSize/pitchY;
  unsigned int version;//version='rtk0' has pLock, pReceived; 'rtk1' has targetFormat; 'rtk2' has width;
  unsigned int pLock;
  unsigned int pReceived;
  unsigned int targetFormat;
  //see enum wb_targetFormat...
  //bit 0=>NV21, 1:NV21, 0:NV12;
  //bit 1=>422, 1:422, 0:420;
  //bit 2=>bit depth, 1:10 bits, 0: 8 bits;
  //bit 3=>mode_10b, 0: use 2 bytes to store 1 components. MSB justified. 1: use 4 bytes to store 3 components, LSB justified;
  //bit 4=>wb_use_v1: config vo use which plane to do transcode, 1:V1, 0:V2
  //bit 5=>wb_mix1: transcode content after mixer 1(OSD+V1+...), 1:mixer 1, 0: V1/V2 only
  unsigned int width;
  unsigned int reserved[2];
} VIDEO_VO_WB_PICTURE_OBJECT;


typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int   PTSH ;
  unsigned int   PTSL ;

  unsigned int   pPxd_poolId_addr ;
  unsigned int   pCmd_poolId_addr ;

  unsigned int   topPxd_Addr ;
  unsigned int   botPxd_Addr ;
  unsigned int   cmd_Addr ;
  unsigned int   top_num ;   // number of LN_CTLI in the top    field
  unsigned int   bot_num ;   // number of LN_CTLI in the bottom field
  unsigned int   ctrl_cmd ;  // DCCMD, bit 0 is FSTA_DSP, bit 1 is STA_DSP ...etc
                             // bit  8: highlight enable
                             // bit  9: highlight disable
                             // bit 10: palette command

  unsigned int   dispWin_x ;
  unsigned int   dispWin_y ;
  unsigned int   dispWin_width ;
  unsigned int   dispWin_height ;

  unsigned int   dftColor_px2;
  unsigned int   dftColor_px1;
  unsigned int   dftColor_pat;
  unsigned int   dftColor_bg;

  unsigned int   dftContrast_px2;
  unsigned int   dftContrast_px1;
  unsigned int   dftContrast_pat;
  unsigned int   dftContrast_bg;

  unsigned int   hlColor_px2;
  unsigned int   hlColor_px1;
  unsigned int   hlColor_pat;
  unsigned int   hlColor_bg;

  unsigned int   hlContrast_px2;
  unsigned int   hlContrast_px1;
  unsigned int   hlContrast_pat;
  unsigned int   hlContrast_bg;

  unsigned int   hlWindow_x ;
  unsigned int   hlWindow_y ;
  unsigned int   hlWindow_width ;
  unsigned int   hlWindow_height ;

  unsigned int   palette[PALETTE_SIZE] ;

  unsigned int   pLockAddr;    /* for blocking, when subtitle-decoder send decoded-object to VO. */
} VIDEO_VO_OBJ_DVD_SP ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int   time_out ;     /* in seconds */
  unsigned int   pixel_depth ;  /* 1: 2-bit; 2: 4-bit; 3: 8-bit */
  unsigned int   st_mode ;      /* 0: block mode; 1: sequential mode */
  unsigned int   last_in_page ; /* 1 if the region is the last one in the region list of a display page */

  unsigned int   start_x ;
  unsigned int   start_y ;
  unsigned int   width ;
  unsigned int   height ;
  unsigned int   pitch ;
  unsigned int   pixel_addr ;  /* absolute DRAM address of the first pixel */
  unsigned int   pCLUT ;       /* receiver must copy the CLUT contents upon receival of this object */
  unsigned int   pbuffer ;

  unsigned int   video_width ; /* fullWidth and fullHeight for BD subtitle */
  unsigned int   video_height ;
  unsigned int   PTSH ;
  unsigned int   PTSL ;
  unsigned int   pLockAddr;    /* for blocking, when subtitle-decoder send decoded-object to VO. */
} VIDEO_VO_OBJ_DVB_SP_REGION ;

/* sp set resolution by VO inband queue (old) */
typedef struct {
  INBAND_CMD_PKT_HEADER header ;
  long spFullWidth;
  long spFullHeight;
  unsigned int   pLockAddr;    /* for blocking, when subtitle-decoder send decoded-object to VO. */
} VIDEO_RPC_SP_CONFIG_RESOLUTION;

/* for VO debug, VO can dump picture */
typedef struct  {
    unsigned int bDumpEn;
    unsigned int bTerminate;
    unsigned int bDumpPicDone;
    unsigned int pDumpAddrPhy;
    unsigned int size;
} VO_DUMP_DATA;

/* for VO debug, VO can dump picture */
typedef struct  {
    INBAND_CMD_PKT_HEADER header ;
    unsigned int pShareMem;
    unsigned int Reserved[8];
} VO_DUMP_DATA_HANDSHAKE;

enum{
  ENUM_SP_LOCK = 1,
  ENUM_SP_UNLOCK = 2,
};

enum{
    ENUM_IPC_RVSD_VIDEO_RESET = 0x15,
    ENUM_IPC_RVSD_VIDEO_INIT = 0x16,
    ENUM_IPC_RVSD_VIDEO_UNINIT = 0x17,
    ENUM_IPC_RVSD_VIDEO_REF_CLOCK = 0x20,
    ENUM_IPC_RVSD_VIDEO_BS_HANDLER = 0x21,
    ENUM_IPC_RVSD_VIDEO_INBAND_CMD = 0x22,
    ENUM_IPC_RVSD_VIDEO_FLUSH = 0x23,
    ENUM_IPC_RVSD_VIDEO_PAUSE = 0x24,
    ENUM_IPC_RVSD_VIDEO_RUN = 0x25,
    ENUM_IPC_RVSD_VIDEO_SEEK = 0x26,
    ENUM_IPC_RVSD_VIDEO_GET_SEQUENCE_INFO = 0x27,
    ENUM_IPC_RVSD_VIDEO_VO_INSTANCE_ID = 0x28,
    ENUM_IPC_RVSD_VIDEO_STOP = 0x29,
    ENUM_IPC_RVSD_VIDEO_ROTATE = 0x2a,
    ENUM_IPC_RVSD_AUDIO_ION_HANDLER = 0x30,
    ENUM_IPC_RVSD_AUDIO_BS_HANDLER = 0x31,
    ENUM_IPC_RVSD_AUDIO_INBAND_CMD = 0x32,
    ENUM_IPC_RVSD_VIDEO_PRIVATE_INFO = 0x33,
    ENUM_IPC_RVSD_VIDEO_ERROR_CONCEALMENT = 0x34,
    ENUM_IPC_RVSD_VIDEO_BS_VALIDATION = 0x35,
    ENUM_IPC_RVSD_AUDIO_INIT = 0x36,
    ENUM_IPC_RVSD_AUDIO_PRIVATE_INFO = 0x37,
    ENUM_IPC_RVSD_AUDIO_FLUSH = 0x38,
    ENUM_IPC_RVSD_VIDEO_PRE_PARSE_SEQUENCE_INFO = 0x39,
    ENUM_IPC_RVSD_SP_CREATE = 0x50,
    ENUM_IPC_RVSD_SP_INIT_RING_BUF = 0x51,
    ENUM_IPC_RVSD_SP_DESTROY = 0x52,
    ENUM_IPC_RVSD_SP_STOP = 0x53,
    ENUM_IPC_RVSD_SP_PAUSE = 0x54,
    ENUM_IPC_RVSD_SP_RUN = 0x55,
    ENUM_IPC_RVSD_SP_FLUSH = 0x56,
    ENUM_IPC_RVSD_SP_ENABLE = 0x57,
    ENUM_IPC_RVSD_SP_SHOW = 0x58,
    ENUM_IPC_RVSD_SP_HIDE = 0x59,
    ENUM_IPC_RVSD_SP_CONFIG = 0x5a,
    ENUM_IPC_RVSD_TTAK_KO_CC = 0x5b,
    ENUM_IPC_RVSD_VIDEO_INIT_MPEG2_CC_RING_BUF_SECURITY = 0x5c,
    ENUM_IPC_RVSD_VIDEO_INIT_MPEG2_CC_RING_BUF = 0x5d,
    ENUM_IPC_RVSD_VIDEO_SCALE = 0x5e,
    ENUM_IPC_RVSD_VIDEO_DYNAMIC_SCALE_ON = 0x60,
    ENUM_IPC_RVSD_VIDEO_DYNAMIC_SCALE_OFF = 0x61,
    ENUM_IPC_RVSD_VIDEO_INIT_H264_CC_RING_BUF = 0x62,
    ENUM_IPC_RVSD_VIDEO_INIT_H265_CC_RING_BUF = 0x63,
    ENUM_IPC_VOMW_CREATE = 0x64,
    ENUM_IPC_VOMW_RUN = 0x65,
    ENUM_IPC_VOMW_FLUSH = 0x66,
    ENUM_IPC_VOMW_STOP = 0x67,
    ENUM_IPC_VOMW_DESTROY = 0x68,
    ENUM_IPC_RVSD_VIDEO_DESTROY = 0x70,
};

//TV code


typedef struct
{
  INBAND_CMD_PKT_HEADER header;
  unsigned int wPtr ;
  unsigned int packetStartOffset;
} VIDEO_DEC_PARSE_FRAME_IN;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  INBAND_PIC_TYPE  pictureType;
  unsigned int     packetStartOffset;
  unsigned int     picStartOffset;
} VIDEO_DEC_PARSE_FRAME_OUT;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int wPtr ;
  DECODE_MODE  mode ;
} VIDEO_DECODE_MODE ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;
  unsigned int secure ;
} VIDEO_SECURE_PROTECTION ;

typedef struct
{
  INBAND_CMD_PKT_HEADER header ;

  unsigned int BitsPerChannel;
  unsigned int ChromaSubsamplingHorz;
  unsigned int ChromaSubsamplingVert;
  unsigned int CbSubsamplingHorz;
  unsigned int CbSubsamplingVert;
  unsigned int ChromaSitingHorz;
  unsigned int ChromaSitingVert;

  unsigned int primaries_;
  unsigned int transfer_;
  unsigned int matrix_;
  unsigned int range_;
  unsigned int custom_primary_matrix_[12];

  unsigned int primary_r_chromaticity_x;
  unsigned int primary_r_chromaticity_y;
  unsigned int primary_g_chromaticity_x;
  unsigned int primary_g_chromaticity_y;
  unsigned int primary_b_chromaticity_x;
  unsigned int primary_b_chromaticity_y;
  unsigned int white_point_chromaticity_x;
  unsigned int white_point_chromaticity_y;
  unsigned int luminance_max;
  unsigned int luminance_min;

  unsigned int max_cll;
  unsigned int max_fall;

} HDR10_METADATA ;

/*
 * VCPU Flash output farme info data structure
 * Size = 256 bytes
 * */
typedef struct
{
  unsigned int nSize;
  unsigned int nVersion;
  unsigned int pUserData;
  unsigned int nRRKey;
  unsigned int nContext;  
  unsigned int nBufID;
  unsigned int nPicFlags;
  unsigned int nPicWidth;
  unsigned int nPicHeight;
  unsigned int nDecimatePicWidth;
  unsigned int nDecimateicHeight;
  unsigned int nBitDepthLuma;
  unsigned int nBitDepthChroma;
  
  unsigned int nPtsHigh;
  unsigned int nPtsLow;
  unsigned int nRPtsHigh;
  unsigned int nRPtsLow;
  unsigned int nPts2High;
  unsigned int nPts2Low;
  unsigned int nPicPhysicalAddr;
  unsigned int nPicPitch;
  unsigned int nClkTimeHigh;
  unsigned int nClkTimeLow;
  unsigned int nFramerateD;
  unsigned int nFramerateN;
  unsigned int eScanType;              /* 0: progressive, 1: interlaced */
  /* nInterlaceMode define  */
  //typedef enum
  //{
  //  INTERLEAVED_TOP_FIELD = 0,  /* top    field data stored in even lines of a frame buffer */
  //  INTERLEAVED_BOT_FIELD,      /* bottom field data stored in odd  lines of a frame buffer */
  //  CONSECUTIVE_TOP_FIELD,      /* top    field data stored consecutlively in all lines of a field buffer */
  //  CONSECUTIVE_BOT_FIELD,      /* bottom field data stored consecutlively in all lines of a field buffer */
  //  CONSECUTIVE_FRAME,           /* progressive frame data stored consecutlively in all lines of a frame buffer */
  //  INTERLEAVED_TOP_FIELD_422,  /* top    field data stored in even lines of a frame buffer */
  //  INTERLEAVED_BOT_FIELD_422,      /* bottom field data stored in odd  lines of a frame buffer */
  //  CONSECUTIVE_TOP_FIELD_422,      /* top    field data stored consecutlively in all lines of a field buffer */
  //  CONSECUTIVE_BOT_FIELD_422,      /* bottom field data stored consecutlively in all lines of a field buffer */
  //  CONSECUTIVE_FRAME_422,        /* progressive frame with 4:2:2 chroma */
  //  TOP_BOTTOM_FRAME,            /* top field in the 0~height/2-1, bottom field in the height/2~height-1 in the frame */
  //  UNPAIRED_INTERLEAVED_TOP_FIELD,
  //  UNPAIRED_INTERLEAVED_BOT_FIELD,
  //} VP_PICTURE_MODE ;
  unsigned int nInterlaceMode;
  unsigned int nSeiPtsHigh;            /* SEI PTS High */
  unsigned int nSeiPtsSLow;            /* SEI PTS Low */
  unsigned int nPicCPitch;
  unsigned int nPicCPhysicalAddr;
  unsigned int nSampleWidth;
  unsigned int nSampleHeight;
  unsigned int qlevel_sel_y;
  unsigned int qlevel_sel_c;
  /* nHDR_Type define
  typedef enum
  {
      VP_HDR_FORMAT_INVALID = -1,
      VP_HDR_FORMAT_SDR = 0,
      VP_HDR_FORMAT_SDR_2020 = 1,   
      VP_HDR_FORMAT_DOLBY = 2,
      VP_HDR_FORMAT_HDR10_PLUS = 3,
      VP_HDR_FORMAT_TC = 4,
      VP_HDR_FORMAT_HDR10 = 5,
      VP_HDR_FORMAT_HLG = 6,    
  } VP_HDR_FORMAT ; */
  unsigned int nHDR_Type;
  unsigned int nDisplayPrimaries_X[3];
  unsigned int nDisplayPrimaries_Y[3];
  unsigned int nWhitePoint_X;
  unsigned int nWhitePoint_Y;
  unsigned int nMaxDisplayMasteringLuminance;
  unsigned int nMinDisplayMasteringLuminance;
  unsigned int nTransferCharacteristics;
  unsigned int nMatrixCoefficiets;
  unsigned int nVideoFullRangeFlag;
  unsigned int hdr_metadata_addr;
  unsigned int hdr_metadata_size;
  unsigned int tch_metadata_addr;
  unsigned int tch_metadata_size;
  unsigned int film_grain_metadata_addr;
  unsigned int film_grain_metadata_size;
  
  /* 0: raw, 1: lossless, 2: lossy */
  unsigned int nCmprsMode;
  unsigned int nPicYCmprsHdrAddr;
  unsigned int nPicCCmprsHdrAddr;
  unsigned int nPicCmprsPitch;
  unsigned int nPicCPhysicalAddr2;
  
  unsigned int nBufLockPhysicalAddr;
  unsigned int max_fb_num;
  unsigned int max_cmprs_head_size;
  unsigned int reserved[1];
} vrpc_flash_frame_info_t;
#endif
