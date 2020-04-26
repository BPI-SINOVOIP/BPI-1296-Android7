#ifndef __MP4_BITS_H__
#define __MP4_BITS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "mp4struct.h"



#define BYTE_NUMBIT     8
#define BYTE_NUMBIT_LD  3
//#define bit2byte(a) ((a+7)/BYTE_NUMBIT)
#define bit2byte(a) ((a+7)>>BYTE_NUMBIT_LD)

//static unsigned long count = 0;

typedef struct _bitfile
{
	/* bit input */
	unsigned long bufa;
	unsigned long bufb;
	unsigned long bits_left;
	unsigned char error;
	unsigned long long bytes_used;
	
#ifdef ENABLE_CMOV	  	
  unsigned char *start;// pointer to the beginning of the buffer, used in seek func.
  unsigned char *buffer;// pointer to the buffer read.
#endif  
} bitfile;

void mp4_initbits(mp4_input_plugin_t *this, bitfile *ld);

unsigned char mp4_byte_align(mp4_input_plugin_t *this, bitfile *ld);
int64_t mp4_get_processed_bits(bitfile *ld);
void mp4_flushbits_ex(mp4_input_plugin_t *this, bitfile *ld, unsigned long bits);

#ifdef ENABLE_CMOV	

static __inline unsigned long getdword_io(mp4_input_plugin_t *this)
{
	unsigned long tmp;
	unsigned char data[4];

	this->ioPlugin->read(this->fileHandler, &data[0],1, NULL);
	this->ioPlugin->read(this->fileHandler, &data[1],1, NULL);
	this->ioPlugin->read(this->fileHandler, &data[2],1, NULL);
	this->ioPlugin->read(this->fileHandler, &data[3],1, NULL);

	tmp = ((data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3]);
//  count += 4;

	return (unsigned long)tmp;
}

static __inline unsigned long getdword_mem(unsigned char *mem)
{
  unsigned long tmp;

  ((unsigned char*)&tmp)[0] = mem[3];
  ((unsigned char*)&tmp)[1] = mem[2];
  ((unsigned char*)&tmp)[2] = mem[1];
  ((unsigned char*)&tmp)[3] = mem[0];

//  count += 4;

	return tmp;
}

static __inline unsigned long getdword(mp4_input_plugin_t *this, bitfile *ld)
{
  unsigned long tmp;
    
  if(!this->bIsHasCMOV){
    tmp = getdword_io(this);
  }else{
    tmp = getdword_mem(ld->buffer);
    ld->buffer += 4;
  }  
  
  return tmp;
}
#else

static __inline unsigned long getdword_seekable(mp4_input_plugin_t *this, bitfile *ld)
{
	unsigned long tmp;
	unsigned char data[4];

	this->ioPlugin->read(this->fileHandler, &data[0],1, NULL);
	this->ioPlugin->read(this->fileHandler, &data[1],1, NULL);
	this->ioPlugin->read(this->fileHandler, &data[2],1, NULL);
	this->ioPlugin->read(this->fileHandler, &data[3],1, NULL);

	tmp = ((data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3]);
//  count += 4;

	return (unsigned long)tmp;
}

static __inline unsigned long getdword(mp4_input_plugin_t *this, bitfile *ld)
{
    unsigned long tmp;
    unsigned char data[4];
    int i = 0;
    int ret;
    //if(this->ioInfo.bSeekable)
        //return getdword_seekable(this, ld);
    read:
    while(this->mp4stream.byte_left >=1){
        data[i] = this->mp4stream.data_pool[this->mp4stream.byte_used];
        i++;
        this->mp4stream.byte_left--;
        this->mp4stream.byte_used++;
        if(i>=4)
            break;
    }
    if(i<4){
        if(this->ioInfo.bSeekable)
            this->mp4stream.file_address = this->ioPlugin->seek(this->fileHandler, 0, SEEK_CUR);
        else
            this->mp4stream.file_address += (this->mp4stream.byte_used+this->mp4stream.byte_left);//this->ioPlugin->seek(this->fileHandler, 0, SEEK_CUR);
        DBGP("getword fileaddr: 0x%llx\n", this->mp4stream.file_address);
        ret = this->ioPlugin->read(this->fileHandler, this->mp4stream.data_pool,MP4_IO_SIZE, NULL);
        pli_flushMemory(this->mp4stream.data_pool, ret);
        DBGP("get word read_bytes = %d(%d,%s)\n", ret,errno, strerror(errno));
        if(ret<0){
            printf("MP4_GETWORD:IOPLUGIN FAIL:addr = %lld;return value = %d\n",this->mp4stream.file_address,ret);
            this->bIsSeekEnd = true;
            return -1;
        }
        this->mp4stream.byte_used = 0;
        this->mp4stream.byte_left = ret;
        goto read;
    }
    tmp = ((data[0]<<24)|(data[1]<<16)|(data[2]<<8)|data[3]);
    return (unsigned long)tmp;
}
#endif

static __inline unsigned long mp4_showbits(bitfile *ld, unsigned long bits)
{
	if (bits <= ld->bits_left){
		//return (ld->bufa >> (ld->bits_left - bits)) & bitmask[bits];
		return (ld->bufa << (32 - ld->bits_left)) >> (32 - bits);
	}

	bits -= ld->bits_left;
	//return ((ld->bufa & bitmask[ld->bits_left]) << bits) | (ld->bufb >> (32 - bits));
	return ((ld->bufa & ((1<<ld->bits_left)-1)) << bits) | (ld->bufb >> (32 - bits));
}

static __inline void mp4_flushbits(mp4_input_plugin_t *this, bitfile *ld, unsigned long bits)
{
	/* do nothing if error */
	if (ld->error != 0)
		return;

	if (bits < ld->bits_left){
		ld->bits_left -= bits;
	} else {
		mp4_flushbits_ex(this,ld, bits);
	}
}

/* return next n bits (right adjusted) */
static unsigned long mp4_getbits(mp4_input_plugin_t *this, bitfile *ld, unsigned long n)
{
	unsigned long ret;

	if (n == 0)
		return 0;

	ret = mp4_showbits(ld, n);
	mp4_flushbits(this,ld, n);

	return ret;
}

static __inline unsigned char mp4_get1bit(mp4_input_plugin_t *this, bitfile *ld)
{
	unsigned char r;

	if (ld->bits_left > 0){
		ld->bits_left--;
		r = (unsigned char)((ld->bufa >> ld->bits_left) & 1);
		return r;
	}

	// bits_left == 0 
	r = (unsigned char)mp4_getbits(this, ld, 1);

	return r;
}

#ifdef __cplusplus
}
#endif

#endif
