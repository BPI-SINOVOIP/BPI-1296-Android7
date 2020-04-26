#include "dvrboot_inc/util.h"
//#include "dvrboot_inc/string.h"
#include "otp_reg.h"
#include "include/dvrboot_inc/sb2_reg.h"
#include "dvrboot_inc/io.h"
#include "include/rpmb.h"

extern unsigned char vm_adv_rsa_pub_key[];
extern unsigned char vm_adv_rsa_pub_key_end;
extern unsigned char vm_adv_rsa_pub_final[];
extern unsigned char vm_adv_rsa_pub_final_end;
extern unsigned char aes_key[];
extern unsigned char aes_key_end;
extern unsigned char aes_key1[];
extern unsigned char aes_key1_end;
extern unsigned char aes_key2[];
extern unsigned char aes_key2_end;
extern unsigned char aes_key3[];
extern unsigned char aes_key3_end;
extern unsigned char aes_seed[];
extern unsigned char aes_seed_end;
extern unsigned char cust_data[];
extern unsigned char cust_data_end;
extern unsigned char chip_id[];
extern unsigned char chip_id_end;
extern unsigned char jtag_pwd[];
extern unsigned char jtag_pwd_end;
extern unsigned char market_id[];
extern unsigned char market_id_end;
extern unsigned char add_data1[];
extern unsigned char add_data1_end;
extern unsigned char add_data2[];
extern unsigned char add_data2_end;
extern unsigned char add_data3[];
extern unsigned char add_data3_end;
extern unsigned char add_data4[];
extern unsigned char add_data4_end;
extern unsigned char enc_root_key[];
extern unsigned char enc_root_key_end;
extern unsigned char key_count[];
extern unsigned char key_count_end;

/************************************************************************
 * User Function Configuration
 ***********************************************************************/
//#define USER_OTP_PROGRAM
#ifdef USER_OTP_PROGRAM
#define OTP_PROGARM_OFFSET	2976
#define OTP_PROGARM_LEN		MARKETID_LEN
#endif

//#define USB_MP_TOOL_USAGE
#ifdef USB_MP_TOOL_USAGE
//#define BURN_USB2SRAM
#endif
#define DEBUG_PRT	1
//#define TEST_MODE

/************************************************************************
 *  Public variables
 ************************************************************************/
__align__ unsigned char dec_array[32*2]={0};
__align__ unsigned char v_array[1024]={0};
__align__ unsigned char otp_retry_cnt=0;
#define KEY_LEN 	16
#define MARKETID_LEN 	4
int printf(const char *fmt, ...);
int obfuse_getset(otp_method_enum_t type, unsigned char* enc_array, unsigned char* enc_array1, unsigned char* enc_array2, unsigned char* enc_array3, unsigned int len);
int obfuse_getset_b00(otp_method_enum_t type, unsigned char * seed_array, unsigned char* enc_array, unsigned char* enc_array1, unsigned char* enc_array2, unsigned char* enc_array3, unsigned int len);

//-----------------------------------------------------------------------------
// dummy function
//-----------------------------------------------------------------------------
int ctrlc()
{
	return 0;
}

int printf(const char *fmt, ...)
{
	return 1;
}
//-----------------------------------------------------------------------------
void* memcpy(void* dest, const void* src, size_t count) {
    char* dst8 = (char*)dest;
    char* src8 = (char*)src;

    while (count--) {
        *dst8++ = *src8++;
    }
    return dest;
}

/**
 *  * memcmp - Compare two areas of memory
 *   * @cs: One area of memory
 *    * @ct: Another area of memory
 *     * @count: The size of the area.
 *      */
int memcmp(const void * cs,const void * ct,size_t count)
{
        const unsigned char *su1, *su2;
        int res = 0;

        for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
                if ((res = *su1 - *su2) != 0)
                        break;
        return res;
}

int burn_kf(void)
{
    int i=0;
    unsigned char kf[16];
    unsigned char otp_kf[16];

    set_memory(kf, 0x0, 16);
    set_memory(otp_kf, 0x0, 16);
    OTP_Get_Byte(4736, otp_kf, 16);
    for(i=0;i<16;i++) {
	if (otp_kf[i] != 0)
		return 1;
    }

    if ((genTRNG(kf) == 0) || (genTRNG(kf+4) == 0) ||
        (genTRNG(kf+8) == 0) || (genTRNG(kf+12) == 0)) {
        prints("genTRNG fail\n");
        hexdump("kf: ", kf, 16);
            
        return 1;
    }

    hexdump("kf: ", kf, 16);

    // burn Kf key
    start_program(4736, 16, kf);

    OTP_Get_Byte(4736, otp_kf, 16);

    hexdump("otp_kf: ", otp_kf, 16);

    return 0;
}

//-----------------------------------------------------------------------------
int do_program(int secure, int count)
{
 	UINT32	vm_adv_rsa_pub_key_size  = (unsigned int )(&vm_adv_rsa_pub_key_end  - vm_adv_rsa_pub_key);
 	UINT32	vm_adv_rsa_pub_final_size  = (unsigned int )(&vm_adv_rsa_pub_final_end  - vm_adv_rsa_pub_final);
    	UINT32	aes_key_size = (unsigned int )(&aes_key_end - aes_key);
    	UINT32	aes_key1_size = (unsigned int )(&aes_key1_end - aes_key1);
    	UINT32	aes_key2_size = (unsigned int )(&aes_key2_end - aes_key2);
    	UINT32	aes_key3_size = (unsigned int )(&aes_key3_end - aes_key3);
    	UINT32	aes_seed_size = (unsigned int )(&aes_seed_end - aes_seed);
    	UINT32	cust_data_size = (unsigned int )(&cust_data_end - cust_data);
    	UINT32	chip_id_size = (unsigned int )(&chip_id_end - chip_id);
    	UINT32	jtag_pwd_size = (unsigned int )(&jtag_pwd_end - jtag_pwd);
    	UINT32	market_id_size = (unsigned int )(&market_id_end - market_id);
    	UINT32	add_data1_size = (unsigned int )(&add_data1_end - add_data1);
    	UINT32	add_data2_size = (unsigned int )(&add_data2_end - add_data2);
    	UINT32	add_data3_size = (unsigned int )(&add_data3_end - add_data3);
    	UINT32	add_data4_size = (unsigned int )(&add_data4_end - add_data4);
	UINT32	enc_root_key_size = (unsigned int )(&enc_root_key_end - enc_root_key);
	UINT32	key_count_size = (unsigned int )(&key_count_end - key_count);
	UINT32  key_count_int = (unsigned int)*((unsigned int *)key_count);
	unsigned char vmx_adv_rsa_pub[vm_adv_rsa_pub_key_size];
	unsigned char *pChar = NULL, *pChar1 = NULL;
#ifdef USER_OTP_PROGRAM
	unsigned char user_data[512];
#endif

	int res = -1, i=0;
    	//unsigned char rma_key[16] = { 0x0a, 0x01, 0x02, 0x03, 0xba, 0xbe, 0xef, 0xba,
        //                          0xbe, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
	prints("\nefuse programmer v1.08 start....\n");

#ifdef USER_OTP_PROGRAM
	prints("\totp program start....\n");
	//step1. customize user data first
        // should be implemented by user ....TBD....

        //step2. start otp program
	start_program(OTP_PROGARM_OFFSET, OTP_PROGARM_LEN, user_data);
	sync();

        //step3. stop otp program
	prints("\totp program ....\n");
	res = 0;	// complete without failure
	return res;
#endif
	prints("\tkey_count_int: ");
	print_hex(key_count_int);
	prints("\t \n");
	prints("\t-w rsa \n");

	sync();

	//check key count validation
	if ((chip_id_size != (KEY_LEN*key_count_int)) || (jtag_pwd_size != (KEY_LEN*key_count_int)) || (market_id_size != (MARKETID_LEN*key_count_int)) || (add_data1_size != (KEY_LEN*key_count_int)) || (add_data2_size != (KEY_LEN*key_count_int)) || (add_data3_size != (KEY_LEN*key_count_int)) || (add_data4_size != (KEY_LEN*key_count_int)) || (aes_seed_size != (KEY_LEN*key_count_int))) {
		prints("\tkey count is not correct!! \n");
		return 1;
	}

	if ((enc_root_key_size != (KEY_LEN*key_count_int))) {
		prints("\tenc root key count is not correct!! \n");
		return 1;
	}

	//count is from 1
	if (--count < 0) {
		prints("\tcount is not correct!! \n");
		return 1;
	}
	if (vm_adv_rsa_pub_final_size != vm_adv_rsa_pub_key_size) {
		prints("\tvm_adv_rsa_pub_key_size and vm_adv_rsa_pub_final_size is different !! \n");
		return 1;
	}

	for(i=0;i<vm_adv_rsa_pub_key_size;i++) {
		if (secure == 10) {
			pChar = vm_adv_rsa_pub_final;
			vmx_adv_rsa_pub[i] = pChar[i];
		}
		else {
			pChar = vm_adv_rsa_pub_key;
			vmx_adv_rsa_pub[i] = pChar[i];
		}
	}
	if (secure == 10) {
		pChar = vmx_adv_rsa_pub;
		pChar1 = vm_adv_rsa_pub_final;
		for(i=0;i<vm_adv_rsa_pub_key_size;i++) {
			if (pChar[i] != pChar1[i]){
				prints("\tvmx key error\n");
				return 1;
			}
		}
		hexdump("krsa(vmx)0:", vmx_adv_rsa_pub, vm_adv_rsa_pub_key_size);
		hexdump("krsa(vmx)1:", vm_adv_rsa_pub_final, vm_adv_rsa_pub_key_size);
		secure = 7; //re-use the mode 7
	}

	switch(secure) {
		case 1:
			start_program(0, vm_adv_rsa_pub_key_size, vm_adv_rsa_pub_key); 
			do_burn_efuse_rsa_pgm_dis();
			return 0;
		case 2:
			if (market_id_size > 0) {
				prints("\tmarket_id\n");
				hexdump("market_id :", market_id+(MARKETID_LEN*count), MARKETID_LEN);
				sync();
				start_program(2976, MARKETID_LEN, market_id+(MARKETID_LEN*count));
				sync();
			}
			else
				prints("error market_id length \n");
			return 0;
		case 3:
			do_burn_efuse_secureboot();
			return 0;
		case 4:
			do_burn_efuse_normal_dbg();
			return 0;
		case 5:
			do_burn_efuse_usb2sram();
			return 0;
		case 9:
			do_burn_efuse_normal_dbg();
			do_burn_efuse_secureboot();
		case 7:
			start_program(0, vm_adv_rsa_pub_key_size, vmx_adv_rsa_pub); 
			do_burn_efuse_rsa_pgm_dis();
		case 6:
			otp_retry_cnt=0;
			res = obfuse_getset_b00(ENUM_TYPE_B00_KEY_OBFUSE,aes_seed+(KEY_LEN*count),aes_key,aes_key1,aes_key2,aes_key3,KEY_LEN);
			#ifndef TEST_MODE 
			if (res)
				return res;
			#endif
			#ifdef TEST_MODE
			res = obfuse_getset_b00(ENUM_TYPE_B00_KEY_DEOBFUSE,aes_seed+(KEY_LEN*count),aes_key,aes_key1,aes_key2,aes_key3,KEY_LEN);
        		#ifndef TEST_MODE 
        		if (res)
                		return res;
        		#endif
        		#endif
			if ((chip_id_size > 0) && (jtag_pwd_size > 0))
			{
				if ((chip_id_size % KEY_LEN) || (jtag_pwd_size % KEY_LEN) || (market_id_size % MARKETID_LEN))
				{
					prints("\tmax. chip_id and jtag_pwd length is 16B or market_id lengtis 4B exceed!!\n");
					res = 0;
					return res;
	        		}
				prints("\t-w chip_id\n");
				#ifdef DEBUG_PRT
				hexdump("chip_id :", chip_id+(KEY_LEN*count), KEY_LEN);
				#endif
				prints("\t-w jtag_pwd\n");
				#ifdef DEBUG_PRT
				hexdump("jtag_pwd:", jtag_pwd+(KEY_LEN*count), KEY_LEN);
				#endif
				otp_retry_cnt=0;
        			res = obfuse_getset_b00(ENUM_TYPE_VMX_BB,chip_id+(KEY_LEN*count),jtag_pwd+(KEY_LEN*count),NULL,NULL,NULL,KEY_LEN);
        			sync();
        			if (!res)
                			return res;
				#ifdef DEBUG_PRT
				hexdump("add_data1:", add_data1+(KEY_LEN*count), KEY_LEN);
				#endif
				#ifdef DEBUG_PRT
				hexdump("add_data2:", add_data2+(KEY_LEN*count), KEY_LEN);
				#endif
				#ifdef DEBUG_PRT
				hexdump("add_data3:", add_data3+(KEY_LEN*count), KEY_LEN);
				#endif
				#ifdef DEBUG_PRT
				hexdump("add_data4:", add_data4+(KEY_LEN*count), KEY_LEN);
				#endif
				otp_retry_cnt=0;
        			res = obfuse_getset_b00(ENUM_TYPE_VMX_BB_ADD_DATA,add_data1+(KEY_LEN*count),add_data2+(KEY_LEN*count),add_data3+(KEY_LEN*count),add_data4+(KEY_LEN*count),NULL,KEY_LEN);
        			sync();
        			if (!res)
                			return res;
				sync();
				if(enc_root_key_size > 0) {
					prints("\tburn enc_root_key\n");
					hexdump("enc_root_key :", enc_root_key+(KEY_LEN*count), KEY_LEN);
					sync();
					start_program(5632, KEY_LEN , enc_root_key+(KEY_LEN*count));
					sync();
				}
				else
					prints("enc_root_key_size is not correct !!");
				do_burn_mp_efuse_bits_vmCertification();
			}
			else
				prints("chip_id, pwd, is not correct !!");
			return 0;
		case 8:
			do_burn_efuse_normal_dbg();
			do_burn_efuse_usb2sram();
			do_burn_efuse_secureboot();
			return 0;
		default:
			return 0;
	}
	res = 0;	// complete without failure
	return res;
}


/************************************************************************
 *
 *                          efusemain
 *  Description :
 *  -------------
 *  main function of efuse programmer
 *
 *  Parameters :
 *  ------------
 *
 *
 *
 *  Return values :
 *  ---------------
 *
 *
 *
 *
 ************************************************************************/
int efusemain (int argc, char * const argv[])
{
	int		ret = -1;
	extern unsigned int mcp_dscpt_addr;
	int chip_id=0;
	int vmx_mode=0;
	int offset=0;

	prints("efuse_programmer vmx for b00\n");
	chip_id = rtd_inl(SB2_CHIP_INFO);
	vmx_mode = rtd_inl(0x01400000);
	offset = rtd_inl(0x01400004);
	prints("chip_id=0x");
	print_hex(chip_id);
	prints("\n");
	prints("vmx_mode=0x");
	print_hex(vmx_mode);
	prints("\n");
	prints("offset=0x");
	print_hex(offset);
	prints("\n");
	mcp_dscpt_addr = 0;
	otp_retry_cnt = 0;
	init_uart();
	set_focus_uart(0);

#ifdef USB_MP_TOOL_USAGE
	prints("vmx program secure ...\n");
	do_burn_efuse_normal_dbg();
	do_burn_efuse_secureboot();
#ifdef BURN_USB2SRAM
	do_burn_efuse_usb2sram();
#endif
	ret = 0;
#else
    	ret = burn_kf();

	if (vmx_mode == 0x00000001) {
		prints("\tprogram krsa !!!\n");
		ret = do_program(1,offset);
	}
	else if (vmx_mode == 0x00000002) {
		prints("\tprogram market_id !!!\n");
		ret = do_program(2,offset);
   	}
        else if(vmx_mode == 0x00000003) {
		prints("\tprogram secure_boot !!!\n");
		ret = do_program(3,offset);
	} else if (vmx_mode == 0x00000004) {
		prints("\tprogram normal_dbg !!!\n");
		ret = do_program(4,offset);
	} else if (vmx_mode == 0x00000005) {
		prints("\tprogram usb2_sram !!!\n");
		ret = do_program(5,offset);
	} else if (vmx_mode == 0x00000006) {
		prints("\tprogram VMX BB Certification 0 !!!\n");
		ret = do_program(6,offset);
	} else if (vmx_mode == 0x00000007) {
		prints("\tprogram VMX BB Certification 1 !!!\n");
		ret = do_program(7,offset);
	} else if (vmx_mode == 0x00000008) {
		prints("\tprogram otp bits for usb mp tool !!!\n");
		ret = do_program(8,offset);
	} else if (vmx_mode == 0x00000009) {
		prints("\tprogram Product otp bits !!!\n");
		ret = do_program(9,offset);
	} else if (vmx_mode == 0x0000000a) {
		prints("\tprogram VMX BB Certification 2 !!!\n");
		ret = do_program(0xa,offset);
	}
	else {
		prints("\t Do nothing !!!!!!!\n");
   	}
#endif
	if (ret == 0)
		prints("program OTP OKOKOKOKOKOK!!!!!!!\n");
	else
		prints("program OTP failfailfail!!!!!!!\n");
    return 0;
}

