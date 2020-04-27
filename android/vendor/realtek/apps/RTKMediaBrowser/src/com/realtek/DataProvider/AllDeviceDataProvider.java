package com.realtek.DataProvider;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.util.Log;
import com.realtek.DLNA_DMP_1p5.DLNABrowser;
import com.realtek.Utils.StorageUtils;
import com.realtek.rtksmb.Computer;
import com.realtek.rtksmb.SubnetScanner;
import jcifs.netbios.NbtAddress;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class AllDeviceDataProvider  extends AbstractDataProvider
{	
	private static final String TAG = "AllDeviceDataProvider";
	private static final boolean DEBUG = true;
	private Handler mHandler = null;
    DLNAFileDataProvider mDLNAFileDataProvider = null;
    DLNAFileDataProvider mDLNAFileDataProvider_bg = null;
    SMBFileDataProvider mSMBFileDataProvider = null;
    private ArrayList<String> mUSBDeviceList = new ArrayList<String>();
    private ArrayList<String> mUSBDeviceList_bg = new ArrayList<String>();
    private ArrayList<String> mUSBDeviceList_Title = new ArrayList<String>();
    private ArrayList<String> mUSBDeviceList_Title_bg = new ArrayList<String>();
    private ArrayList<Computer> mSMBDeviceList = new ArrayList<>();
    private ArrayList<Computer> mSMBDeviceList_bg = new ArrayList<>();
    private ArrayList<String> mSMBDomainList_bg = new ArrayList<String>();
    private int mSMBDomainSize = 0;
    private int mDMSSize = 0;
    private int mDMSSize_bg = 0;
	private Context mContext;

    public static final int USB_QUERY_DONE = 1;
    public static final int DMS_QUERY_DONE = 2;
    public static final int SMB_QUERY_DOMAIN_DONE = 4;
    public static final int SMB_QUERY_ALL_DOMAIN_DONE = 8;
    public static final int DMS_QUERY_ABORT = 16;
    public static final int SMB_QUERY_DOMAIN_ABORT = 32;

	public static final int UPDATE_QUERY_USB = 1;	
	public static final int UPDATE_QUERY_DMS = 2;	
	public static final int UPDATE_QUERY_SMB = 4;	
	public static final int UPDATE_SMB = 8;
    
	private static boolean StopDevQuery = true;
	private static final Object lock = new Object();
	private SubnetScanner subnetScanner;

	public AllDeviceDataProvider (Handler handler, Context context, int type)
	{
		super.SetName("AllDeviceDataProvider");
		
	    mHandler = handler;
		mContext = context;
	    CreateFileListArray(type);
		
	}
	
	public String GetDataAt(int i)
	{

	    synchronized(mSMBDeviceList) 
        {   
           
    		if (i < mUSBDeviceList.size())
            {
                return mUSBDeviceList.get(i);
            }
            else if (i < (mUSBDeviceList.size() + mDMSSize))
            {
                return mDLNAFileDataProvider.GetDataAt(i - mUSBDeviceList.size());
            }
            else if (i < GetSize())
            {
                return "smb://"+mSMBDeviceList.get(i - mUSBDeviceList.size() - mDMSSize).addr+"/";
            }
        }
        return null;
	}

	public File GetFileAt(int i)
	{
		return null;
	}

	public String GetTitleAt    (int i)
	{

	    synchronized(mSMBDeviceList) 
        {
    		if (i < mUSBDeviceList.size())
            {
				return mUSBDeviceList_Title.get(i);
            }
            else if (i < (mUSBDeviceList.size()+ mDMSSize))
            {
                return mDLNAFileDataProvider.GetTitleAt(i - mUSBDeviceList.size());
            }
            else if (i < GetSize())
            {
				String smbTitle = mSMBDeviceList.get(i - mUSBDeviceList.size() - mDMSSize).name;
				if(smbTitle==null)
					smbTitle=mSMBDeviceList.get(i - mUSBDeviceList.size() - mDMSSize).addr;
                String[] segs = smbTitle.split("/");
                if (segs.length ==0)
                {
                    Log.d(TAG,"ERROR String["+mSMBDeviceList.get(i - mUSBDeviceList.size() - mDMSSize)+"]");
                }
                return segs[segs.length - 1];
            }
        }
        return null;
	}
	
	public  String GetMimeTypeAt (int i)
	{
		return null;
	}
	
	public int GetIdAt(int i)
	{
		
		return -1;
	}
	
	public int GetSize ()
	{
        synchronized(mSMBDeviceList) 
        {   
            return mUSBDeviceList.size() + mDMSSize + mSMBDeviceList.size();
        }
    }
	public int GetSize(int type)
	{
		if (type==FileFilterType.DEVICE_FILE_DEVICE)
		{
			return mUSBDeviceList.size();
		}
		else if (type==FileFilterType.DEVICE_FILE_DLNA_DEVICE)
		{
			return mDMSSize;
		}
		else if (type==FileFilterType.DEVICE_FILE_SMB_DEVICE)
		{
			return mSMBDeviceList.size(); 
		}
		else
		{
			return 0;
		}
	}
	
	public int GetFileTypeAt(int i)
	{
	    synchronized(mSMBDeviceList) 
        {   
    	    if (i < mUSBDeviceList.size())
            {
                return FileFilterType.DEVICE_FILE_DEVICE;
            }
            else if (i < (mUSBDeviceList.size() + mDMSSize))
            {
                return FileFilterType.DEVICE_FILE_DLNA_DEVICE;
            }
            else if (i < GetSize())
            {
                return FileFilterType.DEVICE_FILE_SMB_DEVICE;
            }
		    return FileFilterType.DEVICE_FILE_NONE;
       }
	}
	public void UpdateData(int Type)
	{
		synchronized(lock)
		{
			StopDevQuery = false;
		}
		if ((Type & UPDATE_QUERY_USB) ==UPDATE_QUERY_USB)
			CreateUSBDeviceArray();	
		if ((Type & UPDATE_QUERY_DMS) ==UPDATE_QUERY_DMS)
			CreateDMSDeviceArray();	
 
		if ((Type & UPDATE_QUERY_SMB) ==UPDATE_QUERY_SMB)
			CreateSMBDeviceArray(true);
		else if ((Type & UPDATE_SMB) ==UPDATE_SMB)
			CreateSMBDeviceArray(false);	

	}
	private void CreateFileListArray(int Type)
	{
		synchronized(lock)
		{
			StopDevQuery = false;
		}
		if ((Type & UPDATE_QUERY_USB) ==UPDATE_QUERY_USB)
			CreateUSBDeviceArray();	
		if ((Type & UPDATE_QUERY_DMS) ==UPDATE_QUERY_DMS)
			CreateDMSDeviceArray();	
 
		if ((Type & UPDATE_QUERY_SMB) ==UPDATE_QUERY_SMB)
			CreateSMBDeviceArray(true);	
	}
	private void CreateUSBDeviceArray()
	{
        //USB Device
		//Update latest dev list to forward
		Log.d(TAG,"CreateUSBDeviceArray Start");
		if (mUSBDeviceList!= null)
		{
			mUSBDeviceList.clear();
			mUSBDeviceList_Title.clear();
		}
        mUSBDeviceList = new ArrayList<String>(mUSBDeviceList_bg);
        mUSBDeviceList_Title = new ArrayList<String>(mUSBDeviceList_Title_bg);
        mUSBDeviceList_bg.clear();
        mUSBDeviceList_Title_bg.clear();

		List<StorageUtils.StorageInfo> storageList = StorageUtils.getStorageList(mContext);
		for(StorageUtils.StorageInfo info : storageList){
			mUSBDeviceList_bg.add(info.path);
			mUSBDeviceList_Title_bg.add(info.getDisplayName());
		}

		if (!StopDevQuery)
		{
			mHandler.sendEmptyMessage(USB_QUERY_DONE);
		}
	}
	private void CreateDMSDeviceArray()	
	{
        //DMS Device
		Log.d(TAG, "CreateDMSDeviceArray Start");
		if (mDLNAFileDataProvider_bg != null)
		{
			mDLNAFileDataProvider = mDLNAFileDataProvider_bg;
			mDLNAFileDataProvider_bg = null;
			mDMSSize = mDMSSize_bg;
		}
        else
        {
            mDLNAFileDataProvider = null;
            mDMSSize = 0;
        }

		new QueryDLNATask().execute();
	}

	class QueryDLNATask extends AsyncTask<Void, Void, Void>{

		@Override
		protected Void doInBackground(Void... voids) {
			log("update dlna data start");
			//DLNA Device
			mDLNAFileDataProvider_bg = new DLNAFileDataProvider(DLNABrowser.GetInstance().GetPluginRootPath(),
					FileFilterType.DEVICE_FILE_DIR,
					-1, 0, null);
			mDLNAFileDataProvider_bg.UpdateData(0);
			mDMSSize_bg = mDLNAFileDataProvider_bg.GetSize();
			if (IsSystemState(NETWORK_DISCONNECT))
			{
				mDLNAFileDataProvider_bg = null;
				mHandler.sendEmptyMessage(DMS_QUERY_ABORT);
			}
			else if (!StopDevQuery)
			{
				mHandler.sendEmptyMessage(DMS_QUERY_DONE);
			}
			log("update dlna data end");
			return null;
		}
	}

	private static void log(String s){
		if(DEBUG)
			Log.d(TAG, s);
	}

	private void CreateSMBDeviceArray(boolean QueryAgain)
	{
        //SMB Device
		Log.d(TAG,"CreateSMBDeviceArray Start QueryAgain:"+QueryAgain);
	    synchronized(mSMBDeviceList)
        { 
	        if (mSMBDeviceList!= null)
	        {
	        	mSMBDeviceList.clear();
	        }
	        mSMBDeviceList = new ArrayList<Computer>(mSMBDeviceList_bg);
        }
		if (QueryAgain)
		{
			log("subnetScanner method");
			subnetScanner = new SubnetScanner(mContext);
			subnetScanner.setObserver(new SubnetScanner.ScanObserver() {
				@Override
				public void computerFound(final Computer computer) {
					if(computer.name!=null && computer.name.equals(NbtAddress.MASTER_BROWSER_NAME)) return;
//						log("computerFound:"+computer.name+","+computer.addr);
					if(!mSMBDeviceList_bg.contains(computer))
						mSMBDeviceList_bg.add(computer);
					mHandler.sendEmptyMessage(SMB_QUERY_DOMAIN_DONE);
				}

				@Override
				public void searchFinished() {
					mHandler.sendEmptyMessage(SMB_QUERY_ALL_DOMAIN_DONE);
				}
			});
			mHandler.postDelayed(new Runnable() {
				@Override
				public void run() {
					subnetScanner.start();
				}
			}, 100);
		}
		Log.d(TAG,"CreateSMBDeviceArray End QueryAgain:"+QueryAgain);
	}
	public void StopQuery()
	{
		synchronized(lock)
		{
			StopDevQuery = true;
		}
		if(subnetScanner!=null) {
			subnetScanner.interrupt();
			subnetScanner = null;
		}
	}

	public int getDirnum() {
		return 0;
	}

	public void sortListByType() {
	}

	public void clearNetworkItems(){
		if(mSMBDeviceList!=null){
			mSMBDeviceList.clear();
		}
		if(mSMBDeviceList_bg!=null){
			mSMBDeviceList_bg.clear();
		}
		mDLNAFileDataProvider = null;
		mDLNAFileDataProvider_bg = null;
		mDMSSize = 0;
		mDMSSize_bg = 0;
	}
}
