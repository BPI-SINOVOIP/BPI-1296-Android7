package com.realtek.DataProvider;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;

import android.os.Handler;
import android.os.Message;
import android.os.AsyncTask;

import android.util.Log;

import com.realtek.Utils.MimeTypes;
import com.realtek.DLNA_DMP_1p5.DLNABrowser;
import com.realtek.DLNA_DMP_1p5.UPNP_DMP_RES_TYPE;
import com.realtek.DLNA_DMP_1p5.FileAttr;

public class DLNAFileDataProvider extends AbstractDataProvider {

    private static final String TAG = "DLNAFileDataProvider";
	//Below is sync from DLNA JNI
	private final static int JNI_DEVICE_FILE_AUDIO = 0x00000001;
	private final static int JNI_DEVICE_FILE_PHOTO = 0x00000010;
	private final static int JNI_DEVICE_FILE_VIDEO = 0x00000100;
	private final static int JNI_DEVICE_FILE_DIR   = 0x00001000;
	private final static int JNI_DEVICE_FILE_NONE  = 0x00010000;
    private static final boolean DEBUG = true;

    public static final int QUERY_TIMEOUT = 3000;

    private String mPath = "";
	private String mUniqueID = "";
	private int mFileType;
    private int mPortion = 512;
	private int mLimitCnt = 0;
	private int mSortMode = 0;
	private MimeTypes mMimeTypes = null;
    private final Object mDLNALibBrowseLock = new Object();
	private DLNABrowser mDLNABrowser;
	private List<FileAttr> mTotalFileList = new ArrayList<FileAttr>();
	private List<FileAttr> mTotalFileList_bg = new ArrayList<FileAttr>();
	private List<FileAttr> mDirectoryList = new ArrayList<FileAttr>();
	private List<FileAttr> mAudioFileList = new ArrayList<FileAttr>();
	private List<FileAttr> mPhotoFileList = new ArrayList<FileAttr>();
	private List<FileAttr> mVideoFileList = new ArrayList<FileAttr>();
	private List<FileAttr> mNoneFileList = new ArrayList<FileAttr>();
    private ArrayList<String> mPlaylist_Audio = new ArrayList<String>();	
    private ArrayList<String> mPlaylist_Video = new ArrayList<String>();	
    private ArrayList<String> mPlaylist_Photo = new ArrayList<String>();	
    private ArrayList<String> mNamelist_Audio = new ArrayList<String>();	
    private ArrayList<String> mNamelist_Video = new ArrayList<String>();	
    private ArrayList<String> mNamelist_Photo = new ArrayList<String>();	
 
    private static HashMap<String, String> DirectoryUniqueIDMap = new HashMap<String, String>();//<k,v>, key is path, v is Unique ID

    private Handler mCallerHandler = null;
    private FileListTask mCreateTotalFileLstTask = null;
    private boolean mIsCreateTotalFileList;

    public DLNAFileDataProvider (String path, int fileType,int limit,int sortMode,MimeTypes mimeTypes, Handler handler)
	{
        log("DLNAFileDataProvider constructor1");
		super.SetName("DLNAFileDataProvider");
	    mPath     =path;
        String tmp = path.substring(0,path.length()-1);//expected path ex :/http://upnp/RTMM5405-X230 : TVersity Media Server/Photos/All/
        int Dirs = tmp.lastIndexOf("/");
        String Directory = tmp.substring(Dirs+1);
        mUniqueID = DirectoryUniqueIDMap.get(Directory);        
        Log.d(TAG,"Directory:"+Directory+", UniqueID:"+mUniqueID);
	    mFileType =fileType; 
	    mLimitCnt =limit;
	    mSortMode =sortMode;
	    mMimeTypes = mimeTypes;
        mCallerHandler = handler;
	    
	    mDLNABrowser = DLNABrowser.GetInstance();
        mCreateTotalFileLstTask = new FileListTask();
        mCreateTotalFileLstTask.execute();
	}
	public DLNAFileDataProvider (String path, int fileType,int limit,int sortMode,MimeTypes mimeTypes)
	{
        log("DLNAFileDataProvider constructor2");
		super.SetName("DLNAFileDataProvider");
	    mPath     =path;
	    mFileType =fileType; 
	    mLimitCnt =limit;
	    mSortMode =sortMode;
	    mMimeTypes = mimeTypes;
	    
	    mDLNABrowser = DLNABrowser.GetInstance();
        mDLNABrowser.SetMagicCode(Thread.currentThread().getId());

	    CreateTotalFileLst();
	}

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    @Override
	public String GetDataAt(int index)
	{
		String data = null;
		if (mTotalFileList!=null && mTotalFileList.size()>0 && index < mTotalFileList.size() &&
                mTotalFileList.get(index).GetFileType() != FileFilterType.DEVICE_FILE_DIR)
		{
            synchronized(mDLNALibBrowseLock)
            {
                FileAttr fa = mTotalFileList.get(index);
                String DLNAUniqueID = fa.GetDLNAUniqueID();
                data = mDLNABrowser.GetItemMediaInfo(DLNAUniqueID, UPNP_DMP_RES_TYPE.UPNP_DMP_RES_URI);
		        //Log.d(TAG,"GetDataAtID["+mTotalFileList.get(index).GetDLNAUniqueID()+"]Data["+data+"]");
            }
		}
		return data;
	}

	@Override
	public String GetTitleAt(int index)
	{
		if (index >= mTotalFileList.size()) return null;
		
        return mTotalFileList.get(index).GetName();
	}

	@Override
	public String GetMimeTypeAt(int i) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public File GetFileAt(int i)
	{
		return null;
	}

	@Override
	public int GetIdAt(int i) {
		return -1;
	}

	@Override
	public int GetFileTypeAt(int index)
	{
		if (index >= mTotalFileList.size()) return 0;
		
		return mTotalFileList.get(index).GetFileType();
	}
   
    public void UpdateData(int Type)
    {
        int i;
        synchronized(mTotalFileList_bg)
        {
            synchronized(mTotalFileList)
            {
                mTotalFileList.clear();
                mTotalFileList.addAll(mTotalFileList_bg);
            }
        }
    }
    public long GetSizeAt(int i)
    {
       String Ssize; 
       synchronized(mDLNALibBrowseLock)
       {
            Ssize = mDLNABrowser.QueryAndGetItemMediaInfo(mTotalFileList.get(i).GetDLNAUniqueID(), UPNP_DMP_RES_TYPE.UPNP_DMP_RES_SIZE);
       }
       if (Ssize == null||Ssize.length()==0){
            return 0;
       }
        if("false".equals(Ssize))
            return -1;
       Long Lsize = Long.decode(Ssize);
       return Lsize.longValue();
    }
   
	@Override
	public int GetSize()
	{
        int size =0;
        synchronized(mTotalFileList)
        {
            size = mTotalFileList.size();
        }
		return size;
	}
    @Override
    public String GetMediaInfo(int i, int type)
    {
        String MediaInfo = null;
        synchronized(mDLNALibBrowseLock)
        {
            if(i>=0 && i<mTotalFileList.size())
                MediaInfo = mDLNABrowser.QueryAndGetItemMediaInfo(mTotalFileList.get(i).GetDLNAUniqueID(), type);
        }
        return MediaInfo;
    }
	
    public ArrayList<String> GetPlayList(int type)
    {
            if (type == FileFilterType.DEVICE_FILE_PHOTO)
                return mPlaylist_Photo;
            else if (type == FileFilterType.DEVICE_FILE_VIDEO)
                return mPlaylist_Video;
            else if (type == FileFilterType.DEVICE_FILE_AUDIO)
                return mPlaylist_Audio;
            else 
                return null;
    }	
    public ArrayList<String> GetNameList(int type)
    {
            if (type == FileFilterType.DEVICE_FILE_PHOTO)
                return mNamelist_Photo;
            else if (type == FileFilterType.DEVICE_FILE_VIDEO)
                return mNamelist_Video;
            else if (type == FileFilterType.DEVICE_FILE_AUDIO)
                return mNamelist_Audio;
            else 
                return null;
    }

    private synchronized void CreateTotalFileLst()
    {
		if (mPath == null || mPath.equals("")) return;
		mIsCreateTotalFileList=true;
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {Thread.sleep(QUERY_TIMEOUT);}
                catch (InterruptedException e) {e.printStackTrace();}

                if(mIsCreateTotalFileList) {
                    log("CreateTotalFileLst timeout, stopWaitingResponse");
                    if(mDLNABrowser!=null)
                        mDLNABrowser.StopPreviousWork();
                }
            }
        }).start();

		int TotalSize =0;
        DirectoryUniqueIDMap.clear();
        log("mDLNABrowser.GetMagicCode()="+mDLNABrowser.GetMagicCode());
        log("Thread.currentThread().getId()="+Thread.currentThread().getId());
        if (mDLNABrowser.GetMagicCode() ==(Thread.currentThread().getId())){
		    TotalSize = mDLNABrowser.GetTotalObjectSize(mPath, mUniqueID);
        }
        if(TotalSize <= 0) return;

        int nextRequestIndex = 0;
        int i=0;
        while(nextRequestIndex < TotalSize)
        {
            synchronized(mDLNALibBrowseLock)
            {
                int ActualPortionSize = 0;
                int DirectorySize = 0;
                int ItemSize = 0;
                if (mDLNABrowser.GetMagicCode() ==(Thread.currentThread().getId())){
                    if (mPath.equals(mDLNABrowser.GetPluginRootPath()))
                        ActualPortionSize = mDLNABrowser.GetDirectorySize();
                    else {
                        ActualPortionSize = mDLNABrowser.BrowseCurDirStep(nextRequestIndex, mPortion);
//                        if(ActualPortionSize==-1) //connect is invalid
                    }

                    DirectorySize = mDLNABrowser.GetDirectorySize();
                    ItemSize = mDLNABrowser.GetItemSize();
                    nextRequestIndex += DirectorySize;
                    nextRequestIndex += ItemSize;
                }
                if ((mFileType&FileFilterType.DEVICE_FILE_DIR) == FileFilterType.DEVICE_FILE_DIR)
                {
                    for (int j=0; j<DirectorySize; j++)
                    {
                       String DirName = mDLNABrowser.GetDirectoryTitle(j);
                       String UniqueID = mDLNABrowser.GetDirectoryUniqueID(j);
                       mDirectoryList.add(new FileAttr(DirName,UniqueID,FileFilterType.DEVICE_FILE_DIR, i)); 
                       DirectoryUniqueIDMap.put(DirName,UniqueID);
                    }   
                    
                    synchronized(mTotalFileList_bg)
                    {
		                mTotalFileList_bg.addAll(mDirectoryList);
                        mDirectoryList.clear();
                    }
                }
                if (mFileType != FileFilterType.DEVICE_FILE_DIR)
                {
                    for (int j=0; j<ItemSize; j++)
                    {
		                String FileName = mDLNABrowser.GetItemTitle(j);
                        String DLNAUniqueID = mDLNABrowser.GetItemUniqueID(j);
                        String MimeTypeDMS = mDLNABrowser.GetItemMimeType(j);
                        String Data = mDLNABrowser.GetItemMediaInfo(DLNAUniqueID, UPNP_DMP_RES_TYPE.UPNP_DMP_RES_URI);

//		                Log.d(TAG,"FileName="+FileName);
//		                Log.d(TAG,"MimeType from DMS="+MimeTypeDMS);
//		                Log.d(TAG,"ID["+DLNAUniqueID+"]Data["+Data+"]");
//		                Log.d(TAG, "GetMediaType(DLNAUniqueID)="+mDLNABrowser.GetMediaType(DLNAUniqueID));
		                switch(mDLNABrowser.GetMediaType(DLNAUniqueID))
		                {
		                	case JNI_DEVICE_FILE_AUDIO:
                                if (mMimeTypes.isAudioFile(Data) || mMimeTypes.isSupportThisMimeType(MimeTypeDMS)|| Data.endsWith(".unk"))
                                {
                                    String Size = mDLNABrowser.GetItemMediaInfo(DLNAUniqueID,UPNP_DMP_RES_TYPE.UPNP_DMP_RES_SIZE);
                                    String Duration = mDLNABrowser.GetItemMediaInfo(DLNAUniqueID,UPNP_DMP_RES_TYPE.UPNP_DMP_RES_DURATION);
                                    //Log.d(TAG, "Audio contentlength="+Size+", duration="+Duration); 
                                    mPlaylist_Audio.add(Data+" contentlength="+Size+" duration="+Duration);
                                    mNamelist_Audio.add(FileName);
		                		    mAudioFileList.add(new FileAttr(FileName, DLNAUniqueID, FileFilterType.DEVICE_FILE_AUDIO, i));
                                }
		                		break;
		                	case JNI_DEVICE_FILE_PHOTO:
                                if (mMimeTypes.isImageFile(Data) || mMimeTypes.isSupportThisMimeType(MimeTypeDMS)|| Data.endsWith(".unk"))
                                { 
                                    mPlaylist_Photo.add(Data);
                                    mNamelist_Photo.add(FileName);
		                		    mPhotoFileList.add(new FileAttr(FileName, DLNAUniqueID, FileFilterType.DEVICE_FILE_PHOTO, i));
                                }
		                		break;
		                	case JNI_DEVICE_FILE_VIDEO:
                                if (mMimeTypes.isVideoFile(Data) || mMimeTypes.isSupportThisMimeType(MimeTypeDMS)|| Data.endsWith(".unk"))
                                { 
                                    mPlaylist_Video.add(Data);
                                    mNamelist_Video.add(FileName);
                                    ArrayList<String> AllSubList = mDLNABrowser.GetSubtitleList(DLNAUniqueID);
                                    if (AllSubList!= null){
                                        mPlaylist_Video.addAll(AllSubList);
                                    }
		                		    mVideoFileList.add(new FileAttr(FileName, DLNAUniqueID, FileFilterType.DEVICE_FILE_VIDEO, i));
                                }
		                		break;
		                	case JNI_DEVICE_FILE_NONE:
		                		mNoneFileList.add(new FileAttr(FileName, DLNAUniqueID, FileFilterType.DEVICE_FILE_NONE, i));
		                		break;
		                	default:
                                log("Unknown mDLNABrowser.GetMediaType(DLNAUniqueID)="+mDLNABrowser.GetMediaType(DLNAUniqueID));
		                		break;
		                }
                    }
                    synchronized(mTotalFileList_bg)
                    {
	                    if ((mFileType&FileFilterType.DEVICE_FILE_AUDIO) == FileFilterType.DEVICE_FILE_AUDIO)
	                    	mTotalFileList_bg.addAll(mAudioFileList);
	                    
	                    if ((mFileType&FileFilterType.DEVICE_FILE_PHOTO) == FileFilterType.DEVICE_FILE_PHOTO)
	                    	mTotalFileList_bg.addAll(mPhotoFileList);
	                    
	                    if ((mFileType&FileFilterType.DEVICE_FILE_VIDEO) == FileFilterType.DEVICE_FILE_VIDEO)
	                    	mTotalFileList_bg.addAll(mVideoFileList);
	                    
	                    if ((mFileType&FileFilterType.DEVICE_FILE_NONE) == FileFilterType.DEVICE_FILE_NONE)
	                    	mTotalFileList_bg.addAll(mNoneFileList);
                        mVideoFileList.clear();
                        mAudioFileList.clear();
                        mPhotoFileList.clear();
                        mNoneFileList.clear();
                    }
                }
                //1. If Step is 1 don't feedback CREATE_PORTION.-->Feedback CREATE_DONE instead. 
                //2. Don't feedback CREATE_PORTION for last step.-->Feedback CREATE_DONE instead. 
                if(nextRequestIndex<TotalSize)
                {
                    Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_PORTION, TotalSize, nextRequestIndex);
                    mCallerHandler.sendMessage(msg);
                    
//                    try{
//                        Log.d(TAG,"WaitResourceRelease");
//                        Thread.sleep(50);
//                    }catch(InterruptedException e) {
//                        e.printStackTrace();
//                    }
                    
                }
            }
            i++;
        }
        mIsCreateTotalFileList=false;
    }
	
	public int getDirnum() {
		return 0;
	}

	public void sortListByType() {
	}
    
	public void StopQuery()
    {
        if(mCreateTotalFileLstTask != null)
        {
            mCreateTotalFileLstTask.cancel(true);
        } 
    }
    private void StopPreviousWork()
    {
        mDLNABrowser.SetMagicCode(Thread.currentThread().getId());
        mDLNABrowser.StopPreviousWork();
    }
    private class FileListTask extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... para)
        {
	        Log.d(TAG,"doInBackground [Start]"+mPath);
            if (!isCancelled())
            {
                Log.d(TAG,"Stop Previous work without check");
                StopPreviousWork();
                CreateTotalFileLst();
            }
	        Log.d(TAG,"doInBackground [End]"+mPath);
            return null;
        }
        @Override
        protected void onPostExecute (Void rlt)
        {
	        Log.d(TAG,"onPostExecute [Start]"+mPath);
            if (!isCancelled())
            {
                Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_DONE,0,0,mPath);
                mCallerHandler.sendMessage(msg);
            }
	        Log.d(TAG,"onPostExecute [End]"+mPath);
        }
        @Override
        protected void onCancelled()
        {
            super.onCancelled();
	        Log.d(TAG,"onCancelled [Start]"+mPath);
            Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_ABORT,0,0,mPath);
            mCallerHandler.sendMessage(msg);
	        Log.d(TAG,"onCancelled [End]"+mPath);
        }
        @Override
        protected void finalize ()throws Throwable
        {
            Log.d(TAG,"FileListTask finalize for Path:"+mPath);
            super.finalize();
        }
    }
}
