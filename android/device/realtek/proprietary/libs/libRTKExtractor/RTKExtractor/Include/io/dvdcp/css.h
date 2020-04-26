#ifndef _CSS_H_
#define _CSS_H_
#include <io/dvdcp/dvdcp.h>

typedef struct css_s
{
    int             i_agid;      /* Current Authenication Grant ID. */
    dvd_key_t       p_bus_key;   /* Current session key. */
    dvd_key_t       p_disc_key;  /* This DVD disc's key. */
    dvd_key_t       p_title_key; /* Current title key. */
} css_t;

#endif //_CSS_H_

