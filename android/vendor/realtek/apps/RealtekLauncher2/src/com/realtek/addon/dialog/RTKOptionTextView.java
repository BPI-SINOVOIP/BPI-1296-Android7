package com.realtek.addon.dialog;

import com.realtek.addon.dialog.RTKPopupWindowBaseAdapter.ViewHolder;
import com.realtek.addon.helper.DebugHelper;
import com.realtek.launcher.R;

import android.content.Context;
import android.util.AttributeSet;
import android.view.View;
import android.widget.TextView;

public class RTKOptionTextView extends TextView {

	public RTKOptionTextView(Context context) {
		super(context);
	}
	
	public RTKOptionTextView (Context context, AttributeSet attrs){
		super(context,attrs);
	}
	
	public RTKOptionTextView (Context context, AttributeSet attrs, int defStyle){
		super(context,attrs,defStyle);
	}
	
	protected void drawableStateChanged() {
		DebugHelper.dump("RTKOptionTextView drawableStateChanged");
		
		if(canExecuted()){
			if(isSelected()){
				setTextAppearance(getContext(), R.style.rtk_option_menu_text_focus);
			}else{
				setTextAppearance(getContext(), R.style.rtk_option_menu_text_unfocus);
			}
		}else{
			setTextAppearance(getContext(), R.style.rtk_option_menu_text_disabled);
		}
		
		super.drawableStateChanged();
	}
	
	private boolean canExecuted(){
		View pv = (View)getParent();
		ViewHolder tag = (ViewHolder)pv.getTag();
		if(tag!=null)
			return tag.isEnabled;
		else
			return true;
	}

}
