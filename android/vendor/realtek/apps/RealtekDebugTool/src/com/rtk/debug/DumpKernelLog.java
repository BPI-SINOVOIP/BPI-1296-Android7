package com.rtk.debug;

import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.helper.*;
import com.rtk.debug.util.Utils;

import java.io.File;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

/**
 * Created by archerho on 2015/10/7.
 */
public class DumpKernelLog implements IDump{
    public static final String TAG = "DumpKernelLog";
    private static DumpKernelLog mIns;
    private final Context context;
    private static Process sProc;

    public static DumpKernelLog getInstance(Context ctx){
        if(mIns==null){
            mIns = new DumpKernelLog(ctx);
        }
        return mIns;
    }

    private DumpKernelLog(Context context) {
        this.context = context;
    }

    Process startDaemon() throws IOException {
        if(sProc==null)
            sProc = LogcatHelper.getLogcatToFileProcess("kernel", Utils.getLogFilePath(TAG));
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
                log("kernel logcat is cleared");
            } catch (IOException e) {
                e.printStackTrace();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        Intent intent = new Intent(context, LogcatService.class);
        intent.putExtra(LogcatService.EXTRA_TASK, TAG);
        context.startService(intent);
    }

    public void off(){
        log("disable");
        Intent intent = new Intent(context, LogcatService.class);
        intent.putExtra(LogcatService.EXTRA_TASK, "Stop"+TAG);
        context.startService(intent);
    }

    public boolean isOn() {
        return sProc!=null;
    }

    private void log(String s) {
        Log.d(TAG, s);
    }
}
