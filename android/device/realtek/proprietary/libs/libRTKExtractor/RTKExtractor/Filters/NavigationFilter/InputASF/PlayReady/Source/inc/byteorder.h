/**@@@+++@@@@******************************************************************
**
** Microsoft PlayReady
** Copyright (c) Microsoft Corporation. All rights reserved.
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

#define COPY_FROMBUFFER(to, from, index, size, buffersize)  {ChkOverflow((index)+(size),(index));ChkBOOL((index)+(size)<=(buffersize),DRM_E_BUFFERTOOSMALL);DRM_BYT_CopyBytes((DRM_BYTE*)&(to),0,(from),(index),(size));(index)+=(size);}

#define REVERSE_BYTES_WORD(w)               DRM_BYT_ReverseBytes((DRM_BYTE*)&(w),SIZEOF(DRM_WORD))
#define REVERSE_BYTES_DWORD(dw)             DRM_BYT_ReverseBytes((DRM_BYTE*)&(dw),SIZEOF(DRM_DWORD))
#define REVERSE_BYTES_QWORD(qw)             DRM_BYT_ReverseBytes((DRM_BYTE*)&(qw),SIZEOF(DRM_UINT64))

#define BYTE2LITTLEENDIAN(dword, byte)     DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,SIZEOF(DRM_DWORD));
#define DWORD2LITTLEENDIAN( byte, dword )  DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));

#define BYTE2BIGENDIAN(dword, byte)        {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),0,SIZEOF(DRM_DWORD));FIX_ENDIAN_DWORD((dword));}
#define DWORD2BIGENDIAN(byte, dword)       {FIX_ENDIAN_DWORD((dword));DRM_BYT_CopyBytes((byte),0,(DRM_BYTE*)&(dword),0,SIZEOF(DRM_DWORD));FIX_ENDIAN_DWORD((dword));}

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

#define NETWORKBYTES_FROMBUFFER(to, from, index, size, buffersize)       {COPY_FROMBUFFER(to,from,index,size,buffersize);DRM_BYT_ReverseBytes((DRM_BYTE*)&(to),(size));}
#define NETWORKBYTES_FROMBUFFER_TO_WORD(word, byte, index, buffersize)   NETWORKBYTES_FROMBUFFER(word,byte,index,SIZEOF(DRM_WORD),buffersize)
#define NETWORKBYTES_FROMBUFFER_TO_DWORD(dword, byte, index, buffersize) NETWORKBYTES_FROMBUFFER(dword, byte,index,SIZEOF(DRM_DWORD),buffersize)

#define BYTES_TO_DWORD BYTE2LITTLEENDIAN
#define DWORD_TO_BYTES DWORD2LITTLEENDIAN
#define FIX_ENDIAN_WORD(w)
#define FIX_ENDIAN_DWORD(dw)
#define FIX_ENDIAN_QWORD(qw)

/* Copies WORD written to buffer in little endian format into WORD variable */
#define LITTLEENDIAN_BYTES_TO_WORD(word, byte, index)     {DRM_BYT_CopyBytes((DRM_BYTE*)&(word),0,(byte),(index),SIZEOF(DRM_WORD)); }

/* Copies DWORD written to buffer in little endian format into DWORD variable */
#define LITTLEENDIAN_BYTES_TO_DWORD(dword, byte, index)   {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),(index),SIZEOF(DRM_DWORD)); }

/* Copies DWORD variable into buffer with DWORD in little endian format*/
#define DWORD_TO_LITTLEENDIAN_BYTES(byte, index, dword)   {DRM_DWORD dwTemp = (dword); COPY_DWORD((byte), (index), dwTemp);}

/* Copies WORD variable into buffer with WORD in little endian format*/
#define WORD_TO_LITTLEENDIAN_BYTES(byte, index, word)   {DRM_WORD wTemp = (word); COPY_WORD((byte), (index), wTemp);}

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

#define NETWORKBYTES_FROMBUFFER(to, from, index, size, buffersize)       COPY_FROMBUFFER(to,from,index,size,buffersize)
#define NETWORKBYTES_FROMBUFFER_TO_WORD(word, byte, index, buffersize)   NETWORKBYTES_FROMBUFFER(word,byte,index,SIZEOF(DRM_WORD),buffersize)
#define NETWORKBYTES_FROMBUFFER_TO_DWORD(dword, byte, index, buffersize) NETWORKBYTES_FROMBUFFER(dword,byte,index,SIZEOF(DRM_DWORD),buffersize)

#define BYTES_TO_DWORD BYTE2BIGENDIAN
#define DWORD_TO_BYTES DWORD2BIGENDIAN

#if __TMS320C55X__
/* Work around a uniqueness in the TI compiler */
#define FIX_ENDIAN_WORD(w)   {DRM_DWORD __dw = SIZEOF(DRM_WORD);DRM_BYT_ReverseBytes((DRM_BYTE*)&(w), __dw);}
#define FIX_ENDIAN_DWORD(dw) {DRM_DWORD __dw = SIZEOF(DRM_DWORD);DRM_BYT_ReverseBytes((DRM_BYTE*)&(dw),__dw);}
#define FIX_ENDIAN_QWORD(qw) {DRM_DWORD __dw = SIZEOF(DRM_UINT64);DRM_BYT_ReverseBytes((DRM_BYTE*)&(qw),__dw);NATIVE64_TO_NONNATIVE64((qw));}
#else
#define FIX_ENDIAN_WORD(w)   DRM_BYT_ReverseBytes((DRM_BYTE*)&(w), SIZEOF(DRM_WORD))
#define FIX_ENDIAN_DWORD(dw) DRM_BYT_ReverseBytes((DRM_BYTE*)&(dw),SIZEOF(DRM_DWORD))
#define FIX_ENDIAN_QWORD(qw) {DRM_BYT_ReverseBytes((DRM_BYTE*)&(qw),SIZEOF(DRM_UINT64));NATIVE64_TO_NONNATIVE64((qw));}
#endif


/* Copies WORD written to buffer in little endian format into WORD variable */
#define LITTLEENDIAN_BYTES_TO_WORD(word, byte, index)     {DRM_BYT_CopyBytes((DRM_BYTE*)&(word),0,(byte),(index),SIZEOF(DRM_WORD)); REVERSE_BYTES_WORD((word));}

/* Copies DWORD written to buffer in little endian format into DWORD variable */
#define LITTLEENDIAN_BYTES_TO_DWORD(dword, byte, index)   {DRM_BYT_CopyBytes((DRM_BYTE*)&(dword),0,(byte),(index),SIZEOF(DRM_DWORD)); REVERSE_BYTES_DWORD((dword));}

/* Copies DWORD variable into buffer with DWORD in little endian format*/
#define DWORD_TO_LITTLEENDIAN_BYTES(byte, index, dword)   {DRM_DWORD dwTemp = (dword); REVERSE_BYTES_DWORD(dwTemp); COPY_DWORD((byte), (index), dwTemp);}

/* Copies WORD variable into buffer with WORD in little endian format*/
#define WORD_TO_LITTLEENDIAN_BYTES(byte, index, word)   {DRM_WORD wTemp = (word); REVERSE_BYTES_WORD(wTemp); COPY_WORD((byte), (index), wTemp);}

#endif /* TARGET_LITTLE_ENDIAN */

#endif /* __BYTEORDER_H__ */
