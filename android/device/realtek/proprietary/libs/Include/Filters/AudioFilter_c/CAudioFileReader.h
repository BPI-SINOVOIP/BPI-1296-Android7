  

/**
/////////////////////////////////////////////////////////////////////////////////
//	@file CAudioFileReader.h
//
//	@brief Definition for DVD-VENUS Binary File Reader Filter Class
//
//	@author Taro     08/20/2004
//
//	copyright (c)RealTek.   All rights reserved.
/////////////////////////////////////////////////////////////////////////////////
*/

#ifndef __C_AUDIO_FILE_READER_H__
#define __C_AUDIO_FILE_READER_H__

#include <CStream.h>
#include <Util.h>
#include "CBinaryFileReader.h"

#define FILE_LIST_SIZE 512



class CAudioFileReader: public CAudioInFilter
{
	friend class				CAudioFileReaderOutPin;

public:

	struct BSFILE
	{
		char	mainBS[128];
		char	extBS[128];
		int		samplingRate;
		int		numberOfChannels;
		int		bitsPerSample;
	};

    CAudioFileReader(BSFILE* pFileList, int Num);
	virtual						~CAudioFileReader(void);

	HRESULT						Stop();

protected:
    // re-write this function for user's purpose
	bool                        MainProcess(void);
	HRESULT						OpenFile(char*	pFileName);
	HRESULT						CloseFile();
	HRESULT						OpenExtFile(char*	pFileName);
	HRESULT						CloseExtFile();

private:
    FILE*                       m_pFileHandle;
    FILE*                       m_pExtFileHandle;
	BSFILE                      m_FileList[FILE_LIST_SIZE];
	int							m_FileNum;

};



class CAudioFileReaderOutPin: public CAudioInOutputPin
{
public:
	CAudioFileReaderOutPin() {};
	virtual ~CAudioFileReaderOutPin() {};

};
#endif
