#include <allocator/IonAllocator.h>

class SRBuffer_priv {
public:
    SRBuffer_priv(enum IonAllocator::IonOper oper, long size, unsigned int heap_mask, unsigned int heap_flag);
    SRBuffer_priv(enum IonAllocator::IonOper oper, int fd);
    virtual ~SRBuffer_priv();
    virtual void *          GetPhyAddr();
    virtual void *          GetVirAddr();
    virtual int             GetShareFd();
    virtual void            Sync();
    virtual void            Flush();
    virtual void            Invalidate();
    virtual unsigned long   GetSize();
private:
    IonAllocator *      mAlloc;
};
