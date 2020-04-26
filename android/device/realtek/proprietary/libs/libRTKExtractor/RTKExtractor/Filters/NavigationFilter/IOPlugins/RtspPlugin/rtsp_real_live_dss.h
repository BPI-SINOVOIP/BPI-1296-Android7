#ifndef __RTSP_REAL_LIVE_DSS_H__
#define __RTSP_REAL_LIVE_DSS_H__

struct packet_info
{
	int type;
	int64_t pts;
	int size;
	
};

struct rtp_rtsp_session_t {
  int rtp_socket;
  int rtcp_socket;
  char *control_url;
  int count;
};

struct rtpbits {
  unsigned int v:2;           /* version: 2 */
  unsigned int p:1;           /* is there padding appended: 0 */
  unsigned int x:1;           /* number of extension headers: 0 */
  unsigned int cc:4;          /* number of CSRC identifiers: 0 */
  unsigned int m:1;           /* marker: 0 */
  unsigned int pt:7;          /* payload type: 33 for MPEG2 TS - RFC 1890 */
  unsigned int sequence:16;   /* sequence number: random */
};


struct rtpheader {	/* in network byte order */
  struct rtpbits b;
  int64_t timestamp;	/* start: random */
  int ssrc;		/* random */
};


int get_avid(int pt);

int real_setup_and_get_header(struct stream_t *stream,
			      struct rmff_header_t **rmff_header_ret);
int real_rdt_get_media_packet(struct stream_t *stream,
			      uint8_t *buffer,size_t max_size);
int rtp_connect2 (char *hostname, int port);

int real_rtsp_options(struct stream_t *stream,struct rtsp_header_t **rtsp_hdr_ret);

int live_setup_and_get_header(struct stream_t *stream);

int live_server_rtp_get_media_packet(struct stream_t *stream,
			      uint8_t *buffer,size_t max_size);

//add by yunfeng_han
int live_rtsp_play(struct stream_t * stream,long long t_offset);
int live_rtsp_play2(struct stream_t * stream);
int live_rtsp_pause(struct stream_t * stream);
int real_rtsp_play(struct stream_t * stream,long long t_offset);
int real_rtsp_pause(struct stream_t * stream);

void rtcp_send_rr (struct stream_t *stream, struct rtp_rtsp_session_t *st);

void rtp_session_free (struct rtp_rtsp_session_t *st);
 int getrtp3(int fd, struct rtpheader *rh, unsigned char** data, int*  lengthData);

#endif
