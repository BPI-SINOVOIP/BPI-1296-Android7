#ifndef __C_DTV_HLS_READER_H__
#define __C_DTV_HLS_READER_H__

class HLS
{
	friend class CDtvFileReader;
	
protected :
	int m_presentTime;	//in sec
	int m_seekToTime;	//in milli sec
	int m_baseTime;		//in milli sec
	int m_byteRate;		// bytes per second for a specific clip
	int m_clipDuration;	//in milli sec
	int64_t m_clipSize;
	int64_t m_readPosInClip;
	
public:	
	HLS() 
	: m_presentTime(0)
	, m_seekToTime(-1)
	, m_baseTime(0)
	, m_byteRate(0)
	, m_clipDuration(-1)
	, m_clipSize(0)
	, m_readPosInClip(0)
	{
	}
	
	~HLS() {}
	
	inline bool execTimeSeek(IOPLUGIN* pIOPlugin, void* pFileHandle)
	{
		if(m_seekToTime < 0)
			return false;
		int64_t baseTimePos = pIOPlugin->seekByTime(pFileHandle, m_seekToTime);
    	if(baseTimePos < 0)
    		return false;
    	
    	m_baseTime = baseTimePos;
    	
    	bool res = false;
    	NAVIOCLIPINFO clipInfo;
    	if(pIOPlugin->getClipInfo(pFileHandle, m_seekToTime, &clipInfo))
    	{
    		m_clipSize = clipInfo.clipSize;
    		m_clipDuration = clipInfo.clipDuration;
			if(m_clipSize != -1)
			{
				m_byteRate = m_clipSize*1000 / m_clipDuration;
			
			}

			if(m_byteRate!= 0)
			{
	    		m_readPosInClip = (int64_t)(m_seekToTime-m_baseTime)*m_byteRate/1000;
	    		res = (pIOPlugin->seek(pFileHandle, m_readPosInClip, SEEK_CUR) >= 0);
			}
	   	}

		if(res)
			m_presentTime = m_seekToTime / 1000;
		else
		{
			m_presentTime = m_baseTime / 1000;
			m_readPosInClip = 0;
		}	
		m_seekToTime = -1;
		return true;
    	
	}
	
	inline void updateReadPositionInClip(IOPLUGIN* pIOPlugin, void* pFileHandle, int readSize)
	{
		if(readSize <= 0)
			return;
			
		m_readPosInClip += readSize;
		// if we adopt == in this case, getClipInfo command would prior to file open for next clip.
		// it would influence current cache mechanism.

		if(m_clipSize == -1)
		{
			NAVIOCLIPINFO clipInfo;
			if(pIOPlugin->getClipInfo(pFileHandle, m_baseTime+1, &clipInfo))
			{
				m_clipSize = clipInfo.clipSize;
				m_clipDuration = clipInfo.clipDuration;
				if(m_clipSize != -1)
				{
					m_byteRate = m_clipSize*1000 / m_clipDuration;
				}
			}
		}
		else
		{
			if(m_readPosInClip > m_clipSize)
			{
				m_baseTime += m_clipDuration;
				m_readPosInClip -= m_clipSize;
				
				NAVIOCLIPINFO clipInfo;
				if(pIOPlugin->getClipInfo(pFileHandle, m_baseTime+1, &clipInfo))
				{
	    			m_clipSize = clipInfo.clipSize;
	    			m_clipDuration = clipInfo.clipDuration;
	    	
					if(m_clipSize != -1)
					{
						m_byteRate = m_clipSize*1000 / m_clipDuration;
					}
	    		}
			}
		}
	}
	
	inline int getContext()
	{
		return (m_byteRate == 0) ? m_baseTime/1000 : (m_baseTime/1000 + ((m_clipDuration/1000 > (m_readPosInClip / m_byteRate)) ? m_readPosInClip / m_byteRate:m_clipDuration/1000));
	}
} ;

#endif
