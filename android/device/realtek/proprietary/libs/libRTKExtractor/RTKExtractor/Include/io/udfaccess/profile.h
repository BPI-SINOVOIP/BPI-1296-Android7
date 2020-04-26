#ifndef __H_UDFACCESS_PROFILE_
#define __H_UDFACCESS_PROFILE_

// Profile List(refer to GET CONFIGURATION Command)

#define		CD_ROM		0x0008
#define		CD_R		0x0009
#define		CD_RW		0x000A

#define		DVD_ROM		0x0010
#define		DVD_R		0x0011
#define		DVD_RAM		0x0012
#define		DVD_RW_OW	0x0013
#define		DVD_RW_SEQ	0x0014
#define		DVD_R_DL_SEQ	0x0015
#define		DVD_R_DL_JMP	0x0016
#define		PLUS_RW		0x001A
#define		PLUS_R		0x001B
#define		PLUS_R_DL	0x002B

#define		BD_ROM		0x0040
#define		BD_R_SRM	0x0041
#define		BD_R_RRM	0x0042
#define		BD_RE		0x0043

// Disc Type(refer to READ DISC INFORMATION Command)

#define		_CD_ROM		0x00
#define		_CD_I		0x10
#define		_CD_ROM_XA	0x20
#define		_UNDEFINED	-1

#endif

