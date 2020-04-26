/************************************************************************
 *
 *  logo_disp_read_edid.c
 *
 *  get monitor mode (DVI/HDMI) from display device
 *
 *
 ************************************************************************/

/************************************************************************
 *  Include files
 ************************************************************************/
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <arch/ops.h>

#include <rtk_i2c-lib.h>
#include <platform/io.h>
#include <platform/gpio.h>
#include <platform/setting.h>
#include <platform/sb2.h>
#include <fw_info.h>
#include <factorylib.h>

#include "rtk_rpc.h"
#include "rtk_edid.h"
#include "rtk_avi_infoframe.h"

#define BOOT_TVSYSTEM_FILE_NAME		FACTORY_HEADER_FILE_NAME"video_rpc.bin"
#define BOOT_CHECKSUM_FILE_NAME		FACTORY_HEADER_FILE_NAME"edid_checksum.bin"

#define S_OK         0
#define S_FALSE     -1

#define EDID_LENGTH 128

#define HDMITX_I2C_ID 1 //I2C1

static unsigned int vo_hdmi_mode = VO_HDMI_ON;

unsigned char EDID[EDID_LENGTH*2];
uchar checksum_128=0;
uchar checksum_256=0;

struct edid_hdmi2p0_info hdmi2p0_info;


struct VIDEO_FORMAT_CONVERTER vid_support_list[]=
{	
	{VO_STANDARD_NTSC_J,0x06},
	{VO_STANDARD_NTSC_J,0x07},
	{VO_STANDARD_NTSC_J,0x02},
	{VO_STANDARD_NTSC_J,0x03},    //480
	{VO_STANDARD_PAL_I,0x15},
	{VO_STANDARD_PAL_I,0x16},
	{VO_STANDARD_PAL_I,0x11},
	{VO_STANDARD_PAL_I,0x12},    //576
	{VO_STANDARD_HDTV_720P_50,0x13},
	{VO_STANDARD_HDTV_720P_60,0x04},  //720
	{VO_STANDARD_HDTV_1080I_50,0x14},
	{VO_STANDARD_HDTV_1080I_60,0x05},  //1080i
	{VO_STANDARD_HDTV_1080P_24,0x20},
	{VO_STANDARD_HDTV_1080P_50,0x1f},
	{VO_STANDARD_HDTV_1080P_60,0x10}//1080p
};

/************************************************************************
**
** get video_rpc.bin in factory area of flash
**
*************************************************************************/
int get_one_step_info(void)
{
	char *dst_addr,*dst_addr_chk;
	int dst_length,dst_length_chk;

	//printf("%s:\n", __FUNCTION__);

	if (factory_read(BOOT_TVSYSTEM_FILE_NAME, &dst_addr, &dst_length)) {
		printf("------------can't find %s\n", BOOT_TVSYSTEM_FILE_NAME);
		//printf("dst_addr=%x\n, dst_length=%d\n",dst_addr,dst_length);
		return 0;
	} else {
		printf("------------%s found\n", BOOT_TVSYSTEM_FILE_NAME);
				
    	rtd_outl(VO_RESOLUTION, 0xee0bdec0); /* set magic pattern in first word */
		memcpy((unsigned char *)(VO_RESOLUTION+4), dst_addr ,dst_length);		
		arch_clean_cache_range(VO_RESOLUTION, 4+ dst_length);
		
		
		if (factory_read(BOOT_CHECKSUM_FILE_NAME, &dst_addr_chk, &dst_length_chk)) 
		{
			printf("------------can't find %s\n", BOOT_CHECKSUM_FILE_NAME);
			//printf("dst_addr=%x\n, dst_length=%d\n",dst_addr,dst_length);
			return 0;
		}
		else
		{				
			memcpy(&checksum_128, dst_addr_chk ,sizeof(unsigned char));
			memcpy(&checksum_256, dst_addr_chk+sizeof(unsigned char),sizeof(unsigned char));	
			if(checksum_128==0 && checksum_256==0)
				return 0;			
		}
		
		return 1;
	}

	return 0;
}


/************************************************************************
 *  Implementation : Global functions
 ************************************************************************/

void dump_VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM(struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM *arg) 
{
#if 0
	printf( "----------VOUT_CONFIG_TV_SYSTEM----------\n");
    printf( "interfaceType       =0x%x\n", arg->interfaceType);
 
	printf( "videoInfo.standard  =0x%x\n", arg->videoInfo.standard);
	printf( "videoInfo.enProg    =0x%x\n", arg->videoInfo.enProg);
	printf( "videoInfo.enDIF     =0x%x\n", arg->videoInfo.enDIF);
	printf( "videoInfo.enCompRGB =0x%x\n", arg->videoInfo.enCompRGB);
	printf( "videoInfo.pedType   =0x%x\n", arg->videoInfo.pedType);
	printf( "videoInfo.dataInt0  =0x%x\n", arg->videoInfo.dataInt0);
	printf( "videoInfo.dataInt1  =0x%x\n", arg->videoInfo.dataInt1);

	printf( "hdmiInfo.hdmiMode           =0x%x\n", arg->hdmiInfo.hdmiMode);
    printf( "hdmiInfo.audioSampleFreq    =0x%x\n", arg->hdmiInfo.audioSampleFreq);	
	printf( "hdmiInfo.audioChannelCount  =0x%x\n", arg->hdmiInfo.audioChannelCount);
	printf( "hdmiInfo.dataByte1          =0x%x\n", arg->hdmiInfo.dataByte1);
	printf( "hdmiInfo.dataByte2          =0x%x\n", arg->hdmiInfo.dataByte2);
	printf( "hdmiInfo.dataByte3          =0x%x\n", arg->hdmiInfo.dataByte3);
	printf( "hdmiInfo.dataByte4          =0x%x\n", arg->hdmiInfo.dataByte4);
	printf( "hdmiInfo.dataByte5          =0x%x\n", arg->hdmiInfo.dataByte5);		
	printf( "hdmiInfo.dataInt0           =0x%x\n", arg->hdmiInfo.dataInt0);
	printf( "hdmiInfo.hdmi2p0_feature    =0x%x\n",arg->hdmiInfo.hdmi2p0_feature);
	//printf( "hdmiInfo.reserved2          =0x%x\n",arg->hdmiInfo.reserved2);
	//printf( "hdmiInfo.reserved3          =0x%x\n",arg->hdmiInfo.reserved3);
	//printf( "hdmiInfo.reserved4          =0x%x\n",arg->hdmiInfo.reserved4);
	printf( "-----------------------------------------\n");
#endif
}

void dump_raw_edid(unsigned char *edid , int len)
{	
	int i;
	printf("RAW EDID:\n");
	
	for(i=1;i<=len;i++){	
					
		printf("0x%02x,",edid[i-1]);
			
			if(i%16==0)
				printf("\n");					
	}	  
}

int comp( const void * p, const void * q) 
{ 
    return ( * ( int * ) p - * ( int * ) q) ; 
}

int do_cea_modes (u8 *db, u8 len)
{
	u8 * mode; 	
	int i=0,j,k;
	int formats= sizeof(vid_support_list)/sizeof(struct VIDEO_FORMAT_CONVERTER);
	u_int *cea_mode = malloc(sizeof(int)*(int)len);
	
	for (mode = db; mode < db + len; mode++) {		
		cea_mode[i++] =(int)(*mode & 127); /* CEA modes are numbered 1..127 */			
	}

	qsort(cea_mode,len,sizeof(int),comp);
	
	printf("vid=");
	for(k=(int)len-1; k>=0; k--){
		printf("%d,",cea_mode[k]);
	}
	printf("\n");
				
	for(j=formats-1; j>=0; j--){			
		for(i=(int)len-1; i>=0; i--){
			if(cea_mode[i]== vid_support_list[j].vid){													
				free(cea_mode);
				return vid_support_list[j].standard;
			}
		}					
	}

	return S_FALSE;
	
}

static int cea_db_payload_len(const u8 *db)
{
	return db[0] & 0x1f;
}

static int cea_db_tag(const u8 *db)
{
	return db[0] >> 5;
}

int cea_revision(const u8 *cea)
{
	return cea[1];
}

int cea_db_offsets(const u8 *cea, int *start, int *end)
{
	/* Data block offset in CEA extension block */
	*start = 4;
	*end = cea[2];
	if (*end == 0)
		*end = 127;
	if (*end < 4 || *end > 127)
		return -1;
	return 0;
}

u8 *rtk_find_cea_extension(struct edid *edid)
{
	u8 *edid_ext = NULL;
	int i;

	/* No EDID or EDID extensions */
	if (edid == NULL || edid->extensions == 0)
		return NULL;

	/* Find CEA extension */
	for (i = 0; i < edid->extensions; i++) {
		edid_ext = (u8 *)edid + EDID_LENGTH * (i + 1);
		if (edid_ext[0] == CEA_EXT)		
			break;
	}

	if (i == edid->extensions)
		return NULL;

	return edid_ext;
}

static bool cea_db_is_hdmi_forum_vsdb(const u8 *db)
{
	int hdmi_id;

	if (cea_db_tag(db) != VENDOR_BLOCK)
		return false;

	if (cea_db_payload_len(db) < 7)
		return false;

	hdmi_id = db[1] | (db[2] << 8) | (db[3] << 16);

	hdmi2p0_info.hdmi_id = hdmi_id;

	return hdmi_id == HDMI_2P0_IDENTIFIER;
}

static void parse_hdmi_forum_vsdb(const u8 *db)
{
	hdmi2p0_info.max_tmds_char_rate = db[5]*5;
	hdmi2p0_info.scdc_capable = db[6];
	hdmi2p0_info.dc_420 = db[7]&0x7;
}

#define for_each_cea_db(cea, i, start, end) \
	for ((i) = (start); (i) < (end) && (i) + cea_db_payload_len(&(cea)[(i)]) < (end); (i) += cea_db_payload_len(&(cea)[(i)]) + 1)

int add_cea_modes(struct edid *edid)
{
	u8 * cea = rtk_find_cea_extension(edid);
	u8 * db, dbl;
	int modes = 0;

	if (cea && cea_revision(cea) >= 3) {
		int i, start, end;

		if (cea_db_offsets(cea, &start, &end))
			return 0;

		for_each_cea_db(cea, i, start, end) {
			db = &cea[i];
			dbl = cea_db_payload_len(db);

			switch (cea_db_tag(db)) {
				case VIDEO_BLOCK:
					modes = do_cea_modes (db+1, dbl);
					break;
				case VENDOR_BLOCK:
					if(cea_db_is_hdmi_forum_vsdb(db))//HDMI 2.0
						parse_hdmi_forum_vsdb(db);
					break;
				default:
					break;
			}
				
		}
	}

	return modes;
}

static const unsigned char edid_header[] = {
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00
};


int rtk_edid_header_is_valid(const unsigned char *raw_edid)
{
	unsigned int i, score = 0;

	for (i = 0; i < sizeof(edid_header); i++)
		if (raw_edid[i] == edid_header[i])
			score++;

	return score;
}

int rtk_edid_block_valid(unsigned char *raw_edid, int block)
{
	int i;
	unsigned char csum = 0;
	
	if (block == 0) {
		int score = rtk_edid_header_is_valid(raw_edid);
		if (score != 8)
			return 0;		
	}

	for (i = 0; i < EDID_LENGTH; i++)
		csum += raw_edid[i];
	if (csum) {
		printf("EDID checksum is invalid, remainder is %d\n", csum);		

		/* allow CEA to slide through, switches mangle this */
		if (raw_edid[0] != 0x02)
			return 0;
	}

	return 1;
}


int I2C_Read_Segment(unsigned char* Segment, int block, unsigned char* pData)
{	
	unsigned char SubAddr=EDID_LENGTH*block;
	
	I2C_Write_EX(HDMITX_I2C_ID, 0x30,1, Segment, NON_STOP);
			
	return I2C_Read_EX(HDMITX_I2C_ID, 0x50,1,&SubAddr, EDID_LENGTH, pData , NON_STOP);
}

static int Read_EDID_block(int block, unsigned char *EDID_buf)
{

	unsigned char i2c_addr=0x50;
	unsigned char SubAddr=EDID_LENGTH*block;
	unsigned short nSubAddrByte=1;
	unsigned short nDataByte=EDID_LENGTH;
	unsigned char  Segment=block/2;
	

	if(block<=1)
		return I2C_Read_EX(HDMITX_I2C_ID, i2c_addr,nSubAddrByte,&SubAddr, nDataByte, EDID_buf, 0);	
	else
		return I2C_Read_Segment(&Segment,block,EDID_buf);
	
	return S_OK ;
}


static int Read_EDID(unsigned char *EDID_buf)
{
	int i;
	unsigned char buf[EDID_LENGTH];
	
	I2CN_Init(1);
	I2C_SetSpeed(1, 80);

	for (i = 0; i < 4; i++) {
	
		if(Read_EDID_block(0, buf))
			return S_FALSE;
			
		if (rtk_edid_block_valid(buf, 0))
			break;	
	}
	//dump_raw_edid(buf , EDID_LENGTH);
	
	memcpy(EDID_buf,buf,sizeof(buf));
	
	if (EDID_buf[0x7e] == 0)
	{
		vo_hdmi_mode = VO_DVI_ON;
		printf("DVI mode\n");		
		I2CN_UnInit(1);
		return 0;
	}
	
	memset(buf,0x0,sizeof(buf));	
	for (i = 0; i < 4; i++) {
	
		if(Read_EDID_block(1, buf))
			return S_FALSE;
			
		if (rtk_edid_block_valid(buf, 1))
			break;	
	}		
	//dump_raw_edid(buf,EDID_LENGTH);
	
	memcpy(EDID_buf+EDID_LENGTH,buf,sizeof(buf));
	
	//dump_raw_edid(EDID_buf , 256);	
	
#if 0
	for (i = 0; i < 4; i++) {
	
		if(Read_EDID_block(2, buf))
			return S_FALSE;
			
		if (rtk_edid_block_valid(buf, 2))
			break;	
	}		
	dump_raw_edid(buf,EDID_LENGTH);
	
	for (i = 0; i < 4; i++) 
	{
		if(Read_EDID_block(3, buf))
			return S_FALSE;
			
		if (rtk_edid_block_valid(buf, 3))
			break;	
	}		
	dump_raw_edid(buf,EDID_LENGTH);
#endif
				
	I2CN_UnInit(1);

	return add_cea_modes((struct edid *)EDID_buf);
}

static int HDMITx_HPD(void)
{
#ifdef HDMITX_HPD_IGPIO_NUM	
	return getISOGPIO(HDMITX_HPD_IGPIO_NUM);
#else	
	return 0;
#endif		
}

void set_AVI_Infoframe_Color_Component_and_Chroma_Sample_Format(struct AVI_InfoFrame_Format *format, struct edid *edid)
{
	if (!(edid->input & DRM_EDID_INPUT_DIGITAL))
		return;
		
	if (edid->features & DRM_EDID_FEATURE_RGB_YCRCB444)
		format->Data_Byte1 |= AVI_InfoFrame_DB1_Y1Y0(YCBCR444);
	else if(edid->features & DRM_EDID_FEATURE_RGB_YCRCB422)
		format->Data_Byte1 |= AVI_InfoFrame_DB1_Y1Y0(YCBCR422);
	else
		format->Data_Byte1 |= AVI_InfoFrame_DB1_Y1Y0(RGB);
		
}


void set_AVI_Infoframe_Aspect_Ratio(struct AVI_InfoFrame_Format *format, int vid)
{
	
	//DataByte2: C1 C0 M1 M0 R3 R2 R1 R0; 
	//1.set Active Portion Aspect Ratio R3 R2 R1 R0
	//2.set Coded Frame Aspect Ratio M1 M0
	//3.set Colorimetry C1 C0	
	format->Data_Byte2 |= AVI_InfoFrame_DB2_R3210(SAME_AS_CODED_FRAME_AR);	
	
	if(vid < (int)VO_STANDARD_SECAM)//SD
	{
		format->Data_Byte2 |= AVI_InfoFrame_DB2_M1M0(_4X3);
		format->Data_Byte2 |= AVI_InfoFrame_DB2_C1C0(SMPTE_170M);						
	}
	else
	{
		format->Data_Byte2 |= AVI_InfoFrame_DB2_M1M0(_16X9);
		format->Data_Byte2 |= AVI_InfoFrame_DB2_C1C0(ITU_R_709);		
	}
	 
	
	if(0) //don't set xvYCC at this stage.
	{	
		if(0) //SD & support xvYCC601.
			format->Data_Byte3 |= AVI_InfoFrame_DB3_EC201(xvYCC601);
		if(0) //HD & support xvYCC709.
			format->Data_Byte3 |= AVI_InfoFrame_DB3_EC201(xvYCC709);
		
		format->Data_Byte2 |= AVI_InfoFrame_DB2_C1C0(EXTENDED_COLORIMETRY_INFO_VALID);
		//DataByte1: Y1 Y0 A0 R3 R2 R1 R0;
		// set Active Format Information Present A0
		format->Data_Byte1 |= AVI_InfoFrame_DB1_A0(ACTIVE_FORMAT_INFO_PRESENT);
	}
		
}

#define SCDC_I2C_ADDR	0x54

//Status and Control Data Channel Structure
#define SCDCS_TMDS_Config	0x20
#define SCDCS_Config_0		0x30
#define SCDCS_Status_Flag_0	0x40
static int hdmitx_write_scdc_port(unsigned char offset, unsigned char value)
{
	int ret_val;
	unsigned char data[2];

	data[0]= offset;
	data[1]= value;

	I2CN_Init(HDMITX_I2C_ID);
	I2C_SetSpeed(HDMITX_I2C_ID, 80);
	ret_val = I2C_Write_EX(HDMITX_I2C_ID, SCDC_I2C_ADDR, 2, (unsigned char*)&data, NO_READ);
	if (ret_val != S_OK)
		printf("Write SCDC port fail, offset(0x%02x) value(0x%02x)\n",offset,value);

	I2CN_UnInit(HDMITX_I2C_ID);
	return ret_val;
}


static int hdmitx_send_scdc_TmdsConfig(unsigned int standard, unsigned int dataInt0)
{
	unsigned char config_data;
	unsigned char deep_color,depp_depth;

	deep_color = (dataInt0>>1)&0x1;
	depp_depth = (dataInt0>>2)&0xF;

	if((depp_depth!=5)&&(depp_depth!=6))
		deep_color = 0;

	switch(standard)
	{
		//Scramble, 1/40 data rate
		case VO_STANDARD_HDTV_2160P_50:
		case VO_STANDARD_HDTV_2160P_59:
		case VO_STANDARD_HDTV_2160P_60:
		case VO_STANDARD_HDTV_4096_2160P_50:
		case VO_STANDARD_HDTV_4096_2160P_60:
		{
			config_data = 0x3;
			break;
		}

		//Scramble, 1/40 data rate
		case VO_STANDARD_HDTV_2160P_23:
		case VO_STANDARD_HDTV_2160P_24:
		case VO_STANDARD_HDTV_2160P_25:
		case VO_STANDARD_HDTV_2160P_29:
		case VO_STANDARD_HDTV_2160P_30:
		case VO_STANDARD_HDTV_4096_2160P_24:
		case VO_STANDARD_HDTV_4096_2160P_25:
		case VO_STANDARD_HDTV_4096_2160P_30:
		{
			if(deep_color)
				config_data = 0x3;
			else
				config_data = 0x0;

			break;
		}

		//Depends on 340M_SCRAMBLE support
		case VO_STANDARD_HDTV_2160P_50_420:
		case VO_STANDARD_HDTV_2160P_59_420:
		case VO_STANDARD_HDTV_2160P_60_420:
		case VO_STANDARD_HDTV_4096_2160P_50_420:
		case VO_STANDARD_HDTV_4096_2160P_60_420:
		{
			if(deep_color)
				config_data = 0x3;
			else if(hdmi2p0_info.scdc_capable&SCDC_340M_SCRAMBLE)
				config_data = 0x1;// Scramble, 1/10
			else
				config_data = 0x0;

			break;
		}
		//No Scramble, 1/10 data rate
		default:
			config_data = 0x0;
	}

	if (hdmi2p0_info.scdc_capable&SCDC_PRESENT) {
		if (hdmitx_write_scdc_port(SCDCS_TMDS_Config, config_data)!= S_OK) {
			printf("Error: Send SCDC command fail, skip one step\n");
			return -1;
		} else {
			printf("Send SCDC TMDS_Config(0x%02x) standard(%u) deep_color(%u)\n",config_data,standard,deep_color);
		}
	} else if (config_data != 0) {
		printf("Error: Sink not support SCDC_PRESENT, but need scramble, skip one step\n");
		return -1;
	} else if ((standard == VO_STANDARD_HDTV_2160P_60_420) ||
		(standard == VO_STANDARD_HDTV_2160P_50_420) ||
		(standard == VO_STANDARD_HDTV_4096_2160P_60_420) ||
		(standard == VO_STANDARD_HDTV_4096_2160P_50_420) ||
		(standard == VO_STANDARD_HDTV_2160P_59_420)) {
		/* 4K YUV420*/
		printf("Sink not support SCDC_PRESENT, but YUV420 format should need\n");
		if (hdmitx_write_scdc_port(SCDCS_TMDS_Config, config_data)!= S_OK) {
			printf("Error: Send SCDC command fail, skip one step\n");
			return -1;
		} else {
			printf("Send SCDC TMDS_Config(0x%02x) standard(%u) deep_color(%u)\n",config_data,standard,deep_color);
		}
	} else {
		printf("Skip send SCDC command\n");
	}

	return config_data;
}

static int set_resolution(int video_format)
{
	struct _BOOT_TV_STD_INFO boot_info;
	struct AVI_InfoFrame_Format avi={0};
	int ret_val,tv_system;
	unsigned int hdmi2p0_feature=0;

	if(get_one_step_info())
	{
		//compare current sink with one step info.
		memset(&boot_info, 0x0, sizeof(struct _BOOT_TV_STD_INFO));
		memcpy(&boot_info, (unsigned char *)VO_RESOLUTION,sizeof(struct _BOOT_TV_STD_INFO));

		if (get_chip_id() == RTK1198_CPU_ID
			&& SWAPEND32(boot_info.tv_sys.videoInfo.standard) > VO_STANDARD_HDTV_1080P_59)
		{
			printf("Skip 4K one step\n");
		}
		else if( SWAPEND32(boot_info.tv_sys.hdmiInfo.hdmiMode)== vo_hdmi_mode
			&& SWAPEND32(boot_info.tv_sys.interfaceType) <= VO_DIGITAL_ONLY
			&& EDID[EDID_LENGTH-1] == checksum_128 
			&& EDID[2*EDID_LENGTH-1] == checksum_256)
	    {
	    	ret_val = hdmitx_send_scdc_TmdsConfig(SWAPEND32(boot_info.tv_sys.videoInfo.standard), SWAPEND32(boot_info.tv_sys.hdmiInfo.dataInt0));
	    	if(ret_val>=0)
	    	{
	    		if(hdmi2p0_info.hdmi_id==HDMI_2P0_IDENTIFIER)
	    			hdmi2p0_feature |= 0x1;//[Bit0] HDMI2.0
	    		if(ret_val>=1)
	    			hdmi2p0_feature |= 0x2;//[Bit1]Scrabmle

				boot_info.tv_sys.hdmiInfo.hdmi2p0_feature = SWAPEND32(hdmi2p0_feature);
				printf("One step resolution, standard(%d) mode(%d)\n", SWAPEND32(boot_info.tv_sys.videoInfo.standard), SWAPEND32(boot_info.tv_sys.hdmiInfo.hdmiMode));
				dump_VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM(&boot_info.tv_sys);

				// Set one step info
				memcpy((unsigned char *)VO_RESOLUTION, &boot_info, sizeof(struct _BOOT_TV_STD_INFO) );
				arch_clean_cache_range(VO_RESOLUTION, sizeof(struct _BOOT_TV_STD_INFO));
				return S_OK;
			}
		}
		else
			printf("Sink changed, skip one step\n");
	}
	
	memset(&boot_info, 0x0, sizeof(struct _BOOT_TV_STD_INFO));

	boot_info.dwMagicNumber = SWAPEND32(0xC0DE0BEE); /* set magic pattern in first word */
	boot_info.tv_sys.interfaceType = SWAPEND32(VO_ANALOG_AND_DIGITAL);

    if(vo_hdmi_mode == VO_HDMI_ON && video_format != S_FALSE)
	{		
		tv_system = video_format;
		set_AVI_Infoframe_Color_Component_and_Chroma_Sample_Format(&avi,(struct edid*)EDID);
		set_AVI_Infoframe_Aspect_Ratio(&avi,video_format);	
	}
	else		
		tv_system = DEFAULT_TV_SYSTEM;

	printf("tv_system=%d mode=%d\n",tv_system,vo_hdmi_mode);
				
	boot_info.tv_sys.videoInfo.standard  = SWAPEND32(tv_system);
	boot_info.tv_sys.videoInfo.enProg    =  1;
	boot_info.tv_sys.videoInfo.enDIF     =  1;				// ignored.
	boot_info.tv_sys.videoInfo.enCompRGB =  0;				// just for component.
	boot_info.tv_sys.videoInfo.pedType   = SWAPEND32(1);	// ignored.
	boot_info.tv_sys.videoInfo.dataInt0  = SWAPEND32(4);	// related to deep color.
	boot_info.tv_sys.videoInfo.dataInt1  = SWAPEND32(0);	// ignored.
		
	boot_info.tv_sys.hdmiInfo.hdmiMode          = SWAPEND32(vo_hdmi_mode);
	boot_info.tv_sys.hdmiInfo.audioSampleFreq   = SWAPEND32(3);		// ignored.
	boot_info.tv_sys.hdmiInfo.audioChannelCount = 1; 				// ignored.
	boot_info.tv_sys.hdmiInfo.dataByte1         = vo_hdmi_mode == VO_HDMI_ON ? avi.Data_Byte1 : 64;
	boot_info.tv_sys.hdmiInfo.dataByte2         = vo_hdmi_mode == VO_HDMI_ON ? avi.Data_Byte2 : 168;
	boot_info.tv_sys.hdmiInfo.dataByte3         = 0;				
	boot_info.tv_sys.hdmiInfo.dataByte4         = 0;				// ignored.
	boot_info.tv_sys.hdmiInfo.dataByte5         = 0;				// ignored.
	boot_info.tv_sys.hdmiInfo.dataInt0          = SWAPEND32(1);		// ignored.

	dump_VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM(&boot_info.tv_sys);

	memcpy((unsigned char *)VO_RESOLUTION, &boot_info, sizeof(struct _BOOT_TV_STD_INFO) );
	arch_clean_cache_range(VO_RESOLUTION, sizeof(struct _BOOT_TV_STD_INFO));

	return S_OK;
}

void set_hdmi_off(void)
{
	struct _BOOT_TV_STD_INFO boot_info;
	memset(&boot_info, 0x0, sizeof(struct _BOOT_TV_STD_INFO));
	boot_info.dwMagicNumber = SWAPEND32(0xC0DE0BEE); /* set magic pattern in first word */

	boot_info.tv_sys.videoInfo.standard  = SWAPEND32(VO_STANDARD_NTSC_J);
	boot_info.tv_sys.videoInfo.enProg    =  1;
	boot_info.tv_sys.videoInfo.pedType   = SWAPEND32(1);	// ignored.
	boot_info.tv_sys.videoInfo.dataInt0  = SWAPEND32(4);	// related to deep color.
	boot_info.tv_sys.hdmiInfo.hdmiMode	 = SWAPEND32(VO_HDMI_OFF);

	memcpy((unsigned char *)VO_RESOLUTION, &boot_info, sizeof(struct _BOOT_TV_STD_INFO) );
	arch_clean_cache_range(VO_RESOLUTION, sizeof(struct _BOOT_TV_STD_INFO));
	printf("Set HDMI TX OFF\n");
}

int sink_capability_handler(int set)
{
	int vid = S_FALSE;
	
	if(HDMITx_HPD())
		vid = Read_EDID(EDID);
	else
	{
		printf("HDMITx_HPD=False\n");
		if(HDMITX_MODE==2)/* Set HDMI TX off when HPD=0 */
		{
			set_hdmi_off();
			return S_OK;
		}
	}

	if(set)
		set_resolution(vid);
		
	return S_OK;
}

