#ifndef __RTSP_REAL_LIVE_DSS_H__
#define __RTSP_REAL_LIVE_DSS_H__

struct packet_info
{
	int type;
	int pts;
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
  int timestamp;	/* start: random */
  int ssrc;		/* random */
};



int rtp_rtp_connect2 (char *hostname, int port);

int rtp_get_header(struct stream_t *stream);

int rtp_server_rtp_get_media_packet(struct stream_t *stream,
			      uint8_t *buffer,size_t max_size);




void rtp_rtp_session_free (struct rtp_rtsp_session_t *st);
 int getrtp3(int fd, struct rtpheader *rh, unsigned char** data, int*  lengthData);

#endif
