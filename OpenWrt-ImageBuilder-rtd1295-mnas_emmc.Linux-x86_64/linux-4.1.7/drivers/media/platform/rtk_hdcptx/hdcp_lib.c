/*
 * hdcp_lib.c - RTK hdcp tx driver
 *
 * Copyright (C) 2017 Realtek Semiconductor Corporation
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/delay.h>
#include <linux/dma-mapping.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include "hdcp_top.h"    
#include "hdcp.h"
#include "hdcp_ddc.h"

#include "hdmitx_reg.h"


#ifdef CONFIG_RTK_HDCP1x_REPEATER
/* RX function, for HDCP repeater */
extern void HdmiRx_save_tx_ksv(struct hdcp_ksvlist_info *tx_ksv);
extern void HdmiRx_set_bstatus(unsigned char byte1,unsigned char byte0);
#endif
extern int hdmitx_switch_get_state(void);

static void hdcp_lib_read_an(uint8_t *an);
static int hdcp_lib_read_aksv(uint8_t *ksv_data);
static int hdcp_lib_generateKm(uint8_t *Bksv);
static int hdcp_lib_write_bksv(uint8_t *ksv_data);
static int hdcp_lib_generate_an(uint8_t *an);
static void hdcp_lib_SHA_append_bstatus_M0(struct hdcp_sha_in *sha, uint8_t *data);
static void hdcp_lib_set_repeater_bit_in_tx(enum hdcp_repeater rx_mode);
//static void hdcp_lib_toggle_repeater_bit_in_tx(void);
static int hdcp_lib_initiate_step1(void);
static int hdcp_lib_check_ksv(uint8_t ksv[5]);
static void hdcp_lib_set_wider_window(void);	
	
/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_read_an
 *-----------------------------------------------------------------------------
 */
static void hdcp_lib_read_an(uint8_t *an)
{
		unsigned int temp;
		
		/* Convert HDCP An from bit endien to little endien. 
		HDCP An should stored in little endien,	but HDCP HW store in big endien. */
		
		temp= RD_REG_32( hdcp.hdcp_base_addr , HDMI_HDCP_ANLR);	//HDCP_ANLR => An LSB
		
		an[0] = temp & 0x0ff;
		an[1] = (temp >> 8) & 0xff;
		an[2] = (temp >>16) & 0xff;
		an[3] = (temp >>24) & 0xff;
		
		temp= RD_REG_32( hdcp.hdcp_base_addr , HDMI_HDCP_ANMR);	//HDCP_ANMR => An MSB
				
		an[4] = temp & 0x0ff;
		an[5] = (temp>> 8) & 0xff;
		an[6] = (temp>>16) & 0xff;
		an[7] = (temp>>24) & 0xff;
			
		return; 								 
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_generate_an (Generate An using HDCP HW)
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_generate_an(uint8_t *an)
{	
		int retry =0;
		
		/* Get An */
		/* get An influence from CRC64 */
		 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(0) |
												   HDMI_HDCP_AUTH_ddpken(0) |
												   HDMI_HDCP_AUTH_write_en8(0) |
												   HDMI_HDCP_AUTH_resetkmacc(0) |
												   HDMI_HDCP_AUTH_write_en7(0) |
												   HDMI_HDCP_AUTH_update_an(0) |
												   HDMI_HDCP_AUTH_write_en6(1) |
												   HDMI_HDCP_AUTH_aninfreq(1) |
												   HDMI_HDCP_AUTH_write_en5(0) |
												   HDMI_HDCP_AUTH_seedload(0) |
												   HDMI_HDCP_AUTH_write_en4(0) |
												   HDMI_HDCP_AUTH_deviceauthenticated(0) |
												   HDMI_HDCP_AUTH_write_en3(0) |
												   HDMI_HDCP_AUTH_forcetounauthenticated(0) |
												   HDMI_HDCP_AUTH_write_en2(0) |
												   HDMI_HDCP_AUTH_authcompute(0) |
												   HDMI_HDCP_AUTH_write_en1(0) |
												   HDMI_HDCP_AUTH_authrequest(0));
											 
		/* set An Influence Mode, influence will be load from AnIR0, AnIR1 */		
		 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR, HDMI_HDCP_CR_write_en4(1) |
												 HDMI_HDCP_CR_en1_1_feature(1) |  //disable
												 HDMI_HDCP_CR_write_en3(0) |
												 HDMI_HDCP_CR_downstrisrepeater(0) |
												 HDMI_HDCP_CR_write_en2(1) |
												 HDMI_HDCP_CR_aninfluencemode(1) |
												 HDMI_HDCP_CR_write_en1(0) |
												 HDMI_HDCP_CR_hdcp_encryptenable(0));
			   
		/* trigger to get An */		
		 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(0) |
												   HDMI_HDCP_AUTH_ddpken(0) |
												   HDMI_HDCP_AUTH_write_en8(0) |
												   HDMI_HDCP_AUTH_resetkmacc(0) |
												   HDMI_HDCP_AUTH_write_en7(0) |
												   HDMI_HDCP_AUTH_update_an(0) |
												   HDMI_HDCP_AUTH_write_en6(0) |
												   HDMI_HDCP_AUTH_aninfreq(0) |
												   HDMI_HDCP_AUTH_write_en5(0) |
												   HDMI_HDCP_AUTH_seedload(0) |
												   HDMI_HDCP_AUTH_write_en4(0) |
												   HDMI_HDCP_AUTH_deviceauthenticated(0) |
												   HDMI_HDCP_AUTH_write_en3(0) |
												   HDMI_HDCP_AUTH_forcetounauthenticated(0) |
												   HDMI_HDCP_AUTH_write_en2(0) |
												   HDMI_HDCP_AUTH_authcompute(0) |
												   HDMI_HDCP_AUTH_write_en1(1) |
												   HDMI_HDCP_AUTH_authrequest(1));
			

		while(! HDMI_HDCP_SR_get_anready( RD_REG_32(hdcp.hdcp_base_addr ,HDMI_HDCP_SR) ) )      //check if An is ready
		{			
			mdelay(100);
			
			if(retry == 5)
				return -HDCP_AUTH_FAILURE;
	
			retry++;		
		}

		// leave An influence mode 
		// set use internal rander bit generator
		 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR, HDMI_HDCP_CR_write_en4(1) |
												 HDMI_HDCP_CR_en1_1_feature(1) |//disable
												 HDMI_HDCP_CR_write_en3(0) |
												 HDMI_HDCP_CR_downstrisrepeater(0) |
												 HDMI_HDCP_CR_write_en2(1) |
												 HDMI_HDCP_CR_aninfluencemode(0) |
												 HDMI_HDCP_CR_write_en1(0) |
												 HDMI_HDCP_CR_hdcp_encryptenable(0));
		
        hdcp_lib_read_an(an);

        HDCP_DEBUG("AN1: %x %x %x %x %x %x %x %x", an[0], an[1], an[2], an[3],an[4], an[5], an[6], an[7]);								   
		
		return HDCP_OK;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_read_aksv
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_read_aksv(uint8_t *ksv_data)
{
	if(hdcp.hdcp_enabled == 0)
		return -HDCP_CANCELLED_AUTH;		
			
	*(ksv_data+0)=hdcp.en_ctrl->Aksv[0];
	*(ksv_data+1)=hdcp.en_ctrl->Aksv[1];
	*(ksv_data+2)=hdcp.en_ctrl->Aksv[2];
	*(ksv_data+3)=hdcp.en_ctrl->Aksv[3];
	*(ksv_data+4)=hdcp.en_ctrl->Aksv[4];
		
	return HDCP_OK;

}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_check_ksv
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_check_ksv(uint8_t ksv[5])
{
        int i, j;
        int zero =0, one =0;

        for (i =0; i < 5; i++) {
                /* Count number of zero / one */
                for (j =0; j < 8; j++) {
                        if (ksv[i] & (0x01 << j))
                                one++;
                        else
                                zero++;
                }
        }

        if (one == zero)
            return 0;
        else
            return -1;

}
/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_set_repeater_bit_in_tx
 *-----------------------------------------------------------------------------
 */
static void hdcp_lib_set_repeater_bit_in_tx(enum hdcp_repeater rx_mode)
{
        HDCP_DEBUG("hdcp_lib_set_repeater_bit_in_tx() value=%d", rx_mode);
		
		WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR, HDMI_HDCP_CR_write_en4(1) |
												HDMI_HDCP_CR_en1_1_feature(1) |//disable
												HDMI_HDCP_CR_write_en3(1) |
												HDMI_HDCP_CR_downstrisrepeater(rx_mode) |
												HDMI_HDCP_CR_write_en2(0) |
												HDMI_HDCP_CR_aninfluencemode(0) |
												HDMI_HDCP_CR_write_en1(0) |
												HDMI_HDCP_CR_hdcp_encryptenable(0));	   
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_check_repeater_bit_in_tx
 *-----------------------------------------------------------------------------
 */
int hdcp_lib_check_repeater_bit_in_tx(void)
{

#if(USED_TA_CMD)
                    return ta_hdcp_lib_check_repeater_bit_in_tx();
#else
		return HDMI_HDCP_CR_get_downstrisrepeater(RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR));	
#endif
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_generateKm
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_generateKm(uint8_t *Bksv)
{
	int i, j;
	int key0=0,key1=0;
	unsigned char DK1,DK2,DK3,DK4,DK5,DK6,DK7;
	int retry = 0;
	
	if(hdcp.hdcp_enabled==0)
		return -HDCP_CANCELLED_AUTH;
		
	for(i=0; i<5; i++) {
		for(j=0; j<8; j++) {
			if(Bksv[i] & (1<<j))
			{
				DK1=hdcp.en_ctrl->PK[i*56+j*7];
				DK2=hdcp.en_ctrl->PK[i*56+j*7+1];
				DK3=hdcp.en_ctrl->PK[i*56+j*7+2];
				DK4=hdcp.en_ctrl->PK[i*56+j*7+3];
				DK5=hdcp.en_ctrl->PK[i*56+j*7+4];
				DK6=hdcp.en_ctrl->PK[i*56+j*7+5];
				DK7=hdcp.en_ctrl->PK[i*56+j*7+6];		
				key1=DK7&0x0ff;
				key1=(key1<<8)|DK6;
				key1=(key1<<8)|DK5;
				key1=(key1<<8)|DK4;
				key0=DK3&0x0ff;
				key0=(key0<<8)|DK2;
				key0=(key0<<8)|DK1;

				
				/* write to HW */
				 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_DPKLR, HDMI_HDCP_DPKLR_ddpklsb24(key0) |  HDMI_HDCP_DPKLR_dpkencpnt(0x55));
				 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_DPKMR, HDMI_HDCP_DPKMR_ddpkmsw(key1));
				
				//trigger accumulation
				 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(1) |
														   HDMI_HDCP_AUTH_ddpken(1) |
														   HDMI_HDCP_AUTH_write_en8(0) |
														   HDMI_HDCP_AUTH_resetkmacc(0) |
														   HDMI_HDCP_AUTH_write_en7(0) |
														   HDMI_HDCP_AUTH_update_an(0) |
														   HDMI_HDCP_AUTH_write_en6(0) |
														   HDMI_HDCP_AUTH_aninfreq(0) |
														   HDMI_HDCP_AUTH_write_en5(0) |
														   HDMI_HDCP_AUTH_seedload(0) |
														   HDMI_HDCP_AUTH_write_en4(0) |
														   HDMI_HDCP_AUTH_deviceauthenticated(0) |
														   HDMI_HDCP_AUTH_write_en3(0) |
														   HDMI_HDCP_AUTH_forcetounauthenticated(0) |
														   HDMI_HDCP_AUTH_write_en2(0) |
														   HDMI_HDCP_AUTH_authcompute(0) |
														   HDMI_HDCP_AUTH_write_en1(0) |
														   HDMI_HDCP_AUTH_authrequest(0));	
				
				/* wait HW accumulation */
				while( !HDMI_HDCP_SR_get_curdpkaccdone( RD_REG_32(hdcp.hdcp_base_addr,HDMI_HDCP_SR) ) )
				{
					retry++;					
				}
			}
		}
	}
	
	return HDCP_OK;
}


/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_write_bksv
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_write_bksv(uint8_t *ksv_data)
{
	int status = HDCP_OK;	
	
	//HDCP_DEBUG("[%s] %s  %d :%u\n", __FILE__,__FUNCTION__,__LINE__,jiffies_to_msecs(jiffies));
	
	/* force Encryption disable */	
	 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR, HDMI_HDCP_CR_write_en4(1) |
											 HDMI_HDCP_CR_en1_1_feature(1) |//disable
											 HDMI_HDCP_CR_write_en3(0) |
											 HDMI_HDCP_CR_downstrisrepeater(0) |
											 HDMI_HDCP_CR_write_en2(0) |
											 HDMI_HDCP_CR_aninfluencemode(0) |
											 HDMI_HDCP_CR_write_en1(1) |
											 HDMI_HDCP_CR_hdcp_encryptenable(0));
	
	/* reset Km accumulation */
	 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(0) |
											   HDMI_HDCP_AUTH_ddpken(0) |
											   HDMI_HDCP_AUTH_write_en8(1) |
											   HDMI_HDCP_AUTH_resetkmacc(1) |
											   HDMI_HDCP_AUTH_write_en7(0) |
											   HDMI_HDCP_AUTH_update_an(0) |
											   HDMI_HDCP_AUTH_write_en6(0) |
											   HDMI_HDCP_AUTH_aninfreq(0) |
											   HDMI_HDCP_AUTH_write_en5(0) |
											   HDMI_HDCP_AUTH_seedload(0) |
											   HDMI_HDCP_AUTH_write_en4(0) |
											   HDMI_HDCP_AUTH_deviceauthenticated(0) |
											   HDMI_HDCP_AUTH_write_en3(0) |
											   HDMI_HDCP_AUTH_forcetounauthenticated(0) |
											   HDMI_HDCP_AUTH_write_en2(0) |
											   HDMI_HDCP_AUTH_authcompute(0) |
											   HDMI_HDCP_AUTH_write_en1(0) |
											   HDMI_HDCP_AUTH_authrequest(0));
					
	if( (status = hdcp_lib_generateKm(ksv_data))!= HDCP_OK )
		return status;
	

	/* disable Ri update interrupt */
	 WR_REG_32(hdcp.hdcp_base_addr,HDMI_INTEN, HDMI_INTEN_enriupdint(1) |
										  HDMI_INTEN_enpjupdint(0) |
										  HDMI_INTEN_write_data(0));		
						 
	/* clear Ri updated pending bit */
	 WR_REG_32(hdcp.hdcp_base_addr,HDMI_INTST, HDMI_INTST_riupdated(0) |
										  HDMI_INTST_pjupdated(0));
				
	/* trigger hdcpBlockCipher at authentication */	
	 WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(0) |
											   HDMI_HDCP_AUTH_ddpken(0) |
											   HDMI_HDCP_AUTH_write_en8(0) |
											   HDMI_HDCP_AUTH_resetkmacc(0) |
											   HDMI_HDCP_AUTH_write_en7(0) |
											   HDMI_HDCP_AUTH_update_an(0) |
											   HDMI_HDCP_AUTH_write_en6(0) |
											   HDMI_HDCP_AUTH_aninfreq(0) |
											   HDMI_HDCP_AUTH_write_en5(0) |
											   HDMI_HDCP_AUTH_seedload(0) |
											   HDMI_HDCP_AUTH_write_en4(0) |
											   HDMI_HDCP_AUTH_deviceauthenticated(0) |
											   HDMI_HDCP_AUTH_write_en3(0) |
											   HDMI_HDCP_AUTH_forcetounauthenticated(0) |
											   HDMI_HDCP_AUTH_write_en2(1) |
											   HDMI_HDCP_AUTH_authcompute(1) |
											   HDMI_HDCP_AUTH_write_en1(0) |
											   HDMI_HDCP_AUTH_authrequest(0));
	return status;								
}
/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_initiate_step1
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_initiate_step1(void)
{

        /* HDCP authentication steps:
         *   1) Read Bksv - check validity (is HDMI Rx supporting HDCP ?)
         *   2) Initializes HDCP (CP reset release)
         *   3) Read Bcaps - is HDMI Rx a repeater ?
         *   *** First part authentication ***
         *   4) Read Bksv - check validity (is HDMI Rx supporting HDCP ?)
         *   5) Generates An
         *   6) DDC: Writes An, Aksv
         *   7) DDC: Write Bksv
         */
		int status = HDCP_OK,i;
        uint8_t an_ksv_data[5], an_bksv_data[5];
        uint8_t rx_type;
		uint8_t genAn[8];
				
		//HDCP_DEBUG("[%s] %s  %d :%u\n", __FILE__,__FUNCTION__,__LINE__,jiffies_to_msecs(jiffies));
#if(USED_TA_CMD)

		 /* Generate An */
        if((status = ta_hdcp_lib_generate_an(genAn))!= HDCP_OK)
			return status;
#else
		 /* Generate An */
        if((status = hdcp_lib_generate_an(genAn))!= HDCP_OK)
			return status;
#endif
		HDCP_DEBUG("AN: %x %x %x %x %x %x %x %x", genAn[0], genAn[1], genAn[2], genAn[3],
												  genAn[4], genAn[5], genAn[6], genAn[7]);
	
		if((status =hdcp_lib_read_aksv(an_ksv_data))!= HDCP_OK)
			return status;
			
        HDCP_DEBUG("AKSV: %02x %02x %02x %02x %02x", an_ksv_data[0], an_ksv_data[1], an_ksv_data[2], 
													 an_ksv_data[3],an_ksv_data[4]);

        if (hdcp_lib_check_ksv(an_ksv_data)) {
                HDCP_ERROR("AKSV error (number of,0 and 1)\n");
                HDCP_ERROR("AKSV: %02x %02x %02x %02x %02x",
                       an_ksv_data[0], an_ksv_data[1],an_ksv_data[2], an_ksv_data[3], an_ksv_data[4]);
                return -HDCP_AKSV_ERROR;
        }	
		memcpy(ksvlist_info.Aksv,an_ksv_data,sizeof(an_ksv_data));
		
		/* DDC: Write An */
        if (ddc_write(DDC_AN_LEN, DDC_AN_ADDR ,genAn))	
                return -DDC_ERROR;
						
		/* DDC: Write AKSV */
        if (ddc_write(DDC_AKSV_LEN, DDC_AKSV_ADDR, an_ksv_data))
                return -DDC_ERROR;
		
        /* Read BCAPS to determine if HDCP RX is a repeater */
        if (ddc_read(DDC_BCAPS_LEN, DDC_BCAPS_ADDR, &rx_type))
                return -DDC_ERROR;


        rx_type = FLD_GET(rx_type, DDC_BIT_REPEATER, DDC_BIT_REPEATER);

        /* Set repeater bit in HDCP CTRL */
        if (rx_type == 1) {
            #if(USED_TA_CMD)
                ta_hdcp_lib_set_repeater_bit_in_tx(HDCP_REPEATER);
            #else
                hdcp_lib_set_repeater_bit_in_tx(HDCP_REPEATER);
            #endif
                HDCP_DEBUG("HDCP RX is a repeater");
        } else {
            #if(USED_TA_CMD)
                ta_hdcp_lib_set_repeater_bit_in_tx(HDCP_RECEIVER);
            #else
                hdcp_lib_set_repeater_bit_in_tx(HDCP_RECEIVER);
            #endif
                HDCP_DEBUG("HDCP RX is a receiver");
        }
		
        /* DDC: Read BKSV from RX */
		for(i=0;i<2;i++) // retry for CTS 1A-05.
		{
			if (ddc_read(DDC_BKSV_LEN, DDC_BKSV_ADDR , an_bksv_data)&& i==1)
				return -DDC_ERROR;			
			else
			{
				if (hdcp_lib_check_ksv(an_bksv_data) && i==1) {                
					HDCP_ERROR("BKSV: %02x %02x %02x %02x %02x", an_bksv_data[0], an_bksv_data[1],an_bksv_data[2], 
														an_bksv_data[3], an_bksv_data[4]);
					return -HDCP_AKSV_ERROR;
				}
			}
		}

		memcpy(ksvlist_info.Bksv,an_bksv_data,sizeof(an_bksv_data));

		HDCP_DEBUG("BKSV: %02x %02x %02x %02x %02x", an_bksv_data[0], an_bksv_data[1],an_bksv_data[2], 
													 an_bksv_data[3], an_bksv_data[4]);

                    HDCP_DEBUG("PK: %02x %02x %02x %02x %02x", hdcp.en_ctrl->PK[0], hdcp.en_ctrl->PK[1],hdcp.en_ctrl->PK[2], 
													 hdcp.en_ctrl->PK[3], hdcp.en_ctrl->PK[4]);
#if(USED_TA_CMD)
        /* Authentication 1st step initiated HERE */
		status = ta_hdcp_lib_write_bksv(an_bksv_data, hdcp.en_ctrl->PK);
#else
        /* Authentication 1st step initiated HERE */
		status = hdcp_lib_write_bksv(an_bksv_data);
#endif
				        
        return status;	
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_step1_start
 *-----------------------------------------------------------------------------
 */
int hdcp_lib_step1_start(void)
{
        int status;
		
        //HDCP_DEBUG("[%s] %s  %d :%u\n", __FILE__,__FUNCTION__,__LINE__,jiffies_to_msecs(jiffies));

#if 0
        /* Set AV Mute if needed */
        if (hdcp.av_mute_needed)
                hdcp_lib_set_av_mute(AV_MUTE_SET);

        /* Must turn encryption off when AVMUTE */
        hdcp_lib_set_encryption(HDCP_ENC_OFF);
#endif		
        status = hdcp_lib_initiate_step1();
	
     // if (ddc.pending_disable)
       //         return -HDCP_CANCELLED_AUTH;
       // else
                return status;

}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_r0_check
 *----------------------------------------------------------------------------
 */
int hdcp_lib_r0_check(void)
{	
		uint8_t ro_rx[2]={0,0}, ro_tx[2]={0,0};
		

		/* DDC: Read Ri' from RX */
		if (ddc_read(DDC_Ri_LEN, DDC_Ri_ADDR , ro_rx))
			return -DDC_ERROR;
#if(USED_TA_CMD) //modify for kylin B walkaround
		return ta_hdcp_check_r0(ro_rx);
#else
		ro_tx[0] = (RD_REG_32( hdcp.hdcp_base_addr , HDMI_HDCP_LIR )>>8)&0xff;
		ro_tx[1] = (RD_REG_32( hdcp.hdcp_base_addr , HDMI_HDCP_LIR )>>16)&0xff;	
				
		/* Compare values */
        if ((ro_rx[0] == ro_tx[0]) && (ro_rx[1] == ro_tx[1]))
		{		
			HDCP_DEBUG("ROTX: %x%x RORX:%x%x\n", ro_tx[0], ro_tx[1], ro_rx[0], ro_rx[1]);
            return HDCP_OK;
		}		
        else
		{		
			HDCP_ERROR("ROTX: %x%x RORX:%x%x\n", ro_tx[0], ro_tx[1], ro_rx[0], ro_rx[1]);	
            return -HDCP_AUTH_FAILURE;
		}						
#endif
}


/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_set_encryption
 *-----------------------------------------------------------------------------
 */
void hdcp_lib_set_encryption(enum encryption_state enc_state)
{
        //unsigned long flags;
		//int delay = 50000; //to fix #31554
		int rx_mode = hdcp_lib_check_repeater_bit_in_tx();  
	
       // spin_lock_irqsave(&hdcp.spinlock, flags);
		if(enc_state == HDCP_ENC_ON)
		{
			/* set HDCP module to authenticated state */
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(0) |
													  HDMI_HDCP_AUTH_ddpken(0) |
													  HDMI_HDCP_AUTH_write_en8(0) |
													  HDMI_HDCP_AUTH_resetkmacc(0) |
													  HDMI_HDCP_AUTH_write_en7(0) |
													  HDMI_HDCP_AUTH_update_an(0) |
													  HDMI_HDCP_AUTH_write_en6(0) |
													  HDMI_HDCP_AUTH_aninfreq(0) |
													  HDMI_HDCP_AUTH_write_en5(0) |
													  HDMI_HDCP_AUTH_seedload(0) |
													  HDMI_HDCP_AUTH_write_en4(1) |
													  HDMI_HDCP_AUTH_deviceauthenticated(1) |
													  HDMI_HDCP_AUTH_write_en3(0) |
													  HDMI_HDCP_AUTH_forcetounauthenticated(0) |
													  HDMI_HDCP_AUTH_write_en2(0) |
													  HDMI_HDCP_AUTH_authcompute(0) |
													  HDMI_HDCP_AUTH_write_en1(0) |
													  HDMI_HDCP_AUTH_authrequest(0));
			
			/* start encryption */				
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR, HDMI_HDCP_CR_write_en4(1) |
													HDMI_HDCP_CR_en1_1_feature(1) |//disable
													HDMI_HDCP_CR_write_en3(0) |
													HDMI_HDCP_CR_downstrisrepeater(rx_mode) |
													HDMI_HDCP_CR_write_en2(0) |
													HDMI_HDCP_CR_aninfluencemode(0) |
													HDMI_HDCP_CR_write_en1(1) |
													HDMI_HDCP_CR_hdcp_encryptenable(1));
		}
		else if(enc_state == HDCP_ENC_OFF)
		{				
			/* disable link integry check */		
			WR_REG_32(hdcp.hdcp_base_addr,HDMI_INTEN, HDMI_INTEN_enriupdint(1) |
										         HDMI_INTEN_enpjupdint(1) |
										         HDMI_INTEN_write_data(0));	
	
			//usleep(delay);
			/* force Encryption disable */	
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR, HDMI_HDCP_CR_write_en4(1) |
								 HDMI_HDCP_CR_en1_1_feature(1) |//disable
								 HDMI_HDCP_CR_write_en3(0) |
								 HDMI_HDCP_CR_downstrisrepeater(rx_mode) |
								 HDMI_HDCP_CR_write_en2(0) |
								 HDMI_HDCP_CR_aninfluencemode(0) |
								 HDMI_HDCP_CR_write_en1(1) |
								 HDMI_HDCP_CR_hdcp_encryptenable(0));	
				

			//usleep(delay);
			/* force HDCP module to unauthenticated state */
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH, HDMI_HDCP_AUTH_write_en9(0) |
								 HDMI_HDCP_AUTH_ddpken(0) |
								 HDMI_HDCP_AUTH_write_en8(0) |
								 HDMI_HDCP_AUTH_resetkmacc(0) |
								 HDMI_HDCP_AUTH_write_en7(0) |
								 HDMI_HDCP_AUTH_update_an(0) |
								 HDMI_HDCP_AUTH_write_en6(0) |
								 HDMI_HDCP_AUTH_aninfreq(0) |
								 HDMI_HDCP_AUTH_write_en5(0) |
								 HDMI_HDCP_AUTH_seedload(0) |
								 HDMI_HDCP_AUTH_write_en4(0) |
								 HDMI_HDCP_AUTH_deviceauthenticated(0) |
								 HDMI_HDCP_AUTH_write_en3(1) |
								 HDMI_HDCP_AUTH_forcetounauthenticated(1) |
								 HDMI_HDCP_AUTH_write_en2(0) |
								 HDMI_HDCP_AUTH_authcompute(0) |
								 HDMI_HDCP_AUTH_write_en1(0) |
								 HDMI_HDCP_AUTH_authrequest(0));
							
		}
		else
			HDCP_ERROR("unknown ENC state \n");
        
		//spin_unlock_irqrestore(&hdcp.spinlock, flags);

        if (hdcp.print_messages)		
                HDCP_DEBUG("Encryption state changed: %s HDCP_AUTH_reg: %02x",
                                enc_state == HDCP_ENC_OFF ? "OFF" : "ON",
                                RD_REG_32(hdcp.hdcp_base_addr ,HDMI_HDCP_AUTH));
								
}


/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_auto_ri_check
 *-----------------------------------------------------------------------------
 */
void hdcp_lib_auto_ri_check(bool state)
{

        //unsigned long flags;	
		void *irq_dev_id = (void *)& hdcp.hdcp_irq_num;

		//spin_lock_irqsave(&hdcp.spinlock, flags);
		
		if(state == true)
		{
                                #if(USED_TA_CMD)
                                        ta_hdcp_lib_set_wider_window();
                                #else
			            hdcp_lib_set_wider_window();
                                #endif
			
			if (request_irq( hdcp.hdcp_irq_num, HDCP_interrupt_handler,IRQF_SHARED, "rtk_hdcp",irq_dev_id))
				HDCP_ERROR("request HDCP irq failed\n");
			else
			{
				HDCP_DEBUG("request HDCP irq %d\n",hdcp.hdcp_irq_num);


                                 #if(USED_TA_CMD)
                                        ta_hdcp_lib_set_ri(HDCP_RI_ON);
                                 #else
				WR_REG_32(hdcp.hdcp_base_addr,HDMI_INTEN, HDMI_INTEN_enriupdint(1) |
													 HDMI_INTEN_enpjupdint(0) |
													 HDMI_INTEN_write_data(1));		
                                 #endif
			}										 
		}
		else
		{
		             #if(USED_TA_CMD)
                                     ta_hdcp_lib_set_ri(HDCP_RI_OFF);
                                 #else
			WR_REG_32(hdcp.hdcp_base_addr,HDMI_INTEN, HDMI_INTEN_enriupdint(0) |
												 HDMI_INTEN_enpjupdint(0) |
												 HDMI_INTEN_write_data(1));
                                 #endif
		}
				
    	//spin_unlock_irqrestore(&hdcp.spinlock, flags);
		  
		
		HDCP_DEBUG("[%s] %s  %d :%u ,state=%s\n", __FILE__,__FUNCTION__,__LINE__,jiffies_to_msecs(jiffies),state == true ? "ON" : "OFF");
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_polling_bcaps_rdy_check
 *-----------------------------------------------------------------------------
 */
int hdcp_lib_polling_bcaps_rdy_check(void) 
{
	int i;
	int ret_value;
	int hotplug_state;
	uint8_t ready_bit;

	ret_value = -HDCP_AUTH_FAILURE;
	/*
	* State A8: Wait for Ready
	* sets up 5 secs timer and polls the Receiver's READY bit
	*/
	for (i = 0; i < 50; i++) {
		ready_bit = 0;
		if(ddc_read(DDC_BCAPS_LEN, DDC_BCAPS_ADDR, &ready_bit))
			HDCP_ERROR("I2C error, read Bcaps failed\n");

		if (FLD_GET(ready_bit, DDC_BIT_READY, DDC_BIT_READY)) {
			HDCP_DEBUG("Bcaps ready bit asserted\n");
			ret_value = HDCP_OK;
			goto ret_check;
		} else {
			hotplug_state = hdmitx_switch_get_state();
			if (hotplug_state == 1)
				usleep_range(100000, 110000);
			else
				goto ret_check;
		}
	}

ret_check:
	return ret_value;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_step1_r0_check
 *-----------------------------------------------------------------------------
 */
int hdcp_lib_step1_r0_check(void)
{
        int status = HDCP_OK;

        /* HDCP authentication steps:
         *   1) DDC: Read M0'
         *   2) Compare M0 and M0'
         *   if Rx is a receiver: switch to authentication step 3
         *   3) Enable encryption / auto Ri check / disable AV mute
         *   if Rx is a repeater: switch to authentication step 2
         *   3) Get M0 from HDMI IP and store it for further processing (V)
         *   4) Enable encryption / auto Ri check / auto BCAPS RDY polling
         *      Disable AV mute
         */

		//HDCP_DEBUG("[%s] %s  %d :%u\n", __FILE__,__FUNCTION__,__LINE__,jiffies_to_msecs(jiffies)); 
     
        status = hdcp_lib_r0_check();
        if (status < 0)
            return status;

        /* Authentication 1st step done */

        /* Now prepare 2nd step authentication in case of RX repeater and
         * enable encryption / Ri check
         */

    //    if (ddc.pending_disable)
    //            return -HDCP_CANCELLED_AUTH;

        if (!hdcp_lib_check_repeater_bit_in_tx()) 
		{

        #if(USED_TA_CMD)
            /* Receiver: enable encryption and auto Ri check */
            ta_hdcp_lib_set_encryption(HDCP_ENC_ON);
        #else
            /* Receiver: enable encryption and auto Ri check */
            hdcp_lib_set_encryption(HDCP_ENC_ON);
        #endif
            
            /* Enable Auto Ri */
            hdcp_lib_auto_ri_check(true);
        }

        /* Clear AV mute */
        //hdcp_lib_set_av_mute(AV_MUTE_CLEAR);

        return status;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_dump_sha
 *-----------------------------------------------------------------------------
 */
 static void hdcp_lib_dump_sha(struct hdcp_sha_in *sha)
 {
		int i;
		
		printk("sha.byte_counter= %d\n",sha->byte_counter);
		
		printk("sha.vprime=[ ");
		for(i=0;i< MAX_SHA_VPRIME_SIZE;i++)
		{			
			printk("0x%x,",sha->vprime[i]);
		}
		printk("]\n");
		printk("sha.data=[ ");
		for(i=0;i< sha->byte_counter;i++)
		{			
			printk("0x%x,",sha->data[i]);
		}
		printk("]\n");
 }

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_SHA_append_bstatus_M0
 *-----------------------------------------------------------------------------
 */
static void hdcp_lib_SHA_append_bstatus_M0(struct hdcp_sha_in *sha ,uint8_t *data)
{
		uint32_t MILSW ,MIMSW ;
		    
		sha->data[sha->byte_counter++] = data[0];
        sha->data[sha->byte_counter++] = data[1];
			
		MILSW = RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_MILSW);
		MIMSW = RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_MIMSW);
		        
        memcpy( (void *)&sha->data[sha->byte_counter], &MILSW, sizeof(MILSW));
		sha->byte_counter += sizeof(MILSW);
		memcpy( (void *)&sha->data[sha->byte_counter], &MIMSW, sizeof(MIMSW));
        sha->byte_counter += sizeof(MIMSW);

        return ;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_verify_V
 *-----------------------------------------------------------------------------
 */
static void hdcp_lib_dump_SHA_block(uint8_t *sha_blk)
{
	int i;
	
	for(i=0;i<SHA_BLK_SIZE/4;i++)
	{
		printk("sha_blk[%2d,%2d,%2d,%2d]= %2x, %2x, %2x, %2x,\n",4*i+0,4*i+1,4*i+2,4*i+3,sha_blk[4*i+0],sha_blk[4*i+1],sha_blk[4*i+2],sha_blk[4*i+3]);
	}

}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_verify_V
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_compute_V(struct hdcp_sha_in *sha)
{
		int blk_num =0;
		uint8_t sha_blk[64];
		uint32_t sha_wd;
		int i,j,retry =0;
		int sha_no_padding_len=(sha->byte_counter/32);
		int sha_data_len_in_bit =sha->byte_counter*8;
		
		//calculate SHA data(w/o padding) length in byte. 	
		if (sha->byte_counter &(32-1)) 
		    sha_no_padding_len ++;
		
		//padding?	
		sha->data[sha->byte_counter++] =0x80; 
		sha->data[sha->byte_counter++] =0x0;  
		
		//total length= Ksv list + Bstatus + M0 + padding*2 + SHA data len(bytes)	
		blk_num = (sha->byte_counter + sha_no_padding_len) /SHA_BLK_SIZE;						
		if ((sha->byte_counter + sha_no_padding_len) &(SHA_BLK_SIZE-1)) 
		    blk_num ++;
			
			
		for(j=0;j<blk_num;j++)
		{			
			memset( sha_blk, 0, SHA_BLK_SIZE );
			memcpy( sha_blk, (void *)&sha->data[SHA_BLK_SIZE*j], SHA_BLK_SIZE);
			
			if(j== blk_num -1) //fill data length(bits) of the last blk.
			{					
				for(i=0;i<sha_no_padding_len;i++)
				{
					sha_blk[SHA_BLK_SIZE-1-i]= sha_data_len_in_bit % 256;
					sha_data_len_in_bit/=256;
				}
			}
		
			//hdcp_lib_dump_SHA_block(sha_blk);
		
			WR_REG_32(hdcp.hdcp_base_addr , HDMI_HDCP_SHACR, HDMI_HDCP_SHACR_shastart(0) |
														HDMI_HDCP_SHACR_shafirst(0) |
														HDMI_HDCP_SHACR_rstshaptr(1) |
														HDMI_HDCP_SHACR_write_data(1));//reset pointer
			for(i=0;i<16;i++) //16 = SHA_BLK_SIZE /word
			{				
				sha_wd = (sha_blk[4*i+0]<<24)|(sha_blk[4*i+1]<<16)|(sha_blk[4*i+2]<<8) |(sha_blk[4*i+3]<<0);  //change endian.
				HDCP_DEBUG("sha_blk[%2d,%2d,%2d,%2d]= %2x %2x %2x %2x, sha_wd=%x\n",4*i+0,4*i+1,4*i+2,4*i+3,sha_blk[4*i+0],sha_blk[4*i+1],sha_blk[4*i+2],sha_blk[4*i+3],sha_wd);
				WR_REG_32(hdcp.hdcp_base_addr , HDMI_HDCP_SHADR, HDMI_HDCP_SHADR_sha_data(sha_wd));//push first 512bits data				
			}
			
			if(j==0)
			{
				WR_REG_32(hdcp.hdcp_base_addr , HDMI_HDCP_SHACR, HDMI_HDCP_SHACR_shastart(1) |
															HDMI_HDCP_SHACR_shafirst(1) |
															HDMI_HDCP_SHACR_rstshaptr(0) |
															HDMI_HDCP_SHACR_write_data(1));//set first and start
			}
			else
			{
				WR_REG_32(hdcp.hdcp_base_addr , HDMI_HDCP_SHACR, HDMI_HDCP_SHACR_shastart(0) |
															HDMI_HDCP_SHACR_shafirst(1) |
															HDMI_HDCP_SHACR_rstshaptr(0) |
															HDMI_HDCP_SHACR_write_data(0));//reset first bit
				
				WR_REG_32(hdcp.hdcp_base_addr , HDMI_HDCP_SHACR, HDMI_HDCP_SHACR_shastart(1) |
															HDMI_HDCP_SHACR_shafirst(0) |
															HDMI_HDCP_SHACR_rstshaptr(0) |
															HDMI_HDCP_SHACR_write_data(1));//set start
			}
		}
		
		while(! HDMI_HDCP_SHARR_get_shaready(RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_SHARR)))	// wait for ready
		{
			mdelay(100);   //Wait 0.1 sec
			if(retry == 20)
			{	
				HDCP_ERROR("2rd step authentication : compute V failed\n");
				return -HDCP_SHA1_ERROR;
			}	
			retry ++;
		}
	
		HDCP_INFO("2rd step authentication : compute V is done\n");
		
		return HDCP_OK;
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_verify_V
 *-----------------------------------------------------------------------------
 */
static int hdcp_lib_verify_V(struct hdcp_sha_in *sha)
{
		int i,retry=0;
		uint32_t data;
		
		for(i=0;i<SHA1_HASH_SIZE ;i++)
		{
			memcpy( &data, (void *)&sha->vprime[DDC_V_LEN*i], sizeof(data)); 
			HDCP_DEBUG("2rd step authentication : v data=%x\n",data);
			//data = htonl(data);  //should not swap!
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_SHADR, HDMI_HDCP_SHADR_sha_data(data));
			
		}
			
		while(!HDMI_HDCP_SHARR_get_shaready(RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_SHARR)))	// wait for ready
		{
			mdelay(100);   //Wait 0.1 sec
			if(retry == 30)
			{	
				HDCP_ERROR("2rd step authentication : timeout of shaready;verify V failed\n");				
				return -HDCP_SHA1_ERROR;
			}	
			retry ++;					
		}
		
			
		if(HDMI_HDCP_SHARR_get_vmatch(RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_SHARR)))
		{
			HDCP_INFO("2rd step authentication : verify V passed \n");
			return HDCP_OK;
		}
		else
		{					
			HDCP_ERROR("2rd step authentication : verify V failed\n");				
			return -HDCP_SHA1_ERROR;				
		}
		
}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_step2
 *-----------------------------------------------------------------------------
 */
int hdcp_lib_step2(void)
{

        /* HDCP authentication steps:
         *   1) Disable auto Ri check
         *   2) DDC: read BStatus (nb of devices, MAX_DEV
         */

        uint8_t bstatus[2]={0};
        int status = HDCP_OK;
        int i;

        HDCP_DEBUG("[%s] %s  %d :%u\n", __FILE__,__FUNCTION__,__LINE__,jiffies_to_msecs(jiffies));

        if((status=hdcp_lib_polling_bcaps_rdy_check())!= HDCP_OK)
		{
			HDCP_ERROR("polling Bcaps ready failed\n");
#ifndef CONFIG_RTK_HDCP1x_REPEATER
			return status;
#endif
		}

        /* DDC: Read Bstatus (1st byte) from Rx */
        if (ddc_read(DDC_BSTATUS_LEN, DDC_BSTATUS_ADDR, bstatus))
                return -DDC_ERROR;
        
        HDCP_DEBUG("bstatus=0x%x,0x%x\n",bstatus[0],bstatus[1]);       

#ifdef CONFIG_RTK_HDCP1x_REPEATER
		HdmiRx_set_bstatus(bstatus[1],bstatus[0]);

		if(status!=HDCP_OK)
			return status;
#endif

	   /* Check BStatus topology errors */
        if (bstatus[0] & DDC_BSTATUS0_MAX_DEVS) {
                HDCP_ERROR("MAX_DEV_EXCEEDED set");
                return -HDCP_AUTH_FAILURE;
        }

        if (bstatus[1] & DDC_BSTATUS1_MAX_CASC) {
                HDCP_ERROR("MAX_CASCADE_EXCEEDED set");
                return -HDCP_AUTH_FAILURE;
        }

        HDCP_DEBUG("Retrieving KSV list...");
		/* Get KSV list size */
        sha_input.byte_counter = (bstatus[0] & DDC_BSTATUS0_DEV_COUNT) * 5;

        /* Clear all SHA input data */
        /* TODO: should be done earlier at HDCP init */
        memset(sha_input.data,0, MAX_SHA_DATA_SIZE);

     //   if (ddc.pending_disable)
     //           return -HDCP_CANCELLED_AUTH;

        /* DDC: read KSV list */
        if (sha_input.byte_counter) {
                if (ddc_read(sha_input.byte_counter, DDC_KSV_FIFO_ADDR,(uint8_t *)&sha_input.data))
                        return -DDC_ERROR;
				else{				
					ksvlist_info.device_count = (bstatus[0] & DDC_BSTATUS0_DEV_COUNT);
					memcpy(ksvlist_info.ksvlist,sha_input.data, sizeof(ksvlist_info.ksvlist));    																					
					memcpy(ksvlist_info.bstatus,bstatus, sizeof(ksvlist_info.bstatus));													
#ifdef CONFIG_RTK_HDCP1x_REPEATER
                    HdmiRx_save_tx_ksv(&ksvlist_info);//Send KSV list to RX immediately for Repeater CTS 3C-II
#endif
				}					
        }

        /* Read and add Bstatus */
		//hdcp_lib_SHA_append_bstatus_M0( &sha_input, &bstatus);

        #if(USED_TA_CMD)
                    ta_hdcp_lib_SHA_append_bstatus_M0( &sha_input, bstatus);
        #else
		hdcp_lib_SHA_append_bstatus_M0( &sha_input, bstatus);
        #endif
		
       // if (ddc.pending_disable)
         //       return -HDCP_CANCELLED_AUTH;

        /* Read V' */
        for(i=0;i<5;i++)
		{
			if (ddc_read(DDC_V_LEN, DDC_V_ADDR+DDC_V_LEN*i, sha_input.vprime+DDC_V_LEN*i))
					return -DDC_ERROR;
			
			HDCP_DEBUG("sha_input.vprime[%d]=%x,%x,%x,%x\n",DDC_V_LEN*i,sha_input.vprime[DDC_V_LEN*i+0],
																	    sha_input.vprime[DDC_V_LEN*i+1],	
																	    sha_input.vprime[DDC_V_LEN*i+2],	
																	    sha_input.vprime[DDC_V_LEN*i+3]);		
		}				
		//hdcp_lib_dump_sha(&sha_input);		

       // if (ddc.pending_disable)
        //        return -HDCP_CANCELLED_AUTH;

        #if(USED_TA_CMD)
		if( (status = ta_hdcp_lib_compute_V (&sha_input))!= HDCP_OK )
			goto sha_err;
		
		status = ta_hdcp_lib_verify_V(&sha_input);	

        #else
        
		if( (status = hdcp_lib_compute_V (&sha_input))!= HDCP_OK )
			goto sha_err;
		
		status = hdcp_lib_verify_V(&sha_input);	

        #endif
			
        if (status == HDCP_OK) {
                /* Re-enable Ri check */

        #if(USED_TA_CMD)
            /* Receiver: enable encryption and auto Ri check */
            ta_hdcp_lib_set_encryption(HDCP_ENC_ON);
        #else
                hdcp_lib_set_encryption(HDCP_ENC_ON);
        #endif
                hdcp_lib_auto_ri_check(true);
#ifdef CONFIG_RTK_HDCP1x_REPEATER
				HdmiRx_save_tx_ksv(&ksvlist_info);
#endif
        }

sha_err:
        return status;

}

/*-----------------------------------------------------------------------------
 * Function: hdcp_lib_set_av_mute
 *-----------------------------------------------------------------------------
 */
void hdcp_lib_set_av_mute(enum av_mute av_mute_state)
{
        //HDCP_DEBUG("[%s] %s  %d av_mute_state= %d \n", __FILE__,__FUNCTION__,__LINE__,av_mute_state);

        //spin_lock_irqsave(&hdcp.spinlock, flags);
		
		if (av_mute_state == AV_MUTE_SET)
		{		
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_GCPCR, HDMI_GCPCR_enablegcp(1) |
												  HDMI_GCPCR_gcp_clearavmute(1) |
												  HDMI_GCPCR_gcp_setavmute(1) |
												  HDMI_GCPCR_write_data(0));
												  
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_GCPCR, HDMI_GCPCR_enablegcp(1) |
												  HDMI_GCPCR_gcp_clearavmute(0) |
												  HDMI_GCPCR_gcp_setavmute(1) |
												  HDMI_GCPCR_write_data(1));
										
		}		
		else if (av_mute_state == AV_MUTE_CLEAR)
		{			
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_GCPCR, HDMI_GCPCR_enablegcp(1) |
												  HDMI_GCPCR_gcp_clearavmute(1) |
												  HDMI_GCPCR_gcp_setavmute(1) |
												  HDMI_GCPCR_write_data(0));
										
			WR_REG_32(hdcp.hdcp_base_addr, HDMI_GCPCR, HDMI_GCPCR_enablegcp(1) |
												  HDMI_GCPCR_gcp_clearavmute(1) |
												  HDMI_GCPCR_gcp_setavmute(0) |
												  HDMI_GCPCR_write_data(1));								
		}else
			HDCP_ERROR("unknown av mute parameter\n");
       
		 //spin_unlock_irqrestore(&hdcp.spinlock, flags);
		return ;
 
}

static void hdcp_lib_set_wider_window(void)
{	
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_OWR, HDMI_HDCP_OWR_write_en2(1) |
											 HDMI_HDCP_OWR_hdcpoppwinend(526) |
											 HDMI_HDCP_OWR_write_en1(1) |
											 HDMI_HDCP_OWR_hdcpoppwinstart(510));
}

int hdcp_lib_query_sink_hdcp_capable(void)
{
	int i;
	int retry;
	int ret_value;
	int hotplug_state;
	uint8_t bcaps_data[DDC_BCAPS_LEN];

	retry = 6;
	ret_value = -HDCP_AKSV_ERROR;

	/* HDCP CTS 1A-04: Continue to read the HDCP port for 9 seconds */
	for (i = 0; i < retry; i++) {
		if (ddc_read(DDC_BCAPS_LEN, DDC_BCAPS_ADDR, bcaps_data)) {
			HDCP_ERROR("Read BCAPS error %d time(s)\n",i);
		} else {
			ret_value = HDCP_OK;
			goto ret_cap;
		}

		hotplug_state = hdmitx_switch_get_state();
		if (hotplug_state == 1)
			msleep(1500);
		else
			goto ret_cap;
	}

ret_cap:
	return ret_value;
}				
	
void hdcp_lib_dump_22_cipher_setting(void)
{
	
	printk("HDCP_2_2_HW_RIV_1=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_RIV_1));
	printk("HDCP_2_2_HW_RIV_2=0x08%x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_RIV_2));	
    
	printk("HDCP_2_2_SW_KEY_1_1=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_1));
	printk("HDCP_2_2_SW_KEY_1_2=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_2));
	printk("HDCP_2_2_SW_KEY_1_3=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_3));	
	printk("HDCP_2_2_SW_KEY_1_4=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_4));	
	
	printk("HDCP_2_2_HW_FRAME_NUM_1=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_1));	
	printk("HDCP_2_2_HW_FRAME_NUM_2=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_2));
	
	printk("HDCP_2_2_HW_FRAME_NUM_ADD_1=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_ADD_1));
	printk("HDCP_2_2_HW_FRAME_NUM_ADD_2=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_ADD_2));
	
	printk("HDCP_2_2_HW_DATA_NUM=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_DATA_NUM));
	
	printk("HDCP_2_2_HW_DATA_NUM_ADD=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_DATA_NUM_ADD));
	
	printk("HDCP_2_2_CTRL=0x%08x\n",RD_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_CTRL));
	
}

void hdcp_lib_set_1x_state_machine_for_22_cipher(void)
{
	volatile int i;
	
	// set HDCP 1.4 state machine
    WR_REG_32(hdcp.hdcp_base_addr, HDMI_CR, HDMI_HDCP_CR_write_en2(1)|HDMI_HDCP_CR_enablehdcp(1));
    WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH,HDMI_HDCP_AUTH_write_en1(1)|HDMI_HDCP_AUTH_authrequest(1));
    for (i=0; i<2;i++);
    WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH,HDMI_HDCP_AUTH_write_en2(1)|HDMI_HDCP_AUTH_authcompute(1));
    for (i=0; i<2;i++);
    WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_AUTH,HDMI_HDCP_AUTH_write_en4(1)|HDMI_HDCP_AUTH_deviceauthenticated(1));
	
}

void hdcp_lib_set_22_cipher(HDCP_22_CIPHER_INFO *arg)
{
	if (arg->hdcp_22_en == 1)
		hdcp_lib_set_1x_state_machine_for_22_cipher();

	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_CTRL,
		HDMI_HDCP_2_2_CTRL_write_en4(1)|HDMI_HDCP_2_2_CTRL_hdcp_2_2_en(arg->hdcp_22_en));

	/* riv           : hw_riv_1_(1-2) */
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_RIV_1,
		arg->riv[0]|(arg->riv[1]<<8)|(arg->riv[2]<<16)|(arg->riv[3]<<24));
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_RIV_2,
		arg->riv[4]|(arg->riv[5]<<8)|(arg->riv[6]<<16)|(arg->riv[7]<<24));

	/* ks_xor_lc128  : sw_key_1_(1-4) */
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_1,
		(arg->ks_xor_lc128[0])|
		(arg->ks_xor_lc128[1]<<8)|
		(arg->ks_xor_lc128[2]<<16)|
		(arg->ks_xor_lc128[3]<<24));
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_2,
		(arg->ks_xor_lc128[4])|
		(arg->ks_xor_lc128[5]<<8)|
		(arg->ks_xor_lc128[6]<<16)|
		(arg->ks_xor_lc128[7]<<24));
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_3,
		(arg->ks_xor_lc128[8])|
		(arg->ks_xor_lc128[9]<<8)|
		(arg->ks_xor_lc128[10]<<16)|
		(arg->ks_xor_lc128[11]<<24));
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_SW_KEY_1_4,
		(arg->ks_xor_lc128[12])|
		(arg->ks_xor_lc128[13]<<8)|
		(arg->ks_xor_lc128[14]<<16)|
		(arg->ks_xor_lc128[15]<<24));

	/* frame_num     : frame_num_(1-2) */
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_1, (u32)arg->frame_num);
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_2, (u32)(arg->frame_num>>32));

	/* frame_num_add : frame_num_add_(1-2)	*/
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_ADD_1, (u32)arg->frame_num_add);

	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_FRAME_NUM_ADD_2, (u32)(arg->frame_num_add>>32));

	/* data_num      : data_num */
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_DATA_NUM, arg->data_num);

	/* data_num_add  : data_num_add */
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_HW_DATA_NUM_ADD, arg->data_num_add);

	/* aes_sw_en     : hdcp_22_ctrl:aes_sw_en */
	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_CTRL,
		HDMI_HDCP_2_2_CTRL_write_en3(1)|HDMI_HDCP_2_2_CTRL_aes_sw_en(1));/* always set to SW mode first */

	WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_2_2_CTRL,
		HDMI_HDCP_2_2_CTRL_write_en3(1)|HDMI_HDCP_2_2_CTRL_aes_sw_en(arg->aes_sw_en));

	/* hdcp_lib_dump_22_cipher_setting(); */

}

void hdcp_lib_control_22_cipher(int control_flag)
{
	if (control_flag == HDCP_22_CIPHER_RESUME) {
		/* Enable HDCP22 encrypt */
		WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR,
			HDMI_HDCP_CR_write_en4(1)|HDMI_HDCP_CR_en1_1_feature(1)|
			HDMI_HDCP_CR_write_en1(1)|HDMI_HDCP_CR_hdcp_encryptenable(1));
		HDCP_INFO("Enable 22 enc");
	} else {
		WR_REG_32(hdcp.hdcp_base_addr, HDMI_HDCP_CR,
			HDMI_HDCP_CR_write_en1(1)|HDMI_HDCP_CR_hdcp_encryptenable(0));
		HDCP_INFO("Disable 22 enc");
	}

}
