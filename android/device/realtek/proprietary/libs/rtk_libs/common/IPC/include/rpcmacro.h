#ifndef __RPCMACRO_H__
#define __RPCMACRO_H__

#include <string.h>
#include <errno.h>

static inline int open_rpc_dev(const char *name, int flags)
{
    int fd = -1;
    char fn[32];

    sprintf(fn, "/dev/%s", name);
    fd = open(fn, flags);
    if(fd >= 0){
        ALOGV("open %s success:%d", fn, fd);
        return fd;
    }

    sprintf(fn, "/rtk/%s", name);
    fd = open(fn, flags);
    if(fd >= 0){
        ALOGV("open %s success:%d", fn, fd);
        return fd;
    }

    ALOGE("open %s failed: %s", fn, strerror(errno));
    abort();
    return fd;
}

#endif //__RPCMACRO_H__

