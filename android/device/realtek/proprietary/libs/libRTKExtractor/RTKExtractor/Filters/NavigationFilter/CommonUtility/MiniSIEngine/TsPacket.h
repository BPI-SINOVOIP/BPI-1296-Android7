#ifndef __MINI_SIENGINE_TS_PACKET_H__
#define __MINI_SIENGINE_TS_PACKET_H__


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
        const unsigned char* data;    
        unsigned char  len;        
    }adap;
    struct {
        const unsigned char* data;    
        unsigned long  len;        
    }payload;    
}TS_PACKET_INFO;


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
        const unsigned char* data;    
        unsigned long  len;        
    }payload;
}TS_SESSION_INFO;

#define MAX_PAT_PROG_CNT    8

typedef struct 
{    
    unsigned char prog_cnt;
        
    struct 
    {
        unsigned short number;
        unsigned short pid;            
    }prog[MAX_PAT_PROG_CNT];
       
}PSI_PAT;


#define MAX_PMT_STREAM_CNT    8

typedef struct 
{    
    unsigned short  pcr_pid;
    const unsigned char*  prog_info;
    unsigned short  prog_info_len;
    unsigned char   stream_count;
    struct 
    {        
        unsigned char  type;
        unsigned short pid;        
        const unsigned char* es_info;
        unsigned short es_info_len;        
    }stream[MAX_PMT_STREAM_CNT];
    
}PSI_PMT;



/*
#define STREAM_TYPE_VIDEO_ISO_IEC_11172_2  0x01        // MPEG1
#define STREAM_TYPE_VIDEO_ISO_IEC_13818_2  0x02        // MPEG2
#define STREAM_TYPE_AUDIO_ISO_IEC_11172_3  0x03
#define STREAM_TYPE_AUDIO_ISO_IEC_13818_3  0x04
#define STREAM_TYPE_AUDIO_ISO_IEC_13818_7  0x0F
#define STREAM_TYPE_AUDIO_ISO_IEC_14496_3  0x11
#define STREAM_TYPE_VIDEO_H264_AVC         0x1B        // H264

*/
#endif  // __MINI_SIENGINE_TS_PACKET_H__
