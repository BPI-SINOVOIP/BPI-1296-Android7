
/***********************************************************************
 *
 *  md.c
 *
 *  Implementation of MD module
 *
 * 
 *
 ************************************************************************/


/************************************************************************
 *  Include files
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <debug.h>
#include <trace.h>
#include <arch/ops.h>
#include <platform/io.h>
#include <platform/setting.h>
#include <platform/md.h>
#include <platform/rbus/md_reg.h>

#define LOCAL_TRACE	0
/************************************************************************
 *  Definitions
 ************************************************************************/
#define PTR_TO_U32(ptr)			(unsigned int)((unsigned long)ptr & 0xFFFFFFFF) // Keeps only lower 32-bit
#define SS_BLK_LENGTH			(1024*1024)

#define CMD_MOV_BACKWARD		(1 << 7)

#define CMD_SEQ_TO_SEQ			(0x0 << 4)
#define CMD_SEQ_TO_BLK			(0x1 << 4)
#define CMD_BLK_TO_SEQ			(0x2 << 4)
#define CMD_SEQ_TO_SEQ_PITCH		(0x3 << 4)

#define CMD_OP_NORMAL			(0x5)
#define CMD_OP_2B			(0x7)
#define CMD_OP_4B			(0xd)

/************************************************************************
 *  Public variables
 ************************************************************************/


/************************************************************************
 *  Static variables
 ************************************************************************/


/************************************************************************
 *  Static function prototypes
 ************************************************************************/
unsigned int *allocat_md_cmd(unsigned int num_of_cmd)
{
	unsigned int *cmd;
	unsigned int num_of_bytes = 4 * sizeof(unsigned int) * num_of_cmd;

        cmd = (unsigned int*)memalign(16,num_of_bytes);
        if (!cmd) {
            printf("Can not allocate memory of MD\n");
            free(cmd);
            return NULL;
        }

	memset(cmd, 0, num_of_bytes);

	return cmd;
}

static void setup_cmd(void* dst, void* src, unsigned int len, unsigned int *cmd, DATA_MODE d_mode, MOVE_MODE m_mode)
{
	unsigned int num_of_cmd = len / SS_BLK_LENGTH;
	unsigned int ith_dst = PTR_TO_U32(dst);
	unsigned int ith_src = PTR_TO_U32(src);
	unsigned int final_cmd = 0;
	int i = 0, ith_len = 0, index = 0;

	if (len % SS_BLK_LENGTH != 0)
		num_of_cmd = num_of_cmd + 1;

	if (cmd == NULL) {
		printf("CMD buffer not allocated !!!\n");
		return;
	}

	/* check overlap to decide direction */
	if ((dst > src) && (dst < src + len))
		final_cmd |= CMD_MOV_BACKWARD;

	/* move mode */
	if (m_mode == M_SEQ_TO_SEQ)
		final_cmd |= CMD_SEQ_TO_SEQ;
	else if (m_mode == M_SEQ_TO_BLK)
		final_cmd |= CMD_SEQ_TO_BLK;
	else if (m_mode == M_BLK_TO_SEQ)
		final_cmd |= CMD_BLK_TO_SEQ;
	else if (m_mode == M_SEQ_TO_SEQ_PITCH)
		final_cmd |= M_SEQ_TO_SEQ_PITCH;
	else
		printf("Error Move Mode %u\n", m_mode);

	/* OP code */
	if (d_mode == D_NORMAL)
		final_cmd |= CMD_OP_NORMAL;
	else if (d_mode == D_2B)
		final_cmd |= CMD_OP_2B;
	else if (d_mode == D_4B)
		final_cmd |= CMD_OP_4B;
	else
		printf("Error DATA Mode %u\n", d_mode);

	LTRACEF("Final Command : 0x%08x\n", final_cmd);

	for (i = 0; i < (int)num_of_cmd; i++) {

		/* If start of dst overlaps end of src, need to copy in reverse */
		if (final_cmd & CMD_MOV_BACKWARD)
			index = num_of_cmd - i - 1;
		else
			index = i;

		ith_len = (len > SS_BLK_LENGTH ? SS_BLK_LENGTH : len);
		cmd[index*4]   = final_cmd;
		cmd[index*4+1] = ith_dst;
		cmd[index*4+2] = ith_src;
		cmd[index*4+3] = ith_len;
		ith_dst += ith_len;
		ith_src += ith_len;
		len -= ith_len;
	}

	arch_clean_cache_range((unsigned long)cmd, (unsigned long)(4 * sizeof(unsigned int)*num_of_cmd));

}

static int md_transfer(unsigned int *cmd, unsigned int num_of_cmd, unsigned int kmq)
{
	int i = 0;

	unsigned int md_cntl = MD_SMQ_CNTL + kmq * 0x60;
	unsigned int md_int_status = MD_SMQ_INT_STATUS + kmq * 0x60;
	unsigned int md_base = MD_SMQBASE + kmq * 0x60;
	unsigned int md_rdptr = MD_SMQRDPTR + kmq * 0x60;
	unsigned int md_wrptr = MD_SMQWRPTR + kmq * 0x60;
	unsigned int md_limit = MD_SMQLIMIT + kmq * 0x60;

	rtd_outl(md_cntl,0x6);
	rtd_outl(md_int_status,0x3e);
	rtd_outl(md_base , PTR_TO_U32(cmd));
	rtd_outl(md_rdptr, PTR_TO_U32(cmd));
	rtd_outl(md_wrptr, PTR_TO_U32(cmd) + num_of_cmd * sizeof(unsigned int) * 4);
	rtd_outl(md_limit,(rtd_inl(MD_SMQWRPTR) + sizeof(unsigned int) * 4));

	rtd_outl(md_cntl, 0x7);

	i = 1000000;
	while (i--) {
		// check idle
		if (rtd_inl(md_cntl) & 0x8)
			return 0;
		// check error
		if (rtd_inl(md_int_status) & 0x2) {
			printf("[ERROR] opcode error\n");
			return -1;
		}

		if (rtd_inl(md_int_status) & 0x4) {
			printf("[ERROR] length error\n");
			return -1;
		}
		spin(1);
	}
	printf("[ERROR] MD no response\n");

	return -1;
}

/************************************************************************
 *  Implementation : Public functions
 ************************************************************************/
int md_memcpy(void* dst, void* src, unsigned int len, unsigned int kmq)
{
	unsigned int *cmd;
	unsigned int num_of_cmd = len / SS_BLK_LENGTH;
	int ret = 0;

	if (len % SS_BLK_LENGTH != 0)
		num_of_cmd = num_of_cmd + 1;

	LTRACEF("%s, dst:%p, src:%p, len:0x%x\n", __func__, dst, src, len);

	if ((cmd = allocat_md_cmd(num_of_cmd)) == NULL) {

		if((unsigned long)dst < ARM_ROMCODE_SIZE)
			printf("Using MD failed.");
		else
			memcpy((void*)dst, (void*)src, len);	// use memcpy instead...
		return 0;
	}

	setup_cmd(dst, src, len, cmd, D_NORMAL, M_SEQ_TO_SEQ);
	ret = md_transfer(cmd, num_of_cmd, kmq);

	free(cmd);
	return ret;
}

int md_memcpy_swap_endian(void* dst, void* src, unsigned int len, DATA_MODE endian_mode)
{
	unsigned int *cmd;
	unsigned int num_of_cmd = len / SS_BLK_LENGTH;
	int ret = 0;

	if (len % SS_BLK_LENGTH != 0)
		num_of_cmd = num_of_cmd + 1;

	if ((cmd = allocat_md_cmd(num_of_cmd)) == NULL) {

		if((unsigned long)dst < ARM_ROMCODE_SIZE) {
			printf("Using MD failed.");
			return -1;
		}
	}

	setup_cmd(dst, src, len, cmd, endian_mode, M_SEQ_TO_SEQ);
	ret = md_transfer(cmd, num_of_cmd, 0);

	free(cmd);
	return ret;
}
