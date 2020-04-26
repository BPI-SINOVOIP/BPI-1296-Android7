#ifndef __SDPREAL_H__
#define __SDPREAL_H__


struct sdpreal_t {
  
    char *title;
    char *author;

    char *abstract;
    int flags;
    int stream_count;
    char *copyright;
    char *keywords;  

    struct list_h *streams; 
  
};

struct sdpreal_stream_t {



    int stream_id;
  
    int duration;
    int preroll;
    int start_time;

    char *id;
    char *bandwidth;

  
    char *stream_name;
    int stream_name_size;
    char *mime_type;
    int mime_type_size;

    
    int max_bit_rate;
    int avg_bit_rate;
    int max_packet_size;
    int avg_packet_size;
    
    uint8_t *mlti_data;
    int mlti_data_size;
    char *asm_rule_book;
    int asm_rule_book_size;

};




struct sdpreal_t *sdpreal_parse(char *sdpDat);
void free_sdpreal_t(struct sdpreal_t *sdpVal);


#endif 
