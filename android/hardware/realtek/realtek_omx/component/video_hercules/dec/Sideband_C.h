#ifndef REALTEK_GUI_SIDEBAND_C_H
#define REALTEK_GUI_SIDEBAND_C_H

#include <system/window.h>
#include <android/native_window.h>

#ifdef __cplusplus
extern "C" {
#endif
    int SidebandUser_createSurface              (char * name, int w, int h, int format, int usage);
    int SidebandUser_connect                    (int api);
    int SidebandUser_disconnect                 ();
    int SidebandUser_getMinUndequeuedBuffers    ();
    int SidebandUser_setBuffersUserDimensions   (int w, int h);
    int SidebandUser_setBufferFormat            (int format);
    int SidebandUser_setAudioHwSync             (unsigned int nAudioHwSync);
    int SidebandUser_setBufferUsage             (int usage);
    int SidebandUser_setBufferCount             (int count);
    int SidebandUser_dequeue                    (/* ANativeWindowBuffer*/void ** pANativeWindowBuffer, buffer_handle_t * buffer);
    int SidebandUser_enqueue                    (/* ANativeWindowBuffer*/void ** pANativeWindowBuffer);
    int SidebandUser_cancelBuffer               (/* ANativeWindowBuffer*/void ** pANativeWindowBuffer);
    int SidebandUser_creatSidebandStream        (native_handle_t ** handle);
    int SidebandUser_destroy                    ();
    int SidebandUser_flush                      ();

#ifdef __cplusplus
}
#endif
#endif /* End of REALTEK_GUI_SIDEBAND_C_H */
