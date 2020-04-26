#ifndef __VIDEO_DECODER_INBAND_API_H__
#define __VIDEO_DECODER_INBAND_API_H__

/** Video Inband APIs */
/**
	  This spec defines a set of API to send command via cmd fifo.
		Inband command is passed through inband cmd fifo.
		System software should take the responsibility
		to convert and write cmds to the cmd fifo 
		keep the cmd fifo from overflow.
*/

/** Video inband cmd type. I use prefix "VIDEO_DEC_" to label the cmd used in video decoder. */
typedef enum
{
  /* Video Decoder In-band Command */
  VIDEO_DEC_INBAND_CMD_TYPE_PTS = 0,
  VIDEO_DEC_INBAND_CMD_TYPE_PTS_SKIP,
  VIDEO_DEC_INBAND_CMD_TYPE_NEW_SEG,
  VIDEO_DEC_INBAND_CMD_TYPE_SEQ_END,
  VIDEO_DEC_INBAND_CMD_TYPE_EOS,
  VIDEO_DEC_INBAND_CMD_TYPE_CONTEXT,
  VIDEO_DEC_INBAND_CMD_TYPE_OUTPUT_FMT,
  VIDEO_DEC_INBAND_CMD_TYPE_DECODE,
  VIDEO_DEC_INBAND_CMD_TYPE_VOBU,
  VIDEO_DEC_INBAND_CMD_TYPE_DVDVR_DCI_CCI,
  VIDEO_DEC_INBAND_CMD_TYPE_DVDV_VATR,
  
  /* MSG Type for parse mode */
  VIDEO_DEC_INBAND_CMD_TYPE_SEG_INFO,
  VIDEO_DEC_INBAND_CMD_TYPE_PIC_INFO,

  /* Sub-picture Decoder In-band Command */
  SUBPIC_DEC_INBAND_CMD_TYPE_SET_PALETTE,
  SUBPIC_DEC_INBAND_CMD_TYPE_PTS,

  /* Video Mixer In-band Command */
  VIDEO_MIXER_INBAND_CMD_TYPE_SET_BG_COLOR,
  VIDEO_MIXER_INBAND_CMD_TYPE_SET_MIXER_RPTS,
  VIDEO_MIXER_INBAND_CMD_TYPE_BLEND,

} VIDEO_INBAND_CMD_TYPE ;


/* Video Decoder Inband APIs */
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
 * VIDEO_INBAND_CMD_PKT_HEADER *header ;
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
 * VIDEO_INBAND_CMD_PKT_HEADER *header = icq.RdPtr ;
 * unsigned long *cmd ;
 * while( is_this_cmd_shoud_be_processed( header->wPtr))
 * {
 *   cmd = malloc(sizeof( header->size) );
 *   memcpy( cmd, icq.RdPtr + sizeof( VIDEO_INBAND_CMD_PKT_HEADER) ) ;
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
  VIDEO_INBAND_CMD_TYPE type ;
  int  size ;
} VIDEO_INBAND_CMD_PKT_HEADER ;


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
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
  long segID ;
} VIDEO_DEC_NEW_SEG ;
 
/**
	\brief
	\param wPtr
		An coresponding address to bit stream buffer.
*/
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
} VIDEO_DEC_SEQ_END ;

/**
 *  \brief Mark a PTS on an address.
 *  \param:
 */
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
  long PTSH ;
  long PTSL ;
} VIDEO_DEC_PTS_INFO ;

/**
 *  \brief Tell decoder to look at the system PTS (SPTS not RPTS) in shared reference clock.
 *  if (SPTS of current picture + OffsetPTS < current SPTS in ref clock) skip decoding this picture.
 *  This is used to catch the display quickly. OffsetPTS is the estimated latency between decoder and
 *  VOUT with a margin.
 *  \param:
 */
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
  long OffsetPTSH ;
  long OffsetPTSL ;
} VIDEO_DEC_PTS_SKIP ;

/**
	\brief Mark a EOS on an address. 
		This is the last inband command of a segment and is mandatary.
	\param wPtr
*/
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
} VIDEO_DEC_EOS ;

/**
	\brief Send a general purpose context which is associated a wPtr.
	\param: wPtr
*/
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
  long context;
} VIDEO_DEC_CONTEXT ;


/**
	\brief This command is mainly for decoding thumbnail.
*/
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  long colorFormat ;    //0: YUV, 1: RGB8888, 2: RGB565, 3: RGB332
  long pTargetLuma ;    //Linear address of Luman buffer.
  long pTargetChroma ;  //Linear address of Chroma buffer.
  long pitch ;
  long targetRectX ;
  long targetRectY ;
  long targetRectWidth ;
  long targetRectHeight ;
} VIDEO_DEC_OUTFORMAT ;



/**
	\brief This struct indicates when to decode and how long to decode the bitstream.
	To determine the first picture to decode:  
		1. find the first decodable GOP
		2. skip "skip_GOP" GOPs
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
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  long 	RelativePTSH ;
  long 	RelativePTSL ;
  long 	PTSDurationH ;
  long 	PTSDurationL ;
  long 	skip_GOP ;
  long	mode ; // may be changed to enum
} VIDEO_DEC_DECODE ;


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
	VIDEO_INBAND_CMD_PKT_HEADER header ;
	unsigned long wPtr ;
	long startPTM_L ;
	long startPTM_H ;
	long endPTM_L ;
	long endPTM_H ;
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
	VIDEO_INBAND_CMD_PKT_HEADER header ;
	long DCI ;
	long CCI ;
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
	VIDEO_INBAND_CMD_PKT_HEADER header ;
	long V_ATR ;
} VIDEO_DEC_DVDV_VATR ;


/**
 * This is the picture info decoder write to the message ringbuffer when parse mode is turned on.
 */

typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  //long	VIDEO_DEC_INFO_TYPE;  	// enum	
  unsigned long wPtr ;
  long	PTSH;		/* System PTS High */
  long	PTSL;		/* System PTS Low */
  long	RPTSH;		/* Relative PTS High */
  long	RPTSL;		/* Relative PTS Low */
  long	picType;	/* Picture coding type, as defined in video/common/mpeg_def.h: 
				 *		#define I_TYPE 1
				 *		#define P_TYPE 2
				 *		#define B_TYPE 3 */
  long	numOfField;	/* 2 fields or 3 fields */
  long	byteFromNS;	/* Bytes from NEW_SEG */
  long	picSize;	/* Picture size in bytes, including high level headers (SEQ and/or GOP) before and stuffing after.
    				 * from the current header to the next header. */
  long	headSize;	/* High level (SEG and/or GOP) header size, included in picSize, in bytes. 0 means no SEG or GOP header
    				 * before current picture */
} VIDEO_DEC_PIC_INFO ;


typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  //long VIDEO_DEC_INFO_TYPE ;	// enum  	
  unsigned long wPtr ;
  long VideoEncoderType ;     // enum mpeg1 or mpeg2
  long FrameRate ;            // enum video frame rate
  long FrameWidth ;           // picture vertical size
  long FrameHeight ;          // picture horizontal size
  long VideoBitRate ;
  long AspectRatio ;          // enum aspect ratio information 

} VIDEO_DEC_SEG_INFO ;

/**
 * \defgroup   Sub-picture Decoder in-band command definitions.
 * \@{
 */

/**
	\brief This struct is used to pass the default 16-color sub-picture
 *       palette for the sub-picture display
 */

typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  long color[16] ;
} SUBPIC_DEC_PALETTE ;


/**
	\brief This struct is used to pass the default 16-color sub-picture
 *       palette for the sub-picture display
 */

typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  unsigned long wPtr ;
  unsigned long PTSH ;
  unsigned long PTSL ;

} SUBPIC_DEC_PTS ;


/** Video Mixer Inband APIs */
/**
	  This spec defines a set of API to send command via cmd fifo.
		Inband command is passed through inband cmd fifo. Each command
		is contained in VIDEO_INBAND_CMD_PKT. Once the system software
		push a VIDEO_INBAND_CMD_PKT in the cmd fifo, it should then update
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
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  long Y ;
  long U ;
  long V ;
} VIDEO_MIXER_BG_COLOR ;


/**
	\brief Set mixer Relative PTS 
	\param  a 'VIDEO_MIXER_RPTS' structure.
*/
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  long PTSH ;
  long PTSL ;
} VIDEO_MIXER_RPTS ;


/**
	\brief Alpha blend 1 or 2 streams and/or with background 
	\param  a 'VIDEO_MIXER_BLEND' structure.
*/
typedef struct
{
  VIDEO_INBAND_CMD_PKT_HEADER header ;
  long PTSDurationH ;	// Duration in PTS 
  long PTSDurationL ;	// Duration in PTS 
  long YalphaA ;	    // Ignored in BlendMode 0, 0 to 255
  long UValphaA ;	    // Ignored in BlendMode 0, 0 to 255
  long YalphaB ;	    // Ignored in BlendMode 1, 0 to 255
  long UValphaB ;	    // Ignored in BlendMode 1, 0 to 255
  long BlendMode ;	  // May change to enum. 
                      // 0: reserved; 1: A only; 2: B only; 3: AB 
} VIDEO_MIXER_BLEND ;

/** \@} */

 
 
#endif 
