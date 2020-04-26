/*
 * Copyright (C) 2012 The Android Open Source Project
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
 * limitations under the License.*/


package com.android.bluetooth.rtkbt;

import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.IBluetooth;
import com.android.bluetooth.Utils;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.IBinder;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.provider.Settings;
import android.util.Log;
import com.android.bluetooth.btservice.AdapterService;
import com.android.bluetooth.btservice.ProfileService;
import com.android.bluetooth.Utils;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import android.content.pm.PackageManager;

import android.bluetooth.IBluetoothRtkbt;
import android.bluetooth.BluetoothRtkbt;


/**
 * Provides Bluetooth Gatt Host profile, as a service in
 * the Bluetooth application.
 */
public class RtkbtService extends ProfileService {
    private static final boolean DBG = true;
    private static final String TAG = "RtkbtService";

    private static RtkbtService mRtkbtService;
    public static final String RTKBT_PROFILE = "RtkbtProfile";

    static {
        classInitNative();
    }

    public String getName() {
        return TAG;
    }

    public IProfileServiceBinder initBinder() {
        if (DBG) log("initBinder()");
        return new BluetoothRtkbtBinder(this);
    }

    protected boolean start() {
        log("start RTKBT Service");
        InitNative();
        setRtkbtService(this);
        return true;
    }

    protected boolean stop() {
        if (DBG) log("Stopping RTKBT Service");
        return true;
    }

    protected boolean cleanup() {
        log("cleanup RTKBT Service");
        clearRtkbtService();
        CleanupNative();
        return true;
    }

    public static synchronized RtkbtService getRtkbtService() {
        if(mRtkbtService != null && mRtkbtService.isAvailable()) {
            if (DBG) Log.d(TAG, "getRtkbtService(): returning " + mRtkbtService);
            return mRtkbtService;
        }
        if(DBG){
            if(mRtkbtService == null) {
                Log.d(TAG, "getRtkbtService(): service is NULL");
            } else if (!(mRtkbtService.isAvailable())) {
                Log.d(TAG,"getRtkbtService(): service is not available");
            }
        }
        return null;
    }

    private static synchronized void setRtkbtService(RtkbtService instance) {
        if (instance != null && instance.isAvailable()) {
            if (DBG) Log.d(TAG, "setRtkbtService(): set to: " + instance);
            mRtkbtService = instance;
        } else {
            if (DBG){
                if(mRtkbtService == null) {
                    Log.d(TAG, "setRtkbtService(): service not available");
                } else if (!mRtkbtService.isAvailable()) {
                    Log.d(TAG,"setRtkbtService(): service is cleaning up");
                }
            }
        }
    }

    private static synchronized void clearRtkbtService() {
        mRtkbtService = null;
    }


    /**
     * Handlers for incoming service calls
     */
    private static class BluetoothRtkbtBinder extends IBluetoothRtkbt.Stub implements IProfileServiceBinder
    {
        private RtkbtService mService;
        public BluetoothRtkbtBinder(RtkbtService svc) {
            mService = svc;
        }

        public boolean cleanup() {
            mService = null;
            return true;
        }

        private RtkbtService getService() {
            if (!Utils.checkCaller()) {
                Log.w(TAG,"InputDevice call not allowed for non-active user");
                return null;
            }

            if (mService  != null && mService.isAvailable()) {
                return mService;
            }
            return getRtkbtService();
        }

        public int GetFeature(int id) {
            Log.d(TAG, "GetFeature");
            RtkbtService service = getService();
            int result = 0;
            if (service == null) {
                Log.w(TAG, "service is null");
                return -9999;
            }

            try{
                result =  service.GetFeature(id);
            } catch(RemoteException e) {
                Log.w(TAG, "GetFeature catch remote exception!");
            }
            return result;
        }
        public int GenericCommand(int id, int command, byte[] data, int len) {
            Log.d(TAG, "GenericCommand");
            RtkbtService service = getService();
            int result = 0;
            if (service == null) {
                Log.w(TAG, "service is null");
                return -9999;
            }

            try{
                result =  service.GenericCommand(id, command, data, len);
            } catch(RemoteException e) {
                Log.w(TAG, "GenericCommand catch remote exception!");
            }
            return result;
        }
    };

    //Methods
    public int GetFeature(int id) throws RemoteException {
        log("GetFeature id=" + id);
        return GetFeatureNative(id);
    }
    public int GenericCommand(int id, int command, byte[] data, int len) throws RemoteException {
        log("onGenericEvent id=" + id + " command="+ command +" len=" + len);
        return GenericCommandNative(id, command, data, len);
    }

    //Events
    public void onGenericEvent(int id , int event, byte[] data, int len) {
        log("onGenericEvent id=" + id + " event="+ event +" len=" + len);
        if(data == null)
        {
            Intent intent = new Intent(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_id, id);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_event, event);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_len, 0);
            intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
            sendBroadcast(intent, BLUETOOTH_PERM);
            return;
        }
        if(len <= data.length)
        {
            Intent intent = new Intent(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_id, id);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_event, event);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_data, data);
            intent.putExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_len, len);
            intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
            sendBroadcast(intent, BLUETOOTH_PERM);
        }else
            log("onGenericEvent id=" + id + " event="+ event +" len=" + len +" Discard!!!");
    }

    public void log(String message){
        if(DBG) Log.d(TAG, message);
    }

    //private native static void bt3ddClassInitNative();
    private native static void classInitNative();
    private native void InitNative();
    private native void CleanupNative();
    private native int GetFeatureNative(int id);
    private native int GenericCommandNative(int id, int command, byte[] data, int len);
}


