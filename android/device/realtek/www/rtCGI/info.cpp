#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#include <stdlib.h>
#include <string.h>

#include <cutils/properties.h>
#include <Application/AppClass/SetupClassAndroid.h>

#include <sys/utsname.h>

#include "info.h"
#include "response.h"
#include "rtCGI.h"

#define PROP_DEFAULT_VALUE	"NULL"

// the pointer must be free if not used
static char *getDeviceName()
{
    const char *dev_name = SetupClass::GetInstance()->GetDeviceName();
    if (dev_name == NULL)
        return strdup("A20"); // default name
    else
    	return strdup(dev_name);
}

static SOFTWARE_INFO_T *getSoftwareInfo()
{
	char prop_value[PROPERTY_VALUE_MAX] = {0};
	
	SOFTWARE_INFO_T *pSwInfo = (SOFTWARE_INFO_T*) malloc(sizeof(SOFTWARE_INFO_T));
	memset(pSwInfo, 0, sizeof(SOFTWARE_INFO_T));

	strncpy(prop_value, SetupClass::GetInstance()->getProperty("ro.product.brand", PROP_DEFAULT_VALUE), PROPERTY_VALUE_MAX);
	if (strncmp(prop_value, PROP_DEFAULT_VALUE, strlen(PROP_DEFAULT_VALUE)))
		pSwInfo->mVendor = strdup(prop_value);

	strncpy(prop_value, SetupClass::GetInstance()->getProperty("ro.product.model", PROP_DEFAULT_VALUE), PROPERTY_VALUE_MAX);
	if (strncmp(prop_value, PROP_DEFAULT_VALUE, strlen(PROP_DEFAULT_VALUE)))
		pSwInfo->mProduct = strdup(prop_value);

	strncpy(prop_value, SetupClass::GetInstance()->getProperty("ro.build.version.release", PROP_DEFAULT_VALUE), PROPERTY_VALUE_MAX);
	if (strncmp(prop_value, PROP_DEFAULT_VALUE, strlen(PROP_DEFAULT_VALUE)))
		pSwInfo->mAndroidVersion = strdup(prop_value);

	strncpy(prop_value, SetupClass::GetInstance()->getProperty("ro.build.display.id", PROP_DEFAULT_VALUE), PROPERTY_VALUE_MAX);
	if (strncmp(prop_value, PROP_DEFAULT_VALUE, strlen(PROP_DEFAULT_VALUE)))
		pSwInfo->mAndroidVersionDetail = strdup(prop_value);

	snprintf(prop_value,PROPERTY_VALUE_MAX, "%d.%d.%d", CGI_MAJOR_VER, CGI_MINOR_VER/10, CGI_MINOR_VER%10);
	pSwInfo->mCgiVersion = strdup(prop_value);

	utsname kernelInfo;
	memset(&kernelInfo, 0, sizeof(utsname));	
	uname(&kernelInfo);
	snprintf(prop_value, PROPERTY_VALUE_MAX, "%s %s", kernelInfo.sysname, kernelInfo.release);
	pSwInfo->mKernelVersion = strdup(prop_value);
	
	/**
	mDvdPlayerVersion
	mAudioVersion
	mVideoVersion **/

	return pSwInfo;
}

static HARDWARE_INFO_T *getHardwareInfo()
{
	char prop_value[PROPERTY_VALUE_MAX] = {0};
	HARDWARE_INFO_T *pHwInfo = (HARDWARE_INFO_T*) malloc(sizeof(HARDWARE_INFO_T));
	memset(pHwInfo, 0, sizeof(HARDWARE_INFO_T));

	strncpy(prop_value, SetupClass::GetInstance()->getProperty("ro.product.manufacturer", PROP_DEFAULT_VALUE), PROPERTY_VALUE_MAX);
	if (strncmp(prop_value, PROP_DEFAULT_VALUE, strlen(PROP_DEFAULT_VALUE)))
		pHwInfo->mVendor = strdup(prop_value);

	strncpy(prop_value, SetupClass::GetInstance()->getProperty("ro.product.device", PROP_DEFAULT_VALUE), PROPERTY_VALUE_MAX);
	if (strncmp(prop_value, PROP_DEFAULT_VALUE, strlen(PROP_DEFAULT_VALUE)))
		pHwInfo->mChip = strdup(prop_value);
	
	return pHwInfo;
}


int handleQueryInfo(void)
{
	char *info_type = NULL;
	int infoSet = 0;
	CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));
	memset(pCgiRet, 0, sizeof(CGI_RESULT_T));

	pCgiRet->type = CGI_RESULT_QUERYINFO;	
	
	do 
	{
		if (cgi_get_svalue("info", &info_type) != CGI_OK)
		{
			pCgiRet->QueryInfoResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->QueryInfoResult.mErrStr, MAX_SIZE_ERROR_STR, "Invalid parameters");
			break;	
		}

		char *p = NULL;
		infoSet |= cmd_string2id(info_type, tbl_InfoSet, SIZEOF_ARRAY(tbl_InfoSet));
		p = strchr(info_type, '|');
		while (p != NULL) {
			infoSet |= cmd_string2id(p+1, tbl_InfoSet, SIZEOF_ARRAY(tbl_InfoSet));
			p = strchr(p+1, '|');			
		}

		if (infoSet == 0)
		{
			pCgiRet->GetSetSettingItemResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->GetSetSettingItemResult.mErrStr, MAX_SIZE_ERROR_STR, "Invalid parameters");
			break;				
		}

		if (infoSet & INFO_DEVICE_NAME)
		{
			// Fill device name
			pCgiRet->QueryInfoResult.mDeviceName = getDeviceName();			
		}

		if (infoSet & INFO_SOFTWARE)
		{
			// Fill software info
			pCgiRet->QueryInfoResult.mSwInfo = getSoftwareInfo();
		}

		if (infoSet & INFO_HARDWARE)
		{
			// Fill hardware info
			pCgiRet->QueryInfoResult.mHwInfo= getHardwareInfo();
		}

		pCgiRet->QueryInfoResult.mRet = CGI_OK;
		
	} while(0);

	dumpCgiResult(pCgiRet);

	SAFE_FREE_QUERY_INFO_RESULT_T(pCgiRet->QueryInfoResult);
	SAFE_FREE(pCgiRet);
	SAFE_FREE(info_type);
	
	return CGI_OK;		
}
