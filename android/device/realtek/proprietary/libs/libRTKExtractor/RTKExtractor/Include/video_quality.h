#ifndef __VIDEO_QUALITY_DEF_H__
#define __VIDEO_QUALITY_DEF_H__
typedef enum _rec_quality {
	RECORD_HQ,
	RECORD_SP,
	RECORD_LP,
	RECORD_EP,
	RECORD_SLP,
	RECORD_SEP,
	RECORD_SPP, // Verona-specific
	RECORD_QUALITY_MAX,
} ENUM_REC_QUALITY;

typedef enum _rec_format {
	RECORD_FORMAT_MPEG2,
	RECORD_FORMAT_MPEG4,
	RECORD_FORMAT_MAX,
} ENUM_REC_FORMAT;


/*********************************************/
/* HQ (High Quality): 1 hr mode              */
/*********************************************/
#define  HQ_RESOLUTION          VIDEO_D1
#define  HQ_RATEMODE            VIDEO_RATE_CVBR
#define  HQ_MPEG2_PEAKBITRATE   9000000
#define  HQ_MPEG4_PEAKBITRATE   5000000
#define  HQ_MPEG2_AVEBITRATE    8974000
#define  HQ_MPEG4_AVEBITRATE    1048576
#define	 HQ_BITBUFSIZE          (64*1024*1024*8)
#define  HQ_INITBUFFULLNESS     (32*1024*1024*8)
#define  HQ_CONFIGVOBU          1
#define  HQ_GOP_M               3
#define  HQ_GOP_N_NTSC          15
#define  HQ_GOP_N_PAL           12


#define  HQ_AUDIO_AC3_DATARATE      256000
#define  HQ_AUDIO_MP2_DATARATE      256000
#define  HQ_AUDIO_LPCM_DATARATE     1536000
#define  HQ_AUDIO_MP3_DATARATE      128000
#define  HQ_AUDIO_AAC_DATARATE      32000

#define  HQ_AUDIO_KARAOKE_SAMPLERATE -1
#define  HQ_AUDIO_AC3_SAMPLERATE    48000
#define  HQ_AUDIO_MP2_SAMPLERATE    48000
#define  HQ_AUDIO_LPCM_SAMPLERATE   48000
#define  HQ_AUDIO_MP3_SAMPLERATE    48000
#define  HQ_AUDIO_AAC_SAMPLERATE    48000



/*********************************************/
/* SP (Standard Play): 2 hr mode             */
/*********************************************/
#define  SP_RESOLUTION          VIDEO_D1
#define  SP_RATEMODE            VIDEO_RATE_CVBR
#define  SP_MPEG2_PEAKBITRATE   7000000
#define  SP_MPEG4_PEAKBITRATE   5000000
#define  SP_MPEG2_AVEBITRATE    4494000
#define  SP_MPEG4_AVEBITRATE    1048576
#define  SP_BITBUFSIZE          HQ_BITBUFSIZE 
#define  SP_INITBUFFULLNESS     HQ_INITBUFFULLNESS
#define  SP_CONFIGVOBU          1
#define  SP_GOP_M               3
#define  SP_GOP_N_NTSC          15
#define  SP_GOP_N_PAL           12

#define  SP_AUDIO_AC3_DATARATE      256000
#define  SP_AUDIO_MP2_DATARATE      256000
#define  SP_AUDIO_LPCM_DATARATE     1536000
#define  SP_AUDIO_MP3_DATARATE      128000
#define  SP_AUDIO_AAC_DATARATE      32000

#define  SP_AUDIO_KARAOKE_SAMPLERATE -1
#define  SP_AUDIO_AC3_SAMPLERATE    48000
#define  SP_AUDIO_MP2_SAMPLERATE    48000
#define  SP_AUDIO_LPCM_SAMPLERATE   48000
#define  SP_AUDIO_MP3_SAMPLERATE    48000
#define  SP_AUDIO_AAC_SAMPLERATE    48000



/*********************************************/
/* LP (Long Play): 3 hr mode                 */
/*********************************************/
#define  LP_RESOLUTION          VIDEO_D1
#define  LP_RATEMODE            VIDEO_RATE_CVBR
#define  LP_MPEG2_PEAKBITRATE   6000000
#define  LP_MPEG4_PEAKBITRATE   5000000
#define  LP_MPEG2_AVEBITRATE    2910000
#define  LP_MPEG4_AVEBITRATE    1048576
#define  LP_BITBUFSIZE          HQ_BITBUFSIZE
#define  LP_INITBUFFULLNESS     HQ_INITBUFFULLNESS
#define  LP_CONFIGVOBU          1
#define  LP_GOP_M               3
#define  LP_GOP_N_NTSC          15
#define  LP_GOP_N_PAL           12

#define  LP_AUDIO_AC3_DATARATE      256000
#define  LP_AUDIO_MP2_DATARATE      256000
#define  LP_AUDIO_LPCM_DATARATE     1536000
#define  LP_AUDIO_MP3_DATARATE      128000
#define  LP_AUDIO_AAC_DATARATE      32000

#define  LP_AUDIO_KARAOKE_SAMPLERATE -1
#define  LP_AUDIO_AC3_SAMPLERATE    48000
#define  LP_AUDIO_MP2_SAMPLERATE    48000
#define  LP_AUDIO_LPCM_SAMPLERATE   48000
#define  LP_AUDIO_MP3_SAMPLERATE    48000
#define  LP_AUDIO_AAC_SAMPLERATE    48000



/*********************************************/
/* EP (Extended Play): 4 hr mode             */
/*********************************************/
#define  EP_RESOLUTION          VIDEO_HALF_D1
#define  EP_RATEMODE            VIDEO_RATE_CVBR
#define  EP_MPEG2_PEAKBITRATE   5000000
#define  EP_MPEG4_PEAKBITRATE   5000000
#define  EP_MPEG2_AVEBITRATE    2109000
#define  EP_MPEG4_AVEBITRATE    1048576
#define  EP_BITBUFSIZE          HQ_BITBUFSIZE
#define  EP_INITBUFFULLNESS     HQ_INITBUFFULLNESS
#define  EP_CONFIGVOBU          1
#define  EP_GOP_M               3
#define  EP_GOP_N_NTSC          15
#define  EP_GOP_N_PAL           12

#define  EP_AUDIO_AC3_DATARATE      256000
#define  EP_AUDIO_MP2_DATARATE      256000
#define  EP_AUDIO_LPCM_DATARATE     1536000
#define  EP_AUDIO_MP3_DATARATE      128000
#define  EP_AUDIO_AAC_DATARATE      32000

#define  EP_AUDIO_KARAOKE_SAMPLERATE -1
#define  EP_AUDIO_AC3_SAMPLERATE    48000
#define  EP_AUDIO_MP2_SAMPLERATE    48000
#define  EP_AUDIO_LPCM_SAMPLERATE   48000
#define  EP_AUDIO_MP3_SAMPLERATE    48000
#define  EP_AUDIO_AAC_SAMPLERATE    48000



/*********************************************/
/* SLP (Super Long Play): 6 hr mode          */
/*********************************************/
#define  SLP_RESOLUTION          VIDEO_HALF_D1
#define  SLP_RATEMODE            VIDEO_RATE_CVBR
#define  SLP_MPEG2_PEAKBITRATE   5000000
#define  SLP_MPEG4_PEAKBITRATE   5000000
#define  SLP_MPEG2_AVEBITRATE    1431000
#define  SLP_MPEG4_AVEBITRATE    1431000
#define  SLP_BITBUFSIZE          HQ_BITBUFSIZE
#define  SLP_INITBUFFULLNESS     HQ_INITBUFFULLNESS
#define  SLP_CONFIGVOBU          2
#define  SLP_GOP_M               3
#define  SLP_GOP_N_NTSC          12
#define  SLP_GOP_N_PAL           12


#define  SLP_AUDIO_AC3_DATARATE      192000
#define  SLP_AUDIO_MP2_DATARATE      192000
#define  SLP_AUDIO_LPCM_DATARATE     1536000
#define  SLP_AUDIO_MP3_DATARATE      128000
#define  SLP_AUDIO_AAC_DATARATE      32000

#define  SLP_AUDIO_KARAOKE_SAMPLERATE -1
#define  SLP_AUDIO_AC3_SAMPLERATE    48000
#define  SLP_AUDIO_MP2_SAMPLERATE    48000
#define  SLP_AUDIO_LPCM_SAMPLERATE   48000
#define  SLP_AUDIO_MP3_SAMPLERATE    48000
#define  SLP_AUDIO_AAC_SAMPLERATE    48000


/*********************************************/
/* SEP (Super Extended Play): 8 hr mode      */
/*********************************************/
#define  SEP_RESOLUTION          VIDEO_SIF
#define  SEP_RATEMODE            VIDEO_RATE_CVBR
#define  SEP_MPEG2_PEAKBITRATE   5000000
#define  SEP_MPEG4_PEAKBITRATE   5000000
#define  SEP_MPEG2_AVEBITRATE    990600
#define  SEP_MPEG4_AVEBITRATE    990600
#define  SEP_BITBUFSIZE          HQ_BITBUFSIZE
#define  SEP_INITBUFFULLNESS     HQ_INITBUFFULLNESS
#define  SEP_CONFIGVOBU          2
#define  SEP_GOP_M               3
#define  SEP_GOP_N_NTSC          12
#define  SEP_GOP_N_PAL           12


#define  SEP_AUDIO_AC3_DATARATE      192000
#define  SEP_AUDIO_MP2_DATARATE      192000
#define  SEP_AUDIO_LPCM_DATARATE     1536000
#define  SEP_AUDIO_MP3_DATARATE      128000
#define  SEP_AUDIO_AAC_DATARATE      32000

#define  SEP_AUDIO_KARAOKE_SAMPLERATE -1
#define  SEP_AUDIO_AC3_SAMPLERATE    48000
#define  SEP_AUDIO_MP2_SAMPLERATE    48000
#define  SEP_AUDIO_LPCM_SAMPLERATE   48000
#define  SEP_AUDIO_MP3_SAMPLERATE    48000
#define  SEP_AUDIO_AAC_SAMPLERATE    48000



#define LIVEPAUSE_BITBUFSIZE		HQ_BITBUFSIZE
#define LIVEPAUSE_INITBUFFULLNESS	HQ_INITBUFFULLNESS


#endif // __VIDEO_QUALITY_DEF_H__




