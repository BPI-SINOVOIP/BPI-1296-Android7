#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/idr.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/tee_drv.h>
#include <linux/uaccess.h>
#include "tee_private.h"
#include "tee_client_api.h"

#define TA_HDCPTX14_UUID { 0x87ef28e8, 0xf581, 0x4e3d, \
		{ 0xb2, 0xb2, 0xd7, 0xe3, 0xd4, 0x8b, 0x23, 0x21} }

#define HDCP14_AN_CMD_LEN                      8
#define HDCP14_BKSV_CMD_LEN                      5
#define HDCP14_R0_LEN                      2

#define HDCP14_PK_SIZE 280

#define MAX_SHA_DATA_SIZE       645
#define MAX_SHA_VPRIME_SIZE     20

enum HDCP14_CMD_FOR_TA
{
	TA_TEE_HDCP14_GenAn 			= 0x1,
	TA_TEE_HDCP14_WriteBKSV 		= 0x2,
	TA_TEE_HDCP14_SetRepeaterBitInTx			= 0x3,	
	TA_TEE_HDCP14_CheckRepeaterBitInTx 			= 0x4,
	TA_TEE_HDCP14_SetEnc 		= 0x5,
	TA_TEE_HDCP14_SetWinderWin			= 0x6,	
	TA_TEE_HDCP14_EnRi			= 0x7,	
	TA_TEE_HDCP14_SetAVMute			= 0x8,
	TA_TEE_HDCP14_SHAAppend			= 0x9,
	TA_TEE_HDCP14_ComputeV			= 0xa,
	TA_TEE_HDCP14_VerifyV			= 0xb,	
	TA_TEE_HDCP14_CheckR0			= 0xc,
};

enum hdcp_repeater {
        HDCP_RECEIVER = 0,
        HDCP_REPEATER = 1
};

enum encryption_state {
        HDCP_ENC_OFF = 0x0,
        HDCP_ENC_ON  = 0x1
};

enum ri_state {
        HDCP_RI_OFF = 0x0,
        HDCP_RI_ON  = 0x1
};

enum av_mute {
        AV_MUTE_SET = 0x01,
        AV_MUTE_CLEAR = 0x10
};

struct hdcp_sha_in {
        uint8_t data[MAX_SHA_DATA_SIZE];
        uint32_t byte_counter;
        uint8_t vprime[MAX_SHA_VPRIME_SIZE];
};


struct tee_ioctl_open_session_arg arg ;
struct tee_param  param[4];
struct tee_context *ctx ;

void ta_hdcp14_init(void)
{
pr_err("[TEE_KERNEL_DBG]   ta_hdcp14_init.\n");


  TEEC_UUID test_id = TA_HDCPTX14_UUID;
	

 ctx=tee_client_open_context(NULL,NULL,NULL,NULL);

//context->fd =ctx;

//memcpy(arg->uuid , &test_id,sizeof(test_id));
//pr_err("UUID = %x ",arg->uuid);


//&ctx_t =(void*) ctx;
memcpy(&arg.uuid , &test_id,sizeof(test_id));

arg.clnt_login = TEEC_LOGIN_PUBLIC;
arg.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;
//
param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
/*
params = kcalloc(arg.num_params, sizeof(struct tee_param),
				 GFP_KERNEL);
*/
//========================================================================
pr_err("UUID = %x ,%x ,%x \n ",arg.uuid[0],arg.uuid[1],arg.uuid[2]);
pr_err("before session  = %x   \n",arg.session);
tee_client_open_session(ctx,&arg,param);
pr_err("after  session  = %x   \n",arg.session);

	
}


int ta_hdcp_check_r0(uint8_t *ro)
{	
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;

	struct tee_shm *shm;
          int ret=0;

 	arg_I.func = TA_TEE_HDCP14_CheckR0;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx, HDCP14_R0_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);

	//pr_err("[TEE_KERNEL_DBG]   ta_hdcp_check_r0.\n");

	invoke_param[0].u.memref.size =HDCP14_R0_LEN ;
	invoke_param[0].u.memref.shm = shm;

            invoke_param[1].u.value.a = 0;

	memcpy(invoke_param[0].u.memref.shm->kaddr, ro, HDCP14_R0_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

          invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

tee_client_invoke_func(ctx,&arg_I,invoke_param);

ret=invoke_param[1].u.value.a;

//pr_err("[TEE_KERNEL_DBG]   ta_hdcp_check_r0 invoke_param[1].u.value.a: %d\n",invoke_param[1].u.value.a);
tee_shm_free(shm);
kfree(invoke_param);

return ret;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_generate_an (Generate An using HDCP HW)
 *-----------------------------------------------------------------------------
 */
int ta_hdcp_lib_generate_an(uint8_t *an)
{	
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;

 	arg_I.func = TA_TEE_HDCP14_GenAn;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx, HDCP14_AN_CMD_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_generate_an.\n");

	invoke_param[0].u.memref.size =HDCP14_AN_CMD_LEN ;
	invoke_param[0].u.memref.shm = shm;

	memcpy(invoke_param[0].u.memref.shm->kaddr, an, HDCP14_AN_CMD_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




tee_client_invoke_func(ctx,&arg_I,invoke_param);


memcpy(an, invoke_param[0].u.memref.shm->kaddr, HDCP14_AN_CMD_LEN);

tee_shm_free(shm);
kfree(invoke_param);
//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id3[0],Message_BACK_id3[1],Message_BACK_id3[2]);

return 0;//HDCP_OK
}

int ta_hdcp_lib_write_bksv(uint8_t *ksv_data, uint8_t *pk){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
          struct tee_shm *shm1;

 	arg_I.func = TA_TEE_HDCP14_WriteBKSV;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx, HDCP14_BKSV_CMD_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_write_bksv.\n");

	invoke_param[0].u.memref.size =HDCP14_BKSV_CMD_LEN ;
	invoke_param[0].u.memref.shm = shm;

	memcpy(invoke_param[0].u.memref.shm->kaddr, ksv_data, HDCP14_BKSV_CMD_LEN);

	shm1 = tee_shm_alloc(ctx, HDCP14_PK_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm1))
	return PTR_ERR(shm1);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_write_bksv pk.\n");

	invoke_param[1].u.memref.size =HDCP14_PK_SIZE ;
	invoke_param[1].u.memref.shm = shm1;

	memcpy(invoke_param[1].u.memref.shm->kaddr, pk, HDCP14_PK_SIZE);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




tee_client_invoke_func(ctx,&arg_I,invoke_param);


memcpy(ksv_data, invoke_param[0].u.memref.shm->kaddr, HDCP14_BKSV_CMD_LEN);

tee_shm_free(shm);
tee_shm_free(shm1);
kfree(invoke_param);
//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id3[0],Message_BACK_id3[1],Message_BACK_id3[2]);

return 0;//HDCP_OK

}

void ta_hdcp_lib_set_repeater_bit_in_tx(enum hdcp_repeater rx_mode)
{
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;

 	arg_I.func = TA_TEE_HDCP14_SetRepeaterBitInTx;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return;

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_set_repeater_bit_in_tx.\n");
        invoke_param[0].u.value.a = rx_mode;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




tee_client_invoke_func(ctx,&arg_I,invoke_param);
kfree(invoke_param);
return;

}

int ta_hdcp_lib_check_repeater_bit_in_tx(void){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
           unsigned int check_val=0;

 	arg_I.func = TA_TEE_HDCP14_CheckRepeaterBitInTx;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_check_repeater_bit_in_tx.\n");
        invoke_param[0].u.value.a = 0;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




tee_client_invoke_func(ctx,&arg_I,invoke_param);
check_val=invoke_param[0].u.value.a;
kfree(invoke_param);
pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_check_repeater_bit_in_tx: %d\n", check_val);

return check_val;
}

void ta_hdcp_lib_set_encryption(enum encryption_state enc_state)
{
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;

 	arg_I.func = TA_TEE_HDCP14_SetEnc;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return;

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_set_encryption.\n");
        invoke_param[0].u.value.a = enc_state;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

tee_client_invoke_func(ctx,&arg_I,invoke_param);
kfree(invoke_param);
return;
}

void ta_hdcp_lib_set_wider_window(void){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;

 	arg_I.func = TA_TEE_HDCP14_SetWinderWin;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return;

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_set_wider_window.\n");
        invoke_param[0].u.value.a = 1;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

tee_client_invoke_func(ctx,&arg_I,invoke_param);
kfree(invoke_param);
return;

}

void ta_hdcp_lib_set_ri(enum ri_state ri_state){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;

 	arg_I.func = TA_TEE_HDCP14_EnRi;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return;

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_set_ri.\n");
        invoke_param[0].u.value.a = ri_state;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

tee_client_invoke_func(ctx,&arg_I,invoke_param);
kfree(invoke_param);
return;

}

void ta_hdcp_lib_set_av_mute(enum av_mute av_mute_state)
{
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;

 	arg_I.func = TA_TEE_HDCP14_SetAVMute;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return;

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_set_av_mute.\n");
        invoke_param[0].u.value.a = av_mute_state;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

tee_client_invoke_func(ctx,&arg_I,invoke_param);
kfree(invoke_param);
return;

}

int ta_hdcp_lib_SHA_append_bstatus_M0(struct hdcp_sha_in *sha, uint8_t *bstatus){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
          struct tee_shm *shm1;

 	arg_I.func = TA_TEE_HDCP14_SHAAppend;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx, MAX_SHA_DATA_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_SHA_append.\n");

	invoke_param[0].u.memref.size =MAX_SHA_DATA_SIZE ;
	invoke_param[0].u.memref.shm = shm;

	memcpy(invoke_param[0].u.memref.shm->kaddr, sha->data, MAX_SHA_DATA_SIZE);

	shm1 = tee_shm_alloc(ctx, 2, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm1))
	return PTR_ERR(shm1);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_SHA_append bstaus.\n");

	invoke_param[1].u.memref.size =2 ;
	invoke_param[1].u.memref.shm = shm1;

	memcpy(invoke_param[1].u.memref.shm->kaddr, bstatus, 2);

            invoke_param[2].u.value.a = sha->byte_counter;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

pr_err("[TEE_KERNEL_DBG]   sha->byte_counter bef: %d\n",invoke_param[2].u.value.a);


tee_client_invoke_func(ctx,&arg_I,invoke_param);

 sha->byte_counter = invoke_param[2].u.value.a;
 memcpy(sha->data, invoke_param[0].u.memref.shm->kaddr, MAX_SHA_DATA_SIZE);
pr_err("[TEE_KERNEL_DBG]   sha->byte_counter aft: %d\n",invoke_param[2].u.value.a);
tee_shm_free(shm);
tee_shm_free(shm1);
kfree(invoke_param);
//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id3[0],Message_BACK_id3[1],Message_BACK_id3[2]);

return 0;//HDCP_OK

}

int ta_hdcp_lib_compute_V(struct hdcp_sha_in *sha){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;

            int ret=0;

 	arg_I.func = TA_TEE_HDCP14_ComputeV;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx, MAX_SHA_DATA_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_compute_V.\n");

	invoke_param[0].u.memref.size =MAX_SHA_DATA_SIZE ;
	invoke_param[0].u.memref.shm = shm;

	memcpy(invoke_param[0].u.memref.shm->kaddr, sha->data, MAX_SHA_DATA_SIZE);

            invoke_param[1].u.value.a = sha->byte_counter;
             invoke_param[1].u.value.b = 0;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

pr_err("[TEE_KERNEL_DBG]   return invoke_param[1].u.value.b: %d\n",invoke_param[1].u.value.b);


tee_client_invoke_func(ctx,&arg_I,invoke_param);

 sha->byte_counter = invoke_param[1].u.value.a;
 memcpy(sha->data, invoke_param[0].u.memref.shm->kaddr, MAX_SHA_DATA_SIZE);

ret=invoke_param[1].u.value.b;

tee_shm_free(shm);
kfree(invoke_param);
//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id3[0],Message_BACK_id3[1],Message_BACK_id3[2]);

return ret;

}

int ta_hdcp_lib_verify_V(struct hdcp_sha_in *sha){
       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
          int ret=0;

 	arg_I.func = TA_TEE_HDCP14_VerifyV;
	arg_I.session= arg.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx, MAX_SHA_VPRIME_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);

	pr_err("[TEE_KERNEL_DBG]   ta_hdcp_lib_verify_V.\n");

	invoke_param[0].u.memref.size =MAX_SHA_DATA_SIZE ;
	invoke_param[0].u.memref.shm = shm;

	memcpy(invoke_param[0].u.memref.shm->kaddr, sha->vprime, MAX_SHA_VPRIME_SIZE);

            invoke_param[1].u.value.a = 0;

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_VALUE_INOUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




tee_client_invoke_func(ctx,&arg_I,invoke_param);

 memcpy(sha->vprime, invoke_param[0].u.memref.shm->kaddr, MAX_SHA_VPRIME_SIZE);

pr_err("[TEE_KERNEL_DBG]   return invoke_param[1].u.value.a: %d\n",invoke_param[1].u.value.a);

ret=invoke_param[1].u.value.a;

tee_shm_free(shm);
kfree(invoke_param);
//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id3[0],Message_BACK_id3[1],Message_BACK_id3[2]);

return ret;

}


