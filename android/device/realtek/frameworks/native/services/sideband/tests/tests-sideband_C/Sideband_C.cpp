#include <string.h>
#include <binder/ProcessState.h>
#include <gui/SidebandClient.h>
#include <gui/SidebandControl.h>
#include <gui/Surface.h>
#include <gui/BufferQueue.h>
#include <gui/IConsumerListener.h>
#include <gui/IProducerListener.h>
#include <hardware/gralloc.h>
#include <cutils/properties.h>
#include <android/native_window.h>

#include "Sideband_C.h"

#define CHECK assert
#define ASSERT_EQ(x, y) assert((x) == (y))

using namespace android;


sp<SidebandClient> gClient;
sp<SidebandControl> gControl;
sp<Surface> gSurface;
sp<ANativeWindow> gANW;

int SidebandUser_createSurface(
        char * name,
        int w, int h,
        int format, int usage) {

    gClient = new SidebandClient();

    gControl = gClient->createSurface(String8(name), w, h, PIXEL_FORMAT_RGB_888);
    CHECK(gControl != NULL);
    CHECK(gControl->isValid());

    gSurface = gControl->getSurface();
    CHECK(gSurface != NULL);

    gANW = gSurface;

    SidebandUser_setBufferFormat(format);
    SidebandUser_setBuffersUserDimensions(w,h);
    SidebandUser_setBufferUsage(usage);
    return 0;
}

int SidebandUser_getMinUndequeuedBuffers(int * pCount) {
    CHECK(pCount != NULL);
    status_t err = gANW.get()->query(gANW.get(),
        NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
        pCount);
    ASSERT_EQ(err, NO_ERROR);
    return 0;
}

int SidebandUser_setBuffersUserDimensions(int w, int h) {
    status_t err = native_window_set_buffers_user_dimensions(gANW.get(), w, h);
    ASSERT_EQ(err, NO_ERROR);
    return 0;
}

int SidebandUser_setBufferFormat(int format) {
    status_t err = native_window_set_buffers_format(gANW.get(), format);
    ASSERT_EQ(err, NO_ERROR);
    return 0;
}

int SidebandUser_setBufferUsage(int usage) {
    status_t err = native_window_set_usage(gANW.get(), usage);
    ASSERT_EQ(err, NO_ERROR);
    return 0;
}

int SidebandUser_setBufferCount(int count) {
    CHECK(count > 0);
    status_t err = native_window_set_buffer_count(gANW.get(), count);
    ASSERT_EQ(err, NO_ERROR);

    ANativeWindowBuffer* ANB[count];
    for (int i=0; i<count; i++) {
        err = native_window_dequeue_buffer_and_wait(gANW.get(), &(ANB[i]));
        ASSERT_EQ(err, NO_ERROR);
    }
    for (int i=0; i<count; i++) {
        err = gANW.get()->cancelBuffer(gANW.get(), ANB[i], -1);
        ASSERT_EQ(err, NO_ERROR);
    }
    return 0;
}

int SidebandUser_dequeue(/*ANativeWindowBuffer*/ void ** pANativeWindowBuffer, buffer_handle_t * buffer) {
    CHECK(pANativeWindowBuffer!= NULL);
    ANativeWindowBuffer* anb = NULL;
    status_t err = native_window_dequeue_buffer_and_wait(gANW.get(), &anb);
    ASSERT_EQ(err, NO_ERROR);
    CHECK(anb!=NULL);
    *(ANativeWindowBuffer**)pANativeWindowBuffer = anb;
    if (buffer!= NULL)
        *buffer = anb->handle;
    return 0;
}

int SidebandUser_queue(/*ANativeWindowBuffer*/ void ** pANativeWindowBuffer) {
    CHECK(pANativeWindowBuffer!= NULL);
    ANativeWindowBuffer* anb = *(ANativeWindowBuffer**)pANativeWindowBuffer;
    status_t err = gANW->queueBuffer(gANW.get(), anb, -1);
    ASSERT_EQ(err, NO_ERROR);
    return err;
}

#if 0
int SidebandUser_creatSidebandStream(native_handle_t ** handle) {
    return gControl->createSidebandStream(handle);
}
#endif

int SidebandUser_destroy() {
    gSurface.clear();
    gControl.clear();
    gClient.clear();
    return 0;
}

