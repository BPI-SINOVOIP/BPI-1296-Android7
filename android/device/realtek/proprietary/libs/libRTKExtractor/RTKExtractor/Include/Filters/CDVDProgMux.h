//---------------------------------------------------------------------------
//File: CDVDProgMux.h
//
//Desc: Header file for DVD-VENUS Program Mux Filter Class interface define
//
//Author: Yuantao Wu     06/24/2004
//
//Copyright (c)RealTek.   All rights reserved.

#ifndef __C_PROG_MUX_FILTER_H__
#define __C_PROG_MUX_FILTER_H__

#include <string.h>
#include <OSAL.h>
#include <Util.h>
#include <CStream.h>
#include <Filters/CBaseMuxFilter.h>
#include <VideoRPCBaseDS_data.h>
#include <AudioRPCBaseDS_data.h>
#include <Filters/PrivateInfo.h>
#if IS_CHIP(JUPITER) || IS_CHIP(SATURN)
  #include <Platform_Lib/JupiterDummyCommon.h>
#endif

struct _PROG_MUX_DATA;
struct _DVD_MUX_INPUT_INFO;
struct _DVDV_INFO;
struct _STINFO;

#define DVD_MUX_PAL					MEDIASUBTYPE_AnalogVideo_PAL
#define DVD_MUX_NTSC					MEDIASUBTYPE_AnalogVideo_NTSC

//define audio stream type
#define DVD_MUX_AUD_MP2					MEDIASUBTYPE_MPEG1ES
#define DVD_MUX_AUD_AC3					MEDIASUBTYPE_DOLBY_AC3
#define DVD_MUX_AUD_LPCM				MEDIASUBTYPE_DVD_LPCM_AUDIO
#define DVD_MUX_AUD_IEC					MEDIASUBTYPE_DOLBY_AC3_SPDIF
#define DVD_MUX_AUD_NULL				0xffffffff

typedef ulDArray<CMediaType*>            MediaTypeArr;

//Mux filter private info for output define
typedef enum _ENUM_DVD_MUX_OUTPUT_INFO_TYPE {
    MUXOUTPUT_RECORD_GEN,
    MUXOUTPUT_VOBU,
    MUXOUTPUT_CELL,
    MUXOUTPUT_VOB,
    MUXOUTPUT_TMAP_RUNTIME,
    MUXOUTPUT_TMAP_END,
	MUXOUTPUT_STILL,
    MUXOUTPUT_TOTAL
}ENUM_DVD_MUX_OUTPUT_INFO_TYPE;

//Mux Output info format -- GenInfo
typedef struct  _ProgStreamGenInfo {
    ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
    ENUM_MEDIA_TYPE videoEncoderType;
    ENUM_MEDIA_TYPE videoInputSource;
    ENUM_MEDIA_TYPE TVSYStemFormat;
    ENUM_VIDEO_HORIZONTAL_RESOLUTION horizontalResolusion; 
    ENUM_VIDEO_ENCODER_RC_MODE videoRCMoce;
    unsigned int videoBitRate;
    ENUM_MEDIA_TYPE audioEncoderType;
    unsigned int audioRate;
    unsigned int samplingFreq;
    unsigned int audioEncodingMode;
    unsigned char video_Gop_M;
    unsigned char video_Gop_N;
    unsigned char numOfGOPsPerVOBU;
	unsigned char dummy;
} PROG_STREAM_GEN_INFO;

//Mux Output info format -- VOBUInfo
typedef struct  _ProgStreamVobuInfo {
    ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
    unsigned int vobuNumber;
    unsigned int firstRefPicSize;
    unsigned int secondRefPicSize;
    unsigned int ThirdRefPicSize;
    unsigned int audio1AsyncA;
    unsigned int audio2AsyncA;
	unsigned int audio1AsyncA_Prev;
    unsigned int audio2AsyncA_Prev;
    unsigned int vobuPbTime;
    unsigned int vobuSize;
    unsigned int vobuStartPTM;
    unsigned int vobuEndPTM;
    unsigned int vobuStartSCR;
    unsigned int vobuEndSCR;
    unsigned char keyFrame; 
	unsigned char VOBU_C_IDN;
	unsigned char dummy;
	unsigned char lastVOBUofVOB;
	unsigned int vobuAbsAddr;
	CGMS_INFO CopyRightInfo;
} PROG_STREAM_VOBU_INFO;

//Mux Output info format -- CellInfo
typedef struct  _ProgStreamCellInfo {
	ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
	unsigned int cellID;
	unsigned int PBTime;
	unsigned int firstVOBUStartAddr;
	unsigned int firstILVUEndAddr;
	unsigned int lastVOBUStartAddr;
	unsigned int lastVOBUEndAddr;
	unsigned char newChapter;
	unsigned char newLayer;
	unsigned char numOfCellCommand;
	unsigned char lastCell;
	unsigned int command[256];
	bool discontinuity;
}PROG_STREAM_CELL_INFO;

//Mux Output info format -- VOBInfo
typedef struct  _ProgStreamVobInfo {
	ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
	unsigned int vobStartPTM;
	unsigned int vobEndPTM;
	unsigned int vobId;
	unsigned int numOfVOBU;
        unsigned char newChapter;
        unsigned char lastVOB;
}PROG_STREAM_VOB_INFO;

//Mux Output info format -- RuntimeTMAPInfo
typedef struct  _ProgStreamTmapRuntimeInfo {
	ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
	unsigned int TMAPEntryIndex;
	unsigned int numOfVOBUs;
	unsigned int timeDiff;
	unsigned int VOBUAddr;
	unsigned int VOBUInfo[25];
}PROG_STREAM_TMAP_RUNTIME_INFO;

//Mux Output info format --TMAPEndInfo
typedef struct  _ProgStreamTmapEndInfo {
	ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
	unsigned int vobStartPTM;
	unsigned int vobEndPTM;
	unsigned int VobLastSCRBase;
	unsigned int vobLastSCRExt;
}PROG_STREAM_TMAP_END;

//Mux Output info format --TMAPEndInfo
typedef struct  _ProgStreamStillInfo {
	ENUM_DVD_MUX_OUTPUT_INFO_TYPE infoType;
	unsigned int size;
	unsigned int lastData;
}PROG_STREAM_STILL_INFO;

//define input elementary type
typedef enum {
	DVD_MUX_AUDIO1=0, 
	DVD_MUX_AUDIO2, 
	DVD_MUX_VIDEO, 
	NUMBER_OF_MUXER_INPUT
} DVD_MUX_INPUTSTREAMTYPE;

//Copy Control mode define
typedef enum _DVDMuxCopyControlMode
{   
    DVD_MUX_COPYCONTROL_OPTION_COPY,
    DVD_MUX_COPYCONTROL_OPTION_CONVERT,
    DVD_MUX_COPYCONTROL_OPTION_USER_SPECIFY
}ENUM_DVD_MUX_COPYCONTROL_MODE;

//PES scrambling control define
typedef enum _DVDMuxPESScramblingMode
{
    DVD_MUX_PES_SCRAMBLING_OPTION_DISABLE,   
    DVD_MUX_PES_SCRAMBLING_OPTION_COPY,   
    DVD_MUX_PES_SCRAMBLING_OPTION_CONVERT,   
    DVD_MUX_PES_SCRAMBLING_OPTION_USER_SPECIFY   
}ENUM_DVD_MUX_PESSCRAMBLING_MODE;

class   CDVDProgramMuxInputPin;
class   CDVDProgramMuxDataOutputPin;
class   CDVDProgramMuxInfoOutputPin;

class CDVDProgMuxFilter : public CBaseMuxFilter
{

    friend class CDVDProgramMuxInputPin;
    friend class CDVDProgramMuxDataOutputPin;
    friend class CDVDProgramMuxInfoOutputPin;

public:

    CDVDProgMuxFilter(int editOnly = 0, ENUM_MEDIA_TYPE TVSystem = MEDIASUBTYPE_AnalogVideo_NTSC);
    ~CDVDProgMuxFilter(void);

    // Override Stop(). Mux Filter is stopped by its upstream filter, i.e., Video Encoder,
    // automatically.
    HRESULT                     Stop(void);

    //Override Pause(). Muxer filter do its initialization and is ready to receive data.
    HRESULT                     Pause(FILTER_STATE State);

    //Override Run. Muxer filter is started by its upstream filter automatically.
    HRESULT                     Run(void);

    HRESULT                     StartNewCell(unsigned int timeCodeHigh,
                                                          unsigned int timeCodeLow,
                                                          unsigned int newCellId,
                                                          unsigned int newChapter,
                                                          unsigned int newDiscLayer,
                                                          unsigned int numOfCellCommand,
                                                          unsigned long *pCommand);

    HRESULT                     GenerateDVDMenuVOB(struct _STINFO *pStinfo);

    HRESULT                     InsertRDNTPack(unsigned int PayloadSize,
                                                                       char * pSubPicUnit,
                                                                       unsigned int RecordingDate,
                                                                       unsigned int RecordingTime);

    HRESULT                     SetStartRLBN(unsigned int startRLBN);

    HRESULT                     ConfigRDIPack(unsigned int * pVobuRecordTime,
                                                                    char * pMfgId);

    HRESULT                     SetCopyControl(
		                                     ENUM_DVD_MUX_COPYCONTROL_MODE CopyControlMode,
                                                   unsigned int setting);

    HRESULT                     SetPESScramblingMode(
                                                   ENUM_DVD_MUX_PESSCRAMBLING_MODE PESScramblingMode,
                                                   unsigned int PESScrambling);

    HRESULT                     SetDVDVideoVOBSize(unsigned int VOBSize);

    HRESULT                     SetCDTrackID(unsigned int CDTrackID);

    HRESULT                     CheckOutputAvailable(int bufferNum);

    HRESULT                     GetRecordPosition(long long videoPTS, unsigned int *second, unsigned *frame);

    HRESULT                     ConfigLivePause(int on, unsigned int vobuPerCell);

    int                         InputDataEmpty();

    void						ProcessData();
private:

	//Private Function member 
    void						dvd_muxing_Process();
	void						dvd_muxing_NormalInit();
	void						dvd_muxing_RunTimeInit();
	int							dvd_muxing_FindFirstAudio();
	void						dvd_muxing_StillPic();
	void						dvd_muxing_CDDA();
	void						dvd_muxing_Program();
	void						dvd_muxing_RequestOutputSpace();
	void						dvd_muxing_SendOutData();
	void						dvd_muxing_SendOutMessages(int msgQueueIndex);
	void						dvd_muxing_UpdateAudioSTDBuffer(int audioStreamID);
	void						dvd_muxing_UpdateVideoSTDBuffer();
	int							dvd_muxing_DecideNextPackType();
	int							dvd_muxing_GenerateRecoverPack();
	int							dvd_muxing_GenerateAudioPack(int audioStreamID);
	int							dvd_muxing_GenerateVideoPack();
	void						dvd_muxing_GenerateRDIPack();
	void						dvd_muxing_GenerateNVPack();
	int						dvd_muxing_GenerateSubPicturePack();
	void						dvd_muxing_GenerateRDNTPack();
	void						dvd_muxing_GeneratePrivatePack();
	void						dvd_muxing_UpdateSCR();
	void						dvd_muxing_UpdateSCRtoNearestDTS();
	int							dvd_muxing_StopGenerateVideoPack();
	void						dvd_muxing_checkCondition();
	void						dvd_muxing_PostProcessing();
	int							dvd_muxing_GenerateCdromHeader(unsigned char *pPack,
																DVD_MUX_INPUTSTREAMTYPE type);
	void						dvd_muxing_ClearPointer();
	int							dvd_muxing_Calc_PTS_DTS(int *ptsdts, 
														int *ptsdtslength, 
														int *stuffingbytes,
														int *totalpaddingbytes);
	int							dvd_muxing_GeneratePackHeader(unsigned char *pPack, 
																unsigned int scrBase, 
																unsigned int scrExt);
	int							dvd_muxing_GenerateSystemHeader(unsigned char *pPack, 
																int type);
	int							dvd_muxing_GeneratePESheader(unsigned char *pPack, 
															char ptsDts, 
															char ptsDtsLength, 
															char stuffingBytes, 
															int pesExt, 
															int packHeaderLength, 
															unsigned int index, 
															unsigned int streamID, 
															int audioStreamID );

	void						dvd_muxing_GeneratePaddingPack(unsigned char *pPack,
																int size);
	void						dvd_muxing_Log_RTRW();
	void						dvd_muxing_Update_Ref_Frame_EA(int picType, 
																int seperatePack);
	int							dvd_muxing_PackBits(unsigned char *pPack,
													int value,
													int numOfBits);
	int							dvd_muxing_SpecialCompare(unsigned int a, 
														unsigned int b);
	int							dvd_muxing_Update_Input_Fullness(struct _DVD_MUX_INPUT_INFO *pPort);
	void						dvd_muxing_prepareNavPacket();
	void						dvd_muxing_FillRDIPack();
	void						dvd_muxing_FillNavPack();
	void						dvd_muxing_calNavSRI(unsigned char *pNavPack,
													struct _DVDV_INFO *pDVDVinfo,
													unsigned int streamFormat);
	void						dvd_muxing_copyData(unsigned char *dest,
													unsigned char *source,
													int size);
	void						dvd_muxing_updateOutput();
	void						dvd_muxing_prepareMessage();
	void						dvd_muxing_updatePausedPTS();
	int 						dvd_muxing_ProcessPrivateInfo(BYTE *pInfo, 
																int length,
																DVD_MUX_INPUTSTREAMTYPE input);
	void						dvd_muxing_ProcessDataInfo(BYTE *pWritePointer, 
															long WriteBufferSize,
															DVD_MUX_INPUTSTREAMTYPE input);
	void						dvd_muxing_ProcessVideoEndOfStream();
	void						dvd_muxing_ProcessAudioEndOfStream();
	void						dvd_muxing_Menu();													
    void                        dvd_muxing_MoveToNextBuffer();
    CDVDProgramMuxInputPin					*m_pVideoInput;
    CDVDProgramMuxInputPin					*m_pMainAudioInput;
    CDVDProgramMuxInputPin					*m_pSecondAudioInput;
    CDVDProgramMuxDataOutputPin				*m_pDataOutput;
    CDVDProgramMuxInfoOutputPin				*m_pInfoOutput;

    CMemRingAllocator						*m_pVideoInputAllocator;
    CMemRingAllocator						*m_pMainAudioInputAllocator;
    CMemRingAllocator						*m_pSecondAudioInputAllocator;

    IReadPointerHandle						*m_pReadPointerHandle[NUMBER_OF_MUXER_INPUT];

    CCritSec								SemInput;
    struct _PROG_MUX_DATA*							m_pMuxData;
};

class CDVDProgramMuxInputPin : public CBaseInputPin
{

    friend class CDVDProgMuxFilter;

public:

    CDVDProgramMuxInputPin(const char *Id);
    ~CDVDProgramMuxInputPin(void);

    //Override QueryAccept since Mux filter input accepts multiple MediaTypes.
    HRESULT                     QueryAccept(const MEDIA_TYPE *pmt);

    //Override Connect to save the MEDIA_TYPE for the successful connection
    HRESULT                     ReceiveConnection(IPin *pReceivePin,
                                                                             const MEDIA_TYPE *pmt);

    //Override EndOfStream since it is empty for the base class.
    HRESULT                     EndOfStream(unsigned int eventID);

    //Override Receive(). Mux filter will do muxing inside Receive.
    HRESULT                     Receive(BYTE *pWritePointer, long WriteBufferSize);

    //Override ReceiveCanBlock() since it is empty for the base class.
    HRESULT                     ReceiveCanBlock(void);

    //Override PrivateInfo(). Mux filter will handle all the information coming from 
    //upstream filter here.
    HRESULT                     PrivateInfo(BYTE *pInfo, int length);

    //Override Disconnect() to release ReadHandle
    HRESULT						Disconnect(void);

private:
    IReadPointerHandle						*m_pReadPointerHandle;
    MediaTypeArr  m_pMediaTypeArray;
	DVD_MUX_INPUTSTREAMTYPE input;
	
};


class CDVDProgramMuxDataOutputPin : public CBaseOutputPin
{

    friend class CDVDProgMuxFilter;

public:

    CDVDProgramMuxDataOutputPin(const char *Id);
    ~CDVDProgramMuxDataOutputPin();	

    //Override QueryAccept since Mux filter input accepts multiple MediaTypes.
    HRESULT                     QueryAccept(const MEDIA_TYPE *pmt);

    //Override Connect since no Negotiate about allocator in the Base Class
    HRESULT                     Connect(IPin *pReceivePin,
                                                       const MEDIA_TYPE *pmt);

	HRESULT						EndOfStream(unsigned int eventID);
private:

    MediaTypeArr  m_pMediaTypeArray;
};	

class CDVDProgramMuxInfoOutputPin : public CBaseOutputPin
{

    friend class CDVDProgMuxFilter;

public:

    CDVDProgramMuxInfoOutputPin(const char *Id);
    //~DVDProgramMuxInfoOutputPin(void);	

    //Override QueryAccept since no media type involved.
    HRESULT                     QueryAccept(const MEDIA_TYPE *pmt);

    //Override DeliverPrivateInfo since it is empty for the base class
    HRESULT                     DeliverPrivateInfo(BYTE *pInfo,
                                                                         int length);

    //Override SetAllocator since no Allocator
    HRESULT                     SetAllocator(IMemAllocator *pAllocator);

	//Override GetAllocator since no Allocator
    HRESULT                     GetAllocator(IMemAllocator **pAllocator);

};	

#endif
