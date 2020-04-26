/*
 * Copyright Samsung Electronics Co.,LTD.
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/*!
 * \file MemoryHeapBaseIon.cpp
 * \brief source file for MemoryHeapBaseIon
 * \author MinGu, Jeon(mingu85.jeon)
 * \date 2011/11/20
 *
 * <b>Revision History: </b>
 * - 2011/11/20 : MinGu, Jeon(mingu85.jeon)) \n
 * Initial version
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "MemoryHeapBaseIon"
#include <utils/Log.h>

#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <cutils/log.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IMemory.h>
#include <binder/MemoryHeapBaseIon.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <ion/ion.h>

//#define HEAP_TYPE   RTK_PHOENIX_ION_HEAP_MEDIA_MASK
//#define HEAP_TYPE   ION_HEAP_SYSTEM_MASK
//#define HEAP_TYPE   ION_HEAP_SYSTEM_CONTIG_MASK

namespace android {

MemoryHeapBaseIon::MemoryHeapBaseIon(size_t size, uint32_t flags, char const *name)
{
    ALOGV("MemoryHeapBaseIon: size:%zu flags:%x name:%s", size, flags, name);
    mIonClient = ion_open();
    if (mIonClient < 0) {
        mIonClient = -1;
        ALOGE("ION client creation failed");
        return;
    }

    const size_t pagesize = getpagesize();
    size = ((size + pagesize-1) & ~(pagesize-1));

    ion_user_handle_t handle;
    int ret = ion_alloc(mIonClient, size, 0, RTK_PHOENIX_ION_HEAP_MEDIA_MASK, (ION_FLAG_NONCACHED | ION_FLAG_SCPUACC | ION_FLAG_HWIPACC), &handle);
    if(ret < 0){
        ALOGE("ION memory allocation failed");
        return;
    }

    int fd;
    ret = ion_share(mIonClient, handle, &fd);

#if 0 //used to get physical address of ion memory by shared fd
    unsigned long addr;
    unsigned int len;
    ion_phys(mIonClient, handle, &addr, &len);
    ALOGD("get ion physical addr:%lx len:%d", addr, len);
#endif

    ion_free(mIonClient, handle); //just free handle directly

    if (fd < 0) {
        ALOGE("ION share failed");
        return;
    }

    void* base = NULL;
    flags |= USE_ION_FD;
    base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (base == MAP_FAILED) {
        ALOGE("mmap failed");
        return;
    }

    init(fd, base, size, flags, NULL);
}

MemoryHeapBaseIon::MemoryHeapBaseIon(int fd, size_t size, uint32_t flags, uint32_t offset)
{
    ALOGV("MemoryHeapBaseIon: fd:%d size:%zu flags:%x offset:%u", fd, size, flags, offset);
    ALOGE_IF(fd < 0, "file discriptor error. fd is not for ION Memory");
    mIonClient = ion_open();
    if (mIonClient < 0) {
        mIonClient = -1;
        ALOGE("ION client creation failed");
    }

    void* base = NULL;
    if (fd >= 0) {
        int dup_fd = dup(fd);
        flags |= USE_ION_FD;

        const size_t pagesize = getpagesize();
        size = ((size + pagesize-1) & ~(pagesize-1));

        base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, dup_fd, 0);
        if (base != MAP_FAILED)
            init(dup_fd, base, size, flags, NULL);
        else
            ALOGE("mmap failed");
    }
}

MemoryHeapBaseIon::~MemoryHeapBaseIon()
{
    if (mIonClient != -1) {
        munmap(getBase(), getSize());
        close(getHeapID());
        ion_close(mIonClient);
        mIonClient = -1;
    }
}

};
