#include <stdio.h>
#include <string.h>
#include <hardware/gralloc.h>
#include "Sideband_C.h"

#define COUNT 10
int main(int argc , char *argv[]) {
    int i;
    argc = argc;
    argv = argv;
    SidebandUser_createSurface(
            /* name */      "TEST",
            /* w */         100,
            /* h */         100,
            /* format */    HAL_PIXEL_FORMAT_RTK_16_YCbCr_420_SP,
            /* usage */     GRALLOC_USAGE_HW_ION);
    SidebandUser_setBufferCount(COUNT);
    void  * ABuffer[COUNT];
    buffer_handle_t buffer = 0;
    for (i=0;i<COUNT;i++) {
        SidebandUser_dequeue(&ABuffer[i], &buffer);
        printf("[%2d] dequeue buffer : %p\n", i, buffer);
    }

    for (i=0;i<COUNT;i++) {
        SidebandUser_queue(&ABuffer[i]);
    }

    SidebandUser_destroy();
    return 0;
}
