package com.realtek.app.tv;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class DvbChannelInfo implements Parcelable {
	private static final String TAG = "DvbChannelInfo";

	// SERVICE_TYPE maps to it in DtvServiceDefs.h
	public final static int SERVICE_TYPE_UNDEFINED				= 0x00;
	public final static int SERVICE_TYPE_DTV					= 0x01;
	public final static int SERVICE_TYPE_RADIO					= 0x02;
	public final static int SERVICE_TYPE_TELETEXT				= 0x03;
	public final static int SERVICE_TYPE_NVOD_REFERENCE			= 0x04;
	public final static int SERVICE_TYPE_NVOD_TIMESHIFT			= 0x05;
	public final static int SERVICE_TYPE_MOSAIC					= 0x06;
	public final static int SERVICE_TYPE_FMRADIO				= 0x07;
	public final static int SERVICE_TYPE_DVB_SRM				= 0x08;
	public final static int SERVICE_TYPE_RESERVED				= 0x09;
	public final static int SERVICE_TYPE_ADV_RADIO				= 0x0A;
	public final static int SERVICE_TYPE_ADV_MOSAIC				= 0x0B;
	public final static int SERVICE_TYPE_DATA					= 0x0C;
	public final static int SERVICE_TYPE_CIU					= 0x0D;
	public final static int SERVICE_TYPE_RCS_MAP				= 0x0E;
	public final static int SERVICE_TYPE_RCS_FLS				= 0x0F;
	public final static int SERVICE_TYPE_MHP					= 0x10;
	public final static int SERVICE_TYPE_VIDEO_HD				= 0x11;
	public final static int SERVICE_TYPE_ADVANCED_CODEC_SD_TV	= 0x16;
	public final static int SERVICE_TYPE_ADVANCED_CODEC_HD_TV	= 0x19;

	// CHANNEL_FLAGS maps to it in DtvServiceDefs.h
	public final static int CHANNEL_FLAGS_IGNORE = 0x00000000;
	public final static int CHANNEL_FLAGS_IS_DELETED = 0x00000001;
	public final static int CHANNEL_FLAGS_IS_LOCKED = 0x00000002;
	public final static int CHANNEL_FLAGS_IS_FAVORITE = 0x00000004;
	public final static int CHANNEL_FLAGS_IS_HIDDEN = 0x00000008;
	public final static int CHANNEL_FLAGS_DISAPPEARED = 0x00000200;
	public final static int CHANNEL_FLAG_IS_DTV = 0x00100000;
	public final static int CHANNEL_FLAG_IS_RADIO = 0x00200000;
	
	public int logicalChNum;
	public int userChNum;
	public int frequency;
	public int bandwidth;
	public int type;	// the meaning of type refers to SERVICE_TYPE
	public int flags;	// the meaning of flags refers to CHANNEL_FLAGS
	public int channelNameLen;
	public String channelName;

	// Marshaler
	public void writeToParcel(Parcel dest, int iflags) {
		dest.writeInt(logicalChNum);
		dest.writeInt(userChNum);
		dest.writeInt(frequency);
		dest.writeInt(bandwidth);
		dest.writeInt(type);
		dest.writeInt(flags);
		dest.writeInt(channelNameLen);		
		dest.writeString(channelName);
	}

	// Unmarshaler
	public static final Parcelable.Creator<DvbChannelInfo> CREATOR
		= new Parcelable.Creator<DvbChannelInfo>() {
			public DvbChannelInfo createFromParcel(Parcel src) {
				DvbChannelInfo dvbChannelInfo = new DvbChannelInfo();
				dvbChannelInfo.readFromParcel(src);
				return dvbChannelInfo;
			}

			public DvbChannelInfo[] newArray(int size) {
				return new DvbChannelInfo[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public DvbChannelInfo() {
		logicalChNum = 0;
		userChNum = 0;
		frequency = 0;
		bandwidth = 0;
		type = 0;
		flags = 0;
		channelNameLen = 0;
		channelName = "";
	}

	public DvbChannelInfo(Parcel src) {
		readFromParcel(src);
	}

	public void readFromParcel(Parcel src) {
		logicalChNum = src.readInt();
		userChNum = src.readInt();		
		frequency = src.readInt();
		bandwidth = src.readInt();
		type = src.readInt();
		flags = src.readInt();
		channelNameLen = src.readInt();
		channelName = src.readString();
	}
	
}
