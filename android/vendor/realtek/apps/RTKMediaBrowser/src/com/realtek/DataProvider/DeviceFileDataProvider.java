package com.realtek.DataProvider;

import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import com.realtek.Utils.BDParser;
import com.realtek.Utils.FileInfo;
import com.realtek.Utils.FileUtils;
import com.realtek.Utils.MimeTypes;
import com.rtk.mediabrowser.Util;

import java.io.File;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class DeviceFileDataProvider extends AbstractDataProvider
{	
	private static final String TAG = "DeviceFileDataProvider";
    private static final boolean DEBUG = true;
    private String mPath="/tmp/usbmounts/";
	private int   mFileType=FileFilterType.DEVICE_FILE_PHOTO|FileFilterType.DEVICE_FILE_DIR;
	private int   mLimitCnt=0;
	private int   mSortMode=0;
	private MimeTypes mMimeTypes=null;
	private int dirnum =0;
    private Handler mCallerHandler = null;
    private boolean mStopQuery = false;
    private boolean mQueryDone = false;
    private boolean mBDQueryDone = false;
    private FileListTask mCreateFileListTask = null;
    private BDFileListTask mBDFileListTask = null;
	private ArrayList<FileInfo> mFileList =  new ArrayList<FileInfo>(); 
	private ArrayList<FileInfo> mFileList_bg =  new ArrayList<FileInfo>(); 
    private ArrayList<String> mPlaylist_Audio = new ArrayList<String>();
    private ArrayList<String> mPlaylist_Video = new ArrayList<String>();
    private ArrayList<String> mPlaylist_Photo = new ArrayList<String>();
    private ArrayList<String> mNamelist_Audio = new ArrayList<String>();
    private ArrayList<String> mNamelist_Video = new ArrayList<String>();
    private ArrayList<String> mNamelist_Photo = new ArrayList<String>();
    private ArrayList<String> mPlaylist_BD = new ArrayList<>();
    private ArrayList<String> mNamelist_BD = new ArrayList<>();

    public DeviceFileDataProvider(String path, int fileType, int limit, int sortMode, MimeTypes mimeTypes, Handler handler)
	{
		super.SetName("DeviceFileDataProvider");

	    mPath     =path;
	    mFileType =fileType; 
	    mLimitCnt =limit;
	    mSortMode =sortMode;
	    mMimeTypes = mimeTypes;
	    dirnum = 0;
        mCallerHandler = handler;
        mCreateFileListTask = new FileListTask();
        mCreateFileListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
	    //CreateFileListArray();
        mBDFileListTask = new BDFileListTask();
        mBDFileListTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
		
	}

    public String GetDataAt(int i) {
        if (i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();
            return file.getPath();
        }
        return null;
    }

    public File GetFileAt(int i) {
        if (i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();
            return file;
        }
        return null;
    }

    public String GetTitleAt(int i) {
        if (i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();
            return file.getName();
        }
        return null;
    }

    public String GetMimeTypeAt(int i) {
        if (i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();
            return mMimeTypes.getMimeType(file.getName());
        }
        return null;
    }

    public int GetIdAt(int i) {
        return -1;
    }

    public int GetSize() {
        int size = 0;
        synchronized (mFileList) {
            size = this.mFileList.size();
        }

        if (size <= 0) size = 0;

        return size;
    }

    public int GetFileTypeAt(int i) {
        int rlt = FileFilterType.DEVICE_FILE_NONE;
        if (i >= 0 && i < this.GetSize()) {
            //Log.d(TAG,"GetFileTypeAt:"+mFileTypeList[i]);
            rlt = this.mFileList.get(i).getmFileType();
        }
        return rlt;
    }

    public long GetSizeAt(int i) {
        long rlt = 0;
        if (i >= 0 && i < this.GetSize()) {
            rlt = this.mFileList.get(i).getSize();
        }
        return rlt;
    }

    public void UpdateData(int Type) {
        synchronized (mFileList_bg) {
            synchronized (mFileList) {
                mFileList.clear();
                //Log.e(TAG, "New File List[Start]");
                mFileList = new ArrayList<FileInfo>(mFileList_bg);
                //Log.e(TAG, "New File List[End]");
            }
        }
    }

    public ArrayList<String> GetPlayList(int type) {
        if (mQueryDone) {
            if (type == FileFilterType.DEVICE_FILE_PHOTO)
                return mPlaylist_Photo;
            else if (type == FileFilterType.DEVICE_FILE_VIDEO)
                return mPlaylist_Video;
            else if (type == FileFilterType.DEVICE_FILE_AUDIO)
                return mPlaylist_Audio;
            else if (type == FileFilterType.DEVICE_FILE_BD_FOLDER) {
                if (mBDQueryDone) {
                    return mPlaylist_BD;
                }
                Log.d(TAG, "GetBDPlayList Fail, BD Query is not done.");
                return null;
            } else
                return null;
        }
        Log.d(TAG, "GetPlayList Fail, Query is not done.");
        return null;
    }

    public ArrayList<String> GetNameList(int type) {
        if (mQueryDone) {
            if (type == FileFilterType.DEVICE_FILE_PHOTO)
                return mNamelist_Photo;
            else if (type == FileFilterType.DEVICE_FILE_VIDEO)
                return mNamelist_Video;
            else if (type == FileFilterType.DEVICE_FILE_AUDIO)
                return mNamelist_Audio;
            else if (type == FileFilterType.DEVICE_FILE_BD_FOLDER) {
                if (mBDQueryDone) {
                    return mNamelist_BD;
                }
                return null;
            } else
                return null;
        }
        return null;
    }

    private String getBdDvdPlayEntry(String path)
    {
        //skip dlna files
        if(path.startsWith("http")) return null;
        BDParser BD = new BDParser(path);
        if (BD.getType() != BDParser.NOT_BD_DVD)
            return BD.getPlayEntry();
        return null;
    }

    private void CreateFileListArray() {
        //Log.e(TAG, "CreateFileListArray");
        mQueryDone = false;

        File file = new File(mPath);
        if (!file.canRead()) {
            Log.e(TAG, "Can not read " + file);
            return;
        }
        File[] files = file.listFiles();
        if (files == null) {
            return;
        }

        Log.e(TAG, "CreateFileListArray:" + files.length);

        if (files.length <= 0)
            return;

        int j = 0;
        int step = files.length > 2000 ? (files.length) >> 2 : (files.length) + 1;
        for (int i = 0; i < files.length; i++) {
            if (mStopQuery == true) {
                break;
            }
            String filename = files[i].getName();

            if ((files[i].isDirectory()) &&
                    (!filename.equals(".")) &&
                    (!filename.equals("..")) &&
                    (!filename.startsWith(".")) &&
                    ((mFileType & FileFilterType.DEVICE_FILE_DIR) == FileFilterType.DEVICE_FILE_DIR)
                    ) {
//                String entry = getBdDvdPlayEntry(files[i].toString());
                FileInfo fileInfo;
//              if(entry!=null){
//                    fileInfo = new FileInfo(
//                            FileFilterType.DEVICE_FILE_DIR,
//                            files[i],
//                            filename,
//                            Util.getUri(Util.getAppCtx(), "file://" + entry).toString()
//                    );
//                } else {
                    fileInfo = new FileInfo(
                            FileFilterType.DEVICE_FILE_DIR,
                            files[i],
                            filename
                    );
//                }

                fileInfo.setSize(files[i].length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
                dirnum++;
                j++;
            } else if (((mFileType & FileFilterType.DEVICE_FILE_PHOTO) == FileFilterType.DEVICE_FILE_PHOTO) &&
                    (this.mMimeTypes.isImageFile(filename))
                    ) {
                FileInfo fileInfo = new FileInfo(
                        FileFilterType.DEVICE_FILE_PHOTO,
                        files[i],
                        filename,
                        Util.getUri(Util.getAppCtx(), "file://" + files[i].getPath()).toString()
                );
                fileInfo.setSize(files[i].length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
                j++;
            } else if (((mFileType & FileFilterType.DEVICE_FILE_VIDEO) == FileFilterType.DEVICE_FILE_VIDEO) &&
                    (this.mMimeTypes.isVideoFile(filename))
                    ) {
                FileInfo fileInfo = new FileInfo(
                        FileFilterType.DEVICE_FILE_VIDEO,
                        files[i],
                        filename,
                        Util.getUri(Util.getAppCtx(), "file://" + files[i].getPath()).toString()
                );
                fileInfo.setSize(files[i].length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
                j++;
            } else if (((mFileType & FileFilterType.DEVICE_FILE_AUDIO) == FileFilterType.DEVICE_FILE_AUDIO) &&
                    (this.mMimeTypes.isAudioFile(filename))
                    ) {
                FileInfo fileInfo = new FileInfo(
                        FileFilterType.DEVICE_FILE_AUDIO,
                        files[i],
                        filename,
                        Util.getUri(Util.getAppCtx(), "file://" + files[i].getPath()).toString()
                );
                fileInfo.setSize(files[i].length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
                j++;
            } else if (((mFileType & FileFilterType.DEVICE_FILE_PLAYLIST) == FileFilterType.DEVICE_FILE_PLAYLIST) &&
                    (this.mMimeTypes.isPlaylistFile(filename))
                    ) {
                FileInfo fileInfo = new FileInfo(
                        FileFilterType.DEVICE_FILE_PLAYLIST,
                        files[i],
                        filename
                );
                fileInfo.setSize(files[i].length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
                j++;
            } else if (((mFileType & FileFilterType.DEVICE_FILE_APK) == FileFilterType.DEVICE_FILE_APK) &&
                    (this.mMimeTypes.isAPKFile(filename))
                    ) {
                FileInfo fileInfo = new FileInfo(
                        FileFilterType.DEVICE_FILE_APK,
                        files[i],
                        filename
                );
                fileInfo.setSize(files[i].length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
                j++;
            } else {
                String extension = FileUtils.getExtension(filename).toLowerCase();
                Log.d(TAG, "get unknown file:" + files[i].getName());
                if (extension.endsWith(".mlp")) {
//                     FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_AUDIO,files[i]);
//                     fileInfo.setSize(files[i].length());
//                     mPlaylist_Audio.add("file://"+files[i].getPath());
//                     mNamelist_Audio.add(files[i].getName());
//                     synchronized(mFileList_bg)
//                     {
//                         mFileList_bg.add(fileInfo);
//                     }
//                     j++;
                } else {
//                     Log.d(TAG, "get unknown file:" + files[i].getName());
//                     FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_NONE, files[i]);
//                     fileInfo.setSize(files[i].length());
//                     synchronized (mFileList_bg) {
//                         mFileList_bg.add(fileInfo);
//                     }
//                     j++;
                }
            }
//            log("mFileList_bg:" + Arrays.toString(mFileList_bg.toArray()));
            if (i > 0 && i % step == 0) {
                sortListByType();
                Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_PORTION, files.length, i);
                mCallerHandler.sendMessage(msg);
            }
        }
        sortListByType();
        getPlaylist(mFileList_bg);
        mQueryDone = true;
    }

    private void CreateBDFileListArray() {
        mBDQueryDone = false;

        File file = new File(mPath);
        if (!file.canRead()) {
            Log.e(TAG, "Can not read " + file);
            return;
        }
        File[] files = file.listFiles();
        if (files == null) {
            return;
        }

        Log.e(TAG, "CreateBDFileListArray:" + files.length);

        if (files.length <= 0)
            return;

        for (int i = 0; i < files.length; i++) {
            if (mStopQuery == true) {
                Log.d(TAG,"BD Stop Query");
                break;
            }
            String filename = files[i].getName();

            if ((files[i].isDirectory()) &&
                    (!filename.equals(".")) &&
                    (!filename.equals("..")) &&
                    (!filename.startsWith(".")) &&
                    ((mFileType & FileFilterType.DEVICE_FILE_DIR) == FileFilterType.DEVICE_FILE_DIR)
                    ) {
                String entry = getBdDvdPlayEntry(files[i].toString());
                FileInfo fileInfo;
                if(entry!=null){
                    fileInfo = new FileInfo(
                            FileFilterType.DEVICE_FILE_DIR,
                            files[i],
                            filename,
                            Util.getUri(Util.getAppCtx(), "file://" + entry).toString()
                    );
                    mPlaylist_BD.add(fileInfo.path);
                    mNamelist_BD.add(files[i].getName());
                }
            }
        }
        mBDQueryDone = true;
    }

    private void getPlaylist(ArrayList<FileInfo> allFiles) {
        for(FileInfo fi : allFiles){
            if(fi.mFileType==FileFilterType.DEVICE_FILE_AUDIO) {
                mPlaylist_Audio.add(fi.path);
                mNamelist_Audio.add(fi.fileName);
            } else if (fi.mFileType==FileFilterType.DEVICE_FILE_VIDEO) {
                mPlaylist_Video.add(fi.path);
                mNamelist_Video.add(fi.fileName);
            } else if (fi.mFileType==FileFilterType.DEVICE_FILE_PHOTO) {
                mPlaylist_Photo.add(fi.path);
                mNamelist_Photo.add(fi.fileName);
//            } else if (fi.mFileType==FileFilterType.DEVICE_FILE_DIR
//                    && fi.path!=null){
//                File f = fi.getmFile();
//                mPlaylist_BD.add(fi.path);
//                mNamelist_BD.add(f.getName());
            }
        }
    }

    private static void log(String s) {
        if(DEBUG) Log.d(TAG, s);
    }

    public int getDirnum() {
		return dirnum;
	}

	public void sortListByType() {
		Collections.sort(mFileList_bg, new SortByType());
	}

    class SortByType implements Comparator<FileInfo> {
        @Override
        public int compare(FileInfo s1, FileInfo s2) {
            if (s1.getmFileType() == FileFilterType.DEVICE_FILE_DIR
                    && s2.getmFileType() != FileFilterType.DEVICE_FILE_DIR)
                return -1;
            else if (s1.getmFileType() != FileFilterType.DEVICE_FILE_DIR
                    && s2.getmFileType() == FileFilterType.DEVICE_FILE_DIR)
                return 1;
            String f1 = s1.fileName;
            String f2 = s2.fileName;
            if (f1 != null && f2 != null)
                return f1.compareToIgnoreCase(f2);
            return 0;
        }
    }

	public void StopQuery()
    {
        Log.d(TAG,"StopQuery");
        if (mBDFileListTask != null) {
            mBDFileListTask.cancel(true);
        }

        if (mCreateFileListTask != null)
        {
            mCreateFileListTask.cancel(true);
        }
        mStopQuery = true;
    }

    private class FileListTask extends AsyncTask<Void, Void, Void> {

        @Override
        protected Void doInBackground(Void... para)
        {
	        Log.d(TAG,"doInBackground [Start]"+mPath);
            if (!isCancelled())
            {
                Log.d(TAG,"Stop Previous work without check");
                CreateFileListArray();
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

    private class BDFileListTask extends AsyncTask<Void, Void, Void> {

        @Override
        protected Void doInBackground(Void... para)
        {
            Log.d(TAG,"BDFileListTask doInBackground [Start]"+mPath);
            if (!isCancelled())
            {
                Log.d(TAG,"Stop Previous work without check");
                CreateBDFileListArray();
            }
            Log.d(TAG,"BDFileListTask doInBackground [End]"+mPath);
            return null;
        }

        @Override
        protected void onCancelled()
        {
            super.onCancelled();
            Log.d(TAG,"onCancelled BDFileListTask "+mPath);
        }
        @Override
        protected void finalize ()throws Throwable
        {
            Log.d(TAG,"BDFileListTask finalize for Path:"+mPath);
            super.finalize();
        }
    }
}
