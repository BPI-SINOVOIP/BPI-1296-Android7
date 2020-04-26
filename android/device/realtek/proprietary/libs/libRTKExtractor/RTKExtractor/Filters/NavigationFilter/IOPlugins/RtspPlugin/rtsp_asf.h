#ifndef __RTSP_ASF_H__
#define __RTSP_ASF_H__




enum {

    ASF_COMMAND_MEDIA_STREAM = 0x59dacfc0,
    ASF_FILE_TRANSFER_MEDIA_STREAM = 0x91bd222c,
    ASF_AUDIO_STREAM = 0xf8699e40,
    ASF_VIDEO_STREAM = 0xbc19efc0,    
};

enum {

    ASF_HEADER_READ_AT_ONCE = 10000,
    ASF_GUID_LENGTH = 16,    
};


struct __attribute__((packed))  asf_obj_header_t {
    uint8_t guid[ASF_GUID_LENGTH];
    uint64_t size;
}; 


struct __attribute__((packed)) asf_header_t {
    struct asf_obj_header_t objh; 
    uint32_t cno;               
    uint8_t v1;                 
    uint8_t v2;                  
};




struct __attribute__((packed)) asf_stream_header_t {
    uint8_t type[ASF_GUID_LENGTH];        
    uint8_t concealment[ASF_GUID_LENGTH]; 
    uint64_t unk1;        
    uint32_t type_size; 
    uint32_t stream_size; 
    uint16_t stream_no;   
    uint32_t unk2;     
};



struct __attribute__((packed)) asf_stream_chunk_t {
    uint16_t type;
    uint16_t size;
    uint32_t seqnum;
};



struct __attribute__((packed)) asf_stream_chunk_extra_t {
    uint16_t unknown;
    uint16_t size_confirm;
};

struct __attribute__((packed)) asf_file_header_t {
    uint8_t  stream_id[ASF_GUID_LENGTH];  /* stream GUID */
    uint64_t file_size;
    uint64_t creation_time; 
    uint64_t num_packets;   
    uint64_t play_duration;  
    uint64_t send_duration;  
    uint64_t preroll;       
    uint32_t flags;          
    uint32_t min_packet_size;
    uint32_t max_packet_size;
    uint32_t max_bitrate;   
};


struct error_correction_data {

    int error_correction_length_type;
    int error_correction_present;
    int error_correction_data_length;
    int opaque_data_present;    
};


struct payload_parsing_information {

    /* property flags */
    int replicated_data_length_type;
    int offset_into_media_object_length_type;
    int media_object_number_length_type;
    int stream_number_length_type;

    /*length type flags*/
    int multiple_payloads_present;
    int sequence_type;
    int padding_length_type;
    int packet_length_type;
    int error_correction_present;
    
    uint32_t packet_length;
    uint32_t sequence;
    uint32_t padding_length;
    uint32_t send_time;
    uint32_t duration;
};


struct asf_data_packet_header_t {
    struct error_correction_data ecd;
    struct payload_parsing_information ppi;
};




struct asf_headerinfo_t {
    uint8_t *asf_header;             
    int asf_header_len;         
    struct asf_file_header_t *fileh;  
    struct asf_streams_t *streams;  
};




struct asf_streams_t {
    int audio_id,video_id; 
    int n_audio, n_video;  
    int *audio_streams,*video_streams;
};




int get_file_property(const uint8_t *header,int asf_header_size,
			  struct asf_file_header_t *fileh);
			  
int get_best_streams(const uint8_t *header, int asf_header_size,
			    int bw, struct asf_streams_t *streams);

int parse_asf_header(uint8_t *packet,struct asf_data_packet_header_t *adph);

int get_data_sendtime(uint8_t *packet,uint32_t *send_time);

struct asf_headerinfo_t *new_asf_headerinfo_t(void);

void free_asf_headerinfo_t(struct asf_headerinfo_t *info);

int asf_interpret_header(struct asf_headerinfo_t *hinfo,const int bw,
			 const uint8_t *buffer,const size_t asf_header_size);

int get_asf_guid(const uint8_t *buffer,int cur_pos,const uint8_t *guid,int buffer_len);

#endif
