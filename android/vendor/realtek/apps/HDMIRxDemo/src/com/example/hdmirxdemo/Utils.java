package com.example.hdmirxdemo;

public class Utils {

    private static Utils sInstance = null;

    static {
        System.loadLibrary("hdmidemoutils");
    }

    public native void init();
    public native void prepare();

    public static Utils getInstance(){
        if(sInstance == null){
            sInstance = new Utils();
        }
        return sInstance;
    }

}
