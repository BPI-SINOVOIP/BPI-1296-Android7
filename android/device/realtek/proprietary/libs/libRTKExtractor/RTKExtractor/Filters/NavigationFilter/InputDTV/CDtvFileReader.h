#ifndef __C_DTV_FILE_READER_H__
#define __C_DTV_FILE_READER_H__



#include <Filters/NavigationFilter/NavPlugins.h>
#include <Filters/NavigationFilter/SiEngine.h>
#include <Filters/GeneralIFO.h>

#include "CDtvContextInfo.h"
#include "CDtvFileEditor.h"
#include "HLS.h"

#include <utils/Log.h>
#undef LOG_TAG
#define LOG_TAG "CDtvFileReader"
//#define ENABLE_DEBUG
#ifdef ENABLE_DEBUG
#define printf(fmt, args...) ALOGE("%s(%d): " fmt, __FUNCTION__, __LINE__, ##args)
#else /* else of ENABLE_DEBUG */
#define printf(fmt, args...)
#endif /* end of ENABLE_DEBUG */

#ifndef DONTFIXBUG16051
	#define IS_SMOOTH_TRICK_PLAY(_p)	(AP_CONFIG_WITH(APCFG_SMOOTH_TRICK_PLAY) && ((_p)->m_speed > 0))
#else
	#define IS_SMOOTH_TRICK_PLAY(_p) 	(AP_CONFIG_WITH(APCFG_SMOOTH_TRICK_PLAY))
#endif

class CDtvFileReader : public CDtvFileEditor
{
    friend class CInputDTV;

protected:

    enum { IDLE, PLAY_WITH_IFO, PLAY_WITH_IFO_SHARED, PLAY_WITHOUT_IFO, PLAY_HLS } m_mode;

    IOPLUGIN*          m_pIOPlugin;
    void*              m_pFileHandle;
    void*              m_pIfoContext;
    osal_mutex_t*      m_pNavMutex;
    osal_mutex_t       m_ifoMutex;
    unsigned int       m_readPos;           // read position (where to read data from)
    unsigned int       m_readPosTime;       // read position in time (what time to read data from. in seconds)
    unsigned int       m_presentPos;        // presentation position (where audio/video is presented at)
    unsigned int       m_recordPos;         // record position (where recording is to write at, use 0xFFFFFFFF if no recording)
    unsigned int       m_scanningStepBegin; // where we start feeding data for current scanning step
    unsigned int       m_scanningStepLimit; // where current scanning step should never go beyond (if no limit, use 0xFFFFFFFF)
    unsigned int       m_scanningStepSec;   // elapsed second of current scanning step
    unsigned int       m_scanningPosStride; // position difference between scan steps
    unsigned int       m_scanningSecStride; // time difference between scan steps in seconds
    unsigned int       m_stateSegBegin;     // where the current state segment begins
    unsigned int       m_stateSegEnd;       // where the current state segment ends (for open segment, use 0xFFFFFFFF)    
    int                m_speed;             // 256: normal playback; 257+: I-scanning; Negative: reverse I-scanning
    unsigned int       m_byteRate;          // estimated byte rate
    unsigned int       m_totalSeconds;
    int64_t            m_totalBytes;
    int                m_preambleLength;
    int                m_PCRPID;
    int64_t            m_beginPCR;          // the first PCR in the file
    int                m_presentTimeByPCR;  // when m_PCRPID != 0x1fff, calculate elapsed time by PCR
    int                m_readingTimeByPCR;  // when m_PCRPID != 0x1fff, calculate elapsed time by PCR

	int				   m_gIFOUnitSizeExp;	//for bug 20671
	
	DTV_CONTEXT_TABLE  m_contextTable;
	HLS				   m_hls;
	
	int                m_readCount;
	bool               m_timeSeekable;
	bool               m_httpIO;
	bool               m_bStreaming; 		// means the file may getting bigger
	
	//For AB Repeat
	int                m_segASec;
	int64_t            m_segBPos;
public:

    CDtvFileReader(IOPLUGIN* pIOPlugin, void* pFileHandle, osal_mutex_t* pNavMutex)
        : m_mode(IDLE)
        , m_pIOPlugin(pIOPlugin)
        , m_pFileHandle(pFileHandle)
        , m_pNavMutex(pNavMutex)
        //for those files without IFO or for the case that flag FIXBUG20671 not defined
        //use CGeneralIFO::UNIT_SIZE_EXP
        , m_gIFOUnitSizeExp(CGeneralIFO::UNIT_SIZE_EXP) 
	{
		m_totalSeconds = 0;
		m_timeSeekable = false;
		m_bStreaming = false;
	#ifndef DONTFIXBUG13634	
		m_contextTable.InitContextTable();
	#endif	
	}

	~CDtvFileReader() { Release(); }
	
    // attach to shared IFO exposed by recording (return true if succeeded, false if failed)
    bool LoadSharedIFO(NAVIOINFO* pIOInfo);

    // load IFO from file (return true if succeeded, false if failed)
    bool LoadIFO(NAVIOINFO* pIOInfo, unsigned char* pBuffer, int size);

    // load based on estimated byte-rate (return true if succeeded, false if failed)
    bool LoadByteRate(NAVIOINFO* pIOInfo, unsigned int byteRate);

    // release a loaded file when we're done with it
    void Release();

    // read up to 'maxSize' bytes from file
    int Read(unsigned char* pBuffer, int maxSize, NAVBUF* pEventReceived);

    // update the latest record position (from shared information)
    void UpdateRecordPosition();

    // get present playback elapsed-time (in seconds) based on the present file position
    int GetPresentElapsedSeconds();

    // get total playback time (in seconds) (for live-pause, it's the elapsed-time of the newest content)
    int GetTotalSeconds();

    // update state segment range and feedback new state object corresponding to the current read position
	void UpdateStateSegment(void (*stateFeedback)(void* instance, CGeneralIFO::STATE_TYPE type, void* state, int bytes), void* pInstance);

    // update the read-position on read error (skip forward/backward depending on speed), return false if it reaches end of file
    bool SkipOnReadError();

    // update the read-position/scanning-step-begin/scanning-step-limit for the next scanning step (return false means it reaches end-of-scanning)
    bool GotoNextScanningStep();

    // move read-position to the content corresponding to the elapsed-time (in seconds) (return false means the destination is invalid)
    bool GotoTimePosition(int second);

    // move read-position to the oldest content (for non-live-pause, it's at the beginning of file, file position is 0)
    void GotoOldestPosition();

    // move read-position to the newest content (for non-live-pause, it's at EOF, file position 0xFFFFFFFF is used)
    bool GotoNewestPosition();

    // to notify the change of speed and do some preparations inside this class (return true if speed changed)
    bool SetSpeed(int speed);

    // reset all positions
    void ResetPosition();
    
    // get recording status
    HRESULT GetRecordStatus(NAVRECORDSTATUS* pStatus);
    
    // get the first entry of the state table
    HRESULT GetSIState(int bytes, unsigned char* pState, unsigned int* pReturnedSize);
    
    //for nonseekable IO to rewind file position
	void* Rewind(char* path);	
	
	HRESULT GetTimePosition(unsigned int second, int64_t* pByteOffset);
	
	int inline GetContext()
	{
		if(m_mode == PLAY_HLS)
		{
			return m_hls.getContext();
		}
		else
		{
			if(m_speed != 256)	//scanning
	    	{
				if (IS_SMOOTH_TRICK_PLAY(this)) 
					return m_readPos;
				else 
					return m_scanningStepBegin;
			}
			else
			{
		    	if(m_PCRPID == 0x1fff)
		       		return m_readPos;
		    	else
		    	{
		    	#ifndef DONTFIXBUG13634	
		    		return m_contextTable.GetContext(m_readPos, m_readingTimeByPCR);
				#else                	
		        	return ((m_readPos & 0xFFFFFF80) << 9) | (m_readingTimeByPCR & 0x0000FFFF);
				#endif                    
				}
			}
		}
	}
	
	inline void UpdateContext(int context)
	{
		if(m_mode == PLAY_HLS)
		{
			m_hls.m_presentTime = context;
			m_presentPos = context;
		}
		else
		{
			if(m_PCRPID == 0x1fff || m_speed != 256)
            	m_presentPos = context;
        	else
        	{
         
			#ifndef DONTFIXBUG13634	            
        	    m_contextTable.QueryContextInfo(context, &m_presentPos, &m_presentTimeByPCR);
    		#else
       			m_presentPos = (context >> 9) & 0xFFFFFF80;
            	m_presentTimeByPCR = (context & 0x0000FFFF);            
    		#endif
        	}
		}
	}
	inline bool Seek(void*& fileHandler)
	{
		printf("m_mode[%d] PLAY_HLS[%d] m_timeSeekable [%d] seekByTime_function[%p] --------\n", m_mode, PLAY_HLS, m_timeSeekable, m_pIOPlugin->seekByTime);
		if(m_mode == PLAY_HLS)
		{
			return m_hls.execTimeSeek(m_pIOPlugin, m_pFileHandle);
		}
		else if(m_timeSeekable && m_pIOPlugin->seekByTime)		
		{
	    	long long seekRes =  m_pIOPlugin->seekByTime(m_pFileHandle, m_readPosTime);
			printf("@@@@@ edPosTime [%d] seekRes[%lld] ====\n", m_readPosTime, seekRes);
			return (seekRes >= 0);
		}
		else
		{
			//Seek(0) is a special case for nonseekable io.
			
	    	long long seekRes =  m_pIOPlugin->seek(m_pFileHandle,
	                      		(((int64_t)m_readPos) << m_gIFOUnitSizeExp) + m_preambleLength,
	           	          		SEEK_SET);
			printf("$$$$$ m_readPos [%d] m_gIFOUnitSizeExp [%d] m_preambleLength[%d] seekRes [%lld] =====\n", m_readPos, m_gIFOUnitSizeExp, m_preambleLength, seekRes);
			
			//In case io plugin doesn't handle seek(0),
			//we do it ourself.
			if(seekRes < 0 && m_readPos == 0 && m_pSrcPathName)
			{
				m_pIOPlugin->close(m_pFileHandle);
				fileHandler = m_pFileHandle = m_pIOPlugin->open(m_pSrcPathName, 0);
				return (m_pFileHandle != NULL);
			}
			return (seekRes >= 0);
			
		}
		
	}
};


#endif /*__C_DTV_FILE_READER_H__*/
