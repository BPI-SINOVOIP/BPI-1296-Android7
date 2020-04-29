#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/ioctl.h> /* needed for the _IOW etc stuff used later */
#include <linux/syscalls.h> /* needed for the _IOW etc stuff used later */
#include <linux/mpage.h>
#include <linux/dcache.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <sound/asound.h>
#include <asm/cacheflush.h>
#include <linux/slab.h>

#include <linux/power-control.h>
#include <linux/reset-helper.h> // rstc_get
#include <linux/reset.h>
#include <linux/clkdev.h>  // clk_get
#include <linux/clk-provider.h>

#include <soc/realtek/kernel-rpc.h>
#include <linux/dma-mapping.h>

#include "rtk_fw_dbg_info_rpc.h"



#include "uapi/ion.h"
#include "ion/ion.h"
#include "uapi/rtk_phoenix_ion.h"

struct ion_client *fw_dbg_info_rpc_ion_client;
extern struct ion_device *rtk_phoenix_ion_device;



#define CONVERT_FOR_AVCPU(x)		((unsigned int)(x) | 0xA0000000)




// Need ION_AUDIO_HEAP
void rtk_fw_dbg_info_register_ion_client(const char *name)
{
    fw_dbg_info_rpc_ion_client = ion_client_create(rtk_phoenix_ion_device, name);
	
}

void rtk_fw_dbg_info_deregister_ion_client(const char *name)
{
    if (fw_dbg_info_rpc_ion_client != NULL)
    {
        FW_INFO_ERROR_PRINT("%s :%s", __FUNCTION__,name);
        ion_client_destroy(fw_dbg_info_rpc_ion_client);
    }
}


//Only for test
int FW_DBG_RPC_ToAgent_Vout_EDIDdata(struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG *arg)
{
	struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG *rpc = NULL;
	int ret = -1;
	u32 RPC_ret;
	struct ion_handle *handle = NULL;
	ion_phys_addr_t dat;
	size_t len;

	handle = ion_alloc(fw_dbg_info_rpc_ion_client, 4096, 1024, RTK_PHOENIX_ION_HEAP_AUDIO_MASK, ION_FLAG_NONCACHED |
																					ION_FLAG_SCPUACC | 
																					ION_FLAG_ACPUACC);

	if (IS_ERR(handle)) {
		FW_INFO_ERROR_PRINT( "[%s %d ion_alloc fail]", __FUNCTION__, __LINE__);
		goto exit;
	}

	if(ion_phys(fw_dbg_info_rpc_ion_client, handle, &dat, &len) != 0) {
		FW_INFO_ERROR_PRINT( "[%s %d fail]", __FUNCTION__, __LINE__);
		goto exit;
	}

	rpc = ion_map_kernel(fw_dbg_info_rpc_ion_client, handle);

	memcpy(rpc,arg,sizeof(struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG));

	if (send_rpc_command(RPC_AUDIO,
		ENUM_VIDEO_KERNEL_RPC_VOUT_EDID_DATA,
		CONVERT_FOR_AVCPU(dat),
		CONVERT_FOR_AVCPU(dat + get_rpc_alignment_offset(sizeof(struct VIDEO_RPC_VOUT_EDID_DATA_FW_DBG))),
		&RPC_ret))
	{
		FW_INFO_ERROR_PRINT( "[%s %d RPC fail]",__FUNCTION__, __LINE__);
		goto exit;
	}

	ret = 0;
exit:
	if(handle != NULL) {
		ion_unmap_kernel(fw_dbg_info_rpc_ion_client, handle);
		ion_free(fw_dbg_info_rpc_ion_client, handle);
	}
	return ret;
}




int FW_DBG_KERNEL_RPC_QUERY_FW_DEBUG_INFO(int *query_what, RPC_FW_QUERY_RETURN *query_ret)
{
	int *i_rpc = NULL;
	struct RPC_FW_QUERY_RETURN *o_rpc = NULL;
	int ret = -1;
	u32 RPC_ret;
	struct ion_handle *handle = NULL;
	ion_phys_addr_t dat;
	size_t len;
	unsigned int offset;


	handle = ion_alloc(fw_dbg_info_rpc_ion_client, 4096, 1024, RTK_PHOENIX_ION_HEAP_AUDIO_MASK, ION_FLAG_NONCACHED |
																					ION_FLAG_SCPUACC | 
																					ION_FLAG_ACPUACC);

	if (IS_ERR(handle)) {
		FW_INFO_ERROR_PRINT( "[%s %d ion_alloc fail]", __FUNCTION__, __LINE__);
		goto exit;
	}

	if(ion_phys(fw_dbg_info_rpc_ion_client, handle, &dat, &len) != 0) {
	    FW_INFO_ERROR_PRINT( "[%s %d fail]", __FUNCTION__, __LINE__);
	    goto exit;
	}



	i_rpc = ion_map_kernel(fw_dbg_info_rpc_ion_client, handle);


    *i_rpc = htonl(*query_what);

	offset = get_rpc_alignment_offset(sizeof(int));
	o_rpc = (struct RPC_FW_QUERY_RETURN *)((unsigned long)i_rpc + offset);

	if (send_rpc_command(RPC_AUDIO,
        ENUM_KERNEL_RPC_QUERY_FW_DEBUG_INFO,
        CONVERT_FOR_AVCPU(dat),
        CONVERT_FOR_AVCPU(dat + offset),
        &RPC_ret))
    {
        FW_INFO_ERROR_PRINT( "[%s %d RPC fail]",__FUNCTION__, __LINE__);
        goto exit;
    }

	memcpy(query_ret,o_rpc,sizeof(struct RPC_FW_QUERY_RETURN));

	ret = 0;
exit:
	if(handle != NULL) {
		ion_unmap_kernel(fw_dbg_info_rpc_ion_client, handle);
		ion_free(fw_dbg_info_rpc_ion_client, handle);
	}
	return ret;

}
