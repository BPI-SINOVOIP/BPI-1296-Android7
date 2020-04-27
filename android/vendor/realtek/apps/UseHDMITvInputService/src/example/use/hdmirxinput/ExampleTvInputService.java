package example.use.hdmirxinput;

import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.database.Cursor;
import android.media.MediaPlayer;
import android.media.tv.TvContract;
import android.media.tv.TvInputManager;
import android.media.tv.TvInputService;
import android.media.tv.TvInputHardwareInfo;
import android.media.tv.TvInputInfo;
import android.media.tv.TvStreamConfig;
import android.net.Uri;
import android.view.Surface;
import android.util.Log;
import android.content.pm.ResolveInfo;
import android.content.pm.PackageManager;
import android.content.Intent;
import android.content.ContentResolver;
import java.io.IOException;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.os.SystemProperties;
import android.content.SharedPreferences;

public class ExampleTvInputService extends TvInputService {

    private static final boolean DEBUG=false;
    public static final String KEY_HW_ID = "hardwareId";

    private static final String TAG="ExampleUseHDMITvInputService";
    private static final String TAG_SESSION = TAG+"-Session";
    private static final String TAG_DUMMY = TAG+"-DummySession";

    private static SharedPreferences mSP = null;
    private TvInputManager mManager = null;

    @Override
    public Session onCreateSession(String inputId) {
        if(DEBUG) Log.d(TAG,"onCreateSession inputId:"+inputId+" this:"+this);
        String hdmiHwId = getHDMITvInputId();
        if(hdmiHwId != null){
            return new UseHDMISessionImpl(this);
        }else{
            return new DummySessionImpl(this);
        }
    }

    public static SharedPreferences getSP(Context c){
        if(mSP == null){
            mSP = c.getSharedPreferences("hardwareid",Context.MODE_PRIVATE);
        }
        return mSP;
    }

    private String readHardwareIdFromSP(){
        SharedPreferences sp = getSP(this);
        return sp.getString(KEY_HW_ID, null);
    }

    private String getHDMITvInputId(){
        if(mManager == null){
            mManager = (TvInputManager) getSystemService(Context.TV_INPUT_SERVICE);
        }
        TvInputInfo passThroughInput = null;
        List<TvInputInfo> list = mManager.getTvInputList();
        for(int i=0;i<list.size();i++){
            if(list.get(i).isPassthroughInput() && list.get(i).getType() == TvInputInfo.TYPE_HDMI){
                if(DEBUG) Log.d(TAG, " -- TvInputInfo: "+list.get(i));
                passThroughInput = list.get(i);
                break;
            }
        }
        if(DEBUG) Log.d(TAG, "-- passThroughInput: "+passThroughInput);
        if(passThroughInput!=null){
            String id = passThroughInput.getId();
            Log.d(TAG, "getHDMITvInputId id:"+id);
            return id;
        }else{
            Log.d(TAG, "getHDMITvInputId id: N/A");
            return null;
        }
    }

    private class UseHDMISessionImpl extends TvInputService.HardwareSession {

        private Uri mChannelUri = null;
        private Surface mSurface = null;

        UseHDMISessionImpl(Context context){
            super(context);
            if(DEBUG) Log.d(TAG_SESSION, "create UseHDMISessionImpl");
        }

        public void onRelease() {
            if(DEBUG) Log.d(TAG_SESSION,"onRelease");
        }

        public void onSetStreamVolume(float volume) {
        }

        public void onSetCaptionEnabled(boolean enabled) {
        }

        /*
        public boolean onSetSurface(Surface surface) {
            mSurface = surface;
            return true;
        }
        */

        public void onHardwareVideoAvailable(){
            if(DEBUG) Log.d(TAG_SESSION, "onHardwareVideoAvailable");
            notifyVideoAvailable();
            notifyContentAllowed();
        }

        public String getHardwareInputId(){
            return readHardwareIdFromSP();
        }

        public boolean onTune(Uri channelUri) {
            mChannelUri = channelUri;
            return true;
        }

    } // end Session

    // Dummy session
    private class DummySessionImpl extends TvInputService.Session {

        DummySessionImpl(Context context){
            super(context);
        }

        public void onRelease() {
            if(DEBUG){Log.d(TAG_DUMMY, "onRelease");}
        }

        public void onSetStreamVolume(float volume) {
        }

        public void onSetCaptionEnabled(boolean enabled) {
        }

        public boolean onSetSurface(Surface surface) {
            if(DEBUG) Log.d(TAG_DUMMY, "onSetSurface surface:"+surface);
            return false;
        }

        public boolean onTune(Uri channelUri) {
            return false;
        }
    } // end session
}
