package com.realtek.app.tv;

import android.os.Parcel;
import android.os.Parcelable;
import android.util.Log;

public class EpgProgram implements Parcelable {
	private static final String TAG = "EpgProgram";
	public int isValid;
	public int eventID;
	public int beginDateTime;
	public int endDateTime;
	public String iso639LanguageCode;
	public int titleLen;
	public String title;
	public int detailDescrLen;
	public String detailDescr;
	public byte rating;
	public byte audioNum;
	public byte hasSubtitle;
	public byte hasTeletext;
	public byte hasTTXSubtitle;
	public byte isScrambled;

	// Marshaler
	public void writeToParcel(Parcel dest, int iflags) {
		dest.writeInt(isValid);
		dest.writeInt(eventID);
		dest.writeInt(beginDateTime);
		dest.writeInt(endDateTime);
		dest.writeString(iso639LanguageCode);
		dest.writeInt(titleLen);
		dest.writeString(title);
		dest.writeInt(detailDescrLen);
		dest.writeString(detailDescr);
		dest.writeByte(rating);
		dest.writeByte(audioNum);
		dest.writeByte(hasSubtitle);
		dest.writeByte(hasTeletext);
		dest.writeByte(hasTTXSubtitle);
		dest.writeByte(isScrambled);
	}

	// Unmarshaler
	public static final Parcelable.Creator<EpgProgram> CREATOR
		= new Parcelable.Creator<EpgProgram>() {
			public EpgProgram createFromParcel(Parcel src) {
				EpgProgram epgProgram = new EpgProgram();
				epgProgram.readFromParcel(src);
				return epgProgram;
			}

			public EpgProgram[] newArray(int size) {
				return new EpgProgram[size];
			}
		};

	public int describeContents() {
		return 0;
	}

	public EpgProgram() {
		isValid = 0;
		eventID = 0;
		beginDateTime = 0;
		endDateTime = 0;
		iso639LanguageCode = "";
		titleLen = 0;
		title = "";
		detailDescrLen = 0;
		detailDescr = "";
		rating = 0;
		audioNum = 0;
		hasSubtitle = 0;
		hasTeletext = 0;
		hasTTXSubtitle = 0;
		isScrambled = 0;
	}

	public EpgProgram(Parcel src) {
		readFromParcel(src);
	}

	public void readFromParcel(Parcel src) {
		isValid = src.readInt();
		eventID = src.readInt();		
		beginDateTime = src.readInt();
		endDateTime = src.readInt();
		iso639LanguageCode = src.readString();
		titleLen = src.readInt();
		title = src.readString();
		detailDescrLen = src.readInt();
		detailDescr = src.readString();
		rating = src.readByte();
		audioNum = src.readByte();
		hasSubtitle = src.readByte();
		hasTeletext = src.readByte();
		hasTTXSubtitle = src.readByte();
		isScrambled = src.readByte();
	}
	
}
