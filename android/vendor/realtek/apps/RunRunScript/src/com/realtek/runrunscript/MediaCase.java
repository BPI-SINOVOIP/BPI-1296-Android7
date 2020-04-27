package com.realtek.runrunscript;


import android.net.Uri;
import android.util.Log;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
/**
 * Created by sam.chen on 7/10/17.
 */
public class MediaCase {
    private static final String TAG = "MediaCase";
    private String mParentDir;
    private String mMediaName;
    private int mTunnel;
    private double mFPS;
    private double mAvgFPS;
    private JSONArray mErrIgnore;

    MediaCase(String ParentDir, String MediaName){
       mParentDir = ParentDir;
       mMediaName = MediaName;
    }
    MediaCase(String ParentDir, JSONObject json)throws JSONException {
        mParentDir = ParentDir;
        mMediaName = json.getString("file");
        mTunnel = json.getInt("tunnel");
        mFPS = json.getDouble("fps");
        try{
            mErrIgnore = json.getJSONArray("ignore");
        }catch(JSONException e){
            Log.d(TAG, "No ignores");
        }
        Log.d(TAG, "New file:" + mMediaName + " , tunnel:" + mTunnel);
    }

    public int isTunnel(){return mTunnel; }

    public String getMountPoint(){
        String path = mParentDir;
        Log.d(TAG, "getMountPoint of " + mParentDir);
        if(mParentDir.startsWith("/storage/")){
            int index = mParentDir.indexOf('/', "/storage/".length());
            if(index != -1){
                path = mParentDir.substring(0, index);
            }
        }
        Log.d(TAG, "getMountPoint: " + path);
        return path;
    }

    public Uri getMediaURI(){
        return Uri.parse(mParentDir+"/"+mMediaName);
    }
    public String getMediaPath(){
        return mParentDir+"/"+mMediaName;
    }

    public String getDirPath(){
        return mParentDir;
    }

    public String getLogDir(){
        File logDir = new File(mParentDir + "/" + "RunRunLog");
        if(!logDir.exists()){
            logDir.mkdir();
        }
        return logDir.toString();
    }
    public String getLogFile(){
        String name = getMediaFileName();
        int pos = name.lastIndexOf(".");
        if(pos > 0){
            name = name.substring(0, pos);
        }
        return getLogDir() + "/" + name + ".log";
    }

    public String getMediaFileName(){
        File mediaFile = new File(getMediaPath());
        if(!mediaFile.exists()){
            Log.e(TAG,"file not found!");
            return "";
        }
        return mediaFile.getName();
    }

    public void setAvgFPS(double fps){
        mAvgFPS = fps;
    }
    public double getAvgFPS(){
        return mAvgFPS;
    }
    public double getFPS(){
        return mFPS;
    }
    public boolean checkIgnoreErr(int code){
        if(mErrIgnore == null){
            return false;
        }

        if(mErrIgnore.length() > 0){
            for(int i = 0; i<mErrIgnore.length(); i++){
                int ig = 0;
                try {
                    ig = mErrIgnore.getInt(i);
                } catch (JSONException e) {
                    Log.d(TAG,"Get Array Item Error");
                }
                if(ig == code)
                    return true;
            }
        }
        return false;
    }
}
