#ifndef COMMON_H
#define COMMON_H

#include<stdio.h>
#include<stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>



typedef enum
{
  FSDP_NETWORK_TYPE_UNDEFINED,		    
  FSDP_NETWORK_TYPE_INET		     
} fsdp_network_type_t;


typedef enum
{
  FSDP_ADDRESS_TYPE_UNDEFINED,		  
  FSDP_ADDRESS_TYPE_IPV4,		    
  FSDP_ADDRESS_TYPE_IPV6		   
} fsdp_address_type_t;


typedef enum
{
  FSDP_BW_MOD_TYPE_UNDEFINED,		
  FSDP_BW_MOD_TYPE_UNKNOWN,		
  FSDP_BW_MOD_TYPE_CONFERENCE_TOTAL,	
  FSDP_BW_MOD_TYPE_APPLICATION_SPECIFIC, 
  FSDP_BW_MOD_TYPE_RTCP_SENDERS,	
  FSDP_BW_MOD_TYPE_RTCP_RECEIVERS,	
} fsdp_bw_modifier_type_t;


typedef enum
{
  FSDP_ENCRYPTION_METHOD_UNDEFINED, 
  FSDP_ENCRYPTION_METHOD_CLEAR,	       
  FSDP_ENCRYPTION_METHOD_BASE64,      
  FSDP_ENCRYPTION_METHOD_URI,	      
  FSDP_ENCRYPTION_METHOD_PROMPT	       
} fsdp_encryption_method_t;


typedef enum
{
  FSDP_SENDRECV_UNDEFINED,		    
  FSDP_SENDRECV_SENDRECV,		   
  FSDP_SENDRECV_RECVONLY,		   
  FSDP_SENDRECV_SENDONLY,		    
  FSDP_SENDRECV_INACTIVE		    
} fsdp_sendrecv_mode_t;

typedef enum
{
  FSDP_ORIENT_UNDEFINED,		   
  FSDP_ORIENT_PORTRAIT,		
  FSDP_ORIENT_LANDSCAPE,		 
  FSDP_ORIENT_SEASCAPE			
} fsdp_orient_t;


typedef enum
{
  FSDP_SESSION_TYPE_UNDEFINED,		  
  FSDP_SESSION_TYPE_BROADCAST,		   
  FSDP_SESSION_TYPE_MEETING,		   
  FSDP_SESSION_TYPE_MODERATED,	
  FSDP_SESSION_TYPE_TEST,		      
  FSDP_SESSION_TYPE_H332		     
} fsdp_session_type_t;


typedef enum
{
  FSDP_MEDIA_UNDEFINED,		 
  FSDP_MEDIA_VIDEO,		 
  FSDP_MEDIA_AUDIO,		
  FSDP_MEDIA_APPLICATION,	 
  FSDP_MEDIA_DATA,		  
  FSDP_MEDIA_CONTROL		 
} fsdp_media_t;


typedef enum
{
  FSDP_TP_UNDEFINED,		  
  FSDP_TP_RTP_AVP,		
  FSDP_TP_UDP,			
  FSDP_TP_TCP,			
  FSDP_TP_UDPTL,		
  FSDP_TP_VAT,			  
  FSDP_TP_OLD_RTP,		 
  FSDP_TP_H320			
} fsdp_transport_protocol_t;


typedef enum
{
  FSDP_SESSION_STR_ATT_CATEGORY,
  FSDP_SESSION_STR_ATT_KEYWORDS,
  FSDP_SESSION_STR_ATT_TOOL,
  FSDP_SESSION_STR_ATT_CHARSET,
} fsdp_session_str_att_t;



typedef struct fsdp_media_description_t_s fsdp_media_description_t;


typedef struct fsdp_description_t_s fsdp_description_t;


fsdp_description_t *fsdp_description_new (void);


void fsdp_description_delete (fsdp_description_t * dsc);



const char *fsdp_strerror (int err_no);





static inline uint32_t bswap_32_1(uint32_t x)
{
    x= ((x<<8)&0xFF00FF00) | ((x>>8)&0x00FF00FF);
    return (x>>16) | (x<<16);
}


#ifdef WORDS_BIGENDIAN

#define be2me_16(x) (x)
#define be2me_32(x) (x)
#define be2me_64(x) (x)
#define le2me_16(x) bswap_16(x)
#define le2me_32(x) bswap_32_1(x)
#define le2me_64(x) bswap_64(x)

#else 

#define be2me_16(x) bswap_16(x)
#define be2me_32(x) bswap_32_1(x)
#define be2me_64(x) bswap_64(x)
#define le2me_16(x) (x)
#define le2me_32(x) (x)
#define le2me_64(x) (x)

#endif



static inline void put8(void *pVal,uint8_t x)
{
    *(uint8_t *)pVal       = x       & 0xff;
}

static inline void put16_be(void *pVal,uint16_t x)
{
    *((uint8_t *)pVal    ) = x >>8   & 0xff;
    *((uint8_t *)pVal + 1) = x       & 0xff;
}

static inline void put16_le(void *pVal,uint16_t x)
{
    *((uint8_t *)pVal    ) = x       & 0xff;
    *((uint8_t *)pVal + 1) = x >>8   & 0xff;
}

static inline void put32_be(void *pVal,uint32_t x)
{
    *((uint8_t *)pVal    ) = x >> 24 & 0xff;
    *((uint8_t *)pVal + 1) = x >> 16 & 0xff;
    *((uint8_t *)pVal + 2) = x >>  8 & 0xff;
    *((uint8_t *)pVal + 3) = x       & 0xff;
}

static inline void put32_le(void *pVal,uint32_t x)
{
    *((uint8_t *)pVal    ) = x       & 0xff;
    *((uint8_t *)pVal + 1) = x >>  8 & 0xff;
    *((uint8_t *)pVal + 2) = x >> 16 & 0xff;
    *((uint8_t *)pVal + 3) = x >> 24 & 0xff;
}


static inline uint32_t get32_be(void *pVal)
{
    return *(uint8_t *)pVal << 24 | *((uint8_t *)pVal + 1) << 16 |
	*((uint8_t *)pVal + 2) << 8 | *((uint8_t *)pVal + 3);
}

static inline uint32_t get32_le(void *pVal)
{
    return *(uint8_t *)pVal | *((uint8_t *)pVal + 1) << 8 |
	*((uint8_t *)pVal + 2) << 16 | *((uint8_t *)pVal + 3) << 24;
}

static inline uint64_t get64_be(void *pVal)
{
    return (uint64_t)get32_be((uint8_t *)pVal) << 32 | (uint64_t)get32_be((uint8_t *)pVal + 4);
}

static inline uint64_t get64_le(void *pVal)
{
    return (uint64_t)get32_le((uint8_t *)pVal + 4) << 32 | (uint64_t)get32_le((uint8_t *)pVal);
}

static inline uint64_t getnb_le(void *pVal,int n)
{
    uint64_t val = 0;
    for(; n > 0 ; n--) {
	val <<= 8;
	val += *((uint8_t *)pVal + n - 1);
    }
    return val;
}

static inline uint64_t getnb_be(void *pVal,int n)
{
    uint64_t val = 0;
    int i = 0;
    for(; i < n; i++) {
	val <<= 8;
	val += *((uint8_t *)pVal + i);
    }
    return val;
}


static inline uint16_t get16_be(void *pVal)
{
    return *(uint8_t *)pVal << 8 | *((uint8_t *)pVal + 1);
}

static inline uint16_t get16_le(void *pVal)
{
    return *(uint8_t *)pVal | *((uint8_t *)pVal + 1) << 8;
}

static inline uint8_t get8(void *pVal)
{
    return *(uint8_t *)pVal;
}







struct list_h {
    void *p;
    struct list_h *next;
    struct list_h *prev;
};

#define free(p) do {free((p));(p) = NULL;} while(0)


char *xstrcasechr(const char *s1,int c);
char *xstrstr(const char *s1,const char *s2);
char *xstrcasestr(const char *s1,const char *s2);

void *xmalloc2(const size_t size);
void *xrealloc2(void *p,const size_t size);

struct list_h *list_h_append(struct list_h **list,void *p);

struct list_h *new_list_h(void *p);
void free_list_h(struct list_h *list,void (*free_er)(void *));
int base64_decode(char *enc,char *dec,int decsize);
int is_line(char *line,char *keyword,char **buf,int *len);
char *next_line(char *data);
int adjust_size(char **buf,int *buflen,int keep);


enum {
    S_ERROR = -1,
    S_NULL,
    S_AND,
    S_OR,

    S_LESS,            
    S_LESSEQ,
    S_EQ,
    S_MOREEQ,
    S_MORE,           
  
    S_ID, 
    S_NUM,

    S_SUBSTI, 
  
    S_LPAREN,
    S_RPAREN,
  
} symbols;



enum {
    O_ERR = -1,
    O_NULL,
    O_AND,
    O_OR,

    O_LESS,        
    O_LESSEQ,
    O_EQ,
    O_MOREEQ,
    O_MORE,      
  
    O_ID, 
    O_NUM,
  
} operators;


struct c_desc { 
    char *whole_line;
    char *cur_p;

    int op;
    int val;
    char *id;
    int id_len;
};


struct node_t {
    int op;
    struct node_t *left;
    struct node_t *right;
};

int is_match_in_asm(char *rulestr,int bw);


#define NTP_EPOCH_OFFSET 2208988800UL
#define FSDP_MAX_LENGTH 2000

typedef struct fsdp_connection_address_t_s
{
  char *address;

  unsigned int address_count;
    unsigned int address_ttl;
} fsdp_connection_address_t;

typedef struct 
{
  int length;
  int width;
}fsdp_framesize_t;



typedef struct
{
  char *pt;
  char *encoding_name;

  char *parameters;
    unsigned int clock_rate;
} fsdp_rtpmap_t;

  typedef struct
{
  fsdp_bw_modifier_type_t b_mod_type;

  char *b_unknown_bw_modt;
    unsigned long int b_value;
} fsdp_bw_modifier_t; 




struct fsdp_media_description_t_s
{

  unsigned int port;
  unsigned int port_count;
  fsdp_media_t media_type;
  fsdp_transport_protocol_t transport;
  char **formats;
  unsigned int formats_count;

  char *i_title;


  fsdp_connection_address_t c_address;
  fsdp_network_type_t c_network_type;
  fsdp_address_type_t c_address_type;
  
  fsdp_bw_modifier_t *bw_modifiers;
  unsigned int bw_modifiers_count;


  unsigned long int a_ptime;
  unsigned long int a_maxptime;
  
  fsdp_encryption_method_t k_encryption_method;
  char *k_encryption_content;


  fsdp_rtpmap_t **a_rtpmaps;
  unsigned int a_rtpmaps_count;
  fsdp_orient_t a_orient;
  fsdp_sendrecv_mode_t a_sendrecv_mode;

  char **a_sdplangs;
  unsigned int a_sdplangs_count;
  char **a_langs;
  unsigned int a_langs_count;

  char **a_controls;
  unsigned int a_controls_count;

  int fmtp_type;
  int profile_level_id;
  int sizelength;
  int indexlength;
  int indexdeltalength;
  int streamtype;
  char *config;
  char *mode;
  char *sprop_parameter_sets;
  char *a_range;

  float a_framerate;
  unsigned int a_quality;
  char **a_fmtps;
  unsigned int a_fmtps_count;

  unsigned int a_rtcp_port;
  fsdp_network_type_t a_rtcp_network_type;
  fsdp_address_type_t a_rtcp_address_type;
  char *a_rtcp_address;

  fsdp_framesize_t *a_framesize;


  char **unidentified_attributes;
  unsigned int unidentified_attributes_count;
  
  int config_size;
  unsigned  char *config_data;
  

};

typedef struct fsdp_media_description_t_s fsdp_media_announcement_t;


typedef struct
{
  unsigned long int duration;
  unsigned long int *offsets;
  unsigned long int interval;

  unsigned int offsets_count;
} fsdp_repeat_t;


typedef struct
{
  time_t start;
  time_t stop;

  unsigned int repeats_count;
    fsdp_repeat_t **repeats;
    
} fsdp_time_period_t;


struct fsdp_description_t_s
{

  unsigned int version;

  char *o_username;
  char *o_session_id;
  char *o_announcement_version;
  fsdp_network_type_t o_network_type;
  fsdp_address_type_t o_address_type;
  char *o_address;

  char *s_name;

  char *i_information;

  char *u_uri;

  const char **emails;
  unsigned int emails_count;

  const char **phones;
  unsigned int phones_count;

  fsdp_network_type_t c_network_type;
  fsdp_address_type_t c_address_type;
  fsdp_connection_address_t c_address;

  fsdp_bw_modifier_t *bw_modifiers;
  unsigned int bw_modifiers_count;

  fsdp_time_period_t **time_periods;
  unsigned int time_periods_count;

  char *timezone_adj;

  fsdp_encryption_method_t k_encryption_method;
  char *k_encryption_content;

  char *a_category;
  char *a_keywords;
  char *a_tool;
  char *a_range;

  fsdp_rtpmap_t **a_rtpmaps;
  unsigned int a_rtpmaps_count;
  fsdp_sendrecv_mode_t a_sendrecv_mode;
  fsdp_session_type_t a_type;
  char *a_charset;

  char **a_sdplangs;
  unsigned int a_sdplangs_count;
  char **a_langs;
  unsigned int a_langs_count;

  char **a_controls;
  unsigned int a_controls_count;

  fsdp_media_announcement_t **media_announcements;
  unsigned int media_announcements_count;

  char **unidentified_attributes;
  unsigned int unidentified_attributes_count;
};

#define MEDIA_RTPMAPS_MAX_COUNT 5
#define SDPLANGS_MAX_COUNT 5
#define SDPCONTROLS_MAX_COUNT 10
#define UNIDENTIFIED_ATTRIBUTES_MAX_COUNT 5

#endif
