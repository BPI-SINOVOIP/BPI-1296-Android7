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
import android.text.TextUtils;
import android.util.Log;
import com.realtek.Utils.FileInfo;
import com.realtek.Utils.MimeTypes;
import com.rtk.mediabrowser.Util;

import java.io.File;
import java.util.*;

public class QueryDataProvider extends AbstractDataProvider {
    private static final String TAG = "QueryDataProvider";
    private static final boolean DEBUG = true;
    private final String mQueryStr;
    private static final int QUERY_VIDEO = 0;
    private static final int QUERY_AUDIO = 1;
    private static final int QUERY_IMAGE = 2;

    private int mFileType = FileFilterType.DEVICE_FILE_NONE;
    private int mLimitCnt = 0;
    private int mSortMode = 0;
    private MimeTypes mMimeTypes = null;
    private int dirnum = 0;
    private String mPhotoTakenDate = null;
    private Context mContext;
    //static private LinkedHashMap<String, ArrayList> mPhotoDateHashMap = new LinkedHashMap<String, ArrayList>();
    static private HashMap<String, String> mPhotoDateHashMap = new HashMap<String, String>();

    private Handler mCallerHandler = null;
    private boolean mStopQuery = false;
    private boolean mQueryDone = false;
    private FileListTask mCreateFileListTask = null;
    private ArrayList<FileInfo> mFileList = new ArrayList<FileInfo>();
    private ArrayList<Integer> mIDList = new ArrayList<Integer>();
    private ArrayList<String> mPhotoDateList = new ArrayList<String>();

    private final ArrayList<FileInfo> mFileList_bg = new ArrayList<FileInfo>();
    private ArrayList<String> mPlaylist_Audio = new ArrayList<String>();
    private ArrayList<String> mPlaylist_Video = new ArrayList<String>();
    private ArrayList<String> mPlaylist_Photo = new ArrayList<String>();
    private ArrayList<String> mNamelist_Audio = new ArrayList<String>();
    private ArrayList<String> mNamelist_Video = new ArrayList<String>();
    private ArrayList<String> mNamelist_Photo = new ArrayList<String>();
    private ArrayList<String> mPlaylist_BD = new ArrayList<>();
    private ArrayList<String> mNamelist_BD = new ArrayList<>();

    public class PhotoDateInfo {
        private int id;
        private String path;
    }

    public class SortMode {
        public final static int SORT_BY_NAME_ASC = 0x00000001;
        public final static int SORT_BY_NAME_DESC = 0x00000002;
        public final static int SORT_BY_DATE_MODIFIED_ASC = 0x00000004;
        public final static int SORT_BY_DATE_MODIFIED_DESC = 0x00000008;
        public final static int SORT_BY_SIZE_ASC = 0x00000010;
        public final static int SORT_BY_SIZE_DESC = 0x00000020;
        public final static int SORT_BY_RECENTLY_PLAY_ASC = 0x00000040;
        public final static int SORT_BY_RECENTLY_PLAY_DESC = 0x00000080;
        public final static int SORT_BY_DATE_TAKEN_ASC = 0x00000100;
        public final static int SORT_BY_DATE_TAKEN_DESC = 0x00000200;
        public final static int SORT_BY_DAY_TAKEN_ASC = 0x00000400;
        public final static int SORT_BY_DAY_TAKEN_DESC = 0x00000800;
    }


    public QueryDataProvider(int fileType, String queryStr, int limit, int sortMode, MimeTypes mimeTypes, Context context, Handler handler) {
        super.SetName("DBFileDataProvider");

        mFileType = fileType;
        mQueryStr = queryStr;
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

    public QueryDataProvider(String photoTakenDate, int limit, MimeTypes mimeTypes, Context context, Handler handler) {
        super.SetName("DBFileDataProvider");
        mQueryStr = "";
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

    public String GetDataAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE
                && !mPhotoDateList.isEmpty() && i >= 0 && i < this.GetSize()) {
            return mPhotoDateHashMap.get(mPhotoDateList.get(i));
        } else if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();

            return file.getPath();

        }
        return null;
    }

    public File GetFileAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE) {
            return null;
        } else if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();

            return file;
        }
        return null;
    }

    public String GetTitleAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE
                && !mPhotoDateList.isEmpty() && i >= 0 && i < this.GetSize()) {
            return mPhotoDateList.get(i);
        } else if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();

            return file.getName();

        }
        return null;
    }

    public String GetMimeTypeAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE) {
            return null;
        } else if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize()) {
            File file = (File) this.mFileList.get(i).getmFile();

            return mMimeTypes.getMimeType(file.getName());

        }
        return null;
    }

    public int GetIdAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE
                && !mPhotoDateList.isEmpty() && i < this.GetSize()) {
            return i;
        } else if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize() && !mIDList.isEmpty()) {
            return mIDList.get(i).intValue();

        }
        return -1;
    }

    public int GetSize() {
        int size = 0;
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE) {
            if (mQueryDone)
                size = mPhotoDateList.size();
        } else {
            synchronized (mFileList) {
                size = mFileList.size();
            }
        }
        return size;
    }

    public int GetFileTypeAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE) {
            return FileFilterType.DEVICE_FILE_PHOTO_DATE;
        }
        if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize()) {
            return this.mFileList.get(i).getmFileType();

        }

        return FileFilterType.DEVICE_FILE_NONE;
    }

    public long GetSizeAt(int i) {
        if (mFileType == FileFilterType.DEVICE_FILE_PHOTO_DATE) {
            return 0;
        }
        if (!mFileList.isEmpty() && i >= 0 && i < this.GetSize()) {
            return this.mFileList.get(i).getSize();
        }
        return 0;
    }

    public void UpdateRecentPlay(String path) {
        Uri uri = null;
        if ((mFileType & FileFilterType.DEVICE_FILE_PHOTO) != 0)
            uri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
        else if ((mFileType & FileFilterType.DEVICE_FILE_VIDEO) != 0)
            uri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;

        if (uri != null) {
            try {
                String selection = MediaStore.MediaColumns.DATA + " LIKE ?";
                String[] selectionArgs = {String.valueOf(path)};

                ContentResolver cr = mContext.getContentResolver();
                ContentValues values = new ContentValues();
                Date date = new Date();
                values.put("date_recentplay", (date.getTime() / 1000));
                cr.update(
                        uri,
                        values,
                        selection,
                        selectionArgs);
            } catch (Exception e) {
                Log.e(TAG, "Exception", e);
            }
        }
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
            else if (type == FileFilterType.DEVICE_FILE_BD_FOLDER)
                return mPlaylist_BD;
            else
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
            else if (type == FileFilterType.DEVICE_FILE_BD_FOLDER)
                return mNamelist_BD;
            else
                return null;
        }
        return null;
    }

    private List<String> queryMediaStore(int queryType, String query) {
        Context ctx = mContext;

        String[] projection = new String[]{MediaStore.MediaColumns.DATA};

        String selection = null;
        String[] selectionArgs = null;
        String[] querys = query.split(" ");
        if (querys != null && querys.length > 0) {
            selection = MediaStore.Files.FileColumns.DATA + " like ?";
            selectionArgs =
                    new String[]{"%" + TextUtils.join("%", querys) + "%"}; // there is no ? in selection so null here
        }

        Uri qUri = null;
        switch(queryType){
            case QUERY_VIDEO:
                qUri = MediaStore.Video.Media.EXTERNAL_CONTENT_URI;
                break;
            case QUERY_AUDIO:
                qUri = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
                break;
            case QUERY_IMAGE:
                qUri = MediaStore.Images.Media.EXTERNAL_CONTENT_URI;
                break;
        }

        String order = MediaStore.MediaColumns.DATE_MODIFIED;
        Cursor cursor = ctx.getContentResolver()
                .query(qUri,
                        projection,
                        selection,
                        selectionArgs,
                        order + " DESC");
        if (cursor == null) {
            log("no data");
            return null;
        }
        ArrayList<String> ret = null;
        try {
            log("begin cursor cursor.getCount()=" + cursor.getCount());
            if (cursor.getCount() > 0) {
                ret = new ArrayList<String>();
                while (cursor.moveToNext()) {
                    String data = cursor.getString(0);
                    ret.add(data);
                }
            }
        } finally {
            cursor.close();
        }
        log("end cursor");
        if (ret != null)
            log("ret=" + Arrays.toString(ret.toArray()));
        return ret;
    }

    private void CreateFileListArray() {
        mQueryDone = false;
        //Log.d(TAG, "CreateFileListArray");
        ArrayList<String> queryResults = new ArrayList<>();
        List<String> videoSets = queryMediaStore(QUERY_VIDEO, mQueryStr);
        if (videoSets != null)
            queryResults.addAll(videoSets);
        videoSets = null;
        List<String> audioSets = queryMediaStore(QUERY_AUDIO, mQueryStr);
        if (audioSets != null)
            queryResults.addAll(audioSets);
        audioSets = null;
        List<String> photoSets = queryMediaStore(QUERY_IMAGE, mQueryStr);
        if(photoSets!=null)
            queryResults.addAll(photoSets);
        photoSets = null;

        for (String path : queryResults) {
            File f = null;
            int fileFilterType = -1;
            if (mMimeTypes.isVideoFile(path)) {
                f = new File(path);
                fileFilterType = FileFilterType.DEVICE_FILE_VIDEO;
            } else if (mMimeTypes.isAudioFile(path)) {
                f = new File(path);
                fileFilterType = FileFilterType.DEVICE_FILE_AUDIO;
            } else if(mMimeTypes.isImageFile(path)){
                f= new File(path);
                fileFilterType = FileFilterType.DEVICE_FILE_PHOTO;
            }
            if(f!=null && fileFilterType!=-1) {
                FileInfo fileInfo = new FileInfo(
                        fileFilterType,
                        f,
                        f.getName(),
                        Util.getUri(Util.getAppCtx(), "file://" + path).toString()
                );
                fileInfo.setSize(f.length());
                synchronized (mFileList_bg) {
                    mFileList_bg.add(fileInfo);
                }
            }
        }

        sortListByType();
        getPlaylist(mFileList_bg);
        mQueryDone = true;
    }

    private void getPlaylist(ArrayList<FileInfo> allFiles) {
        for (FileInfo fi : allFiles) {
            if (fi.mFileType == FileFilterType.DEVICE_FILE_AUDIO) {
                mPlaylist_Audio.add(fi.path);
                mNamelist_Audio.add(fi.fileName);
            } else if (fi.mFileType == FileFilterType.DEVICE_FILE_VIDEO) {
                mPlaylist_Video.add(fi.path);
                mNamelist_Video.add(fi.fileName);
            } else if (fi.mFileType == FileFilterType.DEVICE_FILE_PHOTO) {
                mPlaylist_Photo.add(fi.path);
                mNamelist_Photo.add(fi.fileName);
            } else if (fi.mFileType == FileFilterType.DEVICE_FILE_DIR) {
//                File f = fi.getmFile();
//                String playEntry = getBdDvdPlayEntry(f.toString());
//                if(playEntry!=null) {
//                    mPlaylist_BD.add(fi.path);
//                    mNamelist_BD.add(f.getName());
//                }
            }
        }
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

    private int AdjustFileType(int type) {
        if (type == FileFilterType.DEVICE_FILE_VIDEO_ISO_ONLY) {
            return FileFilterType.DEVICE_FILE_VIDEO;
        } else {
            return type;
        }
    }

    public int getDirnum() {
        return dirnum;
    }

    public void StopQuery() {
        if (mCreateFileListTask != null) {
            mCreateFileListTask.cancel(true);
        }
        mStopQuery = true;
    }

    private class FileListTask extends AsyncTask<Void, Void, Void> {
        public FileListTask() {
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
        }

        @Override
        protected Void doInBackground(Void... para) {
            Log.d(TAG, "doInBackground [Start]");
            if (!isCancelled()) {
                Log.d(TAG, "Stop Previous work without check");
                CreateFileListArray();
            }
            Log.d(TAG, "doInBackground [End]");
            return null;
        }

        @Override
        protected void onPostExecute(Void rlt) {
            Log.d(TAG, "onPostExecute [Start]");
            if (!isCancelled()) {
                Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_DONE, 0, 0);
                mCallerHandler.sendMessage(msg);
            }
            Log.d(TAG, "onPostExecute [End]");
        }

        @Override
        protected void onCancelled() {
            super.onCancelled();
            Log.d(TAG, "onCancelled [Start]");
            Message msg = Message.obtain(mCallerHandler, FILE_LIST_CREATE_ABORT, 0, 0);
            mCallerHandler.sendMessage(msg);
            Log.d(TAG, "onCancelled [End]");
        }

        @Override
        protected void finalize() throws Throwable {
            Log.d(TAG, "FileListTask finalize for Path:");
            super.finalize();
        }
    }

    private static void log(String s) {
        if (DEBUG) Log.d(TAG, s);
    }
}
