package com.realtek.DataProvider;

import java.io.File;
import android.util.Log;
import java.util.ArrayList;
import android.net.Uri;

public abstract class AbstractDataProvider
{
    private static final boolean DEBUG = false;
    private static final String TAG = AbstractDataProvider.class.getSimpleName();
    public final static int MAGIC_CODE = 0x01000000;
    public final static int FILE_LIST_CREATE_DONE  = MAGIC_CODE|0x01;
    public final static int FILE_LIST_CREATE_PORTION  = MAGIC_CODE|0x02;
    public final static int FILE_LIST_CREATE_ABORT = MAGIC_CODE|0x10;
    private String mName;
	public static final int NETWORK_DISCONNECT = 0x1;
    private int mSystemState; 
	public void SetName(String name) 
	{ 
		this.mName =name;
	}
	
	public String GetName()
	{	
		return mName;
	}
	 
	public abstract String GetDataAt     (int i);
	public abstract String GetTitleAt    (int i);
	public abstract String GetMimeTypeAt (int i);
	public abstract int    GetIdAt       (int i);
	public abstract int    GetFileTypeAt (int i);
	public abstract int    GetSize       ();
	public abstract File GetFileAt(int i);
	public abstract void sortListByType();
	public abstract int getDirnum();
	public void StopQuery()
	{
	}
	public void UpdateData(int Type)
	{
	}
	public int GetSize(int Type)
	{
		return 0;
	}
    public String GetMediaInfo(int i, int type)
    {
        return null;
    }
    public long GetSizeAt(int i)
	{
		return 0;
	}
    public int GetItemIndex(String item){
        log("GetItemIndex item="+item);
        int size = GetSize();
        // Remove prefix "file://"
        if(item.startsWith("file://")){
            item = Uri.decode(item);
            item = item.substring("file://".length());
        }
        // Remove prefix "content://"
        //e.g. content://com.rtk.mediabrowser.provider/rtk_media/tmp/ramfs/mnt/172.21.181.120_DATA_smb/video/141230%20%EC%97%90%EC%9D%B4%ED%95%91%ED%81%AC(Apink)%20%26%20%EC%95%84%EC%9D%B4%ED%95%91%ED%81%AC%20%EC%97%B0%EC%98%88%EB%8C%80%EC%83%81%20LUV%20%26%20NoNoNo%201080p%2060fps.mp4
        if(item.startsWith("content://")){
            item = Uri.decode(item);
            item = item.substring("content://".length());
        }
        log("GetItemIndex remove decoded item="+item);
        for(int i=0;i<size;i++){
            String data = GetDataAt(i);
            log("GetItemIndex data="+data);
            if(data!= null && item.endsWith(data))
                return i;
        }
        return -1;
    }

    private static void log(String s) {
        if(DEBUG) Log.d(TAG, s);
    }

    public void SetSystemState(int state)
    {
        mSystemState = mSystemState |state;
    }
    public void ClearSystemState(int state)
    {
        mSystemState = mSystemState & (~state);
    }
    public boolean IsSystemState(int state)
    {
        if ((mSystemState & state) == state)
            return true;
        else
            return false;
    }
    public ArrayList<String> GetPlayList(int type)
    {
        return null; 
    } 
    public ArrayList<String> GetNameList(int type)
    {
        return null; 
    } 
}
