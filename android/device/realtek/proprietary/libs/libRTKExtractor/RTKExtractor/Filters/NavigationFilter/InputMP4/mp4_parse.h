#ifndef MP4_PARSE_H
#define MP4_PARSE_H

#include "mp4struct.h"

HRESULT mp4_read_header_objects(void *pInstance);
HRESULT SetNextDeliverType(mp4_input_plugin_t *this);

#endif

