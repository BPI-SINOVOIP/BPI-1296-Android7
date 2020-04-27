/* * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 * 		2013.12.20 : First draft, wifi functions are copied from Settings app.
 */

package com.realtek.realtekinitialsettings;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActionBar;
import android.app.ActivityManager;
import android.app.Dialog;
import android.content.BroadcastReceiver;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnKeyListener;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.MotionEvent;
import android.view.View;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.LayoutInflater;
import android.view.View.OnClickListener;
import android.view.View.OnHoverListener;
import android.view.Gravity;
import android.view.Window;
import android.view.WindowManager;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AbsListView;
import android.widget.Toast;
import android.widget.LinearLayout;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.AnimationDrawable;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AbsListView.OnScrollListener;
import android.util.Log;
import android.util.TypedValue;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.wifi.ScanResult;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WpsInfo;
import android.os.SystemProperties;
import java.util.Collection;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;
import java.util.Locale;

import java.lang.reflect.Method;
import java.lang.reflect.Field;
import java.util.concurrent.atomic.AtomicBoolean;

import java.util.Timer;
import java.util.TimerTask;

/**
 *   Realtek InitialWirelessSettingsActivitY is used to show the initial setting functionalities.
 */
public class InitialWirelessSettingsActivity extends Activity implements OnScrollListener {
	public static String TAG = "InitialWirelessSettingsActivity";
	public static final int MAX_ITEMS_PER_PAGE = 7;  // display 7 items per page.
	private ListView mWirelessAPMenuList; 
	private MenuArrayAdapterEx mAdapter;
	private ImageButton mSkipIcon;
	private ImageButton mRefreshIcon;
	private View mCurrentFocusView;
	private static ImageView mListViewHeader;
	private static ImageView mListViewFooter;
	private int mCurrentFocusIdx; 
	private int mSupportLanguageCount;
	
	private WifiManager mWifiManager;
	private WifiManager.ActionListener mConnectListener;
	private ConnectivityManager mConnectivityManager;
    private final IntentFilter mFilter;
    private final BroadcastReceiver mReceiver;
    private final Scanner mScanner;	
    private AtomicBoolean mConnected = new AtomicBoolean(false);
    private DetailedState mLastState;
    private WifiInfo mLastInfo;		
    private static final int WIFI_RESCAN_INTERVAL_MS = 10 * 1000;  // Combo scans can take 5-6s to complete - set to 10s.
	private ArrayList<AccessPoint> mAccessPointsArray;
	private int mNetworkDescriptionId;  // the ID of the newly created network description.
	private Dialog mSecurityInputDialog;
	private Dialog mSecurityResultDialog;
	private String mSecurityPassword;
	private boolean mConnectionResult;
	private AccessPoint mCurrentSelectedAP;  // currently selected AP
	private DetailedState mOnConnectState;  // used to store the lastest connection state.
	private DetailedState mOnConnectPreviousState;  // used to store the previous connection state.
	private Activity mActivity;
	private boolean mDoConnectByThis;  // we use this variable to record the connection is issued by this app, because Android did provide auto connect mechanism if this system connect the network before.
	private Dialog mConnectingDialog;
	
	/**
	 *  Constructor
	*/
	public InitialWirelessSettingsActivity() {
        mFilter = new IntentFilter();
        mFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
        mFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
        mFilter.addAction(WifiManager.NETWORK_IDS_CHANGED_ACTION);
        mFilter.addAction(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION);
        mFilter.addAction(WifiManager.CONFIGURED_NETWORKS_CHANGED_ACTION);
        mFilter.addAction(WifiManager.LINK_CONFIGURATION_CHANGED_ACTION);
        mFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        mFilter.addAction(WifiManager.RSSI_CHANGED_ACTION);

        mReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                handleEvent(context, intent);
            }
        };

        mScanner = new Scanner();	
		mAccessPointsArray = null;
		mNetworkDescriptionId = 0;
		mSecurityInputDialog = null;
		mSecurityResultDialog = null;
		mSecurityPassword = null;
		mConnectionResult = false;
		mOnConnectState = DetailedState.DISCONNECTED;
		mOnConnectPreviousState = DetailedState.DISCONNECTED;
		mActivity = null;
		mDoConnectByThis = false;
	}
	
	/**
	 *  Override this function 
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

		mActivity = this;

		// Remove app title 
		requestWindowFeature(Window.FEATURE_NO_TITLE);

		// Remove status bar and navigation bar

		// Below method only funcion with Android 4.0 and lower.
		//getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);

		// Below method only funcion with Android 4.1 and higher.
		View decorView = getWindow().getDecorView();
		int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
		decorView.setSystemUiVisibility(uiOptions);
		//ActionBar actionBar = getActionBar();
		//actionBar.hide();

        setContentView(R.layout.activity_initial_wireless_settings);

		// header
		mListViewHeader = (ImageView) findViewById(R.id.menu_list_header_image);
		//mListViewHeader.setVisibility(View.INVISIBLE);  // at the beginning, do not show the header.

		// footer
		mListViewFooter = (ImageView) findViewById(R.id.menu_list_footer_image);

		displayHeaderAndFooter();

		mWirelessAPMenuList = (ListView) findViewById(R.id.wireless_ap_menu);

		// enable wifi
		mWifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);		
		mConnectivityManager = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
		mConnectListener = new WifiManager.ActionListener() {
			public void onSuccess() {
               	//Toast.makeText(mActivity, "mConnectListener.onSuccess()", Toast.LENGTH_LONG).show();
			}

			public void onFailure(int reason) {
               	//Toast.makeText(mActivity, "mConnectListener.onFailure()", Toast.LENGTH_LONG).show();
			}
		};
/*
		if(mWifiManager.isWifiEnabled() == false) {  // disable, let's enable the wifi.
			// since we do enable wifi here is too late, it takes too much time to enable. So we move below code to onPostCreate() of the previous activity.
			mWifiManager.setWifiEnabled(true);
			Log.d(TAG, "Wifi is disable!");
		} else 
			Log.d(TAG, "Wifi is enable!");
*/
		
		mWirelessAPMenuList.setOnItemSelectedListener(new ListViewItemSelectedListener());
		mWirelessAPMenuList.setOnItemClickListener(new ListViewItemClickListener());
//		mWirelessAPMenuList.setOnScrollListener(this);

		mSkipIcon = (ImageButton) findViewById(R.id.skip_icon);
		mSkipIcon.setOnClickListener(new OnClickListener() {  // register the listener for the icon's click event
			@Override
			public void onClick(View view) {
				//Log.d(TAG, "mSkipIcon, onClick = " + mCurrentFocusIdx);
				goToHomeApp();
			}
		});
	
		mSkipIcon.setOnHoverListener(new OnHoverListener() {
			@Override
			public boolean onHover(View view, MotionEvent event) {
				int what = event.getAction();
				switch(what) {
					case MotionEvent.ACTION_HOVER_ENTER:
					case MotionEvent.ACTION_HOVER_MOVE:
						highlightSkipIcon(true);
						break;
					case MotionEvent.ACTION_HOVER_EXIT:
						highlightSkipIcon(false);
						break;
				}
				return true;
			}
		});

		mRefreshIcon = (ImageButton) findViewById(R.id.refresh_icon);
		mRefreshIcon.setOnClickListener(new OnClickListener() {  // register the listener for the icon's click event
			@Override
			public void onClick(View view) {
				//Log.d(TAG, "mRefreshIcon, onClick = " + mCurrentFocusIdx);
				
				mScanner.forceScan();		
				updateAccessPoints();

				mWirelessAPMenuList.requestFocus();
				mRefreshIcon.clearFocus();
				mCurrentFocusView = mWirelessAPMenuList;

				displayHighlightInItem(false);  // not highlight
			
				if(mAdapter != null) {
					mAdapter = null;
					AccessPoint[] tmpAccessAP = new AccessPoint[1];
					mAdapter = new MenuArrayAdapterEx(getApplicationContext(), R.layout.menu_list_wireless_item, mAccessPointsArray.toArray(tmpAccessAP));
					mWirelessAPMenuList.setAdapter(mAdapter);
					mAdapter.notifyDataSetChanged();
				}
				//mAdapter.notifyDataSetChanged();
			}
		});

		mRefreshIcon.setOnHoverListener(new OnHoverListener() {
			@Override
			public boolean onHover(View view, MotionEvent event) {
				int what = event.getAction();
				switch(what) {
					case MotionEvent.ACTION_HOVER_ENTER:
					case MotionEvent.ACTION_HOVER_MOVE:
						highlightRefreshIcon(true);
						break;
					case MotionEvent.ACTION_HOVER_EXIT:
						highlightRefreshIcon(false);
						break;
				}
				return true;
			}
		});

		mRefreshIcon = (ImageButton) findViewById(R.id.refresh_icon);
		mCurrentFocusView = mWirelessAPMenuList;  // we set the current focus view to mWirelessAPMenuList
		mCurrentFocusIdx = 0;
		mScanner.forceScan();		
    }

	/**
	 *  Override this function 
	*/
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

		mWirelessAPMenuList.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
		mWirelessAPMenuList.requestFocus();
		mWirelessAPMenuList.setSelection(0);
		mWirelessAPMenuList.setItemChecked(0, true);
    }

	/**
	 *  Override this function 
	*/
    @Override
    protected void onResume() {
        super.onResume();

		registerReceiver(mReceiver, mFilter);

		updateAccessPoints();  // collect all available AP information.

		AccessPoint[] tmpAccessAP = new AccessPoint[1];
		mAdapter = new MenuArrayAdapterEx(this, R.layout.menu_list_wireless_item, mAccessPointsArray.toArray(tmpAccessAP));
		mWirelessAPMenuList.setAdapter(mAdapter);
    }

	/**
	 *  Override this function 
	*/
    @Override
    protected void onPause() {
        super.onPause();

		unregisterReceiver(mReceiver);
		mScanner.pause();
		
		// disable network
		//mWifiManager.disableNetwork(mNetworkDescriptionId);
    }

	/**
	 *  Override this function in order to move the focus between listview and imageview.
	*/
/*
	@Override
	public boolean dispatchKeyEvent(KeyEvent event) {
		
		if(event.getAction() == KeyEvent.ACTION_DOWN) {
			switch(event.getKeyCode()) {
				case KeyEvent.KEYCODE_DPAD_UP:
				Log.d(TAG, "dispatchKeyEvent(UP), mCurrentFocusIdx = " + mCurrentFocusIdx);
				break;
				case KeyEvent.KEYCODE_DPAD_DOWN:
				Log.d(TAG, "dispatchKeyEvent(DOWN), mCurrentFocusIdx = " + mCurrentFocusIdx);
				break;
			}
		}
	
		return super.dispatchKeyEvent(event);
	}
*/

	/**
	 *  Override this function in order to move the focus between listview and imageview.
	*/
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {

		switch(keyCode) {
			case KeyEvent.KEYCODE_DPAD_LEFT:
            	Log.d(TAG,"onKeyDown: LEFT, mCurrentFocusIdx = " + mCurrentFocusIdx);

				if(mWirelessAPMenuList != null && mSkipIcon != null && mSkipIcon == getCurrentFocus()) {
					mWirelessAPMenuList.requestFocus();
					mSkipIcon.clearFocus();
					mCurrentFocusView = mWirelessAPMenuList;

					displayHighlightInItem(false);  // not highlight
					return true;
				} else if(mWirelessAPMenuList != null && mRefreshIcon != null && mRefreshIcon == getCurrentFocus()) {
					mWirelessAPMenuList.requestFocus();
					mRefreshIcon.clearFocus();
					mCurrentFocusView = mWirelessAPMenuList;

					displayHighlightInItem(false);  // not highlight
					return true;
				}
				break;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
            	Log.d(TAG,"onKeyDown: RIGHT, mCurrentFocusIdx = " + mCurrentFocusIdx);

				if(mSkipIcon != null && mSkipIcon != getCurrentFocus() && mRefreshIcon != getCurrentFocus()) {
					mSkipIcon.setFocusable(true);
					mSkipIcon.requestFocus();
					mCurrentFocusView = mSkipIcon;

					displayHighlightInItem(true);  // highlight
					return true;
				}
				break;
			case KeyEvent.KEYCODE_DPAD_UP:
            	Log.d(TAG,"onKeyDown: UP");

				if(mCurrentFocusView == mSkipIcon)  { // no need to do UP in mSkipIcon
					return true;
				} else if (mCurrentFocusView == mRefreshIcon) {
					mSkipIcon.setFocusable(true);
					mSkipIcon.requestFocus();
					mCurrentFocusView = mSkipIcon;
					return true;	
				}
				break;
			case KeyEvent.KEYCODE_DPAD_DOWN:
            	Log.d(TAG,"onKeyDown: DOWN");

				if(mCurrentFocusView == mSkipIcon) {
					mRefreshIcon.setFocusable(true);
					mRefreshIcon.requestFocus();
					mCurrentFocusView = mRefreshIcon;
				}
				return true;
			default:
				break;
		}		

		return super.onKeyDown(keyCode, event);
	}

	/**
	 *  Register to monitor the state of scrolling in ListView.
	*/

	@Override
	public void onScroll(AbsListView view, int firstVisibleItem, int visibleItemCount, int totalItemCount) {
		Log.d(TAG, "firstVisibleItem = " + firstVisibleItem + ", visibleItemCount = " + visibleItemCount);
		Log.d(TAG, "totalItemCount = " + totalItemCount);
		//Log.d(TAG, "onScroll, view = " + view);
	
	}

	@Override
	public void onScrollStateChanged(AbsListView view, int scrollState) {
		Log.d(TAG, "onScrollStateChanged = " + scrollState);
	}

	/**
	 *  Helper function to determinie whether it's time to show header and footer image.
	*/
	private void displayHeaderAndFooter() {

		// Header
		if(mCurrentFocusIdx <= 0) {  
			mListViewHeader.setVisibility(View.INVISIBLE); 
		} else {
			mListViewHeader.setVisibility(View.VISIBLE); 
		}

		// Footer
		if(mSupportLanguageCount > InitialWirelessSettingsActivity.MAX_ITEMS_PER_PAGE && mCurrentFocusIdx < mSupportLanguageCount - 1) {
			mListViewFooter.setVisibility(View.VISIBLE); 
		} else {
			mListViewFooter.setVisibility(View.INVISIBLE); 
		}

	}

	/**
	 *  Helper function to show the highlight focus on the selected item on listview when user moving the focus to 
	 *  right NextIcon.
	 *  Parameter:
	 *  	highlight: 
	 *			on = highlight
	 *			off = no highlight
	*/
	private boolean displayHighlightInItem(boolean highlight) {
		int idx = mCurrentFocusIdx - mWirelessAPMenuList.getFirstVisiblePosition();
		View v = mWirelessAPMenuList.getChildAt(idx);  // idx is visible index on the listview which displayed currently.

		if(v == null)
			return false;

		LinearLayout layout = (LinearLayout) v.findViewById(R.id.wireless_ap_item);

		if(layout == null)
			return false;

		if(highlight) {  // highlight
			layout.setBackgroundResource(R.drawable.list_bg_focus);
		} else {  // no highlight
			layout.setBackgroundResource(R.drawable.list_bg_none);
			mAdapter.notifyDataSetChanged();
		}
		return true;
	}

    /**
     * Shows the latest access points available with supplimental information like
     * the strength of network and the security for it.
     */
    private void updateAccessPoints() {
        final int wifiState = mWifiManager.getWifiState();

        switch (wifiState) {
            case WifiManager.WIFI_STATE_ENABLED:
                // AccessPoints are automatically sorted with TreeSet.
                Collection<AccessPoint> accessPoints = constructAccessPoints();

                if(accessPoints.size() == 0) {
					Log.d(TAG, "No available wifi AP");
                }
				Log.d(TAG, "Total AP found = " + accessPoints.size());

				if(mAccessPointsArray != null) {  // clean old data
					mAccessPointsArray = null;
				}

				mAccessPointsArray = new ArrayList<AccessPoint>();
				int availableAPIdx = 0;

				for (AccessPoint accessPoint : accessPoints) {  // TODO:// There is a method called toArray() in Collection class can be used to convert to array-based.
					//Log.d(TAG, "SSID = " + accessPoint.ssid + ", security = " + accessPoint.security + ", signal strength = " + accessPoint.getLevel());
					if(accessPoint.getLevel() >= 0) {  // signal strength < 0 is N/A AP, should not display.
						mAccessPointsArray.add(accessPoint);
						availableAPIdx++;
					}
				}
				Log.d(TAG, "Total AP w/ signal strength > 0 found = " + availableAPIdx);
				
				// In some situation, the scan result only contains certain AP with signal strength = -1, since we filter them in above section, add fake AP to show the string that user can understand what
				// happen.
				if(availableAPIdx == 0) {  // if there is no availabel AP, put fake one.
					AccessPoint fakeAP = new AccessPoint(this);
					fakeAP.generateOpenNetworkConfig();
					fakeAP.ssid = getResources().getString(R.string.wifi_empty_list_wifi_press_refresh);
					mAccessPointsArray.add(fakeAP);
					availableAPIdx++;
					Log.d(TAG, "Total AP w/ signal strength > 0 found (fakeAP) = " + availableAPIdx);
				}

                break;

            case WifiManager.WIFI_STATE_ENABLING:
				Log.d(TAG, "updateAccessPoints(), WifiManager.WIFI_STATE_ENABLING");
                break;

            case WifiManager.WIFI_STATE_DISABLING:
				Log.d(TAG, "updateAccessPoints(), WifiManager.WIFI_STATE_DISABLING");
                break;

            case WifiManager.WIFI_STATE_DISABLED:
				Log.d(TAG, "updateAccessPoints(), WifiManager.WIFI_STATE_DISABLED");
                break;
        }
    }

    /**
     *  Returns sorted list of access points 
     */
    private List<AccessPoint> constructAccessPoints() {
        ArrayList<AccessPoint> accessPoints = new ArrayList<AccessPoint>();
        /** Lookup table to more quickly update AccessPoints by only considering objects with the
         * correct SSID.  Maps SSID -> List of AccessPoints with the given SSID.  */
        Multimap<String, AccessPoint> apMap = new Multimap<String, AccessPoint>();

        final List<WifiConfiguration> configs = mWifiManager.getConfiguredNetworks();
        if (configs != null) {
            for (WifiConfiguration config : configs) {
                AccessPoint accessPoint = new AccessPoint(this, config);
                accessPoint.update(mLastInfo, mLastState);
                accessPoints.add(accessPoint);
				//Log.d(TAG, "constructAccessPoints[1] SSID = " + accessPoint.ssid + ", security = " + accessPoint.security + ", signal strength = " + accessPoint.getLevel());
                apMap.put(accessPoint.ssid, accessPoint);
            }
        }

        final List<ScanResult> results = mWifiManager.getScanResults();
        if (results != null) {
            for (ScanResult result : results) {
                // Ignore hidden and ad-hoc networks.
                if (result.SSID == null || result.SSID.length() == 0 ||
                        result.capabilities.contains("[IBSS]")) {
                    continue;
                }

                boolean found = false;
                for (AccessPoint accessPoint : apMap.getAll(result.SSID)) {
                    if (accessPoint.update(result))
                        found = true;
                }
                if (!found) {
                    AccessPoint accessPoint = new AccessPoint(this, result);
                    accessPoints.add(accessPoint);
					//Log.d(TAG, "constructAccessPoints[2] SSID = " + accessPoint.ssid + ", security = " + accessPoint.security + ", signal strength = " + accessPoint.getLevel());
                    apMap.put(accessPoint.ssid, accessPoint);
                }
            }
        }

        // Pre-sort accessPoints to speed preference insertion
        Collections.sort(accessPoints);
        return accessPoints;
    }
	
    /**
     *  A restricted multimap for use in constructAccessPoints
     */
    private class Multimap<K,V> {
        private HashMap<K,List<V>> store = new HashMap<K,List<V>>();
        /** retrieve a non-null list of values with key K */
        List<V> getAll(K key) {
            List<V> values = store.get(key);
            return values != null ? values : Collections.<V>emptyList();
        }

        void put(K key, V val) {
            List<V> curVals = store.get(key);
            if (curVals == null) {
                curVals = new ArrayList<V>(3);
                store.put(key, curVals);
            }
            curVals.add(val);
        }
    }
	
    /**
     *   Main event handling function when receving broadcast event from other modules.
     */
	private void handleEvent(Context context, Intent intent) {
        String action = intent.getAction();
        if (WifiManager.WIFI_STATE_CHANGED_ACTION.equals(action)) {
            updateWifiState(intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,
                    WifiManager.WIFI_STATE_UNKNOWN));
        } else if (WifiManager.SCAN_RESULTS_AVAILABLE_ACTION.equals(action) ||
                WifiManager.CONFIGURED_NETWORKS_CHANGED_ACTION.equals(action) ||
                WifiManager.LINK_CONFIGURATION_CHANGED_ACTION.equals(action)) {
                updateAccessPoints();
        } else if (WifiManager.SUPPLICANT_STATE_CHANGED_ACTION.equals(action)) {
            //Ignore supplicant state changes when network is connected
            //TODO: we should deprecate SUPPLICANT_STATE_CHANGED_ACTION and
            //introduce a broadcast that combines the supplicant and network
            //network state change events so the apps dont have to worry about
            //ignoring supplicant state change when network is connected
            //to get more fine grained information.
            SupplicantState state = (SupplicantState) intent.getParcelableExtra(
                    WifiManager.EXTRA_NEW_STATE);
            if (!mConnected.get() && SupplicantState.isHandshakeState(state)) {
                updateConnectionState(WifiInfo.getDetailedStateOf(state));
            }

/*
			if(state == SupplicantState.COMPLETED) {
				Log.d(TAG, "************************ handleEvent(), SupplicantState == COMPLETED");
               	//Toast.makeText(mActivity, "SupplicantState == COMPLETED", Toast.LENGTH_LONG).show();
				if(SupplicantState.isHandshakeState(state) == true) {
					mConnectionResult = true;
					showSecurityResultDialog(true, mWifiManager.getConnectionInfo().getSSID());
				} else {
					mConnectionResult = false;
					showSecurityResultDialog(false, null);
				}
			} else if(state == SupplicantState.DISCONNECTED) {
				Log.d(TAG, "************************ handleEvent(), SupplicantState == DISCONNECTED");
               	//Toast.makeText(mActivity, "SupplicantState == DISCONNECTED", Toast.LENGTH_LONG).show();
			}
*/
        } else if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equals(action)) {
            NetworkInfo info = (NetworkInfo) intent.getParcelableExtra(
                    WifiManager.EXTRA_NETWORK_INFO);
            mConnected.set(info.isConnected());
			Log.d(TAG, "************************ handleEvent(), isConnected() = " + info.isConnected());
			if(info.isConnected() == false) {
				mConnectionResult = false;
			} else {
				mConnectionResult = true;
				showSecurityResultDialog(true, mWifiManager.getConnectionInfo().getSSID());
			}
			Log.d(TAG, "************************ handleEvent(), isFailover() = " + info.isFailover());
			
            updateAccessPoints();
            updateConnectionState(info.getDetailedState());

        } else if (WifiManager.RSSI_CHANGED_ACTION.equals(action)) {
            updateConnectionState(null);
        }
    }
	
    /**
     *   Update connection state
     */
    private void updateConnectionState(DetailedState state) {
        /* sticky broadcasts can call this when wifi is disabled */
        if (!mWifiManager.isWifiEnabled()) {
            mScanner.pause();
            return;
        }

        // set dialog text
        TextView dialog_wireless_connection_state = null;
		String infoText = null;
		if(mConnectingDialog != null) {
	        dialog_wireless_connection_state = (TextView) mConnectingDialog.findViewById(R.id.dialog_wireless_connection_state);
		}
	
		Log.d("stanely", "mOnConnectPreviousState = " + mOnConnectPreviousState + ", mOnConnectState = " + state);
		mOnConnectPreviousState = mOnConnectState;
        if (state == DetailedState.DISCONNECTED) {
			Log.i(TAG, "updateConnectionState = DetailedState.DISCONNECTED");
			mOnConnectState = DetailedState.DISCONNECTED;
	
	        infoText = getResources().getString(R.string.wifi_state_disconnected);
        } else if (state == DetailedState.CONNECTING) {
			Log.i(TAG, "updateConnectionState = DetailedState.CONNECTING");
			mOnConnectState = DetailedState.CONNECTING;
	        infoText = getResources().getString(R.string.wifi_state_connecting);
			if(mOnConnectPreviousState == DetailedState.AUTHENTICATING) {  // connection failed due to incorrect password or etc.
				mConnectionResult = false;
				showSecurityResultDialog(false, null);
			}
        } else if (state == DetailedState.AUTHENTICATING) {
			Log.i(TAG, "updateConnectionState = DetailedState.AUTHENTICATING");
			mOnConnectState = DetailedState.AUTHENTICATING;
	        infoText = getResources().getString(R.string.wifi_state_authenticating);
        } else if (state == DetailedState.OBTAINING_IPADDR) {
			Log.i(TAG, "updateConnectionState = DetailedState.OBTAINING_IPADDR");
			mOnConnectState = DetailedState.OBTAINING_IPADDR;
            mScanner.pause();
	        infoText = getResources().getString(R.string.wifi_state_obtaining_ipaddr);
		} else if (state == DetailedState.CONNECTED) {
			Log.i(TAG, "updateConnectionState = DetailedState.CONNECTED");
			mOnConnectState = DetailedState.CONNECTED;
			// D/stanely ( 2220): mOnConnectPreviousState = OBTAINING_IPADDR, mOnConnectState = CAPTIVE_PORTAL_CHECK
			//if(mOnConnectPreviousState == DetailedState.OBTAINING_IPADDR) {  // connection successfully
				//mConnectionResult = true;
				//showSecurityResultDialog(true, mLastInfo.getSSID());
			//}
	        infoText = getResources().getString(R.string.wifi_state_connected);
		} else if (state == DetailedState.FAILED) {
			Log.i(TAG, "updateConnectionState = DetailedState.FAILED");
			mOnConnectState = DetailedState.FAILED;
	        infoText = getResources().getString(R.string.wifi_state_failed);
		} else if (state == DetailedState.BLOCKED) {
			Log.i(TAG, "updateConnectionState = DetailedState.BLOCKED");
			mOnConnectState = DetailedState.BLOCKED;
	        infoText = getResources().getString(R.string.wifi_state_blocked);
        } else {
            mScanner.resume();
			mOnConnectState = DetailedState.DISCONNECTED;
	        infoText = getResources().getString(R.string.wifi_state_disconnected);
        }

		if(mConnectingDialog != null && dialog_wireless_connection_state != null) {
	        dialog_wireless_connection_state.setText(infoText);
		}

        mLastInfo = mWifiManager.getConnectionInfo();
        if (state != null) {
            mLastState = state;
        }

		// to monitor connection state
		/*
		Log.i(TAG,"network state: " + state.toString());
		switch(state)
		{
			case FAILED:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.FAILED");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.FAILED", Toast.LENGTH_LONG).show();
			}
			break;				
			case CONNECTED:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.CONNECTED");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.CONNECTED", Toast.LENGTH_LONG).show();
			}
			break;			
			case AUTHENTICATING:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.AUTHENTICATING");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.AUTHENTICATING", Toast.LENGTH_LONG).show();
			}
			break;
			case CONNECTING:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.CONNECTING");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.CONNECTING", Toast.LENGTH_LONG).show();
			}
			break;
			case OBTAINING_IPADDR:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.OBTAINING_IPADDR");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.OBTAINING_IPADDR", Toast.LENGTH_LONG).show();
			}
			break;
			case DISCONNECTED:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.DISCONNECTED");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.DISCONNECTED", Toast.LENGTH_LONG).show();
			}
			break;
			case SCANNING:
			{
				Log.i(TAG, "updateConnectionState = DetailedState.SCANNING");
               	//Toast.makeText(this, "updateConnectionState = DetailedState.SCANNING", Toast.LENGTH_LONG).show();
			}
			break;
		}		
		*/
    }
	
    /**
     *   Update Wifi state
     */
    private void updateWifiState(int state) {

        switch (state) {
            case WifiManager.WIFI_STATE_ENABLED:
                mScanner.resume();
                return; // not break, to avoid the call to pause() below

            case WifiManager.WIFI_STATE_ENABLING:
                
                break;

            case WifiManager.WIFI_STATE_DISABLED:
                
                break;
        }

        mLastInfo = null;
        mLastState = null;
        mScanner.pause();
    }	
	
    /**
     *   Used to do connection.
	 *  Parameter:
	 *  	ap: AccessPoint instance which we are trying do connection. 
	 *		securitykey : key for WEP/WPA	
     */
	private boolean onConnect(AccessPoint ap, String securitykey) {

		// check exist connection here, if it has just disconnect old one first.
		//if(mConnectivityManager != null & mConnectivityManager.getNetworkInfo(ConnectivityManager.TYPE_WIFI).isConnected()) {
			if(mWifiManager != null) {
				mWifiManager.disconnect();
				mOnConnectState = DetailedState.DISCONNECTED;
			}
		//}

		showConnectingDialog();

		WifiConfiguration wc = new WifiConfiguration();
		wc.SSID = "\"" + ap.ssid + "\"";
		
		if (AccessPoint.SECURITY_NONE == ap.security)  // no security
		{
			wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			wc.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
			wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
			
			wc.allowedAuthAlgorithms.clear();
			
			wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
			wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
		}
		else if (AccessPoint.SECURITY_WEP == ap.security)  // wep security
		{
			wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
			wc.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
			wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
			wc.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
			wc.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.SHARED);
			wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
			wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);

			wc.wepKeys[0] = "\"".concat(securitykey).concat("\"");
			wc.wepTxKeyIndex = 0;

		}
		else  // wpa
		{
			wc.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
			wc.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
			wc.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
			wc.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
			wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
			wc.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
			wc.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
			
			wc.preSharedKey = "\"".concat(securitykey).concat("\"");

		}

//		mNetworkDescriptionId = mWifiManager.addNetwork(wc);
		
//		boolean res = mWifiManager.enableNetwork(mNetworkDescriptionId, true);
		//Log.d(TAG, "enableNetwork returned " + res);	

		mWifiManager.connect(wc, mConnectListener);
		mDoConnectByThis = true;

		//if(res == true) {  // connecting successfully.
			//mConnectionResult = true;
			//showSecurityResultDialog(true, ap.ssid);
			//goToAccountSettings();
		//} else {  // connecting failed.
			//mConnectionResult = false;
			//showSecurityResultDialog(false, null);
		//}
		return true; //res;
	}
	
    /**
     *   Show the result to user to know the wifi connecting is successful or failed.
	 *  Parameter:
	 *  	success: display success or fail image.
	 *		ssid : when the result is success, append the ssid on the dialog.
     */
	private boolean showSecurityResultDialog(boolean success, String ssid) {
		if(mSecurityResultDialog != null && mSecurityResultDialog.isShowing()) {
			return false;
		}

        if(mConnectingDialog != null && mConnectingDialog.isShowing()) {
            mConnectingDialog.cancel();
        }

			
		if(mDoConnectByThis == false)
			return false;

		mSecurityResultDialog = new Dialog(InitialWirelessSettingsActivity.this, R.style.SecurityDialogTheme);
		mSecurityResultDialog.setContentView(R.layout.dialog_security_result);
		//mSecurityResultDialog.setCancelable(false);

		// adjust the attributes of dialog in order to fit the design
		Window dialogWin = mSecurityResultDialog.getWindow();
		WindowManager.LayoutParams lp = dialogWin.getAttributes();
		//dialogWin.setGravity(Gravity.LEFT | Gravity.TOP);
		dialogWin.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
		//lp.x = 460;
		//lp.y = 300;
		lp.width = 1000;
		lp.height = 300;
		dialogWin.setAttributes(lp);
		
		mSecurityResultDialog.setOnKeyListener(dialogResultOnKeyListener);
	
		// set dialog text	
		TextView dialog_security_result_text = (TextView) mSecurityResultDialog.findViewById(R.id.dialog_security_result_text);
		if(success == true) {
			String firstText = getResources().getString(R.string.security_result_success);
			String dotText = getResources().getString(R.string.security_result_success_dot);
			String lastText = getResources().getString(R.string.security_result_quit);
			String allText = firstText + " " + ssid + dotText + lastText;  // TODO:// From the art design, using different color to highlight AP's name.
			dialog_security_result_text.setText(allText);
		} else {
			String firstText = getResources().getString(R.string.security_result_fail);
			String allText = firstText;
			dialog_security_result_text.setText(allText);
		}
	
		// set icon
		ImageView dialog_security_result_icon = (ImageView) mSecurityResultDialog.findViewById(R.id.dialog_security_result_icon);
		if(success == true) {
			dialog_security_result_icon.setImageDrawable(getResources().getDrawable(R.drawable.icon_connect_ap));
		} else {
			dialog_security_result_icon.setImageDrawable(getResources().getDrawable(R.drawable.icon_connect_ap_fail));
		}

		mSecurityResultDialog.show();		
		return true;
	}

    /**
     *   Show the dialog to let user input the password (key) for WEP/WPA.
	 *  Parameter:
	 *  	ap: AccessPoint instance.
     */
	private boolean showSecurityInputDialog(AccessPoint ap) {
		mSecurityInputDialog = new Dialog(InitialWirelessSettingsActivity.this, R.style.SecurityDialogTheme);
		mSecurityInputDialog.setContentView(R.layout.dialog_security);

		// adjust the attributes of dialog in order to fit the design
		Window dialogWin = mSecurityInputDialog.getWindow();
		WindowManager.LayoutParams lp = dialogWin.getAttributes();
		//dialogWin.setGravity(Gravity.LEFT | Gravity.TOP);
		dialogWin.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
		//lp.x = 460;
		//lp.y = 300;
		lp.width = 1000;
		lp.height = 480;
		dialogWin.setAttributes(lp);
		
		mSecurityInputDialog.setOnKeyListener(dialogOnKeyListener);
	
		// set dialog title	
		TextView dialog_security_title = (TextView) mSecurityInputDialog.findViewById(R.id.dialog_security_title);
		dialog_security_title.setText(ap.ssid);
	
		// set signal strength	
		ImageView dialog_security_signal_strength_icon = (ImageView) mSecurityInputDialog.findViewById(R.id.dialog_security_signal_strength_icon);
		int sigStrengthLevel = ap.getLevel();
		int securityType = ap.security;
		dialog_security_signal_strength_icon.setImageResource(Utils.getSignalStrengthImageId(sigStrengthLevel, securityType));

		// set security type text
		TextView dialog_security_type = (TextView) mSecurityInputDialog.findViewById(R.id.dialog_security_type);
		dialog_security_type.setText(ap.getSecurityString(false));
	
		// set password display to "*", not "." 
		EditText dialog_password_input = (EditText) mSecurityInputDialog.findViewById(R.id.dialog_password_input);
		dialog_password_input.setTransformationMethod(new AsteriskPasswordTransformationMethod());
	
		mSecurityInputDialog.show();		
		return true;
	}

	 /**
     *   Show wireless connecting dialog while trying to connect an AP.
     */
	private boolean showConnectingDialog() {
		AnimationDrawable loadingAnimation;

		if(mConnectingDialog != null && mConnectingDialog.isShowing()) {
			return false;
		}
			
//		if(mDoConnectByThis == false)
//			return false;

		mConnectingDialog = new Dialog(InitialWirelessSettingsActivity.this, R.style.SecurityDialogTheme);
		mConnectingDialog.setContentView(R.layout.dialog_wireless_connecting);

		// adjust the attributes of dialog in order to fit the design
		Window dialogWin = mConnectingDialog.getWindow();
		WindowManager.LayoutParams lp = dialogWin.getAttributes();
		//dialogWin.setGravity(Gravity.LEFT | Gravity.TOP);
		dialogWin.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
		//lp.x = 460;
		//lp.y = 300;
		lp.width = 1000;
		lp.height = 300;
		dialogWin.setAttributes(lp);
		
		// set dialog text	
		//TextView dialog_wireless_connection_state = (TextView) mConnectingDialog.findViewById(R.id.dialog_wireless_connection_state);
		//dialog_wireless_connection_state.setText("TTTTTTTTTTTTTTTTTTTTTTT");
	
		// set animated icon
		ImageView dialog_wireless_connecting_image = (ImageView) mConnectingDialog.findViewById(R.id.dialog_wireless_connecting_image);
		dialog_wireless_connecting_image.setBackgroundResource(R.drawable.animation_wireless_connecting);
		loadingAnimation = (AnimationDrawable) dialog_wireless_connecting_image.getBackground();
		loadingAnimation.start();

		mConnectingDialog.show();		
		return true;
	}
	
    /**
     *  Helper function to go to next page - Account Settings
    */
    private void goToAccountSettings() {
        // Launch next activity, third page.
        Intent intent = new Intent(this, InitialAccountSettingsActivity.class);
        startActivity(intent);
    }
	
    /**
     *  Helper function to go to Home page - Launcher app
    */
    private void goToHomeApp() {
		// write to persist.sys.RunRealtekInitialSettings so that do not need to launch this app again.
		String propRes = SystemProperties.get("persist.sys.RunRTKIniSettings");
		Log.d("stanely", "gotToHomeApp(), read persist.sys.RunRTKIniSettings = " + propRes);
		SystemProperties.set("persist.sys.RunRTKIniSettings", "true");
		propRes = SystemProperties.get("persist.sys.RunRTKIniSettings");
		Log.d("stanely", "gotToHomeApp(), read persist.sys.RunRTKIniSettings = " + propRes);

		// exit the RealtekInitialSettings app and go to Home.
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_MAIN);
		intent.addCategory(Intent.CATEGORY_HOME);
		startActivity(intent);
		finish();
	}

	/**
	 *  Helper function to highlight the skip icon.
	 *  Parameter:
	 *      highlight: focus/unfocus the image
	*/
	private void highlightSkipIcon(boolean highlight) {
		if(highlight == true) {
			mSkipIcon.setBackgroundResource(R.drawable.btn_next_focus);
		} else {
			mSkipIcon.setBackgroundResource(R.drawable.btn_next_none);
		}
	}

	/**
	 *  Helper function to highlight the refresh icon.
	 *  Parameter:
	 *      highlight: focus/unfocus the image
	*/
	private void highlightRefreshIcon(boolean highlight) {
		if(highlight == true) {
			mRefreshIcon.setBackgroundResource(R.drawable.btn_refresh_focus);
		} else {
			mRefreshIcon.setBackgroundResource(R.drawable.btn_refresh_none);
		}
	}

	/**
	 *  Helper class to change the focus/unfocus image for each item.
	*/
    private class ListViewItemSelectedListener implements OnItemSelectedListener {

        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position , long id)
        {
            //Log.d(TAG,"onItemSelected:" + position);
            //Log.d(TAG,"onItemSelected, getFirstVisiblePosition:" + mWirelessAPMenuList.getFirstVisiblePosition());
            //Log.d(TAG,"onItemSelected, getChildCount:" + mWirelessAPMenuList.getChildCount());
			mCurrentFocusIdx = position;
			displayHeaderAndFooter();  // keep updating
        }

        @Override
        public void onNothingSelected(AdapterView<?> parent)
        {
            Log.d(TAG, "onNothingSelected:");
        }
    }

	/**
	 *  Helper class to monitor the item click event.
	*/
	private class ListViewItemClickListener implements OnItemClickListener {
		@Override
		public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
			//Log.d(TAG, "onItemClick, position = " + position + ", id = " + id);

			// do wifi connection.
			
			// 1. check security
			AccessPoint tmpAP = mAccessPointsArray.get(mCurrentFocusIdx);
			
			if(tmpAP.security != AccessPoint.SECURITY_NONE)  { // has security
				// popup input dialog to enter the key (password)
				showSecurityInputDialog(tmpAP);
			} else {  // no security, do connecting here.
				onConnect(tmpAP, null);
			}
			
			// 2. connecting w/ security, move to below inside dialogOnKeyListener::onKey().
			mCurrentSelectedAP = tmpAP;
		}
	}

	/**
	 *  An interface we register to monitor onKey() function when display result dialog,
	 * then press any key to exit.
	*/
	private OnKeyListener dialogResultOnKeyListener = new OnKeyListener() {
		@Override
		public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
			// TODO:// Not only KEYCODE_ENTER, but also other keys.
			if(keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
				if(mConnectionResult == true) {
					// remove the dialog.
					mSecurityResultDialog.cancel();	
					goToHomeApp();
					//goToAccountSettings();
				} else {
					// remove the dialog.
					mSecurityResultDialog.cancel();	
				}
				mDoConnectByThis = false;
				return true;
			} else 
				return false;
		}
	};

	/**
	 *  An interface we register to monitor onKey() function when display security input dialog.
	*/
	private OnKeyListener dialogOnKeyListener = new OnKeyListener() {
		@Override
		public boolean onKey(DialogInterface dialog, int keyCode, KeyEvent event) {
			if(keyCode == KeyEvent.KEYCODE_ENTER && event.getAction() == KeyEvent.ACTION_DOWN) {
				EditText passwordText = (EditText) mSecurityInputDialog.findViewById(R.id.dialog_password_input);
				mSecurityPassword = passwordText.getText().toString();

				// for those AP have WEP/WPA security, do connecting here.
				// The length of WPA's password is 8 ~ 63, we should check it before onConnect().
				boolean lenCheckingPass = false;
				if(mCurrentSelectedAP.security == AccessPoint.SECURITY_PSK) {
					lenCheckingPass = (mSecurityPassword.length() >= 8 && mSecurityPassword.length() <= 63);
					//String sss = "lenCheckingPass = " + lenCheckingPass;
                	//Toast.makeText(getApplicationContext(), sss, Toast.LENGTH_LONG).show();
				}

				if(mSecurityPassword != null && mSecurityPassword.length() != 0 && lenCheckingPass == true) {
					Log.d(TAG, "onKey[1], mSecurityPassword = " + mSecurityPassword);
					onConnect(mCurrentSelectedAP, mSecurityPassword);
				} else {  // password is empty.
					Log.d(TAG, "onKey[2], mSecurityPassword = " + mSecurityPassword);
					mConnectionResult = false;
					mDoConnectByThis = true;
					showSecurityResultDialog(false, null);
				}
	
				// remove the dialog.
				mSecurityInputDialog.cancel();	
				return true;
			} else 
				return false;
		}
	};

	/**
	 *  Helper class to do wifi scanning.
	*/
	private class Scanner extends Handler {
        private int mRetry = 0;

        void resume() {
            if (!hasMessages(0)) {
                sendEmptyMessage(0);
            }
        }

        void forceScan() {
            removeMessages(0);
            sendEmptyMessage(0);
        }

        void pause() {
            mRetry = 0;
            removeMessages(0);
        }

        @Override
        public void handleMessage(Message message) {
            //if (mWifiManager.startScanActive()) {  // It's working with Android 4.2.2.
            if (mWifiManager.startScan()) {  // It's working with Android 4.4.
                mRetry = 0;
            } else if (++mRetry >= 3) {
                mRetry = 0;
				Activity activity = getParent();
				if(activity != null) {
                	Toast.makeText(activity, R.string.wifi_fail_to_scan, Toast.LENGTH_LONG).show();
				}
                return;
            }
            sendEmptyMessageDelayed(0, WIFI_RESCAN_INTERVAL_MS);
        }
    }

}
