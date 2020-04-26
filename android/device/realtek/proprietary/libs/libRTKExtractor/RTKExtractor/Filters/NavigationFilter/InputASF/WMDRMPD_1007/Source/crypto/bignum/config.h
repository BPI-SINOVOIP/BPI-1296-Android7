/**@@@+++@@@@******************************************************************
**
** Microsoft Windows Media
** Copyright (C) Microsoft Corporation. All rights reserved.
**
***@@@---@@@@******************************************************************
*/
/****************************************************************************
*
*
*	FILENAME:	config.h
*
*	PRODUCT NAME:	CRYPTOGRAPHIC TOOLKIT
*
*	FILE STATUS:
*
*	DESCRIPTION:	Cryptographic Toolkit Configuration File
*			Machine Architecture Dependant Definitions
*		    
*
*	PUBLIC FUNCTIONS:
*
*
*	REVISION  HISTORY:
*
*
*		10 Feb 96	AK		Created
*
*
* Created for Cylink Corporation by Secant
*
****************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


/* For C++ */
#ifdef __cplusplus
extern "C" {
#endif


/*************************************
*
* Module Defines
*
*************************************/


/* Define if you have the ANSI C header files.  */
#define	STDC_HEADERS		1


/* Define processor endian */
#define	LITTLE_ENDIAN
/* define BIG_ENDIAN */


/* use ANSI or K&R style C? */
#define __USE_ANSI_C__ 		1


/* Define ORD size */
#define	ORD_16
/* #define	ORD_32 */

/* Bit Size */
#ifdef ORD_16
#define BITS_COUNT	16
#else
#define BITS_COUNT	32
#endif


/* Max digit */
#define	MAXDIGIT	(ord)(0xFFFF)


/* The number of bytes in a double.  */
#define SIZEOF_DOUBLE		8


/* The number of bytes in a int.  */
#define SIZEOF_INT		4


/* The number of bytes in a long.  */
#define SIZEOF_LONG		4


/* The number of bytes in a short.  */
#define SIZEOF_SHORT		2


/***************************************
*
* Perform checks
*
***************************************/

/* Check endian */
#if  !defined(LITTLE_ENDIAN) && !defined(BIG_ENDIAN)
#error Not defined CPU type LITTLE or BIG ENDIAN.
#endif
#if  defined(LITTLE_ENDIAN) && defined(BIG_ENDIAN)
#error Use only one define CPU type LITTLE or BIG ENDIAN.
#endif


/* Check ord size */
#if !defined(ORD_16) && !defined(ORD_32)
#error Not defined ORD size for the machine.
#endif
#if defined(ORD_16) && defined(ORD_32)
#error Machine size cannot be both 16 and 32.
#endif


#ifdef __cplusplus
}
#endif


#endif /* CONFIG_H */
