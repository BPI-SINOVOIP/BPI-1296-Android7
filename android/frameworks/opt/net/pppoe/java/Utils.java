package com.android.server.pppoe;

import android.util.Log;

import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.util.Enumeration;
import java.util.List;
import java.net.InetAddress;
import java.net.UnknownHostException;
import android.net.StaticIpConfiguration;
import android.os.SystemProperties;
import android.net.LinkProperties;
import android.net.LinkAddress;
import android.net.RouteInfo;
import android.net.IpPrefix;
import java.net.Inet6Address;
import static android.net.RouteInfo.RTN_UNREACHABLE;

public class Utils {

    private static boolean DBG = true;
    private static final String TAG = "Utils";

    public static NetworkInterface getIface(String regex){
        Enumeration<NetworkInterface> ifaces = null;
        try {
            ifaces = NetworkInterface.getNetworkInterfaces();
            while (ifaces.hasMoreElements()) {
                NetworkInterface iface = ifaces.nextElement();
                if (!iface.getDisplayName().matches(regex))
                    continue;
                return iface;
            }
        } catch (SocketException e) {
            e.printStackTrace();
        }
        return null;
    }

    //Used for ppp interface
    public static InterfaceAddress getIfaceIp(String regex) {
        NetworkInterface iface = getIface(regex);
        if(iface==null)
            return null;
        List<InterfaceAddress> addressList = iface.getInterfaceAddresses();
        return addressList.get(addressList.size()-1);
    }

    /**
     * Get an {@link InetAddress} from a string
     *
     * @throws IllegalArgumentException if the string is not a valid IP address.
     */
    public static InetAddress getInetAddress(String ipAddress) {
        if (!InetAddress.isNumeric(ipAddress)) {
            throw new IllegalArgumentException(
                    String.format("IP address %s is not numeric", ipAddress));
        }

        try {
            return InetAddress.getByName(ipAddress);
        } catch (UnknownHostException e) {
            throw new IllegalArgumentException(
                    String.format("IP address %s could not be resolved", ipAddress));
        }
    }

    public static LinkProperties getPppoeLinkProperties(){
        String ifaceMatch = "ppp\\d";
        StaticIpConfiguration staticIpConfig = new StaticIpConfiguration();
        NetworkInterface netIface = Utils.getIface(ifaceMatch);
        if(netIface==null){
            Log.e(TAG, "netIface is null, stop pppoe process");
            return null;
        }
        String ifaceName = netIface.getDisplayName();
        InterfaceAddress ifaceAddr = Utils.getIfaceIp(ifaceMatch);
        if(ifaceAddr==null){
            Log.e(TAG, "ifaceAddr is null, stop pppoe process");
            return null;
        }
        InetAddress ipAddress = ifaceAddr.getAddress();
        int prefixLength = ifaceAddr.getNetworkPrefixLength();
        staticIpConfig.ipAddress = new LinkAddress(ipAddress, prefixLength);
        String gw = SystemProperties.get("net.pppoe.gw", "");
        if(gw.length()>0)
            staticIpConfig.gateway = Utils.getInetAddress(gw);
        String dns1 = SystemProperties.get("net.pppoe.dns1", "");
        if(dns1.length()>0)
            staticIpConfig.dnsServers.add(Utils.getInetAddress(dns1));
        String dns2 = SystemProperties.get("net.pppoe.dns2", "");
        if(dns2.length()>0)
            staticIpConfig.dnsServers.add(Utils.getInetAddress(dns2));
        LinkProperties lp = staticIpConfig.toLinkProperties(ifaceName);
        lp.setInterfaceName("ppp0");
        // lp.addRoute(new RouteInfo(new IpPrefix(Inet6Address.ANY, 0), RTN_UNREACHABLE));
        return lp;
    }

    private static void log(String s){
        if(DBG)
            Log.d(TAG, s);
    }
}

