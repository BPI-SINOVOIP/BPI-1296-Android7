#ifndef _DWC3_DEFS_H_
#define _DWC3_DEFS_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>	// for printf
#include <string.h>	// for memcpy

/** Data types needed by the PCD */
typedef uint64_t	u64, u_int64_t;
typedef uint32_t	u32, u_int32_t;
typedef uint16_t	u16, u_int16_t;
typedef uint8_t		u8, u_int8_t;

typedef int64_t		s64;
typedef int32_t		s32;
typedef int16_t		s16;
typedef int8_t		s8;

typedef unsigned long	u_long;
typedef unsigned int	u_int;
typedef unsigned short	u_short;
typedef unsigned char	u_char;

/** Data type for DMA addresses */
typedef unsigned long		dwc_dma_t;
#define DWC_DMA_ADDR_INVALID	(~(dwc_dma_t)0)
/** @} */

/** Compiler 'packed' attribute */
#define UPACKED		__attribute__ ((__packed__))

/** Compiler 'aligned(16)' attribute */
#define UALIGNED16	__attribute__ ((__aligned__(16)))

/** I/O memory attribute for pointers. Needed for Linux "sparse" tool. */
#define __iomem		/* */

// from /usr/include/asm-x86_64/errno.h
#define EIO		5	/* I/O error */
#define EAGAIN		11	/* Try again */
#define ENOMEM		12	/* Out of memory */
#define EBUSY		16	/* Device or resource busy */
#define ENODEV		19	/* No such device */
#define EINVAL		22	/* Invalid argument */
#define ENOSPC		28	/* No space left on device */
#define EPIPE		32	/* Broken pipe */
#define EDOM		33	/* Math argument out of domain of func */
#define ENODATA		61	/* No data available */
#define ENOSR		63	/* Out of streams resources */
#define ECOMM		70	/* Communication error on send */
#define EPROTO		71	/* Protocol error */
#define EOVERFLOW	75	/* Value too large for defined data type */
#define ERESTART	85	/* Interrupted system call should be restarted */
#define EOPNOTSUPP	95	/* Operation not supported on transport endpoint */
#define ECONNABORTED	103	/* Software caused connection abort */
#define ECONNRESET	104	/* Connection reset by peer */
#define ESHUTDOWN	108	/* Cannot send after transport endpoint shutdown */
#define ETIMEDOUT	110	/* Connection timed out */
#define EINPROGRESS	115	/* Operation now in progress */

/** @name Error Codes */
#define DWC_E_INVALID		EINVAL
#define DWC_E_NO_MEMORY		ENOMEM
#define DWC_E_NO_DEVICE		ENODEV
#define DWC_E_NOT_SUPPORTED	EOPNOTSUPP
#define DWC_E_TIMEOUT		ETIMEDOUT
#define DWC_E_BUSY		EBUSY
#define DWC_E_AGAIN		EAGAIN
#define DWC_E_RESTART		ERESTART
#define DWC_E_ABORT		ECONNABORTED
#define DWC_E_SHUTDOWN		ESHUTDOWN
#define DWC_E_NO_DATA		ENODATA
#define DWC_E_DISCONNECT	ECONNRESET
#define DWC_E_UNKNOWN		EINVAL
#define DWC_E_NO_STREAM_RES	ENOSR
#define DWC_E_COMMUNICATION	ECOMM
#define DWC_E_OVERFLOW		EOVERFLOW
#define DWC_E_PROTOCOL		EPROTO
#define DWC_E_IN_PROGRESS	EINPROGRESS
#define DWC_E_PIPE		EPIPE
#define DWC_E_IO		EIO
#define DWC_E_NO_SPACE		ENOSPC
#define DWC_E_DOMAIN		EDOM

/**
 * The number of DMA Descriptors (TRBs) to allocate for each endpoint type.
 * NOTE: The driver currently supports more than 1 TRB for Isoc EPs only.
 * So the values for Bulk and Intr must be 1.
 */
#define DWC_NUM_BULK_TRBS	1
#define DWC_NUM_INTR_TRBS	1
#define DWC_NUM_ISOC_TRBS	32

#define USSE_GET_MAX_STREAMS(a) 	((a) & 0x1f)
#define USSE_SET_MAX_STREAMS(a, b) 	((a) | ((b) & 0x1f))
#define USSE_GET_MAX_PACKET_NUM(a) 	((a) & 0x03)
#define USSE_SET_MAX_PACKET_NUM(a, b) 	((a) | ((b) & 0x03))

#endif /* _DWC3_DEFS_H_ */
