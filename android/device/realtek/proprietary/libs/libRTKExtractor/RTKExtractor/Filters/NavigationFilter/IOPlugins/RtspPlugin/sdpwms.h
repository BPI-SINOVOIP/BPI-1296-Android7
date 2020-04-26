/***********************************************************************
 *    sdpwms.h: interpret wms protocol sdp
 ***********************************************************************/


#ifndef __SDPWMS_H__
#define __SDPWMS_H__


struct sdpwms_stream_t {

  char *id;
  int streamnum;
  char *control;

};



struct sdpwms_t {
  
  int asf_header_len;
  uint8_t *asf_header;  
  struct list_h *streams; /* sdpwms_stream_t list */
  
};


struct sdpwms_t *new_sdpwms_t(void);
void free_sdpwms_t(struct sdpwms_t *p);
struct sdpwms_t *wms_parse_sdp(char *data);


#endif /* __SDPWMS_H__ */

