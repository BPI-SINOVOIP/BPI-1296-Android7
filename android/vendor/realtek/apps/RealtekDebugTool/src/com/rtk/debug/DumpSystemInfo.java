package com.rtk.debug;

import java.text.SimpleDateFormat;
import java.util.Date;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.io.OutputStreamWriter;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.FileNotFoundException;

import android.content.Context;
import android.os.SystemClock;
import android.util.Log;
import android.os.SystemProperties;

import com.rtk.debug.util.Utils;

public class DumpSystemInfo implements IDump{

    private static final String TAG = "DumpSystemInfo";
    private static DumpSystemInfo mInstance;

    private Context mContext;
    // runnables to log system dump periodically 
    private LogRunnable mSurfaceRunnable;
    private LogRunnable mPowerRunnable;
    private LogRunnable mMeminfoRunnable;
    private LogRunnable mProcrankRunnable;
    private Thread mThread;

    /**
     * Inner helper class to do polling tasks asynchronously
     */
    class LogRunnable implements Runnable{

        PrintWriter mWriter;
        int mSleepInterval;
        String mCommand;
        boolean mBThreadIsAlive;
        int mLoops;
        String mFullPath;
        String mClassName;
        boolean mBAsRoot;

        /**
         * Constructor
         * Path:            file path of log file
         * command:         system command
         * sleepInterval:   polling interval
         * asRoot:          run as root uid or not (root is not supported right now)
         */
        LogRunnable(String logPath, String command, int sleepInterval, boolean asRoot){
            mCommand = command;
            mBAsRoot = asRoot;
            mSleepInterval = sleepInterval;
            if(mSleepInterval <= 0){
                mSleepInterval = 5;
            }
            mLoops = mSleepInterval * 2;
            String parentPath = Utils.mkdirs(SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+"DumpSystemInfo");
            mFullPath = parentPath+"/"+logPath;
            //mFullPath = SaveLogHelper.getSavedLogsDirectory().toString()+"/"+logPath;
            mClassName = this.getClass().getName();
            //mBThreadIsAlive = true;
        }

        @Override
        public void run(){
            mBThreadIsAlive = true;
            Utils.mkdirs(SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+"DumpSystemInfo");
            String path = mFullPath+"-"+getCurrentTimeStr("yyyyMMdd_HHmmss")+".log";
            // create PrintWriter
            Log.d(TAG, "start monitoring "+path);
            try{
                mWriter = new PrintWriter(path);
            }catch(FileNotFoundException e){
                e.printStackTrace();
                mWriter = null;
                Log.e(TAG, "Create writer \""+path+"\" failed");
                return;
            }
            int flushCounter = 0;
            while(mBThreadIsAlive){
                //String rst = "";
                //if(mBAsRoot)
                //rst = processSystemCommandAsRoot(mCommand);
                //else
                Log.d(TAG, "SystemCommand begin");
                processSystemCommand(mCommand,mWriter);
                Log.d(TAG, "SystemCommand end");
                flushCounter++;
                if(flushCounter>=5){
                    mWriter.flush();
                    flushCounter=0;
                }
                for(int i=0;i<mLoops;i++){
                    if(!mBThreadIsAlive)break;
                    SystemClock.sleep(500);
                }
            }
            mWriter.close();
            Log.d(TAG, mClassName+" run thread exit, close file");
        }

        /**
         * API to stop polling thread.
         */
        public void stop(){
            mBThreadIsAlive=false;
        }
    }

    public static DumpSystemInfo getInstance(Context c){
        if(mInstance==null){
            mInstance = new DumpSystemInfo(c);
        }
        return mInstance;
    }

    /**
     * Constructor
     */
    public DumpSystemInfo(Context c){
        mContext = c;

        //String[] surface = {"dumpsys SurfaceFlinger"};
        //String[] power = {"dumpsys power"};
        //String[] meminfo = {"dumpsys meminfo"};
        //String[] procrank = {"su", "-c", "procrank"};

        mSurfaceRunnable = new LogRunnable("surface", "dumpsys SurfaceFlinger", 5, false);
        mPowerRunnable = new LogRunnable("power", "dumpsys power", 5, false);
        mMeminfoRunnable = new LogRunnable("meminfo", "dumpsys meminfo", 5, false);
        //mProcrankRunnable = new LogRunnable("procrank", "procrank", 2, false);
    }

    public void on(){
        if(isOn()) return;
//        new Thread(mSurfaceRunnable).start();
//        new Thread(mPowerRunnable).start();
        mThread = new Thread(mMeminfoRunnable);
        mThread.start();
        //new Thread(mProcrankRunnable).start();
    }

    public void off(){
//        mSurfaceRunnable.stop();
//        mPowerRunnable.stop();
        mMeminfoRunnable.stop();
        if(mThread!=null){
            Log.d(TAG, "off");
            try {
                mThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Log.d(TAG, "off done");
        }
        //mProcrankRunnable.stop();
    }

    @Override
    public boolean isOn() {
        boolean isAllOn = mSurfaceRunnable!=null && mSurfaceRunnable.mBThreadIsAlive
                && mPowerRunnable!=null && mPowerRunnable.mBThreadIsAlive
                && mMeminfoRunnable!=null && mMeminfoRunnable.mBThreadIsAlive;
        isAllOn = mMeminfoRunnable!=null && mMeminfoRunnable.mBThreadIsAlive;
        return isAllOn;
    }

    /*
    public void on(){
        // create a polling thread....
        bThreadAlive = true;
        mSavedPath = SaveLogHelper.getSavedLogsDirectory().toString()+"/dumpsysSurfaceFlinger";
        new Thread(new Runnable() {
            public void run() {
                // create output file resource
                PrintWriter writer=null;

                try{
                    writer = new PrintWriter(mSavedPath);
                }catch(FileNotFoundException e){
                    e.printStackTrace();
                    writer = null;
                    Log.e(TAG,"Create file error, terminate thread");
                    return;
                }

                while(bThreadAlive){
                    // get the String
                    String rst = processSystemCommand(SURFACEFLINGER);
                    writer.println(rst);
                    writer.flush();
                    // do a simple sleep..
                    for(int i=0;i<10;i++){
                        if(!bThreadAlive) break;
                        SystemClock.sleep(500);
                    }
                }
                writer.close();
                Log.d(TAG, "run thread exit, file closed");
            }
        }).start();
    }
    */

    /*
    public void off(){
        bThreadAlive = false;
    }
    */

    /**
     * Alternative su function, but none-functioned right now
     */
    private String processSystemCommandAsRoot2(String cmd){
        Log.d(TAG, "process "+cmd+" as su");
        String[] cmds = {"su","-c",cmd};

        for(int i=0;i<cmds.length;i++){
            Log.d(TAG, "cmd:"+cmds[i]);
        }

        Process process = null;
        BufferedReader bufferedReader = null;
        try{
            process = Runtime.getRuntime().exec(cmds);
            Log.d(TAG, "process:"+process);
            process.waitFor();
            bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = bufferedReader.readLine()) != null){
                Log.d(TAG, line);
            }
            bufferedReader.close();
            bufferedReader = null;
        }catch(IOException e){
            e.printStackTrace();
        }catch(InterruptedException e){
            e.printStackTrace();
        }finally{
            if(bufferedReader!=null){
                try{
                    bufferedReader.close();
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        }
        return "";
    }

    /**
     * A simple function to get output message from system command run as root
     */
    private String processSystemCommandAsRoot(String cmd){
        //OutputStreamWriter osw = null;
        DataOutputStream os = null;
        Process process = null;
        try{
            process = Runtime.getRuntime().exec("/system/xbin/su");
            Log.d(TAG, "process:"+process);
            os = new DataOutputStream(process.getOutputStream());
            os.writeBytes(cmd+"\n"); 
            os.writeBytes("exit\n");
            os.flush();
            process.waitFor();
            //osw = new OutputStreamWriter(process.getOutputStream());
            //osw.write(cmd);
            //osw.flush();
            //osw.close();
            //osw=null;
        }catch(IOException e){
            e.printStackTrace();
        }catch(InterruptedException e){
            e.printStackTrace();
        }finally{
            if (os != null) {
                try{
                    os.close();
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        }

        if(process!=null){
            BufferedReader bufferedReader=null;
            Log.d(TAG, "read out from process");
            try{
                bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
                String line;
                while ((line = bufferedReader.readLine()) != null){
                    Log.d(TAG, line);
                }
                bufferedReader.close();
                bufferedReader=null;
            }catch(IOException e){
                e.printStackTrace();
            }finally{
                if(bufferedReader!=null){
                    try{
                        bufferedReader.close();
                    }catch(IOException e){
                        e.printStackTrace();
                    }
                }
            }
        }

        return "WTF...";
    }

    /**
     * A simple function to get output message from system command
     */
    synchronized private void processSystemCommand(String cmd, PrintWriter writer){
        BufferedReader bufferedReader=null;
        try{
            Process process = Runtime.getRuntime().exec(cmd);
            process.waitFor();
            bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            writer.append("TimeStamp: "+getCurrentTimeStr("HH:mm:ss")+"\n\n");
            String line;
            int num=0;
            while ((line = bufferedReader.readLine()) != null){
                num = num + line.length();
                writer.append(line+"\n");
            }
            writer.append("\n=======================================================\n");
            //Log.d(TAG, "close buffered reader total length:"+num);
            bufferedReader.close();
            bufferedReader=null;
        }catch(IOException e){
            e.printStackTrace();
        }catch(InterruptedException e){
            e.printStackTrace();
        }finally{
            if(bufferedReader!=null){
                try{
                    bufferedReader.close();
                    bufferedReader=null;
                }catch(IOException e){
                    e.printStackTrace();
                }
            }
        }
    }

    /**
     * return current time string
     */
    private String getCurrentTimeStr(String format){
        SimpleDateFormat sdf = new SimpleDateFormat(format);
        String currentDateandTime = sdf.format(new Date());
        return currentDateandTime;
    }
}
