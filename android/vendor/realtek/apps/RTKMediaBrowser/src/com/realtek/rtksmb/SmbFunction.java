package com.realtek.rtksmb;

import android.util.Log;

public final class SmbFunction
{
    public static String TAG = "SmbFunction";
    public native static String Samba_NetBiosName2Ip(String NetBiosName);
    public native static long Samba_QueryDomainList(Object num);
    public native static long Samba_QueryServerList(Object num, String domain);
    public native static long Samba_QueryServerShareList(Object ret, Object num, String ip, String username, String password, String domain);
    public native static String Samba_GetDomainList(long handle, int num);
    public native static String Samba_GetServerList(long handle, int num);
    public native static String Samba_GetServerShareList(long handle, int num, Object type);
    public native static void Samba_freeHandle(long handle);
    static
	{
		System.loadLibrary("rtksmb");
	}
}
