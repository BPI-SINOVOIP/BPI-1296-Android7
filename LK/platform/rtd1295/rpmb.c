/**************************************************************
 * This file is for RPMB read/write operation
 * ======================================================
 *
 * e.g.
 * >> rpmb read DDR_ADDRESS start_block block_count
 * >> rpmb write DDR_ADDRESS dest_block block_count
 * ========================================================
 * one block equals 256 bytes.
 * RPMB reserves 4MB for store. 
 * But we don't guarantee the data you input whether exceed
 * the space.Maybe you should do your own check.
 *
 ************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <platform/rbus/crt_sys_reg.h>
#include <platform/rbus/sb2_reg.h>
#include <platform/io.h>
#include <platform/sb2.h>
#include <platform/setting.h>
#include <lib/heap.h>
#include <lib/bio.h>
#include <lib/console.h>
#include <arch/ops.h>
#include <arch/arm64.h>
#include <arch/arm64/mmu.h>
#include <debug.h>
#include <lib/mincrypt/sha256.h>
#include <lib/aes.h>


#include "rtkemmc.h"
#include "efuse.h"

/*******************declearation******************************/

static int emmc_rpmb_counter( volatile unsigned int * wcounter);
static int emmc_set_blockcount(int count,bool reliable);
static int emmc_rw_ip_rpmb(unsigned int cmd_idx,unsigned int blk_addr, unsigned char * dma_addr,unsigned int dma_length);

static void make_ip_des_rpmb(const unsigned char *dma_addr, unsigned int dma_length);

static int  rtk_emmc_rpmb_burn_key(void);
static int emmc_rpmb_request(struct rpmb_packet * rpmb_frame,int count ,bool reliable);

static int emmc_rpmb_response(struct rpmb_packet * rpmb_frame,unsigned short expected);

static void dump_hex(unsigned char* pData, unsigned short Len);
static int Needed_To_Burn(void);

/*******************declearation******************************/

#define KF_LEN 16
#define ISB __asm__ volatile("isb" ::: "memory")

#define RPMB_EMMC_CID_SIZE 16
#define MAX_DESCRIPTOR_NUM    (8)
#define EMMC_MAX_SCRIPT_BLK   (8)
#define EMMC_MAX_MULTI_BLK    (EMMC_MAX_SCRIPT_BLK * MAX_DESCRIPTOR_NUM)
#define wait_done(_a, _m, _v) __wait_done(U64_PTR(_a), _m, _v)
#define uswap_32(x) \
		( (((x) & 0xff000000)>>24) | \
		  (((x) & 0x00ff0000)>>8 ) | \
		  (((x) & 0x0000ff00)<<8 ) | \
		  (((x) & 0x000000ff)<<24) )

#define uswap_16(x) \
	( (((x) & 0xff00)>>8) | \
	  (((x) & 0x00ff)<<8)  )



/* Error messages */
static const char * const rpmb_err_msg[] = {
	"",
	"General failure",
	"Authentication failure",
	"Counter failure",
	"Address failure",
	"Write failure",
	"Read failure",
	"Authentication key not yet programmed",
};

/******************global variables***********************/
unsigned char ks[16]={0};
				

//unsigned char emmc_cid[16]={0};
unsigned char key_pad_to_64[64]={0};
unsigned char first_step_key[64]={0};
unsigned char second_step_key[64]={0};
unsigned char rpmb_key[32] ={0};
static unsigned int emmc_rca = (1<<16);  //emmc address
static unsigned int * rw_descriptor  = NULL;



static void __wait_done(volatile unsigned int *addr, unsigned int mask, unsigned int value){	
	int n = 0;	
	while (((*addr) & mask) != value)  {		
		if(n++ > 0x1000) {			
			printf("TIMEOUT\n");	
			printf("%s: addr=0x%08x, mask=0x%08x, value=0x%08x\n",__func__, PTR_U32(addr), mask, value);		
			break;
		}

		mdelay(1);
		sync();	
	}
}

static inline void __writel(unsigned int value, volatile unsigned int *addr){	
	*addr = value;
}
static inline unsigned int __readl(volatile unsigned int *addr){	
	return *addr;
}





static void KEY_pad_to_64bytes(unsigned char * original_key,int original_len,unsigned char * padded_key){
	memcpy(padded_key, original_key , original_len);

	//zero padding
	memset(padded_key+original_len ,0x00  , (HASH_BLOCK_SIZE - original_len));
}

static int Calculate_RPMB_key(unsigned char *rpmb_key){	
	/*some varibles*/	
	SHA256_CTX ctx;	int index=0;	
	unsigned char first_step_hash[32]={0};		

	/*get kf from otp area*/	
	GET_KF_IN_OTP(ks);
	
	
	/*make it pad to 64 bytes*/	
	KEY_pad_to_64bytes(ks,KF_LEN,key_pad_to_64);	  

	/* Create the initial vector  */	
	for(index=0; index < HASH_BLOCK_SIZE; index++) {  
		first_step_key[index] = key_pad_to_64[index] ^ 0x36;
	}

	/* Pre-pend that to the hash data */
	
	
	SHA256_init(&ctx);	
	SHA256_update(&ctx, first_step_key,HASH_BLOCK_SIZE);
	SHA256_update(&ctx, emmc_cid,RPMB_EMMC_CID_SIZE);


	/* Get the hash of the first  vector plus the data */	
	memcpy(first_step_hash,SHA256_final(&ctx),SHA256_DIGEST_SIZE);	

	/* Create the second  vector vector */   
	for(index=0; index < HASH_BLOCK_SIZE; index++) {
		second_step_key[index] = key_pad_to_64[index] ^ 0x5C;  
	}

	/* Now calculate the "outer" hash */	
	memset(&ctx,0,sizeof(struct HASH_CTX));
	SHA256_init(&ctx);
	SHA256_update(&ctx, second_step_key,HASH_BLOCK_SIZE);	
	SHA256_update(&ctx, first_step_hash,SHA256_DIGEST_SIZE);
	memcpy(	rpmb_key,SHA256_final(&ctx),SHA256_DIGEST_SIZE);   
	return 0;
}

static void Calculate_RPMB_Hmac(unsigned char *key ,unsigned char *buff,int len,unsigned char *output){	
	SHA256_CTX ctx;
	int i;	
	unsigned char k_ipad[HASH_BLOCK_SIZE];	
	unsigned char k_opad[HASH_BLOCK_SIZE];
	SHA256_init(&ctx);
	for (i = 0; i < RPMB_SZ_MAC; i++) {	
		k_ipad[i] = key[i] ^ 0x36;	
		k_opad[i] = key[i] ^ 0x5c;	
	}	

	for ( ; i < HASH_BLOCK_SIZE; i++) {		
		k_ipad[i] = 0x36;		
		k_opad[i] = 0x5c;	
	}		

	SHA256_update(&ctx, k_ipad, HASH_BLOCK_SIZE);	
	SHA256_update(&ctx, buff, len);	
	memcpy(output,SHA256_final(&ctx),SHA256_DIGEST_SIZE);	

	/* Init context for second pass */	
	SHA256_init(&ctx);	

	/* start with outer pad */
	SHA256_update(&ctx, k_opad, HASH_BLOCK_SIZE);

	/* then results of 1st hash */	
	SHA256_update(&ctx, output, RPMB_SZ_MAC);	

	

	/* finish up 2nd pass */	
	memcpy(	output,SHA256_final(&ctx),SHA256_DIGEST_SIZE);

}


static int rtk_rpmb_read_block(void * buff ,bnum_t block , uint count, bool Decrypt){
		/*rpmb key is global varible*/	 
		unsigned char rpmb_key[32]={0};	
		int ret = 1;
		int times =0;
		uint index =0;	
		unsigned char KF[16]={0};
		AES_KEY aes_key_de;

		/* judge whether exceed address range*/
		if( (block+count) > (rpmb_size<<9 )){
			printf("address out of range\n");
			return 1;
		}


		/* 1.alloc mem for rpmb packet*/ 
		struct rpmb_packet * rpmb_frame = (struct rpmb_packet*)memalign(0x200,0x200);	
		
		/* 2.calculate rpmb key*/
		Calculate_RPMB_key(rpmb_key);	

		/* 3.decrypt data by KF */
		if(Decrypt){
			GET_KF_IN_OTP(KF);
			unsigned int KF_temp[4];
			memcpy(KF_temp,KF,16);
			KF_temp[0]= uswap_32(KF_temp[0]);
			KF_temp[1]= uswap_32(KF_temp[1]);
			KF_temp[2]= uswap_32(KF_temp[2]);
			KF_temp[3]= uswap_32(KF_temp[3]);	
			memcpy(KF,KF_temp,16);
			AES_set_decrypt_key(KF, 128, &aes_key_de);
			printf("start decryption...\n");
		}

		
		/* 4.deal with each packet */	
		for(index=0;index<count;index++){		
			/*clear */		
			memset(rpmb_frame, 0x00 ,sizeof(struct rpmb_packet));	

			/*fullfill*/		
			rpmb_frame->request = uswap_16(RPMB_REQ_READ_DATA);		
			rpmb_frame->address = uswap_16(block+index);		

			/*send packet*/	
			ret = emmc_rpmb_request(rpmb_frame,1,false);
			if(ret){
				printf("rpmb request fail\n");
				break;
			}

			/*get response*/	
			//memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));	
			ret = emmc_rpmb_response(rpmb_frame,RPMB_RESP_READ_DATA);
			if(ret){
				printf("rpmb response fail\n");
				break;
			}

			//dump_hex((unsigned char *)rpmb_frame,0x200);
			
			/*check hmac*/		
			unsigned char ret_hmac[RPMB_SZ_MAC];
			Calculate_RPMB_Hmac(rpmb_key,rpmb_frame->data,284,ret_hmac);	


			/*compare whether match*/		
			if( memcmp(ret_hmac,rpmb_frame->mac,RPMB_SZ_MAC) ){		
				printf("hmac error at %d\n",index);	
				break;	
			}	

			if(Decrypt){
				unsigned char plain_text[256]={0}; 			
				memset(plain_text,0x00,256); 
				for(times=0;times<16;times++){
					AES_decrypt((const unsigned char *)(rpmb_frame->data)+times*16, plain_text+times*16, &aes_key_de);
				}				
				memcpy(buff+index*RPMB_SZ_DATA, plain_text,RPMB_SZ_DATA);
			}else{
				memcpy(buff+index*RPMB_SZ_DATA, rpmb_frame->data,RPMB_SZ_DATA);
			}
		}

	   /* 5.succeed or not a*/	
			if(index==count){		
				printf("%d blocks read success\n",index);	
	
			}else{		
				printf("read fail at block %d\n",index);			
			}

	free(rpmb_frame);
	return ret;
}


static int rtk_rpmb_write_block( void *buff , bnum_t block , uint count, bool Encrypt){
	unsigned int index=0;		
	int times =0;
	volatile unsigned int write_counter=0;	
	unsigned char rpmb_key[32]={0};		
	int ret =1;
	unsigned char KF[16]={0};
	AES_KEY aes_key_en;

	/* judge whether exceed address range
 	 * rpmb_size * 128 * 1024 / 256 ==max block count
	 */
	if( (block+count) > (rpmb_size <<9) ){
		printf("address out of range\n");
		return 1;
	}
	
	/*1.alloc mem for rpmb packet*/		
	struct rpmb_packet * rpmb_frame = (struct rpmb_packet*)memalign(0x200,0x200);

	/*2.calculate rpmb key*/	
	Calculate_RPMB_key(rpmb_key);		

	
	/*3. Encrypt data by KF */
	if(Encrypt){
		GET_KF_IN_OTP(KF);	
		unsigned int KF_temp[4];
		memcpy(KF_temp,KF,16);
		KF_temp[0]= uswap_32(KF_temp[0]);
		KF_temp[1]= uswap_32(KF_temp[1]);
		KF_temp[2]= uswap_32(KF_temp[2]);
		KF_temp[3]= uswap_32(KF_temp[3]);
		memcpy(KF,KF_temp,16);	
		AES_set_encrypt_key(KF, 128, &aes_key_en);
		printf("start encryption...\n");
	}

	/* 4.data loop*/		
	for(index=0 ; index < count;index++){		
		/*get write counter*/		
		emmc_rpmb_counter(& write_counter);			
		//printf("write counter is %d\n",write_counter);
		
		/*clear*/			
		memset(rpmb_frame,0x00,0x200);	

		/*encrypt*/
		if(Encrypt){   //encrypt by kf
			unsigned char cipher_text[256]={0};		
			memset(cipher_text,0x00,256); 
			for(times=0;times<16;times++){
				AES_encrypt((const unsigned char *)buff+times*16+index*256, cipher_text+times*16, &aes_key_en);
			}
			memcpy( rpmb_frame->data, cipher_text , RPMB_SZ_DATA );	
		}else{  //write raw data
			memcpy( rpmb_frame->data , buff+index * RPMB_SZ_DATA , RPMB_SZ_DATA ); 
		}
		
		rpmb_frame->request       = uswap_16(RPMB_REQ_WRITE_DATA);		
		rpmb_frame->address       = uswap_16(block+index);		
		rpmb_frame->block_count   = uswap_16(1);			
		rpmb_frame->write_counter = uswap_32(write_counter);		  

		/*calculate Hmac*/		
		Calculate_RPMB_Hmac(rpmb_key,rpmb_frame->data,284,rpmb_frame->mac);		


		/*send request*/		
		ret = emmc_rpmb_request(rpmb_frame,1,true);
		if(ret){
			printf("rpmb request fail\n");
			break;
		}

		/*get status*/		
		memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));		
		rpmb_frame->request = uswap_16(RPMB_REQ_STATUS);	
		ret = emmc_rpmb_request(rpmb_frame,1,false);
		if(ret){
			printf("rpmb request fail\n");
			break;
		}
		
		
		memset(rpmb_frame,0x00,sizeof(struct rpmb_packet)); 
		ret = emmc_rpmb_response(rpmb_frame,RPMB_RESP_WRITE_DATA);
		if(ret){
			printf("rpmb response fail\n");
			break;
		}
		if(uswap_16(rpmb_frame ->result)){   //non zero means failure
			break;
		}
	}

	/*5. whether succeed*/
	if(index==count){	
		printf("%d blocks write done\n",index);	
			
	}else{		
		printf("rpmb write fail at %d\n",index);		
	}	

	free(rpmb_frame);
	return ret;
}

/* change partition to RPMB or vice versa
 * ret : 1 == fail
 *       0 == success
 */
static int emmc_switch_partition(unsigned int part_num){
	/* switch to partition,we should check response by MMC_SEND_STATUS	
	 * MMC_SWITCH_MODE_WRITE_BYTE <<24 |	 
	 * EXT_CSD_PART_CONF<<16 |
	 * part_num << 8 |	
	 * (1 <<0)	
	 */
	 emmc_send_cmd_get_rsp(MMC_SWITCH, 				
	 			          ((MMC_SWITCH_MODE_WRITE_BYTE<<24)|(EXT_CSD_PART_CONF<<16) | ((part_num & PART_ACCESS_MASK)<<8)|(1<<0)) ,					
	 			           1,				
	 			           1);
	emmc_send_cmd_get_rsp(MMC_SEND_STATUS,emmc_rca,1,1);

	ISB;

	if( (cr_readl(CR_EMMC_RESP0) & SWITCH_ERROR) ==1){
		printf("switch fail\n");
		return 1;
	}
	else if( (cr_readl(CR_EMMC_RESP0) & SWITCH_ERROR) ==0){ 
		printf("switch success\n");
		return 0;
	}
	else{
		printf("can't get response\n");
		return 1;
	}
}



/*
 * set block count to 1 in the case of rpmb read/write
 * ret:
 * 1 == fail
 * 0 == success
 */
static int emmc_set_blockcount(int count,bool reliable){
	int ret =1;

	/*argument*/
	unsigned int arg = (count & 0xff);
	if(reliable)
		arg |= (1<<31);

	ret = emmc_send_cmd_get_rsp(MMC_SET_BLOCK_COUNT,arg,1,1);
	return ret;
}

static int emmc_rw_ip_rpmb(unsigned int cmd_idx,unsigned int blk_addr,
					       unsigned char * dma_addr,unsigned int dma_length){
	cr_writel(0,CR_EMMC_SWC_SEL);
	cr_writel(0,CR_EMMC_CP);
	ISB;
	sync();

	arch_clean_cache_range(PTR_U32(dma_addr), dma_length);
	
	make_ip_des_rpmb(dma_addr,dma_length);
	
	cr_writel(0x200, CR_EMMC_BYTCNT);	
	
	cr_writel(PTR_U32(rw_descriptor), CR_EMMC_DBADDR);
	cr_writel(0x0000ffff, CR_EMMC_RINTSTS);	
	cr_writel(0x0000ffff, CR_EMMC_INTMASK);	
	cr_writel(blk_addr, CR_EMMC_CMDARG);	
	ISB;	
	sync();		
	
	if (cmd_idx == MMC_READ_MULTIPLE_BLOCK){	
		//printf("inside read multiple\n");		
		cr_writel(0xa0002340 | cmd_idx, CR_EMMC_CMD);   
		
	}else if (cmd_idx ==MMC_WRITE_MULTIPLE_BLOCK){	
		//printf("inside write multiple\n");		
		cr_writel(0xa0002740 | cmd_idx, CR_EMMC_CMD);		
		
	}else if(cmd_idx ==MMC_SEND_EXT_CSD){	
		//printf("inside send ext csd\n");		
		cr_writel(0xa0000340 | cmd_idx, CR_EMMC_CMD);	
		
	}else	
		return -1;	

	ISB;
	sync();

	wait_done(CR_EMMC_RINTSTS, 0x4, 0x4); //command done	
	wait_done(CR_EMMC_RINTSTS, 0x8, 0x8);//data transfer over	

	ISB;	
	sync();		

	//read: polling dma done	
	if (cmd_idx == MMC_READ_MULTIPLE_BLOCK || cmd_idx == MMC_SEND_EXT_CSD) {	  
		wait_done(CR_EMMC_ISR, 0x2, 0x2); //dma done	  
		cr_writel(0x2, CR_EMMC_ISR);	    
		ISB;	   
		sync();	
	}	

	wait_done(CR_EMMC_STATUS, 0x200, 0x0);//data busy	
	ISB;	
	sync();	
	return check_error();
}

static int emmc_rpmb_response(struct rpmb_packet * rpmb_frame,unsigned short expected){
	int ret =1;
	//printf("inside rpmb response\n");

	/*set block count to 1*/
	ret = emmc_set_blockcount(1,false);
	if(ret){
		printf("set block count fail\n");		
		return ret;
	}

	//printf("set block count success\n");

	ret = emmc_rw_ip_rpmb(MMC_READ_MULTIPLE_BLOCK,0,(unsigned char *)rpmb_frame,0x200);
	if(ret){
		printf("rpmb read write ip fail\n");	
		return ret;
	}
	
	arch_invalidate_cache_range(PTR_U32(rpmb_frame), 0x200);

	
	/*deal with expected*/
	if( uswap_16(rpmb_frame->request) != expected){
		printf("response type error\n");
		printf("response type value:%04x\n",uswap_16(rpmb_frame->request));
		//return 1;
	}

	/* whether error occurs */
	if(uswap_16(rpmb_frame ->result)){   //non zero means failure
		/*print result*/
		printf("%s\n",rpmb_err_msg[uswap_16(rpmb_frame->result) & RESULT_ERROR_MASK] );
	}

	//dump_hex((unsigned char *)rpmb_frame,0x200);
	return 0;
}

static int emmc_rpmb_request(struct rpmb_packet * rpmb_frame,int count ,bool reliable){
	int ret =1;

	/*set block count  to 1 */
	ret = emmc_set_blockcount(count,reliable);
	if(ret){
		printf("set block count fail\n");		
		return ret;
	}
	
	ret = emmc_rw_ip_rpmb(MMC_WRITE_MULTIPLE_BLOCK,0,( unsigned char *)rpmb_frame,0x200);
	if(ret){
		printf("emmc r/w ip fail\n");
		return ret;
	}

	return 0;
}

static void make_ip_des_rpmb(const unsigned char *dma_addr, unsigned int dma_length)

{
	unsigned int *des_base = rw_descriptor;
	unsigned int blk_cnt;                                                                                                                               
	unsigned int remain_blk_cnt;

	blk_cnt        = dma_length >> 9;
  	remain_blk_cnt = blk_cnt;
	ISB;
	sync();

	arch_clean_cache_range(PTR_U32(rw_descriptor), 4 * sizeof(unsigned int) * MAX_DESCRIPTOR_NUM);

	while(remain_blk_cnt) {
		unsigned int para;
		unsigned int cur_blk_cnt;

		/* setting des0; transfer parameter  */
		para = 0x80000000 | 0x2 | 0x10;
		para |= remain_blk_cnt == blk_cnt ? 0x8 : 0;
		para |= remain_blk_cnt <= EMMC_MAX_SCRIPT_BLK ? 0x4 : 0;
		des_base[0] = para;		

		/* setting des1; buffer size in byte */                         
		cur_blk_cnt = remain_blk_cnt > EMMC_MAX_SCRIPT_BLK ? EMMC_MAX_SCRIPT_BLK : remain_blk_cnt;
		des_base[1] = (cur_blk_cnt << 9);                                                   

		/* setting des2; Physical address to DMA to/from */                             
		des_base[2] = PTR_U32(dma_addr);

		/* setting des3; next descrpter entry */                                        
		des_base[3] = PTR_U32(des_base + 4);

		dma_addr       += (cur_blk_cnt << 9);
		remain_blk_cnt -= cur_blk_cnt; 
		des_base       += 4;

		ISB;
		sync();
	}       

	arch_clean_cache_range(PTR_U32(rw_descriptor), 4 * sizeof(unsigned int)*  MAX_DESCRIPTOR_NUM);
} 


static void dump_hex(unsigned char* pData, unsigned short Len){
	unsigned short index =0;
	for(;index < Len; index ++){
		printf("%02x",pData[index]);
	}
	printf("\n");
}


/*  get write counter from emmc
 *  ret:
 *  1 == fail
 *  0 == success
 */
static int emmc_rpmb_counter(volatile unsigned int * wcounter){
	int ret =1;
	struct rpmb_packet * rpmb_frame = (struct rpmb_packet*)memalign(0x200,0x200);
	memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));

	rpmb_frame->request = uswap_16(RPMB_REQ_WCOUNTER);

	/* send request */
	ret = emmc_rpmb_request(rpmb_frame,1,false);
	if(ret){
		printf("rpmb request fail\n");
		goto FAIL;
	}

	/* get response */
	memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));
	ret = emmc_rpmb_response(rpmb_frame,RPMB_RESP_WCOUNTER);
	if(ret){
		printf("rpmb response fail\n");
		goto FAIL;
	}


	/*print response*/
	if(uswap_16(rpmb_frame ->result)){ //non zero means failure
			printf("%s\n",rpmb_err_msg[uswap_16(rpmb_frame->result) & RESULT_ERROR_MASK] );
			goto FAIL;
	}
	
	/*get counter*/
	(*wcounter) = uswap_32(rpmb_frame->write_counter);
	free(rpmb_frame);
	return 0;
FAIL:
	free(rpmb_frame);
	rpmb_frame = NULL;
	return ret;
}


int BURN_KF_IN_OTP(void){
	int result =0;
	 unsigned char on_chip_kf[KF_LEN]={0};
	 unsigned int gen_kf[4]={0};	
	 unsigned char compare_kf[KF_LEN]={0};
	
	result = Needed_To_Burn();
	if(result ==1){
		return 1;  //burned before
	}

	

	/*****gen random number****/
	 int index;
	 for(index =0;index<4;index++){
	 	genTRNG( &gen_kf[index]); //16 bytes
	 }

     /*debug to see*/
	 for(index =0;index<4;index++){
		printf("gen kf[%d]:%02x\n",index,gen_kf[index]);	
	}

    start_program_KF((unsigned char *)gen_kf);	 
	GET_KF_IN_OTP(on_chip_kf);
	if(memcmp(on_chip_kf,compare_kf,KF_LEN)!=0){
		printf("burn KF done\n");
	}else{
		printf("burn KF fail\n");
	}

	 return 0;
	
}

static int Needed_To_Burn(void){
	 unsigned char on_chip_kf[KF_LEN]={0};
	 unsigned char compare_bytes[KF_LEN] = {0};
     GET_KF_IN_OTP(on_chip_kf);

	
	if(memcmp(on_chip_kf,compare_bytes,KF_LEN)){
		printf("KF burned before\n");
		return 1; //KF burned before
	}else{
		//printf("need burned\n");
		return 0; //need burned
	}
}


/*
 * 0  ==success
 * !0 ==fail
 */
static int  rtk_emmc_rpmb_burn_key(void){	

	int ret =1;
	
	/* 1. burn kf
	 *   
	 * whether or not kf has been burned,the process will continue
	 */
	BURN_KF_IN_OTP();

	 
	/*2.invoke Calculate_RPMB_key */
	unsigned char rpmb_key[32]={0};
	Calculate_RPMB_key(rpmb_key);


	/*3.fullfill the request packet*/
	struct rpmb_packet * rpmb_frame = (struct rpmb_packet*)memalign(0x200,0x200);
	memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));

	rpmb_frame->request = uswap_16(RPMB_REQ_KEY);   //MSB write key

	/*copy key to data packet*/
	memcpy(rpmb_frame->mac, rpmb_key , RPMB_SZ_MAC);

	/*send packet*/
	ret = emmc_rpmb_request(rpmb_frame, 1,true);
	if(ret){
		printf("rpmb request fail\n");
		goto FAIL;
	}
	
	/* get response*/
	memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));		
	rpmb_frame->request = uswap_16(RPMB_REQ_STATUS);	
	ret = emmc_rpmb_request(rpmb_frame,1,false);
	if(ret){
		printf("RPMB_REQ_STATUS fail\n");
	}
	
	memset(rpmb_frame,0x00,sizeof(struct rpmb_packet));
	ret = emmc_rpmb_response(rpmb_frame,RPMB_RESP_KEY);
	if(ret){
		printf("rpmb response fail\n");
		goto FAIL;
	}

	if(uswap_16(rpmb_frame ->result)){   //non zero means failure
		printf("rpmb key burned before\n");
	}else{
		printf("burn rpmb key done\n");
	}

	return 0;

FAIL:
	free(rpmb_frame);
	return ret;
	
}

static void print_rpmb_usage(void){		
	printf("rpmb write_En  buff_addr dest_block block_count \n");
	printf("rpmb write_Raw buff_addr dest_block block_count \n");
	printf("rpmb read_De  buff_addr start_block block_count \n");
	printf("rpmb read_Raw buff_addr start_block block_count \n");
	printf("rpmb counter  -->get rpmb counter \n");		
	printf("rpmb burn     -->start burn rpmb key and OTP!!!cautiously use!!!\n");
}



int do_rtkRpmb(int argc, const cmd_args *argv){
	if(argc == 1){		
		print_rpmb_usage();		
		return 1;	
	}
	
	void * 		 buff_addr = NULL;
	unsigned int dest_block = 0;
	unsigned int count =0;
	if( !strncmp(argv[1].str,"write",5) || !strncmp(argv[1].str,"read",4)){
		if( argc <5 ){
			printf("illegal parameter\n");
			return 1;
		}
		buff_addr  = (void *)strtoul(argv[2].str,NULL,16);
		dest_block = argv[3].u;
		count      = argv[4].u;
	}

	volatile unsigned int wcounter=0;	
	rw_descriptor = memalign(8, EMMC_MAX_MULTI_BLK * sizeof(unsigned int) * 4);
	

	if(!strncmp(argv[1].str,"write_En",8)){
		
			emmc_switch_partition(MMC_PART_RPMB);
			rtk_rpmb_write_block(buff_addr,dest_block,count,true);	
			emmc_switch_partition(MMC_PART_UDA);	
			
	}else if(! strncmp(argv[1].str, "write_Raw",9)){
	
			emmc_switch_partition(MMC_PART_RPMB);
			rtk_rpmb_write_block(buff_addr,dest_block,count,false);	
			emmc_switch_partition(MMC_PART_UDA);	
			
	}else if(! strncmp(argv[1].str, "read_De",7)){
	
			emmc_switch_partition(MMC_PART_RPMB);
			rtk_rpmb_read_block(buff_addr,dest_block,count,true);	
			emmc_switch_partition(MMC_PART_UDA);	
			
	}else if(! strncmp(argv[1].str, "read_Raw",8)){
	
			emmc_switch_partition(MMC_PART_RPMB);
			rtk_rpmb_read_block(buff_addr,dest_block,count,false);	
			emmc_switch_partition(MMC_PART_UDA);	
			
	}else if (! strncmp(argv[1].str, "counter",7)){	
	
			emmc_switch_partition(MMC_PART_RPMB);		
			emmc_rpmb_counter(&wcounter);		
			printf("write counter is %d\n",wcounter);		
			emmc_switch_partition(MMC_PART_UDA);
		
	}else if ( !strncmp(argv[1].str, "burn", 4)){
		
			emmc_switch_partition(MMC_PART_RPMB);		
			rtk_emmc_rpmb_burn_key();
			emmc_switch_partition(MMC_PART_UDA);
		
	}else{		
		printf("illegal command\n");		
	}		

	free(rw_descriptor);
	rw_descriptor = NULL;
	return 0;
}

STATIC_COMMAND_START
STATIC_COMMAND("rpmb", "do some rpmb operation",(console_cmd)&do_rtkRpmb)
STATIC_COMMAND_END(rpmb);



