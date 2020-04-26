package com.realtek.hardware;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class RtkTVSystem implements Parcelable {
	public int intVideoSystem;
	public int intVideoStandard;
	public int intIsWideScreen;

	// TV deep color setting menu
	public static final int TV_SYS_COLORSPACE_AUTO_DETECT = 0;
	public static final int TV_SYS_RGB444_8BIT = 1;
	public static final int TV_SYS_RGB444_10BIT = 2;
	public static final int TV_SYS_RGB444_12BIT = 3;
	public static final int TV_SYS_YCbCr444_8BIT = 4;
	public static final int TV_SYS_YCbCr444_10BIT = 5;
	public static final int TV_SYS_YCbCr444_12BIT = 6;
	public static final int TV_SYS_YCbCr422 = 7;
	public static final int TV_SYS_YCbCr420_8BIT = 8;
	public static final int TV_SYS_YCbCr420_10BIT = 9;
	public static final int TV_SYS_YCbCr420_12BIT = 10;
	public static final int TV_SYS_DEEPCOLOR_AUTO_DETECT = 11;

	// TV setting menu
	public static final int TV_SYS_HDMI_AUTO_DETECT = 0;
	public static final int TV_SYS_NTSC = 1;
	public static final int TV_SYS_PAL = 2;
	public static final int TV_SYS_480P = 3;
	public static final int TV_SYS_576P = 4;
	public static final int TV_SYS_720P_50HZ = 5;
	public static final int TV_SYS_720P_60HZ = 6;
	public static final int TV_SYS_1080I_50HZ = 7;
	public static final int TV_SYS_1080I_60HZ = 8;
 	public static final int TV_SYS_1080P_50HZ = 9;
	public static final int TV_SYS_1080P_60HZ = 10;
	public static final int TV_SYS_2160P_24HZ = 11;	//3840*2160 24Hz
	public static final int TV_SYS_2160P_25HZ = 12;	//3840*2160 25Hz
	public static final int TV_SYS_2160P_30HZ = 13;	//3840*2160 30Hz
	public static final int TV_SYS_4096_2160P_24HZ = 14;	//4096*2160 24Hz
	public static final int TV_SYS_1080P_24HZ = 15;
	public static final int TV_SYS_720P_59HZ = 16;
	public static final int TV_SYS_1080I_59HZ = 17;
 	public static final int TV_SYS_1080P_23HZ = 18;
 	public static final int TV_SYS_1080P_59HZ = 19;
 	public static final int TV_SYS_2160P_23HZ = 20;
 	public static final int TV_SYS_2160P_29HZ = 21;
 	public static final int TV_SYS_2160P_60HZ = 22;
 	public static final int TV_SYS_2160P_50HZ = 23;
	public static final int TV_SYS_4096_2160P_50HZ = 24;
	public static final int TV_SYS_4096_2160P_60HZ = 25;
	public static final int TV_SYS_4096_2160P_25HZ = 26;
	public static final int TV_SYS_4096_2160P_30HZ = 27;
 	public static final int TV_SYS_2160P_59HZ = 28;
 	public static final int TV_SYS_1080P_25HZ = 29;
 	public static final int TV_SYS_1080P_30HZ = 30;
	public static final int TV_SYS_NUM = 31;

	// Video System constants
	/* Important!! Must exactly match with video_system in setupupdef.h */
	public static final int VIDEO_NTSC = 0;
	public static final int VIDEO_PAL = 1;
	public static final int VIDEO_HD720_50HZ = 2;
	public static final int VIDEO_HD720_60HZ = 3;
  	public static final int VIDEO_HD1080_50HZ = 4;
	public static final int VIDEO_HD1080_60HZ = 5;
	public static final int VIDEO_HD1080_24HZ = 6;
	public static final int VIDEO_HD2160_24HZ = 7;
  	public static final int VIDEO_HD2160_25HZ = 8;
	public static final int VIDEO_HD2160_30HZ = 9;
	public static final int VIDEO_HD4096_2160_24HZ = 10;
	public static final int VIDEO_SVGA800x600 = 11;
	public static final int VIDEO_HD720_59HZ = 12;
	public static final int VIDEO_HD1080_59HZ = 13;
	public static final int VIDEO_HD1080_23HZ = 14;
	public static final int VIDEO_HD2160_23HZ = 15;
	public static final int VIDEO_HD2160_29HZ = 16;
	public static final int VIDEO_HD2160_60HZ = 17;
	public static final int VIDEO_HD1080_30HZ = 18;
	public static final int VIDEO_HD2160_50HZ = 19;
	public static final int VIDEO_HD4096_2160_50HZ = 20;
	public static final int VIDEO_HD4096_2160_60HZ = 21;
	public static final int VIDEO_HD4096_2160_25HZ = 22;
	public static final int VIDEO_HD4096_2160_30HZ = 23;
	public static final int VIDEO_DP_1080P_60HZ = 24;
	public static final int VIDEO_DP_2160P_30HZ = 25;
	public static final int VIDEO_HD2160_59HZ = 26;
	public static final int VIDEO_HD1080_25HZ = 27;
	public static final int VIDEO_SYSTEM_NUM = 28; // VIDEO_SYSTEM_NUM should be the last.

	// Video Standard constants
  	public static final int VIDEO_INTERLACED = 0;
	public static final int VIDEO_PROGRESSIVE = 1;

	// TV System Auto Mode
	public static final int TV_SYSTEM_AUTO_MODE_OFF = 0;
	public static final int TV_SYSTEM_AUTO_MODE_PLAYBACK = 1;
	public static final int TV_SYSTEM_AUTO_MODE_HDMI_EDID = 2;

	public static final int HDMI_MODE = 1;
	public static final int MHL_MODE = 2;

	// TV surpported eotf mode.
	public static final int HDR_TV_SUPPOTED_EOTF_SDR = 0;	
	public static final int HDR_TV_SUPPOTED_EOTF_HDR = 1;
	public static final int HDR_TV_SUPPOTED_EOTF_SMPTEST2084 = 2;
	public static final int HDR_TV_SUPPOTED_EOTF_HLG = 3;

	//HDR color depth
	public static final int HDR_COLOR_DEPTH_AUTO = 0;
	public static final int HDR_COLOR_DEPTH_8BIT = 1;
	public static final int HDR_COLOR_DEPTH_10BIT = 2;
 	public static final int HDR_COLOR_DEPTH_12BIT = 3;

    // HDR setting
    public static final int VO_HDR_CTRL_NONE = 0;
    public static final int VO_HDR_CTRL_DV_ON = 1;
    public static final int VO_HDR_CTRL_SDR = 2;
    public static final int VO_HDR_CTRL_HDR_GAMMA = 3;
    public static final int VO_HDR_CTRL_PQHDR = 4;
    public static final int VO_HDR_CTRL_FUTURE = 5;
    public static final int VO_HDR_CTRL_INPUT = 6;

	// map TV setting menu to video system and video standard
	public static final int[][] tv_map = {	{TV_SYS_NTSC, VIDEO_NTSC, VIDEO_INTERLACED},
									{TV_SYS_PAL, VIDEO_PAL, VIDEO_INTERLACED},
									{TV_SYS_480P, VIDEO_NTSC, VIDEO_PROGRESSIVE},
									{TV_SYS_576P, VIDEO_PAL, VIDEO_PROGRESSIVE},
									{TV_SYS_720P_50HZ, VIDEO_HD720_50HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_720P_60HZ, VIDEO_HD720_60HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080I_50HZ, VIDEO_HD1080_50HZ, VIDEO_INTERLACED},
									{TV_SYS_1080I_60HZ, VIDEO_HD1080_60HZ, VIDEO_INTERLACED},
									{TV_SYS_1080P_50HZ, VIDEO_HD1080_50HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080P_60HZ, VIDEO_HD1080_60HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_24HZ, VIDEO_HD2160_24HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_25HZ, VIDEO_HD2160_25HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_30HZ, VIDEO_HD2160_30HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_4096_2160P_24HZ, VIDEO_HD4096_2160_24HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080P_24HZ, VIDEO_HD1080_24HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_720P_59HZ, VIDEO_HD720_59HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080I_59HZ, VIDEO_HD1080_59HZ, VIDEO_INTERLACED},
									{TV_SYS_1080P_23HZ, VIDEO_HD1080_23HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080P_59HZ, VIDEO_HD1080_59HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_23HZ, VIDEO_HD2160_23HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_29HZ, VIDEO_HD2160_29HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_60HZ, VIDEO_HD2160_60HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_50HZ, VIDEO_HD2160_50HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_4096_2160P_50HZ, VIDEO_HD4096_2160_50HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_4096_2160P_60HZ, VIDEO_HD4096_2160_60HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_4096_2160P_25HZ, VIDEO_HD4096_2160_25HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_4096_2160P_30HZ, VIDEO_HD4096_2160_30HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_2160P_59HZ, VIDEO_HD2160_59HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080P_25HZ, VIDEO_HD1080_25HZ, VIDEO_PROGRESSIVE},
									{TV_SYS_1080P_30HZ, VIDEO_HD1080_30HZ, VIDEO_PROGRESSIVE},
									};

	public static final Parcelable.Creator<RtkTVSystem> CREATOR = new Creator<RtkTVSystem>() {

		@Override
		public RtkTVSystem createFromParcel(Parcel source) {
			// It must be read by the order of the member in the class.
			// Otherwise, it fails.
			RtkTVSystem rtk = new RtkTVSystem();
			if(rtk != null) {
				rtk.setVideoSystem(source.readInt());
				rtk.setVideoStandard(source.readInt());
				rtk.setIsWideScreen(source.readInt());
			}
			return rtk;
		}

		@Override
		public RtkTVSystem[] newArray(int  size) {
			return new RtkTVSystem[size];
		}

	};


	//@Override
	public int describeContents() {
		return 0;
	}

	//@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(intVideoSystem);
		dest.writeInt(intVideoStandard);
		dest.writeInt(intIsWideScreen);
	}

	public int getVideoSystem() {
		return intVideoSystem;
	}

	public void setVideoSystem(int intVideoSystem) {
		this.intVideoSystem = intVideoSystem;
	}

	public int getVideoStandard() {
		return intVideoStandard;
	}

	public void setVideoStandard(int intVideoStandard) {
		this.intVideoStandard = intVideoStandard;
	}

	public int getIsWideScreen() {
		return intIsWideScreen;
	}

	public void setIsWideScreen(int intIsWideScreen) {
		this.intIsWideScreen = intIsWideScreen;
	}

	public void print(String tag) {
		Log.d(tag, String.format("print: intVideoSystem=%d, intVideoStandard=%d, intIsWildScreen=%d", intVideoSystem, intVideoStandard, intIsWideScreen));
	}
}
