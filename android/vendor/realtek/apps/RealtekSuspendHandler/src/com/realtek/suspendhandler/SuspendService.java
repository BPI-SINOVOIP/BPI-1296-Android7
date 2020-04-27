package com.realtek.suspendhandler;

import android.app.Service;
import android.os.IBinder;
import android.os.Binder;
import android.os.SystemProperties;
import android.content.Intent;
//import android.util.Log;
import android.util.Slog;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Bundle;
import android.os.PowerManager;
import android.os.SystemClock;
import android.os.storage.IMountService;
import android.os.storage.VolumeInfo;
import android.os.storage.DiskInfo;
import android.os.storage.StorageManager;
import android.os.storage.StorageEventListener;
import android.os.ServiceManager;
import android.os.UserHandle;

import java.io.PrintWriter;
import java.io.IOException;
import java.io.FileNotFoundException;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.FileNotFoundException;
import java.io.IOException;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.app.SearchManager;

import java.util.GregorianCalendar;
import java.util.Calendar;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;

public class SuspendService extends Service {

    private static final String TAG = "RealtekSuspendService";
    // un-used
    //public static final String UNMOUNT_TIMEOUT_BROADCAST = "realtek.action.UNMOUNT_TIMEOUT";

    public static final int SCREEN_OFF_SUSPEND_ON    = 1;
    public static final int SCREEN_OFF_SUSPEND_OFF   = 2;

    public static final boolean HANDLE_BT = false;

    private final IBinder mBinder = new LocalBinder();

    private SuspendHandler mSuspendReceiver = null;
    private UnmountReceiver mUnmountReceiver = null;
    private UnmountTimeoutReceiver mTimeoutReceiver = null;
    private BluetoothHandler mBtHandler = null;

    private static final String TIMER_CONTROL_FILE="/sys/kernel/suspend/wakeup";
    private static final String TIMER_RESUME_DELAY_FILE="/sys/kernel/suspend/timer";

    private static final long WAKEUP_RESERVED_TIME_MS = 0; //45*1000; // 60 seconds

    /**
     * wakelock to protect suspend process
     */
    private PowerManager.WakeLock mWakeLock = null;

    /**
     * WakeLock to prevent system go to sleep.
     */
    private PowerManager.WakeLock mStrongWakeLock = null;

    private long mSuspendContext;

    private Handler mHandler = new Handler();

    // set to true when SCREEN_OFF is receive, cleared when:
    // - cleared when SCREEN_OFF is return and without unmount
    // - cleared when unmount is finished
    // - cleared when unmount is timeout -> in delayed post message handler
    public boolean mIsSuspendMode = false;
    public int mUnmountCounter = 0;
    public int mBtWaitLock = 0;

    public List<VolumeInfo> mSDCardList = null;
    public List<VolumeInfo> mUnmountedSDCardList = null;

    private StorageManager mStorageManager = null;

    public RealetkGCHelper mGCHelper = null;

    private final StorageEventListener mStorageListener = new StorageEventListener() {
        @Override
        public void onVolumeStateChanged(VolumeInfo vol, int oldState, int newState) {
            //Slog.d(TAG, "onVolumeStateChanged vol DiskInfo:"+vol.disk.sysPath);
        }

        @Override
        public void onDiskDestroyed(DiskInfo disk) {

            Slog.d(TAG, "onDiskDestroyed "+disk.sysPath);

            /*
            Iterator<VolumeInfo> i = mUnmountedSDCardList.iterator();
            while(i.hasNext()){
                VolumeInfo info = i.next();
                if(info.disk != null && info.disk.equals(disk)) {
                    Slog.d(TAG, "Volume: "+info.path+" has been removed");
                    i.remove();
                }
            }
            */
        }
    };

    public class LocalBinder extends Binder {
        SuspendService getService() {
            return SuspendService.this;
        }
    }

    class TimeoutRunnable implements Runnable{
        @Override
        public void run() {
            if(mIsSuspendMode){
                Slog.d(TAG,"TimeoutRunnable executed, release wake lock");
                mUnmountCounter = 0;
                mBtWaitLock = 0;
                checkAndRemoveWakeLock();
            }else{
                Slog.e(TAG, "TimeoutRunnable executed , run in none suspend period, do nothing");
            }
        }
    }

    class ReleaseWakeLockRunnable implements Runnable{
        public void run() {
            if(canReleaseWakeLock()){
                Slog.d(TAG, "Execute releaseWakeLock in delayRunnable");
                mIsSuspendMode = false;
                releaseWakeLock(true);
            }
        }
    }

    class ReleaseStrongWakeLockRunnable implements Runnable {
        public void run(){
            releaseStrongWakeLock();
        }
    }

    class WakeUpRunnable implements Runnable{
        public void run() {
            long suspendContext =  getKernelSuspendContext();
            if(suspendContext > mSuspendContext){
                Slog.d(TAG, "Execute WakeUp");
                PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
                long time = SystemClock.uptimeMillis();
                pm.wakeUp(time, "android.policy:KEY");

                // Search, do in SCREEN_ON
                /*
                if(runSearchAfterWakeup()){
                    long delay = 7500;
                    Slog.d(TAG, "launch search after "+delay+" ms");
                    mHandler.postDelayed(mSearchRunnable,delay);
                }
                */

            }else{
                mHandler.postDelayed(mWakeupRunnable,500);
            }
        }
    }

    class SearchRunnable implements Runnable {
        public void run(){
            Slog.d(TAG, "run search");
            SearchManager mSearchManager = (SearchManager) getSystemService(Context.SEARCH_SERVICE);
            Bundle args = new Bundle();
            args.putInt(Intent.EXTRA_ASSIST_INPUT_DEVICE_ID, 2 /*event.getDeviceId()*/);
            mSearchManager.launchLegacyAssist(null,UserHandle.USER_CURRENT,args);
        }
    }

    class GCRunnable implements Runnable {
        public void run(){
            if(mGCHelper != null) {
                mGCHelper.clearGarbageApps();
            }
            mHandler.postDelayed(mGCRunnable, 5000);
        }
    }

    private TimeoutRunnable mTimeoutRunnable = new TimeoutRunnable();
    private ReleaseWakeLockRunnable mReleaseWakeLockRunnable = new ReleaseWakeLockRunnable();
    private ReleaseStrongWakeLockRunnable mReleaseStrongWakeLockRunnable = new ReleaseStrongWakeLockRunnable();
    private WakeUpRunnable mWakeupRunnable = new WakeUpRunnable();
    private SearchRunnable mSearchRunnable = new SearchRunnable();
    private GCRunnable mGCRunnable = new GCRunnable();

    @Override
    public IBinder onBind (Intent intent) {
        return mBinder;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        Slog.d(TAG, "service onCreate");
        acquireWakeLock();
    }

    @Override
    public int onStartCommand (Intent intent, int flags, int startId) {
        Slog.d(TAG , "service onStartCommand");

        mStorageManager = (StorageManager) getSystemService(Context.STORAGE_SERVICE);
        mStorageManager.registerListener(mStorageListener);

        mSDCardList = new ArrayList<VolumeInfo>();
        mUnmountedSDCardList = new ArrayList<VolumeInfo>();

        // for first time boot up, move to onCreate
        // acquireWakeLock();
        
        //Intent piIntent = new Intent(UNMOUNT_TIMEOUT_BROADCAST);
        //mPi = PendingIntent.getBroadcast(this, 1, piIntent, PendingIntent.FLAG_UPDATE_CURRENT);

        IntentFilter filter = new IntentFilter();

        mSuspendReceiver = new SuspendHandler(this);
        // - when receive SCREEN_OFF, unmount USB and then wait UNMOUNTED event to release wake_lock
        filter.addAction(Intent.ACTION_SCREEN_OFF);
        //  "rtk.intent.action.POWER_SUSPEND" is for shutdown mode
        /* handle android standard shutdown broadcast */
        filter.addAction(Intent.ACTION_SHUTDOWN);
        //filter.addAction("rtk.intent.action.POWER_SUSPEND");
        // - when receive SCREEN_ON, acquire wake_lock, SuspendService has mechanism to protect wake_lock been over-acquired.
        filter.addAction(Intent.ACTION_SCREEN_ON);
        // make this receiver called at final.
        filter.setPriority(IntentFilter.SYSTEM_LOW_PRIORITY+1);
        registerReceiver(mSuspendReceiver, filter /*, null, mHandler */);

        // register unmounted broadcast receiver
        mUnmountReceiver = new UnmountReceiver(this);
        filter = new IntentFilter();
        filter.addAction(Intent.ACTION_MEDIA_UNMOUNTED);
        filter.addDataScheme("file");
        registerReceiver(mUnmountReceiver, filter /*, null, mHandler */);

        /*
        // cause PendingIntent would use wake_lock to block suspend flow, so do not use it
        mTimeoutReceiver = new UnmountTimeoutReceiver(this);
        filter = new IntentFilter();
        filter.addAction(UNMOUNT_TIMEOUT_BROADCAST);
        registerReceiver(mTimeoutReceiver, filter, null, mHandler);
        */

        mBtHandler = new BluetoothHandler(this,null);
        mGCHelper = new RealetkGCHelper(this);

        int gcEnabled = SystemProperties.getInt("persist.rtk.gc.service", 0);
        if(gcEnabled > 0){
            mHandler.postDelayed(mGCRunnable,5000);   // 5 sec later
        }
        // if Suspend service is killed, it would restart itself again
        return Service.START_STICKY;
        //return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public void onDestroy() {
        Slog.d(TAG, "service destroy");
        releaseWakeLock(false);
        super.onDestroy();
    }

    public BluetoothHandler getBtHandler() {
        return mBtHandler;
    }

    public void acquireStrongWakeLock(){
        if(mStrongWakeLock==null){
            Slog.d(TAG,"acquire strong wake lock");
            PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
            mStrongWakeLock = pm.newWakeLock(PowerManager.RTK_STRONG_WAKE_LOCK, "Realtek.Suspend");
            mStrongWakeLock.acquire();
        }else{
            Slog.e(TAG,"acquireStrongWakeLock - wakelock already hold");
        }
    }

    public void releaseStrongWakeLock(){
        if(mStrongWakeLock != null){
            Slog.d(TAG,"release strong wakelock");
            mStrongWakeLock.release();
            mStrongWakeLock=null;
        }else{
            Slog.e(TAG,"release strong wakelock , but no wakelock is hold");
        }
    }

    public void acquireWakeLock(){
        if(mWakeLock==null){
            Slog.d(TAG, "acquire wake lock");
            PowerManager pm = (PowerManager)getSystemService(Context.POWER_SERVICE);
            mWakeLock = pm.newWakeLock(PowerManager.RTK_WAKE_LOCK, "Realtek.Suspend");
            mWakeLock.acquire();
            cancelWakeUpTask();
        }else{
            Slog.d(TAG , "acquire wake lock failed - wake_lock already exists "+ mWakeLock);
        }
    }

    /**
     * API to let SCREEN_OFF receiver to post a delayed message
     */
    public void postDelayedMessage(){
        mHandler.postDelayed(mTimeoutRunnable, 7*1000);
    }

    public void removeDelayMessage(){
        mHandler.removeCallbacks(mTimeoutRunnable);
    }

    private boolean canReleaseWakeLock() {
        if(mUnmountCounter <= 0 && mBtWaitLock <= 0) {
            Slog.d(TAG, "pass release wake lock checking");
            return true;
        }else{
            return false;
        }
    }

    private boolean checkAndRemoveWakeLock() {
        if(canReleaseWakeLock()) {

            removeDelayMessage();
            releaseWakeLock(true);
            mIsSuspendMode = false;

            return true;
        }
        return false;
    }

    private String millisToTimeString(long timeMillis){
        Calendar c = new GregorianCalendar();
        c.setTimeInMillis(timeMillis);
        SimpleDateFormat format1 = new SimpleDateFormat("yyyy.MM.dd 'at' HH:mm:ss");
        String formatted = format1.format(c.getTime());
        return formatted;
    }

    private void configTimer(){

        Slog.i(TAG, "Exec configTimer");
        long wakeupTimeMillis = WakeUpBroadcastReceiver.getWakeUpTime();

        if(wakeupTimeMillis<0){
            Slog.d(TAG,"no auto resume request");
            return;
        }

        long currentTimeMillis = getCurrentTimeMillis();

        Slog.d(TAG, "configTimer wakeTime:"+millisToTimeString(wakeupTimeMillis)+" currentTime:"+millisToTimeString(currentTimeMillis));

        long delayMillis = wakeupTimeMillis - WAKEUP_RESERVED_TIME_MS - currentTimeMillis;

        Slog.d(TAG, "call configTimer delayMillis:"+delayMillis+" wakeupTimeMillis:"+wakeupTimeMillis);

        if(delayMillis>0){
            int delaySec = (int) (delayMillis/1000);
            int sec = delaySec%60;
            int min = delaySec/60;
            Slog.d(TAG, "configTimer with delay "+ min+":"+sec);
            setWakeUpTimerWithChecking(true);
            setAutoResumeTimeSecWithChecking(delaySec);
        }else{
            Slog.e(TAG,"delayMillis < 0 : ["+delayMillis+"]");
        }
    }

    private long getCurrentTimeMillis(){
        Calendar calendar = new GregorianCalendar();
        Date trialtime = new Date();
        calendar.setTime(trialtime);
        return calendar.getTimeInMillis();
    }

    public void configTimerResumeOnShutdown() {
        Slog.d(TAG, "configTimerResumeOnShutdown");
        setWakeUpTimerWithChecking(false);
        configTimer();
    }

    private void releaseWakeLock(boolean configAutoResume){

        // BUG_FIX: one should always set timer
        setWakeUpTimerWithChecking(false);
        //SystemClock.sleep(50);
        if(configAutoResume){
            configTimer();
            //SystemClock.sleep(50);
        }

        if(mWakeLock!=null){
            Slog.d(TAG, "release wake lock");
            mWakeLock.release();
            mWakeLock = null;
            postWakeUpTask();
        }else{
            Slog.e(TAG, "release wake lock failed - wake_lock is null");
        }

    }

    public void releaseStrongWakeLockDelay(int delayMs){
        mHandler.postDelayed(mReleaseStrongWakeLockRunnable,delayMs);
    }

    public void checkAndReleaseWakeLockDelay(int delayMs){
        Slog.d(TAG, "releaseWakeLockDelay "+delayMs);

        mHandler.postDelayed(mReleaseWakeLockRunnable, delayMs);
    }

    public void dumpWakeLockStatus(){
        Slog.d(TAG, "current wake_lock status: "+mWakeLock);
    }

    public static long getCurrentThreadID(){
        long threadId = Thread.currentThread().getId();
        return threadId;
    }

    private String getWakeUpTimeOpMode(){
        String mode = SystemProperties.get("persist.rtk.wakeup.mode","sqa");

        Slog.d(TAG, "getWakeUpTimeOpMode :"+mode);

        return mode;
    }

    public String getPlatformSuspendMode(){

        if(xenEnabled()){
            return "suspend";
        }

        String mode = SystemProperties.get("persist.rtk.suspend.mode","standby");
        Slog.d(TAG, "persist.rtk.suspend.mode:"+mode);
        return mode;
    }

    private boolean xenEnabled(){
        String mode = SystemProperties.get("ro.boot.storage","emmc");

        Slog.d(TAG, "ro.boot.storage:"+mode);

        if(mode.equals("xen")){
            return true;
        }else{
            return false;
        }
    }

    public int getPlatformScreenOffMode(){

        /*
        if(xenEnabled()){
            return SCREEN_OFF_SUSPEND_OFF;
        }
        */

        // check if there is strong wake lock holding
        try{
            BufferedReader br = new BufferedReader(new FileReader("/sys/power/wake_lock"));
            String line = br.readLine().trim();
            if(line.contains("PowerManagerService.RtkStrongLocks")){
                Slog.d(TAG, "detect strong wake lock, act as SCREEN_OFF_SUSPEND_OFF");
                return SCREEN_OFF_SUSPEND_OFF;
            }
        }catch (FileNotFoundException e){
            e.printStackTrace();
        }catch (IOException e){
            e.printStackTrace();
        }

        int mode = SystemProperties.getInt("persist.rtk.screenoff.suspend",0);
        if(mode == 1){
            Slog.d(TAG, "ScreenOff mode: SCREEN_OFF_SUSPEND_ON");
            return SCREEN_OFF_SUSPEND_ON;
        }else{
            Slog.d(TAG, "ScreenOff mode: SCREEN_OFF_SUSPEND_OFF");
            return SCREEN_OFF_SUSPEND_OFF;
        }
    }

    private boolean wakeUpTimerEnabled(){

        String opMode = getWakeUpTimeOpMode();
        if(!opMode.equals("normal")){
            return false;
        }

        try{
            BufferedReader br = new BufferedReader(new FileReader(TIMER_CONTROL_FILE));
            String line;
            while((line=br.readLine()) != null){
                Slog.d(TAG, "wakeUpTimerEnabled : "+line);
                if(line.contains("timer")){
                    if(line.contains("*")){
                        br.close();
                        return true;
                    }
                }
            }
            br.close();
            return false;
        } catch (FileNotFoundException e){
            Slog.e(TAG, "Read FileNotFoundException "+TIMER_CONTROL_FILE);
            return false;
        } catch (IOException e) {
            Slog.e(TAG, "Read IOException "+TIMER_CONTROL_FILE);
            return false;
        }
    }

    private boolean checkTimerDelayTimeSec(int seconds){
        try{
            BufferedReader br = new BufferedReader(new FileReader(TIMER_RESUME_DELAY_FILE));
            String line;
            boolean rst = false;
            while((line=br.readLine()) != null){
                Slog.d(TAG, "checkTimerDelayTimeSec : "+line);
                if(line.contains(""+seconds)){
                    rst = true;
                    break;
                }
            }
            br.close();

            Slog.d(TAG, "checkTimerDelayTimeSec "+seconds+" rst:"+rst);

            return rst;
        } catch (FileNotFoundException e){
            Slog.e(TAG, "checkTimerDelayTimeSec Read FileNotFoundException "+TIMER_CONTROL_FILE);
            return false;
        } catch (IOException e) {
            Slog.e(TAG, "checkTimerDelayTimeSec Read IOException "+TIMER_CONTROL_FILE);
            return false;
        }
    }

    private void dumpTimerDelayTimeSec(){
        try{
            BufferedReader br = new BufferedReader(new FileReader(TIMER_RESUME_DELAY_FILE));
            String line;
            while((line=br.readLine()) != null){
                Slog.d(TAG, "dumpTimerDelayTimeSec : "+line);
            }
            br.close();
        } catch (FileNotFoundException e){
            Slog.e(TAG, "Read FileNotFoundException "+TIMER_CONTROL_FILE);
        } catch (IOException e) {
            Slog.e(TAG, "Read IOException "+TIMER_CONTROL_FILE);
        }
    }

    private void setAutoResumeTimeSecWithChecking(int seconds){

        String opMode = getWakeUpTimeOpMode();
        if(!opMode.equals("normal")){
            Slog.d(TAG, "setAutoResumeTimeSecWithChecking break in SQA mode");
            return;
        }

        setAutoResumeTimeSec(seconds);
        while(!checkTimerDelayTimeSec(seconds)){
            SystemClock.sleep(50);
            setAutoResumeTimeSec(seconds);
        }
    }

    private void setAutoResumeTimeSec(int seconds){

        Slog.d(TAG,"setAutoResumeTimeSec "+seconds);

        try{
            BufferedWriter bw = new BufferedWriter(new FileWriter(TIMER_RESUME_DELAY_FILE));
            bw.write(""+seconds);
            bw.close();
        } catch (FileNotFoundException e){
            Slog.e(TAG, "Write FileNotFoundException "+TIMER_RESUME_DELAY_FILE);
        } catch (IOException e) {
            Slog.e(TAG, "Write IOException "+TIMER_RESUME_DELAY_FILE);
        }
    }

    public void setWakeUpTimerWithChecking(boolean enable){

        String opMode = getWakeUpTimeOpMode();
        if(!opMode.equals("normal")){
            Slog.d(TAG, "setWakeUpTimerWithChecking break in SQA mode");
            return;
        }

        Slog.d(TAG, "setWakeUpTimerWithChecking enable:"+enable);

        // program first time
        setWakeUpTimer(enable);
        // get result
        boolean rst = wakeUpTimerEnabled();

        while(rst != enable){
            Slog.d(TAG, "setWakeUpTimerWithChecking [retry] "+enable+" rst:"+rst);
            SystemClock.sleep(50);
            setWakeUpTimer(enable);
            rst = wakeUpTimerEnabled();
        }
    }

    private void setWakeUpTimer(boolean enable){
        boolean en = wakeUpTimerEnabled();
        if(enable != en){
            Slog.d(TAG, "write to "+TIMER_CONTROL_FILE);
            // write
            try{
                BufferedWriter bw = new BufferedWriter(new FileWriter(TIMER_CONTROL_FILE));
                bw.write("timer");
                bw.close();
            } catch (FileNotFoundException e){
                Slog.e(TAG, "Write FileNotFoundException "+TIMER_CONTROL_FILE);
            } catch (IOException e) {
                Slog.e(TAG, "Write IOException "+TIMER_CONTROL_FILE);
            }
        }

        boolean en2 = wakeUpTimerEnabled();
        Slog.d(TAG, "wakeUpTimerEnabled after set "+en2);
    }

    public long getKernelSuspendContext(){

        // if system is configured as "screen off no suspend", then context is meanless.
        //if(getPlatformScreenOffMode() == SCREEN_OFF_SUSPEND_OFF)
        //    return -1;

        try {
            BufferedReader br = new BufferedReader(new FileReader("/sys/kernel/suspend/context"));
            String line = br.readLine().trim();
            Slog.d(TAG, "getSuspendContext line "+line);
            br.close();
            return Long.parseLong(line);
        } catch (FileNotFoundException e){
            Slog.e(TAG, "FileNotFoundException /sys/kernel/suspend/context");
            return -1;
        } catch (IOException e) {
            Slog.e(TAG, "IOException /sys/kernel/suspend/context");
            return -1;
        } catch (NumberFormatException e) {
            Slog.e(TAG, "NumberFormatException /sys/kernel/suspend/context");
            return -1;
        }
    }

    public void setSuspendContext(long value){
        mSuspendContext = value;
    }

    public void postWakeUpTask(){
        mHandler.postDelayed(mWakeupRunnable,500);
    }

    public void cancelWakeUpTask(){
        mHandler.removeCallbacks(mWakeupRunnable);
    }

    public void remountSDCardsbyVolumeId() {
        Slog.d(TAG, "remountSDCardsbyVolumeId");
        for(VolumeInfo volume:mUnmountedSDCardList){
            String volumeId = volume.getId();
            try {
                mStorageManager.mount(volumeId);
            } catch(Exception e) {
                e.printStackTrace();
            }
        }
        mUnmountedSDCardList.clear();
    }

    public void remountSDCards(){

        Slog.d(TAG, "remountSDCards");
        IBinder service = ServiceManager.getService("mount");
        if (service != null) {

            IMountService mountService = IMountService.Stub.asInterface(service);
            for(VolumeInfo volume:mUnmountedSDCardList){

                String path = volume.path;

                Slog.d(TAG, "remount : "+path);
                try {
                    mountService.mountVolume(path);
                } catch(android.os.RemoteException e) {
                    Slog.e(TAG, "remount "+path+" failed");
                } catch (IllegalArgumentException e) {
                    Slog.e(TAG, "SDCard "+path+" not exists");
                } catch (IllegalStateException e) {
                    Slog.e(TAG, "SDCard "+path+" not exists [IllegalStateException]");
                }
                Slog.d(TAG, "remount complete");
            }

            // clear the list
            mUnmountedSDCardList.clear();

        }
    }

    public boolean runSearchAfterWakeup(){
        try{
            BufferedReader br = new BufferedReader(new FileReader("/sys/kernel/suspend/resume_state"));
            String line = br.readLine().trim();
            if(line.contains("gpio") && line.contains("135")){
                Slog.d(TAG, "need run search after voice wakeup");
                return true;
            }
        }catch (FileNotFoundException e){
            e.printStackTrace();
            return false;
        }catch (IOException e){
            e.printStackTrace();
            return false;
        }

        return false;
    }

    public void runSearch(long delay) {
        Slog.d(TAG, "run search after "+delay+" ms");
        mHandler.postDelayed(mSearchRunnable,delay);
    }

    public void cancelPendingSearch() {
        mHandler.removeCallbacks(mSearchRunnable);
    }

}
