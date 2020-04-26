
#ifndef __INPUT_FILE_INDEX_OPEN_H__
#define __INPUT_FILE_INDEX_OPEN_H__

#ifdef __cplusplus
extern "C" {
#endif

int  FileIndexOpen(char *lpszDVDPath);
int  FileIndexClose();
int FileIndexGetTimePosition(uint64_t i64FrameNo, uint64_t *pStartByte, int *pGopNo, int *pOffsetFrameNo);

#ifdef __cplusplus
}
#endif 

#endif 


