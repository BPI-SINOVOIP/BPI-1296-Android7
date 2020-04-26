#ifndef __TRANSCODE_SERVER_PROTOCOL__
#define __TRANSCODE_SERVER_PROTOCOL__


#define CMD_DATA_CONNECT	"DataConnect\r\n"
#define CMD_DATA_CONNECT_RE	"DataConnect Return %d\r\n"
#define CMD_TRANS_START		"TransStart %c %s\r\n" // arg: %c - ('0': transfer, '1': transcode, 'S' : query size only), %s - source-path
#ifdef WIN32 
#define CMD_TRANS_START_RE	"TransStart Return %d %I64d\r\n" // arg: returnValue fileSize
#else
#define CMD_TRANS_START_RE	"TransStart Return %d %lld\r\n"
#endif

#define CMD_TRANS_STOP		"TransStop\r\n"
#define CMD_TRANS_STOP_RE	"TransStop Return %d\r\n"
#define CMD_READ_RE			"Read Return %d\r\n"
#define CMD_FLUSH			"Flush\r\n"
#ifdef WIN32 // arg: offset-in-bytes, size-in-bytes (-1 means until-eof)
  #define 	CMD_FLUSH_RE	"Flush Return %I64d\r\n"
#else
  #define 	CMD_FLUSH_RE	"Flush Return %lld\r\n"
#endif
#define CMD_EOS				"EOS\r\n"
#define CMD_ERROR			"Error %d\r\n"

#ifdef WIN32 // arg: offset-in-bytes, size-in-bytes (-1 means until-eof)
  #define CMD_READ		"Read %I64d %I64d\r\n"
#else
  #define CMD_READ		"Read %lld %lld\r\n"
#endif

#ifdef WIN32 // arg: seconds, frames, percentage, fps (int units of 0.01), encoded-bytes, streaming-position, file-size, file-size-unit
  #define CMD_STATUS	"Status %d %d %d %d %I64d %I64d %d %c\r\n"
#else
  #define CMD_STATUS	"Status %d %d %d %d %lld %lld %d %c\r\n"
#endif


// xxx_RE use '%d' as the error code, 0 means success, other value identifies error cause

enum TRANS_ERR_CODES
{
	TRANS_ERROR_NONE            = 0,
	TRANS_ERROR_INVALID_ARG     = 1,
	TRANS_ERROR_NO_CONNECTION   = 2,
	TRANS_ERROR_NOT_TRANSCODING = 3,
	TRANS_ERROR_ENCODER_ERROR   = 4,
	TRANS_ERROR_GENERIC         = 1000
};


#endif //__TRANSCODE_SERVER_PROTOCOL__
