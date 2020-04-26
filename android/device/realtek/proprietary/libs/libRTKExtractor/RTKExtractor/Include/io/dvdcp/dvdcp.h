#ifndef _DVDCP_H_
#define _DVDCP_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_SIZE 5
#define DVD_KEY_SIZE 5
#define DVD_CHALLENGE_SIZE 10
#define DVD_DISCKEY_SIZE 2048

typedef uint8_t dvd_key_t[KEY_SIZE];

#include "css.h"
#ifdef USE_CPRM
#include "cprm.h"
#endif

typedef struct dvd_title_s
{
    int                 i_startlb;
    dvd_key_t           p_key;
    struct dvd_title_s *p_next;
} dvd_title_t;

struct dvdcp_s
{
    /* File descriptor */
    int    i_fd;
    int    i_pos;

    /* Error management */
    int    b_errors;
    int    b_debug;

    /* Decryption stuff */
    int          b_ioctls;
    int          b_scrambled;
    int		 cgms;
    dvd_title_t *p_titles;
    union
    {
	css_t        css;
#ifdef USE_CPRM
	cprm_t       cprm;
#endif
    };
};


/** Library instance handle, to be used for each library call. */
typedef struct dvdcp_s* dvdcp_t;

/** The block size of a DVD. */
#define DVDCP_BLOCK_SIZE      2048

void _dvdcp_error ( dvdcp_t, char * );
void _dvdcp_debug ( dvdcp_t, char * );

#ifdef __cplusplus
}
#endif


#endif //_DVDCP_H_
