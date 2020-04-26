#ifndef RTK_LIBRTKALLOCATOR_RINGBUFFER_H
#define RTK_LIBRTKALLOCATOR_RINGBUFFER_H

#include <allocator/Allocator.h>
#include <system/RingBufferHeader.h>
#include <system/RingBufferBody.h>

#ifdef __cplusplus
class RingBuffer {
public:
    void * const PHYADDR_ERROR = DEF_PHYADDR_ERROR;
    void * const VIRADDR_ERROR = DEF_VIRADDR_ERROR;

    typedef enum ReturnState {
        RB_S_OK                  = 0,
        RB_S_ERROR               = 0x80000000,
        RB_S_ERROR_INSUFFICIENT_RESOURCES,
    } RETURN_STATE;

    RingBuffer();
    virtual ~RingBuffer();

    /* Spec API of RingBuffer sync */
    virtual void                Sync                (void);
    virtual void                Flush               (void);
    virtual void                Invalidate          (void);

    /* Spec API of RingBuffer Header/Body */
    virtual void                SetDeleteHeaderOnRelease    (bool deleteOnRelease = true) {
        mHeader_DeleteOnRelease = deleteOnRelease;
    };
    virtual void                SetDeleteBodyOnRelease      (bool deleteOnRelease = true) {
        mBody_DeleteOnRelease   = deleteOnRelease;
    };
    virtual RingBufferHeader *  GetHeader           (void) {return mHeader;};
    virtual RingBufferBody *    GetBody             (void) {return mBody;};
    virtual void                GenHeader           (void);
    virtual void                GenBody             (void);
    virtual void                ReleaseHeader       (void);
    virtual void                ReleaseBody         (void);
    virtual void                SetHeader           (RingBufferHeader * header);
    virtual void                SetBody             (RingBufferBody * body);
    virtual int                 GetHeaderShareFd    (void);
    virtual int                 GetBodyShareFd      (void);
    virtual RETURN_STATE        ImportHeader        (int fd);
    virtual RETURN_STATE        ImportBody          (int fd);
    virtual RETURN_STATE        Alloc               (unsigned long size);
    virtual RETURN_STATE        Reset               (void);
    virtual RETURN_STATE        SetEndianSwap       (bool enable);
    virtual RETURN_STATE        SetRingBufferType   (unsigned int type);
    virtual void                SetAutoFlush        (bool enable = true);
    virtual void                SetAutoInvalidate   (bool enable = true);

    /* Spec API of RingBuffer Info */
    virtual unsigned long       GetSize             (void);
    virtual int                 GetStorePercentage  (void);
    virtual void *              GetHeaderPhyAddr    (void);
    virtual void *              GetBodyPhyAddr      (void);

    virtual unsigned long       GetWritableSize     (void);
    virtual unsigned long       GetReadableSize     (void);

    /* Spec API of RingBuffer Read/Write */
    virtual RETURN_STATE        ParserBuffer        (void ** ppVirAddr, unsigned long size);
    virtual RETURN_STATE        PushBuffer          (void * src, unsigned long size); /* update write point */
    virtual RETURN_STATE        PullBuffer          (void * dst, unsigned long size); /* update read  point */

private:
    int                         InitHeader          (void);
    bool                        mReady;
    RingBufferHeader *          mHeader;
    RingBufferBody *            mBody;
    bool                        mHeader_DeleteOnRelease;
    bool                        mBody_DeleteOnRelease;
    bool                        mAutoFlush;
    bool                        mAutoInvalidate;
};

#else
#define RINGBUFFER_PHYADDR_ERROR DEF_PHYADDR_ERROR;
#define RINGBUFFER_VIRADDR_ERROR DEF_VIRADDR_ERROR;
#define RINGBUFFER_S_OK                             (0x00000000)
#define RINGBUFFER_S_ERROR                          (0x80000000)
#define RINGBUFFER_S_ERROR_INSUFFICIENT_RESOURCES   (0x80000001)
typedef struct RingBuffer       RingBuffer;
#endif

#ifdef __cplusplus
extern "C" {
#endif
    RingBuffer *        RingBuffer_new();
    void                RingBuffer_delete                       (RingBuffer * pInstance);

    void                RingBuffer_Sync                         (RingBuffer * pInstance);
    void                RingBuffer_Flush                        (RingBuffer * pInstance);
    void                RingBuffer_Invalidate                   (RingBuffer * pInstance);
    void                RingBuffer_SetDeleteHeaderOnRelease     (RingBuffer * pInstance, int deleteOnRelease);
    void                RingBuffer_SetDeleteBodyOnRelease       (RingBuffer * pInstance, int deleteOnRelease);
    RingBufferHeader *  RingBuffer_GetHeader                    (RingBuffer * pInstance);
    RingBufferBody *    RingBuffer_GetBody                      (RingBuffer * pInstance);
    void                RingBuffer_GenHeader                    (RingBuffer * pInstance);
    void                RingBuffer_GenBody                      (RingBuffer * pInstance);
    void                RingBuffer_ReleaseHeader                (RingBuffer * pInstance);
    void                RingBuffer_ReleaseBody                  (RingBuffer * pInstance);
    void                RingBuffer_SetHeader                    (RingBuffer * pInstance, RingBufferHeader * header);
    void                RingBuffer_SetBody                      (RingBuffer * pInstance, RingBufferBody * body);
    int                 RingBuffer_GetHeaderShareFd             (RingBuffer * pInstance);
    int                 RingBuffer_GetBodyShareFd               (RingBuffer * pInstance);
    int                 RingBuffer_ImportHeader                 (RingBuffer * pInstance, int fd);
    int                 RingBuffer_ImportBody                   (RingBuffer * pInstance, int fd);
    int                 RingBuffer_Alloc                        (RingBuffer * pInstance, unsigned long size);
    int                 RingBuffer_Reset                        (RingBuffer * pInstance);
    int                 RingBuffer_SetEndianSwap                (RingBuffer * pInstance, int enable);
    int                 RingBuffer_SetRingBufferType            (RingBuffer * pInstance, unsigned int type);
    void                RingBuffer_SetAutoFlush                 (RingBuffer * pInstance, int enable);
    void                RingBuffer_SetAutoInvalidate            (RingBuffer * pInstance, int enable);
    unsigned long       RingBuffer_GetSize                      (RingBuffer * pInstance);
    int                 RingBuffer_GetStorePercentage           (RingBuffer * pInstance);
    void *              RingBuffer_GetHeaderPhyAddr             (RingBuffer * pInstance);
    void *              RingBuffer_GetBodyPhyAddr               (RingBuffer * pInstance);
    unsigned long       RingBuffer_GetWritableSize              (RingBuffer * pInstance);
    unsigned long       RingBuffer_GetReadableSize              (RingBuffer * pInstance);
    int                 RingBuffer_ParserBuffer                 (RingBuffer * pInstance, void ** ppVirAddr, unsigned long size);
    int                 RingBuffer_PushBuffer                   (RingBuffer * pInstance, void * src, unsigned long size); /* update write point */
    int                 RingBuffer_PullBuffer                   (RingBuffer * pInstance, void * dst, unsigned long size); /* update read  point */

#ifdef __cplusplus
}
#endif

#endif /* End of RTK_LIBRTKALLOCATOR_RINGBUFFER_H */
