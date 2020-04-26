#ifndef _RTK_NAV_DEF_H_
#define _RTK_NAV_DEF_H_

    /**
     * @20140422: copy from src/Include/Filters/NavigationFilter/NavDef.h (DvdPlayer source)
     */
    /* Nav Property Identifiers ***********************************************/

    typedef enum {

        /* general property get/set for every input plugin */

        NAVPROP_INPUT_GET_PLAYBACK_STATUS, /* [out] NAVPLAYBACKSTATUS                     */
        NAVPROP_INPUT_GET_VIDEO_STATUS,    /* [out] NAVVIDEOSTATUS                        */
        NAVPROP_INPUT_GET_AUDIO_STATUS,    /* [out] NAVAUDIOSTATUS                        */
        NAVPROP_INPUT_GET_SPIC_STATUS,     /* [out] NAVSPICSTATUS                         */
        NAVPROP_INPUT_GET_MENU_STATUS,     /* [out] NAVMENUSTATUS                         */
        NAVPROP_INPUT_GET_DISC_STATUS,     /* [out] NAVDISCSTATUS                         */
        NAVPROP_INPUT_GET_TITLE_STATUS,    /* [out] NAVTITLESTATUS [in] int (title index) */
        NAVPROP_INPUT_GET_NAV_STATE,       /* [out] variable size block                   */
        NAVPROP_INPUT_GET_CMD_BLK_FLAGS,   /* [out] int, bit mask of NAV_CMD_BLK members  */
        NAVPROP_INPUT_GET_MEDIA_SIGNATURE, /* [out] 16 unsigned char's                    */
        NAVPROP_INPUT_GET_PRIVATE_INFO_FB, /* [out] NAVBUF to carry private info feedback */
        NAVPROP_INPUT_GET_TIME_POSITION,   /* [out] int64_t (file offset) [in] unsigned int (elapsed seconds) */
        NAVPROP_INPUT_GET_NET_INFO,        /* [out] NAVNETINFO */

        NAVPROP_INPUT_SET_NAV_STATE,        /* [in] variable size block              */
        NAVPROP_INPUT_SET_NAV_STATE_FORCED, /* [in] variable size block              */
        NAVPROP_INPUT_SET_REPEAT_MODE,      /* [in] NAV_REPEAT_MODE                  */
        NAVPROP_INPUT_SET_RATE_INFO,        /* [in] long (playback rate information) */
        NAVPROP_INPUT_SET_VIDEO_PRES_MODE,  /* [in] NAV_VIDEO_PRES_MODE              */
        NAVPROP_INPUT_SET_RINGBUFFER_INFO,  /* [in] NAVRINGBUFFERINFO                */
        NAVPROP_INPUT_SET_FLOW_INTERFACE,   /* [in] IFlowManager*                    */

        NAVPROP_INPUT_SET_THUMBNAIL_TIME_POS,   /* [in] unsigned int, the time position(in seconds) of the thumbnail */
        NAVPROP_INPUT_SET_DECODE_THUMBNAIL,     /* [in] NAVTHUMBNAILPARA */
        NAVPROP_INPUT_SET_DECODE_MULTI_THUMBNAIL, /*[in] NAVMULTITHUMBNAILPARA */

        NAVPROP_INPUT_SET_PLAYBACK_MODE,            /* [in] NAV_PLAYBACK_MODE*/
        NAVPROP_INPUT_SET_LPCM_INFO,        /* [in] NAVLPCMINFO     LPCM info setting*/

        NAVPROP_INPUT_SET_ERROR_CONCEALMENT_LEVEL,

        NAVPROP_INPUT_SET_AUDIO_PREFERENCE,         /* [in] ENUM_MEDIA_TYPE */
        NAVPROP_AUDIO_GET_PTS,                  /* [out] int64_t */

        NAVPROP_INPUT_SET_EIO_LEVEL,            /* [in] long (0: fatal error, 1: read error) */

        /* special property get/set for DVD-Video nav input plugin */

        NAVPROP_DVDV_GET_BUTTON_JUMP_TARGET, /* [out] int (title index)                         */
        NAVPROP_DVDV_GET_PLAYBACK_STATUS_EX, /* [out] NAVPLAYBACKSTATUS_DVDV                    */
        NAVPROP_DVDV_SET_PARENTAL_CONTROL,   /* [in]  NAVPARENTALCONTROL                        */
        NAVPROP_DVDV_SET_PLAYER_REGION,      /* [in]  int (player region code 1~8)              */
        NAVPROP_DVDV_SET_LANG_PREFERENCE,    /* [in]  NAVLANGPREFERENCE                         */
        NAVPROP_DVDV_APPROVE_TMP_PARENTAL,   /* [in]  bool, non-zero to approve, zero to reject */

        /* special property get/set for DVD-VR nav input plugin */

        NAVPROP_VR_GET_RTR_VMGI,                 /* [out] RTR_VMGI pointer                            */
        NAVPROP_VR_DECODE_KEYFRAME,              /* [in]  decode the key frame                        */
        NAVPROP_VR_SET_BOOKMARK,                 /* [in]  set bookmark                                */
        NAVPROP_VR_RESTORE_BOOKMARK,             /* [out] restore bookmark                            */
        NAVPROP_VR_SET_PLAYDOMAIN,               /* [in]  set playback domain (0:program, 1:playlist) */
        NAVPROP_VR_DECODE_MULTIPLE_KEYFRAME,     /* [in]  decode multiple key frame                   */
        NAVPROP_VR_IS_NEW_REC_PG,                /* [out] get the pg state does ever played           */
        NAVPROP_VR_DISABLE_AUTO_RESET_NEW_TITLE, /* [in]  turn off auto reset new title flag          */
        NAVPROP_VR_CAN_SPEED,                    /* [out] get the playback can be fast forward or not */

        /* special property get/set for VCD nav input plugin */

        NAVPROP_VCD_GET_PBC_ON_OFF, /* [out] NAVVCDPBCINFO */
        NAVPROP_VCD_SET_PBC_ON_OFF, /* [in]  unsigned int, non-zero if PBC is on, zero if PBC is off */

        /* special property get/set for I-Frame only mode rate control */
        NAVPROP_ISCAN_WITH_RATE_AWARE, /* [in] int, for I scan mode, send with rate awareness (non-zero) or not(zero) */

        /* special property get/set for DivX Digital Right Management */

        NAVPROP_DIVX_DRM_QUERY,          /* [out] NAVDIVXDRMINFO                      */
        NAVPROP_DIVX_DRM_APPROVE_RENTAL, /* [in]  to use one of the reantal available */
        NAVPROP_DIVX_EDITIONNUM_QUERY,
        NAVPROP_DIVX_TITLENUM_QUERY,
        NAVPROP_DIVX_CHAPTERTYPE_QUERY,

        /* special property get/set for CDDA */

        NAVPROP_CDDA_DISCINFO, /* [out] NAVCDDAINFO output disc information for CDDA */

        /* special property get/set for DV nav input plugin */

        NAVPROP_DV_CTRL_CMD,                    /* [in] NAV_DV_ID */
        NAVPROP_DV_DISPLAY_CTRL,                /* [in] unsigned int DV Display control */

        /* special property get/set for DTV nav input plugin */

        NAVPROP_DTV_SET_TUNER_HANDLE,      /* [in]  unsigned char, the tuner handle */
        NAVPROP_DTV_GET_SI_ENGINE_HANDLE,  /* [out] unsigned int, the SI engine handle */ /* FIXME: OBSELETE */
        NAVPROP_DTV_SET_SI_ENGINE_HANDLE,  /* [in]  unsigned int, the SI engine handle */
        NAVPROP_DTV_SET_PREVIEW_MODE,       /* [in] NAV_PREVIEW_MODE */
        NAVPROP_DTV_SET_RECORD_MODE,       /* [in]  NAVRECORDMODE, recording mode */
        NAVPROP_DTV_GET_RECORD_STATUS,     /* [out] NAVRECORDSTATUS, recording status */
        NAVPROP_DTV_GET_EDITING_INTERFACE, /* [out] IFileEditing*, pointer to editing interface */
        NAVPROP_DTV_AB_FAST_COPY,           /* [in] NAVEDITINGPARA, editing parameters */
        NAVPROP_DTV_GET_SI_STATE,          /* [out] the SI state whose size would be set in outDataSize */
        NAVPROP_DTV_GET_AUDIO_FORMAT_FB,    /* [in] SI_CODEC_TYPE [out] privateData in SI_FILTER_DESC */

        NAVPROP_GET_IOPLUGIN_SUBTITLE_INFO,/*for hls subtitle*/
        NAVPROP_GET_IOPLUGIN_THUMBNAIL_INFO,/*for hls thumbnail*/

        /* general property get/set for every demux plugin */

        /*
        NAVPROP_DEMUX_xxxxxxxx,
        */

        /* general property get/set for nav filter */

        NAVPROP_NAV_SET_TIMEOUT_LIMITS,   /* [in] NAVTIMEOUTLIMITS */
        NAVPROP_NAV_SET_STARTUP_FULLNESS, /* [in] long, av sync start-up fullness requirement in bytes */

        /*Subtitle fonts Get for mkv internlal Subtitle*/
        NAVPROP_SUBTITLE_GET_FONTS,       /* [in] char *, for filename */

        /*Allow input plugin to set its favorable fullness timeout.
          It is internally used by navigator */
        NAVPROP_INPUT_GET_TIMEOUT_LIMITS,   /* [out] NAVTIMEOUTLIMITS */
        NAVPROP_INPUT_GET_WRMHEADER,    /* [out] WRMHEADER */
        NAVPROP_INPUT_GET_WRMHEADERSIZE,    /* [out] size of WRMHEADER */

#ifndef DISABLE_GETPLAYBACK_PTSINFO
        NAVPROP_NAV_GET_PLAYBACK_PTSINFO,   /* [out] NAVPLAYBACKPTSINFO */
#endif


        /* misc */

        NAVPROP_MISC_HACK_UOP = 12345678,

        //for flash
        NAVPROP_INPUT_FLASH_SET_BUFFER, /* [in] FLASH_BUF_NODE */
        NAVPROP_INPUT_FLASH_SET_CALLBACK_FUN,
        NAVPROP_INPUT_FLASH_FLUSH_BUFFER,
        NAVPROP_INPUT_FLASH_SET_DECODE_CMD, /* [in] NAVFLASHDECODECMD */
        NAVPROP_INPUT_FLASH_SET_STREAMPLAYER,   /*[in] void* */
        NAVPROP_INPUT_FLASH_SET_MEDIA_TYPE,     /*[in] ENUM_MEDIA_TYPE */

        //For NRD2.X
        NAVPROP_INPUT_NRD2_SET_HEADER,
    NAVPROP_INPUT_NRD2_SET_DRM_HEADER,
        NAVPROP_INPUT_NRD2_GET_CHALLENGE,
        NAVPROP_INPUT_NRD2_STORE_LICENSE,
        NAVPROP_INPUT_NRD2_CLEAR_LICENSE,

    //added by baili_feng for getting Content Description of ASF
    NAVPROP_INPUT_GET_CONTENT_DESCRIPTION,

    //for marlinBB drm
    NAVPROP_INPUT_GET_DESCRIPTOR_INFO,
    NAVPROP_INPUT_SET_DECRYPT_KEY,

        //For ACETREX PLAYBACK
        NAVPROP_INPUT_SET_ACETREX_DATA,

        //To enable/disable menu and config menu size. 
        NAVPROP_INPUT_SET_MENU_CONFIG,  /* [in] NAVMENUCONFIG*/

        //for skype to open audio or video stream
        NAVPROP_INPUT_OPEN_SUB_STREAM,  /* [in] path, [out] fd of this substream 
                                         * Note : Should use sync SetGetProperty
                                         */
        NAVPROP_INPUT_CLOSE_SUB_STREAM, /* [in] fd to be closed 
                                         * Note : Should use sync SetGetProperty
                                         */

        NAVPROP_INPUT_SET_STEREOSCOPIC_OUTPUT_MODE, /*[in] bool */
        NAVPROP_INPUT_GET_STEREOSCOPIC_OUTPUT_CAP, /*[out] bool */

        NAVPROP_NRD3_SET_DROP_COMMAND, /* [in] NAVUSERDROP */

        NAVPROP_NAV_EASE_REBUF_REQUIREMENT, /*[in] bool */

        NAVPROP_BD_GET_TITLE_LIST,  /* [out] NAVBDTITLELIST * numTitle (which could be from playbck status)*/

        NAVPROP_NAV_SET_THREAD_PRIORITY,    /* [int] navigation thread priority */
                NAVPROP_INPUT_SET_DEC_IGNORE_RATIO, /*widi calpella must ignore aspect ratio */
        NAVPROP_INPUT_SET_MEDIA_ATTRIBUTES,
        NAVPROP_INPUT_SET_HDCP2_INFO,

        NAVPROP_INPUT_FLUSH_AND_SET_VIDEO_LOW_DELAY_MODE, /* [in] NAVFLUSHSETMODE */

		NAVPROP_INPUT_GET_TS_PARSED_ON_LOAD, /* [out] bool, isTsParsedOnLoad */

		NAVPROP_INPUT_CHECK_MVC_IN_264, /* [out] bool, isMvcIn264 */

		NAVPROP_INPUT_GET_MVC_IN_264, /* [out] bool, isMvcIn264 */
                   NAVPROP_INPUT_SET_DISABLE_REORDER, /* [in] NAVREORDER */

        NAVPROP_INPUT_GET_MEDIA_TYPE,

        NAVPROP_INPUT_SET_STREAMING_IO_BUFFER_THRESHOLD,

        NAVPROP_INPUT_GET_TS_VIDEO_INFO,	/* [in] NAVGETTSVIDEOINFOPARAM, [out] NAVTSVIDEOINFO*/

		NAVPROP_INPUT_GET_TS_SEPARATE_AUDIO_READER, /* [out] bool, isSeparateAudioReader*/

		NAVPROP_DEMUX_GET_HAS_SET_AUDIO_TARGET, /* [out] bool, isAudioTargetSet */

		NAVPROP_INPUT_SET_CCTRANSPORT_ENABLE, /*[in] bool */

		NAVPROP_INPUT_GET_CHAPTER_INFO,

        NAVPROP_GET_FAST_PLAY,
	NAVPROP_SET_TS_PTS_WAIT,
        NAVPROP_SET_AUDIO_BYTERATES,
	NAVPROP_GET_AUDIO_STREAM,
	NAVPROP_GET_STREAM_BYTERATE,
        NAVPROP_INPUT_SET_DISABLE_CHECK_BROKEN_LINK,   /* [in] NAVCHECKBROKENLINK */
        NAVPROP_INPUT_SET_VIDEO_SCALED_SIZE,   /* [in] scaledWidth and scaledHeight int array */
        NAVPROP_SET_SEEKING_INFO,
        NAVPROP_INPUT_GET_TS_PARSED_ON_LOAD_DONE,
		NAVPROP_SET_GET_STEP_MODE,
		NAVPROP_GET_TS_PKT_SIZE,

		NAVPROP_INPUT_GET_BD_ISO_INFO,
		NAVPROP_INPUT_GET_BD_ISO_CHAPTER_INFO

    } NAV_PROP_ID;

#endif // _RTK_NAV_DEF_H_
