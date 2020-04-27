package com.rtk.debug;

import android.content.Context;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.helper.LogcatHelper;
import com.rtk.debug.helper.RuntimeHelper;
import com.rtk.debug.util.UtilLogger;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by archerho on 2015/10/7.
 */
public class DumpAudio implements IDump{
    private static final String TAG = DumpAudio.class.getSimpleName();
    private static DumpAudio mIns;
    private final Context context;
    private static Process sProc;

    public static DumpAudio getInstance(Context ctx){
        if(mIns==null){
            mIns = new DumpAudio(ctx);
        }
        return mIns;
    }

    private DumpAudio(Context context) {
        this.context = context;
    }

    public void on(){

    }

    public void off(){

    }

    private static String read(String path) throws IOException {
        StringBuilder output = new StringBuilder();
        BufferedReader reader = new BufferedReader(new FileReader(path));
        output.append(reader.readLine());
        for (String line = reader.readLine(); line != null; line = reader.readLine()) {
            output.append('\n').append(line);
        }
        reader.close();
        return output.toString();
    }

    private boolean isBootArgsHasKeyword(String keyword){
        int pid = android.os.Process.myPid();
        String cmdline = "";
        try {
            cmdline = read(String.format("/proc/%d/cmdline", pid));
            log("cmdline="+cmdline);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return cmdline.contains(keyword);
    }

    private void log(String s) {
        Log.d(TAG, s);
    }

    public boolean isOn() {
        final String keyword = "androidboot.fwdbg=0x11";
        return isBootArgsHasKeyword(keyword);
    }
}
