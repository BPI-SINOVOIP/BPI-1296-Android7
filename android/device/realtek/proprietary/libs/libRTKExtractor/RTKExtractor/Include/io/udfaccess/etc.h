#ifndef __H_UDFACCESS_ETC_
#define __H_UDFACCESS_ETC_

// global definition

//for debug
//#define		USE_ISO9660
#define		BTC_LOADER

#define		MAX_DEV			2

#define		ECC				16*2048

// RSAT
#define		RSAT_ITEMS		992
#define		RLIMIT			0x3E00

// default Partition Starting Location
#define		PTSTLOC			0x110		

// UDF definition

#define		STD_DIR			4
#define		STD_FILE		5

// Compression ID
#define		CMP_ID8			8
// 8 bits per character
#define		CMP_ID16		16
// 16 bits per character

// File Characteristics
#define		EXISTENCE		0x01		
#define		DIRECTORY		0x02
#define		DELETED			0x04		
#define		PARENT			0x08		


// File Access Mode
#define		rdmode			0x01
#define		wrmode			0x02
#define		dirmode			0x04
#define		streammode		0x08

#define		read10mode		0x80


// Disc Status
#define		EMPTY_DISC		0x00
#define		INCOMPLETE_DISC	0x01
#define		COMPLETE_DISC	0x02

// Track Status
// DVD-R
#define		DASH_COMPLETE			0
#define		DASH_COMPLETE_INC		1
#define		DASH_BLANK				2
#define		DASH_INVISIBLE_INC		3
#define		DASH_PARTIAL			4
#define		DASH_PARTIAL_INC		5
#define		DASH_RESERVED			6
#define		DASH_RESERVED_INC		7


// +R
#define		PLUS_INVISIBLE			0
#define		PLUS_BLANK				1
#define		PLUS_COMPLETE			2
#define		PLUS_RESERVED			3


// DVD 1X speed (1000 byte/sec)
#define		DVD_ONE_X			1385	

#endif		// __H_UDFACCESS_ETC_




