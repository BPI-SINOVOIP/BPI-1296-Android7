package com.realtek.addon.helper;

import android.view.KeyEvent;

public class ConstantsDef {
	
	// category defines:
	//public static final int NONE=-1;
	public static final int FAV=0;
	public static final int VIDEO=1;
	public static final int ALL=2;
	
	// database defined:
	public static final String DB_TABLE_NAME="APPDB";
	public static final String DB_KEY_CATEGORY="CATEGORY";
	public static final String DB_KEY_APPPKGNAME="PACKAGENAME";
	// COMMENT: use different db name to record favorite and video app list
	public static final String DB_FILE_NAME="rtklauncher_addon.db";
	
	// RTKCOMMENT: special intent added for Realtek launcher
	public static final String CATEGORIED_APP_ADDED_INTENT="com.realtek.launcher.action.CATEGORIED_APP_ADDED";
	public static final String CATEGORIED_APP_REMOVED_INTENT="com.realtek.launcher.action.CATEGORIED_APP_REMOVED";
	
	/**
	 * RTKCOMMENT: intent to launch app page
	 */
	public static final String LAUNCH_APP_CUSTOMIZED_PAGE_INTENT="com.realtek.launcher.action.LAUNCH_APP_PAGE";
	/**
	 * RTKCOMMENT: intent to launch favorite app page
	 */
	public static final String LAUNCH_FAVAPP_CUSTOMIZED_PAGE_INTENT="com.realtek.launcher.action.LAUNCH_FAVAPP_PAGE";
	/**
	 * RTKCOMMENT: intent to launch video app page
	 */
	public static final String LAUNCH_VIDEOAPP_CUSTOMIZED_PAGE_INTENT="com.realtek.launcher.action.LAUNCH_VIDEOAPP_PAGE";
	
	/**
	 * RTKCOMMENT: intent to init DTV/HDMI/Camera engine
	 */
	public static final String INIT_SURFACEVIEW_WIDGET_INTENT="com.realtek.launcher.action.INIT_SURFACEVIEW_WIDGET";
	
	/**
	 * RTKCOMMENT: intent to uninit DTV/HDMI/Camera engine
	 */
	public static final String UNINIT_SURFACEVIEW_WIDGET_INTENT="com.realtek.launcher.action.UNINIT_SURFACEVIEW_WIDGET";
	
	/**
	 * RTKCOMMENT: intent to switch source between DTV/HDMI/Camera
	 */
	public static final String SWITCH_SURFACEVIEW_WIDGET_SOURCE_INTENT="com.realtek.launcher.action.SWITCH_SURFACEVIEW_WIDGET_SOURCE";
	
	/**
	 * RTKCOMMENT: surfaceview widget uninit/init/switch_source intent extra name
	 */
	public static final String UNINIT_EXTRA_NAME=".uninit.condition";
	public static final String INIT_EXTRA_NAME=".init.condition";
	public static final String SOURCE_EXTRA_NAME=".source";
	
	/**
	 * RTKCOMMENT: define system setting package name here.
	 */
	public static final String SETTING_PKG_NAME="com.android.settings";
	public static final String TV_SETTING_PKG_NAME="com.android.tv.settings";
	/**
	 * RTKCOMMENT: define platform media browser package name here.
	 */
	public static final String MEDIA_BROWSER_PKG_NAME="com.rtk.mediabrowser";
	
	/**
	 * RTKCOMMENT: define music app package name.
	 */
	public static final String ANDROID_MUSIC_APP_PKG_NAME="com.android.music";
	
	/**
	 * RTKCOMMENT: define miracast app package name.
	 */
	public static final String RTK_MIRACAST_APP_PKG_NAME="com.example.RTKMiracastSink";
	
	/**
	 * RTKCOMMENT: define KeyCode to show option menu.
	 */
	public static final int LAUNCHER_KEY_OPTION_MENU = KeyEvent.KEYCODE_MENU;
	
	/**
	 * RTKCOMMENT: key to get current device name.
	 * android.provider.Settings.Global.DEVICE_NAME
     * device_name
	 */
	public static final String KEY_DEVICE_NAME = android.provider.Settings.Global.DEVICE_NAME;
	
	/**
	 * RTKCOMMENT: key to get miracast source status
	 * android.provider.Settings.Global.WIFI_DISPLAY_ON
	 * wifi_display_on
	 */
	public static final String KEY_MIRACAST_SOURCE_ONOFF = android.provider.Settings.Global.WIFI_DISPLAY_ON;
	
	/**
	 * RTKCOMMENT: key to get miracast sink status
	 * realtek_setup_miracast_sink_toggle_on
	 * android.provider.Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON
	 */
	public static final String KEY_MIRACAST_SINK_ONOFF = android.provider.Settings.System.REALTEK_SETUP_MIRACAST_SINK_TOGGLE_ON;
	
	/**
	 * RTKCOMMENT: key to get miracast sink pin code.
	 * realtek_setup_miracast_sink_pin_code
	 * android.provider.Settings.System.REALTEK_SETUP_MIRACAST_SINK_PIN_CODE
	 */
	public static final String KEY_MIRACAST_SINK_PIN_CODE = android.provider.Settings.System.REALTEK_SETUP_MIRACAST_SINK_PIN_CODE;
	
	public static final int NET_NONE=-1;
    public static final int NET_WIFI=0;
    public static final int NET_ETH0=1;
    public static final int NET_PPPOE=2;
    
    public static final int P2P=0;
    public static final int SOFTAP=1;
}
