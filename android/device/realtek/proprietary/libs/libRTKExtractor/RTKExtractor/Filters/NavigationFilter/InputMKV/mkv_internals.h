#ifndef __MKV_INPUT_PLUGIN_INTERNAL_HEADER__
#define __MKV_INPUT_PLUGIN_INTERNAL_HEADER__

#include <OSAL/Types.h>
#include <Filters/NavigationFilter/NavDef.h>
#include <Filters/PrivateInfo.h>
#include <FlowEvent.h>
#include <StreamClass/EType.h>
#include <stdio.h> //for debug
#include <sys/time.h>

#include "mmreg.h"

#ifdef DIVX_PLUS
#include "DrmApi.h"
#include "DrmTypes.h"
#endif

#undef RDDEBUG

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))


#define MKV_IO_SIZE			(256*1024)

#define AUDIO_DEINTERLEAVE_SIZE (32*1024)

#define MKV_IO_ALIGN_MASK	0xfffffffffffff000LL
#define MKV_IO_ALIGN_UNIT	(~MKV_IO_ALIGN_MASK)


#if 1//#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
#define MKV_CMD_SIZE		(64*1024)
#else
#define MKV_CMD_SIZE		(1024)
#endif

//#define MKV_BUFSIZE 		(128*1024)
#define MKV_BUFSIZE 		(256*1024)

#define AUDIO_PRELOAD_PTS       90000

//for index of index
#define INDEXING_SIZE           3*60
#define INDEX_PTS_DISTANCE      60*90000

/*DivX ultra define junk*/
#define MENU_MEMORY_POOL_SIZE (40*1024)
#define ALIGMENT              8

#define FWD_IONLY_SPEED 256
#define RWD_IONLY_SPEED 0

/* private table definition */
#define PENDING_AUDIO_FORMAT			(1LL << 0)
#define PENDING_VIDEO_FORMAT			(1LL << 1)
#define PENDING_SPIC_FORMAT				(1LL << 2)
#define PENDING_AUDIO_VORBIS_BS_TWEAK 	(1LL << 3)
#define PENDING_AUDIO_WMA_TWEAK			(1LL << 4)
#define PENDING_DIVX3_TWEAK				(1LL << 5)
#define PENDING_AUDIO_AAC_TWEAK 		(1LL << 6)
#define PENDING_AUDIO_VORBIS_TWEAK 		(1LL << 7)
#define PENDING_AUDIO_FLUSH				(1LL << 8)
#define PENDING_VIDEO_FLUSH				(1LL << 9)
#define PENDING_SPIC_FLUSH				(1LL << 10)
#define PENDING_ALL_FLUSH				(1LL << 11)
#define PENDING_SPIC_CONFIG				(1LL << 12)
#define PENDING_RESET_SPEED				(1LL << 13)
#define PENDING_VIDEO_END_SEQ			(1LL << 14)
//#define PENDING_DCI_CCI					(1LL << 15)
#define PENDING_EOS_RDY					(1LL << 16)
#define PENDING_NOT_AUTHORIZED			(1LL << 17)
#define PENDING_READ_ERROR				(1LL << 18)
#define PENDING_WAIT_APPROVE			(1LL << 19)
#define PENDING_MPEG4_TWEAK				(1LL << 20)
#define PENDING_MPEG2_TWEAK				(1LL << 21)
#define PENDING_SPU_PALETTE				(1LL << 22)
#define PENDING_SPIC_FRAME      		(1LL << 23)
#define PENDING_NOT_AUTH_USER			(1LL << 24)
#define PENDING_RENTAL_EXPIRED			(1LL << 25)
#define PENDING_REQUEST_RENTAL			(1LL << 26)
#define PENDING_AUDIO_FLAC_TWEAK		(1LL << 27)
#define PENDING_REQUEST_TMPPML			(1LL << 28)
#define PENDING_WAIT_TMPPML				(1LL << 29)
#define PENDING_AUDIO_HEADER_STRIPED	(1LL << 30)
#define PENDING_ORDERED_CHAPTER_EOS		(1LL << 31)
#define PENDING_NOTSUPPORT_AUDIO_FORMAT (1LL << 32)
#define PENDING_WAIT_TITLESELECT		(1LL << 33)
#define PENDING_STOP_RDY				(1LL << 34)
#define PENDING_VIDEO_HEADER_STRIPED    (1LL << 35)
//#define PENDING_REQUEST_MULTIEDITION	(1LL << 31)
//#define PENDING_WAIT_MULTIEDITION		(1LL << 32)


#define STATE_RUNNING			0x00000001
#define STATE_PAUSED			0x00000002
#define STATE_INITIALIZED		0x00000004
#define STATE_HEADER_PARSED		0x00000008
#define STATE_MENU_RESUME		0x00010000
#define STATE_STOP_RESUME		0x00020000
#define STATE_IO_ERROR			0x00040000
#define STATE_IO_READ_ERROR		0x00080000



#define	CMD_MKV_NEW_SEGMENT			(1L << 0)
#define	CMD_MKV_DECODE_INIT			(1L << 1)
#define	CMD_MKV_BLEND				(1L << 2)
#define	CMD_MKV_DECODE				(1L << 3)
#define CMD_MKV_VIDEO_FORMAT        (1L << 4)
#define	CMD_MKV_VIDEO_USABILITY_INFO (1L << 5)
#define CMD_MKV_VIDEO_MPEG4_USABILITY_INFO (1L << 6)
#define CMD_MKV_END_OF_SEGMENT      (1L << 7)
#define CMD_MKV_STOP                (1L << 8)
#define CMD_MKV_MEDIA_REMOVED		(1L << 9)
#define CMD_MKV_VOBINFO             (1L << 10)
#define CMD_MKV_RM_FRAMEINFO		(1L << 11)
#define CMD_MKV_RM_SEGMENTINFO		(1L << 12)
#define CMD_MKV_RM_AUDIO_OPAQUE_DATA (1L << 13)
#define CMD_MKV_DRM_DETECTED		(1L << 14)
#define CMD_MKV_VOBU_INFO			(1L << 15)
#define CMD_MKV_AVSYNC_CHANGE		(1L << 16)
#define CMD_MKV_DCI_CCI				(1L << 17)
#define CMD_MKV_NEW_VIDEO_FORMAT	(1L << 18)




#define MAX_CLUSTER_INDEX 256

//#define MKV_RETRIAL_LIMIT	10
#define MKV_IOERROR_LIMIT	64
#define MKV_READERROR_LIMIT 20

#ifndef IS_SET
		#define IS_SET(x,y)		((x) & (y))
		#define ClearFlag(x,y)	((x) &= ~(y))
		#define SetFlag(x,y)	((x) |= (y))
#endif

//return code
#define RET_OK     0 
#define RET_FAIL   -1
#define RET_IGNORE 1

//////////////////////////Matroska File Structure/////////////////////////////
#define	EBML_VERSION	      1
#define	EBML_MAX_ID_LENGTH    4
#define	EBML_MAX_SIZE_LENGTH  8
#define	MATROSKA_VERSION      2
#define	MATROSKA_DOCTYPE      "matroska"
#define WEBM_DOCTYPE		  "webm"

//Track type
#define UNDEF_TRACK           0x00
#define VIDEO_TRACK           0x01
#define AUDIO_TRACK           0x02
#define COMPLEX_TRACK         0x03 
#define LOGO_TRACK            0x10
#define SUBTITLE_TRACK        0x11
#define BUTTON_TRACK          0x12
#define CONTROL_TRACK         0x20

#define	STRING_MAX            1023
// It will crash wehn TRACKS_MAX over 22
#define	TRACKS_MAX	          127 // 20 //127
#define	TRACKS_MIN            3
#define	READAHEAD_MAX	      (256*1024)

#define	MAXCLUSTER	          (64*1048576)
#define	MAXFRAME	          (4*1048576)

#define	COMP_ZLIB   0
#define	COMP_BZIP   1
#define	COMP_LZO1X  2
#define	COMP_PREPEND 3

//#define MKVDEBUG(fmt,args...)  ALOGE(fmt,##args)
#define MKVDEBUG(fmt,args...)


typedef uint64_t      UINT;
typedef int64_t       INT;
typedef double        FLOAT;
//typedef unsigned char BYTE;

//Level 0
typedef struct __EBML
{
	UINT EBMLVersion;        //The version of EBML parser used to create the file
	UINT EBMLReadVersion;    //The minimum EBML version a parser should support to read this file
	UINT EBMLMaxIDLength;    //4 or less in matroska
	UINT EBMLMaxSizeLength;  //less than 8
	char *DocType;
	UINT DocTypeVersion;     //The version of DocType interpreter used to create the file
	UINT DocTypeReadVersion; //The minimum DocType version an interpreter has to support 
							 //to read this file.
}EBML;

///////////////////SegmentInfo/////////////////////
//Level 2
//A tuple of corresponding ID used by chapter codecs to represent this segment.
typedef struct __ChapterTranslate
{
	UINT        mEditionUID; //Specify an edition UID on which this correspondance applies. 
							//When not specified, it means for all editions found in the segment
	UINT        mCodec;     //The chapter codec using this ID (0: Matroska Script, 1: DVD-menu)
	void        *mID;       //The binary value used to represent this segment in the chapter 
							//codec data. The format depends on the ChapProcessCodecID used.
}ChapterTranslate;

//Level 1
//Contains miscellaneous general information and statistics on the file.
typedef struct __SegmentInfo
{
	char        mSUID[16];      //A randomly generated unique ID to identify the 
								//current segment between many others (128 bits).
	char        *mFileName;     //A filename corresponding to this segment,UTF-8

	char        mPrevUID[16];   //A unique ID to identify the previous chained segment (128 bits)
	char        *mPrevFileName; //An escaped filename corresponding to the previous segment,UTF-8
	char        mNextUID[16];   //A unique ID to identify the next chained segment (128 bits)
	char        *mNextFileName; //An escaped filename corresponding to the next segment,UTF-8

	UINT        mTimeCodeScale; //default 1000000.Timecode scale in nanoseconds (1.000.000 means 
								//all timecodes in the segment are expressed in milliseconds).
	FLOAT       mDuration;      //Duration of the segment (based on TimecodeScale).
	INT         mDateUTC;        //Date of the origin of timecode (value 0), i.e. production date.

	char        *mTitle;         //General name of the segment.UTF-8
	char        *mMuxingApp;     //Muxing application or library ("libmatroska-0.4.3").UTF-8
	char        *mWritingApp;    //Writing application ("mkvmerge-0.3.3").UTF-8

	char        mSegmentFamily[16]; //A randomly generated unique ID that all segments 
									//related to each other must use (128 bits).
	ChapterTranslate  mChapterTranslate;
}SegmentInfo;

/////////////////SeekHead////////////////////////
//Level 2
//Contains a single seek entry to an EBML element
typedef struct __Seek
{
	UINT        mSeekID;  //The binary ID corresponding to the element name.
	UINT        mSeekPos; //The position of the element in the segment in octets 
						  //(0 = first level 1 element).
}Seek;

//Level 1
//Contains the position of other level 1 elements
typedef struct __SeekHead
{
	Seek        mSeek;
}SeekHead;

////////////////////Tracks////////////////////////
//Level 3
//The track identification for the given Chapter Codec.
typedef struct __TrackTranslate
{
	UINT       TrackTranslateEditionUID; //Specify an edition UID on which this translation applies.
										 //When not specified, it means for all editions found in
										 //the segment.
	UINT       TrackTranslateCodec;      //The chapter codec using this ID (0: Matroska Script, 
										 //1: DVD-menu).
	void       *TrackTranslateTrackID;   //The binary value used to represent this track in the 
										 //chapter codec data. The format depends on the 
										 //ChapProcessCodecID used.
}TrackTranslate;

//Level 5 
//Settings describing the compression used. Must be present if the value of ContentEncodingType is
//0 and absent otherwise. Each block must be decompressable even if no previous block is available
//in order not to prevent seeking.
typedef struct __ContentCompression
{
	char       mContentCompAlgo;      //The compression algorithm used. Algorithms that have been
									  //specified so far are:
									  //0 - zlib,
									  //1 - bzlib,
									  //2 - lzo1x
									  //3 - Header Stripping,
	void       *mContentCompSettings; //Settings that might be needed by the decompressor. For 
									  //Header Stripping (ContentCompAlgo=3), the bytes that were
									  //removed from the beggining of each frames of the track.
	int			mContentCompSettingLen;
}ContentCompression;

//Level 5
//Settings describing the encryption used. Must be present if the value of ContentEncodingType 
//is 1 and absent otherwise.
typedef struct __ContentEncryption
{
	UINT       mContentEncAlgo;     //The encryption algorithm used. The value '0' means that the
									//contents have not been encrypted but only signed. Predefined
									//values:
									//1 - DES, 2 - 3DES, 3 - Twofish, 4 - Blowfish, 5 - AES
	UINT       mContentSigAlgo;     //The algorithm used for the signature. A value of '0' means 
									//that the contents have not been signed but only encrypted. 
									//Predefined values: 1 - RSA
	UINT       mContentSigHashAlgo; //The hash algorithm used for the signature. A value of '0' 
									//means that the contents have not been signed but only 
									//encrypted. Predefined values:
									//1 - SHA1-160
									//2 - MD5
	void       *mContentEncKeyID;   //For public key algorithms this is the ID of the public key 
									//the the data was encrypted with.
	void       *mContentSignature;  //A cryptographic signature of the contents
	void       *mContentSigKeyID;   //This is the ID of the private key the data was signed with.
}ContentEncryption;

//Level 4
//Settings for one content encoding like compression or encryption
typedef struct __ContentEncoding
{
	UINT       mContentEncodingOrder; //Tells when this modification was used during 
									  //encoding/muxing starting with 0 and counting upwards. The
									  //decoder/demuxer has to start with the highest order number
									  //it finds and work its way down. This value has to be
									  //unique over all ContentEncodingOrder elements in the segment
	UINT       mContentEncodingScope; //A bit field that describes which elements have been modified
									  //in this way. Values (big endian) can be OR'ed.Possible values:
									  //1 - all frame contents,
									  //2 - the track's private data,
									  //4 - the next ContentEncoding (next ContentEncodingOrder. 
									  //    Either the data inside ContentCompression and/or 
									  //    ContentEncryption)
	unsigned char mContentEncodingType;  //A value describing what kind of transformation has been 
										 //done. Possible values:
										 //0 - compression,
										 //1 - encryption

	ContentCompression mContentCompression;
	ContentEncryption  mContentEncryption;

}ContentEncoding;

//Level 3
//Settings for several content encoding mechanisms like compression or encryption.
typedef struct __ContentEncodings
{
	ContentEncoding mContentEncoding;
}ContentEncodings;

//Level 2
//Describes a track with all elements
typedef struct __TrackEntry
{
	UINT        mTrackNum; //The track number as used in the Block Header (using more than 127 
						   //tracks is not encouraged, though the design allows an unlimited number)
	UINT        mTrackUID; //A unique ID to identify the Track. This should be kept the same when 
						   //making a direct stream copy of the Track to another file.
	unsigned char mTrackType; //A set of track types coded on 8 bits (1: video, 2: audio, 3: 
							  //complex, 0x10: logo, 0x11: subtitle, 0x12: buttons, 0x20: control).
	unsigned int mEnabled:1; //if 1, track is used
	unsigned int mDefault:1; //Set if that track (audio, video or subs) SHOULD be used if no 
							 //language found matches the user preference.
	unsigned int mForced:1;  //Set if that track MUST be used during playback. There can be many
							 //forced track for a kind (audio, video or subs), the player should
							 //select the one which language matches the user preference or the 
							 //default + forced track. Overlay MAY happen between a forced and 
							 //non-forced track of the same kind.
	unsigned int mLacing:1;  //Set if the track may contain blocks using lacing

	UINT        mMinCache;   //The minimum number of frames a player should be able to cache during
							 //playback. If set to 0, the reference pseudo-cache system is not used
	UINT        mMaxCache;   //The maximum cache size required to store referenced frames in and 
							 //the current frame. 0 means no cache is needed.
	UINT        mDefaultDuration; //default number of nanoseconds a frame lasts
	FLOAT       mTimecodeScale;   //The scale to apply on this track to work at normal speed in 
								  //relation with other tracks (mostly used to adjust video speed
								  //when the audio length differs).
	INT         mTrackOffset; //A value to add to the Block's Timecode. This can be used to adjust
							  //the playback offset of a track.
	UINT        mMaxBlockAddtionID; //The maximum value of BlockAddID. A value 0 means there is no
									//BlockAdditions for this track.

	char        *mName;
	char        mLanguage[4];
	char        *mCodecID;          //An ID corresponding to the codec, 
	char        *mCodecPrivate;     //Private data only known to the codec.

	char		*mCodecPrivate2;
	unsigned int mCodecPrivate2Len;
	
	//add by yunfeng_han
	//unsigned int nalu_length_size;  //only used for H264/AVC

	UINT		TrickTrackUID;
	UINT		TrickTrackSegUID[16];
	UINT		TrickTrackFlag;
	
	UINT		MasterTrackUID;
	UINT		MasterTrackSegUID[16];

	
	
	unsigned int mCodecPrivateLen;  //length of private data
	char        *mCodecName;
	UINT        mAttachmentLink;   //The UID of an attachment that is used by this codec.

	char        *mCodecSetttings;   //A string describing the encoding setting used.
	char        *mCodecInfoURL;     //A URL to find information about the codec used.
	char        *mCodecDowloadURL;  //A URL to download about the codec used.
	unsigned int mCodecDecodeAll:1; //The codec can decode potentially damaged data.

	UINT        mTrackOverlay;  //Specify that this track is an overlay track for the Track
								//specified (in the u-integer). That means when this track
								//has a gap the overlay track should be used instead. The 
								//order of multiple TrackOverlay matters,the first one is the
								//one that should be used. If not found it should be the second,etc

	union
	{
		struct
		{
			unsigned int mInterlaced:1; //Set if the video is interlaced.

			unsigned int mStereoMode;   //Stereo-3D video mode on 2 bits 
										//(0: mono, 1: right eye, 2: left eye, 3: both eyes)
			unsigned char mAspectRatioType; //Specify the possible modifications to the aspect 
											//ratio (0: free resizing, 1: keep aspect ratio, 
											//2: fixed)
//				void    *mColorSpace;       //Same value as in AVI (32 bits).
			unsigned int  mColorSpace;      //Same value as in AVI (32 bits).
			FLOAT   mGammaValue;

			unsigned int  mPixelWidth;        //Width of the encoded video frames in pixels
			unsigned int  mPixelHeight;
			unsigned int  mPixelCropBottom;   //The number of video pixels to remove at the bottom
			                                  //of the image (for HDTV content).
			unsigned int  mPixelCropTop;
			unsigned int  mPixelCropLeft;
			unsigned int  mPixelCropRight;
			unsigned int  mDisplayWidth;      //Width of the video frames to display.
			unsigned int  mDisplayHeight;
			unsigned int  mDisplayUnit;       //Type of the unit for DisplayWidth/Height
			                                  //(0: pixels, 1: centimeters, 2: inches).
		}Video; //Level 3, Video settings.

		struct
		{
			FLOAT   mSamplingFreq;       //default 8000Hz;
			FLOAT   mOutputSamplingFreq; //Real output sampling frequency in Hz (used for SBR
											  //techniques).
			unsigned char mChannels;
			unsigned char mBitDepth;          //Bits per sample
			void    *mChannelPositions;
		}Audio; //Level 3, Audio settings
	}Media;

	TrackTranslate mTrackTranslate;
	ContentEncodings mContentEncodings;
}TrackEntry;

//Level 1
//A top-level block of information with many tracks described.
typedef struct __Track
{
	TrackEntry  **mTrackEntry;
}Track;


/////////////////Cues/////////////////////
//Level 4
//The Clusters containing the required referenced Blocks.
typedef struct __CueReference
{
	UINT       mRefTime;       //Timecode of the referenced Block
	UINT       mRefCluster;    //Position of the Cluster containing the referenced Block
	UINT       mRefNumber;     //Number of the referenced Block of Track X in the specified Cluster
	UINT       mRefCodecState; //The position of the Codec State corresponding to this referenced
							   //element.0 means that the data is taken from the initial Track Entry
}CueReference;

//Level 3
//Contain positions for different tracks corresponding to the timecode.
typedef struct __CueTrackPositions
{
	UINT       mTrack;           //The track for which a position is given.
	UINT       mClusterPosition; //The position of the Cluster containing the required Block
	UINT       mBlockNumber;     //Number of the Block in the specified Cluster.

	UINT       mCodecState;   //The position of the Codec State corresponding to this Cue element.
							  //0 means that the data is taken from the initial Track Entry.
	CueReference mCueReference;
}CueTrackPositions;

//Level 2
//Contains all information relative to a seek point in the segment.
typedef struct __CuePoint
{
	UINT       mCueTime;  //Absolute timecode according to the segment time base.

	CueTrackPositions mCueTrackPositions;
}CuePoint;

//Level 1
//A top-level element to speed seeking access. All entries are local to the segment.
typedef struct __Cues
{
	CuePoint   **mCuePoint;
}Cues;

///////////////Attachments/////////////////
//Level 2
//An attached file
typedef struct __AttachedFile
{
	char       *mFileDescription; //A human-friendly name for the attached file.
	char       *mFileName;        //Filename of the attached file.
	char       *mFileMimeType;    //MIME type of the file
	//void       *mFileData;        //The data of the file.
	//void       *mFileReferral;    //A binary value that a track/codec can refer to when the 
								  //attachment is needed.
	UINT       mFileUID;          //Unique ID representing the file, as random as possible.
	UINT	   mFileLen;
	UINT	   mFilePos;
}AttachedFile;

//Level 1
//Contain attached files.
typedef struct __Attachments
{
	AttachedFile *mAttachedFile;
}Attachments;

////////////////Chapters//////////////////
//Level 4
//List of tracks on which the chapter applies. If this element is not present, all tracks apply
typedef struct __ChapterTrack
{
	UINT         mChapterTrackNumber; //UID of the Track to apply this chapter too. In the absense
									  //of a control track, choosing this chapter will select the 
									  //listed Tracks and deselect unlisted tracks. Absense of this
									  //element indicates that the Chapter should be applied to any
									  //currently used Tracks.
}ChapterTrack;

//Level 4
//Contains all possible strings to use for the chapter display.
typedef struct __ChapterDisplay
{
	char         *mChapterString;    //Contains the string to use as the chapter atom
	char         *mChapterLanguage;  //The languages corresponding to the string
	char         *mChapterCountry;   //The countries corresponding to the string, same 2 octets as
									 //in Internet domains.
}ChapterDisplay;

//Level 5
//Contains all the commands associated to the Atom
typedef struct __ChapterProcessCommand
{
	UINT         mChapProcessTime; //Defines when the process command should be handled (0: during
								   //the whole chapter, 1: before starting playback, 2: after 
								   //playback of the chapter).
	void         *ChapProcessData; //Contains the command information. The data should be 
								   //interpreted depending on the ChapProcessCodecID value. For 
								   //ChapProcessCodecID = 1, the data correspond to the binary DVD
								   //cell pre/post commands.
}ChapterProcessCommand;

//Level 4
//Contains all the commands associated to the Atom.
typedef struct __ChapterProcess
{
	UINT         mChapProcessCodecID;  //Contains the type of the codec used for the processing. A
									   //value of 0 means native Matroska processing (to be defined)
									   //, a value of 1 means the DVD command set is used. 
	void         *mChapProcessPrivate; //Some optional data attached to the ChapProcessCodecID 
									   //information. For ChapProcessCodecID = 1, it is the "DVD 
									   //level" equivalent.

	ChapterProcessCommand  *mChapProcessCommand; 
}ChapterProcess;

typedef enum _law_rating_level {
    RATE_G,
    RATE_PG,
    RATE_PG13,
    RATE_R,
    RATE_NC17,
} LAW_RATING_LEVEL;

//Level 3
typedef struct __ChapterAtom
{
	UINT         mChapterUID;       //A unique ID to identify the Chapter
	UINT         mChapterTimeStart; //Timecode of the start of Chapter (not scaled).
	UINT         mChapterTimeEnd;   //Timecode of the end of Chapter(timecode excluded, not scaled)

	unsigned int mFlagHidden:1;     //If a chapter is hidden (1), it should not be available to the
									//user interface (but still to Control Tracks).
	unsigned int mFlagEnabled:1;    //Specify wether the chapter is enabled. It can be 
									//enabled/disabled by a Control Track. When disabled, the movie
									//should skip all the content between the TimeStart and TimeEnd
									//of this chapter.

	void         *mSegmentUID;      //A segment to play in place of this chapter. Edition 
									//ChapterSegmentEditionUID should be used for this segment, 
									//otherwise no edition is used
	void         *mChapterSegmentEditionUID; //The edition to play from the segment linked in 
											 //ChapterSegmentUID
	UINT         mChapterPhysicalEquiv; //Specify the physical equivalent of this ChapterAtom like
										//"DVD" (60) or "SIDE" (50), 

	ChapterTrack     mChapterTrack;   	
	ChapterDisplay   mChapterDisplay; 	
	ChapterProcess   mChapterProcess;
	LAW_RATING_LEVEL mRatingLevel;
}ChapterAtom;

//Level 2
//Contains all information about a segment edition.
typedef struct __EditionEntry
{
	UINT       mEditionUID;      //A unique ID to identify the edition. It's useful for tagging 
								 //an edition

	unsigned int mFlagHidden:1;  //If an edition is hidden (1), it should not be available to the
								 //user interface (but still to Control Tracks).
	unsigned int mFlagDefault:1; //If a flag is set(1),the edition should be used as the default one
	unsigned int mFlagOrdered:1; //Specify if the chapters can be defined multiple times and the 
								 //order to play them is enforced

	ChapterAtom  *mChapterAtom;  //Contains the atom information to use as the chapter atom (apply
								 //to all tracks).
	int 		 nChapterAtom;
	
}EditionEntry;

//Level 1
//A system to define basic menus and partition data.
typedef struct __Chapters
{
	EditionEntry  *mEditionEntry;
}Chapters;


//////////////Taging///////////////////
//Level 3
//Contains general information about the target
typedef struct __SimpleTag
{
	char         *mTagName;     //The name of the Tag that is going to be stored
	char         *mTagLanguage;  //Specifies the language of the tag specified
	char         *mTagString;   //The value of the Tag

	unsigned int mTagDefault:1; //Indication to know if this is the default/original language to 
								//use for the given tag
	void         *mTagBinary;   //The values of the Tag if it is binary. Note that this cannot be
								//used in the same SimpleTag as TagString.
}SimpleTag;

//Level 3
//Contain all UIDs where the specified meta data apply. It is void to describe everything 
//in the segment.
typedef struct __Targets
{
	UINT         mTargetTypeValue; //A number to indicate the logical level of the target 
	char         *mTargetType;     //An informational string that can be used to display the 
								   //logical level of the target like "ALBUM", "TRACK", "MOVIE",
								   //"CHAPTER", etc 
	UINT         mTrackUID;      //A unique ID to identify the Track(s) the tags belong to. If the
								 //value is 0 at this level, the tags apply to all tracks in the 
								 //Segment.
	UINT         mEditionUID;    //A unique ID to identify the EditionEntry(s) the tags belong to.
								 //If the value is 0 at this level, the tags apply to all editions
								 //in the Segment
	UINT         mChapterUID;    //A unique ID to identify the Chapter(s) the tags belong to. If 
								 //the value is 0 at this level, the tags apply to all chapters in
								 //the Segment
	UINT         mAttachmentUID; //A unique ID to identify the Attachment(s) the tags belong to. 
								 //If the value is 0 at this level, the tags apply to all the 
								 //attachments in the Segment.
}Targets;

//Level 2
//Element containing elements specific to Tracks/Chapters
typedef struct __Tag
{
	Targets      mTargets;
	SimpleTag    *mSimpleTag;
	int			 nSimpleTag;
}Tag;

//Level 1
//Element containing elements specific to Tracks/Chapters
typedef struct __Tags
{
	Tag          *mTag;
}Tags;

///////////////////Cluster/////////////////
//Level 4
//Contain the BlockAdditional and some parameters.
typedef struct __BlockMore
{
	UINT         mBlockAddID;       //An ID to identify the BlockAdditional level
	void         *mBlockAdditional; //Interpreted by the codec as it wishes (using the BlockAddID)
}BlockMore;

//Level 3
//Contain additional blocks to complete the main one. An EBML parser that has no knowledge of 
//the Block structure could still see and use/skip these data.
typedef struct __BlockAdditions
{
	BlockMore    *mBlockMore;
}BlockAdditions;

//Level 4
typedef struct __TimeSlice
{
	UINT         mLaceNumber;      //The reverse number of the frame in the lace (0 is the last 
								   //frame, 1 is the next to last, etc). While there are a few 
								   //files in the wild with this element, it is no longer in use 
								   //and has been deprecated. Being able to interpret this element
								   //is not required for playback.
	UINT         mFrameNumber;     //The number of the frame to generate from this lace with this 
								   //delay (allow you to generate many frames from the same 
								   //Block/Frame).
	UINT         mBlockAdditionID; //The ID of the BlockAdditional element (0 is the main Block).
	UINT         mDelay;           //The (scaled) delay to apply to the element.
	UINT         mDuration;        //The (scaled) duration to apply to the element.
}TimeSlice;

//Level 3
//Contains slices description.
typedef struct __Slices
{
	TimeSlice    *mTimeSlice;      //Contains extra time information about the data contained in 
								   //the Block. While there are a few files in the wild with this
								   //element, it is no longer in use and has been deprecated.Being
								   //able to interpret this element is not required for playback.
}Slices;

//Level 2
//The list of tracks that are not used in that part of the stream. It is useful when using overlay 
//tracks on seeking. Then you should decide what track to use.
typedef struct __SilentTracks
{
	UINT         mSilentTrackNumber; //One of the track number that are not used from now on in 
									//the stream. It could change later if not specified as silent
									//in a further Cluster.
}SilentTracks;

//Level 2
//Basic container of information containing a single Block or BlockVirtual, and information 
//specific to that Block/VirtualBlock.
typedef struct __BlockGroup
{
	void         *mBlock;             //Block containing the actual data to be rendered and 
									  //a timecode relative to the Cluster Timecode.
	void         *mBlockVirtual;      //A Block with no data. It must be stored in the stream 
									  //at the place the real Block should be in display order.
	void         *mCodecState;        //The new codec state to use. Data interpretation is 
									  //private to the codec. This information should always be 
									  //referenced by a seek entry.
	UINT         mBlockDuration;      //The duration of the Block (based on TimecodeScale). This 
									  //element is mandatory when DefaultDuration is set for the
									  //track. When not written and with no DefaultDuration, the
									  //value is assumed to be the difference between the timecode
									  //of this Block and the timecode of the next Block in 
									  //"display" order (not coding order). This element can be 
									  //useful at the end of a Track (as there is not other Block
									  //available), or when there is a break in a track like for
									  //subtitle tracks.
	UINT         mReferencePriority;  //This frame is referenced and has the specified cache 
									  //priority. In cache only a frame of the same or higher 
									  //priority can replace this frame. A value of 0 means the 
									  //frame is not referenced.
	INT          mReferenceBlock;     //Timecode of another frame used as a reference (ie: B or P 
									  //frame). The timecode is relative to the block it's attached
									  //to.
	INT          mReferenceVirtual;   //Relative position of the data that should be in position 
									  //of the virtual block

	BlockAdditions  mBlockAdditions;
	Slices       mSlices;
}BlockGroup;

//Level 1
//The lower level element containing the (monolithic) Block structure.
typedef struct __Cluster
{
	UINT         mTimeCode; //Absolute timecode of the cluster (based on TimecodeScale).
	UINT         mPosition; //Position of the Cluster in the segment (0 in live broadcast streams)
							//It might help to resynchronise offset on damaged streams.
	UINT         mPrevSize; //Size of the previous Cluster,in octets.Can be useful for backward 
							//playing.

	SilentTracks mSilenrTracks;
	BlockGroup   *mBlockGroup;
	void         *mSimpleBlock;    //Similar to Block but without all the extra information,mostly
								   //used to reduced overhead when no extra feature is needed.
	void         *mEncryptedBlock; //Similar to SimpleBlock but the data inside the Block are 
								   //Transformed (encrypt and/or signed).
}Cluster;

//Level 0
//This element contains all other top-level (level 1) elements. 
//Typically a Matroska file is composed of 1 segment.
typedef struct __Segment
{
	//Element start position
	UINT        mSegmentPos;
	UINT        mSegmentInfoPos;
	UINT        mSeekHeadPos;
	UINT        mClusterPos;
	UINT        mTracksPos;
	UINT        mCuesPos;
	UINT        mAttachmentsPos;
	UINT        mChaptersPos;
	UINT        mTagsPos;

	//Element existance bit
	struct{
	  unsigned int  SegmentInfo:1;
	  unsigned int  SeekHead:1;
	  unsigned int  Cluster:1;
	  unsigned int  Tracks:1;
	  unsigned int  Cues:1;
	  unsigned int  Attachments:1;
	  unsigned int  Chapters:1;
	  unsigned int  Tags:1;
	}exist;

	//Element content
	SegmentInfo mSegmentInfo;
	SeekHead    mSeekHead;      
	Cluster     mCluster;
	Track       mTracks;
	Cues        mCues;
	Chapters    mChapters;
	Attachments mAttachments;
	Tags        mTags;
}Segment;


typedef struct _tagDRMParams {
	uint8_t 		rentalMessageFlag;
	uint8_t 		useLimit;
	uint8_t 		useCount;

	uint32_t		leftCount;

	uint8_t			acptbSignal;
	uint8_t			cgmsaSignal;
	uint8_t			ict;
	uint8_t			digitalProtectionSignal;
} DRMPARAMS;

typedef struct _tagDRMInfoChunk {
	uint16_t	frameKeyIndex;
	uint32_t	encryptionOffset;
	uint32_t	encryptionLength;
} __attribute__((packed)) DRMINFOCHUNK;


typedef struct _Frame
{
  UINT  startTime;
  UINT  duration;
  UINT  framePos;
  unsigned int frameLen;
  unsigned int bKeyFrame;
  #ifdef DIVX_PLUS
  char drmInfo[10];
  #endif
}Frame;

typedef struct _FrameList
{
	Frame *pHeader;
	unsigned int nFrames;
	unsigned int nUnread;
}FrameList;

typedef struct _IdxEntry
{
	UINT pos;      //cluster position
	UINT timecode; //cluster timecode
}IdxEntry;

//for subtitle memory management
typedef struct _QueueItem QueueItem;
struct _QueueItem
{
	QueueItem *pNext;
	unsigned int nTime;
	char *pData;
};

typedef struct _SubtitleQueue
{
	QueueItem *pHead;
	QueueItem *pTail;
}SubtitleQueue;

//add by yunfeng_han
typedef struct _H264_VIDEO_INFO
{
  unsigned int displayRatioHorizontal ; /* horizontal factor of the display aspect ratio */
  unsigned int displayRatioVertical ;   /* vertical   factor of the display aspect ratio */
  unsigned int frameDurationInPTS ;     /* frame rate information evaluated in PTS increment
										on consecutive frames, equal to 90000/FPS */
} H264_VIDEO_INFO ; 





#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |    \
				 (((uint32_t)(ch4) & 0xFF00) << 8) |    \
				 (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
				 (((uint32_t)(ch4) & 0xFF000000) >> 24))

typedef struct _riffchunk 
{
   uint32_t fcc;
   uint32_t  cb;
} RIFFCHUNK;

typedef enum {
	SEEK_FORWARD,
	SEEK_BACKWARD,
} seekDirection;

typedef enum {
	AUDIO_PACKET,
	VIDEO_PACKET,
	SPIC_PACKET,
	OTHERS,
} BlockType;

typedef enum {
	Init,
	Recalc,
} calcType;

#if !defined(_BITMAPINFOHEADER_)
#define _BITMAPINFOHEADER_
typedef struct {
    uint32_t   biSize;
    uint32_t   biWidth;
    uint32_t   biHeight;
    uint16_t   biPlanes;
    uint16_t   biBitCount;
    uint32_t   biCompression;
    uint32_t   biSizeImage;
    uint32_t   biXPelsPerMeter;
    uint32_t   biYPelsPerMeter;
    uint32_t   biClrUsed;
    uint32_t   biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER, *LPBITMAPINFOHEADER;

typedef struct {
	BITMAPINFOHEADER bmiHeader;
	int        bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO;

#endif

  
typedef struct{
	uint32_t cbLength;
    uint32_t moftag;
    uint32_t submoftag;
    uint16_t uiWidth;
    uint16_t uiHeight;
    uint16_t uiBitCount;
    uint16_t uiPadWidth;
    uint16_t uiPadHeight;
    uint32_t framesPerSecond;
    uint32_t ulSPOExtra;
    uint32_t ulStreamVersion;
}__attribute__((packed)) rmprivate_type_specific_data ;


typedef struct{
  uint32_t fourcc1;             /* '.', 'r', 'a', 0xfd */
  uint16_t version1;            /* 4 or 5 */
  uint16_t unknown1;            /* 00 00 */
  uint32_t fourcc2;             /* .ra4 or .ra5 */
  uint32_t stream_length;       /* ??? */
  uint16_t version2;            /* 4 or 5 */
  uint32_t header_size;         /* == 0x4e */
  uint16_t flavor;              /* codec flavor id */
  uint32_t coded_frame_size;    /* coded frame size */
  uint32_t unknown3;            /* big number */
  uint32_t bytes_per_miniute;	
  uint32_t bytes_per_miniute2;  /* yet another number */
  uint16_t block_num;
  uint16_t sub_block_size;
  uint16_t frame_size;
  uint16_t unknown6;            /* 00 00 */
  uint16_t sample_rate;
  uint16_t unknown8;            /* 0 */
  uint16_t sample_size;
  uint16_t channels;
  uint32_t unknown9;
  uint32_t extra_data_size;
} __attribute__((packed)) real_audio_v4_props_t;

typedef struct {
  uint32_t fourcc1;             /* '.', 'r', 'a', 0xfd */
  uint16_t version1;            /* 4 or 5 */
  uint16_t unknown1;            /* 00 00 */
  uint32_t fourcc2;             /* .ra4 or .ra5 */
  uint32_t stream_length;       /* ??? */
  uint16_t version2;            /* 4 or 5 */
  uint32_t header_size;         /* == 0x4e */
  uint16_t flavor;              /* codec flavor id */
  uint32_t coded_frame_size;    /* coded frame size */
  uint32_t unknown3;            /* big number */
  uint32_t bytes_per_miniute;	
  uint32_t bytes_per_miniute2;  /* yet another number */
  uint16_t block_num;
  uint16_t sub_block_size;
  uint16_t frame_size;
  uint16_t unknown6;            /* 00 00 */
  uint8_t unknown7[6];          /* 0, srate, 0 */
  uint16_t sample_rate;
  uint16_t unknown8;            /* 0 */
  uint16_t sample_size;
  uint16_t channels;
  uint32_t interleaverID;                /* "genr" */
  uint32_t fourcc3;             /* fourcc */
  unsigned char  isInterleaved;		
  unsigned char  can_copy;
  unsigned char  streamType; 
  unsigned char  hasInterleavePattern; 
  //FIXME @may have extra data if hasInterleavePattern == 1
  uint32_t extra_data_size;             
} __attribute__((packed)) real_audio_v5_props_t ;

/* rm audio codec-specific information -- one for each stream */
typedef struct rm_audio_opaque
{
	unsigned short flavorIdx;			/* codec flavor index */
	unsigned short sampleSize;			/* number of bits per audio sample*/
	unsigned short interleaveFactor;	/* the number of packet(blocks) over which interleaving is performed(a superblock) */
	unsigned short interleaveBlockSize; /* the size of each block (packet) */
	unsigned long  bitRate;				/* bit rate per second of this substream */		
	unsigned long  sampleRate;
	unsigned short frameSize;			/* codec frame size : size, in bytes, of each encoded frame of audio for this substream. */
	unsigned short numChannels;			/* number of channels in this audio substream */
	unsigned long  interleaverID;		/* a 4CC that spcifies the interleaver for this substream */
	unsigned long  codecID;				/* a 4CC that identifies the codec for this substream */
	unsigned long  codecExtraSize;		/* the size of codec opaque data */
	unsigned char* pCodecExtraData;		/* codec-specific opaque data of length codecExtraSize */
	unsigned char  streamType;			/* it is significant only when codec type is RAAC */
	unsigned char  isInterleaved;		/* specify whether or not interleaving is used on this substream */
	unsigned char  hasInterleavePattern;/* non-zero means a user defined interleave pattern is used if this substream is interleaved */
	unsigned short* pInterleavePattern; /* a user defined interleave pattern */
	unsigned short* pDeinterleavePattern;/* the de-interleave pattern is transformed from interleave pattern which is used to organize audio packet */
	
	int 		   	framesPerSuperBlock;/* number of frames in each superblock */
	int 			superblockSize;		/* size, in byte, of each superblock */
}rm_audio_opaque_t;


typedef struct __MKV_TITLE_INFO
{
	int64_t	segmentPos;
	int64_t	segmentLen;
	int		RealIndex;
	int		HasTrickPlayTrack;	 //0 NO   1 YES
	int		TrickPlayTrackIndex; // valid when HasTrickPlayTrack == 1
}MKV_TITLE_INFO;
#define MAX_MKV_TITLE 32

typedef struct __MKV_TITLE_INFO2
{
	int64_t	segmentPos;
	int64_t	segmentLen;
	int		checked;		  //this title has been checked
	int		isTrickPlayTrack; //0 NO  1 YES
	int		MasterTrackIndex; //valid only when isTrickPlayTrack == 1
}MKV_TITLE_INFO2;




typedef struct __MKV_NAV_STATE
{
	int title_num;
	int elapsedtime;
}MKV_NAV_STATE;

/* plugin instance object */

typedef struct _tagInputMKV_t {
	/*--------------------------------------------------------------
		MKV file format stuff
	---------------------------------------------------------------*/
	EBML    ebml;
	Segment segment;
		
	UINT streamTotalBytes;

	UINT currReadPosition;

	//Number of elements
	unsigned int nTracks;
	unsigned int nCues;
	unsigned int nTags;
	unsigned int nAttachedFile;
	unsigned int nEditionEntry;
	//unsigned int nChapterAtom;
	//unsigned int nSimpleTag;

	TrackEntry **pTracks;     // point to pMKV->segment.mTracks.mTrackEntry

	UINT TimecodeStart;	      // timecode of the first block
	UINT SegmentEnd;          // end position of Segment
	UINT currClusterTimeCode; // timecode of  current cluster

	//Store frame positions in a BlockGroup
	FrameList videoFrameList;
	FrameList audioFrameList;
	FrameList spicFrameList;

	unsigned int nVideoLaced;  //frames lacing in video blockgroup
	unsigned int nAudioLaced;  //frames lacing in audio blockgroup
	unsigned int nSpicLaced;

	//Track in use
	int				audioTrackIdx;
	int				videoTrackIdx;
	int				spicTrackIdx;

	unsigned int  currTrackType; 
	unsigned int  biCompressionVideo;  //Video FOURCC type
	unsigned char bAACSync;            //used for aac sync word
	unsigned char bDIVX3Sync;
	unsigned int  divx3Len;
	RIFFCHUNK     tmpChunk;            // for Divx3
	unsigned int  bMPEG2Sync;
	int           firstMPEG2PTS;       // for mpeg2 FF/FR


	//used only in H264 Video add by yunfeng_han 
	//used in H264 Video 0 means to send startcode  1 means to send nalu unit 
	unsigned int  bH264Sync; 
	//used in H264 Video 0 means last frame last frame was sent completely otherwise means how much data not sent in last frame 
	unsigned long  nH264FrameLeft;
	//used in H264 Video  means how much data has been sent in last frame 
	unsigned long  nH264AlreadyRead;
	unsigned int   nalu_length_size;
	unsigned char *	pPayloadData;
	unsigned int	bFirstFrameUnit;//indacate if we have to send pts
	unsigned int    bNaluDataLeft;

	//used when nalu length can not be read at once from buffer
	unsigned char	nalu_len_left_bytes[4];
	unsigned int	nalu_len_left_num;
	
	unsigned int    bH264FourCC; //indiatce if H264 video form VFW/FOURCC
	unsigned int    bMPEG4FourCC;//indiatce if MPEG4 video form VFW/FOURCC 
	unsigned int    bMPEG2FourCC;//indiatce if MPEG2 video form VFW/FOURCC 

	long long		LastAudioFrameStartTime;
	unsigned int	bIs_IONLY; //indacate if in I_ONLY Decode Mode


	unsigned int   read_error_count;//fix bug sometimes,frame size is 0,need to count this error

	//vorbis related 
	//bool		bVorbisHeaderSent;
	//AUDIOVORBIS audioVorbis;

	//for audio bitstream header strip
	bool         bHeaderStripSync;

	//for video bitstream header strip
	bool		 bVideoHeaderStripSync;
	int			 nVideoHeaderStripSize;
	bool		 bVideoHeaderStripUsed;
	unsigned char* pDivx3payload;
	int				nDivx3FrameLen;
	unsigned long  nDivx3FrameLeft;
	
	bool 		 bVorbisSync;
	unsigned int vorbis_bitstream_size;

	//for audio stream switch
	unsigned char bAudioSync;

	//for subtitle switch
	unsigned char bSubtitleSync;
	UINT          currReadPosBak;
	UINT          currClusterTCBak;


	unsigned int palette[16];//vobsub spu palette
	SPUDVDVPALETTE spuPalette;
	unsigned char bspuSync;  //used for send spu palette
	unsigned int spu_frame_width;
	unsigned int spu_frame_height;
	
	//for A-B repeat
	UINT          abStart; 
	UINT          abEnd;
	unsigned int  bABReset;

	SubtitleQueue  spicQueue;
	EventSpicFrame spicFrame;

	//for rewind playback
	IdxEntry keyFrameCache[1024];
	unsigned int nKeyFrame;


	IdxEntry AudioFrameCache[MAX_CLUSTER_INDEX];
	int		 Audio_LowIndex;
	int		 Audio_HignIndex;

	//temporary use
	unsigned int totalVideoLen;
	unsigned int totalAudioLen;
	FILE            *pAudioFile;
	FILE            *pSubFile;
	FILE            *pVideoFile;

	PRESENTATION_POSITIONS	currPlaybackPosition;
	int64_t			currVideoSystemPTS;
	int64_t			currAudioSystemPTS;
	int64_t			prevVideoSystemPTS;
	int64_t			currRefClock;

	/*--------------------------------------------------------------
		Playback control
	---------------------------------------------------------------*/
	int64_t			mEvent;
	int				mState;

	bool 			bIsType1DV;

	bool			bAudioSeek;
	bool			bVideoSeek;
	bool			bSpicSeek;

	bool			bAudioTerm;
	bool			bVideoTerm;
	bool			bSpicTerm;

	bool			bVideoEndSeq;
	bool			bStreamEnd;

	NAV_REPEAT_MODE  repeatMode;

	AUDIOFORMAT audioFormat;
	VIDEOFORMAT videoFormat;
	VIDEODIVX4	videoFormat2;
#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	VIDEOVC1SEQINFO videoFormat3;
	VIDEORMFORMAT	videoFormat4;
	VIDEORMSEGMENT	RmSegment;
#endif
	/* Rm Sequence Number Used only in RM video*/
	uint32_t		nRmsequenceNum;
	bool			bRealVideoSync;
	int				nRealVideoAlreadyRead;
	int				nRealVideoFrameLeft;
	rm_audio_opaque_t rmaudio_format;
	unsigned char*	pAudioDeinterleaveBuffer;
	unsigned long   CurrentAudioIndex;
	int64_t			CurrentAudioSuperblockPts;


	bool			bTotalFile_Checked;
	UINT			CurrCueCheckPosition;

	
	SPUCONFIG	spuConfig;
	int64_t		audio_preload_pts;

	NAVMEDIAINFO	media;

	int64_t			audioPTS;
	int64_t			videoPTS;
	int64_t			spicPTS;

	int64_t			initVideoPTS;

	VIDEODVDDCICCI	DCICCI;

	/* for speed control, I-Frame Scan Only */
	int				m_speed;
	int				m_skip;

	/* for spic */
	int				m_spicOnOff;


	//for uncompress zlib video or audio only
	unsigned char*	pAVUncompressBuffer;
	bool			bAVUncompressed;

	unsigned long	mkv_buf_size;
	
	/*--------------------------------------------------------------
		Inband command stuff
	---------------------------------------------------------------*/
    
    long 				statusFlag; 
	long				cmdFlag;
	int64_t				decodeInitCMD;
	VIDEOMIXERBLEND 	blendCMD;
	VIDEODECODECOMMAND	decodeCMD;
	VIDEODVDVOBUINFO    vobuCMD;
	NAV_AVSYNC_MODE		AVSyncMode;

	/*--------------------------------------------------------------
		IO stuff
	---------------------------------------------------------------*/
	NAVIOINFO 			ioInfo;
	int64_t 			ioOffset;
	bool				bDirectIO;
	int					ioErrorCnt;

    int 				initialOffset;
	void*				fileHandler;

	unsigned char		*cmdBuffer;

	unsigned char   	*buffer;
	long long 			bufferStart;
	int					bufferSize;

	unsigned char		*ioBuffer;
	long long			ioBufferStart;
	int					ioBufferSize;

	IOPLUGIN* 			ioPlugin;
    osal_mutex_t*		ioMutex;
	osal_mutex_t		m_readMutex;
	long fatalErrCode;

#if AP_CONFIG_WITH(APCFG_SMOOTH_TRICK_PLAY)
  	bool 	bShowNextGOP;
	bool	bGOPSentIframe;
	int		nGOPSkipTotal;
	int 	nFramesGOPSent;
	int		nFramesNeedSkip;
	int 	nFramesSkip;
#endif
	VIDEODVDVOBUINFO mvobuinfo;

	int		DTSFramePTS;
	bool	bGotDTSFramePTS;
	int		DTSFramecount;
	bool    bDTSFramePTSChecked;
	int		nDTSPTSFixNum;

	int		AACFramePTS;
	bool	bGotAACFramePTS;
	int		AACFramecount;
	bool    bAACFramePTSChecked;
	int64_t AACPTS;
	int		nAACPTSFixNum;

	int		nContinousSPUFrame;

	int     bMPEG4_DX50;

	
	VIDEOUSABILITYINFO videousabilityinfo;
	VIDEOMPEG4USABILITYINFO mp4info;
	int		nCurrentAngle;
	bool	bAngleChanged;
	
#ifdef DIVX_PLUS
	unsigned int bDrmDetected;
	bool	bIsDRM;
	unsigned int drmVersion;
	unsigned char 	*drmHeader;
	int			drmHeaderLength;
	DRMINFOCHUNK	*drmInfo;
	DRMPARAMS		drmParams;
	void 			*drmContext;
	NAVDIVXDRMINFO	drmStatus;
	//char 			*decryptBuffer; 
	unsigned int 	VideoDataContainDRM;//indicate if the video data currently read is DRM encrypted

#endif

	int eos_handling_count;
	NAVLOADFAILURE* m_pFailure;

	

	bool	bPlayAccordingPlayList;
	int		nCurrentEditionEntryIndex;
	
	int		nCurrentChapAtomIndex;
	int		nPrevChapAtomIndex;
	int		nTmpChapAtomIndex;
	
	bool	bChapterWating;
	int 	mPmlRequestedLevel;


	//title known to user
	MKV_TITLE_INFO Mkv_Title[MAX_MKV_TITLE];
	int 	nMkvTitle;
	int     nCurrent_Title_Index;
	bool 			bTitle_Lawratingchecked;
	LAW_RATING_LEVEL mCurrent_Title_RatingLevel;

	//total titles incluing trickplaytrack Titles
	MKV_TITLE_INFO2 Total_Mkv_Title[MAX_MKV_TITLE];
	int 	nTotalMkvTitle;
	


	bool				bTrickPlayTrackMode;//if more than 4X FF or FR Mode,set to true else set false
	bool				bHasTrickPlayTrack;
	bool				bParseTrickPlaySegment;
	int     			nTrickTrack_Title_Index;
	int    	 			nMasterTrack_Title_Index;		
	Segment 			TrickTracksegment;
	unsigned int		nTrickTracks;
	int					videoTrickTrackIdx;
	unsigned int 		nTrickCues;
	UINT 				TrickTrackTimecodeStart;	      // timecode of the first block in trick track
	UINT 				TrickTrackSegmentEnd;          // end position of trick track Segment
	unsigned int        biTrickTrackCompressionVideo;
	ENUM_MEDIA_TYPE		TrickTrackVideoType;
	unsigned int 		bTrickTrackH264FourCC;
	unsigned int		TrickTracknalu_length_size;
	unsigned int 		bTrickTrackMPEG4FourCC;
	unsigned int 		bTrickTrackMPEG2FourCC;


	int					TrackToIdx[255];
	int					TrickTrackToIdx[255];
	long				total_orderd_mseconds;
	long				average_autochapter_mseconds;
	int 				orderd_chapter_send_eof_flag;
	int					in_last_ordered_chapter;
	int					in_first_ordered_chapter;

	int                 divx_disabled;//flag to determin if file has to loadmedia fail due to divx lisence  issue
    bool b_mUsedInband;
}InputMKV_t;

UINT mkvGetFilePos(InputMKV_t *pMKV);
void mkvSkip(InputMKV_t *pMKV, UINT len);
int mkvSeekTo(InputMKV_t *pMKV, UINT pos);
char mkvReadByte(InputMKV_t *pMKV, char *charOut);
char mkvReadBytes(InputMKV_t *pMKV, void *buffer, int len);
char mkvReadVUINT(InputMKV_t *pMKV, UINT *retInteger, int *len);
char mkvReadVINT(InputMKV_t *pMKV, INT *retInteger);
char mkvReadUINT(InputMKV_t *pMKV, UINT *retInteger, unsigned int len);
char mkvReadINT(InputMKV_t *pMKV, INT *retInteger, unsigned int len);
char mkvReadFloat(InputMKV_t *pMKV, FLOAT *pFloat, unsigned int len);
char mkvReadSize(InputMKV_t *pMKV, UINT *retSize);
char mkvReadID(InputMKV_t *pMKV, int *retID);
char mkvReadLanguage(InputMKV_t *pMKV, UINT len, char languageCC[4]);
int mkvParseSegmentInfo(InputMKV_t *pMKV, UINT len);
int mkvParseSeek(InputMKV_t *pMKV, UINT len);
int mkvParseSeekHead(InputMKV_t *pMKV, UINT len);
int mkvParseVideoInfo(InputMKV_t *pMKV, UINT len, TrackEntry *pTrack);
int mkvParseAudioInfo(InputMKV_t *pMKV, UINT len, TrackEntry *pTrack);
int mkvParseCompression(InputMKV_t *pMKV, UINT len, TrackEntry *pTrack);
int mkvParseContentEncoding(InputMKV_t *pMKV, UINT len, TrackEntry *pTrack);
int mkvParseContentEncodings(InputMKV_t *pMKV, UINT len, TrackEntry *pTrack);
int mkvParseTrackEntry(InputMKV_t *pMKV, UINT len);
#ifdef DIVX_PLUS
int mkvParseTrackData(InputMKV_t *pMKV, UINT len);
#endif
int mkvParseTracks(InputMKV_t *pMKV, UINT len);
int mkvParseCueTrackPos(InputMKV_t *pMKV, UINT len, CuePoint *pCuePoint);
int mkvParseCuePoint(InputMKV_t *pMKV, UINT len);
int mkvParseCues(InputMKV_t *pMKV, UINT len);
int mkvFindAndParseCues(InputMKV_t *pMKV);
int mkvParseAttachments(InputMKV_t *pMKV, UINT len);
int mkvParseChapterDisplay(InputMKV_t *pMKV, UINT len);
int mkvParseChapterAtom(InputMKV_t *pMKV, UINT len, EditionEntry *pEditionEntry);
int mkvParseEditionEntry(InputMKV_t *pMKV, UINT len);
int mkvParseChapters(InputMKV_t *pMKV, UINT len);
int mkvParseTags(InputMKV_t *pMKV, UINT len);
int mkvParseSimpleBlock(InputMKV_t *pMKV, UINT len, UINT timecode);
int mkvParseSubBlock(InputMKV_t *pMKV, UINT len, UINT clusterTimecode);
int mkvParseBlockGroup(InputMKV_t *pMKV, UINT len, UINT clusterTimecode);
int mkvParseClusterStartTime(InputMKV_t *pMKV, UINT len);
int mkvParseSegment(InputMKV_t *pMKV, UINT len);
int mkvParseTrickTrackSegment(InputMKV_t *pMKV, UINT len);
int mkvParseEBML(InputMKV_t *pMKV, UINT len);
int mkvGetAndCheckAllTitles(InputMKV_t *pMKV);
int mkvParseFile(InputMKV_t *pMKV);
void mkvReleaseMKV(InputMKV_t *pMKV);
void mkvReleaseTrickTrackMKV(InputMKV_t *pMKV);
void mkvCheckTrickPlayTrack(InputMKV_t* pMKV);

#endif //__MKV_INPUT_PLUGIN_INTERNAL_HEADER__

