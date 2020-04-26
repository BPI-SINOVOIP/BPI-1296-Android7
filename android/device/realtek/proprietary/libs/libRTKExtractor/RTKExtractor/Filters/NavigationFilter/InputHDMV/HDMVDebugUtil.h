#ifndef _HDMV_DEBUG_UTIL_H_
#define _HDMV_DEBUG_UTIL_H_

//__________________________________________________________________________
//
static inline void printf_off( const char *format, ... ) {} 


#define HDMV_TRACE 		printf//_off
#define HDMV_DEBUG		printf//_off
#define HDMV_INFO		printf//_off
#define HDMV_ERROR		printf//_off

#define HDMV_DETECT_MEM_LEAK	printf//_off

#define HDMV_MOD_TRACE			printf_off //HDMV_TRACE
#define HDMV_RENDERER			HDMV_TRACE
#define HDMV_STRM_CTRL			HDMV_TRACE
#define HDMV_CLIP				HDMV_TRACE
#define HDMV_PATH				HDMV_TRACE
#define HDMV_PL_MGR				HDMV_TRACE
#define HDMV_NAV				HDMV_TRACE
#define HDMV_PB_END_CB			HDMV_TRACE

#endif
