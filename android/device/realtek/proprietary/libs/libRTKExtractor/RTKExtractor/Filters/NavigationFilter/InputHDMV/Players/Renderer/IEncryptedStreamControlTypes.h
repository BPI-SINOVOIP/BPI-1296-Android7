//----------------------------------------------------------------------------- 
// IEncryptedStreamControlTypes.h
// Copyright (c) 2002-2007, Sonic Solutions. All rights reserved.
//
// These are the types needed to use the IEncryptedStreamControl interface.
//----------------------------------------------------------------------------- 

#ifndef _IEncryptedStreamControlTypes_h_
#define _IEncryptedStreamControlTypes_h_



// These values are reserved for output ids of ConfigureOutput
#define PRIMARY_VIDEO_OUTPUT_ID     0
#define PRIMARY_AUDIO_OUTPUT_ID     1
#define SECONDARY_VIDEO_OUTPUT_ID   2
#define SECONDARY_AUDIO_OUTPUT_ID   3
#define SPU_OUTPUT_ID               4
#define INTERACTIVE_AUDIO_FIRST_OUTPUT_ID 5
#define INTERACTIVE_AUDIO_LAST_OUTPUT_ID 12

#if 0

// Input types used in AddStream
typedef enum tagINPUT_STREAM_TYPE
{
    eInputTypeMP2PS,        // An MPEG-2 program stream (not ripped from a DVD)
    eInputTypeMP2TS,        // An MPEG-2 transport stream
    eInputTypeDVD,          // A VOB from a DVD (where private_stream_2 contains nav packets etc)
    eInputTypeHDDVDC1,      // An EVOB from a HD DVD Content 1
    eInputTypeHDDVDC2,      // An EVOB from a HD DVD Content 2
    eInputTypeHDDVDC3,      // An EVOB from a HD DVD Content 3
    eInputTypeBDAV,         // A Bluray transport stream
    eInputTypeMP1SS,        // An MPEG-1 system stream
    eInputTypeVCD,          // A VCD track from a VideoCD
    eInputTypeES,           // An elementary stream of any type
    eInputTypeRawDV,        // Raw DV stream (like the ones found on BDRE)
    eInputTypeMP4SS         // MPEG-4 system stream
    
} INPUT_STREAM_TYPE;

// Output stream types and descriptions used in ConfigureOutput and RouteOutput
typedef enum tagOUTPUT_STREAM_TYPE
{
    eOutputTypeAudio,
    eOutputTypeVideo,
    
} OUTPUT_STREAM_TYPE;

typedef enum tagOUTPUT_STREAM_SUBTYPE
{
    eOutputAudioMPEG1,
    eOutputAudioMPEG2,
    eOutputAudioAC3,
    eOutputAudioLPCM,
    eOutputAudioLPCM_HDC1,
    eOutputAudioDTS,
    eOutputAudioDTS_HD,
    eOutputAudioDDPlus,
    eOutputAudioAAC,
    eOutputAudioSDDS,
    eOutputAudioMLP,
    eOutputVideoMPEG1,
    eOutputVideoMPEG2,
    eOutputVideoVC1,
    eOutputVideoAVC,
    eOutputVideoSPU,
    eOutputVideoHDC1_SPU,
    eOutputVideoInteractiveGraphics,
    eOutputVideoPresentationGraphics,
    eOutputAudioLPCM_BD,
    eOutputVideoDV,
    eOutputAudioDV_LPCM,
    eOutputAudioMP3,
    eOutputTextSubtitle,
    eOutputAudioDTS_HD_LBR
} OUTPUT_STREAM_SUBTYPE;


typedef struct tagOUTPUT_STREAM_DESC
{
    OUTPUT_STREAM_TYPE      type;
    OUTPUT_STREAM_SUBTYPE   subtype;
    
    WORD                    id;
    WORD                    subid;
    
    // audio properties (only used if type is eOutputTypeAudio)
    UINT32                  channels;
    UINT32                  frequency;
    UINT32                  bitsPerSample;
    UINT32                  chanConfig;

    // video properties (only used if type is eOutputTypeVideo)
    UINT32                  width;
    UINT32                  height;
    UINT32                  ax;
    UINT32                  ay;
    __int64                 timePerFrame;
    
} OUTPUT_STREAM_DESC;

#endif
// metadata types
typedef enum _tagEMD_FORMAT
{
    EMD_DATA_DISCONTINUITY, // value (not used)
                            //
                            // Use this to signal a break in the input stream
                            // data (like from a seek or transition to a new 
                            // stream).
    
    EMD_TIME_DISCONTINUITY, // value = SInt64 with the offset in 10 nano-secs units
                            //
                            // Use this to map the native pts's in the stream
                            // to the current presentation time.
                            // This is for non-seamless transitions in the 
                            // presentation like seeking, navigating, etc.
                            // The meta data value is an offset used to add an 
                            // initial offset to the first a/v sample 
                            // (use 0 for no offset).
                        
    EMD_TIME_DELTA,         // value = SInt64 of an amout of time (in 10 
                            // nano-secs units)to add to the specified output 
                            // stream's pts's.
                            //
                            // Use this for seamless tranistions between a/v
                            // stream segments (like a looping menu or tile
                            // clips) with know presentation durations.  If
                            // an a/v segment is 5 seconds, set a 5 second
                            // time delta on the first sample of the segment
                            // following the 5 second segment.  These values
                            // are cummulative so you do not need to keep
                            // track of them.  The cummulative deltas are
                            // reset when processing EMD_TIME_DISCONTINUITY 
                            // meta datas.

    EMD_RATE_CHANGE,        // value = double of the new rate (neg means reverse play)
                            //
                            // Use this to change the playback rate of an input 
                            // stream (1.0 is the default rate).  The user must
                            // properly format the data for the particular rate
                            // like I frame only FF/RW.  I frame only modes
                            // must have data discontinuity and still frame 
                            // flags on the first sample of each I frame.
    
    EMD_END_OF_MENU,        // value (not used)
                            //
                            // Use this after sending all the data associated
                            // with a menu.  Decoders can use this info to 
                            // properly handle still frames encoded without
                            // a sequence end code.
    
    EMD_END_OF_FILE,		// value (not used)
    
    EMD_SKIP_HEAD_BYTES,    // value SInt64
                            //
                            // Skip specified bytes at the beginning of the 
                            // buffer after decryption and before processing.

    EMD_SKIP_TAIL_BYTES,    // value SInt64
                            //
                            // Skip specified bytes at the end of the 
                            // buffer after decryption and before processing.
    
    EMD_SET_START_OFFSET,   // value SInt64
                            //
                            // The native pts at which to start the
                            // presentation.  Samples with pts's less than this
                            // will not be rendererd.  Use ENCRYPTED_METADATA_NO_TIME_VALUE
                            // if there are no start offset requirements.  This
                            // meta data must be followed by EMD_SET_STOP_OFFSET.

    EMD_RESET_START_OFFSET, // value (not used)
                            //
                            // Remove existing start offset (will automatically
                            // be removed once playback time exceeds this time
                            // or on flushes).

    EMD_SET_STOP_OFFSET,    // value SInt64
                            //
                            // The native pts at which to stop the
                            // presentation.  Samples with pts's greater than this
                            // will not be rendererd.  Use ENCRYPTED_METADATA_NO_TIME_VALUE
                            // if there are no start offset requirements.  This
                            // meta data must be proceeded by EMD_SET_START_OFFSET.

    EMD_RESET_STOP_OFFSET,  // value (not used)
                            //
                            // Remove existing stop offset (will automatically
                            // be removed on flushes).

    EMD_BD_DECRYPT_METADATA,// value: Start SPN of data being provided., Playitem ID, and Clip ID
                            //     Bits 0-31  (32 bits): Start SPN of data being provided.
                            //     Bits 32-48 (17 bits): Clip ID (valid range: 0-99999). 0x1FFFF if invalid.
                            //     Bits 49-58 (10 bits): Playitem ID (valid range: 0-999). 0x3FF if invalid.
                            //     Bits 59-61 ( 3 bits): RESERVED for SKB Ordinal (valid range: 1-6). 0 if invalid.
                            //
                            // AACS/BD+ Sample Decryption Metadata
                            
    EMD_MAP_TIME,           // value = UInt32 of the output id to map to
                            //
                            // Use this to map the native pts's in the stream
                            // to the current presentation time of another 
                            // stream as an alternative to EMD_TIME_DISCONTINUITY.
                            // This is for picture-in-picture and other 
                            // secondary streams dependent on a different 
                            // stream's time base.  The map is reset when
                            // processing EMD_TIME_DISCONTINUITY or
                            // another EMD_MAP_TIME meta data.


    EMD_ENABLE_PREROLL,     // value (not used)
                            //
                            // Use this start pre-roll in the pipe line at
                            // the next portion of data.  The pipe line
                            // will fill up until its allocators are full
                            // (or the input stream ends) then playback
                            // will start.


    EMD_BD_LPCM_COEF,       // value = pointer to 18 bytes with the coefficients 
                            // OR 0 to disable.
                            //
                            // Set this if the BD LPCM stream requires
                            // down-mixing using specific coefficients.
                            // Down-mixing will not be performed if this is
                            // not called.


	EMD_MAX_PLAY_RATE		// value = x_speed * 1000 (speed in thousandths precision)
							// 
							// Sets the maximum speed which will be treated as a normal
							// playback speed (play all frames, no data discontinuities).
							// Anything above this rate will be treated as a trick play
							// or scan mode playback.
    
}EMD_FORMAT;

#define ENCRYPTED_METADATA_STREAM_ALL           0xFFFF
#define ENCRYPTED_METADATA_SUB_STREAM_ALL       0xFFFF

#define ENCRYPTED_METADATA_NO_TIME_VALUE        0x8000000000000000

typedef struct _tagEncryptedMetaData
{
    EMD_FORMAT	        format;		    // Describes the type of meta data
    
    long long             value;          // All meta data types use this one value.
                                        // Floating point types must be in the form
                                        // of value*1000 so 1.5 is 1500.

    WORD                streamID;       // Identify a specific stream in the input stream.
    WORD				subStreamID;    // Sub stream ID associated the stream ID.
                                        //
                                        // Use 0xFFFF for stream/sub_stream id
                                        // to apply the meta data to all output
                                        // streams in the input stream.  This 
                                        // is the most common use.  Sub stream
                                        // specific meta is for complex 
                                        // presentation like picture-in-picture.
} EncryptedMetaData;


#endif //_IEncryptedStreamControlTypes_h_
