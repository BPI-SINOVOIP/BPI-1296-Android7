

#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/idr.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/tee_drv.h>
#include <linux/uaccess.h>
#include "tee_private.h"
//#include "tee_client_api.h"


#include <ta_HDCP2p2_RX.h>
#include <hdcp2_TEE_TA.h>
#include <hdcp2_messages.h>

extern int h2MessageParse(unsigned char *message, unsigned int length);
extern int h2MessagePoll(unsigned char *message, unsigned int length);

extern int TEE2p1_Flag  ; 
void CA_hdcp2_load_from_TEE(void);

#if 1

//HDCP  2.2 COMMAND LENG
#define AKE_INIT_CMD_LEN                       12
#define AKE_SEND_NO_STORED_KM_LEN    129
#define AKE_SEND_STORED_KM_LEN    33
#define AKE_SEND_CERT_LEN     534
#define AKE_SEND_HPRIME_LEN     33
#define AKE_SEND_LPRIME_LEN     33
#define AKE_SEND_LC_INIT_LEN     9
#define AKE_SEND_EKS_LEN     25

   #define TA_HDCPRX_UUID { 0x80d173e0,0x0db9,0x11e6, \
		{ 0xa2,0x5c, 0x00,0x02,0xa5,0xd5,0xc5,0x1b} }

#define TA_HELLO_WORLD_CMD_INC_VALUE 0xff   

// 


unsigned char TEE_flag = 0  ,TA_Load_flag =0 ,AKE_INIT_flag =0;
unsigned char Message_BACK_id3[AKE_SEND_CERT_LEN];
unsigned char Message_BACK_id7[AKE_SEND_HPRIME_LEN];
unsigned char Message_BACK_id10[33];




//    struct tee_context ctx_t ;
       struct tee_context *ctx_HDCP2p2 ;

	struct tee_ioctl_open_session_arg arg_HDCP2p2 ;
	struct tee_param  param_HDCP2p2[4];
//TEEC_Operation op;

    unsigned char data_buffer[16]={0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 
                            0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x00};    
    u64  buffer[64];
#define TA_processAkeInit_Command                      0x08




void CA_processAkeInit(unsigned char *message, unsigned int length)
{

       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
	struct tee_shm *shm2;
     //  if (TEE_DRV_FLAG != 0x55)   return;
	//if (AKE_INIT_flag ==1)  return;
		
//	   TEE_HDCP_DEBUG("CA_processAkeInit %x  ",length);
//      CA_hdcp2_load_from_test();
       
      //   pr_err("CA_hdcp2_load_from_test finish  TTTTTT %x  \n");
        if (TEE2p1_Flag == 0)   return;
	 if (TA_Load_flag ==0)  return;
#if 1
		


 	arg_I.func = TA_processAkeInit_Command;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, AKE_INIT_CMD_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);


	shm2 = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_CERT_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm2))
	return PTR_ERR(shm2);


	invoke_param[0].u.memref.size =AKE_INIT_CMD_LEN ;
	invoke_param[0].u.memref.shm = shm;


	invoke_param[1].u.memref.size = AKE_SEND_CERT_LEN;
	invoke_param[1].u.memref.shm = shm2;

	memcpy(invoke_param[0].u.memref.shm->kaddr, message, AKE_INIT_CMD_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);


memcpy(Message_BACK_id3, invoke_param[1].u.memref.shm->kaddr, AKE_SEND_CERT_LEN);


//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id3[0],Message_BACK_id3[1],Message_BACK_id3[2]);
  
	
#endif
}



void CA_processAkeNoStoredEkm(unsigned char *message, unsigned int length)
{

       struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
	struct tee_shm *shm2;


 	arg_I.func = TA_processAkeNoStoredEkm_Command;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_NO_STORED_KM_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);


	shm2 = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_HPRIME_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm2))
	return PTR_ERR(shm2);
	//pr_err("[TEE_KERNEL_DBG]   length =%x  3.\n",length);

	invoke_param[0].u.memref.size = AKE_SEND_NO_STORED_KM_LEN;
	invoke_param[0].u.memref.shm = shm;


	invoke_param[1].u.memref.size = AKE_SEND_HPRIME_LEN;
	invoke_param[1].u.memref.shm = shm2;

	memcpy(invoke_param[0].u.memref.shm->kaddr, message, AKE_SEND_NO_STORED_KM_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ; 


	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




        tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

	memcpy(Message_BACK_id7, invoke_param[1].u.memref.shm->kaddr, AKE_SEND_HPRIME_LEN);
	// test 
       	memcpy(message, Message_BACK_id7, AKE_SEND_HPRIME_LEN);

//pr_err("Message_BACK_id7 [0] =%x  , %x ,%x ",Message_BACK_id7[0],Message_BACK_id7[1],Message_BACK_id7[2]);
  
    
    
}


void CA_processAkeStoredEkm(unsigned char *message, unsigned int length)
{
	struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
	struct tee_shm *shm2;



 	arg_I.func = TA_processAkeStoredEkm_Command;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_STORED_KM_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);


	shm2 = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_HPRIME_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm2))
	return PTR_ERR(shm2);
	//pr_err("[TEE_KERNEL_DBG]   3.\n");

	invoke_param[0].u.memref.size = AKE_SEND_STORED_KM_LEN;
	invoke_param[0].u.memref.shm = shm;


	invoke_param[1].u.memref.size = AKE_SEND_HPRIME_LEN;
	invoke_param[1].u.memref.shm = shm2;

	memcpy(invoke_param[0].u.memref.shm->kaddr, message, AKE_SEND_STORED_KM_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ; 


	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




        tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

	memcpy(Message_BACK_id7, invoke_param[1].u.memref.shm->kaddr, AKE_SEND_HPRIME_LEN);


//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id7[0],Message_BACK_id7[1],Message_BACK_id7[2]);
  

	 

	 	
	
	
}

void CA_processLcInit(unsigned char *message, unsigned int length)
{
	struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
	struct tee_shm *shm2;



 	arg_I.func = TA_processLcInit_Command;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_LC_INIT_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);


	shm2 = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_LPRIME_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm2))
	return PTR_ERR(shm2);
//	pr_err("[TEE_KERNEL_DBG]   3.\n");

	invoke_param[0].u.memref.size = AKE_SEND_LC_INIT_LEN;
	invoke_param[0].u.memref.shm = shm;


	invoke_param[1].u.memref.size = AKE_SEND_LPRIME_LEN;
	invoke_param[1].u.memref.shm = shm2;

	memcpy(invoke_param[0].u.memref.shm->kaddr, message, AKE_SEND_LC_INIT_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




        tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

	memcpy(Message_BACK_id10, invoke_param[1].u.memref.shm->kaddr, AKE_SEND_LPRIME_LEN);


//pr_err("Message_BACK_id3 [0] =%x  , %x ,%x ",Message_BACK_id10[0],Message_BACK_id10[1],Message_BACK_id10[2]);
  


	
	
}


	
void CA_processSkeSendEks(unsigned char *message, unsigned int length)
{

	struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;
	struct tee_shm *shm2;



 	arg_I.func = TA_processSkeSendEks_Command;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_EKS_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);


	shm2 = tee_shm_alloc(ctx_HDCP2p2, AKE_SEND_LPRIME_LEN, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm2))
	return PTR_ERR(shm2);
	//pr_err("[TEE_KERNEL_DBG] CA_processSkeSendEks  success.\n");

	invoke_param[0].u.memref.size = AKE_SEND_EKS_LEN;
	invoke_param[0].u.memref.shm = shm;


	invoke_param[1].u.memref.size = AKE_SEND_LPRIME_LEN;
	invoke_param[1].u.memref.shm = shm2;

	memcpy(invoke_param[0].u.memref.shm->kaddr, message, AKE_SEND_EKS_LEN);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_OUTPUT ; 


	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




        tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

		



	
}

#if 1
void  CA_hdcp2_Rx_GetEKhKm(unsigned char *pOut, unsigned int  ulSize)
{
       int i ;

struct tee_param *invoke_param = NULL;
	struct tee_ioctl_invoke_arg arg_I;
	
	struct tee_shm *shm;



      // pr_err("CA_hdcp2_Rx_GetEKhKm") ;
 	arg_I.func = TA_hdcp2_Rx_GetEKhKm_Command;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, 16, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);




	invoke_param[0].u.memref.size = 16;
	invoke_param[0].u.memref.shm = shm;



	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INOUT ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;




        tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

          
	
	 memcpy(pOut,invoke_param[0].u.memref.shm->kaddr, 16);	
/*
       for (i=0;i<16;i++)
       {
        pr_err("CA_hdcp2_Rx_GetEKhKm pout[%x] = %x ",i,pOut[i]) ;
       }
       */
       //    pr_err("CA_hdcp2_Rx_GetEKhKm pout = %x ,%x ,%x,%x,%x",pOut[0],pOut[1],pOut[2],pOut[3],pOut[4]) ;

	

}
#endif


void ta_init(void)
{


  TEEC_UUID test_id = TA_HDCPRX_UUID;
	

 ctx_HDCP2p2=tee_client_open_context(NULL,NULL,NULL,NULL);

//context->fd =ctx;

//memcpy(arg->uuid , &test_id,sizeof(test_id));
//pr_err("UUID = %x ",arg->uuid);


//&ctx_t =(void*) ctx;
memcpy(&arg_HDCP2p2.uuid , &test_id,sizeof(test_id));

arg_HDCP2p2.clnt_login = TEEC_LOGIN_PUBLIC;
arg_HDCP2p2.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;
//
param_HDCP2p2[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
param_HDCP2p2[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
param_HDCP2p2[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
param_HDCP2p2[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 
/*
params = kcalloc(arg.num_params, sizeof(struct tee_param),
				 GFP_KERNEL);
*/
//========================================================================
TEE_HDCP_INFO("UUID = %x ,%x ,%x \n ",arg_HDCP2p2.uuid[0],arg_HDCP2p2.uuid[1],arg_HDCP2p2.uuid[2]);
//pr_err("before session  = %x   \n",arg.session);
tee_client_open_session(ctx_HDCP2p2,&arg_HDCP2p2,&param_HDCP2p2);
//pr_err("after  session  = %x   \n",arg.session);

	
}


void CA_hdcp2_Rx_SetLc128(unsigned char *pIn)
{

        struct tee_param *invoke_param = NULL;
	 struct tee_ioctl_invoke_arg arg_I;
//	 unsigned char LC128[LC128_SIZE] , i ;
	struct tee_shm *shm;	

      // pr_err("CA_hdcp2_Rx_SetLc128\n");
	/* Initialize a context connecting us to the TEE */
	TEE_HDCP_DEBUG("CA_hdcp2_Rx_SetLc128");

/*

       for (i=0;i<LC128_SIZE ;i++)
       {
		LC128[i] = *(pIn+i) ;
	   
       }
*/	
 	arg_I.func = TA_HDCP2p2_SETLC128;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, LC128_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);
	


	invoke_param[1].u.memref.size = LC128_SIZE;
	invoke_param[1].u.memref.shm = shm;

	memcpy(invoke_param[1].u.memref.shm->kaddr, pIn, LC128_SIZE);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ; 

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

	tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

}

#if 1
void CA_hdcp2_Rx_SetCERT_RX(unsigned char *pIn)
{

        struct tee_param *invoke_param = NULL;
	 struct tee_ioctl_invoke_arg arg_I;
	struct tee_shm *shm;	

       //pr_err("CA_hdcp2_Rx_SetCERT_RX\n");
	/* Initialize a context connecting us to the TEE */
	TEE_HDCP_DEBUG("CA_hdcp2_Rx_SetCERT_RX  % x  %x %x ",pIn[0],pIn[1],pIn[2]);

	arg_I.func = TA_HDCP2p2_CERT_RX;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, CERT_RX_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);
	


	invoke_param[1].u.memref.size = CERT_RX_SIZE;
	invoke_param[1].u.memref.shm = shm;

	memcpy(invoke_param[1].u.memref.shm->kaddr, pIn, CERT_RX_SIZE);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ; 


	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

	tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);

	

	//pr_err("TA incremented value to 0x%x\n", op.params[0].value.a);

}


void CA_hdcp2_Rx_SetKPRIVRX(unsigned char *pIn)
{

        struct tee_param *invoke_param = NULL;
	 struct tee_ioctl_invoke_arg arg_I;
	 struct tee_shm *shm;	


	

	TEE_HDCP_INFO("CA_hdcp2_Rx_SetKPRIVRX\n");

	arg_I.func = TA_HDCP2p2_SETKPRIVRX;
	arg_I.session= arg_HDCP2p2.session;
	arg_I.num_params = TEEC_CONFIG_PAYLOAD_REF_COUNT;

	invoke_param = kcalloc(TEEC_CONFIG_PAYLOAD_REF_COUNT, sizeof(struct tee_param),GFP_KERNEL);
		if (!invoke_param)
			return -ENOMEM;

       
	shm = tee_shm_alloc(ctx_HDCP2p2, KPRIVRX_SIZE, TEE_SHM_MAPPED | TEE_SHM_DMA_BUF);
	if (IS_ERR(shm))
	return PTR_ERR(shm);
	


	invoke_param[1].u.memref.size = KPRIVRX_SIZE;
	invoke_param[1].u.memref.shm = shm;

	memcpy(invoke_param[1].u.memref.shm->kaddr, pIn, KPRIVRX_SIZE);

	invoke_param[0].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

       invoke_param[1].attr = TEE_IOCTL_PARAM_ATTR_TYPE_MEMREF_INPUT ; 
	

	invoke_param[2].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ; 

	invoke_param[3].attr = TEE_IOCTL_PARAM_ATTR_TYPE_NONE ;

	tee_client_invoke_func(ctx_HDCP2p2,&arg_I,invoke_param);



//	pr_err("TA incremented value to 0x%x\n", op.params[0].value.a);

}

#endif


void CA_hdcp2_load_from_MP(unsigned char* keyIn)
{
       if (TEE2p1_Flag == 0)   return;
       if (TA_Load_flag ==0)
       {
 	ta_init();
	#if 1
   	CA_hdcp2_Rx_SetLc128(keyIn);
	 CA_hdcp2_Rx_SetCERT_RX(keyIn+16);
        CA_hdcp2_Rx_SetKPRIVRX(keyIn+16+522);

	TEE_HDCP_INFO("MP KEY INPUTkeyIn = 0x%x,0x%x ,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x",keyIn[0],keyIn[1],keyIn[2],keyIn[3],keyIn[4],keyIn[5],keyIn[6],keyIn[7],keyIn[8],keyIn[9],keyIn[10],keyIn[11]);
       #else
        CA_hdcp2_Rx_SetLc128(&TEST_key[4]);
	 CA_hdcp2_Rx_SetCERT_RX(&TEST_key[40]);
        CA_hdcp2_Rx_SetKPRIVRX(&TEST_key[40+522]);


	#endif
	TA_Load_flag = 1;
       }
}

	
void CA_hdcp2_load_from_TEE(void)
{
//  if (TEE_DRV_FLAG != 0x55)   return;
       if (TEE2p1_Flag == 0)   return;
#if 1// real key 
       if (TA_Load_flag ==0)
       {
        ta_init(); 
        TEE_HDCP_INFO("CA_hdcp2_load_from_TEE \n ");
       //  ta_init(); 
 
        CA_hdcp2_Rx_SetLc128(&TEST_key[4]);
	 CA_hdcp2_Rx_SetCERT_RX(&TEST_key[40]);
        CA_hdcp2_Rx_SetKPRIVRX(&TEST_key[40+522]);


		
	TA_Load_flag = 1;
	//ta_deinit();
       }
#else   //test key 
       if (TA_Load_flag ==0)
       {
        CA_hdcp2_Rx_SetLc128(&gcLc128_test_cloud[0]);
	 CA_hdcp2_Rx_SetCERT_RX(gcCert1_test_cloud);
        CA_hdcp2_Rx_SetKPRIVRX(gcKpriv1_test_cloud);
			TA_Load_flag = 1;
	//ta_deinit();
       }
//ta_deinit();
#endif
	//ta_deinit();
}

unsigned char TX_msg_AKE_INIT[12] =
{
	0x2,0xf9,0xf1,0x30,0xa8,0x2d,0x5b,0xe5,0xc3,0x02,0x0,0x0 //PDF SAMPLE code
//0x2 ,0xf0 ,0x82  , 0x4c  ,0x18 , 0x86  ,0xeb  , 0xe1  ,0xb ,0x2  ,0x0  , 0x0 

};

unsigned const char TX_msg_AKE_NO_Stored_Km[129] =
{

#if 1
	0x04,0xa8, 0x55, 0xc2, 0xc4, 0xc6, 0xbe, 
	0xef, 0xcd, 0xcb, 0x9f, 0xe3, 0x9f, 
	0x2a, 0xb7, 0x29, 0x76, 0xfe, 0xd8, 
	0xda, 0xc9, 0x38, 0xfa, 0x39, 0xf0, 
	0xab, 0xca, 0x8a, 0xed, 0x95, 0x7b, 
	0x93, 0xb2, 0xdf, 0xd0, 0x7d, 0x09, 
	0x9d, 0x05, 0x96, 0x66, 0x03, 0x6e, 
	0xba, 0xe0, 0x63, 0x0f, 0x30, 0x77, 
	0xc2, 0xbb, 0xe2, 0x11, 0x39, 0xe5, 
	0x27, 0x78, 0xee, 0x64, 0xf2, 0x85, 
	0x36, 0x57, 0xc3, 0x39, 0xd2, 0x7b, 
	0x79, 0x03, 0xb7, 0xcc, 0x82, 0xcb, 
	0xf0, 0x62, 0x82, 0x43, 0x38, 0x09, 
	0x9b, 0x71, 0xaa, 0x38, 0xa6, 0x3f, 
	0x48, 0x12, 0x6d, 0x8c, 0x5e, 0x07, 
	0x90, 0x76, 0xac, 0x90, 0x99, 0x51, 
	0x5b, 0x06, 0xa5, 0xfa, 0x50, 0xe4, 
	0xf9, 0x25, 0xc3, 0x07, 0x12, 0x37, 
	0x64, 0x92, 0xd7, 0xdb, 0xd3, 0x34, 
	0x1c, 0xe4, 0xfa, 0xdd, 0x09, 0xe6, 
	0x28, 0x3d, 0x0c, 0xad, 0xa9, 0xd8, 
	0xe1, 0xb5,
#else

0x4  , 0x1c  ,        
0xa9 , 0x58  ,       
0x6c , 0x93  ,       
0xe7 , 0x7f  ,       
0x6b , 0x25  ,       
0xd3 , 0x9   ,       
0x52 , 0x5f  ,       
0x7e , 0xd1  ,       
0x10 , 0x94  ,       
0x32 , 0x39  ,       
0x10 , 0xbf  ,       
0xc5 , 0xba  ,       
0x74 , 0x41  ,       
0xd  , 0x37  ,        
0x5c , 0x2e  ,       
0x2d , 0xf1  ,       
0xbf , 0x35  ,       
0x94 , 0x34  ,       
0xf9 , 0x15  ,       
0xeb , 0xcc  ,       
0xcb , 0x99  ,       
0xbe , 0x10  ,       
0xf4 , 0xab  ,       
0x67 , 0x83  ,       
0xa7 , 0x75  ,       
0xf0 , 0xa   ,       
0x9  , 0xce  ,        
0x67 , 0x53  ,       
0xa8 , 0x78  ,       
0xf2 , 0xe4  ,       
0x8  , 0xed  ,        
0x9f , 0x98  ,       
0x2f , 0x60  ,       
0xcd , 0x3c  ,       
0xc7 , 0x73  ,       
0xd7 , 0xec  ,       
0x9c , 0xa   ,       
0xfd , 0x52  ,       
0x62 , 0x82  ,       
0x29 , 0x4f  ,       
0xaa , 0x5a  ,       
0x73 , 0x96  ,       
0x55 , 0xa1  ,       
0x55 , 0xf   ,       
0xab , 0x1a  ,       
0x91 , 0x82  ,       
0x28 , 0x20  ,       
0x18 , 0x76  ,       
0x3d , 0xbe  ,       
0x11 , 0x6e  ,       
0x1c , 0x44  ,       
0x21 , 0x7f  ,       
0x53 , 0x9b  ,       
0x6a , 0x74  ,       
0xda , 0x86  ,       
0x13 , 0x7c  ,       
0x7b , 0x52  ,       
0x33 , 0xd3  ,       
0x52 , 0xeb  ,       
0x7f , 0xf2  ,       
0xf2 , 0x9b  ,       
0x97 , 0xc1  ,       
0x48 , 0x50  ,       
0xc4 , 0x1b  ,       
0x5c  

 


#endif

};

unsigned const char TX_msg_AKE_Stored_Km[33] =
{
0x05,
0xe6 ,0x57 ,0x8e, 0xbc ,0xc7 ,0x68 ,0x44 ,0x87 ,0x88 ,0x8a ,0x9b,0xd7,0xd6,0xae,0x38,0xbe,  // Ekh(km)
0xf9, 0xf1, 0x30, 0xa8, 0x2d, 0x5b ,0xe5 ,0xc3 ,0xe1 ,0x7a ,0xb0 ,0xfd ,0x0f, 0x54, 0x40, 0x52 ,//m

};


unsigned const char TX_msg_LC_init[9] =
{
	0x09,0xa0,0xfe,0x9b,0xb8,0x20,0x60,0x58,0xca,
// 0x09,   0x2E  ,0x4A   ,0x69    ,0x45       ,0x20    ,0x08   ,0xE9   ,0x2C       
};



unsigned const char TX_msg_SKE_send_EKS[] =
{
	0x0b,0xb6 , 0x8b, 0x8a, 0xa4, 0xd2, 0xcb, 0xba, 0xff, 0x53, 0x33, 0xc1, 0xd9 ,0xbb, 0xb7, 0x10, 0xa9,  
	0x9a, 0x6d, 0x11, 0x00, 0xa9, 0xb7, 0x6f , 0x64,  
};


unsigned char x_bLen;
unsigned char C_TX_msg_id[540];




void cloud_TEE_2p1_test(void)
{


        unsigned int i;
   


        CA_hdcp2_load_from_TEE();

#if 0
	//AKE INIT test 
        	 x_bLen = 12;
	memcpy(C_TX_msg_id,TX_msg_AKE_INIT,x_bLen);
	h2MessageParse(C_TX_msg_id,x_bLen);
       pr_err("TA AKE INIT BACK COMMAND =  0x%x\n",C_TX_msg_id[0] );
	//#else

	//step2
	
        	 x_bLen = 129;
		memcpy(C_TX_msg_id,TX_msg_AKE_NO_Stored_Km,x_bLen);
	h2MessageParse(C_TX_msg_id,x_bLen);
	
	for (i=0;i<33;i++)
	{
	 pr_err("TA AKE SNO stored Km  BACK COMMAND =  0x%x\n",C_TX_msg_id[i] );
	}
	
		//step3
       h2MessagePoll(C_TX_msg_id, 0);

#if 0
         x_bLen = 33;
		memcpy(C_TX_msg_id,TX_msg_AKE_Stored_Km,x_bLen);
	h2MessageParse(C_TX_msg_id,x_bLen);
#endif
        	//step4
	 x_bLen = 9;
		memcpy(C_TX_msg_id,TX_msg_LC_init,x_bLen);
	h2MessageParse(C_TX_msg_id,x_bLen);   


	x_bLen = 25;
		memcpy(C_TX_msg_id,TX_msg_SKE_send_EKS,x_bLen);
	h2MessageParse(C_TX_msg_id,x_bLen);   


	//ta_deinit();

#endif
}
#else

void cloud_TEE_2p1_test(void)
{

}

#endif

