//#include "fcgi_stdio.h"
//#include <stdlib.h>
//#include "fcgiapp.h"

#define LOG_NDEBUG	0
#define LOG_TAG	"togoRead.fcgi"
#include <utils/Log.h>

#include "togoRead.h"
//#include "libcgi.h"
#include "util.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IServiceManager.h>
#include <RtkIpc/IRecordBuffer.h>
#include "ToGoService.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>
#include <unistd.h>
#include <Platform_Lib/SocketAVData/SocketAVData.h>
#include <sys/mman.h>

pthread_t gStreamingThread;
static bool gbIsStreaming = false;
static bool gbIsQuitStreaming = false;

#define LISTENSOCK_FILENO 0
#define LISTENSOCK_FLAGS 0

#define DTV_LOCAL_SOCKET	"/tmp/DtvSocket"

static const PARAMETER_TABLE tbl_togo_read_commands[] = {
	INSERT_PARAM("start", CMD_START),
	INSERT_PARAM("stop", CMD_STOP),
};

using namespace android;

struct StreamingThreadArgs {
	sp<IMemory> data;
	sp<IMemory> info;
	char togofifo[256];
	int handle;
};

StreamingThreadArgs gThreadArgs;
sp<IMemory> gIMemData = NULL;
sp<IMemory> gIMemInfo = NULL;

static char gNullpacket[188];


void sighandler(int num)
{
	ALOGV("got signal:%d (SIGINT=%d.SIGTERM=%d.SIGPIPE=%d.SIGUSR1=%d.)\n", num,SIGINT,SIGTERM,SIGPIPE,SIGUSR1);
	if (num == SIGINT || num == SIGTERM || num == SIGPIPE || num == SIGUSR1) {
		ALOGV("sighandler, set gbIsQuitStreaming to true.");
		gbIsQuitStreaming = true;
	}
}

#ifdef ENABLE_1195_DTV_FLOW
int streamingDTV(int fifofd, ToGoService *pToGo, int handle)
{
	ALOGV("streamingDTV.enter.");
	char sock_path[32];
	int tunerId = pToGo->StartListen(handle);
	if (tunerId < 0) {
		ALOGE("no matched tunerId.handle=%d.",handle);
		return -1;
	}

	memset(sock_path, 0, sizeof(char)*32);
	sprintf(sock_path, "%s%d", DTV_LOCAL_SOCKET,tunerId);

	struct sockaddr_un address;
	int client_sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, (const char *)sock_path);

	int connect_count = 0;
	int sharefd = -1;
	void *pMemory = NULL;
	while (connect_count < 5) {
		if (connect(client_sockfd, (const struct sockaddr *)&address, sizeof(address)) == 0) {
			ALOGV("connect to sock_path=%s success.client_sockfd=%d.",sock_path,client_sockfd);
			struct cmsghdr *cmsg;
			struct iovec count_vec;
			struct msghdr child_msg;
			char* child_buf[100];
			char buf[CMSG_SPACE(sizeof(int))];

			count_vec.iov_base = child_buf;
			count_vec.iov_len = sizeof(child_buf);
			child_msg.msg_control = buf;
			child_msg.msg_controllen = sizeof(buf);
			child_msg.msg_iov = &count_vec;
			child_msg.msg_iovlen = 1;

			recvmsg(client_sockfd, &child_msg, 0);
			cmsg = CMSG_FIRSTHDR(&child_msg);
			if (cmsg == NULL) {
				ALOGE("cmsg == NULL.");
			} else {
				sharefd = *(int*)CMSG_DATA(cmsg);
				ALOGV("sharefd=%d.",sharefd);
			}

			if (sharefd != -1) {
				pMemory = mmap(NULL, (2*1024*1024+2048), PROT_READ | PROT_WRITE, MAP_SHARED, sharefd, 0);
				if (pMemory != NULL) {
					SOCKET_SHARED_MEM* pMemHeader = (SOCKET_SHARED_MEM*)pMemory;
					//ALOGV("beginPhyAddr=0x%lX size=%ll endPhyAddr=0x%lX readPtr=0x%lX readPtr2=0x%lX writePtr=0x%lX",
					//	pMemHeader->beginPhyAddr,pMemHeader->size,pMemHeader->endPhyAddr,
					//	pMemHeader->readPtr,pMemHeader->readPtr2,pMemHeader->writePtr);
				}
				else {
					ALOGE("[%s:%d].pMemory == NULL.",__FILE__,__LINE__);
				}
			}
			break;	// break while
		}
		else {
			ALOGE("connect() fail.errno=%d(%s).sock_path=%s.",errno,strerror(errno),sock_path);
			connect_count++;
			usleep(10000);	
			continue;
		}
	}

	if (client_sockfd != -1) {
		close(client_sockfd);
		client_sockfd = -1;
	}

	pToGo->StopListen(handle);

	if (connect_count == 5) {
		ALOGE("connect() 5 times,still fail.");
		return -1;
	}

	if (pMemory != NULL) {
		SOCKET_SHARED_MEM* pMemHeader = (SOCKET_SHARED_MEM*)pMemory;
		pMemHeader->readPtr2 = pMemHeader->writePtr;
		ALOGV("streaming start.readPtr=0x%lX.readPtr2=0x%lX.writePtr=0x%lX.",pMemHeader->readPtr,pMemHeader->readPtr2,pMemHeader->writePtr);

		//bool bGotDataFromRecordBuffer = false;
		while (!gbIsQuitStreaming) {
			//ALOGV("in while.readPtr=0x%X.readPtr2=0x%X.writePtr=0x%X.",pMemHeader->readPtr,pMemHeader->readPtr2,pMemHeader->writePtr);
			int cnt = 0;
			int size = 0;
			if (pMemHeader->writePtr >= pMemHeader->readPtr2) {
				size = pMemHeader->writePtr - pMemHeader->readPtr2;
			}
			else {
				size = pMemHeader->endPhyAddr - pMemHeader->readPtr2;
			}
		
			if (size <= 0){
				//if (bGotDataFromRecordBuffer == false) {
				//	int bytes = write(fifofd, gNullpacket, 188);
					//ALOGV("[%s:%d].write gNullpacket.bytes=%d.",__FILE__,__LINE__,bytes);
				//	usleep(200000);
				//}
				//else {
				//	usleep(100000);
				//}
					usleep(100000);
				continue;
			}

			//ALOGV("togoRead.readPtr=0x%X.readPtr2=0x%X.writePtr=0x%X.size=%d.beginPhyAddr=0x%X.endPhyAddr=0x%X.\n",pMemHeader->readPtr,pMemHeader->readPtr2,
			//	pMemHeader->writePtr,size,pMemHeader->beginPhyAddr,pMemHeader->endPhyAddr);

			//bGotDataFromRecordBuffer = true;
			char* pReadPtr = (char*)pMemHeader + pMemHeader->readPtr2;
			cnt = write(fifofd, pReadPtr, size);
			//ALOGV("write from readPtr2=0x%X.size=%d.cnt=%d.writePtr=0x%X.",pMemHeader->readPtr2,size,cnt,pMemHeader->writePtr);
			pMemHeader->readPtr2 += cnt;
			if (pMemHeader->readPtr2 >= pMemHeader->endPhyAddr) {
				pMemHeader->readPtr2 = pMemHeader->beginPhyAddr;
			}
			//ALOGV("new readPtr2=0x%X.",pMemHeader->readPtr2);
		}
		ALOGV("exit streamingDTV while");
		munmap(pMemory, (2*1024*1024+2048));
		close(sharefd);
	}

	ALOGV("streamingDTV.leave.");
	return 0;
}
#endif	//	#ifdef ENABLE_1195_DTV_FLOW

int streamingTranscode(int fifofd)
{
	ALOGV("streamingTranscode.enter.");
	uint32_t readPtr = 0;
	//bool bGotDataFromRecordBuffer = false;

	if (getRecordBuffer(gIMemData, gIMemInfo, 0) != 0) {
		ALOGE("getRecordBuffer() NG.");
		return -1;
	}
	ALOGV("gIMemData=%p,size=%d.gIMemInfo=%p,size=%d.",gIMemData.get(),gIMemData->size(),gIMemInfo.get(),gIMemInfo->size());

	struct BufferInfo_t *bufInfo = (struct BufferInfo_t *)gIMemInfo->pointer();
	if (bufInfo->magic != ASHMEM_RTK_MAGIC_NUMBER) {
		ALOGE("magic number is wrong.number=%d.ASHMEM_RTK_MAGIC_NUMBER=%d.",bufInfo->magic,ASHMEM_RTK_MAGIC_NUMBER);
		return -1;
	}

	readPtr = bufInfo->writePtr;
	 
	//ALOGV("gbIsQuitStreaming=%d.bufInfo->endOfStream=%d.",gbIsQuitStreaming,bufInfo->endOfStream);
	// for the case that togoRead then start transcode, not to check bufInfo->endOfStream.
	while (!gbIsQuitStreaming && 
		(checkRecordBufferService() == true)) {
	//while (!gbIsQuitStreaming && 
	//	(checkRecordBufferService() == true) && 
	//	(bufInfo->endOfStream == false)) {

		//ALOGV("[%s:%d].readPtr=0x%X.writePtr=0x%X.",__FILE__,__LINE__,bufInfo->readPtr,bufInfo->writePtr);

		uint32_t writePtr = bufInfo->writePtr;
		int size = 0;
		int cnt = 0;
		 
		if (writePtr >= readPtr) {
			size = writePtr - readPtr;
		}
		else {
			size = bufInfo->size - readPtr;
		}
		
		if (size <= 0){
			//if (bGotDataFromRecordBuffer == false) {
			//	int bytes = write(fifofd, gNullpacket, 188);
				//ALOGV("[%s:%d].write gNullpacket.bytes=%d.",__FILE__,__LINE__,bytes);
			//	usleep(200000);
			//}
			//else {
			//	usleep(100000);
			//}
				usleep(100000);
			continue;
		}

		//bGotDataFromRecordBuffer = true;
		if (bufInfo->packet_size > 0) {
			int *current_size = (int *)((long)gIMemData->pointer() + (long)readPtr);
		 
			if (current_size[0] > 0) {
				cnt = write(fifofd, (char *)gIMemData->pointer() + readPtr + 4, current_size[0]);
				readPtr = (readPtr + 1504) % bufInfo->size;
			}
			else {
				ALOGE("!!!! size [%d] writePtr [%d] readPtr[%d] bufInfo->size[%d] [%x %x %x %x ]!!!!", size, writePtr, readPtr, bufInfo->size, current_size[0], current_size[1], current_size[2], current_size[3]);
			}
		}
		else {
			//ALOGV("before write gIMemData.ptr=%p.size=%d.",((char *)gIMemData->pointer() + readPtr),size);
			cnt = write(fifofd, (char *)gIMemData->pointer() + readPtr, size);
			//ALOGV("after write gIMemData.ptr=%p.size=%d.cnt=%d.",((char *)gIMemData->pointer() + readPtr),size,cnt);
			readPtr = (readPtr + cnt) % bufInfo->size;
			bufInfo->readPtr = readPtr;
		}
		ALOGV("size:%d readPtr:%u writePtr:%u", cnt, readPtr, writePtr);
	}
	
	ALOGV("streamingTranscode.leave. gbIsQuitStreaming=%d", gbIsQuitStreaming);
	return 0;
}

/**
 * Thread for sending video streaming to fifo 
 * [VideoStream] => fifo => lighttpd sendfile => client
 */
void* StreamingThreadProc(void* args)
{
	ALOGV("StreamingThreadProc() enter ");
	uint32_t readPtr = 0;
	ToGoService *pToGo = NULL;
	int togoState;
	StreamingThreadArgs* pArgs = (StreamingThreadArgs*)args;

	ALOGV("before open.togofifo=%s.",pArgs->togofifo);
	int fifofd = open(pArgs->togofifo, O_WRONLY);
	if (fifofd == -1) {
		ALOGE("open togofifo=%s NG.",pArgs->togofifo);
		goto togoStreamingThreadEnd;
	}
	ALOGV("open togofifo=%s OK.",pArgs->togofifo);

	pToGo = ToGoService::GetInstance();
	if (pToGo == NULL) {
		ALOGE("pToGo == NULL.");
		goto togoStreamingThreadEnd;
	}

	memset(gNullpacket, 0, sizeof(char)*188);
	gNullpacket[0] = 0x47;
	gNullpacket[1] = 0x1F;
	gNullpacket[2] = 0xFF;
	gNullpacket[3] = 0x10;
	for (int i=4;i<188;i++) {
		gNullpacket[i] = 0x00;
	}
/*
	if(gRangeStart > 0) {
		// do seek
		// 2min 5sec=125000ms
		// transcode result: 12976512 bytes
		// vbps: 800000
		int msec = gRangeStart / 100;
		ALOGV("Seek to %d msec", msec);
		pToGo->seek(pArgs->handle, msec);
	}

	togoState = pToGo->getHandleState(pArgs->handle);
	if(togoState == TOGO_STATE_INITED) {
		// start togo 
		
	} // else if(togoState == TOGO_STATE_RUN) {}
*/
	/*
	if (pToGo->start(pArgs->handle) < 0) {
		ALOGE("start() NG.cid=%d.",pArgs->handle);
		goto togoStreamingThreadEnd;
	} else {
		sleep(2);
		ALOGE("start() OK, cid=%d", pArgs->handle);
	}
*/

#ifdef ENABLE_1195_DTV_FLOW
	if (pToGo->getHandleSourceType(pArgs->handle) == TOGO_TYPE_DTV) {
		if (streamingDTV(fifofd, pToGo, pArgs->handle) == -1) {
			goto togoStreamingThreadEnd;
		}
	}
	else {
#endif	//	#ifdef ENABLE_1195_DTV_FLOW
		if (streamingTranscode(fifofd) == -1) {
			goto togoStreamingThreadEnd;
		}
#ifdef ENABLE_1195_DTV_FLOW
	}
#endif	//	#ifdef ENABLE_1195_DTV_FLOW

togoStreamingThreadEnd:
	if (pToGo == NULL) {
		ToGoService::ReleaseInstance();
	} 
	if(fifofd != -1) {
		close(fifofd);
	}
	pthread_detach(pthread_self());
	gbIsQuitStreaming = false;
	gbIsStreaming = false;
	
	ALOGV("StreamingThreadProc().leave.");
	return NULL;
}

bool startStreaming(int handle, FCGX_Request *request)
{
	int rangeRet = 0;

/*
	int lockRet = pthread_mutex_trylock(&togoStream_mutex);
	if(lockRet == EBUSY) {
		ALOGV("Exist video streaming... call for exit... ");
		KillStreamingProc(0);
		pthread_mutex_lock(&togoStream_mutex);
	}
	pthread_mutex_unlock(&togoStream_mutex);
	ALOGV("startStreaming() start handle=%d.",handle);
*/

	gbIsQuitStreaming = false;
	gbIsStreaming = true;

	char togofifo[256];
	memset(togofifo, 0, sizeof(char)*256);
	sprintf(togofifo, "/tmp/togofifo-cid%d", handle);
	ALOGV("togofifo=%s.",togofifo);
	mkfifo(togofifo, 0777);

	//FCGX_FPrintF(request->out, "Accept-Ranges: bytes\r\n");
	FCGX_FPrintF(request->out, "Content-type: video/mp2t\r\n");
	FCGX_FPrintF(request->out, "X-Sendfile: %s\r\n\r\n", togofifo);

	gThreadArgs.data = gIMemData;
	gThreadArgs.info = gIMemInfo;
	memset(gThreadArgs.togofifo, 0, sizeof(char)*256);
	memcpy(gThreadArgs.togofifo, togofifo, sizeof(togofifo));
	gThreadArgs.handle = handle;
	pthread_create(&gStreamingThread, NULL, StreamingThreadProc, &gThreadArgs);

	ALOGV("startStreaming().leave.handle=%d.",handle);
	return true;
}

int get_query_param_string(FCGX_Request *request, const char* name, char **ret)
{
	char* env_query_string = NULL;
	char* querystr = NULL;
	char* p = NULL;

	env_query_string = FCGX_GetParam("QUERY_STRING", request->envp);
	if (env_query_string == NULL) {
		ALOGE("env_query_string == NULL.");
		return CGI_FAIL;
	}
	ALOGV("env_query_string=%s.",env_query_string);
	querystr = strdup(env_query_string);

	p = querystr;
	while(1) {
		char *pname;

		pname = p;
		if (!strncmp(p, name, strlen(name)) &&
			(p[strlen(name)] == '='))
		{
			while (*p != '&' && *p != '\0')
				p++;
			if (*p == '&')
				*p = '\0';

			*ret = cgi_decode_url(pname + strlen(name) + 1);
			free(querystr);
			return CGI_OK;
		}

		while (*p != '&' && *p != '\0')
			p++;
		if (*p == '\0')
		{
			free(querystr);
			return CGI_FAIL;
		}
		p++;
	}
	
	return CGI_OK;
}

static int cgi_check_number(char *p)
{
	if ((*p != '-') && (*p > '9' || *p < '0'))
		return CGI_FAIL;
	p++;
	while (*p != '\0')
	{
		if (*p > '9' || *p < '0')
			return CGI_FAIL;
		p++;
	}
	return CGI_OK;
}

int get_query_param_int(FCGX_Request *request, const char* name, int *retValue)
{
	char* pvalue = NULL;

	if (get_query_param_string(request, name, &pvalue) != CGI_OK) {
		return CGI_FAIL;
	}

	if (cgi_check_number(pvalue) != CGI_OK) {
		return CGI_FAIL;
	}
	*retValue = atoi(pvalue);
	if (pvalue) {
		free(pvalue);
	}

	return CGI_OK;
}

int handleCmd(FCGX_Request *request)
{
	ALOGV("handleCmd().enter.");
	char* cmd_str = NULL;
	int cmd = -1;
	int cid = -1;
	int t = 0;
	int ret = CGI_OK;
	

	do {
		if (get_query_param_string(request, "cmd", &cmd_str) != CGI_OK) {
			ALOGE("get_query_param_string(cmd) NG.");
				ret = CGI_FAIL;
				break;
		}
		ALOGV("cmd_str=%s.",cmd_str);

		cmd = cmd_string2id(cmd_str, tbl_togo_read_commands, SIZEOF_ARRAY(tbl_togo_read_commands));
		if (cmd == -1) {
			ALOGE("illegal cmd_str=%s.",cmd_str);
				ret = CGI_FAIL;
				break;
		}
		ALOGV("cmd=%d.",cmd);

		//if (cgi_get_ivalue("cid", &cid) != CGI_OK) {
		if (get_query_param_int(request, "cid", &cid) != CGI_OK) {
			ALOGE("get_query_param_int(cid) NG.");
				ret = CGI_FAIL;
				break;
		}
		ALOGV("cid=%d.",cid);
		//if (cid != 0) {
		//	ALOGE("only support one client now. cid should be 0.");
		//	return CGI_FAIL;
		//}
	
		ToGoService *pToGo = ToGoService::GetInstance();										
			if (pToGo) {
				int checkRet = pToGo->isHandleLegal(cid);
				ToGoService::ReleaseInstance();
	
				if(checkRet < 0) {
					ALOGE("illegal cid=%d.",cid);
					ret = CGI_FAIL;
					break;
				}
		}

	switch (cmd) {
		case CMD_START:
			if (gbIsStreaming) {
				ALOGE("Already streaming... cid=%d.",cid);
					ret = CGI_FAIL;
					break;
			}
			if (startStreaming(cid, request) != true) {
				ALOGE("startStreaming() NG.cid=%d.",cid);
					ret = CGI_FAIL;
					break;
			}
			break;
		case CMD_STOP:
			if (!gbIsStreaming) {
				ALOGE("no streaming,no need stop.cid=%d.",cid);
				break;
			}
			ALOGV("ask quit streaming thread.");
			gbIsQuitStreaming = true;
			break;
		default:
			ret = CGI_FAIL;
		}
	} while(0);


	ALOGV("handleCmd().leave.");
	return ret;
}

int main(int argc, char** argv)
{
	ALOGV("start togoRead.fcgi.");
	int count = 0;
	int ret = 0;
	char *server_name = NULL;

	if (FCGX_Init() != 0) {
		ALOGE("FCGX_Init() NG.");
		return 1;
	}
	ALOGV("FCGX_Init() OK.");

	FCGX_Request request;
	if (FCGX_InitRequest(&request, LISTENSOCK_FILENO, LISTENSOCK_FLAGS) != 0) {
		ALOGE("FCGX_InitRequest() NG.");
		return 2;
	}
	ALOGV("FCGX_InitRequest() OK.");

	signal(SIGPIPE, sighandler);	// client aborts a connection
	signal(SIGINT, sighandler);		// togo.cpp, stop command will send SIGINT to togoRead

	SaveTogoPid();

	while (1) {
		ret = FCGX_Accept_r(&request);
		if (ret < 0) {
			ALOGE("FCGX_Accept_r() NG.");
			break;
		}
		handleCmd(&request);

		FCGX_Finish_r(&request);
		ALOGE("after FCGX_Finish_r().");
	}

	ALOGV("exit togoRead.fcgi.");
	return 0;
}
