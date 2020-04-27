package com.realtek.Utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.SystemProperties;
import android.provider.Settings;
import com.rtk.mediabrowser.R;

/**
 * Created by archerho on 2016/2/18.
 */
public class UseRtMediaPlayer {
    private static final String PREF_NAME = "UseRtMediaPlayer";
    private static final String FORCE_RT_MEDIA_PLAYER = "FORCE_RT_MEDIA_PLAYER";
    private boolean mUseRtPlayer;
    private SharedPreferences mPref;
    private static UseRtMediaPlayer mInstance;
    private Context mCtx;

    public static UseRtMediaPlayer getInstance(Context ctx){
        if(mInstance==null)
            mInstance = new UseRtMediaPlayer(ctx);
        return mInstance;
    }

    private UseRtMediaPlayer(Context ctx){
        if(mPref==null)
            mPref = ctx.getSharedPreferences(PREF_NAME, Context.MODE_PRIVATE);
        mCtx = ctx;
    }

    public boolean setUseRtPlayer(boolean b){
        if(mPref==null) return false;
        SharedPreferences.Editor edit = mPref.edit();
        edit.putBoolean(FORCE_RT_MEDIA_PLAYER, b);
        edit.apply();
        mUseRtPlayer = b;
        return b;
    }

    public boolean switchUseRtPlayer(){
        return setUseRtPlayer(!mUseRtPlayer);
    }

    public boolean getUseRtPlayer(){
        if(mPref==null) return false;
        if(mCtx==null) return false;
        if(mCtx.getContentResolver()==null) return false;
        boolean def = false;
        try {
            def = Settings.System.getInt(mCtx.getContentResolver(), Settings.System.REALTEK_FORCE_RT_MEDIAPLAYER) != 0;
        } catch (Settings.SettingNotFoundException e) {
            e.printStackTrace();
            return false;
        }
        mUseRtPlayer = mPref.getBoolean(FORCE_RT_MEDIA_PLAYER, def);
        return mUseRtPlayer;
    }
}
