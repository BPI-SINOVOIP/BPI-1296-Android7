#ifndef __C_LOWER_LEVEL_TRANS_H__
#define __C_LOWER_LEVEL_TRANS_H__

#include <assert.h>

#define VCD_PREGAP_SECTORS 150  

typedef struct msf_s 
{
	unsigned char mm;
	unsigned char ss;
	unsigned char ff;
    
} __attribute__((packed)) msf_t;
  
static inline unsigned char ToBCD(unsigned char n)
{
	assert(n < 100);

	return ((n/10)<<4) | (n%10);
}

static inline unsigned char FromBCD(unsigned char p)
{
  return (0xf & p) + (10*(p >> 4));
}

static inline unsigned long LBAtoLSN(unsigned long lba)
{
  return lba - VCD_PREGAP_SECTORS; 
}

static inline void LBAtoMSF(unsigned long lba, msf_t* msf, int sectorPerSec)
{
	assert(msf);
	
	msf->mm = ToBCD(lba / (60 * sectorPerSec));
	msf->ss = ToBCD((lba / sectorPerSec) % 60);
	msf->ff = ToBCD(lba % 75);

}

static inline unsigned long LSNtoLBA(unsigned long lsn, unsigned char TimeConcern)
{

	if(TimeConcern)
		return lsn;
	else
		return lsn + VCD_PREGAP_SECTORS; 
}

static inline void LSNtoMSF(unsigned long lsn, msf_t* msf, unsigned char TimeConcern, int sectorPerSec)
{
	assert(msf);
	
	LBAtoMSF( LSNtoLBA(lsn, TimeConcern), msf, sectorPerSec);
}


static inline unsigned long MSFtoLBA(const msf_t* msf, int sectorPerSec)
{
	assert(msf);
  
	unsigned long lba = FromBCD(msf->mm) * 60;
	
	lba += FromBCD(msf->ss);
	lba *= sectorPerSec;
  
	lba += FromBCD(msf->ff);

	return lba;
}

static inline unsigned long MSFtoLSN(const msf_t* msf, int sectorPerSec)
{
	return LBAtoLSN( MSFtoLBA(msf, sectorPerSec));
}

//////////////////////////////////////////////////////////////////////

static inline unsigned int LenToBlocks(unsigned int len, int blocksize)//_vcd_len2blocks
{
	unsigned int blocks = len / blocksize;
	if (len % blocksize)
		blocks++;

	return blocks;
}

/* swap frome little endian to big endian */
static inline void TwoBytesSwap(const unsigned char* pBuf, int size)
{	
    			
    /* every two bytes swap, 
       but we can speed up by doing 2 2-byte swaps one time in 32bit CPU*/

    unsigned long* pTempBuf = (unsigned long*)pBuf;
    unsigned long value; 
    
    for(int i = 0; i < size; i+=4)
    {
    	value = *pTempBuf;
    	*pTempBuf++ = ((value & (unsigned long) 0xff00ff00UL) >> 8) | ((value & (unsigned long) 0x00ff00ffUL) << 8);					    					
    }
    assert((unsigned char*)pTempBuf == (pBuf+size));
    			
}
	
#endif	//__C_LOWER_LEVEL_TRANS_H__
