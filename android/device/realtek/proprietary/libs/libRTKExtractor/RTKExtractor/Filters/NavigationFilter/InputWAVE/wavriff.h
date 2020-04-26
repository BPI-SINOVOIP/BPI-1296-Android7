#ifndef __WAVE_RIFF_HEADER__
#define __WAVE_RIFF_HEADER__

#define FCC(ch4) ((((uint32_t)(ch4) & 0xFF) << 24) |     \
                  (((uint32_t)(ch4) & 0xFF00) << 8) |    \
                  (((uint32_t)(ch4) & 0xFF0000) >> 8) |  \
                  (((uint32_t)(ch4) & 0xFF000000) >> 24))


typedef struct _riffchunk {
   uint32_t fcc;
   uint32_t  cb;
   } RIFFCHUNK, * LPRIFFCHUNK;

typedef struct _rifflist {
   uint32_t fcc;
   uint32_t cb;
   uint32_t fccListType;
   } RIFFLIST, * LPRIFFLIST;

#endif
