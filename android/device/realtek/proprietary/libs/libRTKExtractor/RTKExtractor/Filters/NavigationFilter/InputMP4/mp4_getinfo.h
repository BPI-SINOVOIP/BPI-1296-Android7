#ifndef __MP4_GETINFO__H__
#define __MP4_GETINFO__H__
int64_t mp4_getchunkpos(mp4ff_t *f,int TrackIndex,int index);
unsigned long mp4_getchunksize(mp4ff_t *f,int TrackIndex,int index);
unsigned long mp4_getchunkSamnum(mp4ff_t *f,int TrackIndex,int index);
int64_t mp4_getsamplepos(mp4ff_t *f,int TrackIndex,int index);
unsigned long mp4_getsamplesize(mp4ff_t *f,int TrackIndex,int index);
int64_t mp4_getsampledur(mp4ff_t *f,int TrackIndex,int index);
unsigned int get_LI_byidx(void* pInstance, int track_idx, int idx);
unsigned int get_RI_byidx(void* pInstance, int track_idx, int idx);
unsigned int get_frmIdx_byPTS(void* pInstance, int track_idx, int64_t currSystemPTS);

#endif

