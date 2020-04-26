/*********************************************************************************/
/*                                                                               */
/* DVBSP_Decode_const.h                                                          */
/*                                                                               */
/* Purpose: DVB subtitling decoder constant data definitions                     */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/* Copyright (c) 2006 Realtek Semiconductor Corp. All rights reserved.           */
/*                                                                               */
/* No part of this document may be copied, reproduced,                           */
/* or transmitted in any form without the prior written                          */
/* consent of Realtek Semiconductor Corp. Hsinchu, Taiwan                        */
/*                                                                               */
/* Revision History:                                                             */
/*                                                                               */
/*      Date               By                  Description                       */
/* ============================================================================= */
/*    09/22/06         Shiyan Pan              Creation                          */
/*                                                                               */
/*                                                                               */
/*                                                                               */
/*********************************************************************************/
#ifndef _DVBSP_DECODE_CONTST_H_
#define _DVBSP_DECODE_CONTST_H_

#ifndef DVBSP_DECODE_MAIN
#define GLOB	extern
#else
#define GLOB
#endif

#ifdef DVBSP_SOFTWARE_DECODE
/* default 2-bit to 4-bit bit map table
   (ETSI EN 300 743 v1.2.1 sec 10.4 Table 11a) */
GLOB char const DVBSP_default_2_to_4_bit_map_table[4]
#ifdef DVBSP_DECODE_MAIN
    = {0, 0x77, 0x88, 0xff}
#endif
      ;


/* default 2-bit to 8-bit bit map table
   (ETSI EN 300 743 v1.2.1 sec 10.5 Table 12) */
GLOB char const DVBSP_default_2_to_8_bit_map_table[4]
#ifdef DVBSP_DECODE_MAIN
    = {0, 0x77, 0x88, 0xff}
#endif
      ;

/* default 4-bit to 8-bit bit map table
   (ETSI EN 300 743 v1.2.1 sec 10.6 Table 13) */
GLOB char const DVBSP_default_4_to_8_bit_map_table[16]
#ifdef DVBSP_DECODE_MAIN
    = {0, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff} ;
#endif
;
#endif

/* 256-entry CLUT default contents table
   (ETSI EN 300 743 v1.2.1 sec 10.1)     */
GLOB unsigned int const DVBSP_default_8_bit_CLUT[256]
#ifdef DVBSP_DECODE_MAIN
= {
    0x00000000, 0xff00003f, 0x00ff003f, 0xffff003f, 0x0000ff3f, 0xff00ff3f, 0x00ffff3f, 0xffffff3f,
    0x0000007f, 0x5500007f, 0x0055007f, 0x5555007f, 0x0000557f, 0x5500557f, 0x0055557f, 0x5555557f,
    0xaa0000ff, 0xff0000ff, 0xaa5500ff, 0xff5500ff, 0xaa0055ff, 0xff0055ff, 0xaa5555ff, 0xff5555ff,
    0xaa00007f, 0xff00007f, 0xaa55007f, 0xff55007f, 0xaa00557f, 0xff00557f, 0xaa55557f, 0xff55557f,
    0x00aa00ff, 0x55aa00ff, 0x00ff00ff, 0x55ff00ff, 0x00aa55ff, 0x55aa55ff, 0x00ff55ff, 0x55ff55ff,
    0x00aa007f, 0x55aa007f, 0x00ff007f, 0x55ff007f, 0x00aa557f, 0x55aa557f, 0x00ff557f, 0x55ff557f,
    0xaaaa00ff, 0xffaa00ff, 0xaaff00ff, 0xffff00ff, 0xaaaa55ff, 0xffaa55ff, 0xaaff55ff, 0xffff55ff,
    0xaaaa007f, 0xffaa007f, 0xaaff007f, 0xffff007f, 0xaaaa557f, 0xffaa557f, 0xaaff557f, 0xffff557f,
    0x0000aaff, 0x5500aaff, 0x0055aaff, 0x5555aaff, 0x0000ffff, 0x5500ffff, 0x0055ffff, 0x5555ffff,
    0x0000aa7f, 0x5500aa7f, 0x0055aa7f, 0x5555aa7f, 0x0000ff7f, 0x5500ff7f, 0x0055ff7f, 0x5555ff7f,
    0xaa00aaff, 0xff00aaff, 0xaa55aaff, 0xff55aaff, 0xaa00ffff, 0xff00ffff, 0xaa55ffff, 0xff55ffff,
    0xaa00aa7f, 0xff00aa7f, 0xaa55aa7f, 0xff55aa7f, 0xaa00ff7f, 0xff00ff7f, 0xaa55ff7f, 0xff55ff7f,
    0x00aaaaff, 0x55aaaaff, 0x00ffaaff, 0x55ffaaff, 0x00aaffff, 0x55aaffff, 0x00ffffff, 0x55ffffff,
    0x00aaaa7f, 0x55aaaa7f, 0x00ffaa7f, 0x55ffaa7f, 0x00aaff7f, 0x55aaff7f, 0x00ffff7f, 0x55ffff7f,
    0xaaaaaaff, 0xffaaaaff, 0xaaffaaff, 0xffffaaff, 0xaaaaffff, 0xffaaffff, 0xaaffffff, 0xffffffff,
    0xaaaaaa7f, 0xffaaaa7f, 0xaaffaa7f, 0xffffaa7f, 0xaaaaff7f, 0xffaaff7f, 0xaaffff7f, 0xffffff7f,
    0x808080ff, 0xaa8080ff, 0x80aa80ff, 0xaaaa80ff, 0x8080aaff, 0xaa80aaff, 0x80aaaaff, 0xaaaaaaff,
    0x000000ff, 0x2a0000ff, 0x002a00ff, 0x2a2a00ff, 0x00002aff, 0x2a002aff, 0x002a2aff, 0x2a2a2aff,
    0xd58080ff, 0xff8080ff, 0xd5aa80ff, 0xffaa80ff, 0xd580aaff, 0xff80aaff, 0xd5aaaaff, 0xffaaaaff,
    0x550000ff, 0x7f0000ff, 0x552a00ff, 0x7f2a00ff, 0x55002aff, 0x7f002aff, 0x552a2aff, 0x7f2a2aff,
    0x80d580ff, 0xaad580ff, 0x80ff80ff, 0xaaff80ff, 0x80d5aaff, 0xaad5aaff, 0x80ffaaff, 0xaaffaaff,
    0x005500ff, 0x2a5500ff, 0x007f00ff, 0x2a7f00ff, 0x00552aff, 0x2a552aff, 0x007f2aff, 0x2a7f2aff,
    0xd5d580ff, 0xffd580ff, 0xd5ff80ff, 0xffff80ff, 0xd5d5aaff, 0xffd5aaff, 0xd5ffaaff, 0xffffaaff,
    0x555500ff, 0x7f5500ff, 0x557f00ff, 0x7f7f00ff, 0x55552aff, 0x7f552aff, 0x557f2aff, 0x7f7f2aff,
    0x8080d5ff, 0xaa80d5ff, 0x80aad5ff, 0xaaaad5ff, 0x8080ffff, 0xaa80ffff, 0x80aaffff, 0xaaaaffff,
    0x000055ff, 0x2a0055ff, 0x002a55ff, 0x2a2a55ff, 0x00007fff, 0x2a007fff, 0x002a7fff, 0x2a2a7fff,
    0xd580d5ff, 0xff80d5ff, 0xd5aad5ff, 0xffaad5ff, 0xd580ffff, 0xff80ffff, 0xd5aaffff, 0xffaaffff,
    0x550055ff, 0x7f0055ff, 0x552a55ff, 0x7f2a55ff, 0x55007fff, 0x7f007fff, 0x552a7fff, 0x7f2a7fff,
    0x80d5d5ff, 0xaad5d5ff, 0x80ffd5ff, 0xaaffd5ff, 0x80d5ffff, 0xaad5ffff, 0x80ffffff, 0xaaffffff,
    0x005555ff, 0x2a5555ff, 0x007f55ff, 0x2a7f55ff, 0x00557fff, 0x2a557fff, 0x007f7fff, 0x2a7f7fff,
    0xd5d5d5ff, 0xffd5d5ff, 0xd5ffd5ff, 0xffffd5ff, 0xd5d5ffff, 0xffd5ffff, 0xd5ffffff, 0xffffffff,
    0x555555ff, 0x7f5555ff, 0x557f55ff, 0x7f7f55ff, 0x55557fff, 0x7f557fff, 0x557f7fff, 0x7f7f7fff
} ;
#endif
;

/* 16-entry CLUT default contents table
   (ETSI EN 300 743 v1.2.1 sec 10.2)     */
GLOB unsigned int const DVBSP_default_4_bit_CLUT[16]
#ifdef DVBSP_DECODE_MAIN
= {
    0x00000000, 0xff0000ff, 0x00ff00ff, 0xffff00ff, 0x0000ffff, 0xff00ffff, 0x00ffffff, 0xffffffff,
    0x000000ff, 0x800000ff, 0x008000ff, 0x808000ff, 0x000080ff, 0x800080ff, 0x008080ff, 0x808080ff
} ;
#endif
;

/* 4-entry CLUT default contents table
   (ETSI EN 300 743 v1.2.1 sec 10.3)     */
GLOB unsigned int const DVBSP_default_2_bit_CLUT[4]
#ifdef DVBSP_DECODE_MAIN
= {
    0x00000000, 0xffffffff, 0x000000ff, 0x808080ff
} ;
#endif
;

#endif
