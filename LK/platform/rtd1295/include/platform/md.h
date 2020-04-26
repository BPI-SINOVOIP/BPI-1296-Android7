#ifndef __RTK_MD_H__
#define __RTK_MD_H__

typedef enum {
	D_NORMAL = 0,
	D_2B,
	D_4B
}DATA_MODE;

typedef enum {
	M_SEQ_TO_SEQ = 0,
	M_SEQ_TO_BLK,
	M_BLK_TO_SEQ,
	M_SEQ_TO_SEQ_PITCH
}MOVE_MODE;


int md_memcpy(void* dst, void* src, unsigned int len, unsigned int kmq);
int md_memcpy_swap_endian(void* dst, void* src, unsigned int len, DATA_MODE endian_mode);

#endif
