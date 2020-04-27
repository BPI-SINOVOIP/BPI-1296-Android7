package com.realtek.rtksmb;

import android.content.Context;
import android.content.SharedPreferences;
import android.util.Base64;
import android.util.Log;
import com.realtek.Utils.Encryption;

import java.util.HashMap;

/**
 * Created by archerho on 2016/5/13.
 */
public class SmbUserMgr {
    private static final boolean DEBUG = false;
    private static final String TAG = "SmbUserMgr";
    private static SmbUserMgr mInstance;
    private final Context mCtx;
    private SharedPreferences mPref;
    private static final String PREF_NAME = "SmbUserInfo";
    private static final String SMB_USERINFO = "SMB_USERINFO";
    private static final String SMB_USERNAME = "SMB_USERNAME";
    private static final String SMB_PASSWORD = "SMB_PASSWORD";
    private static final String SMB_SERVER = "SMB_SERVER";
    private static final String SMB_KEY = "SMB_KEY";
    public static final String USERINFO_SPLIT = "/_/";
    private static final HashMap<String, String> mCache = new HashMap<>();

    private SmbUserMgr(Context ctx) {
        if(mPref==null)
            mPref = ctx.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        mCtx = ctx;
    }

    /*
    encrypt
    decrypt
    saveUserInfo
    getUserInfo
    enableSaveUserInfo
    isEnableSave
     */
    public static SmbUserMgr getInstance(Context ctx){
        if(mInstance==null)
            mInstance = new SmbUserMgr(ctx);
        return mInstance;
    }

    public void keepUserInfo(String server, String username, String password){
        Log.i(TAG, String.format("keepUserInfo %s %s", server, username));
        mCache.put(server, username+USERINFO_SPLIT+password);
    }

    public void saveUserInfo(String server, String username, String password){
        Log.i(TAG, "saveUserInfo server="+server);
        if(mPref==null) return;
        String k = mPref.getString(SMB_KEY, null);
        SharedPreferences.Editor edit = mPref.edit();
        edit.putString(SMB_SERVER, server);
        Encryption enc = Encryption.getInstance();
        if(k==null) {
            k = enc.generateRndString(16);
            edit.putString(SMB_KEY, k);
        }
        byte[] iv = enc.generateRndBytes(16);
        String ivBase64 = Base64.encodeToString(iv, Base64.NO_WRAP);
        log("enc.encrypt(password)="+enc.encrypt(password, k, ivBase64));
        edit.putString(SMB_SERVER+"."+server, username+USERINFO_SPLIT+enc.encrypt(password, k, ivBase64)+USERINFO_SPLIT+ ivBase64);
        edit.apply();
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    public String getUserInfo(String server){
        if(mCache==null) return null;
        String s = server;
        String v = mCache.get(server);
        if(v!=null) {
//            log("getUserInfo "+v);
            return v;
        } else {
            //if not in cache, fallback to machine login
            if(server.lastIndexOf("/")<server.length()-1){
                s = server.substring(0, server.lastIndexOf("/")+1);
               // String s = server.substring(0, server.lastIndexOf("/")+1);
                String ret = mCache.get(s);
                if(ret!=null) return ret;
            }
        }
        if(mPref==null) return null;
        if(mCtx==null) return null;
        if(mCtx.getContentResolver()==null) return null;
        String k = mPref.getString(SMB_KEY, null);
        if(k==null) return null;
        String userInfo = mPref.getString(SMB_SERVER+"."+server, null);
        if(userInfo!=null) {
            Encryption enc = Encryption.getInstance();
            String[] infos = userInfo.split(USERINFO_SPLIT);
            infos[1] = enc.decrypt(infos[1], k, infos[2]);
            return infos[0]+USERINFO_SPLIT+infos[1];
        }else {
            userInfo = mPref.getString(SMB_SERVER+"."+s, null);
            if(userInfo!=null) {
                Encryption enc = Encryption.getInstance();
                String[] infos = userInfo.split(USERINFO_SPLIT);
                infos[1] = enc.decrypt(infos[1], k, infos[2]);
                return infos[0]+USERINFO_SPLIT+infos[1];
            }
        }
        return null;
    }
}
