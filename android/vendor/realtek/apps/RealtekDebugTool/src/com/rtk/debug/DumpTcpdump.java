package com.rtk.debug;

import android.content.Context;
import android.content.Intent;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.helper.LogcatHelper;
import com.rtk.debug.helper.RuntimeHelper;
import com.rtk.debug.util.Utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;

/**
 * Created by archerho on 2015/10/7.
 */
public class DumpTcpdump implements IDump{
    public static final String TAG = "DumpTcpdump";
    private static DumpTcpdump mIns;
    private final Context context;
    private static Process sProc;

    public static DumpTcpdump getInstance(Context ctx){
        if(mIns==null){
            mIns = new DumpTcpdump(ctx);
        }
        return mIns;
    }

    private DumpTcpdump(Context context) {
        this.context = context;
    }

    public static String getLogPath(){
        Date date = new Date();
        SimpleDateFormat dateFormat = new SimpleDateFormat("yyyyMMdd_HHmmss");
        String filename = dateFormat.format(date)+".pcap";
        String logFolder = SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+ TAG;
        File f = new File(logFolder);
        if(!f.exists()) f.mkdirs();

        String logFilePath = logFolder+"/"+filename;
        return logFilePath;
    }

    Process startDaemon() throws IOException {
        //tcpdump -i eth0 -s 65535 -w /data/tcpdump.pcap
        //try run command but get error: tcpdump: eth0: You don't have permission to capture on that device(socket: Operation not permitted)
        if(sProc==null) {
            String [] args = { "tcpdump", "-i", "eth0", "-s", "65535", "-w", getLogPath() };
            StringBuffer output = new StringBuffer();
            Process process = new ProcessBuilder(args).redirectErrorStream(true).start();
            BufferedReader reader = new BufferedReader(
                    new InputStreamReader(process.getInputStream()));
            try {
                String line = null;
                while ((line = reader.readLine()) != null) {
                    output.append(line);
                }
            } finally {
                if (reader != null) {
                    reader.close();
                }
            }
            log("output="+output);
            sProc = process;
        }
        return sProc;
    }

    void stopDaemon(){
        if(sProc!=null) {
            RuntimeHelper.destroy(sProc);
            sProc = null;
        }
    }

    public void on(){
        log("enable");
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
