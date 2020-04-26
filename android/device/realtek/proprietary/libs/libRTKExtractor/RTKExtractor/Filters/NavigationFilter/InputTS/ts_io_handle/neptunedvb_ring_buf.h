#ifndef __NEPTUNEDVB_RING_BUF_H__
#define __NEPTUNEDVB_RING_BUF_H__

#include "Neptune_TP_control.h"


#define NEPTUNEDVB_NUM_OF_READ_PTR	4

typedef struct _neptunedvb_ring_t
{
	
	
	unsigned long   ul_base;				// physical address
	unsigned long	ul_limit;
	unsigned long	ul_size;				// if size = 0, this ring buffer entry is invalid
	unsigned long 	ul_virReadPtr[NEPTUNEDVB_NUM_OF_READ_PTR];		// virtual read pointer
	unsigned int 	ui_bReadPtrActive[NEPTUNEDVB_NUM_OF_READ_PTR];	/* 1 : the corresponding read pointer is valid; */
	unsigned char*  puc_CachedVirAddr;	
	long            l_addrDiff;			// value = PhysicalAddress - VirtualAddress
	//unsigned char	uc_dataPiece[NEPTUNETP_MAX_TS_PACKET_LEN];	// an internal memory to handle wraparound

	
}neptunedvb_ring_t;

neptunedvb_ring_t*	neptunedvb_ring_create(unsigned long ul_size);
int 				neptunedvb_ring_destroy(neptunedvb_ring_t* p_ring);
int 				neptunedvb_ring_get_free_size(unsigned long ul_totalSize, unsigned long ul_writePtr, unsigned long ul_readPtr);
unsigned long 		neptunedvb_ring_get_vir_addr(neptunedvb_ring_t* p_ring, unsigned long	ul_phyAddr);
unsigned long 		neptunedvb_ring_get_phy_addr(neptunedvb_ring_t* p_ring, unsigned long ul_virAddr);
unsigned long 		neptunedvb_ring_get_vir_write_ptr(neptunedvb_ring_t* p_ring, TP_CONTEXT* pTPInterface, char bufIndex);
unsigned long 		neptunedvb_ring_get_vir_read_ptr(neptunedvb_ring_t* p_ring, int readIndex);
unsigned long 		neptunedvb_ring_get_vir_base_ptr(neptunedvb_ring_t* p_ring);
unsigned long 		neptunedvb_ring_get_vir_limit_ptr(neptunedvb_ring_t* p_ring);
void 				neptunedvb_ring_flush(neptunedvb_ring_t* p_ring, TP_CONTEXT* pTPInterface, char bufIndex);
void 				neptunedvb_ring_set_read_ptr(neptunedvb_ring_t* p_ring, TP_CONTEXT* pTPInterface, char bufIndex, unsigned long ul_virAddr, int readIndex);

#endif
