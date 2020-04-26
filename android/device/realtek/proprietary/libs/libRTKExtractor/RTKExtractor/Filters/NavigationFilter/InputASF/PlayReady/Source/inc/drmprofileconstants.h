/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __DRMPROFILECONSTANTS_H__
#define __DRMPROFILECONSTANTS_H__

#define PERF_MOD_BLACKBOX                       1
#define PERF_MOD_DRMCHAIN                       2
#define PERF_MOD_DRMHDS                         3
#define PERF_MOD_DRMHDSIMPL                     4
#define PERF_MOD_DRMLICACQREQ                   5
#define PERF_MOD_DRMLICACQRESP                  6
#define PERF_MOD_DRMLICEVAL                     7
#define PERF_MOD_DRMLICSTORE                    8
#define PERF_MOD_DRMSECSTORE                    9
#define PERF_MOD_DEVCERT                        10
#define PERF_MOD_DEVICEDEVCERT                  11
#define PERF_MOD_DRM_APP                        12
#define PERF_MOD_METERING                       13
#define PERF_MOD_OEMFILEIO                      14
#define PERF_MOD_SECURECLOCKREQUEST             15
#define PERF_MOD_SECURECLOCKRESPONSE            16
#define PERF_MOD_DRMGENSYNCCHALLENGE            17
#define PERF_MOD_DRMHDRPARSER                   18
#define PERF_MOD_DRMVIEWRIGHTS                  19
#define PERF_MOD_DRMLICPARSER                   20
#define PERF_MOD_DRM_ECC_P160                   21
#define PERF_MOD_DRMSYNCSTORE                   22
#define PERF_MOD_DRMUTILITIES                   23
#define PERF_MOD_DRMXMLPARSER                   24
#define PERF_MOD_DRMBASE64                      25
#define PERF_MOD_OEMINFO                        26
#define PERF_MOD_DRMCIPHER                      27
#define PERF_MOD_DESKEY                         28
#define PERF_MOD_RC4                            29
#define PERF_MOD_DRMCBC                         30
#define PERF_MOD_CFD                            31      /* Copy From Device Porting Kit */
#define PERF_MOD_OEMAES                         32
#define PERF_MOD_DRMAES                         33
#define PERF_MOD_DRMRSA                         34
#define PERF_MOD_DRMSHA256                      35
#define PERF_MOD_DRM_ECC_P256                   36
#define PERF_MOD_BCERT                          37      /* Binary Certificates */
#define PERF_MOD_DOMAINSTORE                    38      /* Domain store */
#define PERF_MOD_DOMAINAPI                      39      /* Domain APIs */
#define PERF_MOD_XMLSIG                         40      /* XML encryption/signature */
#define PERF_MOD_SOAPXML                        41      /* SOAP/XML utility */
#define PERF_MOD_LICACQV3                       42      /* V3 license acquisition */
#define PERF_MOD_UTF8                           43      /* UTF-8 */
#define PERF_MOD_DRMHMAC                        44
#define PERF_MOD_NDR                            45      /* Network Device - Receiver */
#define PERF_MOD_NDT                            46      /* Network Device - Transmitter */
#define PERF_MOD_DEVICESTORE                    47
#define PERF_MOD_REVOCATION                     48
#define PERF_MOD_RSA                            49
#define PERF_MOD_NDT_NETMSG                     50
#define PERF_MOD_NDT_ASF                        51
#define PERF_MOD_NDT_ASF_SAMPLE                 52
#define PERF_MOD_NDT_ASF_MUX                    53
#define PERF_MOD_NDT_ASF_STREAM                 54
#define PERF_MOD_DRMPRO                         55      /* PlayReady Object APIs */
#define PERF_MOD_DRMEMB                         56      /* License Embedding APIs */
#define PERF_MOD_DRMXMLBUILDER                  57      /* Both UTF-16 and UTF-8 XML builders */
#define PERF_MOD_NDCERT                         58      /* ND Certificates */


/* blackbox\blackbox.c */
#define PERF_FUNC_DRM_BBX_CipherKeySetup        1
#define PERF_FUNC_DRM_BBX_LegacyCipherKeySetup  2
#define PERF_FUNC_DRM_BBX_HashValue             3
#define PERF_FUNC_DRM_BBX_SymmetricVerify       4
#define PERF_FUNC_DRM_BBX_Initialize            5
#define PERF_FUNC_DRM_BBX_CanBind               6
#define PERF_FUNC__DecryptContentKey            7
#define PERF_FUNC_VerifyChecksum                8
#define PERF_FUNC__GetDevicePrivkey             9
#define PERF_FUNC_DRM_BBX_RebindSLK             10
#define PERF_FUNC_DRM_BBX_DecryptLicense        11
#define PERF_FUNC_DRM_BBX_SymmetricCipher       12
#define PERF_FUNC_DRM_BBX_StoreDomainPrivateKeys 13

/* core\drmchain.c */
#define PERF_FUNC__PerformActions               1
#define PERF_FUNC__ProcessEndOfChain            2
#define PERF_FUNC_DRM_LIC_CompleteLicenseChain  3
#define PERF_FUNC_DRM_LIC_ReportActions         4
#define PERF_FUNC_DRM_LIC_CheckClockRollback    5
#define PERF_FUNC__GetLicenseInfoAndSetup       6
#define PERF_FUNC__ValidateRevocationVersions   7
#define PERF_FUNC_DRM_LIC_GetInclusionList      8

/* core\drmhds.c */
#define PERF_FUNC_DRM_HDS_OpenStore2            1
#define PERF_FUNC_DRM_HDS_CloseStore            2
#define PERF_FUNC_DRM_HDS_OpenNamespace         3
#define PERF_FUNC_DRM_HDS_OpenSlot              4
#define PERF_FUNC_DRM_HDS_InitSlotEnum          5
#define PERF_FUNC_DRM_HDS_SlotEnumNext          6
#define PERF_FUNC_DRM_HDS_Init                  7

/* core\drmhds.c */
#define PERF_FUNC__HdsGetPutBlockHDR            1
#define PERF_FUNC__HdsGetPutBlockPayload        2
#define PERF_FUNC__HdsSearchSlotInFile          3
#define PERF_FUNC__HdsHashToChildBlock          4
#define PERF_FUNC__HdsSearchSlotInBlock         5
#define PERF_FUNC__HdsCopyBlockBuffer           6
#define PERF_FUNC__HdsAllocBlockBuffer          7
#define PERF_FUNC__HdsHashKeyToIndex            8
#define PERF_FUNC__HdsLoadBlockHDR              9
#define PERF_FUNC__HdsLoadSRN                   10
#define PERF_FUNC__HdsGenSRNHash                11
#define PERF_FUNC__WriteSRN                     12
#define PERF_FUNC__ReadSRN                      13
#define PERF_FUNC__WriteCommonBlockHeader       14
#define PERF_FUNC__ReadCommonBlockHeader        15
#define PERF_FUNC__WriteChildBlockHeader        16
#define PERF_FUNC__ReadChildBlockHeader         17
#define PERF_FUNC__WriteDataBlockHeader         18
#define PERF_FUNC__ReadDataBlockHeader          19
#define PERF_FUNC__HdsGenBlockHash              20
#define PERF_FUNC__HdsGetPutChildBlockNum       21
#define PERF_FUNC__HdsExpandStore               22
#define PERF_FUNC__HdsUpdateSRN                 23
#define PERF_FUNC__HdsAllocBlock                24
#define PERF_FUNC__HdsFreeBlock                 25
#define PERF_FUNC__HdsLocateFreeBlockForSlot    26
#define PERF_FUNC__HdsCreateOversizedSlot       27
#define PERF_FUNC__HdsOpenSlotFromHint          28
#define PERF_FUNC__HdsAllocSlotInFile           29
#define PERF_FUNC__HdsCreateAndOpenSlot         30
#define PERF_FUNC__HdsOpenSlot                  31
#define PERF_FUNC__HdsVerifySlotContext         32
#define PERF_FUNC__HdsReadWriteSlot             33
#define PERF_FUNC__HdsAdjustChildPayload        34
#define PERF_FUNC__HdsFindLeftmostLeafBlock     35
#define PERF_FUNC__HdsCopySlot_Child2Child      36
#define PERF_FUNC__HdsCopyChildPayload          37
#define PERF_FUNC__HdsDefragmentFile            38
#define PERF_FUNC__HdsDeleteSubTree             39
#define PERF_FUNC__HdsTraverseNextRightSiblingBlock 40
#define PERF_FUNC__HdsTraverseBlocksInPostOrder     41
#define PERF_FUNC__HdsGetPutSlotAttributes          42

/* core\drmlicacqreq.c */
#define PERF_FUNC_DRM_LA_CreateChallenge        1
#define PERF_FUNC_DRM_LA_GetClientInfo          2

/* core\drmlicacqresp.c */
#define PERF_FUNC_DRM_LA_ProcessResponse                1
#define PERF_FUNC__UpdateLicenseWithSymmetricData       2
#define PERF_FUNC__UpdateLicenseWithSymmetricKey        3
#define PERF_FUNC__UpdateLicenseWithSymmetricSignature  4

/* core\drmliceval.c */
#define PERF_FUNC_DRM_LEVL_PerformOperationsXML         1
#define PERF_FUNC_DRM_LEVL_GetLicenseReasonForUnusable  2
#define PERF_FUNC_Eval                                  3
#define PERF_FUNC_SetOrUpdateContentRevocation          4
#define PERF_FUNC_DRM_LEVL_EvaluateExpression           5
#define PERF_FUNC_DRM_LEVL_PerformOperationsXMR         6
#define PERF_FUNC__SetOrUpdateContentRevocation         7

/* core\drmlicstore.c */
#define PERF_FUNC__InitEnum                     1
#define PERF_FUNC_DRM_LST_EnumNext              2
#define PERF_FUNC_DRM_LST_Open                  3
#define PERF_FUNC_DRM_LST_Close                 4
#define PERF_FUNC_DRM_LST_GetLicense            5

/* core\drmsecstore.c */
#define PERF_FUNC_DRM_SST_OpenKeyTokens                 1
#define PERF_FUNC_DRM_SST_CreateGlobalStorePassword     2
#define PERF_FUNC_DRM_SST_CreateLicenseStatePassword    3
#define PERF_FUNC_DRM_SST_CloseKey                      4
#define PERF_FUNC_DRM_SST_GetData                       5
#define PERF_FUNC_DRM_SST_SetData                       6
#define PERF_FUNC_DRM_SST_GetExtendedTokenValue         7
#define PERF_FUNC_DRM_SST_SetExtendedTokenValue         8
#define PERF_FUNC__VerifySlotHash                       9

/* core\drmplayreadyobj.c */
#define PERF_FUNC_DRM_PRO_Create                          1
#define PERF_FUNC_DRM_PRO_GetCipherTypeFromPlayReadyHeader  2
#define PERF_FUNC_DRM_PRO_GetCipherType                 3
#define PERF_FUNC_DRM_PRO_ConvertHeaderFromWmdrmToPlayReady 4
#define PERF_FUNC_Drm_PlayReadyObject_ConvertFromWmdrmHeader 5

/* devcertparser\devcert.c */
#define PERF_FUNC_DRM_DCP_GetAttribute          1
#define PERF_FUNC_DRM_DCP_LoadPropertiesCache   2

/* devicedevcert\devicedevcert.c */
#define PERF_FUNC__CompareMachineId                  1
#define PERF_FUNC__VerifySymmetricSignature          2
#define PERF_FUNC_DRM_DDC_GetDeviceCertificate       3
#define PERF_FUNC_DRM_DDC_GetBinaryDeviceCertificate 4
#define PERF_FUNC_DRM_DDC_XMLCertificate_Initialize  5
#define PERF_FUNC_DRM_DDC_BINCertificate_Initialize  6

/* drmmanager\drmmanager.c */
#define PERF_FUNC_Drm_Initialize                            1
#define PERF_FUNC_Drm_Uninitialize                          2
#define PERF_FUNC_Drm_LicenseAcq_GenerateChallenge          3
#define PERF_FUNC_Drm_Device_GetProperty                    4
#define PERF_FUNC_Drm_LicenseAcq_ProcessResponse            5
#define PERF_FUNC_Drm_Reader_Bind                           6
#define PERF_FUNC_Drm_Reader_Commit                         7
#define PERF_FUNC_Drm_LicenseQuery_GetState                 8
#define PERF_FUNC_Drm_Sync_GenerateChallenge                9
#define PERF_FUNC_Drm_SecureClock_ProcessResponse           10
#define PERF_FUNC_Drm_SecureClock_GenerateChallenge         11
#define PERF_FUNC_Drm_SecureClock_GetValue                  12
#define PERF_FUNC_Drm_StoreMgmt_CleanupStore                13
#define PERF_FUNC_Drm_LicenseQuery_IsAllowed                14
#define PERF_FUNC__CheckSecureClock                         15
#define PERF_FUNC__SetupLicEvalObjectToShare                16
#define PERF_FUNC__CreateDeviceStore                        17
#define PERF_FUNC_Drm_SyncListItem_InitEnum                 18
#define PERF_FUNC_Drm_SyncListItem_EnumNext                 19
#define PERF_FUNC_Drm_Envelope_Open                         20
#define PERF_FUNC_Drm_Envelope_InitializeRead               21
#define PERF_FUNC_Drm_Envelope_Close                        22
#define PERF_FUNC_Drm_Envelope_GetSize                      23
#define PERF_FUNC_Drm_Envelope_Read                         24
#define PERF_FUNC_Drm_Envelope_Seek                         25
#define PERF_FUNC_Drm_StoreMgmt_DeleteLicenses              26
#define PERF_FUNC_Drm_License_GetProperty                   27
#define PERF_FUNC_Drm_Content_UpdateEmbeddedStore           28
#define PERF_FUNC_Drm_Content_GetProperty                   29
#define PERF_FUNC_Drm_Envelope_WritePlayReadyObject         30
#define PERF_FUNC_Drm_Content_UpdateEmbeddedStore_Commit    31

/* metering\drmmeterimpv1.c */
/* metering\drmmeterimp.c */
/* metering\drmmeterapi.c */
#define PERF_FUNC_DRM_MTR_UpdateData                1
#define PERF_FUNC_DRM_MTR_ProcessMeterResponse      2
#define PERF_FUNC__BuildMeterCertChallengeXML       3
#define PERF_FUNC__ProcessMeterCertInResponse       4
#define PERF_FUNC_DRM_MTR_GenerateMeterCertChallenge    5
#define PERF_FUNC_DRM_MTR_ProcessMeterCertResponse  6
#define PERF_FUNC_Drm_Metering_GenerateChallenge    7
#define PERF_FUNC_Drm_Metering_ProcessResponse      8
#define PERF_FUNC_Drm_MeterCert_Update              9
#define PERF_FUNC_Drm_MeterCert_InitEnum            10
#define PERF_FUNC_Drm_MeterCert_EnumNext            11
#define PERF_FUNC_Drm_GetMeterCert_By_MID           12
#define PERF_FUNC_Drm_MeterCert_GenerateChallenge   13
#define PERF_FUNC_Drm_MeterCert_ProcessResponse     14
#define PERF_FUNC_DRM_MTR_GenerateMeterDataChallenge    15
#define PERF_FUNC_DRM_MTR_ProcessMeterDataResponse  16
#define PERF_FUNC__VerifyMeterDataResponse          17
#define PERF_FUNC__ProcessMeterDataResponse         18
#define PERF_FUNC__CalcMaximumChallengeCharCount    19
#define PERF_FUNC__CalcFixedUnencryptedChallengeCharCount   20
#define PERF_FUNC__CalcFixedEncryptedChallengeCharCount     21
#define PERF_FUNC__CalcVariableChallengeCharCount   22
#define PERF_FUNC__ProcessAllKIDs                   23
#define PERF_FUNC__CalcKIDDataCharCount             24
#define PERF_FUNC__BuildKIDDataXML                  25
#define PERF_FUNC__BuildMeterDataChallengeXML       26
#define PERF_FUNC__BuildMeterDataChallengeDataXML   27
#define PERF_FUNC__PrepareMeterChallengeContext     28
#define PERF_FUNC__ParseMeterCert                   29


/* oem\ansi\oemfileio.c */
#define PERF_FUNC_Oem_File_SetFilePointer        1
#define PERF_FUNC_Oem_File_Lock                  2
#define PERF_FUNC_Oem_File_Unlock                3
#define PERF_FUNC_Oem_File_Open                  4
#define PERF_FUNC_Oem_File_Read                  5
#define PERF_FUNC_Oem_File_Write                 6
#define PERF_FUNC_Oem_File_GetSize               7
#define PERF_FUNC_Oem_File_FlushBuffers          8

/* secure_clock\secureclockrequest.c */
#define PERF_FUNC_DRM_CLK_CreateChallenge       1
#define PERF_FUNC_DRM_CLK_InitCheckSecureClock  2

/* secure_clock\secureclockresponse.c */
#define PERF_FUNC_DRM_CLK_ProcessResponse       1

/* synclist\drmgensyncchallenge.c */
#define PERF_FUNC_DRM_SNC_GenerateSyncChallenge 1

/* core\drmhdrparser.c */
#define PERF_FUNC_DRM_HDR_GetAttribute          1
#define PERF_FUNC_DRM_HDR_Verify                2
#define PERF_FUNC_DRM_ASD_GetLicenseAggregateData       1
#define PERF_FUNC_DRM_ASD_ParseV2License                2
#define PERF_FUNC__QueryAndAggregateLicStatesForKID     3
#define PERF_FUNC__LoadNextLicense                      4
#define PERF_FUNC__QueryAndAggregateLicStatesForKID 3
#define PERF_FUNC__QueryRightForAction                  6
#define PERF_FUNC__CollectLicStateforRequestedActions   5
#define PERF_FUNC__QueryRightForAction          6

/* core\drmlicparser.c */
#define PERF_FUNC_DRM_LIC_VerifySignature       1
#define PERF_FUNC_DRM_LIC_VerifyCertChain       2
#define PERF_FUNC_DRM_LIC_GetAttribute          3
#define PERF_FUNC_DRM_LIC_GetEnablingBits       4

/* crypto\pkcrypto\drmeccp160.c */
#define PERF_FUNC_DRM_ECC_Verify_P160           1
#define PERF_FUNC_DRM_ECC_GenKeyPair_P160       2
#define PERF_FUNC_DRM_ECC_Sign_P160             3
#define PERF_FUNC_DRM_ECC_Decrypt_P160          4

/* crypto\pkcrypto\drmeccp256.c */
#define PERF_FUNC_DRM_ECDSA_Verify_P256                             1
#define PERF_FUNC_DRM_ECDSA_Sign_P256                               2
#define PERF_FUNC_DRM_ECC_GenKeyPair_P256                           3
#define PERF_FUNC_DRM_ECC_Encrypt_P256                              4
#define PERF_FUNC_DRM_ECC_Decrypt_P256                              5
#define PERF_FUNC_DRM_ECC_CanMapToPoint_P256                        6
#define PERF_FUNC_DRM_AES_GenerateHMACKey                           7
#define PERF_FUNC_Convert_P256_PointToPlaintext                     8
#define PERF_FUNC_Convert_P256_PointToDigits                        9
#define PERF_FUNC_Convert_P256_PointToBigEndianBytes                10
#define PERF_FUNC_Convert_P256_PlaintextToPoint                     11
#define PERF_FUNC_Convert_P256_ModularIntToDigitsModOrder           12
#define PERF_FUNC_Convert_P256_ModularIntToBigEndianBytesModOrder   13
#define PERF_FUNC_Convert_P256_DigitsToBigEndianBytes               14
#define PERF_FUNC_Convert_P256_DigitsToPoint                        15
#define PERF_FUNC_Convert_P256_BigEndianBytesToPoint                16
#define PERF_FUNC_Convert_P256_BigEndianBytesToModular              17
#define PERF_FUNC_Convert_P256_BigEndianBytesToDigitsModOrder       18
#define PERF_FUNC_Convert_DigitsToBigEndianBytes                    19
#define PERF_FUNC_Convert_P256_BigEndianBytesToDigits               20
#define PERF_FUNC_Convert_BigEndianBytesToDigits                    21
#define PERF_FUNC_DRM_ECC_MapX2PointP256                            22

/* crypto\drmsha256\drmsha256.c */
#define PERF_FUNC_DRM_SHA256_Init               1
#define PERF_FUNC_DRM_SHA256_Update             2
#define PERF_FUNC_DRM_SHA256_Finalize           3
#define PERF_FUNC_SHA256_Transform              4

/* synclist\drmsyncstore.c */
#define PERF_FUNC_DRM_SNC_OpenStore             1
#define PERF_FUNC_DRM_SNC_GetSyncStoreEntry     2
#define PERF_FUNC_DRM_SNC_UpdateKID             3

/* core\drmutilities.c */
#define PERF_FUNC_DRM_UTL_DecodeKID             1
#define PERF_FUNC__AddCachedCertificate         2
#define PERF_FUNC__CheckCertificate             3
#define PERF_FUNC__VerifyCachedCertificate      4

/* core\drmxmlparser.c */
#define PERF_FUNC__GetXMLSubNodeW               1
#define PERF_FUNC_DRM_XML_GetSubNodeByPath      2
#define PERF_FUNC_DRM_XML_GetNodeAttribute      3
#define PERF_FUNC_DRM_XML_GetSubNode            4
#define PERF_FUNC__ScanTagW                     5
#define PERF_FUNC__CheckTagW                    6
#define PERF_FUNC_DRM_XML_GetSubNodeA           7

/* core\drmbase64.c */
#define PERF_FUNC_DRM_B64_DecodeW               1
#define PERF_FUNC_DRM_B64_DecodeA               2
#define PERF_FUNC_DRM_B64_EncodeW               3
#define PERF_FUNC_DRM_B64_EncodeA               4

/* oem\ansi\oeminfo.c */
#define PERF_FUNC_Oem_Device_GetCert            1
#define PERF_FUNC__GetPrivateKey                2
#define PERF_FUNC_Oem_Clock_GetResetState       3
#define PERF_FUNC_Oem_Clock_SetResetState       4

/* core\drmcipher.c */
#define PERF_FUNC_DRM_CPHR_InitDecrypt          1
#define PERF_FUNC_DRM_CPHR_Decrypt              2
#define PERF_FUNC_DRM_CPHR_Encrypt              3
#define PERF_FUNC_DRM_CPHR_Init                 4

/* crypto\drmcrypto\deskey.c */
#define PERF_FUNC_DRM_DES_Cipher                1

/* crypto\drmcrypto\rc4.c */
#define PERF_FUNC_DRM_RC4_KeySetup              1
#define PERF_FUNC_DRM_RC4_Cipher                2

/* core\drmcbc.c */
#define PERF_FUNC_DRM_CBC64Update               1

/* oem\ansi\oemaes.c */
#define PERF_FUNC_Oem_Aes_SetKey                1
#define PERF_FUNC_Oem_Aes_EncryptOne            2
#define PERF_FUNC_Oem_Aes_DecryptOne            3

/* core\drmaes.c */
#define PERF_FUNC_DRM_Aes_CtrProcessData        1
#define PERF_FUNC_DRM_Aes_EcbEncryptData        2
#define PERF_FUNC_DRM_Aes_EcbDecryptData        3
#define PERF_FUNC_Omac1_GenerateSignTag         4
#define PERF_FUNC_Omac1_GenerateSignInfo        5
#define PERF_FUNC_DRM_Omac1_Sign                6
#define PERF_FUNC_DRM_Omac1_Verify              7
#define PERF_FUNC_DRM_Aes_CbcEncryptData        8
#define PERF_FUNC_DRM_Aes_CbcDecryptData        9

/* core\drmrsa.c */
#define PERF_FUNC_DRM_RSA_OaepDecrypt           1
#define PERF_FUNC_DRM_RSA_OaepEncrypt           2
#define PERF_FUNC_DRM_RSA_PssSign               3
#define PERF_FUNC_DRM_RSA_PssVerify             4

/* certs\drmbcertbuilder.c */
/* certs\drmbcertparser.c */
#define PERF_FUNC_calcManufacturerInfoSize      1
#define PERF_FUNC_calcKeyInfoSize               2
#define PERF_FUNC_calcFeatureInfoSize           3
#define PERF_FUNC_calcSignatureInfoSize         4
#define PERF_FUNC_calcDomainInfoSize            5
#define PERF_FUNC_calcCertSize                  6
#define PERF_FUNC_checkBuffer                   7
#define PERF_FUNC_addAlignedData                8
#define PERF_FUNC_updateCertChainHeader         9
#define PERF_FUNC_addCertChainHeader           10
#define PERF_FUNC_addCertHeader                11
#define PERF_FUNC_addCertObjectHeader          12
#define PERF_FUNC_addCertBasicInfo             13
#define PERF_FUNC_addCertFeatureInfo           14
#define PERF_FUNC_addCertKeyInfo               15
#define PERF_FUNC_addCertManufacturerString    16
#define PERF_FUNC_addCertManufacturerInfo      17
#define PERF_FUNC_addCertSignatureInfo         18
#define PERF_FUNC_addCertDomainInfo            19
#define PERF_FUNC_addCertPCInfo                20
#define PERF_FUNC_addCertDeviceInfo            21
#define PERF_FUNC_DRM_BCert_Add_Cert           22
#define PERF_FUNC_verifyKeyUsage               23
#define PERF_FUNC_verifyFeature                24
#define PERF_FUNC_verifyAdjacentCerts          25
#define PERF_FUNC_getObjectHeader              26
#define PERF_FUNC_parseCertHeader              27
#define PERF_FUNC_parseCertBasicInfo           28
#define PERF_FUNC_parseDomainInfo              29
#define PERF_FUNC_parsePCInfo                  30
#define PERF_FUNC_parseDeviceInfo              31
#define PERF_FUNC_parseFeatureInfo             32
#define PERF_FUNC_parseManufacturerString      33
#define PERF_FUNC_parseSignatureInfo           34
#define PERF_FUNC_parseKeyInfo                 35
#define PERF_FUNC_parseCertificate             36
#define PERF_FUNC_FindObjectInCertByType       37
#define PERF_FUNC_GetChainHeader               38
#define PERF_FUNC_ParseCertificateChain        39
#define PERF_FUNC_GetCertificate               40
#define PERF_FUNC_GetPublicKey                 41
#define PERF_FUNC_addCertSilverLightInfo       42
#define PERF_FUNC_parseSilverLightInfo         43
#define PERF_FUNC_verifySignatureInfo          44
#define PERF_FUNC_calcExtDataSignKeyInfoSize   45
#define PERF_FUNC_calcExtDataContainerSize     46
#define PERF_FUNC_addExtDataSignKeyInfo        47
#define PERF_FUNC_addExtDataContainer          48
#define PERF_FUNC_DRM_BCert_AddExtDataToCert   49
#define PERF_FUNC_verifyExtDataSignature       50
#define PERF_FUNC_parseExtDataSignKeyInfo      51
#define PERF_FUNC_parseExtDataContainer        52
#define PERF_FUNC_addCertMeteringInfo          53
#define PERF_FUNC_calcMeteringInfoSize         54
#define PERF_FUNC_parseMeteringInfo            55
#define PERF_FUNC_GetPublicKeyByUsage          56
#define PERF_FUNC_parseServerInfo              57
#define PERF_FUNC_addCertServerInfo            58

/* domain\drmdomainstore.c */
#define PERF_FUNC__DomainStore_AddData          1
#define PERF_FUNC__DomainStore_GetData          2
#define PERF_FUNC__DomainStore_DeleteData       3
#define PERF_FUNC__DomainStore_InitEnumData     4
#define PERF_FUNC__DomainStore_EnumNextData     5
#define PERF_FUNC_DRM_DOMST_OpenStore           6
#define PERF_FUNC_DRM_DOMST_CloseStore          7
#define PERF_FUNC_DRM_DOMST_DeleteKeys          8

/* domain\drmdomainapi.c */
/* domain\drmdomainimp.c */
/* domain\drmdomainkeyxmrparser.c */
#define PERF_FUNC__BuildJoinChallengeDataXML    1
#define PERF_FUNC__BuildLeaveChallengeDataXML   2
#define PERF_FUNC__BuildJoinChallengeXML        3
#define PERF_FUNC__BuildLeaveChallengeXML       4
#define PERF_FUNC__VerifyJoinResponse           5
#define PERF_FUNC__VerifyLeaveResponse          6
#define PERF_FUNC__StoreDomainJoinData          7
#define PERF_FUNC__StoreDomainJoinPreviousKeys  8
#define PERF_FUNC__ProcessJoinDataFromResponse  9
#define PERF_FUNC__ExtractLeaveDataFromResponse 10
#define PERF_FUNC_DRM_DOM_GenerateJoinChallenge 11
#define PERF_FUNC_DRM_DOM_ProcessJoinResponse   12
#define PERF_FUNC_DRM_DOM_GenerateLeaveChallenge    13
#define PERF_FUNC_DRM_DOM_ProcessLeaveResponse  14
#define PERF_FUNC_DRM_DOM_FindCert              15
#define PERF_FUNC_DRM_DOM_InitCertEnum          16
#define PERF_FUNC_DRM_DOM_EnumNextCert          17
#define PERF_FUNC__ParseJoinDomainChallengeQueryData 18
#define PERF_FUNC__ParseLeaveDomainChallengeQueryData 19
#define PERF_FUNC_DRM_DOMKEYXMR_HEADER          20
#define PERF_FUNC_DRM_DOMKEYXMR_SESSIONKEY      21
#define PERF_FUNC_DRM_DOMKEYXMR_GetPrivKeyContainer 22
#define PERF_FUNC_DRM_DOMKEYXMR_GetPrivKey      23


#define PERF_FUNC__

/* core\drmxmlsig.c */
#define PERF_FUNC__CalcSHA256Hash               1
#define PERF_FUNC__VerifySHA256Hash             2
#define PERF_FUNC__BuildCipherDataNode          3
#define PERF_FUNC__ExtractCipherData            4
#define PERF_FUNC__BuildSignedInfoNode          5
#define PERF_FUNC__BuildSignatureValueNode      6
#define PERF_FUNC__BuildPublicKeyInfoNodeWithName   7
#define PERF_FUNC__ExtractPublicKeyByName       8
#define PERF_FUNC__BuildECC256PublicKeyInfoNode 9
#define PERF_FUNC__ExtractECC256PublicKey       10
#define PERF_FUNC_DRM_XMLSIG_BuildEncryptedKeyInfoNode  11
#define PERF_FUNC_DRM_XMLSIG_ExtractEncryptedKey   12
#define PERF_FUNC_DRM_XMLSIG_BuildSignatureNode     13
#define PERF_FUNC_DRM_XMLSIG_VerifySignature        14
#define PERF_FUNC_DRM_XMLSIG_BuildEncryptedDataNode 15
#define PERF_FUNC_DRM_XMLSIG_ExtractEncryptedData   16

/* core\drmsoapxmlutility.c */
#define PERF_FUNC_DRM_SOAPXML_EncodeData                 1
#define PERF_FUNC_DRM_SOAPXML_DecodeData                 2
#define PERF_FUNC_DRM_SOAPXML_GetDeviceCert              3
#define PERF_FUNC_DRM_SOAPXML_InitXMLKey                 4
#define PERF_FUNC_DRM_SOAPXML_EncryptDataWithXMLKey      5
#define PERF_FUNC_DRM_SOAPXML_BuildSOAPHeaderXML         6
#define PERF_FUNC__ParseCustomData                       7
#define PERF_FUNC__ParseCustomDataByPath                 8
#define PERF_FUNC_DRM_SOAPXML_ParseStatusCode            9
#define PERF_FUNC_Drm_GetAdditionalResponseData          10
#define PERF_FUNC_DRM_SOAPXML_GetAdditionalResponseData  11
#define PERF_FUNC_DRM_SOAPXML_ParseCustomDataForProtocol 12
#define PERF_FUNC_DRM_SOAPXML_ValidateProtocolSignature  13


/* core\drmlicacqv3.c */
#define PERF_FUNC__CalcDeviceCertCharCount      1
#define PERF_FUNC__CalcDomainCertsCharCount     2
#define PERF_FUNC__BuildDomainCertsXML          3
#define PERF_FUNC__BuildCertChainsXML           4
#define PERF_FUNC__BuildClientInfoXML           5
#define PERF_FUNC__BuildRevListInfoXML          6
#define PERF_FUNC__BuildLicenseChallengeDataXML 7
#define PERF_FUNC__BuildLicenseAcknowledgementDataXML   8
#define PERF_FUNC__BuildLicenseChallengeXML     9
#define PERF_FUNC__BuildLicenseAcknowledgementXML   10
#define PERF_FUNC__PrepareViewRights            11
#define PERF_FUNC__GetLicenseState              12
#define PERF_FUNC__PrepareUplinks               13
#define PERF_FUNC__PrepareLicenseChallenge      14
#define PERF_FUNC__ParseLicenseAcquisitionURL   15
#define PERF_FUNC__ProcessRevInfo               16
#define PERF_FUNC__ExtractLicensesFromLicenseResponse   17
#define PERF_FUNC__ExtractRevInfoFromLicenseResponse    18
#define PERF_FUNC__ExtractDataFromLicenseResponse   19
#define PERF_FUNC__GenerateLicenseChallengeV3   20
#define PERF_FUNC__ProcessLicenseResponseV3     21
#define PERF_FUNC_Drm_LicenseAcq_GenerateAck    22
#define PERF_FUNC_Drm_LicenseAcq_ProcessAckResponse 23
#define PERF_FUNC_DRM_LA_ProcessLicenseV3       24

/* core\drmutf.c */
#define PERF_FUNC_DRM_UTF8_VerifyBytes          1

/* core\drmhmac.c */
#define PERF_FUNC_DRM_HMAC_CreateMAC            1
#define PERF_FUNC_DRM_HMAC_VerifyMAC            2

/* ndr\drmndr.c */
#define PERF_FUNC_Drm_Ndr_Registration_GenerateChallenge   1
#define PERF_FUNC_Drm_Ndr_Registration_ProcessResponse     2
#define PERF_FUNC_Drm_Ndr_License_GenerateChallenge        3
#define PERF_FUNC_Drm_Ndr_License_ProcessResponse          4
#define PERF_FUNC_Drm_Ndr_Proximity_GenerateStart          5
#define PERF_FUNC_Drm_Ndr_Proximity_GenerateResponse       6
#define PERF_FUNC_Drm_Ndr_Proximity_ProcessChallenge       7
#define PERF_FUNC_Drm_Ndr_Proximity_ProcessResult          8


/* Device Store */
#define PERF_FUNC__SetDirtyFlag                                     1
#define PERF_FUNC__GetDirtyFlag                                     2
#define PERF_FUNC__InitDeviceHandle                                 3
#define PERF_FUNC_DRM_DVS_Initialize                                4
#define PERF_FUNC_DRM_DVS_RegisterDevice                            5
#define PERF_FUNC__CalculateStoreID                                 6
#define PERF_FUNC__WriteDeviceStoreEntryItem                        7
#define PERF_FUNC_DRM_DVS_UnRegisterDevice                          8
#define PERF_FUNC_DRM_DVS_GetFirstDevice                            9
#define PERF_FUNC_DRM_DVS_GetNextDevice                             10
#define PERF_FUNC_DRM_DVS_GetDeviceByID                             11
#define PERF_FUNC_DRM_DVS_GetDeviceID                               12
#define PERF_FUNC_DRM_DVS_GetDeviceAttr                             13
#define PERF_FUNC__ReadDeviceStoreEntryItem                         14
#define PERF_FUNC_DRM_DVS_GetDeviceAttrCount                        15
#define PERF_FUNC_DRM_DVS_GetDeviceAttrByIndex                      16
#define PERF_FUNC_DRM_DVS_SetDeviceAttr                             17
#define PERF_FUNC_DRM_DVS_CommitDevice                              18
#define PERF_FUNC_DRM_DeviceStore_MarkDeviceAsValid                 19
#define PERF_FUNC_DRM_DeviceStore_UnRegisterDevice                  20
#define PERF_FUNC__GenerateDeviceID                                 21
#define PERF_FUNC_DRM_DeviceStore_GetDeviceByID                     22
#define PERF_FUNC_DRM_DeviceStore_GetDeviceInfo                     23
#define PERF_FUNC__ExtractDeviceInfo                                24

/* Revocation */
#define PERF_FUNC_DRM_RVK_PerformAppRevocationCheck                 1
#define PERF_FUNC_DRM_RVK_GetCurrentAppRevocationList               2
#define PERF_FUNC__CheckCertInRevocationList                        3
#define PERF_FUNC_DRM_RVK_UpdateAppRevocationListA                  4
#define PERF_FUNC_DRM_RVK_UpdateAppRevocationListW                  5
#define PERF_FUNC_DRM_RVK_UpdateAppRevocationListDecoded            6
#define PERF_FUNC_DRM_RVK_UpdateRevocationVersionsCache             7
#define PERF_FUNC_DRM_RVK_VerifyRevocationList                      8
#define PERF_FUNC_DRM_RVK_UpdateRevocationList                      9
#define PERF_FUNC__UpdateRevocationList                             10
#define PERF_FUNC__GetSSTRevocationList                             11
#define PERF_FUNC__VerifyWMDRMNETRevocationList                     12
#define PERF_FUNC_DRM_RVK_UpdateWMDRMNETRevocationList              13
#define PERF_FUNC__UpdateWMDRMNETRevocationList                     14
#define PERF_FUNC__VerifyCRLSignature                               15
#define PERF_FUNC_DRM_RVK_GetWMDRMNETRevocationEntries              16
#define PERF_FUNC__VerifyBinaryWMDRMNETSignature                    17
#define PERF_FUNC_DRM_RVK_VerifyRevocationInfo                      18
#define PERF_FUNC_DRM_RVK_GetCurrentRevocationInfo                  19
#define PERF_FUNC_DRM_RVK_StoreRevocationLists                      20
#define PERF_FUNC_DRM_RVK_StoreRevInfo                              21
#define PERF_FUNC_DRM_RVK_ProcessRevocationInfo                     22
#define PERF_FUNC__ExtractRevocationList                            23
#define PERF_FUNC_DRM_RVK_GetCRL                                    24
#define PERF_FUNC_DRM_RVK_SetCRL                                    25
#define PERF_FUNC_DRM_RVS_InitRevocationStore                       26
#define PERF_FUNC_DRM_RVS_StoreRevocationData                       27
#define PERF_FUNC__CreateRevocationStorePassword                    28
#define PERF_FUNC__LoopkupRevocationLIDFromGUID                     29
#define PERF_FUNC_DRM_RVS_GetRevocationData                         30
#define PERF_FUNC_DRM_BCrl_VerifySignature                          31
#define PERF_FUNC_DRM_RVK_StoreMsdbCRLPackage                       32

/* RSA */
#define PERF_FUNC__OAEPDecode                                       1
#define PERF_FUNC__GenerateMGF1Mask                                 2
#define PERF_FUNC__OAEPEncode                                       3
#define PERF_FUNC__PSSEncode                                        4
#define PERF_FUNC__PSSVerify                                        5
#define PERF_FUNC__BigEndianBytesToDigits                           6
#define PERF_FUNC__DigitsToBigEndianBytes                           7
#define PERF_FUNC_OEM_RSA_SetPublicKey                              8
#define PERF_FUNC_OEM_RSA_SetPrivateKey                             9
#define PERF_FUNC_OEM_RSA_ParsePublicKey                            10
#define PERF_FUNC_OEM_RSA_ZeroPublicKey                             11
#define PERF_FUNC_OEM_RSA_ZeroPrivateKey                            12
#define PERF_FUNC__ModularExponentiate                              13
#define PERF_FUNC_OEM_RSA_Decrypt                                   14

/* ND Certificates */
#define PERF_FUNC__AddDSigPublicKeyNode                             1
#define PERF_FUNC__AddPublicKeyNode                                 2
#define PERF_FUNC_DRM_ND_CB_CreateDigitalSignature                  3
#define PERF_FUNC_DRM_ND_CB_CreateCertificateInit                   4
#define PERF_FUNC_DRM_ND_CB_CreateCertificateAddTemplateData        5
#define PERF_FUNC_DRM_ND_CB_CreateCertificateSignAndClose           6
#define PERF_FUNC_DRM_NDCert_CreateDeviceCertificate                7
#define PERF_FUNC_DRM_ND_CB_AddAttribute                            8
#define PERF_FUNC_DRM_ND_CB_AddManufacturerNodes                    9
#define PERF_FUNC_DRM_ND_CB_SignCertificate                        10
#define PERF_FUNC_DRM_ND_CB_CreateCertificateFile                  11
#define PERF_FUNC_DRM_XML_RSA_WritePublicKeyNodeA                  12
#define PERF_FUNC_DRM_XML_RSA_WritePublicKeyNode                   13
#define PERF_FUNC__ConvertB64DStrToASCII                           14
#define PERF_FUNC_DRM_XML_RSA_ParseBase64PublicKey                 15
#define PERF_FUNC_DRM_XML_RSA_ParseBase64PublicKeyA                16
#define PERF_FUNC_DRM_XML_RSA_WritePrivateKeyNode                  17
#define PERF_FUNC_DRM_XML_RSA_ParseBase64PrivateKey                18
#define PERF_FUNC_DRM_NDCert_Certificate_Initialize                19
#define PERF_FUNC_DRM_NDCert_GetDeviceCertificate                  20
#define PERF_FUNC__VerifyRSACertCollectionOnDevice                 21
#define PERF_FUNC__GetModelRSAPrivKeyOnDevice                      22
#define PERF_FUNC__UpdateDeviceCertificateProperties               23
#define PERF_FUNC_DRM_ND_CB_CreateCertificate                      24


/* NDT */
#define PERF_FUNC_DRM_WCP_GetCertificateType                        1
#define PERF_FUNC_DRM_WCP_GetAttribute                              2
#define PERF_FUNC__ExtractValueFromAttribute                        3
#define PERF_FUNC_DRM_WCP_GetKeyUsage                               4
#define PERF_FUNC_DRM_WCP_GetFeatures                               5
#define PERF_FUNC_DRM_WCP_GetSecurityLevel                          6
#define PERF_FUNC_DRM_WCP_GetPublicKey                              7
#define PERF_FUNC_DRM_WCP_GetHardwareID                             8
#define PERF_FUNC_DRM_WCP_GetSignatureDigest                        9
#define PERF_FUNC_DRM_WCP_GetManufacturerNodes                      10
#define PERF_FUNC_DRM_WCP_VerifyCertificate                         11
#define PERF_FUNC__VerifyCertificateMethods                         12
#define PERF_FUNC__VerifyCertificateSignature                       13
#define PERF_FUNC__VerifyCertificateRequiredTags                    14
#define PERF_FUNC_DRM_WCP_VerifyCertificateCollection               15
#define PERF_FUNC__GetPublicKey                                     16
#define PERF_FUNC_DRM_WCP_VerifyDigitalSignature                    17
#define PERF_FUNC__ExtractPublicKey                                 18
#define PERF_FUNC_Drm_Pfd_Initialize                                19
#define PERF_FUNC_Drm_Pfd_Registration_ProcessRequest               20
#define PERF_FUNC_Drm_Pfd_Registration_GenerateResponse             21
#define PERF_FUNC_Drm_Pfd_Proximity_GenerateChallenge               22
#define PERF_FUNC_Drm_Pfd_Proximity_ProcessResponse                 23
#define PERF_FUNC_Drm_Pfd_License_ProcessRequest                    24
#define PERF_FUNC_Drm_Pfd_License_GenerateResponse                  25
#define PERF_FUNC__OutputLevelsCallback                             26
#define PERF_FUNC_NDT_RetrieveLicenseDataV1                         27
#define PERF_FUNC__RetrieveLicense                                  28
#define PERF_FUNC_NDT_RetrieveLicenseDataV2                         29
#define PERF_FUNC__DRM_XMR_RootSignatureCallback                    30
#define PERF_FUNC__DRM_XMR_LeafSignatureCallback                    31
#define PERF_FUNC_NDT_License_BuildSimpleXMR                        32
#define PERF_FUNC_NDT_License_BuildRootXMR                          33
#define PERF_FUNC_NDT_License_BuildLeafXMR                          34
#define PERF_FUNC_Drm_Ndt_Initialize                                35
#define PERF_FUNC_Drm_Ndt_Session_Create                            36
#define PERF_FUNC_Drm_Ndt_Session_Close                             37
#define PERF_FUNC_Drm_Ndt_Registration_ProcessRequest               38
#define PERF_FUNC_DRM_NDT_Proximity_GetChallenge                    39
#define PERF_FUNC_Drm_Ndt_Proximity_ProcessStartMsg                 40
#define PERF_FUNC_DRM_NDT_Proximity_VerifyEncryptedChallenge        41
#define PERF_FUNC__DecryptNonce                                     42
#define PERF_FUNC_Drm_Ndt_Proximity_ProcessResponseMsg              43
#define PERF_FUNC_Drm_Ndt_License_ProcessChallenge                  44
#define PERF_FUNC_DRM_NDT_License_GenerateKeys                      45
#define PERF_FUNC__GetDeviceHandle                                  46
#define PERF_FUNC_DRM_NDT_License_SetRootPolicy                     47
#define PERF_FUNC_Drm_Ndt_License_GenerateLeafResponse              48
#define PERF_FUNC_Drm_Ndt_RevocationList_GenerateChallenge          49
#define PERF_FUNC_Drm_Ndt_RevocationList_ProcessRequest             50
#define PERF_FUNC__GUIDFromDrmRevocationTypeEnum                    51
#define PERF_FUNC_Drm_Ndt_RevocationList_ProcessResponse            52
#define PERF_FUNC_GetWMDRMNETRevocationEntries                      53
#define PERF_FUNC_GetWMDRMNDRevocationList                          54
#define PERF_FUNC_NDTMGR_Internal_FindSession                       55
#define PERF_FUNC_NDTMGR_Internal_CreateSession                     56
#define PERF_FUNC_NDTMGR_Internal_RemoveSession                     57
#define PERF_FUNC__NdtManagerFreeContentSession                     58
#define PERF_FUNC_NDT_ValidateCertificate                           59
#define PERF_FUNC_NDT_GetCertificateAttributes                      60
#define PERF_FUNC_NDT_EncryptSeed                                   61
#define PERF_FUNC_NDT_SignRegistrationMessage                       62
#define PERF_FUNC_NDTMGR_Encrypt                                    63
#define PERF_FUNC__PackDataSegmentDescriptor                        64
#define PERF_FUNC_Drm_Pfd_Aavt_OpenMediaSession                     65
#define PERF_FUNC_Drm_Pfd_Aavt_CloseMediaSession                    66
#define PERF_FUNC_Drm_Pfd_Aavt_GetNextDataBlock                     67
#define PERF_FUNC_Drm_Pfd_Aavt_SetCurrentTimePosition               68
#define PERF_FUNC__PfdDrmManagerFreeContentSession                  69
#define PERF_FUNC__PfdDrmManagerFreeAllContentSessions              70
#define PERF_FUNC__PfdDrmManagerGetSession                          71
#define PERF_FUNC_OEM_NDT_AllowNewDeviceRegistration                72
#define PERF_FUNC_OEM_NDT_GetProximityDetectionAddress              73
#define PERF_FUNC_OEM_NDT_GetLicenseOPLs                            74
#define PERF_FUNC_OEM_NDT_GetLicenseInclusionList                   75
#define PERF_FUNC_OEM_NDT_GetSourceID                               76
#define PERF_FUNC_OEM_NDT_GetLicenseRevInfoVersion                  77
#define PERF_FUNC_OEM_NDT_GetWMDRMNETMinAppSec                      78

/* NDT NetMsg */
#define PERF_FUNC_WmdrmNetAllocateMessageDescriptor                 1
#define PERF_FUNC_WmdrmNetValidateMessage                           2
#define PERF_FUNC_WmdrmNetUnpackRegistrationRequestMessage          3
#define PERF_FUNC_WmdrmNetPackRegistrationResponseMessage           4
#define PERF_FUNC_WmdrmNetPackRegistrationMessage                   5
#define PERF_FUNC_WmdrmNetUnpackRegistrationResponseMessage         6
#define PERF_FUNC_WmdrmNetUnpackProximityStartMessage               7
#define PERF_FUNC_WmdrmNetPackProximityChallengeMessage             8
#define PERF_FUNC_WmdrmNetUnpackProximityResponseMessage            9
#define PERF_FUNC_WmdrmNetPackProximityResultMessage                10
#define PERF_FUNC_WmdrmNetPackProximityResponseMessage              11
#define PERF_FUNC_WmdrmNetUnpackProximityChallengeMessage           12
#define PERF_FUNC_WmdrmNetUnpackProximityResultMessage              13
#define PERF_FUNC_WmdrmNetPackProximityStartMessage                 14
#define PERF_FUNC_WmdrmNetUnpackLicenseRequestMessage               15
#define PERF_FUNC_WmdrmNetPackLicenseResponseMessage                16
#define PERF_FUNC_WmdrmNetPackLicenseRequestMessage                 17
#define PERF_FUNC_WmdrmNetUnpackLicenseResponseMessage              18
#define PERF_FUNC_WmdrmNetUnpackRevocationListRequestMessage        19
#define PERF_FUNC_WmdrmNetPackRevocationListResponseMessage         20
#define PERF_FUNC_WmdrmNetPackRevocationListRequestMessage          21
#define PERF_FUNC_WmdrmNetUnpackRevocationListResponseMessage       22

/* NDT ASF */
#define PERF_FUNC__CreateDefaultPolicy                              1
#define PERF_FUNC_Drm_Ndt_Asf_Init                                  2
#define PERF_FUNC_Drm_Ndt_Asf_InitTranscryption                     3
#define PERF_FUNC_Drm_Ndt_Asf_GetHeader                             4
#define PERF_FUNC_Drm_Ndt_Asf_GenerateLeafLicenseResponse           5
#define PERF_FUNC_Drm_Ndt_Asf_Seek                                  6
#define PERF_FUNC_Drm_Ndt_Asf_ReadData                              7
#define PERF_FUNC_Drm_Ndt_Asf_ReadSample                            8
#define PERF_FUNC_Drm_Ndt_Asf_Close                                 9
#define PERF_FUNC_NDTMGR_ASF_Internal_CheckFileDRM                  10
#define PERF_FUNC__ASFParsePacketHeader                             11
#define PERF_FUNC__ASFReadData                                      12
#define PERF_FUNC__ASFParseLengthType                               13
#define PERF_FUNC__ASFParsePayloadHeader                            14
#define PERF_FUNC_NDTMGR_ASF_Internal_InitializeTranscryption       15
#define PERF_FUNC_NDTMGR_ASF_Internal_Initialize                    16
#define PERF_FUNC_NDTMGR_ASF_Internal_GetFileInfo                   17
#define PERF_FUNC__ASFParseHeaders                                  18
#define PERF_FUNC__ASFLoadHeaderObject                              19
#define PERF_FUNC__ASFLoadObjectHeader                              20
#define PERF_FUNC__ASFParseFilePropertiesObject                     21
#define PERF_FUNC__ASFParseStreamPropertiesObject                   22
#define PERF_FUNC__AddToStreamInfoList                              23
#define PERF_FUNC__FindStreamInStreamInfoList                       24
#define PERF_FUNC__ASFParseHeaderExtensionObject                    25
#define PERF_FUNC__ASFParseStreamPropertiesObjectEx                 26
#define PERF_FUNC__ASFParseStreamBitratePropertiesObject            27
#define PERF_FUNC_NDTMGR_ASF_Internal_TranscryptNextBlock           28
#define PERF_FUNC__ASFSetHeaderDone                                 29
#define PERF_FUNC__ASFTranscryptHeader                              30
#define PERF_FUNC__ASFProcessHeaders                                31
#define PERF_FUNC__CopyObjectToHeaderBuffer                         32
#define PERF_FUNC__AppendToHeaderBuffer                             33
#define PERF_FUNC__SaveHeaderObjectToNewHeader                      34
#define PERF_FUNC__ASFProcessContentEncryptionObject                35
#define PERF_FUNC__ASFProcessFilePropertiesObject                   36
#define PERF_FUNC__ASFProcessStreamPropertiesObject                 37
#define PERF_FUNC__ASFProcessHeaderExtensionObject                  38
#define PERF_FUNC__ASFProcessStreamPropertiesObjectEx               39
#define PERF_FUNC__ASFAppendStreamPropertiesObjectEx                40
#define PERF_FUNC__ASFAppendAdvContentEncryptionObject              41
#define PERF_FUNC__ASFTranscryptNextPackets                         42
#define PERF_FUNC__ASFTranscryptData                                43
#define PERF_FUNC__ASFTranscryptPacket                              44
#define PERF_FUNC__AppendToDataBuffer                               45
#define PERF_FUNC__ASFProcessPacketHeader                           46
#define PERF_FUNC__ParseLengthType                                  47
#define PERF_FUNC__ASFProcessPayloadHeader                          48
#define PERF_FUNC__DRMDecrypt                                       49
#define PERF_FUNC__ASFTranscryptDataObjectHeader                    50
#define PERF_FUNC__FillOutputBufferWithMuxPackets                   51
#define PERF_FUNC__ASFSeekToTimePosition                            52
#define PERF_FUNC_NDTMGR_ASF_Internal_SetTimePosition               53
#define PERF_FUNC_NDTMGR_ASF_Internal_Uninitialize                  54
#define PERF_FUNC_NDTMGR_ASF_Internal_GetHeader                     55
#define PERF_FUNC_NDTMGR_ASF_Internal_ReadSample                    56
#define PERF_FUNC__GetNextReadySample                               57
#define PERF_FUNC__GetSampleInProgress                              58
#define PERF_FUNC_NDTMGR_ASF_Internal_ReadData                      59
#define PERF_FUNC__ASFSetupTranscryption                            60

/* NDT ASF Sample */
#define PERF_FUNC_ASFSample_Initialize                              1
#define PERF_FUNC_ASFSample_GetBufferByIndex                        2
#define PERF_FUNC_ASFSample_AddBuffer                               3
#define PERF_FUNC_ASFSample_CopyToBuffer                            4

/* NDT ASF Mux */
#define PERF_FUNC_ASFMux_Initialize                                 1
#define PERF_FUNC_ASFMux_ProcessSample                              2
#define PERF_FUNC_ASFMux_GetNextPacket                              3
#define PERF_FUNC_ASFMux_Flush                                      4
#define PERF_FUNC_ASFMux_OnCompletedPacket                          5
#define PERF_FUNC_ASFMux_GetMinStreamSendtimeCutoff                 6
#define PERF_FUNC_ASFMux_AdjustSendTimeForOffset                    7
#define PERF_FUNC_CreateMuxStream                                   8
#define PERF_FUNC_SizeQueueObjectsArray                             9
#define PERF_FUNC_ASFMuxStream_Initialize                           10
#define PERF_FUNC_ASFMuxStream_HowManyObjectsWillBeReturned         11
#define PERF_FUNC_ASFMuxStream_OnSample                             12
#define PERF_FUNC_OnSample                                          13
#define PERF_FUNC_ASFMuxStream_GetObjectSizeForBandwidthPolicing    14
#define PERF_FUNC_ASFMuxStream_ApproxSampleDuration                 15
#define PERF_FUNC_ApproxSampleDuration                              16
#define PERF_FUNC_ASFMuxStream_GetEndTime                           17
#define PERF_FUNC_GetEndTime                                        18
#define PERF_FUNC_ASFMuxStream_GetHappyCutoffSendtime               19
#define PERF_FUNC_GetHappyCutoffSendtime                            20
#define PERF_FUNC_ASFMuxStream_AdjustSendTimeForOffset              21
#define PERF_FUNC_AdjustSendTimeForOffset                           22
#define PERF_FUNC_ASFMuxStream_UpdateStreamPropertiesEx             23
#define PERF_FUNC_ASFMuxStream_IsAvgTimePerFrameKnown               24
#define PERF_FUNC_ASFMuxStream_ReadMediaType                        25
#define PERF_FUNC_InternalInit                                      26
#define PERF_FUNC_SetupLeakyBucket                                  27
#define PERF_FUNC_ProcessNewWriterSample                            28
#define PERF_FUNC_SendTimeFromPresTime                              29
#define PERF_FUNC_GetAdjustedSampleTime                             30
#define PERF_FUNC_ASFAudioMuxStream_HowManyObjectsWillBeReturned    31
#define PERF_FUNC_ASFAudioMuxStream_OnSample                        32
#define PERF_FUNC_ASFAudioMuxStream_ApproxSampleDuration            33
#define PERF_FUNC_ASFAudioMuxStream_GetHappyCutoffSendtime          34
#define PERF_FUNC_ASFVideoMuxStream_OnSample                        35
#define PERF_FUNC_ASFVideoMuxStream_ApproxSampleDuration            36
#define PERF_FUNC_ASFVideoMuxStream_GetHappyCutoffSendtime          37
#define PERF_FUNC_ASFVideoMuxStream_ReadMediaType                   38

/* NDT ASF Streaming */
#define PERF_FUNC_ASFMuxSendWindow_Initialize                       1
#define PERF_FUNC_ASFMuxSendWindow_AddObjectsAndSendPackets         2
#define PERF_FUNC_ASFMuxSendWindow_Flush                            3
#define PERF_FUNC_InitializeWritePacket                             4
#define PERF_FUNC_PacketizeAndSend                                  5
#define PERF_FUNC_PacketizeSample                                   6
#define PERF_FUNC_AddPayload                                        7
#define PERF_FUNC_FindFirstSampleThatFits                           8
#define PERF_FUNC_SendOffWritePacket                                9
#define PERF_FUNC_PackageAndSend                                    10
#define PERF_FUNC_HandleFinishedQueueObject                         11
#define PERF_FUNC_ASFMuxQueueObject_Initialize                      12
#define PERF_FUNC_ASFMuxQueueObject_GeneratePayload                 13
#define PERF_FUNC_ASFMuxQueueObject_FillInASFPayloadInfo            14
#define PERF_FUNC_ASFMuxQueueObject_PopUsedSample                   15
#define PERF_FUNC_ASFMuxQueueObject_AddSegment                      16
#define PERF_FUNC_FragmentableObject_GeneratePayload                17
#define PERF_FUNC_FragmentableObject_PopUsedSample                  18
#define PERF_FUNC_UnfragmentableObject_Initialize                   19
#define PERF_FUNC_UnfragmentableObject_GeneratePayload              20
#define PERF_FUNC_UnfragmentableObject_PopUsedSample                21
#define PERF_FUNC_UnfragmentableObject_AddSegment                   22
#define PERF_FUNC_UnfragmentableObject_StartMFSample                23
#define PERF_FUNC_ASFLeakyBucket_AddToBucket                        24
#define PERF_FUNC_ASFPacketWriter_Initialize                        25
#define PERF_FUNC_ASFPacketWriter_SetPacketInfo                     26
#define PERF_FUNC_ASFPacketWriter_GetPacketInfo                     27
#define PERF_FUNC_ASFPacketWriter_GetDataSpaceAvailable             28
#define PERF_FUNC_ASFPacketWriter_AddPayload                        29
#define PERF_FUNC_ASFPacketWriter_WritePacket                       30
#define PERF_FUNC_ASFPacketWriter_Clear                             31
#define PERF_FUNC_InitializePacketInfoForWrite                      32
#define PERF_FUNC_GetCurrentPacketSnapshot                          33
#define PERF_FUNC_RestorePacket                                     34
#define PERF_FUNC_InternalAddPayloadInfo                            35
#define PERF_FUNC_CalcPayloadSize                                   36
#define PERF_FUNC_GetReplicatedData                                 37
#define PERF_FUNC_WritePacketHeader                                 38
#define PERF_FUNC_WritePayloadHeader                                39
#define PERF_FUNC_WritePayloadData                                  40
#define PERF_FUNC_WritePadding                                      41
#define PERF_FUNC_WritePayloadDataWithoutEncrypt                    42
#define PERF_FUNC_SaveVarField                                      43
#define PERF_FUNC_ASFMediaBuffer_Initialize                         44
#define PERF_FUNC_ASFMediaBuffer_InitializeWrapper                  45


/* core\drmembedding.c */
#define PERF_FUNC_DRM_EMB_UpdateEmbeddedStore                       1

/* core\drmxmlbuilder.c & core\drmxmlbuildera.c */
#define PERF_FUNC_DRM_XMB_CreateDocumentA                  1
#define PERF_FUNC_DRM_XMB_CloseDocumentA                   2
#define PERF_FUNC_DRM_XMB_OpenNodeA                        3
#define PERF_FUNC_DRM_XMB_CloseCurrNodeA                   4
#define PERF_FUNC_DRM_XMB_AddAttributeA                    5
#define PERF_FUNC_DRM_XMB_AddDataA                         6
#define PERF_FUNC_DRM_XMB_WriteTagA                        7
#define PERF_FUNC_DRM_XMB_AddCDataA                        8
#define PERF_FUNC_DRM_XMB_WriteCDATATagA                   9
#define PERF_FUNC_DRM_XMB_ReserveSpaceA                   10
#define PERF_FUNC_DRM_XMB_ShiftDataFromCurrentPositionA   11
#define PERF_FUNC_DRM_XMB_ShiftCurrentPointerA            12
#define PERF_FUNC_DRM_XMB_GetCurrentBufferPointerA        13
#define PERF_FUNC_DRM_XMB_AppendNodeA                     14

#define PERF_FUNC_DRM_XMB_AESEncryptAndCloseCurrNodeA     15
#define PERF_FUNC_DRM_XMB_RSASignAndCloseCurrNodeA        16
#define PERF_FUNC_DRM_XMB_HashAndRSASignAndCloseCurrNodeA 17
#define PERF_FUNC_DRM_XMB_HashAndCloseCurrNodeA           18

/* Start wide-char function with some gap in values */
#define PERF_FUNC_DRM_XMB_SimulateCreateDocument          21
#define PERF_FUNC_DRM_XMB_CreateDocument                  22
#define PERF_FUNC_DRM_XMB_ReallocDocument                 23
#define PERF_FUNC_DRM_XMB_CloseDocument                   24
#define PERF_FUNC_DRM_XMB_OpenNode                        25
#define PERF_FUNC_DRM_XMB_CloseCurrNode                   26
#define PERF_FUNC_DRM_XMB_GetCurrNodeName                 27
#define PERF_FUNC_DRM_XMB_AddAttribute                    28
#define PERF_FUNC_DRM_XMB_AddData                         29
#define PERF_FUNC_DRM_XMB_ReserveSpace                    30
#define PERF_FUNC_DRM_XMB_AddCData                        31
#define PERF_FUNC_DRM_XMB_AppendNode                      32
#define PERF_FUNC_DRM_XMB_AddXMLNode                      33
#define PERF_FUNC_DRM_XMB_WriteTag                        34
#define PERF_FUNC_DRM_XMB_WriteCDATATag                   35

#define PERF_FUNC_DRM_XMB_EncryptAndCloseCurrNode         36
#define PERF_FUNC_DRM_XMB_SignAndCloseCurrNode            37
#define PERF_FUNC_DRM_XMB_KeyedHashAndCloseCurrNode       38

#endif   /* #ifndef __DRMPROFILECONSTANTS_H__ */
