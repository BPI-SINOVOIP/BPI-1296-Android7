#ifndef RTK_LIBRTKALLOCATOR_MALLOC_ALLOCATOR_H
#define RTK_LIBRTKALLOCATOR_MALLOC_ALLOCATOR_H

#include <allocator/Allocator.h>

#ifdef __cplusplus
class MallocAllocator : public AllocatorDef {
public:
    void * const PHYADDR_ERROR = AllocatorDef::PHYADDR_ERROR;
    void * const VIRADDR_ERROR = AllocatorDef::VIRADDR_ERROR;

    MallocAllocator();
    virtual ~MallocAllocator();

    /* Base API of AllocatorDef */
    virtual int                 Alloc           (unsigned long nByte);
    virtual int                 ReAlloc         (unsigned long nByte);
    virtual void                Free            (void);
    virtual unsigned long       GetSize         (void) {return mSize;};
    virtual void *              GetVirAddr      (void) {return mPool;};
    virtual void *              GetPhyAddr      (void) {return PHYADDR_ERROR;};
    virtual void                Sync            (void);
private:
    void * mPool;
    unsigned long mSize;
};
#else
#define MALLOCALLOCATOR_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define MALLOCALLOCATOR_VIRADDR_ERROR DEF_VIRADDR_ERROR;

typedef struct MallocAllocator MallocAllocator;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    MallocAllocator *  MallocAllocator_new        (void);

    void            MallocAllocator_delete     (MallocAllocator * pInstance);

    /* Base API of AllocatorDef */
    int             MallocAllocator_Alloc      (MallocAllocator * pInstance, unsigned long nByte);
    int             MallocAllocator_ReAlloc    (MallocAllocator * pInstance, unsigned long nByte);
    void            MallocAllocator_Free       (MallocAllocator * pInstance);
    unsigned long   MallocAllocator_GetSize    (MallocAllocator * pInstance);
    void *          MallocAllocator_GetVirAddr (MallocAllocator * pInstance);
    void *          MallocAllocator_GetPhyAddr (MallocAllocator * pInstance);
    void            MallocAllocator_Sync       (MallocAllocator * pInstance);
    void            MallocAllocator_Flush      (MallocAllocator * pInstance);
    void            MallocAllocator_Invalidate (MallocAllocator * pInstance);

#ifdef __cplusplus
}
#endif
#endif /* End of RTK_LIBRTKALLOCATOR_MALLOC_ALLOCATOR_H */
