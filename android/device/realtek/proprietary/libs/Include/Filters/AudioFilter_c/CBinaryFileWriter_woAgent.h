  
/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CBinaryFileWriter.h
//
//	@brief Definition for DVD-VENUS Binary File Writer Filter Class
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_BINARY_FILE_WRITER_H__
#define __C_BINARY_FILE_WRITER_H__

#include <CStream.h>
#include "CAudioOut.h"
#include "AudioType.h"

#ifdef DTS71_INTERLEAVE_2CH
#define FWRITE_OUTPUT_CHNUM 4
#elif defined DTS51_INTERLEAVE_2CH
#define FWRITE_OUTPUT_CHNUM 3
#else
#define FWRITE_OUTPUT_CHNUM 2
#endif

typedef enum
{
	FWRITE_CFG_INTERLEAVE,
	FWRITE_CFG_NONINTERLEAVE

} ENUM_FWRITE_CFG_INTERLEAVE;


typedef struct 
{
	ENUM_FWRITE_CFG_INTERLEAVE	interleave;
} FWRITE_CFG;

typedef struct 
{
	int					chnum;
	int					padzero;
	FWRITE_CFG			cfg;
} FWRITE_DATA;


class CThread;

class CBinaryFileWriter_woAgent : public CAudioOutFilter
{
public:

                                CBinaryFileWriter_woAgent(char* pFileName);
	virtual						~CBinaryFileWriter_woAgent(void);

	HRESULT						Pause(FILTER_STATE State);
	HRESULT						Stop();
	HRESULT						OpenFile(char*	pFileName);
	HRESULT						CloseFile();
	void 						SetDownmixMode(int mode);
	void						SetConfigure(FWRITE_CFG cfg)
								{	m_Data.cfg = cfg;		};
protected:
    // re-write this function for user's purpose

    static void					ThreadProcEntry(void* pInstance);

    void						ThreadProc();
	void						StartStreaming();
	void						StopStreaming();

protected:

	osal_thread_t				m_thread;

private:

	FWRITE_DATA					m_Data;
	FILE*						m_pFileHdlArr[FWRITE_OUTPUT_CHNUM];
    char*                       m_pFileName;
};


#endif
