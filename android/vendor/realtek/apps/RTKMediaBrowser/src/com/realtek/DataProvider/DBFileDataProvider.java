package com.realtek.DataProvider;

import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.provider.MediaStore;
import android.util.Log;
import com.realtek.Utils.FileInfo;
import com.realtek.Utils.MimeTypes;
import com.rtk.mediabrowser.Util;

import java.io.File;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;

public class DBFileDataProvider extends AbstractDataProvider
{	
	private static final String TAG = "DBFileDataProvider";

	private int   mFileType = FileFilterType.DEVICE_FILE_NONE;
	private int   mLimitCnt=0;
	private int   mSortMode=0;
	private MimeTypes mMimeTypes = null;
	private int dirnum =0;
    private String mPhotoTakenDate = null;
    private Context mContext;
    //static private LinkedHashMap<String, ArrayList> mPhotoDateHashMap = new LinkedHashMap<String, ArrayList>();
    static private HashMap<String, String> mPhotoDateHashMap = new HashMap<String, String>();
	
    private Handler mCallerHandler = null;
    private boolean mStopQuery = false;
    private boolean mQueryDone = false;
    private FileListTask mCreateFileListTask = null;	
	private ArrayList<FileInfo> mFileList =  new ArrayList<FileInfo>(); 
    private ArrayList<Integer> mIDList =  new ArrayList<Integer>(); 
    private ArrayList<String> mPhotoDateList =  new ArrayList<String>(); 

	private ArrayList<FileInfo> mFileList_bg =  new ArrayList<FileInfo>(); 
    private ArrayList<String> mPhotoDateList_bg =  new ArrayList<String>(); 
    private ArrayList<String> mPlaylist = new ArrayList<String>();	
    private ArrayList<String> mNamelist = new ArrayList<String>();	
    public class PhotoDateInfo
    {
		private int id;
		private String path;
    }

    public class SortMode
    {
        public final static int SORT_BY_NAME_ASC =  0x00000001;
        public final static int SORT_BY_NAME_DESC =  0x00000002;
        public final static int SORT_BY_DATE_MODIFIED_ASC =  0x00000004;
        public final static int SORT_BY_DATE_MODIFIED_DESC =  0x00000008;
        public final static int SORT_BY_SIZE_ASC =  0x00000010;
        public final static int SORT_BY_SIZE_DESC =  0x00000020;
        public final static int SORT_BY_RECENTLY_PLAY_ASC =  0x00000040;
        public final static int SORT_BY_RECENTLY_PLAY_DESC =  0x00000080;
        public final static int SORT_BY_DATE_TAKEN_ASC =  0x00000100;
        public final static int SORT_BY_DATE_TAKEN_DESC =  0x00000200;
        public final static int SORT_BY_DAY_TAKEN_ASC =  0x00000400;
        public final static int SORT_BY_DAY_TAKEN_DESC =  0x00000800;
    }

 
	public DBFileDataProvider (int fileType,int limit,int sortMode,MimeTypes mimeTypes, Context context, Handler handler)
	{
		super.SetName("DBFileDataProvider");
		
	    mFileType = fileType; 
	    mLimitCnt = limit;
	    mSortMode = sortMode;
	    mMimeTypes = mimeTypes;
	    dirnum = 0;
        mContext = context;
        mCallerHandler = handler;
        mCreateFileListTask = new FileListTask();
        mCreateFileListTask.execute();
	    //CreateFileListArray();
		
	}

    public DBFileDataProvider (String photoTakenDate,int limit, MimeTypes mimeTypes, Context context, Handler handler)
	{
		super.SetName("DBFileDataProvider");
		mFileType = FileFilterType.DEVICE_FILE_PHOTO;
	    mPhotoTakenDate = photoTakenDate; 
	    mLimitCnt = limit;
	    mMimeTypes = mimeTypes;
	    dirnum = 0;
        mContext = context;
        mCallerHandler = handler;
        mCreateFileListTask = new FileListTask();
        mCreateFileListTask.execute();
	    //CreatePhotoDateFileListArray();
		
	}
	
	public String GetDataAt     (int i)
	{
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE 
            && !mPhotoDateList.isEmpty() && i>=0 && i<this.GetSize())
        {
            return mPhotoDateHashMap.get(mPhotoDateList.get(i));
        }
		else if( !mFileList.isEmpty() && i>=0 && i<this.GetSize())
		{
			File file =  (File)this.mFileList.get(i).getmFile();
			
			return file.getPath();
			
		}	
		return null;
	}

	public File GetFileAt(int i)
	{
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            return null;
        }
		else if(!mFileList.isEmpty() &&i>=0 && i<this.GetSize())
		{
			File file =  (File)this.mFileList.get(i).getmFile();
			
			return file;
		}	
		return null;
	}

	public String GetTitleAt    (int i)
	{
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE 
            && !mPhotoDateList.isEmpty() && i>=0 && i<this.GetSize())
        {
            return mPhotoDateList.get(i);
        }
		else if(!mFileList.isEmpty() && i>=0 && i<this.GetSize())
		{
			File file =  (File)this.mFileList.get(i).getmFile();
			
			return file.getName();
			
		}	
		return null;
	}
	
	public  String GetMimeTypeAt (int i)
	{
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            return null;
        }
		else if(!mFileList.isEmpty() && i>=0 && i<this.GetSize())
		{
			File file =  (File)this.mFileList.get(i).getmFile();
			
			return mMimeTypes.getMimeType(file.getName());
			
		}	
		return null;
	}
	
	public int GetIdAt(int i)
	{
		if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE 
            && !mPhotoDateList.isEmpty() && i < this.GetSize())
        {
            return i;
        }
		else if(!mFileList.isEmpty() && i>=0 && i<this.GetSize() && !mIDList.isEmpty() )
		{
			return mIDList.get(i).intValue();
			
		}	
		return -1;
	}
	
	public int GetSize ()
	{
        int size =0;
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            if (mQueryDone)
                size = mPhotoDateList.size();
        }
		else
        {
            synchronized(mFileList)
            {
                size = mFileList.size();
            }
        }
		return size;
	}
	
	public int GetFileTypeAt(int i)
	{   
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            return FileFilterType.DEVICE_FILE_PHOTO_DATE;
        }
		if(!mFileList.isEmpty() && i>=0 && i<this.GetSize())
		{
			return this.mFileList.get(i).getmFileType();
			
		}
		
		return FileFilterType.DEVICE_FILE_NONE;
	}
    public long GetSizeAt(int i)
    {
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            return 0;
        }
		if(!mFileList.isEmpty() && i>=0 && i<this.GetSize())
		{
			return this.mFileList.get(i).getSize();
		}
        return 0;
    }
    public void UpdateRecentPlay(String path)
    {
        Uri uri = null;
        if ((mFileType & FileFilterType.DEVICE_FILE_PHOTO) != 0)
            uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
        else if ((mFileType & FileFilterType.DEVICE_FILE_VIDEO) != 0)
            uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;

        if (uri != null)
        {
            try
            {
                String selection = MediaStore.MediaColumns.DATA + " LIKE ?";
                String[] selectionArgs = { String.valueOf(path) };

                ContentResolver cr = mContext.getContentResolver();
                ContentValues values = new ContentValues();
                Date date = new Date();
                values.put("date_recentplay", (date.getTime() / 1000));
                cr.update(
                    uri,
                    values,
                    selection,
                    selectionArgs);
            }
            catch (Exception e)
            {
                Log.e(TAG, "Exception", e);
            }
        }
    }
    /*
    private void CreatePhotoDateFileListArray()
	{
	    if (mPhotoDateHashMap != null)
        {
            ArrayList<PhotoDateInfo> pathList = mPhotoDateHashMap.get(mPhotoTakenDate);
            if (pathList != null)
            {
                for (int i = 0; i < pathList.size(); i++)
                {
                	PhotoDateInfo photoDate = pathList.get(i);
                    FileInfo fileInfo = new FileInfo(FileFilterType.DEVICE_FILE_PHOTO, new File(photoDate.path));
                    this.mFileList.add(fileInfo);
                    this.mIDList.add(photoDate.id);
                }
            }
        }
    }
    */
    public void UpdateData(int Type)
    {
	    if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE){
            mPhotoDateList.clear();
            synchronized(mPhotoDateList_bg)
            {
                synchronized(mPhotoDateList)
                {
                    mPhotoDateList.clear();
                    mPhotoDateList = new ArrayList<String>(mPhotoDateList_bg);
                }
            }
        }
        else{
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
    }

    public ArrayList<String> GetPlayList(int type)
    {
        if (mQueryDone)
        {
            return mPlaylist;
        }
        Log.d(TAG,"GetPlayList Fail, Query is not done.");
        return null;
    }

    public ArrayList<String> GetNameList(int type)
    {
        if (mQueryDone)
        {
            return mNamelist;
        }
        return null;
    }
	private void CreateFileListArray()
	{
        mQueryDone = false;
        //Log.d(TAG, "CreateFileListArray");
        ContentResolver cr = null;
        Cursor cursor = null;

        String order = null;
        Log.d(TAG, "mSortMode = " + mSortMode);
        switch(mSortMode)
        {
            case SortMode.SORT_BY_NAME_ASC :
                order = MediaStore.MediaColumns.DISPLAY_NAME + " ASC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_NAME_DESC :
                order = MediaStore.MediaColumns.DISPLAY_NAME + " DESC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_DATE_MODIFIED_ASC :
                order = MediaStore.MediaColumns.DATE_MODIFIED + " ASC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_DATE_MODIFIED_DESC :
                order = MediaStore.MediaColumns.DATE_MODIFIED + " DESC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_SIZE_ASC :
                order = MediaStore.MediaColumns.SIZE + " ASC";
                Log.d(TAG, "order = " + order);
                break;    
            case SortMode.SORT_BY_SIZE_DESC :
                order = MediaStore.MediaColumns.SIZE + " DESC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_RECENTLY_PLAY_ASC :
                order = "date_recentplay ASC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_RECENTLY_PLAY_DESC :
                order = "date_recentplay DESC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_DATE_TAKEN_ASC :
                order = MediaStore.Images.ImageColumns.DATE_TAKEN + " ASC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_DATE_TAKEN_DESC :
                order = MediaStore.Images.ImageColumns.DATE_TAKEN + " DESC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_DAY_TAKEN_ASC :
                order = "daytaken" + " ASC";
                Log.d(TAG, "order = " + order);
                break;
            case SortMode.SORT_BY_DAY_TAKEN_DESC :
                order = "daytaken" + " DESC";
                Log.d(TAG, "order = " + order);
                break;
            default:
                break;
        }
        if (mFileType == FileFilterType.DEVICE_FILE_AUDIO)
        {
            String[] columns = {MediaStore.MediaColumns._ID, MediaStore.MediaColumns.DATA, MediaStore.MediaColumns.SIZE}; 
            cr = mContext.getContentResolver();
            cursor = cr.query(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI,
            	columns, null, null, order);
        }
        else if ((mFileType & FileFilterType.DEVICE_FILE_VIDEO) != 0)
        {
            String[] columns = {MediaStore.MediaColumns._ID, MediaStore.MediaColumns.DATA, MediaStore.MediaColumns.SIZE}; 
            cr = mContext.getContentResolver();
             
            if (mFileType != FileFilterType.DEVICE_FILE_VIDEO_ISO_ONLY)//All Video
            {
                cursor = cr.query(MediaStore.Video.Media.EXTERNAL_CONTENT_URI,columns, null, null, order);
            }
            else// BD/DVD only
            {
                String Selection=MediaStore.Video.VideoColumns.MIME_TYPE+"=?"+" OR "+MediaStore.Video.VideoColumns.MIME_TYPE+"=?"+" OR "+MediaStore.Video.VideoColumns.MIME_TYPE+"=?";
                String[] SelectionArgs = new String[]{"video/avchd", "video/dvd", "video/iso"};
                //String[] SelectionArgs = new String[]{"video/x-matroska","video/mp4"};
                //String[] SelectionArgs = new String[]{"video/x-matroska"};
                cursor = cr.query(MediaStore.Video.Media.EXTERNAL_CONTENT_URI,columns, Selection, SelectionArgs, order);
            }
        }
        else if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            //String[] columns = {"DISTINCT "+ MediaStore.Images.ImageColumns.DATE_TAKEN}; 
            String[] columns = {"DISTINCT "+ "daytaken"}; 
            cr = mContext.getContentResolver();
            cursor = cr.query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
            	columns, null, null, order);
           
        }
        else if ((mFileType & FileFilterType.DEVICE_FILE_PHOTO)!= 0)
        {
            String[] columns = {MediaStore.MediaColumns._ID, MediaStore.MediaColumns.DATA, MediaStore.MediaColumns.SIZE}; 
            cr = mContext.getContentResolver();
            if (mPhotoTakenDate == null)
            {
                cursor = cr.query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
            	    columns, null, null, order);
            }
            else
            {
                //String Selection=MediaStore.Images.ImageColumns.DATE_TAKEN+"=?";
                String Selection="daytaken=?";
                String[] SelectionArgs = new String[]{mPhotoTakenDate};
                cursor = cr.query(MediaStore.Images.Media.EXTERNAL_CONTENT_URI,
            	    columns, Selection, SelectionArgs, order);
            }
        }
        else
        {   
            Log.e (TAG, "Unknown file type for DBFileDataProvider, mFileType = " + mFileType);
            return;
        }


        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE)
        {
            mPhotoDateHashMap.clear();
            mPhotoDateList_bg.clear();
            while (cursor.moveToNext())
            {
                String time = cursor.getString(cursor
    						//.getColumnIndex(MediaStore.Images.ImageColumns.DATE_TAKEN));
    						.getColumnIndex("daytaken"));

                Date date = new Date(Long.parseLong(time));

                String displayDate = String.format("%tF", date);
                if (mPhotoDateHashMap.containsKey(displayDate))
                {
                    Log.e(TAG, "[ERROR] Query DISTINCT with DATE_TAKEN, We got same DATE_TAKEN.");
                }
                else
                {
                    mPhotoDateHashMap.put(displayDate,time);//displayDate is key, time is value. 
                    mPhotoDateList_bg.add(displayDate);
                }
            }
            /*
            while (cursor.moveToNext())
            {
                String id = cursor.getString(cursor
    						.getColumnIndex(MediaStore.MediaColumns._ID));
                String path = cursor.getString(cursor
    						.getColumnIndex(MediaStore.MediaColumns.DATA));
                String time = cursor.getString(cursor
    						.getColumnIndex(MediaStore.Images.ImageColumns.DATE_TAKEN));

                Date date = new Date(Long.parseLong(time));

                String displayDate = String.format("%tF", date);
				
                PhotoDateInfo photoDate = new PhotoDateInfo();
                photoDate.id = Integer.parseInt(id);
                photoDate.path = path;

                if (mPhotoDateHashMap.containsKey(displayDate))
                {
                    ArrayList<PhotoDateInfo> photoList = mPhotoDateHashMap.get(displayDate);
                    photoList.add(photoDate);
                    mPhotoDateHashMap.put(displayDate, photoList);
                }
                else
                {
                    ArrayList<PhotoDateInfo> photoList = new ArrayList<PhotoDateInfo>();
                    photoList.add(photoDate);
                    mPhotoDateHashMap.put(displayDate, photoList);
                }
            }

            for(Entry<String,ArrayList> entry:mPhotoDateHashMap.entrySet()){
                String key = entry.getKey();
                Log.d(TAG, "Date = " + key);
                ArrayList<PhotoDateInfo> list = entry.getValue();

				int count=0;
				for(count=0; count < mPhotoDateList.size(); count++)
				{
					if( mSortMode!=SortMode.SORT_BY_NAME_DESC && key.compareTo(mPhotoDateList.get(count)) <= 0 )
					{
						break;
					}
					else if( mSortMode==SortMode.SORT_BY_NAME_DESC && key.compareTo(mPhotoDateList.get(count)) > 0 )
					{
						break;
					}
					else if( count == mPhotoDateList.size()-1 )
					{
						count++;
						break;
					}
				}
				mPhotoDateList.add(count, key);

                for (int i = 0; i < list.size(); i++)
                {
                    Log.d(TAG, "path = " + list.get(i).path);
                }
            }
            */
        }
        else
        {
            int i =0;
            int size = cursor.getCount();
            int step = size>2000? size>>2:size+1;
            while (cursor.moveToNext())
            {
                if (mStopQuery == true)
                {
                    break;
                } 
                String path = cursor.getString(cursor
    						.getColumnIndex(MediaStore.MediaColumns.DATA));
                if(path==null) continue;
                String id = cursor.getString(cursor
    						.getColumnIndex(MediaStore.MediaColumns._ID));
                long Size = cursor.getLong(cursor
    						.getColumnIndex(MediaStore.MediaColumns.SIZE));
/*
                        String time = cursor.getString(cursor
                        			.getColumnIndex("date_recentplay"));
                        if (time != null)
                        {
                            date = new Date(Long.parseLong(time) * 1000);
                            Log.d(TAG, path + " date_recentplay = " + time );
                            Log.d(TAG, path + " date_recentplay year = " + (date.getYear() + 1900) );
                            Log.d(TAG, path + " date_recentplay month = " + (date.getMonth() + 1));
                            Log.d(TAG, path + " date_recentplay day = " + date.getDate());
                            Log.d(TAG, path + " hours = " + date.getHours());
                            Log.d(TAG, path + " minutes = " + date.getMinutes());
                            Log.d(TAG, path + " seconds = " + date.getSeconds());
                        }
                        else
                        {
                             Log.d(TAG, path + " date_recentplay = null");
                        }
*/
                /* 
                if (mFileType == FileFilterType.DEVICE_FILE_VIDEO_FILE_ONLY)
                {
                    if (path.endsWith(".iso") || path.endsWith(".ISO"))
                        continue;
                }
                else if (mFileType == FileFilterType.DEVICE_FILE_VIDEO_ISO_ONLY)
                {
                    if (!(path.endsWith(".iso") || path.endsWith(".ISO")) )
                        continue;
                }
                */
                if(isValidFileExtension(mFileType, path)) {
                    FileInfo fileInfo = new FileInfo(AdjustFileType(mFileType), new File(path));
                    fileInfo.setSize(Size);
                    mPlaylist.add(Util.getUri(Util.getAppCtx(), "file://" + fileInfo.getmFile().getPath()).toString());
                    mNamelist.add(fileInfo.getmFile().getName());
                    synchronized (mFileList_bg) {
                        this.mFileList_bg.add(fileInfo);
                    }
                    this.mIDList.add(Integer.parseInt(id));
                }
                i++;
                if (i>0 && i%step==0)
                {
                    Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_PORTION, size, i);
                    mCallerHandler.sendMessage(msg);
                }
            }
        }
        if (cursor != null)
        {
            cursor.close();
        }
        mQueryDone = true;
	}

    private boolean isValidFileExtension(int fileType, String path) {
        if(fileType == FileFilterType.DEVICE_FILE_PHOTO_DATE ||
                (fileType & FileFilterType.DEVICE_FILE_PHOTO)!=0){
            return mMimeTypes.isImageFile(path);
        }
        return true;
    }

    private int AdjustFileType(int type)
    {
        if (type == FileFilterType.DEVICE_FILE_VIDEO_ISO_ONLY)
        {
            return FileFilterType.DEVICE_FILE_VIDEO;
        }
        else
        {
            return type;
        }
    }
	public int getDirnum() {
		return dirnum;
	}

	@SuppressWarnings("unchecked")
	public void sortListByType() {
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
        }
        @Override
        protected Void doInBackground(Void... para)
        {
	        Log.d(TAG,"doInBackground [Start]");
            if (!isCancelled())
            {
                Log.d(TAG,"Stop Previous work without check");
                CreateFileListArray();
            }
	        Log.d(TAG,"doInBackground [End]");
            return null;
        }
        @Override
        protected void onPostExecute (Void rlt)
        {
	        Log.d(TAG,"onPostExecute [Start]");
            if (!isCancelled())
            {
                Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_DONE,0,0);
                mCallerHandler.sendMessage(msg);
            }
	        Log.d(TAG,"onPostExecute [End]");
        }
        @Override
        protected void onCancelled()
        {
            super.onCancelled();
	        Log.d(TAG,"onCancelled [Start]");
            Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_ABORT,0,0);
            mCallerHandler.sendMessage(msg);
	        Log.d(TAG,"onCancelled [End]");
        }
        @Override
        protected void finalize ()throws Throwable
        {
            Log.d(TAG,"FileListTask finalize for Path:");
            super.finalize();
        }
    }
}
