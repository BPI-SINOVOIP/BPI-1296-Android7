#ifndef __MIRACAST_IO_DEBUG_H__
#define __MIRACAST_IO_DEBUG_H__

//#define ENABLE_DUMP_TS_FILE
//#define ENABLE_DUMP_TS_DISTRIBUTION

#ifdef ENABLE_DUMP_TS_DISTRIBUTION
void writeLineToTsDistFile(unsigned char* content, unsigned int size);
void openDumpTsDistFile(void);
void closeDumpTsDistFile(void);
void writeTsDistToFile(unsigned char* pTsPkts, unsigned int tsPktsBytes, unsigned short seq, unsigned char marker);
#endif	//	#ifdef ENABLE_DUMP_TS_DISTRIBUTION

#ifdef ENABLE_DUMP_TS_FILE
void openDumpTsFile(void);
void closeDumpTsFile(void);
void writeTsPktsToFile(unsigned char* pTsPkts, unsigned int tsPktsBytes);
#endif	//	#ifdef ENABLE_DUMP_TS_FILE

#endif	//	#define __MIRACAST_IO_DEBUG_H__
