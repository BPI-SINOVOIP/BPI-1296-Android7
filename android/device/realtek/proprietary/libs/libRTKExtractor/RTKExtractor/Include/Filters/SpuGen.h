#ifndef __SPUGEN_H_
#define __SPUGEN_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <Filters/spu_types.h>
#include <Platform_Lib/FontRenderer/render.h>

#define SUB_MAX_TEXT            16
#define SUB_BUFFER_MAX          53220
#define SUB_BUFFER_HEADROOM     1024


void do_rle(int count, int color,
            unsigned char *sub, unsigned int *subo, int *remainbit);
            
void store_nibble(int val,
                  unsigned char *sub, unsigned int *sudo, int *remainbit);

void store_bits(unsigned int val, int bits,
                unsigned char *sub, unsigned int *subo, int *remainbit);
                
void store_align(unsigned char *sub, unsigned int *subo, int *remainbit);

void set_header(int field, unsigned char *sub, unsigned int *subo);
void store_1(int val, unsigned char *sub, unsigned int *subo, int *remainbit);
void store_2(int val, unsigned char *sub, unsigned int *subo, int *remainbit);
void store_trinibble(int val, unsigned char *sub, unsigned int *subo, int *remainbit);

//=============================================================================//
// The coordinates is counted from (1,1) 
void dvd_encode_row(int y,int xd, unsigned char *icptr,
                     unsigned char *sub, unsigned int *subo, int *remainbit);
int SpuEncode(STINFO *cursti, unsigned char *pixelImg);
void DrawLine(unsigned char linePixel, unsigned char *pixelImg, int pixelWidth,
              int x0, int y0, int lineWidth);

//void DrawUpButtonSPU(unsigned char *pixelImg, int pixelWidth, enum MENU_STYLE menuStyle); 
//void DrawDownButtonSPU(unsigned char *pixelImg, int pixelWidth, enum MENU_STYLE menuStyle);
//void DrawExitButtonSPU(unsigned char *pixelImg, FontRenderer *Render, 
//                       int pixelWidth, bool hasThumbnail, enum MENU_STYLE menuStyle);

void DrawTitleLengthInfo(unsigned char *pixelImg, FontRenderer *Render, 
                         char *titleLength, int x0, int y0);
void DrawTitleInfo(int titlenum, unsigned char *pixelImg, 
                   FontRenderer *Render, char *titleName, 
                   int y0, bool hasThumbnail, int titlesPerPage, int x0);
//int DrawTitleSPU(unsigned char *pixelImg, FontRenderer *Render, 
//                 int titlenum, int titlesPerPage, 
//                 char *titleName, bool hasThumbnail, 
//                 enum MENU_STYLE menuStyle);
void DrawThumbnailSPU(unsigned char *pixelImg, 
                      int rectHeight, int rectWidth, int rectX, int rectY);

//void DrawPageNumber(unsigned char *pixelImg, FontRenderer *Render, 
//                    int pageNum, int numOfPages, bool hasThumbnail);
void DrawPageIndex(unsigned char *pixelImg, int pageNum, int numOfPages);

int CreateSPUPixelMap(unsigned char *pixelImg);

int SpuGen(STINFO *cursti, unsigned char *pixelImg);

#endif // __SPUGEN_H_

