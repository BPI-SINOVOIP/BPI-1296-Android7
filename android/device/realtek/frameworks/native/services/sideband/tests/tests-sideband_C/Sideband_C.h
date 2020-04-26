#ifndef REALTEK_GUI_SIDEBAND_C_H
#define REALTEK_GUI_SIDEBAND_C_H

#include <system/window.h>
#include <android/native_window.h>

#ifdef __cplusplus
extern "C" {
#endif
int SidebandUser_createSurface              (char * name, int w, int h, int format, int usage);
int SidebandUser_getMinUndequeuedBuffers    (int * pCount);
int SidebandUser_setBuffersUserDimensions   (int w, int h);
int SidebandUser_setBufferFormat            (int format);
int SidebandUser_setBufferUsage             (int usage);
int SidebandUser_setBufferCount             (int count);
int SidebandUser_dequeue                    (/* ANativeWindowBuffer*/void ** pANativeWindowBuffer, buffer_handle_t * buffer);
int SidebandUser_queue                      (/* ANativeWindowBuffer*/void ** pANativeWindowBuffer);
#if 0
int SidebandUser_creatSidebandStream        (native_handle_t ** handle);
#endif
int SidebandUser_destroy();

#ifdef __cplusplus
}
#endif
#endif /* End of REALTEK_GUI_SIDEBAND_C_H */
