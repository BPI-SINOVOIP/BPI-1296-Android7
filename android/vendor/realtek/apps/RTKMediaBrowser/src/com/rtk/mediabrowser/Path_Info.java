package com.rtk.mediabrowser;

import java.util.ArrayList;
import java.util.List;
import android.util.Log;
import android.os.Parcel;
import android.os.Parcelable;


import com.rtk.mediabrowser.Level_Info;

public class Path_Info	implements Parcelable 
{
	
	public int dirLevel = -1;
    private static String TAG="Path_Info";	
	public List<Level_Info> curPathArr = new ArrayList<Level_Info>();
	
	// level info handling
	public void addLevelInfo(String path)
	{
	    Level_Info info = new Level_Info();
	    info.path = path;
	    info.position = 0;
	    curPathArr.add(info);
	    dirLevel++;
	}
	    
    public void cleanLevelInfo()
    {
        curPathArr.clear();
	    dirLevel = -1;     		            
	}
		    
    public void backToLastLevel() 
	{
        Log.d(TAG, "backToLastLevel");
        curPathArr.remove(dirLevel);
		dirLevel--;		            	
		//getFileListByPath(curPathArr.get(dirLevel).path, MediaUtils.getFileExt(cur_type));
	}  
    
    public void setLevelFocus(int level, int position)
    {
        curPathArr.get(level).position = position;
    }
    
    public void setLastLevelFocus(int position)
    {
        curPathArr.get(dirLevel).position = position;
    }
    
	    
	public int getLastLevel()
	{
	    return dirLevel;
	}
	    
	public String getLastLevelPath()
	{
	    return curPathArr.get(dirLevel).path;
	}
	    
	   
	public int getLastLevelFocus()
	{
	    return curPathArr.get(dirLevel).position;
	}
	//requirement of Parcelable
	public int describeContents(){
		return 0;
	}
	public void writeToParcel(Parcel out, int flags){
		out.writeInt(dirLevel);
		out.writeList(curPathArr);	
	}
	public static final Parcelable.Creator<Path_Info> CREATOR = new Parcelable.Creator<Path_Info>(){
		public Path_Info createFromParcel(Parcel in){
			return new Path_Info(in);
		}
		public Path_Info[] newArray(int size){
			return new Path_Info[size];
		}
	};
	private Path_Info(Parcel in)
	{
		dirLevel = in.readInt();
		in.readList(curPathArr, Path_Info.class.getClassLoader());
	}
	public Path_Info()
	{
	}
}
