#ifndef RVSD_ION_H
#define RVSD_ION_H

#include "vdi_osal.h"
#include <ion/ion.h>


class RvsdIonAllocator
{
public:
    RvsdIonAllocator();
    ~RvsdIonAllocator();

    bool Allocate(int size, unsigned int mask, unsigned int flag);
    bool Release();

    unsigned long *mp_virtual;
    unsigned long *mp_physical;
    int m_size;

private:
    void Reset();

    int m_ion_fd;
    int m_mmap_fd;
    ion_user_handle_t m_ion_handle;
};

#endif
