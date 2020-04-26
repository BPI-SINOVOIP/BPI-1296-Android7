#ifndef __NET_WRAP_H__
#define __NET_WRAP_H__

#include <inttypes.h>


int speed_valid_and_guess(const char *str,int *guessed_speed,char **reason_ret);
char *make_byterange_from_filesize(uint64_t filesize);
int get_protocol_type(char *protocol);

int sock_check_data(int sock,int timeout);

int server_connect(const char *servername,const int port);

struct netsock_t *new_netsock_t(void);
void free_netsock_t(struct netsock_t *ns);



struct serverinfo_t *new_server_info_t(void);
void free_server_info_t(struct serverinfo_t *si);

struct stream_ctrl_t *new_stream_ctrl_t(void);
void free_stream_ctrl_t(struct stream_ctrl_t *sc);

struct resumeinfo_t *new_resumeinfo_t(void);
void free_resumeinfo_t(struct resumeinfo_t *ri);



struct stream_t *new_stream_t(void);
void free_stream_t(struct stream_t *st);

struct stream_t *streaming_init_common();
void streaming_close_common(struct stream_t *stream);



enum {
    UNKNOWN_PROTOCOL = 0,
    HTTP,           
    RTSP,          
    RTSP_REAL,   
    RTSP_WMS,      
    
};


enum {
    INT_MAX_BANDWIDTH = 0x7fffffff, 
};

enum {
    HTTP_PORT = 80,

    RTSP_PORT = 554,
        HTTP_PROXY_PORT = 8080,
};



enum {
    BUFSIZE_1K = 1024,
    BUF_SIZE = 102400,    
    BUFSIZE_4_DL = 10240, 

};



int rtp_recv(int sock,void *buf,size_t count);
int xrecv(int sock,void *buf,size_t count);
int xsend(int sock,void *buf,size_t count);

int stream_check_data(struct stream_t *stream,int timeout);
int get_data(int sock, void *buf, size_t count);
int read_data1(struct stream_t *stream, void *buffer, size_t size);
int recv_data(struct stream_t *stream, void *buffer, size_t max);
int stream_data_push_back(struct stream_t *stream,void *buffer,int size);

struct netsock_t {
    int sock;                
    
    uint8_t *buffer;         
    uint32_t buffer_size;  
    uint32_t data_len;    
    uint32_t buffer_pos;      
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
    int resume_req_success;      
    uint64_t resume_start_offset; 
};



struct stream_t {
      
    struct stream_ctrl_t *stream_ctrl; 
    struct resumeinfo_t *resumeinfo;   
    
    struct download_opts_t *dlopts;    
    struct url_t *url;               

    struct serverinfo_t *serverinfo;  
    struct netsock_t *netsock;   
    
    int (*start)(struct stream_t *);                 
    int (*read)(struct stream_t *, uint8_t *,size_t);  
    void (*close)(struct stream_t *);                 
};



enum {
    STREAMING_HANDSHAKING,     
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


    unsigned int bandwidth;     
    int status;                 
    int retry_protocol;        
    char *retry_urlstr;      
    
    uint8_t *write_buffer;      
    uint32_t write_buffer_size; 
    uint32_t write_data_len;  
    uint32_t write_pos;        

    
    union {
	void *data;               
	struct rtsp_ctrl_t *rtsp_ctrl;  
    };
};









struct url_t {
    char *url;        
    char *protocol;    
    char *hostname;   
    char *file;     
    char *filepath;  
    char *username;
    char *password;    
    unsigned int port; 
    int protocol_type;

};

struct url_t *new_url_t(const char *url);
void free_url_t(struct url_t *u);
void copy_url_t(struct url_t *url,struct url_t *srcurl);

void set_serverinfo(struct serverinfo_t *serverinfo,
		    char *target_host,int target_port,
		    char *proxy_host,int proxy_port,int protocol_default_port);


#endif
