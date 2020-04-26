#ifndef __UDF_ACCESS_INCLUDE_H__
#define __UDF_ACCESS_INCLUDE_H__

#include <io/udfaccess/udf_if.h>

#ifdef __cplusplus
extern "C" {
#endif

int dvdLoaderModel(const char *dpath, UB *model);
int dvdDiscAppInfo(const char *dpath, dappif *dapp);
int dvdDiscBlank(const char *dpath, int full);
int dvdDiscFormat(const char *dpath, int force, int mode);
int dvdTrayCntl(const char *dpath, int open);
int dvdLockTray(const char *dpath, int lock);
int dvdGetXferMode(const char *dpath, UB *mode);
int dvdSetXferMode(const char *dpath, UB mode);
int dvdSetStandby(const char *dpath);
int dvdStandby(const char *dpath);


HANDLE dvdGetDevHandle(const char *dpath);
HANDLE dvdDiscOpen(const char *dpath, int overWrite, dinfo **info);

int dvdGetSectorCGMS(HANDLE h, UI LSN, char *CGMS);

int dvdSeekNWA(HANDLE h);
int dvdCreateDummyECC(HANDLE h, UI desireLoc);

int dvdDECSSOn(HANDLE h, UI key0, UI key1);
int dvdDECSSOff(HANDLE h);

int dvdSetRegionCode(HANDLE h, UB region);
int dvdSetL0Capacity(HANDLE h, UI capacity);
int dvdDiscSet(HANDLE h, dinfo *info);
dinfo *dvdDiscStat(HANDLE h, int order);
void dvdDiscPeek(HANDLE h);

int dvdChDir(HANDLE h, char *path);
int dvdFileDelete(HANDLE h, char *path, char force);
int dvdFileShare(HANDLE dev, char *tar_path, char *src_path);

fobject *dvdFileOpen(HANDLE dev, char *path,const char *mode, UI loc, int perm, 
				  int CGMS, int pSysType);

int dvdFileSeek(fobject *foptr, UI offset, int origin);

UI dvdFileRead(char *ptr, UI size, fobject *foptr);
UI dvdFileWrite(char *ptr, UI size, fobject *foptr);

int dvdFileFlush(fobject *stream);
int dvdFileClose(fobject *stream);


int dvdFileSystemFlush(HANDLE h, const char *volName);
int dvdRecordableFinalize(HANDLE h, const char *volName);
int dvdDiscClose(HANDLE h, const char *volName);

int dvdMergePhilipsVOB(HANDLE h);

int dvdSetWriteSpeed(HANDLE h, double *sFactor);
int dvdJumpLayer(HANDLE h);
int dvdPollProgress(HANDLE h, int *progress);
int dvdReArrangeVOB(HANDLE h, UI size_limit);

#ifdef __cplusplus
}
#endif
#endif   // __UDF_ACCESS_INCLUDE_H__
