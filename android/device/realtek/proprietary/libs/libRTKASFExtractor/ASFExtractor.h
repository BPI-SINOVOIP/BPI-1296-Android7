#ifndef ASF_EXTRACTOR_H_

#define ASF_EXTRACTOR_H_

#include <media/stagefright/foundation/ABase.h>
#include <media/stagefright/MediaExtractor.h>
#include <media/IMediaSource.h>
#include <utils/Vector.h>
#include "ASFGUID.h"

namespace android {

struct ASFExtractor : public MediaExtractor {
    ASFExtractor(const sp<DataSource> &dataSource);

    virtual size_t countTracks();

    virtual sp<IMediaSource> getTrack(size_t index);

    virtual sp<MetaData> getTrackMetaData(
            size_t index, uint32_t flags);

    virtual sp<MetaData> getMetaData();

protected:
    virtual ~ASFExtractor();

private:
    struct ASFSource;

    struct Track {
        sp<MetaData> mMeta;

        enum Kind {
            AUDIO,
            VIDEO,
            OTHER
        } mKind;

        size_t mMaxSampleSize;

        // 0: not VC1, 1: WVC1, 2: WMV3
        uint32_t mVC1Type;
    };

    sp<DataSource> mDataSource;
    status_t mInitCheck;
    Vector<Track> mTracks;

    // ASFExtractor used
    /* read_payloads() return status */
    typedef enum {
        ASF_READ_ERROR = -1,
        ASF_READ_END = -2,
        ASF_READ_ERROR_BUSY = -3,
        ASF_NO_DEVICE = -4,
        ASF_READ_AGAIN = -5,
        ASF_READ_ERROR_EIO = -6,
        ASF_READ_BITSTREAM_ERROR = -7,
        ASF_READ_HEADER_ERROR = -8
    } asf_read_status_t;

    typedef struct _file_properties_object
    {
        ASF_GUID fileID;
        long long fileSize; // the size of the entire file
        long long creationDate; // the date of the initial creation of the file
        long long dataPacketsCount; // number of Data Packet within the Data Object.
        long long playDuration; // the time need to play this file in 100-nanosec units.
        long long sendDuration; // the time need to send this file in 100-nanosec units.
        long long preroll; // the time to buffer data before playing this file
        struct {
            unsigned long broadcastFlag : 1; // bit 0 : broadcast flag
            unsigned long seekableFlag : 1; // bit 1 : seekable flag
            unsigned long reserved : 30;
        } __attribute__((packed)) flags;
        long minDataPacketSize; // the minimum Data Packet size in bytes
        long maxDataPacketSize; // the maximun Data Packet size in bytes
        long maxBitRate; // the maximum instantaneous bit rate in bits per second for this file
    } __attribute__((packed)) file_properties_object_t;

    typedef struct _presentation_time
    {
        long PTS;
        long deltaPTS;
    } __attribute__((packed)) presentation_time_t;

    typedef struct _payload_parsing_info
    {
        /*-----------------------------------
        payload parsing information fields
        -------------------------------------*/
        struct {
            unsigned char multiplePayloadsPresent : 1; // Multiple Payloads Present
            unsigned char sequenceType : 2; // Sequence Type
            unsigned char paddingLenType : 2; // Padding Length Type
            unsigned char packetLenType : 2; // Packet Length Type
            unsigned char errorCorrPresent : 1; // Error Correction Present
        } __attribute__((packed)) lenTypeFlag;

        //char	propertyFlags;
        struct {
            unsigned char dataLenType : 2; // Replicated Data Length Type
            unsigned char offsetLenType : 2; // Offset into Media Object Length Type
            unsigned char objectNumType : 2; // Media Object Number Length Type
            unsigned char streamNumType : 2; // Stream Number Length Type
        } __attribute__((packed)) propertyFlags;

        long packetLen; // the length of the data packet
        long paddingLen; // the length of the padding at the end of the data packet
        long sendTime; // the send time of the Data Packet
        short duration; // duration of the Data Packet

        /*-----------------------------------
        payload  fields
        -------------------------------------*/

        struct {
            unsigned char	numPayloads : 6; // Number of Payloads
            unsigned char	payloadLenType : 2; // Payload Length Type
        } __attribute__((packed)) payloadFlags;

        struct {
            unsigned char streamNum : 7; // the number of the stream
            unsigned char isKeyFrame : 1; // Key frame bit
        } __attribute__((packed)) streamNumFlags;

        union {
            long mediaObjectOffset; // non-compressed Payloads
            presentation_time_t presentationTime; // Compressed Payloads
        };

        /*----------------------------------------
        the followings are not standard  fields
        ------------------------------------------*/

        long packetHeaderLen; // the length of Payload parsing information
        long payloadHeaderLen;
        long replicatedDataLen;
        long totalSubPayloadSize; // for compressed payload data, this field is total subpayload data size
                                                   // for multiple payloads single payload data, it is the size of one payload data
        long packetTotalReadBytes;	// count if each packet really match to maxDatapacketSize
        long mediaObjectSize;
        long pts;
    } __attribute__((packed)) payload_parsing_info_t;

    typedef struct _audio_media_data
    {
        short codecID; // unigue ID of the codec used to encode audio
        short numChannel;
        long samplesPerSec;
        long avgBytesPerSec;
        short blockAlign;
        short bitsPerSample;
        short specificDataSize;
        //unsigned char*	specificData;
    } __attribute__((packed)) audio_media_data_t;

    typedef struct _video_media_data
    {
        long imageWidth;
        long imageHeight;
        char reservedFlag;
        short formatDataSize;
        //unsigned char*	formatData;
    } __attribute__((packed)) video_media_data_t;

    typedef struct _audio_error_correction_data
    {
        char span;
        /*
        short           virtualPacketLength;
        short           virtualChunkLength;
        short           silenceDataLength;
        unsigned char* silenceData;
        */
    }__attribute__((packed)) audio_error_correction_data_t;

    typedef struct _media_properites
    {
        unsigned char isAudio; // 0: this entry is the properites of video stream;
                                            // otherwise, it is audio's;
        union {
            audio_media_data_t audio;
            video_media_data_t video;
        };

        audio_error_correction_data_t error; //added by helen_he for mantis bug 29311
        unsigned char* mediaData; // if Stream Type is ASF_Audio_Media, it is specificData of Audio media data
                                                    // otherwise, it is formatData of Video media data
    } __attribute__((packed)) media_properties_t;

    typedef struct _payload_extension
    {
        ASF_GUID GUID;
        short extDataSize;
        long extSysInfoLen; // Extension System Info Length
        struct _payload_extension* next; // pointer to the next payload extension system
    } __attribute__((packed)) payload_extension_t;

    typedef struct _extended_stream_properties
    {
        long long startTime; // the presentation time of the first object
        long long  endTime; // the presentation time of the last object
        long dataBitrate1;
        long bufferSize1;
        long bufferFullness1;
        long dataBitrate2; // Alternate Data Bitrate
        long bufferSize2; // Alternate Buffer Size
        long bufferFullness2; // Alternate Initial Buffer Fullness
        long maxObjectSize; // max size of the largest sample in the data packet
        long flags;
        short streamNum;
        short streamLanguateIDidx; // Stream Language ID index
        long long avgTimePerFrame;
        short streamNameCount;
        short payloadExtSysCount; // Payload Extension System Count
        struct {
            short LangIDidx; // Language ID index
            short streamNameLen;
        }__attribute__((packed)) streamNames;

        payload_extension_t* payloadExtSystems;
    } __attribute__((packed)) extended_stream_properties_t;

    typedef struct _metadata_description
    {
        short streamNum;
        unsigned char* pName;
        unsigned char* pData;
        short nameLen;
        int dataLen;
        short dataType;
    } metadata_description_t;

    typedef struct _metadata_object
    {
        int recordCount;
        metadata_description_t* pDescRecord;
    } metadata_t;

    typedef struct _asf_pinpong_buffer_st {
        unsigned char* myBuffer;
        int totalReadBytes;
    }asf_pinpong_buffer_t;

    typedef struct _asf_jump_collector {
        bool bNeedToJump;
        long long jumpPos;
    } asf_jump_collector_t;

    typedef struct _idx_specifier
    {
        short streamNum;
        short indexType;
    } __attribute__((packed)) idx_specifier_t;

    typedef struct _idx_block
    {
        long entry_count;
        long long* blockPosition; // long long blockPosition[specifiersCount]
        long* offset; // long offset[entry_count][specifiersCount]
    } __attribute__((packed)) idx_block_t;

    typedef struct _index_object
    {
        long timeInterval; // Index Entry Time Interval
        short specifiersCount; // Index Specifiers Count
        long blocksCount; // Index Blocks Count

        idx_specifier_t* idxSpecifers;
        idx_block_t* idxBlocks;
    } __attribute__((packed)) index_object_t;

    typedef struct _s_idx_block_t
    {
        int packetNumber;
        short packetCount;
    } __attribute__((packed)) s_idx_block_t;

    typedef struct _simple_index_object
    {
        long long timeInterval;
        int MaxpacketsInEntry;
        int idxCount;
        s_idx_block_t* idxEntryTable;
    } __attribute__((packed)) simple_index_object_t;

    typedef struct _header_object
    {
        ASF_GUID GUID;
        long long objectSize;
        long numHeaders; // number of objects within the HEADER OBJECT, not including this one
        char reserved1; // ASF parser may safely ignore this value
        char reserved2; // must be set to 0x02
    } __attribute__((packed)) header_object_t;

    typedef struct _asf_sub_header
    {
        ASF_GUID GUID;
        long long objectSize;
    }__attribute__((packed)) asf_sub_header_t;

    typedef struct _stream_properties_object
    {
        ASF_GUID streamType;
        ASF_GUID errCorrType; // audio stream : ASF_Audio+Spread, the others : ASF_No_Error_Correction
        long long timeOffset; // the presentation time offset of the stream in 100-nanosec units
        long typeDataLen;
        long errCorrDataLen;
        struct {
            unsigned char streamNum : 7;
            unsigned char reserved : 8;
            unsigned char encryptedContentFlag : 1;
        }__attribute__((packed)) flags;
        //short flags;
        long reserved;
    } __attribute__((packed)) stream_properties_object_t;

    typedef struct _bitmap_info_header
    {
        long formatDataSize;
        long imageWidth;
        long imageHeight;
        short reserved;
        short bitsPerPixelCnt;
        long compressionID;
        long imageSize;
        long horizontalPixels;
        long verticalPixels;
        long colorsUsedCnt;
        long importantColorsCnt;
    } __attribute__((packed)) _bitmap_info_header_t;

    typedef struct _tagVIDEOVC1SEQINFO
    {
        unsigned int profile;
        unsigned int displayWidth;
        unsigned int displayHeight;
        unsigned char extraData[128];
    }VIDEOVC1SEQINFO;

    /* video format information (PRIVATEINFO_VIDEO_FORMAT) */
    typedef struct _tagVIDEOFORMAT {
        unsigned int dwWidth;
        unsigned int dwHeight;
    } VIDEOFORMAT;

    typedef struct _binary_media_data
    {
        ASF_GUID majorMediatype;
        ASF_GUID mediaSubtype;
        long fixdSizeSamples;
        long temporalCompression;
        long sampleSize;
        ASF_GUID formatType;
        long formatDataSize;
        //unsigned char*	formatData;
    } __attribute__((packed)) binary_media_data_t;

    // Trick Play related structure //
    typedef struct _asf_count_jump_res {
        int bSuccess;
        int bUseIndex;
        long long dest;
    }asf_count_jump_res_t;

    typedef struct _error_correct_data
    {
        unsigned char errCorrDataLen : 4; // Error Correction Data Length
        unsigned char opaqueDataPresent : 1; // Opaque Data Present
        unsigned char errCorrLenType : 2; // Error Correction Length Type
        unsigned char errCorrPresent : 1; // Error Correction Present
    } __attribute__((packed)) error_correct_data_t ;

    typedef struct _WAVEFORMATEX {
        uint16_t wFormatTag;
        uint16_t nChannels;
        uint32_t nSamplesPerSec;
        uint32_t nAvgBytesPerSec;
        uint16_t nBlockAlign;
        uint16_t wBitsPerSample;
        uint16_t cbSize;
        uint16_t reserved;
    } WAVEFORMATEX;

    #define MIN_ASF_HEADER_OBJECT_SIZE	24 // sizeof(ASF_GUID) + sizeof(long long)
    #define ASF_DATA_OBJECT_HEADER_SIZE 50	//Data Object GUID(16) + Object Size (8) + File ID(16) + Total Data Packets(8) + Reversed (2) = 50 bytes
    #define MAX_STREAM_NUM 127

    #define ASF_CURR_STREAM_INDEX_VIDEO 0
    #define ASF_CURR_STREAM_INDEX_VIDEO1 1
    #define ASF_CURR_STREAM_INDEX_VIDEO2 2
    #define ASF_CURR_STREAM_INDEX_VIDEO3 3
    #define ASF_CURR_STREAM_INDEX_VIDEO4 4
    #define ASF_CURR_STREAM_INDEX_VIDEO5 5
    #define ASF_CURR_STREAM_INDEX_VIDEO6 6
    #define ASF_CURR_STREAM_INDEX_VIDEO7 7

    #define ASF_CURR_STREAM_INDEX_AUDIO 8
    #define ASF_CURR_STREAM_INDEX_AUDIO1 9
    #define ASF_CURR_STREAM_INDEX_AUDIO2 10
    #define ASF_CURR_STREAM_INDEX_AUDIO3 11
    #define ASF_CURR_STREAM_INDEX_AUDIO4 12
    #define ASF_CURR_STREAM_INDEX_AUDIO5 13
    #define ASF_CURR_STREAM_INDEX_AUDIO6 14
    #define ASF_CURR_STREAM_INDEX_AUDIO7 15
    #define ASF_CURR_STREAM_INDEX_AUDIO8 16
    #define ASF_CURR_STREAM_INDEX_AUDIO9 17
    #define ASF_CURR_STREAM_INDEX_UNKNOWN 18

    // flags for ompressionID field for _bitmap_info_header_t
    #define WMV_FORMAT_WMV1 0x31564D57
    #define WMV_FORMAT_WMV2 0x32564D57
    #define WMV_FORMAT_WMV3 0x33564D57
    #define WMV_FORMAT_MP4S 0x5334504D
    #define WMV_FORMAT_M4S2 0x3253344D
    #define WMV_FORMAT_MP43 0x3334504D
    #define WMV_FORMAT_WVC1 0x31435657
    #define WMV_FORMAT_WMVA 0x41564D57
    // dvix4
    #define WMV_FORMAT_DIVX 0x58564944
    //divx3
    #define WMV_FORMAT_DIV3 0x33564944
    //prdy playready
    #define WMV_FORMAT_PRDY 0x59445250
    //H264
    #define WMV_FORMAT_H264 0x34363248
    //"DVR "
    #define WMV_FORMAT_DVR 0x20525644

    // flags for wFormatTag field of WAVEFORMAT
    #define WMA_FORMAT_PCM 0x0001
    #define WMA_FORMAT_ADPCM 0x0002
    #define WMA_FORMAT_MPEGLAYER3 0x0055
    #define WMA_FORMAT_WMAPRO 0x0162
    #define WMA_FORMAT_G726_ADPCM 0x0045
    #define WMA_FORMAT_AAC_2 0x1601
    #define WMA_FORMAT_AC3 0x0081
//////////////////////////////////

    /*--------------------------------------------------------------
        Medium information
    ---------------------------------------------------------------*/
    file_properties_object_t FileProperties;
    payload_parsing_info_t payloadInfo;
    media_properties_t* asfStream[MAX_STREAM_NUM]; // array index is (streamNum - 1)
    extended_stream_properties_t* extStreamProperties[MAX_STREAM_NUM]; // array index is (streamNum - 1)
    metadata_t metaData;
    index_object_t* idxObject;
    simple_index_object_t* sidxObject;

    /*--------------------------------------------------------------
        IO stuff
    ---------------------------------------------------------------*/
    long long PacketStartPos;
    long long PacketEndPos;
    long long LastVideoReadOffset;
    long long LastAudioReadOffset;
    long long currFileReadPos;
    long long fileTotalSize;

    /*--------------------------------------------------------------
        Playback information
    ---------------------------------------------------------------*/
    asf_jump_collector_t jumpCollector;
    int curStreamNum[ASF_CURR_STREAM_INDEX_UNKNOWN+1]; // currStreamNum[0] : the number of current video stream
                                                                                                  // currStreamNum[1] : the number of current audio stream
                                                                                                  // currStreamNum[2] : the number of current audio stream
                                                                                                  // currStreamNum[3] : the number of remembering unknown stream
    long long dataObjectStart; // start address of the Data Object
    long long dataObjectEnd; // end address of the Data Object
    long long dataObjectEndInHeader; // end address of the Data Object
    bool isSeekVideoOrAudio;

    /*--------------------------------------------------------------
        Trick Play stuff
    ---------------------------------------------------------------*/
    int idxOfIndex; // trick play's index reference (has index table)
    asf_count_jump_res_t jump_res;

    /*--------------------------------------------------------------
        Inband command stuff
    ---------------------------------------------------------------*/
    VIDEOFORMAT videoFormat[8];
    VIDEOVC1SEQINFO videoFormatVC1[8];
    WAVEFORMATEX waveformHeader;

    /*--------------------------------------------------------------
        WMV stuff
    ---------------------------------------------------------------*/
    int curAudioStreamNum; // for mutiple audio channel
    int curVideoStreamChannel; // for mutiple video channel
    int bIsVideoUseDefaultStream; // use default video stream num or the first packet
// end of ASFExtractor used

    long long doing_seek(long long size);
    ssize_t doing_read(off64_t offset, void* pBuffer, size_t size);
    long long jump_next_data_packet_position();
    int read_payload_header();
    int read_data_packets();
    status_t read_data_and_index_objects(asf_sub_header_t* psh);
    status_t read_stream_properties_object();
    status_t read_metadata();
    status_t read_header_extension_objects(long long extension_object_size);
    status_t read_header_objects();
    status_t parseHeaders();

    status_t asf_query_index_from_time(int64_t jumpUSecond);

    status_t asf_playback_atTime(
            size_t trackIndex,
            int64_t timeUs, IMediaSource::ReadOptions::SeekMode mode,
            long long *seektofileoffset);
};

class String8;
struct AMessage;

//bool SniffASF(
//        const sp<DataSource> &source, String8 *mimeType, float *confidence,
//        sp<AMessage> *);

}  // namespace android

#endif  // ASF_EXTRACTOR_H_
