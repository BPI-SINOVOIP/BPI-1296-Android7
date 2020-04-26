#ifndef __C_BD_READER_H__
#define __C_BD_READER_H__

#include <Filters/NavigationFilter/NavPlugins.h>
#include "mpls_parse.h"
#include "clpi_parse.h"
#include "CBdParser.h"
#if defined(ENABLE_BLURAY_3D)
#include "CBdSubPath.h"
#endif


#define USE_DIRECT_IO

#define DEFAULT_READ_SIZE		(96 * 1024)
#define PACKET_SIZE				192
#ifdef USE_DIRECT_IO
#define TIMECODE_OFFSET			0
#else
#define TIMECODE_OFFSET			4
#endif

class CInputBD ; 

class CBdReader
{
	friend class CInputBD;

#define	MAX_FILES_NUM	1024
	
enum { 
	MAX_TOTAL_LENGTH		= 1 << 0,
	REMOVE_DUP_PLAYITEM		= 1 << 1,
};	
	
enum {
	STATUS_OPENED           			= 1 << 0,	
};
	
public:
	
	CBdReader();
	~CBdReader();
	
	HRESULT	Open(char* path, osal_mutex_t* pNavMutex);
	HRESULT Close();
	
	HRESULT FindMoviePlayList(unsigned int flag, int* pPlaylistNum, NAVBDTITLELIST** ppMPLSList, int* pIdx);
	
	HRESULT LoadMoviePlayList(int mplsId, int mplsItemNo);
	
	// to notify the change of speed and do some preparations inside this class (return true if speed changed)
    bool 	SetSpeed(int speed, int presentSec);
    
    // read up to 'maxSize' bytes from file
    int Read(unsigned char* pBuffer, int maxSize, NAVBUF* pEventReceived);
    
    // reset all positions
    void ResetPosition();
    
    bool GotoTimePosition(int64_t second);
    
    // update the read-position/scanning-step-begin/scanning-step-limit for the next scanning step (return false means it reaches end-of-scanning)
    bool GotoNextScanningStep();
    
    int GetTotalSeconds();
    
    int64_t GetTimePosition(int64_t time);
    
    bool NewPosition();
    
    // update the mark(chapter) number during playback; pts in 45kHz
    void UpdateMarkNo(uint16_t playItemNo, int64_t pts);	
    
    // crate signature
    HRESULT ReadMediaSignature(uint32_t *mediaSignature);
	
    void GotoOldestPosition();
	
	void SetInputBDPtr(CInputBD* pInputBD) { m_pInputBD = pInputBD; }
		
#if defined(ENABLE_BLURAY_3D) 	
	//For MVC Subpath
	inline bool HasSubPathEvents() { return (m_bdSubPath->m_events > 0); }
	inline bool HandleSubPathEvents(NAVBUF* pBuffer) { return m_bdSubPath->HandleEvents(pBuffer); }
	inline bool FlushSubPath() { return m_bdSubPath->OnFlush();}
		   bool ReadSubPath(NAVBUF* pBuffer, bool bMainPathFull);
	inline bool IsSubPathLoad() { return m_bdSubPath->IsLoad(); }
	inline bool SubPathReachScanLimit() 
			{
				if(!m_bdSubPath->IsLoad()) return true;
				else	return (m_bdSubPath->m_speed != 256 && m_bdSubPath->m_readSPN >= m_bdSubPath->m_scanLimitSPN);
			}
	inline bool StopSubPath(){m_bdSubPath->OnStop(); return true;}
	inline bool GetOutputMode() { return m_bdSubPath->m_3DOutput;}
	inline void SetOutputMode(bool bEnable) { m_bdSubPath->m_3DOutput = bEnable;}
	inline bool UpdateSubPathPresentPos(int64_t pts) { return m_bdSubPath->UpdatePresentSPN(pts);}
#endif
		
protected:
	
	bool		get_pts_spn(int64_t pts, uint32_t *pSpn);	// get epn of the pts
	
	
private:
	
	unsigned int	m_status;
	
	int				m_mplsNum;
	int				m_mplsIdList[MAX_FILES_NUM];
	int				m_clpiNum;
	int				m_clpiIdList[MAX_FILES_NUM];	
	
	// playback status.
	int				m_mplsId;			// playing playlist index
	bdt_pl_t		m_mplsInfo;			// playing playlist information
	bdt_cl_t		m_clpiInfo;
	int				m_mplsItemNo;
	int				m_mplsMarkNo;
	int				m_actAudStrm;		// active audio stream number
	int				m_actPGStrm;		// active presentation graphics stream number
	int				m_fd;				// file handle of playing m2ts file
	int64_t			m_readPos;
	int64_t	        m_presentPos;        // presentation position (where audio/video is presented at)
	int				m_presentItemNo;
	
	int             m_speed;             // 256: normal playback; 257+: I-scanning; Negative: reverse I-scanning
	int             m_PCRPID;
    int64_t    		m_scanningStepBegin; // where we start feeding data for current scanning step
    int64_t    		m_scanningStepLimit; // where current scanning step should never go beyond (if no limit, use 0xFFFFFFFF)
    int64_t    		m_scanningStepSec;   // elapsed second of current scanning step
    int64_t    		m_scanningPosStride; // position difference between scan steps
    int64_t    		m_scanningSecStride; // time difference between scan steps in seconds	
	int64_t		    m_stateSegBegin;     // where the current state segment begins
    int64_t	        m_stateSegEnd;       // where the current state segment ends (for open segment, use 0xFFFFFFFF)      
	
	osal_mutex_t*   m_pNavMutex;
	
	CInputBD*		m_pInputBD;
	
	CBdParser		m_bdParser;
		
#if defined(ENABLE_BLURAY_3D)	
	CBdSubPath*		m_bdSubPath;
	EPMap			m_epMap;
	int				m_readCount;
#endif	
};

#endif // __C_BD_READER_H__
