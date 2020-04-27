package com.realtek.DataProvider;
import java.io.File;
import java.util.ArrayList;





public class HttpTestDataProvider  extends AbstractDataProvider 
{	
	private static final String TAG = "HttpTestDataProvider";
	private int   mTotalCnt=0;
	private ArrayList<String> mDataStringArray =  new ArrayList<String>(); 
	
	public HttpTestDataProvider ()
	{
		super.SetName("HttpTestDataProvider");

	    CreateDataStringtArray();
		
	}
	
	public String GetDataAt     (int i)
	{
		return mDataStringArray.get(i);
	}

	public File GetFileAt(int i)
	{	
		return null;
	}

	public String GetTitleAt    (int i)
	{
		return i+".jpg";
	}
	
	public  String GetMimeTypeAt (int i)
	{


		return null;
	}
	
	public int GetIdAt(int i)
	{
		return i;
		
	}
	
	public int GetSize ()
	{
		return 3;
	}
	
	public int GetFileTypeAt(int i)
	{
		return FileFilterType.DEVICE_FILE_PHOTO;
	}
	
	private void CreateDataStringtArray()
	{
	
	    
		 for(int i = 0; i < 3; i++) 
		 { 
			 String tmpStr= "http://172.21.98.227/"+i+".jpg";
			 mDataStringArray.add(tmpStr);
              
         }
	}



	
	public int getDirnum() {
		return 0;
	}

	public void sortListByType() {
	}
	

}
