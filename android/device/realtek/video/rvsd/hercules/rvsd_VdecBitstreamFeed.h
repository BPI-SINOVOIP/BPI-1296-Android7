#ifndef RVSD_VDECBITSTREAMFEED_H
#define RVSD_VDECBITSTREAMFEED_H

#include "rpc_common.h"
#include "vdi_osal.h"
#include <ion/ion.h>


enum BufferItemIndex
{
    BlBufferItem,
    ElBufferItem,
    BufferItemAmount
};

class FeedBufferItem
{
public:
    FeedBufferItem();
    ~FeedBufferItem();

    bool Initialize(void **pp_vpu_handle, RINGBUFFER_HEADER **pp_header,
                    unsigned char **pp_virtual, int *p_size);
    bool Uninitialize();

    bool IsInitialized() const;

    unsigned long VirtualToPhysical(unsigned long address) const;
    unsigned long PhysicalToVirtual(unsigned long address) const;

    void PrintRingHeader() const;

    void **mpp_vpu_handle;
    RINGBUFFER_HEADER **mpp_header;
    unsigned char **mpp_virtual;
    int *mp_size;

private:
    bool Allocate();
    bool Release();

    void Reset();

    RINGBUFFER_HEADER *mp_internal_header;
    unsigned long *mp_internal_virtual;
    int m_internal_size;

    unsigned long *mp_ion_physical;

    int m_ion_fd;
    int m_mmap_fd;
    ion_user_handle_t m_ion_handle;

    int m_is_initialized;

};

class Splitter
{
public:
    Splitter();
    ~Splitter();

    bool SetVpuHandle(void **pp_vpu_handle,
                      RINGBUFFER_HEADER **pp_header,
                      unsigned char **pp_virtual,
                      int *p_size,
                      bool is_base);
    bool GetBuffer(void *p_vpu_handle, RINGBUFFER_HEADER **pp_header,
                   unsigned char **pp_base, int *p_size) const;

private:
    FeedBufferItem m_buffer[BufferItemAmount];

    void Allocate();
    void Release();

    int GetFreeBufferIndex() const;
    bool IsBufferIndexValid(int index) const;
};

#endif
