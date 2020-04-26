///////////////////////////////////////////////////////////////////////////////
//
// Author:
//
// Created:
////////////////////////////////////////////////////////////////////////////////
#include "AirFunIpc.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include <errno.h>
#include <cutils/properties.h>
#include <cutils/log.h>
#define printf ALOGI

#define DIRECTCONNECTAGENT_FILE_SOCKET_PATH "/tmp/DirectConnectAgent"
#define DIRECTCONNECTAGENT_NOTIFY_FILE_SOCKET_PATH "/tmp/DirectConnectAgentNotify"

#define CMD_SERVICE_OPEN_IMAGE_FILE			"AIRFUN.OPEN.IMAGE"
#define CMD_SERVICE_EXTRACT_METADATA		"AIRFUN.EXTRACT.METADATA"

#define SIZEOF_ARRAY(x)		(sizeof(x)/sizeof(x[0]))

#define BUFFER_SIZE (128)

enum PLAYBACKSTATUS {
	CURRENTPLAYMODE,
	CONTRAST,
	BRIGHTNESS,
	VOLUME,
	MUTE,
	TRANSPORTSTATE,	//5
	DURATION,
	TIMEPOSITION,
	SEEK,
	TYPE,
};

enum {
	AIRFUN_QUERY_PLAYBACK_INFO,
	AIRFUN_DOWNLOAD_IMAGE_OK,
	AIRFUN_RESET_PLAYBACK_INFO,
    AIRFUN_RESET_VIDEO_INFO,
    AIRFUN_RESET_AUDIO_INFO,
    AIRFUN_RESET_IMAGE_INFO,
    AIRFUN_DOWNLOAD_IMAGE_NG,
};


enum {
    MIME_ID_NONE = 0,
	MIME_ID_VIDEO,
    MIME_ID_AUDIO,
	MIME_ID_IMAGE,
};

AirFunIpc* AirFunIpc::m_singleton = NULL;

static struct {
	char *cmdString;
	int cmdId;
}  CMD_STR[] =
{
	{"AIRFUN.QUERY.PLAYBACK.INFO", AIRFUN_QUERY_PLAYBACK_INFO},
	{"AIRFUN.DOWNLOAD.IMAGE.OK", AIRFUN_DOWNLOAD_IMAGE_OK},
	{"AIRFUN.RESET.PLAYBACK.INFO", AIRFUN_RESET_PLAYBACK_INFO},
    {"AIRFUN.RESET.VIDEO.INFO", AIRFUN_RESET_VIDEO_INFO},
    {"AIRFUN.RESET.AUDIO.INFO", AIRFUN_RESET_AUDIO_INFO},
    {"AIRFUN.RESET.IMAGE.INFO", AIRFUN_RESET_IMAGE_INFO},
    {"AIRFUN.DOWNLOAD.IMAGE.NG", AIRFUN_DOWNLOAD_IMAGE_NG},
};

////////////////////////////////////////////////////////////////////////////////
// AirFunIpc Implementation
////////////////////////////////////////////////////////////////////////////////
AirFunIpc::AirFunIpc(void) :
m_socket(-1), m_newsocket(-1), m_socketClient(-1), m_NotifyFunc(NULL)
{
	memset( &m_playbackInfo, 0, sizeof(m_playbackInfo) );
	pthread_create( &m_ipcThread, NULL, AirFunIpc::IpcThread, this);
    pthread_mutex_init(&m_mutex, NULL);
}

AirFunIpc::~AirFunIpc(void)
{
	if( m_socket != -1)
	{
		if( m_newsocket != -1)
		{
			shutdown(m_newsocket, SHUT_RDWR);
			close(m_newsocket);
			m_newsocket = -1;
		}
		shutdown(m_socket, SHUT_RDWR);
		close(m_socket);
		m_socket = -1;
	}
    pthread_mutex_destroy(&m_mutex);
}

AirFunIpc* AirFunIpc::GetInstance(void)
{
    if(NULL == m_singleton)
    {
        m_singleton = new AirFunIpc();
    }
    return m_singleton;
}

void AirFunIpc::DelInstance(void)
{
    if( m_singleton )
	{
        delete m_singleton;
        m_singleton = NULL;
    }
}

bool AirFunIpc::CreateServer(void)
{
	bool bRet = false;
	do
	{
		m_socket = socket(AF_UNIX, SOCK_STREAM, 0);
		if (-1 == m_socket)
		{
			printf("[%s:%d]:%s()- Fail to create socket\n", __FILE__, __LINE__, __func__);
			break;
		}

		struct sockaddr_un remote;
		remote.sun_family = AF_UNIX;
		strcpy(remote.sun_path, DIRECTCONNECTAGENT_FILE_SOCKET_PATH);
		int len = strlen(remote.sun_path) + sizeof(remote.sun_family);
        char value[PROPERTY_VALUE_MAX];
        property_get("persist.rtk.abstract_socket", value, "false");
        if(strcmp(value, "true") == 0)
            remote.sun_path[0] = 0;

		unlink(remote.sun_path);
		if (bind(m_socket, (struct sockaddr *)&remote, len) == -1)
		{
			printf("[%s:%d]:%s()- Fail to bind socket\n", __FILE__, __LINE__, __func__);
			close(m_socket);
			break;
		}

		if (listen(m_socket, 5) == -1)
		{
			printf("[%s:%d]:%s()- Fail to listen socket\n", __FILE__, __LINE__, __func__);
			close(m_socket);
			break;
		}
		bRet = true;
	} while (false);

	return bRet;
}

bool AirFunIpc::CreateConnection(void)
{
	bool bRet = false;

	do
	{
		m_socketClient = socket(AF_UNIX, SOCK_STREAM, 0);
		if (-1 == m_socketClient)
		{
			printf("[%s:%d]:%s()- Fail to create socket\n", __FILE__, __LINE__, __func__);
			break;
		}

		struct sockaddr_un remote;
		remote.sun_family = AF_UNIX;
		strcpy(remote.sun_path, DIRECTCONNECTAGENT_NOTIFY_FILE_SOCKET_PATH);
		int len = strlen(remote.sun_path) + sizeof(remote.sun_family);
        char value[PROPERTY_VALUE_MAX];
        property_get("persist.rtk.abstract_socket", value, "false");
        if(strcmp(value, "true") == 0)
		    remote.sun_path[0] = 0;

		if (connect(m_socketClient, (struct sockaddr *)&remote, len) == -1)
		{
			printf("[%s:%d]:%s()- Fail to connect socket, error(%d)\n", __FILE__, __LINE__, __func__, errno);
			close(m_socketClient);
			m_socketClient = -1;
			break;
		}

		bRet = true;
	} while (false);

	return bRet;
}

void AirFunIpc::SetNotifyFunc(AirfunNotifyFunc fptr)
{
	m_NotifyFunc = fptr;
}

void AirFunIpc::UpdatePlaybackStatus(int typeId, int value)
{
	switch( typeId ) {
	case TRANSPORTSTATE:
		m_playbackInfo.state = value;
		break;
	case DURATION:
		m_playbackInfo.duration = value;
		break;
	case TIMEPOSITION:
		m_playbackInfo.timePosition = value;
		break;
	case VOLUME:
		m_playbackInfo.volume = value;
		break;
	case MUTE:
		m_playbackInfo.mute = value;
		break;
	case TYPE:
		m_playbackInfo.playType = value;
		break;
	default:
		break;
	}
	//printf("%s, typeId(%d) value(%d)", __func__, typeId, value);
}

void AirFunIpc::UpdatePlaybackStatusByMime(int mimeId, int typeId, int value)
{
    switch (mimeId)
    {
    case MIME_ID_VIDEO:
        if (TRANSPORTSTATE == typeId)
        {
            m_playbackInfo.videoState = value;
        }
        break;
    case MIME_ID_AUDIO:
        if (TRANSPORTSTATE == typeId)
        {
            m_playbackInfo.audioState = value;
        }
        break;
    case MIME_ID_IMAGE:
        if (TRANSPORTSTATE == typeId)
        {
            m_playbackInfo.imageState = value;
        }
        break;
    default:
        break;
    }
}

bool AirFunIpc::OpenAndDownloadImage(const char *url, char buf[], int bufLen)
{
    bool bRet = false;
    char tmp[128];

    do
    {
        if(m_socketClient == -1 && false == CreateConnection())
        {
            break;
        }
        snprintf(tmp, sizeof(tmp), "%s|%s",CMD_SERVICE_OPEN_IMAGE_FILE, url);
        printf("%s, str(%s)\n",__func__, tmp);

        // send url to DirectConnect Agent, and it will return the corresponding file name.
        memset(m_filename, 0, sizeof(m_filename));
        pthread_mutex_lock(&m_mutex);
        if (-1 != write(m_socketClient, tmp, strlen(tmp)) &&
            -1 != read(m_socketClient, m_filename, sizeof(m_filename)))
        {
            bRet = true;
        }
        else
        {
            printf("%s, %d, socket read/write error", __func__, __LINE__);
        }
        pthread_mutex_unlock(&m_mutex);
    } while (false);

    if (true == bRet)
    {
        printf("%s, filename(%s)\n", __func__, m_filename);
        strncpy( buf, m_filename, bufLen );
    }
    return bRet;
}

bool AirFunIpc::ExtractMetaData(const char *url, AirFunMetaData *metaData)
{
    bool bRet = false;
    char tmp[128];

    do
    {
        if(m_socketClient == -1 && false == CreateConnection())
        {
            break;
        }
        snprintf(tmp, sizeof(tmp), "%s|%s",CMD_SERVICE_EXTRACT_METADATA, url);
        printf("%s, str(%s)\n",__func__, tmp);

        // send url to DirectConnect Agent, and it will return the corresponding MetaData.
        pthread_mutex_lock(&m_mutex);
        if (-1 != write(m_socketClient, tmp, strlen(tmp)) &&
            -1 != read(m_socketClient, metaData, sizeof(AirFunMetaData)))
        {
            bRet = true;
        }
        else
        {
            printf("%s, %d, socket read/write error", __func__, __LINE__);
        }
        pthread_mutex_unlock(&m_mutex);
    } while (false);

    if (true == bRet)
    {
        printf("%s, title(%s) album(%s) artist(%s)\n", __func__, metaData->title, metaData->album, metaData->artist);
    }
    return bRet;
}

void *AirFunIpc::IpcThread(void *args)
{
	AirFunIpc *afIpc = (AirFunIpc*)args;
    char readBuf[BUFFER_SIZE] = {0};
    char cmdBuf[BUFFER_SIZE] = {0};
    char *pCmdStart = NULL, *pCmdEnd = NULL;

	if( ! afIpc->CreateServer() )
	{
		afIpc->m_socket = -1;
		goto finish;
	}
#if 0
    while (1)
    {
        if (false == afIpc->CreateConnection())
        {
            printf("[%s:%d]:%s()- Fail to connect socket\n", __FILE__, __LINE__, __func__);
        }
        else
        {
            break;
        }
        usleep(1000000);    // sleep 1 seconds
    }
#endif
	while( 1 )
	{
		struct sockaddr_un cli_addr;
		int cli_len = sizeof(cli_addr);

		// accept() blocks until connection from client
		afIpc->m_newsocket = accept( afIpc->m_socket, (struct sockaddr *)&cli_addr, (socklen_t *)&cli_len);
		if( afIpc->m_newsocket < 0 )
		{
			printf("[%s:%d]:%s()- Fail to accept socket\n", __FILE__, __LINE__, __func__);
			goto finish;
		}

		while( 1 )
		{
			memset( readBuf, 0, sizeof(readBuf) );

			int byteRead = read( afIpc->m_newsocket, readBuf, sizeof(readBuf) );

			// if no byte is read, it indicates the end of the file..
			if(0 == byteRead) {
				printf("[%s:%d]:%s(), read zero bytes\n", __FILE__, __LINE__, __func__);
			}
            else {
                printf("IpcThread, read(%s)\n", readBuf);

                // parse the received message.
                // there might be 2-consecutive message coming, each one is separated by a separator(;)
                pCmdStart = readBuf;
                while (pCmdStart)
                {
                    pCmdEnd = strchr(pCmdStart, ';');
                    if (pCmdEnd)
                    {
                        // There is a complete command
                        *pCmdEnd = 0;

                        // Process command
                        strncat(cmdBuf, pCmdStart, strlen(pCmdStart));
                        processCommand(afIpc, cmdBuf);
                        memset(cmdBuf, 0, sizeof(cmdBuf));

                        pCmdStart = pCmdEnd + 1;

                        if (pCmdStart - readBuf >= BUFFER_SIZE)
                        {
                            // pCmdStart is outside readBuf
                            break;
                        }
                    }
                    else
                    {
                        // There is an incomplete command
                        strncat(cmdBuf, pCmdStart, BUFFER_SIZE - (pCmdStart - readBuf));
                        break;
                    }
                }
            }
            usleep(50000);
		}
		close(afIpc->m_newsocket);
		close(afIpc->m_socketClient);
		afIpc->m_socketClient = -1;
        usleep(50000);
	}

finish:
	pthread_detach(pthread_self());

	printf("----- AirFunIpc::IpcThread exit -----");
	return NULL;
}

void AirFunIpc::processCommand(AirFunIpc *afIpc, char *pCmdStr)
{
    unsigned int i;
    bool bNotify = true;
    if (NULL == afIpc || NULL == pCmdStr || 0 == strlen(pCmdStr))
    {
        return;
    }

    printf("[%s:%d]:%s()- %s\n", __FILE__, __LINE__, __func__, pCmdStr);

    // if query-related message, write the result back to Client..
    for (i=0; i<SIZEOF_ARRAY(CMD_STR); i++)
    {
        if (strncmp(pCmdStr, CMD_STR[i].cmdString, strlen(CMD_STR[i].cmdString)) == 0)
        {
            bNotify = false;
            switch( CMD_STR[i].cmdId ) {
            case AIRFUN_QUERY_PLAYBACK_INFO:
                write( afIpc->m_newsocket, &(afIpc->m_playbackInfo), sizeof(afIpc->m_playbackInfo) );
                break;
            case AIRFUN_DOWNLOAD_IMAGE_OK:
                // compare if the filename is the same or not?
                if (afIpc->m_NotifyFunc)
                {
                    afIpc->m_NotifyFunc("S|OK");
                }
                break;
            case AIRFUN_DOWNLOAD_IMAGE_NG:
                if (afIpc->m_NotifyFunc)
                {
                    afIpc->m_NotifyFunc("Z|NG");
                }
                break;
            case AIRFUN_RESET_PLAYBACK_INFO:
                memset(&(afIpc->m_playbackInfo), 0, sizeof(afIpc->m_playbackInfo));
                break;
            case AIRFUN_RESET_VIDEO_INFO:
                afIpc->m_playbackInfo.videoState = 0;
                break;
            case AIRFUN_RESET_AUDIO_INFO:
                afIpc->m_playbackInfo.audioState = 0;
                break;
            case AIRFUN_RESET_IMAGE_INFO:
                afIpc->m_playbackInfo.imageState = 0;
                break;
            default:
                //write( afIpc->m_newsocket, "unsupported cmd", strlen("unsupported cmd"));
                printf("Unsupported Command!");
                break;
            }
            break;
        }
    }
    // otherwise, it's command-related message, pass the command to Java(Andorid).
    if (bNotify && afIpc->m_NotifyFunc)
    {
        afIpc->m_NotifyFunc(pCmdStr);
    }
}
