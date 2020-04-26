/*
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_MHL_INTERFACE_H
#define ANDROID_MHL_INTERFACE_H

#include <hardware/hardware.h>

#include <stdint.h>
#include <sys/cdefs.h>
#include <sys/types.h>
#include <drm_crtc.h>
//#include <rtk_hal.h>
#include <hdmi_common.h>

#define MHL_HARDWARE_DEVICE "/dev/mhltx"

/**
 * The id of this module
 */
#define MHL_HARDWARE_MODULE_ID "mhl"
#define MHL_HARDWARE_MHL0		"mhl0"

/* MHL ioctl */
enum {
	MHL_GET_SINK_CAPABILITY,
	MHL_GET_RAW_EDID,
	MHL_GET_LINK_STATUS,
	MHL_GET_VIDEO_CONFIG,
	MHL_SEND_AVMUTE,
	MHL_CONFIG_TV_SYSTEM,
	MHL_CONFIG_AVI_INFO,
	MHL_SET_FREQUNCY,
	MHL_SEND_AUDIO_MUTE,
	MHL_SEND_AUDIO_VSDB_DATA,
	MHL_SEND_AUDIO_EDID2,
	MHL_CHECK_TMDS_SRC,
	MHL_SEND_MSC_CMD,
	MHL_RECEIVE_MSC_MSG,
};


/**
 * Every hardware module must have a data structure named HAL_MODULE_INFO_SYM
 * and the fields of this data structure must begin with hw_module_t
 * followed by module specific information.
 */
struct mhl_module_t {
    struct hw_module_t common;
};



/**
 * Every device data structure must begin with hw_device_t
 * followed by module specific public methods and attributes.
 */
struct mhl_device_t {
    struct hw_device_t common;
    struct sink_capabilities_t sink_cap;
	struct raw_edid edid;

    int (*get_capability)(struct mhl_device_t *dev);
    int (*is_connected)(struct mhl_device_t *dev);
	int (*get_edid)(struct mhl_device_t *dev);
	int (*send_avmute)(struct mhl_device_t *dev, int select); 
	int (*check_tmds_src)(struct mhl_device_t *dev, int *tmds); 
	
	
	//video RPC-related
	int (*config_tv_system)(struct mhl_device_t *dev, struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM argp);
	int (*config_avi_info)(struct mhl_device_t *dev, struct VIDEO_RPC_VOUT_CONFIG_HDMI_INFO_FRAME argp);
	
	//audio RPC-related
	int (*set_frequency)(struct mhl_device_t *dev, struct AUDIO_HDMI_SET argp);
	int (*send_audio_mute)(struct mhl_device_t *dev, struct AUDIO_HDMI_MUTE_INFO argp);
	int (*send_audio_VSDB_data)(struct mhl_device_t *dev, struct AUDIO_HDMI_OUT_VSDB_DATA argp);
    int (*send_audio_edid2)(struct mhl_device_t *dev, struct AUDIO_HDMI_OUT_EDID_DATA2 argp);
	
	//cbus-related
	int (*receive_msc_rcp)(struct mhl_device_t *dev, unsigned char *key_id);
};


/** convenience API for opening and closing a device */

static inline int mhl_dev_open(const struct hw_module_t* module, 
        struct mhl_device_t** device) {
    return module->methods->open(module, 
            MHL_HARDWARE_MHL0, (struct hw_device_t**)device);
}

static inline int mhl_dev_close(struct mhl_device_t* device) {
    return device->common.close(&device->common);
}


//__END_DECLS

#endif  // ANDROID_MHL_INTERFACE_H

