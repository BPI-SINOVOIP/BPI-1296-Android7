/*
 * Copyright (C) 2015 The Realtek Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.BluetoothRTKAutoPairService;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothRtkbt;
import android.bluetooth.BluetoothDevice;
import android.util.Log;
import android.widget.Toast;
import android.util.Log;
import java.util.HashMap;


/*
 *BluetoothRTKAutoPairBroadcastReceiver sync state with bluetooth,
 *STATE_ON start BluetoothRTKAutoPairService
 *STATE_TURNING_OFF stop BluetoothRTKAutoPairService
 */

public class BluetoothRTKAutoPairBroadcastReceiver extends BroadcastReceiver {
    private static final String TAG = "<===AutoPair===>BluetoothRTKAutoPairBroadcastReceiver";
    private static final boolean RTK_DBG = true;
    public static final BluetoothRTKAutoPairLoadConfig mLoadConfig = BluetoothRTKAutoPairLoadConfig.getInstance();

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(TAG, "receive intent:" + intent.getAction());

        if(intent.getAction().equals("android.intent.action.BOOT_COMPLETED")) {
            mLoadConfig.do_config();
            if((byte)(mLoadConfig.autopair_type & (byte)BluetoothRTKAutoPairLoadConfig.vendor_event_pair) == (byte)BluetoothRTKAutoPairLoadConfig.vendor_event_pair) {
                if (mLoadConfig.service_start == false) {
                    context.startService(new Intent(context, BluetoothRTKAutoPairService.class));
                    mLoadConfig.service_start = true;
                }
            }
        }

        if (intent.getAction().equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
            switch (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR)) {
                case BluetoothAdapter.STATE_TURNING_OFF:
                    if (RTK_DBG) Log.v(TAG,  "Receiver BLUETOOTH_STATE_CHANGED_ACTION, STATE_TURNING_OFF");
                    // bluetooth is off, stop RTKAutoPair service
                    context.stopService(new Intent(context, BluetoothRTKAutoPairService.class));
                    break;
                case BluetoothAdapter.STATE_ON:
                {
                    if (RTK_DBG) Log.v(TAG,  "Receiver BLUETOOTH_STATE_CHANGED_ACTION, STATE_ON");
                    mLoadConfig.do_config();
                    if(mLoadConfig.data_length == 0 && mLoadConfig.autoPairInfoMap.size() == 0) {
                        CharSequence text = "Attention! Autopair config is not exist! Please check it";
                        for(int i = 0; i < 3; i++){
                            Toast.makeText(context.getApplicationContext(),text,Toast.LENGTH_LONG).show();
                        }
                    }
                    if((byte)(mLoadConfig.autopair_type & (byte)BluetoothRTKAutoPairLoadConfig.vendor_event_pair) == (byte)BluetoothRTKAutoPairLoadConfig.vendor_event_pair) {
                        if (mLoadConfig.service_start == false) {
                            context.startService(new Intent(context, BluetoothRTKAutoPairService.class));
                            mLoadConfig.service_start = true;
                        }
                    }
                }
                    break;
               default:
                    break;
            }
        }
        //privode android.action.rtk.START_SERV and android.action.rtk.STOP_SERV content to start or stop serivce
        if (intent.getAction().equals("android.action.rtk.START_SERV")) {
            if (RTK_DBG) Log.v(TAG,  "android.action.rtk.START_SERV");
            if (mLoadConfig.service_start == false) {
                mLoadConfig.do_config();
                mLoadConfig.service_start = true;
                context.startService(new Intent(context, BluetoothRTKAutoPairService.class));
            }
        }
        if (intent.getAction().equals("android.action.rtk.STOP_SERV")) {
            if (RTK_DBG) Log.v(TAG,  "android.action.rtk.STOP_SERV");
            context.stopService(new Intent(context, BluetoothRTKAutoPairService.class));
        }
    }


}
