#ifndef __STREAM_PARSE_INFO_H__
#define __STREAM_PARSE_INFO_H__



typedef enum {
	FRAME_TYPE_I,
	FRAME_TYPE_P,
	FRAME_TYPE_B,
	
} ENUM_FRAME_TYPE;

typedef enum {

	FRAME_RATE_Forbidden,	// forbidden 
	FRAME_RATE_23976,		// 23.976 
	FRAME_RATE_24,			// 24 
	FRAME_RATE_25,			// 25 
	FRAME_RATE_2997,		// 29.97 
	FRAME_RATE_30,			// 30 
	FRAME_RATE_50,			// 50 
	FRAME_RATE_5994,		// 59.94 
	FRAME_RATE_60,			// 60 
	FRAME_RATE_Reserved,	// Reserved 
	
} ENUM_FRAME_RATE_CODE;

typedef enum {
	ASPECT_RATIO_Forbidden,		// forbidden
	ASPECT_RATIO_SquareSample,	// 1.0 (square sample)
	ASPECT_RATIO_4_3,			// 4 : 3
	ASPECT_RATIO_16_9,			// 16 : 9
	ASPECT_RATIO_221_1,			// 2.21 : 1
	ASPECT_RATIO_Reserved,
	
} ENUM_ASPECT_RATIO;


typedef struct {

	ENUM_MEDIA_TYPE 		VideoEncoderType;		
	ENUM_FRAME_RATE_CODE	FrameRate;				// video frame rate
	int						FrameWidth;				// picture vertical size
	int						FrameHeight;			// picture horizontal size
	unsigned int			VideoBitRate;
	ENUM_ASPECT_RATIO		AspectRatio;			// aspect ratio information
	/*ENUM_MEDIA_TYPE		AudioEncoderType;
	unsigned int		AudioBitRate;
	unsigned int		SamplingFrequence;		// audio sampleing frequence
	unsigned int		ChannelNumber;			// audio channel number. ( 5.1 channel ==> 5+1=6 )*/
	
	
} PARSE_STREAM_ATTR_INFO;


typedef struct {
	ENUM_FRAME_TYPE		FrameType;			// I/P/B
	int					FieldNumber;		// number of field per frame
	unsigned int		PICTSize;			// size of picture (byte)

} PARSE_STREAM_PICT_INFO;


typedef struct {

	unsigned int		FirstRefPicSize;		// first reference(I or P) picture size. (from GOP header to next picture header inclusive of audio stream)
	unsigned int		SecondRefPicSize;		// second reference(I or P) picture size. (from current picture header to next picture header)
	unsigned int		ThirdRefPicSize;		// third reference(I or P) picture size. (from current picture header to next picture header)
	unsigned int		FieldNumber;			// field number per GOP
	unsigned int		VOBUStartAddr;			// GOP start address (including pack header, packet header)
	unsigned int		VOBUSize;				// GOP size (# bytes per GOP) 
	
} PARSE_STREAM_VOBU_INFO;


typedef struct {

	unsigned long long	VOBStartPTM;			// the first PTM of the movie
	unsigned long long	VOBEndPTM;				// the last PTM of the movie
	unsigned int		VOBUNumber;				// total GOP number
	
} PARSE_STREAM_VOB_INFO;


////////////////////////////////////////////////////////////////////////////////
///																			 ///
/// mpeg2 parse agent will use these structures to deliver parse stream info ///
///																			 ///
////////////////////////////////////////////////////////////////////////////////

typedef enum
{
	PARSE_STREAM_TYPE_ATTR_INFO,
	PARSE_STREAM_TYPE_VOBU_INFO,
	PARSE_STREAM_TYPE_VOB_INFO,
	PARSE_STREAM_TYPE_END_OF_STREAM,
	
} ENUM_PARSE_STREAM_TYPE;

typedef struct {
	
	ENUM_PARSE_STREAM_TYPE	Type;
	PARSE_STREAM_VOB_INFO   VOBInfo;
	
} PARSE_STREAM_VOB_INFO_PACKET; 

typedef struct {
	
	ENUM_PARSE_STREAM_TYPE	Type;		// this item MUST be the first

	unsigned int		FirstRefPicStartAddr;
	unsigned int		SecondRefPicStartAddr;
	unsigned int		ThirdRefPicStartAddr;	
	unsigned int		FirstRefPicEndAddr;
	unsigned int		SecondRefPicEndAddr;
	unsigned int		ThirdRefPicEndAddr;
	unsigned int		FieldNumber;		
	unsigned int		VOBUStartAddr;		
	unsigned int		VOBUEndAddr;			
	

	//PARSE_STREAM_VOBU_INFO  VOBUInfo;
	
} PARSE_STREAM_VOBU_INFO_PACKET; 

typedef struct {
	
	ENUM_PARSE_STREAM_TYPE	Type;
	PARSE_STREAM_ATTR_INFO  AttrInfo;
	
} PARSE_STREAM_ATTR_INFO_PACKET; 

typedef struct {
	
	ENUM_PARSE_STREAM_TYPE	Type;
	union {
		PARSE_STREAM_ATTR_INFO  AttrInfo;
		PARSE_STREAM_VOBU_INFO  VOBUInfo;
		PARSE_STREAM_VOB_INFO	VOBInfo;
	};
	
} PARSE_STREAM_INFO;

#endif

