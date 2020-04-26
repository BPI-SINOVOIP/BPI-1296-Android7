package com.realtek.app.tv;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class TtxSubInfo implements Parcelable {
	private static final String TAG = "TtxSubInfo";
	public int pid;
	public int langCode;
	public String shortLangStr;
	public int magazineNum;
	public int pageNum;

	// Marshaler
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(pid);
		dest.writeInt(langCode);
		dest.writeString(shortLangStr);
		dest.writeInt(magazineNum);
		dest.writeInt(pageNum);
	}

	// Unmarshaler
	public static final Parcelable.Creator<TtxSubInfo> CREATOR
		= new Parcelable.Creator<TtxSubInfo>() {
			public TtxSubInfo createFromParcel(Parcel src) {
				TtxSubInfo ttxSubInfo = new TtxSubInfo();
				ttxSubInfo.readFromParcel(src);
				return ttxSubInfo;
			}

			public TtxSubInfo[] newArray(int size) {
				return new TtxSubInfo[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public TtxSubInfo() {
		pid = 0;
		langCode = 0;
		shortLangStr = "";
		magazineNum = 0;
		pageNum = 0;
	}

	public TtxSubInfo(Parcel src) {
		readFromParcel(src);
	}

	public void readFromParcel(Parcel src) {
		pid = src.readInt();
		langCode = src.readInt();
		shortLangStr = src.readString();
		magazineNum = src.readInt();
		pageNum = src.readInt();
	}
	
}

