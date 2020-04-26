#ifndef BSWAP_H_INCLUDED
#define BSWAP_H_INCLUDED


#include <Filters/config.h>

#if defined(WORDS_BIGENDIAN)
/* All bigendian systems are fine, just ignore the swaps. */  
#define B2N_16(x) (void)(x)
#define B2N_32(x) (void)(x)
#define B2N_64(x) (void)(x)

#else 

/* For __FreeBSD_version */
#include <sys/param.h>

#include <byteswap.h>
#define B2N_16(x) x = bswap_16(x)
#define B2N_32(x) x = bswap_32(x)
#define B2N_64(x) x = bswap_64(x)

#endif /* WORDS_BIGENDIAN */

#endif /* BSWAP_H_INCLUDED */
