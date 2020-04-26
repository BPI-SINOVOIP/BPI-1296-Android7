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

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothRtkbt;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.util.Log;
import java.util.HashMap;
import java.util.List;

/**
 * AutoPair Profile
 */
final class BluetoothRTKAutoPairProfile {
    private static final String TAG = "BluetoothRTKAutoPairProfile";
    private static boolean V = true;

    private BluetoothRtkbt mService;
    private boolean mIsProfileReady;
    private static final int id = BluetoothRtkbt.RTKBT_ID_AUTOPAIR;
    private static final byte autopair_opcode = (byte)0x77;

    static final String NAME = "Auto Pair";


    // These callbacks run on the main thread.
    private final class BluetoothRTKAutoPairListener
            implements BluetoothRtkbt.ServiceListener {

        public void onServiceConnected(BluetoothRtkbt proxy) {
            if (V) Log.d(TAG,"Bluetooth autopair service connected");
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

    BluetoothRTKAutoPairProfile(Context context) {
        BluetoothRtkbt autopair = new BluetoothRtkbt(context, new BluetoothRTKAutoPairListener());
    }

    public int GetFeature(int id) {
        if (mService == null) return 0;
        return mService.GetFeature(id);
    }

    public int GenericCommand(byte[] data, int len) {
        if (mService == null) return 0;
        return mService.GenericCommand(id, autopair_opcode,data,len);
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
