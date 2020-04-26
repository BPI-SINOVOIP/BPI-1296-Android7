#ifndef MP4_STREAM_H
#define MP4_STREAM_H

#include "mp4struct.h"

#if 1

long mp4_fill_data_pool(mp4_input_plugin_t* this, mp4stream_t *sd);
long mp4_read_stream_align(mp4_input_plugin_t *this, mp4stream_t *sd, long read_len, unsigned char* buffer, int64_t FileOffset);

#else

long mp4_read_stream_align_init(mp4_input_plugin_t *this, mp4stream_t *sd, int initialOffset);
long mp4_read_stream_align(mp4_input_plugin_t *this, mp4stream_t *sd, long read_len);

#endif

long mp4_read_stream_jump_align(mp4_input_plugin_t *this, mp4stream_t *sd, int64_t offset);

long mp4_read_stream(mp4_input_plugin_t *this, mp4stream_t *sd, long ReqSize);

int64_t SetStreamToFramePos(void* pInstance, int track_idx, unsigned long frameIdx);

#endif

