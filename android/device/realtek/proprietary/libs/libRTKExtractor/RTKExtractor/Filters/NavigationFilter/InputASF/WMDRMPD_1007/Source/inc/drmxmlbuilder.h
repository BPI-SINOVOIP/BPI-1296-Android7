/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMXMLBUILDER_H__
#define __DRMXMLBUILDER_H__


#include <drmcommon.h>
#include <drmresults.h>
#include <drmtypes.h>
#include <drmlicense.h>
#include <drmhmac.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CB_XMB_EXTRA              100
#define CCH_CDATA_EXTRA           12
#define CCH_CDATA_REQUIRED(pdstr) ((pdstr)->cchString+CCH_CDATA_EXTRA)


#define DRM_SIMXMLDOC_BUFFSIZE    192
#define DRM_XMB_OVERHEAD          (SIZEOF(_XMBContext) + CB_XMB_EXTRA)


extern const DRM_DWORD g_cbXMLBuilderMinimum;


typedef struct _tagXMBContext
{
    /*
    ** Usage of buffer:
    ** This context buffer is allocated by user at CreateDocument(). Its size  can be reallocated
    ** via ReallocDocument(). The XML string is built on the way when each of the API calls are
    ** made. The XML string grow from top to bottom. On the way, all opened node's open tag 
    ** position is kept and saved on the buffer from bottom to top. So when wNextStringPos
    ** and wNextOpenNodePos collides, NO_MEM is returned and user should reallocate more
    ** memory to finish the operation. 
    ** Note: The open nodes positions will be saved as DWORDs, so it aligns perfectly with the 
    ** growing XML string within the WCHAR buffer.
    */

    /* !!! Caller is ensuring only 2 byte alignment. Never add any member variable greater than 2 bytes*/
    
    DRM_BOOL fInited;
    DRM_BOOL fIsSimMode;        /* TRUE to create a faked doc to calculate the length needed */
    DRM_DWORD wMaxStackUsed;    /* Max stack space used for nested nodes */
    DRM_DWORD wSize;            /* size of context */
    DRM_DWORD wNextOpenNodePos; 
    DRM_DWORD wNextStringPos;   /* next insertion point */
    DRM_DWORD wBuffSize;        /* size of buffer in WCHARs*/
    DRM_WCHAR XmlString [1];
} _XMBContext;




/* state to be used for save/restore XML context during API calls */
typedef struct _tagXMBState
{
    DRM_DWORD cwchMaxStackUsed;    /* Max stack space used for nested nodes */
    DRM_DWORD iwchNextOpenNodePos; 
    DRM_DWORD iwchNextStringPos;   /* next insertion point */
} XMBState;


/*
*******************************************************************************
** API methods
*******************************************************************************
*/

DRM_RESULT DRM_API DRM_XMB_SimulateCreateDocument(
    IN const DRM_DWORD         cbXmlContext,
    IN OUT   _XMBContext      *pbXmlContext,
    IN const DRM_CONST_STRING *pdstrRootNodeName);

DRM_RESULT DRM_API DRM_XMB_CreateDocument(
    IN const DRM_DWORD         cbXmlContext,
    IN OUT   _XMBContext      *pbXmlContext,
    IN const DRM_CONST_STRING *pdstrRootNodeName);

DRM_RESULT DRM_API DRM_XMB_ReallocDocument(
    IN       _XMBContext *pbOldXmlContext,
    IN const DRM_DWORD    cbNewXmlContext,
    IN OUT   _XMBContext *pbNewXmlContext );
 
/**********************************************************************
** Function:    DRM_XMB_CloseDocument
** Synopsis:    Pop the opened nodes and close the document, if space allowed.
** Arguments:   
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_XMB_CloseDocument(
    IN OUT _XMBContext *pbXmlContext,
       OUT DRM_STRING  *pdstrXML);   /* if opened with DRM_XMB_SimulateCreateDocument, this contains the counts of WCHAR's needed for the real string */

DRM_RESULT DRM_API DRM_XMB_OpenNode(
    IN OUT   _XMBContext      *pbXmlContext,
    IN const DRM_CONST_STRING *pdstrNodeName);

DRM_RESULT DRM_API DRM_XMB_SaveState(
    IN OUT   _XMBContext *pbXmlContext,
       OUT   XMBState    *pState);

DRM_RESULT DRM_API DRM_XMB_RestoreState(
    IN OUT   _XMBContext *pbXmlContext,
    IN       XMBState    *pState);


/**********************************************************************
** Function:    DRM_XMB_CloseCurrNode
** Synopsis:    
** Arguments:   [pbXmlContext] -- 
**              [pdstrXMLFragment] -- XML fragment of the current node, optional.
** Returns:     DRM_SUCCESS on success
** Notes:       
**********************************************************************/
DRM_RESULT DRM_API DRM_XMB_CloseCurrNode(
    IN OUT _XMBContext *pbXmlContext,
       OUT DRM_STRING  *pdstrXMLFragment);    /* optional: XML fragment of the current node, optional. */

/**********************************************************************
** Function:    DRM_XMB_EncryptAndCloseCurrNode
** Synopsis:    Encrypt and Base64 encode the node (not including the tag)
** Arguments:   [pXmlContext] -- 
**              [pdstrXMLFragment] -- XML fragment of the current node, optional.
** Returns:     DRM_SUCCESS on success
** Notes:       encryption does not include the tag
***********************************************************************/
DRM_RESULT DRM_API DRM_XMB_EncryptAndCloseCurrNode(
    IN OUT   _XMBContext        *pXmlContext,
    IN       DRM_CRYPTO_CONTEXT *pCryptoContext,    /* Pointer to DRM_CRYPTO_CONTEXT */
    IN const PUBKEY             *pPubkey,           /* pub key to use for encrypt */
       OUT   DRM_STRING         *pdstrXMLFragment); /* optional: XML fragment of the current node, optional. */

/**********************************************************************
** Function:    DRM_XMB_SignAndCloseCurrNode
** Synopsis:    Encrypt and Base64 encode the node (not including the tag)
** Arguments:   [pbXmlContext] -- 
**              [pdstrXMLFragment] -- XML fragment of the current node, optional.
** Returns:     DRM_SUCCESS on success
** Notes:       
***********************************************************************/
DRM_RESULT DRM_API DRM_XMB_SignAndCloseCurrNode(
    IN OUT   _XMBContext        *pbXmlContext,
    IN       DRM_CRYPTO_CONTEXT *pCryptoContext,  /* Pointer to DRM_CRYPTO_CONTEXT */
    IN const PRIVKEY            *pPrivkey,        /* pub key to use for encrypt */
    IN       DRM_BOOL            fIncludeTag,
       OUT   DRM_BYTE            rgbSign    [__CB_DECL(PK_ENC_SIGNATURE_LEN)],       /* cannot be both NULL */
       OUT   DRM_WCHAR           wszB64Sign[PK_ENC_SIGNATURE_B64LEN], /* cannot be both NULL */
       OUT   DRM_STRING         *pdstrXMLFragment); /* optional: XML fragment of the current node, optional. */


DRM_RESULT DRM_API DRM_XMB_KeyedHashAndCloseCurrNode(
    IN OUT   _XMBContext        *pbXmlContext,
    IN       HMAC_CONTEXT       *pHmacContext,     /* HMAC context */
    IN const DRM_BYTE           *pbHashkey,        /* Hash key for HMAC */
    IN       DRM_DWORD           cbHashkey,        /* byte count of HMAC */
    IN       DRM_BOOL            fIncludeTag,
    OUT      DRM_BYTE            rgbSign    [__CB_DECL(SHA_DIGEST_LEN)],       /* cannot be both NULL */
    OUT      DRM_WCHAR           wszB64Sign[SHA_B64ENC_DIGEST_LEN], /* cannot be both NULL */
    OUT      DRM_STRING         *pdstrXMLFragment); /* optional: XML fragment of the current node, optional. */


DRM_RESULT DRM_API DRM_XMB_GetCurrNodeName(
    IN  _XMBContext *pbXmlContext,
    OUT DRM_STRING  *pdstrNodeName);

DRM_RESULT DRM_API DRM_XMB_GetContextSize(
    IN  _XMBContext *pbXmlContext,
    OUT DRM_DWORD   *pcbXmlContext);

DRM_RESULT DRM_API DRM_XMB_AddAttribute(
    IN OUT   _XMBContext      *pXmlContext,
    IN const DRM_CONST_STRING *pdstrAttrName,
    IN const DRM_CONST_STRING *pdstrAttrValue );

DRM_RESULT DRM_API DRM_XMB_AddData(
    IN OUT   _XMBContext      *pXmlContext,
    IN const DRM_CONST_STRING *pdstrData );
 
DRM_RESULT DRM_API DRM_XMB_AddCData(
    IN OUT   _XMBContext      *pXmlContext,
    IN const DRM_CONST_STRING *pdstrCData );

/**********************************************************************
** Function:    DRM_XMB_AppendNode
** Synopsis:    Appends a caller provided node nested from current opened node
** Arguments:   [pdwStamp] -- 
** Returns:     DRM_SUCCESS on success
** Notes:       The caller is responsible for the validity of the appending node
***********************************************************************/
DRM_RESULT DRM_API DRM_XMB_AppendNode(
    IN OUT   _XMBContext      *pXmlContext,
    IN const DRM_CONST_STRING *pdstrXmlString);


/*******************************************************************
 *          PUBLIC FUNCTION DRM_XMB_RequiredCharsForTag
 *
 * purpose: calculates the number of XML characters required for
 *          the indicated tag, optional text, and optional 
 *          attribute label and text
 ******************************************************************/

DRM_DWORD DRM_API DRM_XMB_RequiredCharsForTag (
    IN const DRM_DWORD cchTag,
    IN const DRM_DWORD cchData,
    IN const DRM_DWORD cchAttrLabel,
    IN const DRM_DWORD cchAttrText);

enum WriteTagType
{
    wttOpen,
    wttClosed
};

/*********************************************************************
 *                 PUBLIC FUNCTION DRM_XMB_WriteTag
 *                                                                   
 *      function: write one or both tags of an XML pair              
 *                  and optionally the enclosed data and/or
 *                  a single attribute
 *
 *      if parameter wtt is wttClosed, the tag is closed and the
 *      XML builder insertion point remains at the same scope as
 *      when the function was called
 *
 *        <TAG[ attrlabel="attrtext"]>[data][</TAG>]
 *
 ********************************************************************/

DRM_RESULT DRM_API DRM_XMB_WriteTag (
    IN OUT       _XMBContext      *pbDataOut,
    IN     const DRM_CONST_STRING *pdstrTag,
    IN     const DRM_CONST_STRING *pdstrData,
    IN     const DRM_CONST_STRING *pdstrAttrLabel,
    IN     const DRM_CONST_STRING *pdstrAttrText,
    IN     enum  WriteTagType      wtt);

/*********************************************************************
 *             PUBLIC FUNCTION DRM_XMB_WriteCDATATag
 *                                                                   
 * as above but writes a CDATA tag
 ********************************************************************/

DRM_RESULT DRM_API DRM_XMB_WriteCDATATag (
    IN OUT       _XMBContext      *pbDataOut,
    IN     const DRM_CONST_STRING *pdstrTag,
    IN     const DRM_CONST_STRING *pdstrCDATA,
    IN     const DRM_CONST_STRING *pdstrAttrLabel,
    IN     const DRM_CONST_STRING *pdstrAttrText,
    IN     enum  WriteTagType      wtt);

DRM_RESULT DRM_API DRM_XMB_RemainingBuffer(
    IN  _XMBContext *f_pbXMB,
    OUT DRM_DWORD   *f_pcbRemaining);

#ifdef __cplusplus
}
#endif

#endif      /* __XMLBUILDER_H__ */


