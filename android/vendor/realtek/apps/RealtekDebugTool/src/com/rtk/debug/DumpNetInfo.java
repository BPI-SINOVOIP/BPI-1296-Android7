package com.rtk.debug;

import android.content.Context;
import android.net.ConnectivityManager;
import android.os.SystemProperties;
import android.util.Log;
import com.rtk.debug.util.Utils;

import java.io.*;
import java.text.SimpleDateFormat;
import java.util.Date;

public class DumpNetInfo implements IDump{

    private static final String TAG = "DumpNetInfo";
    private static final boolean DBG = false;

    private static final String ETH0_STATISTICS = "/sys/class/net/eth0/statistics/rx_bytes";
    private static final String ETH9_STATISTICS = "/sys/class/net/eth9/statistics/rx_bytes";
    private static final String WLAN0_STATISTICS = "/sys/class/net/wlan0/statistics/rx_bytes";
    private static final int TIME_INTERVAL = 10;
    private static String mLogPath;
    private static DumpNetInfo mIns;
    private static boolean isCancelled = false;
    private Context context;
    private SimpleDateFormat dateFormat = new SimpleDateFormat("HHmmss");
    private SimpleDateFormat timeFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
    private Thread mThread;

    public static DumpNetInfo getInstance(Context ctx){
        if(mIns==null){
            mIns = new DumpNetInfo(ctx);
        }
        return mIns;
    }

    private DumpNetInfo(Context context) {
        this.context = context;
    }

    public void on(){
        if(isOn()) return;
        isCancelled = false;
        Date date = new Date();
        String filename = dateFormat.format(date);
        String logPath = Utils.mkdirs(SystemProperties.get(SettingsFrag.KEY_DUMP_PATH)+"DumpNetInfo");
        mLogPath = logPath+"/NetInfo_"+filename;
        mThread = new Thread(new Runnable() {
            @Override
            public void run() {
                int oldNetType=ConnectivityManager.TYPE_NONE;
                int netType=0;
                long oldNetworkStatistics=0;
                long networkStatistics=0;
                long diffStatistics=0;
                String netTypeName = null;
                while(!isCancelled) {
                    if(!Utils.hasNetwork(context)) {
                        log("No Internet Connection");
                    }else {
                        netType = Utils.getActiveNetworkType(context);
                        netTypeName = Integer.toString(netType);
                        if(isOpenWrtNet()) {
                            networkStatistics = getNetworkStatistics(ETH9_STATISTICS);
                            netTypeName = "eth9";
                        }
                        else if(netType==ConnectivityManager.TYPE_ETHERNET) {
                            networkStatistics = getNetworkStatistics(ETH0_STATISTICS);
                            netTypeName = "eth0";
                        }
                        else if(netType==ConnectivityManager.TYPE_WIFI) {
                            networkStatistics = getNetworkStatistics(WLAN0_STATISTICS);
                            netTypeName = "wlan0";
                        }
                      
                        if(oldNetType!= netType) {
                            if(oldNetType!=ConnectivityManager.TYPE_NONE)
                                log("Network changed from "+oldNetType+" to "+netType);
                            oldNetType = netType;
                        }
                        else {
                            diffStatistics = ((networkStatistics-oldNetworkStatistics)/TIME_INTERVAL)/1024;
                            log(diffStatistics+" KBytes/sec ( type:"+netTypeName+", "+oldNetworkStatistics+" -> "+networkStatistics+" )");
                        }
                        oldNetworkStatistics = networkStatistics;
                    }
                    for(int i=0;i<TIME_INTERVAL*2;i++){
                        try {
                            if(isCancelled) break;
                            Thread.sleep(500);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }

                }
            }
        });
        mThread.start();
    }

    public void off() {
        isCancelled = true;
        if(mThread!=null) {
            Log.d(TAG, "off");
            try {
                mThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            Log.d(TAG, "off done");
        }
    }

    public boolean isOn(){return mThread!=null && !isCancelled;}

    private void log(String str) {
        if(DBG)
            Log.d(TAG, str);
        writeToFile(str);
    }

    private static boolean isOpenWrtNet(){
        return SystemProperties.getBoolean("persist.rtk.net.openwrt", false)
                && !SystemProperties.getBoolean("persist.rtk.net.openwrt.ottwifi", false)
                && new File("/sys/class/net/eth9").exists();
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

    private long getNetworkStatistics(String statusPath) {
        String data=null;
        try {
            BufferedReader br = new BufferedReader(new FileReader(statusPath));
            data = br.readLine();
            br.close();
        } catch (Exception e) {
            Log.d(TAG, "Err:"+e.getMessage());
            return -1;
        }
        if(data==null) return -1;
        long networkStatistics = Long.parseLong(data);
        if(DBG)
            Log.d(TAG, "getNetworkStatistics: "+networkStatistics);
        return networkStatistics;
    }
}
