package com.rtk.debug;

import android.content.Context;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.PowerManager;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.helper.SaveLogHelper;
import com.rtk.debug.util.Utils;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by archerho on 2015/10/2.
 */
// usually, subclasses of AsyncTask are declared inside the activity class.
// that way, you can easily modify the UI thread from here
public class DumpiQiYi implements IDump{

    private static final String TAG = "DumpiQiYi";
    private static String mSavedPath;
    private static DumpiQiYi mIns;
    private Context context;
    private static boolean isCancelled = false;
    private static Handler mHandler = new Handler();
    private static final String QUERY_URL = "http://data.video.qiyi.com/v.f4v";
    private static final int SLEEP_MS = 10000;
    private Thread mThread;
    private SimpleDateFormat dateFormat = new SimpleDateFormat("HHmmss");
    private SimpleDateFormat timeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private String mLogPath;

    public static DumpiQiYi getInstance(Context ctx){
        if(mIns==null){
            mIns = new DumpiQiYi(ctx);
        }
        return mIns;
    }

    private DumpiQiYi(Context context) {
        this.context = context;
    }

    public void on(){
        isCancelled = false;
        String filename = dateFormat.format(new Date());
        String logPath = Utils.mkdirs(SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+"DumpiQiYi");
        mLogPath = logPath+"/iQiYi_log_"+filename;
        mSavedPath = logPath+"/status";
        mThread = new Thread(new Runnable() {
            @Override
            public void run() {
                while(!isCancelled) {
                    if(!Utils.hasNetwork(context)) {
                        log("No Internet Connection");
                    }else {
                        final int res = networkGetStatus(QUERY_URL);
                        if (res == 200) {
                            File f = new File(mSavedPath);
                            if (f.length() > 0)
                                log("server status OK");
                        } else {
                            log("networkGetStatus err res=" + res);
                        }
                    }
                    try {
                        Thread.sleep(SLEEP_MS);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            }
        });
        mThread.start();
    }

    public void off() {
        isCancelled = true;
    }

    @Override
    public boolean isOn() {
        return mThread!=null && !isCancelled;
    }

    private void log(String s) {
        Log.d(TAG, s);
        writeToFile(s);
    }

    private void writeToFile(String str) {
        String outStr;
        if(mLogPath!=null) {
            try{
                BufferedWriter bw = new BufferedWriter(new FileWriter(mLogPath, true));
                Date date = new Date();
                String append = timeFormat.format(date);
                outStr=append +": "+str+"\n";
                bw.write(outStr, 0, outStr.length());
                bw.close();
            } catch (Exception e) {
                Log.d(TAG, "Err:"+e.getMessage());
            }
        }
    }

    private int networkGetStatus(String queryUrl){
        InputStream input = null;
        OutputStream output = null;
        HttpURLConnection connection = null;
        try {
            URL url = new URL(queryUrl);
            connection = (HttpURLConnection) url.openConnection();
            connection.connect();

            // expect HTTP 200 OK, so we don't mistakenly save error report
            // instead of the file
            if (connection.getResponseCode() != HttpURLConnection.HTTP_OK) {
                log("Server returned HTTP " + connection.getResponseCode()
                        + " " + connection.getResponseMessage());
                return connection.getResponseCode();
            }

            // this will be useful to display download percentage
            // might be -1: server did not report the length
            int fileLength = connection.getContentLength();

            // download the file
            input = connection.getInputStream();
            output = new FileOutputStream(mSavedPath);

            byte data[] = new byte[4096];
            long total = 0;
            int count;
            while ((count = input.read(data)) != -1) {
                // allow canceling with back button
                if (isCancelled) {
                    input.close();
                    return -1;
                }
                total += count;
                // publishing the progress....
//                if (fileLength > 0) // only if total length is known
//                    publishProgress((int) (total * 100 / fileLength));
                output.write(data, 0, count);
            }
        } catch (Exception e) {
            log("err:"+e.getMessage());
            return -1;
        } finally {
            try {
                if (output != null)
                    output.close();
                if (input != null)
                    input.close();
            } catch (IOException ignored) {
            }

            if (connection != null)
                connection.disconnect();
        }
        return 200;
    }
}
