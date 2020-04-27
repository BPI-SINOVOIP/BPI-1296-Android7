/*
 * Copyright (C) 2013 Realtek Semiconductor Corp.
 * Author: stanely
 * History:
 *      2013.12.20 : First draft.
 */
 
package com.realtek.softwareupdater;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.view.LayoutInflater;
import android.widget.ArrayAdapter;
import android.widget.TextView;
import android.util.TypedValue;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View.OnHoverListener;

/**
 * Implement this class for our purpose
 */
public class MenuArrayAdapter extends ArrayAdapter<String> {
	public static String TAG = "MenuArrayAdapter";
	private Context mContext;
	private int mResId;
	private String[] mContentString;

	public MenuArrayAdapter(Context context, int textViewResourceId, String[] values) {
		super(context, textViewResourceId, values);
		mContext = context;
		mResId = textViewResourceId;
		mContentString = values;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
//		Log.d(TAG, "getView, position = " + position);

		if(convertView == null) {
			LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
			convertView = inflater.inflate(mResId, parent, false);
		}

		final TextView text = (TextView) convertView.findViewById(R.id.display_item);
		text.setBackgroundResource(R.drawable.menu_item_focus_selector);
		text.setText(mContentString[position]);	

		// To get focus/unfocus effect when mouse hover.
		convertView.setOnHoverListener(new OnHoverListener() {
			@Override
			public boolean onHover(View view, MotionEvent event) {
				int what = event.getAction();
				switch(what) {
					case MotionEvent.ACTION_HOVER_ENTER:
					case MotionEvent.ACTION_HOVER_MOVE:
						highlightItem(text);
						break;
					case MotionEvent.ACTION_HOVER_EXIT:
						unHighlightItem(text);
						break;
				}
				return true;
			}
		});

		return convertView;
	}

	public void highlightItem(TextView view) {
		view.setBackgroundResource(R.drawable.list_bg_focus);
		view.setWidth(1086);  // width: 1086px
	}

	public void unHighlightItem(TextView view) {
		view.setBackgroundResource(R.drawable.list_bg_none);
		view.setWidth(934);  // width: 934px
		notifyDataSetChanged();
	}
}


