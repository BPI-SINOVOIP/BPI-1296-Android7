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

package com.android.BluetoothRTK3ddService;

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
BluetoothRTK3ddService only start 3dd sync flow or stop sync flow
*/

public class BluetoothRTK3ddBroadcastReceiver extends BroadcastReceiver {
    private static final String TAG = "BluetoothRTK3ddBroadcastReceiver";
    private static final boolean RTK_DBG = true;

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.i(TAG, "receive intent:" + intent.getAction());

        if (intent.getAction().equals("android.intent.action.BOOT_COMPLETE")) {
            if (RTK_DBG) Log.v(TAG,  "android.intent.action.BOOT_COMPLETE");
            context.startService(new Intent(context, BluetoothRTK3ddService.class));
        }
        if (intent.getAction().equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
			int bt_state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR);
		    Log.i(TAG, "bt_state:" + bt_state);
            switch (intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, BluetoothAdapter.ERROR)) {
                case BluetoothAdapter.STATE_TURNING_OFF:
				{
                    if (RTK_DBG) Log.v(TAG,  "Receiver BLUETOOTH_STATE_CHANGED_ACTION, STATE_TURNING_OFF");
                    // bluetooth is off, stop RTKAutoPair service
                    //context.stopService(new Intent(context, BluetoothRTK3ddService.class));
                }
                    break;
                case BluetoothAdapter.STATE_ON:
                {
                    if (RTK_DBG) Log.v(TAG,  "Receiver BLUETOOTH_STATE_CHANGED_ACTION, STATE_ON");
                    context.startService(new Intent(context, BluetoothRTK3ddService.class));
                }
                    break;
               default:
                    break;
            }
        }

    }
}
