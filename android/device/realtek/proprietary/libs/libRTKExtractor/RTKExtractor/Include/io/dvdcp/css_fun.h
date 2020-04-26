#ifndef _CSS_FUN_H_
#define _CSS_FUN_H_

int   _dvdcp_css_title       ( dvdcp_t );
int   _dvdcp_disckey     ( dvdcp_t );
int   _dvdcp_unscramble  ( uint8_t *, uint8_t * );
void _dvdcp_error( dvdcp_t, char * );
int ioctl_ReadCopyright     ( int, int, int * );

#endif //_CSS_FUN_H_
 
