#ifndef __RTSPDOWN_H__
#define __RTSPDOWN_H__

#include <inttypes.h>
#include "net_wrap.h"

#ifdef __cplusplus
extern "C" {
#endif

enum {
    AUTO_RETRY_DEFAULT = 10,
};


enum {
    FORCE_NOT_METAFILE = -1,
    IS_NOT_METAFILE = 0,
    IS_METAFILE = 1,
};

enum {
    STREAM_CHECK_DATA_TIME = 5,
};






struct target_t {
    int metafile_f; 
    char *target_name;
};

struct options_t {

    char *protocol;
    char *username;
    char *password;
    char *local_filename;
    char *logfile;    
    char *http_proxy;


    
    unsigned int bandwidth;
    int auto_retry_times;
    int resume;
    
    char *speed;
    char *range;

    
    struct list_h *targets; 
};


struct dlresult_t {
    struct list_h *success_list; 
    struct list_h *failed_list; 
};

struct download_opts_t {
    int dl_protocol;     
    int auto_retry;
    int resume_download;  
    
    unsigned int bandwidth;
    char *speed;
    char *range;
    char *byterange;
    

    union {
	char *proxy;      
	char *http_proxy;
    };



    char *password;
    char *username;    
};





struct dlresult_t *new_dlresult_t(void);
void free_dlresult_t(struct dlresult_t *dlret);
struct download_opts_t *set_dlopts(struct options_t *options,
						struct download_opts_t *dlopts);
struct stream_t * shen_streaming_download_open(const char *local_file,struct url_t *url,struct download_opts_t *dlopts);

struct target_t *new_target_t(char *tgname,int meta_f);
void free_target_t(struct target_t *t);
struct options_t *new_options_t(void);
void free_options_t(struct options_t *opt);
struct download_opts_t *new_dlopts(void);
void free_dlopts(struct download_opts_t *downldopts);
 int prepare_download(char *tgstring,struct options_t *options,
			    struct url_t **url_ret,struct download_opts_t **dlopts_ret,
			    char **local_name);
			    
struct list_h *set_options(struct options_t *options,char *argv);


#ifdef __cplusplus
}
#endif

#endif
