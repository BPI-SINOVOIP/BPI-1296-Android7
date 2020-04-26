package com.realtek.app.togo;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class ToGoClientStatus implements Parcelable {
	private static final String TAG = "ToGoClientStatus";

	public static final String TOGO_STATE_IDLE = "IDLE";
	public static final String TOGO_STATE_INITED = "INITED";
	public static final String TOGO_STATE_RUN = "RUN";
	public static final String TOGO_STATE_STOP = "STOP";

	public static final int ID_OF_STATE_IDLE = 0;
	public static final int ID_OF_STATE_INITED = 1;
	public static final int ID_OF_STATE_RUN = 2;
	public static final int ID_OF_STATE_STOP = 3;


	public int mClientMax;
	public int mCount;
	public int[] mClientId;
	public int[] mSourceType;
	public int[] mState;

	public void writeToParcel(Parcel dest, int iflags) {
		dest.writeInt(mClientMax);
		dest.writeInt(mCount);
		dest.writeIntArray(mClientId);
		dest.writeIntArray(mSourceType);
		dest.writeIntArray(mState);
	}

	public void readFromParcel(Parcel src) {
		mClientMax = src.readInt();
		mCount = src.readInt();
		src.readIntArray(mClientId);
		src.readIntArray(mSourceType);
		src.readIntArray(mState);
	}

	public static final Parcelable.Creator<ToGoClientStatus> CREATOR
		= new Parcelable.Creator<ToGoClientStatus>() {
			public ToGoClientStatus createFromParcel(Parcel src) {
				ToGoClientStatus togoClientStatus = new ToGoClientStatus();
				togoClientStatus.readFromParcel(src);
				return togoClientStatus;
			}

			public ToGoClientStatus[] newArray(int size) {
				return new ToGoClientStatus[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public ToGoClientStatus() {
		mClientMax = 0;
		mCount = 0;
		mClientId = new int[1];
		mClientId[0] = -1;
		mSourceType = new int[1];
		mSourceType[0] = -1;
		mState = new int[1];
		mState[0] = 0;
	}

	public ToGoClientStatus(Parcel src) {
		readFromParcel(src);
	}
	
	

}
