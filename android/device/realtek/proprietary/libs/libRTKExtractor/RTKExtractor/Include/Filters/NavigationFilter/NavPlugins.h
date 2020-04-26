#ifndef __NAV_PLUGINS_H__
#define __NAV_PLUGINS_H__

/* Definitions for Nav (Input/Demux) Plugins */

#include "NavDef.h"
#include "OSAL/Threads.h"
#ifdef __cplusplus
extern "C" {
#endif

//typedef long HRESULT;
typedef int ssize_t;

    /* Nav Buffer Type Identifiers ********************************************/

    typedef enum {

        /* to indicate there is nothing in Nav Buffer */
        NAVBUF_NONE,

        /* to send data (bit-stream or private-info), use fields under "data" */
        NAVBUF_DATA,

        /* to ask nav thread to wait, use fields under "wait" */
        NAVBUF_WAIT,

        /* to ask nav thread to wait (fullness pending is kept), use fields under "wait" */
        NAVBUF_SHORT_WAIT,

        /* to ask for streaming stop, use fields under "stop" */
        NAVBUF_STOP,

        /* to flush everything buffered downstream, no arguments */
        NAVBUF_FLUSH,

        /* to begin a new bit-stream segment, use fields under "segment" */
        NAVBUF_NEW_SEGMENT,

        /* to end the current bit-stream segment, use fields under "segment" */
        NAVBUF_END_SEGMENT,

        /* to set new media type for demux, use fields under "media" */
        NAVBUF_NEW_MEDIA_TYPE,

        /* to set new av synchronization mode, use fields under "avsync" */
        NAVBUF_NEW_AVSYNC_MODE,

        /* to adjust clock in system-master av sync mode, use fields under "clock" */
        NAVBUF_ADJUST_CLOCK,

        /* to fire event, use fields under "event" */
        NAVBUF_EVENT,
        
        /* to set new video type to change new video decoder, use fields under "media" */
        NAVBUF_NEW_VIDEO_TYPE,
        
        NAVBUF_PREFETCH,

		/*to set new av synchronization mode, use fields under "avsync" 
		  and field 'reason' is valid. */
		NAVBUF_NEW_AVSYNC_MODE_EXT,
		
		/* to ask video pin to stop streaming, use fields under "stop" */
        NAVBUF_VIDEO_STOP,
        
    } NAV_BUF_ID;

    /* IO Plugin Error Codes **************************************************/

    typedef enum {

        /* generic io error, (-1) matches error code returned by read() or lseek64() */
        IOPLUGIN_ERROR_GENERIC     = -1,

        /* End-of-File error */
        IOPLUGIN_ERROR_EOF         = -2,

        /* Busy, used in asynchronous io modes or network streaming io */
        IOPLUGIN_ERROR_BUSY        = -3,

        /* error after device removal */
        IOPLUGIN_ERROR_NO_DEVICE   = -4,

        /* error in file-system */
        IOPLUGIN_ERROR_FILE_SYSTEM = -5,

        /* the requested file position already gets overwritten in a circular file */
        IOPLUGIN_ERROR_OVERWRITTEN = -6,

        /* playback catch record */
        IOPLUGIN_ERROR_CATCH_RECORD = -7,

        /* Erroneous I/O due to hardware error */
        IOPLUGIN_ERROR_EIO = -8,
        
        /* waiting for in-coming data but timeout */
        IOPLUGIN_ERROR_TIMEOUT = -9,

    } IOPLUGIN_ERROR;

	typedef enum {
         	CAUSED_BY_AUDIO_TRACK_CHANGE = 0,
    } AVSYNC_REASON;
    
    /* Nav Buffer Descriptor Block ********************************************/

    typedef struct _tagNAVBUF {

        /* identify nav buffer type */
        NAV_BUF_ID type;

        /* non-zero (true) means recepient should read another block without checking user commands */
        unsigned int bDoNotInterruptWithUserCmd;

#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
		unsigned int rtp_seq;
		unsigned int offset;
#endif

        union {

            /* if type == NAVBUF_DATA */
            struct {
                unsigned int pinIndex; /* index (0~) of the pin to deliver data */
                                       /* NOTE: pinIndex is decided by demux */

                unsigned int   channelIndex;     /* index (0~) of the channel used */
                unsigned char* payloadData;      /* bit-stream buffer (NULL means no bit-stream) */
                unsigned int   payloadSize;      /* size of payloadData in bytes (0 means no data) */
                int64_t        pts;              /* presentation time (-1 means N/A) */
                unsigned int   dataOffsetForPTS; /* offset of the bit-stream byte associated with pts */
                unsigned char  rtp_header_marker_bit;
                unsigned short rtp_header_seq_number;
                unsigned int   infoId;           /* private-info id (can be PRIVATEINFO_NONE) */
                unsigned char* infoData;         /* private-info data buffer */
                unsigned int   infoSize;         /* private-info data size in bytes */

                unsigned long startAddress; /* start address where the input block was read */
                                            /* NOTE: for parser application only, use 0xFFFFFFFF otherwise */

/* ------------ the following lines are to be deleted after new interface adopted --------------*/
                unsigned long  ArrayAddress;  /* for DV App*/
                unsigned char  Array_num;     /* for DV App*/
                unsigned long  Array_size;    /* for DV Video */
                unsigned long  dv_frame_size; /* for DV Video */
/* ---------------------------------------------------------------------------------------------*/
            } data;

            /* if type == NAVBUF_WAIT */
            struct {
                long waitTime; /* in milliseconds */
            } wait;

            /* if type == NAVBUF_STOP or type == NAVBUF_ONEPIN_STOP*/
            struct {
                unsigned int lastChannelIndex; /* index (0~) of the last channel used before stop */
            } stop;

            /* if type == NAVBUF_NEW_SEGMENT or NAVBUF_END_SEGMENT */
            struct {
                unsigned int channelIndex; /* index (0~) of the channel used */
            } segment;

            /* if type == NAVBUF_NEW_MEDIA_TYPE or NAVBUF_NEW_VIDEO_TYPE*/
            struct {
                unsigned int    channelIndex; /* index (0~) of the channel used */
                ENUM_MEDIA_TYPE mediaType;    /* new media type for demux */
            } media;

            /* if type == NAVBUF_NEW_AVSYNC_MODE or
            			NAVBUF_NEW_AVSYNC_MODE_EXT */
            struct {
                NAV_AVSYNC_MODE mode;
                AVSYNC_REASON reason;		//can be accessed only when nav type is NAVBUF_NEW_AVSYNC_MODE_EXT
            } avsync;

            /* if type == NAVBUF_ADJUST_CLOCK */
            struct {
                int __TO_BE_DEFINED__;
            } clock;

            /* if type == NAVBUF_EVENT */
            struct {
                unsigned int   eventId;   /* EVCode */
                unsigned char* eventData; /* event data buffer */
                unsigned int   eventSize; /* event data size in bytes */
            } event;
            
             /* if type == NAVBUF_PREFETCH */
            struct {
                unsigned char*  buff;
                unsigned int    size;
            } prefetch;
        };

    } NAVBUF;

    /* Structure to Update Status of Input Data for Demux Plugin **************/

    typedef struct _tagNAVDEMUXIN {

        unsigned int   channelIndex;     /* index (0~) of the channel used */
        unsigned char* pBegin;           /* mark begin of input data for demux plugin */
        unsigned char* pEnd;             /* mark end of input data for demux plugin */
        unsigned char* pCurr;            /* point to the first unused input data byte */
        int64_t        pts;              /* presentation time (-1 means N/A) */
        int64_t        Lastpts;          /* Last non -1 PTS */
        unsigned int   dataOffsetForPTS; /* offset of the bit-stream byte associated with pts */
        unsigned long  startAddress;     /* start address where the input block was read */

		/****** MINIMIZE_AUDIO_MUTED_DURATION start ******/
		
		long			videoBufFreeSpace;	/*free space in bytes of video ring buf */
		long			audioBufFreeSpace;  /* free space in bytes of audio ring buf */
		unsigned int	infoId;           	/* private-info id (can be PRIVATEINFO_NONE) */
		unsigned char*	pInfoData;         	/* private-info data buffer */
		unsigned int	infoSize;         	/* private-info data size in bytes */
		NAVBUF			navBuf;
		/****** MINIMIZE_AUDIO_MUTED_DURATION end ******/
		
		unsigned int	reservedInfoId;    	/**/
		unsigned char*	reservedInfoData;   /**/
		unsigned int	reservedInfoSize;

    } NAVDEMUXIN;

    /* Structure to Set Configuration of Demux Plugin *************************/

    typedef struct _tagNAVDEMUXCONFIG {

        int pinIndexVideo;    /* video pin index */
        int pinIndexAudio;    /* audio pin index */
        int pinIndexSpic;     /* sub-picture pin index */
        int pinIndexTeletext; /* teletext pin index */

    } NAVDEMUXCONFIG;

    /* Structure to Receive Output from Demux Plugin **************************/

    typedef struct _tagNAVDEMUXOUT {

        NAVBUF*      pNavBuffers;  /* point to the start of NAVBUF array */
        unsigned int numBuffers;   /* number of NAVBUF items in array */
        unsigned char	rtp_header_marker_bit;
        unsigned short	rtp_header_seq_number;

    } NAVDEMUXOUT;

    /* Structure for HTTP Live Streaming(HLS) clip information ****************/

    typedef struct _tagNAVIOCLIPINFO 
    {
        int64_t clipSize;        //in bytes
        int64_t clipDuration;    //in milli seconds
    } NAVIOCLIPINFO;

    /* Structure for HTTP Live Streaming(HLS) subtitle information ************/

    typedef struct _tagNAVSUBTITLE
    {
        char* pLanguage;
        char* pURL;
    } NAVSUBTITLE;

    typedef struct _tagNAVSUBTITLEINFO 
    {
        unsigned int iCounts;    // The counts of subtitle information
        NAVSUBTITLE* pSubtitle;  // An array of NAVSUBTITLE.
    } NAVSUBTITLEINFO;

    /* Structure for HTTP Live Streaming(HLS) thumbnail information ***********/

    typedef struct _tagNAVTHUMBNAILINFO 
    {
        unsigned int iCounts;    // The counts of thumbnail
        unsigned int iInterval;  // Thumbnail's interval
        char* pURL;
    } NAVTHUMBNAILINFO;

    /* IO Plugin Descriptor Block *********************************************/

    // This function will be called when IO Plug-in needs authentication information.
    //
    //  Parameters:
    //      1. pAccountBuf: Copy the account string into this buffer.
    //      2. iAccountBufSize: The size of pAccountBuf.
    //      3. pPwdBuf: Copy the password string into this buffer.
    //      4. iPwdBufSize: The size of pPwdBuf.
    //      5. iRertyCounts: The retry counter for this authentication.
    //                       For example: If user inputs wrong ID/PWD, iRertyCounts will be 1 
    //                                    when the second calling of this function.
    //                                    This means that if iRertyCounts > 0, the user inputs
    //                                    wrong ID/PWD.
    //
    //  Return:
    //      true --> Success. IO Plug-in will use this account and password to authenticate user.
    //      false --> Failed. IO Plug-in will abort this connection.
    //

	typedef struct _tagIOBUFFERINFO
	{
		int bufferSize;	// Actual size of the IO buffer
		int dataSize;	// Current size of data in the IO buffer
	} IOBUFFERINFO;

	typedef enum {
        IO_ERROR_TYPE_NONE,
        IO_ERROR_TYPE_HTTP_NOT_CONNECT,
        IO_ERROR_TYPE_HTTP_TIMEOUT,
        IO_ERROR_TYPE_HTTP_ERROR,
        IO_ERROR_TYPE_HTTP_ERROR_STATUS
    } IO_ERROR_TYPE;

	typedef struct _tagIOErrorStatus
	{
		IO_ERROR_TYPE errorType;
		int statusCode;
	} IOErrorStatus;

    typedef bool (*fpGetAuthInfo)(char* pAccountBuf, int iAccountBufSize, 
                                  char* pPwdBuf, int iPwdBufSize,
                                  int iRertyCounts);
    
    typedef struct _tagIOPLUGIN {

        /* IO plugin has no need to have any instance.
         * Suppose we should remove this field.
         * It is reserved for any possibility. */
        long pInstance;

        /* open file */
        void* (*open)(char* path, int b_direct_io);

        /* get IO info */
        void (*getIOInfo)(void* pInstance, NAVIOINFO *info);

        /* set blocking/non-blocking mode and time-out (-1 means no time-out and wait forever) */
        void (*setBlockingMode)(void* pInstance, int bBlocking, int timeout);

        /* keep reading */
        int (*read)(void* pInstance, unsigned char* pBuffer, int size, NAVBUF* pEventReceived);

        /* seek to a new position */
        long long (*seek)(void* pInstance, long long offset, int origin);

        /*seek to a new positon by time, for rtsp*/
        long long (*seek1)(void* pInstance, long long t_offset);
        
        /* close file */
        int (*close)(void* pInstance);

        /* dispose this plugin */
        int (*dispose)(void* pInstance);
        
#ifndef DISABLE_IOFULLNESS	// for IO plugin buffer fullness requirement
		// fullness: buffer data size (in byte)
		// return value: 1 -> success, 0 -> fail
		int (*getBufferFullness)(void* pInstance, int* fullness, int* pFlag);
		int (*getBufferInfo)(void* pInstance, IOBUFFERINFO* pIOBufferInfo);
#endif
        
            /*
             * open another sub stream within an existing IOPlugin,
             * params:
             *      pInstance: the existing IOPlugin, must not be NULL
             *      path: if NULL, the new stream will be the same url with
             *            the original IOPlugin. Otherwise, the new stream
             *            will be based on new path.
             *      b_direct_io: 1 to open as directo IO, 0 otherwise
             *      offset: initial offset of the new open stream.
             * return:
             *      integer >= 0, to be used as fd for getIOInfoN/readN
             *      negative int, indicates error
             */
        int (*openN) (void *pIstance, char * path, int b_direct_io, long long offset);

            /*
             * retrieve the substream information (info seekable etc). 
             * params:
             *       pInstance: the IOPlugin. cannot be NULL
             *       fd: int returned from openN, denoding of the substream.
             */
        void (*getIOInfoN)(void *pIstance, int fd, NAVIOINFO *info);

            /*
             * Seek to an offset in the substream. 
             * params:
             *       pInstance: the IOPlugin. cannot be NULL
             *       fd: int returned from openN, descriptor of the substream.
             *       offset: which offset to seek to
             *       origin: SEEK_END, SEEK_SET etc.
             * return:
             *       the offset of the substream after seek.
             *       negative number means error
             */
        long long (*seekN)(void *pIstance, int fd, long long offset, int origin);
            /*
             * Read from a substream. 
             * params:
             *       pInstance: the IOPlugin. cannot be NULL
             *       fd: int returned from openN, descriptor of the substream.
             *       pBuffer: buffer to store the read
             *       size: length of buffer.
             *       pEventReceived: event received
             * return:
             *       positive number of bytes read
             *       negative number for error
             */
        int (*readN)(void *pIstance, int fd, unsigned char* pBuffer, int size, NAVBUF* pEventReceived);
            /*
             * Close a specific substream.
             * params:
             *       pInstance: the IOPlugin. cannot be NULL
             *       fd: int returned from openN, descriptor of the substream.
             * return:
             *
             * note: calling close on the original IOPlugin will close all
             * sub-streams on the IOPlugin.
             */      
        int (*closeN)(void *pIstance, int fd);

        // Seek to a new position by time.
        //
        //  Parameters: 
        //      1. pInstance: The handle of the IO Plugin.
        //      2. timeOffset: Time offset, time unit is milli-second
        //
        //  Return:
        //      The new position in time, time unit is milli-second
        //
        long long (*seekByTime)(void* pInstance, long long timeOffset);

        // Get specific clip info
        //
        //  Parameters:
        //      1. pInstance: The handle of the IO Plugin.
        //      2. iTimeStamp: An absolute timestamp in milli seconds to indicate which clip I want. 
        //      3. pInfo: Contain clip info when function returned
        //
        //  Return:
        //      true --> if clip is found and info is available.
        //
        bool (*getClipInfo)(void* pInstance, int iTimeStamp, NAVIOCLIPINFO* pClipInfo);
        
        // Get subtitle info
        //
        //  Parameters:
        //      1. pInstance: The handle of the IO Plugin.
        //
        //  Return:
        //      Subtitle information. NOTE: NAVSUBTITLEINFO.pSubtitle is an array of NAVSUBTITLE.
        //      NULL --> Failed.
        //
        NAVSUBTITLEINFO* (*getSubtitleInfo)(void* pInstance);
        
        // Get thumbnail info
        //
        //  Parameters:
        //      1. pInstance: The handle of the IO Plugin.
        //
        //  Return:
        //      Thumbnail information.
        //      NULL --> Failed.
        //
        NAVTHUMBNAILINFO* (*getThumbnailInfo)(void* pInstance);

        // Set the callback function for IO Plug-in to get the authentication information.
        //
        //  Parameters:
        //      1. GetAuthInfo: Function pointer to the callback function.
        //
        // NOTE: YOU SHOULD CALL THIS FUNCTION BEFORE OPENING CONNECTION!!
        // NOTE: When you dose not use this callback(GetAuthInfo) anymore, 
        //       please reset it by calling setAuthCallback(NULL).
        //       IO Plug-in will not reset this callback(GetAuthInfo) for you.
        //
        void (*setAuthCallback)(fpGetAuthInfo GetAuthInfo);
        
        int (*getReadPtr)(void* pInstance, unsigned char** ppBuffer, int size, NAVBUF* pEventReceived);
        
//        ssize_t (*IOreadAt)(off64_t offset, void *data, size_t size);
        ssize_t (*IOreadAt)(void* pInstance,  ssize_t (*funcPtr)(off64_t offset, void *data, size_t size));
        int IOuse_fd;
    } IOPLUGIN;

    /* Input Plugin Descriptor Block ******************************************/

    typedef struct _tagINPUTPLUGIN {

        /* plugin object instance */
        void* pInstance;

        /* load media */
        HRESULT (*loadMedia)(
            void* pInstance,
            char* path,
            bool* pbContinue,
            NAVLOADFAILURE* pFailure);

        /* unload media */
        HRESULT (*unloadMedia)(
            void* pInstance);

        /* report information of the media loaded by input plugin */
        HRESULT (*getMediaInfo)(
            void* pInstance,
            NAVMEDIAINFO* pMediaInfo);

        /* read from input plugin for playable unit or commands */
        HRESULT (*read)(
            void* pInstance,
            NAVBUF* pBuffer,
            unsigned int prohibitedChannelMask,
            NAV_STREAM_TYPE* channelFullnessInfo);

        /* update playback position */
        HRESULT (*updatePlaybackPosition)(
            void* pInstance,
            PRESENTATION_POSITIONS* pPosition,
            NAVDEMUXPTSINFO* pDemuxPTSInfo);

        /* private data feedback for input plugin's reference */
        HRESULT (*privateDataFeedback)(
            void* pInstance,
            unsigned int id,
            unsigned char* data,
            unsigned int size,
            int64_t lastPTS);

        /* ask input plugin to execute user commands */
        HRESULT (*execUserCmd)(
            void* pInstance,
            NAV_CMD_ID id,
            void* cmdData,
            unsigned int cmdDataSize,
            unsigned int* pIsFlushRequired);

        /* property set/get for input plugin */
        HRESULT (*propertySetGet)(
            void* pInstance,
            NAV_PROP_ID id,
            void* inData,
            unsigned int inDataSize,
            void* outData,
            unsigned int outDataSize,
            unsigned int* returnedSize);

        /* to dispose this input plugin */
        HRESULT (*dispose)(
            void* pInstance);

        HRESULT (*identify)(void* pInstance,
                            char* path,
                            unsigned char* streamBuffer,
                            unsigned int streamBufferBytes,
                            unsigned int streamBufferOffset);

        HRESULT (*registerIOPlugin)(void* pInstance, 
						IOPLUGIN* ioPlugin, 
						bool b_opening, 
						bool b_direct_io,
						void* fh);

#ifndef DISABLE_IOFULLNESS	// for IO plugin buffer fullness requirement
	// fullness: buffer data size (in byte)
	HRESULT (*getIOBufferFullness)(void* pInstance, int* fullness, int* pFlag);		
#endif
    } INPUTPLUGIN;

    /* Demux Plugin Descriptor Block ******************************************/

    typedef struct _tagDEMUXPLUGIN {

        /* plugin object instance */
        void* pInstance;

        /* parse input data and demultiplex */
        HRESULT (*parse)(
            void* pInstance,
            NAVDEMUXIN* pDemuxIn,
            NAVDEMUXOUT* pDemuxOut);

        /* handle or dispatch private info */
        HRESULT (*handlePrivateInfo)(
            void* pInstance,
            unsigned int infoId,
            unsigned char* infoData,
            unsigned int infoSize);

        /* flush demux plugin (drop any buffered stuff) */
        HRESULT (*flush)(
            void* pInstance);

        /* property set/get for demux plugin */
        HRESULT (*propertySetGet)(
            void* pInstance,
            NAV_PROP_ID id,
            void* inData,
            unsigned int inDataSize,
            void* outData,
            unsigned int outDataSize,
            unsigned int* returnedSize);

        /* to dispose this demux plugin */
        HRESULT (*dispose)(
            void* pInstance);

    } DEMUXPLUGIN;

    /* Definitions of Entry Function to Open Input/Demux/IO Plugin ************/

    typedef HRESULT (*PFUNC_OPEN_INPUT_PLUGIN)(
        ENUM_MEDIA_TYPE majorType,   /* IN:  media major type */
        ENUM_MEDIA_TYPE subType,     /* IN:  media sub type */
        osal_mutex_t*   mutex,       /* IN:  reference to the streaming lock */
        INPUTPLUGIN*    pInputPlugin /* OUT: the input plugin instance created */
    );

    typedef HRESULT (*PFUNC_OPEN_DEMUX_PLUGIN)(
        ENUM_MEDIA_TYPE type,         /* IN:  media type */
        int             channelIndex, /* IN:  channel index */
        INPUTPLUGIN*    pInputPlugin, /* IN:  reference to upstream input plugin */
        NAVDEMUXCONFIG* pDemuxConfig, /* IN:  struct to set demux configuration */
        DEMUXPLUGIN*    pDemuxPlugin  /* OUT: the demux plugin instance created */
    );

    typedef HRESULT (*PFUNC_OPEN_IO_PLUGIN)(
        IOPLUGIN* pIOPlugin
    );

    #define FUNCNAME_OPEN_INPUT_PLUGIN      NavOpenInputPlugin
    #define FUNCNAME_OPEN_DEMUX_PLUGIN      NavOpenDemuxPlugin
    #define STR_FUNCNAME_OPEN_INPUT_PLUGIN "NavOpenInputPlugin"
    #define STR_FUNCNAME_OPEN_DEMUX_PLUGIN "NavOpenDemuxPlugin"

#ifdef __cplusplus
}
#endif

#endif /*__NAV_PLUGINS_H__*/
