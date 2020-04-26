  
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
	FWRITE_CFG			cfg;
} FWRITE_DATA;


class CThread;
class CBinaryFileWriterInPin;

class CBinaryFileWriter : public CAudioOutFilter
{

	friend class CBinaryFileWriterInPin;

public:

                                CBinaryFileWriter(char*	pFileName);
	virtual						~CBinaryFileWriter(void);

	HRESULT						Pause(FILTER_STATE State);
	HRESULT						Stop();

	HRESULT						OpenFile(char*	pFileName);
	HRESULT						CloseFile();
	void						SetConfigure(FWRITE_CFG cfg)
								{	m_Data.cfg = cfg;		};
protected:
    // re-write this function for user's purpose
    bool                        MainProcess(void);

private:
	void						ProcessPrivateInfo(BYTE* pInfo, int length);

private:

	FWRITE_DATA					m_Data;
	CBaseInputPin*				m_pCHPinArr[AUDIO_DEC_OUTPIN];
	FILE*						m_pFileHdlArr[AUDIO_DEC_OUTPIN];
	CMemRingAllocator*			m_pAllocatorArr[AUDIO_DEC_OUTPIN];
    FILE*                       m_pFileHandle;
    char*                       m_pFileName;

};


//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


class CBinaryFileWriterInPin: public CAudioOutMultiInPin
{
public:
	HRESULT                     PrivateInfo(BYTE* pInfo, int length);

};

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////


class CBaseBinaryFileWriter : public CAudioOutFilter
{

public:

    CBaseBinaryFileWriter(char*	pFileName);
	virtual						~CBaseBinaryFileWriter(void);

	HRESULT						Pause(FILTER_STATE State);
	HRESULT						Stop();

protected:
    // re-write this function for user's purpose
    bool                        MainProcess(void);

private:
    FILE*                       m_pFileHandle;
    char*                       m_pFileName;


};

#endif
