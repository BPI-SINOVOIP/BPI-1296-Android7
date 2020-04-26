#ifndef INPUTSOCKET_TS_DEBUG_H
#define INPUTSOCKET_TS_DEBUG_H

extern long long gLastReadPacketSeq;
#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
extern int g_received_pcr_packet_count;
extern int g_parse_video_frame_count;
extern int g_deliver_video_frame_count;
extern int g_nav_video_frame_end_packet_count;

extern int g_received_video_frame_end_packet_count;
extern check_time g_input_socket_check_time_block[MAX_TIME_SLOTS_IN_SHARED_MEMORY];
extern check_time g_input_socket_frame_end_time_block[MAX_TIME_SLOTS_IN_SHARED_MEMORY];
extern packet_pos g_possible_video_frame_end_packet_pos;
extern packet_pos g_video_frame_end_packet_pos;
#endif	//	#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
#ifdef ENABLE_CHECK_CONTINUITY_COUNTER
extern unsigned char gLastReadPacketSync[16];
#endif
#endif //INPUTSOCKET_TS_DEBUG_H
