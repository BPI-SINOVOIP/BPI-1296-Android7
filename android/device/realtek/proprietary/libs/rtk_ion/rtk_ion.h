#ifndef RTK_ION_API_H
#define RTK_ION_API_H

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <ion/ion.h>
#include <pthread.h>

typedef struct  {
    int mmap_fd;
    /* usually size >= want_size, ex: if want_size == 32B, size = 4KB */
    unsigned int size;  // memroy size got from ION
    unsigned char* ion_virt;
    unsigned char* ion_phy;
    int flag;
    ion_user_handle_t ion_handle;
    unsigned int want_size; // memory size which want to get
} ION_DATA;

typedef struct
{
    int ion_fd;
}ION_HDL;

#ifdef __cplusplus
extern "C" {
#endif

int rtk_ion_malloc(ION_DATA *p, size_t size, size_t align, unsigned int heap, unsigned int flag);
int rtk_ion_free(ION_DATA *p);
void rtk_ion_close();
int rtk_ion_flush_handle_fd(int handle_fd);
int rtk_ion_invalidate_fd(int handle_fd);

#if defined(__LINARO_SDK__)
int ion_flush_range(int fd, int handle_fd, unsigned long addr, unsigned int size);
int ion_invalidate_range(int fd, int handle_fd, unsigned long addr, unsigned int size);
#endif

ION_HDL* ION_hdl_init();
void ION_hdl_uninit(ION_HDL *pInfo);
int ION_hdl_malloc(ION_HDL *hdl, ION_DATA *p, size_t size, size_t align, unsigned int heap, unsigned int flag);
int ION_hdl_free(ION_HDL *hdl, ION_DATA *p);
int ION_hdl_flush_handle_fd(ION_HDL *hdl, int handle_fd);
int ION_hdl_invalidate_fd(ION_HDL *hdl, int handle_fd);
unsigned char *ION_GetPhyFromVirt(ION_DATA *pIon, unsigned char *pVirt);

#ifdef __cplusplus
}
#endif

#endif
