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
	if( (ext_csd[EXT_CSD_WP_STATUS] & (0x02)) &&
		(ext_csd[EXT_CSD_WP_STATUS] & (0x08))){
		printf("permanet protection on 1 and 2.can not change pattern any more\n");
		free(ext_csd);
		return 0;
	}
	
	if(ext_csd[EXT_CSD_WP_STATUS] & (0x05)){
		printf("power on protection is working,replug to reset\n");
		free(ext_csd);
		return 0;
	}

	/* if certain partition was permanent protected,we should
	 * restore parameter
	 */	 
	char reg = ext_csd[EXT_CSD_BOOT_WP];

	switch(ext_csd[EXT_CSD_WP_STATUS]){
		case 0x2:  //part 1
			reg |= (1<<7);
			break;
		case 0x8: //part 2
			reg |= ( (1<<7) | (1<<3) );
			break;
	}

	
	//prepare register parameter
	reg |= ( EXT_CSD_BOOT_WP_PWR_WP_EN  );
	if( 1== boot_part){
		if( reg & ( (1<<2))){
			printf("boot 1 has been set to permanent protection\n");
			free(ext_csd);
			return 0;
		}
		reg &= ~(1<<1);  //boot 1
		
	}
	else if (2 == boot_part){
		if( reg & (1<<2)){
			if( reg & (1<<3)){
			printf("boot 2 has been set to permanent protection\n");
			free(ext_csd);
			return 0;
			}
		}
		reg |= (1<<1);  //boot 2
		
	}
	else if (3 == boot_part){
		if( reg & ( (1<<2))){
			printf("permanent protection has been set.Forbid operating\n");			
			free(ext_csd);
			return 0;
		}
		reg &= ~(1<<7);  //boot 1 and 2
		
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
		
	if(ext_csd[EXT_CSD_WP_STATUS] & (0x02)){
		printf("boot 1 is permanet protecting.can not change pattern any more \n");
		if(ext_csd[EXT_CSD_WP_STATUS] & (0x08)){
			printf("boot 2 is permanet protecting.can not change pattern any more \n");
		}
		free(ext_csd);
		return 0;
	}

	/* now we can contiune*/
	//prepare register parameter
	char reg = ext_csd[EXT_CSD_BOOT_WP];
	reg 	|= (EXT_CSD_BOOT_WP_PART_SELECT | EXT_CSD_BOOT_PERM_EN);
	if( 1== boot_part)
		reg &= ~(1<<3);  //boot 1
	else if (2 == boot_part)
		reg |= (1<<3);  //boot 2
	else if (3 == boot_part)
		reg &= ~(1<<7);  //boot 1 and 2


	//issue command	
	emmc_send_cmd_get_rsp(MMC_SWITCH,
						  ((MMC_SWITCH_MODE_WRITE_BYTE<<24) | (EXT_CSD_BOOT_WP<<16) | (reg<<8) ),
						  1,1);

	//check consequence
	ext_csd =memalign(16, 512);
	memset(ext_csd,0x00,0x200);
	emmc_read_write_ip(MMC_SEND_EXT_CSD, 0, ext_csd, 512);
	arch_invalidate_cache_range(PTR_U32(ext_csd), 512);

	//int index=0;
	//for(index=0;index<512;index++)
		//printf("ext_csd[%d]:0x%x\n",index,ext_csd[index]);
	
	if( ext_csd[EXT_CSD_WP_STATUS] & (0x02)){
		printf("boot 1 permanent protection on\n");
	}

	if( ext_csd[EXT_CSD_WP_STATUS] & (0x08)){
		printf("boot 2 permanent protection on\n");
	}

	free(ext_csd);
	return 0;

}


int do_bootPartition(int argc, const cmd_args *argv){
	if(argc == 1){		
		print_boot_usage();		
		return 1;	
	}

	
	if( argv[1].u ==1 || argv[1].u ==2 || argv[1].u ==3){
		//none
	}else{
		printf("x should be 1 or 2 or 3\n"); 	
		return 1;
	}
	
	if( !strncmp(argv[2].str,"write",5) || !strncmp(argv[2].str,"read",4)){
		if( argc <6 ){
			printf("illegal parameter number\n");
			return 1;
		}
		if( (argv[5].u % 0x200) !=0 ){
			printf("illegal byte counte\n");
			return 1;
		}
	}

	unsigned int boot_part 	=0;
	void * ddr_addr         = (void *)strtoul(argv[3].str,NULL,16);
	unsigned int dest_block = strtoul(argv[4].str,NULL,16);
	unsigned int count      = strtoul(argv[5].str,NULL,16);

	if( argv[1].u & MMC_PART_BOOT_1){
		boot_part |= MMC_PART_BOOT_1;
	}
	if( argv[1].u & MMC_PART_BOOT_2){
		boot_part |= MMC_PART_BOOT_2;
	}

//	printf("=================\n");
//	printf("dest block %d\n",dest_block);
//	printf("count %d\n",count);
//	printf("count %s\n",argv[2].str);
//	printf("=================\n");
	
	if(!strncmp(argv[2].str,"write",5)){
		
			emmc_switch_partition(boot_part);
			if(! rtk_emmc_write(dest_block,count,ddr_addr))
				printf("data write fail\n");
			else
				printf("data write ok\n");
			emmc_switch_partition(MMC_PART_UDA);	
			
	}else if(!strncmp(argv[2].str, "read",4)){
		
			emmc_switch_partition(boot_part);
			if( !rtk_emmc_read(dest_block,count,ddr_addr))
				printf("data read fail\n");
			else
				printf("data read ok\n");
			emmc_switch_partition(MMC_PART_UDA);	
			
	}else if(! strncmp(argv[2].str,"poweron",7)){
			boot_partition_power_on_protect(boot_part);
	}else if(!strncmp(argv[2].str,"permanent",9)){
			boot_partition_permanent_protect(boot_part);
	}else{		
		printf("illegal command\n");		
	}		
	return 0;
}



STATIC_COMMAND_START
STATIC_COMMAND("boot_part", "r/w boot partition x",(console_cmd)&do_bootPartition)
STATIC_COMMAND_END(boot_part);





