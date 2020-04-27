/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.20 : First draft, use this custom adapter as a provider for Wifi AP Menu.
 */
 
 package com.realtek.realtekinitialsettings;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.widget.ImageView;
import android.util.TypedValue;
import android.util.Log;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.view.MotionEvent;
import android.view.View.OnHoverListener;

/**
 * Implement this class for our purpose
 */
public class MenuArrayAdapterEx extends ArrayAdapter<AccessPoint> {
	public static String TAG = "MenuArrayAdapterEx";
	private Context mContext;
	private int mResId;
	private AccessPoint[] mContentAP;
	private LinearLayout mLayout;

	public MenuArrayAdapterEx(Context context, int textViewResourceId, AccessPoint[] values) {
		super(context, textViewResourceId, values);
		mContext = context;
		mResId = textViewResourceId;
		mContentAP = values;
		mLayout = null;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
//		Log.d(TAG, "getView, position = " + position);
		if(convertView == null) {
			LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(mResId, parent, false);
		}

		mLayout = (LinearLayout) convertView.findViewById(R.id.wireless_ap_item);
		if(mLayout == null)
			return null;
		mLayout.setBackgroundResource(R.drawable.menu_item_focus_selector);

		// AP text
		TextView text = (TextView) convertView.findViewById(R.id.display_item);
		if(text == null)
			return null;
		text.setText(mContentAP[position].ssid);	

		// signal strength
		ImageView image = (ImageView) convertView.findViewById(R.id.wireless_signal_strength_icon);
		if(image == null)
			return null;
		int sigStrengthLevel = mContentAP[position].getLevel();
		int securityType = mContentAP[position].security;
		int imageId = getSignalStrengthImageId(sigStrengthLevel, securityType);
		image.setImageResource(imageId);

		// To get focus/unfocus effect when mouse hover.
		convertView.setOnHoverListener(new OnHoverListener() {
			@Override
			public boolean onHover(View view, MotionEvent event) {
				int what = event.getAction();
				switch(what) {
					case MotionEvent.ACTION_HOVER_ENTER:
					case MotionEvent.ACTION_HOVER_MOVE:
						highlightItem(mLayout);
						break;
					case MotionEvent.ACTION_HOVER_EXIT:
						unHighlightItem(mLayout);
						break;
				}
				return true;
			}
		});


		return convertView;
	}

	public void highlightItem(LinearLayout layout) {
		layout.setBackgroundResource(R.drawable.list_bg_focus);
	}

	public void unHighlightItem(LinearLayout layout) {
		layout.setBackgroundResource(R.drawable.list_bg_none);
		notifyDataSetChanged();
	}

	/* Helper function to let us getting the proper image id for signal
	 * strength display.
	*/
	private int getSignalStrengthImageId(int level, int securityType) {
		int imgId = 0;
		if(AccessPoint.SECURITY_NONE == securityType) {  // no security
			switch(level) {
				case 3:  // full
					imgId = R.drawable.wifi_3;
					break;
				case 2:
					imgId = R.drawable.wifi_2;
					break;
				case 1:
					imgId = R.drawable.wifi_1;
					break;
				case 0:  // 0
				default:
					imgId = R.drawable.wifi_0;
					break;
			}
		} else {  // has security (WEP, WPA)
			switch(level) {
				case 3:  // full
					imgId = R.drawable.wifi_lock_3;
					break;
				case 2:
					imgId = R.drawable.wifi_lock_2;
					break;
				case 1:
					imgId = R.drawable.wifi_lock_1;
					break;
				case 0:  // 0
				default:
					imgId = R.drawable.wifi_lock_0;
					break;
			}
		}

		return imgId;
	}

}


