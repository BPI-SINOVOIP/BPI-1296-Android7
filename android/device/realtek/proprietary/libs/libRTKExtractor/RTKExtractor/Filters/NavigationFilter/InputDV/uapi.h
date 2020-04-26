#ifndef __UAPI_H__
#define	__UAPI_H__

#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>

#define	ALLOC(ptr, type, nSize, error) {\
											ptr = (type *)malloc(nSize); \
											if(!ptr) \
												return error; \
											memset(ptr, 0, nSize);\
										}
#define	FREE(x)	 						{\
											free(x); \
											x = NULL; \
										}
	

#define	ERROR 		-1
#define	OK 			1
#define	NONE		

/* IMPORTANT NOTE: 
	These must match the definitions in 1394.h
 */

#if 0
typedef int     INT;
typedef char    CHAR;
typedef short   SHORT;
typedef long    LONG;

typedef unsigned int    *PUINT;
typedef unsigned char   * PUCHAR;
typedef unsigned short  * PUSHORT;
typedef unsigned int    UINT;
typedef unsigned char   UCHAR;
typedef unsigned short  USHORT;
typedef unsigned long   ULONG;
#endif

typedef enum
{
FALSE,
TRUE
}BOOLEAN;

typedef int BOOL;
typedef int STATUS;
//typedef void * HANDLE;

/* Address Range Allocation Choice Definitions */
#define ANY_RANGE                       0
#define SPECIFIC_RANGE                  1
#define ABOVE_SPECIFIED_RANGE           2

/* Address Range Access Type Definitions */
#define ACCESS_TYPE_READ                0x1
#define ACCESS_TYPE_WRITE               0x2
#define ACCESS_TYPE_LOCK                0x4

#define LOCK_TRANSACTION_MASK_SWAP          0x1
#define LOCK_TRANSACTION_COMPARE_SWAP       0x2
#define LOCK_TRANSACTION_FETCH_ADD          0x3
#define LOCK_TRANSACTION_LITTLE_ADD         0x4
/*#define LOCK_TRANSACTION_BOUNDED_ADD        0x5*/
#define LOCK_TRANSACTION_BOUNDED_ADD        0x2
#define LOCK_TRANSACTION_WRAP_ADD           0x6

typedef enum _EMI_MODE
{
    EMI_COPY_FREE = 0,
    MODE_C_5C = 1, /*NO_MORE_COPIES*/
    MODE_B_5C = 2, /*COPY_ONE_GENERATION*/
    MODE_A_5C = 3  /*COPY_NEVER*/
}EMI_MODE;

#ifndef __KERNEL__
/* Define Transaction codes to be accessed by the user */
#define TCODE_WRITE_REQUEST_QUADLET			0	
#define TCODE_WRITE_REQUEST_BLOCK			1
#define TCODE_WRITE_RESPONSE				2
#define TCODE_RESERVED1						3
#define TCODE_READ_REQUEST_QUADLET			4
#define TCODE_READ_REQUEST_BLOCK			5
#define TCODE_READ_RESPONSE_QUADLET			6
#define TCODE_READ_RESPONSE_BLOCK			7
#define TCODE_LOCK_REQUEST					9

/* Function Type Definition for Address Range CallBack */
typedef 
void 
(*PBUS_ADDR_CALL_BACK)(
	void * 				pCallbackContext,
	unsigned char				uchSpeed,
	unsigned short				usSrcNodeID,
	unsigned char				uchTCode,
	unsigned char				uchExtendedTcode,
	unsigned short				usOffsetHigh,
	unsigned int				uOffsetLow,
	unsigned int				uNumberOfBytes,
	unsigned char               *pWriteData,
    unsigned char               **pReadData,
    unsigned char				*pResponse_Code
	);
#endif	/* __KERNEL__ */





STATUS
LuclGetCycleTime(
    int fd,
    unsigned char * pCycleTime
    );


STATUS
LuclProgramContentKey(int fd, unsigned char uchEmiType, BOOLEAN Initial,
    BOOLEAN fOddEven, unsigned int uKeyLow, unsigned int uKeyHigh);

STATUS
LuclSetupForAVCSource(int fd, unsigned char uchChannel, BOOLEAN OddEven,
    unsigned char uchEmiMode);

STATUS
LuclSetupForAVCSink(int fd, unsigned char uchChannel);

STATUS
LuclSetupForAVCSourceNoAuth(int fd, unsigned char uchChannel,
    BOOLEAN OddEven, unsigned char uchEmiMode);

STATUS
LuclSetupForAVCSinkNoAuth(int fd, unsigned char uchChannel);


STATUS
LuclTransmitCopyProtectedContentForDemo(int fd);

STATUS
LuclProgramOddEvenBit(int fd, unsigned char Channel,
    BOOLEAN OddEven);

STATUS
LuclInitiateBusReset(int fd);

STATUS
LuclGetBusInfoBlk(int fd, unsigned char *pBuffer, unsigned short nBufferLen);

STATUS
LuclGetGenerationCount(int fd, unsigned int *pGenCnt);

STATUS
LuclGetNodeId(int fd, unsigned short    *pNodeId);

STATUS
LuclGetLocalSPD(int fd, unsigned char   *pLocalSpd);

STATUS
LuclGetIRMId(int fd, unsigned short *pIrmId);

STATUS
LuclGetEuiHi(int fd, unsigned int *pEuiHi);

STATUS
LuclGetEuiLo(int fd, unsigned int *pEuiLo);


STATUS
LuclGetMaxSPDToDestination(int fd, unsigned short usDestId, unsigned char   *pSpdToDest);

#if (HOST_ISOCH_TX || HOST_ISOCH_RX)
STATUS
LuclIsBroadcastChannelSupported(int fd, unsigned char   *pBcastChan);

STATUS
LuclGetBroadcastSpeed(int fd, unsigned char *pBcastSpd);

STATUS
LuclAllocateChannel(int fd, unsigned char   uchChannel, unsigned char *pAllocChan);

STATUS
LuclFreeChannel(int fd, unsigned char uchChannel);

STATUS
LuclAllocateBandwidth(int fd, unsigned short usBandWidth);

STATUS
LuclFreeBandwidth(int fd, unsigned short    usBw);

#endif	/* HOST_ISOCH */

STATUS
LuclGetNumNodes(int fd, int *pCnt);

STATUS
LuclGetDestinationConfigROM(int fd, unsigned short  usDestId, unsigned char *pBuffer, unsigned short usBufferLen);

STATUS
LuclGetDestinationEui(int fd, unsigned short  usDestinationID, unsigned int * pEuiHi, unsigned int * pEuiLo);

STATUS
LuclAppendConfigROM(int fd, unsigned char* pUnitDir,unsigned int uUnitDirLength,
                unsigned int *puiByteOffset, unsigned short *pusLen);

STATUS
LuclRemoveConfigROM(int fd, unsigned int uByteOffset, unsigned short usLen);

STATUS
LuclAsyncRead(int fd,
    unsigned short          usNodeId,
    unsigned short          usOffsetHi,
    unsigned int            uOffsetLo,
    unsigned int            uGenCount,
    unsigned short         usNumOfBytes,
    unsigned char           uchSpeed,
    unsigned char           *pBuffer,
    int             *pStatus);


STATUS
LuclAsyncWrite(int fd,
    unsigned char               *pBuffer,
    unsigned short              usBufferLen,
    unsigned short              usNodeId,
    unsigned short              usOffsetHi,
    unsigned int                uOffsetLo,
    unsigned int                uGenCount,
    unsigned char               uchSpeed,
    int                 *pStatus);


STATUS
LuclAsyncLock(int fd,
    unsigned char               *pLockRequest,
    unsigned char               fuchTransactionType,
    unsigned short              usDataLength,
    unsigned short              usNodeId,
    unsigned short              usOffsetHi,
    unsigned int                uOffsetLo,
    unsigned int                uGenCount,
    unsigned char               uchSpeed,
    int                 *pStatus);


STATUS
LuclProgramPathSel(int fd, unsigned char uchPathSel);

typedef struct AddrRangeUserHandle
{
	unsigned int	uOffsetLo;
	unsigned short	usOffsetHi;
	void 	*handle;
}AddrRangeUserHandle_t;

AddrRangeUserHandle_t *
LuclAllocateAddressRange(
            int                     nFd,
            PBUS_ADDR_CALL_BACK     addrCbk,
            void *                   pCbkContext,
            unsigned short                  usOffsetHigh,
            unsigned int                    uOffsetLow,
            int                     nLen,
            int                     nStatus,
            unsigned char                   fuchFlags,
            unsigned char                   fuchAccessType);

int LuclFreeAddressRange(int fd, AddrRangeUserHandle_t *handle);
int Lucl5C_InitiateAuthentication(int nFd, unsigned short usDestId);

unsigned long ReadReg(int nFd,unsigned long address);
void WriteReg(int nFd,unsigned long address,unsigned long data);
int LuclAppInit(int nFd);
int RingBufferAlloc(int nFd,unsigned long RingBufferlength,unsigned char **str);
int RingBufferFree(int nFd,unsigned long RingBufferlength,unsigned char **str);
int DMAEnable(int nFd,char type,char ch);
void BusConnectionInfo(int nFd,void* info);
//char PollingBusReset(int fd,void* error_signal);
char PollingBusReset(int fd);
char DeleteBusResetThread(int fd);
#endif	/* __UAPI_H__ */
