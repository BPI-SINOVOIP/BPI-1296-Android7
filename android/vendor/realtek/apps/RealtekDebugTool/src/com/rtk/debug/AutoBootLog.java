package com.rtk.debug;

import android.content.Context;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.util.Utils;

/**
 * Created by archerho on 2017/4/6.
 */
public class AutoBootLog implements IDump {
    private static final boolean DEBUG = true;
    private static final String TAG = AutoBootLog.class.getSimpleName();
    public static final String KEY_DUMP_PATH = "rtk.debug.dump_path";
    private static AutoBootLog mIns;
    Context mCtx;

    private AutoBootLog(Context ctx) {
        mCtx = ctx;
    }

    public static AutoBootLog getInstance(Context ctx){
        if(mIns==null){
            mIns = new AutoBootLog(ctx);
        }
        return mIns;
    }

    @Override
    public void on() {
//        SystemProperties.set(IntentReceiver.KEY_BOOT_START_LOG, "1");
        if(checkStorageAndInitLogPath())
            dumpAll(true);
    }

    @Override
    public void off() {
        dumpAll(false);
//        SystemProperties.set(IntentReceiver.KEY_BOOT_START_LOG, "0");
    }

    private void dumpAll(boolean on){
        if(on)
            DumpOnce.getInstance(mCtx).on();
        dumpLog(on);
        dumpKernel(on);
        dumpNetInfo(on);
        dumpSystemInfo(on);
        dumpStream(on);
        dumpBufferInfo(on);
        dumpVideoInfo(on);
        dumpDvdplayer(on);
    }

    @Override
    public boolean isOn() {
        return isAutoLog();
    }

    private boolean isAutoLog() {
        int isAuto = SystemProperties.getInt(IntentReceiver.KEY_BOOT_START_LOG, 0);
        return (isAuto == 1);
    }

    private boolean checkStorageAndInitLogPath() {
        final Context ctx = mCtx.getApplicationContext();
        String logRootPath = Utils.getSavedLogsDirectory(ctx);
        if(!logRootPath.startsWith("/")){
            //save log path as property
            SystemProperties.set(KEY_DUMP_PATH, "");
            return false;
        } else {
            //save log path as property
            SystemProperties.set(KEY_DUMP_PATH, logRootPath+"/");
            return true;
        }
    }

    private void dumpDvdplayer(boolean b) {
        final Context ctx = mCtx.getApplicationContext();
        if(b){
            DumpDvdplayer.getInstance(ctx).on();
        }else {
            DumpDvdplayer.getInstance(ctx).off();
        }
    }

    private void dumpBufferInfo(boolean b) {
        final Context ctx = mCtx.getApplicationContext();
        if(b){
            DumpBufferInfo.getInstance(ctx).on();
        }else {
            DumpBufferInfo.getInstance(ctx).off();
        }
    }

    private void dumpStream(boolean b) {
        final Context ctx = mCtx.getApplicationContext();
        if(b){
            DumpStream.getInstance(ctx).on();
        }else {
            DumpStream.getInstance(ctx).off();
        }
    }

    private void dumpLog(boolean on){
        final Context ctx = mCtx.getApplicationContext();
        if(on){
            DumpLog.getInstance(ctx).on();
        }else{
            DumpLog.getInstance(ctx).off();
        }
    }

    private void dumpKernel(boolean on) {
        final Context ctx = mCtx.getApplicationContext();
        if(on){
            DumpKernelLog.getInstance(ctx).on();
        }else{
            DumpKernelLog.getInstance(ctx).off();
        }
    }

    private void dumpNetInfo(boolean on){
        final Context ctx = mCtx.getApplicationContext();
        if(on){
            DumpNetInfo.getInstance(ctx).on();
        }else{
            DumpNetInfo.getInstance(ctx).off();
        }
    }

    private void dumpVideoInfo(boolean on){
        final Context ctx = mCtx.getApplicationContext();
        if(on){
            DumpVideoInfo.getInstance(ctx).on();
        }else{
            DumpVideoInfo.getInstance(ctx).off();
        }
    }

    private void dumpSystemInfo(boolean on){
        final Context ctx = mCtx.getApplicationContext();
        if(on){
            DumpSystemInfo.getInstance(ctx).on();
        }else{
            DumpSystemInfo.getInstance(ctx).off();
        }
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }
}