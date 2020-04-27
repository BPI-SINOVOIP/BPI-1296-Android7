/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.20 : First draft.
 */

package com.realtek.realtekinitialsettings;

import android.annotation.TargetApi;
import android.app.Activity;
import android.app.ActionBar;
import android.app.ActivityManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.view.MotionEvent;
import android.view.View;
import android.view.KeyEvent;
import android.view.Window;
import android.view.WindowManager;
import android.view.LayoutInflater;
import android.view.View.OnClickListener;
import android.view.View.OnHoverListener;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.ImageView;
import android.widget.ImageButton;
import android.widget.ArrayAdapter;
import android.widget.AdapterView;
import android.widget.AbsListView;
import android.widget.Toast;
import android.graphics.Color;
import android.graphics.Typeface;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AbsListView.OnScrollListener;
import android.util.Log;
import android.content.Context;
import android.content.Intent;
import android.util.TypedValue;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.wifi.WifiManager;
import java.util.ArrayList;
import java.util.Locale;

import java.lang.reflect.Method;
import java.lang.reflect.Field;

/**
 *   Realtek InitialSettingsActivity is used to show the initial setting functionalities.
 */
public class InitialSettingsActivity extends Activity implements OnScrollListener {
	public static String TAG = "InitialSettingsActivity";
	public static final int MAX_ITEMS_PER_PAGE = 7;  // display 7 items per page.
	private ListView mLanguageMenuList; 
	private static String[] mSupportLanguageArray;
	private MenuArrayAdapter mAdapter;
	private ImageButton mNextIcon;
	private View mCurrentFocusView;
	private static ImageView mListViewHeader;
	private static ImageView mListViewFooter;
	private int mCurrentFocusIdx; 
	private int mSupportLanguageCount;
	private ArrayList<Locale> mAvailableLanguage;
//	private ArrayList<String> mAvailableLanguageInString;
	private Handler mWifiHandler;
	private Handler mLanguageHandler;
	private Locale mSelectedLocale;

	/**
	 *  Override this function
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

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

        setContentView(R.layout.activity_initial_settings);

		// header
		mListViewHeader = (ImageView) findViewById(R.id.menu_list_header_image);
		//mListViewHeader.setVisibility(View.INVISIBLE);  // at the beginning, do not show the header.

		// footer
		mListViewFooter = (ImageView) findViewById(R.id.menu_list_footer_image);

		displayHeaderAndFooter();

		// Collect all available supported language list.
		mAvailableLanguage = new ArrayList<Locale>();
		//mAvailableLanguageInString = new ArrayList<String>();
		doLanguageItemPreparation();  // prepare language data

		mLanguageMenuList = (ListView) findViewById(R.id.language_menu);
		//mSupportLanguageArray = mAvailableLanguageInString.toArray(new String[0]);
		mSupportLanguageArray = getResources().getStringArray(R.array.string_array_support_language_list);  // use pre-defined string array for display.
		mSupportLanguageCount = mSupportLanguageArray.length;

		mAdapter = new MenuArrayAdapter(this, R.layout.menu_list_item, mSupportLanguageArray);
		mLanguageMenuList.setAdapter(mAdapter);
		mLanguageMenuList.setOnItemSelectedListener(new ListViewItemSelectedListener());
		mLanguageMenuList.setOnItemClickListener(new ListViewItemClickListener());
//		mLanguageMenuList.setOnScrollListener(this);

		mNextIcon = (ImageButton) findViewById(R.id.next_icon);
		mNextIcon.setOnClickListener(new OnClickListener() {  // register the listener for the icon's click event
			@Override
			public void onClick(View view) {
				Log.d(TAG, "mNextIcon, onClick = " + mCurrentFocusIdx);
   				doLanguageSetting(mAvailableLanguage.get(mCurrentFocusIdx));

				goToWirelessSettings();
			}
		});

		mNextIcon.setOnHoverListener(new OnHoverListener() {
			@Override
			public boolean onHover(View view, MotionEvent event) {
				int what = event.getAction();
				switch(what) {
					case MotionEvent.ACTION_HOVER_ENTER:
					case MotionEvent.ACTION_HOVER_MOVE:
						//Log.d(TAG, "mNextIcon, onHover = MotionEvent.ACTION_HOVER_ENTER/MOVE");
						highlightNextIcon(true);
						break;
					case MotionEvent.ACTION_HOVER_EXIT:
						//Log.d(TAG, "mNextIcon, onHover = MotionEvent.ACTION_HOVER_EXIT");
						highlightNextIcon(false);
						break;
				}
				return true;
			}
		});

		mCurrentFocusView = mLanguageMenuList;  // we set the current focus view to mLanguageMenuList
		mCurrentFocusIdx = 0;

    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onPostCreate(Bundle savedInstanceState) {
        super.onPostCreate(savedInstanceState);

		mLanguageMenuList.setChoiceMode(ListView.CHOICE_MODE_SINGLE);
		mLanguageMenuList.requestFocus();
		mLanguageMenuList.setSelection(0);
		mLanguageMenuList.setItemChecked(0, true);

		// enable wifi first, it will be used on next step.
		mWifiHandler = new Handler();
		mWifiHandler.postDelayed(wifiEnableRunnable, 500);  // executed after 500ms
    }

	/**
	 *  Runnable instance to execute the wifi enable action in any handler.
	*/
	final Runnable wifiEnableRunnable = new Runnable() {
		public void run() {
			WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
			if(wifiManager.isWifiEnabled() == false) {  // disable, let's enable the wifi.
				wifiManager.setWifiEnabled(true);
			}
		}
	};

	/**
	 *  Runnable instance to execute the language switch action in any handler.
	*/
	final Runnable languageSwitchRunnable = new Runnable() {
		public void run() {
    		try {
       			Configuration conf = null;
       			Object amnObj = null;
    
    			// This is the way what LocalePicker did from com.android.internal.app package.
    			Class activitymanagernClass = Class.forName("android.app.ActivityManagerNative");
       			Method methodGetConfiguration = activitymanagernClass.getMethod("getConfiguration");
       			methodGetConfiguration.setAccessible(true);
        
       			Method methodGetDefault = activitymanagernClass.getMethod("getDefault");
       			methodGetDefault.setAccessible(true);
       			amnObj = methodGetDefault.invoke(activitymanagernClass);
        
       			conf = (Configuration) methodGetConfiguration.invoke(amnObj);
       
       	        Class configClass = conf.getClass();
               	Field field = configClass.getField("userSetLocale");
               	field.setBoolean(conf, true);
        
       			conf.locale = mSelectedLocale;  //locale;
                	
       			Method methodUpdateConfiguration = activitymanagernClass.getMethod("updateConfiguration", Configuration.class);
               	methodUpdateConfiguration.setAccessible(true);
               	methodUpdateConfiguration.invoke(amnObj, conf);
    		} catch (Exception e) {

			}
		}
	};

	/**
	 *  Override this function
	*/
    @Override
    protected void onResume() {
        super.onResume();
    }

	/**
	 *  Override this function
	*/
    @Override
    protected void onPause() {
        super.onPause();
		if(mWifiHandler != null) {
			mWifiHandler.removeCallbacks(wifiEnableRunnable);
		}

		if(mLanguageHandler != null) {
			mLanguageHandler.removeCallbacks(languageSwitchRunnable);
		}
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

				if(mLanguageMenuList != null && mNextIcon != null && mNextIcon == getCurrentFocus()) {
					mLanguageMenuList.requestFocus();
					mNextIcon.clearFocus();
					mCurrentFocusView = mLanguageMenuList;

					displayHighlightInItem(false);  // not highlight
					return true;
				}
				break;
			case KeyEvent.KEYCODE_DPAD_RIGHT:
            	Log.d(TAG,"onKeyDown: RIGHT, mCurrentFocusIdx = " + mCurrentFocusIdx);

				if(mNextIcon != null && mNextIcon != getCurrentFocus()) {
					mNextIcon.setFocusable(true);
					mNextIcon.requestFocus();
					mCurrentFocusView = mNextIcon;

					displayHighlightInItem(true);  // highlight
					return true;
				}
				break;
			case KeyEvent.KEYCODE_DPAD_UP:
            	Log.d(TAG,"onKeyDown: UP");
				if(mCurrentFocusView == mNextIcon)  // no need to do UP in mNextIcon
					return true;
				break;
			case KeyEvent.KEYCODE_DPAD_DOWN:
            	Log.d(TAG,"onKeyDown: DOWN");
				return true;
/*
				if(mCurrentFocusView == mNextIcon)  // no need to do DOWN in mNextIcon
					return true;
				else
            		Log.d(TAG,"onKeyDown: DOWN, focus is mLanguageMenuList");
				break;
*/
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
	 *  Helper function to load all available language.
	*/
	private void doLanguageItemPreparation() {
		String[] str = Resources.getSystem().getAssets().getLocales();  // get languages that we support (Display in the Settings), zh_CN/en_US/zh_TW/en.
   		Locale[] l = Locale.getAvailableLocales();  // get all languages Android support  

   		for (Locale ll : l) {
   			//Log.d(TAG, "locale = " + ll.toString());
   			for (String s : str) {
				// Locale, Android 4.2.2 => zh_CN, zh_TW, en_US
				// Locale, Android 4.4 => zh_HANS_CN, zh_HANT_TW, en_US
				// Since the text is changed from 4.4, we should do manual comparison.
				String[] langCode = ll.toString().split("_");
				String newLangCode = "en";
				if(langCode.length == 1) {  // uk
					newLangCode = langCode[0];
				} else if(langCode.length == 2) {  // en_US 
					newLangCode = langCode[0] + "_" + langCode[1];
				} else if(langCode.length == 3) {  // zh_HANT_TW
					newLangCode = langCode[0] + "_" + langCode[2];
				} 
   				//Log.d(TAG, "newLangCode = " + newLangCode);

   				//if(ll.toString().equals(s) && s.length() > 2) {  // only working on Android 4.2.2
   				if(newLangCode.equals(s) && s.length() > 2) { // working on Android 4.4
  					mAvailableLanguage.add(ll);
//					mAvailableLanguageInString.add(ll.getDisplayLanguage());
   					Log.d(TAG, "locale cmp, " + ll.toString() + " = " + s + ", DisplayName = " + ll.getDisplayLanguage() + " v = " + ll.getDisplayVariant());
   				}
   			}
   		}
   		Log.d(TAG, "mAvailableLanguage count =  " + mAvailableLanguage.size());
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
		if(mSupportLanguageCount > InitialSettingsActivity.MAX_ITEMS_PER_PAGE && mCurrentFocusIdx < mSupportLanguageCount - 1) {
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
		int idx = mCurrentFocusIdx - mLanguageMenuList.getFirstVisiblePosition();
		View v = mLanguageMenuList.getChildAt(idx);  // idx is visible index on the listview which displayed currently.

		if(v == null)
			return false;

		TextView t = (TextView) v.findViewById(R.id.display_item);

		if(t == null)
			return false;

		if(highlight) {  // highlight
			t.setBackgroundResource(R.drawable.list_bg_focus);
			t.setWidth(1086);
		} else {  // no highlight
			t.setBackgroundResource(R.drawable.list_bg_none);
			t.setWidth(934);
			mAdapter.notifyDataSetChanged();
		}
		return true;
	}

	/**
	 *  Helper function to do real action - be careful! It will halts a moment to reload setting.
	 *  Parameter:
	 *  	locale: target locale variable. 
	*/
	private void doLanguageSetting(Locale locale) {
		// From Android 4.4, the available locales we get is differ that Android 4.2.2, until now we only know zh_CN/zh_TW are different.
		// Originally, the locale looks like "zh_CN/zh_TW", but it change to "zh_HANS_CN/zh_HANT_TW" and can not be useed as input paramter to change language setting now.
		// So we put a workaround here for zh_CN, zh_TW locales, other locales are same as beofre.
		if(locale.toString().equals("zh_HANT_TW")) {  // zh_TW
			mSelectedLocale = Locale.TAIWAN;
		} else if(locale.toString().equals("zh_HANS_CN")) {  // zh_CN
			mSelectedLocale = Locale.CHINA;
		} else {  // other locales
			mSelectedLocale = locale;
		}
		Log.d(TAG, "doLanguageSetting(), locale = " + mSelectedLocale.toString());
		mLanguageHandler = new Handler();
		mLanguageHandler.post(languageSwitchRunnable); 
/*
		try {
   			Configuration conf = null;
   			Object amnObj = null;

			// This is the way what LocalePicker did from com.android.internal.app package.
			Class activitymanagernClass = Class.forName("android.app.ActivityManagerNative");
   			Method methodGetConfiguration = activitymanagernClass.getMethod("getConfiguration");
   			methodGetConfiguration.setAccessible(true);
    
   			Method methodGetDefault = activitymanagernClass.getMethod("getDefault");
   			methodGetDefault.setAccessible(true);
   			amnObj = methodGetDefault.invoke(activitymanagernClass);
    
   			conf = (Configuration) methodGetConfiguration.invoke(amnObj);
   
   	        Class configClass = conf.getClass();
           	Field field = configClass.getField("userSetLocale");
           	field.setBoolean(conf, true);
    
   			conf.locale = locale;
            	
   			Method methodUpdateConfiguration = activitymanagernClass.getMethod("updateConfiguration", Configuration.class);
           	methodUpdateConfiguration.setAccessible(true);
           	methodUpdateConfiguration.invoke(amnObj, conf);
		} catch (Exception e) {

		}
*/
	}

	/**
	 *  Helper function to go to next page - Wireless Settings
	*/
	private void goToWirelessSettings() {
		WifiManager wifiManager = (WifiManager) getSystemService(Context.WIFI_SERVICE);
		if(wifiManager.isWifiEnabled() == true) {
			// Launch next activity, second page.
			Intent intent = new Intent(this, InitialWirelessSettingsActivity.class);
			startActivity(intent);
		} else {
			//Toast.makeText(this, "wifiManager.isWifiEnabled() == false", Toast.LENGTH_LONG).show();
			Intent intent = new Intent(this, InitialPrepareWirelessSettingsActivity.class);
			startActivity(intent);
		}
	}

	/**
	 *  Helper function to highlight the next icon.
	 *  Parameter:
	 *  	highlight: focus/unfocus the image
	*/
	private void highlightNextIcon(boolean highlight) {
		if(highlight == true) {
			mNextIcon.setBackgroundResource(R.drawable.btn_next_focus);
		} else {
			mNextIcon.setBackgroundResource(R.drawable.btn_next_none);
		}
	}

	/**
	 *  Helper class to change the focus/unfocus image for each item.
	*/
    private class ListViewItemSelectedListener implements OnItemSelectedListener {

        @Override
        public void onItemSelected(AdapterView<?> parent, View view, int position , long id)
        {
            Log.d(TAG,"onItemSelected:" + position);
            //Log.d(TAG,"onItemSelected, getFirstVisiblePosition:" + mLanguageMenuList.getFirstVisiblePosition());
            //Log.d(TAG,"onItemSelected, getChildCount:" + mLanguageMenuList.getChildCount());
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
			Log.d(TAG, "onItemClick, position = " + position + ", id = " + id);

			mCurrentFocusIdx = position;

			// do language setting.
			doLanguageSetting(mAvailableLanguage.get(mCurrentFocusIdx));
			goToWirelessSettings();
		}
	}

}
