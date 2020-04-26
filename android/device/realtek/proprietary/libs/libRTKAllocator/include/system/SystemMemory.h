#ifndef RTK_LIBRTKALLOCATOR_SYSTEM_MEMORY_H
#define RTK_LIBRTKALLOCATOR_SYSTEM_MEMORY_H

#include <allocator/Allocator.h>

#ifdef __cplusplus
class SystemMemory : public AllocatorDef {
public:
    void * const PHYADDR_ERROR = AllocatorDef::PHYADDR_ERROR;
    void * const VIRADDR_ERROR = AllocatorDef::VIRADDR_ERROR;

    SystemMemory();
    virtual ~SystemMemory();

    /* Spec API of SystemMemory */
    virtual void            setAllocator(AllocatorDef * pAllocator, bool deleteOnRelease = true);
    virtual void            setDeleteAllocatorOnRelease(bool deleteOnRelease) {
        mAllocator_DeleteOnRelease = deleteOnRelease;
    };
    virtual void            genIonAllocator(bool deleteOnRelease = true);
    virtual void            genMallocAllocator(bool deleteOnRelease = true);
    virtual void            releaseAllocator();
    virtual AllocatorDef *  getAllocator() {return mAllocator;};
    virtual void            setCheckSizeAnyTime(bool en);

    /* Base API of AllocatorDef */
    virtual int             Alloc(unsigned long nByte);
    virtual int             ReAlloc(unsigned long nByte);
    virtual void            Free();
    virtual unsigned long   GetSize();
    virtual void *          GetVirAddr();
    virtual void *          GetPhyAddr();
    virtual void            Sync();
    virtual void            Flush();
    virtual void            Invalidate();
    virtual void            FlushRange	    (unsigned long addr, unsigned long size);
    virtual void            InvalidateRange	(unsigned long addr, unsigned long size);

    /* Spec API of SystemMemory */
    virtual void            CheckSize       (unsigned long byte); // realloc
    virtual void            MemcpyFrom      (const void * src, unsigned long nByte);
    virtual void            MemcpyTo        (void * dest, unsigned long nByte);
    virtual void            MemcpySkipFrom  (unsigned long dst_skip_offset, const void * src, unsigned long nByte);
    virtual void            MemcpySeekTo    (unsigned long src_seek_offset, void * dest, unsigned long nByte);

    /* Spec API of SystemMemory */
    virtual void            PUT_BYTE        (unsigned char data);
    virtual void            PUT_BUFFER      (void * buf, unsigned long len);
    virtual void            PUT_LE32        (unsigned int data);
    virtual void            PUT_BE32        (unsigned int data);
    virtual void            PUT_LE16        (unsigned int data);
    virtual void            PUT_BE16        (unsigned int data);
    virtual void            PUT_RESET       () {mPutOffset = 0;};
    virtual int             PUT_SIZE        () {return (int) mPutOffset;};
    virtual void            PUT_OFFSET      (unsigned long offset) {mPutOffset = offset;};

private:
    AllocatorDef *  mAllocator;
    bool            mAllocator_DeleteOnRelease;
    unsigned long   mPutOffset;
    bool            mCheckSizeAnyTime;
};
#else
#define SYSTEMMEMORY_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define SYSTEMMEMORY_VIRADDR_ERROR DEF_VIRADDR_ERROR;

typedef struct SystemMemory SystemMemory;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    SystemMemory *  SystemMemory_new();
    void            SystemMemory_delete(SystemMemory * pInstance);

    /* Spec API of SystemMemory */
    void            SystemMemory_setAllocator                       (SystemMemory * pInstance, AllocatorDef * pAllocator);
    void            SystemMemory_setDeleteAllocatorOnRelease        (SystemMemory * pInstance, int deleteOnRelease);
    void            SystemMemory_genIonAllocator                    (SystemMemory * pInstance);
    void            SystemMemory_genMallocAllocator                 (SystemMemory * pInstance);
    void            SystemMemory_releaseAllocator                   (SystemMemory * pInstance);
    AllocatorDef *  SystemMemory_getAllocator                       (SystemMemory * pInstance);
    void            SystemMemory_setCheckSizeAnyTime                (SystemMemory * pInstance, int en);

    /* Base API of AllocatorDef */
    int             SystemMemory_Alloc                              (SystemMemory * pInstance, unsigned long nByte);
    int             SystemMemory_ReAlloc                            (SystemMemory * pInstance, unsigned long nByte);
    void            SystemMemory_Free                               (SystemMemory * pInstance);
    unsigned long   SystemMemory_GetSize                            (SystemMemory * pInstance);
    void *          SystemMemory_GetVirAddr                         (SystemMemory * pInstance);
    void *          SystemMemory_GetPhyAddr                         (SystemMemory * pInstance);
    void            SystemMemory_Sync                               (SystemMemory * pInstance);
    void            SystemMemory_Flush                              (SystemMemory * pInstance);
    void            SystemMemory_Invalidate                         (SystemMemory * pInstance);
    void            SystemMemory_FlushRange                         (SystemMemory * pInstance, unsigned long addr, unsigned long size);
    void            SystemMemory_InvalidateRange                    (SystemMemory * pInstance, unsigned long addr, unsigned long size);

    /* Spec API of SystemMemory */
    void            SystemMemory_CheckSize                          (SystemMemory * pInstance, unsigned long byte);
    void            SystemMemory_MemcpyFrom                         (SystemMemory * pInstance, const void * src, unsigned long nByte);
    void            SystemMemory_MemcpyTo                           (SystemMemory * pInstance, void * dest, unsigned long nByte);
    void            SystemMemory_MemcpySkipFrom                     (SystemMemory * pInstance, unsigned long dst_skip_offset, const void * src, unsigned long nByte);
    void            SystemMemory_MemcpySeekTo                       (SystemMemory * pInstance, unsigned long src_seek_offset, void * dest, unsigned long nByte);

    /* Spec API of SystemMemory */
    void            SystemMemory_PUT_BYTE                           (SystemMemory * pInstance, unsigned char data);
    void            SystemMemory_PUT_BUFFER                         (SystemMemory * pInstance, void * buf, unsigned long len);
    void            SystemMemory_PUT_LE32                           (SystemMemory * pInstance, unsigned int data);
    void            SystemMemory_PUT_BE32                           (SystemMemory * pInstance, unsigned int data);
    void            SystemMemory_PUT_LE16                           (SystemMemory * pInstance, unsigned int data);
    void            SystemMemory_PUT_BE16                           (SystemMemory * pInstance, unsigned int data);
    void            SystemMemory_PUT_RESET                          (SystemMemory * pInstance);
    int             SystemMemory_PUT_SIZE                           (SystemMemory * pInstance);
    void            SystemMemory_PUT_OFFSET                         (SystemMemory * pInstance, unsigned long offset);

#ifdef __cplusplus
}
#endif
#endif /* End of  RTK_LIBRTKALLOCATOR_SYSTEM_MEMORY_H */
