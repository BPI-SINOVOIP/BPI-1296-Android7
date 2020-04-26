#ifndef MP4FF_INTERNAL_H
#define MP4FF_INTERNAL_H

#include <StreamClass/EType.h>

//#define MP4_SEND_CONTEXT
//#define MP4_SEND_PTS
//#define MP4_SUPPORT_AUDIO_RAW

//#define MAX_TRACKS 1024
#define MAX_TRACKS 12 // neco+

#define TRACK_UNKNOWN	0
#define TRACK_AUDIO		1
#define TRACK_VIDEO		2
#define TRACK_SYSTEM	3
#define TRACK_SUBPIC        4
#define TRACK_SUBTXT        5
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
#if IS_CHIP(MARS)||IS_CHIP(JUPITER)||IS_CHIP(SATURN)
#define ATOM_AVC1   175 // RAY +
#endif
#define ATOM_H263          176
#define ATOM_ELST          177
#define ATOM_CHPL          178
#define ATOM_UNKNOWN	255
#define ATOM_FREE ATOM_UNKNOWN
#define ATOM_SKIP ATOM_UNKNOWN

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
#define ATOM_GENRE2		20
#define ATOM_TEMPO		21
#define ATOM_COVER		22
#define ATOM_DRMS		23
#define ATOM_SINF		24
#define ATOM_SCHI		25
#define ATOM_CMOV		26
#define ATOM_UDTA		27
//#define ATOM_DINF		26 // neco
//#define ATOM_DREF		27 // neco

/* SBR signalling */
#define NO_SBR				0
#define SBR_UPSAMPLED		1
#define SBR_DOWNSAMPLED		2
#define NO_SBR_UPSAMPLED	3

//=== Ivan+ =====================================================

/* one byte tag identifiers */
#define MP4ODescrTag				0x01 
#define MP4IODescrTag				0x02 
#define MP4ESDescrTag				0x03 
#define MP4DecConfigDescrTag		0x04 
#define MP4DecSpecificDescrTag		0x05 
#define MP4SLConfigDescrTag			0x06 
#define MP4ContentIdDescrTag		0x07 
#define MP4SupplContentIdDescrTag	0x08 
#define MP4IPIPtrDescrTag			0x09 
#define MP4IPMPPtrDescrTag			0x0A 
#define MP4IPMPDescrTag				0x0B 
#define MP4RegistrationDescrTag		0x0D 
#define MP4ESIDIncDescrTag			0x0E 
#define MP4ESIDRefDescrTag			0x0F 
#define MP4FileIODescrTag			0x10 
#define MP4FileODescrTag			0x11 
#define MP4ExtProfileLevelDescrTag	0x13 
#define MP4ExtDescrTagsStart		0x80 
#define MP4ExtDescrTagsEnd			0xFE 

#define	STSD_READ			(1L << 0)
#define	STSZ_READ			(1L << 1)
#define	STCO_READ			(1L << 2)
#define	STSC_READ			(1L << 3)
#define STTS_READ			(1L << 4)
#define	TRACK_READ			(0x1F)

/* object type identifiers in the ESDS */
/* See http://gpac.sourceforge.net/tutorial/mediatypes.htm */
/* BIFS stream version 1 */
#define MP4OTI_MPEG4Systems1                                0x01
/* BIFS stream version 2 */
#define MP4OTI_MPEG4Systems2                                0x02
/* MPEG-4 visual stream */
#define MP4OTI_MPEG4Visual                                  0x20
/* MPEG-4 audio stream */
#define MP4OTI_MPEG4Audio                                   0x40
/* MPEG-2 visual streams with various profiles */
#define MP4OTI_MPEG2VisualSimple                            0x60
#define MP4OTI_MPEG2VisualMain                              0x61
#define MP4OTI_MPEG2VisualSNR                               0x62
#define MP4OTI_MPEG2VisualSpatial                           0x63
#define MP4OTI_MPEG2VisualHigh                              0x64
#define MP4OTI_MPEG2Visual422                               0x65
/* MPEG-2 audio stream part 7 ("AAC") with various profiles */
#define MP4OTI_MPEG2AudioMain                               0x66
#define MP4OTI_MPEG2AudioLowComplexity                      0x67
#define MP4OTI_MPEG2AudioScaleableSamplingRate              0x68
/* MPEG-2 audio part 3 ("MP3") */
#define MP4OTI_MPEG2AudioPart3                              0x69
/* MPEG-1 visual visual stream */
#define MP4OTI_MPEG1Visual                                  0x6A
/* MPEG-1 audio stream part 3 ("MP3") */
#define MP4OTI_MPEG1Audio                                   0x6B
/* JPEG visual stream */
#define MP4OTI_JPEG                                         0x6C
/* 3GPP2 */
#define MP4OTI_13kVoice                                     0xE1

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

          
typedef struct _video_info_data{
	int				track_idx;
    
	unsigned short	FrameRate;
	unsigned long	Timescale;
	unsigned long	Duration;
   
	unsigned char is_send_video_des;// yllin+
	unsigned char *description;//yllin+
	int           description_sz;// yllin+
	
	VideoSampleDescription_t SampleDescription;
#if IS_CHIP(MARS)||IS_CHIP(JUPITER)||IS_CHIP(SATURN)
	//======for avc=====
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
	#ifdef BIG_FILE
	unsigned long* ssntable;//startsamplenum
	#endif
} mp4ff_stsc_t;

typedef struct {
	unsigned int StartSampleNo; // start sample# of each duration.
    unsigned int count;
    unsigned int duration;
    #ifdef BIG_FILE
    unsigned int StartSampledur;
    #endif
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
	unsigned long	duration;
	char lan[3];
} mp4ff_mdhd_t;

typedef struct {
	int				    codec_id;	// esds
	int				    decoderConfigLen; //esde
	unsigned char *decoderConfig;
	
	unsigned char checkVorbis;
	unsigned char *vorbis_extra_data;
	int           vorbis_data_len;
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
	int				type;
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
	Chunk_index_table_t		*ChunkIndex;
	Sample_index_table_t	*SampleIndex;

	unsigned long	TotalTime; // sec
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
	bool			bIsNewChunk; // neco+
	ENUM_MEDIA_TYPE mediatype; // neco+
	//playback info end
//========================================================================
  int64_t     TotalSamples;// yllin+
  int         ConstFrmRate;
  audio_info_data_t privateAudioInfo;
  unsigned long m_TrakEditCount;
  mp4edit_t* m_pTrakEdits;
  int m_TrakStartIdx;
  int width;
  int height;
  int palette[16];
} mp4ff_track_t;

/* mp4 main file structure */
typedef struct
{
	/* stream to read from */
	long long int			current_position;

	int						moov_read;
	unsigned long long int	moov_offset;
	unsigned long long int	moov_size;
	unsigned char			last_atom;
	unsigned long long int	file_size;

	// mvhd 
	unsigned long			time_scale;
	unsigned long			duration;

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
	int specalcase_for_bwdlastpart;
	int64_t currRefClock;
} mp4ff_t;
#endif
//#define DEBUG
#ifdef DEBUG
#define DBGP(fmt,args...)  printf(fmt,## args)
#else
#define DBGP(fmt,args...) 
#endif
#endif

