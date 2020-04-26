/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMSTRINGSCFD_H__
#define __DRMSTRINGSCFD_H__

#include <drmtypes.h>

ENTER_PK_NAMESPACE;

    /* License Generation */
    extern const DRM_CONST_STRING       g_dstrTagLicensorInfo;
    extern const DRM_CONST_STRING       g_dstrTagEnablingBits;
    extern const DRM_CONST_STRING       g_dstrEnablingBitsAlgorithm;
    extern const DRM_CONST_STRING       g_dstrChainEnablingBitsAlgorithm;
    extern const DRM_CONST_STRING       g_dstrPUBKEY;               
    extern const DRM_CONST_STRING       g_dstrType;                 
    extern const DRM_CONST_STRING       g_dstrMachine;              
    extern const DRM_CONST_STRING       g_dstrVALUE;                
    extern const DRM_CONST_STRING       g_dstrXMLHeader;                             
    extern const DRM_CONST_STRING       g_dstrSOURCEID;                              
    extern const DRM_CONST_STRING       g_dstrConditionPlayAlways;                 
    extern const DRM_CONST_STRING       g_dstrConditionPlayCount;  
    extern const DRM_CONST_STRING       g_dstrActionPlayCountDecrement;
    extern const DRM_CONST_STRING       g_dstrActionPlayCountCheckDelete;
    extern const DRM_CONST_STRING       g_dstrActionSetFirstUseDate;
    extern const DRM_CONST_STRING       g_dstrTagONACTION;                           
    extern const DRM_CONST_STRING       g_dstrTypePlay;                              
    extern const DRM_CONST_STRING       g_dstrLicenseServerPrivateKey;               
    extern const DRM_CONST_STRING       g_dstrSignature;                             
    extern const DRM_CONST_STRING       g_dstrHashAlgorithmType;                     
    extern const DRM_CONST_STRING       g_dstrSignAlgorithmType;                     
    extern const DRM_CONST_STRING       g_dstrTagCertChain;                          
    extern const DRM_CONST_STRING       g_dstrCertificate2;                          
    extern const DRM_CONST_STRING       g_dstrCertificate1;                          
    extern const DRM_CONST_STRING       g_dstrTagISSUEDATE;
    
    extern const DRM_CONST_STRING       g_dstrCHAINCHECKSUM;
    extern const DRM_CONST_STRING       g_dstrUPLINK;
    
    /* ONSELECT */
    extern const DRM_CONST_STRING       g_dstrONSELECT;                              
    extern const DRM_CONST_STRING       g_dstrTagCondition;
    extern const DRM_CONST_STRING       g_dstrOnSelectSecurityLevel;                     
    extern const DRM_CONST_STRING       g_dstrOnSelectBeginDateCheckP1;      
    extern const DRM_CONST_STRING       g_dstrOnSelectBeginDateCheckP2;      
    extern const DRM_CONST_STRING       g_dstrOnSelectExpirationDateCheckP1; 
    extern const DRM_CONST_STRING       g_dstrOnSelectExpirationDateCheckP2; 
    extern const DRM_CONST_STRING       g_dstrOnSelectGracePeriod;
    extern const DRM_CONST_STRING       g_dstrOnSelectExpireHoursCheck1;
    extern const DRM_CONST_STRING       g_dstrOnSelectDelete;
    extern const DRM_CONST_STRING       g_dstrOnSelectExpireHoursCheck2;
    extern const DRM_CONST_STRING       g_dstrOnSelectDateTimeCheck1;
    extern const DRM_CONST_STRING       g_dstrOnSelectDateTimeCheck2;
    
    /* ONCLOCKROLLBACK */ 
    extern const DRM_CONST_STRING       g_dstrOnClockRollbackNode;
    
    /* ONSTORE */ 
    extern const DRM_CONST_STRING       g_dstrOnStoreHoursFromFirstUse;
    extern const DRM_CONST_STRING       g_dstrOnStorePlayCount;
    extern const DRM_CONST_STRING       g_dstrOnStoreBeginPlayCount;
    extern const DRM_CONST_STRING       g_dstrOnStoreOldSaveTime;
    extern const DRM_CONST_STRING       g_dstrOnStoreSecurityLevel;
    extern const DRM_CONST_STRING       g_dstrOnStoreBeginDate;
    extern const DRM_CONST_STRING       g_dstrOnStoreExpirationDate;

    extern const DRM_CONST_STRING       g_dstrREVINFOVERSION;                       

    /* RESTRICTIONS */
    extern const DRM_CONST_STRING       g_dstrRESTRICTIONS;                          
    extern const DRM_CONST_STRING       g_dstrTypeLevel;
    extern const DRM_CONST_STRING       g_dstrUNCOMPRESSEDDIGITALVIDEO;             
    extern const DRM_CONST_STRING       g_dstrCOMPRESSEDDIGITALVIDEO;                
    extern const DRM_CONST_STRING       g_dstrANALOGVIDEO;                           
    extern const DRM_CONST_STRING       g_dstrCOMPRESSEDDIGITALAUDIO;                
    extern const DRM_CONST_STRING       g_dstrUNCOMPRESSEDDIGITALAUDIO;              
    extern const DRM_CONST_STRING       g_dstrEXTENSIONLIST;                         
    extern const DRM_CONST_STRING       g_dstrEXTENSION;                             
    extern const DRM_CONST_STRING       g_dstrguid;
    extern const DRM_CONST_STRING       g_dstrvalue;

    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderLicenseResponse;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderSLK;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderSLKclose;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderSLKID;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderSLKIDclose;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderSLKDATA;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderSLKDATAclose;
    extern const DRM_ANSI_CONST_STRING  g_dastrHeaderLicense;            
    extern const DRM_ANSI_CONST_STRING  g_dastrTailLicenseResponse;              
    
    /* CRL Update */
    extern const DRM_CONST_STRING       g_dstrLICENSERESPONSE;
    extern const DRM_CONST_STRING       g_dstrREVOCATIONINFO; 
    extern const DRM_CONST_STRING       g_dstrREVOCATION;     
    extern const DRM_CONST_STRING       g_dstrGUID;
    extern const DRM_CONST_STRING       g_dstrOpenTemplateTag; 
    extern const DRM_CONST_STRING       g_dstrCloseTemplateTag;

    /* Build Header */
    extern const DRM_CONST_STRING       g_dstrTagCID;
    
EXIT_PK_NAMESPACE;

#endif /* __DRMSTRINGSCFD_H__ */


