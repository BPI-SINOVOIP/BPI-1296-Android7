#ifndef __NET_WRAP_H__
#define __NET_WRAP_H__

#include <inttypes.h>

enum {
    HTTP_PORT = 80,
    RTP_PORT = 554,    
    HTTP_PROXY_PORT = 8080,

};



enum {
    UNKNOWN_PROTOCOL = 0, 
    HTTP=1,        
    RTSP,         
    RTP,
    RTSP_REAL,     
    RTSP_WMS,       
};


enum {
    INT_MAX_BANDWIDTH = 0x7fffffff,
};


enum {

    BUFSIZE_1K = 1024,  
    BUFSIZE_4_DL = 10240,     
    BUF_SIZE = 102400,      
};




struct serverinfo_t {
    char *connect_host; 
    int   connect_port;

    char *host;
    int   port;

    char *proxy_host;
    int   proxy_port;
};


struct resumeinfo_t {
    uint64_t resume_start_offset; 
    int resume_req_success;     
    
};


struct netsock_t {
    int sock;              
    
    uint8_t *buffer;          
    uint32_t buffer_size;    
    uint32_t data_len;        
    uint32_t buffer_pos;      
};




struct stream_t {
    struct serverinfo_t *serverinfo;  
    struct netsock_t *netsock;        
    struct stream_ctrl_t *stream_ctrl; 
    struct resumeinfo_t *resumeinfo;   
    
    struct download_opts_t *dlopts;    
    struct url_t *url;               
              
    int (*read)(struct stream_t *, uint8_t *,size_t); 
    
    int (*start)(struct stream_t *);        
    void (*close)(struct stream_t *);                 
};



enum {
    STREAMING_HANDSHAKING=0,     
    STREAMING_DOWNLOADING,      
    STREAMING_FINISHED,         
    STREAMING_REWIND,          
    STREAMING_RESUME_BUFFERING,  
    STREAMING_NO_NEED_TO_DOWNLOAD,  
    STREAMING_OTHER_PROTOCOL,    
    STREAMING_REDIRECTED,      
};




struct stream_ctrl_t {
    int protocol;            

    int packet_length;         

    uint64_t file_size;        
  
    int total_packets;          
    int packet_count;           

    uint32_t write_buffer_size; 
    uint32_t write_data_len;   
    uint32_t write_pos;  
    
    uint8_t *write_buffer;     
      

    unsigned int bandwidth;    
    int status;                
    int retry_protocol;         
    char *retry_urlstr;         
    
    union {
	void *data;            
	struct rtsp_ctrl_t *rtsp_ctrl;  
    };
};





struct stream_t *new_rtp_stream_t(void);
void free_rtp_stream_t(struct stream_t *st);

struct stream_t *rtp_streaming_init_common();
void rtp_streaming_close_common(struct stream_t *stream);

struct netsock_t *new_rtp_netsock_t(void);
void free_rtp_netsock_t(struct netsock_t *ns);

struct serverinfo_t *new_rtp_server_info_t(void);
void free_rtp_server_info_t(struct serverinfo_t *si);

struct stream_ctrl_t *new_rtp_stream_ctrl_t(void);
void free_rtp_stream_ctrl_t(struct stream_ctrl_t *sc);

struct resumeinfo_t *new_rtp_resumeinfo_t(void);
void free_rtp_resumeinfo_t(struct resumeinfo_t *ri);

void set_rtp_serverinfo(struct serverinfo_t *serverinfo,
		    char *target_host,int target_port,
		    char *proxy_host,int proxy_port,int protocol_default_port);


int get_rtp_protocol_type(char *protocol);

int rtp_sock_check_data(int sock,int timeout);

int rtp_server_connect(const char *servername,const int port);


int rtp_rtp_recv(int sock,void *buf,size_t count);
int rtp_xrecv(int sock,void *buf,size_t count);
int rtp_xsend(int sock,void *buf,size_t count);

int rtp_stream_check_data(struct stream_t *stream,int timeout);
int rtp_get_data(int sock, void *buf, size_t count);
int rtp_read_data1(struct stream_t *stream, void *buffer, size_t size);
int rtp_recv_data(struct stream_t *stream, void *buffer, size_t max);
int rtp_stream_data_push_back(struct stream_t *stream,void *buffer,int size);



struct url_t {
    char *url;         
    char *protocol;    
    char *hostname;    
    char *file;       
    char *filepath;    
    unsigned int port; 
    int protocol_type; 
    char *username;
    char *password;
};

struct url_t *new_rtp_url_t(const char *url);
void free_rtp_url_t(struct url_t *u);
void copy_rtp_url_t(struct url_t *url,struct url_t *srcurl);

#endif
