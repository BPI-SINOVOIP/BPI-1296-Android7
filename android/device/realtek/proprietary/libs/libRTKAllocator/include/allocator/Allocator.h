#ifndef RTK_LIBRTKALLOCATOR_ALLOCATOR_DEF_H
#define RTK_LIBRTKALLOCATOR_ALLOCATOR_DEF_H

#include <stdlib.h>
#include <stddef.h>
#include <sys/mman.h>
#include <memory.h>

#define DEF_PHYADDR_ERROR ((void *) -1)
#define DEF_VIRADDR_ERROR (MAP_FAILED)

#ifdef __cplusplus
class AllocatorDef {
public:
    void * const PHYADDR_ERROR = DEF_PHYADDR_ERROR;
    void * const VIRADDR_ERROR = DEF_VIRADDR_ERROR;

    AllocatorDef() {};
    virtual ~AllocatorDef() {};

    virtual int                 Alloc           (unsigned long nByte) = 0;
    virtual int                 ReAlloc         (unsigned long nByte) = 0;
    virtual void                Free            (void) = 0;
    virtual unsigned long       GetSize         (void) = 0;
    virtual void *              GetVirAddr      (void) = 0;
    virtual void *              GetPhyAddr      (void) = 0;
    virtual void                Sync            (void) = 0;
    virtual void                Flush           (void) {return Sync();};
    virtual void                Invalidate      (void) {return Sync();};
    virtual void                FlushRange      (unsigned long /*addr*/, unsigned long /*size*/) {return Flush();};
    virtual void                InvalidateRange (unsigned long /*addr*/, unsigned long /*size*/) {return Invalidate();};
};
#else
#define ALLOCATORDEF_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define ALLOCATORDEF_VIRADDR_ERROR DEF_VIRADDR_ERROR;

typedef struct AllocatorDef AllocatorDef;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    void                AllocatorDef_delete          (AllocatorDef * pInstance);

    int                 AllocatorDef_Alloc           (AllocatorDef * pInstance, unsigned long nByte);
    int                 AllocatorDef_ReAlloc         (AllocatorDef * pInstance, unsigned long nByte);
    void                AllocatorDef_Free            (AllocatorDef * pInstance);
    unsigned long       AllocatorDef_GetSize         (AllocatorDef * pInstance);
    void *              AllocatorDef_GetVirAddr      (AllocatorDef * pInstance);
    void *              AllocatorDef_GetPhyAddr      (AllocatorDef * pInstance);
    void                AllocatorDef_Sync            (AllocatorDef * pInstance);
    void                AllocatorDef_Flush           (AllocatorDef * pInstance);
    void                AllocatorDef_Invalidate      (AllocatorDef * pInstance);
    void                AllocatorDef_FlushRange      (AllocatorDef * pInstance, unsigned long addr, unsigned long size);
    void                AllocatorDef_InvalidateRange (AllocatorDef * pInstance, unsigned long addr, unsigned long size);
#ifdef __cplusplus
}
#endif

#endif /* End of RTK_LIBRTKALLOCATOR_ALLOCATOR_DEF_H */
