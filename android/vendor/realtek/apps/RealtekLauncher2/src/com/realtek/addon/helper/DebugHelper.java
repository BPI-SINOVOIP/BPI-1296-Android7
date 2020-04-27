package com.realtek.addon.helper;

import android.util.Log;

public class DebugHelper {

    public static boolean ENABLE_DEBUG_TRACE=false;

    public static void dump(String msg){
        //Log.d("RTK",msg);
    }
	
    public static void dump2(String msg){
        //Log.d("RTK",msg);
    }
	
    public static void dumpSQA(String msg){
        Log.d("RtkLauncherDebug",msg);
    }
	
    public static void backtrace(){
        try{
            throw new Exception();
        }catch(Exception e){
            e.printStackTrace();
        }
    }
}
