#ifndef __SPU_TYPES_H
#define __SPU_TYPES_H

#include <io/dvd_parse.h>

//For Sub Picture Stream Info.
typedef struct _BUTTON{
  char name[16];
  char up, down, left, right;
  unsigned int x0, x1, y0, y1;
  unsigned int cmd[2];
  char autoaction, grp;
}BUTTON;

typedef struct _STINFO{
  unsigned int x0, y0, xd, yd;
  unsigned int spts, sd;
  unsigned char numgroups, numbuttons, numpal, vobId;
  unsigned int groupmap[3][2];
  BUTTON buttons[36];
  unsigned char masterpal[4][2];
  unsigned int subSize;           // spu stream size after encoding
  unsigned char *pSubStream;      // spu stream after encoding
  unsigned int videoSize;
  unsigned char *pVideoStream;
  unsigned int audioSize;
  unsigned char *pAudioStream;
  FILE *pMenuVOB;
}STINFO;

#endif // __SPU_TYPES_H
