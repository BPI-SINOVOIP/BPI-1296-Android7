  
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
	int					samplebit;
	FWRITE_CFG			cfg;
} FWRITE_DATA;


class CBinaryFileWriter : public CAudioOutFilter
{
public:

                                CBinaryFileWriter(char*	pFileName);
	virtual						~CBinaryFileWriter(void);

	HRESULT						OpenFile(char*	pFileName);
	HRESULT						CloseFile();
	void						SetConfigure(FWRITE_CFG cfg)
								{	m_Data.cfg = cfg;		};
protected:

private:

private:

	FWRITE_DATA					m_Data;
    FILE*                       m_pFileHandle;
    char*                       m_pFileName;

};


#endif
