#ifndef RVSD_RINGBUFFER_H
#define RVSD_RINGBUFFER_H

////////////
// Define //
////////////
#define RING_BUF_UNDERFLOW 0x80000000
#define LE_PLATFORM 1   // little endian platform

///////////
// Macro //
///////////
#define DBG_RINGBUF(a) ALOGD("[¡¹¡¹ b %x l %x w %x r %x @ %s %d]\n", a->base, a->limit, a->wp, a->rp, __FUNCTION__, __LINE__)

////////////////////
// Data Structure //
////////////////////
typedef struct  {
    unsigned long base;
    unsigned long limit;
    unsigned long wp;
    unsigned long rp;
} RVSD_RINGBUF;
typedef struct  {
    unsigned long base;
    unsigned long limit;
    unsigned long wp;
    unsigned long rp;
    unsigned long bit_offset;   // the bit offset of the first 4 bytes which ReadPtr points to. Smaller than 32.
} RVSD_EXTRACT_RINGBUF;

class RVSD_EXTRACT_BITS
{
public:
    RVSD_EXTRACT_BITS(unsigned long base, unsigned long limit, unsigned long wp, unsigned long rp, unsigned long bit_offset);
    ~RVSD_EXTRACT_BITS(void);
    RVSD_EXTRACT_RINGBUF nRing;
    unsigned long ExtractBits(unsigned long nNumBits);
    unsigned long PeekBits(unsigned long nNumBits);
    unsigned long UpdateRp();
    void IgnoreBytes(unsigned long nNumBytes);
    void SetRp(unsigned long rp, unsigned long bit_offset);
    void GetRp(unsigned long *pRp, unsigned long *pBitOffset);
};

////////////////////////
// Function Prototype //
////////////////////////
unsigned long ring_add(unsigned long ring_base, unsigned long ring_limit, unsigned long ptr, unsigned int bytes);
unsigned long ring_minus(unsigned long ring_base,unsigned long ring_limit,unsigned long ptr,unsigned long bytes);
void ring_read(unsigned long ring_base, unsigned long ring_limit, unsigned long ptr, unsigned int bytes, char* buf);
unsigned long ring_valid_data(unsigned long ring_base, unsigned long ring_limit, unsigned long ring_rp, unsigned long ring_wp);
unsigned long ring_memcpy2_buf(char* buf,unsigned long base, unsigned long limit, unsigned long ptr, unsigned long size);
unsigned long buf_memcpy2_ring(unsigned long base, unsigned long limit, unsigned long ptr, char* buf, unsigned long size);
unsigned long valid_free_size(unsigned long base, unsigned long limit, unsigned long rp, unsigned long wp);
void ring1_to_ring2_general(RVSD_RINGBUF* ring1, RVSD_RINGBUF* ring2, long size);

#endif  // #ifndef RVSD_RINGBUFFER_H


