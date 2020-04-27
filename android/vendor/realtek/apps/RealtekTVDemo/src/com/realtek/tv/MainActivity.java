package com.realtek.tv;

import android.app.Activity;
import android.os.Bundle;
import android.content.Context;
import android.os.Handler;
//import android.content.res.Resources;
//import android.content.Intent;
import android.util.Log;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.Toast;

import android.media.tv.TvInputInfo;
import android.media.tv.TvInputManager;
import android.media.tv.TvInputManager.TvInputCallback;
import android.media.tv.TvView;
import android.media.tv.TvContract;
import android.net.Uri;

import java.util.List;
import java.util.ArrayList;

public class MainActivity extends Activity implements ViewTreeObserver.OnGlobalLayoutListener {

    private static final boolean DEBUG = true;
    private static final String TAG = "RealtekTV";

    private final Handler mHandler = new Handler();
    private Context mContext;
    private TvInputManager mTvInputManager;
    private List<TvInputInfo> mPassthroughTvInputList;

    private TvView mTvView;
    //private ShowRunnable mRunnable = new ShowRunnable();
    private SimpleTvInputCallback mCallback = new SimpleTvInputCallback();

    /*
    public class ShowRunnable implements Runnable {
        public void run(){
            playHDMI();
        }
    }
    */

    public class SimpleTvInputCallback extends TvView.TvInputCallback {
        @Override
        public void onDisconnected (String inputId) {
            showToast(""+inputId+" disconnected");
        }

        @Override
        public void onVideoUnavailable (String inputId, int reason) {
            showToast("onVideoUnavailable");
        }

        @Override
        public void onEvent(String inputId, String eventType, Bundle eventArgs) {
            Log.d(TAG, "onEvent "+inputId+" eventType:"+eventType);
            if(eventType.equals("remove")){
            }else{
                //showToast("playHDMI again");
                mTvView.reset();
                playHDMI();
            }
        }
    }

    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        mContext = this;
        mTvInputManager = (TvInputManager)mContext.getSystemService(Context.TV_INPUT_SERVICE);
        buildPassthroughTvInputList();

        setContentView(R.layout.activity_main);

        mTvView = (TvView) findViewById(R.id.tvview);
        mTvView.getViewTreeObserver().addOnGlobalLayoutListener(this);
        mTvView.setCallback(mCallback);
    }

    @Override
    public void onGlobalLayout() {
        Log.d(TAG, "onGlobalLayout");
        mTvView.getViewTreeObserver().removeOnGlobalLayoutListener(this);
        showToast("TIF Rx Preview");
        playHDMI();
    }

    private void showToast(String message){
        Toast.makeText(this,message,Toast.LENGTH_SHORT).show();
    }

    private void playHDMI(){
        if(mPassthroughTvInputList.size()>0){
            TvInputInfo info = mPassthroughTvInputList.get(0);
            String id = info.getId();
            Uri channel = TvContract.buildChannelUriForPassthroughInput(id);
            //showToast("Play HDMI via TIF");
            mTvView.tune(id, channel);
        }
    }

    public void onResume(){
        super.onResume();
    }

    private void buildPassthroughTvInputList(){

        if(mPassthroughTvInputList==null){
            mPassthroughTvInputList = new ArrayList<TvInputInfo>();
        }

        mPassthroughTvInputList.clear();

        for (TvInputInfo input : mTvInputManager.getTvInputList()) {
            if(input.isPassthroughInput()){
                String inputId = input.getId();
                if (DEBUG) Log.d(TAG, "Passthrough Input detected " + inputId);
                mPassthroughTvInputList.add(input);
            }
        }
    }

    public void onPause(){
        super.onPause();
        Log.d(TAG, "untune");
        mTvView.reset();
        //overridePendingTransition(0,0);
        finish();
    }
}
