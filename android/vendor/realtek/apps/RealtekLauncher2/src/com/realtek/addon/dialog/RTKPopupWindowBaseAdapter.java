package com.realtek.addon.dialog;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.StateListDrawable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import com.android.launcher2.Launcher;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

public class RTKPopupWindowBaseAdapter extends BaseAdapter {

	public Launcher mLauncher;
	public int mMode;
	public OnClickListener mClickListener;
	public RTKPopupWindowItemModel[] mContents;
	public LayoutInflater inflater;
	//public RTKPopupWindowHelper mHelper;
	
	//public RTKStateListDrawable bgStateDrawable;
	public BitmapDrawable focusedBG=null;
	public BitmapDrawable unfocusBG=null;
	int drawableWidth;
	int drawableHeight;
	
	public RTKPopupWindowBaseAdapter(Launcher l, 
			int mode,
			RTKPopupWindowItemModel[] contents,
			OnClickListener listener) {
		
		mLauncher=l;
		mMode=mode;
		mContents=contents;
		mClickListener=listener;
		//mHelper=helper;
		
		inflater = (LayoutInflater)mLauncher.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
		
		focusedBG = (BitmapDrawable)mLauncher.getResources().getDrawable(R.drawable.icon_option_focus);
		
		drawableWidth = mLauncher.getResources().getDimensionPixelSize(R.dimen.option_menu_focus_width);//focusedBG.getBitmap().getWidth();
		drawableHeight = mLauncher.getResources().getDimensionPixelSize(R.dimen.option_menu_focus_height);//focusedBG.getBitmap().getHeight();
		
		DebugHelper.dump("focused bg w="+drawableWidth+" h="+drawableHeight);
		
		Bitmap unfocusBGBitmap = Bitmap.createBitmap(drawableWidth, drawableHeight, Bitmap.Config.ARGB_8888);
		Canvas c = new Canvas(unfocusBGBitmap);
		c.drawARGB(0, 0, 0, 0);
		
		unfocusBG = new BitmapDrawable(mLauncher.getResources(),unfocusBGBitmap);
		
		focusedBG.setBounds(0, 0, drawableWidth, drawableHeight);
		unfocusBG.setBounds(0, 0, drawableWidth, drawableHeight);
		
		/*
		bgStateDrawable = new RTKStateListDrawable();
		
		bgStateDrawable.addState(new int[]{ 
				android.R.attr.state_selected
				},
			focusedBG);
		
		bgStateDrawable.addState(new int[] {-android.R.attr.state_selected}, unfocusBG);
		
		bgStateDrawable.setState(new int[]{ 
				android.R.attr.state_selected,
				-android.R.attr.state_selected
				});
		
		bgStateDrawable.setBounds(0, 0, width, height);
		*/
		
	}
	
	private Drawable getIndicatorDrawable(){
		StateListDrawable bgStateDrawable=new StateListDrawable();
		
		bgStateDrawable.addState(new int[]{ android.R.attr.state_selected}, focusedBG);
		bgStateDrawable.addState(new int[] {-android.R.attr.state_selected}, unfocusBG);
		
		bgStateDrawable.setState(new int[]{ 
				android.R.attr.state_selected,
				-android.R.attr.state_selected
				});
		
		bgStateDrawable.setBounds(0, 0, drawableWidth, drawableHeight);
		//bgStateDrawable.setAlpha((int)(255*0.85));
		return bgStateDrawable;
	}
	
	@Override
	public int getCount() {
		return mContents.length;
	}

	@Override
	public Object getItem(int position) {
		return mContents[position];
	}

	@Override
	public long getItemId(int arg0) {
		// always return 0
		return 0;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		
		//DebugHelper.dump("Adapter getView");
		
		String label = mContents[position].label;
		boolean isEnabled = mContents[position].bIsEnable;
		
		View rst=null;
		
		if(convertView==null){
			View v=inflater.inflate(R.layout.rtk_popup_listview, null);
			Drawable indicator = getIndicatorDrawable();
			TextView tv = (TextView)v.findViewById(R.id.popup_text);
			tv.setText(label);
			tv.setCompoundDrawables(indicator, null, null, null);
			//tv.setCompoundDrawablePadding(10);
			//int padding = tv.getCompoundDrawablePadding();
			//DebugHelper.dump("RTKTextView, padding="+padding);
			
			ViewHolder vh = new ViewHolder();
			vh.textView = tv;
			vh.indicator=indicator;
			vh.opt=mContents[position].tag;
			vh.isEnabled=isEnabled; //itemEnable(vh.opt);
			
			v.setTag(vh);
			//return v;
			rst = v;
			v.setOnClickListener(mClickListener);
		}else{
			ViewHolder vh = (ViewHolder)convertView.getTag();
			vh.textView.setText(label);
			vh.textView.setCompoundDrawables(vh.indicator, null, null, null);
			vh.opt=mContents[position].tag;
			vh.isEnabled=isEnabled; //itemEnable(vh.opt);
			
			//return convertView;
			rst = convertView;
		}
		
		if(!hasEnabledItem()){
			ViewHolder vh = (ViewHolder)rst.getTag();
			// always set unfocused background.
			vh.textView.setCompoundDrawables(unfocusBG, null, null, null);
			
		}
		return rst;
	}
	
	/**
	 * API to check if there is any enabled item
	 * @return
	 */
	private boolean hasEnabledItem(){
		for(RTKPopupWindowItemModel item:mContents){
			if(item.bIsEnable)
				return true;
		}
		return false;
	}
	
	class ViewHolder{
		TextView textView;
		Drawable indicator;
		OPTION_TAG opt;
		boolean isEnabled;
		//ImageView imageView;
	}
}
