package com.realtek.app.tv;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class DvbSubInfo implements Parcelable {
	private static final String TAG = "DvbSubInfo";
	public int pid;
	public int langCode;
	public String shortLangStr;
	public int isHardOfHearing;

	// Marshaler
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(pid);
		dest.writeInt(langCode);
		dest.writeString(shortLangStr);
		dest.writeInt(isHardOfHearing);
	}

	// Unmarshaler
	public static final Parcelable.Creator<DvbSubInfo> CREATOR
		= new Parcelable.Creator<DvbSubInfo>() {
			public DvbSubInfo createFromParcel(Parcel src) {
				DvbSubInfo dvbSubInfo = new DvbSubInfo();
				dvbSubInfo.readFromParcel(src);
				return dvbSubInfo;
			}

			public DvbSubInfo[] newArray(int size) {
				return new DvbSubInfo[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public DvbSubInfo() {
		pid = 0;
		langCode = 0;
		shortLangStr = "";
		isHardOfHearing = 0;
	}

	public DvbSubInfo(Parcel src) {
		readFromParcel(src);
	}

	public void readFromParcel(Parcel src) {
		pid = src.readInt();
		langCode = src.readInt();
		shortLangStr = src.readString();
		isHardOfHearing = src.readInt();
	}
	
}
