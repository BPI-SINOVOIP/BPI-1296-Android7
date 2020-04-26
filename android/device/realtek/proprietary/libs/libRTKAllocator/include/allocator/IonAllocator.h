#ifndef RTK_LIBRTKALLOCATOR_ION_ALLOCATOR_H
#define RTK_LIBRTKALLOCATOR_ION_ALLOCATOR_H

#include <allocator/Allocator.h>
#include <ion/ion.h>

#ifdef __cplusplus
class IonAllocator : public AllocatorDef {
public:
    void * const PHYADDR_ERROR = AllocatorDef::PHYADDR_ERROR;
    void * const VIRADDR_ERROR = AllocatorDef::VIRADDR_ERROR;

    enum IonOper {
        eNoneBuffer,
        eAllocBuffer,
        eImportBuffer,
        eImportBuffer_DontMmap,
    };

    enum IonHeapMask {
        MediaHeapMask   = 0x1U << 0,
        AudioHeapMask   = 0x1U << 1,
        SecureHeapMask  = 0x1U << 2,
    };

    enum IonFlagMask {
        NONCACHED       = 0x1U << 0,
        SCPUACC         = 0x1U << 1,
        ACPUACC         = 0x1U << 2,
        HWIPACC         = 0x1U << 3,
        PROTECTED       = 0x1U << 4,
        SECURE_AUDIO    = 0x1U << 5,
    };

    /* Default:
     *      Heap Mask : MediaHeapMask
     *      Heap Flag : NONCACHED|SCPUACC|HWIPACC
     */
    IonAllocator(enum IonOper oper = eNoneBuffer, long oper_data = 0,
            unsigned int heap_mask = MediaHeapMask,
            unsigned int flag_mask = (NONCACHED|SCPUACC|HWIPACC));

    virtual ~IonAllocator();

    /* Base API of AllocatorDef */
    virtual int                 Alloc           (unsigned long nByte);
    virtual int                 ReAlloc         (unsigned long nByte);
    virtual void                Free            (void);
    virtual unsigned long       GetSize         (void) {return mSize;};
    virtual void *              GetVirAddr      (void) {return mViraddr;};
    virtual void *              GetPhyAddr      (void) {return mPhyaddr;};
    virtual void                Sync            (void);
    virtual void                Flush           (void);
    virtual void                Invalidate      (void);
    virtual void                FlushRange      (unsigned long addr, unsigned long size);
    virtual void                InvalidateRange (unsigned long addr, unsigned long size);

    /* Spec API of IonAllocator */
    virtual void                SetHeapMask     (unsigned int heap_mask) {mHeapMask = heap_mask;};
    virtual void                SetHeapFlag     (unsigned int heap_flag) {mFlagMask = heap_flag;};
    virtual unsigned int        GetHeapMask     (void) {return mHeapMask;};
    virtual unsigned int        GetHeapFlag     (void) {return mFlagMask;};
    virtual int                 ImportBuffer    (int import_fd, bool dont_mmap = false);
    virtual int                 GetShareFd      (void) {return mIonMmapFd;};
    virtual int                 GetIonFd        (void) {return mIonFd;};

private:
    virtual void allocBuffer(unsigned long bufferSize);
    virtual void importBuffer(int import_fd, bool dont_mmap);
    virtual void freeBuffer();

    enum {
        sNoneBuffer,
        sAllocBuffer,
        sImportBuffer,
        sImportBuffer_DontMmap,
    } mState;

    int                 mIonFd;
    int                 mIonMmapFd;
    void *              mViraddr;
    void *              mPhyaddr;
    unsigned long       mSize;
    ion_user_handle_t   mIonHandle;
    unsigned int        mHeapMask;
    unsigned int        mFlagMask;
};
#else
#define IONALLOCATOR_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define IONALLOCATOR_VIRADDR_ERROR DEF_VIRADDR_ERROR;

enum IonHeapMask {
    IonAllocator_MediaHeapMask      = 0x1U << 0,
    IonAllocator_AudioHeapMask      = 0x1U << 1,
    IonAllocator_SecureHeapMask     = 0x1U << 2,
};

enum IonFlagMask {
    IonAllocator_NONCACHED          = 0x1U << 0,
    IonAllocator_SCPUACC            = 0x1U << 1,
    IonAllocator_ACPUACC            = 0x1U << 2,
    IonAllocator_HWIPACC            = 0x1U << 3,
    IonAllocator_PROTECTED          = 0x1U << 4,
    IonAllocator_SECURE_AUDIO       = 0x1U << 5,
};

typedef struct IonAllocator IonAllocator;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    IonAllocator *  IonAllocator_new        (void);

    void            IonAllocator_delete     (IonAllocator * pInstance);

    /* Base API of AllocatorDef */
    int             IonAllocator_Alloc      (IonAllocator * pInstance, unsigned long nByte);
    int             IonAllocator_ReAlloc    (IonAllocator * pInstance, unsigned long nByte);
    void            IonAllocator_Free       (IonAllocator * pInstance);
    unsigned long   IonAllocator_GetSize    (IonAllocator * pInstance);
    void *          IonAllocator_GetVirAddr (IonAllocator * pInstance);
    void *          IonAllocator_GetPhyAddr (IonAllocator * pInstance);
    void            IonAllocator_Sync       (IonAllocator * pInstance);
    void            IonAllocator_Flush      (IonAllocator * pInstance);
    void            IonAllocator_Invalidate (IonAllocator * pInstance);
    void            IonAllocator_FlushRange      (IonAllocator * pInstance, unsigned long addr, unsigned long size);
    void            IonAllocator_InvalidateRange (IonAllocator * pInstance, unsigned long addr, unsigned long size);

    /* Spec API of IonAllocator */
    void            IonAllocator_SetHeapMask            (IonAllocator * pInstance, unsigned int heap_mask);
    void            IonAllocator_SetHeapFlag            (IonAllocator * pInstance, unsigned int heap_flag);
    unsigned int    IonAllocator_GetHeapMask            (IonAllocator * pInstance);
    unsigned int    IonAllocator_GetHeapFlag            (IonAllocator * pInstance);
    int             IonAllocator_ImportBuffer           (IonAllocator * pInstance, int import_fd);
    int             IonAllocator_ImportBuffer_DontMmap  (IonAllocator * pInstance, int import_fd);
    int             IonAllocator_GetShareFd             (IonAllocator * pInstance);
    int             IonAllocator_GetIonFd               (IonAllocator * pInstance);

#ifdef __cplusplus
}
#endif


#endif /* End of RTK_LIBRTKALLOCATOR_ION_ALLOCATOR_H */
