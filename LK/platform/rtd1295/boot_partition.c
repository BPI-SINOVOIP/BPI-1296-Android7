/************************************************************************
 * This file is for protection  and read/write towards boot partition 1 and 2.
 * You can setup protection on partition 1 and 2 seperated or union.
 * ==================================================================
 * e.g.
 *   for setup power on protection on boot partition 1
 *   >> boot_part 1 poweron
 *  
 *	 for setup power on protection on both partition
 *   >> boot_part 3 poweron
 *
 *   for setup permanent protection on partition 1
 *	 >> boot_part 1 permanent
 *	 
 *   for write data to boot partition 1
 *   >> boot_part 1 write DDR_ADDRESS START_BLOCK BYTE_COUNT
 *
 * ===================================================================   
 * When setup power-on and permanent protection to same partition.the
 * way of permanentness occupies priority.
 * 
 * Remember You can only setup permanent approach ONCE!!
 *
 * More details please refer to emmc specification.
 *
 **************************************************************************/

#include "boot_partition.h"


#define ISB __asm__ volatile("isb" ::: "memory")
static unsigned int emmc_rca = (1<<16);  //emmc address

static inline unsigned int __readl(volatile unsigned int *addr)
{
	return *addr;
}
	

static void dump_hex(unsigned char* pData, unsigned short Len){
	unsigned short index =0;
	for(;index < Len; index ++){
		printf("%02x",pData[index]);
	}
	printf("\n");
}



static void print_boot_usage(void){		
	printf("boot_part x read  ddr_addr,start_block,byte_count(multiple of 0x200) \n");
	printf("boot_part x write ddr_addr,dest_block, byte_count(multiple of 0x200)  \n");	
	printf("boot_part x poweron    ==>power on protection \n");
	printf("boot_part x permanent  ==>permanent protection !!!!can't reverse,cautiously use!!!!\n");
	printf("x means 1/2/3 \n");
}

static int emmc_switch_partition(unsigned int part_num){
	/*switch to partition,we should check response by MMC_SEND_STATUS	
	 * MMC_SWITCH_MODE_WRITE_BYTE <<24 |	 
	 *EXT_CSD_PART_CONF<<16 |
	 * part_num << 8 |	
	 * 1 <<0	
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

/* success:return 0
 * fail:   return 1
 */
int boot_partition_power_on_protect(int boot_part){
	
	/* detect some permanent parameter
	 */
	unsigned char* ext_csd =memalign(16, 512);
	memset(ext_csd,0x00,0x200);
	emmc_read_write_ip(MMC_SEND_EXT_CSD, 0, ext_csd, 512);
	arch_invalidate_cache_range(PTR_U32(ext_csd), 512);


	/*It means boot part 1 and 2 were permanent protected,so return*/	
	if( (ext_csd[EXT_CSD_WP_STATUS] & BOOT1_PERMANENT_PROTECTION ) && (boot_part==MMC_PART_BOOT_1)){
		printf("permanet protection on 1.can not change pattern any more\n");
		free(ext_csd);
		return 0;
	}

	if( (ext_csd[EXT_CSD_WP_STATUS] & BOOT2_PERMANENT_PROTECTION ) && (boot_part==MMC_PART_BOOT_2)){
		printf("permanet protection on 2.can not change pattern any more\n");
		free(ext_csd);
		return 0;
	}
	
	if(ext_csd[EXT_CSD_WP_STATUS] & (BOOT1_POWERON_PROTECTION | BOOT2_POWERON_PROTECTION)){
		printf("power on protection is working,replug to reset\n");
		free(ext_csd);
		return 0;
	}

	/* if certain partition was permanent protected,we should
	 * restore parameter
	 * restore parameter.
	 * ============================
	 * the situation maybe:
	 * 1.boot 1 set to permanent
	 * 2.boot 2 set to permanent
	 * 
 	 */
	char reg = 0x00;

	switch(ext_csd[EXT_CSD_WP_STATUS]){
		case BOOT1_PERMANENT_PROTECTION: 
			//restore corresponding bit
			reg |= (B_SEC_WP_SEL | B_PERM_WP_EN );
			break;
		case BOOT2_PERMANENT_PROTECTION: 
			reg |= ( B_SEC_WP_SEL | B_PERM_WP_SEL_BOOT2 | B_PERM_WP_EN );
			break;
	}

	
	//prepare register parameter	
	char status_reg = ext_csd[EXT_CSD_WP_STATUS];  //174
	if( MMC_PART_BOOT_1 == boot_part){
		if( status_reg & ( BOOT1_PERMANENT_PROTECTION)){
			printf("boot 1 has been set to permanent protection\n");
			free(ext_csd);
			return 0;
		}	
		reg |= B_SEC_WP_SEL; //individual
		reg |= B_PWR_WP_EN;
		reg &= ~(B_POW_WP_SEL_BOOT2);  //boot 1
	}else if (MMC_PART_BOOT_2 == boot_part){
		if( status_reg & ( BOOT2_PERMANENT_PROTECTION )){
			printf("boot 2 has been set to permanent protection\n");
			free(ext_csd);
			
		}
		reg |= B_SEC_WP_SEL;
		reg |= B_POW_WP_SEL_BOOT2;	//boot 2		
		reg |= B_PWR_WP_EN;
	}else if ((MMC_PART_BOOT_1 | MMC_PART_BOOT_2) == boot_part){
		if( status_reg & ( BOOT1_PERMANENT_PROTECTION | BOOT2_PERMANENT_PROTECTION  )){
			printf("permanent protection has been set.Forbid operating\n");			
			free(ext_csd);
			return 0;
		}
		
		reg &= ~(B_SEC_WP_SEL);  //boot 1 and 2
		reg |= B_PWR_WP_EN;
	}

	//issue command	
	emmc_send_cmd_get_rsp(MMC_SWITCH, 
	           			  ((MMC_SWITCH_MODE_WRITE_BYTE<<24) | (EXT_CSD_BOOT_WP<<16) | (reg<<8) ),
						  1,1);

	//check consequence
	ext_csd =memalign(16, 512);
	memset(ext_csd,0x00,0x200);
	emmc_read_write_ip(MMC_SEND_EXT_CSD, 0, ext_csd, 512);
	arch_invalidate_cache_range(PTR_U32(ext_csd), 512);

//	int index=0;
	//for(index=0;index<512;index++)
		//printf("ext_csd[%d]:0x%x\n",index,ext_csd[index]);

	if( ext_csd[EXT_CSD_WP_STATUS] & (0x01)){
		printf("boot part 1 power-on protect on\n");
	}

	if( ext_csd[EXT_CSD_WP_STATUS] & (0x04)){
		printf("boot part 2 power-on protect on\n");
	}

	free(ext_csd);
	return 0;
}

int boot_partition_permanent_protect(int boot_part){
	/* detect if power on protection is on,
	 * if yes,return 
 	 * else continue
  	 */
	unsigned char* ext_csd =memalign(16, 512);
	memset(ext_csd,0x00,0x200);
	emmc_read_write_ip(MMC_SEND_EXT_CSD, 0, ext_csd, 512);
	arch_invalidate_cache_range(PTR_U32(ext_csd), 512); 
	char reg =0x00;

		
	if(ext_csd[EXT_CSD_WP_STATUS] & (BOOT1_PERMANENT_PROTECTION)){
		printf("boot 1 is permanet protecting.can not change pattern any more \n");
		free(ext_csd);
		return 0;
	}


	if(ext_csd[EXT_CSD_WP_STATUS] & (BOOT2_PERMANENT_PROTECTION)){
		printf("boot 2 is permanet protecting.can not change pattern any more \n");
		free(ext_csd);
		return 0;
	}

	
	/* restore ext_csd[173] value, specially bit 7/3 */
	switch(ext_csd[EXT_CSD_WP_STATUS]){
		case BOOT1_PERMANENT_PROTECTION: 
			//restore corresponding bit
			reg |= (B_SEC_WP_SEL | B_PERM_WP_EN );
			break;
		case BOOT2_PERMANENT_PROTECTION: 
			reg |= ( B_SEC_WP_SEL | B_PERM_WP_EN | B_PERM_WP_SEL_BOOT2 );
			break;
	}
	
	/* continue */
	if( MMC_PART_BOOT_1 == boot_part){
		
		reg |= B_SEC_WP_SEL; //individual
		reg |= B_PERM_WP_EN; //enable permanent protection
		reg &= ~(B_PERM_WP_SEL_BOOT2);	//boot 1
		
	}else if ( MMC_PART_BOOT_2 == boot_part ){	
		reg |= B_SEC_WP_SEL; //individual		
		reg |= B_PERM_WP_EN; //enable permanent protection
		reg |= B_PERM_WP_SEL_BOOT2;  //boot 2
	
	}else if ( (MMC_PART_BOOT_1|MMC_PART_BOOT_2) == boot_part){
		
		reg &= ~B_SEC_WP_SEL; //both
		reg |= B_PERM_WP_EN; //enable permanent protection
	}

	

	//issue command	
	emmc_send_cmd_get_rsp(MMC_SWITCH,
						  ((MMC_SWITCH_MODE_WRITE_BYTE<<24) | (EXT_CSD_BOOT_WP<<16) | (reg<<8) ),
						  1,1);

	//check consequence
	ext_csd =memalign(16, 512);
	memset(ext_csd,0x00,0x200);
	emmc_read_write_ip(MMC_SEND_EXT_CSD, 0, ext_csd, 512);
	arch_invalidate_cache_range(PTR_U32(ext_csd), 512);
	
	if( ext_csd[EXT_CSD_WP_STATUS] & (BOOT1_PERMANENT_PROTECTION)){
		printf("boot 1 permanent protection on\n");
	}

	if( ext_csd[EXT_CSD_WP_STATUS] & (BOOT2_PERMANENT_PROTECTION)){
		printf("boot 2 permanent protection on\n");
	}

	free(ext_csd);
	return 0;

}


t_OperationType parameter_check(int argc,const cmd_args * argv){

	if( argc == 1 || argc ==2 ){
		print_boot_usage();
		return FAIL;
	}

	//R/W ops
	if( !strncmp(argv[2].str,"write",5) ){
		if( argc != 6 ){
			printf("parameter count error\n");
			return FAIL;
		}
		if( argv[1].u == 1 || argv[1].u == 2  ){	
			//legal
		}else{
			printf("x should be 1 or 2 \n");
			return FAIL;
		}
		if( (argv[5].u % 0x200) !=0 ){
			printf("byte conut mod 512 should be zero \n");
			return FAIL;
		}
		return WRITE;
	}

	
	if( !strncmp(argv[2].str,"read",4) ){
		if( argc != 6 ){
			printf("parameter count error\n");
			return FAIL;
		}
		if( argv[1].u == 1 || argv[1].u == 2  ){	
			//legal
		}else{
			printf("x should be 1 or 2 \n");
			return FAIL;
		}

 		if( (argv[5].u % 0x200) !=0 ){
			printf("byte conut mod 512 should be zero \n");
			return FAIL;
		}
		return READ;
	}

	//power on ops
	if( !strncmp(argv[2].str,"poweron",7) ){
		if( argv[1].u == 1 || argv[1].u == 2 || argv[1].u ==3  ){  //protect at the same time	
			//legal
		}else{
			printf("x should be 1 or 2 or 3 \n");
			return FAIL;
		}
		return POWERON;
	}

	
	if(!strncmp(argv[2].str,"permanent",9)){
		if( argv[1].u == 1 || argv[1].u == 2 || argv[1].u ==3  ){  //protect at the same time	
			//legal
		}else{
			printf("x should be 1 or 2 or 3 \n");
			return FAIL;
		}
		return PERMANENT;
	}
	return FAIL;
}


int do_bootPartition(int argc, const cmd_args *argv){

	unsigned int boot_part	= 0;	
	void * ddr_addr 		= NULL;
	unsigned int dest_block = 0;
	unsigned int count		= 0;
	t_OperationType OpsType = parameter_check(argc,argv);
	
	if( argv[1].u & MMC_PART_BOOT_1){
		boot_part |= MMC_PART_BOOT_1;
	}
	if( argv[1].u & MMC_PART_BOOT_2){
		boot_part |= MMC_PART_BOOT_2;
	}

	
	if( (OpsType == READ) || (OpsType == WRITE) ){	//only this ops should we access argv[4 && 5].or it will be buffer overflow
		ddr_addr   = (void *)strtoul(argv[3].str,NULL,16);
		dest_block = strtoul(argv[4].str,NULL,16);
		count	   = strtoul(argv[5].str,NULL,16);
	}
	
	
	if( OpsType == WRITE ){
		emmc_switch_partition(boot_part);
		if(! rtk_emmc_write(dest_block,count,ddr_addr))
			printf("data write fail\n");
		else
			printf("data write ok\n");
		emmc_switch_partition(MMC_PART_UDA);	
		
	}else if( OpsType == READ ){
		
		emmc_switch_partition(boot_part);
		if( !rtk_emmc_read(dest_block,count,ddr_addr))
			printf("data read fail\n");
		else
			printf("data read ok\n");
		emmc_switch_partition(MMC_PART_UDA);	
			
	}else if( OpsType == POWERON ){
			boot_partition_power_on_protect(boot_part);
	}else if( OpsType == PERMANENT ){
			boot_partition_permanent_protect(boot_part);
	}else{		
		printf("illegal command\n");		
	}		
	return 0;
}



STATIC_COMMAND_START
STATIC_COMMAND("boot_part", "r/w boot partition x",(console_cmd)&do_bootPartition)
STATIC_COMMAND_END(boot_part);





