/*
 * Copyright (C) 2008 The Android Open Source Project
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

package android.bluetooth;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.RemoteException;
import android.os.IBinder;
import android.os.ServiceManager;
import android.util.Log;

/**
 * The Android Bluetooth API is not finalized, and *will* change. Use at your
 * own risk.
 *
 * Android only supports one connected Bluetooth Pce at a time.
 *
 * @hide
 */
public class BluetoothRtkbt {

    private static final String TAG = "BluetoothRtkbt";
    private static final boolean DBG = true;
    private static final boolean VDBG = false;


    private IBluetoothRtkbt mService;
    private final Context mContext;
    private ServiceListener mServiceListener;
    private BluetoothAdapter mAdapter;


    /** There was an error trying to obtain the state */
    public static final int STATE_OK        = 0;
    public static final int STATE_ERROR        = -10000;

    public static final String ACTION_RTKBT_GENERIC_EVENT = "com.android.bluetooth.btservice.rtkbt.action.GenericEvent";
    public static final String ACTION_RTKBT_GENERIC_EVENT_id = "com.android.bluetooth.btservice.rtkbt.action.GenericEvent.id";
    public static final String ACTION_RTKBT_GENERIC_EVENT_event = "com.android.bluetooth.btservice.rtkbt.action.GenericEvent.event";
    public static final String ACTION_RTKBT_GENERIC_EVENT_data = "com.android.bluetooth.btservice.rtkbt.action.GenericEvent.data";
    public static final String ACTION_RTKBT_GENERIC_EVENT_len = "com.android.bluetooth.btservice.rtkbt.action.GenericEvent.len";

    public final static int RTKBT_ID_TEST = 0;
    public final static int RTKBT_ID_VHID = 1;
    public final static int RTKBT_ID_AUTOPAIR = 2;
    public final static int RTKBT_ID_VR = 3;
    public final static int RTKBT_ID_3DD      = 4;

    /**
     * An interface for notifying Bluetooth PCE IPC clients when they have
     * been connected to the BluetoothPbap service.
     */
    public interface ServiceListener {
        /**
         * Called to notify the client when this proxy object has been
         * connected to the BluetoothPbap service. Clients must wait for
         * this callback before making IPC calls on the BluetoothPbap
         * service.
         */
        public void onServiceConnected(BluetoothRtkbt proxy);

        /**
         * Called to notify the client that this proxy object has been
         * disconnected from the BluetoothPbap service. Clients must not
         * make IPC calls on the BluetoothPbap service after this callback.
         * This callback will currently only occur if the application hosting
         * the BluetoothPbap service, but may be called more often in future.
         */
        public void onServiceDisconnected();
    }

    final private IBluetoothStateChangeCallback mBluetoothStateChangeCallback =
            new IBluetoothStateChangeCallback.Stub() {
                public void onBluetoothStateChange(boolean up) {
                    if (DBG) Log.d(TAG, "onBluetoothStateChange: up=" + up);
                    if (!up) {
                        if (VDBG) Log.d(TAG,"Unbinding service...");
                        synchronized (mConnection) {
                            try {
                                mService = null;
                                mContext.unbindService(mConnection);
                            } catch (Exception re) {
                                Log.e(TAG,"",re);
                            }
                        }
                    } else {
                        synchronized (mConnection) {
                            try {
                                if (mService == null) {
                                    if (VDBG) Log.d(TAG,"Binding service...");
                                    doBind();
                                }
                            } catch (Exception re) {
                                Log.e(TAG,"",re);
                            }
                        }
                    }
                }
        };

    private final ServiceConnection mConnection = new ServiceConnection() {
        public void onServiceConnected(ComponentName className, IBinder service) {
            if (DBG) log("Proxy object connected");
            mService = IBluetoothRtkbt.Stub.asInterface(service);
            if (mServiceListener != null) {
                mServiceListener.onServiceConnected(BluetoothRtkbt.this);
            }
        }
        public void onServiceDisconnected(ComponentName className) {
            if (DBG) log("Proxy object disconnected");
            mService = null;
            if (mServiceListener != null) {
                mServiceListener.onServiceDisconnected();
            }
        }
    };

    /**
        * Create a BluetoothRtkbt proxy object.
        */
    public BluetoothRtkbt(Context context, ServiceListener l) {
        mContext = context;
        mServiceListener = l;
        mAdapter = BluetoothAdapter.getDefaultAdapter();
        IBluetoothManager mgr = mAdapter.getBluetoothManager();
        if (mgr != null) {
            try {
                mgr.registerStateChangeCallback(mBluetoothStateChangeCallback);
            } catch (RemoteException e) {
                Log.e(TAG,"",e);
            }
        }
        doBind();
    }

    boolean doBind() {
        Intent intent = new Intent(IBluetoothRtkbt.class.getName());
        ComponentName comp = intent.resolveSystemService(mContext.getPackageManager(), 0);
        intent.setComponent(comp);
        if (comp == null || !mContext.bindService(intent, mConnection, 0)) {
            Log.e(TAG, "Could not bind to Bluetooth Pbap Service with " + intent);
            return false;
        }
        return true;
    }

    protected void finalize() throws Throwable {
        try {
            close();
        } finally {
            super.finalize();
        }
    }

    /**
     * Close the connection to the backing service.
     * Other public functions of BluetoothRtkbt will return default error
     * results once close() has been called. Multiple invocations of close()
     * are ok.
     */
    public synchronized void close() {
        IBluetoothManager mgr = mAdapter.getBluetoothManager();
        if (mgr != null) {
            try {
                mgr.unregisterStateChangeCallback(mBluetoothStateChangeCallback);
            } catch (Exception e) {
                Log.e(TAG,"",e);
            }
        }

        synchronized (mConnection) {
            if (mService != null) {
                try {
                    mService = null;
                    mContext.unbindService(mConnection);
                } catch (Exception re) {
                    Log.e(TAG,"",re);
                }
            }
        }
        mServiceListener = null;
    }
   /**
     * Public methods.
     */
    public int GetFeature(int id) {
        if (DBG) log("GetFeature()");
        if (mService != null) {
            try {
                return mService.GetFeature(id);
            } catch (RemoteException e) {Log.e(TAG, e.toString());}
        } else {
            Log.w(TAG, "Proxy not attached to service");
            if (DBG) log(Log.getStackTraceString(new Throwable()));
        }
        return BluetoothRtkbt.STATE_ERROR;
    }
    public int GenericCommand(int id, int command, byte[] data, int len) {
        if (DBG) log("GetFeature()");
        if (mService != null) {
            try {
                return mService.GenericCommand(id, command, data, len);
            } catch (RemoteException e) {Log.e(TAG, e.toString());}
        } else {
            Log.w(TAG, "Proxy not attached to service");
            if (DBG) log(Log.getStackTraceString(new Throwable()));
        }
        return BluetoothRtkbt.STATE_ERROR;
    }

    private static void log(String msg) {
        Log.d(TAG, msg);
    }
}
