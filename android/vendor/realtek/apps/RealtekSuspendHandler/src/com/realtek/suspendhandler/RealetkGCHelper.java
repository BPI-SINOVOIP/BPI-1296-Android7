package com.realtek.suspendhandler;

import android.content.Intent;
import android.content.Context;
import android.content.pm.PackageManager;
import android.content.pm.ActivityInfo;
import android.content.pm.ResolveInfo;
import android.content.ComponentName;
import android.content.ComponentCallbacks2;
import android.content.res.Configuration;
import android.app.ActivityManager;
import android.app.ActivityManager.RunningAppProcessInfo;

import android.util.Slog;

import java.util.ArrayList;
import java.util.List;

public class RealetkGCHelper implements ComponentCallbacks2 {

    private static final String TAG = "RealtekSuspend-GC";
    private static final int MAX_TASKS = 50;

    private Context mContext;

    private PackageManager mPm = null;
    private ActivityManager mAm = null;

    private long mAvailMem = 0;
    private List<SimpleProcessInfo> mProcessList;
    private ActivityInfo mHomeInfo = null;

    private static final long MB = (1024*1024);

    /**
     * A simple class to keep process info
     */
    class SimpleProcessInfo {

        String mName;
        int mImportance;

        int mPid;
        int mPss;

        RunningAppProcessInfo mRawInfo;
        android.os.Debug.MemoryInfo mMemInfo;

        SimpleProcessInfo(
            RunningAppProcessInfo info,
            android.os.Debug.MemoryInfo memInfo) {

            mRawInfo = info;
            mMemInfo = memInfo;

            mName = mRawInfo.processName;
            mPid = mRawInfo.pid;
            mImportance = info.importance;
            mPss = Integer.parseInt(mMemInfo.getMemoryStat("summary.total-pss"));
        }

        public String toString() {
            return "Process "+mName+" ("+mImportance+") pss:"+mPss+"kB";
        }
    }

    public RealetkGCHelper(Context context) {

        mContext = context;

        mPm = mContext.getPackageManager();
        mAm = (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);
        mProcessList = new ArrayList<SimpleProcessInfo>();
    }

    @Override
    public void onTrimMemory (int level) {
        Slog.d(TAG, "onTrimMemory "+level);
    }

    @Override
    public void onLowMemory () {
        Slog.d(TAG, "onLowMemory");
    }

    @Override
    public void onConfigurationChanged (Configuration newConfig) {
        Slog.d(TAG, "onConfigurationChanged");
    }

    public synchronized void collectInfo() {
        mAvailMem = getSystemMemoryInfo();
        constructRunningProcessList();
        getHomeProcessInfo();
        //dumpProcessList();
    }

    public void dumpProcessList() {
        Slog.d(TAG, "dumpProcessList:");
        for(int i=0;i<mProcessList.size();i++) {
            Slog.d(TAG, " "+mProcessList.get(i));
        }
    }

    private void constructRunningProcessList() {
        mProcessList.clear();

        final List<RunningAppProcessInfo> processList =
            mAm.getRunningAppProcesses();

        int[] pidArray = new int[processList.size()];

        for(int i=0;i<processList.size();i++) {
            pidArray[i] = processList.get(i).pid;
        }

        android.os.Debug.MemoryInfo[] memInfos =
            mAm.getProcessMemoryInfo(pidArray);

        for(int i=0;i<processList.size();i++) {
            RunningAppProcessInfo p = processList.get(i);
            mProcessList.add( new SimpleProcessInfo(p, memInfos[i]) );
        }
    }

    public void clearGarbageApps() {
        collectInfo();
        if(mHomeInfo == null) {
            Slog.e(TAG, "can not find homeInfo");
            return;
        }

        Slog.d(TAG, "- availMem "+getSystemMemoryInfo()/MB+"MB");

        for(int i=0;i<mProcessList.size();i++) {
            String name = mProcessList.get(i).mName;
            if(isCurrentHomeActivity(name,mHomeInfo)){
                Slog.d(TAG, "Ignore process - "+name);
            }else{
                if(mProcessList.get(i).mImportance == 400){
                    Slog.d(TAG, "\t forceStopPackage - " + mProcessList.get(i));
                    //mAm.killBackgroundProcesses(name);
                    mAm.forceStopPackage(name);
                }else{
                    Slog.d(TAG, "\t!forceStopPackage - " + mProcessList.get(i));
                }
            }
        }

        Slog.d(TAG, "+ availMem "+getSystemMemoryInfo()/MB+"MB");
    }

    private String getRunningAppProcessesInfo(RunningAppProcessInfo process) {
        String rst = "";
        rst = rst + "Name:"+process.processName+" ";
        rst = rst + getProcessImportanceString(process.importance)+" ";
        return rst;
    }

    private String getProcessImportanceString(int importance) {
        switch(importance) {
            case RunningAppProcessInfo.IMPORTANCE_FOREGROUND:
                return "IMPORTANCE_FOREGROUND";
            case RunningAppProcessInfo.IMPORTANCE_FOREGROUND_SERVICE:
                return "IMPORTANCE_FOREGROUND_SERVICE";
            case RunningAppProcessInfo.IMPORTANCE_TOP_SLEEPING:
                return "IMPORTANCE_TOP_SLEEPING";
            case RunningAppProcessInfo.IMPORTANCE_VISIBLE:
                return "IMPORTANCE_VISIBLE";
            case RunningAppProcessInfo.IMPORTANCE_PERCEPTIBLE:
                return "IMPORTANCE_PERCEPTIBLE";
            case RunningAppProcessInfo.IMPORTANCE_SERVICE:
                return "IMPORTANCE_SERVICE";
            case 400: //RunningAppProcessInfo.IMPORTANCE_CACHED:
                return "IMPORTANCE_CACHED";
            case RunningAppProcessInfo.IMPORTANCE_GONE:
                return "IMPORTANCE_GONE";
            default:
                return "IMPORTANCE_NA ("+importance+")";
        }
    }

    private long getSystemMemoryInfo() {
        ActivityManager.MemoryInfo memInfo = new ActivityManager.MemoryInfo();
        mAm.getMemoryInfo(memInfo);
        //Slog.d(TAG, "Available Memory : "+memInfo.availMem);
        return memInfo.availMem;
    }

    private void getRunningTasksInfo() {
        int maxTasks = 30;
        List<ActivityManager.RunningTaskInfo> runningTasks = mAm.getRunningTasks(maxTasks);

        for(int i=0;i<runningTasks.size();i++) {
            ActivityManager.RunningTaskInfo info = runningTasks.get(i);
            Slog.d(TAG, "RunningTask : "+ info.baseActivity.getPackageName() + "id "+info.id);
        }

    }

    private void getHomeProcessInfo() {
        mHomeInfo= new Intent(Intent.ACTION_MAIN).
                addCategory(Intent.CATEGORY_HOME).
                resolveActivityInfo(mPm, 0);
    }

    private boolean isCurrentHomeActivity(String packageName, ActivityInfo homeInfo) {
        return homeInfo != null && homeInfo.packageName.equals(packageName);
    }

}
