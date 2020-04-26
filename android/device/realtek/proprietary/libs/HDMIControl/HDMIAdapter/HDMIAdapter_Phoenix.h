#ifndef __PHOENIX_HDMI_ADAPTER_H__
#define __PHOENIX_HDMI_ADAPTER_H__

#include "HDMIAdapter.h"

#define HDMI_HARDWARE_DEVICE "/dev/hdmitx"

/**
 * The id of this module
 */
#define HDMI_HARDWARE_MODULE_ID "hdmi"
#define HDMI_HARDWARE_HDMI0		"hdmi0"


class PhoenixHDMIAdapter: public HDMIAdapter {
public:
	PhoenixHDMIAdapter();
	~PhoenixHDMIAdapter();

private:
	int m_fd;
	struct {
		unsigned char init_rdy : 1;
	}m_status;

protected:
	// Hardware dependent layer
	virtual int get_capability(void *);
	virtual int is_connected(void *);
	virtual int get_edid(void *);
	virtual int send_avmute(void *,int select);
	virtual int check_tmds_src(void *,int *tmds);
	virtual int check_rx_sense(void *, int *rx_sense);

	// video RPC-related
	virtual int config_tv_system(void *,struct VIDEO_RPC_VOUT_CONFIG_TV_SYSTEM argp);
	virtual int config_avi_info(void *,struct VIDEO_RPC_VOUT_CONFIG_HDMI_INFO_FRAME argp);

	// audio RPC-related
	virtual int set_frequency(void *,struct AUDIO_HDMI_SET argp);
	virtual int send_audio_mute(void *,struct AUDIO_HDMI_MUTE_INFO argp);
	virtual int send_audio_VSDB_data(void *,struct AUDIO_HDMI_OUT_VSDB_DATA argp);
	virtual int send_audio_edid2(void *,struct AUDIO_HDMI_OUT_EDID_DATA2 argp);
};

#endif
