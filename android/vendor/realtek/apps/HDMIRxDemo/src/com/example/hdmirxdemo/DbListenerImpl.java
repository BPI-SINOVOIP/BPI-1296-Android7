package com.example.hdmirxdemo;

import android.os.SystemClock;
import android.os.Bundle;
import android.os.Handler;
import android.os.PowerManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.util.Log;
import android.app.AlarmManager;
import android.widget.Toast;

import java.util.List;
import java.util.ArrayList;
import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;

public class DbListenerImpl implements DbListener{

    private static final String TAG_VERBOSE = "DbListenerImpl-VERBOSE";

    private static final String TAG = "HDMIDbHelper-DbListenerImpl-Suspend";

    private static final boolean DEBUG_VERBOSE = Keys.DB_LISTENER_DEBUG_VERBOSE;

    private static final String SCH_REC_BROADCAST = "android.intent.action.HDMIRX_RECORD";
    private static final String ADD_REC_BROADCAST = "android.intent.action.WAKE_UP_ADD";
    private static final String RM_REC_BROADCAST = "android.intent.action.WAKE_UP_RM";

    private static final long WAKEUP_RESERVED_TIME_MS = 90*1000;
    private static final long DETECT_H264_TIME_MS = 15*1000;

    private static final String KEY_START_TIME_MILLIS = "timeStart";
    private static final String KEY_END_TIME_MILLIS = "timeEnd";

    private static final String BUNDLE_NAME="arg";
    //private static int sRequestCode = 1;

    private final Handler mHandler = GeneralService2.mHandler;

    private List<SchRecItmPI> mSPis = new ArrayList<SchRecItmPI>();
    private Context mContext = null;
    private static PowerManager mPm = null;
    private HDMIDbHelper mDbHelper;

    /*
    static {
        System.loadLibrary("hdmidemoutils");
    }

    public native void sendSchBC(int bc, long s, long e);
    */

    public DbListenerImpl(Context c,HDMIDbHelper dbHelper){
        Log.d(TAG_VERBOSE,"create DbListenerImpl");
        Utils.getInstance().init();
        mContext = c;
        mDbHelper = dbHelper;
    }

    class SchRecItmPI {

        private static final String TAG2 = TAG+"-SchRecItmPI";
        private static final String TAG2_VERBOSE = "DbListenerImpl-SchRecItmPI-VERBOSE";

        public SchRecItem2 item;
        public long bookTimeMillis;
        public long lockTimeMillis;
        public long detectH264TimeMillis;
        public Bundle mBundle;
        public boolean bWarningDisplayed=false;

        PowerManager.WakeLock mStrongWakeLock;
        PowerManager.WakeLock mPartialWakeLock;

        AcquireRunnable mAcqRunnable = new AcquireRunnable();
        ReleaseRunnable mRlsRunnable = new ReleaseRunnable();
        public ScheduleRecordRunnable mRecRunnable = new ScheduleRecordRunnable();

        SchRecItmPI(SchRecItem2 item){
            this.item = item;
            bookTimeMillis = item.mStartTime.getTimeInMillis();
            lockTimeMillis = bookTimeMillis-WAKEUP_RESERVED_TIME_MS;
            detectH264TimeMillis = bookTimeMillis-DETECT_H264_TIME_MS;
            mStrongWakeLock = null;
            mPartialWakeLock = null;
            createBundle();
        }

        public void createBundle(){
            Bundle b = new Bundle();
            b.putString(Keys.KEY_OUTPUT_PATH,item.mRecordPath);
            b.putInt(Keys.KEY_OUTPUT_FORMAT,item.mFormat);
            b.putLong(Keys.KEY_RECORD_END_TIME_MILLIS,item.mEndTime.getTimeInMillis());
            b.putLong(Keys.KEY_BOOKED_START_TIME_MILLIS, item.mStartTime.getTimeInMillis());
            b.putInt(Keys.KEY_RECORD_WIDTH,item.mWidth);
            b.putInt(Keys.KEY_RECORD_HEIGHT,item.mHeight);
            mBundle = b;
        }

        class ScheduleRecordRunnable implements Runnable {
            public void run(){

                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE, "start to run ScheduleRecordRunnable");
                }

                refreshSPIListAndDb();

                // bad work-around, then check time again
                //long startTimeMillis = bookTimeMillis;
                // current time:
                long currentTimeMillis = getCurrentTimeMillis();

                if((bookTimeMillis + (5*1000)) < currentTimeMillis) {
                    Log.e(TAG_VERBOSE, "ScheduleRecordRunnable outdated - bookTimeMillis:"+bookTimeMillis+" currentTimeMillis:"+currentTimeMillis );
                    return;
                }

                tryToStartRecord(mBundle);
            }
        }

        class AcquireRunnable implements Runnable {
            public void run(){
                acquireStrongWakeLock();
            }
        }

        class ReleaseRunnable implements Runnable {
            public void run(){
                releaseStrongWakeLock();
            }
        }

        class RecordRunnable implements Runnable {
            Bundle mBundle;
            RecordRunnable(Bundle b){
                mBundle = b;
            }

            @Override
            public void run(){
                startRecordWithBundle(mBundle);
            }
        }

        public void refreshSPIListAndDb(){
            // remove item in list
            int index = -1;
            for(int i=0;i<mSPis.size();i++){
                if(mSPis.get(i).bookTimeMillis == bookTimeMillis){
                    index = i;
                    break;
                }
            }
            Log.d(TAG2,"refreshSPIListAndDb remove "+index+" in list");

            if(DEBUG_VERBOSE) {
                Log.i(TAG2_VERBOSE, "refreshSPIListAndDb remove "+index+" in list");
            }

            if(index >= 0){
                SchRecItem2 item = mSPis.get(index).item;

                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE, "Call dbHelper to delete item :\""+item.getDisplayString()+"\"");
                }

                // remove in db
                mDbHelper.deleteItem(item,false);

                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE,"release wakelock");
                }

                // remove wakeup
                sendSchRecBroadcast(RM_REC_BROADCAST,
                    item.mStartTime.getTimeInMillis(),
                    item.mEndTime.getTimeInMillis());

                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE,"do remove index:"+index);
                }

                mSPis.remove(index);

                if(DEBUG_VERBOSE){
                    dumpMSPisList(TAG2_VERBOSE);
                }

            }else{
                if(DEBUG_VERBOSE) {
                    Log.e(TAG2_VERBOSE, "refreshSPIListAndDb index == -1 error");
                }
            }

        }

        public boolean tryToStartRecord(Bundle org){
            if(!FloatingWindowService2.bServiceIsAlive){
                Log.d(TAG2, "tryToStartRecord - start a invisible service");

                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE, "tryToStartRecord - start a invisible service  item: \""+item.getDisplayString()+"\"");
                }

                Bundle b = new Bundle();
                b.putBoolean(Keys.KEY_TOUCHABLE,false);
                int[] pos = {5000,5000,1,1};
                b.putIntArray(Keys.KEY_DIMENSION,pos);
                b.putBoolean(Keys.KEY_AUDIO_ON,false);
                b.putBoolean(Keys.KEY_VIDEO_ON,false);
                Intent serviceIntent = new Intent(
                        mContext, com.example.hdmirxdemo.FloatingWindowService2.class);
                serviceIntent.putExtra("rx",b);
                mContext.startService(serviceIntent);

                // post delay runnable
                RecordRunnable r = new RecordRunnable(org);
                mHandler.postDelayed(r,2000);
                return true;
            }else{
                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE, "Start record item: \""+item.getDisplayString()+"\"");
                }
               return startRecordWithBundle(org);
            }
        }

        public void acquireStrongWakeLock(){
            if(mStrongWakeLock==null){
                Log.d(TAG2,"acquireStrongWakeLock");

                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE, "acquireStrongWakeLock item: \""+item.getDisplayString()+"\"");
                }

                mStrongWakeLock = getPowerManager().newWakeLock(PowerManager.RTK_STRONG_WAKE_LOCK, "lock-"+(bookTimeMillis/1000));
                mPartialWakeLock = getPowerManager().newWakeLock(PowerManager.PARTIAL_WAKE_LOCK,"lock-"+(bookTimeMillis/1000));
                mStrongWakeLock.acquire();
                mPartialWakeLock.acquire();
            }
        }

        public void releaseStrongWakeLock(){
            if(mStrongWakeLock!=null){
                Log.d(TAG2,"releaseStrongWakeLock");
                if(DEBUG_VERBOSE) {
                    Log.i(TAG2_VERBOSE, "releaseStrongWakeLock item: \""+item.getDisplayString()+"\"");
                }
                mStrongWakeLock.release();
                mStrongWakeLock=null;
                mPartialWakeLock.release();
                mPartialWakeLock=null;
            }else{
                Log.d(TAG2, "releaseStrongWakeLock - do nothing");
            }
        }

        /*
        void bookTasks(Handler handler){
            Log.i(TAG2,"do nothing in bookTasks");
            long currentTime = getCurrentTimeMillis();
            long delay = item.mStartTime.getTimeInMillis()-WAKEUP_RESERVED_TIME_MS-currentTime;
            if(delay < 0){
                delay = 1;
            }
            // 1. acquire/release wakelock
            handler.postDelayed(mAcqRunnable,delay);
            long releaseDelay = item.mEndTime.getTimeInMillis()-currentTime;
            handler.postDelayed(mRlsRunnable,releaseDelay);
            // 2. post record Runnable
            long recordDelay = item.mStartTime.getTimeInMillis()-currentTime;
            handler.postDelayed(mRecRunnable,recordDelay);
        }
        */

        /*
        void removeTasks(Handler handler){
            Log.i(TAG2, "do nothing in removeTasks");
            releaseStrongWakeLock();
            handler.removeCallbacks(mAcqRunnable);
            handler.removeCallbacks(mRlsRunnable);
            handler.removeCallbacks(mRecRunnable);
        }
        */

        public boolean startRecordWithBundle(Bundle org){
            if(FloatingWindowService2.bServiceIsAlive){
                if(FloatingWindowService2.isRecording()){
                    showToast("ERROR: existing record instance found");
                    if(DEBUG_VERBOSE) {
                        Log.e(TAG2_VERBOSE, "startRecordWithBundle - ERROR - found existing reocrd instance - item: \""+item.getDisplayString()+"\"");
                    }
                    return false;
                }else{
                    Bundle arg = new Bundle();
                    // mode
                    arg.putInt(Keys.KEY_OUTPUT_MODE,Keys.OUTPUT_STORAGE);
                    // path
                    String path = org.getString(Keys.KEY_OUTPUT_PATH);
                    arg.putString(Keys.KEY_OUTPUT_PATH,path);
                    // format
                    int format = org.getInt(Keys.KEY_OUTPUT_FORMAT);
                    arg.putInt(Keys.KEY_OUTPUT_FORMAT,format);
                    // width, height
                    int width = org.getInt(Keys.KEY_RECORD_WIDTH);
                    int height = org.getInt(Keys.KEY_RECORD_HEIGHT);
                    arg.putInt(Keys.KEY_RECORD_WIDTH,width);
                    arg.putInt(Keys.KEY_RECORD_HEIGHT,height);
                    // durationMin
                    long endTimeMillis = org.getLong(Keys.KEY_RECORD_END_TIME_MILLIS,-1);
                    Log.d(TAG2, "tryToStartRecord - path:"+path+" width:"+width+" height:"+height+" endTimeMillis:"+endTimeMillis);

                    if(DEBUG_VERBOSE) {
                        Log.d(TAG2_VERBOSE, "tryToStartRecord - path:"+path+" item:\""+item.getDisplayString()+"\"");
                    }

                    arg.putLong(Keys.KEY_RECORD_END_TIME_MILLIS,endTimeMillis);
                    FloatingWindowService2.startRecord(arg);
                    releaseStrongWakeLock();
                    return true;
                }
            
            }else{
                if(DEBUG_VERBOSE){
                    Log.e(TAG2_VERBOSE, "service is not running item:\""+item.getDisplayString()+"\"");
                }
                releaseStrongWakeLock();
                return false;
            }
        }
    }

    private void dumpMSPisList(String tag){
        if(DEBUG_VERBOSE) {
            Log.i(tag,"\tDUMP mSPis size is :"+mSPis.size());

            for(int i=0;i<mSPis.size();i++){
                Log.i(tag, "\t\titem "+i+" : \""+mSPis.get(i).item.getDisplayString()+"\"");
            }
            Log.i(tag, "\tDUMP done");
        }
    }

    @Override
    public void onItemInserted(SchRecItem2 item){
        Utils.getInstance().init();
        SchRecItmPI spi = new SchRecItmPI(item);
        boolean checkRst = checkTimeValidate(spi);

        Log.d(TAG_VERBOSE,"onItemInserted item:\""+item.getDisplayString()+"\" time validate:"+checkRst);

        // add to list
        if(checkRst){
            mSPis.add(spi);
            // booking task uses Handler
            //spi.bookTasks(mHandler);

            if(DEBUG_VERBOSE){
                dumpMSPisList(TAG_VERBOSE);
            }

            sendSchRecBroadcast(ADD_REC_BROADCAST,
                item.mStartTime.getTimeInMillis(),
                item.mEndTime.getTimeInMillis());

        }
    }

    @Override
    public void onItemDeleted(SchRecItem2 item){
        int size = mSPis.size();
        int removedId=-1;
        for(int i=0;i<size;i++){
            if(item.equals(mSPis.get(i).item)){
                removedId = i;
                break;
            }
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "dumpMSPisList before remove");
            dumpMSPisList(TAG_VERBOSE);
        }

        Log.d(TAG_VERBOSE, "onItemDeleted - remove "+removedId);
        Utils.getInstance().init();
        if(removedId >= 0){
            //mSPis.get(removedId).pi.cancel();
            //mSPis.get(removedId).removeTasks(mHandler);
            mSPis.remove(removedId);

            sendSchRecBroadcast(RM_REC_BROADCAST,
                item.mStartTime.getTimeInMillis(),
                item.mEndTime.getTimeInMillis());
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "dumpMSPisList after remove");
            dumpMSPisList(TAG_VERBOSE);
        }

    }

    private PowerManager getPowerManager(){
        if(mPm == null){
            mPm = (PowerManager)mContext.getSystemService(Context.POWER_SERVICE);
        }
        return mPm;
    }

    /**
     * use broadcast to send schedule record info to SuspendHandler
     */
    private void sendSchRecBroadcast(String broadcast, long startTimeMillis, long endTimeMillis){
        Intent intent = new Intent();
        intent.setAction(broadcast);
        startTimeMillis = startTimeMillis - WAKEUP_RESERVED_TIME_MS;
        intent.putExtra(KEY_START_TIME_MILLIS, startTimeMillis);
        intent.putExtra(KEY_END_TIME_MILLIS, endTimeMillis);
        mContext.sendBroadcast(intent);
    }

    private long getCurrentTimeMillis(){
        Calendar calendar = new GregorianCalendar();
        Date trialtime = new Date();
        calendar.setTime(trialtime);
        long currentTimeSecMillis = calendar.getTimeInMillis();
        return currentTimeSecMillis;
    }

    private boolean checkTimeValidate(SchRecItmPI spi){

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "checkTimeValidate item:\""+spi.item.getDisplayString()+"\"");
        }

        long currentTimeSecMillis = getCurrentTimeMillis();
        long scheduledTimeSecMillis = spi.item.mStartTime.getTimeInMillis();

        long delaySecMillis = scheduledTimeSecMillis - currentTimeSecMillis;
        if(delaySecMillis<0){
            Log.e(TAG_VERBOSE, "checkTimeValidate but outdated item:\""+spi.item.getDisplayString()+"\"");
            return false;
        }

        Log.d(TAG, "checkTimeValidate "+delaySecMillis/1000 + " OK");
        return true;
    }

    private void showToast(String message) {
        Toast.makeText(mContext,message,Toast.LENGTH_SHORT).show();
    }

    public void initSPIListAndRegister(List<SchRecItem2> itemList){
        Log.d(TAG,"initSPIListAndRegister");
        //List<SchRecItem2> itemList = dbh.getAllItemsByOrder();
        for(int i=0;i<itemList.size();i++){
            SchRecItem2 item = itemList.get(i);

            if(DEBUG_VERBOSE) {
                Log.i(TAG_VERBOSE, "initSPIListAndRegister - item : \""+item.getDisplayString()+"\"");
            }

            //PendingIntent pi = createPendingIntent(item);
            SchRecItmPI spi = new SchRecItmPI(item);

            boolean checkRst = checkTimeValidate(spi);
            if(checkRst){
                mSPis.add(spi);
                //spi.bookTasks(mHandler);
                // pass all booked information
                sendSchRecBroadcast(ADD_REC_BROADCAST,
                    item.mStartTime.getTimeInMillis(),
                    item.mEndTime.getTimeInMillis());

            }
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "dump mSPis list after initSPIListAndRegister");
            dumpMSPisList(TAG_VERBOSE);
        }

    }

    public SchRecItmPI getLatestSchItem(){

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getLatestSchItem - start");
        }

        if(mSPis==null)
            return null;
        if(mSPis.size()==0)
            return null;

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getLatestSchItem - pass list check - dumpMSPisList first");
            dumpMSPisList(TAG_VERBOSE);
        }

        SchRecItmPI rst = mSPis.get(0);
        for(int i=0;i<mSPis.size();i++){
            SchRecItmPI item = mSPis.get(i);
            if(item.item.mStartTime.getTimeInMillis()< rst.item.mStartTime.getTimeInMillis()){
                rst = item;
            }
        }

        if(DEBUG_VERBOSE) {
            Log.i(TAG_VERBOSE, "getLatestSchItem - result : \""+rst.item.getDisplayString()+"\"");
        }

        return rst;
    }
}
