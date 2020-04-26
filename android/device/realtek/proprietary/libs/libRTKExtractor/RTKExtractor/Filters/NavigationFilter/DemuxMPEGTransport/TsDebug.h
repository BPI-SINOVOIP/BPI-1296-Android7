#ifndef TS_DEMUX_DEBUG_H
#define TS_DEMUX_DEBUG_H

extern long long gLastReadPacketSeq;
extern int g_offset_in_pDemuxIn;


#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
extern int g_received_pcr_packet_count;
extern int g_parse_video_frame_count;
extern int g_deliver_video_frame_count;
#endif	//	#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
#ifdef ENABLE_CHECK_CONTINUITY_COUNTER
extern unsigned char gLastReadPacketSync[16];
extern unsigned char gLastDeliverPacketSync[16];
extern unsigned char gLastDemuxOutBytes[7][16];
extern unsigned int gLastDemuxOutNumBuffers;
#endif	//	#define ENABLE_CHECK_CONTINUITY_COUNTER

#endif //TS_DEMUX_DEBUG_H
