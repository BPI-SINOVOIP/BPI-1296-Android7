#ifndef __RTSP_RM_H__
#define __RTSP_RM_H__

#include <inttypes.h>


enum {
    PN_SAVE_ENABLED         = 0x01,

    PN_LIVE_BROADCAST       = 0x04,

        PN_PERFECT_PLAY_ENABLED = 0x02,
} prop_flags;



enum {

    DATA_TAG = ('D'<<24) | ('A'<<16) | ('T'<<8) | 'A',
    INDX_TAG = ('I'<<24) | ('N'<<16) | ('D'<<8) | 'X',
    PNA_TAG  = ('P'<<24) | ('N'<<16) | ('A'<<8) |  0 ,
    MLTI_TAG = ('M'<<24) | ('L'<<16) | ('T'<<8) | 'I',
        RMF_TAG  = ('.'<<24) | ('R'<<16) | ('M'<<8) | 'F',
    PROP_TAG = ('P'<<24) | ('R'<<16) | ('O'<<8) | 'P',
    MDPR_TAG = ('M'<<24) | ('D'<<16) | ('P'<<8) | 'R',
    CONT_TAG = ('C'<<24) | ('O'<<16) | ('N'<<8) | 'T',
};


struct rmff_pheader_t {

    uint16_t object_version;

    uint16_t length;
    uint16_t stream_number;
    uint32_t timestamp;
    uint8_t reserved;
    uint8_t flags;
};

void rmff_build_pheader(struct rmff_pheader_t *ph,uint8_t *data);
int rmff_read_pheader(uint8_t *data,struct rmff_pheader_t *ph);

struct rmff_header_t {


    struct rmff_cont_t *cont;
    
    struct rmff_data_t *data;
    struct rmff_fileheader_t *fileheader;
    struct rmff_prop_t *prop;
    struct rmff_mdpr_t **streams;    
}; 


void rmff_fill_header(struct rmff_header_t *h);

struct rmff_data_t {
  
    uint32_t object_id;
    uint32_t size;
    uint16_t object_version;

    uint32_t num_packets;
    uint32_t next_data_header; /* rarely used */
};




struct rmff_prop_t {

    uint32_t object_id;
    uint32_t size;
    uint16_t object_version;
  


    uint32_t max_bit_rate;
    uint32_t avg_bit_rate;
    uint32_t max_packet_size;
    uint32_t duration;
    uint32_t preroll;
    uint32_t index_offset;
    uint32_t data_offset;    
    uint32_t avg_packet_size;
    uint32_t num_packets;

    uint16_t num_streams;
    uint16_t flags;
};




struct rmff_mdpr_t {
  
    uint32_t  object_id;
    uint32_t  size;
    uint16_t  object_version;

    uint32_t  start_time;
    uint32_t  preroll;
    uint32_t  duration;
    uint8_t   stream_name_size;
    char      *stream_name;
    uint8_t   mime_type_size;
    char      *mime_type;
    uint32_t  type_specific_len;
    char      *type_specific_data;

    uint16_t  stream_number;
    uint32_t  max_bit_rate;
    uint32_t  avg_bit_rate;
    uint32_t  max_packet_size;
    uint32_t  avg_packet_size;


    int       mlti_data_size;
    char      *mlti_data;
};

struct rmff_cont_t {

    uint32_t  object_id;
    uint32_t  size;
    uint16_t  object_version;

    uint16_t  copyright_len;
    char      *copyright;

    uint16_t  title_len;
    char      *title;
    uint16_t  author_len;
    char      *author;    
    uint16_t  comment_len;
    char      *comment;
};

struct rmff_fileheader_t {



    uint32_t file_version;
    uint32_t num_headers;
        uint32_t object_id;
    uint32_t size;
    uint16_t object_version;
};



				  
struct rmff_mdpr_t *new_rmff_mdpr(uint16_t   stream_number,
				  uint32_t   max_bit_rate,
				  uint32_t   avg_bit_rate,
				  uint32_t   max_packet_size,
				  uint32_t   avg_packet_size,
				  uint32_t   startTime,
				  uint32_t   preroll,
				  uint32_t   duration,
				  const char *streamName,
				  const char *mime_type,
				  uint32_t   type_specific_len,
				  const uint8_t *type_specific_data);

struct rmff_fileheader_t *new_rmff_fileheader(uint32_t headNums);

struct rmff_header_t *new_rmff_header_t(void);
void free_rmff_header_t(struct rmff_header_t *h);

int rmff_build_header(struct rmff_header_t *h, uint8_t *buffer);
struct rmff_data_t *new_rmff_dataheader(uint32_t num_packets,
					uint32_t next_data_header);



struct rmff_prop_t *new_rmff_prop (uint32_t max_bit_rate,
				   uint32_t avg_bit_rate,
				   uint32_t max_packet_size,
				   uint32_t avg_packet_size,
				   uint32_t num_packets,
				   uint32_t duration,
				   uint32_t preroll,
				   uint32_t index_offset,
				   uint32_t data_offset,
				   uint16_t streamNums,
				   uint16_t flags );



struct rmff_cont_t *new_rmff_cont(const char *title,
				  const char *author,
				  const char *copyright,
				  const char *comment);

  

void rmff_print_header(struct rmff_header_t *h);

#endif

