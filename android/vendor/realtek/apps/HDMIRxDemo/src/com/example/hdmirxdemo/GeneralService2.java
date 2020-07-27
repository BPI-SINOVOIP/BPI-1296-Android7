package com.example.hdmirxdemo;

import android.app.Service;
import android.content.Context;
import android.os.IBinder;
import android.os.Binder;
import android.os.Handler;
import android.os.AsyncTask;
import android.os.SystemClock;
import android.app.Dialog;
import android.content.DialogInterface;

import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.KeyEvent;
import android.view.Window;
import android.widget.TextView;
import android.view.WindowManager;
import android.view.LayoutInflater;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;
import java.util.List;
import java.util.ArrayList;
import java.text.SimpleDateFormat;

public class GeneralService2 {

    private static final String TAG_VERBOSE = "GeneralService2-VERBOSE";

    private static final String TAG = "GeneralService2-suspend";

    private static final boolean DEBUG_VERBOSE = Keys.GENERAL_SERVICE_DEBUG_VERBOSE;

    private static final String WARN_MSG_1 =
        "Heavy loading playback detected.\nIt might effect record quality.\nPlease stop record or playback instance.";
    private static final String WARN_MSG_2 =
        "Record is starting in 15 seconds,\nplease stop existing playback to get better record quality.";

    public static final Handler mHandler = new Handler();

    public boolean running = true;
    private Context mContext;

    public static GeneralService2 sInstance = null;
    private SchRecCheckTask mTask;

    private List<VideoStatusInfo> mVideoStatusList;
    private Dialog mWarningDialog;

    class WarningDialogRunnable implements Runnable{
        String mMessage;
        WarningDialogRunnable(String msg){
            mMessage = msg;
        }
        public void run(){
            showWarningDialog(mMessage);
        }
    }

    public class VideoStatusInfo{
        String mime;
        int width;
        int height;
        int dimension;
        int fps;

        public VideoStatusInfo(String m, int w, int h, int f){
            mime = m;
            width = w;
            height = h;
            fps = f;
            dimension = w*h;
        }
        public boolean isH264(){
            if(mime.contains("264")){
                return true;
            }else{
                return false;
            }
        }

        public boolean isHeavyInstance(){
            // TODO
            boolean b264 = isH264();
            if(b264 && dimension>(2000*2000)){
                return true;
            }
            return false;
        }

        public boolean equals(Object obj){
            if(obj==null) return false;

            if(obj instanceof VideoStatusInfo){
                VideoStatusInfo info = (VideoStatusInfo)obj;
                if(info.mime.equals(mime) && info.width == width && info.height == height && info.fps == fps){
                    return true;
                }
            }
            return false;
        }
    }

    public GeneralService2(Context c){
        long tid = Thread.currentThread().getId();
        Log.d(TAG,"create GeneralService2 [tid]="+tid);
        mContext = c;

        mVideoStatusList = new ArrayList<VideoStatusInfo>();

        new SchRecCheckTask().executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    public synchronized static GeneralService2 getInstance(Context c){
        if(sInstance==null){
            sInstance = new GeneralService2(c);
        }
        return sInstance;
    }

    private String millisToTimeString(long timeMillis){
        Calendar c = new GregorianCalendar();
        c.setTimeInMillis(timeMillis);
        SimpleDateFormat format1 = new SimpleDateFormat("yyyy.MM.dd '-' HH:mm:ss");
        String formatted = format1.format(c.getTime());
        return formatted;
    }

    private long getCurrentTimeMillis(){
        Calendar calendar = new GregorianCalendar();
        Date trialtime = new Date();
        calendar.setTime(trialtime);
        long currentTimeSecMillis = calendar.getTimeInMillis();
        return currentTimeSecMillis;
    }

    class SchRecCheckTask extends AsyncTask<Void, Void, Boolean> {
        @Override
        protected Boolean doInBackground(Void... voids){
            while(running){
                SystemClock.sleep(1000);
                long nowTime = getCurrentTimeMillis();
                long nowTimeSec = nowTime/1000;
                //if(nowTime%5 ==0){
                //Log.i(TAG,"service alive");
                //}

                DbListenerImpl.SchRecItmPI item = HDMIDbHelper.getInstance(mContext).getLatestSchItem();
                if(item!=null){

                    long bookTime = item.bookTimeMillis;
                    long lockTime = item.lockTimeMillis;
                    long detectTime = item.detectH264TimeMillis;
                    // hold strong wakelock
                    if(nowTime>=lockTime){

                        if(DEBUG_VERBOSE) {
                            Log.i(TAG_VERBOSE, "start to acquire wakelock");
                        }

                        item.acquireStrongWakeLock();
                    }
                    // start record, refresh booking list, release strong wakelock
                    if(nowTime>=bookTime){

                        if(DEBUG_VERBOSE){
                            Log.i(TAG_VERBOSE, "start to record - start");
                        }

                        //item.tryToStartRecord(item.mBundle);
                        mHandler.post(item.mRecRunnable);
                        //item.refreshSPIListAndDb();

                        if(DEBUG_VERBOSE){
                            Log.i(TAG_VERBOSE, "start to record - complete");
                        }
                    }
                    if(nowTime>=detectTime){
                        if(hasHeavyLoadingInstance()){
                            if(!item.bWarningDisplayed){
                                item.bWarningDisplayed = true;

                                if(DEBUG_VERBOSE) {
                                    Log.i(TAG_VERBOSE, "show Heavy playback warning dialog");
                                }

                                mHandler.post(new WarningDialogRunnable(WARN_MSG_2));
                            }
                        }
                    }
                }
            }

            return true;
        }

        @Override
        protected void onPostExecute(Boolean result){
            Log.d(TAG,"Task terminated");
        }
    }

    public void insertVideoStatus(String mime, int width, int height, int fps){
        Log.d(TAG, "insertVideoStatus "+mime+" "+width+" "+height+" "+fps);
        mVideoStatusList.add(new VideoStatusInfo(mime,width,height,fps));

        // then, pop up warning dialog if necessary.
        if(hasHeavyLoadingInstance()){
            if(FloatingWindowService2.bServiceIsAlive){
                if(FloatingWindowService2.isRecording()){
                    FloatingWindowViewGroup3 ctrl = FloatingWindowService2.getViewGroupControl();
                    if(ctrl != null){
                        ctrl.showWarningDialog(WARN_MSG_1);
                    }
                }
            }
        }
    }

    public void deleteVideoStatus(String mime, int width, int height, int fps){
        Log.d(TAG, "deleteVideoStatus "+mime+" "+width+" "+height+" "+fps);
        int index = -1;
        VideoStatusInfo dst = new VideoStatusInfo(mime,width,height,fps);

        for(int i=0;i<mVideoStatusList.size();i++){
            if(mVideoStatusList.get(i).equals(dst)){
                index = i;
                break;
            }
        }

        if(index>=0){
            Log.d(TAG, "deleteVideoStatus remove index:"+index);
            mVideoStatusList.remove(index);
        }

    }

    public boolean hasHeavyLoadingInstance(){
        for(int i=0;i<mVideoStatusList.size();i++){
            VideoStatusInfo info = mVideoStatusList.get(i);
            if(info.isHeavyInstance()){
                return true;
            }
        }
        return false;
    }

    private void dismissWarningDialog(){
        Log.d(TAG, "dismissWarningDialog");
        if(mWarningDialog!=null){
            mWarningDialog.dismiss();
            mWarningDialog = null;
        }
    }

    public void showWarningDialog(String message){
        Log.d(TAG, "showWarningDialog "+message);
        dismissWarningDialog();
        mWarningDialog = new Dialog(mContext);
        mWarningDialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        mWarningDialog.setContentView(R.layout.layout_warning_dialog);
        Window w = mWarningDialog.getWindow();
        w.getAttributes().type = WindowManager.LayoutParams.TYPE_TOAST;

        TextView warningTextView = (TextView) mWarningDialog.findViewById(R.id.warning_textview);
        TextView confirmTextView = (TextView) mWarningDialog.findViewById(R.id.ok_textview);

        confirmTextView.setText("Press \"OK\" button to close dialog");
        warningTextView.setText(message);

        // support on click event
        View okArea = mWarningDialog.findViewById(R.id.ok_area);
        okArea.setOnClickListener(new View.OnClickListener(){
            public void onClick (View v){
                dismissWarningDialog();
            }
        });

        mWarningDialog.setOnKeyListener( new DialogInterface.OnKeyListener() {
            public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event){
                if(keyCode == KeyEvent.KEYCODE_DPAD_CENTER || keyCode == KeyEvent.KEYCODE_ENTER || keyCode == KeyEvent.KEYCODE_BACK){
                    if(event.getAction() == KeyEvent.ACTION_UP){
                        Log.d(TAG,"keypress dismiss mWarningDialog");
                        mWarningDialog.dismiss();
                        mWarningDialog = null;
                    }
                }
                return true;
            }
        });

        mWarningDialog.show();
        Log.d(TAG, "showWarningDialog - show msg:\""+message+"\"");
    }
}
