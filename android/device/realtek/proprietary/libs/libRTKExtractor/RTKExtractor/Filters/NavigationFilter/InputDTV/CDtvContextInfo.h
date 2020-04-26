#ifndef __DTV_CONTEXT_INFO_H__
#define __DTV_CONTEXT_INFO_H__

#if IS_CHIP(MARS) || IS_CHIP(JUPITER) || IS_CHIP(SATURN)
	#define DTV_CONTEXT_TABLE_SIZE	1024
#else
	#define DTV_CONTEXT_TABLE_SIZE	512
#endif

typedef struct _tagDTV_CONTEXT_INFO {

	unsigned int readPos;
	int 		 readingTimeByPCR;
	

}DTV_CONTEXT_INFO;

typedef struct _tagDTV_CONTEXT_TABLE {
	
	DTV_CONTEXT_INFO m_table[DTV_CONTEXT_TABLE_SIZE];
	int m_start;
	int m_end;
	
	//context table for playback status
	void 	InitContextTable();
	int 	GetContext(unsigned int readPos, int readingTimeByPCR);
	bool	QueryContextInfo(int tableIndex, unsigned int* pReadPos, int* pReadingTimeByPCR);
	
	
} DTV_CONTEXT_TABLE;

#endif //#define __DTV_CONTEXT_INFO_H__
