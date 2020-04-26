#ifndef __FILELINK_INTERNALS_HEADER_FILE__
#define __FILELINK_INTERNALS_HEADER_FILE__

#ifdef __cplusplus
extern "C" {
#endif

#define DIRECT_IO_ALIGN_UNIT	0x0000000000000800LL
#define DIRECT_IO_ALIGN_MASK	0xfffffffffffff800LL
#define DIRECT_IO_UNIT_SIZE		(256*1024)

/* plugin instance object */

typedef struct _tagIOPlugin_FileLink
{
	int fileCount;

	int *fh; // file handler array
	char **localPath;
	off_t *fileLength; // file size array := this is an accumulated value
	off_t totalLength;
	int64_t totalPTS;

	int currFileNo;
	off_t currOffset; 	// what upper layer thinks we are at
	off_t ioOffset; 	// real position we are at
	int b_direct_io;

	unsigned char *cacheBuffer;
	off_t cacheBufferStart;
	off_t cacheBufferSize;

} IOPlugin_FileLink;

int filelink_parse(void *pInstance, char *path, int b_direct_io);

#ifdef __cplusplus
}
#endif

#endif
