#ifndef __MCP_EXPORT_API_H__
#define __MCP_EXPORT_API_H__


#ifdef __cplusplus
extern "C" {
#endif
#include "osal_type.h"

#define SHA256_SIZE	32
typedef int ion_user_handle_t;

int SHA1_dohash(unsigned int pDataIn, int length, unsigned int pDataOut, unsigned int iv[5]);
int SHA256_dohash(unsigned int pDataIn, int length, unsigned int pDataOut, unsigned int iv[8]);

int RTKIon_alloc(size_t size, unsigned char **virt_addr, unsigned long *phys_addr, ion_user_handle_t *ionhdl);
void RTKIon_free(void *addr, unsigned int size, int mapfd, ion_user_handle_t ionhdl);
int RTKIon_flush(int mapfd);
int RTKIon_invalidate(int mapfd);
#ifdef __cplusplus
}
#endif

#endif
