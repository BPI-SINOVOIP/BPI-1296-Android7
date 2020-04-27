package com.realtek.DLNA_DMP_1p5;

import com.realtek.DataProvider.FileFilterType;

public class FileAttr {
	private int mFileType = FileFilterType.DEVICE_FILE_NONE;
	private int mIndex = -1;
    private String mFileName; 
    private String mDLNA_Unique_ID; 
	
	public FileAttr(String name, String ID, int fileType, int index)
	{
		mFileType = fileType;
		mIndex = index;
        mFileName = name;
        mDLNA_Unique_ID = ID;
	}
	
	public int GetFileType()
	{
		return mFileType;
	}
	
	public int GetIndex()
	{
		return mIndex;
	}
	public String GetName()
	{
		return mFileName;
	}
	public String GetDLNAUniqueID()
	{
		return mDLNA_Unique_ID;
	}
}
