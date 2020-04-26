/*
 * Copyright (C) 2014 The Android Open Source Project
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

package com.android.server.ethernet;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.DhcpResults;
import android.net.EthernetManager;
import android.net.IEthernetServiceListener;
import android.net.InterfaceConfiguration;
import android.net.IpConfiguration;
import android.net.IpConfiguration.IpAssignment;
import android.net.IpConfiguration.ProxySettings;
import android.net.LinkAddress;
import android.net.LinkProperties;
import android.net.NetworkAgent;
import android.net.NetworkCapabilities;
import android.net.NetworkFactory;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.StaticIpConfiguration;
import android.net.ip.IpManager;
import android.net.ip.IpManager.ProvisioningConfiguration;
import android.net.ip.IpManager.WaitForProvisioningCallback;
import android.os.Handler;
import android.os.IBinder;
import android.os.INetworkManagementService;
import android.os.Looper;
import android.os.RemoteCallbackList;
import android.os.RemoteException;
import android.os.ServiceManager;
import android.text.TextUtils;
import android.util.Log;

import com.android.internal.util.IndentingPrintWriter;
import com.android.server.net.BaseNetworkObserver;

import java.io.FileDescriptor;
import java.io.PrintWriter;

import java.net.Inet4Address;
import java.util.Random;
import java.util.Iterator;
import android.os.SystemProperties;

//[rtk begin]
import android.content.Intent;
import android.content.IntentFilter;
import android.content.BroadcastReceiver;
import android.os.Parcel;
import com.android.server.pppoe.*;
import android.os.HandlerThread;
import android.provider.Settings;
import com.rtk.net.openwrt.UbusRpc;
import com.rtk.net.openwrt.Server;
import com.rtk.net.openwrt.UdpServer;
//[rtk end]

/**
 * Manages connectivity for an Ethernet interface.
 *
 * Ethernet Interfaces may be present at boot time or appear after boot (e.g.,
 * for Ethernet adapters connected over USB). This class currently supports
 * only one interface. When an interface appears on the system (or is present
 * at boot time) this class will start tracking it and bring it up, and will
 * attempt to connect when requested. Any other interfaces that subsequently
 * appear will be ignored until the tracked interface disappears. Only
 * interfaces whose names match the <code>config_ethernet_iface_regex</code>
 * regular expression are tracked.
 *
 * This class reports a static network score of 70 when it is tracking an
 * interface and that interface's link is up, and a score of 0 otherwise.
 *
 * @hide
 */
class EthernetNetworkFactory {
    private static final String NETWORK_TYPE = "Ethernet";
    private static final String TAG = "EthernetNetworkFactory";
    private static final int NETWORK_SCORE = 70;
    private static final boolean DBG = true;

    /** Tracks interface changes. Called from NetworkManagementService. */
    private InterfaceObserver mInterfaceObserver;

    /** For static IP configuration */
    private EthernetManager mEthernetManager;

    /** To set link state and configure IP addresses. */
    private INetworkManagementService mNMService;

    /* To communicate with ConnectivityManager */
    private NetworkCapabilities mNetworkCapabilities;
    private NetworkAgent mNetworkAgent;
    private LocalNetworkFactory mFactory;
    private Context mContext;

    /** Product-dependent regular expression of interface names we track. */
    private static String mIfaceMatch = "";

    /** To notify Ethernet status. */
    private final RemoteCallbackList<IEthernetServiceListener> mListeners;

    /** Data members. All accesses to these must be synchronized(this). */
    private static String mIface = "";
    private String mHwAddr;
    private static boolean mLinkUp;
    private NetworkInfo mNetworkInfo;
    private LinkProperties mLinkProperties;
    private IpManager mIpManager;
    private Thread mIpProvisioningThread;

    EthernetNetworkFactory(RemoteCallbackList<IEthernetServiceListener> listeners) {
        mNetworkInfo = new NetworkInfo(ConnectivityManager.TYPE_ETHERNET, 0, NETWORK_TYPE, "");
        mLinkProperties = new LinkProperties();
        initNetworkCapabilities();
        mListeners = listeners;
    }

    private class LocalNetworkFactory extends NetworkFactory {
        LocalNetworkFactory(String name, Context context, Looper looper) {
            super(looper, context, name, new NetworkCapabilities());
        }

        protected void startNetwork() {
            onRequestNetwork();
        }
        protected void stopNetwork() {
        }
    }

    private void stopIpManagerLocked() {
        if (mIpManager != null) {
            mIpManager.shutdown();
            mIpManager = null;
        }
    }

    private void stopIpProvisioningThreadLocked() {
        stopIpManagerLocked();

        if (mIpProvisioningThread != null) {
            mIpProvisioningThread.interrupt();
            mIpProvisioningThread = null;
        }
    }

    /**
     * Updates interface state variables.
     * Called on link state changes or on startup.
     */
    private void updateInterfaceState(String iface, boolean up) {
        if (!mIface.equals(iface)) {
            return;
        }
        Log.d(TAG, "updateInterface: " + iface + " link " + (up ? "up" : "down"));

        synchronized(this) {
            mLinkUp = up;
            mNetworkInfo.setIsAvailable(up);
            //DHCKYLIN-1186, DHCKYLIN-1188, reconnect pppoe if eth reconnect.
            if(!up && isPppoeOn()){
                Log.d(TAG, "down pppoe");
                PppoeStart(false);
            }
            if (!up) {
                // Tell the agent we're disconnected. It will call disconnect().
                mNetworkInfo.setDetailedState(DetailedState.DISCONNECTED, null, mHwAddr);
                stopIpProvisioningThreadLocked();
            }
            updateAgent();
            // set our score lower than any network could go
            // so we get dropped.  TODO - just unregister the factory
            // when link goes down.
            mFactory.setScoreFilter(up ? NETWORK_SCORE : -1);
        }
    }

    private class InterfaceObserver extends BaseNetworkObserver {
        @Override
        public void interfaceLinkStateChanged(String iface, boolean up) {
            updateInterfaceState(iface, up);
        }

        @Override
        public void interfaceAdded(String iface) {
            maybeTrackInterface(iface);
        }

        @Override
        public void interfaceRemoved(String iface) {
            stopTrackingInterface(iface);
        }
    }

    private boolean isXenOr3in1(){
        return ("xen".equals(SystemProperties.get("ro.boot.storage")) 
            || is3in1Net());
    }

    private boolean is3in1Net(){
        return (SystemProperties.getBoolean("persist.rtk.net.openwrt", false) 
                && !SystemProperties.getBoolean("persist.rtk.net.openwrt.ottwifi", false));
    }

    private void setInterfaceUp(String iface) {
        // Bring up the interface so we get link status indications.
        //DHCKYLIN-2256, if openwrt veth restart, Android have to slow down setInterfaceUp to work correctly.
        //DHCKYLIN-2527, DHCKYLIN-2771, cannot use UbusRpc in this method, so check SystemProperties here.
        if(isXenOr3in1())
            try{Thread.sleep(100);}catch(InterruptedException ie){}

        try {
            mNMService.setInterfaceUp(iface);
            String hwAddr = null;
            InterfaceConfiguration config = mNMService.getInterfaceConfig(iface);

            if (config == null) {
                Log.e(TAG, "Null iterface config for " + iface + ". Bailing out.");
                return;
            }

            synchronized (this) {
                if (!isTrackingInterface()) {
                    setInterfaceInfoLocked(iface, config.getHardwareAddress());
                    mNetworkInfo.setIsAvailable(true);
                    mNetworkInfo.setExtraInfo(mHwAddr);
                } else {
                    Log.e(TAG, "Interface unexpectedly changed from " + iface + " to " + mIface);
                    mNMService.setInterfaceDown(iface);
                }
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Error upping interface " + mIface + ": " + e);
        }
    }

    private boolean maybeTrackInterface(String iface) {
        // If we don't already have an interface, and if this interface matches
        // our regex, start tracking it.
        if(is3in1Net()){
            if (!iface.equals("eth9"))
                return false;
        } else {
            if (!iface.equals(SystemProperties.get("net.eth.iface", "eth0")))
                return false;
        }

        if(isTrackingInterface())
            return false;

        Log.d(TAG, "Started tracking interface " + iface);
        setInterfaceUp(iface);
        return true;
    }

    private void stopTrackingInterface(String iface) {
        if (!iface.equals(mIface))
            return;

        Log.d(TAG, "Stopped tracking interface " + iface);
        // TODO: Unify this codepath with stop().
        synchronized (this) {
            stopIpProvisioningThreadLocked();
            setInterfaceInfoLocked("", null);
            mNetworkInfo.setExtraInfo(null);
            mLinkUp = false;
            mNetworkInfo.setDetailedState(DetailedState.DISCONNECTED, null, mHwAddr);
            updateAgent();
            mNetworkAgent = null;
            mNetworkInfo = new NetworkInfo(ConnectivityManager.TYPE_ETHERNET, 0, NETWORK_TYPE, "");
            mLinkProperties = new LinkProperties();
        }
    }

    private boolean setStaticIpAddress(StaticIpConfiguration staticConfig) {
        if (staticConfig.ipAddress != null &&
                staticConfig.gateway != null &&
                staticConfig.dnsServers.size() > 0) {
            try {
                Log.i(TAG, "Applying static IPv4 configuration to " + mIface + ": " + staticConfig);
                InterfaceConfiguration config = mNMService.getInterfaceConfig(mIface);
                config.setLinkAddress(staticConfig.ipAddress);
                mNMService.setInterfaceConfig(mIface, config);
                return true;
            } catch(RemoteException|IllegalStateException e) {
               Log.e(TAG, "Setting static IP address failed: " + e.getMessage());
            }
        } else {
            Log.e(TAG, "Invalid static IP configuration.");
        }
        return false;
    }

    public void updateAgent() {
        synchronized (EthernetNetworkFactory.this) {
            if (mNetworkAgent == null) return;
            if (DBG) {
                Log.i(TAG, "Updating mNetworkAgent with: " +
                      mNetworkCapabilities + ", " +
                      mNetworkInfo + ", " +
                      mLinkProperties);
            }
            mNetworkAgent.sendNetworkCapabilities(mNetworkCapabilities);
            mNetworkAgent.sendNetworkInfo(mNetworkInfo);
            mNetworkAgent.sendLinkProperties(mLinkProperties);
            // never set the network score below 0.
            mNetworkAgent.sendNetworkScore(mLinkUp? NETWORK_SCORE : 0);
        }
    }

    /* Called by the NetworkFactory on the handler thread. */
    public void onRequestNetwork() {
        synchronized(EthernetNetworkFactory.this) {
            if (mIpProvisioningThread != null) {
                return;
            }
        }

        if(isPppoeOn()){
            synchronized(EthernetNetworkFactory.this) {
                PppoeStart(true);
            }
        }

        if(!mLinkUp) return;
        final Thread ipProvisioningThread = new Thread(new Runnable() {
            public void run() {
                if (DBG) {
                    Log.d(TAG, String.format("starting ipProvisioningThread(%s): mNetworkInfo=%s",
                            mIface, mNetworkInfo));
                }

                LinkProperties linkProperties;

                IpConfiguration config = mEthernetManager.getConfiguration();

                if (config.getIpAssignment() == IpAssignment.STATIC) {
                    if (!setStaticIpAddress(config.getStaticIpConfiguration())) {
                        // We've already logged an error.
                        return;
                    }
                    linkProperties = config.getStaticIpConfiguration().toLinkProperties(mIface);
                } else {
                    mNetworkInfo.setDetailedState(DetailedState.OBTAINING_IPADDR, null, mHwAddr);
                    WaitForProvisioningCallback ipmCallback = new WaitForProvisioningCallback() {
                        @Override
                        public void onLinkPropertiesChange(LinkProperties newLp) {
                            synchronized(EthernetNetworkFactory.this) {
                                //DHCKYLIN-2212, this callback cannot be used when connect pppoe.
                                if(mLinkProperties!=null && "ppp0".equals(mLinkProperties.getInterfaceName()))
                                    return;
                                if (mNetworkAgent != null && mNetworkInfo.isConnected()) {
                                    mLinkProperties = newLp;
                                    mNetworkAgent.sendLinkProperties(newLp);
                                }
                            }
                        }
                    };

                    synchronized(EthernetNetworkFactory.this) {
                        if (mNetworkAgent != null) {
                            Log.e(TAG, "Already have a NetworkAgent2 - aborting new request");
                            stopIpManagerLocked();
                            mIpProvisioningThread = null;
                            return;
                        }
                        stopIpManagerLocked();
                        mIpManager = new IpManager(mContext, mIface, ipmCallback);

                        if (config.getProxySettings() == ProxySettings.STATIC ||
                                config.getProxySettings() == ProxySettings.PAC) {
                            mIpManager.setHttpProxy(config.getHttpProxy());
                        }

                        final String tcpBufferSizes = mContext.getResources().getString(
                                com.android.internal.R.string.config_ethernet_tcp_buffers);
                        if (!TextUtils.isEmpty(tcpBufferSizes)) {
                            mIpManager.setTcpBufferSizes(tcpBufferSizes);
                        }

                        final ProvisioningConfiguration provisioningConfiguration =
                                mIpManager.buildProvisioningConfiguration()
                                        .withProvisioningTimeoutMs(0)
                                        .build();
                        mIpManager.startProvisioning(provisioningConfiguration);
                    }

                    linkProperties = ipmCallback.waitForProvisioning();
                    if (linkProperties == null) {
                        Log.e(TAG, "IP provisioning error");
                        // set our score lower than any network could go
                        // so we get dropped.
                        mFactory.setScoreFilter(-1);
                        synchronized(EthernetNetworkFactory.this) {
                            stopIpManagerLocked();
                        }
                        return;
                    }
                }

                synchronized(EthernetNetworkFactory.this) {
                    if (mNetworkAgent != null) {
                        Log.e(TAG, "Already have a NetworkAgent - aborting new request");
                        stopIpManagerLocked();
                        mIpProvisioningThread = null;
                        return;
                    }
                    mLinkProperties = linkProperties;
                    mNetworkInfo.setIsAvailable(true);
                    mNetworkInfo.setDetailedState(DetailedState.CONNECTED, null, mHwAddr);

                    // Add OpenWRT to hosts
                    if(SystemProperties.getBoolean("persist.rtk.net.openwrt", false)){
                        if (DBG) Log.i(TAG, "has openwrt");
                        SystemProperties.set("ctl.start", "update_hosts");
                        Server server = new Server(mContext);
                        server.updateDNSServer(mLinkProperties);
                    }

                    // Create our NetworkAgent.
                    mNetworkAgent = new NetworkAgent(mFactory.getLooper(), mContext,
                            NETWORK_TYPE, mNetworkInfo, mNetworkCapabilities, mLinkProperties,
                            NETWORK_SCORE) {
                        public void unwanted() {
                            Log.i(TAG, "unwanted");
                            synchronized(EthernetNetworkFactory.this) {
                                if (this == mNetworkAgent) {
                                    //DHCKYLIN-1186, DHCKYLIN-1188, reconnect pppoe if eth reconnect.
                                    // PppoeStart(false);
                                    stopIpManagerLocked();
                                    mLinkProperties.clear();
                                    mNetworkInfo.setDetailedState(DetailedState.DISCONNECTED, null,
                                            mHwAddr);
                                    updateAgent();
                                    mNetworkAgent = null;
                                    try {
                                        mNMService.clearInterfaceAddresses(mIface);
                                    } catch (Exception e) {
                                        Log.e(TAG, "Failed to clear addresses or disable ipv6" + e);
                                    }
                                } else {
                                    Log.d(TAG, "Ignoring unwanted as we have a more modern " +
                                            "instance");
                                }
                            }
                        };
                    };

                    mIpProvisioningThread = null;
                }

                if (DBG) {
                    Log.d(TAG, String.format("exiting ipProvisioningThread(%s): mNetworkInfo=%s",
                            mIface, mNetworkInfo));
                }
            }
        });

        synchronized(EthernetNetworkFactory.this) {
            if (mIpProvisioningThread == null) {
                mIpProvisioningThread = ipProvisioningThread;
                mIpProvisioningThread.start();
            }
        }
    }

    /**
     * Begin monitoring connectivity
     */
    public synchronized void start(Context context, Handler target) {
        Log.i(TAG, "monitoring start");
        // The services we use.
        IBinder b = ServiceManager.getService(Context.NETWORKMANAGEMENT_SERVICE);
        mNMService = INetworkManagementService.Stub.asInterface(b);
        mEthernetManager = (EthernetManager) context.getSystemService(Context.ETHERNET_SERVICE);

        // Interface match regex.
        mIfaceMatch = context.getResources().getString(
                com.android.internal.R.string.config_ethernet_iface_regex);

        // Create and register our NetworkFactory.
        mFactory = new LocalNetworkFactory(NETWORK_TYPE, context, target.getLooper());
        mFactory.setCapabilityFilter(mNetworkCapabilities);
        mFactory.setScoreFilter(-1); // this set high when we have an iface
        mFactory.register();

        mContext = context;

        // Start tracking interface change events.
        mInterfaceObserver = new InterfaceObserver();
        try {
            mNMService.registerObserver(mInterfaceObserver);
        } catch (RemoteException e) {
            Log.e(TAG, "Could not register InterfaceObserver " + e);
        }

        // If an Ethernet interface is already connected, start tracking that.
        // Otherwise, the first Ethernet interface to appear will be tracked.
        try {
            final String[] ifaces = mNMService.listInterfaces();
            for (String iface : ifaces) {
                synchronized(this) {
                    if (maybeTrackInterface(iface)) {
                        // We have our interface. Track it.
                        // Note: if the interface already has link (e.g., if we
                        // crashed and got restarted while it was running),
                        // we need to fake a link up notification so we start
                        // configuring it. Since we're already holding the lock,
                        // any real link up/down notification will only arrive
                        // after we've done this.
                        if (mNMService.getInterfaceConfig(iface).hasFlag("running")) {
                            updateInterfaceState(iface, true);
                        }
                        break;
                    }
                }
            }
        } catch (RemoteException|IllegalStateException e) {
            Log.e(TAG, "Could not get list of interfaces " + e);
        }


        if(mPppoeStateFilter==null){
            mPppoeStateFilter = new IntentFilter();
            mPppoeStateFilter.addAction("PPPOE_SET_INTERFACE");
            mPppoeStateFilter.addAction(Intent.ACTION_SCREEN_OFF);
            mPppoeStateFilter.addAction(Intent.ACTION_SCREEN_ON);
            mPppoeStateFilter.addAction("rtk.intent.action.POWER_SUSPEND");
            context.registerReceiver(mPppoeStateReceiver, mPppoeStateFilter, null, null);
        }

        if(isXenOr3in1())
            UdpServer.getInstance().start();
    }

    public synchronized void stop() {
        Log.i(TAG, "monitoring stop");
        if(isXenOr3in1())
            UdpServer.getInstance().stop();
        //DHCKYLIN-1416
        PppoeStart(false);
        stopIpProvisioningThreadLocked();

        // ConnectivityService will only forget our NetworkAgent if we send it a NetworkInfo object
        // with a state of DISCONNECTED or SUSPENDED. So we can't simply clear our NetworkInfo here:
        // that sets the state to IDLE, and ConnectivityService will still think we're connected.
        //
        // TODO: stop using explicit comparisons to DISCONNECTED / SUSPENDED in ConnectivityService,
        // and instead use isConnectedOrConnecting().
        mNetworkInfo.setDetailedState(DetailedState.DISCONNECTED, null, mHwAddr);

        mLinkUp = false;
        updateAgent();
        mLinkProperties = new LinkProperties();
        mNetworkAgent = null;
        //DHCKYLIN-1366, DHCP/static IP configuration switching failed.
        try {
            mNMService.clearInterfaceAddresses(mIface);
        } catch (Exception e) {
            Log.e(TAG, "Failed to clear addresses or disable ipv6" + e);
        }
        setInterfaceInfoLocked("", null);
        mNetworkInfo = new NetworkInfo(ConnectivityManager.TYPE_ETHERNET, 0, NETWORK_TYPE, "");
        mFactory.unregister();
    }

    private void initNetworkCapabilities() {
        mNetworkCapabilities = new NetworkCapabilities();
        mNetworkCapabilities.addTransportType(NetworkCapabilities.TRANSPORT_ETHERNET);
        mNetworkCapabilities.addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET);
        mNetworkCapabilities.addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_RESTRICTED);
        // We have no useful data on bandwidth. Say 100M up and 100M down. :-(
        mNetworkCapabilities.setLinkUpstreamBandwidthKbps(100 * 1000);
        mNetworkCapabilities.setLinkDownstreamBandwidthKbps(100 * 1000);
    }

    public synchronized boolean isTrackingInterface() {
        return !TextUtils.isEmpty(mIface);
    }

    /**
     * Set interface information and notify listeners if availability is changed.
     * This should be called with the lock held.
     */
    private void setInterfaceInfoLocked(String iface, String hwAddr) {
        boolean oldAvailable = isTrackingInterface();
        mIface = iface;
        mHwAddr = hwAddr;
        boolean available = isTrackingInterface();

        if (oldAvailable != available) {
            int n = mListeners.beginBroadcast();
            for (int i = 0; i < n; i++) {
                try {
                    mListeners.getBroadcastItem(i).onAvailabilityChanged(available);
                } catch (RemoteException e) {
                    // Do nothing here.
                }
            }
            mListeners.finishBroadcast();
        }
    }

    synchronized void dump(FileDescriptor fd, IndentingPrintWriter pw, String[] args) {
        if (isTrackingInterface()) {
            pw.println("Tracking interface: " + mIface);
            pw.increaseIndent();
            pw.println("MAC address: " + mHwAddr);
            pw.println("Link state: " + (mLinkUp ? "up" : "down"));
            pw.decreaseIndent();
        } else {
            pw.println("Not tracking any interface");
        }

        pw.println();
        pw.println("NetworkInfo: " + mNetworkInfo);
        pw.println("LinkProperties: " + mLinkProperties);
        pw.println("NetworkAgent: " + mNetworkAgent);
        if (mIpManager != null) {
            pw.println("IpManager:");
            pw.increaseIndent();
            mIpManager.dump(fd, pw, args);
            pw.decreaseIndent();
        }
    }

    //PPPoE begin
    private IntentFilter mPppoeStateFilter;
    private NetworkInfo oldNetworkInfo;
    private LinkProperties oldLinkProperties;
    private int mPppoeState = 0;
    private boolean mSuspend = false;
    private static final String PPPOE_COLD_BOOT_RECONNECT = "persist.pppoe.coldboot.conn";

    private final BroadcastReceiver mPppoeStateReceiver = new BroadcastReceiver(){
        @Override
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if ("PPPOE_SET_INTERFACE".equals(action)) {
                int up = intent.getIntExtra("EXTRA_PPPOE_STATE", -1);
                setPppoeState(up);
            } else if(Intent.ACTION_SCREEN_OFF.equals(action)) {
                int mode = SystemProperties.getInt("persist.rtk.screenoff.suspend",0);
                if(mode==1) { //SCREEN_OFF_SUSPEND_ON
                    mSuspend = true;
                    Log.d(TAG, "suspend stop pppoe");
                    PppoeStart(false);
                    stopIpManagerLocked();
                    //DHCKYLIN-1256, eth data have to be clear if power suspend for some network environment.
                    mLinkProperties.clear();
                    mNetworkInfo.setDetailedState(DetailedState.DISCONNECTED, null,
                            mHwAddr);
                    updateAgent();
                    mNetworkAgent = null;
                    try {
                        mNMService.clearInterfaceAddresses(mIface);
                        if(isXenOr3in1())
                            mNMService.setInterfaceDown(mIface);
                    } catch (Exception e) {
                        Log.e(TAG, "Failed to clear addresses or disable ipv6" + e);
                    }
                }
            } else if(Intent.ACTION_SCREEN_ON.equals(action)){
                if(mSuspend) {
                    mSuspend = false;
                    if(isXenOr3in1()){
                        try {
                            mNMService.setInterfaceUp(mIface);
                        } catch (RemoteException e) {
                            Log.e(TAG, "Failed to setInterfaceUp" + e);
                        }
                    }
                }
            } else if("rtk.intent.action.POWER_SUSPEND".equals(action)){
                // SystemProperties.set(PPPOE_POWER_SUSPEND, "true");
            }
        }
    };

    private boolean isPppoeOn(){
        if(mContext==null) return false;
        boolean isColdBootReconnect = SystemProperties.getBoolean(PPPOE_COLD_BOOT_RECONNECT, false);
        boolean isPppoeSettingOn = Settings.Global.getInt(mContext.getContentResolver(), Settings.Global.PPPOE_ON, 0)==1;
        if(isColdBootReconnect) {
            return isPppoeSettingOn;
        } else {
            return isPppoeSettingOn && mPppoeState==1;
        }
    }

    private LinkProperties cloneLinkProperties(LinkProperties pObj){
        Parcel p = Parcel.obtain();
        p.writeValue(pObj);
        p.setDataPosition(0);
        LinkProperties foo = (LinkProperties)p.readValue(LinkProperties.class.getClassLoader());
        p.recycle();
        return foo;
    }

    private NetworkInfo cloneNetworkInfo(NetworkInfo pObj){
        Parcel p = Parcel.obtain();
        p.writeValue(pObj);
        p.setDataPosition(0);
        NetworkInfo foo = (NetworkInfo)p.readValue(NetworkInfo.class.getClassLoader());
        p.recycle();
        return foo;
    }

    private synchronized void PppoeStart(boolean up){
        Log.i(TAG, "PppoeStart "+up);
        try{
            PppoeUtil util = PppoeUtil.getInstance(mContext.getApplicationContext());
            if(up){
                if(util.isUp()!=1 && mLinkUp){
                    Log.i(TAG, "call pppoe util start");
                    util.start();
                }
            } else {
                if(util.isUp()==1){
                    Log.i(TAG, "call pppoe util stop");
                    util.stop();
                }
            }
        } catch (NoClassDefFoundError e){
            e.printStackTrace();
        }

    }

    Handler mHandler;
    PppoeNetworkFactory mPppoeTracker;
    private void startPppoeFactory(){
        log("startPppoeFactory");

        HandlerThread handlerThread = new HandlerThread("PppoeThread");
        handlerThread.start();
        mHandler = new Handler(handlerThread.getLooper());
        mPppoeTracker = new PppoeNetworkFactory();
        mPppoeTracker.start(mContext, mHandler);

        // mStarted.set(true);
        log("startPppoeFactory mPppoeTracker="+mPppoeTracker);
    }

    private void stopPppoeFactory(){
        log("stopPppoeFactory");
        // mStarted.set(false);
        if(mPppoeTracker!=null){
            log("stop pppoe factory");
            mPppoeTracker.stop();
            mPppoeTracker = null;
        } else {
            log("mPppoeTracker==null");
        }
        log("stopPppoeFactory end");
    }

    private void setPppoeState (int pppoeState){
        switch (pppoeState){
            case 1: //Enable
                Log.i(TAG,"broadcast pppoe enable");
                // before we change the state ,  backup them
                // oldLinkProperties = cloneLinkProperties(mLinkProperties);
                // oldNetworkInfo = cloneNetworkInfo(mNetworkInfo);
                // mLinkProperties.setInterfaceName("ppp0");

                // if(mLinkProperties==null){
                //     Log.e(TAG, "setPppoeState mLinkProperties==null");
                //     break;
                // }
                // mNetworkInfo.setDetailedState(DetailedState.CONNECTED, null, mHwAddr);
                // mNetworkInfo.setIsAvailable(true);

                // if(mNetworkAgent!=null){
                //     mNetworkAgent.sendNetworkInfo(mNetworkInfo);
                //     mNetworkAgent.sendLinkProperties(mLinkProperties);
                // }
                startPppoeFactory();

                mPppoeState = 1;
                break;

            case 0: //Disable
                Log.i(TAG, "broadcast pppoe disable:" + mIface);
                stopPppoeFactory();
                // if(oldLinkProperties == null)
                //     break;
                // //set back mInface and state
                // mLinkProperties = oldLinkProperties;
                // mNetworkInfo = oldNetworkInfo;
                // if(mNetworkAgent!=null){
                //     mNetworkAgent.sendNetworkInfo(mNetworkInfo);
                //     mNetworkAgent.sendLinkProperties(mLinkProperties);
                // }

                // oldLinkProperties = null;
                // oldNetworkInfo = null;

                mPppoeState = 0;
                break;

            default:
                Log.e(TAG, "broadcast pppoe unknown state:"+pppoeState);
                break;
        }
    }

    private static void log(String s){
        Log.d(TAG, s);
    }
    //PPPoE end
}
