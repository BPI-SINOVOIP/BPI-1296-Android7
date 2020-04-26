#ifndef _VCD_CORE_H
#define _VCD_CORE_H

#include "CLowLevelTrans.h"

#define VCD_MAX_PATH_LEN		1024

#define VCD_EMPTY_ARRAY_SIZE	0
#define VCD_MAX_SEGMENTS		1980
#define VCD_MAX_ENTRIES			500
#define VCD_SEGMENT_SECTOR_SIZE 150

#define VCD_LID_MASK				0x7FFF

#define PSD_OFFSET_DISABLED			0xFFFF
#define PSD_OFFSET_MULTI_DEF		0xFFFE
#define PSD_OFFSET_MULTI_DEF_NO_NUM	0xFFFD

#define VCD_INVALID_ENTRY		0xFFFF
#define VCD_INVALID_LID			VCD_INVALID_ENTRY
#define VCD_INVALID_TRACK		0xFF
#define VCD_INVALID_ITEMID  	0xFFFF
#define VCD_INVALID_OFFSET		0xFFFF
#define VCD_INVALID_LSN			0xFFFFFFFF
#define VCD_INVALID_AUDIO_TYPE  4
#define VCD_INVALID_BSN			200		/* In Spec VI-7, the range of BSN is 1- 99 ? */

/* see Segment Play Item Contexts bytes in III-20 */
typedef enum 
{
	VCD_SEG_VIDEO_TYPE_NOSTREAM		= 0,   
	VCD_SEG_VIDEO_TYPE_NTSC_STILL	= 1,   
	VCD_SEG_VIDEO_TYPE_NTSC_STILL2	= 2,  /* lo+hires*/
	VCD_SEG_VIDEO_TYPE_NTSC_MOTION	= 3,
	VCD_SEG_VIDEO_TYPE_PAL_STILL	= 5,    
	VCD_SEG_VIDEO_TYPE_PAL_STILL2	= 6,   /* lo+hires*/
	VCD_SEG_VIDEO_TYPE_PAL_MOTION	= 7,
	VCD_SEG_VIDEO_TYPE_INVALID		= 8
} VCD_SEG_VIDEO_TYPE;

typedef enum 
{
	PSD_DESC_TYPE_PLAY_LIST			 = 0x10,	/* Play List */
	PSD_DESC_TYPE_SELECTION_LIST	 = 0x18,	/* Selection List (+Ext. for SVCD) */
	PSD_DESC_TYPE_EXT_SELECTION_LIST = 0x1a,	/* Extended Selection List (VCD2.0) */
	PSD_DESC_TYPE_END_LIST 			 = 0x1f,	/* End List */
	PSD_DESC_TYPE_COMMAND_LIST 		 = 0x20		/* Command List */
} PSD_DESCRIPTOR_TYPE;

typedef struct  
{
	unsigned char	type;
	unsigned char	numItem;			/* number of items */
	unsigned short	listID;				/* high-bit means this list is rejected in the LOT.
									   		listID of the list with offset 0x0000(the beginning of PSD) must be 1 */
	unsigned short	prevOffset;			/* previous list offset (0xffff disables) */
	unsigned short	nextOffset;			/* next list offset (0xffff disables) */
	unsigned short	returnOffset;		/* return list offset (0xffff disables) */
	unsigned short	playTime;			/* play time in 1/15 s, 0x0000 meaning full item */
	unsigned char	playItemWaitTime;	/* delay after, in seconds, 
											if 1 <= playItemWaitTime <= 60 wait time is playItemWaitTime 
											else if 61 <= playItemWaitTime <= 254 wait time is (playItemWaitTime-60) * 10 + 60 
											else playItemWaitTime == 255 wait time is infinite  */
												
	unsigned char	autoPauseTime;		/* auto pause wait time calculated same as play item wait time,
                     						used for each item in list if the auto pause flag in a sector is true */
	unsigned short	playItemNum[VCD_EMPTY_ARRAY_SIZE]; 	/* play item number
                                   							0 <= n <= 1      - play nothing
                                   							2 <= n <= 99     - play track n
                                   							100 <= n <= 599    - play entry
                                               				(n - 99) from entries table to end of track
                                   							600 <= n <= 999    - reserved
                                   							1000 <= n <= 2979   - play segment play item (n - 999)
                                   							2980 <= n <= 0xffff - reserved */
} __attribute__((packed))  PSD_PLAYLIST_DESC;

typedef struct {

	unsigned char	selectionAreaFlag : 1;
	unsigned char	commandListFlag : 1;
	unsigned int	reserved : 6;
} __attribute__((packed)) PSD_SELECTION_LIST_FLAG;
  	
typedef struct 
{
	unsigned char			type;
	PSD_SELECTION_LIST_FLAG flags;
	unsigned char			numSelections;	/* Number of Selection (NOS) */
	unsigned char			baseSelectNum;	/* base of selection number (BSN) */
	unsigned short			listID;
	unsigned short			prevOffset;
	unsigned short			nextOffset;
	unsigned short			returnOffset;
	unsigned short			defaultOffset;
	unsigned short			timeoutOffset;
	unsigned char			timeToWait;		/* wait time for time out */
	unsigned char			loopJump;		/* loop count and jump timing */
	unsigned short			playItemNum;
	unsigned short			offset[VCD_EMPTY_ARRAY_SIZE]; /* variable length */
  
} __attribute__((packed)) PSD_SELECTION_LIST_DESC;

typedef struct 
{
    
	PSD_DESCRIPTOR_TYPE	descriptorType;
	
	union { 
		PSD_PLAYLIST_DESC* pPlayListDesc; 
		PSD_SELECTION_LIST_DESC* pSelectListDesc;
	}; 
    
} PSD_LIST_DESC;
  
typedef struct 
{

	unsigned char		bKARINFOBIH : 1;		/* 0 : KARINFO.BIH isn't present; 1 : is present */
		
	unsigned int		restriction : 2;		/* restriction, eg. "unsuitable for kids":
                                              		0x0 : unrestricted,
                                              		0x1 : restricted category 1,
                                              		0x2 : restricted category 2,
                                              		0x3 : restricted category 3 */
                                              		
	unsigned char		bSpecialInfo : 1;       /* 1 : Special Information is encoded in the pictures */
	unsigned char       bUserData	 : 1;       /* 1 : MPEG User Data is used for Closed Caption */
	unsigned char		bUseLID2 	 : 1;		/* 1 : if the PSD is interpreted and the next disc has the same album id 
		                                              then start the next disc at List ID #2, otherwise List ID #1 */ 
                                              
	unsigned char		bUseTrack3	: 1;        /* 1 : if the PSD is not interpreted and next disc has same album id, 
														then start next disc with track 3, otherwise start with track 2 */ 
														
	unsigned char		bPBC_x		: 1;		/* 1 : extended PBC available (PSD_X.VCD and LOT_X.VCD are present) */

} __attribute__((packed)) INFO_VCD_STATUS_FLAGS;

typedef struct 
{

	unsigned int	audioType : 2;		/* Audio characteristics:
                                           0x0 : No MPEG audio stream
                                           0x1 : One MPEG1 or MPEG2 audio stream without extension
                                           0x2 : Two MPEG1 or MPEG2 audio streams without extension
                                           0x3 : One MPEG2 multi-channel audio stream w/ extension
                                         */                                           
	unsigned int	videoType : 3;      /* Video characteristics:
                                           0x0 : No MPEG video data
                                           0x1 : NTSC normal resolution still picture
                                           0x2 : NTSC normal and high resolution still picture
                                           0x3 : NTSC motion picture
                                           0x4 : Reserved
                                           0x5 : PAL normal resolution still picture
                                           0x6 : PAL normal and high resolution still picture
                                           0x7 : PAL motion picture */
                                           
	unsigned char	itemCont : 1;		/* Indicates segment is continuation
                                           0x0 : 1st or only segment of item
                                           0x1 : 2nd or later segment of item */
                                            
	unsigned int	ogt 	 : 2;		/* 0x0 : no OGT substream 
                                           0x1 : sub-stream 0 available
                                           0x2 : sub-stream 0 & 1 available
                                           0x3 : all OGT sub-substreams available */

} __attribute__((packed)) INFO_VCD_SPI_CONTENTS;

typedef struct 
{

	char					ID[8];			/* "VIDEO_CD" : VCD
  										   		"SUPERVCD" or "HQ-VCD" : SVCD */
  								
	unsigned char			version;		/* 0x02 : VCD2.0,
										   	   0x01 : SVCD or VCD1.x */ 
                                
	unsigned char			sysProfTag;      /* System Profile Tag, used to define the set of
                                				mandatory parts to be applied for compatibility;
                                				0x00 for "SUPERVCD",
                                				0x01 for "HQ-VCD  ",
                                				0x0n for VCDx.n */ 
                                
	char					albumID[16];	/* Album Identification */
	unsigned short			numVolumes;	    /* Number of Volumes in Album */
  	unsigned short			seqNum;		    /* Album Set Sequence Number */
	unsigned char			picSizeFlag[13];/* Size Map of Track Motion Pictures (bit 0 of the first byte is for Track #2)
												   0 : NTSC (352*240)
												   1 : PAL  (352*288) */
													   
	INFO_VCD_STATUS_FLAGS	statusFlags;     
	unsigned long			psdSize;		/* size of PSD.VCD file */
	msf_t					firstSegAddr;	/* BCD sector location.
                                				The first sector of the Segment Play Item Area, in the form mm:ss:00. 
                                				Must be 00:00:00 if the PSD size is 0. */
                                			
	unsigned char			offsetMult;      /* offset multiplier, must be 8 */
	unsigned short			lotEntryCount;   /* offsets in lot */
	unsigned short			maxSegNum;       /* segments used for segmentitems */
	
	INFO_VCD_SPI_CONTENTS	SPIContents[VCD_MAX_SEGMENTS]; /* The next 1980 bytes contain one byte for each possible segment play item. 
														  Each byte indicates contents. */

	unsigned short			playingTime[5];	/* in seconds */
	char					reserved[2];          
	
} __attribute__((packed)) INFO_VCD;

typedef struct 
{
	char ID[8];					/* "ENTRYVCD" */
	unsigned char version;		/* 0x02 : VCD2.0
                                   0x01 : SVCD, should be same as version in INFO.SVD */
                                   
	unsigned char sysProfTag;   /* 0x01 if VCD1.1
                                   0x00 else */
	unsigned short entryCount;	/* 1 <= entries <= 500 */
	
	struct 
	{							/* all fields are BCD */
    	unsigned char	n;		/* 2 <= track number <= 99 */
		msf_t 			msf;	/* mm:ss:ff Sector address */
	} __attribute__((packed)) entry[VCD_MAX_ENTRIES];
	
	unsigned char	reserved2[36];		/* RESERVED, must be 0x00 */
	
} __attribute__((packed)) ENTRIES_VCD;	/* sector 00:04:01 */

//////////////////////////////////////////////////////////////////////////////
// This file must be present if the PSD Size in INFO.VCD is not zero.		//
// This table allows user to start playing the PSD from Lists other than	//
// the default start List ID Number.										//
//																			//
// The value of List ID #1 offset is 0x0000 (start of PSD)					//
// For those unused List ID, offset must be 0xFFFF							//
//////////////////////////////////////////////////////////////////////////////

#define MAX_LIST_ID	32767

typedef struct 
{
	unsigned short	reserved;  
	unsigned short	offset[MAX_LIST_ID];  /* offset given in 8 byte units */
	
} __attribute__((packed)) LOT_VCD;



class CVCDCore	
{
	friend class CInputVCD;
protected:
	
	INFO_VCD 		m_infoVCD;		// from INFO.VCD //info
	ENTRIES_VCD		m_entriesVCD;	// from ENTRIES.VCD	//entries
	LOT_VCD*		m_pLotVCD;
	LOT_VCD*		m_pLotxVCD;
	LOT_VCD			m_fullLot;		// including all offset in LOT.VCD and prev/next/return/default offset in PXD
	unsigned char*	m_pPsdVCD;
	unsigned char*	m_pPsdxVCD;
	
	void*			m_pTracksBuf;
	void*			m_pSearchBuf;
	void*			m_pScandataBuf;
	
public:    
	
	
	int				m_sectorPerSec;	//sector_per_sec;  
	unsigned long   m_firstSegLSN;	//first_segment_lsn
	unsigned int	m_psdxSize;
	//unsigned char extended;
	//unsigned char has_xa;           /* True if has extended attributes (XA) */
	
	//char* 		m_pSourceName; /* VCD device or file currently open */

protected:
	const static int ISO_BLOCKSIZE		= 2048; /** Number of bytes in an ISO 9660 block */
	const static int LOT_VCD_SIZE		= 32;
	const static int LOT_VCD_SIZE_BYTE	= LOT_VCD_SIZE * 2048;

	unsigned char readFile(char* pSourceName, char* pFileNameLit, char* pFileNameCap, unsigned int len, void* pData);
	unsigned char readFile(char* pPathNameLit, char* pPathNameCap, unsigned int len, void* pData);
	unsigned char readINFO_VCD(char* pSourceName, unsigned char bIsVCD, char* pDirName = NULL) ;
	unsigned char readENTRIES_VCD(char* pSourceName, unsigned char bIsVCD, char* pDirName = NULL);
	unsigned char readLOT_VCD(char* pSourceName, LOT_VCD* pLot, unsigned char bIsVCD, char* pDirName = NULL) ;
	unsigned char readPSD_VCD(char* pSourceName, unsigned char bIsVCD, char* pDirName = NULL);
	unsigned char readPSD_X_VCD(char* pSourceName);
	unsigned char readLOT_X_VCD(char* pSourceName);
	unsigned char readTRACKS_SVD(char* pSourceName);
	unsigned char readSEARCH_DAT(char* pSourceName);    
	unsigned char readSCANDATA_DAT(char* pSourceName);
	//unsigned char BuildLOT(unsigned char extended);
	//unsigned char xBuildLOT(VCD_PBC_CONTEXT* pContext);
	
	unsigned char	xGetPxdFromLID(PSD_LIST_DESC *pxd, unsigned short listID, unsigned char ext) ;
	unsigned short	xGetLIDFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc);
	unsigned short	xGetLIDFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	int 			xGetRealWaitTime(unsigned short time);	
	
	unsigned char	xGetPXDLIDFromLOT(unsigned int offset, unsigned char ext, PSD_LIST_DESC* pPXD, unsigned short* pListID);
	unsigned short	xGetLotOffset(const LOT_VCD* pLot, unsigned int n) ;
	unsigned char	xBuildFullLOT(LOT_VCD* pLot, LOT_VCD* pFullLot);
	unsigned char	xGoThroughPXD(LOT_VCD* pFullLot, unsigned int offset);
		
public:
    CVCDCore()  {
    				m_pLotVCD = m_pLotxVCD = NULL;
    				m_pPsdVCD = m_pPsdxVCD = NULL;
					m_pScandataBuf = m_pTracksBuf = m_pSearchBuf = NULL;
					
					memset(&m_entriesVCD, 0, sizeof(m_entriesVCD));
					memset(&m_infoVCD, 0, sizeof(m_infoVCD));
    			}
    ~CVCDCore();

	unsigned char Init(char* pSourceName, unsigned char bIsVCD);
	
	
	unsigned short	GetNumLIDs();
	unsigned long	GetPsdSize();	
	
	unsigned char	GetTrackNum(const unsigned int entryNum);
	unsigned long	GetTrackLSN(unsigned char trackNum);
		
	const msf_t*	GetEntryMSF(unsigned int entryNum);
	unsigned long	GetEntryLSN(unsigned int entryNum);
	unsigned int	GetNumEntries();
	
	VCD_SEG_VIDEO_TYPE	GetSegVideoType(unsigned short segNum);
	unsigned int 		GetSegAudioType(unsigned short segNum);
	unsigned short		GetNumSeg();
	unsigned long		GetSegLSN(unsigned short segNum);
	
	unsigned char	GetPxdFromLID(PSD_LIST_DESC* pPxd, unsigned short listID);
	unsigned short	GetDefaultOffsetFromListID(unsigned short listID);
	
	unsigned int	GetNumSelections(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned int 	GetBaseSelectNum(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetPlayItemNumFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetPrevOffsetFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetNextOffsetFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetReturnOffsetFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetDefaultOffsetFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short 	GetTimeOutOffsetFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetLoopJumpFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	unsigned short	GetOffsetFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc, int i);
	unsigned short	GetRandomSelectLID(const PSD_SELECTION_LIST_DESC* pSelectListDesc, int entry);
	int 			GetTimeToWaitFromSelectListDesc(const PSD_SELECTION_LIST_DESC* pSelectListDesc);
	
	
	
	unsigned short	GetPrevOffsetFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc);
	unsigned short	GetNextOffsetFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc);
	unsigned short	GetReturnOffsetFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc);
	unsigned short 	GetPlayItemNumFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc, unsigned int i);
	int 			GetNumItemFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc);
	int 			GetWaitTimeFromPlayListDesc(const PSD_PLAYLIST_DESC* pPlayListDesc);
		
	
	
	unsigned short GetLIDFromFullLOT(unsigned int offset);
	
	unsigned short GetLotOffset(const LOT_VCD* pLot, unsigned int n);
	
	unsigned char BuildFullLOT(unsigned char extended);
	
	static int DirSVCDFilter(const struct dirent* file);
};

#endif //#define _VCD_CORE_H
