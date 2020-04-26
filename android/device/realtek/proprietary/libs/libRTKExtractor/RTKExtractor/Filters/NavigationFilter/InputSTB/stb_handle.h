#ifndef __STB_HANDLE_H__
#define __STB_HANDLE_H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

typedef struct stb_handle_t stb_handle_t;

stb_handle_t*
stb_io_handle_create(
    char*                   p_mrl);

void
stb_io_handle_delete(
    stb_handle_t*           p_stb_handle);

int
stb_io_read(
    stb_handle_t*           p_stb_handle,
    int                     offset_read,
    BYTE**                  pp_buffer,
    int*                    p_length);

int
stb_io_pid_filter(
    stb_handle_t*           p_stb_handle,
    tsp_pid_t*              p_pid_array,
    int                     len_pid_array);

int
stb_io_seek(
    stb_handle_t*           p_stb_handle,
    int                     offset_fuck);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __STB_HANDLE_H__
