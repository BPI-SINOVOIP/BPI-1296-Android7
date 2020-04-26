package com.realtek.hardware;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class HDMIRxStatus implements Parcelable {
	public int type;
	public int status;
	public int width;
	public int height;
	public int scanMode;
	public int color;
	public int freq;
    public int spdif;
    public int audioStatus;

	public static final int UNKNOWN = -1;

	public static final int TYPE_HDMI_RX = 0;
	public static final int TYPE_MIPI = 1;

	public static final int STATUS_NOT_READY = 0;
	public static final int STATUS_READY = 1;

	public static final int SCANMODE_PROGRESSIVE = 0;
	public static final int SCANMODE_INTERLACED = 1;

	public static final int COLOR_RGB = 0;
	public static final int COLOR_YUV422 = 1;
	public static final int COLOR_YUV444 = 2;
	public static final int COLOR_YUV411 = 3;

    public static final int SPDIF_LPCM = 0;
    public static final int SPDIF_RAW = 1;

	/**
	* Sticky broadcast of the current HDMI plugged state.
	*/
	public final static String ACTION_HDMIRX_PLUGGED = "android.intent.action.HDMIRX_PLUGGED";
    public final static String ACTTION_HDMIRX_AUDIO_CHANGED = "android.intent.action.HDMIRX_AUDIO_CHANGED";
    public final static String ACTTION_HDMIRX_HDCP_CHANGED = "android.intent.action.HDMIRX_HDCP_STATUS";

    /**
 	* Extra in {@link #ACTION_HDMIRX_PLUGGED} indicating the state: true if
 	* plugged in to HDMI, false if not.
 	*/
    public final static String EXTRA_HDMIRX_PLUGGED_STATE = "state";
    public final static String EXTRA_HDMIRX_AUDIO_STATE = "state";
    public final static String EXTRA_HDMIRX_HDCP_STATUS = "state";
	public HDMIRxStatus() {
		type = UNKNOWN;
		status = STATUS_NOT_READY;
		width = 0;
		height = 0;
		scanMode = UNKNOWN;
		color = UNKNOWN;
		freq = 0;
        spdif = SPDIF_LPCM;
        audioStatus = STATUS_NOT_READY;
	}

	protected HDMIRxStatus(Parcel in) {
		type = in.readInt();
		status = in.readInt();
		width = in.readInt();
		height = in.readInt();
		scanMode = in.readInt();
		color = in.readInt();
		freq = in.readInt();
        spdif = in.readInt();
        audioStatus = in.readInt();
	}

	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel dest, int flags) {
		dest.writeInt(type);
		dest.writeInt(status);
		dest.writeInt(width);
		dest.writeInt(height);
		dest.writeInt(scanMode);
		dest.writeInt(color);
		dest.writeInt(freq);
        dest.writeInt(spdif);
        dest.writeInt(audioStatus);
	}

	@SuppressWarnings("unused")
	public static final Parcelable.Creator<HDMIRxStatus> CREATOR = new Parcelable.Creator<HDMIRxStatus>() {
		@Override
		public HDMIRxStatus createFromParcel(Parcel in) {
			return new HDMIRxStatus(in);
		}

		@Override
		public HDMIRxStatus[] newArray(int size) {
			return new HDMIRxStatus[size];
		}
	};
}
