package com.rtk.mediabrowser;

import android.provider.BaseColumns;
import android.database.sqlite.SQLiteOpenHelper;
import com.realtek.Utils.MbwType;
import android.database.Cursor;
import android.content.ContentValues;
import android.database.sqlite.SQLiteDatabase;
import android.content.Context;
/*
    _id   BWType    ViewType    Category        Sortby
    0     Video     Preview     All or BD       NameAToZ
    1     Photo     Thumbnail   All or Date     NameAToZ
    2     AllDev    Preview     None            None
*/
public class DataBaseHelper extends SQLiteOpenHelper {

    public static final String TEXT_TYPE=" TEXT";
    private static final String COMMA_SEP = ",";
    private static final String TABLE_NAME = "MediaBrowser";
    //Entry
    public static final String BW_TYPE = "Browseing_type";
    public static final String VIEW_TYPE = "View_type";
    public static final String CATEGORY = "Category";
    public static final String SORT_BY = "SortBy";

    //Data 
    public static final String VIDEO_VIEW = "video_view";
    public static final String PHOTO_VIEW = "photo_view";
    public static final String DEV_VIEW = "dev_view";
    public static final String PREVIEW = "Preview";
    public static final String THUMBNAIL = "Thumbnail";
    public static final String CATEGORY_ALL    = "All";
    public static final String CATEGORY_BD     = "BD";
    public static final String CATEGORY_DATE   = "Date";
    public static final String SORT_NAME_AZ = "Name_a_z"; 
    public static final String SORT_NAME_ZA = "Name_z_a"; 

    private static final String SQL_CREATE_ENTRIES =
    "CREATE TABLE " + TABLE_NAME + " (" +
    BaseColumns._ID + " INTEGER PRIMARY KEY," +
    BW_TYPE+" "+ TEXT_TYPE + COMMA_SEP +
    VIEW_TYPE+" "+ TEXT_TYPE + COMMA_SEP +
    CATEGORY+" "+ TEXT_TYPE + COMMA_SEP +
    SORT_BY+" "+ TEXT_TYPE +
    " )";

    private static final String SQL_DELETE_ENTRIES =
    "DROP TABLE IF EXISTS " + TABLE_NAME;
    /*
    private static final String SQL_DEFAULT_VALUES = 
    "INSERT INTO " + TABLE_NAME + 
    " (" + BaseColumns._ID+COMMA_SEP+VIDEO_VIEW+COMMA_SEP+PHOTO_VIEW+COMMA_SEP+DEV_VIEW+") VALUES (" +
    "1"+COMMA_SEP+"\""+PREVIEW+"\""+COMMA_SEP+"\""+THUMBNAIL+"\""+COMMA_SEP+"\""+PREVIEW+"\""+
    ")";
     */
    public static final int DATABASE_VERSION = 1;
    public static final String DATABASE_NAME = "MediaBrowser.db";


    public DataBaseHelper (Context context)
    {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
    }
   
    public void onCreate(SQLiteDatabase db)
    {
        db.execSQL(SQL_CREATE_ENTRIES);
        
        ContentValues values = new ContentValues();
        //Video
        values.put(BW_TYPE,VIDEO_VIEW);
        values.put(VIEW_TYPE,PREVIEW);
        values.put(CATEGORY,CATEGORY_ALL);
        values.put(SORT_BY,SORT_NAME_AZ);
        db.insert(TABLE_NAME, null, values);
        //Photo
        values.clear(); 
        values.put(BW_TYPE,PHOTO_VIEW);
        values.put(VIEW_TYPE,THUMBNAIL);
        values.put(CATEGORY,CATEGORY_ALL);
        values.put(SORT_BY,SORT_NAME_AZ);
        db.insert(TABLE_NAME, null, values);
        //All Dev
        values.clear(); 
        values.put(BW_TYPE,DEV_VIEW);
        values.put(VIEW_TYPE,PREVIEW);
        values.put(CATEGORY,CATEGORY_ALL);
        values.put(SORT_BY,SORT_NAME_AZ);
        db.insert(TABLE_NAME, null, values);
    }
    
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        // This database is only a cache for online data, so its upgrade policy is
        // to simply to discard the data and start over
        db.execSQL(SQL_DELETE_ENTRIES);
        onCreate(db);
    }

    public void UpdateChange(int type, String entry, String Newdate)
    {
        SQLiteDatabase db = getWritableDatabase();
        ContentValues values = new ContentValues();
        values.put(entry,Newdate);
        String[] args = {TypeTransfer(type)};
        db.update(TABLE_NAME, values, BW_TYPE+"=?",args);
        db.close();
    }
    public String QueryDate(int type, String entry)
    {
        SQLiteDatabase db = getReadableDatabase();
        String[] columns = {entry};
        String[] args = {TypeTransfer(type)};
        Cursor cursor = null;
        String Rlt = null;
        cursor = db.query(TABLE_NAME, columns, BW_TYPE+"=?", args, null, null, null);
        while (cursor.moveToNext())
        {
            Rlt = cursor.getString(cursor.getColumnIndex(entry));      
        }
        cursor.close();
        db.close();
        return Rlt;
    }
    private String TypeTransfer(int type)
    {
        String Rlt =null;
        if (type == MbwType.TYPE_DEVICE_VIDEO)
            Rlt = VIDEO_VIEW;
        else if (type == MbwType.TYPE_DEVICE_PHOTO)
            Rlt = PHOTO_VIEW;
        else
            Rlt = DEV_VIEW;
        return Rlt;
    }
}
