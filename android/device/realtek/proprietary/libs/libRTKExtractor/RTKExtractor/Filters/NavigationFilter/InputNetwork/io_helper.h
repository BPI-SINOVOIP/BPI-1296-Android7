#ifndef IO_HELPER_H
#define IO_HELPER_H



/* select states */
#define XIO_READ_READY    1
#define XIO_WRITE_READY   2

/* xine select return codes */
#define XIO_READY         0
#define XIO_ERROR         1
#define XIO_ABORTED       2
#define XIO_TIMEOUT       3


/*
 * Waits for a file descriptor/socket to change status.
 *
 * network input plugins should use this function in order to
 * not freeze the engine.
 *
 * params :
 *   stream        needed for aborting and reporting errors but may be NULL
 *   fd            file/socket descriptor
 *   state         XIO_READ_READY, XIO_WRITE_READY
 *   timeout_sec   timeout in seconds
 *
 * An other thread can abort this function if stream != NULL by setting
 * stream->demux_action_pending.
 *
 * return value :
 *   XIO_READY     the file descriptor is ready for cmd
 *   XIO_ERROR     an i/o error occured
 *   XIO_ABORTED   command aborted by an other thread
 *   XIO_TIMEOUT   the file descriptor is not ready after timeout_msec milliseconds
 */
int _x_io_select (void *stream, int fd, int state, int timeout_msec);


/*
 * open a tcp connection
 *
 * params :
 *   stream        needed for reporting errors but may be NULL
 *   host          address of target
 *   port          port on target
 *
 * returns a socket descriptor or -1 if an error occured
 */
int _x_io_tcp_connect(void *stream, const char *host, int port);

/*
 * wait for finish connection
 *
 * params :
 *   stream        needed for aborting and reporting errors but may be NULL
 *   fd            socket descriptor
 *   timeout_msec  timeout in milliseconds
 *
 * return value:
 *   XIO_READY     host respond, the socket is ready for cmd
 *   XIO_ERROR     an i/o error occured
 *   XIO_ABORTED   command aborted by an other thread
 *   XIO_TIMEOUT   the file descriptor is not ready after timeout
 */
int _x_io_tcp_connect_finish(void *stream, int fd, int timeout_msec);

/*
 * read from tcp socket checking demux_action_pending
 *
 * network input plugins should use this function in order to
 * not freeze the engine.
 *
 * aborts with zero if no data is available and *abort is set
 */
off_t _x_io_tcp_read (void *stream, int s, char *buf, off_t todo);


/*
 * write to a tcp socket checking demux_action_pending
 *
 * network input plugins should use this function in order to
 * not freeze the engine.
 *
 * aborts with zero if no data is available and *abort is set
 */
off_t _x_io_tcp_write (void *stream, int s, char *buf, off_t todo);

/*
 * read from a file descriptor checking demux_action_pending
 *
 * the fifo input plugin should use this function in order to
 * not freeze the engine.
 *
 * aborts with zero if no data is available and *abort is set
 */
off_t _x_io_file_read (void *stream, int fd, char *buf, off_t todo);


/*
 * write to a file descriptor checking demux_action_pending
 *
 * the fifo input plugin should use this function in order to
 * not freeze the engine.
 *
 * aborts with zero if *abort is set
 */
off_t _x_io_file_write (void *stream, int fd, char *buf, off_t todo);

/*
 * read a string from socket, return string length (same as strlen)
 * the string is always '\0' terminated but given buffer size is never exceeded
 * that is, _x_io_tcp_read_line(,,,X) <= (X-1) ; X > 0
 */
int _x_io_tcp_read_line(void *stream, int sock, char *str, int size);

int _x_io_tcp_connect_ipv4(void *stream, const char *host, int port);

#endif
