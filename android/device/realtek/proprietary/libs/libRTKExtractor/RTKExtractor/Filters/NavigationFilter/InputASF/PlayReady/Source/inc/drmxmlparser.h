/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/


#ifndef __DRMXMLPARSER_H__
#define __DRMXMLPARSER_H__

#include <drmcommon.h>
#include <drmresults.h>
#include <drmtypes.h>
#include <drmlicense.h>
#include <drmhmac.h>

ENTER_PK_NAMESPACE;

/* 
** UNICODE  
*/

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetAndDecryptNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,
    IN  PRIVKEY                *f_pPrivkey,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetAndVerifyNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,
    IN  const PUBKEY           *f_pPubkey,
    IN  DRM_BOOL               f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData );

#define DRM_XML_HASH_INCLUDE_TAGS   TRUE
#define DRM_XML_HASH_EXCLUDE_TAGS   FALSE

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetAndVerifyKeyedHashNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,
    IN  DRM_HMAC_CONTEXT       *f_pHmacContext,
    IN  DRM_BYTE               *f_pbHashkey, 
    IN  DRM_DWORD              f_cbHashkey, 
    IN  DRM_BOOL               f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetNodeAttribute(
    IN  const DRM_CONST_STRING *f_pdstrXMLNode,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    OUT DRM_CONST_STRING       *f_pdstrAttrValue );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetNodeCData(
    IN  const DRM_CONST_STRING *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrCData );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,        
    OUT DRM_CONST_STRING       *f_pdstrXMLNode, 
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    IN  DRM_DWORD              f_iLayer );    

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetAndDecryptSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode, 
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,
    IN  PRIVKEY                *f_pPrivkey,      
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,  
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    IN  DRM_DWORD              f_iLayer );   

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetAndVerifySubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,  
    IN  const PUBKEY           *f_pPubkey,         
    IN  DRM_BOOL               f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,    
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    IN  DRM_DWORD              f_iLayer );


DRM_API DRM_RESULT DRM_CALL DRM_XML_GetAndVerifyKeyedHashSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD        f_iNode,        
    IN  DRM_HMAC_CONTEXT       *f_pHmacContext,
    IN  DRM_BYTE               *f_pbHashkey,   
    IN  DRM_DWORD              f_cbHashkey,    
    IN  DRM_BOOL               f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData, 
    IN  DRM_DWORD              f_iLayer );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetSubNodeByPath(
    IN const DRM_CONST_STRING *f_pdstrXML, 
    IN const DRM_CONST_STRING  *f_pdstrXMLNodeString, 
    IN const DRM_CONST_STRING  *f_pdstrAttrName, 
    IN const DRM_CONST_STRING  *f_pdstrAttrValue,
    OUT      DRM_CONST_STRING  *f_pdstrResultXML, 
    OUT      DRM_CONST_STRING  *f_pdstrResultXMLData,
    IN       DRM_WCHAR         f_chSeparator );


DRM_API DRM_RESULT DRM_CALL DRM_XML_EnumNextNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    OUT DRM_CONST_STRING       *f_pdstrTag,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    OUT DRM_CONST_STRING       *f_pdstr1stAttrName, 
    OUT DRM_CONST_STRING       *f_pdstr1stAttrValue, 
    OUT DRM_BOOL               *f_pfIsLeafNode );

DRM_API DRM_RESULT DRM_CALL DRM_XML_EnumNextSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    OUT DRM_CONST_STRING       *f_pdstrTag,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    OUT DRM_CONST_STRING       *f_pdstr1stAttrName,
    OUT DRM_CONST_STRING       *f_pdstr1stAttrValue,
    OUT DRM_BOOL               *f_pfIsLeafNode,
    IN  DRM_DWORD              f_iLayer );

/* 
** ANSI
*/

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetNodeA(
    __in_ecount( f_padstrXML->m_ich + f_padstrXML->m_cch ) const DRM_CHAR *f_pszBase,
    IN  const DRM_SUBSTRING         *f_padstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_padstrTag,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrName,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrValue,
    IN  const DRM_DWORD             f_iNode,
    OUT DRM_SUBSTRING               *f_psubstrXMLNode,
    OUT DRM_SUBSTRING               *f_psubstrXMLNodeData );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetSubNodeA(
    __in_ecount( f_padstrXML->m_ich + f_padstrXML->m_cch ) const DRM_CHAR *f_pszBase,
    IN  const DRM_SUBSTRING         *f_padstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_padstrTag,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrName,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrValue,
    IN  const DRM_DWORD             f_iNode,
    OUT DRM_SUBSTRING               *f_psubstrXMLNode,
    OUT DRM_SUBSTRING               *f_psubstrXMLNodeData,
    IN  DRM_DWORD                    f_iLayer );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetNodeAttributeA(
    __in_ecount( f_padstrXML->m_ich + f_padstrXML->m_cch ) const DRM_CHAR *f_pszBase,
    IN  const DRM_SUBSTRING         *f_padstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrName,
    OUT DRM_SUBSTRING               *f_psubstrAttrValue );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetNodeCDataA(
    __in_ecount( f_pdasstrXML->m_ich + f_pdasstrXML->m_cch ) const DRM_CHAR *f_pszXMLBase,
    IN  const DRM_SUBSTRING *f_pdasstrXML,
    OUT DRM_SUBSTRING       *f_pdasstrCData );

DRM_API DRM_RESULT DRM_CALL DRM_XML_GetSubNodeByPathA(
    __in_ecount( f_pdasstrXML->m_ich + f_pdasstrXML->m_cch ) const DRM_CHAR *f_pszBase,
    IN  const DRM_SUBSTRING         *f_pdasstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_pdastrNodePath,
    IN  const DRM_ANSI_CONST_STRING *f_pdastrAttrName,
    IN  const DRM_ANSI_CONST_STRING *f_pdastrAttrValue,
    OUT DRM_SUBSTRING               *f_pdasstrXMLNodeOut,
    OUT DRM_SUBSTRING               *f_pdasstrXMLDataOut,
    IN  DRM_CHAR                    chSeparator );

DRM_API DRM_RESULT DRM_CALL DRM_XML_EnumNextNodeA(
    __in_ecount( f_pdasstrXML->m_ich + f_pdasstrXML->m_cch ) const DRM_CHAR *f_pszBase,
    IN  const DRM_SUBSTRING *f_pdasstrXML,
    IN  const DRM_DWORD     f_iNode,
    OUT DRM_SUBSTRING       *f_pdasstrTag,
    OUT DRM_SUBSTRING       *f_pdasstrNode,
    OUT DRM_SUBSTRING       *f_pdasstrNodeData,
    OUT DRM_SUBSTRING       *f_pdasstr1stAttrName,
    OUT DRM_SUBSTRING       *f_pdasstr1stAttrValue );

DRM_API DRM_RESULT DRM_CALL DRM_XML_CountMatchingNodesA(
    __in_ecount( f_pdasstrXML->m_ich + f_pdasstrXML->m_cch ) const DRM_CHAR *f_pszBase,
    IN  const DRM_SUBSTRING         *f_pdasstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_pdasstrTag,
    IN  DRM_ANSI_CONST_STRING       *f_pdasstr1stAttrName,
    IN  DRM_ANSI_CONST_STRING       *f_pdasstr1stAttrValue,
    OUT DRM_DWORD                   *f_cMatchingNodes );

DRM_API DRM_BOOL DRM_CALL DRM_XML_Validate(
    IN const DRM_CONST_STRING *f_pdstrXML );

EXIT_PK_NAMESPACE;

#endif      /* __DRMXMLPARSER_H__ */



