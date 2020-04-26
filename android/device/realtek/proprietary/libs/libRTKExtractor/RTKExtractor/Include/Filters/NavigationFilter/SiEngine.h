#ifndef __SI_ENGINE_H__
#define __SI_ENGINE_H__

/* Definitions for SI Engine */

#include <FlowEvent.h>

#define SI_MAX_FILTER_NUMBER  64 // to fix bug 17273  

//#if IS_CHIP(MARS) || IS_CHIP(JUPITER)|| IS_CHIP(SATURN)
#define ADD_BLURAY_SP					
//#endif

#ifdef DVB_T
#define FIXBUG_13912
#define FEATURE_001		// get a new PAT table
#define FIXBUG_1637
#define FIXBUG_17293
#define FIXBUG_2551
#define FIXBUG_17588
#define FIXBUG_17756
#define FIXBUG_16914
#define FIXBUG_17802
#define FIXBUG_18179

#define FIXBUG_1720		// It happen that same channel number
#define FIXBUG_2500		// The channel DB should divide into TV and Radio 
#define FIXBUG_1117		// Sometimes, it doesn't support the logical channel number 

#define FEATURE_002		// PDD signaling mechanism
#define FEATURE_010	// add SI_SetTimeOffset and SI_GetTimeOffset api
#define ADD_MULTISEC
#define SUPPORT_UNDEFINED_SERVICE_TYPE

#define SUPPORT_AVS_CODEC

#define ENABLE_PARSE_EXTENDED_EVENT_DESCRIPTOR


#endif

#ifdef __cplusplus
extern "C" {
#endif

    /* SI Filter Types ********************************************************/

    typedef enum {

        SI_FILTER_UNKNOWN = 0,
        SI_FILTER_SI,
        SI_FILTER_VIDEO,
        SI_FILTER_AUDIO,
        SI_FILTER_SUBTITLE,
        SI_FILTER_TELETEXT,
		SI_FILTER_CHECK_SCRAMBLE,
		SI_FILTER_IG,
		SI_FILTER_TEXTST,
	SI_FILTER_VIDEO_2ND,	
    } SI_FILTER_TYPE;

    /* SI Codec Types *********************************************************/

    typedef enum {

        SI_CODEC_UNKNOWN = 0,
        SI_CODEC_VIDEO_11172,
        SI_CODEC_VIDEO_13818,
        SI_CODEC_VIDEO_14496,
        SI_CODEC_VIDEO_H264,
        SI_CODEC_VIDEO_H265,    // H265/HEVC
        SI_CODEC_VIDEO_VC1,		// privateData (int) : VC1 profile ( privatDataSize = sizeof(int))
       
        
        SI_CODEC_AUDIO_11172,
        SI_CODEC_AUDIO_13818,
        SI_CODEC_AUDIO_AC3,
        SI_CODEC_AUDIO_E_AC3,
        SI_CODEC_AUDIO_DTS,
        SI_CODEC_AUDIO_AAC,
        SI_CODEC_AUDIO_LATM_AAC,
		SI_CODEC_AUDIO_LPCM,	// privateData (LPCM_SETTING)
		SI_CODEC_AUDIO_HDMV_MLP,		// trueHD
		SI_CODEC_AUDIO_HDMV_AC3,
		SI_CODEC_AUDIO_HDMV_DDP,
		SI_CODEC_AUDIO_HDMV_DTS,
		SI_CODEC_AUDIO_HDMV_DTS_HD,
		
#ifdef ADD_BLURAY_SP
		SI_CODEC_SP_HDMV,
#else
		//used to keep each value behind this consistent if ADD_BLURAY_SP undefined.
		SI_CODEC_SP_HDMV_UNUSED,
#endif		

//For bug 21306. For backward compatible, it is STRONGLY recommended
//to add a new codec type in the end.
//#ifdef SUPPORT_AVS_CODEC
		SI_CODEC_VIDEO_AVS,
//#endif
		SI_CODEC_IG_HDMV,
		
		
		SI_CODEC_VIDEO_MVC_DEPENDENT,
		
		SI_CODEC_AUDIO_WFD_LPCM,
        SI_CODEC_DEFAULT,
        SI_CODEC_OTHERS

    } SI_CODEC_TYPE;

	typedef struct _tagLPCM_SETTING
	{
		int bitsPerSample;
		int numberOfChannels;
		int samplingRate;
		char bHDMV;
		
	} LPCM_SETTING;
	
    /* return value of CheckScramble ******************************************/
    typedef enum {
    	SCRAMBLE_CHECK_UNKNOWN_PID,		/* the queried PID is not inserted yet. */
    	SCRAMBLE_CHECK_NOT_READY,		/* the scramble control info is not ready. please wait a minute. */
    	SCRAMBLE_CHECK_OK				
    	
    } SI_SCRAMBLE_CHECK_RESULT;

    /* SI Filter Description **************************************************/

    typedef struct _tagSI_FILTER_DESC {

        int            pid;
        int            bActive; /* 1 - this pid is for the active stream among its type; 0 - otherwise */
        SI_FILTER_TYPE type;
        SI_CODEC_TYPE  codecType;

		int				privateDataSize;
		void*			privateData;


    } SI_FILTER_DESC;
    
    /* SI Engine Private Info ID *******************************************/
	typedef enum {
		SI_PRIVATE_DTCP, 	//SI_INFO_DTCP
		
	} SI_PRIVATE_INFO_ID;
	
    /* SI Engine Outgoing Interface *******************************************/

        /* Note: for each function in this interface, it returns 1 to indicate successful execution, and 0 on failure */
        /*       (in most cases, a failure is not expected nor acceptable, so the caller may simply ignore the return value */

    typedef struct _tagSI_ENGINE_OUT {

        /**
		* \fn (*InsertFilterList)(unsigned int navHandle, unsigned int count, SI_FILTER_DESC* filterDescArray);
		*
		* \brief insert pid filter list
		*
		* \return int:
		*		1 to indicate successful execution, and 0 on failure.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		* \param count:
		*		indicate how many entries in filterDescArray.	
		*
		* \param filterDescArray:
		*		a list of PID filter
		*/
        int (*InsertFilterList)(
            unsigned int    navHandle,
            unsigned int    count,
            SI_FILTER_DESC* filterDescArray);

        /**
		* \fn (*RemoveFilterList)(unsigned int navHandle, unsigned int count, unsigned int* pidArray, SI_FILTER_TYPE* typeArray);
		*
		* \brief remove pid filter by PID, type or both.
		*		 If PID and type are both specified, the PID filter which matches PID and type will be removed.
		*		 If only PID is specified, the PID filter which matches PID will be removed.
		*		 If only type is specified, those PID filters which match type will be removed.
		*
		* \return int:
		*		1 to indicate successful execution, and 0 on failure.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		* \param count:
		*		If pidArray and typeArray are not null, count is the entry number of pidArray and typeArray.
		*		If pidArray (or typeArray) is null, count is the entry number of typeArray (or pidArray).
		*
		* \param pidArray:
		*		a list of PID.
		*
		* \param typeArray:
		*		a list of PID type.
		*/
        int (*RemoveFilterList)(
            unsigned int  navHandle,
            unsigned int  count,
            unsigned int* pidArray,
            SI_FILTER_TYPE* typeArray);

        /**
		* \fn int (*RemoveAllFilters)(unsigned int navHandle)
		*
		* \brief remove all pid filters
		*
		* \return int:
		*		1 to indicate successful execution, and 0 on failure.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		*/
        int (*RemoveAllFilters)(
            unsigned int navHandle);

        /**
		* \fn int (*SetActiveComponent)(unsigned int navHandle, SI_FILTER_TYPE type, int pid);
		*
		* \brief set new active component (or set all components inactive) among a filter type
		*
		* \return int:
		*		1 to indicate successful execution, and 0 on failure.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		* \param type:
		*		pid type
		*
		* \param pid:
		*		if pid >= 0, it identifies the new active stream among the type (SI_FILTER_TYPE)
        *       if pid < 0 (ex: -1), that means none of the streams are active in this type
		*
		*/
        int (*SetActiveComponent)(
            unsigned int   navHandle,
            SI_FILTER_TYPE type,
            int            pid); 

        /**
		* \fn  int (*PostEvent)(unsigned int navHandle, unsigned int eventId, unsigned char* eventData, unsigned int eventSize); 
		*
		* \brief post flow event to APP
		*
		* \return int:
		*		1 to indicate successful execution, and 0 on failure.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		* \param eventId:
		*		EVCode (defined in "FlowEvent.h")
		*
		* \param eventData:
		*		event data buffer (NULL if not needed)
		*
		* \param eventSize:
		*		event data size in bytes (should be 0 or integral multiple of 4)
	 	*/
        int (*PostEvent)(
            unsigned int   navHandle,
            unsigned int   eventId,    
            unsigned char* eventData,
            unsigned int   eventSize); 

        /**
		* \fn  int (*SetSiState)(unsigned int navHandle, unsigned char* pStateData, unsigned int bytes)
		*
		* \brief set new SI state
		*
		* \return int:
		*		1 to indicate successful execution, and 0 on failure.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		* \param pStateData:
		*		the SI state 
		*
		* \param bytes:
		*		size of the SI state.
	 	*/
        int (*SetSiState)(
            unsigned int   navHandle,
            unsigned char* pStateData,
            unsigned int   bytes);

		 
		/**
		* \fn  SI_SCRAMBLE_CHECK_RESULT (*CheckScramble)(unsigned int navHandle, int pid, int* pIsScramble)
		*
		* \brief query if the stream with the specified PID is scramble
		*
		* \return SI_SCRAMBLE_CHECK_RESULT:
		*		the result of this query. The data of *pIsScramble is valid 
		*		only when return value is SCRAMBLE_CHECK_OK.
		*
		* \param navHandle:
		*		handle of the nav outside
		*
		* \param pid:
		*		the stream PID we want to query 
		*
		* \param pIsScramble:
		*		1 means this stream is scramble while 0 is nonscramble.
	 	*/
        SI_SCRAMBLE_CHECK_RESULT (*CheckScramble)(
        	unsigned int   navHandle,
        	int			 pid,
        	int*		 pIsScramble);
        
		/**
		* \fn  int (*ResetPosition)(unsigned int navHandle)
		*
		* \brief Reset file position to the beginning. 
		*		 It only has effect when SI at file scanning mode (scan video and audio PID).
		*
		* \return int:
		*		return 1 to indicate successful execution, and 0 on failure
		*
		* \param navHandle:
		*		handle of the nav outside
		*
	 	*/
		int (*ResetPosition)(
        	unsigned int   navHandle);
        	
        /**
		* \fn  int (*FeedbackPrivateInfo)(unsigned int   navHandle, SI_PRIVATE_INFO_ID id, long infoLen, unsigned char* pInfo)
		*
		* \brief Feedback private info (for example, some data in descriptor) to input plugin.
		*
		* \return void
		*
		* \param navHandle:
		*		handle of the nav outside
		*
	 	* \param id:
		*		private info id
		*
		* \param infoLen:
		*		length of pInfo
		*
		* \param pInfo:
		*		a pointer to private info.
		*
	 	*/
        void (*FeedbackPrivateInfo)(
        	unsigned int   navHandle, 
        	SI_PRIVATE_INFO_ID id, 
        	long infoLen, 
        	unsigned char* pInfo);
        	
    } SI_ENGINE_OUT;

    /* SI Engine Incoming Interface *******************************************/

        /* Note: for each function in this interface, it returns 1 to indicate successful execution, and 0 on failure */
        /*       (in most cases, a failure is not expected nor acceptable, so the caller may simply ignore the return value */

    typedef struct _tagSI_ENGINE_IN {

        /**
		* \fn  int (*StoreSiPacket)(unsigned int siHandle, unsigned char* packetBuf, unsigned int bufLen)
		*
		* \brief send SI packets into SI engine.
		*
		* \return int:
		*		return 1 to indicate successful execution, and 0 on failure
		*
		* \param siHandle:
		*		handle of the SI engine
		*
		* \param packetBuf:
		*		SI packet which pass from PID filters
		*
		* \param bufLen:
		*		the size of the SI packet
	 	*/
        int (*StoreSiPacket)(
            unsigned int   siHandle,
            unsigned char* packetBuf,
            unsigned int   bufLen);

        
        /**
		* \fn  int (*StoreSiState)(unsigned int siHandle, unsigned char* pStateData, unsigned int bytes)
		*
		* \brief assign SI state back to SI engine.
		*
		* \return int:
		*		return 1 to indicate successful execution, and 0 on failure
		*
		* \param siHandle:
		*		handle of the SI engine
		*
		* \param pStateData:
		*		SI state which comes from SI engine by SetSiState()
		*
		* \param bytes:
		*		the size of the SI state
	 	*/
        int (*StoreSiState)(
            unsigned int   siHandle,
            unsigned char* pStateData,
            unsigned int   bytes);

        
	    /**
		* \fn  void (*Flush)(unsigned int siHandle)
		*
		* \brief flush partial SI data due to stream discontinuity (ex: time-seek).
		*
		* \return none
		*
		* \param  siHandle:
		*		handle of the SI engine
	 	*/
        void (*Flush)(
            unsigned int siHandle);

#ifdef FEATURE_001
		/**
		* \fn  int (*GetPAT)(unsigned int siHandle, unsigned char* pOldPat, unsigned char* pNewPat)
		*
		* \brief get new PAT packet if it indeed to be modified
		*
		* \return int
		*		return 2 to indicate successful execution, and pNewPat filled with new data to replace pOldPat value.
		*		return 1 to indicate successful execution, and pOldPat has been updated(NOTE: MUST DO pli_flushMemory(pOldPat, 188)).
		*		return 0 on failure
		*
		* \param  pOldPat:
		*		the pointer of original PAT packet(188 bytes)
		*
		* \param  pNewPat:
		*		the pointer of new PAT packet(188 bytes)
	 	*/
        int (*GetPAT)(
            unsigned int siHandle,
            unsigned char* pOldPat,
            unsigned char* pNewPat);
#endif // FEATURE_001 

    } SI_ENGINE_IN;

    /* Definitions of Entry Functions to Create/Destroy SI Engine *************/

        /* Note: these functions return 1 to indicate successful execution, and 0 on failure */

    int CreateSiEngine( /* FIXME: OBSELETE */
        char*          path,         /* IN:  URL path for media initialization */
        unsigned int   navHandle,    /* IN:  handle of the nav outside */
        SI_ENGINE_OUT* pSiEngineOut, /* IN:  outgoing interface for SI engine */
        unsigned int*  pSiHandle,    /* OUT: handle of the SI engine */
        SI_ENGINE_IN*  pSiEngineIn   /* OUT: incoming interface for SI engine */
    );

    int DestroySiEngine( /* FIXME: OBSELETE */
        unsigned int siHandle /* IN: handle of the SI engine */
    );

    int AttachSiEngine(
        unsigned int   navHandle,    /* IN:  handle of the nav outside */
        SI_ENGINE_OUT* pSiEngineOut, /* IN:  outgoing interface for SI engine */
        unsigned int   siHandle,     /* IN:  handle of the SI engine */
        SI_ENGINE_IN*  pSiEngineIn   /* OUT: incoming interface for SI engine */
    );

    int DetachSiEngine(
        unsigned int siHandle /* IN: handle of the SI engine */
    );

	/* Definitions of SI Private Info *************/
	typedef struct {
		unsigned int CCI;
		unsigned int APS;
		
	} SI_INFO_DTCP;
	
#ifdef __cplusplus
}
#endif

#endif /*__SI_ENGINE_H__*/
