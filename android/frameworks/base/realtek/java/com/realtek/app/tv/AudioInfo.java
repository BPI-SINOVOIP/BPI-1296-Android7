package com.realtek.app.tv;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class AudioInfo implements Parcelable {
	private static final String TAG = "AudioInfo";

	// CODEC_TYPE maps to it in DtvServiceDefs.h
	// DTV should not use the value behinds CODEC_AUDIO_LPCM in CODEC_TYPE, so ignore it.
	public final static int CODEC_UNKNOWN = 0;
	public final static int CODEC_VIDEO_11172 = 1;
	public final static int CODEC_VIDEO_13818 = 2;
	public final static int CODEC_VIDEO_14496 = 3;
	public final static int CODEC_VIDEO_H264 = 4;
	public final static int CODEC_VIDEO_VC1 = 5;
	public final static int CODEC_AUDIO_11172 = 6;
	public final static int CODEC_AUDIO_13818 = 7;
	public final static int CODEC_AUDIO_AC3 = 8;
	public final static int CODEC_AUDIO_E_AC3 = 9;
	public final static int CODEC_AUDIO_DTS = 10;
	public final static int CODEC_AUDIO_AAC = 11;
	public final static int CODEC_AUDIO_LATM_AAC = 12;
	public final static int CODEC_AUDIO_LPCM = 13;
	
	public int pid;
	public int streamType;
	public int type;	// the meaning of type refers to CODEC_TYPE
	public int langCount;
	public int [] langCode;
	public String [] shortLangStr;	

	// Marshaler
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(pid);
		dest.writeInt(streamType);
		dest.writeInt(type);
		dest.writeInt(langCount);
		dest.writeIntArray(langCode);
		dest.writeStringArray(shortLangStr);
	}

	// Unmarshaler
	public static final Parcelable.Creator<AudioInfo> CREATOR
		= new Parcelable.Creator<AudioInfo>() {
			public AudioInfo createFromParcel(Parcel src) {
				AudioInfo audioInfo = new AudioInfo();
				audioInfo.readFromParcel(src);
				return audioInfo;
			}

			public AudioInfo[] newArray(int size) {
				return new AudioInfo[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public AudioInfo() {
		pid = 0;
		streamType = 0;
		type = 0;
		langCount = 0;
		langCode = new int[2];
		langCode[0] = langCode[1] = 0;
		shortLangStr = new String[2];
		shortLangStr[0] = shortLangStr[1] = "";
	}

	public AudioInfo(Parcel src) {
		readFromParcel(src);
	}

	public void readFromParcel(Parcel src) {
		pid = src.readInt();
		streamType = src.readInt();
		type= src.readInt();
		langCount = src.readInt();
		src.readIntArray(langCode);
		src.readStringArray(shortLangStr);
	}
	
}

