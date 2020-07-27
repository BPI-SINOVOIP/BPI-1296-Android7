package com.example.hdmirxdemo;

import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;
import android.database.Cursor;
import android.database.DatabaseUtils;
import android.content.Context;
import android.content.ContentValues;
import android.util.Log;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;
import java.util.ArrayList;

public class HDMIDbHelper extends SQLiteOpenHelper {

    private static final boolean DEBUG_VERBOSE = Keys.DB_HELPER_DEBUG_VERBOSE;

    private static final String TAG="HDMIDbHelper";

    private static final String TAG_VERBOSE = TAG+"-VERBOSE";

    private static final String TEXT_TYPE = " TEXT";
    private static final String INTEGER_TYPE = " INTEGER";
    private static final String BIGINT_TYPE = " BIGINT";
    private static final String COMMA_SEP = ",";

    private static final Object sLock = new Object();

    private static final String SQL_CREATE_ENTRIES =
        "CREATE TABLE " + SchRecContract.SchRecEntry.TABLE_NAME + " (" +
        SchRecContract.SchRecEntry._ID + " INTEGER PRIMARY KEY," +

        // start date
        SchRecContract.SchRecEntry.COLUMN_NAME_START_YEAR + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_START_MONTH + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_START_DAY + INTEGER_TYPE + COMMA_SEP +

        // start time
        SchRecContract.SchRecEntry.COLUMN_NAME_START_HOUR + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_START_MIN + INTEGER_TYPE + COMMA_SEP +

        // end date
        SchRecContract.SchRecEntry.COLUMN_NAME_END_YEAR + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_END_MONTH + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_END_DAY + INTEGER_TYPE + COMMA_SEP +

        // end time
        SchRecContract.SchRecEntry.COLUMN_NAME_END_HOUR + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_END_MIN + INTEGER_TYPE + COMMA_SEP +

        // dimension
        SchRecContract.SchRecEntry.COLUMN_NAME_WIDTH + INTEGER_TYPE + COMMA_SEP +
        SchRecContract.SchRecEntry.COLUMN_NAME_HEIGHT + INTEGER_TYPE + COMMA_SEP +

        // format
        SchRecContract.SchRecEntry.COLUMN_NAME_FORMAT + INTEGER_TYPE + COMMA_SEP +
        // path
        SchRecContract.SchRecEntry.COLUMN_NAME_PATH + TEXT_TYPE + COMMA_SEP +

        // compare time
        SchRecContract.SchRecEntry.COLUMN_NAME_COMPARE_TIME+ BIGINT_TYPE + " )";

    private static final String SQL_DELETE_ENTRIES =
        "DROP TABLE IF EXISTS " + SchRecContract.SchRecEntry.TABLE_NAME;

    public static final int DATABASE_VERSION = 1;
    public static final String DATABASE_NAME = "hdmi.db";

    private static HDMIDbHelper sInstance = null;

    private DbListener mListener = null;
    private DbChangeUIListener mUIChangeListener = null;

    public HDMIDbHelper(Context context) {
        super(context, DATABASE_NAME, null, DATABASE_VERSION);
        Log.d(TAG_VERBOSE,"create new HDMIDbHelper");
        mListener = new DbListenerImpl(context,this);
        deleteOutdatedItems();
        //registerRecList();
    }

    public void onCreate(SQLiteDatabase db) {
        synchronized(sLock){
            db.execSQL(SQL_CREATE_ENTRIES);
        }
    }

    public synchronized static HDMIDbHelper getInstance(Context c){
        if(sInstance==null){
            sInstance = new HDMIDbHelper(c);
            sInstance.registerRecList();
        }
        return sInstance;
    }

    public void registerRecList(){

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "Call registerRecList");
        }

        // bad workaround, but accept it.
        ((DbListenerImpl)mListener).initSPIListAndRegister(getAllItemsByOrder());
    }

    public DbListenerImpl.SchRecItmPI getLatestSchItem(){

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "Call getLatestSchItem");
        }

        // bad workaround, but accept it.
        DbListenerImpl.SchRecItmPI rst = ((DbListenerImpl)mListener).getLatestSchItem();

        if(DEBUG_VERBOSE) {
            if(rst != null){
                Log.i(TAG_VERBOSE, "getLatestSchItem - item: \""+rst.item.getDisplayString()+"\"");
            }else{
                Log.i(TAG_VERBOSE, "getLatestSchItem - item: null");
            }
        }

        return rst;
    }

    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL(SQL_DELETE_ENTRIES);
        onCreate(db);
    }

    public void onDowngrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        onUpgrade(db, oldVersion, newVersion);
    }

    public int numberOfRows(){
        SQLiteDatabase db = getReadableDatabase();
        int numRows = (int) DatabaseUtils.queryNumEntries(db, SchRecContract.SchRecEntry.TABLE_NAME);

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "call - numberOfRows : "+numRows);
        }

        return numRows;
    }

    public void setDbChangeUIListener(DbChangeUIListener l){
        mUIChangeListener = l;
    }

    /*
    public synchronized Cursor getData(int id) {
        SQLiteDatabase db = getReadableDatabase();
        String cmd = "select * from "+SchRecContract.SchRecEntry.TABLE_NAME+" where id="+id+"";
        Cursor res =  db.rawQuery( cmd, null );
        return res;
    }
    */

    public synchronized ArrayList<SchRecItem2> getAllItemsByOrder() {
        ArrayList<SchRecItem2> array_list = new ArrayList<SchRecItem2>();
        SQLiteDatabase db = this.getReadableDatabase();
        String cmd = "select * from "+SchRecContract.SchRecEntry.TABLE_NAME+
                    " order by "+SchRecContract.SchRecEntry.COLUMN_NAME_COMPARE_TIME+" ASC";

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getAllItemsByOrder - cmd: "+cmd);
        }

        Cursor res = db.rawQuery( cmd , null );
        res.moveToFirst();
        while(res.isAfterLast() == false){
            SchRecItem2 item = getItemFromCursor(res);
            array_list.add(item);
            res.moveToNext();
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getAllItemsByOrder finish, close cursor");
        }

        // close cursor
        res.close();

        if(DEBUG_VERBOSE) {
            dumpItemList(array_list);
        }

        return array_list;
    }

    private void dumpItemList(ArrayList<SchRecItem2> list) {

        Log.i(TAG_VERBOSE, "\tDUMP itemList - start size: "+list.size());

        for(int i=0;i<list.size();i++) {
            Log.i(TAG_VERBOSE,"\t\titem - "+i+ " \""+list.get(i).getDisplayString()+"\"");
        }

        Log.i(TAG_VERBOSE, "\tDUMP itemList - finish size: "+list.size());
    }

    private SchRecItem2 getItemFromCursor(Cursor res) {

        int syear = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_START_YEAR));
        int smonth = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_START_MONTH));
        int sday = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_START_DAY));

        int sh = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_START_HOUR));
        int sm = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_START_MIN));

        int eyear = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_END_YEAR));
        int emonth = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_END_MONTH));
        int eday = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_END_DAY));

        int eh = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_END_HOUR));
        int em = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_END_MIN));

        int width = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_WIDTH));
        int height = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_HEIGHT));

        int format = res.getInt(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_FORMAT));
        String path = res.getString(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_PATH));

        long compareTimeFromDb = res.getLong(res.getColumnIndex(SchRecContract.SchRecEntry.COLUMN_NAME_COMPARE_TIME));

        Calendar start = new GregorianCalendar(syear,smonth,sday,sh, sm);
        //start.set(syear,smonth,sday,sh,sm);

        Calendar end = new GregorianCalendar(eyear,emonth,eday,eh,em);
        //end.set(eyear,emonth,eday,eh,em);

        SchRecItem2 item = new SchRecItem2(start,end,path,width,height,format);
        item.mCompareTimeInDb = compareTimeFromDb;
        return item;
    }

    public SchRecItem2 getOverlappingItem(SchRecItem2 item) {

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "call getOverlappingItem of item : \""+item.getDisplayString()+"\"");
        }

        ArrayList<SchRecItem2> list = getAllItemsByOrder();
        for(int i=0;i<list.size();i++){
            if(item.isOverlapping(list.get(i))){

                if(DEBUG_VERBOSE) {
                    Log.i(TAG_VERBOSE, "\tfind overlapping item is \""+list.get(i).getDisplayString()+"\" - src is :\""+item.getDisplayString()+"\"");
                }

                return list.get(i);
            }
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getOverlappingItem find nothing");
        }

        return null;
    }

    public void insert(SchRecItem2 data){
        //Log.d(TAG_VERBOSE, "inset data:"+data.getDisplayString()+" to db");
        ContentValues values = new ContentValues();

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_START_YEAR, data.mStartTime.get(SchRecItem2.YEAR));
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_START_MONTH, data.mStartTime.get(SchRecItem2.MONTH));
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_START_DAY, data.mStartTime.get(SchRecItem2.DATE));

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_START_HOUR, data.mStartTime.get(SchRecItem2.HOUR));
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_START_MIN, data.mStartTime.get(SchRecItem2.MINUTE));

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_END_YEAR, data.mEndTime.get(SchRecItem2.YEAR));
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_END_MONTH, data.mEndTime.get(SchRecItem2.MONTH));
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_END_DAY, data.mEndTime.get(SchRecItem2.DATE));

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_END_HOUR, data.mEndTime.get(SchRecItem2.HOUR));
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_END_MIN, data.mEndTime.get(SchRecItem2.MINUTE));

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_WIDTH, data.mWidth);
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_HEIGHT, data.mHeight);

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_FORMAT,data.mFormat);
        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_PATH, data.mRecordPath);

        long compareTime = data.getCompareTime();
        Log.d(TAG, "insert compareTime:"+compareTime);

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "insert item: \""+data.getDisplayString()+ "\" calculated compareTime:"+compareTime);
        }

        values.put(SchRecContract.SchRecEntry.COLUMN_NAME_COMPARE_TIME, compareTime);

        SQLiteDatabase db = getWritableDatabase();
        long newRowId = db.insert(SchRecContract.SchRecEntry.TABLE_NAME, null, values);

        // use close to flush, not so sure the effect right now
        db.close();

        Log.d(TAG_VERBOSE+"-DbListener","insert - "+mListener);

        if(mListener != null){
            mListener.onItemInserted(data);
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "insert finish - dump db status");
            dumpItemList(getAllItemsByOrder());
        }
    }

    public synchronized void deleteItem(SchRecItem2 item, boolean updateDbListener) {
        SQLiteDatabase db = this.getWritableDatabase();

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "call deleteItem - item: \""+item.getDisplayString()+"\" updateDbListener:  "+updateDbListener);
            Log.i(TAG_VERBOSE, "db content before deleting:");
            dumpItemList(getAllItemsByOrder());
        }

        long cmpTime = item.getCompareTime();

        Log.d(TAG, "delete compareTime:"+cmpTime+" doUpdate:"+updateDbListener);

        String table = SchRecContract.SchRecEntry.TABLE_NAME;
        String selection = SchRecContract.SchRecEntry.COLUMN_NAME_COMPARE_TIME + " = ?";
        String[] args = {""+cmpTime};
        db.delete(table, selection, args);

        db.close();

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "deleteItem - finish - dblistener info: mListener:"+mListener+" updateDbListener:"+updateDbListener);
        }

        if(mListener != null && updateDbListener){
            mListener.onItemDeleted(item);
        }

        int dbItems = numberOfRows();

        Log.d(TAG, "delete done - mUIChangeListener:"+mUIChangeListener+" numsofrow:"+dbItems);

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "db content after deleting:");
            dumpItemList(getAllItemsByOrder());
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "deleteItem - finish - check & call UIChangeListener "+mUIChangeListener);
        }

        if(mUIChangeListener != null) {
            mUIChangeListener.onDbUIUpdate();
        }
    }

    public synchronized void deleteOutdatedItems() {

        Log.d(TAG, "deleteOutdatedItems");

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "deleteOutdatedItems - start");
            dumpItemList(getAllItemsByOrder());
        }

        SQLiteDatabase db = this.getWritableDatabase();
        SchRecItem2 current = SchRecItem2.getCurrentTimeSchRecItem2();
        long currentTime = current.getCompareTime();

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "deleteOutdatedItems - currentTimeCmp "+currentTime);
        }

        String table = SchRecContract.SchRecEntry.TABLE_NAME;
        String selection = SchRecContract.SchRecEntry.COLUMN_NAME_COMPARE_TIME + " <= ?";
        String[] args = {""+currentTime};
        db.delete(table, selection, args);
        // TODO: not sure the effect
        db.close();
        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "deleteOutdatedItems - finish");
            dumpItemList(getAllItemsByOrder());
        }
    }
}
