package com.realtek.DataProvider;

import android.net.Uri;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import com.realtek.Utils.Const;
import com.realtek.Utils.FileInfo;
import com.realtek.Utils.MimeTypes;
import com.realtek.rtksmb.SmbBrowser;
import com.realtek.rtksmb.SmbUtils;
import jcifs.CIFSContext;
import jcifs.CIFSException;
import jcifs.smb.*;

import java.io.File;
import java.net.MalformedURLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class SMBFileDataProvider2 extends AbstractDataProvider
{
	private static final String TAG = "SMBFileDataProvider2";
    private static final boolean DEBUG = false;
    private final String mUsername;
    private final String mPassword;
    private final String mMntPath;
    private final String mDomain;
    private String mPath="";
	private int   mFileType=FileFilterType.DEVICE_FILE_PHOTO|FileFilterType.DEVICE_FILE_DIR;
	private int   mLimitCnt=0;
	private int   mSortMode=0;
	private MimeTypes mMimeTypes=null;
	private int dirnum =0;
    private Handler mCallerHandler = null;
    private boolean mStopQuery = false;
    private boolean mQueryDone = false;
    private FileListTask mCreateFileListTask = null;
	private ArrayList<FileInfo> mFileList =  new ArrayList<FileInfo>();
	private ArrayList<FileInfo> mFileList_bg =  new ArrayList<FileInfo>();
    private ArrayList<String> mPlaylist_Audio = new ArrayList<String>();
    private ArrayList<String> mPlaylist_Video = new ArrayList<String>();
    private ArrayList<String> mPlaylist_Photo = new ArrayList<String>();
    private ArrayList<String> mNamelist_Audio = new ArrayList<String>();
    private ArrayList<String> mNamelist_Video = new ArrayList<String>();
    private ArrayList<String> mNamelist_Photo = new ArrayList<String>();
	public SMBFileDataProvider2(String path, String mntPath, String domain, String username, String password, int fileType, int limit, int sortMode, MimeTypes mimeTypes, Handler handler)
	{
		super.SetName("SMBFileDataProvider2");
		
	    mPath     =path;
        mMntPath = mntPath;
        mDomain = domain;
        mUsername = username;
        mPassword = password;
	    mFileType =fileType; 
	    mLimitCnt =limit;
	    mSortMode =sortMode;
	    mMimeTypes = mimeTypes;
	    dirnum = 0;
        mCallerHandler = handler;
        mCreateFileListTask = new FileListTask();
        mCreateFileListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
	    //CreateFileListArray();
		
	}

    private static int nthOccurrence(String str, char c, int n) {
        int pos = str.indexOf(c, 0);
        while (n-- > 0 && pos != -1)
            pos = str.indexOf(c, pos+1);
        return pos;
    }

    private String getMntDataPath(String smbPath){
        if(!smbPath.startsWith("smb://"))
            return null;
        int shareRootIndex = nthOccurrence(smbPath, '/', 3);
        String fileRelPath = smbPath.substring(shareRootIndex);
        String dataPath = mMntPath+fileRelPath;
        return dataPath;
    }
	
	public String GetDataAt(int i)
	{
        log("GetDataAt i="+i);
		if( i>=0 && i<this.GetSize())
		{
			SmbFile file =  this.mFileList.get(i).getSmbFile();
			log("mMntPath="+mMntPath);
            log("file.getPath()="+file.getPath());

            if(file.getPath().endsWith("/"))
                return file.getPath();
            else {
                //if current file is a data, then use getMntDataPath to translate to mount path.
                log("getMntDataPath(file.getPath())="+getMntDataPath(file.getPath()));
                return getMntDataPath(file.getPath());
            }
		}
		return null;
	}

	public File GetFileAt(int i)
	{
		if(i>=0 && i<this.GetSize())
		{
            File file = this.mFileList.get(i).getmFile();
			
			return file;
		}	
		return null;
	}

    public String GetTitleAt(int i)
    {
		if(i>=0 && i<this.GetSize())
		{
			SmbFile file =  this.mFileList.get(i).getSmbFile();
            String title = file.getName();
			if(title.endsWith("/"))
                return title.substring(0, title.lastIndexOf('/'));
            else
			    return file.getName();
		}
		return null;
	}
	
	public  String GetMimeTypeAt (int i)
	{
		if(i>=0 && i<this.GetSize())
		{
			SmbFile file =  this.mFileList.get(i).getSmbFile();
			
			return mMimeTypes.getMimeType(file.getName());
			
		}	
		return null;
	}
	
	public int GetIdAt(int i)
	{
		return -1;
	}
	
	public int GetSize ()
	{
		int size =0;
        synchronized(mFileList)
        {
		    size =this.mFileList.size();
        }
		
		if(size<=0) size = 0;
		
		return size;
	}
	
	public int GetFileTypeAt(int i)
	{
        int rlt = FileFilterType.DEVICE_FILE_NONE;
		if(i>=0 && i<this.GetSize())
		{
			//Log.d(TAG,"GetFileTypeAt:"+mFileTypeList[i]);
			rlt = this.mFileList.get(i).getmFileType();
		}
		return rlt;
	}
    public long GetSizeAt(int i)
    {
        long rlt = 0;
		if(i>=0 && i<this.GetSize())
		{
			rlt = this.mFileList.get(i).getSize();
        }
        return rlt;
    }
    public void UpdateData(int Type)
    {
        int i;
        synchronized(mFileList_bg)
        {
            synchronized(mFileList)
            {
                mFileList.clear();
                //Log.e(TAG, "New File List[Start]");
                mFileList = new ArrayList<FileInfo>(mFileList_bg);
                //Log.e(TAG, "New File List[End]");
            }
        } 
    }

    public ArrayList<String> GetPlayList(int type)
    {
        if (mQueryDone)
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
        Log.d(TAG,"GetPlayList Fail, Query is not done.");
        return null;
    }

    public ArrayList<String> GetNameList(int type)
    {
        if (mQueryDone)
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
        return null;
    }

    private String getSmbPath(String path){
        String part1="";
        String part3="";
        if(!path.startsWith("smb:"))
            part1 = "smb:";
        if(!path.endsWith("/"))
            part3 = "/";
        String smbPath = String.format("%s%s%s", part1, path, part3);
        return smbPath;
    }

    private static void log(String s){
        if(DEBUG)
            Log.d(TAG, s);
    }

    private void CreateFileListArray()
    {
        Log.d(TAG, "CreateFileListArray");
        mQueryDone = false;

        String smbPath = getSmbPath(mPath);
        final String MNT_DATA_PATH = getMntDataPath(smbPath);
        if(DEBUG) {
            log(String.format("Samba login info mDomain=%s, mUsername=%s, mPath=%s", mDomain, mUsername, mPath));
            log("smbPath = "+smbPath);
        }
        CIFSContext cifsContext = null;
        if(TextUtils.isEmpty(mUsername) || mUsername.equals(Const.GUEST)) {
            cifsContext = SmbUtils.getContext().withDefaultCredentials();
        } else {
            NtlmPasswordAuthenticator auth = new NtlmPasswordAuthenticator(mDomain, mUsername, mPassword);
            cifsContext = SmbUtils.getContext().withCredentials(auth);
        }

        try {
            SmbFile file = new SmbFile(smbPath, cifsContext);
            if (!file.canRead()) {
                Log.e(TAG, "Can not read " + file);
                return;
            }
            SmbFile[] files = file.listFiles();
            if (files == null) {
                log("file.listFiles() return null");
                return;
            }

            log("CreateFileListArray:" + files.length);

            if (files.length <= 0)
                return;

            int j = 0;
            int step = files.length > 2000 ? (files.length) >> 2 : (files.length) + 1;
            for (int i = 0; i < files.length; i++) {
                if (mStopQuery == true) {
                    break;
                }
                String filename = files[i].getName();
                if (DEBUG) {
                    String str = String.format("mFileType=%s, files[i].getName()=%s, files[i].getPath()=%s", mFileType, files[i].getName(), files[i].getPath());
                    log(str);
                }
                if ((files[i].isDirectory() == true) &&
                        (filename.equals(".") == false) &&
                        (filename.equals("..") == false) &&
                        ((mFileType & FileFilterType.DEVICE_FILE_DIR) == FileFilterType.DEVICE_FILE_DIR)
                        ) {
                    FileInfo fileInfo;
                    fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_DIR, files[i]);
                    fileInfo.setSize(files[i].length());
                    synchronized (mFileList_bg) {
                        mFileList_bg.add(fileInfo);
                    }
                    dirnum++;
                    j++;
                } else if (((mFileType & FileFilterType.DEVICE_FILE_PHOTO) == FileFilterType.DEVICE_FILE_PHOTO) &&
                        (this.mMimeTypes.isImageFile(filename) == true)
                        ) {
                    FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_PHOTO, files[i]);
                    fileInfo.setSize(files[i].length());
                    mPlaylist_Photo.add("file://" + MNT_DATA_PATH + Uri.encode(files[i].getName()));
                    mNamelist_Photo.add(files[i].getName());
                    synchronized (mFileList_bg) {
                        mFileList_bg.add(fileInfo);
                    }
                    j++;
                } else if (((mFileType & FileFilterType.DEVICE_FILE_VIDEO) == FileFilterType.DEVICE_FILE_VIDEO) &&
                        (this.mMimeTypes.isVideoFile(filename) == true)
                        ) {
                    FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_VIDEO, files[i]);
                    fileInfo.setSize(files[i].length());
                    mPlaylist_Video.add("file://" + MNT_DATA_PATH + Uri.encode(files[i].getName()));
                    mNamelist_Video.add(files[i].getName());
                    synchronized (mFileList_bg) {
                        mFileList_bg.add(fileInfo);
                    }
                    j++;
                } else if (((mFileType & FileFilterType.DEVICE_FILE_AUDIO) == FileFilterType.DEVICE_FILE_AUDIO) &&
                        (this.mMimeTypes.isAudioFile(filename) == true)
                        ) {
                    FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_AUDIO, files[i]);
                    fileInfo.setSize(files[i].length());
                    mPlaylist_Audio.add("file://" + MNT_DATA_PATH + Uri.encode(files[i].getName()));
                    mNamelist_Audio.add(files[i].getName());
                    synchronized (mFileList_bg) {
                        mFileList_bg.add(fileInfo);
                    }
                    j++;
                } else if (((mFileType & FileFilterType.DEVICE_FILE_PLAYLIST) == FileFilterType.DEVICE_FILE_PLAYLIST) &&
                        (this.mMimeTypes.isPlaylistFile(filename) == true)
                        ) {
                    FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_PLAYLIST, files[i]);
                    fileInfo.setSize(files[i].length());
                    synchronized (mFileList_bg) {
                        mFileList_bg.add(fileInfo);
                    }
                    j++;
                } else if (((mFileType & FileFilterType.DEVICE_FILE_APK) == FileFilterType.DEVICE_FILE_APK) &&
                        (this.mMimeTypes.isAPKFile(filename) == true)
                        ) {
                    FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_APK, files[i]);
                    fileInfo.setSize(files[i].length());
                    synchronized (mFileList_bg) {
                        mFileList_bg.add(fileInfo);
                    }
                    j++;
                }

                if (i > 0 && i % step == 0) {
                    sortListByType();
                    Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_PORTION, files.length, i);
                    mCallerHandler.sendMessage(msg);
                }
            }
            sortListByType();
            mQueryDone = true;
        }catch(SmbAuthException e){
            e.printStackTrace();
            Log.e(TAG, e.getMessage());
            mQueryDone = false;
            //If login failed, show login dialog
            Message msg = Message.obtain(mCallerHandler, SmbBrowser.SmbAuthFailed,0,0,null);
            mCallerHandler.sendMessage(msg);
        }catch(SmbException e){
            e.printStackTrace();
            Log.e(TAG, e.getMessage());
            mQueryDone = false;
        } catch (MalformedURLException e) {
            e.printStackTrace();
            Log.e(TAG, e.getMessage());
            mQueryDone = false;
        }
    }

	public int getDirnum() {
		return dirnum;
	}

	@SuppressWarnings("unchecked")
	public void sortListByType() {
		Collections.sort(mFileList_bg, new SortByType());

	}

	@SuppressWarnings("rawtypes")
	class SortByType implements Comparator {
		public int compare(Object o1, Object o2) {
			FileInfo s1 = (FileInfo) o1;
			FileInfo s2 = (FileInfo) o2;
			if (s1.getmFileType() == FileFilterType.DEVICE_FILE_DIR
					&& s2.getmFileType() != FileFilterType.DEVICE_FILE_DIR)
				return -1;
			else if (s1.getmFileType() != FileFilterType.DEVICE_FILE_DIR
					&& s2.getmFileType() == FileFilterType.DEVICE_FILE_DIR)
				return 1;
            return 0;
		}
	}

	public void StopQuery()
    {
        if (mCreateFileListTask != null)
        {
            mCreateFileListTask.cancel(true);
        }
        mStopQuery = true;
    }

    private class FileListTask extends AsyncTask<Void, Void, Void> {

        public FileListTask()
        {
        }
        @Override
        protected void onPreExecute()
        {
            super.onPreExecute();
//            Message msg = Message.obtain(mCallerHandler, SHOW_PROGRESS,0,0,null);
//            mCallerHandler.sendMessage(msg);
        }
        @Override
        protected Void doInBackground(Void... para)
        {
	        log("doInBackground [Start]"+mPath);
            if (!isCancelled())
            {
                log("Stop Previous work without check");
                CreateFileListArray();
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
