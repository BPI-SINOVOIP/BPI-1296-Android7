#define LOG_NDEBUG 0
#include <utils/Log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "util.h"

int GetTogoPid(int cid)
{
	FILE* pFile = NULL;
	char buf[16];
	int mypid = -1;
	char filename[32];

	memset(filename, 0, sizeof(char)*32);	
	//snprintf(filename, 32, "/tmp/togoReadPid_%d", cid);	// for togoOutCGI
	snprintf(filename, 32, "/tmp/togoReadPid");

	if ((pFile = fopen(filename, "rb")) == NULL) {
		return mypid;
	}
	memset(buf, 0, sizeof(char)*16);
	fread(buf, 1, 16, pFile);
	mypid = atoi(buf);
	return mypid;
}


void SaveTogoPid()
{
	FILE* pFile = NULL;
	char filename[32];
	
	pid_t mypid = getpid();
	ALOGV("SaveTogoPid.mypid=%d.",mypid);

	memset(filename, 0, sizeof(char)*32);
	snprintf(filename, 32, "/tmp/togoReadPid");

	unlink(filename);
	
	if ((pFile = fopen(filename, "wb")) == NULL) {
		ALOGE("open %s NG.",filename);
		return;
	}
	fprintf(pFile, "%d", mypid);
	fclose(pFile);	
}

void KillStreamingProc(int cid)
{
	int mypid = GetTogoPid(cid);
	if (mypid != (-1)) {
		int ret = kill(mypid, SIGINT);
		if (ret != 0) {
			ALOGE("kill(%d, SIGINT) fail.", mypid);
		}
		else {
			ALOGV("kill(%d, SIGINT) success.", mypid);
		}
	}
}

