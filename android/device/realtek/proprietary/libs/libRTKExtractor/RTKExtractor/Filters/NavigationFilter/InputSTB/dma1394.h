
#ifndef __DMA_1394_H__
#define __DMA_1394_H__

#include "OSAL.h"
#include "tsp.h"

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#define MAX_PID_FILTER  32
typedef struct dma_1394_buffer_t{
    int                 fd;
    BYTE*               p_buffer_lower;
    BYTE*               p_buffer_upper;
#ifdef DMA_FILE_FAKE
    BYTE*               p_read_pointer;
    BYTE*               p_write_pointer;

    tsp_pid_t           pid_filter_array[MAX_PID_FILTER];
    int                 n_pid_filter;
#endif
} dma_1394_buffer_t;

int
dma_create_buffer(
    dma_1394_buffer_t*  p_dma_buffer,
    int                 fd,
    int                 buffer_size);

int
dma_delete_buffer(
    dma_1394_buffer_t*  p_dma_buffer);

int
dma_size_total(
    dma_1394_buffer_t*  p_dma_buffer);

int
dma_size_read(
    dma_1394_buffer_t*  p_dma_buffer,
    int                 b_check_error);

int
dma_size_readable(
    dma_1394_buffer_t*  p_dma_buffer,
    int                 b_check_error);

int
dma_is_error(
    dma_1394_buffer_t*  p_dma_buffer);

int
dma_reset(
    dma_1394_buffer_t*  p_dma_buffer);

BYTE*
dma_get_read_pointer(
    dma_1394_buffer_t*  p_dma_buffer);

BYTE*
dma_set_read_pointer(
    dma_1394_buffer_t*  p_dma_buffer,
    BYTE*               p_read_pointer);

BYTE*
dma_get_lower_bound(
    dma_1394_buffer_t*  p_dma_buffer);

BYTE*
dma_get_upper_bound(
    dma_1394_buffer_t*  p_dma_buffer);

int
dma_set_pid_filter(
    dma_1394_buffer_t*  p_dma_buffer,
    tsp_pid_t*          p_pid_array,
    int                 n_pid);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __STB_VM_H__

