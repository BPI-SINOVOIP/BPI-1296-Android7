/***************************************************************************
Copyright (c) 2007 Realtek, All rights reserved.
***************************************************************************/
//All rights reserved. No part of this document may be reproduced or transmitted in any form or by any means,
//electronic, mechanical, photocopying, recording, or otherwise, without prior written permission of Realtek.
//One permission is only valid for one document, it is not appliable to any other documents from Realtek.
//There is no warranty of the document and its binary form.
//This programs may contain sensitive DRM management methods and codes,
//violation of DRM is liable to legal issues.
//************************************************************************

#ifndef FW_SOCKET_CLIENT_H
#define FW_SOCKET_CLIENT_H

#define RPC_PKT_HEADER 8
#define RPC_DATA_OFFSET 2
#define RPC_PKT_TYPE_IDX 0
#define RPC_PKT_SIZE_IDX 1
#define RPC_PKT_DATA_IDX 2

#define SHARE_MEM_SIZE 4096
#define SHARE_MEM_TYPE 0    // for int
#define SHARE_MEM_DATA 1    // for int

enum
{
    /*################
    IMPORTANT: the naming must be ENUM_RPC
    ex:
    if RPC is AUDIO_RPC_ToAgent_PrivateInfo_0_svc, the naming is ENUM_AUDIO_RPC_ToAgent_PrivateInfo_0
    #################*/
    ENUM_AUDIO_RPC_ToAgent_Create_0 = 1,
    ENUM_AUDIO_RPC_ToAgent_InitRingBufferHeader_0,
    ENUM_AUDIO_RPC_ToAgent_Run_0,
    ENUM_AUDIO_RPC_ToAgent_Pause_0,
    ENUM_AUDIO_RPC_ToAgent_Stop_0,
    ENUM_AUDIO_RPC_ToAgent_Destroy_0,
    ENUM_AUDIO_RPC_ToAgent_Flush_0,
    ENUM_AUDIO_RPC_ToAgent_AskDebugMemoryAddress_0,
    ENUM_AUDIO_RPC_ToAgent_HDMI_OUT_EDID2_0,
    ENUM_AUDIO_RPC_ToAgent_AudioConfig_0,
    ENUM_AUDIO_RPC_ToAgent_AOUT_HDMI_Set_0,
    ENUM_Destroy_Socket,
    ENUM_AUDIO_RPC_ToAgent_DAC_I2S_Config_0,
    ENUM_AUDIO_RPC_ToAgent_DAC_SPDIF_Config_0,
    ENUM_AUDIO_RPC_ToAgent_PrivateInfo_0,
    ENUM_AUDIO_RPC_ToAgent_MS12_Init_Cfg_0,
    ENUM_AUDIO_RPC_ToAgent_MS12_Runtime_Cfg_0,
    ENUM_AUDIO_RPC_ToAgent_Connect_0,
    ENUM_AUDIO_RPC_ToAgent_MS12_Update_Param_0,
    ENUM_AUDIO_RPC_ToAgent_MS12_Clear_Update_Param_0,
    ENUM_AUDIO_RPC_ToAgent_ShareMemInit,
    ENUM_AUDIO_RPC_ToAgent_ShareMemSendMsg,
    ENUM_AUDIO_RPU_ToAgent_StartEncoder_0,
    ENUM_AUDIO_RPC_ToAgent_MS12_Runtime_Update_Param_0,
    ENUM_AUDIO_RPC_ToAgent_Add_IonFD_Info_0,
    ENUM_AUDIO_RPC_ToAgent_Flush_FD_Info_0,
    ENUM_AUDIO_RPC_ToAgent_MS12v2_3_Init_Cfg_0,
    ENUM_AUDIO_RPC_ToAgent_MS12v2_3_Runtime_Cfg_0
};

enum
{
    ENUM_SHARE_DTS_FILENAME = 1,
    ENUM_SHARE_DTS_FILENAME2,
    ENUM_SHARE_DTS_OUTPUT_DIR,
    ENUM_SHARE_MPEGH_ASC_DATA,
    ENUM_SHARE_MPEGH_XML_DATA
};

typedef struct  {
    AUDIO_RPC_RINGBUFFER_HEADER nRingHdr;
    int ionhdl_header_fd[8];    // ring buf header
    int ionhdl_fd[8];   // ring buf base
} FW_RINGBUFFER_HDR;

typedef struct{
    RPC_RINGBUFF_FD_INFO fd_info;
    int ionhdl_header_fd[8];    // ring buf header
    int ionhdl_fd[8];   // ring buf base
} FW_RINGBUF_FD_INFO;

#ifdef __cplusplus
extern "C" {
#endif

int fw_socket_init();
void fw_socket_uninit();
void fw_socket_uninit_all();
int fw_socket_send(int type, int *pArg, int *pRet, int nSizeArg, int nSizeRet);
int fw_socket_send_size(int type, int *pArg, int *pRet, int nSizeArg, int nSizeRet);
int fw_socket_send_fd(int type, FW_RINGBUF_FD_INFO *pArg);

#ifdef __cplusplus
}
#endif

#endif


