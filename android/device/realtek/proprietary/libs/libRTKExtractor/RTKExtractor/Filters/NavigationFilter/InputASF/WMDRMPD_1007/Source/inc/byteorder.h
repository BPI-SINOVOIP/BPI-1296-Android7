/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/

#ifndef __BYTEORDER_H__
#define __BYTEORDER_H__

#include <drmtypes.h>

#define BITS_PER_STANDARD_BYTE 8

#define BYTES2WORD_LE( word, byte)          DRM_BYT_CopyBytes((DRM_BYTE*)&(word),0,(byte),0,SIZEOF(DRM_WORD));
#define WORD2BYTES_LE( byte, word )         DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(word),0,SIZEOF(DRM_WORD));
#define BYTES2DWORD_LE( dword, byte)        DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,SIZEOF(DRM_DWORD));
#define DWORD2BYTES_LE( byte, dword )       DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));
#define BYTES2QWORD_LE( qword, byte)        DRM_BYT_CopyBytes((DRM_BYTE*)&(qword),0,(byte),0,SIZEOF(DRM_UINT64));
#define QWORD2BYTES_LE( byte, qword )       DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(qword),0,SIZEOF(DRM_UINT64));

#define BYTES2WORD_BE( word, byte)          {DRM_BYT_CopyBytes((DRM_BYTE*)&(word),0,(byte),0,SIZEOF(DRM_WORD));FIX_ENDIAN_WORD((word));}
#define WORD2BYTES_BE( byte, word )         {FIX_ENDIAN_WORD((word));DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(word),0,SIZEOF(DRM_WORD));FIX_ENDIAN_WORD((word));}
#define BYTES2DWORD_BE( dword, byte)        {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,SIZEOF(DRM_DWORD));FIX_ENDIAN_DWORD((dword));}
#define DWORD2BYTES_BE( byte, dword )       {FIX_ENDIAN_DWORD((dword));DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));FIX_ENDIAN_DWORD((dword));}
#define BYTES2QWORD_BE( qword, byte)        {DRM_BYT_CopyBytes((DRM_BYTE*)&(qword),0,(byte),0,SIZEOF(DRM_UINT64));FIX_ENDIAN_QWORD((qword));}
#define QWORD2BYTES_BE( byte, qword )       {FIX_ENDIAN_QWORD((qword));DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(qword),0,SIZEOF(DRM_UINT64));FIX_ENDIAN_QWORD((qword));}

#define COPY_WORD(byte, index, word)        DRM_BYT_CopyBytes((byte),(index),(DRM_BYTE*)&(word),0,SIZEOF(DRM_WORD));
#define COPY_DWORD(byte, index, dword)      DRM_BYT_CopyBytes((byte),(index),(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));
#define COPY_QWORD(byte, index, qword)      DRM_BYT_CopyBytes((byte),(index),(DRM_BYTE*)&(qword),0,SIZEOF(DRM_UINT64));


#define REVERSE_BYTES_WORD(w)               DRM_BYT_ReverseBytes((DRM_BYTE*)&(w),SIZEOF(DRM_WORD))
#define REVERSE_BYTES_DWORD(dw)             DRM_BYT_ReverseBytes((DRM_BYTE*)&(dw),SIZEOF(DRM_DWORD))
#define REVERSE_BYTES_QWORD(qw)             DRM_BYT_ReverseBytes((DRM_BYTE*)&(qw),SIZEOF(DRM_UINT64))

#if TARGET_LITTLE_ENDIAN

#define BYTES_TO_WORD( word, byte )                 BYTES2WORD_LE( word, byte )
#define WORD_TO_BYTES( byte, word )                 WORD2BYTES_LE( byte, word )
#define BYTES_TO_QWORD( qword, byte )               BYTES2QWORD_LE( qword, byte )
#define QWORD_TO_BYTES( byte, qword )               QWORD2BYTES_LE( byte, qword )

#define WORD_TO_NETWORKBYTES(byte, index, word)     {DRM_WORD wTemp = (word);    REVERSE_BYTES_WORD(wTemp);   COPY_WORD((byte), (index), wTemp);}
#define DWORD_TO_NETWORKBYTES(byte, index, dword)   {DRM_DWORD dwTemp = (dword); REVERSE_BYTES_DWORD(dwTemp); COPY_DWORD((byte), (index), dwTemp);}
#define QWORD_TO_NETWORKBYTES(byte, index, qword)   {DRM_UINT64 qwTemp = (qword); REVERSE_BYTES_QWORD(qwTemp); COPY_QWORD((byte), (index), qwTemp);}
#define NETWORKBYTES_TO_WORD(word, byte, index)     {DRM_BYT_CopyBytes((DRM_BYTE*)&(word),0,(byte),(index),SIZEOF(DRM_WORD));  REVERSE_BYTES_WORD((word));}
#define NETWORKBYTES_TO_DWORD(dword, byte, index)   {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),(index),SIZEOF(DRM_DWORD)); REVERSE_BYTES_DWORD((dword));}
#define NETWORKBYTES_TO_QWORD(qword, byte, index)   {DRM_BYT_CopyBytes((DRM_BYTE*)&(qword),0,(byte),(index),SIZEOF(DRM_UINT64)); REVERSE_BYTES_QWORD((qword));}

#else /* TARGET_LITTLE_ENDIAN */

#define BYTES_TO_WORD( word, byte )                 BYTES2WORD_BE( word, byte )
#define WORD_TO_BYTES( byte, word )                 WORD2BYTES_BE( byte, word )
#define BYTES_TO_QWORD( qword, byte )               BYTES2QWORD_BE( qword, byte )
#define QWORD_TO_BYTES( byte, qword )               QWORD2BYTES_BE( byte, qword )

#define WORD_TO_NETWORKBYTES(byte, index, word)     COPY_WORD(byte, index, word)
#define DWORD_TO_NETWORKBYTES(byte, index, dword)   COPY_DWORD(byte, index, dword)
#define QWORD_TO_NETWORKBYTES(byte, index, qword)   COPY_QWORD(byte, index, qword)
#define NETWORKBYTES_TO_WORD(word, byte, index)     {DRM_BYT_CopyBytes((DRM_BYTE*)&(word),0,(byte),(index),SIZEOF(DRM_WORD)); }
#define NETWORKBYTES_TO_DWORD(dword, byte, index)   {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),(index),SIZEOF(DRM_DWORD)); }
#define NETWORKBYTES_TO_QWORD(qword, byte, index)   {DRM_BYT_CopyBytes((DRM_BYTE*)&(qword),0,(byte),(index),SIZEOF(DRM_UINT64)); }

#endif /* TARGET_LITTLE_ENDIAN */

#endif /* __BYTEORDER_H__ */
