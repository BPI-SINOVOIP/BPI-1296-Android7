INCLUDE(../../../../include/config.txt)

SET(CFLAGS " -DHAVE_CONFIG_H  -D_LARGEFILE64_SOURCE -DNE_LFS -g -O2 -Wall")

SET(INCLUDES ../)

SET(SRCS ne_request.c ne_session.c ne_string.c ne_uri.c ne_dates.c ne_alloc.c ne_utils.c ne_socket.c ne_redirect.c ne_compress.c ne_stats.c ne_stubssl.c)


SET(LIBNAME neon)
SET(LIBRARY_OUTPUT_PATH ../../../../lib/)

INCLUDE_DIRECTORIES(${INCLUDES})
ADD_DEFINITIONS(${CFLAGS})
ADD_LIBRARY(${LIBNAME} ${SRCS})
