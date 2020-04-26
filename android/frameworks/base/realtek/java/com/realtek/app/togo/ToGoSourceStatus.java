package com.realtek.app.togo;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class ToGoSourceStatus implements Parcelable {
	private static final String TAG = "ToGoSourceStatus";

	public static final String SOURCE_CAMERA = "Camera";
	public static final String SOURCE_HDMI_RX = "HDMI RX";
	public static final String SOURCE_FILE	= "FILE";
	public static final String SOURCE_DTV = "DTV";

	public static final int ID_OF_CAMERA = 0;
	public static final int ID_OF_HDMI_RX = 1;
	public static final int ID_OF_FILE = 2;
	public static final int ID_OF_DTV = 3;

	public int mCount;
	public String[] mSourceName;
	public int[] mSourceId;
	public int[] mIsSourceAvailable;

	public void writeToParcel(Parcel dest, int iflags) {
		/*for (int i=0;i<mSourceName.length;i++) {
			Log.v(TAG, "#" + i + ".name=" + mSourceName[i]);
		}
		for (int i=0;i<mSourceId.length;i++) {
			Log.v(TAG, "#" + i + ".id=" + mSourceId[i]);
		}
		for (int i=0;i<mIsSourceAvailable.length;i++) {
			Log.v(TAG, "#" + i + ".available=" + mIsSourceAvailable[i]);
		}*/
		dest.writeInt(mCount);
		dest.writeStringArray(mSourceName);
		dest.writeIntArray(mSourceId);
		dest.writeIntArray(mIsSourceAvailable);
	}

	public static final Parcelable.Creator<ToGoSourceStatus> CREATOR
		= new Parcelable.Creator<ToGoSourceStatus>() {
			public ToGoSourceStatus createFromParcel(Parcel src) {
				ToGoSourceStatus togoSourceStatus = new ToGoSourceStatus();
				togoSourceStatus.readFromParcel(src);
				return togoSourceStatus;
			}

			public ToGoSourceStatus[] newArray(int size) {
				return new ToGoSourceStatus[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public ToGoSourceStatus() {
		mCount = 4;
		mSourceName = new String [4];
		mSourceName[0] = SOURCE_CAMERA;
		mSourceName[1] = SOURCE_HDMI_RX;
		mSourceName[2] = SOURCE_FILE;
		mSourceName[3] = SOURCE_DTV;

		mSourceId = new int [4];
		mSourceId[0] = ID_OF_CAMERA;
		mSourceId[1] = ID_OF_HDMI_RX;
		mSourceId[2] = ID_OF_FILE;
		mSourceId[3] = ID_OF_DTV;

		mIsSourceAvailable = new int [4];
		for (int i=0;i<4;i++) {
			mIsSourceAvailable[i] = 0;
		}
	}

	public ToGoSourceStatus(Parcel src) {
		readFromParcel(src);
	}

	public void readFromParcel(Parcel src) {
		mCount = src.readInt();
		src.readStringArray(mSourceName);
		src.readIntArray(mSourceId);
		src.readIntArray(mIsSourceAvailable);
	}

}