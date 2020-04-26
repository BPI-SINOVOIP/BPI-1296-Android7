
#ifndef RTK_OSAL_H
#define RTK_OSAL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Types compatible with OMX types */
typedef unsigned long OSAL_U32;
typedef unsigned char OSAL_U8;
typedef long OSAL_S32;
typedef char OSAL_S8;
typedef void * OSAL_PTR;
typedef unsigned long OSAL_ERRORTYPE;
//typedef unsigned long OSAL_BOOL;

typedef enum OSAL_BOOL {
    OSAL_FALSE = 0,
    OSAL_TRUE = !OSAL_FALSE,
    OSAL_BOOL_MAX = 0x7FFFFFFF
} OSAL_BOOL;
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
