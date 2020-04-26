#ifndef __WMSERVER_H__
#define __WMSERVER_H__

int wms_setup_and_get_header(struct stream_t *stream,
				  struct asf_headerinfo_t **asf_headerinfo_ret);
				  
int wms_rtp_get_media_packet(struct stream_t *stream,
				  uint8_t *buffer,size_t max_size);

//add by yunfeng_han
int wms_rtsp_play(struct stream_t *stream,long long t_offset);
int wms_rtsp_pause(struct stream_t *stream);

#endif
