#ifndef __ASF_DRM__H__
#define __ASF_DRM__H__

#if defined(ASF_SUPPORT_DRM)
	#define IS_DRM_ASF(_this) (_this->extendedContentEncryptionDataCount != 0)
	
	#ifdef SUPPORT_WMDRM_PD                         
		#define ASF_DRM_DECRYPT(_drmContext, _ctrContext, _payload, _payloadSize)		DRM_MGR_Decrypt(_drmContext, _payload, _payloadSize)
		#define	ASF_DRM_INITDECRYPT(_drmContext, _payload, _payloadSize)	DRM_MGR_InitDecrypt(_drmContext, _payload, _payloadSize)
		#define ASF_DRM_INITIALIZE(_drmManager, _oemContext, _opaqueBuf, _opaqueBufSize, _licenseStoreName)			DRM_MGR_Initialize(_drmManager, _licenseStoreName)
		#define ASF_DRM_MEMALLOC(_size)										OEM_malloc(_size)
		#define ASF_DRM_MEMFREE(_ptr)										OEM_free(_ptr)
		#define ASF_DRM_UNINITIALIZE(_drmManager)							DRM_MGR_Uninitialize(_drmManager)
		#define ASF_DRM_COMMIT(_drmManager)									DRM_MGR_Commit(_drmManager)
		#define ASF_DRM_BIND(_drmManager, _rights, _numRights, _pOutLevelCB, _outLevel, _drmContext)	DRM_MGR_Bind (_drmManager, _rights, _numRights, _pOutLevelCB, _outLevel, _drmContext)
		#define ASF_DRM_GENERATELICENSECHALLENGE(_drmManager, _rights, _numRights, _pUrl, _pUrlLen, _pChallenge, _pChallengeLen) DRM_MGR_GenerateLicenseChallenge (_drmManager, _rights, _numRights, _pUrl, _pUrlLen, _pChallenge, _pChallengeLen )
		#define ASF_DRM_FILETIME_TO_SYSTEMTIME(_filetime, _systemtime)		OEM_FileTimeToSystemTime(_filetime, _systemtime)
		#define ASF_DRM_SET_PROPERTY(_drmManage, _propertyData, _propertySize)	DRM_MGR_SetV2Header(_drmManage, _propertyData, _propertySize)
		#define ASF_DRM_GET_LICENSE_DATA(_drmManage, _rights, _numRights, _stateData, _cb, _pv)	DRM_MGR_GetLicenseData(_drmManage, _rights, _stateData, _numRights)
	#else
		#define ASF_DRM_DECRYPT(_drmContext, _ctrContext, _payload, _payloadSize)		Drm_Reader_Decrypt (_drmContext, _ctrContext, _payload, _payloadSize)
		#define ASF_DRM_INITDECRYPT(_drmContext, _payload, _payloadSize)	Drm_Reader_InitDecrypt(_drmContext, _payload, _payloadSize)
		#define ASF_DRM_MEMALLOC(_size)										Oem_MemAlloc(_size)
		#define ASF_DRM_MEMFREE(_ptr)										Oem_MemFree(_ptr)
		#define ASF_DRM_UNINITIALIZE(_drmManager)							Drm_Uninitialize(_drmManager)
		#define ASF_DRM_COMMIT(_drmManager)									Drm_Reader_Commit(_drmManager, NULL, NULL )
		#define ASF_DRM_BIND(_drmManager, _rights, _numRights, _pOutLevelCB, _outLevel, _drmContext)	Drm_Reader_Bind(_drmManager, _rights, _numRights, _pOutLevelCB, NULL, _drmContext)
		#define ASF_DRM_GENERATELICENSECHALLENGE(_drmManager, _rights, _numRights, _pUrl, _pUrlLen, _pChallenge, _pChallengeLen) Drm_LicenseAcq_GenerateChallenge (_drmManager, _rights, _numRights, NULL, NULL, 0, _pUrl, _pUrlLen, NULL, NULL, _pChallenge, _pChallengeLen)
		#define ASF_DRM_FILETIME_TO_SYSTEMTIME(_filetime, _systemtime)		Oem_Clock_FileTimeToSystemTime(_filetime, _systemtime)
		#define ASF_DRM_SET_PROPERTY(_drmManage, _propertyData, _propertySize)	Drm_Content_SetProperty(_drmManage, DRM_CSP_AUTODETECT_HEADER, _propertyData, _propertySize)
		#define ASF_DRM_GET_LICENSE_DATA(_drmManage, _rights, _numRights, _stateData, _cb, _pv)	Drm_LicenseQuery_GetState(_drmManage, _rights, _numRights, _stateData, _cb, _pv)
		
		#if defined(SUPPORT_PLAYREADY)
			#define ASF_DRM_INITIALIZE(_drmManager, _oemContext, _opaqueBuf, _opaqueBufSize, _licenseStoreName)			Drm_Initialize(_drmManager, _oemContext, _licenseStoreName)
			#define ASF_DRM_PROC_RESPONSE(_drmManager, _flag, _cb, _cbContext, _response, _responseSize, _result)	Drm_LicenseAcq_ProcessResponse(_drmManager, _cb, _cbContext, _response, _responseSize, _result)
		#elif defined(SUPPORT_PLAYREADY20)
			#define ASF_DRM_INITIALIZE(_drmManager, _oemContext, _opaqueBuf, _opaqueBufSize, _licenseStoreName)			Drm_Initialize(_drmManager, _oemContext, _opaqueBuf, _opaqueBufSize, _licenseStoreName)
			#define ASF_DRM_PROC_RESPONSE(_drmManager, _flag, _cb, _cbContext, _response, _responseSize, _result)		Drm_LicenseAcq_ProcessResponse(_drmManager, _flag, _cb, _cbContext, _response, _responseSize, _result)
		#else
			#error "unknown drm library!"
		#endif
	#endif
	#ifdef SUPPORT_WMDRM_PD 
		#define ASF_DRM_LICENSE_STATE_NORIGHT				WM_DRM_LICENSE_STATE_NORIGHT
		#define ASF_DRM_LICENSE_STATE_UNLIM					WM_DRM_LICENSE_STATE_UNLIM
		#define ASF_DRM_LICENSE_STATE_COUNT					WM_DRM_LICENSE_STATE_COUNT
		#define ASF_DRM_LICENSE_STATE_FROM					WM_DRM_LICENSE_STATE_FROM
		#define ASF_DRM_LICENSE_STATE_UNTIL					WM_DRM_LICENSE_STATE_UNTIL
		#define ASF_DRM_LICENSE_STATE_FROM_UNTIL			WM_DRM_LICENSE_STATE_FROM_UNTIL
		#define ASF_DRM_LICENSE_STATE_COUNT_FROM			WM_DRM_LICENSE_STATE_COUNT_FROM
		#define ASF_DRM_LICENSE_STATE_COUNT_UNTIL			WM_DRM_LICENSE_STATE_COUNT_UNTIL
		#define ASF_DRM_LICENSE_STATE_COUNT_FROM_UNTIL		WM_DRM_LICENSE_STATE_COUNT_FROM_UNTIL
		#define ASF_DRM_LICENSE_STATE_EXPIRE_AFTER_FIRSTUSE	WM_DRM_LICENSE_STATE_EXPIRATION_AFTER_FIRSTUSE
	#else
		#define ASF_DRM_LICENSE_STATE_NORIGHT				DRM_LICENSE_STATE_NORIGHT
		#define ASF_DRM_LICENSE_STATE_UNLIM					DRM_LICENSE_STATE_UNLIM
		#define ASF_DRM_LICENSE_STATE_COUNT					DRM_LICENSE_STATE_COUNT
		#define ASF_DRM_LICENSE_STATE_FROM					DRM_LICENSE_STATE_FROM
		#define ASF_DRM_LICENSE_STATE_UNTIL					DRM_LICENSE_STATE_UNTIL
		#define ASF_DRM_LICENSE_STATE_FROM_UNTIL			DRM_LICENSE_STATE_FROM_UNTIL
		#define ASF_DRM_LICENSE_STATE_COUNT_FROM			DRM_LICENSE_STATE_COUNT_FROM
		#define ASF_DRM_LICENSE_STATE_COUNT_UNTIL			DRM_LICENSE_STATE_COUNT_UNTIL
		#define ASF_DRM_LICENSE_STATE_COUNT_FROM_UNTIL		DRM_LICENSE_STATE_COUNT_FROM_UNTIL
		#define ASF_DRM_LICENSE_STATE_EXPIRE_AFTER_FIRSTUSE	DRM_LICENSE_STATE_EXPIRATION_AFTER_FIRSTUSE
	#endif
	
	
	
#else
	#define IS_DRM_ASF(_this) 0
#endif

void asf_drm_init_drm_thread(asf_input_plugin_t* this);

int asf_drm_propertySetGet(void* pInstance, NAV_PROP_ID id, 
    void* inData, unsigned int inDataSize,
    void* outData, unsigned int outDataSize, unsigned int* returnedSize);

int  asf_drm_get_actual_read_size(asf_input_plugin_t* this, int payload_len, NAVBUF* pBuffer);
void asf_drm_deinit_context(asf_input_plugin_t* this);
void asf_drm_init_context(asf_input_plugin_t* this);
int asf_drm_decrypt(asf_input_plugin_t* this, NAVBUF* pBuffer, unsigned char* pPayload, int payload_len);

#endif //__ASF_DRM__H__