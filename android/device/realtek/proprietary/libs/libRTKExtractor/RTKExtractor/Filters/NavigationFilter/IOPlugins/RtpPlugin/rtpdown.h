#ifndef __RTSPDOWN_H__
#define __RTSPDOWN_H__

#include <inttypes.h>
#include "rtp_net.h"

enum {
    STREAM_CHECK_DATA_TIME = 5,
};

enum {
    AUTO_RETRY_DEFAULT = 10,
};






enum {
    FORCE_NOT_METAFILE = -1,
    IS_NOT_METAFILE = 0,
    IS_METAFILE = 1,
};



struct dlresult_t {
    struct list_h *failed_list; 
    struct list_h *success_list; 

};


struct target_t {
    int metafile_f; /* tri-state */
    char *target_name;
};

struct options_t {

    char *username;
    char *password;
    char *http_proxy;
    char *local_filename;
    char *logfile;
    char *protocol;


    int auto_retry_times;
    int resume;
    
    unsigned int bandwidth;


    char *speed;
    char *range;

    
    struct list_h *targets; 
};

struct download_opts_t {
    int dl_protocol;      
    
    unsigned int bandwidth;
    char *speed;
    char *range;
    char *byterange;
    
    int auto_retry;
    int resume_download; 

    union {
	char *proxy;     
	char *http_proxy; 
    };

    char *password;
    char *username;

};





struct target_t *new_rtp_target_t(char *name,int meta_f);
void free_rtp_target_t(struct target_t *t);
struct options_t *new_rtp_options_t(void);
void free_rtp_options_t(struct options_t *opt);
struct download_opts_t *new_rtp_dlopts(void);
void free_rtp_dlopts(struct download_opts_t *dlopts);
struct dlresult_t *new_rtp_dlresult_t(void);
void free_rtp_dlresult_t(struct dlresult_t *dlr);
struct download_opts_t *set_rtp_dlopts(struct options_t *options,
						struct download_opts_t *dlopts);
struct stream_t * rtp_streaming_download_open(const char *local_file,struct url_t *url,struct download_opts_t *dlopts);

 int rtp_prepare_download(char *target_str,struct options_t *options,
			    struct url_t **url_ret,struct download_opts_t **dlopts_ret,
			    char **local_name);
			    
struct list_h *rtp_set_options(struct options_t *options,char *argv);

#endif
