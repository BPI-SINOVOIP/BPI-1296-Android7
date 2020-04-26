#ifndef _UDF_IF_H_
#define _UDF_IF_H_

//#ifndef __linux
//#define		off64_t			_int64
//#endif

#include <io/udfaccess/etc.h>

#if defined(__ANDROID__)
typedef off64_t __off64_t;

#else

#ifdef __MIPSEL__
#define		off64_t			__off64_t
#endif

#ifndef off64_t
#define off64_t __off64_t
#endif

#endif

//#define		HANDLE			int
typedef int HANDLE;

#define		UB		unsigned char
#define		UI		unsigned int

#define		MAX_SHARE		16

typedef struct node node;
typedef struct dinfo dinfo;
typedef struct dappif dappif;
typedef struct fobject fobject;

struct node{
    UB *id;				// 16 bits per character
	UB id_len;			// id length
	UI fELoc;			// file entry location
	UI loc, len;
	UI off9660, len9660;// len9660 = Data Length in Directory Record
	UB fType;
	UI fLkCnt;
	UI perm;			// permission
	char CGMS, pSysType;
	time_t ltime;
	UB share;			// share flag=1: mean to write File Entry
	node *sharePt;
	UI rdCnt, wrCnt;	// fobject read/ write count
	node *lPt, *rPt;
};

struct fobject{
	UB mode;		// file access mode( bit0:read, bit1:write, bit2: directory, bit3:streaming read/write)
	UI wbufLen;		// write buffer current length
	UB *wbuf;		// write buffer pointer
	UB devHandle;

	off64_t fptr;	// file read/ write position pointer	
	node *n;
};

struct rsat_st{
	int RSAT;					// 0: no RSAT; 1: read from disc RSAT
	int RSAT_COUNT;
	int PREV_RSAT;				// the location of the first instance of the previous version of RSAT
	int CURRENT_RSAT;			// the location of the first instance of the current version of RSA
	int END_DATA;
	int rsat_item[RSAT_ITEMS];	// for DVD+R and DVD-R
};

struct dinfo{
	// HW variables
	char blkName[128];
	int fd;
	UB  appendable;		// 1: Appendable; 0: Read only
    	int profile;		// bit 16: blank
	UI  capacity;
	int is8cm;
	int doOPC;
	int l0_init;		// indicate L0 data area is changeable or not
				// 0: changeable, 1: non-changeable 
	UI  l0_capacity;	// Layer 0 data area capacity

	int rmwFlag;		// Read Modify Write flag

	UI  NWA;
	int order;			// bit 0: Format, bit 1: Setup, bit 2:Reserve, bit 3: dirty flag,
						// bit 4: Close Disc(DVD-R/DVD+R), bit 5: Write & Verify

	UB regionType;		// Type Code, (00b, 01b, 10b, 11b, 100b) = (NONE, SET, LAST CHANCE, PERM, ALL REGION)
	UB regionVendor;	// # of Vendor Resets Available
	UB regionMask;		// Region Mask
	UB regionChange;	// # of User Controlled Changes Available


	int	nxtTrack;
	char dType;
	char dStatus;
	UB rb[2048];

	struct rsat_st r_st;

	//double sFactor;				// speed factor set by user; 0.0 = default value
	//int velocity;				// current write speed
	int bufSize;

	// file system
	UI	fSys;					// bit 0: ISO9660, bit 1: UDF1.1

	node root;
	UI maxVol, minVol;


	// ISO9660 volume information
	UB	*volID9660;
	UI	volSSize;			// Volume Space Size
	UI	pathLTbl;			// Path Table L location
	UI	pathMTbl;			// Path Table M location
	UI	pTblSize;			// Path Table Size
	UI	rootLoc9660;		// root location
	UI	rootLen9660;		// Data Length of root directory 
	UI	off9660;
	node **pathPtr;


	

	// UDF
	UB volID[32];
	UI noFile, noDir;
	UI lVolLoc;
	UI pStLoc, pLen;
	UI mainVolExtLoc, resVolExtLoc;
	UI iSeqExtLoc;
	UI rdVersion, wrVersion;


	UI shareArray[MAX_SHARE];
	node *shareNode[MAX_SHARE];

	// common variables
	time_t globalTime;
	node *curPt;
};

struct dappif{
	// HW variables
	int trayOpen;		//
	int mediaPresent;	//
	int	profile;		// disc type
	UI	capacity;		// disc capacity

	char discSt;		// 0: Empty Disc, 1: Appendable, 2: Complete Disc
	int	nxtTrack;
	char dType;

	int fSys;			// bit 0: ISO9660, bit 1: UDF
	UI rdVersion, wrVersion;

	struct rsat_st r_st;

	UI mainVolExtLoc, resVolExtLoc;


	//unsigned char vender[20];

};

// USER MACRO
#define		SETRMWFLAG(X)		((X)->rmwFlag=1)
#define		CLRRMWFLAG(X)		((X)->rmwFlag=0)
#define		FILE_LEN(X)			((X)->n->len)
#define		FILE_LOC(X)			((X)->n->loc)
#define		FILE_CNT(X)			(((X)->n->rdCnt)+((X)->n->wrCnt))
#define		FILE_CGMS(X)		((X)->n->CGMS)


// Order 
#define		FORMAT				0x01
#define		SETUP				0x02
#define		RESERVE				0x04
#define		DIRTY				0x08
#define		CLOSE				0x10
#define		BLANK				0x20
#define		OPC				0x40

// USER MACRO
#define		DVD_SET_DIRTY(X)	X|=DIRTY
#define		DVD_CLR_DIRTY(X)	X&=~DIRTY


// Format parameters for dvdDiscFormat
#define		STOP_DE_ICING		0x00
#define		COMPLETED			0x01	
#define		BACKGROUND_RUN		0x02
#define		APPEND_RW_FORMAT	0x03


// Vender ID

#define		RICHO			0
#define		ASUS			1		
#define		UNKNOWN			-1
	



// dvdno value

#define		DOK			0		// OK, no error
#define		DOPEN		1
#define		DFMT		2
#define		DSTBG		3
#define		DTRAY		4
#define		DNODEV		5
#define		DRPTOP		6
#define		DINIT		7
#define		DBINARY		8
#define		DSETUP		9
#define		DNODIR		10
#define		DFILE		11
#define		DPARM		12
#define		DDEL		13
#define		DINS		14
#define		DMEM		15
#define		DLEN		16
#define		DWRITE		17
#define		DREAD		18
#define		DSEEK		19
#define		DWMEDIA		20
#define		DSHARE		21
#define		DNOOPEN		22
#define		DNOFILE		23
#define		DTYPE		24
#define		D2W			25
#define		DRANDOM		26


// ide transfer mode
#define		MDMA0		1
#define		MDMA1		2
#define		MDMA2		3
#define		UDMA0		4
#define		UDMA1		5
#define		UDMA2		6
#define		UDMA3		7
#define		UDMA4		8
#define		UDMA5		9
#define		UDMA6		10


#endif		// _UDF_IF_H_
