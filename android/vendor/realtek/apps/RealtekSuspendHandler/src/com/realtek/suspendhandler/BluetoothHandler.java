package com.realtek.suspendhandler;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;

import android.bluetooth.BluetoothAdapter;
import android.util.Slog;

public class BluetoothHandler {

    private static final boolean DEBUG = true;
    private static final String TAG = "RealtekSuspend-BT";

    private Context mContext;

    private BluetoothStateListener mListener;
    private BluetoothStateReceiver mReceiver;

    private boolean mPreservedState = false;

    // this receiver is unused right now, we are using blocking mode to control bt
    class BluetoothStateReceiver extends BroadcastReceiver {
        boolean mNotifyListener = false;
        @Override
        public void onReceive(Context context, Intent intent) {

            int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, 0);
            int prevState = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_STATE,0);

            if(state == BluetoothAdapter.STATE_OFF || state == BluetoothAdapter.STATE_ON) {
                Slog.d(TAG, "onReceive "+intent.getAction()+" "+state);
                if(mListener != null && mNotifyListener) {
                    mListener.onStateChanged();
                    mNotifyListener = false;
                }
            }
        }
    }

    public BluetoothHandler(Context c, BluetoothStateListener l) {

        mContext = c;
        mListener = l;

        // then book a BroadcastReceiver for bluetooth state changed broadcast
        mReceiver = new BluetoothStateReceiver();
        IntentFilter filter = new IntentFilter();
        filter.addAction(BluetoothAdapter.ACTION_STATE_CHANGED);
        c.registerReceiver(mReceiver,filter);
    }

    private boolean isDeviceAvailable(){
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if(adapter != null) {
            return true;
        }

        return false;
    }

    public boolean isDeviceEnabled(){
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        if(adapter != null) {
            return adapter.isEnabled();
        }
        return false;
    }

    public boolean getBtOldStateBeforeSuspend() {
        return mPreservedState;
    }

    public void saveBtCurrentState(){
        mPreservedState = isDeviceEnabled();
    }

    public void turnOffBtAsync(){
        mReceiver.mNotifyListener = true;
        setDeviceStateAsync(false);
    }

    public void turnOnBtAsync(){
        mReceiver.mNotifyListener = false;
        setDeviceStateAsync(true);
    }

    private void setDeviceStateAsync(boolean enable) {
        boolean currentState = isDeviceEnabled();

        if(enable != currentState){
            BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();

            if(adapter != null) {
                mReceiver.mNotifyListener = true;
                if(enable){
                    adapter.enable();
                }else{
                    adapter.disable();
                }
            }
            
        }
    }
}

