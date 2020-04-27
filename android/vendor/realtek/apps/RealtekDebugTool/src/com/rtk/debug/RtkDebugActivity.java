package com.rtk.debug;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;

public class RtkDebugActivity extends Activity {
    private static final boolean DEBUG = true;
    private static final String TAG = "RtkDebugActivity";
    private ViewGroup m_Root;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "onCreate");
        super.onCreate(savedInstanceState);
        // Display the fragment as the main content.
        getFragmentManager().beginTransaction()
                .replace(android.R.id.content, new SettingsFrag())
                .commit();
    }

    private void log(String s) {
        if(DEBUG)
            Log.d(TAG, s);
    }

    @Override
    public void onStop() {
        log("onStop");
        super.onStop();
    }

    @Override
    public void onResume() {
        log("onResume");
        super.onResume();
    }

    @Override
    public void onPause() {
        log("onPause");
        super.onPause();
    }

    @Override
    public void onDestroy() {
        log("onDestroy");
        super.onDestroy();
    }
}
