/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef _HDS_IMPL_H
#define _HDS_IMPL_H

#include <oemimpl.h>
#include <drmmd5.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
** These first few enums are public information.
** They are followed by internal implementation details of the HDS
*/

typedef enum 
{
    eDRM_HDSBLKNUM_WORD  = 2,      /* max of 65536 blocks */
    eDRM_HDSBLKNUM_DWORD = 4       /* max of 4 G blocks */
} DRM_HDSBLKNUM;

typedef enum
{
    eDRM_HDS_CREATE_IF_NEW = 0x1000,      /* create if not exist, if exists, open instead */
    eDRM_HDS_OPEN_EXISTING = 0x2000       /* opened if exist, failed if not */
} DRM_HDS_OPENMODE;


typedef enum
{
    eDRM_HDS_LOCKSHARED    = 0x0,
    eDRM_HDS_LOCKEXCLUSIVE = 0x1,
    eDRM_HDS_LOCKWAIT      = 0x2
} DRM_HDS_LOCKMODE;

typedef enum
{
    eDRM_HDS_SEEKCUR = 1,
    eDRM_HDS_SEEKEND,
    eDRM_HDS_SEEKSET
} DRM_HDS_SEEKMODE;

typedef DRM_KID DRM_HDS_HASHKEY;
typedef DRM_LID DRM_HDS_UNIQUEKEY;

/*
** uncomment the following if do not need DRM_HDS_InitSlotEnum() and DRM_HDS_SlotEnumNext() 
*/
/* #define _NOENUMALL */


/*
** Note: this is implementation specific. 
*/
#define CONTEXT_LENGTH    3072
#define MAXSLOTPOS        0xFFFF
#define ROOTPARENTNUM     0x00


#define DRM_E_HDSINVALIDSTORE     DRM_E_INVALIDLICENSESTORE
#define DRM_E_HDSSRNCORRUPTED     DRM_E_HDSFILECORRUPTED
#define DRM_E_HDSNAMESPACEEXIST     DRM_E_FAIL

#define DRM_HDS_HASHKEY_SIZE        DRM_ID_SIZE
#define DRM_HDS_UNIQUEKEY_SIZE      DRM_ID_SIZE
#define DRM_HDS_NSNAME_SIZE 16

/* create a namespace tree in the data store */
#define DRM_HDS_MAXIMUM_CHILDREN 16



/*
** Useful macros used in this source file only
*/
#define HDS_CONTEXT_LENGTH(srnsize)    \
    (SIZEOF(_HdsContext) - SIZEOF(_SRN) + (srnsize) + CONTEXT_LENGTH)

#define ISVALIDCONTEXT(ctx, eSignature) \
    ((ctx)!=NULL && (((ctx)->eContextSignature==(eSignature))?TRUE : FALSE))

#define CALC_SLOTCONTEXTLEN(cfg) \
    (SIZEOF(_SlotContext)+GetMemsize_ChildBlock((cfg))+GetMemsize_DataBlock())

/* There cound be an integer overflow, as pointed out by PREfast */
#define ChkOverflowSLOTCONTEXTLEN(cfg) \
    ChkOverflow( CALC_SLOTCONTEXTLEN(cfg), SIZEOF(_SlotContext) + SIZEOF(_ChildBlockHDR) + GetMemsize_DataBlock() );
    
#define CALC_MAXSLOTCONTEXTLEN \
    (SIZEOF(_SlotContext)+GetMaxMemsize_ChildBlock()+GetMemsize_DataBlock())

#define CALC_MAXENUMCONTEXTLEN \
    (SIZEOF(_EnumContext)+GetMaxMemsize_ChildBlock()-SIZEOF(_CommBlockHDR))

#define CALC_MAXBLOCKSCANCONTEXTLEN \
    (SIZEOF(_BlockScanContext)+GetMaxMemsize_ChildBlock()-SIZEOF(_CommBlockHDR))

/*
**  NOTE:
**  -Child Node number = (Fileposition / NodeSize) + 1
**  -Node number of the RootNode is always 0
**  -Uses DRM_USHORT for Nodenum gives roughly 64K nodes in a file. On average, if a node
**   can store 5 licenses, a license file can store up to 320K licenses.
**  -If a larger number of licneses need to be stored, DRM_DWORD can be used. This will gives
**   4096M nodes or 20480M licenses.
**  -to search a node containing a particular license, we only have to traverse Log(N) times where
**   N is the number of licenses in the file.
*/




/******************************************************************************
** Major implementation data structure
*******************************************************************************
*/


/* 
** Block Type 
*/
typedef enum 
{
    eINVALIDBLOCK = 0x00,
    eCHILDBLOCK,
    eDATABLOCK,
    eFREEBLOCK
} _EBlockType;

typedef enum
{
    eHdsContextSignature  = 0x01,
    eCfgContextSignature  = 0x02,
    eSlotContextSignature = 0x03,
    eEnumContextSignature = 0x04
} _EContextSignature;


/*
**
*/
typedef enum
{
    eSlotCtxUninit = 0,
    eSlotCtxInited,
    eSlotCtxReady
} _ESlotContextStatus;


typedef enum
{
    eSearchSlotDontCare,
    eSearchDataSlot,
    eSearchNamespace,
    eSearchHiddenOnly
} _ESlotSearchMode;


typedef enum
{
    eRemoveSlotSetHidden = 1,
    eRemoveSlotSetNull   = 2,
    eRemoveSlotPermanent = 3
} _ERemoveSlotMode;

/*
** Namespace entry in SRN's namespace alloc table
*/
typedef struct _tag_NSEntry
{
    DRM_WORD  wMaxNumChildren;
    DRM_WORD  wPadding;
    DRM_DWORD nNSRBlockNum;                  /* block # of the Namespace Root node */
    DRM_BYTE  bNSName [__CB_DECL(DRM_HDS_NSNAME_SIZE)]; /* Namespace name */
} _NSEntry;


/*
** Super Root Node
*/
typedef struct _tag_SRN
{
    DRM_UINT64    ui64Timestamp;    /* updates every time SRN is updated */
    DRM_DWORD     dwSRNSize;        /* super root node size */
    DRM_DWORD     dwBlockSize;      /* File block size */
    DRM_DWORD     eBlockNumType;    /* DRM_HDSBLKNUM */
    DRM_DWORD     nFreeListHead;
    DRM_DWORD     nNsStoreRootBlockNum;
    DRM_DWORD     nHighestFormattedBlockNum;  /* block num of the highest formated block */
    DRM_BYTE      bSRNHash [__CB_DECL(MD5DIGESTLEN)];    /* MD5 hash value of SRN */
} _SRN;

/*
** HDS context
*/
typedef struct _tag_Context
{
    DRM_BOOL fCSInited;
    OEM_CRITICAL_SECTION oCS;
    DRM_DWORD   dwContextSize;
    DRM_BOOL    fInited;
    DRM_DWORD   eContextSignature;
    OEM_FILEHDL fp;
    DRM_MD5_CTX contextMD5;
    DRM_DWORD   nGrowbySizeinKB;

    /* computed when HDS is 'opened' */
    DRM_DWORD   nImagesize_FileBlockHDR;     /* file image size of _CommBlockHDR, not including payload */
    DRM_DWORD   nImagesize_DataBlockHDR;     /* file image size of _DataBlockHDR, not including payload */
    DRM_DWORD   nImagesize_DataBlockPayload; /* payload size of a _DataBlockHDR */
    DRM_DWORD   nDataBlockPayloadPos;        /* payload pos from begining of a _DataBlockHDR */

    /* work buffer initialized when HDS is opened */
    DRM_STACK_ALLOCATOR_CONTEXT oHeap;

    _SRN oSRN;    /* super root node, pbStack points to the area after oSRN */
} _HdsContext;

/*
** HDS Config context
*/
typedef struct _tag_CfgContext
{
    DRM_DWORD wContextSize;
    DRM_DWORD eContextSignature;
    DRM_BOOL fInited;
    _HdsContext *pHDS;
    DRM_BYTE oNamespace[__CB_DECL(DRM_HDS_NSNAME_SIZE)];

    /* computed when a Namespace is 'used' */
    DRM_DWORD nCfgRootBlockNum;    
    DRM_DWORD wMaxNumChildren;                /* # of children of the current selected Namespace */
    DRM_DWORD nImagesize_ChildBlockHDR;       /* file image size of _ChildBlockHDR, not including payload */
    DRM_DWORD nImagesize_ChildAllocTable;     /* file image size of ChildAlloc table of _ChildBlockHDR and _NSRootBlock */
    DRM_DWORD nImagesize_ChildBlockPayload;   /* payload size of _ChildBlockHDR  */
    DRM_DWORD nChildBlockPayloadPos;          /* payload pos from begining of _ChildBlockHDR */
} _NsContext;


#define Memsize_FileBlock  SIZEOF(_CommBlockHDR)
typedef struct _tag_GenericBlockHeader {
    DRM_UINT64  ui64TimeStamp;
    DRM_BYTE    bBlockHash [__CB_DECL(MD5DIGESTLEN)];   /* MD5 hash of the above */
    DRM_DWORD   nParentBlockNum;
    DRM_BYTE    bBlockType;    
} _CommBlockFileHDR;    /*_FileImage; */

typedef struct _tag_FileBlock
{
    /* computed at run time */
    DRM_DWORD nBlockNum;   
    _NsContext *pNS;

    /* read from file */
    union 
    {        
        _CommBlockFileHDR _image;
        DRM_BYTE bFileImage[__CB_DECL(SIZEOF( _CommBlockFileHDR ))];
    } File;
} _CommBlockHDR;


typedef struct _tag_ChildBlockHeader 
{
    DRM_DWORD nFreeMem;
    DRM_BYTE bChildAllocTable [1];
} _ChildBlockFileHDR;


#define GetMemsize_ChildBlock(cfg)    (SIZEOF(_ChildBlockHDR)+(cfg)->nImagesize_ChildAllocTable)
#define GetMaxMemsize_ChildBlock()    (SIZEOF(_ChildBlockHDR)+(DRM_HDS_MAXIMUM_CHILDREN*eDRM_HDSBLKNUM_DWORD))
typedef struct _tag_ChildBlock
{
    _CommBlockHDR oBlockHeader;    /* derive from _CommBlockHDR */

    /* calculated during init'd */
    DRM_DWORD nCurrSlotPos;
    DRM_DWORD nNextSlotPos;
    DRM_DWORD nPayloadSize;
    union 
    {
        DRM_BYTE bFileImage[__CB_DECL(SIZEOF(_ChildBlockFileHDR))];
        _ChildBlockFileHDR _image;
    } File;
} _ChildBlockHDR;


typedef struct _tag_DataBlockHeader {
    DRM_DWORD nCascadingBlockNum;
} _DataBlockFileHDR;


#define GetMemsize_DataBlock()   SIZEOF(_DataBlockHDR)
typedef struct _tag_DataBlock
{
    _CommBlockHDR oBlockHeader;    /* derive from _CommBlockHDR */

    DRM_DWORD nPayloadSize;
    union 
    {
        DRM_BYTE bFileImage[__CB_DECL(SIZEOF(DRM_DWORD))];
        _DataBlockFileHDR _image;
    } File;
} _DataBlockHDR;



typedef enum
{
    eSlotIsNamespace = 0x00000001,
    eSlotIsHidden    = 0x00000002
} _ESlotAtributes;

/*
** record header structure precedes the actual storage of data record
*/
typedef struct _tag_SlotHeader
{
    DRM_DWORD dwSlotSize;           /* actual Data Record Size */
    DRM_DWORD dwAttributes;         /* Attributes for this slot (see _ESlotAtributes) */
    DRM_HDS_HASHKEY oHashkey; 
    DRM_HDS_UNIQUEKEY oUniquekey; 
} _SlotHeader;


/*
** License FindFirst /  FindNext context
*/
typedef struct _tag_SlotContext
{
    DRM_DWORD dwContextSize;
    DRM_DWORD eContextSignature;
    DRM_DWORD eStatus;
    _NsContext *pNS;
    DRM_HDS_LOCKMODE eLockMode;

    /* cache of search result for slot allocation */
    DRM_DWORD nBlockWithLargestFreeSpace;
    DRM_DWORD dwLargestSpace;

    _CommBlockHDR oFileBlock;      /* a copy of the file block in _ChildBlockHDR for verification purpose */
    _CommBlockHDR *pCurrChildBlock; /* pointer of the _ChildBlockHDR containing the Slot, pointing somewhere in bBuff */
    DRM_DWORD dwSlotPosInBlock;   /* the position in ChildBlock's payload where the slot is located (at begining _SlotHeader ) */
    _SlotHeader oSlotHeader;    /* Slot header record in _ChildBlockHDR */
    DRM_DWORD dwSeekPointer;      /* the current position in Slot where next slot read/write occurs */

    /* the following is valid if slot data is stored in _DataBlockHDR chain */
    DRM_DWORD dwNumofDataBlockVisited;  /* num of _DataBlockHDR visited not including current one */
    DRM_DWORD dwFirstDataBlockNum;    /* first _DataBlockHDR number in the chain */
    _CommBlockHDR *pCurrDataBlock;

    /* buffer to contain _ChildBlockHDR and _DataBlockHDR */
    DRM_BYTE bBuff [1];  /* size = dynamic size of _ChildBlockHDR + _DataBlockHDR */
} _SlotContext;


/*
** Slot Enum context
** this structure's size must be greater than or equal to CALC_MAXENUMCONTEXTLEN 
*/
typedef struct _tag_EnumContext
{
    DRM_DWORD wContextSize;
    DRM_DWORD eContextSignature;
    _NsContext *pNS;
    DRM_BOOL fInited;

    _CommBlockHDR oFileBlock;      /* a copy of the file block in _ChildBlockHDR for verification purpose */
    _SlotHeader oSlotHeader;    /* current slot header */

    DRM_HDS_LOCKMODE eLockMode;
    DRM_BOOL fIsQuery;             /* TRUE if oHashKey is not NULL, FALSE if enum all */
    DRM_HDS_HASHKEY oHashKey;  

    DRM_BOOL fCurrBlockIsValid;
    _CommBlockHDR oCurrBlock;  /* this field must be located at end of structure */
} _EnumContext;

typedef struct _tag_BlockScanContext
{
    DRM_DWORD                cbContext;
    DRM_DWORD                eContextSignature;
    _NsContext              *pcontextNS;
    DRM_BOOL                 fInited;
    _CommBlockHDR            blockheader;  /* a copy of the file block in _ChildBlockHDR for verification purpose */
    _SlotHeader              slotheader;   /* current slot header */
    DRM_HDS_LOCKMODE         lockmode;
    DRM_HDS_HASHKEY          keyHash;  
    const DRM_HDS_UNIQUEKEY *pkeyUnique;  /* array of KIDs to delete */
    DRM_DWORD                cKeysUnique; /* size of array */
    DRM_DWORD                cKeysFound;  /* number of KIDs marked for deletion thus far */
    DRM_DWORD                iKeyCurrent; /* index of next unmarked KID in the array */
    DRM_BOOL                 fCurrBlockIsValid;
    _CommBlockHDR            blockheaderCurrent;  /* this field must be located at end of structure */
} _BlockScanContext;

typedef struct _tag_SlotHint
{
    DRM_DWORD nSlotPosInBlock;
    DRM_DWORD nBlockNum;
} _SlotHint;

/* HDS context sizes */
#define DRM_HDS_CONTEXT_LEN HDS_CONTEXT_LENGTH(SIZEOF(_SRN))
#define DRM_HDS_NAMESPACE_CONTEXT_LEN (SIZEOF( _NsContext ))
#define DRM_HDS_SLOT_CONTEXT_LEN       CALC_MAXSLOTCONTEXTLEN
#define DRM_HDS_ENUM_CONTEXT_LEN       CALC_MAXENUMCONTEXTLEN
#define DRM_HDS_BLOCKSCAN_CONTEXT_LEN  CALC_MAXBLOCKSCANCONTEXTLEN
#define DRM_HDS_SLOT_HINT_LEN          (SIZEOF(_SlotHint))

#ifdef __cplusplus
}
#endif


#endif  /* _HDS_IMPL_H */


