#ifndef __LIBRTSP_H__
#define __LIBRTSP_H__


struct rtsp_header_t {
    char *protocol;

    unsigned int status_code;
    char *reason_phrase;
    char *method;
    char *uri;    
    unsigned int rtsp_minor_version;
    unsigned int content_length;
  

    /* field variables */
    struct rtsp_field_t *first_field;
    struct rtsp_field_t *last_field;
    unsigned int field_nb;
    char *field_search;
    struct rtsp_field_t *field_search_pos;
  

             
    char *buffer;
    size_t buffer_len;             
    unsigned int is_parsed;
    char *body;
    size_t body_len;      
  
};


struct rtsp_field_t {
    struct rtsp_field_t *next;
    char *field_name;
    
};



struct rtsp_ctrl_t {
    int rtsp_protocol;    
  

    uint32_t server_state;
    uint32_t server_caps;


    char *etag;
    uint32_t cseq;
    char *session;    
    char *challenge; 
    char *mrl;       
    char *server;
    
    uint32_t resume_start_ts;

    int (*get_media_packet)(struct stream_t *stream,
			    uint8_t *buffer,size_t max_size);

	//add by yunfeng_han
	int (*play)(struct stream_t *stream,long long t_offset);
  	int (*pause)(struct stream_t *stream);
	
    int prev_stream_number;
    int prev_ts;
    
    union {
	struct rmff_header_t *rmff_header;
	struct asf_headerinfo_t *asf_headerinfo;
    };
};


void rtsp_set_uri(struct rtsp_header_t *rtspHead,const char *uri);
void rtsp_set_method(struct rtsp_header_t *rtspHead, const char *method);
char *rtsp_build_request(struct rtsp_header_t *rtspHead);


int rtsp_parse_response(struct rtsp_ctrl_t *rtsp_ctrl,struct rtsp_header_t *rtsp_header);
int rtsp_check_totoal_header(struct rtsp_header_t *rtspHead);
int rtsp_response_parse(struct rtsp_header_t *rtspHead);
struct stream_t *rtsp_streaming_init();
int rtsp_streaming_read(struct stream_t *stream,
			uint8_t *buffer, size_t buffer_size);
void rtsp_streaming_close(struct stream_t *stream);

struct rtsp_header_t *new_rtsp_header_with_standard_fields(struct rtsp_ctrl_t *rtsp_ctrl);

int rtsp_set_standard_fields(struct rtsp_ctrl_t *rtsp_ctrl,
			     struct rtsp_header_t *rtspHead);

int rtsp_200ok(struct stream_t *stream,int cseq,char *session);
void free_rtsp_header(struct rtsp_header_t *h);
char *rtsp_get_next_field(struct rtsp_header_t *rtspHead);
char *rtsp_get_field(struct rtsp_header_t *rtspHead, const char *field_name);
void rtsp_set_field(struct rtsp_header_t *rtspHead, const char *field_name);

int rtsp_send_request(struct stream_t *stream,struct rtsp_header_t *rtspHead);
int rtsp_recv_header(struct stream_t *stream, struct rtsp_header_t *rtspHead);
int rtsp_build_path(struct rtsp_ctrl_t *rtsp_ctrl,char *host,int port,char *filepath);


int rtsp_check_valid_npttime(const char *str,char **reason_ret);
int rtsp_check_valid_nptrange(const char *str,char **reason_ret);
char *rtsp_time2npttime(const char *str,char **reason_ret);
char *rtsp_range_to_npt_range(const char *str,char **reason_ret);


void rtsp_set_range_field(struct rtsp_header_t *rtspHead,const char *rangestr);
void rtsp_set_speed_field(struct rtsp_header_t *rtspHead,const char *speedstr);

struct rtsp_header_t *new_rtsp_header(void);


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



int rtsp_send_request_and_free(struct stream_t *stream,struct rtsp_header_t *rtspHead);
int is_rtsp_response_ok(int status_code);
int rtsp_recv_header_ignore_message(struct stream_t *stream);
int rtsp_ignore_data_after_header(struct stream_t *stream,struct rtsp_header_t *rtspHead);
int rtsp_recv_header(struct stream_t *stream,struct rtsp_header_t *rtspHead);



char *rtsp_request_options(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_describe(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_set_parameter(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_get_parameter(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_setup(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_play(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_pause(struct rtsp_header_t *rtspHead,char *request_uri);
char *rtsp_request_teardown(struct rtsp_header_t *rtspHead,char *request_uri);



#endif
