package android.net;

import android.util.Log;
import android.net.wifi.*;
import java.io.*;
import java.net.*;
import java.util.*;
import java.util.regex.Pattern;
import android.os.SystemProperties;

import android.content.Context;
import android.net.ConnectivityManager;
import android.net.LinkProperties;
import android.net.RouteInfo;
import android.util.Log;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.IOException;
import java.net.Inet4Address;

import com.android.okhttp.MediaType;
import com.android.okhttp.OkHttpClient;
import com.android.okhttp.Request;
import com.android.okhttp.RequestBody;
import com.android.okhttp.Response;

//if("true".equals(SystemProperties.get("persist.rtk.net.openwrt", "false")))
public class OpenWrtNetUtils {
	private static final String TAG = "OpenWrtNetUtils";
	private static boolean isFirstGet = true;
	private static boolean mIsEnabled = false;

	public static boolean isEnabled() {
		// if(true) return false;
		if(isFirstGet){
			mIsEnabled = isXen() || (isOpenWrt() && !isOttWifi());
		}
		return mIsEnabled;
	}

    private static boolean isXen(){
        return "xen".equals(SystemProperties.get("ro.boot.storage"));
    }

    private static boolean isOpenWrt(){
        return SystemProperties.getBoolean("persist.rtk.net.openwrt", false);
    }

    private static boolean isOttWifi(){
        return SystemProperties.getBoolean("persist.rtk.net.openwrt.ottwifi", false);
    }
	// ConnectivityManager targets:
	//   getActiveNetworkInfo()
	//   getActiveNetworkInfoForUid( int )		UNDOCUMENTED
	//   getNetworkInfo( int )
	//   getAllNetworkInfo()
	//   isActiveNetworkMetered()

	// LOG Only
	//   requestRouteToHost()
	//   getActiveLinkProperties()			UNDOCUMENTED
	//   getLinkProperties( int )

	public static NetworkInfo getActiveNetworkInfo() {
		return getFakeNetworkInfo();
	}

	public static NetworkInfo getActiveNetworkInfoForUid(int uid) {
		return getFakeNetworkInfo();
	}

	public static NetworkInfo[] getAllNetworkInfo(NetworkInfo[] param) {
		NetworkInfo[] networks = param;
		int i;			// wifi networkinfo index
		boolean wifi_found = false;
		for (i = 0; i < networks.length; i++)
			if (networks[i].getType() == ConnectivityManager.TYPE_WIFI)
			{  wifi_found = true;  break;  }

		// if we're already on wifi don't interfere.
		if (wifi_found && networks[i].isConnected()) {
			log("getAllNetworkInfo" + ", on wifi already.");
			return param;
		}

		log("getAllNetworkInfo" + ", faking wifi");
		if (wifi_found)
			networks[i] = getFakeNetworkInfo();
		else
			networks = (NetworkInfo[]) push(networks, getFakeNetworkInfo());
		return networks;
	}

	public static NetworkInfo getNetworkInfo(int networkType) {
		if (networkType == ConnectivityManager.TYPE_WIFI)
			return getFakeNetworkInfo();
		else {
			log("getNetworkInfo called");
			return null;
		}
	}

	public static boolean isActiveNetworkMetered() {
		log("isActiveNetworkMetered(), faking wifi !");
		return false;
	}

	private static Object[] push(Object[] array, Object item) {
		Object[] longer = new Object[array.length + 1];
		System.arraycopy(array, 0, longer, 0, array.length);
		longer[array.length] = item;
		return longer;
	}

	private static NetworkInfo getFakeNetworkInfo() {
		NetworkInfo info = createNetworkInfo(ConnectivityManager.TYPE_WIFI, true);
		return info;
	}

	private static NetworkInfo createNetworkInfo(final int type, final boolean connected) {
		NetworkInfo networkInfo = new NetworkInfo(type, 0, "WIFI", null);

		// networkInfo.mNetworkType=type;
		// networkInfo.mTypeName="WIFI";
		// networkInfo.mState=NetworkInfo.State.CONNECTED;
		networkInfo.setDetailedState(NetworkInfo.DetailedState.CONNECTED, null, null);
		networkInfo.setIsAvailable(true);
		return networkInfo;
	}

	// WifiManager targets:
	//   isWifiEnabled()
	//   getWifiState()
	//   getConnectionInfo()
	//   getDhcpInfo()

	public static boolean isWifiEnabled() {
		return true;
	}

	public static boolean setWifiEnabled(boolean b){
		return false;
	}

	public static int getWifiState() {
		return WifiManager.WIFI_STATE_ENABLED;
	}

	public static WifiInfo getConnectionInfo() {
		return createWifiInfo();
	}

	public static DhcpInfo getDhcpInfo() {
		return createDhcpInfo();
	}

	private static WifiInfo createWifiInfo() {
		WifiInfo info = new WifiInfo();

		// NEEDED ?
		//    private boolean mHiddenSSID;

		IPInfo ip = getIPInfo();
		InetAddress addr = (ip != null ? ip.addr : null);
		info.setNetworkId(1);
		info.setSupplicantState(SupplicantState.COMPLETED);
		info.setBSSID("66:55:44:33:22:11");
		info.setMacAddress("11:22:33:44:55:66");
		info.setInetAddress(addr);
		info.setLinkSpeed(65);  // Mbps
		info.setFrequency(5000); // MHz
		info.setRssi(200); // MAX_RSSI

		info.setSSID(WifiSsid.createFromAsciiEncoded("OpenWRT"));

		return info;
	}

	static class IPInfo {
		NetworkInterface intf;
		InetAddress addr;
		String ip;
		int ip_hex;
		int netmask_hex;
	}

	// get current ip and netmask
	private static IPInfo getIPInfo() {
		try {
			List<NetworkInterface> interfaces = Collections.list(NetworkInterface.getNetworkInterfaces());
			for (NetworkInterface intf : interfaces) {
				List<InetAddress> addrs = Collections.list(intf.getInetAddresses());
				for (InetAddress addr : addrs) {
					if (!addr.isLoopbackAddress()) {
						String sAddr = addr.getHostAddress().toUpperCase();
						boolean isIPv4 = isIPv4Address(sAddr);
						if (isIPv4) {
							IPInfo info = new IPInfo();
							info.addr = addr;
							info.intf = intf;
							info.ip = sAddr;
							info.ip_hex = InetAddress_to_hex(addr);
							info.netmask_hex = netmask_to_hex(intf.getInterfaceAddresses().get(0).getNetworkPrefixLength());
							return info;
						}
					}
				}
			}
		} catch (Exception ex) { } // for now eat exceptions
		return null;
	}

	private static boolean isIPv4Address(String input) {
		Pattern IPV4_PATTERN = Pattern.compile("^(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)(\\.(25[0-5]|2[0-4]\\d|[0-1]?\\d?\\d)){3}$");
		return IPV4_PATTERN.matcher(input).matches();
	}

	private static DhcpInfo createDhcpInfo() {
		DhcpInfo i = new DhcpInfo();
		IPInfo ip = getIPInfo();
		i.ipAddress = ip.ip_hex;
		i.netmask = ip.netmask_hex;
		i.dns1 = 0x08080808;
		i.dns2 = 0x08080404;
		// gateway, leaseDuration, serverAddress

		String s = ("ip address: " + String.format("%x", i.ipAddress) +
		            " netmask: /" + i.netmask +
		            "dns1: " + String.format("%x", i.dns1) +
		            "dns2: " + String.format("%x", i.dns2));
		log(s);

		return i;
	}

	private static int netmask_to_hex(int netmask_slash) {
		int r = 0;
		int b = 1;
		for (int i = 0; i < netmask_slash;  i++, b = b << 1)
			r |= b;
		return r;
	}

	// for DhcpInfo
	private static int InetAddress_to_hex(InetAddress a) {
		int result = 0;
		byte b[] = a.getAddress();
		for (int i = 0; i < 4; i++)
			result |= (b[i] & 0xff) << (8 * i);
		return result;
	}

	private static void log(String s) {
		Log.d(TAG, s);
	}
}
