#ifndef __RM_READER_H__
#define __RM_READER_H__

#include <stdio.h>
#include <stdlib.h>
#include <Filters/NavigationFilter/NavPlugins.h>

/***********************************************************************/
//get data info if streaming mode is SEQUENTIAL
#define RM_GET_SEQ_MODE_DATA_INFO(_pRM)	(&(_pRM->reader.info[0]))

//get data info if streaming mode is AV_APART_ONE/TWO_FH
#define RM_GET_VIDEO_DATA_INFO(_pRM)	(&(_pRM->reader.info[0]))
#define RM_GET_AUDIO_DATA_INFO(_pRM)	(&(_pRM->reader.info[1]))

/***********************************************************************/
#define IS_RM_VIDEO_STREAM(streamNum)		(pRM->videoStreamNum != 0xFFFF && streamNum == pRM->videoStreamNum)
#define IS_RM_AUDIO_STREAM(streamNum)		(pRM->audioStreamNum != 0xFFFF && streamNum == pRM->audioStreamNum)
#define RM_HAS_VIDEO_STREAM(_pRM)			(_pRM->videoStreamNum != 0xFFFF)
#define RM_HAS_AUDIO_STREAM(_pRM)			(_pRM->audioStreamNum != 0xFFFF)

/***********************************************************************/

#define RM_HAS_VIDEO_INDEX_TABLE(_pRM)	((_pRM->pIndexChunk != NULL) &&	\
										  RM_HAS_VIDEO_STREAM(_pRM) && 	\
										  (_pRM->pIndexChunk[_pRM->videoStreamNum].pIndexRecord != NULL))

#define RM_HAS_AUDIO_INDEX_TABLE(_pRM)	((_pRM->pIndexChunk != NULL) &&	\
										  RM_HAS_AUDIO_STREAM(_pRM) && 	\
										  (_pRM->pIndexChunk[_pRM->audioStreamNum].pIndexRecord != NULL))

#define RM_HAS_INDEX_TABLE(_pRM)	(RM_HAS_VIDEO_INDEX_TABLE(_pRM) || RM_HAS_AUDIO_INDEX_TABLE(_pRM))

/***********************************************************************/
#define RM_IS_SEEKABLE(_pRM)	(pRM->reader.flag.bIOSeekable)

/***********************************************************************/

#define RM_INIT_DATA_INFO(_pInfo)		\
do{										\
	_pInfo->pBuffer				= 0;	\
	_pInfo->subFileHandler		= -1;	\
	_pInfo->dataChunkBegin 		= 0;	\
	_pInfo->dataChunkEnd 		= -1;	\
	_pInfo->dataOffset			= 0;	\
	_pInfo->dataSize			= 0;	\
	_pInfo->currReadOffset 		= 0;	\
	_pInfo->totalReadBytes 		= 0;	\
	_pInfo->currFilePos	   		= 0;	\
	_pInfo->phyFilePos			= 0;	\
	_pInfo->lastDemuxTimestamp	= -1;	\
	_pInfo->prevJumpTimestamp 	= -1;	\
}while(0)
/***********************************************************************/

typedef enum
{
	SEQUENTIAL = 0,
	AV_APART_ONE_FH = 1,
	AV_APART_TWO_FH = 2
	
} RM_STREAMING_MODE ;

typedef struct rm_data_info
{
	unsigned char* 	pBuffer; 
	int				subFileHandler; // -1 means use open(), read(), seek() etc
	unsigned long	dataOffset;		// start offset in bytes from the start of the file
	unsigned long	dataSize;		
	unsigned long	dataChunkBegin;
	unsigned long	dataChunkEnd;
	
	int				totalReadBytes;		/* the total bytes in the input internal buffer */
	int				currReadOffset;		/* the offset from the start of the input internal buffer */
    long long		currFilePos;		/* current reading file position */
    long long		phyFilePos;
    
    int 			lastDemuxTimestamp;	//the latest demuxed timestamp
    int				prevJumpTimestamp;	//the latest timestamp where we jump 
    
}rm_data_info_t;

typedef struct rm_reader
{
	RM_STREAMING_MODE streamingMode;
	IOPLUGIN* 		  ioPlugin;
	void*			  fileHandler;
	long long		  phyFilePos;
	
	struct {
		unsigned char bIOSeekable  : 1;
		unsigned char bUseDirectIO : 1;
		unsigned char bOpenMany    : 1;
	} flag;
    
	rm_data_info_t 	  info[2];
} rm_reader_t;


/***********************************************************************/
typedef enum {
	INDEX_TYPE_VIDEO = 0,
	INDEX_TYPE_AUDIO = 1,
	INDEX_TYPE_ANY	 = 2
} RM_INDEX_TYPE;


#endif	//__RM_READER_H__
