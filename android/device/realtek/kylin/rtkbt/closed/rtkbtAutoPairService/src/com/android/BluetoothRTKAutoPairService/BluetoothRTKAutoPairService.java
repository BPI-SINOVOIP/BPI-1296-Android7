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

import android.app.Service;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothClass;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.res.Resources;
import android.os.RemoteException;
import android.os.Binder;
import android.os.IBinder;
import android.os.Handler;
import android.widget.Toast;
import java.lang.CharSequence;
import java.lang.InterruptedException;
import android.util.Log;
import android.os.ParcelUuid;
import android.os.Message;

import android.bluetooth.BluetoothAdapter;
import android.content.SharedPreferences;
import android.bluetooth.BluetoothRtkbt;
import android.bluetooth.BluetoothInputDevice;
import android.bluetooth.BluetoothUuid;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeAdvertiser;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanRecord;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;


import android.os.SystemClock;
import java.util.HashMap;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Set;

import android.app.NotificationManager;
import android.app.Notification;
import android.support.v4.app.NotificationCompat;

/*
 * BluetoothRTKAutoPairService receive auto_pairable_device found event
 */

public class BluetoothRTKAutoPairService extends Service {
/***************************************************************************************************
        autopair debug and version message
****************************************************************************************************/
    private static String TAG = " < === AutoPair ===>BluetoothRTKAutoPairService";
    private static boolean RTK_DBG = false;
    private static boolean RTK_INFO = true;
    private static boolean RTK_NOTIFY = false;
    private static final int RTK_SERVICE_RETRY_NUM = 10;
    private static String verison = "20160727";

/************************************************************************************************************
        autopair event table
*************************************************************************************************************/
    private static final int RTKBT_AUTOPAIR_Event_FoundRCU                      = 1;
    private static final int RTKBT_AUTOPAIR_EVENT_BLERemoveBondResult           = 2;
    private static final int RTKBT_AUTOPAIR_Event_ClassicRemoveBondResult       = 3;
    private static final int RTKBT_AUTOPAIR_Event_BLEUUID                       = 4;
    private static final int RTKBT_AUTOPAIR_Event_BLECreateBondResult           = 5;
    private static final int RTKBT_AUTOPAIR_Event_ClassicUUID                   = 6;
    private static final int RTKBT_AUTOPAIR_Event_ClassicCreateBondResult       = 7;
    private static final int RTKBT_AUTOPAIR_Event_SaveNewRCU                    = 8;
    private static final int RTKBT_AUTOPAIR_Event_PairSuccess                   = 9;
    private static final int RTKBT_AUTOPAIR_Event_PairFail                      = 10;
    private static final int RTKBT_AUTOPAIR_Event_PairTimeOut                   = 11;
    private static final int RTKBT_AUTOPAIR_Event_CheckScanTimeOut              = 12;
    private static final int RTKBT_AUTOPAIR_Event_StartScan                     = 13;
    private static final int RTKBT_AUTOPAIR_Event_weak_signal                   = 14;
    private static final int RTKBT_AUTOPAIR_Event_ScanInterval                  = 15;
    private static final int RTKBT_AUTOPAIR_Event_CheckServiceStart             = 16;
    private static final int RTKBT_AUTOPAIR_Event_ConnectSuccess                = 17;
    private static final int RTKBT_AUTOPAIR_Event_DeviceConnectResult           = 18;
    private static final int RTKBT_AUTOPAIR_Event_ServiceTimeOut                = 19;

/************************************************************************************************************
        autopair state table
*************************************************************************************************************/
    private static final int RTKBT_AUTOPAIR_State_Idle                     = 0;
    private static final int RTKBT_AUTOPAIR_State_Found                    = 1;
    private static final int RTKBT_AUTOPAIR_State_RemoveBond               = 2;
    private static final int RTKBT_AUTOPAIR_State_BLEBond                  = 3;
    private static final int RTKBT_AUTOPAIR_State_ClassicBond              = 4;
    private static final int RTKBT_AUTOPAIR_State_SaveNewRCU               = 5;
    private static final int RTKBT_AUTOPAIR_State_PairSuccess_to_connect   = 6;
    private static final int RTKBT_AUTOPAIR_State_ConnectSuccess           = 7;
    private static final int RTKBT_AUTOPAIR_State_BLEScan                  = 8;

    private static final int RTKBT_AUTOPAIR_State_PairFail                 = 0x101;
    private static final int RTKBT_AUTOPAIR_State_PairTimeout              = 0x102;
    private static final int RTKBT_AUTOPAIR_State_ConnectFail              = 0x103;

    private static final int RTKBT_AUTOPAIR_State_Cancel_inqury_4_scan     = 100;


/****************************************************************************************************
        autopair Internal object
*****************************************************************************************************/
    private IntentFilter                    mIntentFilter = null;
    private BluetoothRTKAutoPairLoadConfig  mLoadConfig;
    private BluetoothRTKAutoPairProfile     mPairProfile;
    private BluetoothRTKAutoPairHidProfile  mPairHidProfile;
    private BluetoothAdapter                RTKBT_AUTOPAIR_BluetoothAadapter;
    private BluetoothDevice                 RTKBT_AUTOPAIR_ble_device;
    private BluetoothDevice                 RTKBT_AUTOPAIR_classic_device;
    private BluetoothDevice                 RTKBT_AUTOPAIR_pending_device;
    private BluetoothLeScanner              RTKBT_AUTOPAIR_scanner;

/****************************************************************************************************
        autopair member variables
*****************************************************************************************************/
    private boolean             RTKBT_AUTOPAIR_auto_scan            = false;
    private boolean             mReveiverRegister;
    private static boolean      RTKBT_has_hogp                      = false;
    private static int          RTKBT_AUTOPAIR_State                = RTKBT_AUTOPAIR_State_Idle;
    private static int          RTKBT_AUTOPAIR_notify_counter       = 0;

    private static byte[]       RTKBT_AUTOPAIR_vendor_event_arry;
    private static int[]        RTKBT_AUTOPAIR_rand_num             ={0,0,0,0};
    private static int[]        RTKBT_AUTOPAIR_broadcast_num        ={0,0,0,0,0};
    private static byte[]       RTKBT_VENDOR_data                   ={0,0,0,0,0,0};
    private static final int[]  RTKBT_AUTOPAIR_RcuId                = {0x0002};

    private static boolean      RTKBT_IS_scan                       = false;
    private static final int    RTKBT_AUTOPAIR_CheckScanDelay       = 10000;   // N*ms
    private static final int    RTKBT_AUTOPAIR_MaxRcuNum            = 2;   // 0: unlimited
    private static int          RTKBT_AUTOPAIR_SignalThreshold      = 42;   // db, 10cm txpower - rssi
    private static final int    RTKBT_AUTOPAIR_Event_ClassicCreateBondResult_RETRY = 0;      //should not be same as BluetoothDevice.BOND_BONDED,BluetoothDevice.BOND_BONDING,BluetoothDevice.BOND_NONE
    private static int          RTKBT_AUTOPAIR_Service_Retry_Num    = 0;

/*********************************************************************************************************
        autopair storage properties
**********************************************************************************************************/
    private static final String RTKBT_AUTOPAIR_PropPrefix               = "RCU_";
    private static final String RTKBT_AUTOPAIR_PropStage                = "stage";
    private static final String RTKBT_AUTOPAIR_PropName                 = ".name";
    private static final String RTKBT_AUTOPAIR_PropBLEAddress           = ".ble_address";
    private static final String RTKBT_AUTOPAIR_PropClassicAddress       = ".classic_address";

/*********************************************************************************************************
        autopair message ID for APP
**********************************************************************************************************/
    private static final String RTKBT_AUTOPAIR_MESSAGE = "com.realtek.autopair.message";
    private static final String RTKBT_AUTOPAIR_MESSAGE_FIELD_RCUADDR = "com.realtek.autopair.message.rcuaddr";
    private static final String RTKBT_AUTOPAIR_MESSAGE_FIELD_MESSAGE_ID = "com.realtek.autopair.message.id";
    private static final String RTKBT_AUTOPAIR_MESSAGE_FIELD_DATA = "com.realtek.autopair.message.data";
    public  static final String BLUETOOTH_PERM = android.Manifest.permission.BLUETOOTH;



/*********************************************************************************************************
**      handler messages
**********************************************************************************************************/

    private Handler mHandler = new Handler()
    {
        public void handleMessage(Message msg) {
        info("handleMessage:"+msg.what +" RTKBT_AUTOPAIR_State: " + RTKBT_AUTOPAIR_State);
            switch (msg.what) {
                case RTKBT_AUTOPAIR_Event_FoundRCU:
                    if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_Found)
                        return;
                    notifyUser(R.string.found,RTKBT_AUTOPAIR_ble_device.toString());
                    removeOldRCU(RTKBT_AUTOPAIR_ble_device.getAddress(), false);
                    RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_RemoveBond;
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_RemoveBond);
                    sendMessageDelayed(obtainMessage(RTKBT_AUTOPAIR_Event_PairTimeOut,RTKBT_AUTOPAIR_State,0),30000);
                    RTKBT_AUTOPAIR_pending_device = RTKBT_AUTOPAIR_ble_device;
                    if(RTKBT_AUTOPAIR_pending_device.getBondState() != BluetoothDevice.BOND_NONE)
                    {
                        info(RTKBT_AUTOPAIR_pending_device+":removeBond\n");
                        removeBond(RTKBT_AUTOPAIR_pending_device,0);
                    }else
                        sendMessage(obtainMessage(RTKBT_AUTOPAIR_EVENT_BLERemoveBondResult,0,0));
                    break;
                case RTKBT_AUTOPAIR_EVENT_BLERemoveBondResult:
                    info(RTKBT_AUTOPAIR_pending_device+":removeBond return "+msg.arg2+"\n");
                    RTKBT_AUTOPAIR_pending_device = RTKBT_AUTOPAIR_classic_device;
                    if((RTKBT_AUTOPAIR_classic_device != null)&&(RTKBT_AUTOPAIR_pending_device.getBondState() != BluetoothDevice.BOND_NONE))
                    {
                        info(RTKBT_AUTOPAIR_pending_device+":removeBond\n");
                        removeBond(RTKBT_AUTOPAIR_pending_device,0);
                    }else {
                        try {
                            Thread.sleep(msg.arg2);
                            }catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_ClassicRemoveBondResult));
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_ClassicRemoveBondResult:
                    removeMessages(RTKBT_AUTOPAIR_Event_PairTimeOut);
                    info(RTKBT_AUTOPAIR_pending_device+":removeBond return "+msg.arg1+"\n");
                    sendMessageDelayed(obtainMessage(RTKBT_AUTOPAIR_Event_PairTimeOut,RTKBT_AUTOPAIR_State,0),30000);
                    RTKBT_AUTOPAIR_pending_device = RTKBT_AUTOPAIR_ble_device;
                    RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_BLEBond;
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_BLEBond);
                    info(RTKBT_AUTOPAIR_pending_device+":createBond\n");
                    RTKBT_AUTOPAIR_pending_device.createBond(BluetoothDevice.TRANSPORT_LE);
                    break;
                case RTKBT_AUTOPAIR_Event_BLECreateBondResult:
                    switch(msg.arg1)
                    {
                        case BluetoothDevice.BOND_BONDED:
                            removeMessages(RTKBT_AUTOPAIR_Event_PairTimeOut);
                            if(RTKBT_AUTOPAIR_classic_device != null)
                            {
                                notifyUser(R.string.ble_pair_success);
                                RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_ClassicBond;
                                notifyAutoPairState(RTKBT_AUTOPAIR_State_ClassicBond);
                                sendMessageDelayed(obtainMessage(RTKBT_AUTOPAIR_Event_PairTimeOut,RTKBT_AUTOPAIR_State,0),90000);
                                RTKBT_AUTOPAIR_pending_device = RTKBT_AUTOPAIR_classic_device;
                                info(RTKBT_AUTOPAIR_pending_device+":createBond\n");
                                RTKBT_AUTOPAIR_pending_device.createBond(BluetoothDevice.TRANSPORT_BREDR);
                            }else{
                                sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_PairSuccess));
                            }
                            break;
                        case BluetoothDevice.BOND_NONE:
                        case BluetoothDevice.ERROR:
                            notifyUser(R.string.ble_pair_fail);
                            info(RTKBT_AUTOPAIR_pending_device+":createBond fail("+msg.arg1+")\n");
                            sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_PairFail));
                            break;
                        case RTKBT_AUTOPAIR_Event_ClassicCreateBondResult_RETRY:
                            info(RTKBT_AUTOPAIR_pending_device+":createBond Retry\n");
                            removeBond(RTKBT_AUTOPAIR_pending_device,5000);
                            RTKBT_AUTOPAIR_pending_device.createBond(BluetoothDevice.TRANSPORT_BREDR);
                        default:
                            break;
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_ClassicCreateBondResult:
                    switch(msg.arg1)
                    {
                        case BluetoothDevice.BOND_BONDED:
                            notifyUser(R.string.classic_pair_success);
                            sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_PairSuccess));
                            break;
                        case BluetoothDevice.BOND_NONE:
                        case BluetoothDevice.ERROR:
                            notifyUser(R.string.classic_pair_fail);
                            info(RTKBT_AUTOPAIR_pending_device+":createBond fail("+msg.arg1+")\n");
//                                              sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_PairFail));
                            sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_BLECreateBondResult,RTKBT_AUTOPAIR_Event_ClassicCreateBondResult_RETRY,0));
                            break;
                        default:
                            break;
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_DeviceConnectResult:
                    switch(msg.arg1)
                    {
                        case BluetoothProfile.STATE_DISCONNECTED:
                            info(RTKBT_AUTOPAIR_pending_device + ":connect fail \n");
                            sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_PairFail));
                            notifyAutoPairState(RTKBT_AUTOPAIR_State_ConnectFail);
                            break;

                        case BluetoothProfile.STATE_CONNECTING:
                            notifyUser(R.string.connect_to_device);
                            break;

                        case BluetoothProfile.STATE_CONNECTED:
                            info(RTKBT_AUTOPAIR_pending_device + ":connect success \n");
                            RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_ConnectSuccess;
                            notifyUser(R.string.over);
                            notifyAutoPairState(RTKBT_AUTOPAIR_State_ConnectSuccess);
                            sendMessage(obtainMessage(RTKBT_AUTOPAIR_Event_SaveNewRCU));
                            new Thread((new Runnable()
                            {
                                @Override
                                public void run()
                                {
                                    try {
                                        Runtime.getRuntime().exec("sync");
                                        info("sync successfully.");
                                    }catch(Exception ex) {
                                        ex.printStackTrace();
                                    }
                                }
                            })).start();
                            if(!RTKBT_AUTOPAIR_auto_scan) {
                                stopSelf();
                            }
                            break;

                        default:
                            break;
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_SaveNewRCU:
                    int RTKBT_AUTOPAIR_Prev_State = RTKBT_AUTOPAIR_State;
                    RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_SaveNewRCU;
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_SaveNewRCU);
                    saveNewRCU(RTKBT_AUTOPAIR_ble_device, RTKBT_AUTOPAIR_classic_device);
                    //RTKBT_AUTOPAIR_State_ConnectSuccess is the end state, if this state chage to RTKBT_AUTOPAIR_Event_SaveNewRCU,
                    //it means hogp profile is connected success!
                    info("RTKBT_AUTOPAIR_Prev_State" + RTKBT_AUTOPAIR_Prev_State);
                    if(RTKBT_AUTOPAIR_Prev_State == RTKBT_AUTOPAIR_State_ConnectSuccess) {
                        reset();
                        releasePendingRCU();
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_PairSuccess:
                    RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_PairSuccess_to_connect;
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_PairSuccess_to_connect);

                    if(RTKBT_has_hogp == true) {
                        mPairHidProfile.connect(RTKBT_AUTOPAIR_pending_device);
                        sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_PairTimeOut,RTKBT_AUTOPAIR_State,0),30000);
                    }
                    else {
                        sendMessage(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_PairTimeOut,RTKBT_AUTOPAIR_State,0));
                    }
                    checkAndRestartScan(false);
                    break;
                case RTKBT_AUTOPAIR_Event_PairFail:
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_PairFail);
                    if(RTKBT_AUTOPAIR_ble_device != null) {
                        removeBond(RTKBT_AUTOPAIR_ble_device,5000);
                    }
                    cleanupNewRCU(RTKBT_AUTOPAIR_ble_device, RTKBT_AUTOPAIR_classic_device);
                    releasePendingRCU();
                    reset();
                    checkAndRestartScan(false);
                    if(!RTKBT_AUTOPAIR_auto_scan) {
                        stopSelf();
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_PairTimeOut:
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_PairTimeout);
                    switch(msg.arg1)
                    {
                        case RTKBT_AUTOPAIR_State_RemoveBond:
                            notifyUser(R.string.removebond_timeout);
                            break;
                        case RTKBT_AUTOPAIR_State_BLEBond:
                            removeBond(RTKBT_AUTOPAIR_pending_device,5000);
                            notifyUser(R.string.ble_pair_timeout);
                            break;
                        case RTKBT_AUTOPAIR_State_ClassicBond:
                            removeBond(RTKBT_AUTOPAIR_pending_device,5000);
                            notifyUser(R.string.classic_pair_timeout);
                            break;
                        case RTKBT_AUTOPAIR_State_PairSuccess_to_connect:
                            removeBond(RTKBT_AUTOPAIR_pending_device,5000);
                            notifyUser(R.string.ble_pair_timeout);
                            break;
                        default:
                            break;
                    }
                    cleanupNewRCU(RTKBT_AUTOPAIR_ble_device, RTKBT_AUTOPAIR_classic_device);
                    releasePendingRCU();
                    reset();
                    checkAndRestartScan(false);
                    if(!RTKBT_AUTOPAIR_auto_scan) {
                        stopSelf();
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_CheckScanTimeOut:
                    //sendMessageDelayed(obtainMessage(RTKBT_AUTOPAIR_Event_CheckScanTimeOut),RTKBT_AUTOPAIR_CheckScanDelay);
                    //checkAndRestartDiscover();
                    //removeMessages(RTKBT_AUTOPAIR_Event_ScanInterval);
                    checkAndRestartScan(false);
                    RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_Idle;
                    notifyUser(R.string.auto_pair_scan_stop);
                    if(!RTKBT_AUTOPAIR_auto_scan) {
                        stopSelf();
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_StartScan:
                    checkAndRestartScan(true);
                    break;
                case RTKBT_AUTOPAIR_Event_weak_signal:
                    {
                    notifyUser(R.string.error_weak_signal);
                    }
                    break;

                case RTKBT_AUTOPAIR_Event_ScanInterval:
                    {
                        if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_Idle)
                            return;
                        checkAndRestartScan(!RTKBT_IS_scan);
                        if(!RTKBT_IS_scan)
                            mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_ScanInterval),3000);
                    }
                    break;
                case RTKBT_AUTOPAIR_Event_CheckServiceStart:
                    {
                        if(!mPairProfile.isProfileReady() || mPairProfile.GetFeature(BluetoothRtkbt.RTKBT_ID_AUTOPAIR) == 0) {
                            info("AutoPair profile is not ready, stop autopair service, retry: " + RTKBT_AUTOPAIR_Service_Retry_Num);
                            if(RTKBT_AUTOPAIR_Service_Retry_Num < RTK_SERVICE_RETRY_NUM) {
                                RTKBT_AUTOPAIR_Service_Retry_Num++;
                                mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_CheckServiceStart), 200);
                            } else {
                                stopSelf();
                            }
                        }else {
                            if(mLoadConfig.autopair_rssi != 0)
                                RTKBT_AUTOPAIR_SignalThreshold =  mLoadConfig.autopair_rssi;
                            if((byte)(mLoadConfig.autopair_type & (byte)BluetoothRTKAutoPairLoadConfig.vendor_event_pair) == (byte)BluetoothRTKAutoPairLoadConfig.vendor_event_pair) {
                                RTKBT_AUTOPAIR_vendor_event_arry = generate_vendor_cmd_arry();
                                print_vendor_event();

                                RTKBT_AUTOPAIR_vendor_event_arry[0] = 0x01;
                                if(mPairProfile.GenericCommand(RTKBT_AUTOPAIR_vendor_event_arry,RTKBT_AUTOPAIR_vendor_event_arry.length) == 1)
                                    RTKBT_AUTOPAIR_auto_scan = true;
                                if((mLoadConfig.autopair_type & (byte)BluetoothRTKAutoPairLoadConfig.key_pair) == (byte)BluetoothRTKAutoPairLoadConfig.key_pair) {
                                    make_random_num();
                                }
                            }else {
                                if((mLoadConfig.autopair_type & (byte)BluetoothRTKAutoPairLoadConfig.key_pair) == (byte)BluetoothRTKAutoPairLoadConfig.key_pair) {
                                    RTKBT_AUTOPAIR_vendor_event_arry = generate_vendor_cmd_arry();
                                    make_random_num();
                                    print_vendor_event();
                                }
                                RTKBT_AUTOPAIR_auto_scan = false;
                                checkAndRestartScan(true);
                            }
                            if(mLoadConfig.service_timeout != 0)
                                mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_ServiceTimeOut),mLoadConfig.service_timeout);
                        }

                    }
                    break;
                case RTKBT_AUTOPAIR_Event_ServiceTimeOut:
                    if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_Idle || RTKBT_IS_scan) {
                        mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_ServiceTimeOut),5000);
                    }
                    else {
                        stopSelf();
                    }
                    break;
                default:
                    info("AutoPair profile unknown message!");
                    break;
            }
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();
        log("<===AutoPair===>+++ onCreate version +++"+ verison);

        mReveiverRegister               = false;
        RTKBT_AUTOPAIR_ble_device       = null;
        RTKBT_AUTOPAIR_classic_device   = null;
        RTKBT_AUTOPAIR_pending_device   = null;
    }

    public int startServiceForeground(Intent intent, int flags, int startId) {
        /*Notification notification = new Notification(R.drawable.ic_bluetooth, "RTKBT AutoPair Process"
                        , System.currentTimeMillis());
        notification.setLatestEventInfo(this, "Realtek Bluetooth",
                "AutoPair Process", null);
        notification.flags = Notification.FLAG_SHOW_LIGHTS;*/

	/* bpi, fix setLatestEventInfo() is deprecated */
	Notification notification = new NotificationCompat.Builder(this)
		.setSmallIcon(R.drawable.ic_bluetooth)
		.setContentTitle("Realtek Bluetooth")
		.setContentText("AutoPair Process")
		.setDefaults(Notification.DEFAULT_SOUND)
		.setWhen(System.currentTimeMillis())
		.build();

        startForeground(1, notification);

        return START_STICKY;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        info("<===AutoPair===>++++onStartCommand++++"+ verison);

        int retCode = super.onStartCommand(intent, flags, startId);
        RTKBT_AUTOPAIR_BluetoothAadapter = BluetoothAdapter.getDefaultAdapter();
        RTKBT_AUTOPAIR_scanner = RTKBT_AUTOPAIR_BluetoothAadapter.getBluetoothLeScanner();
        mLoadConfig = BluetoothRTKAutoPairLoadConfig.getInstance();
        if(null == RTKBT_AUTOPAIR_scanner) {
            mLoadConfig.service_start = false;
            info("RTKBT_AUTOPAIR_scanner is null");
            RTKBT_AUTOPAIR_BluetoothAadapter.isLeEnabled();
        }
        else {
            mPairProfile = new BluetoothRTKAutoPairProfile(this);
            mPairHidProfile = new BluetoothRTKAutoPairHidProfile(this);
            startServiceForeground(intent,flags,startId);
            notifyUser("AutoPair start");
        if(!mLoadConfig.service_start) {
            mLoadConfig.do_config();
            mLoadConfig.service_start = true;
        }
        RTKBT_AUTOPAIR_notify_counter = 0;
        cleanupPendingRCU();
        registerBroadCastReceiver();
        RTKBT_AUTOPAIR_Service_Retry_Num = 0;
        mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_CheckServiceStart), 200);
        }
        return retCode;
    }

    public IBinder onBind(Intent intent) {
        // not implement
        log("<===AutoPair===>onBind");
        return null;
    }

    @Override
    public void onDestroy() {
        info("<===AutoPair===>onDestroy");
        super.onDestroy();
        stopForeground(true);

        if(mReveiverRegister) {
            unregisterReceiver(mReceiver);
        }
        if(RTKBT_AUTOPAIR_auto_scan && RTKBT_AUTOPAIR_BluetoothAadapter.getState() == BluetoothAdapter.STATE_ON) {
            RTKBT_AUTOPAIR_vendor_event_arry[0] = (byte)0xff;
            mPairProfile.GenericCommand(RTKBT_AUTOPAIR_vendor_event_arry,RTKBT_AUTOPAIR_vendor_event_arry.length);
        }
        checkAndRestartScan(false);
        mLoadConfig.service_start   = false;
        RTKBT_AUTOPAIR_auto_scan    = false;
        if(RTKBT_AUTOPAIR_State == RTKBT_AUTOPAIR_State_BLEBond){
            if(RTKBT_AUTOPAIR_pending_device.getBondState() != BluetoothDevice.BOND_NONE)
            {
                info(RTKBT_AUTOPAIR_pending_device+":removeBond\n");
                removeBond(RTKBT_AUTOPAIR_pending_device,0);
            }
        }
        reset();
        mHandler.removeCallbacksAndMessages(null);

        if(mPairProfile != null) {
            mPairProfile.finalize();
            mPairProfile = null;
        }
        if(mPairHidProfile != null) {
            mPairHidProfile.finalize();
            mPairHidProfile = null;
        }
    }

    /***************************************************************************
     * registerBroadCastReceiver is used for register receiver device_found event
     ****************************************************************************/
    private void registerBroadCastReceiver() {
        log("<===AutoPair===>registerBroadCastReceiver");
        mIntentFilter = new IntentFilter();
        //mIntentFilter.addAction(BluetoothDevice.ACTION_FOUND);
        mIntentFilter.addAction(BluetoothDevice.ACTION_BOND_STATE_CHANGED);
        mIntentFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_STARTED);
        mIntentFilter.addAction(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        mIntentFilter.addAction(BluetoothDevice.ACTION_UUID);
        mIntentFilter.addAction(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT);
        mIntentFilter.addAction(BluetoothInputDevice.ACTION_CONNECTION_STATE_CHANGED);

        mIntentFilter.addAction(RTKBT_AUTOPAIR_MESSAGE);

        registerReceiver(mReceiver, mIntentFilter);
        mReveiverRegister = true;
    }

    private final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            Log.d(TAG,"<===AutoPair===>BroadcastReceive onReceive("+action+")");
            if (action.equals(RTKBT_AUTOPAIR_MESSAGE)){
                Log.d(TAG, "<===AutoPair===>RTKBT_AUTOPAIR_MESSAGE: ID:"+intent.getIntExtra(RTKBT_AUTOPAIR_MESSAGE_FIELD_MESSAGE_ID, 0)+" ADDR:"+intent.getStringExtra(RTKBT_AUTOPAIR_MESSAGE_FIELD_RCUADDR)+" DATA:"+intent.getByteArrayExtra(RTKBT_AUTOPAIR_MESSAGE_FIELD_DATA));
            }

            if (action.equals(BluetoothDevice.ACTION_BOND_STATE_CHANGED)){
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                int bondState = intent.getIntExtra(BluetoothDevice.EXTRA_BOND_STATE,
                                               BluetoothDevice.ERROR);

                if(RTKBT_AUTOPAIR_pending_device == null){
                    info("RTKBT_AUTOPAIR_pending_device is null return!\n");
                    return;
                }
                if((compare(device, RTKBT_AUTOPAIR_pending_device) != 0) || (RTKBT_AUTOPAIR_BluetoothAadapter.getState() != BluetoothAdapter.STATE_ON))
                    return;
                info(RTKBT_AUTOPAIR_pending_device+":Pair Status("+bondState+")\n");
                if(RTKBT_AUTOPAIR_pending_device == RTKBT_AUTOPAIR_ble_device)
                    mHandler.sendMessage(mHandler.obtainMessage((RTKBT_AUTOPAIR_State == RTKBT_AUTOPAIR_State_BLEBond)?RTKBT_AUTOPAIR_Event_BLECreateBondResult:RTKBT_AUTOPAIR_EVENT_BLERemoveBondResult, bondState, 1000));
                if(RTKBT_AUTOPAIR_pending_device == RTKBT_AUTOPAIR_classic_device)
                    mHandler.sendMessage(mHandler.obtainMessage((RTKBT_AUTOPAIR_State == RTKBT_AUTOPAIR_State_ClassicBond)?RTKBT_AUTOPAIR_Event_ClassicCreateBondResult:RTKBT_AUTOPAIR_Event_ClassicRemoveBondResult, bondState, 0));
                return;
            }
            if (action.equals(BluetoothAdapter.ACTION_DISCOVERY_STARTED)) {
                info("Start Scanning...");
            }
            if (action.equals(BluetoothAdapter.ACTION_DISCOVERY_FINISHED)) {
                RTKBT_AUTOPAIR_notify_counter = 0;
                if((RTKBT_AUTOPAIR_State/RTKBT_AUTOPAIR_State_Cancel_inqury_4_scan) != 0) {
                    RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_Idle;
                    notifyAutoPairState(RTKBT_AUTOPAIR_State_Idle);
                    //checkAndRestartScan(true);
                }
            }
            if (action.equals(BluetoothDevice.ACTION_UUID)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                if(compare(device, RTKBT_AUTOPAIR_pending_device) != 0)
                    return;
                ParcelUuid[] uuids = device.getUuids();
                if((uuids == null) || (device == null) || (RTKBT_AUTOPAIR_pending_device == null)){
                    if(uuids == null)
                        info("uuid is null! return\n");
                    if(device == null)
                        info("device is null! return\n");
                    if(RTKBT_AUTOPAIR_pending_device == null)
                        info("RTKBT_AUTOPAIR_pending_device is null! return\n");
                        return;
                }
                info(RTKBT_AUTOPAIR_pending_device+":Sdp or Gatt search success("+uuids.length+")\n");
                for (ParcelUuid uuid : uuids) {
                    info(RTKBT_AUTOPAIR_pending_device+":Service="+uuid+"\n");
                    if(uuid.equals(BluetoothUuid.Hogp) && mPairHidProfile.isProfileReady()){
                        info("have hogp service RTKBT_AUTOPAIR_State = " + RTKBT_AUTOPAIR_State +"\n");
                        RTKBT_has_hogp = true;
                        break;
                    }
                }
                return;
            }
            if(action.equals(BluetoothInputDevice.ACTION_CONNECTION_STATE_CHANGED))
            {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                int new_state = intent.getIntExtra(BluetoothProfile.EXTRA_STATE,-1);
                int pre_state = intent.getIntExtra(BluetoothProfile.EXTRA_PREVIOUS_STATE,-1);
                if(compare(device, RTKBT_AUTOPAIR_pending_device) != 0 || RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_PairSuccess_to_connect)
                    return;
                mHandler.sendMessage(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_DeviceConnectResult,new_state, pre_state));
            }
            if(action.equals(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT))
            {
                int id = intent.getIntExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_id, 0);
                info("Receive BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT, id is : " + id +"RTKBT_AUTOPAIR_State:"+ RTKBT_AUTOPAIR_State);
                if(id == BluetoothRtkbt.RTKBT_ID_AUTOPAIR && RTKBT_AUTOPAIR_State == RTKBT_AUTOPAIR_State_Idle) {
                    byte[] data = intent.getByteArrayExtra(BluetoothRtkbt.ACTION_RTKBT_GENERIC_EVENT_data);
                    BluetoothDevice rcu = getRcuFromData(data);
                    info("RCU="+rcu);
                    Set<BluetoothDevice> BondedDevices = RTKBT_AUTOPAIR_BluetoothAadapter.getBondedDevices();
                    for(int i=0; i<BondedDevices.size(); i++)
                    {
                        BluetoothDevice device = (BluetoothDevice) BondedDevices.iterator().next();
                        info("Bonded device["+i+"]:"+device);
                        if(compare(device,rcu) == 0 && rcu.getBondState() == BluetoothDevice.BOND_BONDED && checkRCUisSaved(rcu))
                        {
                            info("RCU["+rcu+"]has connected");
                            return;
                        }
                    }
                    checkAndRestartScan(true);
                }
            }
        }
    };
    private BluetoothDevice getRcuFromData(byte[] array){
        byte[] address = new byte[6];
        for(int i=8; i>2; i--)
            address[8-i] = array[i];
        return RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(address);
    }

/******************************************************************************************************
    ** function : onLeScanResult
                  scanCallback
                  settings
                  filters
    ** API from BluetoothLeScanner
*******************************************************************************************************/
    private void onLeScanResult(BluetoothDevice device, int rssi, ScanRecord scanRecord){
        info("mLeScanCallback : "+device.getAddress());
        if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_BLEScan)
            return;
        if(create_bond_by_type(device, scanRecord, RTKBT_AUTOPAIR_auto_scan)) {
            check_to_create_bond_by_rssi(device, rssi);
        }
    }

    private ScanSettings settings = new ScanSettings.Builder()
                .setCallbackType(ScanSettings.CALLBACK_TYPE_ALL_MATCHES)
                .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY).build();

    private ScanCallback scanCallback = new ScanCallback() {
                @Override
                public void onScanResult(int callbackType, ScanResult result) {
                    if (callbackType != ScanSettings.CALLBACK_TYPE_ALL_MATCHES) {
                        // Should not happen.
                        Log.e(TAG, "LE Scan has already started");
                        return;
                    }
                    ScanRecord scanRecord = result.getScanRecord();
                    if (scanRecord == null) {
                        return;
                    }
                    onLeScanResult(result.getDevice(), result.getRssi(),
                                    scanRecord);
                }
                @Override
                public void onScanFailed(int errorCode) {
                    RTKBT_IS_scan = false;
                }

            };
    private List<ScanFilter> filters = new ArrayList<ScanFilter>();

/******************************************************************************************************
        ** function : compare

        ** description: compare two bluetoothdevice
*******************************************************************************************************/
    private int compare(BluetoothDevice src, BluetoothDevice dst)
    {
        if((src == null)||(dst == null))
            return -1;
        if(src == dst)
            return 0;
        if(src.getAddress().equals(dst.getAddress()) == false)
            return -1;
        if(src.getType() != dst.getType())
            return -1;
        return 0;
    }
    private synchronized int checkAndRestartDiscover()
    {
        if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_Idle)
            return -1;
        if(RTKBT_AUTOPAIR_BluetoothAadapter.isDiscovering())
            return -1;
        info("checkAndRestartDiscover startDiscovery");
        RTKBT_AUTOPAIR_BluetoothAadapter.startDiscovery();
        return 0;
    }

/************************************************************************************************
        **      function :  checkAndRestartScan
        **      description: start Le scan
        **      args     :  enable true: start scan, false : stop scan
*************************************************************************************************/
/************************************************************************************************
    private synchronized int checkAndRestartScan( boolean enable)
    {
        info("checkAndRestartDiscover start scan"+ enable);
        if(!enable) {
            if(RTKBT_AUTOPAIR_BluetoothAadapter.isDiscovering()){
                RTKBT_AUTOPAIR_BluetoothAadapter.cancelDiscovery();
            }
            else {
                if(RTKBT_IS_scan)
                    RTKBT_AUTOPAIR_BluetoothAadapter.stopLeScan(mLeScanCallback);
            }
            RTKBT_IS_scan = false;
            return -1;
        }
        if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_Idle)
            return -1;
        if(RTKBT_AUTOPAIR_BluetoothAadapter.isDiscovering()) {
            RTKBT_AUTOPAIR_BluetoothAadapter.cancelDiscovery();
            return -1;
        }
        if(RTKBT_IS_scan == false) {
            RTKBT_IS_scan = RTKBT_AUTOPAIR_BluetoothAadapter.startLeScan(mLeScanCallback);
            info("checkAndRestartDiscover start scan : " + RTKBT_IS_scan);
            //mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_ScanInterval),5000);
            return 0;
        }
        else
            return -1;
    }
***************************************************************************************************************/

/************************************************************************************************
            **      function :  checkAndRestartScan
            **      description: start Le scan
            **      args     :  enable true: start scan, false : stop scan
*************************************************************************************************/
    private synchronized int checkAndRestartScan( boolean enable)
    {
        info("checkAndRestartScan start scan: "+ enable);
        if(!enable) {
            if(RTKBT_AUTOPAIR_BluetoothAadapter.isDiscovering()){
                RTKBT_AUTOPAIR_BluetoothAadapter.cancelDiscovery();
            }
            else {
                if(RTKBT_IS_scan) {
                    if(RTKBT_AUTOPAIR_BluetoothAadapter.getState() == BluetoothAdapter.STATE_ON)
                        RTKBT_AUTOPAIR_scanner.stopScan(scanCallback);
                    else
                        RTKBT_AUTOPAIR_scanner.cleanup();

                    mHandler.removeMessages(RTKBT_AUTOPAIR_Event_CheckScanTimeOut);
                }
            }
            RTKBT_IS_scan = false;
            info("checkAndRestartScan RTKBT_AUTOPAIR_State:"+RTKBT_AUTOPAIR_State+" ==> RTKBT_AUTOPAIR_State_Idle");
            return -1;
        }
        if(RTKBT_AUTOPAIR_State != RTKBT_AUTOPAIR_State_Idle)
            return -1;
        if(RTKBT_AUTOPAIR_BluetoothAadapter.isDiscovering()) {
            RTKBT_AUTOPAIR_BluetoothAadapter.cancelDiscovery();
            RTKBT_AUTOPAIR_State += RTKBT_AUTOPAIR_State_Cancel_inqury_4_scan;
            return -1;
        }
        if(RTKBT_IS_scan == false) {
            RTKBT_AUTOPAIR_scanner.startScan(filters, settings, scanCallback);
            RTKBT_IS_scan = true;
            info("checkAndRestartScan RTKBT_AUTOPAIR_State:"+RTKBT_AUTOPAIR_State+" ==> RTKBT_AUTOPAIR_State_BLEScan");
            RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_BLEScan;
            info("checkAndRestartScan real start scan : " + RTKBT_IS_scan);
            //mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_ScanInterval),5000);
            mHandler.sendMessageDelayed(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_CheckScanTimeOut),12000);
            return 0;
        }
        else
            return -1;
    }


    private int holdPendingRCU(BluetoothDevice ble_device, BluetoothDevice classic_device)
    {
        int i,num;
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        info("holdPendingRCU BLE:"+ble_device.getAddress());
        if(classic_device != null)
            info("holdPendingRCU CLASSIC:"+classic_device.getAddress());

        editor.putString(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropName, ble_device.getName());
        editor.putString(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropBLEAddress, ble_device.getAddress());
        if(classic_device != null)
            editor.putString(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropClassicAddress, classic_device.getAddress());
        else
            editor.remove(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropClassicAddress);

        editor.commit();
        return 0;
    }

    private int releasePendingRCU()
    {
        int i,num;
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        info("releasePendingRCU");

        editor.remove(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropName);
        editor.remove(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropBLEAddress);
        editor.remove(RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropClassicAddress);
        editor.commit();
        return 0;
    }

    private int cleanupPendingRCU()
    {
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);
        String PropBLEAddress = RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropBLEAddress;
        String PropClassicAddress = RTKBT_AUTOPAIR_PropPrefix+RTKBT_AUTOPAIR_PropStage+RTKBT_AUTOPAIR_PropClassicAddress;

        info("cleanupPendingRCU");

        if(preferences.contains(PropClassicAddress))
        {
            info("cleanupPendingRCU ClassicAddress:"+preferences.getString(PropClassicAddress,""));
            removeBond(RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(preferences.getString(PropClassicAddress,"")),5000);
        }
        if(preferences.contains(PropBLEAddress))
        {
            info("cleanupPendingRCU BLEAddress:"+preferences.getString(PropBLEAddress,""));
            removeBond(RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(preferences.getString(PropBLEAddress,"")),5000);
        }
        releasePendingRCU();
        return 0;
    }

    private int saveNewRCU(BluetoothDevice ble_device, BluetoothDevice classic_device)
    {
        int i,num;
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        if(classic_device != null)
            displayAllRCU("saveNewRCU_1("+ble_device.getAddress()+","+classic_device.getAddress()+")");
        else
            displayAllRCU("saveNewRCU_1("+ble_device.getAddress()+",null)");

        editor.putString(RTKBT_AUTOPAIR_PropPrefix+ble_device.getAddress()+RTKBT_AUTOPAIR_PropName, ble_device.getName());
        editor.putString(RTKBT_AUTOPAIR_PropPrefix+ble_device.getAddress()+RTKBT_AUTOPAIR_PropBLEAddress, ble_device.getAddress());
        if(classic_device != null)
            editor.putString(RTKBT_AUTOPAIR_PropPrefix+ble_device.getAddress()+RTKBT_AUTOPAIR_PropClassicAddress, classic_device.getAddress());
        else
            editor.remove(RTKBT_AUTOPAIR_PropPrefix+ble_device.getAddress()+RTKBT_AUTOPAIR_PropClassicAddress);

        num = preferences.getInt("NUM", 0);
        for(i = 0;i<num;i++)
        {
            if(preferences.getString(RTKBT_AUTOPAIR_PropPrefix+i, "").equals(ble_device.getAddress()))
                break;
        }
        if(i<num)
        {
            for(;i<(num-1);i++)
                editor.putString(RTKBT_AUTOPAIR_PropPrefix+i,preferences.getString(RTKBT_AUTOPAIR_PropPrefix+(i+1), ""));
        }
        editor.putString(RTKBT_AUTOPAIR_PropPrefix+i, ble_device.getAddress());
        num = i+1;
        editor.putInt("NUM", num);
        editor.commit();

        if((RTKBT_AUTOPAIR_MaxRcuNum != 0)&&(num > RTKBT_AUTOPAIR_MaxRcuNum))
        {
            removeOldRCU(preferences.getString(RTKBT_AUTOPAIR_PropPrefix+0, ""), true);
        }
        if(classic_device != null)
            displayAllRCU("saveNewRCU_2("+ble_device.getAddress()+","+classic_device.getAddress()+")");
        else
            displayAllRCU("saveNewRCU_2("+ble_device.getAddress()+",null)");
        return 0;
    }
    private boolean checkRCUisSaved(BluetoothDevice ble_device)
    {
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);
        int num = preferences.getInt("NUM",0);
        int i;
        for(i = 0; i < num; i++)
        {
            info("SavedRCU["+i+"]="+preferences.getString(RTKBT_AUTOPAIR_PropPrefix+i, ""));
            if(preferences.getString(RTKBT_AUTOPAIR_PropPrefix+i, "").equals(ble_device.getAddress()))
                return true;
        }
        return false;
    }
    private void displayAllRCU(String prefix)
    {
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);
        int num = preferences.getInt("NUM",0);
        int i;
        info(prefix+" NUM="+num);
        for(i = 0;i < num;i++)
        {
            String rcu = preferences.getString(RTKBT_AUTOPAIR_PropPrefix+i,"");
            info(prefix+" RCU["+i+"]="+rcu+" "+preferences.getString(RTKBT_AUTOPAIR_PropPrefix+rcu+RTKBT_AUTOPAIR_PropBLEAddress,"")+"-"+preferences.getString(RTKBT_AUTOPAIR_PropPrefix+rcu+RTKBT_AUTOPAIR_PropClassicAddress,""));
        }
    }
    private int cleanupNewRCU(BluetoothDevice ble_device, BluetoothDevice classic_device)
    {
        if(classic_device != null && ble_device != null)
            info("cleanupNewRCU "+ble_device.getAddress()+" "+classic_device.getAddress());
        else if(ble_device != null)
            info("cleanupNewRCU "+ble_device.getAddress());
        if(classic_device != null){
            removeBond(classic_device,5000);
        }
        if(ble_device != null){
            removeBond(ble_device,5000);
        }
        return 0;
    }
    private int removeOldRCU(String addr,boolean removebond)
    {
        int i, num;
        SharedPreferences preferences = getSharedPreferences("RCU",Context.MODE_PRIVATE);;
        SharedPreferences.Editor editor = preferences.edit();
        String PropClassicAddress = RTKBT_AUTOPAIR_PropPrefix+addr+RTKBT_AUTOPAIR_PropClassicAddress;
        String PropBLEAddress = RTKBT_AUTOPAIR_PropPrefix+addr+RTKBT_AUTOPAIR_PropBLEAddress;
        String PropName = RTKBT_AUTOPAIR_PropPrefix+addr+RTKBT_AUTOPAIR_PropName;

        if(!RTKBT_AUTOPAIR_BluetoothAadapter.checkBluetoothAddress(addr))
            return 0;
        info("removeOldRCU_1 "+addr);
        if(removebond)
        {
            info("removeOldRCU_2 "+addr);

            if(preferences.contains(PropClassicAddress))
            {
                info("removeOldRCU_3A "+addr+" ClassicAddress:"+preferences.getString(PropClassicAddress,""));
                RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(preferences.getString(PropClassicAddress,"")).removeBond();
            }
            if(preferences.contains(PropBLEAddress))
            {
                info("removeOldRCU_3A "+addr+" BLEAddress:"+preferences.getString(PropBLEAddress,""));
                RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(preferences.getString(PropBLEAddress,"")).removeBond();
            }
/*
            for(BluetoothDevice device : RTKBT_AUTOPAIR_BluetoothAadapter.getBondedDevices())
            {
                if(preferences.contains(PropClassicAddress) && preferences.getString(PropClassicAddress,"").equals(device.getAddress()))
                {
                    info("removeOldRCU_3B "+addr+" ClassicAddress:"+preferences.getString(PropClassicAddress,""));
                    device.removeBond();
                }
                if(preferences.contains(PropBLEAddress) && preferences.getString(PropBLEAddress,"").equals(device.getAddress()))
                {
                    info("removeOldRCU_3B "+addr+" BLEAddress:"+preferences.getString(PropBLEAddress,""));
                    device.removeBond();
                }
            }
*/
        }

        editor.remove(PropClassicAddress);
        editor.remove(PropBLEAddress);
        editor.remove(PropName);

        num = preferences.getInt("NUM", 0);

        for(i = 0;i<num;i++)
        {
            if(preferences.getString(RTKBT_AUTOPAIR_PropPrefix+i, "").equals(addr))
                break;
        }
        if(i<num)
        {
            for(;i<(num-1);i++)
                editor.putString(RTKBT_AUTOPAIR_PropPrefix+i,preferences.getString(RTKBT_AUTOPAIR_PropPrefix+(i+1), ""));
            editor.remove(RTKBT_AUTOPAIR_PropPrefix+i);
            editor.putInt("NUM", i);
        }
        editor.commit();
        return 0;
    }

    private void reset()
    {
        mHandler.removeMessages(RTKBT_AUTOPAIR_Event_PairTimeOut);
        RTKBT_AUTOPAIR_pending_device       = null;
        RTKBT_AUTOPAIR_ble_device           = null;
        RTKBT_AUTOPAIR_classic_device       = null;
        RTKBT_AUTOPAIR_State                = RTKBT_AUTOPAIR_State_Idle;
        RTKBT_has_hogp                      = false;
    }

    private void removeBond(BluetoothDevice dev,int wait)
    {
        int itv = 200;
        int i = wait/itv;
        if(dev == null) {
            info("remove bond, bluetooth device is null!");
            return;
        }
        if(dev.getBondState() != BluetoothDevice.BOND_NONE) {
            info("removeBond:cancelBondProcess "+dev.getAddress());
            dev.cancelBondProcess();
            info("removeBond:removeBond "+dev.getAddress());
            dev.removeBond();
            if(wait <= 0)
            {
                info("removeBond:removeBond "+dev.getAddress()+" finished(wait=0)");
                return;
            }
            while((dev.getBondState() != BluetoothDevice.BOND_NONE)&&(i-->0))
            {
                try{
                Thread.sleep(itv);
                }catch(InterruptedException e){};
            }
            info("removeBond:removeBond "+dev.getAddress()+" finished");
        }
    }

/*******************************************************************************************************
    ** Function : generate_vendor_cmd_arry
    ** Description: according to vendor type to make specific cmd arry
                    get vendor_value and mask_value from xml, change the string to byte array
                    byte[0]~byte[6]are autopair scan parameter
                    01/00/00/00/00/00/00 is start auto scan
                    ff/00/00/00/00/00/00 is stop auto scan
   Parameter       Size (octets)       Description
   --------------------------------------------------------------------------
   ScanType            1               0x00: Passive scan
                                       0x01: Active scan
                                       0xFF: Disable
   --------------------------------------------------------------------------
   ScanInterval        2               Background LE scan interval (unit: 625 us)
   --------------------------------------------------------------------------
   ScanWindow          2               Background LE scan window (unit: 625 us)
   --------------------------------------------------------------------------
   OwnAddrType         1               0x00: Public device address
                                       0x01: Random device address
   --------------------------------------------------------------------------
   ScanFilterPolicy    1                   *******
   --------------------------------------------------------------------------
   Rcu Address Value   6               Rcu Address Value
   --------------------------------------------------------------------------
   Rcu Address Mask    6               Rcu Address Mask
   --------------------------------------------------------------------------
   CheckType           1               EIR type
   --------------------------------------------------------------------------
   Value_length        1               The length of data to check starting
                                       from the EIR data
   --------------------------------------------------------------------------
   Value               Value_length    value which is compaired by controller
                                       to make vendor event
   --------------------------------------------------------------------------
   Mask_value          Value_length    mask which is used by controller
   --------------------------------------------------------------------------

   ** return : byte[]
********************************************************************************************************/
    private byte[] generate_vendor_cmd_arry() {
        switch(mLoadConfig.vendor_type)
        {
            case BluetoothRTKAutoPairLoadConfig.address_pair:
            {
                return getBytesFromAddress(mLoadConfig.autoPairInfoMap.get("address_1"));
            }
            case BluetoothRTKAutoPairLoadConfig.name_pair:
            {
                return getBytesFromName(mLoadConfig.autoPairInfoMap.get("name_1"));
            }
            case BluetoothRTKAutoPairLoadConfig.manufactSpeciData_pair :
            {
                return getBytesFromManufactSpecidata(mLoadConfig.autopair_mfsd_str, mLoadConfig.autopair_mask_str,mLoadConfig.data_length);
            }
            default :
            break;
        }
        return new byte[21];
    }

    private byte[] getBytesFromAddress(String address_str) {
        int i, j = 0;
        byte[] output = new byte[21];
        for (i = 0; i < 7; i++) {
            output[j] = 0;
            j++;
        }
        if(address_str.length() > 17 || address_str.length() == 0) {
            for(i = 0; i < 12; i++) {
                output[j] = 0;
                j++;
            }
        }
        else {
            for (i = 0; i < address_str.length();i++) {
                if (address_str.charAt(i) != ':') {
                    output[7 + 12%j] = (byte) Integer.parseInt(address_str.substring(i, i+2), 16);
                    output[13 + 12%j] = (byte)0xFF;
                    j++;
                    i++;
                }
            }

        }
        return output;
    }

    private byte[] getBytesFromName(String name_str) {
        int i,j = 0;
        int length = name_str.length();
        byte[] name_byte = name_str.getBytes();
        byte[] output = new byte[21 + 2 * length];
        if((mLoadConfig.autopair_type & (byte)BluetoothRTKAutoPairLoadConfig.name_flag_pair)==(byte)BluetoothRTKAutoPairLoadConfig.name_flag_pair)
            output[19] = (byte)0x09;
        else
            output[19] = (byte)0x08;
        output[20] = (byte)length;
        System.arraycopy(name_byte, 0 , output, 21, length);
        for(i = 0; i < length; i ++) {
            output[21 + length + i] = (byte)0xFF;
        }
        return output;
    }

    private byte[] getBytesFromManufactSpecidata(String vendor_str, String mask_str, int data_length) {
        int i,j = 0;
        byte[] output = new byte[21 + 2 * data_length];
        output[19] = (byte)0xFF;
        output[20+j] = (byte)data_length;
        j++;
        if(data_length != vendor_str.length()) {
            info("data_length is wrong");
        }
        for (i = 0; i < vendor_str.length();i++) {
            if (vendor_str.charAt(i) != ':') {
                output[j + 20] = (byte) Integer.parseInt(vendor_str.substring(i, i+2), 16);
                j++;
                i++;
            }
        }
        for (i = 0; i < mask_str.length();i++) {
            if (mask_str.charAt(i) != ':') {
                output[j + 20] = (byte) Integer.parseInt(mask_str.substring(i, i+2), 16);
                j++;
                i++;
            }
        }
        return output;
    }

    private byte[] getBytesFromString(String vendor_str, String mask_str, int data_length) {
        int i, j = 0;
        byte[] output = new byte[data_length * 2 + 8];
        for (i = 0; i < 7; i++) {
            output[j] = 0;
            j++;
        }

        output[j] = (byte) data_length;
        j++;
        info("getBytesFromString vendor_str :" + vendor_str +" mask_str : " + mask_str);
        for (i = 0; i < vendor_str.length();i++) {
            if (vendor_str.charAt(i) != ':') {
                output[j] = (byte) Integer.parseInt(vendor_str.substring(i, i+2), 16);
                j++;
                i++;
            }
        }
        for (i = 0; i < mask_str.length();i++) {
            if (mask_str.charAt(i) != ':') {
                output[j] = (byte) Integer.parseInt(mask_str.substring(i, i+2), 16);
                j++;
                i++;
            }
        }
        return output;
    }

/************************************************************************************************
    **      function :  make_random_num
    **      description: if the autopair type is keypair, make random num first
*************************************************************************************************/
    private void make_random_num() {
        info("make random number");
        mLoadConfig.do_config();
        int length = Integer.parseInt(mLoadConfig.autoPairInfoMap.get("key_num"));
        RTKBT_AUTOPAIR_broadcast_num[0] =  length;
        String a ="please set";
        String key_str;
        for(int i =0; i < length; i++) {
            RTKBT_AUTOPAIR_rand_num[i] = ((int)(Math.random() * 16))%4;
            key_str = mLoadConfig.autoPairInfoMap.get("key_" + String.valueOf(RTKBT_AUTOPAIR_rand_num[i]+1));
            a= a + ": " + key_str;
            RTKBT_AUTOPAIR_broadcast_num[i+1] = Integer.parseInt(key_str);
        }
        notifyUser(a);
    }

/************************************************************************************************
**      function :  check_keypair_data
**      description: if the autopair type is keypair check the data
*************************************************************************************************/
    private boolean check_keypair_data() {
        int length = RTKBT_VENDOR_data[0] & 0x0F;
        info("check_keypair_data :" + RTKBT_VENDOR_data[1]);
        if(length == 0 || length > Integer.parseInt(mLoadConfig.autoPairInfoMap.get("key_num")))
            return false;
        for(int i = 0; i < length; i++) {
            int temp = (RTKBT_VENDOR_data[1] >> ((3-i)*2)) & 0x03;
            if(RTKBT_AUTOPAIR_rand_num[i] != temp)
                return false;
        }
        return true;
    }

/*****************************************************************************************************
**          function : check_to_create_bond_by_rssi
**          description: creating bond with device will depend on rssi,if rssi is too low,we will not
                         create bond.
******************************************************************************************************/
    private void check_to_create_bond_by_rssi( BluetoothDevice ble_device, int rssi) {
        info("check_to_create_bond");
        int i;
        if((0-rssi) > RTKBT_AUTOPAIR_SignalThreshold)
        {
            info("[ERROR-Weak Signal(" + "0 - "+rssi+" > "+RTKBT_AUTOPAIR_SignalThreshold+")] RCU: name["+"***"+"] ADDR["+ble_device+"] rssi["+rssi+"]\n");
            //if(compare(ble_device,RTKBT_AUTOPAIR_ble_device) != 0)
            //{
                //removeBond(ble_device, 5000);
            //}
            if(5 == RTKBT_AUTOPAIR_notify_counter)
                mHandler.sendMessage(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_weak_signal));
                RTKBT_AUTOPAIR_notify_counter = (RTKBT_AUTOPAIR_notify_counter + 1) % 100;
                return;
        }else{
            info("[Good Signal("+"0"+" - "+rssi+" > "+RTKBT_AUTOPAIR_SignalThreshold+")] RCU: name["+"**"+"] ADDR["+ble_device+"] rssi["+rssi+"]\n");
        }

        if(RTKBT_AUTOPAIR_State !=  RTKBT_AUTOPAIR_State_BLEScan )
        {
            info("[ERROR-Pair Pending] RCU: name[***] ADDR["+ble_device+"] rssi["+rssi+"]\n");
            if(compare(ble_device,RTKBT_AUTOPAIR_ble_device) != 0)
            {
                removeBond(ble_device, 5000);
            }
            return;
        }

        info("[FOUND] RCU: name["+" ** "+"] ADDR["+ble_device+"] rssi["+rssi+"]\n");
        RTKBT_AUTOPAIR_ble_device = ble_device;
        RTKBT_AUTOPAIR_classic_device = null;
        RTKBT_AUTOPAIR_State = RTKBT_AUTOPAIR_State_Found;
        notifyAutoPairState(RTKBT_AUTOPAIR_State_Found);
        holdPendingRCU(RTKBT_AUTOPAIR_ble_device, RTKBT_AUTOPAIR_classic_device);
        checkAndRestartScan(false);
        mHandler.sendMessage(mHandler.obtainMessage(RTKBT_AUTOPAIR_Event_FoundRCU));
        return;
    }

  /**************************************************************************************************************
    **      function : match_vendor_data
    **      decription: check vendor data is ok
    ***************************************************************************************************************/
    private boolean match_vendor_data(ScanRecord scanRecord_obj) {
        int j =0;
        int length =0;
        int vendor_length = RTKBT_AUTOPAIR_vendor_event_arry[20];
        byte[] scanRecord = scanRecord_obj.getBytes();
        while(j < scanRecord.length) {
            length = scanRecord[j];
            if (length == 0)
                break;
            j++;
            info("mLeScanCallback length: "+ length + " j: " + scanRecord[j]+ "j+1: "+scanRecord[j+1]+"j+2: "+ scanRecord[j+2]);
            if(((byte)scanRecord[j] == (byte)0xff)){
                for(int i = 0; i < vendor_length; i++) {
                    if(((byte)(scanRecord[j +1+i] & RTKBT_AUTOPAIR_vendor_event_arry[21 + vendor_length + i]) != (byte)RTKBT_AUTOPAIR_vendor_event_arry[21+i])) {
                        return false;
                    }
                }
                return true;
            }
            j += length;
        }

        return false;
    }

    /**************************************************************************************************************
      **      function : match_address
      **      decription: check address is ok
      ***************************************************************************************************************/
    private boolean match_address(BluetoothDevice device) {
        int num = Integer.parseInt(mLoadConfig.autoPairInfoMap.get("address_num"));
        for(int j = 0; j < num; j++) {
            String address_name = "address_" + String.valueOf(j+1);
            String addr_value = mLoadConfig.autoPairInfoMap.get(address_name);
            info(address_name + " : " + addr_value);
            if(device.getAddress().indexOf(addr_value) == 0) {
                return true;
            }
        }
        return false;
    }

    /**************************************************************************************************************
      **      function : match_name
      **      decription: check name is ok
      ***************************************************************************************************************/
    private boolean match_name(BluetoothDevice device) {
        int num = Integer.parseInt(mLoadConfig.autoPairInfoMap.get("name_num"));
        for(int j = 0; j < num; j++) {
            String name = "name_" + String.valueOf(j+1);
            String name_value = mLoadConfig.autoPairInfoMap.get(name);
            info(name + " : " + name_value);
            if(device.getName().equals(name_value)) {
                return true;
            }
        }
        return false;
    }

    /**************************************************************************************************************
      **      function : match_key
      **      decription: check key is ok
      ***************************************************************************************************************/
    private boolean match_key(byte[] scanRecord, boolean is_pair) {
        int j =0;
        int k;
        int length =0;
        int vendor_length = RTKBT_AUTOPAIR_vendor_event_arry[20];
        vendor_length = RTKBT_AUTOPAIR_vendor_event_arry[20];
        while(j < scanRecord.length) {
            length = scanRecord[j];
            if (length == 0)
                break;
            j++;
            info("mLeScanCallback length: "+ length);
            if(((byte)scanRecord[j] == (byte)0xff && !is_pair)){
                for(k = 0; k < vendor_length; k++) {
                    if(((byte)(scanRecord[j +1+k] & RTKBT_AUTOPAIR_vendor_event_arry[21 + vendor_length + k]) != (byte)RTKBT_AUTOPAIR_vendor_event_arry[21+k])) {
                        return false;
                    }
                }
                is_pair = true;
            }
            if(is_pair) {
                k = vendor_length;
                info("mLeScanCallback it is iflytek device adv");
                RTKBT_VENDOR_data[0] = (byte)scanRecord[j+k+1];
                RTKBT_VENDOR_data[1] = (byte)scanRecord[j+k+2];
                info("mLeScanCallback it is iflytek device adv: "+ RTKBT_VENDOR_data[0] +" : " + RTKBT_VENDOR_data[1]);
                if(check_keypair_data()) {
                    return true;
                }
                else
                    return false;
            }
            j += length;
        }
        return false;
    }

/**************************************************************************************************************
**      function : create_bond_by_type
**      decription: creating bond with device will depend on type
**      args     : auto_scan if true the scan will auto with autopair vendor event, else scan will begin depending
                   on user.
***************************************************************************************************************/
    private boolean create_bond_by_type(BluetoothDevice device,ScanRecord scanRecord, boolean auto_scan) {
        info("create_bond_by_type auto_scan: " + auto_scan);
        boolean is_pair = false;
        if(auto_scan && mLoadConfig.vendor_type == 8) {
            if(match_vendor_data(scanRecord)) {
                if((byte)mLoadConfig.autopair_type != (byte)0x88){
                    is_pair = true;
                }
                else
                    return true;
            }
            else
                return false;
        }
        for(int i = 0; i < mLoadConfig.total_type_num; i++){
            int autopair_type_temp = mLoadConfig.autopair_type & (0x01 << i);
            switch(autopair_type_temp) {
                case BluetoothRTKAutoPairLoadConfig.address_pair :
                {
                    info("address pair");
                    if(match_address(device))
                        return true;
                }
                break;

                case BluetoothRTKAutoPairLoadConfig.name_pair :
                {
                    info("name pair");
                    if(match_name(device))
                        return true;
                }
                break;

                case BluetoothRTKAutoPairLoadConfig.key_pair :
                {
                    info("key pair");
                    if(match_key(scanRecord.getBytes(), is_pair))
                        return true;
                }
                break;

                case BluetoothRTKAutoPairLoadConfig.manufactSpeciData_pair :
                {
                    info("manufactSpeciData_pair");
                    if(match_vendor_data(scanRecord))
                        return true;
                }

                default :
                    info("unknow pair type!");
                break;
            }
        }
        return false;
    }

    private void test() {
        long time = SystemClock.currentTimeMicro();
        String addr = String.format("%02X:%02X:%02X:%02X", ((time>>24)&0xff), ((time>>16)&0xff), ((time>>8)&0xff), ((time)&0xff));
        String ble_addr = "00:11:"+addr;
        String classic_addr;
        if((time&0x8) != 0)
            classic_addr = "00:22:"+addr;
        else
            classic_addr = null;

        info("addr:"+addr+"ble_addr:"+ble_addr+"classic_addr:"+classic_addr);
        if(classic_addr == null)
            saveNewRCU(RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(ble_addr), null);
        else
            saveNewRCU(RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(ble_addr), RTKBT_AUTOPAIR_BluetoothAadapter.getRemoteDevice(classic_addr));
    }

    /**************************************************************************************************************
        **      function : notifyUser
        **      decription: toast content to the user
    ***************************************************************************************************************/
    private void notifyUser(String string) {
        if(RTK_NOTIFY) {
            log(string);
            CharSequence text = string;
            Toast.makeText(getApplicationContext(),text,Toast.LENGTH_LONG).show();
        }
    }

    private void notifyUser(int resId) {
        if(RTK_NOTIFY) {
            Toast.makeText(getApplicationContext(),resId,Toast.LENGTH_LONG).show();
        }
    }

    private void notifyUser(int resId, String string) {
        if(RTK_NOTIFY) {
            Toast.makeText(getApplicationContext(),getResources().getText(resId)+string,Toast.LENGTH_LONG).show();
        }
    }
    private void notifyAutoPairState(int state)
    {
        notifyApp(state, (RTKBT_AUTOPAIR_ble_device != null)? RTKBT_AUTOPAIR_ble_device.toString(): "00:00:00:00:00:00", null);
    }
    private void notifyApp(int message_id, String address, byte[] data)
    {
        int len;
        Intent intent = new Intent(RTKBT_AUTOPAIR_MESSAGE);
        info("Broadcast message(id:"+message_id+" address:"+address+")");
        intent.putExtra(RTKBT_AUTOPAIR_MESSAGE_FIELD_MESSAGE_ID, message_id);
        intent.putExtra(RTKBT_AUTOPAIR_MESSAGE_FIELD_RCUADDR, address);
        if(data != null)
        {
            intent.putExtra(RTKBT_AUTOPAIR_MESSAGE_FIELD_DATA, data);
        }
        intent.addFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
        sendBroadcast(intent, BLUETOOTH_PERM);
        return;
    }

/**************************************************************************************************************
    **      function : print_vendor_event
    **      decription: print vendor event data which is parse from xml
***************************************************************************************************************/
    private void print_vendor_event() {
        if(RTK_INFO) {
            info("print_vendor_event ");
            if(RTKBT_AUTOPAIR_vendor_event_arry != null) {
                int length = RTKBT_AUTOPAIR_vendor_event_arry.length;
                String a = "vendor_value and mask_value is : ";
                for(int i = 0; i < length; i++) {
                    a = a + String.format("%02x : ",RTKBT_AUTOPAIR_vendor_event_arry[i]);
                }
                info(a);
            }
        }
    }
    private static void log(String msg) {
        if(RTK_DBG)
            Log.d(TAG, msg);
    }
    private static void info(String msg) {
        if(RTK_INFO)
            Log.i(TAG, msg);
    }
}
