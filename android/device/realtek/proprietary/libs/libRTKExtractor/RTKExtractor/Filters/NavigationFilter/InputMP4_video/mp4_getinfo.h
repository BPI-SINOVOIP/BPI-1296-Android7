#ifndef __MP4_GETINFO__H__
#define __MP4_GETINFO__H__
extern int64_t mp4_getchunkpos(mp4ff_t *f,int TrackIndex,int index);
extern unsigned long mp4_getchunksize(mp4ff_t *f,int TrackIndex,int index);
extern unsigned long mp4_getchunkSamnum(mp4ff_t *f,int TrackIndex,int index);
extern int64_t mp4_getsamplepos(mp4ff_t *f,int TrackIndex,int index);
extern unsigned long mp4_getsamplesize(mp4ff_t *f,int TrackIndex,int index);
extern int64_t mp4_getsampledur(mp4ff_t *f,int TrackIndex,int index);
extern void testtest(mp4_input_plugin_t *this);
#endif

