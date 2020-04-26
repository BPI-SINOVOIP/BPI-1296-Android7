package com.android.server.pppoe;

import android.os.SystemProperties;
import java.io.PrintWriter;
import java.io.FileNotFoundException;
import android.util.Log;
import java.io.File;

public class PppoeNative {

    private static boolean DBG = false;
    private static final String TAG = "PppoeNative";

    private static final int NAP_TIME = 200;
    private static final String PPPOE_STATUS = "net.pppoe.status";
    private static boolean waitProperty(String key, String expect, int maxwait){
        int maxnaps = (maxwait * 1000) / NAP_TIME;
        if (maxnaps < 1)
            maxnaps = 1;

        while(maxnaps-- >= 0){
            String val = SystemProperties.get(key, "");
            // log("val="+val+", expect="+expect);
            if(val.equals(expect))
                return true;

            if (maxnaps >= 0) {
                try{Thread.sleep(200);}catch(InterruptedException e){}
            }
        }
        return false;
    }

    public static int setup(String iface, String user, String password){
        final String tmpParamPath = "/tmp/pppoe-setup-args";
        String args = String.format("%s\t%s\t%s", iface, user, password);
        try(  PrintWriter out = new PrintWriter( tmpParamPath )  ){
            out.println( args );
        } catch (FileNotFoundException e){
            e.printStackTrace();
        }
        SystemProperties.set("ctl.start", "pppoe-setup");
        if(!waitProperty(PPPOE_STATUS, "setuped", 5)) {
            log("waitProperty timeout");
            SystemProperties.set(PPPOE_STATUS, "setup-err");
            new File(tmpParamPath).delete();
            return -1;
        } else {
            new File(tmpParamPath).delete();
        }
        return 0;
    }

    public static int start(){
        SystemProperties.set("ctl.start", "pppoe-start");
        if(!waitProperty(PPPOE_STATUS, "connected", 85)){
            log("waitProperty status connected timeout");
            SystemProperties.set(PPPOE_STATUS, "start-err");
            return -1;
        }
        return 0;
    }

    public static int stop(){
        SystemProperties.set("ctl.start", "pppoe-stop");
        if(!waitProperty(PPPOE_STATUS, "disconnected", 10)){
            log("waitProperty status disconnected timeout");
            SystemProperties.set(PPPOE_STATUS, "stop-err");
            return -1;
        }
        return 0;
    }

    public static int isUp(){
        String val = SystemProperties.get(PPPOE_STATUS, "");
        if ("connected".equals(val)) return 1;
        return 0;
    }

    private static void log(String msg) {
        Log.d(TAG, msg);
    }
}