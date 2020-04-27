package com.realtek.DataProvider;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import com.realtek.Utils.MimeTypes;
import com.realtek.rtksmb.SmbBrowser;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

public class SMBFileDataProvider extends AbstractDataProvider {

    private static final String TAG = "SMBFileDataProvider";
    private static final boolean DEBUG = true;
    private FileListTask mCreateFileListTask;
    private String mPath = "";
	private int mFileType;
	private int mLimitCnt = 0;
	private int mSortMode = 0;
    private int mQueryType = -1;
	private MimeTypes mMimeTypes = null;
    private Handler mCallerHandler = null;
	
	private SmbBrowser mSmbBrowser;

	
	public SMBFileDataProvider (String path, int fileType,int limit,int sortMode,MimeTypes mimeTypes, Context context, Handler handler)
	{
		super.SetName("SMBFileDataProvider");
        log("path="+path);
	    mPath     = path;
	    mFileType = fileType; 
	    mLimitCnt = limit;
	    mSortMode = sortMode;
	    mMimeTypes = mimeTypes;

	    mSmbBrowser = SmbBrowser.GetInstance(fileType, context, mimeTypes, handler);

//	    CreateTotalFileLst();
        if(handler!=null) {
            mCallerHandler = handler;
            mCreateFileListTask = new FileListTask();
            mCreateFileListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        } else {
            CreateTotalFileLst();
        }

	}

    public SMBFileDataProvider (String path, int fileType,int limit,int sortMode,MimeTypes mimeTypes, Context context, Handler handler, String user, String password)
	{
		super.SetName("SMBFileDataProvider");
        log("path="+path);
	    mPath     = path;
	    mFileType = fileType; 
	    mLimitCnt = limit;
	    mSortMode = sortMode;
	    mMimeTypes = mimeTypes;
	    
	    mSmbBrowser = SmbBrowser.GetInstance(fileType, context, mimeTypes, handler);
        mSmbBrowser.SetAuthentication(user, password);
	    
//	    CreateTotalFileLst();
        if(handler!=null) {
            mCallerHandler = handler;
            mCreateFileListTask = new FileListTask();
            mCreateFileListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
        } else {
            CreateTotalFileLst();
        }
	}

    private class FileListTask extends AsyncTask<Void, Void, Void> {
        @Override
        protected Void doInBackground(Void... para)
        {
            log("doInBackground [Start]"+mPath);
            if (!isCancelled())
            {
                log("Stop Previous work without check");
                CreateTotalFileLst();
            }
            log("doInBackground [End]"+mPath);
            return null;
        }
        @Override
        protected void onPostExecute (Void rlt)
        {
            Log.d(TAG,"onPostExecute [Start]"+mPath);
            if (!isCancelled())
            {
                int what = FILE_LIST_CREATE_ABORT;
                int arg1 = 0;
                Object obj = null;
                if(mQueryType==SmbBrowser.SMB_QUERY_SHARE_FAILED) {
                    what = FILE_LIST_CREATE_ABORT;
                    obj = mSmbBrowser.lastException;
                }else
                    what = FILE_LIST_CREATE_DONE;
                Message msg = Message.obtain(mCallerHandler, what, arg1, 0, obj);
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
	
	@Override
	public String GetDataAt(int index)
	{
		if (mQueryType == SmbBrowser.SMB_QUERY_FOLDER)
        {
            return mSmbBrowser.GetMntDataAt(index);
        }
        else
        {
            if (mPath.endsWith("/"))
            {
                return mPath + mSmbBrowser.GetSmbDirList(mQueryType).get(index);
            }
            else
            {
                return mPath + "/" + mSmbBrowser.GetSmbDirList(mQueryType).get(index);
            }
        }
	}

	@Override
	public String GetTitleAt(int index)
	{
		if (mQueryType == SmbBrowser.SMB_QUERY_FOLDER)
        {
            return mSmbBrowser.GetMntTitleAt(index);
        }
        else
        {
            List<String> list = mSmbBrowser.GetSmbDirList(mQueryType);
            if(list.size()==0) return "";
            return list.get(index);
        }
	}

	@Override
	public String GetMimeTypeAt(int i) {
		if (mQueryType == SmbBrowser.SMB_QUERY_FOLDER)
        {
            mSmbBrowser.GetMntMimeTypeAt(i);
        }
        return null;
	}
	
	@Override
	public File GetFileAt(int i)
	{
	    if (mQueryType == SmbBrowser.SMB_QUERY_FOLDER)
        {
            return mSmbBrowser.GetMntFileAt(i);
        }
		return null;
	}

	@Override
	public int GetIdAt(int i) {
		return -1;
	}

	@Override
	public int GetFileTypeAt(int index)
	{
		if (mQueryType == SmbBrowser.SMB_QUERY_DOMAIN)
        {
            return FileFilterType.DEVICE_FILE_DOMAIN;
        }
        else if (mQueryType == SmbBrowser.SMB_QUERY_SERVER)
        {
            return FileFilterType.DEVICE_FILE_SMB_DEVICE;
        }
        else if (mQueryType == SmbBrowser.SMB_QUERY_SHARE)
        {
            return FileFilterType.DEVICE_FILE_DIR;
        }
        else if (mQueryType == SmbBrowser.SMB_QUERY_FOLDER)
        {
            return mSmbBrowser.GetMntFileTypeAt(index);
        }

        return FileFilterType.DEVICE_FILE_NONE;

	}

	@Override
	public int GetSize()
	{
		if (mQueryType == SmbBrowser.SMB_QUERY_FOLDER)
        {
            return mSmbBrowser.GetMntSize();
        }
        else if (mQueryType == SmbBrowser.SMB_QUERY_SHARE_FAILED)
        {
            return -1;// Login failed maybe.
        }
        else
        {
            return mSmbBrowser.GetSmbDirList(mQueryType).size();
        }
	}

    public ArrayList<String> GetPlayList(int type)
    {
        return mSmbBrowser.GetPlayList(type);
    }
    public ArrayList<String> GetNameList(int type)
    {
        return mSmbBrowser.GetNameList(type);
    }
	
	public void CreateTotalFileLst()
	{
		if (mPath == null || mPath.equals("")) return;

        mQueryType = mSmbBrowser.SmbQuery(mPath);
		
	}

	public int getDirnum() {
		return 0;
	}

	public void sortListByType() {
	}
    public void UpdateData(int Type)
    {
        mSmbBrowser.UpdateData(Type);
    }

    private static void log(String s){
        if(DEBUG)
            Log.d(TAG, s);
    }
}

