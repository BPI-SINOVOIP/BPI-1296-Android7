#ifndef IO_HELPER_H
#define IO_HELPER_H

extern int _x_io_select (int fd, int state, int timeout_msec);

extern int _x_io_tcp_connect_finish( int fd, int timeout_msec);

extern off_t _x_io_tcp_write ( int s, char *buf, off_t todo);

extern int _x_io_tcp_connect_ipv4(const char *host, int port);

/* select states */
#define XIO_READ_READY    1
#define XIO_WRITE_READY   2

/* xine select return codes */
#define XIO_READY         0
#define XIO_ERROR         1
#define XIO_ABORTED       2
#define XIO_TIMEOUT       3

#endif
