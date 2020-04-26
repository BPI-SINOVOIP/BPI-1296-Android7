/*
 * Copyright (C) 2007 The Android Open Source Project
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

package com.android.server;

import android.Manifest;
import android.app.AppOpsManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.content.pm.UserInfo;
import android.content.res.ObbInfo;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.content.res.XmlResourceParser;
import android.hardware.usb.UsbManager;
import android.net.Uri;
import android.os.Binder;
import android.os.Environment;
import android.os.Environment.UserEnvironment;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.SystemClock;
import android.os.SystemProperties;
import android.os.UserHandle;
import android.os.RecoverySystem;
import android.os.IRtkGPIOService;  // TODO:
import android.text.TextUtils;
import android.util.AttributeSet;
import android.util.Slog;
import android.util.Xml;

import com.android.internal.annotations.GuardedBy;
import com.android.internal.annotations.VisibleForTesting;
import com.android.internal.app.IMediaContainerService;
import com.android.internal.util.IndentingPrintWriter;
import com.android.internal.util.Preconditions;
import com.android.internal.util.XmlUtils;
import com.android.server.NativeDaemonConnector.Command;
import com.android.server.NativeDaemonConnector.SensitiveArg;
import com.android.server.am.ActivityManagerService;
import com.android.server.pm.PackageManagerService;
import com.android.server.pm.UserManagerService;
import com.google.android.collect.Lists;
import com.google.android.collect.Maps;

import org.xmlpull.v1.XmlPullParserException;

import java.io.File;
import java.io.FileDescriptor;
import java.io.IOException;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.KeySpec;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;


/**
 * RtkGPIOService implements back-end services for Realtek platform, such as : monitor GPIO.
 * management.
 */
class RtkGPIOService extends IRtkGPIOService.Stub
        implements INativeDaemonConnectorCallbacks, Watchdog.Monitor {

    private static final boolean DEBUG_EVENTS = false;

    private static final String TAG = "RtkGPIOService";

    private static final String REALTEKGPIO_TAG = "RtkGPIOConnector";

    /** Maximum number of ASEC containers allowed to be mounted. */
    private static final int MAX_CONTAINERS = 250;

    private Context mContext;
    private NativeDaemonConnector mConnector;

    /*
     * These values shoule be  consistent with the definitions in GPIOProxy/ResponseCode.h
    */
    class GPIOResponseCode {
        // 100 series - Requested action was initiated; expect another reply
        // before proceeding with a new command;
        public static final int ActionInitiated = 100;
    
        // 200 series - Requested action has been successfully completed
        public static final int CommandOkay    = 200;
        public static final int GpioResult     = 210;
    
        // 400 series - The command was accepted but the requested action
        // did not take place.
        public static final int OperationFailed    = 400;
    
        // 500 series - The command was not accepted and the requested
        // action did not take place.
        public static final int CommandSyntaxError = 500;
        public static final int CommandParameterError = 501;
        public static final int CommandNoPermission = 502;
    
        // 600 series - Unsolicited broadcasts.
        public static final int GpioTrigger = 650;  // used to notify higer (java) layer gpio trigger.

    };

    class RealtekGPIOServiceHandler extends Handler {

        RealtekGPIOServiceHandler(Looper l) {
            super(l);
        }

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                default:
                    if (DEBUG_EVENTS) Slog.d(TAG, "RealtekGPIOServiceHandler::handleMessage: msg.what=" + msg.what);
                    break;
            }
        }
    };

    private final Handler mHandler;

    /**
     * Callback from NativeDaemonConnector
     */
    public void onDaemonConnected() {

        /*
         * Since we'll be calling back into the NativeDaemonConnector,
         * we need to do our work in a new thread.
         */
        new Thread("RtkGPIOService#onDaemonConnected") {
            @Override
            public void run() {
                /**
                 * Determine media state and UMS detection status
                 */
                if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onDaemonConnected: gpio get");
                try {
                    final String[] vols = NativeDaemonEvent.filterMessageList( 
                            mConnector.executeForList("gpio", "get", "4"), GPIOResponseCode.GpioResult);  // gpio 4
                    /*for(int i = 0; i < vols.length; i++) {
                        Slog.d(TAG, "vols: " + i + ", " + vols[i]);
                    }*/
                } catch (Exception e) {
                    Slog.e(TAG, "Error processing initial gpio state", e);
                }

            }
        }.start();
    }

    /**
     * Callback from NativeDaemonConnector
     */
    public boolean onEvent(int code, String raw, String[] cooked) {
        if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: " + code + ", " + raw);

        /*for(int i = 0; i < cooked.length; i++) {
            if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: " + i + ", " + cooked[i]);
        }*/

        switch(code) {
            case GPIOResponseCode.GpioTrigger:
                if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: GpioTrigger case.");

                if((cooked[1].compareToIgnoreCase("gpio_press")) == 0) {  // gpio_press
                    if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: gpio_press.");
                    try {
                        RecoverySystem.rebootWipeUserData(mContext);
                    } catch (IOException e) {
                        if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: rebootWipeUserData failed.");
                    }
                } else 
                    if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: gpio_release.");

                break;
            default:
                if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::onEvent: default case.");
                break;
        }

        return true;
    }

    /**
     * Constructs a new RtkGPIOService instance
     *
     * @param context  Binder context for this service
     */
    public RtkGPIOService(Context context) {
        if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService is constructed!");
        mContext = context;

        HandlerThread hthread = new HandlerThread(TAG);
        hthread.start();
        mHandler = new RealtekGPIOServiceHandler(hthread.getLooper());

        /*
         * Create the connection to rtk_gpio with a maximum queue of twice the
         * amount of containers we'd ever expect to have. This keeps an
         * "asec list" from blocking a thread repeatedly.
         */
        mConnector = new NativeDaemonConnector(this, "rtk_gpio", MAX_CONTAINERS * 2, REALTEKGPIO_TAG, 25, null);

        Thread thread = new Thread(mConnector, REALTEKGPIO_TAG);
        thread.start();

    }


    /** {@inheritDoc} */
    public void monitor() {
        if (mConnector != null) {
            mConnector.monitor();
        }
    }


    /**
     *  test
     *
     */
    public void test() {
        if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::test()");
    }

    /**
     *  gpio read function - usually use to read the status of gpio (input)
     *
     */
    public int read(int gpio_num) {
        int ret = 0;
        if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::read(), gpio_num: " + gpio_num );
        
        try {
            String gpio = Integer.toString(gpio_num);

            NativeDaemonEvent nativeEvent[] = mConnector.executeForList("gpio", "get", gpio);  // gpio 4

            /*for(int i = 0; i < nativeEvent.length; i++) {
                Slog.d(TAG, "evt: " + i + ", " + nativeEvent[i].getCode() + ", " + nativeEvent[i].getMessage());
            }*/
            
            if(nativeEvent[0].getCode() == GPIOResponseCode.GpioResult) {
                if (DEBUG_EVENTS) Slog.e(TAG, "(BINGO) rtk gpio state: read(), get message: " + nativeEvent[0].getMessage());

                if(nativeEvent[0].getMessage().compareTo("get_gpio_press") == 0) {  // C level sends "get_gpio_press" string to indicate the specified gpio is in press state.
                    ret = 1;
                } else if (nativeEvent[0].getMessage().compareTo("get_gpio_release") == 0) {  // C level sends "get_gpio_release" string to indicate the specified gpio is in release state.
                    ret = 0;
                }
            } else {
                if (DEBUG_EVENTS) Slog.e(TAG, "rtk gpio state: read(), not matching the code!");
            }

        } catch (Exception e) {
            if (DEBUG_EVENTS) Slog.e(TAG, "Error processing gpio state: read()", e);
        }

        return ret;
    }

    /**
     * Callback from NativeDaemonConnector
     */
    public boolean onCheckHoldWakeLock(int code) {
        return false;
    }

    /**
     *  gpio write function - usually use to write the {hi|low} to gpio (output)
     *
     */
    public int write(int gpio_num, int value) {
        int ret = 0;
        if (DEBUG_EVENTS) Slog.d(TAG, "RtkGPIOService::write(), gpio_num: " + gpio_num + ", value: " + value);
        
        try {
            String gpio = Integer.toString(gpio_num);
            String str = Integer.toString(value);

            NativeDaemonEvent nativeEvent[] = mConnector.executeForList("gpio", "set", gpio, str);  // gpio 7
            //NativeDaemonEvent nativeEvent = mConnector.execute("gpio", "set", "7", str);  // gpio 7

            for(int i = 0; i < nativeEvent.length; i++) {
                Slog.d(TAG, "evt: " + i + ", " + nativeEvent[i].getCode() + ", " + nativeEvent[i].getMessage());
            }
            
            if(nativeEvent[0].getCode() == GPIOResponseCode.GpioResult) {
                if (DEBUG_EVENTS) Slog.e(TAG, "(BINGO) rtk gpio state: write(), get message: " + nativeEvent[0].getMessage());
            } else {
                if (DEBUG_EVENTS) Slog.e(TAG, "rtk gpio state: write(), not matching the code!");
            }

        } catch (Exception e) {
            if (DEBUG_EVENTS) Slog.e(TAG, "Error processing gpio state: write()", e);
        }

        return ret;
    }

}
