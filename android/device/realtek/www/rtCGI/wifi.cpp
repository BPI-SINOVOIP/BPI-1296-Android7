#include "fcgi_stdio.h"	/* fcgi library; put it fist */
#include <stdlib.h>
#include <string.h>

#include <Application/AppClass/SetupClassAndroid.h>

#include "wifi.h"
#include "response.h"

int handleWifi(void)
{
	int cmd;
	char *cmd_type = NULL;
	const char* result = NULL;
	
	CGI_RESULT_T* pCgiRet = (CGI_RESULT_T*) malloc (sizeof(CGI_RESULT_T));	
	memset(pCgiRet, 0, sizeof(CGI_RESULT_T));
	
	do
	{			
		if (cgi_get_svalue("command", &cmd_type) != CGI_OK)
		{
			responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_FAIL, "Invalid command");
            dumpCgiResult(pCgiRet);
			break;
		}

		cmd = cmd_string2id(cmd_type, tbl_WifiCommandSet, SIZEOF_ARRAY(tbl_WifiCommandSet));
		switch(cmd) 
		{
		case WIFI_CHECK_SETUP_NECESSARY:
            {
                // So far, the Wifi SoftAP is not implemented. 
                // Wifi One-way setup should be turned off
                result = "{\"status\":0, \"wifi_connect_needed\":false}";
                break;
            }
            break;
		case WIFI_QUERY_STATUS:
			result = SetupClass::GetInstance()->wifiCmd(WIFI_CMD_QUERY_STATUS);
			break;
		case WIFI_ENABLE:
			result = SetupClass::GetInstance()->wifiCmd(WIFI_CMD_ENABLE);
			break;
		case WIFI_DISABLE:
            {
                // Wifi is going to be turned off...must response first
                responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_OK, NULL);
                dumpCgiResult(pCgiRet);                
                
			    SetupClass::GetInstance()->wifiCmd(WIFI_CMD_DISABLE);
            }
			break;
		case WIFI_SEARCH_AP_LIST:
			result = SetupClass::GetInstance()->wifiCmd(WIFI_CMD_SEARCH_AP);
			break;
		case WIFI_CONNECT:
			{
				char *ssid=NULL, *passwd=NULL, *security=NULL;
				if (cgi_get_svalue(WIFI_CGI_SSID_TAG, &ssid) == CGI_FAIL ||
					cgi_get_svalue(WIFI_CGI_PASSWD_TAG, &passwd) == CGI_FAIL ||
					cgi_get_svalue(WIFI_CGI_SECURITY_TAG, &security) == CGI_FAIL)
				{
					responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_FAIL, "One of SSID, Password and Security is not specified");
                    dumpCgiResult(pCgiRet);
				}
                else {
                    // when wifi connect to another AP, the original connection must be turned off
                    // Thus, it has to response first.
                    responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_OK, NULL);
                    dumpCgiResult(pCgiRet);
                    
                    SetupClass::GetInstance()->wifiCmd(WIFI_CMD_CONNECT, ssid, passwd, security);
                }
                SAFE_FREE(ssid);
                SAFE_FREE(passwd);
                SAFE_FREE(security);                
			}
			break;
		case WIFI_DISCONNECT:
            {
                // Before the connection is off, it must response
                responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_OK, NULL);
                dumpCgiResult(pCgiRet);                
                
                SetupClass::GetInstance()->wifiCmd(WIFI_CMD_DISCONNECT);
            }
			break;
		case WIFI_REMOVE:
			{
				char *netId;
				if( cgi_get_svalue(WIFI_CGI_NETID_TAG, &netId) != CGI_OK) {
					responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_FAIL, "Net id is needed!");
                    dumpCgiResult(pCgiRet);
					break;
				}
                else { 
                    responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_OK, NULL);
                    dumpCgiResult(pCgiRet);
                    
                    SetupClass::GetInstance()->wifiCmd(WIFI_CMD_REMOVE, netId);                    
                }
			}
			break;
		case WIFI_QUERY_CONNECT_INFO:
			result = SetupClass::GetInstance()->wifiCmd(WIFI_CMD_QUERY_CONNECTION);
			break;
		default:
			{
				responseGeneralResult(pCgiRet, RESPONSE_TYPE_WIFI, CGI_FAIL, "Not support cmd string: %s", cmd_type);
                dumpCgiResult(pCgiRet);
			}
			break;			
		}
				
	} while(0);	 	

	if (result != NULL)
	{
		printf("Content-type: application/json\r\n"
			   "\r\n"
			   "%s\n",
			   result);
	}

	SAFE_FREE(pCgiRet);	
	SAFE_FREE(cmd_type);
	
	return CGI_OK;
}
