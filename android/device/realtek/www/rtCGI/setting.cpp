#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#include <stdlib.h>

#include <Application/AppClass/SetupClassAndroid.h>

#include "setting.h"
#include "response.h"

int setSettingsValue(int settingId, char *value, GETSET_SETTING_ITEM_T *pResult)
{
	if (pResult == NULL)
		return CGI_FAIL;

	if (settingId == -1 || value == NULL)
	{
		pResult->mRet = CGI_FAIL;
		snprintf(pResult->mErrStr, MAX_SIZE_ERROR_STR, "Invalid parameters");
		return CGI_FAIL;
	}

	pResult->mRet = CGI_OK;
	switch (settingId)
	{
		case SETTING_OSD_LANGUAGE:
		{
			ENUM_OSD_LANGUAGE langId = (ENUM_OSD_LANGUAGE) cmd_string2id(value, 
														ISO_639_language_3bytes_code_table, 
														SIZEOF_ARRAY(ISO_639_language_3bytes_code_table));
			if (langId == -1) {
				pResult->mRet = CGI_FAIL;
				snprintf(pResult->mErrStr, MAX_SIZE_ERROR_STR, "Not recognized language code: %s", value);
				break;
			}
			SetupClass::GetInstance()->SetOsdLanguage(langId);
			pResult->mValue = strdup(value);
		}
			break;
		default:
			break;
	}		

	return pResult->mRet;
		
}

int getSettingsValue(int settingId, GETSET_SETTING_ITEM_T *pResult)
{
	if (pResult == NULL)
		return CGI_FAIL;

	if (settingId == -1)
	{
		pResult->mRet = CGI_FAIL;
		snprintf(pResult->mErrStr, MAX_SIZE_ERROR_STR, "Invalid parameters");
		return CGI_FAIL;
	}
	
	pResult->mRet = CGI_OK;
	switch (settingId)
	{
		case SETTING_OSD_LANGUAGE:
		{
			ENUM_OSD_LANGUAGE langId = SetupClass::GetInstance()->GetOsdLanguage();
			pResult->mValue = cmd_id2string(langId, ISO_639_language_3bytes_code_table, SIZEOF_ARRAY(ISO_639_language_3bytes_code_table));
			if (pResult->mValue == NULL) {
				pResult->mRet = CGI_FAIL;
				snprintf(pResult->mErrStr, MAX_SIZE_ERROR_STR, "Not recognized langId: %d", langId);
			}
		}
			break;
		default:
			break;
	}

	return pResult->mRet;
}

int handleGetSetSettingItem(void)
{
	char *setting_type = NULL, *value = NULL;
	int settingId;

	CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));
	memset(pCgiRet, 0, sizeof(CGI_RESULT_T));
	
	pCgiRet->type = CGI_RESULT_GETSETSETTINGITEM;

	do 
	{
		if (cgi_get_svalue("setting_item", &setting_type) != CGI_OK)
		{
			pCgiRet->GetSetSettingItemResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->GetSetSettingItemResult.mErrStr, MAX_SIZE_ERROR_STR, "Invalid parameters");
			break;	
		}

		settingId = cmd_string2id(setting_type, tbl_SettingSet, SIZEOF_ARRAY(tbl_SettingSet));
		if (settingId == -1)
		{
			pCgiRet->GetSetSettingItemResult.mRet = CGI_FAIL;
			snprintf(pCgiRet->GetSetSettingItemResult.mErrStr, MAX_SIZE_ERROR_STR, "Not support setting item: %d", settingId);
			break;
		}		

		pCgiRet->QueryInfoResult.mRet = CGI_OK;
		// setting value
		if (cgi_get_svalue("value", &value) != CGI_OK)
		{
			// No value, it just queries setting value
			getSettingsValue(settingId, &(pCgiRet->GetSetSettingItemResult));			
		}
		else 
		{
			// with value, it wants to set vlaue
			setSettingsValue(settingId, value, &(pCgiRet->GetSetSettingItemResult));
			
		}
		
	} while(0);

	dumpCgiResult(pCgiRet);
	
	SAFE_FREE_GETSET_SETTING_ITEM_T(pCgiRet->GetSetSettingItemResult);
	SAFE_FREE(pCgiRet);

	SAFE_FREE(setting_type);
	SAFE_FREE(value);
	
	return CGI_OK;		
}
