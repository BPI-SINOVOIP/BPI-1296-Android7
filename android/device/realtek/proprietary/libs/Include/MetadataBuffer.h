#ifndef METADATA_BUFFER_H
#define METADATA_BUFFER_H

#ifdef __cplusplus
extern "C" {
namespace android {
#endif

typedef struct {
    unsigned int    type;       //kMetadataBufferTypeCameraSource
    unsigned long   handle;     // buffer_handle_t, handle of gralloc
} video_metadata_t;

typedef struct {
    uint32_t        type;       //kMetadataBufferTypePhysicalAddr
    uint32_t        physaddr;   //physical address
    uint32_t        aspect_h;
    uint32_t        aspect_l;
    uint32_t        vui_frame_rate;
    unsigned long   *pLockaddr;
} phys_metadata_t;

#ifdef __cplusplus
}  // namespace android
}
#endif

#endif  // METADATA_BUFFER_H
