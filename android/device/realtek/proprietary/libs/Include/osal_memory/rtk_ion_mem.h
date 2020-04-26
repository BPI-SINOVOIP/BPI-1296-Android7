#ifndef _RTK_OSAL_ION_MEM_H_
#define _RTK_OSAL_ION_MEM_H_

#include "osal_type.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void *OSAL_RTKIon_alloc(OSAL_U32 size, OSAL_U8 **noncacheable, OSAL_U32 *phys_addr, ion_user_handle_t *ionhdl);
void OSAL_RTKIon_free(void *addr, OSAL_U32 size, ion_user_handle_t *ionhdl);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // _RTK_OSAL_ION_MEM_H_
