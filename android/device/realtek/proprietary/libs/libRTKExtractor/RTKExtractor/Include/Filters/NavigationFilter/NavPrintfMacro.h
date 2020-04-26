#ifndef __NAV_PRINTF_MACRO_H__
#define __NAV_PRINTF_MACRO_H__

#include <sys/time.h>

#if defined(NAV_LOG_TO_FILE)
#define NAV_LOG_FILE			"/tmp/nav_log"
#define NAV_LOG_FILE_MAX_SIZE	(10*1024*1024)
extern FILE* g_pNavLogFd;
#endif

#if defined(ENABLE_NAV_LOG) || defined(USE_PCR_PACKET_TO_CHECK_LATENCY)

#if defined(NAV_LOG_TO_FILE)
#define NAV_TIME_PRINTF(type, format, args...) { \
	struct timeval curTime; \
	struct tm curTm; \
	gettimeofday(&curTime, NULL); \
	localtime_r((const time_t *)(&(curTime.tv_sec)), &curTm); \
	if (g_pNavLogFd != NULL) { \
		struct stat st; \
		if (stat(NAV_LOG_FILE, &st)==0) { \
			if (st.st_size >= NAV_LOG_FILE_MAX_SIZE) { \
				fclose(g_pNavLogFd); \
				g_pNavLogFd = NULL; \
			} \
		} \
		if (g_pNavLogFd != NULL) { \
			if (type==0) { \
				fprintf(g_pNavLogFd, "[%s:%d][%02d:%02d:%02d:%ldms].NAV+INFO.",__FILE__,__LINE__,curTm.tm_hour,curTm.tm_min,curTm.tm_sec,(curTime.tv_usec/1000)); \
			} else { \
				fprintf(g_pNavLogFd, "[%s:%d][%02d:%02d:%02d:%ldms].NAV+ERROR.",__FILE__,__LINE__,curTm.tm_hour,curTm.tm_min,curTm.tm_sec,(curTime.tv_usec/1000)); \
			} \
			fprintf(g_pNavLogFd, format, ##args); \
		} \
	} \
}

#define NAV_PRINTF(type, format, args...) { \
	if (g_pNavLogFd != NULL) { \
		struct stat st; \
		if (stat(NAV_LOG_FILE, &st)==0) { \
			if (st.st_size >= NAV_LOG_FILE_MAX_SIZE) { \
				fclose(g_pNavLogFd); \
				g_pNavLogFd = NULL; \
			} \
		} \
		if (g_pNavLogFd != NULL) { \
			if (type==0) { \
				fprintf(g_pNavLogFd, "[%s:%d].NAV+INFO.",__FILE__,__LINE__); \
			} else { \
				fprintf(g_pNavLogFd, "[%s:%d].NAV+ERROR.",__FILE__,__LINE__); \
			} \
			fprintf(g_pNavLogFd, format, ##args); \
		} \
	} \
}
#else	//	#if defined(NAV_LOG_TO_FILE)
#define NAV_TIME_PRINTF(type, format, args...) { \
	struct timeval curTime; \
	struct tm curTm; \
	gettimeofday(&curTime, NULL); \
	localtime_r((const time_t *)(&(curTime.tv_sec)), &curTm); \
	if (type==0) { \
		printf("[%s:%d][%02d:%02d:%02d:%ldms].NAV+INFO.",__FILE__,__LINE__,curTm.tm_hour,curTm.tm_min,curTm.tm_sec,(curTime.tv_usec/1000)); \
	} else { \
		printf("[%s:%d][%02d:%02d:%02d:%ldms].NAV+ERROR.",__FILE__,__LINE__,curTm.tm_hour,curTm.tm_min,curTm.tm_sec,(curTime.tv_usec/1000)); \
	} \
	printf(format, ##args); \
}

#define NAV_PRINTF(type, format, args...) { \
	if (type==0) { \
		printf("[%s:%d].NAV+INFO.",__FILE__,__LINE__); \
	} else { \
		printf("[%s:%d].NAV+ERROR.",__FILE__,__LINE__); \
	} \
	printf(format, ##args); \
}
#endif	//	#if defined(NAV_LOG_TO_FILE)

#else	//	#if defined(ENABLE_NAV_LOG) || defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
#define NAV_TIME_PRINTF(type, format, args...)
#define NAV_PRINTF(type, format, args...) 
#endif	//	#if defined(ENABLE_NAV_LOG) || defined(USE_PCR_PACKET_TO_CHECK_LATENCY)

#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)
#define SHARED_MEMORY_FRAME_START_RECEIVED_TIME_BLOCK_SIZE	4096
#define SHARED_MEMORY_FRAME_END_RECEIVED_TIME_BLOCK_SIZE	4096
#define MAX_SHARED_MEMORY_SIZE (SHARED_MEMORY_FRAME_START_RECEIVED_TIME_BLOCK_SIZE+SHARED_MEMORY_FRAME_END_RECEIVED_TIME_BLOCK_SIZE)
typedef struct {
	unsigned int hour;
	unsigned int min;	// minute
	unsigned int sec;	// second
	unsigned int ms;	// millisecond
	unsigned int rtp_seq;	
	unsigned int offset;	// offset in rtp packet
	int64_t	ptsAfterMemcpy;	// only use for demux video end packet
	int64_t ptsAfterSyncPtr;
	unsigned long phyWPAfterMemcpy;
	unsigned long phyWPAfterSync;
	unsigned int demuxVideoBytes;
	unsigned long long readCount;
	unsigned int readCountPtsDiffBig;
	int64_t ptsInStream;
} check_time;
typedef struct {
	bool isValid;
	//unsigned int rtp_seq;	
	unsigned int rtp_timestamp;
	//unsigned int offset;	// offset in rtp packet
	check_time arrive_time;
	//int64_t	ptsAfterMemcpy;	// only use for demux video end packet
	//int64_t ptsAfterSyncPtr;
	//unsigned long phyWPAfterMemcpy;
	//unsigned long phyWPAfterSync;
} packet_pos;
typedef enum {
	NAV_PTS_RECEIVED_TIME,
	NAV_FRAME_END_TIME,
	INPUT_PCR_RECEIVED_TIME,
	INPUT_FRAME_END_TIME
} TIME_SLOT_TYPE;

#if defined(ENABLE_MIRACAST_IO_PLUGIN)
#define MAX_TIME_SLOTS_IN_SHARED_MEMORY	(900)
#else
//	MAX_SHARED_MEMORY_SIZE/sizeof(packet_pos), the slots of PCR received time and video frame end packet received time must be the same
#define MAX_TIME_SLOTS_IN_SHARED_MEMORY	(MAX_SHARED_MEMORY_SIZE/sizeof(packet_pos))
#endif

#define CALCU_LATENCY_EVERY_FRAMES	(330)	// how many frames to log and calculate the latency every time
void SaveTimeToSlot(unsigned int slot, check_time* ckTimeBlock, TIME_SLOT_TYPE type, unsigned int rtp_seq, unsigned int offset, unsigned int demuxVideoBytes=0, unsigned long phyWPAfterMemcpy=0, int64_t ptsInStream=0);
void SaveVideoFrameEndReceivedTime(unsigned int slot, check_time* ckTimeBlock, packet_pos* pPos, TIME_SLOT_TYPE type);
bool IsMatchLatencyCheckCondition(int count, int criteria, unsigned int* pSlotNumber);
#endif	//	#if defined(USE_PCR_PACKET_TO_CHECK_LATENCY)

#if !defined(CONFIG_19)
#define NAV_TIME_PRINTF(type, format, args...)
#define NAV_PRINTF(type, format, args...)
#endif	//	#if !defined(CONFIG_19M)

#endif	//	#define __NAV_PRINTF_MACRO_H__
