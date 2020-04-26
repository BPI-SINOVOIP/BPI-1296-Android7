#ifndef __DUMMY_TS_PARSER_H
#define __DUMMY_TS_PARSER_H

typedef struct 
{    
    unsigned char tsp_error  : 1;
    unsigned char payload_start : 1;
    unsigned char reserved   : 6;
    unsigned char tsc : 4;
    unsigned char cc  : 4;
    
    unsigned short pid;
    struct {
        unsigned char  mode : 2;
        unsigned char* data;    
        unsigned char  len;        
    }adap;
    struct {
        unsigned char* data;    
        unsigned long  len;        
    }payload;    
}TSP_INFO;


typedef struct 
{    
    unsigned char       table_id;
    unsigned char       section_number;    
    unsigned char       last_section_number;
    union {
        unsigned short  transport_stream_id;
        unsigned short  program_number;
    };
    unsigned char       reserved : 2;    
    unsigned char       cur_next : 1;
    unsigned char       version  : 5;
    unsigned long       crc;            
    struct {
        unsigned char* data;    
        unsigned long  len;        
    }payload;
}SESSION_INFO;

#define MAX_PAT_PROG_CNT    8

typedef struct 
{    
    unsigned char prog_cnt;
        
    struct 
    {
        unsigned short number;
        unsigned short pid;            
    }prog[MAX_PAT_PROG_CNT];
       
}PAT_INFO;


#define MAX_PMT_STREAM_CNT    8

typedef struct 
{    
    unsigned short  pcr_pid;
    unsigned char*  prog_info;
    unsigned short  prog_info_len;
    unsigned char   stream_count;
    struct 
    {        
        unsigned char  type;
        unsigned short pid;        
        unsigned char* es_info;
        unsigned short es_info_len;        
    }stream[MAX_PMT_STREAM_CNT];
    
}PMT_INFO;


typedef struct 
{    
    unsigned char  stream_id;

#define STREAM_ID_PADDING_STREAM         0xBE    
#define STREAM_ID_PROGRAM_STREAM_MAP     0xBC
#define STREAM_ID_PRIVATE_STREAM_2       0xBF
#define STREAM_ID_ECM                    0xF0
#define STREAM_ID_EMM                    0xF1
#define STREAM_ID_DSMCC_STREAM           0xF2
#define STREAM_ID_PROGRAM_STREAM_DIRECTORY  0xFF
#define STREAM_ID_ITU_H221_TYPE_E        0xF8
    
    unsigned short packet_length;
         
    unsigned char psc           : 2;      // pes scramble control
    unsigned char pts_dts_flags : 2;                
    unsigned char escr_flag     : 1;
    unsigned char es_rate_flag  : 1;
    unsigned char dsm_trick_mode_flag : 1;
    unsigned char additional_copy_info_flag : 1;                            
    unsigned char pes_crc_flag  : 1;                            
    unsigned char pes_ext_flag  : 1;            
    unsigned char header_data_length;   
    
    unsigned long pts;
    
    struct {
        unsigned char* private_data;
        unsigned char flags;
        unsigned char* data;
    }ext;
    
    struct {
        unsigned char* data;
        unsigned short len;
    }payload;
    
}PES_INFO;

static int ts_parse_av_pid(unsigned char * buffer, int len, PMT_INFO*info);
static void ts_parse_init(void);

#endif
