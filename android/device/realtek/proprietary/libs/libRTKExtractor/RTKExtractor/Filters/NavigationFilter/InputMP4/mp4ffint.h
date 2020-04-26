#ifndef MP4FF_INTERNAL_H
#define MP4FF_INTERNAL_H
#include <sys/time.h>
#include <string.h>
#include <StreamClass/EType.h>
#include "Platform_Lib/MCPControl/MCPUtil.h"
//#define MP4_SEND_CONTEXT
//#define MP4_SEND_PTS
//#define MP4_SUPPORT_AUDIO_RAW

//#define MAX_TRACKS 1024
//#define MAX_TRACKS 	30 // neco+
#define MAX_TRACKS 	12 // neco+ // Avoid mp4 detect over 12 subtitle caused mediaserver crashed. The reason is inputMP4 memory overflow.
#define MAX_IPMPS	5

#define SUBATOMIC		128

/* atoms without subatoms */
#define ATOM_FTYP		129
#define ATOM_MDAT		130
#define ATOM_MVHD		131
#define ATOM_TKHD		132
#define ATOM_TREF		133
#define ATOM_MDHD		134
#define ATOM_VMHD		135
#define ATOM_SMHD		136
#define ATOM_HMHD		137
#define ATOM_STSD		138
#define ATOM_STTS		139
#define ATOM_STSZ		140
#define ATOM_STZ2		141
#define ATOM_STCO		142
#define ATOM_STSC		143
#define ATOM_MP4A		144
#define ATOM_MP4V		145
#define ATOM_MP4S		146
#define ATOM_ESDS		147
#define ATOM_META		148 /* iTunes Metadata box */
#define ATOM_NAME		149 /* iTunes Metadata name box */
#define ATOM_DATA		150 /* iTunes Metadata data box */
#define ATOM_CTTS		151
#define ATOM_FRMA		152
#define ATOM_IVIV		153
#define ATOM_PRIV		154
//#define ATOM_UDTA		155 // skip this atom	// Ivan+
#define ATOM_HDLR		156 // neco
#define ATOM_IODS		157 // neco
#define ATOM_NMHD		158 // neco
#define ATOM_JPEG		159 // neco
#define ATOM_DINF		160 // neco
#define ATOM_PCM		161 // neco, big-endian pcm
#define ATOM_MPEG		162 // neco
#define ATOM_MJPG		163 // neco
#define ATOM_ULAW   164 // yllin
#define ATOM_STSS   165 // yllin
#define ATOM_MP3    166 // yllin
#define ATOM_AC3    167 // yllin
#define ATOM_ALAW   168 // yllin
#define ATOM_PCML   169 // yllin, little-endian pcm
#define ATOM_MS_ADPCM  170 // yllin, MS ADPCM
#define ATOM_CO64   171 // yllin, 64 bits stco table
#define ATOM_DIV3   172 // yllin, divx3
#define ATOM_DCOM		173
#define ATOM_CMVD		174
#if IS_CHIP(MARS)||IS_CHIP(JUPITER)||IS_CHIP(SATURN) || defined(IS_TV_CHIP)
#define ATOM_AVC1   175 // RAY +
#endif
#define ATOM_H263          176
#define ATOM_ELST          177
#define ATOM_CHPL          178
#define ATOM_AVS2		179
#define ATOM_EAC3		180
#define ATOM_SAMR		181
#define ATOM_DAMR		182
#define ATOM_SAWB		183
#define ATOM_IMA_ADPCM		184
#define ATOM_GLBL		185
#define ATOM_MEHD		186
#define ATOM_MOOF		187
#define ATOM_ALAC       188 // ken
#define ATOM_DTSC		190
#define ATOM_HVC1		191
#define ATOM_HVCC		192
#define ATOM_HEV1		193
#define ATOM_HEVC		194
#define ATOM_LOCATION   195

#define ATOM_UNKNOWN	255
#define ATOM_FREE ATOM_UNKNOWN
#define ATOM_SKIP ATOM_UNKNOWN
#define ATOM_GENRE2		ATOM_UNKNOWN

/* atoms with subatoms */
#define ATOM_MOOV		1
#define ATOM_TRAK		2
#define ATOM_EDTS		3
#define ATOM_MDIA		4
#define ATOM_MINF		5
#define ATOM_STBL		6
//#define ATOM_UDTA		7 					// Ivan-
#define ATOM_ILST		8 /* iTunes Metadata list */
#define ATOM_TITLE		9
#define ATOM_ARTIST		10
#define ATOM_WRITER		11
#define ATOM_ALBUM		12
#define ATOM_DATE		13
#define ATOM_TOOL		14
#define ATOM_COMMENT	15
#define ATOM_GENRE1		16
#define ATOM_TRACK		17
#define ATOM_DISC		18
#define ATOM_COMPILATION 19
//#define ATOM_GENRE2		20
#define ATOM_TEMPO		21
#define ATOM_COVER		22
#define ATOM_DRMS		23
#define ATOM_SINF		24
#define ATOM_SCHI		25
#define ATOM_CMOV		26
#define ATOM_UDTA		27
#define ATOM_SCHM		28
#define ATOM_MVEX		29
//#define ATOM_DINF		26 // neco
//#define ATOM_DREF		27 // neco

/* SBR signalling */
#define NO_SBR				0
#define SBR_UPSAMPLED		1
#define SBR_DOWNSAMPLED		2
#define NO_SBR_UPSAMPLED	3

//=== Ivan+ =====================================================

/* one byte tag identifiers */
#define Tag_ODescr				0x01
#define Tag_IODescr				0x02
#define Tag_ESDescr				0x03
#define Tag_DecConfigDescr		0x04
#define Tag_DecSpecificDescr		0x05
#define Tag_SLConfigDescr			0x06

#define	STSD_READ			(1L << 0)
#define	STSZ_READ			(1L << 1)
#define	STCO_READ			(1L << 2)
#define	STSC_READ			(1L << 3)
#define STTS_READ			(1L << 4)
#define	TRACK_READ			(0x1F)

#define OTI_MP3			0x6B
#define OTI_ML2P3			0x69
//===============================================================

#if 1

//========= Ivan+ ========================================================
//stsd :: Sample Description
typedef struct {
	unsigned short Width;
	unsigned short Height;
	unsigned long DataSize;
	unsigned short FrameCount;
} VideoSampleDescription_t;

typedef struct {
	unsigned short NumberOfChannels;
	unsigned short SampleSize; // not used
	unsigned long SampleRate;
	unsigned short bitsPerSample; // neco+
} AudioSampleDescription_t;

typedef struct ALACSpecificConfig // Ken+
{
	uint32_t    frameLength;        // uint32_t
	uint8_t     compatibleVersion;  // uint8_t
	uint8_t     bitDepth;           // uint8_t
	uint8_t     pb;                 // uint8_t
	uint8_t     mb;                 // uint8_t
	uint8_t     kb;                 // uint8_t
	uint8_t     numChannels;        // uint8_t
	uint16_t    maxRun;             // uint16_t
	uint32_t    maxFrameBytes;      // uint32_t
	uint32_t    avgBitRate;         // uint32_t
	uint32_t    sampleRate;         // uint32_t
} ALACSpecificConfig;

typedef struct _video_info_data{
	int				track_idx;

	unsigned short	FrameRate;
	unsigned long	Timescale;
	unsigned long	Duration;

	unsigned char is_send_video_des;// yllin+
	unsigned char *description;//yllin+
	int           description_sz;// yllin+

	VideoSampleDescription_t SampleDescription;
#if IS_CHIP(MARS)||IS_CHIP(JUPITER)||IS_CHIP(SATURN) || defined(IS_TV_CHIP)
	//======for avc=====
	unsigned int config_version;
	unsigned int profile;
	unsigned int level;
        unsigned long lengthSizeMinusOne;//RAY +
        unsigned int numOfSequenceParameterSets;//RAY +
        unsigned int numOfPictureParameterSets;//RAY +
        unsigned int* psequenceParameterSetLength;//RAY +
        unsigned int* ppictureParameterSetLength;//RAY +
        unsigned char** sps;//RAY +
        unsigned char** pps;//RAY +
#endif
} video_info_data_t;

//========================================================================

typedef struct _audio_info_data{
	int				track_idx;	// Ivan+
	//Info from esds
	char			object_type;
	char			sbr_present_flag;
	unsigned char	sampling_freq_index;
	unsigned char	nch;
	char      ver1_header_present_flag;
	int       samples_per_packet;
	int       bytes_per_frame;
  int64_t   ver1_header_addr;
  char      downSampledSBR;
  char      forceUpSampling;
	//Info from stsd
	AudioSampleDescription_t SampleDescription; // Ivan+
	ALACSpecificConfig ALACSpecificConfig; // Ken+

} audio_info_data_t;

//========= Ivan+ ========================================================

typedef struct {
	unsigned long	FirstChunk;
	unsigned long	samplesPerChunk;
	unsigned long	sampleDescriptionIndex;
} SampleToChunkTable_t;

typedef struct { // stsc
	unsigned long	count;
	unsigned long	cur_index;
	int64_t	      f_addr;
	SampleToChunkTable_t	*table;
	unsigned long* ssntable;//startsamplenum
} mp4ff_stsc_t;

typedef struct {
	unsigned int StartSampleNo; // start sample# of each duration.
    unsigned int count;
    unsigned int duration;
    unsigned long long StartSampledur;
} mov_durmap_t;

typedef struct { // stts
	unsigned long	count;
	unsigned long	cur_index;
	int64_t	      f_addr;
	mov_durmap_t	*table;
} mp4ff_stts_t;

typedef struct { // stsz
	unsigned long	sample_size;
	unsigned long	sample_count;
	unsigned long	cur_index;
	int64_t	      f_addr;
	unsigned long	*table;
} mp4ff_stsz_t;

typedef struct { // stco
	unsigned long	entry_count;
	unsigned long	cur_index;
	int64_t	      f_addr;

	unsigned char is_64bits_table;
	unsigned long	*chunk_offset;
	int64_t     	*chunk_offset64;
} mp4ff_stco_t;

typedef struct { // mdhd
	unsigned long	time_scale;
	uint64_t		duration;
	char lan[3];
} mp4ff_mdhd_t;

typedef struct {
	int				    codec_id;	// esds
	int				    decoderConfigLen; //esde
	unsigned char *decoderConfig;

	unsigned char checkVorbis;
	int esds_raw_len;	//esds raw data length
	unsigned char *esds_raw_buffer;	//esda raw data
} mp4ff_esds_t;

typedef struct { // stss
	unsigned long	count;
	int64_t	      f_addr;
	unsigned long	*table;
} mp4ff_stss_t;

typedef struct {
	int64_t	      Pos;
	unsigned long	Size;
	unsigned long	SamplesPerChunk;
} Chunk_index_table_t;

typedef struct {
	int64_t	      Pos;
	unsigned long	Size;
	int64_t       durPts;
} Sample_index_table_t;

/* mp4 track edits */
typedef struct
{
    unsigned long segment_duration;
    unsigned long media_time;
    unsigned long media_rate;
}mp4edit_t;
//========================================================================
typedef struct
{
    uint64_t start_time;
    char* name;
}chpl_Entry;

typedef struct
{
        unsigned long entryCount;
        unsigned long curEntryNumber;
        chpl_Entry *chplEntries;
}chpl_List;

typedef struct
{
        bool bStorefcc;
        unsigned long storedfcc;
        unsigned long audiofcc;
        unsigned long videofcc;
}fcccheck_t;

typedef struct { // fragment
	unsigned long	sample_duration;
	unsigned long	sample_size;
	unsigned long sample_flags;
	unsigned long sample_composition_time_offset;
} mp4ff_fragmented_sample_t;

typedef struct mp4ff_trun_t{ // fragment
	unsigned long	sample_count;
	unsigned long num_stts;//assistant field to expand track metadata
	int constant_sample_size;
	int constant_sample_dur;
	int key_frame_head;
	int64_t data_offset;
	mp4ff_fragmented_sample_t* p_fragmentSamples;
	struct mp4ff_trun_t* p_nexttrun;
} mp4ff_trun_t;

typedef struct mp4ff_traf_t{ // fragment
	unsigned long track_ID;
	unsigned long num_stsc;//assistant field to expand track metadata
	unsigned long num_truns;
	mp4ff_trun_t* p_truns;
	struct mp4ff_traf_t* p_nexttraf;
} mp4ff_traf_t;

typedef struct mp4ff_moof_t{ // fragment
	int64_t moof_addr;
	mp4ff_traf_t* p_trafs;
	struct mp4ff_moof_t* p_nextMoof;
} mp4ff_moof_t;

typedef struct
{
    unsigned long schm;
    unsigned char* data;
}mp4ff_sinf_t;

typedef struct
{
    int ipmpdec_id;
    int track_idx;
    mp4ff_sinf_t sinf[2];
}mp4ff_ipmp_t;

typedef enum{
    TRACK_UNKNOWN,
    TRACK_AUDIO	,
    TRACK_VIDEO,
    TRACK_SYSTEM,
    TRACK_SUBPIC,
    TRACK_SUBTXT,
    TRACK_ODSM
}ENUM_TRACK_TYPE;

typedef struct
{
	int				complete; // Ivan+
	int				channelCount;
	int				sampleSize;
	unsigned short	sampleRate;
	int				audioType;



	// stsd
	int				stsd_entry_count;

	mp4ff_stsc_t	stsc;	// Ivan+
	mp4ff_mdhd_t	mdhd;	// Ivan+
	mp4ff_stts_t	stts;	// neco+
	mp4ff_stsz_t	stsz;	// Ivan*
	mp4ff_stco_t	stco;	// Ivan*
	mp4ff_esds_t	esds;	// Ivan*
  mp4ff_stss_t	stss; // yllin+


//========= Ivan+ ========================================================

	//playback info start, create index before playback
	int				SampleConstSize;
	unsigned long	NumChunks;
	unsigned long	NumSamples;
	unsigned long	CurChunkIndex;
	unsigned long	CurSampleIndex;
	unsigned long	ChunkDataLeft;
	unsigned long	DataLeft;
	// the len of the remaining payload not sent in the previous run
	// for video: sample data left
	// for audio: chunk data left
	int64_t	      CurChunkOffset;
	//unsigned long	CurSampleOffset;
	ENUM_MEDIA_TYPE mediatype; // neco+
	ENUM_TRACK_TYPE type;
	//playback info end
//========================================================================
  int         ConstFrmRate;
  audio_info_data_t privateAudioInfo;
  video_info_data_t privateVideoInfo;
  unsigned long m_TrakEditCount;
  mp4edit_t* m_pTrakEdits;
  int m_TrakStartIdx;
  int width;
  int height;
  int palette[16];
  unsigned long track_ID;
  unsigned int rotationDegrees;
//======================
  bool bIsSendingheader;
  int NaluLeft;//for 264 nalu
  bool bNeedSendHeader;
  int64_t tracksize;
  int LenByteLeft;
  int DataInBuffer;
} mp4ff_track_t;

/* mp4 main file structure */
typedef struct
{
	/* stream to read from */
	int						moov_read;
	unsigned long long int	moov_size;
	unsigned long long int	file_size;

	// mvhd
	unsigned long			time_scale;
	unsigned long long		duration;
    /* According frameworks/av/media/libstagefright/MPEG4Extractor.cpp, '0A9','x','y','z' */
    #define MAX_LOCATIONBUFFER_LENGTH   18
    unsigned char           locationBuffer[MAX_LOCATIONBUFFER_LENGTH];
    unsigned int            mp4Fps;


	// incremental track index while reading the file
	int						total_tracks;
	int						audio_track_count;	// Ivan+
	int						video_track_count;	// Ivan+

	// track data
	mp4ff_track_t			track[MAX_TRACKS]; // neco*

//====== yllin ADD ==========================
	//int						stsd_read;
	//int						stsz_read;
	//int						stco_read;
	audio_info_data_t		audioInfo;
	int						send_stsz_size;
//===========================================
	video_info_data_t		videoInfo;			// Ivan+
	chpl_List trak_chpllist;
	int curstssidx;
	int64_t currRefClock;
	bool bneedSeek;
	fcccheck_t fcc_check;
	int End_sending;
	int m_scanFrameIdx;
	int m_stsssize;
	int MP4_AVSYNCPTS;
	int mp4_resumepoint;
	int setfrmidx;
	int toseekset;
	int profileall;
	int64_t totalIOtime;
	int64_t totalDemuxtime;
	int64_t totalSent;
	struct timeval lint;
	int framesendcounter;
	int IsBesTV;
//===============fragments===============
	int mvex_read;
	unsigned long MoofCount;
	mp4ff_moof_t* m_pmoof;
	mp4ff_ipmp_t m_ipmp[MAX_IPMPS];
	unsigned char m_IV[16];
	unsigned char m_aKey[16];
	unsigned char m_vKey[16];
	unsigned char m_sKey[16];
      MCP_CIPHER_CTX  m_CipherContext;
      int b_DoNotInitMCP;
#ifdef ENABLE_FILELIST_PLAYBACK
	long long m_posbase;
#endif
} mp4ff_t;
#endif
#define DEBUG
#ifdef DEBUG
#define DBGP(fmt,args...)  printf(fmt,## args)
#else
#define DBGP(fmt,args...) 
#endif
#endif

