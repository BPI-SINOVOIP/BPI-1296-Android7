#include <DLNA_DMR.h>
#include "ILibThreadPool.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

//TODO the value should be double checked
#define DMR_HTTP_ERROR_NONE    0
#define DMR_HTTP_ERROR_GENERIC -10
#define DMR_HTTP_ERROR_EOF     -20

#define DMR_HTTP_MAX_RETRY 5
static int bPlayContainer = false;
#ifdef DLNADMRCTT
#include "DLNA_DMP.h"
extern char g_playcontaiercid[128];
#endif

playlistRenderer::playlistRenderer()
{
	m_pSess = NULL;
	m_pReq = NULL;
	m_Sockerror = 0;
}

playlistRenderer::~playlistRenderer()
{
	if(!bPlayContainer)
		http_close();
}

int playlistRenderer::preParse(char *filename, unsigned int *NumberOfTracks, char ***MediaTrackURI, SUBRENDERER_TYPE **MediaType, long *TotalTime,char ***ProtocalInfo)
{
	int bSuccess = E_FAIL;
	bPlayContainer = false;
	#ifdef DLNADMRCTT
	if(filename && !strncmp(filename,"dlna-playcontainer://",strlen("dlna-playcontainer://")))
	{

		bPlayContainer = true;
		ULOGD("[DMR playlistRenderer] dlna-playcontainer preParse");
		char uuid[64];
		char cid[128];
		char fii[128];
		char *cur = NULL;
		char *next = NULL;
		int startindex;
		unsigned long ret;
        if( (cur = strstr( filename, "uuid:"))!= NULL )
        {
        	memset(uuid,0,64);
            cur += 5;
            strncpy(uuid,cur,36);
        }
		else if( (cur = strstr( filename, "uuid%3A"))!= NULL )
        {
        	memset(uuid,0,64);
            cur += 7;
            strncpy(uuid,cur,36);
        }
		else if( (cur = strstr( filename, "uuid%3a"))!= NULL )
        {
        	memset(uuid,0,64);
            cur += 7;
            strncpy(uuid,cur,36);
        }

		cur = NULL;
		next = NULL;
		if( (cur = strstr( filename, "cid="))!= NULL )
        {
        	memset(cid,0,128);
            cur += 4;
			if( (next = strchr( cur, '&'))!= NULL )
        	{
            	strncpy(cid,cur,next-cur);
				strcpy(g_playcontaiercid,cid);
        	}
			else if( (next = strstr( cur, "%26"))!= NULL )
        	{
            	strncpy(cid,cur,next-cur);
				strcpy(g_playcontaiercid,cid);
        	}
		}

		cur = NULL;
		next = NULL;
		if( (cur = strstr( filename, "fii="))!= NULL )
        {
        	memset(fii,0,128);
            cur += 4;
			if( (next = strchr( cur, '&'))!= NULL )
        	{
            	strncpy(fii,cur,next-cur);
        	}
			else
				strcpy(fii,cur);

			startindex = atoi(fii);
		}

		ULOGD("[DMR playlistRenderer] uuid=%s cid=%s startindex=%d",uuid,cid,startindex);

		RTK_DLNA_DMP *m_pUpnpDmp = RTK_DLNA_DMP_Singleton::GetInstance();
		if(m_pUpnpDmp)m_pUpnpDmp->unsetMediaServer();
		if(m_pUpnpDmp)m_pUpnpDmp->Start();
		sleep(5);

		if(m_pUpnpDmp && !m_pUpnpDmp->setMediaServerByUDN(uuid))
		{
			ULOGE("[DMR playlistRenderer] DMP setMediaServerByUDN Error");
			if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
			return bSuccess;
		}

		if(!m_pUpnpDmp->UPnPServiceBrowse(cid))
		{
			ULOGE("[DMR playlistRenderer] DMP UPnPServiceBrowse Error");
			if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
			return bSuccess;
		}


		//get all item
		int totalcount=m_pUpnpDmp->MediaItemObjectSizes();
		ULOGD("[DMR playlistRenderer] DMP item totalcount=%d",totalcount);
		if(startindex < totalcount){
			*NumberOfTracks =	totalcount - startindex;
			*TotalTime = 0;
			*MediaTrackURI = new char*[*NumberOfTracks];
			*ProtocalInfo  = new char*[*NumberOfTracks];
			*MediaType = new SUBRENDERER_TYPE[*NumberOfTracks];

			int nIndex = 0;
			for(int i=startindex;i<totalcount;i++){
				(*MediaType)[nIndex] = DMR_SUBR_UNKNOWN;
                		(*MediaTrackURI)[nIndex] = (*ProtocalInfo)[nIndex] = NULL;
				if(m_pUpnpDmp->queryResourceByIndex(i, UPNP_DMP_RES_PROTOCOLINFO, &ret))
				{       // CTT 1.5.58 7.3.45.1
					if (strstr((char*)ret, "audio/x-ms-wma:DLNA.ORG_PN=WMA") == NULL &&
                        		    strstr((char*)ret, "audio/mpeg:DLNA.ORG_PN=MP3") == NULL )
                    			{
                        			continue;
                    			}

					(*ProtocalInfo)[nIndex] = strdup((char*)ret);
	                		if(m_pUpnpDmp->queryResourceByIndex(i, UPNP_DMP_RES_URI, &ret))
					{
						(*MediaTrackURI)[nIndex] = strdup((char*)ret);
					}

                    			++nIndex;
				}
			}

            		*NumberOfTracks = nIndex;
			if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
			return S_OK;
		}
		else
			if(m_pUpnpDmp)m_pUpnpDmp->Terminate();
			return bSuccess;
	}
	#endif

	//initialze the values;
	*NumberOfTracks = 0;
	*TotalTime = 0;

	if(filename!=NULL)
		bSuccess = loadMedia(filename);

	if( bSuccess == S_OK )
	{
		//TODO, if content length missing ?
		//if content length missing, then return E_FAIL !!!
		char *tmpBuffer = (char *)calloc(m_contentlength+1, sizeof(char));
		int index = 0;
		char *cur = tmpBuffer;
		int lineLength = 0;
		int ret = http_read(tmpBuffer, m_contentlength);
		tmpBuffer[m_contentlength] = '\0';

		if( ret != m_contentlength )
		{
			http_close();
			return E_FAIL;// TODO, return E_FAIL, and think about why ?
		}

		*NumberOfTracks = getListLength(tmpBuffer);
		*TotalTime = 0;
		if( *NumberOfTracks == 0 )
		{
			http_close();
			return E_FAIL;
		}
		else
		{
			*MediaTrackURI = new char*[*NumberOfTracks];
			*MediaType = new SUBRENDERER_TYPE[*NumberOfTracks];
		}

		while( *cur != '\0')
		{
			lineLength = getLineLength(cur);
			if( *cur != '#' && *cur != '\n')
			{
				//TODO, notice, end of string?
				(*MediaTrackURI)[index] = (char *)strndup(cur, lineLength);
				(*MediaTrackURI)[index][lineLength] = '\0';
				(*MediaTrackURI)[index][lineLength-1] = '\0';
				(*MediaTrackURI)[index][lineLength-2] = '\0';
				(*MediaType)[index] = DMR_SUBR_UNKNOWN;
				index++;
			}
			cur += lineLength;
		}
		http_close();
		return S_OK;
	}
	else{
		return E_FAIL;
	}


}

int playlistRenderer::loadMedia(char *filename)
{
	return http_open(filename);
	//return S_OK;
}

int playlistRenderer::getListLength(char *pFileBuffer)
{
	int count = 0;
	if( pFileBuffer != NULL )
	{
		char *cur = pFileBuffer;
		while( *cur != '\0' )
		{
			if( *cur != '#' && *cur != '\n')
				count++;
			cur += (getLineLength(cur));
		}
	}
	ULOGD("[DMR playlistRenderer] there are %d media url in the playlist", count);
	return count;
}

int playlistRenderer::getLineLength(char *pFileBuffer)
{
	int count = 0;
	if( pFileBuffer != NULL )
	{
		while( pFileBuffer[count] != '\0' && pFileBuffer[count] != '\n')
			count++;
	}
	if( pFileBuffer[count] == '\0' ) count--;
	return (count+1);
}

int playlistRenderer::http_open(char *filename)
{
	int retry = 0, ret = 0;
	const char *value = NULL;
	const ne_status *status;

	if( ne_sock_init())
		return E_FAIL;

	if( ne_uri_parse(filename, &m_uri) || m_uri.host == NULL || m_uri.path == NULL ||
	    (strcasecmp(m_uri.scheme, "https") == 0))
	{
		//TODO, check this.....ne_uri_free()
		ne_uri_free(&m_uri);
		ne_sock_exit();
		ULOGE("[DMR playlistRenderer] [%s] parse url error", __func__);
		return E_FAIL;
	}

	if( m_uri.scheme == NULL )
		m_uri.scheme = ne_strndup("http", 4);
	if( m_uri.port == 0)
		m_uri.port = ne_uri_defaultport(m_uri.scheme);

	m_curpos = 0;
	m_pSess = NULL;
	m_pReq = NULL;
	m_contentlength = 0;

    while( retry < DMR_HTTP_MAX_RETRY )
	{
		m_pSess = ne_session_create(m_uri.scheme, m_uri.host, m_uri.port);
		ne_set_session_flag(m_pSess, NE_SESSFLAG_NOBUFFER, 1);
		ne_redirect_register(m_pSess);
		ne_set_useragent(m_pSess, "RealtekVOD");
		ne_set_connect_timeout(m_pSess, 10);
		ne_set_read_timeout(m_pSess, 10);
		if( m_uri.query == NULL )
		{
			m_pReq = ne_request_create(m_pSess, "GET", m_uri.path);
		}
		else
		{
			ne_buffer *reqbuf;
			reqbuf = ne_buffer_create();
			ne_buffer_concat(reqbuf, m_uri.path, "?", NULL);
			ne_buffer_zappend(reqbuf, m_uri.query);
			m_pReq = ne_request_create(m_pSess, "GET", reqbuf->data);
			//seekable = false;
		}
		ne_add_request_header(m_pReq, "transferMode.dlna.org", "Streaming");
		ne_add_request_header(m_pReq, "contentFeature.dlna.org", "1");
		ne_add_request_header(m_pReq, "Accept", "*/*");
		ret = ne_begin_request(m_pReq);
		if( ret != NE_OK )
		{
			ULOGE("[DMR playlistRenderer] begin request fail fail %d!!!",ret);
			ULOGE("[DMR playlistRenderer] the error message:%s !!!", ne_get_error(m_pSess));
			goto error_finish;
		}

		status = ne_get_status(m_pReq);
		value = ne_get_response_header(m_pReq, "Content-Length");
		if( status->klass == 3)
		{
			/* redirect! */
			ret = ne_end_request(m_pReq);
			if( ret == NE_REDIRECT)
			{
				const ne_uri *re_uri;
				ULOGE("[DMR playlistRenderer] HTTP redirect encountered!");
				re_uri = ne_redirect_location(m_pSess);
				ne_uri_free(&m_uri);
				ne_uri_copy(&m_uri, re_uri);

				if( m_uri.scheme == NULL )
					m_uri.scheme = ne_strndup("http", 4);
				if( m_uri.port == 0)
					m_uri.port = ne_uri_defaultport(m_uri.scheme);
				ne_request_destroy(m_pReq);
				ne_session_destroy(m_pSess);
				retry++;
				continue;
			}
			ULOGE("[DMR playlistRenderer] http error return %d", status->code);
			goto error_finish;
		}

		if( status->klass == 4)
		{
			//http error return
			ULOGE("[DMR playlistRenderer] http return error %d", status->code);
			goto error_finish;
		}

		if( value != NULL )
			m_contentlength = strtoll(value, NULL, 0);

		break;
	}

	if( m_contentlength == 0)
	{
		m_contentlength = -1;
		//seekable = false;
	}

	m_Sockerror = DMR_HTTP_ERROR_NONE;

	ULOGD("[DMR playlistRenderer] playlist http open success, content len %lld", m_contentlength);
	return S_OK;

error_finish:
	ULOGE("[DMR playlistRenderer] http open error finish");
	if( retry < DMR_HTTP_MAX_RETRY)
	{
		ne_request_destroy(m_pReq);
		m_pReq = NULL;
		ne_session_destroy(m_pSess);
		m_pSess = NULL;
	}
	ne_uri_free(&m_uri);
	return E_FAIL;
}

//TODO: think about if network removed ?!
int playlistRenderer::http_read(char *pBuffer, int size)
{
	off_t read_bytes = 0;
	off_t nlen = 0;
	ssize_t ret;

	if( m_Sockerror == DMR_HTTP_ERROR_EOF )
		return DMR_HTTP_ERROR_EOF;

	if( m_pSess == NULL || m_pReq == NULL || m_Sockerror )
		return -1;

	if( pBuffer == NULL )
		return -1;

	nlen = size;
	while( nlen )
	{
		ret = ne_read_response_block(m_pReq, pBuffer+read_bytes, nlen);
		if( ret < 0)
		{
			ULOGE("[DMR playlistRenderer] http stream, end of file encountered!!!");
			m_Sockerror = DMR_HTTP_ERROR_GENERIC;
			return read_bytes;
		}
		read_bytes += ret;
		nlen -= ret;
		/* end of file */
		if( ret == 0 )
		{
			ULOGE("[DMR playlistRenderer] http stream, end of file encountered!!!");
			m_Sockerror = DMR_HTTP_ERROR_EOF;
			return read_bytes;
		}
	}

	if( read_bytes == 0)
	{
		m_Sockerror = DMR_HTTP_ERROR_EOF;
		return DMR_HTTP_ERROR_EOF;
	}
	return read_bytes;
}

void playlistRenderer::http_close()
{
	if( m_pReq )
		ne_request_destroy(m_pReq);
	m_pReq = NULL;
	if( m_pSess )
		ne_session_destroy(m_pSess);
	m_pSess = NULL;
	ne_uri_free(&m_uri);
}

