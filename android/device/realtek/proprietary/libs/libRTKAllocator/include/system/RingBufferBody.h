#ifndef RTK_LIBRTKALLOCATOR_RINGBUFFER_BODY_H
#define RTK_LIBRTKALLOCATOR_RINGBUFFER_BODY_H

#include <allocator/Allocator.h>
#include <allocator/IonAllocator.h>
#include <allocator/MallocAllocator.h>

#ifdef __cplusplus
class RingBufferBody : public IonAllocator {
public:
    void * const PHYADDR_ERROR = AllocatorDef::PHYADDR_ERROR;
    void * const VIRADDR_ERROR = AllocatorDef::VIRADDR_ERROR;

    /* Default:
     *      Heap Mask : MediaHeapMask
     *      Heap Flag : SCPUACC|HWIPACC|ACPUACC
     */
    RingBufferBody (
            enum IonAllocator::IonOper oper = IonAllocator::eNoneBuffer,
            long oper_data = 0,
            unsigned int heap_mask = IonAllocator::MediaHeapMask,
            unsigned int flag_mask = (
                IonAllocator::SCPUACC   |
                IonAllocator::ACPUACC   |
                IonAllocator::HWIPACC));

    virtual ~RingBufferBody();

    virtual void                SetEndianSwap       (bool enable = false) {mEndianSwap = enable;};
    virtual bool                GetEndianSwap       (void) {return mEndianSwap;};

    /* Spec API of RingBufferBody op */
    virtual void *              GetParserPtr        (unsigned long offset, unsigned long size);
    virtual void                MemcpySkipFrom      (unsigned long dst_skip_offset, const void * src, unsigned long nByte);
    virtual void                MemcpySeekTo        (unsigned long src_seek_offset, void * dst, unsigned long nByte);

private:
    void                        EndianSwapMemcpy    (void * dst, const void * src, unsigned long size);
    void                        EndianSwap          (void * pVirAddr, unsigned long size);
    MallocAllocator *           mTempMemory;
    MallocAllocator *           mSwapMemory;
    bool                        mEndianSwap;
};

#else
#define RINGBUFFERBODY_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define RINGBUFFERBODY_VIRADDR_ERROR DEF_VIRADDR_ERROR;
typedef struct RingBufferBody   RingBufferBody;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    RingBufferBody *    RingBufferBody_new();
    void                RingBufferBody_delete(RingBufferBody * pInstance);
    void                RingBufferBody_SetEndianSwap       (RingBufferBody * pInstance, int enable);
    int                 RingBufferBody_GetEndianSwap       (RingBufferBody * pInstance);
    void *              RingBufferBody_GetParserPtr        (RingBufferBody * pInstance, unsigned long offset, unsigned long size);
    void                RingBufferBody_MemcpySkipFrom      (RingBufferBody * pInstance, unsigned long dst_skip_offset, const void * src, unsigned long nByte);
    void                RingBufferBody_MemcpySeekTo        (RingBufferBody * pInstance, unsigned long src_seek_offset, void * dst, unsigned long nByte);

#ifdef __cplusplus
}
#endif


#endif /* End of RTK_LIBRTKALLOCATOR_RINGBUFFER_BODY_H */
