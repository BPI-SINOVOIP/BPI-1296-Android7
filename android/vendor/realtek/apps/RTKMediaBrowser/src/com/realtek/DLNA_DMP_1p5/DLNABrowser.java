package com.realtek.DLNA_DMP_1p5;


import java.util.ArrayList;
import java.util.List;
import android.util.Log;

public class DLNABrowser {

    private static final String TAG = "DLNABrowser";
    private static final boolean DEBUG = true;
    private final String ROOT_PATH = "/http://upnp/";
	private String mPath;
	private List<String> mCharIDList = new ArrayList<String>();
	
    private long MagicCode=0;
    private long m_LocalMagicCode=0;
    private static final Object lock = new Object();	
	private static DLNABrowser objDLNABrowser = null;
    private static int mHashCode = 0;
    private static boolean isRestart = false;
	public static DLNABrowser GetInstance()
	{
        synchronized (lock) {
            if (objDLNABrowser == null) {
                objDLNABrowser = new DLNABrowser();
                if (mHashCode != 0 && mHashCode != objDLNABrowser.hashCode()) {
                    isRestart = true;
                }
                mHashCode = objDLNABrowser.hashCode();
            }
            return objDLNABrowser;
        }
	}
	
	public static void DelInstance()
	{
        synchronized (lock) {
            DLNA_DMP_1p5.DLNA_DMP_1p5_UnInit();
            objDLNABrowser = null;
            isRestart = false;
            //System.gc();
        }
	}

    public static boolean isLoaded(){
        return objDLNABrowser!=null;
    }

    public static boolean isRestarted(){
        synchronized(lock)
        {
            return isRestart;
        }
    }

    public static void setRestarted(boolean restart){
        synchronized(lock)
        {
            isRestart = restart;
        }
    }

	private DLNABrowser()
	{
		DLNA_DMP_1p5.DLNA_DMP_1p5_Init();
		DLNA_DMP_1p5.DLNA_DMP_1p5_Start();
	}
    /*	
	@Override
	protected void finalize() throws Throwable
	{
		//do finalization here
		DLNA_DMP_1p5.DLNA_DMP_1p5_UnInit();
		super.finalize(); //not necessary if extending Object.
	} 
	*/
	public String GetPluginRootPath()
	{
		return ROOT_PATH;
	}
    public long GetMagicCode()
    {
        synchronized(lock)
        {
            return MagicCode;
        }
    }
    public void SetMagicCode(long code)
    {
        synchronized(lock)
        {
            MagicCode = code;
            m_LocalMagicCode = code;
            log("Set MagicCode ="+MagicCode);
        }
    }

    public void StopPreviousWork()
    {
        log("call DLNA_DMP_1p5_stopWatingResponse");
        DLNA_DMP_1p5.DLNA_DMP_1p5_stopWatingResponse();//Blocking API
    }
	public synchronized int GetTotalObjectSize(String path, String DLNA_UniqueID)
	{
		if (path == null || path.equals("")) return 0;

        log("GetTotalObjectSize path="+path);
		int size = 0;
        boolean goBrowse = false;
		if (path.equals(ROOT_PATH))
		{
            log("query server list, call DLNA_DMP_1p5_MediaServerSizes");
			mCharIDList.clear();
			size = DLNA_DMP_1p5.DLNA_DMP_1p5_MediaServerSizes();
			mPath = path;
		}
		else if (path.length() > mPath.length()) //MoveNext
		{
			String directory = path.substring(mPath.length());
			if (directory.indexOf("/") == 0)
				directory = directory.substring(1);
			if (directory.contains("/"))
				directory = directory.substring(0, directory.indexOf("/"));
			
            synchronized(lock)
            {
                if (m_LocalMagicCode ==MagicCode){goBrowse = true;}
            }
            if (goBrowse)
            {
			    MoveNext(directory, DLNA_UniqueID);
            }
			
            synchronized(lock)
            {
                if (m_LocalMagicCode == MagicCode)
			        size = DLNA_DMP_1p5.DLNA_DMP_1p5_BrowseTotalSizes();
                else
                    size = 0;
			}
			mPath += directory + "/";
		}
		else if (path.length() < mPath.length()) //MoveBack
		{
			String directory = path.substring(0, path.lastIndexOf("/"));
			directory = directory.substring(directory.lastIndexOf("/") + 1);
			
            synchronized(lock)
            {
                if (m_LocalMagicCode ==MagicCode){goBrowse = true;}
            }
            if (goBrowse)
            {
			    MoveBack(directory);
            }
			
            synchronized(lock)
            {
                if (m_LocalMagicCode == MagicCode)
			        size = DLNA_DMP_1p5.DLNA_DMP_1p5_BrowseTotalSizes();
                else
                    size = 0;
            }
			mPath = path;
		}
		else if (path.equals(mPath))
		{
            if (m_LocalMagicCode == MagicCode) {
                size = DLNA_DMP_1p5.DLNA_DMP_1p5_BrowseTotalSizes();
            } else {
                Log.e(TAG, "ID: " + Thread.currentThread().getId() + ", ERROR to use GetTotalObjectSize().");
                size = DLNA_DMP_1p5.DLNA_DMP_1p5_MediaContainerObjectSizes();
            }
		}
	    Log.d(TAG, "ID: " + Thread.currentThread().getId() + ", GetTotalObjectSize:" + size +", path="+path);
		return size;
	}

    public synchronized int GetDirectorySize()
    {
        int Size;
        if (mPath.equals(ROOT_PATH))
        {
            Size = DLNA_DMP_1p5.DLNA_DMP_1p5_MediaServerSizes();
        }else{
            Size = DLNA_DMP_1p5.DLNA_DMP_1p5_MediaContainerObjectSizes();
        }
	    Log.d(TAG,"ID: "+Thread.currentThread().getId()+", GetDirectorySize:"+Size);
        return Size;
    }	
	
	public synchronized String GetDirectoryTitle(int index)
	{		
		String title = null;
		if (mPath.equals(ROOT_PATH))
		{
			if (index < DLNA_DMP_1p5.DLNA_DMP_1p5_MediaServerSizes())
				title = DLNA_DMP_1p5.DLNA_DMP_1p5_MediaServerName(index);
		}
		else
		{
			if (index < DLNA_DMP_1p5.DLNA_DMP_1p5_MediaContainerObjectSizes())
				title = DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceContentDirectory(index);
		}
		return title;
	}
    public synchronized String GetDirectoryUniqueID(int index)
    {
        return DLNA_DMP_1p5.DLNA_DMP_1p5_queryContainerObjInfoByIndex(index).UniqueCharID;
    }	
	
    public synchronized int GetItemSize()
    {
        int Size;
        Size = DLNA_DMP_1p5.DLNA_DMP_1p5_MediaItemObjectSizes();
	    Log.d(TAG,"ID: "+Thread.currentThread().getId()+", GetItemSize:"+Size);
        return Size;
    }
	
	public synchronized String GetItemTitle(int index)
	{
		if (index >= DLNA_DMP_1p5.DLNA_DMP_1p5_MediaItemObjectSizes()) return null;
		return DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceContentFile(index);
	}
    public synchronized String GetItemUniqueID(int index)
    {
        return DLNA_DMP_1p5.DLNA_DMP_1p5_queryItemObjInfoByIndex(index).UniqueCharID;
    }	
	public synchronized int GetMediaType(String filename)
	{
		return DLNA_DMP_1p5.DLNA_DMP_1p5_GetMediaType(filename);
	}
	
    public String GetItemData(String UniqueID)
	{
        return GetItemMediaInfo(UniqueID, UPNP_DMP_RES_TYPE.UPNP_DMP_RES_URI);
	}
    public synchronized String QueryAndGetItemMediaInfo(String UniqueID, int type)
    {
        if(!DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceBrowseMetadata(UniqueID)){
            log("UPnPServiceBrowse ID["+UniqueID+"] failed");
            return "false";
        }
        return GetItemMediaInfo(UniqueID, type);
    }
    public synchronized String GetItemMediaInfo(String UniqueID, int type)
    {
        String mediaInfo = DLNA_DMP_1p5.DLNA_DMP_1p5_queryResourceByID(UniqueID, type);
        if (mediaInfo == null){
            log("[Wrong] Get null info of DLNA UniqueID:"+UniqueID+ " type:"+type);
        }
	    return mediaInfo;
    }
    
    public synchronized ArrayList<String> GetSubtitleList(String UniqueCharID)
    {
        ArrayList<String> AllSubList = new ArrayList<String>();
        if (UniqueCharID != null)
        {
            Object[] SubList = DLNA_DMP_1p5.DLNA_DMP_1p5_querySubtitleURIListByID(UniqueCharID, "text/sub");
            if (SubList != null && SubList.length>0){
                for (int i=0; i<SubList.length;i++){
                    String Sub = "<text/sub>"+SubList[i];
                    AllSubList.add(Sub);
                    //Log.d(TAG,"[Sub]["+i+"]=" + SubList[i]);
                }
            }
            SubList = DLNA_DMP_1p5.DLNA_DMP_1p5_querySubtitleURIListByID(UniqueCharID, "text/idx");
            if (SubList != null && SubList.length>0){
                for (int i=0; i<SubList.length;i++){
                    String Sub = "<text/idx>"+SubList[i];
                    AllSubList.add(Sub);
                    //Log.d(TAG,"[idx]["+i+"]=" + SubList[i]);
                }
            }
            SubList = DLNA_DMP_1p5.DLNA_DMP_1p5_querySubtitleURIListByID(UniqueCharID, "text/smi");
            if (SubList != null && SubList.length>0){
                for (int i=0; i<SubList.length;i++){
                    String Sub = "<text/smi>"+SubList[i];
                    AllSubList.add(Sub);
                    //Log.d(TAG,"[smi]["+i+"]=" + SubList[i]);
                }
            }
            SubList = DLNA_DMP_1p5.DLNA_DMP_1p5_querySubtitleURIListByID(UniqueCharID, "text/srt");
            if (SubList != null && SubList.length>0){
                for (int i=0; i<SubList.length;i++){
                    String Sub = "<text/srt>"+SubList[i];
                    AllSubList.add(Sub);
                    //Log.d(TAG,"[srt]["+i+"]=" + SubList[i]);
                }
            }
            SubList = DLNA_DMP_1p5.DLNA_DMP_1p5_querySubtitleURIListByID(UniqueCharID, "text/ssa");
            if (SubList != null && SubList.length>0){
                for (int i=0; i<SubList.length;i++){
                    String Sub = "<text/ssa>"+SubList[i];
                    AllSubList.add(Sub);
                    //Log.d(TAG,"[ssa]["+i+"]=" + SubList[i]);
                }
            }
            SubList = DLNA_DMP_1p5.DLNA_DMP_1p5_querySubtitleURIListByID(UniqueCharID, "text/plain");
            if (SubList != null && SubList.length>0){
                for (int i=0; i<SubList.length;i++){
                    String Sub = "<text/plain>"+SubList[i];
                    AllSubList.add(Sub);
                    //Log.d(TAG,"[plain]["+i+"]=" + SubList[i]);
                }
            }
		    return AllSubList;
        }
        else
        {
            log("[Wrong] UniqueCharID is null");
            return null;
        }
    }
	
    public synchronized String GetItemMimeType(int index)
    {
        UPnPObjInfo UPnPObj = DLNA_DMP_1p5.DLNA_DMP_1p5_queryItemObjInfoByIndex(index);
        String Mimetype = null;
        if (UPnPObj != null)
        {
	        String ProtocolInfo = UPnPObj.ProtocolInfo;
            if (ProtocolInfo != null)
            {
                int ptr1 = ProtocolInfo.indexOf(":");
                int ptr2;
                if (ptr1 >0)
                {
                    ptr2 = ProtocolInfo.indexOf(":", ptr1+1);
                    ptr1 = ProtocolInfo.lastIndexOf(":");
                    Mimetype = ProtocolInfo.substring(ptr2+1,ptr1);
                }
                //Log.d(TAG,"ProtocolInfo:"+ProtocolInfo);
                //Log.d(TAG,"Mimetype:"+Mimetype);
            }
        }
	    	
		return Mimetype;
    }

	private synchronized void MoveNext(String directory, String DLNA_UniqueID)
	{
        log("MoveNext tid="+Thread.currentThread().getId());
		if (mPath.equals(ROOT_PATH))
		{
			DLNA_DMP_1p5.DLNA_DMP_1p5_setMediaServerByFriendlyName(directory);
			DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceDeviceBrowse();
			mCharIDList.add(directory);
		}
		else
		{
			int ret = DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceBrowse(DLNA_UniqueID, 0, 1);
			mCharIDList.add(DLNA_UniqueID);
		}
	}
	
	private synchronized void MoveBack(String directory)
	{
        log("MoveBack tid="+Thread.currentThread().getId());
		mCharIDList.remove(mCharIDList.size()-1);
		switch(mCharIDList.size())
		{
			case 0:
				break;
			case 1:
				DLNA_DMP_1p5.DLNA_DMP_1p5_setMediaServerByFriendlyName(directory);
				DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceDeviceBrowse();
				break;
			default:
                int ret = DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceBrowse(mCharIDList.get(mCharIDList.size()-1), 0, 1);
				break;
		}
	}

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    public synchronized int BrowseCurDirStep(int StartIndex, int count)
    {
        boolean goBrowse = false;
        int size = 0;
        synchronized(lock)
        {
            if (m_LocalMagicCode ==MagicCode){goBrowse = true;}
        }
        if (goBrowse)
        {
            if (mCharIDList.size()>1){
	            size = DLNA_DMP_1p5.DLNA_DMP_1p5_UPnPServiceBrowse(mCharIDList.get(mCharIDList.size()-1), StartIndex, count);
            }
        }
        return size;
    }
}
