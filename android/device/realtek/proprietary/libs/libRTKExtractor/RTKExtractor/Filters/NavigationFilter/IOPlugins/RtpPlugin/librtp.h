#ifndef __LIBRTSP_H__
#define __LIBRTSP_H__

enum {
    RTSP_UNKNOWN_PROTOCOL = 0,
    RTSP_REAL_PROTOCOL,
    RTSP_WMS_PROTOCOL,
};


enum {

    RTSP_STATUS_SET_PARAMETER = 10,
    RTSP_STATUS_OPTIONS = 20,
    RTSP_STATUS_ANNOUNCE = 30,
    RTSP_STATUS_OK = 200,    
};





struct rtsp_field_t {
    struct rtsp_field_t *next;
    char *field_name;
    
};



struct rtsp_ctrl_t {
    int rtsp_protocol; 


    uint32_t cseq;
    char *session;
    char *etag;
    char *challenge; 
    
    char *mrl;    
    char *server;
    uint32_t server_state;
    uint32_t server_caps;

    
    uint32_t resume_start_ts; 

    int (*get_media_packet)(struct stream_t *stream,
			    uint8_t *buffer,size_t max_size);

	//add by yunfeng_han
	int (*play)(struct stream_t *stream);
  	int (*pause)(struct stream_t *stream);
	
    int prev_ts;
    int prev_stream_number;
    
    union {
	struct asf_headerinfo_t *asf_headerinfo;
	struct rmff_header_t *rmff_header;	
    };
};


struct rtsp_header_t {
    char *protocol;
    char *method;
    char *uri;
    unsigned int rtsp_minor_version;
    unsigned int content_length;    
    unsigned int status_code;
    char *reason_phrase;

  



    char *body;
    size_t body_len;                
    char *buffer;
    size_t buffer_len;            
    unsigned int is_parsed;
    
    struct rtsp_field_t *first_field;
    struct rtsp_field_t *last_field;
    unsigned int field_nb;
    char *field_search;
    struct rtsp_field_t *field_search_pos;
      
  
};






void rtsp_set_uri(struct rtsp_header_t *rtsp_hdr,const char *uri);

char *rtsp_build_request(struct rtsp_header_t *rtsp_hdr);

struct rtsp_header_t *new_rtsp_header(void);
void free_rtsp_header(struct rtsp_header_t *h);

struct stream_t *rtp_streaming_init();
int rtp_streaming_read(struct stream_t *stream,
			uint8_t *buffer, size_t buffer_size);
void rtp_streaming_close(struct stream_t *stream);





int rtp_build_path(struct rtsp_ctrl_t *rtsp_ctrl,char *host,int port,char *filepath);



#endif
