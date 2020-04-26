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

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothRtkbt;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.util.Log;
import java.util.HashMap;
import java.util.List;

/**
 * 3dd Profile
 */
final class BluetoothRTK3ddProfile {
    private static final String TAG = "BluetoothRTK3ddProfile";
    private static boolean V = true;

    private BluetoothRtkbt mService;
    private boolean mIsProfileReady;
    public static final int id = 4;
    public static final int HANDSHAKE_3DG_EVT  =  0x23;
    public static final int TOGGLE_EVT = 0x28;
    static final String NAME = "3DD service";


    // These callbacks run on the main thread.
    private final class BluetoothRTK3ddListener
            implements BluetoothRtkbt.ServiceListener {

        public void onServiceConnected(BluetoothRtkbt proxy) {
            if (V) Log.d(TAG,"Bluetooth 3dd service connected");
            mService = (BluetoothRtkbt) proxy;
            mIsProfileReady=true;
        }

        public void onServiceDisconnected() {
            if (V) Log.d(TAG,"Bluetooth autopair service disconnected");
            mIsProfileReady=false;
        }
    }

    public boolean isProfileReady() {
        return mIsProfileReady;
    }

    BluetoothRTK3ddProfile(Context context) {
        BluetoothRtkbt service_3dd = new BluetoothRtkbt(context, new BluetoothRTK3ddListener());
    }

    public int GetFeature(int id) {
        if (mService == null) return 0;
        return mService.GetFeature(id);
    }

    public int GenericCommand(byte[] data, int len, int start) {
        if (mService == null) return 0;
        Log.d(TAG,"3dd begin send start command: "+start);
        return mService.GenericCommand(id, start,data,len);
    }
    public String toString() {
        return NAME;
    }

    protected void finalize() {
        if (V) Log.d(TAG, "finalize()");
        if (mService != null) {
            try {
                mService.close();
                mService = null;
            }catch (Throwable t) {
                Log.w(TAG, "Error cleaning up Autopair proxy", t);
            }
        }
    }
}
