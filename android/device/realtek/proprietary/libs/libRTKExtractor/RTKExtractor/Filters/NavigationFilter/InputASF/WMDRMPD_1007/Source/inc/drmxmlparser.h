/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
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

#ifdef __cplusplus
extern "C" {
#endif

/* UNICODE  */

/*
** extract license node from given XML string matching <tag AttrName="Value"> ... </tag>
*/
DRM_RESULT DRM_API DRM_XML_GetNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,            /* nth occurence of the node in the xml stirng, start from 0 */
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,       /* either of these 2 parameter can be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData);  /* data enclosed by the immediate <tag>...</tag> in the given XML string */

/*
** extract license node from given XML string matching <tag AttrName="Value"> ... </tag>
*/
DRM_RESULT DRM_API DRM_XML_GetAndDecryptNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,           /* nth occurence of the node in the xml stirng, start from 0 */
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,    /* Pointer to DRM_CRYPTO_CONTEXT */
    IN  PRIVKEY                *f_pPrivkey,          /* priv key to use for decrypt */
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,      /* either of these 2 parameter can be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData); /* data enclosed by the immediate <tag>...</tag> in the given XML string */

/*
** extract license node from given XML string matching <tag AttrName="Value"> ... </tag>
*/
DRM_RESULT DRM_API DRM_XML_GetAndVerifyNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,           /* nth occurence of the node in the xml stirng, start from 0 */
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,    /* Pointer to DRM_CRYPTO_CONTEXT */
    IN  const PUBKEY           *f_pPubkey,           /* pub key to use for verify */
    IN  DRM_BOOL                f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,      /* either of these 2 parameter can be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData); /* data enclosed by the immediate <tag>...</tag> in the given XML string */

#define DRM_XML_HASH_INCLUDE_TAGS   TRUE
#define DRM_XML_HASH_EXCLUDE_TAGS   FALSE

DRM_RESULT DRM_API DRM_XML_GetAndVerifyKeyedHashNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,            /* nth occurence of the node in the xml stirng, start from 0 */
    IN  HMAC_CONTEXT           *f_pHmacContext,       /* HMAC context */
    IN  DRM_BYTE               *f_pbHashkey,          /* Hash key for HMAC */
    IN  DRM_DWORD               f_cbHashkey,          /* byte count of HMAC */
    IN  DRM_BOOL                f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,       /* either of these 2 parameter can be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData);  /* data enclosed by the immediate <tag>...</tag> in the given XML string */

/*
** 
*/
DRM_RESULT DRM_API DRM_XML_GetNodeAttribute(
    IN const DRM_CONST_STRING *f_pdstrXMLNode,    /* the topmost node in this XML string will be worked on */
    IN const DRM_CONST_STRING *f_pdstrAttrName,   /* attrName to retrieve */
    OUT      DRM_CONST_STRING *f_pdstrAttrValue);	/* returned attrValue */

/*
** extract the CDATA section from a given XML node
*/
DRM_RESULT DRM_API DRM_XML_GetNodeCData(
    IN const DRM_CONST_STRING *f_pdstrXMLNode,     /* the topmost node in this XML string will be worked on */
    OUT      DRM_CONST_STRING *f_pdstrCData);      /* returned CData */

/*
** extract license node from given XML string matching <tag AttrName="Value"> ... </tag>
*/
DRM_RESULT DRM_API DRM_XML_GetSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,          /* nth occurence of the node in the xml stirng, start from 0 */
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,     /* these two parameters can either be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    IN  DRM_DWORD               f_iLayer);     /* outermost layer is 0 */

/*
** extract license node from given XML string matching <tag AttrName="Value"> ... </tag>
*/
DRM_RESULT DRM_API DRM_XML_GetAndDecryptSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,        /* nth occurence of the node in the xml stirng, start from 0 */
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext, /* Pointer to DRM_CRYPTO_CONTEXT */
    IN  PRIVKEY                *f_pPrivkey,       /* priv key to use for decrypt */
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,   /* these two parameters can either be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    IN  DRM_DWORD               f_iLayer);   /* outermost layer is 0 */

/*
** extract license node from given XML string matching <tag AttrName="Value"> ... </tag>
*/
DRM_RESULT DRM_API DRM_XML_GetAndVerifySubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,          /* nth occurence of the node in the xml stirng, start from 0 */
    IN  DRM_CRYPTO_CONTEXT     *f_pCryptoContext,   /* Pointer to DRM_CRYPTO_CONTEXT */
    IN  const PUBKEY           *f_pPubkey,          /* pub key to use for verify */
    IN  DRM_BOOL                f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,     /* these two parameters can either be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData,
    IN  DRM_DWORD               f_iLayer);     /* outermost layer is 0 */


DRM_RESULT DRM_API DRM_XML_GetAndVerifyKeyedHashSubNode(
    IN  const DRM_CONST_STRING *f_pdstrXML,
    IN  const DRM_CONST_STRING *f_pdstrTag,
    IN  const DRM_CONST_STRING *f_pdstrAttrName,
    IN  const DRM_CONST_STRING *f_pdstrAttrValue,
    IN  const DRM_DWORD         f_iNode,          /* nth occurence of the node in the xml stirng, start from 0 */
    IN  HMAC_CONTEXT           *f_pHmacContext,     /* HMAC context */
    IN  DRM_BYTE               *f_pbHashkey,        /* Hash key for HMAC */
    IN  DRM_DWORD               f_cbHashkey,        /* byte count of HMAC */
    IN  DRM_BOOL                f_fIncludeTag,
    IN  DRM_CONST_STRING       *f_pdstrB64Signature,
    OUT DRM_CONST_STRING       *f_pdstrXMLNode,     /* either of these 2 parameter can be NULL but not both */
    OUT DRM_CONST_STRING       *f_pdstrXMLNodeData, /* data enclosed by the immediate <tag>...</tag> in the given XML string */
    IN  DRM_DWORD               f_iLayer);     /* outermost layer is 0 */

DRM_RESULT DRM_API DRM_XML_GetSubNodeByPath(
    IN const DRM_CONST_STRING* pdstrXML, 
    IN const DRM_CONST_STRING *f_pdstrXMLNodeString, 
    IN const DRM_CONST_STRING *f_pdstrAttrName, 
    IN const DRM_CONST_STRING *f_pdstrAttrValue,
    OUT      DRM_CONST_STRING *f_pdstrResultXML, 
    OUT      DRM_CONST_STRING *f_pdstrResultXMLData,
    IN       DRM_WCHAR         f_chSeparator);


DRM_RESULT DRM_API DRM_XML_EnumNextNode(
    IN const DRM_CONST_STRING *f_pdstrXML,
       OUT   DRM_CONST_STRING *f_pdstrTag,
       OUT   DRM_CONST_STRING *f_pdstrXMLNode,       /* either of these 2 parameter can be NULL but not both */
       OUT   DRM_CONST_STRING *f_pdstrXMLNodeData,
       OUT   DRM_CONST_STRING *f_pdstr1stAttrName,   /* optional */
       OUT   DRM_CONST_STRING *f_pdstr1stAttrValue,  /* optional */
       OUT   DRM_BOOL         *f_pfIsLeafNode);

DRM_RESULT DRM_API DRM_XML_EnumNextSubNode(
    IN const DRM_CONST_STRING *f_pdstrXML,
       OUT   DRM_CONST_STRING *f_pdstrTag,
       OUT   DRM_CONST_STRING *f_pdstrXMLNode,     /* these two parameters can either be NULL but not both */
       OUT   DRM_CONST_STRING *f_pdstrXMLNodeData,
       OUT   DRM_CONST_STRING *f_pdstr1stAttrName,   /* optional */
       OUT   DRM_CONST_STRING *f_pdstr1stAttrValue,  /* optional */
       OUT   DRM_BOOL         *f_pfIsLeafNode,
    IN       DRM_DWORD         f_iLayer);      /* outermost layer is 0 */

/* ANSI  */

DRM_RESULT DRM_API DRM_XML_GetNodeA(
    IN  const DRM_CHAR              *f_pszBase,
    IN  const DRM_SUBSTRING         *f_padstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_padstrTag,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrName,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrValue,
    IN  const DRM_DWORD              f_iNode,             /* nth occurence of the node in the xml stirng, start from 0 */
    OUT DRM_SUBSTRING               *f_psubstrXMLNode,      /* either of these 2 parameter can be NULL but not both */
    OUT DRM_SUBSTRING               *f_psubstrXMLNodeData); /* data enclosed by the immediate <tag>...</tag> in the given XML string */

DRM_RESULT DRM_API DRM_XML_GetSubNodeA(
    IN  const DRM_CHAR              *f_pszBase,
    IN  const DRM_SUBSTRING         *f_padstrXML,
    IN  const DRM_ANSI_CONST_STRING *f_padstrTag,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrName,
    IN  const DRM_ANSI_CONST_STRING *f_padstrAttrValue,
    IN  const DRM_DWORD              f_iNode,           /* nth occurence of the node in the xml stirng, start from 0 */
    OUT DRM_SUBSTRING               *f_psubstrXMLNode,      /* either of these 2 parameter can be NULL but not both */
    OUT DRM_SUBSTRING               *f_psubstrXMLNodeData, /* data enclosed by the immediate <tag>...</tag> in the given XML string */
    IN  DRM_DWORD                    f_iLayer);      /* outermost layer is 0 */

DRM_RESULT DRM_API DRM_XML_GetNodeAttributeA(
    IN const DRM_CHAR              *f_pszBase,
    IN const DRM_SUBSTRING         *f_padstrXML,
    IN const DRM_ANSI_CONST_STRING *f_padstrAttrName,   /* attrName to retrieve */
       OUT   DRM_SUBSTRING         *f_psubstrAttrValue); /* returned attrValue */

DRM_RESULT DRM_API DRM_XML_EnumNextNodeA(
    IN const DRM_CHAR         *f_pszBase,
    IN const DRM_SUBSTRING    *f_pdasstrXML,
    IN const DRM_DWORD         f_iNode,
       OUT   DRM_SUBSTRING    *f_pdasstrTag,
       OUT   DRM_SUBSTRING    *f_pdasstrNode,          /* either of these 2 parameter can be NULL but not both */
       OUT   DRM_SUBSTRING    *f_pdasstrNodeData,      /* data enclosed by the immediate <tag>...</tag> in the given XML string */
       OUT   DRM_SUBSTRING    *f_pdasstr1stAttrName,   /* optional */
       OUT   DRM_SUBSTRING    *f_pdasstr1stAttrValue); /* optional */

DRM_RESULT DRM_API DRM_XML_CountMatchingNodesA(
    IN const DRM_CHAR              *f_pszBase,
    IN const DRM_SUBSTRING         *f_pdasstrXML,
    IN const DRM_ANSI_CONST_STRING *f_pdasstrTag,
    IN       DRM_ANSI_CONST_STRING *f_pdasstr1stAttrName,  /* optional */
    IN       DRM_ANSI_CONST_STRING *f_pdasstr1stAttrValue, /* optional */
       OUT   DRM_DWORD             *f_cMatchingNodes);

DRM_BOOL DRM_API DRM_XML_Validate( IN const DRM_CONST_STRING *f_pdstrXML );

#ifdef __cplusplus
}
#endif

#endif      /* __DRMXMLPARSER_H__ */



