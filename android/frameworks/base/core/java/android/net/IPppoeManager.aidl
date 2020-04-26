/**
 * Copyright (c) 2014, Realtek Corp. 
 *
 */
 
package android.net;

import android.net.NetworkInfo;

/**
 * Interface that allows controlling and querying pppoe connectivity.
 *
 */
interface IPppoeManager
{
    int getPppoeState();
    boolean setupPppoe(String iface, String user, String password);
    boolean startPppoe();
    boolean stopPppoe();
    String getPppoePhyIface();
    
    NetworkInfo getNetworkInfo();
    String getAddress();
    String getRouteAddress();
}

