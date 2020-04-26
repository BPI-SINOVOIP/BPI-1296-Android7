#ifndef RTK_LIBRTKALLOCATOR_GRALLOC_WRAPPER_H
#define RTK_LIBRTKALLOCATOR_GRALLOC_WRAPPER_H
#include <hardware/gralloc.h>
#ifdef __cplusplus
class GrallocWrapper {
public:
    GrallocWrapper();
    virtual ~GrallocWrapper();

    int GetPhyAddrs     (buffer_handle_t handle, unsigned int auiPhyAddr[MAX_ION_INFO_ALLOCS]);
    int GetYUVData      (buffer_handle_t handle,void * pYUVData);
    int SetPrivData     (buffer_handle_t handle,void * pPrivData, unsigned int size);
    int GetPrivData     (buffer_handle_t handle,void * pPrivData, unsigned int size);
    int Validate        (buffer_handle_t handle);
    int GetPhyAddrs64   (buffer_handle_t handle,unsigned long long auiPhyAddr[MAX_ION_INFO_ALLOCS]);
    int GetFormat       (buffer_handle_t handle);
    int GetUsage        (buffer_handle_t handle);
    int GetStride       (buffer_handle_t handle);
    int GetWidth        (buffer_handle_t handle);
    int GetHeight       (buffer_handle_t handle);
    int GetSize         (buffer_handle_t handle);
    int GetSSID         (buffer_handle_t handle);
    void Sync           (buffer_handle_t handle);
    void Flush          (buffer_handle_t handle);
    void Invalidate     (buffer_handle_t handle);
private:
    void Init();
    int getIonMmapFd    (buffer_handle_t handle);

    const struct hw_module_t *      psModule;
    mali_gralloc_private_module_t * psGrallocModule;
    int                             mIonFd;

};
#else
typedef struct GrallocWrapper GrallocWrapper;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    GrallocWrapper * GrallocWrapper_new();
    void GrallocWrapper_delete(GrallocWrapper * pInstance);

    int GrallocWrapper_GetPhyAddrs     (GrallocWrapper * pInstance,
            buffer_handle_t handle,
            unsigned int auiPhyAddr[MAX_ION_INFO_ALLOCS]);

    int GrallocWrapper_GetYUVData      (GrallocWrapper * pInstance,
            buffer_handle_t handle,
            void * pYUVData);

    int GrallocWrapper_SetPrivData     (GrallocWrapper * pInstance,
            buffer_handle_t handle,
            void * pPrivData,
            unsigned int size);

    int GrallocWrapper_GetPrivData     (GrallocWrapper * pInstance,
            buffer_handle_t handle,
            void * pPrivData,
            unsigned int size);

    int GrallocWrapper_Validate        (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetPhyAddrs64   (GrallocWrapper * pInstance,
            buffer_handle_t handle,
            unsigned long long auiPhyAddr[MAX_ION_INFO_ALLOCS]);

    int GrallocWrapper_GetFormat       (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetUsage        (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetStride       (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetWidth        (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetHeight       (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetSize         (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    int GrallocWrapper_GetSSID         (GrallocWrapper * pInstance,
            buffer_handle_t handle);


    void GrallocWrapper_Sync           (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    void GrallocWrapper_Flush          (GrallocWrapper * pInstance,
            buffer_handle_t handle);

    void GrallocWrapper_Invalidate     (GrallocWrapper * pInstance,
            buffer_handle_t handle);
#ifdef __cplusplus
}
#endif

#endif /* End of  RTK_LIBRTKALLOCATOR_GRALLOC_WRAPPER_H */
