/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef RTK_FILE_SOURCE_H_

#define RTK_FILE_SOURCE_H_

#include <stdio.h>

#include <media/stagefright/DataSource.h>
#include <media/stagefright/MediaErrors.h>
#include <utils/threads.h>
#include <drm/DrmManagerClient.h>

namespace android {

class RTKFileSource : public DataSource {
public:
    RTKFileSource(const sp<DataSource> &source);
    RTKFileSource(const char *filename);
    RTKFileSource(int fd, int64_t offset, int64_t length);

    virtual status_t initCheck() const;

    virtual ssize_t readAt(off64_t offset, void *data, size_t size);

    virtual status_t getSize(off64_t *size);

    virtual sp<DecryptHandle> DrmInitialization(const char *mime);

    virtual void getDrmInfo(sp<DecryptHandle> &handle, DrmManagerClient **client);

    void read_block(void);

    virtual int getfd(void);

    int RTK_EXT;
    
    virtual ~RTKFileSource();

//    RTKFileSource &operator=(const RTKFileSource &);
//    RTKFileSource(const RTKFileSource &);

protected:
//    virtual ~RTKFileSource();

private:
    int mFd;
    int64_t mOffset;
    int64_t mLength;
    Mutex mLock;

    /*for DRM*/
    sp<DecryptHandle> mDecryptHandle;
    DrmManagerClient *mDrmManagerClient;
    int64_t mDrmBufOffset;
    int64_t mDrmBufSize;
    unsigned char *mDrmBuf;

    ssize_t readAtDRM(off64_t offset, void *data, size_t size);

    RTKFileSource(const RTKFileSource &);
    RTKFileSource &operator=(const RTKFileSource &);
};

}  // namespace android

#endif  // RTK_FILE_SOURCE_H_
