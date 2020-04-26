/***********************************************************************************

THE COPYRIGHT IN THE CONTENTS OF THIS SOFTWARE VEST WITH WIPRO
LIMITED A COMPANY INCORPORATED UNDER THE LAWS OF INDIA AND HAVING
ITS REGISTERED OFFICE AT 10TH FLOOR DU PARC TRINITY, 17 M.G ROAD,
BANGALORE 560 001. DISTRIBUTION OR COPYING OF THIS SOFTWARE BY
ANY INDIVIDUAL OR ENTITY OTHER THAN THE ADDRESSEE IS STRICTLY
PROHIBITED AND MAY INCUR LEGAL LIABILITY. IF YOU ARE NOT THE
ADDRESSEE PLEASE NOTIFY US IMMEDIATELY BY PHONE OR BY RETURN EMAIL.
THE ADDRESSEE IS ADVISED TO MAINTAIN THE PROPRIETARY INTERESTS OF
THIS COPYRIGHT AS PER APPLICABLE LAWS.

***********************************************************************************/
/***********************************************************************************
 * NAME : avc_uapi.h
 * 
 * SYNOPSIS : Contains data structures and definitions specified by AVLINK  
 *            hardware.                                                     
 *
 * EXPORTED FUNCTIONS : None.
 *
 * IMPORTED FUNCTIONS : Functions imported from memMgr.h.
 *                 
 * DESCRIPTION : This file contains functions 
 *               Contains data structures and definitions specified by AVLINK  
 *               hardware.                                                     
 *               
 * NOTES: None.
 *
 * SEE ALSO : None.
 *
 *
************************************************************************************/

#ifndef AVC_UAPI_H
#define AVC_UAPI_H

/* IMPORTANT NOTE :
    These definiations have been DUPLICATED for THE USER SPACE libraries.
    These MUST MATCH EXACTLY WITH THOSE in "AvcUnitCmdLibDefs.h".
*/

#define UNIT_TYPE                 0x1F
#define UNIT_I                    0x07

#define AVC_SUTYPE_MONITOR        0x00
#define AVC_SUTYPE_AUDIO          0x01
#define AVC_SUTYPE_PRINTER        0x02
#define AVC_SUTYPE_DISC           0x03
#define AVC_SUTYPE_TAPE_RECORDER  0x04
#define AVC_SUTYPE_TUNER          0x05
#define AVC_SUTYPE_AC             0x06
#define AVC_SUTYPE_CAMERA         0x07
#define AVC_SUTYPE_RESERVED       0x08
#define AVC_SUTYPE_PANEL          0x09
#define AVC_SUTYPE_BULLETIN_BOARD 0x1A
#define AVC_SUTYPE_CAMERA_STORAGE 0x1B
#define AVC_SUTYPE_VENDOR         0x1C
#define AVC_SUTYPE_EXTEND         0x1E
#define AVC_SUTYPE_UNIT           0x1F

#define AVC_RESULT_UNKNOWN  (unsigned char)0xFF
#define AVC_RESERVED        (unsigned char)0x00

#define POWER_ON                  0x70
#define POWER_OFF                 0x60
#define DEFAULT_ID                0x00
#define SOURCE_PLUG               0x03
#define DESTINATION_PLUG          0x0A
#define PRIORITY                  0x04
#define ISOCHRONOUS_PLUG_INFO     0x00
#define ASYNCHRONOUS_PLUG_INFO    0x01
#define ISOCHRONOUS_CHANNEL       0x05

unsigned short usAppSrcId, usAppDestnId;

/* Enumerations */

enum{
    PWR         = 0xB2,
    UINFO       = 0x30,
    SUINFO      = 0x31,
    RSRV        = 0x01,
    VERS        = 0xB0,
    PLGINFO     = 0x02, 
    CHANUSAGE   = 0x12,
    CONN        = 0x24,
    CONNAV      = 0x20,
    CONNS       = 0x22,
    DIGINP      = 0x11,
    DIGOUT      = 0x10,
    DISCON      = 0x25,
    DISCONAV    = 0x21,
    INP_PLG_SIG = 0x19,
    OUT_PLG_SIG = 0x18
}GENERAL_COMMAND_SET;


typedef enum{
    MPEG_L = 0,
    DVCR_L
}enSigFormat;


enum{
    CONTROL_L =0x0,
    STATUS_L,
    SPECIFIC_INQUIRY_L,
    NOTIFY_L,
    GENERAL_INQUIRY_L
}COMMAND_TYPE;

enum{
    NOTIMPLEMENTED_L = 0x8,
    ACCEPTED_L,
    REJECTED_L,
    IN_TRANSITION_L,
    IMPLEMENTED_L,
    CHANGED_L,
    RESERVED_L,
    INTERIM_L
}RESPONSE_CODE;


/* Structure Declarations */

#ifdef LITTLE_ENDIAN

typedef struct _OPERANDS
{
    union{        
        struct{
            unsigned char ucEoh:1;
            unsigned char ucForm:1;
            unsigned char ucFmt:6;
        }PLUG_SIGNAL_FORMAT;        
        unsigned char ucOperand;    
    }OPERAND;
}OPERANDS, *POPERAND;

#else

typedef struct _OPERANDS
{
    union{        
        struct{
            unsigned char ucFmt:6;
            unsigned char ucForm:1;
            unsigned char ucEoh:1; 
        }PLUG_SIGNAL_FORMAT;
        unsigned char ucOperand;
    }OPERAND;
}OPERANDS, *POPERAND;

#endif /*LITTLE_ENDIAN */

char PwrCmd(
    unsigned int fd, unsigned char uchPwrState, unsigned char uchCmdType, 
    unsigned char uchSuType, unsigned char uchSuId, unsigned char **ppAvcFrame,
    unsigned int *pAvcFrameLen, unsigned short usAppDestnId);

char UInfoCmd(
    unsigned int fd, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId );

char SUInfoCmd(
    unsigned int fd, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned char uchPage, unsigned short usAppDestnId );
    
char SUInfoCmdEx(
    unsigned int fd, unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen, 
    unsigned char uchPage, unsigned short usAppDestnId, char fUsrBuff);        

char RsrvCmd(
    unsigned int fd, unsigned char uchPty, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);

char VersionCmd(
    unsigned int fd, unsigned char uchVerInfo, unsigned char **ppAvcFrame,
    unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char PlgInfoCmd(
    unsigned int fd,unsigned char uchSubFn, unsigned char uchSuType, 
    unsigned char uchSuId,unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId );

char ChUsageCmd(
    unsigned int fd,unsigned char uchIsoChanNo, unsigned char uchCmdType, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char ConnectCmd(
    unsigned int fd,unsigned char uchCmdType, unsigned char uchLockBit, 
    unsigned char uchSrcSuType, unsigned char uchSrcSuId, unsigned char uchSrcPlug, 
    unsigned char uchDestSuType, unsigned char uchDestSuId, unsigned char uchDestPlug, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char ConnectAVCmd(
    unsigned int fd,unsigned char uchCmdType, unsigned char uchAvPlgType, 
    unsigned char uchVideoSrc, unsigned char uchAudioSrc, unsigned char uchVideoDest, 
    unsigned char uchAudioDest, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestId);

char ConnectionsCmd(
    unsigned int fd,unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);

char DigInputCmd(
    unsigned int fd, unsigned char uchCmdType, unsigned char ucConState, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char DigOutputCmd(
    unsigned int fd, unsigned char uchCmdType,  unsigned char ucConState, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char DisConnectCmd(
    unsigned int fd,unsigned char uchSrcSuType, unsigned char uchSrcSuId, 
    unsigned char uchSrcPlug, unsigned char uchDestSuType, unsigned char uchDestSuId, 
    unsigned char uchDestPlug, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);

char DisConnectAVCmd(
    unsigned int fd,unsigned char uchAvPlgType, unsigned char uchVideoSrc, 
    unsigned char uchAudioSrc, unsigned char uchVideoDest, unsigned char uchAudioDest, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestId);

char InpSigCmd(
    unsigned int fd,unsigned char uchPlgIndex,unsigned char uchCmdType, 
    unsigned char uchSigFormat, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);

char OutSigCmd(
    unsigned int fd,unsigned char uchPlgIndex,unsigned char uchCmdType, 
    unsigned char uchSigFormat, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);

char GenPlgBusCmd(
    unsigned int fd,unsigned char uchCmdType,unsigned char uchPlgData[12], int len, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, unsigned short usAppDestnId);

//VCR command
char AVC_VCR_RecTimeCmd(
    unsigned int fd, unsigned char uchFunc, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usDestnId);
    
char AVC_VCR_RecDateCmd(
    unsigned int fd, unsigned char uchFunc, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usDestnId);
    
char AVC_VCR_EDIT_MODECmd(
    unsigned int fd,unsigned char  Operand,unsigned char uchSuType,
    unsigned char uchSuId,unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);
    
char AVC_VCR_PRESETCmd(
    unsigned int fd, unsigned char uchCmdType,unsigned char  Operand,
    unsigned char *auchOperand, unsigned char uchSuType,unsigned char uchSuId,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);
    
char AVC_VCR_INPUTSIGNALMODECmd(
    unsigned int fd, unsigned char uchSuType, unsigned char uchSuId,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);
    
char AVC_VCR_LoadMediumCmd(
    unsigned int fd, unsigned char uchSubFunc, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId);
    
char AVC_VCR_TransportStateCmd(
    unsigned int fd, unsigned char uchCmdType, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId);
    
char AVC_VCR_TimeCodeCmd(
    unsigned int fd, unsigned char uchFunc, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId);
    
char AVC_VCR_RecordCmd(
    unsigned int fd, unsigned char uchRecordingMode, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId);

char AVC_VCR_PlayCmd(
    unsigned int fd, unsigned char uchPlaybackMode, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId);

char AVC_VCR_WindCmd(
    unsigned int fd, unsigned char uchFunc, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId);
    
char AVC_VCR_BackwardCmd(
    unsigned int fd, unsigned char uchMeasureUnit, unsigned char *auchOperand, 
    unsigned short usOperandLen, unsigned char uchSuType,unsigned char uchSuId, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char AVC_VCR_ForwardCmd(
    unsigned int fd, unsigned char uchMeasureUnit, unsigned char *auchOperand, 
    unsigned short usOperandLen, unsigned char uchSuType,unsigned char uchSuId, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

//AVHDD command
char AVC_AVHDD_PlayCmd(
    unsigned int fd, unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char AVC_AVHDD_StopCmd(
    unsigned int fd, unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId);

char AVC_AVHDD_SetPlugAssociationCmd(
    unsigned int fd, unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen, unsigned short usAppDestnId);  

char AVC_AVHDD_SendCmd(
    unsigned int fd, unsigned char uchCmdType,unsigned char uchOpcode,
    unsigned char uchOperand0,unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen, unsigned short usAppDestnId);
    
/*----------------------------------------------------------------*/

char PwrCmdEx(
    unsigned int fd, unsigned char uchPwrState, unsigned char uchCmdType, 
    unsigned char uchSuType, unsigned char uchSuId, unsigned char **ppAvcFrame,
    unsigned int *pAvcFrameLen, unsigned short usAppDestnId, char fUsrBuff);

char UInfoCmdEx(
    unsigned int fd, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff );
    
char SUInfoCmdEx(
    unsigned int fd, unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen, 
    unsigned char uchPage, unsigned short usAppDestnId, char fUsrBuff);        

char RsrvCmdEx(
    unsigned int fd, unsigned char uchPty, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);

char VersionCmdEx(
    unsigned int fd, unsigned char uchVerInfo, unsigned char **ppAvcFrame,
    unsigned int *pAvcFrameLen,unsigned short usAppDestnId, char fUsrBuff);

char PlgInfoCmdEx(
    unsigned int fd,unsigned char uchSubFn, unsigned char uchSuType, 
    unsigned char uchSuId,unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);

char ChUsageCmdEx(
    unsigned int fd,unsigned char uchIsoChanNo, unsigned char uchCmdType, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId, 
    char fUsrBuff);

char ConnectCmdEx(
    unsigned int fd,unsigned char uchCmdType, unsigned char uchLockBit, 
    unsigned char uchSrcSuType, unsigned char uchSrcSuId, unsigned char uchSrcPlug, 
    unsigned char uchDestSuType, unsigned char uchDestSuId, unsigned char uchDestPlug, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId, 
    char fUsrBuff);

char ConnectAVCmdEx(
    unsigned int fd,unsigned char uchCmdType, unsigned char uchAvPlgType, 
    unsigned char uchVideoSrc, unsigned char uchAudioSrc, unsigned char uchVideoDest, 
    unsigned char uchAudioDest, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestId, char fUsrBuff);

char ConnectionsCmdEx(
    unsigned int fd,unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);

char DigInputCmdEx(
    unsigned int fd, unsigned char uchCmdType, unsigned char ucConState, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId, 
    char fUsrBuff);

char DigOutputCmdEx(
    unsigned int fd, unsigned char uchCmdType,  unsigned char ucConState, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId, 
    char fUsrBuff);

char DisConnectCmdEx(
    unsigned int fd,unsigned char uchSrcSuType, unsigned char uchSrcSuId, 
    unsigned char uchSrcPlug, unsigned char uchDestSuType, unsigned char uchDestSuId, 
    unsigned char uchDestPlug, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);

char DisConnectAVCmdEx(
    unsigned int fd,unsigned char uchAvPlgType, unsigned char uchVideoSrc, 
    unsigned char uchAudioSrc, unsigned char uchVideoDest, unsigned char uchAudioDest, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestId, 
    char fUsrBuff);

char InpSigCmdEx(
    unsigned int fd,unsigned char uchPlgIndex,unsigned char uchCmdType, 
    unsigned char uchSigFormat, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);

char OutSigCmdEx(
    unsigned int fd,unsigned char uchPlgIndex,unsigned char uchCmdType, 
    unsigned char uchSigFormat, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);

char GenPlgBusCmdEx(
    unsigned int fd,unsigned char uchCmdType,unsigned char uchPlgData[12], int len, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, unsigned short usAppDestnId, 
    char fUsrBuff);
    
//VCR command
/*----------------------------------------------------------------*/  
char AVC_VCR_RecTimeCmdEx(
    unsigned int fd, unsigned char uchFunc, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usDestnId, char fUsrBuff);
        
char AVC_VCR_RecDateCmdEx(
    unsigned int fd, unsigned char uchFunc, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usDestnId, char fUsrBuff);
        
char AVC_VCR_EDIT_MODECmdEx(
    unsigned int fd,unsigned char  Operand,unsigned char uchSuType,
    unsigned char uchSuId,unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);    
    
char AVC_VCR_PRESETCmdEx(
    unsigned int fd, unsigned char uchCmdType,unsigned char  Operand,
    unsigned char *auchOperand, unsigned char uchSuType,unsigned char uchSuId,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId,
    char fUsrBuff);
    
char AVC_VCR_INPUTSIGNALMODECmdEx(
    unsigned int fd, unsigned char uchSuType, unsigned char uchSuId,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId, char fUsrBuff);
    
char AVC_VCR_LoadMediumCmdEx(
    unsigned int fd, unsigned char uchSubFunc, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId, char fUsrBuff);
    
char AVC_VCR_TransportStateCmdEx(
    unsigned int fd, unsigned char uchCmdType, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId,char fUsrBuff);    
    
char AVC_VCR_TimeCodeCmdEx(
    unsigned int fd, unsigned char uchFunc, unsigned char uchCmdType,
    unsigned char *auchOperand, unsigned short usOperandLen, unsigned char uchSuType,
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,
    unsigned short usAppDestnId,char fUsrBuff);      

char AVC_VCR_RecordCmdEx(
    unsigned int fd, unsigned char uchRecordingMode, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId,char fUsrBuff);

char AVC_VCR_PlayCmdEx(
    unsigned int fd, unsigned char uchPlaybackMode, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId,char fUsrBuff);

char AVC_VCR_WindCmdEx(
    unsigned int fd, unsigned char uchFunc, unsigned char uchSuType, 
    unsigned char uchSuId, unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen, 
    unsigned short usAppDestnId,char fUsrBuff);    

char AVC_VCR_BackwardCmdEx(
    unsigned int fd, unsigned char uchMeasureUnit, unsigned char *auchOperand, 
    unsigned short usOperandLen, unsigned char uchSuType,unsigned char uchSuId, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId,
    char fUsrBuff);
    
char AVC_VCR_ForwardCmdEx(
    unsigned int fd, unsigned char uchMeasureUnit, unsigned char *auchOperand, 
    unsigned short usOperandLen, unsigned char uchSuType,unsigned char uchSuId, 
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId,
    char fUsrBuff);
    
/*----------------------------------------------------------------*/
//AVHDD command

char AVC_AVHDD_PlayCmdEx(
    unsigned int fd, unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId,
    char fUsrBuff);

char AVC_AVHDD_StopCmdEx(
    unsigned int fd, unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame,unsigned int *pAvcFrameLen,unsigned short usAppDestnId,
    char fUsrBuff); 
       
char AVC_AVHDD_SetPlugAssociationCmdEx(
    unsigned int fd, unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen, unsigned short usAppDestnId,
    char fUsrBuff);    
    
char AVC_AVHDD_SendCmdEx(
    unsigned int fd, unsigned char uchCmdType,unsigned char uchOpcode,
    unsigned char uchOperand0,unsigned char *auchOperand, unsigned short usOperandLen,
    unsigned char **ppAvcFrame, unsigned int *pAvcFrameLen, unsigned short usAppDestnId,
    char fUsrBuff);
#endif /*AVC_UAPI_H */    
