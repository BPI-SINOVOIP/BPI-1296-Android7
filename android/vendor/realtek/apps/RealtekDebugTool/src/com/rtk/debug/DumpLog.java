package com.rtk.debug;

import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.helper.LogcatHelper;
import com.rtk.debug.helper.RuntimeHelper;
import com.rtk.debug.util.Utils;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

/**
 * Created by archerho on 2015/10/7.
 */
public class DumpLog implements IDump{
    public static final String TAG = "DumpLog";
    private static DumpLog mIns;
    private final Context context;
    private static Process sProc;

    public static DumpLog getInstance(Context ctx){
        if(mIns==null){
            mIns = new DumpLog(ctx);
        }
        return mIns;
    }

    private DumpLog(Context context) {
        this.context = context;
    }

    Process startDaemon() throws IOException {
        if(sProc==null)
            sProc = LogcatHelper.getLogcatToFileProcess(LogcatHelper.BUFFER_MAIN, Utils.getLogFilePath(TAG));
        return sProc;
    }

    Process startDaemon(String savePath) throws IOException {
        if(sProc==null)
            sProc = LogcatHelper.getLogcatToFileProcess(LogcatHelper.BUFFER_MAIN, savePath);
        return sProc;
    }

    void stopDaemon(){
        if(sProc!=null) {
            log("stopDaemon");
            RuntimeHelper.destroy(sProc);
            log("stopDaemon done");
            sProc = null;
        }
    }

    public void on(){
        if(isOn()) return;
        int isAuto = SystemProperties.getInt(IntentReceiver.KEY_BOOT_START_LOG, 0);
        if(isAuto==0) {
            try {
                RuntimeHelper.exec(Arrays.asList("logcat", "-c"));
                Thread.sleep(100);
                log("logcat is cleared");
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Utils.emptyFolder(new File("/data/anr"));
            Utils.emptyFolder(new File("/data/tombstones"));
            Utils.emptyFolder(new File("/data/system/dropbox"));
        }

        DumpDvdplayer.getInstance(context).on();
        Intent intent = new Intent(context, LogcatService.class);
        intent.putExtra(LogcatService.EXTRA_TASK, TAG);
        context.startService(intent);
    }

    public void off(){
        log("disable");
        String logFolder = SystemProperties.get(SettingsFrag.KEY_DUMP_PATH);
        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String dateStr = dateFormat.format(date);

        try {
            Utils.copyDirectory(new File("/data/anr"), new File(logFolder+"anr_"+dateStr));
        } catch (IOException e) {
            log(e.getMessage());
        }

        try {
            Utils.copyDirectory(new File("/data/tombstones"), new File(logFolder+"tombstones_"+dateStr));
        } catch (IOException e) {
            log(e.getMessage());
        }

        String dropboxFilename = "dropbox_"+dateStr+".zip";
        Utils.zipFolder("/data/system/dropbox", logFolder+dropboxFilename);

        Intent intent = new Intent(context, LogcatService.class);
        intent.putExtra(LogcatService.EXTRA_TASK, "Stop"+TAG);
        context.startService(intent);
        DumpDvdplayer.getInstance(context).off();
    }

    public boolean isOn() {
        return sProc!=null;
    }

    private void log(String s) {
        Log.d(TAG, s);
    }
}
